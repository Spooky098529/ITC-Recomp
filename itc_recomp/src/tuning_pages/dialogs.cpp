// The confirm dialogs' yes answers and the stickers' records they store
// (docs/functions/tuning-pages.md): see tuning_pages.h. Single-precision steps
// as the originals' (each result rounded to a float); needs flush-to-zero off,
// as the originals' lfs leaves it.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"
#include "native/buffers.h"
#include "strings_and_font/steps.h"
#include "tuning_screen/tuning_screen.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <cmath>

namespace tuning_pages {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Byte(uint8_t* base, uint32_t address) { return guest::LoadU8(base, address); }
// lfs: a float made a double.
double Lfs(uint8_t* base, uint32_t address) {
  return double(std::bit_cast<float>(Load(base, address)));
}
// stfs: a double rounded to a float.
void Stfs(uint8_t* base, uint32_t address, double value) {
  Store(base, address, std::bit_cast<uint32_t>(float(value)));
}
// An int made a float (std, lfd, fcfid, frsp): the doubleword at `at`.
double IntToFloat(uint8_t* base, uint32_t at, int32_t value) {
  const uint64_t word = uint64_t(int64_t(value));
  Store(base, at, uint32_t(word >> 32));
  Store(base, at + 4, uint32_t(word));
  return double(float(double(int64_t(word))));
}

constexpr uint32_t kZero = 0x82000A34;        // 0.0
constexpr uint32_t kOne = 0x82000A30;         // 1.0
constexpr uint32_t kMinusOne = 0x82001BE0;    // probably -1.0
constexpr uint32_t kMirror = 0x82002244;      // StoreStickers' mirrored angle's offset
constexpr uint32_t kStoreScale = 0x82002948;  // the angle's scale
constexpr uint32_t kLoadScale = 0x8200294C;   // LoadStickers' angle scale
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kPickNext = 0x821159B0;        // Career::PickNext (its adapter)
constexpr uint32_t kHolderPointer = 0x826D83E0;   // the garage stage's holder

constexpr uint32_t kState = 9652;
constexpr uint32_t kCategory = 9656;
constexpr uint32_t kItem = 9660;
constexpr uint32_t kDepth = 10008;   // the state history's depth (a byte)
constexpr uint32_t kQueue = 10036;   // the command queue
constexpr uint32_t kMenu = 10040;    // the front-end menu
constexpr uint32_t kImages = 10048;  // the decal images

// memcpy(destination, source, 24) from `frame` with return address `lr`:
// Buffers_Memcpy's plain C++ where it takes it, else the guest's; r3 the
// destination.
uint64_t Copy24(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t destination,
                uint64_t source, uint32_t frame, uint32_t lr) {
  if (native::Buffers_Memcpy(base, uint32_t(destination), uint32_t(source), 24, lr)) {
    return destination;
  }
  return call(kMemcpy, {destination, source, 24}, frame, lr);
}

// The state history popped (its depth byte stepped down, wrapping) until the
// state +9652 it gives is `target` (no bound, as the original). Returns it.
uint32_t PopTo(uint8_t* base, uint32_t s, int32_t target) {
  uint32_t depth = Byte(base, s + kDepth);
  for (;;) {
    depth = (depth + 255) & 0xFF;
    guest::StoreU8(base, s + kDepth, uint8_t(depth));
    const uint32_t state = Load(base, s + (depth + 2439) * 4);
    Store(base, s + kState, state);
    if (int32_t(state) == target) {
      return state;
    }
  }
}

// The state `state` pushed on the history: its word at the depth, the depth
// up one.
void Push(uint8_t* base, uint32_t s, uint32_t state) {
  Store(base, s + (Byte(base, s + kDepth) + 2439) * 4, state);
}

// The queue [+10036] reset to `state`: +4, +0, +8 = state, +20 = +12 = -1,
// +24 = 0.
void Queue(uint8_t* base, uint32_t s, uint32_t state) {
  const uint32_t q = Load(base, s + kQueue);
  Store(base, q + 4, state);
  Store(base, q + 0, state);
  Store(base, q + 8, state);
  Store(base, q + 20, 0xFFFFFFFFu);
  Store(base, q + 12, 0xFFFFFFFFu);
  Store(base, q + 24, 0);
}

// The queue's command `command` then `next` for the state `current`: +8 = +0
// where +0 differs from it, +12, +16, +4 = current, +0 = -1.
void Then(uint8_t* base, uint32_t s, uint32_t current, uint32_t command, uint32_t next) {
  const uint32_t q = Load(base, s + kQueue);
  const uint32_t head = Load(base, q + 0);
  if (int32_t(current) != int32_t(head)) {
    Store(base, q + 8, head);
  }
  Store(base, q + 12, command);
  Store(base, q + 16, next);
  Store(base, q + 4, current);
  Store(base, q + 0, 0xFFFFFFFFu);
}

// Then for the state +9652 as it is now.
void ThenHere(uint8_t* base, uint32_t s, uint32_t command, uint32_t next) {
  const uint32_t q = Load(base, s + kQueue);
  const uint32_t current = Load(base, s + kState);
  const uint32_t head = Load(base, q + 0);
  if (int32_t(current) != int32_t(head)) {
    Store(base, q + 8, head);
  }
  Store(base, q + 12, command);
  Store(base, q + 16, next);
  Store(base, q + 4, current);
  Store(base, q + 0, 0xFFFFFFFFu);
}

// The menu [+10040] reset to `count` items with the cursor `cursor`.
void ResetMenu(uint8_t* base, uint32_t s, uint32_t count, uint32_t cursor) {
  const uint32_t m = Load(base, s + kMenu);
  Store(base, m + 28, 0);
  Store(base, m + 24, 0);
  Store(base, m + 64, 0);
  Store(base, m + 36, 0);
  Store(base, m + 60, 8);
  Store(base, m + 56, 0);
  Store(base, m + 44, 0);
  Store(base, m + 4, cursor);
  Store(base, m + 0, cursor);
  Store(base, m + 8, 0);
  Store(base, m + 20, 1);
  Store(base, m + 16, count - 1);
  Store(base, m + 12, 0);
}

// The menu's rows +48, +52 from the screen's +25100, +25104.
void MenuRows(uint8_t* base, uint32_t s) {
  const uint32_t first = Load(base, s + kMenu);
  Store(base, first + 48, Load(base, s + 25100));
  const uint32_t m = Load(base, s + kMenu);
  Store(base, m + 52, Load(base, s + 25104));
}

// A word of the screen at +offset + 4 x [+30492] (the light kit group, read
// again for each store, as the original does).
uint32_t GroupWord(uint8_t* base, uint32_t s, uint32_t index) {
  return s + (Load(base, s + 30492) + index) * 4;
}

// The light kits' defaults for group 3 (state 12's other groups).
void GroupThreeDefaults(uint8_t* base, uint32_t s) {
  Store(base, s + 30592, 0);
  Store(base, s + 30608, 180);
  Store(base, s + 30624, 180);
  Store(base, s + 30640, 180);
  Store(base, s + 30672, 0);
  Store(base, s + 30688, 0);
  Store(base, s + 30704, 180);
}

}  // namespace

