// The race's update (src/race_update/, docs/functions/race-update.md)
// against the recompiled functions. The originals run with
// --native_race_update off, the plain C++ through the register steps
// (hooks/race_update.h). Each case fills its blocks (from the allocation
// layer) and the stack below the call's stack pointer with random words from
// the case's seed, shapes the fields the functions test, runs one side, and
// compares what it leaves; the game's globals the functions write are put
// back after each side. Compared with harness::Outputs: the vectors
// RaceLights::UploadDirectional normalizes as floats, everything else
// exactly.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "course_scenery/course_scenery.h"
#include "hooks/course_scenery.h"
#include "hooks/race_update.h"
#include "memory/heap.h"
#include "race_update/race_update.h"

#include <rex/memory/utils.h>

#include <fmt/format.h>

#include <algorithm>
#include <cstring>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_update);
ITC_FLAG_DECLARE(bool, native_course_scenery);

namespace harness {

namespace {

namespace ru = race_update;

constexpr uint32_t kStackBytes = 512;  // below the call's stack pointer

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

// Guest bytes put back when it ends.
class SavedBytes {
 public:
  SavedBytes(uint8_t* base, std::vector<std::pair<uint32_t, uint32_t>> ranges)
      : base_(base), ranges_(std::move(ranges)) {
    for (const auto& [address, bytes] : ranges_) {
      saved_.push_back(Read(base_, address, bytes));
    }
  }
  ~SavedBytes() {
    for (size_t i = 0; i < ranges_.size(); i++) {
      Write(base_, ranges_[i].first, saved_[i].data(), ranges_[i].second);
    }
  }

