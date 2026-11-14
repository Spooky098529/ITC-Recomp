// The in-race screens never seen in play natively (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/race-screens-unseen.md), ported by the user's call
// (2026-11-12, to minimize the code left recompiled). Plain C++ over guest
// memory, with the originals' loads and stores in their order, as the other
// in-race screens' (race_screens.h): `sp` is the caller's stack pointer,
// each function's frame made below it with its back chain (its register
// saves aren't written), and the guest calls made from that frame with the
// original's return address, so the UI movie calls see what they'd see.
// Adapters: src/hooks/race_screens_unseen.cpp (behind
// --native_race_screens_unseen).

#pragma once

#include "race_screens/race_screens.h"

#include <cstdint>

namespace race_screens {

// --- Kind 4's pause menu (pause_menu4.cpp) ---
//
// The screen [0x826D84E0] (its +0 the movie, UI movie 5), the data
// [0x826D84E4] (80 bytes), the page object [0x826D84E8] and the menu list
// [0x826D84EC], read again before each use as the originals do. The UI
// movie calls (Play, Loop, LoopLast, Finish, FinishNamed, PlayNamed, Run,
// SetText, SetNodeText, ResetPose, RestorePose), MenuList::Init, Update,
// Accepted, Cancelled and Pressed152, Options::ShowVolumeBar, the sound
// options' setters and MenuSound::Play through their overrides (`calls`);
// UiMovies::Find, UiMovie::FindNode and Child, ScreenPages::Go and Finish,
// MenuList::Selection, SetSelection and IsEnabled, SaveData::Of and the
// confirm dialog's Update, Open and Draw by their plain C++. SetText's and
// SetNodeText's unset varargs are passed as 0 (the user's call for the
// parking area, 2026-10-30: only dead words of the caller's frame differ).
// The items by kind: six words a kind at 0x8200CD08, -1 ends them.
constexpr uint32_t kPause4Items = 0x8200CD08;

// sub_82211390 PauseMenu4::Open (r3 the screen, r4 the player who paused,
// r5 the kind): UI movie 5 found (the id at the frame's +80) into the
// screen's +0, UiMovie::ResetPose; the type -1 nodes 5 and 3's +108 = the
// player; the list's +60 and the data's +72 = 1 for a player other than 0,
// else 0; +76 = the kind; Go(0, transition 1, then 2), MenuSound::Play(6),
// LoadVolumes. LoadVolumes' r3 (player 0's save data).
uint64_t Pause4Open(uint8_t* base, uint32_t screen, uint64_t player, uint64_t kind, uint32_t sp,
                    const Calls& calls);
// sub_82211458 PauseMenu4::Update: the page's handler (0 MainPage, 1
// VolumePage, 2 ConfirmPage with `r4`, 3 AnswerPage; their table stored in
// the frame as the original's), its r3; 0 with no page (-1). A page past 3
// runs the frame's word through `calls`, as the original's bctrl would.
uint64_t Pause4Update(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_82211590 PauseMenu4::Draw (r3 the screen): with a transition running
// (the page object's +12), its handler (11, their table stored in the frame)
// with the page object's +24, then ScreenPages::Finish with its answer; a
// negative transition other than -1 Finish(1); none UiMovie::Run(the
// screen's movie, 0). Then on page 2 the confirm dialog's draw. r3 as the
// original leaves it.
uint64_t Pause4Draw(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls);

// The pages (no arguments but ConfirmPage's r4, which RaceDialog::Update
// passes on). sub_8220FD48 MainPage (its case 0, sub_8220FE3C, merged),
// sub_82210E58 VolumePage, sub_8220FF30 ConfirmPage: 0. sub_82210910
// AnswerPage: SaveVolumes, then the data's +56.
uint64_t Pause4MainPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Pause4VolumePage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Pause4ConfirmPage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls);
uint64_t Pause4AnswerPage(uint8_t* base, uint32_t sp);

// The helpers. sub_82210488 StepVolume: by the list's +28 (-1 or 1) the
// selected volume (+8, +12) stepped within 0 to 15, or item 2's +60
// toggled; 1 when it changed (a step past a bound stores the bound and
// answers 0). sub_822107B8 LoadVolumes and sub_82210820 SaveVolumes: the
// data's +0 and +4 from or to player 0's save data's bytes +0x1F878 and
// +0x1F879 (LoadVolumes sign-extends them and sets +16 and +20 to 15); the
// save data (r3).
uint32_t Pause4StepVolume(uint8_t* base, uint32_t sp);
uint32_t Pause4LoadVolumes(uint8_t* base, uint32_t sp);
uint32_t Pause4SaveVolumes(uint8_t* base, uint32_t sp);
// sub_82210C00 SetUpMain: the item count (+52), MenuList::Init(list, count,
// count, 1, 154, 153, 155, 156; 159, 158 at the frame's +84 and +92), the
// selection from +64, ShowItems; ShowItems' r3. sub_82210360 ShowItems: the
// type-4 node 0's children: string 1989 + the kind's item (SetText) for the
// first six, else hidden (+80 = 0). sub_82210CD0 ColorItems: each child's
// +56 to +64 = 205, 235, 67 when MenuList::IsEnabled, else 255 each. Each
// returns r3 as the original leaves it.
uint64_t Pause4SetUpMain(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Pause4ShowItems(uint8_t* base, uint32_t sp, const Calls& calls);
uint32_t Pause4ColorItems(uint8_t* base, uint32_t sp);
// sub_82211250 SetUpVolume: MenuList::Init(list, 3, 3, 1, ..), the edits
// (+8, +12) from +0 and +4, +60 = 0, RestoreNodes(-1, 6), the labels
// (SetNodeText: strings 2004 and 2005 on the type-6 node 0's children 5 and
// 6, 4 and 7 on node 1's children 3 and 4), ShowVolumes, ShowVolumesB; the
// last's r3.
uint64_t Pause4SetUpVolume(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_822106E8 RestoreNodes: child `child` (of child `group` unless -1) of
// the movie's file (+24), live (+16) and saved (+20) scenes;
// UiMovie::RestorePose(movie, live, file, 0, 0) and (movie, saved, file, 0,
// 0); the last's r3.
uint64_t Pause4RestoreNodes(uint8_t* base, uint32_t group, uint32_t child, uint32_t sp,
                            const Calls& calls);
// sub_82210670 ShowVolumes: ShowVolume on the type-6 nodes 5 (+8) and 6
// (+12). sub_822105F8 ShowVolume (r3 the node, r4 the volume): SetNodeText(
// movie, child 2, 8, volume), then children 4 to 18's +80 = 1 for the one at
// the volume, else 0; the last child. sub_82210DE0 ShowVolumesB:
// Options::ShowVolumeBar on nodes 5 (+8) and 6 (+12). The last's r3.
uint64_t Pause4ShowVolumes(uint8_t* base, uint32_t sp, const Calls& calls);
uint32_t Pause4ShowVolume(uint8_t* base, uint32_t node, uint64_t volume, uint32_t sp,
                          const Calls& calls);
uint64_t Pause4ShowVolumesB(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82210420 SetArrow (r3 0 or 40): the type-6 node 0's child 0's float
// +16 = the value (its double word left at the frame's +80). The child.
uint32_t Pause4SetArrow(uint8_t* base, uint64_t value, uint32_t sp);

// The transitions (r3 the step word: on the first call, 0, their work and
// the word 1; then UiMovie::Run(movie, 0)), answering whether they're done.
// sub_8220FF88 RunTen (the data's +68 counted, done at 10), sub_82211098
// OpenMain (animation 1, +64 = 0, SetUpMain, ColorItems; done when the run
// answers 0), sub_82210010 ShowMain (animation 0 and the type-4 selection +
// 1 played and looped; 1), sub_822100D8 BackToMain (the type-4 selection +
// 1; 1), sub_82210228 MoveMain (the previous row's finished, the new one's
// played and looped; 1), sub_82210170 CloseMain (the row's and animation 0
// finished, 2 played; the run's 0), sub_822114D8 OpenVolume (+64 = the
// selection, its row finished, SetUpVolume, animation 3; the run's 0),
// sub_822102D8 ShowVolumeRow (type-6 animation 1, LoopLast; 1),
// sub_82210940 MoveVolume (the rows by the selections, SetArrow, named 19
// and 20; 1), sub_82210B08 SetVolume (item 2's rows 3 and 4 swapped, else
// ShowVolume on nodes 6 and 5; 1), sub_82211138 CloseVolume (the row
// finished, animation 4, SetUpMain; when the run answers 0, Go(0,
// transition 3) and the step word 0; 0).
uint64_t Pause4RunTen(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Pause4OpenMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Pause4ShowMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Pause4BackToMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Pause4MoveMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Pause4CloseMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Pause4OpenVolume(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Pause4ShowVolumeRow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Pause4MoveVolume(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Pause4SetVolume(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Pause4CloseVolume(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

}  // namespace race_screens
