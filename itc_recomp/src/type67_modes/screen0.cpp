// The type 6 and 7 first screens' helpers (docs/functions/type6-type7-modes.md):
// the confirms that fill the yes/no dialog (movie 16) and post its open
// (Type6Screen0::Confirm, ConfirmAgain, Type7Screen0::Confirm), and the type
// 6 menu's page pictures (Type6Screen0::ShowPage). UiMovies::Find,
// UiMovie::SetText (Strings::FormatV's guest calls through FormatCalls) and
// the node setters through their plain C++.

#include "type67_modes/type67_modes.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "car_makers/car_makers.h"
#include "guest.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "mode_tick/mode_tick.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstdlib>

namespace type67_modes {

namespace {

using guest::LoadU32;
using guest::StoreU32;

// The movie with id `id`: the id at the frame's +80, then UiMovies::Find.
uint32_t MovieAt(uint8_t* base, uint32_t frame, uint32_t id) {
  StoreU32(base, frame + 80, id);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

// UiMovie::SetText with r7 to r10 0 (the user's call for its unset varargs,
// 2026-10-30), from the frame at `frame`; a refusal after the adapters'
// checks is a logged abort.
uint32_t SetText(uint8_t* base, const strings_and_font::FormatCalls& calls, uint32_t frame,
                 uint32_t movie, uint32_t sub_scene, uint32_t child, uint32_t id) {
  static constexpr uint64_t kNoArguments[4] = {0, 0, 0, 0};
  const std::optional<uint32_t> r3 =
      ui_movies::SetText(base, calls, frame, movie, sub_scene, child, id, kNoArguments);
  if (!r3) {
    ITC_LOG_ERROR("type67_modes: UiMovie::SetText refused after the adapter's checks");
    std::abort();
  }
  return *r3;
}

// A message (`message`, 0, 0) queued in a dispatcher's ring (+204), as the
// handlers do it inline.
void Post(uint8_t* base, uint32_t dispatcher, uint32_t message) {
  const uint32_t ring = dispatcher + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
  StoreU32(base, slot + 0, message);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
}

void Show(uint8_t* base, uint32_t movie, uint32_t child, uint32_t value,
          uint32_t sub_scene = 3) {
  StoreU32(base, ui_movies::FindNode(base, movie, sub_scene, child) + 80, value);
}

uint32_t Texture(uint8_t* base, uint32_t movie, uint32_t child, uint32_t texture,
                 uint32_t sub_scene = 3) {
  return ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, sub_scene, child),
                                   texture);
}

// UiMovie::HideNode's work: the node's +80 = 0; the node (its r3).
uint32_t Hide(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  StoreU32(base, node + 80, 0);
  return node;
}

// A UI movie text helper that has no native path where GetText would run
// the original: a logged abort (the adapters run these only while the UI
// movies' text setters are native).
uint32_t Required(const std::optional<uint32_t>& r3, const char* what) {
  if (!r3) {
    ITC_LOG_ERROR("type67_modes: {} refused after the adapter's checks", what);
    std::abort();
  }
  return *r3;
}

uint64_t SoundThunk(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                    uint32_t lr) {
  return static_cast<const Calls*>(user)->call_set(function, args, set, r1, lr);
}

uint64_t MenuThunk(void* user, uint32_t function, uint64_t r3, uint64_t r4, uint32_t r1,
                   uint32_t lr) {
  return static_cast<const Calls*>(user)->call(function, r1, lr, r3, r4);
}

menu_helpers::Calls Menus(const Calls& calls) {
  return {const_cast<Calls*>(&calls), &MenuThunk};
}

menu_sounds::Calls Sounds(const Calls& calls) {
  return {const_cast<Calls*>(&calls), &SoundThunk};
}

constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kMakeModel = 0x8217FCB0;    // GarageStage::MakeModel
constexpr uint32_t kCommand = 0x82180108;      // GarageStage::Command
constexpr uint32_t kGarageCarReset = 0x82110448;
constexpr uint32_t kMemcpy = 0x825A7410;

// The selected garage car's node: the list at `garage` +204 walked
// [save +0x19920] times through the halfword links (each node 4 bytes at
// `garage` + 4 x its index, sign-extended), as the originals' loops do.
uint32_t GarageNode(uint8_t* base, uint32_t garage, uint32_t steps) {
  uint32_t node = garage + (uint32_t(int32_t(int16_t(guest::LoadU16(base, garage + 204)))) << 2);
  while (steps != 0) {
    node = garage + (uint32_t(int32_t(int16_t(guest::LoadU16(base, node)))) << 2);
    steps--;
  }
  return node;
}

}  // namespace

