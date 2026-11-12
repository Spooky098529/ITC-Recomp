// The type 12 mode's Init, delegate functions, +32 slots and the helpers
// over its block (docs/functions/network-mode.md): see network_mode.h.
// The Init: The fourteen InitStates as their plain C++ (init.cpp), the
// other callees through their overrides (guest calls from the original's
// frame with its return addresses): the screen start, Strings::Load,
// Archive::WaitIdle, StandScene::Create, VirtualAlloc, ScreenFades::FadeIn
// and LoadingScreen::Show. The session's words and the network's state are
// plain guest reads, as the original's (no Live call here).

#include "network_mode/network_mode.h"

#include "base/log.h"
#include "network_mode/internal.h"
#include "guest.h"

#include <bit>

namespace network_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;

using InitStatesFn = uint32_t (*)(uint8_t*, uint32_t, uint32_t, bool*);

// The machines in the order the Init sets them up and adds them to the
// dispatcher: their InitStates, their offset in the block and its frame.
struct Member {
  InitStatesFn init;
  uint32_t offset;
  uint32_t frame;
};
constexpr Member kMembers[] = {
    {ModeSelectInitStates, kModeSelectAt, kModeSelectInitFrame},
    {Screen1InitStates, kScreen1At, kScreen1InitFrame},
    {Screen3InitStates, kScreen3At, kScreen3InitFrame},
    {RankingInitStates, kRankingAt, kRankingInitFrame},
    {TaCourseInitStates, kTaCourseAt, kTaCourseInitFrame},
    {Screen6InitStates, kScreen6At, kScreen6InitFrame},
    {Screen7InitStates, kScreen7At, kScreen7InitFrame},
    {Dialog10InitStates, kDialog10At, kDialog10InitFrame},
    {Dialog11InitStates, kDialog11At, kDialog11InitFrame},
    {Screen8InitStates, kScreen8At, kScreen8InitFrame},
    {Screen9InitStates, kScreen9At, kScreen9InitFrame},
    {Dialog12InitStates, kDialog12At, kDialog12InitFrame},
    {Dialog13InitStates, kDialog13At, kDialog13InitFrame},
    {PowerGraphInitStates, kPowerGraphAt, kPowerGraphInitFrame},
};
// The machines whose +9612 = 1 and +9616 = 0 are stored directly (+9616
// first for the first of them), after the first four's (from the frame's
// words), in the original's order.
constexpr uint32_t kOpened[] = {kTaCourseAt,  kScreen6At, kScreen7At,  kDialog10At, kDialog11At,
                                kScreen8At,   kScreen9At, kDialog12At, kDialog13At, kPowerGraphAt};

// The first screen: the ranking (4) or screen 3 back from a race (the mode
// before 1) with a session whose +244 or +240 is set; else screen 9 back
// from a race with the network's state 0, screen 7 with it 1, else the mode
// select (0).
uint32_t FirstScreen(uint8_t* base) {
  const int32_t before = int32_t(LoadU32(base, kModeBefore));
  const uint32_t session = LoadU32(base, kSession);
  if (before == 1 && session != 0) {
    if (LoadU32(base, session + 244) != 0) {
      return 4;
    }
    if (LoadU32(base, session + 240) != 0) {
      return 3;
    }
  }
  const int32_t state = int32_t(LoadU32(base, kNetworkState));
  if (before == 1 && state == 0) {
    return 9;
  }
  return state == 1 ? 7 : 0;
}

}  // namespace

uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82278058 ModeType12::Init");
  const uint32_t frame = sp - kInitFrame;
  StoreU32(base, frame, sp);
  StoreU32(base, sp + 20, mode);
  auto block = [&] { return LoadU32(base, mode + kBlock); };
  calls.Call(kScreenStart, frame, 0x82278070, mode);
  const uint64_t strings =
      calls.Call(kStringsLoad, frame, 0x82278080, LoadU32(base, kStringTable), kStringFile);
  calls.Call(kArchiveWaitIdle, frame, 0x82278084, strings, kStringFile);
  calls.Call(kStandSceneCreate, frame, 0x8227808C, 1);
  const uint64_t allocated = calls.Call(kVirtualAlloc, frame, 0x822780A4, 0, kBlockBytes, 4096, 4);
  StoreU32(base, mode + kBlock, uint32_t(allocated));
  // The fades' table's page, kept in the frame for the second FadeIn.
  StoreU32(base, frame + 112, kFadesPasses & 0xFFFF0000u);
  calls.Call(kFadeIn, frame, 0x822780C8, LoadU32(base, kFadesPasses), 0, 0, 0);
  for (const Member& m : kMembers) {
    bool replaced = false;
    m.init(base, block() + m.offset, frame - m.frame, &replaced);
  }
  const uint32_t dispatcher = mode + kDispatcher;
  for (const Member& m : kMembers) {
    const uint32_t count = LoadU32(base, dispatcher + 200);
    const uint32_t machine = block() + m.offset;
    StoreU32(base, dispatcher + 200, count + 1);
    StoreU32(base, dispatcher + (count << 2), machine);
  }
  const uint32_t first = FirstScreen(base);
  StoreU32(base, block() + kFirstShown, first);
  StoreU32(base, block() + kShown, first);
  // The frame's words the first four machines' +9612 and +9616 are copied
  // from.
  StoreU32(base, frame + 88, 0);
  StoreU32(base, frame + 96, 0);
  StoreU32(base, frame + 80, 0);
  StoreU32(base, frame + 104, 0);
  StoreU32(base, frame + 84, 1);
  StoreU32(base, frame + 92, 1);
  StoreU32(base, block() + kModeSelectAt + 9612, 1);
  StoreU32(base, frame + 100, 1);
  StoreU32(base, block() + kModeSelectAt + 9616, 0);
  StoreU32(base, block() + kScreen1At + 9612, LoadU32(base, frame + 84));
  StoreU32(base, block() + kScreen1At + 9616, LoadU32(base, frame + 88));
  StoreU32(base, block() + kScreen3At + 9612, LoadU32(base, frame + 92));
  StoreU32(base, block() + kScreen3At + 9616, LoadU32(base, frame + 96));
  StoreU32(base, block() + kRankingAt + 9612, LoadU32(base, frame + 100));
  StoreU32(base, block() + kRankingAt + 9616, LoadU32(base, frame + 104));
  bool first_opened = true;
  for (const uint32_t offset : kOpened) {
    if (first_opened) {
      StoreU32(base, block() + offset + 9616, 0);
      StoreU32(base, block() + offset + 9612, 1);
      first_opened = false;
    } else {
      StoreU32(base, block() + offset + 9612, 1);
      StoreU32(base, block() + offset + 9616, 0);
    }
  }
  calls.Call(kLoadingShow, frame, 0x82278568, 1, 0, ~uint64_t(0));
  const uint64_t r3 =
      calls.Call(kFadeIn, frame, 0x82278584, LoadU32(base, LoadU32(base, frame + 112) + 0x1BEC),
                 0, 0, 0);
  // The delegate: WaitForLoads through Mode::Slot8Thunk.
  StoreU32(base, frame + 116, 0);
  StoreU32(base, frame + 112, kSlot8Thunk);
  const uint32_t delegate = mode + 8;
  uint32_t holder = 0;
  if (delegate != 0) {
    holder = delegate;
    const uint32_t function = LoadU32(base, frame + 112), adjustment = LoadU32(base, frame + 116);
    StoreU32(base, delegate + 4, mode);
    StoreU32(base, delegate + 8, function);
    StoreU32(base, delegate + 12, adjustment);
    StoreU32(base, delegate + 0, kDelegateVtable);
  }
  StoreU32(base, mode + 4, holder);
  StoreU32(base, block() + kSessionMade, 0);
  return r3;
}

