// The views of docs/functions/heap.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/heap.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: [8297149C]
struct ProcessHeap : View {
  using View::View;
  static constexpr uint32_t kSize = 1412;
  // the heap's flags; 0x40000: check the caller's process type (+379)
  uint32_t flags() const { return guest::LoadU32(base, address + 20); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t flags_address() const { return address + 20; }
  // ORed into every call's flags (HeapReAlloc)
  uint32_t force_flags() const { return guest::LoadU32(base, address + 24); }
  void set_force_flags(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t force_flags_address() const { return address + 24; }
  // probably (NT's): sizes above it get their own virtual block
  uint32_t virtual_memory_threshold() const { return guest::LoadU32(base, address + 28); }
  void set_virtual_memory_threshold(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t virtual_memory_threshold_address() const { return address + 28; }
  // probably (NT's), read by HeapFree
  uint32_t decommit_free_block_threshold() const { return guest::LoadU32(base, address + 40); }
  void set_decommit_free_block_threshold(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t decommit_free_block_threshold_address() const { return address + 40; }
  // probably (NT's), read by HeapFree
  uint32_t decommit_total_free_threshold() const { return guest::LoadU32(base, address + 44); }
  void set_decommit_total_free_threshold(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t decommit_total_free_threshold_address() const { return address + 44; }
  // probably (NT's): updated by every allocation and free
  uint32_t total_free_size() const { return guest::LoadU32(base, address + 48); }
  void set_total_free_size(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t total_free_size_address() const { return address + 48; }
  // added to a size before align_mask (HeapReAlloc)
  uint32_t align_round() const { return guest::LoadU32(base, address + 80); }
  void set_align_round(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t align_round_address() const { return address + 80; }
  uint32_t align_mask() const { return guest::LoadU32(base, address + 84); }
  void set_align_mask(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t align_mask_address() const { return address + 84; }
  // compared with KeGetCurrentProcessType when flags has 0x40000
  uint8_t process_type() const { return guest::LoadU8(base, address + 379); }
  void set_process_type(uint8_t v) const { guest::StoreU8(base, address + 379, v); }
  uint32_t process_type_address() const { return address + 379; }
  // read by HeapReAlloc: when 0 (and no other flag), the size isn't padded by 16
  uint32_t unknown_380() const { return guest::LoadU32(base, address + 380); }
  void set_unknown_380(uint32_t v) const { guest::StoreU32(base, address + 380, v); }
  uint32_t unknown_380_address() const { return address + 380; }
  // free list 0's head (blocks of 128 units and more, by size)
  template <typename T = HeapListEntry>
  T large_free_first() const { return T(base, guest::LoadU32(base, address + 384)); }
  void set_large_free_first(const View& v) const { guest::StoreU32(base, address + 384, v.address); }
  void set_large_free_first(uint32_t v) const { guest::StoreU32(base, address + 384, v); }
  uint32_t large_free_first_address() const { return address + 384; }
  template <typename T = HeapListEntry>
  T large_free_last() const { return T(base, guest::LoadU32(base, address + 388)); }
  void set_large_free_last(const View& v) const { guest::StoreU32(base, address + 388, v.address); }
  void set_large_free_last(uint32_t v) const { guest::StoreU32(base, address + 388, v); }
  uint32_t large_free_last_address() const { return address + 388; }
  // free lists 1 to 127 (8 bytes each, indexed by size)
  uint32_t free_lists_address() const { return address + 392; }
  // the heap's critical section (taken unless the flags have 0x1)
  uint32_t lock() const { return guest::LoadU32(base, address + 1408); }
  void set_lock(uint32_t v) const { guest::StoreU32(base, address + 1408, v); }
  uint32_t lock_address() const { return address + 1408; }
};

// embedded; measure: [8297149C].+384, [8297149C].+384.+0, [8297149C].+388, HeapFree.r5.+-44, HeapFree.r5.+-48
struct HeapListEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // a free block's size in 16-byte units (its header's +0)
  uint16_t free_size() const { return guest::LoadU16(base, address + -8); }
  void set_free_size(uint16_t v) const { guest::StoreU16(base, address + -8, v); }
  uint32_t free_size_address() const { return address + -8; }
  template <typename T = HeapListEntry>
  T next() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  template <typename T = HeapListEntry>
  T previous() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
};

// allocated; measure: HeapFree.r5, HeapSize.r5, new<?>@HeapReAlloc, [8297149C].+384.+0.+4, HeapReAlloc.r5
struct HeapBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  // a large block's link in the heap's virtual blocks list
  uint32_t large_next() const { return guest::LoadU32(base, address + -48); }
  void set_large_next(uint32_t v) const { guest::StoreU32(base, address + -48, v); }
  uint32_t large_next_address() const { return address + -48; }
  uint32_t large_previous() const { return guest::LoadU32(base, address + -44); }
  void set_large_previous(uint32_t v) const { guest::StoreU32(base, address + -44, v); }
  uint32_t large_previous_address() const { return address + -44; }
  // HeapReAlloc's in-place resize: a header's size the measure puts 48 bytes before the block (probably another header than this block's)
  uint16_t unknown_m48() const { return guest::LoadU16(base, address + -48); }
  void set_unknown_m48(uint16_t v) const { guest::StoreU16(base, address + -48, v); }
  uint32_t unknown_m48_address() const { return address + -48; }
  // the same header's segment index (probably)
  uint8_t unknown_m44() const { return guest::LoadU8(base, address + -44); }
  void set_unknown_m44(uint8_t v) const { guest::StoreU8(base, address + -44, v); }
  uint32_t unknown_m44_address() const { return address + -44; }
  // the same header's flags (probably)
  uint8_t unknown_m43() const { return guest::LoadU8(base, address + -43); }
  void set_unknown_m43(uint8_t v) const { guest::StoreU8(base, address + -43, v); }
  uint32_t unknown_m43_address() const { return address + -43; }
  // a large block's committed bytes (HeapSize: less the unused bytes and 48)
  uint32_t large_commit_size() const { return guest::LoadU32(base, address + -24); }
  void set_large_commit_size(uint32_t v) const { guest::StoreU32(base, address + -24, v); }
  uint32_t large_commit_size_address() const { return address + -24; }
  // in 16-byte units; a large block's unused bytes
  uint16_t size() const { return guest::LoadU16(base, address + -16); }
  void set_size(uint16_t v) const { guest::StoreU16(base, address + -16, v); }
  uint32_t size_address() const { return address + -16; }
  // 0x1 busy, 0x2 with extra, 0x8 a large virtual block
  uint8_t flags() const { return guest::LoadU8(base, address + -11); }
  void set_flags(uint8_t v) const { guest::StoreU8(base, address + -11, v); }
  uint32_t flags_address() const { return address + -11; }
  uint8_t unused_bytes() const { return guest::LoadU8(base, address + -10); }
  void set_unused_bytes(uint8_t v) const { guest::StoreU8(base, address + -10, v); }
  uint32_t unused_bytes_address() const { return address + -10; }
  // the measure joins HeapReAlloc's block with a free link: the free lists' next written here
  uint32_t link_next() const { return guest::LoadU32(base, address + 0); }
  void set_link_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t link_next_address() const { return address + 0; }
};

// embedded; measure: _calloc_impl.r5
struct CallocErrorOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 12 (ENOMEM) on failure
  uint32_t error() const { return guest::LoadU32(base, address + 0); }
  void set_error(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t error_address() const { return address + 0; }
};

// allocated; measure: new<16>@D3DXCreateBuffer
#pragma pack(push, 1)
struct D3DXBufferData {
  uint32_t vtable = 0;  // +0
  uint32_t ref_count = 0;  // +4
  uint32_t size = 0;  // +8
  uint32_t data = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(D3DXBufferData) == 16);
inline constexpr memory::LayoutField kD3DXBufferFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kD3DXBufferLayout = {kD3DXBufferFields, 4};

struct D3DXBuffer : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82019DC0;
  D3DXBufferData* data() const { return static_cast<D3DXBufferData*>(memory::HandleObject(address)); }
  static D3DXBuffer New(uint8_t* base) {
    return D3DXBuffer(base, memory::MakeHandle(new D3DXBufferData(), sizeof(D3DXBufferData), "D3DXBuffer", &kD3DXBufferLayout));
  }
  template <uint32_t Stride = kSize>
  static D3DXBuffer NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(D3DXBufferData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<D3DXBufferData*>(::operator new(sizeof(D3DXBufferData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) D3DXBufferData();
    return D3DXBuffer(base, memory::MakeHandle(p, sizeof(D3DXBufferData) * count, "D3DXBuffer", &kD3DXBufferLayout, sizeof(D3DXBufferData)));
  }
  D3DXBuffer at(uint32_t i) const { return D3DXBuffer(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kD3DXBufferLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kD3DXBufferLayout, offset, width, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  uint32_t ref_count_address() const { return address + 4; }
  uint32_t ref_count() const { return data()->ref_count; }
  void set_ref_count(uint32_t v) const { data()->ref_count = v; }
  uint32_t ref_count_bits() const { return data()->ref_count; }
  void set_ref_count_bits(uint32_t v) const { data()->ref_count = v; }
  uint32_t size_address() const { return address + 8; }
  uint32_t size() const { return data()->size; }
  void set_size(uint32_t v) const { data()->size = v; }
  uint32_t size_bits() const { return data()->size; }
  void set_size_bits(uint32_t v) const { data()->size = v; }
  // the buffer's bytes (vtable slot 6's XMemAlloc)
  uint32_t data_address() const { return address + 12; }
  uint32_t data() const { return data()->data; }
  void set_data(uint32_t v) const { data()->data = v; }
  uint32_t data_bits() const { return data()->data; }
  void set_data_bits(uint32_t v) const { data()->data = v; }
};

// embedded; measure: D3DXCreateBuffer.r4
struct D3DXBufferOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = D3DXBuffer>
  T buffer() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_buffer(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_buffer(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t buffer_address() const { return address + 0; }
};

// embedded; measure: sub_825A67A8.r3, sub_825A67A8.r4, sub_820919B8.r3, sub_820915F8.r3
struct StdException : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 0x82000CC0 (std::exception's)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = ExceptionMessage>
  T what() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_what(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_what(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t what_address() const { return address + 4; }
  // probably: set when what was copied into a block of its own
  uint32_t owns_what() const { return guest::LoadU32(base, address + 8); }
  void set_owns_what(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t owns_what_address() const { return address + 8; }
};

// allocated; measure: sub_825A67A8.r4.+4
struct ExceptionMessage : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  uint8_t first_char() const { return guest::LoadU8(base, address + 0); }
  void set_first_char(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t first_char_address() const { return address + 0; }
};

// allocated; measure: sub_820901D8.r3, sub_82090268.r3, sub_820926C8.r3, sub_820927E8.r3, sub_82092830.r3, sub_82094B68.r3, sub_8209FB80.r3, sub_820A44C0.r3, sub_820A70B0.r3, sub_820AC950.r3, sub_820ACCF0.r3, sub_8210B0E8.r3, sub_8210B0E8.ret, sub_82126818.r3, sub_82133C18.r3, sub_82589480.r3, sub_8258DEB8.r3, sub_8258DEB8.ret, sub_82591898.r3, sub_825925A8.r3, sub_8259C8C8.r3, sub_8259C910.r3
#pragma pack(push, 1)
struct VtableObjectData {
  uint32_t vtable = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(VtableObjectData) == 4);
inline constexpr memory::LayoutField kVtableObjectFields[] = {{0, 4}};
inline constexpr memory::Layout kVtableObjectLayout = {kVtableObjectFields, 1};

struct VtableObject : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  VtableObjectData* data() const { return static_cast<VtableObjectData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static VtableObject New(uint8_t* base) {
    return VtableObject(base, memory::MakeHandle(new VtableObjectData(), sizeof(VtableObjectData), "VtableObject", &kVtableObjectLayout));
  }
  template <uint32_t Stride = kSize>
  static VtableObject NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(VtableObjectData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<VtableObjectData*>(::operator new(sizeof(VtableObjectData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) VtableObjectData();
    return VtableObject(base, memory::MakeHandle(p, sizeof(VtableObjectData) * count, "VtableObject", &kVtableObjectLayout, sizeof(VtableObjectData)));
  }
  VtableObject at(uint32_t i) const { return VtableObject(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kVtableObjectLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kVtableObjectLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return on_host() ? uint32_t(data()->vtable) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
};

// allocated; measure: sub_82090D60.r3@-4
struct DualVtableObject : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_4() const { return guest::LoadU32(base, address + 4); }
  void set_vtable_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vtable_4_address() const { return address + 4; }
};

// allocated; measure: sub_820A9010.r3
struct VectorHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t first() const { return guest::LoadU32(base, address + 4); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_address() const { return address + 4; }
  uint32_t last() const { return guest::LoadU32(base, address + 8); }
  void set_last(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t last_address() const { return address + 8; }
  uint32_t end() const { return guest::LoadU32(base, address + 12); }
  void set_end(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t end_address() const { return address + 12; }
};

// allocated; measure: sub_820A9208.r3
struct VectorHolderOwner : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr uint32_t kVtable = 0x82002780;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = VectorHolder>
  T holder() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_holder(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_holder(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t holder_address() const { return address + 4; }
};

// allocated; measure: new<16>@sub_820A81C0#2, FloatPairItem
#pragma pack(push, 1)
struct FloatPairItemData {
  uint32_t vtable = 0;  // +0
  uint32_t unknown_4 = 0;  // +4
  uint32_t argument = 0;  // +8
  uint32_t value = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(FloatPairItemData) == 16);
inline constexpr memory::LayoutField kFloatPairItemFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kFloatPairItemLayout = {kFloatPairItemFields, 4};

struct FloatPairItem : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82002518;
  FloatPairItemData* data() const { return static_cast<FloatPairItemData*>(memory::HandleObject(address)); }
  static FloatPairItem New(uint8_t* base) {
    return FloatPairItem(base, memory::MakeHandle(new FloatPairItemData(), sizeof(FloatPairItemData), "FloatPairItem", &kFloatPairItemLayout));
  }
  template <uint32_t Stride = kSize>
  static FloatPairItem NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(FloatPairItemData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FloatPairItemData*>(::operator new(sizeof(FloatPairItemData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FloatPairItemData();
    return FloatPairItem(base, memory::MakeHandle(p, sizeof(FloatPairItemData) * count, "FloatPairItem", &kFloatPairItemLayout, sizeof(FloatPairItemData)));
  }
  FloatPairItem at(uint32_t i) const { return FloatPairItem(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kFloatPairItemLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kFloatPairItemLayout, offset, width, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // 0 when made
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4() const { return data()->unknown_4; }
  void set_unknown_4(uint32_t v) const { data()->unknown_4 = v; }
  uint32_t unknown_4_bits() const { return data()->unknown_4; }
  void set_unknown_4_bits(uint32_t v) const { data()->unknown_4 = v; }
  // the maker's r4
  uint32_t argument_address() const { return address + 8; }
  uint32_t argument() const { return data()->argument; }
  void set_argument(uint32_t v) const { data()->argument = v; }
  uint32_t argument_bits() const { return data()->argument; }
  void set_argument_bits(uint32_t v) const { data()->argument = v; }
  uint32_t value_address() const { return address + 12; }
  template <typename T = FloatPair>
  T value() const { return T(base, data()->value); }
  void set_value(const View& w) const { const uint32_t v = w.address; data()->value = v; }
  void set_value(uint32_t v) const { data()->value = v; }
  uint32_t value_bits() const { return data()->value; }
  void set_value_bits(uint32_t v) const { data()->value = v; }
};

// allocated; measure: new<12>@FloatPairItem::FloatPairItem
#pragma pack(push, 1)
struct FloatPairData {
  uint8_t flag = 0;  // +0
  uint8_t pad_1[3] = {};
  float a = 0;  // +4
  float b = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(FloatPairData) == 12);
inline constexpr memory::LayoutField kFloatPairFields[] = {{4, 4}, {8, 4}};
inline constexpr memory::Layout kFloatPairLayout = {kFloatPairFields, 2};

struct FloatPair : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  FloatPairData* data() const { return static_cast<FloatPairData*>(memory::HandleObject(address)); }
  static FloatPair New(uint8_t* base) {
    return FloatPair(base, memory::MakeHandle(new FloatPairData(), sizeof(FloatPairData), "FloatPair", &kFloatPairLayout));
  }
  template <uint32_t Stride = kSize>
  static FloatPair NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(FloatPairData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FloatPairData*>(::operator new(sizeof(FloatPairData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FloatPairData();
    return FloatPair(base, memory::MakeHandle(p, sizeof(FloatPairData) * count, "FloatPair", &kFloatPairLayout, sizeof(FloatPairData)));
  }
  FloatPair at(uint32_t i) const { return FloatPair(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kFloatPairLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kFloatPairLayout, offset, width, v); }
  // 1 when made
  uint32_t flag_address() const { return address + 0; }
  uint8_t flag() const { return data()->flag; }
  void set_flag(uint8_t v) const { data()->flag = v; }
  uint8_t flag_bits() const { return data()->flag; }
  void set_flag_bits(uint8_t v) const { data()->flag = v; }
  // both set to the float at 0x82000A34
  uint32_t a_address() const { return address + 4; }
  float a() const { return data()->a; }
  void set_a(float v) const { data()->a = v; }
  uint32_t a_bits() const { return std::bit_cast<uint32_t>(data()->a); }
  void set_a_bits(uint32_t v) const { data()->a = std::bit_cast<float>(v); }
  uint32_t b_address() const { return address + 8; }
  float b() const { return data()->b; }
  void set_b(float v) const { data()->b = v; }
  uint32_t b_bits() const { return std::bit_cast<uint32_t>(data()->b); }
  void set_b_bits(uint32_t v) const { data()->b = std::bit_cast<float>(v); }
};

// allocated; measure: new<16>@sub_820A81C0
#pragma pack(push, 1)
struct FloatTripleItemData {
  uint32_t vtable = 0;  // +0
  uint32_t unknown_4 = 0;  // +4
  uint32_t argument = 0;  // +8
  uint32_t value = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(FloatTripleItemData) == 16);
inline constexpr memory::LayoutField kFloatTripleItemFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kFloatTripleItemLayout = {kFloatTripleItemFields, 4};

struct FloatTripleItem : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82002628;
  FloatTripleItemData* data() const { return static_cast<FloatTripleItemData*>(memory::HandleObject(address)); }
  static FloatTripleItem New(uint8_t* base) {
    return FloatTripleItem(base, memory::MakeHandle(new FloatTripleItemData(), sizeof(FloatTripleItemData), "FloatTripleItem", &kFloatTripleItemLayout));
  }
  template <uint32_t Stride = kSize>
  static FloatTripleItem NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(FloatTripleItemData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FloatTripleItemData*>(::operator new(sizeof(FloatTripleItemData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FloatTripleItemData();
    return FloatTripleItem(base, memory::MakeHandle(p, sizeof(FloatTripleItemData) * count, "FloatTripleItem", &kFloatTripleItemLayout, sizeof(FloatTripleItemData)));
  }
  FloatTripleItem at(uint32_t i) const { return FloatTripleItem(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kFloatTripleItemLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kFloatTripleItemLayout, offset, width, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // 0 when made
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4() const { return data()->unknown_4; }
  void set_unknown_4(uint32_t v) const { data()->unknown_4 = v; }
  uint32_t unknown_4_bits() const { return data()->unknown_4; }
  void set_unknown_4_bits(uint32_t v) const { data()->unknown_4 = v; }
  // the maker's r4
  uint32_t argument_address() const { return address + 8; }
  uint32_t argument() const { return data()->argument; }
  void set_argument(uint32_t v) const { data()->argument = v; }
  uint32_t argument_bits() const { return data()->argument; }
  void set_argument_bits(uint32_t v) const { data()->argument = v; }
  uint32_t value_address() const { return address + 12; }
  template <typename T = FloatTriple>
  T value() const { return T(base, data()->value); }
  void set_value(const View& w) const { const uint32_t v = w.address; data()->value = v; }
  void set_value(uint32_t v) const { data()->value = v; }
  uint32_t value_bits() const { return data()->value; }
  void set_value_bits(uint32_t v) const { data()->value = v; }
};

// allocated; measure: new<20>@sub_820A7AF8
#pragma pack(push, 1)
struct FloatTripleData {
  uint8_t flag = 0;  // +0
  uint8_t pad_1[3] = {};
  float a = 0;  // +4
  float b = 0;  // +8
  uint8_t flag_12 = 0;  // +12
  uint8_t pad_13[3] = {};
  float c = 0;  // +16
};
#pragma pack(pop)
static_assert(sizeof(FloatTripleData) == 20);
inline constexpr memory::LayoutField kFloatTripleFields[] = {{4, 4}, {8, 4}, {16, 4}};
inline constexpr memory::Layout kFloatTripleLayout = {kFloatTripleFields, 3};

struct FloatTriple : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  FloatTripleData* data() const { return static_cast<FloatTripleData*>(memory::HandleObject(address)); }
  static FloatTriple New(uint8_t* base) {
    return FloatTriple(base, memory::MakeHandle(new FloatTripleData(), sizeof(FloatTripleData), "FloatTriple", &kFloatTripleLayout));
  }
  template <uint32_t Stride = kSize>
  static FloatTriple NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(FloatTripleData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FloatTripleData*>(::operator new(sizeof(FloatTripleData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FloatTripleData();
    return FloatTriple(base, memory::MakeHandle(p, sizeof(FloatTripleData) * count, "FloatTriple", &kFloatTripleLayout, sizeof(FloatTripleData)));
  }
  FloatTriple at(uint32_t i) const { return FloatTriple(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kFloatTripleLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kFloatTripleLayout, offset, width, v); }
  // 1 when made
  uint32_t flag_address() const { return address + 0; }
  uint8_t flag() const { return data()->flag; }
  void set_flag(uint8_t v) const { data()->flag = v; }
  uint8_t flag_bits() const { return data()->flag; }
  void set_flag_bits(uint8_t v) const { data()->flag = v; }
  // a, b and c set to the float at 0x82000A34
  uint32_t a_address() const { return address + 4; }
  float a() const { return data()->a; }
  void set_a(float v) const { data()->a = v; }
  uint32_t a_bits() const { return std::bit_cast<uint32_t>(data()->a); }
  void set_a_bits(uint32_t v) const { data()->a = std::bit_cast<float>(v); }
  uint32_t b_address() const { return address + 8; }
  float b() const { return data()->b; }
  void set_b(float v) const { data()->b = v; }
  uint32_t b_bits() const { return std::bit_cast<uint32_t>(data()->b); }
  void set_b_bits(uint32_t v) const { data()->b = std::bit_cast<float>(v); }
  // 0 when made
  uint32_t flag_12_address() const { return address + 12; }
  uint8_t flag_12() const { return data()->flag_12; }
  void set_flag_12(uint8_t v) const { data()->flag_12 = v; }
  uint8_t flag_12_bits() const { return data()->flag_12; }
  void set_flag_12_bits(uint8_t v) const { data()->flag_12 = v; }
  uint32_t c_address() const { return address + 16; }
  float c() const { return data()->c; }
  void set_c(float v) const { data()->c = v; }
  uint32_t c_bits() const { return std::bit_cast<uint32_t>(data()->c); }
  void set_c_bits(uint32_t v) const { data()->c = std::bit_cast<float>(v); }
};

// allocated; measure: new<64>@sub_820A81C0
#pragma pack(push, 1)
struct Item820025D8Data {
  uint32_t vtable = 0;  // +0
  uint32_t unknown_4 = 0;  // +4
  uint32_t argument = 0;  // +8
  uint8_t pad_12[4] = {};
  uint64_t unknown_16 = 0;  // +16
  uint64_t unknown_24 = 0;  // +24
  uint64_t unknown_32 = 0;  // +32
  uint64_t unknown_40 = 0;  // +40
  float unknown_48 = 0;  // +48
  float unknown_52 = 0;  // +52
  uint8_t pad_56[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(Item820025D8Data) == 64);
inline constexpr memory::LayoutField kItem820025D8Fields[] = {{0, 4}, {4, 4}, {8, 4}, {16, 8}, {24, 8}, {32, 8}, {40, 8}, {48, 4}, {52, 4}};
inline constexpr memory::Layout kItem820025D8Layout = {kItem820025D8Fields, 9};

struct Item820025D8 : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x820025D8;
  Item820025D8Data* data() const { return static_cast<Item820025D8Data*>(memory::HandleObject(address)); }
  static Item820025D8 New(uint8_t* base) {
    return Item820025D8(base, memory::MakeHandle(new Item820025D8Data(), sizeof(Item820025D8Data), "Item820025D8", &kItem820025D8Layout));
  }
  template <uint32_t Stride = kSize>
  static Item820025D8 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(Item820025D8Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<Item820025D8Data*>(::operator new(sizeof(Item820025D8Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) Item820025D8Data();
    return Item820025D8(base, memory::MakeHandle(p, sizeof(Item820025D8Data) * count, "Item820025D8", &kItem820025D8Layout, sizeof(Item820025D8Data)));
  }
  Item820025D8 at(uint32_t i) const { return Item820025D8(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kItem820025D8Layout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kItem820025D8Layout, offset, width, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4() const { return data()->unknown_4; }
  void set_unknown_4(uint32_t v) const { data()->unknown_4 = v; }
  uint32_t unknown_4_bits() const { return data()->unknown_4; }
  void set_unknown_4_bits(uint32_t v) const { data()->unknown_4 = v; }
  // the maker's r4
  uint32_t argument_address() const { return address + 8; }
  uint32_t argument() const { return data()->argument; }
  void set_argument(uint32_t v) const { data()->argument = v; }
  uint32_t argument_bits() const { return data()->argument; }
  void set_argument_bits(uint32_t v) const { data()->argument = v; }
  uint32_t unknown_16_address() const { return address + 16; }
  uint64_t unknown_16() const { return data()->unknown_16; }
  void set_unknown_16(uint64_t v) const { data()->unknown_16 = v; }
  uint64_t unknown_16_bits() const { return data()->unknown_16; }
  void set_unknown_16_bits(uint64_t v) const { data()->unknown_16 = v; }
  uint32_t unknown_24_address() const { return address + 24; }
  uint64_t unknown_24() const { return data()->unknown_24; }
  void set_unknown_24(uint64_t v) const { data()->unknown_24 = v; }
  uint64_t unknown_24_bits() const { return data()->unknown_24; }
  void set_unknown_24_bits(uint64_t v) const { data()->unknown_24 = v; }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_28_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kItem820025D8Layout, 28, 4)); }
  void set_unknown_28_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kItem820025D8Layout, 28, 4, v); }
  float unknown_28() const { return std::bit_cast<float>(unknown_28_bits()); }
  void set_unknown_28(float v) const { set_unknown_28_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint64_t unknown_32() const { return data()->unknown_32; }
  void set_unknown_32(uint64_t v) const { data()->unknown_32 = v; }
  uint64_t unknown_32_bits() const { return data()->unknown_32; }
  void set_unknown_32_bits(uint64_t v) const { data()->unknown_32 = v; }
  uint32_t unknown_40_address() const { return address + 40; }
  uint64_t unknown_40() const { return data()->unknown_40; }
  void set_unknown_40(uint64_t v) const { data()->unknown_40 = v; }
  uint64_t unknown_40_bits() const { return data()->unknown_40; }
  void set_unknown_40_bits(uint64_t v) const { data()->unknown_40 = v; }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_44_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kItem820025D8Layout, 44, 4)); }
  void set_unknown_44_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kItem820025D8Layout, 44, 4, v); }
  float unknown_44() const { return std::bit_cast<float>(unknown_44_bits()); }
  void set_unknown_44(float v) const { set_unknown_44_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_48_address() const { return address + 48; }
  float unknown_48() const { return data()->unknown_48; }
  void set_unknown_48(float v) const { data()->unknown_48 = v; }
  uint32_t unknown_48_bits() const { return std::bit_cast<uint32_t>(data()->unknown_48); }
  void set_unknown_48_bits(uint32_t v) const { data()->unknown_48 = std::bit_cast<float>(v); }
  uint32_t unknown_52_address() const { return address + 52; }
  float unknown_52() const { return data()->unknown_52; }
  void set_unknown_52(float v) const { data()->unknown_52 = v; }
  uint32_t unknown_52_bits() const { return std::bit_cast<uint32_t>(data()->unknown_52); }
  void set_unknown_52_bits(uint32_t v) const { data()->unknown_52 = std::bit_cast<float>(v); }
};

// allocated; measure: sub_825788C0.r3, new<8>@sub_825788C0
#pragma pack(push, 1)
struct ByteCountedListData {
  uint8_t count = 0;  // +0
  uint8_t pad_1[3] = {};
  uint32_t entries = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(ByteCountedListData) == 8);
inline constexpr memory::LayoutField kByteCountedListFields[] = {{4, 4}};
inline constexpr memory::Layout kByteCountedListLayout = {kByteCountedListFields, 1};

struct ByteCountedList : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ByteCountedListData* data() const { return static_cast<ByteCountedListData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ByteCountedList New(uint8_t* base) {
    return ByteCountedList(base, memory::MakeHandle(new ByteCountedListData(), sizeof(ByteCountedListData), "ByteCountedList", &kByteCountedListLayout));
  }
  template <uint32_t Stride = kSize>
  static ByteCountedList NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ByteCountedListData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ByteCountedListData*>(::operator new(sizeof(ByteCountedListData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ByteCountedListData();
    return ByteCountedList(base, memory::MakeHandle(p, sizeof(ByteCountedListData) * count, "ByteCountedList", &kByteCountedListLayout, sizeof(ByteCountedListData)));
  }
  ByteCountedList at(uint32_t i) const { return ByteCountedList(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kByteCountedListLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kByteCountedListLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t count_address() const { return address + 0; }
  uint8_t count() const { return on_host() ? uint8_t(data()->count) : uint8_t(guest::LoadU8(base, address + 0)); }
  void set_count(uint8_t v) const { if (on_host()) data()->count = v; else guest::StoreU8(base, address + 0, v); }
  uint8_t count_bits() const { return on_host() ? data()->count : guest::LoadU8(base, address + 0); }
  void set_count_bits(uint8_t v) const { if (on_host()) data()->count = v; else guest::StoreU8(base, address + 0, v); }
  // count 12-byte entries, compared by their first byte
  uint32_t entries_address() const { return address + 4; }
  uint32_t entries() const { return on_host() ? uint32_t(data()->entries) : uint32_t(guest::LoadU32(base, address + 4)); }
  void set_entries(uint32_t v) const { if (on_host()) data()->entries = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t entries_bits() const { return on_host() ? data()->entries : guest::LoadU32(base, address + 4); }
  void set_entries_bits(uint32_t v) const { if (on_host()) data()->entries = v; else guest::StoreU32(base, address + 4, v); }
};

// embedded; measure: sub_82578AF0.r5
struct ByteCountedListArgs : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  template <typename T = ByteCountedList>
  T list() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_list(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_list(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t list_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// The image words docs/functions/heap.md's code uses.
struct HeapGlobals {
  // GetProcessHeap's word, made at boot by sub_82297C08
  template <typename T = ProcessHeap>
  static T process_heap(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8297149Cu)); }
  static void set_process_heap(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8297149Cu, v.address); }
  static void set_process_heap(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8297149Cu, v); }
  static constexpr uint32_t process_heap_address() { return 0x8297149Cu; }
  // when not 0, XPhysicalAlloc and XPhysicalAllocEx clear X_MEM_LARGE_PAGES from the protection
  static uint32_t no_large_physical_pages(uint8_t* base) { return guest::LoadU32(base, 0x829537B0u); }
  static void set_no_large_physical_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x829537B0u, v); }
  static constexpr uint32_t no_large_physical_pages_address() { return 0x829537B0u; }
  // XMemAlloc's protections for physical memory: four words, by (attributes >> 28) & 3
  static uint32_t physical_protections(uint8_t* base) { return guest::LoadU32(base, 0x82638770u); }
  static void set_physical_protections(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82638770u, v); }
  static constexpr uint32_t physical_protections_address() { return 0x82638770u; }
  // the new handler _callnewh calls
  static uint32_t new_handler(uint8_t* base) { return guest::LoadU32(base, 0x82971170u); }
  static void set_new_handler(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82971170u, v); }
  static constexpr uint32_t new_handler_address() { return 0x82971170u; }
  // probably _newmode: malloc retries through _callnewh when set
  static uint32_t new_mode(uint8_t* base) { return guest::LoadU32(base, 0x8297119Cu); }
  static void set_new_mode(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8297119Cu, v); }
  static constexpr uint32_t new_mode_address() { return 0x8297119Cu; }
};

}  // namespace schema
