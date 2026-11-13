// The type 6 and 7 list screens' shared helpers, item loaders and handlers
// (ListScreen, NodeListScreen, ColorListScreen; docs/functions/type67-screens.md):
// see type67_screens.h. UiMovies::Find, UiMovie::FindNode, ShowNode,
// HideNode, SetNodeTexture and MenuList::Selection through their plain C++
// (GetPreviousSelection, a two-instruction leaf, inline); UiMovie::
// FinishNamed, PlayNamed, LoopNamed, ResetPose, Run, ClearText, ScrollText
// and sub_8211C5F8 guest calls to their overrides; SetText through
// Calls::set_text; the screens' slots through Slot.

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "guest.h"
#include "race_screens/race_screens.h"
#include "ui_movies/game.h"

namespace type67_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kFinishNamed = 0x8211DF20, kPlayNamed = 0x8211DAC8, kLoopNamed = 0x8211DDE0;
constexpr uint32_t kClearText = 0x8211C470, kScrollText = 0x8211C570,
                   kResetTextScroll = 0x8211C5F8;
constexpr uint32_t kNone = 0xFFFFFFFFu;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The screen's movie: [+9680] at the frame's +80, then UiMovies::Find.
// `found` the address Find answers (the r3 it leaves).
uint32_t MovieOf(uint8_t* base, uint32_t screen, uint32_t frame, uint32_t* found) {
  StoreU32(base, frame + 80, LoadU32(base, screen + 9680));
  *found = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  return LoadU32(base, *found);
}

// A node's +108 set (FindNode, then the store); the node.
uint32_t SetNodeWord(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
                     uint32_t value) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  StoreU32(base, node + 108, value);
  return node;
}

// UiMovie::ShowNode (`shown` 1) and HideNode (0): the node's +80; the node.
uint32_t SetShown(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
                  uint32_t shown) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  StoreU32(base, node + 80, shown);
  return node;
}

// FinishNamed, then PlayNamed (chained 1), from `frame` returning to
// `lr[0]` and `lr[1]`. PlayNamed's r3.
uint64_t Restart(const Calls& calls, uint32_t frame, uint32_t lr0, uint32_t lr1, uint32_t movie,
                 uint32_t named) {
  calls.call(kFinishNamed, frame, lr0, movie, named);
  const uint64_t args[6] = {movie, named, 1, 0, 0, 0};
  return calls.call_set(kPlayNamed, args, 6, frame, lr1);
}

}  // namespace

// --- The destructors and the item loaders ---

uint64_t ListScreenDestruct(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82251EB8 ListScreen::~ListScreen");
  StoreU32(base, uint32_t(r3), kListScreenVtable);
  return r3;
}

uint64_t ListScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252248 ListScreen::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, uint32_t(r3), kListScreenVtable);
  if ((flags & 1) != 0) {
    const uint64_t args[6] = {r3, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x82252274);
  }
  return r3;
}

uint64_t NodeListScreenItemInfo(uint8_t* base, uint64_t r3, uint32_t info) {
  ITC_NATIVE_RUNNING("sub_82256FE8 NodeListScreen::ItemInfo");
  StoreU32(base, info + 0, kNone);
  StoreU32(base, info + 4, kNone);
  StoreU32(base, info + 8, kNone);
  return r3;
}

uint64_t NodeListScreenLoadItem(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257060 NodeListScreen::LoadItem");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint32_t info = frame + 80;
  uint64_t result = Slot5(base, r3, 100, r4, info, frame, 0x82257088, calls);
  if (LoadU32(base, screen + 9672) != 0) {
    result = NodeTableSetPairs(base, r3 + 10360, info);
  }
  for (uint32_t i = 0; i < 2; i++) {
    // The original's (1 << i) != 0, ANDed with the info's +12 word.
    const uint32_t bit = (i & 0x20) != 0 ? 0 : 1u << (i & 0x1F);
    StoreU32(base, screen + 9700 + 4 * i, (bit != 0 ? 1u : 0u) & LoadU32(base, info + 12));
  }
  return result;
}

uint64_t ColorListScreenLoadItem(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5,
                                 uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82255238 ColorListScreen::LoadItem");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 144);
  const uint32_t info = frame + 80;
  Slot5(base, r3, 100, r4, info, frame, 0x82255268, calls);
  const uint64_t result = ColorNodeTableSet(base, r3 + 10432, info);
  const int32_t index = int32_t(uint32_t(r5));
  uint32_t value = kNone;
  if (index >= 0 && index < int32_t(LoadU32(base, info + 8))) {
    value = LoadU32(base, (uint32_t(r5) << 2) + (frame + 100));
  }
  StoreU32(base, screen + 10908, value);
  return result;
}