namespace {

// The delegate's thunks (Mode::NextModeThunk, Mode::Slot10Thunk to 12).
constexpr uint32_t kNextModeThunk = 0x82235C08;
constexpr uint32_t kSlot10Thunk = 0x8223D4A0;
constexpr uint32_t kSlot11Thunk = 0x8222FC60;
constexpr uint32_t kSlot12Thunk = 0x8222FC70;

// The callees by address (through their overrides).
constexpr uint32_t kDispatch = 0x821062E8;
constexpr uint32_t kLoadingRun = 0x82247898;
constexpr uint32_t kLoadingFinish = 0x82247A60;
constexpr uint32_t kFadeOut = 0x8209D0C8;
constexpr uint32_t kSoundPlay = 0x82175850;
constexpr uint32_t kPollTeardown = 0x82179AE8;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kOperatorDelete = 0x825A70E8;
constexpr uint32_t kOperatorDeleteArray = 0x825A71C0;
constexpr uint32_t kMemRelease = 0x8000;
// The time attack's race set-up.
constexpr uint32_t kResetGlobal = 0x821021A8;
constexpr uint32_t kRaceSetRoute = 0x82101D38;
constexpr uint32_t kStagesSetRoute = 0x82101E90;
constexpr uint32_t kSetWord59618 = 0x820FF918;
constexpr uint32_t kCarInit = 0x820CD5D8;
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kCarRefresh = 0x820C4998;
constexpr uint32_t kAddRacer = 0x821017C0;
constexpr uint32_t kStagesAddRacer = 0x820FFAB8;
constexpr uint32_t kAddStage = 0x82101828;
constexpr uint32_t kSetWord32 = 0x820FF968;
constexpr uint32_t kPlaceRacers = 0x82102048;
constexpr uint32_t kStageList = 0x59620;
constexpr uint32_t kZero = 0x82000A34;
// The sound manager's slots (bytes into its vtable).
constexpr uint32_t kReleaseBankSlot = 20;
constexpr uint32_t kIsBankLoadingSlot = 24;
constexpr uint32_t kStopCueSlot = 52;
constexpr uint32_t kIsCuePlayingSlot = 56;

// The machines by index (ModeType12::Machine's jump table); 2 none.
constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMachineAt[15] = {
    kModeSelectAt, kScreen1At, kNone,       kScreen3At,  kRankingAt,
    kTaCourseAt,   kScreen6At, kScreen7At,  kScreen8At,  kScreen9At,
    kDialog10At,   kDialog11At, kDialog12At, kDialog13At, kPowerGraphAt};
// AllLoaded's order.
constexpr uint32_t kLoadedOrder[14] = {kModeSelectAt, kScreen1At,  kScreen3At,  kRankingAt,
                                       kTaCourseAt,   kScreen6At,  kScreen7At,  kDialog10At,
                                       kDialog11At,   kScreen8At,  kScreen9At,  kDialog12At,
                                       kDialog13At,   kPowerGraphAt};

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
  const uint32_t delegate = mode + 8;
  uint32_t holder = 0;
  if (delegate != 0) {
    holder = delegate;
    Store(base, delegate + 4, mode);
    Store64(base, delegate + 8, Load64(base, slot));
    Store(base, delegate + 0, kDelegateVtable);
  }
  Store(base, mode + 4, holder);
}
// The same to `function`: the doubleword (function, 0) written at `slot`
// first, as the originals build it there.
void SetDelegate(uint8_t* base, uint32_t mode, uint32_t slot, uint32_t function) {
  Store(base, slot + 4, 0);
  Store(base, slot, function);
  Delegate(base, mode, slot);
}

// The answer (r3) cleared: +4 and its first byte.
void ClearAnswer(uint8_t* base, uint32_t answer) {
  Store(base, answer + 4, 0);
  guest::StoreU8(base, answer, 0);
}

uint32_t BlockOf(uint8_t* base, uint32_t mode) { return Load(base, mode + kBlock); }

int32_t ShownState(uint8_t* base, uint32_t mode) {
  const uint32_t block = BlockOf(base, mode);
  return int32_t(Load(base, Machine(block, Load(base, block + kShown)) + kMachineState));
}

