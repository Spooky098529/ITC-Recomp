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

}  // namespace xaudio
