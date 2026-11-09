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
