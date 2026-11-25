// The achievements (docs/functions/achievements.md): see achievements.h.

#include "achievements/achievements.h"

#include "base/log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "schema/achievements.h"
#include "schema/race_event.h"
#include "schema/race_states.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace achievements {

namespace {

// A packed time in signed thousandths, as the original's 32-bit mullw.
int32_t Thousandths(uint32_t time) {
  const uint32_t minutes = (time >> 16) & 0x7FFF;
  const uint32_t seconds = (time >> 10) & 0x3F;
  const uint32_t value = (minutes * 60 + seconds) * 1000 + (time & 0x3FF);
  const uint32_t sign = (time & 0x80000000u) != 0 ? 0xFFFFFFFFu : 1u;
  return int32_t(value * sign);
}

}  // namespace

uint32_t TimeLess(uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820FF708 Time::Less");
  return Thousandths(a) < Thousandths(b) ? 1 : 0;
}

namespace {

// AllBeaten's walk over the items, wherever their words are (the guest list,
// or Check's lists, C++ locals since E5 step 6).
template <typename Item>
uint32_t AllBeatenItems(uint32_t count, Item item_at) {
  ITC_NATIVE_RUNNING("sub_82290698 Achievements::AllBeaten");
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t item = item_at(i);
    // The item's record: SaveData's item records are 12 bytes by item id.
    if (schema::SaveData(item * 12 + save).item_record_wins() == 0) {
      return 0;
    }
  }
  return 1;
}

}  // namespace

uint32_t AllBeaten(uint32_t list, uint32_t count, uint32_t sp) {
  // No guest frame (E5 step 6): the original's 112 bytes held only the back
  // chain, and nothing here calls guest code.
  (void)sp;
  return AllBeatenItems(count,
                        [&](uint32_t i) { return guest::LoadU32(list + 4 * i); });
}

uint32_t BeatsCourseTime(uint32_t route, uint32_t time, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82290710 Achievements::BeatsCourseTime");
  (void)sp;  // no guest frame (E5 step 6): only the back chain, no guest calls
  const int32_t wanted = int32_t(guest::LoadU32(route));
  for (uint32_t i = 0; i < kCourseTargetCount; i++) {
    const schema::AchievementCourseTarget target(schema::AchievementsGlobals::course_targets_address() +
                  schema::AchievementCourseTarget::kSize * i);
    if (target.route() == wanted) {
      return TimeLess(schema::AchievementsPackedTime(time).packed(), target.time());
    }
  }
  return 0;
}

uint32_t Check(uint32_t kind, uint32_t out, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82290788 Achievements::Check");
  // No guest frame (E5 step 6): the item lists (the original's +80, +88
  // and +112) are C++ locals, read only by AllBeaten; nothing here calls
  // guest code.
  (void)sp;
  // Ids 2 to 16 but 9 and 13: the one item (id 10's through the frame's
  // +80 like the others).
  static constexpr uint32_t kOneItem[15] = {376, 377, 378, 381, 379, 380, 382, 0,
                                            391, 396, 392, 0,   397, 398, 399};
  uint32_t any = 0;
  for (int i = 0; i < kCount; i++) {
    const schema::AchievementFlag word(out + schema::AchievementFlag::kSize * i);
    word.set_earned(0);
    const schema::AchievementPair pair(schema::AchievementsGlobals::table_address() + schema::AchievementPair::kSize * i);
    if (pair.kind() != int32_t(kind)) {
      continue;
    }
    const uint32_t index = pair.id() - 2;
    if (index <= 14) {
      uint32_t items[9];
      uint32_t count;
      if (index == 7) {  // id 9: items 383 to 391
        for (uint32_t k = 0; k < 9; k++) {
          items[k] = 383 + k;
        }
        count = 9;
      } else if (index == 11) {  // id 13: items 393 to 395
        for (uint32_t k = 0; k < 3; k++) {
          items[k] = 393 + k;
        }
        count = 3;
      } else {
        items[0] = kOneItem[index];
        count = 1;
      }
      word.set_earned(AllBeatenItems(count, [&](uint32_t k) { return items[k]; }));
    } else if (index <= 22) {  // ids 17 to 24: route id - 17
      const schema::RaceProgress progress = schema::AchievementsGlobals::progress();
      if (int32_t(progress.route_course()) == int32_t(index - 15)) {
        word.set_earned(BeatsCourseTime(progress.route_course_address(),
                                        progress.e0_time_address(), sp));
      }
    } else if (index <= 26) {  // ids 25 to 28: a count of the player's
      const int32_t slot = mode_tick::PlayersSignedInSlot(0);
      if (slot >= 0) {
        const uint32_t threshold =
            guest::LoadU32(schema::AchievementsGlobals::thresholds_address() + 4 * i);
        const uint32_t value = guest::LoadU32(schema::AchievementsGlobals::user_counts_address() + 4 * uint32_t(slot)) + 1;
        if (value >= threshold) {
          word.set_earned(1);
        }
      }
    }
    if (int32_t(word.earned()) == 1) {
      any = 1;
    }
  }
  return any;
}

uint64_t Write(uint64_t words, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82290A30 Achievements::Write");
  // No guest frame (E5 step 6): the original's 144 bytes held only the back
  // chain; XUserWriteAchievements (native, plugged in) is called from `sp`
  // and reads no caller slot.
  uint64_t r3 = words;
  for (uint32_t i = 0; i < uint32_t(kCount); i++) {
    if (schema::AchievementFlag(uint32_t(words) + schema::AchievementFlag::kSize * i)
            .earned() == 0) {
      continue;
    }
    const schema::AchievementEntry entry(schema::AchievementsGlobals::entries_address() + schema::AchievementEntry::kSize * i);
    entry.set_user(schema::AchievementsGlobals::player_user());
    entry.set_id(schema::AchievementPair(schema::AchievementsGlobals::table_address() +
                                                   schema::AchievementPair::kSize * i)
                     .id());
    // The XAM wrapper through the guest, as the original (the user's call,
    // 2026-11-13): the SDK records the unlock.
    r3 = calls.Call(kWriteAchievements, sp, kWriteReturn, 1, entry.address,
                    schema::AchievementsGlobals::overlappeds_address() +
                        schema::AchievementOverlapped::kSize * i);
  }
  return r3;
}

uint32_t WriteAchievements(uint32_t count, uint32_t list,
                           uint32_t overlapped) {
  ITC_NATIVE_RUNNING("sub_82294E58 XUserWriteAchievements");
  // Plugged in (the user's call, 2026-11-16): the game reads nothing back
  // (the answer is dropped, the overlapped and entries never read again), so
  // XMsgStartIORequest isn't called and no unlock is recorded anywhere; the
  // ids are only logged.
  for (uint32_t i = 0; i < count; i++) {
    ITC_LOG_INFO("achievement {} earned (not recorded)",
                 schema::AchievementEntry(list + schema::AchievementEntry::kSize * i).id());
  }
  constexpr uint32_t kIoPending = 997;
  return overlapped != 0 ? kIoPending : 0;
}

}  // namespace achievements
