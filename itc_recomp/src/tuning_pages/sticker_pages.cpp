// The tuning screen's sticker editor (states 38 to 46 and 49 of
// TuningScreen::HandleInput), its movie sides and helpers
// (docs/functions/sticker-editor.md): see tuning_pages.h. None of it was
// seen in play; ported to leave as little recompiled as possible (the
// user's call, 2026-11-12), with the originals' loads and stores in their
// order and their quirks kept.

#include "tuning_pages/tuning_pages.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "guest.h"
#include "strings_and_font/steps.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>
#include <cmath>
#include <cstdint>

namespace tuning_pages {

namespace {

namespace fs = strings_and_font::steps;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

constexpr uint32_t kAll = 0xFFFFFFFFu;  // sub-scene -1: the scene itself
constexpr uint32_t kStickerMovie = 14;
constexpr uint32_t kTable = 0x8200B630;    // T: 61 words, 1 the sticker on the slot's second node
constexpr uint32_t kLayouts = 0x8200B728;  // 16 bytes a car id and area
constexpr uint32_t kHalf = 0x82001BE4;     // 0.5f
constexpr uint32_t kFirstNodeScale = 0x8200C548;

// The screen's fields.
constexpr uint32_t kArea = 30724;
constexpr uint32_t kSolo = 33500;
constexpr uint32_t kPreview = 33504;

// The players' action mapper entries (8 bytes each: the mapper, blocked
// when +4 is 1) and their indices (a word a player).
constexpr uint32_t kMapperEntries = 0x826BAB80;
constexpr uint32_t kControllerEntries = 0x826BABA4;
constexpr uint32_t kHeldSlot = 0x8210BE70;     // ActionMapper::Held (slot 8, +32)
constexpr uint32_t kPressedSlot = 0x8210BF40;  // ActionMapper::Pressed (slot 10, +40)
constexpr uint32_t kFiredSlot = 0x8210C010;    // ActionMapper::Fired (slot 12, +48)

// The player's mapper entry (0x826BAB80 + 8 × [0x826BABA4]).
uint32_t PlayerEntry(uint8_t* base) {
  return (Load(base, kControllerEntries) << 3) + kMapperEntries;
}

// The mapper's read slot at `offset` on `action`, as the original's bctrl
// from `frame` returning to `lr`: ActionMapper::Held's, Pressed's or Fired's
// plain C++ when the vtable holds it and `native_slots` (logged for
// --log_pause_menu as their adapters do), else the slot through `call`. Its
// r3.
uint64_t ReadSlot(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t mapper,
                  uint32_t offset, uint64_t action, uint32_t frame, uint32_t lr,
                  bool native_slots) {
  const uint32_t function = Load(base, Load(base, mapper) + offset);
  if (native_slots) {
    const uint32_t id = uint32_t(action);
    uint32_t r3;
    int slot;
    if (function == kHeldSlot) {
      r3 = action_mappers::Held(base, mapper, id);
      slot = 8;
    } else if (function == kPressedSlot) {
      r3 = action_mappers::Pressed(base, mapper, id);
      slot = 10;
    } else if (function == kFiredSlot) {
      r3 = action_mappers::Fired(base, mapper, id);
      slot = 12;
    } else {
      return call(function, {mapper, action}, frame, lr);
    }
    debug::PauseMenuLog_NoteSlotRead(slot, mapper, id, lr, double(r3));
    return r3;
  }
  return call(function, {mapper, action}, frame, lr);
}

// Whether the player's mapper reads `action` set on the slot at `offset`
// (skipped, false, while the entry's +4 is 1).
bool Asked(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t offset,
           uint64_t action, uint32_t frame, uint32_t lr, bool native_slots) {
  const uint32_t entry = PlayerEntry(base);
  if (int32_t(Load(base, entry + 4)) == 1) {
    return false;
  }
  return (ReadSlot(base, call, Load(base, entry), offset, action, frame, lr, native_slots) &
          0xFF) != 0;
}

// UiMovie::ShowNode and HideNode (sub_8211AC00, sub_8211AC48): the node's
// +80 set; the node.
uint32_t SetShown(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
                  uint32_t shown) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  Store(base, node + 80, shown);
  return node;
}

// T[id] is 1 (id below 61, signed; the word read at 0x8200B630 + 4 × id).
bool OnSecondNode(uint8_t* base, uint32_t id) {
  return int32_t(id) < 61 && int32_t(Load(base, kTable + (id << 2))) == 1;
}

// A doubleword at `at` (an int sign-extended through the stack for fcfid).
void StoreDoubleword(uint8_t* base, uint32_t at, int64_t value) {
  Store(base, at, uint32_t(uint64_t(value) >> 32));
  Store(base, at + 4, uint32_t(value));
}

// The hold toggles: `flag` 0 -> 1 with `action` held, 1 -> 0 without.
uint32_t UpdateHold(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t s,
                    uint32_t sp, bool native_slots, uint32_t action, uint32_t flag,
                    uint32_t lr) {
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t at = s + flag;
  if (Asked(base, call, 32, action, frame, lr, native_slots)) {
    if (int32_t(Load(base, at)) == 0) {
      Store(base, at, 1);
      return 1;
    }
  } else if (int32_t(Load(base, at)) == 1) {
    Store(base, at, 0);
    return 1;
  }
  return 0;
}

}  // namespace

