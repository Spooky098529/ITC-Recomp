// The wheels' point set-up (see wheels.h), load for load and store for
// store as the recompiled functions.

#include "second_set/wheels.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "graphics/resources.h"
#include "guest.h"
#include "material_passes/material_passes.h"
#include "native/buffers.h"
#include "scene/scene.h"
#include "scene_loaders/pools.h"
#include "second_set/morph.h"
#include "second_set/points.h"

namespace second_set {

namespace {

using namespace camera_modes::steps;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
int32_t LoadS(uint8_t* base, uint32_t address) { return int32_t(Load(base, address)); }

constexpr uint32_t R = kPointRecordBytes;
constexpr uint32_t kTireTable = 0x82003EF8;   // 24 bytes an entry
constexpr uint32_t kIndexTable = 0x820044B0;  // 44 bytes an entry
constexpr uint32_t kOne = 0x82000A30;         // 1.0
constexpr uint32_t kNinth = 0x82006634;       // 1/9
constexpr uint32_t kMatrixOffset = 6976;      // the car model's loading object's matrix

void Left(WheelCalls& calls, Flush flush) {
  if (flush != Flush::kUnchanged) {
    calls.flush = flush;
  }
}

// 32 bytes of the matrix at [group + 0] (its first half, or its second) to
// `to`, the matrix's word read again for the second 16.
void Rows(uint8_t* base, uint32_t group, uint32_t to, bool second) {
  const uint32_t from = second ? 32 : 0;
  uint32_t m = Load(base, group + 0);
  Store64(base, to + 0, Load64(base, m + from + 0));
  Store64(base, to + 8, Load64(base, m + from + 8));
  m = Load(base, group + 0);
  Store64(base, to + 16, Load64(base, m + from + 16));
  Store64(base, to + 24, Load64(base, m + from + 24));
}

void Run(uint8_t* base, uint32_t model, uint32_t record, uint32_t out1, uint32_t out2,
         uint32_t sp, uint32_t lr, WheelCalls& calls) {
  if (const auto result = MorphRun(base, record, out1, out2, sp)) {
    Left(calls, result->flush);
    return;
  }
  Left(calls, calls.morph_run(model, record, out1, out2, sp, lr) ? Flush::kOn : Flush::kOff);
}

void Copy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp, uint32_t lr,
          WheelCalls& calls) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    calls.memcpy(to, from, bytes, sp, lr);
  }
}

// For the renderer (docs/conventional-renderer.md, "Step 2b"): the morph
// copied over shape `shape`'s streams, so its buffers (the headers
// Mesh::Parse relocated at +288, +320 and +352) are written.
void ShapeBuffersWritten(uint8_t* base, uint32_t model, uint32_t shape) {
  constexpr uint32_t kShapeBytes = 544;
  const uint32_t at = Load(base, model + 40) + kShapeBytes * shape;
  for (uint32_t field : {288u, 320u, 352u}) {
    graphics::BufferWritten(base, at + field);
  }
}

uint64_t Free(uint8_t* base, uint32_t at, uint32_t sp, uint32_t lr, WheelCalls& calls) {
  if (const auto r3 = scene::FreePagesIn(base, at)) {
    return *r3;
  }
  return calls.free_pages_in(at, sp, lr);
}

// The return addresses of one morph's calls, from its first (StreamSize).
struct Sites {
  uint32_t run, copy1, copy2, free1, free2;
};
constexpr Sites kRimSites{0x6C, 0x84, 0x9C, 0xA4, 0xB0};
constexpr Sites kBrakeSites{0x64, 0x7C, 0x94, 0x9C, 0xA8};

// The rims' and brakes' frame words: the target's index, the weight, the
// morph record.
struct MorphFrame {
  uint32_t weight, record;
};
constexpr MorphFrame kRimFrame{128, 160};
constexpr MorphFrame kBrakeFrame{144, 176};

