// The power graph screen's steps and Update (docs/functions/
// power-graph-screen.md): Step0 (the open animation, with SetTexts and the
// never-seen SetParts and its three helpers), Step1 (the close) and Update,
// which runs them from its table. UiMovies::Find and UiMovie::FindNode are
// their plain C++; SetText, Play and Run go through their overrides, from
// frames like the originals'.

#include "power_graph_screen/power_graph_screen.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

#include <bit>
#include <climits>
#include <cmath>

namespace power_graph_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// UiMovies::Find with the id stored at the frame's +80: movie 9.
uint32_t FindMovie(uint8_t* base, uint32_t frame) {
  StoreU32(base, frame + 80, kMovie);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

uint32_t Done(uint64_t run) { return uint32_t(run) == 0 ? 1 : 0; }

double Lfd(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>((uint64_t(LoadU32(base, address)) << 32) |
                               LoadU32(base, address + 4));
}

// fctiwz as the translated code does it: NaN 0x80000000, at or above
// INT_MAX INT_MAX, else truncated (0x80000000 below INT_MIN).
uint32_t Fctiwz(double d) {
  if (std::isnan(d)) {
    return 0x80000000u;
  }
  if (d >= double(INT_MAX)) {
    return uint32_t(INT_MAX);
  }
  if (!(d > -2147483649.0)) {
    return 0x80000000u;
  }
  return uint32_t(int32_t(d));
}

// The steps' table in Update's frame: Step0, Step1 (8-byte entries, the
// function and a this-adjust 0).
constexpr uint32_t kStep0 = 0x82251320;
constexpr uint32_t kStep1 = 0x82250AA8;

// The multipliers SetTexts' numbers and the float read (doubles).
constexpr uint32_t kScale10 = 0x8200A510;
constexpr uint32_t kScale11 = 0x82009F00;
constexpr uint32_t kScale12 = 0x8200A550;  // nodes 12 to 14 (kept in f31)
constexpr uint32_t kScale34 = 0x82009F08;

}  // namespace

uint64_t SetPartIcon(uint8_t* base, uint32_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82250B48 PowerGraphScreen67::SetPartIcon");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame);
  const uint32_t node = ui_movies::FindNode(base, movie, 3, 0);
  StoreU32(base, node + 108, value);
  return node;
}

uint64_t SetPartRow2(uint8_t* base, uint32_t values, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82250BA0 PowerGraphScreen67::SetPartRow2");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, frame);
  const uint32_t group = ui_movies::FindNode(base, movie, 3, 2);
  uint32_t r3 = group;
  // The children's count re-read each time (a child's stores could reach it).
  for (int32_t i = 0; i < int32_t(LoadU32(base, group + 104)); i++) {
    const uint32_t first = LoadU32(base, group + 100);
    const uint32_t value = LoadU32(base, values + 4 * uint32_t(i));
    r3 = ui_movies::FindNode(base, movie, 3, first + uint32_t(i));
    if (int32_t(value) != 10) {
      StoreU32(base, r3 + 108, value);
      StoreU32(base, r3 + 80, 1);
    } else {
      StoreU32(base, r3 + 80, 0);
    }
  }
  return r3;
}

uint64_t SetPartRow1(uint8_t* base, uint32_t values, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82250C40 PowerGraphScreen67::SetPartRow1");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, frame);
  const uint32_t group = ui_movies::FindNode(base, movie, 3, 1);
  uint32_t r3 = group;
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t first = LoadU32(base, group + 100);
    const uint32_t value = LoadU32(base, values + 4 * i);
    r3 = ui_movies::FindNode(base, movie, 3, i + first);
    if (int32_t(value) < 37) {
      StoreU32(base, r3 + 108, value);
      StoreU32(base, r3 + 80, 1);
    } else {
      StoreU32(base, r3 + 80, 0);
    }
  }
  return r3;
}

uint64_t SetParts(uint8_t* base, uint32_t parts, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82250CD0 PowerGraphScreen67::SetParts");
  const uint32_t frame = Frame(base, sp, 144);
  for (uint32_t i = 0; i < 3; i++) {
    StoreU32(base, frame + 80 + 4 * i, uint32_t(int32_t(int8_t(guest::LoadU8(base, parts + 2 + i)))));
  }
  for (uint32_t i = 0; i < 7; i++) {
    StoreU32(base, frame + 96 + 4 * i, uint32_t(int32_t(int8_t(guest::LoadU8(base, parts + 5 + i)))));
  }
  SetPartIcon(base, uint32_t(int32_t(int8_t(guest::LoadU8(base, parts + 1)))), frame);
  SetPartRow2(base, frame + 80, frame);
  return SetPartRow1(base, frame + 96, frame);
}

