// Kind 6's screen after the race (Kind6Menu::, docs/functions/
// race-screens-unseen.md): its update with the pages of its two page
// objects, the helpers only they reach, and the network manager's small
// functions they call; see unseen.h. Read from the generated code; never
// seen in play (ported by the user's call, 2026-11-12). The Xbox Live work
// (the network manager's state machine sub_8214B330 and the session's
// start sub_822917B8) stays recompiled, called through `calls`; the
// leaderboard read sub_82292CC0 is answered as no Live (the user's call,
// 2026-11-14).

#include "race_screens/unseen.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "user/user.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_screens {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint64_t kMinusOne = ~uint64_t(0);

// The guest calls the pages make, through their overrides or originals.
constexpr uint32_t kShowNode = 0x8211AC00;
constexpr uint32_t kHideNode = 0x8211AC48;
constexpr uint32_t kSetNodeCharText = 0x8211C8A0;
constexpr uint32_t kMenuStep = 0x82186568;
constexpr uint32_t kMenuPressed = 0x821864C8;
constexpr uint32_t kSlotState = 0x8228F9E8;
constexpr uint32_t kSaveBuffersDestroy = 0x8228F8F8;
constexpr uint32_t kGetLocalTime = 0x822968B8;
constexpr uint32_t kNetSetState = 0x8214B330;
constexpr uint32_t kSessionStart = 0x822917B8;
constexpr uint32_t kOperatorDeleteArray = 0x825A71C0;
// The "returns 0" stub in the pages' tables (vehicle-accessors.md).
constexpr uint32_t kReturns0 = 0x825C8388;

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

uint64_t Register(uint32_t address) { return uint64_t(int64_t(int32_t(address))); }

uint32_t Screen(uint8_t* base) { return Get(base, kKind6Menu); }
uint32_t Field(uint8_t* base, uint32_t field) { return guest::LoadU32(base, Screen(base) + field); }
void SetField(uint8_t* base, uint32_t field, uint32_t value) {
  guest::StoreU32(base, Screen(base) + field, value);
}
uint32_t Session(uint8_t* base) { return Get(base, kKind6Session); }

uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return (uint64_t(guest::LoadU32(base, address)) << 32) | guest::LoadU32(base, address + 4);
}

// ScreenPages::Go on page object A (+40) or B (+208), the screen read again.
void GoA(uint8_t* base, uint32_t page, uint32_t transition = kNone, uint32_t queued = kNone) {
  PagesGo(base, Screen(base) + 40, page, transition, queued);
}
void GoB(uint8_t* base, uint32_t page, uint32_t transition = kNone, uint32_t queued = kNone) {
  PagesGo(base, Screen(base) + 208, page, transition, queued);
}

uint64_t Sound(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t id) {
  return calls.Call(kMenuSoundPlay, frame, lr, id);
}

// Menu::Pressed(menu, action) answered nonzero (its r3 compared as a word).
bool Pressed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t menu, uint32_t action) {
  return int32_t(uint32_t(calls.Call(kMenuPressed, frame, lr, menu, action))) != 0;
}

// The leaderboard results [0x826C6AD4] (an XUSER_STATS_READ_RESULTS): its
// view (+4), whose +4 is the total row count, +8 the rows read, +12 the
// 48-byte rows.
uint32_t Results(uint8_t* base) { return Get(base, kKind6Results); }

}  // namespace

// --- The network manager's small functions ---

uint64_t Kind6Rank(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822914A8 Leaderboard::Rank");
  const uint32_t results = Results(base);
  if (results == 0) return kMinusOne;
  const uint32_t view = guest::LoadU32(base, results + 4);
  const uint32_t count = guest::LoadU32(base, view + 8);
  uint32_t rank = 0;
  if (count != 0) {
    const uint32_t rows = guest::LoadU32(base, view + 12);
    const uint64_t xuid = LoadU64(base, Session(base) + 168);
    for (uint32_t i = 0; i < count; i++) {
      if (LoadU64(base, rows + 48 * i) == xuid) {
        rank = guest::LoadU32(base, rows + 48 * i + 8);
        break;
      }
    }
  }
  return uint64_t(rank) - 1;
}

