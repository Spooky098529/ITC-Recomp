// Game logic prepares the matrices the draw step reads, at stable addresses:
// each camera pass's view-projection (+0x90 of the 21 pass objects at
// 0x826BAC00), and a render-state block (0x70180000-0x70190000) holding
// copies of those and per-object transforms such as the cars' body and
// wheels. Other object matrices are handed to the per-object recorder
// (sub_825953D0, r4 and r5).
//
// After each tick, every such 4x4 matrix that moved rigidly since the
// previous tick is paired with itself by address (the same object every
// tick), and before each Draw it's replaced with a blend of the two (with
// the eye-pivot maths in transform.h), then restored. Copies of a matrix
// get identical blends, so everything drawn with a camera stays together.

#include "blend_state.h"

#include "frame_pacing/frame_pacer.h"
#include "guest.h"
#include "transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <string>
#include <mutex>
#include <optional>
#include <set>
#include <unordered_map>
#include <vector>

REXCVAR_DEFINE_BOOL(interpolate_draw_state, true, "ITC",
                    "Smooth motion between 30 Hz ticks in races by blending the matrices "
                    "the draw step reads");
REXCVAR_DEFINE_BOOL(log_blend_state, false, "ITC",
                    "Log how many matrices were blended, once a second");
REXCVAR_DEFINE_BOOL(log_blend_detail, false, "ITC",
                    "Log why candidate matrices were or weren't blended, once a second");

namespace interpolation {
namespace {

struct Region {
  uint32_t start;
  uint32_t end;
};

constexpr Region kCameraPasses = {0x826BAC00, 0x826BAC00 + 21 * 592};
constexpr Region kRenderStateBlock = {0x70180000, 0x70190000};
constexpr uint32_t kMatrixBytes = 64;
constexpr uint32_t kRowBytes = 16;

// A matrix moving further than this in one tick is a cut (a camera switch,
// a reset), drawn without blending.
constexpr float kCutDistance = 4.0f;  // metres
constexpr float kCutAngle = 0.5f;     // radians, ~30 degrees

Matrix4 LoadMatrix(uint8_t* base, uint32_t address) {
  Matrix4 m;
  for (int i = 0; i < 16; i++) {
    m[i] = guest::LoadFloat(base, address + 4 * i);
  }
  return m;
}

void StoreMatrix(uint8_t* base, uint32_t address, const Matrix4& m) {
  for (int i = 0; i < 16; i++) {
    guest::StoreFloat(base, address + 4 * i, m[i]);
  }
}

bool IsFinite(const Matrix4& m) {
  return std::all_of(m.begin(), m.end(), [](float v) { return std::isfinite(v); });
}

// How far a rigid motion moves its subject: the eye for a perspective
// matrix (rotating about a pivot), otherwise the translation.
float MovedDistance(const RigidMotion& motion) {
  if (!motion.pivot) {
    return Distance(motion.motion.position, Vec3{});
  }
  Matrix4 full = WorldMatrixFromTransform(motion.motion);
  const Vec3& p = *motion.pivot;
  Vec3 q;
  for (int k = 0; k < 3; k++) {
    q[k] = p[0] * full[k] + p[1] * full[4 + k] + p[2] * full[8 + k] + full[12 + k];
  }
  return Distance(p, q);
}

enum class Outcome { kNew, kUnchanged, kNotFinite, kNotRigid, kCut, kBlended, kInside, kCount };
constexpr const char* kOutcomeNames[] = {"new", "unchanged", "nonfinite", "notrigid",
                                         "cut",  "blended",   "inside"};
enum RegionIndex { kPasses, kBlock, kRecorder, kRegionCount };
constexpr const char* kRegionNames[] = {"passes", "block", "recorder"};
constexpr char kOutcomeLetters[] = "nu!rcbi";

std::string FormatMatrix(const Matrix4& m) {
  std::string s;
  for (int i = 0; i < 16; i++) {
    s += fmt::format("{}{:.4g}", i == 0 ? "" : (i % 4 == 0 ? " | " : " "), m[i]);
  }
  return s;
}

class DrawState {
 public:
  // Main thread, from the per-object recorder: a matrix it was handed.
  void OnMatrixUsed(uint32_t address) {
    std::lock_guard lock(mutex_);
    used_this_tick_.insert(address);
  }

