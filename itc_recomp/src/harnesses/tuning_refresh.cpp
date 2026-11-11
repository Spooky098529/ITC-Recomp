// TuningScreen::Refresh (sub_821E3AD0, src/tuning_screen/refresh.cpp,
// docs/functions/tuning-screen.md's "The refresh") against the recompiled
// one, part of the tuning_screen harness (run at its end, so its groups
// before keep their seeds). Its world is the real one for everything it
// calls, natively or recompiled: the stand's (car_makers.cpp's TuningStand
// world: the garage and its walk, the wheels, the stage's models loading,
// the camera not set up; SetCarStats' race data), the screen in a block of
// its own (its save data the world's), the text movie (tuning_screen_pages'
// LayTextMovie: movies 2 and 13 name it, movie 14 is the missing movie, the
// same; its string table without argument codes, its named animations), and
// a small block for RefreshMovie13's texture table. The callees run as their
// adapters run them on both sides. Each case runs every state 3 to 58 and
// some outside (below 3, above 58, negative), the categories 0 to 6 and the
// items in their play ranges mostly (never 5 to 8). States 5 to 8 have
// groups of their own after them (so their seeds stay), over the same world
// with the sub-scenes 28 to 33 made real (LaySubScenes) and the level, type,
// choice and option lists' fields shaped (ShapeLevelList, ShapeTypeList,
// ShapeChoiceList; state 6's nodes LayTypeNodes, states 7's and 8's
// LayChoiceNodes). Compared: the text records SetText made,
// the globals, the blocks, the stack below r1 (without the original's
// register saves, the top 96 bytes of its frame, and the frame's +40 to +79,
// the varargs SetText's parameter area, where the unset arguments are passed
// 0 natively: the user's call, 2026-10-30), the caller's area, r1, all of
// r3, lr and f1 (as a double).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/tuning_screen.h"
#include "memory/heap.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <bit>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_car_specs);

namespace harness {

// The stand's world (car_makers.cpp): TuningStandWorldBytes() in a block,
// LayTuningStandWorld returning the save data, the globals pointed at it.
uint32_t TuningStandWorldBytes();
uint32_t LayTuningStandWorld(uint8_t* base, uint32_t block, Random& r, int c);
void PointTuningStandWorld(uint8_t* base, uint32_t block);
void LayTuningStandTires(uint8_t* base, uint32_t block, Random& r);
void LayTuningStandEngines(uint8_t* base, uint32_t block);
const std::vector<uint32_t>& TuningStandWorldGlobals();
uint32_t RaceDataBytes();
void ResetRaceData(uint8_t* base, uint32_t allocation, uint32_t count, uint32_t added);

// The text world (tuning_screen_pages.cpp).
constexpr uint32_t kRefreshTextBytes = 0x16000;
constexpr uint32_t kRefreshStringsBytes = 0x9000;
uint32_t LayTextMovie(uint8_t* base, uint32_t block, Random& r);
void MakeTextStrings(uint8_t* base, uint32_t strings);
const std::vector<std::pair<uint32_t, uint32_t>>& TextMovieGlobals();
void PointTextMovieGlobals(uint8_t* base, uint32_t block, uint32_t strings);
void TextMovieRecords(uint8_t* base, uint32_t movie, Outputs& out);

namespace {

namespace ts = tuning_screen;

constexpr uint32_t kScreenBytes = 0x9400;
constexpr uint32_t kMiscBytes = 0x1000;     // RefreshMovie13's movie +28 table and targets
constexpr uint32_t kStackBytes = 0x6000;    // below the call's r1
constexpr uint32_t kCallerBytes = 256;      // above it
constexpr uint32_t kFrame = 368;            // the refresh's frame
constexpr uint32_t kSaves = 96;             // its register saves (r22 to r31, lr, f31)
constexpr uint32_t kAtEnd = 0x82001BE0;     // AnimationTime's answer for none playing

// The regions a case lays and writes back for each version.
struct Region {
  uint32_t address;
  uint32_t bytes;
  std::vector<uint8_t> inputs;
};

uint32_t Small(Random& r, uint32_t below) {
  return r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(below);
}

// The items in each category's play range (CategoryItemCount's).
constexpr uint32_t kItemCounts[7] = {4, 5, 1, 3, 10, 8, 4};

// The sub-scenes 28 to 33 the list pages (states 5 to 8) reach, each in a
// block of its own (LaySubScenes): the sub-scene node, its children 0 and 1
// (the lists' parents), and 64 leaves for each of those two.
constexpr uint32_t kFirstSubScene = 28;
constexpr uint32_t kSubSceneCount = 6;
constexpr uint32_t kBigNode = 0x400;   // 112 child entries from +128
constexpr uint32_t kLeaf = 0x100;      // 16 child entries from +128
constexpr uint32_t kLeaves = 64;       // per parent
constexpr uint32_t kSubSceneBytes = 3 * kBigNode + 2 * kLeaves * kLeaf;
constexpr uint32_t kSubScenesBytes = kSubSceneCount * kSubSceneBytes;
constexpr uint32_t kPoolNodes = 40;    // the text movie's node pool (LayTextMovie)
constexpr uint32_t kPoolNode = 0x400;
constexpr uint32_t kTxt = 0x54585400;  // "TXT\0"

// A node of `bytes` at random, a TXT with its first child's index `first`,
// its child entries the pool's (from the live scene, node 0 of the pool).
void LayNode(uint8_t* base, uint32_t node, uint32_t bytes, uint32_t first, uint32_t pool,
             Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, node + i, uint32_t(r.Next()));
  }
  guest::StoreU32(base, node + 0, kTxt);
  guest::StoreU32(base, node + 100, r.Below(60));
  guest::StoreU32(base, node + 104, r.Below(8));
  guest::StoreU32(base, node + 108, first);
  for (uint32_t e = 0; 128 + 8 * e < bytes; e++) {
    guest::StoreU32(base, node + 128 + 8 * e, pool + kPoolNode * r.Below(kPoolNodes));
  }
}

