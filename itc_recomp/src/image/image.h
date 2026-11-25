// The game's image data compiled in (docs/e5-native.md, step 8): default.xex's
// .rdata and .data sections (the constants, strings, tables and initialized
// globals the code reads at 0x82000000 on), written as C++ data by
// itc_recomp/tools/image_data.py on the developer's machine. default.xex is
// no longer read.

#pragma once

#include <cstddef>
#include <cstdint>

namespace image {

// The image's address and size, and the main thread's stack size (its XEX
// header's default).
inline constexpr uint32_t kLoadAddress = 0x82000000;
inline constexpr uint32_t kSize = 0x9B0000;
inline constexpr uint32_t kStackSize = 0x40000;

// A run of the image's words (big-endian values) from `address` on.
struct Chunk {
  uint32_t address;
  uint32_t words;
  const uint32_t* data;
};

extern const Chunk kRdataChunks[];
extern const size_t kRdataChunkCount;
extern const Chunk kDataChunks[];
extern const size_t kDataChunkCount;

// Writes the image's data into guest memory at kLoadAddress (zeroed before).
void CopyInto();

}  // namespace image
