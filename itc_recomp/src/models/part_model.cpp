// The car part model (see part_model.h), store for store as the recompiled
// functions.

#include "models/part_model.h"

#include "base/log.h"
#include "guest.h"
#include "models/model.h"
#include "schema/models.h"

namespace models {

namespace {

// The words after the base Model, in the originals' order.
void Clear(uint32_t model) {
  const schema::Model m(model);
  m.set_vinyl_sheet(0u);
  m.set_vinyl_texture_b(0u);
  m.set_category3_texture(0u);
  m.set_override_texture(0u);
  m.set_replacement_texture(0u);
  m.set_use_override_texture(0u);
  m.set_replaced_texture_index(0xFFFFFFFFu);
  m.set_force_pair_1005(0u);
}

}  // namespace

void PartModelClear(uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820CE9C8 PartModel::Clear");
  Clear(model);
}

void PartModelSetVinylTextures(uint32_t model, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820CE9F8 Model::SetVinylTextures");
  const schema::Model m(model);
  m.set_vinyl_sheet(a);
  m.set_vinyl_texture_b(b);
}

bool PartModelConstruct(uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820CE928 PartModel::PartModel");
  if (!ModelConstruct(model)) {
    return false;
  }
  schema::Model(model).set_vtable(kPartModelVtable);
  Clear(model);
  return true;
}

std::optional<uint32_t> PartModelDestruct(uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820CE990 PartModel::~PartModel");
  schema::Model(model).set_vtable(kPartModelVtable);
  Clear(model);
  return ModelDestruct(model);
}

}  // namespace models
