// The type 8 mode's slots and +32 interface (docs/functions/type8-mode.md):
// ModeType8::Release, WaitForLoads, AllLoaded, NextMode, PollTeardown,
// TearDown, Interrupt and ForceExit, and SaveData::FreeTwoPlayerBlock. See
// type8_mode.h. As the type 2 mode's (src/type2_mode/delegates.cpp): the
// messages queued in the mode's ring (+240) inline with no overflow check,
// the delegate's member functor rebuilt from its two words in the
// original's frame (read back as the original's ld does, so the frame bytes
// it never writes go where it puts them). AllLoaded, StandScene::Release and
// PollTeardown, SaveData::TwoPlayerBlock, memcpy, Sound::Play and
// GarageStages::DrawAll through their plain C++; GameMessages::Dispatch,
// LoadingScreen::Run and Finish, GarageStage::Command and SetDrawWord,
// Strings::Release, UiMovies::DeleteAll's jump, operator delete and the sound
// manager's slots as guest calls to their overrides.

#include "type8_mode/type8_mode.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "car_list/car_list.h"
#include "car_makers/car_makers.h"
#include "game_modes/game_modes.h"
#include "options_screen/options_screen.h"
#include "save_screen/save_screen.h"
#include "stand_models/stand_models.h"
#include "vehicle_accessors/vehicle_accessors.h"
#include "guest.h"
#include "menu_sounds/menu_sounds.h"
#include "mode_tick/mode_tick.h"
#include "save_data/save_data.h"
#include "schema/boot_mode.h"
#include "schema/car_makers.h"
#include "schema/game_modes.h"
#include "schema/stand_models.h"
#include "schema/tuning_screen.h"
#include "schema/type6_type7_modes.h"
#include "schema/type8_mode.h"
#include "schema/type8_screens.h"
#include "type8_screens/type8_screens.h"

namespace type8_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using Machine = schema::StateMachine;
using G = schema::Type8ModeGlobals;
using Mode = schema::ModeType8;
using Member = schema::ModeType8Member;
using Answer = schema::ModeAnswer;
using Slot = schema::StandSlot;
constexpr uint32_t kDelegateVtable = 0x82010FA0;
constexpr uint32_t kNextModeThunk = 0x82235C08, kSlot9Thunk = 0x8223D4C0,
                   kSlot10Thunk = 0x8223D4A0;
constexpr uint32_t kLeavingFlag = 0x826364EC;
constexpr uint32_t kPadBlocked0 = 0x826C7960, kPadBlocked1 = 0x826C7964;
constexpr uint32_t kSaveDataSlots = 0x826C5B58, kStringTable = 0x826C5C34;
constexpr uint32_t kDispatch = 0x821062E8, kLoadingRun = 0x82247898, kLoadingFinish = 0x82247A60;
constexpr uint32_t kCommand = 0x82180108, kSetDrawWord = 0x82178B40;
constexpr uint32_t kStandSceneRelease = 0x82180738, kStringsRelease = 0x8210A538;
constexpr uint32_t kDeleteAllJump = 0x8211E370, kOperatorDelete = 0x825A70E8,
                   kMemcpy = 0x825A7410;
constexpr uint64_t kAll = ~uint64_t(0);

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

void ClearAnswer(uint8_t* base, uint32_t answer) {
  const Answer a(base, answer);
  a.set_next_type(0);
  a.set_changed(0);
}

// The mode's dispatcher (+36: its handlers, the ring after them).
schema::ModeDispatcher DispatcherOf(uint8_t* base, uint32_t mode) {
  return schema::ModeDispatcher(base, Mode(base, mode).handlers_address());
}