// The sub-scenes 28 to 33 made real in `block` (the text movie laid): the
// live scene's entries for them pointed at their nodes, each sub-scene's
// children 0 and 1 its own parents (first child's index 0), whose children
// 0 to 63 are their own distinct leaves (TXT, index below 8, entries the
// pool's), so the pages' rows, marks and markers are distinct nodes as in
// the game, never the scene's own (the original's state 5 writes the +108 of
// its rows' marks: on the pool's nodes that changed the scene's children).
void LaySubScenes(uint8_t* base, uint32_t block, uint32_t movie, Random& r) {
  const uint32_t scene = guest::LoadU32(base, movie + ui_movies::kMovieLive);
  const uint32_t pool = scene;
  for (uint32_t k = 0; k < kSubSceneCount; k++) {
    const uint32_t sub = block + k * kSubSceneBytes;
    const uint32_t parents[2] = {sub + kBigNode, sub + 2 * kBigNode};
    LayNode(base, sub, kBigNode, 0, pool, r);
    guest::StoreU32(base, sub + 128, parents[0]);
    guest::StoreU32(base, sub + 128 + 8, parents[1]);
    for (uint32_t p = 0; p < 2; p++) {
      LayNode(base, parents[p], kBigNode, 0, pool, r);
      const uint32_t leaves = sub + 3 * kBigNode + p * kLeaves * kLeaf;
      for (uint32_t j = 0; j < kLeaves; j++) {
        const uint32_t leaf = leaves + j * kLeaf;
        LayNode(base, leaf, kLeaf, r.Below(8), pool, r);
        guest::StoreU32(base, parents[p] + 128 + 8 * j, leaf);
      }
    }
    const uint32_t index = guest::LoadU32(base, scene + 108) + kFirstSubScene + k;
    guest::StoreU32(base, scene + 128 + 8 * index, sub);
  }
}

// State 5's fields as play gives them: the level +9661 0 to 9 (now and then
// to 15) where the stand's world takes any (EquipChoice stores it into the
// garage car, which SetCarStats then reads): the engine stage (category 0
// item 0) 0 to 9, the world's engine records (LayTuningStandEngines: the
// car's engine is copied from the spec's record at that stage, unchecked;
// a random record's torque curve count ran its copy off the memory), the
// power train's +1676 (item 3) 0 to 6 (a frame
// table of 7, unchecked), the tire grades (category 3) 1 to 5 (the world's
// tire records); the list's first row +25100 the level minus 0 to 7 (now and then
// 0, or above the level by 1 or 2), its rows +9677 1 to 10 (now and then 0
// or 11: the rows' children are indexed unchecked), the power list's row
// +33900 0 to 4 (now and then 5 to 7), and the words IsItemAvailable,
// GetChoiceFlag and GetCurrentChoice read (+34000 to +36800) 0 a quarter of
// the time, else small, now and then any word.
void ShapeLevelList(uint8_t* base, uint32_t screen, uint32_t category, uint32_t item,
                    Random& r) {
  uint32_t level = r.Below(8) == 0 ? r.Below(16) : r.Below(10);
  if (category == 0 && item == 0) {
    level = r.Below(10);
  } else if (category == 0 && item == 3) {
    level = r.Below(7);
  } else if (category == 3) {
    level = 1 + r.Below(5);
  }
  guest::StoreU8(base, screen + tuning_screen::kLevel, uint8_t(level));
  uint32_t first;
  switch (r.Below(8)) {
    case 0: first = 0; break;
    case 1: first = level + 1 + r.Below(2); break;
    default: {
      const uint32_t back = r.Below(8);
      first = level >= back ? level - back : 0;
      break;
    }
  }
  guest::StoreU32(base, screen + 25100, first);
  const uint32_t rows = r.Below(10) == 0 ? (r.Below(2) == 0 ? 0 : 11) : 1 + r.Below(10);
  guest::StoreU8(base, screen + 9677, uint8_t(rows));
  guest::StoreU32(base, screen + tuning_screen::kPartSet,
                  r.Below(8) == 0 ? 5 + r.Below(3) : r.Below(5));
  for (uint32_t at = 34000; at < 36800; at += 4) {
    const uint32_t word =
        r.Below(4) == 0 ? 0 : (r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(10));
    guest::StoreU32(base, screen + at, word);
  }
}

