// The camera modes (src/camera_modes/, docs/functions/camera-modes.md)
// against the recompiled functions. Each case fills blocks from the
// allocation layer with its inputs, points the globals the functions read
// at them (the viewports [0x826C2CE8], the race data [0x826C5B2C], the
// color filter PostEffects::Get(2)) or saves the ones they write (the
// camera animations' table, the screen fades), runs the original (with
// --native_camera_modes off) or the adapter's step (hooks/camera_modes.h),
// and compares what they leave, putting the globals back after each run.
// Floats the functions load and store as floats are compared with
// harness::Outputs' float entries; the rest exactly.

#include "harness.h"
#include "exit_table.h"
#include "test_route.h"

#include "base/flags.h"
#include "base/log.h"
#include "camera_modes/camera_modes.h"
#include "c_runtime/exit.h"
#include "command_list/command_list.h"
#include "hooks/archive.h"
#include "hooks/c_runtime.h"
#include "hooks/camera_modes.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/race_hud.h"
#include "hooks/ui_movies_game.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "game_modes/game_modes.h"
#include "mode_tick/mode_tick.h"
#include "post_effects/post_effects.h"
#include "track_zones/track_zones.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <cstring>
#include <limits>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_camera_modes);
ITC_FLAG_DECLARE(bool, log_pause_movie);

namespace harness {

namespace {

namespace cm = camera_modes;

// Runs the recompiled version with the native flag off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  const bool saved = ITC_FLAG(native_camera_modes);
  ITC_FLAG(native_camera_modes) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_camera_modes) = saved;
}

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

// A global word (or bytes) saved and put back.
class Saved {
 public:
  Saved(uint8_t* base, uint32_t address, uint32_t bytes)
      : base_(base), address_(address), bytes_(Read(base, address, bytes)) {}
  ~Saved() { Restore(); }
  void Restore() { Write(base_, address_, bytes_.data(), uint32_t(bytes_.size())); }
  Saved(const Saved&) = delete;
  Saved& operator=(const Saved&) = delete;

 private:
  uint8_t* base_;
  uint32_t address_;
  std::vector<uint8_t> bytes_;
};

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(base, address + i, uint8_t(r.Next()));
  }
}

// A float's bits: mostly ordinary values (-1000 to 1000, now and then tiny
// or huge), now and then 0, -0, an infinity, a NaN or a denormal.
uint32_t RandomFloat(Random& r) {
  switch (r.Below(24)) {
    case 0: return 0;
    case 1: return 0x80000000u;
    case 2: return r.Below(2) ? 0x7F800000u : 0xFF800000u;
    case 3: return 0x7FC00000u | uint32_t(r.Below(0x400000));
    case 4: return uint32_t(r.Below(0x800000)) | (r.Below(2) << 31);
    case 5: return std::bit_cast<uint32_t>(float(double(r.Next() % 2000001) * 1e-12));
    case 6: return std::bit_cast<uint32_t>(float(double(r.Next() % 2000001) * 1e30));
    default: {
      const double v = (double(r.Next() % 2000001) - 1000000.0) / 1000.0;
      return std::bit_cast<uint32_t>(float(v));
    }
  }
}

// An ordinary float (no NaN or infinity): -range to range.
uint32_t OrdinaryFloat(Random& r, double range) {
  const double v = (double(r.Next() % 2000001) - 1000000.0) / 1000000.0 * range;
  return std::bit_cast<uint32_t>(float(v));
}

PPCContext AlignedContext(const Context& h) {
  PPCContext t = CallContext(h);
  t.r1.u64 = t.r1.u64 & ~0xFull;
  return t;
}

// --- The director's handlers ---

constexpr uint32_t kRaceBytes = 0xA3B00;
constexpr uint32_t kRecords = 0x59AE8;
constexpr uint32_t kRecordBytes = 3936;
constexpr uint32_t kRecordCount = 0x49FE0;
constexpr uint32_t kDirectorBytes = 0x12E70;
constexpr uint32_t kViewportBytes = 1872;

void Handlers(Context& h, Random& r) {
  Block race(kRaceBytes);
  Block director(kDirectorBytes);
  Block viewports(16 + 2 * kViewportBytes);
  const uint32_t block = viewports.address;
  const uint32_t vp0 = block + 16;
  const uint32_t vp1 = vp0 + kViewportBytes;
  Saved saved_viewports(h.base, cm::kViewports, 4);
  Saved saved_race(h.base, cm::kRaceData, 4);
  const struct {
    const char* name;
    GuestFunction recompiled, native;
  } kFunctions[] = {
      {"CameraDirector::ToView4 (sub_82152400)", Original<__imp__sub_82152400>,
       hooks::CameraModesStep82152400},
      {"CameraDirector::ToView5 (sub_821524B8)", Original<__imp__sub_821524B8>,
       hooks::CameraModesStep821524B8},
      {"CameraDirector::ToView3 (sub_821524D8)", Original<__imp__sub_821524D8>,
       hooks::CameraModesStep821524D8},
      {"CameraDirector::ToView3IfCourse (sub_82153160)", Original<__imp__sub_82153160>,
       hooks::CameraModesStep82153160},
  };
  for (const auto& f : kFunctions) {
    Results results(f.name);
    for (int c = 0; c < 200; c++) {
      // The fields read: the split flag, the kind and course, the records'
      // count and the last one's +3924, the view.
      const uint32_t split = r.Below(2) ? 0 : uint32_t(r.Next());
      const uint32_t kind = r.Below(3);
      const uint32_t course = r.Below(13);
      const uint32_t count = 1 + r.Below(5);
      const uint32_t last = r.Below(4);
      const uint32_t view = r.Below(6);
      const uint32_t fields[4] = {uint32_t(r.Next()), uint32_t(r.Next()), uint32_t(r.Next()),
                                  uint32_t(r.Next())};
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        guest::StoreU32(h.base, cm::kViewports, block);
        guest::StoreU32(h.base, cm::kRaceData, race.address);
        guest::StoreU32(h.base, block + 0, vp0);
        guest::StoreU32(h.base, block + 4, vp1);
        guest::StoreU32(h.base, block + 8, split);
        guest::StoreU32(h.base, vp0 + 1860, fields[0]);
        guest::StoreU32(h.base, race.address + 0xA3AF0, kind);
        guest::StoreU32(h.base, race.address + 4, course);
        guest::StoreU32(h.base, race.address + kRecords + kRecordCount, count);
        guest::StoreU32(h.base,
                        race.address + kRecords + kRecordBytes * (count - 1) + 3924, last);
        guest::StoreU32(h.base, director.address + cm::kView, view);
        guest::StoreU32(h.base, director.address + cm::kViewKept, fields[1]);
        guest::StoreU32(h.base, director.address + cm::kFollowed, fields[2]);
        guest::StoreU32(h.base, director.address + cm::kFollowedInput, fields[3]);
        PPCContext t = CallContext(h);
        t.r3.u64 = director.address;
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        out[v].Bytes(Read(h.base, block, 12));
        out[v].Word(guest::LoadU32(h.base, vp0 + 1860));
        out[v].Bytes(Read(h.base, director.address + cm::kView, 12));
        out[v].Word(guest::LoadU32(h.base, director.address + cm::kFollowedInput));
        out[v].Word(t.r3.u32);
        saved_viewports.Restore();
        saved_race.Restore();
      }
      results.Check(out[0], out[1], fmt::format("split {:X} kind {} course {} last {} view {}",
                                                split, kind, course, last, view));
    }
    results.Report();
  }
}

// --- The zoom, the roll and the quaternions ---

