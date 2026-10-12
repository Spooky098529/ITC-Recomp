// What the draw step reads (measured in a race, --dump_draw_state):
//
// - Camera passes: 21 objects of 592 bytes at 0x826BAC00, each with a view
//   matrix at +0x10, a projection at +0x50 and their product (view *
//   projection) at +0x90. Pass 0 is the main 1280x720 camera; game logic
//   sets it during Update. Some passes (the car reflection and shadow maps)
//   are recomputed inside Draw from other state.
// - Objects: Draw works out each object's world matrix (often into a
//   temporary on its stack) and hands it to the per-object recorder
//   (sub_825953D0) with the camera's view-projection; the recorder copies
//   world * view-projection and world into the command list.
//
// So, the usual engine interpolation: before Draw, each camera whose
// view-projection is view * projection is moved to a blend of its last two
// ticks' positions and rotations, and its view and view-projection are
// rebuilt. In the recorder, each object's world matrix is blended with its
// previous tick's, matched by address and by how many times that address has
// been recorded in this Draw (the draw step visits objects in the same order
// every tick). Everything is put back afterwards.
//
// Drawing runs a tick behind: the logic pass draws the previous tick (amount
// 0) and the passes after it move towards the current one.

#include "blend_state.h"

#include "frame_pacing/frame_pacer.h"
#include "frame_pacing/game_messages.h"
#include "guest.h"
#include "transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <chrono>
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

REXCVAR_DEFINE_BOOL(interpolate_draw_state, true, "ITC",
                    "Smooth motion between 30 Hz ticks in races by blending cameras and "
                    "object transforms");
REXCVAR_DEFINE_DOUBLE(debug_camera_constant_lift, 0.0, "ITC",
                      "Testing: draw the camera constants recorded during Update from this many "
                      "metres higher, to show what they affect");
REXCVAR_DEFINE_DOUBLE(debug_camera_lift, 0.0, "ITC",
                      "Testing: draw with the blended main camera (pass 0) raised this many "
                      "metres, to show what it affects");
REXCVAR_DEFINE_BOOL(log_replayed_constants, false, "ITC",
                    "Log the vertex constant matrices the render thread replays, once a second");
REXCVAR_DEFINE_BOOL(log_blend_state, false, "ITC",
                    "Log how many cameras and objects were blended, once a second");
REXCVAR_DEFINE_BOOL(log_camera_products, false, "ITC",
                    "Log matrix multiplies that read a camera's view-projection, once a second");
REXCVAR_DEFINE_STRING(dump_draw_state, "", "ITC",
                      "Folder to dump the camera passes and the draw step's stack to, before "
                      "and after Draw, for 60 race ticks starting 20 s into the first race");

