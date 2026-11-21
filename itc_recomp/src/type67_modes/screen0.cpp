// The type 6 and 7 first screens' helpers (docs/functions/type6-type7-modes.md):
// the confirms that fill the yes/no dialog (movie 16) and post its open
// (Type6Screen0::Confirm, ConfirmAgain, Type7Screen0::Confirm), and the type
// 6 menu's page pictures (Type6Screen0::ShowPage). UiMovies::Find,
// UiMovie::SetText (Strings::FormatV's guest calls through FormatCalls) and
// the node setters through their plain C++.

#include "base/float_mode.h"
#include "type67_modes/type67_modes.h"
#include "type67_modes/views.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "car_makers/car_makers.h"
#include "car_specs/car_specs.h"
#include "career/career.h"
#include "guest.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "mode_tick/mode_tick.h"
#include "power_graph/power_graph.h"
#include "race_setup/race_setup.h"
#include "stand_models/stand_models.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"
#include "xaudio/ppc_float.h"

#include <cstdlib>
#include <utility>

namespace type67_modes {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::Type6Type7ModesGlobals;

// The movie with id `id`: the id at the frame's +80, then UiMovies::Find.
uint32_t MovieAt(uint8_t* base, uint32_t frame, uint32_t id) {
  StoreU32(base, frame + 80, id);
  return schema::UiMovieWord(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80)).value();
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
  const schema::ModeDispatcher d(base, dispatcher);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t end = d.ring_start() + count - 1;
  d.set_ring_count(count);
  const schema::ModeMessage slot(base, d.ring_address() + (end - (end / 50) * 50) * 12);
  slot.set_id(message);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

void Show(uint8_t* base, uint32_t movie, uint32_t child, uint32_t value,
          uint32_t sub_scene = 3) {
  schema::UiMovieNode(base, ui_movies::FindNode(base, movie, sub_scene, child)).set_visible(value);
}

uint32_t Texture(uint8_t* base, uint32_t movie, uint32_t child, uint32_t texture,
                 uint32_t sub_scene = 3) {
  return ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, sub_scene, child),
                                   texture);
}

// UiMovie::HideNode's work: the node's +80 = 0; the node (its r3).
uint32_t Hide(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  schema::UiMovieNode(base, node).set_visible(0);
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

// PowerGraph::SetFromCar's guest calls (where its callees' plain C++
// declines) through `calls`.
power_graph::SetFromCarCall SetFromCarCalls(const Calls& calls) {
  return [&calls](uint32_t function, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t frame,
                  uint32_t lr) {
    const uint64_t args[6] = {r3, r4, r5, 0, 0, 0};
    calls.call_set(function, args, 3, frame, lr);
  };
}

constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kCommand = 0x82180108;      // GarageStage::Command
constexpr uint32_t kGarageCarReset = 0x82110448;
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kMinusOne = 0xFFFFFFFF;

// Flush-to-zero off, as the car's plain C++ expects at entry.
void FlushOff() {
  using Fpscr = ::base::FloatMode;
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
}

// The selected garage car's node: the list at `garage` +204 walked
// [save +0x19920] times through the halfword links (each node 4 bytes at
// `garage` + 4 x its index, sign-extended), as the originals' loops do.
uint32_t GarageNode(uint8_t* base, uint32_t garage, uint32_t steps) {
  uint32_t node =
      garage + (uint32_t(int32_t(int16_t(schema::CareerGarageList(base, garage).head()))) << 2);
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
  StoreU32(base, frame, sp);  // the back chain (stwu)
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
  StoreU32(base, sp - 112, sp);  // the back chain (stwu)
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
      schema::UiMovieNode(base, node).set_visible(0);
      return node;
    }
  }
}

uint64_t Type7Screen0ShowPage(uint8_t* base, uint32_t page, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82244930 Type7Screen0::ShowPage");
  StoreU32(base, sp - 112, sp);  // the back chain (stwu)
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

uint64_t Screen0PlaceCursor(uint8_t* base, bool type7, uint64_t item, uint32_t sp) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_822448B0 Type7Screen0::PlaceCursor");
  } else {
    ITC_NATIVE_RUNNING("sub_82234428 Type6Screen0::PlaceCursor");
  }
  const uint32_t frame = sp - 112;
  StoreU32(base, frame, sp);  // the back chain (stwu)
  const uint32_t movie = MovieAt(base, frame, 22);
  const uint32_t node = ui_movies::FindNode(base, movie, 0xFFFFFFFF, type7 ? 3 : 4);
  // mulli and extsw, the doubleword through the frame's +80 (std, lfd,
  // fcfid, frsp).
  const int64_t value = int64_t(int32_t(uint32_t(item * 40)));
  StoreU32(base, frame + 80, uint32_t(uint64_t(value) >> 32));
  StoreU32(base, frame + 84, uint32_t(value));
  const uint32_t child = ui_movies::Child(base, node, 0);
  schema::UiMovieNode(base, child).set_y(float(double(value)));
  return child;
}

uint64_t Screen0ShowHelp(uint8_t* base, bool type7, uint32_t item, uint32_t sp,
                         const strings_and_font::FormatCalls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82244AB8 Type7Screen0::ShowHelp");
  } else {
    ITC_NATIVE_RUNNING("sub_822346A8 Type6Screen0::ShowHelp");
  }
  const uint32_t frame = sp - 128;
  StoreU32(base, frame, sp);  // the back chain (stwu)
  const uint32_t movie = MovieAt(base, frame, 22);
  static constexpr uint32_t kType6Texts[5] = {920, 921, 916, 917, 918};
  static constexpr uint32_t kType7Texts[4] = {919, 916, 917, 918};
  uint32_t text = 919;
  if (type7) {
    if (item < 4) text = kType7Texts[item];
  } else if (item < 5) {
    text = kType6Texts[item];
  }
  const uint32_t sub_scene = type7 ? 1 : 2;
  Required(ui_movies::ResetTextScroll(base, movie, sub_scene, 0), "sub_8211C5F8");
  SetText(base, calls, frame, movie, sub_scene, 0, text);
  return Required(ui_movies::ScrollText(base, movie, sub_scene, 0, 1), "UiMovie::ScrollText");
}

