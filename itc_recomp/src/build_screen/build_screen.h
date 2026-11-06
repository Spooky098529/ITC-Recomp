// The build screen natively (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/build-screen.md): the front-end screen over the
// screen data [0x826D8448] and UI movie 6 whose levels are the states of
// the transition machine [0x826D843C], its menu lists [0x826D8440] and
// [0x826D8444], the groups' and builds' lists and rows, and the garage
// stage's lights mode under it. Plain C++ over guest memory (the screen,
// its lists and the movie are shared with the recompiled code), with the
// originals' loads and stores in their order. `sp` is the caller's stack
// pointer: the back chain of the original's frame and the words it stores
// in that frame (the movie id for UiMovies::Find, MenuList::Init's stack
// arguments) are written there as the original leaves them; its register
// saves aren't. Adapters: src/hooks/build_screen.cpp (behind
// --native_build_screen), register steps shared with the harness in
// src/hooks/build_screen.h.

#pragma once

#include "race_hud_object/race_hud_object.h"

#include <cstdint>

namespace build_screen {

// Guest calls (r3 to r10 from the frame `r1` with the original's return
// address): MenuList::Init and the UI movie animation calls, through their
// overrides.
using Calls = race_hud_object::Calls;

// The globals (pointers).
constexpr uint32_t kMachine = 0x826D843C;  // the transition machine (+24 a word per transition)
constexpr uint32_t kList = 0x826D8440;     // the level's menu list (72 bytes)
constexpr uint32_t kGroupList = 0x826D8444;  // the groups, stepped on levels 1 and 2
constexpr uint32_t kScreen = 0x826D8448;   // the screen data

// The screen data's fields.
constexpr uint32_t kGroups = 36;            // 56 groups of 3,252 bytes
constexpr uint32_t kGroupBytes = 3252;
constexpr uint32_t kGroupCount = 56;
constexpr uint32_t kShownGroups = 0x318C4;  // pointers to the groups shown
constexpr uint32_t kShownCount = 0x319A4;
constexpr uint32_t kBuilds = 0x319A8;       // the build list shown (+1608 its count)
constexpr uint32_t kBuildCount = 0x31FF0;   // a halfword
constexpr uint32_t kCategory = 0x32634;
constexpr uint32_t kGroupSelection = 0x32638, kGroupScroll = 0x3263C;
constexpr uint32_t kBuildSelection = 0x32640, kBuildScroll = 0x32644;
constexpr uint32_t kStandSelection = 0x32648, kStandScroll = 0x3264C;

// A build list's fields (16-bit links: +0 a node's next, +2 its slot).
constexpr uint32_t kListFree = 1600, kListHead = 1604, kListCount = 1608, kListSlots = 1612;
// A group's fields.
constexpr uint32_t kGroupCategory = 4, kGroupMark = 32, kGroupList_ = 40, kGroupBuildCount = 1648;

// Movie 6: its id at `frame` + 80, then UiMovies::Find (rows.cpp).
constexpr uint32_t kMovieId = 6;
uint32_t Movie(uint8_t* base, uint32_t frame);

// --- The menu lists (lists.cpp) ---

// sub_8219BA50 BuildScreen::KeepGroupList, sub_8219BB20 KeepBuildList,
// sub_8219BBF0 KeepStandList: the screen's kept selection and scroll =
// [0x826D8440]'s selection and scroll (+4). Their r3: the selection.
uint32_t KeepGroupList(uint8_t* base, uint32_t sp);
uint32_t KeepBuildList(uint8_t* base, uint32_t sp);
uint32_t KeepStandList(uint8_t* base, uint32_t sp);
// sub_8219BAB0 RestoreGroupList, sub_8219BB80 RestoreBuildList,
// sub_8219BC50 RestoreStandList: MenuList::SetSelection([0x826D8440], the
// kept selection), its scroll = the kept scroll, MenuList::Fit(list, 1).
// Their r3: the list.
uint32_t RestoreGroupList(uint8_t* base, uint32_t sp);
uint32_t RestoreBuildList(uint8_t* base, uint32_t sp);
uint32_t RestoreStandList(uint8_t* base, uint32_t sp);
// sub_8219BF08 BuildScreen::InitGroupLists: both lists over the shown
// groups (MenuList::Init through `calls`), then RestoreGroupList. r3 the
// list.
uint64_t InitGroupLists(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8219C078 BuildScreen::InitStandList: [0x826D8440] over the build
// count, one row. Init's r3.
uint64_t InitStandList(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8219BCC0 BuildScreen::ValueText: the string id of a value, from the
// ten-word table it stores below `sp`, 533 below 0 (no upper check: past 9
// it reads the stack above the table, as the original).
uint32_t ValueText(uint8_t* base, uint32_t sp, uint32_t value);

// --- The build lists and the groups (lists.cpp) ---

// sub_8219C1C8 BuildList::InsertNode: the free head's node linked after the
// node at `position` - 1 (the head for 0); the count + 1.
void InsertNode(uint8_t* base, uint32_t list, uint32_t position);
// sub_8219C230 BuildList::Insert: InsertNode(list, position as a
// halfword), then the slot of the node at `position` = the word at
// `pointer`. r3 the list.
void Insert(uint8_t* base, uint32_t sp, uint32_t list, uint32_t pointer, uint32_t position);
// sub_8219EC50 BuildScreen::MarkGroups: each group's +32 = whether any of
// its builds has +32 not equal to +36. Its r3 (the loop's end, 0x2C760).
uint32_t MarkGroups(uint8_t* base);
// sub_8219CC70 BuildScreen::FilterGroups: the groups of the category with
// builds listed as shown; 0 when none, else the 11 rows' nodes shown or
// hidden and 1.
uint32_t FilterGroups(uint8_t* base, uint32_t sp);

// --- The garage stage's lights (lists.cpp) ---

// sub_8217B7C8 GarageLights::SetMode: `mode` 0 the 11 records from the
// table at 0x82635950 and +1072 (x, x, x, 1.0); 1 every record (0, 0, 0,
// 1.0) and +1072 the same; the vectors built below `sp` first, as the
// original's leaf does.
void LightsSetMode(uint8_t* base, uint32_t sp, uint32_t lights, uint32_t mode);
// sub_8217D0C8 GarageStage::SetDark: SetMode(stage + 4512, dark), stage
// +6060 = dark is 1. r3 stage + 4512.
uint32_t SetDark(uint8_t* base, uint32_t sp, uint32_t holder, uint32_t dark);

// --- The rows, arrows, marks, prompts and help (rows.cpp) ---

// sub_8219D1B0 BuildScreen::GroupArrows and sub_8219D4B8 BuildArrows: the
// up arrow (7, 10) or (8, 2) hidden at scroll 0, the down one (7, 11) or
// (8, 3) hidden when scroll + the rows (11 or 6) is `count`. r3 the down
// arrow.
uint32_t GroupArrows(uint8_t* base, uint32_t sp, uint32_t scroll, uint32_t count);
uint32_t BuildArrows(uint8_t* base, uint32_t sp, uint32_t scroll, uint32_t count);
// sub_8219D7B0 BuildScreen::StandArrows: node (-1, 23) hidden when the
// build count is 1. r3 the node.
uint32_t StandArrows(uint8_t* base, uint32_t sp);
// sub_8219D440 BuildScreen::ShowBuildMark: node (17, 0) hidden when the
// build's +16 is 0, else shown with +108 = the build's +0 - 310. r3 the
// node.
uint32_t ShowBuildMark(uint8_t* base, uint32_t sp, uint32_t build);
// sub_8219DA58 BuildScreen::SetPrompts: `pairs` r3 to r8 (three icon and
// label pairs, -1 for none). r3 the last label node.
uint32_t SetPrompts(uint8_t* base, uint32_t sp, const uint32_t pairs[6]);
// sub_8219DB50 BuildScreen::SetHelp: node (1, 0)'s text reset, set to
// string `id` and scrolled. ScrollNodeText's r3.
uint64_t SetHelp(uint8_t* base, uint32_t sp, uint32_t id, const Calls& calls);
// sub_8219BD30 BuildScreen::PowI: `x` to the `n` (signed) by squaring with
// single-precision products, its reciprocal for n below 0. f1.
double PowI(uint8_t* base, double x, uint32_t n);
// sub_8219D090 BuildScreen::ShowNumber: `number`'s digits on node (12, 3)'s
// children, the leading zeros hidden; node (12, 0) shown at 100. r3 the
// last node found.
uint32_t ShowNumber(uint8_t* base, uint32_t sp, uint32_t number);
// sub_8219CE08 BuildScreen::DrawGroupRow: row `row`'s nodes for the shown
// group `index` (its name, mark and new mark), nothing written past the
// count. r3 SetNodeText's, else the text node.
uint64_t DrawGroupRow(uint8_t* base, uint32_t sp, uint32_t row, uint32_t index,
                      const Calls& calls);
// sub_8219FA28 BuildScreen::DrawGroupRows: the 11 rows from the scroll.
uint64_t DrawGroupRows(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The transitions (transitions.cpp) ---
//
// Each takes `step`, its word at the machine's +24 (r3): the set-up on the
// first call (the word 0, then 1), then UiMovie::Run(movie 6, 0). r3 1 but
// for FadeOut.
//
// sub_8219C528 BuildScreen::HighlightGroups: PlayNamed and LoopNamed 44, 42,
// 43, 80, 84 and 29 + the selection, FinishNamed 76 to 79, PlayNamed(76 +
// the category, chained). sub_8219C6D0 HighlightGroups2: the same without
// 84.
uint64_t HighlightGroupsA(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t HighlightGroupsB(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8219C850 BuildScreen::FadeOut: ScreenFades::FadeOut([0x82001BF0], 0,
// 0, 0, layer 0) first; 1 once both fade counts words are 0, else 0.
uint64_t FadeOut(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8219C8F8 BuildScreen::HighlightBuilds (71, 70, 69, 61 + the
// selection), sub_8219CA08 HighlightStand (96, 97, 108, 93), sub_8219CB18
// HighlightStand2 (108, 93), sub_8219CBD8 HighlightDetails (74): PlayNamed
// then LoopNamed of each.
uint64_t HighlightBuilds(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t HighlightStand(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t HighlightStand2(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t HighlightDetails(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

}  // namespace build_screen
