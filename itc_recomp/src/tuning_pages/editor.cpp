// The vinyl and sticker editors' set-up from the tuning pages
// (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"
#include "tuning_screen/tuning_screen.h"

namespace tuning_pages {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The vinyls: category 5's item 6.
bool Vinyls(uint8_t* base, uint32_t screen) {
  return int32_t(Load(base, screen + 9656)) == 5 && guest::LoadU8(base, screen + 9660) == 6;
}

}  // namespace

void SetUpVinylBusyIcon(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821FE560 TuningScreen::SetUpVinylBusyIcon");
  Store(base, screen + 10016, 0);
  Store(base, screen + 10020, 13);
  Store(base, screen + 10024, 155);
  Store(base, screen + 10028, 154);
  Store(base, screen + 10032, 156);
}

uint32_t AllLoaded(uint8_t* base, uint32_t images) {
  ITC_NATIVE_RUNNING("sub_821FE5D8 DecalImages::AllLoaded");
  uint32_t loaded = 1;
  for (uint32_t i = 0; i < 200; i++) {
    if (int32_t(Load(base, images + 36 * i + 8)) == 1) {
      loaded = 0;
    }
  }
  return loaded;
}

void KeepListTextures(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821FE7D8 TuningScreen::KeepListTextures");
  if (!Vinyls(base, screen)) {
    return;
  }
  const uint32_t images = screen + 10048;
  for (uint32_t j = 0; j < 9; j++) {
    const uint32_t entry = images + 432 + 36 * j;
    const uint32_t kept = images + 7236 + 48 + 8 * j;
    Store(base, kept, Load(base, entry + 28));
    Store(base, kept + 4, Load(base, entry + 32));
  }
}

void SetEditorLimits(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821FE420 TuningScreen::SetEditorLimits");
  if (!Vinyls(base, screen)) {
    Store(base, screen + 30788, 4);
    Store(base, screen + 30784, 5);
    Store(base, screen + 30792, 4);
    Store(base, screen + 30796, 198);
    return;
  }
  const int32_t set = int32_t(Load(base, screen + 30724));
  Store(base, screen + 30788, 2);
  Store(base, screen + 30784, 1);
  Store(base, screen + 30792, 2);
  if (set == 0) {
    Store(base, screen + 30796, 4);
  } else if (set == 1) {
    Store(base, screen + 30796, 193);
  }
}

void LoadPageLayouts(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_822045F8 TuningScreen::LoadPageLayouts");
  const uint32_t car = tuning_screen::SelectedGarageCar(base, screen) + 212;
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t id = Load(base, car + 12);
    const uint32_t from = ((id * 2 + k) << 4) + 0x8200C560u;
    const uint32_t to = screen + 30800 + 16 * k;
    for (uint32_t i = 0; i < 16; i += 4) {
      Store(base, to + i, Load(base, from + i));
    }
  }
}

}  // namespace tuning_pages
