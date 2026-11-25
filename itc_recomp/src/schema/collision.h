// The views of docs/functions/collision.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/collision.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: BoxShape, Shape, sub_820BAB48.r4, sub_820BAB48.ret, sub_820BB688.r3, sub_820BB688.r4, BoxShape::Place.r5, sub_822BE2C8.ret, sub_820911F8.ret, sub_820BA1D0.r3, sub_820B9C30.r3, sub_820BC560.r5, Part464::Restore.r4, Part464::Save.r4, Vehicle.+344, WallQuery.+12.+0, AiDriver.+160.+344, CollisionShape::Place.r3.+8
#pragma pack(push, 1)
struct BoxShapeData {
  uint32_t vtable = 0;  // +0
  uint8_t pad_4[12] = {};
  float car_float_16 = 0;  // +16
  float car_float_20 = 0;  // +20
  float car_float_24 = 0;  // +24
  float friction = 0;  // +28
  float radius = 0;  // +32
  uint8_t pad_36[12] = {};
  float center_x = 0;  // +48
  float center_y = 0;  // +52
  float center_z = 0;  // +56
  float center_w = 0;  // +60
  float axis0_x = 0;  // +64
  float axis0_y = 0;  // +68
  float axis0_z = 0;  // +72
  float axis0_w = 0;  // +76
  float axis1_x = 0;  // +80
  float axis1_y = 0;  // +84
  float axis1_z = 0;  // +88
  float axis1_w = 0;  // +92
  float axis2_x = 0;  // +96
  float axis2_y = 0;  // +100
  float axis2_z = 0;  // +104
  float axis2_w = 0;  // +108
  uint16_t unknown_112 = 0;  // +112
  uint16_t unknown_114 = 0;  // +114
  uint8_t pad_116[12] = {};
  float half_extent_x = 0;  // +128
  float half_extent_y = 0;  // +132
  float half_extent_z = 0;  // +136
  float half_extent_w = 0;  // +140
  float inverse_half_extent_x = 0;  // +144
  float inverse_half_extent_y = 0;  // +148
  float inverse_half_extent_z = 0;  // +152
  float inverse_half_extent_w = 0;  // +156
  uint32_t lower_corner_position_0 = 0;  // +160
  uint32_t lower_corner_position_1 = 0;  // +164
  uint32_t lower_corner_position_2 = 0;  // +168
  uint32_t lower_corner_position_3 = 0;  // +172
  float corner_x = 0;  // +176
  float corner_y = 0;  // +180
  float corner_z = 0;  // +184
  float corner_w = 0;  // +188
  uint8_t pad_192[112] = {};
  float face_px_distance = 0;  // +304
  uint8_t pad_308[12] = {};
  float face_px_normal_x = 0;  // +320
  float face_px_normal_y = 0;  // +324
  float face_px_normal_z = 0;  // +328
  float face_px_normal_w = 0;  // +332
  float face_py_distance = 0;  // +336
  uint8_t pad_340[12] = {};
  float face_py_normal_x = 0;  // +352
  float face_py_normal_y = 0;  // +356
  float face_py_normal_z = 0;  // +360
  float face_py_normal_w = 0;  // +364
  float face_pz_distance = 0;  // +368
  uint8_t pad_372[12] = {};
  float face_pz_normal_x = 0;  // +384
  float face_pz_normal_y = 0;  // +388
  float face_pz_normal_z = 0;  // +392
  float face_pz_normal_w = 0;  // +396
  float face_nx_distance = 0;  // +400
  uint8_t pad_404[12] = {};
  float face_nx_normal_x = 0;  // +416
  float face_nx_normal_y = 0;  // +420
  float face_nx_normal_z = 0;  // +424
  float face_nx_normal_w = 0;  // +428
  float face_ny_distance = 0;  // +432
  uint8_t pad_436[12] = {};
  float face_ny_normal_x = 0;  // +448
  float face_ny_normal_y = 0;  // +452
  float face_ny_normal_z = 0;  // +456
  float face_ny_normal_w = 0;  // +460
  float face_nz_distance = 0;  // +464
  uint8_t pad_468[12] = {};
  float face_nz_normal_x = 0;  // +480
  float face_nz_normal_y = 0;  // +484
  float face_nz_normal_z = 0;  // +488
  float face_nz_normal_w = 0;  // +492
};
#pragma pack(pop)
static_assert(sizeof(BoxShapeData) == 496);
inline constexpr memory::LayoutField kBoxShapeFields[] = {{0, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 2}, {114, 2}, {128, 4}, {132, 4}, {136, 4}, {140, 4}, {144, 4}, {148, 4}, {152, 4}, {156, 4}, {160, 4}, {164, 4}, {168, 4}, {172, 4}, {176, 4}, {180, 4}, {184, 4}, {188, 4}, {304, 4}, {320, 4}, {324, 4}, {328, 4}, {332, 4}, {336, 4}, {352, 4}, {356, 4}, {360, 4}, {364, 4}, {368, 4}, {384, 4}, {388, 4}, {392, 4}, {396, 4}, {400, 4}, {416, 4}, {420, 4}, {424, 4}, {428, 4}, {432, 4}, {448, 4}, {452, 4}, {456, 4}, {460, 4}, {464, 4}, {480, 4}, {484, 4}, {488, 4}, {492, 4}};
inline constexpr memory::Layout kBoxShapeLayout = {kBoxShapeFields, 70};

