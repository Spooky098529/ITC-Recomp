// The views of docs/functions/vehicle-accessors.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/vehicle-accessors.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: CollisionObject, CollisionObject.+228, CollisionObject.+232, new<72>@CollisionWorld::Create.+0, sub_820B7CB8.r3, sub_820B7CC0.r3
struct CollisionObject : View {
  using View::View;
  static constexpr uint32_t kSize = 320;
  static constexpr uint32_t kVtable = 0x82002A94;
  // the objects' block count header (the first object's -16)
  uint32_t array_count() const { return guest::LoadU32(base, address + -16); }
  void set_array_count(uint32_t v) const { guest::StoreU32(base, address + -16, v); }
  uint32_t array_count_address() const { return address + -16; }
  // 0x82002A94
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // its rigid body
  template <typename T = RigidBody>
  T body() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_body(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_body(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t body_address() const { return address + 16; }
  // its shape (the vehicle's +344 box)
  template <typename T = BoxShape>
  T shape() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_shape(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_shape(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t shape_address() const { return address + 20; }
  // the embedded shape record's (+32 to +144) template shape
  template <typename T = BoxShape>
  T shape_template() const { return T(base, guest::LoadU32(base, address + 32)); }
  void set_shape_template(const View& v) const { guest::StoreU32(base, address + 32, v.address); }
  void set_shape_template(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t shape_template_address() const { return address + 32; }
  // the shape record's previous placed copy
  uint32_t shape_previous() const { return guest::LoadU32(base, address + 36); }
  void set_shape_previous(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t shape_previous_address() const { return address + 36; }
  // the shape record's current placed copy (goes to the second query)
  uint32_t shape_current() const { return guest::LoadU32(base, address + 40); }
  void set_shape_current(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t shape_current_address() const { return address + 40; }
  // the shape record's +24
  uint32_t unknown_56() const { return guest::LoadU32(base, address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  // minus the lowest wheel travel (the shape record's +112)
  float lowest_travel() const { return guest::LoadFloat(base, address + 144); }
  void set_lowest_travel(float v) const { guest::StoreFloat(base, address + 144, v); }
  uint32_t lowest_travel_address() const { return address + 144; }
  // the largest push per axis this tick
  float push_x() const { return guest::LoadFloat(base, address + 160); }
  void set_push_x(float v) const { guest::StoreFloat(base, address + 160, v); }
  uint32_t push_x_address() const { return address + 160; }
  float push_y() const { return guest::LoadFloat(base, address + 164); }
  void set_push_y(float v) const { guest::StoreFloat(base, address + 164, v); }
  uint32_t push_y_address() const { return address + 164; }
  float push_z() const { return guest::LoadFloat(base, address + 168); }
  void set_push_z(float v) const { guest::StoreFloat(base, address + 168, v); }
  uint32_t push_z_address() const { return address + 168; }
  float push_w() const { return guest::LoadFloat(base, address + 172); }
  void set_push_w(float v) const { guest::StoreFloat(base, address + 172, v); }
  uint32_t push_w_address() const { return address + 172; }
  // the summed torque from pairs this tick
  float pair_torque_x() const { return guest::LoadFloat(base, address + 176); }
  void set_pair_torque_x(float v) const { guest::StoreFloat(base, address + 176, v); }
  uint32_t pair_torque_x_address() const { return address + 176; }
  float pair_torque_y() const { return guest::LoadFloat(base, address + 180); }
  void set_pair_torque_y(float v) const { guest::StoreFloat(base, address + 180, v); }
  uint32_t pair_torque_y_address() const { return address + 180; }
  float pair_torque_z() const { return guest::LoadFloat(base, address + 184); }
  void set_pair_torque_z(float v) const { guest::StoreFloat(base, address + 184, v); }
  uint32_t pair_torque_z_address() const { return address + 184; }
  float pair_torque_w() const { return guest::LoadFloat(base, address + 188); }
  void set_pair_torque_w(float v) const { guest::StoreFloat(base, address + 188, v); }
  uint32_t pair_torque_w_address() const { return address + 188; }
  // how many pushes
  uint32_t push_count() const { return guest::LoadU32(base, address + 192); }
  void set_push_count(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t push_count_address() const { return address + 192; }
  // a per-axis scale of the push-out (1, 1, 1 from CollisionObject::Init); sub_820B7D68 sets it
  float push_scale_x() const { return guest::LoadFloat(base, address + 208); }
  void set_push_scale_x(float v) const { guest::StoreFloat(base, address + 208, v); }
  uint32_t push_scale_x_address() const { return address + 208; }
  float push_scale_y() const { return guest::LoadFloat(base, address + 212); }
  void set_push_scale_y(float v) const { guest::StoreFloat(base, address + 212, v); }
  uint32_t push_scale_y_address() const { return address + 212; }
  float push_scale_z() const { return guest::LoadFloat(base, address + 216); }
  void set_push_scale_z(float v) const { guest::StoreFloat(base, address + 216, v); }
  uint32_t push_scale_z_address() const { return address + 216; }
  float push_scale_w() const { return guest::LoadFloat(base, address + 220); }
  void set_push_scale_w(float v) const { guest::StoreFloat(base, address + 220, v); }
  uint32_t push_scale_w_address() const { return address + 220; }
  // sub_820B7D70 sets it
  float torque_scale() const { return guest::LoadFloat(base, address + 224); }
  void set_torque_scale(float v) const { guest::StoreFloat(base, address + 224, v); }
  uint32_t torque_scale_address() const { return address + 224; }
  // the next in the world's active or free list
  template <typename T = CollisionObject>
  T next() const { return T(base, guest::LoadU32(base, address + 228)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 228, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t next_address() const { return address + 228; }
  template <typename T = CollisionObject>
  T previous() const { return T(base, guest::LoadU32(base, address + 232)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 232, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 232, v); }
  uint32_t previous_address() const { return address + 232; }
  uint32_t unknown_236() const { return guest::LoadU32(base, address + 236); }
  void set_unknown_236(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t unknown_236_address() const { return address + 236; }
  // its contacts, linked by their +68
  template <typename T = CollisionContact>
  T first_contact() const { return T(base, guest::LoadU32(base, address + 240)); }
  void set_first_contact(const View& v) const { guest::StoreU32(base, address + 240, v.address); }
  void set_first_contact(uint32_t v) const { guest::StoreU32(base, address + 240, v); }
  uint32_t first_contact_address() const { return address + 240; }
  // its own groups (CollisionObject::AddGroups, RemoveGroups)
  uint32_t groups() const { return guest::LoadU32(base, address + 244); }
  void set_groups(uint32_t v) const { guest::StoreU32(base, address + 244, v); }
  uint32_t groups_address() const { return address + 244; }
  // the groups it collides with
  uint32_t collides_with() const { return guest::LoadU32(base, address + 248); }
  void set_collides_with(uint32_t v) const { guest::StoreU32(base, address + 248, v); }
  uint32_t collides_with_address() const { return address + 248; }
  // a bit mask (sub_820B7D18 clears bits)
  uint32_t mask() const { return guest::LoadU32(base, address + 252); }
  void set_mask(uint32_t v) const { guest::StoreU32(base, address + 252, v); }
  uint32_t mask_address() const { return address + 252; }
  // the surface bits that get a response (sub_820B7D28 sets, sub_820B7D38 clears)
  uint32_t response_bits() const { return guest::LoadU32(base, address + 256); }
  void set_response_bits(uint32_t v) const { guest::StoreU32(base, address + 256, v); }
  uint32_t response_bits_address() const { return address + 256; }
  // the surface bits that push it out (sub_820B7D48 sets, sub_820B7D58 clears)
  uint32_t push_out_bits() const { return guest::LoadU32(base, address + 260); }
  void set_push_out_bits(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t push_out_bits_address() const { return address + 260; }
  // the head of its pair list (sub_820B7CB8, sub_820B7CC0)
  template <typename T = CollisionPair>
  T first_pair() const { return T(base, guest::LoadU32(base, address + 264)); }
  void set_first_pair(const View& v) const { guest::StoreU32(base, address + 264, v.address); }
  void set_first_pair(uint32_t v) const { guest::StoreU32(base, address + 264, v); }
  uint32_t first_pair_address() const { return address + 264; }
  // objects with the same nonzero id don't pair
  uint32_t owner_id() const { return guest::LoadU32(base, address + 268); }
  void set_owner_id(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t owner_id_address() const { return address + 268; }
  // set: no queries
  uint32_t no_queries() const { return guest::LoadU32(base, address + 272); }
  void set_no_queries(uint32_t v) const { guest::StoreU32(base, address + 272, v); }
  uint32_t no_queries_address() const { return address + 272; }
  uint32_t unknown_276() const { return guest::LoadU32(base, address + 276); }
  void set_unknown_276(uint32_t v) const { guest::StoreU32(base, address + 276, v); }
  uint32_t unknown_276_address() const { return address + 276; }
  // set: friction from the +344 object alone
  uint32_t box_friction() const { return guest::LoadU32(base, address + 280); }
  void set_box_friction(uint32_t v) const { guest::StoreU32(base, address + 280, v); }
  uint32_t box_friction_address() const { return address + 280; }
  // 0 after a move
  uint32_t unmoved() const { return guest::LoadU32(base, address + 284); }
  void set_unmoved(uint32_t v) const { guest::StoreU32(base, address + 284, v); }
  uint32_t unmoved_address() const { return address + 284; }
  // set when moved (sub_820B7998)
  uint32_t moved() const { return guest::LoadU32(base, address + 288); }
  void set_moved(uint32_t v) const { guest::StoreU32(base, address + 288, v); }
  uint32_t moved_address() const { return address + 288; }
  // this pass's push-out
  float push_out_x() const { return guest::LoadFloat(base, address + 304); }
  void set_push_out_x(float v) const { guest::StoreFloat(base, address + 304, v); }
  uint32_t push_out_x_address() const { return address + 304; }
  float push_out_y() const { return guest::LoadFloat(base, address + 308); }
  void set_push_out_y(float v) const { guest::StoreFloat(base, address + 308, v); }
  uint32_t push_out_y_address() const { return address + 308; }
  float push_out_z() const { return guest::LoadFloat(base, address + 312); }
  void set_push_out_z(float v) const { guest::StoreFloat(base, address + 312, v); }
  uint32_t push_out_z_address() const { return address + 312; }
  float push_out_w() const { return guest::LoadFloat(base, address + 316); }
  void set_push_out_w(float v) const { guest::StoreFloat(base, address + 316, v); }
  uint32_t push_out_w_address() const { return address + 316; }
};

// embedded; measure: CollisionRecords, sub_820B7D78.r3
struct CollisionHitRecords : View {
  using View::View;
  static constexpr uint32_t kSize = 160;
  uint32_t point_address() const { return address + 0; }
  // probably the surface normal
  uint32_t normal_address() const { return address + 16; }
  // the push direction
  uint32_t push_address() const { return address + 32; }
  float depth() const { return guest::LoadFloat(base, address + 48); }
  void set_depth(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t depth_address() const { return address + 48; }
  // copied as a word (sub_820B7D78)
  uint32_t depth_bits() const { return guest::LoadU32(base, address + 48); }
  void set_depth_bits(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t depth_bits_address() const { return address + 48; }
  // the depth again (walls)
  float depth_2() const { return guest::LoadFloat(base, address + 52); }
  void set_depth_2(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t depth_2_address() const { return address + 52; }
  uint32_t depth_2_bits() const { return guest::LoadU32(base, address + 52); }
  void set_depth_2_bits(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t depth_2_bits_address() const { return address + 52; }
  uint32_t surface_bits() const { return guest::LoadU32(base, address + 56); }
  void set_surface_bits(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t surface_bits_address() const { return address + 56; }
  // hits with the same key are merged
  uint32_t key() const { return guest::LoadU32(base, address + 60); }
  void set_key(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t key_address() const { return address + 60; }
  float time() const { return guest::LoadFloat(base, address + 64); }
  void set_time(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t time_address() const { return address + 64; }
  uint32_t time_bits() const { return guest::LoadU32(base, address + 64); }
  void set_time_bits(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t time_bits_address() const { return address + 64; }
  // the second record's +48
  float hit1_depth() const { return guest::LoadFloat(base, address + 128); }
  void set_hit1_depth(float v) const { guest::StoreFloat(base, address + 128, v); }
  uint32_t hit1_depth_address() const { return address + 128; }
  // the second record's +60
  uint32_t hit1_key() const { return guest::LoadU32(base, address + 140); }
  void set_hit1_key(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t hit1_key_address() const { return address + 140; }
};

// embedded; measure: Stages, Stages::Copy.r4
struct Stages : View {
  using View::View;
  static constexpr uint32_t kSize = 1224;
  // stage 0's +0 (the stages: RaceStage at + 80 x i)
  uint32_t stage0_type() const { return guest::LoadU32(base, address + 0); }
  void set_stage0_type(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t stage0_type_address() const { return address + 0; }
  uint32_t stage0_word_4() const { return guest::LoadU32(base, address + 4); }
  void set_stage0_word_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t stage0_word_4_address() const { return address + 4; }
  uint32_t stage0_word_8() const { return guest::LoadU32(base, address + 8); }
  void set_stage0_word_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t stage0_word_8_address() const { return address + 8; }
  uint32_t stage0_racer0() const { return guest::LoadU32(base, address + 44); }
  void set_stage0_racer0(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t stage0_racer0_address() const { return address + 44; }
  uint32_t stage0_racer0_word() const { return guest::LoadU32(base, address + 48); }
  void set_stage0_racer0_word(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t stage0_racer0_word_address() const { return address + 48; }
  uint32_t stage0_slot_count() const { return guest::LoadU32(base, address + 76); }
  void set_stage0_slot_count(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t stage0_slot_count_address() const { return address + 76; }
  // the stage count
  uint32_t count() const { return guest::LoadU32(base, address + 800); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 800, v); }
  uint32_t count_address() const { return address + 800; }
  // the current stage (-1 in the accessors means it)
  uint32_t current() const { return guest::LoadU32(base, address + 804); }
  void set_current(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t current_address() const { return address + 804; }
  // sub_820FFAB0 sets it, sub_820FFC68 reads it
  uint32_t word_808() const { return guest::LoadU32(base, address + 808); }
  void set_word_808(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t word_808_address() const { return address + 808; }
  // a course record (race-states.toml's)
  uint32_t course_record_address() const { return address + 812; }
  // a word Stages::Clear keeps
  uint32_t kept_word() const { return guest::LoadU32(base, address + 1220); }
  void set_kept_word(uint32_t v) const { guest::StoreU32(base, address + 1220, v); }
  uint32_t kept_word_address() const { return address + 1220; }
};

// embedded; measure: -
struct RaceStage : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  // 2 and 3 answer sub_820FFC30; 4 is never added (the reset's)
  uint32_t type() const { return guest::LoadU32(base, address + 0); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t type_address() const { return address + 0; }
  uint32_t word_4() const { return guest::LoadU32(base, address + 4); }
  void set_word_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t word_4_address() const { return address + 4; }
  uint32_t word_8() const { return guest::LoadU32(base, address + 8); }
  void set_word_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t word_8_address() const { return address + 8; }
  // its route (a StageRoute, Stages::SetRoute)
  uint32_t route_address() const { return address + 24; }
  // slot i's racer index at +44 + 8 x i (-1 ends them)
  uint32_t racer0() const { return guest::LoadU32(base, address + 44); }
  void set_racer0(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t racer0_address() const { return address + 44; }
  // slot i's word at +48 + 8 x i
  uint32_t racer0_word() const { return guest::LoadU32(base, address + 48); }
  void set_racer0_word(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t racer0_word_address() const { return address + 48; }
  // its slot count (Stages::AddRacer)
  uint32_t slot_count() const { return guest::LoadU32(base, address + 76); }
  void set_slot_count(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t slot_count_address() const { return address + 76; }
};

// embedded; measure: RacerRecords, RacerRecords::At.r3
struct RacerRecordArray : View {
  using View::View;
  static constexpr uint32_t kSize = 303076;
  uint32_t count() const { return guest::LoadU32(base, address + 303072); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 303072, v); }
  uint32_t count_address() const { return address + 303072; }
};

// file; measure: List
struct AccessorList : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // the first node
  template <typename T = AccessorListNode>
  T first() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_first(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t count() const { return guest::LoadU32(base, address + 8); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t count_address() const { return address + 8; }
  // another list kind's (the measure's)
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // another list kind's (the measure's)
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// file; measure: List::At.ret, List.+0
struct AccessorListNode : View {
  using View::View;
  static constexpr uint32_t kSize = 5076;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  template <typename T = AccessorListNode>
  T next() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_address() const { return address + 8; }
  // 0 free, 3 in use (ScenePools::Claim, Get)
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // the second set's pool 7's model kind (ScenePools::FindModel)
  uint32_t kind() const { return guest::LoadU32(base, address + 32); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t kind_address() const { return address + 32; }
  // a measure fault (another node type's)
  uint32_t fault_4296() const { return guest::LoadU32(base, address + 4296); }
  void set_fault_4296(uint32_t v) const { guest::StoreU32(base, address + 4296, v); }
  uint32_t fault_4296_address() const { return address + 4296; }
  // a measure fault (another node type's)
  uint32_t fault_5040() const { return guest::LoadU32(base, address + 5040); }
  void set_fault_5040(uint32_t v) const { guest::StoreU32(base, address + 5040, v); }
  uint32_t fault_5040_address() const { return address + 5040; }
  // a measure fault (another node type's)
  uint32_t fault_5048() const { return guest::LoadU32(base, address + 5048); }
  void set_fault_5048(uint32_t v) const { guest::StoreU32(base, address + 5048, v); }
  uint32_t fault_5048_address() const { return address + 5048; }
  // a measure fault (another node type's)
  uint32_t fault_5068() const { return guest::LoadU32(base, address + 5068); }
  void set_fault_5068(uint32_t v) const { guest::StoreU32(base, address + 5068, v); }
  uint32_t fault_5068_address() const { return address + 5068; }
  // a measure fault (another node type's)
  uint32_t fault_5072() const { return guest::LoadU32(base, address + 5072); }
  void set_fault_5072(uint32_t v) const { guest::StoreU32(base, address + 5072, v); }
  uint32_t fault_5072_address() const { return address + 5072; }
};

// embedded; measure: Vec3, Vec4, Vec3::Add.r4, Vec3::Add.r5, Vec3::Sub.r4, Vec3::Sub.r5, Vec3::Cross.r4, Vec3::Cross.r5, Vec3::CrossW1.r4, Vec3::CrossW1.r5, Vec3::Direction.r4, Vec3::Direction.r5, Vec3::Normalize.r4, Vec4::Copy.r4, Vec4::ScaleTo.r4, Matrix::SetTranslation.r4, Matrix::TransformPoint.r5, Quaternion::ToMatrix3.r4, sub_82293CD8.r4, sub_82293CD8.ret, sub_82294360.r3, sub_822943C8.r3, sub_82294470.r3, sub_822944B0.r3
struct VectorLibVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  uint32_t x_bits() const { return guest::LoadU32(base, address + 0); }
  void set_x_bits(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t x_bits_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  uint32_t y_bits() const { return guest::LoadU32(base, address + 4); }
  void set_y_bits(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t y_bits_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  uint32_t z_bits() const { return guest::LoadU32(base, address + 8); }
  void set_z_bits(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t z_bits_address() const { return address + 8; }
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
  uint32_t w_bits() const { return guest::LoadU32(base, address + 12); }
  void set_w_bits(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t w_bits_address() const { return address + 12; }
};

// embedded; measure: Matrix::Rotate.r4, Matrix::TransformPoint.r4, sub_820B97D8.r4
struct VectorLibMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  float m00() const { return guest::LoadFloat(base, address + 0); }
  void set_m00(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t m00_address() const { return address + 0; }
  float m01() const { return guest::LoadFloat(base, address + 4); }
  void set_m01(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t m01_address() const { return address + 4; }
  float m02() const { return guest::LoadFloat(base, address + 8); }
  void set_m02(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t m02_address() const { return address + 8; }
  float m03() const { return guest::LoadFloat(base, address + 12); }
  void set_m03(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t m03_address() const { return address + 12; }
  float m10() const { return guest::LoadFloat(base, address + 16); }
  void set_m10(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t m10_address() const { return address + 16; }
  float m11() const { return guest::LoadFloat(base, address + 20); }
  void set_m11(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t m11_address() const { return address + 20; }
  float m12() const { return guest::LoadFloat(base, address + 24); }
  void set_m12(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t m12_address() const { return address + 24; }
  float m13() const { return guest::LoadFloat(base, address + 28); }
  void set_m13(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t m13_address() const { return address + 28; }
  float m20() const { return guest::LoadFloat(base, address + 32); }
  void set_m20(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t m20_address() const { return address + 32; }
  float m21() const { return guest::LoadFloat(base, address + 36); }
  void set_m21(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t m21_address() const { return address + 36; }
  float m22() const { return guest::LoadFloat(base, address + 40); }
  void set_m22(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t m22_address() const { return address + 40; }
  float m23() const { return guest::LoadFloat(base, address + 44); }
  void set_m23(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t m23_address() const { return address + 44; }
  // the translation's x
  float m30() const { return guest::LoadFloat(base, address + 48); }
  void set_m30(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t m30_address() const { return address + 48; }
  uint32_t m30_bits() const { return guest::LoadU32(base, address + 48); }
  void set_m30_bits(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t m30_bits_address() const { return address + 48; }
  float m31() const { return guest::LoadFloat(base, address + 52); }
  void set_m31(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t m31_address() const { return address + 52; }
  uint32_t m31_bits() const { return guest::LoadU32(base, address + 52); }
  void set_m31_bits(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t m31_bits_address() const { return address + 52; }
  float m32() const { return guest::LoadFloat(base, address + 56); }
  void set_m32(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t m32_address() const { return address + 56; }
  uint32_t m32_bits() const { return guest::LoadU32(base, address + 56); }
  void set_m32_bits(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t m32_bits_address() const { return address + 56; }
  float m33() const { return guest::LoadFloat(base, address + 60); }
  void set_m33(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t m33_address() const { return address + 60; }
  uint32_t m33_bits() const { return guest::LoadU32(base, address + 60); }
  void set_m33_bits(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t m33_bits_address() const { return address + 60; }
};

// embedded; measure: sub_820FFE98.r3, sub_820FFE98.r4, sub_820FFEB0.r3, sub_820FFF10.r3
struct AccessorPair : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint16_t first() const { return guest::LoadU16(base, address + 0); }
  void set_first(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
  // the two as one word (sub_820FFF10's (-1, -1))
  uint32_t both() const { return guest::LoadU32(base, address + 0); }
  void set_both(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t both_address() const { return address + 0; }
  uint16_t second() const { return guest::LoadU16(base, address + 2); }
  void set_second(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t second_address() const { return address + 2; }
};

// embedded; measure: sub_820FFEB0.r4
struct AccessorPairs : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint16_t pair0_first() const { return guest::LoadU16(base, address + 0); }
  void set_pair0_first(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t pair0_first_address() const { return address + 0; }
  uint16_t pair4_first() const { return guest::LoadU16(base, address + 16); }
  void set_pair4_first(uint16_t v) const { guest::StoreU16(base, address + 16, v); }
  uint32_t pair4_first_address() const { return address + 16; }
  uint16_t pair4_second() const { return guest::LoadU16(base, address + 18); }
  void set_pair4_second(uint16_t v) const { guest::StoreU16(base, address + 18, v); }
  uint32_t pair4_second_address() const { return address + 18; }
};

// embedded; measure: sub_820B9680.r3, sub_82293F68.r4, sub_82293FD0.r4
struct Sub820B9680Shape : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // probably a radius
  float unknown_32() const { return guest::LoadFloat(base, address + 32); }
  void set_unknown_32(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
};

// embedded; measure: sub_820B9680.r4
struct Sub820B9680Record : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: sub_820BA228.r3, sub_820BA688.r3
struct Sub820BA688Object : View {
  using View::View;
  static constexpr uint32_t kSize = 468;
  float unknown_32() const { return guest::LoadFloat(base, address + 32); }
  void set_unknown_32(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  float unknown_128() const { return guest::LoadFloat(base, address + 128); }
  void set_unknown_128(float v) const { guest::StoreFloat(base, address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  float unknown_132() const { return guest::LoadFloat(base, address + 132); }
  void set_unknown_132(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t unknown_132_address() const { return address + 132; }
  float unknown_136() const { return guest::LoadFloat(base, address + 136); }
  void set_unknown_136(float v) const { guest::StoreFloat(base, address + 136, v); }
  uint32_t unknown_136_address() const { return address + 136; }
  float unknown_144() const { return guest::LoadFloat(base, address + 144); }
  void set_unknown_144(float v) const { guest::StoreFloat(base, address + 144, v); }
  uint32_t unknown_144_address() const { return address + 144; }
  float unknown_148() const { return guest::LoadFloat(base, address + 148); }
  void set_unknown_148(float v) const { guest::StoreFloat(base, address + 148, v); }
  uint32_t unknown_148_address() const { return address + 148; }
  float unknown_152() const { return guest::LoadFloat(base, address + 152); }
  void set_unknown_152(float v) const { guest::StoreFloat(base, address + 152, v); }
  uint32_t unknown_152_address() const { return address + 152; }
  float unknown_176() const { return guest::LoadFloat(base, address + 176); }
  void set_unknown_176(float v) const { guest::StoreFloat(base, address + 176, v); }
  uint32_t unknown_176_address() const { return address + 176; }
  float unknown_180() const { return guest::LoadFloat(base, address + 180); }
  void set_unknown_180(float v) const { guest::StoreFloat(base, address + 180, v); }
  uint32_t unknown_180_address() const { return address + 180; }
  float unknown_184() const { return guest::LoadFloat(base, address + 184); }
  void set_unknown_184(float v) const { guest::StoreFloat(base, address + 184, v); }
  uint32_t unknown_184_address() const { return address + 184; }
  float unknown_304() const { return guest::LoadFloat(base, address + 304); }
  void set_unknown_304(float v) const { guest::StoreFloat(base, address + 304, v); }
  uint32_t unknown_304_address() const { return address + 304; }
  float unknown_336() const { return guest::LoadFloat(base, address + 336); }
  void set_unknown_336(float v) const { guest::StoreFloat(base, address + 336, v); }
  uint32_t unknown_336_address() const { return address + 336; }
  float unknown_368() const { return guest::LoadFloat(base, address + 368); }
  void set_unknown_368(float v) const { guest::StoreFloat(base, address + 368, v); }
  uint32_t unknown_368_address() const { return address + 368; }
  float unknown_400() const { return guest::LoadFloat(base, address + 400); }
  void set_unknown_400(float v) const { guest::StoreFloat(base, address + 400, v); }
  uint32_t unknown_400_address() const { return address + 400; }
  float unknown_432() const { return guest::LoadFloat(base, address + 432); }
  void set_unknown_432(float v) const { guest::StoreFloat(base, address + 432, v); }
  uint32_t unknown_432_address() const { return address + 432; }
  float unknown_464() const { return guest::LoadFloat(base, address + 464); }
  void set_unknown_464(float v) const { guest::StoreFloat(base, address + 464, v); }
  uint32_t unknown_464_address() const { return address + 464; }
};

// embedded; measure: sub_820BB450.r3
struct Sub820BB450Object : View {
  using View::View;
  static constexpr uint32_t kSize = 140;
  float unknown_128() const { return guest::LoadFloat(base, address + 128); }
  void set_unknown_128(float v) const { guest::StoreFloat(base, address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  float unknown_132() const { return guest::LoadFloat(base, address + 132); }
  void set_unknown_132(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t unknown_132_address() const { return address + 132; }
  float unknown_136() const { return guest::LoadFloat(base, address + 136); }
  void set_unknown_136(float v) const { guest::StoreFloat(base, address + 136, v); }
  uint32_t unknown_136_address() const { return address + 136; }
};

// embedded; measure: sub_820BD740.r3
struct Sub820BD740Object : View {
  using View::View;
  static constexpr uint32_t kSize = 92;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_84() const { return guest::LoadFloat(base, address + 84); }
  void set_unknown_84(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  float unknown_88() const { return guest::LoadFloat(base, address + 88); }
  void set_unknown_88(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
};

// The image words docs/functions/vehicle-accessors.md's code uses.
struct VehicleAccessorsGlobals {
  // the seven 6,272-byte vehicles (a pointer)
  static uint32_t vehicles(uint8_t* base) { return guest::LoadU32(base, 0x826C2E34u); }
  static void set_vehicles(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E34u, v); }
  static constexpr uint32_t vehicles_address() { return 0x826C2E34u; }
  // player 0's save data (a pointer); player i's at + 4 x i (SaveData::Of)
  static uint32_t save_data(uint8_t* base) { return guest::LoadU32(base, 0x826C5B58u); }
  static void set_save_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B58u, v); }
  static constexpr uint32_t save_data_address() { return 0x826C5B58u; }
  // the race data (a pointer; race-states.toml's RaceData)
  static uint32_t race_data(uint8_t* base) { return guest::LoadU32(base, 0x826C5B2Cu); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the first query function (sub_820B8768 sets it)
  static uint32_t query_walls(uint8_t* base) { return guest::LoadU32(base, 0x826C2D78u); }
  static void set_query_walls(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2D78u, v); }
  static constexpr uint32_t query_walls_address() { return 0x826C2D78u; }
  // the second query function (sub_820B8778 sets it)
  static uint32_t query_ground(uint8_t* base) { return guest::LoadU32(base, 0x826C2D7Cu); }
  static void set_query_ground(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2D7Cu, v); }
  static constexpr uint32_t query_ground_address() { return 0x826C2D7Cu; }
  // the collision world (a pointer; car-physics.toml's CollisionWorld)
  static uint32_t collision_world(uint8_t* base) { return guest::LoadU32(base, 0x826C2D88u); }
  static void set_collision_world(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2D88u, v); }
  static constexpr uint32_t collision_world_address() { return 0x826C2D88u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 1.0, copied as a word
  static uint32_t one_bits(uint8_t* base) { return guest::LoadU32(base, 0x82000A30u); }
  static void set_one_bits(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A30u, v); }
  static constexpr uint32_t one_bits_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 0.0, copied as a word
  static uint32_t zero_bits(uint8_t* base) { return guest::LoadU32(base, 0x82000A34u); }
  static void set_zero_bits(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_bits_address() { return 0x82000A34u; }
  // 2.0 (probably): Quaternion::ToMatrix3's scale
  static float quaternion_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_quaternion_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t quaternion_scale_address() { return 0x820021E8u; }
  // 3.6 (m/s to km/h)
  static float kmh_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820029E0u); }
  static void set_kmh_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029E0u, v); }
  static constexpr uint32_t kmh_scale_address() { return 0x820029E0u; }
  // 1/30
  static float frame_seconds(uint8_t* base) { return guest::LoadFloat(base, 0x820029ECu); }
  static void set_frame_seconds(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029ECu, v); }
  static constexpr uint32_t frame_seconds_address() { return 0x820029ECu; }
  // Vehicle::WheelSlip's threshold scale (1/3.6, ai-drivers.toml)
  static float slip_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002BD8u); }
  static void set_slip_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002BD8u, v); }
  static constexpr uint32_t slip_scale_address() { return 0x82002BD8u; }
};

}  // namespace schema
