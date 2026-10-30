// Career::SetUpEvent (docs/functions/career.md): the day's offers picked from
// the data tables and the race event made, over the guest layout of a
// player's save data and the original's 121,200-byte frame (its lists and
// counts written where the original keeps them).

#include "career/career.h"

#include "base/log.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "race_overlays/race_overlays.h"
#include "race_setup/race_setup.h"

namespace career {

namespace {

// The frame's parts (offsets from the frame's stack pointer).
constexpr uint32_t kLocations = 80;  // a 64-bit set of locations
constexpr uint32_t kCaps = 96;       // 13 words: the picks allowed by kind
constexpr uint32_t kCounts = 160;    // words by (kind x 6 + location)
constexpr uint32_t kPicked = 480;    // 200 words: the entries picked
constexpr uint32_t kEntries = 1280;  // {table, index} by (kind x 6 + location) x 192 + n
// The save data's parts.
constexpr uint32_t kTable0Done = 0x1CA7C;   // a bit set: table 0's entries granted
constexpr uint32_t kTable0List = 0x1C778;   // 12-byte records, the count at +768
constexpr uint32_t kTable1Words = 0x1B278;  // table 1's words by entry
constexpr uint32_t kTableWords = 0x1B578;   // tables 2 to 7's, 768 bytes a table
constexpr uint32_t kOffers = 0x1AD20;       // 6 locations of 100 bytes, the count at +96
// RaceData::ResetGlobal's frame (its native r3 is from below it).
constexpr uint32_t kResetFrame = 112;

uint32_t Slw(uint32_t value, uint32_t amount) {
  amount &= 0x3F;
  return (amount & 0x20) != 0 ? 0 : value << amount;
}

uint32_t BitWord(uint32_t set, int32_t b) { return set + (uint32_t(b >> 5) << 2); }

bool BitSet(uint8_t* base, uint32_t set, int32_t b) {
  return (guest::LoadU32(base, BitWord(set, b)) & Slw(1, uint32_t(b) & 31)) != 0;
}

void SetBit(uint8_t* base, uint32_t set, int32_t b) {
  const uint32_t word = BitWord(set, b);
  guest::StoreU32(base, word, guest::LoadU32(base, word) | Slw(1, uint32_t(b) & 31));
}

// An entry's three conditions {kind, value} at +24 (all needed when +20 is
// set, else any; none listed passes): Condition::Test's answers.
bool ConditionsPass(uint8_t* base, uint32_t entry, uint32_t location, uint32_t frame) {
  bool grant = false;
  const bool all = guest::LoadU32(base, entry + 20) != 0;
  for (int32_t k = 0; k < 3; k++) {
    const uint32_t condition = entry + 24 + 8 * uint32_t(k);
    if (int32_t(guest::LoadU32(base, condition + 0)) == -1 ||
        int32_t(guest::LoadU32(base, condition + 4)) == -1) {
      if (k == 0) {
        grant = true;
      }
      break;
    }
    const uint32_t pass = ConditionTest(base, condition, location, 0, frame).value_or(0);
    if (!all) {
      if (pass != 0) {
        return true;
      }
    } else {
      if (pass == 0) {
        return false;
      }
      grant = true;
    }
  }
  return grant;
}

// An entry offered: counted under (kind x 6 + location - 14), {table, index}
// added to that list on the frame.
void Collect(uint8_t* base, uint32_t frame, uint32_t entry, int32_t location, uint32_t table,
             uint32_t index) {
  const uint32_t at = guest::LoadU32(base, entry + 0) * 6 + uint32_t(location) - 14;
  const uint32_t count_at = frame + kCounts + (at << 2);
  const uint32_t n = guest::LoadU32(base, count_at);
  const uint32_t slot = (at * 192 + n) << 3;
  guest::StoreU32(base, count_at, n + 1);
  guest::StoreU32(base, frame + kEntries + slot, table);
  guest::StoreU32(base, frame + kEntries + 4 + slot, index);
}

// Kinds 0, 1, 2 and 4 are left out of a table's entries whose word is 2.
bool Done(int32_t kind, uint32_t word) {
  return word == 2 && (kind == 0 || kind == 1 || kind == 2 || kind == 4);
}

}  // namespace

bool SetUpEventNative(uint8_t* base, uint32_t sp) {
  const uint32_t frame = sp - kSetUpEventFrame;
  return ConditionTestFits(base, frame) && race_setup::RaceDataResetNative(frame - kResetFrame);
}

uint64_t SetUpEvent(uint8_t* base, uint32_t save, uint32_t mode, uint32_t location, uint32_t sp,
                    const race_event::Guest& guest, MakeCall make, void* user) {
  ITC_NATIVE_RUNNING("sub_82114268 Career::SetUpEvent");
  const uint32_t frame = sp - kSetUpEventFrame;
  // The arguments in the caller's frame (RaceEvent::Make reads them there).
  guest::StoreU32(base, sp + 28, mode);
  guest::StoreU32(base, sp + 36, location);
  // Locations 14 to 19.
  guest::StoreU32(base, frame + kLocations + 0, 0);
  guest::StoreU32(base, frame + kLocations + 4, 0);
  for (int32_t l = 14; l < 20; l++) {
    SetBit(base, frame + kLocations, l);
  }
  guest::StoreU32(base, save + 6104, mode);
  guest::StoreU32(base, save + 6108, location);
  guest::StoreU32(base, save + 6112, 48);
  race_setup::RaceDataResetGlobal(base, frame - kResetFrame);
  const int32_t day = int32_t(guest::LoadU32(base, save + 6100));
  if (day <= 999 && day == 1) {
    // RaceData::ResetGlobal's r3 (its adapter's).
    return uint64_t(frame) - kResetFrame - 112 - 128 - race_setup::kRouteFrameBytes + 80;
  }

  // Table 0's unlocks, for a location 14 to 19.
  const int32_t here = int32_t(guest::LoadU32(base, sp + 36));
  if (here >= 0 && BitSet(base, frame + kLocations, here)) {
    for (int32_t i = 0;; i++) {
      if (i >= 0 && BitSet(base, save + kTable0Done, i)) {
        continue;
      }
      const uint32_t entry = data_tables::Entry56(base, 0, uint32_t(i));
      if (int32_t(guest::LoadU32(base, entry + 0)) == -1) {
        break;
      }
      const uint32_t mask = guest::LoadU32(base, entry + 4);
      if ((mask & 0x40) != 0 && (mask & 0x3F) == 0) {
        continue;
      }
      const uint32_t stage = StageOf(base, save);
      if (Entry56Allows(base, entry, guest::LoadU32(base, save + 6104),
                        guest::LoadU32(base, save + 6108), stage, 1) == 0) {
        continue;
      }
      if (!ConditionsPass(base, entry, 0, frame)) {
        continue;
      }
      if (i >= 0) {
        SetBit(base, save + kTable0Done, i);
      }
      const uint32_t list = save + kTable0List;
      const uint32_t at = guest::LoadU32(base, list + 768) * 12 + list;
      guest::StoreU32(base, at + 0, 0);
      guest::StoreU32(base, at + 4, 0);
      guest::StoreU32(base, at + 8, uint32_t(i));
      guest::StoreU32(base, list + 768, guest::LoadU32(base, list + 768) + 1);
    }
  }

  // The offers cleared, and the frame's counts.
  for (uint32_t l = 0; l < 6; l++) {
    guest::StoreU32(base, save + kOffers + 96 + 100 * l, 0);
    for (uint32_t k = 0; k < 13; k++) {
      guest::StoreU32(base, frame + kCounts + 4 * l + 24 * k, 0);
    }
  }

  // Table 1's entries, for each location 14 to 19.
  for (int32_t l = 14; l < 20; l++) {
    uint32_t i = 0;
    uint32_t word = save + kTable1Words;
    for (uint32_t entry = data_tables::Entry56(base, 1, 0);
         int32_t(guest::LoadU32(base, entry + 0)) != -1;
         i++, word += 4, entry = data_tables::Entry56(base, 1, i)) {
      const int32_t kind = int32_t(guest::LoadU32(base, entry + 0));
      if (Done(kind, guest::LoadU32(base, word))) {
        continue;
      }
      const uint32_t stage = StageOf(base, save);
      if (Entry56Allows(base, entry, guest::LoadU32(base, save + 6104), uint32_t(l), stage, 1) ==
          0) {
        continue;
      }
      if (!ConditionsPass(base, entry, 1, frame)) {
        continue;
      }
      Collect(base, frame, entry, l, 1, i);
    }
  }

  // Tables 2 to 7, each for one location (table t for location t + 12).
  for (int32_t l = 14; l < 20; l++) {
    const uint32_t table = uint32_t(l - 12);
    uint32_t i = 0;
    uint32_t word = save + kTableWords + 768 * uint32_t(l - 14);
    for (uint32_t entry = data_tables::Entry56(base, table, 0);
         int32_t(guest::LoadU32(base, entry + 0)) != -1;
         i++, word += 4, entry = data_tables::Entry56(base, table, i)) {
      const int32_t kind = int32_t(guest::LoadU32(base, entry + 0));
      if (Done(kind, guest::LoadU32(base, word))) {
        continue;
      }
      const int32_t stage = int32_t(StageOf(base, save));
      const uint32_t entry_mode = guest::LoadU32(base, save + 6104);
      if (kind == 4) {
        continue;
      }
      // The entry's mask: bits 7 to 9 the modes 0, 1 and others (mode 3
      // untested), bits 10 to 14 the stages (13 for stage 4).
      const uint32_t mask = guest::LoadU32(base, entry + 4);
      bool ok = false;
      if ((mask & 0x380) != 0 && int32_t(entry_mode) != 3) {
        const uint32_t bit = entry_mode == 0 ? 7 : entry_mode == 1 ? 8 : 9;
        if ((Slw(1, bit) & mask) == 0) {
          continue;
        }
        ok = true;
      }
      if ((mask & 0x7C00) != 0) {
        if (stage == 4) {
          if ((mask & 0x2000) == 0) {
            continue;
          }
        } else if ((Slw(1, uint32_t(stage + 10)) & mask) == 0) {
          continue;
        }
        ok = true;
      }
      if (!ok) {
        continue;
      }
      if (!ConditionsPass(base, entry, table, frame)) {
        continue;
      }
      Collect(base, frame, entry, l, table, i);
    }
  }

  // The picks: for each location, up to 8 entries in all, each kind's at
  // random without repeats, at most its cap (2, 4, 2, 8, then 0: a kind
  // capped at 0 still gets one).
  guest::StoreU32(base, frame + kCaps + 16, 0);
  guest::StoreU32(base, frame + kCaps + 20, 0);
  guest::StoreU32(base, frame + kCaps + 0, 2);
  guest::StoreU32(base, frame + kCaps + 8, 2);
  guest::StoreU32(base, frame + kCaps + 4, 4);
  guest::StoreU32(base, frame + kCaps + 12, 8);
  for (uint32_t k = 6; k < 13; k++) {
    guest::StoreU32(base, frame + kCaps + 4 * k, 0);
  }
  for (uint32_t l = 0; l < 6; l++) {
    const uint32_t offers = save + kOffers + 100 * l;
    int32_t picks = 0;
    for (uint32_t k = 0; k < 13; k++) {
      const int32_t count = int32_t(guest::LoadU32(base, frame + kCounts + 4 * l + 24 * k));
      if (count == 0) {
        continue;
      }
      const int32_t cap = int32_t(guest::LoadU32(base, frame + kCaps + 4 * k));
      const int32_t n = count > cap ? cap : count;
      for (uint32_t w = 0; w < 200; w++) {
        guest::StoreU32(base, frame + kPicked + 4 * w, 0);
      }
      const uint32_t list = 192 * l + 1152 * k;
      int32_t taken = 0;
      while (picks < 8) {
        int32_t index;
        do {
          const int32_t r = int32_t(race_overlays::Rand(base));
          index = r - (r / count) * count;
        } while (guest::LoadU32(base, frame + kPicked + (uint32_t(index) << 2)) != 0);
        guest::StoreU32(base, frame + kPicked + (uint32_t(index) << 2), 1);
        const uint32_t from = frame + kEntries + ((list + uint32_t(index)) << 3);
        const uint32_t table = guest::LoadU32(base, from + 0);
        const uint32_t entry = guest::LoadU32(base, from + 4);
        const uint32_t at = guest::LoadU32(base, offers + 96) * 12 + offers;
        guest::StoreU32(base, at + 0, 0);
        guest::StoreU32(base, at + 4, table);
        guest::StoreU32(base, at + 8, entry);
        guest::StoreU32(base, offers + 96, guest::LoadU32(base, offers + 96) + 1);
        taken++;
        picks++;
        if (taken >= n) {
          break;
        }
      }
    }
  }

  // The race event, for every location.
  guest::StoreU32(base, frame + kLocations, 0xFFFFFFFFu);
  if (race_event::MakeNative(base)) {
    const auto made = race_event::Make(base, guest::LoadU32(base, sp + 28),
                                       guest::LoadU32(base, sp + 36), frame + kLocations,
                                       guest.Down(kSetUpEventFrame));
    if (made) {
      return *made;
    }
  }
  return make(user, frame);
}

}  // namespace career
