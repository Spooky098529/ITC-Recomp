// The tuning screen's movie calls (docs/functions/tuning-screen.md): see
// tuning_screen.h. The UI movie calls (UiMovie::FinishNamed, PlayNamed,
// SetText) through their overrides from each original's frame with its
// return addresses; SetText's unset varargs passed as 0 (the user's call,
// 2026-10-30).

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace tuning_screen {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

}  // namespace

uint32_t MovieEntry(uint8_t* base, uint32_t frame, uint32_t id) {
  Store(base, frame + 80, id);
  return ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
}

uint64_t FinishNamed(const GuestCall& call, uint32_t frame, uint32_t lr, uint32_t movie,
                     uint32_t named) {
  return call(kFinishNamed, {movie, named}, frame, lr);
}

uint64_t PlayNamed(const GuestCall& call, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint32_t named, uint32_t chained) {
  return call(kPlayNamed, {movie, named, chained, 0, 0, 0}, frame, lr);
}

uint64_t SetText(const GuestCall& call, uint32_t frame, uint32_t lr, uint32_t movie,
                 uint32_t sub_scene, uint32_t child, uint32_t id, uint64_t argument) {
  return call(kSetText, {movie, sub_scene, child, id, argument, 0, 0, 0}, frame, lr);
}

void Advance(uint8_t* base, uint32_t screen) {
  const uint32_t queue = Load(base, screen + 10036);
  const uint32_t next = Load(base, queue + 16);
  if (int32_t(next) == -1) {
    const uint32_t first = Load(base, queue + 4);
    Store(base, queue + 12, 0xFFFFFFFFu);
    Store(base, queue + 0, first);
  } else {
    Store(base, queue + 20, 0);
    Store(base, queue + 12, next);
    Store(base, queue + 16, 0xFFFFFFFFu);
  }
  Store(base, queue + 24, 0);
}

uint64_t UpdateListCounter(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EED00 TuningScreen::UpdateListCounter");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  SetText(call, frame, 0x821EED50, movie, 1, 23, 8, uint64_t(Load(base, screen + 25220)) + 1);
  SetText(call, frame, 0x821EED64, movie, 1, 24, 449, 0);
  return SetText(call, frame, 0x821EED80, movie, 1, 25, 8,
                 uint64_t(Load(base, screen + 25224)) + 1);
}

uint64_t SetPartName(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F2068 TuningScreen::SetPartName");
  const uint32_t frame = Frame(base, sp, 848);
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  const uint32_t table = frame + 96;
  Store(base, table, 376);
  for (uint32_t i = 1; i < 180; i++) {
    Store(base, table + 4 * i, 999 + i);
  }
  const uint64_t number =
      WheelNumber(Load(base, screen + 25204), Load(base, screen + 25220));
  const uint32_t id = Load(base, uint32_t(number << 2) + table);
  return SetText(call, frame, 0x821F20F4, movie, 1, 20, id, 0);
}

uint64_t PlayGroupAnimation(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EE880 TuningScreen::PlayGroupAnimation");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = MovieEntry(base, frame, kMovieId);
  const uint32_t movie = Load(base, entry);
  uint64_t r3 = entry;
  const uint32_t group = Load(base, screen + 25204);
  if (group <= 16) {
    uint32_t named = 68;
    uint32_t lr = 0x821EE930;
    switch (group) {
      case 0: case 7: case 13: named = 65; lr = 0x821EE8F4; break;
      case 8: case 14: named = 66; lr = 0x821EE908; break;
      case 11: named = 67; lr = 0x821EE91C; break;
      default: break;
    }
    FinishNamed(call, frame, lr, movie, named);
    r3 = PlayNamed(call, frame, 0x821EE94C, movie, named, 1);
  }
  if (int32_t(Load(base, screen + 25208)) == 11) {
    const int32_t g = int32_t(Load(base, screen + 25204));
    if (g == 10 || g == 12) {
      FinishNamed(call, frame, 0x821EE978, movie, 71);
      r3 = PlayNamed(call, frame, 0x821EE994, movie, 71, 1);
    }
  }
  return r3;
}

uint64_t Command31(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82202880 TuningScreen::Command31");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = MovieEntry(base, frame, 13);
  const int32_t state = int32_t(Load(base, screen + kState));
  const uint32_t movie = Load(base, entry);
  Store(base, screen + 9720, 1);
  if (state == 23) {
    FinishNamed(call, frame, 0x822028D4, movie, 220);
    return PlayNamed(call, frame, 0x8220290C, movie, 220, 1);
  }
  if (state == 24) {
    FinishNamed(call, frame, 0x822028F0, movie, 222);
    return PlayNamed(call, frame, 0x8220290C, movie, 222, 1);
  }
  return entry;
}

uint64_t Command9(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E9050 TuningScreen::Command9");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = MovieEntry(base, frame, kMovieId);
  uint64_t r3 = entry;
  const uint32_t movie = Load(base, entry);
  if (Load(base, screen + 9716) != 0) {
    FinishNamed(call, frame, 0x821E9098, movie, 61);
    r3 = PlayNamed(call, frame, 0x821E90B4, movie, 61, 1);
    if (Load(base, screen + 9720) != 0) {
      Store(base, screen + 9720, 0);
    }
    Store(base, screen + 9716, 0);
  }
  if (Load(base, screen + 9724) != 0) {
    Advance(base, screen);
  }
  return r3;
}

}  // namespace tuning_screen
