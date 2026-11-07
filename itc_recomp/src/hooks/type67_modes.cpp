// The type 6 and 7 modes natively: adapters in front of src/type67_modes/
// (docs/functions/type6-type7-modes.md), with the register steps the
// harness shares (hooks/type67_modes.h). The state machine Inits are reached
// from the modes' slot 0 (recompiled) and through the screens' vtables, so
// their adapters stay.

#include "hooks/type67_modes.h"

#include "base/flags.h"
#include "hooks/archive.h"
#include "hooks/heap.h"
#include "hooks/ui_movies_game.h"
#include "ui_movies/game.h"
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
              "over the allocation layer, the originals where a block isn't its pages; the "
              "modes' Type6Mode::NextMode sub_822363B8, Type7Mode::NextMode sub_82246800, "
              "Type6Mode::Leaving sub_82236100 and Type7Mode::Leaving sub_82246590, "
              "GameMessages::Dispatch a guest call to its override; the handlers with guest "
              "calls Dialog::OpenMovies sub_82225810, Dialog::Leave sub_82225C70, "
              "Type7Dialog::Leave sub_82241E58, Type6Screen0::Leave sub_82233DF8, "
              "Type7Screen0::Leave sub_82244350 (UiMovies::Open and Delete natively where their "
              "adapters would), ListScreen::Init sub_822515A0, PanelScreen::Init sub_82248E10, "
              "NodeScreen::Init sub_82259128 (their vtable slots' plain C++ where ours), the "
              "Draw handlers Type6Screen0::Draw sub_82234ED8, Type6Dialog::Draw sub_82232740, "
              "Type7Dialog::Draw sub_822421C0, Type7Screen0::Draw sub_82245148 and "
              "ControllerOptions::Draw sub_821C7F28 (the pages' handlers guest calls); "
              "GarageCamera::LoadPresets sub_821802D8 (Archive::Load through its plain C++) and "
              "StandScene::Release sub_82180738; Quaternion::FromAngles sub_8217C120 and "
              "GarageCamera::Reset sub_8217D888 and GarageLights::Init sub_8217D3C8 with the "
              "translated code's float steps; StandSlot::Init sub_82180AA0 with "
              "GarageStage::Init sub_82180968 and GarageCamera::Init sub_82180918; the dialogs' "
              "Type6Dialog::OpenConfirm sub_822320D8, Type7Dialog::OpenConfirm sub_82241B40, "
              "Dialog::DrawWaiting sub_82241C18, Dialog::OpenConfirmStep sub_822325E0, "
              "CloseConfirmStep sub_8223AA10 and MoveCursorStep sub_8223AB50, UiMovie::Run, "
              "FinishAll and FinishNamed guest calls) "
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

// --- The guest calls and the modes' delegate functions ---

t67::Calls Type67Calls(PPCContext& ctx, uint8_t* base) {
  t67::Calls calls;
  calls.call = [&ctx, base](uint32_t target, uint32_t sp, uint32_t lr, uint64_t r3,
                            uint64_t r4) -> uint64_t {
    const uint64_t stack = ctx.r1.u64, link = ctx.lr;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = r3;
    ctx.r4.u64 = r4;
    REX_CALL_INDIRECT_FUNC(target);
    ctx.r1.u64 = stack;
    ctx.lr = link;
    return ctx.r3.u64;
  };
  calls.open = [&ctx, base](uint32_t id, uint32_t asset, uint32_t sp, uint32_t lr) -> uint64_t {
    if (ui_movies_game_hooks::OpenNative()) {
      return ui_movies::OpenMovie(base, id, asset, sp - 128, hooks::NativeArchiveLoad(base),
                                  ui_movies_game_hooks::OpenCalls(ctx, base, sp));
    }
    const uint64_t stack = ctx.r1.u64, link = ctx.lr;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = id;
    ctx.r4.u64 = asset;
    sub_8211E808(ctx, base);
    ctx.r1.u64 = stack;
    ctx.lr = link;
    return ctx.r3.u64;
  };
  calls.remove = [&ctx, base](uint32_t id, uint32_t sp, uint32_t lr) -> uint64_t {
    if (ui_movies_game_hooks::DeleteNative()) {
      return ui_movies::DeleteMovie(base, id, sp - 128,
                                    ui_movies_game_hooks::DeleteCalls(ctx, base, sp));
    }
    const uint64_t stack = ctx.r1.u64, link = ctx.lr;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = id;
    sub_8211E258(ctx, base);
    ctx.r1.u64 = stack;
    ctx.lr = link;
    return ctx.r3.u64;
  };
  calls.call_set = [&ctx, base](uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                                uint32_t lr) -> uint64_t {
    const uint64_t stack = ctx.r1.u64, link = ctx.lr;
    ctx.r1.u64 = r1;
    PPCRegister* registers[4] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6};
    for (int i = 0; i < set; i++) {
      registers[i]->u64 = args[i];
    }
    ctx.lr = lr;
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.lr = link;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  };
  return calls;
}

