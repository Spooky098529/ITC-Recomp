// The front end's save screen natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/save-screen.md): UI movie 44's yes/no
// dialogs and waits around the save layer, one state machine in five
// front-end modes. Plain C++ over guest memory (the machine, the mode, the
// three blocks at [0x826D844C] to [0x826D8454] and the movie are shared
// with the recompiled code), with the originals' loads and stores in their
// order and their frames' words. `sp` is the caller's stack pointer: the
// back chain of the original's frame and its locals are written below it as
// the original leaves them; its register saves aren't. Adapters:
// src/hooks/save_screen.cpp (behind --native_save_screen).
//
// The guest calls go through `Calls` (race_hud_object::Calls: r3 to r6 as
// given, r7 to r10 0) by address, to the callees' overrides: the menu list
// (MenuList::Update, Accepted, Cancelled, Selection,
// GetPreviousSelection), MenuSound::Play, the UI movie calls
// (UiMovie::FinishNamed, PlayNamed, LoopNamed, Run) and the save layer
// (SaveSlots::RequestDevice, State, Device, Pending, GameData::Save, Load
// and Check, and the two wrappers never seen in play, sub_8228FA20 and
// sub_8228FA58, the originals). UiMovies::Find is its plain C++. Each
// function returns r3 as the original leaves it (the last call's, or the
// argument for the leaves).

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace save_screen {

using Calls = race_hud_object::Calls;

// The screen's blocks: the page block (28 bytes: +0 the page Draw runs, -1
// while a step runs; +4 the page after the step; +8 the page before; +12
// the step Update runs, -1 none; +16 the step queued after it; +24 the
// step's started word), the menu list (72 bytes) and the screen's data (68
// bytes: +4 the end's answer, +12 the dialog's message, +16 its state, +20
// the state after the close, +24 the wait counter, +28 the cancel, +32 the
// machine's next state, 21 none, +36 the kind).
constexpr uint32_t kPages = 0x826D844C;
constexpr uint32_t kList = 0x826D8450;
constexpr uint32_t kData = 0x826D8454;
// The mode type (Modes::), which the end pages post by.
constexpr uint32_t kModeType = 0x826032DC;

// The callees by address.
constexpr uint32_t kMenuListUpdate = 0x8220C1E0;
constexpr uint32_t kMenuListAccepted = 0x8220BD50;
constexpr uint32_t kMenuListCancelled = 0x8220BDE0;
constexpr uint32_t kMenuListSelection = 0x8220BC20;
constexpr uint32_t kMenuListPrevious = 0x8220BC28;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kRequestDevice = 0x8228F9B0;
constexpr uint32_t kSlotState = 0x8228F9E8;
constexpr uint32_t kSlotDevice = 0x8228FA90;
constexpr uint32_t kSlotPending = 0x8228FAC8;
constexpr uint32_t kWord992 = 0x8228FA20;    // never seen in play
constexpr uint32_t kRequest980 = 0x8228FA58;  // never seen in play
constexpr uint32_t kGameDataSave = 0x8228FB00;
constexpr uint32_t kGameDataLoad = 0x8228FBC0;
constexpr uint32_t kGameDataCheck = 0x8228FC50;
constexpr uint32_t kReplaysLoad = 0x8228FF88;      // never seen in play
constexpr uint32_t kReplaysBuildList = 0x82290378;
constexpr uint32_t kReplayWord84 = 0x82290640;     // never seen in play
constexpr uint32_t kReplayCopyFrom = 0x821A66A8;   // never seen in play
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kMemRelease = 0x8000;
constexpr uint32_t kMenuListInit = 0x8220BAC0;
constexpr uint32_t kMenuListRelease = 0x8220BBB0;
constexpr uint32_t kMoviesOpen = 0x8211E808;    // UiMovies::Open
constexpr uint32_t kMoviesDelete = 0x8211E258;  // UiMovies::Delete
constexpr uint32_t kFinishAll = 0x8211DFF8;     // UiMovie::FinishAll
constexpr uint32_t kSaveBuffersCreate = 0x8228F6B0;
constexpr uint32_t kSaveBuffersDestroy = 0x8228F8F8;
constexpr uint32_t kDebugPrint = 0x82106540;  // empty in the build
// The kinds 3 to 6's Shows (never seen in play: the originals).
constexpr uint32_t kShowKind3 = 0x821A1E18;
constexpr uint32_t kShowKind4 = 0x821A1ED8;
constexpr uint32_t kShowKind5 = 0x821A1FA8;
constexpr uint32_t kShowKind6 = 0x821A2070;

// The machine's words: +9604 the state, +9608 the movie loaded, +9612 load
// the movie at once.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineAtOnce = 9612;
constexpr uint32_t kSaveDataOf = 0x82105620;

// The save buffers' header (+0 the game data's buffer, +4 a replay's, +20
// the replays' count), the replay kept for the replay screen, the profile's
// read (997 while pending), whether to read it, and the profile reader
// (its +4 the results).
constexpr uint32_t kSaveBuffers = 0x828161F0;
constexpr uint32_t kKeptReplay = 0x826C5B50;
constexpr uint32_t kProfileRead = 0x826C6080;
constexpr uint32_t kProfileWanted = 0x826C7958;
constexpr uint32_t kProfileReader = 0x826C6AD0;
constexpr uint32_t kGameDataBytes = 0x1F978;
// The players' controllers and their action mappers' entries (8 bytes: +0
// the mapper, +4 1 when the controller isn't read).
constexpr uint32_t kControllers = 0x826BABA4;
constexpr uint32_t kMapperEntries = 0x826BAB80;

// --- The state machine Inits (inits.cpp): each adds the machine's
// transitions through StateMachine::Add's plain C++, as the original builds
// them in its frame (the step tables in init_tables.h, from
// itc_recomp/tools/init_step_tables.py). `object` is the original's r3 (the
// machine); each returns the last Add's answer (`replaced`: the index, else
// the new transition) and `machine` the machine it was added to. ---

struct InitResult {
  uint32_t r3;
  uint32_t machine;
  bool replaced;
};
// An Init: its address, name, frame (its stwu size) and the registers
// __savegprlr_<n> saves from (r<n> to r31 and lr).
struct InitInfo {
  uint32_t address;
  const char* name;
  uint32_t frame;
  uint32_t first_saved;
};
// sub_821A6E18 Type2SaveScreen::Init, sub_821A7470 Type4SaveScreen::Init
// (the title's) and sub_821A7AC8 Type10SaveScreen::Init (the replays').
// Type8SaveScreen::Init sub_821A87F0 is never seen in play (recompiled).
constexpr int kInitCount = 3;
extern const InitInfo kInits[kInitCount];
// The Init at `address` (one of kInits) on `object`, its frame at `frame`
// (its back chain written).
InitResult RunInit(uint8_t* base, uint32_t address, uint32_t object, uint32_t frame);

// --- The machine's handlers (handlers.cpp): r3 the machine, r4 the mode ---

// sub_821A2608 SaveScreen::OnEnter (0, 1): +9608 = 0; the page block (28
// bytes), the menu list (72) and the data (68) made by VirtualAlloc
// (MEM_COMMIT, PAGE_READWRITE) into their globals, their words set; +9604 =
// 1 when +9612 is set, else 9. The data's VirtualAlloc r3. 112-byte frame.
uint64_t OnEnter(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_821A1D10 SaveScreen::OpenMovie (1, 1): a tail call of
// UiMovies::Open(44, 22588) on the caller's frame with its lr.
uint64_t OpenMovie(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls);
// sub_821A26F0 SaveScreen::WaitMovie (2, 1): +9608 = 1 once UiMovies::IsLoaded(44)
// (its plain C++); then +9604 = 9 or 4 by +9612. IsLoaded's r3, or the
// machine. 96-byte frame.
uint64_t WaitMovie(uint8_t* base, uint64_t machine, uint32_t sp);
// The open messages' handlers: the data's +36 = `kind`; +9604 = 4 when the
// movie is loaded, else 1. sub_821A2760 OpenKind0, sub_821A2790 OpenKind1,
// sub_821A2820 OpenKind2, sub_821A27F0 OpenKind3, sub_821A2850 OpenKind4,
// sub_821A2880 OpenKind5, sub_821A27C0 OpenKind6, sub_821A28B0 OpenKind7.
void OpenKind(uint8_t* base, uint32_t machine, uint32_t kind);
// sub_821A6608 SaveScreen::Show (4, 1), with its case 0 sub_821A6650
// (ShowCheck) done here: by the data's +36, SaveBuffers::Create (kinds 0, 1
// and 6), StartCheck's and Start's plain C++ (src/front_end_screen_helpers/),
// ShowKind2's and ShowKind7's, the kinds 3 to 6's originals. The last
// call's r3, or `r3` above 7. 96-byte frame.
uint64_t Show(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_821A1D38 SaveScreen::ShowKind2 and sub_821A2138 ShowKind7: the data's
// dialog set (message 2 with the note, page 13 by step 4; message 0, page 24
// by step 6), MenuList::Init([0x826D8450], 1, 1, 0, 154, 153, 155, 156, 159,
// 158; the last two on the frame's +84 and +92). Init's r3. 128-byte frames.
uint64_t ShowKind2(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t ShowKind7(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821A1D20 SaveScreen::Cancel: the data's +28 = 1.
void Cancel(uint8_t* base);
// sub_821A4C58 SaveScreen::Leave (5, 4) and (9, 4): +9604 = 7, movie 44's
// animations finished (UiMovie::FinishAll) and the movie deleted
// (UiMovies::Delete); with the mode type 2, 4, 6, 8 or 10, message 4 posted
// on the mode's ring. Delete's r3. 112-byte frame.
uint64_t Leave(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_821A1CA0 SaveScreen::OnLeft (7, 4): MenuList::Release, the three
// blocks freed (VirtualFree, MEM_RELEASE) and their globals 0. The last
// VirtualFree's r3. 112-byte frame.
uint64_t OnLeft(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The machine's Update and Draw (update.cpp) ---

// sub_821A64A0 SaveScreen::Update (5, 0): the empty sub_82106540(100, 30,
// 0, 0x8200A4F0); movie 44; the step table on the frame (+96 to +120: steps
// 0 to 6); with a step running (the page block's +12 not -1) a step 0 or
// more called through the guest (r3 the block's +24, r4 to r9 steps 0 to 5's
// addresses, r10 the step × 4, as the original leaves them), and when it
// answers nonzero (or at once for a negative step) the queued step started
// (+12 = +16, +16 = -1, +20 = 0) or, with none, the page after shown (+0 =
// +4, +12 = -1), then +24 = 0; with none running and the dialog shown (the
// data's +16 not -1), UiMovie::Run(movie, 0). The last call's r3 (the
// movie with none). 144-byte frame.
uint64_t Update(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821A6AF0 SaveScreen::Draw (5, 1): r3 the machine and r4 the mode
// stored in the caller's area (sp + 20, sp + 28); the cancel (the data's +28,
// unless a step runs or the page is an end page) turned into the kind's end
// page; the page table on the frame (+96 to +200: pages 0 to 26; +80 the
// table's base 0x821A0000) and the page called through the guest (r3 the
// mode, r4 to r9 pages 20 to 25's addresses, r10 the table's address, as
// the original leaves them); then with the data's +32 not 21 the machine's
// +9604 = it, +32 = 21 and, for the kinds 0 and 1, SaveBuffers::Destroy.
// The last call's r3 (0x821A2C98 with none). 368-byte frame.
uint64_t Draw(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls);

// --- The dialog steps (steps.cpp): `step` the started word ---

// sub_821A5118 SaveScreen::MoveSelection (step 1): first call the
// selection's named animation (Selection + 2) played and looped after the
// previous one's (GetPreviousSelection + 2) finished, the word 1; every
// call UiMovie::Run(movie 44, 0). 1. 128-byte frame.
uint64_t MoveSelection(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_821A5038 SaveScreen::CloseDialog (step 2): first call by the data's
// +16 (unsigned) the dialog's close (0: the selection's finished and 14
// played; 1: 15; 2: 0 finished and 15), MenuSound::Play(5), the word 1;
// every call Run; when it answers 0 the data's +16 = its +20. Whether Run
// answered 0. 128-byte frame.
uint64_t CloseDialog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

// --- The pages (pages.cpp, pages_load.cpp, pages_replays.cpp): `mode` r3 ---

// sub_821A28E0 SaveScreen::Page0: kind 0's questions. 128-byte frame.
uint64_t Page0(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_821A2AF8 SaveScreen::Page1: kind 0's notes. 96-byte frame.
uint64_t Page1(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_821A2C98 SaveScreen::Page2: kind 0's wait. 96-byte frame.
uint64_t Page2(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_821A2EE8 SaveScreen::Page3: kind 0's device wait. 96-byte frame.
uint64_t Page3(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// The end pages (leaves): the result posted on the mode's ring by the mode
// type, the data's +32 = 9. sub_821A4D30 Page4 (kind 0), sub_821A4DE0
// Page9 (kind 1), sub_821A4E60 Page16 (the replays), sub_821A4FD8 Page25
// (kind 7).
uint64_t Page4(uint8_t* base, uint64_t mode);
// sub_821A32C0 SaveScreen::Page6: kind 1's note. 96-byte frame.
uint64_t Page6(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_821A33E8 SaveScreen::Page7: kind 1's load and save wait (the loaded
// game data copied into player 0's save data). 96-byte frame.
uint64_t Page7(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_821A3728 SaveScreen::Page8: kind 1's device wait. 96-byte frame.
uint64_t Page8(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_821A3A08 SaveScreen::Page12: the replays' question. 128-byte frame.
uint64_t Page12(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_821A3C60 SaveScreen::Page13: the replays' note. 96-byte frame.
uint64_t Page13(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_821A67E8 SaveScreen::Page14: the replays' wait (the list built, a
// replay loaded). 96-byte frame.
uint64_t Page14(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_821A3DE8 SaveScreen::Page15: the replays' device wait. 96-byte frame.
uint64_t Page15(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_821A4AC0 SaveScreen::Page24: kind 7's controller pick (the players'
// mappers' slot 10, ActionMapper::Pressed, through their vtables). 128-byte
// frame.
uint64_t Page24(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);

// sub_821A24B0 SaveScreen::StoreSettings: once (the data's +60), the
// profile's results into player 0's save data (+0x1F85C: +20, +24, the
// 60-byte block at +44 rewritten through the frame with two pairs of words
// swapped). With the profile's +672 3 or more, +24 the frame's +80 as the
// caller left it (copied as the original leaves it). 1, or 0 while the
// profile's read is pending. 224-byte frame.
uint64_t StoreSettings(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Page9(uint8_t* base, uint64_t mode);
uint64_t Page16(uint8_t* base, uint64_t mode);
uint64_t Page25(uint8_t* base, uint64_t mode);

}  // namespace save_screen
