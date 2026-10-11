#include "interpolator.h"

#include "command_list.h"
#include "draw_tags.h"
#include "guest.h"
#include "transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <chrono>
#include <cmath>
#include <limits>
#include <optional>
#include <unordered_map>
#include <vector>

REXCVAR_DEFINE_BOOL(interpolate, true, "ITC",
                    "Smooth motion between 30 Hz game ticks by blending shader constants");
REXCVAR_DEFINE_BOOL(log_interpolation, false, "ITC",
                    "Log interpolation counts once a second");

namespace interpolation {
namespace {

// See "Known data" in docs/functions.md. Used only to spot camera switches.
constexpr uint32_t kCameraViewMatrixAddress = 0x826BAC10;

// A camera switch moves the camera far from where steady motion would put
// it, or turns it sharply, within one tick.
constexpr float kCameraCutDistance = 1.0f;  // metres from the extrapolated position
constexpr float kCameraCutAngle = 0.5f;     // radians, ~30 degrees

constexpr uint32_t kFloatsPerRegister = 4;

// Pairing: how far apart (as a fraction of their size) two uploads' values
// can be and still be the same object a tick later, and how many uploads
// ahead to search when neighbours don't match.
constexpr float kCloseFraction = 0.1f;
constexpr size_t kLookAhead = 8;
constexpr float kSkipPenalty = 0.5f;

Matrix4 LoadMatrix(uint8_t* base, uint32_t address) {
  Matrix4 m;
  for (int i = 0; i < 16; i++) {
    m[i] = guest::LoadFloat(base, address + 4 * i);
  }
  return m;
}

bool ShouldBlend(const ShaderConstantUpload& upload) {
  return upload.stage == ShaderStage::kVertex && upload.type == kFloatConstantType;
}

// Spots camera switches from the camera's view matrix, one tick at a time.
class CameraCutDetector {
 public:
  // Returns whether the camera cut between the last tick and this one.
  bool Update(uint8_t* base) {
    std::optional<Transform> camera =
        TransformFromViewMatrix(LoadMatrix(base, kCameraViewMatrixAddress));
    // Without three valid ticks in a row (menus, the first ticks of a race)
    // there's no evidence of a switch. Lists that change completely, such as
    // entering a race, fail to pair anyway.
    bool cut = false;
    if (camera && current_ && previous_) {
      Vec3 extrapolated;
      for (int k = 0; k < 3; k++) {
        extrapolated[k] = 2 * current_->position[k] - previous_->position[k];
      }
      cut = Distance(extrapolated, camera->position) > kCameraCutDistance ||
            AngleBetween(current_->rotation, camera->rotation) > kCameraCutAngle;
    }
    previous_ = current_;
    current_ = camera;
    return cut;
  }

 private:
  std::optional<Transform> previous_;
  std::optional<Transform> current_;
};

// One replayed upload: where it lives, its values when recorded, and the
// draw that recorded it, if known.
struct Snapshot {
  uint32_t payload;
  uint64_t signature;
  std::optional<DrawTag> tag;
  std::vector<float> values;
};

class Interpolator {
 public:
  void OnSceneDrawn(uint32_t scene) { scene_ = scene; }

  void OnTickDone(uint8_t* base) {
    blends_.clear();
    const DrawTags& tags = DrawTags_TakeReplayedTags();
    if (!REXCVAR_GET(interpolate) || scene_ == 0) {
      replayed_.clear();
      return;
    }

    // The list replayed now was recorded last tick, so a camera switch
    // between it and the one before shows up one tick late here.
    bool cut_between_replayed_lists = pending_cut_;
    pending_cut_ = camera_cuts_.Update(base);

    std::vector<Snapshot> current = TakeSnapshots(base, tags);
    if (cut_between_replayed_lists) {
      stats_.cuts++;
    } else {
      Pair(current);
    }
    replayed_ = std::move(current);
  }

  void BeginFrame(float tick_progress) {
    tick_progress_ = tick_progress;
    stats_.frames++;
    LogStats();
  }

