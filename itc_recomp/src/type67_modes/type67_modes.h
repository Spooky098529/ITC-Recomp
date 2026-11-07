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

#include "strings_and_font/strings_and_font.h"

#include <cstdint>
#include <functional>
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

// --- The modes' delegate functions (modes.cpp) ---

// The guest calls the modes make: `target` with r3 and r4 from a frame at
// `sp` returning to `lr`; r3 after (GameMessages::Dispatch through its
// override).
struct Calls {
  std::function<uint64_t(uint32_t target, uint32_t sp, uint32_t lr, uint64_t r3, uint64_t r4)>
      call;
  // UiMovies::Open(id, asset) and UiMovies::Delete(id) called from the
  // frame at `sp` returning to `lr` (natively where their adapters would
  // run them natively, else their adapters); their r3.
  std::function<uint64_t(uint32_t id, uint32_t asset, uint32_t sp, uint32_t lr)> open;
  std::function<uint64_t(uint32_t id, uint32_t sp, uint32_t lr)> remove;
  // A guest call with the first `set` of r3 to r8 from `args` (the others
  // left; MenuSound::Play's slot call, as menu_sounds::Calls makes it, sets
  // at most 4 from an array of 4), from the frame at `r1` returning to `lr`;
  // its r3.
  std::function<uint64_t(uint32_t function, const uint64_t args[6], int set, uint32_t r1,
                         uint32_t lr)>
      call_set;
  // --native_action_mappers: Menu::Step's and Pressed's plain C++ answer
  // the mapper's read slots with ActionMapper::Pressed's and Fired's.
  bool native_action_slots = false;
};

// A machine whose state a mode checks: the block's +`test` tested for 0
// (the cast's null check), then the machine at +`machine` (state -1 for
// none).
struct MachineAt {
  uint32_t test;
  uint32_t machine;
};

// What differs between the type 6 and type 7 modes' delegate functions.
struct ModeLayout {
  uint32_t phase;      // the block's phase word (4 leaving, 5 torn down)
  uint32_t answer;     // the block's 8-byte answer
  uint32_t leave_message;
  uint32_t leaving;    // the Leaving function (the delegate's after NextMode)
  uint32_t tear_down;  // the TearDown function (after Leaving)
  uint32_t car;        // the mode's car's +1972 word (-1: made again)
  uint32_t next_lr, leaving_lr;  // the Dispatch calls' return addresses
  const MachineAt* next_checks;  // NextMode's, any in state 10
  uint32_t next_count;
  const MachineAt* leave_checks;  // Leaving's, all in state 6
  uint32_t leave_count;
  MachineAt list;  // the list screen (6 or 11 ends the mode)
};
extern const ModeLayout kType6Layout;
extern const ModeLayout kType7Layout;

constexpr uint32_t kNextModeFrame = 144;  // NextMode's stwu
constexpr uint32_t kLeavingFrame = 128;   // Leaving's
constexpr uint32_t kModeDelegateVtable = 0x82010FA0;
constexpr uint32_t kLeavingFlag = 0x826364EC;  // 0 once NextMode leaves

// sub_822363B8 Type6Mode::NextMode, sub_82246800 Type7Mode::NextMode: Draw
// and Update queued in the mode's ring, GameMessages::Dispatch(mode + 36,
// 0), the answer cleared; when a machine is in state 10, the delegate's
// function = Leaving (its member functor built at the frame's +80),
// [0x826364EC] = 0 and the phase 4. `frame` is the original's r1.
void ModeNextMode(uint8_t* base, const ModeLayout& layout, uint32_t answer, uint32_t mode,
                  uint32_t frame, const Calls& calls);
