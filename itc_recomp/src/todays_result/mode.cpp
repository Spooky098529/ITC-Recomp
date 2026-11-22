// The type 9 mode's Init, delegate functions, IsLoaded and the helpers over
// its block (docs/functions/todays-result.md): see todays_result.h.

#include "base/log.h"
#include "schema/boot_mode.h"
#include "schema/game_modes.h"
#include "schema/race_event.h"
#include "todays_result/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace todays_result {

namespace {

// The delegate's vtable (ModeDelegate::Invoke) and the thunks.
constexpr uint32_t kDelegateVtable = 0x82010FA0;
constexpr uint32_t kNextModeThunk = 0x82235C08;
constexpr uint32_t kSlot8Thunk = 0x8223D4B0;
constexpr uint32_t kSlot10Thunk = 0x8223D4A0;
constexpr uint32_t kSlot11Thunk = 0x8222FC60;
constexpr uint32_t kSlot12Thunk = 0x8222FC70;

constexpr uint32_t kStringFile = 171;

using Mode = schema::ModeType9;
using Block = schema::ModeType9Block;
// Either machine, as the mode's functions read them (their words are the
// StateMachine's).
using Machine = schema::StateMachine;

// The 8-byte word at `at`, as the originals copy it.
uint64_t Load64(uint8_t* base, uint32_t at) {
  return (uint64_t(Load(base, at)) << 32) | Load(base, at + 4);
}
void Store64(uint8_t* base, uint32_t at, uint64_t value) {
  Store(base, at, uint32_t(value >> 32));
  Store(base, at + 4, uint32_t(value));
}

// The mode's delegate (+8, its holder +4) from the doubleword at `slot`.
// The holder 0 when the delegate's address wraps to 0 (never: kept as the
// originals test it).
void Delegate(uint8_t* base, uint32_t mode, uint32_t slot) {
  const Mode m(base, mode);
  const uint32_t delegate = mode + 8;  // the delegate (in host memory, E5 step 4)
  uint32_t holder = 0;
  if (delegate != 0) {
    holder = delegate;
    m.set_delegate_object(mode);
    // The function and the adjustment, as one doubleword.
    const uint64_t words = Load64(base, slot);
    m.set_delegate_function(uint32_t(words >> 32));
    m.set_delegate_adjustment(uint32_t(words));
    m.set_delegate_vtable(kDelegateVtable);
  }
  m.set_delegate_ptr(holder);
}
// The same to `function`: the doubleword (function, 0) written at `slot`
// first, as the originals build it there.
void SetDelegate(uint8_t* base, uint32_t mode, uint32_t slot, uint32_t function) {
  Store(base, slot + 4, 0);
  Store(base, slot, function);
  Delegate(base, mode, slot);
}

// The answer (r3) cleared: its next type, then its changed byte.
void ClearAnswer(uint8_t* base, uint32_t answer) {
  const schema::ModeAnswer a(base, answer);
  a.set_next_type(0);
  a.set_changed(0);
}

// The machine of index `index` (0 today's event, 27 today's result), 0 for
// any other (whose words the originals read as they are).
uint32_t MachineOf(const Block& block, uint32_t index) {
  if (index == 0) {
    return block.event_address();
  }
  return index == kResultIndex ? block.result_address() : 0;
}
// The machine the block's +19280 names.
uint32_t ShownMachine(const Block& block) {
  return MachineOf(block, block.shown());
}

void Dispatch(uint8_t* base, const Calls& calls, uint32_t mode, uint32_t frame, uint32_t lr) {
  // Its r4 a leftover the dispatcher doesn't read.
  calls.Call(kDispatch, frame, lr, mode + 36, mode);  // the dispatcher (+36)
}

}  // namespace

void FindShown(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_8225D658 ModeType9::FindShown");
  for (uint32_t index = 0; index < 47; index++) {
    if (index != 0 && index != kResultIndex) {
      continue;
    }
    const uint32_t machine = MachineOf(Block(base, block), index);
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(Machine(base, machine).state());
    if (state == 5 || state == 4) {
      Block(base, block).set_shown(index);
    }
  }
}

uint32_t Kind(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225BA48 TodaysEvent::Kind");
  Frame(base, sp, 96);
  const schema::SaveData save(base, vehicle_accessors::SaveDataOf(base, 0));
  if (save.unlocks_shown_count() == 0) {
    return 0;
  }
  const int32_t movie = int32_t(save.movie_after_results());
  if (movie < 2) {
    return 0;
  }
  if (movie <= 5) {
    return 1;
  }
  return movie == 6 ? 2 : 0;
}

uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225D6D0 ModeType9::Init");
  const uint32_t frame = Frame(base, sp, 112);
  const Mode m(base, mode);
  auto block = [&] { return m.block(); };
  calls.Call(kScreenStart, frame, 0x8225D6E8, mode);
  calls.Call(kStringsLoad, frame, 0x8225D6F8, G::string_table(base), kStringFile);
  const uint64_t allocated =
      calls.Call(kVirtualAlloc, frame, 0x8225D70C, 0, Block::kSize, 4096, 4);
  m.set_block(uint32_t(allocated));
  bool replaced = false;
  ResultInitStates(base, uint32_t(allocated), frame - kInitStatesFrame, &replaced);
  EventInitStates(base, block().event_address(), frame - kInitStatesFrame, &replaced);
  // The two machines added to the dispatcher: today's result's, then today's
  // event's.
  for (const bool event : {false, true}) {
    const uint32_t count = m.handler_count();
    const Block b = block();
    const uint32_t machine = event ? b.event_address() : b.result_address();
    m.set_handler_count(count + 1);
    schema::host::StoreU32(m.handlers() + (count << 2), machine);  // guest order (E5 step 4)
  }
  const uint32_t shown = Kind(base, frame) == 0 ? kResultIndex : 0;
  block().set_shown_at_init(shown);
  block().set_shown(shown);
  Store(base, frame + 84, 0);
  block().set_result_reenter(1);
  block().set_result_delete_movie(0);
  Store(base, frame + 80, kSlot8Thunk);
  block().set_event_reenter(1);
  block().set_event_delete_movie(0);
  // The delegate: WaitForLoads through Mode::Slot8Thunk.
  Delegate(base, mode, frame + 80);
  calls.Call(kLoadingShow, frame, 0x8225D7F8, 1, 0, ~uint64_t(0));
  return calls.Call(kFadeIn, frame, 0x8225D814, G::fade_in_passes(base), 0, 0, 0);
}

void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225D918 ModeType9::WaitForLoads");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  calls.Call(kLoadingRun, frame, 0x8225D940, ~uint64_t(0));
  const Mode m(base, mode);
  const Block block = m.block();
  if (block.result_loaded() != 0 && block.event_loaded() != 0) {
    const uint32_t shown = block.shown();
    if (shown == 0) {
      PostMode(m, 8);
    } else if (shown == kResultIndex) {
      PostMode(m, 6);
    }
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
    calls.Call(kLoadingFinish, frame, 0x8225DA0C, ~uint64_t(0));
  }
  PostMode(m, 1);
  Dispatch(base, calls, mode, frame, 0x8225DA5C);
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225DC38 ModeType9::NextMode");
  const uint32_t frame = Frame(base, sp, 144);
  ClearAnswer(base, answer);
  const Mode m(base, mode);
  FindShown(base, m.block().address);
  auto shown_state = [&] {
    const uint32_t machine = ShownMachine(m.block());
    return int32_t(Machine(base, machine).state());
  };
  if (shown_state() == 8) {
    SetDelegate(base, mode, frame + 80, kSlot11Thunk);
  }
  if (shown_state() == 10) {
    PostMode(m, 4);
  }
  PostMode(m, 1);
  PostMode(m, 0);
  Dispatch(base, calls, mode, frame, 0x8225DDE8);
  // The answer (byte 1, the next mode) built over the frame's doubleword
  // at +80 (its other three bytes as they are).
  uint32_t next = 0;
  if (shown_state() == 13) {
    next = 2;
  } else if (shown_state() == 20) {
    next = 3;
  } else {
    return;
  }
  const schema::ModeAnswer built(base, frame + 80);
  built.set_next_type(next);
  built.set_changed(1);
  Store64(base, answer, Load64(base, built.address));
}

void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225DB70 ModeType9::LeaveScreen");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  const Mode m(base, mode);
  FindShown(base, m.block().address);
  PostMode(m, 3);
  Dispatch(base, calls, mode, frame, 0x8225DBEC);
  SetDelegate(base, mode, frame + 80, kSlot10Thunk);
}

void WaitForShown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225DA68 ModeType9::WaitForShown");
  const uint32_t frame = Frame(base, sp, 112);
  ClearAnswer(base, answer);
  const Mode m(base, mode);
  FindShown(base, m.block().address);
  const uint32_t machine = ShownMachine(m.block());
  if (int32_t(Machine(base, machine).loaded()) == 1) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
  }
  PostMode(m, 1);
  Dispatch(base, calls, mode, frame, 0x8225DB54);
}

void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225DE78 ModeType9::Leaving");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  const Mode m(base, mode);
  PostMode(m, 4);
  Dispatch(base, calls, mode, frame, 0x8225DEEC);
  const Block block = m.block();
  for (uint32_t index = 0; index < 47; index++) {
    if (index != 0 && index != kResultIndex) {
      continue;
    }
    const uint32_t machine = MachineOf(block, index);
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(Machine(base, machine).state());
    if (state != 6 && state != 13 && state != 14 && state != 12 && state != 20) {
      return;
    }
  }
  SetDelegate(base, mode, frame + 80, kSlot12Thunk);
}

uint32_t IsLoaded(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8225D6A8 ModeType9::IsLoaded");
  // r3 is the mode's +32 interface: the mode's +848.
  const Block block = Mode(base, object - 32).block();
  if (block.result_loaded() == 0) {
    return 0;
  }
  return block.event_loaded() != 0 ? 1 : 0;
}

}  // namespace todays_result
