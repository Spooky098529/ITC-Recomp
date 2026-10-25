// The texture provider and its vector's helpers (see texture_provider.h),
// store for store as the recompiled functions.

#include "models/texture_provider.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <cstdlib>

namespace models {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

// The elements from `from` to `to`, as the originals count them (srawi).
uint32_t Count(uint32_t from, uint32_t to) { return uint32_t(int32_t(to - from) >> 2); }

// Whether operator delete frees `block` natively: 0, or a block the layer
// gave (else free's error path, the original's).
bool Deletable(uint32_t block) { return block == 0 || memory::IsHeapBlock(block); }

// A callee whose checks the caller made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("TextureProvider: {} refused after its checks", what);
    std::abort();
  }
}

void Delete(uint32_t block) { Require(memory::crt::OperatorDelete(block), "operator delete"); }

// How an insert of `count` before `where` goes, decided from the vector's
// words before anything changes.
struct InsertPlan {
  bool grow;          // new storage (else room in the old)
  uint32_t capacity;  // the new storage's, when growing
};

std::optional<InsertPlan> PlanInsert(uint8_t* base, uint32_t first, uint32_t last,
                                     uint32_t end, uint32_t where, uint32_t count) {
  const uint32_t capacity = first == 0 ? 0 : Count(first, end);
  const uint32_t size = first == 0 ? 0 : Count(first, last);
  if (kVectorMaxSize - size < count) {
    return std::nullopt;  // _Xlen
  }
  if (capacity < size + count) {
    const uint32_t half = capacity >> 1;
    uint32_t grown = 0;
    if (!(kVectorMaxSize - half < capacity)) {
      grown = half + capacity;
    }
    if (grown < size + count) {
      grown = size + count;
    }
    if (!CanAllocatePointers(base, grown) || !Deletable(first)) {
      return std::nullopt;
    }
    return InsertPlan{true, grown};
  }
  if (!(Count(where, last) < count)) {
    return std::nullopt;  // copy_backward's path (sub_82590D18), never seen in play
  }
  return InsertPlan{false, 0};
}

// The insert as planned (count not 0). nullopt only when room in the old
// storage and the tail's memmove_s is left to the original (nothing changed).
std::optional<R3> RunInsert(uint8_t* base, uint32_t vector, uint32_t where, uint32_t count,
                            uint32_t value, const InsertPlan& plan) {
  const uint32_t bytes = count << 2;
  if (plan.grow) {
    const auto made = PointersAllocate(base, plan.capacity);
    Require(made.has_value(), "TextureProvider::Allocate");
    const uint32_t storage = *made;
    const uint32_t first = Load(base, vector + kVectorFirst);
    const uint32_t front = Count(first, where);
    const uint32_t front_bytes = front << 2;
    const uint32_t at = storage + front_bytes;
    if (front != 0) {
      Require(c_runtime::MemmoveS(base, storage, front_bytes, first, front_bytes).has_value(),
              "memmove_s");
    }
    for (uint32_t i = 0; i < count; i++) {
      Store(base, at + 4 * i, value);
    }
    const uint32_t tail = Count(where, Load(base, vector + kVectorLast));
    if (tail != 0) {
      const uint32_t tail_bytes = tail << 2;
      Require(c_runtime::MemmoveS(base, at + bytes, tail_bytes, where, tail_bytes).has_value(),
              "memmove_s");
    }
    const uint32_t old = Load(base, vector + kVectorFirst);
    const uint32_t size = (old == 0 ? 0 : Count(old, Load(base, vector + kVectorLast))) + count;
    if (old != 0) {
      Delete(old);
    }
    Store(base, vector + kVectorFirst, storage);
    Store(base, vector + kVectorEnd, storage + (plan.capacity << 2));
    Store(base, vector + kVectorLast, storage + (size << 2));
    return R3{false, old};
  }
  const uint32_t last = Load(base, vector + kVectorLast);
  const uint32_t tail = Count(where, last);
  if (tail != 0) {
    const uint32_t tail_bytes = tail << 2;
    if (!c_runtime::MemmoveS(base, where + bytes, tail_bytes, where, tail_bytes)) {
      return std::nullopt;
    }
  }
  const uint32_t moved = Load(base, vector + kVectorLast);
  const uint32_t more = count - Count(where, moved);
  for (uint32_t i = 0; i < more; i++) {
    Store(base, moved + 4 * i, value);
  }
  const uint32_t new_last = Load(base, vector + kVectorLast) + bytes;
  const uint32_t old_last = new_last - bytes;
  Store(base, vector + kVectorLast, new_last);
  for (uint32_t p = where; p != old_last; p += 4) {
    Store(base, p, value);
  }
  return tail != 0 ? R3{false, 0} : R3{true, 0};
}

}  // namespace

bool CanAllocatePointers(uint8_t* base, uint32_t count) {
  if (count != 0 && 0xFFFFFFFFu / count < 4) {
    return false;  // the bad_alloc path
  }
  return (count << 2) <= 0xFFFFF000u && memory::crt::GetProcessHeap(base) != 0;
}

std::optional<uint32_t> PointersAllocate(uint8_t* base, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_825942B8 TextureProvider::Allocate");
  if (!CanAllocatePointers(base, count)) {
    return std::nullopt;
  }
  return memory::crt::OperatorNew(base, count << 2);
}

