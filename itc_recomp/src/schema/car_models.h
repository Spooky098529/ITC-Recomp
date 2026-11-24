// The views of docs/functions/car-models.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/car-models.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: CarModel, CarModel.+24, Vinyl, CarModel::OnDecalLoaded.r6, CarModel::OnFileLoaded.r6, CarModel::OnFileLoadedB.r6, CarInterior.+24, Wheels.+24, SceneWheels.+192.+24, List::At.ret.+24, Cockpit.+24, TrafficCar.+24, sub_820C3CA0.r4, TGA::CreateImage.r5, CarParts.+24, CarModel::MakePlate.r3
#pragma pack(push, 1)
struct CarModelData {
  uint32_t vtable = 0;  // +0
  float unknown_4 = 0;  // +4
  uint8_t pad_8[8] = {};
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint32_t linked_car_model = 0;  // +24
  uint8_t pad_28[4] = {};
  uint32_t block = 0;  // +32
  uint8_t pad_36[160] = {};
  uint32_t unknown_196 = 0;  // +196
  uint8_t pad_200[16] = {};
  uint32_t layer0_region = 0;  // +216
  uint8_t pad_220[336] = {};
  uint32_t unknown_556 = 0;  // +556
  uint8_t pad_560[32] = {};
  uint32_t emblem_sticker = 0;  // +592
  uint8_t pad_596[4] = {};
  uint32_t emblem_sticker_region = 0;  // +600
  uint8_t pad_604[3060] = {};
  uint64_t plate_w0_w1 = 0;  // +3664
  uint64_t plate_w2_w3 = 0;  // +3672
  uint64_t plate_w4_w5 = 0;  // +3680
  uint64_t plate_w6_w7 = 0;  // +3688
  uint8_t plate_w8_w11[16] = {};  // +3696, guest order
  uint32_t selected_texture = 0;  // +3712
  uint8_t pad_3716[8] = {};
  uint32_t extra_image = 0;  // +3724
  uint32_t node_file_id = 0;  // +3728
  uint32_t node_file_request = 0;  // +3732
  uint32_t node_file_size = 0;  // +3736
  uint8_t pad_3740[4] = {};
  uint32_t node_file_buffer = 0;  // +3744
  uint32_t node_file_state = 0;  // +3748
  uint8_t pad_3752[92] = {};
  uint32_t skeleton_nodes = 0;  // +3844
  uint8_t pad_3848[60] = {};
  uint32_t skeleton_node_matrices = 0;  // +3908
  uint8_t pad_3912[40] = {};
  uint32_t body_xpr_id = 0;  // +3952
  uint32_t body_xpr_request = 0;  // +3956
  uint32_t body_xpr_size = 0;  // +3960
  uint8_t pad_3964[4] = {};
  uint32_t body_xpr_buffer = 0;  // +3968
  uint32_t body_xpr_state = 0;  // +3972
  uint8_t pad_3976[24] = {};
  uint8_t xpr_objects[48] = {};  // +4000, guest order
  uint8_t texture_providers[40] = {};  // +4048, guest order
  uint32_t decals_state = 0;  // +4088
  uint8_t layer_slots[288] = {};  // +4092, guest order
  uint8_t sticker_slots[2304] = {};  // +4380, guest order
  uint32_t vinyl_sheet = 0;  // +6684
  uint32_t vinyl_sheet_data = 0;  // +6688
  uint32_t unknown_6692 = 0;  // +6692
  uint32_t unknown_6696 = 0;  // +6696
  uint32_t plate_texture_a = 0;  // +6700
  uint32_t plate_texture_a_data = 0;  // +6704
  uint32_t plate_texture_b = 0;  // +6708
  uint32_t plate_texture_b_data = 0;  // +6712
  uint8_t pad_6716[4] = {};
  uint8_t tire_matrices[256] = {};  // +6720, guest order
  uint8_t tire_positions[64] = {};  // +6976, guest order
  uint8_t seat_matrices[128] = {};  // +7040, guest order
  uint8_t seat_positions[32] = {};  // +7168, guest order
  uint32_t accessory_matrix = 0;  // +7200
  uint8_t pad_7204[12] = {};
  uint8_t accessory_position[16] = {};  // +7216, guest order
  uint8_t xpr_indices[24] = {};  // +7232, guest order
  uint32_t first_set = 0;  // +7256
  uint8_t pad_7260[4] = {};
};
#pragma pack(pop)
static_assert(sizeof(CarModelData) == 7264);
inline constexpr memory::LayoutField kCarModelFields[] = {{0, 4}, {4, 4}, {16, 4}, {20, 4}, {24, 4}, {32, 4}, {196, 4}, {216, 4}, {556, 4}, {592, 4}, {600, 4}, {3664, 8}, {3672, 8}, {3680, 8}, {3688, 8}, {3712, 4}, {3724, 4}, {3728, 4}, {3732, 4}, {3736, 4}, {3744, 4}, {3748, 4}, {3844, 4}, {3908, 4}, {3952, 4}, {3956, 4}, {3960, 4}, {3968, 4}, {3972, 4}, {4088, 4}, {6684, 4}, {6688, 4}, {6692, 4}, {6696, 4}, {6700, 4}, {6704, 4}, {6708, 4}, {6712, 4}, {7200, 4}, {7256, 4}};
inline constexpr memory::Layout kCarModelLayout = {kCarModelFields, 40};

