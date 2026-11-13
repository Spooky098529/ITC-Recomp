// The tuning screen's update's sticker commands, 32 to 41 and 43 to 51
// (docs/functions/tuning-screen.md, Command32 to Command51; their helpers in
// sticker_pages.cpp, docs/functions/sticker-editor.md): see tuning_pages.h.
// None was seen in play; ported to leave as little recompiled as possible
// (the user's call, 2026-11-12), with the originals' loads and stores in
// their order and their quirks kept. The UI movie calls (FinishNamed,
// PlayNamed, LoopNamed, UnloopNamed, SetText, SetNodeText,
// SetChildNameText, ScrollText and sub_8211C5F8) through their overrides
// from each original's frame with its return addresses; the nodes
// (FindNode, Child, ShowNode's, HideNode's and SetNodeTexture's stores)
// through the UI movies' plain C++; the sticker editor's helpers by their
// plain C++; the unset varargs of SetText and SetNodeText passed as 0 (the
// user's call, 2026-10-30).

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <cstdint>

namespace tuning_pages {

namespace {

namespace steps = camera_modes::steps;
using tuning_screen::GuestCall;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

constexpr uint32_t kAll = 0xFFFFFFFFu;  // sub-scene -1: the scene itself
constexpr uint32_t kUnloopNamed = 0x8211DE30;      // UiMovie::UnloopNamed(movie, named)
constexpr uint32_t kSetNodeText = 0x8211C7E8;      // UiMovie::SetNodeText(movie, node, id, ...)
constexpr uint32_t kSetChildNameText = 0x8211D930;  // (movie, sub, child, name record)
constexpr uint32_t kListPlaces = 0x8200C520;  // Command34's 5 (x, y) floats for sub-scene 30
constexpr uint32_t kListShift = 0x820067B0;   // the float Command34 takes off the slot's x
constexpr uint32_t kEditPlaces = 0x8200C328;  // the edit pages' 7 words pairs an area, then 7 more
constexpr uint32_t kFreeLookX = 0x82008650;   // Command51's float for (-1, 1)'s child 3 +12

// The screen's fields.
constexpr uint32_t kArea = 30724;      // a
constexpr uint32_t kSlot = 30732;      // l
constexpr uint32_t kOperation = 30740;
constexpr uint32_t kEditRow = 30748;
constexpr uint32_t kOrder = 30756;     // the order page's position p
constexpr uint32_t kPick = 30764;
constexpr uint32_t kDecal = 30832;     // D
constexpr uint32_t kEnding = 9720;     // the command's end asked
constexpr uint32_t kMenu = 10044;      // m

uint32_t Menu(uint8_t* base, uint32_t s) { return Load(base, s + kMenu); }

// The queue's previous state (q+8).
int32_t Previous(uint8_t* base, uint32_t s) {
  return int32_t(Load(base, Load(base, s + 10036) + 8));
}

// id(j): s + 33320 + 20a + 4j.
uint32_t PlacedId(uint8_t* base, uint32_t s, uint32_t j) {
  return Load(base, (((Load(base, s + kArea) + 1666) * 5 + j) << 2) + s);
}

// rec(j): (5a + j) × 24 + s + 32360.
uint32_t Record(uint8_t* base, uint32_t s, uint32_t j) {
  return (Load(base, s + kArea) * 5 + j) * 24 + s + 32360;
}

// The order list's word i (+33480 + 4i).
uint32_t OrderWord(uint8_t* base, uint32_t s, uint32_t i) {
  return Load(base, ((i + 8370) << 2) + s);
}

// The float of an int (its doubleword at `at`, fcfid, frsp): its bits.
uint32_t AsFloat(uint8_t* base, uint32_t at, int64_t x) {
  Store(base, at, uint32_t(uint64_t(x) >> 32));
  Store(base, at + 4, uint32_t(x));
  return steps::Bits(steps::Single(double(x)));
}

// mulli by 40 then extsw.
int64_t Times40(uint32_t x) { return int32_t(x * 40u); }

// UiMovie::ShowNode and HideNode (sub_8211AC00, sub_8211AC48): the node's
// +80; the node.
uint32_t SetShown(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
                  uint32_t shown) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  Store(base, node + 80, shown);
  return node;
}

// The sticker menu m reset for a page: +12 0, +16 `last`, +20 `wraps`, the
// rest of its words 0; m+0 the field at `cursor`, m+4 the old m+0.
void ResetMenu(uint8_t* base, uint32_t s, uint32_t last, uint32_t wraps, uint32_t cursor) {
  const uint32_t m = Menu(base, s);
  Store(base, m + 12, 0);
  Store(base, m + 16, last);
  Store(base, m + 20, wraps);
  for (const uint32_t at : {0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    Store(base, m + at, 0);
  }
  const uint32_t menu = Menu(base, s);
  const uint32_t value = Load(base, s + cursor);
  const uint32_t old = Load(base, menu + 0);
  Store(base, menu + 0, value);
  Store(base, menu + 4, old);
}

// The calls on movie 14 from `frame`.
struct Movie {
  uint8_t* base;
  const GuestCall& call;
  uint32_t movie;
  uint32_t frame;
  uint64_t Finish(uint64_t named, uint32_t lr) {
    return call(tuning_screen::kFinishNamed, {movie, named}, frame, lr);
  }
  uint64_t Unloop(uint64_t named, uint32_t lr) {
    return call(kUnloopNamed, {movie, named}, frame, lr);
  }
  uint64_t Play(uint64_t named, uint64_t chained, uint32_t lr) {
    return call(tuning_screen::kPlayNamed, {movie, named, chained, 0, 0, 0}, frame, lr);
  }
  uint64_t Loop(uint64_t named, uint32_t lr) {
    return call(tuning_screen::kLoopNamed, {movie, named}, frame, lr);
  }
  // FinishNamed, then PlayNamed chained.
  uint64_t Restart(uint64_t named, uint32_t finish_lr, uint32_t play_lr) {
    Finish(named, finish_lr);
    return Play(named, 1, play_lr);
  }
  // FinishNamed, PlayNamed chained, LoopNamed.
  uint64_t Looped(uint64_t named, const uint32_t (&lr)[3]) {
    Restart(named, lr[0], lr[1]);
    return Loop(named, lr[2]);
  }
  uint64_t Text(uint64_t sub, uint64_t child, uint64_t id, uint32_t lr) {
    return call(tuning_screen::kSetText, {movie, sub, child, id, 0, 0, 0, 0}, frame, lr);
  }
  uint64_t NodeText(uint32_t node, uint64_t id, uint32_t lr) {
    return call(kSetNodeText, {movie, node, id, 0, 0, 0, 0, 0}, frame, lr);
  }
  // The help line (2, 0): its scroll reset, string `id`, scrolled.
  uint64_t Help(uint64_t id, const uint32_t (&lr)[3]) {
    call(tuning_screen::kResetTextScroll, {movie, 2, 0}, frame, lr[0]);
    Text(2, 0, id, lr[1]);
    return call(tuning_screen::kScrollText, {movie, 2, 0, 1}, frame, lr[2]);
  }
  uint32_t Shown(uint32_t sub, uint32_t child, uint32_t shown) {
    return SetShown(base, movie, sub, child, shown);
  }
  // UiMovie::SetNodeTexture (sub_8211A708).
  uint32_t Texture(uint32_t sub, uint32_t child, uint32_t texture) {
    return ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, sub, child), texture);
  }
  // Node (sub, child)'s child 0's +16: the float of `x` × 40 (through the
  // frame's +80).
  void Mark(uint32_t sub, uint32_t child, uint32_t x) {
    const uint32_t node = ui_movies::FindNode(base, movie, sub, child);
    const uint32_t bits = AsFloat(base, frame + 80, Times40(x));
    Store(base, ui_movies::Child(base, node, 0) + 16, bits);
  }
  // Sub-scene 30's nodes 12, 17, ..., 32 hidden.
  void HideList() {
    for (uint32_t n = 12; n < 37; n += 5) {
      Shown(30, n, 0);
    }
  }
};