// sub_82236100 Type6Mode::Leaving, sub_82246590 Type7Mode::Leaving: the
// answer cleared, Leave queued and dispatched (r4 the mode); when every
// checked machine is done (6): with the list done, phase 5, the car's word
// -1 and the delegate's function TearDown; with the list in 11 (a race
// picked), phase 5, the block's answer = the doubleword at the frame's +80
// (byte 1, the three bytes the frame holds, word 1) and TearDown.
void ModeLeaving(uint8_t* base, const ModeLayout& layout, uint32_t answer, uint32_t mode,
                 uint32_t frame, const Calls& calls);

// sub_82235F28 Type6Mode::WaitForLoads and sub_822463D8
// Type7Mode::WaitForLoads (r3 the answer, r4 the mode): the answer
// cleared, the phase 1, LoadingScreen::Run(-1) (a guest call); when every
// member's loaded word has bit 0 and the sound manager's slot 6 says bank 1
// isn't loading: LoadingScreen::Finish(-1) and GarageStage::Draw (guest
// calls), GarageStage::SetDrawWord and Sound::Play (bank 1's cue 2) through
// their plain C++, the phase 3, the delegate NextModeThunk and the loaded
// message (34, 28) queued, else Draw (1); then GameMessages::Dispatch (a
// guest call). r3 the answer.
uint64_t ModeWaitForLoads(uint8_t* base, bool type7, uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls);

// sub_82235C18 Type6Mode::TearDown and sub_82246098 Type7Mode::TearDown
// (r3 the answer, r4 the mode): the answer cleared; in phase 5, once
// StandScene::PollTeardown (its plain C++) answers nonzero, phase 6 and bank
// 1's cues 0 and 2 stopped where playing (the sound manager's slots 14 and
// 13, guest calls, their indexes in the frame); in phase 6 without a hold
// (the phase's next word), the answer = the block's 8 bytes. Type 7 then
// queues Draw (1) without dispatching. r3 the answer.
constexpr uint32_t kTearDownFrame = 176;
uint64_t ModeTearDown(uint8_t* base, bool type7, uint64_t r3, uint32_t mode, uint32_t sp,
                      const Calls& calls);

// --- The handlers with guest calls (screens.cpp). `sp` is the caller's
// stack pointer (the original's r1 at entry); each returns r3 as the
// original leaves it. ---

