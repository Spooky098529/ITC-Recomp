#include "interpolator.h"

#include "command_list.h"
#include "draw_tags.h"
#include "guest.h"
#include "transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <chrono>
#include <cmath>
#include <bit>
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
constexpr size_t kFloatsPerMatrix = 16;

// How much (as a fraction of their size) an upload's values can change in one
// tick and still be blended. Bigger jumps are teleports or wrong pairs.
constexpr float kMaxRelativeChangePerTick = 0.2f;

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
  std::optional<DrawTagInfo> tag;
  std::vector<float> values;
};

class Interpolator {
 public:
  void OnSceneDrawn(uint32_t scene) { scene_ = scene; }

  void OnTickDone(uint8_t* base) {
    blends_.clear();
    if (!REXCVAR_GET(interpolate) || scene_ == 0) {
      replayed_.clear();
      return;
    }

    // The list replayed now was recorded last tick, so a camera switch
    // between it and the one before shows up one tick late here.
    bool cut_between_replayed_lists = pending_cut_;
    pending_cut_ = camera_cuts_.Update(base);

    std::vector<Snapshot> current = TakeSnapshots(base);
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
    const Blend& blend = it->second;
    const std::vector<float>& from = earlier_values_[blend.earlier];
    const std::vector<float>& to = replayed_[blend.replayed].values;
    for (size_t i = 0; i < to.size(); i++) {
      size_t block = i / kFloatsPerMatrix;
      if (block < blend.rigid_blocks.size() && blend.rigid_blocks[block]) {
        if (i % kFloatsPerMatrix == 0) {
          Matrix4 m = BlendRigidMotion(*blend.rigid_blocks[block], tick_progress_);
          for (size_t k = 0; k < kFloatsPerMatrix; k++) {
            guest::StoreFloat(base, payload + 4 * uint32_t(i + k), m[k]);
          }
        }
        continue;
      }
      // Values that aren't part of a rigidly moving matrix (sprite
      // rectangles, colours, integers) are drawn as recorded for now.
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
    // For each whole 4x4 block of the upload, its rigid motion between the
    // ticks if it has one (a transform). Only those blocks are blended.
    std::vector<std::optional<RigidMotion>> rigid_blocks;
  };

  static std::vector<std::optional<RigidMotion>> FindRigidBlocks(const std::vector<float>& a,
                                                                 const std::vector<float>& b) {
    std::vector<std::optional<RigidMotion>> blocks;
    for (size_t start = 0; start + kFloatsPerMatrix <= a.size(); start += kFloatsPerMatrix) {
      Matrix4 ma, mb;
      for (size_t k = 0; k < kFloatsPerMatrix; k++) {
        ma[k] = a[start + k];
        mb[k] = b[start + k];
      }
      bool same = ma == mb;
      blocks.push_back(same ? std::nullopt : FindRigidMotion(ma, mb));
    }
    return blocks;
  }

  std::vector<Snapshot> TakeSnapshots(uint8_t* base) {
    std::vector<Snapshot> snapshots;
    for (const RecordedUpload& recorded : FindReplayedShaderConstantUploads(base, scene_)) {
      if (!ShouldBlend(recorded.upload)) {
        continue;
      }
      Snapshot snapshot{recorded.payload, recorded.upload.Signature(), std::nullopt, {}};
      snapshot.tag = DrawTags_Find(recorded.payload);
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

  // Pairs each upload in the newly replayed list with the upload that had
  // the same tag in the list replayed before it, if its values changed by a
  // plausible amount for one tick of motion. Everything else is drawn as
  // recorded.
  void Pair(const std::vector<Snapshot>& current) {
    earlier_values_.clear();
    std::unordered_map<DrawTag, const Snapshot*> earlier_by_tag;
    for (const Snapshot& snapshot : replayed_) {
      if (snapshot.tag) {
        earlier_by_tag[snapshot.tag->tag] = &snapshot;
      }
    }
    for (size_t i = 0; i < current.size(); i++) {
      if (!current[i].tag) {
        continue;
      }
      auto it = earlier_by_tag.find(current[i].tag->tag);
      if (it == earlier_by_tag.end() ||
          !IsPlausibleMotion(it->second->values, current[i].values)) {
        continue;
      }
      std::vector<std::optional<RigidMotion>> rigid_blocks =
          FindRigidBlocks(it->second->values, current[i].values);
      bool any_rigid = false;
      for (const auto& block : rigid_blocks) {
        any_rigid |= block.has_value();
      }
      if (!any_rigid) {
        continue;  // nothing to blend
      }
      earlier_values_.push_back(it->second->values);
      blends_[current[i].payload] = {earlier_values_.size() - 1, i, std::move(rigid_blocks)};
    }
    stats_.unpaired = current.size() - blends_.size();
  }

  // Values that can't be blended (integers read as NaN) must match exactly;
  // the rest must change by under kMaxRelativeChangePerTick.
  static bool IsPlausibleMotion(const std::vector<float>& a, const std::vector<float>& b) {
    if (a.size() != b.size()) {
      return false;
    }
    float difference = 0, magnitude = 0;
    for (size_t k = 0; k < a.size(); k++) {
      bool finite_a = std::isfinite(a[k]), finite_b = std::isfinite(b[k]);
      if (!finite_a || !finite_b) {
        if (std::bit_cast<uint32_t>(a[k]) != std::bit_cast<uint32_t>(b[k])) {
          return false;
        }
        continue;
      }
      difference += std::abs(a[k] - b[k]);
      magnitude += std::abs(a[k]) + std::abs(b[k]);
    }
    return difference <= kMaxRelativeChangePerTick * magnitude;
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
        "interpolation: {} frames, {} uploads per list ({} tagged), {} paired, {} unpaired, "
        "{} blended uploads, {} cut ticks",
        stats_.frames, stats_.uploads_found, stats_.uploads_tagged, blends_.size(),
        stats_.unpaired, stats_.blended_uploads, stats_.cuts);
    stats_ = {};
    last_stats_log_ = now;
  }

  struct Stats {
    int frames = 0;
    size_t uploads_found = 0;
    size_t uploads_tagged = 0;
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
