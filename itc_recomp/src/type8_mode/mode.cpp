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
#include "guest.h"
#include "menu_sounds/menu_sounds.h"
#include "mode_tick/mode_tick.h"
#include "save_data/save_data.h"
#include "type8_screens/type8_screens.h"

namespace type8_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kMember = 848, kRing = 240, kDispatcher = 36;
constexpr uint32_t kDelegateVtable = 0x82010FA0;
constexpr uint32_t kNextModeThunk = 0x82235C08, kSlot9Thunk = 0x8223D4C0,
                   kSlot10Thunk = 0x8223D4A0;
constexpr uint32_t kStandHolder = 0x826D83E0, kLeavingFlag = 0x826364EC;
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
  StoreU32(base, answer + 4, 0);
  guest::StoreU8(base, answer + 0, 0);
}

// A message (`message`, 0, 0) queued in the ring at `ring` (its count at
// +600, the start at +604), the count raised first.
void Queue(uint8_t* base, uint32_t ring, uint32_t message) {
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
  StoreU32(base, slot + 0, message);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
}

// The delegate (mode +4 = mode +8 when that isn't 0) from the member
// functor's two words at `words`: its +4 the mode, +8 the doubleword, +0 the
// vtable.
void SetDelegate(uint8_t* base, uint32_t mode, uint32_t words) {
  const uint32_t holder = mode + 4;
  uint32_t delegate = holder + 4;
  if (delegate != 0) {
    const uint32_t function = LoadU32(base, words), adjustment = LoadU32(base, words + 4);
    StoreU32(base, delegate + 4, mode);
    StoreU32(base, delegate + 8, function);
    StoreU32(base, delegate + 12, adjustment);
    StoreU32(base, delegate + 0, kDelegateVtable);
  } else {
    delegate = 0;
  }
  StoreU32(base, holder, delegate);
}

