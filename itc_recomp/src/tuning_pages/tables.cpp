// The tuning pages' counts and owned flags (docs/functions/tuning-pages.md):
// see tuning_pages.h. Word indices into the screen are computed in 32 bits,
// as the originals' address arithmetic is.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"
#include "schema/tuning_screen.h"
#include "tuning_screen/tuning_screen.h"

#include <optional>

namespace tuning_pages {

namespace {

// The screen (tuning-screen.toml's TuningScreen).
schema::TuningScreen Screen(uint32_t s) { return schema::TuningScreen(s); }

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
// The screen's word address at word index `index`.
uint32_t WordAt(uint32_t screen, uint32_t index) { return index * 4 + screen; }

constexpr uint32_t kCategory = 9656;
constexpr uint32_t kItem = 9660;
constexpr uint32_t kLevel = 9661;
constexpr uint32_t kType = 9662;
constexpr uint32_t kType2 = 9663;
constexpr uint32_t kOption = 9664;

// The current pick's word index, as IsPickOwned and MarkOwned's one-word
// cases find it (none where the original answers 0 or writes nothing).
std::optional<uint32_t> PickIndex(uint32_t s) {
  const int32_t m = int32_t(Screen(s).category());
  const uint32_t p = uint32_t(Screen(s).item());
  auto level = [&] { return uint32_t(Screen(s).level()); };
  auto type = [&] { return uint32_t(Screen(s).type()); };
  switch (m) {
    case 0:
      if (p == 0) {
        const uint32_t l = level();
        return l + Screen(s).price_class() * 10 + 8581;
      }
      if (p == 1) {
        const uint32_t l = level();
        return l + Screen(s).price_class() * 7 + 8706;
      }
      if (p == 2) return level() + 8757;
      if (p == 3) return level() + 8786;
      return std::nullopt;
    case 1: {
      static constexpr uint32_t kBases[5] = {8808, 8830, 8852, 8875, 8903};
      if (p < 5) return level() + kBases[p];
      return std::nullopt;
    }
    case 2:
      return level() + 8928;
    case 3: {
      if (p > 2) return std::nullopt;
      const uint32_t l = level();
      const uint32_t o = uint32_t(Screen(s).option());
      static constexpr uint32_t kBases[3] = {8957, 8989, 9021};
      return l * 3 + o + kBases[p];
    }
    case 4: {
      if (p > 9) return std::nullopt;
      if (p == 4) {
        const uint32_t t = type();
        const uint32_t t2 = uint32_t(Screen(s).bonnet_choice());
        return t * 2 + t2 + 9095;
      }
      static constexpr uint32_t kBases[10] = {9042, 9053, 9064, 9076, 0,
                                              9110, 9119, 9128, 9137, 9148};
      return type() + kBases[p];
    }
    case 5:
      if (p == 0) {
        const uint32_t brand = Screen(s).part_group();
        const uint32_t index = Screen(s).list_cursor();
        return brand * 38 + index + 6964;
      }
      if (p == 1) return uint32_t(Screen(s).option()) + 9154;
      if (p == 2) return type() + 9160;
      if (p == 3) return type() + 9168;
      return std::nullopt;
    case 6: {
      if (p == 0) return type() + 9179;
      if (p == 1) return type() + 9189;
      if (p == 2) return type() + 9200;
      if (p != 3) return std::nullopt;
      const int32_t kind = int32_t(Screen(s).interior_mode());
      if (kind == 0) return Screen(s).interior_pick() + 8406;
      if (kind != 1) return std::nullopt;
      const uint32_t a = Screen(s).interior_page_mode();
      const uint32_t b = Screen(s).interior_pair_index();
      return (a + 1691) * 5 + b;
    }
    default:
      return std::nullopt;
  }
}

}  // namespace

uint32_t LevelCount(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CC728 TuningScreen::LevelCount");
  const uint32_t m = Screen(screen).category();
  if (m == 0) {
    const uint32_t p = uint32_t(Screen(screen).item());
    return p == 0 ? 10 : p <= 3 ? 7 : 0;
  }
  if (m == 1) {
    const uint32_t p = uint32_t(Screen(screen).item());
    return p <= 3 ? 7 : p == 4 ? 10 : 0;
  }
  if (m == 3) {
    return uint32_t(Screen(screen).item()) <= 2 ? 5 : 0;
  }
  return 0;
}

uint32_t TypeCount(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CC7B8 TuningScreen::TypeCount");
  const int32_t m = int32_t(Screen(screen).category());
  if (m == 4) {
    const uint32_t p = uint32_t(Screen(screen).item());
    switch (p) {
      case 0: case 1: case 2: case 4: return 5;
      case 3: return 5 + (Screen(screen).aero3_type_remap() != 0 ? 1 : 0);
      case 5: case 6: case 7: case 8: return 4;
      case 9: return 2 + (Screen(screen).aero9_third_type() != 0 ? 1 : 0);
      default: return 0;
    }
  }
  if (m == 5) {
    const uint32_t p = uint32_t(Screen(screen).item());
    return p == 2 ? 3 : p == 3 ? 4 : 0;
  }
  if (m == 6) {
    const uint32_t p = uint32_t(Screen(screen).item());
    return p == 0 ? 5 : p == 1 ? 4 : p == 2 ? 6 : 0;
  }
  return 0;
}

uint32_t PageCursor(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CCD38 TuningScreen::PageCursor");
  switch (Screen(screen).state() - 3) {
    case 0: return Screen(screen).category();
    case 1: return uint32_t(Screen(screen).item());
    case 2: return uint32_t(Screen(screen).level());
    case 3: return uint32_t(Screen(screen).type());
    case 4: return uint32_t(Screen(screen).bonnet_choice());
    case 5: return uint32_t(Screen(screen).option());
    case 6: case 8: return uint32_t(Screen(screen).confirm_cursor());
    default: return 0;
  }
}

uint32_t PageItemCount(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CCDB0 TuningScreen::PageItemCount");
  switch (Screen(screen).state() - 3) {
    case 0: return 7;
    case 1: return uint32_t(Screen(screen).item_count());
    case 2: return uint32_t(Screen(screen).level_count());
    case 3: return uint32_t(Screen(screen).type_count());
    case 4: return uint32_t(Screen(screen).color_rows());
    case 5: return uint32_t(Screen(screen).option_count());
    case 6: case 8: return 2;
    case 7: return 1;
    default: return 0;
  }
}

uint32_t IsPickOwned(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CCE30 TuningScreen::IsPickOwned");
  const std::optional<uint32_t> index = PickIndex(screen);
  return index ? Load(WordAt(screen, *index)) : 0;
}

void MarkOwned(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CD498 TuningScreen::MarkOwned");
  const uint32_t s = screen;
  const int32_t m = int32_t(Screen(s).category());
  // Every level up to the bought one (the byte read again each step).
  auto levels = [&](uint32_t multiplier, uint32_t offset) {
    for (int32_t k = 0; k <= int32_t(uint32_t(Screen(s).level())); k++) {
      const uint32_t row = Screen(s).price_class();
      Store(WordAt(s, row * multiplier + uint32_t(k) + offset), 1);
    }
  };
  if (m == 0) {
    const uint32_t p = uint32_t(Screen(s).item());
    if (p == 0) {
      levels(10, 8581);
      return;
    }
    if (p == 1) {
      levels(7, 8706);
      return;
    }
  } else if (m == 2) {
    uint32_t at = Screen(s).part_tables_address() + 1608;
    for (int32_t k = 0; k <= int32_t(uint32_t(Screen(s).level())); k++) {
      Store(at, 1);
      at += 4;
    }
    return;
  } else if (m == 3) {
    if (uint32_t(Screen(s).item()) > 2) {
      return;
    }
    for (const uint32_t offset : {8957u, 8989u, 9021u}) {
      const uint32_t l = uint32_t(Screen(s).level());
      const uint32_t o = uint32_t(Screen(s).option());
      Store(WordAt(s, l * 3 + o + offset), 1);
    }
    return;
  }
  const std::optional<uint32_t> index = PickIndex(s);
  if (index) {
    Store(WordAt(s, *index), 1);
  }
}

uint32_t TestAndSetWheelFlag(uint32_t screen, uint64_t n) {
  ITC_NATIVE_RUNNING("sub_821CDF98 TuningScreen::TestAndSetWheelFlag");
  // The brands' first wheel numbers (WheelNumber's), the last's end 179.
  static constexpr int32_t kBases[18] = {0,   1,   39,  47,  72,  99,  116, 123, 124,
                                         126, 135, 144, 147, 162, 163, 165, 170, 179};
  const int32_t number = int32_t(uint32_t(n));
  if (number < 0 || number >= 179) {
    return 0;
  }
  uint32_t brand = 0;
  while (number >= kBases[brand + 1]) {
    brand++;
  }
  const uint32_t index = uint32_t(number - kBases[brand]) + 6964 + 38 * brand;
  const uint32_t at = WordAt(screen, index);
  const uint32_t old = Load(at);
  Store(at, 1);
  return old;
}

void SetWheelListEnd(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821E7DB0 TuningScreen::SetWheelListEnd");
  static constexpr uint32_t kEnds[17] = {0, 37, 7, 24, 26, 16, 6, 0, 1,
                                         8, 8,  2, 14, 0, 1,  4,  8};
  const uint32_t brand = Screen(screen).part_group();
  if (brand <= 16) {
    Screen(screen).set_list_last(kEnds[brand]);
  }
}

uint32_t InchListEnd(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821EF288 TuningScreen::InchListEnd");
  static constexpr uint8_t kEnds[23] = {3, 2, 0, 0, 3, 4, 4, 2, 4, 0, 4, 2,
                                        2, 3, 3, 4, 4, 3, 0, 2, 0, 3, 4};
  const uint32_t id = Load(tuning_screen::SelectedGarageCar(screen) + 224);
  return id <= 22 ? kEnds[id] : 0;
}

}  // namespace tuning_pages
