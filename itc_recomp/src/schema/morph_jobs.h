// The views of docs/functions/morph-jobs.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/morph-jobs.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: MorphJob, SkinJob
#pragma pack(push, 1)
struct MorphJobData {
  uint32_t vtable = 0;  // +0
  uint8_t released = 0;  // +4
  uint8_t pad_5[3] = {};
  uint32_t skin_influence_counts = 0;  // +8
  uint32_t skin_weights = 0;  // +12
  uint32_t skin_bone_indices = 0;  // +16
  uint32_t skin_source = 0;  // +20
  uint32_t skin_destination = 0;  // +24
  uint32_t skin_source_2 = 0;  // +28
  uint32_t skin_destination_2 = 0;  // +32
  uint32_t skin_matrices = 0;  // +36
  uint32_t skin_unknown_32 = 0;  // +40
  uint32_t skin_vertex_count = 0;  // +44
  uint32_t skin_stream_kind = 0;  // +48
  uint32_t skin_stream_format = 0;  // +52
  uint8_t morph_record[128] = {};  // +56, guest order
  uint32_t item_vtable = 0;  // +184
  uint32_t item_interface = 0;  // +188
  uint32_t item_worker_value = 0;  // +192
  uint32_t item_state = 0;  // +196
  uint8_t item_skip = 0;  // +200
  uint8_t item_post_flag = 0;  // +201
  uint8_t item_posted = 0;  // +202
  uint8_t pad_203[1] = {};
  uint32_t item_waiter = 0;  // +204
  uint32_t delegate_vtable = 0;  // +208
  uint32_t delegate_object = 0;  // +212
  uint32_t delegate_target = 0;  // +216
  uint32_t kind = 0;  // +220
};
#pragma pack(pop)
static_assert(sizeof(MorphJobData) == 224);
inline constexpr memory::LayoutField kMorphJobFields[] = {{0, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {184, 4}, {188, 4}, {192, 4}, {196, 4}, {204, 4}, {208, 4}, {212, 4}, {216, 4}, {220, 4}};
inline constexpr memory::Layout kMorphJobLayout = {kMorphJobFields, 22};

struct MorphJob : View {
  using View::View;
  static constexpr uint32_t kSize = 224;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x8200286C;
  MorphJobData* data() const { return static_cast<MorphJobData*>(memory::HandleObject(address)); }
  static MorphJob New(uint8_t* base) {
    return MorphJob(base, memory::MakeHandle(new MorphJobData(), sizeof(MorphJobData), "MorphJob", &kMorphJobLayout));
  }
  template <uint32_t Stride = kSize>
  static MorphJob NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(MorphJobData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<MorphJobData*>(::operator new(sizeof(MorphJobData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) MorphJobData();
    return MorphJob(base, memory::MakeHandle(p, sizeof(MorphJobData) * count, "MorphJob", &kMorphJobLayout, sizeof(MorphJobData)));
  }
  MorphJob at(uint32_t i) const { return MorphJob(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kMorphJobLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kMorphJobLayout, offset, width, v); }
  // 0x8200286C (0x8200285C the base's, stored by the destructors)
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = MorphJobVtable>
  T vtable() const { return T(base, data()->vtable); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; data()->vtable = v; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // set by command 22 (slot 3) after the wait; slot 2 reads it
  uint32_t released_address() const { return address + 4; }
  uint8_t released() const { return data()->released; }
  void set_released(uint8_t v) const { data()->released = v; }
  uint8_t released_bits() const { return data()->released; }
  void set_released_bits(uint8_t v) const { data()->released = v; }
  // the same word read 4 bytes wide (the skinning record's neighbor; not read)
  uint32_t released_word_address() const { return address + 4; }
  uint32_t released_word_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kMorphJobLayout, 4, 4)); }
  void set_released_word_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kMorphJobLayout, 4, 4, v); }
  uint32_t released_word() const { return uint32_t(released_word_bits()); }
  void set_released_word(uint32_t v) const { set_released_word_bits(uint32_t(v)); }
  // the skinning record's +0: a word a vertex, its bone count
  uint32_t skin_influence_counts_address() const { return address + 8; }
  template <typename T = SkinIndexList>
  T skin_influence_counts() const { return T(base, data()->skin_influence_counts); }
  void set_skin_influence_counts(const View& w) const { const uint32_t v = w.address; data()->skin_influence_counts = v; }
  void set_skin_influence_counts(uint32_t v) const { data()->skin_influence_counts = v; }
  uint32_t skin_influence_counts_bits() const { return data()->skin_influence_counts; }
  void set_skin_influence_counts_bits(uint32_t v) const { data()->skin_influence_counts = v; }
  // the skinning record's +4: a 16-byte weight vector an influence
  uint32_t skin_weights_address() const { return address + 12; }
  uint32_t skin_weights() const { return data()->skin_weights; }
  void set_skin_weights(uint32_t v) const { data()->skin_weights = v; }
  uint32_t skin_weights_bits() const { return data()->skin_weights; }
  void set_skin_weights_bits(uint32_t v) const { data()->skin_weights = v; }
  // the skinning record's +8: a bone index word an influence
  uint32_t skin_bone_indices_address() const { return address + 16; }
  uint32_t skin_bone_indices() const { return data()->skin_bone_indices; }
  void set_skin_bone_indices(uint32_t v) const { data()->skin_bone_indices = v; }
  uint32_t skin_bone_indices_bits() const { return data()->skin_bone_indices; }
  void set_skin_bone_indices_bits(uint32_t v) const { data()->skin_bone_indices = v; }
  // the skinning record's +12: the source positions (16 bytes a vertex)
  uint32_t skin_source_address() const { return address + 20; }
  template <typename T = SkinVertexStream>
  T skin_source() const { return T(base, data()->skin_source); }
  void set_skin_source(const View& w) const { const uint32_t v = w.address; data()->skin_source = v; }
  void set_skin_source(uint32_t v) const { data()->skin_source = v; }
  uint32_t skin_source_bits() const { return data()->skin_source; }
  void set_skin_source_bits(uint32_t v) const { data()->skin_source = v; }
  // the skinning record's +16: the skinned positions
  uint32_t skin_destination_address() const { return address + 24; }
  template <typename T = SkinVertexStream>
  T skin_destination() const { return T(base, data()->skin_destination); }
  void set_skin_destination(const View& w) const { const uint32_t v = w.address; data()->skin_destination = v; }
  void set_skin_destination(uint32_t v) const { data()->skin_destination = v; }
  uint32_t skin_destination_bits() const { return data()->skin_destination; }
  void set_skin_destination_bits(uint32_t v) const { data()->skin_destination = v; }
  // the skinning record's +20: a second source stream (kinds 7 and 8, probably the normals)
  uint32_t skin_source_2_address() const { return address + 28; }
  uint32_t skin_source_2() const { return data()->skin_source_2; }
  void set_skin_source_2(uint32_t v) const { data()->skin_source_2 = v; }
  uint32_t skin_source_2_bits() const { return data()->skin_source_2; }
  void set_skin_source_2_bits(uint32_t v) const { data()->skin_source_2 = v; }
  // the skinning record's +24: a second destination stream
  uint32_t skin_destination_2_address() const { return address + 32; }
  uint32_t skin_destination_2() const { return data()->skin_destination_2; }
  void set_skin_destination_2(uint32_t v) const { data()->skin_destination_2 = v; }
  uint32_t skin_destination_2_bits() const { return data()->skin_destination_2; }
  void set_skin_destination_2_bits(uint32_t v) const { data()->skin_destination_2 = v; }
  // the skinning record's +28: the skeleton's matrices (64 bytes a bone)
  uint32_t skin_matrices_address() const { return address + 36; }
  uint32_t skin_matrices() const { return data()->skin_matrices; }
  void set_skin_matrices(uint32_t v) const { data()->skin_matrices = v; }
  uint32_t skin_matrices_bits() const { return data()->skin_matrices; }
  void set_skin_matrices_bits(uint32_t v) const { data()->skin_matrices = v; }
  // the skinning record's +32
  uint32_t skin_unknown_32_address() const { return address + 40; }
  uint32_t skin_unknown_32() const { return data()->skin_unknown_32; }
  void set_skin_unknown_32(uint32_t v) const { data()->skin_unknown_32 = v; }
  uint32_t skin_unknown_32_bits() const { return data()->skin_unknown_32; }
  void set_skin_unknown_32_bits(uint32_t v) const { data()->skin_unknown_32 = v; }
  // the skinning record's +36
  uint32_t skin_vertex_count_address() const { return address + 44; }
  uint32_t skin_vertex_count() const { return data()->skin_vertex_count; }
  void set_skin_vertex_count(uint32_t v) const { data()->skin_vertex_count = v; }
  uint32_t skin_vertex_count_bits() const { return data()->skin_vertex_count; }
  void set_skin_vertex_count_bits(uint32_t v) const { data()->skin_vertex_count = v; }
  // the skinning record's +40: the stream's kind (5, 9 or 10 pick the blend)
  uint32_t skin_stream_kind_address() const { return address + 48; }
  uint32_t skin_stream_kind() const { return data()->skin_stream_kind; }
  void set_skin_stream_kind(uint32_t v) const { data()->skin_stream_kind = v; }
  uint32_t skin_stream_kind_bits() const { return data()->skin_stream_kind; }
  void set_skin_stream_kind_bits(uint32_t v) const { data()->skin_stream_kind = v; }
  // the skinning record's +44: with kind 5, 6, 7 or 8 picks the blend
  uint32_t skin_stream_format_address() const { return address + 52; }
  uint32_t skin_stream_format() const { return data()->skin_stream_format; }
  void set_skin_stream_format(uint32_t v) const { data()->skin_stream_format = v; }
  uint32_t skin_stream_format_bits() const { return data()->skin_stream_format; }
  void set_skin_stream_format_bits(uint32_t v) const { data()->skin_stream_format = v; }
  // the morph record copied from Model::MorphShape's frame (Morph::Blend's r3, second-set.md)
  uint32_t morph_record_address() const { return address + 56; }
  uint8_t* morph_record() const { return reinterpret_cast<uint8_t*>(data()) + 56; }
  // the work item's +0: 0x820066DC (0x82000EB8 its base's)
  uint32_t item_vtable_address() const { return address + 184; }
  uint32_t item_vtable() const { return data()->item_vtable; }
  void set_item_vtable(uint32_t v) const { data()->item_vtable = v; }
  uint32_t item_vtable_bits() const { return data()->item_vtable; }
  void set_item_vtable_bits(uint32_t v) const { data()->item_vtable = v; }
  // the work item's +4: 0x82002908 (0x82000AB4 written first, 0x82000BA8 by the destructor)
  uint32_t item_interface_address() const { return address + 188; }
  uint32_t item_interface() const { return data()->item_interface; }
  void set_item_interface(uint32_t v) const { data()->item_interface = v; }
  uint32_t item_interface_bits() const { return data()->item_interface; }
  void set_item_interface_bits(uint32_t v) const { data()->item_interface = v; }
  // the work item's +8: a worker's value, set when it starts
  uint32_t item_worker_value_address() const { return address + 192; }
  uint32_t item_worker_value() const { return data()->item_worker_value; }
  void set_item_worker_value(uint32_t v) const { data()->item_worker_value = v; }
  uint32_t item_worker_value_bits() const { return data()->item_worker_value; }
  void set_item_worker_value_bits(uint32_t v) const { data()->item_worker_value = v; }
  // the work item's +12: 1 queued, 2 running, 3 done
  uint32_t item_state_address() const { return address + 196; }
  uint32_t item_state() const { return data()->item_state; }
  void set_item_state(uint32_t v) const { data()->item_state = v; }
  uint32_t item_state_bits() const { return data()->item_state; }
  void set_item_state_bits(uint32_t v) const { data()->item_state = v; }
  // the work item's +16
  uint32_t item_skip_address() const { return address + 200; }
  uint8_t item_skip() const { return data()->item_skip; }
  void set_item_skip(uint8_t v) const { data()->item_skip = v; }
  uint8_t item_skip_bits() const { return data()->item_skip; }
  void set_item_skip_bits(uint8_t v) const { data()->item_skip = v; }
  // the work item's +17: the post's r5 (1 here)
  uint32_t item_post_flag_address() const { return address + 201; }
  uint8_t item_post_flag() const { return data()->item_post_flag; }
  void set_item_post_flag(uint8_t v) const { data()->item_post_flag = v; }
  uint8_t item_post_flag_bits() const { return data()->item_post_flag; }
  void set_item_post_flag_bits(uint8_t v) const { data()->item_post_flag = v; }
  // the work item's +18
  uint32_t item_posted_address() const { return address + 202; }
  uint8_t item_posted() const { return data()->item_posted; }
  void set_item_posted(uint8_t v) const { data()->item_posted = v; }
  uint8_t item_posted_bits() const { return data()->item_posted; }
  void set_item_posted_bits(uint8_t v) const { data()->item_posted = v; }
  // the work item's +20: the pool's waiter interface; the constructor copies an unset frame word
  uint32_t item_waiter_address() const { return address + 204; }
  uint32_t item_waiter() const { return data()->item_waiter; }
  void set_item_waiter(uint32_t v) const { data()->item_waiter = v; }
  uint32_t item_waiter_bits() const { return data()->item_waiter; }
  void set_item_waiter_bits(uint32_t v) const { data()->item_waiter = v; }
  // the work item's +24: 0x82002850 (0x82000ABC its base's, by the destructor)
  uint32_t delegate_vtable_address() const { return address + 208; }
  uint32_t delegate_vtable() const { return data()->delegate_vtable; }
  void set_delegate_vtable(uint32_t v) const { data()->delegate_vtable = v; }
  uint32_t delegate_vtable_bits() const { return data()->delegate_vtable; }
  void set_delegate_vtable_bits(uint32_t v) const { data()->delegate_vtable = v; }
  // the work item's +28: the job
  uint32_t delegate_object_address() const { return address + 212; }
  template <typename T = MorphJob>
  T delegate_object() const { return T(base, data()->delegate_object); }
  void set_delegate_object(const View& w) const { const uint32_t v = w.address; data()->delegate_object = v; }
  void set_delegate_object(uint32_t v) const { data()->delegate_object = v; }
  uint32_t delegate_object_bits() const { return data()->delegate_object; }
  void set_delegate_object_bits(uint32_t v) const { data()->delegate_object = v; }
  // the work item's +32: MorphJob::RunThunk sub_820AC9C0
  uint32_t delegate_target_address() const { return address + 216; }
  uint32_t delegate_target() const { return data()->delegate_target; }
  void set_delegate_target(uint32_t v) const { data()->delegate_target = v; }
  uint32_t delegate_target_bits() const { return data()->delegate_target; }
  void set_delegate_target_bits(uint32_t v) const { data()->delegate_target = v; }
  // 1 a morph (Morph::Blend on +56), 0 a skin (sub_8258BCD8 on +8)
  uint32_t kind_address() const { return address + 220; }
  uint32_t kind() const { return data()->kind; }
  void set_kind(uint32_t v) const { data()->kind = v; }
  uint32_t kind_bits() const { return data()->kind; }
  void set_kind_bits(uint32_t v) const { data()->kind = v; }
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
#pragma pack(push, 1)
struct SkinRecordData {
  uint32_t influence_counts = 0;  // +0
  uint32_t weights = 0;  // +4
  uint32_t bone_indices = 0;  // +8
  uint32_t source = 0;  // +12
  uint32_t destination = 0;  // +16
  uint32_t source_2 = 0;  // +20
  uint32_t destination_2 = 0;  // +24
  uint32_t matrices = 0;  // +28
  uint32_t unknown_32 = 0;  // +32
  uint32_t vertex_count = 0;  // +36
  uint32_t stream_kind = 0;  // +40
  uint32_t stream_format = 0;  // +44
};
#pragma pack(pop)
static_assert(sizeof(SkinRecordData) == 48);
inline constexpr memory::LayoutField kSkinRecordFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}};
inline constexpr memory::Layout kSkinRecordLayout = {kSkinRecordFields, 12};

