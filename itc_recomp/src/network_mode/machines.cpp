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

constexpr uint32_t kMachineReenter = 9612;
constexpr uint32_t kMachineRace = 9628;
constexpr uint32_t kNoMovie = kNone;

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
     EndKind::kDataBack, 0x82816198, 128,
     {{0x82816190, 0x8227C51C}, {0x82816194, 0x8227C530}, {0x82816198, 0x8227C540}},
     {0x82816190, 0x82816194, 0x82816198}, 3},
    {1, "sub_82271D18 Type12Screen1::OpenMovie", "sub_82272398 Type12Screen1::WaitForMovie",
     "sub_822727E0 Type12Screen1::Leave", "sub_822722B0 Type12Screen1::Destroy", 33, 22580,
     WaitKind::kLatched, 33, 34, 0x82816184, 100, 112, LeaveKind::kScreen1, 0x822727E0,
     EndKind::kDataBack, 0x82816180, 144,
     {{0x8281616C, 0x82272314},
      {0x82816178, 0x82272328},
      {0x82816174, 0x8227233C},
      {0x82816170, 0x82272350},
      {0x8281617C, 0x82272364},
      {0x82816180, 0x82272374}},
     {0x82816178, 0x82816174, 0x8281617C, 0x8281616C, 0x82816170, 0x82816180}, 6},
    {3, "sub_8225DFD8 Type12Screen3::OpenMovie", "sub_8225E1B0 Type12Screen3::WaitForMovie",
     "sub_8225E518 Type12Screen3::Leave", "sub_8225E100 Type12Screen3::Destroy", 32, 22582,
     WaitKind::kLatched, 33, kNoMovie, 0x828160EC, 100, 112, LeaveKind::kStateFirst, 0x8225E518,
     EndKind::kDataBackChecked, 0x828160E8, 128,
     {{0x828160E0, 0x8225E174}, {0x828160E4, 0x8225E188}, {0x828160E8, 0x8225E198}},
     {0x828160E0, 0x828160E4, 0x828160E8}, 3},
    {4, "sub_82282098 NetRanking::OpenMovie", "sub_822825C0 NetRanking::WaitForMovie",
     "sub_82282848 NetRanking::Leave", "sub_82282508 NetRanking::Destroy", 36, 22590,
     WaitKind::kLatched, 33, kNoMovie, 0, 0, 96, LeaveKind::kStateFirst, 0x82282848,
     EndKind::kDataBack, 0x828161D0, 128,
     {{0x828161CC, 0x8228256C},
      {0x828161C4, 0x82282580},
      {0x828161C8, 0x82282594},
      {0x828161D0, 0x822825A4}},
     {0x828161CC, 0x828161C4, 0x828161C8, 0x828161D0}, 4},
    {5, "sub_8227FC70 NetTaCourse::OpenMovie", "sub_8227FF10 NetTaCourse::WaitForMovie",
     "sub_82280218 NetTaCourse::Leave", "sub_8227FE70 NetTaCourse::Destroy", 37, 22593,
     WaitKind::kLatched, 33, kNoMovie, 0x828161C0, 100, 112, LeaveKind::kStateFirst, 0x82280218,
     EndKind::kDataBack, 0x828161BC, 128,
     {{0x828161B4, 0x8227FED4}, {0x828161B8, 0x8227FEE8}, {0x828161BC, 0x8227FEF8}},
     {0x828161B4, 0x828161B8, 0x828161BC}, 3},
    {6, "sub_82265920 Type12Screen6::OpenMovie", "sub_82265C00 Type12Screen6::WaitForMovie",
     "sub_82265DC0 Type12Screen6::Leave", "sub_82265B70 Type12Screen6::Destroy", 38, 22594,
     WaitKind::kEach, 38, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x82265DC0,
     EndKind::kBackAgain, 0, 112, {{0x82816154, 0x82265BC4}, {0x82816158, 0x82265BDC}},
     {0x82816154, 0x82816158}, 2},
    {7, "sub_822688A8 Type12Screen7::OpenMovie", "sub_82268F70 Type12Screen7::WaitForMovie",
     "sub_822697D8 Type12Screen7::Leave", "sub_82268EC8 Type12Screen7::Destroy", 39, 22596,
     WaitKind::kEach, 39, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x822697D8,
     EndKind::kBackRaceAgain, 0, 112,
     {{0x82816160, 0x82268F2C}, {0x82816164, 0x82268F40}, {0x82816168, 0x82268F54}},
     {0x82816160, 0x82816164, 0x82816168}, 3},
    {8, "sub_822609C0 Type12Screen8::OpenMovie", "sub_822611F8 Type12Screen8::WaitForMovie",
     "sub_82261358 Type12Screen8::Leave", "sub_82261168 Type12Screen8::Destroy", 42, 22598,
     WaitKind::kEach, 42, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x82261358,
     EndKind::kBackAgain, 0, 112, {{0x82816144, 0x822611BC}, {0x82816148, 0x822611D4}},
     {0x82816144, 0x82816148}, 2},
    {9, "sub_82263048 Type12Screen9::OpenMovie", "sub_822632C8 Type12Screen9::WaitForMovie",
     "sub_82263448 Type12Screen9::Leave", "sub_82263238 Type12Screen9::Destroy", 43, 22599,
     WaitKind::kEach, 43, kNoMovie, 0, 500, 96, LeaveKind::kStateAfter, 0x82263448,
     EndKind::kBackAgain, 0, 112, {{0x8281614C, 0x8226328C}, {0x82816150, 0x822632A4}},
     {0x8281614C, 0x82816150}, 2},
    {10, "sub_82287CD0 Type12Dialog10::OpenMovie", "sub_82287F40 Type12Dialog10::WaitForMovie",
     "sub_82288028 Type12Dialog10::Leave", "sub_82287EC0 Type12Dialog10::Destroy", 40, 22595,
     WaitKind::kEach, 40, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x82288028,
     EndKind::kBack, 0, 112, {{0x828161D4, 0x82287F00}, {0x828161D8, 0x82287F18}},
     {0x828161D4, 0x828161D8}, 2},
    {11, "sub_82276E40 Type12Dialog11::OpenMovie", "sub_82276FC0 Type12Dialog11::WaitForMovie",
     "sub_822770B8 Type12Dialog11::Leave", "sub_82276F40 Type12Dialog11::Destroy", 41, 22597,
     WaitKind::kEach, 41, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x822770B8,
     EndKind::kBack, 0, 112, {{0x82816188, 0x82276F80}, {0x8281618C, 0x82276F98}},
     {0x82816188, 0x8281618C}, 2},
    {12, "sub_8227ECE8 Type12Dialog12::OpenMovie", "sub_8227EEC0 Type12Dialog12::WaitForMovie",
     "sub_8227F038 Type12Dialog12::Leave", "sub_8227EE40 Type12Dialog12::Destroy", 17, 22560,
     WaitKind::kEach, 17, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x8227F038,
     EndKind::kBack, 0, 112, {{0x828161A8, 0x8227EE80}, {0x828161AC, 0x8227EE98}},
     {0x828161A8, 0x828161AC}, 2},
    {13, "sub_8227DEC0 Type12Dialog13::OpenMovie", "sub_8227E030 Type12Dialog13::WaitForMovie",
     "sub_8227E118 Type12Dialog13::Leave", "sub_8227DFB0 Type12Dialog13::Destroy", 16, 22559,
     WaitKind::kEach, 16, kNoMovie, 0, 0, 96, LeaveKind::kStateAfter, 0x8227E118,
     EndKind::kBack, 0, 112, {{0x8281619C, 0x8227DFF0}, {0x828161A0, 0x8227E008}},
     {0x8281619C, 0x828161A0}, 2},
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

