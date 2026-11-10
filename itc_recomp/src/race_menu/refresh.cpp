// The race menu's texts (docs/functions/race-menu.md, RaceMenu::Refresh):
// see race_menu.h.

#include "race_menu/race_menu.h"

#include "base/log.h"
#include "race_menu/internal.h"
#include "ui_movies/ui_movies.h"

namespace race_menu {

namespace {

constexpr uint64_t kSceneRoot = ~0ull;  // the sub-scene -1 (the scene's own children)

// The UI movie calls through their overrides from `frame`.
struct Movie {
  uint8_t* base;
  const Calls& calls;
  uint32_t frame;
  uint32_t m;

  void Show(uint64_t sub, uint64_t child, uint32_t lr) const {
    calls.Call(kShowNode, frame, lr, m, sub, child);
  }
  void Hide(uint64_t sub, uint64_t child, uint32_t lr) const {
    calls.Call(kHideNode, frame, lr, m, sub, child);
  }
  void Texture(uint64_t sub, uint64_t child, uint64_t texture, uint32_t lr) const {
    calls.Call(kSetNodeTexture, frame, lr, m, sub, child, texture);
  }
  // UiMovie::SetText; r7 given only where the call site sets it, the rest
  // 0 (the original's leftovers: the user's call, 2026-10-30).
  void Text(uint64_t sub, uint64_t child, uint64_t text, uint32_t lr, uint64_t r7 = 0) const {
    calls.CallArgs(kSetText, frame, lr, {m, sub, child, text, r7, 0, 0, 0});
  }
};

// A child's +80 (its shown flag) written.
void SetChildFlag(uint8_t* base, uint32_t node, uint32_t i, uint32_t value) {
  Store(base, ui_movies::Child(base, node, i) + 80, value);
}

// Two children's flags written in turn (the first, then the second).
void SetChildFlags(uint8_t* base, uint32_t node, uint32_t a, uint32_t a_value, uint32_t b,
                   uint32_t b_value) {
  SetChildFlag(base, node, a, a_value);
  SetChildFlag(base, node, b, b_value);
}

// The badges (11, 20) to (11, 23) hidden, then the pair's +4 (the cursor's
// in +9852) shown with its text; `gate_first` (page 4) shows (11, 20) only
// with the progress bit 0x20. `lr` the return addresses of: the four hides,
// then badge 0's two shows and two texts, badge 1's show and text, badge 2's
// show and text.
void Badges(const Movie& movie, uint32_t machine, bool gate_first, const uint32_t (&lr)[12]) {
  uint8_t* base = movie.base;
  for (uint32_t k = 0; k < 4; k++) {
    movie.Hide(11, 20 + k, lr[k]);
  }
  const uint32_t badge = Load(base, 8 * Load(base, machine + 9828) + 9856 + machine);
  if (badge == 0) {
    if (!gate_first || (Load(base, machine + 10148) & 0x20) != 0) {
      movie.Show(11, 20, lr[4]);
    }
    movie.Show(11, 21, lr[5]);
    movie.Text(11, 20, 402, lr[6]);
    movie.Text(11, 21, 401, lr[7]);
  } else if (badge == 1) {
    movie.Show(11, 22, lr[8]);
    movie.Text(11, 22, 407, lr[9]);
  } else if (badge == 2) {
    movie.Show(11, 23, lr[10]);
    movie.Text(11, 23, 408, lr[11]);
  }
}

// The cursor's event type (its +9852 pair's +0).
uint32_t CursorType(uint8_t* base, uint32_t machine) {
  return Load(base, 8 * Load(base, machine + 9828) + machine + 9852);
}

// Page 5: the event's node (10, type + 1) shown and its children by type
// and the variant (+9724).
void VariantChildren(const Movie& movie, uint32_t machine) {
  uint8_t* base = movie.base;
  movie.Show(10, uint64_t(CursorType(base, machine)) + 1, 0x8218BD54);
  const uint32_t node =
      ui_movies::FindNode(base, movie.m, 10, CursorType(base, machine) + 1);
  const uint32_t type = CursorType(base, machine);
  if (type > 8) {
    return;
  }
  const bool variant = Load(base, machine + 9724) != 0;
  switch (type) {
    case 0:
    case 2:
      if (variant) {
        SetChildFlags(base, node, 8, 0, 9, 1);
      } else {
        SetChildFlags(base, node, 8, 1, 9, 0);
      }
      break;
    case 1:
      if (variant) {
        SetChildFlags(base, node, 6, 0, 7, 1);
      } else {
        SetChildFlags(base, node, 6, 1, 7, 0);
      }
      break;
    case 3:
    case 7:
      SetChildFlag(base, node, 9, 1);
      break;
    case 4:
      SetChildFlag(base, node, 8, 1);
      break;
    case 5:
      if (!variant) {
        SetChildFlags(base, node, 8, 0, 9, 1);
      } else {
        SetChildFlags(base, node, 8, 1, 9, 0);
      }
      break;
    case 6:
      if (!variant) {
        SetChildFlags(base, node, 6, 0, 7, 1);
      } else {
        SetChildFlags(base, node, 6, 1, 7, 0);
      }
      break;
    case 8:
      SetChildFlag(base, node, 7, 1);
      break;
  }
}

// The race modes' textures (15, 4), (15, 5), (15, 3) = the mode (+9820).
void ModeTextures(const Movie& movie, uint32_t machine, const uint32_t (&lr)[3]) {
  uint8_t* base = movie.base;
  movie.Texture(15, 4, Load(base, machine + 9820), lr[0]);
  movie.Texture(15, 5, Load(base, machine + 9820), lr[1]);
  movie.Texture(15, 3, Load(base, machine + 9820), lr[2]);
}

// Page 4's list of event names (strings) in the frame from +96, by the
// progress bits; the words past the list are left as they are.
void EventNames(uint8_t* base, uint32_t machine, uint32_t frame) {
  const uint32_t bits = Load(base, machine + 10148);
  const uint32_t names = frame + 96;
  if ((bits & 0x20) != 0) {
    for (uint32_t i = 0; i < 15; i++) {
      Store(base, names + 4 * i, 360 + i);
    }
    return;
  }
  if ((bits & 0x4) != 0 || (bits & 0x8) != 0) {
    static constexpr uint32_t kAll[] = {360, 361, 362, 363, 364, 365, 366,
                                        367, 369, 370, 371, 372, 373};
    for (uint32_t i = 0; i < 13; i++) {
      Store(base, names + 4 * i, kAll[i]);
    }
    return;
  }
  Store(base, names + 0, 360);
  Store(base, names + 4, 361);
  Store(base, names + 8, 365);
  Store(base, names + 12, 366);
  if ((bits & 0x10) != 0) {
    Store(base, names + 16, 367);
    Store(base, names + 20, 369);
    Store(base, names + 24, 370);
    Store(base, names + 28, 373);
  } else {
    Store(base, names + 16, 369);
    Store(base, names + 20, 370);
  }
}

// The name at the list's row `row` from the top (+9828 - +9972 + row).
uint32_t RowName(uint8_t* base, uint32_t machine, uint32_t frame, uint32_t row) {
  const uint32_t top = Load(base, machine + 9828) - Load(base, machine + 9972);
  return Load(base, 4 * top + frame + 96 + 4 * row);
}

void EventPage(const Movie& movie, uint32_t machine) {
  uint8_t* base = movie.base;
  const uint32_t frame = movie.frame;
  ModeTextures(movie, machine, {0x8218BF80, 0x8218BF94, 0x8218BFA8});
  // The list's arrows: (-1, 17) hidden at its top, (-1, 18) at its bottom.
  const uint32_t top = Load(base, machine + 9828) - Load(base, machine + 9972);
  if (Load(base, machine + 9840) == top) {
    movie.Hide(kSceneRoot, 17, 0x8218BFD0);
    movie.Show(kSceneRoot, 18, 0x8218C010);
  } else if (Load(base, machine + 9844) == top + 2) {
    movie.Show(kSceneRoot, 17, 0x8218BFE8);
    movie.Hide(kSceneRoot, 18, 0x8218BFF8);
  } else {
    movie.Show(kSceneRoot, 17, 0x8218C000);
    movie.Show(kSceneRoot, 18, 0x8218C010);
  }
  EventNames(base, machine, frame);
  movie.Text(20, 5, RowName(base, machine, frame, 0), 0x8218C140);
  movie.Text(20, 6, RowName(base, machine, frame, 1), 0x8218C168);
  movie.Text(20, 7, RowName(base, machine, frame, 2), 0x8218C190);
  // The row scrolled in: above the list (1) or below it (2).
  const uint32_t scroll = Load(base, machine + 9836);
  if (scroll == 1 || scroll == 2) {
    const uint32_t cursor = Load(base, machine + 9828);
    uint32_t name;
    uint64_t child;
    if (scroll == 1) {
      const uint32_t last = Load(base, machine + 9844);
      uint32_t first = 0;
      if (last == cursor && (first = Load(base, machine + 9840)) == Load(base, machine + 9832)) {
        name = Load(base, 4 * first + frame + 96);
      } else {
        name = Load(base, 4 * (cursor - Load(base, machine + 9972)) + frame + 108);
      }
      child = 8;
    } else {
      const uint32_t first = Load(base, machine + 9840);
      uint32_t last = 0;
      if (first == cursor && (last = Load(base, machine + 9844)) == Load(base, machine + 9832)) {
        name = Load(base, 4 * last + frame + 96);
      } else {
        name = Load(base, 4 * (cursor - Load(base, machine + 9972) - 1) + frame + 96);
      }
      child = 15;
    }
    movie.Text(20, child, name, 0x8218C248);
    Store(base, machine + 9836, 0);
  }
  // The event's kind and its variant's.
  movie.Show(8, 5, 0x8218C25C);
  movie.Text(8, 5, uint64_t(Load(base, 8 * Load(base, machine + 9828) + machine + 9980)) + 395,
             0x8218C280);
  if (Load(base, 8 * Load(base, machine + 9828) + 9984 + machine) != kNone) {
    movie.Show(8, 6, 0x8218C2A8);
    movie.Text(8, 6,
               uint64_t(Load(base, 8 * Load(base, machine + 9828) + 9984 + machine)) + 395,
               0x8218C2CC);
  } else {
    movie.Hide(8, 6, 0x8218C2D4);
  }
  movie.Show(10, uint64_t(CursorType(base, machine)) + 1, 0x8218C2F4);
  // The events' nodes (10, 1) to (10, 15): two children's flags each
  // cleared, by the event (a jump table).
  for (uint32_t i = 0; i < 15; i++) {
    const uint32_t node = ui_movies::FindNode(base, movie.m, 10, i + 1);
    switch (i) {
      case 0:
      case 2:
      case 5:
        SetChildFlag(base, node, 2, 0);
        break;
      case 1:
      case 6:
        SetChildFlag(base, node, 3, 0);
        break;
      case 4:
        SetChildFlags(base, node, 2, 0, 9, 0);
        break;
      case 8:
      case 10:
        SetChildFlags(base, node, 3, 0, 6, 0);
        break;
      case 9:
      case 12:
      case 13:
        SetChildFlags(base, node, 3, 0, 7, 0);
        break;
      default:  // 3, 7, 11, 14
        SetChildFlags(base, node, 2, 0, 8, 0);
        break;
    }
  }
  movie.Hide(8, 0, 0x8218C3CC);
  Badges(movie, machine, true,
         {0x8218C3DC, 0x8218C3EC, 0x8218C3FC, 0x8218C40C, 0x8218C440, 0x8218C450, 0x8218C464,
          0x8218C4C0, 0x8218C488, 0x8218C4C0, 0x8218C4AC, 0x8218C4C0});
}

}  // namespace

uint64_t Refresh(uint8_t* base, uint64_t machine_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218BC18 RaceMenu::Refresh");
  const uint32_t machine = uint32_t(machine_r3);
  const uint32_t frame = Frame(base, sp, 224);
  const Movie movie{base, calls, frame, FindMovie(base, kMovie, frame + 80)};
  const uint32_t m = movie.m;
  calls.Call(kSetChildNameText, frame, 0x8218BC58, m, 0, 6, machine_r3 + 10112);
  calls.Call(kSetNumberText, frame, 0x8218BC6C, m, 0, 9, Load(base, machine + 10132));
  movie.Text(0, 13, 8, 0x8218BC84, Load(base, machine + 10136));
  movie.Texture(0, 11, Load(base, machine + 10140), 0x8218BC98);
  movie.Hide(2, 2, 0x8218BCA8);
  movie.Hide(2, 10, 0x8218BCB8);
  movie.Texture(2, 6, 1, 0x8218BCCC);
  movie.Texture(2, 7, 1, 0x8218BCE0);
  for (uint32_t i = 1; i <= 15; i++) {
    movie.Hide(10, i, 0x8218BCFC);
  }
  uint64_t text;
  switch (int32_t(Load(base, machine + kPage))) {
    case 3:
      ModeTextures(movie, machine, {0x8218C4DC, 0x8218C4F0, 0x8218C504});
      movie.Text(19, 5, 128, 0x8218C518);
      movie.Text(19, 6, 129, 0x8218C52C);
      movie.Text(19, 7, 130, 0x8218C540);
      text = uint64_t(Load(base, machine + 9820)) + 545;
      break;
    case 4:
      EventPage(movie, machine);
      text = 548;
      break;
    case 5:
      VariantChildren(movie, machine);
      Badges(movie, machine, false,
             {0x8218BE18, 0x8218BE28, 0x8218BE38, 0x8218BE48, 0x8218BE70, 0x8218BE80,
              0x8218BE94, 0x8218BF64, 0x8218BF2C, 0x8218BF64, 0x8218BF50, 0x8218BF64});
      text = 549;
      break;
    case 6:
      return 1;
    default:
      return 0;
  }
  movie.Text(1, 0, text, 0x8218C558);
  calls.Call(kScrollText, frame, 0x8218C56C, m, 1, 0, 1);
  return 1;
}

}  // namespace race_menu
