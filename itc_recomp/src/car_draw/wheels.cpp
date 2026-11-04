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

constexpr RimALayout kRimAMorphed = {
    kWheelsRimAMorphedFrame,
    0x820EA2BC,
    {1040, 528, 1296},
    {0x820EA324, 0x820EA344, 0x820EA364},
    {656, 1680, 784},
    {0x820EA38C, 0x820EA3AC, 0x820EA3CC},
    0x820EA3D8,
    {0x820EA3F8, 0x820EA414, 0x820EA430, 0x820EA44C, 0x820EA468},
    1872,
    1936,
    1808,
    144,
    160,
    {1424, 912, 1616, 400},
    {0x820EA630, 0x820EA654, 0x820EA674, 0x820EA694},
    {1552, 1168, 1744, 464},
    {0x820EA6C0, 0x820EA6E0, 0x820EA700, 0x820EA720},
    {592, 720, 848, 976},
    {0x820EA748, 0x820EA76C, 0x820EA78C, 0x820EA7AC},
    {1104, 1232, 1360, 1488},
    {0x820EA7D8, 0x820EA7F8, 0x820EA818, 0x820EA838},
    0x820EA6A0,
    0x820EA7B8,
    0x820EA844,
    {0x820EA864, 0x820EA87C, 0x820EA894, 0x820EA8AC, 0x820EA8CC},
    true,
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

// Record `from`'s shape (the group's word at `from` + 228) toward record
// `to`'s by the group's +16, as WheelPoints::SetUpRimA: the target's index
// to the frame's +304, the weight to +368, Morph::SetUp into the frame's
// +2000, then Model::MorphShape (its plain C++, else its original returning
// to `lr`).
void RimAMorph(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t group,
               uint32_t model, uint32_t from, uint32_t to, uint32_t lr) {
  const uint32_t target = Load(base, group + to + 228);
  const double weight = Lfs(base, group + 16);
  const uint32_t shape = Load(base, group + from + 228);
  Stfs(base, frame + 368, weight);
  Store(base, frame + 304, target);
  const uint32_t morph = frame + 2000;
  second_set::MorphSetUp(base, model, morph, shape, frame + 304, frame + 368, 1);
  const uint64_t again = Load(base, group + from + 228);
  if (!ModelMorphShape(base, calls, model, morph, again, frame)) {
    const uint64_t regs[3] = {model, morph, again};
    calls.call(kMorphShape, regs, 3, frame, lr);
  }
}

}  // namespace

Left WheelsDrawRimA(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                    uint32_t camera, uint32_t block, uint64_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E9A28 WheelPoints::DrawRimA");
  const uint32_t frame = Frame(base, stack, kWheelsRimAFrame);
  RimAIdentities(base, frame, 80);
  return RimASides(base, calls, frame, group, camera, block, caller_r24, kRimA);
}

Left WheelsDrawRimAMorphed(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t camera, uint32_t block, uint64_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820EA148 WheelPoints::DrawRimAMorphed");
  const uint32_t frame = Frame(base, stack, kWheelsRimAMorphedFrame);
  RimAIdentities(base, frame, 304);
  // Record 0 toward record 1 when record 0 has a model; record 2 toward
  // record 3 on that model regardless.
  const uint32_t model = Load(base, group + 36);
  if (model != 0) {
    RimAMorph(base, calls, frame, group, model, 32, 272, 0x820EA270);
  }
  RimAMorph(base, calls, frame, group, model, 512, 752, 0x820EA2AC);
  return RimASides(base, calls, frame, group, camera, block, caller_r24, kRimAMorphed);
}

// --- Set b's rim ---

