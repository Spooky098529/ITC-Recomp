#include "ui_movies/game.h"

#include "ui_movies/ui_movies.h"

#include "c_runtime/strings.h"

namespace ui_movies {

namespace {
constexpr uint32_t kColors = 0x82008410;  // 12 bytes a color: r, g, b
constexpr uint32_t kTagPic = 0x50494300;  // "PIC\0"
constexpr uint32_t kTagPrm = 0x50524D00;  // "PRM\0"
constexpr uint32_t kTagTxt = 0x54585400;  // "TXT\0"

void CopyWords(uint8_t* base, uint32_t to, uint32_t from, uint32_t first, uint32_t last) {
  for (uint32_t offset = first; offset <= last; offset += 4) {
    guest::StoreU32(base, to + offset, guest::LoadU32(base, from + offset));
  }
}
}  // namespace

uint32_t FindChild(uint8_t* base, uint32_t scene, uint32_t sub_scene, uint32_t child) {
  if (sub_scene != 0xFFFFFFFFu) {
    scene = Child(base, scene, sub_scene);
  }
  return Child(base, scene, child);
}

uint32_t FindNode(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  return FindChild(base, guest::LoadU32(base, movie + kMovieLive), sub_scene, child);
}

void SetColor(uint8_t* base, uint32_t node, uint32_t color) {
  const uint32_t from = kColors + 12 * color;
  for (uint32_t i = 0; i < 12; i += 4) {
    guest::StoreU32(base, node + 56 + i, guest::LoadU32(base, from + i));
  }
}

uint32_t Layer(uint32_t n) {
  if (n <= 11) {
    const uint32_t layer = (n + 62) * 2;
    return layer < 147 ? layer : 147;
  }
  if (n <= 43) {
    const uint32_t layer = (n + 291) * 2;
    return layer < 667 ? layer : 667;
  }
  return 667;
}

void SetLayer(uint8_t* base, uint32_t node, uint32_t n) {
  guest::StoreU32(base, node + 72, Layer(n));
}

void ClearTextIds(uint8_t* base, uint32_t movie) {
  uint32_t chunk = guest::LoadU32(base, movie + kMovieLive);
  while (guest::LoadU32(base, chunk) != kTagTrm) {
    chunk += guest::LoadU32(base, chunk + 4);
    if (guest::LoadU32(base, chunk) == kTagTxt) {
      guest::StoreU32(base, chunk + 108, 0);
    }
  }
}

uint32_t SetNodeTexture(uint8_t* base, uint32_t node, uint32_t texture) {
  const uint32_t tag = guest::LoadU32(base, node);
  if (tag == kTagPrm) {
    guest::StoreU32(base, node + 104, texture);
  } else if (tag == kTagPic) {
    guest::StoreU32(base, node + 108, texture);
  }
  return node;
}

uint32_t PictureTextureSlot(uint8_t* base, uint32_t movie, uint32_t picture) {
  const uint32_t entry = PictureTexture(picture, guest::LoadU32(base, picture + 108));
  return guest::LoadU32(base, movie + kMovieTextures) + 4 * guest::LoadU32(base, entry);
}

uint32_t SwapTexture(uint8_t* base, uint32_t movie, uint32_t i, uint32_t texture) {
  const uint32_t slot = guest::LoadU32(base, movie + kMovieTextures) + 4 * i;
  const uint32_t old = guest::LoadU32(base, slot);
  guest::StoreU32(base, slot, texture);
  return old;
}

uint32_t ResetPose(uint8_t* base, uint32_t movie) {
  RestorePose(base, movie, guest::LoadU32(base, movie + kMovieLive),
              guest::LoadU32(base, movie + kMovieFile), 0, 0);
  return RestorePose(base, movie, guest::LoadU32(base, movie + kMovieSaved),
                     guest::LoadU32(base, movie + kMovieFile), 0, 0);
}

uint32_t ResetNode(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  // The floats among +12..+80 (+36, +68) go through registers (lfs, stfs),
  // which leave their words as they were (checked by the harness's odd
  // floats): every field is copied as a word.
  if (sub_scene != 0xFFFFFFFFu) {
    const uint32_t file_sub = Child(base, guest::LoadU32(base, movie + kMovieFile), sub_scene);
    const uint32_t live_sub = Child(base, guest::LoadU32(base, movie + kMovieLive), sub_scene);
    const uint32_t saved_sub = Child(base, guest::LoadU32(base, movie + kMovieSaved), sub_scene);
    const uint32_t from = Child(base, file_sub, child);
    const uint32_t live = Child(base, live_sub, child);
    const uint32_t saved = Child(base, saved_sub, child);
    CopyWords(base, live, from, 12, 80);
    CopyWords(base, saved, from, 12, 80);
    RestorePose(base, movie, live_sub, file_sub, live, from);
    return RestorePose(base, movie, saved_sub, file_sub, saved, from);
  }
  const uint32_t from = Child(base, guest::LoadU32(base, movie + kMovieFile), child);
  const uint32_t live = Child(base, guest::LoadU32(base, movie + kMovieLive), child);
  const uint32_t saved = Child(base, guest::LoadU32(base, movie + kMovieSaved), child);
  CopyWords(base, live, from, 12, 80);
  CopyWords(base, saved, from, 12, 80);
  RestorePose(base, movie, guest::LoadU32(base, movie + kMovieLive),
              guest::LoadU32(base, movie + kMovieFile), live, from);
  return RestorePose(base, movie, guest::LoadU32(base, movie + kMovieSaved),
                     guest::LoadU32(base, movie + kMovieFile), saved, from);
}

namespace {
constexpr uint32_t kZero = 0x82000A34;  // the float 0.0

void StoreIdentity(uint8_t* base, uint32_t matrix) {
  constexpr uint32_t kOneBits = 0x3F800000;
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreU32(base, matrix + 16 * row + 4 * column, row == column ? kOneBits : 0);
    }
  }
}
}  // namespace

uint32_t MakeQuad(uint8_t* base, uint32_t quad) {
  StoreIdentity(base, quad);
  guest::StoreU32(base, quad + 64, 0);
  guest::StoreU32(base, quad + 68, 0);
  guest::StoreU32(base, quad + 72, 2);
  return quad;
}

uint32_t MakeTextItem(uint8_t* base, uint32_t item) {
  const uint32_t zero = guest::LoadU32(base, kZero);
  guest::StoreU32(base, item + 80, zero);
  guest::StoreU32(base, item, 0);
  for (uint32_t offset = 84; offset <= 96; offset += 4) {
    guest::StoreU32(base, item + offset, zero);
  }
  StoreIdentity(base, item + 16);
  return item;
}

uint32_t ClearTextItem(uint8_t* base, uint32_t item) {
  guest::StoreU32(base, item + 80, guest::LoadU32(base, kZero));
  guest::StoreU32(base, item, 0);
  StoreIdentity(base, item + 16);
  return item + 16;
}

bool CopyTextItem(uint8_t* base, uint32_t to, uint32_t from) {
  if (c_runtime::MayShare(to, 116, from, 116)) {
    return false;
  }
  guest::StoreU32(base, to, guest::LoadU32(base, from));
  CopyWords(base, to, from, 16, 112);
  return true;
}

}  // namespace ui_movies