// One of the rims' or brakes' morphs: record 2k's shape toward record 2k +
// 1's (one target, its index at the frame's +96) into the pages at the
// frame's +80 and +84, copied over the shape's streams (the second by the
// first stream's size where `second_by_first`), the pages freed (+80 then 0).
// `weight_from` the float the rims copy to the frame's weight each time (0
// for the brakes'). Returns the last free's r3.
uint64_t MorphPair(uint8_t* base, uint32_t model, uint32_t group, uint32_t k, uint32_t sp,
                   const MorphFrame& frame, uint32_t site, const Sites& sites,
                   uint32_t weight_from, bool second_by_first, WheelCalls& calls) {
  const uint32_t shape_at = group + 260 + 2 * R * k;
  const uint32_t target_at = group + 500 + 2 * R * k;
  const uint32_t size1 = ModelStreamSize(base, model, Load(base, shape_at));
  const uint32_t size2 = ModelStreamSize2(base, model, Load(base, shape_at));
  scene_loaders::AllocPages(base, sp + 80, size1);
  scene_loaders::AllocPages(base, sp + 84, size2);
  const uint32_t index = Load(base, target_at);
  if (weight_from != 0) {
    Stfs(base, sp + frame.weight, Lfs(base, weight_from));
    calls.flush = Flush::kOff;
  }
  Store(base, sp + 96, index);
  Left(calls, MorphSetUp(base, model, sp + frame.record, Load(base, shape_at), sp + 96,
                         sp + frame.weight, 1));
  Run(base, model, sp + frame.record, Load(base, sp + 80), Load(base, sp + 84), sp,
      site + sites.run, calls);
  Copy(base, ModelStreamData(base, model, Load(base, shape_at)), Load(base, sp + 80), size1, sp,
       site + sites.copy1, calls);
  Copy(base, ModelStreamData2(base, model, Load(base, shape_at)), Load(base, sp + 84),
       second_by_first ? size1 : size2, sp, site + sites.copy2, calls);
  ShapeBuffersWritten(base, model, Load(base, shape_at));
  Free(base, sp + 80, sp, site + sites.free1, calls);
  Store(base, sp + 80, 0);
  return Free(base, sp + 84, sp, site + sites.free2, calls);
}

// FindShape on `count` records from `group + first`; the last one's r3.
uint32_t FindShapes(uint8_t* base, uint32_t group, uint32_t first, uint32_t count,
                    uint32_t model, uint32_t sp) {
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < count; i++) {
    r3 = ModelPointFindShape(base, group + first + R * i, model, sp);
  }
  return r3;
}

uint64_t SetUpRim(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                  WheelCalls& calls, bool rim_a) {
  const uint32_t sp = stack - 336;
  Store(base, sp, stack);
  uint64_t r3 = FindShapes(base, group, 32, 7, model, sp);
  Rows(base, group, group + 1712, LoadS(base, group + 12) == 0);
  if (LoadS(base, Load(base, group + 8) + 8) != 0) {
    return r3;
  }
  Store(base, sp + 80, 0);
  Store(base, sp + 84, 0);
  if (rim_a) {
    if (Load(base, group + 36) != 0) {
      MorphPair(base, model, group, 0, sp, kRimFrame, 0x820E7A8C, kRimSites, group + 16, false,
                calls);
      Store(base, sp + 84, 0);
    }
    // The second morph's second copy by the first stream's size, as the
    // original does.
    return MorphPair(base, model, group, 1, sp, kRimFrame, 0x820E7B4C, kRimSites, group + 16,
                     true, calls);
  }
  static constexpr uint32_t kSites[] = {0x820E7D58, 0x820E7E18, 0x820E7ED8};
  for (uint32_t k = 0; k < 3; k++) {
    r3 = MorphPair(base, model, group, k, sp, kRimFrame, kSites[k], kRimSites, group + 16, false,
                   calls);
    if (k != 2) {
      Store(base, sp + 84, 0);
    }
  }
  return r3;
}

// The tires' and brakes' +4, +0 and +8 from the block: the car model's
// loading object's words when [+0] is set, else +4 and +8; +8 = +12.
void GroupWords(uint8_t* base, uint32_t block, uint32_t front, uint32_t rear) {
  const uint32_t loading = Load(base, block + 0);
  if (loading != 0) {
    Store(base, front + 4, Load(base, loading + 24) + 32);
    Store(base, front + 0, Load(base, loading + 24) + kMatrixOffset);
    Store(base, rear + 4, Load(base, loading + 24) + 32);
    Store(base, rear + 0, Load(base, loading + 24) + kMatrixOffset);
  } else {
    const uint32_t a = Load(base, block + 4);
    const uint32_t b = Load(base, block + 8);
    Store(base, front + 4, a);
    Store(base, front + 0, b);
    Store(base, rear + 4, a);
    Store(base, rear + 0, b);
  }
}

}  // namespace

