// The parts' small draws as plain C++ (docs/functions/car-draw.md: the
// PartsPoints draws of the bumpers, lights, doors, windows, fenders, cabin,
// engine, bonnet and mirrors); see car_draw.h. Every record goes through
// CarPart::Draw's plain C++ (its original from this frame where it
// declines). Built with -ffp-model=strict (CMakeLists.txt): the floats are
// stored as the translated code's single to double to single; the matrix
// work is matrix_math's, noted for frame interpolation as the wrappers note
// the originals' calls.

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "material_passes/material_passes.h"
#include "matrix_math/matrix_math.h"
#include "models/xpr.h"
#include "native/buffers.h"

namespace car_draw {

namespace {

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRaceMode = 0xA3AF4;
constexpr uint32_t kPlateFlag = 0x82816E3C;
constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kDepthKey = 0x8296B8FC;
constexpr uint32_t kOtherKey = 0x8296B8F8;
constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kPi = 0x82002240;
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

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// The block's flag bits (the low word of its +0 doubleword).
uint32_t Bits(uint8_t* base, uint32_t block) {
  return Load(base, block + 4);
}

// memcpy (sub_825A7410) from the frame at `sp` returning to `lr`: natively,
// else its original.
void Memcpy(uint8_t* base, const Calls& calls, uint32_t to, uint32_t from, uint32_t bytes,
            uint32_t sp, uint32_t lr) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    calls.memcpy(to, from, bytes, sp, lr);
  }
}

Left Part(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t part,
          uint32_t view_projection, uint32_t world, uint32_t depth, uint32_t r24, uint32_t r28,
          uint32_t lr) {
  return PartsDrawRecord(base, calls, frame, part, view_projection, world, depth, r24, r28, lr);
}

// PartModel::SetPair1002Alpha(record's model, its shape, the float at
// `alpha`) from `frame`.
void WindowAlpha(uint8_t* base, uint32_t record, uint32_t alpha, uint32_t frame) {
  const uint32_t shape = Load(base, record + 228);
  const double f1 = Lfs(base, alpha);
  material_passes::SetPair1002Alpha(base, Load(base, record + 4), shape, f1, frame);
}

constexpr uint32_t Record(uint32_t index) {
  return 240 * index;
}

}  // namespace

Left PartsDrawRecord(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t part,
                     uint32_t view_projection, uint32_t world, uint32_t depth, uint32_t r24,
                     uint32_t r28, uint32_t lr) {
  const auto draw =
      CarPartDraw(base, calls, frame, part, view_projection, world, 0, depth, r24, r28);
  Left left;
  if (!draw) {
    const uint64_t regs[5] = {part, view_projection, world, 0, depth};
    calls.call_r28(kCarPartDraw, regs, 5, frame, lr, r28);
    left.in_ctx = true;
  } else if (draw->queued) {
    left.in_ctx = true;
  } else {
    left.r3 = draw->r3;
  }
  return left;
}

// --- The lights ---

