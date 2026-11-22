// The type 12 mode's machines' set-up and teardown handlers
// (docs/functions/network-mode.md): see network_mode.h. Each machine's
// Create, OpenMovie, WaitForMovie, Leave and Destroy; the near copies share
// their bodies here with the machine's parameters (movie, asset, globals,
// sizes, return addresses), each original's stores and frame words in its
// order. VirtualAlloc, VirtualFree, UiMovies::Open and Delete,
// UiMovie::FinishAll and GarageCar::Reset are guest calls through their
// overrides; UiMovies::Find and IsLoaded their plain C++. The "Now Loading"
// debug print (the empty sub_82106540) is left out: only the r3 its
// argument leaves.

#include "network_mode/network_mode.h"

#include "base/log.h"
#include "network_mode/internal.h"
#include "schema/system_services.h"
#include "ui_movies/game.h"

#include <initializer_list>
#include <utility>

namespace network_mode {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemCommit = 4096;
constexpr uint32_t kReadWrite = 4;
constexpr uint32_t kMemRelease = 0x8000;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kMoviesOpen = 0x8211E808;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kGarageCarReset = 0x82110448;
// The globals' page (lis r?,-32127), kept in frames by some Creates.
constexpr uint32_t kGlobalsPage = 0x82810000;

constexpr uint32_t kNoMovie = kNone;

using G = schema::NetworkModeGlobals;
using NetMachine = schema::NetMachine;
using PageObject = schema::PageObject;
using FrontEndMenu = schema::FrontEndMenu;

// --- The machines' parameters ---

enum class WaitKind {
  kLatched,  // +9608 set to 1 once the movie (and the second) is loaded
  kEach,     // +9608 = IsLoaded on each call
};
enum class LeaveKind {
  kStateFirst,  // state 7 first, the movie finished and deleted, message 4
  kStateAfter,  // the movie finished and deleted, +9608 = 0 and state 7, message 4
  kScreen1,     // Type12Screen1's (its two movies and the data's lists)
};
enum class EndKind {
  kDataBack,         // 14 when the data's +4 is set; then 14 (+9624 1) or 15 (+9632 1)
  kDataBackChecked,  // the same, the data null-checked
  kBackAgain,        // 14 (+9624 1) or 15 (+9632 1)
  kBackRaceAgain,    // 14 (+9624 1), 11 (+9628 1) or 15 (+9632 1)
  kBack,             // 14 (+9624 1)
};

struct Free {
  uint32_t global;
  uint32_t lr;
};

struct Spec {
  uint32_t index;
  const char* open_name;
  const char* wait_name;
  const char* leave_name;
  const char* destroy_name;
  // OpenMovie (Type12Screen1's opens two of its own).
  uint32_t movie;
  uint32_t asset;
  // WaitForMovie: the movie (and a second, both needed) it waits for, the
  // "Now Loading" counter (0 none) and the r3 the print's argument leaves,
  // its frame.
  WaitKind wait;
  uint32_t wait_movie;
  uint32_t wait_movie2;
  uint32_t counter;
  uint32_t print_r3;
  uint32_t wait_frame;
  // Leave: its kind and address (the calls' return addresses from it).
  LeaveKind leave;
  uint32_t leave_at;
  // Destroy: its state's kind, the data's global, its frame, the frees in
  // order and the globals cleared in order.
  EndKind end;
  uint32_t data;
  uint32_t destroy_frame;
  Free frees[6];
  uint32_t cleared[6];
  uint32_t count;
};

constexpr Spec kSpecs[] = {
    {0, "sub_8227C078 NetModeSelect::OpenMovie", "sub_8227C558 NetModeSelect::WaitForMovie",
     "sub_8227C8C0 NetModeSelect::Leave", "sub_8227C4B8 NetModeSelect::Destroy", 35, 22581,
     WaitKind::kLatched, 35, kNoMovie, 0, 0, 96, LeaveKind::kStateFirst, 0x8227C8C0,
     EndKind::kDataBack, G::select_data_address(), 128,
     {{G::select_pages_address(), 0x8227C51C}, {G::select_menu_address(), 0x8227C530},
      {G::select_data_address(), 0x8227C540}},
     {G::select_pages_address(), G::select_menu_address(), G::select_data_address()}, 3},
    {1, "sub_82271D18 Type12Screen1::OpenMovie", "sub_82272398 Type12Screen1::WaitForMovie",
     "sub_822727E0 Type12Screen1::Leave", "sub_822722B0 Type12Screen1::Destroy", 33, 22580,
     WaitKind::kLatched, 33, 34, G::screen1_loading_count_address(), 100, 112, LeaveKind::kScreen1,
     0x822727E0,
     EndKind::kDataBack, G::screen1_data_address(), 144,
     {{G::screen1_pages_address(), 0x82272314},
      {G::screen1_menu2_address(), 0x82272328},
      {G::screen1_menu1_address(), 0x8227233C},
      {G::screen1_menu0_address(), 0x82272350},
      {G::screen1_menu3_address(), 0x82272364},
      {G::screen1_data_address(), 0x82272374}},
     {G::screen1_menu2_address(), G::screen1_menu1_address(), G::screen1_menu3_address(),
      G::screen1_pages_address(), G::screen1_menu0_address(), G::screen1_data_address()}, 6},
    {3, "sub_8225DFD8 Type12Screen3::OpenMovie", "sub_8225E1B0 Type12Screen3::WaitForMovie",
     "sub_8225E518 Type12Screen3::Leave", "sub_8225E100 Type12Screen3::Destroy", 32, 22582,
     WaitKind::kLatched, 33, kNoMovie, G::screen3_loading_count_address(), 100, 112,
     LeaveKind::kStateFirst, 0x8225E518,
     EndKind::kDataBackChecked, G::screen3_data_address(), 128,
     {{G::screen3_pages_address(), 0x8225E174}, {G::screen3_menu_address(), 0x8225E188},
      {G::screen3_data_address(), 0x8225E198}},
     {G::screen3_pages_address(), G::screen3_menu_address(), G::screen3_data_address()}, 3},
    {4, "sub_82282098 NetRanking::OpenMovie", "sub_822825C0 NetRanking::WaitForMovie",
     "sub_82282848 NetRanking::Leave", "sub_82282508 NetRanking::Destroy", 36, 22590,
     WaitKind::kLatched, 33, kNoMovie, 0, 0, 96, LeaveKind::kStateFirst, 0x82282848,
     EndKind::kDataBack, G::ranking_data_address(), 128,
     {{G::ranking_menu2_address(), 0x8228256C},
      {G::ranking_pages_address(), 0x82282580},
      {G::ranking_menu_address(), 0x82282594},
      {G::ranking_data_address(), 0x822825A4}},
     {G::ranking_menu2_address(), G::ranking_pages_address(), G::ranking_menu_address(),
      G::ranking_data_address()}, 4},
    {5, "sub_8227FC70 NetTaCourse::OpenMovie", "sub_8227FF10 NetTaCourse::WaitForMovie",
     "sub_82280218 NetTaCourse::Leave", "sub_8227FE70 NetTaCourse::Destroy", 37, 22593,
     WaitKind::kLatched, 33, kNoMovie, G::ta_course_loading_count_address(), 100, 112,
     LeaveKind::kStateFirst, 0x82280218,
     EndKind::kDataBack, G::ta_course_data_address(), 128,
     {{G::ta_course_pages_address(), 0x8227FED4}, {G::ta_course_menu_address(), 0x8227FEE8},
      {G::ta_course_data_address(), 0x8227FEF8}},
     {G::ta_course_pages_address(), G::ta_course_menu_address(), G::ta_course_data_address()}, 3},
    {6, "sub_82265920 Type12Screen6::OpenMovie", "sub_82265C00 Type12Screen6::WaitForMovie",
     "sub_82265DC0 Type12Screen6::Leave", "sub_82265B70 Type12Screen6::Destroy", 38, 22594,
     WaitKind::kEach, 38, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x82265DC0,
     EndKind::kBackAgain, 0, 112, {{G::screen6_pages_address(), 0x82265BC4},
     {G::screen6_menu_address(), 0x82265BDC}},
     {G::screen6_pages_address(), G::screen6_menu_address()}, 2},
    {7, "sub_822688A8 Type12Screen7::OpenMovie", "sub_82268F70 Type12Screen7::WaitForMovie",
     "sub_822697D8 Type12Screen7::Leave", "sub_82268EC8 Type12Screen7::Destroy", 39, 22596,
     WaitKind::kEach, 39, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x822697D8,
     EndKind::kBackRaceAgain, 0, 112,
     {{G::screen7_pages_address(), 0x82268F2C}, {G::screen7_menu_address(), 0x82268F40},
      {G::screen7_data_address(), 0x82268F54}},
     {G::screen7_pages_address(), G::screen7_menu_address(), G::screen7_data_address()}, 3},
    {8, "sub_822609C0 Type12Screen8::OpenMovie", "sub_822611F8 Type12Screen8::WaitForMovie",
     "sub_82261358 Type12Screen8::Leave", "sub_82261168 Type12Screen8::Destroy", 42, 22598,
     WaitKind::kEach, 42, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x82261358,
     EndKind::kBackAgain, 0, 112, {{G::screen8_pages_address(), 0x822611BC},
     {G::screen8_menu_address(), 0x822611D4}},
     {G::screen8_pages_address(), G::screen8_menu_address()}, 2},
    {9, "sub_82263048 Type12Screen9::OpenMovie", "sub_822632C8 Type12Screen9::WaitForMovie",
     "sub_82263448 Type12Screen9::Leave", "sub_82263238 Type12Screen9::Destroy", 43, 22599,
     WaitKind::kEach, 43, kNoMovie, 0, 500, 96, LeaveKind::kStateAfter, 0x82263448,
     EndKind::kBackAgain, 0, 112, {{G::screen9_pages_address(), 0x8226328C},
     {G::screen9_menu_address(), 0x822632A4}},
     {G::screen9_pages_address(), G::screen9_menu_address()}, 2},
    {10, "sub_82287CD0 Type12Dialog10::OpenMovie", "sub_82287F40 Type12Dialog10::WaitForMovie",
     "sub_82288028 Type12Dialog10::Leave", "sub_82287EC0 Type12Dialog10::Destroy", 40, 22595,
     WaitKind::kEach, 40, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x82288028,
     EndKind::kBack, 0, 112, {{G::dialog10_pages_address(), 0x82287F00},
     {G::dialog10_menu_address(), 0x82287F18}},
     {G::dialog10_pages_address(), G::dialog10_menu_address()}, 2},
    {11, "sub_82276E40 Type12Dialog11::OpenMovie", "sub_82276FC0 Type12Dialog11::WaitForMovie",
     "sub_822770B8 Type12Dialog11::Leave", "sub_82276F40 Type12Dialog11::Destroy", 41, 22597,
     WaitKind::kEach, 41, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x822770B8,
     EndKind::kBack, 0, 112, {{G::dialog11_pages_address(), 0x82276F80},
     {G::dialog11_menu_address(), 0x82276F98}},
     {G::dialog11_pages_address(), G::dialog11_menu_address()}, 2},
    {12, "sub_8227ECE8 Type12Dialog12::OpenMovie", "sub_8227EEC0 Type12Dialog12::WaitForMovie",
     "sub_8227F038 Type12Dialog12::Leave", "sub_8227EE40 Type12Dialog12::Destroy", 17, 22560,
     WaitKind::kEach, 17, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x8227F038,
     EndKind::kBack, 0, 112, {{G::dialog12_pages_address(), 0x8227EE80},
     {G::dialog12_menu_address(), 0x8227EE98}},
     {G::dialog12_pages_address(), G::dialog12_menu_address()}, 2},
    {13, "sub_8227DEC0 Type12Dialog13::OpenMovie", "sub_8227E030 Type12Dialog13::WaitForMovie",
     "sub_8227E118 Type12Dialog13::Leave", "sub_8227DFB0 Type12Dialog13::Destroy", 16, 22559,
     WaitKind::kEach, 16, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x8227E118,
     EndKind::kBack, 0, 112, {{G::dialog13_pages_address(), 0x8227DFF0},
     {G::dialog13_menu_address(), 0x8227E008}},
     {G::dialog13_pages_address(), G::dialog13_menu_address()}, 2},
};
constexpr size_t kSpecCount = std::size(kSpecs);

// The spec's slot for a machine's index (2 and above 13 none).
constexpr int SlotOf(uint32_t index) {
  for (size_t i = 0; i < kSpecCount; i++) {
    if (kSpecs[i].index == index) {
      return int(i);
    }
  }
  return -1;
}

// --- Shared pieces ---

// Stores at offsets into other areas' objects (the screens' data).
struct Field {
  uint32_t offset;
  uint32_t value;
};
void Fields(uint8_t* base, uint32_t block, std::initializer_list<Field> fields) {
  for (const Field& f : fields) {
    Store(base, block + f.offset, f.value);
  }
}

// Stores through a view's setters, in the list's order (its values computed
// before the first store, as the lists of offsets were).
template <typename V>
struct Set {
  void (V::*set)(uint32_t) const;
  uint32_t value;
};
template <typename V>
void Apply(const V& view, std::initializer_list<Set<V>> stores) {
  for (const Set<V>& s : stores) {
    (view.*s.set)(s.value);
  }
}

uint64_t Alloc(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t bytes) {
  return calls.Call(kVirtualAlloc, frame, lr, 0, bytes, kMemCommit, kReadWrite);
}

// The page object's stores (the step and +20 -1, the rest 0) in the usual
// order: the next page, the page, the page before, +20, the step, its
// started word.
void Pages(uint8_t* base, uint32_t pages) {
  Apply(PageObject(base, pages), {{&PageObject::set_next_page, 0},
                                  {&PageObject::set_page, 0},
                                  {&PageObject::set_previous_page, 0},
                                  {&PageObject::set_unknown_20, kNone},
                                  {&PageObject::set_step, kNone},
                                  {&PageObject::set_step_started, 0}});
}

// A menu's stores after its head (`Apply`'s): the wrapped flag, the visible
// and previous rows, the side, the player and the three step words, 0.
void MenuTail(uint8_t* base, uint32_t menu) {
  const FrontEndMenu m(base, menu);
  m.set_wrapped(0);
  m.set_visible_row(0);
  m.set_previous_row(0);
  m.set_side(0);
  m.set_player(0);
  m.set_up_down_locked(0);
  m.set_left_right_steps(0);
  m.set_shoulders_step(0);
}
// The usual head: the first item 0, the last `count`, wraps `flag`, the
// cursor, the one before and moved 0.
void MenuCountFirst(uint8_t* base, uint32_t menu, uint32_t count, uint32_t flag) {
  Apply(FrontEndMenu(base, menu), {{&FrontEndMenu::set_first_item, 0},
                                   {&FrontEndMenu::set_last_item, count},
                                   {&FrontEndMenu::set_wraps, flag},
                                   {&FrontEndMenu::set_cursor, 0},
                                   {&FrontEndMenu::set_previous, 0},
                                   {&FrontEndMenu::set_moved, 0}});
  MenuTail(base, menu);
}
// The first item 0, wraps `flag`, the cursor and the one before 0, the last
// item `count`, moved 0.
void MenuCountAfter4(uint8_t* base, uint32_t menu, uint32_t count, uint32_t flag) {
  Apply(FrontEndMenu(base, menu), {{&FrontEndMenu::set_first_item, 0},
                                   {&FrontEndMenu::set_wraps, flag},
                                   {&FrontEndMenu::set_cursor, 0},
                                   {&FrontEndMenu::set_previous, 0},
                                   {&FrontEndMenu::set_last_item, count},
                                   {&FrontEndMenu::set_moved, 0}});
  MenuTail(base, menu);
}

// The machine's state once set up: 1 when it opens on its own message
// (reenter), else 9.
void Opened(uint8_t* base, uint32_t machine) {
  const NetMachine m(base, machine);
  m.set_state(m.reenter() != 0 ? 1 : 9);
}
// Once its movie is loaded: 9 (its open message) or 4.
void Loaded(uint8_t* base, uint32_t machine) {
  const NetMachine m(base, machine);
  m.set_state(m.reenter() != 0 ? 9 : 4);
}
// The three exit words (back, the race, the mode again) 0.
void ClearExits(uint8_t* base, uint32_t machine) {
  const NetMachine m(base, machine);
  m.set_back(0);
  m.set_race(0);
  m.set_again(0);
}

// UiMovies::Find for movie `id` with the id stored at `key` (the original's
// frame word): the movie in the pair's value.
uint32_t FindMovie(uint8_t* base, uint32_t key, uint32_t id) {
  Store(base, key, id);
  return Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}
// UiMovie::FinishAll on movie `id` when found.
void Finish(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr, uint32_t id) {
  const uint32_t movie = FindMovie(base, frame + 80, id);
  if (movie != 0) {
    calls.Call(kFinishAll, frame, lr, movie);
  }
}

// --- The handlers by spec ---

template <size_t I>
uint64_t OpenMovieOf(uint32_t sp, uint32_t lr, const Calls& calls) {
  constexpr const Spec& s = kSpecs[I];
  ITC_NATIVE_RUNNING(s.open_name);
  return calls.Call(kMoviesOpen, sp, lr, s.movie, s.asset);
}
// Type12Screen1's: both its movies, from its 96-byte frame.
uint64_t Screen1OpenMovie(uint8_t* base, uint32_t sp, uint32_t, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82271D18 Type12Screen1::OpenMovie");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMoviesOpen, frame, 0x82271D30, 33, 22580);
  return calls.Call(kMoviesOpen, frame, 0x82271D3C, 34, 22589);
}

template <size_t I>
uint64_t WaitForMovieOf(uint8_t* base, uint64_t r3, uint32_t sp) {
  constexpr const Spec& s = kSpecs[I];
  ITC_NATIVE_RUNNING(s.wait_name);
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, s.wait_frame);
  if (s.wait == WaitKind::kEach) {
    r3 = ui_movies::IsLoaded(base, s.wait_movie);
    NetMachine(base, machine).set_loaded(uint32_t(r3));
    if (s.print_r3 != 0) {
      r3 = s.print_r3;  // the print's first argument
    }
    if (NetMachine(base, machine).loaded() != 0) {
      Loaded(base, machine);
    }
    return r3;
  }
  if (NetMachine(base, machine).loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, s.wait_movie);
    if (uint32_t(r3) != 0 && s.wait_movie2 != kNoMovie) {
      r3 = ui_movies::IsLoaded(base, s.wait_movie2);
    }
    if (uint32_t(r3) != 0) {
      NetMachine(base, machine).set_loaded(1);
    }
  }
  if (s.counter != 0) {
    // The print when the counter over 15 is even.
    if (((Load(base, s.counter) / 15) & 1) == 0) {
      r3 = s.print_r3;
    }
    Store(base, s.counter, Load(base, s.counter) + 1);
  }
  if (NetMachine(base, machine).loaded() != 0) {
    Loaded(base, machine);
  }
  return r3;
}

