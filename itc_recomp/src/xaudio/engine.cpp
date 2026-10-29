// XAudio's engine and mixer as plain C++ (engine.h;
// docs/functions/xaudio-engine.md).

#include "xaudio/engine.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

#include <atomic>

namespace xaudio {

namespace {

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

}  // namespace

// --- Leaves ---

uint32_t FactoryTableSize(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_8245D9E8 XAudioFactory::TableSize");
  if (table == 0) {
    return 0;
  }
  const uint32_t classes = guest::LoadU32(base, table);
  if (classes == 0) {
    return 0;
  }
  const uint32_t count = guest::LoadU8(base, classes);
  uint32_t ids = 0;
  if (count != 0) {
    uint32_t entry = guest::LoadU32(base, classes + 4);
    for (uint32_t i = 0; i < count; i++, entry += 12) {
      const uint32_t id = guest::LoadU8(base, entry) + 1u;
      if (id > ids) {
        ids = id;
      }
    }
  }
  return ids << 3;
}

uint32_t FactorySizeOf(uint8_t* base, uint32_t table, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245DBD0 XAudioFactory::SizeOf");
  guest::StoreU32(base, out, FactoryTableSize(base, table) + 24);
  return 0;
}

uint32_t BufferGetFormat(uint8_t* base, uint32_t buffer, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245F6F0 XAudioBuffer::GetFormat");
  guest::StoreU32(base, out, guest::LoadU32(base, buffer + 20));
  guest::StoreU32(base, out + 4, guest::LoadU32(base, buffer + 24));
  guest::StoreU32(base, out + 8, guest::LoadU32(base, buffer + 28));
  return 0;
}

uint32_t BufferGetFormatThunk(uint8_t* base, uint32_t iface, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245F718 XAudioBuffer::GetFormatThunk");
  const uint32_t buffer = iface != 0 ? iface - 8 : 0;
  guest::StoreU32(base, out, guest::LoadU32(base, buffer + 20));
  guest::StoreU32(base, out + 4, guest::LoadU32(base, buffer + 24));
  guest::StoreU32(base, out + 8, guest::LoadU32(base, buffer + 28));
  return 0;
}

void BufferInterface(uint8_t* base, uint32_t buffer, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245F8D0 XAudioBuffer::GetInterface");
  guest::StoreU32(base, out, buffer != 0 ? buffer + 8 : 0);
}

uint64_t AllocatorRemaining(uint8_t* base, uint32_t allocator) {
  ITC_NATIVE_RUNNING("sub_82460388 XAudioAllocator::Remaining");
  return uint64_t(guest::LoadU32(base, allocator + 16)) -
         uint64_t(guest::LoadU32(base, allocator + 20));
}

uint64_t InterfaceAddRef(uint8_t* base, uint32_t iface) {
  ITC_NATIVE_RUNNING("sub_82460370 XAudioAllocator::AddRef (interface)");
  const uint64_t count = uint64_t(guest::LoadU32(base, iface + 8)) + 1;
  guest::StoreU32(base, iface + 8, uint32_t(count));
  return count;
}

uint32_t ObjectInfo(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82461EF8 (an object's info)");
  guest::StoreU8(base, out, 6);
  guest::StoreU16(base, out + 2, 3750);
  return 0;
}

uint32_t TakeStats(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8245F678 XAudioEngine::TakeStats");
  for (uint32_t i = 0; i < kStatCount * 4; i += 4) {
    const uint32_t value = guest::LoadU32(base, kStats + i);
    guest::StoreU32(base, kStats + i, 0);
    guest::StoreU32(base, kStatsCopy + i, value);
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
  const uint32_t left = call(Method(base, allocator, 16), 0x824602B8, Args({allocator}));
  if (rounded > left) {
    return 0;
  }
  const uint32_t used = guest::LoadU32(base, allocator + 20);
  const uint64_t block = uint64_t(guest::LoadU32(base, allocator + 24)) + used;
  guest::StoreU32(base, allocator + 20, used + rounded);
  return block;
}

void DeleteObject(uint8_t* base, uint32_t object, uint32_t* function) {
  ITC_NATIVE_RUNNING("sub_8245AAB8 XAudioObject::Delete");
  *function = object != 0 ? Method(base, object, 0) : 0;
}

void Barrier(uint8_t* base, uint32_t engine, uint32_t barrier, uint32_t processor) {
  ITC_NATIVE_RUNNING("sub_8245A940 XAudioEngine::Barrier");
  if (guest::LoadU32(base, engine + 304) == 0) {
    return;
  }
  guest::StoreU8(base, barrier + processor, 1);
  // Each processor's byte, processor 0 the most significant (the word is
  // big-endian, byte i at +i), read from the last thread to the first as the
  // original does.
  uint64_t all = 0;
  for (int i = 5; i >= 0; i--) {
    const uint64_t has = guest::LoadU32(base, engine + 308 + 4 * uint32_t(i)) != 0 ? 1 : 0;
    all |= has << (56 - 8 * i);
  }
  // The original's spin (db16cyc in the loop, nothing in the translation):
  // REX_LOAD_U64's volatile loads, as the recompiled processors read it.
  uint64_t word = REX_LOAD_U64(barrier);
  if (word == 0) {
    return;
  }
  while (word != all) {
    word = REX_LOAD_U64(barrier);
    if (word == 0) {
      return;
    }
  }
  REX_STORE_U64(barrier, uint64_t(0));
}

// --- The engine's making ---

uint32_t EngineConstruct(uint8_t* base, uint32_t engine, uint32_t allocator,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245C098 XAudioEngine::XAudioEngine");
  guest::StoreU32(base, engine + 8, allocator);
  guest::StoreU32(base, engine, kEngineVtable0);
  guest::StoreU32(base, engine + 4, 1);
  if (allocator != 0) {
    call(Method(base, allocator, 0), 0x8245C0E0, Args({allocator}));
  }
  guest::StoreU32(base, engine, kEngineVtable);
  // The start-pending list (+68, its node offset 24 at +76).
  const uint32_t pending = engine + 68;
  guest::StoreU32(base, pending, pending);
  guest::StoreU32(base, pending + 8, 24);
  guest::StoreU32(base, pending + 4, pending);
  // The source voices' stage (+80: its list, node offset 16; the two lists
  // at +92 and +104, node offset 24; +116 and +120 pointing at them).
  const uint32_t stage = engine + 80;
  const uint32_t active = stage + 12, next = stage + 24;
  guest::StoreU32(base, stage + 8, 16);
  guest::StoreU32(base, stage, stage);
  guest::StoreU32(base, stage + 4, stage);
  guest::StoreU32(base, active + 8, 24);
  guest::StoreU32(base, active, active);
  guest::StoreU32(base, active + 4, active);
  guest::StoreU32(base, next + 8, 24);
  guest::StoreU32(base, next, next);
  guest::StoreU32(base, next + 4, next);
  guest::StoreU32(base, stage + 36, active);
  guest::StoreU32(base, stage + 40, next);
  guest::StoreU32(base, engine + 132, kOneFloatBits);
  guest::StoreU32(base, engine + 136, kOneFloatBits);
  guest::StoreU32(base, kEngine, engine);
  return engine;
}

uint32_t FactoryConstruct(uint8_t* base, uint32_t factory, uint32_t table, uint32_t allocator,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245DA48 XAudioFactory::XAudioFactory");
  guest::StoreU32(base, factory, kFactoryVtable);
  guest::StoreU32(base, factory + 4, 1);
  uint32_t classes = guest::LoadU32(base, table);
  uint32_t count = guest::LoadU8(base, classes);
  uint32_t ids = 0;
  if (count != 0) {
    uint32_t entry = guest::LoadU32(base, classes + 4);
    for (; count != 0; count--, entry += 12) {
      const uint32_t id = guest::LoadU8(base, entry) + 1u;
      if (id > ids) {
        ids = id;
      }
    }
  }
  guest::StoreU32(base, factory + 8, ids);
  if (ids != 0) {
    const uint32_t entries =
        call(Method(base, allocator, 20), 0x8245DAD8, At(frame, Args({allocator, ids << 3})));
    guest::StoreU32(base, factory + 12, entries);
  }
  // Each class's functions at its id's entry, the table read again each
  // time, as the original does.
  classes = guest::LoadU32(base, table);
  if (guest::LoadU8(base, classes) != 0) {
    uint32_t offset = 0;
    uint32_t i = 0;
    do {
      const uint32_t entry = guest::LoadU32(base, classes + 4) + offset;
      i++;
      const uint32_t at = uint32_t(guest::LoadU8(base, entry)) << 3;
      guest::StoreU32(base, guest::LoadU32(base, factory + 12) + at,
                      guest::LoadU32(base, entry + 4));
      const uint32_t again = guest::LoadU32(base, table);
      const uint32_t to = guest::LoadU32(base, factory + 12) + at;
      const uint32_t second = guest::LoadU32(base, guest::LoadU32(base, again + 4) + offset + 8);
      offset += 12;
      guest::StoreU32(base, to + 4, second);
      classes = guest::LoadU32(base, table);
    } while (i < guest::LoadU8(base, classes));
  }
  return factory;
}

uint32_t FactoryCreate(uint8_t* base, uint32_t table, uint32_t allocator, uint32_t out,
                       uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245DC10 XAudioFactory::Create");
  const uint32_t memory =
      call(Method(base, allocator, 20), 0x8245DC40, At(frame, Args({allocator, 24})));
  if (memory != 0) {
    SetRegs(call, Args({memory, table, allocator}));
    const uint32_t factory =
        FactoryConstruct(base, memory, table, allocator, FrameBelow(base, frame, 112), call);
    if (factory != 0) {
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
  uint32_t id = guest::LoadU8(base, descriptor);
  uint32_t entries;
  if (id <= 127) {
    if (id >= guest::LoadU32(base, factory + 8)) {
      return 0x80004001u;
    }
    entries = guest::LoadU32(base, factory + 12);
  } else {
    id -= 128;
    if (id >= guest::LoadU32(base, factory + 16)) {
      return 0x80004001u;
    }
    entries = guest::LoadU32(base, factory + 20);
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
  const uint32_t monitor = guest::LoadU32(base, guest::LoadU32(base, kDebugMonitor));
  if (monitor != 0) {
    call(guest::LoadU32(base, monitor + 24), 0x8245F5D0, At(frame, Args({47, frame + 80})));
  }
  return 0;
}

uint32_t SetUpProcessors(uint8_t* base, uint32_t mask, uint32_t out, uint32_t frame,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A828 XAudio::SetUpProcessors");
  mask &= 0x3F;
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU8(base, out + i, 0);
  }
  // The registers as the original's call leaves them (r3 the mask, r4 out).
  const uint32_t type = call(kKeGetCurrentProcessType, 0x8245A864, At(frame, Regs{}));
  if (int32_t(type) != 1) {
    guest::StoreU8(base, out + 1, 1);
    guest::StoreU8(base, out + 6, 4);
    return type;
  }
  if ((mask & 0xFF) == 0) {
    mask = 16;
  }
  mask &= 0xFF;
  for (uint32_t i = 0, bits = mask; i < 6; i++, bits >>= 1) {
    if ((bits & 1) != 0) {
      guest::StoreU8(base, out + 2 + i, 2);
      guest::StoreU8(base, out, uint8_t(guest::LoadU8(base, out) + 1));
      guest::StoreU8(base, out + 1, uint8_t(guest::LoadU8(base, out + 1) + 1));
    }
  }
  if ((mask & 0x10) != 0) {
    guest::StoreU8(base, out + 6, 1);
    return type;
  }
  for (int i = 5; i >= 0; i--) {
    if (guest::LoadU8(base, out + 2 + uint32_t(i)) != 0) {
      guest::StoreU8(base, out + 2 + uint32_t(i), 1);
      return type;
    }
  }
  return type;
}

uint32_t EngineSize(uint8_t* base, uint32_t params, uint32_t out, uint32_t frame,
                    const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245B2C0 XAudioEngine::Size");
  uint32_t classes = guest::LoadU32(base, params + 4);
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
  REX_STORE_U64(format, uint64_t(0));
  REX_STORE_U64(format + 8, uint64_t(0));
  guest::StoreU32(base, format + 16, 0);
  guest::StoreU8(base, format, 0);
  guest::StoreU8(base, format + 4, 0);
  uint32_t channels = guest::LoadU8(base, params);
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
  const uint32_t mask = guest::LoadU8(base, params + 2);
  SetRegs(call, Args({mask, frame + 96}));
  SetUpProcessors(base, mask, frame + 96, FrameBelow(base, frame, 112), call);
  const uint32_t processors = guest::LoadU8(base, frame + 97);
  uint32_t size = (processors * guest::LoadU32(base, frame + 84)) << 1;
  size += guest::LoadU32(base, frame + 88);
  const uint32_t stages = guest::LoadU8(base, params + 1);
  if (stages != 0) {
    size += stages * 44 + 4;
  }
  guest::StoreU32(base, out, size);
  return r30;
}

// --- The mastering voice ---

namespace {

// The 32-byte voice description the mastering voice's Size and Init build
// at `at`: its first 24 bytes cleared, 6 channels at 48,000 Hz, the
// description's three words (when given) at +8, +16 and +20; +24 on as the
// stack had them.
void MasteringDescription(uint8_t* base, uint32_t at, uint32_t description) {
  REX_STORE_U64(at, uint64_t(0));
  REX_STORE_U64(at + 8, uint64_t(0));
  REX_STORE_U64(at + 16, uint64_t(0));
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
  const uint32_t iface = guest::LoadU32(base, voice + 32);
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
    guest::StoreU32(base, out, guest::LoadU32(base, frame + 80) + 20);
  }
  return r3;
}

void RenderDriverConstruct(uint8_t* base, uint32_t driver, uint32_t description) {
  ITC_NATIVE_RUNNING("sub_82465938 AudioRenderDriver::AudioRenderDriver");
  guest::StoreU32(base, driver + 8, 1);
  guest::StoreU32(base, driver + 4, kDriverVtableFirst);
  guest::StoreU32(base, driver + 12, guest::LoadU32(base, description + 4));
  guest::StoreU32(base, driver, kDriverVtable);
  guest::StoreU32(base, driver + 4, kDriverVtable2);
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
  REX_STORE_U64(driver_description, uint64_t(0));
  guest::StoreU32(base, driver_description + 8, 0);
  guest::StoreU32(base, driver_description + 4, voice);
  guest::StoreU8(base, driver_description, 2);
  const uint32_t allocator = guest::LoadU32(base, voice + 8);
  const uint32_t memory =
      call(Method(base, allocator, 20), 0x8245FD48, At(frame, Args({allocator, 20})));
  uint32_t driver = 0;
  if (memory != 0) {
    SetRegs(call, Args({memory, driver_description, guest::LoadU32(base, voice + 8)}));
    RenderDriverConstruct(base, memory, driver_description);
    driver = memory;
  }
  guest::StoreU32(base, voice + 68, driver);
  if (driver == 0) {
    return 0x8007000Eu;  // E_OUTOFMEMORY
  }
  r3 = call(Method(base, driver, 32), 0x8245FD94,
            At(frame, Args({driver, driver_description, guest::LoadU32(base, voice + 8)})));
  if (int32_t(r3) < 0) {
    return r3;
  }
  return call(Method(base, voice, 56), 0x8245FDB0, At(frame, Args({voice})));
}

uint32_t MasteringCreate(uint8_t* base, uint32_t description, uint32_t out, bool native_heap,
                         uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824600F0 XAudioMasteringVoice::Create");
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
      const uint32_t memory =
          call(Method(base, allocator, 20), 0x8246016C, At(frame, Args({allocator, 72})));
      if (memory == 0) {
        r30 = 0x8007000Eu;  // E_OUTOFMEMORY
      } else {
        SetRegs(call, Args({memory, guest::LoadU32(base, slot), 2}));
        VoiceConstruct(base, memory, guest::LoadU32(base, slot), 2, call);
        guest::StoreU32(base, memory, kMasteringVtable);
        SetRegs(call, Args({memory, description}));
        r30 = MasteringInit(base, memory, description, FrameBelow(base, frame, 144), call);
        if (int32_t(r30) >= 0) {
          guest::StoreU32(base, out, memory);
        } else {
          call(Method(base, memory, 12), 0x824601D0, At(frame, Args({memory})));
        }
      }
    }
  }
  const uint32_t allocator = guest::LoadU32(base, slot);
  if (allocator != 0) {
    call(Method(base, allocator, 4), 0x824601EC, At(frame, Args({allocator})));
  }
  return r30;
}

uint32_t MasteringStart(uint8_t* base, uint32_t voice, uint32_t frame, const Thread& thread,
                        const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245FE20 XAudioMasteringVoice::Start");
  if ((guest::LoadU8(base, voice + 61) & 1) != 0) {
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
  const uint32_t driver = guest::LoadU32(base, voice + 68);
  return call(Method(base, driver, 24), 0x8245FBC4,
              At(frame, Args({driver, guest::LoadU32(base, inout), 0})));
}

// --- Under the voice lists' spin lock ---

namespace {

// The node at `node` added at the end of the list at `list` (+0 next, +4
// prev), as the engine's inline code does.
void LinkAtEnd(uint8_t* base, uint32_t list, uint32_t node) {
  guest::StoreU32(base, node, list);
  guest::StoreU32(base, node + 4, guest::LoadU32(base, list + 4));
  guest::StoreU32(base, list + 4, node);
  guest::StoreU32(base, guest::LoadU32(base, node + 4), node);
}

// The node at `node` out of its list when it's in one, left pointing at
// itself.
void Unlink(uint8_t* base, uint32_t node) {
  const uint32_t next = guest::LoadU32(base, node);
  if (next == node) {
    return;
  }
  guest::StoreU32(base, next + 4, guest::LoadU32(base, node + 4));
  guest::StoreU32(base, guest::LoadU32(base, node + 4), guest::LoadU32(base, node));
  guest::StoreU32(base, node + 4, node);
  guest::StoreU32(base, node, node);
}

}  // namespace

void StageSwap(uint8_t* base, uint32_t stage, uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245B208 XAudioStage::Swap");
  SpinTake(base, thread, call);
  const uint32_t mix = guest::LoadU32(base, stage + 36);
  const uint32_t mixed = guest::LoadU32(base, stage + 40);
  guest::StoreU32(base, stage + 40, mix);
  guest::StoreU32(base, stage + 36, mixed);
  SpinRelease(base, thread, call);
}

void SwapStageLists(uint8_t* base, uint32_t engine, uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245B720 XAudioEngine::SwapStageLists");
  SpinTake(base, thread, call);
  SetRegs(call, Args({engine + 80}));
  StageSwap(base, engine + 80, thread, call);
  for (uint32_t i = 0, offset = 0; i < guest::LoadU8(base, engine + 128); i++, offset += 44) {
    const uint32_t stage = guest::LoadU32(base, engine + 124) + offset;
    SetRegs(call, Args({stage}));
    StageSwap(base, stage, thread, call);
  }
  SpinRelease(base, thread, call);
}

void RunStage(uint8_t* base, uint32_t engine, uint32_t stage, uint32_t processor,
              uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245B480 XAudioEngine::RunStage");
  const uint32_t entry = engine + ((processor + 36) << 2);
  for (;;) {
    SpinTake(base, thread, call);
    const uint32_t list = guest::LoadU32(base, stage + 36);
    const uint32_t first = guest::LoadU32(base, list);
    if (first == list) {
      break;
    }
    const uint32_t offset = guest::LoadU32(base, list + 8);
    const uint32_t voice = first - offset;
    if (voice == 0) {
      break;
    }
    Unlink(base, offset + voice);
    const uint32_t mixed = guest::LoadU32(base, stage + 40);
    LinkAtEnd(base, mixed, guest::LoadU32(base, mixed + 8) + voice);
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
  uint32_t slot = engine + ((stage & 0xFF) << 6) + 168;
  for (uint32_t k = 0; k < 8; k++, slot += 8) {
    guest::StoreU32(base, engine + 296, slot);
    if (guest::LoadU32(base, slot) == 0) {
      continue;
    }
    SpinRelease(base, thread, call);
    const uint32_t context = guest::LoadU32(base, slot + 4);
    call(guest::LoadU32(base, slot), 0x8245ADB4, Args({context}));
    SpinTake(base, thread, call);
  }
  guest::StoreU32(base, engine + 296, 0);
  SpinRelease(base, thread, call);
}

uint32_t RegisterCallback(uint8_t* base, uint32_t engine, uint32_t stage, uint32_t function,
                          uint32_t context, uint32_t out, uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245AB60 XAudioEngine::RegisterCallback");
  SpinTake(base, thread, call);
  const uint32_t index = stage & 0xFF;
  uint32_t i = 0;
  for (; i < 8; i++) {
    if (guest::LoadU32(base, engine + (index << 6) + 168 + 8 * i) == 0) {
      break;
    }
  }
  const uint32_t slot = i < 8 ? (((index << 3) + i + 21) << 3) + engine : 0;
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
  const uint32_t list = engine + 68;
  for (;;) {
    const uint32_t first = guest::LoadU32(base, list);
    if (first == list) {
      break;
    }
    const uint32_t voice = first - guest::LoadU32(base, list + 8);
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
  call(Method(base, voice, 52), 0x8245FC24, At(frame, Args({voice, frame + 80})));
  const uint32_t type = guest::LoadU8(base, frame + 80);
  if (type < 1) {
    return engine + 80;
  }
  if (type == 1) {
    return guest::LoadU8(base, voice + 76) * 44u + guest::LoadU32(base, engine + 124);
  }
  return 0;
}

uint32_t JoinMix(uint8_t* base, uint32_t voice, uint32_t thread, uint32_t frame,
                 const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460000 XAudioVoice::JoinMix");
  const uint32_t engine = guest::LoadU32(base, kEngine);
  SetRegs(call, Args({engine, voice}));
  const uint32_t stage = StageOf(base, engine, voice, FrameBelow(base, frame, 112), call);
  if (stage == 0) {
    return 0;
  }
  SpinTake(base, thread, call);
  const uint32_t mixed = guest::LoadU32(base, stage + 40);
  LinkAtEnd(base, mixed, guest::LoadU32(base, mixed + 8) + voice);
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
  guest::StoreU32(base, allocator, kAllocatorVtable);
  guest::StoreU32(base, allocator + 4, kAllocatorBaseVtable);
  uint32_t r3 = allocator;
  const uint32_t block = guest::LoadU32(base, allocator + 24);
  if (block != 0) {
    r3 = FreeBlock(block, guest::LoadU32(base, allocator + 12), native_heap, 0x82460340, frame,
                   call);
    guest::StoreU32(base, allocator + 24, 0);
  }
  guest::StoreU32(base, allocator + 4, kRootVtable);
  return r3;
}

uint32_t DefaultAllocatorDestroy(uint8_t* base, uint32_t allocator, bool native_heap,
                                 uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824604E8 XAudioDefaultAllocator::~XAudioDefaultAllocator");
  guest::StoreU32(base, allocator, kAllocatorVtable);
  guest::StoreU32(base, allocator + 4, kAllocatorVtable2);
  guest::StoreU32(base, allocator + 24, 0);
  SetRegs(call, Args({allocator}));
  AllocatorDestroy(base, allocator, native_heap, FrameBelow(base, frame, 96), call);
  return allocator;
}

uint32_t InterfaceFree(uint8_t* base, uint32_t iface, bool native_heap, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824603A8 XAudioAllocator::Free (interface)");
  const uint32_t object = iface - 4;
  const uint32_t attributes = guest::LoadU32(base, iface + 8);
  uint32_t r3 = call(Method(base, iface, 0), 0x824603D8, At(frame, Args({iface, 1})));
  if (object != 0) {
    r3 = FreeBlock(object, attributes, native_heap, 0x824603F4, frame, call);
  }
  return r3;
}

}  // namespace xaudio
