// The current choices the confirm dialogs' yes answers write
// (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "native/buffers.h"
#include "tuning_screen/tuning_screen.h"

namespace tuning_pages {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Byte(uint8_t* base, uint32_t address) { return guest::LoadU8(base, address); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

constexpr uint32_t kGetCarParts = 0x820C9E00;  // Params::GetCarParts(out, car)
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kGet82 = 0x820CBC90;  // AeroParts::Get82(out, parts, level)

}  // namespace

uint64_t SetAeroAvailability(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                             uint32_t sp, bool native_specs) {
  ITC_NATIVE_RUNNING("sub_821DE160 TuningScreen::SetAeroAvailability");
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t car = Load(base, tuning_screen::SelectedGarageCar(base, screen) + 224);
  const uint32_t parts_out = frame + 128, parts = frame + 80;
  uint32_t parts_at = parts_out;
  if (native_specs) {
    uint32_t words[9];
    car_specs::GetCarParts(base, car, words);
    for (uint32_t i = 0; i < 9; i++) {
      Store(base, parts_out + 4 * i, words[i]);
    }
  } else {
    parts_at = uint32_t(call(kGetCarParts, {parts_out, car}, frame, 0x821DE1C0));
  }
  uint64_t r3 = parts;
  if (!native::Buffers_Memcpy(base, parts, parts_at, 36, 0x821DE1D0)) {
    r3 = call(kMemcpy, {parts, parts_at, 36}, frame, 0x821DE1D0);
  }
  const uint32_t t = Load(base, screen + 36588);
  if (t == 0) {
    for (uint32_t item = 0; item < 10; item++) {
      Store(base, screen + 25128 + 4 * item, 1);
    }
    return r3;
  }
  uint32_t kit = parts_out;
  bool done = false;
  if (native_specs) {
    uint32_t words[9];
    for (uint32_t i = 0; i < 9; i++) {
      words[i] = Load(base, parts + 4 * i);
    }
    done = car_specs::AeroPart82(base, parts_out, words, int32_t(t));
  }
  r3 = parts_out;
  if (!done) {
    r3 = call(kGet82, {parts_out, parts, t}, frame, 0x821DE1F0);
    kit = uint32_t(r3);
  }
  const uint32_t w = Load(base, kit + 24);
  Store(base, screen + 25128, 0);
  Store(base, screen + 25132, 0);
  Store(base, screen + 25136, 0);
  Store(base, screen + 25148, 1);
  Store(base, screen + 25152, 1);
  Store(base, screen + 25156, 1);
  Store(base, screen + 25160, 0);
  Store(base, screen + 25164, 1);
  Store(base, screen + 25144, (w & 1) != 0 ? 1 : 0);
  Store(base, screen + 25140, (w & 2) != 0 ? 1 : 0);
  return r3;
}

uint64_t SetCurrentChoice(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3,
                          uint32_t sp, bool native_specs) {
  ITC_NATIVE_RUNNING("sub_821E1DB0 TuningScreen::SetCurrentChoice");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  const int32_t category = int32_t(Load(base, s + 9656));
  const uint32_t item = Byte(base, s + 9660);
  const uint32_t l = Byte(base, s + 9661);
  auto set = [&](uint32_t offset, uint32_t value) { Store(base, s + offset, value); };
  // A word of the table at `offset` by [+index]: (index + offset / 4) × 4,
  // 32 bits.
  auto indexed = [&](uint32_t index_at, uint32_t offset, uint32_t value) {
    Store(base, s + ((Load(base, s + index_at) + offset / 4) << 2), value);
  };
  switch (category) {
    case 0:
      if (item == 0) {
        indexed(33900, 34304, l);
      } else if (item == 1) {
        indexed(33900, 34804, l);
      } else if (item == 2) {
        set(35024, Byte(base, s + 9662));
        set(35020, l);
      } else if (item == 3) {
        set(35140, l);
      }
      break;
    case 1:
      if (item == 0) {
        set(35228, l);
      } else if (item == 1) {
        set(35316, l);
      } else if (item == 2) {
        set(35404, l);
      } else if (item == 3) {
        if (l == 0) {
          set(35496, 0);
          set(35492, 0);
        } else {
          set(35492, l);
          set(35496, Byte(base, s + 9663));
        }
      } else if (item == 4) {
        set(35608, l);
      }
      break;
    case 2:
      set(35708, l);
      break;
    case 3: {
      const uint32_t o = Byte(base, s + 9664);
      if (item == 0) {
        set(35820, l);
        set(35824, o);
        set(35948, l);
        set(35952, o);
        set(36076, l);
        set(36080, o);
        return 36080;
      }
      if (item == 1) {
        set(35820, 0);
        set(35824, 0);
        set(35948, l);
        set(35952, o);
      } else if (item == 2) {
        set(35820, 0);
        set(35824, 0);
        set(36076, l);
        set(36080, o);
      }
      break;
    }
    case 4: {
      // A jump table on the item; case 0 the split piece sub_821E203C
      // (merged: the same store, then this function's epilogue).
      const uint32_t t = Byte(base, s + 9662);
      static constexpr uint32_t kOffsets[10] = {36164, 36208, 36252, 36300, 36368,
                                                36436, 36472, 36508, 36544, 36588};
      if (item > 9) {
        break;
      }
      set(kOffsets[item], t);
      if (item == 4) {
        if (t == 0) {
          set(36376, 0);
          set(36372, 0);
        } else {
          set(36376, Byte(base, s + 9664));
          set(36372, Byte(base, s + 9663));
        }
      } else if (item == 9) {
        return SetAeroAvailability(base, call, s, frame, native_specs);
      }
      break;
    }
    case 5:
      if (item == 0) {
        const uint32_t both = Load(base, s + 25196);
        if (both == 0) {
          uint64_t number = 0;
          for (uint32_t i = 0; i < 3; i++) {
            number = tuning_screen::WheelNumber(Load(base, s + 25204), Load(base, s + 25220));
            set(27832 + 4 * i, uint32_t(number));
            set(27844 + 4 * i, Load(base, s + 25228));
          }
          return number;
        }
        set(27832, 0);
        set(27844, 0);
        const uint64_t number =
            tuning_screen::WheelNumber(Load(base, s + 25204), Load(base, s + 25220));
        Store(base, s + ((both + 6958) << 2), uint32_t(number));
        const uint32_t wheel = Load(base, s + 25228);
        Store(base, s + ((Load(base, s + 25196) + 6961) << 2), wheel);
        return number;
      }
      if (item == 1) {
        set(36612, Byte(base, s + 9664));
      } else if (item == 2) {
        set(36636, Byte(base, s + 9662));
      } else if (item == 3) {
        set(36668, Byte(base, s + 9662));
      }
      break;
    case 6:
      if (item == 0) {
        set(36712, Byte(base, s + 9663));
        set(36708, Byte(base, s + 9662));
      } else if (item == 1) {
        set(36752, Byte(base, s + 9662));
      } else if (item == 2) {
        set(36796, Byte(base, s + 9662));
      } else if (item == 3) {
        const int32_t mode = int32_t(Load(base, s + 33540));
        if (mode == 0) {
          const uint32_t a = Load(base, s + 33548);
          const uint32_t b = Load(base, s + 33556);
          set(33616, a);
          set(33620, b);
        } else if (mode == 1) {
          const uint32_t code = Load(base, s + 33564) * 10 + Load(base, s + 33572);
          indexed(33576, 33800, code);
        }
      }
      break;
    default:
      break;
  }
  return r3;
}

}  // namespace tuning_pages
