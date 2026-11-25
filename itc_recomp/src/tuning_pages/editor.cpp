// The vinyl and sticker editors' set-up from the tuning pages
// (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"
#include "schema/tuning_screen.h"
#include "tuning_screen/tuning_screen.h"

namespace tuning_pages {

namespace {

// The screen (tuning-screen.toml's TuningScreen).
schema::TuningScreen Screen(uint32_t s) { return schema::TuningScreen(s); }

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// The vinyls: category 5's item 6.
bool Vinyls(uint32_t screen) {
  return int32_t(Screen(screen).category()) == 5 && Screen(screen).item() == 6;
}

}  // namespace

void SetUpVinylBusyIcon(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821FE560 TuningScreen::SetUpVinylBusyIcon");
  Screen(screen).set_busy_shown(0);
  Screen(screen).set_busy_movie(13);
  Screen(screen).set_busy_animation_0(155);
  Screen(screen).set_busy_animation_1(154);
  Screen(screen).set_busy_animation_2(156);
}

uint32_t AllLoaded(uint32_t images) {
  ITC_NATIVE_RUNNING("sub_821FE5D8 DecalImages::AllLoaded");
  uint32_t loaded = 1;
  for (uint32_t i = 0; i < 200; i++) {
    if (int32_t(DecalEntryAt(images, i).state()) == 1) {
      loaded = 0;
    }
  }
  return loaded;
}

void KeepListTextures(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821FE7D8 TuningScreen::KeepListTextures");
  if (!Vinyls(screen)) {
    return;
  }
  const uint32_t images = Screen(screen).decal_images_address();
  // Entries 12 to 20 (the first list's window) into kept 6 to 14.
  for (uint32_t j = 0; j < 9; j++) {
    const schema::DecalImageEntry entry = DecalEntryAt(images, 12 + j);
    const schema::DecalImageKept kept = DecalKeptAt(images, 6 + j);
    kept.set_texture_header(entry.texture_header());
    kept.set_texture_data(entry.texture_data());
  }
}

void SetEditorLimits(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821FE420 TuningScreen::SetEditorLimits");
  if (!Vinyls(screen)) {
    Screen(screen).set_last_layer(4);
    Screen(screen).set_last_page(5);
    Screen(screen).set_order_last_position(4);
    Screen(screen).set_image_last_index(198);
    return;
  }
  const int32_t set = int32_t(Screen(screen).editor_page());
  Screen(screen).set_last_layer(2);
  Screen(screen).set_last_page(1);
  Screen(screen).set_order_last_position(2);
  if (set == 0) {
    Screen(screen).set_image_last_index(4);
  } else if (set == 1) {
    Screen(screen).set_image_last_index(193);
  }
}

void LoadPageLayouts(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_822045F8 TuningScreen::LoadPageLayouts");
  const uint32_t car = tuning_screen::SelectedGarageCar(screen) + 212;
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t id = Load(car + 12);
    const uint32_t from = ((id * 2 + k) << 4) + 0x8200C560u;
    const uint32_t to = screen + 30800 + 16 * k;
    for (uint32_t i = 0; i < 16; i += 4) {
      Store(to + i, Load(from + i));
    }
  }
}

}  // namespace tuning_pages
