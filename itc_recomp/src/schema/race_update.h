// The views of docs/functions/race-update.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-update.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: RaceScene
#pragma pack(push, 1)
struct RaceSceneData {
  uint32_t scenery = 0;  // +0
  uint8_t pad_4[8] = {};
  uint32_t fault_car_12 = 0;  // +12
  uint32_t car_slot_count = 0;  // +16
  uint8_t pad_20[12] = {};
  uint8_t car_slots[30016] = {};  // +32, 7 x RaceCarSlot
  uint8_t pad_30048[16] = {};
  uint8_t lights[1076] = {};  // +30064, 1 x RaceLights
  uint8_t pad_31140[12] = {};
  uint32_t screen_object = 0;  // +31152
  uint32_t screen_object_first = 0;  // +31156
  uint32_t screen_object_second = 0;  // +31160
  uint8_t pad_31164[4] = {};
};
#pragma pack(pop)
static_assert(sizeof(RaceSceneData) == 31168);
inline constexpr memory::LayoutField kRaceSceneFields[] = {{0, 4}, {12, 4}, {16, 4}, {32, 4}, {48, 4}, {64, 4}, {4304, 4}, {4320, 4}, {4336, 4}, {4352, 4}, {8592, 4}, {8608, 4}, {8624, 4}, {8640, 4}, {12880, 4}, {12896, 4}, {12912, 4}, {12928, 4}, {17168, 4}, {17184, 4}, {17200, 4}, {17216, 4}, {21456, 4}, {21472, 4}, {21488, 4}, {21504, 4}, {25744, 4}, {25760, 4}, {25776, 4}, {25792, 4}, {30032, 4}, {30064, 4}, {30068, 4}, {30072, 4}, {30080, 4}, {30176, 4}, {30272, 4}, {30368, 4}, {30464, 4}, {30560, 4}, {30656, 4}, {30752, 4}, {30848, 4}, {30944, 4}, {31040, 4}, {31136, 4}, {31152, 4}, {31156, 4}, {31160, 4}};
inline constexpr memory::Layout kRaceSceneLayout = {kRaceSceneFields, 49};

