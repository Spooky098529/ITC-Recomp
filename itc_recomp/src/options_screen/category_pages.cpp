// The options screen's category pages (docs/functions/options-screen.md):
// see options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "guest.h"
#include "native/buffers.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace options_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kMinusOne = 0xFFFFFFFFu;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

uint32_t Settings(uint8_t* base) { return LoadU32(base, kSettings); }
uint32_t Menu(uint8_t* base) { return LoadU32(base, kMenu); }

// The page block to `step` with `after` the page after and `queued` the
// step queued (the page before kept when the page changes).
void Go(uint8_t* base, uint32_t after, uint32_t step, uint32_t queued) {
  const uint32_t p = LoadU32(base, kPage);
  const uint32_t page = LoadU32(base, p + 0);
  if (page != after) {
    StoreU32(base, p + 8, page);
  }
  StoreU32(base, p + 16, queued);
  StoreU32(base, p + 4, after);
  StoreU32(base, p + 0, kMinusOne);
  StoreU32(base, p + 12, step);
}

// `count` words from `from` to `to` (the originals' inline copies).
void CopyWords(uint8_t* base, uint32_t to, uint32_t from, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    StoreU32(base, to + 4 * i, LoadU32(base, from + 4 * i));
  }
}

// memcpy(to, from, bytes) from `frame` returning to `lr`: its plain C++
// where it takes it (r3 the destination), else the guest's.
uint64_t Copy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t frame,
              uint32_t lr, const Calls& calls) {
  if (native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    return to;
  }
  return calls.Call(kMemcpy, frame, lr, to, from, bytes);
}

uint64_t Sound(const Calls& calls, uint32_t frame, uint32_t lr, uint64_t sound) {
  return calls.Call(kMenuSoundPlay, frame, lr, sound);
}

bool Pressed(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr, uint32_t action,
             uint64_t& r3) {
  r3 = calls.Call(kMenuPressed, frame, lr, Menu(base), action);
  return uint32_t(r3) != 0;
}

// The page block to `page` with no step (pages 11 to 13, the music
// preview's and the sound page's leave).
void ToPage(uint8_t* base, uint32_t page) {
  const uint32_t p = LoadU32(base, kPage);
  const uint32_t current = LoadU32(base, p + 0);
  if (current != page) {
    StoreU32(base, p + 8, current);
  }
  StoreU32(base, p + 12, kMinusOne);
  StoreU32(base, p + 0, page);
  StoreU32(base, p + 16, kMinusOne);
}

// The volumes as SoundOptions::SetEffectsVolume and SetMusicVolume take
// them: the setting's low byte sign-extended.
uint64_t Setting(uint8_t* base, uint32_t field) {
  return uint64_t(int64_t(int8_t(LoadU32(base, Settings(base) + field) & 0xFF)));
}

// The sound manager's slot at `slot` (bytes into its vtable) with r4 and r5
// the arguments' addresses (r5 0 where the original leaves it unset).
uint64_t Slot(uint8_t* base, const Calls& calls, uint32_t slot, uint32_t frame, uint32_t lr,
              uint64_t r4, uint64_t r5 = 0) {
  const uint32_t manager = LoadU32(base, kSoundManagerGlobal);
  return calls.Call(LoadU32(base, LoadU32(base, manager) + slot), frame, lr, manager, r4, r5);
}

constexpr uint32_t kReleaseBank = 20;    // slot 5
constexpr uint32_t kIsBankSlotEmpty = 32;  // slot 8
constexpr uint32_t kStopCue = 52;        // slot 13
constexpr uint32_t kIsCuePlaying = 56;   // slot 14
constexpr uint32_t kSoundPlay = 0x82175850;  // Sound::Play, through its override

