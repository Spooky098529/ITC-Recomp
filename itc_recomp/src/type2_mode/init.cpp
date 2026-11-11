// The type 2 mode's Init (docs/functions/type2-mode.md): see type2_mode.h.
// The callees through their overrides (guest calls from the original's
// frame with its return addresses): the screen start, Strings::Load,
// Archive::WaitIdle, StandScene::Create, VirtualAlloc, the 14 machines'
// Inits, ScreenFades::FadeIn, Career::NextDay and LoadingScreen::Show;
// SaveData::Of its plain C++.

#include "type2_mode/type2_mode.h"

#include "base/log.h"
#include "guest.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type2_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;

// The machines in the order the Init sets them up: their Init, their
// offset in the block and the call's return address.
struct Member {
  uint32_t init;
  uint32_t offset;
  uint32_t lr;
};
constexpr Member kMembers[] = {
    {0x82190B18, 0, 0x821972D0},        // Type2Hub::InitStates
    {0x8218CE10, 9640, 0x821972DC},     // RaceMenu::Init
    {0x821E7568, 19792, 0x821972E8},    // TuningMachine::Init
    {0x821B90D8, 56624, 0x821972FC},    // PowerGraphScreen2::InitStates
    {0x8220B408, 0x102D8, 0x82197310},  // UserInfo::InitStates
    {0x821C9E00, 0x12880, 0x82197324},  // Type2Options::Init
    {0x821C8578, 0x14E28, 0x82197338},  // Type2ControllerOptions::Init
    {0x821B6FC0, 0x173D0, 0x8219734C},  // CarSettings::Init
    {0x82185920, 0x19978, 0x82197360},  // CarDealer::InitStates
    {0x821A16C0, 0x1BF20, 0x82197374},  // BuildScreen::InitStates
    {0x82196520, 0x23018, 0x82197388},  // GarageScreen::InitStates
    {0x821A6E18, 0x255C0, 0x8219739C},  // Type2SaveScreen::Init
    {0x82188178, 0x1E4C8, 0x821973B0},  // Type2Dialog::Init
    {0x8219B2E8, 0x20A70, 0x821973C4},  // PlateEditor::InitStates
};
// The machines added to the dispatcher, in its order.
constexpr uint32_t kAdded[] = {0,       9640,    19792,   56624,   0x102D8, 0x12880, 0x14E28,
                               0x173D0, 0x19978, 0x1BF20, 0x23018, 0x255C0, 0x1E4C8, 0x20A70};
// The machines whose +9612 = 1 and +9616 = 0 are stored directly, after the
// first four's (copied from the frame's words), in the original's order
// (the user info's +9616 first).
constexpr uint32_t kOpened[] = {0x12880, 0x14E28, 0x173D0, 0x19978, 0x1BF20,
                                0x23018, 0x1E4C8, 0x20A70, 0x255C0};

constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kFadesPasses = 0x82001BEC;
constexpr uint32_t kScreenFadesFadeIn = 0x8209D038;

}  // namespace

