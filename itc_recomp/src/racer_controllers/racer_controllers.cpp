// The racer controllers (racer_controllers.h; docs/functions/racer-controllers.md).

#include "racer_controllers/racer_controllers.h"

#include "guest.h"

#include "action_mappers/action_mappers.h"
#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "data_tables/data_tables.h"
#include "debug/pause_menu_log.h"
#include "memory/c_runtime_heap.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "track_collision/track_collision.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

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

// --- The player's controls ---

uint32_t MapperEntry(uint8_t* base, uint32_t player) {
  return kMapperEntries + (Load(base, kPlayerEntries + (player << 2)) << 3);
}

namespace {

// 0.0 ([0x82000A34]), read as the originals do.
constexpr uint32_t kZero = 0x82000A34;

float LoadFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(Load(base, address));
}

}  // namespace

void Read(uint8_t* base, uint32_t player, uint32_t id, uint32_t kind, uint32_t out, uint32_t sp,
          const Calls& calls, Regs* regs) {
  ITC_NATIVE_RUNNING("sub_82102350 PlayerInput::Read");
  const uint32_t entry = MapperEntry(base, player);
  if (kind >= 5) {
    return;
  }
  const bool unread = int32_t(Load(base, entry + 4)) == 1;
  if (kind == 4) {
    double f1;
    if (unread) {
      f1 = double(LoadFloat(base, kZero));
    } else {
      const uint32_t mapper = Load(base, entry + 0);
      const uint32_t function = Load(base, Load(base, mapper) + 20);
      Store(base, sp - 96, sp);
      regs->r3 = calls.call(calls.user, function, mapper, id, true, sp - 96, 0x821023D0, &f1);
    }
    regs->f1 = f1;
    regs->f1_set = true;
    Store(base, out, std::bit_cast<uint32_t>(float(f1)));
    return;
  }
  if (unread) {
    if (kind == 3) {
      regs->r3 = 0;
    }
    Store(base, out, 0);
    return;
  }
  static constexpr uint32_t kSlotOffsets[4] = {32, 40, 48, 56};
  const uint32_t lr = kind == 3 ? 0x82102400 : 0x82102468;
  const uint32_t mapper = Load(base, entry + 0);
  const uint32_t function = Load(base, Load(base, mapper) + kSlotOffsets[kind]);
  uint64_t r3;
  int slot = -1;
  if (calls.native_slots && function == 0x8210BE70) {
    slot = 8;
    r3 = action_mappers::Held(base, mapper, id);
  } else if (calls.native_slots && function == 0x8210BF40) {
    slot = 10;
    r3 = action_mappers::Pressed(base, mapper, id);
  } else if (calls.native_slots && function == 0x8210C010) {
    slot = 12;
    r3 = action_mappers::Fired(base, mapper, id);
  } else if (calls.native_slots && function == 0x8210BA28) {
    slot = 14;
    r3 = action_mappers::AnalogRaw(base, mapper, id);
  } else {
    Store(base, sp - 96, sp);
    r3 = calls.call(calls.user, function, mapper, id, true, sp - 96, lr, nullptr);
  }
  if (slot >= 0) {
    // The read slots' adapters' line for --log_pause_menu.
    debug::PauseMenuLog_NoteSlotRead(slot, mapper, id, lr,
                                     double(slot == 14 ? uint32_t(r3) & 0xFFFF : uint32_t(r3)));
  }
  regs->r3 = r3;
  Store(base, out, kind == 3 ? uint32_t(r3) : uint32_t(r3) & 0xFF);
}