uint64_t Screen0FadeOutStep(uint8_t* base, bool type7, uint32_t screen, uint32_t sp,
                            const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82244760 Type7Screen0::FadeOutStep");
  } else {
    ITC_NATIVE_RUNNING("sub_822342F0 Type6Screen0::FadeOutStep");
  }
  const uint32_t frame = sp - 112;
  const uint32_t movie = MovieAt(base, frame, 22);
  const schema::PageObject page = Screen0View(base, screen, type7).pages();
  if (page.step_started() == 0) {
    mode_tick::ScreenFadesFadeOut(base, schema::Type67ScreensGlobals::fade_out_passes(base), 0, 0, 0, 0);
    page.set_step_started(1);
  }
  calls.call(kRun, frame, type7 ? 0x822447D4 : 0x82234364, movie, 0);
  const bool done = (schema::Type67ScreensGlobals::fade_out_count(base) |
                     schema::Type67ScreensGlobals::fade_in_count(base)) == 0;
  if (done) {
    calls.call(kFinishAll, frame, type7 ? 0x822447FC : 0x8223438C, movie, 0);
  }
  return done ? 1 : 0;
}

namespace {

constexpr uint32_t kFinishNamed = 0x8211DF20, kPlayNamed = 0x8211DAC8, kLoopNamed = 0x8211DDE0,
                   kUnloopNamed = 0x8211DE30;

// UiMovie::PlayNamed(movie, named, chained, 0, 0, 0), a guest call.
uint64_t PlayNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t named,
                   uint64_t chained) {
  const uint64_t args[6] = {movie, named, chained, 0, 0, 0};
  return calls.call_set(kPlayNamed, args, 6, frame, lr);
}

// The menu (type 6 +9648, type 7 +9652) and the page state (+9644, +9648).
schema::FrontEndMenu MenuOf(uint8_t* base, bool type7, uint32_t screen) {
  return Screen0View(base, screen, type7).menu();
}
schema::PageObject PageStateOf(uint8_t* base, bool type7, uint32_t screen) {
  return Screen0View(base, screen, type7).pages();
}

uint64_t ShowPageOf(uint8_t* base, bool type7, uint32_t page, uint32_t sp) {
  return type7 ? Type7Screen0ShowPage(base, page, sp) : Type6Screen0ShowPage(base, page, sp);
}

}  // namespace

uint64_t Screen0PlayItem(uint8_t* base, bool type7, uint32_t screen, uint32_t sp,
                         const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82244818 Type7Screen0::PlayItem");
  } else {
    ITC_NATIVE_RUNNING("sub_822343A8 Type6Screen0::PlayItem");
  }
  const uint32_t frame = sp - 112;
  StoreU32(base, frame, sp);  // the back chain (stwu)
  const uint32_t movie = MovieAt(base, frame, 22);
  const uint64_t named = uint64_t(MenuOf(base, type7, screen).cursor()) + 5;
  if (type7) {
    calls.call(kFinishNamed, frame, 0x82244864, movie, named);
    calls.call(kUnloopNamed, frame, 0x82244870, movie, named);
  }
  PlayNamed(calls, frame, type7 ? 0x8224488C : 0x82234404, movie, named, 1);
  return calls.call(kLoopNamed, frame, type7 ? 0x82244898 : 0x82234410, movie, named);
}

uint64_t Screen0OpeningStep(uint8_t* base, bool type7, uint32_t screen, uint32_t sp,
                            const Calls& calls, const strings_and_font::FormatCalls& format) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82244E70 Type7Screen0::OpeningStep");
  } else {
    ITC_NATIVE_RUNNING("sub_82234C20 Type6Screen0::OpeningStep");
  }
  const uint32_t frame = sp - 128;
  const uint32_t movie = MovieAt(base, frame, 22);
  const schema::PageObject page = PageStateOf(base, type7, screen);
  auto item = [&] { return MenuOf(base, type7, screen).cursor(); };
  if (page.step_started() == 0) {
    if (type7) {
      calls.call(kFinishAll, frame, 0x82244EB4, movie, 0);
      calls.call(kFinishNamed, frame, 0x82244EC0, movie, 48);
      PlayNamed(calls, frame, 0x82244EDC, movie, 48, 0);
      calls.call(kFinishNamed, frame, 0x82244EE8, movie, 0);
      PlayNamed(calls, frame, 0x82244F04, movie, 0, 1);
      calls.call(kLoopNamed, frame, 0x82244F10, movie, 0);
    } else {
      calls.call(kFinishAll, frame, 0x82234C64, movie, 0);
      PlayNamed(calls, frame, 0x82234C80, movie, 48, 0);
      PlayNamed(calls, frame, 0x82234C9C, movie, 0, 1);
      calls.call(kLoopNamed, frame, 0x82234CA8, movie, 0);
    }
    mode_tick::ScreenFadesFadeIn(base, schema::Type67ScreensGlobals::fade_in_passes(base), 0, 0, 0, 0);
    Screen0PlaceCursor(base, type7, item(), frame);
    ShowPageOf(base, type7, 0, frame);
    Screen0ShowHelp(base, type7, item(), frame, format);
    if (type7) {
      Screen0PlayItem(base, true, screen, frame, calls);
    }
    page.set_step_started(1);
  }
  const uint64_t run = calls.call(kRun, frame, type7 ? 0x82244F74 : 0x82234D04, movie, 0);
  const bool done = uint32_t(run) == 0;
  if (done) {
    if (type7) {
      calls.call(kFinishNamed, frame, 0x82244F8C, movie, 43);
      PlayNamed(calls, frame, 0x82244FA8, movie, 43, 1);
      calls.call(kLoopNamed, frame, 0x82244FB4, movie, 43);
    } else {
      Screen0PlayItem(base, false, screen, frame, calls);
      PlayNamed(calls, frame, 0x82234D34, movie, 43, 1);
      calls.call(kLoopNamed, frame, 0x82234D40, movie, 43);
    }
  }
  return done ? 1 : 0;
}

