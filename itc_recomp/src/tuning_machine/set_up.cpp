// The tuning screen's set-up's callees (docs/functions/tuning-machine.md):
// see tuning_machine.h. Built with -ffp-model=strict and -ffp-contract=off
// (CMakeLists.txt): the vinyl layers' angles are the translated code's
// products rounded to single and truncated as fctiwz.

#include "tuning_machine/tuning_machine.h"

#include <climits>
#include <cmath>

#include "base/log.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "tuning_screen/tuning_screen.h"

namespace tuning_machine {

namespace {

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The original's frame below `sp`, its back chain written.
inline uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// fctiwz as the recompiled code does it: NaN 0x80000000, from INT_MAX
// saturated, below INT_MIN 0x80000000 (cvttsd2si).
inline int32_t TruncateWord(double value) {
  if (std::isnan(value)) {
    return INT32_MIN;
  }
  if (value >= double(INT_MAX)) {
    return INT_MAX;
  }
  if (value < -2147483648.0) {
    return INT32_MIN;
  }
  return int32_t(value);
}

constexpr uint32_t kDegrees = 0x82002244;  // the float the vinyl angles are scaled by

}  // namespace

void LoadInteriorSlots(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821DE280 TuningScreen::LoadInteriorSlots");
  const uint32_t car = tuning_screen::SelectedGarageCar(base, screen);
  if (int32_t(Load(base, car + 224)) == 10) {
    Store(base, screen + 25192, 0);
  }
}

uint64_t LoadCurrentChoices(uint8_t* base, uint32_t screen, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821DA4B0 TuningScreen::LoadCurrentChoices");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, uint32_t(r1), kLoadCurrentChoicesFrame);
  const uint32_t c = tuning_screen::SelectedGarageCar(base, s) + 212;
  auto at = [&](uint32_t offset) { return Load(base, c + offset); };
  auto set = [&](uint32_t offset, uint32_t value) { Store(base, s + offset, value); };
  // The power and power train tables' rows by the part set (read again
  // after the first store, as the original).
  set((Load(base, s + tuning_screen::kPartSet) + 8576) * 4, at(1656));
  set((Load(base, s + tuning_screen::kPartSet) + 8701) * 4, at(1664));
  set(35020, at(1672));
  set(35024, at(1668));
  set(35140, at(1676));
  set(35228, at(1680));
  set(35316, at(1684));
  set(35404, at(1688));
  set(35492, at(1696));
  // The gear set: 0 3, 4 1, 5 4, 6 2, others 0.
  switch (int32_t(at(1692))) {
    case 0: set(35496, 3); break;
    case 4: set(35496, 1); break;
    case 5: set(35496, 4); break;
    case 6: set(35496, 2); break;
    default: set(35496, 0); break;
  }
  {
    const uint32_t low = at(1704);
    if (int32_t(low) == 0) {
      set(35608, 0);
    } else {
      set(35608, at(1700) * 3 + low);
    }
  }
  set(35708, at(1708));
  set(35820, 0);
  set(35824, 0);
  set(35948, at(1716));
  if (int32_t(at(1716)) == 0) {
    set(35952, 0);
  } else {
    set(35952, at(1712) - 1);
  }
  set(36076, at(1724));
  if (int32_t(at(1724)) == 0) {
    set(36080, 0);
  } else {
    set(36080, at(1720) - 1);
  }
  {
    const bool plain = int32_t(Load(base, s + 25172)) == 0;
    set(36164, at(1744));
    set(36208, at(1748));
    set(36252, at(1752));
    const int32_t v = int32_t(at(1756));
    if (plain) {
      set(36300, uint32_t(v));
    } else if (v == 0) {
      set(36300, 0);
    } else if (v > 0 && v < 11) {
      set(36300, uint32_t(v + 1));
    } else {
      set(36300, 1);
    }
  }
  set(36368, at(1760));
  switch (int32_t(at(1764))) {
    case 0:
      set(36372, 0);
      set(36376, 0);
      break;
    case 1:
      set(36376, 0);
      set(36372, 1);
      break;
    case 2:
      set(36376, 1);
      set(36372, 1);
      break;
    default: break;
  }
  set(36436, at(1768));
  set(36472, at(1772));
  set(36508, at(1776));
  set(36544, at(1780));
  set(36588, at(1784));
  set(27836, at(1728));
  set(27848, at(1732));
  set(27840, at(1736));
  set(27852, at(1740));
  set(36612, at(1792));
  set(36636, uint32_t(int32_t(int8_t(guest::LoadU8(base, c + 1568)))));
  set(36668, at(1796));
  // The paint and lights: the car's own words when its paint is -2, else
  // the car spec's entry for it.
  const int32_t paint = int16_t(guest::LoadU16(base, c + 0));
  if (paint == -2) {
    set(30476, guest::LoadU16(base, c + 8));
    set(30480, guest::LoadU8(base, c + 10));
    set(30484, guest::LoadU8(base, c + 11));
    set(30488, uint32_t(int32_t(int16_t(guest::LoadU16(base, c + 6)))));
  } else {
    const uint32_t spec = car_specs::GetCarSpec(base, at(12));
    const uint32_t entry = spec + uint32_t(paint) * 20;
    set(30476, guest::LoadU16(base, entry + 8120));
    set(30480, guest::LoadU8(base, entry + 8122));
    set(30484, guest::LoadU8(base, entry + 8123));
    set(30488, guest::LoadU16(base, entry + 8116));
  }
  // The three vinyl layers (car c + 1588 + 20i, screen s + 30660 + 4i, w).
  const double degrees = double(guest::LoadFloat(base, kDegrees));
  auto angle = [&](uint32_t field) {
    const double value = double(float(double(guest::LoadFloat(base, field)) * degrees));
    return uint32_t(TruncateWord(value));
  };
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t e = c + 1588 + 20 * i;
    const uint32_t w = s + 30660 + 4 * i;
    const int32_t kind = int32_t(Load(base, e));
    if (kind == 0) {
      Store(base, w - 80, 0);
      Store(base, w + 0, 0);
      Store(base, w - 64, 180);
      Store(base, w - 48, 180);
      Store(base, w - 32, 180);
      Store(base, w + 16, 0);
      Store(base, w + 32, 180);
    } else if (kind == 1) {
      Store(base, w - 80, 1);
      Store(base, w + 0, 0);
      Store(base, w - 64, angle(e + 8));
      Store(base, w - 48, angle(e + 12));
      const double value = double(float(double(guest::LoadFloat(base, e + 16)) * degrees));
      Store(base, w + 16, 0);
      Store(base, w + 32, 180);
      Store(base, w - 32, uint32_t(TruncateWord(value)));
    } else if (kind >= 2 && kind <= 11) {
      Store(base, w - 80, 0);
      Store(base, w + 0, 1);
      Store(base, w - 64, 180);
      Store(base, w - 48, 180);
      Store(base, w - 32, 180);
      Store(base, w - 16, 0);
      Store(base, w + 16, Load(base, e) - 2);
      Store(base, w + 32, angle(e + 16));
      Store(base, w + 48, Load(base, e + 4));
    }
  }
  // The mirror table in the frame (+80 to +108), indexed unchecked by the
  // first layer's +4 (in play 0 to 7) for eight words.
  {
    static constexpr uint32_t kTable[][2] = {{80, 0}, {88, 4},  {92, 2},  {96, 1},
                                             {84, 6}, {100, 3}, {108, 5}, {104, 7}};
    for (const auto& [offset, value] : kTable) {
      Store(base, frame + offset, value);
    }
    for (const uint32_t field : {30644u, 30648u, 30652u, 30656u, 30708u, 30712u, 30716u, 30720u}) {
      set(field, Load(base, frame + 80 + at(1592) * 4));
    }
  }
  // The neon: v 0 0; 1 and 2 by w 0 to 3: 3, 1, 0, 2; 3 by w 0 to 2: 2, 0,
  // 1; 4 by w 0 to 3: 2, 0, 1, 3; others nothing.
  set(36708, at(1800));
  {
    const uint32_t v = at(1800);
    int64_t value = -1;
    if (v == 0) {
      value = 0;
    } else if (v <= 4) {
      const uint32_t w = at(1804);
      if (v <= 2) {
        static constexpr int64_t kValues[4] = {3, 1, 0, 2};
        value = w < 4 ? kValues[w] : -1;
      } else if (v == 3) {
        static constexpr int64_t kValues[3] = {2, 0, 1};
        value = w < 3 ? kValues[w] : -1;
      } else {
        static constexpr int64_t kValues[4] = {2, 0, 1, 3};
        value = w < 4 ? kValues[w] : -1;
      }
    }
    if (value >= 0) {
      set(36712, uint32_t(value));
    }
  }
  set(36752, at(1808));
  set(36796, at(1812));
  {
    const uint32_t u = at(1816);
    if (u < 4) {
      set(33616, 3 - u);
    }
    const int32_t x = int32_t(Load(base, s + 33616));
    if (x == 0) {
      set(33620, 0);
    } else {
      const uint32_t v = at(1820);
      if (x == 1 && int32_t(v) == 3) {
        set(33620, 4);
      } else if (x == 1 && int32_t(v) == 4) {
        set(33620, 3);
      } else {
        set(33620, v);
      }
    }
  }
  // The five sticker layers (car c + 1824 + 12i, screen +33800 + 4i), the
  // tens from the frame's table (+80 to +92: 3, 2, 1, 0) indexed unchecked
  // by the layer's +8 - 1 (reading the mirror table's words past it).
  Store(base, frame + 80, 3);
  Store(base, frame + 84, 2);
  Store(base, frame + 88, 1);
  Store(base, frame + 92, 0);
  for (uint32_t i = 0; i < 5; i++) {
    const uint32_t e = c + 1824 + 12 * i;
    const uint32_t layer = s + 33800 + 4 * i;
    const uint32_t v = Load(base, e + 8);
    if (int32_t(v) == 0) {
      Store(base, layer, 0xFFFFFFFFu);
      continue;
    }
    auto tens = [&] { return Load(base, frame + 76 + v * 4) * 10; };
    const int32_t kind = int32_t(Load(base, e + 0));
    const uint32_t unit = Load(base, e + 4);
    if (kind == 1) {
      if (unit == 0) {
        Store(base, layer, tens() + 1);
      } else if (unit == 1) {
        Store(base, layer, tens());
      } else if (unit == 2) {
        Store(base, layer, tens() + 2);
      }
    } else if (kind == 2) {
      if (unit == 0) {
        Store(base, layer, tens() + 4);
      } else if (unit == 1) {
        Store(base, layer, tens() + 3);
      }
    }
  }
  // r3: the frame's +80 (r1's high word kept).
  return ((r1 & 0xFFFFFFFF00000000ull) | frame) + 80;
}

}  // namespace tuning_machine