uint32_t Kind6CarLogo(uint32_t car) {
  ITC_NATIVE_RUNNING("sub_8225FFC8 Leaderboard::CarLogo");
  switch (int32_t(car)) {
    case 5: return 7;
    case 6: return 6;
    case 8: return 8;
    case 16: return 9;
    case 18: return 23;
    case 20: return 25;
    case 24: return 13;
    case 25: return 10;
    case 31: return 21;
    case 32: return 11;
    case 34: return 12;
    case 35: return 14;
    case 39: return 3;
    case 44: return 15;
    case 50: return 16;
    case 53: return 1;
    case 56: return 2;
    case 71: return 17;
    case 74: return 18;
    case 75: return 20;
    case 76: return 19;
    case 90: return 22;
    case 91: return 4;
    case 101: return 5;
    case 103: return 24;
    default: return 0;
  }
}

uint64_t Kind6NetState(uint8_t* base, uint64_t state, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822927B0 Network::SetLiveState");
  const uint32_t frame = Frame(base, sp, 96);
  switch (int32_t(uint32_t(state))) {
    case 4109:
      return calls.Call(kNetSetState, frame, 0x82292818, 4109);
    case 4111:
      calls.Call(kNetSetState, frame, 0x82292800, 4111);
      return calls.Call(kSessionStart, frame, 0x8229280C, Session(base));
    case 4112: {
      const uint64_t r3 = calls.Call(kNetSetState, frame, 0x822927D8, 4112);
      const uint32_t session = Session(base);
      guest::StoreU32(base, kKind6ReadSpec, guest::LoadU32(base, session + 188));
      guest::StoreU32(base, session + 180, 0);
      return r3;
    }
    default:
      return state;
  }
}

uint32_t Kind6ReadSpecSetUp(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82292648 Leaderboard::SetUpSpec");
  // The spec's view id from [0x826C72F4]; for ids 1 to 16 (their sixteen
  // jump table cases, each sub_82292690) three columns, ids 1, 2 and 3.
  const uint32_t view = guest::LoadU32(base, kKind6ReadSpec);
  guest::StoreU32(base, kKind6StatsSpec, view);
  if (view - 1 <= 15) {
    guest::StoreU32(base, kKind6StatsSpec + 4, 3);
    guest::StoreU16(base, kKind6StatsSpec + 8, 1);
    guest::StoreU16(base, kKind6StatsSpec + 10, 2);
    guest::StoreU16(base, kKind6StatsSpec + 12, 3);
  }
  return kKind6StatsSpec;
}

uint64_t Kind6LeaderboardRead(uint8_t* base, uint32_t session, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82292CC0 Leaderboard::Read");
  const uint32_t frame = Frame(base, sp, 144);
  // Not signed in to Xbox Live (XUserGetSigninState of the player's
  // controller isn't 2 for any user today): the read's state 4.
  if (user::SigninState(Get(base, kControllerEntries)) != 2) {
    guest::StoreU32(base, session + 180, 4);
  }
  // The old results freed (operator delete[]).
  const uint32_t results = Get(base, kKind6Results);
  if (results != 0) {
    if (!memory::crt::OperatorDeleteArray(results)) {
      calls.Call(kOperatorDeleteArray, frame, 0x82292D08, results);
    }
    guest::StoreU32(base, kKind6Results, 0);
  }
  Kind6ReadSpecSetUp(base);
  // The stats enumerator (XUserCreateStatsEnumeratorByRank or ByXuid), its
  // buffer into [0x826C6AD4] and XEnumerate: answered as no Live (the
  // user's call, 2026-11-14): the read failed, no buffer, the state 4. The
  // original's r3 there isn't read by its callers.
  guest::StoreU32(base, session + 180, 4);
  return 0;
}

uint64_t Kind6SaveReplay(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D678 Kind6Menu::SaveReplay");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t race = Get(base, kRaceData);
  const uint32_t selection = ListSelection(base, Screen(base) + 136);
  const uint32_t records = race + 0x59AE8;
  // The slot: the list's scroll (+4) and selection.
  const uint32_t slot = Field(base, 140) + selection;
  vehicle_accessors::RacerRecordsAt(base, records, kNone);
  // The 36-byte record in the frame: cleared, the date and time
  // (GetLocalTime), +24 the race data's +60, +16 the last result's time.
  const uint32_t record = frame + 80;
  for (uint32_t k = 0; k < 36; k += 4) guest::StoreU32(base, record + k, 0);
  const uint32_t replay = Get(base, kRaceReplay);
  calls.Call(kGetLocalTime, frame, 0x8221D6E8, record);
  guest::StoreU32(base, record + 24, guest::LoadU32(base, race + 60));
  guest::StoreU32(base, record + 16,
                  ai_drivers::TimeFromTicks(guest::LoadU32(
                      base, vehicle_accessors::RacerRecordsAt(base, records, kNone) + 3932)));
  // By the race kind: +20 the kind's index, +32 a value of the last result.
  switch (int32_t(guest::LoadU32(base, race + 0xA3AF0))) {
    case 1:
      guest::StoreU32(base, record + 32,
                      guest::LoadU32(base, vehicle_accessors::RacerRecordsAt(base, records, kNone) +
                                               980));
      guest::StoreU32(base, record + 20, 0);
      break;
    case 2:
      guest::StoreU32(base, record + 32,
                      guest::LoadU32(base, vehicle_accessors::RacerRecordsAt(base, records, kNone) +
                                               3892));
      guest::StoreU32(base, record + 20, 1);
      break;
    case 4:
      guest::StoreU32(base, record + 32, 0);
      guest::StoreU32(base, record + 20, 2);
      break;
    case 6:
      guest::StoreU32(base, record + 32,
                      guest::LoadU32(base, vehicle_accessors::RacerRecordsAt(base, records, kNone) +
                                               3892));
      guest::StoreU32(base, record + 20, 3);
      break;
    default:
      break;
  }
  return RaceDialogAskSlot(base, slot, record, replay, frame, calls);
}

