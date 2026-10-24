// The racer controllers (racer_controllers.h; docs/functions/racer-controllers.md).

#include "racer_controllers/racer_controllers.h"

#include "guest.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "data_tables/data_tables.h"
#include "memory/c_runtime_heap.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "track_collision/track_collision.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace racer_controllers {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The current stage's word +0 (the stage the race data's list +804 names),
// as Stages::Copy's copy of it holds it.
uint32_t CurrentStageWord0(uint8_t* base, uint32_t race) {
  const uint32_t stages = race + kStages;
  return Load(base, stages + 80 * Load(base, stages + 804));
}

}  // namespace

// --- The cars' input records ---

bool CreateInputs(uint8_t* base, uint32_t* last) {
  ITC_NATIVE_RUNNING("sub_82102258 CarInputs::Create");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  // operator new's error paths are the original's: every block made first,
  // then the records stored and cleared in the original's order.
  uint32_t blocks[7];
  for (int i = 0; i < 7; i++) {
    const auto block = memory::crt::OperatorNew(base, 20);
    if (!block) {
      for (int k = 0; k < i; k++) {
        memory::crt::OperatorDelete(blocks[k]);
      }
      return false;
    }
    blocks[i] = *block;
  }
  for (int i = 0; i < 7; i++) {
    Store(base, kInputs + 4 * i, blocks[i]);
    ai_drivers::ClearCarInput(base, blocks[i]);
  }
  *last = blocks[6];
  return true;
}

bool DestroyInputs(uint8_t* base, uint32_t* last) {
  ITC_NATIVE_RUNNING("sub_82102300 CarInputs::Destroy");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  for (int i = 0; i < 7; i++) {
    const uint32_t block = Load(base, kInputs + 4 * i);
    if (block != 0 && !memory::IsHeapBlock(block)) {
      return false;
    }
  }
  for (int i = 0; i < 7; i++) {
    const uint32_t block = Load(base, kInputs + 4 * i);
    memory::crt::OperatorDelete(block);
    *last = block;
  }
  return true;
}

// --- The input recording ---

uint32_t RecordingAppend(uint8_t* base, uint32_t record, uint32_t player) {
  ITC_NATIVE_RUNNING("sub_821029A8 InputRecording::Append");
  const uint32_t offset = player * kRecordingBytes;
  const uint32_t recording = Load(base, kRecordings) + offset;
  const uint32_t count = Load(base, recording + kRecordingCount);
  if (count >= kRecordingMax) {
    return 0;
  }
  const uint32_t to = recording + 20 * count;
  for (uint32_t i = 0; i < 20; i += 4) {
    Store(base, to + i, Load(base, record + i));
  }
  const uint32_t at = Load(base, kRecordings) + offset + kRecordingCount;
  Store(base, at, Load(base, at) + 1);
  return 1;
}

uint32_t RecordingHasRoom(uint8_t* base, uint32_t player) {
  ITC_NATIVE_RUNNING("sub_82102968 InputRecording::HasRoom");
  const uint32_t recording = Load(base, kRecordings) + player * kRecordingBytes;
  return Load(base, recording + kRecordingCount) < kRecordingMax ? 1 : 0;
}

// --- The race flow's handlers ---

bool OnSetUp(uint8_t* base, uint32_t flow, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8215F728 RaceFlow::OnSetUp");
  uint32_t last = 0;
  if (!CreateInputs(base, &last)) {
    return false;
  }
  const uint32_t race = Load(base, kRaceData);
  const uint32_t stage_word = CurrentStageWord0(base, race);
  const uint32_t type23 =
      vehicle_accessors::StagesIsType2Or3(base, Load(base, kRaceData) + kStages, 0xFFFFFFFFu);
  *r3 = type23;
  if (type23 != 0) {
    return true;
  }
  if (int32_t(Load(base, Load(base, kRaceData) + kMode)) == 6 && int32_t(stage_word) != 1) {
    return true;
  }
  Store(base, flow + 9608, Load(base, flow + 9608) | 1);
  return true;
}

