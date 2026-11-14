// The type 8 color list screen's list part (Type8ColorListPart over
// ListPartBase), its inner part (Type8ColorPartInner over
// ListPartInnerBase) with a block a player (Type8ColorBlock: three menus)
// (docs/functions/type8-screens.md): see type8_screens.h. Read from the
// generated code; the calls made as in list_part.cpp (UiMovie::ClearNodeText
// a guest call too).

#include "type8_screens/type8_screens.h"

#include "base/log.h"
#include "guest.h"
#include "race_screens/race_screens.h"
#include "screen_constructors/screen_constructors.h"
#include "type67_screens/list_pages.h"
#include "type8_screens/part_helpers.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace type8_screens {

using guest::LoadU32;
using guest::StoreU32;

namespace ts = type67_screens;
namespace rs = race_screens;
using namespace part_detail;

namespace {

constexpr uint32_t kClearNodeText = 0x8211CC50;
// The float word PlaceCursorB copies into the nodes' +68 (0.0).
constexpr uint32_t kZero = 0x82000A34;

// The block of the player (0 → +40, 1 → +404, else 0) in the inner part.
uint32_t BlockOf(uint32_t inner, uint64_t player) {
  const uint32_t p = uint32_t(player);
  return p < 1 ? inner + 40 : (p == 1 ? inner + 404 : 0);
}

}  // namespace

// --- The color block ---

uint64_t ColorBlockConstruct(uint8_t* base, uint64_t r3, uint64_t player) {
  ITC_NATIVE_RUNNING("sub_82252EA0 Type8ColorBlock::Type8ColorBlock");
  const uint32_t b = uint32_t(r3), p = uint32_t(player);
  const uint32_t p4 = p << 2, p8 = p << 3, p12 = p * 12;
  constexpr uint32_t t = 0x82011388;
  for (const uint32_t at : {80u, 72u, 152u, 144u, 224u, 216u}) StoreU32(base, b + at, 0);
  StoreU32(base, b + 0, p);
  StoreU32(base, b + 8, 0);
  const uint32_t scene = LoadU32(base, t + 72 + p4);
  StoreU32(base, b + 240, 3);
  StoreU32(base, b + 232, 5);
  StoreU32(base, b + 236, 7);
  StoreU32(base, b + 244, 12);
  StoreU32(base, b + 228, scene);
  StoreU32(base, b + 248, 13);
  StoreU32(base, b + 252, 8);
  StoreU32(base, b + 256, 9);
  StoreU32(base, b + 280, LoadU32(base, t + 16 + p4));
  StoreU32(base, b + 284, LoadU32(base, t + 24 + p4));
  StoreU32(base, b + 288, LoadU32(base, t + 32 + p4));
  StoreU32(base, b + 292, LoadU32(base, t - 64 + p12));
  StoreU32(base, b + 296, LoadU32(base, t - 60 + p12));
  StoreU32(base, b + 300, LoadU32(base, t - 56 + p12));
  StoreU32(base, b + 304, LoadU32(base, t - 40 + p12));
  StoreU32(base, b + 308, LoadU32(base, t - 36 + p12));
  StoreU32(base, b + 312, LoadU32(base, t - 32 + p12));
  StoreU32(base, b + 316, LoadU32(base, t + 40 + p8));
  StoreU32(base, b + 320, LoadU32(base, t + 44 + p8));
  StoreU32(base, b + 324, LoadU32(base, t + 56 + p4));
  StoreU32(base, b + 260, 6);
  StoreU32(base, b + 264, 10);
  StoreU32(base, b + 268, 11);
  StoreU32(base, b + 272, 8);
  StoreU32(base, b + 328, LoadU32(base, t + 64 + p4));
  StoreU32(base, b + 276, 7);
  StoreU32(base, b + 332, LoadU32(base, t - 16 + p8));
  StoreU32(base, b + 336, LoadU32(base, t - 12 + p8));
  StoreU32(base, b + 344, LoadU32(base, t + p8));
  StoreU32(base, b + 356, 8);
  StoreU32(base, b + 360, 9);
  StoreU32(base, b + 348, LoadU32(base, t + 4 + p8));
  return r3;
}

