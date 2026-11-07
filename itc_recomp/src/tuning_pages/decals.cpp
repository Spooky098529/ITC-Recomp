// The decal images' textures and the editors' entry from the tuning pages
// (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "car_models/tga.h"
#include "car_models/vinyl.h"
#include "graphics/resources.h"
#include "guest.h"
#include "memory/heap.h"
#include "mode_tick/mode_tick.h"
#include "native/buffers.h"
#include "tuning_screen/tuning_screen.h"

namespace tuning_pages {

namespace {

constexpr uint32_t kTgaCreateImage = 0x820ED020;
constexpr uint32_t kResourceRelease = 0x822A67B0;
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kTextureFree = 0x820ECFC8;
constexpr uint32_t kTextureFromTga = 0x820ED528;

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

// VirtualFree(address, 0, MEM_RELEASE) as its adapter answers it for the
// layer's range (the native heap active: the callers' condition), else the
// guest's from `frame` with return address `lr`.
uint64_t VirtualFree(const tuning_screen::GuestCall& call, uint32_t address, uint32_t frame,
                     uint32_t lr) {
  if (memory::InRange(address)) {
    return memory::FreePages(address) ? 1 : 0;
  }
  return call(kVirtualFree, {address, 0, 0x8000}, frame, lr);
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

std::optional<uint64_t> TextureFromTga(uint8_t* base, const tuning_screen::GuestCall& call,
                                       uint32_t file, uint32_t size, uint32_t header_out,
                                       uint32_t data_out, uint32_t sp) {
  if (!native::Buffers_NativeMemcpy()) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ED528 Texture::FromTga");
  const uint32_t f = Frame(base, sp, 160);
  Store(base, f + 180, file);  // into the caller's frame, as the original
  car_models::TgaHeaderClear(base, f + 96);
  Store(base, f + 80, 0);
  if (!car_models::TgaCreateImage(base, f + 96, size, f + 180, f + 80, f)) {
    call(kTgaCreateImage, {f + 96, size, f + 180, f + 80}, f, 0x820ED568);
  }
  car_models::TextureCreate(base, car_models::kSheetFormat, guest::LoadU16(base, f + 110),
                            guest::LoadU16(base, f + 112), header_out, data_out, f);
  const uint32_t surface = graphics::GetSurfaceLevel(base, Load(base, header_out), 0);
  graphics::LockSurfaceRect(base, surface, f + 88, 0, 0);
  const uint32_t pitch = Load(base, f + 88);
  graphics::UnlockSurface(base, surface);
  call(kResourceRelease, {surface}, f, 0x820ED5B8);
  // The rows, written straight into the texture's data after the unlock.
  uint32_t to = Load(base, data_out);
  uint32_t from = Load(base, f + 80);
  for (int32_t y = 0; y < int32_t(guest::LoadU16(base, f + 112)); y++) {
    const uint32_t row = uint32_t(guest::LoadU16(base, f + 110)) << 2;
    if (!native::Buffers_Memcpy(base, to, from, row, 0x820ED5E8)) {
      call(kMemcpy, {to, from, row}, f, 0x820ED5E8);
    }
    to += pitch & ~3u;
    from += row;
  }
  VirtualFree(call, Load(base, f + 80), f, 0x820ED61C);
  // The empty sub_82106540(f + 96) left out.
  return uint64_t(f) + 96;
}

uint64_t UpdateImages(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FE628 DecalImages::Update");
  const uint32_t f = Frame(base, sp, 112);
  uint64_t r3 = images;
  for (uint32_t i = 0; i < 200; i++) {
    const uint32_t e = uint32_t(images) + 36 * i;
    if (Load(base, e + 20) == 0 || int32_t(Load(base, e + 8)) != 2) {
      continue;
    }
    const uint32_t file = Load(base, e + 12), size = Load(base, e + 16);
    if (const auto made = TextureFromTga(base, call, file, size, e + 28, e + 32, f)) {
      r3 = *made;
    } else {
      r3 = call(kTextureFromTga, {file, size, e + 28, e + 32}, f, 0x821FE670);
    }
    Store(base, e + 20, 0);
  }
  return r3;
}

uint64_t Unload(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images, uint64_t i,
                uint32_t sp, bool native_free) {
  ITC_NATIVE_RUNNING("sub_821FEA68 DecalImages::Unload");
  const uint32_t f = Frame(base, sp, 112);
  const uint32_t e = uint32_t(i * 36 + images);
  if (int32_t(Load(base, e + 8)) != 2) {
    return images;
  }
  Store(base, e + 0, 0xFFFFFFFFu);
  Store(base, e + 4, 0);
  Store(base, e + 8, 0);
  // FreeTexture's tail call Texture::Free runs from this frame, returning
  // here.
  FreeTexture(base, images, i, [&](uint32_t header_in, uint32_t data_in) -> uint64_t {
    if (native_free && car_models::TextureFreeable(base, header_in)) {
      return car_models::TextureFree(base, header_in, data_in);
    }
    return call(kTextureFree, {header_in, data_in}, f, 0x821FEAA8);
  });
  const uint32_t file = Load(base, e + 12);
  Store(base, e + 28, 0);
  Store(base, e + 32, 0);
  const uint64_t r3 = VirtualFree(call, file, f, 0x821FEAC4);
  Store(base, e + 12, 0);
  Store(base, e + 16, 0);
  Store(base, e + 20, 0);
  Store(base, e + 24, 0);
  return r3;
}

uint64_t UnloadAll(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images,
                   uint32_t sp, bool native_free) {
  ITC_NATIVE_RUNNING("sub_82208738 DecalImages::UnloadAll");
  const uint32_t f = Frame(base, sp, 112);
  uint64_t r3 = RestoreVinylMovie(base, images, f);
  r3 = RestoreStickerMovie(base, images, f);
  for (uint64_t i = 0; i < 200; i++) {
    r3 = Unload(base, call, images, i, f, native_free);
  }
  for (uint32_t k = 0; k < 200; k++) {
    const uint32_t kept = uint32_t(images) + 7236 + 8 * k;
    if (Load(base, kept) == 0 || Load(base, kept + 4) == 0) {
      continue;
    }
    if (native_free && car_models::TextureFreeable(base, kept)) {
      r3 = car_models::TextureFree(base, kept, kept + 4);
    } else {
      r3 = call(kTextureFree, {kept, kept + 4}, f, 0x822087A4);
    }
  }
  return r3;
}

}  // namespace tuning_pages