struct Field {
  uint32_t offset;
  uint32_t value;
};
void Fields(uint8_t* base, uint32_t block, std::initializer_list<Field> fields) {
  for (const Field& f : fields) {
    Store(base, block + f.offset, f.value);
  }
}

uint64_t Alloc(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t bytes) {
  return calls.Call(kVirtualAlloc, frame, lr, 0, bytes, kMemCommit, kReadWrite);
}

// The page object's stores (+12 and +20 -1, the rest 0) in `order`.
void Pages(uint8_t* base, uint32_t pages, std::initializer_list<uint32_t> order) {
  for (const uint32_t offset : order) {
    Store(base, pages + offset, offset == 12 || offset == 20 ? kNone : 0);
  }
}
constexpr std::initializer_list<uint32_t> kPagesOrder = {4, 0, 8, 20, 12, 24};

// A menu's stores: `head` (its +12, +16, +20, +0, +4 and +8 in the
// original's order), then +40, +48, +52, +36, +64, +24, +28 and +32 = 0.
void Menu(uint8_t* base, uint32_t menu, std::initializer_list<Field> head) {
  Fields(base, menu, head);
  for (const uint32_t offset : {40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    Store(base, menu + offset, 0);
  }
}
// The usual head: +12 0, +16 `count`, +20 `flag`, +0, +4, +8 0.
void MenuCountFirst(uint8_t* base, uint32_t menu, uint32_t count, uint32_t flag) {
  Menu(base, menu, {{12, 0}, {16, count}, {20, flag}, {0, 0}, {4, 0}, {8, 0}});
}
// +12 0, +20 `flag`, +0, +4 0, +16 `count`, +8 0.
void MenuCountAfter4(uint8_t* base, uint32_t menu, uint32_t count, uint32_t flag) {
  Menu(base, menu, {{12, 0}, {20, flag}, {0, 0}, {4, 0}, {16, count}, {8, 0}});
}

// The machine's state once set up: 1 when it opens on its own message
// (+9612), else 9.
void Opened(uint8_t* base, uint32_t machine) {
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
}
// Once its movie is loaded: 9 (its open message) or 4.
void Loaded(uint8_t* base, uint32_t machine) {
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 9 : 4);
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
    Store(base, machine + kMachineLoaded, uint32_t(r3));
    if (s.print_r3 != 0) {
      r3 = s.print_r3;  // the print's first argument
    }
    if (Load(base, machine + kMachineLoaded) != 0) {
      Loaded(base, machine);
    }
    return r3;
  }
  if (Load(base, machine + kMachineLoaded) == 0) {
    r3 = ui_movies::IsLoaded(base, s.wait_movie);
    if (uint32_t(r3) != 0 && s.wait_movie2 != kNoMovie) {
      r3 = ui_movies::IsLoaded(base, s.wait_movie2);
    }
    if (uint32_t(r3) != 0) {
      Store(base, machine + kMachineLoaded, 1);
    }
  }
  if (s.counter != 0) {
    // The print when the counter over 15 is even.
    if (((Load(base, s.counter) / 15) & 1) == 0) {
      r3 = s.print_r3;
    }
    Store(base, s.counter, Load(base, s.counter) + 1);
  }
  if (Load(base, machine + kMachineLoaded) != 0) {
    Loaded(base, machine);
  }
  return r3;
}

