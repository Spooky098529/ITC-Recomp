// The course scenery (course_scenery.h; docs/functions/course-scenery.md).

#include "course_scenery/course_scenery.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "hooks/heap.h"
#include "memory/heap.h"

#include <initializer_list>

namespace course_scenery {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Round64(uint32_t size) {
  return (size + 63) & 0xFFFFFFC0u;
}

// The base vtable (PoolObject::Key, the load sub_820FBB68, PoolObject::Reset).
constexpr uint32_t kPoolObjectVtable = 0x82006848;

// The vtables' targets that are native.
constexpr uint32_t kPoolObjectKey = 0x820FB9F0;
constexpr uint32_t kAreaLightsKey = 0x820F1C20;
constexpr uint32_t kAreaModelKey = 0x820F1CD8;
constexpr uint32_t kPoolObjectReset = 0x820FB9D8;

// memcpy (sub_825A7410), for the copies c_runtime::Memcpy leaves to it.
constexpr uint32_t kMemcpy = 0x825A7410;

// PoolObject::PoolObject's stores (ObjectPools::Init does the same inline).
void Construct(uint8_t* base, uint32_t object, uint32_t own_size, uint32_t data_size,
               uint32_t index) {
  const uint32_t data = object + own_size;
  Store(base, object + 0, kPoolObjectVtable);
  Store(base, object + 8, data_size);
  Store(base, object + 12, 0);
  Store(base, object + 4, data);
  Store(base, object + 28, 0);
  Store(base, object + 16, index);
  Store(base, object + 24, data + data_size);
  Store(base, object + 32, 0);
  Store(base, object + 36, 0xFFFFFFFFu);
  Store(base, object + 12, 0);
}

// Calls `function` through `calls` from a frame of `frame_bytes` below
// `stack`, its back chain written as the original's stwu writes it.
uint32_t CallGuest(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t frame_bytes,
                   uint32_t lr, uint32_t function, std::initializer_list<uint32_t> args) {
  const uint32_t frame = stack - frame_bytes;
  Store(base, frame, stack);
  return calls.call(calls.user, function, args.begin(), int(args.size()), frame, lr);
}

// An object's key through its vtable's slot 0, from a caller whose frame
// (112 bytes in both callers) is below `stack`; the guest's key slots write
// the size at the frame's +80, as the originals have them.
uint32_t KeyOf(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t lr, uint32_t object,
               uint32_t* size) {
  const uint32_t slot = Load(base, Load(base, object));
  switch (slot) {
    case kPoolObjectKey:
      return PoolObjectKey(object, size);
    case kAreaLightsKey:
      return AreaLightsKey(object, size);
    case kAreaModelKey:
      return AreaModelKey(object, size);
    default:
      break;
  }
  const uint32_t frame = stack - 112;
  Store(base, frame + 80, 0);
  const uint32_t key = CallGuest(base, calls, stack, 112, lr, slot, {object, frame + 80});
  *size = Load(base, frame + 80);
  return key;
}

// An object reset through its vtable's slot 2.
void ResetThroughVtable(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t frame_bytes,
                        uint32_t lr, uint32_t object) {
  const uint32_t slot = Load(base, Load(base, object) + 8);
  if (slot == kPoolObjectReset) {
    ResetObject(base, object);
    return;
  }
  CallGuest(base, calls, stack, frame_bytes, lr, slot, {object});
}

}  // namespace

// --- Pool objects ---

uint32_t AllocData(uint8_t* base, uint32_t data, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_820FB8B8 PoolObject::AllocData");
  const uint32_t used = Load(base, data + 8);
  const uint32_t at = used + Load(base, data + 0);
  Store(base, data + 8, Round64(size) + used);
  return at;
}

uint32_t InUse(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB8E0 PoolObject::InUse");
  return Load(base, object + kObjectFlags) & 1;
}

uint32_t IsLoaded(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB8F0 PoolObject::IsLoaded");
  return (Load(base, object + kObjectFlags) & 0x9) == 0x9 ? 1 : 0;
}