namespace {

// The return addresses of a set b rim side's draws: the first (record 6) by
// the look's word (anything else, 1, 2), with and without the second
// matrix, then the rest of the second matrix's draws and the last.
struct RimBSites {
  uint32_t first_second[3], first_alone[3];
  uint32_t look_main, record6_second, last;
};

// A set b side's draws: record 6 (+1472) and the look the block's word at
// `look` picks (1 record 2 at +512, 2 record 4 at +992, else record 0 at
// +32) at `main`, and with the block's +592 both again at the frame's +352.
// r28 is `r28` at the first draw; with +592 the original keeps the look in
// r28 for the rest when `look_in_r28` (side 1), else r28 stays.
Left RimBDraws(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t group,
               uint32_t camera, uint32_t block, uint32_t look, uint32_t main, uint64_t r24,
               uint64_t r28, bool look_in_r28, const RimBSites& sites) {
  const uint32_t which = Load(base, block + look);
  const int site = which == 1 ? 1 : which == 2 ? 2 : 0;
  const uint32_t chosen = group + (which == 1 ? 512 : which == 2 ? 992 : 32);
  const uint32_t vp = camera + 144, record6 = group + 1472, second = frame + 352;
  if (int32_t(Load(base, block + 592)) != 0) {
    PartsDrawRecord(base, calls, frame, record6, vp, main, 0, r24, r28,
                    sites.first_second[site]);
    const uint64_t later = look_in_r28 ? uint64_t(chosen) : r28;
    PartsDrawRecord(base, calls, frame, chosen, vp, main, 0, r24, later, sites.look_main);
    PartsDrawRecord(base, calls, frame, record6, vp, second, 0, r24, later,
                    sites.record6_second);
    return PartsDrawRecord(base, calls, frame, chosen, vp, second, 0, r24, later, sites.last);
  }
  PartsDrawRecord(base, calls, frame, record6, vp, main, 0, r24, r28, sites.first_alone[site]);
  return PartsDrawRecord(base, calls, frame, chosen, vp, main, 0, r24, r28, sites.last);
}

// T's row 3 (0, 0, 0, 1) through the frame's +80, copied into T (+272),
// then the side's float4 added unless +732 is set.
void RimBRow(uint8_t* base, uint32_t frame, uint32_t block, uint32_t side, double zero,
             double one) {
  Stfs(base, frame + 80, zero);
  Stfs(base, frame + 84, zero);
  Stfs(base, frame + 88, zero);
  Stfs(base, frame + 92, one);
  CopyDoublewords(base, frame + 272, frame + 80, 16);
  Translate(base, block, frame + 272, Lvx(base, frame + 80), side);
}

}  // namespace