uint64_t WheelsSetUpTire(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                         WheelCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E7598 WheelPoints::SetUpTire");
  const uint32_t sp = stack - 288;
  Store(base, sp, stack);
  FindShapes(base, group, 368, 6, model, sp);
  for (uint32_t k = 0; k < 5; k++) {
    Store(base, group + 304 + 4 * k, Load(base, group + 836 + R * k));
  }
  const bool front = LoadS(base, group + 300) != 0;
  uint32_t wheels = Load(base, group + 8);
  // The record's index, value and float, or the table's by the car (+1080
  // on for traffic, kind 3).
  const uint32_t own = front ? 44 : 56;
  uint32_t index, value;
  double f1;
  if (LoadS(base, wheels + own) != -1) {
    wheels = Load(base, group + 8);
    value = wheels + own + 4;
    f1 = Lfs(base, wheels + own + 8);
    index = Load(base, wheels + own);
  } else {
    const uint32_t car = Load(base, Load(base, group + 4) + 0) * 24;
    const uint32_t at = kTireTable + (LoadS(base, wheels + 0) == 3 ? 1080 : 0) + (front ? 0 : 12);
    index = Load(base, car + at);
    value = car + at + 4;
    f1 = Lfs(base, car + at + 8);
  }
  TireFillEntries(base, group + 12, index, value, f1, sp);
  Rows(base, group, group + 1808, !front);
  // sub_820E7010's index: the record's, else the table's (+1980 for
  // traffic, else plus the record's +72 or +76).
  wheels = Load(base, group + 8);
  uint64_t read;
  const int32_t flag = LoadS(base, group + 300);
  if (LoadS(base, wheels + 0) == 3) {
    if (flag != 0 && LoadS(base, wheels + 44) != -1) {
      read = Load(base, wheels + 44);
    } else if (flag == 0 && LoadS(base, wheels + 56) != -1) {
      read = Load(base, wheels + 56);
    } else {
      read = Load(base, Load(base, Load(base, group + 4) + 0) * 44 + kIndexTable + 1980);
    }
  } else {
    const uint32_t mine = flag != 0 ? 44 : 56;
    if (LoadS(base, wheels + mine) != -1) {
      read = Load(base, wheels + mine);
    } else {
      const uint32_t add = Load(base, wheels + (flag != 0 ? 72 : 76));
      read = uint64_t(Load(base, Load(base, Load(base, group + 4) + 0) * 44 + kIndexTable)) +
             uint64_t(add);
    }
  }
  TireReadEntry(base, uint32_t(read), group + 12, group + 328, group + 352, group + 356, sp);
  calls.flush = Flush::kOff;
  if (LoadS(base, Load(base, group + 8) + 8) != 0) {
    return read;
  }
  Store(base, sp + 84, 0);
  Store(base, sp + 80, 0);
  const uint32_t size1 = ModelStreamSize(base, model, Load(base, group + 596));
  const uint32_t size2 = ModelStreamSize2(base, model, Load(base, group + 596));
  scene_loaders::AllocPages(base, sp + 84, size1);
  scene_loaders::AllocPages(base, sp + 80, size2);
  Left(calls, MorphSetUp(base, model, sp + 96, Load(base, group + 596), group + 304, group + 328,
                         5));
  Run(base, model, sp + 96, Load(base, sp + 84), Load(base, sp + 80), sp, 0x820E78D0, calls);
  Copy(base, ModelStreamData(base, model, Load(base, group + 596)), Load(base, sp + 84), size1,
       sp, 0x820E78E8, calls);
  Copy(base, ModelStreamData2(base, model, Load(base, group + 596)), Load(base, sp + 80), size2,
       sp, 0x820E7900, calls);
  ShapeBuffersWritten(base, model, Load(base, group + 596));
  Free(base, sp + 84, sp, 0x820E7908, calls);
  return Free(base, sp + 80, sp, 0x820E7910, calls);
}

uint64_t WheelsSetUpRimA(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                         WheelCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E79C8 WheelPoints::SetUpRimA");
  return SetUpRim(base, group, model, stack, calls, true);
}

uint64_t WheelsSetUpRimB(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                         WheelCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E7CA0 WheelPoints::SetUpRimB");
  return SetUpRim(base, group, model, stack, calls, false);
}