uint64_t ColorBlockSetUp(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82253308 Type8ColorBlock::SetUp");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 176);
  // The pad players {0, 1} on the frame, by the player.
  StoreU32(base, frame + 96, 0);
  StoreU32(base, frame + 100, 1);
  const uint32_t pad = LoadU32(base, frame + 96 + (LoadU32(base, b) << 2));
  InitList(base, b + 12, {3, 3, 1, 154, 153, 155, 156, 159, 158}, frame, 0x82253374, calls);
  StoreU32(base, b + 72, pad);
  rs::ListSetSelection(base, b + 12, 0);
  InitList(base, b + 84, {2, 2, 1, 156, 155, 154, 153, 159, 158}, frame, 0x822533B4, calls);
  StoreU32(base, b + 144, pad);
  rs::ListSetSelection(base, b + 84, 0);
  InitList(base, b + 156, {5, 5, 1, 156, 155, 154, 153, 159, 158}, frame, 0x822533F4, calls);
  StoreU32(base, b + 216, pad);
  rs::ListSetSelection(base, b + 156, 2);
  const uint32_t movie = MovieAt(base, frame + 96, uint32_t(id));
  const uint32_t scene = LoadU32(base, b + 228);
  uint32_t node = ui_movies::FindNode(base, movie, scene, LoadU32(base, b + 232));
  calls.set_text(frame, 0x82253444, movie, LoadU32(base, b + 228), LoadU32(base, b + 260), 11);
  SetNodeText(movie, ui_movies::Child(base, node, LoadU32(base, b + 264)), 0, frame, 0x82253460,
              calls);
  SetNodeText(movie, ui_movies::Child(base, node, LoadU32(base, b + 268)), 1, frame, 0x8225347C,
              calls);
  node = ui_movies::FindNode(base, movie, LoadU32(base, b + 228), LoadU32(base, b + 236));
  calls.set_text(frame, 0x822534A4, movie, LoadU32(base, b + 228), LoadU32(base, b + 272), 12);
  const uint32_t count = rs::ListSelection(base, b + 156);
  const uint64_t result =
      SetNodeText(movie, ui_movies::Child(base, node, LoadU32(base, b + 276)), 8, frame,
                  0x822534D0, calls, uint64_t(count) + 1);
  StoreU32(base, movie + 40960, 1);
  StoreU32(base, b + 4, 0);
  StoreU32(base, b + 8, 0);
  return result;
}

uint64_t ColorBlockPlaceCursorA(uint8_t* base, uint64_t r3, uint64_t id, uint64_t shown,
                                uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82253728 Type8ColorBlock::PlaceCursorA");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  uint32_t node = ui_movies::Child(base, LoadU32(base, movie + 16), LoadU32(base, b + 228));
  node = ui_movies::Child(base, node, LoadU32(base, b + 236));
  node = ui_movies::Child(base, node, LoadU32(base, b + 240));
  StoreU32(base, node + 80, uint32_t(shown) == 0 ? 1 : 0);
  return node;
}

uint64_t ColorBlockPlaceCursorB(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82253690 Type8ColorBlock::PlaceCursorB");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 144);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  uint32_t node = ui_movies::Child(base, LoadU32(base, movie + 16), LoadU32(base, b + 228));
  node = ui_movies::Child(base, node, LoadU32(base, b + 232));
  const uint32_t zero = LoadU32(base, kZero);
  uint32_t last = 0;
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t a = ui_movies::Child(base, node, LoadU32(base, b + 244 + 4 * i));
    last = ui_movies::Child(base, node, LoadU32(base, b + 252 + 4 * i));
    StoreU32(base, a + 68, zero);
    StoreU32(base, last + 108, 1);
  }
  return last;
}

uint64_t ColorBlockPlaceCursorC(uint8_t* base, uint64_t r3, uint64_t id, uint64_t selection,
                                uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82253600 Type8ColorBlock::PlaceCursorC");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  const uint32_t node =
      ui_movies::FindNode(base, movie, LoadU32(base, b + 228), LoadU32(base, b + 232));
  const bool first = int32_t(uint32_t(selection)) == 0;
  const uint32_t off = LoadU32(base, b + (first ? 356 : 360));
  const uint32_t on = LoadU32(base, b + (first ? 360 : 356));
  StoreU32(base, ui_movies::Child(base, node, off) + 108, 0);
  const uint32_t child = ui_movies::Child(base, node, on);
  StoreU32(base, child + 108, 1);
  return child;
}

