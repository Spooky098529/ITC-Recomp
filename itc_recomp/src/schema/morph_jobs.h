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
  static MorphJob New() {
    return MorphJob(memory::MakeHandle(new MorphJobData(), sizeof(MorphJobData), "MorphJob", &kMorphJobLayout));
  }
  template <uint32_t Stride = kSize>
  static MorphJob NewArray(uint32_t count) {
    static_assert(sizeof(MorphJobData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<MorphJobData*>(::operator new(sizeof(MorphJobData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) MorphJobData();
    return MorphJob(memory::MakeHandle(p, sizeof(MorphJobData) * count, "MorphJob", &kMorphJobLayout, sizeof(MorphJobData)));
  }
  MorphJob at(uint32_t i) const { return MorphJob(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  MorphJobData* direct() const { return static_cast<MorphJobData*>(memory::HandleOwn(address, &kMorphJobLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kMorphJobLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kMorphJobLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x8200286C (0x8200285C the base's, stored by the destructors)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = MorphJobVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  // set by command 22 (slot 3) after the wait; slot 2 reads it
  uint32_t released_address() const { return address + 4; }
  uint8_t released_bits() const { if (auto* d = direct()) return d->released; return uint8_t((memory::IsHandle(address + 4) ? uint8_t(memory::OwnerLoad(address + 4, 1)) : guest::LoadU8(address + 4))); }
  void set_released_bits(uint8_t v) const { if (auto* d = direct()) { d->released = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 1, v); else guest::StoreU8(address + 4, v); }
  uint8_t released() const { if (auto* d = direct()) return d->released; return uint8_t((memory::IsHandle(address + 4) ? uint8_t(memory::OwnerLoad(address + 4, 1)) : guest::LoadU8(address + 4))); }
  void set_released(uint8_t v) const { if (auto* d = direct()) { d->released = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 1, v); else guest::StoreU8(address + 4, v); }
  // the same word read 4 bytes wide (the skinning record's neighbor; not read)
  uint32_t released_word_address() const { return address + 4; }
  uint32_t released_word_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kMorphJobLayout, 4, 4)); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_released_word_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kMorphJobLayout, 4, 4, v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t released_word() const { return uint32_t(released_word_bits()); }
  void set_released_word(uint32_t v) const { set_released_word_bits(uint32_t(v)); }
  // the skinning record's +0: a word a vertex, its bone count
  uint32_t skin_influence_counts_address() const { return address + 8; }
  uint32_t skin_influence_counts_bits() const { if (auto* d = direct()) return d->skin_influence_counts; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_skin_influence_counts_bits(uint32_t v) const { if (auto* d = direct()) { d->skin_influence_counts = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  template <typename T = SkinIndexList>
  T skin_influence_counts() const { return T(skin_influence_counts_bits()); }
  void set_skin_influence_counts(const View& w) const { set_skin_influence_counts_bits(w.address); }
  void set_skin_influence_counts(uint32_t v) const { set_skin_influence_counts_bits(v); }
  // the skinning record's +4: a 16-byte weight vector an influence
  uint32_t skin_weights_address() const { return address + 12; }
  uint32_t skin_weights_bits() const { if (auto* d = direct()) return d->skin_weights; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_skin_weights_bits(uint32_t v) const { if (auto* d = direct()) { d->skin_weights = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t skin_weights() const { if (auto* d = direct()) return d->skin_weights; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_skin_weights(uint32_t v) const { if (auto* d = direct()) { d->skin_weights = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the skinning record's +8: a bone index word an influence
  uint32_t skin_bone_indices_address() const { return address + 16; }
  uint32_t skin_bone_indices_bits() const { if (auto* d = direct()) return d->skin_bone_indices; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_skin_bone_indices_bits(uint32_t v) const { if (auto* d = direct()) { d->skin_bone_indices = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t skin_bone_indices() const { if (auto* d = direct()) return d->skin_bone_indices; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_skin_bone_indices(uint32_t v) const { if (auto* d = direct()) { d->skin_bone_indices = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // the skinning record's +12: the source positions (16 bytes a vertex)
  uint32_t skin_source_address() const { return address + 20; }
  uint32_t skin_source_bits() const { if (auto* d = direct()) return d->skin_source; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_skin_source_bits(uint32_t v) const { if (auto* d = direct()) { d->skin_source = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  template <typename T = SkinVertexStream>
  T skin_source() const { return T(skin_source_bits()); }
  void set_skin_source(const View& w) const { set_skin_source_bits(w.address); }
  void set_skin_source(uint32_t v) const { set_skin_source_bits(v); }
  // the skinning record's +16: the skinned positions
  uint32_t skin_destination_address() const { return address + 24; }
  uint32_t skin_destination_bits() const { if (auto* d = direct()) return d->skin_destination; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_skin_destination_bits(uint32_t v) const { if (auto* d = direct()) { d->skin_destination = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  template <typename T = SkinVertexStream>
  T skin_destination() const { return T(skin_destination_bits()); }
  void set_skin_destination(const View& w) const { set_skin_destination_bits(w.address); }
  void set_skin_destination(uint32_t v) const { set_skin_destination_bits(v); }
  // the skinning record's +20: a second source stream (kinds 7 and 8, probably the normals)
  uint32_t skin_source_2_address() const { return address + 28; }
  uint32_t skin_source_2_bits() const { if (auto* d = direct()) return d->skin_source_2; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_skin_source_2_bits(uint32_t v) const { if (auto* d = direct()) { d->skin_source_2 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t skin_source_2() const { if (auto* d = direct()) return d->skin_source_2; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_skin_source_2(uint32_t v) const { if (auto* d = direct()) { d->skin_source_2 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  // the skinning record's +24: a second destination stream
  uint32_t skin_destination_2_address() const { return address + 32; }
  uint32_t skin_destination_2_bits() const { if (auto* d = direct()) return d->skin_destination_2; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_skin_destination_2_bits(uint32_t v) const { if (auto* d = direct()) { d->skin_destination_2 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t skin_destination_2() const { if (auto* d = direct()) return d->skin_destination_2; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_skin_destination_2(uint32_t v) const { if (auto* d = direct()) { d->skin_destination_2 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // the skinning record's +28: the skeleton's matrices (64 bytes a bone)
  uint32_t skin_matrices_address() const { return address + 36; }
  uint32_t skin_matrices_bits() const { if (auto* d = direct()) return d->skin_matrices; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_skin_matrices_bits(uint32_t v) const { if (auto* d = direct()) { d->skin_matrices = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t skin_matrices() const { if (auto* d = direct()) return d->skin_matrices; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_skin_matrices(uint32_t v) const { if (auto* d = direct()) { d->skin_matrices = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  // the skinning record's +32
  uint32_t skin_unknown_32_address() const { return address + 40; }
  uint32_t skin_unknown_32_bits() const { if (auto* d = direct()) return d->skin_unknown_32; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_skin_unknown_32_bits(uint32_t v) const { if (auto* d = direct()) { d->skin_unknown_32 = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t skin_unknown_32() const { if (auto* d = direct()) return d->skin_unknown_32; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_skin_unknown_32(uint32_t v) const { if (auto* d = direct()) { d->skin_unknown_32 = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  // the skinning record's +36
  uint32_t skin_vertex_count_address() const { return address + 44; }
  uint32_t skin_vertex_count_bits() const { if (auto* d = direct()) return d->skin_vertex_count; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_skin_vertex_count_bits(uint32_t v) const { if (auto* d = direct()) { d->skin_vertex_count = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t skin_vertex_count() const { if (auto* d = direct()) return d->skin_vertex_count; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_skin_vertex_count(uint32_t v) const { if (auto* d = direct()) { d->skin_vertex_count = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  // the skinning record's +40: the stream's kind (5, 9 or 10 pick the blend)
  uint32_t skin_stream_kind_address() const { return address + 48; }
  uint32_t skin_stream_kind_bits() const { if (auto* d = direct()) return d->skin_stream_kind; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_skin_stream_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->skin_stream_kind = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t skin_stream_kind() const { if (auto* d = direct()) return d->skin_stream_kind; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_skin_stream_kind(uint32_t v) const { if (auto* d = direct()) { d->skin_stream_kind = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // the skinning record's +44: with kind 5, 6, 7 or 8 picks the blend
  uint32_t skin_stream_format_address() const { return address + 52; }
  uint32_t skin_stream_format_bits() const { if (auto* d = direct()) return d->skin_stream_format; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_skin_stream_format_bits(uint32_t v) const { if (auto* d = direct()) { d->skin_stream_format = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t skin_stream_format() const { if (auto* d = direct()) return d->skin_stream_format; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_skin_stream_format(uint32_t v) const { if (auto* d = direct()) { d->skin_stream_format = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  // the morph record copied from Model::MorphShape's frame (Morph::Blend's r3, second-set.md)
  uint32_t morph_record_address() const { return address + 56; }
  uint8_t* morph_record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 56 : guest::Pointer(address + 56); }
  // the work item's +0: 0x820066DC (0x82000EB8 its base's)
  uint32_t item_vtable_address() const { return address + 184; }
  uint32_t item_vtable_bits() const { if (auto* d = direct()) return d->item_vtable; return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(address + 184))); }
  void set_item_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->item_vtable = v; return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(address + 184, v); }
  uint32_t item_vtable() const { if (auto* d = direct()) return d->item_vtable; return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(address + 184))); }
  void set_item_vtable(uint32_t v) const { if (auto* d = direct()) { d->item_vtable = v; return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(address + 184, v); }
  // the work item's +4: 0x82002908 (0x82000AB4 written first, 0x82000BA8 by the destructor)
  uint32_t item_interface_address() const { return address + 188; }
  uint32_t item_interface_bits() const { if (auto* d = direct()) return d->item_interface; return uint32_t((memory::IsHandle(address + 188) ? uint32_t(memory::OwnerLoad(address + 188, 4)) : guest::LoadU32(address + 188))); }
  void set_item_interface_bits(uint32_t v) const { if (auto* d = direct()) { d->item_interface = v; return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 4, v); else guest::StoreU32(address + 188, v); }
  uint32_t item_interface() const { if (auto* d = direct()) return d->item_interface; return uint32_t((memory::IsHandle(address + 188) ? uint32_t(memory::OwnerLoad(address + 188, 4)) : guest::LoadU32(address + 188))); }
  void set_item_interface(uint32_t v) const { if (auto* d = direct()) { d->item_interface = v; return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 4, v); else guest::StoreU32(address + 188, v); }
  // the work item's +8: a worker's value, set when it starts
  uint32_t item_worker_value_address() const { return address + 192; }
  uint32_t item_worker_value_bits() const { if (auto* d = direct()) return d->item_worker_value; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(address + 192))); }
  void set_item_worker_value_bits(uint32_t v) const { if (auto* d = direct()) { d->item_worker_value = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(address + 192, v); }
  uint32_t item_worker_value() const { if (auto* d = direct()) return d->item_worker_value; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(address + 192))); }
  void set_item_worker_value(uint32_t v) const { if (auto* d = direct()) { d->item_worker_value = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(address + 192, v); }
  // the work item's +12: 1 queued, 2 running, 3 done
  uint32_t item_state_address() const { return address + 196; }
  uint32_t item_state_bits() const { if (auto* d = direct()) return d->item_state; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(address + 196))); }
  void set_item_state_bits(uint32_t v) const { if (auto* d = direct()) { d->item_state = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(address + 196, v); }
  uint32_t item_state() const { if (auto* d = direct()) return d->item_state; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(address + 196))); }
  void set_item_state(uint32_t v) const { if (auto* d = direct()) { d->item_state = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(address + 196, v); }
  // the work item's +16
  uint32_t item_skip_address() const { return address + 200; }
  uint8_t item_skip_bits() const { if (auto* d = direct()) return d->item_skip; return uint8_t((memory::IsHandle(address + 200) ? uint8_t(memory::OwnerLoad(address + 200, 1)) : guest::LoadU8(address + 200))); }
  void set_item_skip_bits(uint8_t v) const { if (auto* d = direct()) { d->item_skip = v; return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 1, v); else guest::StoreU8(address + 200, v); }
  uint8_t item_skip() const { if (auto* d = direct()) return d->item_skip; return uint8_t((memory::IsHandle(address + 200) ? uint8_t(memory::OwnerLoad(address + 200, 1)) : guest::LoadU8(address + 200))); }
  void set_item_skip(uint8_t v) const { if (auto* d = direct()) { d->item_skip = v; return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 1, v); else guest::StoreU8(address + 200, v); }
  // the work item's +17: the post's r5 (1 here)
  uint32_t item_post_flag_address() const { return address + 201; }
  uint8_t item_post_flag_bits() const { if (auto* d = direct()) return d->item_post_flag; return uint8_t((memory::IsHandle(address + 201) ? uint8_t(memory::OwnerLoad(address + 201, 1)) : guest::LoadU8(address + 201))); }
  void set_item_post_flag_bits(uint8_t v) const { if (auto* d = direct()) { d->item_post_flag = v; return; } if (memory::IsHandle(address + 201)) memory::OwnerStore(address + 201, 1, v); else guest::StoreU8(address + 201, v); }
  uint8_t item_post_flag() const { if (auto* d = direct()) return d->item_post_flag; return uint8_t((memory::IsHandle(address + 201) ? uint8_t(memory::OwnerLoad(address + 201, 1)) : guest::LoadU8(address + 201))); }
  void set_item_post_flag(uint8_t v) const { if (auto* d = direct()) { d->item_post_flag = v; return; } if (memory::IsHandle(address + 201)) memory::OwnerStore(address + 201, 1, v); else guest::StoreU8(address + 201, v); }
  // the work item's +18
  uint32_t item_posted_address() const { return address + 202; }
  uint8_t item_posted_bits() const { if (auto* d = direct()) return d->item_posted; return uint8_t((memory::IsHandle(address + 202) ? uint8_t(memory::OwnerLoad(address + 202, 1)) : guest::LoadU8(address + 202))); }
  void set_item_posted_bits(uint8_t v) const { if (auto* d = direct()) { d->item_posted = v; return; } if (memory::IsHandle(address + 202)) memory::OwnerStore(address + 202, 1, v); else guest::StoreU8(address + 202, v); }
  uint8_t item_posted() const { if (auto* d = direct()) return d->item_posted; return uint8_t((memory::IsHandle(address + 202) ? uint8_t(memory::OwnerLoad(address + 202, 1)) : guest::LoadU8(address + 202))); }
  void set_item_posted(uint8_t v) const { if (auto* d = direct()) { d->item_posted = v; return; } if (memory::IsHandle(address + 202)) memory::OwnerStore(address + 202, 1, v); else guest::StoreU8(address + 202, v); }
  // the work item's +20: the pool's waiter interface; the constructor copies an unset frame word
  uint32_t item_waiter_address() const { return address + 204; }
  uint32_t item_waiter_bits() const { if (auto* d = direct()) return d->item_waiter; return uint32_t((memory::IsHandle(address + 204) ? uint32_t(memory::OwnerLoad(address + 204, 4)) : guest::LoadU32(address + 204))); }
  void set_item_waiter_bits(uint32_t v) const { if (auto* d = direct()) { d->item_waiter = v; return; } if (memory::IsHandle(address + 204)) memory::OwnerStore(address + 204, 4, v); else guest::StoreU32(address + 204, v); }
  uint32_t item_waiter() const { if (auto* d = direct()) return d->item_waiter; return uint32_t((memory::IsHandle(address + 204) ? uint32_t(memory::OwnerLoad(address + 204, 4)) : guest::LoadU32(address + 204))); }
  void set_item_waiter(uint32_t v) const { if (auto* d = direct()) { d->item_waiter = v; return; } if (memory::IsHandle(address + 204)) memory::OwnerStore(address + 204, 4, v); else guest::StoreU32(address + 204, v); }
  // the work item's +24: 0x82002850 (0x82000ABC its base's, by the destructor)
  uint32_t delegate_vtable_address() const { return address + 208; }
  uint32_t delegate_vtable_bits() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 208) ? uint32_t(memory::OwnerLoad(address + 208, 4)) : guest::LoadU32(address + 208))); }
  void set_delegate_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 208)) memory::OwnerStore(address + 208, 4, v); else guest::StoreU32(address + 208, v); }
  uint32_t delegate_vtable() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 208) ? uint32_t(memory::OwnerLoad(address + 208, 4)) : guest::LoadU32(address + 208))); }
  void set_delegate_vtable(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 208)) memory::OwnerStore(address + 208, 4, v); else guest::StoreU32(address + 208, v); }
  // the work item's +28: the job
  uint32_t delegate_object_address() const { return address + 212; }
  uint32_t delegate_object_bits() const { if (auto* d = direct()) return d->delegate_object; return uint32_t((memory::IsHandle(address + 212) ? uint32_t(memory::OwnerLoad(address + 212, 4)) : guest::LoadU32(address + 212))); }
  void set_delegate_object_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_object = v; return; } if (memory::IsHandle(address + 212)) memory::OwnerStore(address + 212, 4, v); else guest::StoreU32(address + 212, v); }
  template <typename T = MorphJob>
  T delegate_object() const { return T(delegate_object_bits()); }
  void set_delegate_object(const View& w) const { set_delegate_object_bits(w.address); }
  void set_delegate_object(uint32_t v) const { set_delegate_object_bits(v); }
  // the work item's +32: MorphJob::RunThunk sub_820AC9C0
  uint32_t delegate_target_address() const { return address + 216; }
  uint32_t delegate_target_bits() const { if (auto* d = direct()) return d->delegate_target; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(address + 216))); }
  void set_delegate_target_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_target = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(address + 216, v); }
  uint32_t delegate_target() const { if (auto* d = direct()) return d->delegate_target; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(address + 216))); }
  void set_delegate_target(uint32_t v) const { if (auto* d = direct()) { d->delegate_target = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(address + 216, v); }
  // 1 a morph (Morph::Blend on +56), 0 a skin (sub_8258BCD8 on +8)
  uint32_t kind_address() const { return address + 220; }
  uint32_t kind_bits() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 220) ? uint32_t(memory::OwnerLoad(address + 220, 4)) : guest::LoadU32(address + 220))); }
  void set_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 220)) memory::OwnerStore(address + 220, 4, v); else guest::StoreU32(address + 220, v); }
  uint32_t kind() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 220) ? uint32_t(memory::OwnerLoad(address + 220, 4)) : guest::LoadU32(address + 220))); }
  void set_kind(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 220)) memory::OwnerStore(address + 220, 4, v); else guest::StoreU32(address + 220, v); }
};

// image; measure: MorphJob.+0
struct MorphJobVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // slot 0: MorphJob::~MorphJob sub_820ACB90 (deleting)
  uint32_t destroy() const { return guest::LoadU32(address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: MorphJob::Wait sub_820AC9B8
  uint32_t wait() const { return guest::LoadU32(address + 4); }
  void set_wait(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t wait_address() const { return address + 4; }
  // slot 2: sub_820AC9D0
  uint32_t is_released() const { return guest::LoadU32(address + 8); }
  void set_is_released(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t is_released_address() const { return address + 8; }
  // slot 3: sub_820AC9D8
  uint32_t mark_released() const { return guest::LoadU32(address + 12); }
  void set_mark_released(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t mark_released_address() const { return address + 12; }
  // slot 4: MorphJob::Run sub_820AC998
  uint32_t run() const { return guest::LoadU32(address + 16); }
  void set_run(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t run_address() const { return address + 16; }
};

// embedded; measure: WorkItem, Job, Job::Wait.r3
struct WorkItem : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // 0x820066DC (0x82000EB8 its base's, by the destructor)
  template <typename T = WorkItemVtable>
  T vtable() const { return T(guest::LoadU32(address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x82002908 (0x82000AB4 written first, 0x82000BA8 by the destructor)
  uint32_t interface() const { return guest::LoadU32(address + 4); }
  void set_interface(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t interface_address() const { return address + 4; }
  // a worker's value (its frame + 80, or 0), set by Started
  uint32_t worker_value() const { return guest::LoadU32(address + 8); }
  void set_worker_value(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t worker_value_address() const { return address + 8; }
  // 1 queued, 2 running, 3 done
  uint32_t state() const { return guest::LoadU32(address + 12); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t state_address() const { return address + 12; }
  // the run isn't made when set
  uint8_t skip() const { return guest::LoadU8(address + 16); }
  void set_skip(uint8_t v) const { guest::StoreU8(address + 16, v); }
  uint32_t skip_address() const { return address + 16; }
  // the post's r5
  uint8_t post_flag() const { return guest::LoadU8(address + 17); }
  void set_post_flag(uint8_t v) const { guest::StoreU8(address + 17, v); }
  uint32_t post_flag_address() const { return address + 17; }
  // cleared by Job::Wait
  uint8_t posted() const { return guest::LoadU8(address + 18); }
  void set_posted(uint8_t v) const { guest::StoreU8(address + 18, v); }
  uint32_t posted_address() const { return address + 18; }
  // the pool's waiter interface (pool - 4); Job::Wait calls its slot 1
  template <typename T = FramePassesJobWaiter>
  T waiter() const { return T(guest::LoadU32(address + 20)); }
  void set_waiter(const View& v) const { guest::StoreU32(address + 20, v.address); }
  void set_waiter(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t waiter_address() const { return address + 20; }
  // a 12-byte delegate: 0x82002850 (0x82000ABC its base's, by the destructor)
  uint32_t delegate_vtable() const { return guest::LoadU32(address + 24); }
  void set_delegate_vtable(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t delegate_vtable_address() const { return address + 24; }
  uint32_t delegate_object() const { return guest::LoadU32(address + 28); }
  void set_delegate_object(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t delegate_object_address() const { return address + 28; }
  uint32_t delegate_target() const { return guest::LoadU32(address + 32); }
  void set_delegate_target(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t delegate_target_address() const { return address + 32; }
};

// image; measure: WorkItem.+0
struct WorkItemVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // slot 0: WorkItem::~WorkItem sub_82090E40 (deleting)
  uint32_t destroy() const { return guest::LoadU32(address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: sub_82090DD0
  uint32_t posted() const { return guest::LoadU32(address + 4); }
  void set_posted(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t posted_address() const { return address + 4; }
  // slot 2: sub_82090DE8
  uint32_t started() const { return guest::LoadU32(address + 8); }
  void set_started(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t started_address() const { return address + 8; }
  // slot 3: sub_82090DF8
  uint32_t finished() const { return guest::LoadU32(address + 12); }
  void set_finished(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t finished_address() const { return address + 12; }
  // slot 4: sub_82090E08
  uint32_t skip() const { return guest::LoadU32(address + 16); }
  void set_skip(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t skip_address() const { return address + 16; }
  // slot 5: sub_82090E18
  uint32_t run() const { return guest::LoadU32(address + 20); }
  void set_run(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t run_address() const { return address + 20; }
  // slot 6: sub_82090CB8
  uint32_t is_skipped() const { return guest::LoadU32(address + 24); }
  void set_is_skipped(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t is_skipped_address() const { return address + 24; }
  // slot 7: WorkItem::Call sub_820ACA30
  uint32_t call() const { return guest::LoadU32(address + 28); }
  void set_call(uint32_t v) const { guest::StoreU32(address + 28, v); }
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
  static SkinRecord New() {
    return SkinRecord(memory::MakeHandle(new SkinRecordData(), sizeof(SkinRecordData), "SkinRecord", &kSkinRecordLayout));
  }
  template <uint32_t Stride = kSize>
  static SkinRecord NewArray(uint32_t count) {
    static_assert(sizeof(SkinRecordData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SkinRecordData*>(::operator new(sizeof(SkinRecordData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SkinRecordData();
    return SkinRecord(memory::MakeHandle(p, sizeof(SkinRecordData) * count, "SkinRecord", &kSkinRecordLayout, sizeof(SkinRecordData)));
  }
  SkinRecord at(uint32_t i) const { return SkinRecord(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SkinRecordData* direct() const { return static_cast<SkinRecordData*>(memory::HandleOwn(address, &kSkinRecordLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSkinRecordLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSkinRecordLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a word a vertex, its bone count
  uint32_t influence_counts_address() const { return address + 0; }
  uint32_t influence_counts_bits() const { if (auto* d = direct()) return d->influence_counts; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_influence_counts_bits(uint32_t v) const { if (auto* d = direct()) { d->influence_counts = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = SkinIndexList>
  T influence_counts() const { return T(influence_counts_bits()); }
  void set_influence_counts(const View& w) const { set_influence_counts_bits(w.address); }
  void set_influence_counts(uint32_t v) const { set_influence_counts_bits(v); }
  // a 16-byte weight vector an influence
  uint32_t weights_address() const { return address + 4; }
  uint32_t weights_bits() const { if (auto* d = direct()) return d->weights; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_weights_bits(uint32_t v) const { if (auto* d = direct()) { d->weights = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t weights() const { if (auto* d = direct()) return d->weights; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_weights(uint32_t v) const { if (auto* d = direct()) { d->weights = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // a bone index word an influence
  uint32_t bone_indices_address() const { return address + 8; }
  uint32_t bone_indices_bits() const { if (auto* d = direct()) return d->bone_indices; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_bone_indices_bits(uint32_t v) const { if (auto* d = direct()) { d->bone_indices = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  template <typename T = SkinIndexList>
  T bone_indices() const { return T(bone_indices_bits()); }
  void set_bone_indices(const View& w) const { set_bone_indices_bits(w.address); }
  void set_bone_indices(uint32_t v) const { set_bone_indices_bits(v); }
  // the source positions (16 bytes a vertex)
  uint32_t source_address() const { return address + 12; }
  uint32_t source_bits() const { if (auto* d = direct()) return d->source; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_source_bits(uint32_t v) const { if (auto* d = direct()) { d->source = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  template <typename T = SkinVertexStream>
  T source() const { return T(source_bits()); }
  void set_source(const View& w) const { set_source_bits(w.address); }
  void set_source(uint32_t v) const { set_source_bits(v); }
  // the skinned positions
  uint32_t destination_address() const { return address + 16; }
  uint32_t destination_bits() const { if (auto* d = direct()) return d->destination; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_destination_bits(uint32_t v) const { if (auto* d = direct()) { d->destination = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  template <typename T = SkinVertexStream>
  T destination() const { return T(destination_bits()); }
  void set_destination(const View& w) const { set_destination_bits(w.address); }
  void set_destination(uint32_t v) const { set_destination_bits(v); }
  // a second source stream (kinds 7 and 8, probably the normals)
  uint32_t source_2_address() const { return address + 20; }
  uint32_t source_2_bits() const { if (auto* d = direct()) return d->source_2; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_source_2_bits(uint32_t v) const { if (auto* d = direct()) { d->source_2 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t source_2() const { if (auto* d = direct()) return d->source_2; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_source_2(uint32_t v) const { if (auto* d = direct()) { d->source_2 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // a second destination stream
  uint32_t destination_2_address() const { return address + 24; }
  uint32_t destination_2_bits() const { if (auto* d = direct()) return d->destination_2; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_destination_2_bits(uint32_t v) const { if (auto* d = direct()) { d->destination_2 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t destination_2() const { if (auto* d = direct()) return d->destination_2; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_destination_2(uint32_t v) const { if (auto* d = direct()) { d->destination_2 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  // the skeleton's matrices (64 bytes a bone)
  uint32_t matrices_address() const { return address + 28; }
  uint32_t matrices_bits() const { if (auto* d = direct()) return d->matrices; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_matrices_bits(uint32_t v) const { if (auto* d = direct()) { d->matrices = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t matrices() const { if (auto* d = direct()) return d->matrices; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_matrices(uint32_t v) const { if (auto* d = direct()) { d->matrices = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_32_bits() const { if (auto* d = direct()) return d->unknown_32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_unknown_32_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t unknown_32() const { if (auto* d = direct()) return d->unknown_32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_unknown_32(uint32_t v) const { if (auto* d = direct()) { d->unknown_32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t vertex_count_address() const { return address + 36; }
  uint32_t vertex_count_bits() const { if (auto* d = direct()) return d->vertex_count; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_vertex_count_bits(uint32_t v) const { if (auto* d = direct()) { d->vertex_count = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t vertex_count() const { if (auto* d = direct()) return d->vertex_count; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_vertex_count(uint32_t v) const { if (auto* d = direct()) { d->vertex_count = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  // 5, 9 or 10 pick the blend
  uint32_t stream_kind_address() const { return address + 40; }
  uint32_t stream_kind_bits() const { if (auto* d = direct()) return d->stream_kind; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_stream_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->stream_kind = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t stream_kind() const { if (auto* d = direct()) return d->stream_kind; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_stream_kind(uint32_t v) const { if (auto* d = direct()) { d->stream_kind = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  // with kind 5: 6, 7 or 8 pick the blend
  uint32_t stream_format_address() const { return address + 44; }
  uint32_t stream_format_bits() const { if (auto* d = direct()) return d->stream_format; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_stream_format_bits(uint32_t v) const { if (auto* d = direct()) { d->stream_format = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t stream_format() const { if (auto* d = direct()) return d->stream_format; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_stream_format(uint32_t v) const { if (auto* d = direct()) { d->stream_format = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
};

// embedded; measure: sub_8258AC58.r3.+0, sub_8258ADA0.r3.+0, sub_8258B1C0.r3.+0, sub_8258B368.r3.+0, sub_8258AC58.r3.+8, sub_8258ADA0.r3.+8, sub_8258B1C0.r3.+8, sub_8258B368.r3.+8, MorphJob@+8.+8
struct SkinIndexList : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t entry_0() const { return guest::LoadU32(address + 0); }
  void set_entry_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t entry_0_address() const { return address + 0; }
  uint32_t entry_1() const { return guest::LoadU32(address + 4); }
  void set_entry_1(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t entry_1_address() const { return address + 4; }
};

// embedded; measure: sub_8258B368.r3.+12, sub_8258B368.r3.+16, MorphJob@+8.+20, MorphJob@+8.+24
struct SkinVertexStream : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // not read in detail
  float float_28() const { return guest::LoadFloat(address + 28); }
  void set_float_28(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t float_28_address() const { return address + 28; }
  // not read in detail
  float float_44() const { return guest::LoadFloat(address + 44); }
  void set_float_44(float v) const { guest::StoreFloat(address + 44, v); }
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
  static MorphJobAt8 New() {
    return MorphJobAt8(memory::MakeHandle(new MorphJobAt8Data(), sizeof(MorphJobAt8Data), "MorphJobAt8", &kMorphJobAt8Layout));
  }
  template <uint32_t Stride = kSize>
  static MorphJobAt8 NewArray(uint32_t count) {
    static_assert(sizeof(MorphJobAt8Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<MorphJobAt8Data*>(::operator new(sizeof(MorphJobAt8Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) MorphJobAt8Data();
    return MorphJobAt8(memory::MakeHandle(p, sizeof(MorphJobAt8Data) * count, "MorphJobAt8", &kMorphJobAt8Layout, sizeof(MorphJobAt8Data)));
  }
  MorphJobAt8 at(uint32_t i) const { return MorphJobAt8(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  MorphJobAt8Data* direct() const { return static_cast<MorphJobAt8Data*>(memory::HandleOwn(address, &kMorphJobAt8Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kMorphJobAt8Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kMorphJobAt8Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_0_bits() const { if (auto* d = direct()) return d->unknown_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_unknown_0_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t unknown_0() const { if (auto* d = direct()) return d->unknown_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_unknown_0(uint32_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4_bits() const { if (auto* d = direct()) return d->unknown_4; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_unknown_4_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_4 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t unknown_4() const { if (auto* d = direct()) return d->unknown_4; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_unknown_4(uint32_t v) const { if (auto* d = direct()) { d->unknown_4 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_8_bits() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_unknown_8_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t unknown_8() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_unknown_8(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_12_bits() const { if (auto* d = direct()) return d->unknown_12; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_unknown_12_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_12 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t unknown_12() const { if (auto* d = direct()) return d->unknown_12; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_unknown_12(uint32_t v) const { if (auto* d = direct()) { d->unknown_12 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_16_bits() const { if (auto* d = direct()) return d->unknown_16; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_unknown_16_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t unknown_16() const { if (auto* d = direct()) return d->unknown_16; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_unknown_16(uint32_t v) const { if (auto* d = direct()) { d->unknown_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_20_bits() const { if (auto* d = direct()) return d->unknown_20; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_unknown_20_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_20 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t unknown_20() const { if (auto* d = direct()) return d->unknown_20; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_unknown_20(uint32_t v) const { if (auto* d = direct()) { d->unknown_20 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_24_bits() const { if (auto* d = direct()) return d->unknown_24; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_unknown_24_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_24 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t unknown_24() const { if (auto* d = direct()) return d->unknown_24; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_unknown_24(uint32_t v) const { if (auto* d = direct()) { d->unknown_24 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_28_bits() const { if (auto* d = direct()) return d->unknown_28; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_unknown_28_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_28 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t unknown_28() const { if (auto* d = direct()) return d->unknown_28; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_unknown_28(uint32_t v) const { if (auto* d = direct()) { d->unknown_28 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  uint32_t unknown_36_bits() const { if (auto* d = direct()) return d->unknown_36; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_unknown_36_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_36 = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t unknown_36() const { if (auto* d = direct()) return d->unknown_36; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_unknown_36(uint32_t v) const { if (auto* d = direct()) { d->unknown_36 = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_40_bits() const { if (auto* d = direct()) return d->unknown_40; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_unknown_40_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_40 = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t unknown_40() const { if (auto* d = direct()) return d->unknown_40; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_unknown_40(uint32_t v) const { if (auto* d = direct()) { d->unknown_40 = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_44_bits() const { if (auto* d = direct()) return d->unknown_44; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_unknown_44_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_44 = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t unknown_44() const { if (auto* d = direct()) return d->unknown_44; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_unknown_44(uint32_t v) const { if (auto* d = direct()) { d->unknown_44 = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
};

// embedded; measure: MorphJob@+8.+0
struct MorphJobVtablePart : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: sub_82090DC8.r3@-4
struct WorkItemOuter4 : View {
  using View::View;
  static constexpr uint32_t kSize = 17;
  uint8_t unknown_16() const { return guest::LoadU8(address + 16); }
  void set_unknown_16(uint8_t v) const { guest::StoreU8(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// embedded; measure: sub_82090E38.r3@-4
struct WorkItemOuter4_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_24() const { return guest::LoadU32(address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
};

}  // namespace schema
