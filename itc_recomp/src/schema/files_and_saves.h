// The views of docs/functions/files-and-saves.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/files-and-saves.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// file; measure: File, sub_82593B10.r3, sub_8259D1F0.r4
struct File : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr uint32_t kVtable = 0x8206C05C;
  // 0x8206C05C; the base stream's 0x8206BFF8 (the destructors)
  template <typename T = FileVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // -1 when closed
  uint32_t handle() const { return guest::LoadU32(base, address + 4); }
  void set_handle(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t handle_address() const { return address + 4; }
  // moved by Read and Seek
  uint32_t position() const { return guest::LoadU32(base, address + 8); }
  void set_position(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t position_address() const { return address + 8; }
  // GetFileSize's answer at the open (File::GetSize returns it)
  uint32_t size() const { return guest::LoadU32(base, address + 12); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t size_address() const { return address + 12; }
};

// image; measure: sub_8259D1F0.r4.+0
struct FileVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // File::Read (sub_8258C820)
  uint32_t read() const { return guest::LoadU32(base, address + 8); }
  void set_read(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t read_address() const { return address + 8; }
  // File::GetSize (sub_822B80D8), probably
  uint32_t get_size() const { return guest::LoadU32(base, address + 20); }
  void set_get_size(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t get_size_address() const { return address + 20; }
};

// allocated; measure: -
struct XactSettingsFile : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  static XactSettingsFile New(uint8_t* base, uint32_t size) { return XactSettingsFile(base, memory::NewBlob(size, "XactSettingsFile")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
};

// embedded; measure: File::Write.r5, sub_8209DFA8.r3@+68
struct FileSizeArg : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t size() const { return guest::LoadU32(base, address + 0); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t size_address() const { return address + 0; }
};

// embedded; measure: File::Seek.r4
struct FileSeekOffsetArg : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  int32_t offset() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_offset(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t offset_address() const { return address + 0; }
};

// embedded; measure: File::Seek.r5
struct FileSeekMethodArg : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t method() const { return guest::LoadU32(base, address + 0); }
  void set_method(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t method_address() const { return address + 0; }
};

// embedded; measure: sub_82593B10.r5
struct FileOpenFlag : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  // nonzero: open existing (3); 0: open always (4)
  uint8_t must_exist() const { return guest::LoadU8(base, address + 0); }
  void set_must_exist(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t must_exist_address() const { return address + 0; }
};

// embedded; measure: GetOverlappedResult.r5
struct OverlappedBytesOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t bytes() const { return guest::LoadU32(base, address + 0); }
  void set_bytes(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t bytes_address() const { return address + 0; }
};

// embedded; measure: GetFileSize.r4
struct FileSizeHighOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t high() const { return guest::LoadU32(base, address + 0); }
  void set_high(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t high_address() const { return address + 0; }
};

// embedded; measure: SetFilePointer.r5
struct FilePointerHigh : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t high() const { return guest::LoadU32(base, address + 0); }
  void set_high(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t high_address() const { return address + 0; }
};

// embedded; measure: InitNamedObjectAttributes.r3
struct KernelObjectAttributes : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // -4 for the named objects' root
  uint32_t root_directory() const { return guest::LoadU32(base, address + 0); }
  void set_root_directory(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t root_directory_address() const { return address + 0; }
  // an ANSI_STRING
  uint32_t name() const { return guest::LoadU32(base, address + 4); }
  void set_name(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t name_address() const { return address + 4; }
  uint32_t attributes() const { return guest::LoadU32(base, address + 8); }
  void set_attributes(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t attributes_address() const { return address + 8; }
};

// embedded; measure: _write_nolock.r4
struct WriteNolockBuffer : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  uint8_t first_byte() const { return guest::LoadU8(base, address + 0); }
  void set_first_byte(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t first_byte_address() const { return address + 0; }
};

// embedded; measure: Ucl::Unpack.r4
struct UclStream : View {
  using View::View;
  static constexpr uint32_t kSize = 22;
  // the first block's unpacked size (0: no blocks); its stored size follows at +22
  uint32_t first_block_size() const { return guest::LoadU32(base, address + 18); }
  void set_first_block_size(uint32_t v) const { guest::StoreU32(base, address + 18, v); }
  uint32_t first_block_size_address() const { return address + 18; }
};

// embedded; measure: Ucl::Unpack.r4@+26
struct UclBlockSource : View {
  using View::View;
  static constexpr uint32_t kSize = 30;
  // the stream's bytes (the measure saw +1 to +29)
  uint32_t bytes_address() const { return address + 0; }
};

// embedded; measure: Ucl::UnpackBlock.r6
struct UclLengthOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t length() const { return guest::LoadU32(base, address + 0); }
  void set_length(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t length_address() const { return address + 0; }
};

// file; measure: DataFile
struct DataFile : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // little-endian (byte-swapped by the reader)
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // the count's first byte, loaded on its own by the original's byte swap
  uint8_t count_byte0() const { return guest::LoadU8(base, address + 0); }
  void set_count_byte0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t count_byte0_address() const { return address + 0; }
  // the first of `count` little-endian offsets (entry i at +4 + 4i)
  uint32_t offsets() const { return guest::LoadU32(base, address + 4); }
  void set_offsets(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t offsets_address() const { return address + 4; }
};

// allocated; measure: MovieSource::AtEnd.r3
struct MovieSource : View {
  using View::View;
  static constexpr uint32_t kSize = 136;
  static constexpr uint32_t kVtable = 0x8204DD50;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // -1 when closed
  uint32_t handle() const { return guest::LoadU32(base, address + 44); }
  void set_handle(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t handle_address() const { return address + 44; }
  // 1 at the end or after a short read (AtEnd returns it)
  uint32_t at_end() const { return guest::LoadU32(base, address + 48); }
  void set_at_end(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t at_end_address() const { return address + 48; }
  // where the data starts in the file
  uint64_t start() const { return guest::LoadU64(base, address + 56); }
  void set_start(uint64_t v) const { guest::StoreU64(base, address + 56, v); }
  uint32_t start_address() const { return address + 56; }
  uint64_t size() const { return guest::LoadU64(base, address + 64); }
  void set_size(uint64_t v) const { guest::StoreU64(base, address + 64, v); }
  uint32_t size_address() const { return address + 64; }
  // the read-ahead window (Prime, Cancel), probably
  uint32_t window() const { return guest::LoadU32(base, address + 72); }
  void set_window(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t window_address() const { return address + 72; }
  // cleared by Cancel
  uint32_t unknown_80() const { return guest::LoadU32(base, address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  uint64_t position() const { return guest::LoadU64(base, address + 88); }
  void set_position(uint64_t v) const { guest::StoreU64(base, address + 88, v); }
  uint32_t position_address() const { return address + 88; }
  uint32_t block_size() const { return guest::LoadU32(base, address + 104); }
  void set_block_size(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t block_size_address() const { return address + 104; }
  uint32_t block_count() const { return guest::LoadU32(base, address + 108); }
  void set_block_count(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t block_count_address() const { return address + 108; }
  // cleared by Cancel
  uint32_t unknown_112() const { return guest::LoadU32(base, address + 112); }
  void set_unknown_112(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  // 1 for 0
  uint32_t blocks_ahead() const { return guest::LoadU32(base, address + 116); }
  void set_blocks_ahead(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t blocks_ahead_address() const { return address + 116; }
  // 1 a path, 2 a wide path, 4 the caller's handle (not closed by Close), 8 looping
  uint32_t flags() const { return guest::LoadU32(base, address + 120); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t flags_address() const { return address + 120; }
  // the blocks' memory when allocated by the set-up
  uint32_t blocks_memory() const { return guest::LoadU32(base, address + 124); }
  void set_blocks_memory(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t blocks_memory_address() const { return address + 124; }
  // cleared by Cancel
  uint32_t unknown_128() const { return guest::LoadU32(base, address + 128); }
  void set_unknown_128(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  uint32_t block_list() const { return guest::LoadU32(base, address + 132); }
  void set_block_list(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t block_list_address() const { return address + 132; }
};

// embedded; measure: SaveInterface::SaveAt.ret
struct XContentData : View {
  using View::View;
  static constexpr uint32_t kSize = 308;
  uint32_t device() const { return guest::LoadU32(base, address + 0); }
  void set_device(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t device_address() const { return address + 0; }
  // 1 a saved game
  uint32_t content_type() const { return guest::LoadU32(base, address + 4); }
  void set_content_type(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t content_type_address() const { return address + 4; }
  // UTF-16, 128 characters
  uint32_t display_name_address() const { return address + 8; }
  uint32_t file_name_address() const { return address + 264; }
};

// allocated; measure: SaveSlot::WaitEnumerate.r4, SaveSlot::Create.r4, SaveSlot::Delete.r4, SaveSlot::LoadStep.r4, SaveSlot::OpenItem.r4, SaveSlot::SaveStep.r4, SaveSlot::ShowDeviceSelector.r4, SaveSlot::WaitDeviceSelector.r4, sub_820A4508.r3, sub_820A5660.r3, SaveSlot::CheckStep.r4, SaveSlot::DeleteStep.r4, SaveSlot::EnumerateStep.r4, SaveSlot::Failed.r4, SaveSlot::Ready.r4, SaveSlot::WaitClose.r4, SaveSlot::WaitForDevice.r4, SaveSlot::WaitForUser.r4, SaveSlot::WaitWrite.r4, SaveSlot::WriteFile.r4@+500, SaveSlot::WaitRead.r4, SaveSlot::ReadFile.r4@+500, SaveSlot::ReadFile.r4@+472, SaveDevices@+44.+0
struct SaveSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 1000;
  static constexpr uint32_t kVtable = 0x82002190;
  // the operator new[] block's count, 8 bytes before the first slot
  uint32_t array_count() const { return guest::LoadU32(base, address + -8); }
  void set_array_count(uint32_t v) const { guest::StoreU32(base, address + -8, v); }
  uint32_t array_count_address() const { return address + -8; }
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // reached only by ReadFile's overlapped reads, which the measure joins to the slot (a measure fault)
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // the device selector's XOVERLAPPED
  uint32_t device_selector_address() const { return address + 8; }
  // 0 for none
  uint32_t device() const { return guest::LoadU32(base, address + 36); }
  void set_device(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t device_address() const { return address + 36; }
  uint64_t device_free_bytes() const { return guest::LoadU64(base, address + 40); }
  void set_device_free_bytes(uint64_t v) const { guest::StoreU64(base, address + 40, v); }
  uint32_t device_free_bytes_address() const { return address + 40; }
  // the enumeration's XOVERLAPPED
  uint32_t enumeration_address() const { return address + 48; }
  // the enumerator's handle
  uint32_t enumerator() const { return guest::LoadU32(base, address + 76); }
  void set_enumerator(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t enumerator_address() const { return address + 76; }
  // XContentData each (308 bytes); freed by the destructor
  uint32_t enumeration_buffer() const { return guest::LoadU32(base, address + 80); }
  void set_enumeration_buffer(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t enumeration_buffer_address() const { return address + 80; }
  // a word array (the owner's +68 words, SaveSlot::AllocBuffers): 1 for each enumerated save that couldn't be opened; freed by the destructor
  uint32_t open_failed() const { return guest::LoadU32(base, address + 84); }
  void set_open_failed(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t open_failed_address() const { return address + 84; }
  uint32_t enumeration_buffer_size() const { return guest::LoadU32(base, address + 88); }
  void set_enumeration_buffer_size(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t enumeration_buffer_size_address() const { return address + 88; }
  // XGetOverlappedResult's result
  uint32_t enumerated_count() const { return guest::LoadU32(base, address + 92); }
  void set_enumerated_count(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t enumerated_count_address() const { return address + 92; }
  uint32_t enumeration_step() const { return guest::LoadU32(base, address + 96); }
  void set_enumeration_step(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t enumeration_step_address() const { return address + 96; }
  // the open's XOVERLAPPED
  uint32_t open_address() const { return address + 100; }
  uint32_t check_step() const { return guest::LoadU32(base, address + 128); }
  void set_check_step(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t check_step_address() const { return address + 128; }
  uint32_t check_item() const { return guest::LoadU32(base, address + 132); }
  void set_check_item(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t check_item_address() const { return address + 132; }
  // create's, delete's and the thumbnail's XOVERLAPPED
  uint32_t content_op_address() const { return address + 136; }
  // an XContentData: the content used
  uint32_t content_data_address() const { return address + 164; }
  // the file read's or write's OVERLAPPED (its event at +488)
  uint32_t file_overlapped_address() const { return address + 472; }
  // the file step's event, also kept here
  uint32_t file_event() const { return guest::LoadU32(base, address + 492); }
  void set_file_event(uint32_t v) const { guest::StoreU32(base, address + 492, v); }
  uint32_t file_event_address() const { return address + 492; }
  uint32_t file_handle() const { return guest::LoadU32(base, address + 496); }
  void set_file_handle(uint32_t v) const { guest::StoreU32(base, address + 496, v); }
  uint32_t file_handle_address() const { return address + 496; }
  // the bytes read or written
  uint32_t file_bytes() const { return guest::LoadU32(base, address + 500); }
  void set_file_bytes(uint32_t v) const { guest::StoreU32(base, address + 500, v); }
  uint32_t file_bytes_address() const { return address + 500; }
  // UTF-16
  uint32_t display_name_address() const { return address + 504; }
  uint32_t file_name_address() const { return address + 760; }
  // TXR360:\TXR_DATA_1
  uint32_t file_path_address() const { return address + 802; }
  uint32_t data() const { return guest::LoadU32(base, address + 932); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 932, v); }
  uint32_t data_address() const { return address + 932; }
  uint32_t data_size() const { return guest::LoadU32(base, address + 936); }
  void set_data_size(uint32_t v) const { guest::StoreU32(base, address + 936, v); }
  uint32_t data_size_address() const { return address + 936; }
  uint32_t thumbnail() const { return guest::LoadU32(base, address + 940); }
  void set_thumbnail(uint32_t v) const { guest::StoreU32(base, address + 940, v); }
  uint32_t thumbnail_address() const { return address + 940; }
  uint32_t thumbnail_size() const { return guest::LoadU32(base, address + 944); }
  void set_thumbnail_size(uint32_t v) const { guest::StoreU32(base, address + 944, v); }
  uint32_t thumbnail_size_address() const { return address + 944; }
  // the save's, load's and delete's sub-step
  uint32_t step() const { return guest::LoadU32(base, address + 948); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 948, v); }
  uint32_t step_address() const { return address + 948; }
  // 0 none, 1 failed, 2 no room
  uint32_t step_error() const { return guest::LoadU32(base, address + 952); }
  void set_step_error(uint32_t v) const { guest::StoreU32(base, address + 952, v); }
  uint32_t step_error_address() const { return address + 952; }
  // -1 none
  uint32_t user() const { return guest::LoadU32(base, address + 956); }
  void set_user(uint32_t v) const { guest::StoreU32(base, address + 956, v); }
  uint32_t user_address() const { return address + 956; }
  // probably; the state the steps share
  uint32_t request_device_selector() const { return guest::LoadU32(base, address + 960); }
  void set_request_device_selector(uint32_t v) const { guest::StoreU32(base, address + 960, v); }
  uint32_t request_device_selector_address() const { return address + 960; }
  // probably
  uint32_t request_enumeration() const { return guest::LoadU32(base, address + 964); }
  void set_request_enumeration(uint32_t v) const { guest::StoreU32(base, address + 964, v); }
  uint32_t request_enumeration_address() const { return address + 964; }
  uint32_t request_save() const { return guest::LoadU32(base, address + 968); }
  void set_request_save(uint32_t v) const { guest::StoreU32(base, address + 968, v); }
  uint32_t request_save_address() const { return address + 968; }
  uint32_t request_load() const { return guest::LoadU32(base, address + 972); }
  void set_request_load(uint32_t v) const { guest::StoreU32(base, address + 972, v); }
  uint32_t request_load_address() const { return address + 972; }
  uint32_t request_delete() const { return guest::LoadU32(base, address + 976); }
  void set_request_delete(uint32_t v) const { guest::StoreU32(base, address + 976, v); }
  uint32_t request_delete_address() const { return address + 976; }
  uint32_t failure_acknowledged() const { return guest::LoadU32(base, address + 980); }
  void set_failure_acknowledged(uint32_t v) const { guest::StoreU32(base, address + 980, v); }
  uint32_t failure_acknowledged_address() const { return address + 980; }
  uint32_t enumerated_saves() const { return guest::LoadU32(base, address + 984); }
  void set_enumerated_saves(uint32_t v) const { guest::StoreU32(base, address + 984, v); }
  uint32_t enumerated_saves_address() const { return address + 984; }
  // 0 no user, 1 no device, 2 a step failed, 3 ready, 4 the device selector, 5 the enumeration, 6 the saves' check, 7 the save, 8 the load, 9 the delete
  uint32_t state() const { return guest::LoadU32(base, address + 988); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 988, v); }
  uint32_t state_address() const { return address + 988; }
  // 2 the save failed, 3 no room, 4 the load failed, 5 the delete failed
  uint32_t failure_reason() const { return guest::LoadU32(base, address + 992); }
  void set_failure_reason(uint32_t v) const { guest::StoreU32(base, address + 992, v); }
  uint32_t failure_reason_address() const { return address + 992; }
};

// allocated; measure: [826A5100]
struct ArchiveStreamer : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = ArchiveStreamerVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: [826A5100].+0
struct ArchiveStreamerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // called at Archive::Init's end
  uint32_t on_init() const { return guest::LoadU32(base, address + 4); }
  void set_on_init(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t on_init_address() const { return address + 4; }
};

// embedded; measure: SaveSlot::Enumerate.r4@+92
struct SaveSlotAt92 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

}  // namespace schema