void Small(Context& h, Random& r) {
  {
    Results results("Viewports::ResetRoll (sub_821524F0)");
    Block viewports(16 + 2 * kViewportBytes);
    const uint32_t block = viewports.address;
    Saved saved(h.base, cm::kViewports, 4);
    for (int c = 0; c < 50; c++) {
      RandomBytes(h.base, block, viewports.bytes, r);
      const std::vector<uint8_t> inputs = Read(h.base, block, viewports.bytes);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, block, inputs.data(), viewports.bytes);
        guest::StoreU32(h.base, cm::kViewports, block);
        guest::StoreU32(h.base, block + 0, block + 16);
        guest::StoreU32(h.base, block + 4, block + 16 + kViewportBytes);
        PPCContext t = CallContext(h);
        (v == 0 ? Original<__imp__sub_821524F0> : hooks::CameraModesStep821524F0)(t, h.base);
        out[v].Bytes(Read(h.base, block, viewports.bytes));
        out[v].Word(t.r3.u32);
        saved.Restore();
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Camera::ZoomFromFov (sub_82152548)");
    for (int c = 0; c < 400; c++) {
      double degrees, width;
      if (c < 8) {
        const double kDegrees[] = {55.0, 45.0, 90.0, 0.0, -55.0, 180.0, 359.0, 720.5};
        degrees = kDegrees[c];
        width = 1280.0;
      } else {
        degrees = double(std::bit_cast<float>(r.Below(4) ? OrdinaryFloat(r, 1000.0)
                                                         : RandomFloat(r)));
        width = double(std::bit_cast<float>(r.Below(4) ? OrdinaryFloat(r, 2000.0)
                                                       : RandomFloat(r)));
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        PPCContext t = CallContext(h);
        t.f1.f64 = degrees;
        t.f2.f64 = width;
        (v == 0 ? Original<__imp__sub_82152548> : hooks::CameraModesStep82152548)(t, h.base);
        out[v].Double(t.f1.u64);
      }
      results.Check(out[0], out[1], fmt::format("degrees {} width {}", degrees, width));
    }
    results.Report();
  }
  {
    Results results("Quat::Rotate (sub_821525F8)");
    for (int c = 0; c < 400; c++) {
      // out apart, or on the vector or the quaternion now and then.
      const uint32_t q = h.data + 0x100, vec = h.data + 0x120;
      const uint32_t k = r.Below(6);
      const uint32_t out_at = k == 0 ? vec : k == 1 ? q : h.data + 0x140;
      uint32_t words[8];
      for (uint32_t& w : words) {
        w = r.Below(6) ? OrdinaryFloat(r, 10.0) : RandomFloat(r);
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        for (uint32_t i = 0; i < 4; i++) {
          guest::StoreU32(h.base, q + 4 * i, words[i]);
          guest::StoreU32(h.base, vec + 4 * i, words[4 + i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = out_at;
        t.r4.u64 = q;
        t.r5.u64 = vec;
        (v == 0 ? Original<__imp__sub_821525F8> : hooks::CameraModesStep821525F8)(t, h.base);
        out[v].Floats(h.base, h.data + 0x100, 24);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {} out {}", c, k));
    }
    results.Report();
  }
  {
    Results results("Matrix::FromQuat (sub_82152698)");
    for (int c = 0; c < 400; c++) {
      uint32_t q[4];
      for (uint32_t& w : q) {
        w = r.Below(6) ? OrdinaryFloat(r, 2.0) : RandomFloat(r);
      }
      if (c == 0) {
        q[0] = q[1] = q[2] = 0;
        q[3] = 0x3F800000;
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        PPCContext t = AlignedContext(h);
        Fill(h.base, t.r1.u32 - 32, 96);
        t.r3.u64 = h.data + 0x100;
        t.r4.u64 = (uint64_t(q[0]) << 32) | q[1];
        t.r5.u64 = (uint64_t(q[2]) << 32) | q[3];
        (v == 0 ? Original<__imp__sub_82152698> : hooks::CameraModesStep82152698)(t, h.base);
        out[v].Floats(h.base, h.data + 0x100, 16);
        out[v].Bytes(Read(h.base, t.r1.u32 + 32, 16));
        out[v].Floats(h.base, t.r1.u32 - 16, 4);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Matrix::FromFloats (sub_82090000)");
    for (int c = 0; c < 200; c++) {
      // 16 pointers into 16 random floats (some shared).
      const uint32_t floats = h.data + 0x200;
      uint32_t words[16], pointers[16];
      for (uint32_t i = 0; i < 16; i++) {
        words[i] = r.Below(8) ? OrdinaryFloat(r, 100.0) : RandomFloat(r);
        pointers[i] = floats + 4 * r.Below(16);
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        PPCContext t = AlignedContext(h);
        for (uint32_t i = 0; i < 16; i++) {
          guest::StoreU32(h.base, floats + 4 * i, words[i]);
        }
        for (uint32_t i = 7; i < 16; i++) {
          guest::StoreU32(h.base, t.r1.u32 + 84 + 8 * (i - 7), pointers[i]);
        }
        t.r3.u64 = h.data + 0x100;
        t.r4.u64 = pointers[0];
        t.r5.u64 = pointers[1];
        t.r6.u64 = pointers[2];
        t.r7.u64 = pointers[3];
        t.r8.u64 = pointers[4];
        t.r9.u64 = pointers[5];
        t.r10.u64 = pointers[6];
        (v == 0 ? Original<__imp__sub_82090000> : hooks::CameraModesStep82090000)(t, h.base);
        out[v].Floats(h.base, h.data + 0x100, 16);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The lists ---

void Halfword(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU16(base, address, uint16_t(value));
}

std::vector<uint32_t> Permutation(Random& r, uint32_t n) {
  std::vector<uint32_t> p(n);
  for (uint32_t i = 0; i < n; i++) {
    p[i] = i;
  }
  for (uint32_t i = n; i > 1; i--) {
    std::swap(p[i - 1], p[r.Below(i)]);
  }
  return p;
}

// An n-slot list in a random state with `count` nodes: nodes linked in a
// random order, the rest on the free list, the data slots a permutation.
void RandomList(uint8_t* base, uint32_t list, uint32_t n, uint32_t count, Random& r) {
  const std::vector<uint32_t> order = Permutation(r, n);
  const std::vector<uint32_t> slots = Permutation(r, n);
  for (uint32_t i = 0; i < n; i++) {
    const uint32_t node = order[i];
    const bool last = i + 1 == count || i + 1 == n;
    Halfword(base, list + 4 * node, last ? 0xFFFF : order[i + 1]);
    Halfword(base, list + 4 * node + 2, slots[node]);
  }
  Halfword(base, list + 4 * n, count < n ? order[count] : 0xFFFF);
  Halfword(base, list + 4 * n + 2, uint32_t(r.Next()));
  Halfword(base, list + 4 * n + 4, count > 0 ? order[0] : 0xFFFF);
  Halfword(base, list + 4 * n + 6, uint32_t(r.Next()));
  Halfword(base, list + 4 * n + 8, count);
}

void Lists(Context& h, Random& r) {
  enum Kind { kLink40, kInsert40, kInsert20, kInsert32, kInsert608, kCopy32 };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t n;
    Kind kind;
  } kFunctions[] = {
      {"List40::Link (sub_82153228)", Original<__imp__sub_82153228>,
       hooks::CameraModesStep82153228, 40, kLink40},
      {"List40::Insert (sub_821544E0)", Original<__imp__sub_821544E0>,
       hooks::CameraModesStep821544E0, 40, kInsert40},
      {"List16::Insert20 (sub_82154458)", Original<__imp__sub_82154458>,
       hooks::CameraModesStep82154458, 16, kInsert20},
      {"List16::Insert32 (sub_82154378)", Original<__imp__sub_82154378>,
       hooks::CameraModesStep82154378, 16, kInsert32},
      {"List16::Insert608 (sub_821543E8)", Original<__imp__sub_821543E8>,
       hooks::CameraModesStep821543E8, 16, kInsert608},
      {"List16::Copy32 (sub_82154300)", Original<__imp__sub_82154300>,
       hooks::CameraModesStep82154300, 16, kCopy32},
  };
  constexpr uint32_t kBytes = 0x4000;
  Block memory(kBytes);
  const uint32_t block = memory.address;
  const uint32_t list = block + 0x100;
  const uint32_t record = block + 0x3000;
  for (const auto& f : kFunctions) {
    Results results(f.name);
    for (int c = 0; c < 300; c++) {
      // Link takes a full list's free head too; the others' walks stay on
      // the list (a position up to the count, after the link).
      const uint32_t count = f.kind == kLink40   ? (r.Below(16) == 0 ? f.n - 1 : r.Below(f.n))
                             : f.kind == kCopy32 ? 1 + r.Below(f.n - 1)
                                                 : r.Below(f.n);
      const uint32_t limit = f.kind == kCopy32 ? count - 1 : count;
      const uint32_t position = r.Below(limit + 1);
      const uint64_t high = r.Below(4) == 0 ? (r.Next() << 32) : 0;
      RandomBytes(h.base, block, kBytes, r);
      RandomList(h.base, list, f.n, count, r);
      const std::vector<uint8_t> inputs = Read(h.base, block, kBytes);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, block, inputs.data(), kBytes);
        PPCContext t = CallContext(h);
        if (f.kind == kCopy32) {
          t.r3.u64 = record;
          t.r4.u64 = list;
          t.r5.u64 = high | position;
        } else {
          t.r3.u64 = list;
          t.r4.u64 = f.kind == kLink40 ? (high | position) : record;
          t.r5.u64 = high | position;
        }
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        out[v].Bytes(Read(h.base, block, kBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("count {} position {}", count, position));
    }
    results.Report();
  }
}

// --- The camera animations' table ---

void Anims(Context& h, Random& r) {
  constexpr uint32_t kTableBytes = cm::kAnimCount - cm::kAnims + 4;
  Saved saved(h.base, cm::kAnims, kTableBytes);
  {
    Results results("CameraAnims::Clear (sub_82150078)");
    for (int c = 0; c < 4; c++) {
      Outputs out[2];
      const uint32_t count = uint32_t(r.Next());
      for (int v = 0; v < 2; v++) {
        guest::StoreU32(h.base, cm::kAnimCount, count);
        PPCContext t = CallContext(h);
        (v == 0 ? Original<__imp__sub_82150078> : hooks::CameraModesStep82150078)(t, h.base);
        out[v].Word(guest::LoadU32(h.base, cm::kAnimCount));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("count {:X}", count));
    }
    results.Report();
  }
  {
    Results results("CameraAnims::Find (sub_82150300)");
    for (int c = 0; c < 200; c++) {
      const uint32_t count = r.Below(12);
      for (uint32_t i = 0; i < 12; i++) {
        guest::StoreU32(h.base, cm::kAnims + cm::kAnimBytes * i, 22824 + r.Below(16));
      }
      guest::StoreU32(h.base, cm::kAnimCount, count);
      const uint32_t id = 22824 + r.Below(18);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        PPCContext t = CallContext(h);
        t.r3.u64 = id;
        (v == 0 ? Original<__imp__sub_82150300> : hooks::CameraModesStep82150300)(t, h.base);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("count {} id {}", count, id));
    }
    results.Report();
  }
}

// --- The color filter and the fade cues ---

void Filter(Context& h, Random& r) {
  {
    Results results("ColorFilter::SetStyle (sub_82152108)");
    Saved saved(h.base, post_effects::kEffects + 8, 4);
    Block filter(8200);
    Block vtable(64);
    for (uint32_t i = 0; i < 16; i++) {
      guest::StoreU32(h.base, vtable.address + 4 * i, 0);
    }
    guest::StoreU32(h.base, vtable.address + 16, 0x821350F8);
    guest::StoreU32(h.base, vtable.address + 24, 0x82135138);
    guest::StoreU32(h.base, vtable.address + 28, 0x82135140);
    for (int c = 0; c < 60; c++) {
      const uint32_t style = c < 50 ? uint32_t(c % 5) : uint32_t(r.Next());
      const bool none = c % 13 == 12;
      RandomBytes(h.base, filter.address + 8064, 64, r);
      const std::vector<uint8_t> inputs = Read(h.base, filter.address + 8064, 64);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, filter.address + 8064, inputs.data(), 64);
        guest::StoreU32(h.base, filter.address, vtable.address);
        guest::StoreU32(h.base, post_effects::kEffects + 8, none ? 0 : filter.address);
        guest::StoreU32(h.base, h.data, style);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data;
        (v == 0 ? Original<__imp__sub_82152108> : hooks::CameraModesStep82152108)(t, h.base);
        out[v].Floats(h.base, filter.address + 8064, 16);
        saved.Restore();
      }
      results.Check(out[0], out[1], fmt::format("style {:X} none {}", style, none));
    }
    results.Report();
  }
  {
    Results results("FadeCue::Step (sub_82152290)");
    constexpr uint32_t kFades = 0x826A50A0;
    constexpr uint32_t kFadesBytes = 0x60;
    Saved saved(h.base, kFades, kFadesBytes);
    for (int c = 0; c < 300; c++) {
      const int32_t start = int32_t(r.Below(40)) - 5;
      const int32_t end = start + int32_t(r.Below(40)) - 5;
      const int32_t frame = r.Below(3) == 0 ? start : int32_t(r.Below(60)) - 5;
      const uint32_t cue[8] = {uint32_t(start), uint32_t(end), r.Below(4),
                               uint32_t(r.Below(256)), uint32_t(r.Below(256)),
                               uint32_t(r.Below(256)), uint32_t(frame),
                               r.Below(4) == 0 ? 1u : 0u};
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        for (uint32_t i = 0; i < 8; i++) {
          guest::StoreU32(h.base, h.data + 4 * i, cue[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data;
        (v == 0 ? Original<__imp__sub_82152290> : hooks::CameraModesStep82152290)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, 32));
        out[v].Bytes(Read(h.base, kFades, kFadesBytes));
        saved.Restore();
      }
      results.Check(out[0], out[1], fmt::format("start {} end {} frame {} kind {} hold {}",
                                                start, end, frame, cue[2], cue[7]));
    }
    results.Report();
  }
}

// --- The camera animations' files ---

// An animation file's object and its blocks, as the files hold them: an
// object's +0 kind, +4 its header's size (to its blocks), +8 its blocks'
// bytes, +12 their count, +16 to +36 words, +40 and +44 floats; a block's
// +0 kind, +4 its header's size (to its data), +8 the data's bytes, +12 and
// +16 floats.
struct FileBlock {
  uint32_t kind, header, bytes;
  uint32_t words[2];
  std::vector<uint8_t> rest;  // the header's bytes after +20, and the data
};
struct FileObject {
  uint32_t kind, header;
  uint32_t words[9];
  std::vector<uint8_t> rest;  // the header's bytes after +48
  std::vector<FileBlock> blocks;
};

uint32_t FloatOrWord(Random& r) {
  return r.Below(6) ? OrdinaryFloat(r, 100.0) : RandomFloat(r);
}

std::vector<FileObject> RandomFile(Random& r, uint32_t objects) {
  std::vector<FileObject> file(objects + 1);
  for (uint32_t i = 0; i <= objects; i++) {
    FileObject& o = file[i];
    o.kind = i == objects ? 14 : (r.Below(4) == 0 ? r.Below(20) : 1 + r.Below(7));
    if (o.kind == 14 && i != objects) {
      o.kind = 6;
    }
    o.header = 48 + 4 * r.Below(5);
    for (uint32_t& w : o.words) {
      w = uint32_t(r.Next());
    }
    o.words[7] = FloatOrWord(r);
    o.words[8] = FloatOrWord(r);
    o.rest.resize(o.header - 48);
    for (uint8_t& b : o.rest) {
      b = uint8_t(r.Next());
    }
    o.blocks.resize(r.Below(5));
    for (FileBlock& b : o.blocks) {
      b.kind = r.Below(8);
      b.header = 20 + 4 * r.Below(4);
      b.bytes = r.Below(4) == 0 ? r.Below(70) : 4 * r.Below(16);
      b.words[0] = FloatOrWord(r);
      b.words[1] = FloatOrWord(r);
      b.rest.resize(b.header - 20 + b.bytes);
      for (uint8_t& x : b.rest) {
        x = uint8_t(r.Next());
      }
    }
  }
  return file;
}

uint32_t FileBytes(const std::vector<FileObject>& file) {
  uint32_t bytes = 0;
  for (const FileObject& o : file) {
    bytes += o.header;
    for (const FileBlock& b : o.blocks) {
      bytes += b.header + b.bytes;
    }
  }
  return bytes;
}

void WriteFile(uint8_t* base, uint32_t at, const std::vector<FileObject>& file) {
  for (const FileObject& o : file) {
    uint32_t blocks = 0;
    for (const FileBlock& b : o.blocks) {
      blocks += b.header + b.bytes;
    }
    guest::StoreU32(base, at + 0, o.kind);
    guest::StoreU32(base, at + 4, o.header);
    guest::StoreU32(base, at + 8, blocks);
    guest::StoreU32(base, at + 12, uint32_t(o.blocks.size()));
    for (uint32_t k = 0; k < 9; k++) {
      guest::StoreU32(base, at + 16 + 4 * k, o.words[k]);
    }
    Write(base, at + 48, o.rest.data(), uint32_t(o.rest.size()));
    uint32_t b_at = at + o.header;
    for (const FileBlock& b : o.blocks) {
      guest::StoreU32(base, b_at + 0, b.kind);
      guest::StoreU32(base, b_at + 4, b.header);
      guest::StoreU32(base, b_at + 8, b.bytes);
      guest::StoreU32(base, b_at + 12, b.words[0]);
      guest::StoreU32(base, b_at + 16, b.words[1]);
      Write(base, b_at + 20, b.rest.data(), uint32_t(b.rest.size()));
      b_at += b.header + b.bytes;
    }
    at = b_at;
  }
}

void FreeIfPages(uint32_t address) {
  if (address != 0 && memory::IsPages(address)) {
    memory::FreePages(address);
  }
}

// A parsed object (CameraAnim::ParseObject's copy) into `out` by its
// contents (its blocks' and their data's, not their addresses), then freed.
void ParsedOut(Outputs& out, uint8_t* base, uint32_t object) {
  out.Word(memory::IsPages(object) ? 1 : 0);
  out.Bytes(Read(base, object, 40));
  out.Floats(base, object + 40, 2);
  out.Bytes(Read(base, object + 48, 32));
  const uint32_t count = guest::LoadU32(base, object + 336);
  out.Word(count);
  for (uint32_t i = 0; i < count && i < 64; i++) {
    const uint32_t block = guest::LoadU32(base, object + 80 + 4 * i);
    out.Word(memory::IsPages(block) ? 1 : 0);
    out.Bytes(Read(base, block, 12));
    out.Floats(base, block + 12, 2);
    out.Bytes(Read(base, block + 20, 40));
    const uint32_t data = guest::LoadU32(base, block + 60);
    const uint32_t bytes = guest::LoadU32(base, block + 8);
    out.Word(memory::IsPages(data) ? 1 : 0);
    out.Bytes(Read(base, data, bytes));
    FreeIfPages(data);
    FreeIfPages(block);
  }
  FreeIfPages(object);
}

// XMemCpy through the translated code from a frame like ParseObject's.
cm::GuestCopy GuestXMemCpy(PPCContext& t, uint8_t* base, uint32_t frame, uint32_t back) {
  return [&t, base, frame, back](uint32_t to, uint32_t from, uint32_t size) {
    guest_call::InFrame(t, base, frame, back, [&](uint32_t) {
      t.r3.u64 = to;
      t.r4.u64 = from;
      t.r5.u64 = size;
      sub_82295890(t, base);
    });
  };
}
cm::GuestCopy GuestMemcpy(PPCContext& t, uint8_t* base, uint32_t frame, uint32_t back) {
  return [&t, base, frame, back](uint32_t to, uint32_t from, uint32_t size) {
    guest_call::InFrame(t, base, frame, back, [&](uint32_t) {
      t.r3.u64 = to;
      t.r4.u64 = from;
      t.r5.u64 = size;
      sub_825A7410(t, base);
    });
  };
}

// CameraAnim::ParseObject (no adapter): r3 = the object; r3 = the copy.
void NativeParseObject(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = cm::AnimParseObject(base, t.r3.u32, GuestXMemCpy(t, base, 144, 0x821502B8));
}

constexpr uint32_t kTableBytes = cm::kAnimCount - cm::kAnims + 4;

// The stack below a case's stack pointer, the same for both versions (the
// unused slots CameraAnims::OnLoaded copies into the entry are its).
void FillStack(uint8_t* base, uint32_t r1, uint64_t seed) {
  Random s(seed);
  for (uint32_t i = 0; i < 1024; i += 4) {
    guest::StoreU32(base, r1 - 1024 + i, uint32_t(s.Next()));
  }
}

void AnimFiles(Context& h, Random& r) {
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue()) {
    ITC_LOG_WARN("harness: camera_modes: the camera animations' files need --native_heap and "
                 "the archive's queue native");
    return;
  }
  Saved saved(h.base, cm::kAnims, kTableBytes);
  {
    Results results("CameraAnim::ParseObject (sub_82150100)");
    for (int c = 0; c < 60; c++) {
      const std::vector<FileObject> file = RandomFile(r, 1);
      const uint32_t bytes = FileBytes(file);
      Block data(bytes + 16);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        WriteFile(h.base, data.address, file);
        PPCContext t = CallContext(h);
        t.r3.u64 = data.address;
        (v == 0 ? Original<__imp__sub_82150100> : NativeParseObject)(t, h.base);
        ParsedOut(out[v], h.base, t.r3.u32);
        out[v].Bytes(Read(h.base, data.address, bytes));
      }
      results.Check(out[0], out[1], fmt::format("case {}: {} blocks", c, file[0].blocks.size()));
    }
    results.Report();
  }
  {
    Results results("CameraAnims::OnLoaded (sub_82150878)");
    for (int c = 0; c < 60; c++) {
      const std::vector<FileObject> file = RandomFile(r, r.Below(6));
      const uint32_t bytes = FileBytes(file);
      const uint32_t id = 22824 + r.Below(40);
      const uint32_t count = r.Below(6);
      const uint64_t seed = r.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        const uint32_t data = memory::AllocPages(bytes, 4096);
        const uint32_t record = memory::AllocPages(4, 4096);
        WriteFile(h.base, data, file);
        guest::StoreU32(h.base, record, id);
        guest::StoreU32(h.base, cm::kAnimCount, count);
        PPCContext t = CallContext(h);
        FillStack(h.base, t.r1.u32, seed);
        t.r3.u64 = 0x1234;
        t.r4.u64 = data;
        t.r5.u64 = bytes;
        t.r6.u64 = record;
        (v == 0 ? Original<__imp__sub_82150878> : hooks::CameraModesStep82150878)(t, h.base);
        out[v].Word(guest::LoadU32(h.base, cm::kAnimCount));
        const uint32_t entry = cm::kAnims + cm::kAnimBytes * count;
        out[v].Word(guest::LoadU32(h.base, entry));
        const uint32_t objects = guest::LoadU32(h.base, entry + 260);
        out[v].Word(objects);
        out[v].Bytes(Read(h.base, entry + 4 + 4 * objects, 256 - 4 * objects));
        for (uint32_t i = 0; i < objects; i++) {
          ParsedOut(out[v], h.base, guest::LoadU32(h.base, entry + 4 + 4 * i));
        }
        out[v].Word(memory::IsPages(data) ? 1 : 0);
        out[v].Word(memory::IsPages(record) ? 1 : 0);
        out[v].Word(t.r3.u32);
        saved.Restore();
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: {} objects, count {}", c, file.size(), count));
    }
    results.Report();
  }
  {
    Results results("CameraAnims::Load (sub_82150B10)");
    for (int c = 0; c < 24; c++) {
      const uint32_t id = c < 16 ? 22824 + uint32_t(c) : r.Below(23000);
      std::vector<int64_t> serials;
      for (int i = 0; i < 4; i++) {
        serials.push_back(r.Below(5) == 0 ? -1 : int64_t(r.Below(0x7FFFFFFF)));
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        ArchiveLoads loads(serials);
        PPCContext t = CallContext(h);
        t.r3.u64 = id;
        (v == 0 ? Original<__imp__sub_82150B10> : hooks::CameraModesStep82150B10)(t, h.base);
        const std::vector<uint32_t>& calls = loads.calls();
        out[v].Word(uint32_t(calls.size()));
        for (size_t i = 0; i + 6 <= calls.size(); i += 6) {
          out[v].Word(calls[i + 0]);
          out[v].Word(memory::IsPages(calls[i + 1]) ? 1 : 0);
          out[v].Word(calls[i + 2]);
          out[v].Word(memory::IsPages(calls[i + 3]) ? 1 : 0);
          out[v].Word(guest::LoadU32(h.base, calls[i + 3]));
          out[v].Word(calls[i + 4]);
          out[v].Word(calls[i + 5]);
          FreeIfPages(calls[i + 1]);
          FreeIfPages(calls[i + 3]);
        }
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("id {}", id));
    }
    results.Report();
  }
}

// An object of `blocks` blocks in pages, each with data pages (or none).
// Returns the object; `pages` gets every address.
uint32_t PagesObject(uint8_t* base, Random& r, uint32_t blocks, std::vector<uint32_t>& pages) {
  const uint32_t object = memory::AllocPages(340, 4096);
  pages.push_back(object);
  guest::StoreU32(base, object + 336, blocks);
  for (uint32_t i = 0; i < blocks; i++) {
    const uint32_t block = memory::AllocPages(64, 4096);
    pages.push_back(block);
    guest::StoreU32(base, object + 80 + 4 * i, block);
    uint32_t data = 0;
    if (r.Below(4) != 0) {
      data = memory::AllocPages(16 + r.Below(64), 4096);
      pages.push_back(data);
    }
    guest::StoreU32(base, block + 60, data);
  }
  return object;
}

void NativeBlockFree(PPCContext& t, uint8_t* base) { cm::BlockFree(base, t.r3.u32); }
void NativeAnimFree(PPCContext& t, uint8_t* base) { cm::AnimFree(base, t.r3.u32); }

void AnimFrees(Context& h, Random& r) {
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_WARN("harness: camera_modes: the camera animations' frees need --native_heap");
    return;
  }
  Saved saved(h.base, cm::kAnims, kTableBytes);
  {
    Results results("CameraAnimBlock::Free (sub_8214FFF0)");
    for (int c = 0; c < 40; c++) {
      const uint64_t seed = r.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random s(seed);
        std::vector<uint32_t> pages;
        const uint32_t object = PagesObject(h.base, s, 1, pages);
        const uint32_t block = guest::LoadU32(h.base, object + 80);
        PPCContext t = CallContext(h);
        t.r3.u64 = block;
        (v == 0 ? Original<__imp__sub_8214FFF0> : NativeBlockFree)(t, h.base);
        out[v].Word(guest::LoadU32(h.base, block + 60));
        for (const uint32_t p : pages) {
          out[v].Word(memory::IsPages(p) ? 1 : 0);
        }
        for (const uint32_t p : pages) {
          if (memory::IsPages(p)) {
            memory::FreePages(p);
          }
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CameraAnim::Free (sub_82150098)");
    for (int c = 0; c < 40; c++) {
      const uint64_t seed = r.Next();
      const uint32_t blocks = r.Below(5);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random s(seed);
        std::vector<uint32_t> pages;
        const uint32_t object = PagesObject(h.base, s, blocks, pages);
        PPCContext t = CallContext(h);
        t.r3.u64 = object;
        (v == 0 ? Original<__imp__sub_82150098> : NativeAnimFree)(t, h.base);
        out[v].Word(guest::LoadU32(h.base, object + 336));
        for (const uint32_t p : pages) {
          out[v].Word(memory::IsPages(p) ? 1 : 0);
        }
        for (const uint32_t p : pages) {
          if (memory::IsPages(p)) {
            memory::FreePages(p);
          }
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}: {} blocks", c, blocks));
    }
    results.Report();
  }
  {
    Results results("CameraAnims::FreeAll (sub_821507D8)");
    for (int c = 0; c < 40; c++) {
      const uint64_t seed = r.Next();
      const uint32_t entries = r.Below(5);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random s(seed);
        std::vector<uint32_t> pages;
        for (uint32_t e = 0; e < entries; e++) {
          const uint32_t entry = cm::kAnims + cm::kAnimBytes * e;
          const uint32_t objects = s.Below(4);
          guest::StoreU32(h.base, entry, 22824 + e);
          guest::StoreU32(h.base, entry + 260, objects);
          for (uint32_t i = 0; i < objects; i++) {
            guest::StoreU32(h.base, entry + 4 + 4 * i, PagesObject(h.base, s, s.Below(4), pages));
          }
        }
        guest::StoreU32(h.base, cm::kAnimCount, entries);
        PPCContext t = CallContext(h);
        t.r3.u64 = 0x5678;
        (v == 0 ? Original<__imp__sub_821507D8> : hooks::CameraModesStep821507D8)(t, h.base);
        out[v].Word(guest::LoadU32(h.base, cm::kAnimCount));
        out[v].Word(uint32_t(pages.size()));
        for (const uint32_t p : pages) {
          out[v].Word(memory::IsPages(p) ? 1 : 0);
        }
        for (const uint32_t p : pages) {
          if (memory::IsPages(p)) {
            memory::FreePages(p);
          }
        }
        out[v].Word(t.r3.u32);
        saved.Restore();
      }
      results.Check(out[0], out[1], fmt::format("case {}: {} entries", c, entries));
    }
    results.Report();
  }
}

// --- The camera animations' evaluation ---

// The table's entries (ids 22824 up) over objects in one block: kinds 1, 6,
// 7, others and the end (14); blocks of the kinds the evaluations read (0 to
// 5, 101, 102, 109) and others, their data four frames of floats.
struct AnimWorld {
  explicit AnimWorld(uint8_t* base) : memory(0x10000), base(base) {}
  Block memory;
  uint8_t* base;
  uint32_t entries = 0;
  std::vector<uint32_t> objects;  // every object, for the per-object cases

  void Build(Random& r) {
    objects.clear();
    uint32_t at = memory.address;
    auto take = [&](uint32_t bytes) {
      const uint32_t a = at;
      at += (bytes + 15) & ~15u;
      return a;
    };
    entries = 1 + r.Below(4);
    for (uint32_t e = 0; e < entries; e++) {
      const uint32_t entry = cm::kAnims + cm::kAnimBytes * e;
      const uint32_t count = 1 + r.Below(5);
      guest::StoreU32(base, entry, 22824 + e);
      guest::StoreU32(base, entry + 260, count);
      for (uint32_t i = 0; i < count; i++) {
        static constexpr uint32_t kKinds[] = {1, 6, 7, 3, 6, 7, 1, 14};
        const uint32_t kind = i + 1 == count && r.Below(2) ? 14 : kKinds[r.Below(8)];
        const uint32_t object = take(340);
        objects.push_back(object);
        guest::StoreU32(base, entry + 4 + 4 * i, object);
        guest::StoreU32(base, object, kind);
        // Kinds 1 and 6 always have blocks (CameraAnims::Duration reads the
        // first one's +16 without a check).
        const uint32_t blocks = (kind == 1 || kind == 6 ? 1 : 0) + r.Below(10);
        guest::StoreU32(base, object + 336, blocks);
        for (uint32_t b = 0; b < blocks; b++) {
          static constexpr uint32_t kSubs[] = {0, 1, 2, 3, 4, 5, 101, 102, 109, 7, 200, 101, 102};
          const uint32_t sub = kSubs[r.Below(13)];
          const uint32_t block = take(64);
          const uint32_t data = take(16);
          guest::StoreU32(base, object + 80 + 4 * b, block);
          guest::StoreU32(base, block, sub);
          guest::StoreU32(base, block + 16, FloatOrWord(r));
          guest::StoreU32(base, block + 60, data);
          for (uint32_t f = 0; f < 4; f++) {
            uint32_t w = r.Below(6) ? OrdinaryFloat(r, 360.0) : RandomFloat(r);
            if ((sub == 101 || sub == 102) && r.Below(8)) {
              w = std::bit_cast<uint32_t>(float(5.0 + double(r.Below(60000)) / 1000.0));
            }
            guest::StoreU32(base, data + 4 * f, w);
          }
        }
      }
    }
    guest::StoreU32(base, cm::kAnimCount, entries);
  }
};

void NativeEvalCamera(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  if (const auto f1 = cm::AnimEvalCamera(base, t.r3.u32, t.r5.u32, t.r6.u32)) {
    t.f1.f64 = *f1;
    return;
  }
  __imp__sub_82150350(t, base);
}
void NativeEvalTarget(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  cm::AnimEvalTarget(base, t.r3.u32, t.r5.u32, t.r6.u32);
}
void NativeMatrix(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  cm::AnimMatrix(base, t.r3.u32, t.r5.u32, t.r6.u32, t.r1.u32 - 320,
                 GuestMemcpy(t, base, 320, 0x82150600));
}

void AnimEvaluation(Context& h, Random& r) {
  Saved saved(h.base, cm::kAnims, kTableBytes);
  AnimWorld world(h.base);
  enum Kind { kCamera, kTarget, kMatrix, kEvaluate, kDuration, kObjectMatrix };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
  } kFunctions[] = {
      {"CameraAnim::EvalCamera (sub_82150350)", Original<__imp__sub_82150350>, NativeEvalCamera,
       kCamera},
      {"CameraAnim::EvalTarget (sub_821504E0)", Original<__imp__sub_821504E0>, NativeEvalTarget,
       kTarget},
      {"CameraAnim::Matrix (sub_82150590)", Original<__imp__sub_82150590>, NativeMatrix, kMatrix},
      {"CameraAnims::Evaluate (sub_82150960)", Original<__imp__sub_82150960>,
       hooks::CameraModesStep82150960, kEvaluate},
      {"CameraAnims::Duration (sub_82150A98)", Original<__imp__sub_82150A98>,
       hooks::CameraModesStep82150A98, kDuration},
      {"CameraAnims::ObjectMatrix (sub_82150A10)", Original<__imp__sub_82150A10>,
       hooks::CameraModesStep82150A10, kObjectMatrix},
  };
  for (const auto& f : kFunctions) {
    Results results(f.name);
    for (int c = 0; c < 200; c++) {
      world.Build(r);
      const uint32_t object = world.objects[r.Below(uint32_t(world.objects.size()))];
      const uint32_t id = 22824 + (f.kind == kDuration ? r.Below(world.entries)
                                                       : r.Below(world.entries + 2));
      const uint32_t time = r.Below(4);
      const uint32_t out_at = h.data + 0x100 + (f.kind == kMatrix && r.Below(4) == 0 ? 4 : 0);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        PPCContext t = AlignedContext(h);
        t.f1.u64 = 0x4045000000000000ull;
        switch (f.kind) {
          case kCamera:
          case kTarget:
          case kMatrix:
            t.r3.u64 = object;
            t.r4.u64 = object;
            t.r5.u64 = out_at;
            t.r6.u64 = time;
            break;
          case kDuration:
            t.r3.u64 = id;
            break;
          default:
            t.r3.u64 = out_at;
            t.r4.u64 = id;
            t.r5.u64 = time;
            break;
        }
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        out[v].Floats(h.base, h.data + 0x100, 20);
        out[v].Double(t.f1.u64);
        // r3 as the adapters leave it (the three without adapters leave
        // theirs to no caller: Evaluate and ObjectMatrix set their own).
        if (f.kind != kCamera && f.kind != kTarget && f.kind != kMatrix) {
          out[v].Word(t.r3.u32);
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}: object kind {} id {} time {}", c,
                                                guest::LoadU32(h.base, object), id, time));
    }
    results.Report();
  }
}

// --- The camera paths ---

void NativeFixKeys(PPCContext& t, uint8_t* base) {
  cm::FixKeys(base, t.r3.u32, t.r4.u32, t.r5.u32);
}
void NativeFixKey(PPCContext& t, uint8_t* base) { cm::FixKey(base, t.r3.u32, t.r4.u32); }
void NativeFixGroup(PPCContext& t, uint8_t* base) { cm::FixGroup(base, t.r3.u32, t.r4.u32); }
void NativeFixTriple(PPCContext& t, uint8_t* base) { cm::FixTriple(base, t.r3.u32, t.r4.u32); }

// A random key's words: the (kind, value) pairs' kinds often 2.
void RandomKeyWords(uint8_t* base, uint32_t at, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 8) {
    static constexpr uint32_t kKinds[] = {0, 1, 2, 2, 2, 3, 0xFFFFFFFEu};
    guest::StoreU32(base, at + i, r.Below(5) ? kKinds[r.Below(7)] : uint32_t(r.Next()));
    guest::StoreU32(base, at + i + 4, r.Below(4) ? r.Below(500) : uint32_t(r.Next()));
  }
}

void Paths(Context& h, Random& r) {
  Block memory(0x8000);
  const uint32_t data = memory.address;
  {
    const struct {
      const char* name;
      GuestFunction recompiled, native;
      uint32_t bytes;
    } kPieces[] = {
        {"CameraPath::FixTriple (sub_820ABE68)", Original<__imp__sub_820ABE68>,
         NativeFixTriple, 24},
        {"CameraPath::FixGroup (sub_820ABED8)", Original<__imp__sub_820ABED8>, NativeFixGroup, 96},
        {"CameraPath::FixKey (sub_820ABF80)", Original<__imp__sub_820ABF80>, NativeFixKey, 216},
    };
    for (const auto& f : kPieces) {
      Results results(f.name);
      for (int c = 0; c < 100; c++) {
        RandomKeyWords(h.base, data, 512, r);
        const std::vector<uint8_t> inputs = Read(h.base, data, 512);
        const uint32_t curves = uint32_t(r.Next());
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          Write(h.base, data, inputs.data(), 512);
          PPCContext t = CallContext(h);
          t.r3.u64 = data + 8;
          t.r4.u64 = curves;
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          out[v].Bytes(Read(h.base, data, 512));
        }
        results.Check(out[0], out[1], fmt::format("case {}", c));
      }
      results.Report();
    }
  }
  // A paths file: +0 the paths' count, +4 the keys', the 20-byte paths
  // (+8 their key count, +16 their first key's index) at +16, the keys.
  auto write_file = [&](uint32_t paths, uint32_t keys) {
    RandomKeyWords(h.base, data, 0x7000, r);
    guest::StoreU32(h.base, data + 0, paths);
    guest::StoreU32(h.base, data + 4, keys);
    uint32_t first = 0;
    for (uint32_t i = 0; i < paths; i++) {
      const uint32_t path = data + 16 + 20 * i;
      const uint32_t count = r.Below(4) == 0 ? 0 : 1 + r.Below(4);
      const uint32_t at = r.Below(8) == 0 ? r.Below(keys + 1) : first;
      guest::StoreU32(h.base, path + 0, uint32_t(r.Next()));
      guest::StoreU32(h.base, path + 4, uint32_t(r.Next()));
      guest::StoreU32(h.base, path + 8, std::min(count, keys - std::min(at, keys)));
      guest::StoreU32(h.base, path + 12, uint32_t(r.Next()));
      guest::StoreU32(h.base, path + 16, at);
      first = std::min(keys, at + count);
    }
  };
  {
    Results results("CameraPath::FixKeys (sub_820AC000)");
    for (int c = 0; c < 100; c++) {
      write_file(1, 1 + r.Below(12));
      const std::vector<uint8_t> inputs = Read(h.base, data, 0x7000);
      const uint32_t keys = data + 16 + 20;
      const uint32_t curves = uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, data, inputs.data(), 0x7000);
        PPCContext t = CallContext(h);
        t.r3.u64 = data + 16;
        t.r4.u64 = keys;
        t.r5.u64 = curves;
        (v == 0 ? Original<__imp__sub_820AC000> : NativeFixKeys)(t, h.base);
        out[v].Bytes(Read(h.base, data, 0x7000));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CameraPath::OnLoaded (sub_82152078)");
    Block buffer(32);
    for (int c = 0; c < 100; c++) {
      const uint32_t paths = c == 0 ? 0 : r.Below(8);
      write_file(paths, r.Below(40));
      const std::vector<uint8_t> inputs = Read(h.base, data, 0x7000);
      RandomBytes(h.base, buffer.address, 32, r);
      const std::vector<uint8_t> buffer_inputs = Read(h.base, buffer.address, 32);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, data, inputs.data(), 0x7000);
        Write(h.base, buffer.address, buffer_inputs.data(), 32);
        PPCContext t = CallContext(h);
        t.r4.u64 = data;
        t.r6.u64 = buffer.address;
        (v == 0 ? Original<__imp__sub_82152078> : hooks::CameraModesStep82152078)(t, h.base);
        out[v].Bytes(Read(h.base, data, 0x7000));
        out[v].Bytes(Read(h.base, buffer.address, 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}: {} paths", c, paths));
    }
    results.Report();
  }
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue()) {
    ITC_LOG_WARN("harness: camera_modes: CameraPath::Request needs --native_heap and the "
                 "archive's queue native");
    return;
  }
  {
    Results results("CameraPath::Request (sub_82152E08)");
    // The indices whose asset pointer is in the image.
    std::vector<uint32_t> indices;
    for (uint32_t i = 0; i < 128; i++) {
      const uint32_t pointer = guest::LoadU32(h.base, cm::kPathAssets + 4 * i);
      if (pointer < 0x82000000u || pointer >= 0x83000000u ||
          guest::LoadU32(h.base, pointer) >= 26000) {
        break;
      }
      indices.push_back(i);
    }
    ITC_LOG_INFO("harness: camera_modes: {} camera path assets", indices.size());
    if (indices.empty()) {
      ITC_LOG_WARN("harness: camera_modes: no camera path assets found at 0x82618D30");
      return;
    }
    Block buffer(32);
    for (int c = 0; c < 60; c++) {
      const uint32_t index =
          r.Below(6) == 0 ? 0xFFFFFFFFu : indices[r.Below(uint32_t(indices.size()))];
      const uint32_t state = r.Below(5);
      const bool block = r.Below(3) != 0;
      uint32_t words[6];
      for (uint32_t& w : words) {
        w = uint32_t(r.Next());
      }
      std::vector<int64_t> serials{r.Below(5) == 0 ? -1 : int64_t(r.Below(0x7FFFFFFF))};
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        const uint32_t old = block ? memory::AllocPages(64, 4096) : 0;
        for (uint32_t i = 0; i < 5; i++) {
          guest::StoreU32(h.base, buffer.address + 4 * i, words[i]);
        }
        guest::StoreU32(h.base, buffer.address + 20, old);
        guest::StoreU32(h.base, buffer.address + 24, state);
        ArchiveLoads loads(serials);
        PPCContext t = CallContext(h);
        t.r3.u64 = 0x9ABC;
        t.r4.u64 = buffer.address;
        t.r5.u64 = index;
        (v == 0 ? Original<__imp__sub_82152E08> : hooks::CameraModesStep82152E08)(t, h.base);
        const uint32_t now = guest::LoadU32(h.base, buffer.address + 20);
        out[v].Bytes(Read(h.base, buffer.address, 20));
        out[v].Word(guest::LoadU32(h.base, buffer.address + 24));
        out[v].Word(now == old ? 1 : 0);
        out[v].Word(old != 0 && memory::IsPages(old) ? 1 : 0);
        out[v].Word(now != 0 && memory::IsPages(now) ? 1 : 0);
        const std::vector<uint32_t>& calls = loads.calls();
        out[v].Word(uint32_t(calls.size()));
        for (size_t i = 0; i + 6 <= calls.size(); i += 6) {
          out[v].Word(calls[i + 0]);
          out[v].Word(calls[i + 1] == now ? 1 : 0);
          out[v].Word(calls[i + 2]);
          out[v].Word(calls[i + 3] == buffer.address ? 1 : 0);
          out[v].Word(calls[i + 4]);
          out[v].Word(calls[i + 5]);
        }
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        if (old != 0 && memory::IsPages(old)) {
          memory::FreePages(old);
        }
        if (now != 0 && now != old && memory::IsPages(now)) {
          memory::FreePages(now);
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}: index {:X} state {} block {}", c,
                                                index, state, block));
    }
    results.Report();
  }
}