// --- The helpers ---

uint64_t Kind6ShowPanelB(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221CEA0 Kind6Menu::ShowPanelB");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Field(base, 24);
  calls.Call(kShowNode, frame, 0x8221CED0, movie, 1, 4);
  calls.Call(kShowNode, frame, 0x8221CEE0, movie, 1, 5);
  uint32_t a = ui_movies::FindNode(base, movie, 1, 4);
  uint32_t b = ui_movies::FindNode(base, movie, 1, 5);
  if (a == 0 || b == 0) return b;
  guest::StoreU32(base, a + 108, 0);
  guest::StoreU32(base, b + 108, 0);
  calls.Call(kShowNode, frame, 0x8221CF34, movie, 1, 6);
  calls.Call(kShowNode, frame, 0x8221CF44, movie, 1, 7);
  a = ui_movies::FindNode(base, movie, 1, 6);
  b = ui_movies::FindNode(base, movie, 1, 7);
  if (a == 0 || b == 0) return b;
  guest::StoreU32(base, a + 108, 1);
  guest::StoreU32(base, b + 108, 1);
  calls.Call(kHideNode, frame, 0x8221CF98, movie, 1, 8);
  return calls.Call(kHideNode, frame, 0x8221CFA8, movie, 1, 9);
}

namespace {

// UiMovie::SetText(movie, -1, 17, 528) with the screen's time (+4: minutes,
// seconds, thousandths), r10 0 (the user's call for the parking area).
uint64_t ShowTime(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr,
                  uint32_t movie) {
  const uint32_t time = Field(base, 4);
  const uint64_t args[8] = {movie, kMinusOne, 17, 528, (time >> 16) & 0x7FFF, (time >> 10) & 0x3F,
                            time & 0x3FF, 0};
  return calls.CallArgs(kSetText, frame, lr, args);
}

// UiMovie::SetNodeText(movie, node, id, r6 to r8), r9 and r10 0.
uint64_t NodeText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint32_t node,
                  uint64_t id, uint64_t r6 = 0, uint64_t r7 = 0, uint64_t r8 = 0) {
  const uint64_t args[8] = {movie, node, id, r6, r7, r8, 0, 0};
  return calls.CallArgs(kSetNodeText, frame, lr, args);
}

}  // namespace

uint64_t Kind6ShowTimeOnly(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D3D0 Kind6Menu::ShowTimeOnly");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Field(base, 24);
  if (int32_t(Field(base, 316)) != 0) calls.Call(kHideNode, frame, 0x8221D404, movie, kMinusOne, 16);
  calls.Call(kShowNode, frame, 0x8221D414, movie, kMinusOne, 17);
  ShowTime(base, calls, frame, 0x8221D444, movie);
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 5; i++) {
    const uint32_t node = ui_movies::FindNode(base, movie, kNone, i + 23);
    guest::StoreU32(base, node + 80, 1);
    NodeText(calls, frame, 0x8221D47C, movie, ui_movies::Child(base, node, 0), 533);
    NodeText(calls, frame, 0x8221D498, movie, ui_movies::Child(base, node, 1), 534);
    const uint32_t mark = ui_movies::Child(base, ui_movies::Child(base, node, 2), 0);
    guest::StoreU32(base, mark + 108, 0);
    r3 = NodeText(calls, frame, 0x8221D4D4, movie, ui_movies::Child(base, node, 6), 531);
  }
  return r3;
}