  // Main thread, before the Draw of a logic pass: this tick's matrices.
  void OnTick(uint8_t* base) {
    std::vector<uint32_t> candidates;
    for (const Region& region : {kCameraPasses, kRenderStateBlock}) {
      for (uint32_t a = region.start; a + kMatrixBytes <= region.end; a += kRowBytes) {
        candidates.push_back(a);
      }
    }
    {
      std::lock_guard lock(mutex_);
      for (uint32_t a : used_last_tick_) {
        if (!InRegion(a)) {
          candidates.push_back(a);
        }
      }
      used_last_tick_ = std::move(used_this_tick_);
      used_this_tick_.clear();
    }

    std::unordered_map<uint32_t, Matrix4> current;
    current.reserve(candidates.size());
    blends_.clear();
    uint32_t taken_start = 0;  // the last matrix taken, whose rows aren't matrices
    std::string pass_outcomes(21, '-');
    for (uint32_t address : candidates) {
      Matrix4 now = LoadMatrix(base, address);
      current[address] = now;
      Outcome outcome = Classify(address, now, taken_start);
      int region = RegionOf(address);
      detail_.counts[region][int(outcome)]++;
      if (region == kPasses && (address - kCameraPasses.start) % 592 == 0x90) {
        pass_outcomes[(address - kCameraPasses.start) / 592] = kOutcomeLetters[int(outcome)];
      }
    }
    detail_.last_pass_outcomes = pass_outcomes;
    detail_.ticks++;
    previous_ = std::move(current);
  }

  Outcome Classify(uint32_t address, const Matrix4& now, uint32_t& taken_start) {
    if (taken_start != 0 && address > taken_start && address < taken_start + kMatrixBytes) {
      return Outcome::kInside;  // inside a matrix already taken
    }
    auto before = previous_.find(address);
    if (before == previous_.end()) {
      return Outcome::kNew;
    }
    if (before->second == now) {
      return Outcome::kUnchanged;
    }
    if (!IsFinite(now) || !IsFinite(before->second)) {
      return Outcome::kNotFinite;
    }
    std::optional<RigidMotion> motion = FindRigidMotion(before->second, now);
    bool is_pass_vp = InRange(kCameraPasses, address) &&
                      (address - kCameraPasses.start) % 592 == 0x90;
    if (!motion) {
      if (is_pass_vp && detail_.not_rigid_sample.empty()) {
        detail_.not_rigid_sample = fmt::format("{:08X}\n  before {}\n  now    {}", address,
                                               FormatMatrix(before->second), FormatMatrix(now));
      }
      return Outcome::kNotRigid;
    }
    float moved = MovedDistance(*motion);
    float angle = AngleBetween(motion->motion.rotation, Quaternion{});
    if (moved > kCutDistance || angle > kCutAngle) {
      counts_.cuts++;
      if (is_pass_vp && detail_.cut_sample.empty()) {
        detail_.cut_sample = fmt::format("{:08X} moved {:.3f} angle {:.3f}", address, moved, angle);
      }
      return Outcome::kCut;
    }
    if (is_pass_vp) {
      detail_.max_pass_moved = std::max(detail_.max_pass_moved, moved);
      detail_.max_pass_angle = std::max(detail_.max_pass_angle, angle);
    }
    blends_.push_back({address, *motion});
    taken_start = address;
    return Outcome::kBlended;
  }

  void BeforeDraw(uint8_t* base) {
    saved_.clear();
    if (!REXCVAR_GET(interpolate_draw_state)) {
      return;
    }
    // Drawing runs a tick behind: the logic pass shows the previous tick
    // (amount 0) and the passes after it move towards the current one.
    float amount = frame_pacing::GetFramePacer().TickProgress();
    for (const auto& [address, motion] : blends_) {
      saved_.push_back({address, LoadMatrix(base, address)});
      StoreMatrix(base, address, BlendRigidMotion(motion, amount));
    }
    counts_.draws++;
    counts_.blended += int(blends_.size());
    LogCounts();
  }

