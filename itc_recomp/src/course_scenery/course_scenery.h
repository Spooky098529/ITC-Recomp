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
struct Calls {
  void* user;
  uint32_t (*call)(void* user, uint32_t function, const uint32_t* args, int count, uint32_t frame,
                   uint32_t lr);
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
// its vtable's slot 1 (always through `calls`: no load is native); returns
// it.
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
// Scenery::StartLoad (sub_820F1E00): the course set taken from pool 2 and
// loaded (+112, +172 = 2, +176 its index); in mode 0 (the configuration's
// +0, the manager's +8) both lists emptied and filled with every entry of
// their tables (distance [0x82000A34], mask 0); with the configuration's
// +36 (the manager's +44) set, pool 0's object the same way into +108;
// +192 = 1. Returns what the original leaves in r3.
uint32_t StartLoad(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager);

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
// Animation::Advance (sub_82146AC0): a flipbook animation (+0 the frame
// count, +4 the current frame, +8 the tick, +12 the length, +16 each frame's
// start tick): back to the start past its length, the next frame when its
// start is reached, the tick up by 1.
void AdvanceAnimation(uint8_t* base, uint32_t animation);

}  // namespace course_scenery
