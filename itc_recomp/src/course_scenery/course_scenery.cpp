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
#include "schema/course_scenery.h"
#include "schema/models.h"
#include "schema/scenery_lists.h"

#include <cstdlib>
#include <initializer_list>

namespace course_scenery {

namespace {

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}

void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

using Manager = schema::Scenery;
using Object = schema::PoolObject;
using Record = schema::ObjectPoolRecord;
using Pools = schema::ObjectPools;
using G = schema::CourseSceneryGlobals;
using Table = schema::SceneryCourseTable;
using List = schema::SceneryItemList;
using Item = schema::SceneryListItem;
using Entry = schema::SceneryListTableEntry;

uint32_t Round64(uint32_t size) {
  return (size + 63) & 0xFFFFFFC0u;
}


// The vtables' targets that are native.
constexpr uint32_t kPoolObjectKey = 0x820FB9F0;
constexpr uint32_t kAreaLightsKey = 0x820F1C20;
constexpr uint32_t kAreaModelKey = 0x820F1CD8;
constexpr uint32_t kPoolObjectReset = 0x820FB9D8;
constexpr uint32_t kCourseSetLoad = 0x820F2520;
constexpr uint32_t kAreaLightsLoad = 0x820F22F8;
constexpr uint32_t kAreaModelLoad = 0x820F23C0;
// The loads' callbacks.
constexpr uint32_t kCourseSetOnLoaded = 0x820F22B8;
constexpr uint32_t kAreaLightsOnLoaded = 0x820F1C30;
constexpr uint32_t kAreaModelOnLoaded = 0x820F1CE8;

// memcpy (sub_825A7410), for the copies c_runtime::Memcpy leaves to it.
constexpr uint32_t kMemcpy = 0x825A7410;

// PoolObject::PoolObject's stores (ObjectPools::Init does the same inline).
void Construct(uint32_t object, uint32_t own_size, uint32_t data_size,
               uint32_t index) {
  const uint32_t data = object + own_size;
  const Object o(object);
  o.set_vtable(Object::kVtable);
  o.set_data_size(data_size);
  o.set_data_used(0);
  o.set_data(data);
  o.set_unknown_28(0);
  o.set_index(index);
  o.set_next(data + data_size);
  o.set_flags(0);
  o.set_countdown(0xFFFFFFFFu);
  o.set_data_used(0);
}

// Calls `function` through `calls` from a frame of `frame_bytes` below
// `stack`, its back chain written as the original's stwu writes it.
uint32_t CallGuest(const Calls& calls, uint32_t stack, uint32_t frame_bytes,
                   uint32_t lr, uint32_t function, std::initializer_list<uint32_t> args) {
  const uint32_t frame = stack - frame_bytes;
  Store(frame, stack);
  return calls.call(calls.user, function, args.begin(), int(args.size()), frame, lr);
}

// An object's key through its vtable's slot 0, from a caller whose frame
// (112 bytes in both callers) is below `stack`; the guest's key slots write
// the size at the frame's +80, as the originals have them.
uint32_t KeyOf(const Calls& calls, uint32_t stack, uint32_t lr, uint32_t object,
               uint32_t* size) {
  const uint32_t slot = Object(object).vtable().key();
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
  Store(frame + 80, 0);
  const uint32_t key = CallGuest(calls, stack, 112, lr, slot, {object, frame + 80});
  *size = Load(frame + 80);
  return key;
}

// An object reset through its vtable's slot 2.
void ResetThroughVtable(const Calls& calls, uint32_t stack, uint32_t frame_bytes,
                        uint32_t lr, uint32_t object) {
  const uint32_t slot = Object(object).vtable().reset();
  if (slot == kPoolObjectReset) {
    ResetObject(object);
    return;
  }
  CallGuest(calls, stack, frame_bytes, lr, slot, {object});
}

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Course scenery: {} refused after its checks", what);
    std::abort();
  }
}

}  // namespace

// --- Pool objects ---

uint32_t AllocData(uint32_t data, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_820FB8B8 PoolObject::AllocData");
  // `data` is the object's +4 (its data, size and used).
  const Object o(data - 4);
  const uint32_t used = o.data_used();
  const uint32_t at = used + o.data();
  o.set_data_used(Round64(size) + used);
  return at;
}

uint32_t InUse(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB8E0 PoolObject::InUse");
  return Object(object).flags() & 1;
}

uint32_t IsLoaded(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB8F0 PoolObject::IsLoaded");
  return (Object(object).flags() & 0x9) == 0x9 ? 1 : 0;
}

uint32_t IsVisible(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB908 PoolObject::IsVisible");
  return (Object(object).flags() & 0x19) == 0x19 ? 1 : 0;
}

uint32_t IsExpiring(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB920 PoolObject::IsExpiring");
  return (Object(object).flags() & 0x29) == 0x29 ? 1 : 0;
}

void SetVisible(uint32_t object, uint32_t on) {
  ITC_NATIVE_RUNNING("sub_820FB938 PoolObject::SetVisible");
  uint32_t flags = Object(object).flags();
  if (on != 0) {
    if ((flags & 0x9) != 0x9) {
      return;
    }
    flags = (flags & ~0x30u) | 0x10;
  } else {
    flags &= ~0x10u;
  }
  Object(object).set_flags(flags);
}

void SetExpiring(uint32_t object, uint32_t on) {
  ITC_NATIVE_RUNNING("sub_820FB968 PoolObject::SetExpiring");
  uint32_t flags = Object(object).flags();
  if (on != 0) {
    if ((flags & 0x9) != 0x9) {
      return;
    }
    flags = (flags & ~0x30u) | 0x20;
  } else {
    flags &= ~0x20u;
  }
  Object(object).set_flags(flags);
}

void ClearLoadFlags(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB998 PoolObject::ClearLoadFlags");
  const Object o(object);
  o.set_flags(o.flags() & ~0x3Cu);
}

void MarkLoaded(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB9A8 PoolObject::MarkLoaded");
  const Object o(object);
  o.set_flags((o.flags() & ~0x2u) | 0x8);
}

void ResetObject(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820FB9D8 PoolObject::Reset");
  const Object o(object);
  o.set_flags(0);
  o.set_countdown(0xFFFFFFFFu);
  o.set_data_used(0);
}

uint32_t PoolObjectKey(uint32_t, uint32_t* size) {
  ITC_NATIVE_RUNNING("sub_820FB9F0 PoolObject::Key");
  *size = 0;
  return 0;
}

uint32_t AreaLightsKey(uint32_t object, uint32_t* size) {
  ITC_NATIVE_RUNNING("sub_820F1C20 AreaLights::Key");
  *size = 4;
  return schema::AreaLights(nullptr, object).key_address();
}

uint32_t AreaModelKey(uint32_t object, uint32_t* size) {
  ITC_NATIVE_RUNNING("sub_820F1CD8 AreaModel::Key");
  *size = 4;
  return schema::AreaModel(nullptr, object).key_address();
}

void ConstructObject(uint32_t object, uint32_t own_size, uint32_t data_size,
                     uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820FBB20 PoolObject::PoolObject");
  Construct(object, own_size, data_size, index);
}

uint32_t SetKey(const Calls& calls, uint32_t stack, uint32_t object,
                uint32_t key) {
  ITC_NATIVE_RUNNING("sub_820FBA00 PoolObject::SetKey");
  uint32_t size = 0;
  const uint32_t buffer = KeyOf(calls, stack, 0x820FBA30, object, &size);
  if (size == 0 || buffer == 0) {
    return buffer;
  }
  if (!c_runtime::Memcpy(buffer, key, size)) {
    return CallGuest(calls, stack, 112, 0x820FBA4C, kMemcpy, {buffer, key, size});
  }
  return buffer;
}

