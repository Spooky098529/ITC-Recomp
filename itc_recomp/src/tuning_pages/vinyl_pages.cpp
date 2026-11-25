// The tuning screen's vinyl editor's pages (states 23 to 31 and 34 of
// TuningScreen::HandleInput) and their movie-side updates
// (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "native/buffers.h"
#include "schema/racer_controllers.h"
#include "schema/tuning_screen.h"
#include "strings_and_font/steps.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>
#include <cmath>
#include <optional>

namespace tuning_pages {

namespace {

// The screen (tuning-screen.toml's TuningScreen).
schema::TuningScreen Screen(uint32_t s) { return schema::TuningScreen(s); }
// The screen's command queue (tuning-screen.toml's TuningCommandQueue).
schema::TuningCommandQueue CommandQueue(uint32_t q) {
  return schema::TuningCommandQueue(q);
}

using G = schema::TuningPagesGlobals;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  Store(sp - bytes, sp);
  return sp - bytes;
}

constexpr uint32_t kNone = 0xFFFFFFFFu;

// The players' action mapper entries (8 bytes each: the mapper, blocked
// when +4 is 1) and their indices (a word a player).
constexpr uint32_t kMapperEntries = schema::RacerControllersGlobals::mapper_entries_address();
constexpr uint32_t kControllerEntries = schema::RacerControllersGlobals::player_entries_address();
constexpr uint32_t kHeldSlot = 0x8210BE70;     // ActionMapper::Held (slot 8, +32)
constexpr uint32_t kPressedSlot = 0x8210BF40;  // ActionMapper::Pressed (slot 10, +40)
constexpr uint32_t kFiredSlot = 0x8210C010;    // ActionMapper::Fired (slot 12, +48)

constexpr uint32_t kMenuSoundPlay = 0x821103F0;  // MenuSound::Play(cue)
constexpr uint32_t kStageCommand = 0x82180108;   // GarageStage::Command(holder, command)
constexpr uint32_t kCameraCommand = 0x821808F0;  // GarageStage::CameraCommand(holder, command)
constexpr uint32_t kFitSelectedCar = 0x8217DAC0;  // GarageStage::FitSelectedCar(holder, 0, 0)
constexpr uint32_t kMemcpy = 0x825A7410;

// The player's mapper entry (0x826BAB80 + 8 × [0x826BABA4]).
uint32_t PlayerEntry() {
  return (Load(kControllerEntries) << 3) + kMapperEntries;
}

// The mapper's read slot at `offset` (+32 slot 8, +40 slot 10, +44 slot 11,
// +48 slot 12) on `action`, as the original's bctrl from `frame` returning
// to `lr`: ActionMapper::Held's, Pressed's or Fired's plain C++ when the
// vtable holds it and `native_slots` (logged for --log_pause_menu as their
// adapters do), else the slot through `call`. Its r3.
uint64_t ReadSlot(const tuning_screen::GuestCall& call, uint32_t mapper,
                  uint32_t offset, uint64_t action, uint32_t frame, uint32_t lr,
                  bool native_slots) {
  const uint32_t function = Load(Load(mapper) + offset);
  if (native_slots) {
    const uint32_t id = uint32_t(action);
    uint32_t r3;
    int slot;
    if (function == kHeldSlot) {
      r3 = action_mappers::Held(mapper, id);
      slot = 8;
    } else if (function == kPressedSlot) {
      r3 = action_mappers::Pressed(mapper, id);
      slot = 10;
    } else if (function == kFiredSlot) {
      r3 = action_mappers::Fired(mapper, id);
      slot = 12;
    } else {
      return call(function, {mapper, action}, frame, lr);
    }
    debug::PauseMenuLog_NoteSlotRead(slot, mapper, id, lr, double(r3));
    return r3;
  }
  return call(function, {mapper, action}, frame, lr);
}

// The pages' reads of the player's mapper entry, each skipped while its +4
// is 1; the last read's r3 kept.
struct Reads {
  uint8_t* base;
  const tuning_screen::GuestCall& call;
  uint32_t entry;
  uint32_t frame;
  bool native_slots;
  uint64_t r3;
  // Slot `offset` on `action`: whether its low byte is set.
  bool operator()(uint32_t offset, uint64_t action, uint32_t lr) {
    if (int32_t(schema::PlayerMapperEntry(entry).unread()) == 1) {
      return false;
    }
    r3 = ReadSlot(call, Load(entry), offset, action, frame, lr, native_slots);
    return (r3 & 0xFF) != 0;
  }
};

// The history popped once: byte +10008 - 1, +9652 = the word there. The
// popped state.
uint32_t Pop(uint32_t s) {
  const uint32_t depth = (uint32_t(Screen(s).history_depth()) + 255) & 0xFF;
  Screen(s).set_history_depth(uint8_t(depth));
  const uint32_t popped = Load(((depth + 2439) << 2) + s);
  Screen(s).set_state(popped);
  return popped;
}

// Push(s): the history's word at the byte +10008 = s, the byte read again
// + 1.
void Push(uint32_t s, uint32_t state) {
  Store(((uint32_t(Screen(s).history_depth()) + 2439) << 2) + s, state);
  // The byte read again (a push at depth 63 writes over it).
  Screen(s).set_history_depth(uint8_t(uint32_t(Screen(s).history_depth()) + 1));
}

// Go's first half to state `state`: q+4 = q+0 = q+8 = s, q+20 = q+12 = -1,
// q+24 = 0 (q = [+10036]).
void GoFirstHalf(uint32_t s, uint32_t state) {
  const uint32_t queue = Screen(s).queue().address;
  CommandQueue(queue).set_step(kNone);
  CommandQueue(queue).set_current(kNone);
  CommandQueue(queue).set_started(0);
  CommandQueue(queue).set_resume(state);
  CommandQueue(queue).set_page(state);
  CommandQueue(queue).set_previous_page(state);
}

// Go's second half with command `command`: the queue reloaded, q+8 = q+0
// when +9652 isn't q+0, q+16 = -1, q+4 = +9652, q+0 = -1, q+12 = command.
void GoSecondHalf(uint32_t s, uint32_t command) {
  const uint32_t queue = Screen(s).queue().address;
  const uint32_t state = Screen(s).state();
  const uint32_t current = CommandQueue(queue).page();
  if (int32_t(state) != int32_t(current)) {
    CommandQueue(queue).set_previous_page(current);
  }
  CommandQueue(queue).set_next(kNone);
  CommandQueue(queue).set_resume(state);
  CommandQueue(queue).set_page(kNone);
  CommandQueue(queue).set_current(command);
}

// Go(state, command): +9652 = state, both halves.
void Go(uint32_t s, uint32_t state, uint32_t command) {
  Screen(s).set_state(state);
  GoFirstHalf(s, state);
  GoSecondHalf(s, command);
}

// memcpy(destination, source, n) from `frame` returning to `lr`: its plain
// C++ where Buffers_Memcpy takes it, else the guest's. Its r3.
uint64_t Memcpy(const tuning_screen::GuestCall& call, uint64_t destination,
                uint64_t source, uint32_t n, uint32_t frame, uint32_t lr) {
  if (native::Buffers_Memcpy(uint32_t(destination), uint32_t(source), n, lr)) {
    return destination;
  }
  return call(kMemcpy, {destination, source, n}, frame, lr);
}

// A bar's position: the word sign-extended as a doubleword at frame + 80,
// converted (fcfid), rounded to a single, times 40.0f rounded to a single;
// its bits.
uint32_t BarPosition(uint32_t frame, uint32_t word) {
  const int64_t x = int32_t(word);
  Store(frame + 80, uint32_t(uint64_t(x) >> 32));
  Store(frame + 84, uint32_t(x));
  const double f13 = double(float(double(x)));
  const double step = double(G::bar_step());
  return std::bit_cast<uint32_t>(float(f13 * step));
}

// Entry i's texture words (+28, +32) copied to the kept texture k (images
// + 7236 + 8k), both read first.
void KeepTexture(uint32_t images, uint32_t i, uint32_t k) {
  const schema::DecalImageEntry entry = DecalEntryAt(images, i);
  const schema::DecalImageKept kept = DecalKeptAt(images, k);
  const uint32_t header = entry.texture_header();
  const uint32_t data = entry.texture_data();
  kept.set_texture_header(header);
  kept.set_texture_data(data);
}

// The stand holder [[0x826D83E0]] (tuning-screen.toml's StandHolder's +0).
uint32_t Holder() {
  return schema::TuningScreenGlobals::stand_holder().stage_block_0();
}

}  // namespace

uint64_t FitSelectedCar(const tuning_screen::GuestCall& call, const StandFit& fit,
                        uint32_t holder, uint64_t r4, uint64_t r5, uint32_t frame, uint32_t lr) {
  if (fit.calls == nullptr) {
    return call(kFitSelectedCar, {holder, r4, r5}, frame, lr);
  }
  double f1 = 0.0;
  const uint64_t r3 = tuning_screen::FitSelectedCar(*fit.calls, holder, r4, r5, frame, &f1);
  *fit.f1 = f1;
  return r3;
}

uint64_t LeaveVinylView(const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots, const StandFit& fit) {
  ITC_NATIVE_RUNNING("sub_82201B40 TuningScreen::LeaveVinylView");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = PlayerEntry();
  const uint32_t found = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(found);
  Reads asked{call, entry, frame, native_slots, found};
  if (!asked(40, 159, 0x82201BA8) && !asked(40, 158, 0x82201BD4)) {
    return asked.r3;
  }
  Screen(s).set_stand_action(8);
  FitSelectedCar(call, fit, Holder(), 0, 0, frame, 0x82201BFC);
  tuning_screen::FinishNamed(call, frame, 0x82201C08, movie, 241);
  uint64_t r3 = tuning_screen::PlayNamed(call, frame, 0x82201C24, movie, 241, 1);
  const int32_t area = int32_t(Screen(s).editor_page());
  if (area == 0) {
    r3 = call(kStageCommand, {Holder(), 40}, frame, 0x82201C50);
  } else if (area == 1) {
    r3 = call(kStageCommand, {Holder(), 41}, frame, 0x82201C50);
  }
  return r3;
}

uint64_t InputVinylView(const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots, const StandFit& fit) {
  ITC_NATIVE_RUNNING("sub_82202928 TuningScreen::InputVinylView");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 112);
  const uint32_t entry = PlayerEntry();
  Reads asked{call, entry, frame, native_slots, 0};
  if (asked(40, 159, 0x82202978) || asked(40, 158, 0x822029A4)) {
    // A or B: Pop, Go(the popped state, 18), the turns cancelled, the camera
    // reset.
    call(kMenuSoundPlay, {2}, frame, 0x822029B4);
    Screen(s).set_refresh_pending(1);
    const uint32_t popped = Pop(s);
    GoFirstHalf(s, popped);
    GoSecondHalf(s, 18);
    call(kStageCommand, {Holder(), 1}, frame, 0x82202A40);
    call(kCameraCommand, {Holder(), 20}, frame, 0x82202A50);
  }
  // The held buttons: the distance, the turns and the tilt.
  if (asked(32, 163, 0x82202A74)) {
    call(kCameraCommand, {Holder(), 4}, frame, 0x82202A8C);
  }
  if (asked(32, 164, 0x82202AB0)) {
    call(kCameraCommand, {Holder(), 5}, frame, 0x82202AC8);
  }
  if (asked(32, 173, 0x82202AEC)) {
    call(kStageCommand, {Holder(), 1}, frame, 0x82202B04);
    call(kStageCommand, {Holder(), 9}, frame, 0x82202B14);
  }
  if (asked(32, 174, 0x82202B38)) {
    call(kStageCommand, {Holder(), 1}, frame, 0x82202B50);
    call(kStageCommand, {Holder(), 10}, frame, 0x82202B60);
  }
  if (asked(32, 171, 0x82202B84)) {
    call(kStageCommand, {Holder(), 1}, frame, 0x82202B9C);
    call(kCameraCommand, {Holder(), 0}, frame, 0x82202BAC);
  }
  if (asked(32, 172, 0x82202BD0)) {
    call(kStageCommand, {Holder(), 1}, frame, 0x82202BE8);
    call(kCameraCommand, {Holder(), 1}, frame, 0x82202BF8);
  }
  return LeaveVinylView(call, screen, frame, native_slots, fit);
}

void SetLastImage(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821FE498 TuningScreen::SetLastImage");
  uint32_t last;
  if (int32_t(Screen(screen).category()) == 5 && uint32_t(Screen(screen).item()) == 6) {
    const int32_t page = int32_t(Screen(screen).editor_page());
    if (page == 0) {
      last = 4;
    } else if (page == 1) {
      last = 193;
    } else {
      return;
    }
  } else {
    last = 198;
  }
  Screen(screen).set_image_last_index(last);
}

