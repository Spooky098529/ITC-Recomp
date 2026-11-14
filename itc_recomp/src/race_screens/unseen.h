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

// --- The results at +1028 and +1032 (results.cpp) ---
//
// RaceResults: the screen [0x82814E18] (52 bytes: +0 to +16 the block,
// +20 the movie, UI movie 7, +24 the page object, +48 the step word).
// SplitResults: the screen [0x82814E1C] (40 bytes: +0 the outcome, +4 the
// mode, +8 the movie, UI movie 8, +12 the page object, +36 the step word),
// with the tick count [0x82814E20] and the "other console's result came"
// word [0x82814E24]. The UI movie calls (PlayNamed, LoopNamed, FinishNamed,
// Run, SetText, ResetPose) and MenuSound::Play through their overrides
// (`calls`); UiMovies::Find, UiMovie::FindNode, ScreenPages::Init, Go and
// Finish, the mapper's slot 10 (MapperPressed), Stages::Copy and the shared
// "returns 1" (sub_825C8390) by their plain C++. SetText's unset varargs are
// passed as 0 (the user's call for the parking area, 2026-10-30).
constexpr uint32_t kSplitTicks = 0x82814E20;
constexpr uint32_t kSplitExchanged = 0x82814E24;

// sub_8221F990 RaceResults::Open (r3 the screen, r4 the 20-byte block):
// the block copied to +0, UI movie 7 found (the id at the frame's +80) into
// +20 and its pose reset, ScreenPages::Init(+24, 0) and Go(0, transition 0,
// then 1), SetTexts. SetTexts' r3.
uint64_t RaceResultsOpen(uint8_t* base, uint32_t screen, uint32_t block, uint32_t sp,
                         const Calls& calls);