uint64_t Kind6RankingTime(uint8_t* base, uint64_t row, uint32_t minutes, uint32_t seconds,
                          uint32_t thousandths) {
  ITC_NATIVE_RUNNING("sub_8221D808 Kind6Menu::RankingTime");
  const uint32_t results = Results(base);
  uint64_t value = 0;
  if (results != 0) {
    const uint32_t rows = guest::LoadU32(base, guest::LoadU32(base, results + 4) + 12);
    const uint32_t columns = guest::LoadU32(base, uint32_t(rows + row * 48) + 44);
    value = LoadU64(base, columns + 16);
  }
  if (value == 0) return row;
  const uint64_t total = value / 1000;
  guest::StoreU32(base, thousandths, uint32_t(value - total * 1000));
  const uint32_t mins = uint32_t(total / 60);
  guest::StoreU32(base, seconds, uint32_t(total - (total / 60) * 60));
  guest::StoreU32(base, minutes, mins);
  if (mins >= 60) {
    guest::StoreU32(base, minutes, 59);
    guest::StoreU32(base, seconds, 59);
    guest::StoreU32(base, thousandths, 999);
  }
  return row;
}

uint64_t Kind6ShowRanking(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221E1A0 Kind6Menu::ShowRanking");
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t movie = Field(base, 24);
  const uint64_t rank64 = Kind6Rank(base);
  const int32_t rank = int32_t(uint32_t(rank64));
  auto total = [&]() -> int32_t {
    const uint32_t results = Results(base);
    if (results == 0) return -1;
    return int32_t(guest::LoadU32(base, guest::LoadU32(base, results + 4) + 4));
  };
  const int32_t rows = total();
  int32_t row = rank >= 2 ? rank - 2 : 0;
  // The first slot shown: fewer than 5 rows leave the slots above the
  // player's empty; no rows (or none read) show nothing.
  uint32_t first;
  if (rows >= 5) {
    first = 0;
  } else if (rows == 1) {
    first = 2;
  } else if (rows == 2) {
    first = (rank == 0 ? 1u : 0u) + 1;
  } else if (rows == 3 || rows == 4) {
    first = rank == 0 ? 2u : (rank - 1 == 0 ? 1u : 0u);
  } else {
    return rank64;
  }
  if (int32_t(Field(base, 316)) != 0) {
    calls.Call(kShowNode, frame, 0x8221E274, movie, kMinusOne, 16);
  }
  calls.Call(kShowNode, frame, 0x8221E284, movie, kMinusOne, 17);
  uint64_t r3 = ShowTime(base, calls, frame, 0x8221E2B4, movie);
  if (rank == -1 || rows == -1 || first >= 5) return r3;
  // A row's entry in the results read again, as the original does.
  auto entry = [&]() -> uint32_t {
    return guest::LoadU32(base, guest::LoadU32(base, Results(base) + 4) + 12) +
           48 * uint32_t(row);
  };
  for (uint32_t slot = first; slot < 5; slot++) {
    if ((rank == 0 && slot <= 1) || (rank == 1 && slot == 0)) continue;
    if (row >= total()) return r3;
    const uint32_t node = ui_movies::FindNode(base, movie, kNone, slot + 23);
    guest::StoreU32(base, node + 80, 1);
    const uint32_t child0 = ui_movies::Child(base, node, 0);
    r3 = child0;
    const uint32_t place = Results(base) != 0 ? guest::LoadU32(base, entry() + 8) : kNone;
    if (int32_t(place) == -1) return r3;
    NodeText(calls, frame, 0x8221E3A4, movie, child0, 8, place);
    const uint32_t child1 = ui_movies::Child(base, node, 1);
    r3 = child1;
    const uint32_t tag = Results(base) != 0 ? entry() + 24 : 0;
    if (tag == 0) return r3;
    r3 = calls.Call(kSetNodeCharText, frame, 0x8221E408, movie, child1, tag);
    auto car = [&]() -> uint32_t {
      return Results(base) != 0 ? guest::LoadU32(base, guest::LoadU32(base, entry() + 44) + 40)
                                : kNone;
    };
    if (int32_t(car()) == -1) return r3;
    const uint32_t mark = ui_movies::Child(base, ui_movies::Child(base, node, 2), 0);
    const uint32_t logo = Kind6CarLogo(car());
    guest::StoreU32(base, frame + 80, 0);
    guest::StoreU32(base, frame + 84, 0);
    guest::StoreU32(base, frame + 88, 0);
    guest::StoreU32(base, mark + 108, logo);
    r3 = Kind6RankingTime(base, uint64_t(int64_t(row)), frame + 80, frame + 84, frame + 88);
    const uint32_t mins = guest::LoadU32(base, frame + 80);
    const uint32_t secs = guest::LoadU32(base, frame + 84);
    const uint32_t ms = guest::LoadU32(base, frame + 88);
    if (mins == 0 && secs == 0 && ms == 0) return r3;
    r3 = NodeText(calls, frame, 0x8221E4F0, movie, ui_movies::Child(base, node, 6), 528, mins,
                  secs, ms);
    row++;
  }
  return r3;
}

