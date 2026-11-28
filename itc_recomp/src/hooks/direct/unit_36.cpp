// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 36's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"

#include "c_runtime/strings.h"
#include "car_physics/car_physics.h"
#include "car_specs/car_specs.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "hooks/heap.h"
#include "lifted/entries.h"
#include "lifted/originals.h"
#include "schema/car_specs.h"
#include "schema/garage_screen.h"
#include "schema/race_setup.h"

#include <iterator>

// --- The car physics (docs/functions/car-physics.md): each passes the
// original's stack frame (its r1 after the stwu) and leaves r3 (and f1)
// as the original does, as the deleted adapters did. Flush-to-zero off for
// the work (the adapters' job). ---

using namespace car_physics;

// Vehicle::SetUpGearing: r3 = the vehicle.
ITC_DIRECT_ENTRY(820B4AA8) {
  ctx.fpscr.disableFlushMode();
  VehicleSetUpGearing(ctx.r3.u32, ctx.r1.u32 - 176);
}
// Vehicle::UpdateSuspension: r3 = the vehicle; r3 and f1 as the original
// leaves them.
ITC_DIRECT_ENTRY(820B6328) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  ctx.r3.u64 = VehicleUpdateSuspension(ctx.r3.u32, ctx.r1.u32 - 144, &f1);
  ctx.f1.f64 = f1;
}
// Vehicle::ShiftGears: r3 = the vehicle.
ITC_DIRECT_ENTRY(820B6430) {
  ctx.fpscr.disableFlushMode();
  VehicleShiftGears(ctx.r3.u32, ctx.r1.u32 - 96);
}
// RigidBody::ContactImpulse: r3 = the body, r4 = the point, r5 = the
// normal; f1 = the impulse, r3 as the original leaves it.
ITC_DIRECT_ENTRY(820B6C78) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  ctx.f1.f64 = RigidBodyContactImpulse(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                       ctx.r1.u32 - 208, &r3);
  ctx.r3.u64 = r3;
}
// RigidBody::FrictionImpulse: r3 = out, r4 = the body, r5 = the point, r6 =
// the normal, f1 x f2 = the limit; r3 comes back as out.
ITC_DIRECT_ENTRY(820B6D48) {
  ctx.fpscr.disableFlushMode();
  RigidBodyFrictionImpulse(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.f1.f64,
                           ctx.f2.f64, ctx.r1.u32 - 192);
}
// RigidBody::SetInertia: r3 = the body, r4 = the inertia; r3 comes back as
// +416. Flush-to-zero on after (its tail call's last step is the vector
// unit's).
ITC_DIRECT_ENTRY(820B7280) {
  ctx.fpscr.disableFlushMode();
  RigidBodySetInertia(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 320);
  ctx.r3.u64 = ctx.r3.u32 + 416;
  ctx.fpscr.enableFlushMode();
}
// RigidBody::ApplyImpulses: r3 = the body, f1 = dt; r3 the body when it's
// fixed, else as UpdateVelocity leaves it.
ITC_DIRECT_ENTRY(820B75B8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = RigidBodyApplyImpulses(ctx.r3.u32, ctx.f1.f64, ctx.r1.u32 - 144);
}
// RigidBody::Reset: r3 = the body; r3 comes back as +96.
ITC_DIRECT_ENTRY(820B77D0) {
  ctx.fpscr.disableFlushMode();
  RigidBodyReset(ctx.r3.u32, ctx.r1.u32 - 176);
  ctx.r3.u64 = ctx.r3.u32 + 96;
}
// RigidBody::RigidBody: r3 = the body, returned as it came.
ITC_DIRECT_ENTRY(820B7928) {
  ctx.fpscr.disableFlushMode();
  RigidBodyConstruct(ctx.r3.u32, ctx.r1.u32 - 96);
}
// CollisionObject::Translate: r3 = the object, r4 = the offset; r3 comes
// back as +304. Where the shape record isn't a box, the original.
ITC_DIRECT_ENTRY(820B7B60) {
  ctx.fpscr.disableFlushMode();
  if (!CollisionObjectTranslate(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 128)) {
    lifted::original::c820B7B60(ctx);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + 304;
}
// CollisionObject::AddGroups: r3 = the object, r4 = the bits.
ITC_DIRECT_ENTRY(820B7CC8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = CollisionObjectAddGroups(ctx.r3.u32, ctx.r4.u32);
}
// CollisionObject::RemoveGroups: r3 = the object, r4 = the bits (the
// floating-point mode left as it is, as the deleted adapter and the
// original, which has no float steps). Where
// RemovePairsByGroups (the original's tail call) can't be native, its
// original with the groups removed (the bits are cleared by then).
ITC_DIRECT_ENTRY(820B7CF0) {
  const uint32_t object = ctx.r3.u32;
  uint32_t groups = 0, r3 = 0;
  if (!CollisionObjectRemoveGroups(object, ctx.r4.u32, &groups, &r3)) {
    ctx.r3.u64 = object;
    ctx.r4.u64 = groups;
    lifted::original::c820B8AF8(ctx);
    return;
  }
  ctx.r3.u64 = r3;
}
// CollisionRecords::Merge: r3 = out, r4 = the summary, r5 = the hits, r6 =
// the count; r3 = the count left.
ITC_DIRECT_ENTRY(820B7ED0) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = CollisionRecordsMerge(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                     ctx.r1.u32 - 304);
}
// CollisionWorld::Destroy: no arguments (r3 isn't read by callers: the
// original leaves operator delete's). Where a block isn't the layer's or an
// object's slot 0 isn't CollisionObject's (CollisionWorldDestroyable), the
// original. The floating-point mode is left as it is (the original has no
// float steps).
ITC_DIRECT_ENTRY(820B86B8) {
  if (!CollisionWorldDestroyable()) {
    lifted::original::c820B86B8(ctx);
    return;
  }
  CollisionWorldDestroy();
}
// CollisionWorld::RemovePair: r3 = the index (the floating-point mode left
// as it is, as the deleted adapter). Where the move's memcpy can't be
// native, the original.
ITC_DIRECT_ENTRY(820B8A88) {
  uint32_t r3 = 0;
  if (!CollisionWorldRemovePair(ctx.r3.u32, &r3)) {
    lifted::original::c820B8A88(ctx);
    return;
  }
  ctx.r3.u64 = r3;
}
// CollisionWorld::Create: r3 = the objects, r4 = the pairs, r5 = the
// contacts, r6 = the buffer (the port makes none: a buffer, the original).
// Natively only while --native_heap routes the game's heap to the
// allocation layer (as the car physics' other lifetimes).
ITC_DIRECT_ENTRY(820B8D30) {
  if (ctx.r6.u32 != 0 || !hooks::NativeHeapActive()) {
    lifted::original::c820B8D30(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!CollisionWorldCreate(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, &r3)) {
    lifted::original::c820B8D30(ctx);
    return;
  }
  ctx.r3.u64 = r3;
}
// CollisionWorld::SetQueryCapacity: r3 = n; r3 = the new block. Natively
// only while --native_heap routes the game's heap to the allocation layer.
ITC_DIRECT_ENTRY(820B9198) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c820B9198(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!CollisionWorldSetQueryCapacity(ctx.r3.u32, &r3)) {
    lifted::original::c820B9198(ctx);
    return;
  }
  ctx.r3.u64 = r3;
}
// CollisionWorld::AddObject: r3 = the body, r4 = the shape, r5 = the groups;
// r3 = the object. Natively only while --native_heap routes the game's heap
// to the allocation layer; with no free object or where Init can't be
// native, the original.
ITC_DIRECT_ENTRY(820B9570) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c820B9570(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  uint32_t object = 0;
  if (!CollisionWorldAddObject(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 128,
                               &object)) {
    lifted::original::c820B9570(ctx);
    return;
  }
  ctx.r3.u64 = object;
}
// The surface setters A to D: r4 = the surface bits, f1 = the value (r3,
// the surfaces, left as it is).
ITC_DIRECT_ENTRY(820BC318) {
  ctx.fpscr.disableFlushMode();
  CollisionSurfaceSetA(ctx.r4.u32, ctx.f1.f64);
}
ITC_DIRECT_ENTRY(820BC360) {
  ctx.fpscr.disableFlushMode();
  CollisionSurfaceSetB(ctx.r4.u32, ctx.f1.f64);
}
ITC_DIRECT_ENTRY(820BC3A8) {
  ctx.fpscr.disableFlushMode();
  CollisionSurfaceSetC(ctx.r4.u32, ctx.f1.f64);
}
ITC_DIRECT_ENTRY(820BC3F0) {
  ctx.fpscr.disableFlushMode();
  CollisionSurfaceSetD(ctx.r4.u32, ctx.f1.f64);
}
// The surface getters: r3 = the flags; f1 = the value.
ITC_DIRECT_ENTRY(820BC530) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = CollisionSurfaceOfC(ctx.r3.u32);
}
ITC_DIRECT_ENTRY(820BC538) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = CollisionSurfaceOfD(ctx.r3.u32);
}
ITC_DIRECT_ENTRY(820BC540) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = CollisionSurfaceOfA(ctx.r3.u32);
}
ITC_DIRECT_ENTRY(820BC548) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = CollisionSurfaceOfB(ctx.r3.u32);
}
// CarData::ApplySettings: r3 = the car data, r4 = the car object, r5 = the
// spec's block 2; f1 = its last pow (r3 isn't kept: pow's).
ITC_DIRECT_ENTRY(820C05C8) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = CarDataApplySettings(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 192);
}
// Vehicle::ApplyTuning: r3 = the car data, r4 = the car object; r3 and f1
// as the original leaves them. Where its memcpy wouldn't be copied natively
// (VehicleApplyTuningCopyFits), the original.
ITC_DIRECT_ENTRY(820C0D28) {
  const uint32_t frame = ctx.r1.u32 - 480;
  if (!VehicleApplyTuningCopyFits(ctx.r4.u32, frame)) {
    lifted::original::c820C0D28(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  ctx.r3.u64 = VehicleApplyTuning(ctx.r3.u32, ctx.r4.u32, frame, &f1);
  ctx.f1.f64 = f1;
}

// --- The car parameter tables (docs/functions/car-specs.md) ---

namespace {

// Whether memcpy copies `size` bytes as the host's memcpy does (car_specs'
// own check, repeated: a destination outside the physical views, each
// range in one view, the two not sharing memory).
bool SpecCopyFits(uint32_t destination, uint32_t source, uint32_t size) {
  return destination < 0xA0000000u && c_runtime::InOneView(destination, size) &&
         c_runtime::InOneView(source, size) &&
         !c_runtime::MayShare(destination, size, source, size);
}

// The car's span its copies write (+1888 to +3884, as Car::Refresh's
// check): in one view outside the physical ones, and none of `froms` read
// from it.
struct SpecCopy {
  uint32_t to, from, size;
};
bool SpecCopiesFit(uint32_t car, const SpecCopy* copies, uint32_t count) {
  const uint32_t span = schema::Car(car).block2_address(), span_bytes = 3884 - 1888;
  if (span >= 0xA0000000u || !c_runtime::InOneView(span, span_bytes)) {
    return false;
  }
  for (uint32_t i = 0; i < count; i++) {
    const SpecCopy& copy = copies[i];
    if (copy.size != 0 && (!SpecCopyFits(copy.to, copy.from, copy.size) ||
                           c_runtime::MayShare(span, span_bytes, copy.from, copy.size))) {
      return false;
    }
  }
  return true;
}

}  // namespace

// Car::CopyAeroParts: r3 = the car, r4 = Params::GetCarParts' block (the 9
// pointers read here); r3 comes back as car + 3748. Where a copy wouldn't be
// the host's memcpy, the original. Flush-to-zero off (the getters' lfs).
ITC_DIRECT_ENTRY(820C4568) {
  const uint32_t car = ctx.r3.u32;
  uint32_t parts[9];
  for (uint32_t i = 0; i < 9; i++) {
    parts[i] = guest::LoadU32(schema::CarPartTables(ctx.r4.u32).table74_address() + 4 * i);
  }
  const schema::Car c(car);
  auto aero = [&](uint32_t to, uint32_t table, uint32_t size, int32_t level, bool use) {
    return SpecCopy{to, use ? parts[table] + size * uint32_t(level) - size : to, use ? size : 0};
  };
  const int32_t level77 = int32_t(c.aero_3()) == -1 ? 0 : int32_t(c.aero_3());
  const bool use79 = c.bonnet_color_kind() != 0;
  const SpecCopy copies[] = {
      aero(c.aero_parts_address() + 0, 0, 28, int32_t(c.aero_0()), int32_t(c.aero_0()) != 0),
      aero(c.aero_parts_address() + 28, 1, 28, int32_t(c.aero_1()), int32_t(c.aero_1()) != 0),
      aero(c.aero_parts_address() + 56, 2, 28, int32_t(c.aero_2()), int32_t(c.aero_2()) != 0),
      aero(c.aero_parts_address() + 84, 3, 28, level77, level77 > 0),
      aero(c.aero_parts_address() + 140, 6, 28, int32_t(c.aero_6()), int32_t(c.aero_6()) != 0),
      aero(c.aero_parts_address() + 168, 7, 36, int32_t(c.aero_9()), int32_t(c.aero_9()) != 0),
      aero(c.aero_parts_address() + 204, 8, 36, int32_t(c.aero_10()), int32_t(c.aero_10()) != 0),
      aero(c.aero_parts_address() + 112, use79 ? 5 : 4, 28, int32_t(c.bonnet()),
           int32_t(c.bonnet()) != 0),
  };
  if (!SpecCopiesFit(car, copies, uint32_t(std::size(copies)))) {
    lifted::original::c820C4568(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = car_specs::CopyAeroParts(car, parts);
}
// Car::CopySpec: r3 = the car, r4 = its spec; r3 comes back as car + 3176.
// Where a copy wouldn't be the host's memcpy (or the car's span shares the
// spec), the original. Flush-to-zero off (Tire::InchUp's steps).
ITC_DIRECT_ENTRY(820C4888) {
  const uint32_t car = ctx.r3.u32;
  const uint32_t spec = ctx.r4.u32;
  const schema::Car c(car);
  const schema::CarSpec s(spec);
  const SpecCopy copies[] = {
      {c.block1_address(), spec, 640},
      {c.block2_address(), s.block2_id_address(), 340},
      {c.engine_address(), c.engine_stage() * 204 + s.engine_stages_address(), 204},
      {c.paints_address(), s.paints_address(), 272},
      {c.front_tire_address(),
       car_specs::GetTire(spec, int32_t(c.front_tire_type()), c.front_tire_grade(), 0), 92},
      {c.rear_tire_address(),
       car_specs::GetTire(spec, int32_t(c.rear_tire_type()), c.rear_tire_grade(), 1), 92},
  };
  if (c_runtime::MayShare(c.block2_address(), 3884 - 1888, spec, car_specs::kSpecBytes) ||
      !SpecCopiesFit(car, copies, uint32_t(std::size(copies)))) {
    lifted::original::c820C4888(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = car_specs::CopySpec(car, spec);
}
// Params::GetWheel: r3 = the wheel number; r3 = the record (or 0).
ITC_DIRECT_ENTRY(820C9EB0) {
  ctx.r3.u64 = car_specs::GetWheel(ctx.r3.s32);
}
// The swaps: r3 = the record (or, for the tables' swaps, the register as
// called), r3 as the original leaves it; flush-to-zero off, as their lfs
// leave it.
ITC_DIRECT_ENTRY(820CB658) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = car_specs::SwapRecord83(ctx.r3.u32);
}
ITC_DIRECT_ENTRY(820CB6C8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = car_specs::SwapAeroPart(ctx.r3.u32);
}
ITC_DIRECT_ENTRY(820CB7E8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = car_specs::SwapAeroPart36(ctx.r3.u32);
}
ITC_DIRECT_ENTRY(820CBE10) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = car_specs::SwapWheels(ctx.r3.u32);
}
ITC_DIRECT_ENTRY(820CC1F0) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = car_specs::SwapCarSpecs(ctx.r3.u32);
}

// --- The data tables (docs/functions/data-tables.md): the entries' swaps,
// r3 = the entry, r3 as the original leaves it (the last word swapped);
// flush-to-zero off, as SwapWord's lfs leaves it. ---

ITC_DIRECT_ENTRY(82111AE0) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = data_tables::SwapEntry20(ctx.r3.u32);
}
ITC_DIRECT_ENTRY(82111B30) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = data_tables::SwapEntry56(ctx.r3.u32);
}
ITC_DIRECT_ENTRY(821128E0) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = data_tables::SwapEntry40(ctx.r3.u32);
}
