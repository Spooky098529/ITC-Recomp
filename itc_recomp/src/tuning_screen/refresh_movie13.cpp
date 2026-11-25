// The vinyl editor's movie 13 refreshed by state (TuningScreen::RefreshMovie13,
// docs/functions/tuning-screen.md): see tuning_screen.h. Movie 13's nodes
// through the UI movies' plain C++ (FindNode, Child, ShowNode's and
// HideNode's stores, SetNodeTexture), the lists' plain C++ (ShowItems,
// LayOutItems, ShowOptionMarks); the texts (SetText, SetNodeText,
// SetNumberText, SetChildNameText) and the scrolls (sub_8211C5F8,
// ScrollText) through their overrides from the original's frame with its
// return addresses, the varargs SetText and SetNodeText leave unset passed
// as 0 (the user's call, 2026-10-30).

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "schema/ui_movies_game.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace tuning_screen {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

namespace steps = camera_modes::steps;

constexpr uint32_t kMovie13 = 13;
constexpr uint32_t kSetNodeText = 0x8211C7E8;      // UiMovie::SetNodeText(movie, node, id, ...)
constexpr uint32_t kSetNumberText = 0x8211C740;    // UiMovie::SetNumberText(movie, sub, child, v)
constexpr uint32_t kSetChildNameText = 0x8211D930;  // (movie, sub, child, name record)
constexpr uint32_t kAll = 0xFFFFFFFFu;             // sub-scene -1 (FindNode's)

struct Movie13 {
  const GuestCall& call;
  uint32_t screen;
  uint32_t frame;
  uint32_t movie;
  std::optional<double>* f1;
  uint64_t r3;

  uint32_t Word(uint32_t offset) const { return Load(screen + offset); }

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
  // UiMovie::SetNodeTexture (sub_8211A708).
  void Texture(uint32_t sub_scene, uint32_t child, uint32_t texture) {
    r3 = ui_movies::SetNodeTexture(Node(sub_scene, child), texture);
  }
  void Call(uint32_t function, std::initializer_list<uint64_t> args, uint32_t lr) {
    r3 = call(function, args, frame, lr);
  }
  // SetText of the live scene's (sub-scene -1, all 64 bits) or a sub-scene's
  // child, `argument` its first vararg (0 where the original leaves it).
  void Text(uint64_t sub_scene, uint32_t child, uint64_t id, uint64_t argument, uint32_t lr) {
    Call(kSetText, {movie, sub_scene, child, id, argument, 0, 0, 0}, lr);
  }
  void NodeText(uint32_t node, uint64_t id, uint32_t lr) {
    Call(kSetNodeText, {movie, node, id, 0, 0, 0, 0, 0}, lr);
  }
  void ShowItems(uint32_t show) { r3 = tuning_screen::ShowItems(screen, show, frame); }

  // The text (2, 0)'s scroll reset, then string `id` (when given), then
  // scrolled.
  void Scroll(std::optional<uint64_t> id, const uint32_t (&lr)[3]) {
    Call(kResetTextScroll, {movie, 2, 0}, lr[0]);
    if (id) {
      Text(2, 0, *id, 0, lr[1]);
    }
    Call(kScrollText, {movie, 2, 0, 1}, lr[2]);
  }

