// The achievements natively (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/achievements.md): after a race, which of the game's
// 27 achievements it earned (Achievements::Check, from the game's data
// only), and their write to the system (Achievements::Write). Plain C++ over
// guest memory with the originals' frame words. The write calls
// XUserWriteAchievements (sub_82294E58) through the guest as the original
// does (`Calls`), and that wrapper is plugged in: nothing is recorded (the
// user's call, 2026-11-16). Adapters: src/hooks/achievements.cpp.

#pragma once

#include <cstdint>

namespace achievements {

constexpr int kCount = 27;
// The image's tables and blocks: schema::AchievementsGlobals
// (itc_recomp/schema/achievements.toml). The course time targets: 8.
constexpr uint32_t kCourseTargetCount = 8;
// The race's progress (AchievementsGlobals::progress) is race-states'
// schema::RaceProgress (route_course, e0_time); the items' wins race-event's
// schema::SaveData (item_record_wins, + 12 x the item).
// XUserWriteAchievements (system-services.md) and the original's return
// address after it.
constexpr uint32_t kWriteAchievements = 0x82294E58;
constexpr uint32_t kWriteReturn = 0x82290A98;

// A guest function called through the function table from the frame at
// `r1` with r3 to r5 and the link register `lr` (the original's return
// address); its r3 (64 bits). The adapter's job (hooks/achievements.cpp).
struct Calls {
  void* user;
  uint64_t (*call)(void* user, uint32_t function, const uint64_t args[3], uint32_t r1,
                   uint32_t lr);

  uint64_t Call(uint32_t function, uint32_t r1, uint32_t lr, uint64_t r3, uint64_t r4,
                uint64_t r5) const {
    const uint64_t args[3] = {r3, r4, r5};
    return call(user, function, args, r1, lr);
  }
};

// Time::Less (sub_820FF708): 1 when the packed time `a` (minutes in bits
// 16-30, seconds 10-15, thousandths 0-9, bit 31 negative) is below `b`, in
// signed thousandths (32-bit products, as the original's mullw), else 0.
uint32_t TimeLess(uint32_t a, uint32_t b);

// Achievements::AllBeaten (sub_82290698): 1 when every item in the `count`
// words at `list` has nonzero wins (or `count` is 0), else 0; SaveData::Of(0)
// first. Its 112-byte frame below `sp` (the back chain).
uint32_t AllBeaten(uint32_t list, uint32_t count, uint32_t sp);

// Achievements::BeatsCourseTime (sub_82290710): the route at `route` looked
// up in the targets; found, Time::Less(the time at `time`, the target), else
// 0. Its 96-byte frame below `sp` (the back chain).
uint32_t BeatsCourseTime(uint32_t route, uint32_t time, uint32_t sp);

// Achievements::Check (sub_82290788): the 27 words at `out` (0, or 1 for
// each pair of kind `kind` earned); 1 when any is 1. Its 240-byte frame
// below `sp` (the back chain, the item lists at +80, +88 and +112).
uint32_t Check(uint32_t kind, uint32_t out, uint32_t sp);

// Achievements::Write (sub_82290A30): for each nonzero word of the 27 at
// `words`, its entry {player 0's user, the id} and XUserWriteAchievements(1,
// the entry, its overlapped) through the guest, from the 144-byte frame
// below `sp`. r3 as the original leaves it (the last call's, else `words`).
uint64_t Write(uint64_t words, uint32_t sp, const Calls& calls);

// XUserWriteAchievements (sub_82294E58), plugged in (the user's call,
// 2026-11-16): the game reads nothing back, so the XAM message isn't sent
// and no unlock is recorded; the `count` entries' ids at `list` are logged.
// 997 (pending) with an overlapped, as the original answered, else 0.
uint32_t WriteAchievements(uint32_t count, uint32_t list,
                           uint32_t overlapped);

}  // namespace achievements