struct SkinRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  SkinRecordData* data() const { return static_cast<SkinRecordData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static SkinRecord New(uint8_t* base) {
    return SkinRecord(base, memory::MakeHandle(new SkinRecordData(), sizeof(SkinRecordData), "SkinRecord", &kSkinRecordLayout));
  }
  template <uint32_t Stride = kSize>
  static SkinRecord NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SkinRecordData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SkinRecordData*>(::operator new(sizeof(SkinRecordData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SkinRecordData();
    return SkinRecord(base, memory::MakeHandle(p, sizeof(SkinRecordData) * count, "SkinRecord", &kSkinRecordLayout, sizeof(SkinRecordData)));
  }
  SkinRecord at(uint32_t i) const { return SkinRecord(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kSkinRecordLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kSkinRecordLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // a word a vertex, its bone count
  uint32_t influence_counts_address() const { return address + 0; }
  template <typename T = SkinIndexList>
  T influence_counts() const { return T(base, on_host() ? data()->influence_counts : guest::LoadU32(base, address + 0)); }
  void set_influence_counts(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->influence_counts = v; else guest::StoreU32(base, address + 0, v); }
  void set_influence_counts(uint32_t v) const { if (on_host()) data()->influence_counts = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t influence_counts_bits() const { return on_host() ? data()->influence_counts : guest::LoadU32(base, address + 0); }
  void set_influence_counts_bits(uint32_t v) const { if (on_host()) data()->influence_counts = v; else guest::StoreU32(base, address + 0, v); }
  // a 16-byte weight vector an influence
  uint32_t weights_address() const { return address + 4; }
  uint32_t weights() const { return on_host() ? uint32_t(data()->weights) : uint32_t(guest::LoadU32(base, address + 4)); }
  void set_weights(uint32_t v) const { if (on_host()) data()->weights = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t weights_bits() const { return on_host() ? data()->weights : guest::LoadU32(base, address + 4); }
  void set_weights_bits(uint32_t v) const { if (on_host()) data()->weights = v; else guest::StoreU32(base, address + 4, v); }
  // a bone index word an influence
  uint32_t bone_indices_address() const { return address + 8; }
  template <typename T = SkinIndexList>
  T bone_indices() const { return T(base, on_host() ? data()->bone_indices : guest::LoadU32(base, address + 8)); }
  void set_bone_indices(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->bone_indices = v; else guest::StoreU32(base, address + 8, v); }
  void set_bone_indices(uint32_t v) const { if (on_host()) data()->bone_indices = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t bone_indices_bits() const { return on_host() ? data()->bone_indices : guest::LoadU32(base, address + 8); }
  void set_bone_indices_bits(uint32_t v) const { if (on_host()) data()->bone_indices = v; else guest::StoreU32(base, address + 8, v); }
  // the source positions (16 bytes a vertex)
  uint32_t source_address() const { return address + 12; }
  template <typename T = SkinVertexStream>
  T source() const { return T(base, on_host() ? data()->source : guest::LoadU32(base, address + 12)); }
  void set_source(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->source = v; else guest::StoreU32(base, address + 12, v); }
  void set_source(uint32_t v) const { if (on_host()) data()->source = v; else guest::StoreU32(base, address + 12, v); }
  uint32_t source_bits() const { return on_host() ? data()->source : guest::LoadU32(base, address + 12); }
  void set_source_bits(uint32_t v) const { if (on_host()) data()->source = v; else guest::StoreU32(base, address + 12, v); }
  // the skinned positions
  uint32_t destination_address() const { return address + 16; }
  template <typename T = SkinVertexStream>
  T destination() const { return T(base, on_host() ? data()->destination : guest::LoadU32(base, address + 16)); }
  void set_destination(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->destination = v; else guest::StoreU32(base, address + 16, v); }
  void set_destination(uint32_t v) const { if (on_host()) data()->destination = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t destination_bits() const { return on_host() ? data()->destination : guest::LoadU32(base, address + 16); }
  void set_destination_bits(uint32_t v) const { if (on_host()) data()->destination = v; else guest::StoreU32(base, address + 16, v); }
  // a second source stream (kinds 7 and 8, probably the normals)
  uint32_t source_2_address() const { return address + 20; }
  uint32_t source_2() const { return on_host() ? uint32_t(data()->source_2) : uint32_t(guest::LoadU32(base, address + 20)); }
  void set_source_2(uint32_t v) const { if (on_host()) data()->source_2 = v; else guest::StoreU32(base, address + 20, v); }
  uint32_t source_2_bits() const { return on_host() ? data()->source_2 : guest::LoadU32(base, address + 20); }
  void set_source_2_bits(uint32_t v) const { if (on_host()) data()->source_2 = v; else guest::StoreU32(base, address + 20, v); }
  // a second destination stream
  uint32_t destination_2_address() const { return address + 24; }
  uint32_t destination_2() const { return on_host() ? uint32_t(data()->destination_2) : uint32_t(guest::LoadU32(base, address + 24)); }
  void set_destination_2(uint32_t v) const { if (on_host()) data()->destination_2 = v; else guest::StoreU32(base, address + 24, v); }
  uint32_t destination_2_bits() const { return on_host() ? data()->destination_2 : guest::LoadU32(base, address + 24); }
  void set_destination_2_bits(uint32_t v) const { if (on_host()) data()->destination_2 = v; else guest::StoreU32(base, address + 24, v); }
  // the skeleton's matrices (64 bytes a bone)
  uint32_t matrices_address() const { return address + 28; }
  uint32_t matrices() const { return on_host() ? uint32_t(data()->matrices) : uint32_t(guest::LoadU32(base, address + 28)); }
  void set_matrices(uint32_t v) const { if (on_host()) data()->matrices = v; else guest::StoreU32(base, address + 28, v); }
  uint32_t matrices_bits() const { return on_host() ? data()->matrices : guest::LoadU32(base, address + 28); }
  void set_matrices_bits(uint32_t v) const { if (on_host()) data()->matrices = v; else guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_32() const { return on_host() ? uint32_t(data()->unknown_32) : uint32_t(guest::LoadU32(base, address + 32)); }
  void set_unknown_32(uint32_t v) const { if (on_host()) data()->unknown_32 = v; else guest::StoreU32(base, address + 32, v); }
  uint32_t unknown_32_bits() const { return on_host() ? data()->unknown_32 : guest::LoadU32(base, address + 32); }
  void set_unknown_32_bits(uint32_t v) const { if (on_host()) data()->unknown_32 = v; else guest::StoreU32(base, address + 32, v); }
  uint32_t vertex_count_address() const { return address + 36; }
  uint32_t vertex_count() const { return on_host() ? uint32_t(data()->vertex_count) : uint32_t(guest::LoadU32(base, address + 36)); }
  void set_vertex_count(uint32_t v) const { if (on_host()) data()->vertex_count = v; else guest::StoreU32(base, address + 36, v); }
  uint32_t vertex_count_bits() const { return on_host() ? data()->vertex_count : guest::LoadU32(base, address + 36); }
  void set_vertex_count_bits(uint32_t v) const { if (on_host()) data()->vertex_count = v; else guest::StoreU32(base, address + 36, v); }
  // 5, 9 or 10 pick the blend
  uint32_t stream_kind_address() const { return address + 40; }
  uint32_t stream_kind() const { return on_host() ? uint32_t(data()->stream_kind) : uint32_t(guest::LoadU32(base, address + 40)); }
  void set_stream_kind(uint32_t v) const { if (on_host()) data()->stream_kind = v; else guest::StoreU32(base, address + 40, v); }
  uint32_t stream_kind_bits() const { return on_host() ? data()->stream_kind : guest::LoadU32(base, address + 40); }
  void set_stream_kind_bits(uint32_t v) const { if (on_host()) data()->stream_kind = v; else guest::StoreU32(base, address + 40, v); }
  // with kind 5: 6, 7 or 8 pick the blend
  uint32_t stream_format_address() const { return address + 44; }
  uint32_t stream_format() const { return on_host() ? uint32_t(data()->stream_format) : uint32_t(guest::LoadU32(base, address + 44)); }
  void set_stream_format(uint32_t v) const { if (on_host()) data()->stream_format = v; else guest::StoreU32(base, address + 44, v); }
  uint32_t stream_format_bits() const { return on_host() ? data()->stream_format : guest::LoadU32(base, address + 44); }
  void set_stream_format_bits(uint32_t v) const { if (on_host()) data()->stream_format = v; else guest::StoreU32(base, address + 44, v); }
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
#pragma pack(push, 1)
struct MorphJobAt8Data {
  uint32_t unknown_0 = 0;  // +0
  uint32_t unknown_4 = 0;  // +4
  uint32_t unknown_8 = 0;  // +8
  uint32_t unknown_12 = 0;  // +12
  uint32_t unknown_16 = 0;  // +16
  uint32_t unknown_20 = 0;  // +20
  uint32_t unknown_24 = 0;  // +24
  uint32_t unknown_28 = 0;  // +28
  uint8_t pad_32[4] = {};
  uint32_t unknown_36 = 0;  // +36
  uint32_t unknown_40 = 0;  // +40
  uint32_t unknown_44 = 0;  // +44
};
#pragma pack(pop)
static_assert(sizeof(MorphJobAt8Data) == 48);
inline constexpr memory::LayoutField kMorphJobAt8Fields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {36, 4}, {40, 4}, {44, 4}};
inline constexpr memory::Layout kMorphJobAt8Layout = {kMorphJobAt8Fields, 11};

struct MorphJobAt8 : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  MorphJobAt8Data* data() const { return static_cast<MorphJobAt8Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static MorphJobAt8 New(uint8_t* base) {
    return MorphJobAt8(base, memory::MakeHandle(new MorphJobAt8Data(), sizeof(MorphJobAt8Data), "MorphJobAt8", &kMorphJobAt8Layout));
  }
  template <uint32_t Stride = kSize>
  static MorphJobAt8 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(MorphJobAt8Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<MorphJobAt8Data*>(::operator new(sizeof(MorphJobAt8Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) MorphJobAt8Data();
    return MorphJobAt8(base, memory::MakeHandle(p, sizeof(MorphJobAt8Data) * count, "MorphJobAt8", &kMorphJobAt8Layout, sizeof(MorphJobAt8Data)));
  }
  MorphJobAt8 at(uint32_t i) const { return MorphJobAt8(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kMorphJobAt8Layout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kMorphJobAt8Layout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_0() const { return on_host() ? uint32_t(data()->unknown_0) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_unknown_0(uint32_t v) const { if (on_host()) data()->unknown_0 = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_bits() const { return on_host() ? data()->unknown_0 : guest::LoadU32(base, address + 0); }
  void set_unknown_0_bits(uint32_t v) const { if (on_host()) data()->unknown_0 = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4() const { return on_host() ? uint32_t(data()->unknown_4) : uint32_t(guest::LoadU32(base, address + 4)); }
  void set_unknown_4(uint32_t v) const { if (on_host()) data()->unknown_4 = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_bits() const { return on_host() ? data()->unknown_4 : guest::LoadU32(base, address + 4); }
  void set_unknown_4_bits(uint32_t v) const { if (on_host()) data()->unknown_4 = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_8() const { return on_host() ? uint32_t(data()->unknown_8) : uint32_t(guest::LoadU32(base, address + 8)); }
  void set_unknown_8(uint32_t v) const { if (on_host()) data()->unknown_8 = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_bits() const { return on_host() ? data()->unknown_8 : guest::LoadU32(base, address + 8); }
  void set_unknown_8_bits(uint32_t v) const { if (on_host()) data()->unknown_8 = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_12() const { return on_host() ? uint32_t(data()->unknown_12) : uint32_t(guest::LoadU32(base, address + 12)); }
  void set_unknown_12(uint32_t v) const { if (on_host()) data()->unknown_12 = v; else guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_bits() const { return on_host() ? data()->unknown_12 : guest::LoadU32(base, address + 12); }
  void set_unknown_12_bits(uint32_t v) const { if (on_host()) data()->unknown_12 = v; else guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_16() const { return on_host() ? uint32_t(data()->unknown_16) : uint32_t(guest::LoadU32(base, address + 16)); }
  void set_unknown_16(uint32_t v) const { if (on_host()) data()->unknown_16 = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_bits() const { return on_host() ? data()->unknown_16 : guest::LoadU32(base, address + 16); }
  void set_unknown_16_bits(uint32_t v) const { if (on_host()) data()->unknown_16 = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_20() const { return on_host() ? uint32_t(data()->unknown_20) : uint32_t(guest::LoadU32(base, address + 20)); }
  void set_unknown_20(uint32_t v) const { if (on_host()) data()->unknown_20 = v; else guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_bits() const { return on_host() ? data()->unknown_20 : guest::LoadU32(base, address + 20); }
  void set_unknown_20_bits(uint32_t v) const { if (on_host()) data()->unknown_20 = v; else guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_24() const { return on_host() ? uint32_t(data()->unknown_24) : uint32_t(guest::LoadU32(base, address + 24)); }
  void set_unknown_24(uint32_t v) const { if (on_host()) data()->unknown_24 = v; else guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_bits() const { return on_host() ? data()->unknown_24 : guest::LoadU32(base, address + 24); }
  void set_unknown_24_bits(uint32_t v) const { if (on_host()) data()->unknown_24 = v; else guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_28() const { return on_host() ? uint32_t(data()->unknown_28) : uint32_t(guest::LoadU32(base, address + 28)); }
  void set_unknown_28(uint32_t v) const { if (on_host()) data()->unknown_28 = v; else guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_bits() const { return on_host() ? data()->unknown_28 : guest::LoadU32(base, address + 28); }
  void set_unknown_28_bits(uint32_t v) const { if (on_host()) data()->unknown_28 = v; else guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  uint32_t unknown_36() const { return on_host() ? uint32_t(data()->unknown_36) : uint32_t(guest::LoadU32(base, address + 36)); }
  void set_unknown_36(uint32_t v) const { if (on_host()) data()->unknown_36 = v; else guest::StoreU32(base, address + 36, v); }
  uint32_t unknown_36_bits() const { return on_host() ? data()->unknown_36 : guest::LoadU32(base, address + 36); }
  void set_unknown_36_bits(uint32_t v) const { if (on_host()) data()->unknown_36 = v; else guest::StoreU32(base, address + 36, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_40() const { return on_host() ? uint32_t(data()->unknown_40) : uint32_t(guest::LoadU32(base, address + 40)); }
  void set_unknown_40(uint32_t v) const { if (on_host()) data()->unknown_40 = v; else guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_bits() const { return on_host() ? data()->unknown_40 : guest::LoadU32(base, address + 40); }
  void set_unknown_40_bits(uint32_t v) const { if (on_host()) data()->unknown_40 = v; else guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_44() const { return on_host() ? uint32_t(data()->unknown_44) : uint32_t(guest::LoadU32(base, address + 44)); }
  void set_unknown_44(uint32_t v) const { if (on_host()) data()->unknown_44 = v; else guest::StoreU32(base, address + 44, v); }
  uint32_t unknown_44_bits() const { return on_host() ? data()->unknown_44 : guest::LoadU32(base, address + 44); }
  void set_unknown_44_bits(uint32_t v) const { if (on_host()) data()->unknown_44 = v; else guest::StoreU32(base, address + 44, v); }
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