// --- Page object A's pages ---

uint64_t Kind6MainPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221E788 Kind6Menu::MainPage");
  const uint32_t frame = Frame(base, sp, 96);
  if (int32_t(Field(base, 340)) == 0) return 0;
  calls.Call(kMenuStep, frame, 0x8221E7B4, Screen(base) + 68);
  if (Pressed(calls, frame, 0x8221E7C4, Screen(base) + 68, 159)) {
    Kind6NetState(base, 4109, frame, calls);
    Sound(calls, frame, 0x8221E7DC, 0);
    const uint32_t cursor = Field(base, 68);
    if (cursor <= 1) {
      GoB(base, 0);
      GoA(base, 1);
    } else if (cursor == 2) {
      GoA(base, 3);
      GoB(base, 0);
      calls.Call(kSaveBuffersCreate, frame, 0x8221E864);
      RaceDialogStartSave(base, frame, calls);
    } else if (cursor == 3) {
      RaceDialogOpen(base, 2, 0, frame, calls);
      GoA(base, 13);
      GoB(base, 0);
    }
  } else if (Pressed(calls, frame, 0x8221E8A0, Screen(base) + 68, 158)) {
    GoA(base, 0, 7);
  } else if (int32_t(Field(base, 76)) != 0) {
    GoA(base, 0, 3);
    Sound(calls, frame, 0x8221E8EC, 4);
  }
  return 0;
}

uint64_t Kind6AnswerPage(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8221C1E0 Kind6Menu::AnswerPage");
  const uint32_t cursor = Field(base, 68);
  uint32_t answer = 3;
  if (cursor <= 1 || cursor == 3) {
    answer = cursor == 0 ? 1 : (cursor == 1 ? 2 : 3);
    guest::StoreU32(base, Session(base) + 240, cursor == 3 ? 1 : 0);
  }
  guest::StoreU32(base, kKind6Word, answer);
  return answer;
}

uint64_t Kind6SlotPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221DA60 Kind6Menu::SlotPage");
  const uint32_t frame = Frame(base, sp, 96);
  if (int32_t(uint32_t(calls.Call(kSlotState, frame, 0x8221DA78, 0))) == 1) {
    RaceDialogShowSaving(base, frame, calls);
    GoA(base, 5);
  }
  calls.Call(kListUpdate, frame, 0x8221DAAC, Screen(base) + 136);
  if (int32_t(uint32_t(calls.Call(kListAccepted, frame, 0x8221DAB8, Screen(base) + 136))) != 0) {
    GoA(base, 4);
    Kind6SaveReplay(base, frame, calls);
  } else if (int32_t(uint32_t(calls.Call(kListCancelled, frame, 0x8221DAE8,
                                         Screen(base) + 136))) != 0) {
    GoA(base, 0, 5);
  } else if (int32_t(Field(base, 160)) != 0 || Field(base, 140) - Field(base, 144) != 0) {
    GoA(base, 2, 6);
  }
  return 0;
}

uint64_t Kind6SavePage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C238 Kind6Menu::SavePage");
  const uint32_t frame = Frame(base, sp, 96);
  const int32_t answer = int32_t(uint32_t(RaceDialogUpdate(base, r4, frame, calls)));
  if (answer == 1) {
    GoA(base, 2, 4);
  } else if (answer == 2) {
    SetField(base, 340, 1);
    GoA(base, 0);
    calls.Call(kSaveBuffersDestroy, frame, 0x8221C284);
  }
  return 0;
}

uint64_t Kind6OverwritePage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221E908 Kind6Menu::OverwritePage");
  const uint32_t frame = Frame(base, sp, 96);
  const int32_t answer = int32_t(uint32_t(RaceDialogUpdate(base, r4, frame, calls)));
  if (answer == 1) {
    GoA(base, 2);
    Kind6SetUpList(base, frame, calls);
  } else if (answer == 2) {
    calls.Call(kSaveBuffersDestroy, frame, 0x8221E92C);
    GoA(base, 0, 5);
  }
  return 0;
}