uint64_t Screen0BackStep(uint8_t* base, bool type7, uint32_t screen, uint32_t sp,
                         const Calls& calls, const strings_and_font::FormatCalls& format) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_822450A8 Type7Screen0::BackStep");
  } else {
    ITC_NATIVE_RUNNING("sub_82234E38 Type6Screen0::BackStep");
  }
  const uint32_t frame = sp - 128;
  const uint32_t movie = MovieAt(base, frame, 22);
  const uint32_t items = type7 ? 4 : 5;
  for (uint32_t i = 0; i < items; i++) {
    calls.call(kFinishNamed, frame, type7 ? 0x822450E4 : 0x82234E74, movie, i + 5);
  }
  calls.call(kFinishNamed, frame, type7 ? 0x822450FC : 0x82234E8C, movie, 15);
  calls.call(kFinishNamed, frame, type7 ? 0x82245108 : 0x82234E98, movie, 16);
  const uint32_t item = MenuOf(base, type7, screen).cursor();
  Screen0PlaceCursor(base, type7, item, frame);
  if (type7) {
    Screen0ShowHelp(base, true, item, frame, format);
    Screen0PlayItem(base, true, screen, frame, calls);
  } else {
    Screen0PlayItem(base, false, screen, frame, calls);
    Screen0ShowHelp(base, false, item, frame, format);
  }
  calls.call(kRun, frame, type7 ? 0x8224513C : 0x82234ECC, movie, 0);
  return 1;
}

uint64_t Screen0CursorStep(uint8_t* base, bool type7, uint32_t screen, uint32_t sp,
                           const Calls& calls, const strings_and_font::FormatCalls& format) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82244FC0 Type7Screen0::CursorStep");
  } else {
    ITC_NATIVE_RUNNING("sub_82234D50 Type6Screen0::CursorStep");
  }
  const uint32_t frame = sp - 144;
  const uint32_t movie = MovieAt(base, frame, 22);
  const schema::FrontEndMenu menu = MenuOf(base, type7, screen);
  const uint32_t old = menu.previous();
  const uint32_t now = menu.cursor();
  const uint64_t named = uint64_t(old) + 5;
  const int32_t moved = int32_t(now - old);
  calls.call(kFinishNamed, frame, type7 ? 0x8224500C : 0x82234D9C, movie, named);
  calls.call(kUnloopNamed, frame, type7 ? 0x82245018 : 0x82234DA8, movie, named);
  calls.call(kFinishNamed, frame, type7 ? 0x82245024 : 0x82234DB4, movie, 15);
  calls.call(kFinishNamed, frame, type7 ? 0x82245030 : 0x82234DC0, movie, 16);
  if (uint32_t(moved + 1) <= 2) {
    if (moved != 0) {
      PlayNamed(calls, frame, type7 ? 0x82245060 : 0x82234DF0, movie, moved > 0 ? 16 : 15,
                type7 ? 1 : 0);
    }
  } else {
    Screen0PlaceCursor(base, type7, now, frame);
  }
  if (type7) {
    Screen0ShowHelp(base, true, now, frame, format);
    Screen0PlayItem(base, true, screen, frame, calls);
  } else {
    Screen0PlayItem(base, false, screen, frame, calls);
    Screen0ShowHelp(base, false, now, frame, format);
  }
  calls.call(kRun, frame, type7 ? 0x8224509C : 0x82234E2C, movie, 0);
  return 1;
}

uint64_t Type6Screen0QuitPage(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82234BF0 Type6Screen0::QuitPage");
  const uint32_t word = schema::SaveScreenGlobals::data(base).prompts();
  if (word == 0) {
    return Type6Screen0ShowPage(base, 4, sp);  // a tail call
  }
  if (word == 1) {
    return Type6Screen0ShowPage(base, 3, sp);
  }
  return r3;
}

uint64_t Type6Screen0ChosenPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82234190 Type6Screen0::ChosenPage");
  const schema::Type6Screen0 screen(base, uint32_t(r3));
  const uint32_t frame = sp - 112;
  const uint32_t item = screen.menu().cursor();
  if (item > 4) {
    return r3;
  }
  if (item == 4) {
    screen.set_state(10);
    return r3;
  }
  screen.set_state(9);
  // r6 as the post leaves it (the ring's start plus its count, 64-bit),
  // passed on to the slot below.
  const schema::ModeDispatcher d(base, dispatcher);
  const uint64_t r6 = uint64_t(d.ring_start()) + uint64_t(d.ring_count());
  static constexpr uint32_t kMessages[4] = {14, 16, 18, 26};
  Post(base, dispatcher, kMessages[item]);
  if (item != 0) {
    return r3;
  }
  // The music: bank 1's cue 2 stopped (the sound manager's slot 13), bank
  // 1's cue 0 played (Sound::Play), the indexes in the frame.
  StoreU32(base, frame + 84, 1);
  StoreU32(base, frame + 80, 2);
  const schema::AudioManager manager = schema::MenuSoundsGlobals::sound_manager(base);
  const uint64_t args[6] = {manager.address, frame + 84, frame + 80, r6, 0, 0};
  calls.call_set(manager.vtable().stop_cue(), args, 6, frame, 0x822342C0);
  StoreU32(base, frame + 92, 1);
  StoreU32(base, frame + 88, 0);
  return menu_sounds::PlaySound(base, frame + 92, frame + 88, frame, Sounds(calls));
}

uint64_t Screen0PowerGraphPage(uint8_t* base, bool type7, uint64_t r3, uint64_t r4, uint32_t sp,
                               const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82244E00 Type7Screen0::PowerGraphPage");
  } else {
    ITC_NATIVE_RUNNING("sub_82234B80 Type6Screen0::PowerGraphPage");
  }
  constexpr uint32_t kDrawCarPowerGraph = 0x82250700;
  const Screen0View screen(base, uint32_t(r3), type7);
  const uint32_t frame = sp - 96;
  uint64_t r4_out = r4;
  if (screen.words().pick() == 2) {
    const schema::PageObject p = screen.pages();
    const uint32_t page = p.page();
    if (page != 0) p.set_previous_page(page);
    p.set_step(kMinusOne);
    p.set_page(0);
    p.set_queued_step(kMinusOne);
    screen.words().set_pick(kMinusOne);
    ShowPageOf(base, type7, 0, frame);
    r4_out = 0;
  }
  // The car's address as lis and addi leave it (sign-extended).
  const uint64_t car = 0xFFFFFFFF00000000ull | (type7 ? kType7Car : kType6Car);
  return calls.call(kDrawCarPowerGraph, frame, type7 ? 0x82244E60 : 0x82234BE0, car, r4_out);
}