  // Replaces one replayed upload with a blend, runs `upload`, and restores
  // the recorded values.
  template <typename Upload>
  void OnShaderConstants(uint8_t* base, uint32_t payload, Upload&& upload) {
    auto it = blends_.find(payload);
    if (it == blends_.end()) {
      upload();
      return;
    }
    const std::vector<float>& from = earlier_values_[it->second.earlier];
    const std::vector<float>& to = replayed_[it->second.replayed].values;
    for (size_t i = 0; i < to.size(); i++) {
      float value = from[i] + (to[i] - from[i]) * tick_progress_;
      if (std::isfinite(value)) {
        guest::StoreFloat(base, payload + 4 * uint32_t(i), value);
      }
    }
    upload();
    for (size_t i = 0; i < to.size(); i++) {
      guest::StoreFloat(base, payload + 4 * uint32_t(i), to[i]);
    }
    stats_.blended_uploads++;
  }

 private:
  struct Blend {
    size_t earlier;   // index into earlier_values_
    size_t replayed;  // index into replayed_
  };

  std::vector<Snapshot> TakeSnapshots(uint8_t* base, const DrawTags& tags) {
    std::vector<Snapshot> snapshots;
    for (const RecordedUpload& recorded : FindReplayedShaderConstantUploads(base, scene_)) {
      if (!ShouldBlend(recorded.upload)) {
        continue;
      }
      Snapshot snapshot{recorded.payload, recorded.upload.Signature(), std::nullopt, {}};
      if (auto tag = tags.find(recorded.payload); tag != tags.end()) {
        snapshot.tag = tag->second;
      }
      uint32_t floats = recorded.upload.register_count * kFloatsPerRegister;
      snapshot.values.resize(floats);
      for (uint32_t i = 0; i < floats; i++) {
        snapshot.values[i] = guest::LoadFloat(base, recorded.payload + 4 * i);
      }
      snapshots.push_back(std::move(snapshot));
    }
    stats_.uploads_found = snapshots.size();
    stats_.uploads_tagged = 0;
    for (const Snapshot& snapshot : snapshots) {
      stats_.uploads_tagged += snapshot.tag.has_value();
    }
    return snapshots;
  }

  // Pairs each upload in the newly replayed list with its counterpart in the
  // list replayed before it. Uploads with the same signature are aligned in
  // recording order, like a diff: objects come and go between ticks as they
  // enter and leave view, so where neighbours don't match, the closest pair
  // by value within a short look-ahead is taken and the uploads skipped over
  // are left unblended.
  void Pair(const std::vector<Snapshot>& current) {
    earlier_values_.clear();
    stats_.tag_paired = 0;

    // Uploads tagged by the draw that recorded them pair exactly.
    std::unordered_map<DrawTag, const Snapshot*> earlier_by_tag;
    for (const Snapshot& snapshot : replayed_) {
      if (snapshot.tag) {
        earlier_by_tag[*snapshot.tag] = &snapshot;
      }
    }
    for (size_t i = 0; i < current.size(); i++) {
      if (!current[i].tag) {
        continue;
      }
      auto it = earlier_by_tag.find(*current[i].tag);
      if (it == earlier_by_tag.end()) {
        continue;
      }
      earlier_values_.push_back(it->second->values);
      blends_[current[i].payload] = {earlier_values_.size() - 1, i};
      stats_.tag_paired++;
    }

    // The rest pair by value, in recording order.
    std::unordered_map<uint64_t, std::vector<const Snapshot*>> earlier_by_signature;
    std::unordered_map<uint64_t, std::vector<size_t>> current_by_signature;
    for (const Snapshot& snapshot : replayed_) {
      if (!snapshot.tag) {
        earlier_by_signature[snapshot.signature].push_back(&snapshot);
      }
    }
    for (size_t i = 0; i < current.size(); i++) {
      if (!current[i].tag) {
        current_by_signature[current[i].signature].push_back(i);
      }
    }

    for (const auto& [signature, now_indices] : current_by_signature) {
      auto it = earlier_by_signature.find(signature);
      if (it == earlier_by_signature.end()) {
        continue;
      }
      std::vector<const Snapshot*> now;
      for (size_t index : now_indices) {
        now.push_back(&current[index]);
      }
      const std::vector<const Snapshot*>& before = it->second;

      size_t i = 0, j = 0;
      while (i < now.size() && j < before.size()) {
        size_t di = 0, dj = 0;
        if (!FindBestPairAhead(now, i, before, j, di, dj)) {
          i++;  // nothing nearby matches this upload
          continue;
        }
        i += di;
        j += dj;
        earlier_values_.push_back(before[j]->values);
        blends_[now[i]->payload] = {earlier_values_.size() - 1, now_indices[i]};
        i++;
        j++;
      }
    }
    stats_.unpaired = current.size() - blends_.size();
  }

