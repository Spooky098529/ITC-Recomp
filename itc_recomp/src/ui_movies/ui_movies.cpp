#include "ui_movies/ui_movies.h"

namespace ui_movies {

namespace {
constexpr uint32_t kOne = 0x82000A30;  // the float 1.0
}  // namespace

uint32_t FindChunk(uint8_t* base, uint32_t chunk, uint32_t tag) {
  for (;;) {
    chunk += guest::LoadU32(base, chunk + 4);
    const uint32_t found = guest::LoadU32(base, chunk);
    if (found == kTagTrm) {
      return tag == kTagTrm ? chunk : 0;
    }
    if (found == tag) {
      return chunk;
    }
  }
}

uint32_t FileScene(uint8_t* base, uint32_t file) {
  const uint32_t scene = file + guest::LoadU32(base, file + 4);
  return guest::LoadU32(base, scene) == kTagSce ? scene : 0;
}

uint32_t XmdEntry(uint8_t* base, uint32_t file, uint32_t i) {
  if (i >= guest::LoadU32(base, file + 12)) {
    return 0;
  }
  return file + guest::LoadU32(base, file + 4 * (i + 4));
}

void SetRenderer(uint8_t* base, uint32_t renderer, uint32_t list) {
  guest::StoreU32(base, kRenderer, renderer);
  guest::StoreU32(base, kCommandList, list);
}

void ResetDrawContext(uint8_t* base, uint32_t context) {
  // The original stores the color's float in each row's w first, then
  // copies the identity over the matrix (memcpy from its stack): only the
  // identity is left.
  constexpr uint32_t kOneBits = 0x3F800000;
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreU32(base, context + 16 * row + 4 * column, row == column ? kOneBits : 0);
    }
  }
  const uint32_t one = guest::LoadU32(base, kOne);
  for (uint32_t offset = 80; offset <= 92; offset += 4) {
    guest::StoreU32(base, context + offset, one);
  }
  for (uint32_t offset : {64u, 68u, 72u, 76u, 96u, 100u, 104u, 108u}) {
    guest::StoreU32(base, context + offset, 0);
  }
}

}  // namespace ui_movies