// State 6's nodes as the game has them (after LaySubScenes): sub-scene 29's
// own children 2 to 20 (its type list's rows, category 0 item 2) distinct
// leaves (its child 1's leaves 32 to 50, which no page reaches), and the
// live scene's children 0 and 17 (the interior's picture and the aero's
// part picture, whose child 0's +108 the page writes: on the pool's nodes
// that changed the scene's children) sub-scene 32's two parents (state 7's
// only), so their children 0 are distinct leaves.
void LayTypeNodes(uint8_t* base, uint32_t block, uint32_t movie) {
  const uint32_t scene = guest::LoadU32(base, movie + ui_movies::kMovieLive);
  const auto sub = [&](uint32_t k) { return block + (k - kFirstSubScene) * kSubSceneBytes; };
  const uint32_t sub29 = sub(29);
  const uint32_t leaves = sub29 + 3 * kBigNode + kLeaves * kLeaf;  // child 1's
  for (uint32_t n = 2; n <= 20; n++) {
    guest::StoreU32(base, sub29 + 128 + 8 * n, leaves + (30 + n) * kLeaf);
  }
  const uint32_t first = guest::LoadU32(base, scene + 108);
  guest::StoreU32(base, scene + 128 + 8 * (first + 17), sub(32) + kBigNode);
  guest::StoreU32(base, scene + 128 + 8 * (first + 0), sub(32) + 2 * kBigNode);
}

// State 6's fields as play gives them: the type list's rows +9678 1 to 10
// (now and then 0, 11 or 12: the rows' children are indexed unchecked), the
// type +9662 below the rows and 6 (the aero below 5, below 6 on the rear
// spoiler with +25172 set: AeroPartValue reads its table of four types per
// car unchecked; EquipChoice stores it into the garage car, which
// SetCarStats reads), now and then up to 12 elsewhere; the list's first row
// +25108 the type minus 0 to 7 (now and then 0, or above the type by 1 or
// 2); +25172 0 or set; the words IsItemAvailable and GetChoiceFlag read
// (+34000 to +36800) 0 a quarter of the time, else small, now and then any
// word; the body kit +36588 0 a third of the time, else 1 to 3 (AeroParts::
// Get82's record, read unchecked), now and then 4 to 7.
void ShapeTypeList(uint8_t* base, uint32_t screen, uint32_t category, uint32_t item,
                   Random& r) {
  const uint32_t rows = r.Below(10) == 0 ? (r.Below(3) == 0 ? 0 : 11 + r.Below(2))
                                         : 1 + r.Below(10);
  guest::StoreU8(base, screen + 9678, uint8_t(rows));
  const bool shift = r.Below(2) != 0;
  guest::StoreU32(base, screen + 25172, shift ? (r.Below(2) == 0 ? 1 : uint32_t(r.Next())) : 0);
  uint32_t type;
  if (category == 4) {
    type = r.Below(item == 3 && shift ? 6 : 5);
  } else if (r.Below(8) == 0) {
    type = r.Below(13);
  } else {
    const uint32_t below = rows == 0 ? 1 : (rows < 6 ? rows : 6);
    type = r.Below(below);
  }
  guest::StoreU8(base, screen + tuning_screen::kType, uint8_t(type));
  uint32_t first;
  switch (r.Below(8)) {
    case 0: first = 0; break;
    case 1: first = type + 1 + r.Below(2); break;
    default: {
      const uint32_t back = r.Below(8);
      first = type >= back ? type - back : 0;
      break;
    }
  }
  guest::StoreU32(base, screen + 25108, first);
  for (uint32_t at = 34000; at < 36800; at += 4) {
    const uint32_t word =
        r.Below(4) == 0 ? 0 : (r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(10));
    guest::StoreU32(base, screen + at, word);
  }
  guest::StoreU32(base, screen + 36588,
                  r.Below(3) == 0 ? 0 : (r.Below(8) == 0 ? 4 + r.Below(4) : 1 + r.Below(3)));
}

// States 7's and 8's nodes as the game has them (after LaySubScenes and
// LayTypeNodes), from leaves no other page reaches: sub-scene 32's children
// 2 and 3 (state 7's swatches: category 1 item 3's and the interior's) its
// child 0's leaves 10 and 11 (their first child's index 0), whose children 1
// to 6 are its child 0's leaves 12 to 23 and those leaves' child 2 its child
// 1's leaves 10 to 21 (without them the pages wrote the colors and +80 of
// the pool's nodes, the live scene's among them); sub-scene 31's own
// children 5 to 14 (state 8's bonnet rows) its child 0's leaves 20 to 29.
void LayChoiceNodes(uint8_t* base, uint32_t block) {
  const auto sub = [&](uint32_t k) { return block + (k - kFirstSubScene) * kSubSceneBytes; };
  const auto leaf = [&](uint32_t k, uint32_t parent, uint32_t j) {
    return sub(k) + 3 * kBigNode + parent * kLeaves * kLeaf + j * kLeaf;
  };
  const auto set_child = [&](uint32_t node, uint32_t i, uint32_t child) {
    const uint32_t first = guest::LoadU32(base, node + 108);
    guest::StoreU32(base, node + 128 + 8 * (first + i), child);
  };
  const uint32_t sub32 = sub(32);
  for (uint32_t p = 0; p < 2; p++) {
    const uint32_t parent = leaf(32, 0, 10 + p);
    guest::StoreU32(base, parent + 108, 0);
    guest::StoreU32(base, sub32 + 128 + 8 * (2 + p), parent);
    for (uint32_t k = 1; k <= 6; k++) {
      const uint32_t row = leaf(32, 0, 12 + 6 * p + (k - 1));
      set_child(parent, k, row);
      set_child(row, 2, leaf(32, 1, 10 + 6 * p + (k - 1)));
    }
  }
  const uint32_t sub31 = sub(31);
  for (uint32_t n = 5; n < 15; n++) {
    guest::StoreU32(base, sub31 + 128 + 8 * n, leaf(31, 0, 15 + n));
  }
}