Movie Open(uint8_t* base, const GuestCall& call, uint32_t frame) {
  return {base, call, Load(base, tuning_screen::MovieEntry(base, frame, 14)), frame};
}

// The slots' images and nodes: for j 0 to 4 ShowStickerListImage(id(j), j)
// and, unless `skip_current` and j is l, ShowSlotImage(id(j), j) and
// PlaceStickerNode(id(j), j, rec(j), or D for l with `decal_current`).
void ShowSlots(uint8_t* base, uint32_t s, uint32_t frame, bool skip_current, bool decal_current) {
  for (uint32_t j = 0; j < 5; j++) {
    ShowStickerListImage(base, s, PlacedId(base, s, j), j, frame);
    if (skip_current && Load(base, s + kSlot) == j) {
      continue;
    }
    ShowSlotImage(base, s, PlacedId(base, s, j), j, frame);
    const bool current = decal_current && j == Load(base, s + kSlot);
    const uint32_t record = current ? s + kDecal : Record(base, s, j);
    PlaceStickerNode(base, s, PlacedId(base, s, j), j, record, frame);
  }
}

// The order page's end (Command40 and Command41): movie 14 found again, the
// arrows (30, 2p + 47) shown when the cursor k is above 0 and (30, 2(p +
// 24)) when below 4, then for j 0 to 4 with i = id(L[j]): ShowSlotImage(i,
// j), PlaceStickerNode(i, j, rec(L[j])), ShowSlotNode(i, j, k). The last
// ShowSlotNode's r3.
uint64_t ShowOrder(uint8_t* base, uint32_t s, uint32_t frame) {
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, 14));
  const uint32_t k = Load(base, Menu(base, s));
  const uint32_t p = Load(base, s + kOrder);
  SetShown(base, movie, 30, (p << 1) + 47, int32_t(k) > 0 ? 1 : 0);
  SetShown(base, movie, 30, (Load(base, s + kOrder) + 24) << 1, int32_t(k) < 4 ? 1 : 0);
  uint64_t r3 = 0;
  for (uint32_t j = 0; j < 5; j++) {
    const uint32_t i =
        Load(base, (((Load(base, s + kArea) + 1666) * 5 + OrderWord(base, s, j)) << 2) + s);
    ShowSlotImage(base, s, i, j, frame);
    const uint32_t record =
        (Load(base, s + kArea) * 5 + OrderWord(base, s, j)) * 24 + s + 32360;
    PlaceStickerNode(base, s, i, j, record, frame);
    r3 = ShowSlotNode(base, s, i, j, k, frame);
  }
  return r3;
}

