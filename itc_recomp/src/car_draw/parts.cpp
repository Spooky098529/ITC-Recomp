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
    calls.call_r24_r28(kCarPartDraw, regs, 5, frame, lr, r24, r28);
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

// --- The doors and the headlights' pop-up: matrices kept for later draws ---

namespace {

// The identity's four rows (vupkd3d of zeros and its shuffles) stored at
// +96 as stvx stores them, then the 64 bytes from +96 copied doubleword by
// doubleword to `to`.
void IdentityTo(uint8_t* base, uint32_t frame, uint32_t to) {
  const uint32_t identity = (frame + 96) & ~0xFu;
  for (uint32_t i = 0; i < 16; i++) {
    Store(base, identity + 4 * i, i % 5 == 0 ? 0x3F800000u : 0);
  }
  for (uint32_t i = 0; i < 64; i += 8) {
    const uint32_t hi = Load(base, frame + 96 + i);
    const uint32_t lo = Load(base, frame + 96 + i + 4);
    Store(base, to + i, hi);
    Store(base, to + i + 4, lo);
  }
}

// The 1.0 w's (f31) of the matrix at `m`, stored before it's written.
void Ws(uint8_t* base, uint32_t m, double one) {
  for (uint32_t at = 12; at < 64; at += 16) {
    Stfs(base, m + at, one);
  }
}

// Matrix::Multiply and Matrix::Copy, noted as their wrappers note the
// originals' calls returning to `lr`.
void NotedMultiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t lr) {
  matrix_math::Multiply(base, out, a, b);
  interpolation::BlendState_OnMultiply(base, out, a, b, lr);
}
void NotedCopy(uint8_t* base, uint32_t to, uint32_t from, uint32_t lr) {
  matrix_math::Copy(base, to, from);
  interpolation::BlendState_OnCopy(base, to, from, lr);
}

// The doors' call sites (the right door's code is the left's with an fneg
// more in each path, so its addresses run 0x2F8 or 0x2FC on).
struct DoorSites {
  uint32_t node_memcpy[2], node_multiply[2];  // the normal door's
  uint32_t scissor_memcpy[2], scissor_multiply[2];
  uint32_t copy, multiply, copy_back, draws[3];
};
constexpr DoorSites kLeftDoor{{0x820E0C40, 0x820E0C50}, {0x820E0C7C, 0x820E0C9C},
                              {0x820E0D18, 0x820E0D28}, {0x820E0D54, 0x820E0D74},
                              0x820E0D84,               0x820E0DA4,
                              0x820E0DB0,               {0x820E0DCC, 0x820E0DF8, 0x820E0E28}};
constexpr DoorSites kRightDoor{{0x820E0F3C, 0x820E0F4C}, {0x820E0F78, 0x820E0F98},
                               {0x820E1018, 0x820E1028}, {0x820E1054, 0x820E1074},
                               0x820E1084,               0x820E10A4,
                               0x820E10B0,               {0x820E10CC, 0x820E10F8, 0x820E1128}};

}  // namespace