uint64_t Kind6SavingPage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C2B8 Kind6Menu::SavingPage");
  const uint32_t frame = Frame(base, sp, 96);
  if (int32_t(uint32_t(RaceDialogUpdate(base, r4, frame, calls))) != 0) {
    calls.Call(kSaveBuffersDestroy, frame, 0x8221C2D4);
    GoA(base, 0, 5);
  }
  return 0;
}

uint64_t Kind6QuitPage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C308 Kind6Menu::QuitPage");
  const uint32_t frame = Frame(base, sp, 96);
  const int32_t answer = int32_t(uint32_t(RaceDialogUpdate(base, r4, frame, calls)));
  if (answer == 1) {
    GoA(base, 1);
  } else if (answer == 2) {
    GoA(base, 0);
  }
  return 0;
}

uint64_t Kind6CheckRecordPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221E510 Kind6Menu::CheckRecordPage");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t session = Session(base);
  if (LoadU64(base, session + 208) > LoadU64(base, session + 200)) {
    Kind6NetState(base, 4111, frame, calls);
    guest::StoreU32(base, Session(base) + 160, 1);
    guest::StoreU32(base, Session(base) + 236, 0);
    GoA(base, 8);
    SetField(base, 316, 1);
  } else {
    SetField(base, 316, 0);
    GoA(base, 9);
    GoB(base, 0, 6);
    Kind6ShowRanking(base, frame, calls);
  }
  return 0;
}

uint64_t Kind6RankingReadPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221C008 Kind6Menu::RankingReadPage");
  const uint32_t frame = Frame(base, sp, 128);
  uint32_t session = Session(base);
  if (int32_t(Field(base, 356)) != 0) return 0;
  if (int32_t(Field(base, 320)) == 0) {
    Kind6NetState(base, 4112, frame, calls);
    Kind6LeaderboardRead(base, session, frame, calls);
    if (int32_t(Field(base, 340)) != 0) {
      SetField(base, 324, 44);
      GoB(base, 5, 5);
    }
    SetField(base, 320, 1);
    session = Session(base);
  }
  const uint32_t state = guest::LoadU32(base, session + 180);
  if (state <= 1) return 0;
  if (state == 2) {
    if (int32_t(Field(base, 340)) == 0) GoB(base, 0, 6);
    GoA(base, 9);
    SetField(base, 320, 0);
  } else {
    if (int32_t(Field(base, 340)) == 0) GoB(base, 3, 6, 2);
    GoA(base, 11);
    SetField(base, 320, 0);
    SetField(base, 324, 57);
    SetField(base, 328, 1);
  }
  return 0;
}

uint64_t Kind6UploadPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221E5E8 Kind6Menu::UploadPage");
  const uint32_t frame = Frame(base, sp, 128);
  uint32_t session = Session(base);
  if (int32_t(guest::LoadU32(base, session + 224)) != 0 &&
      int32_t(guest::LoadU32(base, session + 236)) == 1) {
    const uint32_t rank = uint32_t(Kind6Rank(base));
    session = Session(base);
    if (rank > 19) {
      guest::StoreU32(base, session + 236, 3);
      SetField(base, 316, 1);
      GoA(base, 9);
      GoB(base, 0, 6);
    } else if (int32_t(guest::LoadU32(base, session + 228)) != 0) {
      GoA(base, 11);
      SetField(base, 316, 1);
      SetField(base, 324, 48);
      SetField(base, 332, 1);
      GoB(base, 4, 6);
      Kind6ShowPanelB(base, frame, calls);
    } else {
      GoA(base, 11);
      SetField(base, 316, 1);
      SetField(base, 328, 3);
      SetField(base, 324, 49);
      GoB(base, 3, 6, 2);
      guest::StoreU32(base, Session(base) + 236, 2);
      Kind6ShowPanelA(base, frame, calls);
    }
    Kind6ShowRanking(base, frame, calls);
    session = Session(base);
  }
  if (int32_t(guest::LoadU32(base, session + 232)) != 0) {
    guest::StoreU32(base, session + 236, 3);
    SetField(base, 316, 0);
    SetField(base, 328, 1);
    SetField(base, 324, 57);
    GoB(base, 3, 6, 2);
    GoA(base, 11);
  }
  return 0;
}

uint64_t Kind6MenuBackPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D898 Kind6Menu::MenuBackPage");
  const uint32_t frame = Frame(base, sp, 96);
  if (int32_t(Field(base, 340)) != 0) {
    GoA(base, 0);
    GoB(base, 0);
    Kind6ShowPanelA(base, frame, calls);
  }
  return 0;
}