uint64_t MoveVinylMenu(const tuning_screen::GuestCall& call, uint64_t screen,
                       uint32_t sp, bool native_slots, const StandFit& fit) {
  ITC_NATIVE_RUNNING("sub_821FF3A0 TuningScreen::MoveVinylMenu");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = PlayerEntry();
  const uint32_t found = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(found);
  Reads asked{call, entry, frame, native_slots, found};
  auto page = [&] { return uint64_t(Screen(s).editor_page()); };
  auto finish = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::FinishNamed(call, frame, lr, movie, named);
  };
  auto play = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::PlayNamed(call, frame, lr, movie, named, 1);
  };
  // The bar snapped to the page: node (3, 0)'s +16.
  auto snap = [&] {
    const uint32_t node = ui_movies::FindNode(movie, 3, 0);
    schema::UiMovieNode(node).set_y(std::bit_cast<float>(uint32_t(BarPosition(frame, Screen(s).editor_page()))));
  };
  uint64_t old;
  if (asked(48, 153, 0x821FF408) || asked(48, 167, 0x821FF434)) {
    // Up.
    if (Screen(s).refresh_pending() == 0) {
      return asked.r3;
    }
    if (Screen(s).editor_page_wrapped() != 0) {
      finish(16, 0x821FF62C);
      finish(17, 0x821FF638);
      snap();
      old = 6;
    } else {
      finish(16, 0x821FF67C);
      play(16, 0x821FF698);
      old = page() + 7;
    }
  } else if (asked(48, 154, 0x821FF460) || asked(48, 168, 0x821FF48C)) {
    // Down.
    if (Screen(s).refresh_pending() == 0) {
      return asked.r3;
    }
    if (Screen(s).editor_page_wrapped() != 0) {
      finish(16, 0x821FF588);
      finish(17, 0x821FF594);
      snap();
      old = uint64_t(Screen(s).last_page()) + 6;
    } else {
      finish(17, 0x821FF5E0);
      play(17, 0x821FF5FC);
      old = page() + 5;
    }
  } else if (asked(40, 159, 0x821FF4B8)) {
    // A: the bars and the page's highlight finished, the decals applied on
    // the stand.
    finish(16, 0x821FF4CC);
    finish(17, 0x821FF4D8);
    finish(page() + 6, 0x821FF4E8);
    Screen(s).set_stand_action(5);
    return FitSelectedCar(call, fit, Holder(), 0, 0, frame, 0x821FF508);
  } else if (asked(40, 158, 0x821FF530)) {
    // B: the same, nothing applied.
    finish(16, 0x821FF544);
    finish(17, 0x821FF550);
    return finish(page() + 6, 0x821FF560);
  } else {
    return asked.r3;
  }
  // The old page finished, the new one restarted and looped.
  finish(old, 0x821FF6A8);
  finish(page() + 6, 0x821FF6B8);
  play(page() + 6, 0x821FF6D8);
  return tuning_screen::LoopNamed(call, frame, 0x821FF6E8, movie, page() + 6);
}

uint64_t InputVinylPages(const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_82204B70 TuningScreen::InputVinylPages");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = PlayerEntry();
  const uint64_t images = screen + 10048;
  const uint32_t im = uint32_t(images);
  const bool native_free = flags.native_heap && flags.native_free;
  // The pages' kept textures: page 0's six (kept 0 to 5 = entries 0, 1, 2,
  // 6, 7, 8), page 1's (kept 15 to 20 = entries 21, 22, 23, 27, 28, 29).
  static constexpr uint32_t kPage0[6] = {0, 1, 2, 6, 7, 8};
  static constexpr uint32_t kPage1[6] = {21, 22, 23, 27, 28, 29};
  if (Screen(s).vinyl_kept() != 0) {
    // A page kept: its textures freed and kept again on the third frame.
    const uint32_t count = G::keep_counter() + 1;
    G::set_keep_counter(count);
    if (int32_t(count) == 3) {
      const int32_t page = int32_t(Screen(s).editor_page());
      if (page == 0 || page == 1) {
        const uint32_t first = page == 0 ? 0 : 15;
        const uint32_t lrs0[6] = {0x82204BDC, 0x82204BE8, 0x82204BF4,
                                  0x82204C00, 0x82204C0C, 0x82204C18};
        const uint32_t lrs1[6] = {0x82204C94, 0x82204CA0, 0x82204CAC,
                                  0x82204CB8, 0x82204CC4, 0x82204CD0};
        for (uint32_t k = 0; k < 6; k++) {
          FreeKept(images, first + k,
                   GuestTextureFree(call, frame, page == 0 ? lrs0[k] : lrs1[k],
                                    native_free));
        }
        for (uint32_t k = 0; k < 6; k++) {
          KeepTexture(im, page == 0 ? kPage0[k] : kPage1[k], first + k);
        }
      }
      Screen(s).set_vinyl_kept(0);
      G::set_keep_counter(0);
    }
  }
  if (Screen(s).vinyl_kept() == 0) {
    Reads asked{call, entry, frame, flags.native_slots, 0};
    if (asked(48, 153, 0x82204D6C) || asked(48, 167, 0x82204D98)) {
      // Up, wrapping to the last page +30784.
      call(kMenuSoundPlay, {4}, frame, 0x822050D4);
      const int32_t page = int32_t(Screen(s).editor_page() - 1);
      Screen(s).set_refresh_pending(1);
      Screen(s).set_editor_page(uint32_t(page));
      if (page < 0) {
        Screen(s).set_editor_page(Screen(s).last_page());
        Screen(s).set_editor_page_wrapped(1);
      } else {
        Screen(s).set_editor_page_wrapped(0);
      }
    } else if (asked(48, 154, 0x82204DC4) || asked(48, 168, 0x82204DF0)) {
      // Down, wrapping to 0 past +30784.
      call(kMenuSoundPlay, {4}, frame, 0x822050A4);
      const int32_t page = int32_t(Screen(s).editor_page() + 1);
      const int32_t last = int32_t(Screen(s).last_page());
      Screen(s).set_refresh_pending(1);
      Screen(s).set_editor_page(uint32_t(page));
      if (last < page) {
        Screen(s).set_editor_page(0);
        Screen(s).set_editor_page_wrapped(1);
      } else {
        Screen(s).set_editor_page_wrapped(0);
      }
    } else if (asked(40, 159, 0x82204E1C)) {
      // A: the page's layers 0 and 1 (not 2: the loop stops at 2) copied to
      // their saved ids and records, then the layers' page.
      call(kMenuSoundPlay, {0}, frame, 0x82204E2C);
      SetLastImage(s);
      for (uint32_t j = 0; j < 2; j++) {
        const uint32_t n = Screen(s).editor_page() * 3 + j;
        Store(((n + 7798) << 2) + s, Load(((n + 7792) << 2) + s));
        const uint64_t m = uint64_t(Screen(s).editor_page()) * 3 + j;
        const uint64_t at = m * 24 + screen;
        Memcpy(call, at + 31024, at + 30880, 24, frame, 0x82204E80);
      }
      Screen(s).set_layer_cursor(0);
      Screen(s).set_refresh_pending(1);
      Screen(s).set_command_first_run(1);
      Screen(s).set_layer_button(3);
      Push(s, 23);
      Screen(s).set_state(24);
      GoFirstHalf(s, 24);
      // The page's kept textures set again (no FreeKept this time).
      const int32_t page = int32_t(Screen(s).editor_page());
      if (page == 0 || page == 1) {
        for (uint32_t k = 0; k < 6; k++) {
          KeepTexture(im, page == 0 ? kPage0[k] : kPage1[k], (page == 0 ? 0 : 15) + k);
        }
      }
      GoSecondHalf(s, 16);
    } else if (asked(40, 158, 0x82205018)) {
      // B: state 61 (leaving the editor) with no command, the vinyls stored,
      // the fade out.
      call(kMenuSoundPlay, {2}, frame, 0x82205028);
      Screen(s).set_state(61);
      GoFirstHalf(s, 61);
      const uint32_t queue = Screen(s).queue().address;
      const uint32_t state = Screen(s).state();
      const uint32_t current = CommandQueue(queue).page();
      if (int32_t(state) != int32_t(current)) {
        CommandQueue(queue).set_previous_page(current);
      }
      CommandQueue(queue).set_page(state);
      CommandQueue(queue).set_current(kNone);
      CommandQueue(queue).set_next(kNone);
      StoreVinyls(screen, frame);
      mode_tick::ScreenFadesFadeOut(G::fade_out_passes(), 0, 0, 0, 0);
    }
  }
  return MoveVinylMenu(call, screen, frame, flags.native_slots, flags.fit);
}