// --- The helpers ---

uint64_t ListScreenShowSelection(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822518B0 ListScreen::ShowSelection");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 144);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, screen, frame, &found);
  const uint32_t list = uint32_t(r4);
  const uint32_t selection = race_screens::ListSelection(base, list);
  // MenuList::GetPreviousSelection (sub_8220BC28): +20.
  const uint32_t previous = LoadU32(base, list + 20);
  uint64_t result = previous;
  const uint32_t named = LoadU32(base, screen + 9900);
  if (int32_t(named) >= 0) {
    result = Restart(calls, frame, 0x82251914, 0x82251930, movie, named);
  }
  if (int32_t(LoadU32(base, screen + 9724)) >= 0) {
    if (int32_t(previous) >= 0) {
      const uint32_t child = LoadU32(base, screen + 9764);
      if (int32_t(child) >= 0) {
        result = SetNodeWord(base, movie, LoadU32(base, screen + 9724), child, previous);
      }
    }
    const uint32_t child = LoadU32(base, screen + 9768);
    if (int32_t(child) >= 0) {
      result = SetNodeWord(base, movie, LoadU32(base, screen + 9724), child, selection);
    }
    for (int32_t i = 0; i < int32_t(LoadU32(base, screen + 9784)); i++) {
      const uint32_t first = LoadU32(base, screen + 9772);
      if (first != 0 && int32_t(previous) >= 0) {
        const uint32_t node = LoadU32(base, uint32_t(4 * i) + first);
        if (int32_t(node) >= 0) {
          result = SetNodeWord(base, movie, LoadU32(base, screen + 9724), node, previous);
        }
      }
      const uint32_t second = LoadU32(base, screen + 9776);
      if (second != 0) {
        const uint32_t node = LoadU32(base, uint32_t(4 * i) + second);
        if (int32_t(node) >= 0) {
          result = SetNodeWord(base, movie, LoadU32(base, screen + 9724), node, selection);
        }
      }
    }
  }
  const uint32_t last = LoadU32(base, screen + 9760);
  if (int32_t(last) >= 0) {
    result = SetNodeWord(base, movie, kNone, last, selection);
  }
  return result;
}

uint64_t ListScreenLoopNode(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t table, uint32_t sp,
                            const Calls& calls) {
  // The three functions' return addresses (FinishNamed, PlayNamed, LoopNamed).
  uint32_t lr[3];
  switch (table) {
    case 9936:
      ITC_NATIVE_RUNNING("sub_82251A20 ListScreen::LoopItemNode");
      lr[0] = 0x82251A8C, lr[1] = 0x82251AA8, lr[2] = 0x82251AB4;
      break;
    case 9976:
      ITC_NATIVE_RUNNING("sub_82251B80 ListScreen::LoopSecondNode");
      lr[0] = 0x82251BEC, lr[1] = 0x82251C08, lr[2] = 0x82251C14;
      break;
    default:
      ITC_NATIVE_RUNNING("sub_82251AD0 ListScreen::LoopThirdNode");
      lr[0] = 0x82251B3C, lr[1] = 0x82251B58, lr[2] = 0x82251B64;
      break;
  }
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, screen, frame, &found);
  const uint32_t selection = race_screens::ListSelection(base, uint32_t(r4));
  if (int32_t(selection) < 0) return selection;
  const uint32_t named = LoadU32(base, ((selection + table / 4) << 2) + screen);
  if (int32_t(named) < 0) return selection;
  Restart(calls, frame, lr[0], lr[1], movie, named);
  return calls.call(kLoopNamed, frame, lr[2], movie, named);
}

uint64_t ListScreenSetScrollText(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5,
                                 uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251D00 ListScreen::SetScrollText");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t id = uint32_t(r4);
  if (id == LoadU32(base, screen + 10140)) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, screen, frame, &found);
  uint64_t result = found;
  const uint32_t sub_scene = LoadU32(base, screen + 9740);
  if (int32_t(sub_scene) >= 0) {
    const uint32_t child = LoadU32(base, screen + 9816);
    if (int32_t(child) >= 0 && int32_t(id) >= 0) {
      const uint64_t reset[6] = {movie, sub_scene, child, 0, 0, 0};
      calls.call_set(kResetTextScroll, reset, 3, frame, 0x82251D68);
      const uint32_t text_sub = LoadU32(base, screen + 9740);
      calls.set_r4(text_sub);
      calls.set_text(frame, 0x82251D7C, movie, text_sub, LoadU32(base, screen + 9816), id);
      const uint64_t scroll[6] = {movie, LoadU32(base, screen + 9740),
                                  LoadU32(base, screen + 9816), uint32_t(r5), 0, 0};
      result = calls.call_set(kScrollText, scroll, 4, frame, 0x82251D90);
    }
  }
  StoreU32(base, screen + 10140, id);
  return result;
}

