// Career::SetUpEvent (docs/functions/career.md): the day's offers picked from
// the data tables and the race event made, over the guest layout of a
// player's save data and the original's 121,200-byte frame (its locations'
// set, which RaceEvent::Make reads, kept there; its lists and counts host
// words since E5 step 6).

#include "career/career.h"

#include "base/log.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "race_overlays/race_overlays.h"
#include "race_setup/race_setup.h"
#include "schema/career.h"

#include <vector>

namespace career {

namespace {

// The frame's parts (offsets from the frame's stack pointer).
constexpr uint32_t kLocations = 80;  // a 64-bit set of locations
constexpr uint32_t kCaps = 96;       // 13 words: the picks allowed by kind
constexpr uint32_t kCounts = 160;    // words by (kind x 6 + location)
constexpr uint32_t kPicked = 480;    // 200 words: the entries picked
constexpr uint32_t kEntries = 1280;  // {table, index} by (kind x 6 + location) x 192 + n
// The table words' tables 1 and 2 (+0x1B278, +0x1B578 in the save data):
// 768 bytes a table.
constexpr uint32_t kTable1Words = 0x300;
constexpr uint32_t kTableWords = 0x600;
// An offers record's count (100 bytes a location).
constexpr uint32_t kOfferCount = 96;
// RaceData::ResetGlobal's frame (its native r3 is from below it).
constexpr uint32_t kResetFrame = 112;

// The frame's caps, counts, picks and entries (+96 to its end), which only
// this function reads: host words since E5 step 6, at the same offsets. An
// offset past the frame's end (an entry kind above 12, never in the data)
// reaches the guest stack where the original's would.
class Lists {
 public:
  Lists(uint32_t frame)
      : frame_(frame), words_((kSetUpEventFrame - kCaps) / 4) {}
  uint32_t Load(uint32_t offset) const {
    const uint32_t i = (offset - kCaps) / 4;
    return i < words_.size() ? words_[i] : guest::LoadU32(frame_ + offset);
  }
  void Store(uint32_t offset, uint32_t value) {
    const uint32_t i = (offset - kCaps) / 4;
    if (i < words_.size()) {
      words_[i] = value;
    } else {
      guest::StoreU32(frame_ + offset, value);
    }
  }

