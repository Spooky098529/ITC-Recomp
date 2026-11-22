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

using G = schema::RaceStatesGlobals;
using Machine = schema::RaceStates;
using List = schema::CourseTimeList;
using Entry = schema::CourseTimeEntry;

namespace {

constexpr uint32_t kMemcpy = 0x825A7410;

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
  const List l(base, list);
  if (n == 0) {
    return l.first_address();
  }
  uint32_t at = l.nodes_address() + (uint32_t(int32_t(int16_t(l.first()))) << 2);
  for (uint32_t k = n - 1; k != 0; k--) {
    at = l.nodes_address() + (uint32_t(S16(base, at)) << 2);
  }
  return at;
}

// The entry at node `n` of the list (its halfword +2 an index into the
// entries at +52).
uint32_t EntryAt(uint8_t* base, uint32_t list, uint32_t n) {
  const List l(base, list);
  uint32_t node = l.nodes_address() + (uint32_t(int32_t(int16_t(l.first()))) << 2);
  for (uint32_t k = n; k != 0; k--) {
    node = l.nodes_address() + (uint32_t(S16(base, node)) << 2);
  }
  return l.entries_address() + uint32_t(S16(base, node + 2)) * Entry::kSize;
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
  const Entry e(base, entry);
  e.set_word_0(0);
  e.set_word_4(0);
  e.set_word_8(0);
  e.set_word_12(0);
  e.set_word_16(0);
  e.set_time_a(G::no_time(base));
  e.set_time_b(G::no_time(base));
}

uint32_t CourseRecordPlace(uint8_t* base, uint32_t list, uint32_t time, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82105EA0 CourseRecord::Place");
  const uint32_t frame = sp - 128;
  Store(base, sp + 28, time);
  uint32_t place = 0;
  const List l(base, list);
  const uint32_t count = uint32_t(int32_t(int16_t(l.count())));
  if (count == 0) {
    return 0;
  }
  for (uint32_t i = 0;;) {
    const Entry entry(base, EntryAt(base, list, i));
    const uint64_t total = TimeAdd(entry.time_a(), entry.time_b());
    Store(base, frame + 80, uint32_t(total));
    if (achievements::TimeLess(Load(base, sp + 28), Load(base, frame + 80)) != 0) {
      return place;
    }
    i++;
    place = i;
    if (!(i < uint32_t(int32_t(int16_t(l.count()))))) {
      return place;
    }
  }
}

void CourseRecordLink(uint8_t* base, uint32_t list, int32_t n) {
  ITC_NATIVE_RUNNING("sub_82105D60 CourseRecord::Link");
  const List l(base, list);
  const uint32_t before = LinkBefore(base, list, uint32_t(n));
  const uint32_t node = l.nodes_address() + (uint32_t(int32_t(int16_t(l.free_first()))) << 2);
  const uint16_t index = l.free_first();
  l.set_free_first(U16(base, node));
  StoreU16(base, node, index);
  StoreU16(base, node, U16(base, before));
  StoreU16(base, before, index);
  l.set_count(uint16_t(l.count() + 1));
}

void CourseRecordUnlink(uint8_t* base, uint32_t list, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_82105DC8 CourseRecord::Unlink");
  const List l(base, list);
  const uint32_t before = LinkBefore(base, list, n);
  const uint16_t index = U16(base, before);
  const uint32_t node = l.nodes_address() + (uint32_t(int32_t(int16_t(index))) << 2);
  const uint16_t next = U16(base, node);
  StoreU16(base, node, index);
  StoreU16(base, before, next);
  const uint16_t freed = U16(base, node);
  const uint16_t free_head = l.free_first();
  l.set_free_first(freed);
  StoreU16(base, node, free_head);
  l.set_count(uint16_t(l.count() - 1));
}

void CourseRecordInsert(uint8_t* base, uint32_t list, uint32_t entry, uint32_t n, uint32_t sp,
                        const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82105E30 CourseRecord::Insert");
  CourseRecordLink(base, list, int16_t(n));
  Copy(base, EntryAt(base, list, n), entry, Entry::kSize, sp - 112, 0x82105E98, call);
}

uint32_t CourseRecordAdd(uint8_t* base, uint32_t list, uint32_t entry, uint32_t source,
                         uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82105F50 CourseRecord::Add");
  const uint32_t frame = sp - 128;
  const Entry e(base, entry);
  const uint64_t total = TimeAdd(e.time_a(), e.time_b());
  Store(base, frame + 80, uint32_t(total));
  const uint32_t place = CourseRecordPlace(base, list, Load(base, frame + 80), frame);
  const List l(base, list);
  if (int32_t(place) == 0) {
    Copy(base, l.best_address(), source, 76, frame, 0x82105FA4, call);
  }
  if (int32_t(place) < 10) {
    if (l.count() == 10) {
      CourseRecordUnlink(base, list, 9);
    }
    CourseRecordInsert(base, list, entry, place, frame, call);
  }
  return place;
}

