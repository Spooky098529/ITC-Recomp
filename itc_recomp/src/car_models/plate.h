// The two 300 x 150 textures a car model composes on the CPU from five of
// the scene's TGA images (probably its license plate), as plain C++ (B7 in
// docs/rexglue-removal.md; docs/functions/car-models.md): sub_820ED6D0 with
// the image lookup sub_820C4DD8 and the rectangle copy sub_820ED630. Guest
// data shared with the recompiled code, so these work on guest memory with
// the originals' loads and stores in their order, their frames' words
// included. Adapters: src/hooks/car_models.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace car_models {

// sub_820ED6D0's and sub_820ED630's stack frames.
constexpr uint32_t kPlateFrameBytes = 944;
constexpr uint32_t kCopyRectFrameBytes = 144;

// The image lookup's table of first images, by kind: 0, 4, 7, 11, 15.
// sub_820C4DD8: j = that (0 for kinds 5 and up) + `index`; memcpy of the
// 20-byte TGA header at `set` + 3732 + 20j to `header_out` (from its own
// frame, native::Buffers_Memcpy); [`pixels_out`] = [`set` + 3656 + 4j], the
// image's pixels. Returns memcpy's r3 (`header_out`). Only where
// native::Buffers_NativeMemcpy() (the callers check).
uint32_t PlateImage(uint8_t* base, uint32_t set, uint32_t kind, uint32_t index,
                    uint32_t header_out, uint32_t pixels_out);

// sub_820ED630 (a rectangle copy of 32-bit pixels): its arguments as the
// original takes them, small structs in 64-bit registers (high word first)
// and a 16-byte one on the stack: `dst` and `src` the images, `r4` (x, y)
// in the destination, `r5` (width, height), `r6` (the destination's pitch in
// pixels, unused), `r7` unused, `r9` (x, y) in the source, `r10` unused, and
// the stack's first word ([`stack` + 96]) the source's pitch. The registers
// spilled into the caller's frame (`stack` + 32 to + 95, all but r8's), read
// back from there, then `height` rows of 4 x `width` bytes copied with
// memcpy (native::Buffers_Memcpy). `stack` is the guest r1 at entry.
void CopyRect(uint8_t* base, uint32_t dst, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7,
              uint32_t src, uint64_t r9, uint64_t r10, uint32_t stack);

// sub_820ED630's guest call: Resource::Release (sub_822A67B0) on the first
// texture's level 0 surface, from sub_820ED6D0's frame.
using ReleaseCall = std::function<void(uint32_t surface)>;

// sub_820ED6D0: r3 to r6 where the two textures' headers and data go (A:
// `header_a`, `data_a`; B: `header_b`, `data_b`), `regs` r7 to r10: with the
// 16 bytes at `stack` + 80 a 48-byte struct passed by value (its words W0 to
// W11: W1 a number looked up in the table at 0x8200664C, W2 to W11 the cells
// to draw, 38 or 37 for none). The registers spilled into the caller's frame
// (`stack` + 48), the five images' headers cleared and fetched (PlateImage
// kinds 0 to 4 into the frame), the slots' rectangles (the 16-byte table at
// 0x82602AF0, their sizes from the images) into the frame, both textures
// made (Texture::Create, A8R8G8B8, image 1's size), the first one's surface
// locked for its pitch, unlocked and released, image 1 copied into both,
// then for each slot its cell copied in (CopyRect): slot 0 image 0 into B
// only, slot 1 image 3's cell for W1, slots 2 to 4 image 4's cell W2 to W4,
// slots 5 to 11 image 2's cell W5 to W11 (+2 from 10); the headers'
// destructors (empty). `stack` is the guest r1 at entry (the frame is `stack`
// - 944). nullopt, with nothing done, where memcpy can't be native.
std::optional<uint32_t> MakePlate(uint8_t* base, uint32_t header_a, uint32_t data_a,
                                  uint32_t header_b, uint32_t data_b, const uint64_t regs[4],
                                  uint32_t stack, const ReleaseCall& release);

}  // namespace car_models
