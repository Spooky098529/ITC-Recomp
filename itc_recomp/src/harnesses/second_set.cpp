// The second set's objects (src/second_set/, docs/functions/second-set.md)
// against the recompiled functions. Each case fills a block of guest memory
// and the stack below the call's r1 with random bytes, writes its inputs,
// runs the original (with --native_second_set off, so its callees are the
// originals too) and the register steps (hooks/second_set.h) from the same
// inputs, and compares the block, the frames' words and r3. The point
// records' matrices are only copied (no float math), so they're compared
// exactly.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/second_set.h"
#include "memory/heap.h"
#include "second_set/points.h"

#include <fmt/format.h>

#include <functional>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_second_set);

namespace harness {

namespace {

namespace ss = second_set;

constexpr uint32_t kBlockBytes = 0x10000;
constexpr uint32_t kStackBytes = 1024;  // below the call's r1, compared

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

void Put(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

void PutString(uint8_t* base, uint32_t address, const std::string& s) {
  Write(base, address, s.c_str(), uint32_t(s.size() + 1));
}

std::string RandomName(Random& r, uint32_t length) {
  static const char kChars[] = "abcdefgxyzABZ_0129*";
  std::string name;
  for (uint32_t i = 0; i < length; i++) {
    name += kChars[r.Below(sizeof(kChars) - 1)];
  }
  return name;
}

// `name` with some letters' case flipped, some characters made `*`, or one
// changed.
std::string Variant(Random& r, std::string name) {
  for (char& ch : name) {
    switch (r.Below(8)) {
      case 0:
        if (ch >= 'a' && ch <= 'z') {
          ch = char(ch - 32);
        } else if (ch >= 'A' && ch <= 'Z') {
          ch = char(ch + 32);
        }
        break;
      case 1: ch = '*'; break;
      default: break;
    }
  }
  if (!name.empty() && r.Below(4) == 0) {
    name[r.Below(uint32_t(name.size()))] = char('a' + r.Below(26));
  }
  return name;
}

// One side: --native_second_set off for the recompiled one.
void RunSide(int v, GuestFunction recompiled, GuestFunction native, PPCContext& t,
             uint8_t* base) {
  ITC_FLAG(native_second_set) = v == 1;
  (v == 0 ? recompiled : native)(t, base);
  ITC_FLAG(native_second_set) = true;
}

// One case through both versions: `bytes` of the block and the stack below
// r1 filled at random, `setup(t)` writing the inputs and the registers (once:
// they're written back for the second run).
void Case(Context& h, Results& results, uint32_t block, uint32_t bytes, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, const std::string& what) {
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  const uint32_t stack = start.r1.u32 - kStackBytes;
  RandomBytes(h.base, block, bytes, r);
  RandomBytes(h.base, stack, kStackBytes, r);
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, bytes);
  const std::vector<uint8_t> frames = Read(h.base, stack, kStackBytes);
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), bytes);
    Write(h.base, stack, frames.data(), kStackBytes);
    PPCContext t = start;
    RunSide(v, recompiled, native, t, h.base);
    out[v].Bytes(Read(h.base, block, bytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

// The frames' words compared, the saved registers' and v127's slots left out
// (the plain C++ doesn't write them): a case through both versions with the
// words at r1 - `below` + [0, bytes) compared, for each window.
struct Window {
  uint32_t below;
  uint32_t bytes;
};

void FrameCase(Context& h, Results& results, uint32_t block, uint32_t bytes, Random& r,
               const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
               GuestFunction native, const std::vector<Window>& windows,
               const std::string& what) {
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  const uint32_t stack = start.r1.u32 - kStackBytes;
  RandomBytes(h.base, block, bytes, r);
  RandomBytes(h.base, stack, kStackBytes, r);
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, bytes);
  const std::vector<uint8_t> frames = Read(h.base, stack, kStackBytes);
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), bytes);
    Write(h.base, stack, frames.data(), kStackBytes);
    PPCContext t = start;
    RunSide(v, recompiled, native, t, h.base);
    out[v].Bytes(Read(h.base, block, bytes));
    for (const Window& w : windows) {
      out[v].Bytes(Read(h.base, start.r1.u32 - w.below, w.bytes));
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

// Clear's frame words (+80 to +160) under a caller's frame of `outer` bytes.
Window ClearWindow(uint32_t outer) {
  return {outer + ss::kClearFrameBytes - 80, 80};
}

// --- The point records ---

void Points(Context& h, Random& r, uint32_t block) {
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "PointRecord::SetNode (sub_820CE8D8)"
                               : "PointRecord::SetShape (sub_820CE900)");
    for (int c = 0; c < 32; c++) {
      Case(
          h, results, block, 0x400, r,
          [&](PPCContext& t) {
            PutString(h.base, block + 0x300, RandomName(r, c == 0 ? 0 : r.Below(31)));
            t.r3.u64 = block;
            t.r4.u64 = block + 0x300;
          },
          which == 0 ? __imp__sub_820CE8D8 : __imp__sub_820CE900,
          which == 0 ? hooks::SecondSetPointSetNode : hooks::SecondSetPointSetShape,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("PointRecord::Clear (sub_820CEA08)");
    for (int c = 0; c < 16; c++) {
      FrameCase(
          h, results, block, 0x400, r,
          [&](PPCContext& t) {
            t.r3.u64 = block + 16 * r.Below(8);
          },
          __imp__sub_820CEA08, hooks::SecondSetPointClear, {ClearWindow(0)},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("PointRecord::PointRecord (sub_820D0270)");
    for (int c = 0; c < 16; c++) {
      FrameCase(
          h, results, block, 0x400, r,
          [&](PPCContext& t) {
            t.r3.u64 = block + 16 * r.Below(8);
          },
          __imp__sub_820D0270, hooks::SecondSetPointConstruct,
          {ClearWindow(ss::kConstructFrameBytes)}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // A part model at +0x400: +408 nodes (0 to 8, now and then negative)
    // of 192 bytes at [+36] (+0x800), each +40 a shape index (-1 now and
    // then) into 544-byte shapes at [+40] (+0x1400), each named at +0; the
    // record's +192 one of the names, a variant of one or another.
    Results results("ModelPoint::FindShape (sub_820CEB30)");
    static const char* kNames[] = {"fb00punch_shape", "fog**_shape", "base_shape",
                                   "target_00_shape", "hl**cover_shape", "x"};
    const uint32_t record = block, model = block + 0x400;
    const uint32_t nodes = block + 0x800, shapes = block + 0x1400;
    for (int c = 0; c < 64; c++) {
      FrameCase(
          h, results, block, 0x3000, r,
          [&](PPCContext& t) {
            const uint32_t count = r.Below(9);
            Put(h.base, model + 408, r.Below(16) == 0 ? uint32_t(-int32_t(r.Below(3))) : count);
            Put(h.base, model + 36, nodes);
            Put(h.base, model + 40, shapes);
            std::vector<std::string> names;
            for (uint32_t s = 0; s < 8; s++) {
              names.push_back(r.Below(4) == 0 ? RandomName(r, r.Below(12))
                                              : kNames[r.Below(std::size(kNames))]);
              PutString(h.base, shapes + 544 * s, names.back());
            }
            for (uint32_t n = 0; n < 8; n++) {
              Put(h.base, nodes + 192 * n + 40, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(8));
            }
            std::string wanted = names[r.Below(8)];
            switch (r.Below(4)) {
              case 0: wanted = Variant(r, wanted); break;
              case 1: wanted = RandomName(r, r.Below(12)); break;
              default: break;
            }
            PutString(h.base, record + 192, wanted);
            t.r3.u64 = record;
            t.r4.u64 = model;
          },
          __imp__sub_820CEB30, hooks::SecondSetFindShape,
          {{ss::kFindShapeFrameBytes - 80, 64}}, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// The point groups' resets and inits: the group (or record) at the block,
// its words random; the names from the image's tables. The block, the last
// Clear's frame words (and ResetHeadlights' identity) and r3 compared.
void Groups(Context& h, Random& r, uint32_t block) {
  struct Group {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t frame;  // the function's own frame
    uint32_t bytes;  // what it touches from r3
    bool init;       // a single record's init (Clear under PointRecord's too)
  };
  const Group groups[] = {
      {"PartsPoints::ResetFrontBumper (sub_820DE0E0)", __imp__sub_820DE0E0,
       hooks::SecondSetPartsResetFrontBumper, 112, 2656, false},
      {"PartsPoints::ResetRearBumper (sub_820DE2C8)", __imp__sub_820DE2C8,
       hooks::SecondSetPartsResetRearBumper, 112, 1696, false},
      {"PartsPoints::ResetRearSpoiler (sub_820DE498)", __imp__sub_820DE498,
       hooks::SecondSetPartsResetRearSpoiler, 112, 720, false},
      {"PartsPoints::ResetBonnet (sub_820DE548)", __imp__sub_820DE548,
       hooks::SecondSetPartsResetBonnet, 128, 480, false},
      {"PartsPoints::ResetLeftDoor (sub_820DE600)", __imp__sub_820DE600,
       hooks::SecondSetPartsResetLeftDoor, 128, 6096, false},
      {"PartsPoints::ResetRightDoor (sub_820DE7E8)", __imp__sub_820DE7E8,
       hooks::SecondSetPartsResetRightDoor, 128, 6096, false},
      {"PartsPoints::ResetDoors (sub_820DE9D0)", __imp__sub_820DE9D0,
       hooks::SecondSetPartsResetDoors, 112, 5520, false},
      {"PartsPoints::ResetHeadlights (sub_820DF740)", __imp__sub_820DF740,
       hooks::SecondSetPartsResetHeadlights, 192, 3200, false},
      {"PartsPoints::ResetTailLights (sub_820DEBF0)", __imp__sub_820DEBF0,
       hooks::SecondSetPartsResetTailLights, 128, 3600, false},
      {"PartsPoints::ResetFenders (sub_820DECD0)", __imp__sub_820DECD0,
       hooks::SecondSetPartsResetFenders, 112, 1680, false},
      {"PartsPoints::ResetCabin (sub_820DEE88)", __imp__sub_820DEE88,
       hooks::SecondSetPartsResetCabin, 112, 1200, false},
      {"PartsPoints::ResetEngine (sub_820DF020)", __imp__sub_820DF020,
       hooks::SecondSetPartsResetEngine, 112, 480, false},
      {"PartsPoints::ResetMufflers (sub_820DF0C0)", __imp__sub_820DF0C0,
       hooks::SecondSetPartsResetMufflers, 128, 1456, false},
      {"PartsPoints::ResetShadows (sub_820DF128)", __imp__sub_820DF128,
       hooks::SecondSetPartsResetShadows, 112, 976, false},
      {"PartsPoints::InitCooling (sub_820DF410)", __imp__sub_820DF410,
       hooks::SecondSetPartsInitCooling, 96, 240, true},
      {"PartsPoints::InitSideSkirts (sub_820DF570)", __imp__sub_820DF570,
       hooks::SecondSetPartsInitSideSkirts, 96, 240, true},
      {"PartsPoints::InitLeftMirror (sub_820DF668)", __imp__sub_820DF668,
       hooks::SecondSetPartsInitLeftMirror, 96, 240, true},
      {"PartsPoints::InitRightMirror (sub_820DF6B0)", __imp__sub_820DF6B0,
       hooks::SecondSetPartsInitRightMirror, 96, 240, true},
      {"PartsPoints::InitMirror (sub_820DF6F8)", __imp__sub_820DF6F8,
       hooks::SecondSetPartsInitMirror, 96, 240, true},
      {"CockpitPoints::InitBonnet (sub_820DC0A8)", __imp__sub_820DC0A8,
       hooks::SecondSetCockpitInitBonnet, 96, 240, true},
      {"CockpitPoints::InitMirror (sub_820DC0F0)", __imp__sub_820DC0F0,
       hooks::SecondSetCockpitInitMirror, 96, 240, true},
      {"CockpitPoints::ResetFenders (sub_820DB878)", __imp__sub_820DB878,
       hooks::SecondSetCockpitResetFenders, 112, 720, false},
      {"CockpitPoints::ResetCabin (sub_820DB930)", __imp__sub_820DB930,
       hooks::SecondSetCockpitResetCabin, 112, 480, false},
      {"CockpitPoints::ResetDoors (sub_820DB9E8)", __imp__sub_820DB9E8,
       hooks::SecondSetCockpitResetDoors, 112, 480, false},
      {"CockpitPoints::ResetHeadlights (sub_820DBAA0)", __imp__sub_820DBAA0,
       hooks::SecondSetCockpitResetHeadlights, 128, 1920, false},
  };
  for (const Group& g : groups) {
    Results results(g.name);
    std::vector<Window> windows = {ClearWindow(g.frame)};
    if (g.init) {
      windows.push_back(ClearWindow(g.frame + ss::kConstructFrameBytes));
    }
    if (g.frame == 192) {
      windows.push_back({192 - 80, 64});  // ResetHeadlights' identity
    }
    for (int c = 0; c < 4; c++) {
      FrameCase(
          h, results, block, 0x2000, r,
          [&](PPCContext& t) { t.r3.u64 = block + 16 * r.Below(8); }, g.recompiled, g.native,
          windows, fmt::format("case {} ({} bytes)", c, g.bytes));
    }
    results.Report();
  }
}

}  // namespace

void SecondSet(Context& h) {
  Random r(0x7365636F6E64ull);  // "second"
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Points(h, r, block);
  Groups(h, r, block);
  memory::Free(allocation);
}

}  // namespace harness
