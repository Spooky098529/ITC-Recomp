// The tuning pages' counts and owned flags (docs/functions/tuning-pages.md):
// see tuning_pages.h. Word indices into the screen are computed in 32 bits,
// as the originals' address arithmetic is.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"
#include "tuning_screen/tuning_screen.h"

#include <optional>

namespace tuning_pages {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Byte(uint8_t* base, uint32_t address) { return guest::LoadU8(base, address); }
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
std::optional<uint32_t> PickIndex(uint8_t* base, uint32_t s) {
  const int32_t m = int32_t(Load(base, s + kCategory));
  const uint32_t p = Byte(base, s + kItem);
  auto level = [&] { return Byte(base, s + kLevel); };
  auto type = [&] { return Byte(base, s + kType); };
  switch (m) {
    case 0:
      if (p == 0) {
        const uint32_t l = level();
        return l + Load(base, s + 33900) * 10 + 8581;
      }
      if (p == 1) {
        const uint32_t l = level();
        return l + Load(base, s + 33900) * 7 + 8706;
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
      const uint32_t o = Byte(base, s + kOption);
      static constexpr uint32_t kBases[3] = {8957, 8989, 9021};
      return l * 3 + o + kBases[p];
    }
    case 4: {
      if (p > 9) return std::nullopt;
      if (p == 4) {
        const uint32_t t = type();
        const uint32_t t2 = Byte(base, s + kType2);
        return t * 2 + t2 + 9095;
      }
      static constexpr uint32_t kBases[10] = {9042, 9053, 9064, 9076, 0,
                                              9110, 9119, 9128, 9137, 9148};
      return type() + kBases[p];
    }
    case 5:
      if (p == 0) {
        const uint32_t brand = Load(base, s + 25204);
        const uint32_t index = Load(base, s + 25220);
        return brand * 38 + index + 6964;
      }
      if (p == 1) return Byte(base, s + kOption) + 9154;
      if (p == 2) return type() + 9160;
      if (p == 3) return type() + 9168;
      return std::nullopt;
    case 6: {
      if (p == 0) return type() + 9179;
      if (p == 1) return type() + 9189;
      if (p == 2) return type() + 9200;
      if (p != 3) return std::nullopt;
      const int32_t kind = int32_t(Load(base, s + 33540));
      if (kind == 0) return Load(base, s + 33548) + 8406;
      if (kind != 1) return std::nullopt;
      const uint32_t a = Load(base, s + 33564);
      const uint32_t b = Load(base, s + 33572);
      return (a + 1691) * 5 + b;
    }
    default:
      return std::nullopt;
  }
}

}  // namespace

uint32_t LevelCount(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CC728 TuningScreen::LevelCount");
  const uint32_t m = Load(base, screen + kCategory);
  if (m == 0) {
    const uint32_t p = Byte(base, screen + kItem);
    return p == 0 ? 10 : p <= 3 ? 7 : 0;
  }
  if (m == 1) {
    const uint32_t p = Byte(base, screen + kItem);
    return p <= 3 ? 7 : p == 4 ? 10 : 0;
  }
  if (m == 3) {
    return Byte(base, screen + kItem) <= 2 ? 5 : 0;
  }
  return 0;
}

uint32_t TypeCount(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CC7B8 TuningScreen::TypeCount");
  const int32_t m = int32_t(Load(base, screen + kCategory));
  if (m == 4) {
    const uint32_t p = Byte(base, screen + kItem);
    switch (p) {
      case 0: case 1: case 2: case 4: return 5;
      case 3: return 5 + (Load(base, screen + 25172) != 0 ? 1 : 0);
      case 5: case 6: case 7: case 8: return 4;
      case 9: return 2 + (Load(base, screen + 25168) != 0 ? 1 : 0);
      default: return 0;
    }
  }
  if (m == 5) {
    const uint32_t p = Byte(base, screen + kItem);
    return p == 2 ? 3 : p == 3 ? 4 : 0;
  }
  if (m == 6) {
    const uint32_t p = Byte(base, screen + kItem);
    return p == 0 ? 5 : p == 1 ? 4 : p == 2 ? 6 : 0;
  }
  return 0;
}

