// The power graph screen's steps and Update (docs/functions/
// power-graph-screen.md): Step0 (the open animation, with SetTexts and the
// never-seen SetParts and its three helpers), Step1 (the close) and Update,
// which runs them from its table. UiMovies::Find and UiMovie::FindNode are
// their plain C++; SetText, Play and Run go through their overrides. Only
// SetTexts keeps a frame like the original's (SetText's varargs); the
// others' frames held only values and tables the port reads (host values
// since E5 step 6), their calls made from their own `sp`.

#include "power_graph_screen/power_graph_screen.h"

#include "base/log.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/power_graph_screen.h"
#include "schema/tuning_screen.h"
#include "ui_movies/game.h"

#include <bit>
#include <climits>
#include <cmath>
#include <iterator>

namespace power_graph_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::PowerGraphScreenGlobals;
using Data = schema::PowerGraphScreenData;
using Member = schema::PowerGraphScreen67;
using Node = schema::UiMovieNode;  // tuning-screen.toml

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(frame, sp);
  return frame;
}

// UiMovies::Find with the id stored at the frame's +80: movie 9 (SetTexts,
// whose frame stays).
uint32_t FindMovie(uint32_t frame) {
  StoreU32(frame + 80, kMovie);
  return LoadU32(ui_movies::PairsFind(ui_movies::kMovieTable, frame + 80));
}

// UiMovies::Find on movie 9 by value (the original's id at its frame's +80,
// a value since E5 step 6, for the functions whose frames are left out).
uint32_t Movie() {
  return LoadU32(ui_movies::PairsFindKey(ui_movies::kMovieTable, kMovie));
}

uint32_t Done(uint64_t run) { return uint32_t(run) == 0 ? 1 : 0; }

double Lfd(uint32_t address) {
  return std::bit_cast<double>((uint64_t(LoadU32(address)) << 32) |
                               LoadU32(address + 4));
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

// The steps' table the original builds in Update's frame (+80): Step0,
// Step1, Step1 (8-byte entries, the function and a this-adjust 0); a host
// table since E5 step 6.
constexpr uint32_t kStep0 = 0x82251320;
constexpr uint32_t kStep1 = 0x82250AA8;
struct Step {
  uint32_t function, adjust;
};
constexpr Step kSteps[3] = {{kStep0, 0}, {kStep1, 0}, {kStep1, 0}};
constexpr uint32_t kUpdateFrame = 144;

// SetPartRow2's and SetPartRow1's work over `value(i)`, the array's words.
template <typename Value>
uint32_t PartRow2(Value value) {
  const uint32_t movie = Movie();
  const uint32_t group = ui_movies::FindNode(movie, 3, 2);
  uint32_t r3 = group;
  // The children's count re-read each time (a child's stores could reach it).
  // A group's first child index (+100) and child count (+104), read as words.
  for (int32_t i = 0; i < int32_t(LoadU32(Node(group).height_address())); i++) {
    const uint32_t first = LoadU32(Node(group).width_address());
    const uint32_t v = value(uint32_t(i));
    r3 = ui_movies::FindNode(movie, 3, first + uint32_t(i));
    if (int32_t(v) != 10) {
      Node(r3).set_texture_entry(v);
      Node(r3).set_visible(1);
    } else {
      Node(r3).set_visible(0);
    }
  }
  return r3;
}

template <typename Value>
uint32_t PartRow1(Value value) {
  const uint32_t movie = Movie();
  const uint32_t group = ui_movies::FindNode(movie, 3, 1);
  uint32_t r3 = group;
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t first = LoadU32(Node(group).width_address());
    const uint32_t v = value(i);
    r3 = ui_movies::FindNode(movie, 3, i + first);
    if (int32_t(v) < 37) {
      Node(r3).set_texture_entry(v);
      Node(r3).set_visible(1);
    } else {
      Node(r3).set_visible(0);
    }
  }
  return r3;
}

// The multipliers SetTexts' numbers and the float read (doubles).
constexpr uint32_t kScale10 = 0x8200A510;
constexpr uint32_t kScale11 = 0x82009F00;
constexpr uint32_t kScale12 = 0x8200A550;  // nodes 12 to 14 (kept in f31)
constexpr uint32_t kScale34 = 0x82009F08;

}  // namespace

uint64_t SetPartIcon(uint32_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82250B48 PowerGraphScreen67::SetPartIcon");
  // The original's 112-byte frame held only the movie id (E5 step 6: a value).
  (void)sp;
  const uint32_t movie = Movie();
  const uint32_t node = ui_movies::FindNode(movie, 3, 0);
  Node(node).set_texture_entry(value);
  return node;
}

