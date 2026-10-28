// The archive streaming's functions as plain C++: see archive.h.

#include "archive/archive.h"

#include "archive/semaphores.h"

#include "base/log.h"
#include "guest.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "native/buffers.h"

namespace archive {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Toc(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8209D868 Archive::Toc");
  return LoadU32(base, id < kFirstCrsId ? kBuildToc : kCrsToc);
}

uint32_t AssetSize(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_820A0678 Archive::AssetSize");
  const uint32_t index = id < kFirstCrsId ? id : id - kFirstCrsId;
  const uint32_t entry = Toc(base, id) + ((index + 1) << 4);
  const uint32_t unpacked = LoadU32(base, entry + 8);
  return unpacked != 0 ? unpacked : LoadU32(base, entry + 4);
}

// --- The request queue's list and slots ---

void ListConstruct(uint8_t* base, uint32_t list, uint32_t deletes) {
  ITC_NATIVE_RUNNING("sub_82294B80 List::List");
  StoreU32(base, list + 4, deletes);
  StoreU32(base, list + 0, kListVtable);
  StoreU32(base, list + 8, 0);
  StoreU32(base, list + 12, 0);
  StoreU32(base, list + 16, 0);
}

uint32_t ListAppend(uint8_t* base, uint32_t list, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_82294BA8 List::Append");
  if (node == 0) {
    return 0;
  }
  const uint32_t tail = LoadU32(base, list + 16);
  if (tail != 0) {
    StoreU32(base, tail + 4, node);
  } else {
    StoreU32(base, list + 12, node);
  }
  StoreU32(base, node + 4, 0);
  StoreU32(base, list + 16, node);
  StoreU32(base, list + 8, LoadU32(base, list + 8) + 1);
  return 1;
}

uint32_t ListRemove(uint8_t* base, uint32_t list, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_82294C00 List::Remove");
  if (node == 0) {
    return 0;
  }
  uint32_t previous = 0;
  uint32_t at = LoadU32(base, list + 12);
  while (at != 0 && at != node) {
    previous = at;
    at = LoadU32(base, at + 4);
  }
  if (at == 0) {
    return 0;
  }
  if (previous != 0) {
    StoreU32(base, previous + 4, LoadU32(base, at + 4));
  }
  if (LoadU32(base, list + 12) == at) {
    StoreU32(base, list + 12, LoadU32(base, at + 4));
  }
  if (LoadU32(base, list + 16) == at) {
    StoreU32(base, list + 16, previous);
  }
  StoreU32(base, list + 8, LoadU32(base, list + 8) - 1);
  return 1;
}

void NodeConstruct(uint8_t* base, uint32_t node, uint32_t link) {
  ITC_NATIVE_RUNNING("sub_82294B68 ListNode::ListNode");
  StoreU32(base, node + 4, link);
  StoreU32(base, node + 0, kNodeVtable);
}

uint32_t FindSlot(uint8_t* base, uint32_t serial) {
  ITC_NATIVE_RUNNING("sub_8209D818 Archive::FindSlot");
  for (uint32_t i = 0; i < kSlotCount; i++) {
    const uint32_t slot = kSlots + i * kSlotBytes;
    if (LoadU32(base, slot + 8) == serial) {
      return slot;
    }
  }
  return 0;
}

int32_t QueueStaticInit(uint8_t* base, const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825D9160 RequestQueue::StaticInit");
  ListConstruct(base, kQueue, 0);
  StoreU32(base, kQueue, kQueueVtable);
  return c_runtime::AtexitFrom(base, kQueueCleanUp, call);
}

int32_t SlotsStaticInit(uint8_t* base, const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825D91B0 RequestSlots::StaticInit");
  for (uint32_t i = 0; i < kSlotCount; i++) {
    const uint32_t slot = kSlots + i * kSlotBytes;
    NodeConstruct(base, slot, 0);
    StoreU32(base, slot, kSlotVtable);
  }
  return c_runtime::AtexitFrom(base, kSlotsCleanUp, call);
}

// --- The loose files ---

uint32_t FindLooseFile(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8209D8D8 Archive::FindLooseFile");
  for (uint32_t step = 0;; step++) {
    const uint32_t entry = (step < kFirstCrsId ? kLooseFiles : kCrsLooseFiles) + 8 * step;
    const uint32_t path = LoadU32(base, entry + 4);
    if (path == 0) {
      return 0;
    }
    if (LoadU32(base, entry) == id) {
      return path;
    }
  }
}

// --- The loader thread's pieces ---

std::optional<uint32_t> FinishRequest(uint8_t* base, uint32_t request,
                                      const FinishCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209D688 Archive::FinishRequest");
  uint32_t flags = LoadU32(base, request + 12);
  if ((flags & 0x100) && !(flags & 0x8)) {
    StoreU32(base, kStagingInUse, LoadU32(base, request + 76));
    StoreU32(base, request + 76, LoadU32(base, request + 88));
  }
  if (LoadU32(base, request + 12) & 0x2000) {
    const uint32_t serial = LoadU32(base, request + 8);
    StoreU32(base, kCallbackSerial, serial);
    calls.callback(calls.user, LoadU32(base, request + 48), serial, LoadU32(base, request + 76),
                   LoadU32(base, request + 32), LoadU32(base, request + 44));
  }
  flags = LoadU32(base, request + 12);
  if ((flags & 0x2) && !(flags & 0x1000)) {
    constexpr uint32_t kMemDecommit = 0x4000;
    const uint32_t data = LoadU32(base, request + 76);
    auto r3 = hooks::VirtualFree(data, 0, kMemDecommit, 0x8209D71C);
    if (!r3) {
      calls.guest_free(calls.user, data, 0, kMemDecommit);
    }
    return r3;
  }
  return std::nullopt;
}

uint32_t LoaderFunctorTarget(uint8_t* base, uint32_t functor) {
  ITC_NATIVE_RUNNING("sub_8209DD80 LoaderFunctor::Invoke");
  return LoadU32(base, functor + 4);
}

// --- The queue and the loader under the four semaphores ---

namespace {

HostSemaphore& Lock() {
  return CurrentSemaphores().queue_lock;
}

// A request taken off the queue and its slot freed (the picker's and the
// cancels' steps).
void Unqueue(uint8_t* base, uint32_t request) {
  ListRemove(base, kQueue, request);
  StoreU32(base, request + 8, 0xFFFFFFFFu);
}

}  // namespace

LoadRequest BuildLoad(uint8_t* base, uint32_t id, uint32_t destination, uint32_t callback,
                      uint32_t context, uint32_t r7, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_8209DE48 Archive::Load");
  const uint32_t index = id < kFirstCrsId ? id : id - kFirstCrsId;
  const uint32_t entry = LoadU32(base, id < kFirstCrsId ? kBuildToc : kCrsToc) + ((index + 1) << 4);
  const uint32_t unpacked = LoadU32(base, entry + 8);
  uint32_t flags = unpacked != 0 ? 0x4124 : 0x4024;
  const uint32_t loose = FindLooseFile(base, id);
  uint32_t words[10];
  words[1] = id;
  words[2] = loose;
  words[3] = LoadU32(base, entry) << 11;
  words[4] = r7;
  words[7] = destination;
  words[8] = context;
  words[9] = callback;
  if (flags & 0x100) {
    words[5] = unpacked;
    words[6] = LoadU32(base, entry + 4);
  } else {
    words[5] = size != 0 ? size : LoadU32(base, entry + 4);
    words[6] = 0;
  }
  flags |= loose != 0 ? 0x200 : 0x800;
  if (callback != 0) {
    flags |= 0x2000;
  }
  if (r7 != 0) {
    flags |= 0x80;
  }
  words[0] = flags;
  LoadRequest request;
  for (int i = 0; i < 10; i++) {
    for (int b = 0; b < 4; b++) {
      request[4 * i + b] = uint8_t(words[i] >> (24 - 8 * b));
    }
  }
  return request;
}

int64_t Queue(uint8_t* base, const LoadRequest& request) {
  ITC_NATIVE_RUNNING("sub_8209DC18 Archive::Queue");
  Semaphores& semaphores = CurrentSemaphores();
  semaphores.queue_lock.Wait(base);
  int64_t serial = -1;
  if (const uint32_t slot = FindSlot(base, 0xFFFFFFFFu)) {
    if (LoadU32(base, kQueue + 12) == 0 && semaphores.work.Count() == 0) {
      semaphores.work.Signal(base);
    }
    const uint32_t next = LoadU32(base, kSerial);
    serial = next;
    StoreU32(base, slot + 8, next);
    for (uint32_t i = 0; i < 40; i++) {
      guest::StoreU8(base, slot + 12 + i, request[i]);
    }
    for (uint32_t at = 52; at <= 88; at += 4) {
      StoreU32(base, slot + at, 0);
    }
    const uint32_t after = LoadU32(base, kSerial) + 1;
    StoreU32(base, kSerial, int32_t(after) < 0x7FFFFFFF ? after : 0);
    ListAppend(base, kQueue, slot);
  }
  semaphores.queue_lock.Signal(base);
  return serial;
}

void WaitIdle(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8209D730 Archive::WaitIdle");
  Semaphores& semaphores = CurrentSemaphores();
  semaphores.queue_lock.Wait(base);
  if (LoadU32(base, kQueue + 8) != 0) {
    StoreU32(base, kIdleWanted, 1);
  }
  semaphores.queue_lock.Signal(base);
  if (LoadU32(base, kIdleWanted) != 0) {
    semaphores.idle.Wait(base);
  }
}

uint32_t QueueLength(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8209D7B0 Archive::QueueLength");
  Lock().Wait(base);
  const uint32_t count = LoadU32(base, kQueue + 8);
  Lock().Signal(base);
  return count;
}

void CancelQueued(uint8_t* base, bool wait) {
  ITC_NATIVE_RUNNING("sub_8209DB90 Archive::CancelQueued");
  Lock().Wait(base);
  for (uint32_t at = LoadU32(base, kQueue + 12); at != 0; at = LoadU32(base, at + 4)) {
    if (LoadU32(base, at + 52) == 0 && (LoadU32(base, at + 12) & 0x4000)) {
      Unqueue(base, at);
    }
  }
  Lock().Signal(base);
  if (wait) {
    WaitIdle(base);
  }
}

uint32_t Cancel(uint8_t* base, uint32_t serial) {
  ITC_NATIVE_RUNNING("sub_8209DB20 Archive::Cancel");
  Lock().Wait(base);
  uint32_t r3 = 1;
  if (const uint32_t slot = FindSlot(base, serial)) {
    if (LoadU32(base, slot + 52) == 0) {
      Unqueue(base, slot);
    } else {
      r3 = 0;
    }
  }
  Lock().Signal(base);
  return r3;
}

uint32_t PickNext(uint8_t* base, uint32_t current, uint32_t finished_chained,
                  uint32_t finished_current) {
  ITC_NATIVE_RUNNING("sub_8209D950 Archive::PickNext");
  Semaphores& semaphores = CurrentSemaphores();
  semaphores.queue_lock.Wait(base);
  auto compressed = [&](uint32_t flags) { return (flags & 0x100) && !(flags & 0x8); };
  uint32_t chained = 0;
  if (current != 0 && compressed(LoadU32(base, current + 12))) {
    chained = current;
  }
  for (uint32_t finished : {finished_chained, finished_current}) {
    if (finished != 0) {
      Unqueue(base, finished);
      StoreU32(base, finished + 52, 0);
      StoreU32(base, finished + 12, 0);
    }
  }
  const uint32_t head = LoadU32(base, kQueue + 12);
  uint32_t next = current != 0 ? LoadU32(base, current + 4) : head;
  if (next != 0) {
    if (LoadU32(base, next + 8) == 0xFFFFFFFFu) {
      next = head;
    }
    if (next != 0 && chained != 0 && (LoadU32(base, next + 12) & 1) &&
        (LoadU32(base, chained + 12) & 1)) {
      next = 0;
    }
    if (next != 0 && chained != 0) {
      const uint32_t flags = LoadU32(base, next + 12);
      if (!compressed(flags) && (flags & 0x4) && (LoadU32(base, chained + 12) & 0x4) &&
          LoadU32(base, next + 40) == LoadU32(base, chained + 88)) {
        next = 0;
      }
    }
    if (next != 0) {
      for (uint32_t at : {56u, 60u, 64u, 72u, 76u}) {
        StoreU32(base, next + at, 0);
      }
      StoreU32(base, next + 52, 1);
      for (uint32_t at : {80u, 84u, 88u}) {
        StoreU32(base, next + at, 0);
      }
    }
  }
  if (chained != 0) {
    if (next == 0) {
      next = chained;
      StoreU32(base, chained + 84, chained);
    } else {
      StoreU32(base, next + 84, chained);
    }
    StoreU32(base, chained + 52, 2);
  }
  if (next != 0 && semaphores.work.Count() != 0) {
    semaphores.work.Wait(base);
  }
  semaphores.queue_lock.Signal(base);
  return next;
}

void SetUp(uint8_t* base, uint32_t request, const SetUpCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209D490 Archive::SetUp");
  if (request == 0 || LoadU32(base, request + 52) != 1) {
    return;
  }
  Lock().Wait(base);
  uint32_t flags = LoadU32(base, request + 12);
  int32_t state = 0;
  const bool f10 = flags & 0x10, f20 = flags & 0x20;
  if (flags & 0x8) {
    state = 3;
  } else if (f10 && (flags & 0x800)) {
    state = 4;
  } else if (f10 && (flags & 0x200)) {
    state = 3;
  } else if (f20 && (flags & 0x200)) {
    state = 7;
  } else if (f20 && (flags & 0x400)) {
    state = 7;
  } else if (f20 && (flags & 0x800)) {
    state = 5;
  } else if (f20 && (flags & 0x1000)) {
    state = 6;
  } else if ((flags & 0x40) && (flags & 0x200)) {
    state = 8;
  }
  if (state != 0) {
    StoreU32(base, request + 52, uint32_t(state));
  }
  Lock().Signal(base);
  flags = LoadU32(base, request + 12);
  if (flags & 0x800) {
    StoreU32(base, request + 56, LoadU32(base, request + 24));
  }
  if (flags & 0x80) {
    StoreU32(base, request + 64, LoadU32(base, request + 28));
  }
  uint32_t stored;
  if (flags & 0x100) {
    StoreU32(base, request + 72, LoadU32(base, request + 32));
    stored = LoadU32(base, request + 36);
  } else {
    stored = LoadU32(base, request + 32);
    StoreU32(base, request + 72, stored);
  }
  StoreU32(base, request + 68, stored);
  const uint32_t sectors = (stored + 2047) >> 11;
  StoreU32(base, request + 60, sectors);
  if ((flags & 0x20) && (flags & 0x1)) {
    uint32_t data = 0;
    if (!(flags & 0x1000)) {
      const uint32_t bytes = sectors << 11;
      data = hooks::NativeHeapActive() ? memory::AllocPages(bytes, 4096)
                                       : calls.guest_alloc(calls.user, bytes);
    }
    if (const uint32_t word = LoadU32(base, request + 40)) {
      StoreU32(base, word, data);
    }
    StoreU32(base, request + 76, data);
  }
  flags = LoadU32(base, request + 12);
  if (flags & 0x4) {
    StoreU32(base, request + 76, LoadU32(base, request + 40));
  }
  if ((flags & 0x100) && !(flags & 0x8)) {
    StoreU32(base, request + 88, LoadU32(base, request + 76));
    const uint32_t count = LoadU32(base, kStagingCount);
    StoreU32(base, request + 76, LoadU32(base, kStagingHalves + ((count << 2) & 4)));
    StoreU32(base, kStagingCount, LoadU32(base, kStagingCount) + 1);
  }
}

uint32_t NextRequest(uint8_t* base, uint32_t current, const LoaderCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209DD98 Archive::NextRequest");
  uint32_t finished_chained = 0, finished_current = 0;
  bool keep = false;
  if (current != 0) {
    if (const uint32_t chained = LoadU32(base, current + 84)) {
      finished_chained = chained;
      FinishRequest(base, chained, calls.finish);
      if (current == chained) {
        current = 0;
      } else {
        StoreU32(base, current + 84, 0);
      }
    }
    if (current != 0) {
      const uint32_t flags = LoadU32(base, current + 12);
      keep = (flags & 0x100) && !(flags & 0x8);
    }
  }
  if (current != 0 && !keep) {
    finished_current = current;
    FinishRequest(base, current, calls.finish);
  }
  const uint32_t next = PickNext(base, current, finished_chained, finished_current);
  if (next != 0) {
    SetUp(base, next, calls.set_up);
  }
  return next;
}

// --- Ucl::Unpack ---

UclBlockResult UclBlock(uint8_t* base, uint32_t src, uint32_t src_len, uint32_t dst) {
  ITC_NATIVE_RUNNING("sub_822947F8 Ucl::UnpackBlock");
  uint32_t ilen = 0, olen = 0, last_m_off = 1;
  uint32_t bits = 0, count = 0;
  auto bit = [&]() -> uint32_t {
    if (count != 0) {
      count--;
      return (bits >> count) & 1;
    }
    const uint32_t at = src + ilen;
    bits = uint32_t(guest::LoadU8(base, at)) | uint32_t(guest::LoadU8(base, at + 1)) << 8 |
           uint32_t(guest::LoadU8(base, at + 2)) << 16 |
           uint32_t(guest::LoadU8(base, at + 3)) << 24;
    ilen += 4;
    count = 31;
    return bits >> 31;
  };
  auto byte = [&]() -> uint32_t { return guest::LoadU8(base, src + ilen++); };
  for (;;) {
    while (bit()) {
      guest::StoreU8(base, dst + olen++, uint8_t(byte()));
    }
    uint32_t m_off = 1;
    do {
      m_off = m_off * 2 + bit();
    } while (!bit());
    if (m_off != 2) {
      m_off = ((m_off - 3) << 8) + byte();
      if (m_off == 0xFFFFFFFFu) {
        break;
      }
      last_m_off = m_off + 1;
    }
    uint32_t m_len = bit();
    m_len = m_len * 2 + bit();
    if (m_len == 0) {
      m_len = 1;
      do {
        m_len = m_len * 2 + bit();
      } while (!bit());
      m_len += 2;
    }
    m_len += last_m_off > 0xD00 ? 1 : 0;
    uint32_t from = dst + olen - last_m_off;
    guest::StoreU8(base, dst + olen++, guest::LoadU8(base, from++));
    do {
      guest::StoreU8(base, dst + olen++, guest::LoadU8(base, from++));
    } while (--m_len != 0);
  }
  const int32_t r3 = ilen == src_len ? 0 : (ilen < src_len ? -205 : -201);
  return {r3, olen, ilen};
}

uint32_t UclUnpack(uint8_t* base, uint32_t dst, uint32_t stream, uint32_t expected,
                   const UclCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82294610 Ucl::Unpack");
  uint32_t written = 0;
  uint32_t at = 22;  // the first block's stored size
  uint32_t size = LoadU32(base, stream + 18);
  while (size != 0) {
    const uint32_t stored = LoadU32(base, stream + at);
    const uint32_t data = at + 4;
    if (stored < size) {
      UclBlock(base, stream + data, stored, dst + written);
      written += size;
      if (expected < written) {
        return 0;
      }
    } else {
      if (expected < stored + written) {
        return 0;
      }
      if (!native::Buffers_Memcpy(base, dst + written, stream + data, stored, 0x822946E4)) {
        calls.guest_memcpy(calls.user, dst + written, stream + data, stored);
      }
      written += stored;
    }
    at = data + stored;
    size = LoadU32(base, stream + at);
    at += 4;
  }
  return written == expected ? 1 : 0;
}

}  // namespace archive
