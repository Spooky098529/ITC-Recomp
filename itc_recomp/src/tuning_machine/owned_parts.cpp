// TuningScreen::LoadOwnedParts (docs/functions/tuning-machine.md): see
// tuning_machine.h. The inverse of TuningScreen::StoreOwnedParts
// (tuning_pages/pages.cpp): each owned word 1 or 0 from its bit, in the
// original's order (the save's pointer read again at each of its bits).

#include "tuning_machine/tuning_machine.h"

#include "base/log.h"
#include "guest.h"
#include "tuning_pages/tuning_pages.h"
#include "tuning_screen/tuning_screen.h"

namespace tuning_machine {

namespace {

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// Bit n of the bitset at `set` (word n >> 5's 1 << (n & 31)): 1 or 0.
inline uint32_t Bit(uint8_t* base, uint32_t set, uint32_t n) {
  return (Load(base, set + (n >> 5) * 4) >> (n & 31)) & 1;
}

constexpr uint32_t kSaveBits = 0x1996C;  // the save data's owned bitset

}  // namespace

uint64_t LoadOwnedParts(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DBAB0 TuningScreen::LoadOwnedParts");
  const uint32_t s = screen;
  const uint32_t frame = sp - kLoadOwnedPartsFrame;
  Store(base, frame, sp);
  const uint32_t car = tuning_screen::SelectedGarageCar(base, s) + 212 + 1904;
  auto car_bit = [&](uint32_t n) { return Bit(base, car, n); };
  auto save_bit = [&](uint32_t n) {
    return Bit(base, Load(base, s + tuning_screen::kSaveData) + kSaveBits, n);
  };
  auto set = [&](uint32_t offset, uint32_t value) { Store(base, s + offset, value); };
  // `count` words from s + `from`, word k bit `first` + k of the car's
  // bitset (or the save's).
  auto words = [&](uint32_t from, uint32_t count, uint32_t first, bool save) {
    for (uint32_t k = 0; k < count; k++) {
      set(from + 4 * k, save ? save_bit(first + k) : car_bit(first + k));
    }
  };
  // The power items 0 and 1 of the part set [+33900] (read at each word):
  // 1 and 3 from one half of the bits, 0, 2 and 4 the other, other sets
  // nothing written.
  for (uint32_t i = 0; i < 10; i++) {
    const uint32_t v = Load(base, s + 33900);
    const uint32_t at = (v * 10 + i + 8581) * 4;
    if (v == 1 || v == 3) {
      set(at, car_bit(i));
    } else if (v == 0 || v == 2 || v == 4) {
      set(at, car_bit(i + 10));
    }
  }
  for (uint32_t j = 20; j < 27; j++) {
    const uint32_t v = Load(base, s + 33900);
    const uint32_t at = (v * 7 + j + 8686) * 4;
    if (v == 1 || v == 3) {
      set(at, car_bit(j));
    } else if (v == 0 || v == 2 || v == 4) {
      set(at, car_bit(j + 7));
    }
  }
  words(35028, 7, 34, false);
  words(35144, 7, 41, false);
  words(35232, 7, 48, false);
  words(35320, 7, 55, false);
  words(35408, 7, 62, false);
  words(35500, 7, 69, false);
  for (uint32_t j = 0; j < 10; j++) {
    set(35612 + 4 * j, car_bit(j < 4 ? 76 + j : j < 7 ? 77 + j : 78 + j));
  }
  words(35712, 7, 88, false);
  // The tires: tire 0's three triples 1; the others' from the save's bits
  // 95 + i, 99 + i and 103 + i, each into three words.
  for (uint32_t k = 0; k < 3; k++) {
    set(35828 + 4 * k, 1);
    set(35956 + 4 * k, 1);
    set(36084 + 4 * k, 1);
  }
  for (uint32_t i = 1; i < 5; i++) {
    const uint32_t o = 12 * i;
    uint32_t b = save_bit(95 + i);
    set(36084 + o, b);
    set(35828 + o, b);
    set(35956 + o, b);
    b = save_bit(99 + i);
    set(35832 + o, b);
    set(35960 + o, b);
    set(36088 + o, b);
    b = save_bit(103 + i);
    set(35836 + o, b);
    set(35964 + o, b);
    set(36092 + o, b);
  }
  words(36168, 5, 108, false);
  words(36212, 5, 119, false);
  words(36256, 5, 130, false);
  // The interior's item: with +25172 its words 0 and 1 are 1 and 2 to 5
  // bits 142 to 145; else five words from bit 141.
  if (Load(base, s + 25172) != 0) {
    set(36304, 1);
    set(36308, 1);
    for (uint32_t i = 2; i < 6; i++) {
      set(36304 + 4 * i, car_bit(140 + i));
    }
  } else {
    words(36304, 5, 141, false);
  }
  for (uint32_t k = 0; k < 5; k++) {
    set(36380 + 8 * k, car_bit(152 + k));
    set(36384 + 8 * k, car_bit(158 + k));
  }
  set(36384, 1);
  set(36380, 1);
  words(36440, 4, 170, false);
  words(36476, 4, 174, false);
  words(36512, 4, 178, false);
  words(36548, 4, 182, false);
  words(36592, 3, 186, false);
  // The wheels' flags cleared, then set (TestAndSetWheelFlag) for the
  // save's bits 189 on. r3: 35960 (a register leftover of the tires'), or
  // the last call's.
  for (uint32_t k = 0; k < 646; k++) {
    set(27856 + 4 * k, 0);
  }
  uint64_t r3 = 35960;
  for (uint32_t n = 0; n < 179; n++) {
    if (save_bit(189 + n) != 0) {
      r3 = tuning_pages::TestAndSetWheelFlag(base, s, n);
    }
  }
  words(36616, 2, 389, false);
  set(36640, 1);
  set(36644, 1);
  set(36648, 1);
  words(36672, 4, 391, false);
  words(36716, 5, 395, true);
  words(36756, 4, 400, false);
  words(36800, 6, 404, false);
  words(33624, 4, 435, true);
  // The interior item 3's rows: cleared, then set from the save's bits,
  // row t from bits 414 - t on, the table {3, 2, 1, 0} kept in the frame
  // and read from it as the original does.
  for (uint32_t k = 0; k < 20; k++) {
    set(33820 + 4 * k, 0);
  }
  Store(base, frame + 88, 1);
  Store(base, frame + 92, 0);
  Store(base, frame + 80, 3);
  Store(base, frame + 84, 2);
  for (uint32_t r = 0; r < 4; r++) {
    const uint32_t row = frame + 80 + 4 * r;
    if (save_bit(411 + r) != 0) set(Load(base, row) * 20 + 33824, 1);
    if (save_bit(416 + r) != 0) set(Load(base, row) * 20 + 33820, 1);
    if (save_bit(421 + r) != 0) set(Load(base, row) * 20 + 33828, 1);
    if (save_bit(426 + r) != 0) set(Load(base, row) * 20 + 33836, 1);
    if (save_bit(431 + r) != 0) set(Load(base, row) * 20 + 33832, 1);
  }
  return r3;
}

}  // namespace tuning_machine