// States 7's and 8's fields as play gives them: the level +9661 below 10
// (the tires' grades 1 to 5: GetChoiceFlag reads +12 level, the world's tire
// records; category 1 item 3 below 8, SetCaliper's word); the type +9662
// below 5 (the bonnet's AeroPartValue reads its table of four types per car
// unchecked, GetChoiceFlag + 8 type), now and then up to 12 elsewhere (the
// interior's swatches and pictures check it); +9663 0 to 3 (the bonnet's
// EquipChoice and PreviewItem 0 or 1), now and then any byte where nothing
// indexes with it (the bonnet, the interior: SetInteriorPair checks it); the
// option +9664 0 to 2 (the tires' hardness, the door's kind, the bonnet's),
// now and then below 16 (the help line's string by it); the rows +9679 and
// +9680 0 to 4, now and then up to 12 (the marks' children are indexed
// unchecked: 38 + 2 row within each parent's 64 leaves); the words
// GetChoiceFlag reads (+34000 to +36800) 0 a quarter of the time, else
// small, now and then any word.
void ShapeChoiceList(uint8_t* base, uint32_t screen, uint32_t category, uint32_t item,
                     Random& r) {
  const bool bonnet = category == 4 && item == 4;
  const bool interior = category == 6 && item == 0;
  uint32_t level = r.Below(10);
  if (category == 3) {
    level = 1 + r.Below(5);
  } else if (category == 1 && item == 3) {
    level = r.Below(8);
  }
  guest::StoreU8(base, screen + tuning_screen::kLevel, uint8_t(level));
  uint32_t type = r.Below(5);
  if (!bonnet && category != 4 && r.Below(8) == 0) {
    type = r.Below(13);
  }
  guest::StoreU8(base, screen + tuning_screen::kType, uint8_t(type));
  uint32_t type2 = r.Below(4);
  if ((bonnet || interior) && r.Below(8) == 0) {
    type2 = r.Below(256);
  }
  guest::StoreU8(base, screen + tuning_screen::kType2, uint8_t(type2));
  const uint32_t option = r.Below(8) == 0 && !(category == 5 && item == 1) ? r.Below(16)
                                                                             : r.Below(3);
  guest::StoreU8(base, screen + tuning_screen::kOption, uint8_t(option));
  for (const uint32_t at : {9679u, 9680u}) {
    guest::StoreU8(base, screen + at, uint8_t(r.Below(8) == 0 ? r.Below(13) : r.Below(5)));
  }
  for (uint32_t at = 34000; at < 36800; at += 4) {
    const uint32_t word =
        r.Below(4) == 0 ? 0 : (r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(10));
    guest::StoreU32(base, screen + at, word);
  }
}

}  // namespace

// For TuningMachine::Start's cases (tuning_machine.cpp): the sub-scenes'
// block and LaySubScenes.
uint32_t RefreshSubScenesBytes() { return kSubScenesBytes; }
void LayRefreshSubScenes(uint8_t* base, uint32_t block, uint32_t movie, Random& r) {
  LaySubScenes(base, block, movie, r);
}