// sub_8221F628 RaceResults::SetTexts (r3 the screen), from a 160-byte
// frame: the type-5 texts: items 2 to 5 strings 1976 to 1979; item 6
// string 528 with +4's minutes, seconds and thousandths (past 59 minutes
// 59:59.999); item 7 the place string from the frame's table (11 words at
// +80, by +8, unchecked as the original); item 8 string +12 + 1718; item 9
// by +16 (1985 to 1987 for 0 to 2, 1984 with +16 + 1 for 3 to 9, 1988 for
// 10, nothing above). The last SetText's r3.
uint64_t RaceResultsSetTexts(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls);
// sub_8221F870 RaceResults::MainPage: player 0's mapper entry's slot 10 on
// accept (159), then back (158): either MenuSound::Play(0) and Go(1,
// transition 2). Returns 0.
uint64_t RaceResultsMainPage(uint8_t* base, uint32_t sp, bool native_slots, const Calls& calls);
// sub_8221F930 RaceResults::Update (r3 the screen): the page (+24): -1 0,
// 0 MainPage, 1 the shared "returns 1"; past the table the frame's word
// through `calls` (r3, r4 the caller's).
uint64_t RaceResultsUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                           bool native_slots, const Calls& calls);
// The transitions (r3 the step word; on the first call the word 1 and
// their work, then UiMovie::Run(movie, 0)): sub_8221F428 Intro (named 2,
// or 3 when the block's +0 is 0; the run's 0), sub_8221F4B0 Show (named 4,
// then 6 or 7 by +0, played and looped; 1), sub_8221F580 Close (named 6, 7
// and 4 finished, 5 played; the run's 0).
uint64_t RaceResultsIntro(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t RaceResultsShow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t RaceResultsClose(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8221F7D0 RaceResults::Draw (r3 the screen): with a transition
// running (+36 >= 0) its handler with +48, then ScreenPages::Finish(+24,
// its answer) (r3 +24); else UiMovie::Run(+20, 0) (its r3).
uint64_t RaceResultsDraw(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls);

// sub_82220050 SplitResults::Open (r3 the screen, r4 the outcome, r5 the
// mode): +4 and +0 stored, UI movie 8 found into +8 and its pose reset,
// ScreenPages::Init(+12, 0) and Go(0, transition 0, then 1), the two
// globals 0. Returns the screen + 12.
uint64_t SplitResultsOpen(uint8_t* base, uint32_t screen, uint64_t outcome, uint64_t mode,
                          uint32_t sp, const Calls& calls);
// sub_8221FE70 SplitResults::MainPage, from a 192-byte frame: the stage
// copied to the frame's +80 (Stages::Copy); in the network battle (race
// kind 6, the stage's +0 not 1) with [0x82814E24] 1 [0x82814E20] counted
// and past 210 Go(1, transition 2); else players 0's and 1's mapper
// entries' slot 10 on accept and back: any MenuSound::Play(0) and Go(1,
// transition 2). Returns 0.
uint64_t SplitResultsMainPage(uint8_t* base, uint32_t sp, bool native_slots, const Calls& calls);
// sub_8221FFF0 SplitResults::Update (r3 the screen): as RaceResults::
// Update's on the page +12.
uint64_t SplitResultsUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                            bool native_slots, const Calls& calls);
// sub_8221FAC8 Intro (by the mode +4: 0 the group -1 node 0's +80 = 1 and
// named 0 to 2 by the outcome; 1 +80 = 0 and named 3, 4, 2; 2 +80 = 1 and
// named 5; the run's 0), sub_8221FBE8 Show (named 6, then mode 0 8, 9, 10,
// mode 1 11, 12, 10, mode 2 13, played and looped; 1), sub_8221FD98 Close
// (named 8 to 10, 6, 11 to 13 finished, 7 played; the run's 0).
uint64_t SplitResultsIntro(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t SplitResultsShow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t SplitResultsClose(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

// The network battle's words SplitResults::Draw reads: the sign-in watch's
// state and reason, the network manager's peer list (+4 begin, +8 end,
// 384-byte entries), its peer mode word (the list used when set), the other
// console's answer (a byte, 255 none yet) and the word beside it, and the
// pointer whose +7244 is the id sent.
constexpr uint32_t kSignInWatchState = 0x826BAB3C;
constexpr uint32_t kSignInWatchReason = 0x826BAB5C;
constexpr uint32_t kNetPeers = 0x826C7308;
constexpr uint32_t kNetPeerMode = 0x826C7824;
constexpr uint32_t kNetAnswer = 0x826C6B0C;
constexpr uint32_t kNetAnswerWord = 0x826C6B04;
constexpr uint32_t kNetPlayers = 0x826D82E8;

// sub_822200E0 SplitResults::Draw (r3 the screen), from a 2,816-byte frame:
// the stage copied to the frame's +96 (Stages::Copy). In the network battle
// (race kind 6, the stage's +0 not 1, [0x82814E24] 0): past 900 ticks the
// sign-in watch's state 1 and reason 6 when its state is 0; else the packet
// built in the frame (+176 on the first tick, +1456 after: type 23, the
// outcome, the id's bytes) and its sends (sub_8214AB48, sub_8214AA70)
// Dropped (the user's call, 2026-11-14), the tick counted; once the answer
// byte isn't 255: with the peer mode word 0 the last racer record's +3924
// (or +3920) and the outcome by it; then [0x82814E24] 1, the ticks and
// [0x826C6B04] 0. r3 the copy, the last send's packet (what the dropped
// sends leave) or the record. Otherwise as RaceResults::Draw on +24 (the
// transition), +36 (the step word), +12 (the page object) and +8 (the
// movie).
uint64_t SplitResultsDraw(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls);

// --- Kind 6's screen after the race (kind6_menu.cpp) ---
//
// The screen [0x82814E10] (360 bytes: +24 to +36 UI movies 17, 20, 21 and
// 14; +40 page object A with its step word +64, the menu and the steps; +68
// the cursor, +72 the cursor before, +76 the last move; +136 the replay
// slots' menu list; +208 page object B with its step word +232, a message
// box over A; +236 B's selection; +316 to +356 the screen's flags and
// message ids), read again before each use as the originals do; the word
// beside it [0x82814E14] (the answer); the network session [0x82816E3C]
// (+200 the race's time in milliseconds, +236 the upload's state); the
// replay list [0x828161F0] (+12 the 36-byte records, +20 their count); the
// course [0x82816660] and [0x82816680]. The UI movie calls (PlayNamed,
// LoopNamed, LoopLast, FinishNamed, FinishAll, Run, SetText, SetNodeText,
// ClearText, ClearNodeText, ShowNode, HideNode, ResetPose, the scroll
// setters), MenuList::Init, memcpy where its plain C++ refuses and
// MenuSound::Play through their overrides (`calls`); UiMovies::Find,
// UiMovie::FindNode and Child, ScreenPages::Init, Go and Finish,
// MenuList::Selection and the confirm dialog's draw by their plain C++.
// SetText's and SetNodeText's unset varargs are passed as 0 (the user's
// call for the parking area, 2026-10-30).
constexpr uint32_t kKind6Session = 0x82816E3C;
constexpr uint32_t kKind6Course = 0x82816660;
constexpr uint32_t kKind6CourseFlag = 0x82816680;
constexpr uint32_t kKind6Replays = 0x828161F0;

// sub_8221C168 Kind6Menu::Close: UiMovie::FinishAll on each of the screen's
// four movies (+24 to +36) that isn't 0. The last call's r3 (or the last
// movie word, 0).
uint64_t Kind6Close(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8221EC48 Kind6Menu::Open (r3 the screen, r4 the 24-byte block), from
// a 160-byte frame: the block copied in (memcpy), +4 capped at 59:59.999;
// UI movies 17, 20, 21 and 14 found (the id at the frame's +96) into +24 to
// +36, their poses reset when [0x82814E14] is 0 to 2; page objects A (+40)
// and B (+208) ScreenPages::Init(0); [0x82814E10]'s +336 = 0; with the word
// 0 to 2 +324 = 44, Go(A, 12, transition 0, then 1) and Go(B, 5, 5), else
// Go(A, 0); the fields cleared, MenuList::Init(+136, 4, 4, 1, 154, 153,
// 155, 156; 159, 158), more fields set; with the word 0 to 2 the session's
// +200 = +4 in milliseconds and the result's fields cleared (+348 = 3);
// +340 = 1, SetUpTexts, and with the word 0 to 2 HidePanels. The last
// call's r3.
uint64_t Kind6Open(uint8_t* base, uint32_t screen, uint32_t block, uint32_t sp,
                   const Calls& calls);
// sub_8221EF60 Kind6Menu::Draw (r3 the screen), from a 144-byte frame: A's
// transition (+52; 8 handlers stored at the frame's +80) with +64, then
// ScreenPages::Finish(A, its answer), or with none and [0x82814E10]'s +336
// set UiMovie::Run(+24, 0); the confirm dialog's draw on A's pages 3 to 5
// and 13; then B's transition (+220; its 7 handlers stored over the first
// 7, the eighth left) with +232 and Finish(B) (a negative one other than
// -1 Finish(B, 1)); with none, by B's page (+208) the run of movie 20 (1,
// 2), 21 (3, 4) or 14 (5) when +336 is set, then with [0x82814E10]'s +344
// set: the session's +236 = 3, +344 = 0, and by +348 (0 to 3) Go(B, 0,
// transition 1, 3 or 6) and Go(A, 11). r3 as the original leaves it.
uint64_t Kind6Draw(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls);

// The helpers. sub_8221E038 SetUpTexts (r3 the screen; its jump table's
// case 0, sub_8221E0FC, merged): movie 17's type-3 items 5 to 8 strings
// 1965, 1961, 1962 and 115; nodes -1/23 to 27 hidden (+80 = 0); ShowPanelA;
// the course's name (string 395, 396, 397, 398, 401, 402, 404 or 403 by
// [0x82816660] 0 to 7, else 0) at -1/21 and -1/22 by [0x82816680] (1 when
// it's 0); nodes -1/17 and -1/16 hidden; ShowCourse; ShowHelp. ShowHelp's
// r3. sub_8221D1A8 ShowCourse (its case 0, sub_8221D2B4, merged): the
// type-9 texts 55 to 58, the course's two marks of nodes 9/6 to 32 (two
// tables in its 240-byte frame), the named animations 47 to 65 finished
// and the course's (47 + its row) finished and played. PlayNamed's r3.
// sub_8221D130 ShowHelp: the type-2 text 0 string 967, its scroll speed
// (sub_8211C4F0: 300, the float at 0x82000A30) and ScrollText(1); the
// last's r3. sub_8221D050 ShowPanelA: type-1 nodes 4 and 5 shown, and when
// both exist their +108 = 0 and nodes 6 to 9 hidden. sub_8221CFC0
// HidePanels: type-1 nodes 4 to 9 hidden. Each the last call's r3.
uint64_t Kind6SetUpTexts(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls);
uint64_t Kind6ShowCourse(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6ShowHelp(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6ShowPanelA(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6HidePanels(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8221D7B0 SetCursorX (r3, unsigned): movie 17's type-3 node 0's float
// +16 = r3 (its doubleword left at the frame's +80). The node.
uint32_t Kind6SetCursorX(uint8_t* base, uint64_t value, uint32_t sp);
// sub_8221DF70 SetUpList: MenuList::Init(+136, the replays' count, at most
// 9 shown, 1, ..), movie 17's type-14 node 2's child 0's +16 = 40 x the
// selection (its doubleword at the frame's +96), ShowList, ShowReplay of
// the selected record; ShowReplay's r3. sub_8221D4E8 ShowList: the type-14
// node 2's children 5 to 13 cleared, string 472 (empty) or 471 (used) by
// each shown record's +28 from the scroll on, the scroll marks (nodes 7 and
// 8's +80); node 8. sub_8221CBD8 ShowReplay (r3 a record or 0): with +28
// set the date and time, kind, course, time and result texts and the
// counts' marks (nodes 5 and 6's +108), else the texts cleared and node 0
// hidden; the last call's r3 or node 6.
uint64_t Kind6SetUpList(uint8_t* base, uint32_t sp, const Calls& calls);
uint32_t Kind6ShowList(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6ShowReplay(uint8_t* base, uint32_t record, uint32_t sp, const Calls& calls);

// The transitions (r3 the step word: their first call's work when it's 0,
// the word 1, then the movie run), answering whether they're done (1, or
// the run's answer 0). A's: sub_8221C438 Intro, sub_8221C550 Show,
// sub_8221C5D8 RunToEnd, sub_8221DB48 MoveMain, sub_8221DC88 BackToQuit,
// sub_8221EB98 OpenList, sub_8221C628 CloseList, sub_8221DD78 MoveList. B's:
// sub_8221C6C8 OpenQuestion, sub_8221C7D8 CloseQuestion, sub_8221C888
// MoveQuestion, sub_8221CAB8 OpenMessage, sub_8221CB50 CloseMessage,
// sub_8221C920 OpenWaiting, sub_8221CA20 CloseWaiting. See the rows.
uint64_t Kind6Intro(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6Show(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6RunToEnd(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6MoveMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6BackToQuit(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6OpenList(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6CloseList(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6MoveList(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6OpenQuestion(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6CloseQuestion(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6MoveQuestion(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6OpenMessage(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6CloseMessage(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6OpenWaiting(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t Kind6CloseWaiting(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);


// --- Kind 6's update and pages (kind6_pages.cpp) ---
//
// The leaderboard read's results [0x826C6AD4] (an XUSER_STATS_READ_RESULTS:
// +4 the view, whose +4 is the total row count, +8 the rows read, +12 the
// 48-byte rows: +0 the XUID, +8 the rank, +24 the gamer tag, +44 the
// columns: +16 the time in milliseconds, +40 the car) and the read's spec
// word [0x826C72F4]. Menu::Step and Pressed, MenuList::Update, Accepted and
// Cancelled, MenuSound::Play, the UI movie calls, SaveSlots::State and
// SaveBuffers::Create and Destroy through their overrides (`calls`);
// RaceDialog::StartSave and ShowSaving by their plain C++;
// UiMovie::SetNodeCharText through its override; Kind6Menu::SaveReplay
// (with RaceDialog::AskSlot) and the Xbox Live work
// (the network manager's sub_8214B330 and sub_822917B8, the leaderboard
// read sub_82292CC0) the originals through `calls`; ScreenPages::Go,
// RaceDialog::Update and Open, ShowPanelA, SetUpList, FindNode and Child by
// their plain C++.
constexpr uint32_t kKind6Results = 0x826C6AD4;
constexpr uint32_t kKind6ReadSpec = 0x826C72F4;

// sub_822914A8 (the network manager's, the player's rank): the rows read
// searched for the session's XUID (+168, a doubleword); its rank (+8) less
// one, -1 without results or when it isn't there (64 bits, as the
// original's r3). sub_8225FFC8: a car's logo index (the leaderboard's car
// column: 25 cars to 1 to 25, others 0). sub_822927B0 (the network
// manager's state, r3 4109, 4111 or 4112; others nothing, r3 kept), from a
// 96-byte frame: sub_8214B330(state) (the original); 4111 then
// sub_822917B8(session) (the original, the session's start over Xbox
// Live); 4112 then [0x826C72F4] = the session's +188 and its +180 = 0.
uint64_t Kind6Rank(uint8_t* base);
uint32_t Kind6CarLogo(uint32_t car);
uint64_t Kind6NetState(uint8_t* base, uint64_t state, uint32_t sp, const Calls& calls);

// The helpers: sub_8221CEA0 ShowPanelB (type-1 nodes 4 and 5 shown with
// +108 0, then 6 and 7 with +108 1, 8 and 9 hidden), sub_8221D3D0
// ShowTimeOnly (node 16 hidden with +316 set, node 17 shown with the time
// (string 528), the ranking's rows 23 to 27 shown with placeholders 533,
// 534 and 531), sub_8221E1A0 ShowRanking (the rows read around the player,
// from a 192-byte frame), sub_8221D808 RankingTime (a leaf: row `row`'s
// time split into the words at `minutes`, `seconds` and `thousandths`,
// capped at 59:59.999, nothing written for 0; returns `row`). Each the r3
// the original leaves.
uint64_t Kind6ShowPanelB(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6ShowTimeOnly(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6ShowRanking(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6RankingTime(uint8_t* base, uint64_t row, uint32_t minutes, uint32_t seconds,
                          uint32_t thousandths);

// The pages (see the rows; each returns 0 but AnswerPage). A's: sub_8221E788
// MainPage, sub_8221C1E0 AnswerPage (a leaf), sub_8221DA60 SlotPage,
// sub_8221C238 SavePage, sub_8221E908 OverwritePage, sub_8221C2B8
// SavingPage, sub_8221C308 QuitPage (the four with `r4` passed to
// RaceDialog::Update), sub_8221E510 CheckRecordPage, sub_8221C008
// RankingReadPage, sub_8221E5E8 UploadPage, sub_8221D898 MenuBackPage,
// sub_8221D908 UploadResultPage, sub_8221BE90 RankingPage, sub_8221D9D8
// MessageBackPage. B's: sub_8221E980 QuestionPage, sub_8221C360
// QuestionAnswerPage, sub_8221EAE8 MessagePage, sub_8221C3F0 WaitingPage.
uint64_t Kind6MainPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6AnswerPage(uint8_t* base);
uint64_t Kind6SlotPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6SavePage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls);
uint64_t Kind6OverwritePage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls);
uint64_t Kind6SavingPage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls);
uint64_t Kind6QuitPage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls);
uint64_t Kind6CheckRecordPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6RankingReadPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6UploadPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6MenuBackPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6UploadResultPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6RankingPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6MessageBackPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6QuestionPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6QuestionAnswerPage(uint8_t* base, uint32_t sp);
uint64_t Kind6MessagePage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Kind6WaitingPage(uint8_t* base, uint32_t sp);

// sub_8221F1E8 Kind6Menu::Update (r3 the screen), from a 304-byte frame: the
// pages' tables stored (B's 6 at +80, A's 15 at +112); A's and B's pages
// read; with +352 set: +324 = 44, Go(A, 7), Go(B, 5, transition 5), +352
// and +320 = 0; then B's page's handler, then A's (r4 passed on to the
// dialog pages); A's r3, 0 with A's page -1. A page past its table runs the
// frame's word through `calls`, as the original's bctrl would.
uint64_t Kind6Update(uint8_t* base, uint32_t screen, uint64_t r4, uint32_t sp,
                     const Calls& calls);

}  // namespace race_screens