namespace {

namespace fs = strings_and_font::steps;

constexpr uint32_t kLimits = 0x8200C840;  // the decal's position limits, 16 bytes a page

// The decal edits: state 29 (position), 30 (angle), 31 (scale).
enum class Edit { kPosition, kAngle, kScale };

// The return addresses of an edit page's calls, in the order the original
// makes them.
struct EditInputCalls {
  uint32_t held, released, highlight;
  uint32_t moves[8];
  uint32_t a, a_held, a_released, a_sound;
  uint32_t b, b_held, b_released, b_sound, b_memcpy;
  uint32_t move;
};
// And of its movie side's.
struct EditMoveCalls {
  uint32_t moves[8];
  uint32_t a, b, held, released, b_again, finish, play;
};

constexpr EditInputCalls kPositionInput = {
    0x82206AA4, 0x82206AD8, 0x82206AEC,
    {0x82206B10, 0x82206B3C, 0x82206B68, 0x82206B94, 0x82206BC0, 0x82206BEC, 0x82206C18,
     0x82206C44},
    0x82206C70, 0x82206C9C, 0x82206CC8, 0x82206CD8,
    0x82206D50, 0x82206D7C, 0x82206DA8, 0x82206DB8, 0x82206E18,
    0x82206F30};
constexpr EditInputCalls kAngleInput = {
    0x82206F9C, 0x82206FD0, 0x82206FE4,
    {0x82207008, 0x82207034, 0x82207060, 0x8220708C},
    0x822070B8, 0x822070E4, 0x82207110, 0x82207120,
    0x82207198, 0x822071C4, 0x822071F0, 0x82207200, 0x82207260,
    0x82207308};
constexpr EditInputCalls kScaleInput = {
    0x82207374, 0x822073A8, 0x822073BC,
    {0x822073E0, 0x8220740C, 0x82207438, 0x82207464},
    0x82207490, 0x822074BC, 0x822074E8, 0x822074F8,
    0x82207570, 0x8220759C, 0x822075C8, 0x822075D8, 0x82207638,
    0x822076D0};
constexpr EditMoveCalls kPositionMove = {
    {0x82200E28, 0x82200E54, 0x82200E80, 0x82200EAC, 0x82200ED8, 0x82200F04, 0x82200F30,
     0x82200F5C},
    0x82200F88, 0x82200FB4, 0x82200FE0, 0x8220100C, 0x82201038, 0x82201184, 0x822011A0};
constexpr EditMoveCalls kAngleMove = {
    {0x82201350, 0x8220137C, 0x822013A8, 0x822013D4},
    0x82201400, 0x8220142C, 0x82201458, 0x82201484, 0x822014B0, 0x8220155C, 0x82201578};
constexpr EditMoveCalls kScaleMove = {
    {0x82201688, 0x822016B4, 0x822016E0, 0x8220170C},
    0x82201738, 0x82201764, 0x82201790, 0x822017BC, 0x822017E8, 0x822018D0, 0x822018EC};

// The edited decal's node: movie 13's (-1, 19 + layer) on page 0, (-1, 22 +
// layer) on page 1; 0 on other pages.
uint32_t DecalNode(uint32_t movie, uint32_t s, int32_t page) {
  const uint32_t layer = Screen(s).layer_cursor();
  return ui_movies::FindNode(movie, 0xFFFFFFFFu, layer + (page == 0 ? 19 : 22));
}

// A doubleword at `at` (an int sign-extended through the stack for fcfid).
void StoreDoubleword(uint32_t at, int64_t value) {
  Store(at, uint32_t(uint64_t(value) >> 32));
  Store(at + 4, uint32_t(value));
}

// Place: the node's +12 and +16 from the decal's x and y (+30832, +30836)
// times the page's scale plus its origin (single-precision multiply-adds)
// plus the page's offsets; the conversions' doublewords at frame + 80
// (the y origin) and + 88 (y) as they're left. The node, or nullopt on
// other pages.
std::optional<uint32_t> Place(uint32_t movie, uint32_t s, uint32_t frame) {
  const int32_t page = int32_t(Screen(s).editor_page());
  if (page != 0 && page != 1) {
    return std::nullopt;
  }
  const uint32_t node = DecalNode(movie, s, page);
  const uint32_t scale_at = page == 0 ? 30812 : 30828;
  const uint32_t origin_at = page == 0 ? 30800 : 30816;
  const uint32_t x_offset = page == 0 ? 0x8200CAF4 : 0x8200CAEC;
  const uint32_t y_offset = page == 0 ? 0x8200CAF0 : 0x8200CAE8;
  auto coordinate = [&](uint32_t at, uint32_t origin, uint32_t offset) {
    const double scale = fs::Lfs(s + scale_at);
    const int64_t v = int32_t(Load(s + at));
    const int64_t o = int32_t(Load(s + origin));
    const double f13 = fs::ToSingle(v), f12 = fs::ToSingle(o);
    const double sum = fs::Single(std::fma(f13, scale, f12));
    StoreDoubleword(frame + 80, o);
    StoreDoubleword(frame + 88, v);
    return fs::Single(sum + fs::Lfs(offset));
  };
  schema::UiMovieNode(node).set_x(float(coordinate(30832, origin_at, x_offset)));
  schema::UiMovieNode(node).set_y(float(coordinate(30836, origin_at + 4, y_offset)));
  return node;
}

// The node's pivot: +20 and +24 the first lookup's +100 and +104 times 0.5
// (fctiwz) into the second's.
void Pivot(uint32_t first, uint32_t second) {
  const double half = fs::Lfs(0x82001BE4);
  schema::UiMovieNode(second).set_pivot_x(uint32_t(fs::Fctiwz(fs::Single(double(schema::UiMovieNode(first).width()) * half))));
  schema::UiMovieNode(second).set_pivot_y(uint32_t(fs::Fctiwz(fs::Single(double(schema::UiMovieNode(first).height()) * half))));
}

// Turn: the node (looked up twice) pivoted, its +36 the angle +30840.
std::optional<uint32_t> Turn(uint32_t movie, uint32_t s) {
  const int32_t page = int32_t(Screen(s).editor_page());
  if (page != 0 && page != 1) {
    return std::nullopt;
  }
  const uint32_t first = DecalNode(movie, s, page);
  const uint32_t second = DecalNode(movie, s, page);
  Pivot(first, second);
  schema::UiMovieNode(second).set_rotation(float(double(Screen(s).edited_angle())));
  return second;
}

// Size: the node (looked up twice): +28 and +32 the scale +30844 times the
// page's (+30808 or +30824), then pivoted.
std::optional<uint32_t> Size(uint32_t movie, uint32_t s) {
  const int32_t page = int32_t(Screen(s).editor_page());
  if (page != 0 && page != 1) {
    return std::nullopt;
  }
  const uint32_t first = DecalNode(movie, s, page);
  const uint32_t second = DecalNode(movie, s, page);
  const uint32_t factor = s + (page == 0 ? 30808 : 30824);
  schema::UiMovieNode(second).set_scale_x(float(fs::Single(double(Screen(s).edited_scale()) * fs::Lfs(factor))));
  schema::UiMovieNode(second).set_scale_y(float(fs::Single(double(Screen(s).edited_scale()) * fs::Lfs(factor))));
  Pivot(first, second);
  return second;
}

uint64_t MoveDecal(const tuning_screen::GuestCall& call, uint64_t screen,
                   uint32_t sp, bool native_slots, Edit edit) {
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = PlayerEntry();
  const uint32_t found = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(found);
  Reads asked{call, entry, frame, native_slots, found};
  const EditMoveCalls& lr = edit == Edit::kPosition ? kPositionMove
                            : edit == Edit::kAngle  ? kAngleMove
                                                    : kScaleMove;
  auto apply = [&]() -> std::optional<uint32_t> {
    switch (edit) {
      case Edit::kPosition: return Place(movie, s, frame);
      case Edit::kAngle: return Turn(movie, s);
      default: return Size(movie, s);
    }
  };
  static constexpr uint32_t kPositionMoves[8] = {153, 167, 154, 168, 156, 170, 155, 169};
  static constexpr uint32_t kTurnMoves[4] = {156, 170, 155, 169};
  const uint32_t* moves = edit == Edit::kPosition ? kPositionMoves : kTurnMoves;
  const uint32_t count = edit == Edit::kPosition ? 8 : 4;
  for (uint32_t k = 0; k < count; k++) {
    if (asked(48, moves[k], lr.moves[k])) {
      const std::optional<uint32_t> node = apply();
      return node ? *node : asked.r3;
    }
  }
  if (!asked(40, 159, lr.a) && !asked(40, 158, lr.b)) {
    return asked.r3;
  }
  if (asked(32, 164, lr.held) || asked(44, 164, lr.released)) {
    return asked.r3;
  }
  if (asked(40, 158, lr.b_again)) {
    apply();
  }
  tuning_screen::FinishNamed(call, frame, lr.finish, movie, 228);
  return tuning_screen::PlayNamed(call, frame, lr.play, movie, 228, 1);
}

// An edit page's move: the field at `at` stepped and clamped (the
// position's ints) or the float stepped by [step] and wrapped or clamped
// (the angle's and scale's); +9708 = 1 first.
void Step(uint32_t s, Edit edit, uint32_t move) {
  Screen(s).set_refresh_pending(1);
  if (edit == Edit::kPosition) {
    // 0 up (y - 1, y min), 1 down (y + 1, y max), 2 left (x - 1, x min), 3
    // right (x + 1, x max).
    const uint32_t at = move < 2 ? 30836 : 30832;
    const int32_t delta = (move % 2 == 0) ? -1 : 1;
    const uint32_t limits = (Screen(s).editor_page() << 4) + kLimits;
    const uint32_t limit_at = limits + (move < 2 ? 8 : 0) + (move % 2 == 0 ? 0 : 4);
    const int32_t value = int32_t(Load(s + at) + uint32_t(delta));
    Store(s + at, uint32_t(value));
    const int32_t limit = int32_t(Load(limit_at));
    if (delta < 0 ? value < limit : value > limit) {
      Store(s + at, uint32_t(limit));
    }
    return;
  }
  // 0 left (minus), 1 right (plus).
  const bool plus = move == 1;
  const uint32_t at = s + (edit == Edit::kAngle ? 30840 : 30844);
  const uint32_t step = edit == Edit::kAngle ? 0x82000A30 : 0x8200222C;
  const double old = fs::Lfs(at);
  const double value = fs::Single(plus ? old + fs::Lfs(step) : old - fs::Lfs(step));
  fs::Stfs(at, value);
  if (edit == Edit::kAngle) {
    // 0.0 to 359.0, wrapped.
    if (plus ? value > fs::Lfs(0x8200C54C) : value < fs::Lfs(0x82000A34)) {
      fs::Stfs(at, fs::Lfs(plus ? 0x82000A34 : 0x8200C54C));
    }
  } else {
    // 0.1 to 2.0, clamped.
    const double limit = fs::Lfs(plus ? 0x820021E8 : 0x82001FBC);
    if (plus ? value > limit : value < limit) {
      fs::Stfs(at, limit);
    }
  }
}

uint64_t InputDecal(const tuning_screen::GuestCall& call, uint64_t screen,
                    uint32_t sp, const PageFlags& flags, Edit edit) {
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 112);
  const uint32_t entry = PlayerEntry();
  Reads asked{call, entry, frame, flags.native_slots, 0};
  const EditInputCalls& lr = edit == Edit::kPosition ? kPositionInput
                             : edit == Edit::kAngle  ? kAngleInput
                                                     : kScaleInput;
  // 164 held: the selected decal highlighted (0), released: back (1).
  if (asked(32, 164, lr.held)) {
    tuning_screen::HighlightSelected(screen, 0, frame);
  } else if (asked(44, 164, lr.released)) {
    tuning_screen::HighlightSelected(screen, 1, frame);
  }
  // The moves: the position's up, down, left, right; the angle's and
  // scale's left, right.
  static constexpr uint32_t kPositionMoves[8] = {153, 167, 154, 168, 155, 169, 156, 170};
  static constexpr uint32_t kTurnMoves[4] = {155, 169, 156, 170};
  const uint32_t* moves = edit == Edit::kPosition ? kPositionMoves : kTurnMoves;
  const uint32_t count = edit == Edit::kPosition ? 8 : 4;
  bool moved = false;
  for (uint32_t k = 0; k < count && !moved; k++) {
    if (asked(48, moves[k], lr.moves[k])) {
      Step(s, edit, k / 2);
      moved = true;
    }
  }
  if (!moved) {
    // A or B, with 164 neither held nor released: Pop, Go(the popped
    // state, 18); B first puts the decal back as it was (+30856).
    bool a = false, b = false;
    if (asked(40, 159, lr.a) && !asked(32, 164, lr.a_held) &&
        !asked(44, 164, lr.a_released)) {
      a = true;
    } else if (asked(40, 158, lr.b) && !asked(32, 164, lr.b_held) &&
               !asked(44, 164, lr.b_released)) {
      b = true;
    }
    if (a || b) {
      call(kMenuSoundPlay, {a ? 0u : 2u}, frame, a ? lr.a_sound : lr.b_sound);
      Screen(s).set_refresh_pending(1);
      const uint32_t popped = Pop(s);
      GoFirstHalf(s, popped);
      if (b) {
        Memcpy(call, screen + 30832, screen + 30856, 24, frame, lr.b_memcpy);
      }
      GoSecondHalf(s, 18);
    }
  }
  return MoveDecal(call, screen, frame, flags.native_slots, edit);
}

}  // namespace

uint64_t MoveDecalPosition(const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_82200DC0 TuningScreen::MoveDecalPosition");
  return MoveDecal(call, screen, sp, native_slots, Edit::kPosition);
}
uint64_t MoveDecalAngle(const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_822012E8 TuningScreen::MoveDecalAngle");
  return MoveDecal(call, screen, sp, native_slots, Edit::kAngle);
}
uint64_t MoveDecalScale(const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_82201620 TuningScreen::MoveDecalScale");
  return MoveDecal(call, screen, sp, native_slots, Edit::kScale);
}
uint64_t InputDecalPosition(const tuning_screen::GuestCall& call, uint64_t screen,
                            uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_82206A50 TuningScreen::InputDecalPosition");
  return InputDecal(call, screen, sp, flags, Edit::kPosition);
}
uint64_t InputDecalAngle(const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_82206F48 TuningScreen::InputDecalAngle");
  return InputDecal(call, screen, sp, flags, Edit::kAngle);
}
uint64_t InputDecalScale(const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_82207320 TuningScreen::InputDecalScale");
  return InputDecal(call, screen, sp, flags, Edit::kScale);
}

uint64_t MoveLayerEdit(const tuning_screen::GuestCall& call, uint64_t screen,
                       uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_82200760 TuningScreen::MoveLayerEdit");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = PlayerEntry();
  const uint32_t found = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(found);
  Reads asked{call, entry, frame, native_slots, found};
  uint64_t r3 = found;
  auto cursor = [&] { return Screen(s).edit_cursor(); };
  auto finish = [&](uint64_t named, uint32_t lr) {
    return r3 = tuning_screen::FinishNamed(call, frame, lr, movie, named);
  };
  auto play = [&](uint64_t named, uint32_t lr) {
    return r3 = tuning_screen::PlayNamed(call, frame, lr, movie, named, 1);
  };
  auto loop = [&](uint64_t named, uint32_t lr) {
    return r3 = tuning_screen::LoopNamed(call, frame, lr, movie, named);
  };
  // The list's scroll node: Child(FindNode(11, 1), 0)'s +16 = [constant].
  auto scroll = [&](uint32_t constant) {
    const uint32_t node = ui_movies::Child(ui_movies::FindNode(movie, 11, 1), 0);
    schema::UiMovieNode(node).set_y(std::bit_cast<float>(uint32_t(Load(constant))));
    r3 = node;
  };
  // The rows' highlights finished (130 to 134, 136, 137).
  auto finish_rows = [&](const uint32_t (&lrs)[7]) {
    static constexpr uint32_t kRows[7] = {130, 131, 132, 133, 134, 136, 137};
    for (uint32_t k = 0; k < 7; k++) {
      finish(kRows[k], lrs[k]);
    }
  };
  // The buttons' highlights: 5 136, 6 137, played and looped.
  auto button = [&](uint64_t named, uint32_t play_lr) {
    play(named, play_lr);
    loop(named, 0x82200D4C);
  };
  static constexpr uint32_t kMoves[9] = {153, 167, 154, 168, 156, 170, 155, 169, 158};
  static constexpr uint32_t kMoveLrs[9] = {0x822007C8, 0x822007F4, 0x82200820,
                                           0x8220084C, 0x82200878, 0x822008A4,
                                           0x822008D0, 0x822008FC, 0x82200928};
  bool moved = false;
  for (uint32_t k = 0; k < 9 && !moved; k++) {
    moved = asked(48, kMoves[k], kMoveLrs[k]);
  }
  r3 = asked.r3;
  if (moved) {
    if (Screen(s).refresh_pending() != 0) {
      const int32_t c = int32_t(cursor());
      if (c < 0 || c > 4) {
        finish(102, 0x82200C80);
        finish(103, 0x82200C8C);
        scroll(0x820027A4);  // 200.0
      } else {
        Reads again{call, entry, frame, native_slots, r3};
        if (again(48, 153, 0x82200B50) || again(48, 167, 0x82200B7C)) {
          if (int32_t(cursor()) == 4) {
            scroll(0x820081D8);  // 160.0
          } else {
            finish(102, 0x82200C54);
            play(102, 0x82200C70);
          }
        } else if (again(48, 154, 0x82200BA8) || again(48, 168, 0x82200BD4)) {
          if (int32_t(cursor()) == 0) {
            scroll(0x82000A34);  // 0.0
          } else {
            finish(103, 0x82200C14);
            play(103, 0x82200C70);
          }
        } else {
          r3 = again.r3;
        }
      }
      finish_rows({0x82200CBC, 0x82200CC8, 0x82200CD4, 0x82200CE0, 0x82200CEC, 0x82200CF8,
                   0x82200D04});
      const uint32_t c2 = cursor();
      if (c2 < 5) {
        play(uint64_t(c2) + 130, 0x82200D3C);
        loop(uint64_t(cursor()) + 130, 0x82200D4C);
      } else if (c2 == 5) {
        button(136, 0x82200B04);
      } else if (c2 == 6) {
        button(137, 0x82200AE0);
      }
    }
  } else if (asked(32, 164, 0x82200954) || asked(44, 164, 0x82200980)) {
    r3 = asked.r3;
  } else if (asked(40, 159, 0x822009AC)) {
    r3 = asked.r3;
    const int32_t c = int32_t(cursor());
    if (c == 5) {
      finish(235, 0x822009CC);
      play(235, 0x82200A04);
    } else if (c == 6) {
      finish(226, 0x822009E8);
      play(226, 0x82200A04);
    }
  } else if (asked(40, 158, 0x82200A2C)) {
    scroll(0x820027A4);  // 200.0
    finish_rows({0x82200A68, 0x82200A74, 0x82200A80, 0x82200A8C, 0x82200A98, 0x82200AA4,
                 0x82200AB0});
    const int32_t c = int32_t(cursor());
    if (c == 5) {
      button(136, 0x82200B04);
    } else if (c == 6) {
      button(137, 0x82200AE0);
    }
  } else {
    r3 = asked.r3;
  }
  // The flip shown on the edited decal's node: +132 = (+30848 != 0).
  const int32_t page = int32_t(Screen(s).editor_page());
  if (page == 0 || page == 1) {
    const uint32_t node = DecalNode(movie, s, page);
    schema::UiMovieNode(node).set_flip_x(Screen(s).edited_flip() != 0 ? 1 : 0);
    r3 = node;
  }
  return r3;
}