// --- The rival logo ---

void RivalLogo(Context& h, Random& r) {
  {
    Results results("RivalLogo::RivalLogo (sub_82211848)");
    for (int c = 0; c < 20; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes, uint8_t(c));
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + 0x100;
        (v == 0 ? Original<__imp__sub_82211848> : hooks::CameraModesStep82211848)(t, h.base);
        out[v].Bytes(Read(h.base, h.data + 0x100, 0x80));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  if (ITC_FLAG(log_pause_movie)) {
    return;
  }
  {
    // A movie table of four movies (ids 10 to 13, sorted) in one block,
    // their loaded flags (+0x9FD0) and textures (+28) random; the named
    // animation negative (PlayNamed's own cases are ui_movies_game's).
    Results results("RivalLogo::Play (sub_82211AB0)");
    constexpr uint32_t kMovieBytes = 0xA000;
    Block movies(4 * kMovieBytes + 0x200);
    const uint32_t pairs = movies.address + 4 * kMovieBytes;
    const uint32_t textures = pairs + 0x100;
    Saved table(h.base, ui_movies::kMovieTable, 20);
    for (int c = 0; c < 200; c++) {
      uint32_t loaded[4];
      for (uint32_t& w : loaded) {
        w = r.Below(2) ? 0 : uint32_t(r.Next());
      }
      uint32_t entry_words[24];
      for (uint32_t& w : entry_words) {
        w = uint32_t(r.Next());
      }
      entry_words[0] = r.Below(2) ? 0 : r.Below(3);                  // the logo
      entry_words[1] = r.Below(4) ? (entry_words[1] | 4) : entry_words[1];
      entry_words[2] = 10 + r.Below(4);                               // the movie
      entry_words[3] = 0x80000000u | uint32_t(r.Next());              // the named animation
      entry_words[21] = r.Below(8);                                   // +84, +88: textures
      entry_words[22] = r.Below(8);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        for (uint32_t m = 0; m < 4; m++) {
          const uint32_t movie = movies.address + kMovieBytes * m;
          guest::StoreU32(h.base, pairs + 8 * m, 10 + m);
          guest::StoreU32(h.base, pairs + 8 * m + 4, movie);
          guest::StoreU32(h.base, movie + 28, textures);
          guest::StoreU32(h.base, movie + 0x9FD0, loaded[m]);
        }
        for (uint32_t i = 0; i < 8; i++) {
          guest::StoreU32(h.base, textures + 4 * i, 0x1000 + i);
        }
        guest::StoreU32(h.base, ui_movies::kMovieTable + 0, pairs);
        guest::StoreU32(h.base, ui_movies::kMovieTable + 4, 4);
        guest::StoreU32(h.base, ui_movies::kMovieTable + 8, 4);
        guest::StoreU32(h.base, ui_movies::kMovieTable + 16, movies.address);
        const uint32_t entry = h.data + 0x100;
        for (uint32_t i = 0; i < 24; i++) {
          guest::StoreU32(h.base, entry + 4 * i, entry_words[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = entry;
        (v == 0 ? Original<__imp__sub_82211AB0> : hooks::CameraModesStep82211AB0)(t, h.base);
        out[v].Bytes(Read(h.base, entry, 96));
        out[v].Bytes(Read(h.base, textures, 32));
        out[v].Word(t.r3.u32);
        table.Restore();
      }
      results.Check(out[0], out[1], fmt::format("case {}: logo {} flags {:X} movie {}", c,
                                                entry_words[0], entry_words[1], entry_words[2]));
    }
    results.Report();
  }
}

// --- The views (batch 3): the target tracker, the shake and the viewports'
// moves, the intro camera, the director's start and release, the cutscene's
// shots ---

constexpr uint32_t kSeed = 0x82638188;        // rand's seed
constexpr uint32_t kShakeStatics = 0x826D81C0;  // the shake's step and its guard
constexpr uint32_t kShotStatics = 0x826D81D0;   // MakeShot's two vectors and their guard
constexpr uint32_t kShotStaticsBytes = 0x24;

uint32_t ViewFloat(Random& r) {
  return r.Below(8) ? OrdinaryFloat(r, 200.0) : RandomFloat(r);
}
void RandomFloats(uint8_t* base, uint32_t address, uint32_t words, Random& r) {
  for (uint32_t i = 0; i < words; i++) {
    guest::StoreU32(base, address + 4 * i, ViewFloat(r));
  }
}

// The three without adapters: their plain C++ as their callers call it.
void NativeShakeRandom(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  cm::ShakeRandom(base, t.r3.u32, t.f1.f64, t.r1.u32 - 160);
}
void NativeOffset(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  cm::ViewportOffset(base, t.r3.u32, t.f1.f64, t.r5.u32, t.r1.u32);
}
void NativeMakeShot(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  hooks::GuestAtexit atexit{t, base};
  cm::IntroMakeShot(base, t.r3.u32, t.r4.u32, t.r5.u32, t.r1.u32 - 192, atexit.Call());
}

void Track(Context& h, Random& r) {
  Results results("CameraTarget::Track (sub_82153290)");
  Block tracker(512), vehicle(6272), body(512);
  for (int c = 0; c < 200; c++) {
    RandomFloats(h.base, tracker.address, 56, r);
    RandomBytes(h.base, tracker.address + 224, 96, r);
    RandomFloats(h.base, vehicle.address + 1216, 4, r);
    RandomFloats(h.base, vehicle.address + 1584, 4, r);
    RandomBytes(h.base, vehicle.address + 5232, 96, r);
    guest::StoreU32(h.base, vehicle.address + 336, body.address);
    RandomFloats(h.base, body.address + 80, 4, r);
    RandomFloats(h.base, body.address + 288, 16, r);
    const uint32_t the_vehicle = c % 20 == 0 ? 0 : vehicle.address;
    const auto inputs = Read(h.base, tracker.address, 320);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, tracker.address, inputs.data(), 320);
      PPCContext t = AlignedContext(h);
      t.r3.u64 = tracker.address;
      t.r4.u64 = the_vehicle;
      (v == 0 ? Original<__imp__sub_82153290> : hooks::CameraModesStep82153290)(t, h.base);
      out[v].Floats(h.base, tracker.address, 56);
      out[v].Bytes(Read(h.base, tracker.address + 224, 96));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {} vehicle {:X}", c, the_vehicle));
  }
  results.Report();
}

void Shakes(Context& h, Random& r) {
  enum Kind { kShake, kShakeRandom, kOffset, kPullBack };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
  } kFunctions[] = {
      {"Camera::Shake (sub_82152CD8)", Original<__imp__sub_82152CD8>,
       hooks::CameraModesStep82152CD8, kShake},
      {"Camera::ShakeRandom (sub_82152BD8)", Original<__imp__sub_82152BD8>, NativeShakeRandom,
       kShakeRandom},
      {"Viewport::Offset (sub_821529B0)", Original<__imp__sub_821529B0>, NativeOffset, kOffset},
      {"Viewport::PullBack (sub_82152788)", Original<__imp__sub_82152788>,
       hooks::CameraModesStep82152788, kPullBack},
  };
  Block viewport(kViewportBytes), camera(64), vehicle(64);
  Saved seed(h.base, kSeed, 4);
  Saved statics(h.base, kShakeStatics, 8);
  for (const auto& f : kFunctions) {
    Results results(f.name);
    for (int c = 0; c < 200; c++) {
      RandomFloats(h.base, viewport.address + 1600, 16, r);
      if (c % 10 == 1) {  // the eye on the target: a zero-length direction
        for (uint32_t i = 0; i < 4; i++) {
          guest::StoreU32(h.base, viewport.address + 1616 + 4 * i,
                          guest::LoadU32(h.base, viewport.address + 1600 + 4 * i));
        }
      }
      const auto inputs = Read(h.base, viewport.address + 1600, 64);
      const uint32_t offset[4] = {ViewFloat(r), ViewFloat(r), ViewFloat(r), ViewFloat(r)};
      const double amount = r.Below(10) ? (double(r.Below(2001)) - 500.0) / 1000.0
                                        : double(std::bit_cast<float>(RandomFloat(r)));
      const uint32_t speed = r.Below(10) ? OrdinaryFloat(r, 300.0) : RandomFloat(r);
      const uint32_t seed_value = uint32_t(r.Next());
      const uint32_t step = ViewFloat(r);
      const uint32_t guard = uint32_t(r.Next()) & ~(r.Below(2) ? 1u : 0u);
      const bool aligned = r.Below(4) != 0;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Write(h.base, viewport.address + 1600, inputs.data(), 64);
        for (uint32_t i = 0; i < 4; i++) {
          guest::StoreU32(h.base, h.data + 0x100 + 4 * i, offset[i]);
        }
        guest::StoreU32(h.base, kSeed, seed_value);
        guest::StoreU32(h.base, kShakeStatics, step);
        guest::StoreU32(h.base, kShakeStatics + 4, guard);
        guest::StoreU32(h.base, vehicle.address + 48, speed);
        guest::StoreU32(h.base, camera.address + 16, vehicle.address);
        guest::StoreU32(h.base, camera.address + 20, viewport.address);
        PPCContext t = aligned ? AlignedContext(h) : CallContext(h);
        if (!aligned) {
          t.r1.u64 = (t.r1.u64 & ~0xFull) - 8;
        }
        // The stack below: an unaligned frame's vector loads read words the
        // function didn't write.
        FillStack(h.base, t.r1.u32, uint64_t(c) + 1);
        t.r3.u64 = f.kind == kShake ? camera.address : viewport.address;
        t.f1.f64 = amount;
        t.r5.u64 = h.data + 0x100;
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        out[v].Floats(h.base, viewport.address + 1600, 16);
        out[v].Word(guest::LoadU32(h.base, kSeed));
        out[v].Bytes(Read(h.base, kShakeStatics, 8));
        if (f.kind == kShake || f.kind == kPullBack) {
          out[v].Word(t.r3.u32);
        }
        seed.Restore();
        statics.Restore();
      }
      results.Check(out[0], out[1], fmt::format("case {} amount {} aligned {}", c, amount,
                                                aligned));
    }
    results.Report();
  }
}

void IntroShots(Context& h, Random& r) {
  {
    Results results("IntroCamera::StepShot (sub_82152E78)");
    constexpr uint32_t kPaused = 0x826A50D4;
    Saved paused(h.base, kPaused, 4);
    Block viewport(kViewportBytes), vehicle(512), body(512);
    for (int c = 0; c < 200; c++) {
      RandomFloats(h.base, viewport.address + 1600, 40, r);
      const auto inputs = Read(h.base, viewport.address + 1600, 160);
      guest::StoreU32(h.base, vehicle.address + 336, body.address);
      RandomFloats(h.base, body.address + 288, 16, r);
      const uint32_t shot[5] = {viewport.address, vehicle.address, r.Below(40),
                                uint32_t(r.Next()), r.Below(3) ? 0 : r.Below(5)};
      const uint32_t paused_value = r.Below(4) == 0 ? 1 : 0;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Write(h.base, viewport.address + 1600, inputs.data(), 160);
        for (uint32_t i = 0; i < 5; i++) {
          guest::StoreU32(h.base, h.data + 4 * i, shot[i]);
        }
        guest::StoreU32(h.base, kPaused, paused_value);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data;
        (v == 0 ? Original<__imp__sub_82152E78> : hooks::CameraModesStep82152E78)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, 20));
        out[v].Floats(h.base, viewport.address + 1600, 40);
        out[v].Word(t.r3.u32);
        paused.Restore();
      }
      results.Check(out[0], out[1], fmt::format("case {} count {} phase {}", c, shot[2],
                                                shot[4]));
    }
    results.Report();
  }
  if (!hooks::NativeAtexit()) {
    ITC_LOG_WARN("harness: camera_modes: native atexit off: IntroCamera's cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(c_runtime::ExitMutex());
  Saved statics(h.base, kShotStatics, kShotStaticsBytes);
  // A car's spec: the vehicle's +5712 (an object), its +1888.
  auto spec_words = [&](uint8_t* base, uint32_t object) {
    const uint32_t spec = object + 1888;
    for (uint32_t at : {76u, 80u, 88u}) {
      const uint32_t w = r.Below(10) ? r.Below(6000) : uint32_t(r.Next());
      guest::StoreU32(base, spec + at, w);
    }
  };
  {
    Results results("IntroCamera::MakeShot (sub_82154108)");
    Block viewport(kViewportBytes), vehicle(6272), object(2048);
    for (int c = 0; c < 60; c++) {
      guest::StoreU32(h.base, vehicle.address + 5712, object.address);
      spec_words(h.base, object.address);
      RandomFloats(h.base, viewport.address + 1680, 20, r);
      const auto inputs = Read(h.base, viewport.address + 1680, 80);
      std::vector<uint8_t> stat = Read(h.base, kShotStatics, kShotStaticsBytes);
      if (c < 8) {
        RandomBytes(h.base, kShotStatics, kShotStaticsBytes, r);
        stat = Read(h.base, kShotStatics, kShotStaticsBytes);
        stat[0x23] &= uint8_t(~(1u + uint32_t(c % 3 == 0 ? 2 : 0) * uint32_t(c % 2)));
        if (c == 0) {
          stat[0x23] &= ~3u;
        }
      }
      const uint32_t word = uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Write(h.base, viewport.address + 1680, inputs.data(), 80);
        Write(h.base, kShotStatics, stat.data(), kShotStaticsBytes);
        const uint32_t mark = exit_table::Mark(h.base);
        PPCContext t = AlignedContext(h);
        t.r3.u64 = h.data;
        t.r4.u64 = viewport.address;
        t.r5.u64 = vehicle.address;
        (void)word;
        (v == 0 ? Original<__imp__sub_82154108> : NativeMakeShot)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, 12));
        out[v].Floats(h.base, h.data + 12, 1);
        out[v].Bytes(Read(h.base, h.data + 16, 4));
        out[v].Floats(h.base, viewport.address + 1680, 20);
        out[v].Bytes(Read(h.base, kShotStatics, kShotStaticsBytes));
        exit_table::Take(h.base, mark, out[v]);
        statics.Restore();
      }
      results.Check(out[0], out[1], fmt::format("case {} guard {:X}", c, stat[0x23]));
    }
    results.Report();
  }
  {
    Results results("IntroCamera::IntroCamera (sub_82158F70)");
    constexpr uint32_t kManager = 0x826D82F4;
    constexpr uint32_t kVehicleArray = 0x826C2E34;
    constexpr uint32_t kTables = 0x826C5CA0;
    Saved saved_race(h.base, cm::kRaceData, 4);
    Saved saved_manager(h.base, kManager, 4);
    Saved saved_vehicles(h.base, kVehicleArray, 4);
    Saved saved_tables(h.base, kTables, 4);
    Block race(kRaceBytes), manager(2400), entries(6 * 32), vehicles(7 * 6272),
        objects(7 * 2048), tables(32), items(4 * 3008), camera(768), viewport(kViewportBytes);
    Fill(h.base, race.address, race.bytes, 0);
    for (int c = 0; c < 120; c++) {
      // The racers: ids 0-5, their records' flags and items; the race's
      // racer entries; the stage's list (up to four).
      for (uint32_t id = 0; id < 6; id++) {
        const uint32_t record = race.address + (id + 1) * kRecordBytes;
        guest::StoreU32(h.base, record, r.Below(3) ? (uint32_t(r.Next()) | 2) : 0);
        guest::StoreU32(h.base, record + 24, r.Below(3) ? r.Below(4) : 0xFFFFFFFFu);
      }
      // Every racer has its entry (one without reads vehicle 0's +5712, as
      // the original does: never in play).
      const uint32_t racers = 6;
      guest::StoreU32(h.base, manager.address + 2364, racers);
      const std::vector<uint32_t> ids = Permutation(r, 6);
      for (uint32_t n = 0; n < 6; n++) {
        const uint32_t e = entries.address + 32 * n;
        guest::StoreU32(h.base, manager.address + 2308 + 4 * n, e);
        guest::StoreU32(h.base, e + 0, r.Below(2) ? 0x400 : uint32_t(r.Next()) & ~0x400u);
        guest::StoreU32(h.base, e + 20, r.Below(7));
        guest::StoreU32(h.base, e + 24, ids[n]);
      }
      const uint32_t stages = race.address + 0x59620;
      const uint32_t stage = r.Below(3);
      guest::StoreU32(h.base, stages + 804, stage);
      const uint32_t at = stages + 80 * stage;
      const uint32_t count = r.Below(5);
      guest::StoreU32(h.base, at + 76, count);
      for (uint32_t s = 0; s < 4; s++) {
        guest::StoreU32(h.base, at + 44 + 8 * s, r.Below(6));
      }
      for (uint32_t k = 0; k < 7; k++) {
        guest::StoreU32(h.base, vehicles.address + 6272 * k + 5712, objects.address + 2048 * k);
        spec_words(h.base, objects.address + 2048 * k);
      }
      guest::StoreU32(h.base, tables.address + 16, items.address);
      for (uint32_t k = 0; k < 4; k++) {
        guest::StoreU16(h.base, items.address + 3008 * k + 88, uint16_t(r.Below(5)));
      }
      RandomFloats(h.base, viewport.address + 1680, 20, r);
      const auto inputs = Read(h.base, viewport.address + 1680, 80);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Fill(h.base, camera.address, camera.bytes);
        Write(h.base, viewport.address + 1680, inputs.data(), 80);
        guest::StoreU32(h.base, cm::kRaceData, race.address);
        guest::StoreU32(h.base, kManager, manager.address);
        guest::StoreU32(h.base, kVehicleArray, vehicles.address);
        guest::StoreU32(h.base, kTables, tables.address);
        const uint32_t mark = exit_table::Mark(h.base);
        PPCContext t = AlignedContext(h);
        t.r3.u64 = camera.address;
        t.r4.u64 = viewport.address;
        t.r5.u64 = vehicles.address + 6272;
        (v == 0 ? Original<__imp__sub_82158F70> : hooks::CameraModesStep82158F70)(t, h.base);
        out[v].Bytes(Read(h.base, camera.address, 760));
        out[v].Floats(h.base, viewport.address + 1680, 20);
        out[v].Bytes(Read(h.base, kShotStatics, kShotStaticsBytes));
        exit_table::Take(h.base, mark, out[v]);
        out[v].Word(t.r3.u32);
        saved_race.Restore();
        saved_manager.Restore();
        saved_vehicles.Restore();
        saved_tables.Restore();
        statics.Restore();
      }
      results.Check(out[0], out[1], fmt::format("case {}: stage {} count {} racers {}", c, stage,
                                                count, racers));
    }
    results.Report();
  }
}

void DirectorStartRelease(Context& h, Random& r) {
  {
    Results results("CameraDirector::Start (sub_82152338)");
    if (!hooks::NativeHeapActive() || !hooks::NativeAtexit()) {
      ITC_LOG_WARN("harness: camera_modes: --native_heap or native atexit off: "
                   "CameraDirector::Start's cases skipped");
    } else {
      std::lock_guard<std::recursive_mutex> lock(c_runtime::ExitMutex());
      constexpr uint32_t kNear = 0x826BAC00 + 384;
      Saved saved_viewports(h.base, cm::kViewports, 4);
      Saved saved_near(h.base, kNear, 4);
      Saved saved_count(h.base, cm::kAnimCount, 4);
      // Viewport::Init's three vectors and their guard (0x826C2D20).
      Saved saved_vectors(h.base, 0x826C2CF0, 0x34);
      Block director(kDirectorBytes);
      for (int c = 0; c < 8; c++) {
        const uint32_t words[3] = {uint32_t(r.Next()), uint32_t(r.Next()), uint32_t(r.Next())};
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          guest::StoreU32(h.base, director.address + cm::kFollowed, words[0]);
          guest::StoreU32(h.base, director.address + 13348, words[1]);
          guest::StoreU32(h.base, director.address + cm::kFollowedInput, words[2]);
          guest::StoreU32(h.base, cm::kAnimCount, 5);
          const uint32_t mark = exit_table::Mark(h.base);
          PPCContext t = CallContext(h);
          t.r3.u64 = director.address;
          (v == 0 ? Original<__imp__sub_82152338> : hooks::CameraModesStep82152338)(t, h.base);
          const uint32_t block = guest::LoadU32(h.base, cm::kViewports);
          const uint32_t first = guest::LoadU32(h.base, block);
          const uint32_t second = guest::LoadU32(h.base, block + 4);
          out[v].Word(guest::LoadU32(h.base, director.address + cm::kFollowed));
          out[v].Word(guest::LoadU32(h.base, director.address + 13348));
          out[v].Word(guest::LoadU32(h.base, director.address + cm::kFollowedInput));
          out[v].Word(guest::LoadU32(h.base, block + 8));
          out[v].Bytes(Read(h.base, first, kViewportBytes));
          out[v].Bytes(Read(h.base, second, kViewportBytes));
          out[v].Word(guest::LoadU32(h.base, kNear));
          out[v].Word(guest::LoadU32(h.base, cm::kAnimCount));
          exit_table::Take(h.base, mark, out[v]);
          memory::FreePages(first);
          memory::FreePages(second);
          memory::FreePages(block);
          saved_viewports.Restore();
          saved_near.Restore();
          saved_count.Restore();
          saved_vectors.Restore();
        }
        results.Check(out[0], out[1], fmt::format("case {}", c));
      }
    }
    results.Report();
  }
  {
    Results results("CameraDirector::Release (sub_82153098)");
    if (!hooks::NativeHeapActive()) {
      ITC_LOG_WARN("harness: camera_modes: --native_heap off: CameraDirector::Release's cases "
                   "skipped");
    } else {
      constexpr uint32_t kSaveData = 0x826C5B58;
      Saved saved_viewports(h.base, cm::kViewports, 4);
      Saved saved_save(h.base, kSaveData, 4);
      Saved saved_race(h.base, cm::kRaceData, 4);
      Saved saved_count(h.base, cm::kAnimCount, 4);
      Block director(kDirectorBytes), save(0x1F900), race(kRaceBytes);
      for (int c = 0; c < 100; c++) {
        const uint32_t view = r.Below(7), kept = r.Below(7), saved_view = uint32_t(r.Next());
        const uint32_t setting = uint32_t(r.Next());
        const uint32_t kind = r.Below(6);
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          guest::StoreU32(h.base, director.address + cm::kView, view);
          guest::StoreU32(h.base, director.address + cm::kViewKept, kept);
          guest::StoreU32(h.base, director.address + 9624, saved_view);
          guest::StoreU32(h.base, save.address + 0x1F85C + 24, setting);
          guest::StoreU32(h.base, race.address + 0xA3AF0, kind);
          guest::StoreU32(h.base, kSaveData, save.address);
          guest::StoreU32(h.base, cm::kRaceData, race.address);
          guest::StoreU32(h.base, cm::kAnimCount, 0);
          const uint32_t block = memory::AllocPages(12, 4096);
          guest::StoreU32(h.base, block + 0, memory::AllocPages(kViewportBytes, 4096));
          guest::StoreU32(h.base, block + 4, memory::AllocPages(kViewportBytes, 4096));
          guest::StoreU32(h.base, block + 8, 0);
          guest::StoreU32(h.base, cm::kViewports, block);
          PPCContext t = CallContext(h);
          t.r3.u64 = director.address;
          (v == 0 ? Original<__imp__sub_82153098> : hooks::CameraModesStep82153098)(t, h.base);
          out[v].Word(guest::LoadU32(h.base, director.address + 9624));
          out[v].Word(guest::LoadU32(h.base, save.address + 0x1F85C + 24));
          out[v].Word(guest::LoadU32(h.base, cm::kViewports));
          out[v].Word(memory::IsPages(block) ? 1 : 0);
          out[v].Word(t.r3.u32);
          saved_viewports.Restore();
          saved_save.Restore();
          saved_race.Restore();
          saved_count.Restore();
        }
        results.Check(out[0], out[1], fmt::format("case {}: view {} kept {} kind {}", c, view,
                                                  kept, kind));
      }
    }
    results.Report();
  }
}