void SwapKept(uint8_t* base, uint64_t images, uint64_t k, uint64_t i) {
  ITC_NATIVE_RUNNING("sub_821FE798 DecalImages::SwapKept");
  const uint32_t kept = uint32_t(k << 3) + uint32_t(images);
  const uint32_t entry = uint32_t(i * 36 + images);
  const uint32_t data = uint32_t((k + 905) << 3) + uint32_t(images);
  const uint32_t kept_header = Load(base, kept + 7236);
  const uint32_t header = Load(base, entry + 28);
  const uint32_t pixels = Load(base, entry + 32);
  Store(base, entry + 28, kept_header);
  Store(base, entry + 32, Load(base, data));
  Store(base, kept + 7236, header);
  Store(base, data, pixels);
}

uint32_t GarageDecal(uint8_t* base, uint32_t screen, uint32_t area, uint64_t slot) {
  ITC_NATIVE_RUNNING("sub_821F84C8 TuningScreen::GarageDecal");
  const uint32_t c = tuning_screen::SelectedGarageCar(base, screen) + 212;
  static constexpr uint32_t kBases[8] = {9, 19, 14, 24, 29, 34, 44, 39};
  if (area >= 8) {
    return 0;
  }
  return uint32_t((slot + kBases[area]) << 5) + c;
}

