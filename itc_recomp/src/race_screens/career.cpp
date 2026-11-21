// The career's results' pages and transitions
// (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/race_screens.h"
#include "ui_movies/game.h"

#include <bit>

namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);

using G = schema::RaceScreensGlobals;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The screen and its movie (+180), read again at each use.
schema::CareerResultsScreen Screen(uint8_t* base) { return G::career_results(base); }
uint32_t Movie(uint8_t* base) { return Screen(base).movie().address; }

// UiMovie::Play(movie, -1, index, 0, 0, 0, 0) through its override.
uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t index) {
  const uint64_t args[8] = {movie, kNone, index, 0, 0, 0, 0, 0};
  return calls.CallArgs(kPlay, frame, lr, args);
}

uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

}  // namespace

uint64_t CareerResultsIntro(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82219210 CareerResults::Intro");
  const uint32_t frame = Frame(base, sp, 96);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    const uint32_t kind = Screen(base).kind();
    if (kind < 3) {
      Play(calls, frame, 0x82219284, Movie(base), kind);
    }
  }
  return uint32_t(Run(calls, frame, 0x82219294, Movie(base))) == 0 ? 1 : 0;
}

uint64_t CareerResultsShow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822192B0 CareerResults::Show");
  const uint32_t frame = Frame(base, sp, 96);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    Play(calls, frame, 0x822192FC, Movie(base), 3);
    calls.Call(kLoop, frame, 0x82219310, Movie(base), kNone, 3);
    const uint32_t kind = Screen(base).kind();
    if (kind < 3) {
      const uint32_t lrs[3] = {0x8221939C, 0x82219374, 0x8221934C};
      Play(calls, frame, lrs[kind], Movie(base), 11 + kind);
      calls.Call(kLoop, frame, 0x822193B0, Movie(base), kNone, 11 + kind);
    }
  }
  Run(calls, frame, 0x822193C0, Movie(base));
  return 1;
}

uint64_t CareerResultsClose1(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822193D8 CareerResults::Close1");
  const uint32_t frame = Frame(base, sp, 96);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    calls.Call(kFinish, frame, 0x82219414, Movie(base), kNone, 3);
    const uint32_t kind = Screen(base).kind();
    if (kind < 3) {
      calls.Call(kFinish, frame, 0x82219450, Movie(base), kNone, 11 + kind);
    }
    Play(calls, frame, 0x82219474, Movie(base), 4);
  }
  return uint32_t(Run(calls, frame, 0x82219484, Movie(base))) == 0 ? 1 : 0;
}

uint64_t CareerResultsPage1(uint8_t* base, uint32_t sp, bool native_slots, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82219A78 CareerResults::Page1");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = guest::LoadU32(base, kControllerEntries) * 8 + kMapperEntries;
  const schema::CareerResultsScreen screen = Screen(base);
  // Accept, then back: each 0 when the entry's +4 is 1, else the slot's low
  // byte.
  auto pressed = [&](uint32_t action, uint32_t lr) {
    if (int32_t(guest::LoadU32(base, entry + 4)) == 1) {
      return false;
    }
    return (MapperPressed(base, guest::LoadU32(base, entry), action, frame, lr, native_slots,
                          calls) &
            0xFF) != 0;
  };
  if (!pressed(159, 0x82219AC8) && !pressed(158, 0x82219AF4)) {
    return 0;
  }
  calls.Call(kMenuSoundPlay, frame, 0x82219B04, 0);
  uint64_t page = 4, transition = 6;
  if (int32_t(screen.item_bonus()) > 0) {
    page = 2;
    transition = 2;
  } else if (double(screen.entry_value()) >
             double(guest::LoadFloat(base, race_hud_object::kZero))) {
    page = 3;
    transition = 3;
  }
  calls.Call(kPagesGo, frame, 0x82219B54, Screen(base).pages_page_address(), page, transition, kNone);
  return 0;
}

namespace {

// UiMovie::SetText(movie, sub_scene, child, id, r7) through its override,
// its unset varargs (r8 to r10, and r7 where `value` isn't given) 0 (the
// user's call, 2026-10-30); f1 passed where given.
uint64_t SetText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                 uint64_t sub_scene, uint64_t child, uint64_t id, uint64_t value = 0,
                 const double* f1 = nullptr) {
  const uint64_t args[8] = {movie, sub_scene, child, id, value, 0, 0, 0};
  return calls.call(calls.user, kSetText, args, f1, frame, lr);
}

uint64_t SetNumber(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint64_t sub_scene, uint64_t child, uint64_t value) {
  return calls.Call(kSetNumberText, frame, lr, movie, sub_scene, child, value);
}

// A word of the screen, zero-extended (the originals' lwz).
uint64_t Word(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }

// A sum kept as the original's 64-bit add of zero-extended words, 0 where
// its low word is negative (cmpwi).
uint64_t AtLeastZero(uint64_t sum) { return int32_t(uint32_t(sum)) < 0 ? 0 : sum; }

}  // namespace