namespace interpolation {
namespace {

constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr int kCameraPassCount = 21;
constexpr uint32_t kCameraPassBytes = 592;
constexpr uint32_t kViewOffset = 0x10;
constexpr uint32_t kProjectionOffset = 0x50;
constexpr uint32_t kViewProjectionOffset = 0x90;
// The camera Draw rebuilds the view from (sub_820A3978, once per use):
// eye position, look-at target and up vector, 3 floats each.
constexpr uint32_t kEyeOffset = 0x190;
constexpr uint32_t kTargetOffset = 0x1A0;
constexpr uint32_t kUpOffset = 0x1B0;
constexpr uint32_t kLookAtOffsets[] = {kEyeOffset, kTargetOffset, kUpOffset};

// The command list recorders write into (see "Command list format" in
// docs/functions.md): its two buffers, and the slots used (16 bytes each).
constexpr uint32_t kCommandListPointer = 0x8296B918;
constexpr uint32_t kCommandListCapacityOffset = 8;
constexpr uint32_t kCommandListUsedOffset = 12;
constexpr uint32_t kCommandListPreviousUsedOffset = 16;
constexpr uint32_t kCommandListBufferOffsets[] = {24, 28};
constexpr uint32_t kCommandSlotBytes = 16;
constexpr uint32_t kSetConstantsCommand = 7;

// Where the main thread's stack is during Draw (dumped for analysis).
constexpr uint32_t kDrawStack = 0x70180000;
constexpr uint32_t kDrawStackBytes = 0x10000;

// Moving further than this in one tick is a cut (a camera switch, a reset):
// drawn without blending.
constexpr float kCutDistance = 8.0f;  // metres
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

Vec3 LoadVec3(uint8_t* base, uint32_t address) {
  return {guest::LoadFloat(base, address), guest::LoadFloat(base, address + 4),
          guest::LoadFloat(base, address + 8)};
}

void StoreVec3(uint8_t* base, uint32_t address, const Vec3& v) {
  for (int k = 0; k < 3; k++) {
    guest::StoreFloat(base, address + 4 * k, v[k]);
  }
}

bool IsCut(const Transform& a, const Transform& b) {
  return Distance(a.position, b.position) > kCutDistance ||
         AngleBetween(a.rotation, b.rotation) > kCutAngle;
}

// Whether `product` is `a * b`, to float precision.
bool IsProduct(const Matrix4& product, const Matrix4& a, const Matrix4& b) {
  Matrix4 expected = Multiply(a, b);
  for (int i = 0; i < 16; i++) {
    float tolerance = 1e-3f * std::max(1.0f, std::abs(expected[i]));
    if (!(std::abs(product[i] - expected[i]) <= tolerance)) {
      return false;
    }
  }
  return true;
}

uint32_t CameraPass(int pass) {
  return kCameraPasses + pass * kCameraPassBytes;
}

// Writes the camera passes and the draw step's stack to
// <folder>/tick<N>_<when>.bin as (u32 address, u32 size, bytes) records,
// little endian, with the bytes as in guest memory (big endian).
class DrawStateDump {
 public:
  // Logs every place in guest memory holding a copy of pass 0's camera:
  // the start of its view, view-projection (and their transposes), or its
  // eye position, from this tick and the previous one.
  void ScanForCameraCopies(uint8_t* base, const char* when) {
    struct Pattern {
      std::string name;
      std::array<uint32_t, 3> words;  // as stored in guest memory
    };
    std::vector<Pattern> patterns;
    auto add = [&](const std::string& name, float a, float b, float c) {
      Pattern p{name, {}};
      float values[3] = {a, b, c};
      for (int i = 0; i < 3; i++) {
        uint32_t bits;
        std::memcpy(&bits, &values[i], 4);
        p.words[i] = __builtin_bswap32(bits);
      }
      patterns.push_back(p);
    };
    for (int tick = 0; tick < 2; tick++) {
      const Matrix4& view = tick == 0 ? view_ : previous_view_;
      const Matrix4& vp = tick == 0 ? view_projection_ : previous_view_projection_;
      std::string t = tick == 0 ? "" : "(prev)";
      add("V" + t, view[0], view[1], view[2]);
      add("Vt" + t, view[0], view[4], view[8]);
      add("VP" + t, vp[0], vp[1], vp[2]);
      add("VPt" + t, vp[0], vp[4], vp[8]);
      add("VPt row3" + t, vp[3], vp[7], vp[11]);
      if (std::optional<Transform> eye = TransformFromViewMatrix(view)) {
        add("eye" + t, eye->position[0], eye->position[1], eye->position[2]);
      }
    }
    int found = 0;
    std::ofstream copies(fmt::format("{}/copies_{}.bin", std::string(REXCVAR_GET(dump_draw_state)), when),
                         std::ios::binary);
    std::ifstream maps("/proc/self/maps");
    std::string line;
    uint64_t guest_start = reinterpret_cast<uint64_t>(base);
    uint64_t guest_end = guest_start + 0x100000000ull;
    while (std::getline(maps, line) && found < 400) {
      uint64_t start = std::stoull(line.substr(0, line.find('-')), nullptr, 16);
      uint64_t end = std::stoull(line.substr(line.find('-') + 1), nullptr, 16);
      std::string perms = line.substr(line.find(' ') + 1, 4);
      if (perms[0] != 'r' || end <= guest_start || start >= guest_end) {
        continue;
      }
      start = std::max(start, guest_start);
      end = std::min(end, guest_end);
      for (uint64_t at = start; at + 12 <= end && found < 400; at += 4) {
        const uint32_t* words = reinterpret_cast<const uint32_t*>(at);
        for (const Pattern& p : patterns) {
          if (words[0] == p.words[0] && words[1] == p.words[1] && words[2] == p.words[2]) {
            uint32_t address = uint32_t(at - guest_start);
            REXLOG_INFO("camera copy scan ({}): {} at {:08X}", when, p.name, address);
            found++;
            // Its surroundings, as a dump record (see OnDraw).
            uint32_t from = address - 0x200, size = 0x400;
            if (at - 0x200 >= start && at + 0x200 <= end) {
              copies.write(reinterpret_cast<const char*>(&from), 4);
              copies.write(reinterpret_cast<const char*>(&size), 4);
              copies.write(reinterpret_cast<const char*>(base + from), size);
            }
          }
        }
      }
    }
    REXLOG_INFO("camera copy scan ({}): {} found", when, found);
  }

  void OnTick(uint8_t* base) {
    previous_view_ = view_;
    previous_view_projection_ = view_projection_;
    view_ = LoadMatrix(base, kCameraPasses + kViewOffset);
    view_projection_ = LoadMatrix(base, kCameraPasses + kViewProjectionOffset);
  }

