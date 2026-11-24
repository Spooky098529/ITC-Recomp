// The views of docs/functions/collision-world.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/collision-world.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: TrackMesh, WallQuery::GatherCells.r4
struct TrackMesh : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // an 8-byte block (TrackMesh::Prepare); also the out vectors' x (ColorAt, QuadHeight, TriangleNormal: a measure fault)
  template <typename T = TrackMeshScratch>
  T scratch() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_scratch(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_scratch(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t scratch_address() const { return address + 0; }
  // measure fault: TrackMesh::FindSlice's r3 is a section (its slice count)
  uint16_t fault_section_slice_count() const { return guest::LoadU16(base, address + 0); }
  void set_fault_section_slice_count(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t fault_section_slice_count_address() const { return address + 0; }
  // measure fault: TrackMesh::FindSlice's r3 is a section (its outline's offset)
  uint16_t fault_section_outline() const { return guest::LoadU16(base, address + 2); }
  void set_fault_section_outline(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t fault_section_outline_address() const { return address + 2; }
  // the course's mesh file (VirtualAlloc'd, Archive::Load); also the out vectors' y (a measure fault)
  template <typename T = TrackMeshFile>
  T file() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_file(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_file(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t file_address() const { return address + 4; }
  // 1 loading, 2 loaded, 0 freed (TrackRoute's the same); also the out vectors' z (a measure fault)
  uint32_t state() const { return guest::LoadU32(base, address + 8); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t state_address() const { return address + 8; }
  // measure fault: TrackMesh::TriangleNormal's r3 is the out vector (its w)
  float fault_out_w() const { return guest::LoadFloat(base, address + 12); }
  void set_fault_out_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t fault_out_w_address() const { return address + 12; }
  // measure fault: TrackMesh::SliceContains' r3 is a slice (TrackMeshSlice.corner_0)
  uint16_t fault_slice_corner_0() const { return guest::LoadU16(base, address + 12); }
  void set_fault_slice_corner_0(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t fault_slice_corner_0_address() const { return address + 12; }
  // measure fault: a slice's TrackMeshSlice.corner_3
  uint16_t fault_slice_corner_3() const { return guest::LoadU16(base, address + 18); }
  void set_fault_slice_corner_3(uint16_t v) const { guest::StoreU16(base, address + 18, v); }
  uint32_t fault_slice_corner_3_address() const { return address + 18; }
  // measure fault: a slice's TrackMeshSlice.height
  float fault_slice_height() const { return guest::LoadFloat(base, address + 20); }
  void set_fault_slice_height(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t fault_slice_height_address() const { return address + 20; }
  // measure fault: TrackMesh::TriangleInSlice's r3 is a section (TrackMeshSection.vertices)
  uint32_t fault_section_vertices() const { return guest::LoadU32(base, address + 64); }
  void set_fault_section_vertices(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t fault_section_vertices_address() const { return address + 64; }
  // measure fault: a section's TrackMeshSection.slices
  uint32_t fault_section_slices() const { return guest::LoadU32(base, address + 72); }
  void set_fault_section_slices(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t fault_section_slices_address() const { return address + 72; }
  // measure fault: a section's TrackMeshSection.triangles
  uint32_t fault_section_triangles() const { return guest::LoadU32(base, address + 76); }
  void set_fault_section_triangles(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t fault_section_triangles_address() const { return address + 76; }
};

// allocated; measure: -
struct TrackMeshScratch : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 0
  uint32_t zero() const { return guest::LoadU32(base, address + 0); }
  void set_zero(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t zero_address() const { return address + 0; }
  // an 8 KB block (VirtualAlloc); nothing read here uses it
  uint32_t block() const { return guest::LoadU32(base, address + 4); }
  void set_block(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t block_address() const { return address + 4; }
};

// allocated; measure: TrackMesh.+4, WallQuery::GatherCells.r4.+4, TrackPosition.+4
struct TrackMeshFile : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static TrackMeshFile New(uint8_t* base, uint32_t size) { return TrackMeshFile(base, memory::NewBlob(size, "TrackMeshFile")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint32_t section_count() const { return guest::LoadU32(base, address + 0); }
  void set_section_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t section_count_address() const { return address + 0; }
  // the section table: a pointer to a section a word
  uint32_t sections() const { return guest::LoadU32(base, address + 4); }
  void set_sections(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t sections_address() const { return address + 4; }
};

// embedded; measure: TrackMesh::FindTriangle.r4.+0, sub_820F6C88.r3, TrackMesh::QuadHeight.r4, TrackMesh::TriangleNormal.r4
struct TrackMeshSection : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  uint16_t slice_count() const { return guest::LoadU16(base, address + 0); }
  void set_slice_count(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t slice_count_address() const { return address + 0; }
  // the offset of its outline (8-byte x, z points)
  uint16_t outline() const { return guest::LoadU16(base, address + 2); }
  void set_outline(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t outline_address() const { return address + 2; }
  // six 6-byte links: +0 the slice they leave from (0xFFFF none), +2 the section and +4 the slice they lead to
  uint32_t links_address() const { return address + 4; }
  // the first link's slice it leaves from
  uint16_t link_0_from_slice() const { return guest::LoadU16(base, address + 4); }
  void set_link_0_from_slice(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t link_0_from_slice_address() const { return address + 4; }
  // the first link's section
  uint16_t link_0_section() const { return guest::LoadU16(base, address + 6); }
  void set_link_0_section(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t link_0_section_address() const { return address + 6; }
  // the first link's slice there
  uint16_t link_0_slice() const { return guest::LoadU16(base, address + 8); }
  void set_link_0_slice(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t link_0_slice_address() const { return address + 8; }
  // the bounds' half height (y from center_y - this to center_y + this)
  float half_height() const { return guest::LoadFloat(base, address + 40); }
  void set_half_height(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t half_height_address() const { return address + 40; }
  // the bounds' center height
  float center_y() const { return guest::LoadFloat(base, address + 44); }
  void set_center_y(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t center_y_address() const { return address + 44; }
  float min_x() const { return guest::LoadFloat(base, address + 48); }
  void set_min_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t min_x_address() const { return address + 48; }
  float min_z() const { return guest::LoadFloat(base, address + 52); }
  void set_min_z(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t min_z_address() const { return address + 52; }
  float max_x() const { return guest::LoadFloat(base, address + 56); }
  void set_max_x(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t max_x_address() const { return address + 56; }
  float max_z() const { return guest::LoadFloat(base, address + 60); }
  void set_max_z(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t max_z_address() const { return address + 60; }
  // the offset of its vertices (12 bytes: x, y, z)
  uint32_t vertices() const { return guest::LoadU32(base, address + 64); }
  void set_vertices(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t vertices_address() const { return address + 64; }
  // the offset of a halfword per vertex (probably RGB555 colors)
  uint32_t colors() const { return guest::LoadU32(base, address + 68); }
  void set_colors(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t colors_address() const { return address + 68; }
  // the offset of its 28-byte slices (TrackMeshSlice)
  uint32_t slices() const { return guest::LoadU32(base, address + 72); }
  void set_slices(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t slices_address() const { return address + 72; }
  // the offset of its 12-byte triangles (TrackMeshTriangle)
  uint32_t triangles() const { return guest::LoadU32(base, address + 76); }
  void set_triangles(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t triangles_address() const { return address + 76; }
  // the offset of its 8-byte walls (TrackMeshWall)
  uint32_t walls() const { return guest::LoadU32(base, address + 80); }
  void set_walls(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t walls_address() const { return address + 80; }
};

// embedded; measure: -
struct TrackMeshLink : View {
  using View::View;
  static constexpr uint32_t kSize = 6;
  // the slice of this section it leaves from (0xFFFF unused)
  uint16_t from_slice() const { return guest::LoadU16(base, address + 0); }
  void set_from_slice(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t from_slice_address() const { return address + 0; }
  // the section it leads to
  uint16_t section() const { return guest::LoadU16(base, address + 2); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t section_address() const { return address + 2; }
  // the slice there
  uint16_t slice() const { return guest::LoadU16(base, address + 4); }
  void set_slice(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t slice_address() const { return address + 4; }
};

// embedded; measure: TrackMesh::SliceContains.r4
struct TrackOutlinePoint : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float z() const { return guest::LoadFloat(base, address + 4); }
  void set_z(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t z_address() const { return address + 4; }
};

// embedded; measure: -
struct TrackMeshSlice : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint16_t triangle_count() const { return guest::LoadU16(base, address + 0); }
  void set_triangle_count(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t triangle_count_address() const { return address + 0; }
  uint16_t first_triangle() const { return guest::LoadU16(base, address + 2); }
  void set_first_triangle(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t first_triangle_address() const { return address + 2; }
  uint16_t wall_count() const { return guest::LoadU16(base, address + 4); }
  void set_wall_count(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t wall_count_address() const { return address + 4; }
  uint16_t first_wall() const { return guest::LoadU16(base, address + 6); }
  void set_first_wall(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t first_wall_address() const { return address + 6; }
  // the outline's 3 or 4 corners (halfwords, points of the section's outline)
  uint32_t corners_address() const { return address + 12; }
  uint16_t corner_0() const { return guest::LoadU16(base, address + 12); }
  void set_corner_0(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t corner_0_address() const { return address + 12; }
  // 0xFFFF for three corners
  uint16_t corner_3() const { return guest::LoadU16(base, address + 18); }
  void set_corner_3(uint16_t v) const { guest::StoreU16(base, address + 18, v); }
  uint32_t corner_3_address() const { return address + 18; }
  float height() const { return guest::LoadFloat(base, address + 20); }
  void set_height(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t height_address() const { return address + 20; }
  // 30, 31, 32 give a track position's flags 2, 4, 8
  uint16_t kind() const { return guest::LoadU16(base, address + 24); }
  void set_kind(uint16_t v) const { guest::StoreU16(base, address + 24, v); }
  uint32_t kind_address() const { return address + 24; }
};

// embedded; measure: TrackMesh::ColorAt.r5, TrackMesh::QuadHeight.r5
struct TrackMeshTriangle : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint16_t vertex_0() const { return guest::LoadU16(base, address + 0); }
  void set_vertex_0(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t vertex_0_address() const { return address + 0; }
  uint16_t vertex_1() const { return guest::LoadU16(base, address + 2); }
  void set_vertex_1(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t vertex_1_address() const { return address + 2; }
  uint16_t vertex_2() const { return guest::LoadU16(base, address + 4); }
  void set_vertex_2(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t vertex_2_address() const { return address + 4; }
  // a quad's fourth, or 0xFFFF for a triangle
  uint16_t vertex_3() const { return guest::LoadU16(base, address + 6); }
  void set_vertex_3(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t vertex_3_address() const { return address + 6; }
  uint8_t surface_kind() const { return guest::LoadU8(base, address + 9); }
  void set_surface_kind(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t surface_kind_address() const { return address + 9; }
  // bit 0: a quad with its own height (smooth)
  uint16_t flags() const { return guest::LoadU16(base, address + 10); }
  void set_flags(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t flags_address() const { return address + 10; }
};

// embedded; measure: -
struct TrackMeshWall : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint16_t vertex_0() const { return guest::LoadU16(base, address + 0); }
  void set_vertex_0(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t vertex_0_address() const { return address + 0; }
  uint16_t vertex_1() const { return guest::LoadU16(base, address + 2); }
  void set_vertex_1(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t vertex_1_address() const { return address + 2; }
  // for WallQuery::IgnoresWall (2 to 61)
  uint16_t kind() const { return guest::LoadU16(base, address + 4); }
  void set_kind(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t kind_address() const { return address + 4; }
};

// embedded; measure: TrackPosition, TrackMesh::FindTriangle.r7, TrackPosition::Copy.r3
struct TrackPosition : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  // the mesh section (-1 none)
  uint16_t section() const { return guest::LoadU16(base, address + 0); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  // the section and slice as one word (the wall query's cells); also a point's x: TrackMesh::ColorAt's, GroundHeight's, QuadHeight's and TrackRoute::Follow's, NoteShoulder's, Point's r3 or r4 here is a point (a measure fault)
  uint32_t cell() const { return guest::LoadU32(base, address + 0); }
  void set_cell(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t cell_address() const { return address + 0; }
  // the slice in it (kept by TrackPosition::FindCell)
  uint16_t slice() const { return guest::LoadU16(base, address + 2); }
  void set_slice(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t slice_address() const { return address + 2; }
  // the route position: a section of the course's route (TrackRoute::Follow)
  uint16_t route_section() const { return guest::LoadU16(base, address + 4); }
  void set_route_section(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t route_section_address() const { return address + 4; }
  // the route position (section and slice) as one word; also the mesh's file (TrackPosition::CrossSection's, FindCell's and the searches' r3 or r4 is the mesh) and a point's y (GroundHeight, TrackRoute::): measure faults
  uint32_t route_position() const { return guest::LoadU32(base, address + 4); }
  void set_route_position(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t route_position_address() const { return address + 4; }
  // the route position's slice
  uint16_t route_slice() const { return guest::LoadU16(base, address + 6); }
  void set_route_slice(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t route_slice_address() const { return address + 6; }
  // the point's x and y copied as one (TrackPosition::Copy)
  uint64_t point_xy() const { return guest::LoadU64(base, address + 16); }
  void set_point_xy(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t point_xy_address() const { return address + 16; }
  float point_x() const { return guest::LoadFloat(base, address + 16); }
  void set_point_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t point_x_address() const { return address + 16; }
  // the last ground height under it
  float point_y() const { return guest::LoadFloat(base, address + 20); }
  void set_point_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t point_y_address() const { return address + 20; }
  float point_z() const { return guest::LoadFloat(base, address + 24); }
  void set_point_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t point_z_address() const { return address + 24; }
  uint64_t point_zw() const { return guest::LoadU64(base, address + 24); }
  void set_point_zw(uint64_t v) const { guest::StoreU64(base, address + 24, v); }
  uint32_t point_zw_address() const { return address + 24; }
  // the ground's color there (x, y, z, w)
  uint64_t color_xy() const { return guest::LoadU64(base, address + 32); }
  void set_color_xy(uint64_t v) const { guest::StoreU64(base, address + 32, v); }
  uint32_t color_xy_address() const { return address + 32; }
  float color_x() const { return guest::LoadFloat(base, address + 32); }
  void set_color_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t color_x_address() const { return address + 32; }
  uint64_t color_zw() const { return guest::LoadU64(base, address + 40); }
  void set_color_zw(uint64_t v) const { guest::StoreU64(base, address + 40, v); }
  uint32_t color_zw_address() const { return address + 40; }
  // the ground's normal there (x, y, z, w)
  uint64_t normal_xy() const { return guest::LoadU64(base, address + 48); }
  void set_normal_xy(uint64_t v) const { guest::StoreU64(base, address + 48, v); }
  uint32_t normal_xy_address() const { return address + 48; }
  float normal_x() const { return guest::LoadFloat(base, address + 48); }
  void set_normal_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t normal_x_address() const { return address + 48; }
  float normal_y() const { return guest::LoadFloat(base, address + 52); }
  void set_normal_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t normal_y_address() const { return address + 52; }
  float normal_z() const { return guest::LoadFloat(base, address + 56); }
  void set_normal_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t normal_z_address() const { return address + 56; }
  uint64_t normal_zw() const { return guest::LoadU64(base, address + 56); }
  void set_normal_zw(uint64_t v) const { guest::StoreU64(base, address + 56, v); }
  uint32_t normal_zw_address() const { return address + 56; }
  // copied by TrackPosition::Copy
  uint64_t unknown_64() const { return guest::LoadU64(base, address + 64); }
  void set_unknown_64(uint64_t v) const { guest::StoreU64(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  // copied by TrackPosition::Copy
  uint64_t unknown_72() const { return guest::LoadU64(base, address + 72); }
  void set_unknown_72(uint64_t v) const { guest::StoreU64(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint16_t surface_kind() const { return guest::LoadU16(base, address + 80); }
  void set_surface_kind(uint16_t v) const { guest::StoreU16(base, address + 80, v); }
  uint32_t surface_kind_address() const { return address + 80; }
  // 1 surface kinds 4 and 20; 2, 4, 8 the slice's kind 30, 31, 32
  uint16_t flags() const { return guest::LoadU16(base, address + 82); }
  void set_flags(uint16_t v) const { guest::StoreU16(base, address + 82, v); }
  uint32_t flags_address() const { return address + 82; }
};

// embedded; measure: TrackCollision
struct TrackGroundResult : View {
  using View::View;
  static constexpr uint32_t kSize = 628;
  // the height above the ground
  float depth() const { return guest::LoadFloat(base, address + 0); }
  void set_depth(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t depth_address() const { return address + 0; }
  // measure fault: TrackCollision::GroundHits' r3 is the hit records (the first record's point's z)
  float fault_hits_8() const { return guest::LoadFloat(base, address + 8); }
  void set_fault_hits_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t fault_hits_8_address() const { return address + 8; }
  // the ground's normal
  float normal_x() const { return guest::LoadFloat(base, address + 16); }
  void set_normal_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t normal_x_address() const { return address + 16; }
  float normal_y() const { return guest::LoadFloat(base, address + 20); }
  void set_normal_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t normal_y_address() const { return address + 20; }
  float normal_z() const { return guest::LoadFloat(base, address + 24); }
  void set_normal_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t normal_z_address() const { return address + 24; }
  // the normal again
  float normal_copy_x() const { return guest::LoadFloat(base, address + 32); }
  void set_normal_copy_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t normal_copy_x_address() const { return address + 32; }
  // the surface kind's bits (0x04000000 for none; the ground callback clears 0x7F000000); GroundHits' hit record's depth (a measure fault)
  uint32_t surface() const { return guest::LoadU32(base, address + 48); }
  void set_surface(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t surface_address() const { return address + 48; }
  // the track position used
  template <typename T = TrackPosition>
  T position() const { return T(base, guest::LoadU32(base, address + 52)); }
  void set_position(const View& v) const { guest::StoreU32(base, address + 52, v.address); }
  void set_position(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t position_address() const { return address + 52; }
  // measure fault: GroundHits' hit record's surface bits
  uint32_t fault_hit_surface() const { return guest::LoadU32(base, address + 56); }
  void set_fault_hit_surface(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t fault_hit_surface_address() const { return address + 56; }
  // measure fault: GroundHits' hit record's key
  uint32_t fault_hit_key() const { return guest::LoadU32(base, address + 60); }
  void set_fault_hit_key(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t fault_hit_key_address() const { return address + 60; }
  // measure fault: WallHits' first hit record's time (2.0)
  float fault_hit_time() const { return guest::LoadFloat(base, address + 64); }
  void set_fault_hit_time(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t fault_hit_time_address() const { return address + 64; }
  // measure fault: TrackCollision::QueryWalls' r3 is the wall query (WallQuery.cell_count)
  uint32_t fault_query_cell_count() const { return guest::LoadU32(base, address + 108); }
  void set_fault_query_cell_count(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t fault_query_cell_count_address() const { return address + 108; }
  // measure fault: the wall query's cells
  uint32_t fault_query_cells() const { return guest::LoadU32(base, address + 112); }
  void set_fault_query_cells(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t fault_query_cells_address() const { return address + 112; }
  // measure fault: the wall query's walls_tested
  uint32_t fault_query_walls_tested() const { return guest::LoadU32(base, address + 624); }
  void set_fault_query_walls_tested(uint32_t v) const { guest::StoreU32(base, address + 624, v); }
  uint32_t fault_query_walls_tested_address() const { return address + 624; }
};

// embedded; measure: WallQuery
struct WallQuery : View {
  using View::View;
  static constexpr uint32_t kSize = 1468;
  uint32_t hit_count() const { return guest::LoadU32(base, address + 0); }
  void set_hit_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t hit_count_address() const { return address + 0; }
  // measure fault: WallQuery::IgnoresWall's r3 is the wall's kind's address; ModeSkips' r3 a word's (it fits hit_count)
  uint16_t fault_wall_kind() const { return guest::LoadU16(base, address + 0); }
  void set_fault_wall_kind(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t fault_wall_kind_address() const { return address + 0; }
  // 2.0
  float time() const { return guest::LoadFloat(base, address + 4); }
  void set_time(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t time_address() const { return address + 4; }
  // the collision world's hit records (80 bytes each, car-physics.toml's CollisionHit; the measure's WallQuery.+8)
  template <typename T = CollisionHit>
  T hits() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_hits(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_hits(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t hits_address() const { return address + 8; }
  // the collision object's shape record
  template <typename T = WallQueryShapeRecord>
  T record() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_record(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_record(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t record_address() const { return address + 12; }
  // the wall's first end
  float wall_a_x() const { return guest::LoadFloat(base, address + 16); }
  void set_wall_a_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t wall_a_x_address() const { return address + 16; }
  float wall_a_y() const { return guest::LoadFloat(base, address + 20); }
  void set_wall_a_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t wall_a_y_address() const { return address + 20; }
  float wall_a_z() const { return guest::LoadFloat(base, address + 24); }
  void set_wall_a_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t wall_a_z_address() const { return address + 24; }
  // the wall's second end
  float wall_b_x() const { return guest::LoadFloat(base, address + 32); }
  void set_wall_b_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t wall_b_x_address() const { return address + 32; }
  float wall_b_y() const { return guest::LoadFloat(base, address + 36); }
  void set_wall_b_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t wall_b_y_address() const { return address + 36; }
  float wall_b_z() const { return guest::LoadFloat(base, address + 40); }
  void set_wall_b_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t wall_b_z_address() const { return address + 40; }
  // the hit being made: its point
  uint64_t hit_point_xy() const { return guest::LoadU64(base, address + 48); }
  void set_hit_point_xy(uint64_t v) const { guest::StoreU64(base, address + 48, v); }
  uint32_t hit_point_xy_address() const { return address + 48; }
  float hit_point_x() const { return guest::LoadFloat(base, address + 48); }
  void set_hit_point_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t hit_point_x_address() const { return address + 48; }
  float hit_point_y() const { return guest::LoadFloat(base, address + 52); }
  void set_hit_point_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t hit_point_y_address() const { return address + 52; }
  float hit_point_z() const { return guest::LoadFloat(base, address + 56); }
  void set_hit_point_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t hit_point_z_address() const { return address + 56; }
  uint64_t hit_point_zw() const { return guest::LoadU64(base, address + 56); }
  void set_hit_point_zw(uint64_t v) const { guest::StoreU64(base, address + 56, v); }
  uint32_t hit_point_zw_address() const { return address + 56; }
  // the hit's normal
  uint64_t hit_normal_xy() const { return guest::LoadU64(base, address + 64); }
  void set_hit_normal_xy(uint64_t v) const { guest::StoreU64(base, address + 64, v); }
  uint32_t hit_normal_xy_address() const { return address + 64; }
  float hit_normal_x() const { return guest::LoadFloat(base, address + 64); }
  void set_hit_normal_x(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t hit_normal_x_address() const { return address + 64; }
  float hit_normal_y() const { return guest::LoadFloat(base, address + 68); }
  void set_hit_normal_y(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t hit_normal_y_address() const { return address + 68; }
  float hit_normal_z() const { return guest::LoadFloat(base, address + 72); }
  void set_hit_normal_z(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t hit_normal_z_address() const { return address + 72; }
  uint64_t hit_normal_zw() const { return guest::LoadU64(base, address + 72); }
  void set_hit_normal_zw(uint64_t v) const { guest::StoreU64(base, address + 72, v); }
  uint32_t hit_normal_zw_address() const { return address + 72; }
  // the hit's push direction
  uint64_t hit_push_xy() const { return guest::LoadU64(base, address + 80); }
  void set_hit_push_xy(uint64_t v) const { guest::StoreU64(base, address + 80, v); }
  uint32_t hit_push_xy_address() const { return address + 80; }
  float hit_push_x() const { return guest::LoadFloat(base, address + 80); }
  void set_hit_push_x(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t hit_push_x_address() const { return address + 80; }
  float hit_push_y() const { return guest::LoadFloat(base, address + 84); }
  void set_hit_push_y(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t hit_push_y_address() const { return address + 84; }
  float hit_push_z() const { return guest::LoadFloat(base, address + 88); }
  void set_hit_push_z(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t hit_push_z_address() const { return address + 88; }
  uint64_t hit_push_zw() const { return guest::LoadU64(base, address + 88); }
  void set_hit_push_zw(uint64_t v) const { guest::StoreU64(base, address + 88, v); }
  uint32_t hit_push_zw_address() const { return address + 88; }
  float hit_depth() const { return guest::LoadFloat(base, address + 96); }
  void set_hit_depth(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t hit_depth_address() const { return address + 96; }
  float hit_depth_again() const { return guest::LoadFloat(base, address + 100); }
  void set_hit_depth_again(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t hit_depth_again_address() const { return address + 100; }
  // the wall's number in the query (the hit record's key)
  uint32_t hit_wall() const { return guest::LoadU32(base, address + 104); }
  void set_hit_wall(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t hit_wall_address() const { return address + 104; }
  uint32_t cell_count() const { return guest::LoadU32(base, address + 108); }
  void set_cell_count(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t cell_count_address() const { return address + 108; }
  // the cells near the box: two halfwords each (section, slice); room for 128
  uint32_t cells_address() const { return address + 112; }
  uint32_t walls_tested() const { return guest::LoadU32(base, address + 624); }
  void set_walls_tested(uint32_t v) const { guest::StoreU32(base, address + 624, v); }
  uint32_t walls_tested_address() const { return address + 624; }
  // the walls tested: 6 bytes each (section, two vertices), at most 128
  uint32_t walls_address() const { return address + 628; }
  // the wall's direction
  float direction_x() const { return guest::LoadFloat(base, address + 1408); }
  void set_direction_x(float v) const { guest::StoreFloat(base, address + 1408, v); }
  uint32_t direction_x_address() const { return address + 1408; }
  float direction_y() const { return guest::LoadFloat(base, address + 1412); }
  void set_direction_y(float v) const { guest::StoreFloat(base, address + 1412, v); }
  uint32_t direction_y_address() const { return address + 1412; }
  float direction_z() const { return guest::LoadFloat(base, address + 1416); }
  void set_direction_z(float v) const { guest::StoreFloat(base, address + 1416, v); }
  uint32_t direction_z_address() const { return address + 1416; }
  float direction_w() const { return guest::LoadFloat(base, address + 1420); }
  void set_direction_w(float v) const { guest::StoreFloat(base, address + 1420, v); }
  uint32_t direction_w_address() const { return address + 1420; }
  // the direction negated
  float negated_x() const { return guest::LoadFloat(base, address + 1424); }
  void set_negated_x(float v) const { guest::StoreFloat(base, address + 1424, v); }
  uint32_t negated_x_address() const { return address + 1424; }
  float negated_y() const { return guest::LoadFloat(base, address + 1428); }
  void set_negated_y(float v) const { guest::StoreFloat(base, address + 1428, v); }
  uint32_t negated_y_address() const { return address + 1428; }
  float negated_z() const { return guest::LoadFloat(base, address + 1432); }
  void set_negated_z(float v) const { guest::StoreFloat(base, address + 1432, v); }
  uint32_t negated_z_address() const { return address + 1432; }
  // the wall's horizontal normal
  uint64_t normal_xy() const { return guest::LoadU64(base, address + 1440); }
  void set_normal_xy(uint64_t v) const { guest::StoreU64(base, address + 1440, v); }
  uint32_t normal_xy_address() const { return address + 1440; }
  float normal_x() const { return guest::LoadFloat(base, address + 1440); }
  void set_normal_x(float v) const { guest::StoreFloat(base, address + 1440, v); }
  uint32_t normal_x_address() const { return address + 1440; }
  float normal_y() const { return guest::LoadFloat(base, address + 1444); }
  void set_normal_y(float v) const { guest::StoreFloat(base, address + 1444, v); }
  uint32_t normal_y_address() const { return address + 1444; }
  float normal_z() const { return guest::LoadFloat(base, address + 1448); }
  void set_normal_z(float v) const { guest::StoreFloat(base, address + 1448, v); }
  uint32_t normal_z_address() const { return address + 1448; }
  uint64_t normal_zw() const { return guest::LoadU64(base, address + 1448); }
  void set_normal_zw(uint64_t v) const { guest::StoreU64(base, address + 1448, v); }
  uint32_t normal_zw_address() const { return address + 1448; }
  float normal_w() const { return guest::LoadFloat(base, address + 1452); }
  void set_normal_w(float v) const { guest::StoreFloat(base, address + 1452, v); }
  uint32_t normal_w_address() const { return address + 1452; }
  // the wall's first end along its direction
  float span_a() const { return guest::LoadFloat(base, address + 1456); }
  void set_span_a(float v) const { guest::StoreFloat(base, address + 1456, v); }
  uint32_t span_a_address() const { return address + 1456; }
  // its second end along the direction
  float span_b() const { return guest::LoadFloat(base, address + 1460); }
  void set_span_b(float v) const { guest::StoreFloat(base, address + 1460, v); }
  uint32_t span_b_address() const { return address + 1460; }
  // the normal's distance
  float normal_distance() const { return guest::LoadFloat(base, address + 1464); }
  void set_normal_distance(float v) const { guest::StoreFloat(base, address + 1464, v); }
  uint32_t normal_distance_address() const { return address + 1464; }
};

// embedded; measure: -
struct TrackTestedWall : View {
  using View::View;
  static constexpr uint32_t kSize = 6;
  uint16_t section() const { return guest::LoadU16(base, address + 0); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  uint16_t vertex_0() const { return guest::LoadU16(base, address + 2); }
  void set_vertex_0(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t vertex_0_address() const { return address + 2; }
  uint16_t vertex_1() const { return guest::LoadU16(base, address + 4); }
  void set_vertex_1(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t vertex_1_address() const { return address + 4; }
};

// embedded; measure: WallQuery.+12
struct WallQueryShapeRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the template shape (a box)
  uint32_t template_shape() const { return guest::LoadU32(base, address + 0); }
  void set_template_shape(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t template_shape_address() const { return address + 0; }
};

// embedded; measure: TrackMesh::ColorAt.r6, Segments.+0, Segments.+4, Segments.+8, Segments.+12, Segments::Intersect2D.r4, TrackMesh::SliceContains.r5, TrackMesh::TriangleInSlice.r5, sub_820F6C88.r4, sub_820F7D70.r4, TrackCollision::Ground.r4, BoxShape::LowerCorner.ret, sub_820B97C0.ret
struct TrackQueryPoint : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint64_t xy() const { return guest::LoadU64(base, address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  uint64_t zw() const { return guest::LoadU64(base, address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
};

// embedded; measure: Segments
struct TrackSegments : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  template <typename T = TrackQueryPoint>
  T a() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_a(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_a(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t a_address() const { return address + 0; }
  template <typename T = TrackQueryPoint>
  T b() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_b(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_b(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t b_address() const { return address + 4; }
  template <typename T = TrackQueryPoint>
  T c() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_c(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_c(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t c_address() const { return address + 8; }
  template <typename T = TrackQueryPoint>
  T d() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_d(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_d(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t d_address() const { return address + 12; }
};

// embedded; measure: TrackCollision::GroundHits.r4
struct TrackGroundHitsShape : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = TrackGroundHitsShapeVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// embedded; measure: TrackCollision::GroundHits.r4.+0
struct TrackGroundHitsShapeVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 88;
  // slot 4: the shape's type (2 a box)
  uint32_t type() const { return guest::LoadU32(base, address + 16); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t type_address() const { return address + 16; }
  // slot 11
  uint32_t corner_count() const { return guest::LoadU32(base, address + 44); }
  void set_corner_count(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t corner_count_address() const { return address + 44; }
  // slot 12
  uint32_t corner() const { return guest::LoadU32(base, address + 48); }
  void set_corner(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t corner_address() const { return address + 48; }
  // slot 20 (BoxShape::LowerCorner, probably)
  uint32_t slot_20() const { return guest::LoadU32(base, address + 80); }
  void set_slot_20(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t slot_20_address() const { return address + 80; }
  // slot 21: BoxShape::CacheWord
  uint32_t cache_word() const { return guest::LoadU32(base, address + 84); }
  void set_cache_word(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t cache_word_address() const { return address + 84; }
};

// embedded; measure: TrackMesh::FindTriangle.r4
struct TrackFoundSection : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = TrackMeshSection>
  T section() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_section(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_section(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t section_address() const { return address + 0; }
};

// embedded; measure: TrackMesh::FindTriangle.r5
struct TrackFoundTriangle : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the triangle's index in the section
  uint32_t triangle() const { return guest::LoadU32(base, address + 0); }
  void set_triangle(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t triangle_address() const { return address + 0; }
};

// embedded; measure: sub_820F6C88.r5
struct TrackNearestDistance : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float distance() const { return guest::LoadFloat(base, address + 0); }
  void set_distance(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t distance_address() const { return address + 0; }
};

// allocated; measure: new<4>@TrackCollision::Load
#pragma pack(push, 1)
struct TrackCollisionObjectData {
  uint32_t value = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(TrackCollisionObjectData) == 4);
inline constexpr memory::LayoutField kTrackCollisionObjectFields[] = {{0, 4}};
inline constexpr memory::Layout kTrackCollisionObjectLayout = {kTrackCollisionObjectFields, 1};

struct TrackCollisionObject : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  TrackCollisionObjectData* data() const { return static_cast<TrackCollisionObjectData*>(memory::HandleObject(address)); }
  static TrackCollisionObject New(uint8_t* base) {
    return TrackCollisionObject(base, memory::MakeHandle(new TrackCollisionObjectData(), sizeof(TrackCollisionObjectData), "TrackCollisionObject", &kTrackCollisionObjectLayout));
  }
  template <uint32_t Stride = kSize>
  static TrackCollisionObject NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(TrackCollisionObjectData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<TrackCollisionObjectData*>(::operator new(sizeof(TrackCollisionObjectData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) TrackCollisionObjectData();
    return TrackCollisionObject(base, memory::MakeHandle(p, sizeof(TrackCollisionObjectData) * count, "TrackCollisionObject", &kTrackCollisionObjectLayout, sizeof(TrackCollisionObjectData)));
  }
  TrackCollisionObject at(uint32_t i) const { return TrackCollisionObject(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  TrackCollisionObjectData* direct() const { return static_cast<TrackCollisionObjectData*>(memory::HandleOwn(address, &kTrackCollisionObjectLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kTrackCollisionObjectLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTrackCollisionObjectLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0, then 4; what it is isn't known
  uint32_t value_address() const { return address + 0; }
  uint32_t value_bits() const { if (auto* d = direct()) return d->value; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_value_bits(uint32_t v) const { if (auto* d = direct()) { d->value = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t value() const { if (auto* d = direct()) return d->value; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_value(uint32_t v) const { if (auto* d = direct()) { d->value = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
};

// embedded; measure: TrackPosition@+16
struct TrackPositionAt16 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: TrackPosition@+48
struct TrackPositionAt48 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: TrackPosition@+32
struct TrackMeshAt32 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): TrackMesh::TriangleNormal's r3 is the out vector (its w)
  float out_w() const { return guest::LoadFloat(base, address + 12); }
  void set_out_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t out_w_address() const { return address + 12; }
};

// The image words docs/functions/collision-world.md's code uses.
struct CollisionWorldGlobals {
  // the track's collision mesh (TrackMesh)
  static constexpr uint32_t mesh_address() { return 0x826C5B1Cu; }
  // made by TrackCollision::Load, deleted by Unload (the pointer kept)
  template <typename T = TrackCollisionObject>
  static T collision_object(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B28u)); }
  static void set_collision_object(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B28u, v.address); }
  static void set_collision_object(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B28u, v); }
  static constexpr uint32_t collision_object_address() { return 0x826C5B28u; }
  // TrackPosition::SearchLinked's linked sections tried (halfwords, -1 after the last)
  static constexpr uint32_t linked_tried_address() { return 0x826C5B04u; }
  // the collision world's step (probably 1/30)
  static float tick_step(uint8_t* base) { return guest::LoadFloat(base, 0x8260327Cu); }
  static void set_tick_step(uint8_t* base, float v) { guest::StoreFloat(base, 0x8260327Cu, v); }
  static constexpr uint32_t tick_step_address() { return 0x8260327Cu; }
  // a vector, probably the up axis (TrackCollision::Ground without a track position)
  static constexpr uint32_t up_address() { return 0x82638280u; }
  // a vector, probably the up axis (the wall query)
  static constexpr uint32_t up_too_address() { return 0x826382B0u; }
  // 0.5 (the ground's raise)
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // probably -1.0
  static float minus_one(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE0u); }
  static void set_minus_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // a point's y within this of a slice's height
  static float height_tolerance(uint8_t* base) { return guest::LoadFloat(base, 0x82001BDCu); }
  static void set_height_tolerance(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BDCu, v); }
  static constexpr uint32_t height_tolerance_address() { return 0x82001BDCu; }
  // the margin around a section's bounds
  static float bounds_margin(uint8_t* base) { return guest::LoadFloat(base, 0x82001FD8u); }
  static void set_bounds_margin(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FD8u, v); }
  static constexpr uint32_t bounds_margin_address() { return 0x82001FD8u; }
  // each axis from a slice outline's center
  static float center_reach(uint8_t* base) { return guest::LoadFloat(base, 0x82002A28u); }
  static void set_center_reach(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A28u, v); }
  static constexpr uint32_t center_reach_address() { return 0x82002A28u; }
  static float center_reach_squared(uint8_t* base) { return guest::LoadFloat(base, 0x8200681Cu); }
  static void set_center_reach_squared(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200681Cu, v); }
  static constexpr uint32_t center_reach_squared_address() { return 0x8200681Cu; }
  // TrackPosition::SearchAll's first best distance
  static float farthest(uint8_t* base) { return guest::LoadFloat(base, 0x82006824u); }
  static void set_farthest(uint8_t* base, float v) { guest::StoreFloat(base, 0x82006824u, v); }
  static constexpr uint32_t farthest_address() { return 0x82006824u; }
  // an edge's cross product at most this is inside
  static float inside_epsilon(uint8_t* base) { return guest::LoadFloat(base, 0x820029D8u); }
  static void set_inside_epsilon(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029D8u, v); }
  static constexpr uint32_t inside_epsilon_address() { return 0x820029D8u; }
  // 0.005, added to every ground height and again for kinds 4 and 20
  static float height_offset(uint8_t* base) { return guest::LoadFloat(base, 0x82002A08u); }
  static void set_height_offset(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A08u, v); }
  static constexpr uint32_t height_offset_address() { return 0x82002A08u; }
  // 100.0, the ground's catch distance
  static float catch_distance(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_catch_distance(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t catch_distance_address() { return 0x82002A0Cu; }
  // four words: the slice offsets 1, -1, 2, -2
  static constexpr uint32_t slice_offsets_address() { return 0x8200680Cu; }
  // probably 0.25
  static float quarter(uint8_t* base) { return guest::LoadFloat(base, 0x82002968u); }
  static void set_quarter(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002968u, v); }
  static constexpr uint32_t quarter_address() { return 0x82002968u; }
  // 2.0
  static float two(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_two(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t two_address() { return 0x820021E8u; }
  // the small epsilon collision.md names
  static float small_epsilon(uint8_t* base) { return guest::LoadFloat(base, 0x8200222Cu); }
  static void set_small_epsilon(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200222Cu, v); }
  static constexpr uint32_t small_epsilon_address() { return 0x8200222Cu; }
  // WallQuery::AddHit's depth factor
  static float hit_depth_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820029DCu); }
  static void set_hit_depth_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029DCu, v); }
  static constexpr uint32_t hit_depth_scale_address() { return 0x820029DCu; }
  // TrackMesh::ColorAt's scale of the channels
  static float color_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82006820u); }
  static void set_color_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82006820u, v); }
  static constexpr uint32_t color_scale_address() { return 0x82006820u; }
  // taken off the height for kinds 4 and 20 with no other triangle
  static float no_other_lowering(uint8_t* base) { return guest::LoadFloat(base, 0x82002FB0u); }
  static void set_no_other_lowering(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002FB0u, v); }
  static constexpr uint32_t no_other_lowering_address() { return 0x82002FB0u; }
  // the wall query: the ends' moves toward the wall at least this
  static float slope_floor(uint8_t* base) { return guest::LoadFloat(base, 0x82006908u); }
  static void set_slope_floor(uint8_t* base, float v) { guest::StoreFloat(base, 0x82006908u, v); }
  static constexpr uint32_t slope_floor_address() { return 0x82006908u; }
  // the wall query: the faces' normals scaled by this
  static float outward(uint8_t* base) { return guest::LoadFloat(base, 0x8200690Cu); }
  static void set_outward(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200690Cu, v); }
  static constexpr uint32_t outward_address() { return 0x8200690Cu; }
  // the wall query's corner reach
  static float corner_reach(uint8_t* base) { return guest::LoadFloat(base, 0x82002CB0u); }
  static void set_corner_reach(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002CB0u, v); }
  static constexpr uint32_t corner_reach_address() { return 0x82002CB0u; }
  // the wall query's corner margin
  static float corner_margin(uint8_t* base) { return guest::LoadFloat(base, 0x82001FBCu); }
  static void set_corner_margin(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FBCu, v); }
  static constexpr uint32_t corner_margin_address() { return 0x82001FBCu; }
  // the order of a box's first 8 corners (bytes)
  static constexpr uint32_t corner_order_address() { return 0x82006900u; }
  // the race data (race-states.toml): +4 the course route's index, +20 the branch flags, +24 the route bits, +48 the racer records, +0x59AE4 the stage flags, +0xA3AF4 the race mode
  template <typename T = RaceData>
  static T race_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B2Cu, v.address); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the race's progress (race-states.toml): +6100 read by WallQuery::ModeSkips for mode 5
  template <typename T = RaceProgress>
  static T race_progress(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82E8u)); }
  static void set_race_progress(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82E8u, v.address); }
  static void set_race_progress(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82E8u, v); }
  static constexpr uint32_t race_progress_address() { return 0x826D82E8u; }
  // the racers' manager (race-world.toml): +2308 the racers, +2364 their count
  template <typename T = RaceManager>
  static T racers_manager(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82F4u)); }
  static void set_racers_manager(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82F4u, v.address); }
  static void set_racers_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82F4u, v); }
  static constexpr uint32_t racers_manager_address() { return 0x826D82F4u; }
};

}  // namespace schema
