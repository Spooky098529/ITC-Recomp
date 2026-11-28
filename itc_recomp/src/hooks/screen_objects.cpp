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
#include "lifted/names.h"
#include "lifted/entries.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/direct_slots.h"
#include "hooks/heap.h"
#include "hooks/screen_objects.h"
#include "schema/app_start_up.h"
#include "schema/screen_objects.h"
#include "screen_objects/screen_objects.h"
#include "screen_objects/uv_trees.h"
#include "lifted/sdk.h"

#include <bit>

ITC_FLAG_DECLARE(bool, native_video_output);

ITC_FLAG_BOOL(native_screen_objects, true, "ITC",
              "Run the race's screen objects (docs/functions/screen-objects.md): "
              "ScreenObject::Record sub_82130978 (lift.py's translation, inside its wrap in "
              "src/debug/frame_state_diff.cpp; without the native heap too), "
              "ScreenObject::BuildRings sub_821329B8 with AddRingPoint sub_82132778 (no adapter: only "
              "BuildRings' plain C++ calls it), SetSpeed sub_8212FCC8 (no adapter: only "
              "RaceScene::Update's plain C++ calls it, and --native_race_update switches that), ~ScreenObject sub_82132FC8 "
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
    lifted::original::c##address(ctx); \
    return;                                  \
  }                                          \
  ctx.fpscr.disableFlushMode();

// --- UvMap ---

// UvMap: r3 = the map; r3 the head.
ITC_NATIVE_ENTRY_ALWAYS(82130198) {
  ITC_SCREEN_OBJECTS_FALLBACK(82130198)
  if (!so::CanAllocate()) {
    lifted::original::c82130198(ctx);
    return;
  }
  so::MapConstruct(ctx.r3.u32);
  ctx.r3.u64 = schema::ScreenObjectUvMap(ctx.r3.u32).head().address;
}

// --- ScreenObject ---