  static float Difference(const std::vector<float>& a, const std::vector<float>& b) {
    float sum = 0;
    for (size_t k = 0; k < a.size(); k++) {
      sum += std::abs(a[k] - b[k]);
    }
    return sum;
  }

  static float Magnitude(const std::vector<float>& a) {
    float sum = 0;
    for (float v : a) {
      sum += std::abs(v);
    }
    return sum;
  }

  // The same object one tick apart: values differ by a small fraction.
  static bool IsClose(const std::vector<float>& a, const std::vector<float>& b) {
    float difference = Difference(a, b);
    return std::isfinite(difference) &&
           difference <= kCloseFraction * (Magnitude(a) + Magnitude(b)) + 1e-6f;
  }

  // Searches up to kLookAhead uploads ahead in both lists for the pair most
  // likely to be the same object: the smallest absolute difference in values,
  // plus a small penalty per upload skipped. Absolute, so an object that
  // hasn't moved always beats a similar neighbour (a row of lamp posts).
  // Returns its offsets in `di`, `dj`.
  static bool FindBestPairAhead(const std::vector<const Snapshot*>& now, size_t i,
                               const std::vector<const Snapshot*>& before, size_t j,
                               size_t& di, size_t& dj) {
    float best = std::numeric_limits<float>::infinity();
    for (size_t a = 0; a < kLookAhead && i + a < now.size(); a++) {
      for (size_t b = 0; b < kLookAhead && j + b < before.size(); b++) {
        const std::vector<float>& x = now[i + a]->values;
        const std::vector<float>& y = before[j + b]->values;
        if (!IsClose(x, y)) {
          continue;
        }
        float score = Difference(x, y) + kSkipPenalty * float(a + b);
        if (score < best) {
          best = score;
          di = a;
          dj = b;
        }
      }
    }
    return std::isfinite(best);
  }

  void LogStats() {
    if (!REXCVAR_GET(log_interpolation)) {
      return;
    }
    auto now = std::chrono::steady_clock::now();
    if (now - last_stats_log_ < std::chrono::seconds(1)) {
      return;
    }
    REXLOG_INFO(
        "interpolation: {} frames, {} uploads per list ({} tagged), {} paired ({} by tag), "
        "{} unpaired, {} blended uploads, {} cut ticks",
        stats_.frames, stats_.uploads_found, stats_.uploads_tagged, blends_.size(),
        stats_.tag_paired, stats_.unpaired, stats_.blended_uploads, stats_.cuts);
    stats_ = {};
    last_stats_log_ = now;
  }

  struct Stats {
    int frames = 0;
    size_t uploads_found = 0;
    size_t uploads_tagged = 0;
    size_t tag_paired = 0;
    size_t unpaired = 0;
    int blended_uploads = 0;
    int cuts = 0;
  };

  uint32_t scene_ = 0;
  CameraCutDetector camera_cuts_;
  bool pending_cut_ = false;

  std::vector<Snapshot> replayed_;                 // the list being replayed
  std::vector<std::vector<float>> earlier_values_;  // counterparts from the list before
  std::unordered_map<uint32_t, Blend> blends_;     // by payload address
  float tick_progress_ = 1;

  Stats stats_;
  std::chrono::steady_clock::time_point last_stats_log_{};
};

}  // namespace

Interpolator& GetInterpolator() {
  static Interpolator interpolator;
  return interpolator;
}

void Interpolation_OnTickDone(uint8_t* base) {
  GetInterpolator().OnTickDone(base);
}

void Interpolation_BeginFrame(float tick_progress) {
  GetInterpolator().BeginFrame(tick_progress);
}

}  // namespace interpolation

// Guest entry points. These keep the generated names so the game's calls and
// the function table reach them; they only unpack registers and forward.

// Scene::Draw: r3 = scene. Noted so the interpolator can find the command list.
extern "C" REX_FUNC(sub_82098940) {
  REX_FUNC_PROLOGUE();
  interpolation::GetInterpolator().OnSceneDrawn(ctx.r3.u32);
  __imp__sub_82098940(ctx, base);
}

// Command 7, set shader constants: r3 = payload, r4 = register count,
// r5 = packed stage/type/start/count. Returns r3 = payload slots used.
extern "C" REX_FUNC(sub_8258FD48) {
  REX_FUNC_PROLOGUE();
  interpolation::GetInterpolator().OnShaderConstants(
      base, ctx.r3.u32, [&] { __imp__sub_8258FD48(ctx, base); });
}
