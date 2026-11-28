// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 25's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "car_physics/car_physics.h"
#include "course_scenery/course_scenery.h"
#include "framework/framework.h"
#include "framework/run_loop.h"
#include "hooks/framework.h"
#include "hooks/race_screens_unseen.h"
#include "replay_snapshots/replay_snapshots.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

// --- The vehicle accessors (src/vehicle_accessors/) ---

// Vehicle::WheelContact: r3 = the vehicle, r4 = the wheel; r3 the record.
ITC_DIRECT_ENTRY(820C0170) {
  ctx.r3.u64 = vehicle_accessors::VehicleWheelContact(ctx.r3.u32, ctx.r4.u32);
}
// ScenePools::GetB: r3 = the scene, r4 = the pool, r5 = the index; r3 the
// object or 0.
ITC_DIRECT_ENTRY(820C5C20) {
  ctx.r3.u64 = vehicle_accessors::ScenePoolsGetB(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
// RacerRecords::Add: r3 = the records, r4 = the record; r3 comes back as
// where it went (memcpy's result). The original where the port declines.
ITC_DIRECT_ENTRY(820FFE08) {
  uint32_t added = 0;
  if (!vehicle_accessors::RacerRecordsAdd(ctx.r3.u32, ctx.r4.u32, &added)) {
    lifted::original::c820FFE08(ctx);
    return;
  }
  ctx.r3.u64 = added;
}
// RigidBody::Torque: r3 = the body; r3 its +128.
ITC_DIRECT_ENTRY(82379328) { ctx.r3.u64 = vehicle_accessors::RigidBodyTorque(ctx.r3.u32); }

// The vector library (r3 left as it was, f1 for a returned single).
// Vec3::NormalizeSelf: r3 = v; f1 the length.
ITC_DIRECT_ENTRY(82294298) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(vector_library::Vec3NormalizeSelf(ctx.r3.u32));
}
// Vec4::ScaleBy: r3 = v, f1 = s.
ITC_DIRECT_ENTRY(82294340) {
  ctx.fpscr.disableFlushMode();
  vector_library::Vec4ScaleBy(ctx.r3.u32, ctx.f1.f64);
}
// sub_82294470 (ScaledFrom): r3 = out, r5 = v (r4 isn't read), f1 = s.
ITC_DIRECT_ENTRY(82294470) {
  ctx.fpscr.disableFlushMode();
  vector_library::Vec4ScaledFrom(ctx.r3.u32, ctx.r5.u32, ctx.f1.f64);
}
// Vec3::Cross: r3 = out, r4 = a, r5 = b.
ITC_DIRECT_ENTRY(82294518) {
  ctx.fpscr.disableFlushMode();
  vector_library::Vec3Cross(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// --- The framework (src/framework/) ---

// The folded getter of +64: r3 = the object; r3 the word.
ITC_DIRECT_ENTRY(82090778) { ctx.r3.u64 = framework::Word64(ctx.r3.u32); }
// HandlerMap's _Rrotate and _Lrotate: r3 = the map, r4 = the node (r3 left).
ITC_DIRECT_ENTRY(82092A00) { framework::MapRotateRight(ctx.r3.u32, ctx.r4.u32); }
ITC_DIRECT_ENTRY(82092B10) { framework::MapRotateLeft(ctx.r3.u32, ctx.r4.u32); }
// ForEachTickHandler: r3 = the result, r4 and r5 = the iterators, r6 = the
// functor; r3 the result (its register step).
ITC_DIRECT_ENTRY(82099D50) { hooks::ForEachTickHandler(ctx); }

// --- The car physics (src/car_physics/) ---

// SteerRecord::Fill: r3 = out, r7 = the front axle record, f1, f2, f3 (r3
// left as it is: a leaf).
ITC_DIRECT_ENTRY(820B0668) {
  ctx.fpscr.disableFlushMode();
  car_physics::SteerRecordFill(ctx.r3.u32, ctx.r7.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64);
}
// SteerRecord::Curve: r3 = the record, f1 = the angle; f1 the curve. Where
// tan would fail, the original. r3 isn't kept by the original (tan's
// _ctrlfp leaves it; its callers set it again): left as it is.
ITC_DIRECT_ENTRY(820B3E98) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  if (!car_physics::SteerRecordCurve(ctx.r3.u32, ctx.f1.f64, &f1)) {
    lifted::original::c820B3E98(ctx);
    return;
  }
  ctx.f1.f64 = f1;
}
// RigidBody::SetFloat48: r3 = the body, r4 = the float's address (r3 left).
ITC_DIRECT_ENTRY(8259D5A8) {
  ctx.fpscr.disableFlushMode();
  car_physics::RigidBodySetFloat48(ctx.r3.u32, ctx.r4.u32);
}

// --- The course scenery (src/course_scenery/) ---

// PoolObject::ClearLoadFlags and MarkLoaded: r3 = the object (left in r3).
ITC_DIRECT_ENTRY(820FB998) { course_scenery::ClearLoadFlags(ctx.r3.u32); }
ITC_DIRECT_ENTRY(820FB9A8) { course_scenery::MarkLoaded(ctx.r3.u32); }

// --- The replay snapshots (src/replay_snapshots/) ---

// RaceSnapshot::Copy: r3 = out, r4 = the block; r3 comes back as out.
ITC_DIRECT_ENTRY(82104EA8) {
  ctx.r3.u64 = replay_snapshots::RaceSnapshotCopy(ctx.r3.u32, ctx.r4.u32);
}

// --- The race screens never seen in play (src/race_screens/kind6_pages.cpp),
// through their register steps in hooks/race_screens_unseen.cpp ---

// Leaderboard::Rank (no arguments), CarLogo (r3 the car) and
// Network::SetLiveState (r3 the state).
ITC_DIRECT_ENTRY(822914A8) { hooks::RaceScreensUnseenKind6Rank(ctx); }
ITC_DIRECT_ENTRY(8225FFC8) { hooks::RaceScreensUnseenKind6CarLogo(ctx); }
ITC_DIRECT_ENTRY(822927B0) { hooks::RaceScreensUnseenKind6NetState(ctx); }
