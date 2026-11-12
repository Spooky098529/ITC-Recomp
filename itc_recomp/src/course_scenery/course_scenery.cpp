// The course scenery (course_scenery.h; docs/functions/course-scenery.md).

#include "course_scenery/course_scenery.h"

#include "guest.h"

#include "archive/archive.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "game_data/data_file.h"
#include "memory/c_runtime_heap.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "models/model.h"
#include "models/model_load.h"
#include "models/texture_provider.h"
#include "models/xpr.h"
#include "native/buffers.h"
#include "scenery_lists/scenery_lists.h"

#include <cstdlib>
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

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Course scenery: {} refused after its checks", what);
    std::abort();
  }
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

uint64_t PoolSizes(uint8_t* base, uint32_t area_counts, uint32_t model_counts, uint32_t out,
                   uint32_t config) {
  ITC_NATIVE_RUNNING("sub_820F2030 Scenery::PoolSizes");
  uint64_t r3 = 0;
  // Summed in mode 0, else the largest (signed).
  auto add = [&](int32_t& total, uint32_t size) {
    if (int32_t(Load(base, config)) == 0) {
      total = int32_t(uint32_t(total) + size);
    } else if (total < int32_t(size)) {
      total = int32_t(size);
    }
  };
  for (uint32_t pool = 0; pool < 5; pool++) {
    int32_t total = 0;
    if (pool == 0) {
      const uint32_t size = archive::AssetSize(base, Load(base, Load(base, config + 24)));
      r3 = size;
      total = int32_t(size);
    } else if (pool == 2) {
      const uint32_t asset = Load(base, Load(base, config + 28) + 8);
      r3 = asset;
      if (int32_t(asset) >= 0) {
        const uint32_t size = archive::AssetSize(base, asset);
        r3 = size;
        total = int32_t(uint32_t(total) + size);
      }
    } else if (pool == 3) {
      uint32_t area = 0;
      for (;; area++) {
        const uint32_t table = Load(base, config + 28);
        if (area < Load(base, table + 16)) {
          r3 = Load(base, area * 12 + table + 20);
        } else {
          r3 = ~0ull;  // li r3,-1
        }
        if (int32_t(uint32_t(r3)) < 0) {
          break;
        }
        const uint32_t size = archive::AssetSize(base, uint32_t(r3));
        r3 = size;
        add(total, size);
      }
      Store(base, area_counts, area);
      Store(base, area_counts + 4, area);
    } else if (pool == 4) {
      uint32_t models = 0;
      for (uint32_t area = 0;; area++) {
        const uint32_t table = Load(base, config + 28);
        const uint32_t count = Load(base, table + 16);
        const uint32_t entry = area * 12 + table;
        if (area >= count || int32_t(Load(base, entry + 20)) < 0) {
          break;
        }
        const int32_t n = int32_t(Load(base, entry + 28) - Load(base, entry + 24) + 1);
        for (int32_t model = 0; model < n; model++) {
          const uint32_t asset = ModelAsset(base, config, area, uint32_t(model));
          r3 = asset == 0xFFFFFFFFu ? ~0ull : asset;
          if (int32_t(asset) < 0) {
            continue;
          }
          const uint32_t size = archive::AssetSize(base, asset);
          r3 = size;
          add(total, size);
          models++;
        }
      }
      Store(base, model_counts, models);
      Store(base, model_counts + 4, 16);
      Store(base, model_counts + 4, models < 16 ? models : 16);
    }
    Store(base, out + 4 * pool, uint32_t(total));
  }
  return r3;
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

uint32_t AllocPhysical(uint8_t* base, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_820F1490 Scenery::AllocPhysical");
  // XPhysicalAlloc(size, -1, 0, 4): any physical address, no alignment;
  // PAGE_READWRITE has no X_MEM_LARGE_PAGES for its check to clear.
  const uint32_t block = memory::AllocPhysical(size, 0, 0xFFFFFFFFu, 0, 4);
  const uint32_t held = memory::PhysicalSize(block);
  Store(base, kPhysicalHeld, held + Load(base, kPhysicalHeld));
  return block;
}

void FreePhysical(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820F14E8 Scenery::FreePhysical");
  const uint32_t held = memory::PhysicalSize(block);
  Store(base, kPhysicalHeld, Load(base, kPhysicalHeld) - held);
  memory::FreePhysical(block);
}

bool UnloadPools(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                 uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820F1D20 Scenery::UnloadPools");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  const uint32_t frame = stack - 112;
  Store(base, frame, stack);
  uint32_t shutdown_r3 = 0;
  Require(Shutdown(base, calls, frame, manager, &shutdown_r3), "Scenery::UnloadPools' Shutdown");
  if (Load(base, manager + 188) != 0) {
    Store(base, manager + 188, 0);
  }
  const uint32_t pool0 = Load(base, manager + 184);
  Store(base, manager + 180, 0);
  if (pool0 != 0) {
    FreePhysical(base, pool0);
    Store(base, manager + 184, 0);
  }
  for (uint32_t at : {168u, 164u, 160u}) {
    if (const uint32_t block = Load(base, manager + at); block != 0) {
      FreePhysical(base, block);
    }
  }
  c_runtime::XMemSet(base, manager + 144, 0, 28);
  for (uint32_t at : {140u, 136u, 132u}) {
    if (const uint32_t block = Load(base, manager + at); block != 0) {
      FreePhysical(base, block);
    }
  }
  c_runtime::XMemSet(base, manager + 116, 0, 28);
  *r3 = manager + 116;
  return true;
}

bool Unload(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820F2B38 Scenery::Unload");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  const uint32_t frame = stack - 96;
  Store(base, frame, stack);
  if (const uint32_t areas = Load(base, manager + 224); areas != 0) {
    FreePhysical(base, areas);
    Store(base, manager + 224, 0);
  }
  return UnloadPools(base, calls, frame, manager, r3);
}

uint32_t Destroy(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820F2AF8 Scenery::Destroy");
  ClearGlobal(base);
  FreePhysical(base, Load(base, kCreated));
  Store(base, kCreated, 0);
  return 0;
}

uint32_t Create(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820F2F88 Scenery::Create");
  const uint32_t manager = AllocPhysical(base, 248);
  Store(base, kCreated, manager);
  // lfs and stfs: the constants' bits (none is a NaN).
  Store(base, manager + 200, Load(base, 0x820067B8));
  Store(base, manager + 196, 0x38435);
  Store(base, manager + 204, Load(base, 0x820067B4));
  Store(base, manager + 208, Load(base, 0x82001FAC));
  Store(base, manager + 212, Load(base, 0x820067B0));
  Store(base, manager + 224, 0);
  if (manager + 228 != 0) {
    Require(models::TextureProviderConstruct(base, manager + 228, -1).has_value(),
            "Scenery::Create's TextureProvider");
  }
  Init(base, manager);
  return Load(base, kManager) + 144;
}

uint32_t Setup(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
               uint32_t config) {
  ITC_NATIVE_RUNNING("sub_820F3020 Scenery::Setup");
  const uint32_t frame = stack - 112;
  Store(base, frame, stack);
  SetupPools(base, calls, frame, manager, config);
  const uint32_t areas = Load(base, manager + 92);
  const uint32_t models = Load(base, manager + 100);
  Store(base, manager + 216, 0);
  Store(base, manager + 220, areas);
  const uint32_t block = AllocPhysical(base, areas * 272);
  Store(base, manager + 224, block);
  for (uint32_t i = 0; i < Load(base, manager + 220); i++) {
    const uint32_t entry = Load(base, manager + 224) + 272 * i;
    Store(base, entry + 4, 0);
    Store(base, entry + 8, models);
  }
  return block;
}

uint32_t SetupPools(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                    uint32_t config) {
  ITC_NATIVE_RUNNING("sub_820F2720 Scenery::SetupPools");
  const uint32_t frame = stack - 336;
  Store(base, frame, stack);
  const uint32_t copy = manager + 8;  // the configuration's copy
  Store(base, kPhysicalHeld, 0);
  Store(base, manager + 180, 0);
  Store(base, manager + 184, 0);
  if (!native::Buffers_Memcpy(base, copy, config, 44, 0x820F2758)) {
    CallGuest(base, calls, frame, 0, 0x820F2758, kMemcpy, {copy, config, 44});
  }
  if (int32_t(Load(base, copy)) == 2 && Load(base, manager + 12) == 0) {
    Store(base, copy, Load(base, Load(base, manager + 40)));
  }
  const uint32_t sizes = frame + 80;  // PoolSizes' five words
  PoolSizes(base, manager + 92, manager + 100, sizes, copy);
  // List A: the areas (pool 3).
  const uint32_t area_count = Load(base, manager + 92);
  Store(base, manager + 120, 0);
  Store(base, manager + 124, 0);
  Store(base, manager + 128, area_count);
  Store(base, manager + 116, 3);
  Store(base, manager + 132, AllocPhysical(base, area_count * 4));
  Store(base, manager + 136, AllocPhysical(base, Load(base, manager + 128) * 16));
  Store(base, manager + 140, AllocPhysical(base, Load(base, manager + 128) * 12));
  c_runtime::XMemSet(base, Load(base, manager + 136), 0, Load(base, manager + 128) * 16);
  c_runtime::XMemSet(base, Load(base, manager + 140), 0, Load(base, manager + 128) * 12);
  // List B: the models (pool 4).
  const uint32_t model_count = Load(base, manager + 100);
  Store(base, manager + 148, 0);
  Store(base, manager + 152, 0);
  Store(base, manager + 156, model_count);
  Store(base, manager + 144, 4);
  Store(base, manager + 160, AllocPhysical(base, model_count * 4));
  Store(base, manager + 164, AllocPhysical(base, Load(base, manager + 156) * 16));
  Store(base, manager + 168, AllocPhysical(base, Load(base, manager + 156) * 12));
  c_runtime::XMemSet(base, Load(base, manager + 164), 0, Load(base, manager + 156) * 16);
  c_runtime::XMemSet(base, Load(base, manager + 168), 0, Load(base, manager + 156) * 12);
  // The tables, from the configuration's areas (its +28: +16 the count,
  // 12-byte entries from +20: the light asset, the first and last model).
  uint32_t written_a = 0, written_b = 0;
  for (uint32_t area = 0; written_a < Load(base, manager + 128); area++) {
    uint32_t table = Load(base, copy + 28);
    const int32_t light =
        area < Load(base, table + 16) ? int32_t(Load(base, table + 12 * area + 20)) : -1;
    if (light < 0) {
      continue;
    }
    const uint32_t a = Load(base, manager + 140) + 12 * written_a;
    written_a++;
    const uint32_t key = area & 0xFFFF;
    guest::StoreU16(base, a + 2, 0);
    Store(base, a + 4, uint32_t(light));
    guest::StoreU16(base, a + 0, uint16_t(key));
    table = Load(base, copy + 28);
    int32_t models = -1;
    if (key < Load(base, table + 16)) {
      const uint32_t e = table + 12 * key;
      models = int32_t(Load(base, e + 28) - Load(base, e + 24) + 1);
    }
    for (int32_t model = 0; model < models; model++) {
      const uint32_t asset = ModelAsset(base, copy, area, uint32_t(model));
      if (int32_t(asset) < 0) {
        continue;
      }
      const uint32_t b = Load(base, manager + 168) + 12 * written_b;
      written_b++;
      guest::StoreU16(base, b + 0, uint16_t(key));
      guest::StoreU16(base, b + 2, uint16_t(model));
      Store(base, b + 4, asset);
    }
  }
  // The five pools' descriptors: +0 the pool, +4 the object size, +8 the
  // data size, +12 and +16 the counts, +20 the factory (MakePoolObject).
  constexpr uint32_t kMakePoolObject = 0x820F2638;
  const uint32_t descriptors = frame + 112;
  const uint32_t first_size = Load(base, sizes + 0);
  uint32_t object_size = 0, count = 0, data_count = 0;
  for (uint32_t pool = 0; pool < 5; pool++) {
    const uint32_t mode = Load(base, copy);
    switch (pool) {
      case 0:
        if (Load(base, manager + 44) != 0) {
          Store(base, manager + 180, first_size);
          object_size = 60;
          count = 1;
          data_count = 0;
          Store(base, manager + 184, AllocPhysical(base, first_size));
          Store(base, sizes + 0, 0);
        }
        break;
      case 1:
        if (Load(base, manager + 48) != 0) {
          object_size = 60;
          count = 0;
          data_count = 0;
        }
        break;
      case 2:
        object_size = 3824;
        count = 1;
        data_count = 1;
        break;
      case 3:
        object_size = 88;
        count = mode == 0 ? Load(base, manager + 92) : Load(base, manager + 96);
        data_count = mode == 0 ? 1 : count;
        break;
      case 4: {
        object_size = 560;
        count = mode == 0 ? Load(base, manager + 100) : Load(base, manager + 104);
        data_count = mode == 0 ? 1 : count;
        const uint32_t limit = Load(base, 0x826A5130);
        const uint32_t extra = limit < Load(base, sizes + 16) ? 0 : Load(base, 0x826A5134);
        Store(base, sizes + 16, 0);
        Store(base, manager + 188, extra);
        break;
      }
    }
    const uint32_t d = descriptors + 24 * pool;
    Store(base, d + 0, pool);
    Store(base, d + 4, object_size);
    Store(base, d + 12, count);
    Store(base, d + 16, data_count);
    Store(base, d + 8, Load(base, sizes + 4 * pool));
    Store(base, d + 20, kMakePoolObject);
  }
  uint32_t r3 = 0;
  Require(Init(base, calls, frame, manager, 5, descriptors, &r3), "Scenery::SetupPools' Init");
  Store(base, manager + 192, 0);
  return r3;
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

namespace {

constexpr uint32_t kQueueLength = 0x8209D7B0;  // Archive::QueueLength

uint32_t QueueLength(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t lr,
                     bool native_queue) {
  return native_queue ? archive::QueueLength(base)
                      : CallGuest(base, calls, stack, 160, lr, kQueueLength, {});
}

}  // namespace

uint32_t ListLoadAll(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                     uint32_t list, bool native_queue) {
  ITC_NATIVE_RUNNING("sub_820F1598 SceneryList::LoadAll");
  const uint32_t frame = stack - 160;
  const uint32_t pool = Load(base, list + 0);
  const uint32_t queued = QueueLength(base, calls, stack, 0x820F15B4, native_queue);
  for (uint32_t i = Load(base, list + 4); i < Load(base, list + 8); i++) {
    const uint32_t item = Load(base, Load(base, list + 16) + 4 * i);
    Store(base, item + 12, 0);
    if (Load(base, list + 12) == 0) {
      continue;
    }
    const uint32_t key = Load(base, item + 8);
    uint32_t entry = Load(base, list + 24);
    bool found = false;
    for (uint32_t n = 0;;) {
      if (Load(base, entry) == key) {
        found = true;
        break;
      }
      n++;
      entry += 12;
      if (n >= Load(base, list + 12)) {
        break;
      }
    }
    if (!found) {
      continue;
    }
    if (Load(base, entry + 8) == 0 && queued < 480) {
      const uint32_t index = Acquire(base, calls, frame, manager, pool, item + 8);
      Store(base, entry + 8, LoadObject(base, calls, frame, manager, pool, index));
      Store(base, manager + 172, pool);
      Store(base, manager + 176, index);
    }
    const uint32_t object = Load(base, entry + 8);
    if (object == 0) {
      continue;
    }
    Store(base, item + 12, object);
    SetExpiring(base, object, 0);
    SetVisible(base, Load(base, item + 12), 0);
    Store(base, list + 4, Load(base, list + 4) + 1);
  }
  return Load(base, list + 8) == Load(base, list + 4) ? 1 : 0;
}

uint32_t SceneryIsLoaded(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                         bool native_queue) {
  ITC_NATIVE_RUNNING("sub_820F1F68 Scenery::IsLoaded");
  const uint32_t frame = stack - 112;
  Store(base, frame, stack);
  if (Load(base, manager + 192) != 0) {
    if (ListLoadAll(base, calls, frame, manager, manager + 116, native_queue) == 0 ||
        ListLoadAll(base, calls, frame, manager, manager + 144, native_queue) == 0) {
      return 0;
    }
  }
  const uint32_t pool = Load(base, manager + 172);
  if (int32_t(pool) < 0) {
    return 0;
  }
  const uint32_t index = Load(base, manager + 176);
  if (int32_t(index) < 0) {
    return 0;
  }
  const uint32_t object = PoolObjectAt(base, manager, pool, index);
  if (object == 0) {
    return 0;
  }
  if (InUse(base, object) != 0 && IsLoaded(base, object) == 0) {
    return 0;
  }
  if (Load(base, manager + 192) != 0) {
    Store(base, manager + 192, 0);
  }
  return 1;
}

uint32_t ListStream(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                    uint32_t list, bool native_queue) {
  ITC_NATIVE_RUNNING("sub_820F16C0 SceneryList::Stream");
  const uint32_t frame = stack - 160;
  const uint32_t pool = Load(base, list + 0);
  const uint32_t queued = QueueLength(base, calls, stack, 0x820F16DC, native_queue);
  for (uint32_t i = 0; i < Load(base, list + 8); i++) {
    const uint32_t item = Load(base, Load(base, list + 16) + 4 * i);
    const uint32_t key = item + 8;
    uint32_t index = Find(base, calls, frame, manager, pool, key);
    if (int32_t(index) < 0) {
      if (queued < 960) {
        index = Acquire(base, calls, frame, manager, pool, key);
        if (int32_t(index) < 0) {
          Store(base, item + 12, 0);
          continue;
        }
        LoadObject(base, calls, frame, manager, pool, index);
        Store(base, manager + 172, pool);
        Store(base, manager + 176, index);
      }
      if (int32_t(index) < 0) {
        Store(base, item + 12, 0);
        continue;
      }
    }
    Store(base, item + 12, PoolObjectAt(base, manager, pool, index));
    SetExpiring(base, Load(base, item + 12), 0);
    SetVisible(base, Load(base, item + 12), 0);
  }
  return Load(base, list + 8) == Load(base, list + 4) ? 1 : 0;
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

// --- The files' parses ---

namespace {

// The originals the parses hand to where a callee's plain C++ declines,
// called through `calls` from 512 bytes below `stack`.
struct ParseCalls {
  uint8_t* base;
  const Calls* calls;
  uint32_t stack;

  uint32_t Call(uint32_t lr, uint32_t function, std::initializer_list<uint32_t> args) const {
    return CallGuest(base, *calls, stack, 512, lr, function, args);
  }
  static uint32_t Allocate(void* self, uint32_t size) {
    return static_cast<ParseCalls*>(self)->Call(0x821484C0, 0x825A6880, {size});
  }
  static void Construct(void* self, uint32_t model) {
    static_cast<ParseCalls*>(self)->Call(0x821484D0, 0x825948C0, {model});
  }
  static void Load(void* self, uint32_t model, uint32_t entry) {
    static_cast<ParseCalls*>(self)->Call(0x82148500, 0x82596920, {model, entry, 0});
  }
  static void Init(void* self, uint32_t object, uint32_t microcode) {
    static_cast<ParseCalls*>(self)->Call(0x82594DEC, 0x822A8740, {object, microcode});
  }
  static void Function(void* self, uint32_t function, uint32_t r3, uint32_t r4, uint32_t r5,
                       uint32_t return_address) {
    static_cast<ParseCalls*>(self)->Call(return_address, function, {r3, r4, r5});
  }

  models::LoadCalls ModelLoad() {
    models::LoadCalls load;
    load.init = &Init;
    load.call = &Function;
    load.user = this;
    return load;
  }
  scenery_lists::LoadCalls ListLoad() {
    scenery_lists::LoadCalls load;
    load.allocate = &Allocate;
    load.construct = &Construct;
    load.load = &Load;
    load.model_load = ModelLoad();
    load.user = this;
    return load;
  }
};

// Model::Load(model, entry, 0) natively, else its original; returns its r3
// (the model's +412 natively).
uint32_t LoadModel(uint8_t* base, ParseCalls& parse, uint32_t model, uint32_t entry,
                   uint32_t lr) {
  if (model != 0 && models::ModelLoad(base, model, entry, 0, parse.ModelLoad())) {
    return Load(base, model + 412);
  }
  return parse.Call(lr, 0x82596920, {model, entry, 0});
}

// XPR::Create natively, else its original; returns its r3.
uint32_t CreateXpr(uint8_t* base, ParseCalls& parse, uint32_t xpr, uint32_t file, uint32_t lr) {
  if (const auto r3 = models::XprCreate(base, xpr, file)) {
    return *r3;
  }
  return parse.Call(lr, 0x82596F68, {xpr, file});
}

}  // namespace

uint32_t CourseSetParse(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F1A70 CourseSet::Parse");
  ParseCalls parse{base, &calls, stack};
  const uint32_t frame = stack - 128;
  Store(base, frame, stack);
  const uint32_t first = Load(base, set + 3808);
  uint32_t entry = game_data::GetEntry(base, first, 0, true);
  if (entry != 0) {
    const uint32_t xpr = set + 64;
    if (xpr != 0) {
      models::XprConstruct(base, xpr);
    }
    CreateXpr(base, parse, xpr, entry, 0x820F1AB8);
  }
  entry = game_data::GetEntry(base, first, 1, true);
  if (entry != 0) {
    const uint32_t model = set + 96;
    if (model != 0 && !models::ModelConstruct(base, model)) {
      parse.Call(0x820F1AE0, 0x825948C0, {model});
    }
    LoadModel(base, parse, model, game_data::GetEntry(base, entry, 0, false), 0x820F1B00);
  }
  const scenery_lists::LoadCalls list_calls = parse.ListLoad();
  constexpr uint32_t kLists[] = {528, 1064, 1596, 2124, 2652};
  for (uint32_t i = 0; i < 5; i++) {
    entry = game_data::GetEntry(base, first, 2 + i, true);
    if (entry != 0) {
      const uint32_t list_frame = frame - 160;
      Store(base, list_frame, frame);
      scenery_lists::ModelListLoad(base, set + kLists[i], entry, list_frame, list_calls);
    }
  }
  entry = game_data::GetEntry(base, first, 7, true);
  if (entry != 0) {
    const uint32_t points_frame = frame - 192;
    Store(base, points_frame, frame);
    scenery_lists::ModelListLoadPoints(base, set + 3180, entry, points_frame, list_calls);
  }
  const uint32_t second = Load(base, set + 3812);
  constexpr uint32_t kSecond[] = {1056, 1060, 1592};
  for (uint32_t i = 0; i < 3; i++) {
    entry = game_data::GetEntry(base, second, i, true);
    if (entry != 0) {
      Store(base, set + kSecond[i], entry);
    }
  }
  return entry;
}

uint32_t AreaLightsParse(uint8_t* base, uint32_t lights) {
  ITC_NATIVE_RUNNING("sub_820F11C0 AreaLights::Parse");
  const uint32_t entry = game_data::GetEntry(base, Load(base, lights + 4), 0, true);
  if (entry != 0) {
    Store(base, lights + 80, entry);
  }
  return entry;
}

uint32_t AreaModelParse(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820F1258 AreaModel::Parse");
  ParseCalls parse{base, &calls, stack};
  Store(base, stack - 144, stack);
  const uint32_t file = Load(base, Load(base, kManager) + 188);
  const uint32_t entry = game_data::GetEntry(base, file, 0, true);
  const uint32_t xpr_entry = game_data::GetEntry(base, file, 1, true);
  uint32_t r3 = xpr_entry;
  if (entry != 0) {
    r3 = LoadModel(base, parse, model + 80, entry, 0x820F12C0);
    Store(base, model + 56, model + 80);
  } else {
    Store(base, model + 56, 0);
  }
  if (xpr_entry != 0) {
    r3 = CreateXpr(base, parse, model + 528, xpr_entry, 0x820F12E0);
    Store(base, model + 60, model + 528);
  } else {
    Store(base, model + 60, 0);
  }
  return r3;
}

uint32_t AreaLightsOnLoaded(uint8_t* base, uint32_t lights) {
  ITC_NATIVE_RUNNING("sub_820F1C30 AreaLights::OnLoaded");
  AreaLightsParse(base, lights);
  MarkLoaded(base, lights);
  return lights;
}

uint32_t AreaModelOnLoaded(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820F1CE8 AreaModel::OnLoaded");
  const uint32_t frame = stack - 96;
  Store(base, frame, stack);
  AreaModelParse(base, calls, frame, model);
  MarkLoaded(base, model);
  return model;
}

// --- The pools' objects: made and reset ---

namespace {

// Model::Model natively, else its original.
void MakeModel(uint8_t* base, const ParseCalls& parse, uint32_t model, uint32_t lr) {
  if (!models::ModelConstruct(base, model)) {
    parse.Call(lr, 0x825948C0, {model});
  }
}

// Model::Release natively, else its original; its r3.
uint32_t ReleaseModel(uint8_t* base, const ParseCalls& parse, uint32_t model, uint32_t lr) {
  if (const auto r3 = models::ModelRelease(base, model)) {
    return *r3;
  }
  return parse.Call(lr, 0x82594970, {model});
}

// XPR::Release natively, else its original; its r3.
uint32_t ReleaseXpr(uint8_t* base, const ParseCalls& parse, uint32_t xpr, uint32_t lr) {
  if (const auto r3 = models::XprRelease(base, xpr)) {
    return *r3;
  }
  return parse.Call(lr, 0x825970A8, {xpr});
}

constexpr uint32_t kSceneryModelDestruct = 0x820F1208;

// ModelList::Release's calls: Model::Release, and the models' vtable slot 0
// with 1 (the scenery models' destructor natively).
struct ListRelease {
  uint8_t* base;
  const ParseCalls* parse;

  static uint32_t Release(void* self, uint32_t model) {
    auto& r = *static_cast<ListRelease*>(self);
    return ReleaseModel(r.base, *r.parse, model, 0x82148020);
  }
  static uint32_t Destroy(void* self, uint32_t model) {
    auto& r = *static_cast<ListRelease*>(self);
    const uint32_t slot = Load(r.base, Load(r.base, model));
    if (slot == kSceneryModelDestruct) {
      return SceneryModelDestruct(r.base, *r.parse->calls, r.parse->stack, model, 1);
    }
    return r.parse->Call(0x82148040, slot, {model, 1});
  }
};

}  // namespace

uint32_t MakePoolObject(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t pool,
                        uint32_t object, uint32_t own_size, uint32_t data_size, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820F2638 Scenery::MakePoolObject");
  Store(base, stack - 96, stack);
  if (pool >= 5 || object == 0) {
    return object;
  }
  switch (pool) {
    case 0:
    case 1:
      ConstructObject(base, object, own_size, data_size, index);
      Store(base, object + 0, pool == 0 ? 0x8200674Cu : 0x82006758u);
      Store(base, object + 56, 0);
      break;
    case 2:
      CourseSetConstruct(base, calls, stack - 96, object, own_size, data_size, index);
      break;
    case 3:
      ConstructObject(base, object, own_size, data_size, index);
      Store(base, object + 0, 0x8200676C);
      models::XprConstruct(base, object + 56);
      break;
    default:
      AreaModelConstruct(base, calls, stack - 96, object, own_size, data_size, index);
      break;
  }
  return object;
}

void CourseSetConstruct(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set,
                        uint32_t own_size, uint32_t data_size, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820F2478 CourseSet::CourseSet");
  const uint32_t frame = stack - 112;
  Store(base, frame, stack);
  ConstructObject(base, set, own_size, data_size, index);
  Store(base, set + 0, 0x82006784);
  CourseSetInitParts(base, calls, frame, set + 64);
  Store(base, set + 3808, 0);
  Store(base, set + 3812, 0);
  Store(base, set + 1056, 0);
  Store(base, set + 1060, 0);
  if (set + 528 != 0) ModelListInit(base, set + 528);
  Store(base, set + 1592, 0);
  for (uint32_t offset : {1064u, 1596u, 2124u, 2652u}) {
    if (set + offset != 0) ModelListInit(base, set + offset);
  }
  const uint32_t points = set + 3180;
  if (points != 0) {
    ModelListInit(base, points);
    for (uint32_t offset : {608u, 612u, 616u, 620u}) Store(base, points + offset, 0);
  }
}

void CourseSetInitParts(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t parts) {
  ITC_NATIVE_RUNNING("sub_820F2230 CourseSet::InitParts");
  Store(base, stack - 112, stack);
  ParseCalls parse{base, &calls, stack};
  models::XprConstruct(base, parts);
  MakeModel(base, parse, parts + 32, 0x820F2254);
  for (uint32_t offset : {464u, 1000u, 1532u, 2060u, 2588u, 3116u}) {
    ModelListInit(base, parts + offset);
  }
  for (uint32_t offset : {608u, 612u, 616u, 620u}) Store(base, parts + 3116 + offset, 0);
}

void AreaModelConstruct(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t model,
                        uint32_t own_size, uint32_t data_size, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820F1C68 AreaModel::AreaModel");
  Store(base, stack - 112, stack);
  ParseCalls parse{base, &calls, stack};
  ConstructObject(base, model, own_size, data_size, index);
  Store(base, model + 0, 0x82006778);
  MakeModel(base, parse, model + 80, 0x820F1C9C);
  Store(base, model + 80, scenery_lists::kSceneryModelVtable);
  models::XprConstruct(base, model + 528);
  Store(base, model + 56, 0);
  Store(base, model + 60, 0);
}

uint32_t CourseSetReset(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F1138 CourseSet::Reset");
  const uint32_t frame = stack - 96;
  Store(base, frame, stack);
  ParseCalls parse{base, &calls, stack};
  ReleaseXpr(base, parse, set + 64, 0x820F1154);
  ReleaseModel(base, parse, set + 96, 0x820F115C);
  ListRelease list_release{base, &parse};
  scenery_lists::ReleaseCalls release;
  release.release = &ListRelease::Release;
  release.destroy = &ListRelease::Destroy;
  release.user = &list_release;
  for (uint32_t offset : {528u, 1064u, 1596u, 2124u, 2652u, 3180u}) {
    // ModelList::Release's frame (128 bytes) below this one.
    Store(base, frame - 128, frame);
    scenery_lists::ModelListRelease(base, set + offset, release);
  }
  ResetObject(base, set);
  return set;
}

uint32_t AreaModelReset(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820F12F8 AreaModel::Reset");
  Store(base, stack - 112, stack);
  ParseCalls parse{base, &calls, stack};
  const uint32_t mesh = Load(base, model + 56);
  if (mesh != 0) {
    ReleaseModel(base, parse, mesh, 0x820F1324);
    Store(base, model + 56, 0);
  }
  const uint32_t xpr = Load(base, model + 60);
  if (xpr != 0) {
    ReleaseXpr(base, parse, xpr, 0x820F1338);
    Store(base, model + 60, 0);
  }
  ResetObject(base, model);
  return model;
}

uint32_t SceneryModelDestruct(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t model,
                              uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820F1208 AreaModel::~Model");
  Store(base, stack - 112, stack);
  ParseCalls parse{base, &calls, stack};
  if (!models::ModelDestruct(base, model)) {
    parse.Call(0x820F1228, 0x82594F58, {model});
  }
  if ((flags & 1) != 0 && !memory::crt::OperatorDelete(model)) {
    parse.Call(0x820F1238, 0x825A70E8, {model});
  }
  return model;
}

}  // namespace course_scenery