uint64_t ListScreenClearScrollText(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251DA0 ListScreen::ClearScrollText");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, screen, frame, &found);
  uint64_t result = found;
  const uint32_t sub_scene = LoadU32(base, screen + 9740);
  if (int32_t(sub_scene) >= 0) {
    const uint32_t child = LoadU32(base, screen + 9816);
    if (int32_t(child) >= 0) {
      const uint64_t args[6] = {movie, sub_scene, child, 0, 0, 0};
      result = calls.call_set(kClearText, args, 3, frame, 0x82251DEC);
    }
  }
  StoreU32(base, screen + 10136, 0);
  StoreU32(base, screen + 10140, kNone);
  return result;
}

uint64_t ListScreenSetArrows(uint8_t* base, uint64_t r3, uint32_t which, uint32_t on,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82251E10 ListScreen::SetArrows");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t child = LoadU32(base, screen + (int32_t(which) == 0 ? 9752 : 9756));
  if (int32_t(child) < 0) return r3;
  const uint32_t sub_scene = LoadU32(base, screen + 9728);
  if (int32_t(sub_scene) < 0) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, screen, frame, &found);
  const uint32_t field = int32_t(on) != 0 ? 9800 : 9804;
  if (int32_t(LoadU32(base, screen + field)) < 0) return movie;
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  StoreU32(base, node + 108, LoadU32(base, screen + field));
  return node;
}

uint64_t ListScreenOnToggleMoved(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252068 ListScreen::OnToggleMoved");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, screen, frame, &found);
  const uint32_t list = uint32_t(r4);
  const uint32_t selection = race_screens::ListSelection(base, list);
  // MenuList::GetPreviousSelection (sub_8220BC28), read and unused.
  (void)LoadU32(base, list + 20);
  const bool first = int32_t(selection) == 0;
  // The node finished, then the one played and looped.
  const uint32_t off = first ? 9896 : 9892, on = first ? 9892 : 9896;
  const uint32_t lr_finish_off = first ? 0x822520EC : 0x82252150;
  const uint32_t lr_finish_on = first ? 0x82252100 : 0x82252164;
  const uint32_t lr_play = first ? 0x8225211C : 0x82252180;
  ListScreenSetArrows(base, r3, 0, first ? 1 : 0, frame);
  uint64_t result = ListScreenSetArrows(base, r3, 1, first ? 0 : 1, frame);
  const uint32_t named_off = LoadU32(base, screen + off);
  if (int32_t(named_off) >= 0) {
    result = calls.call(kFinishNamed, frame, lr_finish_off, movie, named_off);
  }
  const uint32_t named_on = LoadU32(base, screen + on);
  if (int32_t(named_on) < 0) return result;
  calls.call(kFinishNamed, frame, lr_finish_on, movie, named_on);
  const uint64_t args[6] = {movie, LoadU32(base, screen + on), 1, 0, 0, 0};
  calls.call_set(kPlayNamed, args, 6, frame, lr_play);
  return calls.call(kLoopNamed, frame, 0x8225218C, movie, LoadU32(base, screen + on));
}

uint64_t ListScreenShowToggle(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252198 ListScreen::ShowToggle");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, screen, frame, &found);
  const uint32_t list = uint32_t(r4);
  ListScreenSetArrows(base, r3, race_screens::ListSelection(base, list), 1, frame);
  const uint32_t selection = race_screens::ListSelection(base, list);
  const uint32_t named = LoadU32(base, screen + (int32_t(selection) == 0 ? 9892 : 9896));
  if (int32_t(named) < 0) return selection;
  Restart(calls, frame, 0x82252214, 0x82252230, movie, named);
  return calls.call(kLoopNamed, frame, 0x8225223C, movie, named);
}

uint64_t ListScreenSetIcons(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82251C30 ListScreen::SetIcons");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, screen, frame, &found);
  // UiMovie::SetNodeTexture (sub_8211A708): the lookup, then the texture.
  static constexpr uint32_t kTextures[4] = {0, 0, 1, 1};
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t node = ui_movies::FindNode(base, movie, LoadU32(base, screen + 9732),
                                              LoadU32(base, screen + 9812) + k);
    ui_movies::SetNodeTexture(base, node, kTextures[k]);
  }
  SetShown(base, movie, LoadU32(base, screen + 9732), LoadU32(base, screen + 9812) + 4, 0);
  return SetShown(base, movie, LoadU32(base, screen + 9732), LoadU32(base, screen + 9812) + 5, 0);
}