uint64_t SetTexts(uint8_t* base, uint64_t member, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250DD8 PowerGraphScreen67::SetTexts");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame);
  const uint64_t all = 0xFFFFFFFFFFFFFFFFull;  // the root (-1)
  auto text = [&](uint32_t lr, uint32_t node, uint64_t string, uint64_t r7 = 0,
                  uint64_t r10 = 0) {
    calls.CallArgs(kSetText, frame, lr, {movie, all, node, string, r7, 0, 0, r10});
  };
  // The labels and fixed strings (r7 to r10 unset: 0, the user's call of
  // 2026-10-30).
  struct Fixed {
    uint32_t lr, node, string;
  };
  static constexpr Fixed kFixed[] = {
      {0x82250E1C, 5, 258},  {0x82250E30, 6, 259},  {0x82250E44, 7, 260},
      {0x82250E58, 8, 261},  {0x82250E6C, 9, 262},  {0x82250E80, 23, 263},
      {0x82250E94, 24, 264}, {0x82250EA8, 25, 265}, {0x82250EBC, 26, 266},
      {0x82250ED0, 27, 267}, {0x82250EE4, 28, 268}, {0x82250EF8, 15, 446},
      {0x82250F0C, 16, 440}, {0x82250F20, 17, 447}, {0x82250F34, 18, 447},
      {0x82250F48, 19, 447}, {0x82250F5C, 35, 437}, {0x82250F70, 36, 439},
      {0x82250F84, 37, 449}, {0x82250F98, 38, 449}, {0x82250FAC, 42, 438},
      {0x82250FC0, 43, 448}, {0x82250FD4, 44, 448}};
  for (const Fixed& f : kFixed) {
    text(f.lr, f.node, f.string);
  }
  // Nodes 10 to 14: the data's word made a float, scaled, made an integer
  // through the frame's +80 (the word's sign extension stored, then the
  // result over its high word), string 8 with it. r10 = the frame's +80
  // where the original sets it (not for node 12).
  struct Number {
    uint32_t lr, node, field, scale;
    bool r10;
  };
  static constexpr Number kNumbers[] = {{0x82251024, 10, 8, kScale10, true},
                                        {0x82251070, 11, 12, kScale11, true},
                                        {0x822510BC, 12, 16, kScale12, false},
                                        {0x82251100, 13, 20, kScale12, true},
                                        {0x82251144, 14, 24, kScale12, true}};
  for (const Number& n : kNumbers) {
    const int32_t word = int32_t(LoadU32(base, LoadU32(base, kData) + n.field));
    StoreU32(base, frame + 80, word < 0 ? 0xFFFFFFFFu : 0);
    StoreU32(base, frame + 84, uint32_t(word));
    const double scaled = double(float(double(float(double(int64_t(word)))) * Lfd(base, n.scale)));
    StoreU32(base, frame + 80, Fctiwz(scaled));
    text(n.lr, n.node, 8, LoadU32(base, frame + 80), n.r10 ? frame + 80 : 0);
  }
  // The record's ids: strings from the maker, the car id, its spec, class
  // and flag.
  struct Id {
    uint32_t lr, node, field, offset;
  };
  static constexpr Id kIds[] = {{0x82251160, 20, 32, 1934}, {0x8225117C, 21, 28, 1646},
                                {0x82251198, 22, 28, 1790}, {0x822511B4, 29, 28, 1718},
                                {0x822511D0, 30, 36, 421},  {0x822511EC, 31, 40, 428},
                                {0x82251208, 32, 44, 426}};
  for (const Id& id : kIds) {
    text(id.lr, id.node, uint64_t(LoadU32(base, LoadU32(base, kData) + id.field)) + id.offset);
  }
  text(0x82251224, 33, 8, LoadU32(base, LoadU32(base, kData) + 52));
  {
    // Node 34: string 436 with the float +60 scaled, a double in r7 (and
    // f1), stored at the frame's +48.
    const float value = guest::LoadFloat(base, LoadU32(base, kData) + 60);
    const double f1 = double(float(double(value) * Lfd(base, kScale34)));
    const uint64_t bits = std::bit_cast<uint64_t>(f1);
    StoreU32(base, frame + 48, uint32_t(bits >> 32));
    StoreU32(base, frame + 52, uint32_t(bits));
    const uint64_t args[8] = {movie, all, 34, 436, bits, 0, 0, 0};
    calls.call(calls.user, kSetText, args, &f1, frame, 0x82251258);
  }
  text(0x82251274, 39, 8, LoadU32(base, LoadU32(base, kData) + 48));
  text(0x82251290, 40, 8, LoadU32(base, LoadU32(base, kData) + 56));
  text(0x822512AC, 41, 8, LoadU32(base, LoadU32(base, kData) + 64));
  // Node 3 (the parts) shown by the tab: 2 and 5 shown, 0, 1, 3 and 4
  // hidden, above 5 unchanged.
  const uint32_t node = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 3);
  const uint32_t tab = LoadU32(base, LoadU32(base, kData) + 88);
  if (tab <= 5) {
    StoreU32(base, node + 80, tab == 2 || tab == 5 ? 1 : 0);
  }
  if (LoadU32(base, node + 80) == 0) {
    return node;
  }
  (void)member;
  return SetParts(base, LoadU32(base, kData) + 68, frame);
}

