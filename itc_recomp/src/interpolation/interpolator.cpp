#include "interpolator.h"
#include "blend_state.h"

#include "command_list.h"
#include "draw_tags.h"
#include "guest.h"
#include "transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <chrono>
#include <cmath>
#include <algorithm>
#include <array>
#include <limits>
#include <map>
#include <set>
#include <bit>
#include <cctype>
#include <cstdio>
#include <string>
#include <optional>
#include <unordered_map>
#include <vector>

// Off by default: superseded by re-recording each frame on the main thread
// (src/frame_pacing/frame_loop.cpp).
REXCVAR_DEFINE_BOOL(interpolate, false, "ITC",
                    "Smooth motion between 30 Hz game ticks by blending shader constants");
REXCVAR_DEFINE_BOOL(log_interpolation, false, "ITC",
                    "Log interpolation counts once a second");
REXCVAR_DEFINE_BOOL(interpolate_debug_hide_cycle, false, "ITC",
                    "Debug: from the first race tick, hide each recorder's uploads in "
                    "turn for 5 s, with 5 s gaps (see docs/functions.md)");
REXCVAR_DEFINE_STRING(interpolate_debug_dump, "", "ITC",
                      "Debug: file to write two consecutive ticks' replayed uploads to, "
                      "20 s into the first race");
REXCVAR_DEFINE_BOOL(interpolate_debug_ab_cycle, false, "ITC",
                    "Debug: cycle 5 s each through blending variants A-D (see "
                    "interpolator.cpp)");
REXCVAR_DEFINE_STRING(interpolate_recorders, "", "ITC",
                      "Debug: comma-separated recorder addresses (hex) to interpolate; empty "
                      "means all");

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

// Recorder id for uploads not recorded by a wrapped recorder.
constexpr uint32_t kUntaggedRecorder = 0;

// Every invertible 4x4 matrix in the list is treated as a shared transform
// (in practice almost all are exact copies of one of a dozen or so camera
// matrices, some used by only a few draws), however many copies it has.
constexpr int kMinCameraCopies = 1;

constexpr size_t kNoEarlier = ~size_t(0);

// Smallest |det| / (product of row lengths) for a matrix to count as a
// transform (see IsInvertible).
constexpr double kMinInvertibility = 1e-8;

// How many of the closest previous matrices to test for a rigid match.
constexpr size_t kMaxCameraCandidates = 8;