// The category pages' set-ups: the menu's words (the cursor 0, `last` the
// last item, wrapping; the words the originals clear), as they store them.
void SetUpMenu(uint8_t* base, uint32_t last) {
  const uint32_t menu = Menu(base);
  StoreU32(base, menu + 12, 0);
  StoreU32(base, menu + 16, last);
  StoreU32(base, menu + 20, 1);
  for (const uint32_t at : {0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    StoreU32(base, menu + at, 0);
  }
}

// The movie's text calls from a set-up's frame: SetNodeText(m, node, id)
// and SetText(m, sub, child, id) (their unset varargs passed as 0, the
// user's call, 2026-10-30), ScrollText(m, 2, 0, 1) on the help line.
struct Texts {
  const Calls& calls;
  uint32_t frame;
  uint32_t m;
  void Node(uint32_t lr, uint32_t node, uint64_t id) const {
    calls.Call(kSetNodeText, frame, lr, m, node, id, 0);
  }
  void Text(uint32_t lr, uint64_t sub, uint64_t child, uint64_t id) const {
    calls.Call(kSetText, frame, lr, m, sub, child, id);
  }
  uint64_t ScrollHelp(uint32_t lr) const { return calls.Call(kScrollText, frame, lr, m, 2, 0, 1); }
  // The OK/Default row's two texts (strings 4 and 7) on `node`'s children
  // 3 and 4.
  void OkDefault(uint8_t* base, uint32_t node, uint32_t lr3, uint32_t lr4) const {
    Node(lr3, ui_movies::Child(base, node, 3), 4);
    Node(lr4, ui_movies::Child(base, node, 4), 7);
  }
};

// The category table (5 contexts of 7 words) entry `index` of row `context`,
// at the 32-bit address the originals compute.
uint32_t Category(uint8_t* base, uint32_t context, uint32_t index) {
  return LoadU32(base, kCategories + ((context * 7 + index) << 2));
}

}  // namespace

uint64_t HudPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C1270 Options::HudPage");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMenuStep, frame, 0x821C128C, Menu(base));
  uint64_t r3;
  if (LoadU32(base, Menu(base) + 8) != 0) {
    r3 = Sound(calls, frame, 0x821C12A4, 4);
    Go(base, 1, 10, kMinusOne);
    return r3;
  }
  if (uint32_t(StepHud(base, frame)) != 0) {
    r3 = Sound(calls, frame, 0x821C12E8, 4);
    Go(base, 1, 11, kMinusOne);
    return r3;
  }
  if (Pressed(base, calls, frame, 0x821C1314, 159, r3)) {
    r3 = Sound(calls, frame, 0x821C1324, 0);
    const uint32_t menu = Menu(base);
    const int32_t cursor = int32_t(LoadU32(base, menu + 0));
    const uint32_t o = Settings(base);
    const int32_t choice = int32_t(LoadU32(base, o + 660));
    if (cursor == 4 && choice == 1) {
      // OK: the edits applied.
      CopyWords(base, o + 44, o + 236, 4);
      r3 = ShowHudValues(base, frame);
      Go(base, kMinusOne, 0, 12);
      return r3;
    }
    if (cursor == 4 && choice == 0) {
      // Default: the defaults into the edits.
      CopyWords(base, o + 236, o + 428, 4);
      return ShowHudValues(base, frame);
    }
    // The cursor sent to OK.
    StoreU32(base, menu + 4, LoadU32(base, menu + 0));
    StoreU32(base, menu + 0, 4);
    StoreU32(base, Settings(base) + 660, 1);
    Go(base, 1, 10, kMinusOne);
    return r3;
  }
  if (!Pressed(base, calls, frame, 0x821C1400, 158, r3)) {
    return r3;
  }
  // Back: the edits dropped.
  Sound(calls, frame, 0x821C1410, 2);
  const uint32_t o = Settings(base);
  CopyWords(base, o + 236, o + 44, 4);
  r3 = ShowHudValues(base, frame);
  Go(base, kMinusOne, 0, 12);
  return r3;
}

