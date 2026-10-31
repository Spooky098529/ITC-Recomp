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
#include "models/model_load.h"

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
// The destructors, each nothing done unless its check (the *Destructible
// below) passed; each returns the r3 the original leaves.
// AccessoryFile::~AccessoryFile (sub_820F0DB8): the vtable, Reset, then the
// list node's vtable. Reset's r3, or the file's when Reset called nothing.
uint32_t FileDestruct(uint8_t* base, uint32_t file);
// AccessorySkeleton::~AccessorySkeleton (sub_820EFFD8): the vtables, the
// job's destructor inline (+240, +220 twice, +216), the file (+180), +176 =
// the list node's vtable, then SkeletonBase::~SkeletonBase (its adapter
// leaves r3: the skeleton).
uint32_t SkeletonDestruct(uint8_t* base, uint32_t skeleton);
// Accessory::~Accessory (sub_820EF310): the vtable, the skeleton, the files
// (+500, +464), then PartModel::~PartModel (its r3).
uint32_t AccessoryDestruct(uint8_t* base, uint32_t accessory);
// AccessoryEntry::Destroy (sub_820F0988): r3 = the entry's slot; when it
// holds an entry, ~Accessory (+48), ~AccessoryFile (+4), then FreeData on
// the slot (its r3); else r3 the slot.
uint32_t EntryDestroy(uint8_t* base, uint32_t slot);
bool FileDestructible(uint8_t* base, uint32_t file);
bool SkeletonDestructible(uint8_t* base, uint32_t skeleton);
bool AccessoryDestructible(uint8_t* base, uint32_t accessory);
bool EntryDestroyable(uint8_t* base, uint32_t slot);

// Accessories::ReleaseAll (sub_820F0AD8): for each of the three entries
// that's set, its count - 1, and at 0 AccessoryEntry::Destroy on its slot.
// The r3 it leaves: the last Destroy's, or nullopt (r3 as given) with none.
std::optional<uint32_t> SetReleaseAll(uint8_t* base, uint32_t set);
// Whether ReleaseAll runs natively: the entries distinct, and each one it
// destroys EntryDestroyable.
bool SetReleasable(uint8_t* base, uint32_t set);
// Accessories::Release (sub_820F0A90; its row in scene-loaders.md): index 3
// is none (nullopt: r3 as given). Else r3 = the slot (+4 + 4 x index); when
// it holds an entry, its count - 1, and at 0 AccessoryEntry::Destroy on the
// slot (a tail call: its r3). The r3 it leaves.
std::optional<uint32_t> SetRelease(uint8_t* base, uint32_t set, int32_t index);
// Whether Release runs natively: the entry it would destroy (if any)
// EntryDestroyable.
bool SetReleaseNative(uint8_t* base, uint32_t set, int32_t index);

// --- The skeleton's set-up ---

// The string a node's name holds for the swinging node (not read).
constexpr uint32_t kSwingNodeName = 0x82006710;
// AccessorySkeleton::Reset (sub_820EFB30): `sp` the caller's stack pointer
// (its 192-byte frame's identity rows at +80 to +143 as the original leaves
// them). Each node's local entry from the node file's rows, the angle and
// its speed 0.0, the two matrices identity (memcpy, native::Buffers_Memcpy),
// the vectors, +152 = 0. Returns memcpy's r3 (the skeleton + 352).
uint32_t SkeletonReset(uint8_t* base, uint32_t skeleton, uint32_t sp);
// AccessorySkeleton::SetUp (sub_820F0070): once (+260 = 0) and with its file
// loaded (+192 = 2): SkeletonBase::SetUp(it, [+196], 0, 1); with nodes, the
// first whose name holds kSwingNodeName (strstr) into +256, +260 = 1 and
// Reset (from the 128-byte frame below `sp`). Returns +260; nullopt, with
// nothing done, where SkeletonBase::SetUp's plain C++ declines (the
// original runs).
std::optional<uint32_t> SkeletonSetUp(uint8_t* base, uint32_t skeleton, uint32_t sp);

// AccessoriesBase::~AccessoriesBase (sub_820F0B58): the base's vtable,
// ReleaseAll, then the list node's vtable. ReleaseAll's r3, or the set's
// when it destroyed nothing. The adapter checks SetReleasable first.
uint32_t SetBaseDestruct(uint8_t* base, uint32_t set);
// Accessories::~Accessories (sub_820F0FE0, deleting; slot 0 of 0x82006740):
// the vtable, the base's destructor, and with `flags` bit 0 operator delete
// (memory::crt::OperatorDelete; the adapter checks it's a heap block).
// Returns the set.
uint32_t SetDestruct(uint8_t* base, uint32_t set, uint32_t flags);

// --- The draw's helpers ---