uint32_t UpdateSoloHold(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                        uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821F7298 TuningScreen::UpdateSoloHold");
  return UpdateHold(base, call, screen, sp, native_slots, 164, kSolo, 0x821F72E8);
}

uint32_t UpdatePreviewHold(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                           uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821F7348 TuningScreen::UpdatePreviewHold");
  return UpdateHold(base, call, screen, sp, native_slots, 163, kPreview, 0x821F7398);
}

uint64_t StepUnlockedSticker(uint8_t* base, uint32_t screen, uint64_t id, uint64_t n) {
  ITC_NATIVE_RUNNING("sub_821F73F8 TuningScreen::StepUnlockedSticker");
  // The rest as the original works it: the quotient's low word (zero
  // extended) times 199 taken from all 64 bits of v.
  auto rest = [](uint64_t v) -> uint64_t {
    const int32_t v32 = int32_t(uint32_t(v));
    const uint64_t q = uint32_t(v32 / 199);
    return v - q * 199;
  };
  uint64_t v = id + 199;
  int32_t count = int32_t(uint32_t(n));
  uint64_t r5 = n;
  while (count != 0) {
    v += count < 0 ? uint64_t(-1) : 1;
    const uint64_t r = rest(v);
    bool owned = false;
    if (int32_t(uint32_t(r)) >= 0) {
      const int32_t i = int32_t(uint32_t(r));
      const uint32_t save = Load(base, screen + 18904);
      const uint32_t word = Load(base, uint32_t(((i >> 5) + 26218) << 2) + save);
      owned = (word & (1u << (i & 31))) != 0;
    }
    if (owned) {
      r5 += count < 0 ? 1 : uint64_t(-1);
      count = int32_t(uint32_t(r5));
    }
  }
  return rest(v);
}

uint32_t ShowSlotNode(uint8_t* base, uint32_t screen, uint32_t id, uint32_t j, uint32_t current,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8070 TuningScreen::ShowSlotNode");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, kStickerMovie));
  const uint32_t first = (j << 1) + 19, second = (j + 10) << 1;
  if (int32_t(Load(base, screen + kSolo)) != 0 && int32_t(j) != int32_t(current) &&
      int32_t(current) < 5) {
    SetShown(base, movie, kAll, first, 0);
    return SetShown(base, movie, kAll, second, 0);
  }
  if (OnSecondNode(base, id)) {
    SetShown(base, movie, kAll, first, 0);
    return SetShown(base, movie, kAll, second, 1);
  }
  SetShown(base, movie, kAll, first, 1);
  return SetShown(base, movie, kAll, second, 0);
}