uint32_t Control(uint8_t* base, uint32_t player, uint32_t control, uint32_t kind, uint32_t out,
                 uint32_t sp, const Calls& calls, Regs* regs) {
  ITC_NATIVE_RUNNING("sub_82102488 PlayerInput::Control");
  const uint32_t frame = sp - 112;
  const auto read = [&](uint32_t id) { Read(base, player, id, kind, out, frame, calls, regs); };
  // One action: returns `answer`.
  const auto single = [&](uint32_t id, uint32_t answer) {
    read(id);
    return answer;
  };
  // An axis of two (`first`, then `second` when the first reads 0).
  const auto axis = [&](uint32_t first, uint32_t second, uint32_t answer) {
    read(first);
    if (kind < 3) {
      if (Load(base, out) != 0) {
        return answer;
      }
    } else if (kind == 3) {
      const uint32_t value = Load(base, out);
      if (value != 0) {
        Store(base, out, uint32_t(0) - value);
        return answer;
      }
    } else if (kind == 4) {
      const double value = double(LoadFloat(base, out));
      if (!(value == double(LoadFloat(base, kZero)))) {
        const double negated = std::bit_cast<double>(std::bit_cast<uint64_t>(value) ^
                                                     0x8000000000000000ull);
        Store(base, out, std::bit_cast<uint32_t>(float(negated)));
        return answer;
      }
    } else {
      return answer;
    }
    read(second);
    return answer;
  };
  switch (control) {
    case 1: return axis(155, 156, 1);
    case 2: return axis(154, 153, 1);
    case 3: return single(155, 1);
    case 4: return single(156, 1);
    case 5: return single(153, 1);
    case 6: return single(154, 1);
    case 7: return single(159, 1);
    case 8: return single(158, 1);
    case 9: return single(160, 1);
    case 10: return single(157, 1);
    case 11: return single(152, 1);
    case 12: return single(151, 1);
    case 13: return single(163, 0);
    case 14: return single(164, 0);
    case 15: return single(161, 1);
    case 16: return single(162, 1);
    case 17: return single(165, 1);
    case 18: return axis(169, 170, 0);
    case 19: return axis(168, 167, 0);
    case 20: return single(169, 0);
    case 21: return single(170, 0);
    case 22: return single(167, 0);
    case 23: return single(168, 0);
    case 24: return single(166, 1);
    case 25: return axis(173, 174, 0);
    case 26: return axis(172, 171, 0);
    case 27: return single(173, 0);
    case 28: return single(174, 0);
    case 29: return single(171, 0);
    case 30: return single(172, 0);
    default:  // 0 and above 30
      return 0;
  }
}

namespace {

// The player's pad: the input manager's slot 9 (+36), then that object's
// slot 5 (+20) with &the player at frame + 80 (`frame` the original's).
uint64_t FindPad(uint8_t* base, uint32_t entry, uint32_t sp, uint32_t frame_bytes,
                 uint32_t lr9, uint32_t lr5, const Calls& calls) {
  const uint32_t frame = sp - frame_bytes;
  const uint32_t mapper = Load(base, entry + 0);
  const uint32_t manager = Load(base, kInputManager);
  Store(base, frame, sp);
  Store(base, frame + 80, Load(base, mapper + 2888));
  const uint64_t object = calls.call(calls.user, Load(base, Load(base, manager) + 36), manager, 0,
                                     false, frame, lr9, nullptr);
  return calls.call(calls.user, Load(base, Load(base, uint32_t(object)) + 20), object,
                    frame + 80, true, frame, lr5, nullptr);
}

}  // namespace

uint32_t PadIsConnected(uint8_t* base, uint32_t entry, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8209F340 Pad::IsConnected");
  const uint64_t pad = FindPad(base, entry, sp, 96, 0x8209F370, 0x8209F384, calls);
  return Load(base, uint32_t(pad) + 4) & 1;
}

uint32_t PadIsWheel(uint8_t* base, uint32_t entry, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8209F3B8 Pad::IsWheel");
  const uint64_t pad = FindPad(base, entry, sp, 112, 0x8209F3F0, 0x8209F404, calls);
  return (guest::LoadU8(base, uint32_t(pad) + 193) & 2) == 2 ? 1 : 0;
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