uint64_t Type7Screen0ChosenPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82244610 Type7Screen0::ChosenPage");
  const schema::Type7Screen0 screen(base, uint32_t(r3));
  const uint32_t frame = sp - 112;
  const uint32_t item = screen.menu().cursor();
  if (item == 3) {
    screen.set_state(10);
    return r3;
  }
  if (item > 3) {
    return r3;
  }
  screen.set_state(9);
  // r6 as the post leaves it (the ring's start plus its count, 64-bit),
  // passed on to the slot below.
  const schema::ModeDispatcher d(base, dispatcher);
  const uint64_t r6 = uint64_t(d.ring_start()) + uint64_t(d.ring_count());
  Post(base, dispatcher, item == 0 ? 13 : item == 1 ? 15 : 23);
  if (item != 0) {
    return r3;
  }
  // The music: bank 1's cue 2 stopped (the sound manager's slot 13), bank
  // 1's cue 0 played (Sound::Play), the indexes in the frame.
  StoreU32(base, frame + 84, 1);
  StoreU32(base, frame + 80, 2);
  const schema::AudioManager manager = schema::MenuSoundsGlobals::sound_manager(base);
  const uint64_t args[4] = {manager.address, frame + 84, frame + 80, r6};
  calls.call_set(manager.vtable().stop_cue(), args, 4, frame, 0x82244730);
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
  const uint32_t holder = G::stand_holder(base).stage_block_0();
  // GarageStage::MakeModel by its plain C++ (its adapter went, B7 loop,
  // 2026-11-13), ScenePools::SetUpB under it through call_set.
  const stand_models::GuestCall call = [&](uint32_t function,
                                           std::initializer_list<uint64_t> args, uint32_t r1,
                                           uint32_t call_lr) {
    uint64_t regs[6] = {};
    int set = 0;
    for (const uint64_t v : args) {
      regs[set++] = v;
    }
    return calls.call_set(function, regs, set, r1, call_lr);
  };
  stand_models::MakeModel(base, call, holder, block, 1, frame);
  return calls.call(kCommand, frame, lr + 0x30, G::stand_holder(base).stage_block_0(), 14);
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
  if (schema::Type67Car(base, car).source() == 0xFFFFFFFF) {
    calls.call(kGarageCarReset, frame, type7 ? 0x82244418 : 0x82233EB8, car, r4);
    const schema::CareerSave saved(base, save);
    if (saved.garage_count() == 0) {
      message = type7 ? 17 : 20;
    } else {
      const uint32_t garage = saved.garage_address();
      const schema::CareerGarageList list(base, garage);
      const int32_t slot = int16_t(guest::LoadU16(base, GarageNode(base, garage, saved.selected_car()) + 2));
      const uint32_t from = uint32_t(slot) * 1968 + list.cars_address();
      if (!c_runtime::Memcpy(base, car, from, 1968)) {
        const uint64_t args[4] = {car, from, 1968, 0};
        calls.call_set(kMemcpy, args, 3, frame, type7 ? 0x82244478 : 0x82233F18);
      }
      schema::Type67Car(base, car).set_source(2);
      schema::Type67Car(base, car).set_garage_index(saved.selected_car());
      const int32_t again =
          int16_t(guest::LoadU16(base, GarageNode(base, garage, saved.selected_car()) + 2));
      schema::Type67Car(base, car).set_paint_word(
               uint32_t(int32_t(int16_t(
                   schema::CarMakersCar(base, uint32_t(again) * 1968 + list.cars_address())
                       .paint_pick()))));
    }
  }
  const schema::ModeDispatcher d(base, dispatcher);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t end = d.ring_start() + count - 1;
  d.set_ring_count(count);
  const schema::ModeMessage slot(base, d.ring_address() + (end - (end / 50) * 50) * 12);
  slot.set_arg1(0);
  slot.set_arg0(0);
  slot.set_id(message);
  mode_tick::ScreenFadesFadeOut(base, 1, 0, 0, 0, 0);
  return 1;  // FadeOut leaves its r3, the passes
}

uint64_t Type6Screen0OnDialogClosed(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82234768 Type6Screen0::OnDialogClosed");
  const schema::Type6Screen0 screen(base, uint32_t(r3));
  const uint32_t frame = sp - 96;
  const uint32_t asked = screen.confirm_asked();
  if (asked != 4 && asked != 5) {
    return r3;
  }
  screen.set_confirm_asked(0xFFFFFFFF);
  const uint32_t pick = screen.words().pick();
  if (pick >= 3) {
    return r3;
  }
  const schema::PageObject p = screen.pages();
  auto clear_pick = [&] { screen.words().set_pick(0xFFFFFFFF); };
  auto page0 = [&] {
    const uint32_t page = p.page();
    if (page != 0) p.set_previous_page(page);
    p.set_page(0);
    p.set_step(0xFFFFFFFF);
    p.set_queued_step(0xFFFFFFFF);
    clear_pick();
    return Type6Screen0ShowPage(base, 0, frame);
  };
  if (asked == 4) {
    if (pick != 1) {
      return page0();
    }
    // Yes to the first confirm: the second one (ConfirmAgain, a guest call
    // to its override).
    const uint32_t page = p.page();
    if (page != 0xFFFFFFFF) p.set_previous_page(page);
    p.set_page(0xFFFFFFFF);
    p.set_step(0xFFFFFFFF);
    p.set_queued_step(0xFFFFFFFF);
    clear_pick();
    calls.call(0x822340B0, frame, 0x82234810, r3, r4);
    screen.set_unknown_9656(0);
    screen.set_confirm_asked(5);
    return Type6Screen0ShowPage(base, 2, frame);
  }
  if (pick == 2) {
    return page0();
  }
  if (pick == 0) {
    // Yes to the second: the fade out step, then page 3.
    const uint32_t page = p.page();
    if (page != 3) p.set_previous_page(page);
    p.set_queued_step(0xFFFFFFFF);
    p.set_page(0xFFFFFFFF);
    p.set_step(3);
    p.set_next_page(3);
    clear_pick();
    return r3;
  }
  // No to the second: page 2, message 29 (the save screen).
  const uint32_t page = p.page();
  if (page != 2) p.set_previous_page(page);
  p.set_step(0xFFFFFFFF);
  p.set_queued_step(0xFFFFFFFF);
  p.set_page(2);
  clear_pick();
  Post(base, uint32_t(r4), 29);
  return Type6Screen0ShowPage(base, 3, frame);
}

