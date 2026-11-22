// The views of docs/functions/xaudio-engine.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/xaudio-engine.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: XAudioAllocator, XAudioEngine.+8, XAudioMasteringVoice.+8, sub_8245B170.r3.+8, XAudioEngine::XAudioEngine.r4, AudioRenderDriver::Create.r4, PcmCodec::Create.r4, Reverb::Create.r4, VolumeMeter::Create.r4, XAudioMatrix::Create.r4, XmaCodec::Create.r4, PacketLists::Alloc.r5, VolumeMeter::Init.r5, XAudioMatrix::Init.r5, XmaCodec::XmaCodec.r5, sub_825A50E0.r4, sub_825A5DB8.r4, XAudioVoice.+8, XAudio::GetAllocator.r5.+0, XAudioObject.+8, XAudioBuffer::XAudioBuffer.r5, XAudioBuffer::Create.r4, sub_824603A0.r3@-4
struct XAudioBumpAllocator : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // 0x8204A954
  template <typename T = XAudioBumpAllocatorVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the +4 interface's: 0x8204A980 the default, 0x8204A96C the sizing one; 0x8204A940 while the base destroys, the root's after
  uint32_t interface_vtable() const { return guest::LoadU32(base, address + 4); }
  void set_interface_vtable(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t interface_vtable_address() const { return address + 4; }
  uint32_t ref_count() const { return guest::LoadU32(base, address + 8); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t ref_count_address() const { return address + 8; }
  // XMemAlloc's attributes, handed to XAudio::Free
  uint32_t attributes() const { return guest::LoadU32(base, address + 12); }
  void set_attributes(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t attributes_address() const { return address + 12; }
  // the block's size (the sizing allocator adds each reservation)
  uint32_t capacity() const { return guest::LoadU32(base, address + 16); }
  void set_capacity(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t capacity_address() const { return address + 16; }
  uint32_t used() const { return guest::LoadU32(base, address + 20); }
  void set_used(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t used_address() const { return address + 20; }
  // the default's is its own +28
  uint32_t block() const { return guest::LoadU32(base, address + 24); }
  void set_block(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t block_address() const { return address + 24; }
  // measure fault: BaseSize's description +8 (the effect chain: its count) (from XAudioVoiceAllocator, xaudio)
  uint8_t chain_count() const { return guest::LoadU8(base, address + 0); }
  void set_chain_count(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t chain_count_address() const { return address + 0; }
};

// image; measure: XAudioAllocator.+0, XAudioEngine.+8.+0, XAudioMasteringVoice.+8.+0, sub_8245B170.r3.+8.+0, XAudioEngine::XAudioEngine.r4.+0, AudioRenderDriver::Create.r4.+0, PcmCodec::Create.r4.+0, Reverb::Create.r4.+0, VolumeMeter::Create.r4.+0, XAudioMatrix::Create.r4.+0, XmaCodec::Create.r4.+0, PacketLists::Alloc.r5.+0, VolumeMeter::Init.r5.+0, XAudioMatrix::Init.r5.+0, XmaCodec::XmaCodec.r5.+0, sub_825A50E0.r4.+0, sub_825A5DB8.r4.+0
struct XAudioBumpAllocatorVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t add_ref() const { return guest::LoadU32(base, address + 0); }
  void set_add_ref(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t add_ref_address() const { return address + 0; }
  uint32_t release() const { return guest::LoadU32(base, address + 4); }
  void set_release(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t release_address() const { return address + 4; }
  // XAudioAllocator::Reserve sub_824601F8 (probably)
  uint32_t reserve() const { return guest::LoadU32(base, address + 8); }
  void set_reserve(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t reserve_address() const { return address + 8; }
  // XAudioAllocator::Commit sub_82460210 (probably)
  uint32_t commit() const { return guest::LoadU32(base, address + 12); }
  void set_commit(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t commit_address() const { return address + 12; }
  // XAudioAllocator::Remaining sub_82460388
  uint32_t remaining() const { return guest::LoadU32(base, address + 16); }
  void set_remaining(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t remaining_address() const { return address + 16; }
  // XAudioAllocator::Alloc sub_82460288: (the allocator, bytes), 0 when out of memory
  uint32_t allocate() const { return guest::LoadU32(base, address + 20); }
  void set_allocate(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t allocate_address() const { return address + 20; }
};

// embedded; measure: sub_824603A8.r3, sub_8246D330.r3@-4
struct XAudioAllocatorInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = XAudioAllocatorInterfaceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the allocator's +12, the free's attributes; sub_82460370 adds one to it as a count (probably another class's interface: not read)
  uint32_t attributes() const { return guest::LoadU32(base, address + 8); }
  void set_attributes(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t attributes_address() const { return address + 8; }
};

// image; measure: sub_824603A8.r3.+0
struct XAudioAllocatorInterfaceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // called with r4 1 before the free
  uint32_t destructor() const { return guest::LoadU32(base, address + 0); }
  void set_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destructor_address() const { return address + 0; }
};

// allocated; measure: new<?>@XAudio::Alloc, new<?>@XAudio::Alloc.+8
struct XAudioAllocBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  // an allocator's 0x8204A954; the XMA object's stream count; a factory class's first function (sub_8245D760)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // an allocator's +4 interface; the XMA object's flags; a factory class's second function
  uint32_t interface_vtable() const { return guest::LoadU32(base, address + 4); }
  void set_interface_vtable(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t interface_vtable_address() const { return address + 4; }
  // an allocator's (1); the XMA object's 96-byte entries
  uint32_t ref_count() const { return guest::LoadU32(base, address + 8); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t ref_count_address() const { return address + 8; }
  uint32_t attributes() const { return guest::LoadU32(base, address + 12); }
  void set_attributes(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t attributes_address() const { return address + 12; }
  uint32_t capacity() const { return guest::LoadU32(base, address + 16); }
  void set_capacity(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t capacity_address() const { return address + 16; }
  uint32_t used() const { return guest::LoadU32(base, address + 20); }
  void set_used(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t used_address() const { return address + 20; }
  uint32_t block() const { return guest::LoadU32(base, address + 24); }
  void set_block(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t block_address() const { return address + 24; }
  // XmaObject::Create's (audio.md), not read
  uint32_t xma_unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_xma_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t xma_unknown_28_address() const { return address + 28; }
  // XmaObject::Create's (audio.md), not read
  uint32_t xma_unknown_32() const { return guest::LoadU32(base, address + 32); }
  void set_xma_unknown_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t xma_unknown_32_address() const { return address + 32; }
  // an XMA entry's output buffer (audio.md)
  uint32_t xma_output_buffer() const { return guest::LoadU32(base, address + 68); }
  void set_xma_output_buffer(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t xma_output_buffer_address() const { return address + 68; }
  // an XMA entry's work buffer (audio.md)
  uint32_t xma_work_buffer() const { return guest::LoadU32(base, address + 72); }
  void set_xma_work_buffer(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t xma_work_buffer_address() const { return address + 72; }
};

// embedded; measure: XAudioEngine::Create.r5, sub_82460538.r3, sub_8245F8D0.r4
struct XAudioEngineOutWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the engine (XAudioEngine::Create), the sizing allocator (sub_82460538), a buffer's interface (sub_8245F8D0)
  uint32_t object() const { return guest::LoadU32(base, address + 0); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t object_address() const { return address + 0; }
};

// embedded; measure: XAudioFactory::SizeOf.r4, XAudioEngine::Size.r4, XAudioMasteringVoice::Size.r4, XAudioBuffer::Size.r4, XAudioSourceVoice::Size.r4, XAudioVoice::OutputsSize.r4, XAudioVoice::BaseSize.r4
struct XAudioSizeOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t size() const { return guest::LoadU32(base, address + 0); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t size_address() const { return address + 0; }
};

// embedded; measure: XAudioBuffer::GetFormat.r4, XAudioBuffer::GetFormatThunk.r4
struct XAudioBufferFormatOut : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the buffer's +20 (its byte +21 the channels in use)
  uint32_t format_word() const { return guest::LoadU32(base, address + 0); }
  void set_format_word(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t format_word_address() const { return address + 0; }
  // the buffer's +24 (48,000)
  uint32_t sample_rate() const { return guest::LoadU32(base, address + 4); }
  void set_sample_rate(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t sample_rate_address() const { return address + 4; }
  // the buffer's +28 (the float samples)
  uint32_t samples() const { return guest::LoadU32(base, address + 8); }
  void set_samples(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t samples_address() const { return address + 8; }
};

// embedded; measure: XAudioMasteringVoice::Process.r4
struct XAudioMixInOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // handed to the render driver's submit
  uint32_t buffer() const { return guest::LoadU32(base, address + 0); }
  void set_buffer(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t buffer_address() const { return address + 0; }
};

// embedded; measure: XAudio::SetUpProcessors.r4
struct XAudioProcessors : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the hardware threads in the mask
  uint8_t masked_count() const { return guest::LoadU8(base, address + 0); }
  void set_masked_count(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t masked_count_address() const { return address + 0; }
  // the processors (1 outside the title's process)
  uint8_t processor_count() const { return guest::LoadU8(base, address + 1); }
  void set_processor_count(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t processor_count_address() const { return address + 1; }
  // a byte a hardware thread: 0 none, 1 the main (MasterThread), 2 a worker (WorkerThread); +6 = 4 outside the title's process
  uint32_t thread_kinds_address() const { return address + 2; }
};

// allocated; measure: XAudioFactory, sub_8245D650.r3, sub_8245D6D8.r3, sub_8245D760.r3, sub_8245D970.r3, XAudioEngine.+0
struct XAudioFactory : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  static constexpr uint32_t kVtable = 0x8204A6F4;
  // 0x8204A6F4 (0x8204A6B8 after the destructor); the measure fault: TableSize's r3 (the class table's pointer) read here
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t ref_count() const { return guest::LoadU32(base, address + 4); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t ref_count_address() const { return address + 4; }
  // the largest id + 1
  uint32_t class_count() const { return guest::LoadU32(base, address + 8); }
  void set_class_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t class_count_address() const { return address + 8; }
  // 8 bytes an id: its two functions
  template <typename T = XAudioFactoryEntries>
  T classes() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_classes(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_classes(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t classes_address() const { return address + 12; }
  // the classes from id 128
  uint32_t high_class_count() const { return guest::LoadU32(base, address + 16); }
  void set_high_class_count(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t high_class_count_address() const { return address + 16; }
  // 8 bytes an id from 128 (sub_8245D760 grows them)
  uint32_t high_classes() const { return guest::LoadU32(base, address + 20); }
  void set_high_classes(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t high_classes_address() const { return address + 20; }
};

// allocated; measure: sub_8245D6D8.r3.+12
struct XAudioFactoryEntries : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t entry2_first_function() const { return guest::LoadU32(base, address + 16); }
  void set_entry2_first_function(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t entry2_first_function_address() const { return address + 16; }
  uint32_t entry2_second_function() const { return guest::LoadU32(base, address + 20); }
  void set_entry2_second_function(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t entry2_second_function_address() const { return address + 20; }
};

// image; measure: XAudioFactory.+0, XAudioFactory::XAudioFactory.r4.+0, sub_8245D760.r4, sub_8245D970.r4
struct XAudioClassTable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint8_t count() const { return guest::LoadU8(base, address + 0); }
  void set_count(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // 12 bytes each
  template <typename T = XAudioClassTableEntry>
  T entries() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_entries(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_entries(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t entries_address() const { return address + 4; }
};

// image; measure: XAudioFactory.+0.+4, XAudioFactory::XAudioFactory.r4.+0.+4, sub_8245D760.r4.+4
struct XAudioClassTableEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // sub_8245D760 writes the id it gives (128 + its slot)
  uint8_t id() const { return guest::LoadU8(base, address + 0); }
  void set_id(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
  // the factory entry's +0, which its create slot (sub_8245D650) hands out; xaudio-objects.md calls class 0's the size
  uint32_t first_function() const { return guest::LoadU32(base, address + 4); }
  void set_first_function(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_function_address() const { return address + 4; }
  // the factory entry's +4 (its size slot sub_8245D6D8's)
  uint32_t second_function() const { return guest::LoadU32(base, address + 8); }
  void set_second_function(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t second_function_address() const { return address + 8; }
};

// embedded; measure: XAudioFactory::XAudioFactory.r4
struct XAudioClassTableRef : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = XAudioClassTable>
  T table() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_table(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_table(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t table_address() const { return address + 0; }
};

// embedded; measure: sub_8245D650.r4, sub_8245D6D8.r4
struct XAudioFactoryDescriptorId : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  // below 128 in +8/+12, from 128 in +16/+20
  uint8_t class_id() const { return guest::LoadU8(base, address + 0); }
  void set_class_id(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t class_id_address() const { return address + 0; }
};

// embedded; measure: XAudioStage, XAudioEngine::RunStage.r4, XAudioEngine::StageOf.ret
struct XAudioStage : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // the stage's voices: a list head
  uint32_t voices_next() const { return guest::LoadU32(base, address + 0); }
  void set_voices_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t voices_next_address() const { return address + 0; }
  uint32_t voices_prev() const { return guest::LoadU32(base, address + 4); }
  void set_voices_prev(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t voices_prev_address() const { return address + 4; }
  // 16
  uint32_t voices_node_offset() const { return guest::LoadU32(base, address + 8); }
  void set_voices_node_offset(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t voices_node_offset_address() const { return address + 8; }
  // one of the two lists to mix and mixed
  uint32_t list_a_next() const { return guest::LoadU32(base, address + 12); }
  void set_list_a_next(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t list_a_next_address() const { return address + 12; }
  uint32_t list_a_prev() const { return guest::LoadU32(base, address + 16); }
  void set_list_a_prev(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t list_a_prev_address() const { return address + 16; }
  // 24
  uint32_t list_a_node_offset() const { return guest::LoadU32(base, address + 20); }
  void set_list_a_node_offset(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t list_a_node_offset_address() const { return address + 20; }
  // the other
  uint32_t list_b_next() const { return guest::LoadU32(base, address + 24); }
  void set_list_b_next(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t list_b_next_address() const { return address + 24; }
  uint32_t list_b_prev() const { return guest::LoadU32(base, address + 28); }
  void set_list_b_prev(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t list_b_prev_address() const { return address + 28; }
  // 24
  uint32_t list_b_node_offset() const { return guest::LoadU32(base, address + 32); }
  void set_list_b_node_offset(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t list_b_node_offset_address() const { return address + 32; }
  // the list to mix (+12 or +24), swapped with +40 each mix
  template <typename T = XAudioList>
  T to_mix() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_to_mix(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_to_mix(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t to_mix_address() const { return address + 36; }
  // the list mixed
  template <typename T = XAudioList>
  T mixed() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_mixed(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_mixed(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t mixed_address() const { return address + 40; }
};

// embedded; measure: XAudioEngine::RunStage.r4.+36, XAudioEngine::RunStage.r4.+40, XAudioEngine::StageOf.ret.+40, PacketList::Next.ret, sub_8245FE88.r3@+24
struct XAudioList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t next() const { return guest::LoadU32(base, address + 0); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  uint32_t prev() const { return guest::LoadU32(base, address + 4); }
  void set_prev(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t prev_address() const { return address + 4; }
  // a list head's only: the node's offset in its object
  uint32_t node_offset() const { return guest::LoadU32(base, address + 8); }
  void set_node_offset(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_offset_address() const { return address + 8; }
};

// embedded; measure: PacketList::Next.ret.+0
struct XAudioTeardownNodeData : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: XAudioEngine::StageOf.r4
struct XAudioMixVoice : View {
  using View::View;
  static constexpr uint32_t kSize = 77;
  template <typename T = XAudioMixVoiceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a submix voice's stage (its index in the engine's +124)
  uint8_t submix_stage() const { return guest::LoadU8(base, address + 76); }
  void set_submix_stage(uint8_t v) const { guest::StoreU8(base, address + 76, v); }
  uint32_t submix_stage_address() const { return address + 76; }
};

// image; measure: XAudioEngine::StageOf.r4.+0
struct XAudioMixVoiceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // the voice's format (byte +0 the type: 0 source, 1 submix)
  uint32_t get_format() const { return guest::LoadU32(base, address + 52); }
  void set_get_format(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t get_format_address() const { return address + 52; }
};

// allocated; measure: XAudioMasteringVoice, XAudioEngine.+64, sub_8245FE88.r3
struct XAudioMasteringVoice : View {
  using View::View;
  static constexpr uint32_t kSize = 72;
  static constexpr uint32_t kVtable = 0x8204A8E8;
  // 0x8204A8E8
  template <typename T = XAudioMasteringVoiceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t ref_count() const { return guest::LoadU32(base, address + 4); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t ref_count_address() const { return address + 4; }
  template <typename T = XAudioBumpAllocator>
  T allocator() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_allocator(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_allocator(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t allocator_address() const { return address + 8; }
  // the active list's node (sub_8245FE88 unlinks it)
  template <typename T = XAudioList>
  T active_next() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_active_next(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_active_next(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t active_next_address() const { return address + 24; }
  template <typename T = XAudioList>
  T active_prev() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_active_prev(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_active_prev(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t active_prev_address() const { return address + 28; }
  // an XAudioBuffer's interface (its +8)
  uint32_t output_buffer() const { return guest::LoadU32(base, address + 32); }
  void set_output_buffer(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t output_buffer_address() const { return address + 32; }
  // bit 0 playing
  uint8_t flags() const { return guest::LoadU8(base, address + 61); }
  void set_flags(uint8_t v) const { guest::StoreU8(base, address + 61, v); }
  uint32_t flags_address() const { return address + 61; }
  // the render driver object (20 bytes)
  template <typename T = XAudioMasteringDriver>
  T driver() const { return T(base, guest::LoadU32(base, address + 68)); }
  void set_driver(const View& v) const { guest::StoreU32(base, address + 68, v.address); }
  void set_driver(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t driver_address() const { return address + 68; }
};

// image; measure: XAudioMasteringVoice.+0, XAudioEngine.+64.+0
struct XAudioMasteringVoiceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 72;
  // the delete (the teardown's)
  uint32_t delete_object() const { return guest::LoadU32(base, address + 12); }
  void set_delete_object(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t delete_object_address() const { return address + 12; }
  // XAudioMasteringVoice::Start sub_8245FE20
  uint32_t start() const { return guest::LoadU32(base, address + 56); }
  void set_start(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t start_address() const { return address + 56; }
  // the mix's pass
  uint32_t pass() const { return guest::LoadU32(base, address + 68); }
  void set_pass(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t pass_address() const { return address + 68; }
};

// allocated; measure: XAudioMasteringVoice.+68, XAudioEngine.+64.+68
struct XAudioMasteringDriver : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // 0x8204AAEC
  template <typename T = XAudioMasteringDriverVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x8204AAD0
  template <typename T = XAudioMasteringDriverInnerVtable>
  T inner_vtable() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_inner_vtable(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_inner_vtable(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t inner_vtable_address() const { return address + 4; }
  // the render driver client's handle (audio.md)
  uint32_t handle() const { return guest::LoadU32(base, address + 16); }
  void set_handle(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t handle_address() const { return address + 16; }
};

// image; measure: XAudioMasteringVoice.+68.+0, XAudioEngine.+64.+68.+0
struct XAudioMasteringDriverVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // (the driver, the frame's buffer, 0): the frame to our audio output
  uint32_t submit() const { return guest::LoadU32(base, address + 24); }
  void set_submit(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t submit_address() const { return address + 24; }
  // AudioRenderDriver::Register sub_824657E0: the mixer's callback to our audio output
  uint32_t register_callback() const { return guest::LoadU32(base, address + 28); }
  void set_register_callback(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t register_callback_address() const { return address + 28; }
};

// image; measure: XAudioMasteringVoice.+68.+4
struct XAudioMasteringDriverInnerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // called with r4 1 by the voice's destructor
  uint32_t delete_object() const { return guest::LoadU32(base, address + 0); }
  void set_delete_object(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t delete_object_address() const { return address + 0; }
};

// embedded; measure: AudioRenderDriver::AudioRenderDriver.r4
struct XAudioDriverDescription : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 2
  uint8_t type() const { return guest::LoadU8(base, address + 0); }
  void set_type(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t type_address() const { return address + 0; }
  // the mastering voice: the driver's +12
  uint32_t voice() const { return guest::LoadU32(base, address + 4); }
  void set_voice(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t voice_address() const { return address + 4; }
  // 0
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// allocated; measure: sub_8245B170.r3
struct XAudioEngineBase : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 0x8204A6CC, then the root's 0x8204A6B8
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // released (slot +4), then 0
  template <typename T = XAudioBumpAllocator>
  T allocator() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_allocator(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_allocator(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t allocator_address() const { return address + 8; }
};

// allocated; measure: sub_8245B118.r3, sub_8245EF28.r3
struct XAudioRootObject : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0x8204A6B8 (sub_8245B118) or 0x8204A868's (sub_8245EF28) written
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// allocated; measure: sub_8245AAB8.r3
struct XAudioDeletable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = XAudioDeletableVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: sub_8245AAB8.r3.+0
struct XAudioDeletableVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the deleting destructor (r4 1), tail-called
  uint32_t delete_object() const { return guest::LoadU32(base, address + 0); }
  void set_delete_object(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t delete_object_address() const { return address + 0; }
};

// embedded; measure: sub_82465398.r3
struct XAudioCodecInner192 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the object's +0 (probably its virtual base table)
  template <typename T = XAudioCodecVbtable>
  T outer_vbtable() const { return T(base, guest::LoadU32(base, address + -192)); }
  void set_outer_vbtable(const View& v) const { guest::StoreU32(base, address + -192, v.address); }
  void set_outer_vbtable(uint32_t v) const { guest::StoreU32(base, address + -192, v); }
  uint32_t outer_vbtable_address() const { return address + -192; }
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// embedded; measure: sub_82465590.r3
struct XAudioCodecInner52 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the object's +0 (probably its virtual base table)
  template <typename T = XAudioCodecVbtable>
  T outer_vbtable() const { return T(base, guest::LoadU32(base, address + -52)); }
  void set_outer_vbtable(const View& v) const { guest::StoreU32(base, address + -52, v.address); }
  void set_outer_vbtable(uint32_t v) const { guest::StoreU32(base, address + -52, v); }
  uint32_t outer_vbtable_address() const { return address + -52; }
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: sub_82465398.r3.+-192, sub_82465590.r3.+-52
struct XAudioCodecVbtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // probably the virtual base's offset
  uint32_t vbase_offset() const { return guest::LoadU32(base, address + 4); }
  void set_vbase_offset(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vbase_offset_address() const { return address + 4; }
};

// The image words docs/functions/xaudio-engine.md's code uses.
struct XaudioEngineGlobals {
  // XAudio's 7 statistics counters (XAudio::AddStat)
  static constexpr uint32_t stats_address() { return 0x82954F24u; }
  // the counters' copies, last frame's (XAudioEngine::TakeStats)
  static constexpr uint32_t stats_copy_address() { return 0x82954F40u; }
  // the debug monitor's data (a pointer to a pointer; its slot +24 registers XAudio's statistics)
  static uint32_t debug_monitor(uint8_t* base) { return guest::LoadU32(base, 0x820007F0u); }
  static void set_debug_monitor(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x820007F0u, v); }
  static constexpr uint32_t debug_monitor_address() { return 0x820007F0u; }
  // probably PsThreadObjectType
  static uint32_t thread_type(uint8_t* base) { return guest::LoadU32(base, 0x820007FCu); }
  static void set_thread_type(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x820007FCu, v); }
  static constexpr uint32_t thread_type_address() { return 0x820007FCu; }
  // the title terminate notification (routine 0x8245B0D0, priority 0x7D800000)
  static constexpr uint32_t terminate_registration_address() { return 0x82954EB4u; }
  // a notification event (title terminating)
  static constexpr uint32_t terminating_address() { return 0x82954EC4u; }
  // a kernel semaphore (count 0, limit 6): the workers' wake-up
  static constexpr uint32_t worker_semaphore_address() { return 0x82954ED4u; }
  // a synchronization event (the master thread's mix done)
  static constexpr uint32_t mix_done_address() { return 0x82954EE8u; }
  // a synchronization event (the master thread's mix now)
  static constexpr uint32_t mix_now_address() { return 0x82954EF8u; }
  // the mixer's kernel critical section (at the lock object 0x8264D4F4's +4)
  static constexpr uint32_t mixer_lock_address() { return 0x8264D4F8u; }
  // XAudio::Free's r3 (its address, unused)
  static constexpr uint32_t free_heap_address() { return 0x82954F20u; }
};

}  // namespace schema