  // The part that every state shares.
  void Common() {
    Call(kSetChildNameText, {movie, 0, 6, uint64_t(screen) + 20792}, 0x82207728);
    Call(kSetNumberText, {movie, 0, 9, TS(screen).money()}, 0x8220773C);
    Text(0, 13, 8, TS(screen).day(), 0x82207754);
    Texture(0, 11, TS(screen).weekday());
    Shown(1, 0, 1);
    Shown(1, 1, 1);
    // (1, 2)'s +68 = 0.0 (lfs, stfs).
    steps::Stfs(UiNode(Node(1, 2)).alpha_address(), steps::Lfs(0x82000A34));
    Shown(1, 10, 0);
    Texture(1, 4, 0);
    Texture(1, 5, 0);
    Texture(1, 6, 1);
    Texture(1, 7, 1);
    ShowItems(0);
    ShowItems(1);
    Text(kAllScenes, 39, 462, 0, 0x82207834);
    Text(kAllScenes, 41, 463, 0, 0x82207848);
    Text(kAllScenes, 43, 457, 0, 0x8220785C);
    Text(kAllScenes, 44, 458, 0, 0x82207870);
    Text(kAllScenes, 45, 459, 0, 0x82207884);
    Text(kAllScenes, 34, 8, TS(screen).edited_x(), 0x8220789C);
    Text(kAllScenes, 35, 8, TS(screen).edited_y(), 0x822078B4);
    // +30840 as an int (fctiwz; the original's low word through the frame's
    // +80, a value since E5 step 6).
    const uint32_t angle =
        uint32_t(steps::Fctiwz(steps::Lfs(TS(screen).edited_angle_address())));
    Text(kAllScenes, 36, 8, angle, 0x822078DC);
    // +30844 as a double in r7 (the original's through the frame's +48, a
    // value since E5 step 6), and in f1.
    const double value = steps::Lfs(TS(screen).edited_scale_address());
    const uint64_t bits = std::bit_cast<uint64_t>(value);
    *f1 = value;
    Text(kAllScenes, 37, 452, bits, 0x822078FC);
    Text(kAllScenes, 38, int32_t(TS(screen).edited_flip()) != 0 ? 0 : 1, 0, 0x82207920);
    if (int32_t(TS(screen).state()) != 28) {
      r3 = LayOutItems(screen, frame);
    }
    r3 = ShowOptionMarks(screen, 0, frame);
  }

  // State 34: the view.
  void View() {
    ShowItems(0);
    Shown(1, 0, 1);
    Shown(1, 1, 0);
    Shown(1, 10, 1);
    Texture(1, 4, 0);
    Texture(1, 5, 1);
    steps::Stfs(UiNode(Node(1, 3)).x_address(), steps::Lfs(0x82008650));
  }

  // State 28: a layer's settings, on sub-scene 26 (+30724 0) or 25 (1).
  void LayerSet() {
    const int32_t set = int32_t(TS(screen).editor_page());
    if (set == 0) {
      Text(26, 2, uint64_t(TS(screen).picked_image()) + 1442, 0, 0x82207A24);
      Text(26, 10, 1241, 0, 0x82207A38);
      Text(26, 7, 449, 0, 0x82207A4C);
      Text(26, 8, 8, uint64_t(TS(screen).picked_image()) + 1, 0x82207A68);
      Text(26, 9, 8, 5, 0x82207AF4);
    } else if (set == 1) {
      Text(25, 2, uint64_t(TS(screen).picked_image()) + 1248, 0, 0x82207A94);
      Text(25, 10, uint64_t(TS(screen).image_group()) + 1229, 0, 0x82207AAC);
      Text(25, 7, 449, 0, 0x82207AC0);
      Text(25, 8, 8, uint64_t(TS(screen).picked_image()) + 1, 0x82207ADC);
      Text(25, 9, 8, 194, 0x82207AF4);
    }
    Shown(kAll, 26, 0);
    Shown(kAll, 25, 0);
    const int32_t now = int32_t(TS(screen).editor_page());
    if (now == 0) {
      Shown(kAll, 26, 1);
    } else if (now == 1) {
      Shown(kAll, 25, 1);
    }
  }