uint64_t Step0(uint8_t* base, uint64_t member, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251320 PowerGraphScreen67::Step0");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame);
  if (LoadU32(base, started) == 0) {
    SetTexts(base, member, frame, calls);
    calls.CallArgs(kPlay, frame, 0x82251384, {movie, 0xFFFFFFFFFFFFFFFFull, 0, 0, 0, 0, 0, 0});
    StoreU32(base, started, 1);
  }
  const uint32_t done = Done(calls.Call(kRun, frame, 0x82251398, movie, 0));
  if (done != 0) {
    StoreU32(base, kShown, 1);
  }
  return done;
}

uint64_t Step1(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250AA8 PowerGraphScreen67::Step1");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame);
  if (LoadU32(base, started) == 0) {
    calls.CallArgs(kPlay, frame, 0x82250B08, {movie, 0xFFFFFFFFFFFFFFFFull, 1, 0, 0, 0, 0, 0});
    StoreU32(base, started, 1);
    StoreU32(base, kShown, 0);
  }
  return Done(calls.Call(kRun, frame, 0x82250B28, movie, 0));
}

uint64_t Update(uint8_t* base, uint64_t member, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822513B8 PowerGraphScreen67::Update");
  // The empty sub_82106540(100, 30, 0, 0x82011208) the original calls first
  // is left out: it returns at once.
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, frame);
  // The table: Step0 at +80, Step1 at +88 (and +96), each with its adjust 0.
  StoreU32(base, frame + 80, kStep0);
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 88, kStep1);
  StoreU32(base, frame + 92, 0);
  StoreU32(base, frame + 96, kStep1);
  StoreU32(base, frame + 100, 0);
  const uint32_t pages = LoadU32(base, kPages);
  const uint32_t step = LoadU32(base, pages + 12);
  uint64_t answer = movie;
  if (step == 0xFFFFFFFFu) {
    answer = calls.Call(kRun, frame, 0x822514D8, movie, 0);
  } else {
    bool done = true;
    if (int32_t(step) >= 0) {
      // No bounds check: the words at the table's index.
      const uint32_t adjust = LoadU32(base, frame + 84 + (step << 3));
      const uint32_t target = LoadU32(base, frame + 80 + (step << 3));
      answer = calls.Call(target, frame, 0x82251470, uint64_t(adjust) + member, pages + 24);
      done = int32_t(answer) != 0;
    }
    if (done) {
      const uint32_t p = LoadU32(base, kPages);
      const uint32_t queued = LoadU32(base, p + 16);
      if (queued == 0xFFFFFFFFu) {
        if (int32_t(step) >= 0) {
          StoreU32(base, p + 0, LoadU32(base, p + 4));
          StoreU32(base, p + 12, queued);
        } else {
          const uint32_t next = LoadU32(base, p + 4);
          StoreU32(base, p + 12, 0xFFFFFFFFu);
          StoreU32(base, p + 0, next);
        }
      } else {
        StoreU32(base, p + 16, 0xFFFFFFFFu);
        StoreU32(base, p + 20, 0);
        StoreU32(base, p + 12, queued);
      }
      StoreU32(base, p + 24, 0);
    }
  }
  const uint32_t state = LoadU32(base, LoadU32(base, kData) + 4);
  if (state != 21) {
    StoreU32(base, uint32_t(member) + kMachineState, state);
    StoreU32(base, LoadU32(base, kData) + 4, 21);
  }
  return answer;
}

}  // namespace power_graph_screen