uint64_t InputLayerEdit(const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_82205110 TuningScreen::InputLayerEdit");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 112);
  const uint32_t entry = PlayerEntry();
  Reads asked{call, entry, frame, flags.native_slots, 0};
  auto cursor = [&] { return int32_t(Screen(s).edit_cursor()); };
  auto sound = [&](uint32_t cue, uint32_t lr) { call(kMenuSoundPlay, {cue}, frame, lr); };
  if (asked(32, 164, 0x82205164)) {
    tuning_screen::HighlightSelected(screen, 0, frame);
  } else if (asked(44, 164, 0x82205198)) {
    tuning_screen::HighlightSelected(screen, 1, frame);
  }
  if (asked(48, 153, 0x822051D0) || asked(48, 167, 0x822051FC)) {
    // Up: from the buttons to the free look (4); else the row before,
    // below 0 to the last (+30752).
    sound(4, 0x822058A0);
    Screen(s).set_refresh_pending(1);
    const int32_t c = cursor();
    if (c == 5 || c == 6) {
      Screen(s).set_edit_cursor(4);
    } else {
      Screen(s).set_edit_cursor(uint32_t(c - 1));
      if (c - 1 < 0) {
        Screen(s).set_edit_cursor(Screen(s).edit_row_saved());
      }
    }
  } else if (asked(48, 154, 0x82205228) || asked(48, 168, 0x82205254)) {
    // Down: from the buttons to 0; else the next row, above 4 to +30752.
    sound(4, 0x8220585C);
    Screen(s).set_refresh_pending(1);
    const int32_t c = cursor();
    if (c == 5 || c == 6) {
      Screen(s).set_edit_cursor(0);
    } else {
      Screen(s).set_edit_cursor(uint32_t(c + 1));
      if (c + 1 > 4) {
        Screen(s).set_edit_cursor(Screen(s).edit_row_saved());
      }
    }
  } else if (asked(48, 155, 0x82205280) || asked(48, 169, 0x822052AC)) {
    // Left, between the buttons only (below 5 to 6); +30752 the button.
    const int32_t c = cursor();
    if (c == 5 || c == 6) {
      sound(4, 0x82205830);
      Screen(s).set_refresh_pending(1);
      Screen(s).set_edit_cursor(uint32_t(c - 1));
      if (c - 1 < 5) {
        Screen(s).set_edit_cursor(6);
      }
      Screen(s).set_edit_row_saved(Screen(s).edit_cursor());
    }
  } else if (asked(48, 156, 0x822052D8) || asked(48, 170, 0x82205304)) {
    // Right, between the buttons only (above 6 to 5).
    const int32_t c = cursor();
    if (c == 5 || c == 6) {
      sound(4, 0x822057E4);
      Screen(s).set_refresh_pending(1);
      Screen(s).set_edit_cursor(uint32_t(c + 1));
      if (c + 1 > 6) {
        Screen(s).set_edit_cursor(5);
      }
      Screen(s).set_edit_row_saved(Screen(s).edit_cursor());
    }
  } else if (asked(32, 164, 0x82205330) || asked(44, 164, 0x8220535C)) {
    // 164 held or released: nothing.
  } else if (asked(40, 159, 0x82205388)) {
    sound(0, 0x82205398);
    Screen(s).set_refresh_pending(1);
    const uint32_t c = Screen(s).edit_cursor();
    if (c <= 2 || c == 4) {
      // The edits (Go(29, 25), (30, 26), (31, 27), the edited decal kept at
      // +30856 for B) and the free look (Go(34, 29)).
      static constexpr uint32_t kStates[5] = {29, 30, 31, 0, 34};
      static constexpr uint32_t kCommands[5] = {25, 26, 27, 0, 29};
      static constexpr uint32_t kMemcpyLrs[3] = {0x82205704, 0x8220567C, 0x822055FC};
      Screen(s).set_command_first_run(1);
      if (c <= 2) {
        Memcpy(call, screen + 30856, screen + 30832, 24, frame, kMemcpyLrs[c]);
      }
      Push(s, 26);
      Go(s, kStates[c], kCommands[c]);
    } else if (c == 3) {
      // The flip.
      Screen(s).set_edited_flip(Screen(s).edited_flip() == 0 ? 1 : 0);
    } else if (c == 5) {
      // OK: back to the layers (Pop until 24, Go(24, 16)), the edit kept in
      // the layer's saved record.
      uint32_t state;
      do {
        state = Pop(s);
      } while (int32_t(state) != 24);
      GoFirstHalf(s, state);
      GoSecondHalf(s, 16);
      const uint64_t n = uint64_t(Screen(s).editor_page()) * 3 + Screen(s).layer_cursor();
      Memcpy(call, n * 24 + screen + 31024, screen + 30832, 24, frame, 0x82205560);
    } else if (c == 6) {
      // Cancel: Pop; from the image picker (28) back to the layer's actions
      // (25) with 24 left pushed.
      if (int32_t(Pop(s)) == 28) {
        uint32_t state;
        do {
          state = Pop(s);
        } while (int32_t(state) != 24);
        Screen(s).set_history_depth(uint8_t(uint32_t(Screen(s).history_depth()) + 1));
        Screen(s).set_state(25);
      }
      const uint32_t state = Screen(s).state();
      GoFirstHalf(s, state);
      const bool vinyls = int32_t(Screen(s).category()) == 5 && uint32_t(Screen(s).item()) == 6;
      GoSecondHalf(s, vinyls ? 17 : 36);
    }
  } else if (asked(40, 158, 0x822057A4)) {
    sound(2, 0x822057B4);
    Screen(s).set_refresh_pending(1);
    Screen(s).set_edit_cursor(6);
  }
  return MoveLayerEdit(call, screen, frame, flags.native_slots);
}

uint64_t MoveVinylLayers(const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821FFB20 TuningScreen::MoveVinylLayers");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = PlayerEntry();
  const uint32_t found = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(found);
  Reads asked{call, entry, frame, native_slots, found};
  auto finish = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::FinishNamed(call, frame, lr, movie, named);
  };
  auto play = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::PlayNamed(call, frame, lr, movie, named, 1);
  };
  auto loop = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::LoopNamed(call, frame, lr, movie, named);
  };
  static constexpr uint32_t kMoves[9] = {153, 167, 154, 168, 156, 170, 155, 169, 158};
  static constexpr uint32_t kMoveLrs[9] = {0x821FFB88, 0x821FFBB4, 0x821FFBE0,
                                           0x821FFC0C, 0x821FFC38, 0x821FFC64,
                                           0x821FFC90, 0x821FFCBC, 0x821FFCE8};
  bool moved = false;
  for (uint32_t k = 0; k < 9 && !moved; k++) {
    moved = asked(48, kMoves[k], kMoveLrs[k]);
  }
  if (moved) {
    if (Screen(s).refresh_pending() == 0) {
      return asked.r3;
    }
    const uint32_t c = Screen(s).layer_cursor();
    const bool page0 = int32_t(Screen(s).editor_page()) == 0;
    if (c >= 5) {
      return asked.r3;
    }
    if (c == 3 || c == 4) {
      // A button: the layers' rows and the other button finished, the
      // button's row restarted and looped.
      static constexpr uint32_t kLrs3[2][4] = {{0x821FFECC, 0x821FFED8, 0x821FFEE4, 0x821FFEF0},
                                               {0x821FFF00, 0x821FFF0C, 0x821FFF18, 0x821FFF24}};
      static constexpr uint32_t kLrs4[2][4] = {{0x821FFE10, 0x821FFE1C, 0x821FFE28, 0x821FFE34},
                                               {0x821FFE44, 0x821FFE50, 0x821FFE5C, 0x821FFE68}};
      const uint32_t(&lrs)[4] = c == 3 ? kLrs3[page0 ? 0 : 1] : kLrs4[page0 ? 0 : 1];
      const uint64_t first = page0 ? 195 : 169, bar = page0 ? 213 : 189;
      for (uint32_t k = 0; k < 3; k++) {
        finish(first + k, lrs[k]);
      }
      finish(bar, lrs[3]);
      const uint64_t button = c == 3 ? 44 : 45;
      play(bar, c == 3 ? 0x821FFF40 : 0x821FFE84);
      finish(button, c == 3 ? 0x821FFF4C : 0x821FFE90);
      play(button, c == 3 ? 0x821FFF68 : 0x821FFEAC);
      return loop(button, 0x821FFF74);
    }
    // A layer's row restarted and looped, the buttons finished, 46 played.
    static constexpr uint32_t kFinishLrs[3][2] = {
        {0x82200060, 0x8220008C}, {0x821FFFF8, 0x82200024}, {0x821FFF90, 0x821FFFBC}};
    static constexpr uint32_t kPlayLrs[3][2] = {
        {0x8220007C, 0x822000A8}, {0x82200014, 0x82200040}, {0x821FFFAC, 0x821FFFD8}};
    const uint64_t row = (page0 ? 195 : 169) + c;
    finish(row, kFinishLrs[c][page0 ? 0 : 1]);
    play(row, kPlayLrs[c][page0 ? 0 : 1]);
    loop(row, 0x822000B4);
    finish(44, 0x822000C0);
    finish(45, 0x822000CC);
    finish(46, 0x822000D8);
    return play(46, 0x822000F4);
  }
  if (asked(40, 159, 0x821FFD14)) {
    // A: on a layer, the rows finished and the layer's picker row played.
    const int32_t c = int32_t(Screen(s).layer_cursor());
    if (c < 0 || c > 2) {
      return asked.r3;
    }
    const bool page0 = int32_t(Screen(s).editor_page()) == 0;
    if (page0) {
      finish(195, 0x821FFD48);
      finish(196, 0x821FFD54);
      finish(197, 0x821FFD60);
    } else {
      finish(169, 0x821FFD74);
      finish(170, 0x821FFD80);
      finish(171, 0x821FFD8C);
    }
    const uint64_t named = uint64_t(Screen(s).layer_cursor()) + (page0 ? 198 : 172);
    return play(named, 0x822000F4);
  }
  // B asked; its answer's low byte stored into the frame (nothing else).
  if (int32_t(schema::PlayerMapperEntry(entry).unread()) == 1) {
    return asked.r3;
  }
  asked(40, 158, 0x821FFDBC);
  Store(frame + 80, uint32_t(asked.r3 & 0xFF));
  return asked.r3;
}