void Cutscenes(Context& h, Random& r) {
  if (hooks::NativeHeapActive() && hooks::NativeArchiveQueue()) {
    Results results("CutsceneShot::Init (sub_82156B68)");
    Block shot(640), entry(640);
    for (int c = 0; c < 100; c++) {
      RandomBytes(h.base, entry.address, entry.bytes, r);
      const uint32_t kind = r.Below(10) ? r.Below(16) : r.Below(20);
      guest::StoreU32(h.base, entry.address, kind);
      RandomList(h.base, entry.address + 8, 16, r.Below(17), r);
      const auto inputs = Read(h.base, entry.address, entry.bytes);
      std::vector<int64_t> serials;
      for (int i = 0; i < 4; i++) {
        serials.push_back(r.Below(5) == 0 ? -1 : int64_t(r.Below(0x7FFFFFFF)));
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Fill(h.base, shot.address, shot.bytes);
        Write(h.base, entry.address, inputs.data(), entry.bytes);
        ArchiveLoads loads(serials);
        PPCContext t = CallContext(h);
        t.r3.u64 = shot.address;
        t.r4.u64 = entry.address;
        (v == 0 ? Original<__imp__sub_82156B68> : hooks::CameraModesStep82156B68)(t, h.base);
        out[v].Bytes(Read(h.base, shot.address, 608));
        const std::vector<uint32_t>& calls = loads.calls();
        out[v].Word(uint32_t(calls.size()));
        for (size_t i = 0; i + 6 <= calls.size(); i += 6) {
          out[v].Word(calls[i + 0]);
          out[v].Word(calls[i + 2]);
          out[v].Word(guest::LoadU32(h.base, calls[i + 3]));
          out[v].Word(calls[i + 4]);
          out[v].Word(calls[i + 5]);
          FreeIfPages(calls[i + 1]);
          FreeIfPages(calls[i + 3]);
        }
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {} kind {}", c, kind));
    }
    results.Report();
  } else {
    ITC_LOG_WARN("harness: camera_modes: --native_heap or the archive's queue off: "
                 "CutsceneShot::Init's cases skipped");
  }
  {
    Results results("CutsceneShot::Step (sub_82155800)");
    constexpr uint32_t kFades = 0x826A50A0;
    constexpr uint32_t kFadesBytes = 0x60;
    Saved saved_fades(h.base, kFades, kFadesBytes);
    Saved saved_table(h.base, cm::kAnims, kTableBytes);
    AnimWorld world(h.base);
    Block shot(640), vehicle(512), body(512), out_block(128);
    for (int c = 0; c < 200; c++) {
      world.Build(r);
      RandomBytes(h.base, shot.address, shot.bytes, r);
      guest::StoreU32(h.base, shot.address + 4, 22824 + r.Below(world.entries + 2));
      guest::StoreU32(h.base, shot.address + 8, vehicle.address);
      const uint32_t list = shot.address + 12;
      RandomList(h.base, list, 16, r.Below(5), r);
      for (uint32_t s = 0; s < 16; s++) {
        const uint32_t cue = list + 76 + 32 * s;
        const int32_t start = int32_t(r.Below(40)) - 5;
        guest::StoreU32(h.base, cue + 0, uint32_t(start));
        guest::StoreU32(h.base, cue + 4, uint32_t(start + int32_t(r.Below(40)) - 5));
        guest::StoreU32(h.base, cue + 8, r.Below(4));
        guest::StoreU32(h.base, cue + 24, r.Below(3) == 0 ? uint32_t(start) : r.Below(60));
        guest::StoreU32(h.base, cue + 28, r.Below(4) == 0 ? 1 : 0);
      }
      guest::StoreU32(h.base, shot.address + 600, r.Below(4));
      guest::StoreU32(h.base, shot.address + 604, uint32_t(r.Next()));
      guest::StoreU32(h.base, vehicle.address + 336, body.address);
      RandomFloats(h.base, body.address + 288, 16, r);
      const auto inputs = Read(h.base, shot.address, shot.bytes);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Fill(h.base, out_block.address, out_block.bytes);
        Write(h.base, shot.address, inputs.data(), shot.bytes);
        PPCContext t = AlignedContext(h);
        // The stack below: Step's frame words Evaluate doesn't write (no
        // camera or target object) are read as they are.
        FillStack(h.base, t.r1.u32, uint64_t(c) + 1);
        t.r3.u64 = shot.address;
        t.r4.u64 = out_block.address;
        t.f1.u64 = 0x4045000000000000ull;
        (v == 0 ? Original<__imp__sub_82155800> : hooks::CameraModesStep82155800)(t, h.base);
        out[v].Bytes(Read(h.base, shot.address, 12));
        out[v].Bytes(Read(h.base, list, 76));
        for (uint32_t s = 0; s < 16; s++) {
          out[v].Bytes(Read(h.base, list + 76 + 32 * s, 12));
          out[v].Bytes(Read(h.base, list + 76 + 32 * s + 24, 8));
        }
        out[v].Bytes(Read(h.base, shot.address + 600, 8));
        out[v].Floats(h.base, out_block.address, 32);
        out[v].Bytes(Read(h.base, kFades, kFadesBytes));
        out[v].Double(t.f1.u64);
        out[v].Word(t.r3.u32);
        saved_fades.Restore();
      }
      results.Check(out[0], out[1], fmt::format("case {} id {}", c,
                                                guest::LoadU32(h.base, shot.address + 4)));
    }
    results.Report();
  }
}


// --- The TV camera's route and paths ---

constexpr uint32_t kRouteGlobal = 0x826C5B10;
constexpr uint32_t kSliceCountGlobal = 0x82603274;
// SplinePoint's static: the matrix (0x826C2D30) and its guard (+64).
constexpr uint32_t kSplineStatic = 0x826C2D30;
constexpr uint32_t kSplineStaticBytes = 68;
constexpr uint32_t kRouteArea = 0x3000;

// The stack below a case's stack pointer, `bytes` of it, the same for both.
void FillStackBytes(uint8_t* base, uint32_t r1, uint64_t seed, uint32_t bytes) {
  Random s(seed);
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, r1 - bytes + i, uint32_t(s.Next()));
  }
}

// A route (test_route.h) whose every section has a forward link (0 to 2):
// TrackRoute::SplinePoint's walk to the slices after a position doesn't
// end at a section without one.
test_route::Route PathRoute(uint8_t* base, uint32_t area, uint32_t table, Random& r) {
  RandomBytes(base, area, kRouteArea, r);
  const test_route::Route route = test_route::BuildRoute(base, area, table, r);
  for (uint32_t s = 0; s < 8; s++) {
    const uint32_t at = route.address[s];
    bool any = false;
    for (uint32_t i = 0; i < 3; i++) {
      any = any || guest::LoadU16(base, at + 28 + 4 * i) != 0xFFFF;
    }
    if (!any) {
      guest::StoreU16(base, at + 28 + 4 * r.Below(3), uint16_t(r.Below(route.sections)));
    }
  }
  // Mostly no NaN or infinite points (the slices' centers, the first three
  // words of their 84-byte records at +124, and the edge points after them), so the spline's
  // results aren't mostly NaNs.
  if (r.Below(4) != 0) {
    for (uint32_t s = 0; s < 8; s++) {
      const uint32_t at = route.address[s];
      const uint32_t n = route.slices[s];
      for (uint32_t k = 0; k < n; k++) {
        for (uint32_t i = 0; i < 3; i++) {
          guest::StoreU32(base, at + 124 + 84 * k + 4 * i, OrdinaryFloat(r, 100.0));
        }
      }
      for (uint32_t i = 0; i < 24 * n + 48; i += 4) {
        guest::StoreU32(base, at + 124 + 84 * n + i, OrdinaryFloat(r, 100.0));
      }
    }
  }
  return route;
}

// A position packed as the originals pass it (section high, slice low).
uint32_t PackedPosition(uint8_t* base, uint32_t scratch, const test_route::Route& route,
                        Random& r) {
  test_route::RandomPosition(base, scratch, route, r);
  return guest::LoadU32(base, scratch);
}

// A valid 64-slot list at `list` (track_zones.h's layout) of `count` nodes
// in a random order of slots, its data words sections of the route mostly.
void PathList(uint8_t* base, uint32_t list, uint32_t count, const test_route::Route& route,
              Random& r) {
  uint32_t order[64];
  for (uint32_t i = 0; i < 64; i++) {
    order[i] = i;
  }
  for (uint32_t i = 64; i > 1; i--) {
    std::swap(order[i - 1], order[r.Below(i)]);
  }
  for (uint32_t i = 0; i < 64; i++) {
    const uint32_t node = order[i];
    guest::StoreU16(base, list + 4 * node, i + 1 < count ? order[i + 1] : 0xFFFF);
    guest::StoreU16(base, list + 4 * node + 2, order[(i + 7) % 64]);
    guest::StoreU32(base, list + 268 + 4 * i,
                    r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(route.sections));
  }
  guest::StoreU16(base, list + 256, count < 64 ? order[count] : 0xFFFF);
  guest::StoreU16(base, list + 260, count > 0 ? order[0] : 0xFFFF);
  guest::StoreU16(base, list + 264, count);
}

// A route range (track_zones.h): the state, the ends, the list of sections
// to take (+12), an empty list to leave out (+536), +1060. With `bounded`,
// the end on the start's section (RouteRange::OnPath's walk then ends at
// once, whatever the links).
void PathRange(uint8_t* base, uint32_t range, uint32_t scratch, const test_route::Route& route,
               Random& r, bool bounded) {
  RandomBytes(base, range, 1064, r);
  const uint32_t states[] = {1, 1, 1, 2, 0};
  guest::StoreU32(base, range, states[r.Below(5)]);
  guest::StoreU32(base, range + 4, PackedPosition(base, scratch, route, r));
  guest::StoreU32(base, range + 8, PackedPosition(base, scratch, route, r));
  if (bounded) {
    guest::StoreU16(base, range + 8, guest::LoadU16(base, range + 4));
  }
  PathList(base, range + 12, r.Below(4) == 0 ? r.Below(64) : r.Below(4), route, r);
  PathList(base, range + 536, 0, route, r);
  guest::StoreU32(base, range + 1060, r.Below(4) == 0 ? 0 : 1);
}

// A curve at `curve`: 2 to 7 keys (times rising from about 0, kinds 0 to 2
// mostly, now and then any), the end key (kind 3), then keys with NaN times
// (any kind) that stop the search past the end. `game_like`: times from 0
// and the keys before the end of kinds 0 to 2, as in the game's paths: no
// segment of kind 3 or more, whose value is the stack's word at +80 as the
// callers' frames left it (under CameraPath::Evaluate, the saved registers
// the original's prologues write there and the native code doesn't).
void PathCurve(uint8_t* base, uint32_t curve, Random& r, bool game_like = false) {
  const uint32_t keys = 1 + r.Below(6);
  float time = r.Below(4) == 0 && !game_like ? -float(r.Below(10)) : 0.0f;
  for (uint32_t k = 0; k <= keys + 4; k++) {
    const uint32_t key = curve + 24 * k;
    for (uint32_t i = 0; i < 24; i += 4) {
      guest::StoreU32(base, key + i, r.Below(8) ? OrdinaryFloat(r, 100.0) : RandomFloat(r));
    }
    if (k <= keys) {
      guest::StoreU32(base, key + 0, r.Below(32) == 0 && !game_like
                                         ? RandomFloat(r)
                                         : std::bit_cast<uint32_t>(time));
      time += float(r.Below(60)) / 4.0f;
      guest::StoreU32(base, key + 20,
                      k == keys ? 3 : (r.Below(16) == 0 && !game_like ? r.Below(8) : r.Below(3)));
    } else {
      guest::StoreU32(base, key + 0, 0x7FC00000u);
      guest::StoreU32(base, key + 20, r.Below(8));
    }
  }
}

// A (kind, value) pair at `pair`: kind 0 a float, 1 a curve (at `curve`),
// else another kind; now and then any word.
void PathPair(uint8_t* base, uint32_t pair, uint32_t curve, Random& r, bool game_like = false) {
  const uint32_t kind = r.Below(16) == 0 ? uint32_t(r.Next()) : r.Below(3);
  guest::StoreU32(base, pair, kind);
  if (kind == 1) {
    PathCurve(base, curve, r, game_like);
    guest::StoreU32(base, pair + 4, curve);
  } else {
    guest::StoreU32(base, pair + 4, r.Below(8) ? OrdinaryFloat(r, 100.0) : RandomFloat(r));
  }
}

uint64_t PathTime(Random& r) {
  switch (r.Below(16)) {
    case 0: return std::bit_cast<uint64_t>(double(std::bit_cast<float>(RandomFloat(r))));
    case 1: return std::bit_cast<uint64_t>(-0.25);
    case 2: return std::bit_cast<uint64_t>(1.5);
    case 3: return std::bit_cast<uint64_t>(0.0);
    case 4: return std::bit_cast<uint64_t>(1.0);
    default: return std::bit_cast<uint64_t>(double(float(r.Below(1u << 20)) / float(1u << 20)));
  }
}

// The functions without adapters: their plain C++ as their callers call it.
void NativeHermite(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  cm::HermiteBasis(base, t.r3.u32, t.r4.u32, t.r5.u32, t.f1.f64);
}
void NativeCurve(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.f1.f64 = cm::CurveEvaluate(base, t.r4.u32, t.f1.f64, t.r1.u32);
}
void NativeKeyValue(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.f1.f64 = cm::KeyValue(base, t.r3.u32, t.f1.f64, t.r1.u32);
}
void NativeKeyVec3(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  cm::KeyVec3(base, t.r3.u32, t.r4.u32, t.f1.f64, t.r1.u32);
}
void NativeFromRows3(PPCContext& t, uint8_t* base) {
  cm::MatrixFromRows3(base, t.r3.u32, t.r4.u32, t.r5.u32, t.r6.u32);
}
void NativeAdvance(PPCContext& t, uint8_t* base) {
  cm::RangeAdvance(base, t.r3.u32, t.r4.u32, t.r5.s32);
}
void NativeSplinePoint(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  hooks::GuestAtexit atexit{t, base};
  cm::SplinePoint(base, t.r3.u32, t.r4.u32, t.r5.u32, t.f1.f64, t.r1.u32, atexit.Call());
}
void NativeRoutePoint(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  hooks::GuestAtexit atexit{t, base};
  cm::RoutePoint(base, t.r3.u32, t.r4.u32, t.r6.u32, t.f1.f64, t.r1.u32, atexit.Call());
}

void RoutePaths(Context& h, Random& r) {
  const uint32_t area = memory::Alloc(kRouteArea, 0);
  const uint32_t table = memory::Alloc(4 * test_route::kTableEntries, 0);
  test_route::FillTable(h.base, table, area);
  Saved saved_route(h.base, kRouteGlobal, 4);
  Saved saved_count(h.base, kSliceCountGlobal, 4);
  Block work(0x8000);
  const uint32_t w = work.address;
  const uint32_t scratch = w + 0x7FF0;
  {
    Results results("Hermite::Basis (sub_82293A48)");
    for (int c = 0; c < 300; c++) {
      RandomBytes(h.base, w, 64, r);
      for (uint32_t i = 0; i < 8; i++) {
        guest::StoreU32(h.base, w + 16 + 4 * i, r.Below(8) ? OrdinaryFloat(r, 50.0) : RandomFloat(r));
      }
      const uint64_t time = PathTime(r);
      const auto inputs = Read(h.base, w, 64);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, w, inputs.data(), 64);
        PPCContext t = AlignedContext(h);
        t.r3.u64 = w;
        t.r4.u64 = w + 16;
        t.r5.u64 = w + 32;
        t.f1.u64 = time;
        (v == 0 ? Original<__imp__sub_82293A48> : NativeHermite)(t, h.base);
        out[v].Floats(h.base, w, 2);
        out[v].Bytes(Read(h.base, w + 8, 56));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // Curve::Evaluate, Key::Value and Key::Vec3 on curves at w + 0x1000.
    const struct {
      const char* name;
      GuestFunction recompiled, native;
    } kFunctions[] = {
        {"Curve::Evaluate (sub_820AC060)", Original<__imp__sub_820AC060>, NativeCurve},
        {"Key::Value (sub_820AC178)", Original<__imp__sub_820AC178>, NativeKeyValue},
        {"Key::Vec3 (sub_820AC1D0)", Original<__imp__sub_820AC1D0>, NativeKeyVec3},
    };
    for (int f = 0; f < 3; f++) {
      Results results(kFunctions[f].name);
      for (int c = 0; c < 400; c++) {
        RandomBytes(h.base, w, 0x1000, r);
        uint32_t curve = w + 0x1000;
        if (f == 0) {
          PathCurve(h.base, curve, r);
        } else {
          for (uint32_t p = 0; p < 3; p++) {
            PathPair(h.base, w + 8 * p, curve + 0x400 * p, r);
          }
        }
        const uint64_t time = PathTime(r);
        const uint64_t seed = r.Next();
        const auto inputs = Read(h.base, w, 0x2000);
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          Write(h.base, w, inputs.data(), 0x2000);
          PPCContext t = AlignedContext(h);
          FillStackBytes(h.base, t.r1.u32, seed, 1024);
          t.r3.u64 = w;
          t.r4.u64 = f == 0 ? curve : w + 64;
          t.f1.u64 = time;
          (v == 0 ? kFunctions[f].recompiled : kFunctions[f].native)(t, h.base);
          if (f == 2) {
            out[v].Floats(h.base, w + 64, 4);
          } else {
            out[v].Double(t.f1.u64);
          }
          out[v].Bytes(Read(h.base, w, 64));
        }
        results.Check(out[0], out[1], fmt::format("case {}", c));
      }
      results.Report();
    }
  }
  {
    Results results("Matrix::FromRows3 (sub_820AB5F0)");
    for (int c = 0; c < 100; c++) {
      RandomBytes(h.base, w, 256, r);
      const uint32_t at = w + 64 + (c % 4 == 0 ? 4 * r.Below(4) : 0);
      const auto inputs = Read(h.base, w, 256);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, w, inputs.data(), 256);
        PPCContext t = AlignedContext(h);
        t.r3.u64 = at;
        t.r4.u64 = w + 160;
        t.r5.u64 = w + 180;
        t.r6.u64 = w + 200;
        (v == 0 ? Original<__imp__sub_820AB5F0> : NativeFromRows3)(t, h.base);
        out[v].Bytes(Read(h.base, w, 256));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("RouteRange::Advance (sub_820FE598)");
    const uint32_t range = w + 0x100;
    for (int c = 0; c < 600; c++) {
      const test_route::Route route = PathRoute(h.base, area, table, r);
      guest::StoreU32(h.base, kRouteGlobal, area + test_route::kFile);
      PathRange(h.base, range, scratch, route, r, false);
      const int32_t distance = r.Below(16) == 0 ? -int32_t(r.Below(100)) : int32_t(r.Below(120));
      const auto inputs = Read(h.base, area, kRouteArea);
      const auto range_bytes = Read(h.base, range, 1064);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, area, inputs.data(), kRouteArea);
        Write(h.base, range, range_bytes.data(), 1064);
        guest::StoreU32(h.base, w, uint32_t(r.Next()));
        PPCContext t = AlignedContext(h);
        t.r3.u64 = w;
        t.r4.u64 = range;
        t.r5.s64 = distance;
        (v == 0 ? Original<__imp__sub_820FE598> : NativeAdvance)(t, h.base);
        out[v].Bytes(Read(h.base, w, 4));
        out[v].Bytes(Read(h.base, range, 1064));
      }
      results.Check(out[0], out[1], fmt::format("case {} distance {}", c, distance));
    }
    results.Report();
  }
  {
    Results results("TrackRoute::SliceFraction (sub_820F9B48)");
    for (int c = 0; c < 400; c++) {
      const test_route::Route route = PathRoute(h.base, area, table, r);
      guest::StoreU32(h.base, kRouteGlobal, area + test_route::kFile);
      test_route::RandomPosition(h.base, w, route, r);
      for (uint32_t i = 0; i < 4; i++) {
        guest::StoreU32(h.base, w + 16 + 4 * i, r.Below(8) ? OrdinaryFloat(r, 100.0) : RandomFloat(r));
      }
      const uint64_t seed = r.Next();
      const auto inputs = Read(h.base, area, kRouteArea);
      const auto words = Read(h.base, w, 32);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, area, inputs.data(), kRouteArea);
        Write(h.base, w, words.data(), 32);
        PPCContext t = AlignedContext(h);
        FillStackBytes(h.base, t.r1.u32, seed, 1024);
        t.r3.u64 = w;
        t.r4.u64 = w + 16;
        (v == 0 ? Original<__imp__sub_820F9B48> : hooks::CameraModesStep820F9B48)(t, h.base);
        out[v].Double(t.f1.u64);
        out[v].Bytes(Read(h.base, w, 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // TrackRoute::SplinePoint, CameraPath::RoutePoint and Evaluate: the
    // static matrix made in a third of the cases (its guard cleared), its
    // atexit taken from the exit table.
    std::lock_guard<std::recursive_mutex> lock(c_runtime::ExitMutex());
    Saved statics(h.base, kSplineStatic, kSplineStaticBytes);
    const auto made = Read(h.base, kSplineStatic, kSplineStaticBytes);
    const struct {
      const char* name;
      GuestFunction recompiled, native;
      int kind;
    } kFunctions[] = {
        {"TrackRoute::SplinePoint (sub_820AB6A0)", Original<__imp__sub_820AB6A0>,
         NativeSplinePoint, 0},
        {"CameraPath::RoutePoint (sub_820ABDA8)", Original<__imp__sub_820ABDA8>,
         NativeRoutePoint, 1},
        {"CameraPath::Evaluate (sub_820AC248)", Original<__imp__sub_820AC248>,
         hooks::CameraModesStep820AC248, 2},
    };
    if (!hooks::NativeAtexit()) {
      ITC_LOG_WARN("harness: camera_modes: TrackRoute::SplinePoint's static needs native atexit");
    }
    for (const auto& f : kFunctions) {
      Results results(f.name);
      const int cases = f.kind == 2 ? 600 : 400;
      for (int c = 0; c < cases; c++) {
        const test_route::Route route = PathRoute(h.base, area, table, r);
        RandomBytes(h.base, w, 0x6000, r);
        const uint32_t range = w + 0x100;
        const uint32_t transform = w + 0x600;
        const uint32_t key = w + 0x700;
        const uint32_t curves = w + 0x1000;
        guest::StoreU32(h.base, kRouteGlobal, area + test_route::kFile);
        PathRange(h.base, range, scratch, route, r, false);
        for (uint32_t i = 0; i < 20; i++) {
          guest::StoreU32(h.base, transform + 4 * i,
                          r.Below(16) ? OrdinaryFloat(r, i < 4 ? 500.0 : 1.5) : RandomFloat(r));
        }
        for (uint32_t p = 0; p < 27; p++) {
          PathPair(h.base, key + 8 * p, curves + 0x200 * p, r, true);
        }
        // The weights and distances of kind 0 often (the paths' usual).
        for (const uint32_t at : {72u, 104u, 112u, 168u, 200u, 208u}) {
          if (r.Below(2)) {
            guest::StoreU32(h.base, key + at, 0);
            const bool weight = at != 112 && at != 208;
            guest::StoreU32(h.base, key + at + 4,
                            r.Below(4) == 0 ? 0u
                            : weight ? std::bit_cast<uint32_t>(float(r.Below(5)) / 4.0f)
                                     : std::bit_cast<uint32_t>(float(r.Below(400)) / 10.0f - 5.0f));
          }
        }
        const uint32_t position = PackedPosition(h.base, scratch, route, r);
        const uint64_t time = PathTime(r);
        auto statics_in = made;
        if (c % 3 == 0) {
          statics_in[67] &= 0xFE;
        }
        const uint64_t seed = r.Next();
        const uint32_t junk = uint32_t(r.Next());
        const auto route_bytes = Read(h.base, area, kRouteArea);
        const auto inputs = Read(h.base, w, 0x6000);
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          Write(h.base, area, route_bytes.data(), kRouteArea);
          Write(h.base, w, inputs.data(), 0x6000);
          Write(h.base, kSplineStatic, statics_in.data(), kSplineStaticBytes);
          const uint32_t mark = exit_table::Mark(h.base);
          PPCContext t = AlignedContext(h);
          FillStackBytes(h.base, t.r1.u32, seed, 2048);
          t.f1.u64 = time;
          if (f.kind == 0) {
            t.r3.u64 = w;
            t.r4.u64 = w + 0x40;
            t.r5.u64 = position;
          } else if (f.kind == 1) {
            t.r3.u64 = w;
            t.r4.u64 = position;
            t.r5.u64 = junk;
            t.r6.u64 = w + 0x40;
          } else {
            t.r3.u64 = key;
            t.r4.u64 = w;
            t.r5.u64 = range;
            t.r6.u64 = transform;
          }
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          if (f.kind == 0) {
            out[v].Floats(h.base, w, 3);
            out[v].Bytes(Read(h.base, w + 12, 4));
            out[v].Floats(h.base, w + 0x40, 16);
            out[v].Bytes(Read(h.base, t.r1.u32 + 36, 4));
          } else if (f.kind == 1) {
            out[v].Floats(h.base, w, 3);
            out[v].Bytes(Read(h.base, w + 12, 0x40));
          } else {
            out[v].Bytes(Read(h.base, w, 16));
            out[v].Floats(h.base, w + 16, 8);
            out[v].Bytes(Read(h.base, w + 48, 16));
            out[v].Floats(h.base, w + 64, 2);
            out[v].Bytes(Read(h.base, w + 72, 8));
            out[v].Bytes(Read(h.base, range, 1064));
          }
          out[v].Bytes(Read(h.base, kSplineStatic + 64, 4));
          out[v].Floats(h.base, kSplineStatic, 16);
          exit_table::Take(h.base, mark, out[v]);
          statics.Restore();
        }
        results.Check(out[0], out[1], fmt::format("case {} guard {:X}", c, statics_in[67]));
      }
      results.Report();
    }
  }
  {
    // RouteRange::CameraAt on ranges whose Test ends (the end on the start's
    // section, or the state 0).
    Results results("RouteRange::CameraAt (sub_820FE830)");
    const uint32_t range = w + 0x100;
    for (int c = 0; c < 400; c++) {
      const test_route::Route route = PathRoute(h.base, area, table, r);
      guest::StoreU32(h.base, kRouteGlobal, area + test_route::kFile);
      PathRange(h.base, range, scratch, route, r, true);
      if (r.Below(3) == 0) {
        guest::StoreU32(h.base, w, guest::LoadU32(h.base, range + 4));
        guest::StoreU16(h.base, w + 2, guest::LoadU16(h.base, range + 6) + r.Below(5) - 2);
      } else {
        test_route::RandomPosition(h.base, w, route, r);
      }
      const uint32_t count = uint32_t(r.Next());
      const auto inputs = Read(h.base, area, kRouteArea);
      const auto range_bytes = Read(h.base, range, 1064);
      const auto io = Read(h.base, w, 16);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, area, inputs.data(), kRouteArea);
        Write(h.base, range, range_bytes.data(), 1064);
        Write(h.base, w, io.data(), 16);
        guest::StoreU32(h.base, kSliceCountGlobal, count);
        PPCContext t = AlignedContext(h);
        t.r3.u64 = range;
        t.r4.u64 = w;
        (v == 0 ? Original<__imp__sub_820FE830> : hooks::CameraModesStep820FE830)(t, h.base);
        out[v].Bytes(Read(h.base, range, 1064));
        out[v].Word(guest::LoadU32(h.base, kSliceCountGlobal));
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // TvCamera::Track: up to 40 ranges (their Tests ending, as above), the
    // four path buffers loading (+24 1 or 2, or another word) so none is
    // asked for: CameraPath::Request's loads are the attract race's.
    Results results("TvCamera::Track (sub_82156980)");
    Block camera(43136);
    const uint32_t cam = camera.address;
    for (int c = 0; c < 150; c++) {
      const test_route::Route route = PathRoute(h.base, area, table, r);
      guest::StoreU32(h.base, kRouteGlobal, area + test_route::kFile);
      RandomBytes(h.base, cam, 456, r);
      const uint32_t ranges = c % 10 == 0 ? 0 : 1 + r.Below(40);
      guest::StoreU32(h.base, cam + 452, ranges);
      for (uint32_t i = 0; i < ranges; i++) {
        PathRange(h.base, cam + 456 + 1064 * i, scratch, route, r, true);
      }
      for (uint32_t k = 0; k < 4; k++) {
        const uint32_t buffer = cam + 43016 + 28 * k;
        RandomBytes(h.base, buffer, 28, r);
        guest::StoreU32(h.base, buffer + 12, r.Below(4) ? r.Below(ranges + 1) : uint32_t(r.Next()));
        const uint32_t states[] = {1, 2, 4, 0x10000};
        guest::StoreU32(h.base, buffer + 24, states[r.Below(4)]);
      }
      if (r.Below(2) && ranges > 0) {
        guest::StoreU32(h.base, w, guest::LoadU32(h.base, cam + 456 + 1064 * r.Below(ranges) + 4));
      } else {
        test_route::RandomPosition(h.base, w, route, r);
      }
      const uint32_t count = uint32_t(r.Next());
      const auto inputs = Read(h.base, area, kRouteArea);
      const auto cam_bytes = Read(h.base, cam, 43136);
      const auto io = Read(h.base, w, 16);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, area, inputs.data(), kRouteArea);
        Write(h.base, cam, cam_bytes.data(), 43136);
        Write(h.base, w, io.data(), 16);
        guest::StoreU32(h.base, kSliceCountGlobal, count);
        PPCContext t = AlignedContext(h);
        t.r3.u64 = cam;
        t.r4.u64 = w;
        (v == 0 ? Original<__imp__sub_82156980> : hooks::CameraModesStep82156980)(t, h.base);
        out[v].Bytes(Read(h.base, cam, 43136));
        out[v].Word(guest::LoadU32(h.base, kSliceCountGlobal));
      }
      results.Check(out[0], out[1], fmt::format("case {} ranges {}", c, ranges));
    }
    results.Report();
  }
  memory::Free(table);
  memory::Free(area);
}

