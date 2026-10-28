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

// For native callers checking before they release or destroy several
// models (nothing changed): the pending frees' nodes ModelRelease (and
// ModelDestruct) may add for `model` (each block not null, and the XPR's
// two), and whether ModelDestruct's members would all run natively (its
// pending frees counted apart: the caller checks the sum).
uint32_t ModelReleaseNodes(uint8_t* base, uint32_t model);
bool ModelMembersDestructible(uint8_t* base, uint32_t model);

// --- The materials and the shared shaders (accessors) ---

// Model::MaterialCount (sub_82594AD8): the shape's (544 bytes each from the
// model's +40) material block (+396): its +8 for group 0, else +12.
uint32_t MaterialCount(uint8_t* base, uint32_t model, uint32_t shape, int32_t group);
// Model::Material (sub_82594B20): the block + 16 + 512 x index, past group
// 0's materials (the block's +8) for any other group.
uint32_t Material(uint8_t* base, uint32_t model, uint32_t shape, uint32_t index, int32_t group);
// sub_825896D8 and sub_82589700: the shared vertex and pixel shaders of pair
// `pair` ([0x8296B7B4] and [0x8296B7B8] + 4 x pair), 0 when the table is
// unset.
uint32_t SharedVertexShader(uint8_t* base, uint32_t pair);
uint32_t SharedPixelShader(uint8_t* base, uint32_t pair);
// sub_82589728: the vertex declaration for three stream types, from the
// tables at 0x8265EC00 and 0x8296B7F4. nullopt for a type pair the tables
// don't cover, where the original prints a message (never seen in play)
// and returns the table's +196: left to it.
std::optional<uint32_t> VertexDeclaration(uint8_t* base, uint32_t a, int32_t b, uint32_t c);

}  // namespace models
