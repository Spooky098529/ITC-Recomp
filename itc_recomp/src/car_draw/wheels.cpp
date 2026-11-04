// The wheels' draws as plain C++ (docs/functions/car-draw.md: the tires,
// rims and brakes of pool 5); see car_draw.h. Every record goes through
// CarPart::Draw's plain C++ (its original from this frame where it
// declines). Built with -ffp-model=strict (CMakeLists.txt): the scalar float
// work as the translated code's doubles rounded to single (fused where the
// original fuses), the translations' vector adds with flush-to-zero on as the
// translation turns it on; the matrix work is matrix_math's, noted for frame
// interpolation as the wrappers note the originals' calls.
//
// The helpers below (fresh identities, the side's translation, the product
// chains, the second rear tire's shift, the depth pass) are shared by the
// wheels' draws; the rims and brakes build their sides the same way.

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "material_passes/material_passes.h"
#include "matrix_math/matrix_math.h"
#include "native/buffers.h"
#include "second_set/morph.h"

#include <initializer_list>

namespace car_draw {

namespace {

using namespace camera_modes::steps;

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kDepthKey = 0x8296B8FC;
constexpr uint32_t kOtherKey = 0x8296B8F8;
constexpr uint32_t kOne = 0x82000A30;     // 1.0
constexpr uint32_t kZero = 0x82000A34;    // 0.0
constexpr uint32_t kPi = 0x82002240;      // the turn of side 2
constexpr uint32_t kShift = 0x82002D0C;   // the second rear tire's step
constexpr uint32_t kThree = 0x820021E4;   // 3.0
constexpr uint32_t kMilli = 0x820029D8;   // 0.001
constexpr uint32_t kOneBits = 0x3F800000;
constexpr uint32_t kIndexTable = 0x820044B0;  // 44 bytes a car
constexpr uint32_t kMorphShape = 0x82595110;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

simde__m128 Lvx(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(base, at), Load(base, at + 4), Load(base, at + 8), Load(base, at + 12));
}

// memcpy (sub_825A7410) from the frame at `sp` returning to `lr`: natively,
// else its original.
void Memcpy(uint8_t* base, const Calls& calls, uint32_t to, uint32_t from, uint32_t bytes,
            uint32_t sp, uint32_t lr) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    calls.memcpy(to, from, bytes, sp, lr);
  }
}

// An identity stvx'd at `at` and its 64 bytes copied doubleword by
// doubleword to `to` (the originals' fresh matrices).
void FreshIdentity(uint8_t* base, uint32_t at, uint32_t to) {
  StoreIdentity(base, at);
  CopyDoublewords(base, to, at, 64);
}

// The side's translation: unless the block's +732 is set, T's row 3 (at
// `row`) = `w` + the group's float4 at `side` (vaddfp, flush-to-zero on).
void Translate(uint8_t* base, uint32_t block, uint32_t row, simde__m128 w, uint32_t side) {
  if (int32_t(Load(base, block + 732)) != 0) {
    return;
  }
  const simde__m128 add = Lvx(base, side);
  FlushToZero ftz;
  Stvx(base, row, simde_mm_add_ps(w, add));
}

// The product chain factors[0] x factors[1] x ... into outs[0], outs[1], ...
// (each with its 1.0 w's written first, noted returning to lrs[i]); the last
// product's address.
uint32_t Chain(uint8_t* base, std::initializer_list<uint32_t> factors, const uint32_t* outs,
               const uint32_t* lrs, double one) {
  const uint32_t* f = factors.begin();
  uint32_t left = f[0];
  for (size_t i = 1; i < factors.size(); i++) {
    const uint32_t out = outs[i - 1];
    for (uint32_t at = 12; at < 64; at += 16) {
      Stfs(base, out + at, one);
    }
    matrix_math::Multiply(base, out, left, f[i]);
    interpolation::BlendState_OnMultiply(base, out, left, f[i], lrs[i - 1]);
    left = out;
  }
  return left;
}

void NotedCopy(uint8_t* base, uint32_t to, uint32_t from, uint32_t lr) {
  matrix_math::Copy(base, to, from);
  interpolation::BlendState_OnCopy(base, to, from, lr);
}