Left PartsDrawFrontBumperLights(uint8_t* base, const Calls& calls, uint32_t stack,
                                uint32_t group, uint32_t block, uint32_t pass, uint32_t r6,
                                uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DE1C8 PartsPoints::DrawFrontBumperLights");
  const uint32_t frame = Frame(base, stack, kPartsFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  auto draw = [&](uint32_t record, uint32_t lr) {
    return Part(base, calls, frame, group + record, vp, world, 0, caller_r24, r6, lr);
  };
  draw(Record(0), 0x820DE1FC);
  draw(int32_t(r6) == 0 ? Record(1) : Record(2), 0x820DE220);
  if (Bits(base, block) & 0x1) {
    draw(Record(3), 0x820DE248);
  }
  if (Bits(base, block) & 0x2) {
    draw(Record(4), 0x820DE270);
  }
  if (Bits(base, block) & 0x4) {
    draw(Record(5), 0x820DE298);
  }
  WindowAlpha(base, group + Record(6), block + 636, frame);
  return draw(Record(6), 0x820DE2C0);
}

Left PartsDrawRearBumperLights(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                               uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DE3C0 PartsPoints::DrawRearBumperLights");
  const uint32_t frame = Frame(base, stack, kPartsFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  auto draw = [&](uint32_t record, uint32_t lr) {
    return Part(base, calls, frame, group + record, vp, world, 0, caller_r24, block, lr);
  };
  Left left = draw(Record(0), 0x820DE3F0);
  if (Bits(base, block) & 0x1) {
    left = draw(Record(1), 0x820DE418);
  }
  if (Bits(base, block) & 0x10) {
    left = draw(Record(2), 0x820DE440);
  }
  if (Bits(base, block) & 0x2) {
    left = draw(Record(3), 0x820DE468);
  }
  if (Bits(base, block) & 0x4) {
    left = draw(Record(4), 0x820DE490);
  }
  return left;
}

Left PartsDrawFenderLights(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t block, uint32_t pass, uint32_t r6, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DEDD8 PartsPoints::DrawFenderLights");
  const uint32_t frame = Frame(base, stack, kPartsFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  auto draw = [&](uint32_t record, uint32_t lr) {
    return Part(base, calls, frame, group + record, vp, world, 0, caller_r24, r6, lr);
  };
  Left left = draw(Record(0), 0x820DEE0C);
  left = draw(int32_t(r6) == 0 ? Record(1) : Record(2), 0x820DEE30);
  if (Bits(base, block) & 0x2) {
    left = draw(Record(3), 0x820DEE58);
  }
  if (Bits(base, block) & 0x4) {
    left = draw(Record(4), 0x820DEE80);
  }
  return left;
}

// --- The doors ---

Left PartsDrawDoorPanel(uint8_t* base, const Calls& calls, bool right, uint32_t stack,
                        uint32_t group, uint32_t block, uint32_t pass, uint32_t caller_r24) {
  if (right) {
    ITC_NATIVE_RUNNING("sub_820DE8C8 PartsPoints::DrawRightDoorPanel");
  } else {
    ITC_NATIVE_RUNNING("sub_820DE6E0 PartsPoints::DrawLeftDoorPanel");
  }
  const uint32_t frame = Frame(base, stack, kPartsFrame);
  Left left;
  left.r3 = group;
  if (Load(base, group + 4) == 0) {
    return left;
  }
  const uint32_t vp = pass + 144, world = group + 6016;
  left = Part(base, calls, frame, group + Record(6), vp, world, 0, caller_r24, block,
              right ? 0x820DE908 : 0x820DE720);
  if ((Bits(base, block) & (right ? 0x4u : 0x2u)) == 0) {
    return left;
  }
  const uint32_t k = Load(base, Load(base, group + 6084) + 28);
  return Part(base, calls, frame, group + (k + 7) * 240, vp, world, 0, caller_r24, block,
              right ? 0x820DE940 : 0x820DE758);
}

Left PartsDrawDoorWindows(uint8_t* base, const Calls& calls, bool first, uint32_t stack,
                          uint32_t group, uint32_t block, uint32_t pass, uint32_t caller_r24) {
  if (first) {
    ITC_NATIVE_RUNNING("sub_820DE948 PartsPoints::DrawDoorWindows01");
  } else {
    ITC_NATIVE_RUNNING("sub_820DE760 PartsPoints::DrawDoorWindows02");
  }
  const uint32_t frame = Frame(base, stack, kPartsSmallFrame);
  Left left;
  left.r3 = group;
  if (Load(base, group + 4) == 0) {
    return left;
  }
  const uint32_t a = first ? Record(4) : Record(2);
  const uint32_t b = first ? Record(5) : Record(3);
  const uint32_t alpha = block + (first ? 632 : 628);
  WindowAlpha(base, group + a, alpha, frame);
  WindowAlpha(base, group + b, alpha, frame);
  const uint32_t vp = pass + 144, world = group + 6016;
  Part(base, calls, frame, group + a, vp, world, 0, caller_r24, 0,
       first ? 0x820DE9AC : 0x820DE7C4);
  return Part(base, calls, frame, group + b, vp, world, 0, caller_r24, 0,
              first ? 0x820DE9C4 : 0x820DE7DC);
}

Left PartsDrawDoorsBody(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                        uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DEA88 PartsPoints::DrawDoorsBody");
  const uint32_t frame = Frame(base, stack, kPartsSmallFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  Left left = Part(base, calls, frame, group + Record(11), vp, world, 0, caller_r24, 0,
                   0x820DEAB8);
  const uint32_t k = Load(base, Load(base, group + 5520) + 12);
  if (int32_t(k) >= 11) {
    return left;
  }
  return Part(base, calls, frame, group + (k + 12) * 240, vp, world, 0, caller_r24, 0,
              0x820DEAE8);
}

Left PartsDrawDoorsWindows(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DEAF0 PartsPoints::DrawDoorsWindows");
  const uint32_t frame = Frame(base, stack, kPartsFrame);
  WindowAlpha(base, group + Record(0), block + 632, frame);
  WindowAlpha(base, group + Record(1), block + 632, frame);
  const uint32_t vp = pass + 144, world = block + 16;
  auto draw = [&](uint32_t record, uint32_t lr) {
    return Part(base, calls, frame, group + record, vp, world, 0, caller_r24, block, lr);
  };
  Left left = draw(Record(0), 0x820DEB48);
  left = draw(Record(1), 0x820DEB60);
  left = draw(Record(2), 0x820DEB78);
  if (Bits(base, block) & 0x2) {
    const uint32_t k = Load(base, Load(base, group + 5520) + 28);
    left = draw((k + 3) * 240, 0x820DEBB0);
  }
  if (Bits(base, block) & 0x4) {
    const uint32_t k = Load(base, Load(base, group + 5520) + 28);
    left = draw((k + 7) * 240, 0x820DEBE8);
  }
  return left;
}

// --- The body ---

Left PartsDrawFendersBody(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                          uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DED88 PartsPoints::DrawFendersBody");
  const uint32_t frame = Frame(base, stack, kPartsSmallFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  Part(base, calls, frame, group + Record(5), vp, world, 0, caller_r24, 0, 0x820DEDB8);
  return Part(base, calls, frame, group + Record(6), vp, world, 0, caller_r24, 0, 0x820DEDD0);
}

Left PartsDrawCabinWindow02(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                            uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DEF38 PartsPoints::DrawCabinWindow02");
  const uint32_t frame = Frame(base, stack, kPartsSmallFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  if (Bits(base, block) & 0x8) {
    Part(base, calls, frame, group + Record(0), vp, world, 0, caller_r24, 0, 0x820DEF74);
  }
  WindowAlpha(base, group + Record(3), block + 628, frame);
  return Part(base, calls, frame, group + Record(3), vp, world, 0, caller_r24, 0, 0x820DEF9C);
}

Left PartsDrawCabinWindow01(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                            uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DEFA8 PartsPoints::DrawCabinWindow01");
  const uint32_t frame = Frame(base, stack, kPartsFrame);
  WindowAlpha(base, group + Record(2), block + 632, frame);
  const uint32_t vp = pass + 144, world = block + 16;
  Left left =
      Part(base, calls, frame, group + Record(2), vp, world, 0, caller_r24, vp, 0x820DEFF0);
  if (Bits(base, block) & 0x8) {
    left = Part(base, calls, frame, group + Record(1), vp, world, 0, caller_r24, vp, 0x820DF018);
  }
  return left;
}

Left PartsDrawEngine(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                     uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DF070 PartsPoints::DrawEngine");
  const uint32_t frame = Frame(base, stack, kPartsSmallFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  Part(base, calls, frame, group + Record(0), vp, world, 0, caller_r24, 0, 0x820DF09C);
  return Part(base, calls, frame, group + Record(1), vp, world, 0, caller_r24, 0, 0x820DF0B4);
}

// --- The bumpers and their plates ---

namespace {

// Whether the plate takes the car model's texture: the group's flag set,
// a plate model, the race mode not 7 or 8; then [0x82816E3C] decides.
bool PlateRace(uint8_t* base, uint32_t group, uint32_t flag, uint32_t plate) {
  if (int32_t(Load(base, group + flag)) == 0 || Load(base, group + plate) == 0) {
    return false;
  }
  const int32_t mode = int32_t(Load(base, Load(base, kRaceData) + kRaceMode));
  return mode != 7 && mode != 8;
}

// XPR::GetTexture(the scene's XPR, 0): probably a blank plate.
uint32_t BlankPlate(uint8_t* base) {
  return uint32_t(models::XprGetTexture(base, Load(base, kScene) + 4648, 0));
}

}  // namespace

Left PartsDrawFrontBumper(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                          uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DF308 PartsPoints::DrawFrontBumper");
  const uint32_t frame = Frame(base, stack, kPartsSmallFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  Left left =
      Part(base, calls, frame, group + Record(7), vp, world, 0, caller_r24, 0, 0x820DF338);
  bool blank = true;
  if (PlateRace(base, group, 2640, 1924)) {
    blank = Load(base, kPlateFlag) != 0;
  } else if (Load(base, group + 1924) == 0) {
    return left;
  }
  if (blank) {
    const uint32_t texture = BlankPlate(base);
    Store(base, Load(base, group + 1924) + 456, texture);
  } else {
    const uint32_t texture = Load(base, group + 2644);
    Store(base, Load(base, group + 1924) + 456, texture);
  }
  Store(base, Load(base, group + 1924) + 432, 1);
  const uint32_t k = Load(base, Load(base, group + 2648) + 3684);
  left = Part(base, calls, frame, group + (k + 8) * 240, vp, world, 0, caller_r24, 0, 0x820DF3FC);
  Store(base, Load(base, group + 1924) + 432, 0);
  return left;
}

Left PartsDrawRearBumper(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                         uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DF458 PartsPoints::DrawRearBumper");
  const uint32_t frame = Frame(base, stack, kPartsSmallFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  Part(base, calls, frame, group + Record(5), vp, world, 0, caller_r24, 0, 0x820DF488);
  // Unlike the front's, with no plate model (+1444 0) this still writes
  // guest words 456 and 432, as the original.
  if (PlateRace(base, group, 1680, 1444) && Load(base, kPlateFlag) == 0) {
    const uint32_t texture = Load(base, group + 1684);
    Store(base, Load(base, group + 1444) + 456, texture);
  } else {
    const uint32_t texture = BlankPlate(base);
    Store(base, Load(base, group + 1444) + 456, texture);
  }
  Store(base, Load(base, group + 1444) + 432, 1);
  const Left left =
      Part(base, calls, frame, group + Record(6), vp, world, 0, caller_r24, 0, 0x820DF558);
  Store(base, Load(base, group + 1444) + 432, 0);
  return left;
}

// --- The bonnet ---

Left PartsDrawBonnet(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                     uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E0998 PartsPoints::DrawBonnet");
  const uint32_t frame = Frame(base, stack, kPartsBonnetFrame);
  Left left;
  left.r3 = group;
  if (Load(base, group + 4) == 0) {
    return left;
  }
  uint32_t world = block + 16;
  if (int32_t(Load(base, Load(base, kScene) + 4)) == 0) {
    // The front end: the bonnet opened about its hinge.
    double angle = Lfs(base, block + 640);
    const double low = Lfs(base, kZero);
    if (angle < low) {
      angle = low;
    }
    const double high = Lfs(base, kPi);
    if (angle > high) {
      angle = high;
    }
    // The identity (stvx: to the 16-byte boundary), then doubleword by
    // doubleword into +160.
    const uint32_t identity = (frame + 96) & ~0xFu;
    for (uint32_t i = 0; i < 16; i++) {
      Store(base, identity + 4 * i, i % 5 == 0 ? 0x3F800000u : 0);
    }
    for (uint32_t i = 0; i < 64; i += 8) {
      const uint32_t hi = Load(base, frame + 96 + i);
      const uint32_t lo = Load(base, frame + 96 + i + 4);
      Store(base, frame + 160 + i, hi);
      Store(base, frame + 160 + i + 4, lo);
    }
    Stfs(base, frame + 80, angle);
    matrix_math::RotateX(base, frame + 160, frame + 160, frame + 80, frame - 80);
    Memcpy(base, calls, frame + 416, group + 16, 64, frame, 0x820E0A80);
    Memcpy(base, calls, frame + 352, group + 80, 64, frame, 0x820E0A90);
    const double one = Lfs(base, kOne);
    auto ws = [&](uint32_t m) {
      for (uint32_t at = 12; at < 64; at += 16) {
        Stfs(base, m + at, one);
      }
    };
    auto multiply = [&](uint32_t out, uint32_t a, uint32_t b, uint32_t lr) {
      matrix_math::Multiply(base, out, a, b);
      interpolation::BlendState_OnMultiply(base, out, a, b, lr);
    };
    auto copy = [&](uint32_t to, uint32_t from, uint32_t lr) {
      matrix_math::Copy(base, to, from);
      interpolation::BlendState_OnCopy(base, to, from, lr);
    };
    ws(frame + 96);
    multiply(frame + 96, frame + 352, frame + 160, 0x820E0AB8);
    ws(frame + 224);
    multiply(frame + 224, frame + 96, frame + 416, 0x820E0AD8);
    copy(frame + 160, frame + 224, 0x820E0AE4);
    ws(frame + 288);
    multiply(frame + 288, frame + 160, block + 16, 0x820E0B04);
    copy(frame + 160, frame + 288, 0x820E0B10);
    world = frame + 160;
  }
  return Part(base, calls, frame, group + Record(1), pass + 144, world, 0, caller_r24, 0,
              0x820E0B30);
}

// --- The mirrors and the spoiler: drawn, then into the shadow's depth ---

Left PartsDrawShadowed(uint8_t* base, const Calls& calls, bool mirror, uint32_t stack,
                       uint32_t record, uint32_t pass, uint32_t world, uint32_t caller_r24) {
  if (mirror) {
    ITC_NATIVE_RUNNING("sub_820E1138 PartsPoints::DrawMirror");
  } else {
    ITC_NATIVE_RUNNING("sub_820E1260 PartsPoints::DrawWithShadow");
  }
  struct Sites {
    uint32_t draw, memcpy, multiply, copy, depth;
  };
  constexpr Sites kMirror{0x820E1164, 0x820E11D8, 0x820E1200, 0x820E120C, 0x820E1248};
  constexpr Sites kShadow{0x820E1288, 0x820E12FC, 0x820E1324, 0x820E1330, 0x820E136C};
  const Sites& sites = mirror ? kMirror : kShadow;
  const uint32_t frame = Frame(base, stack, kPartsShadowFrame);
  Left left = Part(base, calls, frame, record, pass + 144, world, 0, caller_r24, world,
                   sites.draw);
  const uint32_t description = Load(base, kScene) + 5072;
  const uint32_t camera = kCameraPasses + Load(base, description + 16) * 592;
  if (camera == 0 || int32_t(Load(base, camera + 588)) == 0) {
    return left;
  }
  if ((Load(base, description + 60) & 0x100) == 0) {
    return left;
  }
  Memcpy(base, calls, frame + 144, camera + 144, 64, frame, sites.memcpy);
  const double one = Lfs(base, kOne);
  for (uint32_t at = 92; at < 144; at += 16) {
    Stfs(base, frame + at, one);
  }
  matrix_math::Multiply(base, frame + 80, description + 112, frame + 144);
  interpolation::BlendState_OnMultiply(base, frame + 80, description + 112, frame + 144,
                                       sites.multiply);
  matrix_math::Copy(base, frame + 144, frame + 80);
  interpolation::BlendState_OnCopy(base, frame + 144, frame + 80, sites.copy);
  const uint32_t key = Load(base, description + 56);
  Store(base, description + 76, Load(base, kDepthKey));
  Store(base, description + 80, Load(base, kOtherKey));
  Store(base, kDepthKey, key);
  Store(base, kOtherKey, key);
  left = Part(base, calls, frame, record, frame + 144, world, 1, caller_r24, world, sites.depth);
  const uint32_t other = Load(base, description + 80);
  const uint32_t depth = Load(base, description + 76);
  Store(base, kOtherKey, other);
  Store(base, kDepthKey, depth);
  return left;
}

}  // namespace car_draw