// A message (`message`, 0, 0) queued in the dispatcher's ring, the count
// raised first.
void Queue(uint8_t* base, const schema::ModeDispatcher& d, uint32_t message) {
  const uint32_t count = d.ring_count() + 1;
  const uint32_t end = d.ring_start() + count - 1;
  d.set_ring_count(count);
  const game_modes::RingSlot slot{d.ring() + (end - (end / 50) * 50) * schema::ModeMessage::kSize};
  slot.set_id(message);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

// The delegate (mode +4 = mode +8 when that isn't 0) from the member
// functor's two words at `words`: its +4 the mode, +8 the doubleword, +0 the
// vtable.
void SetDelegate(uint8_t* base, uint32_t mode, uint32_t words) {
  const Mode m(base, mode);
  const uint32_t holder = m.delegate_ptr_address();
  uint32_t delegate = holder + 4;  // the mode's inline delegate (+8)
  if (delegate != 0) {
    const uint32_t function = LoadU32(base, words), adjustment = LoadU32(base, words + 4);
    const schema::ModeDelegate d(base, delegate);
    d.set_object(mode);
    d.set_function(function);
    d.set_adjustment(adjustment);
    d.set_vtable(kDelegateVtable);
  } else {
    delegate = 0;
  }
  m.set_delegate_ptr(delegate);
}

// The 8-byte answer at the frame's +80 (byte 1 over the word there, the
// three bytes after it as the frame holds them, then `next`) stored to the
// mode's +87760; the frame's +84 then 0.
void StoreAnswer(uint8_t* base, uint32_t frame, const Answer& to, uint32_t next, bool next_first) {
  if (next_first) {
    StoreU32(base, frame + 84, next);
    guest::StoreU8(base, frame + 80, 1);
  } else {
    guest::StoreU8(base, frame + 80, 1);
    StoreU32(base, frame + 84, next);
  }
  const uint32_t w0 = LoadU32(base, frame + 80), w1 = LoadU32(base, frame + 84);
  StoreU32(base, frame + 84, 0);
  to.set_word0(w0);
  to.set_next_type(w1);
}

uint64_t SoundThunk(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                    uint32_t lr) {
  const uint64_t six[6] = {args[0], args[1], args[2], args[3], 0, 0};
  return static_cast<const ScreenCalls*>(user)->call_set(function, six, set, r1, lr);
}

// The sound manager's slot at `offset` with r4, r5 = the frame's words at
// `a`, `b` (set first to `va`, `vb` in the original's order), from `frame`.
uint64_t ManagerSlot(uint8_t* base, const ScreenCalls& calls, uint32_t offset, uint32_t frame,
                     uint32_t a, uint32_t b, uint32_t lr) {
  const uint32_t manager = LoadU32(base, menu_sounds::kSoundManager);
  const uint64_t args[6] = {manager, frame + a, frame + b, 0, 0, 0};
  return calls.call_set(LoadU32(base, LoadU32(base, manager) + offset), args, 3, frame, lr);
}

}  // namespace

uint32_t ModeAllLoaded(uint8_t* base, uint32_t member) {
  ITC_NATIVE_RUNNING("sub_8223D3C0 ModeType8::AllLoaded");
  // The machines' loaded words: the screens' machines at their +4 (the
  // list screens' machines have no field of their own yet), the dialog's,
  // options' and save screen's at their +0.
  const Member m(base, member);
  for (const uint32_t machine :
       {schema::Type8Screen0(base, m.screen0_address()).machine_address(),
        m.list_screen_address() + 4, m.color_list_screen_address() + 4,
        schema::Type8Dialog(base, m.dialog_address()).machine_address(),
        schema::Type8PanelScreen(base, m.panel_screen_address()).machine_address(),
        m.options_address(), m.save_screen_address()}) {
    if (Machine(base, machine).loaded() == 0) {
      return 0;
    }
  }
  return 1;
}

uint64_t FreeTwoPlayerBlock(uint8_t* base, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82105638 SaveData::FreeTwoPlayerBlock");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t block = LoadU32(base, save_data::kTwoPlayerBlock);
  if (block == 0) {
    return 0;
  }
  const uint64_t r3 = calls.call(kOperatorDelete, frame, 0x8210565C, block, 0);
  StoreU32(base, save_data::kTwoPlayerBlock, 0);
  return r3;
}

