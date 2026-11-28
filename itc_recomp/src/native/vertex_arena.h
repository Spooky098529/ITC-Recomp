// The vertex and index data draws read, in one device-local buffer
// (docs/pc-renderer.md, step 1): the ranges draws use, copied from the
// game's physical memory the first time they're drawn and again after the
// game writes them (memcpy's records, taken at each frame's start); the
// frame's inline vertices; and the built-in index lists for rectangle and
// quad lists. It replaces the guest memory mirrors (two host-visible copies
// of the whole 512 MB physical memory, pinned in system RAM by the driver).
//
// A draw binds each vertex stream it reads at the arena offset of its range
// (Place) as a vertex buffer, and its index buffer at the offset of its
// range.
//
// Render thread only.

#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "gpu/types.h"

namespace gpu {
class Backend;
}

namespace native {

// The built-in index lists in the arena (32-bit indices): rectangle lists
// as two-triangle strips (rectangle << 2 | corner, a restart between
// rectangles), quad lists as triangle lists.
constexpr uint32_t RectangleIndexCount(uint32_t rectangles) {
  return 4 * rectangles + ((rectangles > 1 ? rectangles : 1) - 1);
}

class VertexArena {
 public:
  VertexArena();
  ~VertexArena();

  // The built-in index lists' places (below).
  static constexpr uint32_t kBuiltinPrimitives = 4096;
  static constexpr uint32_t kRectangleIndices = 0;
  static constexpr uint32_t kQuadIndices = (4 * RectangleIndexCount(kBuiltinPrimitives) + 255) & ~255u;

  bool Init(gpu::Backend& backend, uint32_t frames_in_flight);

  // A frame's recording starts in slot `frame` (its fence waited): its
  // staging is free again, and memcpy's writes since the last frame mark
  // the ranges they overlap to be copied again.
  void BeginFrame(uint32_t frame);

  // The arena offset of the physical range [address, address + size),
  // copied in if it's new this way or was written since; UINT32_MAX if
  // there's no room (the draw is left out, counted). endian: the guest
  // data's (xenos::Endian: 0 none, 1 8-in-16, 2 8-in-32, 3 16-in-32; a
  // vertex stream's from its fetch constant, an index buffer's from its
  // size), swapped as it's copied so the arena holds the host's byte order
  // for the vertex input to fetch by (each endian mode a range of its own).
  uint32_t Place(uint32_t address, uint32_t size, uint32_t endian);
  // The frame's inline vertices: copied into this frame's part of the
  // arena, swapped from `endian` as Place does; UINT32_MAX if it's full.
  uint32_t PlaceInline(const void* data, uint32_t size, uint32_t endian);

  // A buffer held by its object (VertexArena_GpuOnlyModel, below): the arena
  // offset of `address` (the GPU's) in it, as `endian` has it; UINT32_MAX if
  // the object holds no buffer there (the draw places it by address).
  uint32_t HeldBuffer(uint32_t object, uint32_t address, uint32_t endian) const;

  gpu::Buffer* object() const;  // the buffer's
  // This frame's copies are all asked for (the backend submits them before
  // its drawing): the next frame's go into its own slot's staging.
  void EndUploads();

  // "arena 41.2 of 192 MB in 1503 ranges; this frame ... " for the log, and
  // the counts reset.
  std::string TakeStats();

 private:
  struct State;
  std::unique_ptr<State> state_;
};

// Buffers held by their objects (docs/pc-renderer.md, step 2b): a model
// whose vertex data only the GPU reads (the course's and the scenery lists'
// models) has its buffers (models::ModelBuffers) copied into the arena once,
// at the next frame's start, in the host's byte order, and its data's pages
// given back; draws bind them by their objects from then on. They go when
// the model's data block is freed (memory::FreePhysical). Any thread, after
// Model::Load.
void VertexArena_GpuOnlyModel(uint32_t model);

}  // namespace native
