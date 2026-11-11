// TuningScreen::Refresh's states 7 and 8 (sub_821E3AD0's blocks 6 and 7,
// docs/functions/tuning-screen.md's "The refresh"; refresh.h).
//
// State 7, the bonnet's and colors' choice: sub-scene 30's list for the
// bonnet (category 4 item 4: rows 376 and 380, the byte +9679 rows' marks by
// GetChoiceFlag), sub-scene 32's swatches for category 1 item 3 (its child
// 2's children 2 to 5) and category 6 item 0 (its child 3's children 1 to 6
// hidden, then 1 to 4 shown by the type +9662 1 to 4) colored with fixed RGB
// values, node 17 (the
// bonnet's AeroPartValue for the type - 1; the interior's shown for a type),
// ShowInteriorPicture(s, +9662, +9663) and the help line (758, 847).
//
// State 8, the option select: the textures 6 and 31 for the door (category 5
// item 1, +9716 set), the option list's rows (sub-scene 31's own children for
// the bonnet: 381 and 382; sub-scene 33's child 1's for the tires: 200 to
// 202; sub-scene 28's child 1's for the door: 376 and 394), the marks over
// the byte +9680 rows (the tires and the door) by GetChoiceFlag, the door's
// GarageStage::Command(holder, 4), node 17 (the bonnet's AeroPartValue) and
// the help line by the byte +9664 (781 + it for the tires, 790 + it for the
// door).
//
// The nodes through the UI movies' plain C++; the texts (SetNodeText)
// through its override from the original's frame with its return addresses,
// r6 and r10 passed where the original's own code sets them (state 8's door
// rows: r6 31, left from the second texture call; the bonnet's rows in both
// states: r10 the item byte, left from its category test), the others 0 (the
// user's call, 2026-10-30). The swatches' color setter sub_821E7DA0 (never
// seen in play: a leaf storing r5, r6 and r7 into the node's +56, +60 and
// +64) stays recompiled, called through the guest with r3 the screen (all 64
// bits), r4 the node, r5 to r7 the color and the original's return
// addresses; it takes no floats. GarageStage::Command by its plain C++, as
// UpdateStand and PickCameraPreset call it (f1 left as its adapter leaves
// it: see State8).

#include "tuning_screen/refresh.h"

#include "front_end_car_draw/front_end_car_draw.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace tuning_screen::refresh {

namespace {

constexpr uint32_t kChoiceRows = 9679;  // a byte: the bonnet's list's rows (state 7)
constexpr uint32_t kOptionRows = 9680;  // a byte: the option list's rows (state 8)
constexpr uint32_t kSetSwatch = 0x821E7DA0;  // (screen, node, r, g, b): +56, +60, +64
constexpr uint32_t kHolderAt = 0x826D83E0;   // [[it]]: the stand's holder

}  // namespace

