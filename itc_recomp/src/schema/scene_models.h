// The views of docs/functions/scene-models.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/scene-models.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: SceneModel, ScenePools::SetUpB.ret
#pragma pack(push, 1)
struct SceneModelData {
  uint32_t vtable = 0;  // +0
  float joined_vector_y = 0;  // +4
  float joined_vector_z = 0;  // +8
  float joined_vector_w = 0;  // +12
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint8_t pad_24[8] = {};
  uint32_t kind = 0;  // +32
  uint8_t pad_36[12] = {};
  uint32_t block = 0;  // +48
  uint8_t pad_52[12] = {};
  uint8_t paint[48] = {};  // +64, guest order
  uint8_t wheel_paint[96] = {};  // +112, guest order
  uint8_t pad_208[3520] = {};
  uint32_t selected_texture = 0;  // +3728
  uint8_t pad_3732[32] = {};
  uint32_t parts_bonnet = 0;  // +3764
  uint32_t parts_aero_kit = 0;  // +3768
  uint32_t parts_aero6 = 0;  // +3772
  uint32_t parts_aero7 = 0;  // +3776
  uint8_t pad_3780[4] = {};
  uint32_t parts_aero9 = 0;  // +3784
  uint32_t parts_engine_stage = 0;  // +3788
  uint8_t pad_3792[32] = {};
  uint32_t interior_gauge1_present = 0;  // +3824
  uint32_t interior_gauge1_kind = 0;  // +3828
  uint8_t pad_3832[92] = {};
  uint32_t cockpit_bonnet = 0;  // +3924
  uint32_t cockpit_aero_kit = 0;  // +3928
  uint32_t cockpit_aero6 = 0;  // +3932
  uint32_t cockpit_aero7 = 0;  // +3936
  uint32_t cockpit_aero9 = 0;  // +3940
  uint32_t cockpit_selected_texture = 0;  // +3944
  uint8_t pad_3948[120] = {};
  uint32_t wheels_kind = 0;  // +4068
  uint32_t wheels_painted = 0;  // +4072
  uint8_t pad_4076[148] = {};
  uint8_t light_kit_record[52] = {};  // +4224, guest order
  uint8_t pad_4276[20] = {};
  uint32_t draw_enabled = 0;  // +4296
  uint32_t cockpit_only = 0;  // +4300
  uint8_t matrix[64] = {};  // +4304, guest order
  uint8_t pad_4368[548] = {};
  float windows_alpha = 0;  // +4916
  uint8_t pad_4920[104] = {};
  uint32_t set_up = 0;  // +5024
  uint32_t release_pending = 0;  // +5028
  uint32_t second_wheels_claimed = 0;  // +5032
  uint32_t ready = 0;  // +5036
  uint32_t build_pending = 0;  // +5040
  uint32_t build_delay = 0;  // +5044
  uint32_t car_model = 0;  // +5048
  uint32_t car_model_ready = 0;  // +5052
  uint32_t decals_reloaded = 0;  // +5056
  uint32_t parts = 0;  // +5060
  uint32_t parts_ready = 0;  // +5064
  uint32_t interior = 0;  // +5068
  uint32_t interior_ready = 0;  // +5072
  uint32_t cockpit = 0;  // +5076
  uint32_t cockpit_ready = 0;  // +5080
  uint32_t light_kit = 0;  // +5084
  uint32_t light_kit_ready = 0;  // +5088
  uint32_t traffic_body = 0;  // +5092
  uint32_t traffic_body_ready = 0;  // +5096
  uint32_t wheels = 0;  // +5100
  uint32_t wheels_ready = 0;  // +5104
  uint32_t second_wheels = 0;  // +5108
  uint32_t second_wheels_ready = 0;  // +5112
  uint32_t scene = 0;  // +5116
};
#pragma pack(pop)
static_assert(sizeof(SceneModelData) == 5120);
inline constexpr memory::LayoutField kSceneModelFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {32, 4}, {48, 4}, {3728, 4}, {3764, 4}, {3768, 4}, {3772, 4}, {3776, 4}, {3784, 4}, {3788, 4}, {3824, 4}, {3828, 4}, {3924, 4}, {3928, 4}, {3932, 4}, {3936, 4}, {3940, 4}, {3944, 4}, {4068, 4}, {4072, 4}, {4296, 4}, {4300, 4}, {4916, 4}, {5024, 4}, {5028, 4}, {5032, 4}, {5036, 4}, {5040, 4}, {5044, 4}, {5048, 4}, {5052, 4}, {5056, 4}, {5060, 4}, {5064, 4}, {5068, 4}, {5072, 4}, {5076, 4}, {5080, 4}, {5084, 4}, {5088, 4}, {5092, 4}, {5096, 4}, {5100, 4}, {5104, 4}, {5108, 4}, {5112, 4}, {5116, 4}};
inline constexpr memory::Layout kSceneModelLayout = {kSceneModelFields, 52};