// The material groups Accessory::SetTexture walks: the words at
// 0x820066D0 (0 and 1).
constexpr uint32_t kMaterialGroups = 0x820066D0;
// Accessory::SetTexture (sub_820EF568): +1040 = `index`; once loaded
// (+1048), the texture provider's (+80) vtable +12 with r4 = 0 (`texture_set`:
// a guest call the adapter makes from the 160-byte frame below `sp`, whose
// back chain is stored here; its r3), and when that answer's +20 is
// above `index`: for the shape (0 only) or every shape (`shape` < 0, +412),
// both groups, each material: its 8 words at +176 = index, the 8 at +144 =
// 0. Returns the r3 the original leaves (the last call's, or the
// accessory's).
uint64_t AccessorySetTexture(
    uint8_t* base, uint32_t accessory, uint32_t index, int32_t shape, uint32_t sp,
    const std::function<uint64_t(uint32_t provider, uint32_t frame)>& texture_set);
// Matrix::FromScaleRotationTranslation (sub_820EF660): from the 208-byte
// frame below `sp` (its identity rows at +96 and the angle at +80 as the
// original leaves them): out = the identity (memcpy, native::Buffers_Memcpy:
// the adapter checks it copies), the scale on the diagonal, turned by
// RotateX, RotateY, RotateZ (matrix_math's plain C++, the turns at the
// frame's -80) by the three angles, the position into row 3. Returns out.
uint32_t FromScaleRotationTranslation(uint8_t* base, uint32_t out, uint32_t angles,
                                      uint32_t position, uint32_t scale, uint32_t sp);

// Whether a part model is constructed natively (models::CanAllocatePointers
// for its texture provider): the constructors' adapters check it.
bool AccessoryConstructible(uint8_t* base);

// --- The load's end ---

// Accessory::Finish's guest calls where a callee's plain C++ declines, each
// from Finish's 112-byte frame at `frame` (its back chain stored): `load`
// Model::Load's original (r3 to r5), `set_up` AccessorySkeleton::SetUp's
// original (its r3). `model_load` Model::Load's plain C++'s own guest calls.
struct FinishCalls {
  models::LoadCalls model_load;
  std::function<void(uint32_t model, uint32_t file, uint32_t xpr, uint32_t frame)> load;
  std::function<uint32_t(uint32_t skeleton, uint32_t frame)> set_up;
};
// Whether Accessory::Finish runs natively: its two files' resets
// (FileResetNative).
bool Finishable(uint8_t* base, uint32_t accessory);
// Accessory::Finish (sub_820EF368): `sp` the caller's stack pointer. Nothing
// once loaded (+1048) or until the model's file (+464) is loaded (+476 = 2);
// then Model::Load(it, [+480], [+516]), and with `release` FileReset on +464
// and +500; then when AccessorySkeleton::SetUp(+544) answers non-zero, the
// anchor (+992) = the 16 bytes at [skeleton +88] + 16 x [skeleton +256], and
// +1048 = 1. Returns +1048. `ran_load` (if given) says whether Model::Load
// was called.
uint32_t AccessoryFinish(uint8_t* base, uint32_t accessory, uint32_t release, uint32_t sp,
                         const FinishCalls& calls, bool* ran_load = nullptr);
// AccessoryEntry::OnLoaded (sub_820F0820, the entry file's callback; r6 the
// entry): with the file's data ([entry + 20]), FileSetData (not owned) on
// the accessory's +464, +500 and +724 with DataFile::GetEntry 0, 1 and 2;
// Accessory::Finish(accessory, 1), then Accessory::SetTexture(accessory,
// [+1040], -1) (its provider's slot through `texture_set`), all from the
// 112-byte frame below `sp`. Returns SetTexture's r3.
uint64_t EntryOnLoaded(
    uint8_t* base, uint32_t entry, uint32_t sp, const FinishCalls& calls,
    const std::function<uint64_t(uint32_t provider, uint32_t frame)>& texture_set,
    bool* ran_load = nullptr);

// --- The acquisition ---

// AccessoryEntry::OnLoaded, the callback AccessoriesAcquireEntry hands
// AccessoryFile::Load.
constexpr uint32_t kEntryOnLoaded = 0x820F0820;
// Accessories::AcquireEntry (sub_820F09E0): `sp` the caller's stack pointer
// (its 128-byte frame below). Index 3 is none (0). With no entry at +4 + 4 x
// index: AccessoryEntry::New stored there (0: returns 0),
// Accessory::SetTexture(accessory, texture, -1), then AccessoryFile::Load
// (entry + 4, 23008 + index, 0, OnLoaded, entry) through `load`; with one,
// its count + 1 and SetTexture. Returns the entry + 48 (the accessory).
// SetTexture's provider slot through `texture_set`.
uint32_t SetAcquireEntry(
    uint8_t* base, uint32_t set, int32_t index, uint32_t texture, uint32_t sp,
    const archive::LoadCall& load,
    const std::function<uint64_t(uint32_t provider, uint32_t frame)>& texture_set);
// Accessories::Acquire (sub_820F0EB8, slot 1 of 0x82006740): AcquireEntry
// from the 96-byte frame below `sp`, and with an accessory, the set's scale
// (+16) into its +1024 (two doublewords). Returns the accessory or 0.
uint32_t SetAcquire(
    uint8_t* base, uint32_t set, int32_t index, uint32_t texture, uint32_t sp,
    const archive::LoadCall& load,
    const std::function<uint64_t(uint32_t provider, uint32_t frame)>& texture_set);

}  // namespace accessories
