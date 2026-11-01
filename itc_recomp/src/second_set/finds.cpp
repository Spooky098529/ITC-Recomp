// The parts' and the cockpit's point set-ups (see finds.h), call for call as
// the recompiled functions.

#include "second_set/finds.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "material_passes/material_passes.h"
#include "matrix_math/matrix_math.h"
#include "native/buffers.h"
#include "scene_loaders/model_point.h"
#include "second_set/points.h"

#include <cstdlib>

namespace second_set {

namespace {

using car_physics::ops::CopyWord;
using car_physics::ops::kOne;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kFive = 0x82005FE4;  // 5.0
constexpr uint32_t R = kPointRecordBytes;

// ModelPoint::Find from a frame at `sp`.
uint32_t NodeFind(uint8_t* base, uint32_t record, uint32_t model, uint32_t sp, FindCalls& calls) {
  const uint32_t r3 = scene_loaders::ModelPointFind(
      base, record, model, sp, [&](uint32_t skeleton) {
        calls.update_matrices(skeleton, sp - scene_loaders::kModelPointFrameBytes);
      });
  calls.flush = r3 == 1;
  return r3;
}

// PartModel::ScaleBlended(record +4, record +228, the float at `factor`).
uint32_t Scale(uint8_t* base, uint32_t record, uint32_t factor, uint32_t sp, FindCalls& calls) {
  const double f = double(guest::LoadFloat(base, factor));
  const uint32_t r3 = material_passes::ScaleBlended(base, Load(base, record + 4),
                                                    Load(base, record + 228), f, sp);
  calls.flush = false;
  return r3;
}

// Records [from, to) of `group`: ModelPoint::Find below `nodes`, FindShape
// from there.
uint32_t FindRecords(uint8_t* base, uint32_t group, uint32_t model, uint32_t from, uint32_t to,
                     uint32_t nodes, uint32_t sp, FindCalls& calls) {
  uint32_t r3 = 0;
  for (uint32_t i = from; i < to; i++) {
    r3 = i < nodes ? NodeFind(base, group + R * i, model, sp, calls)
                   : ModelPointFindShape(base, group + R * i, model, sp);
  }
  return r3;
}

// Records [from, to] scaled by the float at `factor`.
uint32_t ScaleRecords(uint8_t* base, uint32_t group, uint32_t from, uint32_t to, uint32_t factor,
                      uint32_t sp, FindCalls& calls) {
  uint32_t r3 = 0;
  for (uint32_t i = from; i <= to; i++) {
    r3 = Scale(base, group + R * i, factor, sp, calls);
  }
  return r3;
}

}  // namespace

uint32_t PartsFindFrontBumper(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                              FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DE140 PartsPoints::FindFrontBumper");
  const uint32_t sp = stack - 128;
  FindRecords(base, group, model, 0, 11, 0, sp, calls);
  const uint32_t r3 = ScaleRecords(base, group, 3, 5, kFive, sp, calls);
  const uint32_t found = Load(base, group + 8 * R + 4);
  if (found != 0) {
    Store(base, found + 456, Load(base, group + 2644));
  }
  return r3;
}

uint32_t PartsFindRearBumper(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                             FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DE328 PartsPoints::FindRearBumper");
  const uint32_t sp = stack - 128;
  FindRecords(base, group, model, 0, 7, 0, sp, calls);
  Scale(base, group + R, kOne, sp, calls);
  const uint32_t r3 = ScaleRecords(base, group, 2, 4, kFive, sp, calls);
  const uint32_t found = Load(base, group + 6 * R + 4);
  if (found != 0) {
    Store(base, found + 456, Load(base, group + 1684));
  }
  return r3;
}

uint32_t PartsFindRearSpoiler(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                              FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DE4E8 PartsPoints::FindRearSpoiler");
  const uint32_t sp = stack - 128;
  FindRecords(base, group, model, 0, 3, 0, sp, calls);
  Scale(base, group + 2 * R, kFive, sp, calls);
  return Scale(base, group + R, kFive, sp, calls);
}

uint32_t PartsFindBonnet(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                         FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DE5B0 PartsPoints::FindBonnet");
  return FindRecords(base, group, model, 0, 2, 1, stack - 112, calls);
}

uint32_t PartsFindLeftDoor(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                           FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DE668 PartsPoints::FindLeftDoor");
  const uint32_t sp = stack - 128;
  FindRecords(base, group, model, 0, 25, 2, sp, calls);
  return ScaleRecords(base, group, 7, 10, kFive, sp, calls);
}

uint32_t PartsFindRightDoor(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                            FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DE878 PartsPoints::FindRightDoor");
  return FindRecords(base, group, model, 0, 25, 2, stack - 112, calls);
}

uint32_t PartsFindDoors(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                        FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DEA20 PartsPoints::FindDoors");
  const uint32_t sp = stack - 128;
  FindRecords(base, group, model, 0, 23, 0, sp, calls);
  return ScaleRecords(base, group, 3, 10, kFive, sp, calls);
}

uint32_t PartsFindHeadlights(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                             FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DF7F0 PartsPoints::FindHeadlights");
  const uint32_t sp = stack - 128;
  const uint32_t r3 = FindRecords(base, group, model, 0, 13, 7, sp, calls);
  if (Load(base, group + 4) == 0) {
    return r3;
  }
  const uint32_t entries = Load(base, Load(base, group + 0) + 136);
  CopyWord(base, Load(base, group + 224) * 144 + entries + 16, group + 3184);
  calls.flush = false;
  return ScaleRecords(base, group, 7, 10, kFive, sp, calls);
}

uint32_t PartsFindTailLights(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                             FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DEC58 PartsPoints::FindTailLights");
  const uint32_t sp = stack - 128;
  FindRecords(base, group, model, 0, 15, 8, sp, calls);
  return ScaleRecords(base, group, 8, 12, kFive, sp, calls);
}

uint32_t PartsFindFenders(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                          FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DED20 PartsPoints::FindFenders");
  const uint32_t sp = stack - 128;
  FindRecords(base, group, model, 0, 7, 0, sp, calls);
  return ScaleRecords(base, group, 3, 4, kFive, sp, calls);
}

uint32_t PartsFindCabin(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                        FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DEED8 PartsPoints::FindCabin");
  const uint32_t sp = stack - 128;
  FindRecords(base, group, model, 0, 5, 0, sp, calls);
  return ScaleRecords(base, group, 0, 1, kFive, sp, calls);
}

uint32_t PartsFindMufflers(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                           FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E0080 PartsPoints::FindMufflers");
  const uint32_t sp = stack - 192;
  const uint32_t records = group + 256;
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t record = records + R * i;
    NodeFind(base, record, model, sp, calls);
    if (!native::Buffers_Memcpy(base, sp + 80, record + 16, 64, 0x820E00C0)) {
      ITC_LOG_ERROR("Second set: FindMufflers' memcpy refused after its checks");
      std::abort();
    }
    const uint32_t matrix = group + 64 * i;
    matrix_math::Copy(base, matrix, sp + 80);
    interpolation::BlendState_OnCopy(base, matrix, sp + 80, 0x820E00CC);
    calls.flush = false;
  }
  return ModelPointFindShape(base, records + 4 * R, model, sp);
}

