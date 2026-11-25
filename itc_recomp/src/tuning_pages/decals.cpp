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
#include "schema/tuning_screen.h"
#include "tuning_screen/tuning_screen.h"

namespace tuning_pages {

namespace {

// The screen (tuning-screen.toml's TuningScreen).
schema::TuningScreen Screen(uint32_t s) { return schema::TuningScreen(s); }
// The screen's command queue (tuning-screen.toml's TuningCommandQueue).
schema::TuningCommandQueue CommandQueue(uint32_t q) {
  return schema::TuningCommandQueue(q);
}

using G = schema::TuningPagesGlobals;

constexpr uint32_t kTgaCreateImage = 0x820ED020;
constexpr uint32_t kResourceRelease = 0x822A67B0;
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kTextureFree = 0x820ECFC8;
constexpr uint32_t kTextureFromTga = 0x820ED528;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  Store(sp - bytes, sp);
  return sp - bytes;
}

// Movie `movie_word`'s texture n's entry +0 = `value` (the table [movie+28]
// read for it).
void SetMovieTexture(uint32_t movie, uint32_t n, uint32_t value) {
  MovieTexture(movie, n).set_texture(value);
}

// A movie texture restored from a placeholder word: movie `id`'s texture n's
// +0 = the decal images' placeholder w (0 and 1 movie 13's, 2 and 3 movie
// 14's). r3 the movie's entry.
uint32_t Placeholder(uint32_t images, uint32_t w) {
  const schema::DecalImages im(images);
  switch (w) {
    case 0: return im.vinyl_placeholder_0();
    case 1: return im.vinyl_placeholder_1();
    case 2: return im.sticker_placeholder_0();
    default: return im.sticker_placeholder_1();
  }
}
uint32_t RestoreTexture(uint32_t images, uint32_t id, uint32_t w, uint32_t n,
                        uint32_t frame) {
  const uint32_t entry = tuning_screen::MovieEntry(frame, id);
  const uint32_t value = Placeholder(images, w);
  SetMovieTexture(Load(entry), n, value);
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

}  // namespace

uint64_t FreeTexture(uint64_t images, uint64_t i, const TextureFreeFn& free) {
  ITC_NATIVE_RUNNING("sub_821FE6A0 DecalImages::FreeTexture");
  const uint64_t entry = i * 36 + images;
  const uint32_t r3 = uint32_t(images);
  const uint32_t texture = DecalEntryAt(images, i).texture_header();
  bool shared = false;
  for (uint32_t k = 0; k < 200; k++) {
    if (DecalKeptAt(r3, k).texture_header() == texture) {
      shared = true;
    }
  }
  const int32_t index = int32_t(uint32_t(i));
  if (index >= 33 && index <= 50) {
    for (int32_t j = 33; j <= 50; j++) {
      if (j != index && DecalEntryAt(r3, uint32_t(j)).texture_header() == texture) {
        shared = true;
      }
    }
  }
  const schema::DecalImageEntry e = DecalEntryAt(images, i);
  if (shared) {
    return entry + 28;  // the texture header's address, all 64 bits (r3 as the original's)
  }
  return free(e.texture_header_address(), e.texture_data_address());
}

uint64_t RestoreVinylTexture(uint64_t images, uint32_t w, uint32_t n,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82204698 DecalImages::RestoreVinylTexture");
  const uint32_t frame = Frame(sp, 112);
  if (int32_t(w) == 0) {
    return RestoreTexture(uint32_t(images), 13, 0, n, frame);
  }
  if (int32_t(w) == 1) {
    return RestoreTexture(uint32_t(images), 13, 1, n, frame);
  }
  return images;
}

uint64_t RestoreStickerTexture(uint64_t images, uint32_t w, uint32_t n,
                               uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F89B8 DecalImages::RestoreStickerTexture");
  const uint32_t frame = Frame(sp, 112);
  if (int32_t(w) == 2) {
    return RestoreTexture(uint32_t(images), 14, 2, n, frame);
  }
  if (int32_t(w) == 3) {
    return RestoreTexture(uint32_t(images), 14, 3, n, frame);
  }
  return images;
}

uint32_t BindVinylTexture(uint64_t images, uint32_t i, uint32_t group,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82204730 DecalImages::BindVinylTexture");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(entry);
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
    const uint32_t texture = DecalEntryAt(images, i).texture_header();
    SetMovieTexture(movie, uint32_t(n), texture);
  }
  return entry;
}

uint64_t BindVinylTextures(uint64_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82203D40 TuningScreen::BindVinylTextures");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 112);
  if (int32_t(Screen(s).category()) != 5 || Screen(s).item() != 6) {
    return screen;
  }
  const uint32_t entry = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(entry);
  // {entry, texture} in the original's order.
  static constexpr uint8_t kPairs[24][2] = {
      {0, 181},  {1, 183},  {2, 184},  {14, 168}, {15, 169}, {16, 170}, {17, 171}, {18, 172},
      {19, 173}, {6, 191},  {7, 192},  {8, 193},  {21, 175}, {22, 177}, {23, 178}, {35, 160},
      {36, 161}, {37, 162}, {38, 163}, {39, 164}, {40, 165}, {27, 194}, {28, 195}, {29, 196}};
  for (const auto& pair : kPairs) {
    SetMovieTexture(movie, pair[1],
                    DecalEntryAt(Screen(s).decal_images_address(), pair[0]).texture_header());
  }
  return entry;
}

