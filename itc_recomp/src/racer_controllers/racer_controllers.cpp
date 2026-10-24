// The racer controllers (racer_controllers.h; docs/functions/racer-controllers.md).

#include "racer_controllers/racer_controllers.h"

#include "guest.h"

#include "action_mappers/action_mappers.h"
#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "data_tables/data_tables.h"
#include "debug/pause_menu_log.h"
#include "hooks/vehicle.h"
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
  const uint32_t frame = sp - 160;
  const uint32_t record = frame + 112;
  Store(base, frame, sp);  // the frame's back chain, for the call
  if ((Load(base, Load(base, kRaceData) + kFlags) & 0x100) != 0) {
    return;
  }
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

// --- The pad reader and the human cars ---

namespace {

// The settings bytes (+0x1F880 + 3 x player) and control map (+0x1F888 +
// 120 x player) of the save data's player 0 (SaveData::Of(0)).
uint32_t Settings(uint8_t* base, uint32_t player) {
  return vehicle_accessors::SaveDataOf(base, 0) + 3 * player + 0x1F880;
}
uint32_t ControlMap(uint8_t* base, uint32_t player) {
  return vehicle_accessors::SaveDataOf(base, 0) + 120 * player + 0x1F888;
}

int32_t Mul(int32_t a, int32_t b) {
  return int32_t(uint32_t(a) * uint32_t(b));
}

// A one-sided analog (the throttle, the brake, +10): x 110 / 100, at most
// 32,767, 5,000 taken off (0 below), x 32,767 / 27,767, at most 32,767;
// the word at `word` (the control's) left as the original leaves it.
int32_t Pedal(uint8_t* base, uint32_t word) {
  int32_t v = Mul(int32_t(Load(base, word)), 110) / 100;
  if (v > 32767) {
    v = 32767;
  }
  v -= 5000;
  if (v < 0) {
    v = 0;
    Store(base, word, 0);
    return v;
  }
  v = Mul(v, 32767) / 27767;
  Store(base, word, uint32_t(v));
  if (v > 32767) {
    v = 32767;
    Store(base, word, uint32_t(v));
  }
  return v;
}

void OrFlags(uint8_t* base, uint32_t record, uint32_t bits) {
  Store(base, record, Load(base, record) | bits);
}

// The pause: flags 0x1000, message 30 for player 1, then 29.
void PostPause(uint8_t* base, uint32_t record, uint32_t player, uint32_t owner) {
  OrFlags(base, record, 0x1000);
  if (int32_t(player) == 1) {
    PostMessage(base, owner, 30);
  }
  PostMessage(base, owner, 29);
}

}  // namespace

uint32_t RecordingNext(uint8_t* base, uint32_t record, uint32_t player) {
  ITC_NATIVE_RUNNING("sub_82102A28 InputRecording::Next");
  const uint32_t offset = player * kRecordingBytes;
  const uint32_t recording = Load(base, kRecordings) + offset;
  const uint32_t position = Load(base, recording + kRecordingCount);
  if (position >= kRecordingMax) {
    return 0;
  }
  Copy20(base, record, recording + 20 * position);
  const uint32_t at = Load(base, kRecordings) + offset + kRecordingCount;
  Store(base, at, Load(base, at) + 1);
  return 1;
}

void PostMessage(uint8_t* base, uint32_t owner, uint32_t message) {
  const uint32_t ring = owner + 204;
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t at = Load(base, ring + 604) + count - 1;
  Store(base, ring + 600, count);
  const uint32_t record = ring + 12 * (at % 50);
  Store(base, record + 0, message);
  Store(base, record + 4, 0);
  Store(base, record + 8, 0);
}