uint64_t ColorBlockPlayOpening(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822539A8 Type8ColorBlock::PlayOpening");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  ui_movies::PlayNamed(base, movie, LoadU32(base, b + 304), 1, 0, 0, 0);
  ui_movies::PlayNamed(base, movie, LoadU32(base, b + 308), 1, 0, 0, 0);
  ui_movies::PlayNamed(base, movie, LoadU32(base, b + 312), 1, 0, 0, 0);
  ColorBlockPlaceCursorA(base, r3, id, 1, frame);
  return ColorBlockPlaceCursorB(base, r3, id, frame);
}

uint64_t ColorBlockShowSelections(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82253A50 Type8ColorBlock::ShowSelections");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  const uint32_t selection = rs::ListSelection(base, b + 12);
  Restart(base, movie, b + ((selection + 70) << 2), frame, 0x82253AA0, 0x82253AC8, calls);
  Restart(base, movie, b + 332, frame, 0x82253AD4, 0x82253AFC, calls);
  Restart(base, movie, b + 344, frame, 0x82253B08, 0x82253B30, calls);
  Restart(base, movie, b + 336, frame, 0x82253B3C, 0x82253B64, calls);
  Restart(base, movie, b + 348, frame, 0x82253B70, 0x82253B98, calls);
  const uint32_t setting = rs::ListSelection(base, b + 84);
  ColorBlockPlaceCursorC(base, r3, id, setting, frame);
  return Restart(base, movie, b + ((setting + 79) << 2), frame, 0x82253BC8, 0x82253BF0, calls);
}

uint64_t ColorBlockShowMenuMove(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82253BF8 Type8ColorBlock::ShowMenuMove");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 144);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  const uint32_t selection = rs::ListSelection(base, b + 12);
  const uint32_t previous = LoadU32(base, b + 12 + rs::kListSelectionBefore);
  calls.call(kFinishNamed, frame, 0x82253C58, movie, LoadU32(base, b + ((previous + 70) << 2)));
  uint64_t result = Restart(base, movie, b + ((selection + 70) << 2), frame, 0x82253C6C,
                            0x82253C94, calls);
  if (selection == 1) {
    ColorBlockPlaceCursorA(base, r3, id, 0, frame);
    result = Restart(base, movie, b + 324, frame, 0x82253CB8, 0x82253CE0, calls);
  } else if (previous == 1) {
    calls.call(kFinishNamed, frame, 0x82253CF8, movie, LoadU32(base, b + 324));
    result = ColorBlockPlaceCursorA(base, r3, id, 1, frame);
  }
  if (previous == 0) {
    const uint32_t setting = rs::ListSelection(base, b + 84);
    result = calls.call(kFinishNamed, frame, 0x82253D30, movie,
                        LoadU32(base, b + ((setting + 79) << 2)));
  }
  if (selection == 2) {
    result = Restart(base, movie, b + 328, frame, 0x82253D44, 0x82253D6C, calls);
  } else if (previous == 2) {
    result = calls.call(kFinishNamed, frame, 0x82253D84, movie, LoadU32(base, b + 328));
  }
  return result;
}

uint64_t ColorBlockPlayReady(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82253570 Type8ColorBlock::PlayReady");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  calls.call(kFinishNamed, frame, 0x822535B0, movie, LoadU32(base, b + 328));
  calls.call(kFinishNamed, frame, 0x822535BC, movie, LoadU32(base, b + 312));
  calls.call(kFinishNamed, frame, 0x822535C8, movie, LoadU32(base, b + 300));
  return ui_movies::PlayNamed(base, movie, LoadU32(base, b + 300), 1, 0, 0, 0);
}

uint64_t ColorBlockSecondMoved(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822534F0 Type8ColorBlock::SecondMoved");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  const uint32_t node =
      ui_movies::FindNode(base, movie, LoadU32(base, b + 228), LoadU32(base, b + 236));
  const uint32_t child = ui_movies::Child(base, node, LoadU32(base, b + 276));
  calls.call(kClearNodeText, frame, 0x82253544, movie, child);
  const uint32_t count = rs::ListSelection(base, b + 156);
  return SetNodeText(movie, child, 8, frame, 0x82253564, calls, uint64_t(count) + 1);
}

uint64_t ColorBlockUpdateSecond(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82253948 Type8ColorBlock::UpdateSecond");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint64_t result = ts::pages::Call1(calls, rs::kListUpdate, r3 + 156, frame, 0x8225396C);
  if (LoadU32(base, b + 180) == 0) {
    return result;
  }
  ts::pages::PlaySound(base, 4, frame, calls);
  return ColorBlockSecondMoved(base, r3, id, frame, calls);
}

