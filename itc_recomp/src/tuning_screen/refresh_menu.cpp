// The tuning screen's menus on movie 2 by state (TuningScreen::RefreshMenu,
// docs/functions/tuning-screen.md): see tuning_screen.h. Movie 2's nodes
// through the UI movies' plain C++ (FindNode, Child, AnimationTime,
// NamedAnimationTime, ShowNode's and HideNode's stores, SetNodeColor's and
// sub_8211A630's colors); the animations (Finish, Play, Loop and their
// Named versions), SetText and SetNodeText through their overrides from the
// original's frame with its return addresses, the varargs SetText and
// SetNodeText leave unset passed as 0 (the user's call, 2026-10-30).

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace tuning_screen {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  Store(sp - bytes, sp);
  return sp - bytes;
}

namespace steps = camera_modes::steps;

constexpr uint32_t kSetNodeText = 0x8211C7E8;  // UiMovie::SetNodeText(movie, node, id, ...)
constexpr uint32_t kAll = 0xFFFFFFFFu;         // sub-scene -1: the scene itself
constexpr uint32_t kAtEnd = 0x82002CAC;        // C: the time the blocks wait for
constexpr uint32_t kTabsAt = 0x820021E8;       // state 16's first time
constexpr uint32_t kCategoriesAt = 0x82000C34;  // state 3's first time
constexpr uint32_t kOtherAt = 0x82002C78;      // states 56's and 53's other time
constexpr uint32_t kFarRow = 0x820027A4;       // state 56's highlight for 4
constexpr uint32_t kRow = 0x82001FB0;          // 40.0: a row's height

struct Menu {
  const GuestCall& call;
  uint32_t screen;
  uint32_t frame;
  uint32_t movie;
  std::optional<double>* f1;
  uint64_t r3;

  uint32_t Word(uint32_t offset) const { return Load(screen + offset); }

  // The ints made floats through the frame (std at +80, lfd, fcfid, frsp,
  // fmuls by 40.0); the float's bits.
  uint32_t Scaled(int64_t x) const {
    Store(frame + 80, uint32_t(uint64_t(x) >> 32));
    Store(frame + 84, uint32_t(x));
    return steps::Bits(steps::Fmuls(steps::Single(double(x)), steps::Lfs(kRow)));
  }

  // UiMovie::NamedAnimationTime and AnimationTime: f1 set, r3 the movie
  // (left). True when the time is the float at `at`.
  bool NamedIs(uint32_t index, uint32_t at) {
    const double t = double(std::bit_cast<float>(ui_movies::NamedAnimationTime(movie, index)));
    *f1 = t;
    r3 = movie;
    return t == steps::Lfs(at);
  }
  bool TimeIs(uint32_t sub_scene, uint32_t index, uint32_t at) {
    const double t =
        double(std::bit_cast<float>(ui_movies::AnimationTime(movie, sub_scene, index)));
    *f1 = t;
    r3 = movie;
    return t == steps::Lfs(at);
  }

  uint32_t Node(uint32_t sub_scene, uint32_t child) {
    const uint32_t node = ui_movies::FindNode(movie, sub_scene, child);
    r3 = node;
    return node;
  }
  uint32_t ChildOf(uint32_t node, uint32_t i) {
    const uint32_t child = ui_movies::Child(node, i);
    r3 = child;
    return child;
  }
  // UiMovie::ShowNode and HideNode (sub_8211AC00, sub_8211AC48).
  void Shown(uint32_t sub_scene, uint32_t child, uint32_t shown) {
    UiNode(Node(sub_scene, child)).set_visible(shown);
  }
  // UiMovie::SetNodeColor (sub_8211A5B8): the node's color.
  void Color(uint32_t sub_scene, uint32_t child, uint32_t color) {
    ui_movies::SetColor(Node(sub_scene, child), color);
  }