uint64_t SaveAreaDecals(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint64_t area, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F6CE8 TuningScreen::SaveAreaDecals");
  const uint32_t frame = sp - 112;
  Store(base, frame, sp);
  uint32_t id = uint32_t((area + 1610) * 20 + screen);
  uint64_t record = area * 120 + screen + 31240;
  uint64_t r3 = screen;
  for (int k = 0; k < 5; k++) {
    Store(base, id, Load(base, id + 1120));
    r3 = Copy24(base, call, record, record + 1120, frame, 0x821F6D28);
    id += 4;
    record += 24;
  }
  return r3;
}

uint64_t RestoreAreaDecals(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                           uint64_t area, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F6C90 TuningScreen::RestoreAreaDecals");
  const uint32_t frame = sp - 112;
  Store(base, frame, sp);
  uint32_t id = uint32_t((area + 1666) * 20 + screen);
  uint64_t record = area * 120 + screen + 32360;
  uint64_t r3 = screen;
  for (int k = 0; k < 5; k++) {
    Store(base, id, Load(base, id - 1120));
    r3 = Copy24(base, call, record, record - 1120, frame, 0x821F6CD0);
    id += 4;
    record += 24;
  }
  return r3;
}

uint64_t LoadStickers(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F86C0 TuningScreen::LoadStickers");
  namespace steps = strings_and_font::steps;
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = sp - 176;
  Store(base, frame, sp);
  const double offset = Lfs(base, kMirror);
  const double scale = Lfs(base, kLoadScale);
  const double minus = Lfs(base, kMinusOne);
  const uint32_t one = Load(base, kOne);
  const double zero = Lfs(base, kZero);
  const uint32_t zero_word = Load(base, kZero);
  uint32_t ids = s + 32200;
  uint32_t r = s + 31240;  // the record (the original's r31 its +8)
  uint64_t r3 = screen;
  for (uint32_t a = 0; a < 8; a++) {
    const bool mirrored = a == 2 || a == 7;
    for (uint32_t k = 0; k < 5; k++) {
      const uint32_t g = GarageDecal(base, s, a, k);
      Store(base, ids, Load(base, g + 24));
      Store(base, r + 8, zero_word);  // stfs of lfs: 0.0 and 1.0 as they are
      Store(base, r + 12, one);
      Store(base, r + 0, 0);
      Store(base, r + 4, 0);
      Store(base, r + 16, 0);
      Store(base, r + 20, 0);
      if (int32_t(Load(base, g + 24)) > -1) {
        double x = Lfs(base, g + 8);
        if (mirrored) {
          x = double(float(x * minus));
        }
        Store(base, r + 0, uint32_t(steps::Fctiwz(x)));
        Store(base, r + 4, uint32_t(steps::Fctiwz(Lfs(base, g + 12))));
        const double first = Lfs(base, g + 0);
        uint32_t flag;
        double angle;
        if (mirrored) {
          flag = first < zero ? 0 : 1;
          angle = double(float(Lfs(base, g + 16) * scale));
          angle = double(float(std::fma(angle, minus, offset)));
        } else {
          flag = first < zero ? 1 : 0;
          angle = double(float(Lfs(base, g + 16) * scale));
        }
        Stfs(base, r + 8, angle);
        // The original stores g+0 (times -1 with the flag) at +12 first and
        // then g+4 over it.
        Stfs(base, r + 12, flag == 1 ? double(float(Lfs(base, g + 0) * minus)) : Lfs(base, g + 0));
        Store(base, r + 16, flag);
        Stfs(base, r + 12, Lfs(base, g + 4));
      }
      ids += 4;
      r += 24;
    }
    r3 = RestoreAreaDecals(base, call, screen, a, frame);
  }
  return r3;
}

