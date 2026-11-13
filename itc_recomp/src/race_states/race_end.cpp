// The race's end in the race's states (docs/functions/race-states.md):
// RaceStates::OnRaceEnd and the course record it keeps (the race's best
// times: a list of ten 28-byte entries in the race data's stage list, then
// copied into player 0's save data). The course record's functions never
// ran in play; ported with OnRaceEnd (the user's call, 2026-11-12: functions
// never seen in play are ported too).

#include "race_states/race_states.h"
#include "race_states/steps.h"

#include "achievements/achievements.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "career/career.h"
#include "race_setup/race_setup.h"
#include "replay_snapshots/replay_snapshots.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_states {

using namespace steps;
namespace va = vehicle_accessors;

namespace {

constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kNoTime = 0x82006924;     // the entry's empty time
constexpr uint32_t kReplay = 0x826C5B50;     // the kept replay (a pointer)
constexpr uint32_t kRecordings = 0x826C5B4C;  // the input recordings (a pointer)
constexpr uint32_t kFlags = 0x59AE4;         // the race data's flags
constexpr uint32_t kRacerRecords = 0x59AE8;  // the race data's kept results
constexpr uint32_t kStages = 0x59620;        // the race data's stage list
constexpr uint32_t kMode = 0xA3AF4;          // the race data's race mode
constexpr uint32_t kKind = 0xA3AF0;          // the race data's kind

uint16_t U16(uint8_t* base, uint32_t address) {
  return guest::LoadU16(base, address);
}
void StoreU16(uint8_t* base, uint32_t address, uint16_t value) {
  guest::StoreU16(base, address, value);
}

// memcpy's plain C++, or its original through `call` (r1 `sp`, returning to
// `lr`) where it refuses.
void Copy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp, uint32_t lr,
          const GuestCall& call) {
  if (!c_runtime::Memcpy(base, to, from, bytes)) {
    call(kMemcpy, sp, lr, to, from, bytes, 0, 0);
  }
}

// The node `n` steps along the list from its head (+44): the address of the
// link before node n (+44 itself for 0), each step a signed halfword index
// times 4 from the list (the originals' rlwinm, wrapping in 32 bits).
uint32_t LinkBefore(uint8_t* base, uint32_t list, uint32_t n) {
  if (n == 0) {
    return list + 44;
  }
  uint32_t at = list + (uint32_t(S16(base, list + 44)) << 2);
  for (uint32_t k = n - 1; k != 0; k--) {
    at = list + (uint32_t(S16(base, at)) << 2);
  }
  return at;
}

// The entry at node `n` of the list (its halfword +2 an index into the
// entries at +52).
uint32_t EntryAt(uint8_t* base, uint32_t list, uint32_t n) {
  uint32_t node = list + (uint32_t(S16(base, list + 44)) << 2);
  for (uint32_t k = n; k != 0; k--) {
    node = list + (uint32_t(S16(base, node)) << 2);
  }
  return list + 52 + uint32_t(S16(base, node + 2)) * 28;
}

// achievements::Calls over a GuestCall.
uint64_t WriteCall(void* user, uint32_t function, const uint64_t args[3], uint32_t r1,
                   uint32_t lr) {
  const GuestCall& call = *static_cast<const GuestCall*>(user);
  return call(function, r1, lr, args[0], args[1], args[2], 0, 0);
}

}  // namespace

uint64_t TimeAdd(uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820FF458 Time::Add");
  return race_setup::TimePack(uint64_t(Unpack(b) + Unpack(a)));
}

void CourseEntryClear(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82105BC8 CourseEntry::Clear");
  for (uint32_t at = 0; at < 20; at += 4) {
    Store(base, entry + at, 0);
  }
  Store(base, entry + 20, Load(base, kNoTime));
  Store(base, entry + 24, Load(base, kNoTime));
}

uint32_t CourseRecordPlace(uint8_t* base, uint32_t list, uint32_t time, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82105EA0 CourseRecord::Place");
  const uint32_t frame = sp - 128;
  Store(base, sp + 28, time);
  uint32_t place = 0;
  const uint32_t count = uint32_t(S16(base, list + 48));
  if (count == 0) {
    return 0;
  }
  for (uint32_t i = 0;;) {
    const uint32_t entry = EntryAt(base, list, i);
    const uint64_t total = TimeAdd(Load(base, entry + 20), Load(base, entry + 24));
    Store(base, frame + 80, uint32_t(total));
    if (achievements::TimeLess(Load(base, sp + 28), Load(base, frame + 80)) != 0) {
      return place;
    }
    i++;
    place = i;
    if (!(i < uint32_t(S16(base, list + 48)))) {
      return place;
    }
  }
}