uint64_t SlidersPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C1888 Options::SlidersPage");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMenuStep, frame, 0x821C18A4, Menu(base));
  uint64_t r3;
  if (LoadU32(base, Menu(base) + 8) != 0) {
    Go(base, 3, 18, kMinusOne);
    return Sound(calls, frame, 0x821C191C, 4);
  }
  if (uint32_t(StepSliders(base, frame)) != 0) {
    Go(base, 3, 19, kMinusOne);
    return Sound(calls, frame, 0x821C191C, 4);
  }
  if (Pressed(base, calls, frame, 0x821C192C, 159, r3)) {
    r3 = Sound(calls, frame, 0x821C193C, 0);
    const uint32_t menu = Menu(base);
    const int32_t cursor = int32_t(LoadU32(base, menu + 0));
    const uint32_t o = Settings(base);
    const int32_t choice = int32_t(LoadU32(base, o + 660));
    if (cursor == 3 && choice == 0) {
      // OK: the edits applied.
      CopyWords(base, o + 68, o + 260, 3);
      r3 = ShowSliderMarks(base, frame);
      Go(base, kMinusOne, 0, 20);
      return r3;
    }
    if (cursor == 3 && choice == 1) {
      // Default: the defaults into the edits.
      CopyWords(base, o + 260, o + 452, 3);
      return ShowSliderNumbers(base, frame, calls);
    }
    // The cursor sent to OK.
    StoreU32(base, menu + 4, LoadU32(base, menu + 0));
    StoreU32(base, menu + 0, 3);
    StoreU32(base, Settings(base) + 660, 0);
    Go(base, 3, 18, kMinusOne);
    return r3;
  }
  if (!Pressed(base, calls, frame, 0x821C1A1C, 158, r3)) {
    return r3;
  }
  // Back: the edits dropped.
  Sound(calls, frame, 0x821C1A2C, 2);
  const uint32_t o = Settings(base);
  CopyWords(base, o + 260, o + 68, 3);
  r3 = ShowSliderNumbers(base, frame, calls);
  Go(base, kMinusOne, 0, 20);
  return r3;
}

uint64_t ButtonsPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C1D08 Options::ButtonsPage");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kMenuStep, frame, 0x821C1D28, Menu(base));
  uint64_t r3;
  if (LoadU32(base, Menu(base) + 8) != 0) {
    r3 = Sound(calls, frame, 0x821C1D40, 4);
    Go(base, 5, 26, kMinusOne);
    return r3;
  }
  if (StepButtons(base) != 0) {
    r3 = Sound(calls, frame, 0x821C1D84, 4);
    Go(base, 5, 27, kMinusOne);
    return r3;
  }
  if (Pressed(base, calls, frame, 0x821C1DB0, 159, r3)) {
    const int32_t cursor = int32_t(LoadU32(base, Menu(base) + 0));
    const uint32_t o = Settings(base);
    const int32_t choice = int32_t(LoadU32(base, o + 660));
    if (cursor == 12 && choice == 0) {
      // OK: kept only without a button used twice (else sound 3).
      const uint32_t clash = HasButtonClash(base, frame);
      if (clash != 0) {
        return Sound(calls, frame, 0x821C1E40, 3);
      }
      Sound(calls, frame, 0x821C1DEC, clash);
      const uint32_t settings = Settings(base);
      r3 = Copy(base, settings + 92, settings + 284, 48, frame, 0x821C1E00, calls);
      Go(base, kMinusOne, 1, 28);
      return r3;
    }
    if (cursor == 12 && choice == 1) {
      // Default: the defaults into the edits, shown.
      Copy(base, o + 284, o + 476, 48, frame, 0x821C1E30, calls);
      ShowButtonList(base, frame, calls);
      ShowButtonClashes(base, frame, calls);
      return Sound(calls, frame, 0x821C1E40, 0);
    }
    // The cursor sent to OK.
    r3 = Sound(calls, frame, 0x821C1E4C, 0);
    const uint32_t menu = Menu(base);
    const uint32_t before = LoadU32(base, menu + 0);
    StoreU32(base, menu + 0, 12);
    StoreU32(base, menu + 4, before);
    StoreU32(base, Settings(base) + 660, 0);
    Go(base, 5, 26, kMinusOne);
    return r3;
  }
  if (!Pressed(base, calls, frame, 0x821C1E7C, 158, r3)) {
    return r3;
  }
  // Back (the edits not put back: the page's set-up copies them again).
  r3 = Sound(calls, frame, 0x821C1E8C, 2);
  Go(base, kMinusOne, 1, 28);
  return r3;
}

