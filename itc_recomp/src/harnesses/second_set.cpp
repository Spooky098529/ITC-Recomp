// The second set's objects (src/second_set/, docs/functions/second-set.md)
// against the recompiled functions. Each case fills a block of guest memory
// and the stack below the call's r1 with random bytes, writes its inputs,
// runs the original (with --native_second_set off, so its callees are the
// originals too) and the register steps (hooks/second_set.h) from the same
// inputs, and compares the block, the frames' words and r3. The point
// records' matrices are only copied (no float math), so they're compared
// exactly.

#include "harness.h"
#include "car_model_inputs.h"
#include "pending_frees.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/models.h"
#include "hooks/second_set.h"
#include "models/memory_blocks.h"
#include "native/buffers.h"
#include "scene/scene.h"
#include "scene_loaders/objects.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"
#include "second_set/blocks.h"
#include "second_set/finds.h"
#include "second_set/morph.h"
#include "second_set/objects.h"
#include "second_set/points.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <mutex>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_second_set);
ITC_FLAG_DECLARE(bool, native_scene_loaders);
ITC_FLAG_DECLARE(bool, native_material_passes);
ITC_FLAG_DECLARE(bool, native_car_models);

namespace harness {

namespace {

namespace ss = second_set;

constexpr uint32_t kBlockBytes = 0x20000;
constexpr uint32_t kStackBytes = 1024;  // below the call's r1, compared
constexpr uint32_t kPointRecordBytes = ss::kPointRecordBytes;

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
    bool frames = true;  // its Clear's frame words compared (not for nested ones)
  };
  // The inits only PartsPoints::PartsPoints calls (no adapters).
#define ITC_GROUP_STEP(function)                                 \
  [](PPCContext& c, uint8_t* b) {                                \
    c.fpscr.disableFlushMode();                                  \
    c.r3.u64 = ss::function(b, c.r3.u32, c.r1.u32);              \
  }
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
      {"InteriorPoints::InitBody (sub_820E39B0)", __imp__sub_820E39B0,
       hooks::SecondSetInteriorInitBody, 96, 240, true},
      {"InteriorPoints::ResetDials (sub_820E2FC8)", __imp__sub_820E2FC8,
       hooks::SecondSetInteriorResetDials, 128, 2896, false},
      {"InteriorPoints::ResetMeters (sub_820E3200)", __imp__sub_820E3200,
       hooks::SecondSetInteriorResetMeters, 144, 5328, false},
      {"InteriorPoints::ResetSteering (sub_820E33F0)", __imp__sub_820E33F0,
       hooks::SecondSetInteriorResetSteering, 112, 976, false},
      {"InteriorPoints::ResetSeats (sub_820E3440)", __imp__sub_820E3440,
       hooks::SecondSetInteriorResetSeats, 128, 976, false},
      {"InteriorPoints::ResetRollCage (sub_820E34F8)", __imp__sub_820E34F8,
       hooks::SecondSetInteriorResetRollCage, 112, 976, false},
      {"LightPoints::ResetFront (sub_820DC748)", __imp__sub_820DC748,
       hooks::SecondSetLightsResetFront, 112, 4336, false},
      {"LightPoints::ResetSides (sub_820DC7A0)", __imp__sub_820DC7A0,
       hooks::SecondSetLightsResetSides, 112, 4336, false},
      {"LightPoints::ResetRear (sub_820DC7F8)", __imp__sub_820DC7F8,
       hooks::SecondSetLightsResetRear, 112, 4336, false},
      {"TrafficPoints::ResetBody (sub_820E54E8)", __imp__sub_820E54E8,
       hooks::SecondSetTrafficResetBody, 112, 1440, false},
      {"TrafficPoints::ResetHeadlights (sub_820E56D0)", __imp__sub_820E56D0,
       hooks::SecondSetTrafficResetHeadlights, 128, 1680, false},
      {"TrafficPoints::ResetTailLights (sub_820E57A0)", __imp__sub_820E57A0,
       hooks::SecondSetTrafficResetTailLights, 128, 2880, false},
      {"WheelPoints::ResetTire (sub_820E74B8)", __imp__sub_820E74B8,
       hooks::SecondSetWheelsResetTire, 128, 1840, false},
      {"WheelPoints::ResetRimA (sub_820E7918)", __imp__sub_820E7918,
       hooks::SecondSetWheelsResetRimA, 144, 1744, false},
      {"WheelPoints::ResetRimB (sub_820E7C08)", __imp__sub_820E7C08,
       hooks::SecondSetWheelsResetRimB, 128, 1744, false},
      {"WheelPoints::ResetBrakes (sub_820E7F90)", __imp__sub_820E7F90,
       hooks::SecondSetWheelsResetBrakes, 128, 2944, false},
      {"PartsPoints::InitLeftDoor (sub_820DF5B8)", __imp__sub_820DF5B8,
       ITC_GROUP_STEP(PartsInitLeftDoor), 112, 6096, false, false},
      {"PartsPoints::InitRightDoor (sub_820DF610)", __imp__sub_820DF610,
       ITC_GROUP_STEP(PartsInitRightDoor), 112, 6096, false, false},
      {"PartsPoints::InitHeadlights (sub_820E1388)", __imp__sub_820E1388,
       ITC_GROUP_STEP(PartsInitHeadlights), 112, 3200, false, false},
      {"PartsPoints::InitMufflers (sub_820E0010)", __imp__sub_820E0010,
       ITC_GROUP_STEP(PartsInitMufflers), 112, 1456, false, false},
      {"PartsPoints::PartsPoints (sub_820E2A10)", __imp__sub_820E2A10,
       hooks::SecondSetPartsPointsConstruct, 128, 37096, false, false},
      {"PartsPoints::Reset (sub_820E00F0)", __imp__sub_820E00F0,
       hooks::SecondSetPartsPointsReset, 112, 37096, false, false},
      {"CockpitPoints::CockpitPoints (sub_820DC190)", __imp__sub_820DC190,
       hooks::SecondSetCockpitPointsConstruct, 128, 4084, false, false},
      {"CockpitPoints::Reset (sub_820DBC10)", __imp__sub_820DBC10,
       hooks::SecondSetCockpitPointsReset, 112, 4084, false, false},
      {"InteriorPoints::InteriorPoints (sub_820E4FD8)", __imp__sub_820E4FD8,
       hooks::SecondSetInteriorPointsConstruct, 128, 11460, false, false},
      {"InteriorPoints::Reset (sub_820E35D8)", __imp__sub_820E35D8,
       hooks::SecondSetInteriorPointsReset, 96, 11460, false, false},
      {"LightPoints::LightPoints (sub_820DDCF8)", __imp__sub_820DDCF8,
       hooks::SecondSetLightPointsConstruct, 128, 13072, false, false},
      {"LightPoints::Reset (sub_820DC8A8)", __imp__sub_820DC8A8,
       hooks::SecondSetLightPointsReset, 96, 13072, false, false},
      {"TrafficPoints::TrafficPoints (sub_820E68E8)", __imp__sub_820E68E8,
       hooks::SecondSetTrafficPointsConstruct, 128, 6080, false, false},
      {"TrafficPoints::Reset (sub_820E5880)", __imp__sub_820E5880,
       hooks::SecondSetTrafficPointsReset, 96, 6080, false, false},
  };
  for (const Group& g : groups) {
    Results results(g.name);
    std::vector<Window> windows;
    if (g.frames) {
      windows.push_back(ClearWindow(g.frame));
    }
    if (g.init) {
      windows.push_back(ClearWindow(g.frame + ss::kConstructFrameBytes));
    }
    if (g.frame == 192 && g.frames) {
      windows.push_back({192 - 80, 64});  // ResetHeadlights' identity
    }
    if (g.frame == 144 && g.bytes == 1744) {
      windows.push_back({144 - 80, 16});  // ResetRimA's row
    }
    for (int c = 0; c < 4; c++) {
      FrameCase(
          h, results, block, std::max<uint32_t>(0x2000, (g.bytes + 0x8F) & ~0xFu), r,
          [&](PPCContext& t) { t.r3.u64 = block + 16 * r.Below(8); }, g.recompiled, g.native,
          windows, fmt::format("case {} ({} bytes)", c, g.bytes));
    }
    results.Report();
  }
#undef ITC_GROUP_STEP
}

// --- The objects ---