// Type12Screen1's Leave: its two movies, the data's three pointer lists
// (+20, +280, +540, each count at +256 from it) emptied from the end.
uint64_t Screen1Leave(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822727E0 Type12Screen1::Leave");
  constexpr uint32_t kData = 0x82816180;
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, machine + kMachineState, 7);
  Finish(base, calls, frame, 0x82272828, 33);
  Finish(base, calls, frame, 0x8227284C, 34);
  for (const auto& [list, lr] : {std::pair{20u, 0x82272880u}, std::pair{280u, 0x822728B8u},
                                  std::pair{540u, 0x822728F0u}}) {
    for (;;) {
      const uint32_t at = Load(base, kData) + list;
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
  PostRing(base, dispatcher + 204, 4);
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
    Store(base, machine + kMachineState, 7);
    Finish(base, calls, frame, s.leave_at + 0x48, s.movie);
    r3 = calls.Call(kMoviesDelete, frame, s.leave_at + 0x50, s.movie);
  } else {
    Finish(base, calls, frame, s.leave_at + 0x44, s.movie);
    r3 = calls.Call(kMoviesDelete, frame, s.leave_at + 0x4C, s.movie);
    Store(base, machine + kMachineLoaded, 0);
    Store(base, machine + kMachineState, 7);
  }
  PostRing(base, dispatcher + 204, 4);
  return r3;
}