Left PartsDrawDoor(uint8_t* base, const Calls& calls, bool right, uint32_t stack,
                   uint32_t group, uint32_t block, uint32_t pass, uint32_t caller_r24) {
  if (right) {
    ITC_NATIVE_RUNNING("sub_820E0E38 PartsPoints::DrawRightDoor");
  } else {
    ITC_NATIVE_RUNNING("sub_820E0B40 PartsPoints::DrawLeftDoor");
  }
  const DoorSites& sites = right ? kRightDoor : kLeftDoor;
  const uint32_t frame = Frame(base, stack, kPartsDoorFrame);
  Left left;
  left.r3 = group;
  // The angles: the turn (theta) and the scissor door's lift (phi), each
  // into [0.0, pi] (a NaN stays).
  double theta = Lfs(base, block + (right ? 676 : 660));
  double phi = Lfs(base, block + (right ? 680 : 664));
  if (Load(base, group + 4) == 0) {
    return left;
  }
  const double low = Lfs(base, kZero);
  if (theta < low) {
    theta = low;
  }
  const double high = Lfs(base, kPi);
  if (theta > high) {
    theta = high;
  }
  if (phi < low) {
    phi = low;
  }
  if (phi > high) {
    phi = high;
  }
  // The right door turns the other way.
  const double turn = right ? -theta : theta;
  const bool scissor = int32_t(Load(base, Load(base, group + 6080) + 3688)) != 0;
  uint32_t product;
  if (!scissor) {
    // dl**_rot_00's inverse x the turn about y x its matrix.
    IdentityTo(base, frame, frame + 480);
    Stfs(base, frame + 80, turn);
    matrix_math::RotateY(base, frame + 480, frame + 480, frame + 80, frame - 80);
    Memcpy(base, calls, frame + 608, group + 16, 64, frame, sites.node_memcpy[0]);
    Memcpy(base, calls, frame + 544, group + 80, 64, frame, sites.node_memcpy[1]);
    const double one = Lfs(base, kOne);
    Ws(base, frame + 96, one);
    NotedMultiply(base, frame + 96, frame + 544, frame + 480, sites.node_multiply[0]);
    Ws(base, frame + 416, one);
    NotedMultiply(base, frame + 416, frame + 96, frame + 608, sites.node_multiply[1]);
    product = frame + 416;
  } else {
    // Probably a scissor door: lifted about x by -phi, then turned, about
    // dl**_rot_01 (record 1's matrices).
    IdentityTo(base, frame, frame + 288);
    Stfs(base, frame + 80, -phi);
    matrix_math::RotateX(base, frame + 288, frame + 288, frame + 80, frame - 80);
    Stfs(base, frame + 80, turn);
    matrix_math::RotateY(base, frame + 288, frame + 288, frame + 80, frame - 80);
    Memcpy(base, calls, frame + 544, group + 256, 64, frame, sites.scissor_memcpy[0]);
    Memcpy(base, calls, frame + 608, group + 320, 64, frame, sites.scissor_memcpy[1]);
    const double one = Lfs(base, kOne);
    Ws(base, frame + 160, one);
    NotedMultiply(base, frame + 160, frame + 608, frame + 288, sites.scissor_multiply[0]);
    Ws(base, frame + 352, one);
    NotedMultiply(base, frame + 352, frame + 160, frame + 544, sites.scissor_multiply[1]);
    product = frame + 352;
  }
  // The door's matrix (+6016, read by the later door draws and the mirror):
  // that times the block's world.
  const uint32_t door = group + 6016;
  NotedCopy(base, door, product, sites.copy);
  Ws(base, frame + 224, Lfs(base, kOne));
  NotedMultiply(base, frame + 224, door, block + 16, sites.multiply);
  NotedCopy(base, door, frame + 224, sites.copy_back);
  // The handle, the normal or scissor door, and the panel's design k (below
  // 11, signed). r28 is the pass here.
  const uint32_t vp = pass + 144;
  Part(base, calls, frame, group + Record(11), vp, door, 0, caller_r24, pass, sites.draws[0]);
  const bool lambo = int32_t(Load(base, Load(base, group + 6080) + 3688)) != 0;
  left = Part(base, calls, frame, group + (lambo ? Record(13) : Record(12)), vp, door, 0,
              caller_r24, pass, sites.draws[1]);
  const uint32_t k = Load(base, Load(base, group + 6084) + 12);
  if (int32_t(k) >= 11) {
    return left;
  }
  return Part(base, calls, frame, group + (k + 14) * 240, vp, door, 0, caller_r24, pass,
              sites.draws[2]);
}

