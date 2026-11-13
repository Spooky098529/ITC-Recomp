// The achievements (src/achievements/, docs/functions/achievements.md)
// against the recompiled functions. Each case draws a world from its seed in
// play's ranges: the items' wins (the halfwords SaveData::Of(0) + 0x199FA +
// 12 x item, items 376 to 399, laid in the scratch area with player 0's
// save data pointer aimed so they land there), the race's progress (its
// route, -1 to 8, and the player's packed time around the course targets,
// in the scratch area too), the signed-in mask, player 0's user and the
// user table's four count words around the thresholds 1, 10, 50 and 100.
// The original runs with --native_achievements off (its callees then
// recompiled too), the register steps (hooks/achievements.h) with it as
// set. XUserWriteAchievements is handed to a recorder
// (hooks::g_start_up_recorder, through its wrapper's override) on both
// sides, so nothing is unlocked in the SDK: each call's r1, lr, r3 to r5 and
// the entry's two words then are compared, and it answers 997 or 1627.
// Compared exactly (nothing here is a float): the scratch area, the write's
// 27 entries, the stack below the original's register saves, r1 and r3. The
// globals the cases point elsewhere are put back after each run.

#include "harness.h"

#include "achievements/achievements.h"
#include "base/flags.h"
#include "hooks/achievements.h"
#include "hooks/app_start_up.h"
#include "mode_tick/mode_tick.h"

#include <initializer_list>
#include <iterator>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_achievements);

namespace harness {

namespace {

namespace ach = achievements;

constexpr uint32_t kSaveDataPointers = 0x826C5B58;  // SaveData::Of's table
// In the scratch area.
constexpr uint32_t kTime = 0x000;                           // the progress's +2204
constexpr uint32_t kRoute = kTime + ach::kProgressRoute - ach::kProgressTime;  // +6076
constexpr uint32_t kWins = 0x100;                           // items 376 to 399, 12 bytes each
constexpr uint32_t kWords = 0x400;                          // the 27 words
constexpr uint32_t kList = 0x500;                           // AllBeaten's list
constexpr uint32_t kTimes = 0x600;                          // BeatsCourseTime's, Time::Less's
static_assert(kRoute + 4 <= kScratchBytes);

// Fills `bytes` at `address` with the case's random bytes.
void WriteRandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t k = 0; k < bytes; k += 4) {
    guest::StoreU32(base, address + k, uint32_t(r.Next()));
  }
}

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {kSaveDataPointers, 4},
    {ach::kProgress, 4},
    {mode_tick::kSignedInMask, 4},
    {ach::kPlayerUser, 4},
    {ach::kUserCounts, 16},
    {ach::kEntries, 8 * ach::kCount},
};

uint32_t Pick(Random& r, std::initializer_list<uint32_t> choices) {
  return *(choices.begin() + r.Below(uint32_t(choices.size())));
}

// A packed time: minutes, seconds, thousandths (bit 31 now and then).
uint32_t PackedTime(Random& r) {
  uint32_t minutes, seconds;
  switch (r.Below(4)) {
    case 0:  // around a target: 3:30, 5:00, 6:00, 16:30
      minutes = Pick(r, {3, 4, 5, 6, 16, 17});
      seconds = Pick(r, {0, 29, 30, 31, 59});
      break;
    case 1:
      minutes = r.Below(20);
      seconds = r.Below(60);
      break;
    case 2:
      minutes = r.Below(0x8000);
      seconds = r.Below(64);
      break;
    default:
      return uint32_t(r.Next());
  }
  uint32_t time = (minutes << 16) | (seconds << 10) | r.Below(1000);
  if (r.Below(10) == 0) {
    time |= 0x80000000u;
  }
  return time;
}

