// XAudio's engine and mixer as plain C++ (engine.h;
// docs/functions/xaudio-engine.md).

#include "xaudio/engine.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "schema/audio.h"
#include "schema/xaudio.h"
#include "schema/xaudio_engine.h"
#include "schema/xaudio_objects.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <atomic>

namespace xaudio {

namespace {

using G = schema::XaudioEngineGlobals;
using Allocator = schema::XAudioBumpAllocator;
using ClassTable = schema::XAudioClassTable;
using ClassTableEntry = schema::XAudioClassTableEntry;
using ClassTableRef = schema::XAudioClassTableRef;
using Factory = schema::XAudioFactory;
using MasteringVoice = schema::XAudioMasteringVoice;
using Stage = schema::XAudioStage;
using List = schema::XAudioList;
using Engine = schema::XAudioEngine;  // xaudio.toml
using Buffer = schema::XAudioBuffer;  // xaudio.toml
using RenderDriver = schema::AudioRenderDriver;  // audio.toml

// The live engine ([0x82954F08], xaudio.toml's globals).
uint32_t LiveEngine(uint8_t* base) { return schema::XaudioGlobals::engine(base).address; }

Regs Args(std::initializer_list<uint64_t> r) {
  Regs regs;
  for (const uint64_t value : r) {
    regs.r[regs.count++] = value;
  }
  return regs;
}

// The call made at `sp` (a plain C++ function's own frame).
Regs At(uint32_t sp, Regs regs) {
  regs.sp = sp;
  return regs;
}

// The registers the original sets before calling a function whose plain
// C++ runs here instead.
void SetRegs(const GuestCall& call, Regs regs) { call(0, 0, regs); }

thread_local std::optional<MakingStandIn> making_stand_in;

const MakingStandIn* MakingStandInOf() {
  return making_stand_in ? &*making_stand_in : nullptr;
}

}  // namespace

void SetMakingForHarness(std::optional<MakingStandIn> stand_in) { making_stand_in = stand_in; }

// --- Leaves ---

uint32_t FactoryTableSize(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_8245D9E8 XAudioFactory::TableSize");
  if (table == 0) {
    return 0;
  }
  const ClassTable classes = ClassTableRef(base, table).table();
  if (classes.address == 0) {
    return 0;
  }
  const uint32_t count = classes.count();
  uint32_t ids = 0;
  if (count != 0) {
    uint32_t entry = classes.entries().address;
    for (uint32_t i = 0; i < count; i++, entry += 12) {
      const uint32_t id = ClassTableEntry(base, entry).id() + 1u;
      if (id > ids) {
        ids = id;
      }
    }
  }
  return ids << 3;
}

uint32_t FactorySizeOf(uint8_t* base, uint32_t table, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245DBD0 XAudioFactory::SizeOf");
  schema::XAudioSizeOut(base, out).set_size(FactoryTableSize(base, table) + 24);
  return 0;
}

uint32_t BufferGetFormat(uint8_t* base, uint32_t buffer, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245F6F0 XAudioBuffer::GetFormat");
  const schema::XAudioBufferFormatOut format(base, out);
  const Buffer object(base, buffer);
  format.set_format_word(object.current());
  format.set_sample_rate(object.current_rate());
  format.set_samples(object.samples());
  return 0;
}

uint32_t BufferGetFormatThunk(uint8_t* base, uint32_t iface, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245F718 XAudioBuffer::GetFormatThunk");
  const uint32_t buffer = iface != 0 ? iface - 8 : 0;
  const schema::XAudioBufferFormatOut format(base, out);
  const Buffer object(base, buffer);
  format.set_format_word(object.current());
  format.set_sample_rate(object.current_rate());
  format.set_samples(object.samples());
  return 0;
}

void BufferInterface(uint8_t* base, uint32_t buffer, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245F8D0 XAudioBuffer::GetInterface");
  schema::XAudioEngineOutWord(base, out).set_object(buffer != 0 ? buffer + 8 : 0);
}

uint64_t AllocatorRemaining(uint8_t* base, uint32_t allocator) {
  ITC_NATIVE_RUNNING("sub_82460388 XAudioAllocator::Remaining");
  const Allocator a(base, allocator);
  return uint64_t(a.capacity()) - uint64_t(a.used());
}

uint64_t InterfaceAddRef(uint8_t* base, uint32_t iface) {
  ITC_NATIVE_RUNNING("sub_82460370 XAudioAllocator::AddRef (interface)");
  const schema::XAudioAllocatorInterface object(base, iface);
  const uint64_t count = uint64_t(object.attributes()) + 1;
  object.set_attributes(uint32_t(count));
  return count;
}

uint32_t ObjectInfo(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82461EF8 (an object's info)");
  const schema::FactoryInfoOut info(base, out);
  info.set_info_byte(6);
  info.set_info_halfword(3750);
  return 0;
}

uint32_t TakeStats(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8245F678 XAudioEngine::TakeStats");
  for (uint32_t i = 0; i < kStatCount * 4; i += 4) {
    const uint32_t value = guest::LoadU32(base, G::stats_address() + i);
    guest::StoreU32(base, G::stats_address() + i, 0);
    guest::StoreU32(base, G::stats_copy_address() + i, value);
  }
  return 0;
}

uint32_t Free(uint32_t block, uint32_t attributes) {
  ITC_NATIVE_RUNNING("sub_8245EF90 XAudio::Free");
  return memory::crt::XMemFree(block, attributes);
}

uint64_t AllocatorAlloc(uint8_t* base, uint32_t allocator, uint32_t size, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460288 XAudioAllocator::Alloc");
  const uint32_t rounded = (size + 3) & ~3u;
  // r4 stays the caller's size, as in the original's call.
  const Allocator a(base, allocator);
  const uint32_t left = call(a.vtable().remaining(), 0x824602B8, Args({allocator}));
  if (rounded > left) {
    return 0;
  }
  const uint32_t used = a.used();
  const uint64_t block = uint64_t(a.block()) + used;
  a.set_used(used + rounded);
  return block;
}

void DeleteObject(uint8_t* base, uint32_t object, uint32_t* function) {
  ITC_NATIVE_RUNNING("sub_8245AAB8 XAudioObject::Delete");
  *function = object != 0 ? schema::XAudioDeletable(base, object).vtable().delete_object() : 0;
}

void Barrier(uint8_t* base, uint32_t engine, uint32_t barrier, uint32_t processor) {
  ITC_NATIVE_RUNNING("sub_8245A940 XAudioEngine::Barrier");
  const Engine object(base, engine);
  if (object.processor_count() == 0) {
    return;
  }
  guest::StoreU8(base, barrier + processor, 1);
  // Each processor's byte, processor 0 the most significant (the word is
  // big-endian, byte i at +i), read from the last thread to the first as the
  // original does.
  uint64_t all = 0;
  for (int i = 5; i >= 0; i--) {
    const uint64_t has = guest::LoadU32(base, object.processor_threads_address() + 4 * uint32_t(i)) != 0 ? 1 : 0;
    all |= has << (56 - 8 * i);
  }
  // The original's spin (db16cyc in the loop, nothing in the translation):
  // guest::LoadU64's volatile loads, as the recompiled processors read it.
  uint64_t word = guest::LoadU64(base, barrier);
  if (word == 0) {
    return;
  }
  while (word != all) {
    word = guest::LoadU64(base, barrier);
    if (word == 0) {
      return;
    }
  }
  guest::StoreU64(base, barrier, uint64_t(0));
}

// --- The engine's making ---

uint32_t EngineConstruct(uint8_t* base, uint32_t engine, uint32_t allocator,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245C098 XAudioEngine::XAudioEngine");
  const Engine object(base, engine);
  object.set_allocator(allocator);
  object.set_vtable(kEngineVtable0);
  object.set_ref_count(1);
  if (allocator != 0) {
    call(Allocator(base, allocator).vtable().add_ref(), 0x8245C0E0, Args({allocator}));
  }
  object.set_vtable(kEngineVtable);
  // The start-pending list (+68, its node offset 24 at +76).
  const List pending(base, object.pending_next_address());
  pending.set_next(pending.address);
  pending.set_node_offset(24);
  pending.set_prev(pending.address);
  // The source voices' stage (+80: its list, node offset 16; the two lists
  // at +92 and +104, node offset 24; +116 and +120 pointing at them).
  const Stage stage(base, object.source_stage_address());
  const uint32_t active = stage.list_a_next_address(), next = stage.list_b_next_address();
  stage.set_voices_node_offset(16);
  stage.set_voices_next(stage.address);
  stage.set_voices_prev(stage.address);
  stage.set_list_a_node_offset(24);
  stage.set_list_a_next(active);
  stage.set_list_a_prev(active);
  stage.set_list_b_node_offset(24);
  stage.set_list_b_next(next);
  stage.set_list_b_prev(next);
  stage.set_to_mix(active);
  stage.set_mixed(next);
  guest::StoreU32(base, object.category_volumes_address(), kOneFloatBits);
  guest::StoreU32(base, object.category_volume_1_address(), kOneFloatBits);
  schema::XaudioGlobals::set_engine(base, engine);
  return engine;
}

uint32_t FactoryConstruct(uint8_t* base, uint32_t factory, uint32_t table, uint32_t allocator,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245DA48 XAudioFactory::XAudioFactory");
  const Factory object(base, factory);
  const ClassTableRef ref(base, table);
  object.set_vtable(kFactoryVtable);
  object.set_ref_count(1);
  ClassTable classes = ref.table();
  uint32_t count = classes.count();
  uint32_t ids = 0;
  if (count != 0) {
    uint32_t entry = classes.entries().address;
    for (; count != 0; count--, entry += 12) {
      const uint32_t id = ClassTableEntry(base, entry).id() + 1u;
      if (id > ids) {
        ids = id;
      }
    }
  }
  object.set_class_count(ids);
  if (ids != 0) {
    const uint32_t entries = call(Allocator(base, allocator).vtable().allocate(), 0x8245DAD8,
                                  At(frame, Args({allocator, ids << 3})));
    object.set_classes(entries);
  }
  // Each class's functions at its id's entry, the table read again each
  // time, as the original does. The factory's entries are 8 bytes an id
  // (+0 the entry's first function, +4 its second).
  classes = ref.table();
  if (classes.count() != 0) {
    uint32_t offset = 0;
    uint32_t i = 0;
    do {
      const ClassTableEntry entry(base, classes.entries().address + offset);
      i++;
      const uint32_t at = uint32_t(entry.id()) << 3;
      guest::StoreU32(base, object.classes().address + at, entry.first_function());
      const ClassTable again = ref.table();
      const uint32_t to = object.classes().address + at;
      const uint32_t second =
          ClassTableEntry(base, again.entries().address + offset).second_function();
      offset += 12;
      guest::StoreU32(base, to + 4, second);
      classes = ref.table();
    } while (i < classes.count());
  }
  return factory;
}

uint32_t FactoryCreate(uint8_t* base, uint32_t table, uint32_t allocator, uint32_t out,
                       uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245DC10 XAudioFactory::Create");
  const uint32_t memory = call(Allocator(base, allocator).vtable().allocate(), 0x8245DC40,
                               At(frame, Args({allocator, 24})));
  if (memory != 0) {
    SetRegs(call, Args({memory, table, allocator}));
    const uint32_t factory =
        FactoryConstruct(base, memory, table, allocator, FrameBelow(base, frame, 112), call);
    if (factory != 0) {
      // The factory's interface (+8) to [out] (the engine's +60).
      guest::StoreU32(base, out, factory + 8);
      return 0;
    }
  }
  return 0x8007000Eu;  // E_OUTOFMEMORY
}

uint32_t FactoryEntry(uint8_t* base, uint32_t factory, uint32_t descriptor, uint32_t which,
                      uint32_t* function) {
  if (which == 0) {
    ITC_NATIVE_RUNNING("sub_8245D650 XAudioFactory::CreateObject");
  } else {
    ITC_NATIVE_RUNNING("sub_8245D6D8 XAudioFactory::SizeObject");
  }
  *function = 0;
  const Factory object(base, factory);
  uint32_t id = schema::XAudioFactoryDescriptorId(base, descriptor).class_id();
  uint32_t entries;
  if (id <= 127) {
    if (id >= object.class_count()) {
      return 0x80004001u;
    }
    entries = object.classes().address;
  } else {
    id -= 128;
    if (id >= object.high_class_count()) {
      return 0x80004001u;
    }
    entries = object.high_classes();
  }
  const uint32_t entry = (id << 3) + entries;
  if (guest::LoadU32(base, entry + 4) == 0 || guest::LoadU32(base, entry) == 0) {
    return 0x80004001u;  // E_NOTIMPL
  }
  *function = guest::LoadU32(base, entry + 4 * which);
  return 0;
}

uint32_t RegisterStats(uint8_t* base, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245F588 XAudio::RegisterStats");
  guest::StoreU32(base, frame + 80, 4);
  guest::StoreU32(base, frame + 84, kStatsCallback);
  const uint32_t monitor = guest::LoadU32(base, G::debug_monitor(base));
  if (monitor != 0) {
    call(guest::LoadU32(base, monitor + 24), 0x8245F5D0, At(frame, Args({47, frame + 80})));
  }
  return 0;
}

uint32_t SetUpProcessors(uint8_t* base, uint32_t mask, uint32_t out, uint32_t frame,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A828 XAudio::SetUpProcessors");
  if (const auto* stand_in = MakingStandInOf()) {
    for (uint32_t i = 0; i < 8; i++) {
      guest::StoreU8(base, out + i, stand_in->processors[i]);
    }
    return 1;
  }
  mask &= 0x3F;
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU8(base, out + i, 0);
  }
  const schema::XAudioProcessors processors(base, out);
  const uint32_t kinds = processors.thread_kinds_address();  // a byte a hardware thread
  // The registers as the original's call leaves them (r3 the mask, r4 out).
  const uint32_t type = call(kKeGetCurrentProcessType, 0x8245A864, At(frame, Regs{}));
  if (int32_t(type) != 1) {
    processors.set_processor_count(1);
    guest::StoreU8(base, kinds + 4, 4);
    return type;
  }
  if ((mask & 0xFF) == 0) {
    mask = 16;
  }
  mask &= 0xFF;
  for (uint32_t i = 0, bits = mask; i < 6; i++, bits >>= 1) {
    if ((bits & 1) != 0) {
      guest::StoreU8(base, kinds + i, 2);
      processors.set_masked_count(uint8_t(processors.masked_count() + 1));
      processors.set_processor_count(uint8_t(processors.processor_count() + 1));
    }
  }
  if ((mask & 0x10) != 0) {
    guest::StoreU8(base, kinds + 4, 1);
    return type;
  }
  for (int i = 5; i >= 0; i--) {
    if (guest::LoadU8(base, kinds + uint32_t(i)) != 0) {
      guest::StoreU8(base, kinds + uint32_t(i), 1);
      return type;
    }
  }
  return type;
}

uint32_t EngineSize(uint8_t* base, uint32_t params, uint32_t out, uint32_t frame,
                    const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245B2C0 XAudioEngine::Size");
  // The parameters (xaudio.toml describes them on XAudioEngine, its measure
  // faults: +0 the channels, +1 the stages, +2 the processors, +4 the class
  // table where the engine has its ref count).
  const Engine init(base, params);
  uint32_t classes = init.ref_count();
  if (classes == 0) {
    classes = kDefaultClasses;
  }
  guest::StoreU32(base, frame + 80, classes);
  SetRegs(call, Args({frame + 80, frame + 88}));
  const uint32_t factory = FactorySizeOf(base, frame + 80, frame + 88);
  if (int32_t(factory) < 0) {
    return factory;
  }
  // The output buffer's format (20 bytes at +112): type 0, max(+0, 6)
  // channels, 48,000 Hz.
  const uint32_t format = frame + 112;
  guest::StoreU64(base, format, uint64_t(0));
  guest::StoreU64(base, format + 8, uint64_t(0));
  guest::StoreU32(base, format + 16, 0);
  guest::StoreU8(base, format, 0);
  guest::StoreU8(base, format + 4, 0);
  uint32_t channels = init.params_channels();
  if (channels <= 6) {
    channels = 6;
  }
  guest::StoreU8(base, format + 5, uint8_t(channels));
  guest::StoreU32(base, format + 8, kSampleRate);
  SetRegs(call, Args({format, frame + 84}));
  uint32_t r30;
  if (const auto r3 = BufferSize(base, format, frame + 84)) {
    r30 = *r3;
  } else {
    r30 = call(kBufferSize, 0x8245B348, At(frame, Args({format, frame + 84})));
  }
  if (int32_t(r30) < 0) {
    return r30;
  }
  const uint32_t mask = init.params_processors();
  SetRegs(call, Args({mask, frame + 96}));
  SetUpProcessors(base, mask, frame + 96, FrameBelow(base, frame, 112), call);
  const uint32_t processors = guest::LoadU8(base, frame + 97);
  uint32_t size = (processors * guest::LoadU32(base, frame + 84)) << 1;
  size += guest::LoadU32(base, frame + 88);
  const uint32_t stages = init.params_stages();
  if (stages != 0) {
    size += stages * 44 + 4;
  }
  schema::XAudioSizeOut(base, out).set_size(size);
  return r30;
}

// --- The mastering voice ---

namespace {

// The 32-byte voice description the mastering voice's Size and Init build
// at `at`: its first 24 bytes cleared, 6 channels at 48,000 Hz, the
// description's three words (when given) at +8, +16 and +20; +24 on as the
// stack had them.
void MasteringDescription(uint8_t* base, uint32_t at, uint32_t description) {
  guest::StoreU64(base, at, uint64_t(0));
  guest::StoreU64(base, at + 8, uint64_t(0));
  guest::StoreU64(base, at + 16, uint64_t(0));
  guest::StoreU8(base, at, 0);
  guest::StoreU8(base, at + 1, 6);
  guest::StoreU32(base, at + 4, kSampleRate);
  if (description != 0) {
    guest::StoreU32(base, at + 8, guest::LoadU32(base, description));
    guest::StoreU32(base, at + 16, guest::LoadU32(base, description + 4));
    guest::StoreU32(base, at + 20, guest::LoadU32(base, description + 8));
  }
}

// The voice's buffer (+32, an interface) cleared for its own format with
// flags 3 (ClearThunk's plain C++, its original where that refuses).
void ClearBuffer(uint8_t* base, uint32_t voice, uint32_t return_address, uint32_t frame,
                 const GuestCall& call) {
  const uint32_t iface = MasteringVoice(base, voice).output_buffer();
  SetRegs(call, Args({iface, 0, 3}));
  if (!BufferClearThunk(base, iface, 0, 3)) {
    call(kBufferClearThunk, return_address, At(frame, Args({iface, 0, 3})));
  }
}

}  // namespace

uint32_t MasteringSize(uint8_t* base, uint32_t description, uint32_t out, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245FF68 XAudioMasteringVoice::Size");
  const uint32_t local = frame + 96;
  MasteringDescription(base, local, description);
  SetRegs(call, Args({local, frame + 80}));
  const uint32_t r3 = BaseSize(base, local, frame + 80, FrameBelow(base, frame, 176), call);
  if (int32_t(r3) >= 0) {
    schema::XAudioSizeOut(base, out).set_size(guest::LoadU32(base, frame + 80) + 20);
  }
  return r3;
}

void RenderDriverConstruct(uint8_t* base, uint32_t driver, uint32_t description) {
  ITC_NATIVE_RUNNING("sub_82465938 AudioRenderDriver::AudioRenderDriver");
  const RenderDriver object(base, driver);
  object.set_ref_count(1);
  object.set_inner_vtable(kDriverVtableFirst);
  object.set_callback_arg(schema::XAudioDriverDescription(base, description).voice());
  object.set_vtable(kDriverVtable);
  object.set_inner_vtable(kDriverVtable2);
}

uint32_t MasteringInit(uint8_t* base, uint32_t voice, uint32_t description, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245FCA0 XAudioMasteringVoice::Init");
  const uint32_t local = frame + 96;
  MasteringDescription(base, local, description);
  SetRegs(call, Args({voice, local}));
  uint32_t r3 = InitBase(base, voice, local, FrameBelow(base, frame, 144), call);
  if (int32_t(r3) < 0) {
    return r3;
  }
  // The driver's description: +0 the type (2), +4 the voice, +8 0.
  const uint32_t driver_description = frame + 80;
  guest::StoreU64(base, driver_description, uint64_t(0));
  const schema::XAudioDriverDescription driver_desc(base, driver_description);
  driver_desc.set_unknown_8(0);
  driver_desc.set_voice(voice);
  driver_desc.set_type(2);
  const MasteringVoice mastering(base, voice);
  const Allocator allocator = mastering.allocator();
  const uint32_t memory =
      call(allocator.vtable().allocate(), 0x8245FD48, At(frame, Args({allocator.address, 20})));
  uint32_t driver = 0;
  if (memory != 0) {
    SetRegs(call, Args({memory, driver_description, mastering.allocator().address}));
    RenderDriverConstruct(base, memory, driver_description);
    driver = memory;
  }
  mastering.set_driver(driver);
  if (driver == 0) {
    return 0x8007000Eu;  // E_OUTOFMEMORY
  }
  // The driver's slot +32 (audio.md's AudioRenderDriver, not this schema's).
  r3 = call(Method(base, driver, 32), 0x8245FD94,
            At(frame, Args({driver, driver_description, mastering.allocator().address})));
  if (int32_t(r3) < 0) {
    return r3;
  }
  return call(mastering.vtable().start(), 0x8245FDB0, At(frame, Args({voice})));
}

uint32_t MasteringCreate(uint8_t* base, uint32_t description, uint32_t out, bool native_heap,
                         uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824600F0 XAudioMasteringVoice::Create");
  if (const auto* stand_in = MakingStandInOf()) {
    if (int32_t(stand_in->mastering_r3) >= 0) {
      guest::StoreU32(base, out, stand_in->mastering_voice);
    }
    return stand_in->mastering_r3;
  }
  const uint32_t slot = frame + 80;  // the allocator
  guest::StoreU32(base, slot, 0);
  SetRegs(call, Args({description, frame + 84}));
  uint32_t r30 = MasteringSize(base, description, frame + 84, FrameBelow(base, frame, 144), call);
  if (int32_t(r30) >= 0) {
    const uint32_t size = guest::LoadU32(base, frame + 84) + 72;
    SetRegs(call, Args({kMasteringAttributes, size, slot}));
    if (const auto r3 = GetAllocator(base, kMasteringAttributes, size, slot,
                                     FrameBelow(base, frame, 128), native_heap, call)) {
      r30 = *r3;
    } else {
      r30 = call(kGetAllocator, 0x82460148, At(frame, Args({kMasteringAttributes, size, slot})));
    }
    if (int32_t(r30) >= 0) {
      const uint32_t allocator = guest::LoadU32(base, slot);
      const uint32_t memory = call(Allocator(base, allocator).vtable().allocate(), 0x8246016C,
                                   At(frame, Args({allocator, 72})));
      if (memory == 0) {
        r30 = 0x8007000Eu;  // E_OUTOFMEMORY
      } else {
        SetRegs(call, Args({memory, guest::LoadU32(base, slot), 2}));
        VoiceConstruct(base, memory, guest::LoadU32(base, slot), 2, call);
        MasteringVoice(base, memory).set_vtable(kMasteringVtable);
        SetRegs(call, Args({memory, description}));
        r30 = MasteringInit(base, memory, description, FrameBelow(base, frame, 144), call);
        if (int32_t(r30) >= 0) {
          guest::StoreU32(base, out, memory);
        } else {
          call(MasteringVoice(base, memory).vtable().delete_object(), 0x824601D0,
               At(frame, Args({memory})));
        }
      }
    }
  }
  const uint32_t allocator = guest::LoadU32(base, slot);
  if (allocator != 0) {
    call(Allocator(base, allocator).vtable().release(), 0x824601EC, At(frame, Args({allocator})));
  }
  return r30;
}

uint32_t MasteringStart(uint8_t* base, uint32_t voice, uint32_t frame, const Thread& thread,
                        const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245FE20 XAudioMasteringVoice::Start");
  if ((MasteringVoice(base, voice).flags() & 1) != 0) {
    return 0;
  }
  ClearBuffer(base, voice, 0x8245FE6C, frame, call);
  SetRegs(call, Args({voice}));
  return Play(base, voice, thread.r13, call);
}

uint32_t MasteringPass(uint8_t* base, uint32_t voice, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245FDC8 XAudioMasteringVoice::Pass");
  SetRegs(call, Args({voice}));
  const uint32_t r31 = Pass(base, voice, FrameBelow(base, frame, 112), call);
  if (int32_t(r31) >= 0) {
    ClearBuffer(base, voice, 0x8245FE00, frame, call);
  }
  return r31;
}

uint32_t MasteringProcess(uint8_t* base, uint32_t voice, uint32_t inout, uint32_t processor,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245FB80 XAudioMasteringVoice::Process");
  SetRegs(call, Args({voice, inout}));
  const uint32_t r3 =
      ProcessEffects(base, voice, inout, processor, FrameBelow(base, frame, 112), call);
  if (int32_t(r3) < 0) {
    return r3;
  }
  const schema::XAudioMasteringDriver driver = MasteringVoice(base, voice).driver();
  return call(driver.vtable().submit(), 0x8245FBC4,
              At(frame, Args({driver.address, schema::XAudioMixInOut(base, inout).buffer(), 0})));
}

// --- Under the voice lists' spin lock ---

namespace {

// The node at `node` added at the end of the list at `list` (+0 next, +4
// prev), as the engine's inline code does.
void LinkAtEnd(uint8_t* base, uint32_t list, uint32_t node) {
  const List head(base, list), added(base, node);
  added.set_next(list);
  added.set_prev(head.prev());
  head.set_prev(node);
  List(base, added.prev()).set_next(node);
}

// The node at `node` out of its list when it's in one, left pointing at
// itself.
void Unlink(uint8_t* base, uint32_t node) {
  const List out(base, node);
  const uint32_t next = out.next();
  if (next == node) {
    return;
  }
  List(base, next).set_prev(out.prev());
  guest::StoreU32(base, out.prev(), out.next());
  out.set_prev(node);
  out.set_next(node);
}

}  // namespace

void StageSwap(uint8_t* base, uint32_t stage, uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245B208 XAudioStage::Swap");
  SpinTake(base, thread, call);
  const Stage lists(base, stage);
  const uint32_t mix = lists.to_mix().address;
  const uint32_t mixed = lists.mixed().address;
  lists.set_mixed(mix);
  lists.set_to_mix(mixed);
  SpinRelease(base, thread, call);
}

void SwapStageLists(uint8_t* base, uint32_t engine, uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245B720 XAudioEngine::SwapStageLists");
  SpinTake(base, thread, call);
  const Engine object(base, engine);
  SetRegs(call, Args({object.source_stage_address()}));
  StageSwap(base, object.source_stage_address(), thread, call);
  for (uint32_t i = 0, offset = 0; i < object.submix_stage_count(); i++, offset += 44) {
    const uint32_t stage = object.submix_stages() + offset;
    SetRegs(call, Args({stage}));
    StageSwap(base, stage, thread, call);
  }
  SpinRelease(base, thread, call);
}

void RunStage(uint8_t* base, uint32_t engine, uint32_t stage, uint32_t processor,
              uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245B480 XAudioEngine::RunStage");
  // The engine's working voice for the processor (+144 + 4 x it).
  const uint32_t entry = Engine(base, engine).working_voices_address() + (processor << 2);
  for (;;) {
    SpinTake(base, thread, call);
    const List list = Stage(base, stage).to_mix();
    const uint32_t first = list.next();
    if (first == list.address) {
      break;
    }
    const uint32_t offset = list.node_offset();
    const uint32_t voice = first - offset;
    if (voice == 0) {
      break;
    }
    Unlink(base, offset + voice);
    const List mixed = Stage(base, stage).mixed();
    LinkAtEnd(base, mixed.address, mixed.node_offset() + voice);
    guest::StoreU32(base, entry, voice);
    SpinRelease(base, thread, call);
    const uint32_t r3 = call(Method(base, voice, 68), 0x8245B5C4, Args({voice}));
    if (int32_t(r3) >= 0) {
      SetRegs(call, Args({0, 1}));
      AddStat(base, 0, 1);
    }
  }
  SpinRelease(base, thread, call);
  guest::StoreU32(base, entry, 0);
}

void RunCallbacks(uint8_t* base, uint32_t engine, uint32_t stage, uint32_t thread,
                  const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245ACE0 XAudioEngine::RunCallbacks");
  SpinTake(base, thread, call);
  const Engine object(base, engine);
  uint32_t slot = object.callbacks_address() + ((stage & 0xFF) << 6);
  for (uint32_t k = 0; k < 8; k++, slot += 8) {
    object.set_current_voice(slot);
    if (guest::LoadU32(base, slot) == 0) {
      continue;
    }
    SpinRelease(base, thread, call);
    const uint32_t context = guest::LoadU32(base, slot + 4);
    call(guest::LoadU32(base, slot), 0x8245ADB4, Args({context}));
    SpinTake(base, thread, call);
  }
  object.set_current_voice(0);
  SpinRelease(base, thread, call);
}

uint32_t RegisterCallback(uint8_t* base, uint32_t engine, uint32_t stage, uint32_t function,
                          uint32_t context, uint32_t out, uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245AB60 XAudioEngine::RegisterCallback");
  SpinTake(base, thread, call);
  const uint32_t index = stage & 0xFF;
  uint32_t i = 0;
  for (; i < 8; i++) {
    if (guest::LoadU32(base, Engine(base, engine).callbacks_address() + (index << 6) + 8 * i) == 0) {
      break;
    }
  }
  const uint32_t slot = i < 8 ? Engine(base, engine).callbacks_address() + (index << 6) + 8 * i : 0;
  if (slot == 0) {
    SpinRelease(base, thread, call);
    return 0x8007000Eu;  // E_OUTOFMEMORY
  }
  guest::StoreU32(base, slot, function);
  guest::StoreU32(base, slot + 4, context);
  if (out != 0) {
    guest::StoreU32(base, out, slot);
  }
  SpinRelease(base, thread, call);
  return 0;
}

uint32_t Commit(uint8_t* base, uint32_t engine, const Thread& thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245B638 XAudioEngine::Commit");
  uint32_t r27 = 0;
  SpinTake(base, thread.r13, call);
  const List list(base, Engine(base, engine).pending_next_address());  // the start-pending list
  for (;;) {
    const uint32_t first = list.next();
    if (first == list.address) {
      break;
    }
    const uint32_t voice = first - list.node_offset();
    if (voice == 0) {
      break;
    }
    SetRegs(call, Args({voice, 0}));
    r27 = SourceStart(base, voice, 0, thread, call);
    if (int32_t(r27) < 0) {
      break;
    }
  }
  SpinRelease(base, thread.r13, call);
  return r27;
}

uint32_t StageOf(uint8_t* base, uint32_t engine, uint32_t voice, uint32_t frame,
                 const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245FBF0 XAudioEngine::StageOf");
  const schema::XAudioMixVoice mixing(base, voice);
  call(mixing.vtable().get_format(), 0x8245FC24, At(frame, Args({voice, frame + 80})));
  const uint32_t type = guest::LoadU8(base, frame + 80);
  if (type < 1) {
    return Engine(base, engine).source_stage_address();
  }
  if (type == 1) {
    return mixing.submix_stage() * 44u + Engine(base, engine).submix_stages();
  }
  return 0;
}

uint32_t JoinMix(uint8_t* base, uint32_t voice, uint32_t thread, uint32_t frame,
                 const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460000 XAudioVoice::JoinMix");
  const uint32_t engine = LiveEngine(base);
  SetRegs(call, Args({engine, voice}));
  const uint32_t stage = StageOf(base, engine, voice, FrameBelow(base, frame, 112), call);
  if (stage == 0) {
    return 0;
  }
  SpinTake(base, thread, call);
  const List mixed = Stage(base, stage).mixed();
  LinkAtEnd(base, mixed.address, mixed.node_offset() + voice);
  SpinRelease(base, thread, call);
  return 0;
}

// --- The allocators' destructors ---

namespace {

// XAudio::Free on `block`: its plain C++ while the heap is the native
// layer's, else the original through `call`.
uint32_t FreeBlock(uint32_t block, uint32_t attributes, bool native_heap, uint32_t return_address,
                   uint32_t frame, const GuestCall& call) {
  SetRegs(call, Args({kFreeHeap, block, attributes}));
  if (native_heap) {
    return Free(block, attributes);
  }
  return call(kFree, return_address, At(frame, Args({kFreeHeap, block, attributes})));
}

}  // namespace

uint32_t AllocatorDestroy(uint8_t* base, uint32_t allocator, bool native_heap, uint32_t frame,
                          const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824602F8 XAudioAllocator::~XAudioAllocator");
  const Allocator a(base, allocator);
  a.set_vtable(kAllocatorVtable);
  a.set_interface_vtable(kAllocatorBaseVtable);
  uint32_t r3 = allocator;
  const uint32_t block = a.block();
  if (block != 0) {
    r3 = FreeBlock(block, a.attributes(), native_heap, 0x82460340, frame, call);
    a.set_block(0);
  }
  a.set_interface_vtable(kRootVtable);
  return r3;
}

uint32_t DefaultAllocatorDestroy(uint8_t* base, uint32_t allocator, bool native_heap,
                                 uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824604E8 XAudioDefaultAllocator::~XAudioDefaultAllocator");
  const Allocator a(base, allocator);
  a.set_vtable(kAllocatorVtable);
  a.set_interface_vtable(kAllocatorVtable2);
  a.set_block(0);
  SetRegs(call, Args({allocator}));
  AllocatorDestroy(base, allocator, native_heap, FrameBelow(base, frame, 96), call);
  return allocator;
}

uint32_t InterfaceFree(uint8_t* base, uint32_t iface, bool native_heap, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824603A8 XAudioAllocator::Free (interface)");
  const uint32_t object = iface - 4;
  const schema::XAudioAllocatorInterface object_iface(base, iface);
  const uint32_t attributes = object_iface.attributes();
  uint32_t r3 =
      call(object_iface.vtable().destructor(), 0x824603D8, At(frame, Args({iface, 1})));
  if (object != 0) {
    r3 = FreeBlock(object, attributes, native_heap, 0x824603F4, frame, call);
  }
  return r3;
}

// --- The mix and XAudio's threads ---

namespace {

constexpr uint32_t kSwapStageListsFrame = 128;

// The thread's word at the engine's +332 + 4 x its processor.
void StoreThread(uint8_t* base, const Thread& thread) {
  const Engine engine = schema::XaudioGlobals::engine(base);
  guest::StoreU32(base, engine.workers_address() + (thread.processor << 2), thread.mixer);
}

}  // namespace

uint32_t Mix(uint8_t* base, uint32_t engine, uint32_t frame, const Thread& thread,
             const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245BF80 XAudioEngine::Mix");
  vehicle_accessors::DebugSwitchOff();
  SetRegs(call, Args({0}));  // its r3
  call(kRtlEnterCriticalSection, 0x8245BFAC, Args({kMixerLock}));
  const Engine object(base, engine);
  const uint32_t threads = object.processor_count();
  object.set_mixer_thread(thread.mixer);
  bool terminating = false;
  if (int32_t(threads) != 0) {
    call(kKeSetEvent, 0x8245BFD4, Args({kMixNow, 1, 0}));
    guest::StoreU32(base, frame + 96, 0);
    guest::StoreU32(base, frame + 80, kMixDone);
    guest::StoreU32(base, frame + 84, kTerminating);
    // memset (sub_825A71D0) of 44 bytes from +100, its r3 the destination.
    for (uint32_t i = 0; i < 44; i++) {
      guest::StoreU8(base, frame + 100 + i, 0);
    }
    SetRegs(call, Args({frame + 100}));
    const uint32_t r3 = call(kKeWaitForMultipleObjects, 0x8245C024,
                             Args({2, frame + 80, 1, 3, 1, 0, 0, frame + 96}));
    terminating = int32_t(r3) == 1;
  } else {
    SetRegs(call, Args({engine}));
    FrameBelow(base, frame, kSwapStageListsFrame);
    SwapStageLists(base, engine, thread.r13, call);
    call(kMixPasses, 0x8245C048, Args({engine, 1}));
  }
  if (!terminating) {
    const uint32_t master = object.mastering_voice().address;
    const uint32_t r3 =
        call(MasteringVoice(base, master).vtable().pass(), 0x8245C068, Args({master}));
    if (int32_t(r3) >= 0) {
      SetRegs(call, Args({0, 1}));
      AddStat(base, 0, 1);
    }
  }
  object.set_mixer_thread(0);
  TakeStats(base);
  call(kRtlLeaveCriticalSection, 0x8245C08C, Args({kMixerLock}));
  return 0;
}

uint32_t MixerCallback(uint8_t* base, uint32_t frame, const Thread& thread,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245FBE0 XAudio::MixerCallback");
  const uint32_t engine = LiveEngine(base);
  SetRegs(call, Args({engine}));
  return Mix(base, engine, frame, thread, call);
}

uint32_t MasterThread(uint8_t* base, uint32_t frame, const Thread& thread,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245BE08 XAudio::MasterThread");
  StoreThread(base, thread);
  for (;;) {
    call(kKeWaitForSingleObject, 0x8245BE60, Args({kMixNow, 3, 1, 0, 0}));
    const uint32_t engine = LiveEngine(base);
    const bool ending = Engine(base, engine).mixer_thread() == 0;
    if (ending) {
      // r5 the count less one, as a 64-bit sum.
      const uint64_t count = uint64_t(Engine(base, engine).processor_count()) - 1;
      if (int32_t(count) != 0) {
        call(kKeReleaseSemaphore, 0x8245BE98, Args({kWorkerSemaphore, 1, count, 0}));
      }
    } else {
      SetRegs(call, Args({engine}));
      FrameBelow(base, frame, kSwapStageListsFrame);
      SwapStageLists(base, engine, thread.r13, call);
      call(kMixPasses, 0x8245BEAC, Args({LiveEngine(base), 1}));
    }
    call(kKeSetEvent, 0x8245BEBC, Args({kMixDone, 1, 0}));
    if (ending) {
      return 0;
    }
  }
}

uint32_t WorkerThread(uint8_t* base, const Thread& thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245BED0 XAudio::WorkerThread");
  StoreThread(base, thread);
  call(kKeWaitForSingleObject, 0x8245BF20, Args({kWorkerSemaphore, 3, 1, 0, 0}));
  for (;;) {
    const uint32_t engine = LiveEngine(base);
    if (Engine(base, engine).mixer_thread() == 0) {
      return 0;
    }
    call(kMixPasses, 0x8245BF38, Args({engine, 0}));
    call(kKeWaitForSingleObject, 0x8245BF50, Args({kWorkerSemaphore, 3, 1, 0, 0}));
  }
}

// --- The engine's making ---

namespace {

constexpr uint64_t kOutOfMemory = 0xFFFFFFFF8007000Eull;  // E_OUTOFMEMORY, as li/ori leave it
constexpr uint32_t kRegisterStatsFrame = 96;
constexpr uint32_t kFactoryCreateFrame = 112;
constexpr uint32_t kSetUpProcessorsFrame = 112;
constexpr uint32_t kBufferCreateFrame = 112;
constexpr uint32_t kMasteringCreateFrame = 128;
constexpr uint32_t kEngineSizeFrame = 176;
constexpr uint32_t kEngineConstructFrame = 96;
constexpr uint32_t kEngineInitFrame = 208;
constexpr uint32_t kEngineCreateFrame = 128;

// A guest call from `frame`, r3 in full as the callee leaves it.
uint64_t CallFull(const GuestCall& call, uint32_t function, uint32_t return_address, Regs regs,
                  uint32_t frame) {
  regs.sp = frame;
  call(function, return_address, regs);
  return LastCallR3();
}

// A dispatcher header as Init writes it inline: the type, not signaled,
// its wait list empty.
void InlineHeader(uint8_t* base, uint32_t address, uint8_t type) {
  guest::StoreU8(base, address, type);
  guest::StoreU32(base, address + 4, 0);
  guest::StoreU32(base, address + 8, address + 8);
  guest::StoreU32(base, address + 12, address + 8);
}

}  // namespace

uint64_t EngineInit(uint8_t* base, uint32_t engine, uint32_t params, bool native_heap,
                    uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245C198 XAudioEngine::Init");
  SetRegs(call, Args({engine, params}));
  RegisterStats(base, FrameBelow(base, frame, kRegisterStatsFrame), call);
  const Engine object(base, engine);
  const Engine init(base, params);  // the parameters (see EngineSize)
  const uint32_t count = init.params_stages();
  object.set_submix_stage_count(uint8_t(count));
  if (count != 0) {
    const uint32_t bytes = count * 44;
    const uint64_t size = count > 0x5D1745Du || bytes > 0xFFFFFFFBu ? ~0ull : uint64_t(bytes) + 4;
    const uint32_t allocator = object.allocator().address;
    const uint32_t memory = call(Allocator(base, allocator).vtable().allocate(), 0x8245C204,
                                 At(frame, Args({allocator, size})));
    uint32_t stages = 0;
    if (memory != 0) {
      guest::StoreU32(base, memory, count);
      stages = memory + 4;
      for (uint32_t i = 0; i < count; i++) {
        const Stage stage(base, stages + Stage::kSize * i);
        const uint32_t a = stage.list_a_next_address(), b = stage.list_b_next_address();
        stage.set_voices_next(stage.address);
        stage.set_voices_prev(stage.address);
        stage.set_voices_node_offset(16);
        stage.set_list_a_next(a);
        stage.set_list_a_prev(a);
        stage.set_list_a_node_offset(24);
        stage.set_list_b_next(b);
        stage.set_list_b_prev(b);
        stage.set_list_b_node_offset(24);
        stage.set_to_mix(a);
        stage.set_mixed(b);
      }
    }
    object.set_submix_stages(stages);
    if (stages == 0) {
      return kOutOfMemory;
    }
  }
  uint32_t table = init.ref_count();  // the parameters' +4 (the class table)
  if (table == 0) {
    table = kDefaultClasses;
  }
  guest::StoreU32(base, frame + 88, table);
  const uint32_t allocator = object.allocator().address;
  SetRegs(call, Args({frame + 88, allocator, object.factory_address()}));
  const uint32_t factory = FactoryCreate(base, frame + 88, allocator, object.factory_address(),
                                         FrameBelow(base, frame, kFactoryCreateFrame), call);
  // Sign-extended, as its adapter leaves r3 (E_OUTOFMEMORY as lis/ori do).
  uint64_t r25 = uint64_t(int64_t(int32_t(factory)));
  if (int32_t(factory) < 0) {
    return r25;
  }
  InlineHeader(base, kMixNow, 1);
  InlineHeader(base, kMixDone, 1);
  call(kKeInitializeSemaphore, 0x8245C340, At(frame, Args({kWorkerSemaphore, 0, 6})));
  InlineHeader(base, kTerminating, 0);
  guest::StoreU32(base, G::terminate_registration_address(), kTerminateRoutine);
  guest::StoreU32(base, G::terminate_registration_address() + 4, kTerminatePriority);
  call(kExRegisterTitleTerminateNotification, 0x8245C388,
       At(frame, Args({kTerminateRegistration, 1})));
  object.set_processor_count(0);
  const uint32_t processors = frame + 96;
  SetRegs(call, Args({init.params_processors(), processors}));
  SetUpProcessors(base, init.params_processors(), processors,
                  FrameBelow(base, frame, kSetUpProcessorsFrame), call);
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU32(base, frame + 84, 0);
    const uint32_t kind = guest::LoadU8(base, processors + 2 + i);
    if ((kind & 3) != 0) {
      const uint32_t entry = kind == 1 ? kMasterEntry : kWorkerEntry;
      // Suspended (bit 0), on hardware thread i (the top byte's bit i).
      const uint32_t flags = ((1u << i) << 24) | 1u;
      const uint32_t r3 = call(kExCreateThread, 0x8245C3F8,
                               At(frame, Args({frame + 84, 0, 0, 0, entry, 0, flags})));
      if (int32_t(r3) < 0) {
        return kOutOfMemory;
      }
      call(kObReferenceObjectByHandle, 0x8245C410,
           At(frame, Args({guest::LoadU32(base, frame + 84), G::thread_type(base), frame + 80})));
      const uint32_t thread = guest::LoadU32(base, frame + 80);
      call(kKeSetBasePriorityThread, 0x8245C41C, At(frame, Args({thread, 15})));
      call(kKeResumeThread, 0x8245C424, At(frame, Args({guest::LoadU32(base, frame + 80)})));
      call(kObDereferenceObject, 0x8245C42C, At(frame, Args({guest::LoadU32(base, frame + 80)})));
      object.set_processor_count(object.processor_count() + 1);
    }
    guest::StoreU32(base, object.processor_threads_address() + 4 * i, guest::LoadU32(base, frame + 84));
  }
  // The output buffers' format at the frame's +112 (20 bytes).
  const uint32_t format = frame + 112;
  for (uint32_t k = 0; k < 20; k += 4) {
    guest::StoreU32(base, format + k, 0);
  }
  uint32_t channels = init.params_channels();
  if (channels <= 6) {
    channels = 6;
  }
  guest::StoreU8(base, format + 5, uint8_t(channels));
  guest::StoreU32(base, format + 8, kSampleRate);
  for (uint32_t i = 0; i < 6; i++) {
    if (guest::LoadU8(base, processors + 2 + i) == 0) {
      continue;
    }
    for (uint32_t j = 0; j < 2 && int32_t(r25) >= 0; j++) {
      const uint32_t out = object.mix_buffers_address() + 8 * i + 4 * j;
      const uint32_t owner = object.allocator().address;
      SetRegs(call, Args({format, owner, out}));
      FrameBelow(base, frame, kBufferCreateFrame);
      r25 = BufferCreate(base, format, owner, out, call);
    }
  }
  if (int32_t(r25) < 0) {
    return r25;
  }
  const uint32_t description = guest::LoadU32(base, params + 8);
  SetRegs(call, Args({description, object.mastering_voice_address()}));
  const uint32_t r3 = MasteringCreate(base, description, object.mastering_voice_address(), native_heap,
                                      FrameBelow(base, frame, kMasteringCreateFrame), call);
  if (int32_t(r3) < 0) {
    return r3;
  }
  const schema::XAudioMasteringDriver driver =
      object.mastering_voice<MasteringVoice>().driver();
  return CallFull(call, driver.vtable().register_callback(), 0x8245C524,
                  Args({driver.address, kMixerCallback}), frame);
}

