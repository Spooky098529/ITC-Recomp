// The type 2 mode's Init (docs/functions/type2-mode.md): see type2_mode.h.
// The callees through their overrides (guest calls from the original's
// frame with its return addresses): the screen start, Strings::Load,
// Archive::WaitIdle, StandScene::Create, VirtualAlloc, the 14 machines'
// Inits, ScreenFades::FadeIn, Career::NextDay and LoadingScreen::Show;
// SaveData::Of its plain C++.

#include "type2_mode/type2_mode.h"

#include "base/log.h"
#include "guest.h"
#include "schema/race_event.h"
#include "schema/race_states.h"
#include "schema/type2_mode.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type2_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::Type2ModeGlobals;
using Machine = schema::Type2ScreenMachine;

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

constexpr uint32_t kScreenFadesFadeIn = 0x8209D038;

}  // namespace

uint64_t Init(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82197278 ModeType2::Init");
  const uint32_t frame = sp - kInitFrame;
  StoreU32(base, frame, sp);
  StoreU32(base, sp + 20, mode);
  const schema::ModeType2 m(base, mode);
  auto block = [&] { return m.block().address; };
  calls.Call(kScreenStart, frame, 0x82197290, mode);
  const uint64_t strings =
      calls.Call(kStringsLoad, frame, 0x821972A4, G::string_table(base), 0x100BB);
  calls.Call(kArchiveWaitIdle, frame, 0x821972A8, strings, 0x100BB);
  calls.Call(kStandSceneCreate, frame, 0x821972B0, 3);
  const uint64_t allocated =
      calls.Call(kVirtualAlloc, frame, 0x821972C8, 0, schema::Type2ModeBlock::kSize, 4096, 4);
  m.set_block(uint32_t(allocated));
  for (const Member& member : kMembers) {
    // The first from VirtualAlloc's 64-bit r3, the others from the block's
    // word.
    const uint64_t object = member.offset == 0 ? allocated : uint64_t(block()) + member.offset;
    calls.Call(member.init, frame, member.lr, object);
  }
  calls.Call(kScreenFadesFadeIn, frame, 0x821973E0, G::fade_in_passes(base), 0, 0, 0);
  for (const uint32_t offset : kAdded) {
    const uint32_t count = m.handler_count();
    m.set_handler_count(count + 1);
    schema::host::StoreU32(m.handlers() + (count << 2), block() + offset);  // guest order (E5 step 4)
  }
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const schema::SaveData save_data(base, save);
  int32_t day = int32_t(save_data.day());
  const int32_t garage_count = int16_t(save_data.garage_count());
  if (day > 999) {
    day = 999;
  }
  if (day == 0) {
    calls.Call(kCareerNextDay, frame, 0x82197598, save);
  }
  uint32_t before = G::mode_before(base);
  if (before != 4) {
    const int32_t again = int32_t(save_data.day());
    if (again <= 999 && again == 1) {
      calls.Call(kCareerNextDay, frame, 0x821975C4, save);
      before = G::mode_before(base);
    }
  }
  uint32_t first = 0;
  if (garage_count <= 0) {
    first = 4;
  } else if (before == 1) {
    const uint32_t race_mode = G::race_data(base).race_mode();
    if (race_mode == 0 || race_mode == 1) {
      first = 3;
    }
  }
  m.block().set_first_shown(first);
  m.block().set_shown(first);
  // The frame's words the first four machines' +9612 and +9616 are copied
  // from (+100 the spec sheet's offset, not read).
  StoreU32(base, frame + 80, 0);
  StoreU32(base, frame + 88, 0);
  StoreU32(base, frame + 96, 0);
  StoreU32(base, frame + 100, 56624);
  StoreU32(base, frame + 84, 1);
  StoreU32(base, frame + 92, 1);
  StoreU32(base, frame + 104, 1);
  // Each machine's words through a view at block() + its offset (the
  // block's word read again for each store, as the original does).
  auto machine = [&](uint32_t offset) { return Machine(base, block() + offset); };
  m.block().set_hub_reenter(1);
  m.block().set_hub_delete_movie(LoadU32(base, frame + 80));
  machine(9640).set_reenter(LoadU32(base, frame + 84));
  machine(9640).set_delete_movie(LoadU32(base, frame + 88));
  machine(19792).set_reenter(LoadU32(base, frame + 92));
  machine(19792).set_delete_movie(LoadU32(base, frame + 96));
  machine(56624).set_reenter(LoadU32(base, frame + 104));
  machine(56624).set_delete_movie(0);
  machine(0x102D8).set_delete_movie(0);
  machine(0x102D8).set_reenter(1);
  for (const uint32_t offset : kOpened) {
    machine(offset).set_reenter(1);
    machine(offset).set_delete_movie(0);
  }
  const uint64_t r3 = calls.Call(kLoadingShow, frame, 0x8219779C, 1, 0, ~uint64_t(0));
  // The delegate: WaitForLoads through Mode::Slot9Thunk.
  StoreU32(base, frame + 108, 0);
  StoreU32(base, frame + 104, kSlot9Thunk);
  const uint32_t delegate = mode + 8;  // the delegate (in host memory, E5 step 4)
  uint32_t holder = 0;
  if (delegate != 0) {
    holder = delegate;
    const uint32_t function = LoadU32(base, frame + 104), adjustment = LoadU32(base, frame + 108);
    m.set_delegate_object(mode);
    m.set_delegate_function(function);
    m.set_delegate_adjustment(adjustment);
    m.set_delegate_vtable(kDelegateVtable);
  }
  m.set_delegate_ptr(holder);
  return r3;
}

}  // namespace type2_mode