  void Call(uint32_t function, std::initializer_list<uint64_t> args, uint32_t lr) {
    r3 = call(function, args, frame, lr);
  }
  void Text(uint32_t sub_scene, uint32_t child, uint64_t id, uint32_t lr) {
    Call(kSetText, {movie, sub_scene, child, id, 0, 0, 0, 0}, lr);
  }
  void NodeText(uint32_t node, uint64_t id, uint64_t argument, uint32_t lr) {
    Call(kSetNodeText, {movie, node, id, argument, 0, 0, 0, 0}, lr);
  }
  // Finish, Play(.., 1, 0, 0, 0) and Loop of sub-scene 27's animation
  // `index` (the word read again for each, as the original does).
  template <typename Index>
  void Animate27(const Index& index, const uint32_t (&lr)[3]) {
    Call(kFinish, {movie, 27, index()}, lr[0]);
    Call(kPlay, {movie, 27, index(), 1, 0, 0, 0}, lr[1]);
    Call(kLoop, {movie, 27, index()}, lr[2]);
  }
  // FinishNamed, PlayNamed(.., 1, 0, 0, 0) and LoopNamed.
  template <typename Index>
  void AnimateNamed(const Index& index, const uint32_t (&lr)[3]) {
    Call(kFinishNamed, {movie, index()}, lr[0]);
    Call(kPlayNamed, {movie, index(), 1, 0, 0, 0}, lr[1]);
    Call(kLoopNamed, {movie, index()}, lr[2]);
  }
  void Restart71(const uint32_t (&lr)[2]) {
    Call(kFinishNamed, {movie, 71}, lr[0]);
    Call(kPlayNamed, {movie, 71, 1, 0, 0, 0}, lr[1]);
  }
  // Sub-scene 27's nodes 5 to 14 hidden (and their color 0).
  void Hide27(bool color) {
    for (uint32_t n = 5; n < 15; n++) {
      Shown(27, n, 0);
      if (color) {
        Color(27, n, 0);
      }
    }
  }
  // Children 5 to 14 of node (sub_scene, 0) hidden, the node found for each.
  void HideChildren(uint32_t sub_scene, uint32_t child) {
    for (uint32_t i = 0; i < 10; i++) {
      UiNode(ChildOf(Node(sub_scene, child), i + 5)).set_visible(0);
    }
  }

  // State 16: the group tabs.
  void Tabs() {
    if (!NamedIs(1007, kTabsAt) && !NamedIs(62, kAtEnd)) {
      return;
    }
    Store(UiNode(ChildOf(Node(1, 9), 0)).y_address(), Scaled(int32_t(TS(screen).tabs_row())));
    HideChildren(1, 9);
    const uint32_t table = frame + 96;
    Store(table, 376);
    for (uint32_t i = 1; i < 17; i++) {
      Store(table + 4 * i, 1183 + i);
    }
    for (uint32_t j = 0; j < 4; j++) {
      const uint32_t tab = ChildOf(Node(1, 9), j + 5);
      UiNode(tab).set_visible(1);
      const uint32_t k = TS(screen).part_group() - TS(screen).tabs_row() + j;
      NodeText(tab, Load(table + 4 * k), 0, 0x821D8AF0);
    }
    AnimateNamed([&] { return uint64_t(TS(screen).tabs_row()) + 20; },
                 {0x821D8B0C, 0x821D8B2C, 0x821D8B3C});
  }

  // State 15: the three-choice menu.
  void ThreeChoices() {
    if (!NamedIs(964, kAtEnd) && !NamedIs(1008, kAtEnd) &&
        int32_t(TS(screen).command_end_after_setup()) != 1) {
      return;
    }
    Store(UiNode(Node(27, 0)).y_address(), Scaled(int32_t(TS(screen).wheel_part_cursor())));
    Hide27(false);
    for (uint32_t n = 5; n < 8; n++) {
      Shown(27, n, 1);
      Text(27, n, n + 192, 0x821D8BFC);
    }
    AnimateNamed([&] { return uint64_t(TS(screen).wheel_part_cursor()) + 389; },
                 {0x821D8C18, 0x821D8C38, 0x821D8C48});
    if (!NamedIs(1008, kAtEnd) || int32_t(TS(screen).part_group()) != 11) {
      return;
    }
    Restart71({0x821D8C74, 0x821D8C90});
  }