uint64_t EngineCreate(uint8_t* base, uint32_t params, uint32_t allocator, uint32_t out,
                      bool native_heap, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245C538 XAudioEngine::Create");
  const uint32_t slot = frame + 156;  // the caller's parameter word
  guest::StoreU32(base, slot, allocator);
  SetRegs(call, Args({params, frame + 80}));
  uint64_t r31 = EngineSize(base, params, frame + 80, FrameBelow(base, frame, kEngineSizeFrame),
                            call);
  const uint32_t size_word = guest::LoadU32(base, frame + 80);
  const uint32_t size = int32_t(r31) >= 0 ? size_word + 372 : size_word;
  if (int32_t(r31) >= 0) {
    SetRegs(call, Args({kEngineAttributes, size, slot}));
    if (const auto r3 = GetAllocator(base, kEngineAttributes, size, slot,
                                     FrameBelow(base, frame, 128), native_heap, call)) {
      r31 = *r3;
    } else {
      r31 = CallFull(call, kGetAllocator, 0x8245C58C, Args({kEngineAttributes, size, slot}),
                     frame);
    }
  }
  if (int32_t(r31) >= 0) {
    const uint32_t owner = guest::LoadU32(base, slot);
    const uint32_t memory =
        call(Allocator(base, owner).vtable().allocate(), 0x8245C5B0, At(frame, Args({owner, 372})));
    uint32_t engine = 0;
    if (memory != 0) {
      SetRegs(call, Args({memory, guest::LoadU32(base, slot)}));
      FrameBelow(base, frame, kEngineConstructFrame);
      engine = EngineConstruct(base, memory, guest::LoadU32(base, slot), call);
    }
    if (engine == 0) {
      r31 = kOutOfMemory;
    } else {
      SetRegs(call, Args({engine, params}));
      r31 = EngineInit(base, engine, params, native_heap, FrameBelow(base, frame, kEngineInitFrame),
                       call);
      if (int32_t(r31) >= 0) {
        schema::XAudioEngineOutWord(base, out).set_object(engine);
      } else {
        call(Method(base, engine, 12), 0x8245C60C, At(frame, Args({engine})));
      }
    }
  }
  const uint32_t owner = guest::LoadU32(base, slot);
  if (owner != 0) {
    call(Allocator(base, owner).vtable().release(), 0x8245C628, At(frame, Args({owner})));
  }
  return r31;
}