// sub_82225810 Dialog::OpenMovies: UiMovies::Open of movies 16 to 19
// (assets 22559 to 22562).
uint64_t DialogOpenMovies(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82225C70 Dialog::Leave (type 6's, message 4) and sub_82241E58
// Type7Dialog::Leave (message 3): state 7, movies 16 to 19 deleted, the
// message queued in the dispatcher's ring.
uint64_t DialogLeave(uint8_t* base, uint32_t dialog, uint32_t dispatcher, bool type7,
                     uint32_t sp, const Calls& calls);
// sub_82233DF8 Type6Screen0::Leave (state 7 first, message 4) and
// sub_82244350 Type7Screen0::Leave (state 7 at +9608 after, message 3):
// movie 22 deleted, the message queued.
uint64_t Screen0Leave(uint8_t* base, uint32_t screen, uint32_t dispatcher, bool type7,
                      uint32_t sp, const Calls& calls);
// sub_822515A0 ListScreen::Init, sub_82248E10 PanelScreen::Init,
// sub_82259128 NodeScreen::Init: the screen's words reset, then its two
// vtable slots (the state machine Inits' and PanelScreen::InitWords' plain
// C++ where a slot holds them, else guest calls), then the state word 0.
uint64_t ListScreenInit(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t PanelScreenInit(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t NodeScreenInit(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82234ED8 Type6Screen0::Draw: the page's (+9644's +0) handler from the
// table at 0x826367B8, a tail call; nothing for page -1.
uint64_t Type6Screen0Draw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                          const Calls& calls);
// sub_82232740 Type6Dialog::Draw, sub_822421C0 Type7Dialog::Draw: the page
// (+9640)'s handler (ConfirmPage, NoticePage) from a table in the frame.
uint64_t DialogDraw(uint8_t* base, uint64_t r3, uint64_t r4, bool type7, uint32_t sp,
                    const Calls& calls);
// sub_82245148 Type7Screen0::Draw: the page's member function from the
// table at 0x82815E20 (built once, its guard bit 0 at 0x82815E38, from
// words in the frame), on the screen plus its adjustment.
uint64_t Type7Screen0Draw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls);
// sub_821C7F28 ControllerOptions::Draw: the page's (the page object's +0)
// handler from a three-entry table in the frame, on r4.
uint64_t ControllerOptionsDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                               const Calls& calls);

// --- The stand scene's teardown and the garage camera's presets
// (garage.cpp) ---

constexpr uint32_t kStandHolder = 0x826D83E0;  // [it] the stand's holder (three slots)
constexpr uint32_t kPresetsAsset = 22600;
constexpr uint32_t kOnPresetsLoaded = 0x8217FBE8;

// sub_821802D8 GarageCamera::LoadPresets: the presets' asset (22600) size
// from its TOC entry (+8, else +4), that many bytes allocated (the layer's
// pages) and cleared (XMemSet), and Archive::Load(22600, the block,
// OnPresetsLoaded, the camera) through `load`. Returns its serial.
int64_t GarageCameraLoadPresets(uint8_t* base, uint32_t camera,
                                const std::function<int64_t(uint32_t, uint32_t, uint32_t,
                                                            uint32_t)>& load);
using LoadCall = std::function<int64_t(uint32_t, uint32_t, uint32_t, uint32_t)>;
constexpr uint32_t kGarageCameraInitFrame = 96;
constexpr uint32_t kGarageStageInitFrame = 144;
constexpr uint32_t kStandSlotInitFrame = 112;
constexpr uint32_t kGarageStageBytes = 6208;
// sub_82180918 GarageCamera::Init: GarageCamera::Reset (its frame below
// this one's), no presets (+4392 = 0), the presets' index table (+292 =
// 0x82619428), not set up (+0 = 0), then GarageCamera::LoadPresets through
// `load`. `sp` is the caller's stack pointer. Returns the load's serial.
int64_t GarageCameraInit(uint8_t* base, uint32_t camera, uint32_t sp, const LoadCall& load);
// sub_82180968 GarageStage::Init: the stage's words and sizes, its two
// (0, 0, 0, 1.0) vectors through its frame, then GarageStage::ResetPose
// (+32), GarageCamera::Init (+112), GarageLights::Init (+4512) and +6048 =
// 0.
void GarageStageInit(uint8_t* base, uint32_t stage, uint32_t index, uint32_t sp,
                     const LoadCall& load);
// sub_82180AA0 StandSlot::Init: when the slot's +0 is 0, a 6,208-byte
// stage from the allocation layer's pages (VirtualAlloc's zeroed pages)
// stored there and GarageStage::Init'ed. Whether it did (r3 is then
// GarageLights::Init's, else the slot).
bool StandSlotInit(uint8_t* base, uint32_t slot, uint32_t index, uint32_t sp,
                   const LoadCall& load);

// sub_82179AE8 StandScene::PollTeardown (no arguments): 1 once the car
// scene is torn down, else 0. With no teardown under way (the holder's
// +12 0): CarScene::LoadsDone (its plain C++, the finishers guest calls);
// not done with a stand model made (+20), each stage's scene model (pool 7
// of the second set, ScenePools::GetB's plain C++) hidden (+4296 = 0) and
// CarScene::Update and Draw (guest calls to their overrides); else +12 = 1,
// +16 = 10 and CarScene::StartTeardown (a guest call). Under way: +16 down
// by one, CarScene::Update while above 0, else +12 = 0 and 1. `sp` is the
// caller's stack pointer (its frame 128 bytes below).
constexpr uint32_t kPollTeardownFrame = 128;
uint64_t StandScenePollTeardown(uint8_t* base, uint32_t sp, const Calls& calls);

// sub_82180738 StandScene::Release: Viewports::Release, then each of the
// holder's three slots (StandSlot::Free, the slot freed, its word 0), the
// holder freed and [0x826D83E0] = 0. nullopt, with nothing done, where a
// block isn't the allocation layer's pages (the adapter runs the
// original). r3 the holder's free's.
std::optional<uint64_t> StandSceneRelease(uint8_t* base);

// --- The dialogs' confirm (dialogs.cpp): r3 = the dialog; `sp` the
// caller's stack pointer; each returns r3 as the original leaves it. ---

// sub_822320D8 Type6Dialog::OpenConfirm and sub_82241B40
// Type7Dialog::OpenConfirm (`type7`: without the final +9756 = 5): movie 16
// (+9744 = 16) reset (UiMovie::ResetPose, whose r3 it leaves), the cursor
// from the movie's +40960 (the previous to +9672), the pages cleared, then
// state 4 with page 0 through step 0 when the movies are loaded (+9608),
// else state 1.
uint64_t DialogOpenConfirm(uint8_t* base, uint32_t dialog, bool type7, uint32_t sp);
// sub_82241C18 Dialog::DrawWaiting: with a reopen queued (+9748), +9748 =
// 0 and the open's work without the status store; nullopt (r3 left) when
// none.
std::optional<uint64_t> DialogDrawWaiting(uint8_t* base, uint32_t dialog, uint32_t sp);
// sub_822325E0 Dialog::OpenConfirmStep: on the first frame (+9664 0)
// MenuSound::Play(5), named animation 5 and +9664 = 1; UiMovie::Run(movie,
// 0); when it returns 0, the cursor's highlight (named 1 or 2 from a table
// at the frame's +80) played and looped. 1 when done, else 0.
uint64_t DialogOpenConfirmStep(uint8_t* base, uint32_t dialog, uint32_t sp, const Calls& calls);
// sub_8223AA10 Dialog::CloseConfirmStep: as the open's with named 6; when
// done, UiMovie::FinishAll and +9740 = 9.
uint64_t DialogCloseConfirmStep(uint8_t* base, uint32_t dialog, uint32_t sp, const Calls& calls);
// sub_8223AB50 Dialog::MoveCursorStep: on the first frame the previous
// highlight finished (UiMovie::FinishNamed), the cursor's played and looped,
// +9664 = 1; UiMovie::Run. Returns 1.
uint64_t DialogMoveCursorStep(uint8_t* base, uint32_t dialog, uint32_t sp, const Calls& calls);
// sub_82232328 Type6Dialog::ConfirmPage and sub_82241EF8
// Type7Dialog::ConfirmPage (r3 the dialog, `dispatcher` r4): with the shown
// word (+9756, +9760) 4, it = 1 and nothing else; else it = 1, Menu::Step
// and Menu::Pressed (their plain C++) on the menu at +9668: accept (159) or
// back (158) play MenuSound 0 or 2, start the close step (1) and post 9 or
// 10 for the cursor, or 12 (type 7: 8, 9, 11; then the shown word 2); the
// cursor moved (+9676) MenuSound 4 and step 4. r3 as the original leaves it.
uint64_t DialogConfirmPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, bool type7,
                           uint32_t sp, const Calls& calls);
