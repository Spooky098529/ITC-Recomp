// The scene models' parts as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/scene-models.md, the fields and globals in
// docs/known-data.md): the parts' links, the records' defaults and copies,
// the accessories' set's getter, the sound loads' callback and the car
// model's pages. The data is guest data shared with the recompiled code, so
// these work on guest memory, with the originals' loads and stores in their
// order. The originals' stack temporaries (below the stack pointer, or in
// their own frames) aren't written: nothing reads them after the return.
// Adapters: src/hooks/scene_models.cpp.

#pragma once

#include <cstdint>

namespace scene_models {

// --- Getters and links ---

// The accessories' set (probably), made once by sub_820F1038.
constexpr uint32_t kAccessories = 0x826C5510;

// sub_820F0E50: [0x826C5510].
uint32_t Accessories(uint8_t* base);

// SceneModel::Accessory (sub_820D97B8, probably): an interior's loading
// object's +140 (0-2, or 3 for none) picks the accessories' set's entry
// [set + 4 + 4i]; returns it + 48, or 0 for 3 or an entry of 0.
uint32_t Accessory(uint8_t* base, uint32_t interior);

// sub_82175EE0, Archive::Load's callback for SceneModel::SetUp's two sound
// assets: `context` holds the slot i; byte 0x826D8350 + i up by one (wrapping
// at 256), and when it's then 2 (i read again), 0x826D83AC + 4i = 1.
constexpr uint32_t kSoundLoadsDone = 0x826D8350;
constexpr uint32_t kSoundLoaded = 0x826D83AC;
void OnSoundLoaded(uint8_t* base, uint32_t context);

// sub_820E7378: the wheels' +16776 = value.
void WheelsSetRecord(uint8_t* base, uint32_t wheels, uint32_t value);
// sub_820E70B8: +24 = the car model.
void SetCarModel(uint8_t* base, uint32_t object, uint32_t car_model);
// SceneParts::Link (sub_820DF1A8): +24 = the car model, +28 = [model + 24] +
// 3760 (the skeleton).
void PartsLink(uint8_t* base, uint32_t parts, uint32_t car_model);
// sub_820DF1C0: the parts' +37248 = value.
void PartsSetRecord(uint8_t* base, uint32_t parts, uint32_t value);
// SceneInterior::Link (sub_820E37D0): +24 = the car model, +28 = [model +
// 24] + 3760, +32 = [model + 24] + 4048 (each [model + 24] read again).
void InteriorLink(uint8_t* base, uint32_t interior, uint32_t car_model);
// sub_820E3658: +11452 = value.
void SetWord11452(uint8_t* base, uint32_t object, uint32_t value);

// --- Copies ---

// CarModel::SetRecord (sub_820D3C70): memcpy of the 3,696-byte car record to
// the car model's +32. Returns the copy's destination (memcpy's r3). False,
// with nothing copied, where the copy can't be made natively (a destination
// in the physical views, where memcpy's override records copies for the
// renderer's mirrors, or ranges c_runtime::Memcpy refuses); the original runs
// there.
constexpr uint32_t kCarRecordBytes = 3696;
bool CarModelSetRecord(uint8_t* base, uint32_t car_model, uint32_t record);

// CarRecord::Copy (sub_820D7EE8): the 3,696-byte car record `from` into
// `to`, field by field as the original (its padding words not copied): +0;
// +16 to +159 (doublewords); +160, +164; the 64-byte vinyl layer and sticker
// records at +176 (3), +368 (3) and +560 + 384k (6 each, k 0-7): their +0,
// +4, +8, +16 to +31, +32 (a float, lfs and stfs) and +48 to +63; memcpy of
// +3632 (48 bytes); +3680 to +3692. False, with nothing written, where it
// can't be done natively: either record in the physical views, the records
// overlapping, or the memcpy refused (then after the fields before it,
// which the original writes the same again).
bool CarRecordCopy(uint8_t* base, uint32_t to, uint32_t from);

// SceneModelRecord::Copy (sub_820DB290): the 4,244-byte record `from` into
// `to`: +0, the car record (CarRecordCopy, +16), then memcpy of +3712 (60
// bytes), +3772 (120), +3892 (144), +4036 (156) and +4192 (52). False where
// it can't be done natively: either record in the physical views, the
// records overlapping, or a copy refused (then after the parts before it,
// which the original writes the same again).
constexpr uint32_t kSceneModelRecordBytes = 4244;
bool SceneModelRecordCopy(uint8_t* base, uint32_t to, uint32_t from);

// --- Records' defaults ---

// SceneModelRecord::SceneModelRecord (sub_820DB240): CarRecordSetW on +16,
// Record12Reset on +4080 and +4092.
void SceneModelRecordConstruct(uint8_t* base, uint32_t record);

// sub_820D45B0: the image's 1.0 (0x82000A30) into +12 of the three 16-byte
// rows of the blocks at +16, +64 and +112, and into +28 and +60 of the
// 64-byte records at +176 (3), +368 (3) and +560 + 384k (6 each, k 0-7).
void CarRecordSetW(uint8_t* base, uint32_t record);
// sub_820D3B38: +0 = -1, +4 and +8 = [0x82001BE0].
void Record12Reset(uint8_t* base, uint32_t record);
// sub_820D0A20: a 64-byte vinyl layer or sticker record: +0 = -1, +4 = 0, +8
// = 8, +16 to +28 = 0.0, 0.0, 0.0, 1.0, +32 = 0.0, +48 to +60 = 1.0 (0.0 and
// 1.0 the image's, 0x82000A34 and 0x82000A30).
void LayerReset(uint8_t* base, uint32_t layer);
// CarRecord::Reset (sub_820D0A98): the 3,696-byte car record's defaults.
// Returns the original's r3 (32, its last constant).
uint32_t CarRecordReset(uint8_t* base, uint32_t record);
// sub_820D3558: the 60-byte parts record cleared, +52 = 3.
void PartsRecordReset(uint8_t* base, uint32_t record);
// sub_820D3840: the 120-byte interior record cleared, then +28 = 1, +40 = 2,
// +52 = 3, +64 = 4, +72 = 1, +84 = 1, +88 = 1, +112 = 3.
void InteriorRecordReset(uint8_t* base, uint32_t record);
// sub_820D34B8: the 144-byte cockpit record: +0 to +16 = 0, then
// InteriorRecordReset on +24. Returns +24 (the original's r3, from its tail
// call).
uint32_t CockpitRecordReset(uint8_t* base, uint32_t record);
// sub_820D3900: the 156-byte wheels record's defaults.
void WheelsRecordReset(uint8_t* base, uint32_t record);
// sub_820D34E0: the 52-byte pool 4 record cleared, then 1.0 at +8, +12, +16,
// +24, +28, +32, +40, +44, +48.
void Pool4RecordReset(uint8_t* base, uint32_t record);

// SceneModelRecord::Reset (sub_820D9810): the 4,244-byte record's defaults:
// +0 = 1 (kind 1), then CarRecordReset (+16), PartsRecordReset (+3712),
// InteriorRecordReset (+3772), CockpitRecordReset (+3892),
// WheelsRecordReset (+4036) and Pool4RecordReset (+4192). Returns +4192
// (the original's r3, as the last leaves it).
uint32_t SceneModelRecordReset(uint8_t* base, uint32_t record);

// --- Allocation ---

// sub_820C3CA0: VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE) (the
// allocation layer's AllocPages) into [out]. Returns the pages.
uint32_t AllocPages(uint8_t* base, uint32_t out, uint32_t size);

}  // namespace scene_models