struct CarModel : View {
  using View::View;
  static constexpr uint32_t kSize = 7264;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82004F94;
  CarModelData* data() const { return static_cast<CarModelData*>(memory::HandleObject(address)); }
  static CarModel New(uint8_t* base) {
    return CarModel(base, memory::MakeHandle(new CarModelData(), sizeof(CarModelData), "CarModel", &kCarModelLayout));
  }
  template <uint32_t Stride = kSize>
  static CarModel NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CarModelData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarModelData*>(::operator new(sizeof(CarModelData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarModelData();
    return CarModel(base, memory::MakeHandle(p, sizeof(CarModelData) * count, "CarModel", &kCarModelLayout, sizeof(CarModelData)));
  }
  CarModel at(uint32_t i) const { return CarModel(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarModelData* direct() const { return static_cast<CarModelData*>(memory::HandleOwn(address, &kCarModelLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarModelLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarModelLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82004F94 (also where the measure's Vinyl puts Vinyl::DecalCorner's out point, Vinyl::Compress's sheet word and TGA::CreateImage's buffer word: joined)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // only Vinyl::DecalCorner's out point's y, joined with the car model by the measure's Vinyl; the car model's own +4 isn't known
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_4); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_unknown_4_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_4 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  float unknown_4() const { return std::bit_cast<float>(unknown_4_bits()); }
  void set_unknown_4(float v) const { set_unknown_4_bits(std::bit_cast<uint32_t>(v)); }
  // the pool object's: 3 in use
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // its index in the pool
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // the car's object (known-data.md); the measure's loads here are the second set's objects' +24 (their first set's car model: SceneModel::, SceneParts::, CarModel::SetVinylTextures' r3), joined
  uint32_t linked_car_model_address() const { return address + 24; }
  uint32_t linked_car_model_bits() const { if (auto* d = direct()) return d->linked_car_model; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_linked_car_model_bits(uint32_t v) const { if (auto* d = direct()) { d->linked_car_model = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  template <typename T = CarModel>
  T linked_car_model() const { return T(base, linked_car_model_bits()); }
  void set_linked_car_model(const View& w) const { set_linked_car_model_bits(w.address); }
  void set_linked_car_model(uint32_t v) const { set_linked_car_model_bits(v); }
  // a copy of the car's 3,696-byte set-up (the car record)
  uint32_t record_address() const { return address + 32; }
  uint8_t* record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 32 : guest::Pointer(base, address + 32); }
  // the record's +0: the block index 0-44
  uint32_t block_address() const { return address + 32; }
  uint32_t block_bits() const { if (auto* d = direct()) return d->block; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_block_bits(uint32_t v) const { if (auto* d = direct()) { d->block = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t block() const { if (auto* d = direct()) return d->block; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_block(uint32_t v) const { if (auto* d = direct()) { d->block = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // the record's +164: handed to the team emblem's lookup (sub_820D3CF8) as r5, not read there
  uint32_t unknown_196_address() const { return address + 196; }
  uint32_t unknown_196_bits() const { if (auto* d = direct()) return d->unknown_196; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(base, address + 196))); }
  void set_unknown_196_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_196 = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(base, address + 196, v); }
  uint32_t unknown_196() const { if (auto* d = direct()) return d->unknown_196; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(base, address + 196))); }
  void set_unknown_196(uint32_t v) const { if (auto* d = direct()) { d->unknown_196 = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(base, address + 196, v); }
  // six 64-byte vinyl layer records (CarModelDecalRecord)
  uint32_t vinyl_layers_address() const { return address + 208; }
  uint8_t* vinyl_layers() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 208 : guest::Pointer(base, address + 208); }
  // layer 0's +8: the region layers 0 to 2 are drawn into
  uint32_t layer0_region_address() const { return address + 216; }
  uint32_t layer0_region_bits() const { if (auto* d = direct()) return d->layer0_region; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(base, address + 216))); }
  void set_layer0_region_bits(uint32_t v) const { if (auto* d = direct()) { d->layer0_region = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(base, address + 216, v); }
  uint32_t layer0_region() const { if (auto* d = direct()) return d->layer0_region; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(base, address + 216))); }
  void set_layer0_region(uint32_t v) const { if (auto* d = direct()) { d->layer0_region = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(base, address + 216, v); }
  // read by sub_820D5FC8 (a walk from +556, not read)
  uint32_t unknown_556_address() const { return address + 556; }
  uint32_t unknown_556_bits() const { if (auto* d = direct()) return d->unknown_556; return uint32_t((memory::IsHandle(address + 556) ? uint32_t(memory::OwnerLoad(address + 556, 4)) : guest::LoadU32(base, address + 556))); }
  void set_unknown_556_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_556 = v; return; } if (memory::IsHandle(address + 556)) memory::OwnerStore(address + 556, 4, v); else guest::StoreU32(base, address + 556, v); }
  uint32_t unknown_556() const { if (auto* d = direct()) return d->unknown_556; return uint32_t((memory::IsHandle(address + 556) ? uint32_t(memory::OwnerLoad(address + 556, 4)) : guest::LoadU32(base, address + 556))); }
  void set_unknown_556(uint32_t v) const { if (auto* d = direct()) { d->unknown_556 = v; return; } if (memory::IsHandle(address + 556)) memory::OwnerStore(address + 556, 4, v); else guest::StoreU32(base, address + 556, v); }
  // eight groups of six 64-byte sticker records (CarModelDecalRecord), groups 0 to 7 at +1360, +976, +1744, +2128, +3280, +2896, +2512, +592
  uint32_t sticker_groups_address() const { return address + 592; }
  uint8_t* sticker_groups() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 592 : guest::Pointer(base, address + 592); }
  // group 7's first sticker's index: the team emblem's asset id for blocks 18 and up
  uint32_t emblem_sticker_address() const { return address + 592; }
  uint32_t emblem_sticker_bits() const { if (auto* d = direct()) return d->emblem_sticker; return uint32_t((memory::IsHandle(address + 592) ? uint32_t(memory::OwnerLoad(address + 592, 4)) : guest::LoadU32(base, address + 592))); }
  void set_emblem_sticker_bits(uint32_t v) const { if (auto* d = direct()) { d->emblem_sticker = v; return; } if (memory::IsHandle(address + 592)) memory::OwnerStore(address + 592, 4, v); else guest::StoreU32(base, address + 592, v); }
  uint32_t emblem_sticker() const { if (auto* d = direct()) return d->emblem_sticker; return uint32_t((memory::IsHandle(address + 592) ? uint32_t(memory::OwnerLoad(address + 592, 4)) : guest::LoadU32(base, address + 592))); }
  void set_emblem_sticker(uint32_t v) const { if (auto* d = direct()) { d->emblem_sticker = v; return; } if (memory::IsHandle(address + 592)) memory::OwnerStore(address + 592, 4, v); else guest::StoreU32(base, address + 592, v); }
  // its +8: 9 for the emblem (the id itself, not + 383)
  uint32_t emblem_sticker_region_address() const { return address + 600; }
  uint32_t emblem_sticker_region_bits() const { if (auto* d = direct()) return d->emblem_sticker_region; return uint32_t((memory::IsHandle(address + 600) ? uint32_t(memory::OwnerLoad(address + 600, 4)) : guest::LoadU32(base, address + 600))); }
  void set_emblem_sticker_region_bits(uint32_t v) const { if (auto* d = direct()) { d->emblem_sticker_region = v; return; } if (memory::IsHandle(address + 600)) memory::OwnerStore(address + 600, 4, v); else guest::StoreU32(base, address + 600, v); }
  uint32_t emblem_sticker_region() const { if (auto* d = direct()) return d->emblem_sticker_region; return uint32_t((memory::IsHandle(address + 600) ? uint32_t(memory::OwnerLoad(address + 600, 4)) : guest::LoadU32(base, address + 600))); }
  void set_emblem_sticker_region(uint32_t v) const { if (auto* d = direct()) { d->emblem_sticker_region = v; return; } if (memory::IsHandle(address + 600)) memory::OwnerStore(address + 600, 4, v); else guest::StoreU32(base, address + 600, v); }
  // the record's 48-byte plate struct MakePlate takes by value (W0 to W11: W1 a number, W2 to W11 the cells)
  uint32_t plate_address() const { return address + 3664; }
  uint8_t* plate() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 3664 : guest::Pointer(base, address + 3664); }
  uint32_t plate_w0_w1_address() const { return address + 3664; }
  uint64_t plate_w0_w1_bits() const { if (auto* d = direct()) return d->plate_w0_w1; return uint64_t((memory::IsHandle(address + 3664) ? uint64_t(memory::OwnerLoad(address + 3664, 8)) : guest::LoadU64(base, address + 3664))); }
  void set_plate_w0_w1_bits(uint64_t v) const { if (auto* d = direct()) { d->plate_w0_w1 = v; return; } if (memory::IsHandle(address + 3664)) memory::OwnerStore(address + 3664, 8, v); else guest::StoreU64(base, address + 3664, v); }
  uint64_t plate_w0_w1() const { if (auto* d = direct()) return d->plate_w0_w1; return uint64_t((memory::IsHandle(address + 3664) ? uint64_t(memory::OwnerLoad(address + 3664, 8)) : guest::LoadU64(base, address + 3664))); }
  void set_plate_w0_w1(uint64_t v) const { if (auto* d = direct()) { d->plate_w0_w1 = v; return; } if (memory::IsHandle(address + 3664)) memory::OwnerStore(address + 3664, 8, v); else guest::StoreU64(base, address + 3664, v); }
  uint32_t plate_w2_w3_address() const { return address + 3672; }
  uint64_t plate_w2_w3_bits() const { if (auto* d = direct()) return d->plate_w2_w3; return uint64_t((memory::IsHandle(address + 3672) ? uint64_t(memory::OwnerLoad(address + 3672, 8)) : guest::LoadU64(base, address + 3672))); }
  void set_plate_w2_w3_bits(uint64_t v) const { if (auto* d = direct()) { d->plate_w2_w3 = v; return; } if (memory::IsHandle(address + 3672)) memory::OwnerStore(address + 3672, 8, v); else guest::StoreU64(base, address + 3672, v); }
  uint64_t plate_w2_w3() const { if (auto* d = direct()) return d->plate_w2_w3; return uint64_t((memory::IsHandle(address + 3672) ? uint64_t(memory::OwnerLoad(address + 3672, 8)) : guest::LoadU64(base, address + 3672))); }
  void set_plate_w2_w3(uint64_t v) const { if (auto* d = direct()) { d->plate_w2_w3 = v; return; } if (memory::IsHandle(address + 3672)) memory::OwnerStore(address + 3672, 8, v); else guest::StoreU64(base, address + 3672, v); }
  uint32_t plate_w4_w5_address() const { return address + 3680; }
  uint64_t plate_w4_w5_bits() const { if (auto* d = direct()) return d->plate_w4_w5; return uint64_t((memory::IsHandle(address + 3680) ? uint64_t(memory::OwnerLoad(address + 3680, 8)) : guest::LoadU64(base, address + 3680))); }
  void set_plate_w4_w5_bits(uint64_t v) const { if (auto* d = direct()) { d->plate_w4_w5 = v; return; } if (memory::IsHandle(address + 3680)) memory::OwnerStore(address + 3680, 8, v); else guest::StoreU64(base, address + 3680, v); }
  uint64_t plate_w4_w5() const { if (auto* d = direct()) return d->plate_w4_w5; return uint64_t((memory::IsHandle(address + 3680) ? uint64_t(memory::OwnerLoad(address + 3680, 8)) : guest::LoadU64(base, address + 3680))); }
  void set_plate_w4_w5(uint64_t v) const { if (auto* d = direct()) { d->plate_w4_w5 = v; return; } if (memory::IsHandle(address + 3680)) memory::OwnerStore(address + 3680, 8, v); else guest::StoreU64(base, address + 3680, v); }
  uint32_t plate_w6_w7_address() const { return address + 3688; }
  uint64_t plate_w6_w7_bits() const { if (auto* d = direct()) return d->plate_w6_w7; return uint64_t((memory::IsHandle(address + 3688) ? uint64_t(memory::OwnerLoad(address + 3688, 8)) : guest::LoadU64(base, address + 3688))); }
  void set_plate_w6_w7_bits(uint64_t v) const { if (auto* d = direct()) { d->plate_w6_w7 = v; return; } if (memory::IsHandle(address + 3688)) memory::OwnerStore(address + 3688, 8, v); else guest::StoreU64(base, address + 3688, v); }
  uint64_t plate_w6_w7() const { if (auto* d = direct()) return d->plate_w6_w7; return uint64_t((memory::IsHandle(address + 3688) ? uint64_t(memory::OwnerLoad(address + 3688, 8)) : guest::LoadU64(base, address + 3688))); }
  void set_plate_w6_w7(uint64_t v) const { if (auto* d = direct()) { d->plate_w6_w7 = v; return; } if (memory::IsHandle(address + 3688)) memory::OwnerStore(address + 3688, 8, v); else guest::StoreU64(base, address + 3688, v); }
  // its last 16 bytes (memcpy'd into the callback's frame)
  uint32_t plate_w8_w11_address() const { return address + 3696; }
  uint8_t* plate_w8_w11() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 3696 : guest::Pointer(base, address + 3696); }
  // the record's +3680: which of +7232's indices CarModel::SelectedTexture (sub_820D4588) picks
  uint32_t selected_texture_address() const { return address + 3712; }
  uint32_t selected_texture_bits() const { if (auto* d = direct()) return d->selected_texture; return uint32_t((memory::IsHandle(address + 3712) ? uint32_t(memory::OwnerLoad(address + 3712, 4)) : guest::LoadU32(base, address + 3712))); }
  void set_selected_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->selected_texture = v; return; } if (memory::IsHandle(address + 3712)) memory::OwnerStore(address + 3712, 4, v); else guest::StoreU32(base, address + 3712, v); }
  uint32_t selected_texture() const { if (auto* d = direct()) return d->selected_texture; return uint32_t((memory::IsHandle(address + 3712) ? uint32_t(memory::OwnerLoad(address + 3712, 4)) : guest::LoadU32(base, address + 3712))); }
  void set_selected_texture(uint32_t v) const { if (auto* d = direct()) { d->selected_texture = v; return; } if (memory::IsHandle(address + 3712)) memory::OwnerStore(address + 3712, 4, v); else guest::StoreU32(base, address + 3712, v); }
  // 1: an image drawn into the sheet's region 8 (sub_820C4E68, the original's)
  uint32_t extra_image_address() const { return address + 3724; }
  uint32_t extra_image_bits() const { if (auto* d = direct()) return d->extra_image; return uint32_t((memory::IsHandle(address + 3724) ? uint32_t(memory::OwnerLoad(address + 3724, 4)) : guest::LoadU32(base, address + 3724))); }
  void set_extra_image_bits(uint32_t v) const { if (auto* d = direct()) { d->extra_image = v; return; } if (memory::IsHandle(address + 3724)) memory::OwnerStore(address + 3724, 4, v); else guest::StoreU32(base, address + 3724, v); }
  uint32_t extra_image() const { if (auto* d = direct()) return d->extra_image; return uint32_t((memory::IsHandle(address + 3724) ? uint32_t(memory::OwnerLoad(address + 3724, 4)) : guest::LoadU32(base, address + 3724))); }
  void set_extra_image(uint32_t v) const { if (auto* d = direct()) { d->extra_image = v; return; } if (memory::IsHandle(address + 3724)) memory::OwnerStore(address + 3724, 4, v); else guest::StoreU32(base, address + 3724, v); }
  // the node file's LoadSlot
  uint32_t node_file_slot_address() const { return address + 3728; }
  uint8_t* node_file_slot() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 3728 : guest::Pointer(base, address + 3728); }
  // set a: 893 + 226 x block; set b: 11063 + 197 x block
  uint32_t node_file_id_address() const { return address + 3728; }
  uint32_t node_file_id_bits() const { if (auto* d = direct()) return d->node_file_id; return uint32_t((memory::IsHandle(address + 3728) ? uint32_t(memory::OwnerLoad(address + 3728, 4)) : guest::LoadU32(base, address + 3728))); }
  void set_node_file_id_bits(uint32_t v) const { if (auto* d = direct()) { d->node_file_id = v; return; } if (memory::IsHandle(address + 3728)) memory::OwnerStore(address + 3728, 4, v); else guest::StoreU32(base, address + 3728, v); }
  uint32_t node_file_id() const { if (auto* d = direct()) return d->node_file_id; return uint32_t((memory::IsHandle(address + 3728) ? uint32_t(memory::OwnerLoad(address + 3728, 4)) : guest::LoadU32(base, address + 3728))); }
  void set_node_file_id(uint32_t v) const { if (auto* d = direct()) { d->node_file_id = v; return; } if (memory::IsHandle(address + 3728)) memory::OwnerStore(address + 3728, 4, v); else guest::StoreU32(base, address + 3728, v); }
  uint32_t node_file_request_address() const { return address + 3732; }
  uint32_t node_file_request_bits() const { if (auto* d = direct()) return d->node_file_request; return uint32_t((memory::IsHandle(address + 3732) ? uint32_t(memory::OwnerLoad(address + 3732, 4)) : guest::LoadU32(base, address + 3732))); }
  void set_node_file_request_bits(uint32_t v) const { if (auto* d = direct()) { d->node_file_request = v; return; } if (memory::IsHandle(address + 3732)) memory::OwnerStore(address + 3732, 4, v); else guest::StoreU32(base, address + 3732, v); }
  uint32_t node_file_request() const { if (auto* d = direct()) return d->node_file_request; return uint32_t((memory::IsHandle(address + 3732) ? uint32_t(memory::OwnerLoad(address + 3732, 4)) : guest::LoadU32(base, address + 3732))); }
  void set_node_file_request(uint32_t v) const { if (auto* d = direct()) { d->node_file_request = v; return; } if (memory::IsHandle(address + 3732)) memory::OwnerStore(address + 3732, 4, v); else guest::StoreU32(base, address + 3732, v); }
  uint32_t node_file_size_address() const { return address + 3736; }
  uint32_t node_file_size_bits() const { if (auto* d = direct()) return d->node_file_size; return uint32_t((memory::IsHandle(address + 3736) ? uint32_t(memory::OwnerLoad(address + 3736, 4)) : guest::LoadU32(base, address + 3736))); }
  void set_node_file_size_bits(uint32_t v) const { if (auto* d = direct()) { d->node_file_size = v; return; } if (memory::IsHandle(address + 3736)) memory::OwnerStore(address + 3736, 4, v); else guest::StoreU32(base, address + 3736, v); }
  uint32_t node_file_size() const { if (auto* d = direct()) return d->node_file_size; return uint32_t((memory::IsHandle(address + 3736) ? uint32_t(memory::OwnerLoad(address + 3736, 4)) : guest::LoadU32(base, address + 3736))); }
  void set_node_file_size(uint32_t v) const { if (auto* d = direct()) { d->node_file_size = v; return; } if (memory::IsHandle(address + 3736)) memory::OwnerStore(address + 3736, 4, v); else guest::StoreU32(base, address + 3736, v); }
  // its pages
  uint32_t node_file_buffer_address() const { return address + 3744; }
  uint32_t node_file_buffer_bits() const { if (auto* d = direct()) return d->node_file_buffer; return uint32_t((memory::IsHandle(address + 3744) ? uint32_t(memory::OwnerLoad(address + 3744, 4)) : guest::LoadU32(base, address + 3744))); }
  void set_node_file_buffer_bits(uint32_t v) const { if (auto* d = direct()) { d->node_file_buffer = v; return; } if (memory::IsHandle(address + 3744)) memory::OwnerStore(address + 3744, 4, v); else guest::StoreU32(base, address + 3744, v); }
  uint32_t node_file_buffer() const { if (auto* d = direct()) return d->node_file_buffer; return uint32_t((memory::IsHandle(address + 3744) ? uint32_t(memory::OwnerLoad(address + 3744, 4)) : guest::LoadU32(base, address + 3744))); }
  void set_node_file_buffer(uint32_t v) const { if (auto* d = direct()) { d->node_file_buffer = v; return; } if (memory::IsHandle(address + 3744)) memory::OwnerStore(address + 3744, 4, v); else guest::StoreU32(base, address + 3744, v); }
  // 2 loading, 3 loaded
  uint32_t node_file_state_address() const { return address + 3748; }
  uint32_t node_file_state_bits() const { if (auto* d = direct()) return d->node_file_state; return uint32_t((memory::IsHandle(address + 3748) ? uint32_t(memory::OwnerLoad(address + 3748, 4)) : guest::LoadU32(base, address + 3748))); }
  void set_node_file_state_bits(uint32_t v) const { if (auto* d = direct()) { d->node_file_state = v; return; } if (memory::IsHandle(address + 3748)) memory::OwnerStore(address + 3748, 4, v); else guest::StoreU32(base, address + 3748, v); }
  uint32_t node_file_state() const { if (auto* d = direct()) return d->node_file_state; return uint32_t((memory::IsHandle(address + 3748) ? uint32_t(memory::OwnerLoad(address + 3748, 4)) : guest::LoadU32(base, address + 3748))); }
  void set_node_file_state(uint32_t v) const { if (auto* d = direct()) { d->node_file_state = v; return; } if (memory::IsHandle(address + 3748)) memory::OwnerStore(address + 3748, 4, v); else guest::StoreU32(base, address + 3748, v); }
  // the skeleton (model-animation.md) the node file is bound to
  uint32_t skeleton_address() const { return address + 3760; }
  uint8_t* skeleton() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 3760 : guest::Pointer(base, address + 3760); }
  // the skeleton's +84: the node records (160 bytes, the name at +0)
  uint32_t skeleton_nodes_address() const { return address + 3844; }
  uint32_t skeleton_nodes_bits() const { if (auto* d = direct()) return d->skeleton_nodes; return uint32_t((memory::IsHandle(address + 3844) ? uint32_t(memory::OwnerLoad(address + 3844, 4)) : guest::LoadU32(base, address + 3844))); }
  void set_skeleton_nodes_bits(uint32_t v) const { if (auto* d = direct()) { d->skeleton_nodes = v; return; } if (memory::IsHandle(address + 3844)) memory::OwnerStore(address + 3844, 4, v); else guest::StoreU32(base, address + 3844, v); }
  template <typename T = AccessoryNodeRecord>
  T skeleton_nodes() const { return T(base, skeleton_nodes_bits()); }
  void set_skeleton_nodes(const View& w) const { set_skeleton_nodes_bits(w.address); }
  void set_skeleton_nodes(uint32_t v) const { set_skeleton_nodes_bits(v); }
  // the skeleton's +148: the nodes' 64-byte matrices
  uint32_t skeleton_node_matrices_address() const { return address + 3908; }
  uint32_t skeleton_node_matrices_bits() const { if (auto* d = direct()) return d->skeleton_node_matrices; return uint32_t((memory::IsHandle(address + 3908) ? uint32_t(memory::OwnerLoad(address + 3908, 4)) : guest::LoadU32(base, address + 3908))); }
  void set_skeleton_node_matrices_bits(uint32_t v) const { if (auto* d = direct()) { d->skeleton_node_matrices = v; return; } if (memory::IsHandle(address + 3908)) memory::OwnerStore(address + 3908, 4, v); else guest::StoreU32(base, address + 3908, v); }
  template <typename T = AccessoryMatrix>
  T skeleton_node_matrices() const { return T(base, skeleton_node_matrices_bits()); }
  void set_skeleton_node_matrices(const View& w) const { set_skeleton_node_matrices_bits(w.address); }
  void set_skeleton_node_matrices(uint32_t v) const { set_skeleton_node_matrices_bits(v); }
  // the two texture sets' XPR2 LoadSlots (24 bytes each)
  uint32_t xpr_slots_address() const { return address + 3952; }
  uint8_t* xpr_slots() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 3952 : guest::Pointer(base, address + 3952); }
  // set a: 891 + 226 x block; set b: 11061 + 197 x block
  uint32_t body_xpr_id_address() const { return address + 3952; }
  uint32_t body_xpr_id_bits() const { if (auto* d = direct()) return d->body_xpr_id; return uint32_t((memory::IsHandle(address + 3952) ? uint32_t(memory::OwnerLoad(address + 3952, 4)) : guest::LoadU32(base, address + 3952))); }
  void set_body_xpr_id_bits(uint32_t v) const { if (auto* d = direct()) { d->body_xpr_id = v; return; } if (memory::IsHandle(address + 3952)) memory::OwnerStore(address + 3952, 4, v); else guest::StoreU32(base, address + 3952, v); }
  uint32_t body_xpr_id() const { if (auto* d = direct()) return d->body_xpr_id; return uint32_t((memory::IsHandle(address + 3952) ? uint32_t(memory::OwnerLoad(address + 3952, 4)) : guest::LoadU32(base, address + 3952))); }
  void set_body_xpr_id(uint32_t v) const { if (auto* d = direct()) { d->body_xpr_id = v; return; } if (memory::IsHandle(address + 3952)) memory::OwnerStore(address + 3952, 4, v); else guest::StoreU32(base, address + 3952, v); }
  uint32_t body_xpr_request_address() const { return address + 3956; }
  uint32_t body_xpr_request_bits() const { if (auto* d = direct()) return d->body_xpr_request; return uint32_t((memory::IsHandle(address + 3956) ? uint32_t(memory::OwnerLoad(address + 3956, 4)) : guest::LoadU32(base, address + 3956))); }
  void set_body_xpr_request_bits(uint32_t v) const { if (auto* d = direct()) { d->body_xpr_request = v; return; } if (memory::IsHandle(address + 3956)) memory::OwnerStore(address + 3956, 4, v); else guest::StoreU32(base, address + 3956, v); }
  uint32_t body_xpr_request() const { if (auto* d = direct()) return d->body_xpr_request; return uint32_t((memory::IsHandle(address + 3956) ? uint32_t(memory::OwnerLoad(address + 3956, 4)) : guest::LoadU32(base, address + 3956))); }
  void set_body_xpr_request(uint32_t v) const { if (auto* d = direct()) { d->body_xpr_request = v; return; } if (memory::IsHandle(address + 3956)) memory::OwnerStore(address + 3956, 4, v); else guest::StoreU32(base, address + 3956, v); }
  uint32_t body_xpr_size_address() const { return address + 3960; }
  uint32_t body_xpr_size_bits() const { if (auto* d = direct()) return d->body_xpr_size; return uint32_t((memory::IsHandle(address + 3960) ? uint32_t(memory::OwnerLoad(address + 3960, 4)) : guest::LoadU32(base, address + 3960))); }
  void set_body_xpr_size_bits(uint32_t v) const { if (auto* d = direct()) { d->body_xpr_size = v; return; } if (memory::IsHandle(address + 3960)) memory::OwnerStore(address + 3960, 4, v); else guest::StoreU32(base, address + 3960, v); }
  uint32_t body_xpr_size() const { if (auto* d = direct()) return d->body_xpr_size; return uint32_t((memory::IsHandle(address + 3960) ? uint32_t(memory::OwnerLoad(address + 3960, 4)) : guest::LoadU32(base, address + 3960))); }
  void set_body_xpr_size(uint32_t v) const { if (auto* d = direct()) { d->body_xpr_size = v; return; } if (memory::IsHandle(address + 3960)) memory::OwnerStore(address + 3960, 4, v); else guest::StoreU32(base, address + 3960, v); }
  // its pages (set a)
  uint32_t body_xpr_buffer_address() const { return address + 3968; }
  uint32_t body_xpr_buffer_bits() const { if (auto* d = direct()) return d->body_xpr_buffer; return uint32_t((memory::IsHandle(address + 3968) ? uint32_t(memory::OwnerLoad(address + 3968, 4)) : guest::LoadU32(base, address + 3968))); }
  void set_body_xpr_buffer_bits(uint32_t v) const { if (auto* d = direct()) { d->body_xpr_buffer = v; return; } if (memory::IsHandle(address + 3968)) memory::OwnerStore(address + 3968, 4, v); else guest::StoreU32(base, address + 3968, v); }
  uint32_t body_xpr_buffer() const { if (auto* d = direct()) return d->body_xpr_buffer; return uint32_t((memory::IsHandle(address + 3968) ? uint32_t(memory::OwnerLoad(address + 3968, 4)) : guest::LoadU32(base, address + 3968))); }
  void set_body_xpr_buffer(uint32_t v) const { if (auto* d = direct()) { d->body_xpr_buffer = v; return; } if (memory::IsHandle(address + 3968)) memory::OwnerStore(address + 3968, 4, v); else guest::StoreU32(base, address + 3968, v); }
  // 1 empty, 2 loading, 3 made
  uint32_t body_xpr_state_address() const { return address + 3972; }
  uint32_t body_xpr_state_bits() const { if (auto* d = direct()) return d->body_xpr_state; return uint32_t((memory::IsHandle(address + 3972) ? uint32_t(memory::OwnerLoad(address + 3972, 4)) : guest::LoadU32(base, address + 3972))); }
  void set_body_xpr_state_bits(uint32_t v) const { if (auto* d = direct()) { d->body_xpr_state = v; return; } if (memory::IsHandle(address + 3972)) memory::OwnerStore(address + 3972, 4, v); else guest::StoreU32(base, address + 3972, v); }
  uint32_t body_xpr_state() const { if (auto* d = direct()) return d->body_xpr_state; return uint32_t((memory::IsHandle(address + 3972) ? uint32_t(memory::OwnerLoad(address + 3972, 4)) : guest::LoadU32(base, address + 3972))); }
  void set_body_xpr_state(uint32_t v) const { if (auto* d = direct()) { d->body_xpr_state = v; return; } if (memory::IsHandle(address + 3972)) memory::OwnerStore(address + 3972, 4, v); else guest::StoreU32(base, address + 3972, v); }
  // the two XPR objects (24 bytes each)
  uint32_t xpr_objects_address() const { return address + 4000; }
  uint8_t* xpr_objects() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4000 : guest::Pointer(base, address + 4000); }
  // two TextureProviders (20 bytes each)
  uint32_t texture_providers_address() const { return address + 4048; }
  uint8_t* texture_providers() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4048 : guest::Pointer(base, address + 4048); }
  // 0 reset, 2 loading, 3 done
  uint32_t decals_state_address() const { return address + 4088; }
  uint32_t decals_state_bits() const { if (auto* d = direct()) return d->decals_state; return uint32_t((memory::IsHandle(address + 4088) ? uint32_t(memory::OwnerLoad(address + 4088, 4)) : guest::LoadU32(base, address + 4088))); }
  void set_decals_state_bits(uint32_t v) const { if (auto* d = direct()) { d->decals_state = v; return; } if (memory::IsHandle(address + 4088)) memory::OwnerStore(address + 4088, 4, v); else guest::StoreU32(base, address + 4088, v); }
  uint32_t decals_state() const { if (auto* d = direct()) return d->decals_state; return uint32_t((memory::IsHandle(address + 4088) ? uint32_t(memory::OwnerLoad(address + 4088, 4)) : guest::LoadU32(base, address + 4088))); }
  void set_decals_state(uint32_t v) const { if (auto* d = direct()) { d->decals_state = v; return; } if (memory::IsHandle(address + 4088)) memory::OwnerStore(address + 4088, 4, v); else guest::StoreU32(base, address + 4088, v); }
  // six 48-byte decal slots, each two LoadSlots (the first's +20 the state: 0 empty, 1 none asked, 2 loading, 3 arrived)
  uint32_t layer_slots_address() const { return address + 4092; }
  uint8_t* layer_slots() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4092 : guest::Pointer(base, address + 4092); }
  // 48 decal slots as the layers'
  uint32_t sticker_slots_address() const { return address + 4380; }
  uint8_t* sticker_slots() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4380 : guest::Pointer(base, address + 4380); }
  // the vinyl sheet's texture header (A8R8G8B8, then DXT5 once compressed)
  uint32_t vinyl_sheet_address() const { return address + 6684; }
  uint32_t vinyl_sheet_bits() const { if (auto* d = direct()) return d->vinyl_sheet; return uint32_t((memory::IsHandle(address + 6684) ? uint32_t(memory::OwnerLoad(address + 6684, 4)) : guest::LoadU32(base, address + 6684))); }
  void set_vinyl_sheet_bits(uint32_t v) const { if (auto* d = direct()) { d->vinyl_sheet = v; return; } if (memory::IsHandle(address + 6684)) memory::OwnerStore(address + 6684, 4, v); else guest::StoreU32(base, address + 6684, v); }
  uint32_t vinyl_sheet() const { if (auto* d = direct()) return d->vinyl_sheet; return uint32_t((memory::IsHandle(address + 6684) ? uint32_t(memory::OwnerLoad(address + 6684, 4)) : guest::LoadU32(base, address + 6684))); }
  void set_vinyl_sheet(uint32_t v) const { if (auto* d = direct()) { d->vinyl_sheet = v; return; } if (memory::IsHandle(address + 6684)) memory::OwnerStore(address + 6684, 4, v); else guest::StoreU32(base, address + 6684, v); }
  // its pixels
  uint32_t vinyl_sheet_data_address() const { return address + 6688; }
  uint32_t vinyl_sheet_data_bits() const { if (auto* d = direct()) return d->vinyl_sheet_data; return uint32_t((memory::IsHandle(address + 6688) ? uint32_t(memory::OwnerLoad(address + 6688, 4)) : guest::LoadU32(base, address + 6688))); }
  void set_vinyl_sheet_data_bits(uint32_t v) const { if (auto* d = direct()) { d->vinyl_sheet_data = v; return; } if (memory::IsHandle(address + 6688)) memory::OwnerStore(address + 6688, 4, v); else guest::StoreU32(base, address + 6688, v); }
  uint32_t vinyl_sheet_data() const { if (auto* d = direct()) return d->vinyl_sheet_data; return uint32_t((memory::IsHandle(address + 6688) ? uint32_t(memory::OwnerLoad(address + 6688, 4)) : guest::LoadU32(base, address + 6688))); }
  void set_vinyl_sheet_data(uint32_t v) const { if (auto* d = direct()) { d->vinyl_sheet_data = v; return; } if (memory::IsHandle(address + 6688)) memory::OwnerStore(address + 6688, 4, v); else guest::StoreU32(base, address + 6688, v); }
  // handed with the sheet to the part models (Model::SetVinylTextures) and to DeferRelease
  uint32_t unknown_6692_address() const { return address + 6692; }
  uint32_t unknown_6692_bits() const { if (auto* d = direct()) return d->unknown_6692; return uint32_t((memory::IsHandle(address + 6692) ? uint32_t(memory::OwnerLoad(address + 6692, 4)) : guest::LoadU32(base, address + 6692))); }
  void set_unknown_6692_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_6692 = v; return; } if (memory::IsHandle(address + 6692)) memory::OwnerStore(address + 6692, 4, v); else guest::StoreU32(base, address + 6692, v); }
  uint32_t unknown_6692() const { if (auto* d = direct()) return d->unknown_6692; return uint32_t((memory::IsHandle(address + 6692) ? uint32_t(memory::OwnerLoad(address + 6692, 4)) : guest::LoadU32(base, address + 6692))); }
  void set_unknown_6692(uint32_t v) const { if (auto* d = direct()) { d->unknown_6692 = v; return; } if (memory::IsHandle(address + 6692)) memory::OwnerStore(address + 6692, 4, v); else guest::StoreU32(base, address + 6692, v); }
  // handed to Vinyl::DrawDecal (its caller's frame +140) and to DeferRelease
  uint32_t unknown_6696_address() const { return address + 6696; }
  uint32_t unknown_6696_bits() const { if (auto* d = direct()) return d->unknown_6696; return uint32_t((memory::IsHandle(address + 6696) ? uint32_t(memory::OwnerLoad(address + 6696, 4)) : guest::LoadU32(base, address + 6696))); }
  void set_unknown_6696_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_6696 = v; return; } if (memory::IsHandle(address + 6696)) memory::OwnerStore(address + 6696, 4, v); else guest::StoreU32(base, address + 6696, v); }
  uint32_t unknown_6696() const { if (auto* d = direct()) return d->unknown_6696; return uint32_t((memory::IsHandle(address + 6696) ? uint32_t(memory::OwnerLoad(address + 6696, 4)) : guest::LoadU32(base, address + 6696))); }
  void set_unknown_6696(uint32_t v) const { if (auto* d = direct()) { d->unknown_6696 = v; return; } if (memory::IsHandle(address + 6696)) memory::OwnerStore(address + 6696, 4, v); else guest::StoreU32(base, address + 6696, v); }
  // MakePlate's first texture's header
  uint32_t plate_texture_a_address() const { return address + 6700; }
  uint32_t plate_texture_a_bits() const { if (auto* d = direct()) return d->plate_texture_a; return uint32_t((memory::IsHandle(address + 6700) ? uint32_t(memory::OwnerLoad(address + 6700, 4)) : guest::LoadU32(base, address + 6700))); }
  void set_plate_texture_a_bits(uint32_t v) const { if (auto* d = direct()) { d->plate_texture_a = v; return; } if (memory::IsHandle(address + 6700)) memory::OwnerStore(address + 6700, 4, v); else guest::StoreU32(base, address + 6700, v); }
  uint32_t plate_texture_a() const { if (auto* d = direct()) return d->plate_texture_a; return uint32_t((memory::IsHandle(address + 6700) ? uint32_t(memory::OwnerLoad(address + 6700, 4)) : guest::LoadU32(base, address + 6700))); }
  void set_plate_texture_a(uint32_t v) const { if (auto* d = direct()) { d->plate_texture_a = v; return; } if (memory::IsHandle(address + 6700)) memory::OwnerStore(address + 6700, 4, v); else guest::StoreU32(base, address + 6700, v); }
  uint32_t plate_texture_a_data_address() const { return address + 6704; }
  uint32_t plate_texture_a_data_bits() const { if (auto* d = direct()) return d->plate_texture_a_data; return uint32_t((memory::IsHandle(address + 6704) ? uint32_t(memory::OwnerLoad(address + 6704, 4)) : guest::LoadU32(base, address + 6704))); }
  void set_plate_texture_a_data_bits(uint32_t v) const { if (auto* d = direct()) { d->plate_texture_a_data = v; return; } if (memory::IsHandle(address + 6704)) memory::OwnerStore(address + 6704, 4, v); else guest::StoreU32(base, address + 6704, v); }
  uint32_t plate_texture_a_data() const { if (auto* d = direct()) return d->plate_texture_a_data; return uint32_t((memory::IsHandle(address + 6704) ? uint32_t(memory::OwnerLoad(address + 6704, 4)) : guest::LoadU32(base, address + 6704))); }
  void set_plate_texture_a_data(uint32_t v) const { if (auto* d = direct()) { d->plate_texture_a_data = v; return; } if (memory::IsHandle(address + 6704)) memory::OwnerStore(address + 6704, 4, v); else guest::StoreU32(base, address + 6704, v); }
  uint32_t plate_texture_b_address() const { return address + 6708; }
  uint32_t plate_texture_b_bits() const { if (auto* d = direct()) return d->plate_texture_b; return uint32_t((memory::IsHandle(address + 6708) ? uint32_t(memory::OwnerLoad(address + 6708, 4)) : guest::LoadU32(base, address + 6708))); }
  void set_plate_texture_b_bits(uint32_t v) const { if (auto* d = direct()) { d->plate_texture_b = v; return; } if (memory::IsHandle(address + 6708)) memory::OwnerStore(address + 6708, 4, v); else guest::StoreU32(base, address + 6708, v); }
  uint32_t plate_texture_b() const { if (auto* d = direct()) return d->plate_texture_b; return uint32_t((memory::IsHandle(address + 6708) ? uint32_t(memory::OwnerLoad(address + 6708, 4)) : guest::LoadU32(base, address + 6708))); }
  void set_plate_texture_b(uint32_t v) const { if (auto* d = direct()) { d->plate_texture_b = v; return; } if (memory::IsHandle(address + 6708)) memory::OwnerStore(address + 6708, 4, v); else guest::StoreU32(base, address + 6708, v); }
  uint32_t plate_texture_b_data_address() const { return address + 6712; }
  uint32_t plate_texture_b_data_bits() const { if (auto* d = direct()) return d->plate_texture_b_data; return uint32_t((memory::IsHandle(address + 6712) ? uint32_t(memory::OwnerLoad(address + 6712, 4)) : guest::LoadU32(base, address + 6712))); }
  void set_plate_texture_b_data_bits(uint32_t v) const { if (auto* d = direct()) { d->plate_texture_b_data = v; return; } if (memory::IsHandle(address + 6712)) memory::OwnerStore(address + 6712, 4, v); else guest::StoreU32(base, address + 6712, v); }
  uint32_t plate_texture_b_data() const { if (auto* d = direct()) return d->plate_texture_b_data; return uint32_t((memory::IsHandle(address + 6712) ? uint32_t(memory::OwnerLoad(address + 6712, 4)) : guest::LoadU32(base, address + 6712))); }
  void set_plate_texture_b_data(uint32_t v) const { if (auto* d = direct()) { d->plate_texture_b_data = v; return; } if (memory::IsHandle(address + 6712)) memory::OwnerStore(address + 6712, 4, v); else guest::StoreU32(base, address + 6712, v); }
  // tire_fl, tire_fr, tire_rl, tire_rr's 64-byte matrices
  uint32_t tire_matrices_address() const { return address + 6720; }
  uint8_t* tire_matrices() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 6720 : guest::Pointer(base, address + 6720); }
  // their positions (16 bytes each), scaled by 0.1 and turned a quarter about y
  uint32_t tire_positions_address() const { return address + 6976; }
  uint8_t* tire_positions() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 6976 : guest::Pointer(base, address + 6976); }
  // seat_L, seat_R's matrices
  uint32_t seat_matrices_address() const { return address + 7040; }
  uint8_t* seat_matrices() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 7040 : guest::Pointer(base, address + 7040); }
  uint32_t seat_positions_address() const { return address + 7168; }
  uint8_t* seat_positions() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 7168 : guest::Pointer(base, address + 7168); }
  // the acce_null node's matrix (in the skeleton's)
  uint32_t accessory_matrix_address() const { return address + 7200; }
  uint32_t accessory_matrix_bits() const { if (auto* d = direct()) return d->accessory_matrix; return uint32_t((memory::IsHandle(address + 7200) ? uint32_t(memory::OwnerLoad(address + 7200, 4)) : guest::LoadU32(base, address + 7200))); }
  void set_accessory_matrix_bits(uint32_t v) const { if (auto* d = direct()) { d->accessory_matrix = v; return; } if (memory::IsHandle(address + 7200)) memory::OwnerStore(address + 7200, 4, v); else guest::StoreU32(base, address + 7200, v); }
  template <typename T = AccessoryMatrix>
  T accessory_matrix() const { return T(base, accessory_matrix_bits()); }
  void set_accessory_matrix(const View& w) const { set_accessory_matrix_bits(w.address); }
  void set_accessory_matrix(uint32_t v) const { set_accessory_matrix_bits(v); }
  // +7228 1.0 from the constructor
  uint32_t accessory_position_address() const { return address + 7216; }
  uint8_t* accessory_position() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 7216 : guest::Pointer(base, address + 7216); }
  // six indices into the body XPR (-1 none)
  uint32_t xpr_indices_address() const { return address + 7232; }
  uint8_t* xpr_indices() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 7232 : guest::Pointer(base, address + 7232); }
  // the first set of scene pools ([0x826C2E40], scene-loaders.md): its +8 the model set (0 a, 1 b)
  uint32_t first_set_address() const { return address + 7256; }
  uint32_t first_set_bits() const { if (auto* d = direct()) return d->first_set; return uint32_t((memory::IsHandle(address + 7256) ? uint32_t(memory::OwnerLoad(address + 7256, 4)) : guest::LoadU32(base, address + 7256))); }
  void set_first_set_bits(uint32_t v) const { if (auto* d = direct()) { d->first_set = v; return; } if (memory::IsHandle(address + 7256)) memory::OwnerStore(address + 7256, 4, v); else guest::StoreU32(base, address + 7256, v); }
  template <typename T = ScenePools>
  T first_set() const { return T(base, first_set_bits()); }
  void set_first_set(const View& w) const { set_first_set_bits(w.address); }
  void set_first_set(uint32_t v) const { set_first_set_bits(v); }
};