void Dispatch(const Calls& calls, uint32_t mode, uint32_t frame, uint32_t lr) {
  // Its r4 a leftover the dispatcher doesn't read.
  calls.Call(kDispatch, frame, lr, mode + kDispatcher, mode);
}

// The sound manager's slot (bytes into its vtable) with r4 and r5; the
// manager loaded again for each call, as the originals do.
uint64_t SoundSlot(uint8_t* base, const Calls& calls, uint32_t slot, uint32_t frame, uint32_t lr,
                   uint32_t r4, uint32_t r5 = 0) {
  const uint32_t manager = Load(base, kSoundManager);
  return calls.Call(Load(base, Load(base, manager) + slot), frame, lr, manager, r4, r5);
}

// The answer {byte 1, the frame's three bytes after it, `next`} at the
// frame's +80, kept at the block's +0x21014, and the delegate set to
// Teardown (Mode::Slot12Thunk), in the originals' order.
void Answer(uint8_t* base, uint32_t mode, uint32_t block, uint32_t frame) {
  guest::StoreU8(base, frame + 80, 1);
  const uint64_t answer = Load64(base, frame + 80);
  Store(base, frame + 84, 0);
  Store64(base, block + kNextAnswer, answer);
  Store(base, frame + 80, kSlot12Thunk);
  Delegate(base, mode, frame + 80);
}

// sub_82291670 Session::Release (r3 the session).
void SessionRelease(uint8_t* base, uint32_t session, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82291670 Session::Release");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t rows = session + kSessionRows;
  if (Load(base, rows) != 0) {
    calls.Call(kOperatorDelete, frame, 0x82291698, Load(base, rows));
    Store(base, rows, 0);
  }
  if (Load(base, kSessionBuffer) != 0) {
    calls.Call(kOperatorDeleteArray, frame, 0x822916B4, Load(base, kSessionBuffer));
    Store(base, kSessionBuffer, 0);
  }
}

// NextMode's time attack race (screen 5 in state 11), in its frame: the car
// built at +96.
void SetUpTimeAttack(uint8_t* base, uint32_t mode, uint32_t frame, const Calls& calls) {
  calls.Call(kResetGlobal, frame, 0x8227BB10);
  Store(base, Load(base, kRaceData) + 0xA3AF0, 6);
  Store(base, Load(base, kRaceData) + 0xA3AF4, 7);
  calls.Call(kRaceSetRoute, frame, 0x8227BB4C, Load(base, kTaRoute), 48);
  calls.Call(kStagesSetRoute, frame, 0x8227BB6C, Load(base, kRaceData) + kStageList,
             Load(base, kTaRoute), 48, ~uint64_t(0));
  if (Load(base, kTaHasKind) != 0) {
    const int32_t kind = int32_t(Load(base, kTaKind));
    calls.Call(kSetWord59618, frame, 0x8227BBAC, kind == 1 ? 2 : kind == 2 ? 1 : 3);
  }
  const uint32_t car = frame + 96;
  calls.Call(kCarInit, frame, 0x8227BBB4, car);
  calls.Call(kMemcpy, frame, 0x8227BBCC, car, kTaCar, kTaCarBytes);
  Store(base, car + 3892, Load(base, kTaCarWord));
  calls.Call(kCarRefresh, frame, 0x8227BBE0, car);
  const double zero = double(std::bit_cast<float>(Load(base, kZero)));
  calls.Call(kAddRacer, frame, 0x8227BBF8, car, 0, 0, 0, &zero);
  calls.Call(kStagesAddRacer, frame, 0x8227BC10, Load(base, kRaceData) + kStageList, 0, 2,
             ~uint64_t(0));
  calls.Call(kAddStage, frame, 0x8227BC18, 1);
  calls.Call(kSetWord32, frame, 0x8227BC20, Load(base, kTaKind));
  calls.Call(kPlaceRacers, frame, 0x8227BC24);
  // The answer 1 (the race), the session's +240 and +216.
  Store(base, frame + 84, 1);
  guest::StoreU8(base, frame + 80, 1);
  const uint64_t answer = Load64(base, frame + 80);
  Store(base, frame + 84, 0);
  Store(base, frame + 80, kSlot12Thunk);
  Store(base, Load(base, kSession) + 240, 0);
  Store(base, Load(base, kSession) + 216, Load(base, kTaSessionWord));
  Store64(base, BlockOf(base, mode) + kNextAnswer, answer);
  Delegate(base, mode, frame + 80);
}

}  // namespace