Left WheelsDrawRimB(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                    uint32_t camera, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820EA950 WheelPoints::DrawRimB");
  const uint32_t frame = Frame(base, stack, kWheelsRimBFrame);
  const uint32_t t = frame + 224, main = frame + 288, second = frame + 352;
  const uint32_t world = block + 16;
  // T, the side's world and the second matrix, all identities.
  FreshIdentity(base, frame + 96, t);
  FreshIdentity(base, frame + 96, main);
  FreshIdentity(base, frame + 96, second);
  const double zero = Lfs(base, kZero), one = Lfs(base, kOne);
  RimBRow(base, frame, block, group + 1712, zero, one);
  {
    const double z = Lfs(base, t + 56), add = Lfs(base, group + 20);
    Stfs(base, t + 56, Fadds(z, add));
  }
  const double milli = Lfs(base, kMilli), three = Lfs(base, kThree), shift = Lfs(base, kShift);
  // r24: 0 for the front (+12 set), 1 for the rear, through both sides.
  uint64_t r24;
  if (int32_t(Load(base, group + 12)) != 0) {
    r24 = 0;
    static constexpr uint32_t kLrs[] = {0x820EAB3C, 0x820EAB5C, 0x820EAB80};
    const uint32_t outs[] = {frame + 1248, frame + 1888, frame + 608};
    NotedCopy(base, main, Chain(base, {block + 336, block + 80, t, world}, outs, kLrs, one),
              0x820EACAC);
  } else {
    r24 = 1;
    static constexpr uint32_t kLrs[] = {0x820EABB8, 0x820EABD8, 0x820EABFC};
    const uint32_t outs[] = {frame + 1504, frame + 736, frame + 2144};
    NotedCopy(base, main, Chain(base, {block + 464, block + 208, t, world}, outs, kLrs, one),
              0x820EAC08);
    if (int32_t(Load(base, block + 592)) != 0) {
      ShiftX(base, frame, group, t + 48, shift, three, milli);
      static constexpr uint32_t kSecondLrs[] = {0x820EAC60, 0x820EAC80, 0x820EACA0};
      const uint32_t second_outs[] = {frame + 864, frame + 1632, frame + 992};
      NotedCopy(base, second,
                Chain(base, {block + 464, block + 208, t, world}, second_outs, kSecondLrs, one),
                0x820EACAC);
    }
  }
  // r28 the camera at side 1's first draw.
  static constexpr RimBSites kSide1 = {{0x820EACF4, 0x820EAD44, 0x820EAD38},
                                       {0x820EAD60, 0x820EAD78, 0x820EAD6C},
                                       0x820EAD10,
                                       0x820EAD28,
                                       0x820EAD90};
  RimBDraws(base, calls, frame, group, camera, block, 596 + 8 * uint32_t(r24), main, r24, camera,
            true, kSide1);
  // Side 2: the turned world (+96), the mirror (+2336, z scaled by
  // [0x82001BE0]) and the turn by pi about y (+1696).
  const uint32_t turned_world = frame + 96, mirror = frame + 2336, turned = frame + 1696;
  FreshIdentity(base, frame + 160, turned_world);
  FreshIdentity(base, frame + 160, mirror);
  Stfs(base, mirror + 40, Lfs(base, kMirrorZ));
  FreshIdentity(base, frame + 160, turned);
  Stfs(base, frame + 80, Lfs(base, kPi));
  matrix_math::RotateY(base, turned, turned, frame + 80, frame - 80);
  RimBRow(base, frame, block, group + 1728, zero, one);
  {
    const double z = Lfs(base, t + 56), add = Lfs(base, group + 20);
    Stfs(base, t + 56, Fsubs(z, add));
  }
  // The mirrored product goes into side 1's world (+288), never drawn.
  uint32_t s;
  if (int32_t(Load(base, group + 12)) != 0) {
    s = block + 400;
    const uint32_t r = block + 144;
    static constexpr uint32_t kMirrorLrs[] = {0x820EAF50, 0x820EAF74, 0x820EAF94, 0x820EAFB4};
    const uint32_t mirror_outs[] = {frame + 2016, frame + 1120, frame + 1760, frame + 416};
    NotedCopy(base, main, Chain(base, {mirror, s, r, t, world}, mirror_outs, kMirrorLrs, one),
              0x820EAFC0);
    static constexpr uint32_t kTurnedLrs[] = {0x820EAFE0, 0x820EB000, 0x820EB020, 0x820EB040};
    const uint32_t turned_outs[] = {frame + 2272, frame + 1376, frame + 480, frame + 544};
    NotedCopy(base, turned_world,
              Chain(base, {turned, s, r, t, world}, turned_outs, kTurnedLrs, one), 0x820EB22C);
  } else {
    s = block + 528;
    const uint32_t r = block + 272;
    static constexpr uint32_t kMirrorLrs[] = {0x820EB06C, 0x820EB090, 0x820EB0B0, 0x820EB0D0};
    const uint32_t mirror_outs[] = {frame + 672, frame + 800, frame + 928, frame + 1056};
    NotedCopy(base, main, Chain(base, {mirror, s, r, t, world}, mirror_outs, kMirrorLrs, one),
              0x820EB0DC);
    static constexpr uint32_t kTurnedLrs[] = {0x820EB0FC, 0x820EB11C, 0x820EB13C, 0x820EB15C};
    const uint32_t turned_outs[] = {frame + 1184, frame + 1312, frame + 1440, frame + 1568};
    NotedCopy(base, turned_world,
              Chain(base, {turned, s, r, t, world}, turned_outs, kTurnedLrs, one), 0x820EB168);
    if (int32_t(Load(base, block + 592)) != 0) {
      ShiftX(base, frame, group, t + 48, shift, three, milli);
      static constexpr uint32_t kSecondLrs[] = {0x820EB1C0, 0x820EB1E0, 0x820EB200, 0x820EB220};
      const uint32_t second_outs[] = {frame + 1824, frame + 1952, frame + 2080, frame + 2208};
      NotedCopy(base, second,
                Chain(base, {turned, s, r, t, world}, second_outs, kSecondLrs, one),
                0x820EB22C);
    }
  }
  // r28 the side's S at every side 2 draw.
  static constexpr RimBSites kSide2 = {{0x820EB26C, 0x820EB2BC, 0x820EB2B0},
                                       {0x820EB2D8, 0x820EB2F0, 0x820EB2E4},
                                       0x820EB288,
                                       0x820EB2A0,
                                       0x820EB308};
  return RimBDraws(base, calls, frame, group, camera, block, 600 + 8 * uint32_t(r24),
                   turned_world, r24, s, false, kSide2);
}