uint32_t IsVisible(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB908 PoolObject::IsVisible");
  return (Load(base, object + kObjectFlags) & 0x19) == 0x19 ? 1 : 0;
}

uint32_t IsExpiring(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB920 PoolObject::IsExpiring");
  return (Load(base, object + kObjectFlags) & 0x29) == 0x29 ? 1 : 0;
}

void SetVisible(uint8_t* base, uint32_t object, uint32_t on) {
  ITC_NATIVE_RUNNING("sub_820FB938 PoolObject::SetVisible");
  uint32_t flags = Load(base, object + kObjectFlags);
  if (on != 0) {
    if ((flags & 0x9) != 0x9) {
      return;
    }
    flags = (flags & ~0x30u) | 0x10;
  } else {
    flags &= ~0x10u;
  }
  Store(base, object + kObjectFlags, flags);
}

void SetExpiring(uint8_t* base, uint32_t object, uint32_t on) {
  ITC_NATIVE_RUNNING("sub_820FB968 PoolObject::SetExpiring");
  uint32_t flags = Load(base, object + kObjectFlags);
  if (on != 0) {
    if ((flags & 0x9) != 0x9) {
      return;
    }
    flags = (flags & ~0x30u) | 0x20;
  } else {
    flags &= ~0x20u;
  }
  Store(base, object + kObjectFlags, flags);
}

void ClearLoadFlags(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB998 PoolObject::ClearLoadFlags");
  Store(base, object + kObjectFlags, Load(base, object + kObjectFlags) & ~0x3Cu);
}

void MarkLoaded(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB9A8 PoolObject::MarkLoaded");
  Store(base, object + kObjectFlags, (Load(base, object + kObjectFlags) & ~0x2u) | 0x8);
}

void ResetObject(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB9D8 PoolObject::Reset");
  Store(base, object + 32, 0);
  Store(base, object + 36, 0xFFFFFFFFu);
  Store(base, object + 12, 0);
}

uint32_t PoolObjectKey(uint32_t, uint32_t* size) {
  ITC_NATIVE_RUNNING("sub_820FB9F0 PoolObject::Key");
  *size = 0;
  return 0;
}

uint32_t AreaLightsKey(uint32_t object, uint32_t* size) {
  ITC_NATIVE_RUNNING("sub_820F1C20 AreaLights::Key");
  *size = 4;
  return object + 84;
}

uint32_t AreaModelKey(uint32_t object, uint32_t* size) {
  ITC_NATIVE_RUNNING("sub_820F1CD8 AreaModel::Key");
  *size = 4;
  return object + 64;
}

void ConstructObject(uint8_t* base, uint32_t object, uint32_t own_size, uint32_t data_size,
                     uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820FBB20 PoolObject::PoolObject");
  Construct(base, object, own_size, data_size, index);
}

uint32_t SetKey(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t object,
                uint32_t key) {
  ITC_NATIVE_RUNNING("sub_820FBA00 PoolObject::SetKey");
  uint32_t size = 0;
  const uint32_t buffer = KeyOf(base, calls, stack, 0x820FBA30, object, &size);
  if (size == 0 || buffer == 0) {
    return buffer;
  }
  if (!c_runtime::Memcpy(base, buffer, key, size)) {
    return CallGuest(base, calls, stack, 112, 0x820FBA4C, kMemcpy, {buffer, key, size});
  }
  return buffer;
}

uint32_t KeyEquals(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t object,
                   uint32_t key) {
  ITC_NATIVE_RUNNING("sub_820FBA60 PoolObject::KeyEquals");
  uint32_t size = 0;
  const uint32_t buffer = KeyOf(base, calls, stack, 0x820FBA94, object, &size);
  if (size == 0 || buffer == 0) {
    return 1;
  }
  for (uint32_t i = 0; i != size; i++) {
    if (guest::LoadU8(base, buffer + i) != guest::LoadU8(base, key + i)) {
      return 0;
    }
  }
  return 1;
}

