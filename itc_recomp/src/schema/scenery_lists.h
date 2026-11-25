// The views of docs/functions/scenery-lists.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/scenery-lists.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: CourseSet@+3180, ModelList::Load.r3, ModelList::Init.r3, ModelList::Release.r3, PoolObject@+3180
struct ModelList : View {
  using View::View;
  static constexpr uint32_t kSize = 624;
  // the models, from the container's little-endian first word
  uint32_t count() const { return guest::LoadU32(address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // the List64's 64 nodes (4 bytes each: +0 the next node, +2 the data slot; halfwords)
  uint32_t nodes_address() const { return address + 4; }
  // node 0's next (the nodes' stores, from ModelList::Init and Load)
  uint16_t node0_next() const { return guest::LoadU16(address + 4); }
  void set_node0_next(uint16_t v) const { guest::StoreU16(address + 4, v); }
  uint32_t node0_next_address() const { return address + 4; }
  // node 0's data slot
  uint16_t node0_slot() const { return guest::LoadU16(address + 6); }
  void set_node0_slot(uint16_t v) const { guest::StoreU16(address + 6, v); }
  uint32_t node0_slot_address() const { return address + 6; }
  // the List64's +252: node 63's next, -1 when made empty
  uint16_t node63_next() const { return guest::LoadU16(address + 256); }
  void set_node63_next(uint16_t v) const { guest::StoreU16(address + 256, v); }
  uint32_t node63_next_address() const { return address + 256; }
  // the List64's +256: the free list's head node
  uint16_t free_head() const { return guest::LoadU16(address + 260); }
  void set_free_head(uint16_t v) const { guest::StoreU16(address + 260, v); }
  uint32_t free_head_address() const { return address + 260; }
  // the List64's +258: -1 when made empty (not read here)
  uint16_t list_258() const { return guest::LoadU16(address + 262); }
  void set_list_258(uint16_t v) const { guest::StoreU16(address + 262, v); }
  uint32_t list_258_address() const { return address + 262; }
  // the List64's +260: the first node (-1 empty)
  uint16_t head() const { return guest::LoadU16(address + 264); }
  void set_head(uint16_t v) const { guest::StoreU16(address + 264, v); }
  uint32_t head_address() const { return address + 264; }
  // the List64's +262: -1 when made empty (not read here; probably the tail)
  uint16_t list_262() const { return guest::LoadU16(address + 266); }
  void set_list_262(uint16_t v) const { guest::StoreU16(address + 266, v); }
  uint32_t list_262_address() const { return address + 266; }
  // the List64's +264: its count (List64::Insert's position at the end)
  uint16_t list_count() const { return guest::LoadU16(address + 268); }
  void set_list_count(uint16_t v) const { guest::StoreU16(address + 268, v); }
  uint32_t list_count_address() const { return address + 268; }
  // the List64's +268: the 64 data slots, each a model (SceneryModel)
  uint32_t slots_address() const { return address + 272; }
  // the point list's: 20 pointers to point groups (0x82617750 + 256 x i, ModelList::LoadPoints; each 64-byte groups of four 16-byte points)
  uint32_t point_groups_address() const { return address + 528; }
  // the point list's first group pointer (0x82617750)
  uint32_t point_group_0() const { return guest::LoadU32(address + 528); }
  void set_point_group_0(uint32_t v) const { guest::StoreU32(address + 528, v); }
  uint32_t point_group_0_address() const { return address + 528; }
  // the second (0x82617850)
  uint32_t point_group_1() const { return guest::LoadU32(address + 532); }
  void set_point_group_1(uint32_t v) const { guest::StoreU32(address + 532, v); }
  uint32_t point_group_1_address() const { return address + 532; }
  // the point list's: the course set's texture animations (+1060), set by Scenery::DrawSetLists; 0 from LoadPoints
  template <typename T = AnimationSet>
  T animations() const { return T(guest::LoadU32(address + 608)); }
  void set_animations(const View& v) const { guest::StoreU32(address + 608, v.address); }
  void set_animations(uint32_t v) const { guest::StoreU32(address + 608, v); }
  uint32_t animations_address() const { return address + 608; }
  // the point list's: the scenery manager's texture provider, set by Scenery::DrawSetLists; 0 from LoadPoints
  uint32_t texture_provider() const { return guest::LoadU32(address + 612); }
  void set_texture_provider(uint32_t v) const { guest::StoreU32(address + 612, v); }
  uint32_t texture_provider_address() const { return address + 612; }
  // the point list's: the first group drawn with the first model (groups to 2; from the race's table at [0x826D82E8]); 0 from LoadPoints
  int32_t first_group() const { return int32_t(guest::LoadU32(address + 616)); }
  void set_first_group(int32_t v) const { guest::StoreU32(address + 616, uint32_t(v)); }
  uint32_t first_group_address() const { return address + 616; }
  // the point list's: 20 (LoadPoints)
  uint32_t point_group_count() const { return guest::LoadU32(address + 620); }
  void set_point_group_count(uint32_t v) const { guest::StoreU32(address + 620, v); }
  uint32_t point_group_count_address() const { return address + 620; }
};

// file; measure: ModelList::Load.r4
struct SceneryListContainer : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the entries' count, little-endian (byte-swapped by the original)
  uint32_t count_le() const { return guest::LoadU32(address + 0); }
  void set_count_le(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t count_le_address() const { return address + 0; }
  // its first byte, the count's low byte (the original's lbz in the swap)
  uint8_t count_le_low() const { return guest::LoadU8(address + 0); }
  void set_count_le_low(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t count_le_low_address() const { return address + 0; }
};

// allocated; measure: SceneryModel
struct SceneryModel : View {
  using View::View;
  static constexpr uint32_t kSize = 448;
  static constexpr uint32_t kVtable = 0x82006764;
  // 0x82006764
  template <typename T = SceneryModelVtable>
  T vtable() const { return T(guest::LoadU32(address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the model's own shader pairs (16 bytes a pair, by the material's +96)
  template <typename T = ModelShaderPair>
  T shader_pairs() const { return T(guest::LoadU32(address + 24)); }
  void set_shader_pairs(const View& v) const { guest::StoreU32(address + 24, v.address); }
  void set_shader_pairs(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t shader_pairs_address() const { return address + 24; }
  // the Model's header block: 0 (not loaded) draws nothing
  uint32_t header() const { return guest::LoadU32(address + 28); }
  void set_header(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t header_address() const { return address + 28; }
  // 544 bytes a shape
  template <typename T = ModelShape>
  T shapes() const { return T(guest::LoadU32(address + 40)); }
  void set_shapes(const View& v) const { guest::StoreU32(address + 40, v.address); }
  void set_shapes(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t shapes_address() const { return address + 40; }
  // the scenery manager's provider (+228), or the point list's +612
  template <typename T = TextureProvider>
  T texture_provider() const { return T(guest::LoadU32(address + 48)); }
  void set_texture_provider(const View& v) const { guest::StoreU32(address + 48, v.address); }
  void set_texture_provider(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t texture_provider_address() const { return address + 48; }
  // the Model's float4 table (16 bytes an entry): the textures' colors by the material's +208
  uint32_t float4_values() const { return guest::LoadU32(address + 320); }
  void set_float4_values(uint32_t v) const { guest::StoreU32(address + 320, v); }
  uint32_t float4_values_address() const { return address + 320; }
  // the samplers' state 24 and the shapes' state 28 value: 1 by ResetDrawState
  uint32_t sampler_state_24() const { return guest::LoadU32(address + 432); }
  void set_sampler_state_24(uint32_t v) const { guest::StoreU32(address + 432, v); }
  uint32_t sampler_state_24_address() const { return address + 432; }
  // the texture animations (the course set's +1060) or 0 (ResetDrawState)
  template <typename T = AnimationSet>
  T animations() const { return T(guest::LoadU32(address + 436)); }
  void set_animations(const View& v) const { guest::StoreU32(address + 436, v.address); }
  void set_animations(uint32_t v) const { guest::StoreU32(address + 436, v); }
  uint32_t animations_address() const { return address + 436; }
  // 0 by ResetDrawState; not read here
  uint32_t unknown_440() const { return guest::LoadU32(address + 440); }
  void set_unknown_440(uint32_t v) const { guest::StoreU32(address + 440, v); }
  uint32_t unknown_440_address() const { return address + 440; }
};

// image; measure: SceneryModel.+0
struct SceneryModelVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 0, the deleting destructor sub_820F1208 (r4 = 1 from ModelList::Release)
  uint32_t destroy() const { return guest::LoadU32(address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1, the per-draw hook (empty, sub_82106540)
  uint32_t draw_hook() const { return guest::LoadU32(address + 4); }
  void set_draw_hook(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t draw_hook_address() const { return address + 4; }
};

// allocated; measure: Recorder::RecordObject.ret
struct RecordedObjectConstants : View {
  using View::View;
  static constexpr uint32_t kSize = 160;
  // c3's z (the slots from +16): the draw's depth for the render queue (negated for group 0 and the depth draws)
  float depth() const { return guest::LoadFloat(address + 72); }
  void set_depth(float v) const { guest::StoreFloat(address + 72, v); }
  uint32_t depth_address() const { return address + 72; }
};

// allocated; measure: TextureProvider.+8
struct TextureProviderSlots : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // slot 0's texture (the slots follow, 4 bytes each, to the vector's end +12)
  uint32_t texture() const { return guest::LoadU32(address + 0); }
  void set_texture(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t texture_address() const { return address + 0; }
};

// embedded; measure: PoolObject.+3792.+0
struct SceneryShapeStreams : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // the shape's +276: the second stream's kind (6: two streams)
  uint32_t second_kind() const { return guest::LoadU32(address + 20); }
  void set_second_kind(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t second_kind_address() const { return address + 20; }
  // the shape's +280: the second stream's stride
  uint32_t second_stride() const { return guest::LoadU32(address + 24); }
  void set_second_stride(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t second_stride_address() const { return address + 24; }
};

// The image words docs/functions/scenery-lists.md's code uses.
struct SceneryListsGlobals {
  // the race data (a pointer): +4 the course, +20 flags, +24 the +2124 list's entries' flags, +0x59620 the stages, +0x59AE4 flags, +0xA3AF0 the race kind, +0xA3AF4 the race mode
  static uint32_t race_data() { return guest::LoadU32(0x826C5B2Cu); }
  static void set_race_data(uint32_t v) { guest::StoreU32(0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the depth draws' vertex declaration (VertexDeclarations::ForDepth)
  static uint32_t depth_declaration() { return guest::LoadU32(0x8296B8CCu); }
  static void set_depth_declaration(uint32_t v) { guest::StoreU32(0x8296B8CCu, v); }
  static constexpr uint32_t depth_declaration_address() { return 0x8296B8CCu; }
  // the depth draws' vertex declaration for stream type 11
  static uint32_t depth_declaration_type11() { return guest::LoadU32(0x8296B8D0u); }
  static void set_depth_declaration_type11(uint32_t v) { guest::StoreU32(0x8296B8D0u, v); }
  static constexpr uint32_t depth_declaration_type11_address() { return 0x8296B8D0u; }
  // the scene's command list (scene + 8), the recorders'
  static uint32_t record_list() { return guest::LoadU32(0x8296B918u); }
  static void set_record_list(uint32_t v) { guest::StoreU32(0x8296B918u, v); }
  static constexpr uint32_t record_list_address() { return 0x8296B918u; }
  // the scene's render queue (scene + 64), its slot 4 the draws'
  static uint32_t render_queue() { return guest::LoadU32(0x8296B91Cu); }
  static void set_render_queue(uint32_t v) { guest::StoreU32(0x8296B91Cu, v); }
  static constexpr uint32_t render_queue_address() { return 0x8296B91Cu; }
  // the depth-only draws' sort key (Model::DrawShapeDepth)
  static uint32_t depth_key() { return guest::LoadU32(0x8296B8FCu); }
  static void set_depth_key(uint32_t v) { guest::StoreU32(0x8296B8FCu, v); }
  static constexpr uint32_t depth_key_address() { return 0x8296B8FCu; }
  // group 1's sort key
  static uint32_t group1_key() { return guest::LoadU32(0x8296B900u); }
  static void set_group1_key(uint32_t v) { guest::StoreU32(0x8296B900u, v); }
  static constexpr uint32_t group1_key_address() { return 0x8296B900u; }
  // the depth pass's sort key (SceneryModel::DrawShape's depth entry)
  static uint32_t scenery_depth_key() { return guest::LoadU32(0x8296B908u); }
  static void set_scenery_depth_key(uint32_t v) { guest::StoreU32(0x8296B908u, v); }
  static constexpr uint32_t scenery_depth_key_address() { return 0x8296B908u; }
  // group 0's sort key
  static uint32_t group0_key() { return guest::LoadU32(0x8296B90Cu); }
  static void set_group0_key(uint32_t v) { guest::StoreU32(0x8296B90Cu, v); }
  static constexpr uint32_t group0_key_address() { return 0x8296B90Cu; }
  // the point list's 20 groups of 256 bytes (ModelList::LoadPoints)
  static constexpr uint32_t point_groups_address() { return 0x82617750u; }
  // 1.0
  static float one() { return guest::LoadFloat(0x82000A30u); }
  static void set_one(float v) { guest::StoreFloat(0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero() { return guest::LoadFloat(0x82000A34u); }
  static void set_zero(float v) { guest::StoreFloat(0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // -1.0
  static float minus_one() { return guest::LoadFloat(0x82001BE0u); }
  static void set_minus_one(float v) { guest::StoreFloat(0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // the visibility test's upper bound
  static float visible_right() { return guest::LoadFloat(0x82002B90u); }
  static void set_visible_right(float v) { guest::StoreFloat(0x82002B90u, v); }
  static constexpr uint32_t visible_right_address() { return 0x82002B90u; }
  // the visibility test's lower bound
  static float visible_left() { return guest::LoadFloat(0x8206CDC4u); }
  static void set_visible_left(float v) { guest::StoreFloat(0x8206CDC4u, v); }
  static constexpr uint32_t visible_left_address() { return 0x8206CDC4u; }
  // the lowest point y drawn (-1000.0)
  static float point_low() { return guest::LoadFloat(0x82006808u); }
  static void set_point_low(float v) { guest::StoreFloat(0x82006808u, v); }
  static constexpr uint32_t point_low_address() { return 0x82006808u; }
  // a point's w to an angle
  static float point_turn() { return guest::LoadFloat(0x82002948u); }
  static void set_point_turn(float v) { guest::StoreFloat(0x82002948u, v); }
  static constexpr uint32_t point_turn_address() { return 0x82002948u; }
  // the points' second turn
  static float point_turn2() { return guest::LoadFloat(0x82002A50u); }
  static void set_point_turn2(float v) { guest::StoreFloat(0x82002A50u, v); }
  static constexpr uint32_t point_turn2_address() { return 0x82002A50u; }
  // the points' range, near
  static float point_range_near() { return guest::LoadFloat(0x82002814u); }
  static void set_point_range_near(float v) { guest::StoreFloat(0x82002814u, v); }
  static constexpr uint32_t point_range_near_address() { return 0x82002814u; }
  // the points' range, far
  static float point_range_far() { return guest::LoadFloat(0x82006804u); }
  static void set_point_range_far(float v) { guest::StoreFloat(0x82006804u, v); }
  static constexpr uint32_t point_range_far_address() { return 0x82006804u; }
  // the view matrix (pixel c0-c3 of the scene lights)
  static constexpr uint32_t view_address() { return 0x8265ECA0u; }
  // the camera's position (vertex c8, pixel c4)
  static constexpr uint32_t camera_position_address() { return 0x8265ECE0u; }
  // the headlight projectors' origin (x, y, z)
  static constexpr uint32_t projector_origin_address() { return 0x8265ED00u; }
  // the depth draws' pixel c43
  static constexpr uint32_t depth_constant_address() { return 0x8265ED10u; }
  // vertex c86 put back after a draw
  static constexpr uint32_t range_after_address() { return 0x8265ED20u; }
  // pixel c76 put back after a draw
  static constexpr uint32_t pixel_c76_after_address() { return 0x8265ED30u; }
  // the scenery's render state 3 by blend index
  static constexpr uint32_t blend_states_address() { return 0x82008E6Cu; }
  // Model::RecordShape's render state 3 by blend index
  static constexpr uint32_t model_blend_states_address() { return 0x8265EEB4u; }
  // Model::RecordMaterials' render state 3 by blend index
  static constexpr uint32_t car_blend_states_address() { return 0x826018A4u; }
  // the cull modes 2 and 6 swapped when set
  static uint32_t cull_swap() { return guest::LoadU32(0x826C5100u); }
  static void set_cull_swap(uint32_t v) { guest::StoreU32(0x826C5100u, v); }
  static constexpr uint32_t cull_swap_address() { return 0x826C5100u; }
  // render state 48 0 when set (else 1)
  static uint32_t render_state_48_off() { return guest::LoadU32(0x826C5104u); }
  static void set_render_state_48_off(uint32_t v) { guest::StoreU32(0x826C5104u, v); }
  static constexpr uint32_t render_state_48_off_address() { return 0x826C5104u; }
  // sampler 6's texture, or 0 (the provider's slot 6)
  static uint32_t sampler6_texture() { return guest::LoadU32(0x8296B8F0u); }
  static void set_sampler6_texture(uint32_t v) { guest::StoreU32(0x8296B8F0u, v); }
  static constexpr uint32_t sampler6_texture_address() { return 0x8296B8F0u; }
  // 3 directional lights' vectors (normalized into c5-c7)
  static constexpr uint32_t light_directions_address() { return 0x8296B950u; }
  // 12 floats of the projectors
  static constexpr uint32_t projector_rows_a_address() { return 0x8296B980u; }
  // 12 floats of the projectors
  static constexpr uint32_t projector_rows_b_address() { return 0x8296B9B0u; }
  // the 8 headlight slots' eyes (Scenery::RecordHeadlightProjectors)
  static constexpr uint32_t headlight_eyes_address() { return 0x8296B9E0u; }
  // the 8 slots' beam shapes
  static constexpr uint32_t headlight_shapes_address() { return 0x8296BA60u; }
  // the 8 slots' first colors
  static constexpr uint32_t headlight_colors_a_address() { return 0x8296BAE0u; }
  // the 8 slots' second colors
  static constexpr uint32_t headlight_colors_b_address() { return 0x8296BB60u; }
  // the 8 slots' directions (normalized)
  static constexpr uint32_t headlight_directions_address() { return 0x8296BBE0u; }
  // the 8 slots' scene +5424 vectors
  static constexpr uint32_t headlight_extras_address() { return 0x8296BC60u; }
};

}  // namespace schema
