// std::vector of 16-byte elements under TextureProjection::Fit
// (docs/functions/car-scene-draw.md): push_back, insert, _Insert_n and the
// allocator, store for store as the recompiled functions (see
// car_scene_draw.h).

#include "car_scene_draw/car_scene_draw.h"

#include "guest.h"

#include "base/log.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "schema/car_scene_draw.h"

#include <cstdlib>

namespace car_scene_draw {

namespace {

using Vector = schema::Vector16;

constexpr uint32_t kMaxSize = 0x0FFFFFFF;  // _Xlen past it
// malloc's largest size (memory/c_runtime_heap.cpp's kMaxSize).
constexpr uint32_t kMaxAllocation = 0xFFFFF000;

constexpr uint32_t kPushBackFrame = 96;
constexpr uint32_t kInsertFrame = 112;
constexpr uint32_t kInsertNFrame = 144;
constexpr uint32_t kAllocateFrame = 112;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// One element as ld, std, ld, std (the copy loops').
void CopyElement(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t half = 0; half < 16; half += 8) {
    const uint32_t hi = Load(base, from + half);
    const uint32_t lo = Load(base, from + half + 4);
    Store(base, to + half, hi);
    Store(base, to + half + 4, lo);
  }
}

// (last - first) / 16 as srawi, 0 with no storage: the size, and with
// `capacity` (end - first) the capacity.
uint32_t Count(uint8_t* base, uint32_t vector, bool capacity) {
  const Vector v(base, vector);
  const uint32_t first = v.first().address;
  if (first == 0) {
    return 0;
  }
  const uint32_t at = capacity ? v.end().address : v.last().address;
  return uint32_t(int32_t(at - first) >> 4);
}
constexpr bool kCount = false;
constexpr bool kCapacity = true;

// Whether Allocate(count) gives its block natively: not the bad_alloc path
// (0xFFFFFFFF / count below 16) and malloc's checks.
bool Allocatable(uint8_t* base, uint32_t count) {
  if (count != 0 && 0xFFFFFFFFu / count < 16) {
    return false;
  }
  return (count << 4) <= kMaxAllocation && memory::crt::GetProcessHeap(base) != 0;
}

}  // namespace

std::optional<uint32_t> Vector16Allocate(uint8_t* base, uint32_t count, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82598158 Vector16::Allocate");
  if (!hooks::NativeHeapActive() || !Allocatable(base, count)) {
    return std::nullopt;
  }
  Store(base, stack - kAllocateFrame, stack);  // the back chain, as stwu
  // operator new(16 x count) in host memory (E5 step 4): one array of
  // points behind one handle, freed by operator delete as before.
  return schema::Vector16Storage::NewArray(base, count).address;
}

