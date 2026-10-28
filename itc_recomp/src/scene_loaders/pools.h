// The scene's loading objects as plain C++ (B7 in docs/rexglue-removal.md;
// the functions are in docs/functions/scene-loaders.md, the fields in
// docs/known-data.md): the first pool set's allocation, the pool nodes'
// base, the loading objects' queries, the car model's embedded loads, and
// the name helpers (XPR::FindByName, NameMatches). The data is guest data
// shared with the recompiled code, so these work on guest memory, with the
// originals' loads and stores in their order. The originals' stack
// temporaries and saved registers (below the stack pointer, or in their own
// frames) aren't written: nothing reads them after the return. Adapters:
// src/hooks/scene_loaders.cpp.

#pragma once

#include <cstdint>

namespace scene_loaders {

// The first pool set's pointer.
constexpr uint32_t kFirstSet = 0x826C2E40;
constexpr uint32_t kFirstSetBytes = 140;

// The pool node's vtable (PoolNode, the base of every pool object).
constexpr uint32_t kPoolNodeVtable = 0x8206CCB0;

// A load slot's state (+20 of its 24 bytes): 2 loading, 3 arrived.
constexpr int32_t kSlotLoading = 2;
constexpr int32_t kSlotArrived = 3;

// The car parts: 216 slots (24 bytes from +96, their states at +116), the
// part models (464 bytes from +5280), the pool set at +106000 (its +8 the
// model set: 0 a, 1 b).
constexpr uint32_t kPartsCount = 216;
constexpr uint32_t kPartsStates = 116;
constexpr uint32_t kPartsModels = 5280;
constexpr uint32_t kPartsSet = 106000;
constexpr uint32_t kSlotBytes = 24;
constexpr uint32_t kPartModelBytes = 464;

// --- The pool set ---

// ScenePools::MakeA (sub_820C3C40): [0x826C2E40] = VirtualAlloc(0, 140,
// MEM_COMMIT, read-write) (the allocation layer's pages), its +4 = 0.
// Returns the set (the original's r3). Only over the allocation layer (the
// adapter's condition).
uint32_t MakeA(uint8_t* base);

// sub_820D0960: [out] = VirtualAlloc(0, size, MEM_COMMIT, read-write).
// Returns the pages (the original's r3). Only over the allocation layer.
uint32_t AllocPages(uint8_t* base, uint32_t out, uint32_t size);

// --- The pool node ---

// PoolNode::PoolNode (sub_82594050): the vtable, +4 the list, +8 and +12 =
// 0, appended at the list's end (12 bytes: +0 first, +4 last, +8 count),
// each word read again where the original reads it.
void PoolNodeConstruct(uint8_t* base, uint32_t node, uint32_t list);
// PoolNode::~PoolNode (sub_825940B0): the vtable; when its list is set,
// unlinked and the count down by one.
void PoolNodeDestruct(uint8_t* base, uint32_t node);

// --- The loading objects' queries ---

// CarModel::IsLoaded (sub_820D3C28): 0 while an XPR2 slot (+3972 + 24i, two)
// or the node file's slot (+3748) is loading, else 1.
uint32_t CarModelIsLoaded(uint8_t* base, uint32_t car_model);
// Pool4Object::IsLoaded (sub_820D6A08): 0 while its slot (+100) is loading,
// else 1.
uint32_t Pool4IsLoaded(uint8_t* base, uint32_t object);
// Pool4Object::SetRecord (sub_820D6A60): memcpy(+28, record, 52). False,
// with nothing copied, where the copy isn't made natively (a destination in
// the physical views, where memcpy's override records copies for the
// renderer's mirrors, or ranges c_runtime::Memcpy refuses).
bool Pool4SetRecord(uint8_t* base, uint32_t object, uint32_t record);

// CarParts::SlotOf (sub_820D6D48): the slot a part kind (0-15) maps to,
// from `base_slot` and `choice`; `r3` itself above 15 (unsigned). In 64
// bits, as the original adds its registers (kinds 8 and 9 take `choice`'s
// low word only).
uint64_t PartsSlotOf(uint64_t r3, uint32_t kind, uint64_t base_slot, uint64_t choice);
// CarParts::Part (sub_820D6E10): set a: 0 for kind 16, else the slot by
// SlotOf; set b: 0 for kinds 7, 9 and 14, else the kind; the slot's part
// model when it has arrived, else 0; 0 for other sets.
uint32_t PartsPart(uint8_t* base, uint32_t parts, int32_t kind, uint32_t base_slot,
                   uint32_t choice);
// CarParts::IsLoaded (sub_820D7258): set a: 1 unless one of the 216 slots
// is loading; set b: the same over the first 17; 0 for other sets.
uint32_t PartsIsLoaded(uint8_t* base, uint32_t parts);
// sub_820D2270: Model::MaterialCount for groups 0 and 1 and Model::Material
// for each material, over each shape (+412) of a part model (nothing for
// 0); the results unused. Returns the r3 the original leaves (the last
// call's answer, the model with no shapes, 0 for none).
uint32_t PartWalkMaterials(uint8_t* base, uint32_t model);
// CarParts::WalkMaterials (sub_820D6EB8): sub_820D2270 on each arrived part
// model among the first 216 (set 0) or 187. Returns the r3 the original
// leaves (the last call's, else `parts`).
uint32_t PartsWalkMaterials(uint8_t* base, uint32_t parts);

// Accessories::AllLoaded (sub_820F07D8): 0 when one of the set's three
// entries (+4 + 4i) is set with its +1096 = 0, else 1.
uint32_t AccessoriesAllLoaded(uint8_t* base, uint32_t set);
// Interior::IsLoaded (sub_820D7790): sets a and b: 1 unless one of the six
// slots (+180 + 24i) is loading; 0 for other sets; then 0 unless
// Accessories::AllLoaded on the accessories' set.
uint32_t InteriorIsLoaded(uint8_t* base, uint32_t interior);

// --- Names ---

// NameMatches (sub_820CE800): 0 unless the lengths match; then each
// character pair compared after toupper (as signed bytes), a `*` in either
// matching any character; 1 when all match.
uint32_t NameMatches(uint8_t* base, uint32_t a, uint32_t b);
// XPR::Count (sub_82596F20): +20, or 0 when +16 (the records) is 0.
uint32_t XprCount(uint8_t* base, uint32_t xpr);
// XPR::RecordName (sub_82596F40): [+16 + 16 x index + 12] for an index
// below +20 (unsigned), else 0.
uint32_t XprRecordName(uint8_t* base, uint32_t xpr, uint32_t index);
// XPR::FindByName (sub_820D09B8): the first record whose name matches
// (NameMatches(name, record's)), or -1.
int32_t XprFindByName(uint8_t* base, uint32_t xpr, uint32_t name);

}  // namespace scene_loaders