// The objects' resets and the scene model's block: the object at the block,
// its words random. The block, the frames' words the function writes itself
// and r3 compared (the nested Clear frames aren't: as the blocks' resets').
void Objects(Context& h, Random& r, uint32_t block) {
  constexpr uint32_t kNone = 0xFFFFFFFFu;
  struct Object {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t bytes;
    std::vector<Window> windows;
    uint32_t zero = kNone;  // a word set to 0 (a release's first-set object)
    bool list = false;      // r4 a list (the constructors'), past the object
    bool node = false;      // the object on a list past it (the destructors')
    bool flags = false;     // r4 the deleting destructors' flags (bit 0 clear)
  };
  const Object objects[] = {
      {"SceneParts::Reset (sub_820E0510)", __imp__sub_820E0510, hooks::SecondSetScenePartsReset,
       37296, {}},
      {"SceneCockpit::Reset (sub_820DBE20)", __imp__sub_820DBE20,
       hooks::SecondSetSceneCockpitReset, 4320, {}},
      {"SceneInterior::Reset (sub_820E3748)", __imp__sub_820E3748,
       hooks::SecondSetSceneInteriorReset, 11680, {}},
      {"SceneLights::Reset (sub_820DCA40)", __imp__sub_820DCA40,
       hooks::SecondSetSceneLightsReset, 13168, {}},
      {"SceneTraffic::Reset (sub_820E5930)", __imp__sub_820E5930,
       hooks::SecondSetSceneTrafficReset, 9808, {}},
      {"SceneModelBlock::SceneModelBlock (sub_820DA108)", __imp__sub_820DA108,
       hooks::SecondSetSceneModelBlockConstruct, 736, {}},
      {"SceneModelBlock::Reset (sub_820D9F50)", __imp__sub_820D9F50,
       hooks::SecondSetSceneModelBlockReset, 736,
       {{ss::kSceneModelBlockResetFrameBytes - 80, 128}}},
      // Without its first-set car model (+24 = 0): CarRecord::Reset and the
      // words (with one, below).
      {"SceneCarModel::Release (sub_820DB6C0, no car model)", __imp__sub_820DB6C0,
       hooks::SecondSetSceneCarModelRelease, 3744, {}, 24},
      {"SceneWheels::Reset (sub_820E88A8)", __imp__sub_820E88A8, hooks::SecondSetSceneWheelsReset,
       16800, {}},
      {"WheelPoints::Reset (sub_820E85C8)", __imp__sub_820E85C8, hooks::SecondSetWheelPointsReset,
       16608, {}},
      {"WheelPoints::WheelPoints (sub_820ECC48)", __imp__sub_820ECC48,
       hooks::SecondSetWheelPointsConstruct, 16608, {}},
      {"WheelPoints::InitTire (sub_820E8A80)", __imp__sub_820E8A80,
       hooks::SecondSetWheelsInitTire, 1840, {}},
      {"WheelPoints::InitRimA (sub_820E99C0)", __imp__sub_820E99C0,
       hooks::SecondSetWheelsInitRimA, 1744, {}},
      {"WheelPoints::InitRimB (sub_820EA8E8)", __imp__sub_820EA8E8,
       hooks::SecondSetWheelsInitRimB, 1744, {}},
      {"WheelPoints::InitBrakes (sub_820EB320)", __imp__sub_820EB320,
       hooks::SecondSetWheelsInitBrakes, 2944, {}},
      // The releases without their first-set object (ScenePools::ReleaseA's
      // path is SceneCarModel::Release's, below, with ReleaseA's own cases).
      {"SceneParts::Release (sub_820E0590, no first-set object)", __imp__sub_820E0590,
       hooks::SecondSetScenePartsRelease, 37296, {}, 104},
      {"SceneInterior::Release (sub_820E3890, no first-set object)", __imp__sub_820E3890,
       hooks::SecondSetSceneInteriorRelease, 11680, {}, 36},
      {"SceneLights::Release (sub_820DCB20, no first-set object)", __imp__sub_820DCB20,
       hooks::SecondSetSceneLightsRelease, 13168, {}, 28},
      {"SceneWheels::Release (sub_820E89B0, no first-set object)", __imp__sub_820E89B0,
       hooks::SecondSetSceneWheelsRelease, 16800, {}, 28},
      {"SceneTraffic::Release (sub_820E5A18, no first-set object)", __imp__sub_820E5A18,
       hooks::SecondSetSceneTrafficRelease, 9808, {}, 9760},
      {"SceneCarModel::SceneCarModel (sub_820DB770)", __imp__sub_820DB770,
       hooks::SecondSetSceneCarModelConstruct, 3744, {}, kNone, true},
      {"SceneParts::SceneParts (sub_820E2F18)", __imp__sub_820E2F18,
       hooks::SecondSetScenePartsConstruct, 37296, {}, kNone, true},
      {"SceneCockpit::SceneCockpit (sub_820DC3E8)", __imp__sub_820DC3E8,
       hooks::SecondSetSceneCockpitConstruct, 4320, {}, kNone, true},
      {"SceneInterior::SceneInterior (sub_820E5340)", __imp__sub_820E5340,
       hooks::SecondSetSceneInteriorConstruct, 11680, {}, kNone, true},
      {"SceneLights::SceneLights (sub_820DE030)", __imp__sub_820DE030,
       hooks::SecondSetSceneLightsConstruct, 13168, {}, kNone, true},
      {"SceneWheels::SceneWheels (sub_820ECE40)", __imp__sub_820ECE40,
       hooks::SecondSetSceneWheelsConstruct, 16800, {}, kNone, true},
      {"SceneTraffic::SceneTraffic (sub_820E6E70)", __imp__sub_820E6E70,
       hooks::SecondSetSceneTrafficConstruct, 9808, {}, kNone, true},
      {"SceneModel::SceneModel (sub_820DB330)", __imp__sub_820DB330,
       hooks::SecondSetSceneModelConstruct, 5120, {}, kNone, true},
      {"PartsPoints::~PartsPoints (sub_820E2C10)", __imp__sub_820E2C10,
       hooks::SecondSetPartsPointsDestruct, 37120, {}},
      {"CockpitPoints::~CockpitPoints (sub_820DC258)", __imp__sub_820DC258,
       hooks::SecondSetCockpitPointsDestruct, 4128, {}},
      {"InteriorPoints::~InteriorPoints (sub_820E50C0)", __imp__sub_820E50C0,
       hooks::SecondSetInteriorPointsDestruct, 11520, {}},
      {"LightPoints::~LightPoints (sub_820DDD90)", __imp__sub_820DDD90,
       hooks::SecondSetLightPointsDestruct, 13072, {}},
      {"WheelPoints::~WheelPoints (sub_820ECCB8)", __imp__sub_820ECCB8,
       hooks::SecondSetWheelPointsDestruct, 16608, {}},
      {"TrafficPoints::~TrafficPoints (sub_820E6980)", __imp__sub_820E6980,
       hooks::SecondSetTrafficPointsDestruct, 6080, {}},
      // The destructors without their first-set objects (the releases' cases
      // above).
      {"SceneCarModel::~SceneCarModel (sub_820DB720, no car model)", __imp__sub_820DB720,
       hooks::SecondSetSceneCarModelDestruct, 3744, {}, 24, false, true},
      {"SceneParts::~SceneParts (sub_820E2E10, no first-set object)", __imp__sub_820E2E10,
       hooks::SecondSetScenePartsDestruct, 37296, {}, 104, false, true},
      {"SceneInterior::~SceneInterior (sub_820E5178, no first-set object)", __imp__sub_820E5178,
       hooks::SecondSetSceneInteriorDestruct, 11680, {}, 36, false, true},
      {"SceneLights::~SceneLights (sub_820DDE00, no first-set object)", __imp__sub_820DDE00,
       hooks::SecondSetSceneLightsDestruct, 13168, {}, 28, false, true},
      {"SceneWheels::~SceneWheels (sub_820ECDD0, no first-set object)", __imp__sub_820ECDD0,
       hooks::SecondSetSceneWheelsDestruct, 16800, {}, 28, false, true},
      {"SceneTraffic::~SceneTraffic (sub_820E69F0)", __imp__sub_820E69F0,
       hooks::SecondSetSceneTrafficDestruct, 9808, {}, kNone, false, true},
      {"SceneCarModel's deleting destructor (sub_820DB7C8, no car model)", __imp__sub_820DB7C8,
       hooks::SecondSetSceneCarModelDeletingDestruct, 3744, {}, 24, false, true, true},
      {"SceneParts' deleting destructor (sub_820E2F78, no first-set object)", __imp__sub_820E2F78,
       hooks::SecondSetScenePartsDeletingDestruct, 37296, {}, 104, false, true, true},
      {"SceneInterior's deleting destructor (sub_820E53A0, no first-set object)",
       __imp__sub_820E53A0, hooks::SecondSetSceneInteriorDeletingDestruct, 11680, {}, 36, false,
       true, true},
      {"SceneLights' deleting destructor (sub_820DE090, no first-set object)", __imp__sub_820DE090,
       hooks::SecondSetSceneLightsDeletingDestruct, 13168, {}, 28, false, true, true},
      {"SceneWheels' deleting destructor (sub_820ECEB8, no first-set object)", __imp__sub_820ECEB8,
       hooks::SecondSetSceneWheelsDeletingDestruct, 16800, {}, 28, false, true, true},
      {"SceneTraffic's deleting destructor (sub_820E6ED8)", __imp__sub_820E6ED8,
       hooks::SecondSetSceneTrafficDeletingDestruct, 9808, {}, kNone, false, true, true},
      {"PoolObject::DeletingDestructor (sub_820DB400)", __imp__sub_820DB400,
       hooks::SecondSetPoolObjectDeletingDestruct, 5120, {}, kNone, false, true, true},
  };
  for (const Object& o : objects) {
    Results results(o.name);
    // The constructors' list past the object: empty, or one node (at +0x40)
    // the new one is appended after.
    const uint32_t list = block + ((o.bytes + 0x17F) & ~0xFu);
    for (int c = 0; c < 4; c++) {
      FrameCase(
          h, results, block, (o.bytes + 0x28F) & ~0xFu, r,
          [&](PPCContext& t) {
            t.r3.u64 = block + 16 * r.Below(8);
            if (o.zero != kNone) {
              Put(h.base, t.r3.u32 + o.zero, 0);
            }
            if (o.list) {
              const bool one = r.Below(2) == 0;
              Put(h.base, list + 0, one ? list + 0x40 : 0);
              Put(h.base, list + 4, one ? list + 0x40 : 0);
              Put(h.base, list + 8, one ? 1 : 0);
              Put(h.base, list + 0x40 + 8, 0);
              Put(h.base, list + 0x40 + 12, 0);
              t.r4.u64 = list;
            }
            if (o.node) {
              // Off any list, alone on one, last after a node, or between two.
              const uint32_t object = t.r3.u32, before = list + 0x40, after = list + 0x80;
              const uint32_t kind = r.Below(4);
              Put(h.base, object + 4, kind == 0 ? 0 : list);
              Put(h.base, object + 8, kind == 3 ? after : 0);
              Put(h.base, object + 12, kind >= 2 ? before : 0);
              Put(h.base, list + 0, kind >= 2 ? before : object);
              Put(h.base, list + 4, kind == 3 ? after : object);
              Put(h.base, list + 8, kind == 1 ? 1 : kind);
              Put(h.base, before + 8, object);
              Put(h.base, before + 12, 0);
              Put(h.base, after + 8, 0);
              Put(h.base, after + 12, object);
            }
            if (o.flags) {
              t.r4.u64 = r.Next() & ~1ull;
            }
          },
          o.recompiled, o.native, o.windows, fmt::format("case {} ({} bytes)", c, o.bytes));
    }
    results.Report();
  }
}

