// The views of docs/functions/device-shaders.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/device-shaders.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// gpu; measure: RenderQueue, sub_8258E5C8.r3, Font.+8, [82970BD4]
struct RenderQueue : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr uint32_t kVtable = 0x8206CB8C;
  // 0x8206CB8C
  template <typename T = RenderQueueVtable>
  T vtable() const { return T(guest::LoadU32(address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // operator new(88)
  template <typename T = RenderQueueState>
  T state() const { return T(guest::LoadU32(address + 4)); }
  void set_state(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t state_address() const { return address + 4; }
  // the list recorded when the replay index is 1, replayed when it's 0
  template <typename T = PassList>
  T list_1() const { return T(guest::LoadU32(address + 8)); }
  void set_list_1(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_list_1(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t list_1_address() const { return address + 8; }
  // the list recorded when the replay index is 0, replayed when it's 1
  template <typename T = PassList>
  T list_0() const { return T(guest::LoadU32(address + 12)); }
  void set_list_0(const View& v) const { guest::StoreU32(address + 12, v.address); }
  void set_list_0(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t list_0_address() const { return address + 12; }
};

// gpu; measure: RenderQueue.+0, Font.+8.+0, [82970BD4].+0
struct RenderQueueVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // RenderQueue::Init sub_8258ED00
  uint32_t init() const { return guest::LoadU32(address + 0); }
  void set_init(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t init_address() const { return address + 0; }
  // sub_8258E5C8, the drawer's slot 1 (not seen in play)
  uint32_t slot_1() const { return guest::LoadU32(address + 4); }
  void set_slot_1(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t slot_1_address() const { return address + 4; }
  // RenderQueue::ClearRecording sub_8258ED78
  uint32_t clear_recording() const { return guest::LoadU32(address + 8); }
  void set_clear_recording(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t clear_recording_address() const { return address + 8; }
  // the empty destructor sub_82106540
  uint32_t destructor() const { return guest::LoadU32(address + 12); }
  void set_destructor(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t destructor_address() const { return address + 12; }
  // RenderQueue::Add sub_8258EDC8
  uint32_t add() const { return guest::LoadU32(address + 16); }
  void set_add(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t add_address() const { return address + 16; }
  // RenderQueue::Replay sub_82591238
  uint32_t replay() const { return guest::LoadU32(address + 20); }
  void set_replay(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t replay_address() const { return address + 20; }
  // RenderQueue::Flip sub_8258E5E0
  uint32_t flip() const { return guest::LoadU32(address + 24); }
  void set_flip(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t flip_address() const { return address + 24; }
  // RenderQueue::Recording sub_8258E5F8
  uint32_t recording() const { return guest::LoadU32(address + 28); }
  void set_recording(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t recording_address() const { return address + 28; }
  // RenderQueue::Replaying sub_8258E618
  uint32_t replaying() const { return guest::LoadU32(address + 32); }
  void set_replaying(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t replaying_address() const { return address + 32; }
  // sub_82590EA0 (not seen in play)
  uint32_t deleting_destructor() const { return guest::LoadU32(address + 36); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t deleting_destructor_address() const { return address + 36; }
};

// allocated; measure: new<88>@RenderQueue::RenderQueue, sub_8258E5C8.r3.+4
struct RenderQueueState : View {
  using View::View;
  static constexpr uint32_t kSize = 88;
  // 0: +12 recorded, +8 replayed; 1 the other way
  uint32_t replay_index() const { return guest::LoadU32(address + 0); }
  void set_replay_index(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t replay_index_address() const { return address + 0; }
  // the primitive drawer's vtable word (the drawer itself is here, model-animation.md)
  uint32_t drawer() const { return guest::LoadU32(address + 4); }
  void set_drawer(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t drawer_address() const { return address + 4; }
  // the drawer's state, published at 0x82970BBC (single-commands.md)
  uint32_t drawer_state_address() const { return address + 12; }
  // zeroed by the replay
  uint32_t cleared_24() const { return guest::LoadU32(address + 24); }
  void set_cleared_24(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t cleared_24_address() const { return address + 24; }
  // zeroed by the replay
  uint32_t cleared_28() const { return guest::LoadU32(address + 28); }
  void set_cleared_28(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t cleared_28_address() const { return address + 28; }
  // zeroed by the replay
  uint32_t cleared_32() const { return guest::LoadU32(address + 32); }
  void set_cleared_32(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t cleared_32_address() const { return address + 32; }
  // zeroed by the replay
  uint32_t cleared_36() const { return guest::LoadU32(address + 36); }
  void set_cleared_36(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t cleared_36_address() const { return address + 36; }
  // the video output the replay was given
  uint32_t video_output() const { return guest::LoadU32(address + 44); }
  void set_video_output(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t video_output_address() const { return address + 44; }
  // Device::GetRenderTarget(device, 0) (RenderQueue::Init); published at 0x82970BC0
  uint32_t render_target_0() const { return guest::LoadU32(address + 48); }
  void set_render_target_0(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t render_target_0_address() const { return address + 48; }
  uint32_t render_target_1() const { return guest::LoadU32(address + 52); }
  void set_render_target_1(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t render_target_1_address() const { return address + 52; }
  uint32_t render_target_2() const { return guest::LoadU32(address + 56); }
  void set_render_target_2(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t render_target_2_address() const { return address + 56; }
  uint32_t render_target_3() const { return guest::LoadU32(address + 60); }
  void set_render_target_3(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t render_target_3_address() const { return address + 60; }
  // GetDepthStencilSurface(device)
  uint32_t depth_surface() const { return guest::LoadU32(address + 64); }
  void set_depth_surface(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t depth_surface_address() const { return address + 64; }
  // a PtrVector's allocator word: the release list (published at 0x8296BD8C)
  uint32_t release_list() const { return guest::LoadU32(address + 68); }
  void set_release_list(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t release_list_address() const { return address + 68; }
  uint32_t release_first() const { return guest::LoadU32(address + 72); }
  void set_release_first(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t release_first_address() const { return address + 72; }
  uint32_t release_last() const { return guest::LoadU32(address + 76); }
  void set_release_last(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t release_last_address() const { return address + 76; }
  uint32_t release_end() const { return guest::LoadU32(address + 80); }
  void set_release_end(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t release_end_address() const { return address + 80; }
  // the submission sequence, += 0.1 an Add
  float sequence() const { return guest::LoadFloat(address + 84); }
  void set_sequence(float v) const { guest::StoreFloat(address + 84, v); }
  uint32_t sequence_address() const { return address + 84; }
};

// allocated; measure: PassList, RenderQueue.+8, RenderQueue.+12
struct PassList : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  static constexpr uint32_t kVtable = 0x8206CB88;
  // 0x8206CB88
  template <typename T = PassListVtable>
  T vtable() const { return T(guest::LoadU32(address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the replay's "bucket done" flag
  uint8_t bucket_done() const { return guest::LoadU8(address + 4); }
  void set_bucket_done(uint8_t v) const { guest::StoreU8(address + 4, v); }
  uint32_t bucket_done_address() const { return address + 4; }
  // 4,096
  uint32_t item_count() const { return guest::LoadU32(address + 8); }
  void set_item_count(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t item_count_address() const { return address + 8; }
  // 1,024
  uint32_t bucket_count() const { return guest::LoadU32(address + 12); }
  void set_bucket_count(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t bucket_count_address() const { return address + 12; }
  uint32_t items_used() const { return guest::LoadU32(address + 16); }
  void set_items_used(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t items_used_address() const { return address + 16; }
  // item_count items of 20 bytes
  template <typename T = PassListItem>
  T items() const { return T(guest::LoadU32(address + 20)); }
  void set_items(const View& v) const { guest::StoreU32(address + 20, v.address); }
  void set_items(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t items_address() const { return address + 20; }
  // bucket_count buckets of 16 bytes
  template <typename T = PassListBucket>
  T buckets() const { return T(guest::LoadU32(address + 24)); }
  void set_buckets(const View& v) const { guest::StoreU32(address + 24, v.address); }
  void set_buckets(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t buckets_address() const { return address + 24; }
  // zeroed by Clear and the replay
  uint32_t cleared_28() const { return guest::LoadU32(address + 28); }
  void set_cleared_28(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t cleared_28_address() const { return address + 28; }
  // the replay's current item
  template <typename T = PassListItem>
  T current_item() const { return T(guest::LoadU32(address + 32)); }
  void set_current_item(const View& v) const { guest::StoreU32(address + 32, v.address); }
  void set_current_item(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t current_item_address() const { return address + 32; }
};

// gpu; measure: RenderQueue.+8.+0, RenderQueue.+12.+0
struct PassListVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // PassList::~PassList
  uint32_t destructor() const { return guest::LoadU32(address + 0); }
  void set_destructor(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t destructor_address() const { return address + 0; }
};

// allocated; measure: new<4294967295>@PassList::PassList
struct PassListItem : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  template <typename T = PassListItem>
  T next() const { return T(guest::LoadU32(address + 0)); }
  void set_next(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  template <typename T = ChunkItem>
  T chunk() const { return T(guest::LoadU32(address + 4)); }
  void set_chunk(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_chunk(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t chunk_address() const { return address + 4; }
  float key() const { return guest::LoadFloat(address + 8); }
  void set_key(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t key_address() const { return address + 8; }
  float sequence() const { return guest::LoadFloat(address + 12); }
  void set_sequence(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t sequence_address() const { return address + 12; }
  // never written here
  uint32_t unknown_16() const { return guest::LoadU32(address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// allocated; measure: new<4294967295>@PassList::PassList#2
struct PassListBucket : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  template <typename T = PassListItem>
  T first() const { return T(guest::LoadU32(address + 0)); }
  void set_first(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t first_address() const { return address + 0; }
  template <typename T = PassListItem>
  T last() const { return T(guest::LoadU32(address + 4)); }
  void set_last(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_last(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t last_address() const { return address + 4; }
  // the next bucket, 0 for the last (PassList::PassList)
  template <typename T = PassListBucket>
  T next() const { return T(guest::LoadU32(address + 8)); }
  void set_next(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t next_address() const { return address + 8; }
  // items added; Clear never resets it, nothing here reads it
  uint32_t count() const { return guest::LoadU32(address + 12); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t count_address() const { return address + 12; }
};

// gpu; measure: QueueEntry, [8296BD98]
struct RenderQueueEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = ChunkItem>
  T chunk() const { return T(guest::LoadU32(address + 0)); }
  void set_chunk(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_chunk(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t chunk_address() const { return address + 0; }
  float key() const { return guest::LoadFloat(address + 4); }
  void set_key(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t key_address() const { return address + 4; }
  // QueueEntry::Sequence sub_8258E5C0
  float sequence() const { return guest::LoadFloat(address + 8); }
  void set_sequence(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t sequence_address() const { return address + 8; }
};

// gpu; measure: ChunkChain::RunInline.r4, ChunkChain::RunInlineLast.r4, ChunkChain::RunInlineLinked.r4, ChunkChain::RunOutOfLine.r4, ChunkChain::RunOutOfLineLast.r4, ChunkChain::RunSingles.r4, [8296BD98].+0
struct ChunkItem : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // the low halfword, sign-extended: 0 singles, 1 inline, 2 inline linked, 3 out of line, 4 out of line last, 5 inline last, 6 the end
  uint32_t kind() const { return guest::LoadU32(address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  // kinds 3 and 4: the out-of-line commands (ChunkCommand); kind 2: the next item
  uint32_t link() const { return guest::LoadU32(address + 4); }
  void set_link(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t link_address() const { return address + 4; }
  // kind 0: the single commands' count
  uint32_t single_count() const { return guest::LoadU32(address + 8); }
  void set_single_count(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t single_count_address() const { return address + 8; }
  // the first inline command (kinds 1, 2, 5): its id, 36 the end
  uint32_t command_id() const { return guest::LoadU32(address + 16); }
  void set_command_id(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t command_id_address() const { return address + 16; }
  // its r4
  uint32_t command_arg0() const { return guest::LoadU32(address + 20); }
  void set_command_arg0(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t command_arg0_address() const { return address + 20; }
  // its r5
  uint32_t command_arg1() const { return guest::LoadU32(address + 24); }
  void set_command_arg1(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t command_arg1_address() const { return address + 24; }
};

// gpu; measure: ChunkChain::RunOutOfLine.r4.+4, ChunkChain::RunOutOfLineLast.r4.+4
struct ChunkCommand : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t id() const { return guest::LoadU32(address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t id_address() const { return address + 0; }
  uint32_t arg0() const { return guest::LoadU32(address + 4); }
  void set_arg0(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t arg0_address() const { return address + 4; }
  uint32_t arg1() const { return guest::LoadU32(address + 8); }
  void set_arg1(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t arg1_address() const { return address + 8; }
};

// gpu; measure: PtrVector
struct PtrVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t allocator() const { return guest::LoadU32(address + 0); }
  void set_allocator(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t allocator_address() const { return address + 0; }
  template <typename T = PtrVectorSlot>
  T first() const { return T(guest::LoadU32(address + 4)); }
  void set_first(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t first_address() const { return address + 4; }
  template <typename T = PtrVectorSlot>
  T last() const { return T(guest::LoadU32(address + 8)); }
  void set_last(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_last(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t last_address() const { return address + 8; }
  template <typename T = PtrVectorSlot>
  T end() const { return T(guest::LoadU32(address + 12)); }
  void set_end(const View& v) const { guest::StoreU32(address + 12, v.address); }
  void set_end(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t end_address() const { return address + 12; }
};

// gpu; measure: PtrVector.+8, PtrVector::PushBack.r4
struct PtrVectorSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// The image words docs/functions/device-shaders.md's code uses.
struct DeviceShadersGlobals {
  // the replay's release list (state + 68), emptied at its start
  template <typename T = PtrVector>
  static T release_list() { return T(guest::LoadU32(0x8296BD8Cu)); }
  static void set_release_list(const View& v) { guest::StoreU32(0x8296BD8Cu, v.address); }
  static void set_release_list(uint32_t v) { guest::StoreU32(0x8296BD8Cu, v); }
  static constexpr uint32_t release_list_address() { return 0x8296BD8Cu; }
  // the device during the replay, 0 after
  static uint32_t replay_device() { return guest::LoadU32(0x8296BD90u); }
  static void set_replay_device(uint32_t v) { guest::StoreU32(0x8296BD90u, v); }
  static constexpr uint32_t replay_device_address() { return 0x8296BD90u; }
  // the replay's sort array: 5,000 RenderQueueEntry pointers
  static constexpr uint32_t replay_entries_address() { return 0x8296BD98u; }
  // the queue state's +4, the primitive drawer
  static uint32_t replay_drawer() { return guest::LoadU32(0x82970BB8u); }
  static void set_replay_drawer(uint32_t v) { guest::StoreU32(0x82970BB8u, v); }
  static constexpr uint32_t replay_drawer_address() { return 0x82970BB8u; }
  // the queue state's +12
  static uint32_t replay_drawer_state() { return guest::LoadU32(0x82970BBCu); }
  static void set_replay_drawer_state(uint32_t v) { guest::StoreU32(0x82970BBCu, v); }
  static constexpr uint32_t replay_drawer_state_address() { return 0x82970BBCu; }
  // the queue state's +48
  static uint32_t replay_render_targets() { return guest::LoadU32(0x82970BC0u); }
  static void set_replay_render_targets(uint32_t v) { guest::StoreU32(0x82970BC0u, v); }
  static constexpr uint32_t replay_render_targets_address() { return 0x82970BC0u; }
  // the single commands' handlers by id
  static constexpr uint32_t single_commands_address() { return 0x8265ED48u; }
  // the render commands' handlers by id (36 the end)
  static constexpr uint32_t commands_address() { return 0x8265EDE0u; }
  // 0.0
  static float zero() { return guest::LoadFloat(0x82000A34u); }
  static void set_zero(float v) { guest::StoreFloat(0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 10,000.0
  static float key_max() { return guest::LoadFloat(0x82002814u); }
  static void set_key_max(float v) { guest::StoreFloat(0x82002814u, v); }
  static constexpr uint32_t key_max_address() { return 0x82002814u; }
  // -10,000.0
  static float key_min() { return guest::LoadFloat(0x82001C00u); }
  static void set_key_min(float v) { guest::StoreFloat(0x82001C00u, v); }
  static constexpr uint32_t key_min_address() { return 0x82001C00u; }
  // 0.1
  static float sequence_step() { return guest::LoadFloat(0x82001FBCu); }
  static void set_sequence_step(float v) { guest::StoreFloat(0x82001FBCu, v); }
  static constexpr uint32_t sequence_step_address() { return 0x82001FBCu; }
};

}  // namespace schema