uint32_t ReadPad(uint8_t* base, uint32_t record, uint32_t index, uint32_t player, uint32_t owner,
                 uint32_t may_pause, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8215FCA8 PlayerControl::ReadPad");
  const uint32_t frame = sp - 288;
  const uint32_t word = frame + 80;    // the analogs' control word
  const uint32_t button = frame + 84;  // the buttons'
  const uint32_t entry = MapperEntry(base, player);
  const uint32_t settings = Settings(base, player);
  uint32_t map = ControlMap(base, player);
  Store(base, frame, sp);  // the frame's back chain, for the calls
  const uint32_t vehicle = vehicle_accessors::VehicleGet(base, index);
  Store(base, button, 0);
  Store(base, word, 0);
  ai_drivers::ClearCarInput(base, record);
  Regs regs{0, 0, false};
  const auto control = [&](uint32_t which, uint32_t kind, uint32_t out) {
    return Control(base, player, which, kind, out, frame, calls, &regs);
  };
  const uint32_t race = Load(base, kRaceData);
  const bool by_stage =
      int32_t(Load(base, race + kMode)) == 6 && int32_t(CurrentStageWord0(base, race)) != 1;
  if (!by_stage && (PadIsConnected(base, entry, frame, calls) == 0 ||
                    int32_t(Load(base, 0x826C7960)) != 0 || int32_t(Load(base, 0x826C7964)) != 0)) {
    if (may_pause != 0) {
      PostPause(base, record, player, owner);
    }
    return 1;
  }
  if (PadIsWheel(base, entry, frame, calls) != 0) {
    map += 60;
    guest::StoreU8(base, record + 17, 1);
  }
  // The throttle (+4).
  uint32_t answer = control(Load(base, map + 4), 3, word);
  OrFlags(base, record, answer != 0 ? 0x08000000 : 0x04000000);
  int32_t v = Pedal(base, word);
  guest::StoreU16(base, record + 4, uint16_t(v));
  if (int16_t(v) != 0) {
    OrFlags(base, record, 0x4);
  }
  guest::StoreU8(base, record + 14, guest::LoadU8(base, settings + 1));
  // The brake (+6), then the reverse and the speed's checks.
  answer = control(Load(base, map + 8), 3, word);
  OrFlags(base, record, answer != 0 ? 0x20000000 : 0x10000000);
  const int16_t brake = int16_t(Pedal(base, word));
  guest::StoreU16(base, record + 6, uint16_t(brake));
  bool have_speed = false;
  double speed = 0;
  const auto fast = [&]() {
    speed = double(Vehicle_GetSpeedKmh(base, vehicle));
    have_speed = true;
    return !(speed < double(LoadFloat(base, 0x82000A30)));
  };
  const auto reverse = [&]() {
    const uint32_t flags = Load(base, record);
    guest::StoreU16(base, record + 6, 0);
    Store(base, record, flags | 0x200);
  };
  if (int32_t(Load(base, vehicle + 2252)) == -1) {
    if (guest::LoadU16(base, record + 4) != 0) {
      if (fast()) {
        guest::StoreU16(base, record + 6, 32767);
      }
    } else if (brake != 0) {
      reverse();
    }
  } else if (brake != 0) {
    if (!fast() && guest::LoadU16(base, record + 4) == 0) {
      reverse();
    } else {
      OrFlags(base, record, 0x8);
    }
  }
  guest::StoreU8(base, record + 15, guest::LoadU8(base, settings + 2));
  // The steering (+8): control 1, or the map's +0 when that reads 0.
  const bool wheel = guest::LoadU8(base, record + 17) == 1;
  const int32_t dead = wheel ? 1000 : 5000;
  const int32_t scale = wheel ? -130 : -110;
  Store(base, word, 0);
  control(1, 3, word);
  if (Load(base, word) != 0) {
    OrFlags(base, record, 0x02000000);
  } else {
    answer = control(Load(base, map + 0), 3, word);
    OrFlags(base, record, answer != 0 ? 0x02000000 : 0x01000000);
  }
  v = Mul(scale, int32_t(Load(base, word))) / 100;
  if (v > 32767) {
    guest::StoreU16(base, record + 8, 32767);
  } else if (v < -32767) {
    guest::StoreU16(base, record + 8, uint16_t(-32767));
  }
  bool zero;
  if (v >= 0) {
    v -= dead;
    zero = v < 0;
  } else {
    v += dead;
    zero = v > 0;
  }
  if (zero) {
    v = 0;
    Store(base, word, 0);
  } else {
    v = Mul(v, 32767) / (32767 - dead);
    Store(base, word, uint32_t(v));
    if (v > 32767) {
      v = 32767;
      Store(base, word, uint32_t(v));
    } else if (v < -32767) {
      v = -32767;
      Store(base, word, uint32_t(v));
    }
  }
  guest::StoreU16(base, record + 8, uint16_t(v));
  guest::StoreU8(base, record + 16, guest::LoadU8(base, settings + 0));
  // +10 (as the throttle).
  answer = control(Load(base, map + 12), 3, word);
  OrFlags(base, record, answer != 0 ? 0x80000000 : 0x40000000);
  v = Pedal(base, word);
  guest::StoreU16(base, record + 10, uint16_t(v));
  if (int16_t(v) != 0) {
    OrFlags(base, record, 0x8000);
  }
  // +12: x -110 / 100, within ±32,767, a dead zone of 7,500, x 32,767 /
  // 25,267, within ±32,767.
  answer = control(Load(base, map + 48), 3, word);
  OrFlags(base, record, answer != 0 ? 0x02000000 : 0x01000000);
  v = Mul(int32_t(Load(base, word)), -110) / 100;
  bool positive;
  if (v > 32767) {
    v = 32767;
    positive = true;
  } else {
    if (v < -32767) {
      v = -32767;
    }
    positive = v >= 0;
  }
  if (positive) {
    v -= 7500;
    zero = v < 0;
  } else {
    v += 7500;
    zero = v > 0;
  }
  if (zero) {
    v = 0;
    Store(base, word, 0);
  } else {
    v = Mul(v, 32767) / 25267;
    Store(base, word, uint32_t(v));
    if (v > 32767) {
      v = 32767;
      Store(base, word, uint32_t(v));
    } else if (v < -32767) {
      v = -32767;
      Store(base, word, uint32_t(v));
    }
  }
  guest::StoreU16(base, record + 12, uint16_t(v));
  // The buttons: (control, kind, flag); kind 1 pressed, 0 held.
  static constexpr uint32_t kButtons[8][3] = {
      {24, 1, 0x1},   {28, 1, 0x2},   {16, 0, 0x200},   {20, 1, 0x40},
      {32, 0, 0x80},  {36, 0, 0x100}, {40, 0, 0x20000}, {52, 1, 0x10000},
  };
  for (const auto& b : kButtons) {
    control(Load(base, map + b[0]), b[1], button);
    if (Load(base, button) != 0) {
      OrFlags(base, record, b[2]);
    }
  }
  uint32_t result = 0;
  control(Load(base, map + 56), 1, button);
  if (Load(base, button) != 0 && may_pause != 0) {
    PostPause(base, record, player, owner);
    result = 1;
  }
  // The racer's AI driver kept up.
  const uint64_t driver = ai_drivers::OfIndex(base, index);
  calls.update_idle(calls.user, driver, frame, 0x82160404, have_speed ? &speed : nullptr);
  return result;
}

