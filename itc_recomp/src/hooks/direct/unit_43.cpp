// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 43's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"

#include "guest.h"
#include "lifted/originals.h"

#include "archive/archive.h"
#include "frame_passes/frame_passes.h"
#include "hooks/archive.h"
#include "hooks/frame_passes.h"
#include "hooks/location_screen.h"
#include "hooks/morph_jobs.h"
#include "morph_jobs/morph_jobs.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

// --- The vehicle accessors (docs/functions/vehicle-accessors.md) ---
// The setters return nothing callers read (r3 left as it is).

// RigidBody::SetGravity: r3 = the body, r4 = the vector.
ITC_DIRECT_ENTRY(820B6640) {
  ctx.fpscr.disableFlushMode();
  vehicle_accessors::RigidBodySetGravity(ctx.r3.u32, ctx.r4.u32);
}
// RigidBody::SetMass: r3 = the body, f1 = the mass.
ITC_DIRECT_ENTRY(820B6698) {
  ctx.fpscr.disableFlushMode();
  vehicle_accessors::RigidBodySetMass(ctx.r3.u32, ctx.f1.f64);
}
// RigidBody::SetCenterOfMass: r3 = the body, r4 = the offset.
ITC_DIRECT_ENTRY(820B67E0) {
  ctx.fpscr.disableFlushMode();
  vehicle_accessors::RigidBodySetCenterOfMass(ctx.r3.u32, ctx.r4.u32);
}
// RigidBody::Velocity: r3 = the body; r3 = body + 32.
ITC_DIRECT_ENTRY(820B6888) {
  ctx.r3.u64 = vehicle_accessors::RigidBodyVelocity(ctx.r3.u32);
}
// sub_820B6950: r3 = the body, f1 stored at +112.
ITC_DIRECT_ENTRY(820B6950) {
  ctx.fpscr.disableFlushMode();
  vehicle_accessors::RigidBodySetFloat112(ctx.r3.u32, ctx.f1.f64);
}
// The collision object's masks: r3 = the object, r4 = the bits.
ITC_DIRECT_ENTRY(820B7D18) {
  vehicle_accessors::CollisionObjectClearBits252(ctx.r3.u32, ctx.r4.u32);
}
ITC_DIRECT_ENTRY(820B7D28) {
  vehicle_accessors::CollisionObjectSetBits256(ctx.r3.u32, ctx.r4.u32);
}
ITC_DIRECT_ENTRY(820B7D38) {
  vehicle_accessors::CollisionObjectClearBits256(ctx.r3.u32, ctx.r4.u32);
}
ITC_DIRECT_ENTRY(820B7D48) {
  vehicle_accessors::CollisionObjectSetBits260(ctx.r3.u32, ctx.r4.u32);
}
ITC_DIRECT_ENTRY(820B7D58) {
  vehicle_accessors::CollisionObjectClearBits260(ctx.r3.u32, ctx.r4.u32);
}
// CollisionObject::SetSize: r3 = the object, r4 = the vector.
ITC_DIRECT_ENTRY(820B7D68) {
  vehicle_accessors::CollisionObjectSetSize(ctx.r3.u32, ctx.r4.u32);
}
// sub_820B7D70: r3 = the object, f1 stored at +224.
ITC_DIRECT_ENTRY(820B7D70) {
  ctx.fpscr.disableFlushMode();
  vehicle_accessors::CollisionObjectSetFloat224(ctx.r3.u32, ctx.f1.f64);
}
// The collision queries' pointers and CollisionWorld::SetCallback: r3.
ITC_DIRECT_ENTRY(820B8768) { vehicle_accessors::CollisionSetGlobal78(ctx.r3.u32); }
ITC_DIRECT_ENTRY(820B8778) { vehicle_accessors::CollisionSetGlobal7C(ctx.r3.u32); }
ITC_DIRECT_ENTRY(820B8788) { vehicle_accessors::CollisionWorldSetCallback(ctx.r3.u32); }
// The +344 object's setters: r3 = the object, f1 = the value.
ITC_DIRECT_ENTRY(820B9658) {
  ctx.fpscr.disableFlushMode();
  vehicle_accessors::Object344SetFloat20(ctx.r3.u32, ctx.f1.f64);
}
ITC_DIRECT_ENTRY(820B9660) {
  ctx.fpscr.disableFlushMode();
  vehicle_accessors::Object344SetFloat16(ctx.r3.u32, ctx.f1.f64);
}
ITC_DIRECT_ENTRY(820B9670) {
  ctx.fpscr.disableFlushMode();
  vehicle_accessors::Object344SetFloat28(ctx.r3.u32, ctx.f1.f64);
}
ITC_DIRECT_ENTRY(820B9678) {
  ctx.fpscr.disableFlushMode();
  vehicle_accessors::Object344SetFloat24(ctx.r3.u32, ctx.f1.f64);
}
// Matrix::Rotate: r3 = out, r4 = the matrix, r5 = the vector.
ITC_DIRECT_ENTRY(82293880) {
  ctx.fpscr.disableFlushMode();
  vector_library::MatrixRotate(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// --- The archive (docs/functions/files-and-saves.md, archive.md) ---

// The request set-up: r3 = the request (VirtualAlloc through ctx where the
// native layer is off).
ITC_DIRECT_ENTRY(8209D490) { hooks::ArchiveSetUp(ctx); }
// The request finish: r3 = the request.
ITC_DIRECT_ENTRY(8209D688) { hooks::ArchiveFinishRequest(ctx); }
// The slot lookup: r3 = a serial; r3 the slot (sign-extended, as the
// original's lis-based address) or 0.
ITC_DIRECT_ENTRY(8209D818) { ctx.r3.s64 = int32_t(archive::FindSlot(ctx.r3.u32)); }
// Archive::FindLooseFile: r3 = an asset id; r3 the path or 0.
ITC_DIRECT_ENTRY(8209D8D8) { ctx.r3.u64 = archive::FindLooseFile(ctx.r3.u32); }
// The picker: r3 = the current request, r4 and r5 the finished ones; r3 the
// next.
ITC_DIRECT_ENTRY(8209D950) {
  ctx.r3.u64 = archive::PickNext(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
// The queueing: r3 = the 40-byte request; r3 the serial (zero-extended) or
// -1.
ITC_DIRECT_ENTRY(8209DC18) {
  archive::LoadRequest request;
  for (uint32_t i = 0; i < 40; i++) {
    request[i] = guest::LoadU8(ctx.r3.u32 + i);
  }
  hooks::SetArchiveSerial(ctx, archive::Queue(request));
}
// Ucl::UnpackBlock: r3 = the source, r4 = its length, r5 = the destination,
// r6 = where the output's size goes (r7 not read); r3 0, -201
// (sign-extended) or -205 (as a zero-extended word, the original's
// rlwinm before its addi).
ITC_DIRECT_ENTRY(822947F8) {
  const archive::UclBlockResult result =
      archive::UclBlock(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  guest::StoreU32(ctx.r6.u32, result.out);
  if (result.r3 == -205) {
    ctx.r3.u64 = uint32_t(result.r3);
  } else {
    ctx.r3.s64 = result.r3;
  }
}
// ListNode::ListNode: r3 = the node, r4 = the link.
ITC_DIRECT_ENTRY(82294B68) { archive::NodeConstruct(ctx.r3.u32, ctx.r4.u32); }
// List::List: r3 = the list, r4 = whether it deletes its nodes.
ITC_DIRECT_ENTRY(82294B80) { archive::ListConstruct(ctx.r3.u32, ctx.r4.u32); }
// List::Remove: r3 = the list, r4 = a node; r3 1 or 0.
ITC_DIRECT_ENTRY(82294C00) { ctx.r3.u64 = archive::ListRemove(ctx.r3.u32, ctx.r4.u32); }

// --- The location screen (docs/functions/location-screen.md): its steps ---

ITC_DIRECT_ENTRY(82227690) { hooks::LocationScreenKindText(ctx); }
ITC_DIRECT_ENTRY(82227818) { hooks::LocationScreenResetView(ctx); }
ITC_DIRECT_ENTRY(82228588) { hooks::LocationScreenPoseCar(ctx); }
ITC_DIRECT_ENTRY(8222A978) { hooks::LocationScreenShowUnlockEntry(ctx); }
ITC_DIRECT_ENTRY(8222B048) { hooks::LocationScreenShowListEntry(ctx); }
ITC_DIRECT_ENTRY(8222BBD0) { hooks::LocationScreenShowCard(ctx); }

// --- The morph jobs (docs/functions/morph-jobs.md) ---

// SkinJob::SkinJob and MorphJob::MorphJob: r3 = the job, r4 = the worker
// pool's interface, r5 = the record; r3 the job.
ITC_DIRECT_ENTRY(820ACAA8) { hooks::MorphJobsConstruct(ctx, morph_jobs::Kind::kSkin); }
ITC_DIRECT_ENTRY(820ACC08) { hooks::MorphJobsConstruct(ctx, morph_jobs::Kind::kMorph); }

// --- The frame passes (docs/functions/frame-passes.md) ---

// FramePasses::RecordPrimitiveReset: no arguments (the original where the
// plain C++ declines).
ITC_DIRECT_ENTRY(82589990) { hooks::FramePassesRecordPrimitiveReset(ctx); }
// WorkMem::Flip: no arguments; r3 left.
ITC_DIRECT_ENTRY(82589A78) { frame_passes::WorkMemFlip(); }