uint32_t PlaceStickerNode(uint8_t* base, uint32_t screen, uint32_t id, uint32_t j,
                          uint32_t record, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8160 TuningScreen::PlaceStickerNode");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, kStickerMovie));
  // The selected garage car's layout for the area, copied into the frame.
  const uint32_t car = tuning_screen::SelectedGarageCar(base, screen);
  const uint32_t area = Load(base, screen + kArea);
  const uint32_t layout = (((Load(base, car + 224) << 3) + area) << 4) + kLayouts;
  const uint32_t words[4] = {Load(base, layout), Load(base, layout + 4), Load(base, layout + 8),
                             Load(base, layout + 12)};
  for (uint32_t k = 0; k < 4; k++) {
    Store(base, frame + 112 + 4 * k, words[k]);
  }
  const bool second = OnSecondNode(base, id);
  const uint32_t child = second ? (j + 10) << 1 : (j << 1) + 19;
  const uint32_t looked = ui_movies::FindNode(base, movie, kAll, child);
  const uint32_t node = ui_movies::FindNode(base, movie, kAll, child);
  const double scale = fs::Lfs(base, frame + 124);
  const int64_t ox = int32_t(Load(base, frame + 112) + (second ? 816 : 824));
  const int64_t oy = int32_t(Load(base, frame + 116) + (second ? 284 : 252));
  const int64_t x = int32_t(Load(base, record));
  const int64_t y = int32_t(Load(base, record + 4));
  fs::Stfs(base, node + 12, fs::Single(std::fma(fs::ToSingle(x), scale, fs::ToSingle(ox))));
  fs::Stfs(base, node + 16, fs::Single(std::fma(fs::ToSingle(y), scale, fs::ToSingle(oy))));
  // The conversions' doublewords as each path leaves them.
  if (second) {
    StoreDoubleword(base, frame + 80, oy);
    StoreDoubleword(base, frame + 88, ox);
  } else {
    StoreDoubleword(base, frame + 80, x);
    StoreDoubleword(base, frame + 88, ox);
  }
  StoreDoubleword(base, frame + 96, y);
  const double half = fs::Lfs(base, kHalf);
  Store(base, node + 20, uint32_t(fs::Fctiwz(fs::Single(fs::Lfs(base, looked + 100) * half))));
  Store(base, node + 24, uint32_t(fs::Fctiwz(fs::Single(fs::Lfs(base, looked + 104) * half))));
  fs::Stfs(base, node + 36, fs::Lfs(base, record + 8));
  const double size = fs::Lfs(base, frame + 120);
  if (second) {
    fs::Stfs(base, node + 28, fs::Single(size * fs::Lfs(base, record + 12)));
    fs::Stfs(base, node + 32, fs::Single(size * fs::Lfs(base, record + 12)));
  } else {
    const double factor = fs::Lfs(base, kFirstNodeScale);
    fs::Stfs(base, node + 28,
             fs::Single(fs::Single(size * fs::Lfs(base, record + 12)) * factor));
    fs::Stfs(base, node + 32,
             fs::Single(fs::Single(size * fs::Lfs(base, record + 12)) * factor));
  }
  Store(base, looked + 132, Load(base, record + 16));
  return node;
}

uint32_t SetSlot(uint8_t* base, uint32_t images, uint32_t sub, uint32_t first, uint32_t second,
                 uint32_t first_slot, uint32_t second_slot, uint32_t i, uint32_t flag,
                 uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8868 DecalImages::SetSlot");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, kStickerMovie));
  if (int32_t(i) < 0) {
    SetShown(base, movie, sub, first, 1);
    const uint32_t node = SetShown(base, movie, sub, second, 0);
    const uint32_t texture = Load(base, Load(base, movie + 28) + (first_slot << 2));
    Store(base, texture, Load(base, images + 7192));
    return node;
  }
  uint32_t node, slot;
  if (int32_t(flag) == 1) {
    SetShown(base, movie, sub, first, 1);
    node = SetShown(base, movie, sub, second, 0);
    slot = first_slot;
  } else {
    SetShown(base, movie, sub, first, 0);
    node = SetShown(base, movie, sub, second, 1);
    slot = second_slot;
  }
  const uint32_t texture = Load(base, Load(base, movie + 28) + (slot << 2));
  Store(base, texture, Load(base, i * 36 + images + 28));
  return node;
}

uint64_t SetStickerEditTexts(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F7F20 TuningScreen::SetStickerEditTexts");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, kStickerMovie));
  auto text = [&](uint32_t child, uint32_t id, uint64_t argument, uint32_t lr) {
    return call(tuning_screen::kSetText, {movie, kAll, child, id, argument, 0, 0, 0}, frame, lr);
  };
  text(41, 462, 0, 0x821F7F68);
  text(43, 463, 0, 0x821F7F7C);
  text(45, 457, 0, 0x821F7F90);
  text(46, 458, 0, 0x821F7FA4);
  text(47, 459, 0, 0x821F7FB8);
  text(36, 8, Load(base, screen + 30832), 0x821F7FD0);
  text(37, 8, Load(base, screen + 30836), 0x821F7FE8);
  // The angle truncated through the frame's +80 (fctiwz, stfiwx).
  Store(base, frame + 80, uint32_t(fs::Fctiwz(fs::Lfs(base, screen + 30840))));
  text(38, 8, Load(base, frame + 80), 0x821F8010);
  // The scale as a double (f1), its bits in r7 through the frame's +48.
  const double scale = fs::Lfs(base, screen + 30844);
  const uint64_t bits = std::bit_cast<uint64_t>(scale);
  Store(base, frame + 48, uint32_t(bits >> 32));
  Store(base, frame + 52, uint32_t(bits));
  text(39, 452, bits, 0x821F8030);
  return text(40, int32_t(Load(base, screen + 30848)) == 0 ? 1 : 0, 0, 0x821F8054);
}

}  // namespace tuning_pages