// --- The views' cameras (batch 5): the chase camera ---

constexpr uint32_t kChaseStatics = 0x826D8200;  // its two vectors, guard, pull and speed
constexpr uint32_t kChaseStaticsBytes = 0x2C;
constexpr uint32_t kPassNear = 0x826BAC00 + 384;

// A chase camera's world: the camera (+16 the vehicle, +20 the viewport),
// the viewport, the vehicle with its body and spec object.
struct ChaseWorld {
  Block camera{1152}, viewport{kViewportBytes}, vehicle{6272}, body{512}, object{2048};
  Block vectors{128};
  void Randomize(uint8_t* base, Random& r) {
    RandomFloats(base, camera.address + 32, 88, r);
    RandomBytes(base, camera.address + 256, 96, r);
    RandomFloats(base, camera.address + 352, 200, r);
    guest::StoreU32(base, camera.address + 372, r.Below(6));
    guest::StoreU32(base, camera.address + 376, uint32_t(r.Next()));
    guest::StoreU32(base, camera.address + 380, r.Below(2) ? 0 : uint32_t(r.Next()) | 1);
    RandomBytes(base, viewport.address, kViewportBytes, r);
    RandomFloats(base, viewport.address + 1600, 48, r);
    guest::StoreU32(base, viewport.address + 1856, OrdinaryFloat(r, 4.0));
    guest::StoreU32(base, viewport.address + 1864, r.Below(8) ? OrdinaryFloat(r, 4.0)
                                                               : RandomFloat(r));
    guest::StoreU32(base, viewport.address + 1868, r.Below(8) ? OrdinaryFloat(r, 4.0)
                                                               : RandomFloat(r));
    RandomFloats(base, vehicle.address + 1216, 4, r);
    RandomFloats(base, vehicle.address + 1584, 4, r);
    RandomBytes(base, vehicle.address + 5232, 96, r);
    guest::StoreU32(base, vehicle.address + 48, r.Below(10) ? OrdinaryFloat(r, 90.0)
                                                            : RandomFloat(r));
    guest::StoreU32(base, vehicle.address + 336, body.address);
    guest::StoreU32(base, vehicle.address + 5712, object.address);
    // The body's rotation: mostly a unit quaternion near the last one.
    RandomFloats(base, body.address + 80, 4, r);
    if (r.Below(4)) {
      for (uint32_t i = 0; i < 4; i++) {
        guest::StoreU32(base, body.address + 80 + 4 * i, OrdinaryFloat(r, 1.0));
      }
    }
    RandomFloats(base, body.address + 288, 16, r);
    const uint32_t spec = object.address + 1888;
    guest::StoreU32(base, spec + 76, r.Below(8) ? 1500 + r.Below(800) : uint32_t(r.Next()));
    guest::StoreU32(base, spec + 80, r.Below(8) ? 3500 + r.Below(1500) : uint32_t(r.Next()));
    RandomFloats(base, vectors.address, 32, r);
  }
  void Out(Outputs& out, uint8_t* base) const {
    out.Bytes(Read(base, camera.address, 32));
    out.Floats(base, camera.address + 32, 56);
    out.Bytes(Read(base, camera.address + 256, 96));
    out.Floats(base, camera.address + 352, 5);
    out.Bytes(Read(base, camera.address + 372, 12));
    out.Floats(base, camera.address + 384, 172);
    out.Bytes(Read(base, camera.address + 1072, 16));
    out.Floats(base, camera.address + 1088, 5);
    out.Bytes(Read(base, camera.address + 1108, 44));
    out.Bytes(Read(base, viewport.address, 1600));
    out.Floats(base, viewport.address + 1600, 48);
    out.Bytes(Read(base, viewport.address + 1792, kViewportBytes - 1792));
    out.Float(guest::LoadU32(base, kPassNear));
  }
};

void NativePlace(PPCContext& t, uint8_t* base) { hooks::CameraModesStep82153478(t, base); }
void NativeFollow(PPCContext& t, uint8_t* base) { hooks::CameraModesStep82153640(t, base); }

void ChaseCameras(Context& h, Random& r) {
  if (!hooks::NativeAtexit()) {
    ITC_LOG_WARN("harness: camera_modes: native atexit off: the chase camera's cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(c_runtime::ExitMutex());
  enum Kind { kPlace, kFollow, kConstruct, kUpdate };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
  } kFunctions[] = {
      {"ChaseCamera::Place (sub_82153478)", Original<__imp__sub_82153478>, NativePlace, kPlace},
      {"ChaseCamera::Follow (sub_82153640)", Original<__imp__sub_82153640>, NativeFollow,
       kFollow},
      {"ChaseCamera::ChaseCamera (sub_82154548)", Original<__imp__sub_82154548>,
       hooks::CameraModesStep82154548, kConstruct},
      {"ChaseCamera::Update (sub_82154808)", Original<__imp__sub_82154808>,
       hooks::CameraModesStep82154808, kUpdate},
  };
  Saved seed(h.base, kSeed, 4);
  Saved shake(h.base, kShakeStatics, 8);
  Saved statics(h.base, kChaseStatics, kChaseStaticsBytes);
  Saved near(h.base, kPassNear, 4);
  ChaseWorld w;
  for (const auto& f : kFunctions) {
    Results results(f.name);
    for (int c = 0; c < 200; c++) {
      w.Randomize(h.base, r);
      const auto camera_in = Read(h.base, w.camera.address, 1152);
      const auto viewport_in = Read(h.base, w.viewport.address, kViewportBytes);
      // No vehicle now and then (Place and Follow check; Update's shake reads
      // it regardless).
      const uint32_t the_vehicle =
          c % 20 == 0 && (f.kind == kPlace || f.kind == kFollow) ? 0 : w.vehicle.address;
      const double rate = r.Below(4) ? 0.07 : double(std::bit_cast<float>(ViewFloat(r)));
      const double least = r.Below(4) ? 0.1 : double(std::bit_cast<float>(ViewFloat(r)));
      const uint32_t seed_value = uint32_t(r.Next());
      const uint32_t shake_step = ViewFloat(r);
      const uint32_t shake_guard = uint32_t(r.Next());
      uint32_t chase[kChaseStaticsBytes / 4];
      for (auto& word : chase) {
        word = ViewFloat(r);
      }
      chase[8] = r.Below(4);  // the guard (0x826D8220): its bits 1 and 2
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, w.camera.address, camera_in.data(), 1152);
        Write(h.base, w.viewport.address, viewport_in.data(), kViewportBytes);
        if (f.kind != kConstruct) {
          guest::StoreU32(h.base, w.camera.address + 16, the_vehicle);
          guest::StoreU32(h.base, w.camera.address + 20, w.viewport.address);
        }
        guest::StoreU32(h.base, kSeed, seed_value);
        guest::StoreU32(h.base, kShakeStatics, shake_step);
        guest::StoreU32(h.base, kShakeStatics + 4, shake_guard);
        for (uint32_t i = 0; i < kChaseStaticsBytes / 4; i++) {
          guest::StoreU32(h.base, kChaseStatics + 4 * i, chase[i]);
        }
        const uint32_t mark = exit_table::Mark(h.base);
        PPCContext t = AlignedContext(h);
        FillStackBytes(h.base, t.r1.u32, uint64_t(c) + 1, 2048);
        t.r3.u64 = w.camera.address;
        if (f.kind == kPlace) {
          t.r4.u64 = w.vectors.address;
          t.r5.u64 = w.vectors.address + 16;
          t.r6.u64 = w.vectors.address + 32;
          t.r7.u64 = w.vectors.address + 48;
        } else if (f.kind == kFollow) {
          t.f1.f64 = rate;
          t.f2.f64 = least;
        } else if (f.kind == kConstruct) {
          t.r4.u64 = w.viewport.address;
          t.r5.u64 = w.vehicle.address;
        }
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        w.Out(out[v], h.base);
        out[v].Word(guest::LoadU32(h.base, kSeed));
        out[v].Bytes(Read(h.base, kShakeStatics, 8));
        out[v].Floats(h.base, kChaseStatics, 8);
        out[v].Bytes(Read(h.base, kChaseStatics + 32, 4));
        out[v].Floats(h.base, kChaseStatics + 36, 2);
        exit_table::Take(h.base, mark, out[v]);
        if (f.kind == kConstruct) {
          out[v].Word(t.r3.u32);
        }
        seed.Restore();
        shake.Restore();
        statics.Restore();
        near.Restore();
      }
      results.Check(out[0], out[1], fmt::format("case {} vehicle {:X}", c, the_vehicle));
    }
    results.Report();
  }
}

// --- Views 1 and 2's cameras ---

constexpr uint32_t kViewStatics = 0x826D8230;  // views 1 and 2's eight vectors and guards
constexpr uint32_t kViewStaticsBytes = 0x94;

void ViewCameras(Context& h, Random& r) {
  if (!hooks::NativeAtexit()) {
    ITC_LOG_WARN("harness: camera_modes: native atexit off: views 1 and 2's cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(c_runtime::ExitMutex());
  enum Kind { kConstruct, kUpdate };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
  } kFunctions[] = {
      {"ViewCamera1::ViewCamera1 (sub_82154A48)", Original<__imp__sub_82154A48>,
       hooks::CameraModesStep82154A48, kConstruct},
      {"ViewCamera1::Update (sub_82153A50)", Original<__imp__sub_82153A50>,
       hooks::CameraModesStep82153A50, kUpdate},
      {"ViewCamera2::ViewCamera2 (sub_82154D38)", Original<__imp__sub_82154D38>,
       hooks::CameraModesStep82154D38, kConstruct},
      {"ViewCamera2::Update (sub_82153E58)", Original<__imp__sub_82153E58>,
       hooks::CameraModesStep82153E58, kUpdate},
  };
  constexpr uint32_t kManagerAddress = 0x826D82F4, kVehiclesAddress = 0x826C2E34;
  constexpr uint32_t kSceneAddress = 0x826C2E44, kFlagWord = 0x826C7824;
  Saved statics(h.base, kViewStatics, kViewStaticsBytes);
  Saved near(h.base, kPassNear, 4);
  Saved race_data(h.base, cm::kRaceData, 4);
  Saved manager_word(h.base, kManagerAddress, 4);
  Saved vehicles_word(h.base, kVehiclesAddress, 4);
  Saved scene_word(h.base, kSceneAddress, 4);
  Saved flag_word(h.base, kFlagWord, 4);
  ChaseWorld w;
  Block race(kRaceBytes), manager(0x1000), records(64), scene(5120), nodes(128);
  for (const auto& f : kFunctions) {
    Results results(f.name);
    for (int c = 0; c < 200; c++) {
      w.Randomize(h.base, r);
      const auto camera_in = Read(h.base, w.camera.address, 1152);
      const auto viewport_in = Read(h.base, w.viewport.address, kViewportBytes);
      const uint32_t the_vehicle = c % 20 == 0 ? 0 : w.vehicle.address;
      uint32_t words[kViewStaticsBytes / 4];
      for (auto& word : words) {
        word = ViewFloat(r);
      }
      words[0x40 / 4] = r.Below(16);  // view 1's guard (0x826D8270)
      words[0x90 / 4] = r.Below(16);  // view 2's guard (0x826D82C0)
      // The race (its kind and +0x59614 for view 1's model), the flag word,
      // the scene's two models (kind 2, in use), the racers.
      const uint32_t kind = r.Below(2) ? 6 : r.Below(8);
      const uint32_t mode = r.Below(2) ? 2 : r.Below(4);
      const uint32_t flag = r.Below(2) ? 0 : uint32_t(r.Next());
      const uint32_t model_index[2] = {r.Below(4), r.Below(4)};
      const uint32_t racers = r.Below(3);
      const uint32_t car_flags = r.Below(3) == 0 ? 0 : (r.Below(2) ? 0x1u : 0x80u);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, w.camera.address, camera_in.data(), 1152);
        Write(h.base, w.viewport.address, viewport_in.data(), kViewportBytes);
        if (f.kind == kUpdate) {
          guest::StoreU32(h.base, w.camera.address + 16, the_vehicle);
          guest::StoreU32(h.base, w.camera.address + 20, w.viewport.address);
        }
        for (uint32_t i = 0; i < kViewStaticsBytes / 4; i++) {
          guest::StoreU32(h.base, kViewStatics + 4 * i, words[i]);
        }
        guest::StoreU32(h.base, race.address + 0xA3AF0, kind);
        guest::StoreU32(h.base, race.address + 0x59614, mode);
        guest::StoreU32(h.base, cm::kRaceData, race.address);
        guest::StoreU32(h.base, kFlagWord, flag);
        guest::StoreU32(h.base, scene.address + 5028, nodes.address);
        guest::StoreU32(h.base, scene.address + 5036, 2);
        for (uint32_t k = 0; k < 2; k++) {
          const uint32_t node = nodes.address + 64 * k;
          guest::StoreU32(h.base, node + 8, k == 0 ? nodes.address + 64 : 0);
          guest::StoreU32(h.base, node + 16, 3);
          guest::StoreU32(h.base, node + 32, 2);
          guest::StoreU32(h.base, node + 48, model_index[k]);
        }
        guest::StoreU32(h.base, kSceneAddress, scene.address);
        guest::StoreU32(h.base, manager.address + 2364, racers);
        for (uint32_t k = 0; k < 2; k++) {
          guest::StoreU32(h.base, manager.address + 4 * (577 + k), records.address);
        }
        guest::StoreU32(h.base, records.address + 20, 0);
        guest::StoreU32(h.base, kManagerAddress, manager.address);
        guest::StoreU32(h.base, kVehiclesAddress, w.vehicle.address);
        guest::StoreU32(h.base, w.vehicle.address + 6148, car_flags);
        const uint32_t mark = exit_table::Mark(h.base);
        PPCContext t = AlignedContext(h);
        FillStackBytes(h.base, t.r1.u32, uint64_t(c) + 1, 2048);
        t.r3.u64 = w.camera.address;
        if (f.kind == kConstruct) {
          t.r4.u64 = w.viewport.address;
          t.r5.u64 = the_vehicle;
        }
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        w.Out(out[v], h.base);
        for (uint32_t i = 0; i < kViewStaticsBytes / 4; i++) {
          if (i == 0x40 / 4 || i == 0x90 / 4) {
            out[v].Word(guest::LoadU32(h.base, kViewStatics + 4 * i));
          } else {
            out[v].Float(guest::LoadU32(h.base, kViewStatics + 4 * i));
          }
        }
        exit_table::Take(h.base, mark, out[v]);
        if (f.kind == kConstruct) {
          out[v].Word(t.r3.u32);
        }
        statics.Restore();
        near.Restore();
        race_data.Restore();
        manager_word.Restore();
        vehicles_word.Restore();
        scene_word.Restore();
        flag_word.Restore();
      }
      results.Check(out[0], out[1], fmt::format("case {} vehicle {:X}", c, the_vehicle));
    }
    results.Report();
  }
}

// --- The intro camera's update ---

void IntroUpdates(Context& h, Random& r) {
  Results results("IntroCamera::Update (sub_82155E58)");
  constexpr uint32_t kKept = 0x826D82C4, kPaused = 0x826A50D4;
  Saved kept(h.base, kKept, 4);
  Saved paused(h.base, kPaused, 4);
  Saved race_data(h.base, cm::kRaceData, 4);
  ChaseWorld w;
  Block race(kRaceBytes);
  for (int c = 0; c < 300; c++) {
    w.Randomize(h.base, r);
    const uint32_t list = w.camera.address + 352;
    const uint32_t count = r.Below(6) == 0 ? 0 : 1 + r.Below(16);
    RandomList(h.base, list, 16, count, r);
    for (uint32_t i = 0; i < 16; i++) {
      const uint32_t shot = list + 76 + 20 * i;
      guest::StoreU32(h.base, shot + 0, w.viewport.address);
      guest::StoreU32(h.base, shot + 4, w.vehicle.address);
      guest::StoreU32(h.base, shot + 8, r.Below(40));
      guest::StoreU32(h.base, shot + 12, ViewFloat(r));
      guest::StoreU32(h.base, shot + 16, r.Below(3) ? 0 : r.Below(5));
    }
    const uint32_t frames = r.Below(400);
    guest::StoreU32(h.base, w.camera.address + 748, count > 0 ? r.Below(count) : 0);
    guest::StoreU32(h.base, w.camera.address + 752, frames);
    guest::StoreU32(h.base, w.camera.address + 756, r.Below(5));
    guest::StoreU32(h.base, w.camera.address + 16, w.vehicle.address);
    guest::StoreU32(h.base, w.camera.address + 20, w.viewport.address);
    const auto camera_in = Read(h.base, w.camera.address, 1152);
    const uint32_t kept_value = r.Below(4) ? frames - r.Below(90) : uint32_t(r.Next());
    const uint32_t paused_value = r.Below(4) == 0 ? 1 : 0;
    const uint32_t flags = uint32_t(r.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, w.camera.address, camera_in.data(), 1152);
      guest::StoreU32(h.base, kKept, kept_value);
      guest::StoreU32(h.base, kPaused, paused_value);
      guest::StoreU32(h.base, race.address + 0x59AE4, flags);
      guest::StoreU32(h.base, cm::kRaceData, race.address);
      PPCContext t = AlignedContext(h);
      FillStackBytes(h.base, t.r1.u32, uint64_t(c) + 1, 2048);
      t.r3.u64 = w.camera.address;
      (v == 0 ? Original<__imp__sub_82155E58> : hooks::CameraModesStep82155E58)(t, h.base);
      w.Out(out[v], h.base);
      out[v].Bytes(Read(h.base, list, 412));
      out[v].Word(guest::LoadU32(h.base, kKept));
      out[v].Word(guest::LoadU32(h.base, race.address + 0x59AE4));
      kept.Restore();
      paused.Restore();
      race_data.Restore();
    }
    results.Check(out[0], out[1], fmt::format("case {} count {}", c, count));
  }
  results.Report();
}

// --- The rival logo's open and close (the second migration) ---

// The movie table swapped for an array of the case's (ids 10 to 12 and the
// case's `id` with `movie` when `with_id`), put back by `saved`.
uint32_t LogoTable(uint8_t* base, uint32_t id, bool with_id, uint32_t movie) {
  const uint32_t array = memory::crt::OperatorNewArray(base, 64).value_or(0);
  uint32_t count = 0;
  for (uint32_t k = 0; k < 3; k++, count++) {
    guest::StoreU32(base, array + 8 * k, 10 + k);
    guest::StoreU32(base, array + 8 * k + 4, 0x40000000 + k);
  }
  if (with_id) {
    guest::StoreU32(base, array + 8 * count, id);
    guest::StoreU32(base, array + 8 * count + 4, movie);
    count++;
  }
  guest::StoreU32(base, ui_movies::kMovieTable + 0, array);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, count);
  return array;
}

// The table after a case (its pairs, `movie` as 1), with the movies the
// case made deleted and the array freed.
void LogoTableOut(uint8_t* base, uint32_t movie, Outputs& out) {
  const uint32_t array = guest::LoadU32(base, ui_movies::kMovieTable);
  const uint32_t count = guest::LoadU32(base, ui_movies::kMovieTable + 8);
  out.Word(count);
  for (uint32_t k = 0; k < count; k++) {
    out.Word(guest::LoadU32(base, array + 8 * k));
    const uint32_t value = guest::LoadU32(base, array + 8 * k + 4);
    if (value != 0 && value == movie) {
      out.Word(1);
    } else if (value != 0 && memory::IsHeapBlock(value)) {
      out.Word(2);  // a movie Open made
      ui_movies::DestroyMovie(base, value);
      memory::crt::OperatorDelete(value);
    } else {
      out.Word(value);
    }
  }
  memory::crt::OperatorDeleteArray(array);
}

