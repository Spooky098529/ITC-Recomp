// The course scenery as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/course-scenery.md, the layouts in
// docs/known-data.md: 0x826C5514, "scenery list", "pool object"). The
// scenery's manager, its object pools and the lists of areas and models it
// streams are guest data shared with the recompiled code, so these work on
// guest memory, with the originals' loads and stores in their order. The
// originals' stack temporaries aren't written (a key's size comes back by
// value), except where an original reads past what it wrote there
// (Scenery::LimitLargeModels' table). Adapters: src/hooks/course_scenery.cpp.

#pragma once

#include <cstdint>

#include "archive/archive.h"

namespace scenery_lists {
struct DrawCalls;
}  // namespace scenery_lists
namespace car_scene_draw {
struct SceneDrawCalls;
}  // namespace car_scene_draw

namespace course_scenery {

// The manager's pointers ([0x826C5514] by Scenery::Init, [0x826C551C] by
// Scenery::Create; the same object).
constexpr uint32_t kManager = 0x826C5514;

// --- The calls into code that still takes the guest context ---

// Made by the adapters (src/hooks/course_scenery.cpp): `function` runs with
// r3 to r7 set to `args` (`count` of them; the other registers as they
// are), r1 = `frame` (the native function's frame, as the original's call
// site has it, its back chain written) and the link register `lr` (the
// original's return address); returns its r3.
// `load`, when set, is Archive::Load for the pools' load slots' plain C++
// (CourseSet::Load, AreaLights::Load, AreaModel::Load), which
// ObjectPools::Load then calls directly; the adapters set it only where
// those may run (the archive's native queue and the native heap). Without
// it, the slots are called through `call`.
struct Calls {
  void* user;
  uint32_t (*call)(void* user, uint32_t function, const uint32_t* args, int count, uint32_t frame,
                   uint32_t lr);
  const archive::LoadCall* load = nullptr;
};

// --- Pool objects (PoolObject::; 40 bytes of header) ---
// +0 the vtable (0 the key, 1 the load, 2 the reset), +4 the data, +8 its
// size, +12 the data used, +16 the index, +20 and +24 the previous and next
// on the pool's list, +32 the flags (0x1 in use, 0x8 loaded, 0x10 seen this
// update, 0x20 not seen, about to be released), +36 a countdown.
constexpr uint32_t kObjectFlags = 32;

// PoolObject::AllocData (sub_820FB8B8): `data` the object's +4 (the data,
// its size, the data used): data + used, used up by the size rounded up to
// 64. No check against the size.
uint32_t AllocData(uint8_t* base, uint32_t data, uint32_t size);
// PoolObject::InUse (sub_820FB8E0): the flags & 1.
uint32_t InUse(uint8_t* base, uint32_t object);
// PoolObject::IsLoaded (sub_820FB8F0), IsVisible (sub_820FB908) and
// IsExpiring (sub_820FB920): 1 when the flags have 0x1 and 0x8, and 0x10 or
// 0x20 for the last two.
uint32_t IsLoaded(uint8_t* base, uint32_t object);
uint32_t IsVisible(uint8_t* base, uint32_t object);
uint32_t IsExpiring(uint8_t* base, uint32_t object);
// PoolObject::SetVisible (sub_820FB938) and SetExpiring (sub_820FB968):
// `on` (its low word) not 0: nothing unless the object is loaded (0x1 and
// 0x8), then 0x10 (or 0x20) set and the other cleared; 0: 0x10 (or 0x20)
// cleared.
void SetVisible(uint8_t* base, uint32_t object, uint32_t on);
void SetExpiring(uint8_t* base, uint32_t object, uint32_t on);
// PoolObject::ClearLoadFlags (sub_820FB998): flags &= ~0x3C.
void ClearLoadFlags(uint8_t* base, uint32_t object);
// PoolObject::MarkLoaded (sub_820FB9A8): flags 0x2 cleared, 0x8 set.
void MarkLoaded(uint8_t* base, uint32_t object);
// PoolObject::Reset (sub_820FB9D8): flags = 0, +36 = -1, data used = 0.
void ResetObject(uint8_t* base, uint32_t object);
// The vtables' key slots: PoolObject::Key (sub_820FB9F0, no key: size 0,
// returns 0), AreaLights::Key (sub_820F1C20: 4 bytes at +84) and
// AreaModel::Key (sub_820F1CD8: 4 bytes at +64). Each returns the key's
// address and its size in `size`.
uint32_t PoolObjectKey(uint32_t object, uint32_t* size);
uint32_t AreaLightsKey(uint32_t object, uint32_t* size);
uint32_t AreaModelKey(uint32_t object, uint32_t* size);
// PoolObject::PoolObject (sub_820FBB20): `object` with `own_size` bytes of
// its own, then `data_size` of data, at `index` in its pool: the base
// vtable 0x82006848, its data, the next object's address, the flags and
// countdown cleared.
void ConstructObject(uint8_t* base, uint32_t object, uint32_t own_size, uint32_t data_size,
                     uint32_t index);

// PoolObject::SetKey (sub_820FBA00): the object's key buffer and size
// through its vtable's slot 0 (the native ones above, else through
// `calls`); when both aren't 0, the key at `key` copied there (memcpy).
// Returns the key buffer (the original's r3). `stack` is the caller's r1.
uint32_t SetKey(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t object, uint32_t key);
// PoolObject::KeyEquals (sub_820FBA60): 1 when the object has no key (size
// or buffer 0) or its key's bytes equal those at `key`, else 0.
uint32_t KeyEquals(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t object,
                   uint32_t key);

// --- The pools (ObjectPools::; the set +0 the count, +4 the block) ---
// A pool's 24-byte record: +0 the object count, +4 and +8 the object and data
// sizes, +12 its first object, +16 the free list's head, +20 the used
// list's.

// ObjectPools::Pool (sub_820FBB10): the pool's record.
uint32_t PoolRecord(uint8_t* base, uint32_t set, uint32_t pool);
// ObjectPools::Object (sub_820FC0E8): the pool's object `index` (no bounds
// check).
uint32_t PoolObjectAt(uint8_t* base, uint32_t set, uint32_t pool, uint32_t index);
// ObjectPools::Acquire (sub_820FBF08): the free list's head, marked in use
// with its key set, moved to the used list's head; its index, or -1 when
// the pool has none free.
uint32_t Acquire(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set, uint32_t pool,
                 uint32_t key);
// ObjectPools::Find (sub_820FBE80): the index of the first object on the
// used list in use whose key matches (KeyEquals), or -1.
uint32_t Find(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set, uint32_t pool,
              uint32_t key);
// ObjectPools::Load (sub_820FBFD0): the object, when not 0, loaded through
// its vtable's slot 1 (the native loads directly when `calls.load` is set,
// else through `calls`); returns it.
uint32_t LoadObject(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set,
                    uint32_t pool, uint32_t index);
// ObjectPools::Release (sub_820FC038): the object reset through its
// vtable's slot 2 (PoolObject::Reset natively, else through `calls`), moved
// from the used list to the free list's head; returns the object that
// followed it on the used list.
uint32_t Release(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set, uint32_t pool,
                 uint32_t index);
// ObjectPools::Init (sub_820FBBE8): `count` pools from their 24-byte
// descriptors (+4 the object size, +8 the data size, +12 and +16 the object
// and data counts, +20 the factory) in one block of physical memory
// (XPhysicalAlloc(size, -1, 64, 4)); objects made by the factory (through
// `calls`) or as plain pool objects. False, with nothing done, when the
// native heap isn't active (the original runs). `r3` is what the original
// leaves there (the last factory's return, else the block).
bool Init(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set, uint32_t count,
          uint32_t descriptors, uint32_t* r3);
// ObjectPools::Shutdown (sub_820FBDC8): every object reset through its
// vtable's slot 2 (PoolObject::Reset natively, else through `calls`), the block freed (XPhysicalFree) and the set cleared.
// False, with nothing done, when the native heap isn't active. Returns in
// `r3` the block (the original's r3).
bool Shutdown(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set, uint32_t* r3);

// --- The scenery's lists (SceneryList::; 28 bytes) ---
// +0 the pool, +4 the items done, +8 the count, +12 the table's count, +16
// the pointer array, +20 the 16-byte items (+0 the distance, a float; +4 the
// mask; +8 the area and +10 the model, halfwords; +12 the object), +24 the
// table.

// SceneryConfig::ModelAsset (sub_820F1530): `config`'s area table (+28):
// -1 when `area` isn't below its count (+16); else the area's first model
// asset + `model` when that's within its last, else -1.
uint32_t ModelAsset(uint8_t* base, uint32_t config, uint32_t area, uint32_t model);
// Scenery::PoolSizes (sub_820F2030): each of the five pools' data size into
// `out` (five words) for the configuration `config`: 0 the size of
// [[config + 24]]'s asset; 1 0; 2 the size of the area table's +8 asset
// (when not negative); 3 the areas' light assets (+20 of each 12-byte entry,
// up to the first negative or the count), summed in mode 0 ([config] 0)
// else the largest, `area_counts` +0 and +4 = the areas; 4 the areas' model
// assets (ModelAsset), summed or the largest the same way, `model_counts`
// +0 = their count and +4 = it capped at 16 (16 stored there first). The
// assets' sizes by Archive::AssetSize's plain C++. Returns r3 as the
// original leaves it: its last ModelAsset's or AssetSize's answer, or the
// light entry (zero-extended) or -1 that ended the areas.
uint64_t PoolSizes(uint8_t* base, uint32_t area_counts, uint32_t model_counts, uint32_t out,
                   uint32_t config);
// SceneryList::Clear (sub_820F1588): the items done and the count = 0.
void ListClear(uint8_t* base, uint32_t list);
// SceneryList::Add (sub_820F1888): the item with this area and model (the
// registers' low words compared with the halfwords), searched from the
// last, gets the smaller distance and the mask ORed in; else a new item
// (no check for a full list). Returns what the original leaves in r3: the
// last halfword it read, or `r3` when the list was empty.
uint32_t ListAdd(uint8_t* base, uint32_t list, double distance, uint32_t area, uint32_t model,
                 uint32_t mask, uint32_t r3);
// SceneryList::Sort (sub_820F1930): the pointer array's items `first` to
// `last` sorted by distance (the original's quicksort, the middle as the
// pivot).
void ListSort(uint8_t* base, uint32_t list, int32_t first, int32_t last);
// Scenery::LimitLargeModels (sub_820F2B80): when list B holds more than
// the manager's +104 items, large models past the 5th (10th with two
// views) among the first +104 are swapped with the next item after them
// that isn't large. The table of thresholds by area is written where the
// original keeps it, at `stack` - 80 (below the caller's r1), and read from
// guest memory, as an area above 4 reads what's past it.
void LimitLargeModels(uint8_t* base, uint32_t manager, uint32_t stack);
// ModelList::Init (sub_820F1A20): the 528-byte list made empty.
void ModelListInit(uint8_t* base, uint32_t list);
// Scenery::ClearGlobal (sub_820F1480): [0x826C5514] = 0.
void ClearGlobal(uint8_t* base);

// The physical bytes the scenery holds, kept by AllocPhysical and
// FreePhysical.
constexpr uint32_t kPhysicalHeld = 0x826C5518;
// Scenery::AllocPhysical (sub_820F1490): XPhysicalAlloc(size, -1, 0, 4)
// (the layer's AllocPhysical), its allocated size (XPhysicalSize, the
// layer's PhysicalSize: the user's call, 2026-11-06) added to
// [0x826C5518]; returns the block. Only while hooks::NativeHeapActive().
uint32_t AllocPhysical(uint8_t* base, uint32_t size);
// Scenery::FreePhysical (sub_820F14E8): the block's allocated size taken
// off [0x826C5518], then XPhysicalFree. Only while
// hooks::NativeHeapActive().
void FreePhysical(uint8_t* base, uint32_t block);

// Scenery::UnloadPools (sub_820F1D20; 112-byte frame below `stack`, its
// back chain written): ObjectPools::Shutdown on the manager's pools; +188
// = 0 when set, +180 = 0, pool 0's block (+184) freed (FreePhysical) and
// +184 = 0 when there; list B's table, items and pointer array (+168,
// +164, +160) freed when there, list B (+144, 28 bytes) cleared (XMemSet);
// list A's (+140, +136, +132) the same, list A (+116) cleared. False, with
// nothing done, when the native heap isn't active (the original runs).
// `r3` is what the original leaves there (XMemSet's destination, +116).
bool UnloadPools(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                 uint32_t* r3);
// Scenery::Unload (sub_820F2B38; 96-byte frame): the visible areas' block
// (+224) freed and +224 = 0 when there, then UnloadPools. As UnloadPools,
// false with nothing done without the native heap; `r3` as it leaves it.
bool Unload(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager, uint32_t* r3);
// Scenery::Destroy (sub_820F2AF8): ClearGlobal, the manager at [0x826C551C]
// freed (FreePhysical, whatever it holds) and the pointer cleared. Returns
// 0 (FreePhysical's r3). Only while hooks::NativeHeapActive().
uint32_t Destroy(uint8_t* base);

// The manager's pointer Scenery::Create sets (the same object as kManager's).
constexpr uint32_t kCreated = 0x826C551C;
// Scenery::Create (sub_820F2F88): the 248-byte manager from physical memory
// (AllocPhysical) into [0x826C551C]; +200, +196 = 0x38435, +204, +208, +212
// (the floats at 0x820067B8, 0x820067B4, 0x82001FAC, 0x820067B0, their bits
// copied), +224 = 0; the texture provider at +228 made (TextureProvider::
// TextureProvider's plain C++, 64 slots) unless +228 is 0; then Init.
// Returns what Init leaves in r3 ([0x826C5514] + 144). Only where the
// models' lifetime is native (hooks::ModelsNativeLifetime: the native heap
// and the texture provider's allocation).
uint32_t Create(uint8_t* base);
// Scenery::Setup (sub_820F3020): SetupPools, then the visible areas: +216
// = 0, +220 = the area count (+92), +224 = 272 x that (AllocPhysical), each
// entry's +4 = 0 and +8 = the model count (+100, read before). `stack` is
// its r1. Returns the visible areas' block (its r3). Only while the native
// heap is active and memcpy is native (native::Buffers_NativeMemcpy).
uint32_t Setup(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
               uint32_t config);
// Scenery::SetupPools (sub_820F2720): [0x826C5518], +180, +184 = 0; the
// 44-byte configuration copied to +8 (memcpy: the library's for native
// callers, its original through `calls` where that declines); mode 2 with
// +12 = 0 becomes [[+40]]. PoolSizes into its frame's +80; list A (+116:
// pool 3, the area count, its pointer array, 16-byte items and 12-byte
// table from AllocPhysical, the last two cleared with XMemSet) and list B
// (+144: pool 4, the model count) the same; the tables filled from the
// configuration's areas (A: the area, 0, its light asset, for each whose
// asset isn't negative; B: the area, the model, its asset by ModelAsset,
// for each not negative); the five pools' 24-byte descriptors in its frame's
// +112 (pool 0 with +36: a block of pool 0's size from AllocPhysical at
// +184, its size at +180; pool 4's +188 = [0x826A5134] unless
// [0x826A5130] is below its size, which is then 0), ObjectPools::Init, +192
// = 0. `stack` is its r1. Returns Init's r3.
uint32_t SetupPools(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                    uint32_t config);
// Scenery::Init (sub_820F1360): [0x826C5514] = `manager`, then through that
// global (read again before each store, as the original): +52, +56 = 0;
// +60 to +107 cleared in six 8-byte pieces (XMemSet, sub_822962B0); +108,
// +112, +184, +188 = 0; the two lists +116 and +144 (28 bytes each)
// cleared; +172 = +176 = -1; +192 = 0.
void Init(uint8_t* base, uint32_t manager);
// SceneryList::ReleaseUnused (sub_820F17C0): the used objects of `list`'s
// pool in the manager's pools (the manager is the pool set): loaded and not
// seen this update: marked about to be released with a countdown of 2, or,
// already marked, counted down and released (ObjectPools::Release) at 0;
// else the seen flag cleared. Returns what the original leaves in r3 (the
// last call's).
uint32_t ReleaseUnused(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                       uint32_t list);
// SceneryList::LoadAll (sub_820F1598): the archive's queue length first
// (Archive::QueueLength's plain C++ with `native_queue`, else through
// `calls`); from the list's done count (+4) to its count (+8), each item's
// object (+12) = 0, then its key (+8) looked up in the list's table (+24,
// +12 entries of 12 bytes: the key, the asset, +8 the object); with a
// match whose object is 0 while the queue is below 480, one taken from the
// list's pool (+0) by the key's address and loaded (ObjectPools::Acquire
// and Load, the index not checked) into the entry's +8, the manager's +172
// and +176 = the pool and the index; with an object then, the item's +12 =
// it, SetExpiring and SetVisible with 0 on it, +4 up by 1. Returns 1 when
// +4 reached +8, else 0.
uint32_t ListLoadAll(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                     uint32_t list, bool native_queue);
// Scenery::IsLoaded (sub_820F1F68; 112-byte frame below `stack`, its back
// chain written): while +192 is set, ListLoadAll on list A (+116), then
// list B (+144), 0 unless each answers 1; then the object last asked for
// (+172 the pool, +176 its index, 0 if either is negative; ObjectPools::
// Object, 0 for none) must be loaded unless it isn't in use
// (PoolObject::InUse, IsLoaded); then +192 = 0 (when set) and 1, else 0.
uint32_t SceneryIsLoaded(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                         bool native_queue);
// SceneryList::Stream (sub_820F16C0): the queue length as LoadAll; for each
// item: its object in the list's pool by its key's address (ObjectPools::
// Find), else, when the queue is below 960, one taken (Acquire) and, when
// it's there, loaded (+172, +176 = the pool and the index); with one, the
// item's +12 = it (ObjectPools::Object), SetExpiring and SetVisible with 0;
// without, +12 = 0. Returns 1 when +4 equals +8, else 0.
uint32_t ListStream(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                    uint32_t list, bool native_queue);
// Scenery::StartLoad (sub_820F1E00): the course set taken from pool 2 and
// loaded (+112, +172 = 2, +176 its index); in mode 0 (the configuration's
// +0, the manager's +8) both lists emptied and filled with every entry of
// their tables (distance [0x82000A34], mask 0); with the configuration's
// +36 (the manager's +44) set, pool 0's object the same way into +108;
// +192 = 1. Returns what the original leaves in r3.
uint32_t StartLoad(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager);

// --- What a view sees (collect.cpp) ---
//
// The views' masks: the OR of the bits (+12) of the 16-byte records at
// 0x82602F80 (14) whose view (+0) is `view` and whose camera pass (+8; 592
// bytes each from 0x826BAC00) is enabled (its +588 not 0). The distances
// are from the viewport's eye (+1600 x, +1608 z), the far distance its +72.
// Each returns the r3 the original leaves (`r3` when it makes no call); its
// stack temporaries (the corners' copy) aren't written.

// Scenery::CollectAreas (sub_820F3098): every area of the course set's area
// file (the manager's +112, its +1056; little-endian count, each entry by
// DataFile::GetEntry), its rectangle of corners 0 and 2 (16-byte points
// from the entry's +32): 0.0 with the eye inside; else, for each corner in
// turn, half the far distance when the eye moved by that (x, then z, each
// way) is inside, else the nearest corner so far. Added to list A (+116,
// model 0, the view's mask) within the far distance + 70.0.
uint32_t CollectAreas(uint8_t* base, uint32_t stack, uint32_t manager, uint32_t view,
                      uint32_t viewport, uint32_t r3);
// Scenery::CollectModels (sub_820F32D0): every model of list A's areas (the
// entry's big-endian count at +0, 96-byte records from +96: +0 the model's
// number, four corners from +32): the nearest corner, kept within the far
// distance plus the float at 0x8200257C, 0.0 with the eye inside, added to
// list B (+144) with the view's mask when that isn't 0.
uint32_t CollectModels(uint8_t* base, uint32_t stack, uint32_t manager, uint32_t view,
                       uint32_t viewport, uint32_t r3);

// What Scenery::Update's original leaves in its caller's registers that its
// callee LimitLargeModels can read below its frame (the 64-bit words its
// callees' register saves write there): the caller's r25 and r26 (saved by
// CollectAreas and CollectModels, which don't change them) and r3 (the
// manager, its r31).
struct UpdateRegisters {
  uint64_t r25, r26, r3;
};

// Scenery::Update (sub_820F4558): `stack` is its caller's r1 (its frame is
// the 208 bytes below, its back chain written; the adapter makes its
// register saves, __savegprlr_18 and f31 at `stack` - 128). The far
// distance (viewport 0's +72) read once; the 14 camera passes of the table
// at 0x82602F80 that are enabled (+588) set from their viewports
// (cameras::FromViewport, then CameraPass::Update through `calls` from
// FromViewport's 112-byte frame with its return address, as FromViewport's
// adapter); both lists emptied; CollectAreas for each view (0 to +12, the
// viewports' pointer read again each time), list A sorted and cut to +96
// when above 1 item; CollectModels the same, list B sorted, LimitLargeModels
// (with the register saves its table's reads past the 5th area read: the
// words its own __savegprlr_27 writes, and CollectModels' r25 and r26 below
// them), cut to +104. Mode 0 (+8): LoadAll on both lists, else Stream; the
// visible areas (+216, 272-byte entries at +224) from list A's loaded items
// with a mask and list B's loaded models of their area within the far
// distance (each marked seen); not in mode 0, ReleaseUnused on both. Returns
// the r3 the original leaves (its last call's, or the last word loaded
// into r3).
uint32_t Update(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                const UpdateRegisters& registers, bool native_queue);

// Scenery::DrawSetLists (sub_820F52A8): r4's pass description (+20 its
// camera pass, +24 and +28 the draw keys): the texture provider at the
// manager's +228 gets the course set's XPR (+64) in slot 1 (its vtable's
// slot 4: TextureProvider::Set natively, else a guest call); [0x8296B90C] =
// +24, [0x8296B900] = +28; the camera pass's view (+16) copied to
// 0x8265ECA0 (Matrix::Copy, noted for interpolation as its wrapper does);
// then the set's lists drawn with the camera pass's view-projection (+144,
// copied to `stack` - 384 + 192) and an identity world (+80), each model's
// +48 = the provider and +436 = the set's +1060, shape 0 and the range
// (the floats at 0x82002814 and 0x82006804, 0, 0) at +176, +160 or +144:
// every model of +1596; those of +2124 and +2652 that ModelList::IsBEntryOn
// and IsCEntryOn pick (their draw state reset first); and the entries of
// +3180 that PointList::IsEntryOn picks by PointList::Draw (+612, +608 as
// the models', +616 from the race's table at [0x826D82E8]). The draws'
// plain C++ is called directly (SceneryModel::DrawShape with the caller's
// r24 = the view-projection's address, as the original's); where one
// declines, with nothing done, its adapter runs through `calls` (so `ctx`'s
// r24 must be that address too). Its stack temporaries (the identity, the
// copy and the ranges) are written in its frame, as its callees read them.
// Returns the r3 the original leaves.
uint32_t DrawSetLists(uint8_t* base, const scenery_lists::DrawCalls& calls, uint32_t stack,
                      uint32_t manager, uint32_t pass);
// The view-projection's address DrawSetLists draws with (the camera pass's
// +144), its r24.
uint32_t DrawSetListsViewProjection(uint8_t* base, uint32_t pass);

// Scenery::RecordHeadlightProjectors (sub_820F34C8, headlights.cpp): r4's
// pass description (+12 the player, +16 the viewport entry, +20 the camera
// pass, +24 and +28 the render queue's keys). In its 640-byte frame below
// `stack` (its back chain written; the original's temporaries at their
// offsets): +8 = the camera pass, set by cameras::FromViewport, then
// CameraPass::Update through `calls` from FromViewport's 112-byte frame (as
// its adapter). With Vehicle::Get(0): the six 8-slot headlight arrays
// (0x8296B9E0 position, 0x8296BA60 BeamShape, 0x8296BAE0 and 0x8296BB60 the
// colors, 0x8296BBE0 the direction, 0x8296BC60 the scene's +5424), the
// colors cleared first; for each of the scene's first 8 models (List::At on
// [0x826C2E44] + 5028, count +5036) of type 3 with a car (+5048) and +32 1
// or 2, the next slot (0 and 1 swapped for pass 2; at most 8) from its
// matrix (+4304, memcpy) and the scene's lights (+5248 on), and for slots 0
// to 4 a projector (Projector::Build, its multiplies noted for frame
// interpolation) recorded on the scene's command list (header 5, command 7
// vertex c(0x77 + 4 x slot) x4 = the matrix, copied as Matrix::Copy with
// its interpolation note, command 36) and handed to the render queue (key
// 371 for pass 2, else 150; f1 0.0), the render manager's getters called
// before every command list call as the original's; then 0x8265ED00 = the
// scene's +5328. Always: [0x8296B90C] = +24, [0x8296B900] = +28, 0x8265ECE0
// = the camera pass's eye (+400). Its callees' plain C++ runs inside the
// checks of their diagnostics' wraps; guest calls (the getters, the queue,
// CameraPass::Update, the originals of Projector::Build where Perspective
// declines, of memcpy and of the command list's calls where theirs do) go
// through `calls` with the original's return addresses. Leaves
// flush-to-zero off. Returns the r3 the original leaves (its last call's).
uint64_t RecordHeadlightProjectors(uint8_t* base, const car_scene_draw::SceneDrawCalls& calls,
                                   uint32_t stack, uint32_t description);

// Scenery::DrawArea (sub_820F3B70, draw_area.cpp): a visible area (272
// bytes: +0 its mask, +4 the model count, +12 its lights' object, +16 the
// models' entries) for the pass description (+8 the camera pass object,
// +20 the camera pass, +24 and +28 the keys, +32 a mask): nothing unless
// the description's +32, the manager's +196 and the area's +0 share a bit;
// RecordHeadlightProjectors; in its 400-byte frame below `stack` the
// identity (+80, +208) and the pass's view-projection (+144, Matrix::Copy
// noted for interpolation), the view at 0x8265ECA0, the keys
// (0x8296B908 = 163, 0x8296B90C, 0x8296B900); each model's shapes drawn
// (the shadow passes 10, 13, 16: Model::DrawShapeDepth with the keys at
// 0x8296B8F8 and 0x8296B8FC; else the textures set in the provider at the
// manager's +228, SceneryModel::DrawShape, then for types 0, 1, 2 with
// post effect 3, 7 or 11 set and its +52 0 the depth again with keys 116
// or 119); then with +196 & 0x400 outside the shadow passes, the light
// groups of each entry of the area's light file (its count little-endian),
// the description made the billboard set. The draws' guest calls (and
// their originals where their plain C++ declines) go through `draws`, the
// headlights' and light groups' through `calls`. `caller_r28` is the
// caller's r28 (the shadow passes' depth draws' interpolation context).
// The original's r3 isn't given (its caller doesn't read it). Its adapter
// sets ctx's r24 to the camera pass while it runs (SceneryModel::DrawShape's
// adapter reads it where its plain C++ declines).
void DrawArea(uint8_t* base, const scenery_lists::DrawCalls& draws,
              const car_scene_draw::SceneDrawCalls& calls, uint32_t stack, uint32_t manager,
              uint32_t description, uint32_t area, uint32_t caller_r28);

// --- The files' parses (the loads' callbacks) ---
//
// Model::Model, Model::Load, XPR::Create and the model lists' loads are
// their plain C++, so these run natively only where Model::Load's adapter
// does (hooks::ModelsNativeLifetime, the adapters' condition). Where a
// callee's plain C++ declines, its original runs through `calls` (by its
// address, the original's return address) from 512 bytes below `stack`,
// below the frames the originals would have.

// CourseSet::Parse (sub_820F1A70): from the first file (+3808, entries by
// DataFile::GetEntry with r5 = 1): entry 0 into the XPR at +64 (XPR::XPR,
// XPR::Create), entry 1's entry 0 into the model at +96 (Model::Model,
// Model::Load), entries 2 to 6 into the lists at +528, +1064, +1596, +2124,
// +2652 (ModelList::Load) and 7 into +3180 (ModelList::LoadPoints); from the
// second (+3812): entries 0 to 2 to +1056, +1060, +1592 when there. Its
// frame (128 bytes below `stack`) and the lists' below it get their back
// chains. Returns the r3 the original leaves (the last entry looked up).
uint32_t CourseSetParse(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set);
// AreaLights::Parse (sub_820F11C0): +80 = entry 0 of the file at +4 when
// there is one. Returns that entry (0 for none).
uint32_t AreaLightsParse(uint8_t* base, uint32_t lights);
// AreaModel::Parse (sub_820F1258): from the file at the manager's +188:
// entry 0 into the model at +80 (Model::Load, r5 = 0) and +56 = +80, else
// +56 = 0; entry 1 into the XPR at +528 (XPR::Create) and +60 = +528, else
// +60 = 0. Returns the r3 the original leaves (XPR::Create's; else
// Model::Load's, the model's +412; else 0).
uint32_t AreaModelParse(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t model);
// AreaLights::OnLoaded (sub_820F1C30, Archive::Load's callback): Parse,
// then PoolObject::MarkLoaded. AreaModel::OnLoaded (sub_820F1CE8): the
// same with AreaModel::Parse (its frame 96 bytes below `stack`, the back
// chain written). Each returns the object (the r3 MarkLoaded leaves).
uint32_t AreaLightsOnLoaded(uint8_t* base, uint32_t lights);
uint32_t AreaModelOnLoaded(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t model);

// --- The pools' loads (their vtables' slot 1) and the course set's
// callback ---
//
// Each queues its file through `load` (Archive::Load with r7 = r8 = 0) and
// returns the r3 the original leaves: the last load's serial (-1 as li's,
// else zero-extended), or the asset id it skipped (lwz's, zero-extended).

// CourseSet::Load (sub_820F2520, slot 1 of 0x82006784): PoolObject::
// ClearLoadFlags, +1056 = +1060 = 0; its two assets from the configuration's
// area table ([[0x826C5514] + 36]: +12 the first, +8 the second); each not
// negative: the first's block from AllocPhysical (its size by Archive::
// AssetSize) at +3808, the second's from the object's data (AllocData) at
// +3812; a negative one's word 0. Then each not negative loaded into its
// word's block, the last of them with CourseSet::OnLoaded (0x820F22B8) and
// the set as its context, the other with none. Only while
// hooks::NativeHeapActive().
uint64_t CourseSetLoad(uint8_t* base, uint32_t set, const archive::LoadCall& load);
// AreaLights::Load (sub_820F22F8, slot 1 of 0x8200676C): ClearLoadFlags,
// +80 = 0; the asset of list A's table entry (the manager's +140, count
// +128, 12-byte entries: the area and model halfwords, the asset) whose
// halfwords are the object's +84 and +86, else -1; its size (Archive::
// AssetSize) taken from the object's data (AllocData) and the asset loaded
// there with AreaLights::OnLoaded (0x820F1C30) and the object.
uint64_t AreaLightsLoad(uint8_t* base, uint32_t lights, const archive::LoadCall& load);
// AreaModel::Load (sub_820F23C0, slot 1 of 0x82006778): ClearLoadFlags,
// +56 = +60 = 0; the asset of list B's table entry (+168, count +156) whose
// halfwords are the object's +64 and +66, else -1, loaded into the shared
// load buffer (the manager's +188) with AreaModel::OnLoaded (0x820F1CE8)
// and the object.
uint64_t AreaModelLoad(uint8_t* base, uint32_t model, const archive::LoadCall& load);
// CourseSet::OnLoaded (sub_820F22B8, Archive::Load's callback; its frame 96
// bytes below `stack`, the back chain written): CourseSet::Parse, the first
// file's block (+3808) freed (FreePhysical), PoolObject::MarkLoaded.
// Returns the set (the r3 MarkLoaded leaves). As the parses, only where
// Model::Load's adapter runs natively, and with the native heap.
uint32_t CourseSetOnLoaded(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set);

// --- The pools' objects: made and reset ---
//
// As the parses, natively only where Model::Load's adapter is (they make,
// release and destroy models), the originals through `calls` from 512 bytes
// below `stack` where a callee's plain C++ declines.

// Scenery::MakePoolObject (sub_820F2638, the pools' factory): `pool` 0 and
// 1 a PoolObject with the vtable 0x8200674C or 0x82006758 and +56 = 0; 2
// CourseSet::CourseSet; 3 a PoolObject with the vtable 0x8200676C and an
// XPR at +56 (XPR::XPR); 4 AreaModel::AreaModel; others (unsigned) and a
// null `object` nothing. The sizes and the index go to PoolObject::
// PoolObject. Returns `object`.
uint32_t MakePoolObject(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t pool,
                        uint32_t object, uint32_t own_size, uint32_t data_size, uint32_t index);
// CourseSet::CourseSet (sub_820F2478): PoolObject::PoolObject, the vtable
// 0x82006784, CourseSet::InitParts(+64), +3808, +3812, +1056, +1060 = 0,
// the lists at +528 and +1064 made empty (ModelList::Init) with +1592 = 0
// between, then +1596, +2124, +2652 and +3180, the last's +608 to +620 = 0
// (each list skipped when its address is 0, as the original checks).
void CourseSetConstruct(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set,
                        uint32_t own_size, uint32_t data_size, uint32_t index);
// CourseSet::InitParts (sub_820F2230): XPR::XPR at +0, Model::Model at +32,
// ModelList::Init at +464, +1000, +1532, +2060, +2588, +3116, the last's
// +608 to +620 = 0.
void CourseSetInitParts(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t parts);
// AreaModel::AreaModel (sub_820F1C68): PoolObject::PoolObject, the vtable
// 0x82006778, Model::Model at +80 with the vtable 0x82006764, XPR::XPR at
// +528, +56 = +60 = 0.
void AreaModelConstruct(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t model,
                        uint32_t own_size, uint32_t data_size, uint32_t index);
// CourseSet::Reset (sub_820F1138, slot 2): XPR::Release(+64),
// Model::Release(+96), ModelList::Release of the six lists (their models'
// Model::Release and vtable slot 0 with 1: the scenery models'
// SceneryModelDestruct natively), PoolObject::Reset. Returns the set.
uint32_t CourseSetReset(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set);
// AreaModel::Reset (sub_820F12F8, slot 2): the model at +56 released
// (Model::Release) and +56 = 0 when there; the XPR at +60 (XPR::Release) and
// +60 = 0 the same; PoolObject::Reset. Returns the object.
uint32_t AreaModelReset(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t model);
// The scenery models' destructor (sub_820F1208, slot 0 of 0x82006764):
// Model::~Model, then with bit 0 of `flags` operator delete. Returns the
// model.
uint32_t SceneryModelDestruct(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t model,
                              uint32_t flags);

// --- The texture animations (the course set's +1060) ---

// TextureAnimations::Advance (sub_820F11A8): the set at `course_set` +
// 1060, when not 0, advanced (AnimationSet::Advance). Returns what the
// original leaves in r3 (0, the set, or its last animation).
uint32_t AdvanceTextureAnimations(uint8_t* base, uint32_t course_set);
// AnimationSet::Advance (sub_82146B20): each animation of the set (count
// +0, then offsets from the end of the offset table) advanced. Returns the
// last animation, or the set when it has none.
uint32_t AdvanceAnimationSet(uint8_t* base, uint32_t set);
// AnimationSet::FindByTexture (sub_82146A38; probably): the index of the
// set's first animation whose texture word (+20 + 4 x its frame count; for
// a count not above 0 the word at guest address 4, as the original reads
// it) equals `texture`, or -1 (the original's r3, 64 bits).
uint64_t FindAnimationByTexture(uint8_t* base, uint32_t set, uint32_t texture);
// Animation::Advance (sub_82146AC0): a flipbook animation (+0 the frame
// count, +4 the current frame, +8 the tick, +12 the length, +16 each frame's
// start tick): back to the start past its length, the next frame when its
// start is reached, the tick up by 1.
void AdvanceAnimation(uint8_t* base, uint32_t animation);

}  // namespace course_scenery