// Type12Screen1's Leave: its two movies, the data's three pointer lists
// (+20, +280, +540, each count at +256 from it) emptied from the end.
uint64_t Screen1Leave(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822727E0 Type12Screen1::Leave");
  const uint32_t frame = Frame(base, sp, 128);
  NetMachine(base, machine).set_state(7);
  Finish(base, calls, frame, 0x82272828, 33);
  Finish(base, calls, frame, 0x8227284C, 34);
  for (const auto& [list, lr] : {std::pair{20u, 0x82272880u}, std::pair{280u, 0x822728B8u},
                                  std::pair{540u, 0x822728F0u}}) {
    for (;;) {
      const uint32_t at = G::screen1_data(base).address + list;
      const uint32_t count = Load(base, at + 256);
      if (count == 0) {
        break;
      }
      Store(base, at + 256, count - 1);
      calls.Call(kVirtualFree, frame, lr, Load(base, at + 4 * (count - 1)), 0, kMemRelease);
    }
  }
  calls.Call(kMoviesDelete, frame, 0x82272908, 33);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x82272910, 34);
  PostRing(base, dispatcher, 4);
  return r3;
}

template <size_t I>
uint64_t LeaveOf(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                 const Calls& calls) {
  constexpr const Spec& s = kSpecs[I];
  ITC_NATIVE_RUNNING(s.leave_name);
  const uint32_t frame = Frame(base, sp, 112);
  uint64_t r3;
  if (s.leave == LeaveKind::kStateFirst) {
    NetMachine(base, machine).set_state(7);
    Finish(base, calls, frame, s.leave_at + 0x48, s.movie);
    r3 = calls.Call(kMoviesDelete, frame, s.leave_at + 0x50, s.movie);
  } else {
    Finish(base, calls, frame, s.leave_at + 0x44, s.movie);
    r3 = calls.Call(kMoviesDelete, frame, s.leave_at + 0x4C, s.movie);
    NetMachine(base, machine).set_loaded(0);
    NetMachine(base, machine).set_state(7);
  }
  PostRing(base, dispatcher, 4);
  return r3;
}