// Block 6: state 7, the bonnet's and colors' choice.
void Context::State7() {
  const auto bonnet = [&] { return Category() == 4 && Item() == 4; };
  // The bonnet's rows (sub-scene 30's child 1's children 5 to 14) hidden.
  for (uint32_t row = 0; row < 10; row++) {
    if (bonnet()) {
      guest::StoreU32(base, ChildOf(Node(30, 1), row + 5) + 80, 0);
    }
  }
  // The rows' texts: the bonnet's two (r10 the item byte, left from the
  // category test), category 1 item 3's one (sub-scene 32's child 2's child
  // 1).
  if (bonnet()) {
    const uint32_t r10 = Item();
    const uint32_t parent = Node(30, 1);
    const uint32_t first = ChildOf(parent, 5);
    guest::StoreU32(base, first + 80, 1);
    NodeText(first, 376, 0x821E5D08, 0, r10);
    const uint32_t second = ChildOf(parent, 6);
    guest::StoreU32(base, second + 80, 1);
    NodeText(second, 380, 0x821E5D5C, 0, r10);
  } else if (Category() == 1 && Item() == 3) {
    const uint32_t node = ChildOf(Node(32, 2), 1);
    guest::StoreU32(base, node + 80, 1);
    NodeText(node, 376, 0x821E5D5C);
  }

  // The bonnet's marks (sub-scene 30's child 0's children 15 to 24 and 26 to
  // 35) hidden, then each of the +9679 rows' shown (38 + 2 row's +108 0 when
  // GetChoiceFlag answers for the row, else 1).
  for (uint32_t n = 26; n < 36; n++) {
    if (bonnet()) {
      const uint32_t node = Node(30, 0);
      guest::StoreU32(base, ChildOf(node, n - 11) + 80, 0);
      guest::StoreU32(base, ChildOf(node, n) + 80, 0);
    }
  }
  if (bonnet() && Byte(kChoiceRows) != 0) {
    for (uint32_t row = 0; int32_t(row) < int32_t(Byte(kChoiceRows)); row++) {
      const uint32_t node = Node(30, 0);
      const uint32_t mark = ChildOf(node, row + 15);
      const uint32_t flag = ChildOf(node, 38 + 2 * row);
      guest::StoreU32(base, mark + 80, 1);
      guest::StoreU32(base, flag + 108, GetChoiceFlag(base, screen, row) != 0 ? 0 : 1);
      guest::StoreU32(base, ChildOf(node, row + 26) + 80, 1);
    }
  }

  // A swatch: the node's color through the guest (sub_821E7DA0).
  const auto swatch = [&](uint32_t node, uint64_t r, uint64_t g, uint64_t b, uint32_t lr) {
    Call(kSetSwatch, {self, node, r, g, b}, lr);
  };
  if (Category() == 1 && Item() == 3) {
    // Sub-scene 32's child 2: its children 2 to 5's child 2.
    const uint32_t parent = Node(32, 2);
    swatch(ChildOf(ChildOf(parent, 2), 2), 0, 0, 0, 0x821E5EB8);
    swatch(ChildOf(ChildOf(parent, 3), 2), 240, 30, 30, 0x821E5EE4);
    swatch(ChildOf(ChildOf(parent, 4), 2), 30, 30, 240, 0x821E5F10);
    swatch(ChildOf(ChildOf(parent, 5), 2), 255, 220, 30, 0x821E62A0);
  } else if (Category() == 6 && Item() == 0) {
    // Sub-scene 32's child 3: its children 1 to 6 hidden (the loop counts
    // before its test), then by the type 1 to 4 the first ones shown with
    // their child 2's color.
    for (uint32_t k = 1; k <= 6; k++) {
      guest::StoreU32(base, ChildOf(Node(32, 3), k) + 80, 0);
    }
    struct Swatch {
      uint32_t r, g, b, lr;
    };
    const auto colors = [&](std::initializer_list<Swatch> list) {
      const uint32_t parent = Node(32, 3);
      uint32_t k = 1;
      for (const Swatch& s : list) {
        const uint32_t row = ChildOf(parent, k);
        const uint32_t shown = ChildOf(parent, k);
        const uint32_t node = ChildOf(row, 2);
        guest::StoreU32(base, shown + 80, 1);
        swatch(node, s.r, s.g, s.b, s.lr);
        k++;
      }
    };
    switch (Byte(kType)) {
      case 1:
      case 2:
        colors({{170, 170, 170, 0x821E5FD8},
                {0, 0, 0, 0x821E6020},
                {240, 30, 30, 0x821E6068},
                {30, 30, 240, 0x821E62A0}});
        break;
      case 3:
        colors({{0, 0, 0, 0x821E610C}, {240, 30, 30, 0x821E6154}, {30, 30, 240, 0x821E62A0}});
        break;
      case 4:
        colors({{0, 0, 0, 0x821E61C8},
                {216, 0, 150, 0x821E6210},
                {42, 0, 255, 0x821E6258},
                {170, 170, 170, 0x821E62A0}});
        break;
      default: break;
    }
  }

  // Node 17: the bonnet's part picture (its child 0's +108 AeroPartValue's
  // for the type - 1, hidden for type 0 or when it answers -1), the
  // interior's frame shown for a type.
  if (Category() == 4) {
    if (Item() == 4) {
      if (Byte(kType) == 0) {
        Shown(kAll, 17, 0);
      } else {
        Shown(kAll, 17, 1);
        const uint32_t child = ChildOf(Node(kAll, 17), 0);
        const uint64_t r4 = uint64_t(Byte(kType)) - 1;
        const uint64_t r5 = Item();
        // AeroPartValue leaves r4 and r5 as they came: its second call has
        // the same arguments.
        if (int32_t(uint32_t(AeroPartValue(base, screen, r4, r5, frame))) == -1) {
          Shown(kAll, 17, 0);
          guest::StoreU32(base, child + 108, 0);
        } else {
          guest::StoreU32(base, child + 108, uint32_t(AeroPartValue(base, screen, r4, r5, frame)));
        }
      }
    }
  } else if (Category() == 6 && Item() == 0) {
    Shown(kAll, 17, Byte(kType) == 0 ? 0 : 1);
  }
  if (Category() == 6 && Item() == 0) {
    ShowInteriorPicture(base, screen, Byte(kType), Byte(kType2), frame);
  }

  // The help line.
  ResetHelp(0x821E63A4);
  switch (Word(kCategory)) {
    case 1:
      if (Item() == 3) {
        Help(758);
      } else {
        HelpScroll();
      }
      break;
    case 6:
      if (Item() == 0) {
        Help(847);
      } else {
        HelpScroll();
      }
      break;
    default: HelpScroll(); break;
  }
}

