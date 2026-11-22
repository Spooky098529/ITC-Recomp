// The objects the game keeps for the whole session (data_load.h,
// docs/functions/data-load.md): the race data's, the input recordings' and
// the replay's makers, and the two static replays' set-ups.

#include "data_load/data_load.h"

#include "base/log.h"
#include "c_runtime/exit.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "race_setup/race_setup.h"
#include "schema/data_load.h"
#include "schema/replay_snapshots.h"
#include "schema/type10_mode.h"

namespace data_load {

namespace {

using G = schema::DataLoadGlobals;

// The input recordings' two snapshot blocks, A (the race's start) and B (its
// end); no schema type describes the input recordings' block yet
// (replay-snapshots.toml's input_recordings global names the offsets).
constexpr uint32_t kSnapshotA = 0x20F590;
constexpr uint32_t kSnapshotB = 0x218DA0;
// +0x2225B0, set to 1 by the restores.
constexpr uint32_t kRestored = 0x2225B0;

// A kept replay's seven records: its snapshot's (+0x2BF10).
uint32_t KeptReplayRecords(uint8_t* base, uint32_t replay) {
  const schema::KeptReplay kept(base, replay);
  return schema::ReplaySnapshot(base, kept.snapshot_address()).records_address();
}

}  // namespace

std::optional<uint32_t> RaceDataCreate(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82102208 RaceData::Create");
  const auto made = memory::crt::OperatorNew(base, kRaceDataBytes);
  if (!made) {
    return std::nullopt;
  }
  uint32_t data = *made;
  if (data != 0) {
    data = race_setup::RaceDataConstruct(base, data);
  }
  G::set_race_data(base, data);
  return data;
}

uint32_t InputRecordingsConstruct(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_821054A8 InputRecordings::InputRecordings");
  for (const uint32_t snapshot : {kSnapshotA, kSnapshotB}) {
    const uint32_t records = schema::ReplaySnapshot(base, block + snapshot).records_address();
    for (uint32_t i = 0; i < 7; i++) {
      Record5232Construct(base, records + schema::ReplayRecord::kSize * i);
    }
  }
  return block;
}

std::optional<uint32_t> InputRecordingsCreate(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82105508 InputRecordings::Create");
  const auto made = memory::crt::OperatorNew(base, kInputRecordingsBytes);
  if (!made) {
    return std::nullopt;
  }
  uint32_t block = *made;
  if (block != 0) {
    block = InputRecordingsConstruct(base, block);
  }
  G::set_input_recordings(base, block);
  guest::StoreU32(base, block + kRestored, 0);
  return block;
}

std::optional<uint32_t> ReplayCreate(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82105560 Replay::Create");
  const auto made = memory::crt::OperatorNew(base, kReplayBytes);
  if (!made) {
    return std::nullopt;
  }
  const uint32_t block = *made;
  uint32_t r3 = block;
  if (block != 0) {
    const uint32_t records = KeptReplayRecords(base, block);
    for (uint32_t i = 0; i < 7; i++) {
      r3 = Record5232Construct(base, records + schema::ReplayRecord::kSize * i);
    }
  }
  G::set_replay(base, block);
  schema::KeptReplay(base, block).set_unfilled(1);
  return r3;
}

namespace {

int32_t StaticReplayInit(uint8_t* base, uint32_t object, uint32_t exit_function,
                         const c_runtime::AtexitCall& call) {
  const uint32_t records = KeptReplayRecords(base, object);
  for (uint32_t i = 0; i < 7; i++) {
    Record5232Construct(base, records + schema::ReplayRecord::kSize * i);
  }
  return c_runtime::AtexitFrom(base, exit_function, call);
}

}  // namespace

int32_t StaticReplayAInit(uint8_t* base, const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825D9E60 Replay826D8520::StaticInit");
  return StaticReplayInit(base, G::static_replay_a_address(), kStaticReplayAExit, call);
}

int32_t StaticReplayBInit(uint8_t* base, const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825D9F68 Replay82816E50::StaticInit");
  return StaticReplayInit(base, G::static_replay_b_address(), kStaticReplayBExit, call);
}

}  // namespace data_load