struct BoxShape : View {
  using View::View;
  static constexpr uint32_t kSize = 496;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  static constexpr uint32_t kVtable = 0x82002B30;
  BoxShapeData* data() const { return static_cast<BoxShapeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static BoxShape New() {
    return BoxShape(memory::MakeHandle(new BoxShapeData(), sizeof(BoxShapeData), "BoxShape", &kBoxShapeLayout));
  }
  template <uint32_t Stride = kSize>
  static BoxShape NewArray(uint32_t count) {
    static_assert(sizeof(BoxShapeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<BoxShapeData*>(::operator new(sizeof(BoxShapeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) BoxShapeData();
    return BoxShape(memory::MakeHandle(p, sizeof(BoxShapeData) * count, "BoxShape", &kBoxShapeLayout, sizeof(BoxShapeData)));
  }
  BoxShape at(uint32_t i) const { return BoxShape(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  BoxShapeData* direct() const { return static_cast<BoxShapeData*>(memory::HandleOwn(address, &kBoxShapeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kBoxShapeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kBoxShapeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82002B30 (a box); 0x82002AA8 (Shape's) once destroyed or before BoxShape::BoxShape sets it
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // set from the car data (Shape::Shape: 0.5)
  uint32_t car_float_16_address() const { return address + 16; }
  uint32_t car_float_16_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->car_float_16); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_car_float_16_bits(uint32_t v) const { if (auto* d = direct()) { d->car_float_16 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  float car_float_16() const { return std::bit_cast<float>(car_float_16_bits()); }
  void set_car_float_16(float v) const { set_car_float_16_bits(std::bit_cast<uint32_t>(v)); }
  // set from the car data (Shape::Shape: 0.5)
  uint32_t car_float_20_address() const { return address + 20; }
  uint32_t car_float_20_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->car_float_20); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_car_float_20_bits(uint32_t v) const { if (auto* d = direct()) { d->car_float_20 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  float car_float_20() const { return std::bit_cast<float>(car_float_20_bits()); }
  void set_car_float_20(float v) const { set_car_float_20_bits(std::bit_cast<uint32_t>(v)); }
  // set from the car data (Shape::Shape: [0x82002968])
  uint32_t car_float_24_address() const { return address + 24; }
  uint32_t car_float_24_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->car_float_24); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_car_float_24_bits(uint32_t v) const { if (auto* d = direct()) { d->car_float_24 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  float car_float_24() const { return std::bit_cast<float>(car_float_24_bits()); }
  void set_car_float_24(float v) const { set_car_float_24_bits(std::bit_cast<uint32_t>(v)); }
  // set from the car data, read by CollisionObject::Respond (Shape::Shape: [0x82002204])
  uint32_t friction_address() const { return address + 28; }
  uint32_t friction_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->friction); return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_friction_bits(uint32_t v) const { if (auto* d = direct()) { d->friction = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  float friction() const { return std::bit_cast<float>(friction_bits()); }
  void set_friction(float v) const { set_friction_bits(std::bit_cast<uint32_t>(v)); }
  // the bounding sphere's radius
  uint32_t radius_address() const { return address + 32; }
  uint32_t radius_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->radius); return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_radius_bits(uint32_t v) const { if (auto* d = direct()) { d->radius = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  float radius() const { return std::bit_cast<float>(radius_bits()); }
  void set_radius(float v) const { set_radius_bits(std::bit_cast<uint32_t>(v)); }
  // the center (w 0.0 from Shape::Shape)
  uint32_t center_x_address() const { return address + 48; }
  uint32_t center_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->center_x); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_center_x_bits(uint32_t v) const { if (auto* d = direct()) { d->center_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  float center_x() const { return std::bit_cast<float>(center_x_bits()); }
  void set_center_x(float v) const { set_center_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t center_y_address() const { return address + 52; }
  uint32_t center_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->center_y); return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_center_y_bits(uint32_t v) const { if (auto* d = direct()) { d->center_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  float center_y() const { return std::bit_cast<float>(center_y_bits()); }
  void set_center_y(float v) const { set_center_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t center_z_address() const { return address + 56; }
  uint32_t center_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->center_z); return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_center_z_bits(uint32_t v) const { if (auto* d = direct()) { d->center_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  float center_z() const { return std::bit_cast<float>(center_z_bits()); }
  void set_center_z(float v) const { set_center_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t center_w_address() const { return address + 60; }
  uint32_t center_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->center_w); return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_center_w_bits(uint32_t v) const { if (auto* d = direct()) { d->center_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  float center_w() const { return std::bit_cast<float>(center_w_bits()); }
  void set_center_w(float v) const { set_center_w_bits(std::bit_cast<uint32_t>(v)); }
  // the axes: three rows (+64, +80, +96), the identity until placed; a placed copy's are the matrix's rows
  uint32_t axis0_x_address() const { return address + 64; }
  uint32_t axis0_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->axis0_x); return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_axis0_x_bits(uint32_t v) const { if (auto* d = direct()) { d->axis0_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  float axis0_x() const { return std::bit_cast<float>(axis0_x_bits()); }
  void set_axis0_x(float v) const { set_axis0_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t axis0_y_address() const { return address + 68; }
  uint32_t axis0_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->axis0_y); return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_axis0_y_bits(uint32_t v) const { if (auto* d = direct()) { d->axis0_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  float axis0_y() const { return std::bit_cast<float>(axis0_y_bits()); }
  void set_axis0_y(float v) const { set_axis0_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t axis0_z_address() const { return address + 72; }
  uint32_t axis0_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->axis0_z); return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_axis0_z_bits(uint32_t v) const { if (auto* d = direct()) { d->axis0_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  float axis0_z() const { return std::bit_cast<float>(axis0_z_bits()); }
  void set_axis0_z(float v) const { set_axis0_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t axis0_w_address() const { return address + 76; }
  uint32_t axis0_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->axis0_w); return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_axis0_w_bits(uint32_t v) const { if (auto* d = direct()) { d->axis0_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  float axis0_w() const { return std::bit_cast<float>(axis0_w_bits()); }
  void set_axis0_w(float v) const { set_axis0_w_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t axis1_x_address() const { return address + 80; }
  uint32_t axis1_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->axis1_x); return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_axis1_x_bits(uint32_t v) const { if (auto* d = direct()) { d->axis1_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  float axis1_x() const { return std::bit_cast<float>(axis1_x_bits()); }
  void set_axis1_x(float v) const { set_axis1_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t axis1_y_address() const { return address + 84; }
  uint32_t axis1_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->axis1_y); return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_axis1_y_bits(uint32_t v) const { if (auto* d = direct()) { d->axis1_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  float axis1_y() const { return std::bit_cast<float>(axis1_y_bits()); }
  void set_axis1_y(float v) const { set_axis1_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t axis1_z_address() const { return address + 88; }
  uint32_t axis1_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->axis1_z); return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_axis1_z_bits(uint32_t v) const { if (auto* d = direct()) { d->axis1_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  float axis1_z() const { return std::bit_cast<float>(axis1_z_bits()); }
  void set_axis1_z(float v) const { set_axis1_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t axis1_w_address() const { return address + 92; }
  uint32_t axis1_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->axis1_w); return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_axis1_w_bits(uint32_t v) const { if (auto* d = direct()) { d->axis1_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  float axis1_w() const { return std::bit_cast<float>(axis1_w_bits()); }
  void set_axis1_w(float v) const { set_axis1_w_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t axis2_x_address() const { return address + 96; }
  uint32_t axis2_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->axis2_x); return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_axis2_x_bits(uint32_t v) const { if (auto* d = direct()) { d->axis2_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  float axis2_x() const { return std::bit_cast<float>(axis2_x_bits()); }
  void set_axis2_x(float v) const { set_axis2_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t axis2_y_address() const { return address + 100; }
  uint32_t axis2_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->axis2_y); return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_axis2_y_bits(uint32_t v) const { if (auto* d = direct()) { d->axis2_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  float axis2_y() const { return std::bit_cast<float>(axis2_y_bits()); }
  void set_axis2_y(float v) const { set_axis2_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t axis2_z_address() const { return address + 104; }
  uint32_t axis2_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->axis2_z); return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_axis2_z_bits(uint32_t v) const { if (auto* d = direct()) { d->axis2_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  float axis2_z() const { return std::bit_cast<float>(axis2_z_bits()); }
  void set_axis2_z(float v) const { set_axis2_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t axis2_w_address() const { return address + 108; }
  uint32_t axis2_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->axis2_w); return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_axis2_w_bits(uint32_t v) const { if (auto* d = direct()) { d->axis2_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  float axis2_w() const { return std::bit_cast<float>(axis2_w_bits()); }
  void set_axis2_w(float v) const { set_axis2_w_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_112_address() const { return address + 112; }
  uint16_t unknown_112_bits() const { if (auto* d = direct()) return d->unknown_112; return uint16_t((memory::IsHandle(address + 112) ? uint16_t(memory::OwnerLoad(address + 112, 2)) : guest::LoadU16(address + 112))); }
  void set_unknown_112_bits(uint16_t v) const { if (auto* d = direct()) { d->unknown_112 = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 2, v); else guest::StoreU16(address + 112, v); }
  uint16_t unknown_112() const { if (auto* d = direct()) return d->unknown_112; return uint16_t((memory::IsHandle(address + 112) ? uint16_t(memory::OwnerLoad(address + 112, 2)) : guest::LoadU16(address + 112))); }
  void set_unknown_112(uint16_t v) const { if (auto* d = direct()) { d->unknown_112 = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 2, v); else guest::StoreU16(address + 112, v); }
  uint32_t unknown_114_address() const { return address + 114; }
  uint16_t unknown_114_bits() const { if (auto* d = direct()) return d->unknown_114; return uint16_t((memory::IsHandle(address + 114) ? uint16_t(memory::OwnerLoad(address + 114, 2)) : guest::LoadU16(address + 114))); }
  void set_unknown_114_bits(uint16_t v) const { if (auto* d = direct()) { d->unknown_114 = v; return; } if (memory::IsHandle(address + 114)) memory::OwnerStore(address + 114, 2, v); else guest::StoreU16(address + 114, v); }
  uint16_t unknown_114() const { if (auto* d = direct()) return d->unknown_114; return uint16_t((memory::IsHandle(address + 114) ? uint16_t(memory::OwnerLoad(address + 114, 2)) : guest::LoadU16(address + 114))); }
  void set_unknown_114(uint16_t v) const { if (auto* d = direct()) { d->unknown_114 = v; return; } if (memory::IsHandle(address + 114)) memory::OwnerStore(address + 114, 2, v); else guest::StoreU16(address + 114, v); }
  // the half extents (RigidBody::Torque, sub_82379328, returns their address)
  uint32_t half_extent_x_address() const { return address + 128; }
  uint32_t half_extent_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->half_extent_x); return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_half_extent_x_bits(uint32_t v) const { if (auto* d = direct()) { d->half_extent_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  float half_extent_x() const { return std::bit_cast<float>(half_extent_x_bits()); }
  void set_half_extent_x(float v) const { set_half_extent_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t half_extent_y_address() const { return address + 132; }
  uint32_t half_extent_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->half_extent_y); return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_half_extent_y_bits(uint32_t v) const { if (auto* d = direct()) { d->half_extent_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  float half_extent_y() const { return std::bit_cast<float>(half_extent_y_bits()); }
  void set_half_extent_y(float v) const { set_half_extent_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t half_extent_z_address() const { return address + 136; }
  uint32_t half_extent_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->half_extent_z); return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_half_extent_z_bits(uint32_t v) const { if (auto* d = direct()) { d->half_extent_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  float half_extent_z() const { return std::bit_cast<float>(half_extent_z_bits()); }
  void set_half_extent_z(float v) const { set_half_extent_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t half_extent_w_address() const { return address + 140; }
  uint32_t half_extent_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->half_extent_w); return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_half_extent_w_bits(uint32_t v) const { if (auto* d = direct()) { d->half_extent_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  float half_extent_w() const { return std::bit_cast<float>(half_extent_w_bits()); }
  void set_half_extent_w(float v) const { set_half_extent_w_bits(std::bit_cast<uint32_t>(v)); }
  // their inverses (sub_820B6890 returns their address)
  uint32_t inverse_half_extent_x_address() const { return address + 144; }
  uint32_t inverse_half_extent_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->inverse_half_extent_x); return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_inverse_half_extent_x_bits(uint32_t v) const { if (auto* d = direct()) { d->inverse_half_extent_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  float inverse_half_extent_x() const { return std::bit_cast<float>(inverse_half_extent_x_bits()); }
  void set_inverse_half_extent_x(float v) const { set_inverse_half_extent_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t inverse_half_extent_y_address() const { return address + 148; }
  uint32_t inverse_half_extent_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->inverse_half_extent_y); return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(address + 148))); }
  void set_inverse_half_extent_y_bits(uint32_t v) const { if (auto* d = direct()) { d->inverse_half_extent_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(address + 148, v); }
  float inverse_half_extent_y() const { return std::bit_cast<float>(inverse_half_extent_y_bits()); }
  void set_inverse_half_extent_y(float v) const { set_inverse_half_extent_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t inverse_half_extent_z_address() const { return address + 152; }
  uint32_t inverse_half_extent_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->inverse_half_extent_z); return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_inverse_half_extent_z_bits(uint32_t v) const { if (auto* d = direct()) { d->inverse_half_extent_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  float inverse_half_extent_z() const { return std::bit_cast<float>(inverse_half_extent_z_bits()); }
  void set_inverse_half_extent_z(float v) const { set_inverse_half_extent_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t inverse_half_extent_w_address() const { return address + 156; }
  uint32_t inverse_half_extent_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->inverse_half_extent_w); return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(address + 156))); }
  void set_inverse_half_extent_w_bits(uint32_t v) const { if (auto* d = direct()) { d->inverse_half_extent_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(address + 156, v); }
  float inverse_half_extent_w() const { return std::bit_cast<float>(inverse_half_extent_w_bits()); }
  void set_inverse_half_extent_w(float v) const { set_inverse_half_extent_w_bits(std::bit_cast<uint32_t>(v)); }
  // the lower corners' track positions (pointers to the vehicle's +5328 + 96i, sub_820C1D08; BoxShape::SetCacheWord's word i, zeroed by SetCorners, copied from placement to placement)
  uint32_t lower_corner_position_0_address() const { return address + 160; }
  uint32_t lower_corner_position_0_bits() const { if (auto* d = direct()) return d->lower_corner_position_0; return uint32_t((memory::IsHandle(address + 160) ? uint32_t(memory::OwnerLoad(address + 160, 4)) : guest::LoadU32(address + 160))); }
  void set_lower_corner_position_0_bits(uint32_t v) const { if (auto* d = direct()) { d->lower_corner_position_0 = v; return; } if (memory::IsHandle(address + 160)) memory::OwnerStore(address + 160, 4, v); else guest::StoreU32(address + 160, v); }
  uint32_t lower_corner_position_0() const { if (auto* d = direct()) return d->lower_corner_position_0; return uint32_t((memory::IsHandle(address + 160) ? uint32_t(memory::OwnerLoad(address + 160, 4)) : guest::LoadU32(address + 160))); }
  void set_lower_corner_position_0(uint32_t v) const { if (auto* d = direct()) { d->lower_corner_position_0 = v; return; } if (memory::IsHandle(address + 160)) memory::OwnerStore(address + 160, 4, v); else guest::StoreU32(address + 160, v); }
  uint32_t lower_corner_position_1_address() const { return address + 164; }
  uint32_t lower_corner_position_1_bits() const { if (auto* d = direct()) return d->lower_corner_position_1; return uint32_t((memory::IsHandle(address + 164) ? uint32_t(memory::OwnerLoad(address + 164, 4)) : guest::LoadU32(address + 164))); }
  void set_lower_corner_position_1_bits(uint32_t v) const { if (auto* d = direct()) { d->lower_corner_position_1 = v; return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 4, v); else guest::StoreU32(address + 164, v); }
  uint32_t lower_corner_position_1() const { if (auto* d = direct()) return d->lower_corner_position_1; return uint32_t((memory::IsHandle(address + 164) ? uint32_t(memory::OwnerLoad(address + 164, 4)) : guest::LoadU32(address + 164))); }
  void set_lower_corner_position_1(uint32_t v) const { if (auto* d = direct()) { d->lower_corner_position_1 = v; return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 4, v); else guest::StoreU32(address + 164, v); }
  uint32_t lower_corner_position_2_address() const { return address + 168; }
  uint32_t lower_corner_position_2_bits() const { if (auto* d = direct()) return d->lower_corner_position_2; return uint32_t((memory::IsHandle(address + 168) ? uint32_t(memory::OwnerLoad(address + 168, 4)) : guest::LoadU32(address + 168))); }
  void set_lower_corner_position_2_bits(uint32_t v) const { if (auto* d = direct()) { d->lower_corner_position_2 = v; return; } if (memory::IsHandle(address + 168)) memory::OwnerStore(address + 168, 4, v); else guest::StoreU32(address + 168, v); }
  uint32_t lower_corner_position_2() const { if (auto* d = direct()) return d->lower_corner_position_2; return uint32_t((memory::IsHandle(address + 168) ? uint32_t(memory::OwnerLoad(address + 168, 4)) : guest::LoadU32(address + 168))); }
  void set_lower_corner_position_2(uint32_t v) const { if (auto* d = direct()) { d->lower_corner_position_2 = v; return; } if (memory::IsHandle(address + 168)) memory::OwnerStore(address + 168, 4, v); else guest::StoreU32(address + 168, v); }
  uint32_t lower_corner_position_3_address() const { return address + 172; }
  uint32_t lower_corner_position_3_bits() const { if (auto* d = direct()) return d->lower_corner_position_3; return uint32_t((memory::IsHandle(address + 172) ? uint32_t(memory::OwnerLoad(address + 172, 4)) : guest::LoadU32(address + 172))); }
  void set_lower_corner_position_3_bits(uint32_t v) const { if (auto* d = direct()) { d->lower_corner_position_3 = v; return; } if (memory::IsHandle(address + 172)) memory::OwnerStore(address + 172, 4, v); else guest::StoreU32(address + 172, v); }
  uint32_t lower_corner_position_3() const { if (auto* d = direct()) return d->lower_corner_position_3; return uint32_t((memory::IsHandle(address + 172) ? uint32_t(memory::OwnerLoad(address + 172, 4)) : guest::LoadU32(address + 172))); }
  void set_lower_corner_position_3(uint32_t v) const { if (auto* d = direct()) { d->lower_corner_position_3 = v; return; } if (memory::IsHandle(address + 172)) memory::OwnerStore(address + 172, 4, v); else guest::StoreU32(address + 172, v); }
  // the first two, as BoxShape::BoxShape(const&) copies them
  uint32_t lower_corner_positions_01_address() const { return address + 160; }
  uint64_t lower_corner_positions_01_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kBoxShapeLayout, 160, 8)); return uint64_t((memory::IsHandle(address + 160) ? uint64_t(memory::OwnerLoad(address + 160, 8)) : guest::LoadU64(address + 160))); }
  void set_lower_corner_positions_01_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kBoxShapeLayout, 160, 8, v); return; } if (memory::IsHandle(address + 160)) memory::OwnerStore(address + 160, 8, v); else guest::StoreU64(address + 160, v); }
  uint64_t lower_corner_positions_01() const { return uint64_t(lower_corner_positions_01_bits()); }
  void set_lower_corner_positions_01(uint64_t v) const { set_lower_corner_positions_01_bits(uint64_t(v)); }
  uint32_t lower_corner_positions_23_address() const { return address + 168; }
  uint64_t lower_corner_positions_23_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kBoxShapeLayout, 168, 8)); return uint64_t((memory::IsHandle(address + 168) ? uint64_t(memory::OwnerLoad(address + 168, 8)) : guest::LoadU64(address + 168))); }
  void set_lower_corner_positions_23_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kBoxShapeLayout, 168, 8, v); return; } if (memory::IsHandle(address + 168)) memory::OwnerStore(address + 168, 8, v); else guest::StoreU64(address + 168, v); }
  uint64_t lower_corner_positions_23() const { return uint64_t(lower_corner_positions_23_bits()); }
  void set_lower_corner_positions_23(uint64_t v) const { set_lower_corner_positions_23_bits(uint64_t(v)); }
  // the 8 corners: corner 0's (the max); corner i at +16i takes the min's x, y, z where bit 0, 1, 2 of i is set, else the max's (corner 7 at +288 the min)
  uint32_t corner_x_address() const { return address + 176; }
  uint32_t corner_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->corner_x); return uint32_t((memory::IsHandle(address + 176) ? uint32_t(memory::OwnerLoad(address + 176, 4)) : guest::LoadU32(address + 176))); }
  void set_corner_x_bits(uint32_t v) const { if (auto* d = direct()) { d->corner_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 176)) memory::OwnerStore(address + 176, 4, v); else guest::StoreU32(address + 176, v); }
  float corner_x() const { return std::bit_cast<float>(corner_x_bits()); }
  void set_corner_x(float v) const { set_corner_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t corner_y_address() const { return address + 180; }
  uint32_t corner_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->corner_y); return uint32_t((memory::IsHandle(address + 180) ? uint32_t(memory::OwnerLoad(address + 180, 4)) : guest::LoadU32(address + 180))); }
  void set_corner_y_bits(uint32_t v) const { if (auto* d = direct()) { d->corner_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 4, v); else guest::StoreU32(address + 180, v); }
  float corner_y() const { return std::bit_cast<float>(corner_y_bits()); }
  void set_corner_y(float v) const { set_corner_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t corner_z_address() const { return address + 184; }
  uint32_t corner_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->corner_z); return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(address + 184))); }
  void set_corner_z_bits(uint32_t v) const { if (auto* d = direct()) { d->corner_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(address + 184, v); }
  float corner_z() const { return std::bit_cast<float>(corner_z_bits()); }
  void set_corner_z(float v) const { set_corner_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t corner_w_address() const { return address + 188; }
  uint32_t corner_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->corner_w); return uint32_t((memory::IsHandle(address + 188) ? uint32_t(memory::OwnerLoad(address + 188, 4)) : guest::LoadU32(address + 188))); }
  void set_corner_w_bits(uint32_t v) const { if (auto* d = direct()) { d->corner_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 4, v); else guest::StoreU32(address + 188, v); }
  float corner_w() const { return std::bit_cast<float>(corner_w_bits()); }
  void set_corner_w(float v) const { set_corner_w_bits(std::bit_cast<uint32_t>(v)); }
  // the 6 faces, 32 bytes each (BoxShape::Face): +x, +y, +z, -x, -y, -z; the distance, then the outward normal at +16
  uint32_t face_px_distance_address() const { return address + 304; }
  uint32_t face_px_distance_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_px_distance); return uint32_t((memory::IsHandle(address + 304) ? uint32_t(memory::OwnerLoad(address + 304, 4)) : guest::LoadU32(address + 304))); }
  void set_face_px_distance_bits(uint32_t v) const { if (auto* d = direct()) { d->face_px_distance = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 304)) memory::OwnerStore(address + 304, 4, v); else guest::StoreU32(address + 304, v); }
  float face_px_distance() const { return std::bit_cast<float>(face_px_distance_bits()); }
  void set_face_px_distance(float v) const { set_face_px_distance_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_px_normal_x_address() const { return address + 320; }
  uint32_t face_px_normal_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_px_normal_x); return uint32_t((memory::IsHandle(address + 320) ? uint32_t(memory::OwnerLoad(address + 320, 4)) : guest::LoadU32(address + 320))); }
  void set_face_px_normal_x_bits(uint32_t v) const { if (auto* d = direct()) { d->face_px_normal_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 320)) memory::OwnerStore(address + 320, 4, v); else guest::StoreU32(address + 320, v); }
  float face_px_normal_x() const { return std::bit_cast<float>(face_px_normal_x_bits()); }
  void set_face_px_normal_x(float v) const { set_face_px_normal_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_px_normal_y_address() const { return address + 324; }
  uint32_t face_px_normal_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_px_normal_y); return uint32_t((memory::IsHandle(address + 324) ? uint32_t(memory::OwnerLoad(address + 324, 4)) : guest::LoadU32(address + 324))); }
  void set_face_px_normal_y_bits(uint32_t v) const { if (auto* d = direct()) { d->face_px_normal_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 324)) memory::OwnerStore(address + 324, 4, v); else guest::StoreU32(address + 324, v); }
  float face_px_normal_y() const { return std::bit_cast<float>(face_px_normal_y_bits()); }
  void set_face_px_normal_y(float v) const { set_face_px_normal_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_px_normal_z_address() const { return address + 328; }
  uint32_t face_px_normal_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_px_normal_z); return uint32_t((memory::IsHandle(address + 328) ? uint32_t(memory::OwnerLoad(address + 328, 4)) : guest::LoadU32(address + 328))); }
  void set_face_px_normal_z_bits(uint32_t v) const { if (auto* d = direct()) { d->face_px_normal_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 328)) memory::OwnerStore(address + 328, 4, v); else guest::StoreU32(address + 328, v); }
  float face_px_normal_z() const { return std::bit_cast<float>(face_px_normal_z_bits()); }
  void set_face_px_normal_z(float v) const { set_face_px_normal_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_px_normal_w_address() const { return address + 332; }
  uint32_t face_px_normal_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_px_normal_w); return uint32_t((memory::IsHandle(address + 332) ? uint32_t(memory::OwnerLoad(address + 332, 4)) : guest::LoadU32(address + 332))); }
  void set_face_px_normal_w_bits(uint32_t v) const { if (auto* d = direct()) { d->face_px_normal_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 332)) memory::OwnerStore(address + 332, 4, v); else guest::StoreU32(address + 332, v); }
  float face_px_normal_w() const { return std::bit_cast<float>(face_px_normal_w_bits()); }
  void set_face_px_normal_w(float v) const { set_face_px_normal_w_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_py_distance_address() const { return address + 336; }
  uint32_t face_py_distance_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_py_distance); return uint32_t((memory::IsHandle(address + 336) ? uint32_t(memory::OwnerLoad(address + 336, 4)) : guest::LoadU32(address + 336))); }
  void set_face_py_distance_bits(uint32_t v) const { if (auto* d = direct()) { d->face_py_distance = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 336)) memory::OwnerStore(address + 336, 4, v); else guest::StoreU32(address + 336, v); }
  float face_py_distance() const { return std::bit_cast<float>(face_py_distance_bits()); }
  void set_face_py_distance(float v) const { set_face_py_distance_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_py_normal_x_address() const { return address + 352; }
  uint32_t face_py_normal_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_py_normal_x); return uint32_t((memory::IsHandle(address + 352) ? uint32_t(memory::OwnerLoad(address + 352, 4)) : guest::LoadU32(address + 352))); }
  void set_face_py_normal_x_bits(uint32_t v) const { if (auto* d = direct()) { d->face_py_normal_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 352)) memory::OwnerStore(address + 352, 4, v); else guest::StoreU32(address + 352, v); }
  float face_py_normal_x() const { return std::bit_cast<float>(face_py_normal_x_bits()); }
  void set_face_py_normal_x(float v) const { set_face_py_normal_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_py_normal_y_address() const { return address + 356; }
  uint32_t face_py_normal_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_py_normal_y); return uint32_t((memory::IsHandle(address + 356) ? uint32_t(memory::OwnerLoad(address + 356, 4)) : guest::LoadU32(address + 356))); }
  void set_face_py_normal_y_bits(uint32_t v) const { if (auto* d = direct()) { d->face_py_normal_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 356)) memory::OwnerStore(address + 356, 4, v); else guest::StoreU32(address + 356, v); }
  float face_py_normal_y() const { return std::bit_cast<float>(face_py_normal_y_bits()); }
  void set_face_py_normal_y(float v) const { set_face_py_normal_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_py_normal_z_address() const { return address + 360; }
  uint32_t face_py_normal_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_py_normal_z); return uint32_t((memory::IsHandle(address + 360) ? uint32_t(memory::OwnerLoad(address + 360, 4)) : guest::LoadU32(address + 360))); }
  void set_face_py_normal_z_bits(uint32_t v) const { if (auto* d = direct()) { d->face_py_normal_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 360)) memory::OwnerStore(address + 360, 4, v); else guest::StoreU32(address + 360, v); }
  float face_py_normal_z() const { return std::bit_cast<float>(face_py_normal_z_bits()); }
  void set_face_py_normal_z(float v) const { set_face_py_normal_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_py_normal_w_address() const { return address + 364; }
  uint32_t face_py_normal_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_py_normal_w); return uint32_t((memory::IsHandle(address + 364) ? uint32_t(memory::OwnerLoad(address + 364, 4)) : guest::LoadU32(address + 364))); }
  void set_face_py_normal_w_bits(uint32_t v) const { if (auto* d = direct()) { d->face_py_normal_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 364)) memory::OwnerStore(address + 364, 4, v); else guest::StoreU32(address + 364, v); }
  float face_py_normal_w() const { return std::bit_cast<float>(face_py_normal_w_bits()); }
  void set_face_py_normal_w(float v) const { set_face_py_normal_w_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_pz_distance_address() const { return address + 368; }
  uint32_t face_pz_distance_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_pz_distance); return uint32_t((memory::IsHandle(address + 368) ? uint32_t(memory::OwnerLoad(address + 368, 4)) : guest::LoadU32(address + 368))); }
  void set_face_pz_distance_bits(uint32_t v) const { if (auto* d = direct()) { d->face_pz_distance = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 368)) memory::OwnerStore(address + 368, 4, v); else guest::StoreU32(address + 368, v); }
  float face_pz_distance() const { return std::bit_cast<float>(face_pz_distance_bits()); }
  void set_face_pz_distance(float v) const { set_face_pz_distance_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_pz_normal_x_address() const { return address + 384; }
  uint32_t face_pz_normal_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_pz_normal_x); return uint32_t((memory::IsHandle(address + 384) ? uint32_t(memory::OwnerLoad(address + 384, 4)) : guest::LoadU32(address + 384))); }
  void set_face_pz_normal_x_bits(uint32_t v) const { if (auto* d = direct()) { d->face_pz_normal_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 384)) memory::OwnerStore(address + 384, 4, v); else guest::StoreU32(address + 384, v); }
  float face_pz_normal_x() const { return std::bit_cast<float>(face_pz_normal_x_bits()); }
  void set_face_pz_normal_x(float v) const { set_face_pz_normal_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_pz_normal_y_address() const { return address + 388; }
  uint32_t face_pz_normal_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_pz_normal_y); return uint32_t((memory::IsHandle(address + 388) ? uint32_t(memory::OwnerLoad(address + 388, 4)) : guest::LoadU32(address + 388))); }
  void set_face_pz_normal_y_bits(uint32_t v) const { if (auto* d = direct()) { d->face_pz_normal_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 388)) memory::OwnerStore(address + 388, 4, v); else guest::StoreU32(address + 388, v); }
  float face_pz_normal_y() const { return std::bit_cast<float>(face_pz_normal_y_bits()); }
  void set_face_pz_normal_y(float v) const { set_face_pz_normal_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_pz_normal_z_address() const { return address + 392; }
  uint32_t face_pz_normal_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_pz_normal_z); return uint32_t((memory::IsHandle(address + 392) ? uint32_t(memory::OwnerLoad(address + 392, 4)) : guest::LoadU32(address + 392))); }
  void set_face_pz_normal_z_bits(uint32_t v) const { if (auto* d = direct()) { d->face_pz_normal_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 392)) memory::OwnerStore(address + 392, 4, v); else guest::StoreU32(address + 392, v); }
  float face_pz_normal_z() const { return std::bit_cast<float>(face_pz_normal_z_bits()); }
  void set_face_pz_normal_z(float v) const { set_face_pz_normal_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_pz_normal_w_address() const { return address + 396; }
  uint32_t face_pz_normal_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_pz_normal_w); return uint32_t((memory::IsHandle(address + 396) ? uint32_t(memory::OwnerLoad(address + 396, 4)) : guest::LoadU32(address + 396))); }
  void set_face_pz_normal_w_bits(uint32_t v) const { if (auto* d = direct()) { d->face_pz_normal_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 396)) memory::OwnerStore(address + 396, 4, v); else guest::StoreU32(address + 396, v); }
  float face_pz_normal_w() const { return std::bit_cast<float>(face_pz_normal_w_bits()); }
  void set_face_pz_normal_w(float v) const { set_face_pz_normal_w_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_nx_distance_address() const { return address + 400; }
  uint32_t face_nx_distance_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_nx_distance); return uint32_t((memory::IsHandle(address + 400) ? uint32_t(memory::OwnerLoad(address + 400, 4)) : guest::LoadU32(address + 400))); }
  void set_face_nx_distance_bits(uint32_t v) const { if (auto* d = direct()) { d->face_nx_distance = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 400)) memory::OwnerStore(address + 400, 4, v); else guest::StoreU32(address + 400, v); }
  float face_nx_distance() const { return std::bit_cast<float>(face_nx_distance_bits()); }
  void set_face_nx_distance(float v) const { set_face_nx_distance_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_nx_normal_x_address() const { return address + 416; }
  uint32_t face_nx_normal_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_nx_normal_x); return uint32_t((memory::IsHandle(address + 416) ? uint32_t(memory::OwnerLoad(address + 416, 4)) : guest::LoadU32(address + 416))); }
  void set_face_nx_normal_x_bits(uint32_t v) const { if (auto* d = direct()) { d->face_nx_normal_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 416)) memory::OwnerStore(address + 416, 4, v); else guest::StoreU32(address + 416, v); }
  float face_nx_normal_x() const { return std::bit_cast<float>(face_nx_normal_x_bits()); }
  void set_face_nx_normal_x(float v) const { set_face_nx_normal_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_nx_normal_y_address() const { return address + 420; }
  uint32_t face_nx_normal_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_nx_normal_y); return uint32_t((memory::IsHandle(address + 420) ? uint32_t(memory::OwnerLoad(address + 420, 4)) : guest::LoadU32(address + 420))); }
  void set_face_nx_normal_y_bits(uint32_t v) const { if (auto* d = direct()) { d->face_nx_normal_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 420)) memory::OwnerStore(address + 420, 4, v); else guest::StoreU32(address + 420, v); }
  float face_nx_normal_y() const { return std::bit_cast<float>(face_nx_normal_y_bits()); }
  void set_face_nx_normal_y(float v) const { set_face_nx_normal_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_nx_normal_z_address() const { return address + 424; }
  uint32_t face_nx_normal_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_nx_normal_z); return uint32_t((memory::IsHandle(address + 424) ? uint32_t(memory::OwnerLoad(address + 424, 4)) : guest::LoadU32(address + 424))); }
  void set_face_nx_normal_z_bits(uint32_t v) const { if (auto* d = direct()) { d->face_nx_normal_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 424)) memory::OwnerStore(address + 424, 4, v); else guest::StoreU32(address + 424, v); }
  float face_nx_normal_z() const { return std::bit_cast<float>(face_nx_normal_z_bits()); }
  void set_face_nx_normal_z(float v) const { set_face_nx_normal_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_nx_normal_w_address() const { return address + 428; }
  uint32_t face_nx_normal_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_nx_normal_w); return uint32_t((memory::IsHandle(address + 428) ? uint32_t(memory::OwnerLoad(address + 428, 4)) : guest::LoadU32(address + 428))); }
  void set_face_nx_normal_w_bits(uint32_t v) const { if (auto* d = direct()) { d->face_nx_normal_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 428)) memory::OwnerStore(address + 428, 4, v); else guest::StoreU32(address + 428, v); }
  float face_nx_normal_w() const { return std::bit_cast<float>(face_nx_normal_w_bits()); }
  void set_face_nx_normal_w(float v) const { set_face_nx_normal_w_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_ny_distance_address() const { return address + 432; }
  uint32_t face_ny_distance_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_ny_distance); return uint32_t((memory::IsHandle(address + 432) ? uint32_t(memory::OwnerLoad(address + 432, 4)) : guest::LoadU32(address + 432))); }
  void set_face_ny_distance_bits(uint32_t v) const { if (auto* d = direct()) { d->face_ny_distance = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 432)) memory::OwnerStore(address + 432, 4, v); else guest::StoreU32(address + 432, v); }
  float face_ny_distance() const { return std::bit_cast<float>(face_ny_distance_bits()); }
  void set_face_ny_distance(float v) const { set_face_ny_distance_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_ny_normal_x_address() const { return address + 448; }
  uint32_t face_ny_normal_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_ny_normal_x); return uint32_t((memory::IsHandle(address + 448) ? uint32_t(memory::OwnerLoad(address + 448, 4)) : guest::LoadU32(address + 448))); }
  void set_face_ny_normal_x_bits(uint32_t v) const { if (auto* d = direct()) { d->face_ny_normal_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 448)) memory::OwnerStore(address + 448, 4, v); else guest::StoreU32(address + 448, v); }
  float face_ny_normal_x() const { return std::bit_cast<float>(face_ny_normal_x_bits()); }
  void set_face_ny_normal_x(float v) const { set_face_ny_normal_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_ny_normal_y_address() const { return address + 452; }
  uint32_t face_ny_normal_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_ny_normal_y); return uint32_t((memory::IsHandle(address + 452) ? uint32_t(memory::OwnerLoad(address + 452, 4)) : guest::LoadU32(address + 452))); }
  void set_face_ny_normal_y_bits(uint32_t v) const { if (auto* d = direct()) { d->face_ny_normal_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 452)) memory::OwnerStore(address + 452, 4, v); else guest::StoreU32(address + 452, v); }
  float face_ny_normal_y() const { return std::bit_cast<float>(face_ny_normal_y_bits()); }
  void set_face_ny_normal_y(float v) const { set_face_ny_normal_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_ny_normal_z_address() const { return address + 456; }
  uint32_t face_ny_normal_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_ny_normal_z); return uint32_t((memory::IsHandle(address + 456) ? uint32_t(memory::OwnerLoad(address + 456, 4)) : guest::LoadU32(address + 456))); }
  void set_face_ny_normal_z_bits(uint32_t v) const { if (auto* d = direct()) { d->face_ny_normal_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 456)) memory::OwnerStore(address + 456, 4, v); else guest::StoreU32(address + 456, v); }
  float face_ny_normal_z() const { return std::bit_cast<float>(face_ny_normal_z_bits()); }
  void set_face_ny_normal_z(float v) const { set_face_ny_normal_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_ny_normal_w_address() const { return address + 460; }
  uint32_t face_ny_normal_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_ny_normal_w); return uint32_t((memory::IsHandle(address + 460) ? uint32_t(memory::OwnerLoad(address + 460, 4)) : guest::LoadU32(address + 460))); }
  void set_face_ny_normal_w_bits(uint32_t v) const { if (auto* d = direct()) { d->face_ny_normal_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 460)) memory::OwnerStore(address + 460, 4, v); else guest::StoreU32(address + 460, v); }
  float face_ny_normal_w() const { return std::bit_cast<float>(face_ny_normal_w_bits()); }
  void set_face_ny_normal_w(float v) const { set_face_ny_normal_w_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_nz_distance_address() const { return address + 464; }
  uint32_t face_nz_distance_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_nz_distance); return uint32_t((memory::IsHandle(address + 464) ? uint32_t(memory::OwnerLoad(address + 464, 4)) : guest::LoadU32(address + 464))); }
  void set_face_nz_distance_bits(uint32_t v) const { if (auto* d = direct()) { d->face_nz_distance = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 464)) memory::OwnerStore(address + 464, 4, v); else guest::StoreU32(address + 464, v); }
  float face_nz_distance() const { return std::bit_cast<float>(face_nz_distance_bits()); }
  void set_face_nz_distance(float v) const { set_face_nz_distance_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_nz_normal_x_address() const { return address + 480; }
  uint32_t face_nz_normal_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_nz_normal_x); return uint32_t((memory::IsHandle(address + 480) ? uint32_t(memory::OwnerLoad(address + 480, 4)) : guest::LoadU32(address + 480))); }
  void set_face_nz_normal_x_bits(uint32_t v) const { if (auto* d = direct()) { d->face_nz_normal_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 480)) memory::OwnerStore(address + 480, 4, v); else guest::StoreU32(address + 480, v); }
  float face_nz_normal_x() const { return std::bit_cast<float>(face_nz_normal_x_bits()); }
  void set_face_nz_normal_x(float v) const { set_face_nz_normal_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_nz_normal_y_address() const { return address + 484; }
  uint32_t face_nz_normal_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_nz_normal_y); return uint32_t((memory::IsHandle(address + 484) ? uint32_t(memory::OwnerLoad(address + 484, 4)) : guest::LoadU32(address + 484))); }
  void set_face_nz_normal_y_bits(uint32_t v) const { if (auto* d = direct()) { d->face_nz_normal_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 484)) memory::OwnerStore(address + 484, 4, v); else guest::StoreU32(address + 484, v); }
  float face_nz_normal_y() const { return std::bit_cast<float>(face_nz_normal_y_bits()); }
  void set_face_nz_normal_y(float v) const { set_face_nz_normal_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_nz_normal_z_address() const { return address + 488; }
  uint32_t face_nz_normal_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_nz_normal_z); return uint32_t((memory::IsHandle(address + 488) ? uint32_t(memory::OwnerLoad(address + 488, 4)) : guest::LoadU32(address + 488))); }
  void set_face_nz_normal_z_bits(uint32_t v) const { if (auto* d = direct()) { d->face_nz_normal_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 488)) memory::OwnerStore(address + 488, 4, v); else guest::StoreU32(address + 488, v); }
  float face_nz_normal_z() const { return std::bit_cast<float>(face_nz_normal_z_bits()); }
  void set_face_nz_normal_z(float v) const { set_face_nz_normal_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t face_nz_normal_w_address() const { return address + 492; }
  uint32_t face_nz_normal_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->face_nz_normal_w); return uint32_t((memory::IsHandle(address + 492) ? uint32_t(memory::OwnerLoad(address + 492, 4)) : guest::LoadU32(address + 492))); }
  void set_face_nz_normal_w_bits(uint32_t v) const { if (auto* d = direct()) { d->face_nz_normal_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 492)) memory::OwnerStore(address + 492, 4, v); else guest::StoreU32(address + 492, v); }
  float face_nz_normal_w() const { return std::bit_cast<float>(face_nz_normal_w_bits()); }
  void set_face_nz_normal_w(float v) const { set_face_nz_normal_w_bits(std::bit_cast<uint32_t>(v)); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t center_address() const { return address + 48; }
  uint8_t* center() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 48 : guest::Pointer(address + 48); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t axis0_address() const { return address + 64; }
  uint8_t* axis0() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 64 : guest::Pointer(address + 64); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t axis1_address() const { return address + 80; }
  uint8_t* axis1() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 80 : guest::Pointer(address + 80); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t axis2_address() const { return address + 96; }
  uint8_t* axis2() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 96 : guest::Pointer(address + 96); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t half_extent_address() const { return address + 128; }
  uint8_t* half_extent() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 128 : guest::Pointer(address + 128); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t inverse_half_extent_address() const { return address + 144; }
  uint8_t* inverse_half_extent() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 144 : guest::Pointer(address + 144); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t corners_address() const { return address + 176; }
  uint8_t* corners() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 176 : guest::Pointer(address + 176); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t face_px_normal_address() const { return address + 320; }
  uint8_t* face_px_normal() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 320 : guest::Pointer(address + 320); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t face_py_normal_address() const { return address + 352; }
  uint8_t* face_py_normal() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 352 : guest::Pointer(address + 352); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t face_pz_normal_address() const { return address + 384; }
  uint8_t* face_pz_normal() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 384 : guest::Pointer(address + 384); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t face_nx_normal_address() const { return address + 416; }
  uint8_t* face_nx_normal() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 416 : guest::Pointer(address + 416); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t face_ny_normal_address() const { return address + 448; }
  uint8_t* face_ny_normal() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 448 : guest::Pointer(address + 448); }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t face_nz_normal_address() const { return address + 480; }
  uint8_t* face_nz_normal() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 480 : guest::Pointer(address + 480); }
};

