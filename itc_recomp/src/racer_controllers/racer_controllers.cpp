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
#include "memory/handles.h"
#include "memory/heap.h"
#include "memory/host_copy.h"
#include "schema/action_mappers.h"
#include "schema/ai_drivers.h"
#include "schema/car_physics.h"
#include "schema/game_modes.h"
#include "schema/minimap.h"
#include "schema/race_event.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/race_world.h"
#include "schema/racer_controllers.h"
#include "schema/vehicle_accessors.h"
#include "track_collision/track_collision.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <optional>

namespace racer_controllers {

using Globals = schema::RacerControllersGlobals;

namespace {

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}

void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// The current stage's word +0 (the stage the race data's list +804 names),
// as Stages::Copy's copy of it holds it.
uint32_t CurrentStageWord0(uint32_t race) {
  const schema::Stages stages(schema::RaceData(race).stages_address());
  return schema::RaceStage(stages.address + 80 * stages.current()).type();
}

}  // namespace

// --- The cars' input records ---

bool CreateInputs(uint32_t* last) {
  ITC_NATIVE_RUNNING("sub_82102258 CarInputs::Create");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  // operator new's error paths are the original's: every block made first,
  // then the records stored and cleared in the original's order. The
  // records are in host memory (E5 step 4).
  uint32_t blocks[7];
  for (int i = 0; i < 7; i++) {
    const auto block = std::optional<uint32_t>(schema::CarInput::New().address);
    if (!block) {
      for (int k = 0; k < i; k++) {
        memory::crt::OperatorDelete(blocks[k]);
      }
      return false;
    }
    blocks[i] = *block;
  }
  for (int i = 0; i < 7; i++) {
    Store(Globals::input_records_address() + 4 * i, blocks[i]);
    ai_drivers::ClearCarInput(blocks[i]);
  }
  *last = blocks[6];
  return true;
}

bool DestroyInputs(uint32_t* last) {
  ITC_NATIVE_RUNNING("sub_82102300 CarInputs::Destroy");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  for (int i = 0; i < 7; i++) {
    const uint32_t block = Load(Globals::input_records_address() + 4 * i);
    if (block != 0 && !memory::IsHandle(block) && !memory::IsHeapBlock(block)) {
      return false;
    }
  }
  for (int i = 0; i < 7; i++) {
    const uint32_t block = Load(Globals::input_records_address() + 4 * i);
    memory::crt::OperatorDelete(block);
    *last = block;
  }
  return true;
}

bool OnEnd(uint32_t* last) {
  ITC_NATIVE_RUNNING("sub_8215F5E8 RaceFlow::OnEnd");
  return DestroyInputs(last);
}

// --- The input recording ---

uint32_t RecordingAppend(uint32_t record, uint32_t player) {
  ITC_NATIVE_RUNNING("sub_821029A8 InputRecording::Append");
  const uint32_t offset = player * kRecordingBytes;
  const uint32_t recording = Globals::input_recordings() + offset;
  const uint32_t count = Load(recording + kRecordingCount);
  if (count >= kRecordingMax) {
    return 0;
  }
  const uint32_t to = recording + 20 * count;
  for (uint32_t i = 0; i < 20; i += 4) {
    Store(to + i, Load(record + i));
  }
  const uint32_t at = Globals::input_recordings() + offset + kRecordingCount;
  Store(at, Load(at) + 1);
  return 1;
}

uint32_t RecordingHasRoom(uint32_t player) {
  ITC_NATIVE_RUNNING("sub_82102968 InputRecording::HasRoom");
  const uint32_t recording = Globals::input_recordings() + player * kRecordingBytes;
  return Load(recording + kRecordingCount) < kRecordingMax ? 1 : 0;
}

// --- The race flow's handlers ---

bool OnSetUp(uint32_t flow, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8215F728 RaceFlow::OnSetUp");
  uint32_t last = 0;
  if (!CreateInputs(&last)) {
    return false;
  }
  const uint32_t race = Globals::race_data();
  const uint32_t stage_word = CurrentStageWord0(race);
  const uint32_t type23 =
      vehicle_accessors::StagesIsType2Or3(schema::RaceData(Globals::race_data()).stages_address(), 0xFFFFFFFFu);
  *r3 = type23;
  if (type23 != 0) {
    return true;
  }
  if (int32_t(schema::RaceData(Globals::race_data()).kind()) == 6 && int32_t(stage_word) != 1) {
    return true;
  }
  const schema::RaceFlow race_flow(flow);
  race_flow.set_flags(race_flow.flags() | 1);
  return true;
}