struct RaceScene : View {
  using View::View;
  static constexpr uint32_t kSize = 31168;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceSceneData* data() const { return static_cast<RaceSceneData*>(memory::HandleObject(address)); }
  static RaceScene New() {
    return RaceScene(memory::MakeHandle(new RaceSceneData(), sizeof(RaceSceneData), "RaceScene", &kRaceSceneLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceScene NewArray(uint32_t count) {
    static_assert(sizeof(RaceSceneData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceSceneData*>(::operator new(sizeof(RaceSceneData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceSceneData();
    return RaceScene(memory::MakeHandle(p, sizeof(RaceSceneData) * count, "RaceScene", &kRaceSceneLayout, sizeof(RaceSceneData)));
  }
  RaceScene at(uint32_t i) const { return RaceScene(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceSceneData* direct() const { return static_cast<RaceSceneData*>(memory::HandleOwn(address, &kRaceSceneLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceSceneLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceSceneLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the course scenery's manager ([0x826C551C], set by RaceScene::LoadScenery)
  uint32_t scenery_address() const { return address + 0; }
  uint32_t scenery_bits() const { if (auto* d = direct()) return d->scenery; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_scenery_bits(uint32_t v) const { if (auto* d = direct()) { d->scenery = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = Scenery>
  T scenery() const { return T(scenery_bits()); }
  void set_scenery(const View& w) const { set_scenery_bits(w.address); }
  void set_scenery(uint32_t v) const { set_scenery_bits(v); }
  // the car slots' count (7); Car::TickUpdate's r3 is this word's address
  uint32_t car_slot_count_address() const { return address + 16; }
  uint32_t car_slot_count_bits() const { if (auto* d = direct()) return d->car_slot_count; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_car_slot_count_bits(uint32_t v) const { if (auto* d = direct()) { d->car_slot_count = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t car_slot_count() const { if (auto* d = direct()) return d->car_slot_count; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_car_slot_count(uint32_t v) const { if (auto* d = direct()) { d->car_slot_count = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // seven car slots of 4,288 bytes, by vehicle index
  template <typename T = RaceCarSlot>
  T car_slots(uint32_t i) const { return T(address + 32 + i * T::kSize); }
  // the light set
  template <typename T = RaceLights>
  T lights() const { return T(address + 30064); }
  // the race's screen object (1024 x 576; 0 when not made)
  uint32_t screen_object_address() const { return address + 31152; }
  uint32_t screen_object_bits() const { if (auto* d = direct()) return d->screen_object; return uint32_t((memory::IsHandle(address + 31152) ? uint32_t(memory::OwnerLoad(address + 31152, 4)) : guest::LoadU32(address + 31152))); }
  void set_screen_object_bits(uint32_t v) const { if (auto* d = direct()) { d->screen_object = v; return; } if (memory::IsHandle(address + 31152)) memory::OwnerStore(address + 31152, 4, v); else guest::StoreU32(address + 31152, v); }
  template <typename T = ScreenObject>
  T screen_object() const { return T(screen_object_bits()); }
  void set_screen_object(const View& w) const { set_screen_object_bits(w.address); }
  void set_screen_object(uint32_t v) const { set_screen_object_bits(v); }
  // split screen: player 1's screen object
  uint32_t screen_object_first_address() const { return address + 31156; }
  uint32_t screen_object_first_bits() const { if (auto* d = direct()) return d->screen_object_first; return uint32_t((memory::IsHandle(address + 31156) ? uint32_t(memory::OwnerLoad(address + 31156, 4)) : guest::LoadU32(address + 31156))); }
  void set_screen_object_first_bits(uint32_t v) const { if (auto* d = direct()) { d->screen_object_first = v; return; } if (memory::IsHandle(address + 31156)) memory::OwnerStore(address + 31156, 4, v); else guest::StoreU32(address + 31156, v); }
  template <typename T = ScreenObject>
  T screen_object_first() const { return T(screen_object_first_bits()); }
  void set_screen_object_first(const View& w) const { set_screen_object_first_bits(w.address); }
  void set_screen_object_first(uint32_t v) const { set_screen_object_first_bits(v); }
  // split screen: player 2's screen object
  uint32_t screen_object_second_address() const { return address + 31160; }
  uint32_t screen_object_second_bits() const { if (auto* d = direct()) return d->screen_object_second; return uint32_t((memory::IsHandle(address + 31160) ? uint32_t(memory::OwnerLoad(address + 31160, 4)) : guest::LoadU32(address + 31160))); }
  void set_screen_object_second_bits(uint32_t v) const { if (auto* d = direct()) { d->screen_object_second = v; return; } if (memory::IsHandle(address + 31160)) memory::OwnerStore(address + 31160, 4, v); else guest::StoreU32(address + 31160, v); }
  template <typename T = ScreenObject>
  T screen_object_second() const { return T(screen_object_second_bits()); }
  void set_screen_object_second(const View& w) const { set_screen_object_second_bits(w.address); }
  void set_screen_object_second(uint32_t v) const { set_screen_object_second_bits(v); }
  // measure fault: Car::TickUpdate's halfword +0 of the car (the racer record, race-setup's Car: the paint pick), joined with the scene
  uint32_t fault_car_pick_address() const { return address + 0; }
  uint16_t fault_car_pick_bits() const { if (auto* d = direct()) return uint16_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kRaceSceneLayout, 0, 2)); return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_fault_car_pick_bits(uint16_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceSceneLayout, 0, 2, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  uint16_t fault_car_pick() const { return uint16_t(fault_car_pick_bits()); }
  void set_fault_car_pick(uint16_t v) const { set_fault_car_pick_bits(uint16_t(v)); }
  // measure fault: RaceScene::VaryCar's r3 is a car (the racer record, race-setup's Car), its word +12 (64 picks the +1580 byte), joined with the scene
  uint32_t fault_car_12_address() const { return address + 12; }
  uint32_t fault_car_12_bits() const { if (auto* d = direct()) return d->fault_car_12; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_fault_car_12_bits(uint32_t v) const { if (auto* d = direct()) { d->fault_car_12 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t fault_car_12() const { if (auto* d = direct()) return d->fault_car_12; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_fault_car_12(uint32_t v) const { if (auto* d = direct()) { d->fault_car_12 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
};

// embedded; measure: RaceScene::RemakeCarModel.r5
#pragma pack(push, 1)
struct RaceCarSlotData {
  uint32_t model = 0;  // +0
  uint8_t pad_4[12] = {};
  uint32_t record_kind = 0;  // +16
  uint8_t pad_20[12] = {};
  uint32_t record_block = 0;  // +32
  uint8_t pad_36[4236] = {};
  int32_t pool_index = 0;  // +4272
  uint8_t pad_4276[12] = {};
};
#pragma pack(pop)
static_assert(sizeof(RaceCarSlotData) == 4288);
inline constexpr memory::LayoutField kRaceCarSlotFields[] = {{0, 4}, {16, 4}, {32, 4}, {4272, 4}};
inline constexpr memory::Layout kRaceCarSlotLayout = {kRaceCarSlotFields, 4};

struct RaceCarSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 4288;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceCarSlotData* data() const { return static_cast<RaceCarSlotData*>(memory::HandleObject(address)); }
  static RaceCarSlot New() {
    return RaceCarSlot(memory::MakeHandle(new RaceCarSlotData(), sizeof(RaceCarSlotData), "RaceCarSlot", &kRaceCarSlotLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceCarSlot NewArray(uint32_t count) {
    static_assert(sizeof(RaceCarSlotData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceCarSlotData*>(::operator new(sizeof(RaceCarSlotData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceCarSlotData();
    return RaceCarSlot(memory::MakeHandle(p, sizeof(RaceCarSlotData) * count, "RaceCarSlot", &kRaceCarSlotLayout, sizeof(RaceCarSlotData)));
  }
  RaceCarSlot at(uint32_t i) const { return RaceCarSlot(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceCarSlotData* direct() const { return static_cast<RaceCarSlotData*>(memory::HandleOwn(address, &kRaceCarSlotLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceCarSlotLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceCarSlotLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // its pool 7 scene model (ScenePools::SetUpB's; 0 none)
  uint32_t model_address() const { return address + 0; }
  uint32_t model_bits() const { if (auto* d = direct()) return d->model; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_model_bits(uint32_t v) const { if (auto* d = direct()) { d->model = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = SceneModel>
  T model() const { return T(model_bits()); }
  void set_model(const View& w) const { set_model_bits(w.address); }
  void set_model(uint32_t v) const { set_model_bits(v); }
  // its scene model record (car-makers.toml's SceneModelRecord, from SceneModelRecord::FromCar)
  uint32_t record_address() const { return address + 16; }
  uint8_t* record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 16 : guest::Pointer(address + 16); }
  // the record's +0: 3 traffic
  uint32_t record_kind_address() const { return address + 16; }
  uint32_t record_kind_bits() const { if (auto* d = direct()) return d->record_kind; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_record_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->record_kind = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t record_kind() const { if (auto* d = direct()) return d->record_kind; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_record_kind(uint32_t v) const { if (auto* d = direct()) { d->record_kind = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // the record's +16 (the model block): 0 or 7 on traffic sets the block's +592
  uint32_t record_block_address() const { return address + 32; }
  uint32_t record_block_bits() const { if (auto* d = direct()) return d->record_block; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_record_block_bits(uint32_t v) const { if (auto* d = direct()) { d->record_block = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t record_block() const { if (auto* d = direct()) return d->record_block; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_record_block(uint32_t v) const { if (auto* d = direct()) { d->record_block = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // its pool 7 index (ScenePools::ClaimB; -1 none)
  uint32_t pool_index_address() const { return address + 4272; }
  uint32_t pool_index_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->pool_index); return uint32_t((memory::IsHandle(address + 4272) ? uint32_t(memory::OwnerLoad(address + 4272, 4)) : guest::LoadU32(address + 4272))); }
  void set_pool_index_bits(uint32_t v) const { if (auto* d = direct()) { d->pool_index = std::bit_cast<int32_t>(v); return; } if (memory::IsHandle(address + 4272)) memory::OwnerStore(address + 4272, 4, v); else guest::StoreU32(address + 4272, v); }
  int32_t pool_index() const { return int32_t(pool_index_bits()); }
  void set_pool_index(int32_t v) const { set_pool_index_bits(uint32_t(v)); }
};

// embedded; measure: RaceScene@+30064@+16, RaceScene@+30064
#pragma pack(push, 1)
struct RaceLightsData {
  uint32_t count = 0;  // +0
  uint32_t kind1_count = 0;  // +4
  uint32_t kind2_count = 0;  // +8
  uint8_t pad_12[4] = {};
  uint8_t entries[1056] = {};  // +16, 11 x RaceLightEntry
  uint32_t unknown_1072 = 0;  // +1072
};
#pragma pack(pop)
static_assert(sizeof(RaceLightsData) == 1076);
inline constexpr memory::LayoutField kRaceLightsFields[] = {{0, 4}, {4, 4}, {8, 4}, {16, 4}, {112, 4}, {208, 4}, {304, 4}, {400, 4}, {496, 4}, {592, 4}, {688, 4}, {784, 4}, {880, 4}, {976, 4}, {1072, 4}};
inline constexpr memory::Layout kRaceLightsLayout = {kRaceLightsFields, 15};

struct RaceLights : View {
  using View::View;
  static constexpr uint32_t kSize = 1076;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceLightsData* data() const { return static_cast<RaceLightsData*>(memory::HandleObject(address)); }
  static RaceLights New() {
    return RaceLights(memory::MakeHandle(new RaceLightsData(), sizeof(RaceLightsData), "RaceLights", &kRaceLightsLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceLights NewArray(uint32_t count) {
    static_assert(sizeof(RaceLightsData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceLightsData*>(::operator new(sizeof(RaceLightsData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceLightsData();
    return RaceLights(memory::MakeHandle(p, sizeof(RaceLightsData) * count, "RaceLights", &kRaceLightsLayout, sizeof(RaceLightsData)));
  }
  RaceLights at(uint32_t i) const { return RaceLights(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceLightsData* direct() const { return static_cast<RaceLightsData*>(memory::HandleOwn(address, &kRaceLightsLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceLightsLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceLightsLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the entries of kind 1 or 2 (11)
  uint32_t count_address() const { return address + 0; }
  uint32_t count_bits() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_count_bits(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t count() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_count(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // 3
  uint32_t kind1_count_address() const { return address + 4; }
  uint32_t kind1_count_bits() const { if (auto* d = direct()) return d->kind1_count; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_kind1_count_bits(uint32_t v) const { if (auto* d = direct()) { d->kind1_count = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t kind1_count() const { if (auto* d = direct()) return d->kind1_count; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_kind1_count(uint32_t v) const { if (auto* d = direct()) { d->kind1_count = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // 8
  uint32_t kind2_count_address() const { return address + 8; }
  uint32_t kind2_count_bits() const { if (auto* d = direct()) return d->kind2_count; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_kind2_count_bits(uint32_t v) const { if (auto* d = direct()) { d->kind2_count = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t kind2_count() const { if (auto* d = direct()) return d->kind2_count; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_kind2_count(uint32_t v) const { if (auto* d = direct()) { d->kind2_count = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // eleven entries of 96 bytes
  template <typename T = RaceLightEntry>
  T entries(uint32_t i) const { return T(address + 16 + i * T::kSize); }
  // 0 at SetUp
  uint32_t unknown_1072_address() const { return address + 1072; }
  uint32_t unknown_1072_bits() const { if (auto* d = direct()) return d->unknown_1072; return uint32_t((memory::IsHandle(address + 1072) ? uint32_t(memory::OwnerLoad(address + 1072, 4)) : guest::LoadU32(address + 1072))); }
  void set_unknown_1072_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_1072 = v; return; } if (memory::IsHandle(address + 1072)) memory::OwnerStore(address + 1072, 4, v); else guest::StoreU32(address + 1072, v); }
  uint32_t unknown_1072() const { if (auto* d = direct()) return d->unknown_1072; return uint32_t((memory::IsHandle(address + 1072) ? uint32_t(memory::OwnerLoad(address + 1072, 4)) : guest::LoadU32(address + 1072))); }
  void set_unknown_1072(uint32_t v) const { if (auto* d = direct()) { d->unknown_1072 = v; return; } if (memory::IsHandle(address + 1072)) memory::OwnerStore(address + 1072, 4, v); else guest::StoreU32(address + 1072, v); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct RaceLightEntryData {
  uint32_t kind = 0;  // +0
  uint8_t pad_4[12] = {};
  uint8_t direction[16] = {};  // +16, guest order
  uint8_t second[16] = {};  // +32, guest order
  uint8_t third[16] = {};  // +48, guest order
  uint8_t fourth[16] = {};  // +64, guest order
  uint8_t cone[16] = {};  // +80, guest order
};
#pragma pack(pop)
static_assert(sizeof(RaceLightEntryData) == 96);
inline constexpr memory::LayoutField kRaceLightEntryFields[] = {{0, 4}};
inline constexpr memory::Layout kRaceLightEntryLayout = {kRaceLightEntryFields, 1};

struct RaceLightEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceLightEntryData* data() const { return static_cast<RaceLightEntryData*>(memory::HandleObject(address)); }
  static RaceLightEntry New() {
    return RaceLightEntry(memory::MakeHandle(new RaceLightEntryData(), sizeof(RaceLightEntryData), "RaceLightEntry", &kRaceLightEntryLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceLightEntry NewArray(uint32_t count) {
    static_assert(sizeof(RaceLightEntryData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceLightEntryData*>(::operator new(sizeof(RaceLightEntryData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceLightEntryData();
    return RaceLightEntry(memory::MakeHandle(p, sizeof(RaceLightEntryData) * count, "RaceLightEntry", &kRaceLightEntryLayout, sizeof(RaceLightEntryData)));
  }
  RaceLightEntry at(uint32_t i) const { return RaceLightEntry(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceLightEntryData* direct() const { return static_cast<RaceLightEntryData*>(memory::HandleOwn(address, &kRaceLightEntryLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceLightEntryLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceLightEntryLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 1 directional (entries 0 to 2), 2 (3 to 10; probably spot lights)
  uint32_t kind_address() const { return address + 0; }
  uint32_t kind_bits() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t kind() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_kind(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the direction (earlier taken for the color): normalized into the scene lights' 0x8296B950 by RaceLights::Update
  uint32_t direction_address() const { return address + 16; }
  uint8_t* direction() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 16 : guest::Pointer(address + 16); }
  // copied to 0x8296B980
  uint32_t second_address() const { return address + 32; }
  uint8_t* second() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 32 : guest::Pointer(address + 32); }
  // copied to 0x8296B9B0
  uint32_t third_address() const { return address + 48; }
  uint8_t* third() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 48 : guest::Pointer(address + 48); }
  // kind 2
  uint32_t fourth_address() const { return address + 64; }
  uint8_t* fourth() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 64 : guest::Pointer(address + 64); }
  // kind 2: the cosines of two angles (probably the cone's outer and inner)
  uint32_t cone_address() const { return address + 80; }
  uint8_t* cone() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 80 : guest::Pointer(address + 80); }
};

// The image words docs/functions/race-update.md's code uses.
struct RaceUpdateGlobals {
  // the race data: +4 the course, +32 the mode, +48 the racer records (3,936 bytes), +0x59614 the human cars, +0x59620 the stages, +0x59AE8 the racer records' list, +0xA3AF0 the split mode (4 split screen)
  template <typename T = RaceData>
  static T race_data() { return T(guest::LoadU32(0x826C5B2Cu)); }
  static void set_race_data(const View& v) { guest::StoreU32(0x826C5B2Cu, v.address); }
  static void set_race_data(uint32_t v) { guest::StoreU32(0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the race's scenery configurations' two words, 8 bytes an entry (0 to 2 by mode, 3 courses 8 to 10)
  static constexpr uint32_t scenery_configs_address() { return 0x82602EA0u; }
  // the scene lights' directions, 16 bytes a kind 1 entry
  static constexpr uint32_t light_directions_address() { return 0x8296B950u; }
  // the kind 1 entries' +32
  static constexpr uint32_t light_second_address() { return 0x8296B980u; }
  // the kind 1 entries' +48
  static constexpr uint32_t light_third_address() { return 0x8296B9B0u; }
  // (0, 0, 0, 1) from RaceLights::Update
  static constexpr uint32_t light_origin_address() { return 0x8265ED00u; }
  // camera pass 0 (cameras.toml's CameraPass): +400 and +416 the two points Car::TickUpdate takes a direction of
  static constexpr uint32_t camera_pass0_address() { return 0x826BAC00u; }
  // the car scene (car-draw.toml's CarScene)
  template <typename T = CarScene>
  static T car_scene() { return T(guest::LoadU32(0x826C2E44u)); }
  static void set_car_scene(const View& v) { guest::StoreU32(0x826C2E44u, v.address); }
  static void set_car_scene(uint32_t v) { guest::StoreU32(0x826C2E44u, v); }
  static constexpr uint32_t car_scene_address() { return 0x826C2E44u; }
  // the racers' manager: +2308 the racers
  template <typename T = RaceManager>
  static T racers() { return T(guest::LoadU32(0x826D82F4u)); }
  static void set_racers(const View& v) { guest::StoreU32(0x826D82F4u, v.address); }
  static void set_racers(uint32_t v) { guest::StoreU32(0x826D82F4u, v); }
  static constexpr uint32_t racers_address() { return 0x826D82F4u; }
  // the viewports (camera-modes.toml's Viewports): +8 nonzero in split screen
  template <typename T = Viewports>
  static T viewports() { return T(guest::LoadU32(0x826C2CE8u)); }
  static void set_viewports(const View& v) { guest::StoreU32(0x826C2CE8u, v.address); }
  static void set_viewports(uint32_t v) { guest::StoreU32(0x826C2CE8u, v); }
  static constexpr uint32_t viewports_address() { return 0x826C2CE8u; }
  // the vehicles' input records' pointers by vehicle index (+0 the flags: 0x8 brake, 0x100 high beams)
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // 0 for player 2 followed (two human cars)
  static uint32_t followed() { return guest::LoadU32(0x826C7824u); }
  static void set_followed(uint32_t v) { guest::StoreU32(0x826C7824u, v); }
  static constexpr uint32_t followed_address() { return 0x826C7824u; }
  // 2.0 (probably): the wheels' load scale
  static float wheel_load_scale() { return guest::LoadFloat(0x820021E8u); }
  static void set_wheel_load_scale(float v) { guest::StoreFloat(0x820021E8u, v); }
  static constexpr uint32_t wheel_load_scale_address() { return 0x820021E8u; }
  // a wheel's slip above it: level 2 (not read from the image)
  static float slip_high() { return guest::LoadFloat(0x8200979Cu); }
  static void set_slip_high(float v) { guest::StoreFloat(0x8200979Cu, v); }
  static constexpr uint32_t slip_high_address() { return 0x8200979Cu; }
  // a wheel's slip above it: level 1 (not read from the image)
  static float slip_low() { return guest::LoadFloat(0x820097A0u); }
  static void set_slip_low(float v) { guest::StoreFloat(0x820097A0u, v); }
  static constexpr uint32_t slip_low_address() { return 0x820097A0u; }
  // 1/300: the screen object's speed from the km/h
  static float speed_scale() { return guest::LoadFloat(0x820086ECu); }
  static void set_speed_scale(float v) { guest::StoreFloat(0x820086ECu, v); }
  static constexpr uint32_t speed_scale_address() { return 0x820086ECu; }
  // 1.0 (copied as a word by RaceLights::Update and UploadDirectional)
  static float one() { return guest::LoadFloat(0x82000A30u); }
  static void set_one(float v) { guest::StoreFloat(0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero() { return guest::LoadFloat(0x82000A34u); }
  static void set_zero(float v) { guest::StoreFloat(0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
};

}  // namespace schema