uint64_t InputVinylLayers(const tuning_screen::GuestCall& call, uint64_t screen,
                          uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_82208CE0 TuningScreen::InputVinylLayers");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 176);
  const uint32_t entry = PlayerEntry();
  const uint32_t found = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(found);
  const uint64_t images = screen + 10048;
  const bool native_free = flags.native_heap && flags.native_free;
  Reads asked{call, entry, frame, flags.native_slots, found};
  // r3 as the original leaves it: the last call's (the reads' while no
  // other call follows them).
  uint64_t r3 = found;
  bool called = false;
  auto sound = [&](uint64_t cue, uint32_t lr) {
    r3 = call(kMenuSoundPlay, {cue}, frame, lr);
    called = true;
  };
  auto finish = [&](uint64_t named, uint32_t lr) {
    r3 = tuning_screen::FinishNamed(call, frame, lr, movie, named);
    called = true;
  };
  auto cursor = [&] { return int32_t(Screen(s).layer_cursor()); };
  // Push(24), then Go(state, command).
  auto go = [&](uint32_t state, uint32_t command) {
    Push(s, 24);
    Go(s, state, command);
  };
  if (G::layers_wait() == 0) {
    bool answered = true;
    if (asked(48, 153, 0x82208D6C) || asked(48, 167, 0x82208D98)) {
      // Up: from the first layer to the button last left (+30736), from the
      // buttons to the last layer.
      sound(4, 0x82209464);
      const int32_t c = cursor();
      if (c == 0) {
        Screen(s).set_layer_cursor(Screen(s).layer_button());
      } else if (c > 0 && c < 3) {
        Screen(s).set_layer_cursor(uint32_t(c - 1));
      } else if (c == 3 || c == 4) {
        Screen(s).set_layer_cursor(2);
      } else {
        answered = false;
      }
      if (answered) {
        Screen(s).set_refresh_pending(1);
      }
    } else if (asked(48, 154, 0x82208DC4) || asked(48, 168, 0x82208DF0)) {
      // Down: from the last layer to the button, from the buttons to the
      // first layer.
      sound(4, 0x82209418);
      const int32_t c = cursor();
      if (c == 2) {
        Screen(s).set_layer_cursor(Screen(s).layer_button());
      } else if (c >= 0 && c < 2) {
        Screen(s).set_layer_cursor(uint32_t(c + 1));
      } else if (c == 3 || c == 4) {
        Screen(s).set_layer_cursor(0);
      } else {
        answered = false;
      }
      if (answered) {
        Screen(s).set_refresh_pending(1);
      }
    } else if (asked(48, 155, 0x82208E1C) || asked(48, 169, 0x82208E48)) {
      // Left: between the buttons only.
      const int32_t c = cursor();
      if (c == 3 || c == 4) {
        sound(4, 0x822093F0);
        const int32_t next = cursor() - 1;
        Screen(s).set_refresh_pending(1);
        Screen(s).set_layer_cursor(uint32_t(next));
        if (next < 3) {
          Screen(s).set_layer_cursor(4);
        }
        Screen(s).set_layer_button(Screen(s).layer_cursor());
      }
    } else if (asked(48, 156, 0x82208E74) || asked(48, 170, 0x82208EA0)) {
      // Right: the same the other way.
      const int32_t c = cursor();
      if (c == 3 || c == 4) {
        sound(4, 0x822093A8);
        const int32_t next = cursor() + 1;
        Screen(s).set_refresh_pending(1);
        Screen(s).set_layer_cursor(uint32_t(next));
        if (next > 4) {
          Screen(s).set_layer_cursor(3);
        }
        Screen(s).set_layer_button(Screen(s).layer_cursor());
      }
    } else if (asked(40, 159, 0x82208ECC)) {
      sound(0, 0x82208EDC);
      const int32_t c = cursor();
      Screen(s).set_refresh_pending(1);
      Screen(s).set_command_first_run(1);
      if (c >= 0 && c <= 2) {
        const uint32_t page = Screen(s).editor_page();
        const uint32_t id = Load(((page * 3 + uint32_t(c) + 7798) << 2) + s);
        if (int32_t(id) == -1) {
          // An empty layer: the image picker's wait started, the picker's
          // row played, the page's six picker textures put back to their
          // placeholder.
          Screen(s).set_picked_image(0);
          Screen(s).set_image_group(0);
          G::set_layers_wait(1);
          const bool page0 = int32_t(page) == 0;
          if (page0) {
            finish(195, 0x82208F3C);
            finish(196, 0x82208F48);
            finish(197, 0x82208F54);
          } else {
            finish(169, 0x82208F68);
            finish(170, 0x82208F74);
            finish(171, 0x82208F80);
          }
          const uint64_t named = uint64_t(Screen(s).layer_cursor()) + (page0 ? 198 : 172);
          r3 = tuning_screen::PlayNamed(call, frame, 0x82208FA0, movie, named, 1);
          const int32_t now = int32_t(Screen(s).editor_page());
          if (now == 0 || now == 1) {
            const schema::DecalImages decals(uint32_t(images));
            const uint32_t first = now == 0 ? 168 : 160;
            for (uint32_t k = 0; k < 6; k++) {
              const uint32_t again = Load(tuning_screen::MovieEntry(frame, 13));
              const uint32_t value =
                  now == 0 ? decals.vinyl_placeholder_0() : decals.vinyl_placeholder_1();
              MovieTexture(again, first + k).set_texture(value);
              r3 = RestoreStickerTexture(images, uint32_t(now), first + k, frame);
            }
          }
        } else {
          // A layer with an image: its actions.
          Screen(s).set_operation_cursor(0);
          go(25, 17);
        }
      } else if (c == 3) {
        Screen(s).set_before_state_35(0);
        go(35, 30);
      } else if (c == 4) {
        Screen(s).set_before_state_36(0);
        go(36, 30);
      }
    } else if (asked(40, 158, 0x82209370)) {
      // B: to the second button.
      sound(2, 0x82209380);
      Screen(s).set_layer_button(4);
      Screen(s).set_layer_cursor(4);
      Screen(s).set_refresh_pending(1);
    }
    if (!called) {
      r3 = asked.r3;
    }
    if (G::layers_wait() == 0) {
      return MoveVinylLayers(call, screen, frame, flags.native_slots);
    }
  }
  // The wait: on its third frame the picker's window freed and loaded from
  // the picked image; after it, once nothing loads, the window's middle
  // texture bound to the layer and the image picker.
  const uint32_t count = G::layers_wait() + 1;
  G::set_layers_wait(count);
  if (int32_t(count) == 3) {
    const int32_t page = int32_t(Screen(s).editor_page());
    if (page != 0 && page != 1) {
      return r3;
    }
    const uint32_t first = page == 0 ? 12 : 33;
    for (uint32_t k = 0; k < 9; k++) {
      FreeTexture(images, first + k,
                  GuestTextureFree(call, frame, page == 0 ? 0x822094E4 : 0x822095E0,
                                   native_free));
    }
    const uint64_t picked = Screen(s).picked_image();
    const uint64_t x = picked - 4;
    uint64_t v;
    if (page == 0) {
      // Brought into 0 to 4, twice at most.
      auto once = [](uint64_t y) {
        return int32_t(y) < 5 ? (int32_t(y) >= 0 ? y : y + 5) : y - 5;
      };
      v = once(once(x)) + 163;
    } else {
      v = (int32_t(x) >= 194 ? x - 194 : int32_t(x) < 0 ? x + 194 : x) + 189;
    }
    return LoadWindow(call, images, v, first, 9, uint32_t(page), frame);
  }
  if (int32_t(count) < 3) {
    // (A wait of -1 comes to 0 here: the movie side runs.)
    if (count == 0) {
      return MoveVinylLayers(call, screen, frame, flags.native_slots);
    }
    return r3;
  }
  for (uint32_t i = 0; i < 200; i++) {
    if (int32_t(DecalEntryAt(images, i).state()) == 1) {
      return r3;
    }
  }
  const int32_t page = int32_t(Screen(s).editor_page());
  if (page == 0 || page == 1) {
    const uint32_t header = DecalEntryAt(images, page == 0 ? 16 : 37).texture_header();
    const uint32_t n = Screen(s).layer_cursor() + (page == 0 ? 191 : 194);
    MovieTexture(movie, n).set_texture(header);
  }
  Screen(s).set_refresh_pending(1);
  Screen(s).set_command_first_run(1);
  G::set_layers_wait(0);
  go(28, 22);
  return MoveVinylLayers(call, screen, frame, flags.native_slots);
}

uint64_t MoveVinylImages(const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_82202530 TuningScreen::MoveVinylImages");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = PlayerEntry();
  const uint32_t found = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(found);
  Reads asked{call, entry, frame, native_slots, found};
  // Nothing while 164 is held or released, or on a move left or right.
  if (asked(32, 164, 0x82202598) || asked(44, 164, 0x822025C4) || asked(48, 155, 0x822025F0) ||
      asked(48, 169, 0x8220261C) || asked(48, 156, 0x82202648) ||
      asked(48, 170, 0x82202674) || !asked(40, 158, 0x822026A0)) {
    return asked.r3;
  }
  // B: the window's arrows finished, then the layer's row (234 an empty
  // layer, else 224) restarted.
  const int32_t page = int32_t(Screen(s).editor_page());
  if (page == 0) {
    tuning_screen::FinishNamed(call, frame, 0x822026C0, movie, 167);
    tuning_screen::FinishNamed(call, frame, 0x822026E8, movie, 166);
  } else if (page == 1) {
    tuning_screen::FinishNamed(call, frame, 0x822026DC, movie, 162);
    tuning_screen::FinishNamed(call, frame, 0x822026E8, movie, 161);
  }
  const uint32_t n = Screen(s).editor_page() * 3 + Screen(s).layer_cursor();
  const bool empty = int32_t(Load(((n + 7798) << 2) + s)) == -1;
  const uint64_t named = empty ? 234 : 224;
  tuning_screen::FinishNamed(call, frame, empty ? 0x82202718 : 0x82202728, movie, named);
  return tuning_screen::PlayNamed(call, frame, 0x82202744, movie, named, 1);
}

namespace {

// InputVinylImages' wait words.
constexpr uint32_t kShowNode = 0x8211AC00;    // UiMovie::ShowNode(movie, sub, node)
constexpr uint32_t kHideNode = 0x8211AC48;    // UiMovie::HideNode(movie, sub, node)

// once(y): y brought into 0 to 4 by one step of 5.
uint64_t Wrap5(uint64_t y) {
  return int32_t(y) >= 5 ? y - 5 : int32_t(y) < 0 ? y + 5 : y;
}

}  // namespace

