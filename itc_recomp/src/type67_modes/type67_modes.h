// The type 6 and 7 game modes natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/type6-type7-modes.md, the layouts in
// docs/known-data.md: "type 6 screens", "type 7 screens", "type 6 and 7
// modes"): the front-end hubs over the player's car on the stand, their
// screens' state machines and handlers. Plain C++ over guest memory (the
// modes, their screen blocks and machines are shared with the recompiled
// code), with the originals' loads and stores in their order. Where an
// original builds values in its stack frame (the transitions' functors and
// keys), the stores are replayed in its frame at `frame` (its r1 after its
// stwu), so stack words it never writes reach the same places; its back
// chain and saved registers aren't written. Adapters:
// src/hooks/type67_modes.cpp (behind --native_type67_modes), register steps
// shared with the harness in src/hooks/type67_modes.h.

#pragma once

#include <cstdint>
#include <optional>

namespace type67_modes {

// --- The state machine Inits (inits.cpp): each adds its machine's
// transitions through StateMachine::Add's plain C++, as the original builds
// them in its frame (the step tables in init_tables.h). `object` is the
// original's r3; each returns the last Add's answer (`replaced`: the index,
// else the new transition) and `machine` the machine it was added to. ---

struct InitResult {
  uint32_t r3;
  uint32_t machine;
  bool replaced;
};

// An Init: its address, name, frame (its stwu size) and the registers
// __savegprlr_<n> saves from (r<n> to r31 and lr: (32 - n) * 8 + 8 bytes
// below the caller's r1).
struct InitInfo {
  uint32_t address;
  const char* name;
  uint32_t frame;
  uint32_t first_saved;
};

// sub_82235138 Type6Screen0::Init (r3 the screen, its machine at +0),
// sub_822454B8 Type7Screen0::Init (machine at +4), sub_822329A8
// Type6Dialog::Init, sub_82242420 Type7Dialog::Init, sub_821C9918
// Type6ControllerOptions::Init, sub_821C9430 Type7ControllerOptions::Init,
// sub_821CB6E8 Type6Options::Init, sub_821CB090 Type7Options::Init,
// sub_821A8210 Type6SaveScreen::Init (all on r3), and the screens' slot
// InitMachines (machine at +4 but the node screen's +460): sub_8223F9A8
// Type6ListScreen, sub_822436A0 Type7ListScreen, sub_82230F80
// Type6PanelScreen, sub_82240998 Type7PanelScreen, sub_82237320
// Type6NodeScreen, sub_82237B78 PowerGraphScreen6, sub_822470A8
// PowerGraphScreen7.
constexpr int kInitCount = 16;
extern const InitInfo kInits[kInitCount];
// The Init at `address` (one of kInits) on `object`, its frame at `frame`.
InitResult RunInit(uint8_t* base, uint32_t address, uint32_t object, uint32_t frame);

// --- The handlers and helpers without guest calls (handlers.cpp). Each
// returns r3 as the original leaves it (`r3` the one it was called with,
// for those that leave it). Type 6's and type 7's twins differ by where
// their screen's machine sits: `d` is 0 for type 6's (the machine at the
// screen's +0) and 4 for type 7's (at +4), the screen's fields shifted by
// it. The ones that allocate (VirtualAlloc(0, size, MEM_COMMIT,
// read-write)) take the allocation layer's pages (memory::AllocPages; the
// adapters run them only over the layer, hooks::NativeHeapActive); the ones
// that free (VirtualFree(block, 0, MEM_RELEASE)) return nullopt, with
// nothing done, where a block isn't the layer's pages, and the adapters
// run the originals. ---

// The mode type (0x826032DC) and the controller pages' two blocks.
constexpr uint32_t kModeType = 0x826032DC;
constexpr uint32_t kControllerData = 0x826D8490;  // 68 bytes
constexpr uint32_t kControllerPage = 0x826D848C;  // 28 bytes (the page object)

// sub_82178DE8 GarageStage::SetPlates: [holder +0] +6092 = value.
uint64_t GarageStageSetPlates(uint8_t* base, uint64_t r3, uint32_t value);
// sub_8217A2D8 GarageStage::ResetPose: the pose's +0 = 0.0 (0x82000A34),
// +16 to +63 three vectors (0, 0, 0, 1.0 (0x82000A30)) twice and (0,
// [0x82009EAC], 0, 1.0), built below `sp` first as the original's leaf
// does; +64 = 0.
uint64_t GarageStageResetPose(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8217BE08 CameraPreset::CameraPreset: +4 to +12 = 0.0, +16 to +63 the
// vectors (0, 0, 0, 1.0) three times, built below `sp` first; +0 left.
uint64_t CameraPresetConstruct(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8217D7D0 CameraPreset::Copy: a 64-byte preset allocated and
// constructed (its frame 96 below `sp`), +4 to +60 the source's through
// float registers. r3 the copy.
uint64_t CameraPresetCopy(uint8_t* base, uint32_t source, uint32_t sp);
// sub_8217D760 GarageCamera::FreePresets: the presets list (+296, its count
// at +4392) popped, each preset freed.
std::optional<uint64_t> GarageCameraFreePresets(uint8_t* base, uint64_t r3);
// sub_8217FBE8 GarageCamera::OnPresetsLoaded: the archive's callback (r4
// the loaded data: +4 the count, +8 the records' offset, 64 bytes each; r6
// the camera): each record copied (CameraPreset::Copy, its frame 128 below
// `sp`) onto the presets list, the camera's +0 = 1, the data freed.
std::optional<uint64_t> GarageCameraOnPresetsLoaded(uint8_t* base, uint32_t data, uint32_t camera,
                                                    uint32_t sp);
// sub_821800B0 StandSlot::Free: when the slot's block (+0) is set, its
// camera's (+112) presets freed, the block freed, +0 = 0.
std::optional<uint64_t> StandSlotFree(uint8_t* base, uint64_t r3);

// sub_821BB120 ControllerOptions::OnEnter: +9608 = 1, the data (68 bytes)
// and the page (28) allocated into their globals and set up. r3 the page.
uint64_t ControllerOptionsOnEnter(uint8_t* base, uint32_t machine);
// sub_821BB1F8 ControllerOptions::OnLeft: both blocks freed, the globals 0.
std::optional<uint64_t> ControllerOptionsOnLeft(uint8_t* base);
// sub_821BB260 ControllerOptions::Show: the page's +8 = +0 unless it's -1,
// then +0, +12 and +16 = -1.
uint64_t ControllerOptionsShow(uint8_t* base, uint64_t r3);
// sub_821BC660 ControllerOptions::Open: +9604 = 4.
uint64_t ControllerOptionsOpen(uint8_t* base, uint64_t r3);
// sub_821BFB48 ControllerOptions::Leave: +9604 = 7; then by the mode type,
// message 4 (types 2, 4, 6, 8) or 3 (type 7) queued in the dispatcher's
// ring (`dispatcher` +204).
uint64_t ControllerOptionsLeave(uint8_t* base, uint64_t r3, uint32_t dispatcher);

// sub_82230740 PanelScreen::InitWords: +11672 = 4, +11680 = 9.
uint64_t PanelScreenInitWords(uint8_t* base, uint64_t r3);
// sub_82277068 ScreenMachine::Wait: +9604 = 9. sub_8223B740 Screen::Reenter:
// +9608 = 4 when +9612 is set, else 1.
uint64_t ScreenMachineWait(uint8_t* base, uint64_t r3);
uint64_t ScreenReenter(uint8_t* base, uint64_t r3);

// sub_82231F48 Type6Dialog::Shown, sub_822419C0 Type7Dialog::Shown: +`shown`
// (9756, 9760) = 0.
uint64_t DialogShown(uint8_t* base, uint64_t r3, uint32_t shown);
// sub_82231F98 Type6Dialog::OnEnter, sub_82241A08 Type7Dialog::OnEnter: the
// dialog's fields reset, then state 1 and +`shown` -1 when +9612 is set,
// else state 9 and 3.
uint64_t DialogOnEnter(uint8_t* base, uint64_t r3, uint32_t shown);
// sub_82232910 Type6Dialog::WaitForMovies, sub_82242388 Type7's: when +9608
// is 0, it = 1 once movies 16 to 19 are loaded; else +9736 = 1, state 9,
// +`shown` = 3.
uint64_t DialogWaitForMovies(uint8_t* base, uint64_t r3, uint32_t shown);

// sub_822339F0 Type6Screen0::OnEnter (d 0, 4 items), sub_82243F58
// Type7Screen0::OnEnter (d 4, 3 items): the menu (76 bytes), the page (28)
// and the list (68) allocated at +9640+d, +9644+d, +9648+d and set up,
// then state 1 or 9 by +9612+d. r3 the list.
uint64_t Screen0OnEnter(uint8_t* base, uint32_t screen, uint32_t d, uint32_t items);
// sub_82233870 Type6Screen0::OnLeft, sub_82243E08 Type7's: the three blocks
// freed and their words 0.
std::optional<uint64_t> Screen0OnLeft(uint8_t* base, uint32_t screen, uint32_t d);
// sub_82233B18 Type6Screen0::WaitForMovie, sub_82244080 Type7's: when
// +9608+d is 0, it = 1 once movie 22 is loaded; else state 9.
uint64_t Screen0WaitForMovie(uint8_t* base, uint64_t r3, uint32_t d);
// sub_82233990 Type6Screen0::PickYes (`menu` 9640, 1), sub_822339A0 PickNo
// (9640, 0), sub_82243F28 Type7Screen0::PickYes (9644, 1): the menu's +72.
uint64_t Screen0Pick(uint8_t* base, uint64_t r3, uint32_t menu, uint32_t pick);
// sub_822339C0 Type6Screen0::GoOn: the page (+9644)'s +8 = +0 unless it's
// 3; +12 = 3, +16 = -1, +4 = 3, +0 = -1.
uint64_t Type6Screen0GoOn(uint8_t* base, uint64_t r3);

}  // namespace type67_modes