uint64_t Init(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82197278 ModeType2::Init");
  const uint32_t frame = sp - kInitFrame;
  StoreU32(base, frame, sp);
  StoreU32(base, sp + 20, mode);
  auto block = [&] { return LoadU32(base, mode + kBlock); };
  calls.Call(kScreenStart, frame, 0x82197290, mode);
  const uint64_t strings =
      calls.Call(kStringsLoad, frame, 0x821972A4, LoadU32(base, kStringTable), 0x100BB);
  calls.Call(kArchiveWaitIdle, frame, 0x821972A8, strings, 0x100BB);
  calls.Call(kStandSceneCreate, frame, 0x821972B0, 3);
  const uint64_t allocated = calls.Call(kVirtualAlloc, frame, 0x821972C8, 0, kBlockBytes, 4096, 4);
  StoreU32(base, mode + kBlock, uint32_t(allocated));
  for (const Member& m : kMembers) {
    // The first from VirtualAlloc's 64-bit r3, the others from the block's
    // word.
    const uint64_t object = m.offset == 0 ? allocated : uint64_t(block()) + m.offset;
    calls.Call(m.init, frame, m.lr, object);
  }
  calls.Call(kScreenFadesFadeIn, frame, 0x821973E0, LoadU32(base, kFadesPasses), 0, 0, 0);
  const uint32_t dispatcher = mode + kDispatcher;
  for (const uint32_t offset : kAdded) {
    const uint32_t count = LoadU32(base, dispatcher + 200);
    StoreU32(base, dispatcher + 200, count + 1);
    StoreU32(base, dispatcher + (count << 2), block() + offset);
  }
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  int32_t day = int32_t(LoadU32(base, save + 6100));
  const int32_t garage_count = int16_t(guest::LoadU16(base, save + 6328));
  if (day > 999) {
    day = 999;
  }
  if (day == 0) {
    calls.Call(kCareerNextDay, frame, 0x82197598, save);
  }
  uint32_t before = LoadU32(base, kModeBefore);
  if (before != 4) {
    const int32_t again = int32_t(LoadU32(base, save + 6100));
    if (again <= 999 && again == 1) {
      calls.Call(kCareerNextDay, frame, 0x821975C4, save);
      before = LoadU32(base, kModeBefore);
    }
  }
  uint32_t first = 0;
  if (garage_count <= 0) {
    first = 4;
  } else if (before == 1) {
    const uint32_t race_mode = LoadU32(base, LoadU32(base, kRaceData) + 0xA3AF4);
    if (race_mode == 0 || race_mode == 1) {
      first = 3;
    }
  }
  StoreU32(base, block() + kFirstShown, first);
  StoreU32(base, block() + kShown, first);
  // The frame's words the first four machines' +9612 and +9616 are copied
  // from (+100 the spec sheet's offset, not read).
  StoreU32(base, frame + 80, 0);
  StoreU32(base, frame + 88, 0);
  StoreU32(base, frame + 96, 0);
  StoreU32(base, frame + 100, 56624);
  StoreU32(base, frame + 84, 1);
  StoreU32(base, frame + 92, 1);
  StoreU32(base, frame + 104, 1);
  StoreU32(base, block() + 9612, 1);
  StoreU32(base, block() + 9616, LoadU32(base, frame + 80));
  StoreU32(base, block() + 19252, LoadU32(base, frame + 84));
  StoreU32(base, block() + 19256, LoadU32(base, frame + 88));
  StoreU32(base, block() + 29404, LoadU32(base, frame + 92));
  StoreU32(base, block() + 29408, LoadU32(base, frame + 96));
  StoreU32(base, block() + 56624 + 9612, LoadU32(base, frame + 104));
  StoreU32(base, block() + 56624 + 9616, 0);
  StoreU32(base, block() + 0x102D8 + 9616, 0);
  StoreU32(base, block() + 0x102D8 + 9612, 1);
  for (const uint32_t offset : kOpened) {
    StoreU32(base, block() + offset + 9612, 1);
    StoreU32(base, block() + offset + 9616, 0);
  }
  const uint64_t r3 = calls.Call(kLoadingShow, frame, 0x8219779C, 1, 0, ~uint64_t(0));
  // The delegate: WaitForLoads through Mode::Slot9Thunk.
  StoreU32(base, frame + 108, 0);
  StoreU32(base, frame + 104, kSlot9Thunk);
  const uint32_t delegate = mode + 8;
  uint32_t holder = 0;
  if (delegate != 0) {
    holder = delegate;
    const uint32_t function = LoadU32(base, frame + 104), adjustment = LoadU32(base, frame + 108);
    StoreU32(base, delegate + 4, mode);
    StoreU32(base, delegate + 8, function);
    StoreU32(base, delegate + 12, adjustment);
    StoreU32(base, delegate + 0, kDelegateVtable);
  }
  StoreU32(base, mode + 4, holder);
  return r3;
}

}  // namespace type2_mode
