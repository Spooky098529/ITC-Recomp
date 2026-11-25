// The views of docs/functions/second-set.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/second-set.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: SceneCarModel
#pragma pack(push, 1)
struct SceneCarModelData {
  uint32_t vtable = 0;  // +0
  uint32_t list = 0;  // +4
  uint32_t next = 0;  // +8
  uint32_t previous = 0;  // +12
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint32_t car_model = 0;  // +24
  uint8_t pad_28[4] = {};
  uint8_t car_record[3696] = {};  // +32, guest order
  uint32_t scene = 0;  // +3728
  uint8_t pad_3732[12] = {};
};
#pragma pack(pop)
static_assert(sizeof(SceneCarModelData) == 3744);
inline constexpr memory::LayoutField kSceneCarModelFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {3728, 4}};
inline constexpr memory::Layout kSceneCarModelLayout = {kSceneCarModelFields, 8};

struct SceneCarModel : View {
  using View::View;
  static constexpr uint32_t kSize = 3744;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82005130;
  SceneCarModelData* data() const { return static_cast<SceneCarModelData*>(memory::HandleObject(address)); }
  static SceneCarModel New() {
    return SceneCarModel(memory::MakeHandle(new SceneCarModelData(), sizeof(SceneCarModelData), "SceneCarModel", &kSceneCarModelLayout));
  }
  template <uint32_t Stride = kSize>
  static SceneCarModel NewArray(uint32_t count) {
    static_assert(sizeof(SceneCarModelData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SceneCarModelData*>(::operator new(sizeof(SceneCarModelData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SceneCarModelData();
    return SceneCarModel(memory::MakeHandle(p, sizeof(SceneCarModelData) * count, "SceneCarModel", &kSceneCarModelLayout, sizeof(SceneCarModelData)));
  }
  SceneCarModel at(uint32_t i) const { return SceneCarModel(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SceneCarModelData* direct() const { return static_cast<SceneCarModelData*>(memory::HandleOwn(address, &kSceneCarModelLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSceneCarModelLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSceneCarModelLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82005130; the pool objects' base 0x82004F90 before ~PoolNode
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the PoolNode's list
  uint32_t list_address() const { return address + 4; }
  uint32_t list_bits() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_list_bits(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t list() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_list(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the PoolNode's links
  uint32_t next_address() const { return address + 8; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t next() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_next(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t previous_address() const { return address + 12; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t previous() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_previous(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the pool object's: 0 free, 3 in use
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // its index in the pool (-1 from the constructor)
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // the first set's car model (car-models.toml's CarModel); handed back by ScenePools::ReleaseA with its +20
  uint32_t car_model_address() const { return address + 24; }
  uint32_t car_model_bits() const { if (auto* d = direct()) return d->car_model; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_car_model_bits(uint32_t v) const { if (auto* d = direct()) { d->car_model = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  template <typename T = SecondSetFirstSetObject>
  T car_model() const { return T(car_model_bits()); }
  void set_car_model(const View& w) const { set_car_model_bits(w.address); }
  void set_car_model(uint32_t v) const { set_car_model_bits(v); }
  // the 3,696-byte car record (scene-models.toml's CarRecord)
  uint32_t car_record_address() const { return address + 32; }
  uint8_t* car_record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 32 : guest::Pointer(address + 32); }
  // the car scene (0 when reset)
  uint32_t scene_address() const { return address + 3728; }
  uint32_t scene_bits() const { if (auto* d = direct()) return d->scene; return uint32_t((memory::IsHandle(address + 3728) ? uint32_t(memory::OwnerLoad(address + 3728, 4)) : guest::LoadU32(address + 3728))); }
  void set_scene_bits(uint32_t v) const { if (auto* d = direct()) { d->scene = v; return; } if (memory::IsHandle(address + 3728)) memory::OwnerStore(address + 3728, 4, v); else guest::StoreU32(address + 3728, v); }
  template <typename T = CarScene>
  T scene() const { return T(scene_bits()); }
  void set_scene(const View& w) const { set_scene_bits(w.address); }
  void set_scene(uint32_t v) const { set_scene_bits(v); }
};

// embedded; measure: SceneCarModel.+24, SceneModel.+5084
struct SecondSetFirstSetObject : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // its index in its pool (ScenePools::ReleaseA's r5, ReleaseB's)
  uint32_t index() const { return guest::LoadU32(address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t index_address() const { return address + 20; }
};

// allocated; measure: SceneCockpit
#pragma pack(push, 1)
struct SceneCockpitData {
  uint32_t vtable = 0;  // +0
  uint32_t list = 0;  // +4
  uint32_t next = 0;  // +8
  uint32_t previous = 0;  // +12
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint32_t car_model = 0;  // +24
  uint32_t interior = 0;  // +28
  uint32_t body = 0;  // +32
  uint8_t record[144] = {};  // +36, guest order
  uint8_t pad_180[4092] = {};
  uint32_t record_address = 0;  // +4272
  uint8_t pad_4276[12] = {};
  uint32_t part_model_0 = 0;  // +4288
  uint32_t part_model_1 = 0;  // +4292
  uint32_t part_model_2 = 0;  // +4296
  uint32_t part_model_3 = 0;  // +4300
  uint32_t part_model_4 = 0;  // +4304
  uint32_t part_model_5 = 0;  // +4308
  uint32_t finished = 0;  // +4312
  uint32_t scene = 0;  // +4316
};
#pragma pack(pop)
static_assert(sizeof(SceneCockpitData) == 4320);
inline constexpr memory::LayoutField kSceneCockpitFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {4272, 4}, {4288, 4}, {4292, 4}, {4296, 4}, {4300, 4}, {4304, 4}, {4308, 4}, {4312, 4}, {4316, 4}};
inline constexpr memory::Layout kSceneCockpitLayout = {kSceneCockpitFields, 18};

struct SceneCockpit : View {
  using View::View;
  static constexpr uint32_t kSize = 4320;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x8200527C;
  SceneCockpitData* data() const { return static_cast<SceneCockpitData*>(memory::HandleObject(address)); }
  static SceneCockpit New() {
    return SceneCockpit(memory::MakeHandle(new SceneCockpitData(), sizeof(SceneCockpitData), "SceneCockpit", &kSceneCockpitLayout));
  }
  template <uint32_t Stride = kSize>
  static SceneCockpit NewArray(uint32_t count) {
    static_assert(sizeof(SceneCockpitData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SceneCockpitData*>(::operator new(sizeof(SceneCockpitData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SceneCockpitData();
    return SceneCockpit(memory::MakeHandle(p, sizeof(SceneCockpitData) * count, "SceneCockpit", &kSceneCockpitLayout, sizeof(SceneCockpitData)));
  }
  SceneCockpit at(uint32_t i) const { return SceneCockpit(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SceneCockpitData* direct() const { return static_cast<SceneCockpitData*>(memory::HandleOwn(address, &kSceneCockpitLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSceneCockpitLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSceneCockpitLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x8200527C; the pool objects' base 0x82004F90 before ~PoolNode. Measure fault: CockpitPoints::Find's r4 (the part models at +4288) is filed here, its +0 to +20 the six part models
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the PoolNode's list (measure fault: part model 1)
  uint32_t list_address() const { return address + 4; }
  uint32_t list_bits() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_list_bits(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t list() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_list(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the PoolNode's links (measure fault: part models 2 and 3)
  uint32_t next_address() const { return address + 8; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t next() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_next(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t previous_address() const { return address + 12; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t previous() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_previous(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the pool object's: 3 in use (measure fault: part model 4)
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // its index in the pool (measure fault: part model 5)
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // the second set's car model (SceneCockpit::SetUp; 0 when reset)
  uint32_t car_model_address() const { return address + 24; }
  uint32_t car_model_bits() const { if (auto* d = direct()) return d->car_model; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_car_model_bits(uint32_t v) const { if (auto* d = direct()) { d->car_model = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t car_model() const { if (auto* d = direct()) return d->car_model; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_car_model(uint32_t v) const { if (auto* d = direct()) { d->car_model = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  // its own interior (pool 3 of the second set)
  uint32_t interior_address() const { return address + 28; }
  uint32_t interior_bits() const { if (auto* d = direct()) return d->interior; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_interior_bits(uint32_t v) const { if (auto* d = direct()) { d->interior = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  template <typename T = SceneInterior>
  T interior() const { return T(interior_bits()); }
  void set_interior(const View& w) const { set_interior_bits(w.address); }
  void set_interior(uint32_t v) const { set_interior_bits(v); }
  // the first set's cockpit body (pool 2 of [0x826C2E40]; scene-loaders.toml's Cockpit)
  uint32_t body_address() const { return address + 32; }
  uint32_t body_bits() const { if (auto* d = direct()) return d->body; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_body_bits(uint32_t v) const { if (auto* d = direct()) { d->body = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t body() const { if (auto* d = direct()) return d->body; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_body(uint32_t v) const { if (auto* d = direct()) { d->body = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // the 144-byte cockpit record (scene-models.toml's SceneModelsCockpitRecord)
  uint32_t record_address() const { return address + 36; }
  uint8_t* record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 36 : guest::Pointer(address + 36); }
  // the point block (SecondSetCockpitPoints)
  uint32_t points_address() const { return address + 192; }
  uint8_t* points() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 192 : guest::Pointer(address + 192); }
  // the record's address (+36)
  uint32_t record_address_address() const { return address + 4272; }
  uint32_t record_address_bits() const { if (auto* d = direct()) return d->record_address; return uint32_t((memory::IsHandle(address + 4272) ? uint32_t(memory::OwnerLoad(address + 4272, 4)) : guest::LoadU32(address + 4272))); }
  void set_record_address_bits(uint32_t v) const { if (auto* d = direct()) { d->record_address = v; return; } if (memory::IsHandle(address + 4272)) memory::OwnerStore(address + 4272, 4, v); else guest::StoreU32(address + 4272, v); }
  template <typename T = SceneModelsCockpitRecord>
  T record_address() const { return T(record_address_bits()); }
  void set_record_address(const View& w) const { set_record_address_bits(w.address); }
  void set_record_address(uint32_t v) const { set_record_address_bits(v); }
  // the body's six part models (its +688 + 464i; SceneCockpit::Finish)
  uint32_t part_models_address() const { return address + 4288; }
  uint8_t* part_models() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4288 : guest::Pointer(address + 4288); }
  uint32_t part_model_0_address() const { return address + 4288; }
  uint32_t part_model_0_bits() const { if (auto* d = direct()) return d->part_model_0; return uint32_t((memory::IsHandle(address + 4288) ? uint32_t(memory::OwnerLoad(address + 4288, 4)) : guest::LoadU32(address + 4288))); }
  void set_part_model_0_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_0 = v; return; } if (memory::IsHandle(address + 4288)) memory::OwnerStore(address + 4288, 4, v); else guest::StoreU32(address + 4288, v); }
  uint32_t part_model_0() const { if (auto* d = direct()) return d->part_model_0; return uint32_t((memory::IsHandle(address + 4288) ? uint32_t(memory::OwnerLoad(address + 4288, 4)) : guest::LoadU32(address + 4288))); }
  void set_part_model_0(uint32_t v) const { if (auto* d = direct()) { d->part_model_0 = v; return; } if (memory::IsHandle(address + 4288)) memory::OwnerStore(address + 4288, 4, v); else guest::StoreU32(address + 4288, v); }
  uint32_t part_model_1_address() const { return address + 4292; }
  uint32_t part_model_1_bits() const { if (auto* d = direct()) return d->part_model_1; return uint32_t((memory::IsHandle(address + 4292) ? uint32_t(memory::OwnerLoad(address + 4292, 4)) : guest::LoadU32(address + 4292))); }
  void set_part_model_1_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_1 = v; return; } if (memory::IsHandle(address + 4292)) memory::OwnerStore(address + 4292, 4, v); else guest::StoreU32(address + 4292, v); }
  uint32_t part_model_1() const { if (auto* d = direct()) return d->part_model_1; return uint32_t((memory::IsHandle(address + 4292) ? uint32_t(memory::OwnerLoad(address + 4292, 4)) : guest::LoadU32(address + 4292))); }
  void set_part_model_1(uint32_t v) const { if (auto* d = direct()) { d->part_model_1 = v; return; } if (memory::IsHandle(address + 4292)) memory::OwnerStore(address + 4292, 4, v); else guest::StoreU32(address + 4292, v); }
  uint32_t part_model_2_address() const { return address + 4296; }
  uint32_t part_model_2_bits() const { if (auto* d = direct()) return d->part_model_2; return uint32_t((memory::IsHandle(address + 4296) ? uint32_t(memory::OwnerLoad(address + 4296, 4)) : guest::LoadU32(address + 4296))); }
  void set_part_model_2_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_2 = v; return; } if (memory::IsHandle(address + 4296)) memory::OwnerStore(address + 4296, 4, v); else guest::StoreU32(address + 4296, v); }
  uint32_t part_model_2() const { if (auto* d = direct()) return d->part_model_2; return uint32_t((memory::IsHandle(address + 4296) ? uint32_t(memory::OwnerLoad(address + 4296, 4)) : guest::LoadU32(address + 4296))); }
  void set_part_model_2(uint32_t v) const { if (auto* d = direct()) { d->part_model_2 = v; return; } if (memory::IsHandle(address + 4296)) memory::OwnerStore(address + 4296, 4, v); else guest::StoreU32(address + 4296, v); }
  uint32_t part_model_3_address() const { return address + 4300; }
  uint32_t part_model_3_bits() const { if (auto* d = direct()) return d->part_model_3; return uint32_t((memory::IsHandle(address + 4300) ? uint32_t(memory::OwnerLoad(address + 4300, 4)) : guest::LoadU32(address + 4300))); }
  void set_part_model_3_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_3 = v; return; } if (memory::IsHandle(address + 4300)) memory::OwnerStore(address + 4300, 4, v); else guest::StoreU32(address + 4300, v); }
  uint32_t part_model_3() const { if (auto* d = direct()) return d->part_model_3; return uint32_t((memory::IsHandle(address + 4300) ? uint32_t(memory::OwnerLoad(address + 4300, 4)) : guest::LoadU32(address + 4300))); }
  void set_part_model_3(uint32_t v) const { if (auto* d = direct()) { d->part_model_3 = v; return; } if (memory::IsHandle(address + 4300)) memory::OwnerStore(address + 4300, 4, v); else guest::StoreU32(address + 4300, v); }
  uint32_t part_model_4_address() const { return address + 4304; }
  uint32_t part_model_4_bits() const { if (auto* d = direct()) return d->part_model_4; return uint32_t((memory::IsHandle(address + 4304) ? uint32_t(memory::OwnerLoad(address + 4304, 4)) : guest::LoadU32(address + 4304))); }
  void set_part_model_4_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_4 = v; return; } if (memory::IsHandle(address + 4304)) memory::OwnerStore(address + 4304, 4, v); else guest::StoreU32(address + 4304, v); }
  uint32_t part_model_4() const { if (auto* d = direct()) return d->part_model_4; return uint32_t((memory::IsHandle(address + 4304) ? uint32_t(memory::OwnerLoad(address + 4304, 4)) : guest::LoadU32(address + 4304))); }
  void set_part_model_4(uint32_t v) const { if (auto* d = direct()) { d->part_model_4 = v; return; } if (memory::IsHandle(address + 4304)) memory::OwnerStore(address + 4304, 4, v); else guest::StoreU32(address + 4304, v); }
  uint32_t part_model_5_address() const { return address + 4308; }
  uint32_t part_model_5_bits() const { if (auto* d = direct()) return d->part_model_5; return uint32_t((memory::IsHandle(address + 4308) ? uint32_t(memory::OwnerLoad(address + 4308, 4)) : guest::LoadU32(address + 4308))); }
  void set_part_model_5_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_5 = v; return; } if (memory::IsHandle(address + 4308)) memory::OwnerStore(address + 4308, 4, v); else guest::StoreU32(address + 4308, v); }
  uint32_t part_model_5() const { if (auto* d = direct()) return d->part_model_5; return uint32_t((memory::IsHandle(address + 4308) ? uint32_t(memory::OwnerLoad(address + 4308, 4)) : guest::LoadU32(address + 4308))); }
  void set_part_model_5(uint32_t v) const { if (auto* d = direct()) { d->part_model_5 = v; return; } if (memory::IsHandle(address + 4308)) memory::OwnerStore(address + 4308, 4, v); else guest::StoreU32(address + 4308, v); }
  // part models 0 and 1 (cleared as doublewords by Reset)
  uint32_t part_models_01_address() const { return address + 4288; }
  uint64_t part_models_01_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kSceneCockpitLayout, 4288, 8)); return uint64_t((memory::IsHandle(address + 4288) ? uint64_t(memory::OwnerLoad(address + 4288, 8)) : guest::LoadU64(address + 4288))); }
  void set_part_models_01_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSceneCockpitLayout, 4288, 8, v); return; } if (memory::IsHandle(address + 4288)) memory::OwnerStore(address + 4288, 8, v); else guest::StoreU64(address + 4288, v); }
  uint64_t part_models_01() const { return uint64_t(part_models_01_bits()); }
  void set_part_models_01(uint64_t v) const { set_part_models_01_bits(uint64_t(v)); }
  uint32_t part_models_23_address() const { return address + 4296; }
  uint64_t part_models_23_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kSceneCockpitLayout, 4296, 8)); return uint64_t((memory::IsHandle(address + 4296) ? uint64_t(memory::OwnerLoad(address + 4296, 8)) : guest::LoadU64(address + 4296))); }
  void set_part_models_23_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSceneCockpitLayout, 4296, 8, v); return; } if (memory::IsHandle(address + 4296)) memory::OwnerStore(address + 4296, 8, v); else guest::StoreU64(address + 4296, v); }
  uint64_t part_models_23() const { return uint64_t(part_models_23_bits()); }
  void set_part_models_23(uint64_t v) const { set_part_models_23_bits(uint64_t(v)); }
  uint32_t part_models_45_address() const { return address + 4304; }
  uint64_t part_models_45_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kSceneCockpitLayout, 4304, 8)); return uint64_t((memory::IsHandle(address + 4304) ? uint64_t(memory::OwnerLoad(address + 4304, 8)) : guest::LoadU64(address + 4304))); }
  void set_part_models_45_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSceneCockpitLayout, 4304, 8, v); return; } if (memory::IsHandle(address + 4304)) memory::OwnerStore(address + 4304, 8, v); else guest::StoreU64(address + 4304, v); }
  uint64_t part_models_45() const { return uint64_t(part_models_45_bits()); }
  void set_part_models_45(uint64_t v) const { set_part_models_45_bits(uint64_t(v)); }
  // SceneCockpit::Finish
  uint32_t finished_address() const { return address + 4312; }
  uint32_t finished_bits() const { if (auto* d = direct()) return d->finished; return uint32_t((memory::IsHandle(address + 4312) ? uint32_t(memory::OwnerLoad(address + 4312, 4)) : guest::LoadU32(address + 4312))); }
  void set_finished_bits(uint32_t v) const { if (auto* d = direct()) { d->finished = v; return; } if (memory::IsHandle(address + 4312)) memory::OwnerStore(address + 4312, 4, v); else guest::StoreU32(address + 4312, v); }
  uint32_t finished() const { if (auto* d = direct()) return d->finished; return uint32_t((memory::IsHandle(address + 4312) ? uint32_t(memory::OwnerLoad(address + 4312, 4)) : guest::LoadU32(address + 4312))); }
  void set_finished(uint32_t v) const { if (auto* d = direct()) { d->finished = v; return; } if (memory::IsHandle(address + 4312)) memory::OwnerStore(address + 4312, 4, v); else guest::StoreU32(address + 4312, v); }
  uint32_t scene_address() const { return address + 4316; }
  uint32_t scene_bits() const { if (auto* d = direct()) return d->scene; return uint32_t((memory::IsHandle(address + 4316) ? uint32_t(memory::OwnerLoad(address + 4316, 4)) : guest::LoadU32(address + 4316))); }
  void set_scene_bits(uint32_t v) const { if (auto* d = direct()) { d->scene = v; return; } if (memory::IsHandle(address + 4316)) memory::OwnerStore(address + 4316, 4, v); else guest::StoreU32(address + 4316, v); }
  template <typename T = CarScene>
  T scene() const { return T(scene_bits()); }
  void set_scene(const View& w) const { set_scene_bits(w.address); }
  void set_scene(uint32_t v) const { set_scene_bits(v); }
};

// embedded; measure: CockpitPoints::Draw.r3@+720, CockpitPoints::Draw.r3@+1440, CockpitPoints::Find.r3@+2160, SceneCockpit@+192
struct SecondSetCockpitPoints : View {
  using View::View;
  static constexpr uint32_t kSize = 4084;
  // the fenders' 3 records
  uint32_t fenders_address() const { return address + 0; }
  // a group's record 0: its model (the doors' window, DrawDoorWindow)
  uint32_t record0_model() const { return guest::LoadU32(address + 4); }
  void set_record0_model(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t record0_model_address() const { return address + 4; }
  uint32_t record0_shape() const { return guest::LoadU32(address + 228); }
  void set_record0_shape(uint32_t v) const { guest::StoreU32(address + 228, v); }
  uint32_t record0_shape_address() const { return address + 228; }
  // a group's record 1 (the cabin's window, DrawCabinWindow)
  uint32_t record1_model() const { return guest::LoadU32(address + 244); }
  void set_record1_model(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t record1_model_address() const { return address + 244; }
  uint32_t record1_shape() const { return guest::LoadU32(address + 468); }
  void set_record1_shape(uint32_t v) const { guest::StoreU32(address + 468, v); }
  uint32_t record1_shape_address() const { return address + 468; }
  // the cabin's 2 records
  uint32_t cabin_address() const { return address + 720; }
  // the bonnet's record
  uint32_t bonnet_address() const { return address + 1200; }
  // a group's record 5 (the headlights', FindHeadlights' scaled)
  uint32_t record5_model() const { return guest::LoadU32(address + 1204); }
  void set_record5_model(uint32_t v) const { guest::StoreU32(address + 1204, v); }
  uint32_t record5_model_address() const { return address + 1204; }
  uint32_t record5_shape() const { return guest::LoadU32(address + 1428); }
  void set_record5_shape(uint32_t v) const { guest::StoreU32(address + 1428, v); }
  uint32_t record5_shape_address() const { return address + 1428; }
  // the doors' 2 records
  uint32_t doors_address() const { return address + 1440; }
  uint32_t record6_model() const { return guest::LoadU32(address + 1444); }
  void set_record6_model(uint32_t v) const { guest::StoreU32(address + 1444, v); }
  uint32_t record6_model_address() const { return address + 1444; }
  uint32_t record6_shape() const { return guest::LoadU32(address + 1668); }
  void set_record6_shape(uint32_t v) const { guest::StoreU32(address + 1668, v); }
  uint32_t record6_shape_address() const { return address + 1668; }
  // the mirror's record
  uint32_t mirror_address() const { return address + 1920; }
  // the headlights' 8 records
  uint32_t headlights_address() const { return address + 2160; }
  // 0 (CockpitPoints::Reset; not read in detail)
  uint32_t word_4080() const { return guest::LoadU32(address + 4080); }
  void set_word_4080(uint32_t v) const { guest::StoreU32(address + 4080, v); }
  uint32_t word_4080_address() const { return address + 4080; }
};

// allocated; measure: SceneInterior, List::At.ret.+5068
#pragma pack(push, 1)
struct SceneInteriorData {
  uint32_t vtable = 0;  // +0
  uint32_t list = 0;  // +4
  uint32_t next = 0;  // +8
  uint32_t previous = 0;  // +12
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint32_t car_model = 0;  // +24
  uint32_t skeleton = 0;  // +28
  uint32_t texture_set = 0;  // +32
  uint32_t first_set_interior = 0;  // +36
  uint8_t pad_40[8] = {};
  uint32_t gauge0_present = 0;  // +48
  uint32_t gauge0_kind = 0;  // +52
  uint8_t pad_56[84] = {};
  uint32_t seat_model = 0;  // +140
  uint8_t pad_144[8] = {};
  uint32_t accessory = 0;  // +152
  uint32_t accessory_arg = 0;  // +156
  uint8_t pad_160[3156] = {};
  uint32_t meters_car_block = 0;  // +3316
  uint8_t pad_3320[5316] = {};
  uint32_t meters_turbo_provider = 0;  // +8636
  uint8_t pad_8640[1016] = {};
  uint32_t points_seat_model = 0;  // +9656
  uint8_t pad_9660[1940] = {};
  uint32_t points_seat_position = 0;  // +11600
  uint32_t points_record = 0;  // +11604
  uint32_t points_skeleton = 0;  // +11608
  uint32_t points_cockpit = 0;  // +11612
  uint32_t points_accessory_shown = 0;  // +11616
  uint8_t pad_11620[12] = {};
  uint32_t part_model_0 = 0;  // +11632
  uint32_t part_model_1 = 0;  // +11636
  uint32_t part_model_2 = 0;  // +11640
  uint32_t part_model_3 = 0;  // +11644
  uint32_t part_model_4 = 0;  // +11648
  uint32_t part_model_5 = 0;  // +11652
  uint32_t cockpit = 0;  // +11656
  uint32_t finished = 0;  // +11660
  uint32_t cockpit_texture = 0;  // +11664
  uint32_t scene = 0;  // +11668
  uint8_t pad_11672[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(SceneInteriorData) == 11680);
inline constexpr memory::LayoutField kSceneInteriorFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {48, 4}, {52, 4}, {140, 4}, {152, 4}, {156, 4}, {3316, 4}, {8636, 4}, {9656, 4}, {11600, 4}, {11604, 4}, {11608, 4}, {11612, 4}, {11616, 4}, {11632, 4}, {11636, 4}, {11640, 4}, {11644, 4}, {11648, 4}, {11652, 4}, {11656, 4}, {11660, 4}, {11664, 4}, {11668, 4}};
inline constexpr memory::Layout kSceneInteriorLayout = {kSceneInteriorFields, 33};

struct SceneInterior : View {
  using View::View;
  static constexpr uint32_t kSize = 11680;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x820062E4;
  SceneInteriorData* data() const { return static_cast<SceneInteriorData*>(memory::HandleObject(address)); }
  static SceneInterior New() {
    return SceneInterior(memory::MakeHandle(new SceneInteriorData(), sizeof(SceneInteriorData), "SceneInterior", &kSceneInteriorLayout));
  }
  template <uint32_t Stride = kSize>
  static SceneInterior NewArray(uint32_t count) {
    static_assert(sizeof(SceneInteriorData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SceneInteriorData*>(::operator new(sizeof(SceneInteriorData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SceneInteriorData();
    return SceneInterior(memory::MakeHandle(p, sizeof(SceneInteriorData) * count, "SceneInterior", &kSceneInteriorLayout, sizeof(SceneInteriorData)));
  }
  SceneInterior at(uint32_t i) const { return SceneInterior(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SceneInteriorData* direct() const { return static_cast<SceneInteriorData*>(memory::HandleOwn(address, &kSceneInteriorLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSceneInteriorLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSceneInteriorLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x820062E4; the pool objects' base 0x82004F90 before ~PoolNode. Measure fault: InteriorPoints::Find's r4 (the part models at +11632) is filed here, its +0 to +20 the six part models
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the PoolNode's list (measure fault: part model 1)
  uint32_t list_address() const { return address + 4; }
  uint32_t list_bits() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_list_bits(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t list() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_list(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the PoolNode's links (measure fault: part models 2 and 3)
  uint32_t next_address() const { return address + 8; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t next() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_next(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t previous_address() const { return address + 12; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t previous() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_previous(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the pool object's: 3 in use (measure fault: part model 4)
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // its index in the pool (measure fault: part model 5)
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // the second set's car model (SceneInterior::Link): its +24 the first set's
  uint32_t car_model_address() const { return address + 24; }
  uint32_t car_model_bits() const { if (auto* d = direct()) return d->car_model; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_car_model_bits(uint32_t v) const { if (auto* d = direct()) { d->car_model = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  template <typename T = CarModelsSceneCarModel>
  T car_model() const { return T(car_model_bits()); }
  void set_car_model(const View& w) const { set_car_model_bits(w.address); }
  void set_car_model(uint32_t v) const { set_car_model_bits(v); }
  // the first set's car model's +3760 (SceneInterior::Link; the cockpit body + 208 under a cockpit)
  uint32_t skeleton_address() const { return address + 28; }
  uint32_t skeleton_bits() const { if (auto* d = direct()) return d->skeleton; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_skeleton_bits(uint32_t v) const { if (auto* d = direct()) { d->skeleton = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t skeleton() const { if (auto* d = direct()) return d->skeleton; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_skeleton(uint32_t v) const { if (auto* d = direct()) { d->skeleton = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  // the first set's car model's +4048 (SceneInterior::Link; the cockpit body + 496 under a cockpit)
  uint32_t texture_set_address() const { return address + 32; }
  uint32_t texture_set_bits() const { if (auto* d = direct()) return d->texture_set; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_texture_set_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_set = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t texture_set() const { if (auto* d = direct()) return d->texture_set; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_texture_set(uint32_t v) const { if (auto* d = direct()) { d->texture_set = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // the first set's interior (pool 3 of [0x826C2E40])
  uint32_t first_set_interior_address() const { return address + 36; }
  uint32_t first_set_interior_bits() const { if (auto* d = direct()) return d->first_set_interior; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_first_set_interior_bits(uint32_t v) const { if (auto* d = direct()) { d->first_set_interior = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  template <typename T = Interior>
  T first_set_interior() const { return T(first_set_interior_bits()); }
  void set_first_set_interior(const View& w) const { set_first_set_interior_bits(w.address); }
  void set_first_set_interior(uint32_t v) const { set_first_set_interior_bits(v); }
  // the 120-byte interior record (car-draw.toml's InteriorRecord)
  uint32_t record_address() const { return address + 40; }
  uint8_t* record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 40 : guest::Pointer(address + 40); }
  // the record's +8 (SceneInterior::SetGauges)
  uint32_t gauge0_present_address() const { return address + 48; }
  uint32_t gauge0_present_bits() const { if (auto* d = direct()) return d->gauge0_present; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_gauge0_present_bits(uint32_t v) const { if (auto* d = direct()) { d->gauge0_present = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t gauge0_present() const { if (auto* d = direct()) return d->gauge0_present; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_gauge0_present(uint32_t v) const { if (auto* d = direct()) { d->gauge0_present = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // the record's +12
  uint32_t gauge0_kind_address() const { return address + 52; }
  uint32_t gauge0_kind_bits() const { if (auto* d = direct()) return d->gauge0_kind; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_gauge0_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->gauge0_kind = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t gauge0_kind() const { if (auto* d = direct()) return d->gauge0_kind; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_gauge0_kind(uint32_t v) const { if (auto* d = direct()) { d->gauge0_kind = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  // the record's +100 (SceneInterior::Apply)
  uint32_t seat_model_address() const { return address + 140; }
  uint32_t seat_model_bits() const { if (auto* d = direct()) return d->seat_model; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_seat_model_bits(uint32_t v) const { if (auto* d = direct()) { d->seat_model = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  uint32_t seat_model() const { if (auto* d = direct()) return d->seat_model; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_seat_model(uint32_t v) const { if (auto* d = direct()) { d->seat_model = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  // the record's +112: the accessory (3 none; SceneInterior::Draw)
  uint32_t accessory_address() const { return address + 152; }
  uint32_t accessory_bits() const { if (auto* d = direct()) return d->accessory; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_accessory_bits(uint32_t v) const { if (auto* d = direct()) { d->accessory = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  uint32_t accessory() const { if (auto* d = direct()) return d->accessory; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_accessory(uint32_t v) const { if (auto* d = direct()) { d->accessory = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  // the record's +116
  uint32_t accessory_arg_address() const { return address + 156; }
  uint32_t accessory_arg_bits() const { if (auto* d = direct()) return d->accessory_arg; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(address + 156))); }
  void set_accessory_arg_bits(uint32_t v) const { if (auto* d = direct()) { d->accessory_arg = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(address + 156, v); }
  uint32_t accessory_arg() const { if (auto* d = direct()) return d->accessory_arg; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(address + 156))); }
  void set_accessory_arg(uint32_t v) const { if (auto* d = direct()) { d->accessory_arg = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(address + 156, v); }
  // the point block (SecondSetInteriorPoints)
  uint32_t points_address() const { return address + 160; }
  uint8_t* points() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 160 : guest::Pointer(address + 160); }
  // the point block's +3156 (the meters' +4): the car's block (SceneInterior::Finish)
  uint32_t meters_car_block_address() const { return address + 3316; }
  uint32_t meters_car_block_bits() const { if (auto* d = direct()) return d->meters_car_block; return uint32_t((memory::IsHandle(address + 3316) ? uint32_t(memory::OwnerLoad(address + 3316, 4)) : guest::LoadU32(address + 3316))); }
  void set_meters_car_block_bits(uint32_t v) const { if (auto* d = direct()) { d->meters_car_block = v; return; } if (memory::IsHandle(address + 3316)) memory::OwnerStore(address + 3316, 4, v); else guest::StoreU32(address + 3316, v); }
  uint32_t meters_car_block() const { if (auto* d = direct()) return d->meters_car_block; return uint32_t((memory::IsHandle(address + 3316) ? uint32_t(memory::OwnerLoad(address + 3316, 4)) : guest::LoadU32(address + 3316))); }
  void set_meters_car_block(uint32_t v) const { if (auto* d = direct()) { d->meters_car_block = v; return; } if (memory::IsHandle(address + 3316)) memory::OwnerStore(address + 3316, 4, v); else guest::StoreU32(address + 3316, v); }
  // the point block's +8476 (the meters' +5324) = +32 (SceneInterior::Finish)
  uint32_t meters_turbo_provider_address() const { return address + 8636; }
  uint32_t meters_turbo_provider_bits() const { if (auto* d = direct()) return d->meters_turbo_provider; return uint32_t((memory::IsHandle(address + 8636) ? uint32_t(memory::OwnerLoad(address + 8636, 4)) : guest::LoadU32(address + 8636))); }
  void set_meters_turbo_provider_bits(uint32_t v) const { if (auto* d = direct()) { d->meters_turbo_provider = v; return; } if (memory::IsHandle(address + 8636)) memory::OwnerStore(address + 8636, 4, v); else guest::StoreU32(address + 8636, v); }
  uint32_t meters_turbo_provider() const { if (auto* d = direct()) return d->meters_turbo_provider; return uint32_t((memory::IsHandle(address + 8636) ? uint32_t(memory::OwnerLoad(address + 8636, 4)) : guest::LoadU32(address + 8636))); }
  void set_meters_turbo_provider(uint32_t v) const { if (auto* d = direct()) { d->meters_turbo_provider = v; return; } if (memory::IsHandle(address + 8636)) memory::OwnerStore(address + 8636, 4, v); else guest::StoreU32(address + 8636, v); }
  // the point block's +9496: the scene + 784 + 464 x the record's +100 (SceneInterior::Apply)
  uint32_t points_seat_model_address() const { return address + 9656; }
  uint32_t points_seat_model_bits() const { if (auto* d = direct()) return d->points_seat_model; return uint32_t((memory::IsHandle(address + 9656) ? uint32_t(memory::OwnerLoad(address + 9656, 4)) : guest::LoadU32(address + 9656))); }
  void set_points_seat_model_bits(uint32_t v) const { if (auto* d = direct()) { d->points_seat_model = v; return; } if (memory::IsHandle(address + 9656)) memory::OwnerStore(address + 9656, 4, v); else guest::StoreU32(address + 9656, v); }
  uint32_t points_seat_model() const { if (auto* d = direct()) return d->points_seat_model; return uint32_t((memory::IsHandle(address + 9656) ? uint32_t(memory::OwnerLoad(address + 9656, 4)) : guest::LoadU32(address + 9656))); }
  void set_points_seat_model(uint32_t v) const { if (auto* d = direct()) { d->points_seat_model = v; return; } if (memory::IsHandle(address + 9656)) memory::OwnerStore(address + 9656, 4, v); else guest::StoreU32(address + 9656, v); }
  // the point block's +11440: the first set's car model's +7168 (SceneInterior::Finish)
  uint32_t points_seat_position_address() const { return address + 11600; }
  uint32_t points_seat_position_bits() const { if (auto* d = direct()) return d->points_seat_position; return uint32_t((memory::IsHandle(address + 11600) ? uint32_t(memory::OwnerLoad(address + 11600, 4)) : guest::LoadU32(address + 11600))); }
  void set_points_seat_position_bits(uint32_t v) const { if (auto* d = direct()) { d->points_seat_position = v; return; } if (memory::IsHandle(address + 11600)) memory::OwnerStore(address + 11600, 4, v); else guest::StoreU32(address + 11600, v); }
  uint32_t points_seat_position() const { if (auto* d = direct()) return d->points_seat_position; return uint32_t((memory::IsHandle(address + 11600) ? uint32_t(memory::OwnerLoad(address + 11600, 4)) : guest::LoadU32(address + 11600))); }
  void set_points_seat_position(uint32_t v) const { if (auto* d = direct()) { d->points_seat_position = v; return; } if (memory::IsHandle(address + 11600)) memory::OwnerStore(address + 11600, 4, v); else guest::StoreU32(address + 11600, v); }
  // the point block's +11444: the record's address (+40)
  uint32_t points_record_address() const { return address + 11604; }
  uint32_t points_record_bits() const { if (auto* d = direct()) return d->points_record; return uint32_t((memory::IsHandle(address + 11604) ? uint32_t(memory::OwnerLoad(address + 11604, 4)) : guest::LoadU32(address + 11604))); }
  void set_points_record_bits(uint32_t v) const { if (auto* d = direct()) { d->points_record = v; return; } if (memory::IsHandle(address + 11604)) memory::OwnerStore(address + 11604, 4, v); else guest::StoreU32(address + 11604, v); }
  template <typename T = InteriorRecord>
  T points_record() const { return T(points_record_bits()); }
  void set_points_record(const View& w) const { set_points_record_bits(w.address); }
  void set_points_record(uint32_t v) const { set_points_record_bits(v); }
  // the point block's +11448: the first set's car model's +3760
  uint32_t points_skeleton_address() const { return address + 11608; }
  uint32_t points_skeleton_bits() const { if (auto* d = direct()) return d->points_skeleton; return uint32_t((memory::IsHandle(address + 11608) ? uint32_t(memory::OwnerLoad(address + 11608, 4)) : guest::LoadU32(address + 11608))); }
  void set_points_skeleton_bits(uint32_t v) const { if (auto* d = direct()) { d->points_skeleton = v; return; } if (memory::IsHandle(address + 11608)) memory::OwnerStore(address + 11608, 4, v); else guest::StoreU32(address + 11608, v); }
  uint32_t points_skeleton() const { if (auto* d = direct()) return d->points_skeleton; return uint32_t((memory::IsHandle(address + 11608) ? uint32_t(memory::OwnerLoad(address + 11608, 4)) : guest::LoadU32(address + 11608))); }
  void set_points_skeleton(uint32_t v) const { if (auto* d = direct()) { d->points_skeleton = v; return; } if (memory::IsHandle(address + 11608)) memory::OwnerStore(address + 11608, 4, v); else guest::StoreU32(address + 11608, v); }
  // the point block's +11452: 1 under a cockpit (sub_820E3658)
  uint32_t points_cockpit_address() const { return address + 11612; }
  uint32_t points_cockpit_bits() const { if (auto* d = direct()) return d->points_cockpit; return uint32_t((memory::IsHandle(address + 11612) ? uint32_t(memory::OwnerLoad(address + 11612, 4)) : guest::LoadU32(address + 11612))); }
  void set_points_cockpit_bits(uint32_t v) const { if (auto* d = direct()) { d->points_cockpit = v; return; } if (memory::IsHandle(address + 11612)) memory::OwnerStore(address + 11612, 4, v); else guest::StoreU32(address + 11612, v); }
  uint32_t points_cockpit() const { if (auto* d = direct()) return d->points_cockpit; return uint32_t((memory::IsHandle(address + 11612) ? uint32_t(memory::OwnerLoad(address + 11612, 4)) : guest::LoadU32(address + 11612))); }
  void set_points_cockpit(uint32_t v) const { if (auto* d = direct()) { d->points_cockpit = v; return; } if (memory::IsHandle(address + 11612)) memory::OwnerStore(address + 11612, 4, v); else guest::StoreU32(address + 11612, v); }
  // the point block's +11456: the accessory drawn (CarScene::SetInteriorWord; 1 from Reset)
  uint32_t points_accessory_shown_address() const { return address + 11616; }
  uint32_t points_accessory_shown_bits() const { if (auto* d = direct()) return d->points_accessory_shown; return uint32_t((memory::IsHandle(address + 11616) ? uint32_t(memory::OwnerLoad(address + 11616, 4)) : guest::LoadU32(address + 11616))); }
  void set_points_accessory_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->points_accessory_shown = v; return; } if (memory::IsHandle(address + 11616)) memory::OwnerStore(address + 11616, 4, v); else guest::StoreU32(address + 11616, v); }
  uint32_t points_accessory_shown() const { if (auto* d = direct()) return d->points_accessory_shown; return uint32_t((memory::IsHandle(address + 11616) ? uint32_t(memory::OwnerLoad(address + 11616, 4)) : guest::LoadU32(address + 11616))); }
  void set_points_accessory_shown(uint32_t v) const { if (auto* d = direct()) { d->points_accessory_shown = v; return; } if (memory::IsHandle(address + 11616)) memory::OwnerStore(address + 11616, 4, v); else guest::StoreU32(address + 11616, v); }
  // six part models (the first set's interior's +304 + 464i)
  uint32_t part_models_address() const { return address + 11632; }
  uint8_t* part_models() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 11632 : guest::Pointer(address + 11632); }
  uint32_t part_model_0_address() const { return address + 11632; }
  uint32_t part_model_0_bits() const { if (auto* d = direct()) return d->part_model_0; return uint32_t((memory::IsHandle(address + 11632) ? uint32_t(memory::OwnerLoad(address + 11632, 4)) : guest::LoadU32(address + 11632))); }
  void set_part_model_0_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_0 = v; return; } if (memory::IsHandle(address + 11632)) memory::OwnerStore(address + 11632, 4, v); else guest::StoreU32(address + 11632, v); }
  uint32_t part_model_0() const { if (auto* d = direct()) return d->part_model_0; return uint32_t((memory::IsHandle(address + 11632) ? uint32_t(memory::OwnerLoad(address + 11632, 4)) : guest::LoadU32(address + 11632))); }
  void set_part_model_0(uint32_t v) const { if (auto* d = direct()) { d->part_model_0 = v; return; } if (memory::IsHandle(address + 11632)) memory::OwnerStore(address + 11632, 4, v); else guest::StoreU32(address + 11632, v); }
  uint32_t part_model_1_address() const { return address + 11636; }
  uint32_t part_model_1_bits() const { if (auto* d = direct()) return d->part_model_1; return uint32_t((memory::IsHandle(address + 11636) ? uint32_t(memory::OwnerLoad(address + 11636, 4)) : guest::LoadU32(address + 11636))); }
  void set_part_model_1_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_1 = v; return; } if (memory::IsHandle(address + 11636)) memory::OwnerStore(address + 11636, 4, v); else guest::StoreU32(address + 11636, v); }
  uint32_t part_model_1() const { if (auto* d = direct()) return d->part_model_1; return uint32_t((memory::IsHandle(address + 11636) ? uint32_t(memory::OwnerLoad(address + 11636, 4)) : guest::LoadU32(address + 11636))); }
  void set_part_model_1(uint32_t v) const { if (auto* d = direct()) { d->part_model_1 = v; return; } if (memory::IsHandle(address + 11636)) memory::OwnerStore(address + 11636, 4, v); else guest::StoreU32(address + 11636, v); }
  uint32_t part_model_2_address() const { return address + 11640; }
  uint32_t part_model_2_bits() const { if (auto* d = direct()) return d->part_model_2; return uint32_t((memory::IsHandle(address + 11640) ? uint32_t(memory::OwnerLoad(address + 11640, 4)) : guest::LoadU32(address + 11640))); }
  void set_part_model_2_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_2 = v; return; } if (memory::IsHandle(address + 11640)) memory::OwnerStore(address + 11640, 4, v); else guest::StoreU32(address + 11640, v); }
  uint32_t part_model_2() const { if (auto* d = direct()) return d->part_model_2; return uint32_t((memory::IsHandle(address + 11640) ? uint32_t(memory::OwnerLoad(address + 11640, 4)) : guest::LoadU32(address + 11640))); }
  void set_part_model_2(uint32_t v) const { if (auto* d = direct()) { d->part_model_2 = v; return; } if (memory::IsHandle(address + 11640)) memory::OwnerStore(address + 11640, 4, v); else guest::StoreU32(address + 11640, v); }
  uint32_t part_model_3_address() const { return address + 11644; }
  uint32_t part_model_3_bits() const { if (auto* d = direct()) return d->part_model_3; return uint32_t((memory::IsHandle(address + 11644) ? uint32_t(memory::OwnerLoad(address + 11644, 4)) : guest::LoadU32(address + 11644))); }
  void set_part_model_3_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_3 = v; return; } if (memory::IsHandle(address + 11644)) memory::OwnerStore(address + 11644, 4, v); else guest::StoreU32(address + 11644, v); }
  uint32_t part_model_3() const { if (auto* d = direct()) return d->part_model_3; return uint32_t((memory::IsHandle(address + 11644) ? uint32_t(memory::OwnerLoad(address + 11644, 4)) : guest::LoadU32(address + 11644))); }
  void set_part_model_3(uint32_t v) const { if (auto* d = direct()) { d->part_model_3 = v; return; } if (memory::IsHandle(address + 11644)) memory::OwnerStore(address + 11644, 4, v); else guest::StoreU32(address + 11644, v); }
  uint32_t part_model_4_address() const { return address + 11648; }
  uint32_t part_model_4_bits() const { if (auto* d = direct()) return d->part_model_4; return uint32_t((memory::IsHandle(address + 11648) ? uint32_t(memory::OwnerLoad(address + 11648, 4)) : guest::LoadU32(address + 11648))); }
  void set_part_model_4_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_4 = v; return; } if (memory::IsHandle(address + 11648)) memory::OwnerStore(address + 11648, 4, v); else guest::StoreU32(address + 11648, v); }
  uint32_t part_model_4() const { if (auto* d = direct()) return d->part_model_4; return uint32_t((memory::IsHandle(address + 11648) ? uint32_t(memory::OwnerLoad(address + 11648, 4)) : guest::LoadU32(address + 11648))); }
  void set_part_model_4(uint32_t v) const { if (auto* d = direct()) { d->part_model_4 = v; return; } if (memory::IsHandle(address + 11648)) memory::OwnerStore(address + 11648, 4, v); else guest::StoreU32(address + 11648, v); }
  uint32_t part_model_5_address() const { return address + 11652; }
  uint32_t part_model_5_bits() const { if (auto* d = direct()) return d->part_model_5; return uint32_t((memory::IsHandle(address + 11652) ? uint32_t(memory::OwnerLoad(address + 11652, 4)) : guest::LoadU32(address + 11652))); }
  void set_part_model_5_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_5 = v; return; } if (memory::IsHandle(address + 11652)) memory::OwnerStore(address + 11652, 4, v); else guest::StoreU32(address + 11652, v); }
  uint32_t part_model_5() const { if (auto* d = direct()) return d->part_model_5; return uint32_t((memory::IsHandle(address + 11652) ? uint32_t(memory::OwnerLoad(address + 11652, 4)) : guest::LoadU32(address + 11652))); }
  void set_part_model_5(uint32_t v) const { if (auto* d = direct()) { d->part_model_5 = v; return; } if (memory::IsHandle(address + 11652)) memory::OwnerStore(address + 11652, 4, v); else guest::StoreU32(address + 11652, v); }
  // 1 under a cockpit (the cockpit set c)
  uint32_t cockpit_address() const { return address + 11656; }
  uint32_t cockpit_bits() const { if (auto* d = direct()) return d->cockpit; return uint32_t((memory::IsHandle(address + 11656) ? uint32_t(memory::OwnerLoad(address + 11656, 4)) : guest::LoadU32(address + 11656))); }
  void set_cockpit_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit = v; return; } if (memory::IsHandle(address + 11656)) memory::OwnerStore(address + 11656, 4, v); else guest::StoreU32(address + 11656, v); }
  uint32_t cockpit() const { if (auto* d = direct()) return d->cockpit; return uint32_t((memory::IsHandle(address + 11656) ? uint32_t(memory::OwnerLoad(address + 11656, 4)) : guest::LoadU32(address + 11656))); }
  void set_cockpit(uint32_t v) const { if (auto* d = direct()) { d->cockpit = v; return; } if (memory::IsHandle(address + 11656)) memory::OwnerStore(address + 11656, 4, v); else guest::StoreU32(address + 11656, v); }
  // SceneInterior::Finish
  uint32_t finished_address() const { return address + 11660; }
  uint32_t finished_bits() const { if (auto* d = direct()) return d->finished; return uint32_t((memory::IsHandle(address + 11660) ? uint32_t(memory::OwnerLoad(address + 11660, 4)) : guest::LoadU32(address + 11660))); }
  void set_finished_bits(uint32_t v) const { if (auto* d = direct()) { d->finished = v; return; } if (memory::IsHandle(address + 11660)) memory::OwnerStore(address + 11660, 4, v); else guest::StoreU32(address + 11660, v); }
  uint32_t finished() const { if (auto* d = direct()) return d->finished; return uint32_t((memory::IsHandle(address + 11660) ? uint32_t(memory::OwnerLoad(address + 11660, 4)) : guest::LoadU32(address + 11660))); }
  void set_finished(uint32_t v) const { if (auto* d = direct()) { d->finished = v; return; } if (memory::IsHandle(address + 11660)) memory::OwnerStore(address + 11660, 4, v); else guest::StoreU32(address + 11660, v); }
  // the cockpit body's texture (SceneCockpit::Finish)
  uint32_t cockpit_texture_address() const { return address + 11664; }
  uint32_t cockpit_texture_bits() const { if (auto* d = direct()) return d->cockpit_texture; return uint32_t((memory::IsHandle(address + 11664) ? uint32_t(memory::OwnerLoad(address + 11664, 4)) : guest::LoadU32(address + 11664))); }
  void set_cockpit_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit_texture = v; return; } if (memory::IsHandle(address + 11664)) memory::OwnerStore(address + 11664, 4, v); else guest::StoreU32(address + 11664, v); }
  uint32_t cockpit_texture() const { if (auto* d = direct()) return d->cockpit_texture; return uint32_t((memory::IsHandle(address + 11664) ? uint32_t(memory::OwnerLoad(address + 11664, 4)) : guest::LoadU32(address + 11664))); }
  void set_cockpit_texture(uint32_t v) const { if (auto* d = direct()) { d->cockpit_texture = v; return; } if (memory::IsHandle(address + 11664)) memory::OwnerStore(address + 11664, 4, v); else guest::StoreU32(address + 11664, v); }
  uint32_t scene_address() const { return address + 11668; }
  uint32_t scene_bits() const { if (auto* d = direct()) return d->scene; return uint32_t((memory::IsHandle(address + 11668) ? uint32_t(memory::OwnerLoad(address + 11668, 4)) : guest::LoadU32(address + 11668))); }
  void set_scene_bits(uint32_t v) const { if (auto* d = direct()) { d->scene = v; return; } if (memory::IsHandle(address + 11668)) memory::OwnerStore(address + 11668, 4, v); else guest::StoreU32(address + 11668, v); }
  template <typename T = CarScene>
  T scene() const { return T(scene_bits()); }
  void set_scene(const View& w) const { set_scene_bits(w.address); }
  void set_scene(uint32_t v) const { set_scene_bits(v); }
};

// embedded; measure: SceneInterior@+160, SceneInterior@+160@+9488, InteriorPoints::ResetMeters.r3, InteriorPoints::ResetDials.r3, SceneInterior@+160@+10464
struct SecondSetInteriorPoints : View {
  using View::View;
  static constexpr uint32_t kSize = 11460;
  // the body's record
  uint32_t body_address() const { return address + 0; }
  // a group's: the interior record (0 from the resets)
  template <typename T = InteriorRecord>
  T group_record() const { return T(guest::LoadU32(address + 0)); }
  void set_group_record(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_group_record(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t group_record_address() const { return address + 0; }
  // a group's: the car's block (-1 from the resets; FindDials' IsBlockSetA, B)
  uint32_t group_word_4() const { return guest::LoadU32(address + 4); }
  void set_group_word_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t group_word_4_address() const { return address + 4; }
  // the dials' digits' texture provider (0 from ResetDials)
  uint32_t group_word_8() const { return guest::LoadU32(address + 8); }
  void set_group_word_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t group_word_8_address() const { return address + 8; }
  // a group's records (+16 + 240k): the dials' 12, the meters' 22, the others' 4
  uint32_t group_records_address() const { return address + 16; }
  // the dials' group (car-draw.toml's InteriorDials)
  uint32_t dials_address() const { return address + 240; }
  template <typename T = InteriorRecord>
  T dials_record() const { return T(guest::LoadU32(address + 240)); }
  void set_dials_record(const View& v) const { guest::StoreU32(address + 240, v.address); }
  void set_dials_record(uint32_t v) const { guest::StoreU32(address + 240, v); }
  uint32_t dials_record_address() const { return address + 240; }
  // a group's record 8 (+16 + 240k): the dials' digits
  uint32_t record8_model() const { return guest::LoadU32(address + 2180); }
  void set_record8_model(uint32_t v) const { guest::StoreU32(address + 2180, v); }
  uint32_t record8_model_address() const { return address + 2180; }
  uint32_t record8_shape() const { return guest::LoadU32(address + 2404); }
  void set_record8_shape(uint32_t v) const { guest::StoreU32(address + 2404, v); }
  uint32_t record8_shape_address() const { return address + 2404; }
  uint32_t record9_model() const { return guest::LoadU32(address + 2420); }
  void set_record9_model(uint32_t v) const { guest::StoreU32(address + 2420, v); }
  uint32_t record9_model_address() const { return address + 2420; }
  uint32_t record9_shape() const { return guest::LoadU32(address + 2644); }
  void set_record9_shape(uint32_t v) const { guest::StoreU32(address + 2644, v); }
  uint32_t record9_shape_address() const { return address + 2644; }
  uint32_t record10_model() const { return guest::LoadU32(address + 2660); }
  void set_record10_model(uint32_t v) const { guest::StoreU32(address + 2660, v); }
  uint32_t record10_model_address() const { return address + 2660; }
  uint32_t record10_shape() const { return guest::LoadU32(address + 2884); }
  void set_record10_shape(uint32_t v) const { guest::StoreU32(address + 2884, v); }
  uint32_t record10_shape_address() const { return address + 2884; }
  // the meters' group (load-poll.toml's InteriorMeters)
  uint32_t meters_address() const { return address + 3152; }
  template <typename T = InteriorRecord>
  T meters_record() const { return T(guest::LoadU32(address + 3152)); }
  void set_meters_record(const View& v) const { guest::StoreU32(address + 3152, v.address); }
  void set_meters_record(uint32_t v) const { guest::StoreU32(address + 3152, v); }
  uint32_t meters_record_address() const { return address + 3152; }
  // the meters' group: seven gauge texture providers (0 from ResetMeters)
  uint32_t gauge_providers_address() const { return address + 5296; }
  uint32_t gauge0_provider() const { return guest::LoadU32(address + 5296); }
  void set_gauge0_provider(uint32_t v) const { guest::StoreU32(address + 5296, v); }
  uint32_t gauge0_provider_address() const { return address + 5296; }
  // the meters' group: gauge 1's in mode 5
  uint32_t turbo_provider() const { return guest::LoadU32(address + 5324); }
  void set_turbo_provider(uint32_t v) const { guest::StoreU32(address + 5324, v); }
  uint32_t turbo_provider_address() const { return address + 5324; }
  // the steering wheel's group
  uint32_t steering_address() const { return address + 8512; }
  template <typename T = InteriorRecord>
  T steering_record() const { return T(guest::LoadU32(address + 8512)); }
  void set_steering_record(const View& v) const { guest::StoreU32(address + 8512, v.address); }
  void set_steering_record(uint32_t v) const { guest::StoreU32(address + 8512, v); }
  uint32_t steering_record_address() const { return address + 8512; }
  // the seats' group
  uint32_t seats_address() const { return address + 9488; }
  template <typename T = InteriorRecord>
  T seats_record() const { return T(guest::LoadU32(address + 9488)); }
  void set_seats_record(const View& v) const { guest::StoreU32(address + 9488, v.address); }
  void set_seats_record(uint32_t v) const { guest::StoreU32(address + 9488, v); }
  uint32_t seats_record_address() const { return address + 9488; }
  // the seats' +4: +11440
  uint32_t seat_position() const { return guest::LoadU32(address + 9492); }
  void set_seat_position(uint32_t v) const { guest::StoreU32(address + 9492, v); }
  uint32_t seat_position_address() const { return address + 9492; }
  // the seats' +8: the scene + 784 + 464 x the record's +100, when that's set
  uint32_t seat_model() const { return guest::LoadU32(address + 9496); }
  void set_seat_model(uint32_t v) const { guest::StoreU32(address + 9496, v); }
  uint32_t seat_model_address() const { return address + 9496; }
  // the roll cage's group
  uint32_t roll_cage_address() const { return address + 10464; }
  template <typename T = InteriorRecord>
  T roll_cage_record() const { return T(guest::LoadU32(address + 10464)); }
  void set_roll_cage_record(const View& v) const { guest::StoreU32(address + 10464, v.address); }
  void set_roll_cage_record(uint32_t v) const { guest::StoreU32(address + 10464, v); }
  uint32_t roll_cage_record_address() const { return address + 10464; }
  // the first set's car model's +7168 (SceneInterior::Finish; 0 from Reset)
  uint32_t seat_position_source() const { return guest::LoadU32(address + 11440); }
  void set_seat_position_source(uint32_t v) const { guest::StoreU32(address + 11440, v); }
  uint32_t seat_position_source_address() const { return address + 11440; }
  // the interior's record (its +40)
  template <typename T = InteriorRecord>
  T record() const { return T(guest::LoadU32(address + 11444)); }
  void set_record(const View& v) const { guest::StoreU32(address + 11444, v.address); }
  void set_record(uint32_t v) const { guest::StoreU32(address + 11444, v); }
  uint32_t record_address() const { return address + 11444; }
  // the first set's car model's +3760
  uint32_t skeleton() const { return guest::LoadU32(address + 11448); }
  void set_skeleton(uint32_t v) const { guest::StoreU32(address + 11448, v); }
  uint32_t skeleton_address() const { return address + 11448; }
  // 1 under a cockpit (sub_820E3658; DrawRace)
  uint32_t cockpit() const { return guest::LoadU32(address + 11452); }
  void set_cockpit(uint32_t v) const { guest::StoreU32(address + 11452, v); }
  uint32_t cockpit_address() const { return address + 11452; }
  // 1 from Reset (CarScene::SetInteriorWord)
  uint32_t accessory_shown() const { return guest::LoadU32(address + 11456); }
  void set_accessory_shown(uint32_t v) const { guest::StoreU32(address + 11456, v); }
  uint32_t accessory_shown_address() const { return address + 11456; }
};

// allocated; measure: SceneWheels
#pragma pack(push, 1)
struct SceneWheelsData {
  uint32_t vtable = 0;  // +0
  uint32_t list = 0;  // +4
  uint32_t next = 0;  // +8
  uint32_t previous = 0;  // +12
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint32_t car_model = 0;  // +24
  uint32_t first_set_wheels = 0;  // +28
  uint32_t kind = 0;  // +32
  uint8_t pad_36[4] = {};
  uint32_t morph_each_draw = 0;  // +40
  uint32_t front_tire_grade = 0;  // +44
  uint32_t front_wheel = 0;  // +48
  uint8_t pad_52[8] = {};
  uint32_t rear_tire_grade = 0;  // +60
  uint32_t rear_wheel = 0;  // +64
  uint8_t pad_68[8] = {};
  uint32_t front_size_index = 0;  // +76
  uint32_t front_size_a = 0;  // +80
  uint32_t front_size_b = 0;  // +84
  uint32_t rear_size_index = 0;  // +88
  uint32_t rear_size_a = 0;  // +92
  uint32_t rear_size_b = 0;  // +96
  uint32_t brake_kind = 0;  // +100
  uint32_t unknown_104 = 0;  // +104
  uint32_t unknown_108 = 0;  // +108
  uint32_t table_112 = 0;  // +112
  uint32_t table_116 = 0;  // +116
  uint32_t caliper_texture = 0;  // +120
  uint32_t table_124 = 0;  // +124
  uint32_t table_128 = 0;  // +128
  float front_brake_turn = 0;  // +132
  float rear_brake_turn = 0;  // +136
  uint32_t front_brake_size = 0;  // +140
  uint32_t rear_brake_size = 0;  // +144
  uint32_t front_tire = 0;  // +148
  uint32_t rear_tire = 0;  // +152
  uint8_t pad_156[36] = {};
  uint32_t points_car_model = 0;  // +192
  uint32_t points_car_record = 0;  // +196
  uint32_t points_matrix = 0;  // +200
  uint32_t points_record = 0;  // +204
  uint32_t points_tire_model = 0;  // +208
  uint32_t points_rim_model = 0;  // +212
  uint32_t points_brake_model = 0;  // +216
  uint8_t pad_220[16548] = {};
  uint32_t textures_bound = 0;  // +16768
  uint32_t points_set_up = 0;  // +16772
  uint32_t traffic_record = 0;  // +16776
  uint32_t tire_positions = 0;  // +16780
  uint32_t scene = 0;  // +16784
  uint8_t pad_16788[12] = {};
};
#pragma pack(pop)
static_assert(sizeof(SceneWheelsData) == 16800);
inline constexpr memory::LayoutField kSceneWheelsFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {40, 4}, {44, 4}, {48, 4}, {60, 4}, {64, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}, {132, 4}, {136, 4}, {140, 4}, {144, 4}, {148, 4}, {152, 4}, {192, 4}, {196, 4}, {200, 4}, {204, 4}, {208, 4}, {212, 4}, {216, 4}, {16768, 4}, {16772, 4}, {16776, 4}, {16780, 4}, {16784, 4}};
inline constexpr memory::Layout kSceneWheelsLayout = {kSceneWheelsFields, 46};

struct SceneWheels : View {
  using View::View;
  static constexpr uint32_t kSize = 16800;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82006638;
  SceneWheelsData* data() const { return static_cast<SceneWheelsData*>(memory::HandleObject(address)); }
  static SceneWheels New() {
    return SceneWheels(memory::MakeHandle(new SceneWheelsData(), sizeof(SceneWheelsData), "SceneWheels", &kSceneWheelsLayout));
  }
  template <uint32_t Stride = kSize>
  static SceneWheels NewArray(uint32_t count) {
    static_assert(sizeof(SceneWheelsData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SceneWheelsData*>(::operator new(sizeof(SceneWheelsData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SceneWheelsData();
    return SceneWheels(memory::MakeHandle(p, sizeof(SceneWheelsData) * count, "SceneWheels", &kSceneWheelsLayout, sizeof(SceneWheelsData)));
  }
  SceneWheels at(uint32_t i) const { return SceneWheels(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SceneWheelsData* direct() const { return static_cast<SceneWheelsData*>(memory::HandleOwn(address, &kSceneWheelsLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSceneWheelsLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSceneWheelsLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82006638; the pool objects' base 0x82004F90 before ~PoolNode
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the PoolNode's list (pool 5's of the second set)
  uint32_t list_address() const { return address + 4; }
  uint32_t list_bits() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_list_bits(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t list() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_list(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the PoolNode's links
  uint32_t next_address() const { return address + 8; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t next() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_next(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t previous_address() const { return address + 12; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t previous() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_previous(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the pool object's: 3 in use
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // its index in the pool
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // the second set's car model (0 for traffic)
  uint32_t car_model_address() const { return address + 24; }
  uint32_t car_model_bits() const { if (auto* d = direct()) return d->car_model; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_car_model_bits(uint32_t v) const { if (auto* d = direct()) { d->car_model = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t car_model() const { if (auto* d = direct()) return d->car_model; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_car_model(uint32_t v) const { if (auto* d = direct()) { d->car_model = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  // the first set's wheels (pool 5 of [0x826C2E40]; car-models.toml's CarWheels)
  uint32_t first_set_wheels_address() const { return address + 28; }
  uint32_t first_set_wheels_bits() const { if (auto* d = direct()) return d->first_set_wheels; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_first_set_wheels_bits(uint32_t v) const { if (auto* d = direct()) { d->first_set_wheels = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t first_set_wheels() const { if (auto* d = direct()) return d->first_set_wheels; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_first_set_wheels(uint32_t v) const { if (auto* d = direct()) { d->first_set_wheels = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  // the 156-byte wheels record (scene-models.toml's SceneModelsWheelsRecord: +n here is its +n - 32)
  uint32_t record_address() const { return address + 32; }
  uint8_t* record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 32 : guest::Pointer(address + 32); }
  // the record's +0
  uint32_t kind_address() const { return address + 32; }
  uint32_t kind_bits() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t kind() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_kind(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // the record's +8: 0 morphed once at set-up (SceneWheels::Draw)
  uint32_t morph_each_draw_address() const { return address + 40; }
  uint32_t morph_each_draw_bits() const { if (auto* d = direct()) return d->morph_each_draw; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_morph_each_draw_bits(uint32_t v) const { if (auto* d = direct()) { d->morph_each_draw = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t morph_each_draw() const { if (auto* d = direct()) return d->morph_each_draw; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_morph_each_draw(uint32_t v) const { if (auto* d = direct()) { d->morph_each_draw = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  // the record's +12 (SceneWheels::SetTireGrades)
  uint32_t front_tire_grade_address() const { return address + 44; }
  uint32_t front_tire_grade_bits() const { if (auto* d = direct()) return d->front_tire_grade; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_front_tire_grade_bits(uint32_t v) const { if (auto* d = direct()) { d->front_tire_grade = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t front_tire_grade() const { if (auto* d = direct()) return d->front_tire_grade; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_front_tire_grade(uint32_t v) const { if (auto* d = direct()) { d->front_tire_grade = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  // the record's +16
  uint32_t front_wheel_address() const { return address + 48; }
  uint32_t front_wheel_bits() const { if (auto* d = direct()) return d->front_wheel; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_front_wheel_bits(uint32_t v) const { if (auto* d = direct()) { d->front_wheel = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t front_wheel() const { if (auto* d = direct()) return d->front_wheel; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_front_wheel(uint32_t v) const { if (auto* d = direct()) { d->front_wheel = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // the record's +28
  uint32_t rear_tire_grade_address() const { return address + 60; }
  uint32_t rear_tire_grade_bits() const { if (auto* d = direct()) return d->rear_tire_grade; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_rear_tire_grade_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_tire_grade = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t rear_tire_grade() const { if (auto* d = direct()) return d->rear_tire_grade; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_rear_tire_grade(uint32_t v) const { if (auto* d = direct()) { d->rear_tire_grade = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  // the record's +32
  uint32_t rear_wheel_address() const { return address + 64; }
  uint32_t rear_wheel_bits() const { if (auto* d = direct()) return d->rear_wheel; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_rear_wheel_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_wheel = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t rear_wheel() const { if (auto* d = direct()) return d->rear_wheel; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_rear_wheel(uint32_t v) const { if (auto* d = direct()) { d->rear_wheel = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  // the record's +44 to +64: the tires' sizes (SceneWheels::SetSizes)
  uint32_t front_size_index_address() const { return address + 76; }
  uint32_t front_size_index_bits() const { if (auto* d = direct()) return d->front_size_index; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_front_size_index_bits(uint32_t v) const { if (auto* d = direct()) { d->front_size_index = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  uint32_t front_size_index() const { if (auto* d = direct()) return d->front_size_index; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_front_size_index(uint32_t v) const { if (auto* d = direct()) { d->front_size_index = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  uint32_t front_size_a_address() const { return address + 80; }
  uint32_t front_size_a_bits() const { if (auto* d = direct()) return d->front_size_a; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_front_size_a_bits(uint32_t v) const { if (auto* d = direct()) { d->front_size_a = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  uint32_t front_size_a() const { if (auto* d = direct()) return d->front_size_a; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_front_size_a(uint32_t v) const { if (auto* d = direct()) { d->front_size_a = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  uint32_t front_size_b_address() const { return address + 84; }
  uint32_t front_size_b_bits() const { if (auto* d = direct()) return d->front_size_b; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_front_size_b_bits(uint32_t v) const { if (auto* d = direct()) { d->front_size_b = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  uint32_t front_size_b() const { if (auto* d = direct()) return d->front_size_b; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_front_size_b(uint32_t v) const { if (auto* d = direct()) { d->front_size_b = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  uint32_t rear_size_index_address() const { return address + 88; }
  uint32_t rear_size_index_bits() const { if (auto* d = direct()) return d->rear_size_index; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_rear_size_index_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_size_index = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  uint32_t rear_size_index() const { if (auto* d = direct()) return d->rear_size_index; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_rear_size_index(uint32_t v) const { if (auto* d = direct()) { d->rear_size_index = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  uint32_t rear_size_a_address() const { return address + 92; }
  uint32_t rear_size_a_bits() const { if (auto* d = direct()) return d->rear_size_a; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_rear_size_a_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_size_a = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  uint32_t rear_size_a() const { if (auto* d = direct()) return d->rear_size_a; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_rear_size_a(uint32_t v) const { if (auto* d = direct()) { d->rear_size_a = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  uint32_t rear_size_b_address() const { return address + 96; }
  uint32_t rear_size_b_bits() const { if (auto* d = direct()) return d->rear_size_b; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_rear_size_b_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_size_b = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  uint32_t rear_size_b() const { if (auto* d = direct()) return d->rear_size_b; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_rear_size_b(uint32_t v) const { if (auto* d = direct()) { d->rear_size_b = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  // the record's +68
  uint32_t brake_kind_address() const { return address + 100; }
  uint32_t brake_kind_bits() const { if (auto* d = direct()) return d->brake_kind; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_brake_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->brake_kind = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  uint32_t brake_kind() const { if (auto* d = direct()) return d->brake_kind; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_brake_kind(uint32_t v) const { if (auto* d = direct()) { d->brake_kind = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  // the record's +72 (SetSizes' r4)
  uint32_t unknown_104_address() const { return address + 104; }
  uint32_t unknown_104_bits() const { if (auto* d = direct()) return d->unknown_104; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_unknown_104_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_104 = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t unknown_104() const { if (auto* d = direct()) return d->unknown_104; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_unknown_104(uint32_t v) const { if (auto* d = direct()) { d->unknown_104 = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  // the record's +76 (SetSizes' r5)
  uint32_t unknown_108_address() const { return address + 108; }
  uint32_t unknown_108_bits() const { if (auto* d = direct()) return d->unknown_108; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_unknown_108_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_108 = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t unknown_108() const { if (auto* d = direct()) return d->unknown_108; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_unknown_108(uint32_t v) const { if (auto* d = direct()) { d->unknown_108 = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  // the record's +80 to +120: from the table at 0x820044B0 (SceneWheels::SetUp)
  uint32_t table_112_address() const { return address + 112; }
  uint32_t table_112_bits() const { if (auto* d = direct()) return d->table_112; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_table_112_bits(uint32_t v) const { if (auto* d = direct()) { d->table_112 = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t table_112() const { if (auto* d = direct()) return d->table_112; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_table_112(uint32_t v) const { if (auto* d = direct()) { d->table_112 = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t table_116_address() const { return address + 116; }
  uint32_t table_116_bits() const { if (auto* d = direct()) return d->table_116; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_table_116_bits(uint32_t v) const { if (auto* d = direct()) { d->table_116 = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  uint32_t table_116() const { if (auto* d = direct()) return d->table_116; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_table_116(uint32_t v) const { if (auto* d = direct()) { d->table_116 = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  // the record's +88 (SceneWheels::SetCaliperTexture)
  uint32_t caliper_texture_address() const { return address + 120; }
  uint32_t caliper_texture_bits() const { if (auto* d = direct()) return d->caliper_texture; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_caliper_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->caliper_texture = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  uint32_t caliper_texture() const { if (auto* d = direct()) return d->caliper_texture; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_caliper_texture(uint32_t v) const { if (auto* d = direct()) { d->caliper_texture = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  uint32_t table_124_address() const { return address + 124; }
  uint32_t table_124_bits() const { if (auto* d = direct()) return d->table_124; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_table_124_bits(uint32_t v) const { if (auto* d = direct()) { d->table_124 = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  uint32_t table_124() const { if (auto* d = direct()) return d->table_124; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_table_124(uint32_t v) const { if (auto* d = direct()) { d->table_124 = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  uint32_t table_128_address() const { return address + 128; }
  uint32_t table_128_bits() const { if (auto* d = direct()) return d->table_128; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_table_128_bits(uint32_t v) const { if (auto* d = direct()) { d->table_128 = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  uint32_t table_128() const { if (auto* d = direct()) return d->table_128; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_table_128(uint32_t v) const { if (auto* d = direct()) { d->table_128 = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  // the record's +100
  uint32_t front_brake_turn_address() const { return address + 132; }
  uint32_t front_brake_turn_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_brake_turn); return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_front_brake_turn_bits(uint32_t v) const { if (auto* d = direct()) { d->front_brake_turn = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  float front_brake_turn() const { return std::bit_cast<float>(front_brake_turn_bits()); }
  void set_front_brake_turn(float v) const { set_front_brake_turn_bits(std::bit_cast<uint32_t>(v)); }
  // the record's +104
  uint32_t rear_brake_turn_address() const { return address + 136; }
  uint32_t rear_brake_turn_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->rear_brake_turn); return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_rear_brake_turn_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_brake_turn = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  float rear_brake_turn() const { return std::bit_cast<float>(rear_brake_turn_bits()); }
  void set_rear_brake_turn(float v) const { set_rear_brake_turn_bits(std::bit_cast<uint32_t>(v)); }
  // the record's +108
  uint32_t front_brake_size_address() const { return address + 140; }
  uint32_t front_brake_size_bits() const { if (auto* d = direct()) return d->front_brake_size; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_front_brake_size_bits(uint32_t v) const { if (auto* d = direct()) { d->front_brake_size = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  uint32_t front_brake_size() const { if (auto* d = direct()) return d->front_brake_size; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_front_brake_size(uint32_t v) const { if (auto* d = direct()) { d->front_brake_size = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  // the record's +112
  uint32_t rear_brake_size_address() const { return address + 144; }
  uint32_t rear_brake_size_bits() const { if (auto* d = direct()) return d->rear_brake_size; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_rear_brake_size_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_brake_size = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  uint32_t rear_brake_size() const { if (auto* d = direct()) return d->rear_brake_size; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_rear_brake_size(uint32_t v) const { if (auto* d = direct()) { d->rear_brake_size = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  // the record's +116: the entry's +40 (tire 584 for aftermarket wheels)
  uint32_t front_tire_address() const { return address + 148; }
  uint32_t front_tire_bits() const { if (auto* d = direct()) return d->front_tire; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(address + 148))); }
  void set_front_tire_bits(uint32_t v) const { if (auto* d = direct()) { d->front_tire = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(address + 148, v); }
  uint32_t front_tire() const { if (auto* d = direct()) return d->front_tire; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(address + 148))); }
  void set_front_tire(uint32_t v) const { if (auto* d = direct()) { d->front_tire = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(address + 148, v); }
  // the record's +120
  uint32_t rear_tire_address() const { return address + 152; }
  uint32_t rear_tire_bits() const { if (auto* d = direct()) return d->rear_tire; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_rear_tire_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_tire = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  uint32_t rear_tire() const { if (auto* d = direct()) return d->rear_tire; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_rear_tire(uint32_t v) const { if (auto* d = direct()) { d->rear_tire = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  // the point block (SecondSetWheelPoints)
  uint32_t points_address() const { return address + 192; }
  uint8_t* points() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 192 : guest::Pointer(address + 192); }
  // the point block's +0 (SceneWheels::SetUpPoints)
  uint32_t points_car_model_address() const { return address + 192; }
  uint32_t points_car_model_bits() const { if (auto* d = direct()) return d->points_car_model; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(address + 192))); }
  void set_points_car_model_bits(uint32_t v) const { if (auto* d = direct()) { d->points_car_model = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(address + 192, v); }
  uint32_t points_car_model() const { if (auto* d = direct()) return d->points_car_model; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(address + 192))); }
  void set_points_car_model(uint32_t v) const { if (auto* d = direct()) { d->points_car_model = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(address + 192, v); }
  // the point block's +4: the traffic record
  uint32_t points_car_record_address() const { return address + 196; }
  uint32_t points_car_record_bits() const { if (auto* d = direct()) return d->points_car_record; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(address + 196))); }
  void set_points_car_record_bits(uint32_t v) const { if (auto* d = direct()) { d->points_car_record = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(address + 196, v); }
  template <typename T = CarModelsTrafficIndex>
  T points_car_record() const { return T(points_car_record_bits()); }
  void set_points_car_record(const View& w) const { set_points_car_record_bits(w.address); }
  void set_points_car_record(uint32_t v) const { set_points_car_record_bits(v); }
  // the point block's +8: the traffic body's tire positions
  uint32_t points_matrix_address() const { return address + 200; }
  uint32_t points_matrix_bits() const { if (auto* d = direct()) return d->points_matrix; return uint32_t((memory::IsHandle(address + 200) ? uint32_t(memory::OwnerLoad(address + 200, 4)) : guest::LoadU32(address + 200))); }
  void set_points_matrix_bits(uint32_t v) const { if (auto* d = direct()) { d->points_matrix = v; return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 4, v); else guest::StoreU32(address + 200, v); }
  uint32_t points_matrix() const { if (auto* d = direct()) return d->points_matrix; return uint32_t((memory::IsHandle(address + 200) ? uint32_t(memory::OwnerLoad(address + 200, 4)) : guest::LoadU32(address + 200))); }
  void set_points_matrix(uint32_t v) const { if (auto* d = direct()) { d->points_matrix = v; return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 4, v); else guest::StoreU32(address + 200, v); }
  // the point block's +12: the record's address (+32)
  uint32_t points_record_address() const { return address + 204; }
  uint32_t points_record_bits() const { if (auto* d = direct()) return d->points_record; return uint32_t((memory::IsHandle(address + 204) ? uint32_t(memory::OwnerLoad(address + 204, 4)) : guest::LoadU32(address + 204))); }
  void set_points_record_bits(uint32_t v) const { if (auto* d = direct()) { d->points_record = v; return; } if (memory::IsHandle(address + 204)) memory::OwnerStore(address + 204, 4, v); else guest::StoreU32(address + 204, v); }
  template <typename T = SceneModelsWheelsRecord>
  T points_record() const { return T(points_record_bits()); }
  void set_points_record(const View& w) const { set_points_record_bits(w.address); }
  void set_points_record(uint32_t v) const { set_points_record_bits(v); }
  // the point block's +16: the first set's wheels' + 240
  uint32_t points_tire_model_address() const { return address + 208; }
  uint32_t points_tire_model_bits() const { if (auto* d = direct()) return d->points_tire_model; return uint32_t((memory::IsHandle(address + 208) ? uint32_t(memory::OwnerLoad(address + 208, 4)) : guest::LoadU32(address + 208))); }
  void set_points_tire_model_bits(uint32_t v) const { if (auto* d = direct()) { d->points_tire_model = v; return; } if (memory::IsHandle(address + 208)) memory::OwnerStore(address + 208, 4, v); else guest::StoreU32(address + 208, v); }
  uint32_t points_tire_model() const { if (auto* d = direct()) return d->points_tire_model; return uint32_t((memory::IsHandle(address + 208) ? uint32_t(memory::OwnerLoad(address + 208, 4)) : guest::LoadU32(address + 208))); }
  void set_points_tire_model(uint32_t v) const { if (auto* d = direct()) { d->points_tire_model = v; return; } if (memory::IsHandle(address + 208)) memory::OwnerStore(address + 208, 4, v); else guest::StoreU32(address + 208, v); }
  // the point block's +20: + 1216
  uint32_t points_rim_model_address() const { return address + 212; }
  uint32_t points_rim_model_bits() const { if (auto* d = direct()) return d->points_rim_model; return uint32_t((memory::IsHandle(address + 212) ? uint32_t(memory::OwnerLoad(address + 212, 4)) : guest::LoadU32(address + 212))); }
  void set_points_rim_model_bits(uint32_t v) const { if (auto* d = direct()) { d->points_rim_model = v; return; } if (memory::IsHandle(address + 212)) memory::OwnerStore(address + 212, 4, v); else guest::StoreU32(address + 212, v); }
  uint32_t points_rim_model() const { if (auto* d = direct()) return d->points_rim_model; return uint32_t((memory::IsHandle(address + 212) ? uint32_t(memory::OwnerLoad(address + 212, 4)) : guest::LoadU32(address + 212))); }
  void set_points_rim_model(uint32_t v) const { if (auto* d = direct()) { d->points_rim_model = v; return; } if (memory::IsHandle(address + 212)) memory::OwnerStore(address + 212, 4, v); else guest::StoreU32(address + 212, v); }
  // the point block's +24: + 2336
  uint32_t points_brake_model_address() const { return address + 216; }
  uint32_t points_brake_model_bits() const { if (auto* d = direct()) return d->points_brake_model; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(address + 216))); }
  void set_points_brake_model_bits(uint32_t v) const { if (auto* d = direct()) { d->points_brake_model = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(address + 216, v); }
  uint32_t points_brake_model() const { if (auto* d = direct()) return d->points_brake_model; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(address + 216))); }
  void set_points_brake_model(uint32_t v) const { if (auto* d = direct()) { d->points_brake_model = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(address + 216, v); }
  // SceneWheels::Finish
  uint32_t textures_bound_address() const { return address + 16768; }
  uint32_t textures_bound_bits() const { if (auto* d = direct()) return d->textures_bound; return uint32_t((memory::IsHandle(address + 16768) ? uint32_t(memory::OwnerLoad(address + 16768, 4)) : guest::LoadU32(address + 16768))); }
  void set_textures_bound_bits(uint32_t v) const { if (auto* d = direct()) { d->textures_bound = v; return; } if (memory::IsHandle(address + 16768)) memory::OwnerStore(address + 16768, 4, v); else guest::StoreU32(address + 16768, v); }
  uint32_t textures_bound() const { if (auto* d = direct()) return d->textures_bound; return uint32_t((memory::IsHandle(address + 16768) ? uint32_t(memory::OwnerLoad(address + 16768, 4)) : guest::LoadU32(address + 16768))); }
  void set_textures_bound(uint32_t v) const { if (auto* d = direct()) { d->textures_bound = v; return; } if (memory::IsHandle(address + 16768)) memory::OwnerStore(address + 16768, 4, v); else guest::StoreU32(address + 16768, v); }
  // SceneWheels::SetUpPoints
  uint32_t points_set_up_address() const { return address + 16772; }
  uint32_t points_set_up_bits() const { if (auto* d = direct()) return d->points_set_up; return uint32_t((memory::IsHandle(address + 16772) ? uint32_t(memory::OwnerLoad(address + 16772, 4)) : guest::LoadU32(address + 16772))); }
  void set_points_set_up_bits(uint32_t v) const { if (auto* d = direct()) { d->points_set_up = v; return; } if (memory::IsHandle(address + 16772)) memory::OwnerStore(address + 16772, 4, v); else guest::StoreU32(address + 16772, v); }
  uint32_t points_set_up() const { if (auto* d = direct()) return d->points_set_up; return uint32_t((memory::IsHandle(address + 16772) ? uint32_t(memory::OwnerLoad(address + 16772, 4)) : guest::LoadU32(address + 16772))); }
  void set_points_set_up(uint32_t v) const { if (auto* d = direct()) { d->points_set_up = v; return; } if (memory::IsHandle(address + 16772)) memory::OwnerStore(address + 16772, 4, v); else guest::StoreU32(address + 16772, v); }
  // the traffic record (its +0 the traffic car's index)
  uint32_t traffic_record_address() const { return address + 16776; }
  uint32_t traffic_record_bits() const { if (auto* d = direct()) return d->traffic_record; return uint32_t((memory::IsHandle(address + 16776) ? uint32_t(memory::OwnerLoad(address + 16776, 4)) : guest::LoadU32(address + 16776))); }
  void set_traffic_record_bits(uint32_t v) const { if (auto* d = direct()) { d->traffic_record = v; return; } if (memory::IsHandle(address + 16776)) memory::OwnerStore(address + 16776, 4, v); else guest::StoreU32(address + 16776, v); }
  uint32_t traffic_record() const { if (auto* d = direct()) return d->traffic_record; return uint32_t((memory::IsHandle(address + 16776) ? uint32_t(memory::OwnerLoad(address + 16776, 4)) : guest::LoadU32(address + 16776))); }
  void set_traffic_record(uint32_t v) const { if (auto* d = direct()) { d->traffic_record = v; return; } if (memory::IsHandle(address + 16776)) memory::OwnerStore(address + 16776, 4, v); else guest::StoreU32(address + 16776, v); }
  // the traffic body's tire positions (its +7072; SceneModel::PollLoad)
  uint32_t tire_positions_address() const { return address + 16780; }
  uint32_t tire_positions_bits() const { if (auto* d = direct()) return d->tire_positions; return uint32_t((memory::IsHandle(address + 16780) ? uint32_t(memory::OwnerLoad(address + 16780, 4)) : guest::LoadU32(address + 16780))); }
  void set_tire_positions_bits(uint32_t v) const { if (auto* d = direct()) { d->tire_positions = v; return; } if (memory::IsHandle(address + 16780)) memory::OwnerStore(address + 16780, 4, v); else guest::StoreU32(address + 16780, v); }
  uint32_t tire_positions() const { if (auto* d = direct()) return d->tire_positions; return uint32_t((memory::IsHandle(address + 16780) ? uint32_t(memory::OwnerLoad(address + 16780, 4)) : guest::LoadU32(address + 16780))); }
  void set_tire_positions(uint32_t v) const { if (auto* d = direct()) { d->tire_positions = v; return; } if (memory::IsHandle(address + 16780)) memory::OwnerStore(address + 16780, 4, v); else guest::StoreU32(address + 16780, v); }
  uint32_t scene_address() const { return address + 16784; }
  uint32_t scene_bits() const { if (auto* d = direct()) return d->scene; return uint32_t((memory::IsHandle(address + 16784) ? uint32_t(memory::OwnerLoad(address + 16784, 4)) : guest::LoadU32(address + 16784))); }
  void set_scene_bits(uint32_t v) const { if (auto* d = direct()) { d->scene = v; return; } if (memory::IsHandle(address + 16784)) memory::OwnerStore(address + 16784, 4, v); else guest::StoreU32(address + 16784, v); }
  template <typename T = CarScene>
  T scene() const { return T(scene_bits()); }
  void set_scene(const View& w) const { set_scene_bits(w.address); }
  void set_scene(uint32_t v) const { set_scene_bits(v); }
};

// embedded; measure: WheelPoints, WheelPoints::SetUpBrakes.r3, WheelPoints::DrawBrakesMorphed.r3, WheelPoints::SetUpRimB.r3, WheelPoints::DrawBrakes.r3, WheelPoints::SetUpTire.r3, WheelPoints::SetUpRimA.r3, WheelPoints::DrawRimAMorphed.r3, WheelPoints::DrawTireMorphed.r3, WheelPoints::DrawTire.r3, WheelPoints::DrawRimA.r3, WheelPoints::DrawRimB.r3, WheelPoints::InitTire.r3, WheelPoints::InitRimA.r3, WheelPoints::InitRimB.r3, WheelPoints::InitBrakes.r3
struct SecondSetWheelPoints : View {
  using View::View;
  static constexpr uint32_t kSize = 13648;
  // the block's: the second set's car model (SceneCarModel; its +24 the first set's)
  uint32_t car_model() const { return guest::LoadU32(address + 0); }
  void set_car_model(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t car_model_address() const { return address + 0; }
  // a group's: the car's matrix (the first set's car model's +6976, or the block's +8)
  template <typename T = SecondSetWheelMatrix>
  T matrix() const { return T(guest::LoadU32(address + 0)); }
  void set_matrix(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_matrix(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t matrix_address() const { return address + 0; }
  // measure fault: sub_820E6F28's and sub_820E7010's entries (r3 a tire + 12) filed at +0 to +20
  float entry_value() const { return guest::LoadFloat(address + 0); }
  void set_entry_value(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t entry_value_address() const { return address + 0; }
  // the block's (traffic) and a group's: the car record (its +0 the block)
  template <typename T = CarRecord>
  T car_record() const { return T(guest::LoadU32(address + 4)); }
  void set_car_record(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_car_record(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t car_record_address() const { return address + 4; }
  // a group's: the wheels record; the block's: the traffic body's tire positions (a matrix)
  template <typename T = SceneModelsWheelsRecord>
  T wheels_record() const { return T(guest::LoadU32(address + 8)); }
  void set_wheels_record(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_wheels_record(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t wheels_record_address() const { return address + 8; }
  // the block's: the wheels record; a rim's or the brakes': the front flag (1 from the resets)
  uint32_t word_12() const { return guest::LoadU32(address + 12); }
  void set_word_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t word_12_address() const { return address + 12; }
  // a tire's: twelve 24-byte entries (sub_820E6F28, sub_820E7010; cleared by ResetTire)
  uint32_t tire_entries_address() const { return address + 12; }
  // the block's: the tire model (the rear's + 464)
  uint32_t word_16() const { return guest::LoadU32(address + 16); }
  void set_word_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t word_16_address() const { return address + 16; }
  // a rim's or the brakes': the morph's weight
  float weight() const { return guest::LoadFloat(address + 16); }
  void set_weight(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t weight_address() const { return address + 16; }
  // the block's: the rim model
  uint32_t word_20() const { return guest::LoadU32(address + 20); }
  void set_word_20(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t word_20_address() const { return address + 20; }
  // a rim's: the tire's +356 copied (WheelPoints::SetUp)
  float float_20() const { return guest::LoadFloat(address + 20); }
  void set_float_20(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t float_20_address() const { return address + 20; }
  // the block's: the brake and caliper model
  uint32_t brake_model() const { return guest::LoadU32(address + 24); }
  void set_brake_model(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t brake_model_address() const { return address + 24; }
  // a rim's seven or the brakes' twelve records
  uint32_t group_records_address() const { return address + 32; }
  // the front tire's group
  uint32_t front_tire_address() const { return address + 32; }
  // also a rim's or the brakes' record 0's skeleton
  uint32_t front_tire_matrix() const { return guest::LoadU32(address + 32); }
  void set_front_tire_matrix(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t front_tire_matrix_address() const { return address + 32; }
  // also a rim's record 0's model (+32 + 4)
  uint32_t front_tire_car_record() const { return guest::LoadU32(address + 36); }
  void set_front_tire_car_record(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t front_tire_car_record_address() const { return address + 36; }
  template <typename T = SceneModelsWheelsRecord>
  T front_tire_wheels_record() const { return T(guest::LoadU32(address + 40)); }
  void set_front_tire_wheels_record(const View& v) const { guest::StoreU32(address + 40, v.address); }
  void set_front_tire_wheels_record(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t front_tire_wheels_record_address() const { return address + 40; }
  // a rim's or the brakes' record 0's shape (+32 + 228)
  uint32_t rim_record0_shape() const { return guest::LoadU32(address + 260); }
  void set_rim_record0_shape(uint32_t v) const { guest::StoreU32(address + 260, v); }
  uint32_t rim_record0_shape_address() const { return address + 260; }
  // a tire's: 1 the front (ResetTire), 0 the rear (WheelPoints::Reset)
  uint32_t tire_front() const { return guest::LoadU32(address + 300); }
  void set_tire_front(uint32_t v) const { guest::StoreU32(address + 300, v); }
  uint32_t tire_front_address() const { return address + 300; }
  // a tire's: the morph's six shape indices (-1 from ResetTire; records 1 to 5's +228)
  uint32_t tire_shapes_address() const { return address + 304; }
  uint32_t tire_shape0() const { return guest::LoadU32(address + 304); }
  void set_tire_shape0(uint32_t v) const { guest::StoreU32(address + 304, v); }
  uint32_t tire_shape0_address() const { return address + 304; }
  // a tire's: the morph's six weights (sub_820E7010's float4)
  uint32_t tire_weights_address() const { return address + 328; }
  float tire_weight0() const { return guest::LoadFloat(address + 328); }
  void set_tire_weight0(float v) const { guest::StoreFloat(address + 328, v); }
  uint32_t tire_weight0_address() const { return address + 328; }
  // a tire's: sub_820E7010's out1
  float tire_out1() const { return guest::LoadFloat(address + 352); }
  void set_tire_out1(float v) const { guest::StoreFloat(address + 352, v); }
  uint32_t tire_out1_address() const { return address + 352; }
  // a tire's: sub_820E7010's out2
  float tire_out2() const { return guest::LoadFloat(address + 356); }
  void set_tire_out2(float v) const { guest::StoreFloat(address + 356, v); }
  uint32_t tire_out2_address() const { return address + 356; }
  // a tire's six records
  uint32_t tire_records_address() const { return address + 368; }
  // a tire's record 0's model (+368 + 4)
  uint32_t tire_record0_model() const { return guest::LoadU32(address + 372); }
  void set_tire_record0_model(uint32_t v) const { guest::StoreU32(address + 372, v); }
  uint32_t tire_record0_model_address() const { return address + 372; }
  // the block's: the front tire's +352
  float front_tire_out1() const { return guest::LoadFloat(address + 384); }
  void set_front_tire_out1(float v) const { guest::StoreFloat(address + 384, v); }
  uint32_t front_tire_out1_address() const { return address + 384; }
  // the block's: the front tire's +356
  float front_tire_out2() const { return guest::LoadFloat(address + 388); }
  void set_front_tire_out2(float v) const { guest::StoreFloat(address + 388, v); }
  uint32_t front_tire_out2_address() const { return address + 388; }
  // a rim's or the brakes' record 1's shape (the morph's target)
  uint32_t rim_record1_shape() const { return guest::LoadU32(address + 500); }
  void set_rim_record1_shape(uint32_t v) const { guest::StoreU32(address + 500, v); }
  uint32_t rim_record1_shape_address() const { return address + 500; }
  // a tire's record 0's shape: the morph's base
  uint32_t tire_base_shape() const { return guest::LoadU32(address + 596); }
  void set_tire_base_shape(uint32_t v) const { guest::StoreU32(address + 596, v); }
  uint32_t tire_base_shape_address() const { return address + 596; }
  uint32_t rim_record2_shape() const { return guest::LoadU32(address + 740); }
  void set_rim_record2_shape(uint32_t v) const { guest::StoreU32(address + 740, v); }
  uint32_t rim_record2_shape_address() const { return address + 740; }
  // a tire's records 1 to 5's shapes at +836 + 240k
  uint32_t tire_record1_shape() const { return guest::LoadU32(address + 836); }
  void set_tire_record1_shape(uint32_t v) const { guest::StoreU32(address + 836, v); }
  uint32_t tire_record1_shape_address() const { return address + 836; }
  uint32_t rim_record3_shape() const { return guest::LoadU32(address + 980); }
  void set_rim_record3_shape(uint32_t v) const { guest::StoreU32(address + 980, v); }
  uint32_t rim_record3_shape_address() const { return address + 980; }
  uint32_t rim_record4_shape() const { return guest::LoadU32(address + 1220); }
  void set_rim_record4_shape(uint32_t v) const { guest::StoreU32(address + 1220, v); }
  uint32_t rim_record4_shape_address() const { return address + 1220; }
  uint32_t rim_record5_shape() const { return guest::LoadU32(address + 1460); }
  void set_rim_record5_shape(uint32_t v) const { guest::StoreU32(address + 1460, v); }
  uint32_t rim_record5_shape_address() const { return address + 1460; }
  uint32_t rim_record6_shape() const { return guest::LoadU32(address + 1700); }
  void set_rim_record6_shape(uint32_t v) const { guest::StoreU32(address + 1700, v); }
  uint32_t rim_record6_shape_address() const { return address + 1700; }
  // a rim's two rows (32 bytes of the matrix's first or second half; (0, 0, 0, 1) from the resets)
  uint32_t rim_rows_address() const { return address + 1712; }
  float rim_row0_x() const { return guest::LoadFloat(address + 1712); }
  void set_rim_row0_x(float v) const { guest::StoreFloat(address + 1712, v); }
  uint32_t rim_row0_x_address() const { return address + 1712; }
  uint64_t rim_row0_xy() const { return guest::LoadU64(address + 1712); }
  void set_rim_row0_xy(uint64_t v) const { guest::StoreU64(address + 1712, v); }
  uint32_t rim_row0_xy_address() const { return address + 1712; }
  float rim_row0_y() const { return guest::LoadFloat(address + 1716); }
  void set_rim_row0_y(float v) const { guest::StoreFloat(address + 1716, v); }
  uint32_t rim_row0_y_address() const { return address + 1716; }
  float rim_row0_z() const { return guest::LoadFloat(address + 1720); }
  void set_rim_row0_z(float v) const { guest::StoreFloat(address + 1720, v); }
  uint32_t rim_row0_z_address() const { return address + 1720; }
  uint64_t rim_row0_zw() const { return guest::LoadU64(address + 1720); }
  void set_rim_row0_zw(uint64_t v) const { guest::StoreU64(address + 1720, v); }
  uint32_t rim_row0_zw_address() const { return address + 1720; }
  float rim_row0_w() const { return guest::LoadFloat(address + 1724); }
  void set_rim_row0_w(float v) const { guest::StoreFloat(address + 1724, v); }
  uint32_t rim_row0_w_address() const { return address + 1724; }
  uint64_t rim_row1_xy() const { return guest::LoadU64(address + 1728); }
  void set_rim_row1_xy(uint64_t v) const { guest::StoreU64(address + 1728, v); }
  uint32_t rim_row1_xy_address() const { return address + 1728; }
  uint64_t rim_row1_zw() const { return guest::LoadU64(address + 1736); }
  void set_rim_row1_zw(uint64_t v) const { guest::StoreU64(address + 1736, v); }
  uint32_t rim_row1_zw_address() const { return address + 1736; }
  // a tire's two rows
  uint32_t tire_rows_address() const { return address + 1808; }
  float tire_row0_x() const { return guest::LoadFloat(address + 1808); }
  void set_tire_row0_x(float v) const { guest::StoreFloat(address + 1808, v); }
  uint32_t tire_row0_x_address() const { return address + 1808; }
  uint64_t tire_row0_xy() const { return guest::LoadU64(address + 1808); }
  void set_tire_row0_xy(uint64_t v) const { guest::StoreU64(address + 1808, v); }
  uint32_t tire_row0_xy_address() const { return address + 1808; }
  float tire_row0_y() const { return guest::LoadFloat(address + 1812); }
  void set_tire_row0_y(float v) const { guest::StoreFloat(address + 1812, v); }
  uint32_t tire_row0_y_address() const { return address + 1812; }
  float tire_row0_z() const { return guest::LoadFloat(address + 1816); }
  void set_tire_row0_z(float v) const { guest::StoreFloat(address + 1816, v); }
  uint32_t tire_row0_z_address() const { return address + 1816; }
  uint64_t tire_row0_zw() const { return guest::LoadU64(address + 1816); }
  void set_tire_row0_zw(uint64_t v) const { guest::StoreU64(address + 1816, v); }
  uint32_t tire_row0_zw_address() const { return address + 1816; }
  float tire_row0_w() const { return guest::LoadFloat(address + 1820); }
  void set_tire_row0_w(float v) const { guest::StoreFloat(address + 1820, v); }
  uint32_t tire_row0_w_address() const { return address + 1820; }
  uint64_t tire_row1_xy() const { return guest::LoadU64(address + 1824); }
  void set_tire_row1_xy(uint64_t v) const { guest::StoreU64(address + 1824, v); }
  uint32_t tire_row1_xy_address() const { return address + 1824; }
  uint64_t tire_row1_zw() const { return guest::LoadU64(address + 1832); }
  void set_tire_row1_zw(uint64_t v) const { guest::StoreU64(address + 1832, v); }
  uint32_t tire_row1_zw_address() const { return address + 1832; }
  // the rear tire's group
  uint32_t rear_tire_address() const { return address + 1872; }
  uint32_t rear_tire_matrix() const { return guest::LoadU32(address + 1872); }
  void set_rear_tire_matrix(uint32_t v) const { guest::StoreU32(address + 1872, v); }
  uint32_t rear_tire_matrix_address() const { return address + 1872; }
  uint32_t rear_tire_car_record() const { return guest::LoadU32(address + 1876); }
  void set_rear_tire_car_record(uint32_t v) const { guest::StoreU32(address + 1876, v); }
  uint32_t rear_tire_car_record_address() const { return address + 1876; }
  template <typename T = SceneModelsWheelsRecord>
  T rear_tire_wheels_record() const { return T(guest::LoadU32(address + 1880)); }
  void set_rear_tire_wheels_record(const View& v) const { guest::StoreU32(address + 1880, v.address); }
  void set_rear_tire_wheels_record(uint32_t v) const { guest::StoreU32(address + 1880, v); }
  uint32_t rear_tire_wheels_record_address() const { return address + 1880; }
  uint32_t brake_record7_shape() const { return guest::LoadU32(address + 1940); }
  void set_brake_record7_shape(uint32_t v) const { guest::StoreU32(address + 1940, v); }
  uint32_t brake_record7_shape_address() const { return address + 1940; }
  // the block's: the rear tire's +300 (0)
  uint32_t rear_tire_front() const { return guest::LoadU32(address + 2172); }
  void set_rear_tire_front(uint32_t v) const { guest::StoreU32(address + 2172, v); }
  uint32_t rear_tire_front_address() const { return address + 2172; }
  uint32_t brake_record8_shape() const { return guest::LoadU32(address + 2180); }
  void set_brake_record8_shape(uint32_t v) const { guest::StoreU32(address + 2180, v); }
  uint32_t brake_record8_shape_address() const { return address + 2180; }
  // the block's: the rear tire's +352
  float rear_tire_out1() const { return guest::LoadFloat(address + 2224); }
  void set_rear_tire_out1(float v) const { guest::StoreFloat(address + 2224, v); }
  uint32_t rear_tire_out1_address() const { return address + 2224; }
  // the block's: the rear tire's +356
  float rear_tire_out2() const { return guest::LoadFloat(address + 2228); }
  void set_rear_tire_out2(float v) const { guest::StoreFloat(address + 2228, v); }
  uint32_t rear_tire_out2_address() const { return address + 2228; }
  uint32_t brake_record9_shape() const { return guest::LoadU32(address + 2420); }
  void set_brake_record9_shape(uint32_t v) const { guest::StoreU32(address + 2420, v); }
  uint32_t brake_record9_shape_address() const { return address + 2420; }
  // the brakes' record 10 (scaled by 1.0)
  uint32_t brake_record10_model() const { return guest::LoadU32(address + 2436); }
  void set_brake_record10_model(uint32_t v) const { guest::StoreU32(address + 2436, v); }
  uint32_t brake_record10_model_address() const { return address + 2436; }
  uint32_t brake_record10_shape() const { return guest::LoadU32(address + 2660); }
  void set_brake_record10_shape(uint32_t v) const { guest::StoreU32(address + 2660, v); }
  uint32_t brake_record10_shape_address() const { return address + 2660; }
  uint32_t brake_record11_model() const { return guest::LoadU32(address + 2676); }
  void set_brake_record11_model(uint32_t v) const { guest::StoreU32(address + 2676, v); }
  uint32_t brake_record11_model_address() const { return address + 2676; }
  uint32_t brake_record11_shape() const { return guest::LoadU32(address + 2900); }
  void set_brake_record11_shape(uint32_t v) const { guest::StoreU32(address + 2900, v); }
  uint32_t brake_record11_shape_address() const { return address + 2900; }
  // the brakes' two rows
  uint32_t brake_rows_address() const { return address + 2912; }
  float brake_row0_x() const { return guest::LoadFloat(address + 2912); }
  void set_brake_row0_x(float v) const { guest::StoreFloat(address + 2912, v); }
  uint32_t brake_row0_x_address() const { return address + 2912; }
  uint64_t brake_row0_xy() const { return guest::LoadU64(address + 2912); }
  void set_brake_row0_xy(uint64_t v) const { guest::StoreU64(address + 2912, v); }
  uint32_t brake_row0_xy_address() const { return address + 2912; }
  float brake_row0_y() const { return guest::LoadFloat(address + 2916); }
  void set_brake_row0_y(float v) const { guest::StoreFloat(address + 2916, v); }
  uint32_t brake_row0_y_address() const { return address + 2916; }
  float brake_row0_z() const { return guest::LoadFloat(address + 2920); }
  void set_brake_row0_z(float v) const { guest::StoreFloat(address + 2920, v); }
  uint32_t brake_row0_z_address() const { return address + 2920; }
  uint64_t brake_row0_zw() const { return guest::LoadU64(address + 2920); }
  void set_brake_row0_zw(uint64_t v) const { guest::StoreU64(address + 2920, v); }
  uint32_t brake_row0_zw_address() const { return address + 2920; }
  float brake_row0_w() const { return guest::LoadFloat(address + 2924); }
  void set_brake_row0_w(float v) const { guest::StoreFloat(address + 2924, v); }
  uint32_t brake_row0_w_address() const { return address + 2924; }
  uint64_t brake_row1_xy() const { return guest::LoadU64(address + 2928); }
  void set_brake_row1_xy(uint64_t v) const { guest::StoreU64(address + 2928, v); }
  uint32_t brake_row1_xy_address() const { return address + 2928; }
  uint64_t brake_row1_zw() const { return guest::LoadU64(address + 2936); }
  void set_brake_row1_zw(uint64_t v) const { guest::StoreU64(address + 2936, v); }
  uint32_t brake_row1_zw_address() const { return address + 2936; }
  // set a's front rim's group
  uint32_t rim_a_front_address() const { return address + 3712; }
  uint32_t rim_a_front_matrix() const { return guest::LoadU32(address + 3712); }
  void set_rim_a_front_matrix(uint32_t v) const { guest::StoreU32(address + 3712, v); }
  uint32_t rim_a_front_matrix_address() const { return address + 3712; }
  uint32_t rim_a_front_wheels_record() const { return guest::LoadU32(address + 3720); }
  void set_rim_a_front_wheels_record(uint32_t v) const { guest::StoreU32(address + 3720, v); }
  uint32_t rim_a_front_wheels_record_address() const { return address + 3720; }
  // the front tire's +352
  float rim_a_front_weight() const { return guest::LoadFloat(address + 3728); }
  void set_rim_a_front_weight(float v) const { guest::StoreFloat(address + 3728, v); }
  uint32_t rim_a_front_weight_address() const { return address + 3728; }
  float rim_a_front_float_20() const { return guest::LoadFloat(address + 3732); }
  void set_rim_a_front_float_20(float v) const { guest::StoreFloat(address + 3732, v); }
  uint32_t rim_a_front_float_20_address() const { return address + 3732; }
  // set a's rear rim's group
  uint32_t rim_a_rear_address() const { return address + 5456; }
  // measure fault: the matrix word, filed as pointing to the wheels record
  template <typename T = SceneModelsWheelsRecord>
  T rim_a_rear_matrix() const { return T(guest::LoadU32(address + 5456)); }
  void set_rim_a_rear_matrix(const View& v) const { guest::StoreU32(address + 5456, v.address); }
  void set_rim_a_rear_matrix(uint32_t v) const { guest::StoreU32(address + 5456, v); }
  uint32_t rim_a_rear_matrix_address() const { return address + 5456; }
  template <typename T = SceneModelsWheelsRecord>
  T rim_a_rear_wheels_record() const { return T(guest::LoadU32(address + 5464)); }
  void set_rim_a_rear_wheels_record(const View& v) const { guest::StoreU32(address + 5464, v.address); }
  void set_rim_a_rear_wheels_record(uint32_t v) const { guest::StoreU32(address + 5464, v); }
  uint32_t rim_a_rear_wheels_record_address() const { return address + 5464; }
  // 0 (WheelPoints::Reset)
  uint32_t rim_a_rear_front() const { return guest::LoadU32(address + 5468); }
  void set_rim_a_rear_front(uint32_t v) const { guest::StoreU32(address + 5468, v); }
  uint32_t rim_a_rear_front_address() const { return address + 5468; }
  float rim_a_rear_weight() const { return guest::LoadFloat(address + 5472); }
  void set_rim_a_rear_weight(float v) const { guest::StoreFloat(address + 5472, v); }
  uint32_t rim_a_rear_weight_address() const { return address + 5472; }
  float rim_a_rear_float_20() const { return guest::LoadFloat(address + 5476); }
  void set_rim_a_rear_float_20(float v) const { guest::StoreFloat(address + 5476, v); }
  uint32_t rim_a_rear_float_20_address() const { return address + 5476; }
  // set b's front rim's group
  uint32_t rim_b_front_address() const { return address + 7200; }
  uint32_t rim_b_front_matrix() const { return guest::LoadU32(address + 7200); }
  void set_rim_b_front_matrix(uint32_t v) const { guest::StoreU32(address + 7200, v); }
  uint32_t rim_b_front_matrix_address() const { return address + 7200; }
  uint32_t rim_b_front_wheels_record() const { return guest::LoadU32(address + 7208); }
  void set_rim_b_front_wheels_record(uint32_t v) const { guest::StoreU32(address + 7208, v); }
  uint32_t rim_b_front_wheels_record_address() const { return address + 7208; }
  float rim_b_front_weight() const { return guest::LoadFloat(address + 7216); }
  void set_rim_b_front_weight(float v) const { guest::StoreFloat(address + 7216, v); }
  uint32_t rim_b_front_weight_address() const { return address + 7216; }
  float rim_b_front_float_20() const { return guest::LoadFloat(address + 7220); }
  void set_rim_b_front_float_20(float v) const { guest::StoreFloat(address + 7220, v); }
  uint32_t rim_b_front_float_20_address() const { return address + 7220; }
  // set b's rear rim's group
  uint32_t rim_b_rear_address() const { return address + 8944; }
  uint32_t rim_b_rear_matrix() const { return guest::LoadU32(address + 8944); }
  void set_rim_b_rear_matrix(uint32_t v) const { guest::StoreU32(address + 8944, v); }
  uint32_t rim_b_rear_matrix_address() const { return address + 8944; }
  uint32_t rim_b_rear_wheels_record() const { return guest::LoadU32(address + 8952); }
  void set_rim_b_rear_wheels_record(uint32_t v) const { guest::StoreU32(address + 8952, v); }
  uint32_t rim_b_rear_wheels_record_address() const { return address + 8952; }
  // 0 (WheelPoints::Reset)
  uint32_t rim_b_rear_front() const { return guest::LoadU32(address + 8956); }
  void set_rim_b_rear_front(uint32_t v) const { guest::StoreU32(address + 8956, v); }
  uint32_t rim_b_rear_front_address() const { return address + 8956; }
  float rim_b_rear_weight() const { return guest::LoadFloat(address + 8960); }
  void set_rim_b_rear_weight(float v) const { guest::StoreFloat(address + 8960, v); }
  uint32_t rim_b_rear_weight_address() const { return address + 8960; }
  float rim_b_rear_float_20() const { return guest::LoadFloat(address + 8964); }
  void set_rim_b_rear_float_20(float v) const { guest::StoreFloat(address + 8964, v); }
  uint32_t rim_b_rear_float_20_address() const { return address + 8964; }
  // the front brakes' group
  uint32_t brakes_front_address() const { return address + 10688; }
  uint32_t brakes_front_matrix() const { return guest::LoadU32(address + 10688); }
  void set_brakes_front_matrix(uint32_t v) const { guest::StoreU32(address + 10688, v); }
  uint32_t brakes_front_matrix_address() const { return address + 10688; }
  uint32_t brakes_front_car_record() const { return guest::LoadU32(address + 10692); }
  void set_brakes_front_car_record(uint32_t v) const { guest::StoreU32(address + 10692, v); }
  uint32_t brakes_front_car_record_address() const { return address + 10692; }
  uint32_t brakes_front_wheels_record() const { return guest::LoadU32(address + 10696); }
  void set_brakes_front_wheels_record(uint32_t v) const { guest::StoreU32(address + 10696, v); }
  uint32_t brakes_front_wheels_record_address() const { return address + 10696; }
  // the rear brakes' group (to +16576, past the block's measured 13,648)
  uint32_t brakes_rear_address() const { return address + 13632; }
  uint32_t brakes_rear_matrix() const { return guest::LoadU32(address + 13632); }
  void set_brakes_rear_matrix(uint32_t v) const { guest::StoreU32(address + 13632, v); }
  uint32_t brakes_rear_matrix_address() const { return address + 13632; }
  uint32_t brakes_rear_car_record() const { return guest::LoadU32(address + 13636); }
  void set_brakes_rear_car_record(uint32_t v) const { guest::StoreU32(address + 13636, v); }
  uint32_t brakes_rear_car_record_address() const { return address + 13636; }
  uint32_t brakes_rear_wheels_record() const { return guest::LoadU32(address + 13640); }
  void set_brakes_rear_wheels_record(uint32_t v) const { guest::StoreU32(address + 13640, v); }
  uint32_t brakes_rear_wheels_record_address() const { return address + 13640; }
  // 0 (WheelPoints::Reset)
  uint32_t brakes_rear_front() const { return guest::LoadU32(address + 13644); }
  void set_brakes_rear_front(uint32_t v) const { guest::StoreU32(address + 13644, v); }
  uint32_t brakes_rear_front_address() const { return address + 13644; }
};

// embedded; measure: SceneWheels@+192.+0, WheelPoints::SetUpTire.r3.+0, WheelPoints::SetUpRimA.r3.+0, WheelPoints::SetUpRimB.r3.+0, WheelPoints::SetUpBrakes.r3.+0, WheelPoints::DrawBrakesMorphed.r3.+0
struct SecondSetWheelMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint64_t row0_xy() const { return guest::LoadU64(address + 0); }
  void set_row0_xy(uint64_t v) const { guest::StoreU64(address + 0, v); }
  uint32_t row0_xy_address() const { return address + 0; }
  uint64_t row0_zw() const { return guest::LoadU64(address + 8); }
  void set_row0_zw(uint64_t v) const { guest::StoreU64(address + 8, v); }
  uint32_t row0_zw_address() const { return address + 8; }
  uint64_t row1_xy() const { return guest::LoadU64(address + 16); }
  void set_row1_xy(uint64_t v) const { guest::StoreU64(address + 16, v); }
  uint32_t row1_xy_address() const { return address + 16; }
  uint64_t row1_zw() const { return guest::LoadU64(address + 24); }
  void set_row1_zw(uint64_t v) const { guest::StoreU64(address + 24, v); }
  uint32_t row1_zw_address() const { return address + 24; }
  // measure fault: the block's +0's +24 (WheelPoints::SetUp)
  uint32_t car_model() const { return guest::LoadU32(address + 24); }
  void set_car_model(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
  uint64_t row2_xy() const { return guest::LoadU64(address + 32); }
  void set_row2_xy(uint64_t v) const { guest::StoreU64(address + 32, v); }
  uint32_t row2_xy_address() const { return address + 32; }
  uint64_t row2_zw() const { return guest::LoadU64(address + 40); }
  void set_row2_zw(uint64_t v) const { guest::StoreU64(address + 40, v); }
  uint32_t row2_zw_address() const { return address + 40; }
  uint64_t row3_xy() const { return guest::LoadU64(address + 48); }
  void set_row3_xy(uint64_t v) const { guest::StoreU64(address + 48, v); }
  uint32_t row3_xy_address() const { return address + 48; }
  uint64_t row3_zw() const { return guest::LoadU64(address + 56); }
  void set_row3_zw(uint64_t v) const { guest::StoreU64(address + 56, v); }
  uint32_t row3_zw_address() const { return address + 56; }
};

// allocated; measure: SceneTraffic
#pragma pack(push, 1)
struct SceneTrafficData {
  uint32_t vtable = 0;  // +0
  uint32_t list = 0;  // +4
  uint32_t next = 0;  // +8
  uint32_t previous = 0;  // +12
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint8_t pad_24[8] = {};
  uint8_t car_record[3696] = {};  // +32, guest order
  uint8_t pad_3728[1456] = {};
  uint32_t points_front_plate_texture = 0;  // +5184
  uint32_t points_rear_plate_texture = 0;  // +5188
  uint8_t pad_5192[4568] = {};
  uint32_t body = 0;  // +9760
  uint32_t skeleton = 0;  // +9764
  uint32_t part_model_0 = 0;  // +9768
  uint32_t part_model_1 = 0;  // +9772
  uint32_t part_model_2 = 0;  // +9776
  uint32_t part_model_3 = 0;  // +9780
  uint32_t part_model_4 = 0;  // +9784
  uint32_t part_model_5 = 0;  // +9788
  uint32_t finished = 0;  // +9792
  uint32_t scene = 0;  // +9796
  uint8_t pad_9800[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(SceneTrafficData) == 9808);
inline constexpr memory::LayoutField kSceneTrafficFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {5184, 4}, {5188, 4}, {9760, 4}, {9764, 4}, {9768, 4}, {9772, 4}, {9776, 4}, {9780, 4}, {9784, 4}, {9788, 4}, {9792, 4}, {9796, 4}};
inline constexpr memory::Layout kSceneTrafficLayout = {kSceneTrafficFields, 18};

struct SceneTraffic : View {
  using View::View;
  static constexpr uint32_t kSize = 9808;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82006424;
  SceneTrafficData* data() const { return static_cast<SceneTrafficData*>(memory::HandleObject(address)); }
  static SceneTraffic New() {
    return SceneTraffic(memory::MakeHandle(new SceneTrafficData(), sizeof(SceneTrafficData), "SceneTraffic", &kSceneTrafficLayout));
  }
  template <uint32_t Stride = kSize>
  static SceneTraffic NewArray(uint32_t count) {
    static_assert(sizeof(SceneTrafficData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SceneTrafficData*>(::operator new(sizeof(SceneTrafficData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SceneTrafficData();
    return SceneTraffic(memory::MakeHandle(p, sizeof(SceneTrafficData) * count, "SceneTraffic", &kSceneTrafficLayout, sizeof(SceneTrafficData)));
  }
  SceneTraffic at(uint32_t i) const { return SceneTraffic(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SceneTrafficData* direct() const { return static_cast<SceneTrafficData*>(memory::HandleOwn(address, &kSceneTrafficLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSceneTrafficLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSceneTrafficLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82006424; the pool objects' base 0x82004F90 before ~PoolNode. Measure fault: TrafficPoints::Find's r4 (the part models at +9768) is filed here, its +0 to +20 the six part models
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the PoolNode's list (measure fault: part model 1)
  uint32_t list_address() const { return address + 4; }
  uint32_t list_bits() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_list_bits(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t list() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_list(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the PoolNode's links (measure fault: part models 2 and 3)
  uint32_t next_address() const { return address + 8; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t next() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_next(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t previous_address() const { return address + 12; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t previous() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_previous(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the pool object's: 3 in use (measure fault: part model 4)
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // its index in the pool (measure fault: part model 5)
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // the 3,696-byte car record (scene-models.toml's CarRecord)
  uint32_t car_record_address() const { return address + 32; }
  uint8_t* car_record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 32 : guest::Pointer(address + 32); }
  // the point block (car-draw.toml's TrafficPoints)
  uint32_t points_address() const { return address + 3728; }
  uint8_t* points() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 3728 : guest::Pointer(address + 3728); }
  // the point block's +1456: the first set's body's +7040 (SceneTraffic::Finish)
  uint32_t points_front_plate_texture_address() const { return address + 5184; }
  uint32_t points_front_plate_texture_bits() const { if (auto* d = direct()) return d->points_front_plate_texture; return uint32_t((memory::IsHandle(address + 5184) ? uint32_t(memory::OwnerLoad(address + 5184, 4)) : guest::LoadU32(address + 5184))); }
  void set_points_front_plate_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->points_front_plate_texture = v; return; } if (memory::IsHandle(address + 5184)) memory::OwnerStore(address + 5184, 4, v); else guest::StoreU32(address + 5184, v); }
  uint32_t points_front_plate_texture() const { if (auto* d = direct()) return d->points_front_plate_texture; return uint32_t((memory::IsHandle(address + 5184) ? uint32_t(memory::OwnerLoad(address + 5184, 4)) : guest::LoadU32(address + 5184))); }
  void set_points_front_plate_texture(uint32_t v) const { if (auto* d = direct()) { d->points_front_plate_texture = v; return; } if (memory::IsHandle(address + 5184)) memory::OwnerStore(address + 5184, 4, v); else guest::StoreU32(address + 5184, v); }
  // the point block's +1460: the body's +7048
  uint32_t points_rear_plate_texture_address() const { return address + 5188; }
  uint32_t points_rear_plate_texture_bits() const { if (auto* d = direct()) return d->points_rear_plate_texture; return uint32_t((memory::IsHandle(address + 5188) ? uint32_t(memory::OwnerLoad(address + 5188, 4)) : guest::LoadU32(address + 5188))); }
  void set_points_rear_plate_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->points_rear_plate_texture = v; return; } if (memory::IsHandle(address + 5188)) memory::OwnerStore(address + 5188, 4, v); else guest::StoreU32(address + 5188, v); }
  uint32_t points_rear_plate_texture() const { if (auto* d = direct()) return d->points_rear_plate_texture; return uint32_t((memory::IsHandle(address + 5188) ? uint32_t(memory::OwnerLoad(address + 5188, 4)) : guest::LoadU32(address + 5188))); }
  void set_points_rear_plate_texture(uint32_t v) const { if (auto* d = direct()) { d->points_rear_plate_texture = v; return; } if (memory::IsHandle(address + 5188)) memory::OwnerStore(address + 5188, 4, v); else guest::StoreU32(address + 5188, v); }
  // the first set's traffic body (pool 6 of [0x826C2E40])
  uint32_t body_address() const { return address + 9760; }
  uint32_t body_bits() const { if (auto* d = direct()) return d->body; return uint32_t((memory::IsHandle(address + 9760) ? uint32_t(memory::OwnerLoad(address + 9760, 4)) : guest::LoadU32(address + 9760))); }
  void set_body_bits(uint32_t v) const { if (auto* d = direct()) { d->body = v; return; } if (memory::IsHandle(address + 9760)) memory::OwnerStore(address + 9760, 4, v); else guest::StoreU32(address + 9760, v); }
  uint32_t body() const { if (auto* d = direct()) return d->body; return uint32_t((memory::IsHandle(address + 9760) ? uint32_t(memory::OwnerLoad(address + 9760, 4)) : guest::LoadU32(address + 9760))); }
  void set_body(uint32_t v) const { if (auto* d = direct()) { d->body = v; return; } if (memory::IsHandle(address + 9760)) memory::OwnerStore(address + 9760, 4, v); else guest::StoreU32(address + 9760, v); }
  // the body's +3776
  uint32_t skeleton_address() const { return address + 9764; }
  uint32_t skeleton_bits() const { if (auto* d = direct()) return d->skeleton; return uint32_t((memory::IsHandle(address + 9764) ? uint32_t(memory::OwnerLoad(address + 9764, 4)) : guest::LoadU32(address + 9764))); }
  void set_skeleton_bits(uint32_t v) const { if (auto* d = direct()) { d->skeleton = v; return; } if (memory::IsHandle(address + 9764)) memory::OwnerStore(address + 9764, 4, v); else guest::StoreU32(address + 9764, v); }
  uint32_t skeleton() const { if (auto* d = direct()) return d->skeleton; return uint32_t((memory::IsHandle(address + 9764) ? uint32_t(memory::OwnerLoad(address + 9764, 4)) : guest::LoadU32(address + 9764))); }
  void set_skeleton(uint32_t v) const { if (auto* d = direct()) { d->skeleton = v; return; } if (memory::IsHandle(address + 9764)) memory::OwnerStore(address + 9764, 4, v); else guest::StoreU32(address + 9764, v); }
  // the body's six part models (its +4256 + 464i)
  uint32_t part_models_address() const { return address + 9768; }
  uint8_t* part_models() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 9768 : guest::Pointer(address + 9768); }
  uint32_t part_model_0_address() const { return address + 9768; }
  uint32_t part_model_0_bits() const { if (auto* d = direct()) return d->part_model_0; return uint32_t((memory::IsHandle(address + 9768) ? uint32_t(memory::OwnerLoad(address + 9768, 4)) : guest::LoadU32(address + 9768))); }
  void set_part_model_0_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_0 = v; return; } if (memory::IsHandle(address + 9768)) memory::OwnerStore(address + 9768, 4, v); else guest::StoreU32(address + 9768, v); }
  uint32_t part_model_0() const { if (auto* d = direct()) return d->part_model_0; return uint32_t((memory::IsHandle(address + 9768) ? uint32_t(memory::OwnerLoad(address + 9768, 4)) : guest::LoadU32(address + 9768))); }
  void set_part_model_0(uint32_t v) const { if (auto* d = direct()) { d->part_model_0 = v; return; } if (memory::IsHandle(address + 9768)) memory::OwnerStore(address + 9768, 4, v); else guest::StoreU32(address + 9768, v); }
  uint32_t part_model_1_address() const { return address + 9772; }
  uint32_t part_model_1_bits() const { if (auto* d = direct()) return d->part_model_1; return uint32_t((memory::IsHandle(address + 9772) ? uint32_t(memory::OwnerLoad(address + 9772, 4)) : guest::LoadU32(address + 9772))); }
  void set_part_model_1_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_1 = v; return; } if (memory::IsHandle(address + 9772)) memory::OwnerStore(address + 9772, 4, v); else guest::StoreU32(address + 9772, v); }
  uint32_t part_model_1() const { if (auto* d = direct()) return d->part_model_1; return uint32_t((memory::IsHandle(address + 9772) ? uint32_t(memory::OwnerLoad(address + 9772, 4)) : guest::LoadU32(address + 9772))); }
  void set_part_model_1(uint32_t v) const { if (auto* d = direct()) { d->part_model_1 = v; return; } if (memory::IsHandle(address + 9772)) memory::OwnerStore(address + 9772, 4, v); else guest::StoreU32(address + 9772, v); }
  uint32_t part_model_2_address() const { return address + 9776; }
  uint32_t part_model_2_bits() const { if (auto* d = direct()) return d->part_model_2; return uint32_t((memory::IsHandle(address + 9776) ? uint32_t(memory::OwnerLoad(address + 9776, 4)) : guest::LoadU32(address + 9776))); }
  void set_part_model_2_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_2 = v; return; } if (memory::IsHandle(address + 9776)) memory::OwnerStore(address + 9776, 4, v); else guest::StoreU32(address + 9776, v); }
  uint32_t part_model_2() const { if (auto* d = direct()) return d->part_model_2; return uint32_t((memory::IsHandle(address + 9776) ? uint32_t(memory::OwnerLoad(address + 9776, 4)) : guest::LoadU32(address + 9776))); }
  void set_part_model_2(uint32_t v) const { if (auto* d = direct()) { d->part_model_2 = v; return; } if (memory::IsHandle(address + 9776)) memory::OwnerStore(address + 9776, 4, v); else guest::StoreU32(address + 9776, v); }
  uint32_t part_model_3_address() const { return address + 9780; }
  uint32_t part_model_3_bits() const { if (auto* d = direct()) return d->part_model_3; return uint32_t((memory::IsHandle(address + 9780) ? uint32_t(memory::OwnerLoad(address + 9780, 4)) : guest::LoadU32(address + 9780))); }
  void set_part_model_3_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_3 = v; return; } if (memory::IsHandle(address + 9780)) memory::OwnerStore(address + 9780, 4, v); else guest::StoreU32(address + 9780, v); }
  uint32_t part_model_3() const { if (auto* d = direct()) return d->part_model_3; return uint32_t((memory::IsHandle(address + 9780) ? uint32_t(memory::OwnerLoad(address + 9780, 4)) : guest::LoadU32(address + 9780))); }
  void set_part_model_3(uint32_t v) const { if (auto* d = direct()) { d->part_model_3 = v; return; } if (memory::IsHandle(address + 9780)) memory::OwnerStore(address + 9780, 4, v); else guest::StoreU32(address + 9780, v); }
  uint32_t part_model_4_address() const { return address + 9784; }
  uint32_t part_model_4_bits() const { if (auto* d = direct()) return d->part_model_4; return uint32_t((memory::IsHandle(address + 9784) ? uint32_t(memory::OwnerLoad(address + 9784, 4)) : guest::LoadU32(address + 9784))); }
  void set_part_model_4_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_4 = v; return; } if (memory::IsHandle(address + 9784)) memory::OwnerStore(address + 9784, 4, v); else guest::StoreU32(address + 9784, v); }
  uint32_t part_model_4() const { if (auto* d = direct()) return d->part_model_4; return uint32_t((memory::IsHandle(address + 9784) ? uint32_t(memory::OwnerLoad(address + 9784, 4)) : guest::LoadU32(address + 9784))); }
  void set_part_model_4(uint32_t v) const { if (auto* d = direct()) { d->part_model_4 = v; return; } if (memory::IsHandle(address + 9784)) memory::OwnerStore(address + 9784, 4, v); else guest::StoreU32(address + 9784, v); }
  uint32_t part_model_5_address() const { return address + 9788; }
  uint32_t part_model_5_bits() const { if (auto* d = direct()) return d->part_model_5; return uint32_t((memory::IsHandle(address + 9788) ? uint32_t(memory::OwnerLoad(address + 9788, 4)) : guest::LoadU32(address + 9788))); }
  void set_part_model_5_bits(uint32_t v) const { if (auto* d = direct()) { d->part_model_5 = v; return; } if (memory::IsHandle(address + 9788)) memory::OwnerStore(address + 9788, 4, v); else guest::StoreU32(address + 9788, v); }
  uint32_t part_model_5() const { if (auto* d = direct()) return d->part_model_5; return uint32_t((memory::IsHandle(address + 9788) ? uint32_t(memory::OwnerLoad(address + 9788, 4)) : guest::LoadU32(address + 9788))); }
  void set_part_model_5(uint32_t v) const { if (auto* d = direct()) { d->part_model_5 = v; return; } if (memory::IsHandle(address + 9788)) memory::OwnerStore(address + 9788, 4, v); else guest::StoreU32(address + 9788, v); }
  // checked by SceneTraffic::Finish, never set there
  uint32_t finished_address() const { return address + 9792; }
  uint32_t finished_bits() const { if (auto* d = direct()) return d->finished; return uint32_t((memory::IsHandle(address + 9792) ? uint32_t(memory::OwnerLoad(address + 9792, 4)) : guest::LoadU32(address + 9792))); }
  void set_finished_bits(uint32_t v) const { if (auto* d = direct()) { d->finished = v; return; } if (memory::IsHandle(address + 9792)) memory::OwnerStore(address + 9792, 4, v); else guest::StoreU32(address + 9792, v); }
  uint32_t finished() const { if (auto* d = direct()) return d->finished; return uint32_t((memory::IsHandle(address + 9792) ? uint32_t(memory::OwnerLoad(address + 9792, 4)) : guest::LoadU32(address + 9792))); }
  void set_finished(uint32_t v) const { if (auto* d = direct()) { d->finished = v; return; } if (memory::IsHandle(address + 9792)) memory::OwnerStore(address + 9792, 4, v); else guest::StoreU32(address + 9792, v); }
  uint32_t scene_address() const { return address + 9796; }
  uint32_t scene_bits() const { if (auto* d = direct()) return d->scene; return uint32_t((memory::IsHandle(address + 9796) ? uint32_t(memory::OwnerLoad(address + 9796, 4)) : guest::LoadU32(address + 9796))); }
  void set_scene_bits(uint32_t v) const { if (auto* d = direct()) { d->scene = v; return; } if (memory::IsHandle(address + 9796)) memory::OwnerStore(address + 9796, 4, v); else guest::StoreU32(address + 9796, v); }
  template <typename T = CarScene>
  T scene() const { return T(scene_bits()); }
  void set_scene(const View& w) const { set_scene_bits(w.address); }
  void set_scene(uint32_t v) const { set_scene_bits(v); }
};

// embedded; measure: PointRecord, ModelPoint
struct SecondSetPointRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 240;
  // the skeleton (ModelPoint::Find; 0 from Clear)
  uint32_t skeleton() const { return guest::LoadU32(address + 0); }
  void set_skeleton(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t skeleton_address() const { return address + 0; }
  // the model found (0 until found)
  uint32_t model() const { return guest::LoadU32(address + 4); }
  void set_model(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t model_address() const { return address + 4; }
  // 0, or (FindShape) the shape found (544 bytes, the model's +40)
  uint32_t shape() const { return guest::LoadU32(address + 8); }
  void set_shape(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t shape_address() const { return address + 8; }
  // the node's matrix (the identity from Clear)
  uint32_t matrix_address() const { return address + 16; }
  // 1.0 (PointRecord::PointRecord)
  float matrix_row0_w() const { return guest::LoadFloat(address + 28); }
  void set_matrix_row0_w(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t matrix_row0_w_address() const { return address + 28; }
  float matrix_row1_w() const { return guest::LoadFloat(address + 44); }
  void set_matrix_row1_w(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t matrix_row1_w_address() const { return address + 44; }
  float matrix_row2_w() const { return guest::LoadFloat(address + 60); }
  void set_matrix_row2_w(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t matrix_row2_w_address() const { return address + 60; }
  // the translation (ModelPoint::Find scales it)
  float matrix_row3_x() const { return guest::LoadFloat(address + 64); }
  void set_matrix_row3_x(float v) const { guest::StoreFloat(address + 64, v); }
  uint32_t matrix_row3_x_address() const { return address + 64; }
  uint64_t matrix_row3_xy() const { return guest::LoadU64(address + 64); }
  void set_matrix_row3_xy(uint64_t v) const { guest::StoreU64(address + 64, v); }
  uint32_t matrix_row3_xy_address() const { return address + 64; }
  float matrix_row3_y() const { return guest::LoadFloat(address + 68); }
  void set_matrix_row3_y(float v) const { guest::StoreFloat(address + 68, v); }
  uint32_t matrix_row3_y_address() const { return address + 68; }
  float matrix_row3_z() const { return guest::LoadFloat(address + 72); }
  void set_matrix_row3_z(float v) const { guest::StoreFloat(address + 72, v); }
  uint32_t matrix_row3_z_address() const { return address + 72; }
  uint64_t matrix_row3_zw() const { return guest::LoadU64(address + 72); }
  void set_matrix_row3_zw(uint64_t v) const { guest::StoreU64(address + 72, v); }
  uint32_t matrix_row3_zw_address() const { return address + 72; }
  float matrix_row3_w() const { return guest::LoadFloat(address + 76); }
  void set_matrix_row3_w(float v) const { guest::StoreFloat(address + 76, v); }
  uint32_t matrix_row3_w_address() const { return address + 76; }
  // its inverse
  uint32_t inverse_address() const { return address + 80; }
  float inverse_row0_w() const { return guest::LoadFloat(address + 92); }
  void set_inverse_row0_w(float v) const { guest::StoreFloat(address + 92, v); }
  uint32_t inverse_row0_w_address() const { return address + 92; }
  float inverse_row1_w() const { return guest::LoadFloat(address + 108); }
  void set_inverse_row1_w(float v) const { guest::StoreFloat(address + 108, v); }
  uint32_t inverse_row1_w_address() const { return address + 108; }
  float inverse_row2_w() const { return guest::LoadFloat(address + 124); }
  void set_inverse_row2_w(float v) const { guest::StoreFloat(address + 124, v); }
  uint32_t inverse_row2_w_address() const { return address + 124; }
  float inverse_row3_w() const { return guest::LoadFloat(address + 140); }
  void set_inverse_row3_w(float v) const { guest::StoreFloat(address + 140, v); }
  uint32_t inverse_row3_w_address() const { return address + 140; }
  // its translation ((0, 0, 0, 1) from Clear)
  uint32_t position_address() const { return address + 144; }
  uint64_t position_xy() const { return guest::LoadU64(address + 144); }
  void set_position_xy(uint64_t v) const { guest::StoreU64(address + 144, v); }
  uint32_t position_xy_address() const { return address + 144; }
  uint64_t position_zw() const { return guest::LoadU64(address + 152); }
  void set_position_zw(uint64_t v) const { guest::StoreU64(address + 152, v); }
  uint32_t position_zw_address() const { return address + 152; }
  // 1.0 (PointRecord::PointRecord)
  float position_w() const { return guest::LoadFloat(address + 156); }
  void set_position_w(float v) const { guest::StoreFloat(address + 156, v); }
  uint32_t position_w_address() const { return address + 156; }
  // the node's name (SetNode; NameMatches)
  uint32_t node_name_address() const { return address + 160; }
  uint8_t node_name_0() const { return guest::LoadU8(address + 160); }
  void set_node_name_0(uint8_t v) const { guest::StoreU8(address + 160, v); }
  uint32_t node_name_0_address() const { return address + 160; }
  // the shape's name (SetShape)
  uint32_t shape_name_address() const { return address + 192; }
  uint8_t shape_name_0() const { return guest::LoadU8(address + 192); }
  void set_shape_name_0(uint8_t v) const { guest::StoreU8(address + 192, v); }
  uint32_t shape_name_0_address() const { return address + 192; }
  // -1 from Clear
  uint32_t node_index() const { return guest::LoadU32(address + 224); }
  void set_node_index(uint32_t v) const { guest::StoreU32(address + 224, v); }
  uint32_t node_index_address() const { return address + 224; }
  // -1, or (FindShape) the shape's index (the node's +40)
  uint32_t shape_index() const { return guest::LoadU32(address + 228); }
  void set_shape_index(uint32_t v) const { guest::StoreU32(address + 228, v); }
  uint32_t shape_index_address() const { return address + 228; }
  // measure fault: the parts' headlights group's +3120 matrix (InitHeadlights)
  float headlights_matrix_row0_w() const { return guest::LoadFloat(address + 3132); }
  void set_headlights_matrix_row0_w(float v) const { guest::StoreFloat(address + 3132, v); }
  uint32_t headlights_matrix_row0_w_address() const { return address + 3132; }
  float headlights_matrix_row1_w() const { return guest::LoadFloat(address + 3148); }
  void set_headlights_matrix_row1_w(float v) const { guest::StoreFloat(address + 3148, v); }
  uint32_t headlights_matrix_row1_w_address() const { return address + 3148; }
  float headlights_matrix_row2_w() const { return guest::LoadFloat(address + 3164); }
  void set_headlights_matrix_row2_w(float v) const { guest::StoreFloat(address + 3164, v); }
  uint32_t headlights_matrix_row2_w_address() const { return address + 3164; }
  float headlights_matrix_row3_w() const { return guest::LoadFloat(address + 3180); }
  void set_headlights_matrix_row3_w(float v) const { guest::StoreFloat(address + 3180, v); }
  uint32_t headlights_matrix_row3_w_address() const { return address + 3180; }
  // measure fault: the parts' doors' group's +6016 matrix (InitLeftDoor, InitRightDoor)
  float door_matrix_row0_w() const { return guest::LoadFloat(address + 6028); }
  void set_door_matrix_row0_w(float v) const { guest::StoreFloat(address + 6028, v); }
  uint32_t door_matrix_row0_w_address() const { return address + 6028; }
  float door_matrix_row1_w() const { return guest::LoadFloat(address + 6044); }
  void set_door_matrix_row1_w(float v) const { guest::StoreFloat(address + 6044, v); }
  uint32_t door_matrix_row1_w_address() const { return address + 6044; }
  float door_matrix_row2_w() const { return guest::LoadFloat(address + 6060); }
  void set_door_matrix_row2_w(float v) const { guest::StoreFloat(address + 6060, v); }
  uint32_t door_matrix_row2_w_address() const { return address + 6060; }
  float door_matrix_row3_w() const { return guest::LoadFloat(address + 6076); }
  void set_door_matrix_row3_w(float v) const { guest::StoreFloat(address + 6076, v); }
  uint32_t door_matrix_row3_w_address() const { return address + 6076; }
};

// embedded; measure: PointRecord::SetNode.r4, PointRecord::SetShape.r4
struct SecondSetPointName : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  // its characters, to the 0
  uint8_t first() const { return guest::LoadU8(address + 0); }
  void set_first(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t first_address() const { return address + 0; }
};

// embedded; measure: ModelPoint::FindShape.r4
struct SecondSetFindModel : View {
  using View::View;
  static constexpr uint32_t kSize = 412;
  // 192 bytes a node
  template <typename T = SecondSetModelNode>
  T nodes() const { return T(guest::LoadU32(address + 36)); }
  void set_nodes(const View& v) const { guest::StoreU32(address + 36, v.address); }
  void set_nodes(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t nodes_address() const { return address + 36; }
  // 544 bytes a shape
  template <typename T = ModelShape>
  T shapes() const { return T(guest::LoadU32(address + 40)); }
  void set_shapes(const View& v) const { guest::StoreU32(address + 40, v.address); }
  void set_shapes(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t shapes_address() const { return address + 40; }
  uint32_t node_count() const { return guest::LoadU32(address + 408); }
  void set_node_count(uint32_t v) const { guest::StoreU32(address + 408, v); }
  uint32_t node_count_address() const { return address + 408; }
};

// embedded; measure: ModelPoint::FindShape.r4.+36
struct SecondSetModelNode : View {
  using View::View;
  static constexpr uint32_t kSize = 192;
  // its shape's index (-1 none)
  uint32_t shape() const { return guest::LoadU32(address + 40); }
  void set_shape(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t shape_address() const { return address + 40; }
};

// embedded; measure: PartsPoints::DrawRearBumper.r3.+1444, SceneParts@+176@+2896.+1444, PartsPoints::DrawFrontBumper.r3.+1924, SceneParts@+176.+1924
struct SecondSetPlateModel : View {
  using View::View;
  static constexpr uint32_t kSize = 460;
  // set by the bumpers' draws (not read in detail)
  uint32_t unknown_432() const { return guest::LoadU32(address + 432); }
  void set_unknown_432(uint32_t v) const { guest::StoreU32(address + 432, v); }
  uint32_t unknown_432_address() const { return address + 432; }
  // the plate's texture (the group's +1684 or +2644)
  uint32_t texture() const { return guest::LoadU32(address + 456); }
  void set_texture(uint32_t v) const { guest::StoreU32(address + 456, v); }
  uint32_t texture_address() const { return address + 456; }
};

// embedded; measure: Morph, Morph::Run.r4, Morph::SetUp.r4
struct Morph : View {
  using View::View;
  static constexpr uint32_t kSize = 124;
  // the base shape's first stream ([shape + 400] + 16)
  uint32_t base_stream() const { return guest::LoadU32(address + 0); }
  void set_base_stream(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t base_stream_address() const { return address + 0; }
  // its second ([shape + 404] + 16)
  uint32_t base_stream2() const { return guest::LoadU32(address + 4); }
  void set_base_stream2(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t base_stream2_address() const { return address + 4; }
  // the first output (Morph::Run)
  template <typename T = MorphStream>
  T out1() const { return T(guest::LoadU32(address + 8)); }
  void set_out1(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_out1(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t out1_address() const { return address + 8; }
  // the second
  template <typename T = MorphStream>
  T out2() const { return T(guest::LoadU32(address + 12)); }
  void set_out2(const View& v) const { guest::StoreU32(address + 12, v.address); }
  void set_out2(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t out2_address() const { return address + 12; }
  // up to seven targets' first streams
  uint32_t targets_address() const { return address + 16; }
  uint32_t target0() const { return guest::LoadU32(address + 16); }
  void set_target0(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t target0_address() const { return address + 16; }
  // the seventh target's (Morph::SetUp reads it back)
  uint32_t target6() const { return guest::LoadU32(address + 40); }
  void set_target6(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t target6_address() const { return address + 40; }
  // the base's second stream, once a target
  uint32_t target_seconds_address() const { return address + 48; }
  template <typename T = MorphStream>
  T target_second0() const { return T(guest::LoadU32(address + 48)); }
  void set_target_second0(const View& v) const { guest::StoreU32(address + 48, v.address); }
  void set_target_second0(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t target_second0_address() const { return address + 48; }
  // the targets' weights
  uint32_t weights_address() const { return address + 80; }
  float weight0() const { return guest::LoadFloat(address + 80); }
  void set_weight0(float v) const { guest::StoreFloat(address + 80, v); }
  uint32_t weight0_address() const { return address + 80; }
  // the targets (8 and above taken as 7)
  uint32_t count() const { return guest::LoadU32(address + 112); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t count_address() const { return address + 112; }
  // [[shape + 400]]: the vertices (16 bytes each)
  uint32_t vertices() const { return guest::LoadU32(address + 116); }
  void set_vertices(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t vertices_address() const { return address + 116; }
  // [[shape + 404] + 4]: 6 Blend6, 7 Blend7, else 8 (sub_8258B7E0, never ran)
  uint32_t kind() const { return guest::LoadU32(address + 120); }
  void set_kind(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t kind_address() const { return address + 120; }
};

// embedded; measure: Morph.+12, Morph.+48
struct MorphStream : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // sub_8258B7E0 (never ran; not read in detail)
  float unknown_28() const { return guest::LoadFloat(address + 28); }
  void set_unknown_28(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
};

// embedded; measure: SceneCockpit@+4288
struct SceneCockpitAt4288 : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
};

// embedded; measure: SceneInterior@+11632
struct SceneInteriorAt11632 : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
};

// embedded; measure: SceneTraffic@+9768
struct SceneTrafficAt9768 : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
};

// embedded; measure: sub_820E6F28.r3
struct SecondSetWheelPointsAt12 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  float unknown_0() const { return guest::LoadFloat(address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  float unknown_16() const { return guest::LoadFloat(address + 16); }
  void set_unknown_16(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// embedded; measure: sub_820E7010.r5, Morph::SetUp.r7
struct SecondSetWheelPointsAt328 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float unknown_0() const { return guest::LoadFloat(address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: sub_820E7010.r6
struct SecondSetWheelPointsAt352 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float unknown_0() const { return guest::LoadFloat(address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: sub_820E7010.r7
struct SecondSetWheelPointsAt356 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float unknown_0() const { return guest::LoadFloat(address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: SceneWheels@+192
struct SecondSetWheelPointsAt192 : View {
  using View::View;
  static constexpr uint32_t kSize = 13648;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_32() const { return guest::LoadU32(address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_36() const { return guest::LoadU32(address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  float unknown_384() const { return guest::LoadFloat(address + 384); }
  void set_unknown_384(float v) const { guest::StoreFloat(address + 384, v); }
  uint32_t unknown_384_address() const { return address + 384; }
  float unknown_388() const { return guest::LoadFloat(address + 388); }
  void set_unknown_388(float v) const { guest::StoreFloat(address + 388, v); }
  uint32_t unknown_388_address() const { return address + 388; }
  uint32_t unknown_1872() const { return guest::LoadU32(address + 1872); }
  void set_unknown_1872(uint32_t v) const { guest::StoreU32(address + 1872, v); }
  uint32_t unknown_1872_address() const { return address + 1872; }
  uint32_t unknown_1876() const { return guest::LoadU32(address + 1876); }
  void set_unknown_1876(uint32_t v) const { guest::StoreU32(address + 1876, v); }
  uint32_t unknown_1876_address() const { return address + 1876; }
  uint32_t unknown_1880() const { return guest::LoadU32(address + 1880); }
  void set_unknown_1880(uint32_t v) const { guest::StoreU32(address + 1880, v); }
  uint32_t unknown_1880_address() const { return address + 1880; }
  uint32_t unknown_2172() const { return guest::LoadU32(address + 2172); }
  void set_unknown_2172(uint32_t v) const { guest::StoreU32(address + 2172, v); }
  uint32_t unknown_2172_address() const { return address + 2172; }
  float unknown_2224() const { return guest::LoadFloat(address + 2224); }
  void set_unknown_2224(float v) const { guest::StoreFloat(address + 2224, v); }
  uint32_t unknown_2224_address() const { return address + 2224; }
  float unknown_2228() const { return guest::LoadFloat(address + 2228); }
  void set_unknown_2228(float v) const { guest::StoreFloat(address + 2228, v); }
  uint32_t unknown_2228_address() const { return address + 2228; }
  uint32_t unknown_3712() const { return guest::LoadU32(address + 3712); }
  void set_unknown_3712(uint32_t v) const { guest::StoreU32(address + 3712, v); }
  uint32_t unknown_3712_address() const { return address + 3712; }
  uint32_t unknown_3720() const { return guest::LoadU32(address + 3720); }
  void set_unknown_3720(uint32_t v) const { guest::StoreU32(address + 3720, v); }
  uint32_t unknown_3720_address() const { return address + 3720; }
  float unknown_3728() const { return guest::LoadFloat(address + 3728); }
  void set_unknown_3728(float v) const { guest::StoreFloat(address + 3728, v); }
  uint32_t unknown_3728_address() const { return address + 3728; }
  float unknown_3732() const { return guest::LoadFloat(address + 3732); }
  void set_unknown_3732(float v) const { guest::StoreFloat(address + 3732, v); }
  uint32_t unknown_3732_address() const { return address + 3732; }
  uint32_t unknown_5456() const { return guest::LoadU32(address + 5456); }
  void set_unknown_5456(uint32_t v) const { guest::StoreU32(address + 5456, v); }
  uint32_t unknown_5456_address() const { return address + 5456; }
  uint32_t unknown_5464() const { return guest::LoadU32(address + 5464); }
  void set_unknown_5464(uint32_t v) const { guest::StoreU32(address + 5464, v); }
  uint32_t unknown_5464_address() const { return address + 5464; }
  uint32_t unknown_5468() const { return guest::LoadU32(address + 5468); }
  void set_unknown_5468(uint32_t v) const { guest::StoreU32(address + 5468, v); }
  uint32_t unknown_5468_address() const { return address + 5468; }
  float unknown_5472() const { return guest::LoadFloat(address + 5472); }
  void set_unknown_5472(float v) const { guest::StoreFloat(address + 5472, v); }
  uint32_t unknown_5472_address() const { return address + 5472; }
  float unknown_5476() const { return guest::LoadFloat(address + 5476); }
  void set_unknown_5476(float v) const { guest::StoreFloat(address + 5476, v); }
  uint32_t unknown_5476_address() const { return address + 5476; }
  uint32_t unknown_7200() const { return guest::LoadU32(address + 7200); }
  void set_unknown_7200(uint32_t v) const { guest::StoreU32(address + 7200, v); }
  uint32_t unknown_7200_address() const { return address + 7200; }
  uint32_t unknown_7208() const { return guest::LoadU32(address + 7208); }
  void set_unknown_7208(uint32_t v) const { guest::StoreU32(address + 7208, v); }
  uint32_t unknown_7208_address() const { return address + 7208; }
  float unknown_7216() const { return guest::LoadFloat(address + 7216); }
  void set_unknown_7216(float v) const { guest::StoreFloat(address + 7216, v); }
  uint32_t unknown_7216_address() const { return address + 7216; }
  float unknown_7220() const { return guest::LoadFloat(address + 7220); }
  void set_unknown_7220(float v) const { guest::StoreFloat(address + 7220, v); }
  uint32_t unknown_7220_address() const { return address + 7220; }
  uint32_t unknown_8944() const { return guest::LoadU32(address + 8944); }
  void set_unknown_8944(uint32_t v) const { guest::StoreU32(address + 8944, v); }
  uint32_t unknown_8944_address() const { return address + 8944; }
  uint32_t unknown_8952() const { return guest::LoadU32(address + 8952); }
  void set_unknown_8952(uint32_t v) const { guest::StoreU32(address + 8952, v); }
  uint32_t unknown_8952_address() const { return address + 8952; }
  uint32_t unknown_8956() const { return guest::LoadU32(address + 8956); }
  void set_unknown_8956(uint32_t v) const { guest::StoreU32(address + 8956, v); }
  uint32_t unknown_8956_address() const { return address + 8956; }
  float unknown_8960() const { return guest::LoadFloat(address + 8960); }
  void set_unknown_8960(float v) const { guest::StoreFloat(address + 8960, v); }
  uint32_t unknown_8960_address() const { return address + 8960; }
  float unknown_8964() const { return guest::LoadFloat(address + 8964); }
  void set_unknown_8964(float v) const { guest::StoreFloat(address + 8964, v); }
  uint32_t unknown_8964_address() const { return address + 8964; }
  uint32_t unknown_10688() const { return guest::LoadU32(address + 10688); }
  void set_unknown_10688(uint32_t v) const { guest::StoreU32(address + 10688, v); }
  uint32_t unknown_10688_address() const { return address + 10688; }
  uint32_t unknown_10692() const { return guest::LoadU32(address + 10692); }
  void set_unknown_10692(uint32_t v) const { guest::StoreU32(address + 10692, v); }
  uint32_t unknown_10692_address() const { return address + 10692; }
  uint32_t unknown_10696() const { return guest::LoadU32(address + 10696); }
  void set_unknown_10696(uint32_t v) const { guest::StoreU32(address + 10696, v); }
  uint32_t unknown_10696_address() const { return address + 10696; }
  uint32_t unknown_13632() const { return guest::LoadU32(address + 13632); }
  void set_unknown_13632(uint32_t v) const { guest::StoreU32(address + 13632, v); }
  uint32_t unknown_13632_address() const { return address + 13632; }
  uint32_t unknown_13636() const { return guest::LoadU32(address + 13636); }
  void set_unknown_13636(uint32_t v) const { guest::StoreU32(address + 13636, v); }
  uint32_t unknown_13636_address() const { return address + 13636; }
  uint32_t unknown_13640() const { return guest::LoadU32(address + 13640); }
  void set_unknown_13640(uint32_t v) const { guest::StoreU32(address + 13640, v); }
  uint32_t unknown_13640_address() const { return address + 13640; }
  uint32_t unknown_13644() const { return guest::LoadU32(address + 13644); }
  void set_unknown_13644(uint32_t v) const { guest::StoreU32(address + 13644, v); }
  uint32_t unknown_13644_address() const { return address + 13644; }
};

// The image words docs/functions/second-set.md's code uses.
struct SecondSetGlobals {
  // the car scene (its second set at +4944)
  template <typename T = CarScene>
  static T scene() { return T(guest::LoadU32(0x826C2E44u)); }
  static void set_scene(const View& v) { guest::StoreU32(0x826C2E44u, v.address); }
  static void set_scene(uint32_t v) { guest::StoreU32(0x826C2E44u, v); }
  static constexpr uint32_t scene_address() { return 0x826C2E44u; }
  // the first set of scene pools (ScenePools::ReleaseA's r3)
  template <typename T = ScenePools>
  static T first_set() { return T(guest::LoadU32(0x826C2E40u)); }
  static void set_first_set(const View& v) { guest::StoreU32(0x826C2E40u, v.address); }
  static void set_first_set(uint32_t v) { guest::StoreU32(0x826C2E40u, v); }
  static constexpr uint32_t first_set_address() { return 0x826C2E40u; }
  // 1.0
  static float one() { return guest::LoadFloat(0x82000A30u); }
  static void set_one(float v) { guest::StoreFloat(0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero() { return guest::LoadFloat(0x82000A34u); }
  static void set_zero(float v) { guest::StoreFloat(0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 3.0 (the traffic body's lights' scale)
  static float three() { return guest::LoadFloat(0x82006420u); }
  static void set_three(float v) { guest::StoreFloat(0x82006420u, v); }
  static constexpr uint32_t three_address() { return 0x82006420u; }
  // 5.0 (the parts' scale)
  static float five() { return guest::LoadFloat(0x82005FE4u); }
  static void set_five(float v) { guest::StoreFloat(0x82005FE4u, v); }
  static constexpr uint32_t five_address() { return 0x82005FE4u; }
  // 1/9 (probably): the brakes' morph weight a step
  static float brake_step() { return guest::LoadFloat(0x82006634u); }
  static void set_brake_step(float v) { guest::StoreFloat(0x82006634u, v); }
  static constexpr uint32_t brake_step_address() { return 0x82006634u; }
  // 24 bytes an entry by the block (+1080 for traffic): the tire's index, value and float, front then rear
  static constexpr uint32_t tire_table_address() { return 0x82003EF8u; }
  // 44 bytes an entry by the block (+1980 for traffic)
  static constexpr uint32_t index_table_address() { return 0x820044B0u; }
  // the parts', interior's, wheels' and traffic body's name tables (string pointers, to 0x82602A1B)
  static constexpr uint32_t parts_names_address() { return 0x826025F4u; }
  // the cockpit's name tables (to 0x8200527B)
  static constexpr uint32_t cockpit_names_address() { return 0x82005238u; }
  // the light kit's three 18-name tables (to 0x820056C7)
  static constexpr uint32_t light_names_address() { return 0x820055F0u; }
};

}  // namespace schema
