// The refresh's pages never seen in play (docs/functions/tuning-screen.md:
// TuningScreen::RefreshPaint sub_821F2110, RefreshLightKits sub_821F2378,
// RefreshMovie14 sub_821F7A98, and since 2026-11-13 RefreshInterior
// sub_821F2AD8 with IsSlotAvailable's and PreviewInteriorChoices' plain
// C++):
// see tuning_screen.h. The nodes through the UI movies' plain C++
// (UiMovies::Find, FindNode, Child, HideNode's store, SetNodeTexture); the
// texts (SetText, SetNodeText) through their overrides from the original's
// frame with its return addresses, the varargs they leave unset passed as 0
// (the user's call, 2026-10-30). r3 followed through the calls as the
// originals leave it.

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "guest.h"
#include "tuning_pages/tuning_pages.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <initializer_list>
#include <optional>

namespace tuning_screen {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kSetNodeText = 0x8211C7E8;  // UiMovie::SetNodeText(movie, node, id, ...)

// A page over one movie from the original's frame.
struct Page {
  uint8_t* base;
  const GuestCall& call;
  uint32_t frame;
  uint32_t movie;
  uint64_t r3;

  uint32_t Node(uint32_t sub_scene, uint32_t child) {
    const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
    r3 = node;
    return node;
  }
  uint32_t ChildOf(uint32_t node, uint32_t i) {
    const uint32_t child = ui_movies::Child(base, node, i);
    r3 = child;
    return child;
  }
  // UiMovie::HideNode (sub_8211AC48) and ShowNode (sub_8211AC00).
  void Hide(uint32_t sub_scene, uint32_t child) { Store(base, Node(sub_scene, child) + 80, 0); }
  void Show(uint32_t sub_scene, uint32_t child) { Store(base, Node(sub_scene, child) + 80, 1); }
  // UiMovie::SetNodeTexture (sub_8211A708).
  void Texture(uint32_t sub_scene, uint32_t child, uint32_t texture) {
    r3 = ui_movies::SetNodeTexture(base, Node(sub_scene, child), texture);
  }
  void Text(uint64_t sub_scene, uint32_t child, uint64_t id, uint64_t argument, uint32_t lr) {
    r3 = call(kSetText, {movie, sub_scene, child, id, argument, 0, 0, 0}, frame, lr);
  }
  void NodeText(uint32_t node, uint64_t id, uint32_t lr, uint64_t argument = 0) {
    r3 = call(kSetNodeText, {movie, node, id, argument, 0, 0, 0, 0}, frame, lr);
  }
  // The help line (10, 0): its scroll reset (ResetHelp), then string `id`
  // (when given) and its scroll (Help).
  void ResetHelp(uint32_t lr) { r3 = call(kResetTextScroll, {movie, 10, 0}, frame, lr); }
  void Help(std::optional<uint64_t> id, uint32_t text_lr, uint32_t scroll_lr) {
    if (id) {
      Text(10, 0, *id, 0, text_lr);
    }
    r3 = call(kScrollText, {movie, 10, 0, 1}, frame, scroll_lr);
  }
};

// A frame of `bytes` below `sp` (its back chain, as stwu) and the movie
// `id`'s word (UiMovies::Find with the id at the frame's +80).
Page Open(uint8_t* base, const GuestCall& call, uint32_t sp, uint32_t bytes, uint32_t id) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  const uint32_t entry = MovieEntry(base, frame, id);
  return Page{base, call, frame, Load(base, entry), entry};
}

}  // namespace

uint64_t RefreshPaint(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F2110 TuningScreen::RefreshPaint");
  Page p = Open(base, call, sp, 112, kMovieId);
  if (int32_t(Load(base, screen + 9716)) != 0) {
    p.Texture(3, 1, 6);
    p.Texture(3, 2, 34);
  }
  p.Hide(8, 1);
  // Sub-scene 21's labels: children 2 to 4's children 8 and 9.
  static constexpr struct {
    uint32_t child, first, second;
    uint32_t lr[2];
  } kRows[3] = {{2, 221, 224, {0x821F21B4, 0x821F21D0}},
                {3, 222, 225, {0x821F21FC, 0x821F2218}},
                {4, 223, 226, {0x821F2244, 0x821F2260}}};
  for (const auto& row : kRows) {
    const uint32_t node = p.Node(21, row.child);
    p.NodeText(p.ChildOf(node, 8), row.first, row.lr[0]);
    p.NodeText(p.ChildOf(node, 9), row.second, row.lr[1]);
  }
  {
    const uint32_t node = p.Node(21, 5);
    p.NodeText(p.ChildOf(node, 6), 227, 0x821F228C);
    p.NodeText(p.ChildOf(p.ChildOf(node, 4), 0), 228, 0x821F22B0);
    p.NodeText(p.ChildOf(p.ChildOf(node, 5), 0), 229, 0x821F22D4);
  }
  {
    const uint32_t node = p.Node(21, 1);
    p.NodeText(p.ChildOf(node, 3), 4, 0x821F2300);
    p.NodeText(p.ChildOf(node, 4), 5, 0x821F231C);
  }
  Store(base, p.ChildOf(p.Node(21, 3), 4) + 104, 1);
  Store(base, p.ChildOf(p.Node(21, 4), 4) + 104, 1);
  return p.r3;
}

