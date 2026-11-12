// The race's end (src/race_end/, docs/functions/race-end.md and
// race-loading.md) against the recompiled functions. The originals run with
// --native_race_end off (their callees' adapters native, as in the game),
// the plain C++ through the register steps (hooks/race_end.h).
//
// The world the cases build: a race data block (its flags, kind, race mode
// and totals shaped), player 0's save data block (the save slots pointed at
// it: the stage flags, the day, the locations, the unlock lists' counts and
// bits, the item records), the data tables' object with 400 items (their
// kinds and records shaped so Progress::AfterRace reaches every stage) and
// table 0's and the 40-byte table's entries in the race block, the race
// event (none, or a block whose groups are empty but the save's location's,
// with up to three open entries: Career::EventLocation's sort), the session
// none or a word (none whenever Session::Free would run: the network_mode
// harness has its cases), sub_82293358's two words set so its first test
// fails (its unseen callees not reached). RaceMode::NextMode's cases add the
// race mode (its dispatcher without handlers, its ring, the +848 machine's
// state, +167260) and the loading screen's UI movie world
// (loading_world.h; UiMovie::Run's update recorded and answered).
//
// Compared, exactly (nothing here is a float): the save block, the race
// data's flags, the race event block, the answer, the frames' back chains
// and their +80 to +87, the title's flag, the boot movies' table, the
// screen fades' words, the session; for NextMode the race mode's first
// 1,024 bytes, the movie world, the loading screen's words and the update
// calls; r3 (64 bits). The globals are saved and put back after each side.

#include "harness.h"
#include "loading_world.h"

#include "base/flags.h"
#include "base/log.h"
#include "career/career.h"
#include "data_tables/data_tables.h"
#include "hooks/race_end.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "race_end/race_end.h"
#include "race_event/race_event.h"

#include <fmt/format.h>

#include <cstring>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_end);

namespace harness {

namespace {

namespace re = race_end;
namespace lw = loading_world;

constexpr uint32_t kSaveDataBytes = 0x1F978;
constexpr uint32_t kSaveSlots = 0x826C5B58;
constexpr uint32_t kItems = 400;
constexpr uint32_t kItemBytes = 3008;
constexpr uint32_t kRaceDataBytes = 0xA3B00;
constexpr uint32_t kTable0 = 0x1000;   // in the race block
constexpr uint32_t kTable40 = 0x2000;  // in the race block
constexpr uint32_t kEventBytes = 0x400;
constexpr uint32_t kModeBytes = re::kPostUpdate + 4;
constexpr uint32_t kFades = 0x826A50AC;
constexpr uint32_t kFadesBytes = 0x40;
constexpr uint32_t kStackBytes = 0x1400;  // below the call's stack pointer
constexpr uint32_t kAnswer = 0x40;        // in the scratch area

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

uint32_t Pick(Random& r, std::initializer_list<uint32_t> values) {
  return values.begin()[r.Below(uint32_t(values.size()))];
}

// The globals the cases set, saved and put back.
class SavedBytes {
 public:
  explicit SavedBytes(uint8_t* base) : base_(base) {
    for (const auto& [address, bytes] : kRanges) {
      saved_.push_back(Read(base_, address, bytes));
    }
  }
  ~SavedBytes() {
    for (size_t i = 0; i < std::size(kRanges); i++) {
      std::memcpy(base_ + kRanges[i].first, saved_[i].data(), kRanges[i].second);
    }
  }
  SavedBytes(const SavedBytes&) = delete;
  SavedBytes& operator=(const SavedBytes&) = delete;

 private:
  static constexpr std::pair<uint32_t, uint32_t> kRanges[] = {
      {kSaveSlots, 16},       {re::kRaceData, 4},  {data_tables::kTables, 4},
      {race_event::kEvent, 4}, {re::kTitleFlag, 4}, {re::kBootMovies, 32},
      {kFades, kFadesBytes},  {re::kSession, 4},   {0x826C7944, 4},
      {0x826C7958, 4}};
  uint8_t* base_;
  std::vector<std::vector<uint8_t>> saved_;
};

struct World {
  World()
      : save(kSaveDataBytes),
        tables(0x40 + kItems * kItemBytes),
        race(kRaceDataBytes),
        event(kEventBytes) {}
  bool ok() const { return save.address && tables.address && race.address && event.address; }
  uint32_t item(uint32_t id) const { return tables.address + 0x40 + kItemBytes * id; }
  Block save;
  Block tables;
  Block race;
  Block event;
};

void WriteConditions(uint8_t* base, uint32_t at, Random& r, bool first_valid) {
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t c = at + 8 * k;
    uint32_t kind = Pick(r, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xFFFFFFFFu, 0xFFFFFFFFu});
    if (k == 0 && first_valid && kind == 0xFFFFFFFFu) {
      kind = r.Below(10);
    }
    guest::StoreU32(base, c, kind);
    uint32_t value = kind >= 7 && kind <= 9 ? r.Below(6) : r.Below(kind == 5 || kind == 6 ? 60 : 9);
    if (r.Below(10) == 0) {
      value = 0xFFFFFFFFu;
    }
    guest::StoreU32(base, c + 4, value);
  }
}