uint64_t WheelsSetUpBrakes(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                           WheelCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E8028 WheelPoints::SetUpBrakes");
  const uint32_t sp = stack - 368;
  Store(base, sp, stack);
  FindShapes(base, group, 32, 12, model, sp);
  const double one = Lfs(base, kOne);
  material_passes::ScaleBlended(base, Load(base, group + 2436), Load(base, group + 2660), one, sp);
  material_passes::ScaleBlended(base, Load(base, group + 2676), Load(base, group + 2900), one, sp);
  uint32_t count;
  if (LoadS(base, group + 12) != 0) {
    Rows(base, group, group + 2912, false);
    count = Load(base, Load(base, group + 8) + 108);
  } else {
    Rows(base, group, group + 2912, true);
    count = Load(base, Load(base, group + 8) + 112);
  }
  uint64_t r3 = material_passes::SetTexture(base, model, Load(base, Load(base, group + 8) + 88),
                                            -1, sp);
  // The original's epilogue turns the flush mode off either way.
  calls.flush = Flush::kOff;
  const uint32_t wheels = Load(base, group + 8);
  if (LoadS(base, wheels + 8) != 0) {
    return r3;
  }
  Store(base, sp + 80, 0);
  Store(base, sp + 84, 0);
  const int32_t kind = LoadS(base, wheels + 68);
  if (kind >= 3 && kind < 6) {
    count += 1;
  }
  if (kind == 6) {
    count += 2;
  }
  const int64_t steps = int64_t(int32_t(count - 9));
  Store64(base, sp + 128, uint64_t(steps));
  const double weight = Fmuls(Single(double(steps)), Lfs(base, kNinth));
  Stfs(base, sp + 144, weight);
  Stfs(base, group + 16, weight);
  for (uint32_t k = 0; k < 6; k++) {
    r3 = MorphPair(base, model, group, k, sp, kBrakeFrame, 0x820E817C + 0xB8 * k, kBrakeSites, 0,
                   false, calls);
    if (k != 5) {
      Store(base, sp + 84, 0);
    }
  }
  calls.flush = Flush::kOff;
  return r3;
}

uint64_t WheelPointsSetUp(uint8_t* base, uint32_t block, uint32_t stack, WheelCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E8668 WheelPoints::SetUp");
  const uint32_t sp = stack - 112;
  Store(base, sp, stack);
  GroupWords(base, block, block + 32, block + 1872);
  Store(base, block + 40, Load(base, block + 12));
  WheelsSetUpTire(base, block + 32, Load(base, block + 16), sp, calls);
  Store(base, block + 1880, Load(base, block + 12));
  WheelsSetUpTire(base, block + 1872, Load(base, block + 16) + 464, sp, calls);
  const double f0 = Lfs(base, block + 384);
  const double f13 = Lfs(base, block + 388);
  const double f12 = Lfs(base, block + 2224);
  const double f11 = Lfs(base, block + 2228);
  calls.flush = Flush::kOff;
  const bool set_a = LoadS(base, Load(base, scene::kScene) + 4) == 0;
  const uint32_t loading = Load(base, block + 0);
  const uint32_t front = block + (set_a ? 3712 : 7200);
  const uint32_t rear = block + (set_a ? 5456 : 8944);
  Stfs(base, front + 16, f0);
  Stfs(base, front + 20, f13);
  Stfs(base, rear + 16, f12);
  Stfs(base, rear + 20, f11);
  uint32_t matrix;
  if (loading != 0) {
    Store(base, front + 0, Load(base, loading + 24) + kMatrixOffset);
    matrix = Load(base, loading + 24) + kMatrixOffset;
  } else {
    matrix = Load(base, block + 8);
    Store(base, front + 0, matrix);
  }
  Store(base, rear + 0, matrix);
  auto rim = set_a ? WheelsSetUpRimA : WheelsSetUpRimB;
  Store(base, front + 8, Load(base, block + 12));
  rim(base, front, Load(base, block + 20), sp, calls);
  Store(base, rear + 8, Load(base, block + 12));
  rim(base, rear, Load(base, block + 20) + 464, sp, calls);
  GroupWords(base, block, block + 10688, block + 13632);
  Store(base, block + 10696, Load(base, block + 12));
  WheelsSetUpBrakes(base, block + 10688, Load(base, block + 24), sp, calls);
  Store(base, block + 13640, Load(base, block + 12));
  return WheelsSetUpBrakes(base, block + 13632, Load(base, block + 24) + 464, sp, calls);
}

}  // namespace second_set
