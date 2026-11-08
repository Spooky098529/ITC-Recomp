// The front end's options screen natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/options-screen.md, the blocks in
// docs/known-data.md): the screen's handlers, its pages and steps, and the
// settings read from and written back to the save data in memory. Plain C++
// over guest memory (the settings and the page blocks are shared with the
// recompiled code), with the originals' loads and stores in their order.
// Where an original builds values below its stack pointer or in its frame,
// the same words are written there (`sp` is the caller's stack pointer: the
// back chain of the original's frame is written as the original leaves it;
// its register saves aren't). Adapters: src/hooks/options_screen.cpp
// (behind --native_options_screen), register steps shared with the harness
// in src/hooks/options_screen.h.

#pragma once

#include "race_hud_object/race_hud_object.h"

#include <cstdint>

namespace options_screen {

// The guest calls the plain C++ makes: through the function table from the
// original's frame with its return address (the adapter's job).
using Calls = race_hud_object::Calls;

// The mode type (0x826032DC) the context and the posts follow, and the save
// data's options (player 0's save data + 0x1F85C).
constexpr uint32_t kModeType = 0x826032DC;
constexpr uint32_t kSaveOptions = 0x1F85C;
// The guest functions called through `Calls`: memcpy where its plain C++
// declines, SoundOptions::SetEffectsVolume and SetMusicVolume and
// UiMovies::Open through their overrides.
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kSetEffectsVolume = 0x821B9C20;
constexpr uint32_t kSetMusicVolume = 0x821B9CF0;
constexpr uint32_t kMoviesOpen = 0x8211E808;
// The UI movie calls, through their overrides: UiMovie::FinishAll, Run and
// UiMovies::Delete.
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kMoviesDelete = 0x8211E258;
// UiMovie::Play, PlayNamed, Loop, LoopNamed, LoopLast, Finish, FinishNamed.
constexpr uint32_t kPlay = 0x8211D978;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoop = 0x8211DDD8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kLoopLast = 0x8211DDE8;
constexpr uint32_t kFinish = 0x8211DE38;
constexpr uint32_t kFinishNamed = 0x8211DF20;
// UiMovie::SetText and SetNodeText (varargs: their unset r7 to r10 passed
// as 0, the user's call, 2026-10-30) and UiMovie::RestorePose.
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kSetNodeText = 0x8211C7E8;
constexpr uint32_t kRestorePose = 0x82119B80;
// MenuSound::Play through its override.
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
// Menu::Pressed, VirtualFree (outside the allocation layer) and
// Options::ShowCareerInfo (never seen in play: the original) through the
// function table; the sound manager [0x826C5C80].
constexpr uint32_t kMenuPressed = 0x821864C8;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kShowCareerInfo = 0x821C0EF0;
constexpr uint32_t kSoundManagerGlobal = 0x826C5C80;
// Through their overrides: Menu::Step, UiMovie::ResetPose, SetChildNameText,
// SetNumberText, ClearText and the text scroll's reset (sub_8211C5F8); the
// arrows never seen in play (Options::SetHudArrow, SetSoundArrow,
// SetSlidersArrow: the originals).
constexpr uint32_t kMenuStep = 0x82186568;
constexpr uint32_t kResetPose = 0x8211ABA8;
constexpr uint32_t kSetChildNameText = 0x8211D930;
constexpr uint32_t kSetNumberText = 0x8211C740;
constexpr uint32_t kClearText = 0x8211C470;
constexpr uint32_t kResetTextScroll = 0x8211C5F8;
constexpr uint32_t kScrollText = 0x8211C570;  // UiMovie::ScrollText
// The category table (5 contexts of 7 words, -1 past the last).
constexpr uint32_t kCategories = 0x8200A578;
constexpr uint32_t kSetHudArrow = 0x821BE4A0;
constexpr uint32_t kSetSoundArrow = 0x821BE7A8;
constexpr uint32_t kSetSlidersArrow = 0x821BEDC8;

// The screen's blocks (docs/known-data.md): the page block (28 bytes), the
// front-end menu (68), the settings `o` (716).
constexpr uint32_t kPage = 0x826D8470;
constexpr uint32_t kMenu = 0x826D8474;
constexpr uint32_t kSettings = 0x826D8478;

// --- The controls' tables and the settings' leaves (controls.cpp) ---

// sub_821BA950 Controls::ButtonIndex: a button code as the save keeps it
// (0 to 30, unsigned) to its index in the options' button lists (0 to 16),
// -1 for the others (its -1 cases the split piece sub_821BA984, merged).
uint32_t ButtonIndex(uint32_t code);
// sub_821BAA18 Controls::ButtonCode: an index (-1 to 18, by r3 + 1 unsigned)
// to the code the save keeps; 0 outside (its -1 case the split piece
// sub_821BAA50, merged).
uint32_t ButtonCode(uint32_t index);
// sub_821BAAE0 Controls::AnalogIndex: a code to its index in the analog
// lists, -1 for most (its case 7 the split piece sub_821BAB14, merged).
uint32_t AnalogIndex(uint32_t code);
// sub_821BAD08 Controls::AnalogPicture: an analog index (-1 to 18, by r3 +
// 1 unsigned) to the picture shown, else -1 (its -1 cases the split piece
// sub_821BAD40, merged).
uint32_t AnalogPicture(uint32_t index);

// sub_821BA7E0 Options::IsButtonTaken and sub_821BAB58 Options::IsAnalogTaken:
// the twelve button edits (or six analog edits) in menu order copied below
// `sp` (sp - 48 or sp - 32, as the leaves leave them); 1 when another item
// holds the same value as item `item` (read from those words as the
// original does, any item), else 0.
uint32_t IsButtonTaken(uint8_t* base, uint32_t item, uint32_t sp);
uint32_t IsAnalogTaken(uint8_t* base, uint32_t item, uint32_t sp);

// sub_821BA890 Options::HasButtonClash and sub_821BAC78
// Options::HasAnalogClash (leaves, never seen in play, ported with their
// callers): the edits copied below `sp` as the taken checks copy them; 1
// when two items hold the same word, else 0.
uint32_t HasButtonClash(uint8_t* base, uint32_t sp);
uint32_t HasAnalogClash(uint8_t* base, uint32_t sp);

// sub_821BA550 Options::StepButtons and sub_821BABD8 Options::StepAnalog:
// the settings' side word (+668 or +676) = the menu's side (+36) when it's
// -1 or 1, else 0; with a side, 1 on the items before the last (12 or 6),
// and on the last the OK/Default word +660 toggled (left on 1, right on 0)
// and 1, else 0.
uint32_t StepButtons(uint8_t* base);
uint32_t StepAnalog(uint8_t* base);

// sub_821BE5F8 Options::StepSound (a leaf): by the menu's side (-1 or 1)
// and cursor: 0 and 1 the volume edits +252, +256 stepped within 0 to 15
// (past a bound: the bound stored, 0), 2 the OK/Default word +660 (left on
// a 1, right on a 0), 3 the track +688 stepped, wrapping within 0 to 16.
// 1 when something changed.
uint32_t StepSound(uint8_t* base);
// sub_821BEAC8 Options::StepSliders: node (7, 0)'s children 4 to 6 looked up
// (unused); cursor 0 to 2 the slider edits +260 to +268 stepped within -10
// to 10 (none at a bound), 3 the OK/Default word. Its 112-byte frame. 1
// when something changed.
uint32_t StepSliders(uint8_t* base, uint32_t sp);

// sub_821B9DB8 Settings::Reset: r3 = the settings (716 bytes): +0 = 0, the
// three 192-byte blocks (+44, +236, +428) and +12 (32 bytes) cleared
// (memset), then the words its row lists. Its 112-byte frame's back chain
// at sp - 112. Returns r3 as the original leaves it (memset's last
// destination, `settings` + 12).
uint32_t SettingsReset(uint8_t* base, uint32_t settings, uint32_t sp);

// --- The settings' read and write (settings.cpp) ---

// sub_821BBF00 Settings::Read: the settings from player 0's save data (its
// row): the context by the mode type, the current settings, the controls'
// indexes (Controls::ButtonIndex and AnalogIndex over the four 60-byte
// records copied to its frame), the defaults (a 13-word table on its
// frame), type 2's career info, then SoundOptions::SetEffectsVolume and
// SetMusicVolume (through `calls`). Its 272-byte frame below `sp` (+80 the
// globals' base 0x826E0000, +84 the table, +144 the last record). r3
// SetMusicVolume's.
uint64_t SettingsRead(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821BAE10 Settings::Write: the settings back into player 0's save
// data (its row; the analog slot 4's word 8 kept as the original writes
// it). Its 176-byte frame below `sp` (+80 the record). r3 the last
// memcpy's (the last record's address).
uint64_t SettingsWrite(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The machine's handlers (handlers.cpp) ---

// sub_821BB9A8 Options::OnEnter: r3 = the machine: +9608 = 0, the seven
// blocks allocated (the allocation layer's pages: the adapter runs it only
// over the layer) and set up, Settings::Reset on the settings, +9604 = 1 or
// 9 by +9612. Its 128-byte frame below `sp`. r3 the last block.
uint32_t OnEnter(uint8_t* base, uint32_t machine, uint32_t sp);
// sub_821BBBA0 Options::WaitForMovie: r3 = the machine: while +9608 is 0,
// it = 1 once UiMovies::IsLoaded(7); then +9604 = 9 or 4 by +9612. Its
// 96-byte frame. r3 IsLoaded's when asked, else the machine (`r3`).
uint64_t WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_821B9F88 Options::OpenMovie: UiMovies::Open(7, 22553), a tail call
// (`sp` and `lr` the caller's). Its r3.
uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls);
// sub_821BCE38 Options::OnClosed: r3 = the machine, r4 = the dispatcher:
// Settings::Write (from its 112-byte frame), message 6 (types 2, 6, 8), 5
// (type 7) or 8 (type 4) queued in the dispatcher's ring (+204), +9604 = 9.
// r3 Settings::Write's.
uint64_t OnClosed(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                  const Calls& calls);

// A message (three words: `message`, 0, 0) queued in a dispatcher's ring
// (+204: 50 slots of 12 bytes, the count at +600, the start at +604), with
// no overflow check, as the handlers do it inline.
void Post(uint8_t* base, uint32_t dispatcher, uint32_t message);

// `m`: UI movie 7 (UiMovies::Find on the id stored at frame + 80, as the
// originals do), the movie table's value or the word at 0x826C5CDC.
uint32_t FindMovie(uint8_t* base, uint32_t frame);

// sub_821BCD28 Options::Close: r3 the machine, r4 the dispatcher: FinishAll(m),
// with +9616 set UiMovies::Delete(7) and +9608 = 0, then the mode's suspend
// message posted (2 15, 4 11, 6 28, 7 25, 8 24). Its 112-byte frame; r3 the
// last call's.
uint64_t Close(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
               const Calls& calls);
// sub_821BCF08 Options::Leave: +9604 = 7; with `m`, FinishAll and
// UiMovies::Delete(7); message 4 (types 2, 4, 6, 8) or 3 (7) posted. Its
// 112-byte frame; r3 Delete's, else `m` (0).
uint64_t Leave(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
               const Calls& calls);
// sub_821BD330 Options::ClosePage: r3 the dispatcher: by the mode type 2 13,
// 4 9, 6 25, 7 and 8 22 posted. A leaf; r3 left.
uint64_t ClosePage(uint8_t* base, uint64_t dispatcher);
// sub_821BD3C8 Options::Wait (step 0): r3 the step word: first pass the
// settings' +684 = 0 and the word 1; UiMovie::Run(m, 0); +684 up by one; 1
// once it's 10 or more, else 0. Its 112-byte frame.
uint64_t Wait(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

// --- The steps on the movie (steps.cpp) ---
//
// r3 the step word (the page block's +24, 0 on the first pass): the first
// pass's animations (UiMovie::Play, Loop and the like through their
// overrides), then UiMovie::Run(m, 0) every pass; 1 (done). Each from its
// frame below `sp` (+80 the movie id).

// sub_821BD468 Options::OpenCategories (step 4): sub-scene 23's animation
// 1, 16's or 17's 0 by the context (1, or 0 and 2 to 4; none from 5), and
// 21's cursor + 2, each played and looped. A 128-byte frame.
uint64_t OpenCategories(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_821BD5B0 Options::ReturnToCategories (step 5): 23's 1 and 21's cursor
// + 2. A 128-byte frame.
uint64_t ReturnToCategories(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_821BD728 Options::EnterHud (step 9): 5's cursor + 4 below 4, else 2
// (OK/Default +660 1) or 1. A 128-byte frame.
uint64_t EnterHud(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_821BD800 Options::ChangeHudValue (step 11): on the last row (cursor 4)
// 5's 2 or 1 finished and the other played and looped. A 112-byte frame.
uint64_t ChangeHudValue(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_821BD908 Options::EnterSound (step 13): 6's 1 played, LoopLast. 112.
uint64_t EnterSound(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_821BD9A0 Options::EnterSliders (step 17): named 226. 112.
uint64_t EnterSliders(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_821BDAD0 Options::EnterButtons (step 25): named 407, then the button
// picture's (423 + 2 x the settings' +284, from the 17-word table at the
// frame's +96, read as the original does for any value). A 208-byte frame.
uint64_t EnterButtons(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

// --- The node helpers on the movie (nodes.cpp) ---
//
// Each from its frame below `sp` (+80 the movie id, then the words its
// conversions leave); r3 as the original leaves it. The node lookups
// (UiMovie::FindNode, UiMovie::Child) are their plain C++.

// sub_821BE218 Options::SetCategoryArrow: node (-1, 21)'s child 0's +16 =
// `value` as a float. 112-byte frame. The child.
uint32_t SetCategoryArrow(uint8_t* base, uint64_t value, uint32_t sp);
// sub_821BE518 Options::ShowVolumeNumber: SetNodeText(m, the node's child 2,
// 8, volume), then children 4 to 18's +80 = 1 for the volume's (1 to 15).
// 128-byte frame. The last child.
uint64_t ShowVolumeNumber(uint8_t* base, uint32_t node, uint64_t volume, uint32_t sp,
                          const Calls& calls);
// sub_821BE930 Options::ShowMusicName: SetText(m, 6, 8, 501 + value).
// 112-byte frame. SetText's r3.
uint64_t ShowMusicName(uint8_t* base, uint64_t value, uint32_t sp, const Calls& calls);
// sub_821BE988 Options::ShowSlider: SetNodeText(m, child 0, 8, value), child
// 2's +12 = 16 - trunc((value + 10) x [0x8200A6AC]) as a float. 128-byte
// frame. Child 2.
uint32_t ShowSlider(uint8_t* base, uint32_t slider, uint64_t value, uint32_t sp,
                    const Calls& calls);
// sub_821BEA48 Options::ShowSliderMark: child 3's +12 as ShowSlider's child
// 2's. 112-byte frame. Child 3.
uint32_t ShowSliderMark(uint8_t* base, uint32_t slider, uint64_t value, uint32_t sp);
// sub_821BF0F0 Options::ShowButtonPicture and sub_821BF630
// ShowAnalogPicture: nodes (9, 6) to (9, 22), or (11, 6) to (11, 16), +80 =
// 1 for child 6 + `shown`, else 0. 128-byte frames. The last node.
uint32_t ShowButtonPicture(uint8_t* base, uint32_t shown, uint32_t sp);
uint32_t ShowAnalogPicture(uint8_t* base, uint32_t shown, uint32_t sp);
// sub_821BFA68 Options::ResetNode: child `child` (under `group` unless -1)
// of the file, live and saved scenes; UiMovie::RestorePose on the live and
// the saved against the file's. 128-byte frame. The second RestorePose's r3.
uint64_t ResetNode(uint8_t* base, uint32_t group, uint32_t child, uint32_t sp,
                   const Calls& calls);

// sub_821BE5A8 Options::ShowVolumeBar: children 22 to 36 of `node` +80 = 1
// for child 21 + i with i <= the volume, else 0 (no movie lookup). 112-byte
// frame. The last child.
uint32_t ShowVolumeBar(uint8_t* base, uint32_t node, uint64_t volume, uint32_t sp);
// sub_821BE820 Options::ShowVolumeNumbers: ShowVolumeNumber on nodes (6, 6)
// and (6, 7) with the edits +252 and +256. 112-byte frame. The second's r3.
uint64_t ShowVolumeNumbers(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821BECA8 Options::ShowSliderNumbers and sub_821BED38 ShowSliderMarks:
// the edits +260 to +268 copied to the frame's +88, ShowSlider (or
// ShowSliderMark) on node (7, 0)'s children 4 to 6. 144-byte frames. The
// last call's r3.
uint64_t ShowSliderNumbers(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t ShowSliderMarks(uint8_t* base, uint32_t sp);
// sub_821BE290 Options::ShowHudValues: the HUD edits +236 to +248 and +660
// copied to the frame's +96; for each of node (5, 2)'s +104 rows the left
// and right choices' frame (child 0 +108) and shown (child 2 +80) words by
// the value (1: the left lit). 208-byte frame. The last child, or the node.
uint64_t ShowHudValues(uint8_t* base, uint32_t sp);
// sub_821BEFE8 Options::ShowButtonList and sub_821BF520 ShowAnalogList: the
// edits in menu order copied to the frame (+96, or +128 with the names at
// +96); on node (9, 5) or (11, 5) each item's name (SetNodeText, strings
// 348 to 359, or 351 to 355 and 359; its unset varargs 0) and picture
// (child 1 +108 = the edit). 208-byte frames. The last picture node.
uint32_t ShowButtonList(uint8_t* base, uint32_t sp, const Calls& calls);
uint32_t ShowAnalogList(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821BF158 Options::ShowButtonClashes and sub_821BF698
// ShowAnalogClashes: per item, the taken (358 + 4i, or 609 + 4i) or free
// (357 + 4i, or 608 + 4i) animation by IsButtonTaken (IsAnalogTaken),
// PlayNamed(m, it, 1) then FinishNamed. 256-byte frames (two 13-word tables
// at +96 and +160). The last FinishNamed's r3.
uint64_t ShowButtonClashes(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t ShowAnalogClashes(uint8_t* base, uint32_t sp, const Calls& calls);

// The second migration's steps (steps.cpp; r3 the step word, the frame
// below `sp`):
// sub_821BDE88 Options::EnterAnalog (step 33): named 658, then the analog
// picture's (674 + 2 x AnalogPicture(+380), from the 11-word table at the
// frame's +96). A 176-byte frame. 1.
uint64_t EnterAnalog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_821C09B0 Options::MoveSaveDialogCursor (step 45) and sub_821C0B50
// MoveCategory6DialogCursor (step 48): the dialog menu's (+4) before
// finished, its (+0) played and looped (or finished), from 843 (or 859).
// 128-byte frames. 1.
uint64_t MoveSaveDialogCursor(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t MoveCategory6DialogCursor(uint8_t* base, uint32_t step, uint32_t sp,
                                   const Calls& calls);
// sub_821C08B0 Options::ChangeSaveDialog (step 44) and sub_821C0A50
// ChangeCategory6Dialog (step 47): first pass MenuSound::Play(5) and the
// dialog's animations by the state +620 (or +640); 1 once UiMovie::Run
// answers 0 (the state = +624, or +644), else 0. 128-byte frames.
uint64_t ChangeSaveDialog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t ChangeCategory6Dialog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// --- The machine's dispatch and the page helpers (pages.cpp) ---

// sub_821C80F0 Options::Draw: the 15 pages' handlers at its 224-byte
// frame's +80; the page p +0's (not -1, no bounds check) called with r3 =
// r4 = the dispatcher and the original's leftovers in r5 to r10. Its r3, or
// 0x821C2388 (the original's last table register) with no page.
uint64_t Draw(uint8_t* base, uint64_t dispatcher, uint32_t sp, const Calls& calls);
// sub_821C81E8 Options::Update: the 49 steps at its 480-byte frame's +128
// (+80 to +112 the spilled 0x821C0000); the step p +12: -1 UiMovie::Run(m,
// 0) unless the page is 14; >= 0 called (r3 = p + 24, r4 to r10 the
// original's leftovers) and, when it answers nonzero, the step ended (the
// queued one, or the page after); other negatives ended at once. Its r3:
// the step's, Run's or `m`.
uint64_t Update(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821C6F58 Options::SavePage (page 9) and sub_821BD228 Category6Page
// (page 10): the dialog's cancel to step 41 (42) with page 9 (10) after and
// the sub-pages' block ([0x826D847C] or [0x826D8488]) to 5; its sub-page
// called from the 7-entry (6-entry) table at the frame's +80 (r3 as it
// came; r4 too for page 10; the leftovers); page 9 then Menu::Pressed(menu,
// 158). 128- and 112-byte frames. r3: Pressed's; the handler's or `r3`.
uint64_t SavePage(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t Category6Page(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_821BE3E0 Options::StepHud: the five values' addresses at the frame's
// +80; by the menu's side, left on a 0 -> 1, right on a 1 -> 0, then
// ShowHudValues, 1; else 0. 112-byte frame.
uint64_t StepHud(uint8_t* base, uint32_t sp);
// sub_821BE8A8 Options::ShowVolumeBars: ShowVolumeBar on (6, 6) and (6, 7)
// with +252 and +256. 112-byte frame. The second's r3.
uint64_t ShowVolumeBars(uint8_t* base, uint32_t sp);
// sub_821C1068 Options::ShowPrompts: `values` r3 to r8 (three prompts'
// first and second frames) at the frame's +88 and +104; prompt i's node (1,
// i) shown with its two choices' +108 set when neither is -1, else hidden.
// 176-byte frame. The last second choice's node.
uint64_t ShowPrompts(uint8_t* base, const uint64_t (&values)[6], uint32_t sp);
// sub_821C0FF0 Settings::Reload: by the context, 1 ShowCareerInfo (the
// original) and PlayTime::Mark on player 0's save data, 2 and 3 a word -1;
// then Settings::Read. 96-byte frame. Read's r3.
uint64_t SettingsReload(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821B9E70 Options::OnLeft: the sound manager's StopCue(2, 0) and
// ReleaseBank(2) (slots 13 and 5), the seven blocks freed (the allocation
// layer's pages, else VirtualFree) and the globals 0. 160-byte frame. The
// last free's answer.
uint64_t OnLeft(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821BC670 Options::CategoryPage (page 0): Menu::Step; moved -> step 6
// and sound 4; accept (159) -> the category +664, step 7, sound 0; back
// (158) -> sound 2, step 8 with page 14 after (type 4: [0x826364EC] = 0).
// 96-byte frame. The last call's r3.
uint64_t CategoryPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821BCFF0 Options::SetUpMovie: ResetPose, the panels by the context
// (type 2's career info texts), the prompts' frames, the menu music's cue
// (+704), +692 = -1, +0 = 1. 160-byte frame. The last slot 14's r3.
uint64_t SetUpMovie(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821C1158 Options::Show: Settings::Read, the cursor +664 = 0,
// SetUpMovie, the page block (steps 3 then 4) and the controller pages'
// open message posted. 112-byte frame. SetUpMovie's r3.
uint64_t Show(uint8_t* base, uint32_t dispatcher, uint32_t sp, const Calls& calls);
// The cursor steps (steps.cpp; r3 the step word, 1): sub_821C2BE0
// Options::MoveCategoryCursor (step 6, 160-byte frame), sub_821C2E28
// MoveHudCursor (step 10, 160), sub_821C3008 MoveSoundCursor (step 14,
// 144), sub_821C3278 ChangeSoundValue (step 15, 128), sub_821C3398
// MoveSlidersCursor (step 18, 144). Their help lines' SetText unset varargs
// passed as 0.
uint64_t MoveCategoryCursor(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t MoveHudCursor(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t MoveSoundCursor(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t ChangeSoundValue(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t MoveSlidersCursor(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_821C35F0 Options::ChangeSlidersValue (step 19, 112-byte frame): on
// the last row the OK/Default animations (229, 230), else
// ShowSliderNumbers. 1.
uint64_t ChangeSlidersValue(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_821C2778 Options::LeaveButtons (step 1, 336-byte frame) and
// sub_821C2A10 LeaveAnalog (step 2, 240): their tables written each pass;
// first pass +684 = 0, the cursor's animations finished (or the last
// row's), the edits put back (memcpy), the list and clashes shown; then
// UiMovie::Run and +684 up by one: 1 from 10.
uint64_t LeaveButtons(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t LeaveAnalog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

// --- The type 2 and 4 modes' Inits (inits.cpp) ---

// An Init's result: StateMachine::Add's last answer, the machine, and
// whether that Add replaced a transition (its answer then an index).
struct InitResult {
  uint32_t r3;
  uint32_t machine;
  bool replaced;
};
// An Init: its address, name, frame (its stwu size) and the first register
// __savegprlr_<n> saves (r<n> to r31 and lr: (32 - n) * 8 + 8 bytes below
// the caller's r1).
struct InitInfo {
  uint32_t address;
  const char* name;
  uint32_t frame;
  uint32_t first_saved;
};
constexpr int kInitCount = 4;
extern const InitInfo kInits[kInitCount];
// sub_821C9E00 Type2Options::Init, sub_821CA458 Type4Options::Init,
// sub_821C8578 Type2ControllerOptions::Init and sub_821C8A60
// Type4ControllerOptions::Init (r3 the machine, `object`): their frames'
// stores and copies below `sp` (the back chain too) and their
// StateMachine::Add calls (plain C++) in their order.
InitResult RunInit(uint8_t* base, uint32_t address, uint32_t object, uint32_t sp);

// --- The category pages (category_pages.cpp) ---
//
// Options::Draw's pages 1, 3, 5 and 7 (r3 not read): Menu::Step on the
// menu, then the cursor's move, the value's change, accept (OK, Default or
// the cursor sent to the last row) or back, by their rows. From their
// frames below `sp`; r3 the last call's.
// sub_821C1270 Options::HudPage (96-byte frame).
uint64_t HudPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821C1888 Options::SlidersPage (96).
uint64_t SlidersPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821C1D08 Options::ButtonsPage (112).
uint64_t ButtonsPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821C21A8 Options::AnalogPage (96).
uint64_t AnalogPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821C1498 Options::SoundPage (160): the volumes heard on a change
// (SoundOptions::SetEffectsVolume and SetMusicVolume through their
// overrides), the track's row stopping the menu music and going to page 11
// or 12, OK and back leaving through page 13.
uint64_t SoundPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821BA380 Options::LeaveSoundPage (page 13, 160): by the settings'
// +700, the preview stopped, bank 2 released, the menu music started again
// (Sound::Play through its override) and the sound page's close queued;
// the sound manager's slots through `calls`. `r3` returned as it came for
// another +700.
uint64_t LeaveSoundPage(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);

// The set-ups (category_pages.cpp; no arguments, from their frames below
// `sp`, the movie's text calls through their overrides with their unset
// varargs 0):
// sub_821C4BE0 Options::SetUpCategoryPage (144): the menu over the
// context's categories, nodes 28 and 29's frame, the names and help line,
// ShowPrompts(0, 0, 1, 1, -1, -1). ShowPrompts' r3.
uint64_t SetUpCategoryPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821C4DB0 SetUpHudPage (144), sub_821C4FD0 SetUpSoundPage (128),
// sub_821C51D0 SetUpSlidersPage (128), sub_821C5580 SetUpButtonsPage (128),
// sub_821C57D0 SetUpAnalogPage (128): the menu, the edits copied from the
// current settings, the page's node reset, its texts and values shown. The
// last call's r3.
uint64_t SetUpHudPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t SetUpSoundPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t SetUpSlidersPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t SetUpButtonsPage(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t SetUpAnalogPage(uint8_t* base, uint32_t sp, const Calls& calls);

// The third migration's steps (steps.cpp; r3 the step word, 112-byte
// frames below `sp`):
// sub_821C7080 Options::FadeIn (step 3): first pass sub-scene -1's
// animations 1 and 0 played, 0 looped, ScreenFades::FadeIn([0x82001BEC],
// black, layer 0) and SetUpCategoryPage; 1 once UiMovie::Run answers 0 and
// layer 0's fade counts are 0, else 0.
uint64_t FadeIn(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_821C75E0 Options::CloseHud (step 12), sub_821C7710 CloseSound (16),
// sub_821C7868 CloseSliders (20), sub_821C7B08 CloseButtons (28),
// sub_821C7D18 CloseAnalog (36): first pass the cursor's animation finished
// (none for the buttons and analog pages), the help line cleared, the
// page's close played, SetUpCategoryPage; once UiMovie::Run answers 0, step
// 5 with the categories after and the step word 0. 0.
uint64_t CloseHud(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t CloseSound(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t CloseSliders(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t CloseButtons(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t CloseAnalog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

}  // namespace options_screen
