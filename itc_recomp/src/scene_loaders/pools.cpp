// The scene's loading objects (see pools.h), store for store as the
// recompiled functions.

#include "scene_loaders/pools.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "material_passes/material_passes.h"
#include "memory/heap.h"
#include "models/model.h"
#include "models/xpr.h"
#include "scene_loaders/objects.h"
#include "scene_models/scene_models.h"
#include "schema/accessories.h"
#include "schema/car_models.h"
#include "schema/material_passes.h"
#include "schema/models.h"
#include "schema/scene_loaders.h"
#include "schema/stand_models.h"

namespace scene_loaders {

namespace {

using G = schema::SceneLoadersGlobals;

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
  G::set_first_set(base, set);
  schema::ScenePools(base, set).set_state(0);
  return set;
}

uint32_t AllocPages(uint8_t* base, uint32_t out, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_820D0960 AllocPages");
  const uint32_t pages = memory::AllocPages(size, 4096);
  Store(base, out, pages);
  return pages;
}

uint32_t CreateA(uint8_t* base, uint32_t set, uint32_t record, const CreateCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820C3FD8 ScenePools::CreateA");
  const schema::ScenePools s(base, set);
  const schema::ScenePoolsCreateRecord rec(base, record);
  s.set_state(3);
  s.set_model_set(rec.model_set());
  s.set_car_model_count(rec.car_models());
  const uint32_t pool0 = s.car_model_count();
  s.set_car_parts_count(rec.car_parts());
  s.set_interior_count(rec.interiors() + rec.cockpits() + rec.traffic());
  s.set_cockpit_count(rec.cockpits());
  s.set_pool4_count(rec.pool4());
  s.set_wheels_count(rec.wheels() + rec.traffic() + rec.extra_wheels());
  s.set_unused_sum(rec.traffic() + rec.extra_wheels());
  const uint32_t pool6 = rec.traffic();
  s.set_traffic_count(pool6);
  uint32_t r3 = AllocPages(base, s.deferred_releases_address(), (pool0 + pool6) * 80);
  auto records = [&] { return int32_t((s.car_model_count() + s.traffic_count()) << 2); };
  for (int32_t i = 0; i < records(); i++) {
    schema::CarModelsDeferredRelease(base, s.deferred_releases().address + 20 * uint32_t(i))
        .set_state(0);
  }
  // The counts in the set, by pool.
  const uint32_t counts[7] = {s.car_model_count_address(), s.car_parts_count_address(),
                              s.cockpit_count_address(),   s.interior_count_address(),
                              s.pool4_count_address(),     s.wheels_count_address(),
                              s.traffic_count_address()};
  using Construct = bool (*)(uint8_t*, uint32_t, uint32_t);
  static constexpr Construct kConstructs[7] = {CarModelConstruct, CarPartsConstruct,
                                               CockpitConstruct,  InteriorConstruct,
                                               Pool4Construct,    WheelsConstruct,
                                               TrafficConstruct};
  for (uint32_t pool = 0; pool < 7; pool++) {
    const int32_t count = LoadS(base, counts[pool]);
    const uint32_t list = s.pools_address() + 12 * pool;
    for (int32_t i = 0; i < count; i++) {
      // Pools 0 and 5 (the car models and the wheels) in host memory (E5
      // step 5: handles, freed by ScenePools::DestroyA's VirtualFree); the
      // others the layer's pages.
      const uint32_t object = pool == 0   ? schema::CarModel::New(base).address
                              : pool == 5 ? schema::CarWheels::New(base).address
                                          : memory::AllocPages(kPoolObjectBytes[pool], 4096);
      r3 = object;
      if (object != 0 && !kConstructs[pool](base, object, list)) {
        calls.construct(calls.user, pool, object, list);
      }
      schema::PoolNode(base, object).set_index(uint32_t(i));
    }
  }
  return r3;
}

// --- The pool node ---

void PoolNodeConstruct(uint8_t* base, uint32_t node, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82594050 PoolNode::PoolNode");
  const schema::PoolNode n(base, node);
  const schema::PoolList l(base, list);
  n.set_list(list);
  n.set_next(0);
  n.set_vtable(kPoolNodeVtable);
  n.set_previous(0);
  schema::PoolList owner;
  if (l.first().address == 0) {
    l.set_first(node);
    owner = n.list();
  } else {
    l.last().set_next(node);
    owner = n.list();
    n.set_previous(owner.last().address);
  }
  owner.set_last(node);
  owner = n.list();
  owner.set_count(owner.count() + 1);
}

void PoolNodeDestruct(uint8_t* base, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_825940B0 PoolNode::~PoolNode");
  const schema::PoolNode n(base, node);
  n.set_vtable(kPoolNodeVtable);
  const schema::PoolList list = n.list();
  if (list.address == 0) {
    return;
  }
  const schema::PoolNode previous = n.previous();
  if (previous.address == 0) {
    if (n.next().address == 0) {
      // The only node.
      list.set_first(0);
      n.list().set_last(0);
    } else {
      // The first node.
      list.set_first(n.next().address);
      n.list().first().set_previous(0);
    }
  } else {
    const schema::PoolNode next = n.next();
    if (next.address == 0) {
      // The last node.
      list.set_last(previous.address);
      n.list().last().set_next(0);
    } else {
      previous.set_next(next.address);
      n.next().set_previous(n.previous().address);
    }
  }
  const schema::PoolList owner = n.list();
  owner.set_count(owner.count() - 1);
}

// --- The loading objects' queries ---

uint32_t CarModelIsLoaded(uint8_t* base, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820D3C28 CarModel::IsLoaded");
  const schema::CarModel cm(base, car_model);
  // The two XPR2 slots' states.
  if (AnyLoading(base, cm.body_xpr_state_address(), 2)) {
    return 0;
  }
  return int32_t(cm.node_file_state()) == kSlotLoading ? 0 : 1;
}

uint32_t Pool4IsLoaded(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820D6A08 Pool4Object::IsLoaded");
  return int32_t(schema::Pool4Object(base, object).slot_state()) == kSlotLoading ? 0 : 1;
}

bool Pool4SetRecord(uint8_t* base, uint32_t object, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D6A60 Pool4Object::SetRecord");
  const uint32_t to = schema::Pool4Object(base, object).record_address();
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
  const schema::CarParts p(base, parts);
  const uint32_t set = p.first_set().model_set();
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
  if (int32_t(schema::LoadSlot(base, p.slots_address() + kSlotBytes * slot).state()) !=
      kSlotArrived) {
    return 0;
  }
  return p.part_models_address() + kPartModelBytes * slot;
}

uint32_t PartsIsLoaded(uint8_t* base, uint32_t parts) {
  ITC_NATIVE_RUNNING("sub_820D7258 CarParts::IsLoaded");
  const schema::CarParts p(base, parts);
  const uint32_t set = p.first_set().model_set();
  if (set > 1) {
    return 0;
  }
  return AnyLoading(base, schema::LoadSlot(base, p.slots_address()).state_address(),
                    set == 0 ? kPartsCount : 17)
             ? 0
             : 1;
}

uint32_t PartWalkMaterials(uint8_t* base, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820D2270 PartModel::WalkMaterials");
  if (model == 0) {
    return 0;
  }
  const int32_t shapes = int32_t(schema::PartModel(base, model).shape_count());
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
  const schema::CarParts p(base, parts);
  const uint32_t count = p.first_set().model_set() == 0 ? kPartsCount : 187;
  uint32_t r3 = parts;
  for (uint32_t i = 0; i < count; i++) {
    if (int32_t(schema::LoadSlot(base, p.slots_address() + kSlotBytes * i).state()) ==
        kSlotArrived) {
      r3 = PartWalkMaterials(base, p.part_models_address() + kPartModelBytes * i);
    }
  }
  return r3;
}

namespace {

// The car parts' walk's count: 216 for set a, else 187.
uint32_t PartsWalkCount(uint8_t* base, uint32_t parts) {
  return schema::CarParts(base, parts).first_set().model_set() == 0 ? kPartsCount : 187;
}

}  // namespace

uint64_t PartsApplyPalette(uint8_t* base, uint32_t parts, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D6F20 CarParts::ApplyPalette");
  const schema::CarParts p(base, parts);
  const uint32_t count = PartsWalkCount(base, parts);
  for (uint32_t i = 0; i < count; i++) {
    if (int32_t(schema::LoadSlot(base, p.slots_address() + kSlotBytes * i).state()) ==
        kSlotArrived) {
      r3 = material_passes::ApplyPalette(base, p.part_models_address() + kPartModelBytes * i,
                                         sp - 112);
    }
  }
  return r3;
}

uint64_t PartsSetPaint(uint8_t* base, uint32_t parts, uint64_t r3, const uint64_t regs[6],
                       uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D7388 CarParts::SetPaint");
  const schema::CarParts p(base, parts);
  const uint32_t count = PartsWalkCount(base, parts);
  for (uint32_t i = 0; i < count; i++) {
    if (int32_t(schema::LoadSlot(base, p.slots_address() + kSlotBytes * i).state()) ==
        kSlotArrived) {
      r3 = material_passes::SetPaint(base, p.part_models_address() + kPartModelBytes * i, regs,
                                     sp - 160);
    }
  }
  return r3;
}

namespace {

// ApplyPalette (regs null) or SetPaint on each of the six part models from
// `models_at` whose state (from `states`, 24 bytes apart) is 3.
uint64_t PaintSix(uint8_t* base, uint32_t states, uint32_t models_at, uint64_t r3,
                  const uint64_t* regs, uint32_t sp) {
  for (uint32_t i = 0; i < 6; i++) {
    if (LoadS(base, states + kSlotBytes * i) == kSlotArrived) {
      const uint32_t model = models_at + kPartModelBytes * i;
      r3 = regs == nullptr ? material_passes::ApplyPalette(base, model, sp - 112)
                           : material_passes::SetPaint(base, model, regs, sp - 160);
    }
  }
  return r3;
}

}  // namespace

uint64_t CockpitApplyPalette(uint8_t* base, uint32_t cockpit, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D5FC8 Cockpit::ApplyPalette");
  const schema::Cockpit c(base, cockpit);
  return PaintSix(base, c.part_slot_state_address(), c.part_models_address(), r3, nullptr, sp);
}

uint64_t CockpitSetPaint(uint8_t* base, uint32_t cockpit, uint64_t r3, const uint64_t regs[6],
                         uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D6328 Cockpit::SetPaint");
  const schema::Cockpit c(base, cockpit);
  return PaintSix(base, c.part_slot_state_address(), c.part_models_address(), r3, regs, sp);
}

uint64_t InteriorApplyPalette(uint8_t* base, uint32_t interior, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D7690 Interior::ApplyPalette");
  const schema::Interior in(base, interior);
  return PaintSix(base, in.part_slot_state_address(), in.part_models_address(), r3, nullptr, sp);
}

uint64_t InteriorSetPaint(uint8_t* base, uint32_t interior, uint64_t r3, const uint64_t regs[6],
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D7840 Interior::SetPaint");
  const schema::Interior in(base, interior);
  return PaintSix(base, in.part_slot_state_address(), in.part_models_address(), r3, regs, sp);
}

uint64_t TrafficApplyPalette(uint8_t* base, uint32_t traffic, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D7BF8 TrafficCar::ApplyPalette");
  const schema::TrafficCar t(base, traffic);
  return PaintSix(base, t.slot0_state_address(), t.part_models_address(), r3, nullptr, sp);
}

uint64_t CarModelSelectedTexture(uint8_t* base, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820D4588 CarModel::SelectedTexture");
  const schema::CarModel cm(base, car_model);
  // The original's ((+3712 + 1808) << 2) + car_model: +7232's index.
  const uint32_t index = Load(base, cm.xpr_indices_address() + (cm.selected_texture() << 2));
  if (int32_t(index) == -1) {
    return 0;
  }
  return models::XprGetTexture(base, cm.xpr_objects_address(), index);
}

uint32_t AccessoriesAllLoaded(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F07D8 Accessories::AllLoaded");
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t entry = Load(base, schema::Accessories(base, set).entry_0_address() + 4 * i);
    if (entry != 0 && schema::AccessoryEntry(base, entry).accessory_loaded() == 0) {
      return 0;
    }
  }
  return 1;
}