// --- The pools ---

uint32_t PoolRecord(uint8_t* base, uint32_t set, uint32_t pool) {
  ITC_NATIVE_RUNNING("sub_820FBB10 ObjectPools::Pool");
  return pool * 24 + Load(base, set + 4);
}

namespace {

uint32_t ObjectAt(uint8_t* base, uint32_t record, uint32_t index) {
  const uint32_t stride = Load(base, record + 8) + Load(base, record + 4);
  return stride * index + Load(base, record + 12);
}

}  // namespace

uint32_t PoolObjectAt(uint8_t* base, uint32_t set, uint32_t pool, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820FC0E8 ObjectPools::Object");
  return ObjectAt(base, pool * 24 + Load(base, set + 4), index);
}

uint32_t Acquire(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set, uint32_t pool,
                 uint32_t key) {
  ITC_NATIVE_RUNNING("sub_820FBF08 ObjectPools::Acquire");
  const uint32_t record = pool * 24 + Load(base, set + 4);
  const uint32_t object = Load(base, record + 16);
  if (object == 0) {
    return 0xFFFFFFFFu;
  }
  Store(base, object + 36, 0xFFFFFFFFu);
  Store(base, object + kObjectFlags, Load(base, object + kObjectFlags) | 1);
  SetKey(base, calls, stack - 112, object, key);
  // Off the free list.
  const uint32_t next = Load(base, Load(base, record + 16) + 24);
  Store(base, record + 16, next);
  if (next != 0) {
    Store(base, next + 20, 0);
  }
  // Onto the used list's head.
  const uint32_t used = Load(base, record + 20);
  if (used == 0) {
    Store(base, object + 24, 0);
    Store(base, object + 20, 0);
  } else {
    Store(base, object + 24, used);
    Store(base, object + 20, Load(base, Load(base, record + 20) + 20));
    Store(base, Load(base, record + 20) + 20, object);
  }
  Store(base, record + 20, object);
  return Load(base, object + 16);
}

uint32_t Find(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set, uint32_t pool,
              uint32_t key) {
  ITC_NATIVE_RUNNING("sub_820FBE80 ObjectPools::Find");
  const uint32_t record = pool * 24 + Load(base, set + 4);
  for (uint32_t object = Load(base, record + 20); object != 0;
       object = Load(base, object + 24)) {
    if ((Load(base, object + kObjectFlags) & 1) != 0 &&
        KeyEquals(base, calls, stack - 112, object, key) != 0) {
      return Load(base, object + 16);
    }
  }
  return 0xFFFFFFFFu;
}

uint32_t LoadObject(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set,
                    uint32_t pool, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820FBFD0 ObjectPools::Load");
  const uint32_t object = ObjectAt(base, pool * 24 + Load(base, set + 4), index);
  if (object != 0) {
    CallGuest(base, calls, stack, 96, 0x820FC01C, Load(base, Load(base, object) + 4), {object});
  }
  return object;
}

uint32_t Release(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set, uint32_t pool,
                 uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820FC038 ObjectPools::Release");
  const uint32_t record = pool * 24 + Load(base, set + 4);
  const uint32_t object = ObjectAt(base, record, index);
  const uint32_t following = Load(base, object + 24);
  ResetThroughVtable(base, calls, stack, 112, 0x820FC080, object);
  // Off the used list.
  const uint32_t previous = Load(base, object + 20);
  if (previous != 0) {
    Store(base, previous + 24, Load(base, object + 24));
  } else {
    Store(base, record + 20, Load(base, object + 24));
  }
  const uint32_t next = Load(base, object + 24);
  if (next != 0) {
    Store(base, next + 20, Load(base, object + 20));
  }
  // Onto the free list's head.
  const uint32_t free_head = Load(base, record + 16);
  Store(base, object + 20, 0);
  Store(base, object + 24, free_head);
  const uint32_t head = Load(base, record + 16);
  if (head != 0) {
    Store(base, head + 20, object);
  }
  Store(base, record + 16, object);
  return following;
}

