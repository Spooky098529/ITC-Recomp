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

}  // namespace

void RaceUpdate(Context& h) {
  VaryCars(h);
  Lights(h);
  LoadSceneryCases(h);
}

}  // namespace harness