// --- The AI drivers' hand-off ---

namespace {

// The input record of racer `index` (the original's rlwinm: index & 0xFF).
uint32_t InputOf(uint32_t index) {
  return Load(Globals::input_records_address() + 4 * (index & 0xFF));
}

// A record copied (either may be a host record, E5 step 4: copied with
// each field swapped).
void Copy20(uint32_t to, uint32_t from) {
  if (memory::CopyWithHandles(to, from, 20)) {
    return;
  }
  for (uint32_t i = 0; i < 20; i += 4) {
    Store(to + i, Load(from + i));
  }
}

// The racers' manager's i-th racer entry (its +2308 list).
schema::MinimapRacerEntry RacerEntry(uint32_t i) {
  return schema::MinimapRacerEntry(Load(schema::RaceManager(Globals::race()).racers_address() + 4 * i));
}

}  // namespace

void DriveByAi(uint32_t index, uint32_t hold, uint32_t sp, const Calls& calls,
               uint64_t* r3) {
  ITC_NATIVE_RUNNING("sub_8215F968 RacerControl::DriveByAi");
  const uint32_t frame = sp - 160;
  const uint32_t record = frame + 112;
  Store(frame, sp);  // the frame's back chain, for the call
  if ((schema::RaceData(Globals::race_data()).flags() & 0x100) != 0) {
    return;
  }
  calls.drive_car(calls.user, record, index, hold, frame, 0x8215F9A8);
  Copy20(InputOf(index), record);
  // The copy loop's r3 (DriveCar's, the record, past its five words).
  *r3 = record + 20;
}

uint32_t HoldAll(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8215FA10 RaceFlow::HoldAll");
  const uint32_t frame = sp - 112;
  for (int32_t i = 0; i < int32_t(track_collision::RaceRacerCount()); i++) {
    uint64_t r3 = 0;
    DriveByAi(RacerEntry(uint32_t(i)).vehicle_index(), 1, frame, calls, &r3);
  }
  return 1;
}

uint32_t ClearAllRecords(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8215F5F0 RaceFlow::ClearAllRecords");
  const uint32_t record = sp - 144 + 80;
  ai_drivers::ClearCarInput(record);
  for (int32_t i = 0; i < int32_t(track_collision::RaceRacerCount()); i++) {
    Copy20(InputOf(RacerEntry(uint32_t(i)).vehicle_index()), record);
  }
  return 1;
}

uint32_t OnHold(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8215FC28 RaceFlow::OnHold");
  return HoldAll(sp, calls);
}

uint32_t OnState16(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8215FB90 RaceFlow::OnState16");
  const uint32_t frame = sp - 208;
  const uint32_t stage_word = CurrentStageWord0(Globals::race_data());
  if (vehicle_accessors::StagesIsType2Or3(schema::RaceData(Globals::race_data()).stages_address(), 0xFFFFFFFFu) ==
          0 &&
      (int32_t(schema::RaceData(Globals::race_data()).kind()) != 6 || int32_t(stage_word) == 1)) {
    return HoldAll(frame, calls);
  }
  return ClearAllRecords(frame);
}

// --- The player's controls ---

uint32_t MapperEntry(uint32_t player) {
  return Globals::mapper_entries_address() +
         (Load(Globals::player_entries_address() + (player << 2)) << 3);
}

namespace {

// Where Read and Control put the value: a guest word (PlayerInputValue, as
// the game's callers give it) or a C++ word of the port's own (the frame
// slots ReadPad and DriveByAiWithPad kept it in before E5 step 6).
struct Out {
  uint32_t address;
  uint32_t* host = nullptr;
  uint32_t value() const {
    return host != nullptr ? *host : schema::PlayerInputValue(address).value();
  }
  float value_float() const {
    return host != nullptr ? std::bit_cast<float>(*host)
                           : schema::PlayerInputValue(address).value_float();
  }
  void set_value(uint32_t value) const {
    if (host != nullptr) {
      *host = value;
    } else {
      schema::PlayerInputValue(address).set_value(value);
    }
  }
};

void ReadTo(uint32_t player, uint32_t id, uint32_t kind, const Out& out,
            uint32_t sp, const Calls& calls, Regs* regs);
uint32_t ControlTo(uint32_t player, uint32_t control, uint32_t kind,
                   const Out& out, uint32_t sp, const Calls& calls, Regs* regs);

}  // namespace

void Read(uint32_t player, uint32_t id, uint32_t kind, uint32_t out, uint32_t sp,
          const Calls& calls, Regs* regs) {
  ReadTo(player, id, kind, Out{out}, sp, calls, regs);
}