// --- The brakes ---

namespace {

constexpr uint32_t kDegrees = 0x82002948;   // pi / 180
constexpr uint32_t kHalfTurn = 0x82002C34;  // 180.0, side 2's turn

// A side's two products, front or rear: the spinning Z x S x R x T x world
// and the still Z x R x T x world, their outputs and return addresses.
struct BrakesChains {
  uint32_t spin_outs[4], spin_lrs[4], still_outs[3], still_lrs[3];
  uint32_t spin_copy_lr, still_copy_lr;
};

// Where a brake draw keeps its matrices (DrawBrakes' and
// DrawBrakesMorphed's frames differ only in offsets): the six fresh
// identities' destinations in their order, the side matrices' (spinning,
// still for each side) and the chains.
struct BrakesLayout {
  uint32_t fresh[6];
  uint32_t spin1, still1, spin2, still2;
  BrakesChains front1, rear1, front2, rear2;
};

constexpr BrakesLayout kBrakes = {
    {2144, 2016, 2400, 2272, 224, 288},
    2144,
    2016,
    2400,
    2272,
    {{1312, 544, 1952, 672},
     {0x820EB6B0, 0x820EB6D4, 0x820EB6F4, 0x820EB718},
     {1568, 800, 2208},
     {0x820EB744, 0x820EB764, 0x820EB784},
     0x820EB724,
     0x820EB790},
    {{2336, 1440, 480, 608},
     {0x820EB96C, 0x820EB990, 0x820EB9B0, 0x820EB9D4},
     {736, 864, 992},
     {0x820EBA00, 0x820EBA20, 0x820EBA40},
     0x820EB9E0,
     0x820EBA4C},
    {{928, 1696, 1056, 2080},
     {0x820EB850, 0x820EB874, 0x820EB894, 0x820EB8B4},
     {1184, 1824, 416},
     {0x820EB8E0, 0x820EB900, 0x820EB920},
     0x820EB8C0,
     0x820EBBE8},
    {{1120, 1248, 1376, 1504},
     {0x820EBB0C, 0x820EBB30, 0x820EBB50, 0x820EBB70},
     {1632, 1760, 1888},
     {0x820EBB9C, 0x820EBBBC, 0x820EBBDC},
     0x820EBB7C,
     0x820EBBE8},
};

// One side's products from Z (the frame's +96) into the layout's spinning
// and still matrices.
void BrakesSide(uint8_t* base, uint32_t frame, uint32_t s, uint32_t r, uint32_t t,
                uint32_t world, uint32_t spin, uint32_t still, const BrakesChains& c,
                double one) {
  const uint32_t z = frame + 96;
  uint32_t outs[4];
  for (int i = 0; i < 4; i++) outs[i] = frame + c.spin_outs[i];
  NotedCopy(base, frame + spin, Chain(base, {z, s, r, t, world}, outs, c.spin_lrs, one),
            c.spin_copy_lr);
  for (int i = 0; i < 3; i++) outs[i] = frame + c.still_outs[i];
  NotedCopy(base, frame + still, Chain(base, {z, r, t, world}, outs, c.still_lrs, one),
            c.still_copy_lr);
}

// Both sides' matrices (see car_draw.h): the identities, T (+224) and T'
// (+288) from +2912 and +2928 unless the block's +732 is set, Z by the
// wheels record's +100 (rear +104) in degrees, Z' = RotY turned by (that -
// 180) degrees, and each side's products.
void BrakesMatrices(uint8_t* base, uint32_t frame, uint32_t group, uint32_t block,
                    const BrakesLayout& l) {
  for (const uint32_t to : l.fresh) {
    FreshIdentity(base, frame + 96, frame + to);
  }
  FreshIdentity(base, frame + 160, frame + 96);
  const uint32_t t = frame + 224, t2 = frame + 288, world = block + 16;
  const simde__m128 w = VectorOf(0, 0, 0, kOneBits);
  Translate(base, block, t + 48, w, group + 2912);
  Translate(base, block, t2 + 48, w, group + 2928);
  const bool front = int32_t(Load(base, group + 12)) != 0;
  const uint32_t angle_at = front ? 100 : 104;
  const uint32_t z = frame + 96, angle = frame + 80, turn = frame - 80;
  const double degrees = Lfs(base, kDegrees);
  Stfs(base, angle, Fmuls(Lfs(base, Load(base, group + 8) + angle_at), degrees));
  matrix_math::RotateZ(base, z, z, angle, turn);
  const double one = Lfs(base, kOne);
  if (front) {
    BrakesSide(base, frame, block + 336, block + 80, t, world, l.spin1, l.still1, l.front1, one);
  } else {
    BrakesSide(base, frame, block + 464, block + 208, t, world, l.spin1, l.still1, l.rear1, one);
  }
  FreshIdentity(base, frame + 160, z);
  Stfs(base, angle, Lfs(base, kPi));
  matrix_math::RotateY(base, z, z, angle, turn);
  {
    const double f13 = Lfs(base, Load(base, group + 8) + angle_at);
    Stfs(base, angle, Fmuls(Fsubs(f13, Lfs(base, kHalfTurn)), degrees));
  }
  matrix_math::RotateZ(base, z, z, angle, turn);
  if (front) {
    BrakesSide(base, frame, block + 400, block + 144, t2, world, l.spin2, l.still2, l.front2, one);
  } else {
    BrakesSide(base, frame, block + 528, block + 272, t2, world, l.spin2, l.still2, l.rear2, one);
  }
}

// A side's discs and calipers: spinning, record 4 (+992) when the wheels
// record's +68 is below 4, else record 6 (+1472); still, record 0 (+32) when
// +68 and +84 are 0, else record 2 (+512); still, record 8 (+1952) when +68
// is 0 and +88 isn't 1. The wheels record read again before each test, as
// the original does.
Left BrakesDraws(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t group,
                 uint32_t vp, uint32_t spin, uint32_t still, uint64_t r24, uint64_t r28,
                 const uint32_t lrs[3]) {
  const auto wheels = [&] { return Load(base, group + 8); };
  const uint32_t first = int32_t(Load(base, wheels() + 68)) < 4 ? 992 : 1472;
  PartsDrawRecord(base, calls, frame, group + first, vp, spin, 0, r24, r28, lrs[0]);
  uint32_t second = 512;
  if (int32_t(Load(base, wheels() + 68)) == 0 && int32_t(Load(base, wheels() + 84)) == 0) {
    second = 32;
  }
  Left left =
      PartsDrawRecord(base, calls, frame, group + second, vp, still, 0, r24, r28, lrs[1]);
  if (int32_t(Load(base, wheels() + 68)) == 0 && int32_t(Load(base, wheels() + 88)) != 1) {
    left = PartsDrawRecord(base, calls, frame, group + 1952, vp, still, 0, r24, r28, lrs[2]);
  }
  return left;
}

// Record `from`'s shape (the group's word at `from` + 228) toward record
// `to`'s: the target's index to the frame's +224 (the weight already at
// +160), Morph::SetUp into the frame's +288, then Model::MorphShape (its
// plain C++, else its original returning to `lr`).
void BrakesMorph(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t group,
                 uint32_t model, uint32_t from, uint32_t to, uint32_t lr) {
  Store(base, frame + 224, Load(base, group + to + 228));
  const uint32_t morph = frame + 288;
  second_set::MorphSetUp(base, model, morph, Load(base, group + from + 228), frame + 224,
                         frame + 160, 1);
  const uint64_t shape = Load(base, group + from + 228);
  if (!ModelMorphShape(base, calls, model, morph, shape, frame)) {
    const uint64_t regs[3] = {model, morph, shape};
    calls.call(kMorphShape, regs, 3, frame, lr);
  }
}

}  // namespace