namespace {

// The light kits' eight swatches under `parent` (its children 4 to 11, each
// one's child 2): UiNode::SetRgb's stores, inline in the original.
void Swatches(Page& p, uint32_t parent) {
  static constexpr uint32_t kColors[8][3] = {{18, 173, 248},  {149, 45, 255}, {255, 137, 248},
                                             {255, 30, 30},   {255, 150, 30}, {255, 255, 30},
                                             {30, 255, 30},   {255, 255, 255}};
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t node = p.ChildOf(p.ChildOf(parent, 4 + k), 2);
    Store(p.base, node + 56, kColors[k][0]);
    Store(p.base, node + 60, kColors[k][1]);
    Store(p.base, node + 64, kColors[k][2]);
  }
}

}  // namespace

uint64_t RefreshLightKits(uint8_t* base, const StandCalls& stand, uint64_t r3, uint32_t sp,
                          std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821F2378 TuningScreen::RefreshLightKits");
  Page p = Open(base, stand.call, sp, 144, kMovieId);
  const uint32_t s = uint32_t(r3);
  const auto word = [&](uint32_t offset) { return Load(base, s + offset); };
  constexpr uint32_t kHelpScrollLr = 0x821F29E4, kHelpTextLr = 0x821F29D0;
  // The help line by `pick` (unsigned) from `ids`; past them the scroll
  // alone, at `last` the string by `flag` (0: 593, else 594) when given.
  const auto help = [&](uint32_t pick, std::initializer_list<uint32_t> ids,
                        std::optional<uint32_t> flag) {
    std::optional<uint64_t> id;
    if (pick < ids.size()) {
      id = *(ids.begin() + pick);
    } else if (flag && pick == ids.size()) {
      id = int32_t(word(*flag)) == 0 ? 593 : 594;
    }
    p.Help(id, kHelpTextLr, kHelpScrollLr);
  };
  switch (int32_t(word(kState))) {
    case 19: {
      if (int32_t(word(9716)) != 0) {
        p.Texture(3, 1, 6);
        p.Texture(3, 2, 35);
      }
      for (uint32_t i = 0; i < 10; i++) {
        p.Hide(36, 5 + i);
      }
      static constexpr uint32_t kStrings[4] = {273, 274, 275, 272};
      static constexpr uint32_t kLr[4] = {0x821F2A60, 0x821F2A84, 0x821F2AA8, 0x821F2ACC};
      for (uint32_t k = 0; k < 4; k++) {
        p.Show(36, 5 + k);
        p.Text(36, 5 + k, kStrings[k], 0, kLr[k]);
      }
      break;
    }
    case 20: {
      for (uint32_t i = 0; i < 10; i++) {
        p.Hide(37, 5 + i);
      }
      static constexpr uint32_t kStrings[3] = {230, 231, 245};
      static constexpr uint32_t kLr[3] = {0x821F293C, 0x821F2960, 0x821F2984};
      for (uint32_t k = 0; k < 3; k++) {
        p.Show(37, 5 + k);
        p.Text(37, 5 + k, kStrings[k], 0, kLr[k]);
      }
      p.ResetHelp(0x821F2994);
      help(word(30500), {795, 796, 595}, std::nullopt);
      break;
    }
    case 21: {
      p.NodeText(p.ChildOf(p.Node(23, 2), 5), 232, 0x821F2650);
      p.NodeText(p.ChildOf(p.Node(23, 3), 5), 233, 0x821F2678);
      p.NodeText(p.ChildOf(p.Node(23, 4), 5), 234, 0x821F26A0);
      p.NodeText(p.ChildOf(p.Node(23, 5), 12), 451, 0x821F26C8);
      {
        const uint32_t node = p.Node(23, 1);
        p.NodeText(p.ChildOf(node, 3), 4, 0x821F26F4);
        p.NodeText(p.ChildOf(node, 4), 5, 0x821F2710);
      }
      p.Hide(8, 1);
      p.r3 = PreviewLightKit(base, stand, r3, p.frame, f1);
      Swatches(p, p.Node(23, 5));
      p.ResetHelp(0x821F288C);
      help(word(30508), {797, 798, 799, 800}, 30528u);
      break;
    }
    case 22: {
      p.Text(24, 13, 455, 0, 0x821F23DC);
      p.NodeText(p.ChildOf(p.Node(24, 3), 5), 234, 0x821F2404);
      p.NodeText(p.ChildOf(p.Node(24, 4), 12), 451, 0x821F242C);
      {
        const uint32_t node = p.Node(24, 1);
        p.NodeText(p.ChildOf(node, 3), 4, 0x821F2458);
        p.NodeText(p.ChildOf(node, 4), 5, 0x821F2474);
      }
      p.Hide(8, 1);
      p.r3 = PreviewLightKit(base, stand, r3, p.frame, f1);
      Swatches(p, p.Node(24, 4));
      p.ResetHelp(0x821F25EC);
      help(word(30544), {801, 802, 803}, 30560u);
      break;
    }
    default: break;
  }
  return p.r3;
}

