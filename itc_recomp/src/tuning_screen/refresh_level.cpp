// TuningScreen::Refresh's state 5, LEVEL_SELECT (sub_821E3AD0's block 4,
// docs/functions/tuning-screen.md's "The refresh"; refresh.h): the textures
// (+9716 set only), the level list's rows in sub-scene 28 (categories 0 to
// 2) or 30 (category 3; nothing for the others), the current choice's marker
// and the rows' marks (GetCurrentChoice, GetChoiceFlag), the scroll arrows
// and node 20, and the help line by the category, item and level. The nodes through the UI movies' plain C++ (FindNode, Child,
// SetColor, ShowNode's and HideNode's stores); the texts (SetNodeText) through
// its override from the original's frame with its return addresses, r6 and
// r10 passed where the original sets them (r10 its +25100 left from the row's
// arithmetic), the others 0 (the user's call, 2026-10-30).

#include "tuning_screen/refresh.h"

#include "camera_modes/steps.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace tuning_screen::refresh {

namespace {

namespace steps = camera_modes::steps;

constexpr uint32_t kRows = 9677;         // a byte: the level list's rows
constexpr uint32_t kFirstRow = 25100;    // the list's first level shown
constexpr uint32_t kMarkerScale = 0x82001FB0;  // a float: the marker's step

}  // namespace

void Context::NodeText(uint32_t node, uint64_t id, uint32_t lr, uint64_t r6,
                       uint64_t r10) const {
  Call(kSetNodeText, {movie, node, id, r6, 0, 0, 0, r10}, lr);
}