uint64_t Kind6UploadResultPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D908 Kind6Menu::UploadResultPage");
  const uint32_t frame = Frame(base, sp, 96);
  const int32_t state = int32_t(guest::LoadU32(base, Session(base) + 236));
  if (state == 0) {
    GoA(base, 14);
    GoB(base, 4, 6);
  } else if (state == 4) {
    GoA(base, 11);
    GoB(base, 3, 6, 2);
    SetField(base, 324, 52);
    SetField(base, 328, 2);
    Kind6ShowPanelA(base, frame, calls);
  }
  return 0;
}

uint64_t Kind6RankingPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221BE90 Kind6Menu::RankingPage");
  const uint32_t frame = Frame(base, sp, 128);
  uint32_t session = Session(base);
  if (int32_t(guest::LoadU32(base, session + 144)) == 0 && int32_t(Field(base, 320)) == 0) {
    Kind6NetState(base, 4112, frame, calls);
    Kind6LeaderboardRead(base, session, frame, calls);
    SetField(base, 320, 1);
    session = Session(base);
  }
  const uint32_t state = guest::LoadU32(base, session + 180);
  if (state < 2) return 0;
  if (state == 2) {
    GoA(base, 6);
    SetField(base, 308, uint32_t(Kind6Rank(base)));
    const uint32_t results = Results(base);
    SetField(base, 312,
             results != 0 ? guest::LoadU32(base, guest::LoadU32(base, results + 4) + 8) : kNone);
    SetField(base, 320, 0);
  } else if (state == 3) {
    Kind6NetState(base, 4111, frame, calls);
    guest::StoreU32(base, Session(base) + 160, 1);
    GoA(base, 8);
    SetField(base, 316, 1);
    SetField(base, 320, 0);
  } else {
    GoA(base, 11);
    GoB(base, 3, 6, 2);
    SetField(base, 320, 0);
    SetField(base, 324, 57);
    SetField(base, 328, 1);
  }
  return 0;
}

uint64_t Kind6MessageBackPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221D9D8 Kind6Menu::MessageBackPage");
  const uint32_t frame = Frame(base, sp, 96);
  if (int32_t(Field(base, 340)) != 0) {
    GoA(base, 11);
    GoB(base, 3, 2);
    SetField(base, 324, 51);
    SetField(base, 328, 0);
    Kind6ShowPanelA(base, frame, calls);
  }
  return 0;
}

// --- Page object B's pages ---

uint64_t Kind6QuestionPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221E980 Kind6Menu::QuestionPage");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMenuStep, frame, 0x8221E9A0, Screen(base) + 236);
  bool leave = false;
  if (Pressed(calls, frame, 0x8221E9B0, Screen(base) + 236, 159)) {
    Sound(calls, frame, 0x8221E9C0, 0);
    const int32_t item = int32_t(Field(base, 236));
    if (item == 0) {
      GoA(base, 11);
      GoB(base, 2, 1);
      SetField(base, 324, 50);
      SetField(base, 332, 2);
    } else if (item == 1) {
      leave = true;
    }
  } else if (Pressed(calls, frame, 0x8221EA38, Screen(base) + 236, 158)) {
    Sound(calls, frame, 0x8221EA48, 2);
    leave = true;
  }
  if (leave) {
    SetField(base, 320, 0);
    GoA(base, 9);
    GoB(base, 0, 1);
    guest::StoreU32(base, Session(base) + 236, 3);
    SetField(base, 356, 1);
  }
  if (int32_t(Field(base, 244)) != 0) {
    Sound(calls, frame, 0x8221EAB8, 4);
    GoB(base, 1, 4);
  }
  return 0;
}

uint64_t Kind6QuestionAnswerPage(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221C360 Kind6Menu::QuestionAnswerPage");
  Frame(base, sp, 96);
  const int32_t answer = int32_t(Field(base, 332));
  if (answer == 1) {
    GoB(base, 1, 0);
  } else if (answer == 2) {
    guest::StoreU32(base, Session(base) + 236, 2);
    GoA(base, 10);
    GoB(base, 5, 5);
  }
  return 0;
}

uint64_t Kind6MessagePage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221EAE8 Kind6Menu::MessagePage");
  const uint32_t frame = Frame(base, sp, 96);
  if (!Pressed(calls, frame, 0x8221EB0C, Screen(base) + 236, 159)) return 0;
  Sound(calls, frame, 0x8221EB1C, 0);
  const uint32_t kind = Field(base, 328);
  if (kind == 1) {
    Kind6ShowTimeOnly(base, frame, calls);
    GoA(base, 0);
    GoB(base, 0, 3);
  } else if (kind <= 3) {
    SetField(base, 320, 0);
    GoA(base, 9);
    GoB(base, 0, 3);
  }
  return 0;
}