bool Init(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set, uint32_t count,
          uint32_t descriptors, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820FBBE8 ObjectPools::Init");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  const int32_t pools = int32_t(count);
  const uint32_t records = Round64(count * 24);
  uint32_t size = records;
  for (int32_t i = 0; i < pools; i++) {
    const uint32_t d = descriptors + 24 * uint32_t(i);
    size += Round64(Load(base, d + 4)) * Load(base, d + 12) +
            Round64(Load(base, d + 8)) * Load(base, d + 16);
  }
  // XPhysicalAlloc(size, -1, 64, 4): any physical address; the protection
  // (PAGE_READWRITE) has no X_MEM_LARGE_PAGES for its check to clear.
  const uint32_t block = memory::AllocPhysical(size, 0, 0xFFFFFFFFu, 64, 4);
  *r3 = block;
  Store(base, set + 0, count);
  Store(base, set + 4, block);
  uint32_t cursor = records + block;
  for (int32_t i = 0; i < pools; i++) {
    const uint32_t d = descriptors + 24 * uint32_t(i);
    const uint32_t record = 24 * uint32_t(i) + Load(base, set + 4);
    const uint32_t object_size = Round64(Load(base, d + 4));
    const uint32_t data_size = Round64(Load(base, d + 8));
    const uint32_t objects = Load(base, d + 12);
    Store(base, record + 12, cursor);
    Store(base, record + 0, objects);
    Store(base, record + 4, object_size);
    Store(base, record + 8, data_size);
    if (int32_t(objects) > 0) {
      uint32_t at = cursor;
      uint32_t previous = 0;
      int32_t k = 0;
      do {
        const uint32_t factory = Load(base, d + 20);
        if (factory != 0) {
          at = CallGuest(base, calls, stack, 208, 0x820FBD10, factory,
                         {uint32_t(i), at, object_size, data_size, uint32_t(k)});
          *r3 = at;
        } else if (at != 0) {
          Construct(base, at, object_size, data_size, uint32_t(k));
        } else {
          at = 0;
        }
        Store(base, at + 20, previous);
        previous = at;
        k++;
        at = Load(base, at + 24);
      } while (k < int32_t(Load(base, record + 0)));
      if (previous != 0) {
        Store(base, previous + 24, 0);
      }
    }
    Store(base, record + 20, 0);
    Store(base, record + 16, Load(base, record + 12));
    cursor += Load(base, d + 16) * data_size + Load(base, d + 12) * object_size;
  }
  return true;
}

bool Shutdown(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820FBDC8 ObjectPools::Shutdown");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  for (int32_t p = 0; p < int32_t(Load(base, set + 0)); p++) {
    const uint32_t records = Load(base, set + 4);
    const uint32_t offset = 24 * uint32_t(p);
    for (int32_t k = 0; k < int32_t(Load(base, records + offset)); k++) {
      const uint32_t object = ObjectAt(base, offset + Load(base, set + 4), uint32_t(k));
      if (object != 0) {
        ResetThroughVtable(base, calls, stack, 144, 0x820FBE38, object);
      }
    }
  }
  const uint32_t block = Load(base, set + 4);
  *r3 = block;
  if (block != 0) {
    memory::FreePhysical(block);
    Store(base, set + 0, 0);
    Store(base, set + 4, 0);
  }
  return true;
}

// --- The scenery's lists ---

uint32_t ModelAsset(uint8_t* base, uint32_t config, uint32_t area, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820F1530 SceneryConfig::ModelAsset");
  const uint32_t table = Load(base, config + 28);
  if (area >= Load(base, table + 16)) {
    return 0xFFFFFFFFu;
  }
  const uint32_t entry = area * 12 + table;
  const uint32_t first = Load(base, entry + 24);
  const uint32_t last = Load(base, entry + 28);
  if (first > last) {
    return 0xFFFFFFFFu;
  }
  // The original counts up from the first to the last; with the last at
  // 0xFFFFFFFF its count wraps and never ends before reaching `model`.
  if (last == 0xFFFFFFFFu || model <= last - first) {
    return first + model;
  }
  return 0xFFFFFFFFu;
}

void ListClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820F1588 SceneryList::Clear");
  Store(base, list + 4, 0);
  Store(base, list + 8, 0);
}

uint32_t ListAdd(uint8_t* base, uint32_t list, double distance, uint32_t area, uint32_t model,
                 uint32_t mask, uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_820F1888 SceneryList::Add");
  const uint32_t count = Load(base, list + 8);
  if (count != 0) {
    const uint32_t pointers = Load(base, list + 16);
    for (uint32_t k = count; k != 0; k--) {
      const uint32_t item = Load(base, pointers + 4 * k - 4);
      r3 = guest::LoadU16(base, item + 8);
      if (area != r3) {
        continue;
      }
      r3 = guest::LoadU16(base, item + 10);
      if (model != r3) {
        continue;
      }
      // Found: the smaller distance (as the recompiled fcmpu and ble have
      // it) and the mask ORed in.
      double kept = guest::LoadFloat(base, item + 0);
      if (kept > distance) {
        kept = distance;
      }
      const uint32_t old_mask = Load(base, item + 4);
      guest::StoreFloat(base, item + 0, float(kept));
      Store(base, item + 4, old_mask | mask);
      return r3;
    }
  }
  const uint32_t item = (count << 4) + Load(base, list + 20);
  Store(base, Load(base, list + 16) + 4 * count, item);
  guest::StoreFloat(base, item + 0, float(distance));
  Store(base, item + 4, mask);
  guest::StoreU16(base, item + 8, uint16_t(area));
  guest::StoreU16(base, item + 10, uint16_t(model));
  Store(base, list + 8, Load(base, list + 8) + 1);
  return r3;
}

namespace {

float Distance(uint8_t* base, uint32_t list, int32_t index) {
  return guest::LoadFloat(base, Load(base, Load(base, list + 16) + 4 * uint32_t(index)));
}

void Sort(uint8_t* base, uint32_t list, int32_t first, int32_t last) {
  for (;;) {
    const float pivot = Distance(base, list, (first + last) / 2);
    int32_t i = first;
    int32_t j = last;
    for (;;) {
      while (Distance(base, list, i) < pivot) {
        i++;
      }
      while (Distance(base, list, j) > pivot) {
        j--;
      }
      if (i >= j) {
        break;
      }
      const uint32_t pointers = Load(base, list + 16);
      const uint32_t at_j = Load(base, pointers + 4 * uint32_t(j));
      const uint32_t at_i = Load(base, pointers + 4 * uint32_t(i));
      Store(base, pointers + 4 * uint32_t(i), at_j);
      Store(base, Load(base, list + 16) + 4 * uint32_t(j), at_i);
      i++;
      j--;
    }
    if (first < i - 1) {
      Sort(base, list, first, i - 1);
    }
    if (last <= j + 1) {
      return;
    }
    first = j + 1;
  }
}

}  // namespace

void ListSort(uint8_t* base, uint32_t list, int32_t first, int32_t last) {
  ITC_NATIVE_RUNNING("sub_820F1930 SceneryList::Sort");
  Sort(base, list, first, last);
}