template <size_t I>
uint64_t DestroyOf(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  constexpr const Spec& s = kSpecs[I];
  ITC_NATIVE_RUNNING(s.destroy_name);
  const uint32_t frame = Frame(base, sp, s.destroy_frame);
  const NetMachine m(base, machine);
  auto state = [&](uint32_t value) { m.set_state(value); };
  switch (s.end) {
    case EndKind::kDataBack:
    case EndKind::kDataBackChecked: {
      const uint32_t data = Load(base, s.data);
      if ((s.end == EndKind::kDataBack || data != 0) && Load(base, data + 4) != 0) {
        state(14);
      }
      if (m.back() == 1) {
        state(14);
      } else if (m.again() == 1) {
        state(15);
      }
      break;
    }
    case EndKind::kBackAgain:
      if (m.back() == 1) {
        state(14);
      } else if (m.again() == 1) {
        state(15);
      }
      break;
    case EndKind::kBackRaceAgain:
      if (m.back() == 1) {
        state(14);
      } else if (m.race() == 1) {
        state(11);
      } else if (m.again() == 1) {
        state(15);
      }
      break;
    case EndKind::kBack:
      if (m.back() == 1) {
        state(14);
      }
      break;
  }
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < s.count; i++) {
    r3 = calls.Call(kVirtualFree, frame, s.frees[i].lr, Load(base, s.frees[i].global), 0,
                    kMemRelease);
  }
  for (uint32_t i = 0; i < s.count; i++) {
    Store(base, s.cleared[i], 0);
  }
  return r3;
}

