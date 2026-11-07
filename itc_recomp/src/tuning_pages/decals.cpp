// The decal images' textures and the editors' entry from the tuning pages
// (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "tuning_screen/tuning_screen.h"

namespace tuning_pages {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

// Movie `movie_word`'s texture n's entry +0 = `value` (the table [movie+28]
// read for it).
void SetMovieTexture(uint8_t* base, uint32_t movie, uint32_t n, uint32_t value) {
  const uint32_t table = Load(base, movie + 28);
  Store(base, Load(base, table + n * 4), value);
}

// A movie texture restored from a placeholder word: movie `id`'s texture n's
// +0 = [images + placeholder]. r3 the movie's entry.
uint32_t RestoreTexture(uint8_t* base, uint32_t images, uint32_t id, uint32_t placeholder,
                        uint32_t n, uint32_t frame) {
  const uint32_t entry = tuning_screen::MovieEntry(base, frame, id);
  const uint32_t value = Load(base, images + placeholder);
  SetMovieTexture(base, Load(base, entry), n, value);
  return entry;
}

constexpr uint32_t kFadeIn = 0x826A50DC;   // layer 0's fade-in count
constexpr uint32_t kFadeOut = 0x826A50E4;  // and fade-out count
constexpr uint32_t kFadePasses = 0x82001BEC;

}  // namespace

uint64_t FreeTexture(uint8_t* base, uint64_t images, uint64_t i, const TextureFreeFn& free) {
  ITC_NATIVE_RUNNING("sub_821FE6A0 DecalImages::FreeTexture");
  const uint64_t entry = i * 36 + images;
  const uint32_t r3 = uint32_t(images);
  const uint32_t texture = Load(base, uint32_t(entry) + 28);
  bool shared = false;
  for (uint32_t k = 0; k < 200; k++) {
    if (Load(base, r3 + 7236 + 8 * k) == texture) {
      shared = true;
    }
  }
  const int32_t index = int32_t(uint32_t(i));
  if (index >= 33 && index <= 50) {
    for (int32_t j = 33; j <= 50; j++) {
      if (j != index && Load(base, r3 + 36 * uint32_t(j) + 28) == texture) {
        shared = true;
      }
    }
  }
  if (shared) {
    return entry + 28;
  }
  return free(uint32_t(entry) + 28, uint32_t(entry) + 32);
}

uint64_t RestoreVinylTexture(uint8_t* base, uint64_t images, uint32_t w, uint32_t n,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82204698 DecalImages::RestoreVinylTexture");
  const uint32_t frame = Frame(base, sp, 112);
  if (int32_t(w) == 0) {
    return RestoreTexture(base, uint32_t(images), 13, 8836, n, frame);
  }
  if (int32_t(w) == 1) {
    return RestoreTexture(base, uint32_t(images), 13, 8840, n, frame);
  }
  return images;
}

uint64_t RestoreStickerTexture(uint8_t* base, uint64_t images, uint32_t w, uint32_t n,
                               uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F89B8 DecalImages::RestoreStickerTexture");
  const uint32_t frame = Frame(base, sp, 112);
  if (int32_t(w) == 2) {
    return RestoreTexture(base, uint32_t(images), 14, 8844, n, frame);
  }
  if (int32_t(w) == 3) {
    return RestoreTexture(base, uint32_t(images), 14, 8848, n, frame);
  }
  return images;
}

uint32_t BindVinylTexture(uint8_t* base, uint64_t images, uint32_t i, uint32_t group,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82204730 DecalImages::BindVinylTexture");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = tuning_screen::MovieEntry(base, frame, 13);
  const uint32_t movie = Load(base, entry);
  const int32_t index = int32_t(i);
  int32_t n = -1;
  if (int32_t(group) == 0) {
    if (index == 0) n = 181;
    else if (index == 1) n = 183;
    else if (index == 2) n = 184;
    else if (index >= 6 && index <= 8) n = index + 185;
    else if (index >= 14 && index <= 19) n = index + 154;
  } else if (int32_t(group) == 1) {
    if (index == 21) n = 175;
    else if (index == 22) n = 177;
    else if (index == 23) n = 178;
    else if (index >= 27 && index <= 29) n = index + 167;
    else if (index >= 35 && index <= 40) n = index + 125;
  }
  if (n >= 0) {
    const uint32_t texture = Load(base, uint32_t(uint64_t(i) * 36 + images) + 28);
    SetMovieTexture(base, movie, uint32_t(n), texture);
  }
  return entry;
}

uint64_t BindVinylTextures(uint8_t* base, uint64_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82203D40 TuningScreen::BindVinylTextures");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 112);
  if (int32_t(Load(base, s + 9656)) != 5 || guest::LoadU8(base, s + 9660) != 6) {
    return screen;
  }
  const uint32_t entry = tuning_screen::MovieEntry(base, frame, 13);
  const uint32_t movie = Load(base, entry);
  // {entry, texture} in the original's order.
  static constexpr uint8_t kPairs[24][2] = {
      {0, 181},  {1, 183},  {2, 184},  {14, 168}, {15, 169}, {16, 170}, {17, 171}, {18, 172},
      {19, 173}, {6, 191},  {7, 192},  {8, 193},  {21, 175}, {22, 177}, {23, 178}, {35, 160},
      {36, 161}, {37, 162}, {38, 163}, {39, 164}, {40, 165}, {27, 194}, {28, 195}, {29, 196}};
  for (const auto& pair : kPairs) {
    SetMovieTexture(base, movie, pair[1], Load(base, s + 10048 + 36 * pair[0] + 28));
  }
  return entry;
}