namespace {

t67::Calls GuestCalls(PPCContext& ctx, uint8_t* base) { return Type67Calls(ctx, base); }

void NextMode(PPCContext& ctx, uint8_t* base, const t67::ModeLayout& layout) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t answer = ctx.r3.u32, mode = ctx.r4.u32;
  t67::ModeNextMode(base, layout, answer, mode, ctx.r1.u32 - t67::kNextModeFrame,
                    GuestCalls(ctx, base));
  ctx.r3.u64 = r3;
}

void Leaving(PPCContext& ctx, uint8_t* base, const t67::ModeLayout& layout) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t answer = ctx.r3.u32, mode = ctx.r4.u32;
  t67::ModeLeaving(base, layout, answer, mode, ctx.r1.u32 - t67::kLeavingFrame,
                   GuestCalls(ctx, base));
  ctx.r3.u64 = r3;
}

}  // namespace

void Type6ModeNextMode(PPCContext& ctx, uint8_t* base) { NextMode(ctx, base, t67::kType6Layout); }
void Type7ModeNextMode(PPCContext& ctx, uint8_t* base) { NextMode(ctx, base, t67::kType7Layout); }
void Type6ModeLeaving(PPCContext& ctx, uint8_t* base) { Leaving(ctx, base, t67::kType6Layout); }
void Type7ModeLeaving(PPCContext& ctx, uint8_t* base) { Leaving(ctx, base, t67::kType7Layout); }

// --- The handlers with guest calls ---

void DialogOpenMovies(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogOpenMovies(base, ctx.r1.u32, Type67Calls(ctx, base));
}
void Type6DialogLeave(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogLeave(base, ctx.r3.u32, ctx.r4.u32, false, ctx.r1.u32,
                                Type67Calls(ctx, base));
}
void Type7DialogLeave(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogLeave(base, ctx.r3.u32, ctx.r4.u32, true, ctx.r1.u32,
                                Type67Calls(ctx, base));
}
void Type6Screen0Leave(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::Screen0Leave(base, ctx.r3.u32, ctx.r4.u32, false, ctx.r1.u32,
                                 Type67Calls(ctx, base));
}
void Type7Screen0Leave(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::Screen0Leave(base, ctx.r3.u32, ctx.r4.u32, true, ctx.r1.u32,
                                 Type67Calls(ctx, base));
}
void ListScreenInit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::ListScreenInit(base, ctx.r3.u64, ctx.r1.u32, Type67Calls(ctx, base));
}
void PanelScreenInit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::PanelScreenInit(base, ctx.r3.u64, ctx.r1.u32, Type67Calls(ctx, base));
}
void NodeScreenInit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::NodeScreenInit(base, ctx.r3.u64, ctx.r1.u32, Type67Calls(ctx, base));
}
void Type6Screen0Draw(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::Type6Screen0Draw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32,
                                     uint32_t(ctx.lr), Type67Calls(ctx, base));
}
void Type6DialogDraw(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogDraw(base, ctx.r3.u64, ctx.r4.u64, false, ctx.r1.u32,
                               Type67Calls(ctx, base));
}
void Type7DialogDraw(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogDraw(base, ctx.r3.u64, ctx.r4.u64, true, ctx.r1.u32,
                               Type67Calls(ctx, base));
}
void Type7Screen0Draw(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::Type7Screen0Draw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32,
                                     Type67Calls(ctx, base));
}
void ControllerOptionsDraw(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::ControllerOptionsDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32,
                                          Type67Calls(ctx, base));
}