using OpenFn = uint64_t (*)(uint8_t*, uint32_t, uint32_t, const Calls&);
using WaitFn = uint64_t (*)(uint8_t*, uint64_t, uint32_t);
using LeaveFn = uint64_t (*)(uint8_t*, uint32_t, uint32_t, uint32_t, const Calls&);
using DestroyFn = uint64_t (*)(uint8_t*, uint32_t, uint32_t, const Calls&);

template <size_t I>
uint64_t OpenAdapter(uint8_t*, uint32_t sp, uint32_t lr, const Calls& calls) {
  return OpenMovieOf<I>(sp, lr, calls);
}

template <size_t... I>
constexpr auto MakeTables(std::index_sequence<I...>) {
  struct Tables {
    OpenFn open[sizeof...(I)];
    WaitFn wait[sizeof...(I)];
    LeaveFn leave[sizeof...(I)];
    DestroyFn destroy[sizeof...(I)];
  };
  return Tables{{(kSpecs[I].index == 1 ? Screen1OpenMovie : OpenAdapter<I>)...},
                {WaitForMovieOf<I>...},
                {(kSpecs[I].index == 1 ? Screen1Leave : LeaveOf<I>)...},
                {DestroyOf<I>...}};
}
constexpr auto kTables = MakeTables(std::make_index_sequence<kSpecCount>());