  // State 4: a category's item list.
  void Items() {
    static constexpr uint32_t kNamed[] = {966, 1004, 972, 973, 974, 975, 982};
    bool go = TimeIs(kAll, 1, kAtEnd) || TimeIs(kAll, 2, kAtEnd);
    for (const uint32_t named : kNamed) {
      go = go || NamedIs(named, kAtEnd);
    }
    if (!go && int32_t(TS(screen).command_end_after_setup()) != 1) {
      return;
    }
    const bool aero = int32_t(TS(screen).category()) == 4;
    const uint32_t bar = Node(27, 0);
    const int64_t at = aero ? int64_t(int32_t(TS(screen).list_scroll_row()))
                            : int64_t(uint32_t(TS(screen).item()));
    Store(UiNode(bar).y_address(), Scaled(at));
    if (int32_t(TS(screen).category()) != 2) {
      Hide27(true);
      for (uint32_t n = 5; int32_t(n - 5) < int32_t(uint32_t(TS(screen).item_count())); n++) {
        const uint32_t category = TS(screen).category();
        uint64_t id = 0;
        if (category == 0) {
          id = n + 183;
        } else if (category == 1) {
          id = n + 187;
        } else if (category == 3) {
          id = n + 192;
        } else if (category == 5) {
          id = n + 208;
        } else if (category == 6) {
          id = n + 230;
        } else if (category == 4) {
          const uint64_t r = n - 5;
          if (r >= 8) {
            continue;
          }
          Shown(27, n, 1);
          Text(27, n,
               uint64_t(uint32_t(TS(screen).item())) - TS(screen).list_scroll_row() +
                   r + 203,
               0x821D8EB0);
          uint32_t available = 1;
          if (int32_t(TS(screen).category()) == 4) {
            const uint32_t k = uint32_t(uint64_t(uint32_t(TS(screen).item())) -
                                        TS(screen).list_scroll_row() + r);
            available = Load((k + 6282) * 4 + screen);
          }
          if (available == 0) {
            Color(27, n, 2);
          }
          continue;
        } else {
          continue;
        }
        Shown(27, n, 1);
        Text(27, n, id, 0x821D8F54);
      }
    }
    if (int32_t(TS(screen).category()) != 4) {
      Animate27([&] { return uint64_t(uint32_t(TS(screen).item())) + 2; },
                {0x821D8F88, 0x821D8FAC, 0x821D9994});
      return;
    }
    if (TS(screen).aero9_type() != 0 && TS(screen).list_scroll_row() == 0) {
      return;
    }
    Animate27([&] { return uint64_t(TS(screen).list_scroll_row()) + 2; },
              {0x821D8FE8, 0x821D900C, 0x821D9994});
  }

  // State 3: the category list.
  void Categories() {
    if (!TimeIs(kAll, 0, kCategoriesAt) && !NamedIs(966, kAtEnd) && !NamedIs(967, kAtEnd) &&
        !NamedIs(973, kAtEnd) && int32_t(TS(screen).command_end_after_setup()) != 1) {
      return;
    }
    Store(UiNode(Node(27, 0)).y_address(), Scaled(int32_t(TS(screen).category())));
    Hide27(true);
    for (uint32_t n = 5; n < 12; n++) {
      Shown(27, n, 1);
      Text(27, n, n + 176, 0x821D911C);
    }
    Animate27([&] { return uint64_t(TS(screen).category()) + 2; },
              {0x821D913C, 0x821D9160, 0x821D9994});
  }

  // State 17: a setting list.
  void Settings() {
    if (!NamedIs(61, kAtEnd) && int32_t(TS(screen).command_end_now()) != 1) {
      return;
    }
    Store(UiNode(ChildOf(Node(1, 9), 0)).y_address(), Scaled(int32_t(TS(screen).size_cursor())));
    HideChildren(1, 9);
    const uint32_t first = ChildOf(Node(1, 9), 5);
    UiNode(first).set_visible(1);
    NodeText(first, 376, 0, 0x821D922C);
    for (uint32_t i = 1; int32_t(i) < int32_t(TS(screen).size_last() + 1); i++) {
      const uint32_t row = ChildOf(Node(1, 9), i + 5);
      UiNode(row).set_visible(1);
      NodeText(row, 454, i, 0x821D9270);
    }
    AnimateNamed([&] { return uint64_t(TS(screen).size_cursor()) + 20; },
                 {0x821D9294, 0x821D92B4, 0x821D92C4});
    if (int32_t(TS(screen).part_group()) == 11) {
      Restart71({0x821D92DC, 0x821D92F8});
    }
    TS(screen).set_command_end_now(0);
  }

