// The other in-race screens natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/race-screens.md): the pause menu, the
// confirm dialog, the career's results, the menu after the race, the
// makers and teardowns of four screens never seen in play, and the menu
// list's and page object's helpers the front end shares. Plain C++ over
// guest memory (the screens, their lists and page objects are shared with
// the recompiled code), with the originals' loads and stores in their
// order. `sp` is the caller's stack pointer: the back chain of the
// original's frame is written there as the original leaves it; its
// register saves aren't. Adapters: src/hooks/race_screens.cpp (and, for
// the functions logged by --log_pause_menu and --log_pause_movie, their
// logging overrides in src/debug/, which call the adapters' steps).
//
// The screens' updates run on logic passes and their draws on every pass,
// as the originals do (the machine's handlers call them; see the file's
// intro and docs/pause-menu-fix.md): nothing here changes when they run.

#pragma once

#include "race_hud_object/race_hud_object.h"

#include <cstdint>
#include <optional>

namespace race_screens {

using Calls = race_hud_object::Calls;

// The action mappers' table (8 bytes an entry: the mapper, a word +4: 1
// when it isn't read) and each controller's entry index.
constexpr uint32_t kMapperEntries = 0x826BAB80;
constexpr uint32_t kControllerEntries = 0x826BABA4;
// The mapper's read slots the menu list calls (vtable 0x82007F78):
// slot 10 (+40) ActionMapper::Pressed, slot 12 (+48) ActionMapper::Fired.
constexpr uint32_t kPressedSlot = 0x8210BF40;
constexpr uint32_t kFiredSlot = 0x8210C010;

// The menu list's fields (72 bytes; see the file's intro).
constexpr uint32_t kListCount = 0;
constexpr uint32_t kListScroll = 4;
constexpr uint32_t kListScrollBefore = 8;
constexpr uint32_t kListShown = 12;
constexpr uint32_t kListSelection = 16;
constexpr uint32_t kListSelectionBefore = 20;
constexpr uint32_t kListMove = 24;
constexpr uint32_t kListSide = 28;
constexpr uint32_t kListWrapped = 32;
constexpr uint32_t kListDown = 36;
constexpr uint32_t kListUp = 40;
constexpr uint32_t kListLeft = 44;
constexpr uint32_t kListRight = 48;
constexpr uint32_t kListAccept = 52;
constexpr uint32_t kListBack = 56;
constexpr uint32_t kListPlayer = 60;
constexpr uint32_t kListWraps = 64;
constexpr uint32_t kListBlock = 68;

// --- The page object (menu_list.cpp) ---

// ScreenPages::Init (sub_8220BA98): +0, +4, +8 = `page`, +20 and +12 = -1,
// +24 = 0.
void PagesInit(uint8_t* base, uint32_t pages, uint32_t page);
// ScreenPages::Go (sub_8220BA68): `transition` -1: +0 = `page`, +12 and +16
// = -1; else +12 = `transition`, +16 = `queued`, +4 = `page`, +0 = -1.
void PagesGo(uint8_t* base, uint32_t pages, uint32_t page, uint32_t transition, uint32_t queued);
// ScreenPages::Finish (sub_8220BA28): nothing when `result` is 0; else with
// no queued transition (+16 -1) +12 = -1 and +0 = +4, or the queued one
// running (+20 = 0, +12 = it, +16 = -1); then +24 = 0.
void PagesFinish(uint8_t* base, uint32_t pages, uint32_t result);

// --- The menu list (menu_list.cpp) ---

// MenuList::Selection (sub_8220BC20): +16.
uint32_t ListSelection(uint8_t* base, uint32_t list);
// MenuList::SetSelection (sub_8220BC30): +20 = the old +16, +16 = `selection`.
void ListSetSelection(uint8_t* base, uint32_t list, uint32_t selection);
// MenuList::SetEnabled and IsEnabled (sub_8220BC00, sub_8220BC10): the
// item's word in the block (+68).
void ListSetEnabled(uint8_t* base, uint32_t list, uint32_t item, uint32_t value);
uint32_t ListIsEnabled(uint8_t* base, uint32_t list, uint32_t item);
// MenuList::Fit (sub_8220BC40): the scroll and selection refitted; `quiet`
// (r4) nonzero keeps the move word 0 and the "before" words the new ones.
void ListFit(uint8_t* base, uint32_t list, uint32_t quiet);
// MenuList::Fired (sub_8220C020): whether the list's player's mapper fired
// `action` (slot 12; a direction's two ids, 153/167, 154/168, 155/169,
// 156/170, each tried in turn), 0 when the entry's +4 is 1. The slot's
// plain C++ when `native_slots` and it's ActionMapper::Fired, else the slot
// through `calls`.
uint32_t ListFired(uint8_t* base, uint32_t list, uint32_t action, uint32_t sp, bool native_slots,
                   const Calls& calls);
// MenuList::Update (sub_8220C1E0): left, right, up and down (Fired on +44,
// +48, +40, +36) moving the side word or the selection over the enabled
// items, wrapping when +64 is set, the scroll kept around it; returns -1.
uint64_t ListUpdate(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                    const Calls& calls);
// MenuList::Accepted, Cancelled, Pressed152 and Pressed166 (sub_8220BD50,
// sub_8220BDE0, sub_8220BE70, sub_8220BF90): 1 when the list's player's
// mapper answers slot 10 (Pressed) on `action` (+52, +56, 152 or 166) and
// the list's move word (+24) is 0, else 0 (0 when the entry's +4 is 1).
uint32_t ListAccepted(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                      const Calls& calls);
uint32_t ListCancelled(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                       const Calls& calls);
uint32_t ListPressed152(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                        const Calls& calls);
uint32_t ListPressed166(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                        const Calls& calls);

// --- The screens' globals ---

// The pause menu: the screen (4 bytes, its movie), the data (160), the
// page object (28), the menu list (72).
constexpr uint32_t kPauseScreen = 0x826D84D0;
constexpr uint32_t kPauseData = 0x826D84D4;
constexpr uint32_t kPausePages = 0x826D84D8;
constexpr uint32_t kPauseList = 0x826D84DC;
// Kind 4's pause menu (never seen in play): the same with 80 bytes of data.
constexpr uint32_t kPause4Screen = 0x826D84E0;
constexpr uint32_t kPause4Data = 0x826D84E4;
constexpr uint32_t kPause4Pages = 0x826D84E8;
constexpr uint32_t kPause4List = 0x826D84EC;
// The confirm dialog: the menu list (72), the page object (28), the data
// (40).
constexpr uint32_t kDialogList = 0x826D84F4;
constexpr uint32_t kDialogPages = 0x826D84F8;
constexpr uint32_t kDialogData = 0x826D84FC;
// The career's results (212 bytes).
constexpr uint32_t kCareerResults = 0x826D8500;
// The menu after the race: the screen (4), the data (24), the page object
// (28), the menu list (72).
constexpr uint32_t kAfterScreen = 0x826D8510;
constexpr uint32_t kAfterData = 0x826D8514;
constexpr uint32_t kAfterPages = 0x826D8518;
constexpr uint32_t kAfterList = 0x826D851C;
// The screens never seen in play: kind 6's screen after the race (360
// bytes, and a word beside it), the results of kinds 1 and 2 (52), those
// of kinds 4 and 6 (40).
constexpr uint32_t kKind6Menu = 0x82814E10;
constexpr uint32_t kKind6Word = 0x82814E14;
constexpr uint32_t kRaceResults = 0x82814E18;
constexpr uint32_t kSplitResults = 0x82814E1C;

// The guest functions called through `Calls`: memcpy where its plain C++
// refuses.
constexpr uint32_t kMemcpy = 0x825A7410;

// --- The makers and teardowns (screens.cpp) ---
//
// The makers allocate with VirtualAlloc(0, size, MEM_COMMIT, read-write):
// the allocation layer's pages (the adapters run them only over it,
// hooks::NativeHeapActive). Each returns r3 as the original leaves it.
// The teardowns free with VirtualFree(.., 0, MEM_RELEASE): nullopt, with
// nothing done, where a block isn't the layer's pages (or a menu list's
// block isn't releasable natively: alt_hud::MenuListReleasable), and the
// adapters run the originals; with the screen's global 0, the original's
// OutputDebugStringA (its text discarded, helpers::DebugOutput) and r3 the
// text.

// sub_8220D7E8 PauseMenu::Create, sub_82210868 PauseMenu4::Create,
// sub_8221B1F0 AfterRaceMenu::Create, sub_82217D70 RaceDialog::Create,
// sub_82219138 CareerResults::Create, sub_8221F390 RaceResults::Create,
// sub_8221FA30 SplitResults::Create, sub_8221BDD0 Kind6Menu::Create.
uint64_t PauseMenuCreate(uint8_t* base, uint32_t sp);
uint64_t PauseMenu4Create(uint8_t* base, uint32_t sp);
uint64_t AfterRaceMenuCreate(uint8_t* base, uint32_t sp);
uint64_t RaceDialogCreate(uint8_t* base, uint32_t sp);
uint64_t CareerResultsCreate(uint8_t* base, uint32_t sp);
uint64_t RaceResultsCreate(uint8_t* base, uint32_t sp);
uint64_t SplitResultsCreate(uint8_t* base, uint32_t sp);
uint64_t Kind6MenuCreate(uint8_t* base, uint32_t sp);
// sub_8220C4C8 PauseMenu::Destroy, sub_8220FC98 PauseMenu4::Destroy,
// sub_8221A3E8 AfterRaceMenu::Destroy, sub_82217CD0 RaceDialog::Destroy,
// sub_82219170 CareerResults::Destroy, sub_8221F3C8 RaceResults::Destroy,
// sub_8221FA68 SplitResults::Destroy, sub_8221BE30 Kind6Menu::Destroy.
std::optional<uint64_t> PauseMenuDestroy(uint8_t* base, uint32_t sp);
std::optional<uint64_t> PauseMenu4Destroy(uint8_t* base, uint32_t sp);
std::optional<uint64_t> AfterRaceMenuDestroy(uint8_t* base, uint32_t sp);
std::optional<uint64_t> RaceDialogDestroy(uint8_t* base, uint32_t sp);
std::optional<uint64_t> CareerResultsDestroy(uint8_t* base, uint32_t sp);
std::optional<uint64_t> RaceResultsDestroy(uint8_t* base, uint32_t sp);
std::optional<uint64_t> SplitResultsDestroy(uint8_t* base, uint32_t sp);
std::optional<uint64_t> Kind6MenuDestroy(uint8_t* base, uint32_t sp);
// sub_8221A498 AfterRaceMenu::Close: MenuList::Release([0x826D851C]) (a
// tail call: `sp` the caller's); nullopt where it isn't native.
std::optional<uint64_t> AfterRaceMenuClose(uint8_t* base, uint32_t sp);

// --- The pause menu's data and the answers (screens.cpp) ---

// sub_8220F840 PauseMenu::LoadData: the data from player 0's save data (see
// the row); r3 as the original leaves it.
uint64_t PauseMenuLoadData(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8220D6A0 PauseMenu::SaveOptions (and sub_8220D890 PauseMenu::Close,
// its tail call) and sub_8220D740 PauseMenu::SaveHudOptions: the data's
// options written back into player 0's save data (+0x1F85C). Return the
// save data (r3).
uint64_t PauseMenuSaveOptions(uint8_t* base, uint32_t sp);
uint64_t PauseMenuClose(uint8_t* base, uint32_t sp);
uint64_t PauseMenuSaveHudOptions(uint8_t* base, uint32_t sp);
// sub_8220C5D0 PauseMenu::AnswerPage: the data's +140. sub_8221A720
// AfterRaceMenu::AnswerPage: the data's +12. sub_82217D40 RaceDialog::Answer:
// the data's +32 = 0; 1 when its +0 is 1, else 2.
uint32_t PauseMenuAnswerPage(uint8_t* base);
uint32_t AfterRaceMenuAnswerPage(uint8_t* base);
uint32_t RaceDialogAnswer(uint8_t* base);

// --- The pages' dispatch and the pause menu's steps (pages.cpp) ---
//
// The updates run the page's handler from a table the original builds in
// its frame (the handlers' addresses, left there), through `calls` with the
// registers the original has at its bctrl (`r3`, `r4` its caller's where
// it doesn't set them; r5 to r10 the table's words, sign-extended as its
// lis and addi leave them); 0 with no page (-1).

// sub_8220F638 PauseMenu::Update: the page [[0x826D84D8]] (r3 and r4 its
// caller's).
uint64_t PauseMenuUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls);
// sub_8221A090 CareerResults::Update: r3 = the screen, its +184 the page
// (r4 and r5 its caller's).
uint64_t CareerResultsUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t sp,
                             const Calls& calls);
// sub_8221BD20 AfterRaceMenu::Update: the page [[0x826D8518]].
uint64_t AfterRaceMenuUpdate(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_822185B0 RaceDialog::Update: with the data's +36 1 (and no page with
// no transition, or a page) and the page not 5, ScreenPages::Go(5,
// transition 1) and the data's +0 = 0; then the page's handler (r4 its
// caller's, or 5 after the Go).
uint64_t RaceDialogUpdate(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_822191D0 CareerResults::Start: ScreenPages::Go([0x826D8500] + 184, 1,
// transition 1); returns 0.
uint64_t CareerResultsStart(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8220CED0 PauseMenu::StepVolume and sub_8220D248 PauseMenu::StepAdjust:
// by the list's side (+28: -1 down, 1 up) the selected value stepped (the
// volumes +52, +56 within 0 to 15; the three values +60 to +68 within -10
// to 10) or the last item's +144 toggled; 1 when it changed, else 0
// (StepVolume's clamped steps store the bound and answer 0). StepAdjust
// first looks up the type-10 node 0's children 4 to 6 (unused).
uint32_t PauseMenuStepVolume(uint8_t* base, uint32_t sp);
uint32_t PauseMenuStepAdjust(uint8_t* base, uint32_t sp);

// The page object's Go through its override (its --log_pause_movie line
// kept).
constexpr uint32_t kPagesGo = 0x8220BA68;

// --- The pause menu's transitions and item helpers (transitions.cpp) ---
//
// The UI movie calls go through their overrides (`calls`): Play, PlayNamed,
// Loop, Finish, FinishNamed and SetNodeText keep their --log_pause_movie
// lines there (the originals run when logged, the plain C++ otherwise), Run
// its frame pacing, SetText, LoopNamed, LoopLast and ClearText their
// heap checks. SetText's unset varargs (r7 to r10) are passed as 0 (the
// user's call, 2026-10-30: only dead words of the caller's parameter area,
// +48 to +79 of its frame, differ). The node lookups (UiMovie::FindNode,
// UiMovie::Child) are their plain C++.
constexpr uint32_t kPlay = 0x8211D978;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoop = 0x8211DDD8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kLoopLast = 0x8211DDE8;
constexpr uint32_t kFinish = 0x8211DE38;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kSetNodeText = 0x8211C7E8;
constexpr uint32_t kClearText = 0x8211C470;
// The pause menu's items by kind (six words a kind, -1 ends them).
constexpr uint32_t kPauseItems = 0x8200CBF0;

// The transitions (r3 the step word, 0 on the first call: the page object's
// +24) run the pause menu's movie ([[0x826D84D0]]) once (UiMovie::Run(movie,
// 0)) after their first call's work, and answer whether they're done.
// sub_8220C5E0 PauseMenu::RunTen: the data's +152 counted from 0, done at 10.
uint64_t PauseMenuRunTen(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8220C668 PauseMenu::BackToMain: the selected item's animation (type 7,
// selection + 1) played (r6 1) and looped; done.
uint64_t PauseMenuBackToMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8220C700 PauseMenu::CloseMain: the item's animation and animation 0
// finished, animation 2 played; done when the run answers 0.
uint64_t PauseMenuCloseMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8220C7B8 PauseMenu::MoveMain: the previous item's animation (+20 + 1)
// finished, the new one's played (r6 1) and looped; done.
uint64_t PauseMenuMoveMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8220C868 PauseMenu::ShowOptionsRow: the type-8 row (selection + 4
// below item 4, else 2 or 1 by the data's +144) played and looped; done.
uint64_t PauseMenuShowOptionsRow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8220C930 PauseMenu::ToggleOptions: on item 4, the type-8 row by +144
// finished and the other played and looped; done.
uint64_t PauseMenuToggleOptions(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8220CA28 PauseMenu::ShowVolumeRow: the type-9 row 1 played and
// LoopLast; done.
uint64_t PauseMenuShowVolumeRow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8220CAB0 PauseMenu::ShowAdjustRow: named animation 192 played and
// looped; done.
uint64_t PauseMenuShowAdjustRow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8220C578 PauseMenu::ConfirmPage: RaceDialog::Update (r4 the caller's);
// its 1 (yes) Go(5, transition 5), its 2 (no) Go(0) at once. Returns 0.
uint64_t PauseMenuConfirmPage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_8220CB38 PauseMenu::ShowItems: the type-7 node 0's children (+100 on,
// +104 of them): string 1989 + the kind's item (UiMovie::SetText) for the
// first six items, else hidden (+80 = 0). r3 as the original leaves it.
uint64_t PauseMenuShowItems(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8220CBF8 PauseMenu::SetArrowA: the type-8 node 0's child 0's +16 =
// `value` as a float (its double word left at the frame's +80). Returns the
// child.
uint32_t PauseMenuSetArrowA(uint8_t* base, uint64_t value, uint32_t sp);
// sub_8220CC60 PauseMenu::ShowOptions: for each child of the type-8 node 2,
// its two parts' children 0 and 2 marked on or off (+108, +80) by the
// matching edit (the data's +36 to +48, +144, copied to the frame's +80).
// Returns r3 as the original leaves it.
uint32_t PauseMenuShowOptions(uint8_t* base, uint32_t sp);
// sub_8220D0B8 PauseMenu::ShowVolumes: the type-9 nodes 5 and 6 shown by
// PauseMenu::ShowVolume (sub_8220D040, never seen in play: the original, its
// --log_pause_movie line kept) with the data's +52 and +56. r3 its.
uint64_t PauseMenuShowVolumes(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8220D130 PauseMenu::ShowSlider: UiMovie::SetNodeText(movie, child 0,
// 8, value); child 2's +12 = 16 - (value + 10) x [0x8200A6AC] truncated, as
// a float (the original's words left at the frame's +80). Returns child 2.
uint32_t PauseMenuShowSlider(uint8_t* base, uint32_t slider, uint64_t value, uint32_t sp,
                             const Calls& calls);
// sub_8220D5D0 PauseMenu::RestoreNodes: child `child` (under `group` when
// not -1) of the movie's file, live and saved scenes, UiMovie::RestorePose
// (through its override) on the live and the saved one against the file's.
// r3 its.
uint64_t PauseMenuRestoreNodes(uint8_t* base, uint32_t group, uint32_t child, uint32_t sp,
                               const Calls& calls);
// sub_8220DD98 PauseMenu::MoveOptions (transition 8): the previous row's
// animation finished (item 4's by +144, the arrow at 0 or 120 for items 0
// and 3 through SetArrowA, +144 = 1), the new row's played and looped,
// named animations 20 and 19 finished and, unless either row is 4, 20
// (down) or 19 (up) played; done.
uint64_t PauseMenuMoveOptions(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8220E1F8 PauseMenu::MoveAdjust (transition 18): the same with named
// animations 192 to 196 (item 3 by +144: 195 or 196; leaving it +144 = 0
// and sub_8220D1D8, never seen in play, the original, at 0 or 80 for items
// 0 and 2) and the slide 149 (down) or 148 (up); done.
uint64_t PauseMenuMoveAdjust(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8220E700 PauseMenu::ColorItems: the type-7 node 0's children colored
// (+56 to +64): 205, 235, 67 where MenuList::IsEnabled answers nonzero, else
// 255, 255, 255. r3 as the original leaves it.
uint32_t PauseMenuColorItems(uint8_t* base, uint32_t sp);
// sub_8220E888 PauseMenu::ShowAdjust: the type-10 node 0's children 4 to 6
// each by ShowSlider with the data's +60 to +68 (copied to the frame's +80).
// r3 the last ShowSlider's.
uint32_t PauseMenuShowAdjust(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8220EE28 PauseMenu::ShowMain (transition 2): animation 0 played and
// looped, the selected item's (type 7, selection + 1, r6 1) played and
// looped, ColorItems; done.
uint64_t PauseMenuShowMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8220FA88 PauseMenu::Open: the data's +132 = `kind`, +156 = `flag`, UI
// movie 4 found into the screen's +0, UiMovie::ResetPose, PauseMenu::LoadData,
// ScreenPages::Go(0, transition 1, then 2), MenuSound::Play(6) (both through
// their overrides). r3 MenuSound::Play's.
uint64_t PauseMenuOpen(uint8_t* base, uint32_t screen, uint64_t kind, uint64_t flag, uint32_t sp,
                       const Calls& calls);
// --- The pause menu's options page (options.cpp) ---
//
// MenuList::Update, Accepted and Cancelled and MenuList::Init through their
// overrides (Update's --log_pause_menu lines kept), the sounds through
// MenuSound::Play's override.
constexpr uint32_t kListUpdate = 0x8220C1E0;
constexpr uint32_t kListAccepted = 0x8220BD50;
constexpr uint32_t kListCancelled = 0x8220BDE0;
constexpr uint32_t kListInit = 0x8220BAC0;
// sub_8220CDA0 PauseMenu::ToggleOption: by the list's side (+28: -1 sets, 1
// clears) the selected item's edit (+36 to +48, item 4's +144; their
// addresses at the frame's +80) changed when it differs, then ShowOptions; 1
// when it changed, else 0.
uint32_t PauseMenuToggleOption(uint8_t* base, uint32_t sp);
// sub_8220DB98 PauseMenu::OptionsPage (page 1): see the row. 0 or 8.
uint64_t PauseMenuOptionsPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8220E530 PauseMenu::SetUpOptions: MenuList::Init(list, 5, 5, 1, 154,
// 153, 155, 156; 159, 158), the edits from the options, +144 = 1,
// RestoreNodes(-1, 8), the labels (UiMovie::SetNodeText, unset varargs 0),
// ShowOptions. r3 ShowOptions'.
uint64_t PauseMenuSetUpOptions(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8220F1B8 PauseMenu::SetAdjust (transition 19): on item 3 the named
// animations 195 and 196 swapped by +144, else ShowAdjust; done.
uint64_t PauseMenuSetAdjust(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

// --- The menu after the race's pages and transitions (after_race.cpp) ---
//
// The movie the screen's +0 ([[0x826D8510]], UI movie 9), read again before
// each call as the originals do. The items by kind: four words a kind at
// 0x82010428, -1 ends them.
constexpr uint32_t kAfterItems = 0x82010428;
// sub_8221A6C8 AfterRaceMenu::ConfirmPage (page 6): RaceDialog::Update (r4
// the caller's); its 1 Go(7, transition 4), its 2 Go(1) at once. Returns 0.
uint64_t AfterRaceMenuConfirmPage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls);
// The transitions (r3 the step word, set to 1 first on the first call):
// sub_8221A730 AfterRaceMenu::Intro (animation 0 played; done when the run
// answers 0), sub_8221A7A8 AfterRaceMenu::Show (animation 2, the selected
// item's (type 4, r6 1) and named 40 played and looped; done),
// sub_8221A950 AfterRaceMenu::CloseMain (the item's, animation 2 and named
// 40 finished, animation 1 played; done when the run answers 0),
// sub_8221AA20 AfterRaceMenu::Move (the previous item's finished, the new
// one's played and looped; done).
uint64_t AfterRaceMenuIntro(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t AfterRaceMenuShow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t AfterRaceMenuCloseMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t AfterRaceMenuMove(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8221AC00 AfterRaceMenu::ShowItems: the type-4 node 0's children:
// string 1960 + the kind's item (SetText) for the first four, else hidden.
uint64_t AfterRaceMenuShowItems(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8221B5C8 AfterRaceMenu::SetUpItems: MenuList::Init(list, the data's
// +4 twice, 1, 154, 153, 155, 156; 159, 158), the selection from +16, with
// +8 set the items of id 1 and 2 disabled, ShowItems. r3 ShowItems'.
uint64_t AfterRaceMenuSetUpItems(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8221B768 AfterRaceMenu::ColorItems (r3 the screen): as the pause
// menu's on type 4.
uint32_t AfterRaceMenuColorItems(uint8_t* base, uint32_t screen, uint32_t sp);
// sub_8221B860 AfterRaceMenu::Start (page 0): ColorItems, Go(1, transition
// 1). Returns 0.
uint64_t AfterRaceMenuStart(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8221BA68 AfterRaceMenu::Open: the data's +0 = `kind`, UI movie 9 into
// the screen's +0 and its pose reset, +4 the kind's item count (at most 4),
// +8 = `flag`, +16 = 0, SetUpItems, ScreenPages::Init(pages, 1) and Go(0,
// 0, -1), +20 = 0. r3 Go's.
uint64_t AfterRaceMenuOpen(uint8_t* base, uint32_t screen, uint64_t kind, uint64_t flag,
                           uint32_t sp, const Calls& calls);

// --- The confirm dialog's pages, transitions and draw (dialog.cpp) ---
//
// The dialog's movie found each time (UiMovies::Find(14): the id at the
// frame's +80, the movie table's pair, as the originals do). MenuList::
// Update, Accepted and Cancelled, ScreenPages::Go, the UI movie calls,
// MenuSound::Play and SaveSlots::RequestDevice through their overrides;
// Replays::Delete (sub_822905C0, never seen in play) the original.
constexpr uint32_t kRequestDevice = 0x8228F9B0;
constexpr uint32_t kReplaysDelete = 0x822905C0;
// sub_82217F40 RaceDialog::Question (page 1): see the row. Returns 0.
uint64_t RaceDialogQuestion(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_822186A8 RaceDialog::OpenAnim (transition 1): on its first call by the
// data's +16: 0 named animation 15 played; 1 the selection's (+2) finished
// and 14 played; 2 named 0 finished and 15 played; sound 5. Done when the
// run answers 0, then +16 = +20.
uint64_t RaceDialogOpenAnim(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_82218788 RaceDialog::Move (transition 2): on its first call the
// previous item's named animation (+2) finished, the new one's played and
// looped; done.
uint64_t RaceDialogMove(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_82218F30 RaceDialog::Draw: with a transition running, its handler (0
// ShowMessage, 1 OpenAnim, 2 Move, from a table in the frame, through the
// function table) with the page object's +24, then ScreenPages::Finish (a
// negative transition other than -1: Finish(1)); else UiMovie::Run(movie,
// 0). r3 as the original leaves it.
uint64_t RaceDialogDraw(uint8_t* base, uint32_t sp, const Calls& calls);

constexpr uint32_t kShowVolume = 0x8220D040;
constexpr uint32_t kArrowB = 0x8220D1D8;
constexpr uint32_t kRestorePose = 0x82119B80;
constexpr uint32_t kResetPose = 0x8211ABA8;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;

}  // namespace race_screens
