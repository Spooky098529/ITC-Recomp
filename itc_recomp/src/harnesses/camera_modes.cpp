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
#include "hooks/archive.h"
#include "hooks/c_runtime.h"
#include "hooks/camera_modes.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "mode_tick/mode_tick.h"
#include "post_effects/post_effects.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
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
  Block camera{512}, viewport{kViewportBytes}, vehicle{6272}, body{512}, object{2048};
  Block vectors{128};
  void Randomize(uint8_t* base, Random& r) {
    RandomFloats(base, camera.address + 32, 88, r);
    RandomBytes(base, camera.address + 256, 96, r);
    RandomFloats(base, camera.address + 352, 7, r);
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
      const auto camera_in = Read(h.base, w.camera.address, 512);
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
        Write(h.base, w.camera.address, camera_in.data(), 512);
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
}

}  // namespace harness