uint32_t PointersUninitializedCopy(uint8_t* base, uint32_t first, uint32_t last,
                                   uint32_t destination) {
  ITC_NATIVE_RUNNING("sub_82594380 TextureProvider::UninitializedCopy");
  while (first != last) {
    if (destination != 0) {
      Store(base, destination, Load(base, first));
    }
    first += 4;
    destination += 4;
  }
  return destination;
}

std::optional<R3> PointersReserve(uint8_t* base, uint32_t vector, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82594570 TextureProvider::Reserve");
  if (count > kVectorMaxSize) {
    return std::nullopt;  // _Xlen
  }
  const uint32_t first = Load(base, vector + kVectorFirst);
  const uint32_t capacity = first == 0 ? 0 : Count(first, Load(base, vector + kVectorEnd));
  if (capacity >= count) {
    return R3{true, 0};
  }
  if (!CanAllocatePointers(base, count) || !Deletable(first)) {
    return std::nullopt;
  }
  const auto made = PointersAllocate(base, count);
  Require(made.has_value(), "TextureProvider::Allocate");
  const uint32_t storage = *made;
  PointersUninitializedCopy(base, Load(base, vector + kVectorFirst),
                            Load(base, vector + kVectorLast), storage);
  const uint32_t old = Load(base, vector + kVectorFirst);
  uint32_t size = 0;
  if (old != 0) {
    size = Count(old, Load(base, vector + kVectorLast));
    Delete(old);
  }
  Store(base, vector + kVectorFirst, storage);
  Store(base, vector + kVectorEnd, storage + (count << 2));
  Store(base, vector + kVectorLast, storage + (size << 2));
  return R3{false, old};
}

std::optional<R3> PointersInsertN(uint8_t* base, uint32_t vector, uint32_t where,
                                  uint32_t count, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82590EF0 TextureProvider::InsertN");
  if (count == 0) {
    return R3{true, 0};
  }
  const auto plan = PlanInsert(base, Load(base, vector + kVectorFirst),
                               Load(base, vector + kVectorLast),
                               Load(base, vector + kVectorEnd), where, count);
  if (!plan) {
    return std::nullopt;
  }
  return RunInsert(base, vector, where, count, value, *plan);
}

std::optional<R3> PointersAssign(uint8_t* base, uint32_t vector, uint32_t count,
                                 uint32_t value) {
  ITC_NATIVE_RUNNING("sub_825947D0 TextureProvider::Assign");
  const uint32_t first = Load(base, vector + kVectorFirst);
  // After the clear, the insert is before the first element of none.
  if (count != 0 && !PlanInsert(base, first, first, Load(base, vector + kVectorEnd), first,
                                count)) {
    return std::nullopt;
  }
  if (first != Load(base, vector + kVectorLast)) {
    // erase(begin, end): no elements after the end to move (memmove_s with
    // 0 bytes, not called).
    Store(base, vector + kVectorLast, first);
  }
  const auto r3 = PointersInsertN(base, vector, Load(base, vector + kVectorFirst), count, value);
  Require(r3.has_value(), "TextureProvider::InsertN");
  return r3;
}

uint32_t PointersClear(uint8_t* base, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_82590D68 TextureProvider::Clear");
  const uint32_t first = Load(base, vector + kVectorFirst);
  if (first != Load(base, vector + kVectorLast)) {
    Store(base, vector + kVectorLast, first);
  }
  return first;
}

std::optional<uint32_t> TextureProviderConstruct(uint8_t* base, uint32_t provider,
                                                 int32_t count) {
  ITC_NATIVE_RUNNING("sub_82594850 TextureProvider::TextureProvider");
  const uint32_t slots = count == -1 ? kTextureProviderSlots : uint32_t(count);
  if (slots > kVectorMaxSize || (slots != 0 && !CanAllocatePointers(base, slots))) {
    return std::nullopt;
  }
  const uint32_t vector = provider + kTextureProviderVector;
  Store(base, provider, kTextureProviderVtable);
  Store(base, vector + kVectorFirst, 0);
  Store(base, vector + kVectorLast, 0);
  Store(base, vector + kVectorEnd, 0);
  Require(PointersReserve(base, vector, slots).has_value(), "TextureProvider::Reserve");
  Require(PointersAssign(base, vector, slots, 0).has_value(), "TextureProvider::Assign");
  return provider;
}

std::optional<uint32_t> TextureProviderDestruct(uint8_t* base, uint32_t provider) {
  ITC_NATIVE_RUNNING("sub_825943E0 TextureProvider::~TextureProvider");
  const uint32_t vector = provider + kTextureProviderVector;
  if (!Deletable(Load(base, vector + kVectorFirst))) {
    return std::nullopt;
  }
  Store(base, provider, kTextureProviderVtable);
  PointersClear(base, vector);
  const uint32_t storage = Load(base, vector + kVectorFirst);
  if (storage != 0) {
    Delete(storage);
  }
  Store(base, vector + kVectorFirst, 0);
  Store(base, vector + kVectorLast, 0);
  Store(base, vector + kVectorEnd, 0);
  return storage;
}

}  // namespace models