// --- The Creates ---

uint64_t ModeSelectCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227C398 NetModeSelect::Create");
  const uint32_t frame = Frame(base, sp, 128);
  NetMachine(base, machine).set_loaded(0);
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x8227C3C4, 28));
  G::set_select_pages(base, pages);
  Pages(base, pages);
  const uint32_t menu = uint32_t(Alloc(calls, frame, 0x8227C400, 68));
  G::set_select_menu(base, menu);
  MenuCountFirst(base, menu, 1, 1);
  const uint64_t r3 = Alloc(calls, frame, 0x8227C45C, 36);
  const uint32_t data = uint32_t(r3);
  G::set_select_data(base, data);
  using D = schema::NetModeSelectData;
  Apply(D(base, data), {{&D::set_shown_before, 0},
                        {&D::set_back, 0},
                        {&D::set_dialog_page, kNone},
                        {&D::set_dialog_transition, kNone},
                        {&D::set_back_to_title, 0},
                        {&D::set_sign_in_ui_shown, 0},
                        {&D::set_privilege_dialog, 0},
                        {&D::set_sign_in_ticks, 0},
                        {&D::set_sign_in_continues, 1}});
  ClearExits(base, machine);
  Opened(base, machine);
  return r3;
}

uint64_t Screen1Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822720A8 Type12Screen1::Create");
  const uint32_t frame = Frame(base, sp, 128);
  NetMachine(base, machine).set_loaded(0);
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x822720D4, 28));
  G::set_screen1_pages(base, pages);
  Apply(PageObject(base, pages), {{&PageObject::set_next_page, 0},
                                  {&PageObject::set_page, 0},
                                  {&PageObject::set_previous_page, 0},
                                  {&PageObject::set_step_started, 0},
                                  {&PageObject::set_unknown_20, kNone},
                                  {&PageObject::set_step, kNone}});
  struct MenuSpec {
    uint32_t global, lr, count;
  };
  for (const MenuSpec& m : {MenuSpec{G::screen1_menu0_address(), 0x82272110, 2},
                            MenuSpec{G::screen1_menu1_address(), 0x82272170, 2},
                            MenuSpec{G::screen1_menu2_address(), 0x822721C8, 1},
                            MenuSpec{G::screen1_menu3_address(), 0x82272220, 1}}) {
    const uint32_t menu = uint32_t(Alloc(calls, frame, m.lr, 68));
    Store(base, m.global, menu);
    MenuCountFirst(base, menu, m.count, 1);
  }
  const uint32_t data = uint32_t(Alloc(calls, frame, 0x82272274, 2820));
  G::set_screen1_data(base, data);
  const uint64_t r3 = Screen1ResetData(base, data, frame, calls);
  ClearExits(base, machine);
  Opened(base, machine);
  return r3;
}

