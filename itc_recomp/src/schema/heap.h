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

// allocated; measure: HeapFree.r5, HeapSize.r5, new<?>@HeapReAlloc, [8297149C].+384.+0.+4
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
struct D3DXBuffer : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr uint32_t kVtable = 0x82019DC0;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t ref_count() const { return guest::LoadU32(base, address + 4); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t ref_count_address() const { return address + 4; }
  uint32_t size() const { return guest::LoadU32(base, address + 8); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t size_address() const { return address + 8; }
  // the buffer's bytes (vtable slot 6's XMemAlloc)
  uint32_t data() const { return guest::LoadU32(base, address + 12); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t data_address() const { return address + 12; }
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
struct VtableObject : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// allocated; measure: sub_82090D68.ret
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

// allocated; measure: new<16>@sub_820A81C0#2
struct FloatPairItem : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr uint32_t kVtable = 0x82002518;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0 when made
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // the maker's r4
  uint32_t argument() const { return guest::LoadU32(base, address + 8); }
  void set_argument(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t argument_address() const { return address + 8; }
  template <typename T = FloatPair>
  T value() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_value(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t value_address() const { return address + 12; }
};

// allocated; measure: new<12>@sub_820A70F8
struct FloatPair : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 1 when made
  uint8_t flag() const { return guest::LoadU8(base, address + 0); }
  void set_flag(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t flag_address() const { return address + 0; }
  // both set to the float at 0x82000A34
  float a() const { return guest::LoadFloat(base, address + 4); }
  void set_a(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t a_address() const { return address + 4; }
  float b() const { return guest::LoadFloat(base, address + 8); }
  void set_b(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t b_address() const { return address + 8; }
};

// allocated; measure: new<16>@sub_820A81C0
struct FloatTripleItem : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr uint32_t kVtable = 0x82002628;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0 when made
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // the maker's r4
  uint32_t argument() const { return guest::LoadU32(base, address + 8); }
  void set_argument(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t argument_address() const { return address + 8; }
  template <typename T = FloatTriple>
  T value() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_value(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t value_address() const { return address + 12; }
};

// allocated; measure: new<20>@sub_820A7AF8
struct FloatTriple : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // 1 when made
  uint8_t flag() const { return guest::LoadU8(base, address + 0); }
  void set_flag(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t flag_address() const { return address + 0; }
  // a, b and c set to the float at 0x82000A34
  float a() const { return guest::LoadFloat(base, address + 4); }
  void set_a(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t a_address() const { return address + 4; }
  float b() const { return guest::LoadFloat(base, address + 8); }
  void set_b(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t b_address() const { return address + 8; }
  // 0 when made
  uint8_t flag_12() const { return guest::LoadU8(base, address + 12); }
  void set_flag_12(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t flag_12_address() const { return address + 12; }
  float c() const { return guest::LoadFloat(base, address + 16); }
  void set_c(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t c_address() const { return address + 16; }
};

// allocated; measure: new<64>@sub_820A81C0
struct Item820025D8 : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  static constexpr uint32_t kVtable = 0x820025D8;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // the maker's r4
  uint32_t argument() const { return guest::LoadU32(base, address + 8); }
  void set_argument(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t argument_address() const { return address + 8; }
  uint64_t unknown_16() const { return guest::LoadU64(base, address + 16); }
  void set_unknown_16(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint64_t unknown_24() const { return guest::LoadU64(base, address + 24); }
  void set_unknown_24(uint64_t v) const { guest::StoreU64(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  float unknown_28() const { return guest::LoadFloat(base, address + 28); }
  void set_unknown_28(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint64_t unknown_32() const { return guest::LoadU64(base, address + 32); }
  void set_unknown_32(uint64_t v) const { guest::StoreU64(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint64_t unknown_40() const { return guest::LoadU64(base, address + 40); }
  void set_unknown_40(uint64_t v) const { guest::StoreU64(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  float unknown_44() const { return guest::LoadFloat(base, address + 44); }
  void set_unknown_44(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  float unknown_48() const { return guest::LoadFloat(base, address + 48); }
  void set_unknown_48(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  float unknown_52() const { return guest::LoadFloat(base, address + 52); }
  void set_unknown_52(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
};

// allocated; measure: sub_825788C0.r3, new<8>@sub_825788C0
struct ByteCountedList : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint8_t count() const { return guest::LoadU8(base, address + 0); }
  void set_count(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // count 12-byte entries, compared by their first byte
  uint32_t entries() const { return guest::LoadU32(base, address + 4); }
  void set_entries(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t entries_address() const { return address + 4; }
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