// Block 4: state 5, the item's levels.
void Context::State5() {
  // The level relative to the list's first row, all 64 bits (lbz - lwz).
  const auto relative = [&] { return uint64_t(Byte(kLevel)) - uint64_t(Word(kFirstRow)); };

  if (Word(9716) != 0) {
    const uint32_t category = Word(kCategory);
    Texture(3, 1, category == 2 ? 0 : uint64_t(category) + 1);
    // By the category (unsigned): none for 7 and above.
    static constexpr int32_t kFirst[7] = {8, 12, -1, 17, 20, 30, 38};
    const uint32_t again = Word(kCategory);
    if (again < 7) {
      Texture(3, 2, again == 2 ? 3 : uint64_t(Item()) + uint64_t(kFirst[again]));
    }
  }

  // The list's rows (children 5 to 14 of sub-scene 28's or 30's child 1)
  // hidden and colored 0.
  for (uint32_t n = 5; n < 15; n++) {
    const int32_t category = Category();
    if (category == 0 || category == 1 || category == 2) {
      const uint32_t node = ChildOf(Node(28, 1), n);
      guest::StoreU32(base, node + 80, 0);
      ui_movies::SetColor(base, node, 0);
    }
    if (Category() == 3) {
      const uint32_t node = ChildOf(Node(30, 1), n);
      guest::StoreU32(base, node + 80, 0);
      ui_movies::SetColor(base, node, 0);
    }
  }

  // A row shown, colored 2 when IsItemAvailable answers 0 for `item`.
  const auto show_row = [&](uint32_t sub_scene, uint32_t child, auto item) {
    const uint32_t node = ChildOf(Node(sub_scene, 1), child);
    guest::StoreU32(base, node + 80, 1);
    if (IsItemAvailable(base, screen, item()) == 0) {
      ui_movies::SetColor(base, node, 2);
    }
    return node;
  };
  // The lists of byte +9677 rows: the first (string 376), the middle ones
  // (378 with the row), the last at child `last` (377), with the original's
  // return addresses for the first and middle texts.
  const auto list = [&](uint32_t sub_scene, uint32_t last, uint32_t first_lr, uint32_t row_lr) {
    NodeText(show_row(sub_scene, 5, [] { return uint64_t(0); }), 376, first_lr);
    for (uint32_t row = 1; int32_t(row) < int32_t(Byte(kRows)) - 1; row++) {
      const uint32_t node = show_row(sub_scene, row + 5, [&] { return uint64_t(row); });
      NodeText(node, 378, row_lr, row);
    }
    const uint32_t node =
        show_row(sub_scene, last, [&] { return uint64_t(Byte(kRows)) - 1; });
    NodeText(node, 377, 0x821E49A0);
  };

  const int32_t category = Category();
  const auto item_is = [&](uint32_t a, uint32_t b) {
    const uint32_t item = Item();
    return item >= a && item <= b;
  };
  if (category == 0 && Item() == 0) {
    // The power parts' eight levels from +25100: 376 for the level 0, 377
    // for 9, else 378, with the level.
    for (uint32_t row = 0; row < 8; row++) {
      const uint32_t node = show_row(28, row + 5, [&] { return relative() + row; });
      const uint64_t r10 = Word(kFirstRow);
      const uint64_t level = uint64_t(Byte(kLevel)) - r10 + row;
      if (int32_t(level) == 0) {
        NodeText(node, 376, 0x821E4404, level, r10);
      } else if (int32_t(level) == 9) {
        NodeText(node, 377, 0x821E4404, level, r10);
      } else {
        NodeText(node, 378, 0x821E4410, level, r10);
      }
    }
  } else if (category == 0 && Item() == 1) {
    list(28, 11, 0x821E4488, 0x821E44F4);
  } else if (category == 0 && item_is(2, 3)) {
    list(28, 11, 0x821E4590, 0x821E45FC);
  } else if ((category == 1 && item_is(0, 3)) || category == 2) {
    list(28, 11, 0x821E46C0, 0x821E472C);
  } else if (category == 1 && Item() == 4) {
    // The aero-like eight rows from +25100, named through the 8-bit table at
    // 0x8200A8F8 by the level (0 to 9; none above).
    for (uint32_t row = 0; row < 8; row++) {
      const uint32_t node = show_row(28, row + 5, [&] { return relative() + row; });
      const uint64_t r10 = Word(kFirstRow);
      const uint64_t level = uint64_t(Byte(kLevel)) - r10 + row;
      constexpr uint32_t kSet = 0x821E4808, kUnset = 0x821E485C;
      switch (uint32_t(level)) {
        case 0: NodeText(node, 376, kUnset, 0, r10); break;
        case 1: NodeText(node, 383, kSet, 1, r10); break;
        case 2: NodeText(node, 383, kSet, 2, r10); break;
        case 3: NodeText(node, 384, kUnset, 0, r10); break;
        case 4: NodeText(node, 385, kSet, 1, r10); break;
        case 5: NodeText(node, 385, kSet, 2, r10); break;
        case 6: NodeText(node, 386, kUnset, 0, r10); break;
        case 7: NodeText(node, 387, kSet, 1, r10); break;
        case 8: NodeText(node, 387, kSet, 2, r10); break;
        case 9: NodeText(node, 388, kUnset, 0, r10); break;
        default: break;
      }
    }
  } else if (category == 3) {
    list(30, 9, 0x821E48C8, 0x821E4934);
  }

  // The marks (children 15 to 24 and 26 to 35 of sub-scene 28's or 30's
  // child 0) hidden.
  for (uint32_t n = 26; n < 36; n++) {
    const int32_t c = Category();
    uint32_t sub_scene;
    if (c == 0 || c == 1 || c == 2) {
      sub_scene = 28;
    } else if (c == 3) {
      sub_scene = 30;
    } else {
      continue;
    }
    const uint32_t node = Node(sub_scene, 0);
    guest::StoreU32(base, ChildOf(node, n - 11) + 80, 0);
    guest::StoreU32(base, ChildOf(node, n) + 80, 0);
  }

  // A row's marks: child 15 + row shown, child 38 + 2 row's +108 0 when
  // GetChoiceFlag answers for `choice`, else 1, child 26 + row shown.
  const auto marks = [&](uint32_t sub_scene, uint32_t row, auto choice) {
    const uint32_t node = Node(sub_scene, 0);
    const uint32_t shown = ChildOf(node, row + 15);
    const uint32_t flag = ChildOf(node, 38 + 2 * row);
    guest::StoreU32(base, shown + 80, 1);
    guest::StoreU32(base, flag + 108, GetChoiceFlag(base, screen, choice()) != 0 ? 0 : 1);
    guest::StoreU32(base, ChildOf(node, row + 26) + 80, 1);
  };
  // The eight levels from +25100: the current choice's marker (child 2 of
  // sub-scene 28's child 0) placed by its row and shown on the first eight.
  const auto eight = [&] {
    const uint32_t marker = ChildOf(Node(28, 0), 2);
    {
      const uint64_t choice = GetCurrentChoice(base, screen);
      const uint64_t back = uint64_t(Word(kFirstRow)) - uint64_t(Byte(kLevel));
      const int64_t row = int32_t(uint32_t(back + choice));
      guest::StoreU32(base, frame + 80, uint32_t(uint64_t(row) >> 32));
      guest::StoreU32(base, frame + 84, uint32_t(row));
      const double at =
          steps::Fmuls(steps::Single(double(row)), steps::Lfs(base, kMarkerScale));
      steps::Stfs(base, marker + 16, at);
    }
    {
      const uint64_t choice = GetCurrentChoice(base, screen);
      const uint64_t row =
          uint64_t(Word(kFirstRow)) - uint64_t(Byte(kLevel)) + choice;
      guest::StoreU32(base, marker + 80, int32_t(uint32_t(row)) > 7 ? 0 : 1);
    }
    for (uint32_t row = 0; row < 8; row++) {
      marks(28, row, [&] { return relative() + row; });
    }
  };
  const auto rows = [&](uint32_t sub_scene) {
    for (uint32_t row = 0; row < Byte(kRows); row++) {
      marks(sub_scene, row, [&] { return uint64_t(row); });
    }
  };
  {
    const int32_t c = Category();
    if (c == 0 && Item() == 0) {
      eight();
    } else if (c == 0 && item_is(1, 3)) {
      rows(28);
    } else if ((c == 1 && item_is(0, 3)) || c == 2) {
      rows(28);
    } else if (c == 1 && Item() == 4) {
      eight();
    } else if (c == 3) {
      rows(30);
    }
  }

  // The scroll arrows by the rows and the level's place from +25100.
  {
    const uint32_t count = Byte(kRows);
    if (count <= 8) {
      Shown(kAll, 25, 0);
      Shown(kAll, 26, 0);
    } else {
      const uint64_t d = relative();
      if (int32_t(d) == 0) {
        Shown(kAll, 25, 1);
        Shown(kAll, 26, 0);
      } else {
        Shown(kAll, 25, int32_t(count - 1) == int32_t(uint32_t(d + 7)) ? 0 : 1);
        Shown(kAll, 26, 1);
      }
    }
  }
  // Node 20 for the power parts' first three items.
  Shown(kAll, 20, Category() == 0 && Item() <= 2 ? 1 : 0);

  // The help line by the category, the item and the level (the level byte
  // IsItemAvailable leaves in r4), 857 for a level not available.
  ResetHelp(0x821E4F84);
  const uint64_t level = Byte(kLevel);
  if (IsItemAvailable(base, screen, level) == 0) {
    Help(857);
    return;
  }
  const int32_t c = Category();
  if (c == 0) {
    const uint32_t item = Item();
    if (item == 0) {
      switch (int32_t(Word(kPartSet))) {
        case 0: Help(level + 649); break;
        case 1: Help(level + 664); break;
        case 2: Help(level + 679); break;
        case 3: Help(level + 689); break;
        case 4: Help(uint32_t(level) < 5 ? level + 649 : level + 654); break;
        default: HelpScroll(); break;
      }
    } else if (item == 1) {
      switch (int32_t(Word(kPartSet))) {
        case 0: Help(level + 704); break;
        case 1: Help(level + 711); break;
        default: HelpScroll(); break;
      }
    } else if (item == 2) {
      Help(level + 721);
    } else if (item == 3) {
      Help(729);
    } else {
      HelpScroll();
    }
  } else if (c == 1) {
    static constexpr uint32_t kFirst[5] = {730, 737, 744, 751, 759};
    const uint32_t item = Item();
    if (item < 5) {
      Help(level + kFirst[item]);
    } else {
      HelpScroll();
    }
  } else if (c == 2) {
    Help(level + 769);
  } else if (c == 3) {
    Help(level + 776);
  } else {
    HelpScroll();
  }
}

}  // namespace tuning_screen::refresh