uint64_t SetPartRow2(uint32_t values, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82250BA0 PowerGraphScreen67::SetPartRow2");
  // The original's 144-byte frame held only the movie id (E5 step 6: a value).
  (void)sp;
  return PartRow2([&](uint32_t i) { return LoadU32(values + 4 * i); });
}

uint64_t SetPartRow1(uint32_t values, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82250C40 PowerGraphScreen67::SetPartRow1");
  // The original's 144-byte frame held only the movie id (E5 step 6: a value).
  (void)sp;
  return PartRow1([&](uint32_t i) { return LoadU32(values + 4 * i); });
}

uint64_t SetParts(const uint8_t* parts, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82250CD0 PowerGraphScreen67::SetParts");
  // The two arrays the original builds in its 144-byte frame (+80 and +96)
  // for SetPartRow2 and SetPartRow1: host arrays since E5 step 6 (the rows'
  // plain C++ reads them). Row 2 reads as many words as its group has
  // children (probably 3): past its 3, the original reads on into its
  // frame (the gap at +92, row 1's words, then words it never writes),
  // here the gap and the words never written as 0.
  (void)sp;
  uint32_t words[11] = {};  // +80 to +120
  for (uint32_t i = 0; i < 3; i++) {
    words[i] = uint32_t(int32_t(int8_t(parts[2 + i])));
  }
  // +92 is a gap in the original's frame (not written): 0 here.
  for (uint32_t i = 0; i < 7; i++) {
    words[4 + i] = uint32_t(int32_t(int8_t(parts[5 + i])));
  }
  SetPartIcon(uint32_t(int32_t(int8_t(parts[1]))), sp);
  ITC_NATIVE_RUNNING("sub_82250BA0 PowerGraphScreen67::SetPartRow2");
  PartRow2([&](uint32_t i) { return i < std::size(words) ? words[i] : 0u; });
  ITC_NATIVE_RUNNING("sub_82250C40 PowerGraphScreen67::SetPartRow1");
  return PartRow1([&](uint32_t i) { return words[4 + i]; });
}

uint64_t SetTexts(uint64_t member, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250DD8 PowerGraphScreen67::SetTexts");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(frame);
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
    uint32_t lr, node;
    uint32_t (Data::*field)() const;
    uint32_t scale;
    bool r10;
  };
  static constexpr Number kNumbers[] = {{0x82251024, 10, &Data::car_value, kScale10, true},
                                        {0x82251070, 11, &Data::rating, kScale11, true},
                                        {0x822510BC, 12, &Data::spec_word_0, kScale12, false},
                                        {0x82251100, 13, &Data::spec_word_1, kScale12, true},
                                        {0x82251144, 14, &Data::spec_word_2, kScale12, true}};
  for (const Number& n : kNumbers) {
    const int32_t word = int32_t((G::data().*n.field)());
    StoreU32(frame + 80, word < 0 ? 0xFFFFFFFFu : 0);
    StoreU32(frame + 84, uint32_t(word));
    const double scaled = double(float(double(float(double(int64_t(word)))) * Lfd(n.scale)));
    StoreU32(frame + 80, Fctiwz(scaled));
    text(n.lr, n.node, 8, LoadU32(frame + 80), n.r10 ? frame + 80 : 0);
  }
  // The record's ids: strings from the maker, the car id, its spec, class
  // and flag.
  struct Id {
    uint32_t lr, node;
    uint32_t (Data::*field)() const;
    uint32_t offset;
  };
  static constexpr Id kIds[] = {{0x82251160, 20, &Data::spec_byte_0, 1934},
                                {0x8225117C, 21, &Data::car_id, 1646},
                                {0x82251198, 22, &Data::car_id, 1790},
                                {0x822511B4, 29, &Data::car_id, 1718},
                                {0x822511D0, 30, &Data::spec_byte_1, 421},
                                {0x822511EC, 31, &Data::engine_class, 428},
                                {0x82251208, 32, &Data::spec_flag, 426}};
  for (const Id& id : kIds) {
    text(id.lr, id.node, uint64_t((G::data().*id.field)()) + id.offset);
  }
  text(0x82251224, 33, 8, G::data().curve_value_0());
  {
    // Node 34: string 436 with the float +60 scaled, a double in r7 (and
    // f1), stored at the frame's +48.
    const float value = G::data().peak_torque();
    const double f1 = double(float(double(value) * Lfd(kScale34)));
    const uint64_t bits = std::bit_cast<uint64_t>(f1);
    StoreU32(frame + 48, uint32_t(bits >> 32));
    StoreU32(frame + 52, uint32_t(bits));
    const uint64_t args[8] = {movie, all, 34, 436, bits, 0, 0, 0};
    calls.call(calls.user, kSetText, args, &f1, frame, 0x82251258);
  }
  text(0x82251274, 39, 8, G::data().spec_word_3());
  text(0x82251290, 40, 8, G::data().curve_value_1());
  text(0x822512AC, 41, 8, G::data().peak_index());
  // Node 3 (the parts) shown by the tab: 2 and 5 shown, 0, 1, 3 and 4
  // hidden, above 5 unchanged.
  const uint32_t node = ui_movies::FindNode(movie, 0xFFFFFFFFu, 3);
  const uint32_t tab = G::data().tab();
  if (tab <= 5) {
    Node(node).set_visible(tab == 2 || tab == 5 ? 1 : 0);
  }
  if (Node(node).visible() == 0) {
    return node;
  }
  (void)member;
  // The parts' five words through the view (the data is a host object since
  // E5 step 4), their bytes in guest order (most significant first).
  const Data data = G::data();
  const uint32_t words[5] = {data.parts_word_0(), data.parts_word_1(), data.parts_word_2(),
                             data.parts_word_3(), data.parts_word_4()};
  uint8_t parts[20];
  for (uint32_t i = 0; i < 20; i++) {
    parts[i] = uint8_t(words[i / 4] >> (24 - 8 * (i % 4)));
  }
  return SetParts(parts, frame);
}

