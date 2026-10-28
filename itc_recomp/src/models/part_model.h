// The car part model (docs/functions/models.md: sub_820CE928, sub_820CE990,
// sub_820CE9C8): a Model (models/model.h, 432 bytes) with its own vtable and
// eight words after it (+432 to +460, +436 = -1), the 464-byte models the
// car's loading objects keep for their parts (docs/functions/
// scene-loaders.md). The constructor and destructor make and destroy the
// base Model through its plain C++, so they run natively only while
// hooks::ModelsNativeLifetime() (the adapters' condition). Adapters:
// src/hooks/models.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace models {

constexpr uint32_t kPartModelVtable = 0x8200338C;
constexpr uint32_t kPartModelBytes = 464;

// sub_820CE9C8: +444 to +460, +432 and +440 = 0, +436 = -1 (the original's
// order).
void PartModelClear(uint8_t* base, uint32_t model);

// sub_820CE928 (the part model's constructor): Model::Model, the vtable,
// then PartModelClear. False, with nothing done, where Model::Model would go
// to the original.
bool PartModelConstruct(uint8_t* base, uint32_t model);

// sub_820CE990 (the part model's destructor): the vtable, PartModelClear,
// then Model::~Model. Returns the original's r3 (Model::~Model's); nullopt
// where Model::~Model would go to the original, with only the part's own
// words written (the original writes the same first).
std::optional<uint32_t> PartModelDestruct(uint8_t* base, uint32_t model);

}  // namespace models
