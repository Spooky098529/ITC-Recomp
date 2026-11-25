// The archive streaming's functions as plain C++: see archive.h.

#include "archive/archive.h"

#include "archive/semaphores.h"

#include "base/log.h"
#include "guest.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "native/buffers.h"
#include "schema/archive.h"
#include "schema/c_runtime.h"

namespace archive {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::ArchiveGlobals;
using Slot = schema::ArchiveRequestSlot;
using List = schema::ArchiveList;

namespace {

List QueueList() {
  return List(G::queue_address());
}

}  // namespace

uint32_t Toc(uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8209D868 Archive::Toc");
  return (id < kFirstCrsId ? G::build_toc() : G::crs_toc()).address;
}

uint32_t AssetSize(uint32_t id) {
  ITC_NATIVE_RUNNING("sub_820A0678 Archive::AssetSize");
  const uint32_t index = id < kFirstCrsId ? id : id - kFirstCrsId;
  const schema::ArchiveTocEntry entry(Toc(id) + ((index + 1) << 4));
  const uint32_t unpacked = entry.unpacked_size();
  return unpacked != 0 ? unpacked : entry.stored_size();
}

// --- The request queue's list and slots ---

void ListConstruct(uint32_t list, uint32_t deletes) {
  ITC_NATIVE_RUNNING("sub_82294B80 List::List");
  const List l(list);
  l.set_deletes_nodes(deletes);
  l.set_vtable(List::kVtable);
  l.set_count(0);
  l.set_head(0u);
  l.set_tail(0u);
}

uint32_t ListAppend(uint32_t list, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_82294BA8 List::Append");
  if (node == 0) {
    return 0;
  }
  const List l(list);
  if (const Slot tail = l.tail()) {
    tail.set_link(node);
  } else {
    l.set_head(node);
  }
  Slot(node).set_link(0u);
  l.set_tail(node);
  l.set_count(l.count() + 1);
  return 1;
}

uint32_t ListRemove(uint32_t list, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_82294C00 List::Remove");
  if (node == 0) {
    return 0;
  }
  const List l(list);
  uint32_t previous = 0;
  uint32_t at = l.head().address;
  while (at != 0 && at != node) {
    previous = at;
    at = Slot(at).link().address;
  }
  if (at == 0) {
    return 0;
  }
  if (previous != 0) {
    Slot(previous).set_link(Slot(at).link().address);
  }
  if (l.head().address == at) {
    l.set_head(Slot(at).link().address);
  }
  if (l.tail().address == at) {
    l.set_tail(previous);
  }
  l.set_count(l.count() - 1);
  return 1;
}

void NodeConstruct(uint32_t node, uint32_t link) {
  ITC_NATIVE_RUNNING("sub_82294B68 ListNode::ListNode");
  const schema::ArchiveListNode n(node);
  n.set_link(link);
  n.set_vtable(schema::ArchiveListNode::kVtable);
}

void NodeDestruct(uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8224FB10 ListNode::~ListNode");
  schema::ArchiveListNode(node).set_vtable(schema::ArchiveListNode::kVtable);
}

uint32_t FindSlot(uint32_t serial) {
  ITC_NATIVE_RUNNING("sub_8209D818 Archive::FindSlot");
  for (uint32_t i = 0; i < kSlotCount; i++) {
    const Slot slot(G::slots_address() + i * Slot::kSize);
    if (slot.serial() == serial) {
      return slot.address;
    }
  }
  return 0;
}

int32_t QueueStaticInit(const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825D9160 RequestQueue::StaticInit");
  ListConstruct(G::queue_address(), 0);
  QueueList().set_vtable(kQueueVtable);
  return c_runtime::AtexitFrom(kQueueCleanUp, call);
}

int32_t SlotsStaticInit(const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825D91B0 RequestSlots::StaticInit");
  for (uint32_t i = 0; i < kSlotCount; i++) {
    const Slot slot(G::slots_address() + i * Slot::kSize);
    NodeConstruct(slot.address, 0);
    slot.set_vtable(Slot::kVtable);
  }
  return c_runtime::AtexitFrom(kSlotsCleanUp, call);
}

// --- The loose files ---

uint32_t FindLooseFile(uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8209D8D8 Archive::FindLooseFile");
  for (uint32_t step = 0;; step++) {
    const schema::ArchiveLooseFile entry((step < kFirstCrsId ? G::loose_files_address() : G::crs_loose_files_address()) +
                  schema::ArchiveLooseFile::kSize * step);
    const uint32_t path = entry.path().address;
    if (path == 0) {
      return 0;
    }
    if (entry.id() == id) {
      return path;
    }
  }
}

// --- The loader thread's pieces ---

std::optional<uint32_t> FinishRequest(uint32_t request,
                                      const FinishCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209D688 Archive::FinishRequest");
  const Slot r(request);
  uint32_t flags = r.flags();
  if ((flags & 0x100) && !(flags & 0x8)) {
    G::set_staging_in_use(r.data());
    r.set_data(r.unpack_destination());
  }
  if (r.flags() & 0x2000) {
    const uint32_t serial = r.serial();
    G::set_callback_serial(serial);
    calls.callback(calls.user, r.callback(), serial, r.data(), r.size(), r.context());
  }
  flags = r.flags();
  if ((flags & 0x2) && !(flags & 0x1000)) {
    constexpr uint32_t kMemDecommit = 0x4000;
    const uint32_t data = r.data();
    // VirtualFree's answer called directly (E5 step 6, in place of
    // hooks::VirtualFree): a decommit of our allocation layer's pages leaves
    // them as they are and answers 1; else the game's VirtualFree.
    if (hooks::NativeHeapActive() && memory::InRange(data)) {
      return 1u;
    }
    calls.guest_free(calls.user, data, 0, kMemDecommit);
    return std::nullopt;
  }
  return std::nullopt;
}

uint32_t LoaderFunctorTarget(uint32_t functor) {
  ITC_NATIVE_RUNNING("sub_8209DD80 LoaderFunctor::Invoke");
  return schema::LoaderFunctor(functor).target();
}

// --- The queue and the loader under the four semaphores ---

namespace {

HostSemaphore& Lock() {
  return CurrentSemaphores().queue_lock;
}

// A request taken off the queue and its slot freed (the picker's and the
// cancels' steps).
void Unqueue(uint32_t request) {
  ListRemove(G::queue_address(), request);
  Slot(request).set_serial(0xFFFFFFFFu);
}

}  // namespace

LoadRequest BuildLoad(uint32_t id, uint32_t destination, uint32_t callback,
                      uint32_t context, uint32_t r7, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_8209DE48 Archive::Load");
  const uint32_t index = id < kFirstCrsId ? id : id - kFirstCrsId;
  const schema::ArchiveTocEntry entry((id < kFirstCrsId ? G::build_toc() : G::crs_toc()).address +
                ((index + 1) << 4));
  const uint32_t unpacked = entry.unpacked_size();
  uint32_t flags = unpacked != 0 ? 0x4124 : 0x4024;
  const uint32_t loose = FindLooseFile(id);
  uint32_t words[10];
  words[1] = id;
  words[2] = loose;
  words[3] = entry.start_sector() << 11;
  words[4] = r7;
  words[7] = destination;
  words[8] = context;
  words[9] = callback;
  if (flags & 0x100) {
    words[5] = unpacked;
    words[6] = entry.stored_size();
  } else {
    words[5] = size != 0 ? size : entry.stored_size();
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

int64_t Queue(const LoadRequest& request) {
  ITC_NATIVE_RUNNING("sub_8209DC18 Archive::Queue");
  Semaphores& semaphores = CurrentSemaphores();
  semaphores.queue_lock.Wait();
  int64_t serial = -1;
  if (const uint32_t found = FindSlot(0xFFFFFFFFu)) {
    const Slot slot(found);
    if (QueueList().head().address == 0 && semaphores.work.Count() == 0) {
      semaphores.work.Signal();
    }
    const uint32_t next = G::serial();
    serial = next;
    slot.set_serial(next);
    // The request, copied to its fields (+12 to +48) as the original's bytes.
    for (uint32_t i = 0; i < 40; i++) {
      guest::StoreU8(slot.flags_address() + i, request[i]);
    }
    for (uint32_t at = slot.state_address(); at <= slot.unpack_destination_address(); at += 4) {
      StoreU32(at, 0);
    }
    const uint32_t after = G::serial() + 1;
    G::set_serial(int32_t(after) < 0x7FFFFFFF ? after : 0);
    ListAppend(G::queue_address(), slot.address);
  }
  semaphores.queue_lock.Signal();
  return serial;
}

void WaitIdle() {
  ITC_NATIVE_RUNNING("sub_8209D730 Archive::WaitIdle");
  Semaphores& semaphores = CurrentSemaphores();
  semaphores.queue_lock.Wait();
  if (QueueList().count() != 0) {
    G::set_idle_wanted(1);
  }
  semaphores.queue_lock.Signal();
  if (G::idle_wanted() != 0) {
    semaphores.idle.Wait();
  }
}

uint32_t QueueLength() {
  ITC_NATIVE_RUNNING("sub_8209D7B0 Archive::QueueLength");
  Lock().Wait();
  const uint32_t count = QueueList().count();
  Lock().Signal();
  return count;
}

void CancelQueued(bool wait) {
  ITC_NATIVE_RUNNING("sub_8209DB90 Archive::CancelQueued");
  Lock().Wait();
  for (Slot at = QueueList().head(); at; at = at.link()) {
    if (at.state() == 0 && (at.flags() & 0x4000)) {
      Unqueue(at.address);
    }
  }
  Lock().Signal();
  if (wait) {
    WaitIdle();
  }
}

uint32_t Cancel(uint32_t serial) {
  ITC_NATIVE_RUNNING("sub_8209DB20 Archive::Cancel");
  Lock().Wait();
  uint32_t r3 = 1;
  if (const uint32_t slot = FindSlot(serial)) {
    if (Slot(slot).state() == 0) {
      Unqueue(slot);
    } else {
      r3 = 0;
    }
  }
  Lock().Signal();
  return r3;
}

uint32_t PickNext(uint32_t current, uint32_t finished_chained,
                  uint32_t finished_current) {
  ITC_NATIVE_RUNNING("sub_8209D950 Archive::PickNext");
  Semaphores& semaphores = CurrentSemaphores();
  semaphores.queue_lock.Wait();
  auto compressed = [&](uint32_t flags) { return (flags & 0x100) && !(flags & 0x8); };
  auto slot = [](uint32_t at) { return Slot(at); };
  uint32_t chained = 0;
  if (current != 0 && compressed(slot(current).flags())) {
    chained = current;
  }
  for (uint32_t finished : {finished_chained, finished_current}) {
    if (finished != 0) {
      Unqueue(finished);
      slot(finished).set_state(0);
      slot(finished).set_flags(0);
    }
  }
  const uint32_t head = QueueList().head().address;
  uint32_t next = current != 0 ? slot(current).link().address : head;
  if (next != 0) {
    if (slot(next).serial() == 0xFFFFFFFFu) {
      next = head;
    }
    if (next != 0 && chained != 0 && (slot(next).flags() & 1) && (slot(chained).flags() & 1)) {
      next = 0;
    }
    if (next != 0 && chained != 0) {
      const uint32_t flags = slot(next).flags();
      if (!compressed(flags) && (flags & 0x4) && (slot(chained).flags() & 0x4) &&
          slot(next).destination() == slot(chained).unpack_destination()) {
        next = 0;
      }
    }
    if (next != 0) {
      const Slot n = slot(next);
      n.set_read_offset(0);
      n.set_sectors(0);
      n.set_read_r7(0);
      n.set_read_unpacked_size(0);
      n.set_data(0);
      n.set_state(1);
      n.set_unknown_80(0);
      n.set_chained(0u);
      n.set_unpack_destination(0);
    }
  }
  if (chained != 0) {
    if (next == 0) {
      next = chained;
      slot(chained).set_chained(chained);
    } else {
      slot(next).set_chained(chained);
    }
    slot(chained).set_state(2);
  }
  if (next != 0 && semaphores.work.Count() != 0) {
    semaphores.work.Wait();
  }
  semaphores.queue_lock.Signal();
  return next;
}

void SetUp(uint32_t request, const SetUpCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209D490 Archive::SetUp");
  const Slot r(request);
  if (request == 0 || r.state() != 1) {
    return;
  }
  Lock().Wait();
  uint32_t flags = r.flags();
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
    r.set_state(uint32_t(state));
  }
  Lock().Signal();
  flags = r.flags();
  if (flags & 0x800) {
    r.set_read_offset(r.byte_offset());
  }
  if (flags & 0x80) {
    r.set_read_r7(r.load_r7());
  }
  uint32_t stored;
  if (flags & 0x100) {
    r.set_read_unpacked_size(r.size());
    stored = r.stored_size();
  } else {
    stored = r.size();
    r.set_read_unpacked_size(stored);
  }
  r.set_read_stored_size(stored);
  const uint32_t sectors = (stored + 2047) >> 11;
  r.set_sectors(sectors);
  if ((flags & 0x20) && (flags & 0x1)) {
    uint32_t data = 0;
    if (!(flags & 0x1000)) {
      const uint32_t bytes = sectors << 11;
      data = hooks::NativeHeapActive() ? memory::AllocPages(bytes, 4096)
                                       : calls.guest_alloc(calls.user, bytes);
    }
    // The caller's word the allocation is written to.
    if (const uint32_t word = r.destination()) {
      StoreU32(word, data);
    }
    r.set_data(data);
  }
  flags = r.flags();
  if (flags & 0x4) {
    r.set_data(r.destination());
  }
  if ((flags & 0x100) && !(flags & 0x8)) {
    r.set_unpack_destination(r.data());
    const uint32_t count = G::staging_count();
    r.set_data(LoadU32(G::staging_halves_address() + ((count << 2) & 4)));
    G::set_staging_count(G::staging_count() + 1);
  }
}

uint32_t NextRequest(uint32_t current, const LoaderCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209DD98 Archive::NextRequest");
  uint32_t finished_chained = 0, finished_current = 0;
  bool keep = false;
  if (current != 0) {
    if (const uint32_t chained = Slot(current).chained().address) {
      finished_chained = chained;
      FinishRequest(chained, calls.finish);
      if (current == chained) {
        current = 0;
      } else {
        Slot(current).set_chained(0u);
      }
    }
    if (current != 0) {
      const uint32_t flags = Slot(current).flags();
      keep = (flags & 0x100) && !(flags & 0x8);
    }
  }
  if (current != 0 && !keep) {
    finished_current = current;
    FinishRequest(current, calls.finish);
  }
  const uint32_t next = PickNext(current, finished_chained, finished_current);
  if (next != 0) {
    SetUp(next, calls.set_up);
  }
  return next;
}

// --- Ucl::Unpack ---

UclBlockResult UclBlock(uint32_t src, uint32_t src_len, uint32_t dst) {
  ITC_NATIVE_RUNNING("sub_822947F8 Ucl::UnpackBlock");
  uint32_t ilen = 0, olen = 0, last_m_off = 1;
  uint32_t bits = 0, count = 0;
  auto bit = [&]() -> uint32_t {
    if (count != 0) {
      count--;
      return (bits >> count) & 1;
    }
    const uint32_t at = src + ilen;
    bits = uint32_t(guest::LoadU8(at)) | uint32_t(guest::LoadU8(at + 1)) << 8 |
           uint32_t(guest::LoadU8(at + 2)) << 16 |
           uint32_t(guest::LoadU8(at + 3)) << 24;
    ilen += 4;
    count = 31;
    return bits >> 31;
  };
  auto byte = [&]() -> uint32_t { return guest::LoadU8(src + ilen++); };
  for (;;) {
    while (bit()) {
      guest::StoreU8(dst + olen++, uint8_t(byte()));
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
    guest::StoreU8(dst + olen++, guest::LoadU8(from++));
    do {
      guest::StoreU8(dst + olen++, guest::LoadU8(from++));
    } while (--m_len != 0);
  }
  const int32_t r3 = ilen == src_len ? 0 : (ilen < src_len ? -205 : -201);
  return {r3, olen, ilen};
}

uint32_t UclUnpack(uint32_t dst, uint32_t stream, uint32_t expected,
                   const UclCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82294610 Ucl::Unpack");
  uint32_t written = 0;
  uint32_t at = 22;  // the first block's stored size
  uint32_t size = LoadU32(stream + 18);
  while (size != 0) {
    const uint32_t stored = LoadU32(stream + at);
    const uint32_t data = at + 4;
    if (stored < size) {
      UclBlock(stream + data, stored, dst + written);
      written += size;
      if (expected < written) {
        return 0;
      }
    } else {
      if (expected < stored + written) {
        return 0;
      }
      if (!native::Buffers_Memcpy(dst + written, stream + data, stored, 0x822946E4)) {
        calls.guest_memcpy(calls.user, dst + written, stream + data, stored);
      }
      written += stored;
    }
    at = data + stored;
    size = LoadU32(stream + at);
    at += 4;
  }
  return written == expected ? 1 : 0;
}

}  // namespace archive
