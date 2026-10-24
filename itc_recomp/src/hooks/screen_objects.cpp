// The race's screen objects natively: adapters in front of
// src/screen_objects/ (docs/functions/screen-objects.md). The functions
// over the two maps allocate and free their nodes, so they run natively
// only while --native_heap routes the game's heap to the allocation layer,
// and hand the paths left to the original (see uv_trees.h) back to it from
// the start, nothing having been done. Flush-to-zero is left off, as the
// translated code leaves it.
//
// Only the functions recompiled code calls in play have adapters: the
// maps' other functions are reached only from these ones' plain C++, and
// the flag switches those.

#include "guest.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "screen_objects/uv_trees.h"

#include <bit>

ITC_FLAG_BOOL(native_screen_objects, true, "ITC",
              "Run the race's screen objects (docs/functions/screen-objects.md): their two maps "
              "(the C++ library's red-black trees: UvColumn::UvColumn sub_82130140, Copy "
              "sub_82130088, Find sub_821301F0, InsertUnique sub_82131CB8 and ~UvColumn "
              "sub_82131EE0, UvMap::UvMap sub_82130198, Find sub_82130290, InsertUnique "
              "sub_821321F8 and ~UvMap sub_82132F58, and without adapters, as only those call "
              "them: the columns' AllocateNodes sub_8212FBD8, NewHead sub_8212FF08, NewNode "
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

double KeyAt(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>((uint64_t(guest::LoadU32(base, address)) << 32) |
                               guest::LoadU32(base, address + 4));
}

void StoreIterator(uint8_t* base, uint32_t out, so::Iterator it) {
  guest::StoreU32(base, out + 0, it.tree);
  guest::StoreU32(base, out + 4, it.node);
}

void StoreInsertResult(uint8_t* base, uint32_t out, so::InsertResult result) {
  StoreIterator(base, out, result.where);
  guest::StoreU8(base, out + 8, result.inserted ? 1 : 0);
}

}  // namespace

#define ITC_SCREEN_OBJECTS_FALLBACK(address) \
  REX_FUNC_PROLOGUE();                       \
  if (!Native()) {                           \
    __imp__sub_##address(ctx, base);         \
    return;                                  \
  }                                          \
  ctx.fpscr.disableFlushMode();

// --- UvColumn ---

// UvColumn: r3 = the column; r3 the head (NewHead's).
extern "C" REX_FUNC(sub_82130140) {
  ITC_SCREEN_OBJECTS_FALLBACK(82130140)
  if (!so::CanAllocate(base)) {
    __imp__sub_82130140(ctx, base);
    return;
  }
  so::ColumnConstruct(base, ctx.r3.u32);
  ctx.r3.u64 = guest::LoadU32(base, ctx.r3.u32 + so::kHead);
}

// Copy: r3 = the (constructed) column, r4 = the source; r3 the new root.
extern "C" REX_FUNC(sub_82130088) {
  ITC_SCREEN_OBJECTS_FALLBACK(82130088)
  if (!so::CanAllocate(base)) {
    __imp__sub_82130088(ctx, base);
    return;
  }
  ctx.r3.u64 = so::ColumnCopy(base, ctx.r3.u32, ctx.r4.u32);
}

// Find: r3 = the iterator out, r4 = the column, r5 = the key's address; r3
// the iterator out.
extern "C" REX_FUNC(sub_821301F0) {
  ITC_SCREEN_OBJECTS_FALLBACK(821301F0)
  const auto it = so::ColumnFind(base, ctx.r4.u32, KeyAt(base, ctx.r5.u32));
  if (!it) {
    __imp__sub_821301F0(ctx, base);
    return;
  }
  StoreIterator(base, ctx.r3.u32, *it);
}

// InsertUnique: r3 = the result out, r4 = the column, r5 = the value; r3
// the result out.
extern "C" REX_FUNC(sub_82131CB8) {
  ITC_SCREEN_OBJECTS_FALLBACK(82131CB8)
  const auto result = so::ColumnInsertUnique(base, ctx.r4.u32, ctx.r5.u32);
  if (!result) {
    __imp__sub_82131CB8(ctx, base);
    return;
  }
  StoreInsertResult(base, ctx.r3.u32, *result);
}

// ~UvColumn: r3 = the column; r3 the freed head.
extern "C" REX_FUNC(sub_82131EE0) {
  ITC_SCREEN_OBJECTS_FALLBACK(82131EE0)
  const auto head = so::ColumnDestroy(base, ctx.r3.u32);
  if (!head) {
    __imp__sub_82131EE0(ctx, base);
    return;
  }
  ctx.r3.u64 = *head;
}

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

// Find: as the column's.
extern "C" REX_FUNC(sub_82130290) {
  ITC_SCREEN_OBJECTS_FALLBACK(82130290)
  const auto it = so::MapFind(base, ctx.r4.u32, KeyAt(base, ctx.r5.u32));
  if (!it) {
    __imp__sub_82130290(ctx, base);
    return;
  }
  StoreIterator(base, ctx.r3.u32, *it);
}

// InsertUnique: as the column's (the value a key and a column).
extern "C" REX_FUNC(sub_821321F8) {
  ITC_SCREEN_OBJECTS_FALLBACK(821321F8)
  const auto result = so::MapInsertUnique(base, ctx.r4.u32, ctx.r5.u32);
  if (!result) {
    __imp__sub_821321F8(ctx, base);
    return;
  }
  StoreInsertResult(base, ctx.r3.u32, *result);
}

// ~UvMap: r3 = the map; r3 the freed head.
extern "C" REX_FUNC(sub_82132F58) {
  ITC_SCREEN_OBJECTS_FALLBACK(82132F58)
  const auto head = so::MapDestroy(base, ctx.r3.u32);
  if (!head) {
    __imp__sub_82132F58(ctx, base);
    return;
  }
  ctx.r3.u64 = *head;
}
