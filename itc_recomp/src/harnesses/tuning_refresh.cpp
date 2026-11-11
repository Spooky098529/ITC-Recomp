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
// items in their play ranges mostly; states 5 to 8 go to the original on
// both sides for now (the register step's gate; parts 2 to 4 of the
// migration shape their cases). Compared: the text records SetText made,
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
#include <string>
#include <utility>
#include <vector>

namespace harness {

// The stand's world (car_makers.cpp): TuningStandWorldBytes() in a block,
// LayTuningStandWorld returning the save data, the globals pointed at it.
uint32_t TuningStandWorldBytes();
uint32_t LayTuningStandWorld(uint8_t* base, uint32_t block, Random& r, int c);
void PointTuningStandWorld(uint8_t* base, uint32_t block);
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

}  // namespace

void TuningRefresh(Context& h) {
  if (!hooks::TuningStandCalleesNative() || !hooks::TuningCarStatsCalleesNative()) {
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
  if (stand_allocation == 0 || text_allocation == 0 || screen == 0 || misc == 0 ||
      strings == 0 || race == 0) {
    ITC_LOG_ERROR("harness: tuning_screen: no memory for TuningScreen::Refresh's world");
    for (const uint32_t a : {stand_allocation, text_allocation, screen, misc, strings, race}) {
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

  Random r(0x5265667265736800ull);  // "Refresh"
  Results results("TuningScreen::Refresh (sub_821E3AD0)");
  constexpr int kCases = 200;
  for (int c = 0; c < kCases; c++) {
    // The state: each of 3, 4 and 9 to 58 in turn for the first 104 cases
    // (two rounds), then 3 and 4 mostly, the dispatch's states, and outside 3
    // to 58 (below, above, negative). States 5 to 8 aren't among them yet:
    // parts 2 to 4 of the migration add them with the worlds their pages
    // need (in this one the original's state 5 page writes through a node
    // at 1).
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
    const uint32_t race_count = r.Below(6), race_added = r.Below(3);

    std::vector<Region> regions = {{block, stand_bytes, {}},
                                   {screen, kScreenBytes, {}},
                                   {text, kRefreshTextBytes, {}},
                                   {misc, kMiscBytes, {}}};
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
  }
  results.Report();

  for (size_t i = 0; i < globals.size(); i++) {
    Write(base, globals[i].first, saved[i].data(), globals[i].second);
  }
  for (const uint32_t a : {stand_allocation, text_allocation, screen, misc, strings, race}) {
    memory::Free(a);
  }
}

}  // namespace harness