uint32_t PlayerUpdate(uint8_t* base, uint32_t slot, uint32_t index, uint32_t player,
                      uint32_t owner, uint32_t may_pause, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821604C8 PlayerControl::Update");
  const uint32_t frame = sp - 192;
  const uint32_t record = frame + 80;
  Store(base, frame, sp);  // the frame's back chain, for ReadPad's calls
  const uint32_t vehicle = vehicle_accessors::VehicleGet(base, index);
  const uint32_t offset = slot * 3936;
  uint32_t race = Load(base, kRaceData);
  const uint32_t racer = race + offset;
  if ((Load(base, race + kFlags) & 0x100) != 0) {
    return 0;
  }
  if (Load(base, racer + 3940) == 0 && guest::LoadU8(base, vehicle + 2249) != 0) {
    guest::StoreU8(base, vehicle + 2249, 0);
    race = Load(base, kRaceData);
  }
  // Mode 1, without the race data's flag 0x80, for player 0: whether the
  // input record had 0x20000 before.
  const auto counting = [&](uint32_t at) {
    return int32_t(Load(base, at + kMode)) == 1 && (Load(base, at + kFlags) & 0x80) == 0 &&
           int32_t(player) == 0;
  };
  bool held_before = false;
  if (counting(race)) {
    Copy20(base, record, InputOf(base, index));
    held_before = (Load(base, record) & 0x20000) != 0;
  }
  uint32_t result = 0;
  if ((Load(base, racer + 3936) & 0x400) != 0) {
    ai_drivers::ClearCarInput(base, record);
  } else {
    result = ReadPad(base, record, index, player, owner, may_pause, frame, calls);
    race = Load(base, kRaceData);
    const uint32_t flags = Load(base, race + kFlags);
    if ((flags & 0x40) != 0) {
      // The replay: the next recorded record, with the pad's flags 0xC0
      // and halfword +12 kept.
      const uint16_t kept12 = guest::LoadU16(base, record + 12);
      const uint32_t kept_flags = Load(base, record) & 0xC0;
      if (RecordingNext(base, record, player) == 0) {
        const uint32_t at = Load(base, kRaceData) + kFlags;
        Store(base, at, Load(base, at) | 0x20000);
      }
      const uint32_t replayed = Load(base, record);
      guest::StoreU16(base, record + 12, kept12);
      Store(base, record, (replayed & ~0xC0u) | kept_flags);
      race = Load(base, kRaceData);
    } else if ((flags & 0x20) != 0) {
      RecordingAppend(base, record, player);
      race = Load(base, kRaceData);
    }
    if (counting(race) && held_before && (Load(base, record) & 0x20000) == 0) {
      Store(base, race + kCount, Load(base, race + kCount) + 1);
    }
  }
  Copy20(base, InputOf(base, index), record);
  return result;
}