// BuildRings: r3 = the object; r3 as the last AddRingPoint leaves it.
ITC_NATIVE_ENTRY_ALWAYS(821329B8) {
  ITC_SCREEN_OBJECTS_FALLBACK(821329B8)
  const auto r3 = so::BuildRings(ctx.r3.u32, ctx.r1.u32);
  if (!r3) {
    lifted::original::c821329B8(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}

// SetSpeed (sub_8212FCC8) has no adapter: its only caller in play,
// RaceScene::Update, calls its plain C++ (the original where it refuses: the
// object running); --native_race_update switches that.

namespace {

// The video output: [0x8266D088]'s slot 7, then that one's slot 3, with
// the original's return addresses.
uint32_t VideoOutputCall(lifted::Regs& ctx, uint32_t first, uint32_t second) {
  ctx.r3.u64 = schema::AppStartUpGlobals::scene_target_app().address;
  ctx.lr = first;
  // Slot 7's plain C++ where it's the game's (hooks::DirectSlotCall).
  const uint32_t slot7 =
      guest::LoadU32(schema::AppInterface16(ctx.r3.u32).vtable() + 28);
  if (!hooks::DirectSlotCall(ctx, slot7)) {
    lifted::CallFromContext(ctx, slot7);
  }
  // Slot 3 of what slot 7 answers (no schema type yet: app-start-up's
  // AppInterface16 has no vtable type, and slot 7's answer has none).
  ctx.ctr.u64 = guest::LoadU32(guest::LoadU32(ctx.r3.u32) + 12);
  ctx.lr = second;
  lifted::CallFromContext(ctx, ctx.ctr.u32);
  return ctx.r3.u32;
}

// Release's guest calls, from its frame (ctx.r1 there).
so::ReleaseCalls ReleaseCalls(lifted::Regs& ctx, int& releases) {
  return {[&ctx](int which) {
            return which == 0 ? VideoOutputCall(ctx, 0x8212F0E0, 0x8212F0F0)
                              : VideoOutputCall(ctx, 0x8212F128, 0x8212F138);
          },
          [&ctx, &releases](uint32_t resource) {
            ctx.r3.u64 = resource;
            ctx.lr = releases++ == 0 ? 0x8212F100 : 0x8212F110;
            lifted::Enter(ctx, game::Resource::Release);
          }};
}

}  // namespace

namespace hooks {

ITC_REGS_STEP(ScreenObjectRelease) {
  const uint32_t object = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, 128, 0x8212F0E0, [&](uint32_t frame) {
    int releases = 0;
    r3 = so::Release(object, frame, ReleaseCalls(ctx, releases));
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

ITC_REGS_STEP(ScreenObjectDestroy) {
  const uint32_t object = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, 96, 0x82132FE0, [&](uint32_t frame) {
    guest_call::InFrame(ctx, 128, 0x8212F0E0, [&](uint32_t) {
      int releases = 0;
      r3 = so::Destroy(object, frame, ReleaseCalls(ctx, releases));
    });
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

}  // namespace hooks

// ~ScreenObject: r3 = the object; r3 the map's freed head.
ITC_NATIVE_ENTRY_ALWAYS(82132FC8) {
  
  if (!Native() || !ITC_FLAG(native_video_output)) {
    lifted::original::c82132FC8(ctx);
    return;
  }
  hooks::ScreenObjectDestroy(ctx);
}

// For the fallback originals (docs/missing-functions.md).

namespace {

// A checked iterator at `address` (the tree, then the node).
so::Iterator LoadIterator(uint32_t address) {
  return {guest::LoadU32(address), guest::LoadU32(address + 4)};
}

void StoreIterator(uint32_t address, so::Iterator it) {
  guest::StoreU32(address, it.tree);
  guest::StoreU32(address + 4, it.node);
}

// An iterator passed by value in one register (the tree in its high word).
so::Iterator IteratorOf(uint64_t r) { return {uint32_t(r >> 32), uint32_t(r)}; }

double KeyAt(uint32_t address) { return std::bit_cast<double>(guest::LoadU64(address)); }

}  // namespace

// UvColumn::LowerBoundNode: r3 = the column, r4 = the key's address; r3 the
// node.
ITC_DIRECT_ENTRY(8212F8C8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = so::ColumnLowerBoundNode(ctx.r3.u32, KeyAt(ctx.r4.u32));
}

// UvMap::LowerBoundNode: as the column's.
ITC_DIRECT_ENTRY(8212F9F8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = so::MapLowerBoundNode(ctx.r3.u32, KeyAt(ctx.r4.u32));
}

// UvMap::Decrement: r3 = the iterator's address (r3 left so).
ITC_DIRECT_ENTRY(8212FB28) {
  so::Iterator it = LoadIterator(ctx.r3.u32);
  if (!so::MapDecrement(it)) {
    lifted::original::c8212FB28(ctx);
    return;
  }
  StoreIterator(ctx.r3.u32, it);
}

// UvColumn::Insert: r3 = the iterator out, r4 = the column, r5 = add on the
// left (its low byte), r6 = where, r7 = the value; r3 the iterator out.
ITC_DIRECT_ENTRY(82131680) {
  ITC_SCREEN_OBJECTS_FALLBACK(82131680)
  const auto it = so::ColumnInsert(ctx.r4.u32, (ctx.r5.u32 & 0xFF) != 0, ctx.r6.u32, ctx.r7.u32);
  if (!it) {
    lifted::original::c82131680(ctx);
    return;
  }
  StoreIterator(ctx.r3.u32, *it);
}

// UvColumn::Erase: r3 = the iterator out, r4 = the column, r5 and r6 = first
// and last (by value, the tree in the high word); r3 the iterator out.
ITC_DIRECT_ENTRY(82131DD8) {
  ITC_SCREEN_OBJECTS_FALLBACK(82131DD8)
  const auto it = so::ColumnErase(ctx.r4.u32, IteratorOf(ctx.r5.u64), IteratorOf(ctx.r6.u64));
  if (!it) {
    lifted::original::c82131DD8(ctx);
    return;
  }
  StoreIterator(ctx.r3.u32, *it);
}

// UvMap::Insert: as the column's.
ITC_DIRECT_ENTRY(82131FB0) {
  ITC_SCREEN_OBJECTS_FALLBACK(82131FB0)
  const auto it = so::MapInsert(ctx.r4.u32, (ctx.r5.u32 & 0xFF) != 0, ctx.r6.u32, ctx.r7.u32);
  if (!it) {
    lifted::original::c82131FB0(ctx);
    return;
  }
  StoreIterator(ctx.r3.u32, *it);
}

// UvMap::Clear: r3 = the map; r3 as EraseSubtree leaves it.
ITC_DIRECT_ENTRY(82132960) {
  ITC_SCREEN_OBJECTS_FALLBACK(82132960)
  ctx.r3.u64 = so::MapClear(ctx.r3.u32);
}

// For the fallback originals (docs/missing-functions.md): the originals of
// the inserts and erases above call these tree helpers by address.

// UvColumn::RotateLeft and RotateRight: r3 = the column, r4 = the node; r3
// left as it was (leaves).
ITC_DIRECT_ENTRY(8212F920) { so::ColumnRotateLeft(ctx.r3.u32, ctx.r4.u32); }
ITC_DIRECT_ENTRY(8212F988) { so::ColumnRotateRight(ctx.r3.u32, ctx.r4.u32); }

// UvMap::RotateLeft and RotateRight: as the column's.
ITC_DIRECT_ENTRY(8212FA50) { so::MapRotateLeft(ctx.r3.u32, ctx.r4.u32); }
ITC_DIRECT_ENTRY(8212FAB8) { so::MapRotateRight(ctx.r3.u32, ctx.r4.u32); }

// UvColumn::NewNode: r4 = left, r5 = parent, r6 = right, r7 = the value, r8
// = the color (its low byte); r3 the node.
ITC_DIRECT_ENTRY(8212FDB0) {
  ITC_SCREEN_OBJECTS_FALLBACK(8212FDB0)
  ctx.r3.u64 = so::ColumnNewNode(ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                                 uint8_t(ctx.r8.u32));
}

// UvColumn::Clear: r3 = the column; r3 as EraseSubtree leaves it.
ITC_DIRECT_ENTRY(8212FE28) {
  ITC_SCREEN_OBJECTS_FALLBACK(8212FE28)
  ctx.r3.u64 = so::ColumnClear(ctx.r3.u32);
}

// UvMap::ConstructNode: r3 = the node, r4 = left, r5 = parent, r6 = right,
// r7 = the value, r8 = the color (its low byte); r3 the node.
ITC_DIRECT_ENTRY(82131F50) {
  ITC_SCREEN_OBJECTS_FALLBACK(82131F50)
  ctx.r3.u64 = so::MapConstructNode(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                    ctx.r7.u32, uint8_t(ctx.r8.u32));
}

// UvMap::EraseSubtree: r3 = the map, r4 = the node; r3 the last node freed
// (or the map).
ITC_DIRECT_ENTRY(82132720) {
  ITC_SCREEN_OBJECTS_FALLBACK(82132720)
  ctx.r3.u64 = so::MapEraseSubtree(ctx.r3.u32, ctx.r4.u32);
}

// UvColumn::AllocateNodes: r3 = the count; r3 the block; the original where
// the port declines (bad_alloc, operator new failing).
ITC_DIRECT_ENTRY(8212FBD8) {
  ITC_SCREEN_OBJECTS_FALLBACK(8212FBD8)
  const auto block = so::ColumnAllocateNodes(ctx.r3.u32);
  if (!block) {
    lifted::original::c8212FBD8(ctx);
    return;
  }
  ctx.r3.u64 = *block;
}

// UvColumn::EraseSubtree: r3 = the column, r4 = the node; r3 the last node
// freed (or the column).
ITC_DIRECT_ENTRY(8212FD50) {
  ITC_SCREEN_OBJECTS_FALLBACK(8212FD50)
  ctx.r3.u64 = so::ColumnEraseSubtree(ctx.r3.u32, ctx.r4.u32);
}

// For the fallback originals (docs/missing-functions.md): UvColumn::Find and
// UvMap::Find: r3 = the iterator out, r4 = the tree, r5 =
// the key's address; r3 left as the iterator out. The original where the port
// declines (where it traps) or off the allocation layer, as the file's others.
ITC_DIRECT_ENTRY(821301F0) {
  ITC_SCREEN_OBJECTS_FALLBACK(821301F0)
  const auto it = so::ColumnFind(ctx.r4.u32, KeyAt(ctx.r5.u32));
  if (!it) {
    lifted::original::c821301F0(ctx);
    return;
  }
  StoreIterator(ctx.r3.u32, *it);
}
ITC_DIRECT_ENTRY(82130290) {
  ITC_SCREEN_OBJECTS_FALLBACK(82130290)
  const auto it = so::MapFind(ctx.r4.u32, KeyAt(ctx.r5.u32));
  if (!it) {
    lifted::original::c82130290(ctx);
    return;
  }
  StoreIterator(ctx.r3.u32, *it);
}

// For the fallback originals (docs/missing-functions.md): ports with no
// adapter that lifted code calls by address.
// ScreenObject::Release: r3 = the object; through its register step, as
// ~ScreenObject's adapter decides (with the video output native).
ITC_DIRECT_ENTRY(8212F0B8) {
  if (!Native() || !ITC_FLAG(native_video_output)) {
    lifted::original::c8212F0B8(ctx);
    return;
  }
  hooks::ScreenObjectRelease(ctx);
}
// UvMap::NewHead: r3 the head (operator new, as UvMap::UvMap's adapter).
ITC_DIRECT_ENTRY(8212FF68) {
  ITC_SCREEN_OBJECTS_FALLBACK(8212FF68)
  if (!so::CanAllocate()) {
    lifted::original::c8212FF68(ctx);
    return;
  }
  ctx.r3.u64 = so::MapNewHead();
}
// ScreenObject::AddRingPoint: r3 = the object, f1-f4 = x, y, u, v; its
// temporaries in the 272-byte frame below r1. r3 as the original leaves it;
// the original where the port declines.
ITC_DIRECT_ENTRY(82132778) {
  ITC_SCREEN_OBJECTS_FALLBACK(82132778)
  const auto r3 = so::AddRingPoint(ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64, ctx.f4.f64,
                                   ctx.r1.u32);
  if (!r3) {
    lifted::original::c82132778(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// ~UvMap: r3 = the map; r3 the freed head. The original where the port
// declines.
ITC_DIRECT_ENTRY(82132F58) {
  ITC_SCREEN_OBJECTS_FALLBACK(82132F58)
  const auto head = so::MapDestroy(ctx.r3.u32);
  if (!head) {
    lifted::original::c82132F58(ctx);
    return;
  }
  ctx.r3.u64 = *head;
}