// The world a case reads: the globals pointed into the scratch area.
void MakeWorld(uint8_t* base, uint32_t data, Random& r) {
  WriteRandomBytes(base, data, kScratchBytes, r);
  // The wins: mostly all nonzero (so lists pass), else zeros mixed in.
  const uint32_t zeros = Pick(r, {0, 0, 2, 5, 12});
  for (uint32_t k = 0; k < 24; k++) {
    uint32_t wins = r.Below(4) == 0 ? uint32_t(r.Next()) & 0xFFFF : 1 + r.Below(20);
    if (zeros != 0 && r.Below(zeros) == 0) {
      wins = 0;
    }
    guest::StoreU16(base, data + kWins + 12 * k, uint16_t(wins));
  }
  // Item k's wins at kWins + 12 x (k - 376).
  guest::StoreU32(base, kSaveDataPointers, data + kWins - ach::kItemWins - 12 * 376);
  guest::StoreU32(base, ach::kProgress, data + kTime - ach::kProgressTime);
  guest::StoreU32(base, data + kRoute, Pick(r, {0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 0xFFFFFFFFu}));
  guest::StoreU32(base, data + kTime, PackedTime(r));
  guest::StoreU32(base, mode_tick::kSignedInMask, r.Below(4) == 0 ? r.Below(16) : 1);
  guest::StoreU32(base, ach::kPlayerUser, Pick(r, {0, 0, 0, 1, 2, 3, 0xFF}));
  for (uint32_t s = 0; s < 4; s++) {
    guest::StoreU32(base, ach::kUserCounts + 4 * s,
                    Pick(r, {0, 0, 8, 9, 10, 48, 49, 50, 98, 99, 100, 200, 0xFFFFFFFFu}));
  }
  WriteRandomBytes(base, ach::kEntries, 8 * ach::kCount, r);
}

// XUserWriteAchievements' calls in a case.
struct WriteCall {
  uint32_t r1, lr, r3, r4, r5, user, id, answer;
};
std::vector<WriteCall> g_calls;
Random g_answers;
uint8_t* g_base = nullptr;
hooks::StartUpRecorder g_chained = nullptr;

bool RecordWrite(PPCContext& ctx, uint32_t address) {
  if (address != ach::kWriteAchievements) {
    return g_chained != nullptr && g_chained(ctx, address);
  }
  uint8_t* base = g_base;
  const uint32_t answer = g_answers.Below(4) == 0 ? 1627 : 997;
  g_calls.push_back({ctx.r1.u32, uint32_t(ctx.lr), ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                     guest::LoadU32(base, ctx.r4.u32), guest::LoadU32(base, ctx.r4.u32 + 4),
                     answer});
  ctx.r3.u64 = answer;
  return true;
}

using SetupFn = void (*)(PPCContext& t, Random& r, uint8_t* base, uint32_t data);