uint64_t AnalogPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C21A8 Options::AnalogPage");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMenuStep, frame, 0x821C21C4, Menu(base));
  uint64_t r3;
  if (LoadU32(base, Menu(base) + 8) != 0) {
    Go(base, 7, 34, kMinusOne);
    return Sound(calls, frame, 0x821C2374, 4);
  }
  if (StepAnalog(base) != 0) {
    Go(base, 7, 35, kMinusOne);
    return Sound(calls, frame, 0x821C2374, 4);
  }
  if (Pressed(base, calls, frame, 0x821C2240, 159, r3)) {
    const uint32_t menu = Menu(base);
    const int32_t cursor = int32_t(LoadU32(base, menu + 0));
    const uint32_t o = Settings(base);
    const int32_t choice = int32_t(LoadU32(base, o + 660));
    if (cursor == 6 && choice == 0) {
      // OK: kept only without an input used twice (else sound 3).
      if (HasAnalogClash(base, frame) != 0) {
        return Sound(calls, frame, 0x821C2374, 3);
      }
      Copy(base, o + 188, o + 380, 24, frame, 0x821C2288, calls);
      Go(base, kMinusOne, 2, 36);
      return Sound(calls, frame, 0x821C2374, 0);
    }
    if (cursor == 6 && choice == 1) {
      // Default: the defaults into the edits, shown.
      Copy(base, o + 380, o + 572, 24, frame, 0x821C22D4, calls);
      ShowAnalogList(base, frame, calls);
      ShowAnalogClashes(base, frame, calls);
      return Sound(calls, frame, 0x821C2374, 0);
    }
    // The cursor sent to OK.
    StoreU32(base, menu + 4, LoadU32(base, menu + 0));
    StoreU32(base, menu + 0, 6);
    StoreU32(base, Settings(base) + 660, 0);
    Go(base, 7, 34, kMinusOne);
    return Sound(calls, frame, 0x821C2374, 0);
  }
  if (!Pressed(base, calls, frame, 0x821C2330, 158, r3)) {
    return r3;
  }
  // Back (the edits not put back).
  Go(base, kMinusOne, 2, 36);
  return Sound(calls, frame, 0x821C2374, 2);
}

uint64_t SoundPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C1498 Options::SoundPage");
  const uint32_t frame = Frame(base, sp, 160);
  calls.Call(kMenuStep, frame, 0x821C14B0, Menu(base));
  uint64_t r3;
  if (LoadU32(base, Menu(base) + 8) != 0) {
    Go(base, 2, 14, kMinusOne);
    return Sound(calls, frame, 0x821C187C, 4);
  }
  if (StepSound(base) != 0) {
    // The edits heard at once.
    calls.Call(kSetEffectsVolume, frame, 0x821C151C, Setting(base, 256));
    calls.Call(kSetMusicVolume, frame, 0x821C152C, Setting(base, 252));
    r3 = Sound(calls, frame, 0x821C1534, 4);
    Go(base, 2, 15, kMinusOne);
    return r3;
  }
  if (Pressed(base, calls, frame, 0x821C156C, 159, r3)) {
    r3 = Sound(calls, frame, 0x821C157C, 0);
    const uint32_t menu = Menu(base);
    const int32_t cursor = int32_t(LoadU32(base, menu + 0));
    const uint32_t o = Settings(base);
    const int32_t choice = int32_t(LoadU32(base, o + 660));
    if (cursor == 2 && choice == 0) {
      // OK: the edits applied, the page left through page 13.
      CopyWords(base, o + 60, o + 252, 2);
      ShowVolumeBars(base, frame);
      StoreU32(base, Settings(base) + 700, 0);
      ToPage(base, 13);
      calls.Call(kSetEffectsVolume, frame, 0x821C1604, Setting(base, 64));
      return calls.Call(kSetMusicVolume, frame, 0x821C1660, Setting(base, 60));
    }
    if (cursor == 2 && choice == 1) {
      // Default: the defaults into the edits, heard.
      CopyWords(base, o + 252, o + 444, 2);
      ShowVolumeNumbers(base, frame, calls);
      calls.Call(kSetEffectsVolume, frame, 0x821C1650, Setting(base, 256));
      return calls.Call(kSetMusicVolume, frame, 0x821C1660, Setting(base, 252));
    }
    if (cursor == 3) {
      // The track: the menu music (bank 1's cues 0 to 3) stopped, then the
      // preview stopped (page 11) or the track picked played (page 12).
      static constexpr uint32_t kCues[4][2] = {{84, 80}, {92, 88}, {100, 96}, {108, 104}};
      static constexpr uint32_t kLr[4] = {0x821C169C, 0x821C16C0, 0x821C16E8, 0x821C1710};
      for (uint32_t cue = 0; cue < 4; cue++) {
        StoreU32(base, frame + kCues[cue][1], cue);
        StoreU32(base, frame + kCues[cue][0], 1);
        r3 = Slot(base, calls, kStopCue, frame, kLr[cue], frame + kCues[cue][0],
                  frame + kCues[cue][1]);
      }
      const uint32_t settings = Settings(base);
      if (LoadU32(base, settings + 692) == LoadU32(base, settings + 688)) {
        StoreU32(base, settings + 692, kMinusOne);
        StoreU32(base, Settings(base) + 700, 0);
        ToPage(base, 11);
      } else {
        StoreU32(base, settings + 700, 0);
        const uint32_t s = Settings(base);
        StoreU32(base, s + 692, LoadU32(base, s + 688));
        ToPage(base, 12);
      }
      return r3;
    }
    // The cursor sent to OK.
    StoreU32(base, menu + 4, LoadU32(base, menu + 0));
    StoreU32(base, menu + 0, 2);
    StoreU32(base, Settings(base) + 660, 0);
    Go(base, 2, 14, kMinusOne);
    return r3;
  }
  if (!Pressed(base, calls, frame, 0x821C17F0, 158, r3)) {
    return r3;
  }
  // Back: the edits dropped, the page left through page 13.
  const uint32_t o = Settings(base);
  CopyWords(base, o + 252, o + 60, 2);
  ShowVolumeNumbers(base, frame, calls);
  StoreU32(base, Settings(base) + 700, 0);
  ToPage(base, 13);
  calls.Call(kSetEffectsVolume, frame, 0x821C1864, Setting(base, 64));
  calls.Call(kSetMusicVolume, frame, 0x821C1874, Setting(base, 60));
  return Sound(calls, frame, 0x821C187C, 2);
}

