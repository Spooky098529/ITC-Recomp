// The type 8 front-end mode natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/type8-mode.md, the fields in
// docs/known-data.md: "type 8 mode", "type 8 first screen and dialog"): the
// mode's slots, its +32 interface, its first screen (Type8Screen0) and its
// dialog (Type8Dialog). Plain C++ over guest memory (the mode, its member
// and machines are shared with the recompiled code), with the originals'
// loads and stores in their order. Each function takes the stack pointer the
// original is entered with (`sp`); the back chain of an original's frame is
// written as it leaves it, its register saves aren't. Guest calls go through
// type67_modes::Calls (hooks::Type67Calls), by address to the callees'
// overrides. Each returns r3 as the original leaves it. Adapters:
// src/hooks/type8_mode.cpp (behind --native_type8_mode), register steps
// shared with the harness in src/hooks/type8_mode.h.

#pragma once

#include "type67_modes/type67_modes.h"
#include "type67_screens/type67_screens.h"

#include <cstdint>

namespace type8_mode {

using Calls = type67_modes::Calls;

// The asking player the confirms set (Type8Dialog::OpenConfirm reads it).
constexpr uint32_t kDialogPlayer = 0x82814E6C;

// --- The dialog (dialog.cpp): r3 the dialog (the member's +9792, its
// machine at +0: the state +9604); r4 the dispatcher where read ---

// sub_8223A408 Type8Dialog::OnEnter (a leaf): its words reset, the state 1
// when +9612 is set, else 9. r3 left.
uint64_t DialogOnEnter(uint8_t* base, uint64_t r3);
// sub_8223A398 Type8Dialog::OpenMovies (96-byte frame): UiMovies::Open of
// movies 16 to 19 (assets 22559 to 22562) and 20 (22587). Open's r3.
uint64_t DialogOpenMovies(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8223ADC0 Type8Dialog::WaitForMovies (96): with +9608 0, it = 1 once
// movies 16 to 20 are loaded (UiMovies::IsLoaded's plain C++, stopping at
// the first not); else +9736 = 1 and the state 9. r3 as left.
uint64_t DialogWaitForMovies(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8223AC00 Type8Dialog::Draw (96): the page (+9640)'s handler from the
// two-entry table in the frame (ConfirmPage, NoticePage; a guest call with
// r3 and r4), nothing for page -1. The page's r3, or r3 left.
uint64_t DialogDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_8223AC58 Type8Dialog::Update (160): as Type6Dialog::Update with its
// own step 0 (OpenConfirmStep) and the closed message 13, no shown word.
uint64_t DialogUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                      const Calls& calls);
// sub_8223A510 Type8Dialog::OpenConfirm (112): movie 16 (+9744) reset
// (UiMovie::ResetPose), the asking player to +9732, the cursors, the pages
// cleared, state 4 through step 0 when loaded, else 1. ResetPose's r3.
uint64_t DialogOpenConfirm(uint8_t* base, uint32_t dialog, uint32_t sp);
// sub_8223A4A0 Type8Dialog::OpenNotice (a leaf): movie 19, page 1 through
// step 2 when loaded (state 4), else state 1. r3 left.
uint64_t DialogOpenNotice(uint8_t* base, uint64_t r3);
// sub_8223A5F0 Type8Dialog::DrawWaiting (128): with a reopen queued
// (+9748), +9748 = 0 and OpenConfirm's work without the player; ResetPose's
// r3, else r3 left.
uint64_t DialogDrawWaiting(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8223A6B8 Type8Dialog::ConfirmPage (112): Menu::Step and Pressed's
// plain C++ on +9668; accept (159): MenuSound 0, the close step 1, message 9
// (cursor 0) or 10; back (158): MenuSound 2, step 1, message 12; moved
// (+9676): MenuSound 4, the cursor step 4. MenuSound's r3, else Pressed's.
uint64_t DialogConfirmPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                           const Calls& calls);
// sub_8223A858 Type8Dialog::NoticePage (112): Menu::Step; on 159 or 158
// (Pressed(159) asked again), MenuSound 0 or 2, step 3 with page 1 next,
// message 11. MenuSound's r3, else Pressed's.
uint64_t DialogNoticePage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                          const Calls& calls);
// sub_8223A940 Type8Dialog::OpenConfirmStep (128): Dialog::OpenConfirmStep
// with a UiMovie::ResetPose before the cursor's highlight. 1 when done,
// else 0.
uint64_t DialogOpenConfirmStep(uint8_t* base, uint32_t dialog, uint32_t sp, const Calls& calls);

// --- The first screen (screen0.cpp): r3 the screen (the member's +0, its
// machine at +4: the state +9608); r4 the dispatcher where read. The
// functions taking ScreenCalls set texts through its set_text and free the
// menu list through type67_screens::ReleaseList ---

using ScreenCalls = type67_screens::Calls;

// sub_8223D2F0 Type8Screen0::Type8Screen0 (96-byte frame):
// ScreenMachine::ScreenMachine's plain C++ on +4, the vtable 0x82010B74, the
// two players' stand blocks (+9752, +9772) and their animation ids, +9644 =
// 1. r3 left.
uint64_t Screen0Construct(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8223BF00 ~Type8Screen0: the vtable, then MenuList::Release(+9680) (a
// tail call from `sp` returning to `lr`). Its r3.
uint64_t Screen0Destruct(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                         const ScreenCalls& calls);
// sub_8223C528 Type8Screen0::DeletingDestructor (112): the vtable,
// MenuList::Release(+9680), operator delete (a guest call) when bit 0 of
// `flags` is set. r3 left.
uint64_t Screen0DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                 const ScreenCalls& calls);
// sub_8223B630 Type8Screen0::OnEnter (144): its words reset, MenuList::Init
// on +9680 (5, 5, 1, 154, 153, 155, 156, 159, 158; its plain C++ where it
// frees natively, else a guest call) and SetSelection(0), the state 1 when
// +9616 is set, else 9. r3 the list (r3 + 9680).
uint64_t Screen0OnEnter(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls);
// sub_8223B530 Type8Screen0::OpenMovie: UiMovies::Open(24, 22583), a tail
// call from `sp` returning to `lr`. Its r3.
uint64_t Screen0OpenMovie(uint32_t sp, uint32_t lr, const ScreenCalls& calls);
// sub_8223B6D8 Type8Screen0::WaitForMovie (96): with +9612 0, once movie 24
// is loaded, +9612 = 1 and the state 4 after the race (the mode before 1),
// else 9. IsLoaded's r3, else r3 left.
uint64_t Screen0WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8223B540 Type8Screen0::Suspend (96): with +9620 set, movie 24
// deleted and +9612 = 0. Delete's r3, else r3 left.
uint64_t Screen0Suspend(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls);
// sub_8223B590 PickYes, sub_8223B5A0 PickNo, sub_8223B5B0 PickBack (leaves):
// +9648 = 1, 0, 2. r3 left.
uint64_t Screen0Pick(uint8_t* base, uint64_t r3, uint32_t answer);
// sub_8223BD20 Type8Screen0::ShowPage (128; `a`, `b`): movie 24's
// sub-scene 6 nodes shown, hidden and their textures set by the two flags.
// The last node.
uint64_t Screen0ShowPage(uint8_t* base, uint64_t a, uint64_t b, uint32_t sp);
// sub_8223C098 Type8Screen0::OnDialogClosed (a leaf): +9644 = 1; unless the
// answer (+9648) is 1, ShowPage(1, 0) (a tail call from `sp`). Its r3, else
// r3 left.
uint64_t Screen0OnDialogClosed(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8223BA48 Type8Screen0::Leave (112): movie 24 finished
// (UiMovie::FinishAll, a guest call) and deleted when found, message 4
// posted. Delete's r3, or 0 with no movie.
uint64_t Screen0Leave(uint8_t* base, uint32_t dispatcher, uint32_t sp,
                      const ScreenCalls& calls);
// sub_8223B588 Type8Screen0::OnLeft: MenuList::Release(+9680), a tail call
// from `sp` returning to `lr`. Its r3.
uint64_t Screen0OnLeft(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                       const ScreenCalls& calls);
// sub_8223C430 Type8Screen0::Confirm (128; r5 the dispatcher): movie 16's
// texts (-1, 0) 27, (1, 3) 2, (1, 4) 3 (set_text, r7 to r10 0), its +40960
// = 1, the asking player 0, message 7 posted, +9648 = -1, +9644 = 0, then
// ShowPage(1, 1). ShowPage's r3.
uint64_t Screen0Confirm(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                        const ScreenCalls& calls);
// sub_8223B5C0 Type8Screen0::ConfirmPage (a leaf): by the answer (+9648):
// 0 or 2 back to page 0, 1 the fade out step 3 to page 3, else nothing. r3
// left.
uint64_t Screen0ConfirmPage(uint8_t* base, uint64_t r3);
// sub_8223C0C0 Type8Screen0::ChosenPage (112): by the menu's selection,
// state 9 and message 16, 14, 19 or 21 posted (the selection's r3), else
// ShowPage(0, 0) and state 6 (ShowPage's r3).
uint64_t Screen0ChosenPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp);
// sub_8223BE88 Type8Screen0::PlaceCursor (112; r4 an item): movie 24's node
// (0, 0) +16 = float(40 * item) (the doubleword through the frame's +80).
// The node.
uint64_t Screen0PlaceCursor(uint8_t* base, uint64_t item, uint32_t sp);
// sub_8223C888 Type8Screen0::Draw (128): the page table at 0x82814E94
// built on first use (its guard bit 0 at 0x82814EB4; the pairs in the
// frame's +80 to +111), then page +9652's entry on the screen plus its
// adjustment and r4 (MenuPage's, ConfirmPage's and ChosenPage's plain C++, others guest
// calls), nothing for page -1. The page's r3, or r3 left.
uint64_t Screen0Draw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                     const ScreenCalls& calls);

// --- The menu page, steps, update and show (screen0_update.cpp) ---

// sub_8223C6B8 Type8Screen0::MenuPage (144; r4 the dispatcher): while the
// menu takes input (+9644), MenuList::Update, then accepted (MenuSound 0;
// item 4 Confirm and page 1, else the fade out step 3 to page 3), cancelled
// (MenuSound 2; unless on item 4, the cursor there and step 2) or moved
// (+9704: MenuSound 4, step 2); then the item's help (SetText(24, 5, 0,
// 914 + item), ScrollText, and the scroll reset when moved). The last call's
// r3, or r3 left.
uint64_t Screen0MenuPage(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const ScreenCalls& calls);
// The steps (r3 the screen plus its adjustment): sub_8223C1A0 OpeningStep
// (128; done once Run answers 0 and both fades' counts are 0),
// sub_8223BBC8 ItemStep (128; 1), sub_8223C330 CursorStep (128; 1),
// sub_8223BAF0 FadeOutStep (112; 0 while the menu is off, else done once
// both fades' counts are 0).
uint64_t Screen0OpeningStep(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls);
uint64_t Screen0ItemStep(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls);
uint64_t Screen0CursorStep(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls);
uint64_t Screen0FadeOutStep(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls);
// sub_8223B808 Type8Screen0::UpdateStand (4,096-byte frame): the two
// players' stage models (at most one GarageStage::ReloadModel a call, then a
// Car at the frame's +96 set on the stage), their loading animations and
// stage commands, GarageStages::DrawAll's plain C++ (its r3).
uint64_t Screen0UpdateStand(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls);
// sub_8223C588 Type8Screen0::Update (160): the step table at 0x82814E70
// built on first use (its guard bit 0 at 0x82814E90), the step (+9664) run
// (the steps' plain C++, others guest calls) and the page block moved on
// when it answers nonzero, else UiMovie::Run; then UpdateStand (its r3).
uint64_t Screen0Update(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls);
// sub_8223B760 Type8Screen0::SetTexts (160; r3 not read): movie 24 reset,
// the menu's five labels (SetText(m, 0, 5 + i, ...)), named 2 to 6
// finished. The last FinishNamed's r3.
uint64_t Screen0SetTexts(uint8_t* base, uint32_t sp, const ScreenCalls& calls);
// sub_8223BC68 Type8Screen0::CarCard (144; r3 not read; r4 the player, r5
// the car's model id, r6 its spec's byte +769): three SetNodeText on the
// player's node's children 6, 10 and 8. The last one's r3.
uint64_t Screen0CarCard(uint8_t* base, uint64_t player, uint64_t model, uint64_t maker,
                        uint32_t sp, const ScreenCalls& calls);
// sub_8223BF18 Type8Screen0::Show (176): SetTexts, the page block to step 0
// with step 1 queued, both stand blocks' words cleared and their animations
// finished, the car cards, the music (the sound manager's slots 14 and 13,
// Sound::Play's plain C++). The last call's r3.
uint64_t Screen0Show(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls);

}  // namespace type8_mode
