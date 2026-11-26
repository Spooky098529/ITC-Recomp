// The vertex and index data draws read, in one device-local buffer
// (docs/pc-renderer.md, step 1): the ranges draws use, copied from the
// game's physical memory the first time they're drawn and again after the
// game writes them (memcpy's records, taken at each frame's start); the
// frame's inline vertices; and the built-in index lists for rectangle and
// quad lists. It replaces the guest memory mirrors (two host-visible copies
// of the whole 512 MB physical memory, pinned in system RAM by the driver).
//
// The translated shaders read every vertex stream from one storage buffer
// by the address in its fetch constant: a draw gives each stream it reads
// the arena offset of its range in place of the address (Place), and binds
// its index buffer at the offset of its range.
//
// Render thread only.

#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "device_context.h"
#include "vulkan_functions.h"

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

  bool Init(const DeviceContext& context, uint32_t frames_in_flight);

  // A frame's recording starts in slot `frame` (its fence waited): its
  // staging is free again, and memcpy's writes since the last frame mark
  // the ranges they overlap to be copied again.
  void BeginFrame(uint32_t frame);

  // The arena offset of the physical range [address, address + size),
  // copied in if it's new this way or was written since; UINT32_MAX if
  // there's no room (the draw is left out, counted).
  uint32_t Place(uint32_t address, uint32_t size);
  // The frame's inline vertices: copied into this frame's part of the
  // arena; UINT32_MAX if it's full.
  uint32_t PlaceInline(const void* data, uint32_t size);

  VkBuffer buffer() const;
  // This frame's copies, ended, to be submitted before its drawing; null if
  // there were none.
  VkCommandBuffer EndUploads();

  // "arena 41.2 of 192 MB in 1503 ranges; this frame ... " for the log, and
  // the counts reset.
  std::string TakeStats();

 private:
  struct State;
  std::unique_ptr<State> state_;
};

}  // namespace native