// sub_82232798 Type6Dialog::Update and sub_82242218 Type7Dialog::Update:
// the movie at +9744; with no step (+9652 -1) UiMovie::Run, else the step
// from the five-entry table in its frame (a guest call) and, when it's done
// (or the step is another negative value), the page state moved on; then a
// queued state (+9740 not 21) applied (9: message 13 (type 7: 12) posted).
uint64_t DialogUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher, bool type7, uint32_t sp,
                      const Calls& calls);

// --- The first screens' helpers (screen0.cpp) ---

// The confirms: movie 16's texts (-1, 0) = `text`, (1, 3) = 2 and (1, 4) =
// 3 (UiMovie::SetText from the 112-byte frame below `sp`, r7 to r10 0), its
// +40960 (the default choice) = `choice`, then message (`message`, 0, 0)
// queued in the dispatcher's ring. r3 the last SetText's.
struct ConfirmKind {
  const char* name;
  uint32_t text, choice, message;
};
// sub_82233FC8 Type6Screen0::Confirm (27, 1, 7), sub_822340B0
// Type6Screen0::ConfirmAgain (29, 0, 7), sub_82244528 Type7Screen0::Confirm
// (27, 1, 6).
inline constexpr ConfirmKind kType6Confirm{"sub_82233FC8 Type6Screen0::Confirm", 27, 1, 7};
inline constexpr ConfirmKind kType6ConfirmAgain{"sub_822340B0 Type6Screen0::ConfirmAgain", 29, 0,
                                                7};
