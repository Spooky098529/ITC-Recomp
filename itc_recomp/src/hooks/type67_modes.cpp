// The type 6 and 7 modes natively: adapters in front of src/type67_modes/
// (docs/functions/type6-type7-modes.md), with the register steps the
// harness shares (hooks/type67_modes.h). The state machine Inits are reached
// from the modes' slot 0 (recompiled) and through the screens' vtables, so
// their adapters stay.

#include "hooks/type67_modes.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "type67_modes/type67_modes.h"

ITC_FLAG_BOOL(native_type67_modes, true, "ITC",
              "Run the type 6 and 7 modes (docs/functions/type6-type7-modes.md: the state "
              "machine Inits Type6Screen0::Init sub_82235138, Type7Screen0::Init sub_822454B8, "
              "Type6Dialog::Init sub_822329A8, Type7Dialog::Init sub_82242420, "
              "Type6ControllerOptions::Init sub_821C9918, Type7ControllerOptions::Init "
              "sub_821C9430, Type6Options::Init sub_821CB6E8, Type7Options::Init sub_821CB090, "
              "Type6SaveScreen::Init sub_821A8210, Type6ListScreen::InitMachine sub_8223F9A8, "
              "Type7ListScreen::InitMachine sub_822436A0, Type6PanelScreen::InitMachine "
              "sub_82230F80, Type7PanelScreen::InitMachine sub_82240998, "
              "Type6NodeScreen::InitMachine sub_82237320, PowerGraphScreen6::InitMachine "
              "sub_82237B78 and PowerGraphScreen7::InitMachine sub_822470A8, over "
              "StateMachine::Add's plain C++, their frames' stores replayed; the handlers and "
              "helpers without guest calls: GarageStage::SetPlates sub_82178DE8, "
              "GarageStage::ResetPose sub_8217A2D8, CameraPreset::CameraPreset sub_8217BE08, "
              "CameraPreset::Copy sub_8217D7D0, GarageCamera::FreePresets sub_8217D760, "
              "GarageCamera::OnPresetsLoaded sub_8217FBE8, StandSlot::Free sub_821800B0, "
              "ControllerOptions::OnEnter sub_821BB120, OnLeft sub_821BB1F8, Show sub_821BB260, "
              "Open sub_821BC660 and Leave sub_821BFB48, PanelScreen::InitWords sub_82230740, "
              "ScreenMachine::Wait sub_82277068, Screen::Reenter sub_8223B740, the dialogs' "
              "Shown sub_82231F48 and sub_822419C0, OnEnter sub_82231F98 and sub_82241A08 and "
              "WaitForMovies sub_82232910 and sub_82242388, the first screens' OnEnter "
              "sub_822339F0 and sub_82243F58, OnLeft sub_82233870 and sub_82243E08, "
              "WaitForMovie sub_82233B18 and sub_82244080, PickYes sub_82233990 and "
              "sub_82243F28, PickNo sub_822339A0 and Type6Screen0::GoOn sub_822339C0; blocks "
              "over the allocation layer, the originals where a block isn't its pages) "
              "natively instead of "
              "through the translated code (the same writes; checked by "
              "--run_harnesses=type67_modes)");

namespace hooks {

namespace t67 = type67_modes;

bool Type67ModesNative() {
  return ITC_FLAG(native_type67_modes);
}

void Type67Init(PPCContext& ctx, uint8_t* base, uint32_t address) {
  uint32_t frame_bytes = 0;
  for (const t67::InitInfo& info : t67::kInits) {
    if (info.address == address) {
      frame_bytes = info.frame;
    }
  }
  const uint64_t object_r3 = ctx.r3.u64;
  const t67::InitResult result =
      t67::RunInit(base, address, ctx.r3.u32, ctx.r1.u32 - frame_bytes);
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  const uint64_t machine_r3 = object_r3 + (result.machine - ctx.r3.u32);
  ctx.r3.u64 = result.replaced ? result.r3 : machine_r3 + (result.r3 - result.machine);
}

// --- The handlers without guest calls ---

void GarageStageSetPlates(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::GarageStageSetPlates(base, ctx.r3.u64, ctx.r4.u32);
}
void GarageStageResetPose(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::GarageStageResetPose(base, ctx.r3.u64, ctx.r1.u32);
}
void CameraPresetConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::CameraPresetConstruct(base, ctx.r3.u64, ctx.r1.u32);
}
void CameraPresetCopy(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::CameraPresetCopy(base, ctx.r3.u32, ctx.r1.u32);
}