template <size_t I>
uint64_t DestroyOf(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  constexpr const Spec& s = kSpecs[I];
  ITC_NATIVE_RUNNING(s.destroy_name);
  const uint32_t frame = Frame(base, sp, s.destroy_frame);
  auto is = [&](uint32_t field) { return Load(base, machine + field) == 1; };
  auto state = [&](uint32_t value) { Store(base, machine + kMachineState, value); };
  switch (s.end) {
    case EndKind::kDataBack:
    case EndKind::kDataBackChecked: {
      const uint32_t data = Load(base, s.data);
      if ((s.end == EndKind::kDataBack || data != 0) && Load(base, data + 4) != 0) {
        state(14);
      }
      if (is(kMachineBack)) {
        state(14);
      } else if (is(kMachineAgain)) {
        state(15);
      }
      break;
    }
    case EndKind::kBackAgain:
      if (is(kMachineBack)) {
        state(14);
      } else if (is(kMachineAgain)) {
        state(15);
      }
      break;
    case EndKind::kBackRaceAgain:
      if (is(kMachineBack)) {
        state(14);
      } else if (is(kMachineRace)) {
        state(11);
      } else if (is(kMachineAgain)) {
        state(15);
      }
      break;
    case EndKind::kBack:
      if (is(kMachineBack)) {
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
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x8227C3C4, 28));
  Store(base, 0x82816190, pages);
  Pages(base, pages, kPagesOrder);
  const uint32_t menu = uint32_t(Alloc(calls, frame, 0x8227C400, 68));
  Store(base, 0x82816194, menu);
  MenuCountFirst(base, menu, 1, 1);
  const uint64_t r3 = Alloc(calls, frame, 0x8227C45C, 36);
  const uint32_t data = uint32_t(r3);
  Store(base, 0x82816198, data);
  Fields(base, data,
         {{0, 0}, {4, 0}, {20, kNone}, {24, kNone}, {8, 0}, {12, 0}, {16, 0}, {28, 0}, {32, 1}});
  Fields(base, machine, {{kMachineBack, 0}, {kMachineRace, 0}, {kMachineAgain, 0}});
  Opened(base, machine);
  return r3;
}

uint64_t Screen1Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822720A8 Type12Screen1::Create");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x822720D4, 28));
  Store(base, 0x8281616C, pages);
  Pages(base, pages, {4, 0, 8, 24, 20, 12});
  struct MenuSpec {
    uint32_t global, lr, count;
  };
  for (const MenuSpec& m : {MenuSpec{0x82816170, 0x82272110, 2}, MenuSpec{0x82816174, 0x82272170, 2},
                            MenuSpec{0x82816178, 0x822721C8, 1},
                            MenuSpec{0x8281617C, 0x82272220, 1}}) {
    const uint32_t menu = uint32_t(Alloc(calls, frame, m.lr, 68));
    Store(base, m.global, menu);
    MenuCountFirst(base, menu, m.count, 1);
  }
  const uint32_t data = uint32_t(Alloc(calls, frame, 0x82272274, 2820));
  Store(base, 0x82816180, data);
  const uint64_t r3 = Screen1ResetData(base, data, frame, calls);
  Fields(base, machine, {{kMachineBack, 0}, {kMachineRace, 0}, {kMachineAgain, 0}});
  Opened(base, machine);
  return r3;
}

uint64_t Screen3Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225DFE8 Type12Screen3::Create");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x8225E014, 28));
  Store(base, 0x828160E0, pages);
  Pages(base, pages, kPagesOrder);
  const uint32_t menu = uint32_t(Alloc(calls, frame, 0x8225E050, 68));
  Store(base, 0x828160E4, menu);
  MenuCountAfter4(base, menu, 3, 1);
  const uint64_t r3 = Alloc(calls, frame, 0x8225E0B0, 24);
  const uint32_t data = uint32_t(r3);
  Store(base, 0x828160E8, data);
  Fields(base, data, {{0, 0}, {4, 0}, {8, kNone}, {12, kNone}, {16, 0}, {20, 0}});
  Fields(base, machine, {{kMachineBack, 0}, {kMachineRace, 0}, {kMachineAgain, 0}});
  Opened(base, machine);
  return r3;
}

uint64_t RankingCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82282358 NetRanking::Create");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t data = uint32_t(Alloc(calls, frame, 0x82282384, 108));
  Store(base, 0x828161D0, data);
  Fields(base, data,
         {{0, 0},      {4, 0},      {8, 0},      {48, kNone}, {76, 0},  {52, 0},
          {64, 0},     {60, 0},     {24, kNone}, {20, kNone}, {28, 0},  {100, 0},
          {96, 0},     {92, 0},     {32, 0},     {104, kNone}, {36, 0}, {12, 0},
          {40, 1},     {44, 0},     {80, kNone}, {16, 0}});
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x82282404, 28));
  Store(base, 0x828161C4, pages);
  Pages(base, pages, kPagesOrder);
  const uint32_t menu = uint32_t(Alloc(calls, frame, 0x8228243C, 68));
  Store(base, 0x828161C8, menu);
  MenuCountAfter4(base, menu, 7, 1);
  const uint64_t r3 = Alloc(calls, frame, 0x82282498, 68);
  Store(base, 0x828161CC, uint32_t(r3));
  MenuCountFirst(base, uint32_t(r3), 1, 0);
  Fields(base, machine, {{kMachineBack, 0}, {kMachineRace, 0}, {kMachineAgain, 0}});
  Opened(base, machine);
  return r3;
}