uint64_t RestoreMovieTexture(uint64_t images, uint32_t w, uint32_t n,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822081A0 DecalImages::RestoreMovieTexture");
  const uint32_t frame = Frame(sp, 112);
  RestoreVinylTexture(images, w, n, frame);
  return RestoreStickerTexture(images, w, n, frame);
}

uint64_t RestoreVinylMovie(uint64_t images, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822081D8 DecalImages::RestoreVinylMovie");
  // The textures BindVinylTextures replaces: group 0's back to the
  // placeholder +8836 (w 0), group 1's to +8840 (w 1), in the original's
  // order, each followed by RestoreStickerTexture (nothing for w 0 and 1 but
  // its frame).
  static constexpr uint8_t kGroup0[12] = {181, 183, 184, 191, 192, 193,
                                          168, 169, 170, 171, 172, 173};
  static constexpr uint8_t kGroup1[12] = {175, 177, 178, 194, 195, 196,
                                          160, 161, 162, 163, 164, 165};
  const uint32_t frame = Frame(sp, 128);
  uint64_t r3 = images;
  for (uint32_t w = 0; w < 2; w++) {
    for (const uint8_t n : w == 0 ? kGroup0 : kGroup1) {
      RestoreTexture(uint32_t(images), 13, w, n, frame);
      r3 = RestoreStickerTexture(images, w, n, frame);
    }
  }
  return r3;
}

uint64_t RestoreStickerMovie(uint64_t images, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F6DD8 DecalImages::RestoreStickerMovie");
  static constexpr uint8_t kPlaceholder3[16] = {159, 161, 163, 165, 167, 169, 173, 176,
                                                178, 180, 182, 190, 192, 194, 196, 198};
  static constexpr uint8_t kPlaceholder2[16] = {160, 162, 164, 166, 168, 170, 174, 177,
                                                179, 181, 183, 191, 193, 195, 197, 199};
  const uint32_t frame = Frame(sp, 96);
  uint64_t r3 = images;
  for (const uint8_t n : kPlaceholder3) {
    r3 = RestoreMovieTexture(images, 3, n, frame);
  }
  for (const uint8_t n : kPlaceholder2) {
    r3 = RestoreMovieTexture(images, 2, n, frame);
  }
  return r3;
}

uint64_t EnterEditor(uint64_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D84F8 TuningScreen::EnterEditor");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 96);
  if ((G::fade_out_counts() | G::fade_in_counts()) != 0) {
    return screen;
  }
  if (AllLoaded(Screen(s).decal_images_address()) == 0) {
    return 0;
  }
  const uint32_t passes = G::fade_in_passes();
  mode_tick::ScreenFadesFadeIn(passes, 0, 0, 0, 0);
  if (int32_t(Screen(s).category()) != 5) {
    return passes;
  }
  const uint32_t item = Screen(s).item();
  if (item != 6 && item != 7) {
    return passes;
  }
  BindVinylTextures(screen, frame);
  KeepListTextures(s);
  // Push(4): the history's word at the byte +10008, then the byte stepped.
  Store((Screen(s).history_depth() + 2439) * 4 + s, 4);
  const uint8_t depth = uint8_t(Screen(s).history_depth() + 1);
  uint32_t queue = Screen(s).queue().address;
  const uint32_t state = Screen(s).state();
  Screen(s).set_history_depth(depth);
  CommandQueue(queue).set_step(0xFFFFFFFFu);
  CommandQueue(queue).set_current(0xFFFFFFFFu);
  CommandQueue(queue).set_started(0);
  CommandQueue(queue).set_resume(state);
  CommandQueue(queue).set_page(state);
  CommandQueue(queue).set_previous_page(state);
  const bool vinyls =
      int32_t(Screen(s).category()) == 5 && Screen(s).item() == 6;
  const uint32_t next = vinyls ? 23 : 38;
  queue = Screen(s).queue().address;
  Screen(s).set_state(next);
  const uint32_t current = CommandQueue(queue).page();
  if (int32_t(current) != int32_t(next)) {
    CommandQueue(queue).set_previous_page(current);
  }
  CommandQueue(queue).set_next(0xFFFFFFFFu);
  CommandQueue(queue).set_resume(next);
  CommandQueue(queue).set_page(0xFFFFFFFFu);
  CommandQueue(queue).set_current(vinyls ? 15 : 32);
  if (vinyls) {
    SetUpVinylBusyIcon(s);
  }
  return screen;
}