void CourseRecordsSave(uint8_t* base, uint32_t records, uint32_t race, uint32_t sp,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82105C18 CourseRecords::Save");
  const schema::RaceData data(base, race);
  const uint32_t stages = data.stages_address();
  const uint32_t route = data.route_course();
  const uint32_t flag = data.word_59618() == 0 ? 1 : 0;
  const int32_t type = int32_t(va::StagesType(base, stages, 0xFFFFFFFFu));
  if (type == 1 || type == 3) {
    const uint32_t slot = (route << 1) + flag;
    Copy(base, records + slot * List::kSize + 20, data.stages_course_record_address(),
         List::kSize, sp - 128, 0x82105C88, call);
  }
}

uint64_t OnRaceEnd(uint8_t* base, uint64_t machine_r3, uint32_t frame, const GuestCall& call,
                   bool keep_native) {
  ITC_NATIVE_RUNNING("sub_821644D8 RaceStates::OnRaceEnd");
  const uint32_t machine = uint32_t(machine_r3);
  const Machine m(base, machine);
  const schema::RaceData race_first = G::race_data(base);
  if (race_first.flags() & 0x80) {
    race_first.set_flags(race_first.flags() & ~0x40u);
  } else {
    // The results kept.
    const uint32_t records = race_first.results_address();
    const uint32_t record = m.progress().results_address();
    uint32_t added = 0;
    if (!va::RacerRecordsAdd(base, records, record, &added)) {
      call(0x820FFE08, frame, 0x82164518, records, record, 0, 0, 0);
    }
    const int32_t mode = int32_t(G::race_data(base).race_mode());
    int32_t kind = -1;  // the achievements' kind, when checked
    if (mode == 0 || mode == 4) {
      // The course record.
      const schema::RaceData race = G::race_data(base);
      const uint32_t list = race.stages_course_record_address();
      const uint32_t entry = frame + 80;
      CourseEntryClear(base, entry);
      const schema::RaceProgress progress = m.progress();
      const uint32_t time = progress.e0_time();
      Store(base, frame + 100, time);
      Store(base, frame + 92, progress.e0_track_hits());
      Store(base, frame + 96, progress.e0_car_hits());
      Store(base, frame + 104, progress.e0_lap_record_64());
      const uint32_t index = progress.e0_record_index();
      Store(base, frame + 80,
            Load(base, index * kRecordBytes + G::race_data(base).record0_place_address()));
      Store(base, frame + 84, race.effects_word());
      Store(base, frame + 88, race.mode());
      const uint32_t place = CourseRecordPlace(base, list, time, frame);
      m.progress().set_e0_place(place);
      CourseRecordAdd(base, list, entry, m.progress().e0_first_lap_record_address(), frame, call);
      const uint32_t save = va::SaveDataOf(base, 0);
      CourseRecordsSave(base, save + 0x1D52C, G::race_data(base).address, frame, call);
      kind = 1;
    } else if (mode == 2 || mode == 3) {
      const uint32_t save = va::SaveDataOf(base, 0);
      const uint32_t race = G::race_data(base).address;
      if (career::AddRaceResultNative(base, save, race, frame)) {
        career::StatsAddRaceResult(base, save, race, frame);
      } else {
        call(0x82115190, frame, 0x821645A8, save, race, 0, 0, 0);
      }
      kind = 0;
    } else if (mode == 7) {
      kind = 1;
    } else if (mode == 8) {
      const uint32_t which = int32_t(m.progress().end_reason()) == 1 ? 0 : 1;
      const uint32_t identity = va::VehicleIdentity(va::VehicleGet(base, which));
      const schema::RaceProgress to = m.progress();
      to.set_kind8_section(U16(base, TrackPos(base, identity).route_section_address()));
      to.set_kind8_slice(U16(base, TrackPos(base, identity).route_slice_address()));
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
    if (int32_t(G::race_data(base).kind()) == 1) {
      save = va::SaveDataOf(base, 0);
    }
    const uint32_t race = G::race_data(base).address;
    const uint32_t recordings = G::recordings(base);
    const uint32_t replay = G::replay(base);
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
    const schema::RaceData data = G::race_data(base);
    data.set_flags(data.flags() & ~0x40u);
  }
  // The flags lose 0x40 (above), 0x20 and 0x20000.
  {
    const schema::RaceData data = G::race_data(base);
    data.set_flags(data.flags() & ~0x20u);
  }
  {
    const schema::RaceData data = G::race_data(base);
    data.set_flags(data.flags() & ~0x20000u);
  }
  return machine_r3;
}

}  // namespace race_states
