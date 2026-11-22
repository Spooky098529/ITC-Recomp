// The views of docs/functions/morph-jobs.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/morph-jobs.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: MorphJob, SkinJob
struct MorphJob : View {
  using View::View;
  static constexpr uint32_t kSize = 224;
  static constexpr uint32_t kVtable = 0x8200286C;
  // 0x8200286C (0x8200285C the base's, stored by the destructors)
  template <typename T = MorphJobVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // set by command 22 (slot 3) after the wait; slot 2 reads it
  uint8_t released() const { return guest::LoadU8(base, address + 4); }
  void set_released(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t released_address() const { return address + 4; }
  // the same word read 4 bytes wide (the skinning record's neighbor; not read)
  uint32_t released_word() const { return guest::LoadU32(base, address + 4); }
  void set_released_word(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t released_word_address() const { return address + 4; }
  // the skinning record's +0: a word a vertex, its bone count
  template <typename T = SkinIndexList>
  T skin_influence_counts() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_skin_influence_counts(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_skin_influence_counts(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t skin_influence_counts_address() const { return address + 8; }
  // the skinning record's +4: a 16-byte weight vector an influence
  uint32_t skin_weights() const { return guest::LoadU32(base, address + 12); }
  void set_skin_weights(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t skin_weights_address() const { return address + 12; }
  // the skinning record's +8: a bone index word an influence
  uint32_t skin_bone_indices() const { return guest::LoadU32(base, address + 16); }
  void set_skin_bone_indices(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t skin_bone_indices_address() const { return address + 16; }
  // the skinning record's +12: the source positions (16 bytes a vertex)
  template <typename T = SkinVertexStream>
  T skin_source() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_skin_source(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_skin_source(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t skin_source_address() const { return address + 20; }
  // the skinning record's +16: the skinned positions
  template <typename T = SkinVertexStream>
  T skin_destination() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_skin_destination(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_skin_destination(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t skin_destination_address() const { return address + 24; }
  // the skinning record's +20: a second source stream (kinds 7 and 8, probably the normals)
  uint32_t skin_source_2() const { return guest::LoadU32(base, address + 28); }
  void set_skin_source_2(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t skin_source_2_address() const { return address + 28; }
  // the skinning record's +24: a second destination stream
  uint32_t skin_destination_2() const { return guest::LoadU32(base, address + 32); }
  void set_skin_destination_2(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t skin_destination_2_address() const { return address + 32; }
  // the skinning record's +28: the skeleton's matrices (64 bytes a bone)
  uint32_t skin_matrices() const { return guest::LoadU32(base, address + 36); }
  void set_skin_matrices(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t skin_matrices_address() const { return address + 36; }
  // the skinning record's +32
  uint32_t skin_unknown_32() const { return guest::LoadU32(base, address + 40); }
  void set_skin_unknown_32(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t skin_unknown_32_address() const { return address + 40; }
  // the skinning record's +36
  uint32_t skin_vertex_count() const { return guest::LoadU32(base, address + 44); }
  void set_skin_vertex_count(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t skin_vertex_count_address() const { return address + 44; }
  // the skinning record's +40: the stream's kind (5, 9 or 10 pick the blend)
  uint32_t skin_stream_kind() const { return guest::LoadU32(base, address + 48); }
  void set_skin_stream_kind(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t skin_stream_kind_address() const { return address + 48; }
  // the skinning record's +44: with kind 5, 6, 7 or 8 picks the blend
  uint32_t skin_stream_format() const { return guest::LoadU32(base, address + 52); }
  void set_skin_stream_format(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t skin_stream_format_address() const { return address + 52; }
  // the morph record copied from Model::MorphShape's frame (Morph::Blend's r3, second-set.md)
  uint32_t morph_record_address() const { return address + 56; }
  // the work item's +0: 0x820066DC (0x82000EB8 its base's)
  uint32_t item_vtable() const { return guest::LoadU32(base, address + 184); }
  void set_item_vtable(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t item_vtable_address() const { return address + 184; }
  // the work item's +4: 0x82002908 (0x82000AB4 written first, 0x82000BA8 by the destructor)
  uint32_t item_interface() const { return guest::LoadU32(base, address + 188); }
  void set_item_interface(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t item_interface_address() const { return address + 188; }
  // the work item's +8: a worker's value, set when it starts
  uint32_t item_worker_value() const { return guest::LoadU32(base, address + 192); }
  void set_item_worker_value(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t item_worker_value_address() const { return address + 192; }
  // the work item's +12: 1 queued, 2 running, 3 done
  uint32_t item_state() const { return guest::LoadU32(base, address + 196); }
  void set_item_state(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t item_state_address() const { return address + 196; }
  // the work item's +16
  uint8_t item_skip() const { return guest::LoadU8(base, address + 200); }
  void set_item_skip(uint8_t v) const { guest::StoreU8(base, address + 200, v); }
  uint32_t item_skip_address() const { return address + 200; }
  // the work item's +17: the post's r5 (1 here)
  uint8_t item_post_flag() const { return guest::LoadU8(base, address + 201); }
  void set_item_post_flag(uint8_t v) const { guest::StoreU8(base, address + 201, v); }
  uint32_t item_post_flag_address() const { return address + 201; }
  // the work item's +18
  uint8_t item_posted() const { return guest::LoadU8(base, address + 202); }
  void set_item_posted(uint8_t v) const { guest::StoreU8(base, address + 202, v); }
  uint32_t item_posted_address() const { return address + 202; }
  // the work item's +20: the pool's waiter interface; the constructor copies an unset frame word
  uint32_t item_waiter() const { return guest::LoadU32(base, address + 204); }
  void set_item_waiter(uint32_t v) const { guest::StoreU32(base, address + 204, v); }
  uint32_t item_waiter_address() const { return address + 204; }
  // the work item's +24: 0x82002850 (0x82000ABC its base's, by the destructor)
  uint32_t delegate_vtable() const { return guest::LoadU32(base, address + 208); }
  void set_delegate_vtable(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t delegate_vtable_address() const { return address + 208; }
  // the work item's +28: the job
  template <typename T = MorphJob>
  T delegate_object() const { return T(base, guest::LoadU32(base, address + 212)); }
  void set_delegate_object(const View& v) const { guest::StoreU32(base, address + 212, v.address); }
  void set_delegate_object(uint32_t v) const { guest::StoreU32(base, address + 212, v); }
  uint32_t delegate_object_address() const { return address + 212; }
  // the work item's +32: MorphJob::RunThunk sub_820AC9C0
  uint32_t delegate_target() const { return guest::LoadU32(base, address + 216); }
  void set_delegate_target(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t delegate_target_address() const { return address + 216; }
  // 1 a morph (Morph::Blend on +56), 0 a skin (sub_8258BCD8 on +8)
  uint32_t kind() const { return guest::LoadU32(base, address + 220); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 220, v); }
  uint32_t kind_address() const { return address + 220; }
};

// image; measure: MorphJob.+0
struct MorphJobVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // slot 0: MorphJob::~MorphJob sub_820ACB90 (deleting)
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: MorphJob::Wait sub_820AC9B8
  uint32_t wait() const { return guest::LoadU32(base, address + 4); }
  void set_wait(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t wait_address() const { return address + 4; }
  // slot 2: sub_820AC9D0
  uint32_t is_released() const { return guest::LoadU32(base, address + 8); }
  void set_is_released(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t is_released_address() const { return address + 8; }
  // slot 3: sub_820AC9D8
  uint32_t mark_released() const { return guest::LoadU32(base, address + 12); }
  void set_mark_released(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t mark_released_address() const { return address + 12; }
  // slot 4: MorphJob::Run sub_820AC998
  uint32_t run() const { return guest::LoadU32(base, address + 16); }
  void set_run(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t run_address() const { return address + 16; }
};

// embedded; measure: WorkItem, Job, Job::Wait.r3
struct WorkItem : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // 0x820066DC (0x82000EB8 its base's, by the destructor)
  template <typename T = WorkItemVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x82002908 (0x82000AB4 written first, 0x82000BA8 by the destructor)
  uint32_t interface() const { return guest::LoadU32(base, address + 4); }
  void set_interface(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t interface_address() const { return address + 4; }
  // a worker's value (its frame + 80, or 0), set by Started
  uint32_t worker_value() const { return guest::LoadU32(base, address + 8); }
  void set_worker_value(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t worker_value_address() const { return address + 8; }
  // 1 queued, 2 running, 3 done
  uint32_t state() const { return guest::LoadU32(base, address + 12); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t state_address() const { return address + 12; }
  // the run isn't made when set
  uint8_t skip() const { return guest::LoadU8(base, address + 16); }
  void set_skip(uint8_t v) const { guest::StoreU8(base, address + 16, v); }
  uint32_t skip_address() const { return address + 16; }
  // the post's r5
  uint8_t post_flag() const { return guest::LoadU8(base, address + 17); }
  void set_post_flag(uint8_t v) const { guest::StoreU8(base, address + 17, v); }
  uint32_t post_flag_address() const { return address + 17; }
  // cleared by Job::Wait
  uint8_t posted() const { return guest::LoadU8(base, address + 18); }
  void set_posted(uint8_t v) const { guest::StoreU8(base, address + 18, v); }
  uint32_t posted_address() const { return address + 18; }
  // the pool's waiter interface (pool - 4); Job::Wait calls its slot 1
  template <typename T = FramePassesJobWaiter>
  T waiter() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_waiter(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_waiter(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t waiter_address() const { return address + 20; }
  // a 12-byte delegate: 0x82002850 (0x82000ABC its base's, by the destructor)
  uint32_t delegate_vtable() const { return guest::LoadU32(base, address + 24); }
  void set_delegate_vtable(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t delegate_vtable_address() const { return address + 24; }
  uint32_t delegate_object() const { return guest::LoadU32(base, address + 28); }
  void set_delegate_object(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t delegate_object_address() const { return address + 28; }
  uint32_t delegate_target() const { return guest::LoadU32(base, address + 32); }
  void set_delegate_target(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t delegate_target_address() const { return address + 32; }
};

// image; measure: WorkItem.+0
struct WorkItemVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // slot 0: WorkItem::~WorkItem sub_82090E40 (deleting)
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: sub_82090DD0
  uint32_t posted() const { return guest::LoadU32(base, address + 4); }
  void set_posted(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t posted_address() const { return address + 4; }
  // slot 2: sub_82090DE8
  uint32_t started() const { return guest::LoadU32(base, address + 8); }
  void set_started(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t started_address() const { return address + 8; }
  // slot 3: sub_82090DF8
  uint32_t finished() const { return guest::LoadU32(base, address + 12); }
  void set_finished(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t finished_address() const { return address + 12; }
  // slot 4: sub_82090E08
  uint32_t skip() const { return guest::LoadU32(base, address + 16); }
  void set_skip(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t skip_address() const { return address + 16; }
  // slot 5: sub_82090E18
  uint32_t run() const { return guest::LoadU32(base, address + 20); }
  void set_run(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t run_address() const { return address + 20; }
  // slot 6: sub_82090CB8
  uint32_t is_skipped() const { return guest::LoadU32(base, address + 24); }
  void set_is_skipped(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t is_skipped_address() const { return address + 24; }
  // slot 7: WorkItem::Call sub_820ACA30
  uint32_t call() const { return guest::LoadU32(base, address + 28); }
  void set_call(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t call_address() const { return address + 28; }
};

// embedded; measure: sub_8258AC58.r3, sub_8258ADA0.r3, sub_8258B1C0.r3, sub_8258B368.r3
struct SkinRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // a word a vertex, its bone count
  template <typename T = SkinIndexList>
  T influence_counts() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_influence_counts(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_influence_counts(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t influence_counts_address() const { return address + 0; }
  // a 16-byte weight vector an influence
  uint32_t weights() const { return guest::LoadU32(base, address + 4); }
  void set_weights(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t weights_address() const { return address + 4; }
  // a bone index word an influence
  template <typename T = SkinIndexList>
  T bone_indices() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_bone_indices(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_bone_indices(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t bone_indices_address() const { return address + 8; }
  // the source positions (16 bytes a vertex)
  template <typename T = SkinVertexStream>
  T source() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_source(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_source(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t source_address() const { return address + 12; }
  // the skinned positions
  template <typename T = SkinVertexStream>
  T destination() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_destination(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_destination(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t destination_address() const { return address + 16; }
  // a second source stream (kinds 7 and 8, probably the normals)
  uint32_t source_2() const { return guest::LoadU32(base, address + 20); }
  void set_source_2(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t source_2_address() const { return address + 20; }
  // a second destination stream
  uint32_t destination_2() const { return guest::LoadU32(base, address + 24); }
  void set_destination_2(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t destination_2_address() const { return address + 24; }
  // the skeleton's matrices (64 bytes a bone)
  uint32_t matrices() const { return guest::LoadU32(base, address + 28); }
  void set_matrices(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t matrices_address() const { return address + 28; }
  uint32_t unknown_32() const { return guest::LoadU32(base, address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t vertex_count() const { return guest::LoadU32(base, address + 36); }
  void set_vertex_count(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t vertex_count_address() const { return address + 36; }
  // 5, 9 or 10 pick the blend
  uint32_t stream_kind() const { return guest::LoadU32(base, address + 40); }
  void set_stream_kind(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t stream_kind_address() const { return address + 40; }
  // with kind 5: 6, 7 or 8 pick the blend
  uint32_t stream_format() const { return guest::LoadU32(base, address + 44); }
  void set_stream_format(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t stream_format_address() const { return address + 44; }
};

// embedded; measure: sub_8258AC58.r3.+0, sub_8258ADA0.r3.+0, sub_8258B1C0.r3.+0, sub_8258B368.r3.+0, sub_8258AC58.r3.+8, sub_8258ADA0.r3.+8, sub_8258B1C0.r3.+8, sub_8258B368.r3.+8, MorphJob@+8.+8
struct SkinIndexList : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t entry_0() const { return guest::LoadU32(base, address + 0); }
  void set_entry_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t entry_0_address() const { return address + 0; }
  uint32_t entry_1() const { return guest::LoadU32(base, address + 4); }
  void set_entry_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t entry_1_address() const { return address + 4; }
};

// embedded; measure: sub_8258B368.r3.+12, sub_8258B368.r3.+16, MorphJob@+8.+20, MorphJob@+8.+24
struct SkinVertexStream : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // not read in detail
  float float_28() const { return guest::LoadFloat(base, address + 28); }
  void set_float_28(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t float_28_address() const { return address + 28; }
  // not read in detail
  float float_44() const { return guest::LoadFloat(base, address + 44); }
  void set_float_44(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t float_44_address() const { return address + 44; }
};

// embedded; measure: MorphJob@+8
struct MorphJobAt8 : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_36() const { return guest::LoadU32(base, address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(base, address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_44() const { return guest::LoadU32(base, address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
};

// embedded; measure: MorphJob@+8.+0
struct MorphJobVtablePart : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: sub_82090DC8.r3@-4
struct WorkItemOuter4 : View {
  using View::View;
  static constexpr uint32_t kSize = 17;
  uint8_t unknown_16() const { return guest::LoadU8(base, address + 16); }
  void set_unknown_16(uint8_t v) const { guest::StoreU8(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// embedded; measure: sub_82090E38.r3@-4
struct WorkItemOuter4_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
};

}  // namespace schema