uint64_t ModeRelease(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                     const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223D3B8 ModeType8::Release");
  if (const std::optional<uint64_t> done = type67_modes::StandSceneRelease(base)) {
    return *done;
  }
  return calls.call(kStandSceneRelease, sp, lr, r3, r4);
}

uint64_t ModeWaitForLoads(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                          const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223DBC0 ModeType8::WaitForLoads");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t answer = uint32_t(r3);
  ClearAnswer(base, answer);
  calls.call(kLoadingRun, frame, 0x8223DBE8, kAll, mode);
  if (ModeAllLoaded(base, Mode(base, mode).member_address()) != 0 &&
      (LoadU32(base, mode_tick::kFadeOutCounts) | LoadU32(base, mode_tick::kFadeInCounts)) ==
          0) {
    StoreU32(base, frame + 80, 1);
    const uint32_t manager = LoadU32(base, menu_sounds::kSoundManager);
    const uint64_t ask[6] = {manager, frame + 80, 0, 0, 0, 0};
    const uint64_t loading =
        calls.call_set(LoadU32(base, LoadU32(base, manager) + 24), ask, 2, frame, 0x8223DC38);
    if ((loading & 0xFF) == 0) {
      const schema::StandHolder holder = G::stand_holder(base);
      const uint32_t first = holder.stage_block_1();
      if (Slot(base, first).stage().camera_set_up() != 0 &&
          Slot(base, holder.stage_block_2()).stage().camera_set_up() != 0) {
        calls.call(kCommand, frame, 0x8223DC78, first, 56);
        calls.call(kCommand, frame, 0x8223DC88, G::stand_holder(base).stage_block_2(), 57);
        calls.call(kLoadingFinish, frame, 0x8223DC90, kAll, 57);
        StoreU32(base, frame + 88, 1);
        StoreU32(base, frame + 84, 2);
        menu_sounds::PlaySound(base, frame + 88, frame + 84, frame,
                               {const_cast<ScreenCalls*>(&calls), &SoundThunk});
        // The loaded message (2) to the panel screen, the delegate on to
        // NextModeThunk.
        StoreU32(base, frame + 92, 0);
        Queue(base, DispatcherOf(base, mode), 2);
        StoreU32(base, frame + 88, kNextModeThunk);
        SetDelegate(base, mode, frame + 88);
      }
    }
  }
  Queue(base, DispatcherOf(base, mode), 1);
  calls.call(kDispatch, frame, 0x8223DD80, DispatcherOf(base, mode).address, mode);
  return r3;
}

uint64_t ModeNextMode(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                      const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223DD90 ModeType8::NextMode");
  const uint32_t frame = Frame(base, sp, 144);
  ClearAnswer(base, uint32_t(r3));
  if (LoadU32(base, kPadBlocked0) == 0 && LoadU32(base, kPadBlocked1) == 0) {
    Queue(base, DispatcherOf(base, mode), 1);
  }
  Queue(base, DispatcherOf(base, mode), 0);
  calls.call(kDispatch, frame, 0x8223DE70, DispatcherOf(base, mode).address, mode);
  const uint32_t member = Mode(base, mode).member_address();
  const Member m(base, member);
  // A machine's state: the cast's null check on the list screen at
  // `screen`, then its machine at +4 (0 for a screen at address 0).
  const auto state = [&](uint32_t screen) {
    const uint32_t machine = screen != 0 ? screen + 4 : 0;
    return int32_t(Machine(base, machine).state());
  };
  const Answer to(base, m.answer_changed_address());
  if (state(m.color_list_screen_address()) == 11 || state(m.list_screen_address()) == 11) {
    StoreAnswer(base, frame, to, 1, true);  // the race
  } else {
    const uint32_t screen = member != 0 ? member + 4 : 0;
    if (int32_t(Machine(base, screen).state()) != 6) {
      return r3;
    }
    // Player 0's options back from the two-player block, the block freed.
    const uint32_t block = save_data::TwoPlayerBlock(base);
    const uint32_t source = block + 0x3AA58;
    const uint32_t destination = LoadU32(base, kSaveDataSlots) + 0x1F85C;
    if (!c_runtime::Memcpy(base, destination, source, 284)) {
      const uint64_t args[6] = {destination, source, 284, 0, 0, 0};
      calls.call_set(kMemcpy, args, 3, frame, 0x8223DEF4);
    }
    FreeTwoPlayerBlock(base, frame, calls);
    StoreU32(base, kLeavingFlag, 0);
    StoreAnswer(base, frame, to, 4, false);  // the type 4 front end
  }
  StoreU32(base, frame + 80, kSlot9Thunk);
  SetDelegate(base, mode, frame + 80);
  return r3;
}