uint64_t CreateEngine(uint8_t* base, uint32_t params, uint32_t allocator, bool native_heap,
                      uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82459D60 XAudio::CreateEngine");
  constexpr uint32_t kEngineSlot = schema::XaudioGlobals::engine_address();
  SetRegs(call, Args({params, allocator, kEngineSlot}));
  return EngineCreate(base, params, allocator, kEngineSlot, native_heap,
                      FrameBelow(base, frame, kEngineCreateFrame), call);
}

// --- The API entry points over the engine ---

uint64_t ApiRegisterCallback(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6,
                             uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A5E8 XAudioRegisterProcessingCallback");
  ApiEnter(base, call);
  const uint32_t engine = LiveEngine(base);
  if (engine == 0) {
    ApiLeave(base, call);
    return 0xFFFFFFFF80004005ull;  // E_FAIL
  }
  SetRegs(call, Args({engine, r3, r4, r5, r6}));
  const uint32_t result = RegisterCallback(base, engine, uint32_t(r3), uint32_t(r4),
                                           uint32_t(r5), uint32_t(r6), thread, call);
  ApiLeave(base, call);
  return uint64_t(int64_t(int32_t(result)));  // 0 or E_OUTOFMEMORY as lis/ori leave it
}

uint64_t ApiCommit(uint8_t* base, const Thread& thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A6E8 XAudio::Commit");
  ApiEnter(base, call);
  const uint32_t engine = LiveEngine(base);
  if (engine == 0) {
    ApiLeave(base, call);
    return 0xFFFFFFFF80004005ull;  // E_FAIL
  }
  SetRegs(call, Args({engine}));
  const uint32_t result = Commit(base, engine, thread, call);
  ApiLeave(base, call);
  return result;
}

uint64_t ApiInitialize(uint8_t* base, uint32_t params, uint32_t allocator, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A410 XAudioInitialize");
  constexpr uint32_t kCreateTable = 0x826002AC, kCreateTableEnd = 0x826002B0;
  call(kRtlEnterCriticalSection, 0x8245A434, Args({kApiLock}));
  const uint32_t engine = LiveEngine(base);
  if (engine != 0) {
    call(Method(base, engine, 4), 0x8245A454, Args({engine}));  // AddRef
    call(kRtlLeaveCriticalSection, 0x8245A45C, Args({kApiLock}));
    return 0;
  }
  if (params == 0) {
    params = frame + 80;
    for (uint32_t i = 0; i < 12; i += 4) {
      guest::StoreU32(base, params + i, 0);
    }
  }
  uint32_t slot = kCreateTable;
  while (slot < kCreateTableEnd && guest::LoadU32(base, slot) == 0) {
    slot += 4;
  }
  call(guest::LoadU32(base, slot), 0x8245A4CC,
       Args({slot, kCreateTableEnd, params, allocator}));
  const uint64_t r3 = LastCallR3();
  call(kRtlLeaveCriticalSection, 0x8245A4D8, Args({kApiLock}));
  return r3;
}

}  // namespace xaudio