Left WheelsDrawBrakes(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                      uint32_t camera, uint32_t block, uint64_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820EB388 WheelPoints::DrawBrakes");
  const uint32_t frame = Frame(base, stack, kWheelsBrakesFrame);
  BrakesMatrices(base, frame, group, block, kBrakes);
  // The morphs when the wheels record's +8 is set (never in play: its caller
  // runs only when it's 0), all on record 0's model, the weight the group's
  // +16 (to the frame's +160 once).
  if (int32_t(Load(base, Load(base, group + 8) + 8)) != 0) {
    const uint32_t model = Load(base, group + 36);
    Stfs(base, frame + 160, Lfs(base, group + 16));
    BrakesMorph(base, calls, frame, group, model, 32, 272, 0x820EBC38);
    BrakesMorph(base, calls, frame, group, model, 512, 752, 0x820EBC6C);
    BrakesMorph(base, calls, frame, group, model, 1952, 2192, 0x820EBCA0);
    BrakesMorph(base, calls, frame, group, model, 992, 1232, 0x820EBCD4);
    BrakesMorph(base, calls, frame, group, model, 1472, 1712, 0x820EBD08);
  }
  // r24 the caller's, r28 the block at every draw; record 10's materials'
  // alpha by the block's +612 + 8 x rear (side 2 +616) before it's drawn.
  const uint32_t vp = camera + 144;
  const uint32_t rear = int32_t(Load(base, group + 12)) != 0 ? 0 : 8;
  const uint64_t r24 = caller_r24, r28 = block;
  static constexpr uint32_t kSide1[] = {0x820EBD38, 0x820EBD70, 0x820EBDA4};
  static constexpr uint32_t kSide2[] = {0x820EBE04, 0x820EBE3C, 0x820EBE70};
  BrakesDraws(base, calls, frame, group, vp, frame + kBrakes.spin1, frame + kBrakes.still1, r24,
              r28, kSide1);
  material_passes::SetAlpha(base, Load(base, group + 2436), Load(base, group + 2660),
                            Lfs(base, block + 612 + rear), frame);
  PartsDrawRecord(base, calls, frame, group + 2432, vp, frame + kBrakes.spin1, 0, r24, r28,
                  0x820EBDD8);
  BrakesDraws(base, calls, frame, group, vp, frame + kBrakes.spin2, frame + kBrakes.still2, r24,
              r28, kSide2);
  material_passes::SetAlpha(base, Load(base, group + 2436), Load(base, group + 2660),
                            Lfs(base, block + 616 + rear), frame);
  return PartsDrawRecord(base, calls, frame, group + 2432, vp, frame + kBrakes.spin2, 0, r24,
                         r28, 0x820EBEA0);
}

}  // namespace car_draw