uint64_t InputVinylImages(const tuning_screen::GuestCall& call, uint64_t screen,
                          uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_822058F8 TuningScreen::InputVinylImages");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 240);
  const uint32_t found = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(found);
  const uint32_t entry = PlayerEntry();
  const uint64_t images = screen + 10048;
  const uint32_t im = uint32_t(images);
  const bool native_free = flags.native_heap && flags.native_free;
  Reads asked{call, entry, frame, flags.native_slots, found};
  // r3 as the original leaves it: the last call's (the reads' too).
  uint64_t r3 = found;
  auto read = [&](uint32_t offset, uint64_t action, uint32_t lr) {
    asked.r3 = r3;
    const bool answer = asked(offset, action, lr);
    r3 = asked.r3;
    return answer;
  };
  auto sound = [&](uint64_t cue, uint32_t lr) { r3 = call(kMenuSoundPlay, {cue}, frame, lr); };
  auto free_texture = [&](uint64_t i, uint32_t lr) {
    r3 = FreeTexture(images, i, GuestTextureFree(call, frame, lr, native_free));
  };
  auto bind = [&](uint64_t i, uint32_t group) {
    r3 = BindVinylTexture(images, uint32_t(i), group, frame);
  };
  auto memcpy36 = [&](uint64_t destination, uint64_t source, uint32_t lr) {
    r3 = Memcpy(call, destination, source, 36, frame, lr);
  };
  auto loading = [&] {
    bool any = false;
    for (uint32_t i = 0; i < 200; i++) {
      if (int32_t(DecalEntryAt(im, i).state()) == 1) {
        any = true;
      }
    }
    return any;
  };
  auto layer = [&] { return uint64_t(Screen(s).layer_cursor()); };
  auto picked = [&] { return uint64_t(Screen(s).picked_image()); };
  auto page = [&] { return int32_t(Screen(s).editor_page()); };
  // The saved id word of the layer: ((page × 3 + l) + 7798) × 4 + s.
  auto saved_id = [&] {
    return ((Screen(s).editor_page() * 3 + Screen(s).layer_cursor() + 7798) << 2) + s;
  };
  // Movie 13's texture n (by the movie first looked up) = decal image
  // `entry`'s texture.
  auto set_slot = [&](uint32_t n, uint32_t entry) {
    const uint32_t value = DecalEntryAt(im, entry).texture_header();
    MovieTexture(movie, n).set_texture(value);
  };
  if (G::pick_wait() == 0 && G::scroll_wait() == 0 &&
      G::group_wait() == 0) {
    if (read(48, 155, 0x822059B0) || read(48, 169, 0x822059DC)) {
      // Left: the previous image (wrapped to +30796), the window scrolled.
      sound(4, 0x822061A8);
      const uint64_t x = picked() - 1;
      G::set_scroll_wait(1);
      Screen(s).set_refresh_pending(1);
      Screen(s).set_picked_image(uint32_t(x));
      if (int32_t(x) < 0) {
        Screen(s).set_picked_image(Screen(s).image_last_index());
      }
      const int32_t p = page();
      if (p == 0) {
        const uint64_t id = Wrap5(Wrap5(picked() - 4)) + 163;
        r3 = ScrollImages(call, images, 0, 12, 9, id, frame, native_free);
        set_slot(Screen(s).layer_cursor() + 191, 16);
      } else if (p == 1) {
        SetImageGroup(screen, picked());
        r3 = screen;
        const uint64_t v = picked();
        const uint64_t y = v - 4;
        const uint64_t id = (int32_t(y) >= 194 ? v - 198 : int32_t(y) < 0 ? v + 190 : y) + 189;
        r3 = ScrollImages(call, images, 0, 33, 9, id, frame, native_free);
        set_slot(Screen(s).layer_cursor() + 194, 37);
      }
      G::set_scroll_way(1);
    } else if (read(48, 156, 0x82205A08) || read(48, 170, 0x82205A34)) {
      // Right: the next image (wrapped to 0), the window scrolled.
      sound(4, 0x8220600C);
      const uint64_t x = picked() + 1;
      const uint32_t last = Screen(s).image_last_index();
      G::set_scroll_wait(1);
      Screen(s).set_refresh_pending(1);
      Screen(s).set_picked_image(uint32_t(x));
      if (int32_t(x) > int32_t(last)) {
        Screen(s).set_picked_image(0);
      }
      const int32_t p = page();
      if (p == 0) {
        const uint64_t id = Wrap5(Wrap5(picked() + 4)) + 163;
        r3 = ScrollImages(call, images, 1, 12, 9, id, frame, native_free);
        set_slot(Screen(s).layer_cursor() + 191, 16);
      } else if (p == 1) {
        SetImageGroup(screen, picked());
        r3 = screen;
        const uint64_t v = picked();
        const uint64_t y = v + 4;
        const uint64_t id = (int32_t(y) >= 194 ? v - 190 : int32_t(y) < 0 ? v + 198 : y) + 189;
        r3 = ScrollImages(call, images, 1, 33, 9, id, frame, native_free);
        set_slot(Screen(s).layer_cursor() + 194, 37);
      }
      G::set_scroll_way(2);
    } else if (read(32, 164, 0x82205A60) || read(44, 164, 0x82205A8C)) {
      // 164 held or released: nothing.
    } else if (read(48, 161, 0x82205AB8) || read(48, 162, 0x82205AE4)) {
      // The group: page 0's window swapped with the kept textures; page 1's
      // group stepped and its window loaded.
      sound(4, 0x82205E6C);
      const int32_t p = page();
      G::set_group_wait(1);
      Screen(s).set_refresh_pending(1);
      if (p == 0) {
        Screen(s).set_picked_image(0);
        for (uint32_t k = 0; k < 9; k++) {
          const schema::DecalImageEntry e = DecalEntryAt(im, 12 + k);
          const schema::DecalImageKept kept = DecalKeptAt(im, 6 + k);
          const uint32_t kept_header = kept.texture_header();
          const uint32_t header = e.texture_header();
          const uint32_t data = e.texture_data();
          e.set_texture_header(kept_header);
          e.set_texture_data(kept.texture_data());
          kept.set_texture_header(header);
          kept.set_texture_data(data);
          bind(12 + k, 0);
        }
        set_slot(Screen(s).layer_cursor() + 191, 16);
      } else if (p == 1) {
        if (read(48, 162, 0x82205F30)) {
          const uint32_t g = Screen(s).image_group() + 1;
          Screen(s).set_image_group(g);
          if (int32_t(g) > 11) {
            Screen(s).set_image_group(0);
          }
        } else if (read(48, 161, 0x82205F78)) {
          const uint32_t g = Screen(s).image_group() - 1;
          Screen(s).set_image_group(g);
          if (int32_t(g) < 0) {
            Screen(s).set_image_group(11);
          }
        }
        SetGroupImage(screen, Screen(s).image_group());
        r3 = screen;
        for (uint32_t j = 0; j < 9; j++) {
          free_texture(42 + j, 0x82205FB8);
        }
        const uint64_t v = picked();
        const uint64_t y = v - 4;
        const uint64_t first = (int32_t(y) >= 194 ? v - 198 : int32_t(y) < 0 ? v + 190 : y) + 189;
        r3 = LoadWindow(call, images, first, 42, 9, 1, frame);
      }
    } else if (read(40, 159, 0x82205B10) ||
               (read(40, 159, 0x82205B3C) && read(48, 163, 0x82205B68))) {
      // A: the image picked (163 fired too: the layer's saved record back
      // into the edited decal; else the edited decal and the saved record
      // reset), the layer's two entries freed and reloaded with it.
      sound(0, 0x82205C8C);
      G::set_pick_wait(1);
      if (read(40, 159, 0x82205CB4) && read(48, 163, 0x82205CE0)) {
        const uint64_t n = uint64_t(Screen(s).editor_page()) * 3 + Screen(s).layer_cursor();
        Screen(s).set_state(26);
        r3 = Memcpy(call, screen + 30832, n * 24 + screen + 31024, 24, frame, 0x82205D18);
      } else if (read(40, 159, 0x82205D40)) {
        const uint32_t angle = Load(G::zero_address());
        Screen(s).set_state(26);
        Screen(s).set_edited_angle(std::bit_cast<float>(uint32_t(angle)));
        Screen(s).set_edited_x(0);
        Screen(s).set_edited_y(0);
        Screen(s).set_edited_flip(0);
        const uint32_t scale = Load(G::one_address());
        Screen(s).set_edited_word(0);
        Screen(s).set_edited_scale(std::bit_cast<float>(uint32_t(scale)));
        const uint32_t record =
            (Screen(s).editor_page() * 3 + Screen(s).layer_cursor()) * 24 + s;
        Store(record + 31032, angle);
        Store(record + 31024, 0);
        Store(record + 31036, scale);
        Store(record + 31028, 0);
        Store(record + 31040, 0);
        Store(record + 31044, 0);
      }
      const int32_t p = page();
      if (p == 0 || p == 1) {
        const uint64_t a = p == 0 ? 3 : 24, b = p == 0 ? 9 : 30, list = p == 0 ? 163 : 189;
        free_texture(layer() + a, p == 0 ? 0x82205DC8 : 0x82205E20);
        free_texture(layer() + b, p == 0 ? 0x82205DD8 : 0x82205E30);
        const uint64_t i = layer() + a;
        r3 = ReloadImage(call, images, picked() + list, i, frame);
        const uint64_t j = layer() + b;
        r3 = ReloadImage(call, images, picked() + list, j, frame);
      }
    } else if (read(40, 158, 0x82205B94)) {
      // B: back to the state before, the layer's texture bound again.
      sound(2, 0x82205BA4);
      Screen(s).set_refresh_pending(1);
      const uint32_t popped = Pop(s);
      GoFirstHalf(s, popped);
      const int32_t p = page();
      if (p == 0) {
        bind(layer() + 6, 0);
      } else if (p == 1) {
        bind(layer() + 27, 1);
      }
      const bool empty = int32_t(Load(saved_id())) == -1;
      GoSecondHalf(s, empty ? 16 : 17);
    }
  }
  if (G::pick_wait() != 0 && !loading()) {
    // The pick's loads done: to the state +9652 (26 the layer's edit menu,
    // 25 its actions, 24 the layers), the saved id = the picked image, the
    // window's middle entries swapped into the layer's.
    G::set_pick_wait(0);
    Screen(s).set_refresh_pending(1);
    const int32_t state = int32_t(Screen(s).state());
    if (state == 26) {
      Screen(s).set_command_first_run(1);
      Push(s, 28);
      Screen(s).set_edit_cursor(0);
      Screen(s).set_edit_row_saved(5);
      GoFirstHalf(s, Screen(s).state());
      GoSecondHalf(s, 18);
    } else if (state == 25) {
      r3 = tuning_screen::FinishNamed(call, frame, 0x82206410, movie, 224);
      r3 = tuning_screen::PlayNamed(call, frame, 0x8220642C, movie, 224, 1);
      GoFirstHalf(s, Screen(s).state());
      GoSecondHalf(s, 17);
    } else if (state == 24) {
      if (int32_t(Load(saved_id())) == -1) {
        r3 = tuning_screen::FinishNamed(call, frame, 0x822064A4, movie, 234);
        r3 = tuning_screen::PlayNamed(call, frame, 0x822064C0, movie, 234, 1);
      }
      GoFirstHalf(s, Screen(s).state());
      GoSecondHalf(s, 16);
    }
    Store(saved_id(), Screen(s).picked_image());
    const int32_t p = page();
    if (p == 0 || p == 1) {
      // Entries (a + l, b + l) and (c + l, d + l) swapped through the scratch
      // entry at images + 7200.
      static constexpr uint32_t kLrs[2][6] = {
          {0x8220655C, 0x82206574, 0x82206584, 0x822065AC, 0x822065C4, 0x822065D4},
          {0x82206628, 0x82206640, 0x82206650, 0x82206678, 0x82206690, 0x822066A0}};
      const uint32_t(&lrs)[6] = kLrs[p];
      const uint64_t scratch = images + 7200;
      const uint64_t first[2] = {p == 0 ? 3u : 24u, p == 0 ? 9u : 30u};
      const uint64_t second[2] = {p == 0 ? 0u : 21u, p == 0 ? 6u : 27u};
      for (uint32_t k = 0; k < 2; k++) {
        const uint64_t l = layer();
        const uint64_t from = (l + first[k]) * 36 + images;
        const uint64_t to = (l + second[k]) * 36 + images;
        memcpy36(scratch, from, lrs[3 * k]);
        memcpy36(from, to, lrs[3 * k + 1]);
        memcpy36(to, scratch, lrs[3 * k + 2]);
      }
      const uint32_t group = p == 0 ? 0 : 1;
      bind(layer() + second[0], group);
      bind(layer() + second[1], group);
    }
  }
  if (G::scroll_wait() != 0 && !loading() && G::scroll_way() != 0) {
    // The scroll's loads done: command 23 (the window's move) the way it
    // went.
    const uint32_t way = G::scroll_way();
    const uint32_t queue = Screen(s).queue().address;
    G::set_scroll_wait(0);
    Screen(s).set_scroll_direction(way);
    const uint32_t current = CommandQueue(queue).page();
    if (int32_t(current) != 28) {
      CommandQueue(queue).set_previous_page(current);
    }
    CommandQueue(queue).set_next(kNone);
    CommandQueue(queue).set_resume(28);
    CommandQueue(queue).set_page(kNone);
    CommandQueue(queue).set_current(23);
    G::set_scroll_way(0);
  }
  if (G::group_wait() != 0) {
    const int32_t p = page();
    if (p == 0) {
      // On the third frame the kept textures freed and kept again from the
      // window.
      const uint32_t count = G::group_wait() + 1;
      G::set_group_wait(count);
      if (int32_t(count) == 3) {
        G::set_group_wait(0);
        for (uint32_t k = 0; k < 9; k++) {
          r3 = FreeKept(images, 6 + k,
                        GuestTextureFree(call, frame, 0x822067A0, native_free));
          KeepTexture(images, 12 + k, 6 + k);
        }
      }
    } else if (p == 1 && !loading()) {
      if (int32_t(G::group_wait()) == 1) {
        // The group's window (42 to 50) swapped with the list's (33 to 41),
        // movie 13's slots +640 to +660 = entries 35 to 40's headers.
        for (uint32_t j = 0; j < 9; j++) {
          const schema::DecalImageEntry a = DecalEntryAt(im, 42 + j);
          const schema::DecalImageEntry b = DecalEntryAt(im, 33 + j);
          const uint32_t a_header = a.texture_header();
          const uint32_t b_header = b.texture_header();
          const uint32_t a_data = a.texture_data();
          a.set_texture_header(b_header);
          a.set_texture_data(b.texture_data());
          b.set_texture_header(a_header);
          b.set_texture_data(a_data);
        }
        for (uint32_t k = 0; k < 6; k++) {
          const uint32_t again = tuning_screen::MovieEntry(frame, 13);
          r3 = again;
          const uint32_t value = DecalEntryAt(im, 35 + k).texture_header();
          MovieTexture(Load(again), 160 + k).set_texture(value);
        }
        set_slot(Screen(s).layer_cursor() + 194, 37);
      }
      const uint32_t count = G::group_wait() + 1;
      G::set_group_wait(count);
      if (int32_t(count) == 3) {
        G::set_group_wait(0);
        for (uint32_t j = 0; j < 9; j++) {
          free_texture(42 + j, 0x82206998);
          const schema::DecalImageEntry a = DecalEntryAt(im, 42 + j);
          const schema::DecalImageEntry b = DecalEntryAt(im, 33 + j);
          a.set_texture_header(b.texture_header());
          a.set_texture_data(b.texture_data());
        }
      }
    }
  }
  // The layer's node shown or hidden.
  const int32_t p = page();
  if (p == 0 || p == 1) {
    const bool shown = G::show_picker_node() != 0;
    const uint64_t node = layer() + (p == 0 ? 19 : 22);
    r3 = call(shown ? kShowNode : kHideNode, {movie, ~0ull, node}, frame,
              shown ? 0x82206A10 : 0x82206A18);
  }
  if (G::pick_wait() == 0 && G::scroll_wait() == 0 &&
      G::group_wait() == 0) {
    r3 = MoveVinylImages(call, screen, frame, flags.native_slots);
  }
  return r3;
}

void BeginLayerOrder(uint64_t screen) {
  ITC_NATIVE_RUNNING("sub_821FE370 TuningScreen::BeginLayerOrder");
  const uint32_t s = uint32_t(screen);
  if (int32_t(Screen(s).category()) != 5 || uint32_t(Screen(s).item()) != 6) {
    return;
  }
  // The page read again each pass, the index stored before the id.
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t page = Screen(s).editor_page();
    const uint32_t id = Load(((page * 3 + k + 7798) << 2) + s);
    Store(Screen(s).order_indices_address() + 4 * k, k);
    Store(Screen(s).order_ids_address() + 4 * k, id);
  }
}

uint64_t MoveLayerActions(const tuning_screen::GuestCall& call, uint64_t screen,
                          uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_82200230 TuningScreen::MoveLayerActions");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = PlayerEntry();
  const uint32_t found = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(found);
  Reads asked{call, entry, frame, native_slots, found};
  uint64_t r3 = found;
  auto finish = [&](uint64_t named, uint32_t lr) {
    r3 = tuning_screen::FinishNamed(call, frame, lr, movie, named);
  };
  auto play = [&](uint64_t named, uint32_t lr) {
    r3 = tuning_screen::PlayNamed(call, frame, lr, movie, named, 1);
  };
  auto cursor = [&] { return uint64_t(Screen(s).operation_cursor()); };
  // The operate items' rows (49 to 52) and both arrows (60 twice) finished.
  auto finish_rows = [&](const uint32_t (&lrs)[6]) {
    static constexpr uint32_t kRows[6] = {60, 60, 49, 50, 51, 52};
    for (uint32_t k = 0; k < 6; k++) {
      finish(kRows[k], lrs[k]);
    }
  };
  const bool up = asked(48, 153, 0x82200298) || asked(48, 167, 0x822002C4);
  const bool down = !up && (asked(48, 154, 0x822002F0) || asked(48, 168, 0x8220031C));
  r3 = asked.r3;
  if (up || down) {
    if (Screen(s).refresh_pending() == 0) {
      return r3;
    }
    uint64_t named;
    if (Screen(s).operation_wrapped() != 0) {
      // A wrap: both arrows finished, the cursor's bar placed, the row it
      // left (52 down, 49 up) finished.
      finish(59, up ? 0x82200558 : 0x822004B8);
      finish(60, up ? 0x82200564 : 0x822004C4);
      const uint32_t node = ui_movies::FindNode(movie, 10, 0);
      schema::UiMovieNode(node).set_y(std::bit_cast<float>(uint32_t(BarPosition(frame, Screen(s).operation_cursor()))));
      r3 = node;
      named = up ? 49 : 52;
    } else {
      // The arrow (59 up, 60 down) restarted, the row left finished.
      finish(up ? 59 : 60, up ? 0x8220058C : 0x8220050C);
      play(up ? 59 : 60, up ? 0x822005A8 : 0x82200528);
      named = cursor() + (up ? 50 : 48);
    }
    finish(named, 0x822005B8);
    finish(cursor() + 49, 0x822005C8);
    play(cursor() + 49, 0x822005E8);
    r3 = tuning_screen::LoopNamed(call, frame, 0x822005F8, movie, cursor() + 49);
    return r3;
  }
  if (asked(32, 164, 0x82200348) || asked(44, 164, 0x82200374)) {
    return asked.r3;
  }
  if (asked(40, 159, 0x822003A0)) {
    finish_rows({0x822003B4, 0x822003C0, 0x822003CC, 0x822003D8, 0x822003E4, 0x822003F0});
    return r3;
  }
  if (asked(40, 158, 0x82200418)) {
    finish_rows({0x8220042C, 0x82200438, 0x82200444, 0x82200450, 0x8220045C, 0x82200468});
    finish(222, 0x82200474);
    play(222, 0x82200490);
    return r3;
  }
  return asked.r3;
}