void RivalLogoOpenClose(Context& h, Random& r) {
  if (!ui_movies_game_hooks::OpenNative()) {
    ITC_LOG_WARN("harness: camera_modes: RivalLogo::Open and Close need the UI movies' Open "
                 "native (--native_heap, --native_c_runtime_heap, the archive's queue)");
    return;
  }
  {
    // The logo -1 or 0 to 3 (its record's movie in the table or not), +4's
    // bits random; the loads recorded (the pages freed, the entry as 1).
    Results results("RivalLogo::Open (sub_822118E0)");
    for (int c = 0; c < 60; c++) {
      const uint32_t logo = r.Below(6) == 0 ? 0xFFFFFFFFu : r.Below(4);
      const uint32_t flags = r.Below(4) ? (uint32_t(r.Next()) | 2) : uint32_t(r.Next());
      const uint32_t texture = r.Below(4);
      const bool there = r.Below(3) == 0;
      const uint32_t id = logo < 4 ? guest::LoadU32(h.base, 0x8200CD70 + 12 * logo) : 0;
      const uint64_t seed = r.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Saved table(h.base, ui_movies::kMovieTable, 12);
        Fill(h.base, h.data, kScratchBytes);
        Random fill(seed);
        const uint32_t entry = h.data + 0x100;
        RandomBytes(h.base, entry, 96, fill);
        guest::StoreU32(h.base, entry + 4, flags);
        guest::StoreU32(h.base, entry + 24, 0);
        LogoTable(h.base, id, there, 0x12345678);
        ArchiveLoads loads({int64_t(c), int64_t(c + 1)});
        PPCContext t = CallContext(h);
        t.r3.u64 = entry;
        t.r4.u64 = logo;
        t.r5.u64 = texture;
        (v == 0 ? Original<__imp__sub_822118E0> : hooks::CameraModesStep822118E0)(t, h.base);
        out[v].Bytes(Read(h.base, entry, 96));
        out[v].Word(t.r3.u32 == entry ? 1 : t.r3.u32);
        const std::vector<uint32_t>& calls = loads.calls();
        out[v].Word(uint32_t(calls.size()));
        for (size_t k = 0; k < calls.size(); k++) {
          // The movie's pages are the case's; LoadXpr's destination
          // ([0x826A5134]) is the game's buffer, compared as it is.
          if (k % 6 == 1 && calls[k] != 0 && calls[k] != guest::LoadU32(h.base, 0x826A5134) &&
              memory::IsPages(calls[k])) {
            out[v].Word(1);
            memory::FreePages(calls[k]);
          } else if (k % 6 == 3 && memory::IsHeapBlock(calls[k])) {
            out[v].Word(2);
          } else {
            out[v].Word(k % 6 == 3 && calls[k] == entry + 20 ? 3 : calls[k]);
          }
        }
        LogoTableOut(h.base, 0x12345678, out[v]);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: logo {:X} flags {:X} there {}", c, logo, flags, there));
    }
    results.Report();
  }
  {
    // A movie built natively (no animations, textures or texts) in the
    // table or not, +4's bits random, the logo 0 to 2, the XPR slot 0: the
    // entry, r3 (the table as 0, the empty entry as 1), the frame's +80,
    // whether the movie was freed, the table.
    Results results("RivalLogo::Close (sub_82211980)");
    Saved none(h.base, ui_movies::kMovieTable + 16, 4);
    for (int c = 0; c < 60; c++) {
      const uint32_t flags = r.Below(4) ? (uint32_t(r.Next()) | 8) : uint32_t(r.Next());
      const uint32_t logo = r.Below(3);
      const bool there = r.Below(4) != 0;
      const uint64_t seed = r.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Saved table(h.base, ui_movies::kMovieTable, 12);
        Fill(h.base, h.data, kScratchBytes);
        Random fill(seed);
        const uint32_t entry = h.data + 0x100;
        RandomBytes(h.base, entry, 96, fill);
        guest::StoreU32(h.base, entry + 0, logo);
        guest::StoreU32(h.base, entry + 4, flags);
        guest::StoreU32(h.base, entry + 8, 20);
        guest::StoreU32(h.base, entry + 24, 0);
        guest::StoreU32(h.base, entry + 84, fill.Below(4));
        guest::StoreU32(h.base, entry + 88, fill.Below(4));
        const uint32_t movie = memory::crt::OperatorNew(h.base, 0xA010).value_or(0);
        ui_movies::ConstructMovie(h.base, movie);
        ui_movies::InitAnimationLists(h.base, movie);
        guest::StoreU32(h.base, movie + 0x9FD4, 20);
        LogoTable(h.base, 20, there, movie);
        // The word an id not in the table reads as its movie: the case's.
        guest::StoreU32(h.base, ui_movies::kMovieTable + 16, movie);
        PPCContext t = CallContext(h);
        t.r3.u64 = entry;
        (v == 0 ? Original<__imp__sub_82211980> : hooks::CameraModesStep82211980)(t, h.base);
        const uint32_t array = guest::LoadU32(h.base, ui_movies::kMovieTable);
        const uint32_t r3 = t.r3.u32;
        out[v].Bytes(Read(h.base, entry, 96));
        out[v].Word(r3 == ui_movies::kMovieTable          ? 0
                    : r3 == 0x826C5CDC                    ? 1
                    : r3 >= array && r3 < array + 64      ? 0x100 + (r3 - array)
                                                          : r3);
        out[v].Word(guest::LoadU32(h.base, t.r1.u32 - 128 + 80));
        const bool freed = !memory::IsHeapBlock(movie);
        out[v].Word(freed);
        if (!freed) {
          out[v].Bytes(Read(h.base, guest::LoadU32(h.base, movie + 28), 16));
        }
        LogoTableOut(h.base, movie, out[v]);
        if (!freed) {
          ui_movies::DestroyMovie(h.base, movie);
          memory::crt::OperatorDelete(movie);
        }
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: logo {} flags {:X} there {}", c, logo, flags, there));
    }
    results.Report();
  }
}

// --- The cutscene's step and update (the second migration, batch 3) ---

// RivalLogo::Draw's calls while these cases run (its override's stand-in):
// r3 as an offset from the cutscene, lr, and r1 below the case's.
std::vector<uint32_t>* g_logo_draws = nullptr;
uint32_t g_logo_draws_cutscene = 0;
uint32_t g_logo_draws_r1 = 0;
void RecordLogoDraw(PPCContext& ctx, uint8_t*) {
  g_logo_draws->push_back(ctx.r3.u32 - g_logo_draws_cutscene);
  g_logo_draws->push_back(uint32_t(ctx.lr));
  g_logo_draws->push_back(g_logo_draws_r1 - ctx.r1.u32);
}

// The stack below a case's stack pointer (2,048 bytes) as ordinary floats:
// the frame words CutsceneShot::Step reads as they are (no animation
// object writes them) become the view's zoom and roll.
void FillStackFloats(uint8_t* base, uint32_t r1, uint64_t seed) {
  Random s(seed);
  for (uint32_t i = 0; i < 2048; i += 4) {
    guest::StoreU32(base, r1 - 2048 + i, OrdinaryFloat(s, 1000.0));
  }
}

// Cutscene::Step and Update on a cutscene of the case's: 16 shots (as
// CutsceneShot::Step's cases) in a list of 0 to 16, the index anywhere, the
// busy word now and then set; the logo entry's flags random (Play's bit 0x4
// with neither the XPR nor the movie loaded, so Play waits: its own cases
// are RivalLogo's), its movie built natively (Close's cases), in the table
// or not; the viewport random floats; the race data's word +0x59AE4 one of
// the case's. The draw is answered by a stand-in that records it (r3, lr,
// r1). A shot whose roll would keep Viewport::SetRoll's loop from ending
// (infinite, or too large to take 2 pi off) in a dry run of
// CutsceneShot::Step is given an animation not loaded. Compared: the
// cutscene, the logo entry, the viewport, the near plane, the fades, the
// race word, the draws, the movie table, whether the movie was freed, r3
// and lr.
void CutsceneSteps(Context& h, Random& r) {
  if (!ui_movies_game_hooks::OpenNative()) {
    ITC_LOG_WARN("harness: camera_modes: Cutscene::Step and Update need the UI movies' Open "
                 "native (--native_heap, --native_c_runtime_heap, the archive's queue)");
    return;
  }
  constexpr uint32_t kFades = 0x826A50A0;
  constexpr uint32_t kFadesBytes = 0x60;
  constexpr uint32_t kLogo = 10760;
  constexpr uint32_t kCutsceneBytes = kLogo + 96;
  constexpr uint32_t kDone = 0x59AE4;
  Saved saved_fades(h.base, kFades, kFadesBytes);
  Saved saved_table(h.base, cm::kAnims, kTableBytes);
  Saved saved_near(h.base, kPassNear, 4);
  Saved saved_race(h.base, cm::kRaceData, 4);
  Saved saved_none(h.base, ui_movies::kMovieTable + 16, 4);
  AnimWorld world(h.base);
  Block cutscene(kCutsceneBytes), viewport(kViewportBytes), vehicle(512), body(512), race(16);
  std::vector<uint32_t> draws;
  g_logo_draws = &draws;
  g_logo_draws_cutscene = cutscene.address;
  race_hud_hooks::g_rival_logo_draw_stand_in = RecordLogoDraw;
  const uint32_t list = cutscene.address + 952;
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "Cutscene::Step (sub_82155A28)"
                               : "Cutscene::Update (sub_821561F8)");
    for (int c = 0; c < 150; c++) {
      world.Build(r);
      RandomBytes(h.base, cutscene.address, cutscene.bytes, r);
      guest::StoreU32(h.base, cutscene.address + 20, viewport.address);
      RandomFloats(h.base, cutscene.address + 32, 4, r);
      guest::StoreU32(h.base, cutscene.address + 352, r.Below(5) == 0 ? 0xFFFFFFFFu : r.Below(16));
      const uint32_t count = r.Below(17);
      RandomList(h.base, list, 16, count, r);
      const uint32_t index = r.Below(6) == 0   ? r.Below(40)
                             : r.Below(3) == 0 ? (count > 0 ? count - 1 : 0)
                                               : (count > 0 ? r.Below(count) : 0);
      guest::StoreU32(h.base, cutscene.address + 360, index);
      guest::StoreU32(h.base, cutscene.address + 10756, r.Below(8) == 0 ? r.Below(3) + 1 : 0);
      for (uint32_t s = 0; s < 16; s++) {
        const uint32_t shot = list + 76 + 608 * s;
        guest::StoreU32(h.base, shot + 4, 22824 + r.Below(world.entries + 2));
        guest::StoreU32(h.base, shot + 8, vehicle.address);
        const uint32_t cues = shot + 12;
        RandomList(h.base, cues, 16, r.Below(5), r);
        for (uint32_t k = 0; k < 16; k++) {
          const uint32_t cue = cues + 76 + 32 * k;
          const int32_t start = int32_t(r.Below(40)) - 5;
          guest::StoreU32(h.base, cue + 0, uint32_t(start));
          guest::StoreU32(h.base, cue + 4, uint32_t(start + int32_t(r.Below(40)) - 5));
          guest::StoreU32(h.base, cue + 8, r.Below(4));
          guest::StoreU32(h.base, cue + 24, r.Below(3) == 0 ? uint32_t(start) : r.Below(60));
          guest::StoreU32(h.base, cue + 28, r.Below(4) == 0 ? 1 : 0);
        }
        guest::StoreU32(h.base, shot + 600, r.Below(4));
        guest::StoreU32(h.base, shot + 604, uint32_t(r.Next()));
      }
      const uint32_t entry = cutscene.address + kLogo;
      const uint32_t flags = (uint32_t(r.Next()) & ~0xFu) | r.Below(16);
      guest::StoreU32(h.base, entry + 0, r.Below(3));
      guest::StoreU32(h.base, entry + 4, flags);
      guest::StoreU32(h.base, entry + 8, 20);
      guest::StoreU32(h.base, entry + 12, 0x80000000u | uint32_t(r.Next()));
      guest::StoreU32(h.base, entry + 24, 0);
      guest::StoreU32(h.base, entry + 48, guest::LoadU32(h.base, entry + 48) & ~0x10u);
      guest::StoreU32(h.base, entry + 84, r.Below(4));
      guest::StoreU32(h.base, entry + 88, r.Below(4));
      RandomFloats(h.base, viewport.address, viewport.bytes / 4, r);
      guest::StoreU32(h.base, vehicle.address + 336, body.address);
      RandomFloats(h.base, body.address + 288, 16, r);
      guest::StoreU32(h.base, race.address, uint32_t(r.Next()));
      const bool there = r.Below(4) != 0;
      const uint64_t stack_seed = r.Next();
      const uint32_t case_r1 = AlignedContext(h).r1.u32;
      const uint32_t step_frame = case_r1 - 192 - (which == 1 ? 96 : 0);

      // The dry run of the shot Step would step, for its roll.
      if (index < uint32_t(int16_t(guest::LoadU16(h.base, list + 72)))) {
        uint32_t node = (uint32_t(int16_t(guest::LoadU16(h.base, list + 68))) << 2) + list;
        for (uint32_t n = index; n != 0; n--) {
          node = (uint32_t(int16_t(guest::LoadU16(h.base, node))) << 2) + list;
        }
        const uint32_t shot =
            uint32_t(int64_t(int16_t(guest::LoadU16(h.base, node + 2))) * 608) + list + 76;
        const auto shot_bytes = Read(h.base, shot, 608);
        FillStackFloats(h.base, case_r1, stack_seed);
        guest::StoreU32(h.base, step_frame - 144, step_frame);
        PPCContext d = AlignedContext(h);
        d.r1.u64 = step_frame;
        d.r3.u64 = shot;
        d.r4.u64 = step_frame + 80;
        hooks::CameraModesStep82155800(d, h.base);
        const float roll = std::bit_cast<float>(guest::LoadU32(h.base, step_frame + 144));
        saved_fades.Restore();
        Write(h.base, shot, shot_bytes.data(), 608);
        if (std::isinf(roll) || (!std::isnan(roll) && std::fabs(roll) >= 1e5f)) {
          guest::StoreU32(h.base, shot + 4, 0);
        }
      }
      const auto inputs = Read(h.base, cutscene.address, cutscene.bytes);
      const auto viewport_inputs = Read(h.base, viewport.address, viewport.bytes);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Saved table(h.base, ui_movies::kMovieTable, 12);
        Fill(h.base, h.data, kScratchBytes);
        Write(h.base, cutscene.address, inputs.data(), cutscene.bytes);
        Write(h.base, viewport.address, viewport_inputs.data(), viewport.bytes);
        const uint32_t race_word = guest::LoadU32(h.base, race.address);
        guest::StoreU32(h.base, cm::kRaceData, race.address - kDone);
        const uint32_t movie = memory::crt::OperatorNew(h.base, 0xA010).value_or(0);
        ui_movies::ConstructMovie(h.base, movie);
        ui_movies::InitAnimationLists(h.base, movie);
        guest::StoreU32(h.base, movie + 0x9FD0, 0);
        guest::StoreU32(h.base, movie + 0x9FD4, 20);
        LogoTable(h.base, 20, there, movie);
        guest::StoreU32(h.base, ui_movies::kMovieTable + 16, movie);
        PPCContext t = AlignedContext(h);
        FillStackFloats(h.base, t.r1.u32, stack_seed);
        t.r3.u64 = cutscene.address;
        t.lr = which == 0 ? 0x82156214 : 0x82159614;
        draws.clear();
        g_logo_draws_r1 = t.r1.u32;
        if (which == 0) {
          (v == 0 ? Original<__imp__sub_82155A28> : hooks::CameraModesStep82155A28)(t, h.base);
        } else {
          (v == 0 ? Original<__imp__sub_821561F8> : hooks::CameraModesStep821561F8)(t, h.base);
        }
        out[v].Bytes(Read(h.base, cutscene.address, kLogo));
        out[v].Bytes(Read(h.base, entry, 96));
        out[v].Floats(h.base, viewport.address, viewport.bytes / 4);
        out[v].Float(guest::LoadU32(h.base, kPassNear));
        out[v].Bytes(Read(h.base, kFades, kFadesBytes));
        out[v].Word(guest::LoadU32(h.base, race.address));
        out[v].Word(uint32_t(draws.size()));
        for (uint32_t w : draws) {
          out[v].Word(w);
        }
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.lr));
        const bool freed = !memory::IsHeapBlock(movie);
        out[v].Word(freed);
        LogoTableOut(h.base, movie, out[v]);
        if (!freed) {
          ui_movies::DestroyMovie(h.base, movie);
          memory::crt::OperatorDelete(movie);
        }
        guest::StoreU32(h.base, race.address, race_word);
        saved_fades.Restore();
        saved_near.Restore();
        saved_race.Restore();
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: count {} index {} flags {:X} there {}", c, count,
                                index, flags, there));
    }
    results.Report();
  }
  race_hud_hooks::g_rival_logo_draw_stand_in = nullptr;
  g_logo_draws = nullptr;
}

// --- The TV camera (batch 4 of the second migration) ---

constexpr uint32_t kTvBytes = 43536;
constexpr uint32_t kPathRecords = 0x82618D30;  // the camera paths' records (40 pointers)

// A TV camera's world: the camera (+43136 its fallback chase camera), and a
// chase camera's world for the viewport, the vehicle, its body and spec.
struct TvWorld {
  Block camera{kTvBytes};
  ChaseWorld chase;
  void Out(Outputs& out, uint8_t* base) const {
    const uint32_t c = camera.address;
    out.Bytes(Read(base, c, 32));
    out.Floats(base, c + 32, 56);
    out.Bytes(Read(base, c + 256, 96));
    out.Floats(base, c + 352, 2);
    out.Bytes(Read(base, c + 360, 8));
    out.Floats(base, c + 368, 18);
    out.Bytes(Read(base, c + 440, 43136 - 440));
    const uint32_t s = c + 43136;
    out.Bytes(Read(base, s, 32));
    out.Floats(base, s + 32, 56);
    out.Bytes(Read(base, s + 256, 96));
    out.Floats(base, s + 352, 5);
    out.Bytes(Read(base, s + 372, 12));
    out.Bytes(Read(base, c + 43520, kTvBytes - 43520));
    const uint32_t v = chase.viewport.address;
    out.Bytes(Read(base, v, 1600));
    out.Floats(base, v + 1600, 48);
    out.Bytes(Read(base, v + 1792, kViewportBytes - 1792));
    out.Float(guest::LoadU32(base, kPassNear));
  }
};

// The camera paths' records for TvCamera::TvCamera's ranges: +4 the start's
// section, +8 the end's (the route's mostly, now and then any).
void TvRecords(uint8_t* base, uint32_t records, const test_route::Route& route, Random& r) {
  for (uint32_t i = 0; i < 40; i++) {
    const uint32_t record = records + 16 * i;
    RandomBytes(base, record, 16, r);
    for (const uint32_t at : {4u, 8u}) {
      guest::StoreU32(base, record + at, r.Below(8) ? r.Below(route.sections)
                                                    : uint32_t(r.Next()));
    }
    guest::StoreU32(base, kPathRecords + 4 * i, record);
  }
}

void TvConstructs(Context& h, Random& r) {
  if (!hooks::NativeAtexit()) {
    ITC_LOG_WARN("harness: camera_modes: native atexit off: the TV camera's cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(c_runtime::ExitMutex());
  const uint32_t area = memory::Alloc(kRouteArea, 0);
  const uint32_t table = memory::Alloc(4 * test_route::kTableEntries, 0);
  test_route::FillTable(h.base, table, area);
  Saved saved_route(h.base, kRouteGlobal, 4);
  Saved saved_records(h.base, kPathRecords, 160);
  Saved seed(h.base, kSeed, 4);
  Saved statics(h.base, kChaseStatics, kChaseStaticsBytes);
  Saved near(h.base, kPassNear, 4);
  Block records(40 * 16);
  TvWorld w;
  Results results("TvCamera::TvCamera (sub_82155288)");
  for (int c = 0; c < 120; c++) {
    const test_route::Route route = PathRoute(h.base, area, table, r);
    w.chase.Randomize(h.base, r);
    RandomBytes(h.base, w.camera.address, kTvBytes, r);
    const uint32_t ranges = c % 10 == 0 ? 0 : c % 3 == 0 ? 29 + r.Below(12) : 1 + r.Below(40);
    guest::StoreU32(h.base, w.camera.address + 452, ranges);
    TvRecords(h.base, records.address, route, r);
    uint32_t chase[kChaseStaticsBytes / 4];
    for (auto& word : chase) {
      word = ViewFloat(r);
    }
    chase[8] = r.Below(4);  // the guard (0x826D8220): its bits 1 and 2
    const uint32_t seed_value = uint32_t(r.Next());
    const auto route_bytes = Read(h.base, area, kRouteArea);
    const auto camera_in = Read(h.base, w.camera.address, kTvBytes);
    const auto viewport_in = Read(h.base, w.chase.viewport.address, kViewportBytes);
    const auto records_in = Read(h.base, records.address, 40 * 16);
    const auto table_in = Read(h.base, kPathRecords, 160);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, area, route_bytes.data(), kRouteArea);
      Write(h.base, w.camera.address, camera_in.data(), kTvBytes);
      Write(h.base, w.chase.viewport.address, viewport_in.data(), kViewportBytes);
      Write(h.base, records.address, records_in.data(), 40 * 16);
      Write(h.base, kPathRecords, table_in.data(), 160);
      guest::StoreU32(h.base, kRouteGlobal, area + test_route::kFile);
      guest::StoreU32(h.base, kSeed, seed_value);
      for (uint32_t i = 0; i < kChaseStaticsBytes / 4; i++) {
        guest::StoreU32(h.base, kChaseStatics + 4 * i, chase[i]);
      }
      const uint32_t mark = exit_table::Mark(h.base);
      PPCContext t = AlignedContext(h);
      FillStackBytes(h.base, t.r1.u32, uint64_t(c) + 7, 2048);
      t.r3.u64 = w.camera.address;
      t.r4.u64 = w.chase.viewport.address;
      t.r5.u64 = w.chase.vehicle.address;
      (v == 0 ? Original<__imp__sub_82155288> : hooks::CameraModesStep82155288)(t, h.base);
      w.Out(out[v], h.base);
      out[v].Word(guest::LoadU32(h.base, kSeed));
      out[v].Floats(h.base, kChaseStatics, 8);
      out[v].Bytes(Read(h.base, kChaseStatics + 32, 4));
      out[v].Floats(h.base, kChaseStatics + 36, 2);
      exit_table::Take(h.base, mark, out[v]);
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(h.base, t.r1.u32 - 160 + 80, 8));
      seed.Restore();
      statics.Restore();
      near.Restore();
      saved_records.Restore();
    }
    results.Check(out[0], out[1], fmt::format("case {} ranges {}", c, ranges));
  }
  results.Report();
  saved_route.Restore();
  memory::Free(table);
  memory::Free(area);
}

// TvCamera::Step's and Update's outputs: the camera (floats where the
// functions store floats), the fallback chase camera, the viewport.
void TvStepOut(Outputs& out, uint8_t* base, const TvWorld& w) {
  const uint32_t c = w.camera.address;
  out.Bytes(Read(base, c, 32));
  out.Floats(base, c + 32, 56);
  out.Bytes(Read(base, c + 256, 112));
  out.Bytes(Read(base, c + 368, 16));
  out.Floats(base, c + 384, 8);
  out.Bytes(Read(base, c + 416, 16));
  out.Floats(base, c + 432, 2);
  out.Bytes(Read(base, c + 440, 43136 - 440));
  const uint32_t s = c + 43136;
  out.Bytes(Read(base, s, 32));
  out.Floats(base, s + 32, 56);
  out.Bytes(Read(base, s + 256, 96));
  out.Floats(base, s + 352, 5);
  out.Bytes(Read(base, s + 372, 12));
  out.Bytes(Read(base, c + 43520, kTvBytes - 43520));
  const uint32_t v = w.chase.viewport.address;
  out.Bytes(Read(base, v, 1600));
  out.Floats(base, v + 1600, 64);
  out.Bytes(Read(base, v + 1856, kViewportBytes - 1856));
  out.Float(guest::LoadU32(base, kPassNear));
}

// TvCamera::Step's spot and camera slice as it will find them (the ranges'
// updates and RouteRange::CameraAt natively, on the case's inputs, put back
// after): what the cases' path buffers are made from, so that no buffer the
// step would ask a path for (CameraPath::Request's real loads) is empty or
// loaded. False when no range is listed.
bool TvProbe(uint8_t* base, uint32_t camera, uint32_t position, uint32_t scratch,
             int32_t* spot, uint32_t* slice) {
  const auto camera_in = Read(base, camera, 43016);
  const uint32_t count_in = guest::LoadU32(base, kSliceCountGlobal);
  std::vector<uint32_t> listed;
  const track_zones::Position p{uint16_t(position >> 16), uint16_t(position)};
  for (uint32_t i = 0; i < guest::LoadU32(base, camera + 452); i++) {
    if (track_zones::RangeUpdate(base, camera + 456 + 1064 * i, p) != 0) {
      listed.push_back(i);
    }
  }
  bool any = !listed.empty();
  if (any) {
    const uint32_t current = guest::LoadU32(base, camera + 360);
    *spot = int32_t(listed[0]);
    if (listed.size() > 1) {
      for (const uint32_t i : listed) {
        if (i == current) {
          *spot = int32_t(current);
        }
      }
    }
    guest::StoreU32(base, scratch, position);
    *slice = uint32_t(cm::RangeCameraAt(base, camera + 456 + 1064 * uint32_t(*spot), scratch));
  }
  Write(base, camera, camera_in.data(), 43016);
  guest::StoreU32(base, kSliceCountGlobal, count_in);
  return any;
}

// A camera path's key at `key` with its curves at `curves` (27 pairs, as
// CameraPath::Evaluate's cases make them).
void TvKey(uint8_t* base, uint32_t key, uint32_t curves, Random& r) {
  RandomBytes(base, key, 216, r);
  for (uint32_t p = 0; p < 27; p++) {
    PathPair(base, key + 8 * p, curves + 0x200 * p, r, true);
  }
  for (const uint32_t at : {72u, 104u, 112u, 168u, 200u, 208u}) {
    if (r.Below(2)) {
      guest::StoreU32(base, key + at, 0);
      const bool weight = at != 112 && at != 208;
      guest::StoreU32(base, key + at + 4,
                      r.Below(4) == 0 ? 0u
                      : weight ? std::bit_cast<uint32_t>(float(r.Below(5)) / 4.0f)
                               : std::bit_cast<uint32_t>(float(r.Below(400)) / 10.0f - 5.0f));
    }
  }
}

