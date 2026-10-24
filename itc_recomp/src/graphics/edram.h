// The graphics library's EDRAM allocator as plain C++
// (docs/functions/graphics-resources.md: Edram::Alloc and its helpers).
// The allocator is guest memory at [0x82953E20], made on first use: an
// 804-byte object (XMemAlloc, through the allocation layer), +0 to +27 the
// kernel's critical section in the game's, +28 the tiles free, +32 to +800
// a bitmap of 6,144 tiles (bit set = used, the most significant bit first),
// +800 = 0x55555555 (a sentinel that ends the bitmap's scans). The bases
// handed out are the game's exactly: the renderer keys targets by them
// (src/native/passes.cpp). In place of the critical section, one
// std::mutex (the user's call, 2026-10-24); the object's first 28 bytes are
// left as XMemAlloc clears them. Adapters: src/hooks/graphics_resources.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace graphics::edram {

constexpr uint32_t kAllocator = 0x82953E20;  // the allocator's pointer
constexpr uint32_t kObjectBytes = 804;
constexpr uint32_t kFree = 28;                // the tiles free
constexpr uint32_t kBitmap = 32;
constexpr uint32_t kBitmapEnd = 800;          // the sentinel
constexpr uint32_t kSentinel = 0x55555555;
constexpr uint32_t kTiles = 6144;

// sub_822B5C90 Edram::NextRun: the next free run from `position` (a tile):
// its length, and `position` moved to its start; 0 at the bitmap's end
// (`position` then left as it was).
uint32_t NextRun(uint8_t* base, uint32_t allocator, uint32_t& position);

// sub_822B5D70 Edram::Mark: the bits of tiles [start, end) set (used) or
// cleared.
void Mark(uint8_t* base, uint32_t allocator, uint32_t start, uint32_t end, bool used);

// sub_822B5EF8 Edram::Alloc: the smallest free run of at least `tiles`,
// marked used; its first tile (written to the caller's out, -1 when none
// fits), and whether one did. nullopt when the allocator couldn't be made
// (the out then not written).
struct Allocation {
  uint32_t base;  // the first tile, or 0xFFFFFFFF
  bool ok;
};
std::optional<Allocation> Alloc(uint8_t* base, uint32_t tiles);

// sub_822B5EA8 Edram::Free (not seen in play; ported so the allocator has
// one lock): the tiles [start, start + tiles) cleared and counted free.
void Free(uint8_t* base, uint32_t start, uint32_t tiles);

}  // namespace graphics::edram