// Command33, Command37 and Command39: the cursor's move on a menu. prev =
// m+4, k = m+0, d = k - prev: prev + `base_named` finished and unlooped,
// `back` and `back` + 1 finished; d -1 plays `back`, d 1 `back` + 1 (chained
// 0), any other d but 0 puts node (sub, child)'s child 0 at k × 40; then
// `between` (Command33's help line), the movie side with 1.
struct Moved {
  uint32_t bytes;
  uint64_t base_named;
  uint64_t back;
  uint32_t sub, child;
  uint32_t lr[5];  // Finish, Unloop, Finish back, Finish back + 1, Play
};
template <typename Between, typename Side>
uint64_t MoveCursor(uint8_t* base, const GuestCall& call, uint32_t s, uint32_t sp,
                    const Moved& moved, const Between& between, const Side& side) {
  const uint32_t frame = Frame(base, sp, moved.bytes);
  Movie m = Open(base, call, frame);
  const uint32_t menu = Menu(base, s);
  const uint32_t prev = Load(base, menu + 4);
  const uint32_t k = Load(base, menu + 0);
  const int32_t d = int32_t(k - prev);
  const uint64_t named = uint64_t(prev) + moved.base_named;  // lwz, then addi
  m.Finish(named, moved.lr[0]);
  m.Unloop(named, moved.lr[1]);
  m.Finish(moved.back, moved.lr[2]);
  m.Finish(moved.back + 1, moved.lr[3]);
  if (uint32_t(d + 1) <= 2) {
    if (d < 0) {
      m.Play(moved.back, 0, moved.lr[4]);
    } else if (d > 0) {
      m.Play(moved.back + 1, 0, moved.lr[4]);
    }
  } else {
    m.Mark(moved.sub, moved.child, k);
  }
  between(m, prev, k);
  return side(frame);
}

}  // namespace

bool IsStickerCommand(uint32_t command) {
  return (command >= 32 && command <= 41) || (command >= 43 && command <= 51);
}

