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
  static RaceScene New(uint8_t* base) {
    return RaceScene(base, memory::MakeHandle(new RaceSceneData(), sizeof(RaceSceneData), "RaceScene", &kRaceSceneLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceScene NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RaceSceneData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceSceneData*>(::operator new(sizeof(RaceSceneData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceSceneData();
    return RaceScene(base, memory::MakeHandle(p, sizeof(RaceSceneData) * count, "RaceScene", &kRaceSceneLayout, sizeof(RaceSceneData)));
  }
  RaceScene at(uint32_t i) const { return RaceScene(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kRaceSceneLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kRaceSceneLayout, offset, width, v); }
  // the course scenery's manager ([0x826C551C], set by RaceScene::LoadScenery)
  uint32_t scenery_address() const { return address + 0; }
  template <typename T = Scenery>
  T scenery() const { return T(base, data()->scenery); }
  void set_scenery(const View& w) const { const uint32_t v = w.address; data()->scenery = v; }
  void set_scenery(uint32_t v) const { data()->scenery = v; }
  uint32_t scenery_bits() const { return data()->scenery; }
  void set_scenery_bits(uint32_t v) const { data()->scenery = v; }
  // the car slots' count (7); Car::TickUpdate's r3 is this word's address
  uint32_t car_slot_count_address() const { return address + 16; }
  uint32_t car_slot_count() const { return data()->car_slot_count; }
  void set_car_slot_count(uint32_t v) const { data()->car_slot_count = v; }
  uint32_t car_slot_count_bits() const { return data()->car_slot_count; }
  void set_car_slot_count_bits(uint32_t v) const { data()->car_slot_count = v; }
  // seven car slots of 4,288 bytes, by vehicle index
  template <typename T = RaceCarSlot>
  T car_slots(uint32_t i) const { return T(base, address + 32 + i * T::kSize); }
  // the light set
  template <typename T = RaceLights>
  T lights() const { return T(base, address + 30064); }
  // the race's screen object (1024 x 576; 0 when not made)
  uint32_t screen_object_address() const { return address + 31152; }
  template <typename T = ScreenObject>
  T screen_object() const { return T(base, data()->screen_object); }
  void set_screen_object(const View& w) const { const uint32_t v = w.address; data()->screen_object = v; }
  void set_screen_object(uint32_t v) const { data()->screen_object = v; }
  uint32_t screen_object_bits() const { return data()->screen_object; }
  void set_screen_object_bits(uint32_t v) const { data()->screen_object = v; }
  // split screen: player 1's screen object
  uint32_t screen_object_first_address() const { return address + 31156; }
  template <typename T = ScreenObject>
  T screen_object_first() const { return T(base, data()->screen_object_first); }
  void set_screen_object_first(const View& w) const { const uint32_t v = w.address; data()->screen_object_first = v; }
  void set_screen_object_first(uint32_t v) const { data()->screen_object_first = v; }
  uint32_t screen_object_first_bits() const { return data()->screen_object_first; }
  void set_screen_object_first_bits(uint32_t v) const { data()->screen_object_first = v; }
  // split screen: player 2's screen object
  uint32_t screen_object_second_address() const { return address + 31160; }
  template <typename T = ScreenObject>
  T screen_object_second() const { return T(base, data()->screen_object_second); }
  void set_screen_object_second(const View& w) const { const uint32_t v = w.address; data()->screen_object_second = v; }
  void set_screen_object_second(uint32_t v) const { data()->screen_object_second = v; }
  uint32_t screen_object_second_bits() const { return data()->screen_object_second; }
  void set_screen_object_second_bits(uint32_t v) const { data()->screen_object_second = v; }
  // measure fault: Car::TickUpdate's halfword +0 of the car (the racer record, race-setup's Car: the paint pick), joined with the scene
  uint32_t fault_car_pick_address() const { return address + 0; }
  uint16_t fault_car_pick_bits() const { return uint16_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kRaceSceneLayout, 0, 2)); }
  void set_fault_car_pick_bits(uint16_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kRaceSceneLayout, 0, 2, v); }
  uint16_t fault_car_pick() const { return uint16_t(fault_car_pick_bits()); }
  void set_fault_car_pick(uint16_t v) const { set_fault_car_pick_bits(uint16_t(v)); }
  // measure fault: RaceScene::VaryCar's r3 is a car (the racer record, race-setup's Car), its word +12 (64 picks the +1580 byte), joined with the scene
  uint32_t fault_car_12_address() const { return address + 12; }
  uint32_t fault_car_12() const { return data()->fault_car_12; }
  void set_fault_car_12(uint32_t v) const { data()->fault_car_12 = v; }
  uint32_t fault_car_12_bits() const { return data()->fault_car_12; }
  void set_fault_car_12_bits(uint32_t v) const { data()->fault_car_12 = v; }
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
  static RaceCarSlot New(uint8_t* base) {
    return RaceCarSlot(base, memory::MakeHandle(new RaceCarSlotData(), sizeof(RaceCarSlotData), "RaceCarSlot", &kRaceCarSlotLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceCarSlot NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RaceCarSlotData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceCarSlotData*>(::operator new(sizeof(RaceCarSlotData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceCarSlotData();
    return RaceCarSlot(base, memory::MakeHandle(p, sizeof(RaceCarSlotData) * count, "RaceCarSlot", &kRaceCarSlotLayout, sizeof(RaceCarSlotData)));
  }
  RaceCarSlot at(uint32_t i) const { return RaceCarSlot(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kRaceCarSlotLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kRaceCarSlotLayout, offset, width, v); }
  // its pool 7 scene model (ScenePools::SetUpB's; 0 none)
  uint32_t model_address() const { return address + 0; }
  template <typename T = SceneModel>
  T model() const { return T(base, data()->model); }
  void set_model(const View& w) const { const uint32_t v = w.address; data()->model = v; }
  void set_model(uint32_t v) const { data()->model = v; }
  uint32_t model_bits() const { return data()->model; }
  void set_model_bits(uint32_t v) const { data()->model = v; }
  // its scene model record (car-makers.toml's SceneModelRecord, from SceneModelRecord::FromCar)
  uint32_t record_address() const { return address + 16; }
  uint8_t* record() const { return reinterpret_cast<uint8_t*>(data()) + 16; }
  // the record's +0: 3 traffic
  uint32_t record_kind_address() const { return address + 16; }
  uint32_t record_kind() const { return data()->record_kind; }
  void set_record_kind(uint32_t v) const { data()->record_kind = v; }
  uint32_t record_kind_bits() const { return data()->record_kind; }
  void set_record_kind_bits(uint32_t v) const { data()->record_kind = v; }
  // the record's +16 (the model block): 0 or 7 on traffic sets the block's +592
  uint32_t record_block_address() const { return address + 32; }
  uint32_t record_block() const { return data()->record_block; }
  void set_record_block(uint32_t v) const { data()->record_block = v; }
  uint32_t record_block_bits() const { return data()->record_block; }
  void set_record_block_bits(uint32_t v) const { data()->record_block = v; }
  // its pool 7 index (ScenePools::ClaimB; -1 none)
  uint32_t pool_index_address() const { return address + 4272; }
  int32_t pool_index() const { return data()->pool_index; }
  void set_pool_index(int32_t v) const { data()->pool_index = v; }
  uint32_t pool_index_bits() const { return std::bit_cast<uint32_t>(data()->pool_index); }
  void set_pool_index_bits(uint32_t v) const { data()->pool_index = std::bit_cast<int32_t>(v); }
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
  static RaceLights New(uint8_t* base) {
    return RaceLights(base, memory::MakeHandle(new RaceLightsData(), sizeof(RaceLightsData), "RaceLights", &kRaceLightsLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceLights NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RaceLightsData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceLightsData*>(::operator new(sizeof(RaceLightsData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceLightsData();
    return RaceLights(base, memory::MakeHandle(p, sizeof(RaceLightsData) * count, "RaceLights", &kRaceLightsLayout, sizeof(RaceLightsData)));
  }
  RaceLights at(uint32_t i) const { return RaceLights(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kRaceLightsLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kRaceLightsLayout, offset, width, v); }
  // the entries of kind 1 or 2 (11)
  uint32_t count_address() const { return address + 0; }
  uint32_t count() const { return data()->count; }
  void set_count(uint32_t v) const { data()->count = v; }
  uint32_t count_bits() const { return data()->count; }
  void set_count_bits(uint32_t v) const { data()->count = v; }
  // 3
  uint32_t kind1_count_address() const { return address + 4; }
  uint32_t kind1_count() const { return data()->kind1_count; }
  void set_kind1_count(uint32_t v) const { data()->kind1_count = v; }
  uint32_t kind1_count_bits() const { return data()->kind1_count; }
  void set_kind1_count_bits(uint32_t v) const { data()->kind1_count = v; }
  // 8
  uint32_t kind2_count_address() const { return address + 8; }
  uint32_t kind2_count() const { return data()->kind2_count; }
  void set_kind2_count(uint32_t v) const { data()->kind2_count = v; }
  uint32_t kind2_count_bits() const { return data()->kind2_count; }
  void set_kind2_count_bits(uint32_t v) const { data()->kind2_count = v; }
  // eleven entries of 96 bytes
  template <typename T = RaceLightEntry>
  T entries(uint32_t i) const { return T(base, address + 16 + i * T::kSize); }
  // 0 at SetUp
  uint32_t unknown_1072_address() const { return address + 1072; }
  uint32_t unknown_1072() const { return data()->unknown_1072; }
  void set_unknown_1072(uint32_t v) const { data()->unknown_1072 = v; }
  uint32_t unknown_1072_bits() const { return data()->unknown_1072; }
  void set_unknown_1072_bits(uint32_t v) const { data()->unknown_1072 = v; }
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
  static RaceLightEntry New(uint8_t* base) {
    return RaceLightEntry(base, memory::MakeHandle(new RaceLightEntryData(), sizeof(RaceLightEntryData), "RaceLightEntry", &kRaceLightEntryLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceLightEntry NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RaceLightEntryData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceLightEntryData*>(::operator new(sizeof(RaceLightEntryData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceLightEntryData();
    return RaceLightEntry(base, memory::MakeHandle(p, sizeof(RaceLightEntryData) * count, "RaceLightEntry", &kRaceLightEntryLayout, sizeof(RaceLightEntryData)));
  }
  RaceLightEntry at(uint32_t i) const { return RaceLightEntry(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kRaceLightEntryLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kRaceLightEntryLayout, offset, width, v); }
  // 1 directional (entries 0 to 2), 2 (3 to 10; probably spot lights)
  uint32_t kind_address() const { return address + 0; }
  uint32_t kind() const { return data()->kind; }
  void set_kind(uint32_t v) const { data()->kind = v; }
  uint32_t kind_bits() const { return data()->kind; }
  void set_kind_bits(uint32_t v) const { data()->kind = v; }
  // the direction (earlier taken for the color): normalized into the scene lights' 0x8296B950 by RaceLights::Update
  uint32_t direction_address() const { return address + 16; }
  uint8_t* direction() const { return reinterpret_cast<uint8_t*>(data()) + 16; }
  // copied to 0x8296B980
  uint32_t second_address() const { return address + 32; }
  uint8_t* second() const { return reinterpret_cast<uint8_t*>(data()) + 32; }
  // copied to 0x8296B9B0
  uint32_t third_address() const { return address + 48; }
  uint8_t* third() const { return reinterpret_cast<uint8_t*>(data()) + 48; }
  // kind 2
  uint32_t fourth_address() const { return address + 64; }
  uint8_t* fourth() const { return reinterpret_cast<uint8_t*>(data()) + 64; }
  // kind 2: the cosines of two angles (probably the cone's outer and inner)
  uint32_t cone_address() const { return address + 80; }
  uint8_t* cone() const { return reinterpret_cast<uint8_t*>(data()) + 80; }
};

// The image words docs/functions/race-update.md's code uses.
struct RaceUpdateGlobals {
  // the race data: +4 the course, +32 the mode, +48 the racer records (3,936 bytes), +0x59614 the human cars, +0x59620 the stages, +0x59AE8 the racer records' list, +0xA3AF0 the split mode (4 split screen)
  template <typename T = RaceData>
  static T race_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B2Cu, v.address); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
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
  static T car_scene(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E44u)); }
  static void set_car_scene(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E44u, v.address); }
  static void set_car_scene(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E44u, v); }
  static constexpr uint32_t car_scene_address() { return 0x826C2E44u; }
  // the racers' manager: +2308 the racers
  template <typename T = RaceManager>
  static T racers(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82F4u)); }
  static void set_racers(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82F4u, v.address); }
  static void set_racers(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82F4u, v); }
  static constexpr uint32_t racers_address() { return 0x826D82F4u; }
  // the viewports (camera-modes.toml's Viewports): +8 nonzero in split screen
  template <typename T = Viewports>
  static T viewports(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2CE8u)); }
  static void set_viewports(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2CE8u, v.address); }
  static void set_viewports(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2CE8u, v); }
  static constexpr uint32_t viewports_address() { return 0x826C2CE8u; }
  // the vehicles' input records' pointers by vehicle index (+0 the flags: 0x8 brake, 0x100 high beams)
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // 0 for player 2 followed (two human cars)
  static uint32_t followed(uint8_t* base) { return guest::LoadU32(base, 0x826C7824u); }
  static void set_followed(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7824u, v); }
  static constexpr uint32_t followed_address() { return 0x826C7824u; }
  // 2.0 (probably): the wheels' load scale
  static float wheel_load_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_wheel_load_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t wheel_load_scale_address() { return 0x820021E8u; }
  // a wheel's slip above it: level 2 (not read from the image)
  static float slip_high(uint8_t* base) { return guest::LoadFloat(base, 0x8200979Cu); }
  static void set_slip_high(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200979Cu, v); }
  static constexpr uint32_t slip_high_address() { return 0x8200979Cu; }
  // a wheel's slip above it: level 1 (not read from the image)
  static float slip_low(uint8_t* base) { return guest::LoadFloat(base, 0x820097A0u); }
  static void set_slip_low(uint8_t* base, float v) { guest::StoreFloat(base, 0x820097A0u, v); }
  static constexpr uint32_t slip_low_address() { return 0x820097A0u; }
  // 1/300: the screen object's speed from the km/h
  static float speed_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820086ECu); }
  static void set_speed_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820086ECu, v); }
  static constexpr uint32_t speed_scale_address() { return 0x820086ECu; }
  // 1.0 (copied as a word by RaceLights::Update and UploadDirectional)
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
};

}  // namespace schema