// --- ScenePools::ReleaseB and the releases it reaches ---
//
// A second set at the block (its lists at +4944 + 12 x pool, pools 0 to 9,
// each one object long; 8 and 9 hold one of the others) and one object of
// each pool past it, their words random but the list's (+8 = 0: the last
// node; +20 an index, 0 or past the end, both the first node), +16 (3, or
// random) and the first-set words (0: no ScenePools::ReleaseA). The
// cockpit's interior (+28) is the pool 3 object or 0 (+4316 = the set); the
// scene model's (+5044 0 or random, +32 kind 3 or random, +5116 the set)
// parts the pools' objects or 0, so ReleaseB nests through it and the
// cockpit, and an object released once has +16 = 0 the second time. The
// block and r3 compared (the frames aren't: the plain C++ doesn't write the
// saved registers' slots).
constexpr uint32_t kReleasePools = 8;
constexpr uint32_t kReleaseObjectBytes[kReleasePools] = {3744,  37296, 4320, 11680,
                                                         13168, 16800, 9808, 5120};
// The first-set object's word in each (none in the scene model).
constexpr uint32_t kReleaseFirstWords[kReleasePools - 1] = {24, 104, 32, 36, 28, 28, 9760};

struct ReleaseWorld {
  uint32_t set = 0;
  uint32_t objects[kReleasePools] = {};
};

ReleaseWorld MakeReleaseWorld(uint8_t* base, uint32_t block, Random& r) {
  ReleaseWorld w;
  w.set = block;
  uint32_t at = block + 0x1400;
  for (uint32_t p = 0; p < kReleasePools; p++) {
    w.objects[p] = at;
    at += (kReleaseObjectBytes[p] + 15) & ~15u;
  }
  for (uint32_t p = 0; p < 10; p++) {
    Put(base, w.set + 4944 + 12 * p, p < kReleasePools ? w.objects[p] : w.objects[r.Below(8)]);
  }
  for (uint32_t p = 0; p < kReleasePools; p++) {
    const uint32_t object = w.objects[p];
    Put(base, object + 8, 0);
    Put(base, object + 20, r.Below(3));
    Put(base, object + 16, r.Below(4) == 0 ? uint32_t(r.Next()) : 3);
    if (p < kReleasePools - 1) {
      Put(base, object + kReleaseFirstWords[p], 0);
    }
  }
  const uint32_t cockpit = w.objects[2];
  Put(base, cockpit + 28, r.Below(3) == 0 ? 0 : w.objects[3]);
  Put(base, cockpit + 4316, w.set);
  const uint32_t model = w.objects[7];
  Put(base, model + 5044, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
  Put(base, model + 32, r.Below(4) == 0 ? 3 : uint32_t(r.Next()));
  Put(base, model + 5116, w.set);
  static constexpr uint32_t kParts[][2] = {{5048, 0}, {5060, 1}, {5068, 3}, {5084, 4},
                                           {5100, 5}, {5076, 2}, {5092, 6}};
  for (const auto& part : kParts) {
    Put(base, model + part[0], r.Below(4) == 0 ? 0 : w.objects[part[1]]);
  }
  return w;
}

void ReleaseBs(Context& h, Random& r, uint32_t block) {
  struct Group {
    const char* name;
    GuestFunction recompiled, native;
    int target;  // the object's pool for a release's own cases, -1 ReleaseB's
    bool car_models_off;
    bool node = false;   // a destructor's: the object off any list or alone on one
    bool flags = false;  // r4 the deleting destructor's flags (bit 0 clear)
  };
  const Group groups[] = {
      {"ScenePools::ReleaseB (sub_820C5C88, with SceneModel::Release sub_820DA5D0 and "
       "SceneCockpit::Release sub_820DBF88, no first-set objects)",
       __imp__sub_820C5C88, hooks::SecondSetReleaseB, -1, false},
      // Pool 0's release not native (--native_car_models off on both sides):
      // the original ReleaseB for it, at the top and nested.
      {"ScenePools::ReleaseB (sub_820C5C88, pool 0 left to the original)", __imp__sub_820C5C88,
       hooks::SecondSetReleaseB, -1, true},
      {"SceneCockpit::Release (sub_820DBF88, no cockpit body)", __imp__sub_820DBF88,
       hooks::SecondSetSceneCockpitRelease, 2, false},
      {"SceneModel::Release (sub_820DA5D0, no first-set objects)", __imp__sub_820DA5D0,
       hooks::SecondSetSceneModelRelease, 7, false},
      {"SceneCockpit::~SceneCockpit (sub_820DC2F8, no cockpit body)", __imp__sub_820DC2F8,
       hooks::SecondSetSceneCockpitDestruct, 2, false, true},
      {"SceneCockpit's deleting destructor (sub_820DC448, no cockpit body)", __imp__sub_820DC448,
       hooks::SecondSetSceneCockpitDeletingDestruct, 2, false, true, true},
  };
  for (const Group& g : groups) {
    Results results(g.name);
    for (int c = 0; c < 24; c++) {
      // Pools 7 and 2 (the nesting ones) more often; 8 and 9 past the
      // releases.
      const uint32_t pick = r.Below(14);
      const uint32_t pool = pick < 4 ? 7 : (pick < 6 ? 2 : (pick < 8 ? 0 : pick - 6));
      ITC_FLAG(native_car_models) = !g.car_models_off;
      Case(
          h, results, block, kBlockBytes, r,
          [&](PPCContext& t) {
            const ReleaseWorld w = MakeReleaseWorld(h.base, block, r);
            if (g.target >= 0) {
              const uint32_t object = w.objects[g.target];
              t.r3.u64 = object;
              if (g.node) {
                // Its list (past the set's lists): off it, or alone on it.
                const uint32_t list = w.set + 5072;
                const bool on = r.Below(2) == 0;
                Put(h.base, object + 4, on ? list : 0);
                Put(h.base, object + 12, 0);
                Put(h.base, list + 0, object);
                Put(h.base, list + 4, object);
                Put(h.base, list + 8, 1);
              }
              if (g.flags) {
                t.r4.u64 = r.Next() & ~1ull;
              }
              return;
            }
            t.r3.u64 = w.set;
            t.r4.u64 = pool;
            t.r5.u64 = r.Below(4);
          },
          g.recompiled, g.native,
          g.target >= 0 ? fmt::format("case {}", c) : fmt::format("case {} (pool {})", c, pool));
      ITC_FLAG(native_car_models) = true;
    }
    results.Report();
  }
}

// SceneCarModel::Release with its first-set car model (+24): the car model's
// inputs as car_models' CarModel::Release cases build them
// (car_model_inputs.h), the first pool set ([0x826C2E40]) a block whose pool
// 0 list (+56) holds it (List::At by its +20, 0 or past the end). The
// recompiled side with --native_second_set, --native_scene_loaders and
// --native_car_models off. The block, the pending frees' added nodes, the
// allocations' states and r3 compared.
void CarModelReleases(Context& h, Random& r, uint32_t block) {
  namespace cmi = car_model_inputs;
  namespace sl = scene_loaders;
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: second_set: --native_models was off at start-up: "
                 "SceneCarModel::Release's cases with a car model skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  constexpr uint32_t kBytes = 0x6400;
  const uint32_t scratch = block + 0x100;
  const uint32_t car = block + 0x1000;
  const uint32_t loads = car + sl::kCarModelLoads;
  const uint32_t set = block + 0x3000;
  const uint32_t records = set + 0x100;
  const uint32_t first_set = block + 0x3800;
  const uint32_t object = block + 0x4000;
  // The second set (ReleaseB's cases): its pool 0 list (+4944) holds the
  // object.
  const uint32_t second_set = block + 0x5000;
  const uint32_t saved_set = guest::LoadU32(h.base, scene::kFirstSet);
  for (int via = 0; via < 2; via++) {
    Results results(via == 0 ? "SceneCarModel::Release (sub_820DB6C0, with a car model)"
                             : "ScenePools::ReleaseB (sub_820C5C88, pool 0 with a car model)");
    for (int c = 0; c < 24; c++) {
      const uint64_t seed = r.Next();
      const cmi::Plan plan = cmi::MakePlan(r);
      const uint32_t a = r.Below(3), b = r.Below(3);
      const uint32_t index = r.Below(2) == 0 ? 0 : 1 + r.Below(4);
      PPCContext start = CallContext(h);
      start.r1.u64 &= ~0xFull;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random bytes(seed);
        RandomBytes(h.base, block, kBytes, bytes);
        cmi::Held held;
        cmi::Write(h.base, loads, scratch, plan, true, held);
        Put(h.base, car + sl::kCarModelSet, set);
        Put(h.base, set + 40, a);
        Put(h.base, set + 12, b);
        Put(h.base, set + 52, records);
        for (uint32_t i = 0; i < 4 * (a + b); i++) {
          Put(h.base, records + 20 * i + 16, bytes.Below(4) == 0 ? 0 : 2);
        }
        Put(h.base, car + 8, 0);  // the list's last node
        Put(h.base, car + 20, index);
        Put(h.base, first_set + 56, car);
        Put(h.base, object + 24, car);
        Put(h.base, scene::kFirstSet, first_set);
        PPCContext t = start;
        t.r3.u64 = object;
        if (via == 1) {
          Put(h.base, second_set + 4944, object);
          Put(h.base, object + 8, 0);
          Put(h.base, object + 20, bytes.Below(3));
          Put(h.base, object + 16, bytes.Below(4) == 0 ? uint32_t(bytes.Next()) : 3);
          t.r3.u64 = second_set;
          t.r4.u64 = 0;
          t.r5.u64 = bytes.Below(4);
        }
        const pending_frees::ListState was = pending_frees::SaveList(h.base);
        ITC_FLAG(native_second_set) = v == 1;
        ITC_FLAG(native_scene_loaders) = v == 1;
        ITC_FLAG(native_car_models) = v == 1;
        if (via == 0) {
          (v == 0 ? __imp__sub_820DB6C0 : hooks::SecondSetSceneCarModelRelease)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_820C5C88 : hooks::SecondSetReleaseB)(t, h.base);
        }
        ITC_FLAG(native_second_set) = true;
        ITC_FLAG(native_scene_loaders) = true;
        ITC_FLAG(native_car_models) = true;
        Put(h.base, scene::kFirstSet, saved_set);
        pending_frees::TakeAddedNodes(h.base, was, out[v]);
        cmi::Take(h.base, loads, held, out[v]);
        out[v].Bytes(Read(h.base, block, kBytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {} (index {})", c, index));
    }
    results.Report();
  }
  Put(h.base, scene::kFirstSet, saved_set);
}

// --- The morph and the tire's helpers ---
//
// Float words from a mix: small values, zeros of both signs, denormals
// (the vector steps flush them), infinities, NaNs and random bits.
uint32_t MixedFloat(Random& r) {
  switch (r.Below(10)) {
    case 0: return r.Below(2) == 0 ? 0u : 0x80000000u;
    case 1: return (r.Below(2) << 31) | (1 + r.Below(0x7FFFFF));  // denormal
    case 2: return r.Below(2) == 0 ? 0x7F800000u : 0xFF800000u;
    case 3: return 0x7FC00000u | r.Below(0x400000) | (r.Below(2) << 31);  // NaN
    case 4: return 0x7F800001u + r.Below(0x3FFFFF);  // signaling NaN
    case 5: return uint32_t(r.Next());
    default:
      return std::bit_cast<uint32_t>(float(int32_t(r.Below(2001)) - 1000) / 64.0f);
  }
}

// One morph case through both versions: `setup` writes the block (its bytes
// random first) and the registers; the block's words compared exactly but
// `floats` (address, count) ranges as floats, the frame `windows` (their
// float words given as offsets from r1 too), r3 (both halves) and the flush
// mode left. Both sides start with the flush mode `flush_on`.
struct FloatRange {
  uint32_t address, count;
};
void MorphCase(Context& h, Results& results, uint32_t block, uint32_t bytes, Random& r,
               bool flush_on, const std::function<void(PPCContext&, std::vector<FloatRange>&)>& setup,
               GuestFunction recompiled, GuestFunction native, const std::vector<Window>& windows,
               const std::vector<uint32_t>& float_frame_words, const std::string& what) {
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  const uint32_t stack = start.r1.u32 - kStackBytes;
  RandomBytes(h.base, block, bytes, r);
  RandomBytes(h.base, stack, kStackBytes, r);
  std::vector<FloatRange> floats;
  setup(start, floats);
  const std::vector<uint8_t> inputs = Read(h.base, block, bytes);
  const std::vector<uint8_t> frames = Read(h.base, stack, kStackBytes);
  std::vector<bool> is_float(bytes / 4, false);
  for (const FloatRange& f : floats) {
    for (uint32_t w = 0; w < f.count; w++) {
      const uint32_t at = f.address + 4 * w - block;
      if (at / 4 < is_float.size()) is_float[at / 4] = true;
    }
  }
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), bytes);
    Write(h.base, stack, frames.data(), kStackBytes);
    PPCContext t = start;
    if (flush_on) {
      t.fpscr.enableFlushModeUnconditional();
    } else {
      t.fpscr.disableFlushModeUnconditional();
    }
    RunSide(v, recompiled, native, t, h.base);
    const bool left_on =
        (t.fpscr.getcsr() & uint32_t(decltype(t.fpscr)::FlushMask)) != 0;
    t.fpscr.disableFlushModeUnconditional();
    uint32_t w = 0;
    while (w < is_float.size()) {
      uint32_t end = w;
      while (end < is_float.size() && is_float[end] == is_float[w]) end++;
      if (is_float[w]) {
        out[v].Floats(h.base, block + 4 * w, end - w);
      } else {
        out[v].Bytes(Read(h.base, block + 4 * w, 4 * (end - w)));
      }
      w = end;
    }
    for (const Window& win : windows) {
      for (uint32_t at = 0; at < win.bytes; at += 4) {
        const uint32_t address = start.r1.u32 - win.below + at;
        const uint32_t offset = uint32_t(-int32_t(win.below) + int32_t(at));
        bool is_f = false;
        for (const uint32_t f : float_frame_words) is_f |= f == offset;
        if (is_f) {
          out[v].Float(guest::LoadU32(h.base, address));
        } else {
          out[v].Word(guest::LoadU32(h.base, address));
        }
      }
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    out[v].Word(left_on);
  }
  results.Check(out[0], out[1], what);
}