// The second rear tire's (and rim's) shift: T's x (at `x`) +=
// float(the wheels record's +56) x [0x82002D0C] x 3.0, fused with x 0.001
// (the integer through the frame's +80, as fcfid takes it).
void ShiftX(uint8_t* base, uint32_t frame, uint32_t group, uint32_t x, double shift,
            double three, double milli) {
  const uint32_t wheels = Load(base, group + 8);
  const double f13 = Lfs(base, x);
  const int64_t count = int64_t(int32_t(Load(base, wheels + 56)));
  Store64(base, frame + 80, uint64_t(count));
  double f0 = Single(double(count));
  f0 = Fmuls(f0, shift);
  f0 = Fmuls(f0, three);
  f0 = Fmadds(f0, milli, f13);
  Stfs(base, x, f0);
}

// The pass at the description's +4, or 0 when it's disabled (+588 clear).
uint32_t EnabledPass(uint8_t* base, uint32_t description) {
  const uint32_t camera = kCameraPasses + Load(base, description + 4) * 592;
  if (camera == 0 || int32_t(Load(base, camera + 588)) == 0) {
    return 0;
  }
  return camera;
}

// The return addresses of a side's depth pass.
struct DepthSites {
  uint32_t memcpy, draw, second;
};

// A tire side's depth pass: unless the wheels record's +0 is 2, with the
// pass at the description's +4 enabled and its +60 bit 0x4, its
// view-projection memcpy'd to `copy`, the keys saved to the description's +76
// and +80 and set to its +32, the record drawn depth-only at `world` with it
// and, with a `block` (0 for none) whose +592 is set, at `second` with
// `main_vp`, the keys put back. `left` what's in r3 before.
Left TireDepth(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t group,
               uint32_t record, uint32_t world, uint32_t copy, uint32_t block, uint32_t second,
               uint32_t main_vp, uint64_t r24, uint64_t r28, const DepthSites& sites,
               Left left) {
  const uint32_t description = Load(base, kScene) + 5072;
  if (int32_t(Load(base, Load(base, group + 8))) == 2) {
    return left;
  }
  const uint32_t camera = EnabledPass(base, description);
  if (camera == 0 || (Load(base, description + 60) & 0x4) == 0) {
    return left;
  }
  Memcpy(base, calls, copy, camera + 144, 64, frame, sites.memcpy);
  const uint32_t key = Load(base, description + 32);
  Store(base, description + 76, Load(base, kDepthKey));
  Store(base, description + 80, Load(base, kOtherKey));
  Store(base, kDepthKey, key);
  Store(base, kOtherKey, key);
  left = PartsDrawRecord(base, calls, frame, record, copy, world, 1, r24, r28, sites.draw);
  if (block != 0 && int32_t(Load(base, block + 592)) != 0) {
    left = PartsDrawRecord(base, calls, frame, record, main_vp, second, 1, r24, r28,
                           sites.second);
  }
  const uint32_t other = Load(base, description + 80);
  const uint32_t depth = Load(base, description + 76);
  Store(base, kOtherKey, other);
  Store(base, kDepthKey, depth);
  return left;
}

}  // namespace

// --- The tires ---