uint64_t Step0(uint64_t member, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251320 PowerGraphScreen67::Step0");
  // The original's 128-byte frame held only the movie id (a value since E5
  // step 6): its calls run from `sp` (none reads its caller's frame).
  const uint32_t movie = Movie();
  const schema::PowerGraphStepStarted s(started);
  if (s.started() == 0) {
    SetTexts(member, sp, calls);
    calls.CallArgs(kPlay, sp, 0x82251384, {movie, 0xFFFFFFFFFFFFFFFFull, 0, 0, 0, 0, 0, 0});
    s.set_started(1);
  }
  const uint32_t done = Done(calls.Call(kRun, sp, 0x82251398, movie, 0));
  if (done != 0) {
    G::set_shown(1);
  }
  return done;
}

uint64_t Step1(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250AA8 PowerGraphScreen67::Step1");
  // The original's 112-byte frame held only the movie id (a value since E5
  // step 6): its calls run from `sp` (none reads its caller's frame).
  const uint32_t movie = Movie();
  const schema::PowerGraphStepStarted s(started);
  if (s.started() == 0) {
    calls.CallArgs(kPlay, sp, 0x82250B08, {movie, 0xFFFFFFFFFFFFFFFFull, 1, 0, 0, 0, 0, 0});
    s.set_started(1);
    G::set_shown(0);
  }
  return Done(calls.Call(kRun, sp, 0x82250B28, movie, 0));
}

uint64_t Update(uint64_t member, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822513B8 PowerGraphScreen67::Update");
  // The empty sub_82106540(100, 30, 0, 0x82011208) the original calls first
  // is left out: it returns at once.
  // The original's 144-byte frame held the movie id and the steps' table
  // (values and a host table since E5 step 6): its calls run from `sp`
  // (none reads its caller's frame).
  const uint32_t movie = Movie();
  const schema::PageObject pages = G::pages();
  const uint32_t step = pages.step();
  uint64_t answer = movie;
  if (step == 0xFFFFFFFFu) {
    answer = calls.Call(kRun, sp, 0x822514D8, movie, 0);
  } else {
    bool done = true;
    if (int32_t(step) >= 0) {
      // Steps 0 to 2 (in play 0 and 1); past them the original reads past
      // its table without a check: those words read where they would be on
      // the stack.
      uint32_t adjust, target;
      if (step < 3) {
        adjust = kSteps[step].adjust;
        target = kSteps[step].function;
      } else {
        const uint32_t table = sp - kUpdateFrame + 80;
        adjust = LoadU32(table + 4 + (step << 3));
        target = LoadU32(table + (step << 3));
      }
      answer = calls.Call(target, sp, 0x82251470, uint64_t(adjust) + member,
                          pages.step_started_address());
      done = int32_t(answer) != 0;
    }
    if (done) {
      const schema::PageObject p = G::pages();
      const uint32_t queued = p.queued_step();
      if (queued == 0xFFFFFFFFu) {
        if (int32_t(step) >= 0) {
          p.set_page(p.next_page());
          p.set_step(queued);
        } else {
          const uint32_t next = p.next_page();
          p.set_step(0xFFFFFFFFu);
          p.set_page(next);
        }
      } else {
        p.set_queued_step(0xFFFFFFFFu);
        p.set_unknown_20(0);
        p.set_step(queued);
      }
      p.set_step_started(0);
    }
  }
  const uint32_t state = G::data().queued_state();
  if (state != 21) {
    Member(uint32_t(member)).set_machine_state(state);
    G::data().set_queued_state(21);
  }
  return answer;
}

}  // namespace power_graph_screen