  // State 26: a layer's edit menu.
  void LayerEdit() {
    {
      const uint32_t node = Node(11, 1);
      static constexpr uint32_t kIds[5] = {456, 457, 458, 459, 461};
      static constexpr uint32_t kLr[5] = {0x82207B80, 0x82207B9C, 0x82207BB8, 0x82207BD4,
                                          0x82207BF0};
      for (uint32_t i = 0; i < 5; i++) {
        NodeText(ChildOf(node, 5 + i), kIds[i], kLr[i]);
      }
    }
    {
      const uint32_t node = Node(11, 0);
      NodeText(ChildOf(node, 4), 4, 0x82207C1C);
      NodeText(ChildOf(node, 5), 5, 0x82207C38);
    }
    const uint32_t first = ChildOf(Node(11, 1), 0);
    const int32_t pick = int32_t(TS(screen).edit_cursor());
    UiNode(first).set_visible(pick >= 0 && pick <= 4 ? 1 : 0);
    // 816 not used.
    static constexpr uint32_t kHelp[7] = {812, 813, 814, 815, 817, 818, 819};
    const uint32_t which = TS(screen).edit_cursor();
    Scroll(which < 7 ? std::optional<uint64_t>(kHelp[which]) : std::nullopt,
           {0x82207C80, 0x82207CF4, 0x82207D08});
    Shown(1, 1, 0);
  }

  // State 25: the layer kinds.
  void Kinds() {
    static constexpr uint32_t kLr[4] = {0x82207D20, 0x82207D34, 0x82207D48, 0x82207D5C};
    for (uint32_t i = 0; i < 4; i++) {
      Text(10, 5 + i, 333 + i, 0, kLr[i]);
    }
    const uint32_t which = TS(screen).operation_cursor();
    Scroll(which < 4 ? std::optional<uint64_t>(806 + which) : std::nullopt,
           {0x82207D6C, 0x82207DB4, 0x82207DC8});
  }

  // State 24: the layer lists, their records' pointers in the movie's +28
  // table set from the screen.
  void Layers() {
    Shown(kAll, 28, 0);
    Shown(kAll, 27, 0);
    const int32_t set = int32_t(TS(screen).editor_page());
    if (set == 0 || set == 1) {
      Shown(kAll, set == 0 ? 28 : 27, 1);
      static constexpr uint32_t kSlots[2][3] = {{724, 732, 736}, {700, 708, 712}};
      static constexpr uint32_t kFrom[2][3] = {{10076, 10112, 10148}, {10832, 10868, 10904}};
      for (uint32_t i = 0; i < 3; i++) {
        const uint32_t value = Word(kFrom[set][i]);
        const uint32_t textures = Load(schema::UiMovie(movie).textures_address());
        Store(Load(textures + kSlots[set][i]), value);
      }
    }
    Text(9, 4, 4, 0, 0x82207E94);
    Text(9, 5, 5, 0, 0x82207EA8);
    Shown(1, 1, 0);
  }

  // State 23: the top menu.
  void Top() {
    for (uint32_t n = 5; n < 15; n++) {
      Shown(3, n, 0);
    }
    Shown(3, 5, 1);
    Text(3, 5, 276, 0, 0x82207F00);
    Shown(3, 6, 1);
    Text(3, 6, 274, 0, 0x82207F24);
    ShowItems(0);
  }
};

}  // namespace

uint64_t RefreshMovie13(const GuestCall& call, uint32_t screen, uint32_t sp,
                        std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_822076E8 TuningScreen::RefreshMovie13");
  const uint32_t frame = sp - 128;
  Store(frame, sp);  // the back chain, as stwu
  // The movie's id passed to UiMovies::Find as a value (the original's
  // frame +80; E5 step 6).
  const uint32_t entry = ui_movies::PairsFindKey(ui_movies::kMovieTable, kMovie13);
  Movie13 m{call, screen, frame, Load(entry), f1, entry};
  m.Common();
  switch (int32_t(TS(m.screen).state())) {
    case 23: m.Top(); break;
    case 24: m.Layers(); break;
    case 25: m.Kinds(); break;
    case 26: m.LayerEdit(); break;
    case 27:
      m.r3 = ShowOptionMarks(screen, 1, frame);
      m.ShowItems(0);
      break;
    case 28: m.LayerSet(); break;
    case 34: m.View(); break;
    default: break;
  }
  return m.r3;
}

}  // namespace tuning_screen