uint64_t Screen3Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225DFE8 Type12Screen3::Create");
  const uint32_t frame = Frame(base, sp, 128);
  NetMachine(base, machine).set_loaded(0);
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x8225E014, 28));
  G::set_screen3_pages(base, pages);
  Pages(base, pages);
  const uint32_t menu = uint32_t(Alloc(calls, frame, 0x8225E050, 68));
  G::set_screen3_menu(base, menu);
  MenuCountAfter4(base, menu, 3, 1);
  const uint64_t r3 = Alloc(calls, frame, 0x8225E0B0, 24);
  const uint32_t data = uint32_t(r3);
  G::set_screen3_data(base, data);
  Fields(base, data, {{0, 0}, {4, 0}, {8, kNone}, {12, kNone}, {16, 0}, {20, 0}});
  ClearExits(base, machine);
  Opened(base, machine);
  return r3;
}

uint64_t RankingCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82282358 NetRanking::Create");
  const uint32_t frame = Frame(base, sp, 128);
  NetMachine(base, machine).set_loaded(0);
  const uint32_t data = uint32_t(Alloc(calls, frame, 0x82282384, 108));
  G::set_ranking_data(base, data);
  using D = schema::NetRankingData;
  Apply(D(base, data), {{&D::set_unknown_0, 0},     {&D::set_back, 0},
                        {&D::set_unknown_8, 0},     {&D::set_unknown_48, kNone},
                        {&D::set_row_limit, 0},     {&D::set_unknown_52, 0},
                        {&D::set_unknown_64, 0},    {&D::set_unknown_60, 0},
                        {&D::set_unknown_24, kNone}, {&D::set_unknown_20, kNone},
                        {&D::set_unknown_28, 0},    {&D::set_unknown_100, 0},
                        {&D::set_unknown_96, 0},    {&D::set_unknown_92, 0},
                        {&D::set_unknown_32, 0},    {&D::set_unknown_104, kNone},
                        {&D::set_unknown_36, 0},    {&D::set_unknown_12, 0},
                        {&D::set_unknown_40, 1},    {&D::set_unknown_44, 0},
                        {&D::set_row_count, kNone}, {&D::set_unknown_16, 0}});
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x82282404, 28));
  G::set_ranking_pages(base, pages);
  Pages(base, pages);
  const uint32_t menu = uint32_t(Alloc(calls, frame, 0x8228243C, 68));
  G::set_ranking_menu(base, menu);
  MenuCountAfter4(base, menu, 7, 1);
  const uint64_t r3 = Alloc(calls, frame, 0x82282498, 68);
  G::set_ranking_menu2(base, uint32_t(r3));
  MenuCountFirst(base, uint32_t(r3), 1, 0);
  ClearExits(base, machine);
  Opened(base, machine);
  return r3;
}

uint64_t TaCourseCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227FD40 NetTaCourse::Create");
  const uint32_t frame = Frame(base, sp, 128);
  NetMachine(base, machine).set_loaded(0);
  const uint32_t data = uint32_t(Alloc(calls, frame, 0x8227FD6C, 48));
  G::set_ta_course_data(base, data);
  Fields(base, data,
         {{0, 0}, {4, 0}, {8, 0}, {20, 1}, {24, 0}, {12, 0}, {16, 0}, {28, 0}, {32, 0},
          {40, kNone}, {44, kNone}});
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x8227FDC0, 28));
  G::set_ta_course_pages(base, pages);
  Pages(base, pages);
  const uint64_t r3 = Alloc(calls, frame, 0x8227FDF8, 68);
  G::set_ta_course_menu(base, uint32_t(r3));
  MenuCountAfter4(base, uint32_t(r3), 7, 1);
  ClearExits(base, machine);
  Opened(base, machine);
  return r3;
}

// Screen 6 and the dialogs: the page object and the menu allocated first,
// then set up through their globals.
uint64_t PagesAndMenu(uint8_t* base, uint32_t machine, uint32_t frame, const Calls& calls,
                      uint32_t pages_global, uint32_t pages_lr, uint32_t menu_lr, uint32_t count,
                      uint32_t flag) {
  NetMachine(base, machine).set_loaded(0);
  Store(base, pages_global, uint32_t(Alloc(calls, frame, pages_lr, 28)));
  const uint64_t r3 = Alloc(calls, frame, menu_lr, 68);
  Store(base, pages_global + 4, uint32_t(r3));
  Pages(base, Load(base, pages_global));
  MenuCountFirst(base, Load(base, pages_global + 4), count, flag);
  return r3;
}

uint64_t Screen6Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82265A88 Type12Screen6::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t r3 =
      PagesAndMenu(base, machine, frame, calls, G::screen6_pages_address(), 0x82265AB4,
                   0x82265AD0, 2, 1);
  G::set_screen6_word(base, 0);
  NetMachine(base, machine).set_unknown_9656(0);
  Opened(base, machine);
  return r3;
}

