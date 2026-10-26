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

}  // namespace

void CarPhysicsObjects(Context& h) {
  const uint32_t region = memory::Alloc(kRegionBytes, 0);
  const uint32_t saved_world = guest::LoadU32(h.base, kWorldPointer);
  SetFloat48Cases(h, region);
  CallbackCases(h, region);
  ResetTransformCases(h, region);
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
