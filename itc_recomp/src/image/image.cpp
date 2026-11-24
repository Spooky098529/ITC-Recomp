// See image.h.

#include "image/image.h"

#include "base/log.h"
#include "guest.h"

namespace image {

namespace {

size_t Copy(uint8_t* base, const Chunk* chunks, size_t count) {
  size_t bytes = 0;
  for (size_t i = 0; i < count; i++) {
    const Chunk& chunk = chunks[i];
    for (uint32_t w = 0; w < chunk.words; w++) {
      guest::StoreU32(base, chunk.address + 4 * w, chunk.data[w]);
    }
    bytes += 4 * size_t(chunk.words);
  }
  return bytes;
}

}  // namespace

void CopyInto(uint8_t* base) {
  const size_t bytes =
      Copy(base, kRdataChunks, kRdataChunkCount) + Copy(base, kDataChunks, kDataChunkCount);
  ITC_LOG_INFO("image: {} bytes of data compiled in, at {:08X}", bytes, kLoadAddress);
}

}  // namespace image