uint32_t Machine(uint32_t block, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82277E10 ModeType12::Machine");
  if (index > 14 || kMachineAt[index] == kNone) {
    return 0;
  }
  return block + kMachineAt[index];
}

uint32_t FindShown(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_82277FF0 ModeType12::FindShown");
  for (uint32_t index = 0; index < 15; index++) {
    const uint32_t machine = Machine(block, index);
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(Load(base, machine + kMachineState));
    if (state == 5 || state == 4) {
      Store(base, block + kShown, index);
      return index;
    }
  }
  return 14;
}

uint32_t AllLoaded(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_82277EE0 ModeType12::AllLoaded");
  for (const uint32_t offset : kLoadedOrder) {
    if (Load(base, block + offset + kMachineLoaded) == 0) {
      return 0;
    }
  }
  return 1;
}

void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227B440 ModeType12::WaitForLoads");
  const uint32_t frame = Frame(base, sp, 176);
  ClearAnswer(base, answer);
  calls.Call(kLoadingRun, frame, 0x8227B468, ~uint64_t(0));
  if (AllLoaded(base, BlockOf(base, mode)) == 1) {
    Store(base, frame + 80, 1);
    const uint64_t loading =
        SoundSlot(base, calls, kIsBankLoadingSlot, frame, 0x8227B4A0, frame + 80);
    if ((loading & 0xFF) == 0) {
      calls.Call(kLoadingFinish, frame, 0x8227B4B0, ~uint64_t(0));
      const int32_t first = int32_t(Load(base, BlockOf(base, mode) + kShown));
      // The first screen's open message, its cue (bank, cue words in the
      // frame) and whether the sign-in watch's check goes on.
      bool watch = true;
      if (first == 0) {
        PostRing(base, mode + kRing, 5);
        Store(base, frame + 108, 1);
        Store(base, frame + 112, 1);
        calls.Call(kSoundPlay, frame, 0x8227B6BC, frame + 112, frame + 108);
        watch = false;
      } else if (first == 3) {
        PostRing(base, mode + kRing, 9);
        Store(base, frame + 100, 1);
        Store(base, frame + 104, 1);
        calls.Call(kSoundPlay, frame, 0x8227B594, frame + 104, frame + 100);
      } else if (first == 4) {
        PostRing(base, mode + kRing, 10);
        Store(base, frame + 92, 1);
        Store(base, frame + 96, 1);
        calls.Call(kSoundPlay, frame, 0x8227B594, frame + 96, frame + 92);
      } else if (first == 7) {
        PostRing(base, mode + kRing, 13);
        Store(base, frame + 88, 1);
        Store(base, frame + 84, 3);
        calls.Call(kSoundPlay, frame, 0x8227B594, frame + 88, frame + 84);
      } else if (first == 9) {
        PostRing(base, mode + kRing, 15);
      } else {
        watch = false;
      }
      if (watch) {
        Store(base, kSignInWatch + 16, 0);
      }
      calls.Call(kFadeOut, frame, 0x8227B6D4, 1, 0, 0, 0);
      SetDelegate(base, mode, frame + 112, kNextModeThunk);
    }
  }
  PostRing(base, mode + kRing, 1);
  Dispatch(calls, mode, frame, 0x8227B760);
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227B930 ModeType12::NextMode");
  const uint32_t frame = Frame(base, sp, 4112);
  ClearAnswer(base, answer);
  FindShown(base, BlockOf(base, mode));
  if (ShownState(base, mode) == 8) {
    SetDelegate(base, mode, frame + 80, kSlot11Thunk);
  }
  if (ShownState(base, mode) == 10) {
    PostRing(base, mode + kRing, 4);
  }
  PostRing(base, mode + kRing, 1);
  PostRing(base, mode + kRing, 0);
  Dispatch(calls, mode, frame, 0x8227BABC);
  const uint32_t block = BlockOf(base, mode);
  if (int32_t(Load(base, block + kScreen7At + kMachineState)) == 11 ||
      int32_t(Load(base, block + kRankingAt + kMachineState)) == 11) {
    // The race.
    Store(base, frame + 84, 1);
    Answer(base, mode, block, frame);
  } else if (int32_t(Load(base, block + kTaCourseAt + kMachineState)) == 11) {
    SetUpTimeAttack(base, mode, frame, calls);
  } else if (ShownState(base, mode) == 13) {
    // The garage.
    Store(base, frame + 84, 2);
    Answer(base, mode, block, frame);
  } else if (ShownState(base, mode) == 14) {
    // The title: the answer kept before the title's flag and the delegate.
    Store(base, frame + 84, 4);
    guest::StoreU8(base, frame + 80, 1);
    const uint64_t next = Load64(base, frame + 80);
    Store(base, frame + 84, 0);
    Store64(base, block + kNextAnswer, next);
    Store(base, frame + 80, kSlot12Thunk);
    Store(base, kTitleFlag, Load(base, BlockOf(base, mode) + kSessionMade) != 0 ? 1 : 0);
    Delegate(base, mode, frame + 80);
    SessionFree(base, frame, calls);
  }
  // The mode again (15) or the title (6), over any answer above.
  const int32_t state = ShownState(base, mode);
  if (state == 15) {
    Store(base, frame + 84, 12);
  } else if (state == 6) {
    Store(base, frame + 84, 4);
  } else {
    return;
  }
  Answer(base, mode, BlockOf(base, mode), frame);
}