uint64_t ModePollTeardown(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                          const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223D8E8 ModeType8::PollTeardown");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, uint32_t(r3));
  if (int32_t(type67_modes::StandScenePollTeardown(base, frame, calls)) == 0) {
    return r3;
  }
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 80, kSlot10Thunk);
  SetDelegate(base, mode, frame + 80);
  return r3;
}

uint64_t ModeTearDown(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                      const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223D4D0 ModeType8::TearDown");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t answer = uint32_t(r3);
  // Bank 1's cues 0 and 2: stopped (slot 13) where playing (slot 14).
  StoreU32(base, frame + 80, 0);
  StoreU32(base, frame + 84, 1);
  ClearAnswer(base, answer);
  if ((ManagerSlot(base, calls, 56, frame, 84, 80, 0x8223D51C) & 0xFF) != 0) {
    StoreU32(base, frame + 88, 0);
    StoreU32(base, frame + 92, 1);
    ManagerSlot(base, calls, 52, frame, 92, 88, 0x8223D548);
  }
  StoreU32(base, frame + 100, 1);
  StoreU32(base, frame + 96, 2);
  if ((ManagerSlot(base, calls, 56, frame, 100, 96, 0x8223D570) & 0xFF) != 0) {
    StoreU32(base, frame + 104, 2);
    StoreU32(base, frame + 108, 1);
    ManagerSlot(base, calls, 52, frame, 108, 104, 0x8223D59C);
  }
  const uint64_t released =
      calls.call(kStringsRelease, frame, 0x8223D5AC, LoadU32(base, kStringTable), 3);
  calls.call(kDeleteAllJump, frame, 0x8223D5B0, released, 3);
  const Answer stored(base, Member(base, Mode(base, mode).member_address()).answer_changed_address());
  const uint32_t w0 = stored.word0(), w1 = stored.next_type();
  const Answer out(base, answer);
  out.set_word0(w0);
  out.set_next_type(w1);
  return r3;
}

uint64_t ModeInterrupt(uint8_t* base, uint32_t object, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223D960 ModeType8::Interrupt");
  (void)sp;  // its frame held only the back chain (E5 step 6)
  // `object` is the mode's +32 interface.
  const uint32_t mode = object - 32;
  const Member m(base, Mode(base, mode).member_address());
  if (ModeAllLoaded(base, m.address) == 0) {
    return 0;
  }
  if (int32_t(schema::ScreenMachine(base, m.save_screen_address()).state()) != 5) {
    return 1;
  }
  Queue(base, DispatcherOf(base, mode), 30);  // the save screen's cancel
  return 0;
}