Left WheelsDrawTire(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                    uint32_t camera, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820E8AE8 WheelPoints::DrawTire");
  const uint32_t frame = Frame(base, stack, kWheelsTireFrame);
  const uint32_t t = frame + 160, main = frame + 224, second = frame + 288;
  // T, the side's world and the second rear tire's, all identities.
  FreshIdentity(base, frame + 96, t);
  FreshIdentity(base, frame + 96, main);
  FreshIdentity(base, frame + 96, second);
  Translate(base, block, t + 48, VectorOf(0, 0, 0, kOneBits), group + 1808);
  const bool front = int32_t(Load(base, group + 300)) != 0;
  const double milli = Lfs(base, kMilli), three = Lfs(base, kThree), shift = Lfs(base, kShift);
  const double one = Lfs(base, kOne);
  const uint32_t world = block + 16;
  if (front) {
    static constexpr uint32_t kLrs[] = {0x820E8C8C, 0x820E8CAC, 0x820E8CD0};
    const uint32_t outs[] = {frame + 992, frame + 480, frame + 1504};
    NotedCopy(base, main, Chain(base, {block + 336, block + 80, t, world}, outs, kLrs, one),
              0x820E8DF8);
  } else {
    static constexpr uint32_t kLrs[] = {0x820E8D04, 0x820E8D24, 0x820E8D48};
    const uint32_t outs[] = {frame + 608, frame + 1248, frame + 736};
    NotedCopy(base, main, Chain(base, {block + 464, block + 208, t, world}, outs, kLrs, one),
              0x820E8D54);
    if (int32_t(Load(base, block + 592)) != 0) {
      ShiftX(base, frame, group, t + 48, shift, three, milli);
      static constexpr uint32_t kSecondLrs[] = {0x820E8DAC, 0x820E8DCC, 0x820E8DEC};
      const uint32_t second_outs[] = {frame + 1696, frame + 864, frame + 1376};
      NotedCopy(base, second,
                Chain(base, {block + 464, block + 208, t, world}, second_outs, kSecondLrs, one),
                0x820E8DF8);
    }
  }
  const uint32_t vp = camera + 144, record = group + 368;
  const uint64_t r24 = record, r28 = block;
  Left left = PartsDrawRecord(base, calls, frame, record, vp, main, 0, r24, r28, 0x820E8E18);
  if (int32_t(Load(base, block + 592)) != 0) {
    left = PartsDrawRecord(base, calls, frame, record, vp, second, 0, r24, r28, 0x820E8E3C);
  }
  left = TireDepth(base, calls, frame, group, record, main, frame + 1760, block, second, vp,
                   r24, r28, {0x820E8EC8, 0x820E8EFC, 0x820E8F20}, left);
  // Side 2: turned by pi about y.
  const uint32_t turned = frame + 1056;
  FreshIdentity(base, frame + 96, turned);
  Stfs(base, frame + 80, Lfs(base, kPi));
  matrix_math::RotateY(base, turned, turned, frame + 80, frame - 80);
  // T's row 3 back to (0, 0, 0, 1), through +80.
  Stfs(base, frame + 92, one);
  const double zero = Lfs(base, kZero);
  Stfs(base, frame + 80, zero);
  Stfs(base, frame + 84, zero);
  Stfs(base, frame + 88, zero);
  CopyDoublewords(base, t + 48, frame + 80, 16);
  Translate(base, block, t + 48, Lvx(base, frame + 80), group + 1824);
  if (int32_t(Load(base, group + 300)) != 0) {
    static constexpr uint32_t kLrs[] = {0x820E9028, 0x820E9048, 0x820E9068, 0x820E9088};
    const uint32_t outs[] = {frame + 352, frame + 1632, frame + 1120, frame + 416};
    NotedCopy(base, main,
              Chain(base, {turned, block + 400, block + 144, t, world}, outs, kLrs, one),
              0x820E91E8);
  } else {
    static constexpr uint32_t kLrs[] = {0x820E90B4, 0x820E90D8, 0x820E90F8, 0x820E9118};
    const uint32_t outs[] = {frame + 544, frame + 672, frame + 800, frame + 928};
    NotedCopy(base, main,
              Chain(base, {turned, block + 528, block + 272, t, world}, outs, kLrs, one),
              0x820E9124);
    if (int32_t(Load(base, block + 592)) != 0) {
      ShiftX(base, frame, group, t + 48, shift, three, milli);
      static constexpr uint32_t kSecondLrs[] = {0x820E917C, 0x820E919C, 0x820E91BC,
                                                0x820E91DC};
      const uint32_t second_outs[] = {frame + 1184, frame + 1312, frame + 1440, frame + 1568};
      NotedCopy(
          base, second,
          Chain(base, {turned, block + 528, block + 272, t, world}, second_outs, kSecondLrs, one),
          0x820E91E8);
    }
  }
  left = PartsDrawRecord(base, calls, frame, record, vp, main, 0, r24, r28, 0x820E9200);
  if (int32_t(Load(base, block + 592)) != 0) {
    left = PartsDrawRecord(base, calls, frame, record, vp, second, 0, r24, r28, 0x820E9224);
  }
  return TireDepth(base, calls, frame, group, record, main, frame + 1824, block, second, vp,
                   r24, r28, {0x820E9294, 0x820E92C8, 0x820E92EC}, left);
}