void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227B868 ModeType12::LeaveScreen");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  FindShown(base, BlockOf(base, mode));
  PostRing(base, mode + kRing, 3);
  Dispatch(calls, mode, frame, 0x8227B8E4);
  SetDelegate(base, mode, frame + 80, kSlot10Thunk);
}

void WaitForShown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227B770 ModeType12::WaitForShown");
  const uint32_t frame = Frame(base, sp, 112);
  ClearAnswer(base, answer);
  FindShown(base, BlockOf(base, mode));
  const uint32_t block = BlockOf(base, mode);
  const uint32_t machine = Machine(block, Load(base, block + kShown));
  if (int32_t(Load(base, machine + kMachineLoaded)) == 1) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
  }
  PostRing(base, mode + kRing, 1);
  Dispatch(calls, mode, frame, 0x8227B84C);
}

void Teardown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8227BE10 ModeType12::Teardown");
  const uint32_t frame = Frame(base, sp, 144);
  ClearAnswer(base, answer);
  // The screen's cue stopped: bank 1's cue 3 for screen 7, else its cue 1.
  uint32_t bank = frame + 92, cue = frame + 88;
  if (int32_t(Load(base, BlockOf(base, mode) + kShown)) == 7) {
    Store(base, frame + 84, 1);
    Store(base, frame + 80, 3);
    bank = frame + 84;
    cue = frame + 80;
  } else {
    Store(base, frame + 88, 1);
    Store(base, frame + 92, 1);
  }
  SoundSlot(base, calls, kStopCueSlot, frame, 0x8227BE8C, bank, cue);
  PostRing(base, mode + kRing, 1);
  PostRing(base, mode + kRing, 0);
  Dispatch(calls, mode, frame, 0x8227BF24);
  const uint64_t done = calls.Call(kPollTeardown, frame, 0x8227BF28);
  if (int32_t(done) == 0) {
    return;
  }
  const uint32_t block = BlockOf(base, mode);
  const uint32_t session = Load(base, kSession);
  Store(base, answer + 0, Load(base, block + kNextAnswer));
  Store(base, answer + 4, Load(base, block + kNextAnswer + 4));
  if (session != 0 && Load(base, block + kSessionMade) != 0) {
    SessionFree(base, frame, calls);
  }
}