 private:
  uint8_t* base_;
  std::vector<std::pair<uint32_t, uint32_t>> ranges_;
  std::vector<std::vector<uint8_t>> saved_;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// The original with --native_race_update off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  ITC_FLAG(native_race_update) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_race_update) = true;
}

// A small signed number, an edge, or any word.
int32_t Argument(Random& r) {
  switch (r.Below(6)) {
    case 0: return int32_t(r.Below(20));
    case 1: return int32_t(r.Below(200)) - 100;
    case 2: return int32_t(r.Next());
    case 3: {
      constexpr int32_t kEdges[] = {0, -1, 1, 5, 6, -6, 9, 10, -10, INT32_MIN, INT32_MAX,
                                    214748364, -214748365};
      return kEdges[r.Below(sizeof(kEdges) / sizeof(kEdges[0]))];
    }
    default: return int32_t(r.Below(8));
  }
}

// --- RaceScene::VaryCar: random cars (the word +12 64 in half of them),
// the three numbers small, negative, at the edges or any word; the car's
// 1,600 bytes and r3 compared ---

void VaryCars(Context& h) {
  const Block car(1600);
  if (!car.address) {
    ITC_LOG_ERROR("harness: race_update: no block from the allocation layer");
    return;
  }
  Results results("RaceScene::VaryCar (sub_8215B330)");
  for (uint64_t seed = 0; seed < 600; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 977 + 13);
      WriteRandom(h.base, car.address, car.bytes, r);
      if (r.Below(2) == 0) {
        guest::StoreU32(h.base, car.address + 12, 64);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = car.address;
      t.r4.s64 = Argument(r);
      t.r5.s64 = Argument(r);
      t.r6.s64 = Argument(r);
      (v == 0 ? Original<__imp__sub_8215B330> : hooks::RaceUpdateVaryCar)(t, h.base);
      out[v].Bytes(Read(h.base, car.address, car.bytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// A float lane: ordinary, zero, tiny (denormal), huge, infinite or NaN.
uint32_t Lane(Random& r) {
  switch (r.Below(10)) {
    case 0: return 0;
    case 1: return 0x80000000u;
    case 2: return uint32_t(r.Below(0x800000));          // denormal
    case 3: return 0x7F800000u | (r.Below(2) << 31);     // infinity
    case 4: return 0x7FC00000u | uint32_t(r.Below(64));  // NaN
    case 5: return 0x1F000000u + uint32_t(r.Below(0x1000000));  // tiny, squares flush
    case 6: return 0x5F000000u + uint32_t(r.Below(0x1000000));  // huge, squares overflow
    default: {
      float f = (float(r.Below(20001)) - 10000.0f) / float(1 + r.Below(1000));
      uint32_t bits;
      std::memcpy(&bits, &f, 4);
      return bits;
    }
  }
}

// --- RaceLights::Update with UploadDirectional: a random light set (11
// entries of kind 1 now and then, else 2, 0 or any word; their directions'
// lanes ordinary, zero, denormal, huge, infinite or NaN, all-zero now and
// then), the game's arrays and the origin filled with the same random words
// first; the set, the arrays (as floats: the directions, overlapping the
// others past slot 2), the origin, the stack below the call's stack pointer
// (UploadDirectional's vector as floats; but the original's back chain and
// register saves), r3 and r4 compared ---

void Lights(Context& h) {
  const Block set(1088);
  if (!set.address) {
    ITC_LOG_ERROR("harness: race_update: no block from the allocation layer");
    return;
  }
  constexpr uint32_t kArrays = ru::kDirections;
  constexpr uint32_t kArrayBytes = ru::kThird + 16 * 11 - ru::kDirections;
  const SavedBytes saved(h.base, {{kArrays, kArrayBytes}, {ru::kOrigin, 16}});
  Results results("RaceLights::Update (sub_8215D008) with UploadDirectional (sub_8215C698)");
  for (uint64_t seed = 0; seed < 300; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 613 + 71);
      PPCContext t = CallContext(h);
      const uint32_t r1 = t.r1.u32;
      WriteRandom(h.base, r1 - kStackBytes, kStackBytes, r);
      WriteRandom(h.base, set.address, set.bytes, r);
      WriteRandom(h.base, kArrays, kArrayBytes, r);
      WriteRandom(h.base, ru::kOrigin, 16, r);
      const uint32_t mode = r.Below(4);  // 0: the game's three, 1: many, 2: none, 3: any
      for (uint32_t i = 0; i < 11; i++) {
        const uint32_t entry = set.address + 16 + 96 * i;
        uint32_t kind = r.Below(3);
        if (mode == 0) {
          kind = i < 3 ? 1 : 2;
        } else if (mode == 1) {
          kind = r.Below(4) != 0 ? 1 : 2;
        } else if (mode == 3 && r.Below(4) == 0) {
          kind = uint32_t(r.Next());
        }
        guest::StoreU32(h.base, entry, kind);
        const bool zero = r.Below(8) == 0;
        for (uint32_t k = 0; k < 4; k++) {
          guest::StoreU32(h.base, entry + 16 + 4 * k, zero && k < 3 ? 0 : Lane(r));
        }
      }
      t.r3.u64 = set.address;
      (v == 0 ? Original<__imp__sub_8215D008> : hooks::RaceUpdateLights)(t, h.base);
      out[v].Bytes(Read(h.base, set.address, set.bytes));
      out[v].Floats(h.base, kArrays, kArrayBytes / 4);
      out[v].Bytes(Read(h.base, ru::kOrigin, 16));
      // Below the frame (UploadDirectional's normalized vector at its -16,
      // floats), then the frame past its back chain up to the register saves.
      const uint32_t frame = r1 - ru::kLightsFrame;
      out[v].Bytes(Read(h.base, r1 - kStackBytes, frame - 16 - (r1 - kStackBytes)));
      out[v].Floats(h.base, frame - 16, 4);
      out[v].Bytes(Read(h.base, frame + 4, ru::kLightsFrame - 4 - 16));
      out[v].Word(t.r3.u32);
      out[v].Word(t.r4.u32);
      out[v].Word(t.r1.u32);
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// --- RaceScene::LoadScenery: the real configurations (0x82602EA0) from a
// race data block of our own ([0x826C5B2C] pointed at it: the course +4,
// the mode +32, +0xA3AF0 4 or not for split screen); the course set's load
// slot (CourseSet::Load, slot 1 of 0x82006784, in the image's read-only
// data: its page made writable through the SDK's memory utilities, as the
// framework harness's main cases do, then given its access back) pointed at
// PoolObject::Reset while the cases run, so no file is read; the stack below the call filled
// alike on both sides (the configuration's +32 is its leftover). The
// recompiled side with --native_race_update and --native_course_scenery
// off. Compared: r3 against the course set, the scene's +0 against the
// manager, the configuration's 44 bytes, and the manager (SceneryOutputs,
// with Create's provider). ---

void LoadSceneryCases(Context& h) {
  namespace cs = course_scenery;
  Random r(0x4C4F4144u);
  Results results("RaceScene::LoadScenery (sub_8215B790)");
  constexpr uint32_t kCourseSetLoad = 0x82006788, kCourseSetLoadOriginal = 0x820F2520,
                     kPoolObjectReset = 0x820FB9D8;
  if (!hooks::CourseScenerySetUpNative() ||
      guest::LoadU32(h.base, kCourseSetLoad) != kCourseSetLoadOriginal) {
    ITC_LOG_INFO("harness: RaceScene::LoadScenery: can't run here, no cases");
    results.Report();
    return;
  }
  const size_t page_size = rex::memory::page_size();
  uint8_t* const page = reinterpret_cast<uint8_t*>(
      reinterpret_cast<uintptr_t>(h.base + kCourseSetLoad) & ~uintptr_t(page_size - 1));
  rex::memory::PageAccess access = rex::memory::PageAccess::kReadOnly;
  if (!rex::memory::Protect(page, page_size, rex::memory::PageAccess::kReadWrite, &access)) {
    ITC_LOG_ERROR("harness: RaceScene::LoadScenery: the course set's vtable can't be made "
                  "writable");
    results.Report();
    return;
  }
  {
    const SavedBytes saved(h.base,
                           {{ru::kRaceData, 4}, {cs::kManager, 12}, {kCourseSetLoad, 4}});
    guest::StoreU32(h.base, kCourseSetLoad, kPoolObjectReset);
    const uint32_t data = memory::AllocPages(0xA4000, 4096);
    const Block scene(16);
    for (int c = 0; c < 12; c++) {
      const uint32_t course = c < 3 ? 8 + uint32_t(c) : r.Below(13);
      const uint32_t mode = r.Below(3);
      const uint32_t split = r.Below(2) == 0 ? 4 : r.Below(6);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        guest::StoreU32(h.base, ru::kRaceData, data);
        guest::StoreU32(h.base, data + 4, course);
        guest::StoreU32(h.base, data + 32, mode);
        guest::StoreU32(h.base, data + 0xA3AF0, split);
        guest::StoreU32(h.base, scene.address, 0xCDCDCDCD);
        PPCContext t = CallContext(h);
        Fill(h.base, t.r1.u32 - 2048, 2048, uint8_t(0x40 + c));
        t.r3.u64 = scene.address;
        if (v == 0) {
          ITC_FLAG(native_race_update) = false;
          ITC_FLAG(native_course_scenery) = false;
          __imp__sub_8215B790(t, h.base);
          ITC_FLAG(native_course_scenery) = true;
          ITC_FLAG(native_race_update) = true;
        } else {
          hooks::RaceUpdateLoadScenery(t, h.base);
        }
        const uint32_t manager = guest::LoadU32(h.base, cs::kCreated);
        out[v].Word(manager != 0);
        out[v].Word(t.r3.u32 - guest::LoadU32(h.base, manager + 112));
        out[v].Word(guest::LoadU32(h.base, scene.address) - manager);
        out[v].Bytes(Read(h.base, t.r1.u32 - 160 + 80, 44));
        SceneryOutputs(h, out[v], manager, true);
        SceneryFree(h, manager, true);
      }
      results.Check(out[0], out[1],
                    fmt::format("course {} mode {} split {:#x}", course, mode, split));
    }
    memory::FreePages(data);
  }
  rex::memory::Protect(page, page_size, access);
  results.Report();
}

// --- RaceScene::RemakeCarModel: a block holding the second set ([0x826C2E44]
// pointed at it; its pool
// 7 list one scene model, in use or free, its build deferred (+5044 1 to 3)
// so SetUpB's SceneModel::SetUp only copies the record, the scene's +4 0:
// no sound loads), a car slot (its model none, that model loading (+5028
// or +5040 set) or loaded (a car with no parts: SceneModel::PollLoad 1),
// its pool index -1 or the model's, released then), a car object as
// SceneModelRecord::FromCar's cases make it (car_makers.cpp) with the
// stand-in car parameters, and the paint picks they use. Both sides call
// the callees through their adapters; the block and r3 compared, the stack
// below the call filled the same for both ---

}  // namespace

void LayFromCarWorld(uint8_t* base, uint32_t car, uint32_t params, uint32_t tables, Random& r);

namespace {

void RemakeCarModels(Context& h) {
  constexpr uint32_t kBytes = 0x10000;
  constexpr uint32_t kScene = 0x100, kModel = 0x2000, kSlot = 0x4000, kCar = 0x6000;
  constexpr uint32_t kParams = 0x7000, kTables = 0x8000;
  constexpr uint32_t kParamsPointer = 0x826C2E48;
  const Block block(kBytes);
  if (block.address == 0) {
    ITC_LOG_WARN("harness: race_update: no block for RaceScene::RemakeCarModel");
    return;
  }
  constexpr uint32_t kSceneSet = 0x826C2E44;  // the second set (SceneModel::Release's)
  const SavedBytes saved(h.base, {{kParamsPointer, 4}, {kSceneSet, 4}});
  const uint32_t b = block.address;
  Random random(0x52454D414B45ull);
  Results results("RaceScene::RemakeCarModel (sub_8215C0D8)");
  for (int c = 0; c < 200; c++) {
    const uint64_t seed = random.Next();
    Outputs out[2];
    uint32_t r3[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, b, kBytes, r);
      PPCContext t = CallContext(h);
      WriteRandom(h.base, t.r1.u32 - 4096, 4096, r);
      const uint32_t scene = b + kScene, model = b + kModel, slot = b + kSlot, car = b + kCar;
      guest::StoreU32(h.base, kSceneSet, scene);
      guest::StoreU32(h.base, scene + 4, 0);
      guest::StoreU32(h.base, scene + 4944 + 12 * 7, model);
      guest::StoreU32(h.base, scene + 4944 + 12 * 7 + 8, 1);
      guest::StoreU32(h.base, model + 8, 0);
      guest::StoreU32(h.base, model + 16, r.Below(2) == 0 ? 3 : 0);
      guest::StoreU32(h.base, model + 20, r.Below(6));
      guest::StoreU32(h.base, model + 32, 1);
      guest::StoreU32(h.base, model + 5028, r.Below(4) == 0 ? 1 : 0);
      guest::StoreU32(h.base, model + 5040, r.Below(4) == 0 ? 1 : 0);
      guest::StoreU32(h.base, model + 5044, 1 + r.Below(3));
      for (const uint32_t at : {5048u, 5060u, 5068u, 5076u, 5084u, 5100u}) {
        guest::StoreU32(h.base, model + at, 0);
      }
      guest::StoreU32(h.base, model + 5116, scene);
      guest::StoreU32(h.base, slot + 0, r.Below(4) == 0 ? 0 : model);
      guest::StoreU32(h.base, slot + 4272, r.Below(2) == 0 ? 0xFFFFFFFFu : 0);
      guest::StoreU32(h.base, kParamsPointer, b + kParams);
      LayFromCarWorld(h.base, car, b + kParams, b + kTables, r);
      const uint64_t picks[4] = {0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFEull, r.Below(20),
                                 (r.Next() << 32) | r.Below(20)};
      t.r3.u64 = b;
      t.r4.u64 = scene;
      t.r5.u64 = slot;
      t.r6.u64 = car;
      t.r7.u64 = picks[r.Below(4)];
      if (v == 0) {
        Original<__imp__sub_8215C0D8>(t, h.base);
      } else {
        hooks::RaceUpdateRemakeCarModel(t, h.base);
      }
      out[v].Bytes(Read(h.base, b, kBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      r3[v] = t.r3.u32;
    }
    results.Check(out[0], out[1], fmt::format("case {} r3 {}", c, r3[0]));
  }
  results.Report();
}


// --- Car::TickUpdate and RaceScene::Update: a world of their own in a block
// from the allocation layer, the race data in pages of its own, the game's
// globals they read pointed at them (and put back after): the race scene
// (its car slots at +16, 1 to 4 of them, each with a scene model or none and
// a pool index -1 or 0; its light set at +30064 as the Lights cases make
// it; its three screen objects at +31152 or none), up to 4 vehicles
// ([0x826C2E34]; their bodies, data and wheels' floats ordinary or special,
// the light and view flags, the gear, the nitrous byte), 1 to 4 racers
// ([0x826D82F4]: flags with 0x40 (followed), 0x2 and now and then 0x200 for
// one racer, which then remakes its car's scene model in RemakeCarModels'
// world: the second set with one pool 7 scene model, the car laid as
// FromCar's cases make it), the input records ([0x826C5B30]: the brake and
// high-beam flags), the viewports ([0x826C2CE8]: split or not, each view's
// mode 0, 1, 2 or any), the race data (the split mode 4, 6 or other, two
// humans or not, the stage's +0 1 or not, the last racer record's +3924),
// [0x826C7824] 0 or 1, the camera passes (pass 0's light points random, the
// passes Scenery::Update's table names disabled, another one, the screen
// objects', enabled or not), the post effects (1, 5 and 9 glows of the
// game's glow vtable with random floats, the rest none), and for Update a
// course scenery manager that does little (no views, empty lists, mode 0).
// Compared: the block (the scene models' blocks' matrices, wheel loads and
// gauges, the glows' settings and the screen objects' floats as floats),
// the race data's cars, the scene lights' arrays (floats) and origin, and
// r3; the stack below the call filled the same for both (the frames' dead
// temporaries not compared). The recompiled side with --native_race_update
// off. ---

struct TickWorld {
  // The block's layout.
  static constexpr uint32_t kBytes = 0x26000;
  static constexpr uint32_t kSecond = 0x100, kModel0 = 0x2000, kModels = 0x3800;
  static constexpr uint32_t kModelBytes = 0x1400;
  static constexpr uint32_t kParams = 0x7800, kTables = 0x8000;
  static constexpr uint32_t kRaceScene = 0xD000, kVehicles = 0x15000, kBodies = 0x1B400;
  static constexpr uint32_t kVehicleData = 0x1BC00, kRacers = 0x1D000, kRacerList = 0x1D200;
  static constexpr uint32_t kInputs = 0x1DC00, kViewportBlock = 0x1DD00, kViewports = 0x1DE00;
  static constexpr uint32_t kManager = 0x1EE00, kGlows = 0x1F000, kGlowBytes = 8480;
  static constexpr uint32_t kObjects = 0x25400;
  static constexpr uint32_t kDataBytes = 0xA4000;
  // The globals.
  static constexpr uint32_t kSceneSet = 0x826C2E44, kParamsPointer = 0x826C2E48;
  static constexpr uint32_t kVehicleArray = 0x826C2E34, kInputRecords = 0x826C5B30;
  static constexpr uint32_t kFollowed = 0x826C7824, kRacerManager = 0x826D82F4;
  static constexpr uint32_t kViewportsPointer = 0x826C2CE8, kEffects = 0x826C5D80;
  static constexpr uint32_t kPasses = 0x826BAC00, kPassBytes = 592, kViewMasks = 0x82602F80;
  static constexpr uint32_t kGlowVtable = 0x820088B0;
};

// A float word: ordinary mostly, else zero, denormal, infinite, NaN or any.
uint32_t SomeFloat(Random& r) {
  if (r.Below(6) == 0) return Lane(r);
  if (r.Below(40) == 0) return uint32_t(r.Next());
  const float f = (float(r.Below(20001)) - 10000.0f) / float(1 + r.Below(1000));
  uint32_t bits;
  std::memcpy(&bits, &f, 4);
  return bits;
}

// The float ranges of the block's outputs: each scene model block's
// matrices (+80 to +591), wheel loads (+612) and gauges (+696), the glows'
// settings and the screen objects' floats.
std::vector<std::pair<uint32_t, uint32_t>> TickFloatRanges() {
  using W = TickWorld;
  std::vector<std::pair<uint32_t, uint32_t>> ranges;
  auto model = [&](uint32_t m) {
    ranges.push_back({m + 4288 + 80, 128});
    ranges.push_back({m + 4288 + 612, 4});
    ranges.push_back({m + 4288 + 696, 7});
  };
  model(W::kModel0);
  for (uint32_t i = 0; i < 3; i++) model(W::kModels + W::kModelBytes * i);
  for (uint32_t i = 0; i < 3; i++) ranges.push_back({W::kGlows + W::kGlowBytes * i + 8436, 11});
  for (uint32_t i = 0; i < 3; i++) {
    ranges.push_back({W::kObjects + 128 * i + 28, 9});
    ranges.push_back({W::kObjects + 128 * i + 76, 1});
  }
  std::sort(ranges.begin(), ranges.end());
  return ranges;
}

void TickUpdates(Context& h, bool whole) {
  using W = TickWorld;
  const Block block(W::kBytes);
  const uint32_t data = memory::AllocPages(W::kDataBytes, 4096);
  if (block.address == 0 || data == 0) {
    ITC_LOG_WARN("harness: race_update: no block for Car::TickUpdate");
    if (data != 0) memory::FreePages(data);
    return;
  }
  const uint32_t b = block.address;
  // The passes Scenery::Update's table names (disabled while the cases run)
  // and one it doesn't, for ScreenObject::Start.
  std::vector<uint32_t> named;
  for (uint32_t i = 0; i < 14; i++) {
    named.push_back(guest::LoadU32(h.base, W::kViewMasks + 16 * i + 8));
  }
  uint32_t free_pass = 0xFFFFFFFFu;
  for (uint32_t p = 20; int32_t(p) >= 0 && free_pass == 0xFFFFFFFFu; p--) {
    if (std::find(named.begin(), named.end(), p) == named.end()) free_pass = p;
  }
  // None free: the objects' pass is a disabled one (Start does nothing).
  const uint32_t objects_pass = free_pass != 0xFFFFFFFFu ? free_pass : named[0];
  const uint32_t glow_vtable = W::kGlowVtable;
  const SavedBytes saved(
      h.base, {{W::kSceneSet, 8}, {W::kVehicleArray, 4}, {ru::kRaceData, 4},
               {W::kInputRecords, 32}, {W::kFollowed, 4}, {W::kRacerManager, 4},
               {W::kViewportsPointer, 4}, {W::kEffects, 60}, {W::kPasses, 21 * W::kPassBytes},
               {ru::kDirections, ru::kThird + 16 * 11 - ru::kDirections}, {ru::kOrigin, 16}});
  const auto floats = TickFloatRanges();
  Random random(whole ? 0x5550444154455343ull : 0x5449434B55504454ull);
  Results results(whole ? "RaceScene::Update (sub_8215D2B0) with Car::TickUpdate"
                        : "Car::TickUpdate (sub_8215C780)");
  const int cases = whole ? 300 : 400;
  for (int c = 0; c < cases; c++) {
    const uint64_t seed = random.Next();
    Outputs out[2];
    std::string what;
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, b, W::kBytes, r);
      WriteRandom(h.base, data, 0x6000, r);
      WriteRandom(h.base, data + 0x59600, 0x3200, r);
      PPCContext t = CallContext(h);
      WriteRandom(h.base, t.r1.u32 - 16384, 16384, r);
      const uint32_t scene = b + W::kRaceScene, slots = scene + 16;
      // The globals.
      guest::StoreU32(h.base, W::kSceneSet, b + W::kSecond);
      guest::StoreU32(h.base, W::kParamsPointer, b + W::kParams);
      guest::StoreU32(h.base, W::kVehicleArray, b + W::kVehicles);
      guest::StoreU32(h.base, ru::kRaceData, data);
      guest::StoreU32(h.base, W::kFollowed, r.Below(2));
      guest::StoreU32(h.base, W::kRacerManager, b + W::kRacerList);
      guest::StoreU32(h.base, W::kViewportsPointer, b + W::kViewportBlock);
      // The second set and its pool 7 scene model (RemakeCarModels' world).
      const uint32_t second = b + W::kSecond, model0 = b + W::kModel0;
      guest::StoreU32(h.base, second + 4, 0);
      guest::StoreU32(h.base, second + 4944 + 12 * 7, model0);
      guest::StoreU32(h.base, second + 4944 + 12 * 7 + 8, 1);
      guest::StoreU32(h.base, model0 + 8, 0);
      guest::StoreU32(h.base, model0 + 16, r.Below(2) == 0 ? 3 : 0);
      guest::StoreU32(h.base, model0 + 20, r.Below(6));
      guest::StoreU32(h.base, model0 + 5044, 1 + r.Below(3));
      guest::StoreU32(h.base, model0 + 5116, second);
      // Every scene model: a car (+32 1), loading now and then, no parts.
      uint32_t models[4] = {model0};
      for (uint32_t i = 0; i < 3; i++) models[i + 1] = b + W::kModels + W::kModelBytes * i;
      for (const uint32_t m : models) {
        guest::StoreU32(h.base, m + 32, 1);
        guest::StoreU32(h.base, m + 5028, r.Below(6) == 0 ? 1 : 0);
        guest::StoreU32(h.base, m + 5040, r.Below(6) == 0 ? 1 : 0);
        for (const uint32_t at : {5048u, 5060u, 5068u, 5076u, 5084u, 5100u}) {
          guest::StoreU32(h.base, m + at, 0);
        }
      }
      // The car slots.
      const uint32_t slot_count = 1 + r.Below(4);
      guest::StoreU32(h.base, slots, r.Below(10) == 0 ? 0 : slot_count);
      for (uint32_t s = 0; s < 4; s++) {
        const uint32_t slot = slots + 16 + 4288 * s;
        const uint32_t pick = r.Below(4);
        guest::StoreU32(h.base, slot + 0, pick == 0 ? 0 : models[r.Below(4)]);
        guest::StoreU32(h.base, slot + 4272, r.Below(2) == 0 ? 0xFFFFFFFFu : 0);
      }
      // The vehicles: their body, data and wheels.
      for (uint32_t i = 0; i < 4; i++) {
        const uint32_t vehicle = b + W::kVehicles + 6272 * i;
        for (uint32_t at = 0; at < 6272; at += 4) {
          guest::StoreU32(h.base, vehicle + at, SomeFloat(r));
        }
        guest::StoreU32(h.base, vehicle + 336, b + W::kBodies + 352 * i);
        guest::StoreU32(h.base, vehicle + 272, b + W::kVehicleData + 1280 * i);
        constexpr uint32_t kViewFlags[4] = {0x1, 0x80, 0x81, 0x0};
        guest::StoreU32(h.base, vehicle + 6148,
                        kViewFlags[r.Below(4)] | (r.Below(4) == 0 ? uint32_t(r.Next()) : 0));
        guest::StoreU32(h.base, vehicle + 2252, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(7));
        guest::StoreU8(h.base, vehicle + 2424, r.Below(2) == 0 ? 0 : uint8_t(1 + r.Below(255)));
        const float speed = float(r.Below(4000)) / 10.0f - 20.0f;
        guest::StoreFloat(h.base, vehicle + 48, speed);
        const uint32_t vdata = b + W::kVehicleData + 1280 * i;
        for (uint32_t at = 0; at < 1280; at += 4) {
          guest::StoreU32(h.base, vdata + at, SomeFloat(r));
        }
        const uint32_t body = b + W::kBodies + 352 * i;
        for (uint32_t at = 0; at < 352; at += 4) {
          guest::StoreU32(h.base, body + at, SomeFloat(r));
        }
      }
      // The input records.
      for (uint32_t i = 0; i < 8; i++) {
        const uint32_t record = b + W::kInputs + 16 * i;
        guest::StoreU32(h.base, W::kInputRecords + 4 * i, record);
        guest::StoreU32(h.base, record, (r.Below(2) == 0 ? 0x8 : 0) |
                                            (r.Below(2) == 0 ? 0x100 : 0) |
                                            (r.Below(4) == 0 ? uint32_t(r.Next()) : 0));
      }
      // The viewports.
      const uint32_t vp = b + W::kViewportBlock;
      guest::StoreU32(h.base, vp + 0, b + W::kViewports);
      guest::StoreU32(h.base, vp + 4, b + W::kViewports + 1872);
      guest::StoreU32(h.base, vp + 8, r.Below(5) == 0 ? 1 : 0);
      for (uint32_t i = 0; i < 2; i++) {
        const uint32_t viewport = b + W::kViewports + 1872 * i;
        guest::StoreU32(h.base, viewport + 1860,
                        r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(3));
        guest::StoreU32(h.base, viewport + 72, SomeFloat(r));
      }
      // The race data: the split mode, the humans, the stage, the records.
      constexpr uint32_t kModes[4] = {4, 6, 0, 3};
      const uint32_t mode = kModes[r.Below(4)];
      guest::StoreU32(h.base, data + 0xA3AF0, mode);
      guest::StoreU32(h.base, data + 0x59614, r.Below(2) == 0 ? 2 : r.Below(4));
      const uint32_t stage = r.Below(10);
      guest::StoreU32(h.base, data + 0x59620 + 804, stage);
      guest::StoreU32(h.base, data + 0x59620 + 80 * stage, r.Below(2) == 0 ? 1 : r.Below(4));
      const uint32_t records = 1 + r.Below(3);
      guest::StoreU32(h.base, data + 0x59AE8 + 0x49FE0, records);
      guest::StoreU32(h.base, data + 0x59AE8 + 3936 * (records - 1) + 3924, r.Below(3));
      // The racers: one now and then remaking its car's model.
      const uint32_t racers = 1 + r.Below(4);
      const uint32_t list = b + W::kRacerList;
      guest::StoreU32(h.base, list + 2364, racers);
      const uint32_t remaker = r.Below(3) == 0 ? r.Below(racers) : 0xFFFFFFFFu;
      for (uint32_t j = 0; j < 4; j++) {
        const uint32_t racer = b + W::kRacers + 64 * j;
        guest::StoreU32(h.base, list + 2308 + 4 * j, racer);
        const uint32_t vehicle_index = r.Below(4) == 0 ? r.Below(4) : j;
        uint32_t flags = uint32_t(r.Next()) & ~0x200u;
        if (r.Below(4) != 0) flags |= 0x40;
        if (j == remaker && vehicle_index != 0) flags |= 0x200;
        guest::StoreU32(h.base, racer + 0, flags);
        guest::StoreU32(h.base, racer + 16, uint32_t(Argument(r)));
        guest::StoreU32(h.base, racer + 20, vehicle_index);
        const uint32_t car_index = r.Below(6);
        guest::StoreU32(h.base, racer + 24, car_index);
        if ((flags & 0x200) != 0) {
          LayFromCarWorld(h.base, data + 48 + 3936 * car_index, b + W::kParams, b + W::kTables, r);
        }
      }
      // The camera passes: pass 0's points, the named ones disabled, the
      // screen objects' enabled or not.
      for (uint32_t at = 400; at < 432; at += 4) {
        guest::StoreU32(h.base, W::kPasses + at, SomeFloat(r));
      }
      for (const uint32_t p : named) {
        guest::StoreU32(h.base, W::kPasses + W::kPassBytes * p + 588, 0);
      }
      if (free_pass != 0xFFFFFFFFu) {
        guest::StoreU32(h.base, W::kPasses + W::kPassBytes * free_pass + 588, r.Below(3) == 0 ? 0 : 1);
      }
      // The post effects: three glows, the rest none.
      for (uint32_t i = 0; i < 15; i++) guest::StoreU32(h.base, W::kEffects + 4 * i, 0);
      for (uint32_t i = 0; i < 3; i++) {
        const uint32_t glow = b + W::kGlows + W::kGlowBytes * i;
        for (uint32_t at = 4; at < W::kGlowBytes; at += 4) {
          guest::StoreU32(h.base, glow + at, SomeFloat(r));
        }
        guest::StoreU32(h.base, glow, glow_vtable);
        guest::StoreU32(h.base, W::kEffects + 4 * (1 + 4 * i), glow);
      }
      // The race scene's manager (Update's cases), light set and screen
      // objects.
      const uint32_t manager = b + W::kManager;
      guest::StoreU32(h.base, scene + 0, manager);
      guest::StoreU32(h.base, manager + 8, 0);
      guest::StoreU32(h.base, manager + 12, 0xFFFFFFFFu);
      for (const uint32_t l : {116u, 144u}) {
        guest::StoreU32(h.base, manager + l + 4, 0);
        guest::StoreU32(h.base, manager + l + 8, 0);
        guest::StoreU32(h.base, manager + l + 12, 0);
      }
      for (uint32_t i = 0; i < 11; i++) {
        const uint32_t entry = scene + 30064 + 16 + 96 * i;
        guest::StoreU32(h.base, entry, i < 3 ? 1 : r.Below(3));
        for (uint32_t k = 0; k < 4; k++) guest::StoreU32(h.base, entry + 16 + 4 * k, Lane(r));
      }
      for (uint32_t i = 0; i < 3; i++) {
        const uint32_t object = b + W::kObjects + 128 * i;
        guest::StoreU32(h.base, scene + 31152 + 4 * i, r.Below(6) == 0 ? 0 : object);
        guest::StoreU8(h.base, object + 24, r.Below(2));
        guest::StoreU8(h.base, object + 26, r.Below(2));
        guest::StoreU32(h.base, object + 80, objects_pass);
        guest::StoreU32(h.base, object + 88, b + W::kGlows + W::kGlowBytes * r.Below(3));
        guest::StoreU32(h.base, object + 92, 0);
      }
      what = fmt::format("case {} mode {} racers {} slots {} remaker {}", c, mode, racers,
                         guest::LoadU32(h.base, slots), int32_t(remaker));
      if (whole) {
        t.r3.u64 = scene;
        (v == 0 ? Original<__imp__sub_8215D2B0> : hooks::RaceUpdateUpdate)(t, h.base);
      } else {
        t.r3.u64 = slots;
        (v == 0 ? Original<__imp__sub_8215C780> : hooks::RaceUpdateTickUpdate)(t, h.base);
      }
      // The block in order, its float ranges as floats.
      uint32_t at = 0;
      for (const auto& [start, count] : floats) {
        out[v].Bytes(Read(h.base, b + at, start - at));
        out[v].Floats(h.base, b + start, count);
        at = start + 4 * count;
      }
      out[v].Bytes(Read(h.base, b + at, W::kBytes - at));
      out[v].Bytes(Read(h.base, data, 0x6000));
      out[v].Floats(h.base, ru::kDirections, (ru::kThird + 16 * 11 - ru::kDirections) / 4);
      out[v].Bytes(Read(h.base, ru::kOrigin, 16));
      for (uint32_t i = 0; i < 15; i++) out[v].Word(guest::LoadU32(h.base, W::kEffects + 4 * i));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r1.u32);
    }
    results.Check(out[0], out[1], what);
  }
  results.Report();
  memory::FreePages(data);
}

}  // namespace

void RaceUpdate(Context& h) {
  VaryCars(h);
  Lights(h);
  LoadSceneryCases(h);
  RemakeCarModels(h);
  TickUpdates(h, false);
  TickUpdates(h, true);
}

}  // namespace harness