Left PartsDrawHeadlightBody(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                            uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E13E0 PartsPoints::DrawHeadlightBody");
  const uint32_t frame = Frame(base, stack, kPartsHeadlightBodyFrame);
  Left left;
  left.r3 = group;
  if (Load(base, group + 4) == 0) {
    return left;
  }
  // The pop-up's matrix (+3120, read by DrawHeadlights later in the pass):
  // hl_**'s inverse x the turn about x by -angle x its matrix x the block's
  // world.
  const uint32_t popup = group + 3120;
  uint32_t product;
  if (int32_t(Load(base, block + 688)) != 0) {
    // The angle the block's +692 into [0.0, the node's own angle +3184].
    IdentityTo(base, frame, frame + 480);
    double angle = Lfs(base, block + 692);
    const double low = Lfs(base, kZero);
    if (angle < low) {
      angle = low;
    }
    const double top = Lfs(base, group + 3184);
    if (top < angle) {
      angle = top;
    }
    Stfs(base, frame + 80, -angle);
    matrix_math::RotateX(base, frame + 480, frame + 480, frame + 80, frame - 80);
    Memcpy(base, calls, frame + 672, group + 16, 64, frame, 0x820E14C0);
    Memcpy(base, calls, frame + 544, group + 80, 64, frame, 0x820E14D0);
    const double one = Lfs(base, kOne);
    Ws(base, frame + 96, one);
    NotedMultiply(base, frame + 96, frame + 544, frame + 480, 0x820E14F8);
    Ws(base, frame + 288, one);
    NotedMultiply(base, frame + 288, frame + 96, frame + 672, 0x820E1518);
    NotedCopy(base, popup, frame + 288, 0x820E1528);
    Ws(base, frame + 416, one);
    NotedMultiply(base, frame + 416, popup, block + 16, 0x820E1548);
    product = frame + 416;
  } else {
    // Up (+3184) while the lights are off, down (0.0) with the low or high
    // beams (bits 0x1, 0x40).
    IdentityTo(base, frame, frame + 608);
    double angle = Lfs(base, group + 3184);
    const uint32_t bits = Bits(base, block);
    if ((bits & 0x1) != 0 || (bits & 0x40) != 0) {
      angle = Lfs(base, kZero);
    }
    Stfs(base, frame + 80, -angle);
    matrix_math::RotateX(base, frame + 608, frame + 608, frame + 80, frame - 80);
    Memcpy(base, calls, frame + 544, group + 16, 64, frame, 0x820E15D8);
    Memcpy(base, calls, frame + 672, group + 80, 64, frame, 0x820E15E8);
    const double one = Lfs(base, kOne);
    Ws(base, frame + 352, one);
    NotedMultiply(base, frame + 352, frame + 672, frame + 608, 0x820E1610);
    Ws(base, frame + 160, one);
    NotedMultiply(base, frame + 160, frame + 352, frame + 544, 0x820E1630);
    NotedCopy(base, popup, frame + 160, 0x820E1640);
    Ws(base, frame + 224, one);
    NotedMultiply(base, frame + 224, popup, block + 16, 0x820E1660);
    product = frame + 224;
  }
  NotedCopy(base, popup, product, 0x820E166C);
  // hlmg**_shape with it; r28 is the pass here.
  return Part(base, calls, frame, group + Record(12), pass + 144, popup, 0, caller_r24, pass,
              0x820E1684);
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

// --- The parts' top ---

namespace {

// The parts' point block's groups (pb + offset; docs/functions/car-draw.md's
// rows of DrawFrontEnd and DrawRace) and its pointers.
constexpr uint32_t kFrontBumper = 0, kCooling = 2656, kRearBumper = 2896, kSkirts = 4592;
constexpr uint32_t kSpoiler = 4832, kSpoilerLight = 5072, kSpoilerLight01 = 5312;
constexpr uint32_t kBonnet = 5552, kLeftDoorGroup = 6032, kRightDoorGroup = 12128, kDoors = 18224;
constexpr uint32_t kLeftMirror = 23760, kRightMirror = 24000, kMirror = 24240;
constexpr uint32_t kHeadlights = 24480, kTailLights = 27680, kTailBody = 31040;
constexpr uint32_t kFenders = 31280, kCabin = 32960, kCabinBody = 33920, kEngine = 34160;
constexpr uint32_t kMuffler = 35856, kShadows = 36096;
constexpr uint32_t kSceneModelRecord = 37072, kCarRecordPointer = 37076;
constexpr uint32_t kPartsRecordPointer = 37080;
constexpr uint32_t kDoorMatrix = 6016;

}  // namespace

Left PartsDrawFrontEnd(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t points,
                       uint32_t block, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E2448 PartsPoints::DrawFrontEnd");
  const uint32_t frame = Frame(base, stack, kPartsFrontEndFrame);
  // Camera pass 0, whatever the scene's pass; r28 the block's world.
  const uint32_t pass = kCameraPasses, world = block + 16;
  uint32_t r24 = caller_r24;
  auto part = [&](uint32_t at, uint32_t lr) {
    return Part(base, calls, frame, points + at, pass + 144, world, 0, r24, world, lr);
  };
  part(kCooling, 0x820E2480);
  PartsDrawEngine(base, calls, frame, points + kEngine, block, pass, r24);
  part(kMuffler, 0x820E24B0);
  PartsDrawFrontBumper(base, calls, frame, points + kFrontBumper, block, pass, r24);
  PartsDrawBonnet(base, calls, frame, points + kBonnet, block, pass, r24);
  PartsDrawFendersBody(base, calls, frame, points + kFenders, block, pass, r24);
  part(kCabinBody, 0x820E2500);
  PartsDrawDoor(base, calls, false, frame, points + kLeftDoorGroup, block, pass, r24);
  PartsDrawDoor(base, calls, true, frame, points + kRightDoorGroup, block, pass, r24);
  part(kSkirts, 0x820E2540);
  // A compare of the car record's +3688 whose result isn't used.
  (void)Load(base, Load(base, points + kCarRecordPointer) + 3688);
  // The mirrors, each with a copy of its door's matrix at +80.
  Memcpy(base, calls, frame + 80, points + kLeftDoorGroup + kDoorMatrix, 64, frame, 0x820E2564);
  PartsDrawShadowed(base, calls, true, frame, points + kLeftMirror, pass, frame + 80, r24);
  Memcpy(base, calls, frame + 80, points + kRightDoorGroup + kDoorMatrix, 64, frame, 0x820E2588);
  PartsDrawShadowed(base, calls, true, frame, points + kRightMirror, pass, frame + 80, r24);
  // r24 the rear bumper's group from here on (the original keeps it there).
  r24 = points + kRearBumper;
  PartsDrawRearBumper(base, calls, frame, points + kRearBumper, block, pass, r24);
  PartsDrawShadowed(base, calls, false, frame, points + kSpoiler, pass, world, r24);
  PartsDrawHeadlightBody(base, calls, frame, points + kHeadlights, block, pass, r24);
  part(kTailBody, 0x820E25EC);
  if (int32_t(Load(base, block + 728)) == 0) {
    PartsDrawDoorWindows(base, calls, false, frame, points + kLeftDoorGroup, block, pass, r24);
    PartsDrawDoorWindows(base, calls, false, frame, points + kRightDoorGroup, block, pass, r24);
  }
  if (Bits(base, block) & 0x8) {
    part(kSpoilerLight01, 0x820E2640);
  }
  PartsDrawCabinWindow02(base, calls, frame, points + kCabin, block, pass, r24);
  if (int32_t(Load(base, block + 728)) == 0) {
    PartsDrawDoorWindows(base, calls, true, frame, points + kLeftDoorGroup, block, pass, r24);
    PartsDrawDoorWindows(base, calls, true, frame, points + kRightDoorGroup, block, pass, r24);
  }
  PartsDrawCabinWindow01(base, calls, frame, points + kCabin, block, pass, r24);
  PartsDrawFenderLights(base, calls, frame, points + kFenders, block, pass,
                        Load(base, Load(base, points + kPartsRecordPointer) + 56), r24);
  if (Bits(base, block) & 0x8) {
    part(kSpoilerLight, 0x820E26DC);
  }
  PartsDrawDoorPanel(base, calls, false, frame, points + kLeftDoorGroup, block, pass, r24);
  PartsDrawDoorPanel(base, calls, true, frame, points + kRightDoorGroup, block, pass, r24);
  PartsDrawFrontBumperLights(base, calls, frame, points + kFrontBumper, block, pass,
                             Load(base, Load(base, points + kPartsRecordPointer) + 56), r24);
  PartsDrawTailLights(base, calls, frame, points + kTailLights, block, pass, r24, world);
  PartsDrawRearBumperLights(base, calls, frame, points + kRearBumper, block, pass, r24);
  // The headlights: r7 the parts record's +32, r6 the car record's +0.
  const uint32_t parts_record = Load(base, points + kPartsRecordPointer);
  const uint32_t car_record = Load(base, points + kCarRecordPointer);
  const uint64_t sub = Load(base, parts_record + 32);
  const uint64_t index = Load(base, car_record);
  return PartsDrawHeadlights(base, calls, frame, points + kHeadlights, block, pass, index, sub,
                             r24);
}

Left PartsDrawRace(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t points,
                   uint32_t block, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E2760 PartsPoints::DrawRace");
  const uint32_t frame = Frame(base, stack, kPartsRaceFrame);
  // The scene's current camera pass (0 when disabled: its view-projection
  // read from address 144, as the original).
  const uint32_t index = Load(base, Load(base, kScene) + 5072);
  uint32_t pass = kCameraPasses + index * 592;
  if (pass == 0 || int32_t(Load(base, pass + 588)) == 0) {
    pass = 0;
  }
  // r6 for the shadows (1 for kind 2; not read there).
  (void)Load(base, Load(base, points + kSceneModelRecord));
  PartsDrawShadows(base, calls, frame, points + kShadows, block);
  const uint32_t world = block + 16;
  uint32_t r24 = caller_r24;
  auto part = [&](uint32_t at, uint32_t lr) {
    return Part(base, calls, frame, points + at, pass + 144, world, 0, r24, world, lr);
  };
  part(kCooling, 0x820E27F4);
  PartsDrawEngine(base, calls, frame, points + kEngine, block, pass, r24);
  part(kMuffler, 0x820E2824);
  PartsDrawFrontBumper(base, calls, frame, points + kFrontBumper, block, pass, r24);
  PartsDrawBonnet(base, calls, frame, points + kBonnet, block, pass, r24);
  PartsDrawFendersBody(base, calls, frame, points + kFenders, block, pass, r24);
  part(kCabinBody, 0x820E2874);
  part(kSkirts, 0x820E288C);
  PartsDrawDoorsBody(base, calls, frame, points + kDoors, block, pass, r24);
  PartsDrawShadowed(base, calls, false, frame, points + kMirror, pass, world, r24);
  PartsDrawRearBumper(base, calls, frame, points + kRearBumper, block, pass, r24);
  PartsDrawShadowed(base, calls, false, frame, points + kSpoiler, pass, world, r24);
  PartsDrawHeadlightBody(base, calls, frame, points + kHeadlights, block, pass, r24);
  part(kTailBody, 0x820E2900);
  if (Bits(base, block) & 0x8) {
    part(kSpoilerLight01, 0x820E2928);
  }
  // r24 the cabin's group from here on (the original keeps it there).
  r24 = points + kCabin;
  PartsDrawCabinWindow02(base, calls, frame, points + kCabin, block, pass, r24);
  PartsDrawCabinWindow01(base, calls, frame, points + kCabin, block, pass, r24);
  PartsDrawDoorsWindows(base, calls, frame, points + kDoors, block, pass, r24);
  PartsDrawFenderLights(base, calls, frame, points + kFenders, block, pass,
                        Load(base, Load(base, points + kPartsRecordPointer) + 56), r24);
  if (Bits(base, block) & 0x8) {
    part(kSpoilerLight, 0x820E29A8);
  }
  // The headlights before the front bumper's lights here: r7 the parts
  // record's +32, r6 the car record's +0.
  const uint64_t sub = Load(base, Load(base, points + kPartsRecordPointer) + 32);
  const uint64_t color = Load(base, Load(base, points + kCarRecordPointer));
  PartsDrawHeadlights(base, calls, frame, points + kHeadlights, block, pass, color, sub, r24);
  PartsDrawFrontBumperLights(base, calls, frame, points + kFrontBumper, block, pass,
                             Load(base, Load(base, points + kPartsRecordPointer) + 56), r24);
  PartsDrawTailLights(base, calls, frame, points + kTailLights, block, pass, r24, world);
  return PartsDrawRearBumperLights(base, calls, frame, points + kRearBumper, block, pass, r24);
}

Left ScenePartsDraw(uint8_t* base, const Calls& calls, uint32_t stack, uint64_t parts,
                    uint32_t block, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E2EE8 SceneParts::Draw");
  const uint32_t object = uint32_t(parts);
  const uint32_t set = Load(base, Load(base, object + 37284) + 4);
  if (set == 0) {
    return PartsDrawFrontEnd(base, calls, stack, object + 176, block, caller_r24);
  }
  if (set == 1) {
    return PartsDrawRace(base, calls, stack, object + 176, block, caller_r24);
  }
  Left left;
  left.r3 = parts;
  return left;
}

}  // namespace car_draw