  void OnDraw(uint8_t* base, bool logic_pass, const char* when) {
    std::string folder = REXCVAR_GET(dump_draw_state);
    if (folder.empty() || !frame_pacing::GameMessages_InRace() || !logic_pass) {
      return;
    }
    if (std::string(when) == "pre") {
      race_ticks_++;
    }
    int first = 20 * 30;
    if (race_ticks_ <= first || race_ticks_ > first + 60) {
      return;
    }
    std::ofstream out(fmt::format("{}/tick{:03}_{}.bin", folder, race_ticks_ - first, when),
                      std::ios::binary);
    auto write = [&](uint32_t address, uint32_t size) {
      out.write(reinterpret_cast<const char*>(&address), 4);
      out.write(reinterpret_cast<const char*>(&size), 4);
      out.write(reinterpret_cast<const char*>(base + address), size);
    };
    write(kCameraPasses, kCameraPassCount * kCameraPassBytes);
    write(kDrawStack, kDrawStackBytes);
    if (race_ticks_ - first == 10) {
      ScanForCameraCopies(base, when);
    }
  }

 private:
  int race_ticks_ = 0;
  Matrix4 view_{}, view_projection_{}, previous_view_{}, previous_view_projection_{};
};

// Commands recorded during Update (the scenery's per-pass constants) hold
// the tick's view-projection, or an object's world * view-projection, and
// the render thread replays them unchanged on every frame of the tick. As
// each is replayed, it's given the frame's blended camera instead.
struct FrameCamera {
  Matrix4 tick;     // view-projection as game logic set it this tick
  Matrix4 blended;  // for the frame being drawn
};

// Command list slots allocated outside Draw: [start, end) guest ranges.
using Ranges = std::vector<std::pair<uint32_t, uint32_t>>;

std::mutex g_frame_cameras_mutex;
std::vector<FrameCamera> g_frame_cameras;  // from the last Draw
std::shared_ptr<const Ranges> g_update_ranges;  // recorded in the last tick's Update

void PublishFrameCameras(const std::vector<FrameCamera>& cameras) {
  std::lock_guard lock(g_frame_cameras_mutex);
  g_frame_cameras = cameras;
}

void PublishUpdateRanges(std::shared_ptr<const Ranges> ranges) {
  std::lock_guard lock(g_frame_cameras_mutex);
  g_update_ranges = std::move(ranges);
}

bool NearlyEqual(const Matrix4& a, const Matrix4& b) {
  for (int i = 0; i < 16; i++) {
    float tolerance = 1e-4f * std::max(1.0f, std::abs(b[i]));
    if (!(std::abs(a[i] - b[i]) <= tolerance)) {
      return false;
    }
  }
  return true;
}

class ReplayCameras {
 public:
  // Render thread, as a frame's replay starts.
  void OnFrameStart() {
    std::lock_guard lock(g_frame_cameras_mutex);
    update_ranges_ = g_update_ranges;
    cameras_.clear();
    for (const FrameCamera& camera : g_frame_cameras) {
      std::optional<Matrix4> inverse = Inverse(camera.tick);
      if (inverse) {
        cameras_.push_back({camera, *inverse, Multiply(*inverse, camera.blended)});
      }
    }
  }

  // Render thread, before a shader constant upload: the value to upload in
  // place of `m`, if it was made with a tick camera.
  std::optional<Matrix4> Substitute(uint32_t payload, const Matrix4& m) {
    if (!RecordedInUpdate(payload)) {
      return std::nullopt;  // recorded in Draw, already with the blended camera
    }
    for (const Camera& camera : cameras_) {
      if (NearlyEqual(m, camera.frame.tick)) {
        return camera.frame.blended;
      }
      // m = world * tick: world = m * inverse(tick), an affine transform.
      Matrix4 world = Multiply(m, camera.tick_inverse);
      if (IsPlausibleWorld(world)) {
        return Multiply(m, camera.tick_to_blended);
      }
    }
    return std::nullopt;
  }

 private:
  struct Camera {
    FrameCamera frame;
    Matrix4 tick_inverse;
    Matrix4 tick_to_blended;  // inverse(tick) * blended
  };

  static bool IsPlausibleWorld(const Matrix4& w) {
    if (!(std::abs(w[3]) < 1e-3f && std::abs(w[7]) < 1e-3f && std::abs(w[11]) < 1e-3f &&
          std::abs(w[15] - 1) < 1e-3f)) {
      return false;
    }
    for (int i = 0; i < 3; i++) {
      float length = std::sqrt(w[i * 4] * w[i * 4] + w[i * 4 + 1] * w[i * 4 + 1] +
                               w[i * 4 + 2] * w[i * 4 + 2]);
      if (!(length > 1e-3f && length < 1e3f)) {
        return false;
      }
    }
    return true;
  }

