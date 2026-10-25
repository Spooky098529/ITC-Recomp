// The graphics library's model class (docs/functions/models.md, "Model::";
// the layout in docs/known-data.md) as plain C++: its constructor, release
// and destructor. The members are other subsystems' plain C++: the XPR and
// the texture provider (models/xpr.h, texture_provider.h), the skeleton and
// the track sets (model_animation/model_animation.h). The release puts its
// blocks off (models/memory_blocks.h), so these run natively only while
// --native_heap routes the game's heap to the allocation layer (the
// adapters' condition). Each checks first what its callees would leave to
// the original (a block the layer didn't give, no room in the pending
// frees), and returns nullopt with nothing changed then. Adapters:
// src/hooks/models.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace models {

constexpr uint32_t kModelVtable = 0x8206CD18;
constexpr uint32_t kModelXpr = 56;
constexpr uint32_t kModelTextures = 80;
constexpr uint32_t kModelSkeleton = 112;
constexpr uint32_t kModelShaderPairs = 24;  // 16 bytes a pair: four blocks
constexpr uint32_t kModelShaderPairCount = 420;

// sub_825948C0 Model::Model: the vtable, the XPR, the texture provider (64
// slots), the skeleton and the five track sets made, then +16, +20, +28,
// +40, +24, +48, +404 to +420 = 0. False where the original must run.
bool ModelConstruct(uint8_t* base, uint32_t model);

// sub_82594970 Model::Release: +16, +20, each shader pair's four words and
// +24 put off (Memory::FreeLater) when not null; +16, +20, +24, +420, +28,
// +40, +48, +404 to +416 = 0; then XPR::Release on +56. Returns the
// original's r3 (XPR::Release's).
std::optional<uint32_t> ModelRelease(uint8_t* base, uint32_t model);

// sub_82594F58 Model::~Model: the vtable, Model::Release, then the members
// in reverse. Returns the original's r3 (XPR::~XPR's).
std::optional<uint32_t> ModelDestruct(uint8_t* base, uint32_t model);

}  // namespace models