uint32_t KeyEquals(const Calls& calls, uint32_t stack, uint32_t object,
                   uint32_t key) {
  ITC_NATIVE_RUNNING("sub_820FBA60 PoolObject::KeyEquals");
  uint32_t size = 0;
  const uint32_t buffer = KeyOf(calls, stack, 0x820FBA94, object, &size);
  if (size == 0 || buffer == 0) {
    return 1;
  }
  for (uint32_t i = 0; i != size; i++) {
    if (guest::LoadU8(buffer + i) != guest::LoadU8(key + i)) {
      return 0;
    }
  }
  return 1;
}

// --- The pools ---

uint32_t PoolRecord(uint32_t set, uint32_t pool) {
  ITC_NATIVE_RUNNING("sub_820FBB10 ObjectPools::Pool");
  return pool * 24 + Pools(set).records().address;
}

namespace {

uint32_t ObjectAt(uint32_t record, uint32_t index) {
  const Record r(record);
  const uint32_t stride = r.data_size() + r.object_size();
  return stride * index + r.first_object().address;
}

}  // namespace

uint32_t PoolObjectAt(uint32_t set, uint32_t pool, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820FC0E8 ObjectPools::Object");
  return ObjectAt(pool * 24 + Pools(set).records().address, index);
}

uint32_t Acquire(const Calls& calls, uint32_t stack, uint32_t set, uint32_t pool,
                 uint32_t key) {
  ITC_NATIVE_RUNNING("sub_820FBF08 ObjectPools::Acquire");
  const Record r(pool * 24 + Pools(set).records().address);
  const Object o = r.free_head();
  const uint32_t object = o.address;
  if (object == 0) {
    return 0xFFFFFFFFu;
  }
  o.set_countdown(0xFFFFFFFFu);
  o.set_flags(o.flags() | 1);
  SetKey(calls, stack - 112, object, key);
  // Off the free list.
  const Object next = r.free_head().next();
  r.set_free_head(next);
  if (next) {
    next.set_prev(0);
  }
  // Onto the used list's head.
  const uint32_t used = r.used_head().address;
  if (used == 0) {
    o.set_next(0);
    o.set_prev(0);
  } else {
    o.set_next(used);
    o.set_prev(r.used_head().prev());
    r.used_head().set_prev(object);
  }
  r.set_used_head(object);
  return o.index();
}

uint32_t Find(const Calls& calls, uint32_t stack, uint32_t set, uint32_t pool,
              uint32_t key) {
  ITC_NATIVE_RUNNING("sub_820FBE80 ObjectPools::Find");
  const Record r(pool * 24 + Pools(set).records().address);
  for (Object o = r.used_head(); o; o = o.next()) {
    if ((o.flags() & 1) != 0 && KeyEquals(calls, stack - 112, o.address, key) != 0) {
      return o.index();
    }
  }
  return 0xFFFFFFFFu;
}

uint32_t LoadObject(const Calls& calls, uint32_t stack, uint32_t set,
                    uint32_t pool, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820FBFD0 ObjectPools::Load");
  const uint32_t object = ObjectAt(pool * 24 + Pools(set).records().address, index);
  if (object == 0) {
    return object;
  }
  const uint32_t slot = Object(object).vtable().load();
  if (calls.load != nullptr) {
    switch (slot) {
      case kCourseSetLoad:
        CourseSetLoad(object, *calls.load);
        return object;
      case kAreaLightsLoad:
        AreaLightsLoad(object, *calls.load);
        return object;
      case kAreaModelLoad:
        AreaModelLoad(object, *calls.load);
        return object;
      default:
        break;
    }
  }
  CallGuest(calls, stack, 96, 0x820FC01C, slot, {object});
  return object;
}

uint32_t Release(const Calls& calls, uint32_t stack, uint32_t set, uint32_t pool,
                 uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820FC038 ObjectPools::Release");
  const Record r(pool * 24 + Pools(set).records().address);
  const uint32_t object = ObjectAt(r.address, index);
  const Object o(object);
  const uint32_t following = o.next().address;
  ResetThroughVtable(calls, stack, 112, 0x820FC080, object);
  // Off the used list.
  const Object previous = o.prev();
  if (previous) {
    previous.set_next(o.next());
  } else {
    r.set_used_head(o.next());
  }
  const Object next = o.next();
  if (next) {
    next.set_prev(o.prev());
  }
  // Onto the free list's head.
  const uint32_t free_head = r.free_head().address;
  o.set_prev(0);
  o.set_next(free_head);
  const Object head = r.free_head();
  if (head) {
    head.set_prev(object);
  }
  r.set_free_head(object);
  return following;
}

bool Init(const Calls& calls, uint32_t stack, uint32_t set, uint32_t count,
          uint32_t descriptors, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820FBBE8 ObjectPools::Init");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  const int32_t pools = int32_t(count);
  // The original puts the records (rounded up to 64) at the block's start;
  // since E5 step 5 they're a host array of their own and the block holds
  // only the objects and their data (pool 0's first object is its start,
  // which Shutdown frees).
  uint32_t size = 0;
  for (int32_t i = 0; i < pools; i++) {
    const schema::ObjectPoolDesc d(descriptors + 24 * uint32_t(i));
    size += Round64(d.object_size()) * d.object_count() + Round64(d.data_size()) * d.data_count();
  }
  // The original's XPhysicalAlloc(size, -1, 64, 4); the GPU reads nothing
  // in it (the models' vertex data and the XPRs' textures are copied into
  // their own physical blocks), so a blob since E5 step 7 (page-aligned, past
  // the 64 the objects' cursor relies on), freed by Shutdown as before.
  const uint32_t block = schema::ObjectPoolsBlock::New(size).address;
  const uint32_t records = Record::NewArray(count).address;
  *r3 = records;
  const Pools ps(set);
  ps.set_count(count);
  ps.set_records(records);
  uint32_t cursor = block;
  for (int32_t i = 0; i < pools; i++) {
    const schema::ObjectPoolDesc d(descriptors + 24 * uint32_t(i));
    const Record record(24 * uint32_t(i) + ps.records().address);
    const uint32_t object_size = Round64(d.object_size());
    const uint32_t data_size = Round64(d.data_size());
    const uint32_t objects = d.object_count();
    record.set_first_object(cursor);
    record.set_object_count(objects);
    record.set_object_size(object_size);
    record.set_data_size(data_size);
    if (int32_t(objects) > 0) {
      uint32_t at = cursor;
      uint32_t previous = 0;
      int32_t k = 0;
      do {
        const uint32_t factory = d.factory();
        if (factory != 0) {
          at = CallGuest(calls, stack, 208, 0x820FBD10, factory,
                         {uint32_t(i), at, object_size, data_size, uint32_t(k)});
          *r3 = at;
        } else if (at != 0) {
          Construct(at, object_size, data_size, uint32_t(k));
        } else {
          at = 0;
        }
        Object(at).set_prev(previous);
        previous = at;
        k++;
        at = Object(at).next().address;
      } while (k < int32_t(record.object_count()));
      if (previous != 0) {
        Object(previous).set_next(0);
      }
    }
    record.set_used_head(0);
    record.set_free_head(record.first_object());
    cursor += d.data_count() * data_size + d.object_count() * object_size;
  }
  return true;
}