  bool RecordedInUpdate(uint32_t address) const {
    if (!update_ranges_) {
      return false;
    }
    auto it = std::upper_bound(update_ranges_->begin(), update_ranges_->end(),
                               std::pair{address, UINT32_MAX});
    return it != update_ranges_->begin() && address < std::prev(it)->second;
  }

  std::vector<Camera> cameras_;
  std::shared_ptr<const Ranges> update_ranges_;
};

ReplayCameras& GetReplayCameras() {
  static ReplayCameras cameras;
  return cameras;
}

// Diagnostics: who multiplies by a camera's view-projection, and when.
class CameraProductLog {
 public:
  void OnMultiply(uint32_t out, uint32_t a, uint32_t b, uint32_t caller, bool in_draw) {
    if (!REXCVAR_GET(log_camera_products)) {
      return;
    }
    std::lock_guard lock(mutex_);
    if (PassOfMatrix(out) >= 0) {
      // Something computing a pass's matrix: log its operands once a second.
      Entry& e = writes_[{caller, PassOfMatrix(out), out - CameraPass(PassOfMatrix(out)), in_draw}];
      e.count++;
      e.last_out = a;
      e.last_other = b;
    }
    int pass = PassOf(a) >= 0 ? PassOf(a) : PassOf(b);
    if (pass < 0) {
      LogMaybe();
      return;
    }
    Key key{caller, pass, PassOf(a) >= 0, in_draw};
    Entry& e = entries_[key];
    e.count++;
    e.last_out = out;
    e.last_other = PassOf(a) >= 0 ? b : a;
    LogMaybe();
  }

  void LogMaybe() {
    auto now = std::chrono::steady_clock::now();
    if (now - last_log_ < std::chrono::seconds(1)) {
      return;
    }
    for (const auto& [k, v] : writes_) {
      REXLOG_INFO("pass matrix write: caller {:08X} pass {} +{:X} in {}: {} calls, a {:08X} b {:08X}",
                  k.caller, k.pass, k.offset, k.in_draw ? "Draw" : "other", v.count, v.last_out,
                  v.last_other);
    }
    writes_.clear();
    for (const auto& [k, v] : entries_) {
      REXLOG_INFO("camera product: caller {:08X} pass {} vp as {} in {}: {} calls, out {:08X} other {:08X}",
                  k.caller, k.pass, k.vp_first ? "a" : "b", k.in_draw ? "Draw" : "other",
                  v.count, v.last_out, v.last_other);
    }
    entries_.clear();
    last_log_ = now;
  }

 private:
  static int PassOf(uint32_t address) {
    if (address < kCameraPasses || address >= CameraPass(kCameraPassCount) ||
        (address - kCameraPasses) % kCameraPassBytes != kViewProjectionOffset) {
      return -1;
    }
    return int((address - kCameraPasses) / kCameraPassBytes);
  }
  struct Key {
    uint32_t caller;
    int pass;
    bool vp_first;
    bool in_draw;
    auto operator<=>(const Key&) const = default;
  };
  struct WriteKey {
    uint32_t caller;
    int pass;
    uint32_t offset;
    bool in_draw;
    auto operator<=>(const WriteKey&) const = default;
  };
  static int PassOfMatrix(uint32_t address) {
    if (address < kCameraPasses || address >= CameraPass(kCameraPassCount)) {
      return -1;
    }
    return int((address - kCameraPasses) / kCameraPassBytes);
  }
  struct Entry {
    int count = 0;
    uint32_t last_out = 0;
    uint32_t last_other = 0;
  };
  std::mutex mutex_;
  std::map<Key, Entry> entries_;
  std::map<WriteKey, Entry> writes_;
  std::chrono::steady_clock::time_point last_log_{};
};

class DrawState {
 public:
  // Main thread, after a matrix multiply outside Draw whose second operand
  // is a camera's view-projection: game logic prepared world *
  // view-projection for something drawn later (the scenery does this).
  void OnCameraProduct(uint8_t* base, uint32_t out, uint32_t world, uint32_t view_projection) {
    // Draw runs on the main thread; other threads (loading) multiply too.
    if (std::this_thread::get_id() != main_thread_ || in_draw_ ||
        !IsCameraViewProjection(view_projection) || products_pending_.size() >= 4096) {
      return;
    }
    int pass = int((view_projection - kCameraPasses) / kCameraPassBytes);
    products_pending_.push_back({out, pass, LoadMatrix(base, world), LoadMatrix(base, out)});
  }