uint64_t RestoreMovieTexture(uint8_t* base, uint64_t images, uint32_t w, uint32_t n,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822081A0 DecalImages::RestoreMovieTexture");
  const uint32_t frame = Frame(base, sp, 112);
  RestoreVinylTexture(base, images, w, n, frame);
  return RestoreStickerTexture(base, images, w, n, frame);
}

uint64_t RestoreVinylMovie(uint8_t* base, uint64_t images, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822081D8 DecalImages::RestoreVinylMovie");
  // The textures BindVinylTextures replaces: group 0's back to the
  // placeholder +8836 (w 0), group 1's to +8840 (w 1), in the original's
  // order, each followed by RestoreStickerTexture (nothing for w 0 and 1 but
  // its frame).
  static constexpr uint8_t kGroup0[12] = {181, 183, 184, 191, 192, 193,
                                          168, 169, 170, 171, 172, 173};
  static constexpr uint8_t kGroup1[12] = {175, 177, 178, 194, 195, 196,
                                          160, 161, 162, 163, 164, 165};
  const uint32_t frame = Frame(base, sp, 128);
  uint64_t r3 = images;
  for (uint32_t w = 0; w < 2; w++) {
    for (const uint8_t n : w == 0 ? kGroup0 : kGroup1) {
      RestoreTexture(base, uint32_t(images), 13, w == 0 ? 8836 : 8840, n, frame);
      r3 = RestoreStickerTexture(base, images, w, n, frame);
    }
  }
  return r3;
}

uint64_t RestoreStickerMovie(uint8_t* base, uint64_t images, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F6DD8 DecalImages::RestoreStickerMovie");
  static constexpr uint8_t kPlaceholder3[16] = {159, 161, 163, 165, 167, 169, 173, 176,
                                                178, 180, 182, 190, 192, 194, 196, 198};
  static constexpr uint8_t kPlaceholder2[16] = {160, 162, 164, 166, 168, 170, 174, 177,
                                                179, 181, 183, 191, 193, 195, 197, 199};
  const uint32_t frame = Frame(base, sp, 96);
  uint64_t r3 = images;
  for (const uint8_t n : kPlaceholder3) {
    r3 = RestoreMovieTexture(base, images, 3, n, frame);
  }
  for (const uint8_t n : kPlaceholder2) {
    r3 = RestoreMovieTexture(base, images, 2, n, frame);
  }
  return r3;
}

uint64_t EnterEditor(uint8_t* base, uint64_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D84F8 TuningScreen::EnterEditor");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 96);
  if ((Load(base, kFadeOut) | Load(base, kFadeIn)) != 0) {
    return screen;
  }
  if (AllLoaded(base, s + 10048) == 0) {
    return 0;
  }
  const uint32_t passes = Load(base, kFadePasses);
  mode_tick::ScreenFadesFadeIn(base, passes, 0, 0, 0, 0);
  if (int32_t(Load(base, s + 9656)) != 5) {
    return passes;
  }
  const uint32_t item = guest::LoadU8(base, s + 9660);
  if (item != 6 && item != 7) {
    return passes;
  }
  BindVinylTextures(base, screen, frame);
  KeepListTextures(base, s);
  // Push(4): the history's word at the byte +10008, then the byte stepped.
  Store(base, (guest::LoadU8(base, s + 10008) + 2439) * 4 + s, 4);
  const uint8_t depth = uint8_t(guest::LoadU8(base, s + 10008) + 1);
  uint32_t queue = Load(base, s + 10036);
  const uint32_t state = Load(base, s + 9652);
  guest::StoreU8(base, s + 10008, depth);
  Store(base, queue + 20, 0xFFFFFFFFu);
  Store(base, queue + 12, 0xFFFFFFFFu);
  Store(base, queue + 24, 0);
  Store(base, queue + 4, state);
  Store(base, queue + 0, state);
  Store(base, queue + 8, state);
  const bool vinyls =
      int32_t(Load(base, s + 9656)) == 5 && guest::LoadU8(base, s + 9660) == 6;
  const uint32_t next = vinyls ? 23 : 38;
  queue = Load(base, s + 10036);
  Store(base, s + 9652, next);
  const uint32_t current = Load(base, queue + 0);
  if (int32_t(current) != int32_t(next)) {
    Store(base, queue + 8, current);
  }
  Store(base, queue + 16, 0xFFFFFFFFu);
  Store(base, queue + 4, next);
  Store(base, queue + 0, 0xFFFFFFFFu);
  Store(base, queue + 12, vinyls ? 15 : 32);
  if (vinyls) {
    SetUpVinylBusyIcon(base, s);
  }
  return screen;
}

}  // namespace tuning_pages