uint64_t Type7Screen0OnDialogClosed(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82244B60 Type7Screen0::OnDialogClosed");
  const schema::Type7Screen0 screen(base, uint32_t(r3));
  const uint32_t pick = screen.words().pick();
  if (pick >= 3) {
    return r3;
  }
  const schema::PageObject p = screen.pages();
  const uint32_t page = p.page();
  if (pick == 1) {
    if (page != 2) p.set_previous_page(page);
    p.set_queued_step(0xFFFFFFFF);
    p.set_step(3);
    p.set_next_page(2);
    p.set_page(0xFFFFFFFF);
    screen.words().set_pick(0xFFFFFFFF);
    return r3;
  }
  if (page != 0) p.set_previous_page(page);
  p.set_step(0xFFFFFFFF);
  p.set_page(0);
  p.set_queued_step(0xFFFFFFFF);
  screen.words().set_pick(0xFFFFFFFF);
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
  const schema::PageObject page = G::controller_page(base);
  const uint32_t step = page.step();
  for (uint32_t i = 0; i < 11; i++) {
    StoreU32(base, frame + 80 + 4 * i, kSteps[i]);
  }
  // r3 and r4 as the table's build leaves them (sign-extended addresses).
  uint64_t r3 = 0xFFFFFFFF00000000ull | 0x821BFD08;
  constexpr uint64_t kR4 = 0xFFFFFFFF00000000ull | 0x821BFD98;
  if (step == 0xFFFFFFFF) {
    return r3;
  }
  schema::PageObject p = page;
  if (int32_t(step) >= 0) {
    const uint32_t target = LoadU32(base, frame + 80 + (step << 2));
    r3 = calls.call(target, frame, 0x821C8080, uint64_t(page.step_started_address()), kR4);
    p = G::controller_page(base);
    if (int32_t(r3) == 0) {
      return r3;
    }
  }
  const uint32_t next = p.queued_step();
  if (next != 0xFFFFFFFF) {
    p.set_queued_step(0xFFFFFFFF);
    p.set_unknown_20(0);
    p.set_step(next);
  } else {
    p.set_page(p.next_page());
    p.set_step(0xFFFFFFFF);
  }
  p.set_step_started(0);
  return r3;
}

uint64_t Type7Screen0MenuPage(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls, const strings_and_font::FormatCalls* format) {
  ITC_NATIVE_RUNNING("sub_82244BF8 Type7Screen0::MenuPage");
  constexpr uint32_t kConfirm = 0x82244528;  // Type7Screen0::Confirm (the original)
  const schema::Type7Screen0 screen(base, uint32_t(r3));
  const uint32_t dispatcher = uint32_t(r4);
  const uint32_t frame = sp - 112;
  const bool slots = calls.native_action_slots;
  menu_helpers::Step(base, screen.menu().address, frame, slots, Menus(calls));
  // The page state's step: +8 = +0 unless +0 is `keep`, +0 = -1, +4 =
  // `after`, +16 = -1, +12 = `step` (the originals' shared tail).
  auto to_step = [&](uint32_t keep, uint32_t step, uint32_t after) {
    const schema::PageObject p = screen.pages();
    const uint32_t page = p.page();
    if (page != keep) p.set_previous_page(page);
    p.set_page(0xFFFFFFFF);
    p.set_next_page(after);
    p.set_queued_step(0xFFFFFFFF);
    p.set_step(step);
  };
  // The page state shown at once: +12 = +16 = -1, then ShowPage.
  auto show = [&](uint32_t page) {
    const schema::PageObject p = screen.pages();
    p.set_step(0xFFFFFFFF);
    p.set_queued_step(0xFFFFFFFF);
    return Type7Screen0ShowPage(base, page, frame);
  };
  if (screen.menu().moved() != 0) {
    const uint64_t answer = menu_sounds::PlayMenuSound(base, 4, frame, Sounds(calls));
    to_step(0, 2, 0);
    return answer;
  }
  if (int32_t(menu_helpers::Pressed(base, screen.menu().address, 163, frame, slots,
                                    Menus(calls))) != 0) {
    menu_sounds::PlayMenuSound(base, 0, frame, Sounds(calls));
    // lwz, then addi: the tab word.
    power_graph::SetFromCar(base, kType7Car, LoadU32(base, kType7Car + 1972) + 3, frame,
                            SetFromCarCalls(calls));
    Post(base, dispatcher, 18);
    const schema::PageObject p = screen.pages();
    const uint32_t page = p.page();
    if (int32_t(page) != 1) p.set_previous_page(page);
    p.set_page(1);
    return show(1);
  }
  if (int32_t(menu_helpers::Pressed(base, screen.menu().address, 159, frame, slots,
                                    Menus(calls))) != 0) {
    const uint64_t answer = menu_sounds::PlayMenuSound(base, 0, frame, Sounds(calls));
    const schema::FrontEndMenu menu = screen.menu();
    screen.set_item_picked(menu.cursor());
    const uint32_t item = menu.cursor();
    if (item < 3) {
      to_step(2, 3, 2);
      return answer;
    }
    if (item != 3) {
      return answer;
    }
    if (format != nullptr) {
      Screen0Confirm(base, kType7Confirm, dispatcher, frame, *format);
    } else {
      calls.call(kConfirm, frame, 0x82244D44, r3, r4);  // the original
    }
    const schema::PageObject p = screen.pages();
    const uint32_t page = p.page();
    if (int32_t(page) != -1) p.set_previous_page(page);
    p.set_page(0xFFFFFFFF);
    return show(2);
  }
  const uint64_t answer = menu_helpers::Pressed(base, screen.menu().address, 158, frame,
                                                slots, Menus(calls));
  if (int32_t(answer) == 0) {
    return answer;
  }
  const uint64_t played = menu_sounds::PlayMenuSound(base, 2, frame, Sounds(calls));
  const schema::FrontEndMenu menu = screen.menu();
  const uint32_t item = menu.cursor();
  menu.set_cursor(3);
  menu.set_previous(item);
  to_step(0, 1, 0);
  return played;
}