uint64_t InputLayerActions(const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_82209738 TuningScreen::InputLayerActions");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 176);
  const uint32_t entry = PlayerEntry();
  const uint32_t found = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(found);
  const uint64_t images = screen + 10048;
  const bool native_free = flags.native_heap && flags.native_free;
  Reads asked{call, entry, frame, flags.native_slots, found};
  // r3 as the original leaves it: the last call's.
  uint64_t r3 = found;
  auto ask = [&](uint32_t offset, uint64_t action, uint32_t lr) {
    if (int32_t(schema::PlayerMapperEntry(entry).unread()) == 1) {
      return false;
    }
    const bool answered = asked(offset, action, lr);
    r3 = asked.r3;
    return answered;
  };
  auto sound = [&](uint64_t cue, uint32_t lr) { r3 = call(kMenuSoundPlay, {cue}, frame, lr); };
  auto finish = [&](uint64_t named, uint32_t lr) {
    r3 = tuning_screen::FinishNamed(call, frame, lr, movie, named);
  };
  // Push(25), then Go(state, command).
  auto go = [&](uint32_t state, uint32_t command) {
    Push(s, 25);
    Go(s, state, command);
  };
  auto move = [&] { return MoveLayerActions(call, screen, frame, flags.native_slots); };
  // 164 held or released: the selected item lit (all three on release).
  if (ask(32, 164, 0x822097A4)) {
    r3 = tuning_screen::HighlightSelected(screen, 0, frame);
  } else if (ask(44, 164, 0x822097D8)) {
    r3 = tuning_screen::HighlightSelected(screen, 1, frame);
  }
  if (G::actions_wait() == 0) {
    if (ask(48, 153, 0x82209830) || ask(48, 167, 0x8220985C)) {
      // Up: the cursor +30740 back one, from 0 around to 3 (+30744 the wrap).
      sound(4, 0x82209F00);
      const int32_t c = int32_t(Screen(s).operation_cursor()) - 1;
      Screen(s).set_refresh_pending(1);
      Screen(s).set_operation_cursor(uint32_t(c));
      if (c < 0) {
        Screen(s).set_operation_cursor(3);
      }
      Screen(s).set_operation_wrapped(c < 0 ? 1 : 0);
    } else if (ask(48, 154, 0x82209888) || ask(48, 168, 0x822098B4)) {
      // Down: on one, from 3 around to 0.
      sound(4, 0x82209ED8);
      const int32_t c = int32_t(Screen(s).operation_cursor()) + 1;
      Screen(s).set_refresh_pending(1);
      Screen(s).set_operation_cursor(uint32_t(c));
      if (c > 3) {
        Screen(s).set_operation_cursor(0);
      }
      Screen(s).set_operation_wrapped(c > 3 ? 1 : 0);
    } else if (ask(32, 164, 0x822098E0) || ask(44, 164, 0x8220990C)) {
      // 164 held or released: nothing.
    } else if (ask(40, 159, 0x82209938)) {
      // A: the rows and arrows finished, then the item.
      sound(0, 0x82209948);
      static constexpr uint32_t kRows[6] = {60, 60, 49, 50, 51, 52};
      static constexpr uint32_t kLrs[6] = {0x82209954, 0x82209960, 0x8220996C,
                                           0x82209978, 0x82209984, 0x82209990};
      for (uint32_t k = 0; k < 6; k++) {
        finish(kRows[k], kLrs[k]);
      }
      const uint32_t item = Screen(s).operation_cursor();
      if (item == 0) {
        // Edit: the layer's saved record into the edited decal.
        const uint64_t record =
            (uint64_t(Screen(s).editor_page()) * 3 + uint64_t(Screen(s).layer_cursor())) * 24 +
            screen + 31024;
        r3 = Memcpy(call, screen + 30832, record, 24, frame, 0x82209DB4);
        Screen(s).set_refresh_pending(1);
        Screen(s).set_command_first_run(1);
        Push(s, 25);
        Screen(s).set_state(26);
        Screen(s).set_edit_cursor(0);
        Screen(s).set_edit_row_saved(5);
        GoFirstHalf(s, 26);
        GoSecondHalf(s, 18);
      } else if (item == 1) {
        // The order.
        Screen(s).set_refresh_pending(1);
        Screen(s).set_command_first_run(1);
        Push(s, 25);
        Screen(s).set_state(27);
        Screen(s).set_order_position(Screen(s).layer_cursor());
        GoFirstHalf(s, 27);
        GoSecondHalf(s, 19);
        BeginLayerOrder(screen);
        r3 = screen;
      } else if (item == 2) {
        // Another image: the wait started from the layer's saved id, the
        // page's six picker textures put back to their placeholder.
        const uint32_t page = Screen(s).editor_page();
        const uint32_t layer = Screen(s).layer_cursor();
        G::set_actions_wait(1);
        const uint32_t id = Load(((page * 3 + layer + 7798) << 2) + s);
        Screen(s).set_image_group(0);
        Screen(s).set_picked_image(id);
        SetImageGroup(screen, id);
        r3 = screen;
        const int32_t now = int32_t(Screen(s).editor_page());
        if (now == 0 || now == 1) {
          const schema::DecalImages decals(uint32_t(images));
          const uint32_t first = now == 0 ? 168 : 160;
          for (uint32_t k = 0; k < 6; k++) {
            const uint32_t again = Load(tuning_screen::MovieEntry(frame, 13));
            const uint32_t value =
                now == 0 ? decals.vinyl_placeholder_0() : decals.vinyl_placeholder_1();
            MovieTexture(again, first + k).set_texture(value);
            r3 = RestoreStickerTexture(images, uint32_t(now), first + k, frame);
          }
        }
      } else if (item == 3) {
        // Remove.
        Screen(s).set_refresh_pending(1);
        Screen(s).set_command_first_run(1);
        Push(s, 25);
        Screen(s).set_state(37);
        Screen(s).set_before_state_37(0);
        GoFirstHalf(s, 37);
        GoSecondHalf(s, 30);
      }
    } else if (ask(40, 158, 0x82209E4C)) {
      // B: back to the state popped.
      sound(2, 0x82209E5C);
      Screen(s).set_refresh_pending(1);
      const uint32_t popped = Pop(s);
      GoFirstHalf(s, popped);
      GoSecondHalf(s, 16);
    }
    if (G::actions_wait() == 0) {
      return move();
    }
  }
  // The wait: counted every frame, used only on another image (+30740 2):
  // on its third frame the picker's window freed and loaded from the saved
  // id; after it, once nothing loads, the window's middle texture bound to
  // the layer's movie texture and the image picker.
  const uint32_t count = G::actions_wait() + 1;
  const int32_t item = int32_t(Screen(s).operation_cursor());
  G::set_actions_wait(count);
  if (item != 2 || int32_t(count) < 3) {
    // (A wait of -1 comes to 0 here: the movie side runs.)
    return count == 0 ? move() : r3;
  }
  if (int32_t(count) == 3) {
    const int32_t page = int32_t(Screen(s).editor_page());
    if (page != 0 && page != 1) {
      return r3;
    }
    const uint32_t first = page == 0 ? 12 : 33;
    for (uint32_t k = 0; k < 9; k++) {
      FreeTexture(images, first + k,
                  GuestTextureFree(call, frame, page == 0 ? 0x82209F74 : 0x8220A070,
                                   native_free));
    }
    const uint64_t x = uint64_t(Screen(s).picked_image()) - 4;
    uint64_t v;
    if (page == 0) {
      // Brought into 0 to 4, twice at most.
      auto once = [](uint64_t y) {
        return int32_t(y) < 5 ? (int32_t(y) >= 0 ? y : y + 5) : y - 5;
      };
      v = once(once(x)) + 163;
    } else {
      v = (int32_t(x) >= 194 ? x - 194 : int32_t(x) < 0 ? x + 194 : x) + 189;
    }
    r3 = LoadWindow(call, images, v, first, 9, uint32_t(page), frame);
    // (The wait read again.)
    return G::actions_wait() == 0 ? move() : r3;
  }
  for (uint32_t i = 0; i < 200; i++) {
    if (int32_t(DecalEntryAt(images, i).state()) == 1) {
      return r3;
    }
  }
  const int32_t page = int32_t(Screen(s).editor_page());
  if (page == 0 || page == 1) {
    const uint32_t n = Screen(s).layer_cursor() + (page == 0 ? 191 : 194);
    const uint32_t header = DecalEntryAt(images, page == 0 ? 16 : 37).texture_header();
    MovieTexture(movie, n).set_texture(header);
  }
  Screen(s).set_refresh_pending(1);
  Screen(s).set_command_first_run(1);
  G::set_actions_wait(0);
  go(28, 22);
  return move();
}


void SetGroupImage(uint64_t screen, uint64_t group) {
  ITC_NATIVE_RUNNING("sub_821FD5C0 TuningScreen::SetGroupImage");
  // The jump table over the group (its case 0 the piece sub_821FD5F0).
  static constexpr uint32_t kFirst[12] = {0, 43, 78, 106, 114, 142, 156, 165, 175, 180, 187, 192};
  const uint32_t g = uint32_t(group);
  if (g > 11) {
    return;
  }
  Store(uint32_t(screen) + 30764, kFirst[g]);
}

void SetImageGroup(uint64_t screen, uint64_t image) {
  ITC_NATIVE_RUNNING("sub_821FD658 TuningScreen::SetImageGroup");
  static constexpr int32_t kBounds[11] = {43, 78, 106, 114, 142, 156, 165, 175, 180, 187, 192};
  const int32_t i = int32_t(uint32_t(image));
  uint32_t group = 0;
  while (group < 11 && i >= kBounds[group]) {
    group++;
  }
  Store(uint32_t(screen) + 30768, group);
}

uint64_t SwapLayerRecords(const tuning_screen::GuestCall& call, uint64_t screen,
                          uint64_t first, uint64_t second, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FD518 TuningScreen::SwapLayerRecords");
  const uint32_t frame = Frame(sp, 144);
  const int32_t page = int32_t(Load(uint32_t(screen) + 30724));
  if (page != 0 && page != 1) {
    return screen;
  }
  const uint64_t records = page == 0 ? 31024 : 31096;
  const uint64_t a = first * 24 + screen + records;
  const uint64_t b = second * 24 + screen + records;
  const uint64_t temp = uint64_t(frame) + 80;
  Memcpy(call, temp, a, 24, frame, page == 0 ? 0x821FD554 : 0x821FD588);
  Memcpy(call, a, b, 24, frame, 0x821FD5A4);
  return Memcpy(call, b, temp, 24, frame, 0x821FD5B4);
}

void EndLayerOrder(uint64_t screen) {
  ITC_NATIVE_RUNNING("sub_821FE3C8 TuningScreen::EndLayerOrder");
  const uint32_t s = uint32_t(screen);
  if (int32_t(Screen(s).category()) != 5 || uint32_t(Screen(s).item()) != 6) {
    return;
  }
  // The page read again each pass, the id stored before the index.
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t page = Screen(s).editor_page();
    const uint32_t id = Load(Screen(s).order_ids_address() + 4 * k);
    Store(((page * 3 + k + 7798) << 2) + s, id);
    Store(Screen(s).order_indices_address() + 4 * k, k);
  }
}