struct SceneModel : View {
  using View::View;
  static constexpr uint32_t kSize = 5120;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82004F90;
  SceneModelData* data() const { return static_cast<SceneModelData*>(memory::HandleObject(address)); }
  static SceneModel New() {
    return SceneModel(memory::MakeHandle(new SceneModelData(), sizeof(SceneModelData), "SceneModel", &kSceneModelLayout));
  }
  template <uint32_t Stride = kSize>
  static SceneModel NewArray(uint32_t count) {
    static_assert(sizeof(SceneModelData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SceneModelData*>(::operator new(sizeof(SceneModelData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SceneModelData();
    return SceneModel(memory::MakeHandle(p, sizeof(SceneModelData) * count, "SceneModel", &kSceneModelLayout, sizeof(SceneModelData)));
  }
  SceneModel at(uint32_t i) const { return SceneModel(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SceneModelData* direct() const { return static_cast<SceneModelData*>(memory::HandleOwn(address, &kSceneModelLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSceneModelLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSceneModelLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82004F90 (measure fault: also SceneModel::Beam, BeamShape and BeamColor's out vector and CarScene::Draw's reads of it, filed at +0 to +15)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // measure fault: the beam functions' out vector's x and y (ld/std)
  uint32_t joined_vector_xy_address() const { return address + 0; }
  uint64_t joined_vector_xy_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kSceneModelLayout, 0, 8)); return uint64_t((memory::IsHandle(address + 0) ? uint64_t(memory::OwnerLoad(address + 0, 8)) : guest::LoadU64(address + 0))); }
  void set_joined_vector_xy_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSceneModelLayout, 0, 8, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 8, v); else guest::StoreU64(address + 0, v); }
  uint64_t joined_vector_xy() const { return uint64_t(joined_vector_xy_bits()); }
  void set_joined_vector_xy(uint64_t v) const { set_joined_vector_xy_bits(uint64_t(v)); }
  // measure fault: the out vector's y
  uint32_t joined_vector_y_address() const { return address + 4; }
  uint32_t joined_vector_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->joined_vector_y); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_joined_vector_y_bits(uint32_t v) const { if (auto* d = direct()) { d->joined_vector_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  float joined_vector_y() const { return std::bit_cast<float>(joined_vector_y_bits()); }
  void set_joined_vector_y(float v) const { set_joined_vector_y_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: the out vector's z
  uint32_t joined_vector_z_address() const { return address + 8; }
  uint32_t joined_vector_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->joined_vector_z); return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_joined_vector_z_bits(uint32_t v) const { if (auto* d = direct()) { d->joined_vector_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  float joined_vector_z() const { return std::bit_cast<float>(joined_vector_z_bits()); }
  void set_joined_vector_z(float v) const { set_joined_vector_z_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: the out vector's z and w (ld/std)
  uint32_t joined_vector_zw_address() const { return address + 8; }
  uint64_t joined_vector_zw_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kSceneModelLayout, 8, 8)); return uint64_t((memory::IsHandle(address + 8) ? uint64_t(memory::OwnerLoad(address + 8, 8)) : guest::LoadU64(address + 8))); }
  void set_joined_vector_zw_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSceneModelLayout, 8, 8, v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 8, v); else guest::StoreU64(address + 8, v); }
  uint64_t joined_vector_zw() const { return uint64_t(joined_vector_zw_bits()); }
  void set_joined_vector_zw(uint64_t v) const { set_joined_vector_zw_bits(uint64_t(v)); }
  // measure fault: the out vector's w
  uint32_t joined_vector_w_address() const { return address + 12; }
  uint32_t joined_vector_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->joined_vector_w); return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_joined_vector_w_bits(uint32_t v) const { if (auto* d = direct()) { d->joined_vector_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  float joined_vector_w() const { return std::bit_cast<float>(joined_vector_w_bits()); }
  void set_joined_vector_w(float v) const { set_joined_vector_w_bits(std::bit_cast<uint32_t>(v)); }
  // the pool object's: 3 in use
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // its index in the pool: the sound slot (0x826D8334 + 4i and the others)
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // a copy of its 4,244-byte record (SceneModelRecord; SceneModel::SetUp's SceneModelRecord::Copy). Measure fault: SceneModel::Accessory's interior loading object +140 (the interior record's +112, the accessory) is filed at +140
  uint32_t record_address() const { return address + 32; }
  uint8_t* record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 32 : guest::Pointer(address + 32); }
  // the record's +0: 1 and 2 cars (2 the players', the only kind with a cockpit), 3 traffic
  uint32_t kind_address() const { return address + 32; }
  uint32_t kind_bits() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t kind() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_kind(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // the record's +16: the car record (CarRecord)
  uint32_t car_record_address() const { return address + 48; }
  uint8_t* car_record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 48 : guest::Pointer(address + 48); }
  // the car record's +0: the model block (5 sets interior_gauge1_present and _kind)
  uint32_t block_address() const { return address + 48; }
  uint32_t block_bits() const { if (auto* d = direct()) return d->block; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_block_bits(uint32_t v) const { if (auto* d = direct()) { d->block = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t block() const { if (auto* d = direct()) return d->block; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_block(uint32_t v) const { if (auto* d = direct()) { d->block = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // the car record's +16: three 16-byte hue, saturation, value rows (SceneModel::ApplyPaint, load-poll.md)
  uint32_t paint_address() const { return address + 64; }
  uint8_t* paint() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 64 : guest::Pointer(address + 64); }
  // the car record's +64: six rows for the wheels, front then rear (SceneModel::ApplyWheelPaint)
  uint32_t wheel_paint_address() const { return address + 112; }
  uint8_t* wheel_paint() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 112 : guest::Pointer(address + 112); }
  // the car record's +3680 (the car model's selected texture); copied to the cockpit record's +20 by SceneModel::Build
  uint32_t selected_texture_address() const { return address + 3728; }
  uint32_t selected_texture_bits() const { if (auto* d = direct()) return d->selected_texture; return uint32_t((memory::IsHandle(address + 3728) ? uint32_t(memory::OwnerLoad(address + 3728, 4)) : guest::LoadU32(address + 3728))); }
  void set_selected_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->selected_texture = v; return; } if (memory::IsHandle(address + 3728)) memory::OwnerStore(address + 3728, 4, v); else guest::StoreU32(address + 3728, v); }
  uint32_t selected_texture() const { if (auto* d = direct()) return d->selected_texture; return uint32_t((memory::IsHandle(address + 3728) ? uint32_t(memory::OwnerLoad(address + 3728, 4)) : guest::LoadU32(address + 3728))); }
  void set_selected_texture(uint32_t v) const { if (auto* d = direct()) { d->selected_texture = v; return; } if (memory::IsHandle(address + 3728)) memory::OwnerStore(address + 3728, 4, v); else guest::StoreU32(address + 3728, v); }
  // the record's +3712: the parts' 60 bytes (PartsRecord), handed to ScenePools::SetUpB and SceneParts::Apply
  uint32_t parts_record_address() const { return address + 3744; }
  uint8_t* parts_record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 3744 : guest::Pointer(address + 3744); }
  // the parts record's +20: the cockpit record's +0 (plus 5 when 6 or more, SceneModel::Build)
  uint32_t parts_bonnet_address() const { return address + 3764; }
  uint32_t parts_bonnet_bits() const { if (auto* d = direct()) return d->parts_bonnet; return uint32_t((memory::IsHandle(address + 3764) ? uint32_t(memory::OwnerLoad(address + 3764, 4)) : guest::LoadU32(address + 3764))); }
  void set_parts_bonnet_bits(uint32_t v) const { if (auto* d = direct()) { d->parts_bonnet = v; return; } if (memory::IsHandle(address + 3764)) memory::OwnerStore(address + 3764, 4, v); else guest::StoreU32(address + 3764, v); }
  uint32_t parts_bonnet() const { if (auto* d = direct()) return d->parts_bonnet; return uint32_t((memory::IsHandle(address + 3764) ? uint32_t(memory::OwnerLoad(address + 3764, 4)) : guest::LoadU32(address + 3764))); }
  void set_parts_bonnet(uint32_t v) const { if (auto* d = direct()) { d->parts_bonnet = v; return; } if (memory::IsHandle(address + 3764)) memory::OwnerStore(address + 3764, 4, v); else guest::StoreU32(address + 3764, v); }
  // the parts record's +24: the cockpit record's +4
  uint32_t parts_aero_kit_address() const { return address + 3768; }
  uint32_t parts_aero_kit_bits() const { if (auto* d = direct()) return d->parts_aero_kit; return uint32_t((memory::IsHandle(address + 3768) ? uint32_t(memory::OwnerLoad(address + 3768, 4)) : guest::LoadU32(address + 3768))); }
  void set_parts_aero_kit_bits(uint32_t v) const { if (auto* d = direct()) { d->parts_aero_kit = v; return; } if (memory::IsHandle(address + 3768)) memory::OwnerStore(address + 3768, 4, v); else guest::StoreU32(address + 3768, v); }
  uint32_t parts_aero_kit() const { if (auto* d = direct()) return d->parts_aero_kit; return uint32_t((memory::IsHandle(address + 3768) ? uint32_t(memory::OwnerLoad(address + 3768, 4)) : guest::LoadU32(address + 3768))); }
  void set_parts_aero_kit(uint32_t v) const { if (auto* d = direct()) { d->parts_aero_kit = v; return; } if (memory::IsHandle(address + 3768)) memory::OwnerStore(address + 3768, 4, v); else guest::StoreU32(address + 3768, v); }
  // the parts record's +28: the cockpit record's +8
  uint32_t parts_aero6_address() const { return address + 3772; }
  uint32_t parts_aero6_bits() const { if (auto* d = direct()) return d->parts_aero6; return uint32_t((memory::IsHandle(address + 3772) ? uint32_t(memory::OwnerLoad(address + 3772, 4)) : guest::LoadU32(address + 3772))); }
  void set_parts_aero6_bits(uint32_t v) const { if (auto* d = direct()) { d->parts_aero6 = v; return; } if (memory::IsHandle(address + 3772)) memory::OwnerStore(address + 3772, 4, v); else guest::StoreU32(address + 3772, v); }
  uint32_t parts_aero6() const { if (auto* d = direct()) return d->parts_aero6; return uint32_t((memory::IsHandle(address + 3772) ? uint32_t(memory::OwnerLoad(address + 3772, 4)) : guest::LoadU32(address + 3772))); }
  void set_parts_aero6(uint32_t v) const { if (auto* d = direct()) { d->parts_aero6 = v; return; } if (memory::IsHandle(address + 3772)) memory::OwnerStore(address + 3772, 4, v); else guest::StoreU32(address + 3772, v); }
  // the parts record's +32: the cockpit record's +12
  uint32_t parts_aero7_address() const { return address + 3776; }
  uint32_t parts_aero7_bits() const { if (auto* d = direct()) return d->parts_aero7; return uint32_t((memory::IsHandle(address + 3776) ? uint32_t(memory::OwnerLoad(address + 3776, 4)) : guest::LoadU32(address + 3776))); }
  void set_parts_aero7_bits(uint32_t v) const { if (auto* d = direct()) { d->parts_aero7 = v; return; } if (memory::IsHandle(address + 3776)) memory::OwnerStore(address + 3776, 4, v); else guest::StoreU32(address + 3776, v); }
  uint32_t parts_aero7() const { if (auto* d = direct()) return d->parts_aero7; return uint32_t((memory::IsHandle(address + 3776) ? uint32_t(memory::OwnerLoad(address + 3776, 4)) : guest::LoadU32(address + 3776))); }
  void set_parts_aero7(uint32_t v) const { if (auto* d = direct()) { d->parts_aero7 = v; return; } if (memory::IsHandle(address + 3776)) memory::OwnerStore(address + 3776, 4, v); else guest::StoreU32(address + 3776, v); }
  // the parts record's +40: the cockpit record's +16 (plus 7 when 4 or more)
  uint32_t parts_aero9_address() const { return address + 3784; }
  uint32_t parts_aero9_bits() const { if (auto* d = direct()) return d->parts_aero9; return uint32_t((memory::IsHandle(address + 3784) ? uint32_t(memory::OwnerLoad(address + 3784, 4)) : guest::LoadU32(address + 3784))); }
  void set_parts_aero9_bits(uint32_t v) const { if (auto* d = direct()) { d->parts_aero9 = v; return; } if (memory::IsHandle(address + 3784)) memory::OwnerStore(address + 3784, 4, v); else guest::StoreU32(address + 3784, v); }
  uint32_t parts_aero9() const { if (auto* d = direct()) return d->parts_aero9; return uint32_t((memory::IsHandle(address + 3784) ? uint32_t(memory::OwnerLoad(address + 3784, 4)) : guest::LoadU32(address + 3784))); }
  void set_parts_aero9(uint32_t v) const { if (auto* d = direct()) { d->parts_aero9 = v; return; } if (memory::IsHandle(address + 3784)) memory::OwnerStore(address + 3784, 4, v); else guest::StoreU32(address + 3784, v); }
  // the parts record's +44: picks the sound assets' index (SceneModel::SetUp, blocks below 18)
  uint32_t parts_engine_stage_address() const { return address + 3788; }
  uint32_t parts_engine_stage_bits() const { if (auto* d = direct()) return d->parts_engine_stage; return uint32_t((memory::IsHandle(address + 3788) ? uint32_t(memory::OwnerLoad(address + 3788, 4)) : guest::LoadU32(address + 3788))); }
  void set_parts_engine_stage_bits(uint32_t v) const { if (auto* d = direct()) { d->parts_engine_stage = v; return; } if (memory::IsHandle(address + 3788)) memory::OwnerStore(address + 3788, 4, v); else guest::StoreU32(address + 3788, v); }
  uint32_t parts_engine_stage() const { if (auto* d = direct()) return d->parts_engine_stage; return uint32_t((memory::IsHandle(address + 3788) ? uint32_t(memory::OwnerLoad(address + 3788, 4)) : guest::LoadU32(address + 3788))); }
  void set_parts_engine_stage(uint32_t v) const { if (auto* d = direct()) { d->parts_engine_stage = v; return; } if (memory::IsHandle(address + 3788)) memory::OwnerStore(address + 3788, 4, v); else guest::StoreU32(address + 3788, v); }
  // the record's +3772: the interior's 120 bytes (InteriorRecord), handed to ScenePools::SetUpB and SceneInterior::Apply, copied to the cockpit record's +24
  uint32_t interior_record_address() const { return address + 3804; }
  uint8_t* interior_record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 3804 : guest::Pointer(address + 3804); }
  // the interior record's +20: 1 for block 5 (SceneModel::SetUp, ApplyRecord)
  uint32_t interior_gauge1_present_address() const { return address + 3824; }
  uint32_t interior_gauge1_present_bits() const { if (auto* d = direct()) return d->interior_gauge1_present; return uint32_t((memory::IsHandle(address + 3824) ? uint32_t(memory::OwnerLoad(address + 3824, 4)) : guest::LoadU32(address + 3824))); }
  void set_interior_gauge1_present_bits(uint32_t v) const { if (auto* d = direct()) { d->interior_gauge1_present = v; return; } if (memory::IsHandle(address + 3824)) memory::OwnerStore(address + 3824, 4, v); else guest::StoreU32(address + 3824, v); }
  uint32_t interior_gauge1_present() const { if (auto* d = direct()) return d->interior_gauge1_present; return uint32_t((memory::IsHandle(address + 3824) ? uint32_t(memory::OwnerLoad(address + 3824, 4)) : guest::LoadU32(address + 3824))); }
  void set_interior_gauge1_present(uint32_t v) const { if (auto* d = direct()) { d->interior_gauge1_present = v; return; } if (memory::IsHandle(address + 3824)) memory::OwnerStore(address + 3824, 4, v); else guest::StoreU32(address + 3824, v); }
  // the interior record's +24: 0 for block 5
  uint32_t interior_gauge1_kind_address() const { return address + 3828; }
  uint32_t interior_gauge1_kind_bits() const { if (auto* d = direct()) return d->interior_gauge1_kind; return uint32_t((memory::IsHandle(address + 3828) ? uint32_t(memory::OwnerLoad(address + 3828, 4)) : guest::LoadU32(address + 3828))); }
  void set_interior_gauge1_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->interior_gauge1_kind = v; return; } if (memory::IsHandle(address + 3828)) memory::OwnerStore(address + 3828, 4, v); else guest::StoreU32(address + 3828, v); }
  uint32_t interior_gauge1_kind() const { if (auto* d = direct()) return d->interior_gauge1_kind; return uint32_t((memory::IsHandle(address + 3828) ? uint32_t(memory::OwnerLoad(address + 3828, 4)) : guest::LoadU32(address + 3828))); }
  void set_interior_gauge1_kind(uint32_t v) const { if (auto* d = direct()) { d->interior_gauge1_kind = v; return; } if (memory::IsHandle(address + 3828)) memory::OwnerStore(address + 3828, 4, v); else guest::StoreU32(address + 3828, v); }
  // the record's +3892: the cockpit's 144 bytes (SceneModelsCockpitRecord), filled by SceneModel::Build for kind 2 in a race
  uint32_t cockpit_record_address() const { return address + 3924; }
  uint8_t* cockpit_record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 3924 : guest::Pointer(address + 3924); }
  // the cockpit record's +0
  uint32_t cockpit_bonnet_address() const { return address + 3924; }
  uint32_t cockpit_bonnet_bits() const { if (auto* d = direct()) return d->cockpit_bonnet; return uint32_t((memory::IsHandle(address + 3924) ? uint32_t(memory::OwnerLoad(address + 3924, 4)) : guest::LoadU32(address + 3924))); }
  void set_cockpit_bonnet_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit_bonnet = v; return; } if (memory::IsHandle(address + 3924)) memory::OwnerStore(address + 3924, 4, v); else guest::StoreU32(address + 3924, v); }
  uint32_t cockpit_bonnet() const { if (auto* d = direct()) return d->cockpit_bonnet; return uint32_t((memory::IsHandle(address + 3924) ? uint32_t(memory::OwnerLoad(address + 3924, 4)) : guest::LoadU32(address + 3924))); }
  void set_cockpit_bonnet(uint32_t v) const { if (auto* d = direct()) { d->cockpit_bonnet = v; return; } if (memory::IsHandle(address + 3924)) memory::OwnerStore(address + 3924, 4, v); else guest::StoreU32(address + 3924, v); }
  // the cockpit record's +4
  uint32_t cockpit_aero_kit_address() const { return address + 3928; }
  uint32_t cockpit_aero_kit_bits() const { if (auto* d = direct()) return d->cockpit_aero_kit; return uint32_t((memory::IsHandle(address + 3928) ? uint32_t(memory::OwnerLoad(address + 3928, 4)) : guest::LoadU32(address + 3928))); }
  void set_cockpit_aero_kit_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit_aero_kit = v; return; } if (memory::IsHandle(address + 3928)) memory::OwnerStore(address + 3928, 4, v); else guest::StoreU32(address + 3928, v); }
  uint32_t cockpit_aero_kit() const { if (auto* d = direct()) return d->cockpit_aero_kit; return uint32_t((memory::IsHandle(address + 3928) ? uint32_t(memory::OwnerLoad(address + 3928, 4)) : guest::LoadU32(address + 3928))); }
  void set_cockpit_aero_kit(uint32_t v) const { if (auto* d = direct()) { d->cockpit_aero_kit = v; return; } if (memory::IsHandle(address + 3928)) memory::OwnerStore(address + 3928, 4, v); else guest::StoreU32(address + 3928, v); }
  // the cockpit record's +8
  uint32_t cockpit_aero6_address() const { return address + 3932; }
  uint32_t cockpit_aero6_bits() const { if (auto* d = direct()) return d->cockpit_aero6; return uint32_t((memory::IsHandle(address + 3932) ? uint32_t(memory::OwnerLoad(address + 3932, 4)) : guest::LoadU32(address + 3932))); }
  void set_cockpit_aero6_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit_aero6 = v; return; } if (memory::IsHandle(address + 3932)) memory::OwnerStore(address + 3932, 4, v); else guest::StoreU32(address + 3932, v); }
  uint32_t cockpit_aero6() const { if (auto* d = direct()) return d->cockpit_aero6; return uint32_t((memory::IsHandle(address + 3932) ? uint32_t(memory::OwnerLoad(address + 3932, 4)) : guest::LoadU32(address + 3932))); }
  void set_cockpit_aero6(uint32_t v) const { if (auto* d = direct()) { d->cockpit_aero6 = v; return; } if (memory::IsHandle(address + 3932)) memory::OwnerStore(address + 3932, 4, v); else guest::StoreU32(address + 3932, v); }
  // the cockpit record's +12
  uint32_t cockpit_aero7_address() const { return address + 3936; }
  uint32_t cockpit_aero7_bits() const { if (auto* d = direct()) return d->cockpit_aero7; return uint32_t((memory::IsHandle(address + 3936) ? uint32_t(memory::OwnerLoad(address + 3936, 4)) : guest::LoadU32(address + 3936))); }
  void set_cockpit_aero7_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit_aero7 = v; return; } if (memory::IsHandle(address + 3936)) memory::OwnerStore(address + 3936, 4, v); else guest::StoreU32(address + 3936, v); }
  uint32_t cockpit_aero7() const { if (auto* d = direct()) return d->cockpit_aero7; return uint32_t((memory::IsHandle(address + 3936) ? uint32_t(memory::OwnerLoad(address + 3936, 4)) : guest::LoadU32(address + 3936))); }
  void set_cockpit_aero7(uint32_t v) const { if (auto* d = direct()) { d->cockpit_aero7 = v; return; } if (memory::IsHandle(address + 3936)) memory::OwnerStore(address + 3936, 4, v); else guest::StoreU32(address + 3936, v); }
  // the cockpit record's +16
  uint32_t cockpit_aero9_address() const { return address + 3940; }
  uint32_t cockpit_aero9_bits() const { if (auto* d = direct()) return d->cockpit_aero9; return uint32_t((memory::IsHandle(address + 3940) ? uint32_t(memory::OwnerLoad(address + 3940, 4)) : guest::LoadU32(address + 3940))); }
  void set_cockpit_aero9_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit_aero9 = v; return; } if (memory::IsHandle(address + 3940)) memory::OwnerStore(address + 3940, 4, v); else guest::StoreU32(address + 3940, v); }
  uint32_t cockpit_aero9() const { if (auto* d = direct()) return d->cockpit_aero9; return uint32_t((memory::IsHandle(address + 3940) ? uint32_t(memory::OwnerLoad(address + 3940, 4)) : guest::LoadU32(address + 3940))); }
  void set_cockpit_aero9(uint32_t v) const { if (auto* d = direct()) { d->cockpit_aero9 = v; return; } if (memory::IsHandle(address + 3940)) memory::OwnerStore(address + 3940, 4, v); else guest::StoreU32(address + 3940, v); }
  // the cockpit record's +20
  uint32_t cockpit_selected_texture_address() const { return address + 3944; }
  uint32_t cockpit_selected_texture_bits() const { if (auto* d = direct()) return d->cockpit_selected_texture; return uint32_t((memory::IsHandle(address + 3944) ? uint32_t(memory::OwnerLoad(address + 3944, 4)) : guest::LoadU32(address + 3944))); }
  void set_cockpit_selected_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit_selected_texture = v; return; } if (memory::IsHandle(address + 3944)) memory::OwnerStore(address + 3944, 4, v); else guest::StoreU32(address + 3944, v); }
  uint32_t cockpit_selected_texture() const { if (auto* d = direct()) return d->cockpit_selected_texture; return uint32_t((memory::IsHandle(address + 3944) ? uint32_t(memory::OwnerLoad(address + 3944, 4)) : guest::LoadU32(address + 3944))); }
  void set_cockpit_selected_texture(uint32_t v) const { if (auto* d = direct()) { d->cockpit_selected_texture = v; return; } if (memory::IsHandle(address + 3944)) memory::OwnerStore(address + 3944, 4, v); else guest::StoreU32(address + 3944, v); }
  // the record's +4036: the wheels' 156 bytes (SceneModelsWheelsRecord), handed to ScenePools::SetUpB
  uint32_t wheels_record_address() const { return address + 4068; }
  uint8_t* wheels_record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4068 : guest::Pointer(address + 4068); }
  // the wheels record's +0: the kind, stored by SceneModel::SetUp
  uint32_t wheels_kind_address() const { return address + 4068; }
  uint32_t wheels_kind_bits() const { if (auto* d = direct()) return d->wheels_kind; return uint32_t((memory::IsHandle(address + 4068) ? uint32_t(memory::OwnerLoad(address + 4068, 4)) : guest::LoadU32(address + 4068))); }
  void set_wheels_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->wheels_kind = v; return; } if (memory::IsHandle(address + 4068)) memory::OwnerStore(address + 4068, 4, v); else guest::StoreU32(address + 4068, v); }
  uint32_t wheels_kind() const { if (auto* d = direct()) return d->wheels_kind; return uint32_t((memory::IsHandle(address + 4068) ? uint32_t(memory::OwnerLoad(address + 4068, 4)) : guest::LoadU32(address + 4068))); }
  void set_wheels_kind(uint32_t v) const { if (auto* d = direct()) { d->wheels_kind = v; return; } if (memory::IsHandle(address + 4068)) memory::OwnerStore(address + 4068, 4, v); else guest::StoreU32(address + 4068, v); }
  // the wheels record's +4 (SceneModel::ApplyWheelPaint)
  uint32_t wheels_painted_address() const { return address + 4072; }
  uint32_t wheels_painted_bits() const { if (auto* d = direct()) return d->wheels_painted; return uint32_t((memory::IsHandle(address + 4072) ? uint32_t(memory::OwnerLoad(address + 4072, 4)) : guest::LoadU32(address + 4072))); }
  void set_wheels_painted_bits(uint32_t v) const { if (auto* d = direct()) { d->wheels_painted = v; return; } if (memory::IsHandle(address + 4072)) memory::OwnerStore(address + 4072, 4, v); else guest::StoreU32(address + 4072, v); }
  uint32_t wheels_painted() const { if (auto* d = direct()) return d->wheels_painted; return uint32_t((memory::IsHandle(address + 4072) ? uint32_t(memory::OwnerLoad(address + 4072, 4)) : guest::LoadU32(address + 4072))); }
  void set_wheels_painted(uint32_t v) const { if (auto* d = direct()) { d->wheels_painted = v; return; } if (memory::IsHandle(address + 4072)) memory::OwnerStore(address + 4072, 4, v); else guest::StoreU32(address + 4072, v); }
  // the record's +4192: the light kit's 52 bytes (LightKitRecord), handed to ScenePools::SetUpB
  uint32_t light_kit_record_address() const { return address + 4224; }
  uint8_t* light_kit_record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4224 : guest::Pointer(address + 4224); }
  // the 736-byte block of matrices and draw words (car-draw.toml's CarDrawBlock, second-set.md's SceneModelBlock)
  uint32_t draw_block_address() const { return address + 4288; }
  uint8_t* draw_block() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4288 : guest::Pointer(address + 4288); }
  // the block's +8: a word from its owner (sub_8217FCB0); 0 makes Car::DrawUpdate draw nothing; SceneModel::Update's accessory waits for it in the front end
  uint32_t draw_enabled_address() const { return address + 4296; }
  uint32_t draw_enabled_bits() const { if (auto* d = direct()) return d->draw_enabled; return uint32_t((memory::IsHandle(address + 4296) ? uint32_t(memory::OwnerLoad(address + 4296, 4)) : guest::LoadU32(address + 4296))); }
  void set_draw_enabled_bits(uint32_t v) const { if (auto* d = direct()) { d->draw_enabled = v; return; } if (memory::IsHandle(address + 4296)) memory::OwnerStore(address + 4296, 4, v); else guest::StoreU32(address + 4296, v); }
  uint32_t draw_enabled() const { if (auto* d = direct()) return d->draw_enabled; return uint32_t((memory::IsHandle(address + 4296) ? uint32_t(memory::OwnerLoad(address + 4296, 4)) : guest::LoadU32(address + 4296))); }
  void set_draw_enabled(uint32_t v) const { if (auto* d = direct()) { d->draw_enabled = v; return; } if (memory::IsHandle(address + 4296)) memory::OwnerStore(address + 4296, 4, v); else guest::StoreU32(address + 4296, v); }
  // the block's +12: with kind 2 only the cockpit is drawn (probably the cockpit view)
  uint32_t cockpit_only_address() const { return address + 4300; }
  uint32_t cockpit_only_bits() const { if (auto* d = direct()) return d->cockpit_only; return uint32_t((memory::IsHandle(address + 4300) ? uint32_t(memory::OwnerLoad(address + 4300, 4)) : guest::LoadU32(address + 4300))); }
  void set_cockpit_only_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit_only = v; return; } if (memory::IsHandle(address + 4300)) memory::OwnerStore(address + 4300, 4, v); else guest::StoreU32(address + 4300, v); }
  uint32_t cockpit_only() const { if (auto* d = direct()) return d->cockpit_only; return uint32_t((memory::IsHandle(address + 4300) ? uint32_t(memory::OwnerLoad(address + 4300, 4)) : guest::LoadU32(address + 4300))); }
  void set_cockpit_only(uint32_t v) const { if (auto* d = direct()) { d->cockpit_only = v; return; } if (memory::IsHandle(address + 4300)) memory::OwnerStore(address + 4300, 4, v); else guest::StoreU32(address + 4300, v); }
  // the block's +16: the world matrix (SceneModel::Update's accessory placement, CarOverlays::FindModel)
  uint32_t matrix_address() const { return address + 4304; }
  uint8_t* matrix() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4304 : guest::Pointer(address + 4304); }
  // the block's +628: the *_02 windows' alpha ([0x8200296C] in the front end, else [0x82001FBC], probably 0.1)
  uint32_t windows_alpha_address() const { return address + 4916; }
  uint32_t windows_alpha_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->windows_alpha); return uint32_t((memory::IsHandle(address + 4916) ? uint32_t(memory::OwnerLoad(address + 4916, 4)) : guest::LoadU32(address + 4916))); }
  void set_windows_alpha_bits(uint32_t v) const { if (auto* d = direct()) { d->windows_alpha = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4916)) memory::OwnerStore(address + 4916, 4, v); else guest::StoreU32(address + 4916, v); }
  float windows_alpha() const { return std::bit_cast<float>(windows_alpha_bits()); }
  void set_windows_alpha(float v) const { set_windows_alpha_bits(std::bit_cast<uint32_t>(v)); }
  // 1 once SceneModel::SetUp ran
  uint32_t set_up_address() const { return address + 5024; }
  uint32_t set_up_bits() const { if (auto* d = direct()) return d->set_up; return uint32_t((memory::IsHandle(address + 5024) ? uint32_t(memory::OwnerLoad(address + 5024, 4)) : guest::LoadU32(address + 5024))); }
  void set_set_up_bits(uint32_t v) const { if (auto* d = direct()) { d->set_up = v; return; } if (memory::IsHandle(address + 5024)) memory::OwnerStore(address + 5024, 4, v); else guest::StoreU32(address + 5024, v); }
  uint32_t set_up() const { if (auto* d = direct()) return d->set_up; return uint32_t((memory::IsHandle(address + 5024) ? uint32_t(memory::OwnerLoad(address + 5024, 4)) : guest::LoadU32(address + 5024))); }
  void set_set_up(uint32_t v) const { if (auto* d = direct()) { d->set_up = v; return; } if (memory::IsHandle(address + 5024)) memory::OwnerStore(address + 5024, 4, v); else guest::StoreU32(address + 5024, v); }
  // the load poll waits while it's set (probably a release pending)
  uint32_t release_pending_address() const { return address + 5028; }
  uint32_t release_pending_bits() const { if (auto* d = direct()) return d->release_pending; return uint32_t((memory::IsHandle(address + 5028) ? uint32_t(memory::OwnerLoad(address + 5028, 4)) : guest::LoadU32(address + 5028))); }
  void set_release_pending_bits(uint32_t v) const { if (auto* d = direct()) { d->release_pending = v; return; } if (memory::IsHandle(address + 5028)) memory::OwnerStore(address + 5028, 4, v); else guest::StoreU32(address + 5028, v); }
  uint32_t release_pending() const { if (auto* d = direct()) return d->release_pending; return uint32_t((memory::IsHandle(address + 5028) ? uint32_t(memory::OwnerLoad(address + 5028, 4)) : guest::LoadU32(address + 5028))); }
  void set_release_pending(uint32_t v) const { if (auto* d = direct()) { d->release_pending = v; return; } if (memory::IsHandle(address + 5028)) memory::OwnerStore(address + 5028, 4, v); else guest::StoreU32(address + 5028, v); }
  // SceneModel::AddSecondWheels ran (the tuning screen waits for 0)
  uint32_t second_wheels_claimed_address() const { return address + 5032; }
  uint32_t second_wheels_claimed_bits() const { if (auto* d = direct()) return d->second_wheels_claimed; return uint32_t((memory::IsHandle(address + 5032) ? uint32_t(memory::OwnerLoad(address + 5032, 4)) : guest::LoadU32(address + 5032))); }
  void set_second_wheels_claimed_bits(uint32_t v) const { if (auto* d = direct()) { d->second_wheels_claimed = v; return; } if (memory::IsHandle(address + 5032)) memory::OwnerStore(address + 5032, 4, v); else guest::StoreU32(address + 5032, v); }
  uint32_t second_wheels_claimed() const { if (auto* d = direct()) return d->second_wheels_claimed; return uint32_t((memory::IsHandle(address + 5032) ? uint32_t(memory::OwnerLoad(address + 5032, 4)) : guest::LoadU32(address + 5032))); }
  void set_second_wheels_claimed(uint32_t v) const { if (auto* d = direct()) { d->second_wheels_claimed = v; return; } if (memory::IsHandle(address + 5032)) memory::OwnerStore(address + 5032, 4, v); else guest::StoreU32(address + 5032, v); }
  // the parts finished (the load poll; not for traffic)
  uint32_t ready_address() const { return address + 5036; }
  uint32_t ready_bits() const { if (auto* d = direct()) return d->ready; return uint32_t((memory::IsHandle(address + 5036) ? uint32_t(memory::OwnerLoad(address + 5036, 4)) : guest::LoadU32(address + 5036))); }
  void set_ready_bits(uint32_t v) const { if (auto* d = direct()) { d->ready = v; return; } if (memory::IsHandle(address + 5036)) memory::OwnerStore(address + 5036, 4, v); else guest::StoreU32(address + 5036, v); }
  uint32_t ready() const { if (auto* d = direct()) return d->ready; return uint32_t((memory::IsHandle(address + 5036) ? uint32_t(memory::OwnerLoad(address + 5036, 4)) : guest::LoadU32(address + 5036))); }
  void set_ready(uint32_t v) const { if (auto* d = direct()) { d->ready = v; return; } if (memory::IsHandle(address + 5036)) memory::OwnerStore(address + 5036, 4, v); else guest::StoreU32(address + 5036, v); }
  // SceneModel::Build deferred to SceneModel::Update
  uint32_t build_pending_address() const { return address + 5040; }
  uint32_t build_pending_bits() const { if (auto* d = direct()) return d->build_pending; return uint32_t((memory::IsHandle(address + 5040) ? uint32_t(memory::OwnerLoad(address + 5040, 4)) : guest::LoadU32(address + 5040))); }
  void set_build_pending_bits(uint32_t v) const { if (auto* d = direct()) { d->build_pending = v; return; } if (memory::IsHandle(address + 5040)) memory::OwnerStore(address + 5040, 4, v); else guest::StoreU32(address + 5040, v); }
  uint32_t build_pending() const { if (auto* d = direct()) return d->build_pending; return uint32_t((memory::IsHandle(address + 5040) ? uint32_t(memory::OwnerLoad(address + 5040, 4)) : guest::LoadU32(address + 5040))); }
  void set_build_pending(uint32_t v) const { if (auto* d = direct()) { d->build_pending = v; return; } if (memory::IsHandle(address + 5040)) memory::OwnerStore(address + 5040, 4, v); else guest::StoreU32(address + 5040, v); }
  // the updates until the deferred build (signed)
  uint32_t build_delay_address() const { return address + 5044; }
  uint32_t build_delay_bits() const { if (auto* d = direct()) return d->build_delay; return uint32_t((memory::IsHandle(address + 5044) ? uint32_t(memory::OwnerLoad(address + 5044, 4)) : guest::LoadU32(address + 5044))); }
  void set_build_delay_bits(uint32_t v) const { if (auto* d = direct()) { d->build_delay = v; return; } if (memory::IsHandle(address + 5044)) memory::OwnerStore(address + 5044, 4, v); else guest::StoreU32(address + 5044, v); }
  uint32_t build_delay() const { if (auto* d = direct()) return d->build_delay; return uint32_t((memory::IsHandle(address + 5044) ? uint32_t(memory::OwnerLoad(address + 5044, 4)) : guest::LoadU32(address + 5044))); }
  void set_build_delay(uint32_t v) const { if (auto* d = direct()) { d->build_delay = v; return; } if (memory::IsHandle(address + 5044)) memory::OwnerStore(address + 5044, 4, v); else guest::StoreU32(address + 5044, v); }
  // pool 0 of the second set (-1 for traffic)
  uint32_t car_model_address() const { return address + 5048; }
  uint32_t car_model_bits() const { if (auto* d = direct()) return d->car_model; return uint32_t((memory::IsHandle(address + 5048) ? uint32_t(memory::OwnerLoad(address + 5048, 4)) : guest::LoadU32(address + 5048))); }
  void set_car_model_bits(uint32_t v) const { if (auto* d = direct()) { d->car_model = v; return; } if (memory::IsHandle(address + 5048)) memory::OwnerStore(address + 5048, 4, v); else guest::StoreU32(address + 5048, v); }
  template <typename T = CarModelsSceneCarModel>
  T car_model() const { return T(car_model_bits()); }
  void set_car_model(const View& w) const { set_car_model_bits(w.address); }
  void set_car_model(uint32_t v) const { set_car_model_bits(v); }
  uint32_t car_model_ready_address() const { return address + 5052; }
  uint32_t car_model_ready_bits() const { if (auto* d = direct()) return d->car_model_ready; return uint32_t((memory::IsHandle(address + 5052) ? uint32_t(memory::OwnerLoad(address + 5052, 4)) : guest::LoadU32(address + 5052))); }
  void set_car_model_ready_bits(uint32_t v) const { if (auto* d = direct()) { d->car_model_ready = v; return; } if (memory::IsHandle(address + 5052)) memory::OwnerStore(address + 5052, 4, v); else guest::StoreU32(address + 5052, v); }
  uint32_t car_model_ready() const { if (auto* d = direct()) return d->car_model_ready; return uint32_t((memory::IsHandle(address + 5052) ? uint32_t(memory::OwnerLoad(address + 5052, 4)) : guest::LoadU32(address + 5052))); }
  void set_car_model_ready(uint32_t v) const { if (auto* d = direct()) { d->car_model_ready = v; return; } if (memory::IsHandle(address + 5052)) memory::OwnerStore(address + 5052, 4, v); else guest::StoreU32(address + 5052, v); }
  // 1 when the first-set car model's decals are done (+4088 3); 0 from ReloadDecals
  uint32_t decals_reloaded_address() const { return address + 5056; }
  uint32_t decals_reloaded_bits() const { if (auto* d = direct()) return d->decals_reloaded; return uint32_t((memory::IsHandle(address + 5056) ? uint32_t(memory::OwnerLoad(address + 5056, 4)) : guest::LoadU32(address + 5056))); }
  void set_decals_reloaded_bits(uint32_t v) const { if (auto* d = direct()) { d->decals_reloaded = v; return; } if (memory::IsHandle(address + 5056)) memory::OwnerStore(address + 5056, 4, v); else guest::StoreU32(address + 5056, v); }
  uint32_t decals_reloaded() const { if (auto* d = direct()) return d->decals_reloaded; return uint32_t((memory::IsHandle(address + 5056) ? uint32_t(memory::OwnerLoad(address + 5056, 4)) : guest::LoadU32(address + 5056))); }
  void set_decals_reloaded(uint32_t v) const { if (auto* d = direct()) { d->decals_reloaded = v; return; } if (memory::IsHandle(address + 5056)) memory::OwnerStore(address + 5056, 4, v); else guest::StoreU32(address + 5056, v); }
  // pool 1
  uint32_t parts_address() const { return address + 5060; }
  uint32_t parts_bits() const { if (auto* d = direct()) return d->parts; return uint32_t((memory::IsHandle(address + 5060) ? uint32_t(memory::OwnerLoad(address + 5060, 4)) : guest::LoadU32(address + 5060))); }
  void set_parts_bits(uint32_t v) const { if (auto* d = direct()) { d->parts = v; return; } if (memory::IsHandle(address + 5060)) memory::OwnerStore(address + 5060, 4, v); else guest::StoreU32(address + 5060, v); }
  template <typename T = SceneParts>
  T parts() const { return T(parts_bits()); }
  void set_parts(const View& w) const { set_parts_bits(w.address); }
  void set_parts(uint32_t v) const { set_parts_bits(v); }
  uint32_t parts_ready_address() const { return address + 5064; }
  uint32_t parts_ready_bits() const { if (auto* d = direct()) return d->parts_ready; return uint32_t((memory::IsHandle(address + 5064) ? uint32_t(memory::OwnerLoad(address + 5064, 4)) : guest::LoadU32(address + 5064))); }
  void set_parts_ready_bits(uint32_t v) const { if (auto* d = direct()) { d->parts_ready = v; return; } if (memory::IsHandle(address + 5064)) memory::OwnerStore(address + 5064, 4, v); else guest::StoreU32(address + 5064, v); }
  uint32_t parts_ready() const { if (auto* d = direct()) return d->parts_ready; return uint32_t((memory::IsHandle(address + 5064) ? uint32_t(memory::OwnerLoad(address + 5064, 4)) : guest::LoadU32(address + 5064))); }
  void set_parts_ready(uint32_t v) const { if (auto* d = direct()) { d->parts_ready = v; return; } if (memory::IsHandle(address + 5064)) memory::OwnerStore(address + 5064, 4, v); else guest::StoreU32(address + 5064, v); }
  // pool 3 (SceneInterior): +36 its first-set interior
  uint32_t interior_address() const { return address + 5068; }
  uint32_t interior_bits() const { if (auto* d = direct()) return d->interior; return uint32_t((memory::IsHandle(address + 5068) ? uint32_t(memory::OwnerLoad(address + 5068, 4)) : guest::LoadU32(address + 5068))); }
  void set_interior_bits(uint32_t v) const { if (auto* d = direct()) { d->interior = v; return; } if (memory::IsHandle(address + 5068)) memory::OwnerStore(address + 5068, 4, v); else guest::StoreU32(address + 5068, v); }
  uint32_t interior() const { if (auto* d = direct()) return d->interior; return uint32_t((memory::IsHandle(address + 5068) ? uint32_t(memory::OwnerLoad(address + 5068, 4)) : guest::LoadU32(address + 5068))); }
  void set_interior(uint32_t v) const { if (auto* d = direct()) { d->interior = v; return; } if (memory::IsHandle(address + 5068)) memory::OwnerStore(address + 5068, 4, v); else guest::StoreU32(address + 5068, v); }
  uint32_t interior_ready_address() const { return address + 5072; }
  uint32_t interior_ready_bits() const { if (auto* d = direct()) return d->interior_ready; return uint32_t((memory::IsHandle(address + 5072) ? uint32_t(memory::OwnerLoad(address + 5072, 4)) : guest::LoadU32(address + 5072))); }
  void set_interior_ready_bits(uint32_t v) const { if (auto* d = direct()) { d->interior_ready = v; return; } if (memory::IsHandle(address + 5072)) memory::OwnerStore(address + 5072, 4, v); else guest::StoreU32(address + 5072, v); }
  uint32_t interior_ready() const { if (auto* d = direct()) return d->interior_ready; return uint32_t((memory::IsHandle(address + 5072) ? uint32_t(memory::OwnerLoad(address + 5072, 4)) : guest::LoadU32(address + 5072))); }
  void set_interior_ready(uint32_t v) const { if (auto* d = direct()) { d->interior_ready = v; return; } if (memory::IsHandle(address + 5072)) memory::OwnerStore(address + 5072, 4, v); else guest::StoreU32(address + 5072, v); }
  // pool 2 (SceneCockpit)
  uint32_t cockpit_address() const { return address + 5076; }
  uint32_t cockpit_bits() const { if (auto* d = direct()) return d->cockpit; return uint32_t((memory::IsHandle(address + 5076) ? uint32_t(memory::OwnerLoad(address + 5076, 4)) : guest::LoadU32(address + 5076))); }
  void set_cockpit_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit = v; return; } if (memory::IsHandle(address + 5076)) memory::OwnerStore(address + 5076, 4, v); else guest::StoreU32(address + 5076, v); }
  uint32_t cockpit() const { if (auto* d = direct()) return d->cockpit; return uint32_t((memory::IsHandle(address + 5076) ? uint32_t(memory::OwnerLoad(address + 5076, 4)) : guest::LoadU32(address + 5076))); }
  void set_cockpit(uint32_t v) const { if (auto* d = direct()) { d->cockpit = v; return; } if (memory::IsHandle(address + 5076)) memory::OwnerStore(address + 5076, 4, v); else guest::StoreU32(address + 5076, v); }
  // 1 when there's no cockpit
  uint32_t cockpit_ready_address() const { return address + 5080; }
  uint32_t cockpit_ready_bits() const { if (auto* d = direct()) return d->cockpit_ready; return uint32_t((memory::IsHandle(address + 5080) ? uint32_t(memory::OwnerLoad(address + 5080, 4)) : guest::LoadU32(address + 5080))); }
  void set_cockpit_ready_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit_ready = v; return; } if (memory::IsHandle(address + 5080)) memory::OwnerStore(address + 5080, 4, v); else guest::StoreU32(address + 5080, v); }
  uint32_t cockpit_ready() const { if (auto* d = direct()) return d->cockpit_ready; return uint32_t((memory::IsHandle(address + 5080) ? uint32_t(memory::OwnerLoad(address + 5080, 4)) : guest::LoadU32(address + 5080))); }
  void set_cockpit_ready(uint32_t v) const { if (auto* d = direct()) { d->cockpit_ready = v; return; } if (memory::IsHandle(address + 5080)) memory::OwnerStore(address + 5080, 4, v); else guest::StoreU32(address + 5080, v); }
  // pool 4
  uint32_t light_kit_address() const { return address + 5084; }
  uint32_t light_kit_bits() const { if (auto* d = direct()) return d->light_kit; return uint32_t((memory::IsHandle(address + 5084) ? uint32_t(memory::OwnerLoad(address + 5084, 4)) : guest::LoadU32(address + 5084))); }
  void set_light_kit_bits(uint32_t v) const { if (auto* d = direct()) { d->light_kit = v; return; } if (memory::IsHandle(address + 5084)) memory::OwnerStore(address + 5084, 4, v); else guest::StoreU32(address + 5084, v); }
  template <typename T = SceneLightKit>
  T light_kit() const { return T(light_kit_bits()); }
  void set_light_kit(const View& w) const { set_light_kit_bits(w.address); }
  void set_light_kit(uint32_t v) const { set_light_kit_bits(v); }
  uint32_t light_kit_ready_address() const { return address + 5088; }
  uint32_t light_kit_ready_bits() const { if (auto* d = direct()) return d->light_kit_ready; return uint32_t((memory::IsHandle(address + 5088) ? uint32_t(memory::OwnerLoad(address + 5088, 4)) : guest::LoadU32(address + 5088))); }
  void set_light_kit_ready_bits(uint32_t v) const { if (auto* d = direct()) { d->light_kit_ready = v; return; } if (memory::IsHandle(address + 5088)) memory::OwnerStore(address + 5088, 4, v); else guest::StoreU32(address + 5088, v); }
  uint32_t light_kit_ready() const { if (auto* d = direct()) return d->light_kit_ready; return uint32_t((memory::IsHandle(address + 5088) ? uint32_t(memory::OwnerLoad(address + 5088, 4)) : guest::LoadU32(address + 5088))); }
  void set_light_kit_ready(uint32_t v) const { if (auto* d = direct()) { d->light_kit_ready = v; return; } if (memory::IsHandle(address + 5088)) memory::OwnerStore(address + 5088, 4, v); else guest::StoreU32(address + 5088, v); }
  // pool 6 (SceneTraffic)
  uint32_t traffic_body_address() const { return address + 5092; }
  uint32_t traffic_body_bits() const { if (auto* d = direct()) return d->traffic_body; return uint32_t((memory::IsHandle(address + 5092) ? uint32_t(memory::OwnerLoad(address + 5092, 4)) : guest::LoadU32(address + 5092))); }
  void set_traffic_body_bits(uint32_t v) const { if (auto* d = direct()) { d->traffic_body = v; return; } if (memory::IsHandle(address + 5092)) memory::OwnerStore(address + 5092, 4, v); else guest::StoreU32(address + 5092, v); }
  uint32_t traffic_body() const { if (auto* d = direct()) return d->traffic_body; return uint32_t((memory::IsHandle(address + 5092) ? uint32_t(memory::OwnerLoad(address + 5092, 4)) : guest::LoadU32(address + 5092))); }
  void set_traffic_body(uint32_t v) const { if (auto* d = direct()) { d->traffic_body = v; return; } if (memory::IsHandle(address + 5092)) memory::OwnerStore(address + 5092, 4, v); else guest::StoreU32(address + 5092, v); }
  uint32_t traffic_body_ready_address() const { return address + 5096; }
  uint32_t traffic_body_ready_bits() const { if (auto* d = direct()) return d->traffic_body_ready; return uint32_t((memory::IsHandle(address + 5096) ? uint32_t(memory::OwnerLoad(address + 5096, 4)) : guest::LoadU32(address + 5096))); }
  void set_traffic_body_ready_bits(uint32_t v) const { if (auto* d = direct()) { d->traffic_body_ready = v; return; } if (memory::IsHandle(address + 5096)) memory::OwnerStore(address + 5096, 4, v); else guest::StoreU32(address + 5096, v); }
  uint32_t traffic_body_ready() const { if (auto* d = direct()) return d->traffic_body_ready; return uint32_t((memory::IsHandle(address + 5096) ? uint32_t(memory::OwnerLoad(address + 5096, 4)) : guest::LoadU32(address + 5096))); }
  void set_traffic_body_ready(uint32_t v) const { if (auto* d = direct()) { d->traffic_body_ready = v; return; } if (memory::IsHandle(address + 5096)) memory::OwnerStore(address + 5096, 4, v); else guest::StoreU32(address + 5096, v); }
  // pool 5 (SceneWheels)
  uint32_t wheels_address() const { return address + 5100; }
  uint32_t wheels_bits() const { if (auto* d = direct()) return d->wheels; return uint32_t((memory::IsHandle(address + 5100) ? uint32_t(memory::OwnerLoad(address + 5100, 4)) : guest::LoadU32(address + 5100))); }
  void set_wheels_bits(uint32_t v) const { if (auto* d = direct()) { d->wheels = v; return; } if (memory::IsHandle(address + 5100)) memory::OwnerStore(address + 5100, 4, v); else guest::StoreU32(address + 5100, v); }
  uint32_t wheels() const { if (auto* d = direct()) return d->wheels; return uint32_t((memory::IsHandle(address + 5100) ? uint32_t(memory::OwnerLoad(address + 5100, 4)) : guest::LoadU32(address + 5100))); }
  void set_wheels(uint32_t v) const { if (auto* d = direct()) { d->wheels = v; return; } if (memory::IsHandle(address + 5100)) memory::OwnerStore(address + 5100, 4, v); else guest::StoreU32(address + 5100, v); }
  uint32_t wheels_ready_address() const { return address + 5104; }
  uint32_t wheels_ready_bits() const { if (auto* d = direct()) return d->wheels_ready; return uint32_t((memory::IsHandle(address + 5104) ? uint32_t(memory::OwnerLoad(address + 5104, 4)) : guest::LoadU32(address + 5104))); }
  void set_wheels_ready_bits(uint32_t v) const { if (auto* d = direct()) { d->wheels_ready = v; return; } if (memory::IsHandle(address + 5104)) memory::OwnerStore(address + 5104, 4, v); else guest::StoreU32(address + 5104, v); }
  uint32_t wheels_ready() const { if (auto* d = direct()) return d->wheels_ready; return uint32_t((memory::IsHandle(address + 5104) ? uint32_t(memory::OwnerLoad(address + 5104, 4)) : guest::LoadU32(address + 5104))); }
  void set_wheels_ready(uint32_t v) const { if (auto* d = direct()) { d->wheels_ready = v; return; } if (memory::IsHandle(address + 5104)) memory::OwnerStore(address + 5104, 4, v); else guest::StoreU32(address + 5104, v); }
  // a second pool 5 object (SceneModel::AddSecondWheels)
  uint32_t second_wheels_address() const { return address + 5108; }
  uint32_t second_wheels_bits() const { if (auto* d = direct()) return d->second_wheels; return uint32_t((memory::IsHandle(address + 5108) ? uint32_t(memory::OwnerLoad(address + 5108, 4)) : guest::LoadU32(address + 5108))); }
  void set_second_wheels_bits(uint32_t v) const { if (auto* d = direct()) { d->second_wheels = v; return; } if (memory::IsHandle(address + 5108)) memory::OwnerStore(address + 5108, 4, v); else guest::StoreU32(address + 5108, v); }
  uint32_t second_wheels() const { if (auto* d = direct()) return d->second_wheels; return uint32_t((memory::IsHandle(address + 5108) ? uint32_t(memory::OwnerLoad(address + 5108, 4)) : guest::LoadU32(address + 5108))); }
  void set_second_wheels(uint32_t v) const { if (auto* d = direct()) { d->second_wheels = v; return; } if (memory::IsHandle(address + 5108)) memory::OwnerStore(address + 5108, 4, v); else guest::StoreU32(address + 5108, v); }
  // the second wheels' finisher answer: once 1 they replace +5100
  uint32_t second_wheels_ready_address() const { return address + 5112; }
  uint32_t second_wheels_ready_bits() const { if (auto* d = direct()) return d->second_wheels_ready; return uint32_t((memory::IsHandle(address + 5112) ? uint32_t(memory::OwnerLoad(address + 5112, 4)) : guest::LoadU32(address + 5112))); }
  void set_second_wheels_ready_bits(uint32_t v) const { if (auto* d = direct()) { d->second_wheels_ready = v; return; } if (memory::IsHandle(address + 5112)) memory::OwnerStore(address + 5112, 4, v); else guest::StoreU32(address + 5112, v); }
  uint32_t second_wheels_ready() const { if (auto* d = direct()) return d->second_wheels_ready; return uint32_t((memory::IsHandle(address + 5112) ? uint32_t(memory::OwnerLoad(address + 5112, 4)) : guest::LoadU32(address + 5112))); }
  void set_second_wheels_ready(uint32_t v) const { if (auto* d = direct()) { d->second_wheels_ready = v; return; } if (memory::IsHandle(address + 5112)) memory::OwnerStore(address + 5112, 4, v); else guest::StoreU32(address + 5112, v); }
  uint32_t scene_address() const { return address + 5116; }
  uint32_t scene_bits() const { if (auto* d = direct()) return d->scene; return uint32_t((memory::IsHandle(address + 5116) ? uint32_t(memory::OwnerLoad(address + 5116, 4)) : guest::LoadU32(address + 5116))); }
  void set_scene_bits(uint32_t v) const { if (auto* d = direct()) { d->scene = v; return; } if (memory::IsHandle(address + 5116)) memory::OwnerStore(address + 5116, 4, v); else guest::StoreU32(address + 5116, v); }
  template <typename T = CarScene>
  T scene() const { return T(scene_bits()); }
  void set_scene(const View& w) const { set_scene_bits(w.address); }
  void set_scene(uint32_t v) const { set_scene_bits(v); }
};