uint64_t Type6Screen0MenuPage(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls, const strings_and_font::FormatCalls* format) {
  ITC_NATIVE_RUNNING("sub_82234958 Type6Screen0::MenuPage");
  constexpr uint32_t kConfirm = 0x82233FC8;  // Type6Screen0::Confirm (the original)
  const schema::Type6Screen0 screen(base, uint32_t(r3));
  const uint32_t dispatcher = uint32_t(r4);
  const uint32_t frame = sp - 112;
  const bool slots = calls.native_action_slots;
  auto menu = [&] { return screen.menu().address; };
  // The page state's step: +8 = +0 unless +0 is `keep`, +0 = -1, +4 =
  // `after`, +16 = -1, +12 = `step` (the original's shared tail).
  auto to_step = [&](uint32_t keep, uint32_t step, uint32_t after) {
    const schema::PageObject p = screen.pages();
    const uint32_t page = p.page();
    if (page != keep) p.set_previous_page(page);
    p.set_page(kMinusOne);
    p.set_next_page(after);
    p.set_queued_step(kMinusOne);
    p.set_step(step);
  };
  // A page shown at once: +8 = +0 unless +0 is `page`, +0 = `page`, +12 =
  // +16 = -1.
  auto to_page = [&](uint32_t page_now) {
    const schema::PageObject p = screen.pages();
    const uint32_t page = p.page();
    if (page != page_now) p.set_previous_page(page);
    p.set_page(page_now);
    p.set_step(kMinusOne);
    p.set_queued_step(kMinusOne);
  };
  menu_helpers::Step(base, menu(), frame, slots, Menus(calls));
  if (screen.menu().moved() != 0) {
    const uint64_t answer = menu_sounds::PlayMenuSound(base, 4, frame, Sounds(calls));
    to_step(0, 2, 0);
    return answer;
  }
  if (int32_t(menu_helpers::Pressed(base, menu(), 163, frame, slots, Menus(calls))) != 0) {
    menu_sounds::PlayMenuSound(base, 0, frame, Sounds(calls));
    // lwz, then addi: the tab word.
    power_graph::SetFromCar(base, kType6Car, LoadU32(base, kType6Car + 1972) + 3, frame,
                            SetFromCarCalls(calls));
    Post(base, dispatcher, 21);
    to_page(1);
    return Type6Screen0ShowPage(base, 1, frame);
  }
  if (int32_t(menu_helpers::Pressed(base, menu(), 159, frame, slots, Menus(calls))) != 0) {
    const uint64_t answer = menu_sounds::PlayMenuSound(base, 0, frame, Sounds(calls));
    const schema::FrontEndMenu m = screen.menu();
    screen.set_item_picked(m.cursor());
    const uint32_t item = m.cursor();
    if (item < 4) {
      to_step(3, 3, 3);
      return answer;
    }
    if (item != 4) {
      return answer;
    }
    if (format != nullptr) {
      Screen0Confirm(base, kType6Confirm, dispatcher, frame, *format);
    } else {
      calls.call(kConfirm, frame, 0x82234AA4, r3, r4);  // the original
    }
    to_page(kMinusOne);
    screen.set_unknown_9656(0);
    screen.set_confirm_asked(4);
    return Type6Screen0ShowPage(base, 2, frame);
  }
  const uint64_t answer = menu_helpers::Pressed(base, menu(), 158, frame, slots, Menus(calls));
  if (int32_t(answer) == 0) {
    return answer;
  }
  const uint64_t played = menu_sounds::PlayMenuSound(base, 2, frame, Sounds(calls));
  const schema::FrontEndMenu m = screen.menu();
  const uint32_t item = m.cursor();
  m.set_cursor(4);
  m.set_previous(item);
  to_step(0, 1, 0);
  return played;
}

uint64_t Screen0ShowCarInfo(uint8_t* base, bool type7, uint32_t sp, const Calls& calls,
                            const strings_and_font::FormatCalls& format) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_822440D8 Type7Screen0::ShowCarInfo");
  } else {
    ITC_NATIVE_RUNNING("sub_82233B70 Type6Screen0::ShowCarInfo");
  }
  constexpr uint32_t kRefresh = 0x820C4998;
  const uint32_t frame = sp - (type7 ? kType7ShowCarInfoFrame : kType6ShowCarInfoFrame);
  StoreU32(base, frame, sp);  // the back chain, for the guest calls
  const uint32_t movie = MovieAt(base, frame, 22);
  ui_movies::ResetPose(base, movie);
  // The menu's labels, from a table in the frame (+96).
  static constexpr uint32_t kType6Labels[] = {109, 120, 110, 113, 114};
  static constexpr uint32_t kType7Labels[] = {109, 110, 113, 114};
  const uint32_t* labels = type7 ? kType7Labels : kType6Labels;
  const uint32_t count = type7 ? 4 : 5;
  for (uint32_t i = 0; i < count; i++) {
    StoreU32(base, frame + 96 + 4 * i, labels[i]);
  }
  for (uint32_t i = 0; i < count; i++) {
    SetText(base, format, frame, movie, type7 ? 3 : 4, 5 + i, LoadU32(base, frame + 96 + 4 * i));
  }
  // The car object in the frame: built from the type 6 car's id, or the
  // type 7 car copied, then refreshed.
  const uint32_t car = frame + (type7 ? 112 : 128);
  const uint32_t from = type7 ? kType7Car : kType6Car;
  FlushOff();
  race_setup::CarInit(base, car);
  if (type7) {
    if (!c_runtime::Memcpy(base, car, from, 1884)) {
      const uint64_t args[4] = {car, from, 1884, 0};
      calls.call_set(0x825A7410, args, 3, frame, 0x82244174);
    }
  } else {
    race_setup::SetModel(base, car, schema::CarMakersCar(base, from).car_id());
    const schema::CarMakersCar object(base, car);
    object.set_flags(object.flags() | 1);
  }
  FlushOff();
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(base, car, &refreshed)) {
    calls.call(kRefresh, frame, type7 ? 0x8224417C : 0x82233C24, car, 0);
  }
  FlushOff();
  const uint32_t block2 = vehicle_accessors::ObjectRecord1888(car);
  const uint32_t first = type7 ? 12 : 10;
  SetText(base, format, frame, movie, 0xFFFFFFFF, first, guest::LoadU8(base, block2 + 129) + 1934);
  SetText(base, format, frame, movie, 0xFFFFFFFF, first + 1,
          schema::CarMakersCar(base, from).car_id() + 1646);
  return SetText(base, format, frame, movie, 0xFFFFFFFF, first + 2,
                 schema::CarMakersCar(base, from).car_id() + 1718);
}