Left WheelsDrawTireMorphed(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t camera, uint32_t block, uint64_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E9318 WheelPoints::DrawTireMorphed");
  const uint32_t frame = Frame(base, stack, kWheelsTireMorphedFrame);
  const uint32_t t = frame + 176, main = frame + 240, morph = frame + 1392;
  FreshIdentity(base, frame + 80, t);
  FreshIdentity(base, frame + 80, main);
  // The weights again (not for traffic, kind 3), as WheelPoints::SetUpTire:
  // the record's entry index (+44, or the rear's +56) with its float and
  // value, else the car's table index plus the record's +72 (+76).
  const uint32_t wheels = Load(base, group + 8);
  if (int32_t(Load(base, wheels + 0)) != 3) {
    const bool front = int32_t(Load(base, group + 300)) != 0;
    const uint32_t own = front ? 44 : 56;
    uint64_t index;
    if (int32_t(Load(base, wheels + own)) != -1) {
      const uint32_t record = Load(base, group + 8);
      const double f1 = Lfs(base, record + own + 8);
      second_set::TireFillEntries(base, group + 12, Load(base, record + own), record + own + 4,
                                  f1, frame);
      index = Load(base, Load(base, group + 8) + own);
    } else {
      const uint32_t add = Load(base, wheels + (front ? 72 : 76));
      const uint32_t car = Load(base, Load(base, group + 4) + 0);
      index = uint64_t(Load(base, uint32_t(uint64_t(car) * 44) + kIndexTable)) + uint64_t(add);
    }
    // sub_820E6F28 leaves r3 the entries.
    second_set::TireReadEntry(base, uint32_t(index), group + 12, group + 328, group + 352,
                              group + 356, frame);
  }
  // Record 0's shape toward records 1 to 5 by the five weights.
  const uint32_t model = Load(base, group + 372);
  second_set::MorphSetUp(base, model, morph, Load(base, group + 596), group + 304, group + 328,
                         5);
  const uint64_t shape = Load(base, group + 596);
  if (!ModelMorphShape(base, calls, model, morph, shape, frame)) {
    const uint64_t regs[3] = {model, morph, shape};
    calls.call(kMorphShape, regs, 3, frame, 0x820E94E8);
  }
  Translate(base, block, t + 48, VectorOf(0, 0, 0, kOneBits), group + 1808);
  const double one = Lfs(base, kOne);
  const uint32_t world = block + 16;
  if (int32_t(Load(base, group + 300)) != 0) {
    static constexpr uint32_t kLrs[] = {0x820E9540, 0x820E9560, 0x820E9584};
    const uint32_t outs[] = {frame + 1072, frame + 432, frame + 944};
    NotedCopy(base, main, Chain(base, {block + 336, block + 80, t, world}, outs, kLrs, one),
              0x820E95FC);
  } else {
    static constexpr uint32_t kLrs[] = {0x820E95AC, 0x820E95CC, 0x820E95F0};
    const uint32_t outs[] = {frame + 560, frame + 1136, frame + 688};
    NotedCopy(base, main, Chain(base, {block + 464, block + 208, t, world}, outs, kLrs, one),
              0x820E95FC);
  }
  const uint32_t vp = camera + 144, record = group + 368;
  // r24 the block's world from the matrices on; r28 the caller's at the
  // first draw, then lis's sign-extended 0x82970000.
  const uint64_t r24 = world;
  Left left =
      PartsDrawRecord(base, calls, frame, record, vp, main, 0, r24, caller_r28, 0x820E961C);
  const uint64_t r28 = 0xFFFFFFFF82970000ull;
  left = TireDepth(base, calls, frame, group, record, main, frame + 1328, 0, 0, vp, r24, r28,
                   {0x820E96A8, 0x820E96DC, 0}, left);
  // Side 2: turned by pi about y.
  const uint32_t turned = frame + 1200;
  FreshIdentity(base, frame + 80, turned);
  Stfs(base, frame + 144, Lfs(base, kPi));
  matrix_math::RotateY(base, turned, turned, frame + 144, frame - 80);
  // T's row 3 back to (0, 0, 0, 1), through +160.
  Stfs(base, frame + 172, one);
  const double zero = Lfs(base, kZero);
  Stfs(base, frame + 160, zero);
  Stfs(base, frame + 164, zero);
  Stfs(base, frame + 168, zero);
  CopyDoublewords(base, t + 48, frame + 160, 16);
  Translate(base, block, t + 48, Lvx(base, frame + 160), group + 1824);
  if (int32_t(Load(base, group + 300)) != 0) {
    static constexpr uint32_t kLrs[] = {0x820E97E4, 0x820E9804, 0x820E9824, 0x820E9844};
    const uint32_t outs[] = {frame + 304, frame + 816, frame + 1008, frame + 368};
    NotedCopy(base, main,
              Chain(base, {turned, block + 400, block + 144, t, world}, outs, kLrs, one),
              0x820E98D4);
  } else {
    static constexpr uint32_t kLrs[] = {0x820E9868, 0x820E9888, 0x820E98A8, 0x820E98C8};
    const uint32_t outs[] = {frame + 496, frame + 624, frame + 752, frame + 880};
    NotedCopy(base, main,
              Chain(base, {turned, block + 528, block + 272, t, world}, outs, kLrs, one),
              0x820E98D4);
  }
  left = PartsDrawRecord(base, calls, frame, record, vp, main, 0, r24, r28, 0x820E98EC);
  return TireDepth(base, calls, frame, group, record, main, frame + 1264, 0, 0, vp, r24, r28,
                   {0x820E995C, 0x820E9990, 0}, left);
}

