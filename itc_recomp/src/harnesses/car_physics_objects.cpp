// The vehicle's physics objects (src/car_physics/vehicle_physics.cpp)
// against the recompiled functions, part of the car_physics harness: a
// region allocated once from the allocation layer holds a fake collision
// world (its objects, pairs and lists; the world's pointer 0x826C2D88 put
// back after), a rigid body, real boxes (built by the native
// BoxShape::BoxShape, the same on both sides: inputs, not under test), a
// vehicle's pointers and a ground query's records. The originals run with
// the native flags of their callees off (the car physics, the collision and
// collision world, the vector library, the matrix math, the vehicle
// accessors, the C runtime's math). Blocks the functions allocate or free
// come from the layer on both sides (only with --native_heap, the adapters'
// condition), at different addresses: words equal to them are compared as
// their number. Two groups where floats are worked: numbers only (below
// 10^6) and NaNs and infinities anywhere, compared with harness::Outputs
// (floats only for the fields loaded and stored as floats).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_physics/car_physics.h"
#include "collision/collision.h"
#include "hooks/heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cmath>
#include <iterator>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_car_physics);
ITC_FLAG_DECLARE(bool, native_matrix_math);
ITC_FLAG_DECLARE(bool, native_vector_library);
ITC_FLAG_DECLARE(bool, native_vehicle_accessors);
ITC_FLAG_DECLARE(bool, native_math);
ITC_FLAG_DECLARE(bool, native_collision);
ITC_FLAG_DECLARE(bool, native_collision_world);