uint64_t ForceExit(uint8_t* base, uint32_t object, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822788D8 ModeType12::ForceExit");
  const uint32_t frame = Frame(base, sp, 240);
  const uint32_t shown = Load(base, Load(base, object + 816) + kShown);
  Store(base, frame + 80, 1);
  if ((SoundSlot(base, calls, kIsBankLoadingSlot, frame, 0x8227891C, frame + 80) & 0xFF) == 0) {
    // Bank 1's cues 0 to 3 stopped when playing: each check's (cue, bank)
    // words and the stop's, at the frame's offsets, with the return
    // addresses.
    struct Cue {
      uint32_t cue, at, stop_at, playing_lr, stop_lr;
    };
    static constexpr Cue kCues[] = {{0, 84, 92, 0x82278950, 0x8227897C},
                                    {1, 100, 108, 0x822789A0, 0x822789CC},
                                    {2, 116, 124, 0x822789F0, 0x82278A1C},
                                    {3, 132, 140, 0x82278A44, 0x82278A70}};
    for (const Cue& c : kCues) {
      Store(base, frame + c.at, c.cue);
      Store(base, frame + c.at + 4, 1);
      const uint64_t playing = SoundSlot(base, calls, kIsCuePlayingSlot, frame, c.playing_lr,
                                         frame + c.at + 4, frame + c.at);
      if ((playing & 0xFF) != 0) {
        Store(base, frame + c.stop_at, c.cue);
        Store(base, frame + c.stop_at + 4, 1);
        SoundSlot(base, calls, kStopCueSlot, frame, c.stop_lr, frame + c.stop_at + 4,
                  frame + c.stop_at);
      }
    }
  }
  // Screen 9 running: bank 2's cue 0 stopped when playing and the bank
  // isn't loading, and the bank released.
  if (int32_t(Load(base, Load(base, object + 816) + kScreen9At + kMachineState)) == 5) {
    Store(base, frame + 148, 2);
    if ((SoundSlot(base, calls, kIsBankLoadingSlot, frame, 0x82278AA4, frame + 148) & 0xFF) == 0) {
      Store(base, frame + 152, 0);
      Store(base, frame + 156, 2);
      if ((SoundSlot(base, calls, kIsCuePlayingSlot, frame, 0x82278AD0, frame + 156,
                     frame + 152) &
           0xFF) != 0) {
        Store(base, frame + 160, 0);
        Store(base, frame + 164, 2);
        SoundSlot(base, calls, kStopCueSlot, frame, 0x82278AFC, frame + 164, frame + 160);
      }
    }
    Store(base, frame + 168, 2);
    SoundSlot(base, calls, kReleaseBankSlot, frame, 0x82278B18, frame + 168);
  }
  SessionFree(base, frame, calls);
  Store(base, Load(base, object + 816) + kPowerGraphAt + kMachineState, 9);
  PostRing(base, object + 208, shown + 20);
  const uint32_t machine = Machine(Load(base, object + 816), shown);
  Store(base, machine + kMachineState, 10);
  return machine;
}

uint32_t Resume(uint8_t* base, uint32_t object, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82278B98 ModeType12::Resume");
  Frame(base, sp, 96);
  const uint32_t block = Load(base, object + 816);
  const uint32_t machine = Machine(block, Load(base, block + kShown));
  Store(base, machine + kMachineAgain, 0);
  PostRing(base, object + 208, 44);
  return machine;
}

uint64_t SessionFree(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82292710 Session::Free");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t session = Load(base, kSession);
  if (session == 0) {
    return 0;
  }
  SessionRelease(base, session, frame, calls);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x82292748, Load(base, kSession), 0, kMemRelease);
  Store(base, kSession, 0);
  return r3;
}

}  // namespace network_mode