 private:
  uint32_t frame_;
  std::vector<uint32_t> words_;
};

uint32_t Slw(uint32_t value, uint32_t amount) {
  amount &= 0x3F;
  return (amount & 0x20) != 0 ? 0 : value << amount;
}

uint32_t BitWord(uint32_t set, int32_t b) { return set + (uint32_t(b >> 5) << 2); }

bool BitSet(uint32_t set, int32_t b) {
  return (guest::LoadU32(BitWord(set, b)) & Slw(1, uint32_t(b) & 31)) != 0;
}

void SetBit(uint32_t set, int32_t b) {
  const uint32_t word = BitWord(set, b);
  guest::StoreU32(word, guest::LoadU32(word) | Slw(1, uint32_t(b) & 31));
}

// An entry's three conditions {kind, value} at +24 (all needed when +20 is
// set, else any; none listed passes): Condition::Test's answers.
bool ConditionsPass(uint32_t entry, uint32_t location, uint32_t frame) {
  const schema::CareerEntry56 e(entry);
  bool grant = false;
  const bool all = e.all_conditions() != 0;
  for (int32_t k = 0; k < 3; k++) {
    const uint32_t condition = e.conditions_address() + 8 * uint32_t(k);
    const schema::CareerCondition c(condition);
    if (int32_t(c.kind()) == -1 || int32_t(c.value()) == -1) {
      if (k == 0) {
        grant = true;
      }
      break;
    }
    const uint32_t pass = ConditionTest(condition, location, 0, frame).value_or(0);
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
// added to that list.
void Collect(Lists& lists, uint32_t entry, int32_t location, uint32_t table,
             uint32_t index) {
  const uint32_t at = schema::CareerEntry56(entry).kind() * 6 + uint32_t(location) - 14;
  const uint32_t count_at = kCounts + (at << 2);
  const uint32_t n = lists.Load(count_at);
  const uint32_t slot = (at * 192 + n) << 3;
  lists.Store(count_at, n + 1);
  lists.Store(kEntries + slot, table);
  lists.Store(kEntries + 4 + slot, index);
}

// Kinds 0, 1, 2 and 4 are left out of a table's entries whose word is 2.
bool Done(int32_t kind, uint32_t word) {
  return word == 2 && (kind == 0 || kind == 1 || kind == 2 || kind == 4);
}

}  // namespace

bool SetUpEventNative(uint32_t sp) {
  const uint32_t frame = sp - kSetUpEventFrame;
  return ConditionTestFits(frame) && race_setup::RaceDataResetNative(frame - kResetFrame);
}

uint64_t SetUpEvent(uint32_t save, uint32_t mode, uint32_t location, uint32_t sp,
                    const race_event::Guest& guest, MakeCall make, void* user) {
  ITC_NATIVE_RUNNING("sub_82114268 Career::SetUpEvent");
  const uint32_t frame = sp - kSetUpEventFrame;
  const schema::CareerSave sv(save);
  // The arguments in the caller's frame (RaceEvent::Make reads them there).
  guest::StoreU32(sp + 28, mode);
  guest::StoreU32(sp + 36, location);
  // Locations 14 to 19.
  guest::StoreU32(frame + kLocations + 0, 0);
  guest::StoreU32(frame + kLocations + 4, 0);
  for (int32_t l = 14; l < 20; l++) {
    SetBit(frame + kLocations, l);
  }
  sv.set_mode(mode);
  sv.set_location(location);
  sv.set_next_location(48);
  race_setup::RaceDataResetGlobal(frame - kResetFrame);
  const int32_t day = int32_t(sv.day());
  if (day <= 999 && day == 1) {
    // RaceData::ResetGlobal's r3 (its adapter's).
    return uint64_t(frame) - kResetFrame - 112 - 128 - race_setup::kRouteFrameBytes + 80;
  }

  // Table 0's unlocks, for a location 14 to 19.
  const int32_t here = int32_t(guest::LoadU32(sp + 36));
  if (here >= 0 && BitSet(frame + kLocations, here)) {
    for (int32_t i = 0;; i++) {
      if (i >= 0 && BitSet(sv.table0_done_address(), i)) {
        continue;
      }
      const uint32_t entry = data_tables::Entry56(0, uint32_t(i));
      const schema::CareerEntry56 e(entry);
      if (int32_t(e.kind()) == -1) {
        break;
      }
      const uint32_t mask = e.mask();
      if ((mask & 0x40) != 0 && (mask & 0x3F) == 0) {
        continue;
      }
      const uint32_t stage = StageOf(save);
      if (Entry56Allows(entry, sv.mode(), sv.location(), stage, 1) == 0) {
        continue;
      }
      if (!ConditionsPass(entry, 0, frame)) {
        continue;
      }
      if (i >= 0) {
        SetBit(sv.table0_done_address(), i);
      }
      const uint32_t list = sv.unlocks_shown_address();
      const uint32_t at = sv.unlocks_shown_count() * 12 + list;
      guest::StoreU32(at + 0, 0);
      guest::StoreU32(at + 4, 0);
      guest::StoreU32(at + 8, uint32_t(i));
      sv.set_unlocks_shown_count(sv.unlocks_shown_count() + 1);
    }
  }

  // The offers cleared, and the counts.
  Lists lists(frame);
  for (uint32_t l = 0; l < 6; l++) {
    guest::StoreU32(sv.offers_address() + kOfferCount + 100 * l, 0);
    for (uint32_t k = 0; k < 13; k++) {
      lists.Store(kCounts + 4 * l + 24 * k, 0);
    }
  }

  // Table 1's entries, for each location 14 to 19.
  for (int32_t l = 14; l < 20; l++) {
    uint32_t i = 0;
    uint32_t word = sv.table_words_address() + kTable1Words;
    for (uint32_t entry = data_tables::Entry56(1, 0);
         int32_t(schema::CareerEntry56(entry).kind()) != -1;
         i++, word += 4, entry = data_tables::Entry56(1, i)) {
      const int32_t kind = int32_t(schema::CareerEntry56(entry).kind());
      if (Done(kind, guest::LoadU32(word))) {
        continue;
      }
      const uint32_t stage = StageOf(save);
      if (Entry56Allows(entry, sv.mode(), uint32_t(l), stage, 1) == 0) {
        continue;
      }
      if (!ConditionsPass(entry, 1, frame)) {
        continue;
      }
      Collect(lists, entry, l, 1, i);
    }
  }

  // Tables 2 to 7, each for one location (table t for location t + 12).
  for (int32_t l = 14; l < 20; l++) {
    const uint32_t table = uint32_t(l - 12);
    uint32_t i = 0;
    uint32_t word = sv.table_words_address() + kTableWords + 768 * uint32_t(l - 14);
    for (uint32_t entry = data_tables::Entry56(table, 0);
         int32_t(schema::CareerEntry56(entry).kind()) != -1;
         i++, word += 4, entry = data_tables::Entry56(table, i)) {
      const schema::CareerEntry56 e(entry);
      const int32_t kind = int32_t(e.kind());
      if (Done(kind, guest::LoadU32(word))) {
        continue;
      }
      const int32_t stage = int32_t(StageOf(save));
      const uint32_t entry_mode = sv.mode();
      if (kind == 4) {
        continue;
      }
      // The entry's mask: bits 7 to 9 the modes 0, 1 and others (mode 3
      // untested), bits 10 to 14 the stages (13 for stage 4).
      const uint32_t mask = e.mask();
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
      if (!ConditionsPass(entry, table, frame)) {
        continue;
      }
      Collect(lists, entry, l, table, i);
    }
  }

  // The picks: for each location, up to 8 entries in all, each kind's at
  // random without repeats, at most its cap (2, 4, 2, 8, then 0: a kind
  // capped at 0 still gets one).
  lists.Store(kCaps + 16, 0);
  lists.Store(kCaps + 20, 0);
  lists.Store(kCaps + 0, 2);
  lists.Store(kCaps + 8, 2);
  lists.Store(kCaps + 4, 4);
  lists.Store(kCaps + 12, 8);
  for (uint32_t k = 6; k < 13; k++) {
    lists.Store(kCaps + 4 * k, 0);
  }
  for (uint32_t l = 0; l < 6; l++) {
    const uint32_t offers = sv.offers_address() + 100 * l;
    int32_t picks = 0;
    for (uint32_t k = 0; k < 13; k++) {
      const int32_t count = int32_t(lists.Load(kCounts + 4 * l + 24 * k));
      if (count == 0) {
        continue;
      }
      const int32_t cap = int32_t(lists.Load(kCaps + 4 * k));
      const int32_t n = count > cap ? cap : count;
      for (uint32_t w = 0; w < 200; w++) {
        lists.Store(kPicked + 4 * w, 0);
      }
      const uint32_t list = 192 * l + 1152 * k;
      int32_t taken = 0;
      while (picks < 8) {
        int32_t index;
        do {
          const int32_t r = int32_t(race_overlays::Rand());
          index = r - (r / count) * count;
        } while (lists.Load(kPicked + (uint32_t(index) << 2)) != 0);
        lists.Store(kPicked + (uint32_t(index) << 2), 1);
        const uint32_t from = kEntries + ((list + uint32_t(index)) << 3);
        const uint32_t table = lists.Load(from + 0);
        const uint32_t entry = lists.Load(from + 4);
        const uint32_t at = guest::LoadU32(offers + kOfferCount) * 12 + offers;
        guest::StoreU32(at + 0, 0);
        guest::StoreU32(at + 4, table);
        guest::StoreU32(at + 8, entry);
        guest::StoreU32(offers + kOfferCount, guest::LoadU32(offers + kOfferCount) + 1);
        taken++;
        picks++;
        if (taken >= n) {
          break;
        }
      }
    }
  }

  // The race event, for every location.
  guest::StoreU32(frame + kLocations, 0xFFFFFFFFu);
  if (race_event::MakeNative()) {
    const auto made = race_event::Make(guest::LoadU32(sp + 28),
                                       guest::LoadU32(sp + 36), frame + kLocations,
                                       guest.Down(kSetUpEventFrame));
    if (made) {
      return *made;
    }
  }
  return make(user, frame);
}

}  // namespace career