uint32_t Control(uint32_t player, uint32_t control, uint32_t kind, uint32_t out,
                 uint32_t sp, const Calls& calls, Regs* regs) {
  return ControlTo(player, control, kind, Out{out}, sp, calls, regs);
}

namespace {

void ReadTo(uint32_t player, uint32_t id, uint32_t kind, const Out& out,
            uint32_t sp, const Calls& calls, Regs* regs) {
  ITC_NATIVE_RUNNING("sub_82102350 PlayerInput::Read");
  const uint32_t entry = MapperEntry(player);
  if (kind >= 5) {
    return;
  }
  const bool unread = int32_t(schema::PlayerMapperEntry(entry).unread()) == 1;
  if (kind == 4) {
    double f1;
    if (unread) {
      f1 = double(Globals::zero());
    } else {
      const uint32_t mapper = schema::PlayerMapperEntry(entry).mapper<schema::View>().address;
      const uint32_t function = Load(Load(mapper) + 20);
      Store(sp - 96, sp);
      regs->r3 = calls.call(calls.user, function, mapper, id, true, sp - 96, 0x821023D0, &f1);
    }
    regs->f1 = f1;
    regs->f1_set = true;
    out.set_value(std::bit_cast<uint32_t>(float(f1)));
    return;
  }
  if (unread) {
    if (kind == 3) {
      regs->r3 = 0;
    }
    out.set_value(0);
    return;
  }
  static constexpr uint32_t kSlotOffsets[4] = {32, 40, 48, 56};
  const uint32_t lr = kind == 3 ? 0x82102400 : 0x82102468;
  const uint32_t mapper = schema::PlayerMapperEntry(entry).mapper<schema::View>().address;
  const uint32_t function = Load(Load(mapper) + kSlotOffsets[kind]);
  uint64_t r3;
  int slot = -1;
  if (calls.native_slots && function == 0x8210BE70) {
    slot = 8;
    r3 = action_mappers::Held(mapper, id);
  } else if (calls.native_slots && function == 0x8210BF40) {
    slot = 10;
    r3 = action_mappers::Pressed(mapper, id);
  } else if (calls.native_slots && function == 0x8210C010) {
    slot = 12;
    r3 = action_mappers::Fired(mapper, id);
  } else if (calls.native_slots && function == 0x8210BA28) {
    slot = 14;
    r3 = action_mappers::AnalogRaw(mapper, id);
  } else {
    Store(sp - 96, sp);
    r3 = calls.call(calls.user, function, mapper, id, true, sp - 96, lr, nullptr);
  }
  if (slot >= 0) {
    // The read slots' adapters' line for --log_pause_menu.
    debug::PauseMenuLog_NoteSlotRead(slot, mapper, id, lr,
                                     double(slot == 14 ? uint32_t(r3) & 0xFFFF : uint32_t(r3)));
  }
  regs->r3 = r3;
  out.set_value(kind == 3 ? uint32_t(r3) : uint32_t(r3) & 0xFF);
}

uint32_t ControlTo(uint32_t player, uint32_t control, uint32_t kind,
                   const Out& out, uint32_t sp, const Calls& calls, Regs* regs) {
  ITC_NATIVE_RUNNING("sub_82102488 PlayerInput::Control");
  const uint32_t frame = sp - 112;
  const auto read = [&](uint32_t id) { ReadTo(player, id, kind, out, frame, calls, regs); };
  const Out& value_out = out;
  // One action: returns `answer`.
  const auto single = [&](uint32_t id, uint32_t answer) {
    read(id);
    return answer;
  };
  // An axis of two (`first`, then `second` when the first reads 0).
  const auto axis = [&](uint32_t first, uint32_t second, uint32_t answer) {
    read(first);
    if (kind < 3) {
      if (value_out.value() != 0) {
        return answer;
      }
    } else if (kind == 3) {
      const uint32_t value = value_out.value();
      if (value != 0) {
        value_out.set_value(uint32_t(0) - value);
        return answer;
      }
    } else if (kind == 4) {
      const double value = double(value_out.value_float());
      if (!(value == double(Globals::zero()))) {
        const double negated = std::bit_cast<double>(std::bit_cast<uint64_t>(value) ^
                                                     0x8000000000000000ull);
        value_out.set_value(std::bit_cast<uint32_t>(float(negated)));
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

// The player's pad: the input manager's slot 9 (+36), then that object's
// slot 5 (+20) with &the player at frame + 80 (`frame` the original's).
uint64_t FindPad(uint32_t entry, uint32_t sp, uint32_t frame_bytes,
                 uint32_t lr9, uint32_t lr5, const Calls& calls) {
  const uint32_t frame = sp - frame_bytes;
  const uint32_t mapper = schema::PlayerMapperEntry(entry).mapper<schema::View>().address;
  const uint32_t manager = Globals::input_manager();
  Store(frame, sp);
  Store(frame + 80, schema::ActionMapper(mapper).player());
  const uint64_t object = calls.call(calls.user, Load(Load(manager) + 36), manager, 0,
                                     false, frame, lr9, nullptr);
  return calls.call(calls.user, Load(Load(uint32_t(object)) + 20), object,
                    frame + 80, true, frame, lr5, nullptr);
}

}  // namespace

uint32_t PadIsConnected(uint32_t entry, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8209F340 Pad::IsConnected");
  const uint64_t pad = FindPad(entry, sp, 96, 0x8209F370, 0x8209F384, calls);
  return Load(uint32_t(pad) + 4) & 1;
}

uint32_t PadIsWheel(uint32_t entry, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8209F3B8 Pad::IsWheel");
  const uint64_t pad = FindPad(entry, sp, 112, 0x8209F3F0, 0x8209F404, calls);
  return (guest::LoadU8(uint32_t(pad) + 193) & 2) == 2 ? 1 : 0;
}

// --- The pad reader and the human cars ---

namespace {

// The settings bytes (+0x1F880 + 3 x player) and control map (+0x1F888 +
// 120 x player) of the save data's player 0 (SaveData::Of(0)).
uint32_t Settings(uint32_t player) {
  return schema::SaveData(vehicle_accessors::SaveDataOf(0)).setting_0_address() + 3 * player;
}
uint32_t ControlMap(uint32_t player) {
  return vehicle_accessors::SaveDataOf(0) + 120 * player + 0x1F888;
}

int32_t Mul(int32_t a, int32_t b) {
  return int32_t(uint32_t(a) * uint32_t(b));
}

// A one-sided analog (the throttle, the brake, +10): x 110 / 100, at most
// 32,767, 5,000 taken off (0 below), x 32,767 / 27,767, at most 32,767;
// the word `word` (the control's) left as the original leaves it.
int32_t Pedal(uint32_t& word) {
  int32_t v = Mul(int32_t(word), 110) / 100;
  if (v > 32767) {
    v = 32767;
  }
  v -= 5000;
  if (v < 0) {
    v = 0;
    word = 0;
    return v;
  }
  v = Mul(v, 32767) / 27767;
  word = uint32_t(v);
  if (v > 32767) {
    v = 32767;
    word = uint32_t(v);
  }
  return v;
}

void OrFlags(uint32_t record, uint32_t bits) {
  const schema::CarInput input(record);
  input.set_flags(input.flags() | bits);
}

// The pause: flags 0x1000, message 30 for player 1, then 29.
void PostPause(uint32_t record, uint32_t player, uint32_t owner) {
  OrFlags(record, 0x1000);
  if (int32_t(player) == 1) {
    PostMessage(owner, 30);
  }
  PostMessage(owner, 29);
}

}  // namespace

uint32_t RecordingNext(uint32_t record, uint32_t player) {
  ITC_NATIVE_RUNNING("sub_82102A28 InputRecording::Next");
  const uint32_t offset = player * kRecordingBytes;
  const uint32_t recording = Globals::input_recordings() + offset;
  const uint32_t position = Load(recording + kRecordingCount);
  if (position >= kRecordingMax) {
    return 0;
  }
  Copy20(record, recording + 20 * position);
  const uint32_t at = Globals::input_recordings() + offset + kRecordingCount;
  Store(at, Load(at) + 1);
  return 1;
}

void PostMessage(uint32_t owner, uint32_t message) {
  const schema::RacerControlsDispatcher dispatcher(owner);
  const uint32_t count = dispatcher.ring_count() + 1;
  const uint32_t at = dispatcher.ring_start() + count - 1;
  dispatcher.set_ring_count(count);
  const schema::ModeMessage record(dispatcher.ring_address() + 12 * (at % 50));
  record.set_id(message);
  record.set_arg0(0);
  record.set_arg1(0);
}

uint32_t ReadPad(uint32_t record, uint32_t index, uint32_t player, uint32_t owner,
                 uint32_t may_pause, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8215FCA8 PlayerControl::ReadPad");
  const uint32_t frame = sp - 288;
  // The analogs' and the buttons' control words (the original's frame +80
  // and +84; C++ words since E5 step 6, Control writing them directly).
  uint32_t word_value = 0, button_value = 0;
  const Out word{0, &word_value};
  const Out button{0, &button_value};
  const uint32_t entry = MapperEntry(player);
  const uint32_t settings = Settings(player);
  uint32_t map = ControlMap(player);
  Store(frame, sp);  // the frame's back chain, for the calls
  const uint32_t vehicle = vehicle_accessors::VehicleGet(index);
  button_value = 0;
  word_value = 0;
  ai_drivers::ClearCarInput(record);
  const schema::CarInput input(record);
  Regs regs{0, 0, false};
  const auto control = [&](uint32_t which, uint32_t kind, const Out& out) {
    return ControlTo(player, which, kind, out, frame, calls, &regs);
  };
  const uint32_t race = Globals::race_data();
  const bool by_stage =
      int32_t(schema::RaceData(race).kind()) == 6 && int32_t(CurrentStageWord0(race)) != 1;
  if (!by_stage && (PadIsConnected(entry, frame, calls) == 0 ||
                    int32_t(Globals::pad_blocked0()) != 0 ||
                    int32_t(Globals::pad_blocked1()) != 0)) {
    if (may_pause != 0) {
      PostPause(record, player, owner);
    }
    return 1;
  }
  if (PadIsWheel(entry, frame, calls) != 0) {
    map += 60;
    input.set_kind(1);
  }
  // The throttle (+4).
  uint32_t answer = control(Load(map + 4), 3, word);
  OrFlags(record, answer != 0 ? 0x08000000 : 0x04000000);
  int32_t v = Pedal(word_value);
  input.set_throttle(uint16_t(v));
  if (int16_t(v) != 0) {
    OrFlags(record, 0x4);
  }
  input.set_throttle_rate(guest::LoadU8(settings + 1));
  // The brake (+6), then the reverse and the speed's checks.
  answer = control(Load(map + 8), 3, word);
  OrFlags(record, answer != 0 ? 0x20000000 : 0x10000000);
  const int16_t brake = int16_t(Pedal(word_value));
  input.set_brake(uint16_t(brake));
  bool have_speed = false;
  double speed = 0;
  const auto fast = [&]() {
    speed = double(Vehicle_GetSpeedKmh(vehicle));
    have_speed = true;
    return !(speed < double(Globals::one()));
  };
  const auto reverse = [&]() {
    const uint32_t flags = input.flags();
    input.set_brake(0);
    input.set_flags(flags | 0x200);
  };
  if (schema::Vehicle(vehicle).gear() == -1) {
    if (input.throttle() != 0) {
      if (fast()) {
        input.set_brake(32767);
      }
    } else if (brake != 0) {
      reverse();
    }
  } else if (brake != 0) {
    if (!fast() && input.throttle() == 0) {
      reverse();
    } else {
      OrFlags(record, 0x8);
    }
  }
  input.set_brake_rate(guest::LoadU8(settings + 2));
  // The steering (+8): control 1, or the map's +0 when that reads 0.
  const bool wheel = input.kind() == 1;
  const int32_t dead = wheel ? 1000 : 5000;
  const int32_t scale = wheel ? -130 : -110;
  word_value = 0;
  control(1, 3, word);
  if (word_value != 0) {
    OrFlags(record, 0x02000000);
  } else {
    answer = control(Load(map + 0), 3, word);
    OrFlags(record, answer != 0 ? 0x02000000 : 0x01000000);
  }
  v = Mul(scale, int32_t(word_value)) / 100;
  if (v > 32767) {
    input.set_steering(32767);
  } else if (v < -32767) {
    input.set_steering(uint16_t(-32767));
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
    word_value = 0;
  } else {
    v = Mul(v, 32767) / (32767 - dead);
    word_value = uint32_t(v);
    if (v > 32767) {
      v = 32767;
      word_value = uint32_t(v);
    } else if (v < -32767) {
      v = -32767;
      word_value = uint32_t(v);
    }
  }
  input.set_steering(uint16_t(v));
  input.set_steering_rate(guest::LoadU8(settings + 0));
  // +10 (as the throttle).
  answer = control(Load(map + 12), 3, word);
  OrFlags(record, answer != 0 ? 0x80000000 : 0x40000000);
  v = Pedal(word_value);
  input.set_handbrake(uint16_t(v));
  if (int16_t(v) != 0) {
    OrFlags(record, 0x8000);
  }
  // +12: x -110 / 100, within ±32,767, a dead zone of 7,500, x 32,767 /
  // 25,267, within ±32,767.
  answer = control(Load(map + 48), 3, word);
  OrFlags(record, answer != 0 ? 0x02000000 : 0x01000000);
  v = Mul(int32_t(word_value), -110) / 100;
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
    word_value = 0;
  } else {
    v = Mul(v, 32767) / 25267;
    word_value = uint32_t(v);
    if (v > 32767) {
      v = 32767;
      word_value = uint32_t(v);
    } else if (v < -32767) {
      v = -32767;
      word_value = uint32_t(v);
    }
  }
  input.set_camera(uint16_t(v));
  // The buttons: (control, kind, flag); kind 1 pressed, 0 held.
  static constexpr uint32_t kButtons[8][3] = {
      {24, 1, 0x1},   {28, 1, 0x2},   {16, 0, 0x200},   {20, 1, 0x40},
      {32, 0, 0x80},  {36, 0, 0x100}, {40, 0, 0x20000}, {52, 1, 0x10000},
  };
  for (const auto& b : kButtons) {
    control(Load(map + b[0]), b[1], button);
    if (button_value != 0) {
      OrFlags(record, b[2]);
    }
  }
  uint32_t result = 0;
  control(Load(map + 56), 1, button);
  if (button_value != 0 && may_pause != 0) {
    PostPause(record, player, owner);
    result = 1;
  }
  // The racer's AI driver kept up.
  const uint64_t driver = ai_drivers::OfIndex(index);
  calls.update_idle(calls.user, driver, frame, 0x82160404, have_speed ? &speed : nullptr);
  return result;
}

uint32_t PlayerUpdate(uint32_t slot, uint32_t index, uint32_t player,
                      uint32_t owner, uint32_t may_pause, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821604C8 PlayerControl::Update");
  const uint32_t frame = sp - 192;
  const uint32_t record = frame + 80;
  const schema::CarInput input(record);
  Store(frame, sp);  // the frame's back chain, for ReadPad's calls
  const uint32_t vehicle = vehicle_accessors::VehicleGet(index);
  const uint32_t offset = slot * 3936;
  uint32_t race = Globals::race_data();
  const schema::Car racer(schema::RaceData(race).racer_records_address() + offset);
  if ((schema::RaceData(race).flags() & 0x100) != 0) {
    return 0;
  }
  if (racer.unknown_3892() == 0 && schema::Vehicle(vehicle).automatic() != 0) {
    schema::Vehicle(vehicle).set_automatic(0);
    race = Globals::race_data();
  }
  // Mode 1, without the race data's flag 0x80, for player 0: whether the
  // input record had 0x20000 before.
  const auto counting = [&](uint32_t at) {
    return int32_t(schema::RaceData(at).kind()) == 1 && (schema::RaceData(at).flags() & 0x80) == 0 &&
           int32_t(player) == 0;
  };
  bool held_before = false;
  if (counting(race)) {
    Copy20(record, InputOf(index));
    held_before = (input.flags() & 0x20000) != 0;
  }
  uint32_t result = 0;
  if ((racer.flags() & 0x400) != 0) {
    ai_drivers::ClearCarInput(record);
  } else {
    result = ReadPad(record, index, player, owner, may_pause, frame, calls);
    race = Globals::race_data();
    const uint32_t flags = schema::RaceData(race).flags();
    if ((flags & 0x40) != 0) {
      // The replay: the next recorded record, with the pad's flags 0xC0
      // and halfword +12 kept.
      const uint16_t kept12 = input.camera();
      const uint32_t kept_flags = input.flags() & 0xC0;
      if (RecordingNext(record, player) == 0) {
        const schema::RaceData data(Globals::race_data());
        data.set_flags(data.flags() | 0x20000);
      }
      const uint32_t replayed = input.flags();
      input.set_camera(kept12);
      input.set_flags((replayed & ~0xC0u) | kept_flags);
      race = Globals::race_data();
    } else if ((flags & 0x20) != 0) {
      RecordingAppend(record, player);
      race = Globals::race_data();
    }
    if (counting(race) && held_before && (input.flags() & 0x20000) == 0) {
      const schema::RaceData data(race);
      data.set_finish_count(data.finish_count() + 1);
    }
  }
  Copy20(InputOf(index), record);
  return result;
}

void DriveByAiWithPad(uint32_t index, uint32_t hold, uint32_t player,
                      uint32_t sp, const Calls& calls, Regs* regs) {
  ITC_NATIVE_RUNNING("sub_8215F7E8 RacerControl::DriveByAiWithPad");
  const uint32_t frame = sp - 192;
  const uint32_t ai = frame + 128;
  Store(frame, sp);  // the frame's back chain, for the calls
  if ((schema::RaceData(Globals::race_data()).flags() & 0x100) != 0) {
    return;
  }
  calls.drive_car(calls.user, ai, index, hold, frame, 0x8215F820);
  // The record's copy (the original's at +96).
  uint8_t copy[20];
  for (uint32_t i = 0; i < 20; i++) {
    copy[i] = guest::LoadU8(ai + i);
  }
  const auto copy_word = [&]() {
    return uint32_t(copy[0]) << 24 | uint32_t(copy[1]) << 16 | uint32_t(copy[2]) << 8 | copy[3];
  };
  const auto set_half = [&](uint32_t at, uint32_t value) {
    copy[at] = uint8_t(value >> 8);
    copy[at + 1] = uint8_t(value);
  };
  const uint32_t map = ControlMap(player);
  Regs inner{0, 0, false};
  // The two controls' words (the original's frame +80 and +84; C++ words
  // since E5 step 6).
  uint32_t pressed = 0, camera = 0;
  ControlTo(player, Load(map + 20), 1, Out{0, &pressed}, frame, calls, &inner);
  uint32_t flags = copy_word();
  if (pressed != 0) {
    flags |= 0x40;
  }
  const uint32_t answer = ControlTo(player, Load(map + 48), 3,
                                    Out{0, &camera}, frame, calls, &inner);
  flags |= answer != 0 ? 0x02000000 : 0x01000000;
  copy[0] = uint8_t(flags >> 24);
  copy[1] = uint8_t(flags >> 16);
  copy[2] = uint8_t(flags >> 8);
  copy[3] = uint8_t(flags);
  int32_t v = Mul(int32_t(camera), -110) / 100;
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
  const uint32_t to = InputOf(index);
  if (memory::IsHandle(to)) {
    // A host record (E5 step 4): the copy put where the original keeps it
    // (its frame's +96), then copied in with each field swapped.
    for (uint32_t i = 0; i < 20; i++) {
      guest::StoreU8(frame + 96 + i, copy[i]);
    }
    memory::CopyWithHandles(to, frame + 96, 20);
  } else {
    for (uint32_t i = 0; i < 20; i++) {
      guest::StoreU8(to + i, copy[i]);
    }
  }
  regs->r3 = answer;
}

uint32_t AllByAi(uint32_t flow, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8215FA68 RaceFlow::AllByAi");
  const uint32_t frame = sp - 144;
  for (int32_t i = 0; i < int32_t(track_collision::RaceRacerCount()); i++) {
    const uint32_t index = RacerEntry(uint32_t(i)).vehicle_index();
    const uint32_t flags = schema::Vehicle(vehicle_accessors::VehicleGet(index)).car_flags();
    if ((flags & 0x1) != 0 || (flags & 0x80) != 0) {
      const schema::RaceFlow race_flow(flow);
      const uint32_t player = (flags & 0x1) != 0 ? race_flow.player_a() : race_flow.player_b();
      Regs regs{0, 0, false};
      DriveByAiWithPad(index, 1, player, frame, calls, &regs);
    } else {
      uint64_t r3 = 0;
      DriveByAi(index, 1, frame, calls, &r3);
    }
  }
  return 1;
}

// --- The race flow's per-racer states ---

namespace {

// The per-racer loop of sub_821606D0, sub_821607A0 and sub_82160880: the
// human cars (their vehicle's flags 0x1 and 0x80) by PlayerUpdate with
// `pause_first` and `pause_second` deciding whether each may pause (the
// second given the first's answer), the others by DriveByAi with `hold`
// (given the racer's word +0).
template <typename PauseFirst, typename PauseSecond, typename Hold>
uint32_t ByRacer(uint32_t flow, uint32_t owner, uint32_t sp, const Calls& calls,
                 PauseFirst pause_first, PauseSecond pause_second, Hold hold) {
  const uint32_t frame = sp - 144;
  uint32_t first = 0;
  for (int32_t i = 0; i < int32_t(track_collision::RaceRacerCount()); i++) {
    const uint32_t index = RacerEntry(uint32_t(i)).vehicle_index();
    const uint32_t flags = schema::Vehicle(vehicle_accessors::VehicleGet(index)).car_flags();
    const schema::MinimapRacerEntry racer = RacerEntry(uint32_t(i));
    const uint32_t slot = racer.record();
    if ((flags & 0x1) != 0) {
      first = PlayerUpdate(slot, index, schema::RaceFlow(flow).player_a(), owner,
                           pause_first(), frame, calls);
    } else if ((flags & 0x80) != 0) {
      PlayerUpdate(slot, index, schema::RaceFlow(flow).player_b(), owner,
                   pause_second(first), frame, calls);
    } else {
      uint64_t r3 = 0;
      DriveByAi(index, hold(racer.flags()), frame, calls, &r3);
    }
  }
  return 1;
}

}  // namespace

uint32_t PlayersAndHeldAi(uint32_t flow, uint32_t owner, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821606D0 RaceFlow::PlayersAndHeldAi");
  return ByRacer(flow, owner, sp, calls, [] { return 1u; },
      [](uint32_t first) { return first == 0 ? 1u : 0u; }, [](uint32_t) { return 1u; });
}

uint32_t OnState7(uint32_t flow, uint32_t owner, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82160940 RaceFlow::OnState7");
  return PlayersAndHeldAi(flow, owner, sp, calls);
}

uint32_t PlayersAndAi(uint32_t flow, uint32_t owner, uint32_t sp,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821607A0 RaceFlow::PlayersAndAi");
  return ByRacer(flow, owner, sp, calls, [] { return 1u; },
      [](uint32_t first) { return first == 0 ? 1u : 0u; },
      [](uint32_t word) { return (word & 0x10) != 0 && (word & 0x4000) == 0 ? 0u : 1u; });
}

uint32_t PlayersAndAiNoPause(uint32_t flow, uint32_t owner, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82160880 RaceFlow::PlayersAndAiNoPause");
  return ByRacer(flow, owner, sp, calls, [] { return 0u; }, [](uint32_t) { return 0u; },
      [](uint32_t word) { return (word & 0x4000) != 0 ? 1u : 0u; });
}

uint32_t OnState22(uint32_t flow, uint32_t owner, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82160948 RaceFlow::OnState22");
  const uint32_t frame = sp - 208;
  const uint32_t stage_word = CurrentStageWord0(Globals::race_data());
  if (vehicle_accessors::StagesIsType2Or3(schema::RaceData(Globals::race_data()).stages_address(), 0xFFFFFFFFu) ==
          0 &&
      (int32_t(schema::RaceData(Globals::race_data()).kind()) != 6 || int32_t(stage_word) == 1)) {
    return AllByAi(flow, frame, calls);
  }
  return PlayersAndAiNoPause(flow, owner, frame, calls);
}

uint32_t OnState23(uint32_t flow, uint32_t owner, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821609E0 RaceFlow::OnState23");
  const uint32_t frame = sp - 192;
  const uint32_t stage_word = CurrentStageWord0(Globals::race_data());
  uint32_t result;
  if (int32_t(schema::RaceData(Globals::race_data()).kind()) == 6 && int32_t(stage_word) != 1) {
    result = PlayersAndAiNoPause(flow, owner, frame, calls);
  } else {
    result = PlayersAndAi(flow, owner, frame, calls);
  }
  const schema::RaceData data(Globals::race_data());
  if ((data.flags() & 0x20000) != 0) {
    data.set_flags(data.flags() & ~0x20000u);
    if ((schema::RaceData(Globals::race_data()).flags() & 0x40) != 0) {
      PostMessage(owner, 34);
    }
  }
  return result;
}

uint32_t OnState34(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8215FC38 RaceFlow::OnState34");
  const uint32_t frame = sp - 128;
  uint32_t count = track_collision::RaceRacerCount();
  for (int32_t i = 0; i < int32_t(count); count = track_collision::RaceRacerCount()) {
    const uint32_t index = RacerEntry(uint32_t(i)).vehicle_index();
    const uint32_t flags = schema::Vehicle(vehicle_accessors::VehicleGet(index)).car_flags();
    uint64_t r3 = 0;
    DriveByAi(index, (flags & 0x1) != 0 ? 0 : 1, frame, calls, &r3);
    i++;
  }
  return count;
}

// --- The AI drivers' set-up ---

uint64_t Setup(uint32_t index, uint32_t id, uint32_t hold, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82170FF8 AiDriver::Setup");
  const uint32_t frame = sp - 128;
  const uint64_t driver64 = ai_drivers::OfIndex(Load(index));
  const uint32_t driver = uint32_t(driver64);
  ai_drivers::ResetDriver(driver);
  const uint32_t item_id = Load(id);
  const uint32_t item = int32_t(item_id) < 0 ? 0 : data_tables::Item(item_id);
  ai_drivers::SetParams(driver, item);
  if (schema::AiDriverSetupHold(hold).hold() != 0) {
    ai_drivers::ResetControl(driver, frame - 112);
    return driver64 + 6048;
  }
  return ai_drivers::StartRace(driver, frame - 112);
}

}  // namespace racer_controllers
