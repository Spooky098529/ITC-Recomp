// The objects the game keeps for the whole session (data_load.h,
// docs/functions/data-load.md): the race data's, the input recordings' and
// the replay's makers.

#include "data_load/data_load.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "race_setup/race_setup.h"

namespace data_load {

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
  guest::StoreU32(base, kRaceDataSlot, data);
  return data;
}

uint32_t InputRecordingsConstruct(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_821054A8 InputRecordings::InputRecordings");
  for (const uint32_t first : {0x20FE90u, 0x2196A0u}) {
    for (uint32_t i = 0; i < 7; i++) {
      Record5232Construct(base, block + first + kRecordBytes * i);
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
  guest::StoreU32(base, kInputRecordingsSlot, block);
  guest::StoreU32(base, block + 0x2225B0, 0);
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
    for (uint32_t i = 0; i < 7; i++) {
      r3 = Record5232Construct(base, block + 0x2BF10 + kRecordBytes * i);
    }
  }
  guest::StoreU32(base, kReplaySlot, block);
  guest::StoreU32(base, block + 0x2B600, 1);
  return r3;
}

}  // namespace data_load