void TuningRefresh(Context& h) {
  if (!hooks::TuningStandCalleesNative() || !hooks::TuningCarStatsCalleesNative() ||
      !ITC_FLAG(native_car_specs)) {
    ITC_LOG_WARN("harness: tuning_screen: TuningScreen::Refresh skipped (its callees' plain C++ "
                 "isn't native)");
    return;
  }
  uint8_t* const base = h.base;
  const uint32_t stand_bytes = TuningStandWorldBytes();
  const uint32_t stand_allocation = memory::Alloc(stand_bytes + 16, 0);
  const uint32_t text_allocation = memory::Alloc(kRefreshTextBytes + 16, 0);
  const uint32_t screen = memory::Alloc(kScreenBytes, 0);
  const uint32_t misc = memory::Alloc(kMiscBytes, 0);
  const uint32_t strings = memory::Alloc(kRefreshStringsBytes, 0);
  const uint32_t race = memory::Alloc(RaceDataBytes(), 0);
  const uint32_t sub_scenes = memory::Alloc(kSubScenesBytes, 0);
  if (stand_allocation == 0 || text_allocation == 0 || screen == 0 || misc == 0 ||
      strings == 0 || race == 0 || sub_scenes == 0) {
    ITC_LOG_ERROR("harness: tuning_screen: no memory for TuningScreen::Refresh's world");
    for (const uint32_t a :
         {stand_allocation, text_allocation, screen, misc, strings, race, sub_scenes}) {
      if (a != 0) {
        memory::Free(a);
      }
    }
    return;
  }
  const uint32_t block = (stand_allocation + 15) & ~15u;
  const uint32_t text = (text_allocation + 15) & ~15u;
  MakeTextStrings(base, strings);

  // The globals the world points, saved and put back at the end.
  std::vector<std::pair<uint32_t, uint32_t>> globals = TextMovieGlobals();
  for (const uint32_t g : TuningStandWorldGlobals()) {
    globals.push_back({g, 4});
  }
  std::vector<std::vector<uint8_t>> saved;
  for (const auto& [address, bytes] : globals) {
    saved.push_back(Read(base, address, bytes));
  }

  // One case: the world for `state`, `category` and `item` (the rest from
  // `r`), `shape` (when given) run on it after the text movie is laid,
  // then both versions run and compared.
  const auto run = [&](Random& r, Results& results, int c, uint32_t state, uint32_t category,
                       uint32_t item, const std::function<void(uint32_t movie)>& shape) {
    // The stand's world.
    const uint32_t save = LayTuningStandWorld(base, block, r, c);
    PointTuningStandWorld(base, block);
    // The screen at random, its save data the world's.
    for (uint32_t i = 0; i < kScreenBytes; i += 4) {
      guest::StoreU32(base, screen + i, uint32_t(r.Next()));
    }
    const auto field = [&](uint32_t at, uint32_t value) { guest::StoreU32(base, screen + at, value); };
    const auto byte = [&](uint32_t at, uint32_t value) {
      guest::StoreU8(base, screen + at, uint8_t(value));
    };
    field(ts::kSaveData, save);
    field(ts::kState, state);
    field(ts::kCategory, category);
    byte(ts::kItem, item);
    byte(ts::kLevel, r.Below(4) == 0 ? r.Below(256) : r.Below(8));
    byte(ts::kType, r.Below(4) == 0 ? r.Below(256) : r.Below(6));
    byte(ts::kType2, r.Below(6) == 0 ? r.Below(256) : r.Below(3));
    byte(ts::kOption, r.Below(6) == 0 ? r.Below(256) : r.Below(3));
    field(9716, r.Below(2) == 0 ? 0 : (r.Below(2) == 0 ? 1 : uint32_t(r.Next())));
    for (const uint32_t at : {9720u, 9724u}) {
      field(at, r.Below(3) == 0 ? 1 : 0);
    }
    // SetCarStats' drive type and aspiration (as its cases).
    field(20788, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(6));
    field(20784, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(3));
    // The aero list's first row +25100: the item minus 0 to 7 (the rows'
    // strings and availability words stay the list's), now and then 0; its
    // rows +9676 0 to 10, now and then up to 12; the availability words
    // (+25128 + 4 k) 0 or set.
    {
      const uint32_t back = r.Below(8);
      field(25100, r.Below(6) == 0 ? 0 : (item >= back ? item - back : 0));
    }
    byte(9676, r.Below(6) == 0 ? r.Below(13) : r.Below(11));
    for (uint32_t k = 0; k < 280; k++) {
      field(25128 + 4 * k, r.Below(3) == 0 ? 0 : (r.Below(2) == 0 ? 1 : uint32_t(r.Next())));
    }
    // PreviewItem's tables (as its cases): the choice words, the picks.
    const auto choice_word = [&] {
      return r.Below(4) == 0 ? 0xFFFFFFFFu : 10 * r.Below(10) + r.Below(6);
    };
    field(ts::kPartSet, r.Below(4));
    field(25172, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    field(9712, r.Below(4) != 0 ? 0 : uint32_t(r.Next()));
    field(33812, choice_word());
    field(33816, choice_word());
    field(33548, r.Below(6));
    field(33556, r.Below(6));
    // PickCameraPreset's picks (as its cases).
    field(30492, Small(r, 5));
    field(30724, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(2));
    field(25196, Small(r, 4));
    field(25072, r.Below(4) == 0 ? 0xFFFFFFFFu : 14 + r.Below(51));
    // SetStateTexts' (states 15 to 17): the wheel inch, the group and the
    // first tab (the tab table's index +25204 - +25212 + 0 to 3 within its
    // 17 entries).
    field(25228, r.Below(8));
    {
      const uint32_t group = r.Below(3) == 0 ? (r.Below(2) == 0 ? 0 : 16) : 1 + r.Below(15);
      field(25204, group);
      field(25212, uint32_t(int32_t(group) - int32_t(3 + r.Below(11)) + 3));
    }
    // RefreshMovie13's (its cases'): the picks, the floats, the name record
    // at +20792 (also the header's) naming strings two times in three.
    field(30740, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(5));
    field(30748, r.Below(6) == 0 ? uint32_t(-int32_t(r.Below(3))) : r.Below(8));
    field(30764, r.Below(6));
    field(30768, r.Below(6));
    field(30848, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    for (const uint32_t at : {30840u, 30844u, 20780u}) {
      guest::StoreFloat(base, screen + at, (float(r.Below(200000)) - 100000.0f) / 64.0f);
    }
    if (r.Below(3) != 0) {
      const uint32_t name = screen + 20792;
      const uint32_t firsts[] = {0, 1, 4, 5};
      guest::StoreU32(base, name + 4, firsts[r.Below(4)]);
      guest::StoreU16(base, name + 8, uint16_t(r.Below(4)));
      guest::StoreU8(base, name + 10, uint8_t(r.Below(2)));
      guest::StoreU32(base, name + 12, 2 + r.Below(2));
      guest::StoreU16(base, name + 16, uint16_t(r.Below(6)));
      guest::StoreU8(base, name + 18, uint8_t(r.Below(2)));
    }
    // The light kits' (states 19 to 22): their group +30492 (above), its
    // words small.
    for (const uint32_t at : {30500u, 30508u, 30524u, 30528u, 30544u, 30548u, 30556u, 30560u}) {
      field(at, Small(r, 5));
    }
    // The interior's (states 53 to 58): the mode +33564 0 to 4, the slot
    // +33576 0 to 4, the picks +33540 and +33572 small, the five choice
    // codes +33800 -1 or tens and ones below 5 (the frame's tables of five
    // read unchecked), +35708 small.
    field(33540, r.Below(3));
    field(33564, r.Below(5));
    field(33572, r.Below(5));
    field(33576, r.Below(5));
    for (uint32_t k = 0; k < 5; k++) {
      field(33800 + 4 * k, r.Below(4) == 0 ? 0xFFFFFFFFu : 10 * r.Below(5) + r.Below(5));
    }
    field(35708, r.Below(5));

    // The text movie, its +28 table in the misc block (RefreshMovie13's
    // state 24 stores through its pointers).
    const uint32_t movie = LayTextMovie(base, text, r);
    for (uint32_t i = 0; i < kMiscBytes; i += 4) {
      guest::StoreU32(base, misc + i, 0);
    }
    guest::StoreU32(base, movie + 28, misc);
    for (const uint32_t slot : {700u, 708u, 712u, 724u, 732u, 736u}) {
      guest::StoreU32(base, misc + slot, misc + 0x800 + 4 * r.Below(64));
    }
    // State 4's animations: now and then the scene's animation 1 or 2
    // playing (looping) at the end time or another.
    if (r.Below(3) == 0) {
      const uint32_t looping = movie + ui_movies::kMovieLooping;
      const uint32_t n = 1 + r.Below(2);
      for (uint32_t k = 0; k < n; k++) {
        ui_movies::Slot slot{};
        slot[0] = 1 + r.Below(2);
        slot[1] = 0xFFFFFFFFu;
        slot[2] = 0xFFFFFFFFu;
        slot[5] = r.Below(3) != 0 ? guest::LoadU32(base, kAtEnd) : uint32_t(r.Next());
        ui_movies::Append(base, looping, slot);
      }
    }
    if (shape) {
      shape(movie);
    }
    const uint32_t race_count = r.Below(6), race_added = r.Below(3);

    std::vector<Region> regions = {{block, stand_bytes, {}},
                                   {screen, kScreenBytes, {}},
                                   {text, kRefreshTextBytes, {}},
                                   {misc, kMiscBytes, {}},
                                   {sub_scenes, kSubScenesBytes, {}}};
    for (Region& region : regions) {
      region.inputs = Read(base, region.address, region.bytes);
    }
    PPCContext start = CallContext(h);
    start.r1.u64 &= ~0xFull;
    start.r3.u64 = screen | (r.Below(4) == 0 ? r.Next() << 32 : 0);
    start.lr = 0x82000000 | (r.Below(0x10000) << 2);
    start.f1.f64 = double(int32_t(r.Below(2000)) - 1000) / 8.0;
    const uint32_t sp = start.r1.u32;

    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (const Region& region : regions) {
        Write(base, region.address, region.inputs.data(), region.bytes);
      }
      Fill(base, sp - kStackBytes, kStackBytes + kCallerBytes);
      PointTuningStandWorld(base, block);
      PointTextMovieGlobals(base, text, strings);
      ResetRaceData(base, race, race_count, race_added);
      PPCContext t = start;
      t.fpscr.disableFlushModeUnconditional();
      (v == 0 ? __imp__sub_821E3AD0 : hooks::TuningRefresh)(t, base);
      // The recompiled callee's frame (RefreshPaint's and RefreshMovie14's
      // 112 bytes with r30 and r31 saved inline, RefreshLightKits' 144 with
      // r27 to r31, RefreshInterior's 176 with r22 to r31): its saves of
      // the caller's registers and its own parameter area (+40 to +79,
      // where the varargs SetText and SetNodeText it calls store r7 to r10,
      // registers it inherits unset from its caller) left out: the native
      // refresh doesn't hold the original's registers (register saves
      // aren't reproduced, and the varargs leftovers are the user's call of
      // 2026-10-30).
      {
        const uint32_t frame = sp - kFrame;
        const int32_t st = int32_t(state);
        const bool movie13 = st <= 37 && int32_t(category) == 5 && item == 6;
        uint32_t size = 0, saves = 0;
        if (st == 18 || (st >= 23 && st <= 52 && !movie13)) {
          size = 112, saves = 16;
        } else if (st >= 19 && st <= 22) {
          size = 144, saves = 40;
        } else if (st >= 53 && st <= 58) {
          size = 176, saves = 80;
        }
        if (size != 0) {
          Fill(base, frame - 8 - saves, saves, 0);
          Fill(base, frame - size + 40, 40, 0);
        }
        // RefreshInterior's callee PreviewInteriorChoices (sub_821E7E48,
        // state 57) saves r23, which RefreshInterior doesn't set: the
        // refresh's (0x826D83E0 in the original).
        if (size == 176) {
          Fill(base, frame - size - 80, 8, 0);
        }
        // PreviewItem's (160-byte frame) as its own cases leave them out:
        // the 32 under its frame (its guest callees' saves of the caller's
        // registers), the 56 from 112 under it (the saves of a guest
        // callee's callees) and the 16 from 16 under RefitAxles' 4,000-byte
        // frame.
        const uint32_t preview = frame - 160;
        Fill(base, preview - 32, 32, 0);
        Fill(base, preview - 112 - 56, 56, 0);
        Fill(base, preview - 4000 - 16 - 16, 16, 0);
      }
      TextMovieRecords(base, movie, out[v]);
      for (const auto& [address, bytes] : globals) {
        out[v].Bytes(Read(base, address, bytes));
      }
      // The stack: below the frame's parameter area, then from it to the
      // register saves.
      const uint32_t frame = sp - kFrame;
      out[v].Bytes(Read(base, sp - kStackBytes, kStackBytes - kFrame + 40));
      out[v].Bytes(Read(base, frame + 80, kFrame - 80 - kSaves));
      out[v].Bytes(Read(base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(uint32_t(t.lr));
      out[v].Double(t.f1.u64);
      for (const Region& region : regions) {
        out[v].Bytes(Read(base, region.address, region.bytes));
      }
      out[v].Bytes(Read(base, race, RaceDataBytes()));
    }
    results.Check(out[0], out[1],
                  fmt::format("case {} (state {}, category {}, item {})", c, int32_t(state),
                              int32_t(category), item));
  };

  Random r(0x5265667265736800ull);  // "Refresh"
  Results results("TuningScreen::Refresh (sub_821E3AD0)");
  constexpr int kCases = 200;
  for (int c = 0; c < kCases; c++) {
    // The state: each of 3, 4 and 9 to 58 in turn for the first 104 cases
    // (two rounds), then 3 and 4 mostly, the dispatch's states, and outside 3
    // to 58 (below, above, negative). States 5 to 8 aren't among them: they
    // have their groups below.
    uint32_t state;
    if (c < 104) {
      const uint32_t k = uint32_t(c % 52);
      state = k < 2 ? 3 + k : 7 + k;
    } else {
      static constexpr uint32_t kOutside[] = {0, 1, 2, 59, 60, 63, 100, 0xFFFFFFFFu,
                                              0xFFFFFFFDu, 0x80000000u};
      switch (r.Below(6)) {
        case 0: state = kOutside[r.Below(10)]; break;
        case 1: state = 15 + r.Below(44); break;
        case 2: state = 9 + r.Below(6); break;
        default: state = 3 + r.Below(2); break;
      }
    }
    // The category 0 to 6 mostly, now and then 7, -1 or any word; the item
    // in its range mostly, now and then any byte.
    uint32_t category;
    switch (r.Below(10)) {
      case 0: category = r.Below(2) == 0 ? 7 : 0xFFFFFFFFu; break;
      case 1: category = r.Below(4) == 0 ? uint32_t(r.Next()) : 4; break;
      default: category = r.Below(7); break;
    }
    // State 4 the aero's list half the time (its rows' path), states 23 to
    // 37 the vinyl (category 5, item 6: RefreshMovie13) half the time.
    if (state == 4 && r.Below(2) == 0) {
      category = 4;
    }
    const bool vinyl = state >= 23 && state <= 37 && r.Below(2) == 0;
    if (vinyl) {
      category = 5;
    }
    const uint32_t count = category < 7 ? kItemCounts[category] : 10;
    const uint32_t item = vinyl ? 6 : r.Below(10) == 0 ? r.Below(256) : r.Below(count);
    run(r, results, c, state, category, item, nullptr);

  }
  results.Report();

  // State 5, LEVEL_SELECT (part 2 of the migration), a group of its own so
  // the cases above keep their seeds: the same world, then the sub-scenes'
  // nodes (LaySubScenes) and the level list's fields (ShapeLevelList).
  {
    Random r5(0x5265667265736805ull);  // "Refresh", 5
    Results results5("TuningScreen::Refresh (sub_821E3AD0) state 5");
    constexpr int kLevelCases = 420;
    for (int c = 0; c < kLevelCases; c++) {
      // The pages with a level list mostly, each in turn: the power parts'
      // items 0 to 3, the power train's 0 to 4 (item 4 twice: its rows
      // named through the 8-bit table), the category 2 item and the tires'
      // 0 to 2; now and then the categories 4 to 6, 7, -1 or any word, or
      // any item byte.
      static constexpr uint32_t kPages[][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0},
                                               {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 4},
                                               {2, 0}, {3, 0}, {3, 1}, {3, 2}};
      constexpr uint32_t kPageCount = sizeof(kPages) / sizeof(kPages[0]);
      uint32_t category = kPages[uint32_t(c) % kPageCount][0];
      uint32_t item = kPages[uint32_t(c) % kPageCount][1];
      switch (r5.Below(10)) {
        case 0: category = 4 + r5.Below(3); item = r5.Below(kItemCounts[category]); break;
        case 1:
          category = r5.Below(2) == 0 ? 7 : (r5.Below(2) == 0 ? 0xFFFFFFFFu : uint32_t(r5.Next()));
          item = r5.Below(10);
          break;
        case 2: item = r5.Below(256); break;
        default: break;
      }
      run(r5, results5, c, 5, category, item, [&](uint32_t movie) {
        LayTuningStandTires(base, block, r5);
        LayTuningStandEngines(base, block);
        LaySubScenes(base, sub_scenes, movie, r5);
        ShapeLevelList(base, screen, category, item, r5);
        // The power parts' pages: their power row +33900 0 to 5 in turn
        // (their help lines by it; 5 none).
        if (category == 0) {
          guest::StoreU32(base, screen + ts::kPartSet, uint32_t(c) / kPageCount % 6);
        }
      });
    }
    results5.Report();
  }

  // State 6, TYPE_SELECT (part 3 of the migration), a group of its own after
  // state 5's: the same world with the sub-scenes laid, state 6's nodes
  // (LayTypeNodes) and the type list's fields (ShapeTypeList).
  {
    Random r6(0x5265667265736806ull);  // "Refresh", 6
    Results results6("TuningScreen::Refresh (sub_821E3AD0) state 6");
    constexpr int kTypeCases = 480;
    for (int c = 0; c < kTypeCases; c++) {
      // The pages with a type list mostly, each in turn: category 0 item 2
      // (sub-scene 29), the aero's items 0 to 9 (item 3, the rear spoiler,
      // three times: its help line by the garage car's aero parts), the
      // exterior's items 2 and 3, the interior's 0 to 2; then the pages
      // without (category 0's other items, 1 to 3, the exterior's 0, 1 and 4
      // to 7, the interior's 3, the aero's 10); now and then 7, -1 or any
      // word, or any item byte.
      static constexpr uint32_t kPages[][2] = {
          {0, 2}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 3}, {4, 3}, {4, 4}, {4, 5}, {4, 6},
          {4, 7}, {4, 8}, {4, 9}, {5, 2}, {5, 3}, {6, 0}, {6, 1}, {6, 2}, {0, 2}, {6, 0},
          {0, 0}, {0, 1}, {0, 3}, {1, 2}, {2, 0}, {3, 1}, {5, 0}, {5, 1}, {5, 4}, {5, 7},
          {6, 3}, {4, 10}};
      constexpr uint32_t kPageCount = sizeof(kPages) / sizeof(kPages[0]);
      uint32_t category = kPages[uint32_t(c) % kPageCount][0];
      uint32_t item = kPages[uint32_t(c) % kPageCount][1];
      switch (r6.Below(12)) {
        case 0:
          category = r6.Below(2) == 0 ? 7 : (r6.Below(2) == 0 ? 0xFFFFFFFFu : uint32_t(r6.Next()));
          item = r6.Below(10);
          break;
        case 1:
          if (category != 4) {
            item = r6.Below(256);
          }
          break;
        default: break;
      }
      run(r6, results6, c, 6, category, item, [&](uint32_t movie) {
        LayTuningStandTires(base, block, r6);
        LayTuningStandEngines(base, block);
        LaySubScenes(base, sub_scenes, movie, r6);
        LayTypeNodes(base, sub_scenes, movie);
        ShapeTypeList(base, screen, category, item, r6);
      });
    }
    results6.Report();
  }

  // States 7 and 8 (part 4 of the migration), groups of their own after
  // state 6's: the same world with state 6's nodes, states 7's and 8's
  // (LayChoiceNodes) and the fields shaped (ShapeChoiceList).
  const auto choice_group = [&](uint64_t seed, const char* name, uint32_t state,
                                const std::vector<std::pair<uint32_t, uint32_t>>& pages,
                                int cases) {
    Random rc(seed);
    Results resultsc(name);
    for (int c = 0; c < cases; c++) {
      uint32_t category = pages[uint32_t(c) % pages.size()].first;
      uint32_t item = pages[uint32_t(c) % pages.size()].second;
      switch (rc.Below(12)) {
        case 0:
          category = rc.Below(2) == 0 ? 7 : (rc.Below(2) == 0 ? 0xFFFFFFFFu : uint32_t(rc.Next()));
          item = rc.Below(10);
          break;
        case 1:
          if (category != 4 && category != 1) {
            item = rc.Below(256);
          }
          break;
        default: break;
      }
      run(rc, resultsc, c, state, category, item, [&](uint32_t movie) {
        LayTuningStandTires(base, block, rc);
        LayTuningStandEngines(base, block);
        LaySubScenes(base, sub_scenes, movie, rc);
        LayTypeNodes(base, sub_scenes, movie);
        LayChoiceNodes(base, sub_scenes);
        ShapeChoiceList(base, screen, category, item, rc);
      });
    }
    resultsc.Report();
  };
  // State 7: the bonnet (category 4 item 4), category 1 item 3's swatches
  // and the interior's (category 6 item 0, its types 0 to 4 at random) each
  // several times, then the pages without (the aero's other items, the power
  // train's, the interior's others, categories 0, 2, 3 and 5).
  choice_group(0x5265667265736807ull, "TuningScreen::Refresh (sub_821E3AD0) state 7", 7,
               {{4, 4}, {1, 3}, {6, 0}, {4, 4}, {1, 3}, {6, 0}, {4, 4}, {6, 0}, {6, 0},
                {4, 0}, {4, 3}, {4, 9}, {1, 0}, {1, 4}, {6, 1}, {6, 2}, {6, 3}, {0, 0},
                {0, 2}, {2, 0}, {3, 1}, {5, 1}, {5, 3}},
               300);
  // State 8: the tires (category 3 items 0 to 2), the door (category 5 item
  // 1) and the bonnet, each several times, then the pages without.
  choice_group(0x5265667265736808ull, "TuningScreen::Refresh (sub_821E3AD0) state 8", 8,
               {{3, 0}, {5, 1}, {4, 4}, {3, 1}, {5, 1}, {4, 4}, {3, 2}, {5, 1}, {4, 4},
                {4, 0}, {4, 3}, {5, 0}, {5, 2}, {6, 0}, {1, 3}, {0, 0}, {2, 0}, {6, 3}},
               300);

  for (size_t i = 0; i < globals.size(); i++) {
    Write(base, globals[i].first, saved[i].data(), globals[i].second);
  }
  for (const uint32_t a :
       {stand_allocation, text_allocation, screen, misc, strings, race, sub_scenes}) {
    memory::Free(a);
  }
}

}  // namespace harness