// The 8-byte answer at the frame's +80 (byte 1 over the word there, the
// three bytes after it as the frame holds them, then `next`) stored to the
// mode's +87760; the frame's +84 then 0.
void StoreAnswer(uint8_t* base, uint32_t frame, uint32_t to, uint32_t next, bool next_first) {
  if (next_first) {
    StoreU32(base, frame + 84, next);
    guest::StoreU8(base, frame + 80, 1);
  } else {
    guest::StoreU8(base, frame + 80, 1);
    StoreU32(base, frame + 84, next);
  }
  const uint32_t w0 = LoadU32(base, frame + 80), w1 = LoadU32(base, frame + 84);
  StoreU32(base, frame + 84, 0);
  StoreU32(base, to, w0);
  StoreU32(base, to + 4, w1);
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
  for (const uint32_t word : {9612u, 29156u, 40524u, 19400u, 52352u, 67600u, 86880u}) {
    if (LoadU32(base, member + word) == 0) {
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
  if (ModeAllLoaded(base, mode + kMember) != 0 &&
      (LoadU32(base, mode_tick::kFadeOutCounts) | LoadU32(base, mode_tick::kFadeInCounts)) ==
          0) {
    StoreU32(base, frame + 80, 1);
    const uint32_t manager = LoadU32(base, menu_sounds::kSoundManager);
    const uint64_t ask[6] = {manager, frame + 80, 0, 0, 0, 0};
    const uint64_t loading =
        calls.call_set(LoadU32(base, LoadU32(base, manager) + 24), ask, 2, frame, 0x8223DC38);
    if ((loading & 0xFF) == 0) {
      const uint32_t holder = LoadU32(base, kStandHolder);
      const uint32_t first = LoadU32(base, holder + 4);
      if (LoadU32(base, LoadU32(base, first) + 112) != 0 &&
          LoadU32(base, LoadU32(base, LoadU32(base, holder + 8)) + 112) != 0) {
        calls.call(kCommand, frame, 0x8223DC78, first, 56);
        calls.call(kCommand, frame, 0x8223DC88, LoadU32(base, LoadU32(base, kStandHolder) + 8),
                   57);
        calls.call(kLoadingFinish, frame, 0x8223DC90, kAll, 57);
        StoreU32(base, frame + 88, 1);
        StoreU32(base, frame + 84, 2);
        menu_sounds::PlaySound(base, frame + 88, frame + 84, frame,
                               {const_cast<ScreenCalls*>(&calls), &SoundThunk});
        // The loaded message (2) to the panel screen, the delegate on to
        // NextModeThunk.
        StoreU32(base, frame + 92, 0);
        Queue(base, mode + kRing, 2);
        StoreU32(base, frame + 88, kNextModeThunk);
        SetDelegate(base, mode, frame + 88);
      }
    }
  }
  Queue(base, mode + kRing, 1);
  calls.call(kDispatch, frame, 0x8223DD80, mode + kDispatcher, mode);
  return r3;
}

uint64_t ModeNextMode(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                      const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223DD90 ModeType8::NextMode");
  const uint32_t frame = Frame(base, sp, 144);
  ClearAnswer(base, uint32_t(r3));
  if (LoadU32(base, kPadBlocked0) == 0 && LoadU32(base, kPadBlocked1) == 0) {
    Queue(base, mode + kRing, 1);
  }
  Queue(base, mode + kRing, 0);
  calls.call(kDispatch, frame, 0x8223DE70, mode + kDispatcher, mode);
  const uint32_t member = mode + kMember;
  // A machine's state: the cast's null check on the member at `at`, then
  // the machine at `at` + 4 (0 for a member at address 0).
  const auto state = [&](uint32_t at) {
    const uint32_t machine = member + at != 0 ? member + at + 4 : 0;
    return int32_t(LoadU32(base, machine + 9604));
  };
  const uint32_t to = mode + 87760;
  if (state(30912) == 11 || state(19544) == 11) {
    StoreAnswer(base, frame, to, 1, true);  // the race
  } else {
    const uint32_t screen = member != 0 ? member + 4 : 0;
    if (int32_t(LoadU32(base, screen + 9604)) != 6) {
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
  const uint32_t w0 = LoadU32(base, mode + 87760), w1 = LoadU32(base, mode + 87764);
  StoreU32(base, answer + 0, w0);
  StoreU32(base, answer + 4, w1);
  return r3;
}

uint64_t ModeInterrupt(uint8_t* base, uint32_t object, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223D960 ModeType8::Interrupt");
  Frame(base, sp, 96);
  if (ModeAllLoaded(base, object + 816) == 0) {
    return 0;
  }
  if (int32_t(LoadU32(base, object + 87692)) != 5) {
    return 1;
  }
  Queue(base, object + 208, 30);  // the save screen's cancel
  return 0;
}

uint64_t ModeForceExit(uint8_t* base, uint64_t r3, uint32_t number, uint32_t sp,
                       const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223DA00 ModeType8::ForceExit");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t object = uint32_t(r3);
  StoreU32(base, LoadU32(base, LoadU32(base, LoadU32(base, kStandHolder) + 4)) + 6052, 0);
  StoreU32(base, LoadU32(base, LoadU32(base, LoadU32(base, kStandHolder) + 8)) + 6052, 0);
  calls.call(kSetDrawWord, frame, 0x8223DA4C, LoadU32(base, LoadU32(base, kStandHolder) + 4), 0);
  calls.call(kSetDrawWord, frame, 0x8223DA5C, LoadU32(base, LoadU32(base, kStandHolder) + 8), 0);
  const uint64_t drawn = car_list::DrawAll(base, frame, type8_screens::CarListCalls(calls));
  if (number >= 4) {
    return drawn;
  }
  // Leave (4) queued, the delegate on to Slot9Thunk (PollTeardown), the
  // answer {1, the thunk's low three bytes, 4 or 12}.
  const uint32_t mode = object - 32;
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 80, kSlot9Thunk);
  Queue(base, object + 208, 4);
  if (number < 2) {
    StoreU32(base, kLeavingFlag, 1);
  }
  SetDelegate(base, mode, frame + 80);
  StoreU32(base, frame + 84, number < 2 ? 4 : 12);
  guest::StoreU8(base, frame + 80, 1);
  const uint32_t w0 = LoadU32(base, frame + 80), w1 = LoadU32(base, frame + 84);
  StoreU32(base, object + 87728, w0);
  StoreU32(base, object + 87732, w1);
  return r3 - 28;
}

}  // namespace type8_mode