  // Main thread, after the game allocated command list slots.
  void OnCommandSlots(uint32_t address, uint32_t count) {
    if (in_draw_ || address == 0 || std::this_thread::get_id() != main_thread_) {
      return;
    }
    uint32_t end = address + count * 16;
    if (!update_ranges_pending_.empty() && update_ranges_pending_.back().second == address) {
      update_ranges_pending_.back().second = end;
    } else {
      update_ranges_pending_.push_back({address, end});
    }
  }

  bool in_draw() const { return in_draw_; }
  CameraProductLog& camera_products() { return camera_products_; }

  void BeforeDraw(uint8_t* base, bool logic_pass) {
    main_thread_ = std::this_thread::get_id();
    in_draw_ = true;
    logic_pass_ = logic_pass;
    if (logic_pass) {
      dump_.OnTick(base);
    }
    dump_.OnDraw(base, logic_pass, "pre");
    if (logic_pass) {
      OnTick(base);
    }
    amount_ = frame_pacing::GetFramePacer().TickProgress();
    recorded_.clear();
    saved_.clear();
    enabled_ = REXCVAR_GET(interpolate_draw_state);
    if (!enabled_) {
      return;
    }
    // Draw rebuilds each camera's view from these; blend them, as positions.
    for (const LookAtBlend& blend : look_at_blends_) {
      for (int i = 0; i < 3; i++) {
        uint32_t address = CameraPass(blend.pass) + kLookAtOffsets[i];
        Vec3 v;
        for (int k = 0; k < 3; k++) {
          v[k] = blend.from[i][k] + (blend.to[i][k] - blend.from[i][k]) * amount_;
        }
        if (i == 0 && blend.pass == 0) {
          v[1] += float(REXCVAR_GET(debug_camera_lift));
        }
        ReplaceVec3(base, address, v);
      }
      counts_.look_ats++;
    }
    frame_cameras_.clear();
    std::array<std::optional<Matrix4>, kCameraPassCount> blended{};
    for (const CameraBlend& camera : camera_blends_) {
      uint32_t pass = CameraPass(camera.pass);
      Transform eye = Blend(camera.from, camera.to, amount_);
      Matrix4 view = ViewMatrixFromTransform(eye);
      Matrix4 view_projection = Multiply(view, camera.projection);
      Replace(base, saved_, pass + kViewOffset, view);
      Replace(base, saved_, pass + kViewProjectionOffset, view_projection);
      blended[camera.pass] = view_projection;
      frame_cameras_.push_back({camera.tick_view_projection, view_projection});
      counts_.cameras++;
    }
  }

  void AfterDraw(uint8_t* base) {
    in_draw_ = false;
    PublishFrameCameras(frame_cameras_);
    Restore(base, saved_);
    for (auto it = saved_vec3_.rbegin(); it != saved_vec3_.rend(); ++it) {
      if (LoadVec3(base, it->address) == it->written) {
        StoreVec3(base, it->address, it->original);
      }
    }
    saved_vec3_.clear();
    dump_.OnDraw(base, logic_pass_, "post");
    counts_.draws++;
    LogCounts();
  }

  void BeforeRecordObject(uint8_t* base, uint32_t view_projection, uint32_t world) {
    object_saved_.clear();
    if (!IsCameraViewProjection(view_projection)) {
      return;
    }
    ObjectKey key{world, recorded_[world]++};
    Matrix4 now = LoadMatrix(base, world);
    std::optional<ScaledTransform> transform;
    if (IsFinite(now)) {
      transform = ScaledTransformFromWorldMatrix(now);
    }
    if (!transform) {
      counts_.objects_not_transforms++;
      return;
    }
    if (logic_pass_) {
      objects_this_tick_[key] = *transform;
    }
    if (!enabled_) {
      return;
    }
    auto before = objects_last_tick_.find(key);
    if (before == objects_last_tick_.end()) {
      counts_.objects_unmatched++;
      return;
    }
    if (IsCut(before->second.transform, transform->transform)) {
      counts_.object_cuts++;
      return;
    }
    Replace(base, object_saved_, world,
            WorldMatrixFromScaledTransform(Blend(before->second, *transform, amount_)));
    counts_.objects++;
  }

  void AfterRecordObject(uint8_t* base) {
    Restore(base, object_saved_);
  }

 private:
  struct LookAtBlend {
    int pass;
    std::array<Vec3, 3> from;  // eye, target, up
    std::array<Vec3, 3> to;
  };

  struct SavedVec3 {
    uint32_t address;
    Vec3 original;
    Vec3 written;
  };

  void ReplaceVec3(uint8_t* base, uint32_t address, const Vec3& value) {
    saved_vec3_.push_back({address, LoadVec3(base, address), value});
    StoreVec3(base, address, value);
  }