uint64_t Screen0Confirm(uint8_t* base, const ConfirmKind& kind, uint32_t dispatcher, uint32_t sp,
                        const strings_and_font::FormatCalls& calls) {
  // One print per function: the macro's flag is per call site.
  if (&kind == &kType6Confirm) {
    ITC_NATIVE_RUNNING("sub_82233FC8 Type6Screen0::Confirm");
  } else if (&kind == &kType6ConfirmAgain) {
    ITC_NATIVE_RUNNING("sub_822340B0 Type6Screen0::ConfirmAgain");
  } else {
    ITC_NATIVE_RUNNING("sub_82244528 Type7Screen0::Confirm");
  }
  const uint32_t frame = sp - 112;
  const uint32_t movie = MovieAt(base, frame, 16);
  SetText(base, calls, frame, movie, 0xFFFFFFFF, 0, kind.text);
  SetText(base, calls, frame, movie, 1, 3, 2);
  const uint32_t r3 = SetText(base, calls, frame, movie, 1, 4, 3);
  StoreU32(base, movie + 40960, kind.choice);
  Post(base, dispatcher, kind.message);
  return r3;
}

uint64_t Type6Screen0ShowPage(uint8_t* base, uint32_t page, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822344A8 Type6Screen0::ShowPage");
  const uint32_t movie = MovieAt(base, sp - 112, 22);
  Show(base, movie, 4, 1);
  Show(base, movie, 5, 1);
  for (const uint32_t child : {6u, 7u, 8u, 9u, 13u, 14u}) {
    Show(base, movie, child, 0);
  }
  switch (int32_t(page)) {
    case 0:
      Show(base, movie, 6, 1);
      Show(base, movie, 7, 1);
      Texture(base, movie, 4, 0);
      Texture(base, movie, 6, 6);
      Texture(base, movie, 5, 0);
      return Texture(base, movie, 7, 2);
    case 1:
      Texture(base, movie, 4, 6);
      return Texture(base, movie, 5, 1);
    case 2:
      Show(base, movie, 6, 1);
      Show(base, movie, 7, 1);
      Texture(base, movie, 4, 0);
      Texture(base, movie, 6, 1);
      Texture(base, movie, 5, 0);
      return Texture(base, movie, 7, 1);
    case 3:
      Texture(base, movie, 4, 0);
      return Texture(base, movie, 5, 0);
    default: {
      Show(base, movie, 4, 0);
      const uint32_t node = ui_movies::FindNode(base, movie, 3, 5);
      StoreU32(base, node + 80, 0);
      return node;
    }
  }
}

uint64_t Type7Screen0ShowPage(uint8_t* base, uint32_t page, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82244930 Type7Screen0::ShowPage");
  const uint32_t movie = MovieAt(base, sp - 112, 22);
  for (const uint32_t child : {4u, 5u, 6u, 7u}) {
    Show(base, movie, child, 1, 2);
  }
  Hide(base, movie, 2, 8);
  Hide(base, movie, 2, 9);
  switch (int32_t(page)) {
    case 0:
      Texture(base, movie, 4, 0, 2);
      Texture(base, movie, 6, 6, 2);
      Texture(base, movie, 5, 0, 2);
      return Texture(base, movie, 7, 2, 2);
    case 1:
      Texture(base, movie, 4, 6, 2);
      Texture(base, movie, 5, 1, 2);
      Hide(base, movie, 2, 6);
      return Hide(base, movie, 2, 7);
    default:
      Texture(base, movie, 4, 0, 2);
      Texture(base, movie, 6, 1, 2);
      Texture(base, movie, 5, 0, 2);
      return Texture(base, movie, 7, 1, 2);
  }
}