uint64_t ColorBlockThirdMoved(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82253D90 Type8ColorBlock::ThirdMoved");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 144);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  const uint32_t setting = rs::ListSelection(base, b + 84);
  const uint32_t previous = LoadU32(base, b + 84 + rs::kListSelectionBefore);
  calls.call(kFinishNamed, frame, 0x82253DF0, movie, LoadU32(base, b + ((previous + 79) << 2)));
  Restart(base, movie, b + ((setting + 79) << 2), frame, 0x82253E04, 0x82253E2C, calls);
  return ColorBlockPlaceCursorC(base, r3, id, setting, frame);
}

uint64_t ColorBlockUpdateThird(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82253F60 Type8ColorBlock::UpdateThird");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint64_t result = ts::pages::Call1(calls, rs::kListUpdate, r3 + 84, frame, 0x82253F84);
  if (LoadU32(base, b + 108) == 0) {
    return result;
  }
  ts::pages::PlaySound(base, 4, frame, calls);
  return ColorBlockThirdMoved(base, r3, id, frame, calls);
}

uint64_t ColorBlockWhileReady(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82253EF8 Type8ColorBlock::WhileReady");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 112);
  if (int32_t(uint32_t(ts::pages::Call1(calls, rs::kListCancelled, r3 + 12, frame,
                                        0x82253F1C))) != 0) {
    ts::pages::PlaySound(base, 2, frame, calls);
    StoreU32(base, b + 4, 0);
    ColorBlockShowMenuMove(base, r3, id, frame, calls);
  }
  return 1;
}

uint64_t ColorBlockInput(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254070 Type8ColorBlock::Input");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint64_t list = r3 + 12;
  ts::pages::Call1(calls, rs::kListUpdate, list, frame, 0x82254090);
  const uint32_t selection = rs::ListSelection(base, uint32_t(list));
  if (int32_t(uint32_t(ts::pages::Call1(calls, rs::kListAccepted, list, frame, 0x822540A4))) !=
      0) {
    ts::pages::PlaySound(base, 0, frame, calls);
    if (selection < 2) {
      rs::ListSetSelection(base, uint32_t(list), 2);
      ColorBlockShowMenuMove(base, r3, id, frame, calls);
    } else if (selection == 2) {
      StoreU32(base, b + 4, 1);
      ColorBlockPlayReady(base, r3, id, frame, calls);
    }
    return 0;
  }
  if (int32_t(uint32_t(ts::pages::Call1(calls, rs::kListCancelled, list, frame, 0x822540F0))) !=
      0) {
    ts::pages::PlaySound(base, LoadU32(base, b) != 0 ? 3 : 2, frame, calls);
    StoreU32(base, b + 8, 1);
    return 0;
  }
  if (LoadU32(base, b + 36) != 0) {
    ts::pages::PlaySound(base, 4, frame, calls);
    ColorBlockShowMenuMove(base, r3, id, frame, calls);
  } else if (selection == 0) {
    ColorBlockUpdateThird(base, r3, id, frame, calls);
  } else if (selection == 1) {
    ColorBlockUpdateSecond(base, r3, id, frame, calls);
  }
  return 0;
}

// --- The color list part and its inner part ---

uint64_t ColorInnerConstruct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82253030 Type8ColorPartInner::Type8ColorPartInner");
  const uint32_t inner = uint32_t(r3);
  Frame(base, sp, 96);
  screen_constructors::ListPartInnerBase(base, inner);
  StoreU32(base, inner, kColorInnerVtable);
  ColorBlockConstruct(base, r3 + 40, 0);
  ColorBlockConstruct(base, r3 + 404, 1);
  return r3;
}

uint64_t ColorPartConstruct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82253190 Type8ColorListPart::Type8ColorListPart");
  const uint32_t part = uint32_t(r3), frame = Frame(base, sp, 112);
  screen_constructors::ListPartBase(base, part);
  StoreU32(base, part, kColorPartVtable);
  ColorInnerConstruct(base, r3 + 20, frame);
  StoreU32(base, part + 16, part + 20);
  StoreU32(base, part + 12, 20);
  return r3;
}