void DriveByAiWithPad(uint8_t* base, uint32_t index, uint32_t hold, uint32_t player,
                      uint32_t sp, const Calls& calls, Regs* regs) {
  ITC_NATIVE_RUNNING("sub_8215F7E8 RacerControl::DriveByAiWithPad");
  const uint32_t frame = sp - 192;
  const uint32_t ai = frame + 128;
  Store(base, frame, sp);  // the frame's back chain, for the calls
  if ((Load(base, Load(base, kRaceData) + kFlags) & 0x100) != 0) {
    return;
  }
  calls.drive_car(calls.user, ai, index, hold, frame, 0x8215F820);
  // The record's copy (the original's at +96).
  uint8_t copy[20];
  for (uint32_t i = 0; i < 20; i++) {
    copy[i] = guest::LoadU8(base, ai + i);
  }
  const auto copy_word = [&]() {
    return uint32_t(copy[0]) << 24 | uint32_t(copy[1]) << 16 | uint32_t(copy[2]) << 8 | copy[3];
  };
  const auto set_half = [&](uint32_t at, uint32_t value) {
    copy[at] = uint8_t(value >> 8);
    copy[at + 1] = uint8_t(value);
  };
  const uint32_t map = ControlMap(base, player);
  Regs inner{0, 0, false};
  Control(base, player, Load(base, map + 20), 1, frame + 80, frame, calls, &inner);
  uint32_t flags = copy_word();
  if (Load(base, frame + 80) != 0) {
    flags |= 0x40;
  }
  const uint32_t answer = Control(base, player, Load(base, map + 48), 3, frame + 84, frame, calls,
                                  &inner);
  flags |= answer != 0 ? 0x02000000 : 0x01000000;
  copy[0] = uint8_t(flags >> 24);
  copy[1] = uint8_t(flags >> 16);
  copy[2] = uint8_t(flags >> 8);
  copy[3] = uint8_t(flags);
  int32_t v = Mul(int32_t(Load(base, frame + 84)), -110) / 100;
  bool positive;
  if (v > 32767) {
    v = 32767;
    positive = true;
  } else {
    if (v < -32767) {
      v = -32767;
    }
    positive = v >= 0;
  }
  bool zero;
  if (positive) {
    v -= 7500;
    zero = v < 0;
  } else {
    v += 7500;
    zero = v > 0;
  }
  int32_t scaled = 0;
  if (!zero) {
    scaled = Mul(v, 32767) / 25267;
    // The original's overflow: the steering +8 gets ±32,767 (it can't, from
    // a clamped value).
    if (scaled > 32767) {
      set_half(8, 32767);
    } else if (scaled < -32767) {
      set_half(8, uint32_t(-32767));
    }
  }
  set_half(12, uint32_t(scaled));
  const uint32_t to = InputOf(base, index);
  for (uint32_t i = 0; i < 20; i++) {
    guest::StoreU8(base, to + i, copy[i]);
  }
  regs->r3 = answer;
}

uint32_t AllByAi(uint8_t* base, uint32_t flow, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8215FA68 RaceFlow::AllByAi");
  const uint32_t frame = sp - 144;
  for (int32_t i = 0; i < int32_t(track_collision::RaceRacerCount(base)); i++) {
    const uint32_t index = RacerWord(base, uint32_t(i), 20);
    const uint32_t flags = Load(base, vehicle_accessors::VehicleGet(base, index) + 6148);
    if ((flags & 0x1) != 0 || (flags & 0x80) != 0) {
      const uint32_t player = Load(base, flow + ((flags & 0x1) != 0 ? 9612 : 9616));
      Regs regs{0, 0, false};
      DriveByAiWithPad(base, index, 1, player, frame, calls, &regs);
    } else {
      uint64_t r3 = 0;
      DriveByAi(base, index, 1, frame, calls, &r3);
    }
  }
  return 1;
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
