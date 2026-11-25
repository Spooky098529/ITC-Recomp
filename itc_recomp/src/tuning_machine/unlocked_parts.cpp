// TuningScreen::LoadUnlockedParts (docs/functions/tuning-machine.md): see
// tuning_machine.h. The unlocked counterpart of LoadOwnedParts
// (owned_parts.cpp): each unlocked word 1 or 0 from its bit in the save
// data's bitset at +0x19930, in the original's order (the save's pointer
// and the part set read again at each word).

#include "tuning_machine/tuning_machine.h"

#include "base/log.h"
#include "guest.h"
#include "schema/career.h"
#include "schema/tuning_screen.h"
#include "tuning_screen/tuning_screen.h"

namespace tuning_machine {

namespace {

inline uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
inline void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

}  // namespace

void LoadUnlockedParts(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CFFC8 TuningScreen::LoadUnlockedParts");
  const schema::TuningScreen ts(screen);
  // Bit n of the save's unlock set (+0x19930; word n >> 5's 1 << (n & 31)):
  // 1 or 0, the save's pointer read again for each bit.
  auto bit = [&](uint32_t n) {
    const uint32_t set = schema::CareerSave(ts.save_data().address).unlock_set_address();
    return (Load(set + (n >> 5) * 4) >> (n & 31)) & 1;
  };
  // The part tables (the screen's +34104): offsets below are from them,
  // the screen's own in the comments.
  const uint32_t tables = ts.part_tables_address();
  // `count` words from `from`, word k bit `first` + k.
  auto words = [&](uint32_t from, uint32_t count, uint32_t first) {
    for (uint32_t k = 0; k < count; k++) {
      Store(from + 4 * k, bit(first + k));
    }
  };
  // The power items 0 and 1 of the part set (the price class, read at
  // each word): 1 and 3 from one half of the bits, 0, 2 and 4 the other,
  // other sets nothing written. +34104 + 40v + 4i, +34584 + 28v + 4j.
  for (uint32_t i = 0; i < 10; i++) {
    const uint32_t v = ts.price_class();
    const uint32_t at = tables + v * 40 + i * 4;
    if (v == 1 || v == 3) {
      Store(at, bit(i));
    } else if (v == 0 || v == 2 || v == 4) {
      Store(at, bit(i + 10));
    }
  }
  for (uint32_t j = 20; j < 27; j++) {
    const uint32_t v = ts.price_class();
    const uint32_t at = tables + 480 + v * 28 + j * 4;
    if (v == 1 || v == 3) {
      Store(at, bit(j));
    } else if (v == 0 || v == 2 || v == 4) {
      Store(at, bit(j + 7));
    }
  }
  words(tables + 888, 7, 34);   // +34992
  words(tables + 980, 7, 41);   // +35084
  words(tables + 1096, 7, 48);  // +35200
  words(tables + 1184, 7, 55);  // +35288
  words(tables + 1272, 7, 62);  // +35376
  words(tables + 1360, 7, 69);  // +35464
  for (uint32_t j = 0; j < 10; j++) {  // +35568
    Store(tables + 1464 + 4 * j, bit(j < 4 ? 76 + j : j < 7 ? 77 + j : 78 + j));
  }
  words(tables + 1576, 7, 88);  // +35680
  // The tires (+35800): tire 0 from bit 95, tire i (1 to 4) unlocked when
  // any of bits 95 + i, 99 + i and 103 + i is set (tested in that order).
  const uint32_t tires = tables + 1696;
  Store(tires, bit(95));
  for (uint32_t i = 1; i < 5; i++) {
    Store(tires + 4 * i,
          (bit(95 + i) != 0 || bit(99 + i) != 0 || bit(103 + i) != 0) ? 1 : 0);
  }
  // Aero item 9: cleared, then set from bit 187 (word 5's 1 << 27).
  ts.set_aero_available_9(0);
  if (bit(187) != 0) {
    ts.set_aero_available_9(1);
  }
  words(tables + 2472, 3, 186);  // +36576
}

}  // namespace tuning_machine
