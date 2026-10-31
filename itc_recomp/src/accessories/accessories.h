// The interior accessories natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/accessories.md): the set at [0x826C5510]
// with its three 1,104-byte entries, the accessory (a car part model with a
// swinging skeleton) and its 36-byte file class. Plain C++ over guest memory,
// store for store as the recompiled functions; the guest calls they can't
// make themselves (the archive's callbacks, the texture provider's slot) are
// handed back to the adapters (src/hooks/accessories.cpp).

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "archive/archive.h"

namespace accessories {

constexpr uint32_t kSet = 0x826C5510;  // [it] the set
constexpr uint32_t kSetBytes = 32;
constexpr uint32_t kEntryBytes = 1104;
constexpr uint32_t kFileBytes = 36;
// The vtables: the set's base and its own, the file's, the list node's base.
constexpr uint32_t kSetBaseVtable = 0x8200672C;
constexpr uint32_t kSetVtable = 0x82006740;
constexpr uint32_t kFileVtable = 0x82006738;
// The floats 1.0 and 0.0 the originals load.
constexpr uint32_t kOne = 0x82000A30;
constexpr uint32_t kZero = 0x82000A34;
// AccessoryFile::OnArchiveDone, the callback AccessoryFile::Load hands the
// archive.
constexpr uint32_t kFileOnArchiveDone = 0x820F0C20;

// --- The file (AccessoryFile, 36 bytes) ---

// AccessoryFile::AccessoryFile (sub_820F0C50): the vtable, +4 and +8 = -1,
// +12 to +20 = 0, +24 = 1, +28 and +32 = 0.
void FileConstruct(uint8_t* base, uint32_t file);
// AccessoryFile::SetData (sub_820F0BF0): +16 = data, +24 = owned, +20 = 0,
// +4 and +8 = -1, +12 = 2 (loaded), +28 and +32 = 0.
void FileSetData(uint8_t* base, uint32_t file, uint32_t data, uint32_t owned);
// AccessoryFile::Load (sub_820F0C90): +4 = id, +20 = Archive::AssetSize(id);
// with a buffer, +16 = it and +24 = 0, else +16 = VirtualAlloc(size) (the
// allocation layer's pages) and +24 = 1; then Archive::Load(id, +16,
// OnArchiveDone, the file) and +12 = 1, +28 = callback, +32 = context, +8 =
// the serial. Returns the serial (Archive::Load's r3).
int64_t FileLoad(uint8_t* base, uint32_t file, uint32_t id, uint32_t buffer, uint32_t callback,
                 uint32_t context, const archive::LoadCall& load);
// What AccessoryFile::OnArchiveDone (sub_820F0C20) leaves to its tail call.
struct Callback {
  uint32_t function;  // 0: none (no tail call)
  uint32_t context;   // r6 for it
};
// AccessoryFile::OnArchiveDone: nothing unless the file's +8 is `serial`;
// then +12 = 2 (loaded), and its callback (+28) with +32, when it has one.
Callback FileOnArchiveDone(uint8_t* base, uint32_t serial, uint32_t file);
// AccessoryFile::FreeData (sub_820F08D0): when the word at `slot` holds a
// block, VirtualFree(block, 0, MEM_RELEASE) (memory::FreePages) and the word
// = 0. The r3 it leaves: 0 when the word held none, else VirtualFree's.
// nullopt, with nothing done, for a block the allocation layer didn't give
// (the original runs).
std::optional<uint32_t> FileFreeData(uint8_t* base, uint32_t slot);

// AccessoryFile::Reset (sub_820F0D30): when owned (+24), Archive::Cancel(+8)
// while loading (+12 = 1) or FreeData(+16) once loaded (2); then +12 = 0,
// +4 and +8 = -1, +16 and +20 = 0, +24 = 1, +28 and +32 = 0. The r3 it
// leaves: Cancel's or FreeData's, or nullopt (r3 as given) when neither ran.
// The adapter checks a loaded block is the allocation layer's first
// (FileResetNative).
std::optional<uint32_t> FileReset(uint8_t* base, uint32_t file);
// Whether FileReset runs natively for `file`: no loaded, owned block the
// allocation layer didn't give.
bool FileResetNative(uint8_t* base, uint32_t file);

// --- The set ([0x826C5510], 32 bytes) ---

// AccessoriesBase::AccessoriesBase (sub_820F07B8): the base's vtable, the
// three entries (+4, +8, +12) = 0.
void SetBaseConstruct(uint8_t* base, uint32_t set);
// Accessories::Accessories (sub_820F0E60): the base, the vtable, +16 to +28
// = 1.0 (+28 written first and last, as the original). Returns the set.
uint32_t SetConstruct(uint8_t* base, uint32_t set);
// Accessories::New (sub_820F0F48): VirtualAlloc(32) (the allocation layer's
// pages) and Accessories::Accessories on it; 0 when there's none.
uint32_t SetNew(uint8_t* base);
// Accessories::Create (sub_820F1038): when [0x826C5510] is 0, New's answer
// into it. The r3 it leaves: New's, or nullopt (r3 as given) when the set
// was there.
std::optional<uint32_t> SetCreate(uint8_t* base);
// Accessories::SetScale (sub_820F0F00): the 16 bytes at `scale` copied to
// each entry's +1072 (the accessory's scale) and the set's +16, as two
// doublewords each.
void SetScale(uint8_t* base, uint32_t set, uint32_t scale);

// Accessories::Free (sub_820F0F90): r3 = the set's slot; when it holds a
// set, `destroy` (the set's vtable slot 0 with r4 = 0, the destructor
// without delete: a guest call the adapter makes from the 96-byte frame
// below `sp`, whose back chain is stored here), then FreeData on the slot.
// The r3 it leaves: 0 for an empty slot, else FreeData's. The adapter
// checks the set is the allocation layer's pages first.
uint32_t SetFree(uint8_t* base, uint32_t slot, uint32_t sp,
                 const std::function<void(uint32_t set, uint32_t frame)>& destroy);
// Accessories::Destroy (sub_820F1078): when [0x826C5510] holds a set,
// Free(0x826C5510) (a tail call: its frame below `sp`). The r3 it leaves:
// 0x826C5510 sign-extended (lis and addi) when there's none, else Free's.
uint64_t SetDestroy(uint8_t* base, uint32_t sp,
                    const std::function<void(uint32_t set, uint32_t frame)>& destroy);

// --- The entry (1,104 bytes) and the accessory (entry + 48, 1,056 bytes) ---

constexpr uint32_t kAccessoryVtable = 0x820066C8;
// The skeleton's vtables (+0, its second base +176), the second base's base
// (the list node's), the job's (+216, +220 first and then) and its
// delegate's (+240).
constexpr uint32_t kSkeletonVtable = 0x82006700;
constexpr uint32_t kSkeletonVtable2 = 0x820066FC;
constexpr uint32_t kListNodeVtable = 0x82010B5C;
constexpr uint32_t kJobVtable = 0x820066DC;
constexpr uint32_t kJobFunctorFirst = 0x82000AB4;
constexpr uint32_t kJobFunctor = 0x82002908;
constexpr uint32_t kJobDelegate = 0x8200833C;

// AccessorySkeleton::AccessorySkeleton (sub_820F03F0): r3 = the skeleton
// (accessory + 544), `sp` the caller's stack pointer (its 224-byte frame's
// words as the original leaves them: +80 and +84 0, the identity at +96 to
// +159). SkeletonBase::SkeletonBase, the vtables, the file at +180, the job
// at +216, the two matrices identity (memcpy, native::Buffers_Memcpy: the
// adapter checks it copies), the vectors and angles. Returns the skeleton.
uint32_t SkeletonConstruct(uint8_t* base, uint32_t skeleton, uint32_t sp);
// Accessory::Accessory (sub_820EF258): PartModel::PartModel (the adapter
// checks models::CanAllocatePointers first), the vtable, the two files, the
// skeleton (from the 96-byte frame below `sp`), the anchor, angles, scale,
// texture index, visible and loaded. Returns the accessory.
uint32_t AccessoryConstruct(uint8_t* base, uint32_t accessory, uint32_t sp);
// AccessoryEntry::New (sub_820F0920): VirtualAlloc(1104) (the allocation
// layer's pages); +0 = 1, the file at +4, the accessory at +48 (from the
// 96-byte frame below `sp`). Returns it, or 0.
uint32_t EntryNew(uint8_t* base, uint32_t sp);
// Whether a part model is constructed natively (models::CanAllocatePointers
// for its texture provider): the constructors' adapters check it.
bool AccessoryConstructible(uint8_t* base);

}  // namespace accessories