// --- The stand scene's teardown and the presets' load ---

void GarageCameraLoadPresets(PPCContext& ctx, uint8_t* base) {
  SetArchiveSerial(ctx, t67::GarageCameraLoadPresets(base, ctx.r3.u32, NativeArchiveLoad(base)));
}
bool StandSceneRelease(PPCContext& ctx, uint8_t* base) {
  return Set(ctx, t67::StandSceneRelease(base));
}

// --- The dialogs' confirm ---

void Type6DialogOpenConfirm(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogOpenConfirm(base, ctx.r3.u32, false, ctx.r1.u32);
}
void Type7DialogOpenConfirm(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogOpenConfirm(base, ctx.r3.u32, true, ctx.r1.u32);
}
void DialogDrawWaiting(PPCContext& ctx, uint8_t* base) {
  if (const std::optional<uint64_t> r3 = t67::DialogDrawWaiting(base, ctx.r3.u32, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
  }
}
void DialogOpenConfirmStep(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogOpenConfirmStep(base, ctx.r3.u32, ctx.r1.u32, Type67Calls(ctx, base));
}
void DialogCloseConfirmStep(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogCloseConfirmStep(base, ctx.r3.u32, ctx.r1.u32, Type67Calls(ctx, base));
}
void DialogMoveCursorStep(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t67::DialogMoveCursorStep(base, ctx.r3.u32, ctx.r1.u32, Type67Calls(ctx, base));
}

// --- The garage camera's reset ---

void QuaternionFromAngles(PPCContext& ctx, uint8_t* base) {
  t67::QuaternionFromAngles(base, ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}
void GarageCameraReset(PPCContext& ctx, uint8_t* base) {
  const uint64_t camera = ctx.r3.u64;
  t67::GarageCameraReset(base, ctx.r3.u32, ctx.r1.u32 - t67::kGarageCameraResetFrame);
  ctx.r3.u64 = camera + 192;
  ctx.fpscr.disableFlushMode();
}
void GarageLightsInit(PPCContext& ctx, uint8_t* base) {
  const uint32_t frame = ctx.r1.u32 - t67::kGarageLightsFrame;
  t67::GarageLightsInit(base, ctx.r3.u32, frame);
  ctx.r3.u64 = ((ctx.r1.u64 & 0xFFFFFFFF00000000ull) | frame) + 96;
  ctx.fpscr.disableFlushMode();
}

namespace {

// r3 as GarageLights::Init leaves it, its frame below GarageStage::Init's
// at `stage_frame`.
uint64_t LightsR3(const PPCContext& ctx, uint32_t stage_frame) {
  return ((ctx.r1.u64 & 0xFFFFFFFF00000000ull) | (stage_frame - t67::kGarageLightsFrame)) + 96;
}

}  // namespace

void GarageCameraInit(PPCContext& ctx, uint8_t* base) {
  SetArchiveSerial(ctx, t67::GarageCameraInit(base, ctx.r3.u32, ctx.r1.u32, NativeArchiveLoad(base)));
  ctx.fpscr.disableFlushMode();
}
void GarageStageInit(PPCContext& ctx, uint8_t* base) {
  t67::GarageStageInit(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, NativeArchiveLoad(base));
  ctx.r3.u64 = LightsR3(ctx, ctx.r1.u32 - t67::kGarageStageInitFrame);
  ctx.fpscr.disableFlushMode();
}
void StandSlotInit(PPCContext& ctx, uint8_t* base) {
  if (t67::StandSlotInit(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, NativeArchiveLoad(base))) {
    ctx.r3.u64 = LightsR3(ctx, ctx.r1.u32 - t67::kStandSlotInitFrame - t67::kGarageStageInitFrame);
  }
  ctx.fpscr.disableFlushMode();
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
// OnPresetsLoaded), GarageCamera::FreePresets sub_8217D760 (from
// StandSlot::Free) and StandSlot::Free sub_821800B0 (from
// StandScene::Release).
ITC_TYPE67_MODES(82178DE8, GarageStageSetPlates)
ITC_TYPE67_MODES(8217A2D8, GarageStageResetPose)
ITC_TYPE67_FREES(8217FBE8, GarageCameraOnPresetsLoaded)
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

// The modes' delegate functions (held by the delegate, no direct callers).
ITC_TYPE67_MODES(822363B8, Type6ModeNextMode)
ITC_TYPE67_MODES(82246800, Type7ModeNextMode)
ITC_TYPE67_MODES(82236100, Type6ModeLeaving)
ITC_TYPE67_MODES(82246590, Type7ModeLeaving)

// The handlers with guest calls: reached through their functors, the
// screens' vtables and the modes' slot 0 (recompiled).
ITC_TYPE67_MODES(82225810, DialogOpenMovies)
ITC_TYPE67_MODES(82225C70, Type6DialogLeave)
ITC_TYPE67_MODES(82241E58, Type7DialogLeave)
ITC_TYPE67_MODES(82233DF8, Type6Screen0Leave)
ITC_TYPE67_MODES(82244350, Type7Screen0Leave)
ITC_TYPE67_MODES(822515A0, ListScreenInit)
ITC_TYPE67_MODES(82248E10, PanelScreenInit)
ITC_TYPE67_MODES(82259128, NodeScreenInit)
ITC_TYPE67_MODES(82234ED8, Type6Screen0Draw)
ITC_TYPE67_MODES(82232740, Type6DialogDraw)
ITC_TYPE67_MODES(822421C0, Type7DialogDraw)
ITC_TYPE67_MODES(82245148, Type7Screen0Draw)
ITC_TYPE67_MODES(821C7F28, ControllerOptionsDraw)

// GarageCamera::LoadPresets (from GarageCamera::Init, recompiled): over the
// layer and the archive's native queue only.
extern "C" REX_FUNC(sub_821802D8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_type67_modes) || !hooks::NativeHeapActive() ||
      !hooks::NativeArchiveQueue()) {
    __imp__sub_821802D8(ctx, base);
    return;
  }
  hooks::GarageCameraLoadPresets(ctx, base);
}
// StandScene::Release (from StandMode::Release, recompiled).
ITC_TYPE67_FREES(82180738, StandSceneRelease)

// The dialogs' confirm: reached through their machines' transitions and
// the Update's step table (no direct callers).
ITC_TYPE67_MODES(822320D8, Type6DialogOpenConfirm)
ITC_TYPE67_MODES(82241B40, Type7DialogOpenConfirm)
ITC_TYPE67_MODES(82241C18, DialogDrawWaiting)
ITC_TYPE67_MODES(822325E0, DialogOpenConfirmStep)
ITC_TYPE67_MODES(8223AA10, DialogCloseConfirmStep)
ITC_TYPE67_MODES(8223AB50, DialogMoveCursorStep)

// The garage camera: Quaternion::FromAngles (from GarageCamera::Command,
// recompiled) and GarageCamera::Reset (from GarageCamera::Init and Command).
ITC_TYPE67_MODES(8217C120, QuaternionFromAngles)
ITC_TYPE67_MODES(8217D888, GarageCameraReset)
// No adapter for GarageLights::Init sub_8217D3C8: only GarageStage::Init
// (native) calls it; its step above is the harness's.
// StandSlot::Init (from StandScene::Create, recompiled): over the layer and
// the archive's native queue only. GarageStage::Init sub_82180968 and
// GarageCamera::Init sub_82180918 have no adapters (only StandSlot::Init
// and GarageStage::Init reach them; the flag switches those).
extern "C" REX_FUNC(sub_82180AA0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_type67_modes) || !hooks::NativeHeapActive() ||
      !hooks::NativeArchiveQueue()) {
    __imp__sub_82180AA0(ctx, base);
    return;
  }
  hooks::StandSlotInit(ctx, base);
}