uint64_t SwapOrderTextures(uint64_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82207F38 TuningScreen::SwapOrderTextures");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 128);
  const int32_t page = int32_t(Screen(s).editor_page());
  if (page != 0 && page != 1) {
    return screen;
  }
  // The two entries the move swapped (+30760 1 up, 2 down; other values
  // none), their texture words (+28, +32) exchanged.
  const int32_t way = int32_t(Screen(s).order_last_move());
  if (way == 1 || way == 2) {
    const uint64_t p = Screen(s).order_position();
    const uint64_t at = (page == 0 ? 7 : 28) - uint64_t(way - 1);
    const schema::DecalImageEntry a = DecalEntryAt(screen + 10048, p + at);
    const schema::DecalImageEntry b = DecalEntryAt(screen + 10048, p + at - 1);
    const uint32_t a_header = a.texture_header();
    a.set_texture_header(b.texture_header());
    const uint32_t b_data = b.texture_data();
    const uint32_t a_data = a.texture_data();
    a.set_texture_data(b_data);
    b.set_texture_data(a_data);
    b.set_texture_header(a_header);
  }
  // The movie's slots for entries 6 to 8 (page 0) or 27 to 29 (page 1), the
  // movie looked up again each time.
  const uint32_t first = page == 0 ? 6 : 27;
  const uint32_t textures = page == 0 ? 191 : 194;
  uint32_t entry = 0;
  for (uint32_t k = 0; k < 3; k++) {
    entry = tuning_screen::MovieEntry(frame, 13);
    const uint32_t header = DecalEntryAt(Screen(s).decal_images_address(), first + k).texture_header();
    MovieTexture(Load(entry), textures + k).set_texture(header);
  }
  return entry;
}

uint64_t ShowOrderNodes(uint64_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82203A30 TuningScreen::ShowOrderNodes");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(entry);
  const int32_t page = int32_t(Screen(s).editor_page());
  if (page != 0 && page != 1) {
    return entry;
  }
  // UiMovie::ShowNode and HideNode (sub_8211AC00, sub_8211AC48) of the
  // scene's node: its +80 set; r3 the node.
  uint32_t node = 0;
  for (uint32_t k = 0; k < 3; k++) {
    const bool empty = int32_t(Load(Screen(s).order_ids_address() + 4 * k)) == -1;
    node = ui_movies::FindNode(movie, kNone, (page == 0 ? 19 : 22) + k);
    schema::UiMovieNode(node).set_visible(empty ? 0 : 1);
  }
  return node;
}

uint64_t MoveLayerOrder(const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_82201DB8 TuningScreen::MoveLayerOrder");
  (void)screen;
  const uint32_t frame = Frame(sp, 112);
  const uint32_t entry = PlayerEntry();
  const uint32_t found = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(found);
  Reads asked{call, entry, frame, native_slots, found};
  // Nothing on 164 held or released, a move or A.
  if (asked(32, 164, 0x82201E24) || asked(44, 164, 0x82201E50) ||
      asked(48, 153, 0x82201E7C) || asked(48, 167, 0x82201EA8) ||
      asked(48, 154, 0x82201ED4) || asked(48, 168, 0x82201F00) ||
      asked(40, 159, 0x82201F2C) || !asked(40, 158, 0x82201F58)) {
    return asked.r3;
  }
  // B: 238 restarted.
  tuning_screen::FinishNamed(call, frame, 0x82201F6C, movie, 238);
  return tuning_screen::PlayNamed(call, frame, 0x82201F88, movie, 238, 1);
}

namespace {

// A float constant's single loaded and stored (lfs, stfs) at `to`.
void CopyFloat(uint32_t from, uint32_t to) {
  const float value = float(double(std::bit_cast<float>(Load(from))));
  Store(to, std::bit_cast<uint32_t>(value));
}

// An address in the screen's word array: (x << 2) + s, low 32 bits.
uint32_t WordAt(uint32_t s, uint64_t x) { return (uint32_t(x) << 2) + s; }

}  // namespace

uint64_t InputLayerOrder(const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_8220A1C8 TuningScreen::InputLayerOrder");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 176);
  const uint32_t entry = PlayerEntry();
  const uint32_t found = tuning_screen::MovieEntry(frame, 13);
  const uint32_t movie = Load(found);
  const uint64_t images = screen + 10048;
  Reads asked{call, entry, frame, flags.native_slots, found};
  auto sound = [&](uint64_t cue, uint32_t lr) { call(kMenuSoundPlay, {cue}, frame, lr); };
  Screen(s).set_order_last_move(0);
  bool counting = G::order_count() != 0;
  if (!counting) {
    // A move: the work list's ids (+31216 + 4k, words 7804 on) and indices
    // (+31228 + 4k, words 7807 on) at the position and the one it moves to
    // swapped, each word's index read again from +30756 as the original
    // does; then the records and textures swapped and command 20 queued.
    auto moved = [&](uint64_t r4, uint64_t r5, uint64_t at, uint64_t before) {
      Screen(s).set_vinyl_animation_a(Load(WordAt(s, at)));
      Screen(s).set_vinyl_animation_b(Load(WordAt(s, before)));
      SwapLayerRecords(call, screen, r4, r5, frame);
      SwapOrderTextures(screen, frame);
      const uint32_t queue = Screen(s).queue().address;
      const uint32_t current = CommandQueue(queue).page();
      if (int32_t(current) != 27) {
        CommandQueue(queue).set_previous_page(current);
      }
      CommandQueue(queue).set_next(kNone);
      CommandQueue(queue).set_page(kNone);
      CommandQueue(queue).set_resume(27);
      CommandQueue(queue).set_current(20);
    };
    auto position = [&] { return uint64_t(Screen(s).order_position()); };
    if (asked(48, 153, 0x8220A254) || asked(48, 167, 0x8220A280)) {
      // Up, from a position above 0.
      if (int32_t(Screen(s).order_position()) > 0) {
        sound(4, 0x8220A578);
        const uint64_t p = position();
        Screen(s).set_refresh_pending(1);
        Screen(s).set_order_last_move(1);
        const uint32_t id = Load(WordAt(s, p + 7804));
        Screen(s).set_order_position(uint32_t(p - 1));
        const uint32_t other = Load(WordAt(s, p - 1 + 7804));
        Store(WordAt(s, p - 1 + 7804), id);
        Store(WordAt(s, position() + 7805), other);
        const uint64_t q = position();
        const uint32_t index = Load(WordAt(s, q + 7808));
        const uint32_t before = Load(WordAt(s, q + 7807));
        Store(WordAt(s, q + 7807), index);
        Store(WordAt(s, position() + 7808), before);
        const uint64_t r5 = position();
        moved(r5 + 1, r5, r5 + 7808, r5 + 7807);
      }
    } else if (asked(48, 154, 0x8220A2AC) || asked(48, 168, 0x8220A2D8)) {
      // Down, from a position below the last (+30792).
      if (int32_t(Screen(s).order_position()) < int32_t(Screen(s).order_last_position())) {
        sound(4, 0x8220A4D8);
        const uint64_t p = position();
        Screen(s).set_refresh_pending(1);
        Screen(s).set_order_last_move(2);
        const uint32_t id = Load(WordAt(s, p + 7804));
        Screen(s).set_order_position(uint32_t(p + 1));
        const uint32_t other = Load(WordAt(s, p + 1 + 7804));
        Store(WordAt(s, p + 1 + 7804), id);
        Store(WordAt(s, position() + 7803), other);
        const uint64_t q = position();
        const uint32_t before = Load(WordAt(s, q + 7806));
        const uint32_t index = Load(WordAt(s, q + 7807));
        Store(WordAt(s, q + 7807), before);
        Store(WordAt(s, position() + 7806), index);
        const uint64_t r4 = position();
        moved(r4, r4 - 1, r4 + 7807, r4 + 7806);
      }
    } else if (asked(32, 164, 0x8220A304) || asked(44, 164, 0x8220A330)) {
      // 164 held or released: nothing.
    } else if (asked(40, 159, 0x8220A35C)) {
      // A: the order kept, the count started; the layer's animations.
      sound(0, 0x8220A36C);
      const int32_t page = int32_t(Screen(s).editor_page());
      G::set_order_count(1);
      Screen(s).set_refresh_pending(1);
      if (page == 0 || page == 1) {
        auto named = [&] {
          const uint64_t layer = Screen(s).layer_cursor();
          return page == 0 ? uint64_t(uint32_t(layer << 2)) + 202
                           : uint64_t(uint32_t((layer + 44) << 2));
        };
        tuning_screen::PlayNamed(call, frame, page == 0 ? 0x8220A39C : 0x8220A3E4, movie,
                                 page == 0 ? 213 : 189, 1);
        tuning_screen::FinishNamed(call, frame, page == 0 ? 0x8220A3B0 : 0x8220A3F8, movie,
                                   named());
        tuning_screen::PlayNamed(call, frame, 0x8220A41C, movie, named(), 0);
      }
    } else if (asked(40, 158, 0x8220A444)) {
      // B: back to the state popped, command 21 with 17 next.
      sound(2, 0x8220A454);
      Screen(s).set_refresh_pending(1);
      const uint32_t popped = Pop(s);
      GoFirstHalf(s, popped);
      const uint32_t queue = Screen(s).queue().address;
      const uint32_t state = Screen(s).state();
      const uint32_t current = CommandQueue(queue).page();
      if (int32_t(state) != int32_t(current)) {
        CommandQueue(queue).set_previous_page(current);
      }
      CommandQueue(queue).set_next(17);
      CommandQueue(queue).set_page(kNone);
      CommandQueue(queue).set_resume(state);
      CommandQueue(queue).set_current(21);
    }
    counting = G::order_count() != 0;
  }
  if (counting) {
    const uint32_t count = G::order_count() + 1;
    G::set_order_count(count);
    if (count == 10) {
      // The marks hidden, the three layers' nodes placed and their textures
      // and movie slots set in the new order.
      tuning_screen::ShowOptionMarks(s, 0, frame);
      const int32_t page = int32_t(Screen(s).editor_page());
      if (page == 0 || page == 1) {
        const uint32_t sub_scene = page == 0 ? 28 : 27;
        const uint32_t constants[3] = {0x82000A34, page == 0 ? 0x8200CAFCu : 0x82001E8Cu,
                                       page == 0 ? 0x8200CAF8u : 0x820081D8u};
        for (uint32_t k = 0; k < 3; k++) {
          const uint32_t node = ui_movies::FindNode(movie, sub_scene, 2 + k);
          CopyFloat(constants[k], node + 16);
        }
        Store(frame + 80, 13);
        const uint32_t first = page == 0 ? 0 : 21;
        const uint32_t i0 = Load(Screen(s).order_indices_address() + 4);
        const uint32_t i1 = Load(Screen(s).order_indices_address());
        const uint32_t i2 = Load(Screen(s).order_indices_address() + 8);
        const schema::DecalImageEntry a = DecalEntryAt(images + 36 * first, i1);
        const schema::DecalImageEntry b = DecalEntryAt(images + 36 * first, i0);
        const schema::DecalImageEntry c = DecalEntryAt(images + 36 * first, i2);
        const uint32_t a_header = a.texture_header();
        const uint32_t b_header = b.texture_header();
        const uint32_t c_header = c.texture_header();
        const uint32_t a_data = a.texture_data();
        const uint32_t b_data = b.texture_data();
        const uint32_t c_data = c.texture_data();
        const schema::DecalImageEntry to0 = DecalEntryAt(images, first);
        const schema::DecalImageEntry to1 = DecalEntryAt(images, first + 1);
        const schema::DecalImageEntry to2 = DecalEntryAt(images, first + 2);
        to0.set_texture_header(a_header);
        to0.set_texture_data(a_data);
        to1.set_texture_header(b_header);
        to1.set_texture_data(b_data);
        to2.set_texture_header(c_header);
        to2.set_texture_data(c_data);
        // Movie 13's textures 181, 183, 184 (page 0) or 175, 177, 178.
        static constexpr uint32_t kTextures[2][3] = {{181, 183, 184}, {175, 177, 178}};
        for (uint32_t k = 0; k < 3; k++) {
          const uint32_t again = tuning_screen::MovieEntry(frame, 13);
          const uint32_t header = DecalEntryAt(images, first + k).texture_header();
          MovieTexture(Load(again), kTextures[page][k]).set_texture(header);
        }
      }
    } else if (count == 13) {
      // The end: 237 restarted, the history popped down to state 24,
      // Go(24, 16), the layer the position moved to, the order stored.
      G::set_order_count(0);
      tuning_screen::FinishNamed(call, frame, 0x8220A904, movie, 237);
      tuning_screen::PlayNamed(call, frame, 0x8220A920, movie, 237, 1);
      uint32_t depth = uint32_t(Screen(s).history_depth());
      Screen(s).set_refresh_pending(1);
      uint32_t popped;
      do {
        depth = (depth + 255) & 0xFF;
        Screen(s).set_history_depth(uint8_t(depth));
        popped = Load(WordAt(s, depth + 2439));
        Screen(s).set_state(popped);
      } while (int32_t(popped) != 24);
      uint32_t queue = Screen(s).queue().address;
      CommandQueue(queue).set_resume(popped);
      CommandQueue(queue).set_page(popped);
      CommandQueue(queue).set_previous_page(popped);
      CommandQueue(queue).set_step(kNone);
      CommandQueue(queue).set_current(kNone);
      CommandQueue(queue).set_started(0);
      queue = Screen(s).queue().address;
      const uint32_t state = Screen(s).state();
      const uint32_t current = CommandQueue(queue).page();
      if (int32_t(state) != int32_t(current)) {
        CommandQueue(queue).set_previous_page(current);
      }
      CommandQueue(queue).set_next(kNone);
      CommandQueue(queue).set_resume(state);
      CommandQueue(queue).set_page(kNone);
      CommandQueue(queue).set_current(16);
      Screen(s).set_layer_cursor(Screen(s).order_position());
      EndLayerOrder(screen);
      Screen(s).set_layer_cursor(Screen(s).order_position());
    }
  }
  uint64_t r3 = ShowOrderNodes(screen, frame);
  if (G::order_count() == 0) {
    r3 = MoveLayerOrder(call, screen, frame, flags.native_slots);
  }
  return r3;
}

}  // namespace tuning_pages