// The world for a case: see the file's comment.
void Shape(uint8_t* base, const World& w, Random& r) {
  const uint32_t save = w.save.address, race = w.race.address;
  // The race data.
  uint32_t flags = uint32_t(r.Next()) & ~0x18018u;
  if (r.Below(10) == 0) flags |= 0x8000;
  if (r.Below(10) == 0) flags |= 0x10000;
  if (r.Below(3) == 0) flags |= 0x8;
  if (r.Below(3) == 0) flags |= 0x10;
  guest::StoreU32(base, race + re::kFlags, flags);
  guest::StoreU32(base, race + re::kKind,
                  Pick(r, {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 5, 6, 7, uint32_t(r.Next())}));
  guest::StoreU32(base, race + re::kRaceMode, Pick(r, {0, 1, 2, 2, 3, 5, 7, 7, uint32_t(r.Next())}));
  WriteRandom(base, race + 0xA3AD0, 16, r);
  guest::StoreU32(base, re::kRaceData, race);
  guest::StoreU32(base, re::kSession,
                  (flags & 0x18000) == 0 && r.Below(2) == 0 ? uint32_t(r.Next()) | 1 : 0);
  const uint32_t limit = 1 + r.Below(1000);
  guest::StoreU32(base, 0x826C7944, limit);
  guest::StoreU32(base, 0x826C7958, r.Below(limit));
  WriteRandom(base, re::kBootMovies, 32, r);
  guest::StoreU32(base, re::kTitleFlag, uint32_t(r.Next()));

  // The save data.
  WriteRandom(base, save, w.save.bytes, r);
  for (uint32_t p = 0; p < 4; p++) {
    guest::StoreU32(base, kSaveSlots + 4 * p, save);
  }
  const uint32_t locations[] = {48, 48, 14, 16, 19, 20, 5};
  guest::StoreU32(base, save + 6112, locations[r.Below(7)]);
  guest::StoreU32(base, save + 6108, locations[r.Below(7)]);
  guest::StoreU32(base, save + 6104, r.Below(4));
  guest::StoreU32(base, save + 6092, (1u << r.Below(8)) - 1);
  guest::StoreU32(base, save + 6100, Pick(r, {1, 1, 0, 2, 5, 1000, uint32_t(r.Next())}));
  for (uint32_t i = 0; i < 2; i++) {
    guest::StoreU32(base, save + 0x1CA7C + 4 * i, uint32_t(r.Next()) & uint32_t(r.Next()));
    guest::StoreU32(base, save + 0x1D508 + 4 * i, uint32_t(r.Next()) & uint32_t(r.Next()));
  }
  guest::StoreU32(base, save + re::kUnlocksShown, r.Below(2) == 0 ? 0 : r.Below(20));
  guest::StoreU32(base, save + 0x1D504, r.Below(20));
  const uint32_t zero_in = Pick(r, {1, 1, 2, 60});
  for (uint32_t id = 0; id < kItems; id++) {
    const uint32_t record = save + career::kItemRecords + 12 * id;
    for (uint32_t k = 0; k < 6; k += 2) {
      guest::StoreU16(base, record + k, uint16_t(r.Below(zero_in) == 0 ? r.Below(1000) : 0));
    }
  }

  // The data tables: the items (kinds mostly none, so the stage tests'
  // "all won" hold and every stage is reached), table 0 and the 40-byte
  // table.
  guest::StoreU32(base, data_tables::kTables, w.tables.address);
  guest::StoreU32(base, w.tables.address + 8, kItems);
  guest::StoreU32(base, w.tables.address + 16, w.item(0));
  guest::StoreU32(base, w.tables.address + 20, race + kTable0);
  guest::StoreU32(base, w.tables.address + 56, race + kTable40);
  const bool kinds = r.Below(3) == 0;
  for (uint32_t id = 0; id < kItems; id++) {
    const uint32_t item = w.item(id);
    guest::StoreU8(base, item + 35, uint8_t(kinds && r.Below(4) == 0 ? r.Below(7) : 0xFF));
    guest::StoreU8(base, item + 36, uint8_t(r.Below(3)));
    guest::StoreU8(base, item + 37, uint8_t(r.Below(8)));
    guest::StoreU16(base, item + 42, uint16_t(r.Below(6)));
    guest::StoreU16(base, item + 88, 0);
    for (uint32_t k = 0; k < 3; k++) {
      guest::StoreU16(base, item + 92 + 2 * k, 0xFFFF);
    }
  }
  const uint32_t entries0 = r.Below(12);
  for (uint32_t i = 0; i < entries0; i++) {
    const uint32_t e = race + kTable0 + 56 * i;
    guest::StoreU32(base, e + 0, r.Below(5));
    guest::StoreU32(base, e + 4, uint32_t(r.Next()) | (r.Below(2) == 0 ? 0 : 0x7FF0u));
    guest::StoreU32(base, e + 20, r.Below(2));
    WriteConditions(base, e + 24, r, false);
  }
  guest::StoreU32(base, race + kTable0 + 56 * entries0, 0xFFFFFFFFu);
  const uint32_t entries40 = r.Below(12);
  for (uint32_t i = 0; i < entries40; i++) {
    const uint32_t e = race + kTable40 + 40 * i;
    guest::StoreU32(base, e + 0, uint32_t(r.Next()));
    guest::StoreU32(base, e + 4, r.Below(42));
    guest::StoreU32(base, e + 8, r.Below(40));
    guest::StoreU32(base, e + 12, r.Below(2));
    WriteConditions(base, e + 16, r, true);
  }
  guest::StoreU32(base, race + kTable40 + 40 * entries40 + 16, 0xFFFFFFFFu);

  // The race event: none, or every group empty but the save's location's.
  const uint32_t event = w.event.address;
  WriteRandom(base, event, w.event.bytes, r);
  for (uint32_t i = 0; i < 0x100; i += 4) {
    guest::StoreU32(base, event + i, 0);
  }
  const int32_t location = int32_t(guest::LoadU32(base, save + 6108));
  const uint32_t g = location >= 14 && location <= 19 ? uint32_t(location - 10) : 4;
  const uint32_t group = event + (g + 3) * 12;
  guest::StoreU16(base, group + 0, uint16_t(r.Below(4)));
  guest::StoreU16(base, group + 2, 0);
  guest::StoreU32(base, group + 4, event + 0x200);
  guest::StoreU32(base, group + 8, event + 0x300);
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU16(base, event + 0x200 + 4 * k, uint16_t(r.Below(kItems)));
  }
  guest::StoreU32(base, race_event::kEvent, r.Below(4) == 0 ? 0 : event);
}