  struct CameraBlend {
    int pass;
    Transform from;
    Transform to;
    Matrix4 projection;
    Matrix4 tick_view_projection;  // as game logic set it this tick
  };

  struct CameraConstant {
    uint32_t payload;
    int pass;
    Matrix4 written;  // what's there now
  };

  struct CameraProduct {
    uint32_t out;
    int pass;
    Matrix4 world;
    Matrix4 value;  // world * the pass's view-projection, as game logic left it
  };

  struct ObjectKey {
    uint32_t address;
    int occurrence;  // times this address was recorded earlier in the Draw
    bool operator==(const ObjectKey&) const = default;
  };
  struct ObjectKeyHash {
    size_t operator()(const ObjectKey& key) const {
      return std::hash<uint64_t>()(uint64_t(key.address) << 20 ^ uint64_t(key.occurrence));
    }
  };

  // Main thread, before the Draw of a logic pass (after its Update).
  void OnTick(uint8_t* base) {
    auto ranges = std::make_shared<Ranges>(std::move(update_ranges_pending_));
    update_ranges_pending_.clear();
    std::sort(ranges->begin(), ranges->end());
    PublishUpdateRanges(std::move(ranges));
    camera_blends_.clear();
    look_at_blends_.clear();
    for (int pass = 0; pass < kCameraPassCount; pass++) {
      std::optional<Transform> camera = CameraOf(base, pass);
      if (camera && cameras_[pass]) {
        if (IsCut(*cameras_[pass], *camera)) {
          counts_.camera_cuts++;
        } else {
          camera_blends_.push_back({pass, *cameras_[pass], *camera,
                                    LoadMatrix(base, CameraPass(pass) + kProjectionOffset),
                                    LoadMatrix(base, CameraPass(pass) + kViewProjectionOffset)});
        }
      }
      cameras_[pass] = camera;

      std::array<Vec3, 3> look_at;
      bool finite = true;
      for (int i = 0; i < 3; i++) {
        look_at[i] = LoadVec3(base, CameraPass(pass) + kLookAtOffsets[i]);
        finite &= std::all_of(look_at[i].begin(), look_at[i].end(),
                              [](float v) { return std::isfinite(v); });
      }
      if (finite && look_ats_[pass] && look_at != *look_ats_[pass] &&
          Distance((*look_ats_[pass])[0], look_at[0]) <= kCutDistance &&
          Distance((*look_ats_[pass])[1], look_at[1]) <= kCutDistance) {
        look_at_blends_.push_back({pass, *look_ats_[pass], look_at});
      }
      look_ats_[pass] = finite ? std::optional(look_at) : std::nullopt;
    }
    objects_last_tick_ = std::move(objects_this_tick_);
    objects_this_tick_.clear();
    products_ = std::move(products_pending_);
    products_pending_.clear();
  }

  // Commands recorded during Update that set shader constants to a
  // camera's view-projection (the scenery's per-pass constants): rewritten
  // with the blended camera on every pass, since the render thread replays
  // them after Draw.
  void FindCameraConstants(uint8_t* base) {
    camera_constants_.clear();
    uint32_t list = guest::LoadU32(base, kCommandListPointer);
    if (list == 0 || camera_blends_.empty()) {
      return;
    }
    std::vector<std::pair<int, Matrix4>> cameras;
    for (const CameraBlend& camera : camera_blends_) {
      cameras.push_back({camera.pass, LoadMatrix(base, CameraPass(camera.pass) + kViewProjectionOffset)});
    }
    uint32_t capacity = guest::LoadU32(base, list + kCommandListCapacityOffset);
    uint32_t used = std::max(guest::LoadU32(base, list + kCommandListUsedOffset),
                             guest::LoadU32(base, list + kCommandListPreviousUsedOffset));
    uint32_t slots = std::min(capacity, used);
    for (uint32_t offset : kCommandListBufferOffsets) {
      uint32_t buffer = guest::LoadU32(base, list + offset);
      if (buffer == 0) {
        continue;
      }
      for (uint32_t slot = 0; slot + 5 <= slots; slot++) {
        uint32_t header = buffer + slot * kCommandSlotBytes;
        if (guest::LoadU32(base, header) != kSetConstantsCommand) {
          continue;
        }
        uint32_t payload = header + kCommandSlotBytes;
        // Cheap check on the first value before loading the matrix.
        float first = guest::LoadFloat(base, payload);
        for (const auto& [pass, view_projection] : cameras) {
          if (first == view_projection[0] && LoadMatrix(base, payload) == view_projection) {
            camera_constants_.push_back({payload, pass, view_projection});
            break;
          }
        }
      }
    }
  }

