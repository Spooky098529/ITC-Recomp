// The light kit's draw as plain C++ (docs/functions/car-draw.md:
// SceneLights::Draw, LightPoints::DrawFront, DrawSides, DrawRear and their
// neons); see car_draw.h. The LEDs and spots (never seen in play) are guest
// calls from the original's frames. Built with -ffp-model=strict
// (CMakeLists.txt): the floats are stored as the translated code's single
// to double to single.

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "material_passes/material_passes.h"
#include "matrix_math/matrix_math.h"
#include "native/buffers.h"
#include "second_set/objects.h"

namespace car_draw {

namespace {

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kCullSwap = 0x826C5104;
constexpr uint32_t kOne = 0x82000A30;           // 1.0
constexpr uint32_t kFrontEndY = 0x820029D8;     // the copy's world y in the front end
constexpr uint32_t kCarPartDraw = 0x820D06F0;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// memcpy (sub_825A7410) from the frame at `sp` returning to `lr`: natively,
// else its original. r3 the destination either way.
void Memcpy(uint8_t* base, const Calls& calls, uint32_t to, uint32_t from, uint32_t bytes,
            uint32_t sp, uint32_t lr) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    calls.memcpy(to, from, bytes, sp, lr);
  }
}

// Each group's offsets and its calls' return addresses.
struct GroupSites {
  uint32_t mode;      // the record's mode
  uint32_t fraction;  // the record's on fraction (then the fade and the rate)
  uint32_t last;      // the spots' last mode + 1
  uint32_t leds, spots;
  uint32_t spots_copy, spots_call, leds_copy, leds_call, neon_copy;
  uint32_t neon_draw;  // the neon's CarPart::Draw call's return address
};
constexpr GroupSites kSites[3] = {
    {4, 8, 10, 0x820DCC70, 0x820DCF98, 0x820DDB34, 0x820DDB48, 0x820DDB58, 0x820DDB6C,
     0x820DDB7C, 0x820DCC64},
    {20, 24, 12, 0x820DD110, 0x820DD438, 0x820DDBE4, 0x820DDBF8, 0x820DDC08, 0x820DDC1C,
     0x820DDC2C, 0x820DD104},
    {36, 40, 10, 0x820DD620, 0x820DD948, 0x820DDC94, 0x820DDCA8, 0x820DDCB8, 0x820DDCCC,
     0x820DDCDC, 0x820DD614},
};

const GroupSites& Sites(LightGroup which) {
  return kSites[int(which)];
}

}  // namespace

// --- The neons ---

Left LightPointsDrawNeon(uint8_t* base, const Calls& calls, LightGroup which, uint32_t stack,
                         uint32_t group, uint64_t counter, uint32_t block,
                         uint32_t view_projection, uint32_t caller_r24, uint32_t caller_r28) {
  switch (which) {
    case LightGroup::kFront: {
      ITC_NATIVE_RUNNING("sub_820DCC10 LightPoints::DrawFrontNeon");
      break;
    }
    case LightGroup::kSides: {
      ITC_NATIVE_RUNNING("sub_820DD0B0 LightPoints::DrawSidesNeon");
      break;
    }
    case LightGroup::kRear: {
      ITC_NATIVE_RUNNING("sub_820DD5C0 LightPoints::DrawRearNeon");
      break;
    }
  }
  const GroupSites& sites = Sites(which);
  const uint32_t frame = Frame(base, stack, kNeonDrawFrame);
  const uint32_t record = Load(base, group + 0);
  const double rate = Lfs(base, record + sites.fraction + 8);
  const double fade = Lfs(base, record + sites.fraction + 4);
  const double on = Lfs(base, record + sites.fraction);
  const double level = LightKitBlinkLevel(base, counter, on, fade, rate, frame);
  material_passes::SetAlpha(base, Load(base, group + 20), Load(base, group + 244), level, frame);
  const auto draw = CarPartDraw(base, calls, frame, group + 16, view_projection, block + 16, 0, 0,
                                caller_r24, caller_r28);
  Left left;
  if (!draw) {
    const uint64_t regs[5] = {group + 16, view_projection, block + 16, 0, 0};
    calls.call(kCarPartDraw, regs, 5, frame, sites.neon_draw);
    left.in_ctx = true;
  } else if (draw->queued) {
    left.in_ctx = true;
  } else {
    left.r3 = draw->r3;
  }
  return left;
}

// --- LightPoints::DrawFront, DrawSides, DrawRear ---