void TvSteps(Context& h, Random& r) {
  if (!hooks::NativeAtexit()) {
    ITC_LOG_WARN("harness: camera_modes: native atexit off: the TV camera's step skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(c_runtime::ExitMutex());
  const uint32_t area = memory::Alloc(kRouteArea, 0);
  const uint32_t table = memory::Alloc(4 * test_route::kTableEntries, 0);
  test_route::FillTable(h.base, table, area);
  Saved saved_route(h.base, kRouteGlobal, 4);
  Saved saved_count(h.base, kSliceCountGlobal, 4);
  Saved spline(h.base, kSplineStatic, kSplineStaticBytes);
  Saved seed(h.base, kSeed, 4);
  Saved shake(h.base, kShakeStatics, 8);
  Saved statics(h.base, kChaseStatics, kChaseStaticsBytes);
  Saved near(h.base, kPassNear, 4);
  // The color filter's style and guard (0x826D82C8, 0x826D82CC), and a
  // color filter of the case's (PostEffects::Get(2)).
  constexpr uint32_t kStyle = 0x826D82C8;
  Saved style(h.base, kStyle, 8);
  Saved effect(h.base, post_effects::kEffects + 8, 4);
  Block filter(8200);
  Block filter_vtable(64);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(h.base, filter_vtable.address + 4 * i, 0);
  }
  guest::StoreU32(h.base, filter_vtable.address + 16, 0x821350F8);
  guest::StoreU32(h.base, filter_vtable.address + 24, 0x82135138);
  guest::StoreU32(h.base, filter_vtable.address + 28, 0x82135140);
  const auto spline_made = Read(h.base, kSplineStatic, kSplineStaticBytes);
  Block paths(0x8000);  // the records (+0) and two keys (+0x100, +0x200) with their curves
  Block scratch(16);
  TvWorld w;
  const uint32_t cam = w.camera.address;
  enum Kind { kStep, kUpdate };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
  } kFunctions[] = {
      {"TvCamera::Step (sub_821562F8)", Original<__imp__sub_821562F8>,
       hooks::CameraModesStep821562F8, kStep},
      {"TvCamera::Update (sub_821593D8)", Original<__imp__sub_821593D8>,
       hooks::CameraModesStep821593D8, kUpdate},
  };
  for (const auto& f : kFunctions) {
    Results results(f.name);
    int mode_counts[4] = {};
    for (int c = 0; c < 200; c++) {
      const test_route::Route route = PathRoute(h.base, area, table, r);
      guest::StoreU32(h.base, kRouteGlobal, area + test_route::kFile);
      w.chase.Randomize(h.base, r);
      RandomBytes(h.base, cam, kTvBytes, r);
      RandomFloats(h.base, cam + 32, 56, r);
      RandomFloats(h.base, cam + 368, 18, r);
      guest::StoreU32(h.base, cam + 16, w.chase.vehicle.address);
      guest::StoreU32(h.base, cam + 20, w.chase.viewport.address);
      guest::StoreU32(h.base, cam + 448, f.kind == kStep || r.Below(2) ? 0 : uint32_t(r.Next()) | 1);
      const uint32_t ranges = c % 16 == 0 ? 0 : 1 + r.Below(40);
      guest::StoreU32(h.base, cam + 452, ranges);
      for (uint32_t i = 0; i < ranges; i++) {
        PathRange(h.base, cam + 456 + 1064 * i, scratch.address, route, r, true);
      }
      // The car's track position (Vehicle::Identity +4, read back from the
      // tracker's +228): often a range's start.
      const uint32_t vehicle = w.chase.vehicle.address;
      if (ranges > 0 && r.Below(4) != 0) {
        const uint32_t start = guest::LoadU32(h.base, cam + 456 + 1064 * r.Below(ranges) + 4);
        guest::StoreU32(h.base, vehicle + 5236, r.Below(2) ? start : start + r.Below(5) - 2);
      } else {
        test_route::RandomPosition(h.base, vehicle + 5236, route, r);
      }
      const uint32_t position = guest::LoadU32(h.base, vehicle + 5236);
      guest::StoreU32(h.base, cam + 360,
                      ranges > 0 && r.Below(2) ? r.Below(ranges) : uint32_t(r.Next()));
      guest::StoreU32(h.base, cam + 43520, r.Below(3) == 0 ? r.Below(5) : 0);
      // The path buffers from the probe: mode 0 a buffer holding the spot,
      // loaded, with path records about the camera slice; 1 none holding
      // it; 2 one holding it, not loaded. No buffer the step would ask for
      // is empty or loaded.
      int32_t spot = -1;
      uint32_t slice = 0;
      const bool listed = TvProbe(h.base, cam, position, scratch.address, &spot, &slice);
      const uint32_t mode = r.Below(10) < 5 ? 0 : r.Below(2) + 1;
      mode_counts[listed ? mode : 3]++;
      const uint32_t first = r.Below(4);
      const uint32_t busy[] = {1, 2, 4, 0x10000};
      const uint32_t records = paths.address;
      const uint32_t record_count = 1 + r.Below(6);
      for (uint32_t k = 0; k < 4; k++) {
        const uint32_t buffer = cam + 43016 + 28 * k;
        RandomBytes(h.base, buffer, 28, r);
        uint32_t index = uint32_t(spot) + 1 + r.Below(50);
        uint32_t state = busy[r.Below(4)];
        if (mode == 0 && k == first) {
          index = uint32_t(spot);
          state = 3;
        } else if (mode == 0 && k > first && r.Below(2)) {
          index = uint32_t(spot);
          state = r.Below(2) ? 3 : busy[r.Below(4)];
        } else if (mode == 2 && k == first) {
          index = uint32_t(spot);
        }
        guest::StoreU32(h.base, buffer + 12, index);
        guest::StoreU32(h.base, buffer + 24, state);
        guest::StoreU32(h.base, buffer + 4, records);
        guest::StoreU32(h.base, buffer + 8, records + 20 * (record_count - 1));
      }
      // The records: their ends about the camera slice; their keys one of
      // two.
      for (uint32_t i = 0; i < record_count; i++) {
        const uint32_t record = records + 20 * i;
        RandomBytes(h.base, record, 20, r);
        for (const uint32_t at : {0u, 4u}) {
          guest::StoreU32(h.base, record + at,
                          r.Below(8) ? slice + r.Below(9) - 4 : uint32_t(r.Next()));
        }
        guest::StoreU32(h.base, record + 16, paths.address + (r.Below(2) ? 0x100 : 0x200));
      }
      TvKey(h.base, paths.address + 0x100, paths.address + 0x400, r);
      TvKey(h.base, paths.address + 0x200, paths.address + 0x400 + 27 * 0x200, r);
      guest::StoreU32(h.base, cam + 352, r.Below(3) == 0 ? slice : slice + r.Below(7) - 3);
      guest::StoreU32(h.base, cam + 356, slice + r.Below(9) - 4);
      // The fallback chase camera (Update's): ChaseCamera::Update's vtable.
      {
        const uint32_t s = cam + 43136;
        guest::StoreU32(h.base, s, 0x82006B24);
        guest::StoreU32(h.base, s + 16, vehicle);
        guest::StoreU32(h.base, s + 20, w.chase.viewport.address);
        RandomFloats(h.base, s + 32, 88, r);
        guest::StoreU32(h.base, s + 380, r.Below(2) ? 0 : uint32_t(r.Next()) | 1);
      }
      auto spline_in = spline_made;
      if (c % 3 == 0) {
        spline_in[67] &= 0xFE;
      }
      uint32_t chase[kChaseStaticsBytes / 4];
      for (auto& word : chase) {
        word = ViewFloat(r);
      }
      chase[8] = 3;
      const uint32_t seed_value = uint32_t(r.Next());
      const uint32_t shake_step = ViewFloat(r);
      const uint32_t guard = r.Below(2) ? 1u : uint32_t(r.Next());
      const uint32_t style_in = r.Below(5);
      const bool no_filter = c % 13 == 12;
      RandomBytes(h.base, filter.address + 8064, 64, r);
      const uint32_t count = uint32_t(r.Next());
      const auto route_bytes = Read(h.base, area, kRouteArea);
      const auto camera_in = Read(h.base, cam, kTvBytes);
      const auto viewport_in = Read(h.base, w.chase.viewport.address, kViewportBytes);
      const auto vehicle_in = Read(h.base, vehicle, 6272);
      const auto paths_in = Read(h.base, paths.address, 0x8000);
      const auto filter_in = Read(h.base, filter.address + 8064, 64);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, area, route_bytes.data(), kRouteArea);
        Write(h.base, cam, camera_in.data(), kTvBytes);
        Write(h.base, w.chase.viewport.address, viewport_in.data(), kViewportBytes);
        Write(h.base, vehicle, vehicle_in.data(), 6272);
        Write(h.base, paths.address, paths_in.data(), 0x8000);
        Write(h.base, filter.address + 8064, filter_in.data(), 64);
        Write(h.base, kSplineStatic, spline_in.data(), kSplineStaticBytes);
        guest::StoreU32(h.base, kRouteGlobal, area + test_route::kFile);
        guest::StoreU32(h.base, kSliceCountGlobal, count);
        guest::StoreU32(h.base, kSeed, seed_value);
        guest::StoreU32(h.base, kShakeStatics, shake_step);
        guest::StoreU32(h.base, kShakeStatics + 4, 1);
        for (uint32_t i = 0; i < kChaseStaticsBytes / 4; i++) {
          guest::StoreU32(h.base, kChaseStatics + 4 * i, chase[i]);
        }
        guest::StoreU32(h.base, kStyle, style_in);
        guest::StoreU32(h.base, kStyle + 4, guard);
        guest::StoreU32(h.base, filter.address, filter_vtable.address);
        guest::StoreU32(h.base, post_effects::kEffects + 8, no_filter ? 0 : filter.address);
        const uint32_t mark = exit_table::Mark(h.base);
        PPCContext t = AlignedContext(h);
        FillStackBytes(h.base, t.r1.u32, uint64_t(c) + 11, 4096);
        t.r3.u64 = cam;
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        TvStepOut(out[v], h.base, w);
        // Step's frame: the words it writes (under Update, the fallback
        // chase camera's callees' frames before it leave saved registers
        // there, which native code doesn't write: only the words Step
        // writes itself, when it runs).
        const uint32_t frame = t.r1.u32 - (f.kind == kStep ? 720 : 128 + 720);
        if (f.kind == kStep) {
          out[v].Bytes(Read(h.base, frame + 80, 16));
          out[v].Bytes(Read(h.base, frame + 176, 412));
        } else if (guest::LoadU32(h.base, cam + 448) == 0) {
          out[v].Bytes(Read(h.base, frame + 80, 12));
          out[v].Bytes(Read(h.base, frame + 176, 250));
        }
        out[v].Word(guest::LoadU32(h.base, kSliceCountGlobal));
        out[v].Word(guest::LoadU32(h.base, kSeed));
        out[v].Bytes(Read(h.base, kStyle, 8));
        out[v].Floats(h.base, filter.address + 8064, 16);
        out[v].Bytes(Read(h.base, kShakeStatics, 8));
        out[v].Floats(h.base, kChaseStatics, 8);
        out[v].Bytes(Read(h.base, kChaseStatics + 32, 4));
        out[v].Floats(h.base, kChaseStatics + 36, 2);
        out[v].Bytes(Read(h.base, kSplineStatic + 64, 4));
        out[v].Floats(h.base, kSplineStatic, 16);
        if (f.kind == kUpdate) {
          out[v].Bytes(Read(h.base, t.r1.u32 - 128 + 80, 16));
        }
        exit_table::Take(h.base, mark, out[v]);
        spline.Restore();
        seed.Restore();
        shake.Restore();
        statics.Restore();
        near.Restore();
        style.Restore();
        effect.Restore();
      }
      results.Check(out[0], out[1], fmt::format("case {} ranges {} spot {} slice {:X} mode {}",
                                                c, ranges, spot, slice, listed ? mode : 3));
    }
    ITC_LOG_INFO("harness: camera_modes: {} cases by path: {} loaded, {} not held, {} held "
                 "not loaded, {} none listed",
                 f.name, mode_counts[0], mode_counts[1], mode_counts[2], mode_counts[3]);
    results.Report();
  }
  saved_route.Restore();
  memory::Free(table);
  memory::Free(area);
}

// --- The garage camera ---

// GarageCamera::Update on a camera block of the case's: the viewports
// pointer at a block of the case's (viewport 0 random, its vectors floats),
// the app interface [0x826BABA0] at a stand-in scene (its slot 8 and the
// manager's slots 1 and 2 the game's getters, sub_8248CA78 and
// sub_82379320, answering the manager, the case's command list and render
// queue, as the cameras harness's FakeScene). The globals it writes (camera
// pass 0, 0x8296B8F4 to 0x8296B91F, 0x8265ECE0, the open chunks' count)
// saved and put back. The camera's angles mostly ordinary radians, its
// field of view mostly ordinary degrees, now and then any float (where tan
// can't stand in, both run the original).
void GarageCameras(Context& h, Random& r) {
  Results results("GarageCamera::Update (sub_8217BE98)");
  constexpr uint32_t kPass0 = 0x826BAC00, kPassBytes = 592;
  constexpr uint32_t kSceneGlobal = 0x826BABA0;
  constexpr uint32_t kOuts = 0x8296B8F4, kOutsBytes = 0x2C, kEyeOut = 0x8265ECE0;
  constexpr uint32_t kCameraBytes = 272, kSceneBytes = 4096;
  // The stand-in scene's layout (FakeScene's in harnesses/cameras.cpp).
  constexpr uint32_t kSceneVtable = 320, kManager = 400, kManagerVtable = 720, kQueue = 760,
                     kQueueVtable = 800, kList = 840, kSlots = 2048;
  Saved viewports_word(h.base, cm::kViewports, 4);
  Saved scene_word(h.base, kSceneGlobal, 4);
  Saved open_chunks(h.base, command_list::kOpenChunks, 4);
  Saved pass(h.base, kPass0, kPassBytes);
  Saved outs(h.base, kOuts, kOutsBytes);
  Saved eye_out(h.base, kEyeOut, 16);
  Block camera(kCameraBytes), block(16), viewport(kViewportBytes), scene(kSceneBytes);
  for (int c = 0; c < 300; c++) {
    // The camera.
    RandomBytes(h.base, camera.address, kCameraBytes, r);
    RandomFloats(h.base, camera.address + 16, 63, r);
    for (uint32_t at : {100u, 176u}) {
      guest::StoreU32(h.base, camera.address + at, r.Below(8) ? OrdinaryFloat(r, 7.0)
                                                              : ViewFloat(r));
    }
    guest::StoreU32(h.base, camera.address + 180, r.Below(8) ? OrdinaryFloat(r, 20.0)
                                                             : ViewFloat(r));
    guest::StoreU32(h.base, camera.address + 224, r.Below(8) ? OrdinaryFloat(r, 170.0)
                                                             : RandomFloat(r));
    guest::StoreU32(h.base, camera.address + 248, r.Below(8) ? OrdinaryFloat(r, 3.0)
                                                             : ViewFloat(r));
    const auto camera_in = Read(h.base, camera.address, kCameraBytes);
    // Viewport 0 (the block's +4 and +8 random).
    RandomBytes(h.base, viewport.address, kViewportBytes, r);
    RandomFloats(h.base, viewport.address + 1600, 48, r);
    guest::StoreU32(h.base, viewport.address + 1856, OrdinaryFloat(r, 4.0));
    guest::StoreU32(h.base, viewport.address + 1860, r.Below(4));
    const auto viewport_in = Read(h.base, viewport.address, kViewportBytes);
    RandomBytes(h.base, block.address, 16, r);
    guest::StoreU32(h.base, block.address, viewport.address);
    const auto block_in = Read(h.base, block.address, 16);
    // Camera pass 0 as the cameras harness's FromViewport cases make it.
    std::vector<uint8_t> pass_in(kPassBytes);
    for (uint32_t i = 0; i < kPassBytes; i += 4) {
      const uint32_t word = uint32_t(r.Next());
      std::memcpy(pass_in.data() + i, &word, 4);
    }
    const uint32_t type = r.Below(22);
    const uint32_t used = r.Below(1000);
    const uint32_t outs_seed = uint32_t(r.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, camera.address, camera_in.data(), kCameraBytes);
      Write(h.base, viewport.address, viewport_in.data(), kViewportBytes);
      Write(h.base, block.address, block_in.data(), 16);
      guest::StoreU32(h.base, cm::kViewports, block.address);
      Write(h.base, kPass0, pass_in.data(), kPassBytes);
      guest::StoreU32(h.base, kPass0, 0x82001EC0);
      guest::StoreU32(h.base, kPass0 + 584, type);
      guest::StoreU32(h.base, kPass0 + 504, 1280);
      guest::StoreU32(h.base, kPass0 + 508, 720);
      for (uint32_t i = 0; i < kOutsBytes; i += 4) {
        guest::StoreU32(h.base, kOuts + i, outs_seed + i);
      }
      // The stand-in scene, cleared.
      const uint32_t d = scene.address;
      for (uint32_t i = 0; i < kSceneBytes; i += 4) {
        guest::StoreU32(h.base, d + i, 0);
      }
      guest::StoreU32(h.base, d, d + kSceneVtable);
      guest::StoreU32(h.base, d + kSceneVtable + 32, 0x8248CA78);
      guest::StoreU32(h.base, d + 284, d + kManager);
      guest::StoreU32(h.base, d + kManager, d + kManagerVtable);
      guest::StoreU32(h.base, d + kManagerVtable + 4, 0x8248CA78);
      guest::StoreU32(h.base, d + kManagerVtable + 8, 0x82379320);
      guest::StoreU32(h.base, d + kManager + 284, d + kList);
      guest::StoreU32(h.base, d + kManager + 124, d + kQueue);
      guest::StoreU32(h.base, d + kQueue, d + kQueueVtable);
      guest::StoreU32(h.base, d + kQueueVtable + 16, 0x8209F5C8);
      guest::StoreU32(h.base, d + kList + 8, used + 1000);
      guest::StoreU32(h.base, d + kList + 12, used);
      guest::StoreU32(h.base, d + kList + 32, d + kSlots);
      guest::StoreU32(h.base, command_list::kOpenChunks, 0);
      guest::StoreU32(h.base, kSceneGlobal, d);
      PPCContext t = AlignedContext(h);
      FillStackBytes(h.base, t.r1.u32, uint64_t(c) + 1, 2048);
      const uint32_t frame = t.r1.u32 - 272;
      t.r3.u64 = camera.address;
      (v == 0 ? Original<__imp__sub_8217BE98> : hooks::CameraModesStep8217BE98)(t, h.base);
      out[v].Bytes(Read(h.base, camera.address, 108));
      out[v].Float(guest::LoadU32(h.base, camera.address + 108));
      out[v].Bytes(Read(h.base, camera.address + 112, kCameraBytes - 112));
      // The viewport as the cameras harness compares it: everything but the
      // view mode (+1860) as floats (Viewport::LookAt's entries, which the
      // recompiled side runs since LookAt's adapter went).
      out[v].Floats(h.base, viewport.address, 1860 / 4);
      out[v].Word(guest::LoadU32(h.base, viewport.address + 1860));
      out[v].Floats(h.base, viewport.address + 1864, (kViewportBytes - 1864) / 4);
      out[v].Bytes(Read(h.base, kPass0, 16));
      out[v].Floats(h.base, kPass0 + 16, 48);
      out[v].Bytes(Read(h.base, kPass0 + 208, 160));
      out[v].Floats(h.base, kPass0 + 368, 7);
      out[v].Bytes(Read(h.base, kPass0 + 396, 68));
      out[v].Floats(h.base, kPass0 + 464, 8);
      out[v].Bytes(Read(h.base, kPass0 + 496, kPassBytes - 496));
      out[v].Float(guest::LoadU32(h.base, kOuts));
      out[v].Bytes(Read(h.base, kOuts + 4, kOutsBytes - 4));
      out[v].Bytes(Read(h.base, kEyeOut, 16));
      out[v].Bytes(Read(h.base, scene.address, kSceneBytes));
      out[v].Word(guest::LoadU32(h.base, command_list::kOpenChunks));
      // Its frame from +80: the eye and target, the identity and its copy.
      out[v].Bytes(Read(h.base, frame + 80, 160));
      out[v].Word(t.r3.u32);
      viewports_word.Restore();
      scene_word.Restore();
      open_chunks.Restore();
      pass.Restore();
      outs.Restore();
      eye_out.Restore();
    }
    results.Check(out[0], out[1], fmt::format("case {} type {} used {}", c, type, used));
  }
  results.Report();
}

// --- CameraDirector::InitStates: an empty state machine or one with random
// transitions already (fewer than 150, so the 34 fit; keys of small numbers,
// some matching), the original's frame filled with the same random words for
// both (the words it never writes go into the transitions); the director's
// first 9,640 bytes, the frame (but its back chain and saved registers) and
// r3 compared ---