  // States 57 and 58.
  void Options57() {
    if (!NamedIs(143, kAtEnd)) {
      return;
    }
    Store(UiNode(ChildOf(Node(2, 0), 0)).y_address(), Scaled(int32_t(TS(screen).interior_slot())));
    HideChildren(2, 0);
    const uint32_t node = Node(2, 0);
    struct Row {
      uint32_t flag;
      uint64_t id;
      uint64_t argument;
      uint32_t lr;
    };
    static constexpr Row kRows[] = {{25176, 247, 1, 0x821D9438},
                                    {25180, 247, 2, 0x821D94A8},
                                    {25184, 247, 3, 0x821D9518},
                                    {25188, 246, 1, 0x821D9588},
                                    {25192, 246, 2, 0x821D95F0}};
    for (uint32_t k = 0; k < 5; k++) {
      const uint32_t child = ChildOf(node, k + 5);
      UiNode(child).set_visible(1);
      uint32_t flag = 1;
      if (int32_t(TS(screen).category()) == 6 && uint32_t(TS(screen).item()) == 3 &&
          int32_t(TS(screen).interior_mode()) == 1) {
        flag = Word(kRows[k].flag);
      }
      if (flag == 0) {
        // sub_8211A630(movie, child, 2): r3 left (the movie).
        ui_movies::SetColor(child, 2);
        r3 = movie;
      }
      NodeText(child, kRows[k].id, kRows[k].argument, kRows[k].lr);
    }
    AnimateNamed([&] { return uint64_t(TS(screen).interior_slot()) + 74; },
                 {0x821D9600, 0x821D9620, 0x821D8B3C});
  }

  // State 56.
  void Options56() {
    if (!NamedIs(1005, kAtEnd) && !NamedIs(144, kOtherAt) &&
        int32_t(TS(screen).command_end_after_setup()) != 1) {
      return;
    }
    const uint32_t bar = ChildOf(Node(2, 0), 0);
    const uint32_t pick = TS(screen).interior_page_mode();
    Store(UiNode(bar).y_address(),
          int32_t(pick) == 4 ? steps::Bits(steps::Lfs(kFarRow)) : Scaled(int32_t(pick)));
    HideChildren(2, 0);
    const uint32_t node = Node(2, 0);
    static constexpr uint32_t kChildren[] = {5, 6, 7, 8, 10};
    static constexpr uint32_t kIds[] = {239, 240, 241, 242, 245};
    static constexpr uint32_t kLr[] = {0x821D9730, 0x821D9750, 0x821D9770, 0x821D9790,
                                       0x821D97B0};
    for (uint32_t k = 0; k < 5; k++) {
      const uint32_t child = ChildOf(node, kChildren[k]);
      UiNode(child).set_visible(1);
      NodeText(child, kIds[k], 0, kLr[k]);
    }
    if (int32_t(TS(screen).interior_page_mode()) == 4) {
      AnimateNamed([] { return uint64_t(79); }, {0x821D97C8, 0x821D97E4, 0x821D9824});
    } else {
      AnimateNamed([&] { return uint64_t(TS(screen).interior_page_mode()) + 74; },
                   {0x821D97F4, 0x821D9814, 0x821D9824});
    }
    if (TS(screen).command_end_after_setup() != 0) {
      const uint32_t now = TS(screen).interior_page_mode();
      TS(screen).set_command_end_after_setup(0);
      if (int32_t(now) == 4) {
        TS(screen).set_command_end_after_setup(1);
      }
    }
  }

  // State 53: two choices.
  void TwoChoices() {
    if (!NamedIs(1001, kAtEnd) && !NamedIs(1003, kAtEnd) && !NamedIs(1006, kOtherAt) &&
        int32_t(TS(screen).command_end_after_setup()) != 1) {
      return;
    }
    Hide27(false);
    Shown(27, 5, 1);
    Text(27, 5, 243, 0x821D98E4);
    Shown(27, 6, 1);
    Text(27, 6, 244, 0x821D9908);
    Store(UiNode(Node(27, 0)).y_address(), Scaled(int32_t(TS(screen).interior_mode())));
    Animate27([&] { return uint64_t(TS(screen).interior_mode()) + 2; },
              {0x821D995C, 0x821D9980, 0x821D9994});
  }
};

}  // namespace

uint64_t RefreshMenu(const GuestCall& call, uint32_t screen, uint32_t sp,
                     std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821D8978 TuningScreen::RefreshMenu");
  const uint32_t frame = Frame(sp, 240);
  const uint32_t entry = MovieEntry(frame, kMovieId);
  const int32_t state = int32_t(TS(screen).state());
  Menu menu{call, screen, frame, Load(entry), f1, entry};
  switch (state) {
    case 16: menu.Tabs(); break;
    case 15: menu.ThreeChoices(); break;
    case 4: menu.Items(); break;
    case 3: menu.Categories(); break;
    case 17: menu.Settings(); break;
    case 53: menu.TwoChoices(); break;
    case 56: menu.Options56(); break;
    case 57:
    case 58: menu.Options57(); break;
    default: break;
  }
  return menu.r3;
}

}  // namespace tuning_screen