uint64_t RefreshMovie14(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F7A98 TuningScreen::RefreshMovie14");
  Page p = Open(base, call, sp, 112, 14);
  p.Text(0, 9, 9, Load(base, screen + 25116), 0x821F7AE4);
  p.Text(0, 13, 8, Load(base, screen + 25092), 0x821F7AFC);
  p.Texture(0, 11, Load(base, screen + 25096));
  p.Hide(1, 2);
  p.Hide(1, 10);
  p.Hide(1, 8);
  p.Hide(1, 9);
  p.Texture(1, 6, 1);
  p.Texture(1, 7, 1);
  return p.r3;
}

uint64_t RefreshInterior(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F2AD8 TuningScreen::RefreshInterior");
  Page p = Open(base, call, sp, 176, kMovieId);
  const auto word = [&](uint32_t offset) { return Load(base, screen + offset); };
  constexpr uint32_t kMode = 33564, kKind = 33548, kPair = 33572, kSlot = 33576;
  constexpr uint32_t kAll = 0xFFFFFFFFu;  // sub-scene -1: the scene itself
  // The help lines' last SetText and ScrollText return addresses (the
  // shared tail at 0x821F329C).
  constexpr uint32_t kTextLr = 0x821F32AC, kScrollLr = 0x821F32C0;
  switch (int32_t(word(kState))) {
    case 57:
    case 58: {
      p.Hide(kAll, 19);
      if (int32_t(word(kMode)) == 4) {
        p.Hide(2, 2);
      }
      for (uint32_t i = 0; i < 5; i++) {
        p.Hide(2, 4 + i);
      }
      p.Show(2, word(kSlot) + 4);
      const uint32_t available = tuning_pages::IsSlotAvailable(base, screen, word(kSlot));
      p.r3 = available;
      const uint32_t mode = word(kMode);
      std::optional<uint64_t> id;
      if (available == 0) {
        p.ResetHelp(int32_t(mode) == 4 ? 0x821F2BE0 : 0x821F2BD4);
        id = int32_t(mode) == 4 ? 864 : 859;
      } else if (int32_t(mode) == 4 &&
                 int32_t(Load(base, ((word(kSlot) + 8450) << 2) + screen)) == -1) {
        p.ResetHelp(0x821F2C14);
        id = 863;
      } else if (mode <= 3) {
        // 853 to 856 without the reset or the scroll.
        p.Text(10, 0, 853 + mode, 0, 0x821F2CA4);
      } else if (mode == 4) {
        p.ResetHelp(0x821F2C4C);
        id = 595;
      }
      if (id) {
        p.Text(10, 0, *id, 0, 0x821F2C60);
        p.r3 = call(kScrollText, {p.movie, 10, 0, 1}, p.frame, 0x821F2C74);
      }
      if (int32_t(word(kState)) == 57) {
        p.r3 = PreviewInteriorChoices(base, call, screen, p.frame);
      }
      break;
    }
    case 56: {
      if (int32_t(word(kMode)) == 4) {
        p.Hide(2, 2);
      } else {
        p.Show(2, 2);
      }
      if (int32_t(word(9716)) != 0) {
        p.Texture(3, 1, 41);
        p.Texture(3, 2, 43);
      }
      p.Texture(3, 2, 43);
      if (int32_t(word(kMode)) == 4) {
        p.Hide(kAll, 17);
      } else {
        p.Show(kAll, 17);
      }
      const uint32_t picture = p.ChildOf(p.Node(kAll, 17), 0);
      const int32_t pair = int32_t(word(kPair));
      Store(base, picture + 108, pair >= 0 && pair <= 2 ? 10 : 27);
      p.ResetHelp(0x821F2DA4);
      static constexpr uint32_t kIds[5] = {855, 853, 854, 856, 595};
      const uint32_t mode = word(kMode);
      p.Help(mode <= 4 ? std::optional<uint64_t>(kIds[mode]) : std::nullopt, kTextLr, kScrollLr);
      break;
    }
    case 55: {
      for (uint32_t i = 1; i <= 6; i++) {
        Store(base, p.ChildOf(p.Node(32, 3), i) + 80, 0);
      }
      // Children 1 to 6 shown, each one's child 2 colored by the kind being
      // 1 or not (the stores in the original's order).
      const uint32_t node = p.Node(32, 3);
      const auto shown = [&](uint32_t i) {
        const uint32_t child = p.ChildOf(node, i);
        const uint32_t color = p.ChildOf(child, 2);
        Store(base, child + 80, 1);
        return color;
      };
      const auto kind1 = [&] { return int32_t(word(kKind)) == 1; };
      uint32_t c = shown(1);
      Store(base, c + 56, 255);
      Store(base, c + 60, 255);
      Store(base, c + 64, 255);
      c = shown(2);
      Store(base, c + 60, 30);
      Store(base, c + 64, 30);
      Store(base, c + 56, kind1() ? 255 : 240);
      c = shown(3);
      Store(base, c + 56, 30);
      if (kind1()) {
        Store(base, c + 60, 255);
        Store(base, c + 64, 255);
      } else {
        Store(base, c + 60, 30);
        Store(base, c + 64, 240);
      }
      c = shown(4);
      Store(base, c + 64, 30);
      if (kind1()) {
        Store(base, c + 56, 30);
        Store(base, c + 60, 220);
      } else {
        Store(base, c + 56, 240);
        Store(base, c + 60, 240);
      }
      c = shown(5);
      if (kind1()) {
        Store(base, c + 56, 240);
        Store(base, c + 60, 220);
        Store(base, c + 64, 30);
      } else {
        Store(base, c + 60, 200);
        Store(base, c + 64, 0);
        Store(base, c + 56, 50);
      }
      c = shown(6);
      Store(base, c + 56, 255);
      Store(base, c + 64, 200);
      Store(base, c + 60, 140);
      p.r3 = c;
      break;
    }
    case 54: {
      if (int32_t(word(9716)) != 0) {
        p.Texture(3, 1, 41);
        p.Texture(3, 2, 42);
      }
      p.Texture(3, 2, 42);
      for (uint32_t i = 0; i < 10; i++) {
        Store(base, p.ChildOf(p.Node(28, 1), 5 + i) + 80, 0);
      }
      {
        const uint32_t child = p.ChildOf(p.Node(28, 1), 5);
        Store(base, child + 80, 1);
        p.NodeText(child, 375, 0x821F30DC);
      }
      for (uint32_t row = 1; row < 3; row++) {
        const uint32_t child = p.ChildOf(p.Node(28, 1), 5 + row);
        Store(base, child + 80, 1);
        p.NodeText(child, 379, 0x821F3110, row);
      }
      for (uint32_t i = 26; i < 36; i++) {
        const uint32_t node = p.Node(28, 0);
        Store(base, p.ChildOf(node, i - 11) + 80, 0);
        Store(base, p.ChildOf(node, i) + 80, 0);
      }
      for (uint32_t row = 0; row < 3; row++) {
        const uint32_t node = p.Node(28, 0);
        const uint32_t shown = p.ChildOf(node, row + 15);
        const uint32_t mark = p.ChildOf(node, (row + 19) * 2);
        Store(base, shown + 80, 1);
        const uint32_t flag = GetChoiceFlag(base, screen, row == 2 ? 3 : row);
        Store(base, mark + 108, flag != 0 ? 0 : 1);
        Store(base, p.ChildOf(node, row + 26) + 80, 1);
      }
      p.ResetHelp(0x821F31F8);
      static constexpr uint32_t kIds[4] = {595, 850, 851, 852};
      const uint32_t kind = word(kKind);
      p.Help(kind <= 3 ? std::optional<uint64_t>(kIds[kind]) : std::nullopt, kTextLr, kScrollLr);
      break;
    }
    case 53: {
      uint32_t texture = 7;
      if (int32_t(word(9716)) == 0) {
        texture = int32_t(word(33540)) == 0 ? 42 : 43;
      }
      p.Texture(3, 1, texture);
      p.Texture(3, 2, 41);
      p.ResetHelp(0x821F3298);
      p.Help(648, kTextLr, kScrollLr);
      break;
    }
    default: break;
  }
  return p.r3;
}

}  // namespace tuning_screen