// Weighs rotation against movement when comparing candidate motions.
constexpr float kMetresPerRadian = 10.0f;

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
    UpdateAbCycle();
    if (!REXCVAR_GET(interpolate) || scene_ == 0) {
      replayed_.clear();
      return;
    }

    // The list replayed now was recorded last tick, so a camera switch
    // between it and the one before shows up one tick late here.
    bool cut_between_replayed_lists = pending_cut_;
    pending_cut_ = camera_cuts_.Update(base);

    std::vector<Snapshot> current = TakeSnapshots(base);
    DumpForAnalysis(base, current);
    if (cut_between_replayed_lists) {
      stats_.cuts++;
      // Remember this tick's cameras, but don't blend across the cut.
      FindCameras(current);
      camera_index_.clear();
      camera_motions_.clear();
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
    if (HiddenByDebugCycle(payload)) {
      std::vector<uint32_t> saved;
      uint32_t floats = debug_hide_sizes_[payload];
      for (uint32_t i = 0; i < floats; i++) {
        saved.push_back(guest::LoadU32(base, payload + 4 * i));
        guest::StoreU32(base, payload + 4 * i, 0);
      }
      upload();
      for (uint32_t i = 0; i < floats; i++) {
        guest::StoreU32(base, payload + 4 * i, saved[i]);
      }
      return;
    }
    auto it = blends_.find(payload);
    if (it == blends_.end() || ab_disabled_) {
      upload();
      return;
    }
    const Blend& blend = it->second;
    const std::vector<float>& to = replayed_[blend.replayed].values;
    for (const auto& [offset, camera] : blend.cameras) {
      Matrix4 m = BlendRigidMotion(camera_motions_[camera], tick_progress_);
      for (size_t k = 0; k < kFloatsPerMatrix; k++) {
        guest::StoreFloat(base, payload + 4 * uint32_t(offset + k), m[k]);
      }
    }
    if (blend.earlier == kNoEarlier) {
      upload();
      for (size_t i = 0; i < to.size(); i++) {
        guest::StoreFloat(base, payload + 4 * uint32_t(i), to[i]);
      }
      stats_.blended_uploads++;
      return;
    }
    const std::vector<float>& from = earlier_values_[blend.earlier];
    for (const auto& [offset, motion] : blend.matrices) {
      Matrix4 m = BlendRigidMotion(motion, tick_progress_);
      for (size_t k = 0; k < kFloatsPerMatrix; k++) {
        guest::StoreFloat(base, payload + 4 * uint32_t(offset + k), m[k]);
      }
    }
    for (size_t offset : blend.positions) {
      for (size_t k = 0; k < 3; k++) {
        float value = from[offset + k] + (to[offset + k] - from[offset + k]) * tick_progress_;
        guest::StoreFloat(base, payload + 4 * uint32_t(offset + k), value);
      }
    }
    // Other values (sprite rectangles, colours, integers) are drawn as
    // recorded for now.
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
    // 4x4 matrices in the upload that moved rigidly between the ticks (by
    // float offset), blended as rigid motions.
    std::vector<std::pair<size_t, RigidMotion>> matrices;
    // Other (x, y, z, 1) position vectors that moved, blended linearly.
    std::vector<size_t> positions;
    // Exact copies of a camera matrix (by float offset), replaced by that
    // camera's blend so everything drawn with it moves together.
    std::vector<std::pair<size_t, size_t>> cameras;  // offset, index into camera_motions_
  };

  using MatrixBits = std::array<uint32_t, kFloatsPerMatrix>;

  static MatrixBits BitsAt(const std::vector<float>& values, size_t start) {
    MatrixBits bits;
    for (size_t k = 0; k < kFloatsPerMatrix; k++) {
      bits[k] = std::bit_cast<uint32_t>(values[start + k]);
    }
    return bits;
  }

  static Matrix4 MatrixFromBits(const MatrixBits& bits) {
    Matrix4 m;
    for (size_t k = 0; k < kFloatsPerMatrix; k++) {
      m[k] = std::bit_cast<float>(bits[k]);
    }
    return m;
  }

  // Camera matrices: 4x4 matrices copied into many uploads this tick (the
  // view-projection of each pass). Each is paired with the previous tick's
  // camera it moved rigidly from.
  void FindCameras(const std::vector<Snapshot>& current) {
    std::map<MatrixBits, int> copies;
    for (const Snapshot& snapshot : current) {
      for (size_t start = 0; start + kFloatsPerMatrix <= snapshot.values.size();
           start += kFloatsPerRegister) {
        copies[BitsAt(snapshot.values, start)]++;
      }
    }
    // Only matrices that changed since last tick need blending, and only
    // invertible ones can be transforms (sprite and lighting data are
    // singular). That leaves a few dozen a tick.
    std::vector<MatrixBits> invertible;  // every candidate transform this tick
    std::vector<MatrixBits> cameras;     // those that changed, to be paired
    for (const auto& [bits, count] : copies) {
      if (count < kMinCameraCopies) {
        continue;
      }
      Matrix4 m = MatrixFromBits(bits);
      bool finite = true;
      for (float v : m) {
        finite &= std::isfinite(v);
      }
      if (!finite || !IsInvertible(m)) {
        continue;
      }
      invertible.push_back(bits);
      if (!previous_all_.contains(bits)) {
        cameras.push_back(bits);
      }
    }
    previous_all_.clear();
    for (const auto& [bits, count] : copies) {
      previous_all_.insert(bits);
    }

    camera_index_.clear();
    camera_motions_.clear();
    std::vector<std::pair<float, size_t>> candidates;
    for (const MatrixBits& camera : cameras) {
      Matrix4 now = MatrixFromBits(camera);
      // Closest previous matrices first; the first that this one moved
      // rigidly from is its counterpart.
      candidates.clear();
      for (size_t p = 0; p < previous_cameras_.size(); p++) {
        Matrix4 before = MatrixFromBits(previous_cameras_[p]);
        float difference = 0;
        for (size_t k = 0; k < kFloatsPerMatrix; k++) {
          difference += std::abs(now[k] - before[k]);
        }
        candidates.push_back({difference, p});
      }
      // Of the closest few that this matrix moved rigidly from, take the one
      // with the smallest motion: sibling views (offset by metres) can be
      // numerically closer than this view's own previous matrix.
      size_t tested = std::min(candidates.size(), kMaxCameraCandidates);
      std::partial_sort(candidates.begin(), candidates.begin() + tested, candidates.end());
      std::optional<RigidMotion> best;
      float best_size = std::numeric_limits<float>::infinity();
      for (size_t c = 0; c < tested; c++) {
        Matrix4 before = MatrixFromBits(previous_cameras_[candidates[c].second]);
        std::optional<RigidMotion> motion = FindRigidMotion(before, now, rigid_options_);
        if (!motion) {
          continue;
        }
        float size = MotionSize(motion->motion);
        if (size < best_size) {
          best = motion;
          best_size = size;
        }
      }
      if (best) {
        camera_index_[camera] = camera_motions_.size();
        camera_motions_.push_back(*best);
      }
    }
    previous_cameras_ = std::move(invertible);
    stats_.cameras = camera_motions_.size();
  }

  // Cheap invertibility test, independent of scale: the determinant divided
  // by the product of the row lengths (Hadamard's bound) is 1 for a
  // perfectly conditioned matrix and 0 for a singular one. Camera matrices
  // with large translations measure 1e-7 to 0.2; sprite and lighting data
  // are singular.
  static bool IsInvertible(const Matrix4& m) {
    double a[16];
    for (int i = 0; i < 16; i++) {
      a[i] = m[i];
    }
    double s0 = a[0] * a[5] - a[4] * a[1], s1 = a[0] * a[6] - a[4] * a[2];
    double s2 = a[0] * a[7] - a[4] * a[3], s3 = a[1] * a[6] - a[5] * a[2];
    double s4 = a[1] * a[7] - a[5] * a[3], s5 = a[2] * a[7] - a[6] * a[3];
    double c5 = a[10] * a[15] - a[14] * a[11], c4 = a[9] * a[15] - a[13] * a[11];
    double c3 = a[9] * a[14] - a[13] * a[10], c2 = a[8] * a[15] - a[12] * a[11];
    double c1 = a[8] * a[14] - a[12] * a[10], c0 = a[8] * a[13] - a[12] * a[9];
    double det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
    double bound = 1;
    for (int row = 0; row < 4; row++) {
      double length = 0;
      for (int k = 0; k < 4; k++) {
        length += a[row * 4 + k] * a[row * 4 + k];
      }
      bound *= std::sqrt(length);
    }
    return bound > 0 && std::abs(det) > kMinInvertibility * bound;
  }

  // How big a rigid motion is: metres moved plus a metre per ~6 degrees.
  static float MotionSize(const Transform& motion) {
    float distance = Distance(motion.position, Vec3{});
    float angle = AngleBetween(motion.rotation, Quaternion{});
    return distance + kMetresPerRadian * angle;
  }

  // Where an upload holds copies of this tick's camera matrices.
  std::vector<std::pair<size_t, size_t>> FindCameraCopies(const std::vector<float>& values) const {
    std::vector<std::pair<size_t, size_t>> found;
    if (camera_index_.empty()) {
      return found;
    }
    for (size_t start = 0; start + kFloatsPerMatrix <= values.size();
         start += kFloatsPerRegister) {
      auto it = camera_index_.find(BitsAt(values, start));
      if (it != camera_index_.end()) {
        found.push_back({start, it->second});
        start += kFloatsPerMatrix - kFloatsPerRegister;
      }
    }
    return found;
  }

  // Finds what in an upload can be blended. Matrices can start at any
  // register (the camera's view-projection often follows a leading vector),
  // so every register offset is tried; matches don't overlap.
  static Blend FindBlendable(const std::vector<float>& a, const std::vector<float>& b,
                             std::vector<bool> used, const RigidMotionOptions& options) {
    Blend blend{};
    for (size_t start = 0; start + kFloatsPerMatrix <= a.size(); start += kFloatsPerRegister) {
      if (used[start]) {
        continue;
      }
      Matrix4 ma, mb;
      for (size_t k = 0; k < kFloatsPerMatrix; k++) {
        ma[k] = a[start + k];
        mb[k] = b[start + k];
      }
      if (ma == mb) {
        continue;
      }
      if (std::optional<RigidMotion> motion = FindRigidMotion(ma, mb, options)) {
        blend.matrices.push_back({start, *motion});
        std::fill(used.begin() + start, used.begin() + start + kFloatsPerMatrix, true);
        start += kFloatsPerMatrix - kFloatsPerRegister;
      }
    }
    for (size_t start = 0; start + kFloatsPerRegister <= a.size(); start += kFloatsPerRegister) {
      if (used[start] || a[start + 3] != 1.0f || b[start + 3] != 1.0f) {
        continue;
      }
      bool finite = true, moved = false;
      for (size_t k = 0; k < 3; k++) {
        finite &= std::isfinite(a[start + k]) && std::isfinite(b[start + k]);
        moved |= a[start + k] != b[start + k];
      }
      if (finite && moved) {
        blend.positions.push_back(start);
      }
    }
    return blend;
  }

  std::vector<Snapshot> TakeSnapshots(uint8_t* base) {
    std::vector<Snapshot> snapshots;
    std::vector<RecordedUpload> replayed = FindReplayedShaderConstantUploads(base, scene_);
    std::unordered_map<uint64_t, uint32_t> untagged_occurrences;
    UpdateDebugHide(replayed);
    for (const RecordedUpload& recorded : replayed) {
      if (!ShouldBlend(recorded.upload)) {
        continue;
      }
      Snapshot snapshot{recorded.payload, recorded.upload.Signature(), std::nullopt, {}};
      snapshot.tag = DrawTags_Find(recorded.payload);
      race_seen_ |= snapshot.tag && snapshot.tag->recorder == 0x825953D0;
      if (!snapshot.tag) {
        // Recorded outside the wrapped recorders: the few global uploads
        // (such as the main camera), recorded in the same order each tick.
        // Identify them by kind and order.
        uint32_t occurrence = untagged_occurrences[snapshot.signature]++;
        snapshot.tag = DrawTagInfo{
            snapshot.signature * 0x9E3779B97F4A7C15ull + occurrence + 1, kUntaggedRecorder};
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
      stats_.uploads_tagged += snapshot.tag && snapshot.tag->recorder != kUntaggedRecorder;
    }
    return snapshots;
  }

  // Pairs each upload in the newly replayed list with the upload that had
  // the same tag in the list replayed before it, if its values changed by a
  // plausible amount for one tick of motion. Everything else is drawn as
  // recorded.
  void Pair(const std::vector<Snapshot>& current) {
    earlier_values_.clear();
    FindCameras(current);
    std::unordered_map<DrawTag, const Snapshot*> earlier_by_tag;
    for (const Snapshot& snapshot : replayed_) {
      if (snapshot.tag) {
        earlier_by_tag[snapshot.tag->tag] = &snapshot;
      }
    }
    for (size_t i = 0; i < current.size(); i++) {
      if (!current[i].tag || !RecorderEnabled(current[i].tag->recorder)) {
        continue;
      }
      std::vector<std::pair<size_t, size_t>> cameras = FindCameraCopies(current[i].values);
      std::vector<bool> used(current[i].values.size(), false);
      for (const auto& [offset, camera] : cameras) {
        std::fill(used.begin() + offset, used.begin() + offset + kFloatsPerMatrix, true);
      }
      if (!cameras.empty()) {
        Blend camera_only{};
        camera_only.earlier = kNoEarlier;
        camera_only.replayed = i;
        camera_only.cameras = cameras;
        blends_[current[i].payload] = std::move(camera_only);
      }
      RecorderStats& recorder_stats = stats_.recorders[current[i].tag->recorder];
      recorder_stats.found++;
      auto it = earlier_by_tag.find(current[i].tag->tag);
      if (it == earlier_by_tag.end()) {
        continue;
      }
      recorder_stats.matched++;
      if (!IsPlausibleMotion(it->second->values, current[i].values)) {
        continue;
      }
      recorder_stats.plausible++;
      Blend blend = FindBlendable(it->second->values, current[i].values, used, rigid_options_);
      if (blend.matrices.empty() && blend.positions.empty()) {
        continue;  // nothing more to blend
      }
      recorder_stats.blended++;
      earlier_values_.push_back(it->second->values);
      blend.earlier = earlier_values_.size() - 1;
      blend.replayed = i;
      blend.cameras = std::move(cameras);
      blends_[current[i].payload] = std::move(blend);
    }
    stats_.unpaired = current.size() - blends_.size();
  }

  static bool RecorderEnabled(uint32_t recorder) {
    const std::string& list = REXCVAR_GET(interpolate_recorders);
    if (list.empty()) {
      return true;
    }
    char name[16];
    std::snprintf(name, sizeof(name), "%08x", recorder);
    std::string lower = list;
    for (char& c : lower) {
      c = char(std::tolower(static_cast<unsigned char>(c)));
    }
    return lower.find(name) != std::string::npos;
  }

  // Debug: writes two consecutive ticks' uploads, 20 s (600 ticks) into the
  // first race, for working out how each is encoded.
  void DumpForAnalysis(uint8_t* base, const std::vector<Snapshot>& current) {
    const std::string& path = REXCVAR_GET(interpolate_debug_dump);
    if (path.empty() || dump_done_) {
      return;
    }
    bool in_race = false;
    for (const Snapshot& snapshot : current) {
      in_race |= snapshot.tag && snapshot.tag->recorder == 0x825953D0;
    }
    if (!in_race || ++dump_race_ticks_ < 600) {
      return;
    }
    std::FILE* out = std::fopen(path.c_str(), dump_race_ticks_ == 600 ? "w" : "a");
    if (!out) {
      dump_done_ = true;
      return;
    }
    std::fprintf(out, "tick %d\ncamera", dump_race_ticks_);
    for (int i = 0; i < 16; i++) {
      std::fprintf(out, " %.6g", guest::LoadFloat(base, kCameraViewMatrixAddress + 4 * i));
    }
    std::fprintf(out, "\n");
    for (const Snapshot& snapshot : current) {
      std::fprintf(out, "upload %08x %016llx %016llx %u", snapshot.tag ? snapshot.tag->recorder : 0,
                   (unsigned long long)snapshot.signature,
                   (unsigned long long)(snapshot.tag ? snapshot.tag->tag : 0), snapshot.payload);
      for (float v : snapshot.values) {
        std::fprintf(out, " %.6g", v);
      }
      std::fprintf(out, "\n");
    }
    std::fclose(out);
    REXLOG_INFO("interpolation: dumped tick {} to {}", dump_race_ticks_, path);
    dump_done_ = dump_race_ticks_ >= 601;
  }
  int dump_race_ticks_ = 0;
  bool dump_done_ = false;

  // Debug A/B cycle: 5 s each of (A) stable solver + eye pivot, (B) stable
  // solver only, (C) the earlier solver, (D) no interpolation.
  void UpdateAbCycle() {
    rigid_options_ = {};
    ab_disabled_ = false;
    if (!REXCVAR_GET(interpolate_debug_ab_cycle)) {
      return;
    }
    if (!ab_started_) {
      if (!race_seen_) {
        return;  // start with the race
      }
      ab_started_ = true;
      ab_start_ = std::chrono::steady_clock::now();
    }
    auto elapsed = std::chrono::steady_clock::now() - ab_start_;
    int phase = int(std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() / 5) % 4;
    rigid_options_.stable_solver = phase <= 1;
    rigid_options_.eye_pivot = phase == 0;
    ab_disabled_ = phase == 3;
    if (phase != ab_logged_phase_) {
      REXLOG_INFO("interpolation: A/B phase {}", "ABCD"[phase]);
      ab_logged_phase_ = phase;
    }
  }
  RigidMotionOptions rigid_options_;
  bool ab_disabled_ = false;
  bool ab_started_ = false;
  bool race_seen_ = false;
  int ab_logged_phase_ = -1;
  std::chrono::steady_clock::time_point ab_start_;

  // Debug hide cycle: which recorder (if any) is hidden right now.
  static constexpr uint32_t kHideCycle[] = {0x825953D0, 0x8209C1A0, 0x82146BD0, 0x820CECA0};
  uint32_t DebugHiddenRecorder() {
    if (!REXCVAR_GET(interpolate_debug_hide_cycle) || !debug_cycle_started_) {
      return 0;
    }
    auto elapsed = std::chrono::steady_clock::now() - debug_cycle_start_;
    int phase = int(std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() / 5) % 8;
    uint32_t hidden = phase % 2 ? kHideCycle[phase / 2] : 0;
    if (hidden != debug_logged_hidden_) {
      REXLOG_INFO("interpolation: debug hiding {:08x}", hidden);
      debug_logged_hidden_ = hidden;
    }
    return hidden;
  }
  bool HiddenByDebugCycle(uint32_t payload) {
    auto it = debug_hide_recorders_.find(payload);
    return it != debug_hide_recorders_.end() && it->second == debug_hidden_now_;
  }
  void UpdateDebugHide(const std::vector<RecordedUpload>& uploads) {
    debug_hide_recorders_.clear();
    debug_hide_sizes_.clear();
    if (!REXCVAR_GET(interpolate_debug_hide_cycle)) {
      return;
    }
    for (const RecordedUpload& recorded : uploads) {
      std::optional<DrawTagInfo> tag = DrawTags_Find(recorded.payload);
      if (!tag) {
        continue;
      }
      if (!debug_cycle_started_ && tag->recorder == 0x825953D0) {
        debug_cycle_started_ = true;  // first race tick
        debug_cycle_start_ = std::chrono::steady_clock::now();
      }
      debug_hide_recorders_[recorded.payload] = tag->recorder;
      debug_hide_sizes_[recorded.payload] = recorded.upload.register_count * kFloatsPerRegister;
    }
    debug_hidden_now_ = DebugHiddenRecorder();
    if (debug_hidden_now_ == 0) {
      debug_hide_recorders_.clear();
    }
  }
  std::unordered_map<uint32_t, uint32_t> debug_hide_recorders_;
  std::unordered_map<uint32_t, uint32_t> debug_hide_sizes_;
  uint32_t debug_hidden_now_ = 0;
  uint32_t debug_logged_hidden_ = 0;
  bool debug_cycle_started_ = false;
  std::chrono::steady_clock::time_point debug_cycle_start_;

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
    REXLOG_INFO("interpolation: {} cameras paired", stats_.cameras);
    REXLOG_INFO(
        "interpolation: {} frames, {} uploads per list ({} tagged), {} paired, {} unpaired, "
        "{} blended uploads, {} cut ticks",
        stats_.frames, stats_.uploads_found, stats_.uploads_tagged, blends_.size(),
        stats_.unpaired, stats_.blended_uploads, stats_.cuts);
    for (const auto& [recorder, r] : stats_.recorders) {
      REXLOG_INFO("interpolation: recorder {:08x}: {} found, {} matched, {} plausible, {} blended",
                  recorder, r.found, r.matched, r.plausible, r.blended);
    }
    stats_ = {};
    last_stats_log_ = now;
  }

  // Per recorder, summed over a second of ticks.
  struct RecorderStats {
    int found = 0;
    int matched = 0;
    int plausible = 0;
    int blended = 0;
  };

  struct Stats {
    std::unordered_map<uint32_t, RecorderStats> recorders;
    int frames = 0;
    size_t uploads_found = 0;
    size_t uploads_tagged = 0;
    size_t cameras = 0;
    size_t unpaired = 0;
    int blended_uploads = 0;
    int cuts = 0;
  };

  uint32_t scene_ = 0;
  CameraCutDetector camera_cuts_;
  bool pending_cut_ = false;

  std::vector<Snapshot> replayed_;                 // the list being replayed
  std::vector<std::vector<float>> earlier_values_;  // counterparts from the list before
  std::unordered_map<uint32_t, Blend> blends_;
  std::vector<MatrixBits> previous_cameras_;
  std::set<MatrixBits> previous_all_;  // every matrix window last tick
  std::map<MatrixBits, size_t> camera_index_;
  std::vector<RigidMotion> camera_motions_;     // by payload address
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
  uint32_t payload = ctx.r3.u32, packed = ctx.r5.u32;
  interpolation::BlendState_ReplayConstants(base, payload, packed, [&] {
    interpolation::GetInterpolator().OnShaderConstants(
        base, payload, [&] { __imp__sub_8258FD48(ctx, base); });
  });
}