uint32_t InteriorIsLoaded(uint8_t* base, uint32_t interior) {
  ITC_NATIVE_RUNNING("sub_820D7790 Interior::IsLoaded");
  const schema::Interior in(base, interior);
  const uint32_t set = in.first_set().model_set();
  uint32_t loaded = 0;
  if (set <= 1) {
    loaded = AnyLoading(base, in.part_slot_state_address(), 6) ? 0 : 1;
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
    while (guest::LoadU8(base, schema::SceneLoadersName(base, s).text_address() + n) != 0) {
      n++;
    }
    return int32_t(n);
  };
  const schema::SceneLoadersName name_a(base, a);
  const schema::SceneLoadersName name_b(base, b);
  const int32_t count = length(a);
  if (count != length(b)) {
    return 0;
  }
  auto upper = [](uint8_t byte) {
    const int32_t c = int8_t(byte);
    return int8_t(c + c_runtime::ToUpperStep(c));
  };
  for (int32_t i = 0; i < count; i++) {
    const int8_t ca = upper(guest::LoadU8(base, name_a.text_address() + uint32_t(i)));
    const int8_t cb = upper(guest::LoadU8(base, name_b.text_address() + uint32_t(i)));
    if (ca != '*' && cb != '*' && ca != cb) {
      return 0;
    }
  }
  return 1;
}

uint32_t XprCount(uint8_t* base, uint32_t xpr) {
  ITC_NATIVE_RUNNING("sub_82596F20 XPR::Count");
  const schema::XPR x(base, xpr);
  return x.records().address == 0 ? 0 : x.record_count();
}

uint32_t XprRecordName(uint8_t* base, uint32_t xpr, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82596F40 XPR::RecordName");
  const schema::XPR x(base, xpr);
  if (index >= x.record_count()) {
    return 0;
  }
  return schema::XprRecord(base, x.records().address + 16 * index).name();
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
