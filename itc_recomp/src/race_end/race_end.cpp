// The race's end: see race_end.h and docs/functions/race-end.md.

#include "race_end/race_end.h"

#include "base/log.h"
#include "career/career.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "network_mode/network_mode.h"
#include "schema/boot_mode.h"
#include "schema/game_modes.h"
#include "schema/race_end.h"
#include "schema/race_event.h"
#include "schema/race_states.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_end {

namespace {

using G = schema::RaceEndGlobals;

// The race data ([0x826C5B2C], race-states.toml's RaceData).
schema::RaceData Race() { return schema::RaceData(G::race_data().address); }

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// The original's frame below `sp`: its back chain.
uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(frame, sp);
  return frame;
}

// An answer asking for mode `type`: built at the frame's +80 (word +84 the
// type, byte +80 1; +81 to +83 left as the stack has them), then its 8
// bytes stored at `answer` (one doubleword in the original).
void Answer(uint32_t answer, uint32_t frame, uint32_t type) {
  const schema::ModeAnswer built(frame + 80), out(answer);
  built.set_next_type(type);
  built.set_changed(1);
  const uint32_t w0 = built.word0(), w1 = built.next_type();
  out.set_word0(w0);
  out.set_next_type(w1);
}

// The race data's flags word.
uint32_t FlagsAt() { return Race().flags_address(); }

// The flag `bit` cleared (the word loaded again, as the original does).
void ClearFlag(uint32_t at, uint32_t bit) { Store(at, Load(at) & ~bit); }

// ScreenFades::FadeIn([0x82001BEC] passes, black, layer 0).
void FadeIn() {
  mode_tick::ScreenFadesFadeIn(G::fade_in_passes(), 0, 0, 0, 0);
}

// Career::EventLocation(save) from the frame (its plain C++; the condition
// tests' depth below the adapter's stack pointer).
void EventLocation(uint32_t save, const Calls& calls) {
  career::EventLocation(save, calls.event.Down(kModeAfterRaceFrame));
}

// The flags' answer with unlocks to show (stages 2 to 8, but stage 6's and
// 8's without them): 0x8 → 9 (cleared), else 0x10 → cleared,
// Career::EventLocation, 5; else nothing asked.
void UnlocksAnswer(uint32_t answer, uint32_t frame, uint32_t save,
                   const Calls& calls) {
  const uint32_t at = FlagsAt();
  const uint32_t flags = Load(at);
  if (flags & 0x8) {
    Answer(answer, frame, 9);
    ClearFlag(at, 0x8);
  } else if (flags & 0x10) {
    ClearFlag(at, 0x10);
    EventLocation(save, calls);
    Answer(answer, frame, 5);
  }
}

// Stages 6 and 8 without unlocks to show: the boot movies' table {first,
// second, 11, 11, 11} then the career's result (9), skippable, from the
// first; the boot mode (3), the fade-in and Progress::CheckUnlocks.
void MoviesAnswer(uint32_t answer, uint32_t frame, uint32_t save,
                  uint32_t first, uint32_t second, const Calls& calls) {
  for (uint32_t k = 0; k < 5; k++) {
    Store(G::boot_movie_ids_address() + 4 * k, 11);
  }
  G::set_boot_movies_index(0);
  G::set_boot_movies_skippable(1);
  Store(G::boot_movie_ids_address() + 0, first);
  Store(G::boot_movie_ids_address() + 4, second);
  const schema::ModeAnswer built(frame + 80), out(answer);
  built.set_next_type(3);
  G::set_boot_movies_next_mode(9);
  built.set_changed(1);
  const uint32_t w0 = built.word0(), w1 = built.next_type();
  out.set_word0(w0);
  out.set_next_type(w1);
  FadeIn();
  if (career::CheckUnlocksNative(frame)) {
    career::ProgressCheckUnlocks(save, frame);
  } else {
    calls.guest.Call(kCheckUnlocks, frame, 0x82151024, save);
  }
}

}  // namespace