  void AfterDraw(uint8_t* base) {
    for (auto it = saved_.rbegin(); it != saved_.rend(); ++it) {
      StoreMatrix(base, it->first, it->second);
    }
    saved_.clear();
  }

 private:
  static bool InRange(const Region& region, uint32_t address) {
    return address >= region.start && address < region.end;
  }

  static int RegionOf(uint32_t address) {
    if (InRange(kCameraPasses, address)) return kPasses;
    if (InRange(kRenderStateBlock, address)) return kBlock;
    return kRecorder;
  }

  static bool InRegion(uint32_t address) {
    for (const Region& region : {kCameraPasses, kRenderStateBlock}) {
      if (address >= region.start && address < region.end) {
        return true;
      }
    }
    return false;
  }

  void LogCounts() {
    if (!REXCVAR_GET(log_blend_state)) {
      return;
    }
    auto now = std::chrono::steady_clock::now();
    if (now - counts_.last_log < std::chrono::seconds(1)) {
      return;
    }
    REXLOG_INFO("blend state: {} draws, {} matrices blended per draw, {} cuts", counts_.draws,
                counts_.draws ? counts_.blended / counts_.draws : 0, counts_.cuts);
    counts_ = {};
    counts_.last_log = now;
    LogDetail();
  }

  void LogDetail() {
    if (!REXCVAR_GET(log_blend_detail)) {
      detail_ = {};
      return;
    }
    REXLOG_INFO("blend detail: {} ticks, last tick's pass VPs [{}] (letters: {})", detail_.ticks,
                detail_.last_pass_outcomes, kOutcomeLetters);
    for (int r = 0; r < kRegionCount; r++) {
      std::string line;
      for (int o = 0; o < int(Outcome::kCount); o++) {
        line += fmt::format(" {}={}", kOutcomeNames[o], detail_.counts[r][o]);
      }
      REXLOG_INFO("blend detail: {:8}{}", kRegionNames[r], line);
    }
    REXLOG_INFO("blend detail: pass VP max moved {:.3f} max angle {:.3f}", detail_.max_pass_moved,
                detail_.max_pass_angle);
    if (!detail_.cut_sample.empty()) {
      REXLOG_INFO("blend detail: cut {}", detail_.cut_sample);
    }
    if (!detail_.not_rigid_sample.empty()) {
      REXLOG_INFO("blend detail: not rigid {}", detail_.not_rigid_sample);
    }
    detail_ = {};
  }

  std::mutex mutex_;
  std::set<uint32_t> used_this_tick_;
  std::set<uint32_t> used_last_tick_;

  std::unordered_map<uint32_t, Matrix4> previous_;
  std::vector<std::pair<uint32_t, RigidMotion>> blends_;
  std::vector<std::pair<uint32_t, Matrix4>> saved_;

  struct Counts {
    int draws = 0;
    int blended = 0;
    int cuts = 0;
    std::chrono::steady_clock::time_point last_log{};
  } counts_;

  struct Detail {
    int ticks = 0;
    std::array<std::array<int, int(Outcome::kCount)>, kRegionCount> counts{};
    std::string last_pass_outcomes;
    std::string not_rigid_sample;
    std::string cut_sample;
    float max_pass_moved = 0;
    float max_pass_angle = 0;
  } detail_;
};

DrawState& GetDrawState() {
  static DrawState state;
  return state;
}

}  // namespace

void BlendState_OnVehicleSeen(uint32_t) {}

void BlendState_OnMatrixUsed(uint32_t address) {
  GetDrawState().OnMatrixUsed(address);
}

void BlendState_BeforeDraw(uint8_t* base, bool render_only_pass) {
  if (!render_only_pass) {
    GetDrawState().OnTick(base);
  }
  GetDrawState().BeforeDraw(base);
}

void BlendState_AfterDraw(uint8_t* base) {
  GetDrawState().AfterDraw(base);
}

}  // namespace interpolation