namespace {

bool Set(PPCContext& ctx, const std::optional<uint64_t>& r3) {
  if (!r3) {
    return false;
  }
  ctx.r3.u64 = *r3;
  return true;
}

}  // namespace

bool GarageCameraFreePresets(PPCContext& ctx, uint8_t* base) {
  return Set(ctx, t67::GarageCameraFreePresets(base, ctx.r3.u64));
}
bool GarageCameraOnPresetsLoaded(PPCContext& ctx, uint8_t* base) {
  return Set(ctx, t67::GarageCameraOnPresetsLoaded(base, ctx.r4.u32, ctx.r6.u32, ctx.r1.u32));
}
bool StandSlotFree(PPCContext& ctx, uint8_t* base) {
  return Set(ctx, t67::StandSlotFree(base, ctx.r3.u64));
}
void ControllerOptionsOnEnter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::ControllerOptionsOnEnter(base, ctx.r3.u32);
}
bool ControllerOptionsOnLeft(PPCContext& ctx, uint8_t* base) {
  return Set(ctx, t67::ControllerOptionsOnLeft(base));
}
void ControllerOptionsShow(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::ControllerOptionsShow(base, ctx.r3.u64);
}
void ControllerOptionsOpen(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::ControllerOptionsOpen(base, ctx.r3.u64);
}
void ControllerOptionsLeave(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::ControllerOptionsLeave(base, ctx.r3.u64, ctx.r4.u32);
}
void PanelScreenInitWords(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::PanelScreenInitWords(base, ctx.r3.u64);
}
void ScreenMachineWait(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::ScreenMachineWait(base, ctx.r3.u64);
}
void ScreenReenter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::ScreenReenter(base, ctx.r3.u64);
}
void Type6DialogShown(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogShown(base, ctx.r3.u64, 9756);
}
void Type7DialogShown(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogShown(base, ctx.r3.u64, 9760);
}
void Type6DialogOnEnter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogOnEnter(base, ctx.r3.u64, 9756);
}
void Type7DialogOnEnter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogOnEnter(base, ctx.r3.u64, 9760);
}
void Type6DialogWaitForMovies(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogWaitForMovies(base, ctx.r3.u64, 9756);
}
void Type7DialogWaitForMovies(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogWaitForMovies(base, ctx.r3.u64, 9760);
}
void Type6Screen0OnEnter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::Screen0OnEnter(base, ctx.r3.u32, 0, 4);
}
void Type7Screen0OnEnter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::Screen0OnEnter(base, ctx.r3.u32, 4, 3);
}
bool Type6Screen0OnLeft(PPCContext& ctx, uint8_t* base) {
  return Set(ctx, t67::Screen0OnLeft(base, ctx.r3.u32, 0));
}
bool Type7Screen0OnLeft(PPCContext& ctx, uint8_t* base) {
  return Set(ctx, t67::Screen0OnLeft(base, ctx.r3.u32, 4));
}
void Type6Screen0WaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::Screen0WaitForMovie(base, ctx.r3.u64, 0);
}
void Type7Screen0WaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::Screen0WaitForMovie(base, ctx.r3.u64, 4);
}
void Type6Screen0PickYes(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::Screen0Pick(base, ctx.r3.u64, 9640, 1);
}
void Type6Screen0PickNo(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::Screen0Pick(base, ctx.r3.u64, 9640, 0);
}
void Type7Screen0PickYes(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::Screen0Pick(base, ctx.r3.u64, 9644, 1);
}
void Type6Screen0GoOn(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::Type6Screen0GoOn(base, ctx.r3.u64);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_TYPE67_MODES(address, step, ...)  \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_type67_modes)) {     \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    hooks::step(ctx, base, ##__VA_ARGS__);    \
  }

ITC_TYPE67_MODES(82235138, Type67Init, 0x82235138)
ITC_TYPE67_MODES(822454B8, Type67Init, 0x822454B8)
ITC_TYPE67_MODES(822329A8, Type67Init, 0x822329A8)
ITC_TYPE67_MODES(82242420, Type67Init, 0x82242420)
ITC_TYPE67_MODES(821C9918, Type67Init, 0x821C9918)
ITC_TYPE67_MODES(821C9430, Type67Init, 0x821C9430)
ITC_TYPE67_MODES(821CB6E8, Type67Init, 0x821CB6E8)
ITC_TYPE67_MODES(821CB090, Type67Init, 0x821CB090)
ITC_TYPE67_MODES(821A8210, Type67Init, 0x821A8210)
ITC_TYPE67_MODES(8223F9A8, Type67Init, 0x8223F9A8)
ITC_TYPE67_MODES(822436A0, Type67Init, 0x822436A0)
ITC_TYPE67_MODES(82230F80, Type67Init, 0x82230F80)
ITC_TYPE67_MODES(82240998, Type67Init, 0x82240998)
ITC_TYPE67_MODES(82237320, Type67Init, 0x82237320)
ITC_TYPE67_MODES(82237B78, Type67Init, 0x82237B78)
ITC_TYPE67_MODES(822470A8, Type67Init, 0x822470A8)

// An adapter whose step allocates: the original off the allocation layer.
#define ITC_TYPE67_ALLOCATES(address, step)                         \
  extern "C" REX_FUNC(sub_##address) {                              \
    REX_FUNC_PROLOGUE();                                            \
    if (!ITC_FLAG(native_type67_modes) || !hooks::NativeHeapActive()) { \
      __imp__sub_##address(ctx, base);                              \
      return;                                                       \
    }                                                               \
    hooks::step(ctx, base);                                         \
  }

// An adapter whose step frees: the original where it refuses (a block not
// the layer's pages).
#define ITC_TYPE67_FREES(address, step)                                     \
  extern "C" REX_FUNC(sub_##address) {                                      \
    REX_FUNC_PROLOGUE();                                                    \
    if (!ITC_FLAG(native_type67_modes) || !hooks::NativeHeapActive() ||     \
        !hooks::step(ctx, base)) {                                          \
      __imp__sub_##address(ctx, base);                                      \
    }                                                                       \
  }

// The handlers are reached through their functors (no direct callers) and
// the helpers from recompiled callers, so their adapters stay. No adapters
// (only native callers reach them in play, and the flag switches those;
// their steps above are the harness's): CameraPreset::CameraPreset
// sub_8217BE08 (from Copy), CameraPreset::Copy sub_8217D7D0 (from
// OnPresetsLoaded) and GarageCamera::FreePresets sub_8217D760 (from
// StandSlot::Free).
ITC_TYPE67_MODES(82178DE8, GarageStageSetPlates)
ITC_TYPE67_MODES(8217A2D8, GarageStageResetPose)
ITC_TYPE67_FREES(8217FBE8, GarageCameraOnPresetsLoaded)
ITC_TYPE67_FREES(821800B0, StandSlotFree)
ITC_TYPE67_ALLOCATES(821BB120, ControllerOptionsOnEnter)
ITC_TYPE67_FREES(821BB1F8, ControllerOptionsOnLeft)
ITC_TYPE67_MODES(821BB260, ControllerOptionsShow)
ITC_TYPE67_MODES(821BC660, ControllerOptionsOpen)
ITC_TYPE67_MODES(821BFB48, ControllerOptionsLeave)
ITC_TYPE67_MODES(82230740, PanelScreenInitWords)
ITC_TYPE67_MODES(82277068, ScreenMachineWait)
ITC_TYPE67_MODES(8223B740, ScreenReenter)
ITC_TYPE67_MODES(82231F48, Type6DialogShown)
ITC_TYPE67_MODES(822419C0, Type7DialogShown)
ITC_TYPE67_MODES(82231F98, Type6DialogOnEnter)
ITC_TYPE67_MODES(82241A08, Type7DialogOnEnter)
ITC_TYPE67_MODES(82232910, Type6DialogWaitForMovies)
ITC_TYPE67_MODES(82242388, Type7DialogWaitForMovies)
ITC_TYPE67_ALLOCATES(822339F0, Type6Screen0OnEnter)
ITC_TYPE67_ALLOCATES(82243F58, Type7Screen0OnEnter)
ITC_TYPE67_FREES(82233870, Type6Screen0OnLeft)
ITC_TYPE67_FREES(82243E08, Type7Screen0OnLeft)
ITC_TYPE67_MODES(82233B18, Type6Screen0WaitForMovie)
ITC_TYPE67_MODES(82244080, Type7Screen0WaitForMovie)
ITC_TYPE67_MODES(82233990, Type6Screen0PickYes)
ITC_TYPE67_MODES(822339A0, Type6Screen0PickNo)
ITC_TYPE67_MODES(82243F28, Type7Screen0PickYes)
ITC_TYPE67_MODES(822339C0, Type6Screen0GoOn)