// --- The set a rims ---

namespace {

// Where a set a rim draw keeps its matrices and the return addresses of its
// calls (DrawRimA's and DrawRimAMorphed's frames differ only in offsets).
struct RimALayout {
  uint32_t frame_bytes;
  uint32_t world_copy_lr;
  uint32_t front_outs[3], front_lrs[3], rear_outs[3], rear_lrs[3];
  uint32_t side1_copy_lr;
  uint32_t side1_draw_lrs[5];
  uint32_t turned_world, mirror, turned, angle, row;
  uint32_t mirror_front_outs[4], mirror_front_lrs[4], turned_front_outs[4],
      turned_front_lrs[4];
  uint32_t mirror_rear_outs[4], mirror_rear_lrs[4], turned_rear_outs[4], turned_rear_lrs[4];
  uint32_t mirror_front_copy_lr, mirror_rear_copy_lr, turned_copy_lr;
  uint32_t side2_draw_lrs[5];
  bool z_add_swapped;  // fadds f0,f0,f13 (the morphed) against f13,f0
};

constexpr uint32_t kMirrorZ = 0x82001BE0;  // Mz's z scale

constexpr RimALayout kRimA = {
    kWheelsRimAFrame,
    0x820E9B18,
    {1392, 1136, 496},
    {0x820E9B80, 0x820E9BA0, 0x820E9BC0},
    {1520, 624, 1264},
    {0x820E9BE8, 0x820E9C08, 0x820E9C28},
    0x820E9C34,
    {0x820E9C54, 0x820E9C70, 0x820E9C8C, 0x820E9CA8, 0x820E9CC4},
    1840,
    1776,
    1712,
    160,
    144,
    {752, 1648, 880, 304},
    {0x820E9E8C, 0x820E9EB0, 0x820E9ED0, 0x820E9EF0},
    {1008, 1584, 368, 432},
    {0x820E9F1C, 0x820E9F3C, 0x820E9F5C, 0x820E9F7C},
    {560, 688, 816, 944},
    {0x820E9FA4, 0x820E9FC8, 0x820E9FE8, 0x820EA008},
    {1072, 1200, 1328, 1456},
    {0x820EA034, 0x820EA054, 0x820EA074, 0x820EA094},
    0x820E9EFC,
    0x820EA014,
    0x820EA0A0,
    {0x820EA0C0, 0x820EA0D8, 0x820EA0F0, 0x820EA108, 0x820EA128},
    false,
};

// T (+240) and the side's world (+176), fresh: the identity stvx'd at +176
// and copied to +240, then one stvx'd at `scratch` and copied to +176.
void RimAIdentities(uint8_t* base, uint32_t frame, uint32_t scratch) {
  FreshIdentity(base, frame + 176, frame + 240);
  FreshIdentity(base, frame + scratch, frame + 176);
}

// Both sides of a set a rim, from the world copy on (see car_draw.h).
Left RimASides(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t group,
               uint32_t camera, uint32_t block, uint64_t caller_r24, const RimALayout& l) {
  const uint32_t t = frame + 240, main = frame + 176, world = block + 16;
  NotedCopy(base, main, world, l.world_copy_lr);
  Translate(base, block, t + 48, VectorOf(0, 0, 0, kOneBits), group + 1712);
  const bool front = int32_t(Load(base, group + 12)) != 0;
  {
    const double z = Lfs(base, t + 56), add = Lfs(base, group + 20);
    Stfs(base, t + 56, l.z_add_swapped ? Fadds(add, z) : Fadds(z, add));
  }
  const double one = Lfs(base, kOne);
  uint32_t outs[4];
  if (front) {
    for (int i = 0; i < 3; i++) outs[i] = frame + l.front_outs[i];
    NotedCopy(base, main, Chain(base, {block + 336, block + 80, t, world}, outs, l.front_lrs, one),
              l.side1_copy_lr);
  } else {
    for (int i = 0; i < 3; i++) outs[i] = frame + l.rear_outs[i];
    NotedCopy(base, main, Chain(base, {block + 464, block + 208, t, world}, outs, l.rear_lrs, one),
              l.side1_copy_lr);
  }
  // Records 2, 4, 5, 6, then 0; r24 the caller's until record 6's is loaded.
  const uint32_t vp = camera + 144;
  const uint32_t records[5] = {512, 992, 1232, 1472, 32};
  const uint64_t r24s[5] = {caller_r24, caller_r24, caller_r24, group + 1472, group + 1472};
  Left left;
  for (int i = 0; i < 5; i++) {
    left = PartsDrawRecord(base, calls, frame, group + records[i], vp, main, 0, r24s[i], block,
                           l.side1_draw_lrs[i]);
  }
  // Side 2: the turned world's identity, the mirror (z scaled by
  // [0x82001BE0]) and the turn by pi about y.
  const uint32_t turned_world = frame + l.turned_world, mirror = frame + l.mirror,
                 turned = frame + l.turned;
  FreshIdentity(base, frame + 80, turned_world);
  FreshIdentity(base, frame + 80, mirror);
  Stfs(base, mirror + 40, Lfs(base, kMirrorZ));
  FreshIdentity(base, frame + 80, turned);
  Stfs(base, frame + l.angle, Lfs(base, kPi));
  matrix_math::RotateY(base, turned, turned, frame + l.angle, frame - 80);
  // T's row 3 back to (0, 0, 0, 1), through the row's temporary.
  const uint32_t row = frame + l.row;
  Stfs(base, row + 12, one);
  const double zero = Lfs(base, kZero);
  Stfs(base, row + 0, zero);
  Stfs(base, row + 4, zero);
  Stfs(base, row + 8, zero);
  CopyDoublewords(base, t + 48, row, 16);
  Translate(base, block, t + 48, Lvx(base, row), group + 1728);
  {
    const double z = Lfs(base, t + 56), add = Lfs(base, group + 20);
    Stfs(base, t + 56, Fsubs(z, add));
  }
  const bool front2 = int32_t(Load(base, group + 12)) != 0;
  const uint32_t s = front2 ? block + 400 : block + 528;
  const uint32_t r = front2 ? block + 144 : block + 272;
  const uint32_t* mirror_outs = front2 ? l.mirror_front_outs : l.mirror_rear_outs;
  const uint32_t* mirror_lrs = front2 ? l.mirror_front_lrs : l.mirror_rear_lrs;
  const uint32_t* turned_outs = front2 ? l.turned_front_outs : l.turned_rear_outs;
  const uint32_t* turned_lrs = front2 ? l.turned_front_lrs : l.turned_rear_lrs;
  for (int i = 0; i < 4; i++) outs[i] = frame + mirror_outs[i];
  NotedCopy(base, main, Chain(base, {mirror, s, r, t, world}, outs, mirror_lrs, one),
            front2 ? l.mirror_front_copy_lr : l.mirror_rear_copy_lr);
  for (int i = 0; i < 4; i++) outs[i] = frame + turned_outs[i];
  NotedCopy(base, turned_world, Chain(base, {turned, s, r, t, world}, outs, turned_lrs, one),
            l.turned_copy_lr);
  // Records 2, 4, 5, 6 mirrored between two flips of the rim model's
  // culling, then record 0 turned; r28 the side's R from here on.
  const uint64_t r28 = r;
  const uint64_t r24 = group + 1472;
  material_passes::FlipCulling(base, Load(base, group + 36), frame);
  for (int i = 0; i < 4; i++) {
    left = PartsDrawRecord(base, calls, frame, group + records[i], vp, main, 0, r24, r28,
                           l.side2_draw_lrs[i]);
  }
  material_passes::FlipCulling(base, Load(base, group + 36), frame);
  return PartsDrawRecord(base, calls, frame, group + 32, vp, turned_world, 0, r24, r28,
                         l.side2_draw_lrs[4]);
}

}  // namespace

Left WheelsDrawRimA(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                    uint32_t camera, uint32_t block, uint64_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E9A28 WheelPoints::DrawRimA");
  const uint32_t frame = Frame(base, stack, kWheelsRimAFrame);
  RimAIdentities(base, frame, 80);
  return RimASides(base, calls, frame, group, camera, block, caller_r24, kRimA);
}

}  // namespace car_draw