uint64_t Type7Screen0PlaceCursor(uint8_t* base, uint64_t item, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822448B0 Type7Screen0::PlaceCursor");
  const uint32_t frame = sp - 112;
  const uint32_t movie = MovieAt(base, frame, 22);
  const uint32_t node = ui_movies::FindNode(base, movie, 0xFFFFFFFF, 3);
  // mulli and extsw, the doubleword through the frame's +80 (std, lfd,
  // fcfid, frsp).
  const int64_t value = int64_t(int32_t(uint32_t(item * 40)));
  StoreU32(base, frame + 80, uint32_t(uint64_t(value) >> 32));
  StoreU32(base, frame + 84, uint32_t(value));
  const uint32_t child = ui_movies::Child(base, node, 0);
  guest::StoreFloat(base, child + 16, float(double(value)));
  return child;
}

uint64_t Type7Screen0ShowHelp(uint8_t* base, uint32_t item, uint32_t sp,
                              const strings_and_font::FormatCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82244AB8 Type7Screen0::ShowHelp");
  const uint32_t frame = sp - 128;
  const uint32_t movie = MovieAt(base, frame, 22);
  uint32_t text = 919;
  switch (int32_t(item)) {
    case 1: text = 916; break;
    case 2: text = 917; break;
    case 3: text = 918; break;
    default: break;
  }
  Required(ui_movies::ResetTextScroll(base, movie, 1, 0), "sub_8211C5F8");
  SetText(base, calls, frame, movie, 1, 0, text);
  return Required(ui_movies::ScrollText(base, movie, 1, 0, 1), "UiMovie::ScrollText");
}

uint64_t Type7Screen0FadeOutStep(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82244760 Type7Screen0::FadeOutStep");
  const uint32_t frame = sp - 112;
  const uint32_t movie = MovieAt(base, frame, 22);
  const uint32_t page = LoadU32(base, screen + 9648);
  if (LoadU32(base, page + 24) == 0) {
    mode_tick::ScreenFadesFadeOut(base, LoadU32(base, 0x82001BF0), 0, 0, 0, 0);
    StoreU32(base, page + 24, 1);
  }
  calls.call(kRun, frame, 0x822447D4, movie, 0);
  const bool done = (LoadU32(base, mode_tick::kFadeOutCounts) |
                     LoadU32(base, mode_tick::kFadeInCounts)) == 0;
  if (done) {
    calls.call(kFinishAll, frame, 0x822447FC, movie, 0);
  }
  return done ? 1 : 0;
}

uint64_t Type7Screen0ChosenPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82244610 Type7Screen0::ChosenPage");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = sp - 112;
  const uint32_t item = LoadU32(base, LoadU32(base, screen + 9652));
  if (item == 3) {
    StoreU32(base, screen + 9608, 10);
    return r3;
  }
  if (item > 3) {
    return r3;
  }
  StoreU32(base, screen + 9608, 9);
  // r6 as the post leaves it (the ring's start plus its count, 64-bit),
  // passed on to the slot below.
  const uint64_t r6 = uint64_t(LoadU32(base, dispatcher + 204 + 604)) +
                      uint64_t(LoadU32(base, dispatcher + 204 + 600));
  Post(base, dispatcher, item == 0 ? 13 : item == 1 ? 15 : 23);
  if (item != 0) {
    return r3;
  }
  // The music: bank 1's cue 2 stopped (the sound manager's slot 13), bank
  // 1's cue 0 played (Sound::Play), the indexes in the frame.
  StoreU32(base, frame + 84, 1);
  StoreU32(base, frame + 80, 2);
  const uint32_t manager = LoadU32(base, menu_sounds::kSoundManager);
  const uint64_t args[4] = {manager, frame + 84, frame + 80, r6};
  calls.call_set(LoadU32(base, LoadU32(base, manager) + 52), args, 4, frame, 0x82244730);
  StoreU32(base, frame + 92, 1);
  StoreU32(base, frame + 88, 0);
  return menu_sounds::PlaySound(base, frame + 92, frame + 88, frame,
                                {const_cast<Calls*>(&calls), &SoundThunk});
}

uint64_t Screen0OpenMovie(uint8_t* base, bool type7, uint32_t sp, const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82243E80 Type7Screen0::OpenMovie");
  } else {
    ITC_NATIVE_RUNNING("sub_822338E8 Type6Screen0::OpenMovie");
  }
  const uint32_t frame = sp - 96;
  const uint32_t lr = type7 ? 0x82243E9C : 0x82233904;
  calls.open(22, type7 ? 22569 : 22564, frame, lr);
  const uint32_t block = car_makers::BlockOf(0);
  const uint32_t holder = LoadU32(base, LoadU32(base, kStandHolder));
  const uint64_t args[4] = {holder, block, 1, 0};
  calls.call_set(kMakeModel, args, 3, frame, lr + 0x20);
  return calls.call(kCommand, frame, lr + 0x30, LoadU32(base, LoadU32(base, kStandHolder)), 14);
}