Left LightPointsDraw(uint8_t* base, const Calls& calls, LightGroup which, uint32_t stack,
                     uint32_t group, uint64_t counter, uint32_t block, uint32_t view_projection,
                     uint32_t caller_r24, uint32_t caller_r28) {
  switch (which) {
    case LightGroup::kFront: {
      ITC_NATIVE_RUNNING("sub_820DDAE8 LightPoints::DrawFront");
      break;
    }
    case LightGroup::kSides: {
      ITC_NATIVE_RUNNING("sub_820DDB98 LightPoints::DrawSides");
      break;
    }
    case LightGroup::kRear: {
      ITC_NATIVE_RUNNING("sub_820DDC48 LightPoints::DrawRear");
      break;
    }
  }
  const GroupSites& sites = Sites(which);
  const uint32_t frame = Frame(base, stack, kLightPointsDrawFrame);
  const uint32_t mode = Load(base, Load(base, group + 0) + sites.mode);
  Left left;
  left.r3 = group;
  if (mode < 1 || mode >= sites.last) {
    return left;
  }
  if (mode == 1) {
    Memcpy(base, calls, frame + 208, view_projection, 64, frame, sites.neon_copy);
    return LightPointsDrawNeon(base, calls, which, frame, group, counter, block, frame + 208,
                               caller_r24, caller_r28);
  }
  // The LEDs (modes 2 to 6) and spots (7 up), never seen in play: their
  // originals from this frame.
  const bool leds = mode < 7;
  const uint32_t copy = frame + (leds ? 80 : 144);
  Memcpy(base, calls, copy, view_projection, 64, frame, leds ? sites.leds_copy : sites.spots_copy);
  const uint64_t regs[4] = {group, counter, block, copy};
  calls.call(leds ? sites.leds : sites.spots, regs, 4, frame,
             leds ? sites.leds_call : sites.spots_call);
  left.in_ctx = true;
  return left;
}

// --- SceneLights::Draw ---

Left SceneLightsDraw(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t kit,
                     uint32_t block, bool advance_blink, uint32_t caller_r24,
                     uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820DDE58 SceneLights::Draw");
  const uint32_t frame = Frame(base, stack, kSceneLightsDrawFrame);
  const uint32_t scene = Load(base, kScene);
  const uint32_t local = frame + 208;
  second_set::SceneModelBlockConstruct(base, local);
  const uint32_t mask = Load(base, scene + 5132);
  const double one = Lfs(base, kOne);
  Stfs(base, frame + 156, one);
  Stfs(base, frame + 172, one);
  Stfs(base, frame + 188, one);
  Stfs(base, frame + 204, one);
  Left left;
  if ((mask & 0x8) == 0) {
    left.r3 = local;  // the block's constructor's
    return left;
  }
  Memcpy(base, calls, local, block, 736, frame, 0x820DDEB0);
  const uint32_t view_projection = frame + 144;
  if (int32_t(Load(base, Load(base, kit + 13160) + 4)) == 0) {
    // The front end: camera pass 0's view-projection, the copy's world y
    // (+68) the float at 0x820029D8, its world the identity first with the
    // block's +724 set.
    matrix_math::Copy(base, view_projection, kCameraPasses + 144);
    interpolation::BlendState_OnCopy(base, view_projection, kCameraPasses + 144, 0x820DDED4);
    const double y = Lfs(base, kFrontEndY);
    Stfs(base, frame + 276, y);
    if (int32_t(Load(base, frame + 932)) != 0) {
      for (uint32_t i = 0; i < 16; i++) {
        Store(base, frame + 80 + 4 * i, i % 5 == 0 ? 0x3F800000u : 0);
      }
      for (uint32_t i = 0; i < 64; i += 4) {
        Store(base, frame + 224 + i, Load(base, frame + 80 + i));
      }
      Stfs(base, frame + 276, y);
    }
  } else {
    uint32_t pass = kCameraPasses + Load(base, scene + 5084) * 592;
    if (pass == 0 || int32_t(Load(base, pass + 588)) == 0) {
      pass = 0;
    }
    matrix_math::Copy(base, view_projection, pass + 144);
    interpolation::BlendState_OnCopy(base, view_projection, pass + 144, 0x820DDF7C);
  }
  if (int32_t(Load(base, Load(base, kit + 13160) + 5048)) == 0 && advance_blink) {
    BlinkTimersAdvance(base, kit + 96, frame);
  }
  Store(base, kCullSwap, Load(base, kCullSwap) == 0 ? 1 : 0);
  struct Group {
    LightGroup which;
    uint32_t offset, counter, copy_lr;
  };
  constexpr Group kGroups[3] = {{LightGroup::kFront, 144, 112, 0x820DDFB8},
                                {LightGroup::kSides, 4480, 120, 0x820DDFDC},
                                {LightGroup::kRear, 8816, 128, 0x820DE000}};
  const uint32_t copy = frame + 80;
  for (const Group& g : kGroups) {
    Memcpy(base, calls, copy, view_projection, 64, frame, g.copy_lr);
    left = LightPointsDraw(base, calls, g.which, frame, kit + g.offset,
                           LoadU64(base, kit + g.counter), local, copy, caller_r24, caller_r28);
  }
  Store(base, kCullSwap, Load(base, kCullSwap) == 0 ? 1 : 0);
  return left;
}

}  // namespace car_draw
