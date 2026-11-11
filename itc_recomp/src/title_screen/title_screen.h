// The type 4 front-end mode natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/title-screen.md, the globals in
// docs/known-data.md): the title screen the boot leads to. The mode's Init
// and delegate functions, the title load's machine (the type 4 block's
// +28920, UI movie 46) and the title screen's (+38560, UI movie 0) with
// their pages and transitions. Plain C++ over guest memory (the machines,
// their blocks, the mode's message ring and the movies are shared with the
// recompiled code), with the originals' loads and stores in their order and
// their frames' words. `sp` is the caller's stack pointer: the back chain of
// the original's frame and its locals are written below it as the original
// leaves them; its register saves aren't.
// Adapters: src/hooks/title_screen_native.cpp (behind
// --native_title_screen), register steps shared with the harness in
// src/hooks/title_screen_native.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls) by address, to
// the callees' overrides (the UI movie calls, VirtualAlloc and VirtualFree,
// BootData::Load, Archive::QueueLength, ScreenFades::FadeIn, and the pages
// and transitions the tables hold that aren't ours yet). UiMovies::Find and
// IsLoaded are their plain C++. The empty sub_82106540 (debug prints) the
// originals call is left out: it returns at once.

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace title_screen {

using Calls = race_hud_object::Calls;

// The title load's block (68 bytes: +4 a tick count, +8 the transition
// Update runs, -1 none; +12 the page Draw runs, -1 none; +16 the
// transition's started word).
constexpr uint32_t kLoadBlock = 0x82814E28;
// The title screen's blocks: the page object (28 bytes), the menu list (72),
// the title's words (68) and a front-end menu (68).
constexpr uint32_t kPages = 0x82814E2C;
constexpr uint32_t kList = 0x82814E30;
constexpr uint32_t kWords = 0x82814E34;
constexpr uint32_t kMenu = 0x82814E38;
// How the title was entered (1 from the boot).
constexpr uint32_t kEntered = 0x826364EC;
// The pad blocked (two words: the system UI, probably).
constexpr uint32_t kPadBlocked0 = 0x826C7960;
constexpr uint32_t kPadBlocked1 = 0x826C7964;
// The boot data's object BootData::Load gets.
constexpr uint32_t kBootData = 0x826C5B60;
// The fades' passes ScreenFades::FadeIn gets.
constexpr uint32_t kFadesPasses = 0x82001BEC;

// The UI movies and their assets.
constexpr uint32_t kLoadMovie = 46;
constexpr uint32_t kLoadAsset = 22592;
constexpr uint32_t kTitleMovie = 0;
constexpr uint32_t kTitleAsset = 22558;

// The machine's words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;
constexpr uint32_t kMachineDelete = 9616;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kMoviesOpen = 0x8211E808;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kBootDataLoad = 0x82106618;
constexpr uint32_t kQueueLength = 0x8209D7B0;
constexpr uint32_t kFadeIn = 0x8209D038;

constexpr uint32_t kResetPose = 0x8211ABA8;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kListRelease = 0x8220BBB0;
constexpr uint32_t kSoundPlay = 0x82175850;

// The sound manager and its slot 14 (IsCuePlaying), bytes into its vtable.
constexpr uint32_t kSoundManager = 0x826C5C80;
constexpr uint32_t kIsCuePlayingSlot = 56;
// The mode's system checks (four words OnResume clears).
constexpr uint32_t kSystemChecks = 0x826BAB40;

// --- The title load's machine (title_load.cpp) ---

// sub_82220BA8 TitleLoad::Create (r3 the machine): +9608 = 0; the 68-byte
// block VirtualAlloc'ed into [0x82814E28], its +0 = 0; +9604 = 1 or 9 by
// +9612. VirtualAlloc's r3. 112-byte frame.
uint64_t LoadCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_822204B0 TitleLoad::OpenMovie: UiMovies::Open(46, 22592) (a tail
// call, from the caller's frame with its return address). Its r3.
uint64_t LoadOpenMovie(uint32_t sp, uint32_t lr, const Calls& calls);
// sub_82220C18 TitleLoad::WaitForMovie (r3 the machine): while +9608 is 0,
// +9608 = 1 once movie 46 is loaded; once set, +9604 = 9 or 4 by +9612.
// IsLoaded's r3 when called, else `r3`. 96-byte frame.
uint64_t LoadWaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82220C88 TitleLoad::Start: BootData::Load([0x826C5B60]); the block's
// +4 = 0, +12 = -1, +8 = 12. Load's r3. 96-byte frame.
uint64_t LoadStart(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82221198 TitleLoad::Draw (r3 the machine, r4 the mode): the six
// pages' addresses at the frame's +80; unless the page (+12) is -1, the
// page called with r3 the mode (through the guest). Its r3, else `r3`.
// 112-byte frame.
uint64_t LoadDraw(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_82221228 TitleLoad::Update (r3 the machine, r4 the mode): the 13
// transitions' addresses at the frame's +80; the transition (+8) called with
// the block's +16 (through the guest) unless -1; with -1 and the pad
// blocked, +12 = 5, +16 = 0 and, once Archive::QueueLength is 0, message 5
// posted. The transition's r3, QueueLength's, or the table's eighth entry
// (the original's register left). 208-byte frame.
uint64_t LoadUpdate(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_822210C8 TitleLoad::PlayLogo (r3 the started word): movie 46; once,
// UiMovie::PlayNamed(movie, 0, ...) and ScreenFades::FadeIn, the word 1;
// UiMovie::Run(movie, 0); on 0, +8 = -1, +12 = 5, the word 0. Run's r3.
// 112-byte frame.
uint64_t LoadPlayLogo(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_822204C0 TitleLoad::WaitForArchive: once Archive::QueueLength is 0,
// +12 = 4. QueueLength's r3. 96-byte frame.
uint64_t LoadWaitForArchive(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82221078 TitleLoad::PostClose (r3 the mode): message 5 posted. A leaf.
void LoadPostClose(uint8_t* base, uint32_t mode);
// sub_82220EF8 TitleLoad::Unload (r3 the machine): UiMovie::FinishAll(movie
// 46); with +9616 set, UiMovies::Delete(46) and +9608 = 0. The last call's
// r3. 112-byte frame.
uint64_t LoadUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82220F60 TitleLoad::OpenTitle (r3 the machine, r4 the mode):
// [0x826364EC] = 1, message 8 posted, +9604 = 9. A leaf.
void LoadOpenTitle(uint8_t* base, uint32_t machine, uint32_t mode);
// sub_82220FC8 TitleLoad::Leave (r3 the machine, r4 the mode): +9604 = 7;
// when movie 46 is found, UiMovie::FinishAll and UiMovies::Delete(46);
// message 4 posted. Delete's r3, or the movie (0). 112-byte frame.
uint64_t LoadLeave(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                   const Calls& calls);
// sub_82220468 TitleLoad::Destroy: VirtualFree([0x82814E28]), the global 0.
// VirtualFree's r3. 96-byte frame.
uint64_t LoadDestroy(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The title screen's machine (screen_handlers.cpp) ---

// sub_82221D98 TitleScreen::Create (r3 the machine): +9608 = 0; the page
// object (28 bytes), the menu list (72), the menu (68) and the title's words
// (68) VirtualAlloc'ed into their globals with their first words; +9604 = 1
// or 9 by +9612. The last VirtualAlloc's r3. 112-byte frame.
uint64_t ScreenCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82221990 TitleScreen::OpenMovie: UiMovies::Open(0, 22558) (a tail
// call). Its r3.
uint64_t ScreenOpenMovie(uint32_t sp, uint32_t lr, const Calls& calls);
// sub_82221FC8 TitleScreen::WaitForMovie (r3 the machine): as the title
// load's with movie 0. 96-byte frame.
uint64_t ScreenWaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82223AB8 TitleScreen::Start: the title's +16 from [0x826364EC] (0 or
// 1, left above); SetUp (its plain C++); the sound manager's slot 14 for
// bank 1, cue 1 (frame words), Sound::Play(&1, &1) when not playing; the
// fade. The fade's r3. 112-byte frame.
uint64_t ScreenStart(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82222A50 TitleScreen::SetUp: movie 0's pose reset and its seven main
// menu texts (SetText's unset varargs 0); the page object to transition 0
// (from the boot) or 5. The last SetText's r3. 176-byte frame.
uint64_t ScreenSetUp(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82223A18 TitleScreen::Draw (r3 the machine, r4 the mode): the six
// pages' addresses at the frame's +80; unless the page (+0) is -1, the page
// called with r3 the mode, r4 the machine (through the guest). Its r3, else
// `r3`. 128-byte frame.
uint64_t ScreenDraw(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_822240B0 TitleScreen::Update (the transitions): movie 0; the
// transition (+12) from the 18-entry table at the frame's +96 called with
// the page object's +24 (through the guest), the queued one or the page
// after started on a nonzero answer; with none (-1), UiMovie::Run(movie,
// 0). The transition's or Run's r3, or the movie. 288-byte frame.
uint64_t ScreenUpdate(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_822219A0 TitleScreen::OnGameDataLoaded: the page object to page 2
// through transitions 4 and 6. A leaf.
void ScreenOnGameDataLoaded(uint8_t* base);
// sub_82221A78 TitleScreen::OnInterrupt and sub_82221A90 OnResume: the
// title's +40 = 1, or 0 with the system checks cleared. Leaves.
void ScreenOnInterrupt(uint8_t* base);
void ScreenOnResume(uint8_t* base);
// sub_822224D0 TitleScreen::Unload (r3 the machine): UiMovie::FinishAll
// (movie 0); with +9616 set, UiMovies::Delete(0) and +9608 = 0. The last
// call's r3. 112-byte frame.
uint64_t ScreenUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82222538 TitleScreen::OpenNext (r3 the machine, r4 the mode): message
// 10 posted when the list's selection is 6; +9604 = 9. Selection's r3.
// 112-byte frame.
uint64_t ScreenOpenNext(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp);
// sub_822225D8 TitleScreen::Leave (r3 the machine, r4 the mode): as the
// title load's with movie 0. 112-byte frame.
uint64_t ScreenLeave(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                     const Calls& calls);
// sub_82221EC0 TitleScreen::Destroy (r3 the machine): +9604 from the
// title's +4 (the list's selection for 0); MenuList::Release; the four
// blocks VirtualFree'd, three of the globals 0. The last VirtualFree's r3.
// 128-byte frame.
uint64_t ScreenDestroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82222688 TitleScreen::LoadGameData (page 1, r3 the mode): message 13
// posted; the page object to no page. A leaf.
void ScreenLoadGameData(uint8_t* base, uint32_t mode);
// sub_822229D0 TitleScreen::LeavePage (page 5, r3 the mode, r4 the
// machine): by the title's +4, message 7 posted (6) or +9604 = 10 (0 to 5,
// 8). A leaf.
void ScreenLeavePage(uint8_t* base, uint32_t mode, uint32_t machine);

// --- The machines' set-ups (init.cpp) ---
//
// sub_822213B0 TitleLoad::Init and sub_82224270 TitleScreen::Init: their
// transitions added to `machine` (StateMachine::Add's plain C++) from
// functors built in their frames (`frame`, the originals' stores in order).
// The last Add's r3 (an index when `replaced`, else the new transition).
constexpr uint32_t kLoadInitFrame = 912;
constexpr uint32_t kScreenInitFrame = 1232;
uint32_t LoadInit(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t ScreenInit(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

}  // namespace title_screen