uint64_t Command32(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FC7B8 TuningScreen::Command32");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  Movie m = Open(base, call, frame);
  ResetMenu(base, s, 7, 1, kArea);
  if (Load(base, s + kEnding) != 0) {
    uint64_t named;
    if (Previous(base, s) == 42) {
      m.Finish(uint32_t((Load(base, s + kOrder) + 45) << 2), 0x821FC870);
      named = uint32_t((Load(base, s + kOrder) + 45) << 2);
    } else {
      m.Finish(205, 0x821FC888);
      named = 205;
    }
    m.Play(named, 1, 0x821FC8A4);
    Store(base, s + kEnding, 0);
  } else {
    m.Restart(203, 0x821FC8B4, 0x821FC8D0);
    m.Looped(1, {0x821FC8DC, 0x821FC8F8, 0x821FC904});
    m.Looped(153, {0x821FC910, 0x821FC92C, 0x821FC938});
    // The busy icon: shown 0, movie 14, named 155, 154 and 156.
    Store(base, s + 10016, 0);
    Store(base, s + 10020, 14);
    Store(base, s + 10024, 155);
    Store(base, s + 10028, 154);
    Store(base, s + 10032, 156);
  }
  m.Mark(kAll, 3, Load(base, s + kArea));
  m.HideList();
  for (uint32_t i = 5; i < 13; i++) {
    m.Text(3, i, i + 271, 0x821FC9CC);
  }
  call(kSetChildNameText, {m.movie, 0, 6, r3 + 20792}, frame, 0x821FC9EC);
  // The help line: 858 with the garage car's id 0 (+33508) and k 6 or 7,
  // else 825.
  call(tuning_screen::kResetTextScroll, {m.movie, 2, 0}, frame, 0x821FC9FC);
  uint64_t id = 825;
  if (Load(base, s + 33508) != 0) {
    const int32_t k = int32_t(Load(base, Menu(base, s)));
    if (k >= 6 && k <= 7) {
      id = 858;
    }
  }
  m.Text(2, 0, id, 0x821FCA40);
  call(tuning_screen::kScrollText, {m.movie, 2, 0, 1}, frame, 0x821FCA54);
  m.Shown(1, 1, 1);
  const uint64_t answer = AnimateStickerAreas(base, call, s, 1, frame);
  Store(base, s + 9716, 0);
  return answer;
}

uint64_t Command33(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FCA80 TuningScreen::Command33");
  const uint32_t s = uint32_t(r3);
  // The help line again when the garage car's id is 0 (+33508) and k moved
  // into or out of the rows 6 and 7: 858 into them, 825 out.
  auto between = [&](Movie& m, uint32_t prev, uint32_t k) {
    if (Load(base, s + 33508) == 0) {
      return;
    }
    const bool was = int32_t(prev) == 6 || int32_t(prev) == 7;
    const bool is = int32_t(k) == 6 || int32_t(k) == 7;
    if (was != is) {
      m.Help(is ? 858 : 825, {0x821FCBC8, 0x821FCBE8, 0x821FCBFC});
    }
  };
  auto side = [&](uint32_t frame) {
    const uint64_t answer = AnimateStickerAreas(base, call, s, 1, frame);
    tuning_screen::Advance(base, s);
    return answer;
  };
  return MoveCursor(base, call, s, sp,
                    {160, 6, 16, kAll, 3,
                     {0x821FCAD0, 0x821FCADC, 0x821FCAE8, 0x821FCAF4, 0x821FCB24}},
                    between, side);
}