void RandomWords(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

void DirectorInitStates(Context& h, Random& r) {
  namespace gm = game_modes;
  Results results("CameraDirector::InitStates (sub_82159950)");
  Block director(kDirectorBytes);
  const uint32_t frame = CallContext(h).r1.u32 - cm::kInitStatesFrame;
  const uint32_t machine = director.address + cm::kMachine;
  for (int c = 0; c < 24; c++) {
    const uint64_t seed = r.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random w(seed);
      RandomWords(h.base, director.address, 9640, w);
      RandomWords(h.base, frame, cm::kInitStatesFrame, w);
      if (c % 2 == 0) {
        gm::StateMachineConstruct(h.base, machine);
        guest::StoreU32(h.base, machine + gm::kTransitionCount, 0);
      } else {
        guest::StoreU32(h.base, machine + gm::kTransitionCount, w.Below(150));
        for (uint32_t i = 0; i < 200; i++) {
          for (uint32_t k = 0; k < 4; k++) {
            guest::StoreU32(h.base, machine + 48 * i + 4 * k, w.Below(k < 2 ? 60 : 2));
          }
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = director.address;
      (v == 0 ? Original<__imp__sub_82159950> : hooks::CameraModesStep82159950)(t, h.base);
      out[v].Bytes(Read(h.base, director.address, 9640));
      out[v].Bytes(Read(h.base, frame + 80, cm::kInitStatesFrame - 80 - 112));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// --- The cutscene's constructor (the third migration, batch 7) ---

// The movie table swapped for one of the case's: ids 10 to 12, and with
// `there` every logo's movie id (0x8200CD70 + 12 x logo, logos 0 to 24) with
// the movie 0x12345678; room for 32 pairs. Put back by the case's Saved.
void LogosTable(uint8_t* base, bool there) {
  const uint32_t array = memory::crt::OperatorNewArray(base, 256).value_or(0);
  uint32_t count = 0;
  for (uint32_t k = 0; k < 3; k++, count++) {
    guest::StoreU32(base, array + 8 * count, 10 + k);
    guest::StoreU32(base, array + 8 * count + 4, 0x40000000 + k);
  }
  if (there) {
    for (uint32_t logo = 0; logo < 25; logo++, count++) {
      guest::StoreU32(base, array + 8 * count, guest::LoadU32(base, 0x8200CD70 + 12 * logo));
      guest::StoreU32(base, array + 8 * count + 4, 0x12345678);
    }
  }
  guest::StoreU32(base, ui_movies::kMovieTable + 0, array);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 32);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, count);
}

// Cutscene::Cutscene on a cutscene of random bytes: a chase camera's world
// for the viewport and the vehicle (CameraTarget::Track), the race data's
// stage (0 to 2) with 0 to 4 racers (5 in the overflow cases: the fifth id
// is the stage's count word, as the original reads it), six racer records
// (+3936's bit 0x2 mostly set, +3960 the item id), the racers' manager's
// entries (some missing, flag 0x400 random), 400 items (+35 the byte, +88
// the kind halfword), the save data's flags (Stage::Of), the movie table
// with the logos' movies or not, the camera animations' table empty, the
// archive's loads recorded (CameraAnims::Load's and the movie's), the stack
// random (the same for both: the frame words the scripts don't write are
// copied into the lists). The first cases force each event (no racers, -1,
// 27, 28, 181, 182, 376 to 399) and the byte path (0, 1, 2, 3, 0xFF; ids
// in script 1's texture list or not) by giving every racer the same item.
// Compared: the cutscene (floats where the tracker's are), the viewport,
// the near plane, the frame (without its back chain and the saved
// registers' 160 bytes) and the caller's +20, the animations' table, the
// loads, the movie table, r3 and lr.
void CutsceneConstructs(Context& h, Random& r) {
  if (!ui_movies_game_hooks::OpenNative() || !hooks::NativeHeapActive() ||
      !hooks::NativeArchiveQueue()) {
    ITC_LOG_WARN("harness: camera_modes: Cutscene::Cutscene needs the UI movies' Open native "
                 "(--native_heap, --native_c_runtime_heap, the archive's queue)");
    return;
  }
  constexpr uint32_t kManager = 0x826D82F4;
  constexpr uint32_t kVehicleArray = 0x826C2E34;
  constexpr uint32_t kTables = 0x826C5CA0;
  constexpr uint32_t kSaveData = 0x826C5B58;
  constexpr uint32_t kLogo = 10760;
  constexpr uint32_t kCutsceneBytes = kLogo + 96;
  constexpr uint32_t kItems = 400;
  constexpr uint32_t kItemBytes = 3008;
  constexpr uint32_t kSaved = 160;  // f31, r14 to r31 and lr below the caller's frame
  constexpr int16_t kTextureIds[] = {0,   5,   12,  20,  23,  29,  36,  44,  47,  53,  58,  65,
                                     72,  78,  83,  92,  96,  104, 115, 119, 126, 133, 142, 150,
                                     156, 159, 168, 175, 186, 195, 200, 210, 217, 222, 228, 232,
                                     241, 245, 253, 262, 272, 277, 282, 289, 297, 304};
  Saved saved_race(h.base, cm::kRaceData, 4);
  Saved saved_manager(h.base, kManager, 4);
  Saved saved_vehicles(h.base, kVehicleArray, 4);
  Saved saved_tables(h.base, kTables, 4);
  Saved saved_save(h.base, kSaveData, 4);
  Saved saved_near(h.base, kPassNear, 4);
  Saved saved_anims(h.base, cm::kAnims, kTableBytes);
  Block race(kRaceBytes), manager(2400), entries(6 * 32), vehicles(7 * 6272), tables(32),
      items(kItems * kItemBytes), save(6144), cutscene(kCutsceneBytes);
  ChaseWorld w;
  Fill(h.base, race.address, race.bytes, 0);

  // The forced cases: an item id for every racer (-2: no racers) and the
  // item's byte (-1: random).
  struct Target {
    int32_t id;
    int32_t byte;
  };
  std::vector<Target> targets = {{-2, -1}, {-1, -1}, {int32_t(0xFFFF0000u), -1}};
  for (int32_t id : {27, 28, 181, 182}) {
    targets.push_back({id, -1});
  }
  for (int32_t id = 376; id <= 399; id++) {
    targets.push_back({id, -1});
  }
  for (int32_t b : {0, 1, 2, 3, 0xFF}) {
    targets.push_back({kTextureIds[uint32_t(b) % std::size(kTextureIds)], b});
    targets.push_back({301 + b % 7, b});  // not in the texture list
  }
  const int forced = int(targets.size()) * 2;
  const int overflow = 12;
  const int cases = forced + overflow + 240;
  // A random case's item id: the events, script 1's textures, any item, or
  // a negative word (no item; its low halfword the event).
  auto random_id = [&]() -> uint32_t {
    switch (r.Below(6)) {
      case 0: {
        static constexpr int32_t kEvents[] = {-1, 27, 28, 181, 182};
        return uint32_t(kEvents[r.Below(5)]);
      }
      case 1: return 376 + r.Below(24);
      case 2: return uint32_t(kTextureIds[r.Below(uint32_t(std::size(kTextureIds)))]);
      case 3: return 0x80000000u | uint32_t(r.Next());
      default: return r.Below(kItems);
    }
  };
  std::vector<int> scripts(31, 0), logos(26, 0), counts(8, 0);
  int textured = 0;
  Results results("Cutscene::Cutscene (sub_82156CC0)");
  for (int c = 0; c < cases; c++) {
    const bool is_forced = c < forced;
    const bool is_overflow = !is_forced && c < forced + overflow;
    const Target target = is_forced ? targets[size_t(c) % targets.size()] : Target{0, -1};
    // The items: +35 mostly 0 to 3, +88 mostly 0 to 4.
    for (uint32_t k = 0; k < kItems; k++) {
      const uint32_t item = items.address + kItemBytes * k;
      const uint32_t b = r.Below(8);
      guest::StoreU8(h.base, item + 35,
                     uint8_t(b < 6 ? r.Below(4) : b == 6 ? 0xFF : uint32_t(r.Next())));
      guest::StoreU16(h.base, item + 88,
                      uint16_t(r.Below(6) ? r.Below(5) : uint32_t(r.Next())));
    }
    // The stage and its racers.
    const uint32_t stages = race.address + 0x59620;
    const uint32_t stage = r.Below(3);
    guest::StoreU32(h.base, stages + 804, stage);
    const uint32_t at = stages + 80 * stage;
    const uint32_t count = is_overflow                  ? 5
                           : is_forced && target.id == -2 ? 0
                           : is_forced                  ? 1 + r.Below(4)
                                                        : r.Below(5);
    guest::StoreU32(h.base, at + 76, count);
    const std::vector<uint32_t> slots = Permutation(r, 6);
    for (uint32_t s = 0; s < 4; s++) {
      guest::StoreU32(h.base, at + 44 + 8 * s, r.Below(5) ? slots[s] : r.Below(6));
    }
    for (uint32_t id = 0; id < 6; id++) {
      const uint32_t record = race.address + (id + 1) * kRecordBytes;
      const bool set = is_forced || is_overflow || r.Below(4) != 0;
      const uint32_t flags = uint32_t(r.Next());
      guest::StoreU32(h.base, record, set ? (flags | 2) : (flags & ~2u));
      uint32_t item = is_forced ? uint32_t(target.id) : random_id();
      if (is_overflow && id == 5) {
        item = 0;  // the fifth record's halfword over the count's high half: 0
      }
      guest::StoreU32(h.base, record + 24, item);
    }
    if (is_forced && target.byte >= 0) {
      guest::StoreU8(h.base, items.address + kItemBytes * uint32_t(target.id) + 35,
                     uint8_t(target.byte));
    }
    if (is_overflow) {
      // Its byte over the count's third byte: 0 (else the vehicle's store
      // lands far above the frame).
      guest::StoreU8(h.base, items.address + 35, 0);
    }
    // The racers' manager: its entries' ids, flags and vehicles.
    const uint32_t racers = r.Below(4) == 0 ? r.Below(7) : 6;
    guest::StoreU32(h.base, manager.address + 2364, racers);
    const std::vector<uint32_t> ids = Permutation(r, 6);
    for (uint32_t n = 0; n < 6; n++) {
      const uint32_t e = entries.address + 32 * n;
      guest::StoreU32(h.base, manager.address + 2308 + 4 * n, e);
      guest::StoreU32(h.base, e + 0, r.Below(2) ? (0x400 | uint32_t(r.Next()))
                                                : (uint32_t(r.Next()) & ~0x400u));
      guest::StoreU32(h.base, e + 20, r.Below(7));
      guest::StoreU32(h.base, e + 24, ids[n]);
    }
    guest::StoreU32(h.base, tables.address + 16, items.address);
    guest::StoreU32(h.base, save.address + 6092, r.Below(3) ? r.Below(128) : uint32_t(r.Next()));
    w.Randomize(h.base, r);
    RandomBytes(h.base, cutscene.address, cutscene.bytes, r);
    const bool there = r.Below(3) == 0;
    const uint64_t stack_seed = r.Next();
    const auto cutscene_in = Read(h.base, cutscene.address, cutscene.bytes);
    const auto viewport_in = Read(h.base, w.viewport.address, kViewportBytes);
    uint32_t script = 0, logo = 0, records = 0;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Saved table(h.base, ui_movies::kMovieTable, 12);
      Write(h.base, cutscene.address, cutscene_in.data(), cutscene.bytes);
      Write(h.base, w.viewport.address, viewport_in.data(), kViewportBytes);
      guest::StoreU32(h.base, cm::kRaceData, race.address);
      guest::StoreU32(h.base, kManager, manager.address);
      guest::StoreU32(h.base, kVehicleArray, vehicles.address);
      guest::StoreU32(h.base, kTables, tables.address);
      guest::StoreU32(h.base, kSaveData, save.address);
      guest::StoreU32(h.base, cm::kAnimCount, 0);
      LogosTable(h.base, there);
      PPCContext t = AlignedContext(h);
      FillStackBytes(h.base, t.r1.u32, stack_seed, 4096);
      guest::StoreU32(h.base, t.r1.u32 + 20, 0x5EEDF00D);
      const uint32_t frame = t.r1.u32 - cm::kCutsceneFrame;
      ArchiveLoads loads({int64_t(c), int64_t(c + 1), int64_t(c + 2), int64_t(c + 3),
                          int64_t(c + 4), int64_t(c + 5)});
      t.r3.u64 = cutscene.address;
      t.r4.u64 = w.viewport.address;
      t.r5.u64 = w.vehicle.address;
      t.lr = 0x821592B4;
      (v == 0 ? Original<__imp__sub_82156CC0> : hooks::CameraModesStep82156CC0)(t, h.base);
      const uint32_t cs = cutscene.address;
      out[v].Bytes(Read(h.base, cs, 32));
      out[v].Floats(h.base, cs + 32, 56);
      out[v].Bytes(Read(h.base, cs + 256, kCutsceneBytes - 256));
      const uint32_t vp = w.viewport.address;
      out[v].Bytes(Read(h.base, vp, 1600));
      out[v].Floats(h.base, vp + 1600, 48);
      out[v].Bytes(Read(h.base, vp + 1792, kViewportBytes - 1792));
      out[v].Float(guest::LoadU32(h.base, kPassNear));
      out[v].Bytes(Read(h.base, frame + 4, cm::kCutsceneFrame - 4 - kSaved));
      out[v].Word(guest::LoadU32(h.base, t.r1.u32 + 20));
      out[v].Bytes(Read(h.base, cm::kAnims, kTableBytes));
      // The loads: the destinations' pages as 1 (LoadXpr's, the game's
      // buffer [0x826A5134], as it is); a context that's a movie as 2, the
      // logo's XPR holder as 3, CameraAnims::Load's record by its id.
      const std::vector<uint32_t>& calls = loads.calls();
      out[v].Word(uint32_t(calls.size()));
      for (size_t k = 0; k < calls.size(); k++) {
        const uint32_t word = calls[k];
        if (k % 6 == 1 && word != 0 && word != guest::LoadU32(h.base, 0x826A5134) &&
            memory::IsPages(word)) {
          out[v].Word(1);
          memory::FreePages(word);
        } else if (k % 6 == 3 && memory::IsHeapBlock(word)) {
          out[v].Word(2);
        } else if (k % 6 == 3 && word == cs + kLogo + 20) {
          out[v].Word(3);
        } else if (k % 6 == 3 && word != 0 && memory::IsPages(word)) {
          out[v].Word(4);
          out[v].Word(guest::LoadU32(h.base, word));
          memory::FreePages(word);
        } else {
          out[v].Word(word);
        }
      }
      LogoTableOut(h.base, 0x12345678, out[v]);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.lr));
      script = guest::LoadU32(h.base, cs + 352);
      logo = script == 0xFFFFFFFFu ? 0xFFFFFFFFu : guest::LoadU32(h.base, cs + kLogo);
      records = guest::LoadU32(h.base, frame + cm::kCutsceneRecordCount);
      saved_race.Restore();
      saved_manager.Restore();
      saved_vehicles.Restore();
      saved_tables.Restore();
      saved_save.Restore();
      saved_near.Restore();
      saved_anims.Restore();
    }
    scripts[script < 30 ? script : 30]++;
    logos[logo < 25 ? logo : 25]++;
    counts[records < 7 ? records : 7]++;
    if (script == 1 && int32_t(guest::LoadU32(h.base, cutscene.address + kLogo + 0)) == 0) {
      textured++;
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: racers {} item {} script {} logo {} there {}", c, count,
                              is_forced ? target.id : -3, int32_t(script), int32_t(logo),
                              there));
  }
  results.Report();
  auto list = [](const std::vector<int>& seen) {
    std::string text;
    for (size_t i = 0; i < seen.size(); i++) {
      text += fmt::format("{}{}:{}", i ? " " : "", i, seen[i]);
    }
    return text;
  };
  ITC_LOG_INFO("harness: Cutscene::Cutscene scripts (30: none) {}", list(scripts));
  ITC_LOG_INFO("harness: Cutscene::Cutscene logos (25: none) {}", list(logos));
  ITC_LOG_INFO("harness: Cutscene::Cutscene record counts (7: more) {}; script 1 cases {}",
               list(counts), textured);
}

// --- CameraDirector::SetMode: views 0 to 7 (and a wild one), split screen
// or not, the followed player 0 to 3 (2 and 3 read the frame's next words,
// as the original does), the two racers there or not (bit 0x40, a null
// racer); the six constructors answered by a stand-in that records its
// function, registers, stack pointer and return address (their own
// harnesses cover them); the director's words, both viewports, the frame's
// two vehicles, the calls and r3 compared ---

struct ConstructCall {
  uint32_t words[6];
};
std::vector<ConstructCall>* g_construct_calls = nullptr;
// The stand-in's answers when not 0 (the handlers' harness: camera objects
// whose slot 0 its Update then calls), the first call's then the second's.
uint32_t g_construct_answers[2] = {0, 0};

uint32_t RecordConstruct(uint32_t function, uint32_t r3, uint32_t r4, uint32_t r5, uint32_t r1,
                         uint32_t lr) {
  size_t n = 0;
  if (g_construct_calls) {
    n = g_construct_calls->size();
    g_construct_calls->push_back({{function, r3, r4, r5, r1, lr}});
  }
  if (g_construct_answers[0] != 0) {
    return g_construct_answers[n == 0 ? 0 : 1];
  }
  return (r3 & 1) ? 0 : r3 ^ 0x13579BDF;
}

void DirectorSetModes(Context& h, Random& r) {
  constexpr uint32_t kManager = 0x826D82F4;
  constexpr uint32_t kVehicleArray = 0x826C2E34;
  Results results("CameraDirector::SetMode (sub_821591F0)");
  Saved saved_viewports(h.base, cm::kViewports, 4);
  Saved saved_manager(h.base, kManager, 4);
  Saved saved_vehicles(h.base, kVehicleArray, 4);
  Block director(kDirectorBytes), holder(12), first(kViewportBytes), second(kViewportBytes),
      manager(2400), racers(64), vehicles(7 * 6272);
  hooks::SetCameraConstructStandIn(RecordConstruct);
  const uint32_t frame = CallContext(h).r1.u32 - 176;
  for (int c = 0; c < 300; c++) {
    const uint32_t view = c < 16 ? uint32_t(c % 8) : r.Below(12) ? r.Below(8) : uint32_t(r.Next());
    const uint32_t split = r.Below(2) ? 0 : (r.Below(4) ? 1 : uint32_t(r.Next()));
    const uint32_t followed = r.Below(4) ? r.Below(2) : 2 + r.Below(2);
    const uint64_t seed = r.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random w(seed);
      RandomBytes(h.base, director.address + cm::kView - 16, 32, w);
      RandomBytes(h.base, director.address + cm::kCurrentCamera, 8, w);
      guest::StoreU32(h.base, director.address + cm::kFollowed, followed);
      RandomBytes(h.base, first.address, kViewportBytes, w);
      RandomBytes(h.base, second.address, kViewportBytes, w);
      RandomFloats(h.base, first.address + 1600, 48, w);
      RandomFloats(h.base, second.address + 1600, 48, w);
      guest::StoreU32(h.base, holder.address + 0, first.address);
      guest::StoreU32(h.base, holder.address + 4, second.address);
      guest::StoreU32(h.base, holder.address + 8, split);
      guest::StoreU32(h.base, cm::kViewports, holder.address);
      for (uint32_t i = 0; i < 2; i++) {
        const uint32_t racer = racers.address + 32 * i;
        const uint32_t pick = w.Below(5);
        guest::StoreU32(h.base, manager.address + (577 + i) * 4, pick == 0 ? 0 : racer);
        guest::StoreU32(h.base, racer, pick == 1 ? (uint32_t(w.Next()) & ~0x40u)
                                                 : (uint32_t(w.Next()) | 0x40u));
        guest::StoreU32(h.base, racer + 20, w.Below(7));
      }
      guest::StoreU32(h.base, kManager, manager.address);
      guest::StoreU32(h.base, kVehicleArray, w.Below(8) ? vehicles.address : 0);
      RandomBytes(h.base, frame + 80, 16, w);
      std::vector<ConstructCall> calls;
      g_construct_calls = &calls;
      PPCContext t = CallContext(h);
      t.r3.u64 = director.address;
      t.r4.u64 = view;
      (v == 0 ? Original<__imp__sub_821591F0> : hooks::CameraModesStep821591F0)(t, h.base);
      g_construct_calls = nullptr;
      out[v].Bytes(Read(h.base, director.address + cm::kView - 16, 32));
      out[v].Bytes(Read(h.base, director.address + cm::kCurrentCamera, 8));
      out[v].Bytes(Read(h.base, first.address, kViewportBytes));
      out[v].Bytes(Read(h.base, second.address, kViewportBytes));
      out[v].Bytes(Read(h.base, frame + 80, 16));
      out[v].Word(uint32_t(calls.size()));
      for (const ConstructCall& call : calls) {
        for (uint32_t word : call.words) {
          out[v].Word(word);
        }
      }
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: view {} split {} followed {}", c, view, split, followed));
  }
  hooks::SetCameraConstructStandIn(nullptr);
  results.Report();
}

// --- The director's handlers OnRaceStart, Resume and Update: a world of
// the director, both viewports (split screen or not), the race data's kind
// (4, 6 or another) and TV flag (+0x59AE4 bit 0x80), its current stage's
// first word (1 or not), [0x826C7824], the save data's camera setting, the
// color filter (there or not), the players' car input records (the buttons
// 0x40 and 0x80, the stick halfword) and the two racers for SetMode; the
// views' constructors answered by SetMode's stand-in, the camera objects'
// slot 0 the empty sub_82106540 and the TV camera with no route ranges (so
// TvCamera::Track only writes its frame); the director's words, both
// viewports, the color filter's words, the frames' words and the stand-in's
// calls compared ---

void DirectorHandlers(Context& h, Random& r) {
  constexpr uint32_t kManager = 0x826D82F4;
  constexpr uint32_t kVehicleArray = 0x826C2E34;
  constexpr uint32_t kSaveData = 0x826C5B58;
  constexpr uint32_t kTwoPlayer = 0x826C7824;
  constexpr uint32_t kInputs = 0x826C5B30;
  Saved saved_viewports(h.base, cm::kViewports, 4);
  Saved saved_manager(h.base, kManager, 4);
  Saved saved_vehicles(h.base, kVehicleArray, 4);
  Saved saved_race(h.base, cm::kRaceData, 4);
  Saved saved_save(h.base, kSaveData, 4);
  Saved saved_two(h.base, kTwoPlayer, 4);
  Saved saved_inputs(h.base, kInputs, 16);
  Saved saved_filter(h.base, post_effects::kEffects + 8, 4);
  Block director(kDirectorBytes), holder(12), first(kViewportBytes), second(kViewportBytes),
      manager(2400), racers(64), vehicles(7 * 6272), race(kRaceBytes), save(0x1F900),
      filter(8200), vtable(64), inputs(4 * 32), camera_vtable(8), cameras(64);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(h.base, vtable.address + 4 * i, 0);
  }
  guest::StoreU32(h.base, vtable.address + 16, 0x821350F8);
  guest::StoreU32(h.base, vtable.address + 24, 0x82135138);
  guest::StoreU32(h.base, vtable.address + 28, 0x82135140);
  guest::StoreU32(h.base, camera_vtable.address, 0x82106540);
  guest::StoreU32(h.base, camera_vtable.address + 4, 0x82106540);
  guest::StoreU32(h.base, cameras.address, camera_vtable.address);
  guest::StoreU32(h.base, cameras.address + 32, camera_vtable.address);
  hooks::SetCameraConstructStandIn(RecordConstruct);
  g_construct_answers[0] = cameras.address;
  g_construct_answers[1] = cameras.address + 32;
  // The world for one case, from the case's random generator.
  auto world = [&](Random& w) {
    RandomBytes(h.base, director.address + cm::kView - 16, 32, w);
    guest::StoreU32(h.base, director.address + cm::kView, w.Below(8) ? w.Below(6) : w.Below(9));
    guest::StoreU32(h.base, director.address + cm::kViewKept, w.Below(6));
    RandomBytes(h.base, director.address + 12960, 4, w);
    RandomBytes(h.base, director.address + 13356, 4, w);
    guest::StoreU32(h.base, director.address + 13360 + 452, 0);  // no route ranges
    guest::StoreU32(h.base, director.address + cm::kCurrentCamera, cameras.address);
    guest::StoreU32(h.base, director.address + cm::kCurrentCamera2,
                    w.Below(2) ? cameras.address + 32 : 0);
    guest::StoreU32(h.base, director.address + cm::kFollowed, w.Below(2));
    guest::StoreU32(h.base, director.address + cm::kFollowedInput,
                    (uint32_t(w.Next()) & ~0xFFu) | w.Below(4));
    RandomBytes(h.base, first.address, kViewportBytes, w);
    RandomBytes(h.base, second.address, kViewportBytes, w);
    RandomFloats(h.base, first.address + 1600, 48, w);
    RandomFloats(h.base, second.address + 1600, 48, w);
    // The view modes 0-5 only (a view past 5 would leave Update a null camera
    // to call, in the original too).
    guest::StoreU32(h.base, first.address + 1860, w.Below(6));
    guest::StoreU32(h.base, holder.address + 0, first.address);
    guest::StoreU32(h.base, holder.address + 4, second.address);
    guest::StoreU32(h.base, holder.address + 8, w.Below(3) ? 0 : 1);
    guest::StoreU32(h.base, cm::kViewports, holder.address);
    for (uint32_t i = 0; i < 2; i++) {
      const uint32_t racer = racers.address + 32 * i;
      guest::StoreU32(h.base, manager.address + (577 + i) * 4, w.Below(6) ? racer : 0);
      guest::StoreU32(h.base, racer, uint32_t(w.Next()) | 0x40u);
      guest::StoreU32(h.base, racer + 20, w.Below(7));
    }
    guest::StoreU32(h.base, kManager, manager.address);
    guest::StoreU32(h.base, kVehicleArray, vehicles.address);
    const uint32_t kinds[] = {4, 6, 6, 1, 2, 3};
    guest::StoreU32(h.base, race.address + 0xA3AF0, kinds[w.Below(6)]);
    guest::StoreU32(h.base, race.address + 0x59AE4, uint32_t(w.Next()));
    const uint32_t stages = race.address + 0x59620;
    const uint32_t stage = w.Below(4);
    guest::StoreU32(h.base, stages + 804, stage);
    RandomBytes(h.base, stages + 80 * stage, 80, w);
    guest::StoreU32(h.base, stages + 80 * stage, w.Below(2) ? 1 : w.Below(4));
    guest::StoreU32(h.base, cm::kRaceData, race.address);
    guest::StoreU32(h.base, save.address + 0x1F85C + 24, w.Below(5) ? w.Below(4) : uint32_t(w.Next()));
    guest::StoreU32(h.base, kSaveData, save.address);
    guest::StoreU32(h.base, kTwoPlayer, w.Below(2) ? 0 : uint32_t(w.Next()));
    RandomBytes(h.base, filter.address + 8064, 64, w);
    guest::StoreU32(h.base, filter.address, vtable.address);
    guest::StoreU32(h.base, post_effects::kEffects + 8, w.Below(5) ? filter.address : 0);
    for (uint32_t p = 0; p < 4; p++) {
      const uint32_t input = inputs.address + 32 * p;
      RandomBytes(h.base, input, 32, w);
      guest::StoreU32(h.base, kInputs + 4 * p, input);
    }
  };
  auto outputs = [&](Outputs& out, uint32_t frame, uint32_t frame_bytes) {
    out.Bytes(Read(h.base, director.address + cm::kView - 16, 32));
    out.Floats(h.base, director.address + 12960, 1);
    out.Bytes(Read(h.base, director.address + 13356, 4));
    out.Bytes(Read(h.base, director.address + cm::kCurrentCamera, 8));
    out.Bytes(Read(h.base, director.address + cm::kFollowedInput, 4));
    out.Bytes(Read(h.base, first.address, kViewportBytes));
    out.Bytes(Read(h.base, second.address, kViewportBytes));
    out.Word(guest::LoadU32(h.base, holder.address + 8));
    out.Floats(h.base, filter.address + 8064, 16);
    out.Bytes(Read(h.base, frame + 80, frame_bytes - 80));
  };
  struct Handler {
    const char* name;
    void (*original)(PPCContext&, uint8_t*);
    void (*native)(PPCContext&, uint8_t*);
    uint32_t frame;    // the original's frame
    uint32_t compared; // the frame's bytes compared (up to its saved registers)
  };
  const Handler handlers[] = {
      {"CameraDirector::OnRaceStart (sub_821596B0)", Original<__imp__sub_821596B0>,
       hooks::CameraModesStep821596B0, 224, 160},
      {"CameraDirector::Resume (sub_821597F0)", Original<__imp__sub_821597F0>,
       hooks::CameraModesStep821597F0, 240, 160},
      {"CameraDirector::Update (sub_821594B8)", Original<__imp__sub_821594B8>,
       hooks::CameraModesStep821594B8, 176, 120},
  };
  for (const Handler& handler : handlers) {
    Results results(handler.name);
    const uint32_t frame = CallContext(h).r1.u32 - handler.frame;
    for (int c = 0; c < 300; c++) {
      const uint64_t seed = r.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random w(seed);
        world(w);
        RandomBytes(h.base, frame, handler.frame, w);
        std::vector<ConstructCall> calls;
        g_construct_calls = &calls;
        PPCContext t = CallContext(h);
        t.r3.u64 = director.address;
        (v == 0 ? handler.original : handler.native)(t, h.base);
        g_construct_calls = nullptr;
        outputs(out[v], frame, handler.compared);
        out[v].Word(uint32_t(calls.size()));
        for (const ConstructCall& call : calls) {
          for (uint32_t word : call.words) {
            out[v].Word(word);
          }
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  hooks::SetCameraConstructStandIn(nullptr);
  g_construct_answers[0] = g_construct_answers[1] = 0;
}

}  // namespace

void CameraModes(Context& h) {
  Random& r = h.random;
  Handlers(h, r);
  Small(h, r);
  Lists(h, r);
  Anims(h, r);
  Filter(h, r);
  AnimFiles(h, r);
  AnimFrees(h, r);
  AnimEvaluation(h, r);
  Paths(h, r);
  RivalLogo(h, r);
  Track(h, r);
  Shakes(h, r);
  IntroShots(h, r);
  DirectorStartRelease(h, r);
  Cutscenes(h, r);
  RoutePaths(h, r);
  ChaseCameras(h, r);
  ViewCameras(h, r);
  IntroUpdates(h, r);
  RivalLogoOpenClose(h, r);
  CutsceneSteps(h, r);
  TvConstructs(h, r);
  TvSteps(h, r);
  GarageCameras(h, r);
  DirectorInitStates(h, r);
  DirectorSetModes(h, r);
  DirectorHandlers(h, r);
  CutsceneConstructs(h, r);
}

}  // namespace harness