inline constexpr ConfirmKind kType7Confirm{"sub_82244528 Type7Screen0::Confirm", 27, 1, 6};
uint64_t Screen0Confirm(uint8_t* base, const ConfirmKind& kind, uint32_t dispatcher, uint32_t sp,
                        const strings_and_font::FormatCalls& calls);
// sub_822344A8 Type6Screen0::ShowPage: movie 22's sub-scene 3 nodes 4 and 5
// shown, 6 to 9, 13 and 14 hidden, then by `page` (0 to 3) nodes shown and
// their textures set (else 4 and 5 hidden). r3 the last node.
uint64_t Type6Screen0ShowPage(uint8_t* base, uint32_t page, uint32_t sp);
// sub_82244930 Type7Screen0::ShowPage: movie 22's sub-scene 2 nodes 4 to 7
// shown, 8 and 9 hidden, then by `page` (0, 1, else) their textures set
// (page 1 hides 6 and 7). r3 the last node.
uint64_t Type7Screen0ShowPage(uint8_t* base, uint32_t page, uint32_t sp);
// sub_822448B0 Type7Screen0::PlaceCursor: movie 22's node (-1, 3)'s child
// 0's +16 = float(40 * item) (the doubleword through the frame's +80). r3
// the child.
uint64_t Type7Screen0PlaceCursor(uint8_t* base, uint64_t item, uint32_t sp);
// sub_82244AB8 Type7Screen0::ShowHelp: movie 22's text (1, 0) scrolled back
// (sub_8211C5F8), set to string 916 to 919 by `item` (UiMovie::SetText,
// r7 to r10 0) and scrolled (UiMovie::ScrollText). r3 ScrollText's.
uint64_t Type7Screen0ShowHelp(uint8_t* base, uint32_t item, uint32_t sp,
                              const strings_and_font::FormatCalls& calls);
// sub_82244760 Type7Screen0::FadeOutStep: the screen fades' fade out on the
// step's first frame (the page state's +24), UiMovie::Run on movie 22 (a
// guest call); done (1, after UiMovie::FinishAll) when both fades' counts
// are 0, else 0.
uint64_t Type7Screen0FadeOutStep(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls);
// sub_82244610 Type7Screen0::ChosenPage (r4 the dispatcher): by the menu's
// item, state 9 and message 13 (with the music changed: the sound
// manager's slot 13 on bank 1's cue 2, Sound::Play's plain C++ on bank 1's
// cue 0), 15 or 23 posted; 3: state 10. r3 as the original leaves it.
uint64_t Type7Screen0ChosenPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                                const Calls& calls);

// sub_82244BF8 Type7Screen0::MenuPage (r3 the screen, r4 the dispatcher):
// Menu::Step and Menu::Pressed's plain C++ on the menu (+9652), then by
// what happened: the cursor moved (MenuSound 4, the cursor step 2); 163
// (MenuSound 0, the power graph's set-up sub_82250488, never seen in play,
// a guest call to the recompiled original, message 18, page 1 shown);
// accept 159 (MenuSound 0, the item to +9656: items 0 to 2 the fade out
// step 3 to page 2, item 3 Type7Screen0::Confirm (a guest call to its
// override) and page 2 shown); back 158 (MenuSound 2, the cursor on the
// quit item, the back step 1). MenuSound::Play's plain C++. r3 as the
// original leaves it.
uint64_t Type7Screen0MenuPage(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls);