uint64_t Screen0SetUpCar(uint8_t* base, bool type7, uint64_t r4, uint32_t sp, const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_822443E0 Type7Screen0::SetUpCar");
  } else {
    ITC_NATIVE_RUNNING("sub_82233E80 Type6Screen0::SetUpCar");
  }
  const uint32_t frame = sp - 128;
  const uint32_t dispatcher = uint32_t(r4);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t car = type7 ? kType7Car : kType6Car;
  uint32_t message = type7 ? 5 : 6;
  if (LoadU32(base, car + 1972) == 0xFFFFFFFF) {
    calls.call(kGarageCarReset, frame, type7 ? 0x82244418 : 0x82233EB8, car, r4);
    if (guest::LoadU16(base, save + 6328) == 0) {
      message = type7 ? 17 : 20;
    } else {
      const uint32_t garage = save + 6120, steps = save + 0x19920;
      const int32_t slot = int16_t(guest::LoadU16(base, GarageNode(base, garage, LoadU32(base, steps)) + 2));
      const uint32_t from = uint32_t(slot) * 1968 + garage + 212;
      if (!c_runtime::Memcpy(base, car, from, 1968)) {
        const uint64_t args[4] = {car, from, 1968, 0};
        calls.call_set(kMemcpy, args, 3, frame, type7 ? 0x82244478 : 0x82233F18);
      }
      StoreU32(base, car + 1972, 2);
      StoreU32(base, car + 1976, LoadU32(base, steps));
      const int32_t again =
          int16_t(guest::LoadU16(base, GarageNode(base, garage, LoadU32(base, steps)) + 2));
      StoreU32(base, car + 1980,
               uint32_t(int32_t(int16_t(
                   guest::LoadU16(base, uint32_t(again) * 1968 + garage + 212)))));
    }
  }
  const uint32_t ring = dispatcher + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
  StoreU32(base, slot + 8, 0);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 0, message);
  mode_tick::ScreenFadesFadeOut(base, 1, 0, 0, 0, 0);
  return 1;  // FadeOut leaves its r3, the passes
}

uint64_t Type6Screen0OnDialogClosed(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82234768 Type6Screen0::OnDialogClosed");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = sp - 96;
  const uint32_t asked = LoadU32(base, screen + 9664);
  if (asked != 4 && asked != 5) {
    return r3;
  }
  StoreU32(base, screen + 9664, 0xFFFFFFFF);
  const uint32_t pick = LoadU32(base, LoadU32(base, screen + 9640) + 72);
  if (pick >= 3) {
    return r3;
  }
  const uint32_t p = LoadU32(base, screen + 9644);
  auto clear_pick = [&] { StoreU32(base, LoadU32(base, screen + 9640) + 72, 0xFFFFFFFF); };
  auto page0 = [&] {
    const uint32_t page = LoadU32(base, p);
    if (page != 0) StoreU32(base, p + 8, page);
    StoreU32(base, p + 0, 0);
    StoreU32(base, p + 12, 0xFFFFFFFF);
    StoreU32(base, p + 16, 0xFFFFFFFF);
    clear_pick();
    return Type6Screen0ShowPage(base, 0, frame);
  };
  if (asked == 4) {
    if (pick != 1) {
      return page0();
    }
    // Yes to the first confirm: the second one (ConfirmAgain, a guest call
    // to its override).
    const uint32_t page = LoadU32(base, p);
    if (page != 0xFFFFFFFF) StoreU32(base, p + 8, page);
    StoreU32(base, p + 0, 0xFFFFFFFF);
    StoreU32(base, p + 12, 0xFFFFFFFF);
    StoreU32(base, p + 16, 0xFFFFFFFF);
    clear_pick();
    calls.call(0x822340B0, frame, 0x82234810, r3, r4);
    StoreU32(base, screen + 9656, 0);
    StoreU32(base, screen + 9664, 5);
    return Type6Screen0ShowPage(base, 2, frame);
  }
  if (pick == 2) {
    return page0();
  }
  if (pick == 0) {
    // Yes to the second: the fade out step, then page 3.
    const uint32_t page = LoadU32(base, p);
    if (page != 3) StoreU32(base, p + 8, page);
    StoreU32(base, p + 16, 0xFFFFFFFF);
    StoreU32(base, p + 0, 0xFFFFFFFF);
    StoreU32(base, p + 12, 3);
    StoreU32(base, p + 4, 3);
    clear_pick();
    return r3;
  }
  // No to the second: page 2, message 29 (the save screen).
  const uint32_t page = LoadU32(base, p);
  if (page != 2) StoreU32(base, p + 8, page);
  StoreU32(base, p + 12, 0xFFFFFFFF);
  StoreU32(base, p + 16, 0xFFFFFFFF);
  StoreU32(base, p + 0, 2);
  clear_pick();
  Post(base, uint32_t(r4), 29);
  return Type6Screen0ShowPage(base, 3, frame);
}

