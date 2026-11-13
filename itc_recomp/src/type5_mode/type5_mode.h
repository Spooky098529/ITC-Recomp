// The type 5 front-end mode natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/type5-mode.md, the globals in
// docs/known-data.md): the parking area's mode. The mode's Init and delegate
// functions, its +32 interface, and its dialog's machine (the type 5 block's
// +19280, `Type5Dialog`: the yes/no questions and notices over UI movies 16
// to 19, a copy of the type 2 dialog's design, src/type2_dialog/) with its
// pages and steps. Plain C++ over guest memory (the machines, the dialog's
// three blocks, the mode's message ring and the movies are shared with the
// recompiled code), with the originals' loads and stores in their order and
// their frames' words. `sp` is the caller's stack pointer: the back chain of
// the original's frame and its locals are written below it as the original
// leaves them; its register saves aren't.
// Adapters: src/hooks/type5_mode.cpp (behind --native_type5_mode), register
// steps shared with the harness in src/hooks/type5_mode.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls) by address, to
// the callees' overrides: UiMovie::ResetPose, PlayNamed, Run, LoopLast,
// FinishNamed, FinishAll, Menu::Step, Menu::Pressed, MenuSound::Play,
// Sound::Play, VirtualAlloc and VirtualFree, the screen start,
// Strings::Load, Archive::WaitIdle, the parking area's and the car
// settings' Inits, ScreenFades::FadeIn, LoadingScreen::Show, Run and
// Finish, GameMessages::Dispatch, and the pages and steps Draw and Update
// call through their tables (two of the steps the type 2 dialog's).
// UiMovies::Find and IsLoaded are their plain C++. The empty sub_82106540
// (a debug print) Update calls first is left out: it returns at once.

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace type5_mode {

using Calls = race_hud_object::Calls;

// The dialog's blocks: the page object (28 bytes: +0 the page Draw runs, -1
// while a step runs; +4 the page after the step; +8 the page before; +12
// the step Update runs, -1 none; +16 the step queued after it; +20; +24 the
// step's started word), the menu (68: a front-end menu, items 0 to 1) and
// the data (68: +0 the movies loaded, +4 the state queued, 21 none, +8 the
// movie shown, +12 interrupted).
constexpr uint32_t kPages = 0x82814E3C;
constexpr uint32_t kMenu = 0x82814E40;
constexpr uint32_t kData = 0x82814E44;
// The parking area's machine (Resume's +900).
constexpr uint32_t kLocationScreen = 0x82814E50;
// How the title is entered (ForceExit's 0 and 1 set it).
constexpr uint32_t kEntered = 0x826364EC;

// The machines' words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;
constexpr uint32_t kMachineDelete = 9616;

// The mode's fields: the delegate's holder (+4) and the delegate (+8), the
// dispatcher (+36: the handlers, the count +200; its ring +204 is the
// mode's +240), the message ring (+240: 50 entries of 12 bytes, the count
// +840, the start +844) and the block (+848).
constexpr uint32_t kDispatcher = 36;
constexpr uint32_t kRing = 240;
constexpr uint32_t kBlock = 848;
// The block (28,940 bytes): the car settings at +0, the parking area at
// +9640, the dialog at +19280, the shown machine's id (+28920: 3, 21 or
// 16), +28924, and the next mode's 8-byte answer (+28928).
constexpr uint32_t kBlockBytes = 28940;
constexpr uint32_t kCarSettingsAt = 0;
constexpr uint32_t kLocationAt = 9640;
constexpr uint32_t kDialogAt = 19280;
constexpr uint32_t kShown = 28920;
constexpr uint32_t kFirstShown = 28924;
constexpr uint32_t kNextAnswer = 28928;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kResetPose = 0x8211ABA8;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kLoopLast = 0x8211DDE8;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kMenuStep = 0x82186568;
constexpr uint32_t kMenuPressed = 0x821864C8;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kSoundPlay = 0x82175850;
constexpr uint32_t kScreenStart = 0x8211AD00;
constexpr uint32_t kStringsLoad = 0x8210A8D0;
constexpr uint32_t kArchiveWaitIdle = 0x8209D730;
constexpr uint32_t kLocationInitStates = 0x8222F258;
constexpr uint32_t kCarSettingsInitType5 = 0x821B7878;
constexpr uint32_t kFadeIn = 0x8209D038;
constexpr uint32_t kLoadingShow = 0x822478D0;
constexpr uint32_t kLoadingRun = 0x82247898;
constexpr uint32_t kLoadingFinish = 0x82247A60;
constexpr uint32_t kDispatch = 0x821062E8;
// The string table Init loads from, and the fades' passes FadeIn gets.
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kFadesIn = 0x82001BEC;

// --- The dialog's steps (dialog_steps.cpp; r3 the started word's address,
// the page object's +24; each returns 1 when done, else 0) ---
//
// sub_822261E0 OpenConfirmStep (128-byte frame): movie 16; the first call
// MenuSound::Play(5) and the opening (5), then UiMovie::Run; done:
// ResetPose, the highlight {1, 2}[cursor] and LoopLast.
uint64_t OpenConfirmStep(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_822262B8 CloseConfirmStep (112): movie 16, the closing (6); done:
// FinishAll and the data's +4 = 9.
uint64_t CloseConfirmStep(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// The steps of one animation (112-byte frames): sub_82226370
// CloseNoticeStep (movie 17, 3), sub_82226420 OpenConfirm2Step (18, 5),
// sub_822264B8 CloseConfirm2Step (18, 6) and sub_82226568 CloseNotice2Step
// (19, 3); a close done: the data's +4 = 9.
enum class Simple { kCloseNotice, kOpenConfirm2, kCloseConfirm2, kCloseNotice2 };
uint64_t SimpleStep(uint8_t* base, uint32_t started, Simple which, uint32_t sp,
                    const Calls& calls);
// sub_82226618 MoveCursorStep (128): movie 16; the first call the previous
// highlight finished, the new one played and looped; then UiMovie::Run.
// Returns 1.
uint64_t MoveCursorStep(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_822266D0 Step9 (96): MenuSound::Play(5). Returns 1.
uint64_t Step9(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The dialog's pages (dialog_pages.cpp; r3 the dispatcher; each
// returns the last call's r3) ---
//
// sub_82225D10 ConfirmPage (144-byte frame; pages 0 and 2): the menu
// stepped; accept (159) posts 14 (yes) or 15 (no), back (158) 17, each with
// the close step 1; a moved cursor step 8; interrupted posts 20 with the
// close step.
uint64_t ConfirmPage(uint8_t* base, uint32_t dispatcher, uint32_t sp, const Calls& calls);
// sub_82225F30 NoticePage and sub_82226088 Notice2Page (144): accept or
// back posts 16 with the close step (3 or 7); interrupted posts 20.
uint64_t NoticePage(uint8_t* base, uint32_t dispatcher, bool second, uint32_t sp,
                    const Calls& calls);

// --- The dialog's handlers (dialog_handlers.cpp) ---
//
// sub_822266F8 Draw (r3 the machine, r4 the dispatcher; 112-byte frame): the
// page from its table with r3 = the dispatcher. The page's r3, or `r4`.
uint64_t Draw(uint8_t* base, uint64_t machine, uint64_t dispatcher, uint32_t sp,
              const Calls& calls);
// sub_82226788 Update (r3 the machine, r4 the dispatcher; 208-byte frame):
// the step from its table, or UiMovie::Run; then the queued state. The
// step's, Run's r3 or the movie.
uint64_t Update(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                const Calls& calls);
// sub_82225878 OnEnter (r3 the machine; 112-byte frame): the three blocks
// VirtualAlloc'ed with their first words, state 1 or 9. The last
// VirtualAlloc's r3.
uint64_t OnEnter(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82226978 WaitForMovies (r3 the machine; 96-byte frame): movies 16 to
// 19 loaded, then state 9. IsLoaded's r3 when called, else `r3`.
uint64_t WaitForMovies(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82225860 OnInterrupt: the data's +12 = 1. A leaf.
void OnInterrupt(uint8_t* base);
// sub_82225A90 OpenConfirm (r3 the machine; 112-byte frame): movie 16 reset
// and shown, the menu's cursor from the movie's +40960, the open step 0
// started (state 4) or state 1. ResetPose's r3.
uint64_t OpenConfirm(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82225B88 OpenConfirm2 (112): movie 18, step 4. The movie table pair's
// address.
uint64_t OpenConfirm2(uint8_t* base, uint32_t machine, uint32_t sp);
// sub_82225980 OpenNotice and sub_82225A08 OpenNotice2 (leaves): movie 17
// or 19, page 1 or 3, step 2 or 6.
void OpenNotice(uint8_t* base, uint32_t machine, bool second);
// sub_822257A8 OnLeft (112-byte frame): the three blocks VirtualFree'd, the
// globals 0. The last VirtualFree's r3.
uint64_t OnLeft(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The dialog's set-up (dialog_init.cpp) ---
//
// sub_82226A10 Type5Dialog::Init: 15 transitions added to `machine`
// (StateMachine::Add's plain C++) from functors built in its frame
// (`frame`, the original's stores in order). The last Add's r3 (an index
// when `replaced`, else the new transition).
constexpr uint32_t kDialogInitFrame = 1104;
uint32_t DialogInit(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

// --- The mode (mode.cpp) ---
//
// sub_8222FD68 ModeType5::Init (r3 the mode; 128-byte frame): see the row;
// the dialog's Init as plain C++, the other callees through their
// overrides. LoadingScreen::Show's r3.
uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_8222FD08 ModeType5::FindShown: the last of machines 3 (the car
// settings), 16 (the dialog) and 21 (the parking area) in state 4 or 5
// into the block's +28920. A leaf.
void FindShown(uint8_t* base, uint32_t block);
// The delegate functions (r3 the 8-byte answer, r4 the mode; each returns
// the answer): sub_82230050 WaitForLoads (128-byte frame), sub_82230458
// NextMode (160), sub_82230260 LeaveScreen (128), sub_82230168
// WaitForShown (112) and sub_82230328 Leaving (128).
void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void WaitForShown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
// The +32 interface (r3 the mode + 32): sub_8222FED0 Interrupt (0 or 1; a
// leaf), sub_8222FF60 ForceExit (r4 a number; a leaf writing below the
// stack pointer) and sub_8222FC38 Resume (a leaf).
uint32_t Interrupt(uint8_t* base, uint32_t object);
void ForceExit(uint8_t* base, uint32_t object, uint32_t number, uint32_t sp);
void Resume(uint8_t* base);
// sub_8222FC50 Mode::Slot14Thunk (r3 the answer, r4 an object): the
// object's vtable slot 14 (+56) tail-called with them, from the caller's
// frame `sp` with its return address `lr`: this mode's Leaving as plain
// C++, any other through the guest. The callee's r3.
uint64_t Slot14Thunk(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                     const Calls& calls);

}  // namespace type5_mode
