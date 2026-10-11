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
#include <map>
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

// Diagnostics: who multiplies by a camera's view-projection, and when.
class CameraProductLog {
 public:
  void OnMultiply(uint32_t out, uint32_t a, uint32_t b, uint32_t caller, bool in_draw) {
    if (!REXCVAR_GET(log_camera_products)) {
      return;
    }
    std::lock_guard lock(mutex_);
    int pass = PassOf(a) >= 0 ? PassOf(a) : PassOf(b);
    if (pass < 0) {
      return;
    }
    Key key{caller, pass, PassOf(a) >= 0, in_draw};
    Entry& e = entries_[key];
    e.count++;
    e.last_out = out;
    e.last_other = PassOf(a) >= 0 ? b : a;
    auto now = std::chrono::steady_clock::now();
    if (now - last_log_ < std::chrono::seconds(1)) {
      return;
    }
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
  struct Entry {
    int count = 0;
    uint32_t last_out = 0;
    uint32_t last_other = 0;
  };
  std::mutex mutex_;
  std::map<Key, Entry> entries_;
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
    std::array<std::optional<Matrix4>, kCameraPassCount> blended{};
    for (const CameraBlend& camera : camera_blends_) {
      uint32_t pass = CameraPass(camera.pass);
      Matrix4 view = ViewMatrixFromTransform(Blend(camera.from, camera.to, amount_));
      Matrix4 view_projection = Multiply(view, camera.projection);
      Replace(base, saved_, pass + kViewOffset, view);
      Replace(base, saved_, pass + kViewProjectionOffset, view_projection);
      blended[camera.pass] = view_projection;
      counts_.cameras++;
    }
    for (CameraConstant& constant : camera_constants_) {
      if (!blended[constant.pass] || LoadMatrix(base, constant.payload) != constant.written) {
        continue;  // recorded over
      }
      Matrix4 value = *blended[constant.pass];
      if (float lift = float(REXCVAR_GET(debug_camera_constant_lift)); lift != 0) {
        // Moving the eye up moves the world down: world * T(0, -lift, 0) * VP.
        Matrix4 down = WorldMatrixFromTransform(Transform{{}, {0, -lift, 0}});
        value = Multiply(down, value);
      }
      StoreMatrix(base, constant.payload, value);
      constant.written = value;
      counts_.constants++;
    }
    // Products game logic made with a camera, still in place: redo them with
    // the blended camera.
    for (const CameraProduct& product : products_) {
      if (!blended[product.pass] || LoadMatrix(base, product.out) != product.value) {
        continue;
      }
      Replace(base, saved_, product.out, Multiply(product.world, *blended[product.pass]));
      counts_.products++;
    }
  }

  void AfterDraw(uint8_t* base) {
    in_draw_ = false;
    Restore(base, saved_);
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
  struct CameraBlend {
    int pass;
    Transform from;
    Transform to;
    Matrix4 projection;
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
    camera_blends_.clear();
    for (int pass = 0; pass < kCameraPassCount; pass++) {
      std::optional<Transform> camera = CameraOf(base, pass);
      if (camera && cameras_[pass]) {
        if (IsCut(*cameras_[pass], *camera)) {
          counts_.camera_cuts++;
        } else {
          camera_blends_.push_back({pass, *cameras_[pass], *camera,
                                    LoadMatrix(base, CameraPass(pass) + kProjectionOffset)});
        }
      }
      cameras_[pass] = camera;
    }
    objects_last_tick_ = std::move(objects_this_tick_);
    objects_this_tick_.clear();
    products_ = std::move(products_pending_);
    products_pending_.clear();
    FindCameraConstants(base);
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
        "blend state: {} draws; per draw {} cameras, {} camera constants, {} camera products, "
        "{} objects ({} unmatched, {} not transforms); {} camera cuts, {} object cuts",
        counts_.draws, counts_.cameras / draws, counts_.constants / draws,
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
    int objects_unmatched = 0;
    int objects_not_transforms = 0;
    int camera_cuts = 0;
    int object_cuts = 0;
    std::chrono::steady_clock::time_point last_log{};
  } counts_;
};

DrawState& GetDrawState() {
  static DrawState state;
  return state;
}

}  // namespace

void BlendState_BeforeDraw(uint8_t* base, bool render_only_pass) {
  GetDrawState().BeforeDraw(base, !render_only_pass);
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