void LimitLargeModels(uint8_t* base, uint32_t manager, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820F2B80 Scenery::LimitLargeModels");
  const uint32_t list = manager + 144;
  const uint32_t table = stack - 80;
  const uint32_t limit = int32_t(Load(base, manager + 12)) == 1 ? 10 : 5;
  const uint32_t count = Load(base, list + 8);
  Store(base, table + 0, 24);
  Store(base, table + 4, 11);
  Store(base, table + 8, 18);
  Store(base, table + 12, 9);
  Store(base, table + 16, 54);
  const uint32_t cap = Load(base, manager + 104);
  if (count <= cap || cap == 0) {
    return;
  }
  // Whether the item is large: its model at or past its area's threshold.
  auto large = [&](uint32_t item) {
    const uint32_t threshold = Load(base, table + 4 * guest::LoadU16(base, item + 8));
    return threshold <= guest::LoadU16(base, item + 10);
  };
  uint32_t seen = 0;
  for (uint32_t i = 0; i < Load(base, manager + 104); i++) {
    const uint32_t pointers = Load(base, list + 16);
    const uint32_t item = Load(base, pointers + 4 * i);
    if (!large(item)) {
      continue;
    }
    if (limit >= seen) {
      seen++;
      continue;
    }
    for (uint32_t j = i + 1; j < count - 1; j++) {
      const uint32_t other = Load(base, pointers + 4 * j);
      if (!large(other)) {
        const uint32_t at_i = Load(base, Load(base, list + 16) + 4 * i);
        Store(base, Load(base, list + 16) + 4 * j, at_i);
        Store(base, Load(base, list + 16) + 4 * i, other);
        break;
      }
    }
  }
}

void ModelListInit(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820F1A20 ModelList::Init");
  const uint32_t nodes = list + 4;
  for (uint32_t i = 0; i < 64; i++) {
    guest::StoreU16(base, nodes + 4 * i + 2, uint16_t(i));
    guest::StoreU16(base, nodes + 4 * i + 0, uint16_t(i + 1));
  }
  guest::StoreU16(base, nodes + 256, 0);
  guest::StoreU16(base, nodes + 264, 0);
  guest::StoreU16(base, nodes + 252, 0xFFFF);
  guest::StoreU16(base, nodes + 260, 0xFFFF);
  guest::StoreU16(base, nodes + 262, 0xFFFF);
  guest::StoreU16(base, nodes + 258, 0xFFFF);
  Store(base, list + 0, 0);
}

void ClearGlobal(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820F1480 Scenery::ClearGlobal");
  Store(base, kManager, 0);
}

void Init(uint8_t* base, uint32_t manager) {
  ITC_NATIVE_RUNNING("sub_820F1360 Scenery::Init");
  Store(base, kManager, manager);
  Store(base, manager + 52, 0);
  Store(base, Load(base, kManager) + 56, 0);
  for (uint32_t at = 60; at < 108; at += 8) {
    c_runtime::XMemSet(base, Load(base, kManager) + at, 0, 8);
  }
  for (uint32_t at : {108u, 112u, 184u, 188u}) {
    Store(base, Load(base, kManager) + at, 0);
  }
  c_runtime::XMemSet(base, Load(base, kManager) + 116, 0, 28);
  c_runtime::XMemSet(base, Load(base, kManager) + 144, 0, 28);
  Store(base, Load(base, kManager) + 172, 0xFFFFFFFFu);
  Store(base, Load(base, kManager) + 176, 0xFFFFFFFFu);
  Store(base, Load(base, kManager) + 192, 0);
}

uint32_t ReleaseUnused(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                       uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820F17C0 SceneryList::ReleaseUnused");
  const uint32_t frame = stack - 112;
  const uint32_t pool = Load(base, list + 0);
  uint32_t r3 = PoolRecord(base, manager, pool);
  uint32_t object = Load(base, r3 + 20);
  while (object != 0) {
    r3 = IsLoaded(base, object);
    if (r3 != 0) {
      r3 = IsVisible(base, object);
      if (r3 == 0) {
        // Loaded and not seen this update.
        r3 = IsExpiring(base, object);
        if (r3 == 0) {
          SetExpiring(base, object, 1);
          r3 = object;
          Store(base, object + 36, 2);
        } else {
          const int32_t countdown = int32_t(Load(base, object + 36));
          if (countdown <= 0) {
            r3 = Release(base, calls, frame, manager, pool, Load(base, object + 16));
            object = r3;
            continue;
          }
          Store(base, object + 36, uint32_t(countdown - 1));
        }
        object = Load(base, object + 24);
        continue;
      }
    }
    // Not loaded, or seen: the seen flag cleared.
    SetVisible(base, object, 0);
    r3 = object;
    object = Load(base, object + 24);
  }
  return r3;
}