uint32_t PartsFind(uint8_t* base, uint32_t block, uint32_t models, uint32_t stack,
                   FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E0258 PartsPoints::Find");
  const uint32_t sp = stack - 176;
  const uint32_t scene_word = block + 37088;
  const uint32_t w37076 = Load(base, block + 37076);
  const uint32_t car_loads = Load(base, block + 37084);
  const uint32_t scene = Load(base, scene_word);
  Store(base, block + 2648, w37076);
  Store(base, block + 2644, Load(base, Load(base, car_loads + 24) + 6700));
  Store(base, block + 4580, Load(base, Load(base, car_loads + 24) + 6708));
  if (Load(base, scene + 4) == 0) {
    Store(base, block + 12112, w37076);
    Store(base, block + 18208, w37076);
    const uint32_t w37080 = Load(base, block + 37080);
    const uint32_t w37092 = Load(base, block + 37092);
    Store(base, block + 12116, w37080);
    Store(base, block + 12032, w37092);
    Store(base, block + 18212, w37080);
    Store(base, block + 18128, w37092);
  } else {
    const uint32_t w37080 = Load(base, block + 37080);
    Store(base, block + 23744, w37080);
    Store(base, block + 37056, w37080);
  }
  uint32_t r3 = block;
  auto shapes = [&](uint32_t at, uint32_t count, uint32_t model) {
    for (uint32_t i = 0; i < count; i++) {
      r3 = ModelPointFindShape(base, block + at + R * i, model, sp);
    }
  };
  for (uint32_t kind = 0; kind < 17; kind++) {
    const uint32_t model = Load(base, models + 4 * kind);
    if (model == 0) {
      continue;
    }
    const bool set_a = Load(base, Load(base, scene_word) + 4) == 0;
    switch (kind) {
      case 0: r3 = PartsFindFrontBumper(base, block, model, sp, calls); break;
      case 1: shapes(2656, 1, model); break;
      case 2: r3 = PartsFindRearBumper(base, block + 2896, model, sp, calls); break;
      case 3: shapes(4592, 1, model); break;
      case 4: r3 = PartsFindRearSpoiler(base, block + 4832, model, sp, calls); break;
      case 5: r3 = PartsFindBonnet(base, block + 5552, model, sp, calls); break;
      case 6:
        r3 = set_a ? PartsFindLeftDoor(base, block + 6032, model, sp, calls)
                   : PartsFindDoors(base, block + 18224, model, sp, calls);
        break;
      case 7:
        if (set_a) {
          r3 = PartsFindRightDoor(base, block + 12128, model, sp, calls);
        }
        break;
      case 8: shapes(set_a ? 23760 : 24240, 1, model); break;
      case 9:
        if (set_a) {
          shapes(24000, 1, model);
        }
        break;
      case 10: r3 = PartsFindHeadlights(base, block + 24480, model, sp, calls); break;
      case 11: r3 = PartsFindTailLights(base, block + 27680, model, sp, calls); break;
      case 12: r3 = PartsFindFenders(base, block + 31280, model, sp, calls); break;
      case 13: r3 = PartsFindCabin(base, block + 32960, model, sp, calls); break;
      case 14: shapes(34160, 2, model); break;
      case 15: r3 = PartsFindMufflers(base, block + 34640, model, sp, calls); break;
      case 16:
        if (!set_a) {
          shapes(36096, 4, model);
        }
        break;
      default: break;
    }
  }
  return r3;
}

uint32_t CockpitFindHeadlights(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                               FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DBB08 CockpitPoints::FindHeadlights");
  const uint32_t sp = stack - 128;
  FindRecords(base, group, model, 0, 8, 4, sp, calls);
  return ScaleRecords(base, group, 5, 6, kOne, sp, calls);
}

uint32_t CockpitFind(uint8_t* base, uint32_t block, uint32_t models, uint32_t stack,
                     FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DBC88 CockpitPoints::Find");
  const uint32_t sp = stack - 128;
  auto shapes = [&](uint32_t at, uint32_t count, uint32_t model) {
    for (uint32_t i = 0; i < count; i++) {
      ModelPointFindShape(base, block + at + R * i, model, sp);
    }
  };
  shapes(0, 3, Load(base, models + 16));
  shapes(720, 2, Load(base, models + 20));
  ModelPointFindShape(base, block + 1200, Load(base, models + 0), sp);
  shapes(1440, 2, Load(base, models + 4));
  ModelPointFindShape(base, block + 1920, Load(base, models + 8), sp);
  return CockpitFindHeadlights(base, block + 2160, Load(base, models + 12), sp, calls);
}

}  // namespace second_set