uint64_t CareerResultsSetTexts(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls) {
  const schema::CareerResultsScreen results(base, screen);
  ITC_NATIVE_RUNNING("sub_822197A8 CareerResults::SetTexts");
  using namespace camera_modes::steps;
  const uint32_t frame = Frame(base, sp, 128);
  auto movie = [&] { return results.movie().address; };
  const uint64_t earned = AtLeastZero(uint64_t(results.prize()) + uint64_t(results.money()));
  // A node's children (+100 on, +104 of them, read again each step) set to
  // string `id` (`id` + i where `step`).
  auto children = [&](uint64_t sub_scene, uint32_t index, uint64_t id, bool step, uint32_t lr) {
    const uint32_t node = ui_movies::FindNode(base, movie(), uint32_t(sub_scene), index);
    for (int64_t i = 0; i < int32_t(guest::LoadU32(base, node + 104)); i++) {
      SetText(calls, frame, lr, movie(), sub_scene, uint64_t(i) + Word(base, node + 100),
              step ? uint64_t(i) + id : id);
    }
  };
  children(7, 0, 1969, true, 0x82219808);
  children(7, 2, 2051, false, 0x82219854);
  SetNumber(calls, frame, 0x82219878, movie(), 7, 8, uint64_t(results.opponent_prize()));
  SetNumber(calls, frame, 0x8221988C, movie(), 7, 9, uint64_t(results.win_prize()));
  {
    // The float +12 as a double: f1, and r7 through the frame's +48.
    const double f1 = Lfs(base, results.streak_factor_address());
    const uint64_t bits = std::bit_cast<uint64_t>(f1);
    guest::StoreU32(base, frame + 48, uint32_t(bits >> 32));
    guest::StoreU32(base, frame + 52, uint32_t(bits));
    SetText(calls, frame, 0x822198AC, movie(), 7, 10, 1983, bits, &f1);
  }
  SetNumber(calls, frame, 0x822198C0, movie(), 7, 11, uint64_t(results.prize()));
  SetNumber(calls, frame, 0x822198D4, movie(), 7, 12, earned);
  const uint64_t with_bonus = AtLeastZero(uint64_t(results.prize()) + uint64_t(results.money()) +
                                          uint64_t(results.item_bonus()));
  SetText(calls, frame, 0x82219904, movie(), 8, 5, 1973);
  children(8, 3, 2051, false, 0x82219940);
  SetNumber(calls, frame, 0x82219964, movie(), 8, 6, uint64_t(results.item_bonus()));
  SetNumber(calls, frame, 0x82219978, movie(), 8, 7, with_bonus);
  const uint64_t total =
      AtLeastZero(uint64_t(results.prize()) + uint64_t(results.money()) + uint64_t(results.entry_points()) +
                  uint64_t(results.item_bonus()));
  SetText(calls, frame, 0x822199B0, movie(), 9, 10, 1974);
  SetText(calls, frame, 0x822199C4, movie(), 9, 11, 1975);
  SetText(calls, frame, 0x822199D8, movie(), 9, 12, 1973);
  SetText(calls, frame, 0x822199EC, movie(), 9, 4, 1982);
  SetText(calls, frame, 0x82219A00, movie(), 9, 5, 2051);
  SetText(calls, frame, 0x82219A14, movie(), 9, 6, 2051);
  {
    // +24 times the double at 0x8200A510 (fmul), rounded to single (frsp).
    const double scale = std::bit_cast<double>((Word(base, 0x8200A510) << 32) |
                                               Word(base, 0x8200A514));
    const double f1 = Single(Lfs(base, results.entry_value_address()) * scale);
    const uint64_t bits = std::bit_cast<uint64_t>(f1);
    guest::StoreU32(base, frame + 48, uint32_t(bits >> 32));
    guest::StoreU32(base, frame + 52, uint32_t(bits));
    SetText(calls, frame, 0x82219A44, movie(), 9, 7, 2052, bits, &f1);
  }
  SetNumber(calls, frame, 0x82219A58, movie(), 9, 8, uint64_t(results.entry_points()));
  return SetNumber(calls, frame, 0x82219A6C, movie(), 9, 9, total);
}

uint64_t CareerResultsOpen(uint8_t* base, uint32_t screen, uint32_t block, uint32_t sp,
                           const Calls& calls) {
  const schema::CareerResultsScreen results(base, screen);
  ITC_NATIVE_RUNNING("sub_8221A1F0 CareerResults::Open");
  const uint32_t frame = Frame(base, sp, 112);
  if (!c_runtime::Memcpy(base, screen, block, 168)) {
    calls.Call(kMemcpy, frame, 0x8221A210, screen, block, 168);
  }
  guest::StoreU32(base, frame + 80, 6);
  const uint32_t movie =
      guest::LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  results.set_movie(movie);
  calls.Call(kResetPose, frame, 0x8221A234, movie);
  PagesInit(base, results.pages_page_address(), 1);
  calls.Call(kPagesGo, frame, 0x8221A258, results.pages_page_address(), 0, 0, kNone);
  CareerResultsSetTexts(base, screen, frame, calls);
  const uint32_t total =
      results.money() + results.entry_points() + results.item_bonus() + results.prize();
  results.set_total(total);
  if (int32_t(total) < 0) {
    results.set_total(0);
  }
  results.set_entry(0);
  const uint32_t node = ui_movies::FindNode(base, Movie(base), 0xFFFFFFFFu, 11);
  const uint32_t kind = Screen(base).kind();
  if (kind < 3) {
    guest::StoreU32(base, node + 108, kind);
  }
  return node;
}

}  // namespace race_screens
