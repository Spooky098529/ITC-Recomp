// The front end's small screen helpers natively (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/front-end-screen-helpers.md): the 852-byte modes' releases
// (vtable slot 1 of types 2, 4, 5, 9, 10 and 12), the save screen's and the
// options' dialog text setters with the steps above them, and the car cards.
// Plain C++ over guest memory (the modes, the movies and the globals are
// shared with the recompiled code), with the originals' loads and stores in
// their order and their frames' words. `sp` is the caller's stack pointer:
// the back chain of the original's frame and its locals are written below
// it as the original leaves them; its register saves aren't. Adapters:
// src/hooks/front_end_screen_helpers.cpp.
//
// The guest calls go through `Calls` (race_hud_object::Calls: r3 to r6 as
// given, r7 to r10 0) by address, to the callees' overrides: the UI movie
// calls (UiMovie::SetNodeText, SetNodeTextV, SetNodeNumberText,
// ClearNodeText, PlayNamed, LoopNamed, Run), MenuSound::Play,
// Strings::Release, the jump to UiMovies::DeleteAll and the sound manager's
// IsCuePlaying and StopCue (slots 14 and 13, through its vtable, as the
// race sounds' native code reaches them). UiMovies::Find, UiMovie::FindNode
// and UiMovie::Child are their plain C++. The varargs SetNodeText calls
// here pass 0 for r6 and on, which the originals leave as the previous call
// leaves them (the user's call for the parking area, 2026-10-30: only dead
// words of the caller's parameter area differ).

#pragma once

#include <cstdint>
#include <optional>

#include "race_hud_object/race_hud_object.h"