uint64_t Screen7Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82268D18 Type12Screen7::Create");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, sp + 20, machine);
  NetMachine(base, machine).set_loaded(0);
  G::set_screen7_pages(base, uint32_t(Alloc(calls, frame, 0x82268D50, 28)));
  G::set_screen7_menu(base, uint32_t(Alloc(calls, frame, 0x82268D6C, 68)));
  Store(base, frame + 88, kGlobalsPage);
  const uint64_t r3 = Alloc(calls, frame, 0x82268D8C, 8);
  const uint32_t pages = G::screen7_pages(base).address;
  Store(base, frame + 80, 0);
  Store(base, frame + 92, kGlobalsPage);
  Store(base, frame + 84, kNone);
  G::set_screen7_data(base, uint32_t(r3));
  Apply(PageObject(base, pages), {{&PageObject::set_step, kNone},
                                  {&PageObject::set_next_page, 0},
                                  {&PageObject::set_page, 0},
                                  {&PageObject::set_previous_page, 0},
                                  {&PageObject::set_unknown_20, kNone},
                                  {&PageObject::set_step_started, 0}});
  MenuCountFirst(base, Load(base, Load(base, frame + 88) + 0x6164), 3, 1);
  const schema::NetScreen7Data data(base, Load(base, Load(base, frame + 92) + 0x6168));
  data.set_unknown_0(0);
  data.set_unknown_4(0);
  const uint32_t m = Load(base, sp + 20);
  using M = NetMachine;
  Apply(M(base, m), {{&M::set_unknown_9720, 0}, {&M::set_unknown_9656, 0},
                     {&M::set_unknown_9660, 0}, {&M::set_unknown_9664, 0},
                     {&M::set_unknown_9668, 0}, {&M::set_unknown_9672, 0},
                     {&M::set_unknown_9676, 0}, {&M::set_unknown_9680, 0},
                     {&M::set_unknown_9640, 0}, {&M::set_unknown_9644, 1},
                     {&M::set_unknown_9648, 0}, {&M::set_unknown_9652, 1},
                     {&M::set_unknown_9684, 0}, {&M::set_unknown_9688, 0},
                     {&M::set_unknown_9692, 0}, {&M::set_unknown_9696, 0},
                     {&M::set_unknown_9700, 0}, {&M::set_unknown_9704, 0},
                     {&M::set_unknown_9712, 0}, {&M::set_unknown_9708, 0},
                     {&M::set_unknown_9716, 0}, {&M::set_unknown_9724, 1},
                     {&M::set_back, 0},         {&M::set_race, 0},
                     {&M::set_interrupted, 0},  {&M::set_unknown_9728, 0}});
  Opened(base, m);
  return r3;
}

uint64_t Screen8Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82261030 Type12Screen8::Create");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, sp + 20, machine);
  NetMachine(base, machine).set_loaded(0);
  G::set_screen8_pages(base, uint32_t(Alloc(calls, frame, 0x82261068, 28)));
  const uint64_t r3 = Alloc(calls, frame, 0x82261084, 68);
  const uint32_t pages = G::screen8_pages(base).address;
  Store(base, frame + 80, 0);
  Store(base, frame + 84, kNone);
  Store(base, frame + 88, kNone);
  Store(base, frame + 92, kGlobalsPage);
  G::set_screen8_menu(base, uint32_t(r3));
  Apply(PageObject(base, pages), {{&PageObject::set_next_page, 0},
                                  {&PageObject::set_page, 0},
                                  {&PageObject::set_step_started, 0},
                                  {&PageObject::set_previous_page, 0},
                                  {&PageObject::set_unknown_20, Load(base, frame + 84)},
                                  {&PageObject::set_step, Load(base, frame + 88)}});
  MenuCountFirst(base, Load(base, Load(base, frame + 92) + 0x6148), 0, 1);
  const uint32_t m = Load(base, sp + 20);
  using M = NetMachine;
  Apply(M(base, m), {{&M::set_unknown_9640, 0}, {&M::set_unknown_9644, 0},
                     {&M::set_back, 0},         {&M::set_race, 0},
                     {&M::set_again, 0},        {&M::set_unknown_9660, 1},
                     {&M::set_unknown_9664, 1}, {&M::set_unknown_9656, 0},
                     {&M::set_unknown_9648, 0}, {&M::set_unknown_9652, 0}});
  Opened(base, m);
  return r3;
}