namespace harness {

namespace {

namespace cp = car_physics;
namespace co = collision;

constexpr uint32_t kWorldPointer = 0x826C2D88;
constexpr uint32_t kGroundCallback = 0x820FEAB0;

// The region's layout.
constexpr uint32_t kRegionBytes = 0x1800;
constexpr uint32_t kWorld = 0x0;       // 72 bytes
constexpr uint32_t kObjects = 0x100;   // up to 4 objects, 320 bytes each
constexpr uint32_t kPairs = 0x600;     // up to 16 pairs, 32 bytes each
constexpr uint32_t kWorldEnd = 0x800;  // the world's part compared
constexpr uint32_t kBody = 0x800;      // 528 bytes
constexpr uint32_t kTemplate = 0xA80;  // a box, 496 bytes
constexpr uint32_t kCopyA = 0xC80, kCopyB = 0xE80;  // boxes
constexpr uint32_t kResult = 0x1080, kPoint = 0x10C0, kPosition = 0x1100;
constexpr uint32_t kVehicle = 0x1200;  // its +336, +340, +344 compared

template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  bool* flags[] = {&ITC_FLAG(native_car_physics),     &ITC_FLAG(native_matrix_math),
                   &ITC_FLAG(native_vector_library),  &ITC_FLAG(native_vehicle_accessors),
                   &ITC_FLAG(native_math),            &ITC_FLAG(native_collision),
                   &ITC_FLAG(native_collision_world)};
  bool saved[std::size(flags)];
  for (size_t i = 0; i < std::size(flags); i++) {
    saved[i] = *flags[i];
    *flags[i] = false;
  }
  recompiled(ctx, base);
  for (size_t i = 0; i < std::size(flags); i++) {
    *flags[i] = saved[i];
  }
}

// The register steps, as the adapters (src/hooks/car_physics.cpp).
void NativeSetFloat48(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cp::RigidBodySetFloat48(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeCallback(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!cp::CollisionWorldCallback(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32, &r3)) {
    __imp__sub_820B8798(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
void NativeResetTransform(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t object = ctx.r3.u32;
  if (!cp::CollisionObjectResetTransform(base, object, ctx.r1.u32 - 112)) {
    __imp__sub_820B7998(ctx, base);
    return;
  }
  ctx.r3.u64 = object + 32;
}
// Init has no adapter (its only caller is native): the original where it
// wouldn't be native.
void NativeInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!cp::CollisionObjectInitFits(base, ctx.r3.u32, ctx.r5.u32, ctx.r7.u32)) {
    __imp__sub_820B7A00(ctx, base);
    return;
  }
  cp::CollisionObjectInit(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                          ctx.r1.u32 - 112);
}
void NativeAddObject(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t object = 0;
  if (!cp::CollisionWorldAddObject(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 128,
                                   &object)) {
    __imp__sub_820B9570(ctx, base);
    return;
  }
  ctx.r3.u64 = object;
}
void NativeProbeGround(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!cp::CollisionWorldCallbackKnown(base)) {
    __imp__sub_820B4420(ctx, base);
    return;
  }
  double f1 = 0.0;
  ctx.r3.u64 = cp::VehicleProbeGround(base, ctx.r3.u32, ctx.r1.u32 - 304, &f1);
  ctx.f1.f64 = f1;
}
void NativeUnflip(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!cp::VehicleUnflip(base, ctx.r3.u32, ctx.r1.u32 - 384, &r3)) {
    __imp__sub_820AFE90(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
void NativeRefreshFromBody(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!cp::CollisionWorldCallbackKnown(base)) {
    __imp__sub_820B62E8(ctx, base);
    return;
  }
  ctx.r3.u64 = cp::VehicleRefreshFromBody(base, ctx.r3.u32, ctx.r1.u32 - 96);
}
// As the adapter: UpdateSteering's and UpdateTorqueSplit's originals where
// they refuse.
void NativePhysicsStep(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!cp::CollisionWorldCallbackKnown(base)) {
    __imp__sub_820B6498(ctx, base);
    return;
  }
  const uint32_t vehicle = ctx.r3.u32;
  const uint32_t frame = ctx.r1.u32 - 112;
  auto original = [&](uint32_t address) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.r3.u64 = vehicle;
    if (address == 0x820B18B8) {
      __imp__sub_820B18B8(ctx, base);
    } else {
      __imp__sub_820B2100(ctx, base);
    }
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
  };
  ctx.r3.u64 = cp::VehiclePhysicsStep(base, vehicle, ctx.f1.f64, frame, original);
}
void NativeSetUpPhysics(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  uint32_t r3 = 0;
  if (!cp::VehicleSetUpPhysics(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                               ctx.r1.u32 - 208, &f1, &r3)) {
    __imp__sub_820B5670(ctx, base);
    return;
  }
  ctx.f1.f64 = f1;
  ctx.r3.u64 = r3;
}
void NativeResetPhysics(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  uint32_t r3 = 0;
  if (!cp::VehicleResetPhysics(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 208, &f1,
                               &r3)) {
    __imp__sub_820B5B08(ctx, base);
    return;
  }
  ctx.f1.f64 = f1;
  ctx.r3.u64 = r3;
}
void NativeDestroyPhysics(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!cp::VehicleDestroyPhysics(base, ctx.r3.u32, &r3)) {
    __imp__sub_820B0E70(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}

constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001,
                                   0x7F800000, 0xFF800000, 0x00000001, 0x807FFFFF,
                                   0x00000000, 0x80000000, 0x7F7FFFFF, 0x3F800000};

uint32_t AnyWord(Random& r) {
  switch (r.Below(6)) {
    case 0:
      return kOddFloats[r.Below(std::size(kOddFloats))];
    case 1:
      return uint32_t(r.Next());
    default:
      return std::bit_cast<uint32_t>(float(int32_t(r.Below(1 << 20)) - (1 << 19)) / 8192.0f);
  }
}
uint32_t NumberWord(Random& r) {
  uint32_t word;
  do {
    word = AnyWord(r);
  } while (!(std::fabs(std::bit_cast<float>(word)) < 1e6f));
  return word;
}

enum Group { kNumbers, kNaNs };
const char* const kGroupNames[] = {"", ", NaNs and infinities anywhere"};

// The blocks of a case: those given (allocated before the call) and those
// made (new blocks the outputs point at), compared by their number.
struct Blocks {
  std::vector<uint32_t> given, made;
  uint32_t Name(uint32_t w) const {
    for (size_t i = 0; i < given.size(); i++) {
      if (w == given[i]) {
        return uint32_t(0xB10C0000u + i);
      }
    }
    for (size_t i = 0; i < made.size(); i++) {
      if (w == made[i]) {
        return uint32_t(0x0E550000u + i);
      }
    }
    return w;
  }
  void Note(uint32_t w) {
    if (w != 0 && memory::IsHeapBlock(w) &&
        std::find(given.begin(), given.end(), w) == given.end() &&
        std::find(made.begin(), made.end(), w) == made.end()) {
      made.push_back(w);
    }
  }
  void FreeAll() {
    for (uint32_t b : given) {
      if (memory::IsHeapBlock(b)) {
        memory::Free(b);
      }
    }
    for (uint32_t b : made) {
      if (memory::IsHeapBlock(b)) {
        memory::Free(b);
      }
    }
  }
};

// A box's words: the vtable, halfwords +112 and +114 and the cache words
// +160 to +175 exactly, the rest floats.
void BoxOutputs(Outputs& out, uint8_t* base, uint32_t box, const Blocks& blocks) {
  for (uint32_t i = 0; i < 496; i += 4) {
    const uint32_t w = guest::LoadU32(base, box + i);
    if (i == 0 || i == 112 || (i >= 160 && i < 176)) {
      out.Word(blocks.Name(w));
    } else {
      out.Float(w);
    }
  }
}

// A collision object's words: floats where the code loads and stores floats
// (the shape record's bounds, matrix and +112, at +48 to +147; +160 to +191,
// +208 to +227, +304 to +319), the rest exactly (pointers by their number).
void ObjectOutputs(Outputs& out, uint8_t* base, uint32_t object, const Blocks& blocks) {
  for (uint32_t i = 0; i < 320; i += 4) {
    const uint32_t w = guest::LoadU32(base, object + i);
    const bool is_float = (i >= 48 && i < 148) || (i >= 160 && i < 192) ||
                          (i >= 208 && i < 228) || i >= 304;
    if (is_float) {
      out.Float(w);
    } else {
      out.Word(blocks.Name(w));
    }
  }
}

// The fake world: `objects` (2 to 4) objects, some active in a random
// order, one free at least with `keep_one_free`, pairs made by
// the recompiled AddPairs (the same on both sides). The objects' other words
// from `word`.
void BuildWorld(Context& h, uint32_t region, Random& fill, Group group, bool keep_one_free) {
  uint8_t* base = h.base;
  auto word = [group](Random& r) { return group == kNaNs ? AnyWord(r) : NumberWord(r); };
  for (uint32_t i = kWorld; i < kWorldEnd; i += 4) {
    guest::StoreU32(base, region + i, word(fill));
  }
  const uint32_t world = region + kWorld;
  const uint32_t objects = 2 + fill.Below(3);
  auto object_at = [region](uint32_t i) { return region + kObjects + 320 * i; };
  guest::StoreU32(base, world + 0, 0);
  guest::StoreU32(base, world + 8, kGroundCallback);
  guest::StoreU32(base, world + 12, objects);
  guest::StoreU32(base, world + 16, object_at(0));
  guest::StoreU32(base, world + 32, 16);
  guest::StoreU32(base, world + 36, region + kPairs);
  guest::StoreU32(base, world + 40, 0);
  guest::StoreU32(base, kWorldPointer, world);
  uint32_t order[4];
  for (uint32_t i = 0; i < objects; i++) {
    order[i] = i;
  }
  for (uint32_t i = objects - 1; i > 0; i--) {
    std::swap(order[i], order[fill.Below(i + 1)]);
  }
  uint32_t heads[2] = {0, 0};
  for (uint32_t i = 0; i < objects; i++) {
    const uint32_t object = object_at(order[i]);
    guest::StoreU32(base, object + 244, fill.Below(16));
    guest::StoreU32(base, object + 248, fill.Below(16) | (fill.Below(2) == 0 ? 0x01000000 : 0));
    guest::StoreU32(base, object + 268, fill.Below(3));
    guest::StoreU32(base, object + 264, 0);
    guest::StoreU32(base, object + 32 + 4, 0);
    guest::StoreU32(base, object + 32 + 8, 0);
    const bool free_one = (keep_one_free && i == 0) || fill.Below(3) == 0;
    uint32_t& list = heads[free_one ? 1 : 0];
    guest::StoreU32(base, object + 228, list);
    guest::StoreU32(base, object + 232, 0);
    if (list != 0) {
      guest::StoreU32(base, list + 232, object);
    }
    list = object;
  }
  guest::StoreU32(base, world + 20, heads[0]);
  guest::StoreU32(base, world + 24, heads[1]);
  guest::StoreU32(base, world + 28, fill.Below(5));
  const uint32_t adds = fill.Below(5);
  for (uint32_t i = 0; i < adds; i++) {
    PPCContext t = CallContext(h);
    t.r3.u64 = object_at(fill.Below(objects));
    t.r4.u64 = fill.Below(16);
    Original<__imp__sub_820B94B0>(t, base);
  }
}

// A box at `box` from two random corners (min below max on each axis most
// of the time), built by the native constructor with its frame under the
// call's stack.
void BuildBox(Context& h, uint32_t box, Random& r, Group group) {
  const uint32_t corners = CallContext(h).r1.u32 - 0x800;
  for (uint32_t i = 0; i < 3; i++) {
    const float a = std::bit_cast<float>(NumberWord(r));
    const float extent = std::fabs(std::bit_cast<float>(NumberWord(r)));
    guest::StoreFloat(h.base, corners + 4 * i, a);
    guest::StoreFloat(h.base, corners + 16 + 4 * i, a + extent);
  }
  guest::StoreU32(h.base, corners + 12, NumberWord(r));
  guest::StoreU32(h.base, corners + 28, NumberWord(r));
  if (group == kNaNs && r.Below(3) == 0) {
    guest::StoreU32(h.base, corners + 4 * r.Below(8), AnyWord(r));
  }
  co::BoxConstruct(h.base, box, corners, corners + 16, corners - 0x400);
}

void FillStack(Context& h, Random& r) {
  const uint32_t r1 = CallContext(h).r1.u32;
  for (uint32_t a = r1 - 0x1000; a < r1; a += 4) {
    guest::StoreU32(h.base, a, uint32_t(r.Next()));
  }
}

void SetFloat48Cases(Context& h, uint32_t region) {
  Results results("RigidBody::SetFloat48 (sub_8259D5A8)");
  for (uint64_t seed = 0; seed < 200; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 31 + 3);
      for (uint32_t i = 0; i < 0x100; i += 4) {
        guest::StoreU32(h.base, region + kBody + i, AnyWord(r));
      }
      PPCContext t = CallContext(h);
      t.fpscr.disableFlushModeUnconditional();
      t.r3.u64 = region + kBody;
      t.r4.u64 = region + kBody + 0x80 + 4 * r.Below(16);
      (v == 0 ? Original<__imp__sub_8259D5A8> : NativeSetFloat48)(t, h.base);
      out[v].Bytes(Read(h.base, region + kBody, 0x100));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// The ground callback's cases: the world's +8 the ground callback, the
// track position 0 or one whose last point (+16, +24) is the point's x and
// z (so the mesh isn't read: its kind +80 used, its normal +48), its kind
// one the height offset applies to some of the time. Compared: the
// records, the ground's temporaries (the point raised, at its frame + 80)
// and r3.
void CallbackCases(Context& h, uint32_t region) {
  for (Group group : {kNumbers, kNaNs}) {
    Results results(std::string("CollisionWorld::Callback (sub_820B8798)") + kGroupNames[group]);
    for (uint64_t seed = 0; seed < 200; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 41 + 7 + 1000 * uint64_t(group));
        auto word = [group](Random& x) { return group == kNaNs ? AnyWord(x) : NumberWord(x); };
        FillStack(h, r);
        BuildWorld(h, region, r, kNumbers, false);
        for (uint32_t i = kResult; i < kVehicle; i += 4) {
          guest::StoreU32(h.base, region + i, word(r));
        }
        // The point's x and z numbers (a NaN would take the mesh's path).
        guest::StoreU32(h.base, region + kPoint + 0, NumberWord(r));
        guest::StoreU32(h.base, region + kPoint + 8, NumberWord(r));
        const bool no_position = r.Below(4) == 0;
        const uint32_t position = region + kPosition;
        guest::StoreU32(h.base, position + 16, guest::LoadU32(h.base, region + kPoint + 0));
        guest::StoreU32(h.base, position + 24, guest::LoadU32(h.base, region + kPoint + 8));
        const uint32_t kinds[] = {0, 4, 20, 7, 0xFFFF, 0x8001};
        guest::StoreU16(h.base, position + 80, uint16_t(kinds[r.Below(std::size(kinds))]));
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = region + kResult;
        t.r4.u64 = region + kPoint;
        t.r5.u64 = no_position ? 0 : position;
        (v == 0 ? Original<__imp__sub_820B8798> : NativeCallback)(t, h.base);
        // The result: +0 the distance, +16 and +32 vectors (floats), +48 and
        // +52 words; the position's words as floats but +80 (its kind).
        out[v].Floats(h.base, region + kResult, 12);
        out[v].Word(guest::LoadU32(h.base, region + kResult + 48));
        out[v].Word(guest::LoadU32(h.base, region + kResult + 52));
        out[v].Floats(h.base, region + kResult + 56, 2);
        out[v].Floats(h.base, region + kPoint, 16);
        out[v].Floats(h.base, position, 20);
        out[v].Word(guest::LoadU32(h.base, position + 80));
        out[v].Floats(h.base, position + 84, 3);
        const uint32_t ground_frame = CallContext(h).r1.u32 - 96 - 144;
        out[v].Floats(h.base, ground_frame + 80, 4);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// ResetTransform's cases: an object with a body (its matrix +288 numbers)
// and a shape record of three real boxes (the template and two placed
// copies), +112 0.0 or a number (the template lowered on the stack).
// Compared: the object, the boxes, the stack copy of the lowered template
// (Place's frame + 80) and r3.
void ResetTransformCases(Context& h, uint32_t region) {
  for (Group group : {kNumbers, kNaNs}) {
    Results results(std::string("CollisionObject::ResetTransform (sub_820B7998)") +
                    kGroupNames[group]);
    for (uint64_t seed = 0; seed < 200; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 43 + 11 + 1000 * uint64_t(group));
        auto word = [group](Random& x) { return group == kNaNs ? AnyWord(x) : NumberWord(x); };
        const uint32_t object = region + kObjects;
        for (uint32_t i = 0; i < 320; i += 4) {
          guest::StoreU32(h.base, object + i, word(r));
        }
        for (uint32_t i = 0; i < 528; i += 4) {
          guest::StoreU32(h.base, region + kBody + i, word(r));
        }
        BuildBox(h, region + kTemplate, r, group);
        BuildBox(h, region + kCopyA, r, group);
        BuildBox(h, region + kCopyB, r, group);
        FillStack(h, r);
        guest::StoreU32(h.base, object + 16, region + kBody);
        guest::StoreU32(h.base, object + 32, region + kTemplate);
        guest::StoreU32(h.base, object + 36, region + kCopyA);
        guest::StoreU32(h.base, object + 40, region + kCopyB);
        if (r.Below(2) == 0) {
          guest::StoreU32(h.base, object + 32 + 112, 0);
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = object;
        (v == 0 ? Original<__imp__sub_820B7998> : NativeResetTransform)(t, h.base);
        const Blocks none;
        ObjectOutputs(out[v], h.base, object, none);
        for (uint32_t box : {kTemplate, kCopyA, kCopyB}) {
          BoxOutputs(out[v], h.base, region + box, none);
        }
        BoxOutputs(out[v], h.base, CallContext(h).r1.u32 - 112 - 608 + 80, none);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// Init's and AddObject's cases: the fake world (one object free at least),
// the template a real box, the body's words, the free object's record
// copies 0 or boxes from the layer (deleted by CollisionShape::SetUp), the
// world's +0 0 (the clones from operator new) or, for Init, a given box.
// Compared: the world (objects as ObjectOutputs, the rest exactly), the
// template, the clones (the record copies the layer made), r3 for
// AddObject.
void AddObjectCases(Context& h, uint32_t region, bool init) {
  const char* name =
      init ? "CollisionObject::Init (sub_820B7A00)" : "CollisionWorld::AddObject (sub_820B9570)";
  for (Group group : {kNumbers, kNaNs}) {
    Results results(std::string(name) + kGroupNames[group]);
    for (uint64_t seed = 0; seed < 150; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 47 + 13 + 1000 * uint64_t(group) + (init ? 500 : 0));
        auto word = [group](Random& x) { return group == kNaNs ? AnyWord(x) : NumberWord(x); };
        BuildWorld(h, region, r, group, true);
        for (uint32_t i = 0; i < 528; i += 4) {
          guest::StoreU32(h.base, region + kBody + i, word(r));
        }
        BuildBox(h, region + kTemplate, r, group);
        FillStack(h, r);
        const uint32_t world = region + kWorld;
        const uint32_t object = guest::LoadU32(h.base, world + 24);
        Blocks blocks;
        for (uint32_t at : {36u, 40u}) {
          if (r.Below(3) == 0) {
            const uint32_t b = memory::Alloc(496, 0);
            BuildBox(h, b, r, kNumbers);
            blocks.given.push_back(b);
            guest::StoreU32(h.base, object + at, b);
          }
        }
        const bool given_memory = init && r.Below(4) == 0;
        if (given_memory) {
          BuildBox(h, region + kCopyA, r, kNumbers);
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        const uint32_t groups = r.Below(16) | (r.Below(3) == 0 ? 0x07000000 : 0);
        if (init) {
          t.r3.u64 = object;
          t.r4.u64 = region + kBody;
          t.r5.u64 = region + kTemplate;
          t.r6.u64 = groups;
          t.r7.u64 = given_memory ? region + kCopyA : 0;
          (v == 0 ? Original<__imp__sub_820B7A00> : NativeInit)(t, h.base);
        } else {
          t.r3.u64 = region + kBody;
          t.r4.u64 = region + kTemplate;
          t.r5.u64 = groups;
          (v == 0 ? Original<__imp__sub_820B9570> : NativeAddObject)(t, h.base);
        }
        blocks.Note(guest::LoadU32(h.base, object + 36));
        blocks.Note(guest::LoadU32(h.base, object + 40));
        Outputs& o = out[v];
        for (uint32_t i = kWorld; i < kObjects; i += 4) {
          o.Word(blocks.Name(guest::LoadU32(h.base, region + i)));
        }
        for (uint32_t i = 0; i < 4; i++) {
          ObjectOutputs(o, h.base, region + kObjects + 320 * i, blocks);
        }
        for (uint32_t i = kPairs; i < kWorldEnd; i += 4) {
          o.Word(blocks.Name(guest::LoadU32(h.base, region + i)));
        }
        BoxOutputs(o, h.base, region + kTemplate, blocks);
        for (uint32_t b : blocks.given) {
          o.Word(memory::IsHeapBlock(b) ? 1 : 0);
        }
        for (uint32_t b : blocks.made) {
          o.Word(memory::Size(b));
          BoxOutputs(o, h.base, b, blocks);
        }
        if (!init) {
          o.Word(blocks.Name(t.r3.u32));
        }
        blocks.FreeAll();
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// DestroyPhysics' cases: a vehicle whose +336 is 0 or a 528-byte block from
// the layer, +340 0 or an active object of the fake world, +344 0 or a box
// from the layer. Compared: the world (exactly: no floats are worked), the
// vehicle's three pointers, which blocks were freed, r3.
void DestroyPhysicsCases(Context& h, uint32_t region) {
  Results results("Vehicle::DestroyPhysics (sub_820B0E70)");
  for (uint64_t seed = 0; seed < 200; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 53 + 17);
      BuildWorld(h, region, r, kNumbers, false);
      const uint32_t vehicle = region + kVehicle;
      for (uint32_t i = 0; i < 0x200; i += 4) {
        guest::StoreU32(h.base, vehicle + i, uint32_t(r.Next()));
      }
      Blocks blocks;
      uint32_t body = 0, shape = 0, object = 0;
      if (r.Below(4) != 0) {
        body = memory::Alloc(528, 0);
        for (uint32_t i = 0; i < 528; i += 4) {
          guest::StoreU32(h.base, body + i, NumberWord(r));
        }
        blocks.given.push_back(body);
      }
      if (r.Below(4) != 0) {
        shape = memory::Alloc(496, 0);
        BuildBox(h, shape, r, kNumbers);
        blocks.given.push_back(shape);
      }
      const uint32_t active = guest::LoadU32(h.base, region + kWorld + 20);
      if (active != 0 && r.Below(3) != 0) {
        object = active;
        const uint32_t next = guest::LoadU32(h.base, active + 228);
        if (next != 0 && r.Below(2) == 0) {
          object = next;
        }
      }
      guest::StoreU32(h.base, vehicle + 336, body);
      guest::StoreU32(h.base, vehicle + 340, object);
      guest::StoreU32(h.base, vehicle + 344, shape);
      PPCContext t = CallContext(h);
      t.fpscr.disableFlushModeUnconditional();
      t.r3.u64 = vehicle;
      (v == 0 ? Original<__imp__sub_820B0E70> : NativeDestroyPhysics)(t, h.base);
      Outputs& o = out[v];
      for (uint32_t i = kWorld; i < kWorldEnd; i += 4) {
        o.Word(guest::LoadU32(h.base, region + i));
      }
      for (uint32_t at : {336u, 340u, 344u}) {
        o.Word(blocks.Name(guest::LoadU32(h.base, vehicle + at)));
      }
      for (uint32_t b : blocks.given) {
        o.Word(memory::IsHeapBlock(b) ? 1 : 0);
      }
      o.Word(blocks.Name(t.r3.u32));
      blocks.FreeAll();
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// The vehicle's region (allocated from the layer): the vehicle (6,272
// bytes), its car data, rigid body, surface records and track positions
// (one per wheel and one for the body's center).
constexpr uint32_t kVehicleRegionBytes = 0x3400;
constexpr uint32_t kCarData = 0x1900, kVehicleBody = 0x2000, kSurfaces = 0x2280,
                   kPositions = 0x2380, kStepObject = 0x2800, kStepContacts = 0x2980,
                   kSetUpData = 0x2A80;  // 0x780 bytes
constexpr uint32_t kPositionBytes = 0x80;

uint32_t WheelOf(uint32_t vehicle, uint32_t w) {
  return vehicle + 416 + 368 * w;
}

// A track position whose last point (+16, +24) is the point's x and z, its
// kind (+80) one the surfaces' tables cover (0 to 7) or below 0.
void SetPosition(uint8_t* base, uint32_t position, uint32_t point, Random& r) {
  guest::StoreU32(base, position + 16, guest::LoadU32(base, point + 0));
  guest::StoreU32(base, position + 24, guest::LoadU32(base, point + 8));
  const uint16_t kinds[] = {0, 1, 4, 7, 0xFFFF, 0x8001};
  guest::StoreU16(base, position + 80, kinds[r.Below(std::size(kinds))]);
}

// The vehicle's inputs: words by the group, the pointers, the points' x and
// z numbers (a NaN would take the mesh's path), the wheels' track positions
// 0 or at their +80, the body's at its +16 (+2984), the surface tables
// (+2668 and +2796, by the byte +2664) and +48 below 2.0 some of the time.
void FillVehicleRegion(uint8_t* base, uint32_t region, Random& r, Group group) {
  auto word = [group](Random& x) { return group == kNaNs ? AnyWord(x) : NumberWord(x); };
  for (uint32_t i = 0; i < kVehicleRegionBytes; i += 4) {
    guest::StoreU32(base, region + i, word(r));
  }
  const uint32_t v = region;
  const uint32_t body = region + kVehicleBody;
  guest::StoreU32(base, v + 272, region + kCarData);
  guest::StoreU32(base, v + 336, body);
  guest::StoreU8(base, v + 2664, uint8_t(r.Below(4) != 0));
  for (uint32_t k = 0; k < 8; k++) {
    for (uint32_t table : {2668u, 2796u}) {
      guest::StoreU32(base, v + table + 4 * k,
                      r.Below(4) == 0 ? 0 : region + kSurfaces + 32 * r.Below(8));
    }
  }
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = WheelOf(v, w);
    guest::StoreU32(base, v + 2144 + 4 * w, v + (w < 2 ? 1888 : 2016));
    guest::StoreU32(base, v + 2160 + 4 * w, region + kCarData + (w < 2 ? 336 : 608));
    guest::StoreU32(base, wheel + 80, NumberWord(r));
    guest::StoreU32(base, wheel + 88, NumberWord(r));
    const uint32_t position = region + kPositions + kPositionBytes * w;
    SetPosition(base, position, wheel + 80, r);
    guest::StoreU32(base, wheel + 20, r.Below(4) == 0 ? 0 : position);
    guest::StoreU32(base, wheel + 176, r.Below(8));
  }
  guest::StoreU32(base, body + 16, NumberWord(r));
  guest::StoreU32(base, body + 24, NumberWord(r));
  const uint32_t position = region + kPositions + kPositionBytes * 4;
  SetPosition(base, position, body + 16, r);
  guest::StoreU32(base, v + 2984, r.Below(4) == 0 ? 0 : position);
  if (r.Below(2) == 0) {
    guest::StoreU32(base, v + 48, 0);
  }
  if (r.Below(2) == 0) {
    // +16 . +96 below the threshold: +96 against +16.
    for (uint32_t i = 0; i < 12; i += 4) {
      guest::StoreFloat(base, v + 96 + i, -guest::LoadFloat(base, v + 16 + i));
    }
  }
  guest::StoreU8(base, body + 0, uint8_t(r.Below(4) == 0));
}

// The region compared: the vehicle's integer words (the pointers, the
// wheel records' pointers +2144 to +2175, the tables, the wheels'
// positions, surfaces and bytes +16, the bytes +2664 and +3132, +2984) exactly and the rest as floats; the body's +0, +12 and
// +516 exactly; the positions' +80 exactly; the car data and the surface
// records (not written) exactly.
void VehicleRegionOutputs(Outputs& out, uint8_t* base, uint32_t region) {
  auto exact_in_vehicle = [](uint32_t i) {
    if (i == 272 || i == 336 || i == 340 || i == 344 || i == 2664 || i == 3132 || i == 2984) {
      return true;
    }
    if (i >= 2144 && i < 2176) {
      return true;
    }
    if ((i >= 2668 && i < 2700) || (i >= 2796 && i < 2828)) {
      return true;
    }
    if (i >= 416 && i < 416 + 4 * 368) {
      const uint32_t at = (i - 416) % 368;
      return at == 16 || at == 20 || at == 176;
    }
    return false;
  };
  for (uint32_t i = 0; i < kCarData; i += 4) {
    const uint32_t w = guest::LoadU32(base, region + i);
    exact_in_vehicle(i) ? out.Word(w) : out.Float(w);
  }
  out.Bytes(Read(base, region + kCarData, kVehicleBody - kCarData));
  for (uint32_t i = 0; i < 528; i += 4) {
    const uint32_t w = guest::LoadU32(base, region + kVehicleBody + i);
    (i == 0 || i == 12 || i == 516) ? out.Word(w) : out.Float(w);
  }
  out.Bytes(Read(base, region + kSurfaces, kPositions - kSurfaces));
  for (uint32_t i = kPositions; i < kPositions + 5 * kPositionBytes; i += 4) {
    const uint32_t w = guest::LoadU32(base, region + i);
    ((i - kPositions) % kPositionBytes == 80) ? out.Word(w) : out.Float(w);
  }
}

// A ground result on the stack (64 bytes): floats but its words +48, +52.
void ResultOutputs(Outputs& out, uint8_t* base, uint32_t result) {
  out.Floats(base, result, 12);
  out.Word(guest::LoadU32(base, result + 48));
  out.Word(guest::LoadU32(base, result + 52));
  out.Floats(base, result + 56, 2);
}

// ProbeGround's, Unflip's and RefreshFromBody's cases on the vehicle's
// region with the fake world (its +8 the ground callback). Compared: the
// region, the temporaries of the frame (ProbeGround's from +80 to its
// register saves at +240, the result at +176; Unflip's from +80 to +352,
// the result at +288), the ground's raised point (its frame + 80), r3 and,
// for ProbeGround, f1.
void VehicleGroundCases(Context& h, uint32_t region, uint32_t vehicle_region) {
  enum Kind { kProbe, kUnflip, kRefresh };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
  } functions[] = {
      {"Vehicle::ProbeGround (sub_820B4420)", Original<__imp__sub_820B4420>, NativeProbeGround,
       kProbe},
      {"Vehicle::Unflip (sub_820AFE90)", Original<__imp__sub_820AFE90>, NativeUnflip, kUnflip},
      {"Vehicle::RefreshFromBody (sub_820B62E8)", Original<__imp__sub_820B62E8>,
       NativeRefreshFromBody, kRefresh},
  };
  const uint32_t r1 = CallContext(h).r1.u32;
  for (const auto& f : functions) {
    for (Group group : {kNumbers, kNaNs}) {
      Results results(std::string(f.name) + kGroupNames[group]);
      for (uint64_t seed = 0; seed < 200; seed++) {
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          Random r(seed * 59 + 19 + 1000 * uint64_t(group) + 100000 * uint64_t(f.kind));
          BuildWorld(h, region, r, kNumbers, false);
          FillVehicleRegion(h.base, vehicle_region, r, group);
          if (f.kind == kRefresh) {
            // SyncFromBody moves the wheels' points first, so a position
            // at them would send the ground to the mesh (none here): the
            // wheels' positions 0.
            for (uint32_t w = 0; w < 4; w++) {
              guest::StoreU32(h.base, WheelOf(vehicle_region, w) + 20, 0);
            }
          }
          FillStack(h, r);
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          t.r3.u64 = vehicle_region;
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          VehicleRegionOutputs(out[v], h.base, vehicle_region);
          if (f.kind == kProbe) {
            const uint32_t frame = r1 - 304;
            out[v].Floats(h.base, frame + 80, (176 - 80) / 4);
            ResultOutputs(out[v], h.base, frame + 176);
            // (The ground's temporaries aren't compared here: the
            // original's later callees' frames, such as
            // Matrix::InverseRotate's transposed rows, which the native ones
            // don't write, are over them.)
            out[v].Double(t.f1.u64);
          } else if (f.kind == kUnflip) {
            const uint32_t frame = r1 - 384;
            out[v].Floats(h.base, frame + 80, (288 - 80) / 4);
            ResultOutputs(out[v], h.base, frame + 288);
            out[v].Floats(h.base, frame - 96 - 144 + 80, 4);
          }
          out[v].Word(t.r3.u32);
        }
        results.Check(out[0], out[1], fmt::format("seed {}", seed));
      }
      results.Report();
    }
  }
}

// The physics step's inputs beyond FillVehicleRegion's, as the vehicle
// harness's (src/harnesses/car_physics.cpp, FillVehicle): the wheels' bytes
// +17 and +204, the collision object (+340) and its contacts, the bytes and
// counters the updates branch on, the gear count (car data +1268) and the
// substeps (+2368). The wheels' track positions and the body's 0 (the body
// moves before Unflip, and SyncFromBody moves the wheels' points).
void FillStepInputs(uint8_t* base, uint32_t region, Random& r) {
  const uint32_t v = region;
  const uint32_t data = region + kCarData;
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = WheelOf(v, w);
    guest::StoreU8(base, wheel + 17, uint8_t(r.Below(4) != 0));
    guest::StoreU32(base, wheel + 204, r.Below(100));
    guest::StoreU32(base, wheel + 20, 0);
  }
  guest::StoreU32(base, v + 2984, 0);
  guest::StoreU32(base, v + 348, r.Below(5));
  guest::StoreU32(base, v + 340, region + kStepObject);
  const uint32_t object = region + kStepObject;
  guest::StoreU32(base, object + 240, r.Below(4) == 0 ? 0 : region + kStepContacts);
  for (uint32_t c = 0; c < 2; c++) {
    const uint32_t contact = region + kStepContacts + 128 * c;
    constexpr uint32_t kFlags[] = {0x01000000, 0x02000000, 0x04000000, 0x00000001};
    guest::StoreU32(base, contact + 4, kFlags[r.Below(4)]);
    guest::StoreU32(base, contact + 68, c == 0 && r.Below(2) == 0 ? contact + 128 : 0);
  }
  guest::StoreU8(base, v + 2384, uint8_t(r.Below(4) == 0));
  if (r.Below(2) == 0) {
    guest::StoreU32(base, v + 2340, std::bit_cast<uint32_t>(float(r.Below(1200)) / 1000.0f));
  }
  guest::StoreU32(base, v + 2344, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(6));
  guest::StoreU8(base, v + 2386, uint8_t(r.Below(3) != 0));
  guest::StoreU32(base, v + 3156, r.Below(1000));
  guest::StoreU32(base, v + 2252, uint32_t(int32_t(r.Below(8)) - 1));
  if (r.Below(2) == 0) {
    guest::StoreU32(base, v + 2392, r.Below(40));
  }
  guest::StoreU32(base, v + 2256, uint32_t(int32_t(r.Below(8)) - 1));
  guest::StoreU8(base, v + 2424, uint8_t(r.Below(2)));
  guest::StoreU8(base, v + 2512, uint8_t(r.Below(4) != 0));
  guest::StoreU8(base, v + 3240, uint8_t(r.Below(2)));
  guest::StoreU8(base, v + 2281, uint8_t(r.Below(2)));
  if (r.Below(2) == 0) {
    guest::StoreU32(base, v + 2524, r.Below(4000));
  }
  guest::StoreU32(base, v + 2496, r.Below(4));
  guest::StoreU8(base, v + 2625, uint8_t(r.Below(2)));
  guest::StoreU8(base, v + 2383, uint8_t(r.Below(2)));
  guest::StoreU8(base, v + 276, uint8_t(r.Below(2)));
  guest::StoreU32(base, v + 2376, r.Below(7));
  guest::StoreU8(base, data + 1604, uint8_t(r.Below(3) != 0));
  guest::StoreU8(base, data + 1605, uint8_t(r.Below(2)));
  if (r.Below(2) == 0) {
    guest::StoreU32(base, v + 2268, 0);
  }
  if (r.Below(2) == 0) {
    guest::StoreU32(base, v + 2228, std::bit_cast<uint32_t>(float(r.Below(1100)) / 1000.0f));
  }
  if (r.Below(2) == 0) {
    guest::StoreU32(base, v + 296, 0);
  }
  switch (r.Below(3)) {
    case 0:
      guest::StoreU32(base, v + 2264, 0x3F800000);
      break;
    case 1:
      guest::StoreU32(base, v + 2264, 0);
      break;
  }
  guest::StoreU32(base, data + 1268, 1 + r.Below(7));
  guest::StoreU32(base, v + 2368, r.Below(8) == 0 ? 0 : 1 + r.Below(4));
  for (uint32_t at : {2276u, 2277u, 2284u, 2285u, 2286u, 2249u}) {
    guest::StoreU8(base, v + at, uint8_t(r.Below(3) == 0));
  }
  for (uint32_t at : {284u, 288u, 292u}) {
    if (r.Below(3) == 0) {
      guest::StoreU32(base, v + at, 0);
    }
  }
  for (uint32_t at : {3132u, 3241u, 2928u}) {
    guest::StoreU8(base, v + at, uint8_t(r.Below(2)));
  }
}

// The step's cases: three groups, numbers only, one NaN word in the vehicle
// and NaNs and infinities anywhere. Compared: the region, its float words
// as floats (all but the pointers, counters, gears, modes and bytes set
// above; of the collision object only +144: NaNs are made from numbers
// too, 0/0 among them), and r3. dt 1/30 most of the time.
void PhysicsStepCases(Context& h, uint32_t region, uint32_t vehicle_region) {
  enum StepGroup { kStepNumbers, kStepOneNaN, kStepNaNs };
  const char* const kStepGroupNames[] = {"", ", one NaN", ", NaNs and infinities anywhere"};
  auto exact_word = [](uint32_t i) {
    static const uint32_t kExact[] = {272,  276,  336,  340,  344,  348,  2248, 2252,
                                      2256, 2276, 2280, 2284, 2344, 2368, 2376, 2380, 2384,
                                      2392, 2424, 2444, 2456, 2496, 2512, 2524, 2624, 2664,
                                      2928, 2984, 3132, 3156, 3240};
    for (uint32_t e : kExact) {
      if (i == e) {
        return true;
      }
    }
    if ((i >= 2144 && i < 2176) || (i >= 2668 && i < 2700) || (i >= 2796 && i < 2828)) {
      return true;
    }
    if (i >= 416 && i < 416 + 4 * 368) {
      const uint32_t at = (i - 416) % 368;
      return at == 16 || at == 20 || at == 176 || at == 204;
    }
    return false;
  };
  for (int group = 0; group < 3; group++) {
    Results results(std::string("Vehicle::PhysicsStep (sub_820B6498)") + kStepGroupNames[group]);
    for (uint64_t seed = 0; seed < 200; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 61 + 23 + 1000 * uint64_t(group));
        BuildWorld(h, region, r, kNumbers, false);
        FillVehicleRegion(h.base, vehicle_region, r, group == kStepNaNs ? kNaNs : kNumbers);
        FillStepInputs(h.base, vehicle_region, r);
        if (group == kStepOneNaN) {
          guest::StoreU32(h.base, vehicle_region + 4 * r.Below(kCarData / 4),
                          kOddFloats[r.Below(6)]);
        }
        FillStack(h, r);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = vehicle_region;
        t.f1.f64 = r.Below(4) == 0 ? double(std::bit_cast<float>(NumberWord(r)))
                                    : double(1.0f / 30.0f);
        (v == 0 ? Original<__imp__sub_820B6498> : NativePhysicsStep)(t, h.base);
        {
          for (uint32_t i = 0; i < kCarData; i += 4) {
            const uint32_t w = guest::LoadU32(h.base, vehicle_region + i);
            exact_word(i) ? out[v].Word(w) : out[v].Float(w);
          }
          out[v].Bytes(Read(h.base, vehicle_region + kCarData, kVehicleBody - kCarData));
          for (uint32_t i = 0; i < 528; i += 4) {
            const uint32_t w = guest::LoadU32(h.base, vehicle_region + kVehicleBody + i);
            (i == 0 || i == 12 || i == 516) ? out[v].Word(w) : out[v].Float(w);
          }
          out[v].Bytes(Read(h.base, vehicle_region + kSurfaces, kStepObject - kSurfaces));
          // The collision object: its +144 (the step's float) as a float.
          out[v].Bytes(Read(h.base, vehicle_region + kStepObject, 144));
          out[v].Float(guest::LoadU32(h.base, vehicle_region + kStepObject + 144));
          out[v].Bytes(Read(h.base, vehicle_region + kStepObject + 148, 320 - 148));
          out[v].Bytes(Read(h.base, vehicle_region + kStepContacts,
                            kVehicleRegionBytes - kStepContacts));
        }
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// SetUpPhysics' and ResetPhysics' cases: the fake world (one object free
// at least), the vehicle's region with its car data at kSetUpData (the
// gear count 1 to 7), for ResetPhysics the body kept at kVehicleBody, the
// old collision object 0 or an active one, the old shape 0 or a box from
// the layer. Compared: the vehicle (its integer words exactly, the rest as
// floats), the car data (not written), the body (the layer's for
// SetUpPhysics), the new shape, which given blocks were freed, the world
// (objects as ObjectOutputs, the rest exactly), the frame's vectors (+80
// to +111), r3 and f1; heap blocks by their number.
void SetUpCases(Context& h, uint32_t region, uint32_t vehicle_region, bool reset) {
  const char* name = reset ? "Vehicle::ResetPhysics (sub_820B5B08)"
                           : "Vehicle::SetUpPhysics (sub_820B5670)";
  auto exact_word = [](uint32_t i) {
    static const uint32_t kExact[] = {272, 276, 336, 340, 344, 2248, 2376, 2624, 3132};
    for (uint32_t e : kExact) {
      if (i == e) {
        return true;
      }
    }
    if (i >= 2144 && i < 2176) {
      return true;
    }
    if (i >= 416 && i < 416 + 4 * 368) {
      const uint32_t at = (i - 416) % 368;
      return at == 16 || at == 20 || at == 176 || at == 204;
    }
    return false;
  };
  const uint32_t r1 = CallContext(h).r1.u32;
  for (Group group : {kNumbers, kNaNs}) {
    Results results(std::string(name) + kGroupNames[group]);
    for (uint64_t seed = 0; seed < 150; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 67 + 29 + 1000 * uint64_t(group) + (reset ? 500 : 0));
        BuildWorld(h, region, r, kNumbers, true);
        FillVehicleRegion(h.base, vehicle_region, r, group);
        const uint32_t vehicle = vehicle_region;
        const uint32_t data = vehicle_region + kSetUpData;
        guest::StoreU32(h.base, data + 1268, 1 + r.Below(7));
        if (r.Below(3) == 0) {
          guest::StoreU32(h.base, data + 1268, 1);
        }
        Blocks blocks;
        if (reset) {
          guest::StoreU32(h.base, vehicle + 336, vehicle_region + kVehicleBody);
          const uint32_t active = guest::LoadU32(h.base, region + kWorld + 20);
          guest::StoreU32(h.base, vehicle + 340, active != 0 && r.Below(3) != 0 ? active : 0);
          uint32_t shape = 0;
          if (r.Below(4) != 0) {
            shape = memory::Alloc(496, 0);
            BuildBox(h, shape, r, kNumbers);
            blocks.given.push_back(shape);
          }
          guest::StoreU32(h.base, vehicle + 344, shape);
        }
        FillStack(h, r);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = vehicle;
        t.r4.u64 = data;
        const uint32_t groups = r.Below(16) | (r.Below(3) == 0 ? 0x00F00000 : 0);
        if (reset) {
          t.r5.u64 = groups;
          (v == 0 ? Original<__imp__sub_820B5B08> : NativeResetPhysics)(t, h.base);
        } else {
          t.r5.u64 = r.Below(2);
          t.r6.u64 = groups;
          (v == 0 ? Original<__imp__sub_820B5670> : NativeSetUpPhysics)(t, h.base);
        }
        const uint32_t body = guest::LoadU32(h.base, vehicle + 336);
        const uint32_t shape = guest::LoadU32(h.base, vehicle + 344);
        blocks.Note(body);
        blocks.Note(shape);
        // The new collision object's record copies (the world's object).
        const uint32_t object = guest::LoadU32(h.base, vehicle + 340);
        if (object != 0) {
          blocks.Note(guest::LoadU32(h.base, object + 36));
          blocks.Note(guest::LoadU32(h.base, object + 40));
        }
        Outputs& o = out[v];
        for (uint32_t i = 0; i < kCarData; i += 4) {
          const uint32_t w = guest::LoadU32(h.base, vehicle + i);
          exact_word(i) ? o.Word(blocks.Name(w)) : o.Float(w);
        }
        o.Bytes(Read(h.base, data, 0x780));
        const uint32_t body_at = reset ? vehicle_region + kVehicleBody : body;
        if (body_at != 0) {
          for (uint32_t i = 0; i < 528; i += 4) {
            const uint32_t w = guest::LoadU32(h.base, body_at + i);
            (i == 0 || i == 12 || i == 516) ? o.Word(blocks.Name(w)) : o.Float(w);
          }
        }
        if (shape != 0) {
          BoxOutputs(o, h.base, shape, blocks);
        }
        for (uint32_t b : blocks.given) {
          o.Word(memory::IsHeapBlock(b) ? 1 : 0);
        }
        for (uint32_t b : blocks.made) {
          o.Word(memory::Size(b));
        }
        for (uint32_t i = kWorld; i < kObjects; i += 4) {
          o.Word(blocks.Name(guest::LoadU32(h.base, region + i)));
        }
        for (uint32_t i = 0; i < 4; i++) {
          ObjectOutputs(o, h.base, region + kObjects + 320 * i, blocks);
        }
        for (uint32_t i = kPairs; i < kWorldEnd; i += 4) {
          o.Word(blocks.Name(guest::LoadU32(h.base, region + i)));
        }
        o.Floats(h.base, r1 - 208 + 80, 8);
        o.Word(t.r3.u32);
        o.Double(t.f1.u64);
        blocks.FreeAll();
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

}  // namespace

void CarPhysicsObjects(Context& h) {
  const uint32_t region = memory::Alloc(kRegionBytes, 0);
  const uint32_t saved_world = guest::LoadU32(h.base, kWorldPointer);
  SetFloat48Cases(h, region);
  CallbackCases(h, region);
  ResetTransformCases(h, region);
  const uint32_t vehicle_region = memory::Alloc(kVehicleRegionBytes, 0);
  VehicleGroundCases(h, region, vehicle_region);
  PhysicsStepCases(h, region, vehicle_region);
  if (hooks::NativeHeapActive()) {
    SetUpCases(h, region, vehicle_region, false);
    SetUpCases(h, region, vehicle_region, true);
  }
  memory::Free(vehicle_region);
  if (hooks::NativeHeapActive()) {
    AddObjectCases(h, region, true);
    AddObjectCases(h, region, false);
    DestroyPhysicsCases(h, region);
  } else {
    ITC_LOG_INFO("harness: car_physics: the physics objects' lifetime skipped (--native_heap off)");
  }
  guest::StoreU32(h.base, kWorldPointer, saved_world);
  memory::Free(region);
}

}  // namespace harness