uint32_t PageCursor(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CCD38 TuningScreen::PageCursor");
  switch (Load(base, screen + 9652) - 3) {
    case 0: return Load(base, screen + kCategory);
    case 1: return Byte(base, screen + kItem);
    case 2: return Byte(base, screen + kLevel);
    case 3: return Byte(base, screen + kType);
    case 4: return Byte(base, screen + kType2);
    case 5: return Byte(base, screen + kOption);
    case 6: case 8: return Byte(base, screen + 9665);
    default: return 0;
  }
}

uint32_t PageItemCount(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CCDB0 TuningScreen::PageItemCount");
  switch (Load(base, screen + 9652) - 3) {
    case 0: return 7;
    case 1: return Byte(base, screen + 9676);
    case 2: return Byte(base, screen + 9677);
    case 3: return Byte(base, screen + 9678);
    case 4: return Byte(base, screen + 9679);
    case 5: return Byte(base, screen + 9680);
    case 6: case 8: return 2;
    case 7: return 1;
    default: return 0;
  }
}

uint32_t IsPickOwned(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CCE30 TuningScreen::IsPickOwned");
  const std::optional<uint32_t> index = PickIndex(base, screen);
  return index ? Load(base, WordAt(screen, *index)) : 0;
}

void MarkOwned(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CD498 TuningScreen::MarkOwned");
  const uint32_t s = screen;
  const int32_t m = int32_t(Load(base, s + kCategory));
  // Every level up to the bought one (the byte read again each step).
  auto levels = [&](uint32_t multiplier, uint32_t offset) {
    for (int32_t k = 0; k <= int32_t(Byte(base, s + kLevel)); k++) {
      const uint32_t row = Load(base, s + 33900);
      Store(base, WordAt(s, row * multiplier + uint32_t(k) + offset), 1);
    }
  };
  if (m == 0) {
    const uint32_t p = Byte(base, s + kItem);
    if (p == 0) {
      levels(10, 8581);
      return;
    }
    if (p == 1) {
      levels(7, 8706);
      return;
    }
  } else if (m == 2) {
    uint32_t at = s + 35712;
    for (int32_t k = 0; k <= int32_t(Byte(base, s + kLevel)); k++) {
      Store(base, at, 1);
      at += 4;
    }
    return;
  } else if (m == 3) {
    if (Byte(base, s + kItem) > 2) {
      return;
    }
    for (const uint32_t offset : {8957u, 8989u, 9021u}) {
      const uint32_t l = Byte(base, s + kLevel);
      const uint32_t o = Byte(base, s + kOption);
      Store(base, WordAt(s, l * 3 + o + offset), 1);
    }
    return;
  }
  const std::optional<uint32_t> index = PickIndex(base, s);
  if (index) {
    Store(base, WordAt(s, *index), 1);
  }
}

uint32_t TestAndSetWheelFlag(uint8_t* base, uint32_t screen, uint64_t n) {
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
  const uint32_t old = Load(base, at);
  Store(base, at, 1);
  return old;
}

void SetWheelListEnd(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821E7DB0 TuningScreen::SetWheelListEnd");
  static constexpr uint32_t kEnds[17] = {0, 37, 7, 24, 26, 16, 6, 0, 1,
                                         8, 8,  2, 14, 0, 1,  4,  8};
  const uint32_t brand = Load(base, screen + 25204);
  if (brand <= 16) {
    Store(base, screen + 25224, kEnds[brand]);
  }
}

uint32_t InchListEnd(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821EF288 TuningScreen::InchListEnd");
  static constexpr uint8_t kEnds[23] = {3, 2, 0, 0, 3, 4, 4, 2, 4, 0, 4, 2,
                                        2, 3, 3, 4, 4, 3, 0, 2, 0, 3, 4};
  const uint32_t id = Load(base, tuning_screen::SelectedGarageCar(base, screen) + 224);
  return id <= 22 ? kEnds[id] : 0;
}

}  // namespace tuning_pages