void CourseRecordLink(uint8_t* base, uint32_t list, int32_t n) {
  ITC_NATIVE_RUNNING("sub_82105D60 CourseRecord::Link");
  const uint32_t before = LinkBefore(base, list, uint32_t(n));
  const uint32_t node = uint32_t(S16(base, list + 40)) << 2;
  const uint16_t index = U16(base, list + 40);
  StoreU16(base, list + 40, U16(base, list + node));
  StoreU16(base, list + node, index);
  StoreU16(base, list + node, U16(base, before));
  StoreU16(base, before, index);
  StoreU16(base, list + 48, uint16_t(U16(base, list + 48) + 1));
}

void CourseRecordUnlink(uint8_t* base, uint32_t list, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_82105DC8 CourseRecord::Unlink");
  const uint32_t before = LinkBefore(base, list, n);
  const uint16_t index = U16(base, before);
  const uint32_t node = uint32_t(int32_t(int16_t(index))) << 2;
  const uint16_t next = U16(base, list + node);
  StoreU16(base, list + node, index);
  StoreU16(base, before, next);
  const uint16_t freed = U16(base, list + node);
  const uint16_t free_head = U16(base, list + 40);
  StoreU16(base, list + 40, freed);
  StoreU16(base, list + node, free_head);
  StoreU16(base, list + 48, uint16_t(U16(base, list + 48) - 1));
}

void CourseRecordInsert(uint8_t* base, uint32_t list, uint32_t entry, uint32_t n, uint32_t sp,
                        const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82105E30 CourseRecord::Insert");
  CourseRecordLink(base, list, int16_t(n));
  Copy(base, EntryAt(base, list, n), entry, 28, sp - 112, 0x82105E98, call);
}

uint32_t CourseRecordAdd(uint8_t* base, uint32_t list, uint32_t entry, uint32_t source,
                         uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82105F50 CourseRecord::Add");
  const uint32_t frame = sp - 128;
  const uint64_t total = TimeAdd(Load(base, entry + 20), Load(base, entry + 24));
  Store(base, frame + 80, uint32_t(total));
  const uint32_t place = CourseRecordPlace(base, list, Load(base, frame + 80), frame);
  if (int32_t(place) == 0) {
    Copy(base, list + 332, source, 76, frame, 0x82105FA4, call);
  }
  if (int32_t(place) < 10) {
    if (U16(base, list + 48) == 10) {
      CourseRecordUnlink(base, list, 9);
    }
    CourseRecordInsert(base, list, entry, place, frame, call);
  }
  return place;
}

void CourseRecordsSave(uint8_t* base, uint32_t records, uint32_t race, uint32_t sp,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82105C18 CourseRecords::Save");
  const uint32_t stages = race + kStages;
  const uint32_t route = Load(base, race + 4);
  const uint32_t flag = Load(base, race + 0x59618) == 0 ? 1 : 0;
  const int32_t type = int32_t(va::StagesType(base, stages, 0xFFFFFFFFu));
  if (type == 1 || type == 3) {
    const uint32_t slot = (route << 1) + flag;
    Copy(base, records + slot * 408 + 20, stages + 812, 408, sp - 128, 0x82105C88, call);
  }
}

