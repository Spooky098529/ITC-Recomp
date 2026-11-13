// The achievements (docs/functions/achievements.md): see achievements.h.

#include "achievements/achievements.h"

#include "base/log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
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

uint32_t AllBeaten(uint8_t* base, uint32_t list, uint32_t count, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82290698 Achievements::AllBeaten");
  guest::StoreU32(base, sp - 112, sp);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t item = guest::LoadU32(base, list + 4 * i);
    if (guest::LoadU16(base, item * 12 + save + kItemWins) == 0) {
      return 0;
    }
  }
  return 1;
}

uint32_t BeatsCourseTime(uint8_t* base, uint32_t route, uint32_t time, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82290710 Achievements::BeatsCourseTime");
  guest::StoreU32(base, sp - 96, sp);
  const int32_t wanted = int32_t(guest::LoadU32(base, route));
  for (uint32_t i = 0; i < kCourseTargetCount; i++) {
    const uint32_t pair = kCourseTargets + 8 * i;
    if (int32_t(guest::LoadU32(base, pair)) == wanted) {
      return TimeLess(guest::LoadU32(base, time), guest::LoadU32(base, pair + 4));
    }
  }
  return 0;
}

uint32_t Check(uint8_t* base, uint32_t kind, uint32_t out, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82290788 Achievements::Check");
  const uint32_t frame = sp - 240;
  guest::StoreU32(base, frame, sp);
  // Ids 2 to 16 but 9 and 13: the one item (id 10's through the frame's
  // +80 like the others).
  static constexpr uint32_t kOneItem[15] = {376, 377, 378, 381, 379, 380, 382, 0,
                                            391, 396, 392, 0,   397, 398, 399};
  uint32_t any = 0;
  for (int i = 0; i < kCount; i++) {
    const uint32_t word = out + 4 * i;
    guest::StoreU32(base, word, 0);
    const uint32_t pair = kTable + 8 * i;
    if (int32_t(guest::LoadU32(base, pair)) != int32_t(kind)) {
      continue;
    }
    const uint32_t index = guest::LoadU32(base, pair + 4) - 2;
    if (index <= 14) {
      uint32_t list, count;
      if (index == 7) {  // id 9: items 383 to 391
        for (uint32_t k = 0; k < 9; k++) {
          guest::StoreU32(base, frame + 112 + 4 * k, 383 + k);
        }
        list = frame + 112;
        count = 9;
      } else if (index == 11) {  // id 13: items 393 to 395
        for (uint32_t k = 0; k < 3; k++) {
          guest::StoreU32(base, frame + 88 + 4 * k, 393 + k);
        }
        list = frame + 88;
        count = 3;
      } else {
        guest::StoreU32(base, frame + 80, kOneItem[index]);
        list = frame + 80;
        count = 1;
      }
      guest::StoreU32(base, word, AllBeaten(base, list, count, frame));
    } else if (index <= 22) {  // ids 17 to 24: route id - 17
      const uint32_t progress = guest::LoadU32(base, kProgress);
      if (int32_t(guest::LoadU32(base, progress + kProgressRoute)) == int32_t(index - 15)) {
        guest::StoreU32(base, word,
                        BeatsCourseTime(base, progress + kProgressRoute,
                                        progress + kProgressTime, frame));
      }
    } else if (index <= 26) {  // ids 25 to 28: a count of the player's
      const int32_t slot = mode_tick::PlayersSignedInSlot(base, 0);
      if (slot >= 0) {
        const uint32_t threshold = guest::LoadU32(base, kThresholds + 4 * i);
        const uint32_t value = guest::LoadU32(base, kUserCounts + 4 * uint32_t(slot)) + 1;
        if (value >= threshold) {
          guest::StoreU32(base, word, 1);
        }
      }
    }
    if (int32_t(guest::LoadU32(base, word)) == 1) {
      any = 1;
    }
  }
  return any;
}

uint64_t Write(uint8_t* base, uint64_t words, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82290A30 Achievements::Write");
  const uint32_t frame = sp - 144;
  guest::StoreU32(base, frame, sp);
  uint64_t r3 = words;
  for (uint32_t i = 0; i < uint32_t(kCount); i++) {
    if (guest::LoadU32(base, uint32_t(words) + 4 * i) == 0) {
      continue;
    }
    const uint32_t entry = kEntries + 8 * i;
    guest::StoreU32(base, entry, guest::LoadU32(base, kPlayerUser));
    guest::StoreU32(base, entry + 4, guest::LoadU32(base, kTable + 4 + 8 * i));
    // The XAM wrapper through the guest, as the original (the user's call,
    // 2026-11-13): the SDK records the unlock.
    r3 = calls.Call(kWriteAchievements, frame, kWriteReturn, 1, entry,
                    kOverlappeds + kOverlappedBytes * i);
  }
  return r3;
}

}  // namespace achievements
