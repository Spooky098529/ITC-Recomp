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

}  // namespace build_screen
