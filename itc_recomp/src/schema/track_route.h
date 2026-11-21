// The views of docs/functions/track-route.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/track-route.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: TrackRoute, TrackRoute::LinkDistance.ret, TrackRoute::Normalize.ret
struct TrackRoute : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the route's file (TrackRoute::Load); also SlicePoint's and SplinePoint's out vector's x (a measure fault)
  template <typename T = TrackRouteFile>
  T file() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_file(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_file(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t file_address() const { return address + 0; }
  // measure fault: TrackRoute::ClampPosition's r3 is an out route position (its section)
  uint16_t fault_out_section() const { return guest::LoadU16(base, address + 0); }
  void set_fault_out_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t fault_out_section_address() const { return address + 0; }
  // measure fault: TrackRoute::ClampPosition's out position's slice
  uint16_t fault_out_slice() const { return guest::LoadU16(base, address + 2); }
  void set_fault_out_slice(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t fault_out_slice_address() const { return address + 2; }
  // the slices of every section, worked out on load (the linear index's size, TrackRoute::ToIndex); also the out vectors' y (a measure fault)
  uint32_t slice_total() const { return guest::LoadU32(base, address + 4); }
  void set_slice_total(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slice_total_address() const { return address + 4; }
  // 1 loading, 2 loaded, 0 freed (TrackRoute::Prepare, Free); also the out vectors' z (a measure fault)
  uint32_t state() const { return guest::LoadU32(base, address + 8); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t state_address() const { return address + 8; }
};

// allocated; measure: new<?>@TrackRoute::Load, TrackRoute::LinkDistance.ret.+0, TrackRoute::Normalize.ret.+0
struct TrackRouteFile : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  uint32_t section_count() const { return guest::LoadU32(base, address + 0); }
  void set_section_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t section_count_address() const { return address + 0; }
  // measure fault: TrackRoute::LinkDistance's r4 (a route position's section)
  uint16_t fault_position_section() const { return guest::LoadU16(base, address + 0); }
  void set_fault_position_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t fault_position_section_address() const { return address + 0; }
  // a count (use not known)
  uint32_t count_4() const { return guest::LoadU32(base, address + 4); }
  void set_count_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t count_4_address() const { return address + 4; }
  // a slice's length: a distance in slices times it is in meters (TrackRoute::Distance)
  float slice_length() const { return guest::LoadFloat(base, address + 8); }
  void set_slice_length(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t slice_length_address() const { return address + 8; }
  // the sections' table: a pointer (TrackRouteSection) per section after OnLoaded
  uint32_t sections() const { return guest::LoadU32(base, address + 12); }
  void set_sections(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t sections_address() const { return address + 12; }
  // the corners' table (pointers after OnLoaded; read by the AI drivers' paths)
  uint32_t corners() const { return guest::LoadU32(base, address + 16); }
  void set_corners(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t corners_address() const { return address + 16; }
  // twelve more tables, one per route bit of the race data's +24 (OnLoaded)
  uint32_t route_bit_tables_address() const { return address + 20; }
};

// embedded; measure: TrackRoute::BranchesMatching.r5
struct TrackRouteSection : View {
  using View::View;
  static constexpr uint32_t kSize = 124;
  uint16_t slice_count() const { return guest::LoadU16(base, address + 0); }
  void set_slice_count(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t slice_count_address() const { return address + 0; }
  // low four bits: 0 the plain road; 1 to 4 kinds the links treat apart (2 and 4 skipped going forward, 1 and 3 going back; probably branch or pit lanes); 0x30 and up searched by Follow's full search
  uint8_t kind() const { return guest::LoadU8(base, address + 2); }
  void set_kind(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t kind_address() const { return address + 2; }
  // a branch number: a bit in the masks the callers pass (the default mask leaves out 7, 8 and 29)
  uint8_t branch() const { return guest::LoadU8(base, address + 3); }
  void set_branch(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t branch_address() const { return address + 3; }
  float min_x() const { return guest::LoadFloat(base, address + 4); }
  void set_min_x(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t min_x_address() const { return address + 4; }
  float min_y() const { return guest::LoadFloat(base, address + 8); }
  void set_min_y(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t min_y_address() const { return address + 8; }
  float min_z() const { return guest::LoadFloat(base, address + 12); }
  void set_min_z(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t min_z_address() const { return address + 12; }
  float max_x() const { return guest::LoadFloat(base, address + 16); }
  void set_max_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t max_x_address() const { return address + 16; }
  float max_y() const { return guest::LoadFloat(base, address + 20); }
  void set_max_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t max_y_address() const { return address + 20; }
  float max_z() const { return guest::LoadFloat(base, address + 24); }
  void set_max_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t max_z_address() const { return address + 24; }
  // twelve links (TrackRouteLink): 0 to 2 forward, 3 to 5 back, 6 and 8 the sections beside it (left and right)
  uint32_t links_address() const { return address + 28; }
  // 8 signed bytes a link: the lane taken on that link from each lane, -1 none (TrackRoute::LinkLane)
  uint32_t link_lanes_address() const { return address + 76; }
  // the slices' 84-byte records (TrackRouteSlice), then two tables of 12-byte points at +124 + 84 x count and +124 + 96 x count (probably the road's two edges)
  uint32_t slices_address() const { return address + 124; }
};

// embedded; measure: -
struct TrackRouteLink : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the section it leads to, -1 none
  uint16_t section() const { return guest::LoadU16(base, address + 0); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  // the slice there (signed)
  uint16_t slice() const { return guest::LoadU16(base, address + 2); }
  void set_slice(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t slice_address() const { return address + 2; }
};

// embedded; measure: TrackRoute::AlongSlice.r6, TrackRoute::Locate.r6, TrackRoute::LocateShoulder.r6
struct TrackRouteSlice : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  float center_x() const { return guest::LoadFloat(base, address + 0); }
  void set_center_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t center_x_address() const { return address + 0; }
  float center_y() const { return guest::LoadFloat(base, address + 4); }
  void set_center_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t center_y_address() const { return address + 4; }
  float center_z() const { return guest::LoadFloat(base, address + 8); }
  void set_center_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t center_z_address() const { return address + 8; }
  // the road's width
  float width() const { return guest::LoadFloat(base, address + 12); }
  void set_width(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t width_address() const { return address + 12; }
  // the margin beyond the left side (Locate's 5)
  float left_margin() const { return guest::LoadFloat(base, address + 16); }
  void set_left_margin(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t left_margin_address() const { return address + 16; }
  // the margin beyond the right side (Locate's 6)
  float right_margin() const { return guest::LoadFloat(base, address + 20); }
  void set_right_margin(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t right_margin_address() const { return address + 20; }
  // a float OnLoaded raises or lowers by the race's route bits
  float raised() const { return guest::LoadFloat(base, address + 24); }
  void set_raised(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t raised_address() const { return address + 24; }
  // a float OnLoaded raises or lowers by the race's route bits
  float lowered() const { return guest::LoadFloat(base, address + 28); }
  void set_lowered(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t lowered_address() const { return address + 28; }
  // the collision mesh's section and slice there (halfwords; TrackPosition::Init)
  uint32_t mesh_cell() const { return guest::LoadU32(base, address + 32); }
  void set_mesh_cell(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t mesh_cell_address() const { return address + 32; }
  uint16_t mesh_section() const { return guest::LoadU16(base, address + 32); }
  void set_mesh_section(uint16_t v) const { guest::StoreU16(base, address + 32, v); }
  uint32_t mesh_section_address() const { return address + 32; }
  uint16_t mesh_slice() const { return guest::LoadU16(base, address + 34); }
  void set_mesh_slice(uint16_t v) const { guest::StoreU16(base, address + 34, v); }
  uint32_t mesh_slice_address() const { return address + 34; }
  // the side direction (x, z)
  float side_x() const { return guest::LoadFloat(base, address + 36); }
  void set_side_x(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t side_x_address() const { return address + 36; }
  float side_z() const { return guest::LoadFloat(base, address + 40); }
  void set_side_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t side_z_address() const { return address + 40; }
  // the forward direction (x, z)
  float forward_x() const { return guest::LoadFloat(base, address + 44); }
  void set_forward_x(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t forward_x_address() const { return address + 44; }
  float forward_z() const { return guest::LoadFloat(base, address + 48); }
  void set_forward_z(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t forward_z_address() const { return address + 48; }
  // low four bits: the lanes' count
  uint8_t lanes() const { return guest::LoadU8(base, address + 56); }
  void set_lanes(uint8_t v) const { guest::StoreU8(base, address + 56, v); }
  uint32_t lanes_address() const { return address + 56; }
  // 3 and 4 tested (SliceTest, SpecialSection)
  uint8_t kind() const { return guest::LoadU8(base, address + 57); }
  void set_kind(uint8_t v) const { guest::StoreU8(base, address + 57, v); }
  uint32_t kind_address() const { return address + 57; }
  // the corners listed on the slice (AiPath::AddCornersAt)
  uint16_t corner_count() const { return guest::LoadU16(base, address + 58); }
  void set_corner_count(uint16_t v) const { guest::StoreU16(base, address + 58, v); }
  uint32_t corner_count_address() const { return address + 58; }
  // 4-byte entries: a corner's number in the file's +16 table, a slice
  uint32_t corners_address() const { return address + 60; }
};

// embedded; measure: TrackRoute::AlongSlice.r5, TrackRoute::Distance.r4, TrackRoute::Distance.r5, TrackRoute::Follow.r6, TrackRoute::IsMainRoad.r4, TrackRoute::LaneAt.r5, TrackRoute::LanePoint.r5, TrackRoute::LinkDistance.r4, TrackRoute::LinkDistance.r5, TrackRoute::LinkLane.r4, TrackRoute::Normalize.r4, TrackRoute::NormalizeBack.r4, TrackRoute::NormalizeForward.r4, TrackRoute::NormalizeForwardPath.r4, TrackRoute::NormalizeLane.r4, TrackRoute::NormalizeLaneMasked.r4, TrackRoute::NormalizePath.r4, TrackRoute::NoteShoulder.r5, TrackRoute::NoteShoulder.r6, TrackRoute::OnSlice.r5, TrackRoute::SpecialSection.r4, TrackRoute::StepAlongPath.r4, TrackRoute::StepBack.r4, TrackRoute::StepForward.r4, TrackRoute::StepForwardLane.r4, TrackRoute::StepForwardLaneMasked.r4, TrackRoute::StepForwardMasked.r4, TrackRoute::TakeBranch.r4, TrackRoute::ToMainRoad.r4, sub_820FB2B0.r4
struct TrackRoutePosition : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the section and slice as one word (section in the high halfword)
  uint32_t packed() const { return guest::LoadU32(base, address + 0); }
  void set_packed(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t packed_address() const { return address + 0; }
  // a section of the route (-1, or above 0x7FFF, none)
  uint16_t section() const { return guest::LoadU16(base, address + 0); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  // the slice in it (signed: may run past either end before a normalizer moves it)
  uint16_t slice() const { return guest::LoadU16(base, address + 2); }
  void set_slice(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t slice_address() const { return address + 2; }
};

// embedded; measure: TrackRoute::BranchesMatching.r4
struct TrackRouteBranch : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint16_t section() const { return guest::LoadU16(base, address + 0); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  // the forward link's index (0 to 2)
  uint16_t link() const { return guest::LoadU16(base, address + 2); }
  void set_link(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t link_address() const { return address + 2; }
};

// embedded; measure: TrackRoute::AlongSlice.r4, TrackRoute::LaneAt.r4, TrackRoute::LanePoint.r4, TrackRoute::Locate.r4, TrackRoute::LocateShoulder.r4, TrackRoute::OnSlice.r4, TrackRoute::SectionOutcode.r4, sub_820F8FA0.r4, sub_820F8FA0.r5
struct TrackRoutePoint : View {
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
  // 1.0 (LanePoint's and Point's out)
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
};

// embedded; measure: TrackRoute::BranchesMatching.r6, TrackRoute::StepAlongPath.r5, TrackRoute::StepForward.r5, TrackRoute::StepForwardLane.r6, TrackRoute::StepForwardLaneMasked.r6, TrackRoute::StepForwardMasked.r5, TrackRoute::TakeBranch.r5
struct TrackRouteWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: TrackRoute::StepForwardLaneMasked.r7, TrackRoute::StepForwardMasked.r6
struct TrackRouteMaskList : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the first mask (0 ends the list); the others follow, a word each
  uint32_t first() const { return guest::LoadU32(base, address + 0); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
};

// embedded; measure: TrackRoute::LinkLane.r5, TrackRoute::LinkLane.r6, TrackRoute::StepForwardLane.r5, TrackRoute::StepForwardLaneMasked.r5, TrackRoute::TakeBranch.r5.+0
struct TrackRouteHalfword : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  // signed; TakeBranch.r5.+0 is a measure fault (no pointer is read there)
  uint16_t value() const { return guest::LoadU16(base, address + 0); }
  void set_value(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: TrackRoute::LinkDistance.r6, TrackRoute::NoteShoulder.r7, TrackRoute::OnSlice.r6
struct TrackRouteFloat : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float value() const { return guest::LoadFloat(base, address + 0); }
  void set_value(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: TrackRoute::StepAlongPath.r6
struct TrackRoutePath : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // halfwords: sections, a negative one ends it
  uint32_t entries_address() const { return address + 0; }
  uint16_t first() const { return guest::LoadU16(base, address + 0); }
  void set_first(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
  uint16_t second() const { return guest::LoadU16(base, address + 2); }
  void set_second(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t second_address() const { return address + 2; }
};

// The image words docs/functions/track-route.md's code uses.
struct TrackRouteGlobals {
  // the course's route (TrackRoute, 12 bytes): its file
  template <typename T = TrackRouteFile>
  static T route_file(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B10u)); }
  static void set_route_file(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B10u, v.address); }
  static void set_route_file(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B10u, v); }
  static constexpr uint32_t route_file_address() { return 0x826C5B10u; }
  // the route's slices' total
  static uint32_t route_slice_total(uint8_t* base) { return guest::LoadU32(base, 0x826C5B14u); }
  static void set_route_slice_total(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B14u, v); }
  static constexpr uint32_t route_slice_total_address() { return 0x826C5B14u; }
  // the route's state (1 loading, 2 loaded)
  static uint32_t route_state(uint8_t* base) { return guest::LoadU32(base, 0x826C5B18u); }
  static void set_route_state(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B18u, v); }
  static constexpr uint32_t route_state_address() { return 0x826C5B18u; }
  // 0.5 (.rdata)
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // 20.0, the sections' bounds margin (.rdata)
  static float bounds_margin(uint8_t* base) { return guest::LoadFloat(base, 0x82002568u); }
  static void set_bounds_margin(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002568u, v); }
  static constexpr uint32_t bounds_margin_address() { return 0x82002568u; }
  // 10000.0, no distance (.rdata)
  static float no_distance(uint8_t* base) { return guest::LoadFloat(base, 0x82002814u); }
  static void set_no_distance(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002814u, v); }
  static constexpr uint32_t no_distance_address() { return 0x82002814u; }
  // 9.0 (.rdata)
  static float nine(uint8_t* base) { return guest::LoadFloat(base, 0x82006844u); }
  static void set_nine(uint8_t* base, float v) { guest::StoreFloat(base, 0x82006844u, v); }
  static constexpr uint32_t nine_address() { return 0x82006844u; }
  // 10.0 (.rdata)
  static float ten(uint8_t* base) { return guest::LoadFloat(base, 0x82001FD8u); }
  static void set_ten(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FD8u, v); }
  static constexpr uint32_t ten_address() { return 0x82001FD8u; }
  // 0xDFFFFE7F, then 0: the default branch mask list (.rdata)
  static uint32_t default_branch_mask(uint8_t* base) { return guest::LoadU32(base, 0x82008A00u); }
  static void set_default_branch_mask(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82008A00u, v); }
  static constexpr uint32_t default_branch_mask_address() { return 0x82008A00u; }
  // 100.0, 10 m squared (.rdata)
  static float height_reach(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_height_reach(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t height_reach_address() { return 0x82002A0Cu; }
  // 62500.0, 250 m squared (.rdata)
  static float reach_squared(uint8_t* base) { return guest::LoadFloat(base, 0x8200681Cu); }
  static void set_reach_squared(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200681Cu, v); }
  static constexpr uint32_t reach_squared_address() { return 0x8200681Cu; }
  // FullSearch's first score (.rdata)
  static float farthest(uint8_t* base) { return guest::LoadFloat(base, 0x82006824u); }
  static void set_farthest(uint8_t* base, float v) { guest::StoreFloat(base, 0x82006824u, v); }
  static constexpr uint32_t farthest_address() { return 0x82006824u; }
};

}  // namespace schema