// --- The AI drivers' hand-off ---

namespace {

// The input record of racer `index` (the original's rlwinm: index & 0xFF).
uint32_t InputOf(uint8_t* base, uint32_t index) {
  return Load(base, kInputs + 4 * (index & 0xFF));
}

void Copy20(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t i = 0; i < 20; i += 4) {
    Store(base, to + i, Load(base, from + i));
  }
}

// The racers' index word (+20) of the race's i-th racer record.
uint32_t RacerWord(uint8_t* base, uint32_t i, uint32_t offset) {
  return Load(base, Load(base, Load(base, kRace) + 2308 + 4 * i) + offset);
}

}  // namespace

void DriveByAi(uint8_t* base, uint32_t index, uint32_t hold, uint32_t sp, const Calls& calls,
               uint64_t* r3) {
  ITC_NATIVE_RUNNING("sub_8215F968 RacerControl::DriveByAi");
  if ((Load(base, Load(base, kRaceData) + kFlags) & 0x100) != 0) {
    return;
  }
  const uint32_t frame = sp - 160;
  const uint32_t record = frame + 112;
  Store(base, frame, sp);  // the frame's back chain, for the call
  calls.drive_car(calls.user, record, index, hold, frame, 0x8215F9A8);
  Copy20(base, InputOf(base, index), record);
  // The copy loop's r3 (DriveCar's, the record, past its five words).
  *r3 = record + 20;
}

uint32_t HoldAll(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8215FA10 RaceFlow::HoldAll");
  const uint32_t frame = sp - 112;
  for (int32_t i = 0; i < int32_t(track_collision::RaceRacerCount(base)); i++) {
    uint64_t r3 = 0;
    DriveByAi(base, RacerWord(base, uint32_t(i), 20), 1, frame, calls, &r3);
  }
  return 1;
}

uint32_t ClearAllRecords(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8215F5F0 RaceFlow::ClearAllRecords");
  const uint32_t record = sp - 144 + 80;
  ai_drivers::ClearCarInput(base, record);
  for (int32_t i = 0; i < int32_t(track_collision::RaceRacerCount(base)); i++) {
    Copy20(base, InputOf(base, RacerWord(base, uint32_t(i), 20)), record);
  }
  return 1;
}

uint32_t OnState16(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8215FB90 RaceFlow::OnState16");
  const uint32_t frame = sp - 208;
  const uint32_t stage_word = CurrentStageWord0(base, Load(base, kRaceData));
  if (vehicle_accessors::StagesIsType2Or3(base, Load(base, kRaceData) + kStages, 0xFFFFFFFFu) ==
          0 &&
      (int32_t(Load(base, Load(base, kRaceData) + kMode)) != 6 || int32_t(stage_word) == 1)) {
    return HoldAll(base, frame, calls);
  }
  return ClearAllRecords(base, frame);
}

// --- The AI drivers' set-up ---

uint64_t Setup(uint8_t* base, uint32_t index, uint32_t id, uint32_t hold, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82170FF8 AiDriver::Setup");
  const uint32_t frame = sp - 128;
  const uint64_t driver64 = ai_drivers::OfIndex(base, Load(base, index));
  const uint32_t driver = uint32_t(driver64);
  ai_drivers::ResetDriver(base, driver);
  const uint32_t item_id = Load(base, id);
  const uint32_t item = int32_t(item_id) < 0 ? 0 : data_tables::Item(base, item_id);
  ai_drivers::SetParams(base, driver, item);
  if (Load(base, hold) != 0) {
    ai_drivers::ResetControl(base, driver, frame - 112);
    return driver64 + 6048;
  }
  return ai_drivers::StartRace(base, driver, frame - 112);
}

}  // namespace racer_controllers