uint64_t TaCourseCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227FD40 NetTaCourse::Create");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t data = uint32_t(Alloc(calls, frame, 0x8227FD6C, 48));
  Store(base, 0x828161BC, data);
  Fields(base, data,
         {{0, 0}, {4, 0}, {8, 0}, {20, 1}, {24, 0}, {12, 0}, {16, 0}, {28, 0}, {32, 0},
          {40, kNone}, {44, kNone}});
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x8227FDC0, 28));
  Store(base, 0x828161B4, pages);
  Pages(base, pages, kPagesOrder);
  const uint64_t r3 = Alloc(calls, frame, 0x8227FDF8, 68);
  Store(base, 0x828161B8, uint32_t(r3));
  MenuCountAfter4(base, uint32_t(r3), 7, 1);
  Fields(base, machine, {{kMachineBack, 0}, {kMachineRace, 0}, {kMachineAgain, 0}});
  Opened(base, machine);
  return r3;
}

// Screen 6 and the dialogs: the page object and the menu allocated first,
// then set up through their globals.
uint64_t PagesAndMenu(uint8_t* base, uint32_t machine, uint32_t frame, const Calls& calls,
                      uint32_t pages_global, uint32_t pages_lr, uint32_t menu_lr, uint32_t count,
                      uint32_t flag) {
  Store(base, machine + kMachineLoaded, 0);
  Store(base, pages_global, uint32_t(Alloc(calls, frame, pages_lr, 28)));
  const uint64_t r3 = Alloc(calls, frame, menu_lr, 68);
  Store(base, pages_global + 4, uint32_t(r3));
  Pages(base, Load(base, pages_global), kPagesOrder);
  MenuCountFirst(base, Load(base, pages_global + 4), count, flag);
  return r3;
}

uint64_t Screen6Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82265A88 Type12Screen6::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t r3 =
      PagesAndMenu(base, machine, frame, calls, 0x82816154, 0x82265AB4, 0x82265AD0, 2, 1);
  Store(base, 0x8281615C, 0);
  Store(base, machine + 9656, 0);
  Opened(base, machine);
  return r3;
}

uint64_t Screen7Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82268D18 Type12Screen7::Create");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, sp + 20, machine);
  Store(base, machine + kMachineLoaded, 0);
  Store(base, 0x82816160, uint32_t(Alloc(calls, frame, 0x82268D50, 28)));
  Store(base, 0x82816164, uint32_t(Alloc(calls, frame, 0x82268D6C, 68)));
  Store(base, frame + 88, kGlobalsPage);
  const uint64_t r3 = Alloc(calls, frame, 0x82268D8C, 8);
  const uint32_t pages = Load(base, 0x82816160);
  Store(base, frame + 80, 0);
  Store(base, frame + 92, kGlobalsPage);
  Store(base, frame + 84, kNone);
  Store(base, 0x82816168, uint32_t(r3));
  Pages(base, pages, {12, 4, 0, 8, 20, 24});
  MenuCountFirst(base, Load(base, Load(base, frame + 88) + 0x6164), 3, 1);
  const uint32_t data = Load(base, Load(base, frame + 92) + 0x6168);
  Fields(base, data, {{0, 0}, {4, 0}});
  const uint32_t m = Load(base, sp + 20);
  Fields(base, m,
         {{9720, 0}, {9656, 0}, {9660, 0}, {9664, 0}, {9668, 0}, {9672, 0}, {9676, 0},
          {9680, 0}, {9640, 0}, {9644, 1}, {9648, 0}, {9652, 1}, {9684, 0}, {9688, 0},
          {9692, 0}, {9696, 0}, {9700, 0}, {9704, 0}, {9712, 0}, {9708, 0}, {9716, 0},
          {9724, 1}, {kMachineBack, 0}, {kMachineRace, 0}, {9636, 0}, {9728, 0}});
  Opened(base, m);
  return r3;
}