std::optional<uint64_t> TextureFromTga(const tuning_screen::GuestCall& call,
                                       uint32_t file, uint32_t size, uint32_t header_out,
                                       uint32_t data_out, uint32_t sp) {
  if (!native::Buffers_NativeMemcpy()) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ED528 Texture::FromTga");
  const uint32_t f = Frame(sp, 160);
  Store(f + 180, file);  // into the caller's frame, as the original
  car_models::TgaHeaderClear(f + 96);
  Store(f + 80, 0);
  if (!car_models::TgaCreateImage(f + 96, size, f + 180, f + 80, f)) {
    call(kTgaCreateImage, {f + 96, size, f + 180, f + 80}, f, 0x820ED568);
  }
  car_models::TextureCreate(car_models::kSheetFormat, guest::LoadU16(f + 110),
                            guest::LoadU16(f + 112), header_out, data_out, f);
  const uint32_t surface = graphics::GetSurfaceLevel(Load(header_out), 0);
  graphics::LockSurfaceRect(surface, f + 88, 0, 0);
  const uint32_t pitch = Load(f + 88);
  graphics::UnlockSurface(surface);
  call(kResourceRelease, {surface}, f, 0x820ED5B8);
  // The rows, written straight into the texture's data after the unlock.
  uint32_t to = Load(data_out);
  uint32_t from = Load(f + 80);
  for (int32_t y = 0; y < int32_t(guest::LoadU16(f + 112)); y++) {
    const uint32_t row = uint32_t(guest::LoadU16(f + 110)) << 2;
    if (!native::Buffers_Memcpy(to, from, row, 0x820ED5E8)) {
      call(kMemcpy, {to, from, row}, f, 0x820ED5E8);
    }
    to += pitch & ~3u;
    from += row;
  }
  VirtualFree(call, Load(f + 80), f, 0x820ED61C);
  // The empty sub_82106540(f + 96) left out.
  return uint64_t(f) + 96;
}

uint64_t UpdateImages(const tuning_screen::GuestCall& call, uint64_t images,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FE628 DecalImages::Update");
  const uint32_t f = Frame(sp, 112);
  uint64_t r3 = images;
  for (uint32_t i = 0; i < 200; i++) {
    const schema::DecalImageEntry e = DecalEntryAt(images, i);
    if (e.to_make() == 0 || int32_t(e.state()) != 2) {
      continue;
    }
    const uint32_t file = e.file(), size = e.file_size();
    const uint32_t header = e.texture_header_address(), data = e.texture_data_address();
    if (const auto made = TextureFromTga(call, file, size, header, data, f)) {
      r3 = *made;
    } else {
      r3 = call(kTextureFromTga, {file, size, header, data}, f, 0x821FE670);
    }
    e.set_to_make(0);
  }
  return r3;
}

uint64_t Unload(const tuning_screen::GuestCall& call, uint64_t images, uint64_t i,
                uint32_t sp, bool native_free) {
  ITC_NATIVE_RUNNING("sub_821FEA68 DecalImages::Unload");
  const uint32_t f = Frame(sp, 112);
  const schema::DecalImageEntry e = DecalEntryAt(images, i);
  if (int32_t(e.state()) != 2) {
    return images;
  }
  e.set_asset_id(0xFFFFFFFFu);
  e.set_handle(0);
  e.set_state(0);
  // FreeTexture's tail call Texture::Free runs from this frame, returning
  // here.
  FreeTexture(images, i, [&](uint32_t header_in, uint32_t data_in) -> uint64_t {
    if (native_free && car_models::TextureFreeable(header_in)) {
      return car_models::TextureFree(header_in, data_in);
    }
    return call(kTextureFree, {header_in, data_in}, f, 0x821FEAA8);
  });
  const uint32_t file = e.file();
  e.set_texture_header(0);
  e.set_texture_data(0);
  const uint64_t r3 = VirtualFree(call, file, f, 0x821FEAC4);
  e.set_file(0);
  e.set_file_size(0);
  e.set_to_make(0);
  e.set_requested(0);
  return r3;
}

uint64_t UnloadAll(const tuning_screen::GuestCall& call, uint64_t images,
                   uint32_t sp, bool native_free) {
  ITC_NATIVE_RUNNING("sub_82208738 DecalImages::UnloadAll");
  const uint32_t f = Frame(sp, 112);
  uint64_t r3 = RestoreVinylMovie(images, f);
  r3 = RestoreStickerMovie(images, f);
  for (uint64_t i = 0; i < 200; i++) {
    r3 = Unload(call, images, i, f, native_free);
  }
  for (uint32_t k = 0; k < 200; k++) {
    const schema::DecalImageKept kept = DecalKeptAt(images, k);
    if (kept.texture_header() == 0 || kept.texture_data() == 0) {
      continue;
    }
    const uint32_t header = kept.texture_header_address(), data = kept.texture_data_address();
    if (native_free && car_models::TextureFreeable(header)) {
      r3 = car_models::TextureFree(header, data);
    } else {
      r3 = call(kTextureFree, {header, data}, f, 0x822087A4);
    }
  }
  return r3;
}

}  // namespace tuning_pages
