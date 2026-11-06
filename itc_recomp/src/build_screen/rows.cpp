// The build screen's rows, arrows, marks, prompts, help line and number
// (docs/functions/build-screen.md): see build_screen.h. The text setters
// (UiMovie::SetNodeText, ScrollNodeText, ResetNodeTextScroll) through their
// overrides; SetNodeText's unset varargs (r6 to r10) passed as 0 (the
// user's call for SetText's, 2026-10-30: only dead words of the caller's
// parameter area differ).

#include "build_screen/build_screen.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>
#include <climits>
#include <cmath>

namespace build_screen {

namespace {

constexpr uint32_t kSetNodeText = 0x8211C7E8;
constexpr uint32_t kScrollNodeText = 0x8211CCC0;
constexpr uint32_t kResetNodeTextScroll = 0x8211CD28;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Screen(uint8_t* base) { return Load(base, kScreen); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

uint32_t Node(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  return ui_movies::FindNode(base, movie, sub_scene, child);
}

void Show(uint8_t* base, uint32_t node, uint32_t shown) { Store(base, node + 80, shown); }

// The arrows of a list of `rows` rows: `up` hidden at the top, `down`
// hidden when the last row is the count's.
uint32_t Arrows(uint8_t* base, uint32_t sp, uint32_t sub_scene, uint32_t up_child,
                uint32_t rows, uint32_t scroll, uint32_t count) {
  const uint32_t movie = Movie(base, Frame(base, sp, 128));
  const uint32_t up = Node(base, movie, sub_scene, up_child);
  const uint32_t down = Node(base, movie, sub_scene, up_child + 1);
  if (scroll == 0) {
    Show(base, up, 0);
    Show(base, down, 1);
  } else {
    Show(base, up, 1);
    Show(base, down, scroll + rows != count ? 1 : 0);
  }
  return down;
}

}  // namespace

uint32_t Movie(uint8_t* base, uint32_t frame) {
  Store(base, frame + 80, kMovieId);
  return Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

uint32_t GroupArrows(uint8_t* base, uint32_t sp, uint32_t scroll, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8219D1B0 BuildScreen::GroupArrows");
  return Arrows(base, sp, 7, 10, 11, scroll, count);
}

uint32_t BuildArrows(uint8_t* base, uint32_t sp, uint32_t scroll, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8219D4B8 BuildScreen::BuildArrows");
  return Arrows(base, sp, 8, 2, 6, scroll, count);
}

uint32_t StandArrows(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219D7B0 BuildScreen::StandArrows");
  const uint32_t movie = Movie(base, Frame(base, sp, 96));
  const uint32_t node = Node(base, movie, 0xFFFFFFFFu, 23);
  Show(base, node, guest::LoadU16(base, Screen(base) + kBuildCount) == 1 ? 0 : 1);
  return node;
}

uint32_t ShowBuildMark(uint8_t* base, uint32_t sp, uint32_t build) {
  ITC_NATIVE_RUNNING("sub_8219D440 BuildScreen::ShowBuildMark");
  const uint32_t movie = Movie(base, Frame(base, sp, 112));
  const uint32_t node = Node(base, movie, 17, 0);
  const uint32_t mark = Load(base, build + 16);
  if (mark == 0) {
    Show(base, node, mark);
  } else {
    Show(base, node, 1);
    Store(base, node + 108, Load(base, build + 0) - 310);
  }
  return node;
}

uint32_t SetPrompts(uint8_t* base, uint32_t sp, const uint32_t pairs[6]) {
  ITC_NATIVE_RUNNING("sub_8219DA58 BuildScreen::SetPrompts");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = Movie(base, frame);
  // The icons (r3, r5, r7) at +88, the labels (r4, r6, r8) at +104.
  Store(base, frame + 112, pairs[5]);
  Store(base, frame + 92, pairs[2]);
  Store(base, frame + 96, pairs[4]);
  Store(base, frame + 104, pairs[1]);
  Store(base, frame + 108, pairs[3]);
  Store(base, frame + 88, pairs[0]);
  uint32_t last = 0;
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t slot = Node(base, movie, 2, k);
    const uint32_t icon_node = Node(base, movie, 2, Load(base, slot + 100));
    const uint32_t label_node = Node(base, movie, 2, Load(base, slot + 100) + 1);
    const uint32_t icon = Load(base, frame + 88 + 4 * k);
    const uint32_t label = Load(base, frame + 104 + 4 * k);
    if (int32_t(icon) != -1 && int32_t(label) != -1) {
      Show(base, icon_node, 1);
      Show(base, label_node, 1);
      Store(base, icon_node + 108, icon);
      Store(base, label_node + 108, label);
    } else {
      Show(base, icon_node, 0);
      Show(base, label_node, 0);
    }
    last = label_node;
  }
  return last;
}

uint64_t SetHelp(uint8_t* base, uint32_t sp, uint32_t id, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219DB50 BuildScreen::SetHelp");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Movie(base, frame);
  const uint32_t node = Node(base, movie, 1, 0);
  calls.Call(kResetNodeTextScroll, frame, 0x8219DB9C, movie, node);
  calls.Call(kSetNodeText, frame, 0x8219DBAC, movie, node, id);
  return calls.Call(kScrollNodeText, frame, 0x8219DBBC, movie, node, 1);
}

double PowI(uint8_t* base, double x, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_8219BD30 BuildScreen::PowI");
  double square = x;
  uint32_t bits = int32_t(n) < 0 ? 0u - n : n;
  const double one = double(std::bit_cast<float>(Load(base, 0x82000A30)));
  double power = one;
  for (;;) {
    if ((bits & 1) != 0) {
      power = double(float(power * square));
    }
    bits >>= 1;
    if (bits == 0) {
      break;
    }
    square = double(float(square * square));
  }
  if (int32_t(n) < 0) {
    power = double(float(one / power));
  }
  return power;
}

namespace {

// fctiwz as the recompiled code does it: NaN 0x80000000, at or past
// INT_MAX INT_MAX, else truncated (0x80000000 below INT_MIN).
uint32_t ToIntTruncated(double f) {
  if (std::isnan(f)) {
    return 0x80000000u;
  }
  if (f >= double(INT_MAX)) {
    return uint32_t(INT_MAX);
  }
  const double t = std::trunc(f);
  if (t < double(INT_MIN)) {
    return 0x80000000u;
  }
  return uint32_t(int32_t(t));
}

}  // namespace

uint32_t ShowNumber(uint8_t* base, uint32_t sp, uint32_t number) {
  ITC_NATIVE_RUNNING("sub_8219D090 BuildScreen::ShowNumber");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = Movie(base, frame);
  uint32_t n = number;
  Show(base, Node(base, movie, 12, 0), int32_t(n) == 100 ? 1 : 0);
  const uint32_t digits = Node(base, movie, 12, 3);
  uint32_t r3 = digits;
  const double ten = double(std::bit_cast<float>(Load(base, 0x82001FD8)));
  for (int32_t k = int32_t(Load(base, digits + 104) - 1); k >= 0; k--) {
    const uint32_t first = Load(base, digits + 100);
    const int32_t quotient = int32_t(n) / 10;
    const uint32_t digit = n - uint32_t(quotient) * 10;
    const uint32_t child = Node(base, movie, 12, uint32_t(k) + first);
    r3 = child;
    if (int32_t(digit) == 0) {
      const int32_t places = int32_t(Load(base, digits + 104) - uint32_t(k)) - 1;
      if (places > 0) {
        const uint32_t power = ToIntTruncated(PowI(base, ten, uint32_t(places)));
        Store(base, frame + 80, power);
        if (int32_t(n) < int32_t(power)) {
          Show(base, child, 0);
          continue;
        }
      }
    }
    Show(base, child, 1);
    Store(base, child + 108, k == 0 ? 0 : digit);
    n = uint32_t(int32_t(n) / 10);
  }
  return r3;
}

uint64_t DrawGroupRow(uint8_t* base, uint32_t sp, uint32_t row, uint32_t index,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219CE08 BuildScreen::DrawGroupRow");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = Movie(base, frame);
  const uint32_t rows = Node(base, movie, 7, 4);
  const uint32_t name = ui_movies::Child(base, rows, row + 40);
  const uint32_t new_mark = ui_movies::Child(base, rows, row + 16);
  const uint32_t mark = ui_movies::Child(base, rows, row + 4);
  const uint32_t text = ui_movies::Child(base, name, 0);
  const uint32_t screen = Screen(base);
  if (index >= Load(base, screen + kShownCount)) {
    return text;
  }
  const uint32_t group = Load(base, screen + ((index + 0x10000 - 14799) << 2));
  const uint64_t r3 = calls.Call(kSetNodeText, frame, 0x8219CEC4, movie, text,
                                 uint64_t(Load(base, group + 0)) + 5299);
  Show(base, mark, Load(base, group + 32) != 0 ? 1 : 0);
  Show(base, new_mark, Load(base, group + 28) != 0 ? 1 : 0);
  return r3;
}

uint64_t DrawGroupRows(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219FA28 BuildScreen::DrawGroupRows");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t scroll = Load(base, Load(base, kList) + 4);
  uint64_t r3 = 0;
  for (uint32_t r = 0; r < 11; r++) {
    r3 = DrawGroupRow(base, frame, r, r + scroll, calls);
  }
  return r3;
}

}  // namespace build_screen
