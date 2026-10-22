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

namespace {
constexpr uint32_t kTagGrp = 0x47525000;  // "GRP\0"
constexpr uint32_t kTagPic = 0x50494300;  // "PIC\0"
constexpr uint32_t kTagPrm = 0x50524D00;  // "PRM\0"
constexpr uint32_t kTagTfm = 0x54464D00;  // "TFM\0"
constexpr uint32_t kTagTxt = 0x54585400;  // "TXT\0"

void CopyWords(uint8_t* base, uint32_t to, uint32_t from, uint32_t first, uint32_t last) {
  for (uint32_t offset = first; offset <= last; offset += 4) {
    guest::StoreU32(base, to + offset, guest::LoadU32(base, from + offset));
  }
}
}  // namespace

uint32_t RestorePose(uint8_t* base, uint32_t movie, uint32_t live, uint32_t rest, uint32_t group,
                     uint32_t rest_group) {
  const uint32_t range = group != 0 && rest_group != 0 ? group : live;
  uint32_t result = movie;
  // The floats copied through registers (lfs, stfs) are stored unchanged by
  // the translated code, so every field is copied as a word.
  for (int32_t i = int32_t(guest::LoadU32(base, range + 100) + guest::LoadU32(base, range + 104) - 1);
       i >= int32_t(guest::LoadU32(base, range + 100)); i--) {
    const uint32_t to = Child(base, live, uint32_t(i));
    const uint32_t from = Child(base, rest, uint32_t(i));
    result = from;
    const uint32_t tag = guest::LoadU32(base, to);
    if (tag != kTagGrp && tag != kTagPic && tag != kTagPrm && tag != kTagSce && tag != kTagTfm &&
        tag != kTagTxt) {
      continue;
    }
    CopyWords(base, to, from, 12, 80);
    result = from + 28;
    switch (tag) {
      case kTagGrp:
        result = RestorePose(base, movie, live, rest, to, from);
        break;
      case kTagSce:
        result = RestorePose(base, movie, to, from, 0, 0);
        break;
      case kTagPic:
        CopyWords(base, to, from, 100, 136);
        break;
      case kTagTxt:
        CopyWords(base, to, from, 100, 104);
        CopyWords(base, to, from, 116, 136);
        break;
      case kTagPrm:
        CopyWords(base, to, from, 100, 108);
        for (uint32_t k = 0; k < guest::LoadU32(base, from + 116); k++) {
          const uint32_t vertex_to = PrimitiveVertex(base, to, k);
          const uint32_t vertex_from = PrimitiveVertex(base, from, k);
          CopyWords(base, vertex_to, vertex_from, 0, 28);
          result = vertex_from;
        }
        break;
      default:  // TFM: the common fields only
        break;
    }
  }
  return result;
}

}  // namespace ui_movies