// Block 7: state 8, the option select.
void Context::State8() {
  const auto bonnet = [&] { return Category() == 4 && Item() == 4; };
  const auto door = [&] { return Category() == 5 && Item() == 1; };
  // r6 at the door's rows: 31 once the textures were set (the second
  // SetNodeTexture's, which nothing after sets again), else unset (0).
  uint64_t door_r6 = 0;
  if (door() && int32_t(Word(9716)) != 0) {
    Texture(3, 1, 6);
    Texture(3, 2, 31);
    door_r6 = 31;
  }

  // The rows hidden: sub-scene 31's own children 5 to 14 (the bonnet),
  // sub-scene 33's child 1's (the tires) or 28's child 1's (the door).
  for (uint32_t n = 5; n < 15; n++) {
    uint32_t node;
    if (bonnet()) {
      node = Node(31, n);
    } else if (Category() == 3) {
      node = ChildOf(Node(33, 1), n);
    } else if (door()) {
      node = ChildOf(Node(28, 1), n);
    } else {
      continue;
    }
    guest::StoreU32(base, node + 80, 0);
  }

  // The rows' texts.
  const auto row = [&](uint32_t node, uint64_t id, uint32_t lr, uint64_t r6, uint64_t r10) {
    guest::StoreU32(base, node + 80, 1);
    NodeText(node, id, lr, r6, r10);
  };
  if (Category() == 3) {
    const uint32_t parent = Node(33, 1);
    row(ChildOf(parent, 5), 200, 0x821E64F8, 0, 0);
    row(ChildOf(parent, 6), 201, 0x821E6518, 0, 0);
    row(ChildOf(parent, 7), 202, 0x821E6538, 0, 0);
  }
  if (bonnet()) {
    // r10 the item byte, left from the category test.
    const uint32_t r10 = Item();
    row(Node(31, 5), 381, 0x821E6574, 0, r10);
    row(Node(31, 6), 382, 0x821E65F0, 0, r10);
  } else if (door()) {
    const uint32_t parent = Node(28, 1);
    row(ChildOf(parent, 5), 376, 0x821E65D0, door_r6, 0);
    row(ChildOf(parent, 6), 394, 0x821E65F0, door_r6, 0);
  }

  // The marks (the tires' sub-scene 33, the door's 28: child 0's children 15
  // to 24 and 26 to 35) hidden, then each of the +9680 rows' shown.
  const auto marks_scene = [&]() -> uint32_t {
    if (Category() == 3) {
      return 33;
    }
    return door() ? 28 : 0;
  };
  for (uint32_t n = 26; n < 36; n++) {
    const uint32_t sub = marks_scene();
    if (sub == 0) {
      continue;
    }
    const uint32_t node = Node(sub, 0);
    guest::StoreU32(base, ChildOf(node, n - 11) + 80, 0);
    guest::StoreU32(base, ChildOf(node, n) + 80, 0);
  }
  if (const uint32_t sub = marks_scene(); sub != 0 && Byte(kOptionRows) != 0) {
    for (uint32_t r = 0; int32_t(r) < int32_t(Byte(kOptionRows)); r++) {
      const uint32_t node = Node(sub, 0);
      const uint32_t mark = ChildOf(node, r + 15);
      const uint32_t flag = ChildOf(node, 38 + 2 * r);
      guest::StoreU32(base, mark + 80, 1);
      guest::StoreU32(base, flag + 108, GetChoiceFlag(base, screen, r) != 0 ? 0 : 1);
      guest::StoreU32(base, ChildOf(node, r + 26) + 80, 1);
    }
  }

  // The door's stand command 4 (its animation). The original's f1 is then
  // the float it loads for its GarageAnims::Add (60.0 at 0x82000F14); its
  // plain C++ doesn't report it, and its adapter, which the recompiled
  // refresh reaches, leaves f1 as it was: f1 left as that adapter leaves it.
  if (door()) {
    const uint32_t holder = guest::LoadU32(base, guest::LoadU32(base, kHolderAt));
    front_end_car_draw::GarageStageCommand(base, stand.calls, holder, 4, frame);
  }

  // Node 17: the bonnet's part picture, as state 7's.
  if (bonnet()) {
    if (Byte(kType) == 0) {
      Shown(kAll, 17, 0);
    } else {
      Shown(kAll, 17, 1);
      const uint32_t child = ChildOf(Node(kAll, 17), 0);
      const uint64_t r4 = uint64_t(Byte(kType)) - 1;
      const uint64_t r5 = Item();
      if (int32_t(uint32_t(AeroPartValue(base, screen, r4, r5, frame))) == -1) {
        Shown(kAll, 17, 0);
        guest::StoreU32(base, child + 108, 0);
      } else {
        guest::StoreU32(base, child + 108, uint32_t(AeroPartValue(base, screen, r4, r5, frame)));
      }
    }
  }

  // The help line by the option.
  ResetHelp(0x821E6874);
  if (Category() == 3) {
    Help(uint64_t(Byte(kOption)) + 781);
  } else if (door()) {
    Help(uint64_t(Byte(kOption)) + 790);
  } else {
    HelpScroll();
  }
}

}  // namespace tuning_screen::refresh