uint64_t Type7Screen0OnDialogClosed(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82244B60 Type7Screen0::OnDialogClosed");
  const uint32_t screen = uint32_t(r3);
  const uint32_t pick = LoadU32(base, LoadU32(base, screen + 9644) + 72);
  if (pick >= 3) {
    return r3;
  }
  const uint32_t p = LoadU32(base, screen + 9648);
  const uint32_t page = LoadU32(base, p);
  if (pick == 1) {
    if (page != 2) StoreU32(base, p + 8, page);
    StoreU32(base, p + 16, 0xFFFFFFFF);
    StoreU32(base, p + 12, 3);
    StoreU32(base, p + 4, 2);
    StoreU32(base, p + 0, 0xFFFFFFFF);
    StoreU32(base, LoadU32(base, screen + 9644) + 72, 0xFFFFFFFF);
    return r3;
  }
  if (page != 0) StoreU32(base, p + 8, page);
  StoreU32(base, p + 12, 0xFFFFFFFF);
  StoreU32(base, p + 0, 0);
  StoreU32(base, p + 16, 0xFFFFFFFF);
  StoreU32(base, LoadU32(base, screen + 9644) + 72, 0xFFFFFFFF);
  return Type7Screen0ShowPage(base, 0, sp);  // a tail call
}

uint64_t ControllerOptionsUpdate(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C7F98 ControllerOptions::Update");
  const uint32_t frame = sp - 176;
  // (The empty function sub_82106540 it calls first does nothing.) The
  // steps' table in the frame, +124 stored first.
  static constexpr uint32_t kSteps[12] = {0x821BFBD8, 0x821C5D68, 0x821C5F38, 0x821BFC68,
                                          0x821BFD08, 0x821BFD98, 0x821C5F80, 0x821BFF20,
                                          0x821BFFA0, 0x821C0030, 0x821C60D0, 0x821C01B8};
  StoreU32(base, frame + 124, kSteps[11]);
  const uint32_t page = LoadU32(base, kControllerPage);
  const uint32_t step = LoadU32(base, page + 12);
  for (uint32_t i = 0; i < 11; i++) {
    StoreU32(base, frame + 80 + 4 * i, kSteps[i]);
  }
  // r3 and r4 as the table's build leaves them (sign-extended addresses).
  uint64_t r3 = 0xFFFFFFFF00000000ull | 0x821BFD08;
  constexpr uint64_t kR4 = 0xFFFFFFFF00000000ull | 0x821BFD98;
  if (step == 0xFFFFFFFF) {
    return r3;
  }
  uint32_t p = page;
  if (int32_t(step) >= 0) {
    const uint32_t target = LoadU32(base, frame + 80 + (step << 2));
    r3 = calls.call(target, frame, 0x821C8080, uint64_t(page) + 24, kR4);
    p = LoadU32(base, kControllerPage);
    if (int32_t(r3) == 0) {
      return r3;
    }
  }
  const uint32_t next = LoadU32(base, p + 16);
  if (next != 0xFFFFFFFF) {
    StoreU32(base, p + 16, 0xFFFFFFFF);
    StoreU32(base, p + 20, 0);
    StoreU32(base, p + 12, next);
  } else {
    StoreU32(base, p + 0, LoadU32(base, p + 4));
    StoreU32(base, p + 12, 0xFFFFFFFF);
  }
  StoreU32(base, p + 24, 0);
  return r3;
}