void Take(uint8_t* base, const World& w, Outputs& out) {
  out.Bytes(Read(base, w.save.address, w.save.bytes));
  out.Word(guest::LoadU32(base, w.race.address + re::kFlags));
  out.Bytes(Read(base, w.event.address, w.event.bytes));
  out.Word(guest::LoadU32(base, re::kTitleFlag));
  out.Bytes(Read(base, re::kBootMovies, 32));
  out.Bytes(Read(base, kFades, kFadesBytes));
  out.Word(guest::LoadU32(base, re::kSession));
}

// A frame's back chain and its +80 to +87.
void TakeFrame(uint8_t* base, uint32_t frame, Outputs& out) {
  out.Word(guest::LoadU32(base, frame));
  out.Bytes(Read(base, frame + 80, 8));
}

void Run(int v, GuestFunction recompiled, GuestFunction native, PPCContext& t, uint8_t* base) {
  if (v == 0) {
    ITC_FLAG(native_race_end) = false;
    recompiled(t, base);
    ITC_FLAG(native_race_end) = true;
  } else {
    native(t, base);
  }
}

void ModeAfterRaces(Context& h, const World& w) {
  Results results("RaceMode::ModeAfterRace (sub_82150E00)");
  for (uint64_t seed = 0; seed < 200; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      const SavedBytes saved(h.base);
      Random r(seed * 977 + 0x82150E00);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, h.data, kScratchBytes, r);
      WriteRandom(h.base, sp - kStackBytes, kStackBytes, r);
      Shape(h.base, w, r);
      t.r3.u64 = (r.Next() & 0xFFFFFFFF00000000ull) | (h.data + kAnswer);
      t.r4.u64 = r.Next();
      Run(v, __imp__sub_82150E00, hooks::RaceModeModeAfterRace, t, h.base);
      Take(h.base, w, out[v]);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      TakeFrame(h.base, sp - re::kModeAfterRaceFrame, out[v]);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// UiMovie::Run's update calls, kept and answered.
struct UpdateCall {
  uint32_t lr, r1;
  uint64_t r3, r4;
};
std::vector<UpdateCall>* g_updates = nullptr;
Random g_update_answers;

bool UpdateStandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_updates == nullptr || function != 0x8211D2F8) {
    return false;
  }
  g_updates->push_back({uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = g_update_answers.Below(2) == 0 ? 0 : g_update_answers.Next();
  return true;
}

void NextModes(Context& h, const World& w) {
  const Block mode(kModeBytes);
  const lw::MovieWorld movies;
  if (!mode.address || !movies.ok()) {
    ITC_LOG_ERROR("harness: race_end: no race mode or movie world from the allocation layer");
    return;
  }
  lw::BuildStringTable(h.base, movies);
  Results results("RaceMode::NextMode (sub_82151EF0)");
  for (uint64_t seed = 0; seed < 120; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      const SavedBytes saved(h.base);
      const lw::SavedGlobals loading(h.base);
      Random r(seed * 1031 + 0x82151EF0);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, h.data, kScratchBytes, r);
      WriteRandom(h.base, sp - kStackBytes, kStackBytes, r);
      lw::ShapeMovieWorld(h.base, movies, r);
      Shape(h.base, w, r);
      guest::StoreU32(h.base, race_loading::kLoadingCurrent, r.Below(3));
      guest::StoreU32(h.base, race_loading::kLoadingShown, uint32_t(r.Next()));
      for (uint32_t i = 0; i < 3; i++) {
        guest::StoreU32(h.base, race_loading::kLoadingAnimations + 4 * i,
                        r.Below(2) ? (i == 1 ? 13 : 0) : r.Below(256));
      }
      // The race mode: its dispatcher without handlers, its ring, the +848
      // machine's state, +167260.
      WriteRandom(h.base, mode.address, 0x400, r);
      guest::StoreU32(h.base, mode.address + re::kDispatcher + 200, 0);
      guest::StoreU32(h.base, mode.address + re::kRing + 600, r.Below(4));
      guest::StoreU32(h.base, mode.address + re::kRing + 604, r.Below(200));
      guest::StoreU32(h.base, mode.address + re::kMachineState,
                      Pick(r, {0, 1, 3, 4, 5, 35, 35, 35, 35}));
      guest::StoreU32(h.base, mode.address + re::kPostUpdate, Pick(r, {0, 1, uint32_t(r.Next())}));
      t.r3.u64 = (r.Next() & 0xFFFFFFFF00000000ull) | (h.data + kAnswer);
      t.r4.u64 = mode.address;
      g_update_answers = Random(r.Next());
      std::vector<UpdateCall> updates;
      g_updates = &updates;
      const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
      Run(v, __imp__sub_82151EF0, hooks::RaceModeNextMode, t, h.base);
      ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
      g_updates = nullptr;
      Take(h.base, w, out[v]);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Bytes(Read(h.base, mode.address, 0x400));
      const uint32_t frame = sp - re::kNextModeFrame;
      TakeFrame(h.base, frame, out[v]);
      // ModeAfterRace's frame (state 35).
      if (guest::LoadU32(h.base, mode.address + re::kMachineState) == 35) {
        TakeFrame(h.base, frame - re::kModeAfterRaceFrame, out[v]);
      }
      out[v].Bytes(Read(h.base, movies.movies.address, movies.movies.bytes));
      out[v].Bytes(Read(h.base, movies.texts.address, movies.texts.bytes));
      out[v].Word(guest::LoadU32(h.base, race_loading::kLoadingShown));
      out[v].Word(guest::LoadU32(h.base, race_loading::kLoadingCurrent));
      out[v].Word(uint32_t(updates.size()));
      for (const UpdateCall& u : updates) {
        out[v].Word(u.lr);
        out[v].Word(u.r1);
        for (const uint64_t x : {u.r3, u.r4}) {
          out[v].Word(uint32_t(x >> 32));
          out[v].Word(uint32_t(x));
        }
      }
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      loading.Restore();
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

}  // namespace

void RaceEnd(Context& h) {
  const World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: race_end: no block from the allocation layer");
    return;
  }
  {
    // The items and the race block once (the cases shape what's read).
    Random r(0x52454E44ull);  // "REND"
    WriteRandom(h.base, w.tables.address, w.tables.bytes, r);
    WriteRandom(h.base, w.race.address, w.race.bytes, r);
  }
  ModeAfterRaces(h, w);
  NextModes(h, w);
}

}  // namespace harness
