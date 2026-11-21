// The views of docs/functions/collision.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/collision.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: BoxShape, Shape, CollisionObject.+32, sub_820BAB48.r4, sub_820BAB48.ret, sub_820BB688.r3, sub_820BB688.r4, BoxShape::Place.r5, sub_822BE2C8.ret, sub_820911F8.ret, sub_820BA1D0.r3, sub_820B9C30.r3, sub_820BC560.r5, Part464::Restore.r4, Part464::Save.r4, Vehicle.+344, WallQuery.+12.+0
struct BoxShape : View {
  using View::View;
  static constexpr uint32_t kSize = 496;
  static constexpr uint32_t kVtable = 0x82002B30;
  // 0x82002B30 (a box); 0x82002AA8 (Shape's) once destroyed or before BoxShape::BoxShape sets it
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // set from the car data (Shape::Shape: 0.5)
  float car_float_16() const { return guest::LoadFloat(base, address + 16); }
  void set_car_float_16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t car_float_16_address() const { return address + 16; }
  // set from the car data (Shape::Shape: 0.5)
  float car_float_20() const { return guest::LoadFloat(base, address + 20); }
  void set_car_float_20(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t car_float_20_address() const { return address + 20; }
  // set from the car data (Shape::Shape: [0x82002968])
  float car_float_24() const { return guest::LoadFloat(base, address + 24); }
  void set_car_float_24(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t car_float_24_address() const { return address + 24; }
  // set from the car data, read by CollisionObject::Respond (Shape::Shape: [0x82002204])
  float friction() const { return guest::LoadFloat(base, address + 28); }
  void set_friction(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t friction_address() const { return address + 28; }
  // the bounding sphere's radius
  float radius() const { return guest::LoadFloat(base, address + 32); }
  void set_radius(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t radius_address() const { return address + 32; }
  // the center (w 0.0 from Shape::Shape)
  float center_x() const { return guest::LoadFloat(base, address + 48); }
  void set_center_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t center_x_address() const { return address + 48; }
  float center_y() const { return guest::LoadFloat(base, address + 52); }
  void set_center_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t center_y_address() const { return address + 52; }
  float center_z() const { return guest::LoadFloat(base, address + 56); }
  void set_center_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t center_z_address() const { return address + 56; }
  float center_w() const { return guest::LoadFloat(base, address + 60); }
  void set_center_w(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t center_w_address() const { return address + 60; }
  // the axes: three rows (+64, +80, +96), the identity until placed; a placed copy's are the matrix's rows
  float axis0_x() const { return guest::LoadFloat(base, address + 64); }
  void set_axis0_x(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t axis0_x_address() const { return address + 64; }
  float axis0_y() const { return guest::LoadFloat(base, address + 68); }
  void set_axis0_y(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t axis0_y_address() const { return address + 68; }
  float axis0_z() const { return guest::LoadFloat(base, address + 72); }
  void set_axis0_z(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t axis0_z_address() const { return address + 72; }
  float axis0_w() const { return guest::LoadFloat(base, address + 76); }
  void set_axis0_w(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t axis0_w_address() const { return address + 76; }
  float axis1_x() const { return guest::LoadFloat(base, address + 80); }
  void set_axis1_x(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t axis1_x_address() const { return address + 80; }
  float axis1_y() const { return guest::LoadFloat(base, address + 84); }
  void set_axis1_y(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t axis1_y_address() const { return address + 84; }
  float axis1_z() const { return guest::LoadFloat(base, address + 88); }
  void set_axis1_z(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t axis1_z_address() const { return address + 88; }
  float axis1_w() const { return guest::LoadFloat(base, address + 92); }
  void set_axis1_w(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t axis1_w_address() const { return address + 92; }
  float axis2_x() const { return guest::LoadFloat(base, address + 96); }
  void set_axis2_x(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t axis2_x_address() const { return address + 96; }
  float axis2_y() const { return guest::LoadFloat(base, address + 100); }
  void set_axis2_y(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t axis2_y_address() const { return address + 100; }
  float axis2_z() const { return guest::LoadFloat(base, address + 104); }
  void set_axis2_z(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t axis2_z_address() const { return address + 104; }
  float axis2_w() const { return guest::LoadFloat(base, address + 108); }
  void set_axis2_w(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t axis2_w_address() const { return address + 108; }
  uint16_t unknown_112() const { return guest::LoadU16(base, address + 112); }
  void set_unknown_112(uint16_t v) const { guest::StoreU16(base, address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  uint16_t unknown_114() const { return guest::LoadU16(base, address + 114); }
  void set_unknown_114(uint16_t v) const { guest::StoreU16(base, address + 114, v); }
  uint32_t unknown_114_address() const { return address + 114; }
  // the half extents (RigidBody::Torque, sub_82379328, returns their address)
  float half_extent_x() const { return guest::LoadFloat(base, address + 128); }
  void set_half_extent_x(float v) const { guest::StoreFloat(base, address + 128, v); }
  uint32_t half_extent_x_address() const { return address + 128; }
  float half_extent_y() const { return guest::LoadFloat(base, address + 132); }
  void set_half_extent_y(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t half_extent_y_address() const { return address + 132; }
  float half_extent_z() const { return guest::LoadFloat(base, address + 136); }
  void set_half_extent_z(float v) const { guest::StoreFloat(base, address + 136, v); }
  uint32_t half_extent_z_address() const { return address + 136; }
  float half_extent_w() const { return guest::LoadFloat(base, address + 140); }
  void set_half_extent_w(float v) const { guest::StoreFloat(base, address + 140, v); }
  uint32_t half_extent_w_address() const { return address + 140; }
  // their inverses (sub_820B6890 returns their address)
  float inverse_half_extent_x() const { return guest::LoadFloat(base, address + 144); }
  void set_inverse_half_extent_x(float v) const { guest::StoreFloat(base, address + 144, v); }
  uint32_t inverse_half_extent_x_address() const { return address + 144; }
  float inverse_half_extent_y() const { return guest::LoadFloat(base, address + 148); }
  void set_inverse_half_extent_y(float v) const { guest::StoreFloat(base, address + 148, v); }
  uint32_t inverse_half_extent_y_address() const { return address + 148; }
  float inverse_half_extent_z() const { return guest::LoadFloat(base, address + 152); }
  void set_inverse_half_extent_z(float v) const { guest::StoreFloat(base, address + 152, v); }
  uint32_t inverse_half_extent_z_address() const { return address + 152; }
  float inverse_half_extent_w() const { return guest::LoadFloat(base, address + 156); }
  void set_inverse_half_extent_w(float v) const { guest::StoreFloat(base, address + 156, v); }
  uint32_t inverse_half_extent_w_address() const { return address + 156; }
  // the lower corners' track positions (pointers to the vehicle's +5328 + 96i, sub_820C1D08; BoxShape::SetCacheWord's word i, zeroed by SetCorners, copied from placement to placement)
  uint32_t lower_corner_position_0() const { return guest::LoadU32(base, address + 160); }
  void set_lower_corner_position_0(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t lower_corner_position_0_address() const { return address + 160; }
  uint32_t lower_corner_position_1() const { return guest::LoadU32(base, address + 164); }
  void set_lower_corner_position_1(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t lower_corner_position_1_address() const { return address + 164; }
  uint32_t lower_corner_position_2() const { return guest::LoadU32(base, address + 168); }
  void set_lower_corner_position_2(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t lower_corner_position_2_address() const { return address + 168; }
  uint32_t lower_corner_position_3() const { return guest::LoadU32(base, address + 172); }
  void set_lower_corner_position_3(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t lower_corner_position_3_address() const { return address + 172; }
  // the first two, as BoxShape::BoxShape(const&) copies them
  uint64_t lower_corner_positions_01() const { return guest::LoadU64(base, address + 160); }
  void set_lower_corner_positions_01(uint64_t v) const { guest::StoreU64(base, address + 160, v); }
  uint32_t lower_corner_positions_01_address() const { return address + 160; }
  uint64_t lower_corner_positions_23() const { return guest::LoadU64(base, address + 168); }
  void set_lower_corner_positions_23(uint64_t v) const { guest::StoreU64(base, address + 168, v); }
  uint32_t lower_corner_positions_23_address() const { return address + 168; }
  // the 8 corners: corner 0's (the max); corner i at +16i takes the min's x, y, z where bit 0, 1, 2 of i is set, else the max's (corner 7 at +288 the min)
  float corner_x() const { return guest::LoadFloat(base, address + 176); }
  void set_corner_x(float v) const { guest::StoreFloat(base, address + 176, v); }
  uint32_t corner_x_address() const { return address + 176; }
  float corner_y() const { return guest::LoadFloat(base, address + 180); }
  void set_corner_y(float v) const { guest::StoreFloat(base, address + 180, v); }
  uint32_t corner_y_address() const { return address + 180; }
  float corner_z() const { return guest::LoadFloat(base, address + 184); }
  void set_corner_z(float v) const { guest::StoreFloat(base, address + 184, v); }
  uint32_t corner_z_address() const { return address + 184; }
  float corner_w() const { return guest::LoadFloat(base, address + 188); }
  void set_corner_w(float v) const { guest::StoreFloat(base, address + 188, v); }
  uint32_t corner_w_address() const { return address + 188; }
  // the 6 faces, 32 bytes each (BoxShape::Face): +x, +y, +z, -x, -y, -z; the distance, then the outward normal at +16
  float face_px_distance() const { return guest::LoadFloat(base, address + 304); }
  void set_face_px_distance(float v) const { guest::StoreFloat(base, address + 304, v); }
  uint32_t face_px_distance_address() const { return address + 304; }
  float face_px_normal_x() const { return guest::LoadFloat(base, address + 320); }
  void set_face_px_normal_x(float v) const { guest::StoreFloat(base, address + 320, v); }
  uint32_t face_px_normal_x_address() const { return address + 320; }
  float face_px_normal_y() const { return guest::LoadFloat(base, address + 324); }
  void set_face_px_normal_y(float v) const { guest::StoreFloat(base, address + 324, v); }
  uint32_t face_px_normal_y_address() const { return address + 324; }
  float face_px_normal_z() const { return guest::LoadFloat(base, address + 328); }
  void set_face_px_normal_z(float v) const { guest::StoreFloat(base, address + 328, v); }
  uint32_t face_px_normal_z_address() const { return address + 328; }
  float face_px_normal_w() const { return guest::LoadFloat(base, address + 332); }
  void set_face_px_normal_w(float v) const { guest::StoreFloat(base, address + 332, v); }
  uint32_t face_px_normal_w_address() const { return address + 332; }
  float face_py_distance() const { return guest::LoadFloat(base, address + 336); }
  void set_face_py_distance(float v) const { guest::StoreFloat(base, address + 336, v); }
  uint32_t face_py_distance_address() const { return address + 336; }
  float face_py_normal_x() const { return guest::LoadFloat(base, address + 352); }
  void set_face_py_normal_x(float v) const { guest::StoreFloat(base, address + 352, v); }
  uint32_t face_py_normal_x_address() const { return address + 352; }
  float face_py_normal_y() const { return guest::LoadFloat(base, address + 356); }
  void set_face_py_normal_y(float v) const { guest::StoreFloat(base, address + 356, v); }
  uint32_t face_py_normal_y_address() const { return address + 356; }
  float face_py_normal_z() const { return guest::LoadFloat(base, address + 360); }
  void set_face_py_normal_z(float v) const { guest::StoreFloat(base, address + 360, v); }
  uint32_t face_py_normal_z_address() const { return address + 360; }
  float face_py_normal_w() const { return guest::LoadFloat(base, address + 364); }
  void set_face_py_normal_w(float v) const { guest::StoreFloat(base, address + 364, v); }
  uint32_t face_py_normal_w_address() const { return address + 364; }
  float face_pz_distance() const { return guest::LoadFloat(base, address + 368); }
  void set_face_pz_distance(float v) const { guest::StoreFloat(base, address + 368, v); }
  uint32_t face_pz_distance_address() const { return address + 368; }
  float face_pz_normal_x() const { return guest::LoadFloat(base, address + 384); }
  void set_face_pz_normal_x(float v) const { guest::StoreFloat(base, address + 384, v); }
  uint32_t face_pz_normal_x_address() const { return address + 384; }
  float face_pz_normal_y() const { return guest::LoadFloat(base, address + 388); }
  void set_face_pz_normal_y(float v) const { guest::StoreFloat(base, address + 388, v); }
  uint32_t face_pz_normal_y_address() const { return address + 388; }
  float face_pz_normal_z() const { return guest::LoadFloat(base, address + 392); }
  void set_face_pz_normal_z(float v) const { guest::StoreFloat(base, address + 392, v); }
  uint32_t face_pz_normal_z_address() const { return address + 392; }
  float face_pz_normal_w() const { return guest::LoadFloat(base, address + 396); }
  void set_face_pz_normal_w(float v) const { guest::StoreFloat(base, address + 396, v); }
  uint32_t face_pz_normal_w_address() const { return address + 396; }
  float face_nx_distance() const { return guest::LoadFloat(base, address + 400); }
  void set_face_nx_distance(float v) const { guest::StoreFloat(base, address + 400, v); }
  uint32_t face_nx_distance_address() const { return address + 400; }
  float face_nx_normal_x() const { return guest::LoadFloat(base, address + 416); }
  void set_face_nx_normal_x(float v) const { guest::StoreFloat(base, address + 416, v); }
  uint32_t face_nx_normal_x_address() const { return address + 416; }
  float face_nx_normal_y() const { return guest::LoadFloat(base, address + 420); }
  void set_face_nx_normal_y(float v) const { guest::StoreFloat(base, address + 420, v); }
  uint32_t face_nx_normal_y_address() const { return address + 420; }
  float face_nx_normal_z() const { return guest::LoadFloat(base, address + 424); }
  void set_face_nx_normal_z(float v) const { guest::StoreFloat(base, address + 424, v); }
  uint32_t face_nx_normal_z_address() const { return address + 424; }
  float face_nx_normal_w() const { return guest::LoadFloat(base, address + 428); }
  void set_face_nx_normal_w(float v) const { guest::StoreFloat(base, address + 428, v); }
  uint32_t face_nx_normal_w_address() const { return address + 428; }
  float face_ny_distance() const { return guest::LoadFloat(base, address + 432); }
  void set_face_ny_distance(float v) const { guest::StoreFloat(base, address + 432, v); }
  uint32_t face_ny_distance_address() const { return address + 432; }
  float face_ny_normal_x() const { return guest::LoadFloat(base, address + 448); }
  void set_face_ny_normal_x(float v) const { guest::StoreFloat(base, address + 448, v); }
  uint32_t face_ny_normal_x_address() const { return address + 448; }
  float face_ny_normal_y() const { return guest::LoadFloat(base, address + 452); }
  void set_face_ny_normal_y(float v) const { guest::StoreFloat(base, address + 452, v); }
  uint32_t face_ny_normal_y_address() const { return address + 452; }
  float face_ny_normal_z() const { return guest::LoadFloat(base, address + 456); }
  void set_face_ny_normal_z(float v) const { guest::StoreFloat(base, address + 456, v); }
  uint32_t face_ny_normal_z_address() const { return address + 456; }
  float face_ny_normal_w() const { return guest::LoadFloat(base, address + 460); }
  void set_face_ny_normal_w(float v) const { guest::StoreFloat(base, address + 460, v); }
  uint32_t face_ny_normal_w_address() const { return address + 460; }
  float face_nz_distance() const { return guest::LoadFloat(base, address + 464); }
  void set_face_nz_distance(float v) const { guest::StoreFloat(base, address + 464, v); }
  uint32_t face_nz_distance_address() const { return address + 464; }
  float face_nz_normal_x() const { return guest::LoadFloat(base, address + 480); }
  void set_face_nz_normal_x(float v) const { guest::StoreFloat(base, address + 480, v); }
  uint32_t face_nz_normal_x_address() const { return address + 480; }
  float face_nz_normal_y() const { return guest::LoadFloat(base, address + 484); }
  void set_face_nz_normal_y(float v) const { guest::StoreFloat(base, address + 484, v); }
  uint32_t face_nz_normal_y_address() const { return address + 484; }
  float face_nz_normal_z() const { return guest::LoadFloat(base, address + 488); }
  void set_face_nz_normal_z(float v) const { guest::StoreFloat(base, address + 488, v); }
  uint32_t face_nz_normal_z_address() const { return address + 488; }
  float face_nz_normal_w() const { return guest::LoadFloat(base, address + 492); }
  void set_face_nz_normal_w(float v) const { guest::StoreFloat(base, address + 492, v); }
  uint32_t face_nz_normal_w_address() const { return address + 492; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t center_address() const { return address + 48; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t axis0_address() const { return address + 64; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t axis1_address() const { return address + 80; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t axis2_address() const { return address + 96; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t half_extent_address() const { return address + 128; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t inverse_half_extent_address() const { return address + 144; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t corners_address() const { return address + 176; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t face_px_normal_address() const { return address + 320; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t face_py_normal_address() const { return address + 352; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t face_pz_normal_address() const { return address + 384; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t face_nx_normal_address() const { return address + 416; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t face_ny_normal_address() const { return address + 448; }
  // from ReplayPlacedBox (replay-snapshots)
  uint32_t face_nz_normal_address() const { return address + 480; }
};

// image; measure: BoxShape.+0, CollisionObject.+32.+0, BoxShape::Place.r5.+0, sub_822BE2C8.ret.+0, sub_820911F8.ret.+0, sub_820BC560.r5.+0
struct BoxShapeVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  // sub_820BB140 (Shape::DeletingDestructor)
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  // sub_820BB410
  uint32_t slot_1() const { return guest::LoadU32(base, address + 4); }
  void set_slot_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_1_address() const { return address + 4; }
  // sub_820BA888
  uint32_t slot_2() const { return guest::LoadU32(base, address + 8); }
  void set_slot_2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot_2_address() const { return address + 8; }
  // sub_820BB450
  uint32_t slot_3() const { return guest::LoadU32(base, address + 12); }
  void set_slot_3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot_3_address() const { return address + 12; }
  // sub_8244A528: 2 for a box
  uint32_t type() const { return guest::LoadU32(base, address + 16); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t type_address() const { return address + 16; }
  // sub_820BB218 (BoxShape::Clone)
  uint32_t clone() const { return guest::LoadU32(base, address + 20); }
  void set_clone(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t clone_address() const { return address + 20; }
  // sub_820BB270
  uint32_t slot_6() const { return guest::LoadU32(base, address + 24); }
  void set_slot_6(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t slot_6_address() const { return address + 24; }
  // sub_820B9FF8
  uint32_t slot_7() const { return guest::LoadU32(base, address + 28); }
  void set_slot_7(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t slot_7_address() const { return address + 28; }
  // sub_820BA1D0
  uint32_t slot_8() const { return guest::LoadU32(base, address + 32); }
  void set_slot_8(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t slot_8_address() const { return address + 32; }
  // sub_820B9C40 (BoxShape::Place)
  uint32_t place() const { return guest::LoadU32(base, address + 36); }
  void set_place(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t place_address() const { return address + 36; }
  // sub_820BA2F0 (BoxShape::Translate)
  uint32_t translate() const { return guest::LoadU32(base, address + 40); }
  void set_translate(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t translate_address() const { return address + 40; }
  // sub_820B9EC8, probably
  uint32_t corner_count() const { return guest::LoadU32(base, address + 44); }
  void set_corner_count(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t corner_count_address() const { return address + 44; }
  // sub_820B9ED0, probably
  uint32_t corner() const { return guest::LoadU32(base, address + 48); }
  void set_corner(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t corner_address() const { return address + 48; }
  // sub_820B9EE0, probably
  uint32_t face_count() const { return guest::LoadU32(base, address + 52); }
  void set_face_count(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t face_count_address() const { return address + 52; }
  // sub_820B9EE8, probably
  uint32_t face() const { return guest::LoadU32(base, address + 56); }
  void set_face(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t face_address() const { return address + 56; }
  // sub_820B9EF8, probably
  uint32_t edge_count() const { return guest::LoadU32(base, address + 60); }
  void set_edge_count(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t edge_count_address() const { return address + 60; }
  // sub_820B9F00, probably
  uint32_t face_direction() const { return guest::LoadU32(base, address + 64); }
  void set_face_direction(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t face_direction_address() const { return address + 64; }
  // sub_820B9F98, probably
  uint32_t edge_start() const { return guest::LoadU32(base, address + 68); }
  void set_edge_start(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t edge_start_address() const { return address + 68; }
  // sub_820B9FB0, probably
  uint32_t edge_end() const { return guest::LoadU32(base, address + 72); }
  void set_edge_end(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t edge_end_address() const { return address + 72; }
  // sub_820BAA48: returns 4
  uint32_t slot_19() const { return guest::LoadU32(base, address + 76); }
  void set_slot_19(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t slot_19_address() const { return address + 76; }
  // sub_820BA668, probably
  uint32_t set_cache_word() const { return guest::LoadU32(base, address + 80); }
  void set_set_cache_word(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t set_cache_word_address() const { return address + 80; }
  // sub_820BA678, probably
  uint32_t cache_word() const { return guest::LoadU32(base, address + 84); }
  void set_cache_word(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t cache_word_address() const { return address + 84; }
  // sub_820BAA50, probably
  uint32_t copy_cache() const { return guest::LoadU32(base, address + 88); }
  void set_copy_cache(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t copy_cache_address() const { return address + 88; }
  // sub_825C8390
  uint32_t slot_23() const { return guest::LoadU32(base, address + 92); }
  void set_slot_23(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t slot_23_address() const { return address + 92; }
};

// embedded; measure: CollisionShape
struct CollisionShape : View {
  using View::View;
  static constexpr uint32_t kSize = 116;
  // the template shape (the vehicle's +344 box)
  template <typename T = BoxShape>
  T template_shape() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_template_shape(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_template_shape(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t template_shape_address() const { return address + 0; }
  // the previous placed copy (a clone of the template)
  template <typename T = BoxShape>
  T previous() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  // the current one (swapped with the previous and placed by the body's matrix each tick)
  template <typename T = BoxShape>
  T current() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_current(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_current(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t current_address() const { return address + 8; }
  // the box around both copies' bounding spheres: the min
  float bounds_min_x() const { return guest::LoadFloat(base, address + 16); }
  void set_bounds_min_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t bounds_min_x_address() const { return address + 16; }
  float bounds_min_y() const { return guest::LoadFloat(base, address + 20); }
  void set_bounds_min_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t bounds_min_y_address() const { return address + 20; }
  float bounds_min_z() const { return guest::LoadFloat(base, address + 24); }
  void set_bounds_min_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t bounds_min_z_address() const { return address + 24; }
  float bounds_min_w() const { return guest::LoadFloat(base, address + 28); }
  void set_bounds_min_w(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t bounds_min_w_address() const { return address + 28; }
  // the max
  float bounds_max_x() const { return guest::LoadFloat(base, address + 32); }
  void set_bounds_max_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t bounds_max_x_address() const { return address + 32; }
  float bounds_max_y() const { return guest::LoadFloat(base, address + 36); }
  void set_bounds_max_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t bounds_max_y_address() const { return address + 36; }
  float bounds_max_z() const { return guest::LoadFloat(base, address + 40); }
  void set_bounds_max_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t bounds_max_z_address() const { return address + 40; }
  float bounds_max_w() const { return guest::LoadFloat(base, address + 44); }
  void set_bounds_max_w(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t bounds_max_w_address() const { return address + 44; }
  // the motion from the previous copy to the current one, a 4x4 matrix: the turn's rows +48, +64, +80
  float motion_row0_x() const { return guest::LoadFloat(base, address + 48); }
  void set_motion_row0_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t motion_row0_x_address() const { return address + 48; }
  float motion_row0_y() const { return guest::LoadFloat(base, address + 52); }
  void set_motion_row0_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t motion_row0_y_address() const { return address + 52; }
  float motion_row0_z() const { return guest::LoadFloat(base, address + 56); }
  void set_motion_row0_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t motion_row0_z_address() const { return address + 56; }
  float motion_row0_w() const { return guest::LoadFloat(base, address + 60); }
  void set_motion_row0_w(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t motion_row0_w_address() const { return address + 60; }
  float motion_row1_x() const { return guest::LoadFloat(base, address + 64); }
  void set_motion_row1_x(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t motion_row1_x_address() const { return address + 64; }
  float motion_row1_y() const { return guest::LoadFloat(base, address + 68); }
  void set_motion_row1_y(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t motion_row1_y_address() const { return address + 68; }
  float motion_row1_z() const { return guest::LoadFloat(base, address + 72); }
  void set_motion_row1_z(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t motion_row1_z_address() const { return address + 72; }
  float motion_row1_w() const { return guest::LoadFloat(base, address + 76); }
  void set_motion_row1_w(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t motion_row1_w_address() const { return address + 76; }
  float motion_row2_x() const { return guest::LoadFloat(base, address + 80); }
  void set_motion_row2_x(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t motion_row2_x_address() const { return address + 80; }
  float motion_row2_y() const { return guest::LoadFloat(base, address + 84); }
  void set_motion_row2_y(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t motion_row2_y_address() const { return address + 84; }
  float motion_row2_z() const { return guest::LoadFloat(base, address + 88); }
  void set_motion_row2_z(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t motion_row2_z_address() const { return address + 88; }
  float motion_row2_w() const { return guest::LoadFloat(base, address + 92); }
  void set_motion_row2_w(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t motion_row2_w_address() const { return address + 92; }
  // its translation row
  float motion_translation_x() const { return guest::LoadFloat(base, address + 96); }
  void set_motion_translation_x(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t motion_translation_x_address() const { return address + 96; }
  float motion_translation_y() const { return guest::LoadFloat(base, address + 100); }
  void set_motion_translation_y(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t motion_translation_y_address() const { return address + 100; }
  float motion_translation_z() const { return guest::LoadFloat(base, address + 104); }
  void set_motion_translation_z(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t motion_translation_z_address() const { return address + 104; }
  float motion_translation_w() const { return guest::LoadFloat(base, address + 108); }
  void set_motion_translation_w(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t motion_translation_w_address() const { return address + 108; }
  // how far the box's bottom is lowered when placed (the object's +144)
  float bottom_lowering() const { return guest::LoadFloat(base, address + 112); }
  void set_bottom_lowering(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t bottom_lowering_address() const { return address + 112; }
};

// embedded; measure: sub_822BE2C8.r3
struct WordAt8Object : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the word sub_822BE2C8 returns (a shape record's current copy in collision)
  uint32_t word_8() const { return guest::LoadU32(base, address + 8); }
  void set_word_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t word_8_address() const { return address + 8; }
};

// embedded; measure: Query, sub_820BD8F8.r3, sub_820BDE40.r3, sub_820BE4F0.r3, sub_820BE8A8.r3, sub_820BF690.r3
struct CollisionQuery : View {
  using View::View;
  static constexpr uint32_t kSize = 1456;
  // shape record A (object A's +32)
  template <typename T = CollisionShape>
  T shape_a() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_shape_a(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_shape_a(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t shape_a_address() const { return address + 0; }
  // shape record B
  template <typename T = CollisionShape>
  T shape_b() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_shape_b(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_shape_b(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t shape_b_address() const { return address + 4; }
  // B's previous placement in A's unit box (the block at +112; the blocks' pointers swap with A and B)
  template <typename T = CollisionBlock>
  T b_in_a_previous() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_b_in_a_previous(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_b_in_a_previous(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t b_in_a_previous_address() const { return address + 8; }
  // B's current placement in A's (+448)
  template <typename T = CollisionBlock>
  T b_in_a_current() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_b_in_a_current(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_b_in_a_current(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t b_in_a_current_address() const { return address + 12; }
  // A's previous placement in B's (+784)
  template <typename T = CollisionBlock>
  T a_in_b_previous() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_a_in_b_previous(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_a_in_b_previous(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t a_in_b_previous_address() const { return address + 16; }
  // A's current placement in B's (+1120)
  template <typename T = CollisionBlock>
  T a_in_b_current() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_a_in_b_current(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_a_in_b_current(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t a_in_b_current_address() const { return address + 20; }
  // the contact (a QueryContact, Contact::Set's r3): the point
  float contact_point_x() const { return guest::LoadFloat(base, address + 32); }
  void set_contact_point_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t contact_point_x_address() const { return address + 32; }
  float contact_point_y() const { return guest::LoadFloat(base, address + 36); }
  void set_contact_point_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t contact_point_y_address() const { return address + 36; }
  float contact_point_z() const { return guest::LoadFloat(base, address + 40); }
  void set_contact_point_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t contact_point_z_address() const { return address + 40; }
  float contact_point_w() const { return guest::LoadFloat(base, address + 44); }
  void set_contact_point_w(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t contact_point_w_address() const { return address + 44; }
  float contact_normal_x() const { return guest::LoadFloat(base, address + 48); }
  void set_contact_normal_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t contact_normal_x_address() const { return address + 48; }
  float contact_normal_y() const { return guest::LoadFloat(base, address + 52); }
  void set_contact_normal_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t contact_normal_y_address() const { return address + 52; }
  float contact_normal_z() const { return guest::LoadFloat(base, address + 56); }
  void set_contact_normal_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t contact_normal_z_address() const { return address + 56; }
  float contact_normal_w() const { return guest::LoadFloat(base, address + 60); }
  void set_contact_normal_w(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t contact_normal_w_address() const { return address + 60; }
  // the push direction
  float contact_push_x() const { return guest::LoadFloat(base, address + 64); }
  void set_contact_push_x(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t contact_push_x_address() const { return address + 64; }
  float contact_push_y() const { return guest::LoadFloat(base, address + 68); }
  void set_contact_push_y(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t contact_push_y_address() const { return address + 68; }
  float contact_push_z() const { return guest::LoadFloat(base, address + 72); }
  void set_contact_push_z(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t contact_push_z_address() const { return address + 72; }
  float contact_push_w() const { return guest::LoadFloat(base, address + 76); }
  void set_contact_push_w(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t contact_push_w_address() const { return address + 76; }
  float contact_depth() const { return guest::LoadFloat(base, address + 80); }
  void set_contact_depth(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t contact_depth_address() const { return address + 80; }
  // the impact time (2.0 when reset)
  float contact_time() const { return guest::LoadFloat(base, address + 84); }
  void set_contact_time(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t contact_time_address() const { return address + 84; }
  // 2.0 when reset (Query::FindImpact, FindPushOut)
  float unknown_88() const { return guest::LoadFloat(base, address + 88); }
  void set_unknown_88(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  // the push-out direction
  float direction_x() const { return guest::LoadFloat(base, address + 96); }
  void set_direction_x(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t direction_x_address() const { return address + 96; }
  float direction_y() const { return guest::LoadFloat(base, address + 100); }
  void set_direction_y(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t direction_y_address() const { return address + 100; }
  float direction_z() const { return guest::LoadFloat(base, address + 104); }
  void set_direction_z(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t direction_z_address() const { return address + 104; }
  float direction_w() const { return guest::LoadFloat(base, address + 108); }
  void set_direction_w(float v) const { guest::StoreFloat(base, address + 108, v); }
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
  float point_x() const { return guest::LoadFloat(base, address + 0); }
  void set_point_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t point_x_address() const { return address + 0; }
  float point_y() const { return guest::LoadFloat(base, address + 4); }
  void set_point_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t point_y_address() const { return address + 4; }
  float point_z() const { return guest::LoadFloat(base, address + 8); }
  void set_point_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t point_z_address() const { return address + 8; }
  float point_w() const { return guest::LoadFloat(base, address + 12); }
  void set_point_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t point_w_address() const { return address + 12; }
  float normal_x() const { return guest::LoadFloat(base, address + 16); }
  void set_normal_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t normal_x_address() const { return address + 16; }
  float normal_y() const { return guest::LoadFloat(base, address + 20); }
  void set_normal_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t normal_y_address() const { return address + 20; }
  float normal_z() const { return guest::LoadFloat(base, address + 24); }
  void set_normal_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t normal_z_address() const { return address + 24; }
  float normal_w() const { return guest::LoadFloat(base, address + 28); }
  void set_normal_w(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t normal_w_address() const { return address + 28; }
  // the push direction
  float push_x() const { return guest::LoadFloat(base, address + 32); }
  void set_push_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t push_x_address() const { return address + 32; }
  float push_y() const { return guest::LoadFloat(base, address + 36); }
  void set_push_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t push_y_address() const { return address + 36; }
  float push_z() const { return guest::LoadFloat(base, address + 40); }
  void set_push_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t push_z_address() const { return address + 40; }
  float push_w() const { return guest::LoadFloat(base, address + 44); }
  void set_push_w(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t push_w_address() const { return address + 44; }
  float depth() const { return guest::LoadFloat(base, address + 48); }
  void set_depth(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t depth_address() const { return address + 48; }
  float time() const { return guest::LoadFloat(base, address + 52); }
  void set_time(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t time_address() const { return address + 52; }
  // 2.0 when the query is reset
  float unknown_56() const { return guest::LoadFloat(base, address + 56); }
  void set_unknown_56(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
};

// embedded; measure: BoxBox.+8, BoxShape::RelativePlacement.r4
struct CollisionBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 336;
  // a 3x3 (the other box's axes scaled into the unit box): rows +0, +16, +32
  float row0_x() const { return guest::LoadFloat(base, address + 0); }
  void set_row0_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t row0_x_address() const { return address + 0; }
  float row0_y() const { return guest::LoadFloat(base, address + 4); }
  void set_row0_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t row0_y_address() const { return address + 4; }
  float row0_z() const { return guest::LoadFloat(base, address + 8); }
  void set_row0_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t row0_z_address() const { return address + 8; }
  float row1_x() const { return guest::LoadFloat(base, address + 16); }
  void set_row1_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t row1_x_address() const { return address + 16; }
  float row1_y() const { return guest::LoadFloat(base, address + 20); }
  void set_row1_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t row1_y_address() const { return address + 20; }
  float row1_z() const { return guest::LoadFloat(base, address + 24); }
  void set_row1_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t row1_z_address() const { return address + 24; }
  float row2_x() const { return guest::LoadFloat(base, address + 32); }
  void set_row2_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t row2_x_address() const { return address + 32; }
  float row2_y() const { return guest::LoadFloat(base, address + 36); }
  void set_row2_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t row2_y_address() const { return address + 36; }
  float row2_z() const { return guest::LoadFloat(base, address + 40); }
  void set_row2_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t row2_z_address() const { return address + 40; }
  // the other box's center in the unit box
  float center_x() const { return guest::LoadFloat(base, address + 48); }
  void set_center_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t center_x_address() const { return address + 48; }
  float center_y() const { return guest::LoadFloat(base, address + 52); }
  void set_center_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t center_y_address() const { return address + 52; }
  float center_z() const { return guest::LoadFloat(base, address + 56); }
  void set_center_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t center_z_address() const { return address + 56; }
  // bits 1, 2 beyond the +x, -x face; 4, 8 for y; 16, 32 for z (BoxBox::Outcode)
  uint32_t outcode() const { return guest::LoadU32(base, address + 64); }
  void set_outcode(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t outcode_address() const { return address + 64; }
  // the 8 corners, 32 bytes apart: corner 0's
  float corner_x() const { return guest::LoadFloat(base, address + 80); }
  void set_corner_x(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t corner_x_address() const { return address + 80; }
  float corner_y() const { return guest::LoadFloat(base, address + 84); }
  void set_corner_y(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t corner_y_address() const { return address + 84; }
  float corner_z() const { return guest::LoadFloat(base, address + 88); }
  void set_corner_z(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t corner_z_address() const { return address + 88; }
  float corner_w() const { return guest::LoadFloat(base, address + 92); }
  void set_corner_w(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t corner_w_address() const { return address + 92; }
  // corner 0's outcode (corner k's at +32k)
  uint32_t corner_outcode() const { return guest::LoadU32(base, address + 96); }
  void set_corner_outcode(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t corner_outcode_address() const { return address + 96; }
};

// embedded; measure: BoxBox
struct CollisionBoxBoxArgs : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  // measure fault: the measure files every BoxBox:: function's r3 under one type, and the query's block pointers back into it: the fields of a CollisionQuery (fault_query_), a CollisionBlock (fault_block_), a block's corner, the edge flags and a face
  uint32_t fault_query_shape_a() const { return guest::LoadU32(base, address + 0); }
  void set_fault_query_shape_a(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t fault_query_shape_a_address() const { return address + 0; }
  float fault_block_row0_x() const { return guest::LoadFloat(base, address + 0); }
  void set_fault_block_row0_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t fault_block_row0_x_address() const { return address + 0; }
  uint32_t fault_query_shape_b() const { return guest::LoadU32(base, address + 4); }
  void set_fault_query_shape_b(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t fault_query_shape_b_address() const { return address + 4; }
  float fault_block_row0_y() const { return guest::LoadFloat(base, address + 4); }
  void set_fault_block_row0_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t fault_block_row0_y_address() const { return address + 4; }
  uint32_t fault_query_b_in_a_previous() const { return guest::LoadU32(base, address + 8); }
  void set_fault_query_b_in_a_previous(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t fault_query_b_in_a_previous_address() const { return address + 8; }
  float fault_block_row0_z() const { return guest::LoadFloat(base, address + 8); }
  void set_fault_block_row0_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t fault_block_row0_z_address() const { return address + 8; }
  uint32_t fault_query_b_in_a_current() const { return guest::LoadU32(base, address + 12); }
  void set_fault_query_b_in_a_current(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t fault_query_b_in_a_current_address() const { return address + 12; }
  uint32_t fault_query_a_in_b_previous() const { return guest::LoadU32(base, address + 16); }
  void set_fault_query_a_in_b_previous(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t fault_query_a_in_b_previous_address() const { return address + 16; }
  float fault_block_row1_x() const { return guest::LoadFloat(base, address + 16); }
  void set_fault_block_row1_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t fault_block_row1_x_address() const { return address + 16; }
  uint32_t fault_query_a_in_b_current() const { return guest::LoadU32(base, address + 20); }
  void set_fault_query_a_in_b_current(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t fault_query_a_in_b_current_address() const { return address + 20; }
  float fault_block_row1_y() const { return guest::LoadFloat(base, address + 20); }
  void set_fault_block_row1_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t fault_block_row1_y_address() const { return address + 20; }
  float fault_block_row1_z() const { return guest::LoadFloat(base, address + 24); }
  void set_fault_block_row1_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t fault_block_row1_z_address() const { return address + 24; }
  float fault_query_contact_point_x() const { return guest::LoadFloat(base, address + 32); }
  void set_fault_query_contact_point_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t fault_query_contact_point_x_address() const { return address + 32; }
  float fault_block_row2_x() const { return guest::LoadFloat(base, address + 32); }
  void set_fault_block_row2_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t fault_block_row2_x_address() const { return address + 32; }
  float fault_query_contact_point_y() const { return guest::LoadFloat(base, address + 36); }
  void set_fault_query_contact_point_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t fault_query_contact_point_y_address() const { return address + 36; }
  float fault_block_row2_y() const { return guest::LoadFloat(base, address + 36); }
  void set_fault_block_row2_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t fault_block_row2_y_address() const { return address + 36; }
  float fault_query_contact_point_z() const { return guest::LoadFloat(base, address + 40); }
  void set_fault_query_contact_point_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t fault_query_contact_point_z_address() const { return address + 40; }
  float fault_block_row2_z() const { return guest::LoadFloat(base, address + 40); }
  void set_fault_block_row2_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t fault_block_row2_z_address() const { return address + 40; }
  float fault_query_contact_normal_x() const { return guest::LoadFloat(base, address + 48); }
  void set_fault_query_contact_normal_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t fault_query_contact_normal_x_address() const { return address + 48; }
  float fault_block_center_x() const { return guest::LoadFloat(base, address + 48); }
  void set_fault_block_center_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t fault_block_center_x_address() const { return address + 48; }
  float fault_query_contact_normal_y() const { return guest::LoadFloat(base, address + 52); }
  void set_fault_query_contact_normal_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t fault_query_contact_normal_y_address() const { return address + 52; }
  float fault_block_center_y() const { return guest::LoadFloat(base, address + 52); }
  void set_fault_block_center_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t fault_block_center_y_address() const { return address + 52; }
  float fault_query_contact_normal_z() const { return guest::LoadFloat(base, address + 56); }
  void set_fault_query_contact_normal_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t fault_query_contact_normal_z_address() const { return address + 56; }
  float fault_block_center_z() const { return guest::LoadFloat(base, address + 56); }
  void set_fault_block_center_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t fault_block_center_z_address() const { return address + 56; }
  float fault_query_contact_push_x() const { return guest::LoadFloat(base, address + 64); }
  void set_fault_query_contact_push_x(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t fault_query_contact_push_x_address() const { return address + 64; }
  uint32_t fault_block_outcode() const { return guest::LoadU32(base, address + 64); }
  void set_fault_block_outcode(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t fault_block_outcode_address() const { return address + 64; }
  float fault_query_contact_depth() const { return guest::LoadFloat(base, address + 80); }
  void set_fault_query_contact_depth(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t fault_query_contact_depth_address() const { return address + 80; }
  float fault_block_corner_x() const { return guest::LoadFloat(base, address + 80); }
  void set_fault_block_corner_x(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t fault_block_corner_x_address() const { return address + 80; }
  float fault_query_contact_time() const { return guest::LoadFloat(base, address + 84); }
  void set_fault_query_contact_time(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t fault_query_contact_time_address() const { return address + 84; }
  float fault_block_corner_y() const { return guest::LoadFloat(base, address + 84); }
  void set_fault_block_corner_y(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t fault_block_corner_y_address() const { return address + 84; }
  float fault_query_unknown_88() const { return guest::LoadFloat(base, address + 88); }
  void set_fault_query_unknown_88(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t fault_query_unknown_88_address() const { return address + 88; }
  float fault_block_corner_z() const { return guest::LoadFloat(base, address + 88); }
  void set_fault_block_corner_z(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t fault_block_corner_z_address() const { return address + 88; }
  float fault_query_direction_x() const { return guest::LoadFloat(base, address + 96); }
  void set_fault_query_direction_x(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t fault_query_direction_x_address() const { return address + 96; }
  uint32_t fault_block_corner_outcode() const { return guest::LoadU32(base, address + 96); }
  void set_fault_block_corner_outcode(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t fault_block_corner_outcode_address() const { return address + 96; }
  // BoxBox::MoveCorners' r3 is a block's corners (+80): corner k's x, y, z and its outcode +16
  float fault_corner_x() const { return guest::LoadFloat(base, address + 0); }
  void set_fault_corner_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t fault_corner_x_address() const { return address + 0; }
  uint32_t fault_corner_outcode() const { return guest::LoadU32(base, address + 16); }
  void set_fault_corner_outcode(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t fault_corner_outcode_address() const { return address + 16; }
  // BoxBox::EdgesCrossing's r3: its 12 words out (a frame's)
  uint32_t fault_crossing_flag() const { return guest::LoadU32(base, address + 0); }
  void set_fault_crossing_flag(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t fault_crossing_flag_address() const { return address + 0; }
  // BoxBox::FaceSeparates' r3: a face's distance (a BoxShapeFace)
  float fault_face_distance() const { return guest::LoadFloat(base, address + 0); }
  void set_fault_face_distance(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t fault_face_distance_address() const { return address + 0; }
};

// embedded; measure: BoxBox::FaceSeparates.r4
struct BoxShapeFace : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  float distance() const { return guest::LoadFloat(base, address + 0); }
  void set_distance(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t distance_address() const { return address + 0; }
  // the outward normal
  float normal_x() const { return guest::LoadFloat(base, address + 16); }
  void set_normal_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t normal_x_address() const { return address + 16; }
  float normal_y() const { return guest::LoadFloat(base, address + 20); }
  void set_normal_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t normal_y_address() const { return address + 20; }
  float normal_z() const { return guest::LoadFloat(base, address + 24); }
  void set_normal_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t normal_z_address() const { return address + 24; }
  float normal_w() const { return guest::LoadFloat(base, address + 28); }
  void set_normal_w(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t normal_w_address() const { return address + 28; }
};

// embedded; measure: BoxBox::Separated.r4
struct CollisionPairCache : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 when made, then the face that last separated the pair's boxes: bits 0x3F one of A's, 0x3F00 one of B's (+x, -x, +y, -y, +z, -z from bit 0)
  uint32_t separating_faces() const { return guest::LoadU32(base, address + 0); }
  void set_separating_faces(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t separating_faces_address() const { return address + 0; }
};

// allocated; measure: CollisionPair.+20
struct CollisionPairObject : View {
  using View::View;
  static constexpr uint32_t kSize = 320;
  // its rigid body (a RigidBody, car-physics.toml)
  uint32_t body() const { return guest::LoadU32(base, address + 16); }
  void set_body(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t body_address() const { return address + 16; }
  // its shape (the vehicle's +344 object)
  template <typename T = BoxShape>
  T shape() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_shape(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_shape(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t shape_address() const { return address + 20; }
  // a CollisionShape
  uint32_t shape_record_address() const { return address + 32; }
  // a per-axis scale of the push-out ((1, 1, 1) from CollisionObject::Init)
  float push_out_scale_x() const { return guest::LoadFloat(base, address + 208); }
  void set_push_out_scale_x(float v) const { guest::StoreFloat(base, address + 208, v); }
  uint32_t push_out_scale_x_address() const { return address + 208; }
  float push_out_scale_y() const { return guest::LoadFloat(base, address + 212); }
  void set_push_out_scale_y(float v) const { guest::StoreFloat(base, address + 212, v); }
  uint32_t push_out_scale_y_address() const { return address + 212; }
  float push_out_scale_z() const { return guest::LoadFloat(base, address + 216); }
  void set_push_out_scale_z(float v) const { guest::StoreFloat(base, address + 216, v); }
  uint32_t push_out_scale_z_address() const { return address + 216; }
  // its own groups
  uint32_t groups() const { return guest::LoadU32(base, address + 244); }
  void set_groups(uint32_t v) const { guest::StoreU32(base, address + 244, v); }
  uint32_t groups_address() const { return address + 244; }
  // the groups it collides with
  uint32_t collides_with() const { return guest::LoadU32(base, address + 248); }
  void set_collides_with(uint32_t v) const { guest::StoreU32(base, address + 248, v); }
  uint32_t collides_with_address() const { return address + 248; }
  // the surface bits that get a response (matched against the other's groups)
  uint32_t response_bits() const { return guest::LoadU32(base, address + 256); }
  void set_response_bits(uint32_t v) const { guest::StoreU32(base, address + 256, v); }
  uint32_t response_bits_address() const { return address + 256; }
  // the surface bits that push it out
  uint32_t push_out_bits() const { return guest::LoadU32(base, address + 260); }
  void set_push_out_bits(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t push_out_bits_address() const { return address + 260; }
  // set: no queries
  uint32_t no_queries() const { return guest::LoadU32(base, address + 272); }
  void set_no_queries(uint32_t v) const { guest::StoreU32(base, address + 272, v); }
  uint32_t no_queries_address() const { return address + 272; }
  // CollisionPair::NextCheckTime: the step when set
  uint32_t unknown_276() const { return guest::LoadU32(base, address + 276); }
  void set_unknown_276(uint32_t v) const { guest::StoreU32(base, address + 276, v); }
  uint32_t unknown_276_address() const { return address + 276; }
  // set when moved (sub_820B7998)
  uint32_t moved() const { return guest::LoadU32(base, address + 288); }
  void set_moved(uint32_t v) const { guest::StoreU32(base, address + 288, v); }
  uint32_t moved_address() const { return address + 288; }
};

// embedded; measure: RigidBody::Torque.ret, sub_820B6890.ret, BoxShape::SetCorners.r4, BoxShape::SetCorners.r5, sub_82294360.ret, sub_822943C8.ret, sub_822944B0.ret
struct CollisionVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
  // x and y as one doubleword (sub_820BF690)
  uint64_t xy() const { return guest::LoadU64(base, address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  uint64_t zw() const { return guest::LoadU64(base, address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
};

// embedded; measure: BoxShape::Place.r4
struct CollisionPlacementMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the rows: the placed box's axes and face normals
  float row0_x() const { return guest::LoadFloat(base, address + 0); }
  void set_row0_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t row0_x_address() const { return address + 0; }
  float row0_y() const { return guest::LoadFloat(base, address + 4); }
  void set_row0_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t row0_y_address() const { return address + 4; }
  float row0_z() const { return guest::LoadFloat(base, address + 8); }
  void set_row0_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t row0_z_address() const { return address + 8; }
  float row1_x() const { return guest::LoadFloat(base, address + 16); }
  void set_row1_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t row1_x_address() const { return address + 16; }
  float row1_y() const { return guest::LoadFloat(base, address + 20); }
  void set_row1_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t row1_y_address() const { return address + 20; }
  float row1_z() const { return guest::LoadFloat(base, address + 24); }
  void set_row1_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t row1_z_address() const { return address + 24; }
  float row2_x() const { return guest::LoadFloat(base, address + 32); }
  void set_row2_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t row2_x_address() const { return address + 32; }
  float row2_y() const { return guest::LoadFloat(base, address + 36); }
  void set_row2_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t row2_y_address() const { return address + 36; }
  float row2_z() const { return guest::LoadFloat(base, address + 40); }
  void set_row2_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t row2_z_address() const { return address + 40; }
};

// embedded; measure: sub_820BC198.r4, sub_820BC4D0.r3
struct CollisionGroundDefault : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // the height above the ground ([r5 + 4])
  float depth() const { return guest::LoadFloat(base, address + 0); }
  void set_depth(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t depth_address() const { return address + 0; }
  // 0x04000000 (none) from sub_820BC198; sub_820BC4D0 reads and writes it
  uint32_t surface() const { return guest::LoadU32(base, address + 48); }
  void set_surface(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t surface_address() const { return address + 48; }
  // 0 from sub_820BC198
  uint32_t position() const { return guest::LoadU32(base, address + 52); }
  void set_position(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t position_address() const { return address + 52; }
};

// embedded; measure: sub_820BC198.r5
struct CollisionGroundDefaultSource : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // copied to the ground result's +0
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// embedded; measure: sub_820BC560.r4
struct CollisionUnreadHitRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_48() const { return guest::LoadFloat(base, address + 48); }
  void set_unknown_48(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_56() const { return guest::LoadU32(base, address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
};

// embedded; measure: sub_820BE0D8.r3
struct BoxExitBest : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float fraction() const { return guest::LoadFloat(base, address + 0); }
  void set_fraction(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t fraction_address() const { return address + 0; }
};

// embedded; measure: sub_820BE0D8.r4
struct BoxExitFace : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 to 5, -1 for none
  uint32_t face() const { return guest::LoadU32(base, address + 0); }
  void set_face(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t face_address() const { return address + 0; }
};

// The image words docs/functions/collision.md's code uses.
struct CollisionGlobals {
  // 0.5
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // -1.0
  static float minus_one(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE0u); }
  static void set_minus_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // 2.0
  static float two(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_two(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t two_address() { return 0x820021E8u; }
  // a small epsilon, probably (a squared length's floor and the depth's bias)
  static float epsilon(uint8_t* base) { return guest::LoadFloat(base, 0x8200222Cu); }
  static void set_epsilon(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200222Cu, v); }
  static constexpr uint32_t epsilon_address() { return 0x8200222Cu; }
  // Shape::Shape's +24
  static float shape_float_24(uint8_t* base) { return guest::LoadFloat(base, 0x82002968u); }
  static void set_shape_float_24(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002968u, v); }
  static constexpr uint32_t shape_float_24_address() { return 0x82002968u; }
  // Shape::Shape's +28 (the friction)
  static float shape_float_28(uint8_t* base) { return guest::LoadFloat(base, 0x82002204u); }
  static void set_shape_float_28(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002204u, v); }
  static constexpr uint32_t shape_float_28_address() { return 0x82002204u; }
  // BoxBox::PushOutCorners' starting exit fraction
  static float corner_start(uint8_t* base) { return guest::LoadFloat(base, 0x82002A90u); }
  static void set_corner_start(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A90u, v); }
  static constexpr uint32_t corner_start_address() { return 0x82002A90u; }
  // BoxShape::LowerCorner's four corner indices (words)
  static constexpr uint32_t lower_corners_address() { return 0x82002B0Cu; }
  // probably 1.0: Query::FindImpact's limit
  static float impact_time_limit(uint8_t* base) { return guest::LoadFloat(base, 0x82002B90u); }
  static void set_impact_time_limit(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002B90u, v); }
  static constexpr uint32_t impact_time_limit_address() { return 0x82002B90u; }
  // BoxBox::PushOutEdges' depth limit
  static float edge_depth_limit(uint8_t* base) { return guest::LoadFloat(base, 0x82002B94u); }
  static void set_edge_depth_limit(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002B94u, v); }
  static constexpr uint32_t edge_depth_limit_address() { return 0x82002B94u; }
  // BoxBox::PushOutEdges' separation floor (lfd)
  static double edge_separation_floor(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x82002B98u)); }
  static void set_edge_separation_floor(uint8_t* base, double v) { guest::StoreU64(base, 0x82002B98u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t edge_separation_floor_address() { return 0x82002B98u; }
  // the box's 12 edges' first corners (words)
  static constexpr uint32_t edge_starts_address() { return 0x82600D54u; }
  // their second corners (words)
  static constexpr uint32_t edge_ends_address() { return 0x82600D84u; }
  // 12 16-byte entries: probably each edge's outward direction as factors of the three face normals
  static constexpr uint32_t face_direction_factors_address() { return 0x82600DC0u; }
  // CollisionPair::Respond's impulse scale
  static float contact_impulse_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82600E84u); }
  static void set_contact_impulse_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82600E84u, v); }
  static constexpr uint32_t contact_impulse_scale_address() { return 0x82600E84u; }
  // 9 function pointers, entry (A's type x 3 + B's): the push-out tests (box-box, entry 8: BoxBox::FindPushOut)
  static constexpr uint32_t push_out_tests_address() { return 0x82600E90u; }
  // the impact tests likewise (box-box: BoxBox::FindImpact)
  static constexpr uint32_t impact_tests_address() { return 0x82600EB4u; }
  // the collision world's clock (CollisionPair::NextCheckTime)
  static float clock(uint8_t* base) { return guest::LoadFloat(base, 0x826C2D80u); }
  static void set_clock(uint8_t* base, float v) { guest::StoreFloat(base, 0x826C2D80u, v); }
  static constexpr uint32_t clock_address() { return 0x826C2D80u; }
  // and its step
  static float step(uint8_t* base) { return guest::LoadFloat(base, 0x826C2D84u); }
  static void set_step(uint8_t* base, float v) { guest::StoreFloat(base, 0x826C2D84u, v); }
  static constexpr uint32_t step_address() { return 0x826C2D84u; }
  // BoxShape::FaceDirection's static vector (its answer)
  static constexpr uint32_t face_direction_address() { return 0x826C2DE0u; }
};

}  // namespace schema
