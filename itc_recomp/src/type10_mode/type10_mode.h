// The type 10 front-end mode natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/type10-mode.md, the globals in
// docs/known-data.md): the replay theater. The mode's Init and delegate
// functions, its +32 interface, the theater's machine (the type 10 block's
// +0, UI movie 45) with its pages, transitions and the replay list's
// helpers. Plain C++ over guest memory (the machines, the theater's blocks,
// the mode's message ring and the movie are shared with the recompiled
// code), with the originals' loads and stores in their order and their
// frames' words. `sp` is the caller's stack pointer: the back chain of the
// original's frame and its locals are written below it as the original
// leaves them; its register saves aren't.
// Adapters: src/hooks/type10_mode.cpp (behind --native_type10_mode), register
// steps shared with the harness in src/hooks/type10_mode.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls) by address, to
// the callees' overrides (the UI movie calls, VirtualAlloc and VirtualFree,
// the save layer, the menu list, MenuSound::Play, the fades, the screen
// start, Strings::Load, the save screen's Init, GameMessages::Dispatch,
// LoadingScreen::Run, and the pages and transitions the tables hold).
// Replay::SetUpRace sub_82105228 (never seen in play) stays recompiled and
// is called through the guest. UiMovies::Find, IsLoaded, UiMovie::FindNode,
// Child, MenuList::Selection and the previous selection are their plain C++.
// The empty sub_82106540 (a debug print) Update calls first is left out: it
// returns at once.

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace type10_mode {

using Calls = race_hud_object::Calls;

// The theater's blocks (schema/type10_mode.h's Type10ModeGlobals): the
// page object (pages, 28 bytes), the menu list (list, 72) and the theater's
// data (data, ReplayTheaterData, 16); the save layer's header
// (save_buffers: +12 the replays' 36-byte records, +20 their count) and the
// save screen's data (save_screen_data: +40 the replay picked, +56 its
// prompt state); how the title is entered (entered), the pad blocked
// (pad_blocked0 and 1), and the kept replay, the race data and the input
// recordings OnDone hands to Replay::SetUpRace.

// The UI movie and its asset.
constexpr uint32_t kMovie = 45;
constexpr uint32_t kAsset = 22591;

// The mode (ModeType10), its block (ModeType10Block, 19,300 bytes: the
// theater at +0, the save screen at +9640, the shown machine's index, the
// next mode's answer) and the theater's machine (ReplayTheaterMachine): their
// views in schema/type10_mode.h.
constexpr uint32_t kBlockBytes = 19300;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kMoviesOpen = 0x8211E808;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kClearText = 0x8211C470;
constexpr uint32_t kSetNodeText = 0x8211C7E8;
constexpr uint32_t kClearNodeText = 0x8211CC50;
constexpr uint32_t kFadeIn = 0x8209D038;
constexpr uint32_t kFadeOut = 0x8209D0C8;
constexpr uint32_t kSaveBuffersCreate = 0x8228F6B0;
constexpr uint32_t kSaveBuffersDestroy = 0x8228F8F8;
constexpr uint32_t kSlotsState = 0x8228F9E8;
constexpr uint32_t kListInit = 0x8220BAC0;
constexpr uint32_t kListUpdate = 0x8220C1E0;
constexpr uint32_t kListAccepted = 0x8220BD50;
constexpr uint32_t kListCancelled = 0x8220BDE0;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kSetUpRace = 0x82105228;
constexpr uint32_t kDispatch = 0x821062E8;
constexpr uint32_t kLoadingRun = 0x82247898;
constexpr uint32_t kScreenStart = 0x8211AD00;
constexpr uint32_t kStringsLoad = 0x8210A8D0;
constexpr uint32_t kSaveScreenInit = 0x821A7AC8;
// The fades' passes FadeIn and FadeOut get (fades_in, fades_out), and the
// fades' counts FadeOut waits on (fade_count0 and 1): globals.

// --- The mode (mode.cpp) ---

// sub_82288940 ModeType10::Init (r3 the mode): see the row; the theater's
// InitStates as plain C++, the other callees through their overrides.
// ScreenFades::FadeIn's r3. 112-byte frame.
uint64_t ModeInit(uint32_t mode, uint32_t sp, const Calls& calls);
// sub_8228AC48 ReplayTheater::InitStates: its 18 transitions added to
// `machine` (StateMachine::Add's plain C++) from functors built in its
// frame (`frame`, the original's stores in order). The last Add's r3 (an
// index when `replaced`, else the new transition).
constexpr uint32_t kInitStatesFrame = 1296;
uint32_t InitStates(uint32_t machine, uint32_t frame, bool* replaced);
// sub_822888F0 ModeType10::FindShown: the last of machines 0 (the theater)
// and 1 (the save screen) in state 4 or 5 into the block's +19280. A leaf.
void FindShown(uint32_t block);
// The delegate functions (r3 the 8-byte answer, r4 the mode; each returns
// the answer): sub_82288B70 WaitForLoads (112-byte frame), sub_82288E68
// NextMode (128), sub_82288DA0 LeaveScreen (128), sub_82288CA0
// WaitForShown (128), sub_822890D8 Leaving (128, never seen in play) and
// sub_8225D638 ModeType9::NextModeAnswer (a leaf, the type 9 mode's slot 12
// too; never seen in play).
void WaitForLoads(uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void NextMode(uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void LeaveScreen(uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void WaitForShown(uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void Leaving(uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void NextModeAnswer(uint32_t answer, uint32_t mode);
// The +32 interface (r3 the mode + 32), never seen in play: sub_82288A68
// Interrupt (0 or 1; 96-byte frame), sub_8225D828 ModeType9::ForceExit (r4
// a number; the doubleword built at sp - 16 as the original; a leaf, the
// type 9 mode's too) and sub_82288B20 Resume (a leaf).
uint32_t Interrupt(uint32_t object, uint32_t sp);
void ForceExit(uint32_t object, uint32_t number, uint32_t sp);
void Resume(uint32_t object);
// sub_822893E8 ReplayTheater::OnList: 1 when the page is 0 (the list). A
// leaf, never seen in play.
uint32_t OnList();

// --- The theater's machine (theater.cpp; r3 the machine, r4 the
// dispatcher where read) ---

// sub_82289408 Create: +9608 = 0; the page object (28), the list (72) and
// the data (16) VirtualAlloc'ed into their globals with their first words;
// +9604 = 1 or 9 by +9612. The last VirtualAlloc's r3. 112-byte frame.
uint64_t Create(uint32_t machine, uint32_t sp, const Calls& calls);
// sub_822891F8 OpenMovie: UiMovies::Open(45, 22591) (a tail call, from the
// caller's frame with its return address). Its r3.
uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls);
// sub_82289570 WaitForMovie: while +9608 is 0, +9608 = 1 once movie 45 is
// loaded; once set, +9604 = 9 or 4 by +9612. IsLoaded's r3 when called,
// else `r3`. 96-byte frame.
uint64_t WaitForMovie(uint64_t r3, uint32_t sp);
// sub_8228AA58 Start: SaveBuffers::Create, StartPage; the page object to
// page 1 through transition 0. SetPrompts' r3. 96-byte frame.
uint64_t Start(uint32_t sp, const Calls& calls);
// sub_8228A508 StartPage: SetPrompts(-1 x 6); the data's +0 = 1, +8 = 0.
// SetPrompts' r3. 96-byte frame.
uint64_t StartPage(uint32_t sp, const Calls& calls);
// sub_8228A868 Draw: the six pages' addresses at the frame's +80; unless the
// page (+0) is -1, the page called with r3 the dispatcher (through the
// guest). Its r3, else `r3`. 112-byte frame.
uint64_t Draw(uint64_t r3, uint64_t dispatcher, uint32_t sp, const Calls& calls);
// sub_8228A8F8 Update (the transitions): movie 45; the transition (+12)
// from the 7-entry table at the frame's +96 called with the page object's
// +24 (through the guest), the queued one or the page after started on a
// nonzero answer; with none (-1), UiMovie::Run(movie, 0) unless the page is
// 3. The transition's or Run's r3, or the movie. 144-byte frame.
uint64_t Update(uint32_t sp, const Calls& calls);
// The save screen's replies and the interrupts: sub_8228AAB0 OnDone
// (96-byte frame; BuildList as plain C++, Replay::SetUpRace through the
// guest; their r3, else `r3`), sub_82289208 OnFailed, sub_8228ABE0 OnRetry
// (96-byte frame; BuildList's r3, else `r3`), sub_82289308 OnBack,
// sub_82289358 OnInterrupt and sub_822893A8 OnResume (leaves). Only
// OnFailed seen in play.
uint64_t OnDone(uint64_t r3, uint32_t sp, const Calls& calls);
void OnFailed();
uint64_t OnRetry(uint64_t r3, uint32_t sp, const Calls& calls);
void OnBack();
void OnInterrupt();
void OnResume();
// sub_82289600 Unload: UiMovie::FinishAll(movie 45); with +9616 set,
// UiMovies::Delete(45) and +9608 = 0. The last call's r3. 112-byte frame.
uint64_t Unload(uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82289668 Wait: message 4 posted on the dispatcher, +9604 = 9. A leaf.
void Wait(uint32_t machine, uint32_t dispatcher);
// sub_822896C0 Leave: +9604 = 7; SaveBuffers::Destroy, UiMovie::FinishAll,
// UiMovies::Delete(45); message 4 posted. Delete's r3. 112-byte frame.
uint64_t Leave(uint32_t machine, uint32_t dispatcher, uint32_t sp,
               const Calls& calls);
// sub_822894E8 Destroy: +9604 = 11 (a replay loaded) or 14; the three blocks
// VirtualFree'd and their globals 0. The last VirtualFree's r3. 128-byte
// frame.
uint64_t Destroy(uint32_t machine, uint32_t sp, const Calls& calls);

// --- The pages (pages.cpp; r3 the dispatcher) ---

// sub_82289768 ListPage (never seen in play): the save device lost (message
// 9), MenuList::Update, a move to transition 4, accept to page 2 through
// transition 6 (the save screen's +40 the replay), back to page 5 (message
// 9); MenuSound::Play. The sound's r3 or Cancelled's 0. 144-byte frame.
uint64_t ListPage(uint32_t dispatcher, uint32_t sp, const Calls& calls);
// sub_82289958 AskList (message 7), sub_822899E0 AskLoad (message 8, never
// seen in play): to page 5, the data's +4 = 0 or 1. sub_82289A70 LeavePage:
// message 6. Leaves.
void AskList(uint32_t dispatcher);
void AskLoad(uint32_t dispatcher);
void LeavePage(uint32_t dispatcher);
// sub_8228A4B0 WaitPage: by the save screen's +56, SetPrompts(-1 x 6) (0) or
// SetPrompts(0, 0, -1 x 4) (1) as tail calls (from the caller's frame);
// others nothing. SetPrompts' r3, else `r3`.
uint64_t WaitPage(uint64_t r3, uint32_t sp, const Calls& calls);
// sub_8228A3B8 SetPrompts: three pairs (a[0], b[0]) to (a[2], b[2]): each
// pair's two nodes (sub-scene 2's children +100 and +100 + 1 of child i)
// shown with the values, or hidden with a -1 among them. The last FindNode.
// 176-byte frame.
uint64_t SetPrompts(const uint32_t a[3], const uint32_t b[3], uint32_t sp,
                    const Calls& calls);

// --- The transitions and the list's helpers (transitions.cpp) ---
//
// Each transition takes the page object's +24 (its started word): set up
// once while it's 0 (then 1), then UiMovie::Run(movie 45, 0); the answer
// Update reads (1 done).

// sub_82289AC0 Intro (0): PlayNamed 52; on Run's 0, 49, 43 and 0 played
// and looped; 1. 112-byte frame.
uint64_t Intro(uint32_t started, uint32_t sp, const Calls& calls);
// sub_82289BD0 FadeOut (1): the row, 49, 43 and 0 finished, the fade out;
// 1 once the fades' counts are 0. 112-byte frame.
uint64_t FadeOut(uint32_t started, uint32_t sp, const Calls& calls);
// sub_82289CC0 FadeInList (2): PlayNamed 50, the fade in; Run answered 0.
// sub_82289D70 ShowCursor (3): the row played and looped; 1 (128-byte
// frame). sub_8228A658 MoveCursor (4): see the row (176-byte frame).
// sub_82289E08 HideCursor (5): the row finished, PlayNamed 51; Run answered
// 0. sub_82289EB8 PickReplay (6): the row finished; 1. Never seen in play;
// 112-byte frames but where said.
uint64_t FadeInList(uint32_t started, uint32_t sp, const Calls& calls);
uint64_t ShowCursor(uint32_t started, uint32_t sp, const Calls& calls);
uint64_t MoveCursor(uint32_t started, uint32_t sp, const Calls& calls);
uint64_t HideCursor(uint32_t started, uint32_t sp, const Calls& calls);
uint64_t PickReplay(uint32_t started, uint32_t sp, const Calls& calls);
// sub_8228A560 BuildList (never seen in play): MenuList::Init over the
// replays, the cursor, ShowListPage, ShowRecord, SetPrompts(0, 0, 1, 1, -1,
// -1). SetPrompts' r3. 144-byte frame.
uint64_t BuildList(uint32_t sp, const Calls& calls);
// sub_82289F48 ShowListPage (never seen in play): the nine rows' names from
// the scroll, the scroll arrows. The last FindNode. 176-byte frame.
uint64_t ShowListPage(uint32_t sp, const Calls& calls);
// sub_8228A0D8 ShowRecord (never seen in play): the record's texts, or
// cleared with none. The last FindNode's or ClearText's r3. 192-byte frame.
uint64_t ShowRecord(uint32_t record, uint32_t sp, const Calls& calls);

}  // namespace type10_mode
