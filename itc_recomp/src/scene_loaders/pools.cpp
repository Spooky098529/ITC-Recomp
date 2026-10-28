// The scene's loading objects (see pools.h), store for store as the
// recompiled functions.

#include "scene_loaders/pools.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/heap.h"
#include "models/model.h"
#include "scene_models/scene_models.h"

namespace scene_loaders {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
int32_t LoadS(uint8_t* base, uint32_t address) {
  return int32_t(guest::LoadU32(base, address));
}

// The guest views memcpy's override records copies into for the renderer.
constexpr uint32_t kPhysical = 0xA0000000u;

// The model set ([[object + offset] + 8]: 0 a, 1 b).
uint32_t ModelSet(uint8_t* base, uint32_t object, uint32_t offset) {
  return Load(base, Load(base, object + offset) + 8);
}

// Whether any of `count` slot states from `states` (24 bytes apart) is
// loading.
bool AnyLoading(uint8_t* base, uint32_t states, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    if (LoadS(base, states + kSlotBytes * i) == kSlotLoading) {
      return true;
    }
  }
  return false;
}

}  // namespace

// --- The pool set ---

uint32_t MakeA(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820C3C40 ScenePools::MakeA");
  const uint32_t set = memory::AllocPages(kFirstSetBytes, 4096);
  Store(base, kFirstSet, set);
  Store(base, set + 4, 0);
  return set;
}

uint32_t AllocPages(uint8_t* base, uint32_t out, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_820D0960 AllocPages");
  const uint32_t pages = memory::AllocPages(size, 4096);
  Store(base, out, pages);
  return pages;
}

// --- The pool node ---

void PoolNodeConstruct(uint8_t* base, uint32_t node, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82594050 PoolNode::PoolNode");
  Store(base, node + 4, list);
  Store(base, node + 8, 0);
  Store(base, node + 0, kPoolNodeVtable);
  Store(base, node + 12, 0);
  uint32_t owner;
  if (Load(base, list + 0) == 0) {
    Store(base, list + 0, node);
    owner = Load(base, node + 4);
  } else {
    Store(base, Load(base, list + 4) + 8, node);
    owner = Load(base, node + 4);
    Store(base, node + 12, Load(base, owner + 4));
  }
  Store(base, owner + 4, node);
  owner = Load(base, node + 4);
  Store(base, owner + 8, Load(base, owner + 8) + 1);
}

void PoolNodeDestruct(uint8_t* base, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_825940B0 PoolNode::~PoolNode");
  Store(base, node + 0, kPoolNodeVtable);
  const uint32_t list = Load(base, node + 4);
  if (list == 0) {
    return;
  }
  const uint32_t previous = Load(base, node + 12);
  if (previous == 0) {
    if (Load(base, node + 8) == 0) {
      // The only node.
      Store(base, list + 0, 0);
      Store(base, Load(base, node + 4) + 4, 0);
    } else {
      // The first node.
      Store(base, list + 0, Load(base, node + 8));
      Store(base, Load(base, Load(base, node + 4) + 0) + 12, 0);
    }
  } else {
    const uint32_t next = Load(base, node + 8);
    if (next == 0) {
      // The last node.
      Store(base, list + 4, previous);
      Store(base, Load(base, Load(base, node + 4) + 4) + 8, 0);
    } else {
      Store(base, previous + 8, next);
      Store(base, Load(base, node + 8) + 12, Load(base, node + 12));
    }
  }
  const uint32_t owner = Load(base, node + 4);
  Store(base, owner + 8, Load(base, owner + 8) - 1);
}

// --- The loading objects' queries ---

uint32_t CarModelIsLoaded(uint8_t* base, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820D3C28 CarModel::IsLoaded");
  if (AnyLoading(base, car_model + 3972, 2)) {
    return 0;
  }
  return LoadS(base, car_model + 3748) == kSlotLoading ? 0 : 1;
}

uint32_t Pool4IsLoaded(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820D6A08 Pool4Object::IsLoaded");
  return LoadS(base, object + 100) == kSlotLoading ? 0 : 1;
}

bool Pool4SetRecord(uint8_t* base, uint32_t object, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D6A60 Pool4Object::SetRecord");
  const uint32_t to = object + 28;
  if (to >= kPhysical) {
    return false;
  }
  return c_runtime::Memcpy(base, to, record, 52);
}

uint64_t PartsSlotOf(uint64_t r3, uint32_t kind, uint64_t base_slot, uint64_t choice) {
  ITC_NATIVE_RUNNING("sub_820D6D48 CarParts::SlotOf");
  const uint64_t choice4 = uint32_t(uint32_t(choice) << 2);
  switch (kind) {
    case 0: return base_slot;
    case 1: return choice * 7 + base_slot + 13;
    case 2: return base_slot + 104;
    case 3: return base_slot + 117;
    case 4: return base_slot + 130;
    case 5: return base_slot + 143;
    case 6: return base_slot + 151;
    case 7: return base_slot + 154;
    case 8: return choice4 + base_slot + 157;
    case 9: return choice4 + base_slot + 169;
    case 10: return base_slot + 181;
    case 11: return base_slot + 185;
    case 12: return base_slot + 190;
    case 13: return 189;
    case 14: return base_slot + 196;
    case 15: return base_slot + 211;
    default: return r3;
  }
}