uint64_t ModeForceExit(uint8_t* base, uint64_t r3, uint32_t number, uint32_t sp,
                       const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223DA00 ModeType8::ForceExit");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t object = uint32_t(r3);
  Slot(base, G::stand_holder(base).stage_block_1()).stage().set_reflection(0);
  Slot(base, G::stand_holder(base).stage_block_2()).stage().set_reflection(0);
  calls.call(kSetDrawWord, frame, 0x8223DA4C, G::stand_holder(base).stage_block_1(), 0);
  calls.call(kSetDrawWord, frame, 0x8223DA5C, G::stand_holder(base).stage_block_2(), 0);
  const uint64_t drawn = car_list::DrawAll(base, frame, type8_screens::CarListCalls(calls));
  if (number >= 4) {
    return drawn;
  }
  // Leave (4) queued, the delegate on to Slot9Thunk (PollTeardown), the
  // answer {1, the thunk's low three bytes, 4 or 12}.
  const uint32_t mode = object - 32;
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 80, kSlot9Thunk);
  Queue(base, DispatcherOf(base, mode), 4);
  if (number < 2) {
    StoreU32(base, kLeavingFlag, 1);
  }
  SetDelegate(base, mode, frame + 80);
  StoreU32(base, frame + 84, number < 2 ? 4 : 12);
  guest::StoreU8(base, frame + 80, 1);
  const uint32_t w0 = LoadU32(base, frame + 80), w1 = LoadU32(base, frame + 84);
  const Answer to(base, Member(base, Mode(base, mode).member_address()).answer_changed_address());
  to.set_word0(w0);
  to.set_next_type(w1);
  return r3 - 28;
}

uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223D5D8 ModeType8::Init");
  constexpr uint32_t kScreenStart = 0x8211AD00, kStringsLoad = 0x8210A8D0,
                     kStandSceneCreate = 0x82180B00, kCreateTwoPlayerBlock = 0x82105740,
                     kLoadingShow = 0x822478D0, kSlot8Thunk = 0x8223D4B0;
  constexpr uint32_t kGarageCars = G::garage_cars_address();
  const uint32_t frame = Frame(base, sp, kModeInitFrame);
  auto call = [&](uint32_t target, uint32_t lr, std::initializer_list<uint64_t> args) {
    uint64_t regs[6] = {};
    int set = 0;
    for (const uint64_t a : args) regs[set++] = a;
    return calls.call_set(target, regs, set, frame, lr);
  };
  call(kScreenStart, 0x8223D5EC, {mode});
  call(kStringsLoad, 0x8223D5FC, {LoadU32(base, kStringTable), 3});
  call(kStandSceneCreate, 0x8223D604, {3});
  // GarageStage::MakeModel by its plain C++ (no adapter: B7 loop,
  // 2026-11-13), ScenePools::SetUpB under it through call_set.
  const stand_models::GuestCall guest_call = [&](uint32_t function,
                                                 std::initializer_list<uint64_t> args,
                                                 uint32_t r1, uint32_t lr) {
    uint64_t regs[6] = {};
    int set = 0;
    for (const uint64_t v : args) regs[set++] = v;
    return calls.call_set(function, regs, set, r1, lr);
  };
  for (uint32_t p = 0; p < 2; p++) {
    const schema::GarageCarSlot garage(base, kGarageCars + schema::GarageCarSlot::kSize * p);
    const uint32_t block =
        car_makers::BlockOf(schema::CarMakersCar(base, garage.car_address()).car_id());
    const uint32_t holder =
        LoadU32(base, G::stand_holder(base).stage_block_1_address() + 4 * p);
    stand_models::MakeModel(base, guest_call, holder, block, 0, frame);
  }
  if (save_data::TwoPlayerBlock(base) == 0) {
    if (!calls.native_heap || !save_data::CreateTwoPlayerBlock(base)) {
      call(kCreateTwoPlayerBlock, 0x8223D658, {});
    }
    const uint32_t block = save_data::TwoPlayerBlock(base);
    const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
    const uint32_t destination = block + 0x3AA58, source = save + 0x1F85C;
    if (!c_runtime::Memcpy(base, destination, source, 284)) {
      call(kMemcpy, 0x8223D684, {destination, source, 284});
    }
  }
  // The members' Inits (their plain C++; none has an adapter), on the
  // member's addresses as the original computes them.
  const uint32_t member = Mode(base, mode).member_address();
  const Member m(base, member);
  const uint32_t dialog = m.dialog_address(), panel = m.panel_screen_address(),
                 colors = m.color_list_screen_address(), list = m.list_screen_address(),
                 options = m.options_address(), controls = m.controller_pages_address(),
                 saves = m.save_screen_address();
  const auto run67 = [&](uint32_t address, uint32_t object) {
    for (const type67_modes::InitInfo& info : type67_modes::kInits) {
      if (info.address == address) {
        StoreU32(base, frame - info.frame, frame);
        type67_modes::RunInit(base, address, object, frame - info.frame);
      }
    }
  };
  run67(0x8223C950, member);
  run67(0x8223AE60, dialog);
  type67_modes::PanelScreenInit(base, panel, frame, calls);
  type67_modes::ListScreenInit(base, colors, frame, calls);
  type67_modes::ListScreenInit(base, list, frame, calls);
  options_screen::RunInit(base, 0x821CAAB0, options, frame);
  options_screen::RunInit(base, 0x821C8F48, controls, frame);
  for (const save_screen::InitInfo& info : save_screen::kInits) {
    if (info.address == 0x821A87F0) {
      save_screen::RunInit(base, info.address, saves, frame - info.frame);
    }
  }
  // The machines added to the dispatcher (+36, the count at +236; no bound
  // check), the casts' null checks on the members kept.
  const schema::ModeDispatcher dispatcher = DispatcherOf(base, mode);
  const auto add = [&](uint32_t machine) {
    const uint32_t count = dispatcher.handler_count();
    dispatcher.set_handler_count(count + 1);
    schema::host::StoreU32(dispatcher.handlers() + (count << 2), machine);  // a dual view (E5 step 4)
  };
  add(member != 0 ? schema::Type8Screen0(base, m.screen0_address()).machine_address() : 0);
  add(dialog);
  add(panel != 0 ? schema::Type8PanelScreen(base, panel).machine_address() : 0);
  add(colors != 0 ? colors + 4 : 0);  // the list screens' machines at +4
  add(list != 0 ? list + 4 : 0);
  add(options);
  add(controls);
  add(saves);
  // Each machine opens on its message and keeps its movie (+9612 = 1, +9616
  // = 0), in the original's order of stores.
  const uint32_t screen = member != 0 ? member + 4 : 0;
  // The word after each machine is its screen's (Type8Screen0's
  // delete_movie here; the list and panel screens' +9620 have no field yet).
  StoreU32(base, screen + Machine::kSize, 0);
  Machine(base, screen).set_reenter(1);
  const auto open = [&](uint32_t machine) {
    Machine(base, machine).set_reenter(1);
    StoreU32(base, machine + Machine::kSize, 0);
  };
  const auto open_screen = [&](uint32_t machine) {
    const schema::ScreenMachine s(base, machine);
    s.set_reenter(1);
    s.set_delete_movie(0);
  };
  open(colors != 0 ? colors + 4 : 0);
  open(list != 0 ? list + 4 : 0);
  {
    const schema::Type8Dialog d(base, dialog);
    Machine(base, d.machine_address()).set_reenter(1);
    d.set_delete_movies(0);
  }
  open(panel != 0 ? schema::Type8PanelScreen(base, panel).machine_address() : 0);
  open_screen(options);
  open_screen(controls);
  open_screen(saves);
  {
    const uint64_t args[6] = {1, 0, 0xFFFFFFFFFFFFFFFFull, 0, 0, 0};
    calls.call_set(kLoadingShow, args, 3, frame, 0x8223D884);
  }
  const uint32_t passes = G::fade_in_passes(base);
  mode_tick::ScreenFadesFadeIn(base, passes, 0, 0, 0, 0);
  // The delegate's member functor (WaitForLoads through Mode::Slot8Thunk),
  // built at the frame's +80.
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 80, kSlot8Thunk);
  SetDelegate(base, mode, frame + 80);
  return passes;
}

}  // namespace type8_mode