uint64_t Screen9Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822630C0 Type12Screen9::Create");
  const uint32_t frame = Frame(base, sp, 144);
  Store(base, sp + 20, machine);
  NetMachine(base, machine).set_loaded(0);
  G::set_screen9_pages(base, uint32_t(Alloc(calls, frame, 0x822630F8, 28)));
  const uint64_t r3 = Alloc(calls, frame, 0x82263114, 68);
  const uint32_t pages = G::screen9_pages(base).address;
  Fields(base, frame, {{80, 0}, {92, 0}, {100, 0}, {84, kNone}, {88, kNone}, {104, 0},
                       {96, kGlobalsPage}});
  G::set_screen9_menu(base, uint32_t(r3));
  Apply(PageObject(base, pages), {{&PageObject::set_next_page, 0},
                                  {&PageObject::set_page, 0},
                                  {&PageObject::set_previous_page, 0},
                                  {&PageObject::set_unknown_20, Load(base, frame + 84)},
                                  {&PageObject::set_step, Load(base, frame + 88)},
                                  {&PageObject::set_step_started, 0}});
  const uint32_t menu = Load(base, Load(base, frame + 96) + 0x6150);
  Apply(FrontEndMenu(base, menu), {{&FrontEndMenu::set_first_item, 0},
                                   {&FrontEndMenu::set_wraps, 1},
                                   {&FrontEndMenu::set_last_item, 0},
                                   {&FrontEndMenu::set_cursor, 0},
                                   {&FrontEndMenu::set_previous, 0},
                                   {&FrontEndMenu::set_moved, 0}});
  MenuTail(base, menu);
  const uint32_t m = Load(base, sp + 20);
  using M = NetMachine;
  Apply(M(base, m), {{&M::set_unknown_9640, 1}, {&M::set_unknown_9644, 1},
                     {&M::set_unknown_9648, 0}, {&M::set_unknown_9652, 0},
                     {&M::set_unknown_9656, 0}, {&M::set_unknown_9660, 0},
                     {&M::set_unknown_9664, 0}, {&M::set_unknown_9668, 0},
                     {&M::set_unknown_9672, 0}, {&M::set_unknown_9676, 0},
                     {&M::set_back, 0},         {&M::set_race, 0},
                     {&M::set_again, 0},        {&M::set_unknown_9680, 1},
                     {&M::set_unknown_9684, 0}, {&M::set_unknown_9688, 1},
                     {&M::set_unknown_9692, 0}});
  Opened(base, m);
  return r3;
}

uint64_t Dialog10Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82287DD8 Type12Dialog10::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t r3 =
      PagesAndMenu(base, machine, frame, calls, G::dialog10_pages_address(), 0x82287E04,
                   0x82287E20, 0, 1);
  NetMachine(base, machine).set_unknown_9644(0);
  NetMachine(base, machine).set_unknown_9640(0);
  G::set_dialog10_word(base, 0);
  Opened(base, machine);
  return r3;
}

uint64_t Dialog11Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82276E60 Type12Dialog11::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t r3 =
      PagesAndMenu(base, machine, frame, calls, G::dialog11_pages_address(), 0x82276E8C,
                   0x82276EA8, 1, 1);
  NetMachine(base, machine).set_back(0);
  NetMachine(base, machine).set_race(0);
  NetMachine(base, machine).set_unknown_9640(0);
  Opened(base, machine);
  return r3;
}

uint64_t Dialog12Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227ED58 Type12Dialog12::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t r3 =
      PagesAndMenu(base, machine, frame, calls, G::dialog12_pages_address(), 0x8227ED84,
                   0x8227EDA0, 0, 1);
  NetMachine(base, machine).set_unknown_9640(0);
  NetMachine(base, machine).set_unknown_9644(0);
  G::set_dialog12_reason(base, 0);
  Opened(base, machine);
  return r3;
}

uint64_t Dialog13Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227DED0 Type12Dialog13::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t r3 =
      PagesAndMenu(base, machine, frame, calls, G::dialog13_pages_address(), 0x8227DEFC,
                   0x8227DF18, 0, 1);
  NetMachine(base, machine).set_unknown_9640(0);
  G::set_dialog13_reason(base, 0);
  Opened(base, machine);
  return r3;
}

using CreateFn = uint64_t (*)(uint8_t*, uint32_t, uint32_t, const Calls&);
constexpr CreateFn kCreates[] = {
    ModeSelectCreate, Screen1Create,  Screen3Create,  RankingCreate,  TaCourseCreate,
    Screen6Create,    Screen7Create,  Screen8Create,  Screen9Create,  Dialog10Create,
    Dialog11Create,   Dialog12Create, Dialog13Create,
};
static_assert(std::size(kCreates) == kSpecCount);

}  // namespace

uint64_t MachineCreate(uint8_t* base, uint32_t index, uint32_t machine, uint32_t sp,
                       const Calls& calls) {
  return kCreates[SlotOf(index)](base, machine, sp, calls);
}

uint64_t MachineOpenMovie(uint8_t* base, uint32_t index, uint32_t sp, uint32_t lr,
                          const Calls& calls) {
  return kTables.open[SlotOf(index)](base, sp, lr, calls);
}

uint64_t MachineWaitForMovie(uint8_t* base, uint32_t index, uint64_t r3, uint32_t sp) {
  return kTables.wait[SlotOf(index)](base, r3, sp);
}

uint64_t MachineLeave(uint8_t* base, uint32_t index, uint32_t machine, uint32_t dispatcher,
                      uint32_t sp, const Calls& calls) {
  return kTables.leave[SlotOf(index)](base, machine, dispatcher, sp, calls);
}

uint64_t MachineDestroy(uint8_t* base, uint32_t index, uint32_t machine, uint32_t sp,
                        const Calls& calls) {
  return kTables.destroy[SlotOf(index)](base, machine, sp, calls);
}

uint64_t Screen1ResetData(uint8_t* base, uint32_t data, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82272030 Type12Screen1::ResetData");
  const uint32_t frame = Frame(base, sp, 112);
  Fields(base, data, {{0, 0}, {4, 0}, {8, 1}, {16, 0}, {820, 1}});
  const uint64_t r3 = calls.Call(kGarageCarReset, frame, 0x82272064, data + 832);
  Fields(base, data,
         {{824, 0}, {828, 0}, {800, 0}, {804, 0}, {12, 0}, {2800, 1}, {808, 0}, {812, 0},
          {2804, kNone}, {2808, kNone}, {816, 0}, {2812, 0}, {2816, 0}});
  return r3;
}

}  // namespace network_mode