std::optional<uint32_t> Vector16InsertN(uint8_t* base, uint32_t vector, uint32_t where,
                                        uint32_t n, uint32_t value, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82598288 Vector16::InsertN");
  const Vector v(base, vector);
  const uint32_t f = stack - kInsertNFrame;
  const uint32_t first = v.first().address;
  const uint32_t capacity = Count(base, vector, kCapacity);
  if (n != 0) {
    const uint32_t size = Count(base, vector, kCount);
    if (kMaxSize - size < n) {
      return std::nullopt;  // _Xlen (sub_825981C8)
    }
    if (capacity < size + n) {
      // The new capacity: half again, or the size needed when that's more.
      const uint32_t half = capacity >> 1;
      uint32_t grown = 0;
      if (kMaxSize - half >= capacity) {
        grown = half + capacity;
      }
      if (grown < size + n) {
        grown = size + n;
      }
      if (!hooks::NativeHeapActive() || !Allocatable(base, grown) ||
          (first != 0 && !memory::IsHeapBlock(first))) {
        return std::nullopt;
      }
    }
  }
  Store(base, f, stack);  // the back chain, as stwu
  // The value copied to the frame first (ld, ld, std, std).
  {
    uint32_t w[4];
    for (int i = 0; i < 4; i++) {
      w[i] = Load(base, value + 4 * i);
    }
    for (int i = 0; i < 4; i++) {
      Store(base, f + 80 + 4 * i, w[i]);
    }
  }
  const uint32_t copy = f + 80;
  if (n == 0) {
    return vector;
  }
  const uint32_t size = Count(base, vector, kCount);
  const uint32_t bytes = n << 4;
  if (capacity < size + n) {
    const uint32_t half = capacity >> 1;
    uint32_t grown = 0;
    if (kMaxSize - half >= capacity) {
      grown = half + capacity;
    }
    if (grown < size + n) {
      grown = size + n;
    }
    const auto made = Vector16Allocate(base, grown, f);
    if (!made) {
      ITC_LOG_ERROR("Vector16::InsertN: operator new refused {} elements after its checks",
                    grown);
      std::abort();
    }
    const uint32_t storage = *made;
    // [first, where), the n copies, then [where, last), each skipped at a
    // null address (placement new's check).
    uint32_t to = storage;
    for (uint32_t from = v.first().address; from != where; from += 16, to += 16) {
      if (to != 0) {
        CopyElement(base, to, from);
      }
    }
    uint32_t fill = to;
    for (uint32_t k = n; k != 0; k--, fill += 16) {
      if (fill != 0) {
        CopyElement(base, fill, copy);
      }
    }
    const uint32_t last = v.last().address;
    if (where != last) {
      uint32_t at = bytes + to;
      for (uint32_t from = where; from != last; from += 16, at += 16) {
        if (at != 0) {
          CopyElement(base, at, from);
        }
      }
    }
    const uint32_t old = v.first().address;
    const uint32_t count = Count(base, vector, kCount) + n;
    if (old != 0 && !memory::crt::OperatorDelete(old)) {
      ITC_LOG_ERROR("Vector16::InsertN: operator delete refused {:08X} after its checks", old);
      std::abort();
    }
    v.set_first(storage);
    v.set_end(storage + (grown << 4));
    v.set_last(storage + (count << 4));
    return old;  // operator delete leaves its block in r3
  }
  const uint32_t last = v.last().address;
  const uint32_t after = uint32_t(int32_t(last - where) >> 4);
  if (after < n) {
    // Fewer elements after where than n: those moved past the n copies, the
    // copies past last first, then over the moved ones.
    if (where != last) {
      uint32_t to = bytes + where;
      for (uint32_t from = where; from != last; from += 16, to += 16) {
        if (to != 0) {
          CopyElement(base, to, from);
        }
      }
    }
    uint32_t at = v.last().address;
    const uint32_t moved = uint32_t(int32_t(at - where) >> 4);
    for (uint32_t k = n - moved; k != 0; k--, at += 16) {
      if (at != 0) {
        CopyElement(base, at, copy);
      }
    }
    const uint32_t grown_last = v.last().address + bytes;
    const uint32_t old_last = grown_last - bytes;
    v.set_last(grown_last);
    for (uint32_t to = where; to != old_last; to += 16) {
      CopyElement(base, to, copy);
    }
    return vector;
  }
  // The last n moved past last, the rest moved back by n (copy_backward),
  // then the n copies at where.
  const uint32_t tail = last - bytes;
  uint32_t to = last;
  for (uint32_t from = tail; from != last; from += 16, to += 16) {
    if (to != 0) {
      CopyElement(base, to, from);
    }
  }
  v.set_last(to);
  if (where != tail) {
    uint32_t from = tail;
    uint32_t back = bytes + tail;
    do {
      from -= 16;
      back -= 16;
      CopyElement(base, back, from);
    } while (from != where);
  }
  for (uint32_t at = where; at != bytes + where; at += 16) {
    CopyElement(base, at, copy);
  }
  return vector;
}

std::optional<uint32_t> Vector16Insert(uint8_t* base, uint32_t out, uint32_t vector,
                                       uint32_t where, uint32_t value, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82598648 Vector16::Insert");
  const Vector v(base, vector);
  const uint32_t f = stack - kInsertFrame;
  const uint32_t first = v.first().address;
  uint32_t index = 0;
  if (first != 0 && Count(base, vector, kCount) != 0) {
    index = uint32_t(int32_t(where - first) >> 4);
  }
  if (!Vector16InsertN(base, vector, where, 1, value, f)) {
    return std::nullopt;
  }
  Store(base, f, stack);  // the back chain, as stwu (after: InsertN may decline)
  Store(base, out, v.first().address + (index << 4));
  return out;
}

std::optional<uint32_t> Vector16PushBack(uint8_t* base, uint32_t vector, uint32_t value,
                                         uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_825986B8 Vector16::PushBack");
  const Vector v(base, vector);
  const uint32_t f = stack - kPushBackFrame;
  const uint32_t first = v.first().address;
  const uint32_t size = Count(base, vector, kCount);
  if (first != 0 && size < Count(base, vector, kCapacity)) {
    Store(base, f, stack);  // the back chain, as stwu
    const uint32_t last = v.last().address;
    if (last != 0) {
      CopyElement(base, last, value);
    }
    v.set_last(last + 16);
    return vector;
  }
  const uint32_t where = v.last().address;
  Store(base, f + 80, where);
  if (!Vector16Insert(base, f + 80, vector, where, value, f)) {
    return std::nullopt;
  }
  Store(base, f, stack);
  return f + 80;
}

}  // namespace car_scene_draw