uint32_t StartLoad(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager) {
  ITC_NATIVE_RUNNING("sub_820F1E00 Scenery::StartLoad");
  const uint32_t frame = stack - 128;
  // The configuration's distance for every item (0x82000A34).
  constexpr uint32_t kDistance = 0x82000A34;
  uint32_t index = Acquire(base, calls, frame, manager, 2, 0);
  LoadObject(base, calls, frame, manager, 2, index);
  uint32_t r3 = PoolObjectAt(base, manager, 2, index);
  const uint32_t mode = Load(base, manager + 8);
  Store(base, manager + 176, index);
  Store(base, manager + 112, r3);
  Store(base, manager + 172, 2);
  if (mode == 0) {
    const uint32_t list_a = manager + 116, list_b = manager + 144;
    Store(base, list_a + 4, 0);
    Store(base, list_a + 8, 0);
    Store(base, list_b + 4, 0);
    Store(base, list_b + 8, 0);
    const double distance = guest::LoadFloat(base, kDistance);
    for (uint32_t i = 0; i < Load(base, manager + 128); i++) {
      const uint32_t entry = Load(base, manager + 140) + 12 * i;
      r3 = ListAdd(base, list_a, distance, guest::LoadU16(base, entry + 0),
                   guest::LoadU16(base, entry + 2), 0, manager);
    }
    for (uint32_t i = 0; i < Load(base, manager + 156); i++) {
      const uint32_t entry = Load(base, manager + 168) + 12 * i;
      r3 = ListAdd(base, list_b, distance, guest::LoadU16(base, entry + 0),
                   guest::LoadU16(base, entry + 2), 0, manager);
    }
  }
  if (Load(base, manager + 44) != 0) {
    index = Acquire(base, calls, frame, manager, 0, 0);
    LoadObject(base, calls, frame, manager, 0, index);
    r3 = PoolObjectAt(base, manager, 0, index);
    Store(base, manager + 172, 0);
    Store(base, manager + 176, index);
    Store(base, manager + 108, r3);
  }
  Store(base, manager + 192, 1);
  return r3;
}

// --- The texture animations ---

uint32_t AdvanceTextureAnimations(uint8_t* base, uint32_t course_set) {
  ITC_NATIVE_RUNNING("sub_820F11A8 TextureAnimations::Advance");
  const uint32_t set = Load(base, course_set + 1060);
  return set == 0 ? 0 : AdvanceAnimationSet(base, set);
}

uint32_t AdvanceAnimationSet(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82146B20 AnimationSet::Advance");
  uint32_t r3 = set;
  for (int32_t i = 0; i < int32_t(Load(base, set + 0)); i++) {
    const uint32_t offsets = (Load(base, set + 0) + 1) * 4;
    r3 = offsets + Load(base, set + 4 + 4 * uint32_t(i)) + set;
    AdvanceAnimation(base, r3);
  }
  return r3;
}

void AdvanceAnimation(uint8_t* base, uint32_t animation) {
  ITC_NATIVE_RUNNING("sub_82146AC0 Animation::Advance");
  if (int32_t(Load(base, animation + 8)) > int32_t(Load(base, animation + 12))) {
    Store(base, animation + 8, 0);
    Store(base, animation + 4, 0);
  }
  const uint32_t next = Load(base, animation + 4) + 1;
  if (int32_t(next) < int32_t(Load(base, animation + 0)) &&
      int32_t(Load(base, animation + 8)) >= int32_t(Load(base, animation + 4 * (next + 4)))) {
    Store(base, animation + 4, next);
  }
  Store(base, animation + 8, Load(base, animation + 8) + 1);
}

}  // namespace course_scenery