void ModeAfterRace(uint32_t answer, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82150E00 RaceMode::ModeAfterRace");
  const uint32_t frame = Frame(sp, kModeAfterRaceFrame);
  const schema::RaceData race = Race();
  const schema::ModeAnswer out(answer);
  out.set_next_type(0);
  const uint32_t flags = race.flags();
  out.set_changed(0);
  const auto title = [&] {
    // The title (4), the title's flag set.
    const schema::ModeAnswer built(frame + 80);
    built.set_next_type(4);
    built.set_changed(1);
    const uint32_t w0 = built.word0(), w1 = built.next_type();
    G::set_title_flag(1);
    out.set_word0(w0);
    out.set_next_type(w1);
  };
  if (flags & 0x8000) {
    title();
    return;
  }
  if (flags & 0x10000) {
    network_mode::SessionFree(frame, calls.guest);
    Answer(answer, frame, 12);
    return;
  }
  const int32_t kind = int32_t(race.kind());
  switch (kind) {
    case 0:
      FadeIn();
      title();
      return;
    case 2:
      Answer(answer, frame, 6);
      return;
    case 3:
      Answer(answer, frame, 7);
      return;
    case 4:
      Answer(answer, frame, 8);
      return;
    case 5: {
      const bool network = int32_t(race.race_mode()) == 7 && G::session().address != 0;
      Answer(answer, frame, network ? 12 : 10);
      return;
    }
    case 6:
      Answer(answer, frame, 12);
      return;
    default:
      break;
  }
  // The career (kind 1, or any other).
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  const schema::SaveData save_data(save);
  const int32_t race_mode = int32_t(Race().race_mode());
  if (race_mode == 0 || race_mode == 1) {
    Answer(answer, frame, 2);
    return;
  }
  const int32_t stage =
      career::AfterRaceNative(frame)
          ? int32_t(career::ProgressAfterRace(save, frame))
          : int32_t(uint32_t(calls.guest.Call(kAfterRace, frame, 0x82150EB8, save)));
  if (stage == 6) {
    if (save_data.unlocks_shown_count() != 0) {
      UnlocksAnswer(answer, frame, save, calls);
      save_data.set_movie_after_results(6);
    } else {
      MoviesAnswer(answer, frame, save, 6, 7, calls);
    }
    return;
  }
  if (stage == 8) {
    MoviesAnswer(answer, frame, save, 8, 9, calls);
    return;
  }
  if (stage != -1) {
    save_data.set_movie_after_results(uint32_t(stage));
    if (save_data.unlocks_shown_count() != 0) {
      UnlocksAnswer(answer, frame, save, calls);
    } else {
      Answer(answer, frame, 9);
    }
    return;
  }
  // Nothing reached.
  const uint32_t at = FlagsAt();
  const uint32_t now = Load(at);
  if (now & 0x8) {
    Store(at, now & ~0x8u);
    uint32_t type = 9;
    if (int32_t(Race().kind()) == 1) {
      const int32_t day = int32_t(schema::SaveData(vehicle_accessors::SaveDataOf(0)).day());
      if (day <= 999 && day == 1) {
        type = 2;
      }
    }
    Answer(answer, frame, type);
  } else if (now & 0x10) {
    ClearFlag(at, 0x10);
    EventLocation(save, calls);
    Answer(answer, frame, 5);
  } else {
    Answer(answer, frame, 9);
  }
}

void NextMode(uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82151EF0 RaceMode::NextMode");
  const uint32_t frame = Frame(sp, kNextModeFrame);
  const schema::ModeAnswer out(answer);
  out.set_next_type(0);
  out.set_changed(0);
  // The messages, as the original inlines GameMessages::Post: the count + 1
  // stored, then the message and two zero words at (start + count - 1) mod
  // 50 (unsigned).
  const schema::RaceMode race_mode(mode);
  const auto post = [&](uint32_t message) {
    const uint32_t count = race_mode.ring_count() + 1;
    const uint32_t at = race_mode.ring_start() + count - 1;
    race_mode.set_ring_count(count);
    const schema::ModeMessage slot(race_mode.ring_address() + (at % 50) * 12);
    slot.set_id(message);
    slot.set_arg0(0);
    slot.set_arg1(0);
  };
  if (race_mode.running() != 0) {
    post(0);
  }
  post(1);
  const int32_t state = int32_t(race_mode.course_setup_state());
  if (state == 3 || state == 4) {
    race_loading::Run(~0ull, frame, 0x82151FD4, calls.loading);
  }
  race_loading::CheckSystemRequest(frame, calls.loading);
  calls.guest.Call(kDispatch, frame, 0x82151FE0, race_mode.handlers_address());
  if (int32_t(race_mode.course_setup_state()) != 35) {
    return;
  }
  Calls inner = calls;
  inner.event = calls.event.Down(kNextModeFrame);
  const schema::ModeAnswer temporary(frame + 80);
  ModeAfterRace(temporary.address, frame, inner);
  const uint32_t w0 = temporary.word0(), w1 = temporary.next_type();
  out.set_word0(w0);
  out.set_next_type(w1);
}

}  // namespace race_end