// A group: `count` cases of `recompiled` against `native` (the register
// steps), the stack compared from `frame` bytes below r1 up to `saves`
// bytes below it (the function's own frame, its register saves left out;
// its callees' frames below have their own groups).
void Group(Context& h, const std::string& name, int count, GuestFunction recompiled,
           GuestFunction native, uint32_t frame, uint32_t saves, SetupFn setup) {
  Results results(name);
  for (int c = 0; c < count; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      std::vector<std::vector<uint8_t>> saved;
      for (const Global& g : kGlobals) {
        saved.push_back(Read(h.base, g.address, g.bytes));
      }
      MakeWorld(h.base, h.data, r);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandomBytes(h.base, sp - 512, 512, r);
      t.r3.u64 = r.Next();
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      setup(t, r, h.base, h.data);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_calls.clear();
      g_answers = Random(r.Next());
      g_base = h.base;
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      g_chained = recorder;
      hooks::g_start_up_recorder = RecordWrite;
      if (v == 0) {
        const bool flag = ITC_FLAG(native_achievements);
        ITC_FLAG(native_achievements) = false;
        recompiled(t, h.base);
        ITC_FLAG(native_achievements) = flag;
      } else {
        native(t, h.base);
      }
      hooks::g_start_up_recorder = recorder;
      g_chained = nullptr;
      out[v].Word(t.r1.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(g_calls.size()));
      for (const WriteCall& call : g_calls) {
        for (const uint32_t word :
             {call.r1, call.lr, call.r3, call.r4, call.r5, call.user, call.id, call.answer}) {
          out[v].Word(word);
        }
      }
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Bytes(Read(h.base, ach::kEntries, 8 * ach::kCount));
      out[v].Bytes(Read(h.base, sp - frame, frame - saves));
      for (size_t i = 0; i < std::size(kGlobals); i++) {
        Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// 27 words: each 0, 1 or now and then another value.
void Words(uint8_t* base, uint32_t address, Random& r) {
  const uint32_t density = Pick(r, {0, 1, 4, 27});
  for (uint32_t i = 0; i < uint32_t(ach::kCount); i++) {
    uint32_t word = 0;
    if (density != 0 && r.Below(density) == 0) {
      word = r.Below(8) == 0 ? uint32_t(r.Next()) : 1;
    }
    guest::StoreU32(base, address + 4 * i, word);
  }
}

}  // namespace

void Achievements(Context& h) {
  // Time::Less: a leaf.
  Group(
      h, "Time::Less (sub_820FF708)", 400, __imp__sub_820FF708, hooks::TimeLessSteps, 0, 0,
      [](PPCContext& t, Random& r, uint8_t* base, uint32_t data) {
        const uint32_t a = PackedTime(r);
        const uint32_t b = r.Below(4) == 0 ? a : PackedTime(r);
        guest::StoreU32(base, data + kTimes, a);
        guest::StoreU32(base, data + kTimes + 4, b);
        t.r3.u64 = data + kTimes;
        t.r4.u64 = data + kTimes + 4;
      });
  // AllBeaten: r29 to r31 and lr saved in [sp - 32, sp).
  Group(
      h, "Achievements::AllBeaten (sub_82290698)", 400, __imp__sub_82290698,
      hooks::AchievementsAllBeatenSteps, 112, 32, [](PPCContext& t, Random& r, uint8_t* base, uint32_t data) {
        const uint32_t count = Pick(r, {0, 1, 1, 1, 3, 9, 12});
        for (uint32_t i = 0; i < count; i++) {
          guest::StoreU32(base, data + kList + 4 * i, 376 + r.Below(24));
        }
        t.r3.u64 = data + kList;
        t.r4.u64 = count;
      });
  // BeatsCourseTime: r31 and lr saved in [sp - 16, sp).
  Group(
      h, "Achievements::BeatsCourseTime (sub_82290710)", 400, __imp__sub_82290710,
      hooks::AchievementsBeatsCourseTimeSteps, 96, 16, [](PPCContext& t, Random& r, uint8_t* base, uint32_t data) {
        guest::StoreU32(base, data + kTimes, Pick(r, {0, 1, 2, 3, 4, 5, 6, 7, 8, 0xFFFFFFFFu}));
        guest::StoreU32(base, data + kTimes + 4, PackedTime(r));
        t.r3.u64 = data + kTimes;
        t.r4.u64 = data + kTimes + 4;
      });
  // Check: r22 to r31 and lr saved in [sp - 88, sp) (its item lists at
  // the frame's +80 to +144 compared).
  Group(
      h, "Achievements::Check (sub_82290788)", 1000, __imp__sub_82290788,
      hooks::AchievementsCheckSteps, 240, 88, [](PPCContext& t, Random& r, uint8_t*, uint32_t data) {
        t.r3.u64 = r.Below(10) == 0 ? (r.Below(2) == 0 ? 3 : r.Next()) : r.Below(3);
        t.r4.u64 = data + kWords;
      });
  // Write: r25 to r31 and lr saved in [sp - 64, sp).
  Group(
      h, "Achievements::Write (sub_82290A30)", 400, __imp__sub_82290A30,
      hooks::AchievementsWriteSteps, 144, 64,
      [](PPCContext& t, Random& r, uint8_t* base, uint32_t data) {
        Words(base, data + kWords, r);
        t.r3.u64 = data + kWords;
      });
}

}  // namespace harness