namespace front_end_screen_helpers {

using Calls = race_hud_object::Calls;

// The sound manager and its vtable's slots (bytes into it).
constexpr uint32_t kSoundManager = 0x826C5C80;
constexpr uint32_t kStopCueSlot = 52;       // 13
constexpr uint32_t kIsCuePlayingSlot = 56;  // 14

// The callees by address.
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kStringsRelease = 0x8210A538;
constexpr uint32_t kDeleteAllJump = 0x8211E370;
constexpr uint32_t kStandSceneRelease = 0x82180738;
constexpr uint32_t kSetNodeText = 0x8211C7E8;
constexpr uint32_t kSetNodeTextV = 0x8211CBD8;
constexpr uint32_t kSetNodeNumberText = 0x8211CA18;
constexpr uint32_t kClearNodeText = 0x8211CC50;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;

// The save screen's data ([0x826D8454]) and the front end's car data
// ([0x826D83F4]: +12, +36 words, +48 and +52 the list's index, +1136 the
// car entries).
constexpr uint32_t kSaveData = 0x826D8454;
constexpr uint32_t kCarData = 0x826D83F4;

// The mode's block (VirtualFree'd by every release).
constexpr uint32_t kModeBlock = 848;

// --- The modes' releases (releases.cpp) ---
//
// Slot 1 of the modes' vtables, r3 the mode. Each frees the mode's block
// (+848, the allocation layer's pages, VirtualFree's work) and sets +848 =
// 0, stops the menu music's cues of bank 1 (IsCuePlaying then StopCue, the
// indexes as words on the frame), releases the mode's string files
// (Strings::Release) and deletes every UI movie (the jump sub_8211E370).
// nullopt, with nothing done, where the block isn't the allocation layer's
// pages (the adapter runs the original). r3 as the original leaves it.

// sub_82196EC0 ModeType2::Release: cues 0 and 2, StandScene::Release (its
// plain C++, a guest call to its override where that refuses), files
// 0x100BB. 144-byte frame.
std::optional<uint64_t> ModeType2Release(uint8_t* base, uint32_t mode, uint32_t sp,
                                         const Calls& calls);
// sub_82224AB8 ModeType4::Release: files 3, the movies deleted, then
// StopCue(bank 1, cue 1) unconditionally. 112-byte frame.
std::optional<uint64_t> ModeType4Release(uint8_t* base, uint32_t mode, uint32_t sp,
                                         const Calls& calls);
// sub_8222FB90 ModeType5::Release: cue 3, files 59. 128-byte frame.
std::optional<uint64_t> ModeType5Release(uint8_t* base, uint32_t mode, uint32_t sp,
                                         const Calls& calls);
// sub_8225D590 ModeType9::Release: cue 2, files 171. 128-byte frame.
std::optional<uint64_t> ModeType9Release(uint8_t* base, uint32_t mode, uint32_t sp,
                                         const Calls& calls);
// sub_82288898 ModeType10::Release: files 11, no music. 96-byte frame.
std::optional<uint64_t> ModeType10Release(uint8_t* base, uint32_t mode, uint32_t sp,
                                          const Calls& calls);
// sub_82277DB0 ModeType12::Release: StandScene::Release (after the block's
// free), files 59, no music. 96-byte frame.
std::optional<uint64_t> ModeType12Release(uint8_t* base, uint32_t mode, uint32_t sp,
                                          const Calls& calls);

// --- The dialogs' text setters (dialogs.cpp) ---
//
// r3 a string id, `varargs` the original's r4 to r10 (stored in the
// caller's parameter area, `sp` + 24 to + 79, as the originals' prologues
// do; the va_list's address sp + 24).

// sub_821A51C8 SaveScreen::SetText: UI movie 44's FindNode(0, 1): child 0's
// text string `id` with the varargs (SetNodeTextV), Child(n, 1)'s children 3
// and 4 strings 2 and 3 (SetNodeText), then its child 8's +80 = 1. The last
// SetNodeText's r3. 144-byte frame.
uint64_t SaveScreenSetText(uint8_t* base, uint64_t id, const uint64_t varargs[7], uint32_t sp,
                           const Calls& calls);
// sub_821A52C0 SaveScreen::SetNote: movie 44's Child(FindNode(0, 2), 1) set
// by SetNodeTextV. Its r3. 112-byte frame.
uint64_t SaveScreenSetNote(uint8_t* base, uint64_t id, const uint64_t varargs[7], uint32_t sp,
                           const Calls& calls);
// sub_821A5360 SaveScreen::SetMessage: as SetText with one answer: child 3
// string 4, child 4's text cleared (ClearNodeText), child 8's +80 = 0.
// ClearNodeText's r3. 144-byte frame.
uint64_t SaveScreenSetMessage(uint8_t* base, uint64_t id, const uint64_t varargs[7], uint32_t sp,
                              const Calls& calls);
// sub_821C0D70 Options::SetSaveDialogText and sub_821C0BF0
// Options::SetCategory6DialogText: as SaveScreen::SetText on UI movie 7's
// sub-scene 13 or 14, without the child 8 word. 144-byte frames.
uint64_t OptionsSetDialogText(uint8_t* base, uint32_t sub_scene, uint64_t id,
                              const uint64_t varargs[7], uint32_t sp, const Calls& calls);
// sub_821C0E50 Options::SetSaveDialogNote and sub_821C0CD0
// Options::SetCategory6DialogNote: movie 7's Child(FindNode(13 or 14, 2), 1)
// set by SetNodeTextV. 112-byte frames.
uint64_t OptionsSetDialogNote(uint8_t* base, uint32_t sub_scene, uint64_t id,
                              const uint64_t varargs[7], uint32_t sp, const Calls& calls);

// sub_821A63B0 SaveScreen::ShowNotice: step 6 of the save screen's table,
// `step` the started word. First call (the word 0): the word = 1;
// SaveScreen::SetMessage(107) (its plain C++, its varargs 0: the original's
// are UiMovies::Find's leftovers) when the data's +12 is 0; PlayNamed(movie
// 44, 12) when its +16 is 0; MenuSound::Play(5). Every call:
// UiMovie::Run(movie, 0); when it answers 0 and +16 is 0, PlayNamed and
// LoopNamed(movie, 2). Whether Run answered 0. 144-byte frame.
uint64_t SaveScreenShowNotice(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

// --- The car cards (car_cards.cpp) ---

// sub_821821C8 CarCard::Fill: `node` a card's node in UI movie 4, `entry` a
// car entry: Child(node, 3) +108 the maker's logo by the car id (+8432),
// then children 5 to 7 strings 1934 + the byte +769, 1646 + the id and 1718
// + the id (SetNodeText), child 8 the number +744 (SetNodeNumberText),
// child 9 string 435. The last SetNodeText's r3. 128-byte frame.
uint64_t CarCardFill(uint8_t* base, uint32_t node, uint32_t entry, uint32_t sp,
                     const Calls& calls);
// sub_821827E0 CarCard::FillDetail: UI movie 18's +0xA000 = 0 and its nodes
// set from the car entry P[+48 + +52] (P [0x826D83F4] +1136). The last
// SetNodeText's r3. 192-byte frame.
uint64_t CarCardFillDetail(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821830C0 CarDetail::Open: [0x826D83F4] +12 = 1, FillDetail, +36 = 2,
// then the message (36, 0, 0) posted on `mode`'s ring (+204: its count
// +600, start +604, 50 slots of 12 bytes). FillDetail's r3. 112-byte frame.
uint64_t CarDetailOpen(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);

}  // namespace front_end_screen_helpers