uint64_t Command34(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FA658 TuningScreen::Command34");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 160);
  Movie m = Open(base, call, frame);
  {
    // m reset with its cursor l, 5 at most (the buttons' row).
    const uint32_t menu = Menu(base, s);
    Store(base, menu + 12, 0);
    Store(base, menu + 20, 1);
    Store(base, menu + 16, 5);
    for (const uint32_t at : {0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
      Store(base, menu + at, 0);
    }
    uint32_t l = Load(base, s + kSlot);
    if (int32_t(l) >= 5) {
      l = 5;
    }
    const uint32_t again = Menu(base, s);
    const uint32_t old = Load(base, again + 0);
    Store(base, again + 0, l);
    Store(base, again + 4, old);
  }
  if (Load(base, s + kEnding) != 0) {
    const int32_t previous = Previous(base, s);
    uint64_t named = 207;
    if (previous == 41) {
      m.Finish(220, 0x821FA718);
      named = 220;
    } else if (previous == 43) {
      m.Finish(219, 0x821FA73C);
      named = 219;
    } else if (previous == 42) {
      // Back from the order page: sub-scene 30's nodes 2 to 6 put at their
      // places, the slot l's x less the shift.
      ui_movies::FindNode(base, m.movie, 30, 2);
      const double shift = steps::Lfs(base, kListShift);
      for (uint32_t i = 0; i < 5; i++) {
        const uint32_t node = ui_movies::FindNode(base, m.movie, 30, i + 2);
        steps::Stfs(base, node + 12, steps::Lfs(base, kListPlaces + 8 * i));
        steps::Stfs(base, node + 16, steps::Lfs(base, kListPlaces + 8 * i + 4));
        if (int32_t(i) == int32_t(Load(base, s + kSlot))) {
          steps::Stfs(base, node + 12, steps::Fsubs(steps::Lfs(base, node + 12), shift));
        }
      }
      m.Finish(222, 0x821FA7CC);
      m.Play(222, 0, 0x821FA7E8);
      m.Finish(uint32_t((Load(base, s + kOrder) + 45) << 2), 0x821FA7FC);
      named = uint32_t((Load(base, s + kOrder) + 45) << 2);
    } else {
      m.Finish(207, 0x821FA818);
    }
    m.Play(named, 1, 0x821FA834);
    Store(base, s + kEnding, 0);
  } else {
    m.Restart(204, 0x821FA848, 0x821FA864);
    m.Restart(43, 0x821FA870, 0x821FA88C);
  }
  m.HideList();
  ShowSlots(base, s, frame, false, false);
  m.Text(9, 4, 4, 0x821FA94C);
  m.Text(9, 5, 5, 0x821FA960);
  m.Help(826, {0x821FA970, 0x821FA984, 0x821FA998});
  m.Shown(1, 1, 0);
  return AnimateStickerSlots(base, call, s, 1, frame);
}

uint64_t Command35(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FA9C0 TuningScreen::Command35");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t answer = AnimateStickerSlots(base, call, s, 1, frame);
  tuning_screen::Advance(base, s);
  return answer;
}

uint64_t Command36(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FAA28 TuningScreen::Command36");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 128);
  Movie m = Open(base, call, frame);
  ResetMenu(base, s, 3, 1, kOperation);
  if (Load(base, s + kEnding) != 0) {
    const int32_t previous = Previous(base, s);
    if (previous == 41) {
      m.Finish(211, 0x821FAAD8);
      m.Play(211, 1, 0x821FAB6C);
    } else if (previous == 42) {
      m.Finish(223, 0x821FAAF8);
      m.Play(223, 0, 0x821FAB14);
      m.Finish(uint32_t((Load(base, s + kOrder) + 45) << 2), 0x821FAB28);
      m.Play(uint32_t((Load(base, s + kOrder) + 45) << 2), 1, 0x821FAB6C);
    } else if (previous == 43) {
      m.Finish(209, 0x821FAB50);
      m.Play(209, 0, 0x821FAB6C);
    }
    Store(base, s + kEnding, 0);
  } else {
    m.Restart(206, 0x821FAB80, 0x821FAB9C);
  }
  m.Finish(uint64_t(Load(base, s + kSlot)) + 173, 0x821FABAC);
  m.Play(uint64_t(Load(base, s + kSlot)) + 173, 1, 0x821FABCC);
  m.Mark(kAll, 10, Load(base, s + kOperation));
  m.HideList();
  ShowSlots(base, s, frame, false, false);
  for (uint32_t i = 5; i < 9; i++) {
    m.Text(10, i, i + 332, 0x821FACCC);
  }
  m.Shown(1, 1, 1);
  return AnimateStickerOperate(base, call, s, 1, frame);
}

uint64_t Command37(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8C30 TuningScreen::Command37");
  const uint32_t s = uint32_t(r3);
  auto side = [&](uint32_t frame) {
    const uint64_t answer = AnimateStickerOperate(base, call, s, 1, frame);
    tuning_screen::Advance(base, s);
    return answer;
  };
  return MoveCursor(base, call, s, sp,
                    {144, 49, 59, kAll, 10,
                     {0x821F8C80, 0x821F8C8C, 0x821F8C98, 0x821F8CA4, 0x821F8CD4}},
                    [](Movie&, uint32_t, uint32_t) {}, side);
}

uint64_t Command38(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FAD00 TuningScreen::Command38");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  Movie m = Open(base, call, frame);
  ResetMenu(base, s, 5, 1, kEditRow);
  const bool ending = Load(base, s + kEnding) != 0;
  const int32_t previous = Previous(base, s);
  if (ending) {
    if (previous == 44 || previous == 45 || previous == 46) {
      m.Finish(213, 0x821FADDC);
      m.Play(213, 1, 0x821FADF8);
    } else if (previous == 49) {
      m.Finish(226, 0x821FADC8);
      m.Play(226, 1, 0x821FADF8);
    }
    Store(base, s + kEnding, 0);
  } else if (previous == 40) {
    m.Finish(224, 0x821FAE14);
    m.Play(224, 1, 0x821FAE4C);
  } else if (previous == 43) {
    m.Finish(210, 0x821FAE30);
    m.Play(210, 1, 0x821FAE4C);
  }
  m.Finish(uint64_t(Load(base, s + kSlot)) + 173, 0x821FAE5C);
  m.Play(uint64_t(Load(base, s + kSlot)) + 173, 1, 0x821FAE7C);
  ShowSlots(base, s, frame, false, true);
  m.Mark(11, 1, Load(base, s + kEditRow));
  m.HideList();
  // The rows' names: sub-scene 11's child 1's children 5 to 8 strings 456 to
  // 459, its child 9 461; child 0's children 4 and 5 strings 4 and 5.
  const uint32_t rows = ui_movies::FindNode(base, m.movie, 11, 1);
  for (uint32_t i = 0; i < 4; i++) {
    m.NodeText(ui_movies::Child(base, rows, i + 5), i + 456, 0x821FAFC0);
  }
  m.NodeText(ui_movies::Child(base, rows, 9), 461, 0x821FAFE8);
  const uint32_t buttons = ui_movies::FindNode(base, m.movie, 11, 0);
  m.NodeText(ui_movies::Child(base, buttons, 4), 4, 0x821FB014);
  m.NodeText(ui_movies::Child(base, buttons, 5), 5, 0x821FB030);
  m.Texture(1, 4, 0);
  m.Texture(1, 5, 0);
  m.Texture(1, 6, 1);
  m.Texture(1, 7, 1);
  m.Shown(1, 1, 0);
  m.Shown(1, 10, 0);
  return AnimateStickerEditMenu(base, call, s, 1, frame);
}

uint64_t Command39(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FB0B8 TuningScreen::Command39");
  const uint32_t s = uint32_t(r3);
  auto side = [&](uint32_t frame) {
    const uint64_t answer = AnimateStickerEditMenu(base, call, s, 1, frame);
    tuning_screen::Advance(base, s);
    return answer;
  };
  return MoveCursor(base, call, s, sp,
                    {144, 92, 102, 11, 1,
                     {0x821FB108, 0x821FB114, 0x821FB120, 0x821FB12C, 0x821FB15C}},
                    [](Movie&, uint32_t, uint32_t) {}, side);
}

uint64_t Command40(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FCC48 TuningScreen::Command40");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  Movie m = Open(base, call, frame);
  ResetMenu(base, s, 4, 0, kOrder);
  if (Load(base, s + kEnding) != 0) {
    tuning_screen::Advance(base, s);
    Store(base, s + kEnding, 0);
  } else {
    m.Restart(221, 0x821FCD24, 0x821FCD40);
  }
  m.Finish(uint64_t(uint32_t(Load(base, s + kOrder) << 2)) + 179, 0x821FCD54);
  m.Play(uint64_t(uint32_t(Load(base, s + kOrder) << 2)) + 179, 1, 0x821FCD78);
  m.Shown(30, Load(base, s + kOrder) * 5 + 12, 1);
  for (uint32_t j = 0; j < 5; j++) {
    ShowStickerListImage(base, s, PlacedId(base, s, j), j, frame);
  }
  m.Shown(1, 1, 1);
  return ShowOrder(base, s, frame);
}

uint64_t Command41(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FCED0 TuningScreen::Command41");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 160);
  Movie m = Open(base, call, frame);
  const uint32_t l = Load(base, s + kSlot);
  const uint32_t menu = Menu(base, s);
  const uint32_t prev = Load(base, menu + 4);
  const uint32_t k = Load(base, menu + 0);
  const int32_t d = int32_t(k - prev);
  const uint64_t named = uint64_t(prev) + 173;  // lwz, then addi
  m.Finish(named, 0x821FCF24);
  m.Unloop(named, 0x821FCF30);
  for (uint64_t n = 182; n < 202; n += 4) {
    m.Finish(n - 1, 0x821FCF40);
    m.Finish(n, 0x821FCF4C);
  }
  // The moved word's animation from one end, the word now at prev's from
  // the other (chained 0).
  if (uint32_t(d + 1) <= 2 && d != 0) {
    const uint64_t first = uint64_t(uint32_t(l << 2)) + (d < 0 ? 181 : 182);
    m.Play(first, 0, d < 0 ? 0x821FCF8C : 0x821FCFC8);
    const uint64_t second = uint64_t(uint32_t(OrderWord(base, s, prev) << 2)) + (d < 0 ? 182 : 181);
    m.Play(second, 0, 0x821FCFF4);
  }
  const uint64_t answer = ShowOrder(base, s, frame);
  tuning_screen::Advance(base, s);
  return answer;
}

uint64_t Command43(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FD128 TuningScreen::Command43");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 128);
  Movie m = Open(base, call, frame);
  ResetMenu(base, s, 198, 1, kPick);
  {
    const uint32_t menu = Menu(base, s);
    Store(base, menu + 24, 1);
    Store(base, menu + 28, 1);
    Store(base, menu + 32, 0);
  }
  if (Load(base, s + kEnding) != 0) {
    tuning_screen::Advance(base, s);
    Store(base, s + kEnding, 0);
  } else {
    const bool slots = Previous(base, s) == 39;
    m.Finish(slots ? 218 : 208, slots ? 0x821FD220 : 0x821FD230);
    m.Play(slots ? 218 : 208, 1, 0x821FD24C);
  }
  m.Finish(uint64_t(Load(base, s + kSlot)) + 173, 0x821FD25C);
  m.Play(uint64_t(Load(base, s + kSlot)) + 173, 1, 0x821FD27C);
  m.Looped(160, {0x821FD288, 0x821FD2A4, 0x821FD2B0});
  m.Looped(159, {0x821FD2BC, 0x821FD2D8, 0x821FD2E4});
  m.HideList();
  ShowSlots(base, s, frame, true, false);
  m.Help(832, {0x821FD3AC, 0x821FD3C0, 0x821FD3D4});
  m.Shown(1, 1, 1);
  return ShowStickerPicker(base, call, s, 1, frame);
}

uint64_t Command44(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FD3F8 TuningScreen::Command44");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  Movie m = Open(base, call, frame);
  const uint32_t menu = Menu(base, s);
  uint64_t k = Load(base, menu + 0);
  const uint32_t prev = Load(base, menu + 4);
  const int32_t d = int32_t(uint32_t(k) - prev);
  m.Finish(162, 0x821FD440);
  m.Finish(161, 0x821FD44C);
  // The picker's cursor moved one owned sticker from prev: back for d -1 or
  // above 1, forward for d 1 or below -1 (the wraps).
  if (d != 0) {
    const bool back = d == -1 || d > 1;
    m.Play(back ? 161 : 162, 1, back ? 0x821FD4A8 : 0x821FD484);
    k = StepUnlockedSticker(base, s, prev, back ? ~0ull : 1);
  }
  const uint32_t again = Menu(base, s);
  const uint32_t old = Load(base, again + 0);
  Store(base, again + 0, uint32_t(k));
  Store(base, again + 4, old);
  const uint64_t answer = ShowStickerPicker(base, call, s, 1, frame);
  tuning_screen::Advance(base, s);
  return answer;
}

namespace {

// Command45, Command47 and Command49: the edit pages' set-up. With the
// command's end asked the queue's step and +9720 0 (in `clear_first`'s
// order), else `named` restarted chained; the slots (D for l); sub-scene
// 31's children placed by `place`; SetStickerEditTexts, the help line `id`,
// node (1, 1) shown (its r3).
template <typename Place>
uint64_t EditPage(uint8_t* base, const GuestCall& call, uint32_t s, uint32_t sp, uint32_t bytes,
                  bool clear_first, uint64_t named, const uint32_t (&restart_lr)[2], uint64_t id,
                  const uint32_t (&help_lr)[3], const Place& place) {
  const uint32_t frame = Frame(base, sp, bytes);
  Movie m = Open(base, call, frame);
  if (Load(base, s + kEnding) != 0) {
    if (clear_first) {
      Store(base, s + kEnding, 0);
    }
    tuning_screen::Advance(base, s);
    if (!clear_first) {
      Store(base, s + kEnding, 0);
    }
  } else {
    m.Restart(named, restart_lr[0], restart_lr[1]);
  }
  ShowSlots(base, s, frame, false, true);
  place(m, ui_movies::FindNode(base, m.movie, kAll, 31));
  SetStickerEditTexts(base, call, s, frame);
  m.Help(id, help_lr);
  return m.Shown(1, 1, 1);
}

// Sub-scene 31's child c + 4 for the pair k (the area's pairs at
// kEditPlaces + 56a, 8 bytes each, plus the offsets at kEditPlaces + 448 +
// 8k): x at +12, y at +16, through the frame's +80 and +88.
void PlaceEditNode(uint8_t* base, uint32_t s, uint32_t frame, uint32_t node, uint32_t k) {
  const uint32_t child = ui_movies::Child(base, node, k + 4);
  const uint32_t pair = kEditPlaces + ((Load(base, s + kArea) * 7 + k) << 3);
  const int64_t x = int32_t(Load(base, pair) + Load(base, kEditPlaces + 448 + 8 * k));
  Store(base, child + 12, AsFloat(base, frame + 80, x));
  const uint32_t pair_y = kEditPlaces + 4 + ((Load(base, s + kArea) * 7 + k) << 3);
  const int64_t y = int32_t(Load(base, pair_y) + Load(base, kEditPlaces + 452 + 8 * k));
  Store(base, child + 16, AsFloat(base, frame + 88, y));
}

}  // namespace

uint64_t Command45(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FB1F0 TuningScreen::Command45");
  const uint32_t s = uint32_t(r3);
  return EditPage(base, call, s, sp, 160, true, 212, {0x821FB274, 0x821FB290}, 841,
                  {0x821FB40C, 0x821FB420, 0x821FB434}, [&](Movie& m, uint32_t node) {
                    for (uint32_t k = 0; k < 4; k++) {
                      PlaceEditNode(base, s, m.frame, node, k);
                    }
                  });
}

uint64_t Command46(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8FC0 TuningScreen::Command46");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t l = Load(base, s + kSlot);
  PlaceStickerNode(base, s, PlacedId(base, s, l), l, s + kDecal, frame);
  const uint64_t answer = SetStickerEditTexts(base, call, s, frame);
  tuning_screen::Advance(base, s);
  return answer;
}

uint64_t Command47(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FB450 TuningScreen::Command47");
  const uint32_t s = uint32_t(r3);
  return EditPage(base, call, s, sp, 160, false, 214, {0x821FB4D4, 0x821FB4F0}, 842,
                  {0x821FB66C, 0x821FB680, 0x821FB694}, [&](Movie& m, uint32_t node) {
                    for (uint32_t k = 4; k < 6; k++) {
                      PlaceEditNode(base, s, m.frame, node, k);
                    }
                  });
}

uint64_t Command49(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FB6B0 TuningScreen::Command49");
  const uint32_t s = uint32_t(r3);
  // Child 10: the area's pair 6 plus 208 and 272 (through the frame's +80
  // both times).
  return EditPage(base, call, s, sp, 128, false, 215, {0x821FB734, 0x821FB750}, 843,
                  {0x821FB898, 0x821FB8AC, 0x821FB8C0}, [&](Movie& m, uint32_t node) {
                    const uint32_t child = ui_movies::Child(base, node, 10);
                    const uint32_t x = Load(base, kEditPlaces + 48 + Load(base, s + kArea) * 56);
                    Store(base, child + 12, AsFloat(base, m.frame + 80, int32_t(x + 208)));
                    const uint32_t y = Load(base, kEditPlaces + 52 + Load(base, s + kArea) * 56);
                    Store(base, child + 16, AsFloat(base, m.frame + 80, int32_t(y + 272)));
                  });
}

uint64_t Command51(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FB8D8 TuningScreen::Command51");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 128);
  Movie m = Open(base, call, frame);
  if (Load(base, s + kEnding) != 0) {
    tuning_screen::Advance(base, s);
    Store(base, s + kEnding, 0);
  } else {
    m.Restart(225, 0x821FB95C, 0x821FB978);
  }
  for (uint32_t j = 0; j < 5; j++) {
    ShowSlotImage(base, s, kAll, j, frame);
  }
  m.Help(845, {0x821FB9A4, 0x821FB9B8, 0x821FB9CC});
  m.Texture(1, 4, 0);
  m.Texture(1, 5, 1);
  m.Shown(1, 1, 0);
  m.Shown(1, 10, 1);
  const uint32_t child = ui_movies::Child(base, ui_movies::FindNode(base, m.movie, kAll, 1), 3);
  steps::Stfs(base, child + 12, steps::Lfs(base, kFreeLookX));
  return child;
}

std::optional<uint64_t> StickerCommand(uint8_t* base, const GuestCall& call, uint32_t command,
                                       uint64_t r3, uint32_t sp) {
  switch (command) {
    case 32: return Command32(base, call, r3, sp);
    case 33: return Command33(base, call, r3, sp);
    case 34: return Command34(base, call, r3, sp);
    case 35: return Command35(base, call, r3, sp);
    case 36: return Command36(base, call, r3, sp);
    case 37: return Command37(base, call, r3, sp);
    case 38: return Command38(base, call, r3, sp);
    case 39: return Command39(base, call, r3, sp);
    case 40: return Command40(base, call, r3, sp);
    case 41: return Command41(base, call, r3, sp);
    case 43: return Command43(base, call, r3, sp);
    case 44: return Command44(base, call, r3, sp);
    case 45: return Command45(base, call, r3, sp);
    case 46: case 48: case 50: return Command46(base, call, r3, sp);
    case 47: return Command47(base, call, r3, sp);
    case 49: return Command49(base, call, r3, sp);
    case 51: return Command51(base, call, r3, sp);
    default: return std::nullopt;
  }
}

}  // namespace tuning_pages