uint64_t Screen8Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82261030 Type12Screen8::Create");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, sp + 20, machine);
  Store(base, machine + kMachineLoaded, 0);
  Store(base, 0x82816144, uint32_t(Alloc(calls, frame, 0x82261068, 28)));
  const uint64_t r3 = Alloc(calls, frame, 0x82261084, 68);
  const uint32_t pages = Load(base, 0x82816144);
  Store(base, frame + 80, 0);
  Store(base, frame + 84, kNone);
  Store(base, frame + 88, kNone);
  Store(base, frame + 92, kGlobalsPage);
  Store(base, 0x82816148, uint32_t(r3));
  Fields(base, pages,
         {{4, 0}, {0, 0}, {24, 0}, {8, 0}, {20, Load(base, frame + 84)},
          {12, Load(base, frame + 88)}});
  MenuCountFirst(base, Load(base, Load(base, frame + 92) + 0x6148), 0, 1);
  const uint32_t m = Load(base, sp + 20);
  Fields(base, m,
         {{9640, 0}, {9644, 0}, {kMachineBack, 0}, {kMachineRace, 0}, {kMachineAgain, 0},
          {9660, 1}, {9664, 1}, {9656, 0}, {9648, 0}, {9652, 0}});
  Opened(base, m);
  return r3;
}

uint64_t Screen9Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822630C0 Type12Screen9::Create");
  const uint32_t frame = Frame(base, sp, 144);
  Store(base, sp + 20, machine);
  Store(base, machine + kMachineLoaded, 0);
  Store(base, 0x8281614C, uint32_t(Alloc(calls, frame, 0x822630F8, 28)));
  const uint64_t r3 = Alloc(calls, frame, 0x82263114, 68);
  const uint32_t pages = Load(base, 0x8281614C);
  Fields(base, frame, {{80, 0}, {92, 0}, {100, 0}, {84, kNone}, {88, kNone}, {104, 0},
                       {96, kGlobalsPage}});
  Store(base, 0x82816150, uint32_t(r3));
  Fields(base, pages,
         {{4, 0}, {0, 0}, {8, 0}, {20, Load(base, frame + 84)}, {12, Load(base, frame + 88)},
          {24, 0}});
  Menu(base, Load(base, Load(base, frame + 96) + 0x6150),
       {{12, 0}, {20, 1}, {16, 0}, {0, 0}, {4, 0}, {8, 0}});
  const uint32_t m = Load(base, sp + 20);
  Fields(base, m,
         {{9640, 1}, {9644, 1}, {9648, 0}, {9652, 0}, {9656, 0}, {9660, 0}, {9664, 0},
          {9668, 0}, {9672, 0}, {9676, 0}, {kMachineBack, 0}, {kMachineRace, 0},
          {kMachineAgain, 0}, {9680, 1}, {9684, 0}, {9688, 1}, {9692, 0}});
  Opened(base, m);
  return r3;
}

uint64_t Dialog10Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82287DD8 Type12Dialog10::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t r3 =
      PagesAndMenu(base, machine, frame, calls, 0x828161D4, 0x82287E04, 0x82287E20, 0, 1);
  Fields(base, machine, {{9644, 0}, {9640, 0}});
  Store(base, 0x828161DC, 0);
  Opened(base, machine);
  return r3;
}

uint64_t Dialog11Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82276E60 Type12Dialog11::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t r3 =
      PagesAndMenu(base, machine, frame, calls, 0x82816188, 0x82276E8C, 0x82276EA8, 1, 1);
  Fields(base, machine, {{kMachineBack, 0}, {kMachineRace, 0}, {9640, 0}});
  Opened(base, machine);
  return r3;
}

uint64_t Dialog12Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227ED58 Type12Dialog12::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t r3 =
      PagesAndMenu(base, machine, frame, calls, 0x828161A8, 0x8227ED84, 0x8227EDA0, 0, 1);
  Fields(base, machine, {{9640, 0}, {9644, 0}});
  Store(base, 0x828161B0, 0);
  Opened(base, machine);
  return r3;
}

uint64_t Dialog13Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227DED0 Type12Dialog13::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t r3 =
      PagesAndMenu(base, machine, frame, calls, 0x8281619C, 0x8227DEFC, 0x8227DF18, 0, 1);
  Store(base, machine + 9640, 0);
  Store(base, 0x828161A4, 0);
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