uint64_t LeaveSoundPage(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BA380 Options::LeaveSoundPage");
  const uint32_t frame = Frame(base, sp, 160);
  const int32_t state = int32_t(LoadU32(base, Settings(base) + 700));
  if (state == 0) {
    // The preview (bank 2's cue 0) stopped.
    StoreU32(base, frame + 80, 0);
    StoreU32(base, frame + 84, 2);
    r3 = Slot(base, calls, kStopCue, frame, 0x821BA3CC, frame + 84, frame + 80);
    StoreU32(base, Settings(base) + 700, 1);
    return r3;
  }
  if (state == 1) {
    // Once it's stopped, bank 2 released.
    StoreU32(base, frame + 88, 0);
    StoreU32(base, frame + 92, 2);
    r3 = Slot(base, calls, kIsCuePlaying, frame, 0x821BA414, frame + 92, frame + 88);
    if ((r3 & 0xFF) != 0) {
      return r3;
    }
    StoreU32(base, frame + 96, 2);
    r3 = Slot(base, calls, kReleaseBank, frame, 0x821BA438, frame + 96);
    StoreU32(base, Settings(base) + 700, 2);
    return r3;
  }
  if (state != 2) {
    return r3;
  }
  // Once bank 2's slot is empty: the menu music started again unless it's
  // playing, and the sound page's close queued.
  StoreU32(base, frame + 100, 2);
  r3 = Slot(base, calls, kIsBankSlotEmpty, frame, 0x821BA46C, frame + 100);
  if ((r3 & 0xFF) == 0) {
    return r3;
  }
  StoreU32(base, frame + 108, 1);
  StoreU32(base, frame + 104, LoadU32(base, Settings(base) + 704));
  r3 = Slot(base, calls, kIsCuePlaying, frame, 0x821BA4A4, frame + 108, frame + 104);
  if ((r3 & 0xFF) == 0) {
    StoreU32(base, frame + 116, 1);
    StoreU32(base, frame + 112, LoadU32(base, Settings(base) + 704));
    r3 = calls.Call(kSoundPlay, frame, 0x821BA4C8, frame + 116, frame + 112);
  }
  Go(base, kMinusOne, 0, 16);
  return r3;
}