// embedded; measure: CarRecord::Copy.r4, sub_820D45B0.r3, CarRecord::Copy.r3, CarRecord::Reset.r3, SceneModelRecord@+16, SceneModelRecord::FromCar.r3@+16@+3632, WheelPoints::SetUpTire.r3.+4, WheelPoints::DrawTireMorphed.r3.+4
struct CarRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 3696;
  // the model block (traffic: the traffic car's number); -1 after Reset. Measure fault: a layer's +0 (sub_820D0A20) too
  uint32_t block() const { return guest::LoadU32(address + 0); }
  void set_block(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t block_address() const { return address + 0; }
  // measure fault: a layer's +4 and CarRecord::ResetBytes' words (+4 to +44), filed here
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // measure fault: a layer's +8 (the region)
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // measure fault: CarRecord::ResetBytes
  uint32_t unknown_12() const { return guest::LoadU32(address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // three 16-byte hue, saturation, value rows (w 1.0; sub_820D45B0)
  uint32_t paint_address() const { return address + 16; }
  // six rows for the wheels, front then rear
  uint32_t wheel_paint_address() const { return address + 64; }
  // a count or choice passed to the traffic texture name's format
  uint32_t emblem_choice() const { return guest::LoadU32(address + 160); }
  void set_emblem_choice(uint32_t v) const { guest::StoreU32(address + 160, v); }
  uint32_t emblem_choice_address() const { return address + 160; }
  // handed to the team emblem's lookup (sub_820D3CF8) as r5
  uint32_t emblem_word() const { return guest::LoadU32(address + 164); }
  void set_emblem_word(uint32_t v) const { guest::StoreU32(address + 164, v); }
  uint32_t emblem_word_address() const { return address + 164; }
  // three 64-byte vinyl layer records (CarModelDecalRecord)
  uint32_t vinyl_layers_a_address() const { return address + 176; }
  // three more
  uint32_t vinyl_layers_b_address() const { return address + 368; }
  // eight groups of six 64-byte sticker records (384 bytes a group)
  uint32_t stickers_address() const { return address + 560; }
  // twelve words (the number plate: CarRecord::ResetBytes, SetBytes; a 48-byte memcpy in CarRecord::Copy)
  uint32_t plate_address() const { return address + 3632; }
  // which of the car model's XPR indices CarModel::SelectedTexture picks
  uint32_t selected_texture() const { return guest::LoadU32(address + 3680); }
  void set_selected_texture(uint32_t v) const { guest::StoreU32(address + 3680, v); }
  uint32_t selected_texture_address() const { return address + 3680; }
  uint32_t unknown_3684() const { return guest::LoadU32(address + 3684); }
  void set_unknown_3684(uint32_t v) const { guest::StoreU32(address + 3684, v); }
  uint32_t unknown_3684_address() const { return address + 3684; }
  // nonzero the scissor doors (probably)
  uint32_t door_kind() const { return guest::LoadU32(address + 3688); }
  void set_door_kind(uint32_t v) const { guest::StoreU32(address + 3688, v); }
  uint32_t door_kind_address() const { return address + 3688; }
  uint32_t unknown_3692() const { return guest::LoadU32(address + 3692); }
  void set_unknown_3692(uint32_t v) const { guest::StoreU32(address + 3692, v); }
  uint32_t unknown_3692_address() const { return address + 3692; }
};

// embedded; measure: sub_820D34B8.r3
struct SceneModelsCockpitRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  // the parts record's +20 (plus 5 when 6 or more)
  uint32_t bonnet() const { return guest::LoadU32(address + 0); }
  void set_bonnet(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t bonnet_address() const { return address + 0; }
  // the parts record's +24
  uint32_t aero_kit() const { return guest::LoadU32(address + 4); }
  void set_aero_kit(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t aero_kit_address() const { return address + 4; }
  // the parts record's +28
  uint32_t aero6() const { return guest::LoadU32(address + 8); }
  void set_aero6(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t aero6_address() const { return address + 8; }
  // the parts record's +32
  uint32_t aero7() const { return guest::LoadU32(address + 12); }
  void set_aero7(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t aero7_address() const { return address + 12; }
  // the parts record's +40 (plus 7 when 4 or more)
  uint32_t aero9() const { return guest::LoadU32(address + 16); }
  void set_aero9(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t aero9_address() const { return address + 16; }
  // the car record's +3680
  uint32_t selected_texture() const { return guest::LoadU32(address + 20); }
  void set_selected_texture(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t selected_texture_address() const { return address + 20; }
  // its interior part (InteriorRecord), the set-up of the cockpit's own interior
  uint32_t interior_record_address() const { return address + 24; }
};

// embedded; measure: sub_820D3900.r3, WheelPoints::DrawBrakesMorphed.r3.+8, WheelPoints::DrawBrakes.r3.+8, WheelPoints::DrawTireMorphed.r3.+8, WheelPoints::SetUpTire.r3.+8, WheelPoints::SetUpBrakes.r3.+8, WheelPoints::DrawTire.r3.+8, WheelPoints::DrawRimB.r3.+8, WheelPoints::SetUpRimA.r3.+8, WheelPoints::SetUpRimB.r3.+8
struct SceneModelsWheelsRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 156;
  // the car's kind (1 by default)
  uint32_t kind() const { return guest::LoadU32(address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  // 1 after SceneModelRecord::SetPaint
  uint32_t painted() const { return guest::LoadU32(address + 4); }
  void set_painted(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t painted_address() const { return address + 4; }
  // 0 when morphed once at set-up (else at every draw)
  uint32_t morph_each_draw() const { return guest::LoadU32(address + 8); }
  void set_morph_each_draw(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t morph_each_draw_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // the front wheel number (-1 stock)
  uint32_t front_wheel() const { return guest::LoadU32(address + 16); }
  void set_front_wheel(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t front_wheel_address() const { return address + 16; }
  // [0x82002568] by default
  float front_f20() const { return guest::LoadFloat(address + 20); }
  void set_front_f20(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t front_f20_address() const { return address + 20; }
  // 0.5 by default
  float front_f24() const { return guest::LoadFloat(address + 24); }
  void set_front_f24(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t front_f24_address() const { return address + 24; }
  uint32_t unknown_28() const { return guest::LoadU32(address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  // the rear wheel number (-1 stock)
  uint32_t rear_wheel() const { return guest::LoadU32(address + 32); }
  void set_rear_wheel(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t rear_wheel_address() const { return address + 32; }
  float rear_f36() const { return guest::LoadFloat(address + 36); }
  void set_rear_f36(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t rear_f36_address() const { return address + 36; }
  float rear_f40() const { return guest::LoadFloat(address + 40); }
  void set_rear_f40(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t rear_f40_address() const { return address + 40; }
  // the front tire's morph inputs (SceneModelsWheelSize)
  uint32_t front_size_address() const { return address + 44; }
  uint32_t front_size_index() const { return guest::LoadU32(address + 44); }
  void set_front_size_index(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t front_size_index_address() const { return address + 44; }
  float front_size_a() const { return guest::LoadFloat(address + 48); }
  void set_front_size_a(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t front_size_a_address() const { return address + 48; }
  float front_size_b() const { return guest::LoadFloat(address + 52); }
  void set_front_size_b(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t front_size_b_address() const { return address + 52; }
  // the rear tire's (+56 also read as the second rear wheel's offset)
  uint32_t rear_size_address() const { return address + 56; }
  uint32_t rear_size_index() const { return guest::LoadU32(address + 56); }
  void set_rear_size_index(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t rear_size_index_address() const { return address + 56; }
  float rear_size_a() const { return guest::LoadFloat(address + 60); }
  void set_rear_size_a(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t rear_size_a_address() const { return address + 60; }
  float rear_size_b() const { return guest::LoadFloat(address + 64); }
  void set_rear_size_b(float v) const { guest::StoreFloat(address + 64, v); }
  uint32_t rear_size_b_address() const { return address + 64; }
  uint32_t brake_kind() const { return guest::LoadU32(address + 68); }
  void set_brake_kind(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t brake_kind_address() const { return address + 68; }
  uint32_t front_index_add() const { return guest::LoadU32(address + 72); }
  void set_front_index_add(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t front_index_add_address() const { return address + 72; }
  uint32_t rear_index_add() const { return guest::LoadU32(address + 76); }
  void set_rear_index_add(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t rear_index_add_address() const { return address + 76; }
  uint32_t unknown_80() const { return guest::LoadU32(address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  // the calipers
  uint32_t caliper_a() const { return guest::LoadU32(address + 84); }
  void set_caliper_a(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t caliper_a_address() const { return address + 84; }
  // the calipers' texture (probably)
  uint32_t caliper_texture() const { return guest::LoadU32(address + 88); }
  void set_caliper_texture(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t caliper_texture_address() const { return address + 88; }
  uint32_t unknown_92() const { return guest::LoadU32(address + 92); }
  void set_unknown_92(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  uint32_t unknown_96() const { return guest::LoadU32(address + 96); }
  void set_unknown_96(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  // degrees
  float front_brake_turn() const { return guest::LoadFloat(address + 100); }
  void set_front_brake_turn(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t front_brake_turn_address() const { return address + 100; }
  float rear_brake_turn() const { return guest::LoadFloat(address + 104); }
  void set_rear_brake_turn(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t rear_brake_turn_address() const { return address + 104; }
  // 11 by default (probably inches)
  uint32_t front_brake_size() const { return guest::LoadU32(address + 108); }
  void set_front_brake_size(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t front_brake_size_address() const { return address + 108; }
  uint32_t rear_brake_size() const { return guest::LoadU32(address + 112); }
  void set_rear_brake_size(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t rear_brake_size_address() const { return address + 112; }
  uint32_t unknown_116() const { return guest::LoadU32(address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  uint32_t unknown_120() const { return guest::LoadU32(address + 120); }
  void set_unknown_120(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  // -1 by default
  uint32_t unknown_124() const { return guest::LoadU32(address + 124); }
  void set_unknown_124(uint32_t v) const { guest::StoreU32(address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  float unknown_128() const { return guest::LoadFloat(address + 128); }
  void set_unknown_128(float v) const { guest::StoreFloat(address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  float unknown_132() const { return guest::LoadFloat(address + 132); }
  void set_unknown_132(float v) const { guest::StoreFloat(address + 132, v); }
  uint32_t unknown_132_address() const { return address + 132; }
  float unknown_136() const { return guest::LoadFloat(address + 136); }
  void set_unknown_136(float v) const { guest::StoreFloat(address + 136, v); }
  uint32_t unknown_136_address() const { return address + 136; }
  float unknown_140() const { return guest::LoadFloat(address + 140); }
  void set_unknown_140(float v) const { guest::StoreFloat(address + 140, v); }
  uint32_t unknown_140_address() const { return address + 140; }
  float unknown_144() const { return guest::LoadFloat(address + 144); }
  void set_unknown_144(float v) const { guest::StoreFloat(address + 144, v); }
  uint32_t unknown_144_address() const { return address + 144; }
  float unknown_148() const { return guest::LoadFloat(address + 148); }
  void set_unknown_148(float v) const { guest::StoreFloat(address + 148, v); }
  uint32_t unknown_148_address() const { return address + 148; }
  float unknown_152() const { return guest::LoadFloat(address + 152); }
  void set_unknown_152(float v) const { guest::StoreFloat(address + 152, v); }
  uint32_t unknown_152_address() const { return address + 152; }
  // +32 and +36 read as a doubleword (the brakes' draws) (from SecondSetWheelsRecord, second-set)
  uint64_t rear_wheel_f36() const { return guest::LoadU64(address + 32); }
  void set_rear_wheel_f36(uint64_t v) const { guest::StoreU64(address + 32, v); }
  uint32_t rear_wheel_f36_address() const { return address + 32; }
  // +40 and +44 read as a doubleword (from SecondSetWheelsRecord, second-set)
  uint64_t rear_f40_44() const { return guest::LoadU64(address + 40); }
  void set_rear_f40_44(uint64_t v) const { guest::StoreU64(address + 40, v); }
  uint32_t rear_f40_44_address() const { return address + 40; }
};

// embedded; measure: sub_820D3B38.r3
struct SceneModelsWheelSize : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // -1 by default
  uint32_t index() const { return guest::LoadU32(address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t index_address() const { return address + 0; }
  // [0x82001BE0] (probably -1.0) by default
  float a() const { return guest::LoadFloat(address + 4); }
  void set_a(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t a_address() const { return address + 4; }
  float b() const { return guest::LoadFloat(address + 8); }
  void set_b(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t b_address() const { return address + 8; }
};

// allocated; measure: ScenePool4
struct SceneLightKit : View {
  using View::View;
  static constexpr uint32_t kSize = 13168;
  static constexpr uint32_t kVtable = 0x820056E0;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the pool object's: 3 in use
  uint32_t state() const { return guest::LoadU32(address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  uint32_t index() const { return guest::LoadU32(address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t index_address() const { return address + 20; }
  // the second set's car model (sub_820E70B8)
  template <typename T = CarModelsSceneCarModel>
  T car_model() const { return T(guest::LoadU32(address + 24)); }
  void set_car_model(const View& v) const { guest::StoreU32(address + 24, v.address); }
  void set_car_model(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
  // pool 4 of the first set
  template <typename T = Pool4Object>
  T first_set_object() const { return T(guest::LoadU32(address + 28)); }
  void set_first_set_object(const View& v) const { guest::StoreU32(address + 28, v.address); }
  void set_first_set_object(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t first_set_object_address() const { return address + 28; }
  // a copy of its 52-byte record (LightKitRecord)
  uint32_t record_address() const { return address + 32; }
  // the point block (car-draw.toml's LightPoints)
  uint32_t points_address() const { return address + 96; }
  // the first set's object's model (its +112)
  uint32_t model() const { return guest::LoadU32(address + 13152); }
  void set_model(uint32_t v) const { guest::StoreU32(address + 13152, v); }
  uint32_t model_address() const { return address + 13152; }
  // SceneLights::Finish
  uint32_t finished() const { return guest::LoadU32(address + 13156); }
  void set_finished(uint32_t v) const { guest::StoreU32(address + 13156, v); }
  uint32_t finished_address() const { return address + 13156; }
  template <typename T = CarScene>
  T scene() const { return T(guest::LoadU32(address + 13160)); }
  void set_scene(const View& v) const { guest::StoreU32(address + 13160, v.address); }
  void set_scene(uint32_t v) const { guest::StoreU32(address + 13160, v); }
  uint32_t scene_address() const { return address + 13160; }
};

// allocated; measure: sub_820E70B8.r3
struct SceneModelsPartLink : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  template <typename T = CarModelsSceneCarModel>
  T car_model() const { return T(guest::LoadU32(address + 24)); }
  void set_car_model(const View& v) const { guest::StoreU32(address + 24, v.address); }
  void set_car_model(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
};

// embedded; measure: sub_82175EE0.r6
struct SceneModelsSoundContext : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the scene model's slot i
  uint32_t slot() const { return guest::LoadU32(address + 0); }
  void set_slot(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t slot_address() const { return address + 0; }
};

// embedded; measure: CarRecord::Reset.r3@+176
struct CarRecordAt176 : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint64_t unknown_16() const { return guest::LoadU64(address + 16); }
  void set_unknown_16(uint64_t v) const { guest::StoreU64(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint64_t unknown_24() const { return guest::LoadU64(address + 24); }
  void set_unknown_24(uint64_t v) const { guest::StoreU64(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  float unknown_32() const { return guest::LoadFloat(address + 32); }
  void set_unknown_32(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint64_t unknown_48() const { return guest::LoadU64(address + 48); }
  void set_unknown_48(uint64_t v) const { guest::StoreU64(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint64_t unknown_56() const { return guest::LoadU64(address + 56); }
  void set_unknown_56(uint64_t v) const { guest::StoreU64(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
};

// embedded; measure: WheelPoints::DrawTireMorphed.r3.+8@+60
struct SceneModelsWheelsRecordAt60 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float unknown_0() const { return guest::LoadFloat(address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// The image words docs/functions/scene-models.md's code uses.
struct SceneModelsGlobals {
  // the car scene (its second set holds the scene models)
  template <typename T = CarScene>
  static T scene() { return T(guest::LoadU32(0x826C2E44u)); }
  static void set_scene(const View& v) { guest::StoreU32(0x826C2E44u, v.address); }
  static void set_scene(uint32_t v) { guest::StoreU32(0x826C2E44u, v); }
  static constexpr uint32_t scene_address() { return 0x826C2E44u; }
  // the accessories' set (sub_820F0E50 returns it)
  template <typename T = Accessories>
  static T accessories() { return T(guest::LoadU32(0x826C5510u)); }
  static void set_accessories(const View& v) { guest::StoreU32(0x826C5510u, v.address); }
  static void set_accessories(uint32_t v) { guest::StoreU32(0x826C5510u, v); }
  static constexpr uint32_t accessories_address() { return 0x826C5510u; }
  // the sound loads' callback contexts by slot (SceneModelsSoundContext, each holds i)
  static constexpr uint32_t sound_contexts_address() { return 0x826D8334u; }
  // a byte by slot: the files arrived (sub_82175EE0 counts to 2)
  static constexpr uint32_t sound_files_arrived_address() { return 0x826D8350u; }
  // a word by slot: 1 when both files arrived
  static constexpr uint32_t sound_loaded_address() { return 0x826D83ACu; }
  // 1.0
  static float one() { return guest::LoadFloat(0x82000A30u); }
  static void set_one(float v) { guest::StoreFloat(0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero() { return guest::LoadFloat(0x82000A34u); }
  static void set_zero(float v) { guest::StoreFloat(0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // probably -1.0
  static float minus_one() { return guest::LoadFloat(0x82001BE0u); }
  static void set_minus_one(float v) { guest::StoreFloat(0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // 0.5
  static float half() { return guest::LoadFloat(0x82001BE4u); }
  static void set_half(float v) { guest::StoreFloat(0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // the wheels record's +20 and +36 by default
  static float wheel_default() { return guest::LoadFloat(0x82002568u); }
  static void set_wheel_default(float v) { guest::StoreFloat(0x82002568u, v); }
  static constexpr uint32_t wheel_default_address() { return 0x82002568u; }
  // CarRecord::Reset's first block's last row's z
  static float car_record_z0() { return guest::LoadFloat(0x82002A10u); }
  static void set_car_record_z0(float v) { guest::StoreFloat(0x82002A10u, v); }
  static constexpr uint32_t car_record_z0_address() { return 0x82002A10u; }
  // its other blocks' last row's z
  static float car_record_z1() { return guest::LoadFloat(0x82002A88u); }
  static void set_car_record_z1(float v) { guest::StoreFloat(0x82002A88u, v); }
  static constexpr uint32_t car_record_z1_address() { return 0x82002A88u; }
};

}  // namespace schema