uint64_t Screen0Show(uint8_t* base, bool type7, uint64_t r3, uint32_t sp, const Calls& calls,
                     const strings_and_font::FormatCalls& format) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_822441D8 Type7Screen0::Show");
  } else {
    ITC_NATIVE_RUNNING("sub_82233C80 Type6Screen0::Show");
  }
  const Screen0View screen(base, uint32_t(r3), type7);
  const uint32_t frame = sp - kShowFrame;
  StoreU32(base, frame, sp);  // the back chain, for the guest calls
  const uint32_t car = type7 ? kType7Car : kType6Car;
  schema::Type67Car(base, car).set_model_made(0);
  screen.words().set_loading_shown(0);
  if (schema::Type67Car(base, car).source() == 0xFFFFFFFF) {
    const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
    calls.call(kGarageCarReset, frame, type7 ? 0x82244224 : 0x82233CCC, car, 0);
    uint32_t word = 0;  // +1980
    const schema::CareerSave saved(base, save);
    if (saved.garage_count() == 0) {
      FlushOff();
      career::CarSetModelAndClass(base, car, 0, frame);
      // The garage car's head (car-makers' CarMakersCar).
      const schema::CarMakersCar head(base, car);
      if (head.body_class() == 0xFFFF) {
        head.set_body_class(0);
      }
      head.set_unknown_2(head.paint_pick());
      head.set_paint_pick(0);
      schema::Type67Car(base, car).set_source(0);
      schema::Type67Car(base, car).set_garage_index(0);
    } else {
      const uint32_t garage = saved.garage_address();
      const schema::CareerGarageList list(base, garage);
      const int32_t slot =
          int16_t(guest::LoadU16(base, GarageNode(base, garage, saved.selected_car()) + 2));
      const uint32_t source = uint32_t(slot) * 1968 + list.cars_address();
      if (!c_runtime::Memcpy(base, car, source, 1968)) {
        const uint64_t args[4] = {car, source, 1968, 0};
        calls.call_set(kMemcpy, args, 3, frame, type7 ? 0x822442B8 : 0x82233D60);
      }
      schema::Type67Car(base, car).set_source(2);
      schema::Type67Car(base, car).set_garage_index(saved.selected_car());
      const int32_t again =
          int16_t(guest::LoadU16(base, GarageNode(base, garage, saved.selected_car()) + 2));
      word = uint32_t(int32_t(int16_t(
          schema::CarMakersCar(base, uint32_t(again) * 1968 + list.cars_address()).paint_pick())));
    }
    schema::Type67Car(base, car).set_paint_word(word);
  }
  // The menu: the previous item = the cursor, the cursor = the item picked.
  const schema::FrontEndMenu menu = screen.menu();
  const uint32_t cursor = menu.cursor();
  menu.set_cursor(screen.item_picked());
  menu.set_previous(cursor);
  const uint64_t answer = Screen0ShowCarInfo(base, type7, frame, calls, format);
  const schema::PageObject p = screen.pages();
  const uint32_t page = p.page();
  if (page != 0) p.set_previous_page(page);
  p.set_step(0);
  p.set_next_page(0);
  p.set_queued_step(0xFFFFFFFF);
  p.set_page(0xFFFFFFFF);
  return answer;
}


