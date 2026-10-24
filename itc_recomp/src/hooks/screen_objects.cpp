// The race's screen objects natively: adapters in front of
// src/screen_objects/ (docs/functions/screen-objects.md). The functions
// over the two maps allocate and free their nodes, so they run natively
// only while --native_heap routes the game's heap to the allocation layer,
// and hand the paths left to the original (see uv_trees.h) back to it from
// the start, nothing having been done. Flush-to-zero is left off, as the
// translated code leaves it.
//
// Only the functions recompiled code calls in play have adapters
// (BuildRings, SetSpeed, ~ScreenObject and UvMap::UvMap): the others are
// reached only from their plain C++, and the flag switches those. The
// adapters of the trees' functions AddRingPoint and the trees themselves
// call (UvColumn::UvColumn, Copy, Find, InsertUnique, ~UvColumn, UvMap::Find,
// InsertUnique, ~UvMap) went once those callers were native.

#include "guest.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/screen_objects.h"
#include "screen_objects/screen_objects.h"
#include "screen_objects/uv_trees.h"

ITC_FLAG_DECLARE(bool, native_video_output);

ITC_FLAG_BOOL(native_screen_objects, true, "ITC",
              "Run the race's screen objects (docs/functions/screen-objects.md): "
              "ScreenObject::BuildRings sub_821329B8 with AddRingPoint sub_82132778 (no adapter: only "
              "BuildRings' plain C++ calls it), SetSpeed sub_8212FCC8, ~ScreenObject sub_82132FC8 "
              "with Release sub_8212F0B8 (no adapter: only ~ScreenObject's plain C++ calls it; with "
              "--native_video_output); their two maps "
              "(the C++ library's red-black trees: UvMap::UvMap sub_82130198, and without "
              "adapters, as only the functions above and the trees' own call them: "
              "UvColumn::UvColumn sub_82130140, Copy sub_82130088, Find sub_821301F0, "
              "InsertUnique sub_82131CB8 and ~UvColumn sub_82131EE0, UvMap::Find sub_82130290, "
              "InsertUnique sub_821321F8 and ~UvMap sub_82132F58, the columns' AllocateNodes sub_8212FBD8, NewHead sub_8212FF08, NewNode "
              "sub_8212FDB0, CopySubtree sub_8212FE80, RotateLeft sub_8212F920 and RotateRight "
              "sub_8212F988, LowerBoundNode sub_8212F8C8, LowerBound sub_8212FFC8, the iterator "
              "decrement sub_8212EEF8, Insert sub_82131680, EraseSubtree sub_8212FD50, Clear "
              "sub_8212FE28 and Erase sub_82131DD8; the map's AllocateNodes sub_8212FC50, "
              "NewHead sub_8212FF68, ConstructNode sub_82131F50, RotateLeft sub_8212FA50 and "
              "RotateRight sub_8212FAB8, LowerBoundNode sub_8212F9F8, LowerBound sub_82130028, "
              "Decrement sub_8212FB28, Insert sub_82131FB0, EraseSubtree sub_82132720, Clear "
              "sub_82132960 and Erase sub_82132BD8), with --native_heap, natively instead of "
              "through the translated code (the same stores; checked by "
              "--run_harnesses=screen_objects)");

namespace so = screen_objects;

namespace {

// The functions over the maps: only over the allocation layer.
bool Native() { return ITC_FLAG(native_screen_objects) && hooks::NativeHeapActive(); }

}  // namespace

#define ITC_SCREEN_OBJECTS_FALLBACK(address) \
  REX_FUNC_PROLOGUE();                       \
  if (!Native()) {                           \
    __imp__sub_##address(ctx, base);         \
    return;                                  \
  }                                          \
  ctx.fpscr.disableFlushMode();

// --- UvMap ---

// UvMap: r3 = the map; r3 the head.
extern "C" REX_FUNC(sub_82130198) {
  ITC_SCREEN_OBJECTS_FALLBACK(82130198)
  if (!so::CanAllocate(base)) {
    __imp__sub_82130198(ctx, base);
    return;
  }
  so::MapConstruct(base, ctx.r3.u32);
  ctx.r3.u64 = guest::LoadU32(base, ctx.r3.u32 + so::kHead);
}

// --- ScreenObject ---

// BuildRings: r3 = the object; r3 as the last AddRingPoint leaves it.
extern "C" REX_FUNC(sub_821329B8) {
  ITC_SCREEN_OBJECTS_FALLBACK(821329B8)
  const auto r3 = so::BuildRings(base, ctx.r3.u32, ctx.r1.u32);
  if (!r3) {
    __imp__sub_821329B8(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

// SetSpeed: r3 = the object, f1 = the strength (allocates nothing, so not
// tied to --native_heap); r3 and f1 left as they came.
extern "C" REX_FUNC(sub_8212FCC8) {
  REX_FUNC_PROLOGUE();
  ctx.fpscr.disableFlushMode();
  if (!ITC_FLAG(native_screen_objects) || !so::SetSpeed(base, ctx.r3.u32, ctx.f1.f64)) {
    __imp__sub_8212FCC8(ctx, base);
  }
}

namespace {

// The video output: [0x8266D088]'s slot 7, then that one's slot 3, with
// the original's return addresses.
uint32_t VideoOutputCall(PPCContext& ctx, uint8_t* base, uint32_t first, uint32_t second) {
  ctx.r3.u64 = guest::LoadU32(base, 0x8266D088);
  ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 28);
  ctx.lr = first;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 12);
  ctx.lr = second;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  return ctx.r3.u32;
}

// Release's guest calls, from its frame (ctx.r1 there).
so::ReleaseCalls ReleaseCalls(PPCContext& ctx, uint8_t* base, int& releases) {
  return {[&ctx, base](int which) {
            return which == 0 ? VideoOutputCall(ctx, base, 0x8212F0E0, 0x8212F0F0)
                              : VideoOutputCall(ctx, base, 0x8212F128, 0x8212F138);
          },
          [&ctx, base, &releases](uint32_t resource) {
            ctx.r3.u64 = resource;
            ctx.lr = releases++ == 0 ? 0x8212F100 : 0x8212F110;
            sub_822A67B0(ctx, base);
          }};
}

}  // namespace

namespace hooks {

void ScreenObjectRelease(PPCContext& ctx, uint8_t* base) {
  const uint32_t object = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, base, 128, 0x8212F0E0, [&](uint32_t frame) {
    int releases = 0;
    r3 = so::Release(base, object, frame, ReleaseCalls(ctx, base, releases));
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

void ScreenObjectDestroy(PPCContext& ctx, uint8_t* base) {
  const uint32_t object = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, base, 96, 0x82132FE0, [&](uint32_t frame) {
    guest_call::InFrame(ctx, base, 128, 0x8212F0E0, [&](uint32_t) {
      int releases = 0;
      r3 = so::Destroy(base, object, frame, ReleaseCalls(ctx, base, releases));
    });
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

}  // namespace hooks

// ~ScreenObject: r3 = the object; r3 the map's freed head.
extern "C" REX_FUNC(sub_82132FC8) {
  REX_FUNC_PROLOGUE();
  if (!Native() || !ITC_FLAG(native_video_output)) {
    __imp__sub_82132FC8(ctx, base);
    return;
  }
  hooks::ScreenObjectDestroy(ctx, base);
}