void Morphs(Context& h, Random& r, uint32_t block) {
  if (!native::Buffers_NativeMemcpy()) {
    ITC_LOG_WARN("harness: second_set: the morph's cases skipped (memcpy not native)");
    return;
  }
  // The model's shapes: [model + 40] = 8 shapes of 544 bytes at +0x100;
  // the streams' headers at +0x1300 (64 bytes each); the streams at +0x2000
  // (0x100 bytes each).
  const uint32_t model = block, shapes = block + 0x100, headers = block + 0x1300;
  const uint32_t streams = block + 0x2000;
  constexpr uint32_t kBytes = 0x4000;
  struct Accessor {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Accessor accessors[] = {
      {"Model::StreamData (sub_82594A60)", __imp__sub_82594A60, hooks::SecondSetModelStreamData},
      {"Model::StreamSize (sub_82594A78)", __imp__sub_82594A78, hooks::SecondSetModelStreamSize},
      {"Model::StreamData2 (sub_82594A90)", __imp__sub_82594A90, hooks::SecondSetModelStreamData2},
      {"Model::StreamSize2 (sub_82594AA8)", __imp__sub_82594AA8, hooks::SecondSetModelStreamSize2},
  };
  for (const Accessor& a : accessors) {
    Results results(a.name);
    for (int c = 0; c < 8; c++) {
      MorphCase(
          h, results, block, kBytes, r, false,
          [&](PPCContext& t, std::vector<FloatRange>&) {
            Put(h.base, model + 40, shapes);
            t.r3.u64 = model;
            t.r4.u64 = r.Below(8);
          },
          a.recompiled, a.native, {}, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  // Morph::SetUp: the record at +0x3000, the indices at +0x3100 and weights
  // at +0x3140; every shape's first header +0 the vertex count, its second's
  // +4 the kind (the same for all: DebugPrintf never runs, as in play).
  const uint32_t record = block + 0x3000, indices = block + 0x3100, weights = block + 0x3140;
  {
    Results results("Morph::SetUp (sub_82594C18)");
    for (int c = 0; c < 24; c++) {
      const bool flush_on = r.Below(2) == 0;
      MorphCase(
          h, results, block, kBytes, r, flush_on,
          [&](PPCContext& t, std::vector<FloatRange>& floats) {
            Put(h.base, model + 40, shapes);
            const uint32_t vertices = r.Below(9), kind = 6 + r.Below(3);
            for (uint32_t s = 0; s < 8; s++) {
              const uint32_t first = headers + 128 * s, second = first + 64;
              Put(h.base, shapes + 544 * s + 400, first);
              Put(h.base, shapes + 544 * s + 404, second);
              Put(h.base, first + 0, vertices);
              Put(h.base, second + 4, kind);
            }
            for (uint32_t i = 0; i < 8; i++) {
              Put(h.base, indices + 4 * i, r.Below(8));
              Put(h.base, weights + 4 * i, MixedFloat(r));
            }
            floats.push_back({record + 80, 8});
            t.r3.u64 = model;
            t.r4.u64 = record;
            t.r5.u64 = r.Below(8);
            t.r6.u64 = indices;
            t.r7.u64 = weights;
            t.r8.u64 = r.Below(4) == 0 ? 8 + r.Below(4) : r.Below(8);
          },
          __imp__sub_82594C18, hooks::SecondSetMorphSetUp, {}, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  // Morph::Blend and Run: the record's streams in the stream area (base
  // streams, up to 7 targets' first streams and +48's second streams, the
  // two outputs), each 0x100 bytes (up to 16 vertices of 16), their words
  // mixed floats; the kind 6, 7 or another (not 8: the original's, left to
  // it). The outputs (and every stream) compared as floats, Blend6's and
  // Blend7's frame words (their weights as floats).
  for (int run = 0; run < 2; run++) {
    Results results(run == 0 ? "Morph::Blend (sub_8258BD28, with Blend6 sub_8258B578 and "
                               "Blend7 sub_8258B678)"
                             : "Morph::Run (sub_82594D10)");
    for (int c = 0; c < 48; c++) {
      const uint32_t kind = c % 8 == 7 ? 9 : 6 + uint32_t(c % 2);
      const bool flush_on = r.Below(2) == 0;
      MorphCase(
          h, results, block, kBytes, r, flush_on,
          [&](PPCContext& t, std::vector<FloatRange>& floats) {
            const uint32_t vertices = r.Below(5) == 0 ? 0 : 1 + r.Below(16);
            const uint32_t targets = r.Below(8);
            for (uint32_t w = 0; w < 0x2000 / 4; w++) {
              Put(h.base, streams + 4 * w, MixedFloat(r));
            }
            floats.push_back({streams, 0x2000 / 4});
            auto stream = [&](uint32_t n) { return streams + 0x100 * n; };
            Put(h.base, record + 0, stream(0));
            Put(h.base, record + 4, stream(1));
            Put(h.base, record + 8, stream(2));
            Put(h.base, record + 12, stream(3));
            for (uint32_t i = 0; i < 7; i++) {
              Put(h.base, record + 16 + 4 * i, stream(4 + i));
              Put(h.base, record + 48 + 4 * i, r.Below(2) == 0 ? stream(1) : stream(11 + i % 5));
              Put(h.base, record + 80 + 4 * i, MixedFloat(r));
            }
            floats.push_back({record + 80, 7});
            Put(h.base, record + 112, targets);
            Put(h.base, record + 116, vertices);
            Put(h.base, record + 120, kind);
            if (run == 0) {
              t.r3.u64 = record;
            } else {
              t.r3.u64 = r.Next();
              t.r4.u64 = record;
              t.r5.u64 = stream(2);
              t.r6.u64 = stream(3);
            }
          },
          run == 0 ? __imp__sub_8258BD28 : __imp__sub_82594D10,
          run == 0 ? hooks::SecondSetMorphBlend : hooks::SecondSetMorphRun,
          // Blend7's +80 to +112 (r1 - 80: the sizes words, then the
          // weights), Blend6's +80 to +96 (r1 - 48: the weights); the saved
          // registers' slots above them left out.
          kind == 7 ? std::vector<Window>{{160 - 80, 32}}
                    : (kind == 6 ? std::vector<Window>{{128 - 80, 16}} : std::vector<Window>{}),
          {uint32_t(-64), uint32_t(-60), uint32_t(-56), uint32_t(-52), uint32_t(-48),
           uint32_t(-44), uint32_t(-40), uint32_t(-36)},
          fmt::format("case {} (kind {})", c, kind));
    }
    results.Report();
  }
  // sub_820E6F28: the 12 entries at +0x1000 (an index of 0 to 26: entry
  // index - 12, before them for the small ones), the value at +0x0F00, f1 a
  // mixed float; sub_820E7010: an index of 0 to 26 into the entries (their
  // words mixed floats), the outputs at +0x1200. Their floats compared as
  // floats, the red-zone word at r1 - 16 exactly.
  const uint32_t entries = block + 0x1000;
  {
    Results results("sub_820E6F28 (the tire's entries)");
    for (int c = 0; c < 32; c++) {
      MorphCase(
          h, results, block, kBytes, r, r.Below(2) == 0,
          [&](PPCContext& t, std::vector<FloatRange>& floats) {
            Put(h.base, block + 0x0F00, MixedFloat(r));
            // Entry index - 12 lies from 12 before the entries to 2 past.
            floats.push_back({entries - 12 * 24, 27 * 24 / 4});
            t.r3.u64 = entries;
            t.r4.u64 = r.Below(4) == 0 ? r.Below(27) : 12 + r.Below(12);
            t.r5.u64 = block + 0x0F00;
            t.f1.f64 = double(std::bit_cast<float>(MixedFloat(r)));
          },
          __imp__sub_820E6F28, hooks::SecondSetTireFillEntries, {{16, 8}}, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("sub_820E7010 (the tire's entry read)");
    for (int c = 0; c < 32; c++) {
      MorphCase(
          h, results, block, kBytes, r, r.Below(2) == 0,
          [&](PPCContext& t, std::vector<FloatRange>& floats) {
            for (uint32_t w = 0; w < 24 * 24 / 4; w++) {
              Put(h.base, entries - 12 * 24 + 4 * w, MixedFloat(r));
            }
            floats.push_back({entries - 12 * 24, 24 * 24 / 4});
            floats.push_back({block + 0x1200, 8});
            t.r3.u64 = r.Below(4) == 0 ? r.Below(27) : 12 + r.Below(12);
            t.r4.u64 = entries;
            t.r5.u64 = block + 0x1200;
            t.r6.u64 = block + 0x1210;
            t.r7.u64 = block + 0x1214;
          },
          __imp__sub_820E7010, hooks::SecondSetTireReadEntry, {{16, 8}}, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The point set-ups ---
//
// A world of two part models, each with up to 8 nodes (192 bytes, +40 a
// shape index or -1) and 8 shapes (544 bytes, named at +0, +396 one of two
// material blocks of up to 3 materials), its skeleton (+112) bound to a node
// file of 1 to 8 nodes; the point block's records named from the shapes'
// and nodes' names (or variants, or others). Each side builds it from the
// same seed and runs from the same random stack, the recompiled one with
// --native_second_set, --native_scene_loaders and --native_material_passes
// off (ModelPoint::Find, NameMatches and ScaleBlended the originals). The
// block (the records' matrices as floats), the models (the skeleton's array
// words left out), the skeletons' arrays, the materials (their first four
// words as floats) and r3 compared; the frames aren't (the plain C++ doesn't
// write the saved registers' slots).

// A small float (eighths from -4 to 4). As scene_loaders' harness.
uint32_t SmallFloat(Random& r) {
  return std::bit_cast<uint32_t>(float(int32_t(r.Below(65)) - 32) / 8.0f);
}

// A node file (4 KB at `file`) of `names.size()` nodes, as scene_loaders'
// harness builds them (ModelPoint::Find's cases), with valid parents.
void MakeNodeFile(uint8_t* base, uint32_t file, const std::vector<std::string>& names,
                  Random& r) {
  const uint32_t nodes = uint32_t(names.size());
  const uint32_t bones = r.Below(nodes + 1);
  const uint32_t first_bone = r.Below(nodes - bones + 1);
  RandomBytes(base, file, 4096, r);
  Put(base, file + 4, nodes);
  Put(base, file + 8, bones);
  Put(base, file + 12, first_bone);
  const uint32_t kRecords = 256, kSources = 1536, kMatrices = 1920, kTail = 2432;
  Put(base, file + 16, kRecords);
  Put(base, file + 20, kSources);
  Put(base, file + 24, kSources + 128);
  Put(base, file + 28, kSources + 256);
  Put(base, file + 32, kTail + 64);
  Put(base, file + 36, kMatrices);
  Put(base, file + 40, kTail + 128);
  Put(base, file + 44, kTail);
  Put(base, file + 48, kTail + 192);
  for (uint32_t n = 0; n < nodes; n++) {
    const uint32_t record = file + kRecords + 160 * n;
    PutString(base, record, names[n]);
    Put(base, record + 92, n == 0 || r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(n));
    // The parent (Skeleton::UpdateNodeMatrices multiplies by its matrix): an
    // earlier node or none, as a file's are (scene_loaders' harness leaves it
    // random, which can reach unmapped guest memory).
    Put(base, record + 128, n == 0 || r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(n));
    static const uint32_t kTypes[] = {0, 1, 2, 3, 4, 5, 0x80, 0x81, 32};
    Put(base, record + 132, kTypes[r.Below(std::size(kTypes))]);
  }
  for (uint32_t at = kSources; at < kTail; at += 4) Put(base, file + at, SmallFloat(r));
}

// A skeleton's arrays (offset, bytes, header bytes before the address) as
// Skeleton::Bind makes them.
struct SkeletonArray {
  uint32_t offset, bytes, header;
};
std::vector<SkeletonArray> SkeletonArrays(uint8_t* base, uint32_t file) {
  const uint32_t nodes = guest::LoadU32(base, file + 4);
  const uint32_t bones = guest::LoadU32(base, file + 8);
  return {{128, 20, 0},
          {136, 16 + 144 * nodes, 16},
          {132, 16 + 64 * nodes, 16},
          {144, 16 + 64 * bones, 16},
          {176, 16 * nodes, 0}};
}

// The skeleton's arrays' contents into `out` (past their headers), each
// then freed, and their words in the skeleton cleared.
void TakeSkeleton(uint8_t* base, uint32_t skeleton, uint32_t file, Outputs& out) {
  for (const SkeletonArray& a : SkeletonArrays(base, file)) {
    const uint32_t at = guest::LoadU32(base, skeleton + a.offset);
    out.Word(at != 0);
    if (at == 0) continue;
    if (a.offset != 128) {
      out.Floats(base, at, (a.bytes - a.header) / 4);
    }
    memory::crt::OperatorDeleteArray(at - a.header);
    Put(base, skeleton + a.offset, 0);
  }
}

// The world's layout from the block.
constexpr uint32_t kPoints = 0;            // the point block (37,096 bytes)
constexpr uint32_t kModels = 0xA000;       // two part models, 0x400 apart
constexpr uint32_t kNodes = 0xA800;        // their nodes, 0x600 apart
constexpr uint32_t kShapes = 0xB400;       // their shapes, 0x1200 apart
constexpr uint32_t kMaterials = 0xD800;    // two material blocks, 0x800 apart
constexpr uint32_t kFiles = 0xE800;        // two node files, 0x1000 apart
constexpr uint32_t kModelWords = 0x10800;  // the 17 (or 6) models' words
constexpr uint32_t kCarLoads = 0x10900;    // [+37084]: +24 = the next
constexpr uint32_t kCarObject = 0x10A00;   // +6700, +6708 read
constexpr uint32_t kScene = 0x12600;       // [+37088]: +4 the model set
constexpr uint32_t kWorldBytes = 0x12800;

const char* const kShapeNames[] = {"fb00punch_shape", "fog00_shape",   "window01_shape",
                                   "base_shape",      "hl00cover_shape", "rb00num_shape",
                                   "seat00_L_shape",  "x_shape"};
const char* const kNodeNames[] = {"dl00_rot_00", "hl_00", "tl00_00_ef", "mf00_00_ef",
                                  "bn_00",       "hl00ef_00", "x"};

// The point groups: (offset in the block, records). Parts' and cockpit's.
struct Span {
  uint32_t at, records;
};
const Span kPartsSpans[] = {{0, 11},     {2656, 1},  {2896, 7},   {4592, 1},  {4832, 3},
                            {5552, 2},   {6032, 25}, {12128, 25}, {18224, 23}, {23760, 1},
                            {24000, 1},  {24240, 1}, {24480, 13}, {27680, 15}, {31280, 7},
                            {32960, 5},  {34160, 2}, {34896, 5},  {36096, 4}};
const Span kCockpitSpans[] = {{0, 3}, {720, 2}, {1200, 1}, {1440, 2}, {1920, 1}, {2160, 8}};
const Span kInteriorSpans[] = {{0, 1},     {256, 12},  {3168, 22},
                               {8528, 4},  {9504, 4},  {10480, 4}};
const Span kLightsSpans[] = {{64, 18}, {4400, 18}, {8736, 18}};
const Span kTrafficSpans[] = {{16, 6}, {1472, 7}, {3152, 12}};

std::string PickName(Random& r, const char* const* names, size_t count) {
  std::string name = names[r.Below(uint32_t(count))];
  switch (r.Below(6)) {
    case 0: return Variant(r, name);
    case 1: return RandomName(r, r.Below(12));
    default: return name;
  }
}

// The world from `seed`: random bytes, the two models, the point records'
// names (for `spans`), the models' words (`kinds` of them, 0 now and then),
// the car loads and the scene with model set `set`.
void BuildWorld(uint8_t* base, uint32_t block, uint64_t seed, const Span* spans, size_t span_count,
                uint32_t kinds, uint32_t set) {
  Random g(seed);
  RandomBytes(base, block, kWorldBytes, g);
  for (uint32_t m = 0; m < 2; m++) {
    const uint32_t model = block + kModels + 0x400 * m;
    const uint32_t nodes = block + kNodes + 0x600 * m;
    const uint32_t shapes = block + kShapes + 0x1200 * m;
    const uint32_t file = block + kFiles + 0x1000 * m;
    Put(base, model + 36, nodes);
    Put(base, model + 40, shapes);
    Put(base, model + 408, g.Below(8) == 0 ? 0 : 1 + g.Below(8));
    for (uint32_t n = 0; n < 8; n++) {
      Put(base, nodes + 192 * n + 40, g.Below(5) == 0 ? 0xFFFFFFFFu : g.Below(8));
    }
    for (uint32_t i = 0; i < 8; i++) {
      PutString(base, shapes + 544 * i, kShapeNames[g.Below(std::size(kShapeNames))]);
      Put(base, shapes + 544 * i + 396, block + kMaterials + 0x800 * g.Below(2));
    }
    std::vector<std::string> names(1 + g.Below(8));
    for (auto& n : names) n = kNodeNames[g.Below(std::size(kNodeNames))];
    MakeNodeFile(base, file, names, g);
    const uint32_t skeleton = model + 112;
    for (const SkeletonArray& a : SkeletonArrays(base, file)) Put(base, skeleton + a.offset, 0);
    if (!model_animation::SkeletonBind(base, skeleton, file, 0)) {
      ITC_LOG_WARN("harness: second_set: Skeleton::Bind declined");
    }
    // The arrays Bind allocates and leaves as the heap had them (a side's
    // would hold the other side's results) filled from the seed.
    for (const SkeletonArray& a : SkeletonArrays(base, file)) {
      const uint32_t at = guest::LoadU32(base, skeleton + a.offset);
      if (at != 0 && (a.offset == 132 || a.offset == 136 || a.offset == 144)) {
        for (uint32_t w = 0; w < (a.bytes - a.header) / 4; w++) {
          Put(base, at + 4 * w, SmallFloat(g));
        }
      }
    }
    // The root node (+156, Skeleton::UpdateNodeMatrices' first multiply):
    // one of the nodes, as the game's are.
    Put(base, skeleton + 156, g.Below(uint32_t(names.size())));
  }
  for (uint32_t b = 0; b < 2; b++) {
    const uint32_t materials = block + kMaterials + 0x800 * b;
    Put(base, materials + 8, g.Below(2));
    Put(base, materials + 12, g.Below(3));
    for (uint32_t i = 0; i < 3; i++) {
      for (uint32_t w = 0; w < 4; w++) {
        Put(base, materials + 16 + 512 * i + 4 * w, SmallFloat(g));
      }
    }
  }
  for (size_t s = 0; s < span_count; s++) {
    for (uint32_t i = 0; i < spans[s].records; i++) {
      const uint32_t record = block + kPoints + spans[s].at + kPointRecordBytes * i;
      PutString(base, record + 160, PickName(g, kNodeNames, std::size(kNodeNames)));
      PutString(base, record + 192, PickName(g, kShapeNames, std::size(kShapeNames)));
    }
  }
  for (uint32_t k = 0; k < kinds; k++) {
    // The parts' kinds may be missing (0); the cockpit's six are always
    // there (its Find reads each).
    const uint32_t which = g.Below(5);
    Put(base, block + kModelWords + 4 * k,
        which == 0 && kinds == 17 ? 0 : block + kModels + 0x400 * (which & 1));
  }
  // The interior's record ([+11444], its +100 an index or 0) and the dials'
  // car block (+244); the parts' and others' words there are random anyway.
  if (kinds == 6) {
    Put(base, block + 11444, block + kCarObject);
    Put(base, block + kCarObject + 100, g.Below(3) == 0 ? 0 : g.Below(8));
    static const uint32_t kBlocks[] = {1, 10, 26, 37, 5, 0xFFFFFFFFu};
    Put(base, block + 244, kBlocks[g.Below(std::size(kBlocks))]);
  }
  Put(base, block + 37084, block + kCarLoads);
  Put(base, block + kCarLoads + 24, block + kCarObject);
  Put(base, block + 37088, block + kScene);
  Put(base, block + kScene + 4, set);
}

// The block's words as outputs: the point records' +16 to +160 (`spans`),
// the mufflers' matrices and the headlights' copied float (the parts') as
// floats, the rest exactly.
void TakeBlock(uint8_t* base, uint32_t block, uint32_t bytes, const Span* spans,
               size_t span_count, bool parts, Outputs& out) {
  std::vector<bool> floats(bytes / 4, false);
  auto mark = [&](uint32_t from, uint32_t count) {
    for (uint32_t w = 0; w < count && from / 4 + w < floats.size(); w++) floats[from / 4 + w] = true;
  };
  for (size_t s = 0; s < span_count; s++) {
    for (uint32_t i = 0; i < spans[s].records; i++) {
      mark(spans[s].at + kPointRecordBytes * i + 16, 36);
    }
  }
  if (parts) {
    mark(34640, 64);
    mark(24480 + 3184, 1);
  }
  uint32_t w = 0;
  while (w < floats.size()) {
    uint32_t end = w;
    while (end < floats.size() && floats[end] == floats[w]) end++;
    if (floats[w]) {
      out.Floats(base, block + 4 * w, end - w);
    } else {
      out.Bytes(Read(base, block + 4 * w, 4 * (end - w)));
    }
    w = end;
  }
}


void FindSide(int v, GuestFunction recompiled, GuestFunction native, PPCContext& t,
              uint8_t* base) {
  ITC_FLAG(native_second_set) = v == 1;
  ITC_FLAG(native_scene_loaders) = v == 1;
  ITC_FLAG(native_material_passes) = v == 1;
  (v == 0 ? recompiled : native)(t, base);
  ITC_FLAG(native_second_set) = true;
  ITC_FLAG(native_scene_loaders) = true;
  ITC_FLAG(native_material_passes) = true;
}

// One set-up case: the world from `seed`, r3 = the block + `at`, r4 = model
// `model` (0 or 1) or, for -1, the models' words.
void FindCase(Context& h, Results& results, uint32_t block, uint64_t seed, const Span* spans,
              size_t span_count, uint32_t bytes, bool parts, uint32_t kinds, uint32_t set,
              uint32_t at,
              int model, GuestFunction recompiled, GuestFunction native, const std::string& what) {
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  const uint32_t stack = start.r1.u32 - kStackBytes;
  Random sr(seed ^ 0x5354);
  RandomBytes(h.base, stack, kStackBytes, sr);
  const std::vector<uint8_t> frames = Read(h.base, stack, kStackBytes);
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    BuildWorld(h.base, block, seed, spans, span_count, kinds, set);
    Write(h.base, stack, frames.data(), kStackBytes);
    PPCContext t = start;
    t.r3.u64 = block + at;
    t.r4.u64 = model < 0 ? block + kModelWords : block + kModels + 0x400 * uint32_t(model);
    t.r5.u64 = block + kModels + 0x400;  // FindBody's models 1 to 3
    t.r6.u64 = block + kModels;
    t.r7.u64 = block + kModels + 0x400;
    FindSide(v, recompiled, native, t, h.base);
    TakeBlock(h.base, block, bytes, spans, span_count, parts, out[v]);
    for (uint32_t m = 0; m < 2; m++) {
      const uint32_t object = block + kModels + 0x400 * m;
      const uint32_t file = block + kFiles + 0x1000 * m;
      std::vector<uint8_t> words = Read(h.base, object, 464);
      for (const SkeletonArray& a : SkeletonArrays(h.base, file)) {
        std::fill_n(words.begin() + 112 + a.offset, 4, 0);
      }
      out[v].Bytes(words);
      TakeSkeleton(h.base, object + 112, file, out[v]);
    }
    for (uint32_t b = 0; b < 2; b++) {
      const uint32_t materials = block + kMaterials + 0x800 * b;
      out[v].Bytes(Read(h.base, materials, 16));
      for (uint32_t i = 0; i < 3; i++) {
        out[v].Floats(h.base, materials + 16 + 512 * i, 4);
        out[v].Bytes(Read(h.base, materials + 32 + 512 * i, 96));
      }
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Finds(Context& h, Random& r, uint32_t block) {
  if (!hooks::SecondSetFindsNative()) {
    ITC_LOG_WARN("harness: second_set: the point set-ups skipped (memcpy, --native_models or "
                 "--native_material_passes not native)");
    return;
  }
  struct Find {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t at;  // the group in the parts' block
  };
#define ITC_FIND_STEP(function) \
  [](PPCContext& c, uint8_t* b) { hooks::SecondSetFindStep(c, b, ss::function); }
  const Find finds[] = {
      {"PartsPoints::FindFrontBumper (sub_820DE140)", __imp__sub_820DE140,
       ITC_FIND_STEP(PartsFindFrontBumper), 0},
      {"PartsPoints::FindRearBumper (sub_820DE328)", __imp__sub_820DE328,
       ITC_FIND_STEP(PartsFindRearBumper), 2896},
      {"PartsPoints::FindRearSpoiler (sub_820DE4E8)", __imp__sub_820DE4E8,
       ITC_FIND_STEP(PartsFindRearSpoiler), 4832},
      {"PartsPoints::FindBonnet (sub_820DE5B0)", __imp__sub_820DE5B0,
       ITC_FIND_STEP(PartsFindBonnet), 5552},
      {"PartsPoints::FindLeftDoor (sub_820DE668)", __imp__sub_820DE668,
       ITC_FIND_STEP(PartsFindLeftDoor), 6032},
      {"PartsPoints::FindRightDoor (sub_820DE878)", __imp__sub_820DE878,
       ITC_FIND_STEP(PartsFindRightDoor), 12128},
      {"PartsPoints::FindDoors (sub_820DEA20)", __imp__sub_820DEA20,
       ITC_FIND_STEP(PartsFindDoors), 18224},
      {"PartsPoints::FindHeadlights (sub_820DF7F0)", __imp__sub_820DF7F0,
       ITC_FIND_STEP(PartsFindHeadlights), 24480},
      {"PartsPoints::FindTailLights (sub_820DEC58)", __imp__sub_820DEC58,
       ITC_FIND_STEP(PartsFindTailLights), 27680},
      {"PartsPoints::FindFenders (sub_820DED20)", __imp__sub_820DED20,
       ITC_FIND_STEP(PartsFindFenders), 31280},
      {"PartsPoints::FindCabin (sub_820DEED8)", __imp__sub_820DEED8,
       ITC_FIND_STEP(PartsFindCabin), 32960},
      {"PartsPoints::FindMufflers (sub_820E0080)", __imp__sub_820E0080,
       ITC_FIND_STEP(PartsFindMufflers), 34640},
  };
  for (const Find& f : finds) {
    Results results(f.name);
    for (int c = 0; c < 8; c++) {
      FindCase(h, results, block, r.Next(), kPartsSpans, std::size(kPartsSpans), 37096, true, 17,
               r.Below(2), f.at, int(r.Below(2)), f.recompiled, f.native,
               fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("PartsPoints::Find (sub_820E0258)");
    for (int c = 0; c < 16; c++) {
      const uint32_t set = c % 2 == 0 ? 0 : (c % 4 == 1 ? 1 : uint32_t(r.Next()));
      FindCase(h, results, block, r.Next(), kPartsSpans, std::size(kPartsSpans), 37096, true, 17, set,
               0, -1, __imp__sub_820E0258, hooks::SecondSetPartsFind,
               fmt::format("case {} (set {:#x})", c, set));
    }
    results.Report();
  }
  {
    Results results("CockpitPoints::FindHeadlights (sub_820DBB08)");
    for (int c = 0; c < 8; c++) {
      FindCase(h, results, block, r.Next(), kCockpitSpans, std::size(kCockpitSpans), 4096, false, 6,
               0, 2160, int(r.Below(2)), __imp__sub_820DBB08,
               ITC_FIND_STEP(CockpitFindHeadlights), fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CockpitPoints::Find (sub_820DBC88)");
    for (int c = 0; c < 12; c++) {
      FindCase(h, results, block, r.Next(), kCockpitSpans, std::size(kCockpitSpans), 4096, false, 6,
               0, 0, -1, __imp__sub_820DBC88, hooks::SecondSetCockpitFind,
               fmt::format("case {}", c));
    }
    results.Report();
  }
  // The interior's, the light kit's and the traffic body's.
  struct Other {
    const char* name;
    GuestFunction recompiled, native;
    const Span* spans;
    size_t span_count;
    uint32_t bytes, at;
    bool words;  // r4 the models' words, else one model
  };
  const Other others[] = {
      {"InteriorPoints::FindDials (sub_820E30C0)", __imp__sub_820E30C0,
       ITC_FIND_STEP(InteriorFindDials), kInteriorSpans, std::size(kInteriorSpans), 11460, 240,
       false},
      {"InteriorPoints::FindMeters (sub_820E3298)", __imp__sub_820E3298,
       ITC_FIND_STEP(InteriorFindMeters), kInteriorSpans, std::size(kInteriorSpans), 11460, 3152,
       false},
      {"InteriorPoints::FindSeats (sub_820E34A8)", __imp__sub_820E34A8,
       ITC_FIND_STEP(InteriorFindSeats), kInteriorSpans, std::size(kInteriorSpans), 11460, 9488,
       false},
      {"InteriorPoints::Find (sub_820E3660)", __imp__sub_820E3660, hooks::SecondSetInteriorFind,
       kInteriorSpans, std::size(kInteriorSpans), 11460, 0, true},
      {"LightPoints::FindGroup (sub_820DC850)", __imp__sub_820DC850,
       ITC_FIND_STEP(LightsFindGroup), kLightsSpans, std::size(kLightsSpans), 13072, 48, false},
      {"LightPoints::Find (sub_820DC900)", __imp__sub_820DC900, hooks::SecondSetLightsFind,
       kLightsSpans, std::size(kLightsSpans), 13072, 0, false},
      {"TrafficPoints::FindBody (sub_820E5538)", __imp__sub_820E5538,
       [](PPCContext& c, uint8_t* b) {
         c.r3.u64 = ss::TrafficFindBody(b, c.r3.u32, c.r4.u32, c.r5.u32, c.r6.u32, c.r7.u32,
                                        c.r1.u32);
       },
       kTrafficSpans, std::size(kTrafficSpans), 6080, 16, false},
      {"TrafficPoints::FindHeadlights (sub_820E5738)", __imp__sub_820E5738,
       ITC_FIND_STEP(TrafficFindHeadlights), kTrafficSpans, std::size(kTrafficSpans), 6080, 1472,
       false},
      {"TrafficPoints::FindTailLights (sub_820E5808)", __imp__sub_820E5808,
       ITC_FIND_STEP(TrafficFindTailLights), kTrafficSpans, std::size(kTrafficSpans), 6080, 3152,
       false},
      {"TrafficPoints::Find (sub_820E58C8)", __imp__sub_820E58C8, hooks::SecondSetTrafficFind,
       kTrafficSpans, std::size(kTrafficSpans), 6080, 0, true},
  };
  for (const Other& o : others) {
    Results results(o.name);
    for (int c = 0; c < 12; c++) {
      FindCase(h, results, block, r.Next(), o.spans, o.span_count, o.bytes, false, 6, 0, o.at,
               o.words ? -1 : int(r.Below(2)), o.recompiled, o.native, fmt::format("case {}", c));
    }
    results.Report();
  }
#undef ITC_FIND_STEP
}


// --- The wheels' set-up ---
//
// A world for WheelPoints::SetUp and the tire's, rims' and brakes' set-ups:
// the wheels block (at +0x400; the tire's entries may lie before a group),
// the wheels record, two car records and matrices (the block's +4 and +8,
// and the car model's loading object's +32 and +6976), the scene (its +4
// the model set, [0x826C2E44] pointed at it for the cases and put back
// after), a part model pair (+464 apart: each kind's model and the rear's)
// with up to 8 nodes and 8 shapes, a shape before them (index -1, the
// morph's shape for a record not found), their streams (one vertex count
// and kind a model, so Morph::SetUp's DebugPrintf never runs, as in play;
// the sizes a multiple of 16 that may differ from the vertices' and each
// other) and two material blocks. Each case through both versions from the
// same random block and stack, the recompiled one with --native_second_set
// off (FindShape and the morph the originals; AllocPages, FreePagesIn,
// memcpy, ScaleBlended and SetTexture the native adapters on both sides).
// The block (the streams, the materials' first four words, the tires'
// entries and outputs, the rims' and brakes' weights as floats), the
// frames' words but the saved registers' slots (the morph records' weights
// as floats), r3 and the flush mode compared.
constexpr uint32_t kWheelBlock = 0x400;
constexpr uint32_t kWheelsRecord = 0x4600;
constexpr uint32_t kCarRecordA = 0x4680;
constexpr uint32_t kMatrixA = 0x46C0;
constexpr uint32_t kLoading = 0x4700;
constexpr uint32_t kWheelScene = 0x4780;
constexpr uint32_t kLoadTarget = 0x4800;  // +32 car record B, +6976 matrix B
constexpr uint32_t kWheelModels = 0x6400;
constexpr uint32_t kWheelNodes = 0x6800;   // 0x600 a model
constexpr uint32_t kWheelShapes = 0x7400;  // 0x1400 a model, shape -1 first
constexpr uint32_t kWheelMaterials = 0x9C00;  // two blocks 0x900 apart
constexpr uint32_t kWheelStreams = 0xB000;    // 0x420 a shape: H1, H2 at +0x210
constexpr uint32_t kWheelWorldBytes = 0x10000;

uint32_t WheelShape(uint32_t b, uint32_t m, int32_t s) {
  return b + kWheelShapes + 0x1400 * m + 544 * uint32_t(s + 1);
}
uint32_t WheelHeader(uint32_t b, uint32_t m, int32_t s) {
  return b + kWheelStreams + 0x420 * (9 * m + uint32_t(s + 1));
}

// The groups: (offset in the wheels block, first record, records).
struct WheelGroup {
  uint32_t at, first, records;
};
const WheelGroup kWheelGroups[] = {
    {32, 368, 6},     {1872, 368, 6},   {3712, 32, 7},   {5456, 32, 7},
    {7200, 32, 7},    {8944, 32, 7},    {10688, 32, 12}, {13632, 32, 12}};

void BuildWheelWorld(uint8_t* base, uint32_t b, Random& r) {
  for (uint32_t m = 0; m < 2; m++) {
    const uint32_t model = b + kWheelModels + 464 * m;
    const uint32_t nodes = b + kWheelNodes + 0x600 * m;
    Put(base, model + 36, nodes);
    Put(base, model + 40, WheelShape(b, m, 0));
    Put(base, model + 408, 1 + r.Below(8));
    Put(base, model + 412, r.Below(9));
    for (uint32_t n = 0; n < 8; n++) {
      Put(base, nodes + 192 * n + 40, r.Below(6) == 0 ? 0xFFFFFFFFu : r.Below(8));
    }
    const uint32_t vertices = 1 + r.Below(16);
    const uint32_t kind = r.Below(6) == 0 ? 9 : 6 + r.Below(2);
    for (int32_t s = -1; s < 8; s++) {
      const uint32_t shape = WheelShape(b, m, s);
      const uint32_t h1 = WheelHeader(b, m, s), h2 = h1 + 0x210;
      PutString(base, shape, kShapeNames[r.Below(std::size(kShapeNames))]);
      Put(base, shape + 268, 16 * (1 + r.Below(32)));
      Put(base, shape + 284, 16 * (1 + r.Below(32)));
      Put(base, shape + 388, h1 + 16);
      Put(base, shape + 392, h2 + 16);
      Put(base, shape + 396, b + kWheelMaterials + 0x900 * r.Below(2));
      Put(base, shape + 400, h1);
      Put(base, shape + 404, h2);
      Put(base, h1 + 0, vertices);
      Put(base, h2 + 4, kind);
      for (uint32_t w = 0; w < 0x200 / 4; w++) {
        Put(base, h1 + 16 + 4 * w, MixedFloat(r));
        Put(base, h2 + 16 + 4 * w, MixedFloat(r));
      }
    }
  }
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t materials = b + kWheelMaterials + 0x900 * k;
    Put(base, materials + 8, r.Below(3));
    Put(base, materials + 12, r.Below(3));
    for (uint32_t i = 0; i < 4; i++) {
      for (uint32_t w = 0; w < 4; w++) {
        Put(base, materials + 16 + 512 * i + 4 * w, MixedFloat(r));
      }
    }
  }
  // The wheels record: +0 the kind (3 traffic), +8 the morph's switch (0
  // runs it), +44 and +56 an entry's index or -1 (with +48, +52, +60 and +64
  // a float's address and a float), +68 the brakes' kind, +72 and +76 added
  // to the table's index, +88 the texture, +108 and +112 the brakes' steps.
  const uint32_t wheels = b + kWheelsRecord;
  const uint32_t kind = r.Below(4) == 0 ? 3 : r.Below(7);
  Put(base, wheels + 0, kind);
  Put(base, wheels + 8, r.Below(4) == 0 ? 1 + r.Below(3) : 0);
  for (const uint32_t at : {44u, 56u}) {
    Put(base, wheels + at, r.Below(2) == 0 ? 0xFFFFFFFFu : 12 + r.Below(12));
    Put(base, wheels + at + 4, MixedFloat(r));
    Put(base, wheels + at + 8, MixedFloat(r));
  }
  Put(base, wheels + 68, r.Below(8));
  Put(base, wheels + 72, r.Below(3));
  Put(base, wheels + 76, r.Below(3));
  Put(base, wheels + 88, uint32_t(r.Next()));
  Put(base, wheels + 108, 9 + r.Below(12));
  Put(base, wheels + 112, 9 + r.Below(12));
  // The car records' +0 (an index into the image's tables: 45 cars, 4 for
  // traffic) and the matrices.
  for (const uint32_t car : {b + kCarRecordA, b + kLoadTarget + 32}) {
    Put(base, car + 0, kind == 3 ? r.Below(4) : r.Below(45));
  }
  for (const uint32_t matrix : {b + kMatrixA, b + kLoadTarget + 6976}) {
    for (uint32_t w = 0; w < 16; w++) Put(base, matrix + 4 * w, SmallFloat(r));
  }
  Put(base, b + kLoading + 24, b + kLoadTarget);
  // The records' shape names.
  const uint32_t block = b + kWheelBlock;
  for (const WheelGroup& g : kWheelGroups) {
    for (uint32_t i = 0; i < g.records; i++) {
      PutString(base, block + g.at + g.first + kPointRecordBytes * i + 192,
                PickName(r, kShapeNames, std::size(kShapeNames)));
    }
  }
}

// The block's float ranges.
void WheelFloats(uint32_t b, std::vector<FloatRange>& floats) {
  const uint32_t block = b + kWheelBlock;
  for (const uint32_t tire : {32u, 1872u}) {
    floats.push_back({block + tire + 12, 288 / 4});
    floats.push_back({block + tire + 328, 32 / 4});
  }
  for (const uint32_t rim : {3712u, 5456u, 7200u, 8944u}) floats.push_back({block + rim + 16, 2});
  for (const uint32_t brakes : {10688u, 13632u}) floats.push_back({block + brakes + 16, 1});
  for (uint32_t m = 0; m < 2; m++) {
    for (int32_t s = -1; s < 8; s++) {
      floats.push_back({WheelHeader(b, m, s) + 16, 0x200 / 4});
      floats.push_back({WheelHeader(b, m, s) + 0x210 + 16, 0x200 / 4});
    }
  }
  for (uint32_t k = 0; k < 2; k++) {
    for (uint32_t i = 0; i < 4; i++) {
      floats.push_back({b + kWheelMaterials + 0x900 * k + 16 + 512 * i, 4});
    }
  }
}

// The frame's words at offsets from r1 (`from` to `to`, step 4).
std::vector<uint32_t> FrameWords(int32_t from, int32_t to) {
  std::vector<uint32_t> words;
  for (int32_t at = from; at < to; at += 4) words.push_back(uint32_t(at));
  return words;
}

void Wheels(Context& h, Random& r, uint32_t b) {
  if (!hooks::SecondSetWheelsNative()) {
    ITC_LOG_WARN("harness: second_set: the wheels' set-up skipped (memcpy, the heap or "
                 "--native_material_passes not native)");
    return;
  }
  const uint32_t saved_scene = guest::LoadU32(h.base, scene::kScene);
  Put(h.base, scene::kScene, b + kWheelScene);
  const uint32_t block = b + kWheelBlock;
  const uint32_t model = b + kWheelModels;
#define ITC_WHEEL_STEP(function) \
  [](PPCContext& c, uint8_t* base) { hooks::SecondSetWheelStep(c, base, ss::function); }
  struct Step {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t front, rear;  // the groups
    int kind;              // 0 the tire, 1 a rim, 2 the brakes
  };
  const Step steps[] = {
      {"WheelPoints::SetUpTire (sub_820E7598)", __imp__sub_820E7598,
       ITC_WHEEL_STEP(WheelsSetUpTire), 32, 1872, 0},
      {"WheelPoints::SetUpRimA (sub_820E79C8)", __imp__sub_820E79C8,
       ITC_WHEEL_STEP(WheelsSetUpRimA), 3712, 5456, 1},
      {"WheelPoints::SetUpRimB (sub_820E7CA0)", __imp__sub_820E7CA0,
       ITC_WHEEL_STEP(WheelsSetUpRimB), 7200, 8944, 1},
      {"WheelPoints::SetUpBrakes (sub_820E8028)", __imp__sub_820E8028,
       ITC_WHEEL_STEP(WheelsSetUpBrakes), 10688, 13632, 2},
  };
#undef ITC_WHEEL_STEP
  // Each set-up's frame (its size; the back chain, the words from +80 on
  // below the saved registers) and the morph
  // records' weights (and the rims' and brakes' frame weight) as floats.
  for (const Step& step : steps) {
    Results results(step.name);
    const uint32_t frame = step.kind == 0 ? 288 : (step.kind == 1 ? 336 : 368);
    std::vector<Window> windows = {{frame, 4}};
    std::vector<uint32_t> float_words;
    if (step.kind == 0) {
      // (Not its red-zone word at r1 - 304, which sub_820E6F28 and
      // sub_820E7010 write: once the morph runs, its callees' saved
      // registers go there.)
      windows.push_back({208, 140});
      float_words = FrameWords(-112, -84);
    } else if (step.kind == 1) {
      windows.push_back({256, 204});
      float_words = FrameWords(-96, -68);
      float_words.push_back(uint32_t(-208));
    } else {
      windows.push_back({288, 220});
      float_words = FrameWords(-112, -84);
      float_words.push_back(uint32_t(-224));
    }
    for (int c = 0; c < 16; c++) {
      const bool rear = c % 2 == 1;
      MorphCase(
          h, results, b, kWheelWorldBytes, r, r.Below(2) == 0,
          [&](PPCContext& t, std::vector<FloatRange>& floats) {
            BuildWheelWorld(h.base, b, r);
            WheelFloats(b, floats);
            const uint32_t group = block + (rear ? step.rear : step.front);
            const bool b_side = r.Below(2) == 0;
            Put(h.base, group + 0, b_side ? b + kLoadTarget + 6976 : b + kMatrixA);
            Put(h.base, group + 8, b + kWheelsRecord);
            if (step.kind == 0) {
              Put(h.base, group + 4, b_side ? b + kLoadTarget + 32 : b + kCarRecordA);
              Put(h.base, group + 300, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
            } else {
              Put(h.base, group + 12, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
              if (step.kind == 1) Put(h.base, group + 16, MixedFloat(r));
            }
            t.r3.u64 = group;
            t.r4.u64 = model + (rear ? 464 : 0);
          },
          step.recompiled, step.native, windows, float_words,
          fmt::format("case {} ({})", c, rear ? "rear" : "front"));
    }
    results.Report();
  }
  // WheelPoints::SetUp: the frames of the tires (r1 - 400), rims (- 448)
  // and brakes (- 480) under its own (- 112): their back chains and the
  // words from - 400 to - 168 (the callees' saved registers above, and
  // below the tire's frame its callees'), compared exactly (their floats
  // are copies, or the brakes' weight from an integer).
  {
    Results results("WheelPoints::SetUp (sub_820E8668)");
    for (int c = 0; c < 16; c++) {
      const uint32_t set = c % 3 == 0 ? 0 : (c % 3 == 1 ? 1 : uint32_t(r.Next()));
      MorphCase(
          h, results, b, kWheelWorldBytes, r, r.Below(2) == 0,
          [&](PPCContext& t, std::vector<FloatRange>& floats) {
            BuildWheelWorld(h.base, b, r);
            WheelFloats(b, floats);
            Put(h.base, b + kWheelScene + 4, set);
            Put(h.base, block + 0, r.Below(2) == 0 ? 0 : b + kLoading);
            Put(h.base, block + 4, b + kCarRecordA);
            Put(h.base, block + 8, b + kMatrixA);
            Put(h.base, block + 12, b + kWheelsRecord);
            Put(h.base, block + 16, model);
            Put(h.base, block + 20, model);
            Put(h.base, block + 24, model);
            t.r3.u64 = block;
          },
          __imp__sub_820E8668, hooks::SecondSetWheelPointsSetUp,
          {{112, 4}, {448, 4}, {480, 4}, {400, 232}}, {},
          fmt::format("case {} (set {:#x})", c, set));
    }
    results.Report();
  }
  Put(h.base, scene::kScene, saved_scene);
}

}  // namespace

void SecondSet(Context& h) {
  Random r(0x7365636F6E64ull);  // "second"
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Points(h, r, block);
  Groups(h, r, block);
  Finds(h, r, block);
  Objects(h, r, block);
  ReleaseBs(h, r, block);
  CarModelReleases(h, r, block);
  Morphs(h, r, block);
  Wheels(h, r, block);
  memory::Free(allocation);
}

}  // namespace harness
