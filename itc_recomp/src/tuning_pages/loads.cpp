// The decal images' loads the editors' entry asks for
// (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "archive/archive.h"
#include "base/log.h"
#include "guest.h"
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

constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kArchiveLoad = 0x8209DE48;
constexpr uint32_t kMemCommit = 0x1000;
constexpr uint32_t kOnImageLoaded = 0x821FE608;  // the entry's load callback

// The load both functions end with, for the entry `e` (all 64 bits, as
// Archive::Load's context) whose +0 holds the id (`size` its asset size
// already asked): the file's pages (VirtualAlloc(0, size, MEM_COMMIT, 4))
// into +12, Archive::Load(+0, the pages, the callback, e) into +4, +8 = +24
// = 1, +16 the size asked again, +20 = 0. The callers' frame and return
// addresses. The second AssetSize's r3.
uint64_t LoadEntry(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t e, uint32_t size,
                   uint32_t frame, uint32_t alloc_lr, uint32_t load_lr) {
  const uint32_t entry = uint32_t(e);
  const uint64_t pages = call(kVirtualAlloc, {0, size, kMemCommit, 4}, frame, alloc_lr);
  const uint32_t id = Load(base, entry + 0);
  Store(base, entry + 12, uint32_t(pages));
  const uint64_t serial = call(kArchiveLoad, {id, pages, kOnImageLoaded, e, 0, 0}, frame, load_lr);
  const uint32_t again = Load(base, entry + 0);
  Store(base, entry + 4, uint32_t(serial));
  Store(base, entry + 8, 1);
  Store(base, entry + 24, 1);
  const uint32_t asked = archive::AssetSize(base, again);
  Store(base, entry + 16, asked);
  Store(base, entry + 20, 0);
  return asked;
}

}  // namespace

uint64_t RequestImage(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images,
                      uint64_t id, uint64_t i, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FE830 DecalImages::Request");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t e = i * 36 + images;
  const uint32_t entry = uint32_t(e);
  const int32_t state = int32_t(Load(base, entry + 8));
  if (state == 1 || state == 2) {
    return images;
  }
  // A "none" id swapped for the group's stand-in; entries above 108 (or
  // negative) keep their old id.
  const uint32_t n = uint32_t(i), wanted = uint32_t(id);
  auto swap = [&](uint32_t none, uint32_t stand_in) {
    Store(base, entry + 0, wanted == none ? stand_in : wanted);
  };
  if (n <= 20) {
    swap(162, 22601);
  } else if ((int32_t(n) >= 21 && int32_t(n) <= 41) || n - 42 <= 8) {
    swap(188, 22602);
  } else if (n - 51 <= 28) {
    swap(382, 22603);
  } else if (n - 80 <= 28) {
    swap(382, 22604);
  }
  const uint32_t size = archive::AssetSize(base, Load(base, entry + 0));
  return LoadEntry(base, call, e, size, frame, 0x821FE8FC, 0x821FE920);
}

uint64_t LoadImage(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images,
                   uint64_t id, uint64_t i, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F6D40 DecalImages::Load");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t e = i * 36 + images;
  Store(base, uint32_t(e) + 0, uint32_t(id));
  const uint32_t size = archive::AssetSize(base, uint32_t(id));
  return LoadEntry(base, call, e, size, frame, 0x821F6D78, 0x821F6D9C);
}

uint64_t LoadDecalImages(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FEE10 TuningScreen::LoadDecalImages");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 128);
  const uint64_t images = screen + 10048;
  uint64_t r3 = 0;
  if (int32_t(Load(base, s + 9656)) != 5 || guest::LoadU8(base, s + 9660) != 6) {
    // The stickers: images 383 to 581 into entries 0 to 198, 22604 into 199.
    for (uint64_t i = 0; i < 199; i++) {
      LoadImage(base, call, images, i + 383, i, frame);
    }
    return LoadImage(base, call, images, 22604, 199, frame);
  }
  // The vinyls. Entries 0 to 11: the first group's three layer ids
  // (+31168), four times over.
  for (uint64_t i = 0; i < 12; i++) {
    const uint64_t id = uint64_t(Load(base, s + 31168 + 4 * uint32_t(i % 3))) + 163;
    r3 = RequestImage(base, call, images, id, i, frame);
  }
  // Entries 12 to 20: the first group's list window (163 + (k mod 5)).
  for (int64_t k = -4; k <= 4; k++) {
    const int64_t index = k < 0 ? k + 5 : k;
    r3 = RequestImage(base, call, images, uint64_t(index + 163), uint64_t(k + 16), frame);
  }
  // Entries 21 to 32: the second group's layer ids (+31180), four times.
  for (uint64_t i = 21; i < 33; i++) {
    const uint64_t id = uint64_t(Load(base, s + 31180 + 4 * uint32_t((i - 21) % 3))) + 189;
    r3 = RequestImage(base, call, images, id, i, frame);
  }
  // Entries 33 to 41 (189 + (k mod 194)) and 42 to 50 (189 + k + 43).
  for (int64_t k = -4; k <= 4; k++) {
    const int64_t index = k < 0 ? k + 194 : k;
    RequestImage(base, call, images, uint64_t(index + 189), uint64_t(k + 37), frame);
    r3 = RequestImage(base, call, images, uint64_t(k + 43 + 189), uint64_t(k + 46), frame);
  }
  return r3;
}

}  // namespace tuning_pages
