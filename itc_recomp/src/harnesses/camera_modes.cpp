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

#include "base/flags.h"
#include "base/log.h"
#include "camera_modes/camera_modes.h"
#include "hooks/camera_modes.h"
#include "memory/heap.h"
#include "mode_tick/mode_tick.h"
#include "post_effects/post_effects.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <limits>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_camera_modes);

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

}  // namespace

void CameraModes(Context& h) {
  Random& r = h.random;
  Handlers(h, r);
  Small(h, r);
  Lists(h, r);
  Anims(h, r);
  Filter(h, r);
}

}  // namespace harness