uint64_t Screen0Update(uint8_t* base, bool type7, uint64_t r3, uint64_t sp, const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82245200 Type7Screen0::Update");
  } else {
    ITC_NATIVE_RUNNING("sub_82234F08 Type6Screen0::Update");
  }
  constexpr uint32_t kFinishNamed = 0x8211DF20, kPlayNamed = 0x8211DAC8, kLoopNamed = 0x8211DDE0;
  constexpr uint32_t kIsBusy = 0x82179BE0, kReloadModel = 0x8217F880, kSetCar = 0x8217FD58;
  constexpr uint32_t kDraw = 0x8217FE60, kRefresh = 0x820C4998;
  constexpr uint32_t kType6Steps = G::type6_steps_address();
  constexpr uint32_t kType7Steps = G::type7_steps_address();
  // The return addresses: type 6's, type 7's.
  struct Lrs {
    uint32_t step, run, busy, finish45, play45, finish44, play44, loop44, finish44b, finish46,
        play46, reload, memcpy, refresh, set_car, draw;
  };
  static constexpr Lrs kLrs[2] = {
      {0x82234F64, 0x82234FB0, 0x82234FD8, 0x82235004, 0x82235020, 0x8223502C, 0x82235048,
       0x82235054, 0x82235070, 0x8223507C, 0x82235098, 0x822350BC, 0x822350E0, 0x822350F4,
       0x82235104, 0x82235130},
      {0x822452DC, 0x82245328, 0x82245350, 0x82245374, 0x82245390, 0x8224539C, 0x822453B8,
       0x822453C4, 0x822453E8, 0x822453F4, 0x82245410, 0x8224543C, 0x82245460, 0x82245474,
       0x82245484, 0x822454B0}};
  const Lrs& lr = kLrs[type7 ? 1 : 0];
  const Screen0View screen(base, uint32_t(r3), type7);
  const uint32_t frame = uint32_t(sp) - (type7 ? kType7UpdateFrame : kType6UpdateFrame);
  const uint64_t high = sp & 0xFFFFFFFF00000000ull;  // r1's upper word, in addi's results
  StoreU32(base, frame, uint32_t(sp));  // the back chain, for the guest calls
  const uint32_t movie = MovieAt(base, frame, 22);
  uint64_t answer;
  uint32_t step;
  if (type7) {
    // The step table, built on the first call from words in the frame.
    const uint32_t guard = G::type7_steps_guard(base);
    step = screen.pages().step();
    if ((guard & 1) == 0) {
      StoreU32(base, frame + 84, 0);
      StoreU32(base, frame + 108, 0);
      StoreU32(base, frame + 92, 0);
      StoreU32(base, frame + 100, 0);
      G::set_type7_steps_guard(base, guard | 1);
      StoreU32(base, frame + 80, 0x82244E70);   // the opening
      StoreU32(base, frame + 104, 0x822450A8);  // back pressed
      StoreU32(base, frame + 88, 0x82244FC0);   // the cursor moved
      StoreU32(base, frame + 96, 0x82244760);   // the fade out
      for (const auto [to, from] : {std::pair{0u, 80u}, {8u, 104u}, {16u, 88u}, {24u, 96u}}) {
        StoreU32(base, kType7Steps + to, LoadU32(base, frame + from));
        StoreU32(base, kType7Steps + to + 4, LoadU32(base, frame + from + 4));
      }
    }
  } else {
    step = screen.pages().step();
  }
  if (step != kMinusOne) {
    if (type7) {
      const uint32_t target = LoadU32(base, kType7Steps + (step << 3));
      const uint32_t adjustment = LoadU32(base, kType7Steps + 4 + (step << 3));
      answer = calls.call(target, frame, lr.step, uint64_t(adjustment) + r3, step);
    } else {
      answer = calls.call(LoadU32(base, kType6Steps + (step << 2)), frame, lr.step, r3,
                          (high | frame) + 80);
    }
    const schema::PageObject p = screen.pages();
    if (int32_t(uint32_t(answer)) != 0) {
      const uint32_t next = p.queued_step();
      if (next == kMinusOne) {
        const uint32_t after = p.next_page();
        p.set_step(kMinusOne);
        p.set_page(after);
      } else {
        p.set_unknown_20(0);
        p.set_step(next);
        p.set_queued_step(kMinusOne);
      }
      p.set_step_started(0);
    }
  } else {
    answer = calls.call(kRun, frame, lr.run, movie, 0);
  }
  const uint32_t car = type7 ? kType7Car : kType6Car;
  if (schema::Type67Car(base, car).source() == kMinusOne) {
    return answer;
  }
  const uint32_t holder = G::stand_holder(base).stage_block_0();
  const uint64_t busy = calls.call(kIsBusy, frame, lr.busy, holder, 0);
  auto named = [&](uint32_t function, uint32_t id, uint32_t at) {
    calls.call(function, frame, at, movie, id);
  };
  auto play = [&](uint32_t id, uint32_t at) {
    const uint64_t args[6] = {movie, id, 1, 0, 0, 0};
    calls.call_set(kPlayNamed, args, 6, frame, at);
  };
  const schema::Type67Screen0Words loading = screen.words();
  const uint32_t shown = loading.loading_shown();
  if (int32_t(uint32_t(busy)) != 0) {
    if (shown == 0) {
      if (!type7) loading.set_loading_shown(1);
      named(kFinishNamed, 45, lr.finish45);
      play(45, lr.play45);
      named(kFinishNamed, 44, lr.finish44);
      play(44, lr.play44);
      named(kLoopNamed, 44, lr.loop44);
      if (type7) screen.words().set_loading_shown(1);
    }
  } else if (shown == 1) {
    if (!type7) loading.set_loading_shown(0);
    named(kFinishNamed, 44, lr.finish44b);
    named(kFinishNamed, 46, lr.finish46);
    play(46, lr.play46);
    if (type7) screen.words().set_loading_shown(0);
  }
  if (schema::Type67Car(base, car).model_made() != 0) {
    const uint64_t args[6] = {holder, 0, 1, 0, 0, 0};
    return calls.call_set(kDraw, args, 3, frame, lr.draw);
  }
  const uint32_t block = car_makers::BlockOf(schema::CarMakersCar(base, car).car_id());
  const uint64_t reloaded =
      calls.call(kReloadModel, frame, lr.reload, G::stand_holder(base).stage_block_0(),
                 uint64_t(int64_t(int32_t(block))));
  schema::Type67Car(base, car).set_model_made(uint32_t(reloaded));
  if (int32_t(uint32_t(reloaded)) != 1) {
    return reloaded;
  }
  // The car object in the frame, the mode's car copied into it.
  const uint32_t object = frame + (type7 ? 112 : 96);
  FlushOff();
  race_setup::CarInit(base, object);
  if (!c_runtime::Memcpy(base, object, car, 1884)) {
    const uint64_t args[6] = {(high | frame) + (object - frame), car, 1884, 0, 0, 0};
    calls.call_set(kMemcpy, args, 3, frame, lr.memcpy);
  }
  const schema::CarMakersCar made(base, object);
  made.set_flags(made.flags() | 1);
  FlushOff();
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(base, object, &refreshed)) {
    calls.call(kRefresh, frame, lr.refresh, (high | frame) + (object - frame), 0);
  }
  FlushOff();
  const uint64_t args[6] = {holder, (high | frame) + (object - frame), ~0ull, 0, 0, 0};
  calls.call_set(kSetCar, args, 3, frame, lr.set_car);
  // cntlzw and rlwinm: 1 for a car from the garage (+1972 2).
  return GarageStageSetPlates(base, holder, schema::Type67Car(base, car).source() == 2 ? 1 : 0);
}

}  // namespace type67_modes