  // The camera of a pass whose view-projection is its view * projection (set
  // by game logic, rather than recomputed in Draw).
  static std::optional<Transform> CameraOf(uint8_t* base, int pass) {
    uint32_t address = CameraPass(pass);
    Matrix4 view = LoadMatrix(base, address + kViewOffset);
    Matrix4 projection = LoadMatrix(base, address + kProjectionOffset);
    Matrix4 view_projection = LoadMatrix(base, address + kViewProjectionOffset);
    if (!IsFinite(view) || !IsFinite(projection) || !IsFinite(view_projection) ||
        !IsProduct(view_projection, view, projection)) {
      return std::nullopt;
    }
    return TransformFromViewMatrix(view);
  }

  static bool IsCameraViewProjection(uint32_t address) {
    return address >= kCameraPasses && address < CameraPass(kCameraPassCount) &&
           (address - kCameraPasses) % kCameraPassBytes == kViewProjectionOffset;
  }

  struct Saved {
    uint32_t address;
    Matrix4 original;
    Matrix4 written;
  };

  static void Replace(uint8_t* base, std::vector<Saved>& saved, uint32_t address,
                      const Matrix4& value) {
    saved.push_back({address, LoadMatrix(base, address), value});
    StoreMatrix(base, address, value);
  }

  // Puts the originals back, except where the game has since written
  // something else there (the memory may have been freed and reused, as
  // when a race loads).
  static void Restore(uint8_t* base, std::vector<Saved>& saved) {
    for (auto it = saved.rbegin(); it != saved.rend(); ++it) {
      if (LoadMatrix(base, it->address) == it->written) {
        StoreMatrix(base, it->address, it->original);
      }
    }
    saved.clear();
  }

  void LogCounts() {
    if (!REXCVAR_GET(log_blend_state)) {
      return;
    }
    auto now = std::chrono::steady_clock::now();
    if (now - counts_.last_log < std::chrono::seconds(1)) {
      return;
    }
    int draws = std::max(counts_.draws, 1);
    REXLOG_INFO(
        "blend state: {} draws; per draw {} look-ats, {} cameras, {} camera constants, {} camera products, "
        "{} objects ({} unmatched, {} not transforms); {} camera cuts, {} object cuts",
        counts_.draws, counts_.look_ats / draws, counts_.cameras / draws, counts_.constants / draws,
        counts_.products / draws, counts_.objects / draws,
        counts_.objects_unmatched / draws, counts_.objects_not_transforms / draws,
        counts_.camera_cuts, counts_.object_cuts);
    counts_ = {};
    counts_.last_log = now;
  }

  bool logic_pass_ = false;
  std::atomic<bool> in_draw_ = false;
  std::thread::id main_thread_;
  CameraProductLog camera_products_;
  bool enabled_ = false;
  float amount_ = 0;

  std::array<std::optional<Transform>, kCameraPassCount> cameras_;
  std::vector<CameraBlend> camera_blends_;
  std::vector<FrameCamera> frame_cameras_;
  Ranges update_ranges_pending_;
  std::array<std::optional<std::array<Vec3, 3>>, kCameraPassCount> look_ats_;
  std::vector<LookAtBlend> look_at_blends_;
  std::vector<SavedVec3> saved_vec3_;

  std::unordered_map<ObjectKey, ScaledTransform, ObjectKeyHash> objects_last_tick_;
  std::unordered_map<ObjectKey, ScaledTransform, ObjectKeyHash> objects_this_tick_;
  std::unordered_map<uint32_t, int> recorded_;

  std::vector<CameraConstant> camera_constants_;
  std::vector<CameraProduct> products_pending_;  // made since the last tick's Draw
  std::vector<CameraProduct> products_;

  std::vector<Saved> saved_;
  std::vector<Saved> object_saved_;

  DrawStateDump dump_;

  struct Counts {
    int draws = 0;
    int cameras = 0;
    int objects = 0;
    int products = 0;
    int constants = 0;
    int look_ats = 0;
    int objects_unmatched = 0;
    int objects_not_transforms = 0;
    int camera_cuts = 0;
    int object_cuts = 0;
    std::chrono::steady_clock::time_point last_log{};
  } counts_;
};

// Diagnostics on the render thread: the vertex shader constant uploads it
// replays each frame (4+ registers), compared with the same upload (by
// order) in the previous frame.
class ReplayedConstantsLog {
 public:
  void OnFrameStart() {
    if (!REXCVAR_GET(log_replayed_constants)) {
      return;
    }
    auto now = std::chrono::steady_clock::now();
    if (logging_) {
      logging_ = false;
      REXLOG_INFO("replayed constants: frame with {} uploads, {} changed since the last frame",
                  frame_.size(), changed_);
      for (const auto& line : lines_) {
        REXLOG_INFO("replayed constants: {}", line);
      }
    }
    if (now - last_log_ >= std::chrono::seconds(1) && frames_since_tick_ == 2) {
      logging_ = true;
      last_log_ = now;
    }
    frames_since_tick_++;
    previous_ = std::move(frame_);
    frame_.clear();
    lines_.clear();
    changed_ = 0;
  }