uint64_t OnRaceEnd(uint8_t* base, uint64_t machine_r3, uint32_t frame, const GuestCall& call,
                   bool keep_native) {
  ITC_NATIVE_RUNNING("sub_821644D8 RaceStates::OnRaceEnd");
  const uint32_t machine = uint32_t(machine_r3);
  const uint32_t race_first = Load(base, race_world::kRaceData);
  if (Load(base, race_first + kFlags) & 0x80) {
    const uint32_t at = race_first + kFlags;
    Store(base, at, Load(base, at) & ~0x40u);
  } else {
    // The results kept.
    const uint32_t records = race_first + kRacerRecords;
    const uint32_t record = Load(base, machine + 9624) + 2184;
    uint32_t added = 0;
    if (!va::RacerRecordsAdd(base, records, record, &added)) {
      call(0x820FFE08, frame, 0x82164518, records, record, 0, 0, 0);
    }
    const int32_t mode = int32_t(Load(base, Load(base, race_world::kRaceData) + kMode));
    int32_t kind = -1;  // the achievements' kind, when checked
    if (mode == 0 || mode == 4) {
      // The course record.
      const uint32_t race = Load(base, race_world::kRaceData);
      const uint32_t list = race + kStages + 812;
      const uint32_t entry = frame + 80;
      CourseEntryClear(base, entry);
      const uint32_t progress = Load(base, machine + 9624);
      const uint32_t time = Load(base, progress + 2204);
      Store(base, frame + 100, time);
      Store(base, frame + 92, Load(base, progress + 2996));
      Store(base, frame + 96, Load(base, progress + 3000));
      Store(base, frame + 104, Load(base, progress + 2272));
      const uint32_t index = Load(base, progress + 2188);
      Store(base, frame + 80,
            Load(base, index * 3936 + Load(base, race_world::kRaceData) + 60));
      Store(base, frame + 84, Load(base, race + 28));
      Store(base, frame + 88, Load(base, race + 32));
      const uint32_t place = CourseRecordPlace(base, list, time, frame);
      Store(base, Load(base, machine + 9624) + 2196, place);
      CourseRecordAdd(base, list, entry, Load(base, machine + 9624) + 2208, frame, call);
      const uint32_t save = va::SaveDataOf(base, 0);
      CourseRecordsSave(base, save + 0x1D52C, Load(base, race_world::kRaceData), frame, call);
      kind = 1;
    } else if (mode == 2 || mode == 3) {
      const uint32_t save = va::SaveDataOf(base, 0);
      const uint32_t race = Load(base, race_world::kRaceData);
      if (career::AddRaceResultNative(base, save, race, frame)) {
        career::StatsAddRaceResult(base, save, race, frame);
      } else {
        call(0x82115190, frame, 0x821645A8, save, race, 0, 0, 0);
      }
      kind = 0;
    } else if (mode == 7) {
      kind = 1;
    } else if (mode == 8) {
      const uint32_t which = int32_t(Load(base, Load(base, machine + 9624) + 6108)) == 1 ? 0 : 1;
      const uint32_t identity = va::VehicleIdentity(va::VehicleGet(base, which));
      const uint32_t to = Load(base, machine + 9624) + 7244;
      StoreU16(base, to, U16(base, identity + 4));
      StoreU16(base, to + 2, U16(base, identity + 6));
    }
    if (kind >= 0) {
      const uint32_t words = frame + 112;
      if (int32_t(achievements::Check(base, uint32_t(kind), words, frame)) == 1) {
        const achievements::Calls write{const_cast<GuestCall*>(&call), &WriteCall};
        achievements::Write(base, words, frame, write);
      }
    }
    // The race kept for its replay.
    uint32_t save = 0;
    if (int32_t(Load(base, Load(base, race_world::kRaceData) + kKind)) == 1) {
      save = va::SaveDataOf(base, 0);
    }
    const uint32_t race = Load(base, race_world::kRaceData);
    const uint32_t recordings = Load(base, kRecordings);
    const uint32_t replay = Load(base, kReplay);
    if (keep_native) {
      machine_r3 = replay_snapshots::ReplayKeep(
          base, replay, race, recordings, save, 0, frame - 272,
          [&call](uint32_t out, uint32_t stages, uint32_t stage, uint32_t sp) {
            call(0x820FFD88, sp, 0x8210509C, out, stages, uint64_t(int64_t(int32_t(stage))), 0,
                 0);
          });
    } else {
      machine_r3 = call(0x82104F68, frame, 0x821646BC, replay, race, recordings, save, 0);
    }
    const uint32_t at = Load(base, race_world::kRaceData) + kFlags;
    Store(base, at, Load(base, at) & ~0x40u);
  }
  // The flags lose 0x40 (above), 0x20 and 0x20000.
  uint32_t at = Load(base, race_world::kRaceData) + kFlags;
  Store(base, at, Load(base, at) & ~0x20u);
  at = Load(base, race_world::kRaceData) + kFlags;
  Store(base, at, Load(base, at) & ~0x20000u);
  return machine_r3;
}

}  // namespace race_states
