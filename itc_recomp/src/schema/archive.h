// The views of docs/functions/archive.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/archive.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// file; measure: List.+12, List.+12.+4, List.+16, List.+16.+4
struct ArchiveRequestSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 92;
  static constexpr uint32_t kVtable = 0x82001DD8;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the next queued slot
  template <typename T = ArchiveRequestSlot>
  T link() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_link(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_link(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t link_address() const { return address + 4; }
  // the request's serial; -1 when the slot is free
  uint32_t serial() const { return guest::LoadU32(base, address + 8); }
  void set_serial(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t serial_address() const { return address + 8; }
  // 0x4, 0x20, 0x4000 (cancellable) always; 0x80 with Archive::Load's r7, 0x100 compressed, 0x200 a loose file or 0x800 from an archive, 0x2000 a callback; 0x1, 0x8, 0x10, 0x40, 0x400, 0x1000, 0x2 tested by the loader
  uint32_t flags() const { return guest::LoadU32(base, address + 12); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t flags_address() const { return address + 12; }
  // the asset id (from 23011 in BUILD_CRS.DAT)
  uint32_t id() const { return guest::LoadU32(base, address + 16); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t id_address() const { return address + 16; }
  // the loose file's path, 0 for none
  template <typename T = CString>
  T loose_path() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_loose_path(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_loose_path(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t loose_path_address() const { return address + 20; }
  // the TOC's start sector x 2,048
  uint32_t byte_offset() const { return guest::LoadU32(base, address + 24); }
  void set_byte_offset(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t byte_offset_address() const { return address + 24; }
  // Archive::Load's r7 (flag 0x80)
  uint32_t load_r7() const { return guest::LoadU32(base, address + 28); }
  void set_load_r7(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t load_r7_address() const { return address + 28; }
  // the unpacked size when compressed, else Archive::Load's r8 or the stored size
  uint32_t size() const { return guest::LoadU32(base, address + 32); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t size_address() const { return address + 32; }
  // the stored size when compressed, else 0
  uint32_t stored_size() const { return guest::LoadU32(base, address + 36); }
  void set_stored_size(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t stored_size_address() const { return address + 36; }
  // the caller's buffer (with flag 0x20 and 0x1, a word the fresh allocation is written to)
  uint32_t destination() const { return guest::LoadU32(base, address + 40); }
  void set_destination(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t destination_address() const { return address + 40; }
  // the callback's context
  uint32_t context() const { return guest::LoadU32(base, address + 44); }
  void set_context(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t context_address() const { return address + 44; }
  // called by the finish as callback(serial, data, size, context)
  uint32_t callback() const { return guest::LoadU32(base, address + 48); }
  void set_callback(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t callback_address() const { return address + 48; }
  // 0 queued, 1 next, 2 chained, 3 to 8 by kind (5 an archive read, 7 a loose read)
  uint32_t state() const { return guest::LoadU32(base, address + 52); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t state_address() const { return address + 52; }
  // the byte offset read from (+24, flag 0x800)
  uint32_t read_offset() const { return guest::LoadU32(base, address + 56); }
  void set_read_offset(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t read_offset_address() const { return address + 56; }
  // the 2,048-byte sectors read
  uint32_t sectors() const { return guest::LoadU32(base, address + 60); }
  void set_sectors(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t sectors_address() const { return address + 60; }
  // +28 copied (flag 0x80); the loose read passes it on
  uint32_t read_r7() const { return guest::LoadU32(base, address + 64); }
  void set_read_r7(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t read_r7_address() const { return address + 64; }
  uint32_t read_stored_size() const { return guest::LoadU32(base, address + 68); }
  void set_read_stored_size(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t read_stored_size_address() const { return address + 68; }
  uint32_t read_unpacked_size() const { return guest::LoadU32(base, address + 72); }
  void set_read_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t read_unpacked_size_address() const { return address + 72; }
  // where it's read: the destination, an allocation or the staging ring's half
  uint32_t data() const { return guest::LoadU32(base, address + 76); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t data_address() const { return address + 76; }
  uint32_t unknown_80() const { return guest::LoadU32(base, address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  // the compressed request kept to unpack (itself when no next)
  template <typename T = ArchiveRequestSlot>
  T chained() const { return T(base, guest::LoadU32(base, address + 84)); }
  void set_chained(const View& v) const { guest::StoreU32(base, address + 84, v.address); }
  void set_chained(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t chained_address() const { return address + 84; }
  // a compressed request's destination, unpacked into from +76
  uint32_t unpack_destination() const { return guest::LoadU32(base, address + 88); }
  void set_unpack_destination(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t unpack_destination_address() const { return address + 88; }
};

// file; measure: -
struct ArchiveList : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  static constexpr uint32_t kVtable = 0x820128C8;
  // 0x820128C8; the queue's 0x82001DD4
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0 for the queue
  uint32_t deletes_nodes() const { return guest::LoadU32(base, address + 4); }
  void set_deletes_nodes(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t deletes_nodes_address() const { return address + 4; }
  uint32_t count() const { return guest::LoadU32(base, address + 8); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t count_address() const { return address + 8; }
  // the nodes are request slots, linked by +4
  template <typename T = ArchiveRequestSlot>
  T head() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_head(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t head_address() const { return address + 12; }
  template <typename T = ArchiveRequestSlot>
  T tail() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_tail(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_tail(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t tail_address() const { return address + 16; }
};

// file; measure: ListNode
struct ArchiveListNode : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr uint32_t kVtable = 0x82010B5C;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t link() const { return guest::LoadU32(base, address + 4); }
  void set_link(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t link_address() const { return address + 4; }
};

// file; measure: -
struct ArchiveTocEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // 2,048-byte sectors
  uint32_t start_sector() const { return guest::LoadU32(base, address + 0); }
  void set_start_sector(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t start_sector_address() const { return address + 0; }
  uint32_t stored_size() const { return guest::LoadU32(base, address + 4); }
  void set_stored_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t stored_size_address() const { return address + 4; }
  // 0 when stored as is
  uint32_t unpacked_size() const { return guest::LoadU32(base, address + 8); }
  void set_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unpacked_size_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// file; measure: -
struct ArchiveLooseFile : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t id() const { return guest::LoadU32(base, address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
  template <typename T = CString>
  T path() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_path(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_path(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t path_address() const { return address + 4; }
};

// file; measure: LoaderFunctor, new<8>@LoaderFunctor::Clone
struct LoaderFunctor : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr uint32_t kVtable = 0x82001DDC;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the function Invoke calls (0x8209E118, the loader thread)
  uint32_t target() const { return guest::LoadU32(base, address + 4); }
  void set_target(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t target_address() const { return address + 4; }
};

// file; measure: sub_82094D20.r3
struct ArchiveFunctorBase : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr uint32_t kVtable = 0x82000ABC;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// file; measure: sub_8258D508.r3
struct ArchiveVideoOutputDeleting : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr uint32_t kVtable = 0x8206CAF8;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// file; measure: sub_8258DD60.r3
struct ArchiveClosedEvent : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // -1 when none
  uint32_t handle() const { return guest::LoadU32(base, address + 0); }
  void set_handle(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t handle_address() const { return address + 0; }
};

// The image words docs/functions/archive.md's code uses.
struct ArchiveGlobals {
  // BUILD.DAT's and BUILD_CRS.DAT's file handles (-1 when not open)
  static constexpr uint32_t archive_handles_address() { return 0x82600A9Cu; }
  // the serial of the request whose callback runs last (-1 at Archive::Init)
  static uint32_t callback_serial(uint8_t* base) { return guest::LoadU32(base, 0x82600AA4u); }
  static void set_callback_serial(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82600AA4u, v); }
  static constexpr uint32_t callback_serial_address() { return 0x82600AA4u; }
  // 23011
  static uint32_t build_toc_count(uint8_t* base) { return guest::LoadU32(base, 0x82600AA8u); }
  static void set_build_toc_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82600AA8u, v); }
  static constexpr uint32_t build_toc_count_address() { return 0x82600AA8u; }
  // 1359
  static uint32_t crs_toc_count(uint8_t* base) { return guest::LoadU32(base, 0x82600AB8u); }
  static void set_crs_toc_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82600AB8u, v); }
  static constexpr uint32_t crs_toc_count_address() { return 0x82600AB8u; }
  // the first ArchiveLooseFile (the walk's first 23,011 steps)
  static uint32_t loose_files(uint8_t* base) { return guest::LoadU32(base, 0x82600AC8u); }
  static void set_loose_files(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82600AC8u, v); }
  static constexpr uint32_t loose_files_address() { return 0x82600AC8u; }
  // the loader thread's parameters: +8, +12 = 0, +16 = 0, +17 = 1, +18 = 0, +20 a word, +24 a LoaderFunctor, +28 the entry 0x8209E118
  static constexpr uint32_t loader_thread_address() { return 0x82600B50u; }
  // a LoaderFunctor
  static constexpr uint32_t loader_functor_address() { return 0x82600B68u; }
  // the first ArchiveLooseFile after the walk's first 23,011 steps
  static uint32_t crs_loose_files(uint8_t* base) { return guest::LoadU32(base, 0x82678240u); }
  static void set_crs_loose_files(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82678240u, v); }
  static constexpr uint32_t crs_loose_files_address() { return 0x82678240u; }
  // the archive reads' XOVERLAPPED
  static constexpr uint32_t overlapped_address() { return 0x826A50ECu; }
  // the streaming object (Archive::Init's r3)
  static View streaming(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826A5100u)); }
  static void set_streaming(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5100u, v); }
  static constexpr uint32_t streaming_address() { return 0x826A5100u; }
  // the next request's serial (wraps at 0x7FFFFFFF)
  static uint32_t serial(uint8_t* base) { return guest::LoadU32(base, 0x826A5104u); }
  static void set_serial(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5104u, v); }
  static constexpr uint32_t serial_address() { return 0x826A5104u; }
  // 1 while Archive::WaitIdle waits for the queue to drain
  static uint32_t idle_wanted(uint8_t* base) { return guest::LoadU32(base, 0x826A5108u); }
  static void set_idle_wanted(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5108u, v); }
  static constexpr uint32_t idle_wanted_address() { return 0x826A5108u; }
  // half the staging ring (5 MiB)
  static uint32_t staging_half_size(uint8_t* base) { return guest::LoadU32(base, 0x826A510Cu); }
  static void set_staging_half_size(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A510Cu, v); }
  static constexpr uint32_t staging_half_size_address() { return 0x826A510Cu; }
  // alternates the staging ring's halves
  static uint32_t staging_count(uint8_t* base) { return guest::LoadU32(base, 0x826A5110u); }
  static void set_staging_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5110u, v); }
  static constexpr uint32_t staging_count_address() { return 0x826A5110u; }
  // the staging ring's first half (the second at 0x826A5118)
  static uint32_t staging_halves(uint8_t* base) { return guest::LoadU32(base, 0x826A5114u); }
  static void set_staging_halves(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5114u, v); }
  static constexpr uint32_t staging_halves_address() { return 0x826A5114u; }
  static uint32_t staging_half_1(uint8_t* base) { return guest::LoadU32(base, 0x826A5118u); }
  static void set_staging_half_1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5118u, v); }
  static constexpr uint32_t staging_half_1_address() { return 0x826A5118u; }
  // the staging half a compressed request was read into
  static uint32_t staging_in_use(uint8_t* base) { return guest::LoadU32(base, 0x826A511Cu); }
  static void set_staging_in_use(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A511Cu, v); }
  static constexpr uint32_t staging_in_use_address() { return 0x826A511Cu; }
  // BUILD.TOC's buffer (its header, then the entries)
  template <typename T = ArchiveTocEntry>
  static T build_toc(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826A5120u)); }
  static void set_build_toc(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826A5120u, v.address); }
  static void set_build_toc(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5120u, v); }
  static constexpr uint32_t build_toc_address() { return 0x826A5120u; }
  static uint32_t build_toc_size(uint8_t* base) { return guest::LoadU32(base, 0x826A5124u); }
  static void set_build_toc_size(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5124u, v); }
  static constexpr uint32_t build_toc_size_address() { return 0x826A5124u; }
  // BUILD_CRS.TOC's buffer
  template <typename T = ArchiveTocEntry>
  static T crs_toc(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826A5128u)); }
  static void set_crs_toc(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826A5128u, v.address); }
  static void set_crs_toc(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5128u, v); }
  static constexpr uint32_t crs_toc_address() { return 0x826A5128u; }
  static uint32_t crs_toc_size(uint8_t* base) { return guest::LoadU32(base, 0x826A512Cu); }
  static void set_crs_toc_size(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A512Cu, v); }
  static constexpr uint32_t crs_toc_size_address() { return 0x826A512Cu; }
  // the shared load buffer's size (12.5 MiB)
  static uint32_t load_buffer_size(uint8_t* base) { return guest::LoadU32(base, 0x826A5130u); }
  static void set_load_buffer_size(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5130u, v); }
  static constexpr uint32_t load_buffer_size_address() { return 0x826A5130u; }
  // the shared load buffer (0 when its size is 0)
  static uint32_t load_buffer(uint8_t* base) { return guest::LoadU32(base, 0x826A5134u); }
  static void set_load_buffer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5134u, v); }
  static constexpr uint32_t load_buffer_address() { return 0x826A5134u; }
  // the 960 ArchiveRequestSlot (92 bytes each)
  static constexpr uint32_t slots_address() { return 0x826A5168u; }
  // a Semaphore: a request done
  static constexpr uint32_t request_done_semaphore_address() { return 0x826BAA68u; }
  // a Semaphore: the queue drained
  static constexpr uint32_t idle_semaphore_address() { return 0x826BAA90u; }
  // a Semaphore: the loader's work
  static constexpr uint32_t work_semaphore_address() { return 0x826BAAB8u; }
  // a Semaphore: the queue lock (count 1)
  static constexpr uint32_t queue_lock_address() { return 0x826BAAE0u; }
  // an ArchiveList: the request queue (vtable 0x82001DD4)
  static constexpr uint32_t queue_address() { return 0x826BAB08u; }
};

}  // namespace schema