uint64_t Type7Screen0MenuPage(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82244BF8 Type7Screen0::MenuPage");
  constexpr uint32_t kPowerGraphSetUp = 0x82250488;  // never seen in play: recompiled
  constexpr uint32_t kConfirm = 0x82244528;          // Type7Screen0::Confirm's override
  const uint32_t screen = uint32_t(r3), dispatcher = uint32_t(r4);
  const uint32_t frame = sp - 112;
  const bool slots = calls.native_action_slots;
  menu_helpers::Step(base, LoadU32(base, screen + 9652), frame, slots, Menus(calls));
  // The page state's step: +8 = +0 unless +0 is `keep`, +0 = -1, +4 =
  // `after`, +16 = -1, +12 = `step` (the originals' shared tail).
  auto to_step = [&](uint32_t keep, uint32_t step, uint32_t after) {
    const uint32_t p = LoadU32(base, screen + 9648);
    const uint32_t page = LoadU32(base, p);
    if (page != keep) StoreU32(base, p + 8, page);
    StoreU32(base, p + 0, 0xFFFFFFFF);
    StoreU32(base, p + 4, after);
    StoreU32(base, p + 16, 0xFFFFFFFF);
    StoreU32(base, p + 12, step);
  };
  // The page state shown at once: +12 = +16 = -1, then ShowPage.
  auto show = [&](uint32_t page) {
    const uint32_t p = LoadU32(base, screen + 9648);
    StoreU32(base, p + 12, 0xFFFFFFFF);
    StoreU32(base, p + 16, 0xFFFFFFFF);
    return Type7Screen0ShowPage(base, page, frame);
  };
  if (LoadU32(base, LoadU32(base, screen + 9652) + 8) != 0) {
    const uint64_t answer = menu_sounds::PlayMenuSound(base, 4, frame, Sounds(calls));
    to_step(0, 2, 0);
    return answer;
  }
  if (int32_t(menu_helpers::Pressed(base, LoadU32(base, screen + 9652), 163, frame, slots,
                                    Menus(calls))) != 0) {
    menu_sounds::PlayMenuSound(base, 0, frame, Sounds(calls));
    calls.call(kPowerGraphSetUp, frame, 0x82244C7C, kType7Car,
               uint64_t(LoadU32(base, kType7Car + 1972)) + 3);  // lwz, then addi
    Post(base, dispatcher, 18);
    const uint32_t p = LoadU32(base, screen + 9648);
    const uint32_t page = LoadU32(base, p);
    if (int32_t(page) != 1) StoreU32(base, p + 8, page);
    StoreU32(base, p + 0, 1);
    return show(1);
  }
  if (int32_t(menu_helpers::Pressed(base, LoadU32(base, screen + 9652), 159, frame, slots,
                                    Menus(calls))) != 0) {
    const uint64_t answer = menu_sounds::PlayMenuSound(base, 0, frame, Sounds(calls));
    const uint32_t menu = LoadU32(base, screen + 9652);
    StoreU32(base, screen + 9656, LoadU32(base, menu));
    const uint32_t item = LoadU32(base, menu);
    if (item < 3) {
      to_step(2, 3, 2);
      return answer;
    }
    if (item != 3) {
      return answer;
    }
    calls.call(kConfirm, frame, 0x82244D44, r3, r4);
    const uint32_t p = LoadU32(base, screen + 9648);
    const uint32_t page = LoadU32(base, p);
    if (int32_t(page) != -1) StoreU32(base, p + 8, page);
    StoreU32(base, p + 0, 0xFFFFFFFF);
    return show(2);
  }
  const uint64_t answer = menu_helpers::Pressed(base, LoadU32(base, screen + 9652), 158, frame,
                                                slots, Menus(calls));
  if (int32_t(answer) == 0) {
    return answer;
  }
  const uint64_t played = menu_sounds::PlayMenuSound(base, 2, frame, Sounds(calls));
  const uint32_t menu = LoadU32(base, screen + 9652);
  const uint32_t item = LoadU32(base, menu);
  StoreU32(base, menu + 0, 3);
  StoreU32(base, menu + 4, item);
  to_step(0, 1, 0);
  return played;
}

}  // namespace type67_modes