uint32_t PartsPart(uint8_t* base, uint32_t parts, int32_t kind, uint32_t base_slot,
                   uint32_t choice) {
  ITC_NATIVE_RUNNING("sub_820D6E10 CarParts::Part");
  const uint32_t set = ModelSet(base, parts, kPartsSet);
  uint32_t slot;
  if (set == 0) {
    if (kind == 16) {
      return 0;
    }
    slot = uint32_t(PartsSlotOf(parts, uint32_t(kind), base_slot, choice));
  } else if (set == 1) {
    if (kind == 7 || kind == 9 || kind == 14) {
      return 0;
    }
    slot = uint32_t(kind);
  } else {
    return 0;
  }
  if (LoadS(base, parts + kSlotBytes * slot + kPartsStates) != kSlotArrived) {
    return 0;
  }
  return parts + kPartModelBytes * slot + kPartsModels;
}

uint32_t PartsIsLoaded(uint8_t* base, uint32_t parts) {
  ITC_NATIVE_RUNNING("sub_820D7258 CarParts::IsLoaded");
  const uint32_t set = ModelSet(base, parts, kPartsSet);
  if (set > 1) {
    return 0;
  }
  return AnyLoading(base, parts + kPartsStates, set == 0 ? kPartsCount : 17) ? 0 : 1;
}

uint32_t PartWalkMaterials(uint8_t* base, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820D2270 PartModel::WalkMaterials");
  if (model == 0) {
    return 0;
  }
  const int32_t shapes = LoadS(base, model + 412);
  uint32_t r3 = model;
  for (int32_t shape = 0; shape < shapes; shape++) {
    const int32_t first = int32_t(models::MaterialCount(base, model, uint32_t(shape), 0));
    const int32_t second = int32_t(models::MaterialCount(base, model, uint32_t(shape), 1));
    r3 = uint32_t(second);
    for (int32_t i = 0; i < first; i++) {
      r3 = models::Material(base, model, uint32_t(shape), uint32_t(i), 0);
    }
    for (int32_t i = 0; i < second; i++) {
      r3 = models::Material(base, model, uint32_t(shape), uint32_t(i), 1);
    }
  }
  return r3;
}

uint32_t PartsWalkMaterials(uint8_t* base, uint32_t parts) {
  ITC_NATIVE_RUNNING("sub_820D6EB8 CarParts::WalkMaterials");
  const uint32_t count = ModelSet(base, parts, kPartsSet) == 0 ? kPartsCount : 187;
  uint32_t r3 = parts;
  for (uint32_t i = 0; i < count; i++) {
    if (LoadS(base, parts + kPartsStates + kSlotBytes * i) == kSlotArrived) {
      r3 = PartWalkMaterials(base, parts + kPartsModels + kPartModelBytes * i);
    }
  }
  return r3;
}

uint32_t AccessoriesAllLoaded(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F07D8 Accessories::AllLoaded");
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t entry = Load(base, set + 4 + 4 * i);
    if (entry != 0 && Load(base, entry + 1096) == 0) {
      return 0;
    }
  }
  return 1;
}

uint32_t InteriorIsLoaded(uint8_t* base, uint32_t interior) {
  ITC_NATIVE_RUNNING("sub_820D7790 Interior::IsLoaded");
  const uint32_t set = ModelSet(base, interior, 3092);
  uint32_t loaded = 0;
  if (set <= 1) {
    loaded = AnyLoading(base, interior + 180, 6) ? 0 : 1;
  }
  if (AccessoriesAllLoaded(base, scene_models::Accessories(base)) == 0) {
    loaded = 0;
  }
  return loaded;
}

// --- Names ---

uint32_t NameMatches(uint8_t* base, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820CE800 NameMatches");
  auto length = [&](uint32_t s) {
    uint32_t n = 0;
    while (guest::LoadU8(base, s + n) != 0) {
      n++;
    }
    return int32_t(n);
  };
  const int32_t count = length(a);
  if (count != length(b)) {
    return 0;
  }
  auto upper = [](uint8_t byte) {
    const int32_t c = int8_t(byte);
    return int8_t(c + c_runtime::ToUpperStep(c));
  };
  for (int32_t i = 0; i < count; i++) {
    const int8_t ca = upper(guest::LoadU8(base, a + uint32_t(i)));
    const int8_t cb = upper(guest::LoadU8(base, b + uint32_t(i)));
    if (ca != '*' && cb != '*' && ca != cb) {
      return 0;
    }
  }
  return 1;
}

uint32_t XprCount(uint8_t* base, uint32_t xpr) {
  ITC_NATIVE_RUNNING("sub_82596F20 XPR::Count");
  return Load(base, xpr + 16) == 0 ? 0 : Load(base, xpr + 20);
}

uint32_t XprRecordName(uint8_t* base, uint32_t xpr, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82596F40 XPR::RecordName");
  if (index >= Load(base, xpr + 20)) {
    return 0;
  }
  return Load(base, Load(base, xpr + 16) + 16 * index + 12);
}

int32_t XprFindByName(uint8_t* base, uint32_t xpr, uint32_t name) {
  ITC_NATIVE_RUNNING("sub_820D09B8 XPR::FindByName");
  const uint32_t count = XprCount(base, xpr);
  for (uint32_t i = 0; i < count; i++) {
    if (NameMatches(base, name, XprRecordName(base, xpr, i)) != 0) {
      return int32_t(i);
    }
  }
  return -1;
}

}  // namespace scene_loaders