uint64_t ResetStickerEditor(uint8_t* base, uint64_t screen) {
  ITC_NATIVE_RUNNING("sub_821F8440 TuningScreen::ResetStickerEditor");
  const uint32_t s = uint32_t(screen);
  const uint32_t id = Load(base, tuning_screen::SelectedGarageCar(base, s) + 224);
  for (const uint32_t offset : {10016u, 30724u, 30732u, 30740u, 30748u, 30756u, 30768u, 30764u}) {
    Store(base, s + offset, 0);
  }
  Store(base, s + 33508, id == 0 ? 1 : 0);
  return screen;
}

uint64_t StoreStickers(uint8_t* base, uint64_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8598 TuningScreen::StoreStickers");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = sp - 112;
  Store(base, frame, sp);
  const double scale = Lfs(base, kStoreScale);
  const double minus = Lfs(base, kMinusOne);
  const double offset = Lfs(base, kMirror);
  uint32_t record = s + 31240;
  uint32_t ids = s + 32200;
  uint32_t g = 0;
  for (uint32_t a = 0; a < 8; a++) {
    const bool mirrored = a == 2 || a == 7;
    for (uint32_t k = 0; k < 5; k++) {
      g = GarageDecal(base, s, a, k);
      Store(base, g + 24, Load(base, ids));
      uint32_t x = Load(base, record);
      if (mirrored) {
        x = 0u - x;
      }
      Stfs(base, g + 8, IntToFloat(base, frame + 80, int32_t(x)));
      Stfs(base, g + 12, IntToFloat(base, frame + 88, int32_t(Load(base, record + 4))));
      double angle = Lfs(base, record + 8);
      uint32_t flag = Load(base, record + 16);
      if (mirrored) {
        angle = double(float(angle - offset));
        flag = flag == 0 ? 1 : 0;
        angle = double(float(angle * minus));
      }
      Stfs(base, g + 16, double(float(angle * scale)));
      double first = Lfs(base, record + 12);
      if (int32_t(flag) == 1) {
        first = double(float(first * minus));
      }
      Stfs(base, g + 0, first);
      Stfs(base, g + 4, Lfs(base, record + 12));
      ids += 4;
      record += 24;
    }
  }
  return g;
}

uint64_t OnStickerDialogYes(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                            uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FA230 TuningScreen::OnStickerDialogYes");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = sp - 96;
  Store(base, frame, sp);
  const int32_t state = int32_t(Load(base, s + kState));
  uint64_t r3 = screen;
  if (state == 52) {
    // Delete a sticker: back to state 39, the area's slot emptied.
    Store(base, s + 9708, 1);
    Store(base, s + 9720, 1);
    Queue(base, s, 52);
    const uint32_t q = Load(base, s + kQueue);
    Store(base, s + kState, 39);
    const uint32_t head = Load(base, q + 0);
    if (int32_t(head) != 39) {
      Store(base, q + 8, head);
    }
    Store(base, q + 4, 39);
    Store(base, q + 0, 0xFFFFFFFFu);
    const double zero = Lfs(base, kZero);
    Store(base, q + 16, 34);
    const double one = Lfs(base, kOne);
    Store(base, q + 12, 53);
    uint32_t a = Load(base, s + 30724);
    uint32_t k = Load(base, s + 30732);
    Store(base, ((a + 1666) * 5 + k) * 4 + s, 0xFFFFFFFFu);
    a = Load(base, s + 30724);
    k = Load(base, s + 30732);
    const uint32_t r = (a * 5 + k) * 24 + s;
    Stfs(base, r + 32368, zero);
    Store(base, r + 32360, 0);
    Stfs(base, r + 32372, one);
    Store(base, r + 32364, 0);
    Store(base, r + 32376, 0);
    Store(base, r + 32380, 0);
  } else if (state == 50 || state == 51) {
    // Keep (50) or discard (51) the area: back to state 38, the stand's
    // decals applied again.
    Store(base, s + 9708, 1);
    Store(base, s + 9720, 1);
    if (state == 50) {
      SaveAreaDecals(base, call, screen, Load(base, s + 30724), frame);
      StoreStickers(base, screen, frame);
      call(kPickNext, {vehicle_accessors::SaveDataOf(base, 0), 1}, frame, 0x821FA360);
      const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
      r3 = save;
      for (uint32_t i = 0; i < 5; i++) {
        Store(base, s + 20792 + 4 * i, Load(base, save + 8 + 4 * i));
      }
    }
    const uint32_t q = Load(base, s + kQueue);
    Store(base, s + kState, 38);
    Store(base, q + 4, 38);
    Store(base, q + 0, 38);
    Store(base, q + 8, 38);
    Store(base, q + 20, 0xFFFFFFFFu);
    Store(base, q + 12, 0xFFFFFFFFu);
    Store(base, q + 24, 0);
    ThenHere(base, s, 53, 32);
    Store(base, s + 10012, 2);
  }
  return r3;
}