// embedded; measure: -
struct CarModelDecalRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the asset index, negative none (layers + 163 or + 189; stickers + 383, or the id itself with region 9)
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
  // stickers only
  uint32_t mirrored() const { return guest::LoadU32(base, address + 4); }
  void set_mirrored(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t mirrored_address() const { return address + 4; }
  // the sheet's region (9 the whole sheet, the team emblem's)
  uint32_t region() const { return guest::LoadU32(base, address + 8); }
  void set_region(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t region_address() const { return address + 8; }
  // the offset from the region's center (pixels), x and y
  uint64_t offset_xy() const { return guest::LoadU64(base, address + 16); }
  void set_offset_xy(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t offset_xy_address() const { return address + 16; }
  uint64_t offset_zw() const { return guest::LoadU64(base, address + 24); }
  void set_offset_zw(uint64_t v) const { guest::StoreU64(base, address + 24, v); }
  uint32_t offset_zw_address() const { return address + 24; }
  // radians, probably
  float rotation() const { return guest::LoadFloat(base, address + 32); }
  void set_rotation(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t rotation_address() const { return address + 32; }
  // x and y the scale
  uint32_t scale_address() const { return address + 48; }
};

// allocated; measure: -
struct CarModelsFile : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  static CarModelsFile New(uint8_t* base, uint32_t size) { return CarModelsFile(base, memory::NewBlob(size, "CarModelsFile")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
};

// embedded; measure: LoadSlot, LoadSlot::Load.r3, LoadSlot::LoadShared.r3
struct LoadSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // the asset id
  uint32_t id() const { return guest::LoadU32(base, address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
  // Archive::Load's serial; -1 when reset
  uint32_t request() const { return guest::LoadU32(base, address + 4); }
  void set_request(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t request_address() const { return address + 4; }
  // the asset's size
  uint32_t size() const { return guest::LoadU32(base, address + 8); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t size_address() const { return address + 8; }
  // 1: the buffer is the shared load buffer (not freed)
  uint32_t shared() const { return guest::LoadU32(base, address + 12); }
  void set_shared(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t shared_address() const { return address + 12; }
  uint32_t buffer() const { return guest::LoadU32(base, address + 16); }
  void set_buffer(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t buffer_address() const { return address + 16; }
  // 0 empty, 1 nothing loaded, 2 loading, 3 arrived
  uint32_t state() const { return guest::LoadU32(base, address + 20); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t state_address() const { return address + 20; }
};

// allocated; measure: CarInterior
struct CarInterior : View {
  using View::View;
  static constexpr uint32_t kSize = 3104;
  // both
  template <typename T = CarModel>
  T car_model() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_car_model(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_car_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
  // a copy of the set-up record: the interior's 120 bytes, the cockpit's 144
  uint32_t setup_address() const { return address + 28; }
  // the cockpit's record +0: the bonnet's part (body + 3 + it)
  uint32_t cockpit_bonnet() const { return guest::LoadU32(base, address + 28); }
  void set_cockpit_bonnet(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t cockpit_bonnet_address() const { return address + 28; }
  // body + 16 + it
  uint32_t cockpit_door() const { return guest::LoadU32(base, address + 32); }
  void set_cockpit_door(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t cockpit_door_address() const { return address + 32; }
  // body + 19 + it
  uint32_t cockpit_mirrors() const { return guest::LoadU32(base, address + 36); }
  void set_cockpit_mirrors(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t cockpit_mirrors_address() const { return address + 36; }
  // body + 31 + it
  uint32_t cockpit_headlights() const { return guest::LoadU32(base, address + 40); }
  void set_cockpit_headlights(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t cockpit_headlights_address() const { return address + 40; }
  // body + 36 + it
  uint32_t cockpit_fenders() const { return guest::LoadU32(base, address + 44); }
  void set_cockpit_fenders(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t cockpit_fenders_address() const { return address + 44; }
  // the interior's record +112: the accessory picked (set b)
  uint32_t accessory() const { return guest::LoadU32(base, address + 140); }
  void set_accessory(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t accessory_address() const { return address + 140; }
  // the interior's record +116: the accessory load's r5 (set b)
  uint32_t accessory_arg() const { return guest::LoadU32(base, address + 144); }
  void set_accessory_arg(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t accessory_arg_address() const { return address + 144; }
  // the interior's six parts' LoadSlots (CarInterior::LoadParts)
  uint32_t part_slots_address() const { return address + 160; }
  // the cockpit's node file's LoadSlot (19928 + 55 x block)
  uint32_t cockpit_node_file_slot_address() const { return address + 176; }
  // the cockpit's body XPR2 slot: 19926 + 55 x block
  uint32_t cockpit_body_id() const { return guest::LoadU32(base, address + 400); }
  void set_cockpit_body_id(uint32_t v) const { guest::StoreU32(base, address + 400, v); }
  uint32_t cockpit_body_id_address() const { return address + 400; }
  uint32_t cockpit_body_request() const { return guest::LoadU32(base, address + 404); }
  void set_cockpit_body_request(uint32_t v) const { guest::StoreU32(base, address + 404, v); }
  uint32_t cockpit_body_request_address() const { return address + 404; }
  uint32_t cockpit_body_size() const { return guest::LoadU32(base, address + 408); }
  void set_cockpit_body_size(uint32_t v) const { guest::StoreU32(base, address + 408, v); }
  uint32_t cockpit_body_size_address() const { return address + 408; }
  // 2 loading
  uint32_t cockpit_body_state() const { return guest::LoadU32(base, address + 420); }
  void set_cockpit_body_state(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t cockpit_body_state_address() const { return address + 420; }
  // the cockpit's six parts' LoadSlots
  uint32_t cockpit_part_slots_address() const { return address + 536; }
  // the interior's: nonzero the cockpit set (c)
  uint32_t cockpit() const { return guest::LoadU32(base, address + 3088); }
  void set_cockpit(uint32_t v) const { guest::StoreU32(base, address + 3088, v); }
  uint32_t cockpit_address() const { return address + 3088; }
  // the interior's
  template <typename T = ScenePools>
  T first_set() const { return T(base, guest::LoadU32(base, address + 3092)); }
  void set_first_set(const View& v) const { guest::StoreU32(base, address + 3092, v.address); }
  void set_first_set(uint32_t v) const { guest::StoreU32(base, address + 3092, v); }
  uint32_t first_set_address() const { return address + 3092; }
};

// allocated; measure: SceneWheels.+192, SceneCockpit.+24, SceneInterior::Link.r4, ScenePool4.+24
struct CarModelsSceneCarModel : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // the first set's car model
  template <typename T = CarModel>
  T car_model() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_car_model(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_car_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
};

// allocated; measure: Wheels.+3264, SceneWheels.+196
struct CarModelsTrafficIndex : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the traffic car's wheel is 20 + it; its wheel table entry 45 + it
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
};

// embedded; measure: Texture::Create.r6, Texture::Create.r7, Texture::Free.r4, TGA::ToARGB.r4, PlateImage.r7, FreePagesAt.r4, CarModel::MakePlate.r4, CarModel::MakePlate.r6
struct CarModelsAddressWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the address
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: TgaReader
struct TgaReader : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  // a std::string
  uint32_t text_address() const { return address + 0; }
  uint32_t file_size() const { return guest::LoadU32(base, address + 28); }
  void set_file_size(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t file_size_address() const { return address + 28; }
  // the file's buffer, 0 when not loading
  template <typename T = TgaFile>
  T file() const { return T(base, guest::LoadU32(base, address + 32)); }
  void set_file(const View& v) const { guest::StoreU32(base, address + 32, v.address); }
  void set_file(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t file_address() const { return address + 32; }
  // a TgaHeader
  uint32_t header_address() const { return address + 36; }
  // a TgaImage (+0 never written)
  uint32_t image_address() const { return address + 56; }
};

// file; measure: TgaReader.+32
struct TgaFile : View {
  using View::View;
  static constexpr uint32_t kSize = 18;
  uint8_t id_length() const { return guest::LoadU8(base, address + 0); }
  void set_id_length(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t id_length_address() const { return address + 0; }
  uint8_t color_map_type() const { return guest::LoadU8(base, address + 1); }
  void set_color_map_type(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t color_map_type_address() const { return address + 1; }
  // 2 uncompressed true color, 10 RLE
  uint8_t image_type() const { return guest::LoadU8(base, address + 2); }
  void set_image_type(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t image_type_address() const { return address + 2; }
  uint8_t color_map_first_lo() const { return guest::LoadU8(base, address + 3); }
  void set_color_map_first_lo(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t color_map_first_lo_address() const { return address + 3; }
  uint8_t color_map_first_hi() const { return guest::LoadU8(base, address + 4); }
  void set_color_map_first_hi(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t color_map_first_hi_address() const { return address + 4; }
  uint8_t color_map_length_lo() const { return guest::LoadU8(base, address + 5); }
  void set_color_map_length_lo(uint8_t v) const { guest::StoreU8(base, address + 5, v); }
  uint32_t color_map_length_lo_address() const { return address + 5; }
  uint8_t color_map_length_hi() const { return guest::LoadU8(base, address + 6); }
  void set_color_map_length_hi(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t color_map_length_hi_address() const { return address + 6; }
  uint8_t color_map_depth() const { return guest::LoadU8(base, address + 7); }
  void set_color_map_depth(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t color_map_depth_address() const { return address + 7; }
  uint8_t x_origin_lo() const { return guest::LoadU8(base, address + 8); }
  void set_x_origin_lo(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t x_origin_lo_address() const { return address + 8; }
  uint8_t x_origin_hi() const { return guest::LoadU8(base, address + 9); }
  void set_x_origin_hi(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t x_origin_hi_address() const { return address + 9; }
  uint8_t y_origin_lo() const { return guest::LoadU8(base, address + 10); }
  void set_y_origin_lo(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t y_origin_lo_address() const { return address + 10; }
  uint8_t y_origin_hi() const { return guest::LoadU8(base, address + 11); }
  void set_y_origin_hi(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t y_origin_hi_address() const { return address + 11; }
  uint8_t width_lo() const { return guest::LoadU8(base, address + 12); }
  void set_width_lo(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t width_lo_address() const { return address + 12; }
  uint8_t width_hi() const { return guest::LoadU8(base, address + 13); }
  void set_width_hi(uint8_t v) const { guest::StoreU8(base, address + 13, v); }
  uint32_t width_hi_address() const { return address + 13; }
  uint8_t height_lo() const { return guest::LoadU8(base, address + 14); }
  void set_height_lo(uint8_t v) const { guest::StoreU8(base, address + 14, v); }
  uint32_t height_lo_address() const { return address + 14; }
  uint8_t height_hi() const { return guest::LoadU8(base, address + 15); }
  void set_height_hi(uint8_t v) const { guest::StoreU8(base, address + 15, v); }
  uint32_t height_hi_address() const { return address + 15; }
  uint8_t bits_per_pixel() const { return guest::LoadU8(base, address + 16); }
  void set_bits_per_pixel(uint8_t v) const { guest::StoreU8(base, address + 16, v); }
  uint32_t bits_per_pixel_address() const { return address + 16; }
  uint8_t descriptor() const { return guest::LoadU8(base, address + 17); }
  void set_descriptor(uint8_t v) const { guest::StoreU8(base, address + 17, v); }
  uint32_t descriptor_address() const { return address + 17; }
};

// embedded; measure: TgaHeader::Clear.r3
struct TgaHeader : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint8_t id_length() const { return guest::LoadU8(base, address + 0); }
  void set_id_length(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t id_length_address() const { return address + 0; }
  uint8_t color_map_type() const { return guest::LoadU8(base, address + 1); }
  void set_color_map_type(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t color_map_type_address() const { return address + 1; }
  uint8_t image_type() const { return guest::LoadU8(base, address + 2); }
  void set_image_type(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t image_type_address() const { return address + 2; }
  uint16_t color_map_first() const { return guest::LoadU16(base, address + 4); }
  void set_color_map_first(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t color_map_first_address() const { return address + 4; }
  uint16_t color_map_length() const { return guest::LoadU16(base, address + 6); }
  void set_color_map_length(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t color_map_length_address() const { return address + 6; }
  uint8_t color_map_depth() const { return guest::LoadU8(base, address + 8); }
  void set_color_map_depth(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t color_map_depth_address() const { return address + 8; }
  uint16_t x_origin() const { return guest::LoadU16(base, address + 10); }
  void set_x_origin(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t x_origin_address() const { return address + 10; }
  uint16_t y_origin() const { return guest::LoadU16(base, address + 12); }
  void set_y_origin(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t y_origin_address() const { return address + 12; }
  uint16_t width() const { return guest::LoadU16(base, address + 14); }
  void set_width(uint16_t v) const { guest::StoreU16(base, address + 14, v); }
  uint32_t width_address() const { return address + 14; }
  uint16_t height() const { return guest::LoadU16(base, address + 16); }
  void set_height(uint16_t v) const { guest::StoreU16(base, address + 16, v); }
  uint32_t height_address() const { return address + 16; }
  uint8_t bits_per_pixel() const { return guest::LoadU8(base, address + 18); }
  void set_bits_per_pixel(uint8_t v) const { guest::StoreU8(base, address + 18, v); }
  uint32_t bits_per_pixel_address() const { return address + 18; }
  uint8_t descriptor() const { return guest::LoadU8(base, address + 19); }
  void set_descriptor(uint8_t v) const { guest::StoreU8(base, address + 19, v); }
  uint32_t descriptor_address() const { return address + 19; }
};

// embedded; measure: TgaReader@+56
struct TgaImage : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // the file's buffer
  uint32_t file() const { return guest::LoadU32(base, address + 4); }
  void set_file(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t file_address() const { return address + 4; }
  template <typename T = TgaHeader>
  T header() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_header(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_header(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t header_address() const { return address + 8; }
  // the file's pixels copied (VirtualAlloc)
  uint32_t pixels() const { return guest::LoadU32(base, address + 12); }
  void set_pixels(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t pixels_address() const { return address + 12; }
  // their size in bytes
  uint32_t size() const { return guest::LoadU32(base, address + 16); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t size_address() const { return address + 16; }
  uint32_t width() const { return guest::LoadU32(base, address + 20); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t width_address() const { return address + 20; }
  uint32_t height() const { return guest::LoadU32(base, address + 24); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t height_address() const { return address + 24; }
};

// embedded; measure: TGA, TGA.+8
struct TgaHeaderOrImage : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint8_t id_length() const { return guest::LoadU8(base, address + 0); }
  void set_id_length(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t id_length_address() const { return address + 0; }
  uint8_t color_map_type() const { return guest::LoadU8(base, address + 1); }
  void set_color_map_type(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t color_map_type_address() const { return address + 1; }
  uint8_t image_type() const { return guest::LoadU8(base, address + 2); }
  void set_image_type(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t image_type_address() const { return address + 2; }
  uint16_t color_map_first() const { return guest::LoadU16(base, address + 4); }
  void set_color_map_first(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t color_map_first_address() const { return address + 4; }
  uint8_t color_map_first_hi() const { return guest::LoadU8(base, address + 4); }
  void set_color_map_first_hi(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t color_map_first_hi_address() const { return address + 4; }
  uint8_t color_map_first_lo() const { return guest::LoadU8(base, address + 5); }
  void set_color_map_first_lo(uint8_t v) const { guest::StoreU8(base, address + 5, v); }
  uint32_t color_map_first_lo_address() const { return address + 5; }
  uint16_t color_map_length() const { return guest::LoadU16(base, address + 6); }
  void set_color_map_length(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t color_map_length_address() const { return address + 6; }
  uint8_t color_map_length_hi() const { return guest::LoadU8(base, address + 6); }
  void set_color_map_length_hi(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t color_map_length_hi_address() const { return address + 6; }
  uint8_t color_map_length_lo() const { return guest::LoadU8(base, address + 7); }
  void set_color_map_length_lo(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t color_map_length_lo_address() const { return address + 7; }
  uint8_t color_map_depth() const { return guest::LoadU8(base, address + 8); }
  void set_color_map_depth(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t color_map_depth_address() const { return address + 8; }
  uint16_t x_origin() const { return guest::LoadU16(base, address + 10); }
  void set_x_origin(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t x_origin_address() const { return address + 10; }
  uint8_t x_origin_hi() const { return guest::LoadU8(base, address + 10); }
  void set_x_origin_hi(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t x_origin_hi_address() const { return address + 10; }
  uint8_t x_origin_lo() const { return guest::LoadU8(base, address + 11); }
  void set_x_origin_lo(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t x_origin_lo_address() const { return address + 11; }
  uint16_t y_origin() const { return guest::LoadU16(base, address + 12); }
  void set_y_origin(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t y_origin_address() const { return address + 12; }
  uint8_t y_origin_hi() const { return guest::LoadU8(base, address + 12); }
  void set_y_origin_hi(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t y_origin_hi_address() const { return address + 12; }
  uint8_t y_origin_lo() const { return guest::LoadU8(base, address + 13); }
  void set_y_origin_lo(uint8_t v) const { guest::StoreU8(base, address + 13, v); }
  uint32_t y_origin_lo_address() const { return address + 13; }
  uint16_t width() const { return guest::LoadU16(base, address + 14); }
  void set_width(uint16_t v) const { guest::StoreU16(base, address + 14, v); }
  uint32_t width_address() const { return address + 14; }
  uint8_t width_hi() const { return guest::LoadU8(base, address + 14); }
  void set_width_hi(uint8_t v) const { guest::StoreU8(base, address + 14, v); }
  uint32_t width_hi_address() const { return address + 14; }
  uint8_t width_lo() const { return guest::LoadU8(base, address + 15); }
  void set_width_lo(uint8_t v) const { guest::StoreU8(base, address + 15, v); }
  uint32_t width_lo_address() const { return address + 15; }
  uint16_t height() const { return guest::LoadU16(base, address + 16); }
  void set_height(uint16_t v) const { guest::StoreU16(base, address + 16, v); }
  uint32_t height_address() const { return address + 16; }
  uint8_t height_hi() const { return guest::LoadU8(base, address + 16); }
  void set_height_hi(uint8_t v) const { guest::StoreU8(base, address + 16, v); }
  uint32_t height_hi_address() const { return address + 16; }
  uint8_t height_lo() const { return guest::LoadU8(base, address + 17); }
  void set_height_lo(uint8_t v) const { guest::StoreU8(base, address + 17, v); }
  uint32_t height_lo_address() const { return address + 17; }
  uint8_t bits_per_pixel() const { return guest::LoadU8(base, address + 18); }
  void set_bits_per_pixel(uint8_t v) const { guest::StoreU8(base, address + 18, v); }
  uint32_t bits_per_pixel_address() const { return address + 18; }
  uint8_t descriptor() const { return guest::LoadU8(base, address + 19); }
  void set_descriptor(uint8_t v) const { guest::StoreU8(base, address + 19, v); }
  uint32_t descriptor_address() const { return address + 19; }
  // the image's
  uint32_t image_file() const { return guest::LoadU32(base, address + 4); }
  void set_image_file(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t image_file_address() const { return address + 4; }
  uint32_t image_header() const { return guest::LoadU32(base, address + 8); }
  void set_image_header(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t image_header_address() const { return address + 8; }
  uint32_t image_pixels() const { return guest::LoadU32(base, address + 12); }
  void set_image_pixels(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t image_pixels_address() const { return address + 12; }
  uint32_t image_size() const { return guest::LoadU32(base, address + 16); }
  void set_image_size(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t image_size_address() const { return address + 16; }
  uint32_t image_width() const { return guest::LoadU32(base, address + 20); }
  void set_image_width(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t image_width_address() const { return address + 20; }
  uint32_t image_height() const { return guest::LoadU32(base, address + 24); }
  void set_image_height(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t image_height_address() const { return address + 24; }
};

// embedded; measure: SoftRaster
struct DecalImage : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // ARGB words
  uint32_t pixels() const { return guest::LoadU32(base, address + 0); }
  void set_pixels(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t pixels_address() const { return address + 0; }
  uint32_t width() const { return guest::LoadU32(base, address + 4); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t width_address() const { return address + 4; }
  uint32_t height() const { return guest::LoadU32(base, address + 8); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t height_address() const { return address + 8; }
  // bytes
  uint32_t pitch() const { return guest::LoadU32(base, address + 12); }
  void set_pitch(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t pitch_address() const { return address + 12; }
};

// embedded; measure: SoftRaster::DrawSpan.r4
struct DecalRegion : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t width() const { return guest::LoadU32(base, address + 0); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t width_address() const { return address + 0; }
  uint32_t height() const { return guest::LoadU32(base, address + 4); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t height_address() const { return address + 4; }
  // the sheet's, bytes
  uint32_t pitch() const { return guest::LoadU32(base, address + 8); }
  void set_pitch(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t pitch_address() const { return address + 8; }
};

// embedded; measure: SoftRaster::BlendPixel.r4
struct DecalCanvas : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t first_pixel() const { return guest::LoadU32(base, address + 0); }
  void set_first_pixel(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t first_pixel_address() const { return address + 0; }
};

// embedded; measure: SoftRaster::DrawSpan.r8, SoftRaster::DrawSpan.r9, SoftRaster::BlendPixel.r8, Vinyl::DecalCorner.r4, Vinyl::DecalCorner.r5, Vinyl::DecalCorner.r6, Vinyl::DecalCorner.r8
struct DecalVertex : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  uint64_t xy() const { return guest::LoadU64(base, address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  uint64_t zw() const { return guest::LoadU64(base, address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
};

// embedded; measure: SoftRaster::DrawTriangle.r5
struct DecalTriangle : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  float y0() const { return guest::LoadFloat(base, address + 4); }
  void set_y0(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y0_address() const { return address + 4; }
  float y1() const { return guest::LoadFloat(base, address + 20); }
  void set_y1(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t y1_address() const { return address + 20; }
  float y2() const { return guest::LoadFloat(base, address + 36); }
  void set_y2(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t y2_address() const { return address + 36; }
};

// embedded; measure: CarScene::SetUp.r4, CarScene@+4
struct CarSceneSetUpRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // the scene's +4
  uint32_t model_set() const { return guest::LoadU32(base, address + 0); }
  void set_model_set(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t model_set_address() const { return address + 0; }
  // pool 0's count
  uint32_t car_models() const { return guest::LoadU32(base, address + 4); }
  void set_car_models(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t car_models_address() const { return address + 4; }
  // pool 1's
  uint32_t parts() const { return guest::LoadU32(base, address + 8); }
  void set_parts(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t parts_address() const { return address + 8; }
  // pool 3's is this + cockpits + traffic
  uint32_t interiors() const { return guest::LoadU32(base, address + 12); }
  void set_interiors(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t interiors_address() const { return address + 12; }
  // pool 2's
  uint32_t cockpits() const { return guest::LoadU32(base, address + 16); }
  void set_cockpits(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t cockpits_address() const { return address + 16; }
  // pool 4's
  uint32_t pool4_objects() const { return guest::LoadU32(base, address + 20); }
  void set_pool4_objects(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t pool4_objects_address() const { return address + 20; }
  // pool 5's is this + traffic + models
  uint32_t wheels() const { return guest::LoadU32(base, address + 24); }
  void set_wheels(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t wheels_address() const { return address + 24; }
  // pool 7's is this + traffic (probably the scene models besides traffic's)
  uint32_t models() const { return guest::LoadU32(base, address + 28); }
  void set_models(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t models_address() const { return address + 28; }
  // pool 6's
  uint32_t traffic() const { return guest::LoadU32(base, address + 32); }
  void set_traffic(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t traffic_address() const { return address + 32; }
  // 0: the archive's shared load buffer
  uint32_t load_buffer() const { return guest::LoadU32(base, address + 36); }
  void set_load_buffer(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t load_buffer_address() const { return address + 36; }
  uint32_t load_buffer_size() const { return guest::LoadU32(base, address + 40); }
  void set_load_buffer_size(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t load_buffer_size_address() const { return address + 40; }
};

// allocated; measure: Wheels
#pragma pack(push, 1)
struct CarWheelsData {
  uint32_t vtable = 0;  // +0
  uint8_t pad_4[12] = {};
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint32_t car_model = 0;  // +24
  uint32_t type = 0;  // +28
  uint8_t pad_32[8] = {};
  uint32_t front_tire_texture = 0;  // +40
  uint32_t front_wheel = 0;  // +44
  float unknown_48 = 0;  // +48
  float unknown_52 = 0;  // +52
  uint32_t rear_tire_texture = 0;  // +56
  uint32_t rear_wheel = 0;  // +60
  float unknown_64 = 0;  // +64
  float unknown_68 = 0;  // +68
  uint8_t pad_72[44] = {};
  uint32_t caliper_texture = 0;  // +116
  uint8_t pad_120[24] = {};
  uint32_t front_tire_aftermarket = 0;  // +144
  uint32_t rear_tire_aftermarket = 0;  // +148
  uint8_t pad_152[40] = {};
  uint8_t tire_slots[48] = {};  // +192, guest order
  uint8_t pad_240[48] = {};
  uint32_t front_tire_model_provider = 0;  // +288
  uint8_t pad_292[460] = {};
  uint32_t rear_tire_model_provider = 0;  // +752
  uint8_t pad_756[412] = {};
  uint32_t front_rim_id = 0;  // +1168
  uint8_t pad_1172[20] = {};
  uint32_t rear_rim_id = 0;  // +1192
  uint8_t pad_1196[68] = {};
  uint32_t front_rim_model_provider = 0;  // +1264
  uint8_t pad_1268[460] = {};
  uint32_t rear_rim_model_provider = 0;  // +1728
  uint8_t pad_1732[412] = {};
  uint32_t front_xpr_id = 0;  // +2144
  uint8_t pad_2148[20] = {};
  uint32_t rear_xpr_id = 0;  // +2168
  uint8_t pad_2172[20] = {};
  uint8_t xpr_objects[48] = {};  // +2192, guest order
  uint8_t texture_providers[40] = {};  // +2240, guest order
  uint8_t pad_2280[44] = {};
  uint32_t rear_caliper_state = 0;  // +2324
  uint8_t pad_2328[56] = {};
  uint32_t front_caliper_model_provider = 0;  // +2384
  uint8_t pad_2388[460] = {};
  uint32_t rear_caliper_model_provider = 0;  // +2848
  uint8_t pad_2852[412] = {};
  uint32_t traffic = 0;  // +3264
  uint32_t first_set = 0;  // +3268
  uint8_t pad_3272[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(CarWheelsData) == 3280);
inline constexpr memory::LayoutField kCarWheelsFields[] = {{0, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {116, 4}, {144, 4}, {148, 4}, {288, 4}, {752, 4}, {1168, 4}, {1192, 4}, {1264, 4}, {1728, 4}, {2144, 4}, {2168, 4}, {2324, 4}, {2384, 4}, {2848, 4}, {3264, 4}, {3268, 4}};
inline constexpr memory::Layout kCarWheelsLayout = {kCarWheelsFields, 29};

struct CarWheels : View {
  using View::View;
  static constexpr uint32_t kSize = 3280;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82005100;
  CarWheelsData* data() const { return static_cast<CarWheelsData*>(memory::HandleObject(address)); }
  static CarWheels New(uint8_t* base) {
    return CarWheels(base, memory::MakeHandle(new CarWheelsData(), sizeof(CarWheelsData), "CarWheels", &kCarWheelsLayout));
  }
  template <uint32_t Stride = kSize>
  static CarWheels NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CarWheelsData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarWheelsData*>(::operator new(sizeof(CarWheelsData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarWheelsData();
    return CarWheels(base, memory::MakeHandle(p, sizeof(CarWheelsData) * count, "CarWheels", &kCarWheelsLayout, sizeof(CarWheelsData)));
  }
  CarWheels at(uint32_t i) const { return CarWheels(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarWheelsData* direct() const { return static_cast<CarWheelsData*>(memory::HandleOwn(address, &kCarWheelsLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarWheelsLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarWheelsLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82005100 (the pool node's 0x82004F90 while it's built or torn down)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the pool object's: 3 in use
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // its index in the pool
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t car_model_address() const { return address + 24; }
  uint32_t car_model_bits() const { if (auto* d = direct()) return d->car_model; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_car_model_bits(uint32_t v) const { if (auto* d = direct()) { d->car_model = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  template <typename T = CarModel>
  T car_model() const { return T(base, car_model_bits()); }
  void set_car_model(const View& w) const { set_car_model_bits(w.address); }
  void set_car_model(uint32_t v) const { set_car_model_bits(v); }
  // a copy of the 156-byte set-up record
  uint32_t setup_address() const { return address + 28; }
  uint8_t* setup() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 28 : guest::Pointer(base, address + 28); }
  // the record's +0: 3 traffic
  uint32_t type_address() const { return address + 28; }
  uint32_t type_bits() const { if (auto* d = direct()) return d->type; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_type_bits(uint32_t v) const { if (auto* d = direct()) { d->type = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t type() const { if (auto* d = direct()) return d->type; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_type(uint32_t v) const { if (auto* d = direct()) { d->type = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  // the record's +12: probably the front tire's texture (Wheels::BindTextures)
  uint32_t front_tire_texture_address() const { return address + 40; }
  uint32_t front_tire_texture_bits() const { if (auto* d = direct()) return d->front_tire_texture; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_front_tire_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->front_tire_texture = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  uint32_t front_tire_texture() const { if (auto* d = direct()) return d->front_tire_texture; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_front_tire_texture(uint32_t v) const { if (auto* d = direct()) { d->front_tire_texture = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  // the record's +16: -1 stock, else aftermarket wheel 36 + n
  uint32_t front_wheel_address() const { return address + 44; }
  uint32_t front_wheel_bits() const { if (auto* d = direct()) return d->front_wheel; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_front_wheel_bits(uint32_t v) const { if (auto* d = direct()) { d->front_wheel = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  uint32_t front_wheel() const { if (auto* d = direct()) return d->front_wheel; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_front_wheel(uint32_t v) const { if (auto* d = direct()) { d->front_wheel = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  // the record's +20
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_48_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_48); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_unknown_48_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_48 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  float unknown_48() const { return std::bit_cast<float>(unknown_48_bits()); }
  void set_unknown_48(float v) const { set_unknown_48_bits(std::bit_cast<uint32_t>(v)); }
  // the record's +24
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_52_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_52); return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_unknown_52_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_52 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  float unknown_52() const { return std::bit_cast<float>(unknown_52_bits()); }
  void set_unknown_52(float v) const { set_unknown_52_bits(std::bit_cast<uint32_t>(v)); }
  // the record's +28
  uint32_t rear_tire_texture_address() const { return address + 56; }
  uint32_t rear_tire_texture_bits() const { if (auto* d = direct()) return d->rear_tire_texture; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(base, address + 56))); }
  void set_rear_tire_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_tire_texture = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(base, address + 56, v); }
  uint32_t rear_tire_texture() const { if (auto* d = direct()) return d->rear_tire_texture; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(base, address + 56))); }
  void set_rear_tire_texture(uint32_t v) const { if (auto* d = direct()) { d->rear_tire_texture = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(base, address + 56, v); }
  // the record's +32
  uint32_t rear_wheel_address() const { return address + 60; }
  uint32_t rear_wheel_bits() const { if (auto* d = direct()) return d->rear_wheel; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_rear_wheel_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_wheel = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  uint32_t rear_wheel() const { if (auto* d = direct()) return d->rear_wheel; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_rear_wheel(uint32_t v) const { if (auto* d = direct()) { d->rear_wheel = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  // the record's +36
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_64_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_64); return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(base, address + 64))); }
  void set_unknown_64_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_64 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(base, address + 64, v); }
  float unknown_64() const { return std::bit_cast<float>(unknown_64_bits()); }
  void set_unknown_64(float v) const { set_unknown_64_bits(std::bit_cast<uint32_t>(v)); }
  // the record's +40
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_68_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_68); return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(base, address + 68))); }
  void set_unknown_68_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_68 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(base, address + 68, v); }
  float unknown_68() const { return std::bit_cast<float>(unknown_68_bits()); }
  void set_unknown_68(float v) const { set_unknown_68_bits(std::bit_cast<uint32_t>(v)); }
  // the record's +88: the calipers' texture (Wheels::SetCaliperTexture)
  uint32_t caliper_texture_address() const { return address + 116; }
  uint32_t caliper_texture_bits() const { if (auto* d = direct()) return d->caliper_texture; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(base, address + 116))); }
  void set_caliper_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->caliper_texture = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(base, address + 116, v); }
  uint32_t caliper_texture() const { if (auto* d = direct()) return d->caliper_texture; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(base, address + 116))); }
  void set_caliper_texture(uint32_t v) const { if (auto* d = direct()) { d->caliper_texture = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(base, address + 116, v); }
  // the record's +116: 0 tire asset 582, else 584
  uint32_t front_tire_aftermarket_address() const { return address + 144; }
  uint32_t front_tire_aftermarket_bits() const { if (auto* d = direct()) return d->front_tire_aftermarket; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(base, address + 144))); }
  void set_front_tire_aftermarket_bits(uint32_t v) const { if (auto* d = direct()) { d->front_tire_aftermarket = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(base, address + 144, v); }
  uint32_t front_tire_aftermarket() const { if (auto* d = direct()) return d->front_tire_aftermarket; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(base, address + 144))); }
  void set_front_tire_aftermarket(uint32_t v) const { if (auto* d = direct()) { d->front_tire_aftermarket = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(base, address + 144, v); }
  uint32_t rear_tire_aftermarket_address() const { return address + 148; }
  uint32_t rear_tire_aftermarket_bits() const { if (auto* d = direct()) return d->rear_tire_aftermarket; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(base, address + 148))); }
  void set_rear_tire_aftermarket_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_tire_aftermarket = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(base, address + 148, v); }
  uint32_t rear_tire_aftermarket() const { if (auto* d = direct()) return d->rear_tire_aftermarket; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(base, address + 148))); }
  void set_rear_tire_aftermarket(uint32_t v) const { if (auto* d = direct()) { d->rear_tire_aftermarket = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(base, address + 148, v); }
  // the tires' LoadSlots (+192, +216)
  uint32_t tire_slots_address() const { return address + 192; }
  uint8_t* tire_slots() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 192 : guest::Pointer(base, address + 192); }
  uint32_t front_tire_model_address() const { return address + 240; }
  uint8_t* front_tire_model() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 240 : guest::Pointer(base, address + 240); }
  // the model's +48: the scene + 208
  uint32_t front_tire_model_provider_address() const { return address + 288; }
  uint32_t front_tire_model_provider_bits() const { if (auto* d = direct()) return d->front_tire_model_provider; return uint32_t((memory::IsHandle(address + 288) ? uint32_t(memory::OwnerLoad(address + 288, 4)) : guest::LoadU32(base, address + 288))); }
  void set_front_tire_model_provider_bits(uint32_t v) const { if (auto* d = direct()) { d->front_tire_model_provider = v; return; } if (memory::IsHandle(address + 288)) memory::OwnerStore(address + 288, 4, v); else guest::StoreU32(base, address + 288, v); }
  uint32_t front_tire_model_provider() const { if (auto* d = direct()) return d->front_tire_model_provider; return uint32_t((memory::IsHandle(address + 288) ? uint32_t(memory::OwnerLoad(address + 288, 4)) : guest::LoadU32(base, address + 288))); }
  void set_front_tire_model_provider(uint32_t v) const { if (auto* d = direct()) { d->front_tire_model_provider = v; return; } if (memory::IsHandle(address + 288)) memory::OwnerStore(address + 288, 4, v); else guest::StoreU32(base, address + 288, v); }
  uint32_t rear_tire_model_address() const { return address + 704; }
  uint8_t* rear_tire_model() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 704 : guest::Pointer(base, address + 704); }
  uint32_t rear_tire_model_provider_address() const { return address + 752; }
  uint32_t rear_tire_model_provider_bits() const { if (auto* d = direct()) return d->rear_tire_model_provider; return uint32_t((memory::IsHandle(address + 752) ? uint32_t(memory::OwnerLoad(address + 752, 4)) : guest::LoadU32(base, address + 752))); }
  void set_rear_tire_model_provider_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_tire_model_provider = v; return; } if (memory::IsHandle(address + 752)) memory::OwnerStore(address + 752, 4, v); else guest::StoreU32(base, address + 752, v); }
  uint32_t rear_tire_model_provider() const { if (auto* d = direct()) return d->rear_tire_model_provider; return uint32_t((memory::IsHandle(address + 752) ? uint32_t(memory::OwnerLoad(address + 752, 4)) : guest::LoadU32(base, address + 752))); }
  void set_rear_tire_model_provider(uint32_t v) const { if (auto* d = direct()) { d->rear_tire_model_provider = v; return; } if (memory::IsHandle(address + 752)) memory::OwnerStore(address + 752, 4, v); else guest::StoreU32(base, address + 752, v); }
  // the rims' or brakes' LoadSlots
  uint32_t rim_slots_address() const { return address + 1168; }
  uint8_t* rim_slots() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 1168 : guest::Pointer(base, address + 1168); }
  uint32_t front_rim_id_address() const { return address + 1168; }
  uint32_t front_rim_id_bits() const { if (auto* d = direct()) return d->front_rim_id; return uint32_t((memory::IsHandle(address + 1168) ? uint32_t(memory::OwnerLoad(address + 1168, 4)) : guest::LoadU32(base, address + 1168))); }
  void set_front_rim_id_bits(uint32_t v) const { if (auto* d = direct()) { d->front_rim_id = v; return; } if (memory::IsHandle(address + 1168)) memory::OwnerStore(address + 1168, 4, v); else guest::StoreU32(base, address + 1168, v); }
  uint32_t front_rim_id() const { if (auto* d = direct()) return d->front_rim_id; return uint32_t((memory::IsHandle(address + 1168) ? uint32_t(memory::OwnerLoad(address + 1168, 4)) : guest::LoadU32(base, address + 1168))); }
  void set_front_rim_id(uint32_t v) const { if (auto* d = direct()) { d->front_rim_id = v; return; } if (memory::IsHandle(address + 1168)) memory::OwnerStore(address + 1168, 4, v); else guest::StoreU32(base, address + 1168, v); }
  uint32_t rear_rim_id_address() const { return address + 1192; }
  uint32_t rear_rim_id_bits() const { if (auto* d = direct()) return d->rear_rim_id; return uint32_t((memory::IsHandle(address + 1192) ? uint32_t(memory::OwnerLoad(address + 1192, 4)) : guest::LoadU32(base, address + 1192))); }
  void set_rear_rim_id_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_rim_id = v; return; } if (memory::IsHandle(address + 1192)) memory::OwnerStore(address + 1192, 4, v); else guest::StoreU32(base, address + 1192, v); }
  uint32_t rear_rim_id() const { if (auto* d = direct()) return d->rear_rim_id; return uint32_t((memory::IsHandle(address + 1192) ? uint32_t(memory::OwnerLoad(address + 1192, 4)) : guest::LoadU32(base, address + 1192))); }
  void set_rear_rim_id(uint32_t v) const { if (auto* d = direct()) { d->rear_rim_id = v; return; } if (memory::IsHandle(address + 1192)) memory::OwnerStore(address + 1192, 4, v); else guest::StoreU32(base, address + 1192, v); }
  uint32_t front_rim_model_address() const { return address + 1216; }
  uint8_t* front_rim_model() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 1216 : guest::Pointer(base, address + 1216); }
  // the wheels' own provider +2240
  uint32_t front_rim_model_provider_address() const { return address + 1264; }
  uint32_t front_rim_model_provider_bits() const { if (auto* d = direct()) return d->front_rim_model_provider; return uint32_t((memory::IsHandle(address + 1264) ? uint32_t(memory::OwnerLoad(address + 1264, 4)) : guest::LoadU32(base, address + 1264))); }
  void set_front_rim_model_provider_bits(uint32_t v) const { if (auto* d = direct()) { d->front_rim_model_provider = v; return; } if (memory::IsHandle(address + 1264)) memory::OwnerStore(address + 1264, 4, v); else guest::StoreU32(base, address + 1264, v); }
  uint32_t front_rim_model_provider() const { if (auto* d = direct()) return d->front_rim_model_provider; return uint32_t((memory::IsHandle(address + 1264) ? uint32_t(memory::OwnerLoad(address + 1264, 4)) : guest::LoadU32(base, address + 1264))); }
  void set_front_rim_model_provider(uint32_t v) const { if (auto* d = direct()) { d->front_rim_model_provider = v; return; } if (memory::IsHandle(address + 1264)) memory::OwnerStore(address + 1264, 4, v); else guest::StoreU32(base, address + 1264, v); }
  uint32_t rear_rim_model_address() const { return address + 1680; }
  uint8_t* rear_rim_model() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 1680 : guest::Pointer(base, address + 1680); }
  // +2260
  uint32_t rear_rim_model_provider_address() const { return address + 1728; }
  uint32_t rear_rim_model_provider_bits() const { if (auto* d = direct()) return d->rear_rim_model_provider; return uint32_t((memory::IsHandle(address + 1728) ? uint32_t(memory::OwnerLoad(address + 1728, 4)) : guest::LoadU32(base, address + 1728))); }
  void set_rear_rim_model_provider_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_rim_model_provider = v; return; } if (memory::IsHandle(address + 1728)) memory::OwnerStore(address + 1728, 4, v); else guest::StoreU32(base, address + 1728, v); }
  uint32_t rear_rim_model_provider() const { if (auto* d = direct()) return d->rear_rim_model_provider; return uint32_t((memory::IsHandle(address + 1728) ? uint32_t(memory::OwnerLoad(address + 1728, 4)) : guest::LoadU32(base, address + 1728))); }
  void set_rear_rim_model_provider(uint32_t v) const { if (auto* d = direct()) { d->rear_rim_model_provider = v; return; } if (memory::IsHandle(address + 1728)) memory::OwnerStore(address + 1728, 4, v); else guest::StoreU32(base, address + 1728, v); }
  // the wheels' XPR2 LoadSlots
  uint32_t xpr_slots_address() const { return address + 2144; }
  uint8_t* xpr_slots() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 2144 : guest::Pointer(base, address + 2144); }
  // 587 + 3 x block, or 695 + 3n
  uint32_t front_xpr_id_address() const { return address + 2144; }
  uint32_t front_xpr_id_bits() const { if (auto* d = direct()) return d->front_xpr_id; return uint32_t((memory::IsHandle(address + 2144) ? uint32_t(memory::OwnerLoad(address + 2144, 4)) : guest::LoadU32(base, address + 2144))); }
  void set_front_xpr_id_bits(uint32_t v) const { if (auto* d = direct()) { d->front_xpr_id = v; return; } if (memory::IsHandle(address + 2144)) memory::OwnerStore(address + 2144, 4, v); else guest::StoreU32(base, address + 2144, v); }
  uint32_t front_xpr_id() const { if (auto* d = direct()) return d->front_xpr_id; return uint32_t((memory::IsHandle(address + 2144) ? uint32_t(memory::OwnerLoad(address + 2144, 4)) : guest::LoadU32(base, address + 2144))); }
  void set_front_xpr_id(uint32_t v) const { if (auto* d = direct()) { d->front_xpr_id = v; return; } if (memory::IsHandle(address + 2144)) memory::OwnerStore(address + 2144, 4, v); else guest::StoreU32(base, address + 2144, v); }
  uint32_t rear_xpr_id_address() const { return address + 2168; }
  uint32_t rear_xpr_id_bits() const { if (auto* d = direct()) return d->rear_xpr_id; return uint32_t((memory::IsHandle(address + 2168) ? uint32_t(memory::OwnerLoad(address + 2168, 4)) : guest::LoadU32(base, address + 2168))); }
  void set_rear_xpr_id_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_xpr_id = v; return; } if (memory::IsHandle(address + 2168)) memory::OwnerStore(address + 2168, 4, v); else guest::StoreU32(base, address + 2168, v); }
  uint32_t rear_xpr_id() const { if (auto* d = direct()) return d->rear_xpr_id; return uint32_t((memory::IsHandle(address + 2168) ? uint32_t(memory::OwnerLoad(address + 2168, 4)) : guest::LoadU32(base, address + 2168))); }
  void set_rear_xpr_id(uint32_t v) const { if (auto* d = direct()) { d->rear_xpr_id = v; return; } if (memory::IsHandle(address + 2168)) memory::OwnerStore(address + 2168, 4, v); else guest::StoreU32(base, address + 2168, v); }
  uint32_t xpr_objects_address() const { return address + 2192; }
  uint8_t* xpr_objects() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 2192 : guest::Pointer(base, address + 2192); }
  // two TextureProviders (20 bytes each)
  uint32_t texture_providers_address() const { return address + 2240; }
  uint8_t* texture_providers() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 2240 : guest::Pointer(base, address + 2240); }
  // the calipers' LoadSlots (asset 887)
  uint32_t caliper_slots_address() const { return address + 2280; }
  uint8_t* caliper_slots() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 2280 : guest::Pointer(base, address + 2280); }
  // the second slot's state: 2 loading
  uint32_t rear_caliper_state_address() const { return address + 2324; }
  uint32_t rear_caliper_state_bits() const { if (auto* d = direct()) return d->rear_caliper_state; return uint32_t((memory::IsHandle(address + 2324) ? uint32_t(memory::OwnerLoad(address + 2324, 4)) : guest::LoadU32(base, address + 2324))); }
  void set_rear_caliper_state_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_caliper_state = v; return; } if (memory::IsHandle(address + 2324)) memory::OwnerStore(address + 2324, 4, v); else guest::StoreU32(base, address + 2324, v); }
  uint32_t rear_caliper_state() const { if (auto* d = direct()) return d->rear_caliper_state; return uint32_t((memory::IsHandle(address + 2324) ? uint32_t(memory::OwnerLoad(address + 2324, 4)) : guest::LoadU32(base, address + 2324))); }
  void set_rear_caliper_state(uint32_t v) const { if (auto* d = direct()) { d->rear_caliper_state = v; return; } if (memory::IsHandle(address + 2324)) memory::OwnerStore(address + 2324, 4, v); else guest::StoreU32(base, address + 2324, v); }
  uint32_t front_caliper_model_address() const { return address + 2336; }
  uint8_t* front_caliper_model() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 2336 : guest::Pointer(base, address + 2336); }
  // the scene + 296
  uint32_t front_caliper_model_provider_address() const { return address + 2384; }
  uint32_t front_caliper_model_provider_bits() const { if (auto* d = direct()) return d->front_caliper_model_provider; return uint32_t((memory::IsHandle(address + 2384) ? uint32_t(memory::OwnerLoad(address + 2384, 4)) : guest::LoadU32(base, address + 2384))); }
  void set_front_caliper_model_provider_bits(uint32_t v) const { if (auto* d = direct()) { d->front_caliper_model_provider = v; return; } if (memory::IsHandle(address + 2384)) memory::OwnerStore(address + 2384, 4, v); else guest::StoreU32(base, address + 2384, v); }
  uint32_t front_caliper_model_provider() const { if (auto* d = direct()) return d->front_caliper_model_provider; return uint32_t((memory::IsHandle(address + 2384) ? uint32_t(memory::OwnerLoad(address + 2384, 4)) : guest::LoadU32(base, address + 2384))); }
  void set_front_caliper_model_provider(uint32_t v) const { if (auto* d = direct()) { d->front_caliper_model_provider = v; return; } if (memory::IsHandle(address + 2384)) memory::OwnerStore(address + 2384, 4, v); else guest::StoreU32(base, address + 2384, v); }
  uint32_t rear_caliper_model_address() const { return address + 2800; }
  uint8_t* rear_caliper_model() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 2800 : guest::Pointer(base, address + 2800); }
  uint32_t rear_caliper_model_provider_address() const { return address + 2848; }
  uint32_t rear_caliper_model_provider_bits() const { if (auto* d = direct()) return d->rear_caliper_model_provider; return uint32_t((memory::IsHandle(address + 2848) ? uint32_t(memory::OwnerLoad(address + 2848, 4)) : guest::LoadU32(base, address + 2848))); }
  void set_rear_caliper_model_provider_bits(uint32_t v) const { if (auto* d = direct()) { d->rear_caliper_model_provider = v; return; } if (memory::IsHandle(address + 2848)) memory::OwnerStore(address + 2848, 4, v); else guest::StoreU32(base, address + 2848, v); }
  uint32_t rear_caliper_model_provider() const { if (auto* d = direct()) return d->rear_caliper_model_provider; return uint32_t((memory::IsHandle(address + 2848) ? uint32_t(memory::OwnerLoad(address + 2848, 4)) : guest::LoadU32(base, address + 2848))); }
  void set_rear_caliper_model_provider(uint32_t v) const { if (auto* d = direct()) { d->rear_caliper_model_provider = v; return; } if (memory::IsHandle(address + 2848)) memory::OwnerStore(address + 2848, 4, v); else guest::StoreU32(base, address + 2848, v); }
  // the traffic record (+0 its index)
  uint32_t traffic_address() const { return address + 3264; }
  uint32_t traffic_bits() const { if (auto* d = direct()) return d->traffic; return uint32_t((memory::IsHandle(address + 3264) ? uint32_t(memory::OwnerLoad(address + 3264, 4)) : guest::LoadU32(base, address + 3264))); }
  void set_traffic_bits(uint32_t v) const { if (auto* d = direct()) { d->traffic = v; return; } if (memory::IsHandle(address + 3264)) memory::OwnerStore(address + 3264, 4, v); else guest::StoreU32(base, address + 3264, v); }
  template <typename T = CarModelsTrafficIndex>
  T traffic() const { return T(base, traffic_bits()); }
  void set_traffic(const View& w) const { set_traffic_bits(w.address); }
  void set_traffic(uint32_t v) const { set_traffic_bits(v); }
  // the first set of scene pools ([0x826C2E40]): its +8 picks rims (0) or brakes
  uint32_t first_set_address() const { return address + 3268; }
  uint32_t first_set_bits() const { if (auto* d = direct()) return d->first_set; return uint32_t((memory::IsHandle(address + 3268) ? uint32_t(memory::OwnerLoad(address + 3268, 4)) : guest::LoadU32(base, address + 3268))); }
  void set_first_set_bits(uint32_t v) const { if (auto* d = direct()) { d->first_set = v; return; } if (memory::IsHandle(address + 3268)) memory::OwnerStore(address + 3268, 4, v); else guest::StoreU32(base, address + 3268, v); }
  template <typename T = ScenePools>
  T first_set() const { return T(base, first_set_bits()); }
  void set_first_set(const View& w) const { set_first_set_bits(w.address); }
  void set_first_set(uint32_t v) const { set_first_set_bits(v); }
};

// allocated; measure: ScenePools.+52, new<140>@ScenePools::MakeA.+52
struct CarModelsDeferredRelease : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // a texture's header (the car model's +6684 the sheet, or +6700 the plate's first)
  uint32_t first() const { return guest::LoadU32(base, address + 0); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
  // its pixels
  uint32_t first_data() const { return guest::LoadU32(base, address + 4); }
  void set_first_data(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_data_address() const { return address + 4; }
  // the car model's +6692 or +6708 (the plate's second texture's header)
  uint32_t second() const { return guest::LoadU32(base, address + 8); }
  void set_second(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t second_address() const { return address + 8; }
  // +6696 or +6712
  uint32_t second_data() const { return guest::LoadU32(base, address + 12); }
  void set_second_data(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t second_data_address() const { return address + 12; }
  // 0 free, 2 queued
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
};

// embedded; measure: Vinyl@+6684
struct CarModelAt6684 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: Vinyl@+6688
struct CarModelAt6688 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: TgaReader@+56.+8
struct TgaHeaderOrImagePart : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint8_t unknown_0() const { return guest::LoadU8(base, address + 0); }
  void set_unknown_0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint8_t unknown_1() const { return guest::LoadU8(base, address + 1); }
  void set_unknown_1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t unknown_1_address() const { return address + 1; }
  uint8_t unknown_2() const { return guest::LoadU8(base, address + 2); }
  void set_unknown_2(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  uint8_t unknown_4() const { return guest::LoadU8(base, address + 4); }
  void set_unknown_4(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint16_t unknown_4_2i() const { return guest::LoadU16(base, address + 4); }
  void set_unknown_4_2i(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t unknown_4_2i_address() const { return address + 4; }
  uint8_t unknown_5() const { return guest::LoadU8(base, address + 5); }
  void set_unknown_5(uint8_t v) const { guest::StoreU8(base, address + 5, v); }
  uint32_t unknown_5_address() const { return address + 5; }
  uint8_t unknown_6() const { return guest::LoadU8(base, address + 6); }
  void set_unknown_6(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t unknown_6_address() const { return address + 6; }
  uint16_t unknown_6_2i() const { return guest::LoadU16(base, address + 6); }
  void set_unknown_6_2i(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t unknown_6_2i_address() const { return address + 6; }
  uint8_t unknown_7() const { return guest::LoadU8(base, address + 7); }
  void set_unknown_7(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t unknown_7_address() const { return address + 7; }
  uint8_t unknown_8() const { return guest::LoadU8(base, address + 8); }
  void set_unknown_8(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint8_t unknown_10() const { return guest::LoadU8(base, address + 10); }
  void set_unknown_10(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t unknown_10_address() const { return address + 10; }
  uint16_t unknown_10_2i() const { return guest::LoadU16(base, address + 10); }
  void set_unknown_10_2i(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t unknown_10_2i_address() const { return address + 10; }
  uint8_t unknown_11() const { return guest::LoadU8(base, address + 11); }
  void set_unknown_11(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t unknown_11_address() const { return address + 11; }
  uint8_t unknown_12() const { return guest::LoadU8(base, address + 12); }
  void set_unknown_12(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint16_t unknown_12_2i() const { return guest::LoadU16(base, address + 12); }
  void set_unknown_12_2i(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t unknown_12_2i_address() const { return address + 12; }
  uint8_t unknown_13() const { return guest::LoadU8(base, address + 13); }
  void set_unknown_13(uint8_t v) const { guest::StoreU8(base, address + 13, v); }
  uint32_t unknown_13_address() const { return address + 13; }
  uint8_t unknown_14() const { return guest::LoadU8(base, address + 14); }
  void set_unknown_14(uint8_t v) const { guest::StoreU8(base, address + 14, v); }
  uint32_t unknown_14_address() const { return address + 14; }
  uint16_t unknown_14_2i() const { return guest::LoadU16(base, address + 14); }
  void set_unknown_14_2i(uint16_t v) const { guest::StoreU16(base, address + 14, v); }
  uint32_t unknown_14_2i_address() const { return address + 14; }
  uint8_t unknown_15() const { return guest::LoadU8(base, address + 15); }
  void set_unknown_15(uint8_t v) const { guest::StoreU8(base, address + 15, v); }
  uint32_t unknown_15_address() const { return address + 15; }
  uint8_t unknown_16() const { return guest::LoadU8(base, address + 16); }
  void set_unknown_16(uint8_t v) const { guest::StoreU8(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint16_t unknown_16_2i() const { return guest::LoadU16(base, address + 16); }
  void set_unknown_16_2i(uint16_t v) const { guest::StoreU16(base, address + 16, v); }
  uint32_t unknown_16_2i_address() const { return address + 16; }
  uint8_t unknown_17() const { return guest::LoadU8(base, address + 17); }
  void set_unknown_17(uint8_t v) const { guest::StoreU8(base, address + 17, v); }
  uint32_t unknown_17_address() const { return address + 17; }
  uint8_t unknown_18() const { return guest::LoadU8(base, address + 18); }
  void set_unknown_18(uint8_t v) const { guest::StoreU8(base, address + 18, v); }
  uint32_t unknown_18_address() const { return address + 18; }
  uint8_t unknown_19() const { return guest::LoadU8(base, address + 19); }
  void set_unknown_19(uint8_t v) const { guest::StoreU8(base, address + 19, v); }
  uint32_t unknown_19_address() const { return address + 19; }
};

// The image words docs/functions/car-models.md's code uses.
struct CarModelsGlobals {
  // the first set of scene pools (ScenePools, scene-loaders.md)
  static View first_set(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826C2E40u)); }
  static void set_first_set(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E40u, v); }
  static constexpr uint32_t first_set_address() { return 0x826C2E40u; }
  // the car scene (its second set): +5056 the shared load buffer, +5060 its size
  template <typename T = CarScene>
  static T scene(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E44u)); }
  static void set_scene(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E44u, v.address); }
  static void set_scene(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E44u, v); }
  static constexpr uint32_t scene_address() { return 0x826C2E44u; }
  // the cars' names' addresses, by block
  static constexpr uint32_t car_names_address() { return 0x826018C8u; }
  // 1024
  static uint32_t vinyl_sheet_width(uint8_t* base) { return guest::LoadU32(base, 0x82602570u); }
  static void set_vinyl_sheet_width(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82602570u, v); }
  static constexpr uint32_t vinyl_sheet_width_address() { return 0x82602570u; }
  // 1024
  static uint32_t vinyl_sheet_height(uint8_t* base) { return guest::LoadU32(base, 0x82602574u); }
  static void set_vinyl_sheet_height(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82602574u, v); }
  static constexpr uint32_t vinyl_sheet_height_address() { return 0x82602574u; }
  // 16: the corner Vinyl::ClearCorner clears
  static uint32_t vinyl_corner_width(uint8_t* base) { return guest::LoadU32(base, 0x82602578u); }
  static void set_vinyl_corner_width(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82602578u, v); }
  static constexpr uint32_t vinyl_corner_width_address() { return 0x82602578u; }
  // 16
  static uint32_t vinyl_corner_rows(uint8_t* base) { return guest::LoadU32(base, 0x8260257Cu); }
  static void set_vinyl_corner_rows(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8260257Cu, v); }
  static constexpr uint32_t vinyl_corner_rows_address() { return 0x8260257Cu; }
  // the sheet's ten regions (16 bytes each: left, top, right, bottom)
  static constexpr uint32_t vinyl_regions_address() { return 0x82602A50u; }
  // MakePlate's slots' rectangles (16 bytes each)
  static constexpr uint32_t plate_slots_address() { return 0x82602AF0u; }
  // the scene wheels' 44-byte entries, by block (45 + the traffic index for traffic)
  static constexpr uint32_t wheel_table_address() { return 0x820044B0u; }
  // the archive's shared load buffer's size
  static uint32_t archive_buffer_size(uint8_t* base) { return guest::LoadU32(base, 0x826A5130u); }
  static void set_archive_buffer_size(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5130u, v); }
  static constexpr uint32_t archive_buffer_size_address() { return 0x826A5130u; }
  // the archive's shared load buffer
  static uint32_t archive_buffer(uint8_t* base) { return guest::LoadU32(base, 0x826A5134u); }
  static void set_archive_buffer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5134u, v); }
  static constexpr uint32_t archive_buffer_address() { return 0x826A5134u; }
  // 1.0
  static float float_one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_float_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t float_one_address() { return 0x82000A30u; }
  // 0.0
  static float float_zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_float_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t float_zero_address() { return 0x82000A34u; }
  // 0.5
  static float float_half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_float_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t float_half_address() { return 0x82001BE4u; }
  // the texel weights' scale
  static float texel_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82001FACu); }
  static void set_texel_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FACu, v); }
  static constexpr uint32_t texel_scale_address() { return 0x82001FACu; }
  // 0.1
  static float float_tenth(uint8_t* base) { return guest::LoadFloat(base, 0x82001FBCu); }
  static void set_float_tenth(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FBCu, v); }
  static constexpr uint32_t float_tenth_address() { return 0x82001FBCu; }
  // -pi / 2
  static float quarter_turn(uint8_t* base) { return guest::LoadFloat(base, 0x82002A50u); }
  static void set_quarter_turn(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A50u, v); }
  static constexpr uint32_t quarter_turn_address() { return 0x82002A50u; }
};

}  // namespace schema