bool Shutdown(const Calls& calls, uint32_t stack, uint32_t set, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820FBDC8 ObjectPools::Shutdown");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  const Pools ps(set);
  for (int32_t p = 0; p < int32_t(ps.count()); p++) {
    const uint32_t records = ps.records().address;
    const uint32_t offset = 24 * uint32_t(p);
    for (int32_t k = 0; k < int32_t(Record(records + offset).object_count()); k++) {
      const uint32_t object = ObjectAt(offset + ps.records().address, uint32_t(k));
      if (object != 0) {
        ResetThroughVtable(calls, stack, 144, 0x820FBE38, object);
      }
    }
  }
  const uint32_t records = ps.records().address;
  *r3 = records;
  if (records != 0) {
    if (memory::IsHandle(records)) {
      // Init's host records (E5 step 5): the objects' block apart, from
      // pool 0's first object (the block's start).
      const uint32_t block = ps.count() != 0 ? Record(records).first_object().address : 0;
      if (block != 0) {
        memory::FreePhysical(block);
      }
      Record(records).Delete();
    } else {
      memory::FreePhysical(records);  // the original's one block
    }
    ps.set_count(0);
    ps.set_records(0);
  }
  return true;
}

// --- The scenery's lists ---

uint32_t ModelAsset(uint32_t config, uint32_t area, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820F1530 SceneryConfig::ModelAsset");
  const Table table = schema::SceneryConfig(config).course_table();
  if (area >= table.area_count()) {
    return 0xFFFFFFFFu;
  }
  const Table entry(area * 12 + table.address);  // area `area`'s words
  const uint32_t first = entry.first_model_asset();
  const uint32_t last = entry.last_model_asset();
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

uint64_t PoolSizes(uint32_t area_counts, uint32_t model_counts, uint32_t out[5],
                   uint32_t config) {
  ITC_NATIVE_RUNNING("sub_820F2030 Scenery::PoolSizes");
  uint64_t r3 = 0;
  const schema::SceneryConfig c(config);
  // Summed in mode 0, else the largest (signed).
  auto add = [&](int32_t& total, uint32_t size) {
    if (int32_t(c.mode()) == 0) {
      total = int32_t(uint32_t(total) + size);
    } else if (total < int32_t(size)) {
      total = int32_t(size);
    }
  };
  for (uint32_t pool = 0; pool < 5; pool++) {
    int32_t total = 0;
    if (pool == 0) {
      const uint32_t size = archive::AssetSize(c.pool0_asset().value());
      r3 = size;
      total = int32_t(size);
    } else if (pool == 2) {
      const uint32_t asset = c.course_table().set_second_asset();
      r3 = asset;
      if (int32_t(asset) >= 0) {
        const uint32_t size = archive::AssetSize(asset);
        r3 = size;
        total = int32_t(uint32_t(total) + size);
      }
    } else if (pool == 3) {
      uint32_t area = 0;
      for (;; area++) {
        const Table table = c.course_table();
        if (area < table.area_count()) {
          r3 = Table(area * 12 + table.address).light_asset();
        } else {
          r3 = ~0ull;  // li r3,-1
        }
        if (int32_t(uint32_t(r3)) < 0) {
          break;
        }
        const uint32_t size = archive::AssetSize(uint32_t(r3));
        r3 = size;
        add(total, size);
      }
      Store(area_counts, area);
      Store(area_counts + 4, area);
    } else if (pool == 4) {
      uint32_t models = 0;
      for (uint32_t area = 0;; area++) {
        const Table table = c.course_table();
        const uint32_t count = table.area_count();
        const Table entry(area * 12 + table.address);
        if (area >= count || int32_t(entry.light_asset()) < 0) {
          break;
        }
        const int32_t n = int32_t(entry.last_model_asset() - entry.first_model_asset() + 1);
        for (int32_t model = 0; model < n; model++) {
          const uint32_t asset = ModelAsset(config, area, uint32_t(model));
          r3 = asset == 0xFFFFFFFFu ? ~0ull : asset;
          if (int32_t(asset) < 0) {
            continue;
          }
          const uint32_t size = archive::AssetSize(asset);
          r3 = size;
          add(total, size);
          models++;
        }
      }
      Store(model_counts, models);
      Store(model_counts + 4, 16);
      Store(model_counts + 4, models < 16 ? models : 16);
    }
    out[pool] = uint32_t(total);
  }
  return r3;
}

void ListClear(uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820F1588 SceneryList::Clear");
  const List l(list);
  l.set_done(0);
  l.set_count(0);
}

uint32_t ListAdd(uint32_t list, double distance, uint32_t area, uint32_t model,
                 uint32_t mask, uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_820F1888 SceneryList::Add");
  const List l(list);
  const uint32_t count = l.count();
  if (count != 0) {
    const uint32_t pointers = l.pointers();
    for (uint32_t k = count; k != 0; k--) {
      const Item item(Load(pointers + 4 * k - 4));
      r3 = item.area();
      if (area != r3) {
        continue;
      }
      r3 = item.model();
      if (model != r3) {
        continue;
      }
      // Found: the smaller distance (as the recompiled fcmpu and ble have
      // it) and the mask ORed in.
      double kept = item.distance();
      if (kept > distance) {
        kept = distance;
      }
      const uint32_t old_mask = item.mask();
      item.set_distance(float(kept));
      item.set_mask(old_mask | mask);
      return r3;
    }
  }
  const Item item((count << 4) + l.items().address);
  Store(l.pointers() + 4 * count, item.address);
  item.set_distance(float(distance));
  item.set_mask(mask);
  item.set_area(uint16_t(area));
  item.set_model(uint16_t(model));
  l.set_count(l.count() + 1);
  return r3;
}

namespace {

float Distance(uint32_t list, int32_t index) {
  return Item(Load(List(list).pointers() + 4 * uint32_t(index))).distance();
}

void Sort(uint32_t list, int32_t first, int32_t last) {
  for (;;) {
    const float pivot = Distance(list, (first + last) / 2);
    int32_t i = first;
    int32_t j = last;
    for (;;) {
      while (Distance(list, i) < pivot) {
        i++;
      }
      while (Distance(list, j) > pivot) {
        j--;
      }
      if (i >= j) {
        break;
      }
      const uint32_t pointers = List(list).pointers();
      const uint32_t at_j = Load(pointers + 4 * uint32_t(j));
      const uint32_t at_i = Load(pointers + 4 * uint32_t(i));
      Store(pointers + 4 * uint32_t(i), at_j);
      Store(List(list).pointers() + 4 * uint32_t(j), at_i);
      i++;
      j--;
    }
    if (first < i - 1) {
      Sort(list, first, i - 1);
    }
    if (last <= j + 1) {
      return;
    }
    first = j + 1;
  }
}

}  // namespace

void ListSort(uint32_t list, int32_t first, int32_t last) {
  ITC_NATIVE_RUNNING("sub_820F1930 SceneryList::Sort");
  Sort(list, first, last);
}

void LimitLargeModels(uint32_t manager, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820F2B80 Scenery::LimitLargeModels");
  const Manager m(manager);
  const List list(m.list_b_pool_address());
  const uint32_t table = stack - 80;
  const uint32_t limit = int32_t(m.view_count_minus_1()) == 1 ? 10 : 5;
  const uint32_t count = list.count();
  Store(table + 0, 24);
  Store(table + 4, 11);
  Store(table + 8, 18);
  Store(table + 12, 9);
  Store(table + 16, 54);
  const uint32_t cap = m.model_cap();
  if (count <= cap || cap == 0) {
    return;
  }
  // Whether the item is large: its model at or past its area's threshold.
  auto large = [&](uint32_t address) {
    const Item item(address);
    const uint32_t threshold = Load(table + 4 * item.area());
    return threshold <= item.model();
  };
  uint32_t seen = 0;
  for (uint32_t i = 0; i < m.model_cap(); i++) {
    const uint32_t pointers = list.pointers();
    const uint32_t item = Load(pointers + 4 * i);
    if (!large(item)) {
      continue;
    }
    if (limit >= seen) {
      seen++;
      continue;
    }
    for (uint32_t j = i + 1; j < count - 1; j++) {
      const uint32_t other = Load(pointers + 4 * j);
      if (!large(other)) {
        const uint32_t at_i = Load(list.pointers() + 4 * i);
        Store(list.pointers() + 4 * j, at_i);
        Store(list.pointers() + 4 * i, other);
        break;
      }
    }
  }
}

void ModelListInit(uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820F1A20 ModelList::Init");
  const schema::ModelList l(list);
  const uint32_t nodes = l.nodes_address();
  for (uint32_t i = 0; i < 64; i++) {
    guest::StoreU16(nodes + 4 * i + 2, uint16_t(i));
    guest::StoreU16(nodes + 4 * i + 0, uint16_t(i + 1));
  }
  l.set_free_head(0);
  l.set_list_count(0);
  l.set_node63_next(0xFFFF);
  l.set_head(0xFFFF);
  l.set_list_262(0xFFFF);
  l.set_list_258(0xFFFF);
  l.set_count(0);
}

void ClearGlobal() {
  ITC_NATIVE_RUNNING("sub_820F1480 Scenery::ClearGlobal");
  G::set_manager(0u);
}

uint32_t AllocPhysical(uint32_t size) {
  ITC_NATIVE_RUNNING("sub_820F1490 Scenery::AllocPhysical");
  // The original's XPhysicalAlloc(size, -1, 0, 4); the GPU reads none of
  // its blocks (the manager, the lists, the visible areas, pool 0's block,
  // the course set's first file, whose textures and vertex data are copied
  // out), so a blob since E5 step 7. XPhysicalSize answers its whole 4 KB
  // pages, as the physical block's.
  const uint32_t block = schema::SceneryBlock::New(size).address;
  const uint32_t held = memory::PhysicalSize(block);
  G::set_physical_held(held + G::physical_held());
  return block;
}

void FreePhysical(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820F14E8 Scenery::FreePhysical");
  const uint32_t held = memory::PhysicalSize(block);
  G::set_physical_held(G::physical_held() - held);
  memory::FreePhysical(block);
}

bool UnloadPools(const Calls& calls, uint32_t stack, uint32_t manager,
                 uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820F1D20 Scenery::UnloadPools");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  // The original's 112-byte frame holds only its back chain: Shutdown's
  // guest calls make their frames below `stack` (E5 step 6).
  uint32_t shutdown_r3 = 0;
  Require(Shutdown(calls, stack, manager, &shutdown_r3), "Scenery::UnloadPools' Shutdown");
  const Manager m(manager);
  if (m.load_buffer() != 0) {
    m.set_load_buffer(0);
  }
  const uint32_t pool0 = m.pool0_block();
  m.set_pool0_data_size(0);
  if (pool0 != 0) {
    FreePhysical(pool0);
    m.set_pool0_block(0);
  }
  if (const uint32_t block = m.list_b_table().address; block != 0) {
    FreePhysical(block);
  }
  if (const uint32_t block = m.list_b_items().address; block != 0) {
    FreePhysical(block);
  }
  if (const uint32_t block = m.list_b_pointers(); block != 0) {
    FreePhysical(block);
  }
  c_runtime::XMemSet(m.list_b_pool_address(), 0, List::kSize);
  if (const uint32_t block = m.list_a_table().address; block != 0) {
    FreePhysical(block);
  }
  if (const uint32_t block = m.list_a_items().address; block != 0) {
    FreePhysical(block);
  }
  if (const uint32_t block = m.list_a_pointers(); block != 0) {
    FreePhysical(block);
  }
  c_runtime::XMemSet(m.list_a_pool_address(), 0, List::kSize);
  *r3 = m.list_a_pool_address();
  return true;
}

bool Unload(const Calls& calls, uint32_t stack, uint32_t manager, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820F2B38 Scenery::Unload");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  // The original's 96-byte frame held only its back chain (E5 step 6).
  const Manager m(manager);
  if (const uint32_t areas = m.visible_areas().address; areas != 0) {
    FreePhysical(areas);
    m.set_visible_areas(0);
  }
  return UnloadPools(calls, stack, manager, r3);
}

uint32_t Destroy() {
  ITC_NATIVE_RUNNING("sub_820F2AF8 Scenery::Destroy");
  ClearGlobal();
  FreePhysical(G::created().address);
  G::set_created(0u);
  return 0;
}

uint32_t Create() {
  ITC_NATIVE_RUNNING("sub_820F2F88 Scenery::Create");
  const uint32_t manager = AllocPhysical(248);
  G::set_created(manager);
  const Manager m(manager);
  // lfs and stfs: the constants' bits (none is a NaN).
  Store(m.unknown_200_address(), G::create_float_200());
  m.set_mask(0x38435);
  Store(m.unknown_204_address(), G::create_float_204());
  Store(m.unknown_208_address(), G::create_float_208());
  Store(m.unknown_212_address(), G::create_float_212());
  m.set_visible_areas(0);
  if (m.texture_provider_address() != 0) {
    Require(models::TextureProviderConstruct(m.texture_provider_address(), -1).has_value(),
            "Scenery::Create's TextureProvider");
  }
  Init(manager);
  return G::manager().list_b_pool_address();
}

uint32_t Setup(const Calls& calls, uint32_t stack, uint32_t manager,
               uint32_t config) {
  ITC_NATIVE_RUNNING("sub_820F3020 Scenery::Setup");
  // The original's 112-byte frame held only its back chain (E5 step 6).
  SetupPools(calls, stack, manager, config);
  const Manager m(manager);
  const uint32_t areas = m.area_count();
  const uint32_t models = m.model_count();
  m.set_visible_count(0);
  m.set_visible_capacity(areas);
  const uint32_t block = AllocPhysical(areas * schema::SceneryVisibleArea::kSize);
  m.set_visible_areas(block);
  for (uint32_t i = 0; i < m.visible_capacity(); i++) {
    const schema::SceneryVisibleArea entry(m.visible_areas().address + schema::SceneryVisibleArea::kSize * i);
    entry.set_model_count(0);
    entry.set_model_capacity(models);
  }
  return block;
}

uint32_t SetupPools(const Calls& calls, uint32_t stack, uint32_t manager,
                    uint32_t config) {
  ITC_NATIVE_RUNNING("sub_820F2720 Scenery::SetupPools");
  const uint32_t frame = stack - 336;
  Store(frame, stack);
  const uint32_t copy = manager + 8;  // the configuration's copy
  const Manager m(manager);
  G::set_physical_held(0);
  m.set_pool0_data_size(0);
  m.set_pool0_block(0);
  if (!native::Buffers_Memcpy(copy, config, 44, 0x820F2758)) {
    CallGuest(calls, frame, 0, 0x820F2758, kMemcpy, {copy, config, 44});
  }
  if (int32_t(m.mode()) == 2 && m.view_count_minus_1() == 0) {
    m.set_mode(m.mode2_source().value());
  }
  // PoolSizes' five words, the original's frame +80 to +99, which only this
  // function reads: host words (E5 step 6).
  uint32_t sizes[5] = {};
  PoolSizes(m.area_count_address(), m.model_count_address(), sizes, copy);
  // List A: the areas (pool 3).
  const uint32_t area_count = m.area_count();
  m.set_list_a_done(0);
  m.set_list_a_count(0);
  m.set_list_a_capacity(area_count);
  m.set_list_a_pool(3);
  m.set_list_a_pointers(AllocPhysical(area_count * 4));
  m.set_list_a_items(AllocPhysical(m.list_a_capacity() * Item::kSize));
  m.set_list_a_table(AllocPhysical(m.list_a_capacity() * Entry::kSize));
  c_runtime::XMemSet(m.list_a_items().address, 0, m.list_a_capacity() * Item::kSize);
  c_runtime::XMemSet(m.list_a_table().address, 0, m.list_a_capacity() * Entry::kSize);
  // List B: the models (pool 4).
  const uint32_t model_count = m.model_count();
  m.set_list_b_done(0);
  m.set_list_b_count(0);
  m.set_list_b_capacity(model_count);
  m.set_list_b_pool(4);
  m.set_list_b_pointers(AllocPhysical(model_count * 4));
  m.set_list_b_items(AllocPhysical(m.list_b_capacity() * Item::kSize));
  m.set_list_b_table(AllocPhysical(m.list_b_capacity() * Entry::kSize));
  c_runtime::XMemSet(m.list_b_items().address, 0, m.list_b_capacity() * Item::kSize);
  c_runtime::XMemSet(m.list_b_table().address, 0, m.list_b_capacity() * Entry::kSize);
  // The tables, from the configuration's areas (its +28: +16 the count,
  // 12-byte entries from +20: the light asset, the first and last model).
  uint32_t written_a = 0, written_b = 0;
  for (uint32_t area = 0; written_a < m.list_a_capacity(); area++) {
    Table table = m.course_table();
    const int32_t light = area < table.area_count()
                              ? int32_t(Table(table.address + 12 * area).light_asset())
                              : -1;
    if (light < 0) {
      continue;
    }
    const Entry a(m.list_a_table().address + Entry::kSize * written_a);
    written_a++;
    const uint32_t key = area & 0xFFFF;
    a.set_model(0);
    a.set_asset(uint32_t(light));
    a.set_area(uint16_t(key));
    table = m.course_table();
    int32_t models = -1;
    if (key < table.area_count()) {
      const Table e(table.address + 12 * key);
      models = int32_t(e.last_model_asset() - e.first_model_asset() + 1);
    }
    for (int32_t model = 0; model < models; model++) {
      const uint32_t asset = ModelAsset(copy, area, uint32_t(model));
      if (int32_t(asset) < 0) {
        continue;
      }
      const Entry b(m.list_b_table().address + Entry::kSize * written_b);
      written_b++;
      b.set_area(uint16_t(key));
      b.set_model(uint16_t(model));
      b.set_asset(asset);
    }
  }
  // The five pools' descriptors: +0 the pool, +4 the object size, +8 the
  // data size, +12 and +16 the counts, +20 the factory (MakePoolObject).
  constexpr uint32_t kMakePoolObject = 0x820F2638;
  const uint32_t descriptors = frame + 112;
  const uint32_t first_size = sizes[0];
  uint32_t object_size = 0, count = 0, data_count = 0;
  for (uint32_t pool = 0; pool < 5; pool++) {
    const uint32_t mode = m.mode();
    switch (pool) {
      case 0:
        if (m.pool0_on() != 0) {
          m.set_pool0_data_size(first_size);
          object_size = 60;
          count = 1;
          data_count = 0;
          m.set_pool0_block(AllocPhysical(first_size));
          sizes[0] = 0;
        }
        break;
      case 1:
        if (m.pool1_on() != 0) {
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
        count = mode == 0 ? m.area_count() : m.area_count_streamed();
        data_count = mode == 0 ? 1 : count;
        break;
      case 4: {
        object_size = 560;
        count = mode == 0 ? m.model_count() : m.model_cap();
        data_count = mode == 0 ? 1 : count;
        const uint32_t limit = G::load_buffer_size();
        const uint32_t extra = limit < sizes[4] ? 0 : G::load_buffer();
        sizes[4] = 0;
        m.set_load_buffer(extra);
        break;
      }
    }
    const schema::ObjectPoolDesc d(descriptors + schema::ObjectPoolDesc::kSize * pool);
    d.set_pool(pool);
    d.set_object_size(object_size);
    d.set_object_count(count);
    d.set_data_count(data_count);
    d.set_data_size(sizes[pool]);
    d.set_factory(kMakePoolObject);
  }
  uint32_t r3 = 0;
  Require(Init(calls, frame, manager, 5, descriptors, &r3), "Scenery::SetupPools' Init");
  m.set_loading_all(0);
  return r3;
}

void Init(uint32_t manager) {
  ITC_NATIVE_RUNNING("sub_820F1360 Scenery::Init");
  G::set_manager(manager);
  Manager(manager).set_unknown_52(0);
  G::manager().set_unknown_56(0);
  for (uint32_t at = 60; at < 108; at += 8) {
    c_runtime::XMemSet(G::manager().address + at, 0, 8);
  }
  G::manager().set_pool0_object(0u);
  G::manager().set_course_set(0u);
  G::manager().set_pool0_block(0);
  G::manager().set_load_buffer(0);
  c_runtime::XMemSet(G::manager().list_a_pool_address(), 0, List::kSize);
  c_runtime::XMemSet(G::manager().list_b_pool_address(), 0, List::kSize);
  G::manager().set_last_pool(0xFFFFFFFFu);
  G::manager().set_last_index(0xFFFFFFFFu);
  G::manager().set_loading_all(0);
}

uint32_t ReleaseUnused(const Calls& calls, uint32_t stack, uint32_t manager,
                       uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820F17C0 SceneryList::ReleaseUnused");
  const uint32_t frame = stack - 112;
  const uint32_t pool = List(list).pool();
  uint32_t r3 = PoolRecord(manager, pool);
  uint32_t object = Record(r3).used_head().address;
  while (object != 0) {
    r3 = IsLoaded(object);
    if (r3 != 0) {
      r3 = IsVisible(object);
      if (r3 == 0) {
        // Loaded and not seen this update.
        r3 = IsExpiring(object);
        if (r3 == 0) {
          SetExpiring(object, 1);
          r3 = object;
          Object(object).set_countdown(2);
        } else {
          const int32_t countdown = int32_t(Object(object).countdown());
          if (countdown <= 0) {
            r3 = Release(calls, frame, manager, pool, Object(object).index());
            object = r3;
            continue;
          }
          Object(object).set_countdown(uint32_t(countdown - 1));
        }
        object = Object(object).next().address;
        continue;
      }
    }
    // Not loaded, or seen: the seen flag cleared.
    SetVisible(object, 0);
    r3 = object;
    object = Object(object).next().address;
  }
  return r3;
}

namespace {

constexpr uint32_t kQueueLength = 0x8209D7B0;  // Archive::QueueLength

uint32_t QueueLength(const Calls& calls, uint32_t stack, uint32_t lr,
                     bool native_queue) {
  return native_queue ? archive::QueueLength()
                      : CallGuest(calls, stack, 160, lr, kQueueLength, {});
}

}  // namespace

uint32_t ListLoadAll(const Calls& calls, uint32_t stack, uint32_t manager,
                     uint32_t list, bool native_queue) {
  ITC_NATIVE_RUNNING("sub_820F1598 SceneryList::LoadAll");
  const uint32_t frame = stack - 160;
  const List l(list);
  const Manager m(manager);
  const uint32_t pool = l.pool();
  const uint32_t queued = QueueLength(calls, stack, 0x820F15B4, native_queue);
  for (uint32_t i = l.done(); i < l.count(); i++) {
    const Item item(Load(l.pointers() + 4 * i));
    item.set_object(0u);
    if (l.capacity() == 0) {
      continue;
    }
    const uint32_t key = item.key();
    Entry entry = l.table();
    bool found = false;
    for (uint32_t n = 0;;) {
      if (entry.key() == key) {
        found = true;
        break;
      }
      n++;
      entry = Entry(entry.address + Entry::kSize);
      if (n >= l.capacity()) {
        break;
      }
    }
    if (!found) {
      continue;
    }
    if (entry.object().address == 0 && queued < 480) {
      const uint32_t index = Acquire(calls, frame, manager, pool, item.key_address());
      entry.set_object(LoadObject(calls, frame, manager, pool, index));
      m.set_last_pool(pool);
      m.set_last_index(index);
    }
    const uint32_t object = entry.object().address;
    if (object == 0) {
      continue;
    }
    item.set_object(object);
    SetExpiring(object, 0);
    SetVisible(item.object().address, 0);
    l.set_done(l.done() + 1);
  }
  return l.count() == l.done() ? 1 : 0;
}

uint32_t SceneryIsLoaded(const Calls& calls, uint32_t stack, uint32_t manager,
                         bool native_queue) {
  ITC_NATIVE_RUNNING("sub_820F1F68 Scenery::IsLoaded");
  // The original's 112-byte frame held only its back chain (E5 step 6).
  const Manager m(manager);
  if (m.loading_all() != 0) {
    if (ListLoadAll(calls, stack, manager, m.list_a_pool_address(), native_queue) == 0 ||
        ListLoadAll(calls, stack, manager, m.list_b_pool_address(), native_queue) == 0) {
      return 0;
    }
  }
  const uint32_t pool = m.last_pool();
  if (int32_t(pool) < 0) {
    return 0;
  }
  const uint32_t index = m.last_index();
  if (int32_t(index) < 0) {
    return 0;
  }
  const uint32_t object = PoolObjectAt(manager, pool, index);
  if (object == 0) {
    return 0;
  }
  if (InUse(object) != 0 && IsLoaded(object) == 0) {
    return 0;
  }
  if (m.loading_all() != 0) {
    m.set_loading_all(0);
  }
  return 1;
}

uint32_t ListStream(const Calls& calls, uint32_t stack, uint32_t manager,
                    uint32_t list, bool native_queue) {
  ITC_NATIVE_RUNNING("sub_820F16C0 SceneryList::Stream");
  const uint32_t frame = stack - 160;
  const List l(list);
  const Manager m(manager);
  const uint32_t pool = l.pool();
  const uint32_t queued = QueueLength(calls, stack, 0x820F16DC, native_queue);
  for (uint32_t i = 0; i < l.count(); i++) {
    const Item item(Load(l.pointers() + 4 * i));
    const uint32_t key = item.key_address();
    uint32_t index = Find(calls, frame, manager, pool, key);
    if (int32_t(index) < 0) {
      if (queued < 960) {
        index = Acquire(calls, frame, manager, pool, key);
        if (int32_t(index) < 0) {
          item.set_object(0u);
          continue;
        }
        LoadObject(calls, frame, manager, pool, index);
        m.set_last_pool(pool);
        m.set_last_index(index);
      }
      if (int32_t(index) < 0) {
        item.set_object(0u);
        continue;
      }
    }
    item.set_object(PoolObjectAt(manager, pool, index));
    SetExpiring(item.object().address, 0);
    SetVisible(item.object().address, 0);
  }
  return l.count() == l.done() ? 1 : 0;
}

uint32_t StartLoad(const Calls& calls, uint32_t stack, uint32_t manager) {
  ITC_NATIVE_RUNNING("sub_820F1E00 Scenery::StartLoad");
  const uint32_t frame = stack - 128;
  const Manager m(manager);
  uint32_t index = Acquire(calls, frame, manager, 2, 0);
  LoadObject(calls, frame, manager, 2, index);
  uint32_t r3 = PoolObjectAt(manager, 2, index);
  const uint32_t mode = m.mode();
  m.set_last_index(index);
  m.set_course_set(r3);
  m.set_last_pool(2);
  if (mode == 0) {
    const List list_a(m.list_a_pool_address()), list_b(m.list_b_pool_address());
    list_a.set_done(0);
    list_a.set_count(0);
    list_b.set_done(0);
    list_b.set_count(0);
    // The configuration's distance for every item.
    const double distance = G::item_distance();
    for (uint32_t i = 0; i < m.list_a_capacity(); i++) {
      const Entry entry(m.list_a_table().address + Entry::kSize * i);
      r3 = ListAdd(list_a.address, distance, entry.area(), entry.model(), 0, manager);
    }
    for (uint32_t i = 0; i < m.list_b_capacity(); i++) {
      const Entry entry(m.list_b_table().address + Entry::kSize * i);
      r3 = ListAdd(list_b.address, distance, entry.area(), entry.model(), 0, manager);
    }
  }
  if (m.pool0_on() != 0) {
    index = Acquire(calls, frame, manager, 0, 0);
    LoadObject(calls, frame, manager, 0, index);
    r3 = PoolObjectAt(manager, 0, index);
    m.set_last_pool(0);
    m.set_last_index(index);
    m.set_pool0_object(r3);
  }
  m.set_loading_all(1);
  return r3;
}

// --- The texture animations ---

uint32_t AdvanceTextureAnimations(uint32_t course_set) {
  ITC_NATIVE_RUNNING("sub_820F11A8 TextureAnimations::Advance");
  const uint32_t set = schema::CourseSet(course_set).texture_animations().address;
  return set == 0 ? 0 : AdvanceAnimationSet(set);
}

uint32_t AdvanceAnimationSet(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82146B20 AnimationSet::Advance");
  uint32_t r3 = set;
  const schema::AnimationSet s(set);
  for (int32_t i = 0; i < int32_t(s.count()); i++) {
    const uint32_t offsets = (s.count() + 1) * 4;
    r3 = offsets + schema::AnimationSet(set + 4 * uint32_t(i)).first_offset() + set;
    AdvanceAnimation(r3);
  }
  return r3;
}

uint64_t FindAnimationByTexture(uint32_t set, uint32_t texture) {
  ITC_NATIVE_RUNNING("sub_82146A38 AnimationSet::FindByTexture");
  const int32_t count = int32_t(schema::AnimationSet(set).count());
  for (int32_t i = 0; i < count; i++) {
    const uint32_t animation = uint32_t(count + 1) * 4 + set +
                               schema::AnimationSet(set + 4 * uint32_t(i)).first_offset();
    const int32_t frames = int32_t(schema::Animation(animation).frame_count());
    const uint32_t slot = frames > 0 ? uint32_t(frames + 4) * 4 + animation : 0;
    if (Load(slot + 4) == texture) {
      return uint64_t(i);
    }
  }
  return ~uint64_t(0);
}

void AdvanceAnimation(uint32_t animation) {
  ITC_NATIVE_RUNNING("sub_82146AC0 Animation::Advance");
  const schema::Animation a(animation);
  if (int32_t(a.tick()) > int32_t(a.length())) {
    a.set_tick(0);
    a.set_frame(0);
  }
  const uint32_t next = a.frame() + 1;
  if (int32_t(next) < int32_t(a.frame_count()) &&
      int32_t(a.tick()) >= int32_t(schema::Animation(animation + 4 * next).first_start())) {
    a.set_frame(next);
  }
  a.set_tick(a.tick() + 1);
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
    return CallGuest(*calls, stack, 512, lr, function, args);
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
uint32_t LoadModel(ParseCalls& parse, uint32_t model, uint32_t entry,
                   uint32_t lr) {
  if (model != 0 && models::ModelLoad(model, entry, 0, parse.ModelLoad())) {
    return schema::Model(model).shape_count();
  }
  return parse.Call(lr, 0x82596920, {model, entry, 0});
}

// XPR::Create natively, else its original; returns its r3.
uint32_t CreateXpr(ParseCalls& parse, uint32_t xpr, uint32_t file, uint32_t lr) {
  if (const auto r3 = models::XprCreate(xpr, file)) {
    return *r3;
  }
  return parse.Call(lr, 0x82596F68, {xpr, file});
}

}  // namespace

uint32_t CourseSetParse(const Calls& calls, uint32_t stack, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F1A70 CourseSet::Parse");
  ParseCalls parse{&calls, stack};
  // The original's 128-byte frame held only its back chain (E5 step 6): the
  // lists' loads make theirs below `stack`.
  const schema::CourseSet cs(set);
  const uint32_t first = cs.first_file();
  uint32_t entry = game_data::GetEntry(first, 0, true);
  if (entry != 0) {
    const uint32_t xpr = cs.xpr_address();
    if (xpr != 0) {
      models::XprConstruct(xpr);
    }
    CreateXpr(parse, xpr, entry, 0x820F1AB8);
  }
  entry = game_data::GetEntry(first, 1, true);
  if (entry != 0) {
    const uint32_t model = cs.model_address();
    if (model != 0 && !models::ModelConstruct(model)) {
      parse.Call(0x820F1AE0, 0x825948C0, {model});
    }
    LoadModel(parse, model, game_data::GetEntry(entry, 0, false), 0x820F1B00);
  }
  const scenery_lists::LoadCalls list_calls = parse.ListLoad();
  const uint32_t kLists[] = {cs.list_a_address(), cs.list_b_address(), cs.list_c_count_address(),
                             cs.list_d_count_address(), cs.list_e_count_address()};
  for (uint32_t i = 0; i < 5; i++) {
    entry = game_data::GetEntry(first, 2 + i, true);
    if (entry != 0) {
      const uint32_t list_frame = stack - 160;
      Store(list_frame, stack);
      scenery_lists::ModelListLoad(kLists[i], entry, list_frame, list_calls);
    }
  }
  entry = game_data::GetEntry(first, 7, true);
  if (entry != 0) {
    const uint32_t points_frame = stack - 192;
    Store(points_frame, stack);
    scenery_lists::ModelListLoadPoints(cs.points_address(), entry, points_frame, list_calls);
  }
  const uint32_t second = cs.second_file();
  for (uint32_t i = 0; i < 3; i++) {
    entry = game_data::GetEntry(second, i, true);
    if (entry != 0) {
      switch (i) {
        case 0: cs.set_area_file(entry); break;
        case 1: cs.set_texture_animations(entry); break;
        default: cs.set_light_groups(entry); break;
      }
    }
  }
  return entry;
}

uint32_t AreaLightsParse(uint32_t lights) {
  ITC_NATIVE_RUNNING("sub_820F11C0 AreaLights::Parse");
  const schema::AreaLights l(lights);
  const uint32_t entry = game_data::GetEntry(l.data(), 0, true);
  if (entry != 0) {
    l.set_light_file(entry);
  }
  return entry;
}

uint32_t AreaModelParse(const Calls& calls, uint32_t stack, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820F1258 AreaModel::Parse");
  ParseCalls parse{&calls, stack};
  const schema::AreaModel am(model);
  const uint32_t file = G::manager().load_buffer();
  const uint32_t entry = game_data::GetEntry(file, 0, true);
  const uint32_t xpr_entry = game_data::GetEntry(file, 1, true);
  uint32_t r3 = xpr_entry;
  if (entry != 0) {
    r3 = LoadModel(parse, am.own_model_address(), entry, 0x820F12C0);
    am.set_model(am.own_model_address());
  } else {
    am.set_model(0u);
  }
  if (xpr_entry != 0) {
    r3 = CreateXpr(parse, am.own_xpr_address(), xpr_entry, 0x820F12E0);
    am.set_xpr(am.own_xpr_address());
  } else {
    am.set_xpr(0u);
  }
  return r3;
}

uint32_t AreaLightsOnLoaded(uint32_t lights) {
  ITC_NATIVE_RUNNING("sub_820F1C30 AreaLights::OnLoaded");
  AreaLightsParse(lights);
  MarkLoaded(lights);
  return lights;
}

uint32_t AreaModelOnLoaded(const Calls& calls, uint32_t stack, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820F1CE8 AreaModel::OnLoaded");
  AreaModelParse(calls, stack, model);  // its 96-byte frame only a back chain
  MarkLoaded(model);
  return model;
}

// --- The pools' objects: made and reset ---

namespace {

// Model::Model natively, else its original.
void MakeModel(const ParseCalls& parse, uint32_t model, uint32_t lr) {
  if (!models::ModelConstruct(model)) {
    parse.Call(lr, 0x825948C0, {model});
  }
}

// Model::Release natively, else its original; its r3.
uint32_t ReleaseModel(const ParseCalls& parse, uint32_t model, uint32_t lr) {
  if (const auto r3 = models::ModelRelease(model)) {
    return *r3;
  }
  return parse.Call(lr, 0x82594970, {model});
}

// XPR::Release natively, else its original; its r3.
uint32_t ReleaseXpr(const ParseCalls& parse, uint32_t xpr, uint32_t lr) {
  if (const auto r3 = models::XprRelease(xpr)) {
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
    return ReleaseModel(*r.parse, model, 0x82148020);
  }
  static uint32_t Destroy(void* self, uint32_t model) {
    auto& r = *static_cast<ListRelease*>(self);
    const uint32_t slot = Load(Load(model));
    if (slot == kSceneryModelDestruct) {
      return SceneryModelDestruct(*r.parse->calls, r.parse->stack, model, 1);
    }
    return r.parse->Call(0x82148040, slot, {model, 1});
  }
};

}  // namespace

uint32_t MakePoolObject(const Calls& calls, uint32_t stack, uint32_t pool,
                        uint32_t object, uint32_t own_size, uint32_t data_size, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820F2638 Scenery::MakePoolObject");
  // The original's 96-byte frame held only its back chain (E5 step 6).
  if (pool >= 5 || object == 0) {
    return object;
  }
  switch (pool) {
    case 0:
    case 1:
      ConstructObject(object, own_size, data_size, index);
      schema::SceneryBlockObject(object).set_vtable(pool == 0 ? 0x8200674Cu : 0x82006758u);
      schema::SceneryBlockObject(object).set_block(0);
      break;
    case 2:
      CourseSetConstruct(calls, stack, object, own_size, data_size, index);
      break;
    case 3:
      ConstructObject(object, own_size, data_size, index);
      schema::AreaLights(object).set_vtable(schema::AreaLights::kVtable);
      models::XprConstruct(schema::AreaLights(object).xpr_address());
      break;
    default:
      AreaModelConstruct(calls, stack, object, own_size, data_size, index);
      break;
  }
  return object;
}

void CourseSetConstruct(const Calls& calls, uint32_t stack, uint32_t set,
                        uint32_t own_size, uint32_t data_size, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820F2478 CourseSet::CourseSet");
  // The original's 112-byte frame held only its back chain (E5 step 6).
  ConstructObject(set, own_size, data_size, index);
  const schema::CourseSet cs(set);
  cs.set_vtable(schema::CourseSet::kVtable);
  CourseSetInitParts(calls, stack, cs.xpr_address());
  cs.set_first_file(0);
  cs.set_second_file(0);
  cs.set_area_file(0u);
  cs.set_texture_animations(0u);
  if (cs.list_a_address() != 0) ModelListInit(cs.list_a_address());
  cs.set_light_groups(0u);
  for (uint32_t list : {cs.list_b_address(), cs.list_c_count_address(), cs.list_d_count_address(),
                        cs.list_e_count_address()}) {
    if (list != 0) ModelListInit(list);
  }
  const uint32_t points = cs.points_address();
  if (points != 0) {
    ModelListInit(points);
    cs.set_points_animations(0);
    cs.set_points_provider(0);
    cs.set_points_race_word(0);
    cs.set_points_count(0);
  }
}

void CourseSetInitParts(const Calls& calls, uint32_t stack, uint32_t parts) {
  ITC_NATIVE_RUNNING("sub_820F2230 CourseSet::InitParts");
  ParseCalls parse{&calls, stack};
  models::XprConstruct(parts);
  MakeModel(parse, parts + 32, 0x820F2254);
  for (uint32_t offset : {464u, 1000u, 1532u, 2060u, 2588u, 3116u}) {
    ModelListInit(parts + offset);
  }
  const schema::ModelList points(parts + 3116);
  points.set_animations(0u);
  points.set_texture_provider(0);
  points.set_first_group(0);
  points.set_point_group_count(0);
}

void AreaModelConstruct(const Calls& calls, uint32_t stack, uint32_t model,
                        uint32_t own_size, uint32_t data_size, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820F1C68 AreaModel::AreaModel");
  ParseCalls parse{&calls, stack};
  ConstructObject(model, own_size, data_size, index);
  const schema::AreaModel am(model);
  am.set_vtable(schema::AreaModel::kVtable);
  MakeModel(parse, am.own_model_address(), 0x820F1C9C);
  am.set_own_model(scenery_lists::kSceneryModelVtable);
  models::XprConstruct(am.own_xpr_address());
  am.set_model(0u);
  am.set_xpr(0u);
}

uint32_t CourseSetReset(const Calls& calls, uint32_t stack, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F1138 CourseSet::Reset");
  ParseCalls parse{&calls, stack};
  const schema::CourseSet cs(set);
  ReleaseXpr(parse, cs.xpr_address(), 0x820F1154);
  ReleaseModel(parse, cs.model_address(), 0x820F115C);
  ListRelease list_release{&parse};
  scenery_lists::ReleaseCalls release;
  release.release = &ListRelease::Release;
  release.destroy = &ListRelease::Destroy;
  release.user = &list_release;
  for (uint32_t list : {cs.list_a_address(), cs.list_b_address(), cs.list_c_count_address(),
                        cs.list_d_count_address(), cs.list_e_count_address(),
                        cs.points_address()}) {
    scenery_lists::ModelListRelease(list, release);
  }
  ResetObject(set);
  return set;
}

uint32_t AreaModelReset(const Calls& calls, uint32_t stack, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820F12F8 AreaModel::Reset");
  ParseCalls parse{&calls, stack};
  const schema::AreaModel am(model);
  const uint32_t mesh = am.model().address;
  if (mesh != 0) {
    ReleaseModel(parse, mesh, 0x820F1324);
    am.set_model(0u);
  }
  const uint32_t xpr = am.xpr().address;
  if (xpr != 0) {
    ReleaseXpr(parse, xpr, 0x820F1338);
    am.set_xpr(0u);
  }
  ResetObject(model);
  return model;
}

uint32_t SceneryModelDestruct(const Calls& calls, uint32_t stack, uint32_t model,
                              uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820F1208 AreaModel::~Model");
  ParseCalls parse{&calls, stack};
  if (!models::ModelDestruct(model)) {
    parse.Call(0x820F1228, 0x82594F58, {model});
  }
  if ((flags & 1) != 0 && !memory::crt::OperatorDelete(model)) {
    parse.Call(0x820F1238, 0x825A70E8, {model});
  }
  return model;
}

// --- The pools' loads and the course set's callback ---

namespace {

// r3 as Archive::Load's adapter leaves a serial (-1 as li's).
uint64_t SerialR3(int64_t serial) {
  return serial < 0 ? ~uint64_t(0) : uint64_t(uint32_t(serial));
}

// The asset of a list's table entry (12 bytes: the area and model
// halfwords, the asset) whose halfwords are `key`'s, else -1.
uint32_t TableAsset(uint32_t table, uint32_t count, uint32_t key) {
  const Entry k(key);  // the key's halfwords as an entry's
  const uint32_t area = k.area();
  for (uint32_t i = 0; i < count; i++) {
    const Entry entry(table + Entry::kSize * i);
    if (area == entry.area() && k.model() == entry.model()) {
      return entry.asset();
    }
  }
  return 0xFFFFFFFFu;
}

}  // namespace

uint64_t CourseSetLoad(uint32_t set, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820F2520 CourseSet::Load");
  const Manager m = G::manager();
  const schema::CourseSet cs(set);
  ClearLoadFlags(set);
  cs.set_area_file(0u);
  cs.set_texture_animations(0u);
  uint32_t assets[2];
  uint32_t last = 0;
  // The two files' words: +3808 the first, +3812 the second.
  auto file = [&](uint32_t i) { return i == 0 ? cs.first_file_address() : cs.second_file_address(); };
  for (uint32_t i = 0; i < 2; i++) {
    const Table table = m.course_table();
    assets[i] = i == 0 ? table.set_first_asset() : table.set_second_asset();
    if (int32_t(assets[i]) < 0) {
      Store(file(i), 0);
      continue;
    }
    last = i;
    const uint32_t size = archive::AssetSize(assets[i]);
    if (i == 0) {
      cs.set_first_file(AllocPhysical(size));
    } else {
      cs.set_second_file(AllocData(Object(set).data_address(), size));
    }
  }
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 2; i++) {
    r3 = assets[i];
    if (int32_t(assets[i]) < 0) {
      continue;
    }
    r3 = SerialR3(load(assets[i], Load(file(i)),
                       i == last ? kCourseSetOnLoaded : 0, set));
  }
  return r3;
}

uint64_t AreaLightsLoad(uint32_t lights, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820F22F8 AreaLights::Load");
  const Manager m = G::manager();
  const schema::AreaLights l(lights);
  ClearLoadFlags(lights);
  l.set_light_file(0u);
  const uint32_t asset =
      TableAsset(m.list_a_table().address, m.list_a_capacity(), l.key_address());
  const uint32_t data = AllocData(l.data_address(), archive::AssetSize(asset));
  return SerialR3(load(asset, data, kAreaLightsOnLoaded, lights));
}

uint64_t AreaModelLoad(uint32_t model, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820F23C0 AreaModel::Load");
  const Manager m = G::manager();
  const schema::AreaModel am(model);
  ClearLoadFlags(model);
  am.set_model(0u);
  am.set_xpr(0u);
  const uint32_t asset =
      TableAsset(m.list_b_table().address, m.list_b_capacity(), am.key_address());
  return SerialR3(load(asset, m.load_buffer(), kAreaModelOnLoaded, model));
}

uint32_t CourseSetOnLoaded(const Calls& calls, uint32_t stack, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F22B8 CourseSet::OnLoaded");
  CourseSetParse(calls, stack, set);  // its 96-byte frame only a back chain
  FreePhysical(schema::CourseSet(set).first_file());
  MarkLoaded(set);
  return set;
}

}  // namespace course_scenery
