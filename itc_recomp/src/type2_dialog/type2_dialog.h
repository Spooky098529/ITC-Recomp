// The type 2 mode's dialog natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/type2-dialog.md, the globals in
// docs/known-data.md): the machine at the type 2 mode block's +0x1E4C8, its
// four dialogs over UI movies 16 to 19 (two yes/no questions, two notices),
// their pages and steps. Plain C++ over guest memory (the machine, its three
// blocks and two words, the mode's message ring and the movies are shared
// with the recompiled code), with the originals' loads and stores in their
// order and their frames' words. `sp` is the caller's stack pointer: the
// back chain of the original's frame and its locals are written below it as
// the original leaves them; its register saves aren't.
// Adapters: src/hooks/type2_dialog.cpp (behind --native_type2_dialog),
// register steps shared with the harness in src/hooks/type2_dialog.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls) by address, to
// the callees' overrides: UiMovie::ResetPose, PlayNamed, Run, LoopLast,
// LoopNamed, FinishNamed, FinishAll, Menu::Step, Menu::Pressed,
// MenuSound::Play, VirtualAlloc and VirtualFree, and the pages and steps
// Draw and Update call through their tables. UiMovies::Find and IsLoaded are
// their plain C++.

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace type2_dialog {

using Calls = race_hud_object::Calls;

// The dialog's blocks: the page object (28 bytes: +0 the page Draw runs, -1
// while a step runs; +4 the page after the step; +8 the page before; +12
// the step Update runs, -1 none; +16 the step queued after it; +20; +24 the
// step's started word), the menu (68: a front-end menu, items 0 to 1) and
// the data (68: +0 the movies loaded, +4 the state queued, 21 none, +8 the
// movie shown).
constexpr uint32_t kPages = 0x826D83F8;
constexpr uint32_t kMenu = 0x826D83FC;
constexpr uint32_t kData = 0x826D8400;
// The dialog showing (0 to 3) and its status (-1 loading, 3 idle, 5 an
// open asked, 0 just shown, 4 asked by TakesInput, 1 taking input, 2
// cancelled).
constexpr uint32_t kShowing = 0x82636200;
constexpr uint32_t kStatus = 0x82636204;

// The machine's words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kResetPose = 0x8211ABA8;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kLoopLast = 0x8211DDE8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kMenuStep = 0x82186568;
constexpr uint32_t kMenuPressed = 0x821864C8;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;

// Posts the message (id, 0, 0) on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), the words in the order `order` names
// (0: id, +4, +8; 1: +4, +8, id), as the handlers do inline.
void Post(uint8_t* base, uint32_t mode, uint32_t id, bool id_last = false);

// --- The steps (steps.cpp; r3 the started word's address, the page
// object's +24) ---
//
// sub_82187728 OpenConfirmStep and sub_821879F8 OpenConfirm2Step (movies 16
// and 18): the first call MenuSound::Play(5) and the opening (5), then
// UiMovie::Run; done: the highlight {1, 2}[cursor] and LoopLast. Returns 1
// when done. 128-byte frames.
uint64_t OpenConfirmStep(uint8_t* base, uint32_t started, bool second, uint32_t sp,
                         const Calls& calls);
// sub_821877F8 CloseConfirmStep and sub_82187AC8 CloseConfirm2Step: the
// closing (6); done: FinishAll and the data's +4 = 9 (the other order for
// the second). Returns 1 when done. 112-byte frames.
uint64_t CloseConfirmStep(uint8_t* base, uint32_t started, bool second, uint32_t sp,
                          const Calls& calls);
// sub_821878B0 OpenNoticeStep, sub_82187948 CloseNoticeStep, sub_82187B80
// OpenNotice2Step and sub_82187C18 CloseNotice2Step (movies 17 and 19): the
// opening (2) or closing (3); a close done: the data's +4 = 9. Returns 1
// when done. 112-byte frames.
uint64_t NoticeStep(uint8_t* base, uint32_t started, bool second, bool close, uint32_t sp,
                    const Calls& calls);
// sub_82187CC8 MoveCursorStep and sub_82187D80 MoveCursor2Step: the first
// call the previous highlight finished, the new one played and looped
// (LoopLast, or LoopNamed for the second); then UiMovie::Run. Returns 1.
// 128-byte frames.
uint64_t MoveCursorStep(uint8_t* base, uint32_t started, bool second, uint32_t sp,
                        const Calls& calls);

// --- The pages (pages.cpp; r3 the mode) ---
//
// sub_82187160 ConfirmPage (144-byte frame) and sub_82187330 Confirm2Page
// (112): the menu stepped; accept (159) posts 38 (yes) or 39 (no), back
// (158) 41, each with its close step; a moved cursor its move step.
// `r3` the caller's (left when the page skips its input frame). The last
// call's r3.
uint64_t ConfirmPage(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t Confirm2Page(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82187508 NoticePage and sub_82187618 Notice2Page (112): accept or
// back posts 40 with the close step. The last call's r3.
uint64_t NoticePage(uint8_t* base, uint64_t r3, bool second, uint32_t sp, const Calls& calls);

// --- The machine's handlers (handlers.cpp) ---
//
// sub_82187E50 Draw (r3 the machine, r4 the mode): the page from its table
// (112-byte frame) with r3 = the mode. The page's r3, or `r3`.
uint64_t Draw(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_82187EC8 Update (r3 the machine, r4 the mode): the step from its table
// (208-byte frame), or UiMovie::Run; then the queued state. The step's,
// Run's r3 or the movie.
uint64_t Update(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_821869B8 OnEnter (r3 the machine): the three blocks VirtualAlloc'ed
// with their first words, state 1 or 9. The last VirtualAlloc's r3.
// 128-byte frame.
uint64_t OnEnter(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_821880D0 WaitForMovies (r3 the machine): movies 16 to 19 loaded, then
// state 9. IsLoaded's r3 when called, else `r3`. 96-byte frame.
uint64_t WaitForMovies(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82186490 Shown: the status 0. A leaf.
void Shown(uint8_t* base);
// sub_82186AD8 Wait (r3 the machine): state 9, the status 3. A leaf.
void Wait(uint8_t* base, uint32_t machine);
// sub_82186E40 Cancel (r4 the mode): the page showing closed with its back
// message, the status 2. 0 when one of dialogs 0 to 2 posted, else `r3`.
uint64_t Cancel(uint8_t* base, uint64_t r3, uint32_t mode);
// sub_82186C30 OpenConfirm and sub_82186D40 OpenConfirm2 (r3 the machine):
// movie 16 (reset) or 18 shown, the menu's cursor from the movie's +40960,
// the open step started (state 4) or state 1. ResetPose's r3, or the movie
// table pair's address for the second. 112-byte frames.
uint64_t OpenConfirm(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
uint64_t OpenConfirm2(uint8_t* base, uint32_t machine, uint32_t sp);
// sub_82186AF0 OpenNotice and sub_82186B90 OpenNotice2 (r3 the machine):
// movie 17 or 19. Leaves.
void OpenNotice(uint8_t* base, uint32_t machine, bool second);
// sub_82186428 OnLeft: the three blocks VirtualFree'd, the globals 0. The
// last VirtualFree's r3. 112-byte frame.
uint64_t OnLeft(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821864A0 TakesInput: the status 0 becomes 4; 1 when it is 1.
uint32_t TakesInput(uint8_t* base);

// --- The machine's set-up (init.cpp) ---
//
// sub_82188178 Type2Dialog::Init: 16 transitions added to `machine`
// (StateMachine::Add's plain C++) from functors built in its frame
// (`frame`, the original's stores in order). The last Add's r3 (an index
// when `replaced`, else the new transition).
constexpr uint32_t kInitFrame = 1168;
uint32_t Init(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

}  // namespace type2_dialog