uint64_t OnDialogYes(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                     uint32_t sp, bool native_specs, const StandFit& fit) {
  ITC_NATIVE_RUNNING("sub_821E2660 TuningScreen::OnDialogYes");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = sp - 112;
  Store(base, frame, sp);
  const int32_t state = int32_t(Load(base, s + kState));
  const auto category = [&] { return int32_t(Load(base, s + kCategory)); };
  const auto item = [&] { return Byte(base, s + kItem); };
  const auto set_choice = [&] { SetCurrentChoice(base, call, screen, frame, native_specs); };
  const auto copy = [&](uint32_t from, uint32_t to) {
    uint32_t words[4];
    for (uint32_t i = 0; i < 4; i++) {
      words[i] = Load(base, s + from + 4 * i);
    }
    for (uint32_t i = 0; i < 4; i++) {
      Store(base, s + to + 4 * i, words[i]);
    }
  };
  if (state == 9) {
    // CONF BUY: the price paid, the pick owned, CONF EQUIP's page next.
    const uint32_t m = Load(base, s + kMenu);
    const uint32_t price = Load(base, s + 25120);
    const uint32_t money = Load(base, s + 25116) - price;
    const uint32_t cursor = Load(base, m + 0);
    Store(base, s + 25116, money);
    Store(base, s + 9748, cursor);
    guest::StoreU8(base, s + 9665, uint8_t(cursor));
    MarkOwned(base, s);
    const uint32_t depth = Byte(base, s + kDepth);
    guest::StoreU8(base, s + 9665, 0);
    guest::StoreU8(base, s + 9666, 0);
    Store(base, s + 9708, 1);
    Store(base, s + (depth + 2439) * 4, 9);
    const uint32_t up = Byte(base, s + kDepth) + 1;
    const uint32_t q = Load(base, s + kQueue);
    Store(base, s + kState, 11);
    guest::StoreU8(base, s + kDepth, uint8_t(up));
    Store(base, q + 4, 11);
    Store(base, q + 0, 11);
    Store(base, q + 8, 11);
    Store(base, q + 20, 0xFFFFFFFFu);
    Store(base, q + 12, 0xFFFFFFFFu);
    Store(base, q + 24, 0);
    const uint32_t pick = Byte(base, s + 9665);
    const uint32_t menu = Load(base, s + kMenu);
    Store(base, menu + 12, 0);
    Store(base, menu + 16, 1);
    Store(base, menu + 20, 1);
    Store(base, menu + 8, 0);
    Store(base, menu + 0, pick);
    Store(base, menu + 4, pick);
    Store(base, menu + 44, 0);
    Store(base, menu + 56, 0);
    Store(base, menu + 60, 8);
    Store(base, menu + 36, 0);
    Store(base, menu + 64, 0);
    Store(base, menu + 24, 0);
    Store(base, menu + 28, 0);
    const uint32_t q2 = Load(base, s + kQueue);
    const uint32_t current = Load(base, s + kState);
    const uint32_t head = Load(base, q2 + 0);
    if (int32_t(current) != int32_t(head)) {
      Store(base, q2 + 8, head);
    }
    Store(base, q2 + 0, current);
    Store(base, q2 + 12, 0xFFFFFFFFu);
    Store(base, q2 + 16, 0xFFFFFFFFu);
  } else if (state == 11) {
    // CONF EQUIP: the pick made current, back to its list.
    if (!(category() == 6 && item() == 3 && int32_t(Load(base, s + 33540)) == 1)) {
      set_choice();
    }
    const int32_t c = category();
    Store(base, s + 9732, 1);
    if (c == 2) {
      PopTo(base, s, 3);
    } else if (c == 5 && item() == 0) {
      PopTo(base, s, 15);
      Store(base, s + 25204, 0);
      Store(base, s + 25208, 0);
    } else if (c == 6 && item() == 3 && int32_t(Load(base, s + 33540)) == 0) {
      PopTo(base, s, 53);
    } else if (c == 6 && item() == 3 && int32_t(Load(base, s + 33540)) == 1) {
      Store(base, s + 9732, 0);
      Store(base, s + 33576, 0);
      Push(base, s, 11);
      const uint32_t up = Byte(base, s + kDepth) + 1;
      Store(base, s + kState, 57);
      guest::StoreU8(base, s + kDepth, uint8_t(up));
    } else {
      PopTo(base, s, 4);
    }
    const uint32_t q = Load(base, s + kQueue);
    const uint32_t current = Load(base, s + kState);
    Store(base, s + 9708, 1);
    Store(base, s + 9716, 1);
    Store(base, q + 20, 0xFFFFFFFFu);
    Store(base, q + 4, current);
    Store(base, q + 0, current);
    Store(base, q + 8, current);
    Store(base, q + 12, 0xFFFFFFFFu);
    Store(base, q + 24, 0);
    const int32_t c2 = category();
    const bool menu = !((c2 == 5 && item() == 0) || (c2 == 6 && item() == 3)) || c2 == 4;
    if (menu) {
      const uint32_t count = PageItemCount(base, s);
      const uint32_t cursor = PageCursor(base, s);
      ResetMenu(base, s, count, cursor);
    }
    if (category() == 4) {
      MenuRows(base, s);
    }
    const uint32_t now = Load(base, s + kState);
    switch (int32_t(now)) {
      case 3: Then(base, s, now, 64, 0); break;
      case 4: Then(base, s, now, 64, 2); break;
      case 15: Then(base, s, now, 64, 7); break;
      case 53: Then(base, s, now, 64, 54); break;
      case 56: Then(base, s, now, 64, 57); break;
      case 57: Then(base, s, now, 64, 58); break;
      default: break;
    }
  } else if (state == 12 || state == 13) {
    // Keep (12) or discard (13) the paint or the light kit.
    Store(base, s + 9732, 1);
    Store(base, s + 9708, 1);
    Store(base, s + 9716, 1);
    if (category() == 5) {
      const uint32_t it = item();
      if (it == 4) {
        PopTo(base, s, 4);
        Queue(base, s, 4);
        ThenHere(base, s, 64, 2);
        if (state == 12) {
          copy(30444, 30476);
        } else {
          copy(30476, 30444);
        }
      } else if (it == 5) {
        const int32_t kind = int32_t(Load(base, s + 30500));
        if (kind == 0 || kind == 1) {
          PopTo(base, s, 19);
          Queue(base, s, 19);
          ThenHere(base, s, 64, 11);
          if (state == 12 && kind == 0) {
            if (int32_t(Load(base, s + 30492)) == 3) {
              for (uint32_t i = 0; i < 4; i++) {
                const uint32_t at = s + 30660 + 4 * i;
                Store(base, at - 80, 1);
                Store(base, at, 0);
                Store(base, at - 64, Load(base, s + 30512));
                Store(base, at - 48, Load(base, s + 30516));
                Store(base, at - 32, Load(base, s + 30520));
                Store(base, at - 16, Load(base, s + 30524));
                Store(base, at + 48, Load(base, s + 30524));
              }
            } else {
              Store(base, GroupWord(base, s, 7645), 1);
              Store(base, GroupWord(base, s, 7665), 0);
              {
                const uint32_t at = GroupWord(base, s, 7649);
                Store(base, at, Load(base, s + 30512));
              }
              {
                const uint32_t at = GroupWord(base, s, 7653);
                Store(base, at, Load(base, s + 30516));
              }
              {
                const uint32_t at = GroupWord(base, s, 7657);
                Store(base, at, Load(base, s + 30520));
              }
              GroupThreeDefaults(base, s);
              for (uint32_t i = 0; i < 4; i++) {
                Store(base, s + 30644 + 4 * i, Load(base, s + 30524));
                Store(base, s + 30708 + 4 * i, Load(base, s + 30524));
              }
            }
          } else if (state == 12) {
            if (int32_t(Load(base, s + 30492)) == 3) {
              for (uint32_t i = 0; i < 4; i++) {
                const uint32_t at = s + 30580 + 4 * i;
                Store(base, at + 80, 1);
                Store(base, at, 0);
                Store(base, at + 96, Load(base, s + 30548));
                Store(base, at + 112, Load(base, s + 30552));
                Store(base, at + 64, Load(base, s + 30556));
                Store(base, at + 128, Load(base, s + 30556));
              }
            } else {
              Store(base, GroupWord(base, s, 7665), 1);
              Store(base, GroupWord(base, s, 7645), 0);
              {
                const uint32_t at = GroupWord(base, s, 7669);
                Store(base, at, Load(base, s + 30548));
              }
              {
                const uint32_t at = GroupWord(base, s, 7673);
                Store(base, at, Load(base, s + 30552));
              }
              GroupThreeDefaults(base, s);
              for (uint32_t i = 0; i < 4; i++) {
                Store(base, s + 30644 + 4 * i, Load(base, s + 30556));
                Store(base, s + 30708 + 4 * i, Load(base, s + 30556));
              }
            }
          }
        }
      }
    }
  } else if (state == 14) {
    // Remove a part.
    Store(base, s + 9732, 1);
    Store(base, s + 9708, 1);
    Store(base, s + 9716, 1);
    const int32_t c = category();
    bool done = false;
    if (c == 4) {
      const uint32_t it = item();
      const uint32_t type = Byte(base, s + 9662);
      if ((it == 3 && type == 1) || (it == 9 && type == 0)) {
        PopTo(base, s, 4);
        Queue(base, s, 4);
        const uint32_t count = PageItemCount(base, s);
        const uint32_t cursor = PageCursor(base, s);
        const uint32_t m = Load(base, s + kMenu);
        Store(base, m + 12, 0);
        Store(base, m + 16, count - 1);
        Store(base, m + 20, 1);
        Store(base, m + 0, cursor);
        Store(base, m + 4, cursor);
        Store(base, m + 8, 0);
        Store(base, m + 44, 0);
        Store(base, m + 56, 0);
        Store(base, m + 60, 8);
        Store(base, m + 36, 0);
        Store(base, m + 64, 0);
        Store(base, m + 24, 0);
        Store(base, m + 28, 0);
        MenuRows(base, s);
        ThenHere(base, s, 64, 2);
        set_choice();
        done = true;
      }
    }
    if (!done && c == 5 && item() == 5) {
      PopTo(base, s, 19);
      Queue(base, s, 19);
      ThenHere(base, s, 64, 11);
      if (int32_t(Load(base, s + 30492)) == 3) {
        for (uint32_t i = 0; i < 4; i++) {
          const uint32_t at = s + 30596 + 4 * i;
          Store(base, at - 16, 0);
          Store(base, at + 0, 180);
          Store(base, at + 16, 180);
          Store(base, at + 32, 180);
          Store(base, at + 48, 0);
          Store(base, at + 64, 0);
          Store(base, at + 80, 0);
          Store(base, at + 96, 180);
          Store(base, at + 112, 0);
        }
      } else {
        Store(base, GroupWord(base, s, 7645), 0);
        Store(base, GroupWord(base, s, 7649), 180);
        Store(base, GroupWord(base, s, 7653), 180);
        Store(base, GroupWord(base, s, 7657), 180);
        Store(base, GroupWord(base, s, 7665), 0);
        Store(base, GroupWord(base, s, 7669), 0);
        Store(base, GroupWord(base, s, 7673), 180);
      }
    } else if (!done && c == 6 && item() == 3) {
      const int32_t which = int32_t(Load(base, s + 33540));
      if (which == 0) {
        PopTo(base, s, 53);
        Queue(base, s, 53);
        ThenHere(base, s, 64, 54);
        set_choice();
      } else if (which == 1) {
        PopTo(base, s, 56);
        Queue(base, s, 56);
        ThenHere(base, s, 64, 57);
        Store(base, s + (Load(base, s + 33576) + 8450) * 4, 0xFFFFFFFFu);
      }
    }
  } else if (state == 35 || state == 36) {
    // The vinyl editor: discard (35) or keep (36) the page's layers.
    Store(base, s + 9708, 1);
    PopTo(base, s, 23);
    Queue(base, s, 23);
    const bool vinyls = category() == 5 && item() == 6;
    {
      const uint32_t q = Load(base, s + kQueue);
      const uint32_t current = Load(base, s + kState);
      const uint32_t head = Load(base, q + 0);
      if (int32_t(current) != int32_t(head)) {
        Store(base, q + 8, head);
      }
      Store(base, q + 0, 0xFFFFFFFFu);
      Store(base, q + 4, current);
      Store(base, q + 16, vinyls ? 15 : 32);
      Store(base, q + 12, vinyls ? 31 : 53);
    }
    if (category() == 5 && item() == 6) {
      const uint32_t live_id = 7792, saved_id = 7798;
      const uint32_t live = 30880, saved = 31024;
      const uint32_t from_id = state == 35 ? saved_id : live_id;
      const uint32_t to_id = state == 35 ? live_id : saved_id;
      const uint32_t from = state == 35 ? saved : live;
      const uint32_t to = state == 35 ? live : saved;
      const uint32_t lr = state == 35 ? 0x821E35E4 : 0x821E3768;
      for (uint32_t j = 0; j < 3; j++) {
        uint64_t n = uint64_t(Load(base, s + 30724)) * 3 + j;
        Store(base, s + uint32_t(n + to_id) * 4, Load(base, s + uint32_t(n + from_id) * 4));
        n = uint64_t(Load(base, s + 30724)) * 3 + j;
        const uint64_t at = n * 24 + screen;
        Copy24(base, call, at + to, at + from, frame, lr);
      }
      if (state == 36) {
        const int32_t page = int32_t(Load(base, s + 30724));
        Store(base, s + 18900, 1);
        const uint64_t images = screen + kImages;
        if (page == 0 || page == 1) {
          static constexpr uint32_t kSwaps[2][6][2] = {
              {{0, 0}, {1, 1}, {2, 2}, {3, 6}, {4, 7}, {5, 8}},
              {{15, 21}, {16, 22}, {17, 23}, {18, 27}, {19, 28}, {20, 29}}};
          for (const auto& [k, i] : kSwaps[page]) {
            SwapKept(base, images, k, i);
          }
          for (const auto& [k, i] : kSwaps[page]) {
            BindVinylTexture(base, images, i, uint32_t(page), frame);
          }
        }
      }
    }
    StoreVinyls(base, screen, frame);
    Store(base, s + 10012, 6);
    const uint32_t c = tuning_screen::SelectedGarageCar(base, s);
    const uint32_t holder = Load(base, Load(base, kHolderPointer));
    const int64_t a = int8_t(Byte(base, c + 300));
    const int64_t b = int8_t(Byte(base, c + 301));
    FitSelectedCar(base, call, fit, holder, uint64_t(a), uint64_t(b), frame, 0x821E3984);
  } else if (state == 37) {
    // Delete a vinyl layer (item 6) or leave the sticker list (item 7).
    if (category() == 5) {
      const uint32_t it = item();
      if (it == 6 || it == 7) {
        Store(base, s + 9708, 1);
        PopTo(base, s, 24);
        if (it == 6) {
          Queue(base, s, 24);
          ThenHere(base, s, 31, 16);
        }
        if (category() == 5 && item() == 6) {
          const uint32_t layer = Load(base, s + 30732);
          const double zero = Lfs(base, kZero);
          const double one = Lfs(base, kOne);
          Store(base, s + (Load(base, s + 30724) * 3 + layer + 7798) * 4, 0xFFFFFFFFu);
          const uint32_t page = Load(base, s + 30724);
          const uint32_t r = (page * 3 + Load(base, s + 30732)) * 24 + s;
          Stfs(base, r + 31032, zero);
          Store(base, r + 31024, 0);
          Stfs(base, r + 31036, one);
          Store(base, r + 31028, 0);
          Store(base, r + 31040, 0);
          Store(base, r + 31044, 0);
        }
      }
    }
  }
  return OnStickerDialogYes(base, call, screen, frame);
}

}  // namespace tuning_pages