uint64_t Kind6WaitingPage(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221C3F0 Kind6Menu::WaitingPage");
  Frame(base, sp, 96);
  if (int32_t(Field(base, 332)) == 1) GoB(base, 1, 0);
  return 0;
}

// --- The update ---

uint64_t Kind6Update(uint8_t* base, uint32_t screen, uint64_t r4, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221F1E8 Kind6Menu::Update");
  // r4 isn't read: the original sets it before its calls (the dialog pages
  // get the r4 left at their call, which their callees don't read).
  (void)r4;
  const uint32_t frame = Frame(base, sp, 304);
  // B's handlers at +80, A's at +112, as the original stores them.
  constexpr uint32_t kB[6] = {kReturns0,  0x8221E980, 0x8221C360,
                              0x8221EAE8, 0x8221C3F0, kReturns0};
  constexpr uint32_t kA[15] = {0x8221E788, 0x8221C1E0, 0x8221DA60, 0x8221C238, 0x8221E908,
                               0x8221C2B8, 0x8221E510, 0x8221C008, 0x8221E5E8, 0x8221D898,
                               0x8221D908, kReturns0,  0x8221BE90, 0x8221C308, 0x8221D9D8};
  for (uint32_t i = 0; i < 6; i++) guest::StoreU32(base, frame + 80 + 4 * i, kB[i]);
  for (uint32_t i = 0; i < 15; i++) guest::StoreU32(base, frame + 112 + 4 * i, kA[i]);
  const int32_t a = int32_t(guest::LoadU32(base, screen + 40));
  const int32_t b = int32_t(guest::LoadU32(base, screen + 208));
  uint64_t r3 = uint64_t(screen) + 40;
  uint64_t regs4 = Register(0x8221C308), regs5 = Register(0x8221D9D8), regs6 = Register(kReturns0);
  if (int32_t(guest::LoadU32(base, screen + 352)) != 0) {
    guest::StoreU32(base, screen + 324, 44);
    PagesGo(base, screen + 40, 7, kNone, kNone);
    PagesGo(base, screen + 208, 5, 5, kNone);
    guest::StoreU32(base, screen + 352, 0);
    guest::StoreU32(base, screen + 320, 0);
    r3 = uint64_t(screen) + 208;
    regs4 = 5;
    regs5 = 5;
    regs6 = kMinusOne;
  }
  // A page past its table: the frame's word run as the original's bctrl.
  auto beyond = [&](uint32_t table, int32_t page, uint32_t lr) {
    const uint32_t handler = guest::LoadU32(base, (uint32_t(page) << 2) + frame + table);
    const uint64_t regs[8] = {r3,
                              regs4,
                              regs5,
                              regs6,
                              Register(0x8221E980),
                              Register(0x8221C360),
                              Register(0x8221EAE8),
                              Register(0x8221C3F0)};
    return calls.CallArgs(handler, frame, lr, regs);
  };
  if (b != -1) {
    switch (b) {
      case 0:
      case 5: r3 = 0; break;
      case 1: r3 = Kind6QuestionPage(base, frame, calls); break;
      case 2: r3 = Kind6QuestionAnswerPage(base, frame); break;
      case 3: r3 = Kind6MessagePage(base, frame, calls); break;
      case 4: r3 = Kind6WaitingPage(base, frame); break;
      default: r3 = beyond(80, b, 0x8221F364); break;
    }
  }
  if (a == -1) return 0;
  switch (a) {
    case 0: return Kind6MainPage(base, frame, calls);
    case 1: return Kind6AnswerPage(base);
    case 2: return Kind6SlotPage(base, frame, calls);
    case 3: return Kind6SavePage(base, regs4, frame, calls);
    case 4: return Kind6OverwritePage(base, regs4, frame, calls);
    case 5: return Kind6SavingPage(base, regs4, frame, calls);
    case 6: return Kind6CheckRecordPage(base, frame, calls);
    case 7: return Kind6RankingReadPage(base, frame, calls);
    case 8: return Kind6UploadPage(base, frame, calls);
    case 9: return Kind6MenuBackPage(base, frame, calls);
    case 10: return Kind6UploadResultPage(base, frame, calls);
    case 11: return 0;
    case 12: return Kind6RankingPage(base, frame, calls);
    case 13: return Kind6QuitPage(base, regs4, frame, calls);
    case 14: return Kind6MessageBackPage(base, frame, calls);
    default: return beyond(112, a, 0x8221F380);
  }
}

}  // namespace race_screens