uint64_t ColorInnerDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82253088 Type8ColorPartInner::~Type8ColorPartInner");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, uint32_t(r3), kColorInnerVtable);
  ts::ReleaseList(base, r3 + 404 + 156, frame, 0x822530B8, calls);
  ts::ReleaseList(base, r3 + 404 + 84, frame, 0x822530C0, calls);
  ts::ReleaseList(base, r3 + 404 + 12, frame, 0x822530C8, calls);
  ts::ReleaseList(base, r3 + 40 + 156, frame, 0x822530D4, calls);
  ts::ReleaseList(base, r3 + 40 + 84, frame, 0x822530DC, calls);
  ts::ReleaseList(base, r3 + 40 + 12, frame, 0x822530E4, calls);
  return ts::ListPartInnerBaseDestruct(base, r3);
}

uint64_t ColorInnerDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82253268 Type8ColorPartInner::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  ColorInnerDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    Delete(r3, flags, frame, 0x82253298, calls);
  }
  return r3;
}

uint64_t ColorPartDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822531F0 Type8ColorListPart::~Type8ColorListPart");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, uint32_t(r3), kColorPartVtable);
  ColorInnerDestruct(base, r3 + 20, frame, calls);
  return ts::ListPartBaseDestruct(base, r3);
}

uint64_t ColorPartDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822532B8 Type8ColorListPart::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  ColorPartDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    Delete(r3, flags, frame, 0x822532E8, calls);
  }
  return r3;
}

uint64_t ColorInnerIsFirstChoice(uint8_t* base, uint64_t r3, uint64_t player, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82253148 Type8ColorPartInner::IsFirstChoice");
  Frame(base, sp, 96);
  return rs::ListSelection(base, BlockOf(uint32_t(r3), player) + 84) == 0 ? 1 : 0;
}

uint64_t ColorPartIsFirstChoice(uint8_t* base, uint64_t r3, uint64_t player, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82253238 Type8ColorListPart::IsFirstChoice");
  return ColorInnerIsFirstChoice(base, r3 + 20, player, sp);
}

uint64_t ColorPartThirdChoice(uint8_t* base, uint64_t r3, uint64_t player) {
  ITC_NATIVE_RUNNING("sub_82253240 Type8ColorListPart::ThirdChoice");
  return rs::ListSelection(base, BlockOf(uint32_t(r3) + 20, player) + 156);
}

uint64_t ColorInnerOpen(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822537A0 Type8ColorPartInner::Open");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 128);
  OpenBegin(base, inner, uint32_t(r4), frame, 0x82253810, calls);
  ColorBlockSetUp(base, r3 + 40, r4, frame, calls);
  const uint64_t result = ColorBlockSetUp(base, r3 + 404, r4, frame, calls);
  StoreU32(base, inner + 32, 0);
  StoreU32(base, inner + 36, 0);
  return result;
}

uint64_t ColorInnerPick(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t sp,
                        uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254280 Type8ColorPartInner::Pick");
  return InnerPick(base, r3, r4, r5, kColorInnerPages, sp, lr, calls);
}

uint64_t ColorInnerInput(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82253FC0 Type8ColorPartInner::Input");
  return InnerInput(base, r3, r4, kColorInnerSteps, 0x8225401C, 0x82254064, sp, calls);
}

uint64_t ColorInnerStep0(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82253E48 Type8ColorPartInner::Step0");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(r4));
  if (LoadU32(base, inner + 28) == 0) {
    ColorBlockPlayOpening(base, r3 + 40, r4, frame);
    ColorBlockPlayOpening(base, r3 + 404, r4, frame);
    ui_movies::PlayNamed(base, movie, 46, 0, 0, 0, 0);
    StoreU32(base, inner + 28, 1);
  }
  const uint64_t done = uint32_t(calls.call(ts::kRun, frame, 0x82253EC8, movie, 0)) == 0 ? 1 : 0;
  if (done != 0) {
    ColorBlockShowSelections(base, r3 + 40, r4, frame, calls);
    ColorBlockShowSelections(base, r3 + 404, r4, frame, calls);
  }
  return done;
}

uint64_t ColorInnerPage0(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254178 Type8ColorPartInner::Page0");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 128);
  auto block = [&](uint64_t at) -> uint64_t {
    const uint32_t state = LoadU32(base, uint32_t(at) + 4);
    if (state == 0) return ColorBlockInput(base, at, r4, frame, calls);
    if (state == 1) return ColorBlockWhileReady(base, at, r4, frame, calls);
    return 1;
  };
  const uint64_t first = block(r3 + 40);
  const uint64_t second = block(r3 + 404);
  Page0End(base, inner, first, second, inner + 48, calls);
  return 0;
}

}  // namespace type8_screens