uint64_t SetUpCategoryPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C4BE0 Options::SetUpCategoryPage");
  const uint32_t frame = Frame(base, sp, 144);
  const Texts t{calls, frame, FindMovie(base, frame)};
  // The context's entries before its first -1 (at most 7).
  const uint32_t context = LoadU32(base, Settings(base) + 4);
  uint32_t count = 0;
  while (count < 7 && int32_t(Category(base, context, count)) != -1) {
    count++;
  }
  const uint32_t cursor = LoadU32(base, Settings(base) + 664);
  const uint32_t menu = Menu(base);
  StoreU32(base, menu + 12, 0);
  StoreU32(base, menu + 16, count - 1);
  StoreU32(base, menu + 20, 1);
  for (const uint32_t at : {8u, 44u, 56u}) {
    StoreU32(base, menu + at, 0);
  }
  StoreU32(base, menu + 0, cursor);
  StoreU32(base, menu + 4, cursor);
  StoreU32(base, menu + 60, 8);
  for (const uint32_t at : {36u, 64u, 24u, 28u}) {
    StoreU32(base, menu + at, 0);
  }
  // The category under the cursor's frame on nodes 28 and 29.
  const uint32_t first = ui_movies::FindNode(base, t.m, kMinusOne, 28);
  StoreU32(base, first + 108,
           Category(base, LoadU32(base, Settings(base) + 4), LoadU32(base, Menu(base) + 0)));
  const uint32_t second = ui_movies::FindNode(base, t.m, kMinusOne, 29);
  StoreU32(base, second + 108,
           Category(base, LoadU32(base, Settings(base) + 4), LoadU32(base, Menu(base) + 0)));
  // The items' names, then the help line.
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t category = Category(base, LoadU32(base, Settings(base) + 4), i);
    if (int32_t(category) == -1) {
      break;
    }
    t.Text(0x821C4D34, 21, 5 + i, uint32_t(category + 174));
  }
  t.Text(0x821C4D74, 2, 0,
         uint32_t(Category(base, LoadU32(base, Settings(base) + 4), LoadU32(base, Menu(base) + 0)) +
                  982));
  t.ScrollHelp(0x821C4D88);
  const uint64_t prompts[6] = {0, 0, 1, 1, ~uint64_t(0), ~uint64_t(0)};
  return ShowPrompts(base, prompts, frame);
}

uint64_t SetUpHudPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C4DB0 Options::SetUpHudPage");
  const uint32_t frame = Frame(base, sp, 144);
  const Texts t{calls, frame, FindMovie(base, frame)};
  SetUpMenu(base, 4);
  const uint32_t o = Settings(base);
  CopyWords(base, o + 236, o + 44, 4);
  StoreU32(base, Settings(base) + 660, 1);
  ResetNode(base, kMinusOne, 5, frame, calls);
  // The four items' names (strings 167 to 170).
  const uint32_t names = ui_movies::FindNode(base, t.m, 5, 0);
  for (uint32_t i = 0; i < 4; i++) {
    t.Node(0x821C4EA8, ui_movies::Child(base, names, 5 + i), 167 + i);
  }
  t.OkDefault(base, ui_movies::FindNode(base, t.m, 5, 1), 0x821C4EE0, 0x821C4EFC);
  // Each item's two choices (strings 0 and 1).
  const uint32_t items = ui_movies::FindNode(base, t.m, 5, 2);
  for (int32_t i = 0; i < int32_t(LoadU32(base, items + 104)); i++) {
    const uint32_t item = ui_movies::FindNode(base, t.m, 5, LoadU32(base, items + 100) + i);
    const uint32_t left = ui_movies::FindNode(base, t.m, 5, LoadU32(base, item + 100));
    t.Node(0x821C4F5C, ui_movies::Child(base, left, 1), 0);
    const uint32_t right = ui_movies::FindNode(base, t.m, 5, LoadU32(base, item + 100) + 1);
    t.Node(0x821C4F88, ui_movies::Child(base, right, 1), 1);
  }
  t.Text(0x821C4FAC, 2, 0, 989);
  t.ScrollHelp(0x821C4FC0);
  return ShowHudValues(base, frame);
}