// The two modes' cars (1,968-byte garage cars, then +1968 the model made,
// +1972 where from (-1 none, 0 stock, 2 the garage), +1976 the garage index,
// +1980 the car's halfword +0).
constexpr uint32_t kType6Car = 0x826367E8;
constexpr uint32_t kType7Car = 0x826371D0;
// sub_822338E8 Type6Screen0::OpenMovie and sub_82243E80
// Type7Screen0::OpenMovie: UiMovies::Open(22, 22564 or 22569) (calls.open),
// then GarageStage::MakeModel([[0x826D83E0]], CarModels::BlockOf(0), 1)
// and GarageStage::Command(the same, 14), guest calls to their overrides.
// r3 Command's.
uint64_t Screen0OpenMovie(uint8_t* base, bool type7, uint32_t sp, const Calls& calls);
// sub_82233E80 Type6Screen0::SetUpCar and sub_822443E0
// Type7Screen0::SetUpCar (r4 the dispatcher): with no car yet (+1972 -1),
// GarageCar::Reset (a guest call) and the selected garage car copied from
// SaveData::Of(0)'s garage (message 6 or 5), or with an empty garage
// message 20 or 17; with a car, message 6 or 5; then
// ScreenFades::FadeOut(1, 0, 0, 0, 0). r3 1, as FadeOut leaves it.
uint64_t Screen0SetUpCar(uint8_t* base, bool type7, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_82233B70 Type6Screen0::ShowCarInfo and sub_822440D8
// Type7Screen0::ShowCarInfo: movie 22 reset (UiMovie::ResetPose), the
// menu's labels (sub-scene 4, strings 109, 120, 110, 113, 114; type 7
// sub-scene 3 without 120) from a table in the frame, a car object in the
// frame (Car::Init; type 6 Car::SetModel from the type 6 car's id and its
// +3888 |= 1, type 7 the type 7 car copied; Car::Refresh), then texts 10 to
// 12 (type 7: 12 to 14) its class (1934 + block 2's byte +129), name and
// maker (1646 and 1718 + the id). UiMovie::SetText with r7 to r10 0. r3 the
// last SetText's. `sp` is the caller's stack pointer.
constexpr uint32_t kType6ShowCarInfoFrame = 4064;
constexpr uint32_t kType7ShowCarInfoFrame = 4048;
uint64_t Screen0ShowCarInfo(uint8_t* base, bool type7, uint32_t sp, const Calls& calls,
                            const strings_and_font::FormatCalls& format);
// sub_82233C80 Type6Screen0::Show and sub_822441D8 Type7Screen0::Show (r3
// the screen): the car's model word 0, the loading animation's word 0; with
// no car yet (+1972 -1), GarageCar::Reset (a guest call) and the stock car
// 0 (Car::SetModelAndClass's plain C++) for an empty garage, else the
// selected garage car copied; the menu's cursor back on the item picked,
// ShowCarInfo, the page state to the opening step. r3 ShowCarInfo's.
constexpr uint32_t kShowFrame = 128;
uint64_t Screen0Show(uint8_t* base, bool type7, uint64_t r3, uint32_t sp, const Calls& calls,
                     const strings_and_font::FormatCalls& format);
// sub_82234F08 Type6Screen0::Update and sub_82245200 Type7Screen0::Update
// (r3 the screen): movie 22 (UiMovies::Find); the page state's (+9644,
// type 7 +9648) step from its table (type 6 the image's at 0x826367C8, r4
// the frame's +80; type 7 the member pointers at 0x82815E3C, built once
// from the frame under its guard bit 0 at 0x82815E5C, r4 the step) as a
// guest call, the page state moved on when it answers nonzero, else
// UiMovie::Run(movie, 0); then unless the mode's car's +1972 is -1, the
// stand (`[[0x826D83E0]]`): GarageStage::IsBusy and the loading animation
// (named 44 to 46: UiMovie::FinishNamed, PlayNamed, LoopNamed) by the
// loading word (+9640's, type 7 +9644's +0), then while the car's +1968 is
// 0, +1968 = GarageStage::ReloadModel(CarModels::BlockOf(the car's +12)),
// with 1 a car object in the frame (Car::Init, the car copied, its +3888
// |= 1, Car::Refresh), GarageStage::SetCar(-1) and SetPlates; once it's
// set, GarageStage::Draw(0, 1). Guest calls to the callees' overrides, but
// BlockOf's, SetPlates', the car's and the memcpy's plain C++. `sp` is the
// caller's r1 (64-bit). r3 as the original leaves it.
constexpr uint32_t kType6UpdateFrame = 4064;
constexpr uint32_t kType7UpdateFrame = 4080;
uint64_t Screen0Update(uint8_t* base, bool type7, uint64_t r3, uint64_t sp, const Calls& calls);
// sub_82234768 Type6Screen0::OnDialogClosed (r4 the dispatcher): by the
// confirm asked (+9664: 4, 5) and the pick (+9640's +72): page 0 and
// ShowPage 0; the second confirm (Type6Screen0::ConfirmAgain, a guest call
// to its override) and ShowPage 2; the fade out step; page 2, message 29 and
// ShowPage 3. ShowPage's plain C++. r3 as the original leaves it.
uint64_t Type6Screen0OnDialogClosed(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                    const Calls& calls);
// sub_82244B60 Type7Screen0::OnDialogClosed: by the pick (+9644's +72):
// page 0 and ShowPage 0 (a tail call from `sp`), or the fade out step.
uint64_t Type7Screen0OnDialogClosed(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_821C7F98 ControllerOptions::Update: the 12-entry step table in its
// frame, the page object's step (+12) called (a guest call, r3 the started
// word +24) and, when done or another negative step, the page state moved
// on. r3 as the original leaves it.
uint64_t ControllerOptionsUpdate(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The garage camera's reset (garage_camera.cpp) ---

// sub_8217C120 Quaternion::FromAngles: the quaternion of the three angles
// (radians; DirectXMath's roll-pitch-yaw math, the game's sine and cosine
// polynomials) stored at `out` (aligned down to 16). The angles go below
// `sp` first (sp - 32 to sp - 20: f3, f1, 0.0, f2) and are read back with
// lvlx, as the original's leaf does.
void QuaternionFromAngles(uint8_t* base, uint32_t out, double f1, double f2, double f3,
                          uint32_t sp);
// sub_8217D888 GarageCamera::Reset: the camera's eye, target, rotation,
// translation row, angles, field of view, zoom (640 / tan(pi / 3)), its
// identity matrix and its quaternion (FromAngles(0, 0, 0)) set, the vectors
// built in its frame at `frame` (its r1 after its stwu) first.
void GarageCameraReset(uint8_t* base, uint32_t camera, uint32_t frame);
constexpr uint32_t kGarageCameraResetFrame = 304;
// sub_8217D3C8 GarageLights::Init: the 11 light records (96 bytes each)
// copied from the table at 0x82635950 to +16, the ambient (0.08 three
// times, 1.0) to +1072 and 0x8265ED00, and the scene light constants
// (0x8296B950 to 0x8296BC9F: three lights, four spots with their normalized
// directions and cone cosines) written as GarageStage::SetLights does,
// through its frame at `frame` (its r1 after its stwu).
void GarageLightsInit(uint8_t* base, uint32_t lights, uint32_t frame);
constexpr uint32_t kGarageLightsFrame = 320;
constexpr uint32_t kGarageLightsTable = 0x82635950;

}  // namespace type67_modes
