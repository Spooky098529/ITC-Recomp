// The type 12 mode's Init (docs/functions/network-mode.md): see
// network_mode.h. The fourteen InitStates as their plain C++ (init.cpp), the
// other callees through their overrides (guest calls from the original's
// frame with its return addresses): the screen start, Strings::Load,
// Archive::WaitIdle, StandScene::Create, VirtualAlloc, ScreenFades::FadeIn
// and LoadingScreen::Show. The session's words and the network's state are
// plain guest reads, as the original's (no Live call here).

#include "network_mode/network_mode.h"

#include "base/log.h"
#include "guest.h"

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

}  // namespace network_mode
