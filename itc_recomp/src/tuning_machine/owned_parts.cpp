// TuningScreen::LoadOwnedParts (docs/functions/tuning-machine.md): see
// tuning_machine.h. The inverse of TuningScreen::StoreOwnedParts
// (tuning_pages/pages.cpp): each owned word 1 or 0 from its bit, in the
// original's order (the save's pointer read again at each of its bits).

#include "tuning_machine/tuning_machine.h"

#include "base/log.h"
#include "guest.h"
#include "schema/car_makers.h"
#include "schema/career.h"
#include "schema/tuning_screen.h"
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

}  // namespace

uint64_t LoadOwnedParts(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DBAB0 TuningScreen::LoadOwnedParts");
  const schema::TuningScreen ts(base, screen);
  const uint32_t frame = sp - kLoadOwnedPartsFrame;
  Store(base, frame, sp);
  // The selected garage car's owned bitset (the car at its +212).
  const uint32_t car =
      schema::GarageCar(base, tuning_screen::SelectedGarageCar(base, screen) + 212).owned_parts_address();
  auto car_bit = [&](uint32_t n) { return Bit(base, car, n); };
  // The save data's second unlock set (+0x1996C), its pointer read again
  // for each bit.
  auto save_bit = [&](uint32_t n) {
    return Bit(base, schema::CareerSave(base, ts.save_data().address).unlock_set_2_address(), n);
  };
  // The part tables (the screen's +34104): offsets below are from them,
  // the screen's own in the comments.
  const uint32_t tables = ts.part_tables_address();
  // `count` words from `from`, word k bit `first` + k of the car's bitset
  // (or the save's).
  auto words = [&](uint32_t from, uint32_t count, uint32_t first, bool save) {
    for (uint32_t k = 0; k < count; k++) {
      Store(base, from + 4 * k, save ? save_bit(first + k) : car_bit(first + k));
    }
  };
  // The power items 0 and 1 of the part set (the price class, read at
  // each word): 1 and 3 from one half of the bits, 0, 2 and 4 the other,
  // other sets nothing written. +34324 + 40v + 4i, +34744 + 28v + 4j.
  for (uint32_t i = 0; i < 10; i++) {
    const uint32_t v = ts.price_class();
    const uint32_t at = tables + 220 + v * 40 + i * 4;
    if (v == 1 || v == 3) {
      Store(base, at, car_bit(i));
    } else if (v == 0 || v == 2 || v == 4) {
      Store(base, at, car_bit(i + 10));
    }
  }
  for (uint32_t j = 20; j < 27; j++) {
    const uint32_t v = ts.price_class();
    const uint32_t at = tables + 640 + v * 28 + j * 4;
    if (v == 1 || v == 3) {
      Store(base, at, car_bit(j));
    } else if (v == 0 || v == 2 || v == 4) {
      Store(base, at, car_bit(j + 7));
    }
  }
  words(tables + 924, 7, 34, false);   // +35028
  words(tables + 1040, 7, 41, false);  // +35144
  words(tables + 1128, 7, 48, false);  // +35232
  words(tables + 1216, 7, 55, false);  // +35320
  words(tables + 1304, 7, 62, false);  // +35408
  words(tables + 1396, 7, 69, false);  // +35500
  for (uint32_t j = 0; j < 10; j++) {  // +35612
    Store(base, tables + 1508 + 4 * j, car_bit(j < 4 ? 76 + j : j < 7 ? 77 + j : 78 + j));
  }
  words(tables + 1608, 7, 88, false);  // +35712
  // The tires: tire 0's three triples 1; the others' from the save's bits
  // 95 + i, 99 + i and 103 + i, each into three words (+35828, +35956 and
  // +36084 the three rows).
  const uint32_t both = tables + 1724, front = tables + 1852, rear = tables + 1980;
  for (uint32_t k = 0; k < 3; k++) {
    Store(base, both + 4 * k, 1);
    Store(base, front + 4 * k, 1);
    Store(base, rear + 4 * k, 1);
  }
  for (uint32_t i = 1; i < 5; i++) {
    const uint32_t o = 12 * i;
    uint32_t b = save_bit(95 + i);
    Store(base, rear + o, b);
    Store(base, both + o, b);
    Store(base, front + o, b);
    b = save_bit(99 + i);
    Store(base, both + 4 + o, b);
    Store(base, front + 4 + o, b);
    Store(base, rear + 4 + o, b);
    b = save_bit(103 + i);
    Store(base, both + 8 + o, b);
    Store(base, front + 8 + o, b);
    Store(base, rear + 8 + o, b);
  }
  words(tables + 2064, 5, 108, false);  // +36168
  words(tables + 2108, 5, 119, false);  // +36212
  words(tables + 2152, 5, 130, false);  // +36256
  // The interior's item (+36304): with the aero 3 remap its words 0 and 1
  // are 1 and 2 to 5 bits 142 to 145; else five words from bit 141.
  const uint32_t item = tables + 2200;
  if (ts.aero3_type_remap() != 0) {
    Store(base, item, 1);
    Store(base, item + 4, 1);
    for (uint32_t i = 2; i < 6; i++) {
      Store(base, item + 4 * i, car_bit(140 + i));
    }
  } else {
    words(item, 5, 141, false);
  }
  const uint32_t pairs = tables + 2276;  // +36380
  for (uint32_t k = 0; k < 5; k++) {
    Store(base, pairs + 8 * k, car_bit(152 + k));
    Store(base, pairs + 4 + 8 * k, car_bit(158 + k));
  }
  Store(base, pairs + 4, 1);
  Store(base, pairs, 1);
  words(tables + 2336, 4, 170, false);  // +36440
  words(tables + 2372, 4, 174, false);  // +36476
  words(tables + 2408, 4, 178, false);  // +36512
  words(tables + 2444, 4, 182, false);  // +36548
  words(tables + 2488, 3, 186, false);  // +36592
  // The wheels' flags cleared, then set (TestAndSetWheelFlag) for the
  // save's bits 189 on. r3: 35960 (a register leftover of the tires'), or
  // the last call's.
  for (uint32_t k = 0; k < 646; k++) {
    Store(base, ts.wheel_flags_address() + 4 * k, 0);
  }
  uint64_t r3 = 35960;
  for (uint32_t n = 0; n < 179; n++) {
    if (save_bit(189 + n) != 0) {
      r3 = tuning_pages::TestAndSetWheelFlag(base, screen, n);
    }
  }
  words(tables + 2512, 2, 389, false);  // +36616
  Store(base, tables + 2536, 1);        // +36640
  Store(base, tables + 2540, 1);
  Store(base, tables + 2544, 1);
  words(tables + 2568, 4, 391, false);  // +36672
  words(tables + 2612, 5, 395, true);   // +36716
  words(tables + 2652, 4, 400, false);  // +36756
  words(tables + 2696, 6, 404, false);  // +36800
  words(ts.unknown_33624_address(), 4, 435, true);
  // The interior item 3's rows (+33820, 20 bytes a row): cleared, then set
  // from the save's bits, row t from bits 414 - t on, the table {3, 2, 1,
  // 0} kept in the frame and read from it as the original does.
  const uint32_t rows = ts.unknown_33820_address();
  for (uint32_t k = 0; k < 20; k++) {
    Store(base, rows + 4 * k, 0);
  }
  Store(base, frame + 88, 1);
  Store(base, frame + 92, 0);
  Store(base, frame + 80, 3);
  Store(base, frame + 84, 2);
  for (uint32_t r = 0; r < 4; r++) {
    const uint32_t row = frame + 80 + 4 * r;
    if (save_bit(411 + r) != 0) Store(base, rows + Load(base, row) * 20 + 4, 1);
    if (save_bit(416 + r) != 0) Store(base, rows + Load(base, row) * 20 + 0, 1);
    if (save_bit(421 + r) != 0) Store(base, rows + Load(base, row) * 20 + 8, 1);
    if (save_bit(426 + r) != 0) Store(base, rows + Load(base, row) * 20 + 16, 1);
    if (save_bit(431 + r) != 0) Store(base, rows + Load(base, row) * 20 + 12, 1);
  }
  return r3;
}

}  // namespace tuning_machine