uint64_t SetUpSoundPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C4FD0 Options::SetUpSoundPage");
  const uint32_t frame = Frame(base, sp, 128);
  const Texts t{calls, frame, FindMovie(base, frame)};
  ResetNode(base, kMinusOne, 6, frame, calls);
  const uint32_t track = ui_movies::FindNode(base, t.m, 6, 3);
  const bool tracks = int32_t(LoadU32(base, Settings(base) + 8)) == 1;
  const uint32_t menu = Menu(base);
  for (const uint32_t at : {12u, 0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    StoreU32(base, menu + at, 0);
  }
  // The music track's row when it's unlocked.
  StoreU32(base, menu + 20, 1);
  StoreU32(base, menu + 16, tracks ? 3 : 2);
  StoreU32(base, track + 80, tracks ? 1 : 0);
  const uint32_t o = Settings(base);
  CopyWords(base, o + 252, o + 60, 2);
  StoreU32(base, Settings(base) + 688, 0);
  StoreU32(base, Settings(base) + 692, kMinusOne);
  StoreU32(base, Settings(base) + 696, kMinusOne);
  StoreU32(base, Settings(base) + 660, 0);
  const uint32_t names = ui_movies::FindNode(base, t.m, 6, 0);
  t.Node(0x821C5100, ui_movies::Child(base, names, 5), 171);
  t.Node(0x821C511C, ui_movies::Child(base, names, 6), 172);
  t.Node(0x821C5144, ui_movies::Child(base, ui_movies::FindNode(base, t.m, 6, 9), 5), 173);
  t.OkDefault(base, ui_movies::FindNode(base, t.m, 6, 1), 0x821C5170, 0x821C518C);
  ShowMusicName(base, LoadU32(base, Settings(base) + 688), frame, calls);
  ShowVolumeNumbers(base, frame, calls);
  ShowVolumeBars(base, frame);
  t.Text(0x821C51B4, 2, 0, 993);
  return t.ScrollHelp(0x821C51C8);
}

uint64_t SetUpSlidersPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C51D0 Options::SetUpSlidersPage");
  const uint32_t frame = Frame(base, sp, 128);
  const Texts t{calls, frame, FindMovie(base, frame)};
  SetUpMenu(base, 3);
  const uint32_t o = Settings(base);
  CopyWords(base, o + 260, o + 68, 3);
  StoreU32(base, Settings(base) + 660, 0);
  ResetNode(base, kMinusOne, 7, frame, calls);
  // The three sliders' names (strings 345 to 347).
  const uint32_t sliders = ui_movies::Child(base, ui_movies::FindNode(base, t.m, 7, 0), 0);
  for (uint32_t i = 0; i < 3; i++) {
    t.Node(0x821C52C0, ui_movies::Child(base, sliders, 5 + i), 345 + i);
  }
  t.OkDefault(base, ui_movies::FindNode(base, t.m, 7, 1), 0x821C52F8, 0x821C5314);
  t.Text(0x821C5328, 2, 0, 996);
  t.ScrollHelp(0x821C533C);
  ShowSliderNumbers(base, frame, calls);
  return ShowSliderMarks(base, frame);
}

uint64_t SetUpButtonsPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C5580 Options::SetUpButtonsPage");
  const uint32_t frame = Frame(base, sp, 128);
  const Texts t{calls, frame, FindMovie(base, frame)};
  SetUpMenu(base, 12);
  const uint32_t o = Settings(base);
  Copy(base, o + 284, o + 92, 48, frame, 0x821C560C, calls);
  StoreU32(base, Settings(base) + 660, 0);
  ResetNode(base, kMinusOne, 9, frame, calls);
  ShowButtonList(base, frame, calls);
  t.OkDefault(base, ui_movies::FindNode(base, t.m, 9, 2), 0x821C5650, 0x821C566C);
  t.Text(0x821C5680, 2, 0, 999);
  t.ScrollHelp(0x821C5694);
  ShowButtonPicture(base, LoadU32(base, Settings(base) + 284), frame);
  return ShowButtonClashes(base, frame, calls);
}

uint64_t SetUpAnalogPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C57D0 Options::SetUpAnalogPage");
  const uint32_t frame = Frame(base, sp, 128);
  const Texts t{calls, frame, FindMovie(base, frame)};
  SetUpMenu(base, 6);
  const uint32_t o = Settings(base);
  Copy(base, o + 380, o + 188, 24, frame, 0x821C585C, calls);
  StoreU32(base, Settings(base) + 660, 0);
  ResetNode(base, kMinusOne, 11, frame, calls);
  ShowAnalogList(base, frame, calls);
  t.OkDefault(base, ui_movies::FindNode(base, t.m, 11, 2), 0x821C58A0, 0x821C58BC);
  t.Text(0x821C58D0, 2, 0, 999);
  t.ScrollHelp(0x821C58E4);
  ShowAnalogPicture(base, AnalogPicture(LoadU32(base, Settings(base) + 380)), frame);
  return ShowAnalogClashes(base, frame, calls);
}

}  // namespace options_screen