uint64_t ListScreenSetUpMovie(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251EC8 ListScreen::SetUpMovie");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, screen, frame, &found);
  // r4 as Find leaves it (the id's word).
  calls.call(kResetPose, frame, 0x82251F04, movie, frame + 80);
  const uint32_t table = LoadU32(base, screen + 9672);
  if (table != 0) {
    // Slot 1 with r4 as ResetPose left it.
    Slot(base, table, 4, std::nullopt, frame, 0x82251F24, calls);
  }
  const uint32_t sub_scene = LoadU32(base, screen + 9724);
  if (int32_t(sub_scene) >= 0) {
    const uint32_t child = LoadU32(base, screen + 9748);
    if (int32_t(child) >= 0) {
      SetShown(base, movie, sub_scene, child, LoadU32(base, screen + 9788) != 0 ? 1 : 0);
    }
  }
  ListScreenSetArrows(base, r3, 0, 0, frame);
  ListScreenSetArrows(base, r3, 1, 0, frame);
  return ListScreenSetIcons(base, r3, frame);
}

// --- The handlers ---

uint64_t ListScreenShow(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251F98 ListScreen::Show");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  ListScreenSetUpMovie(base, r3, frame, calls);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, screen, frame, &found);
  // r4 the sub-scene as the original loads it (slot 22 is called with the
  // r4 the last of these leaves).
  const uint32_t sub_scene = LoadU32(base, screen + 9728);
  calls.set_r4(sub_scene);
  if (int32_t(sub_scene) >= 0) {
    const uint32_t title = LoadU32(base, screen + 9712);
    if (int32_t(title) >= 0) {
      calls.set_text(frame, 0x82251FF0, movie, sub_scene, title, 322);
    }
    const uint32_t first = LoadU32(base, screen + 9716);
    if (first != 0) {
      const uint32_t at = LoadU32(base, screen + 9728);
      calls.set_r4(at);
      calls.set_text(frame, 0x8225200C, movie, at, first, 0);
    }
    const uint32_t second = LoadU32(base, screen + 9720);
    if (second != 0) {
      const uint32_t at = LoadU32(base, screen + 9728);
      calls.set_r4(at);
      calls.set_text(frame, 0x82252028, movie, at, second, 1);
    }
  }
  StoreU32(base, screen + 9696, 0);
  Slot(base, r3, 88, std::nullopt, frame, 0x82252044, calls);
  return Slot(base, r3, 56, r4, frame, 0x8225205C, calls);
}

uint64_t ListScreenDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251520 ListScreen::Draw");
  const uint32_t frame = Frame(base, sp, 112);
  Slot(base, r3, 92, r4, frame, 0x8225154C, calls);
  return Slot(base, r3, 40, r4, frame, 0x82251564, calls);
}

uint64_t ListScreenUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822517B8 ListScreen::Update");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  if (int32_t(LoadU32(base, screen + 9680)) < 0) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, screen, frame, &found);
  if (LoadU32(base, screen + 9656) != kNone) {
    // r4 as Find leaves it (the id's word).
    const uint64_t done = Slot(base, r3, 96, frame + 80, frame, 0x8225181C, calls);
    if (int32_t(uint32_t(done)) != 0) {
      // The page block (+9644): the queued step (+16) or the page after.
      const uint32_t pages = screen + 9644;
      const uint32_t queued = LoadU32(base, pages + 16);
      if (queued == kNone) {
        const uint32_t after = LoadU32(base, pages + 4);
        StoreU32(base, pages + 12, kNone);
        StoreU32(base, pages + 0, after);
      } else {
        StoreU32(base, pages + 20, 0);
        StoreU32(base, pages + 12, queued);
        StoreU32(base, pages + 16, kNone);
      }
      StoreU32(base, pages + 24, 0);
    }
    return done;
  }
  uint64_t result = calls.call(kRun, frame, 0x82251864, movie, 0);
  if (LoadU32(base, screen + 9696) != 0) {
    const uint32_t part = LoadU32(base, screen + 9676);
    if (part != 0) {
      result = Slot(base, part, 12, r4, frame, 0x82251894, calls);
    }
  }
  return result;
}

uint64_t ListScreenTail(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t slot, uint32_t sp,
                        uint32_t lr, const Calls& calls) {
  switch (slot) {
    case 64:
      ITC_NATIVE_RUNNING("sub_82251580 ListScreen::Suspend");
      break;
    case 68:
      ITC_NATIVE_RUNNING("sub_82251590 ListScreen::Resume");
      break;
    default:
      ITC_NATIVE_RUNNING("sub_822517A0 ListScreen::Open");
      StoreU32(base, uint32_t(r3) + 9608, 4);
      break;
  }
  return Slot(base, r3, slot, r4, sp, lr, calls);
}

}  // namespace type67_screens