  void OnTick() { frames_since_tick_ = 0; }

  void OnUpload(uint8_t* base, uint32_t payload, uint32_t packed) {
    if (!REXCVAR_GET(log_replayed_constants)) {
      return;
    }
    uint32_t stage = packed >> 24, start = (packed >> 8) & 0xFF, count = packed & 0xFF;
    if (stage != 0 || count < 4) {
      return;
    }
    Matrix4 m = LoadMatrix(base, payload);
    size_t index = frame_.size();
    bool changed = index >= previous_.size() || previous_[index] != m;
    changed_ += changed;
    frame_.push_back(m);
    if (logging_ && lines_.size() < 60) {
      lines_.push_back(fmt::format("#{} c{} x{} from {:08X} {} [{:.3g} {:.3g} {:.3g} {:.3g} | ... | "
                                   "{:.4g} {:.4g} {:.4g} {:.4g}]",
                                   index, start, count, payload, changed ? "changed" : "same",
                                   m[0], m[1], m[2], m[3], m[12], m[13], m[14], m[15]));
    }
  }

 private:
  std::vector<Matrix4> frame_, previous_;
  std::vector<std::string> lines_;
  int changed_ = 0;
  int frames_since_tick_ = 0;
  bool logging_ = false;
  std::chrono::steady_clock::time_point last_log_{};
};

ReplayedConstantsLog& GetReplayedConstantsLog() {
  static ReplayedConstantsLog log;
  return log;
}

DrawState& GetDrawState() {
  static DrawState state;
  return state;
}

}  // namespace

void BlendState_BeforeDraw(uint8_t* base, bool render_only_pass) {
  GetDrawState().BeforeDraw(base, !render_only_pass);
}

void BlendState_OnReplayFrameStart() {
  GetReplayedConstantsLog().OnFrameStart();
  GetReplayCameras().OnFrameStart();
}

void BlendState_ReplayConstants(uint8_t* base, uint32_t payload, uint32_t packed,
                                const std::function<void()>& upload) {
  GetReplayedConstantsLog().OnUpload(base, payload, packed);
  uint32_t stage = packed >> 24, count = packed & 0xFF;
  std::optional<Matrix4> original;
  if (REXCVAR_GET(interpolate_draw_state) && stage == 0 && count >= 4) {
    Matrix4 m = LoadMatrix(base, payload);
    if (IsFinite(m)) {
      if (std::optional<Matrix4> substitute = GetReplayCameras().Substitute(payload, m)) {
        original = m;
        StoreMatrix(base, payload, *substitute);
      }
    }
  }
  upload();
  if (original) {
    StoreMatrix(base, payload, *original);
  }
}

void BlendState_OnReplayTick() {
  GetReplayedConstantsLog().OnTick();
}

void BlendState_OnReplayConstants(uint8_t* base, uint32_t payload, uint32_t packed) {
  GetReplayedConstantsLog().OnUpload(base, payload, packed);
}

void BlendState_AfterDraw(uint8_t* base) {
  GetDrawState().AfterDraw(base);
}

void BlendState_BeforeRecordObject(uint8_t* base, uint32_t view_projection, uint32_t world) {
  GetDrawState().BeforeRecordObject(base, view_projection, world);
}

void BlendState_AfterRecordObject(uint8_t* base) {
  GetDrawState().AfterRecordObject(base);
}

}  // namespace interpolation

// Matrix_Multiply: r3 = out, r4 = a, r5 = b; out = a * b (4x4).
extern "C" REX_FUNC(sub_8209BD50) {
  REX_FUNC_PROLOGUE();
  auto& state = interpolation::GetDrawState();
  state.camera_products().OnMultiply(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, uint32_t(ctx.lr),
                                     state.in_draw());
  uint32_t out = ctx.r3.u32, a = ctx.r4.u32, b = ctx.r5.u32;
  __imp__sub_8209BD50(ctx, base);
  state.OnCameraProduct(base, out, a, b);
}

// CommandList_AllocateSlots: r3 = list, r4 = slot count; returns r3 = the
// first slot (16 bytes each), or 0 if the list is full.
extern "C" REX_FUNC(sub_8258CAA0) {
  REX_FUNC_PROLOGUE();
  uint32_t count = ctx.r4.u32;
  __imp__sub_8258CAA0(ctx, base);
  interpolation::GetDrawState().OnCommandSlots(ctx.r3.u32, count);
}
