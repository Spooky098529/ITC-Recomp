// The car part model (see part_model.h), store for store as the recompiled
// functions.

#include "models/part_model.h"

#include "base/log.h"
#include "guest.h"
#include "models/model.h"

namespace models {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The words after the base Model, in the originals' order.
void Clear(uint8_t* base, uint32_t model) {
  for (uint32_t offset : {444u, 448u, 452u, 456u, 460u, 432u}) {
    Store(base, model + offset, 0);
  }
  Store(base, model + 436, 0xFFFFFFFFu);
  Store(base, model + 440, 0);
}

}  // namespace

void PartModelClear(uint8_t* base, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820CE9C8 PartModel::Clear");
  Clear(base, model);
}

bool PartModelConstruct(uint8_t* base, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820CE928 PartModel::PartModel");
  if (!ModelConstruct(base, model)) {
    return false;
  }
  Store(base, model, kPartModelVtable);
  Clear(base, model);
  return true;
}

std::optional<uint32_t> PartModelDestruct(uint8_t* base, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820CE990 PartModel::~PartModel");
  Store(base, model, kPartModelVtable);
  Clear(base, model);
  return ModelDestruct(base, model);
}

}  // namespace models