// image; measure: BoxShape.+0, BoxShape::Place.r5.+0, sub_822BE2C8.ret.+0, sub_820911F8.ret.+0, sub_820BC560.r5.+0, CollisionShape::Place.r3.+8.+0, CollisionShape.+0, CollisionObject@+32.+0
struct BoxShapeVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  // sub_820BB140 (Shape::DeletingDestructor)
  uint32_t deleting_destructor() const { return guest::LoadU32(address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  // sub_820BB410
  uint32_t slot_1() const { return guest::LoadU32(address + 4); }
  void set_slot_1(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t slot_1_address() const { return address + 4; }
  // sub_820BA888
  uint32_t slot_2() const { return guest::LoadU32(address + 8); }
  void set_slot_2(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t slot_2_address() const { return address + 8; }
  // sub_820BB450
  uint32_t slot_3() const { return guest::LoadU32(address + 12); }
  void set_slot_3(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t slot_3_address() const { return address + 12; }
  // sub_8244A528: 2 for a box
  uint32_t type() const { return guest::LoadU32(address + 16); }
  void set_type(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t type_address() const { return address + 16; }
  // sub_820BB218 (BoxShape::Clone)
  uint32_t clone() const { return guest::LoadU32(address + 20); }
  void set_clone(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t clone_address() const { return address + 20; }
  // sub_820BB270
  uint32_t slot_6() const { return guest::LoadU32(address + 24); }
  void set_slot_6(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t slot_6_address() const { return address + 24; }
  // sub_820B9FF8
  uint32_t slot_7() const { return guest::LoadU32(address + 28); }
  void set_slot_7(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t slot_7_address() const { return address + 28; }
  // sub_820BA1D0
  uint32_t slot_8() const { return guest::LoadU32(address + 32); }
  void set_slot_8(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t slot_8_address() const { return address + 32; }
  // sub_820B9C40 (BoxShape::Place)
  uint32_t place() const { return guest::LoadU32(address + 36); }
  void set_place(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t place_address() const { return address + 36; }
  // sub_820BA2F0 (BoxShape::Translate)
  uint32_t translate() const { return guest::LoadU32(address + 40); }
  void set_translate(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t translate_address() const { return address + 40; }
  // sub_820B9EC8, probably
  uint32_t corner_count() const { return guest::LoadU32(address + 44); }
  void set_corner_count(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t corner_count_address() const { return address + 44; }
  // sub_820B9ED0, probably
  uint32_t corner() const { return guest::LoadU32(address + 48); }
  void set_corner(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t corner_address() const { return address + 48; }
  // sub_820B9EE0, probably
  uint32_t face_count() const { return guest::LoadU32(address + 52); }
  void set_face_count(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t face_count_address() const { return address + 52; }
  // sub_820B9EE8, probably
  uint32_t face() const { return guest::LoadU32(address + 56); }
  void set_face(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t face_address() const { return address + 56; }
  // sub_820B9EF8, probably
  uint32_t edge_count() const { return guest::LoadU32(address + 60); }
  void set_edge_count(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t edge_count_address() const { return address + 60; }
  // sub_820B9F00, probably
  uint32_t face_direction() const { return guest::LoadU32(address + 64); }
  void set_face_direction(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t face_direction_address() const { return address + 64; }
  // sub_820B9F98, probably
  uint32_t edge_start() const { return guest::LoadU32(address + 68); }
  void set_edge_start(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t edge_start_address() const { return address + 68; }
  // sub_820B9FB0, probably
  uint32_t edge_end() const { return guest::LoadU32(address + 72); }
  void set_edge_end(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t edge_end_address() const { return address + 72; }
  // sub_820BAA48: returns 4
  uint32_t slot_19() const { return guest::LoadU32(address + 76); }
  void set_slot_19(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t slot_19_address() const { return address + 76; }
  // sub_820BA668, probably
  uint32_t set_cache_word() const { return guest::LoadU32(address + 80); }
  void set_set_cache_word(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t set_cache_word_address() const { return address + 80; }
  // sub_820BA678, probably
  uint32_t cache_word() const { return guest::LoadU32(address + 84); }
  void set_cache_word(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t cache_word_address() const { return address + 84; }
  // sub_820BAA50, probably
  uint32_t copy_cache() const { return guest::LoadU32(address + 88); }
  void set_copy_cache(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t copy_cache_address() const { return address + 88; }
  // sub_825C8390
  uint32_t slot_23() const { return guest::LoadU32(address + 92); }
  void set_slot_23(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t slot_23_address() const { return address + 92; }
};

// embedded; measure: CollisionShape, CollisionShape::Place.r3, CollisionShape::CurrentCenter.r3, CollisionShape::PreviousCenter.r3
struct CollisionShape : View {
  using View::View;
  static constexpr uint32_t kSize = 116;
  // the template shape (the vehicle's +344 box)
  template <typename T = BoxShape>
  T template_shape() const { return T(guest::LoadU32(address + 0)); }
  void set_template_shape(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_template_shape(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t template_shape_address() const { return address + 0; }
  // the previous placed copy (a clone of the template)
  template <typename T = BoxShape>
  T previous() const { return T(guest::LoadU32(address + 4)); }
  void set_previous(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  // the current one (swapped with the previous and placed by the body's matrix each tick)
  template <typename T = BoxShape>
  T current() const { return T(guest::LoadU32(address + 8)); }
  void set_current(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_current(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t current_address() const { return address + 8; }
  // the box around both copies' bounding spheres: the min
  float bounds_min_x() const { return guest::LoadFloat(address + 16); }
  void set_bounds_min_x(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t bounds_min_x_address() const { return address + 16; }
  float bounds_min_y() const { return guest::LoadFloat(address + 20); }
  void set_bounds_min_y(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t bounds_min_y_address() const { return address + 20; }
  float bounds_min_z() const { return guest::LoadFloat(address + 24); }
  void set_bounds_min_z(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t bounds_min_z_address() const { return address + 24; }
  float bounds_min_w() const { return guest::LoadFloat(address + 28); }
  void set_bounds_min_w(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t bounds_min_w_address() const { return address + 28; }
  // the max
  float bounds_max_x() const { return guest::LoadFloat(address + 32); }
  void set_bounds_max_x(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t bounds_max_x_address() const { return address + 32; }
  float bounds_max_y() const { return guest::LoadFloat(address + 36); }
  void set_bounds_max_y(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t bounds_max_y_address() const { return address + 36; }
  float bounds_max_z() const { return guest::LoadFloat(address + 40); }
  void set_bounds_max_z(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t bounds_max_z_address() const { return address + 40; }
  float bounds_max_w() const { return guest::LoadFloat(address + 44); }
  void set_bounds_max_w(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t bounds_max_w_address() const { return address + 44; }
  // the motion from the previous copy to the current one, a 4x4 matrix: the turn's rows +48, +64, +80
  float motion_row0_x() const { return guest::LoadFloat(address + 48); }
  void set_motion_row0_x(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t motion_row0_x_address() const { return address + 48; }
  float motion_row0_y() const { return guest::LoadFloat(address + 52); }
  void set_motion_row0_y(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t motion_row0_y_address() const { return address + 52; }
  float motion_row0_z() const { return guest::LoadFloat(address + 56); }
  void set_motion_row0_z(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t motion_row0_z_address() const { return address + 56; }
  float motion_row0_w() const { return guest::LoadFloat(address + 60); }
  void set_motion_row0_w(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t motion_row0_w_address() const { return address + 60; }
  float motion_row1_x() const { return guest::LoadFloat(address + 64); }
  void set_motion_row1_x(float v) const { guest::StoreFloat(address + 64, v); }
  uint32_t motion_row1_x_address() const { return address + 64; }
  float motion_row1_y() const { return guest::LoadFloat(address + 68); }
  void set_motion_row1_y(float v) const { guest::StoreFloat(address + 68, v); }
  uint32_t motion_row1_y_address() const { return address + 68; }
  float motion_row1_z() const { return guest::LoadFloat(address + 72); }
  void set_motion_row1_z(float v) const { guest::StoreFloat(address + 72, v); }
  uint32_t motion_row1_z_address() const { return address + 72; }
  float motion_row1_w() const { return guest::LoadFloat(address + 76); }
  void set_motion_row1_w(float v) const { guest::StoreFloat(address + 76, v); }
  uint32_t motion_row1_w_address() const { return address + 76; }
  float motion_row2_x() const { return guest::LoadFloat(address + 80); }
  void set_motion_row2_x(float v) const { guest::StoreFloat(address + 80, v); }
  uint32_t motion_row2_x_address() const { return address + 80; }
  float motion_row2_y() const { return guest::LoadFloat(address + 84); }
  void set_motion_row2_y(float v) const { guest::StoreFloat(address + 84, v); }
  uint32_t motion_row2_y_address() const { return address + 84; }
  float motion_row2_z() const { return guest::LoadFloat(address + 88); }
  void set_motion_row2_z(float v) const { guest::StoreFloat(address + 88, v); }
  uint32_t motion_row2_z_address() const { return address + 88; }
  float motion_row2_w() const { return guest::LoadFloat(address + 92); }
  void set_motion_row2_w(float v) const { guest::StoreFloat(address + 92, v); }
  uint32_t motion_row2_w_address() const { return address + 92; }
  // its translation row
  float motion_translation_x() const { return guest::LoadFloat(address + 96); }
  void set_motion_translation_x(float v) const { guest::StoreFloat(address + 96, v); }
  uint32_t motion_translation_x_address() const { return address + 96; }
  float motion_translation_y() const { return guest::LoadFloat(address + 100); }
  void set_motion_translation_y(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t motion_translation_y_address() const { return address + 100; }
  float motion_translation_z() const { return guest::LoadFloat(address + 104); }
  void set_motion_translation_z(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t motion_translation_z_address() const { return address + 104; }
  float motion_translation_w() const { return guest::LoadFloat(address + 108); }
  void set_motion_translation_w(float v) const { guest::StoreFloat(address + 108, v); }
  uint32_t motion_translation_w_address() const { return address + 108; }
  // how far the box's bottom is lowered when placed (the object's +144)
  float bottom_lowering() const { return guest::LoadFloat(address + 112); }
  void set_bottom_lowering(float v) const { guest::StoreFloat(address + 112, v); }
  uint32_t bottom_lowering_address() const { return address + 112; }
};

// embedded; measure: sub_822BE2C8.r3
#pragma pack(push, 1)
struct WordAt8ObjectData {
  uint8_t pad_0[8] = {};
  uint32_t word_8 = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(WordAt8ObjectData) == 12);
inline constexpr memory::LayoutField kWordAt8ObjectFields[] = {{8, 4}};
inline constexpr memory::Layout kWordAt8ObjectLayout = {kWordAt8ObjectFields, 1};

struct WordAt8Object : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  WordAt8ObjectData* data() const { return static_cast<WordAt8ObjectData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static WordAt8Object New() {
    return WordAt8Object(memory::MakeHandle(new WordAt8ObjectData(), sizeof(WordAt8ObjectData), "WordAt8Object", &kWordAt8ObjectLayout));
  }
  template <uint32_t Stride = kSize>
  static WordAt8Object NewArray(uint32_t count) {
    static_assert(sizeof(WordAt8ObjectData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<WordAt8ObjectData*>(::operator new(sizeof(WordAt8ObjectData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) WordAt8ObjectData();
    return WordAt8Object(memory::MakeHandle(p, sizeof(WordAt8ObjectData) * count, "WordAt8Object", &kWordAt8ObjectLayout, sizeof(WordAt8ObjectData)));
  }
  WordAt8Object at(uint32_t i) const { return WordAt8Object(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  WordAt8ObjectData* direct() const { return static_cast<WordAt8ObjectData*>(memory::HandleOwn(address, &kWordAt8ObjectLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kWordAt8ObjectLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kWordAt8ObjectLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the word sub_822BE2C8 returns (a shape record's current copy in collision)
  uint32_t word_8_address() const { return address + 8; }
  uint32_t word_8_bits() const { if (auto* d = direct()) return d->word_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_word_8_bits(uint32_t v) const { if (auto* d = direct()) { d->word_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t word_8() const { if (auto* d = direct()) return d->word_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_word_8(uint32_t v) const { if (auto* d = direct()) { d->word_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
};

// embedded; measure: Query, sub_820BD8F8.r3, sub_820BDE40.r3, sub_820BE4F0.r3, sub_820BE8A8.r3, sub_820BF690.r3
struct CollisionQuery : View {
  using View::View;
  static constexpr uint32_t kSize = 1456;
  // shape record A (object A's +32)
  template <typename T = CollisionShape>
  T shape_a() const { return T(guest::LoadU32(address + 0)); }
  void set_shape_a(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_shape_a(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t shape_a_address() const { return address + 0; }
  // shape record B
  template <typename T = CollisionShape>
  T shape_b() const { return T(guest::LoadU32(address + 4)); }
  void set_shape_b(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_shape_b(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t shape_b_address() const { return address + 4; }
  // B's previous placement in A's unit box (the block at +112; the blocks' pointers swap with A and B)
  template <typename T = CollisionBlock>
  T b_in_a_previous() const { return T(guest::LoadU32(address + 8)); }
  void set_b_in_a_previous(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_b_in_a_previous(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t b_in_a_previous_address() const { return address + 8; }
  // B's current placement in A's (+448)
  template <typename T = CollisionBlock>
  T b_in_a_current() const { return T(guest::LoadU32(address + 12)); }
  void set_b_in_a_current(const View& v) const { guest::StoreU32(address + 12, v.address); }
  void set_b_in_a_current(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t b_in_a_current_address() const { return address + 12; }
  // A's previous placement in B's (+784)
  template <typename T = CollisionBlock>
  T a_in_b_previous() const { return T(guest::LoadU32(address + 16)); }
  void set_a_in_b_previous(const View& v) const { guest::StoreU32(address + 16, v.address); }
  void set_a_in_b_previous(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t a_in_b_previous_address() const { return address + 16; }
  // A's current placement in B's (+1120)
  template <typename T = CollisionBlock>
  T a_in_b_current() const { return T(guest::LoadU32(address + 20)); }
  void set_a_in_b_current(const View& v) const { guest::StoreU32(address + 20, v.address); }
  void set_a_in_b_current(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t a_in_b_current_address() const { return address + 20; }
  // the contact (a QueryContact, Contact::Set's r3): the point
  float contact_point_x() const { return guest::LoadFloat(address + 32); }
  void set_contact_point_x(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t contact_point_x_address() const { return address + 32; }
  float contact_point_y() const { return guest::LoadFloat(address + 36); }
  void set_contact_point_y(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t contact_point_y_address() const { return address + 36; }
  float contact_point_z() const { return guest::LoadFloat(address + 40); }
  void set_contact_point_z(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t contact_point_z_address() const { return address + 40; }
  float contact_point_w() const { return guest::LoadFloat(address + 44); }
  void set_contact_point_w(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t contact_point_w_address() const { return address + 44; }
  float contact_normal_x() const { return guest::LoadFloat(address + 48); }
  void set_contact_normal_x(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t contact_normal_x_address() const { return address + 48; }
  float contact_normal_y() const { return guest::LoadFloat(address + 52); }
  void set_contact_normal_y(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t contact_normal_y_address() const { return address + 52; }
  float contact_normal_z() const { return guest::LoadFloat(address + 56); }
  void set_contact_normal_z(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t contact_normal_z_address() const { return address + 56; }
  float contact_normal_w() const { return guest::LoadFloat(address + 60); }
  void set_contact_normal_w(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t contact_normal_w_address() const { return address + 60; }
  // the push direction
  float contact_push_x() const { return guest::LoadFloat(address + 64); }
  void set_contact_push_x(float v) const { guest::StoreFloat(address + 64, v); }
  uint32_t contact_push_x_address() const { return address + 64; }
  float contact_push_y() const { return guest::LoadFloat(address + 68); }
  void set_contact_push_y(float v) const { guest::StoreFloat(address + 68, v); }
  uint32_t contact_push_y_address() const { return address + 68; }
  float contact_push_z() const { return guest::LoadFloat(address + 72); }
  void set_contact_push_z(float v) const { guest::StoreFloat(address + 72, v); }
  uint32_t contact_push_z_address() const { return address + 72; }
  float contact_push_w() const { return guest::LoadFloat(address + 76); }
  void set_contact_push_w(float v) const { guest::StoreFloat(address + 76, v); }
  uint32_t contact_push_w_address() const { return address + 76; }
  float contact_depth() const { return guest::LoadFloat(address + 80); }
  void set_contact_depth(float v) const { guest::StoreFloat(address + 80, v); }
  uint32_t contact_depth_address() const { return address + 80; }
  // the impact time (2.0 when reset)
  float contact_time() const { return guest::LoadFloat(address + 84); }
  void set_contact_time(float v) const { guest::StoreFloat(address + 84, v); }
  uint32_t contact_time_address() const { return address + 84; }
  // 2.0 when reset (Query::FindImpact, FindPushOut)
  float unknown_88() const { return guest::LoadFloat(address + 88); }
  void set_unknown_88(float v) const { guest::StoreFloat(address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  // the push-out direction
  float direction_x() const { return guest::LoadFloat(address + 96); }
  void set_direction_x(float v) const { guest::StoreFloat(address + 96, v); }
  uint32_t direction_x_address() const { return address + 96; }
  float direction_y() const { return guest::LoadFloat(address + 100); }
  void set_direction_y(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t direction_y_address() const { return address + 100; }
  float direction_z() const { return guest::LoadFloat(address + 104); }
  void set_direction_z(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t direction_z_address() const { return address + 104; }
  float direction_w() const { return guest::LoadFloat(address + 108); }
  void set_direction_w(float v) const { guest::StoreFloat(address + 108, v); }
  uint32_t direction_w_address() const { return address + 108; }
  // a CollisionBlock
  uint32_t b_in_a_previous_block_address() const { return address + 112; }
  // a CollisionBlock
  uint32_t b_in_a_current_block_address() const { return address + 448; }
  // a CollisionBlock
  uint32_t a_in_b_previous_block_address() const { return address + 784; }
  // a CollisionBlock
  uint32_t a_in_b_current_block_address() const { return address + 1120; }
};

// embedded; measure: CollisionPair::Respond.r5
struct QueryContact : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  float point_x() const { return guest::LoadFloat(address + 0); }
  void set_point_x(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t point_x_address() const { return address + 0; }
  float point_y() const { return guest::LoadFloat(address + 4); }
  void set_point_y(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t point_y_address() const { return address + 4; }
  float point_z() const { return guest::LoadFloat(address + 8); }
  void set_point_z(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t point_z_address() const { return address + 8; }
  float point_w() const { return guest::LoadFloat(address + 12); }
  void set_point_w(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t point_w_address() const { return address + 12; }
  float normal_x() const { return guest::LoadFloat(address + 16); }
  void set_normal_x(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t normal_x_address() const { return address + 16; }
  float normal_y() const { return guest::LoadFloat(address + 20); }
  void set_normal_y(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t normal_y_address() const { return address + 20; }
  float normal_z() const { return guest::LoadFloat(address + 24); }
  void set_normal_z(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t normal_z_address() const { return address + 24; }
  float normal_w() const { return guest::LoadFloat(address + 28); }
  void set_normal_w(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t normal_w_address() const { return address + 28; }
  // the push direction
  float push_x() const { return guest::LoadFloat(address + 32); }
  void set_push_x(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t push_x_address() const { return address + 32; }
  float push_y() const { return guest::LoadFloat(address + 36); }
  void set_push_y(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t push_y_address() const { return address + 36; }
  float push_z() const { return guest::LoadFloat(address + 40); }
  void set_push_z(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t push_z_address() const { return address + 40; }
  float push_w() const { return guest::LoadFloat(address + 44); }
  void set_push_w(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t push_w_address() const { return address + 44; }
  float depth() const { return guest::LoadFloat(address + 48); }
  void set_depth(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t depth_address() const { return address + 48; }
  float time() const { return guest::LoadFloat(address + 52); }
  void set_time(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t time_address() const { return address + 52; }
  // 2.0 when the query is reset
  float unknown_56() const { return guest::LoadFloat(address + 56); }
  void set_unknown_56(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
};

// embedded; measure: BoxBox.+8, BoxShape::RelativePlacement.r4
struct CollisionBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 336;
  // a 3x3 (the other box's axes scaled into the unit box): rows +0, +16, +32
  float row0_x() const { return guest::LoadFloat(address + 0); }
  void set_row0_x(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t row0_x_address() const { return address + 0; }
  float row0_y() const { return guest::LoadFloat(address + 4); }
  void set_row0_y(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t row0_y_address() const { return address + 4; }
  float row0_z() const { return guest::LoadFloat(address + 8); }
  void set_row0_z(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t row0_z_address() const { return address + 8; }
  float row1_x() const { return guest::LoadFloat(address + 16); }
  void set_row1_x(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t row1_x_address() const { return address + 16; }
  float row1_y() const { return guest::LoadFloat(address + 20); }
  void set_row1_y(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t row1_y_address() const { return address + 20; }
  float row1_z() const { return guest::LoadFloat(address + 24); }
  void set_row1_z(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t row1_z_address() const { return address + 24; }
  float row2_x() const { return guest::LoadFloat(address + 32); }
  void set_row2_x(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t row2_x_address() const { return address + 32; }
  float row2_y() const { return guest::LoadFloat(address + 36); }
  void set_row2_y(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t row2_y_address() const { return address + 36; }
  float row2_z() const { return guest::LoadFloat(address + 40); }
  void set_row2_z(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t row2_z_address() const { return address + 40; }
  // the other box's center in the unit box
  float center_x() const { return guest::LoadFloat(address + 48); }
  void set_center_x(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t center_x_address() const { return address + 48; }
  float center_y() const { return guest::LoadFloat(address + 52); }
  void set_center_y(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t center_y_address() const { return address + 52; }
  float center_z() const { return guest::LoadFloat(address + 56); }
  void set_center_z(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t center_z_address() const { return address + 56; }
  // bits 1, 2 beyond the +x, -x face; 4, 8 for y; 16, 32 for z (BoxBox::Outcode)
  uint32_t outcode() const { return guest::LoadU32(address + 64); }
  void set_outcode(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t outcode_address() const { return address + 64; }
  // the 8 corners, 32 bytes apart: corner 0's
  float corner_x() const { return guest::LoadFloat(address + 80); }
  void set_corner_x(float v) const { guest::StoreFloat(address + 80, v); }
  uint32_t corner_x_address() const { return address + 80; }
  float corner_y() const { return guest::LoadFloat(address + 84); }
  void set_corner_y(float v) const { guest::StoreFloat(address + 84, v); }
  uint32_t corner_y_address() const { return address + 84; }
  float corner_z() const { return guest::LoadFloat(address + 88); }
  void set_corner_z(float v) const { guest::StoreFloat(address + 88, v); }
  uint32_t corner_z_address() const { return address + 88; }
  float corner_w() const { return guest::LoadFloat(address + 92); }
  void set_corner_w(float v) const { guest::StoreFloat(address + 92, v); }
  uint32_t corner_w_address() const { return address + 92; }
  // corner 0's outcode (corner k's at +32k)
  uint32_t corner_outcode() const { return guest::LoadU32(address + 96); }
  void set_corner_outcode(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t corner_outcode_address() const { return address + 96; }
};

// embedded; measure: BoxBox, BoxBox.+12
struct CollisionBoxBoxArgs : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  // measure fault: the measure files every BoxBox:: function's r3 under one type, and the query's block pointers back into it: the fields of a CollisionQuery (fault_query_), a CollisionBlock (fault_block_), a block's corner, the edge flags and a face
  uint32_t fault_query_shape_a() const { return guest::LoadU32(address + 0); }
  void set_fault_query_shape_a(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t fault_query_shape_a_address() const { return address + 0; }
  uint32_t fault_query_shape_b() const { return guest::LoadU32(address + 4); }
  void set_fault_query_shape_b(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t fault_query_shape_b_address() const { return address + 4; }
  uint32_t fault_query_b_in_a_previous() const { return guest::LoadU32(address + 8); }
  void set_fault_query_b_in_a_previous(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t fault_query_b_in_a_previous_address() const { return address + 8; }
  uint32_t fault_query_b_in_a_current() const { return guest::LoadU32(address + 12); }
  void set_fault_query_b_in_a_current(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t fault_query_b_in_a_current_address() const { return address + 12; }
  uint32_t fault_query_a_in_b_previous() const { return guest::LoadU32(address + 16); }
  void set_fault_query_a_in_b_previous(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t fault_query_a_in_b_previous_address() const { return address + 16; }
  uint32_t fault_query_a_in_b_current() const { return guest::LoadU32(address + 20); }
  void set_fault_query_a_in_b_current(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t fault_query_a_in_b_current_address() const { return address + 20; }
  float fault_block_row1_z() const { return guest::LoadFloat(address + 24); }
  void set_fault_block_row1_z(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t fault_block_row1_z_address() const { return address + 24; }
  float fault_query_contact_point_x() const { return guest::LoadFloat(address + 32); }
  void set_fault_query_contact_point_x(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t fault_query_contact_point_x_address() const { return address + 32; }
  float fault_query_contact_point_y() const { return guest::LoadFloat(address + 36); }
  void set_fault_query_contact_point_y(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t fault_query_contact_point_y_address() const { return address + 36; }
  float fault_query_contact_point_z() const { return guest::LoadFloat(address + 40); }
  void set_fault_query_contact_point_z(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t fault_query_contact_point_z_address() const { return address + 40; }
  float fault_query_contact_normal_x() const { return guest::LoadFloat(address + 48); }
  void set_fault_query_contact_normal_x(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t fault_query_contact_normal_x_address() const { return address + 48; }
  float fault_query_contact_normal_y() const { return guest::LoadFloat(address + 52); }
  void set_fault_query_contact_normal_y(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t fault_query_contact_normal_y_address() const { return address + 52; }
  float fault_query_contact_normal_z() const { return guest::LoadFloat(address + 56); }
  void set_fault_query_contact_normal_z(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t fault_query_contact_normal_z_address() const { return address + 56; }
  float fault_query_contact_push_x() const { return guest::LoadFloat(address + 64); }
  void set_fault_query_contact_push_x(float v) const { guest::StoreFloat(address + 64, v); }
  uint32_t fault_query_contact_push_x_address() const { return address + 64; }
  float fault_query_contact_depth() const { return guest::LoadFloat(address + 80); }
  void set_fault_query_contact_depth(float v) const { guest::StoreFloat(address + 80, v); }
  uint32_t fault_query_contact_depth_address() const { return address + 80; }
  float fault_query_contact_time() const { return guest::LoadFloat(address + 84); }
  void set_fault_query_contact_time(float v) const { guest::StoreFloat(address + 84, v); }
  uint32_t fault_query_contact_time_address() const { return address + 84; }
  float fault_query_unknown_88() const { return guest::LoadFloat(address + 88); }
  void set_fault_query_unknown_88(float v) const { guest::StoreFloat(address + 88, v); }
  uint32_t fault_query_unknown_88_address() const { return address + 88; }
  float fault_query_direction_x() const { return guest::LoadFloat(address + 96); }
  void set_fault_query_direction_x(float v) const { guest::StoreFloat(address + 96, v); }
  uint32_t fault_query_direction_x_address() const { return address + 96; }
};

// embedded; measure: BoxBox::FaceSeparates.r4
#pragma pack(push, 1)
struct BoxShapeFaceData {
  float distance = 0;  // +0
  uint8_t pad_4[12] = {};
  float normal_x = 0;  // +16
  float normal_y = 0;  // +20
  float normal_z = 0;  // +24
  float normal_w = 0;  // +28
};
#pragma pack(pop)
static_assert(sizeof(BoxShapeFaceData) == 32);
inline constexpr memory::LayoutField kBoxShapeFaceFields[] = {{0, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}};
inline constexpr memory::Layout kBoxShapeFaceLayout = {kBoxShapeFaceFields, 5};

struct BoxShapeFace : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  BoxShapeFaceData* data() const { return static_cast<BoxShapeFaceData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static BoxShapeFace New() {
    return BoxShapeFace(memory::MakeHandle(new BoxShapeFaceData(), sizeof(BoxShapeFaceData), "BoxShapeFace", &kBoxShapeFaceLayout));
  }
  template <uint32_t Stride = kSize>
  static BoxShapeFace NewArray(uint32_t count) {
    static_assert(sizeof(BoxShapeFaceData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<BoxShapeFaceData*>(::operator new(sizeof(BoxShapeFaceData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) BoxShapeFaceData();
    return BoxShapeFace(memory::MakeHandle(p, sizeof(BoxShapeFaceData) * count, "BoxShapeFace", &kBoxShapeFaceLayout, sizeof(BoxShapeFaceData)));
  }
  BoxShapeFace at(uint32_t i) const { return BoxShapeFace(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  BoxShapeFaceData* direct() const { return static_cast<BoxShapeFaceData*>(memory::HandleOwn(address, &kBoxShapeFaceLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kBoxShapeFaceLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kBoxShapeFaceLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t distance_address() const { return address + 0; }
  uint32_t distance_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->distance); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_distance_bits(uint32_t v) const { if (auto* d = direct()) { d->distance = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  float distance() const { return std::bit_cast<float>(distance_bits()); }
  void set_distance(float v) const { set_distance_bits(std::bit_cast<uint32_t>(v)); }
  // the outward normal
  uint32_t normal_x_address() const { return address + 16; }
  uint32_t normal_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->normal_x); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_normal_x_bits(uint32_t v) const { if (auto* d = direct()) { d->normal_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  float normal_x() const { return std::bit_cast<float>(normal_x_bits()); }
  void set_normal_x(float v) const { set_normal_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t normal_y_address() const { return address + 20; }
  uint32_t normal_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->normal_y); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_normal_y_bits(uint32_t v) const { if (auto* d = direct()) { d->normal_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  float normal_y() const { return std::bit_cast<float>(normal_y_bits()); }
  void set_normal_y(float v) const { set_normal_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t normal_z_address() const { return address + 24; }
  uint32_t normal_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->normal_z); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_normal_z_bits(uint32_t v) const { if (auto* d = direct()) { d->normal_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  float normal_z() const { return std::bit_cast<float>(normal_z_bits()); }
  void set_normal_z(float v) const { set_normal_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t normal_w_address() const { return address + 28; }
  uint32_t normal_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->normal_w); return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_normal_w_bits(uint32_t v) const { if (auto* d = direct()) { d->normal_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  float normal_w() const { return std::bit_cast<float>(normal_w_bits()); }
  void set_normal_w(float v) const { set_normal_w_bits(std::bit_cast<uint32_t>(v)); }
};

// embedded; measure: BoxBox::Separated.r4
struct CollisionPairCache : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 when made, then the face that last separated the pair's boxes: bits 0x3F one of A's, 0x3F00 one of B's (+x, -x, +y, -y, +z, -z from bit 0)
  uint32_t separating_faces() const { return guest::LoadU32(address + 0); }
  void set_separating_faces(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t separating_faces_address() const { return address + 0; }
};

// allocated; measure: CollisionPair.+20
struct CollisionPairObject : View {
  using View::View;
  static constexpr uint32_t kSize = 320;
  // its rigid body (a RigidBody, car-physics.toml)
  uint32_t body() const { return guest::LoadU32(address + 16); }
  void set_body(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t body_address() const { return address + 16; }
  // its shape (the vehicle's +344 object)
  template <typename T = BoxShape>
  T shape() const { return T(guest::LoadU32(address + 20)); }
  void set_shape(const View& v) const { guest::StoreU32(address + 20, v.address); }
  void set_shape(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t shape_address() const { return address + 20; }
  // a CollisionShape
  uint32_t shape_record_address() const { return address + 32; }
  // a per-axis scale of the push-out ((1, 1, 1) from CollisionObject::Init)
  float push_out_scale_x() const { return guest::LoadFloat(address + 208); }
  void set_push_out_scale_x(float v) const { guest::StoreFloat(address + 208, v); }
  uint32_t push_out_scale_x_address() const { return address + 208; }
  float push_out_scale_y() const { return guest::LoadFloat(address + 212); }
  void set_push_out_scale_y(float v) const { guest::StoreFloat(address + 212, v); }
  uint32_t push_out_scale_y_address() const { return address + 212; }
  float push_out_scale_z() const { return guest::LoadFloat(address + 216); }
  void set_push_out_scale_z(float v) const { guest::StoreFloat(address + 216, v); }
  uint32_t push_out_scale_z_address() const { return address + 216; }
  // its own groups
  uint32_t groups() const { return guest::LoadU32(address + 244); }
  void set_groups(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t groups_address() const { return address + 244; }
  // the groups it collides with
  uint32_t collides_with() const { return guest::LoadU32(address + 248); }
  void set_collides_with(uint32_t v) const { guest::StoreU32(address + 248, v); }
  uint32_t collides_with_address() const { return address + 248; }
  // the surface bits that get a response (matched against the other's groups)
  uint32_t response_bits() const { return guest::LoadU32(address + 256); }
  void set_response_bits(uint32_t v) const { guest::StoreU32(address + 256, v); }
  uint32_t response_bits_address() const { return address + 256; }
  // the surface bits that push it out
  uint32_t push_out_bits() const { return guest::LoadU32(address + 260); }
  void set_push_out_bits(uint32_t v) const { guest::StoreU32(address + 260, v); }
  uint32_t push_out_bits_address() const { return address + 260; }
  // set: no queries
  uint32_t no_queries() const { return guest::LoadU32(address + 272); }
  void set_no_queries(uint32_t v) const { guest::StoreU32(address + 272, v); }
  uint32_t no_queries_address() const { return address + 272; }
  // CollisionPair::NextCheckTime: the step when set
  uint32_t unknown_276() const { return guest::LoadU32(address + 276); }
  void set_unknown_276(uint32_t v) const { guest::StoreU32(address + 276, v); }
  uint32_t unknown_276_address() const { return address + 276; }
  // set when moved (sub_820B7998)
  uint32_t moved() const { return guest::LoadU32(address + 288); }
  void set_moved(uint32_t v) const { guest::StoreU32(address + 288, v); }
  uint32_t moved_address() const { return address + 288; }
};

// embedded; measure: RigidBody::Torque.ret, sub_820B6890.ret, BoxShape::SetCorners.r4, BoxShape::SetCorners.r5, sub_82294360.ret, sub_822943C8.ret, sub_822944B0.ret
struct CollisionVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float x() const { return guest::LoadFloat(address + 0); }
  void set_x(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(address + 4); }
  void set_y(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(address + 8); }
  void set_z(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  float w() const { return guest::LoadFloat(address + 12); }
  void set_w(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t w_address() const { return address + 12; }
  // x and y as one doubleword (sub_820BF690)
  uint64_t xy() const { return guest::LoadU64(address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  uint64_t zw() const { return guest::LoadU64(address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
};

// embedded; measure: BoxShape::Place.r4
struct CollisionPlacementMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the rows: the placed box's axes and face normals
  float row0_x() const { return guest::LoadFloat(address + 0); }
  void set_row0_x(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t row0_x_address() const { return address + 0; }
  float row0_y() const { return guest::LoadFloat(address + 4); }
  void set_row0_y(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t row0_y_address() const { return address + 4; }
  float row0_z() const { return guest::LoadFloat(address + 8); }
  void set_row0_z(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t row0_z_address() const { return address + 8; }
  float row1_x() const { return guest::LoadFloat(address + 16); }
  void set_row1_x(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t row1_x_address() const { return address + 16; }
  float row1_y() const { return guest::LoadFloat(address + 20); }
  void set_row1_y(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t row1_y_address() const { return address + 20; }
  float row1_z() const { return guest::LoadFloat(address + 24); }
  void set_row1_z(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t row1_z_address() const { return address + 24; }
  float row2_x() const { return guest::LoadFloat(address + 32); }
  void set_row2_x(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t row2_x_address() const { return address + 32; }
  float row2_y() const { return guest::LoadFloat(address + 36); }
  void set_row2_y(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t row2_y_address() const { return address + 36; }
  float row2_z() const { return guest::LoadFloat(address + 40); }
  void set_row2_z(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t row2_z_address() const { return address + 40; }
};

// embedded; measure: sub_820BC198.r4, sub_820BC4D0.r3
struct CollisionGroundDefault : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // the height above the ground ([r5 + 4])
  float depth() const { return guest::LoadFloat(address + 0); }
  void set_depth(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t depth_address() const { return address + 0; }
  // 0x04000000 (none) from sub_820BC198; sub_820BC4D0 reads and writes it
  uint32_t surface() const { return guest::LoadU32(address + 48); }
  void set_surface(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t surface_address() const { return address + 48; }
  // 0 from sub_820BC198
  uint32_t position() const { return guest::LoadU32(address + 52); }
  void set_position(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t position_address() const { return address + 52; }
};

// embedded; measure: sub_820BC198.r5
struct CollisionGroundDefaultSource : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // copied to the ground result's +0
  float unknown_4() const { return guest::LoadFloat(address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// embedded; measure: sub_820BC560.r4
struct CollisionUnreadHitRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  float unknown_8() const { return guest::LoadFloat(address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_48() const { return guest::LoadFloat(address + 48); }
  void set_unknown_48(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
};

// embedded; measure: sub_820BE0D8.r3
struct BoxExitBest : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float fraction() const { return guest::LoadFloat(address + 0); }
  void set_fraction(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t fraction_address() const { return address + 0; }
};

// embedded; measure: sub_820BE0D8.r4
struct BoxExitFace : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 to 5, -1 for none
  uint32_t face() const { return guest::LoadU32(address + 0); }
  void set_face(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t face_address() const { return address + 0; }
};

// embedded; measure: BoxBox.+20@+80
struct CollisionBoxBoxArgsAt80 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // the owner's fault_ field before the measure's fixes (2026-11-22): the measure files every BoxBox:: function's r3 under one type, and the query's block pointers back into it: the fields of a CollisionQuery (fault_query_), a CollisionBlock (fault_block_), a block's corner, the edge flags and a face
  float query_shape_a() const { return guest::LoadFloat(address + 0); }
  void set_query_shape_a(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t query_shape_a_address() const { return address + 0; }
  float query_shape_b() const { return guest::LoadFloat(address + 4); }
  void set_query_shape_b(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t query_shape_b_address() const { return address + 4; }
  float query_b_in_a_previous() const { return guest::LoadFloat(address + 8); }
  void set_query_b_in_a_previous(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t query_b_in_a_previous_address() const { return address + 8; }
  uint32_t query_a_in_b_previous() const { return guest::LoadU32(address + 16); }
  void set_query_a_in_b_previous(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t query_a_in_b_previous_address() const { return address + 16; }
};

// embedded; measure: CollisionObject::CollisionObject.ret@+32
struct CollisionShapeAt32 : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_28() const { return guest::LoadFloat(address + 28); }
  void set_unknown_28(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  float unknown_44() const { return guest::LoadFloat(address + 44); }
  void set_unknown_44(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
};

// embedded; measure: CollisionObject@+32
struct CollisionShapeAt32_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 116;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_112() const { return guest::LoadFloat(address + 112); }
  void set_unknown_112(float v) const { guest::StoreFloat(address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
};

// embedded; measure: CollisionObject@+32.+4
#pragma pack(push, 1)
struct BoxShapePartData {
  uint32_t unknown_0 = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(BoxShapePartData) == 4);
inline constexpr memory::LayoutField kBoxShapePartFields[] = {{0, 4}};
inline constexpr memory::Layout kBoxShapePartLayout = {kBoxShapePartFields, 1};

struct BoxShapePart : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  BoxShapePartData* data() const { return static_cast<BoxShapePartData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static BoxShapePart New() {
    return BoxShapePart(memory::MakeHandle(new BoxShapePartData(), sizeof(BoxShapePartData), "BoxShapePart", &kBoxShapePartLayout));
  }
  template <uint32_t Stride = kSize>
  static BoxShapePart NewArray(uint32_t count) {
    static_assert(sizeof(BoxShapePartData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<BoxShapePartData*>(::operator new(sizeof(BoxShapePartData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) BoxShapePartData();
    return BoxShapePart(memory::MakeHandle(p, sizeof(BoxShapePartData) * count, "BoxShapePart", &kBoxShapePartLayout, sizeof(BoxShapePartData)));
  }
  BoxShapePart at(uint32_t i) const { return BoxShapePart(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  BoxShapePartData* direct() const { return static_cast<BoxShapePartData*>(memory::HandleOwn(address, &kBoxShapePartLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kBoxShapePartLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kBoxShapePartLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_0_bits() const { if (auto* d = direct()) return d->unknown_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_unknown_0_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t unknown_0() const { if (auto* d = direct()) return d->unknown_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_unknown_0(uint32_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
};

// embedded; measure: CollisionObject@+32.+4.+0
struct BoxShapeVtablePart : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_36() const { return guest::LoadU32(address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
};

// embedded; measure: CollisionObject@+32.+8
#pragma pack(push, 1)
struct BoxShapePart_2Data {
  uint32_t unknown_0 = 0;  // +0
  uint8_t pad_4[44] = {};
  float unknown_48 = 0;  // +48
  float unknown_52 = 0;  // +52
  float unknown_56 = 0;  // +56
};
#pragma pack(pop)
static_assert(sizeof(BoxShapePart_2Data) == 60);
inline constexpr memory::LayoutField kBoxShapePart_2Fields[] = {{0, 4}, {48, 4}, {52, 4}, {56, 4}};
inline constexpr memory::Layout kBoxShapePart_2Layout = {kBoxShapePart_2Fields, 4};

struct BoxShapePart_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  BoxShapePart_2Data* data() const { return static_cast<BoxShapePart_2Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static BoxShapePart_2 New() {
    return BoxShapePart_2(memory::MakeHandle(new BoxShapePart_2Data(), sizeof(BoxShapePart_2Data), "BoxShapePart_2", &kBoxShapePart_2Layout));
  }
  template <uint32_t Stride = kSize>
  static BoxShapePart_2 NewArray(uint32_t count) {
    static_assert(sizeof(BoxShapePart_2Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<BoxShapePart_2Data*>(::operator new(sizeof(BoxShapePart_2Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) BoxShapePart_2Data();
    return BoxShapePart_2(memory::MakeHandle(p, sizeof(BoxShapePart_2Data) * count, "BoxShapePart_2", &kBoxShapePart_2Layout, sizeof(BoxShapePart_2Data)));
  }
  BoxShapePart_2 at(uint32_t i) const { return BoxShapePart_2(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  BoxShapePart_2Data* direct() const { return static_cast<BoxShapePart_2Data*>(memory::HandleOwn(address, &kBoxShapePart_2Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kBoxShapePart_2Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kBoxShapePart_2Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_0_bits() const { if (auto* d = direct()) return d->unknown_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_unknown_0_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t unknown_0() const { if (auto* d = direct()) return d->unknown_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_unknown_0(uint32_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_48_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_48); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_unknown_48_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_48 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  float unknown_48() const { return std::bit_cast<float>(unknown_48_bits()); }
  void set_unknown_48(float v) const { set_unknown_48_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_52_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_52); return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_unknown_52_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_52 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  float unknown_52() const { return std::bit_cast<float>(unknown_52_bits()); }
  void set_unknown_52(float v) const { set_unknown_52_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_56_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_56); return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_unknown_56_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_56 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  float unknown_56() const { return std::bit_cast<float>(unknown_56_bits()); }
  void set_unknown_56(float v) const { set_unknown_56_bits(std::bit_cast<uint32_t>(v)); }
};

// embedded; measure: CollisionObject@+32.+8.+0
struct BoxShapeVtablePart_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_36() const { return guest::LoadU32(address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
};

// embedded; measure: Vehicles::DestroyAll.r3@+37632.+344
#pragma pack(push, 1)
struct BoxShapePart_3Data {
  uint32_t unknown_0 = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(BoxShapePart_3Data) == 4);
inline constexpr memory::LayoutField kBoxShapePart_3Fields[] = {{0, 4}};
inline constexpr memory::Layout kBoxShapePart_3Layout = {kBoxShapePart_3Fields, 1};

struct BoxShapePart_3 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  BoxShapePart_3Data* data() const { return static_cast<BoxShapePart_3Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static BoxShapePart_3 New() {
    return BoxShapePart_3(memory::MakeHandle(new BoxShapePart_3Data(), sizeof(BoxShapePart_3Data), "BoxShapePart_3", &kBoxShapePart_3Layout));
  }
  template <uint32_t Stride = kSize>
  static BoxShapePart_3 NewArray(uint32_t count) {
    static_assert(sizeof(BoxShapePart_3Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<BoxShapePart_3Data*>(::operator new(sizeof(BoxShapePart_3Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) BoxShapePart_3Data();
    return BoxShapePart_3(memory::MakeHandle(p, sizeof(BoxShapePart_3Data) * count, "BoxShapePart_3", &kBoxShapePart_3Layout, sizeof(BoxShapePart_3Data)));
  }
  BoxShapePart_3 at(uint32_t i) const { return BoxShapePart_3(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  BoxShapePart_3Data* direct() const { return static_cast<BoxShapePart_3Data*>(memory::HandleOwn(address, &kBoxShapePart_3Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kBoxShapePart_3Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kBoxShapePart_3Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_0_bits() const { if (auto* d = direct()) return d->unknown_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_unknown_0_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t unknown_0() const { if (auto* d = direct()) return d->unknown_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_unknown_0(uint32_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
};

// The image words docs/functions/collision.md's code uses.
struct CollisionGlobals {
  // 0.5
  static float half() { return guest::LoadFloat(0x82001BE4u); }
  static void set_half(float v) { guest::StoreFloat(0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // -1.0
  static float minus_one() { return guest::LoadFloat(0x82001BE0u); }
  static void set_minus_one(float v) { guest::StoreFloat(0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // 2.0
  static float two() { return guest::LoadFloat(0x820021E8u); }
  static void set_two(float v) { guest::StoreFloat(0x820021E8u, v); }
  static constexpr uint32_t two_address() { return 0x820021E8u; }
  // a small epsilon, probably (a squared length's floor and the depth's bias)
  static float epsilon() { return guest::LoadFloat(0x8200222Cu); }
  static void set_epsilon(float v) { guest::StoreFloat(0x8200222Cu, v); }
  static constexpr uint32_t epsilon_address() { return 0x8200222Cu; }
  // Shape::Shape's +24
  static float shape_float_24() { return guest::LoadFloat(0x82002968u); }
  static void set_shape_float_24(float v) { guest::StoreFloat(0x82002968u, v); }
  static constexpr uint32_t shape_float_24_address() { return 0x82002968u; }
  // Shape::Shape's +28 (the friction)
  static float shape_float_28() { return guest::LoadFloat(0x82002204u); }
  static void set_shape_float_28(float v) { guest::StoreFloat(0x82002204u, v); }
  static constexpr uint32_t shape_float_28_address() { return 0x82002204u; }
  // BoxBox::PushOutCorners' starting exit fraction
  static float corner_start() { return guest::LoadFloat(0x82002A90u); }
  static void set_corner_start(float v) { guest::StoreFloat(0x82002A90u, v); }
  static constexpr uint32_t corner_start_address() { return 0x82002A90u; }
  // BoxShape::LowerCorner's four corner indices (words)
  static constexpr uint32_t lower_corners_address() { return 0x82002B0Cu; }
  // probably 1.0: Query::FindImpact's limit
  static float impact_time_limit() { return guest::LoadFloat(0x82002B90u); }
  static void set_impact_time_limit(float v) { guest::StoreFloat(0x82002B90u, v); }
  static constexpr uint32_t impact_time_limit_address() { return 0x82002B90u; }
  // BoxBox::PushOutEdges' depth limit
  static float edge_depth_limit() { return guest::LoadFloat(0x82002B94u); }
  static void set_edge_depth_limit(float v) { guest::StoreFloat(0x82002B94u, v); }
  static constexpr uint32_t edge_depth_limit_address() { return 0x82002B94u; }
  // BoxBox::PushOutEdges' separation floor (lfd)
  static double edge_separation_floor() { return std::bit_cast<double>(guest::LoadU64(0x82002B98u)); }
  static void set_edge_separation_floor(double v) { guest::StoreU64(0x82002B98u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t edge_separation_floor_address() { return 0x82002B98u; }
  // the box's 12 edges' first corners (words)
  static constexpr uint32_t edge_starts_address() { return 0x82600D54u; }
  // their second corners (words)
  static constexpr uint32_t edge_ends_address() { return 0x82600D84u; }
  // 12 16-byte entries: probably each edge's outward direction as factors of the three face normals
  static constexpr uint32_t face_direction_factors_address() { return 0x82600DC0u; }
  // CollisionPair::Respond's impulse scale
  static float contact_impulse_scale() { return guest::LoadFloat(0x82600E84u); }
  static void set_contact_impulse_scale(float v) { guest::StoreFloat(0x82600E84u, v); }
  static constexpr uint32_t contact_impulse_scale_address() { return 0x82600E84u; }
  // 9 function pointers, entry (A's type x 3 + B's): the push-out tests (box-box, entry 8: BoxBox::FindPushOut)
  static constexpr uint32_t push_out_tests_address() { return 0x82600E90u; }
  // the impact tests likewise (box-box: BoxBox::FindImpact)
  static constexpr uint32_t impact_tests_address() { return 0x82600EB4u; }
  // the collision world's clock (CollisionPair::NextCheckTime)
  static float clock() { return guest::LoadFloat(0x826C2D80u); }
  static void set_clock(float v) { guest::StoreFloat(0x826C2D80u, v); }
  static constexpr uint32_t clock_address() { return 0x826C2D80u; }
  // and its step
  static float step() { return guest::LoadFloat(0x826C2D84u); }
  static void set_step(float v) { guest::StoreFloat(0x826C2D84u, v); }
  static constexpr uint32_t step_address() { return 0x826C2D84u; }
  // BoxShape::FaceDirection's static vector (its answer)
  static constexpr uint32_t face_direction_address() { return 0x826C2DE0u; }
};

}  // namespace schema
