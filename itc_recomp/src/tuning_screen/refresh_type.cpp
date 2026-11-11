// TuningScreen::Refresh's state 6, TYPE_SELECT (sub_821E3AD0's block 5,
// docs/functions/tuning-screen.md's "The refresh"; refresh.h): the textures
// (+9716 set only), the type list's rows in sub-scene 29 (category 0 item 2)
// or 28 (the aero, category 5 items 2 and 3, category 6 items 0 to 2) with
// IsItemAvailable, their marks (GetChoiceFlag), the scroll arrows by the
// byte +9678 and +9662 - +25108, sub-scene 8's child 2 and textures (category
// 5 item 3), node 20, node 17 (the aero's AeroPartValue, the interior's
// picture frame), ShowInteriorPicture (category 6 item 0) and the help line
// (the rear spoiler's by the selected garage car's aero parts 77 and 82).
// The nodes through the UI movies' plain C++; the texts (SetNodeText) through
// its override from the original's frame with its return addresses, r6 set
// where the original sets it, the others 0 (the user's call, 2026-10-30);
// the callees by their plain C++ (Params::GetCarParts, AeroParts::Get77 and
// Get82 through the guest where their plain C++ declines, memcpy through the
// guest where Buffers_Memcpy declines).
//
// The rear spoiler's help path moves r31 to s + 36588 in the original; only
// r30 (the movie) is read after it (the help line's tail, then the epilogue
// restores r22 to r31), so nothing here depends on it.

#include "tuning_screen/refresh.h"

#include "car_specs/car_specs.h"
#include "guest.h"
#include "native/buffers.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace tuning_screen::refresh {

namespace {

constexpr uint32_t kRows = 9678;          // a byte: the type list's rows
constexpr uint32_t kFirstRow = 25108;     // the list's first type shown
constexpr uint32_t kSpoilerShift = 25172; // set: the rear spoiler's types shifted by one
constexpr uint32_t kBodyKit = 36588;      // AeroParts::Get82's level

constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kGet77 = 0x820CB9E0;  // AeroParts::Get77(out, parts, level)
constexpr uint32_t kGet82 = 0x820CBC90;  // AeroParts::Get82(out, parts, level)

}  // namespace

// Block 5: state 6, the item's types.
void Context::State6() {
  if (Word(9716) != 0) {
    Texture(3, 1, uint64_t(Word(kCategory)) + 1);
    // By the category (unsigned): none for 7 and above.
    static constexpr int32_t kFirst[7] = {8, 12, -1, 17, 20, 30, 38};
    const uint32_t again = Word(kCategory);
    if (again < 7) {
      Texture(3, 2, again == 2 ? 3 : uint64_t(Item()) + uint64_t(kFirst[again]));
    }
  }

  // The pages listed in sub-scene 28: the aero (any item), category 5 items
  // 2 and 3, category 6 items 0 to 2.
  const auto in28 = [&] {
    const int32_t c = Category();
    const uint32_t item = Item();
    return c == 4 || (c == 5 && (item == 2 || item == 3)) || (c == 6 && item <= 2);
  };
  const auto power2 = [&] { return Category() == 0 && Item() == 2; };

  // The list's rows (children 5 to 14 of sub-scene 28's child 1, or
  // sub-scene 29's own children for category 0 item 2) hidden and colored 0.
  for (uint32_t n = 5; n < 15; n++) {
    uint32_t node;
    if (power2()) {
      node = Node(29, n);
    } else if (in28()) {
      node = ChildOf(Node(28, 1), n);
    } else {
      continue;
    }
    guest::StoreU32(base, node + 80, 0);
    ui_movies::SetColor(base, node, 0);
  }

  // A row's node shown.
  const auto shown = [&](uint32_t node) {
    guest::StoreU32(base, node + 80, 1);
    return node;
  };
  const auto row28 = [&](uint32_t child) { return shown(ChildOf(Node(28, 1), child)); };
  {
    const int32_t c = Category();
    const uint32_t item = Item();
    if (c == 0 && item == 2) {
      NodeText(shown(Node(29, 5)), 376, 0x821E52B8);
      for (uint32_t row = 1; int32_t(row) < int32_t(Byte(kRows)); row++) {
        NodeText(shown(Node(29, row + 5)), 379, 0x821E52F0, row);
      }
    } else if (c == 4 && item == 3) {
      if (int32_t(Word(kSpoilerShift)) != 0) {
        // Row 0 376, row 1 375 (none), then 379 with the type from 1.
        const uint32_t parent = Node(28, 1);
        NodeText(shown(ChildOf(parent, 5)), 376, 0x821E5354);
        NodeText(shown(ChildOf(parent, 6)), 375, 0x821E5374);
        if (Byte(kRows) > 2) {
          uint32_t row = 1;
          do {
            NodeText(row28(row + 6), 379, 0x821E53B4, row);
            row++;
          } while (int32_t(row + 1) < int32_t(Byte(kRows)));
        }
      } else {
        NodeText(row28(5), 376, 0x821E53EC);
        for (uint32_t row = 1; int32_t(row) < int32_t(Byte(kRows)); row++) {
          NodeText(row28(row + 5), 379, 0x821E542C, row);
        }
      }
    } else if ((c == 4 && item <= 9) || (c == 5 && (item == 2 || item == 3)) ||
               (c == 6 && item <= 2)) {
      // 375 first for aero item 9, else 376; the rows colored 2 when
      // IsItemAvailable answers 0.
      const uint32_t first = row28(5);
      NodeText(first, Category() == 4 && Item() == 9 ? 375 : 376, 0x821E55A0);
      for (uint32_t row = 1; int32_t(row) < int32_t(Byte(kRows)); row++) {
        const uint32_t node = row28(row + 5);
        if (int32_t(IsItemAvailable(base, screen, row)) == 0) {
          ui_movies::SetColor(base, node, 2);
        }
        NodeText(node, 379, 0x821E5608, row);
      }
    }
  }

  // The marks (children 15 to 24 and 26 to 35 of sub-scene 28's child 0)
  // hidden, then each row's shown (Refresh's state 5 marks: child 38 + 2 row's
  // +108 0 when GetChoiceFlag answers for the row, else 1).
  for (uint32_t n = 26; n < 36; n++) {
    if (!in28()) {
      continue;
    }
    const uint32_t node = Node(28, 0);
    guest::StoreU32(base, ChildOf(node, n - 11) + 80, 0);
    guest::StoreU32(base, ChildOf(node, n) + 80, 0);
  }
  if (in28()) {
    for (uint32_t row = 0; int32_t(row) < int32_t(Byte(kRows)); row++) {
      const uint32_t node = Node(28, 0);
      const uint32_t mark = ChildOf(node, row + 15);
      const uint32_t flag = ChildOf(node, 38 + 2 * row);
      guest::StoreU32(base, mark + 80, 1);
      guest::StoreU32(base, flag + 108, GetChoiceFlag(base, screen, row) != 0 ? 0 : 1);
      guest::StoreU32(base, ChildOf(node, row + 26) + 80, 1);
    }
  }

  // The scroll arrows by the rows and the type's place from +25108.
  {
    const uint32_t count = Byte(kRows);
    if (count <= 8) {
      Shown(kAll, 25, 0);
      Shown(kAll, 26, 0);
    } else {
      const uint64_t d = uint64_t(Byte(kType)) - uint64_t(Word(kFirstRow));
      if (int32_t(d) == 0) {
        Shown(kAll, 25, 1);
        Shown(kAll, 26, 0);
      } else {
        Shown(kAll, 25, int32_t(count - 1) == int32_t(uint32_t(d + 7)) ? 0 : 1);
        Shown(kAll, 26, 1);
      }
    }
  }

  // Category 5 item 3: sub-scene 8's child 2 shown, its children 8 and 9's
  // textures 3 and 10.
  if (Category() == 5 && Item() == 3) {
    Shown(8, 2, 1);
    Texture(8, 8, 3);
    Texture(8, 9, 10);
  }
  // Node 20 for category 0 item 2.
  Shown(kAll, 20, power2() ? 1 : 0);

  // Node 17: the aero's part picture (its child 0's +108 AeroPartValue's for
  // the type - 1, or - 2 on the rear spoiler with +25172 set, hidden for the
  // types without one), the interior's frame for category 6 item 0.
  const int32_t category = Category();
  if (category == 4) {
    const uint32_t type = Byte(kType);
    int64_t step = 0;  // the type's offset (0: node 17 hidden)
    if (Item() == 3 && int32_t(Word(kSpoilerShift)) != 0) {
      step = type == 0 || type == 1 ? 0 : 2;
    } else {
      step = type == 0 ? 0 : 1;
    }
    if (step == 0) {
      Shown(kAll, 17, 0);
    } else {
      Shown(kAll, 17, 1);
      const uint32_t child = ChildOf(Node(kAll, 17), 0);
      const uint64_t r4 = uint64_t(Byte(kType)) - uint64_t(step);
      const uint64_t r5 = Item();
      // AeroPartValue leaves r4 and r5 as they came: its second call has the
      // same arguments.
      if (int32_t(uint32_t(AeroPartValue(base, screen, r4, r5, frame))) == -1) {
        Shown(kAll, 17, 0);
        guest::StoreU32(base, child + 108, 0);
      } else {
        guest::StoreU32(base, child + 108, uint32_t(AeroPartValue(base, screen, r4, r5, frame)));
      }
    }
  } else if (category == 6 && Item() == 0) {
    const uint32_t type = Byte(kType);
    if (type == 0) {
      Shown(kAll, 17, 0);
    } else {
      Shown(kAll, 17, 1);
      const uint32_t child = ChildOf(Node(kAll, 17), 0);
      guest::StoreU32(base, child + 108, Byte(kType) <= 2 ? 4 : 24);
    }
  }
  if (Category() == 6 && Item() == 0) {
    ShowInteriorPicture(base, screen, Byte(kType), 0, frame);
  }

  // The help line by the category and the item.
  switch (Category()) {
    case 0:
      ResetHelp(0x821E5A80);
      if (Item() == 2) {
        Help(728);
      } else {
        HelpScroll();
      }
      break;
    case 4:
      ResetHelp(0x821E5AAC);
      if (Item() == 3) {
        Help(SpoilerHelp());
      } else {
        HelpScroll();
      }
      break;
    case 5:
      ResetHelp(0x821E5C08);
      switch (Item()) {
        case 2: Help(792); break;
        case 3: Help(793); break;
        default: HelpScroll(); break;
      }
      break;
    case 6:
      ResetHelp(0x821E5C44);
      switch (Item()) {
        case 0: Help(846); break;
        case 1: Help(848); break;
        case 2: Help(849); break;
        default: HelpScroll(); break;
      }
      break;
    default: break;  // no help line
  }
}

// The rear spoiler's help string: the selected garage car's parts
// (Params::GetCarParts into the frame's +176, copied to +96 by memcpy), its
// part 77 at the type (+9662; with +25172 set, 0 for 0, - 1 for 2 to 5, -1
// for 1, 0 above 5) into +144 and 82 at +36588 into +224: 865 with +36588 0
// and the type -1; else 629 or 630 by bit 0 of 77's +24, or with +36588 set
// and bit 1 of 82's +24 clear, by its bit 2.
uint64_t Context::SpoilerHelp() const {
  const uint32_t save = Word(kSaveData);
  const uint32_t list = save + 6120;
  uint32_t count = guest::LoadU32(base, save + 0x19920);
  uint32_t at = uint32_t(int32_t(int16_t(guest::LoadU16(base, list + 204)))) * 4 + list;
  while (count != 0) {
    at = uint32_t(int32_t(int16_t(guest::LoadU16(base, at)))) * 4 + list;
    count--;
  }
  const int32_t slot = int16_t(guest::LoadU16(base, at + 2));
  const uint32_t car = guest::LoadU32(base, uint32_t(int64_t(slot) * 1968) + list + 224);
  const uint32_t parts_out = frame + 176, parts = frame + 96;
  uint32_t words[9];
  car_specs::GetCarParts(base, car, words);
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, parts_out + 4 * i, words[i]);
  }
  if (!native::Buffers_Memcpy(base, parts, parts_out, 36, 0x821E5B14)) {
    Call(kMemcpy, {parts, parts_out, 36}, 0x821E5B14);
  }
  int64_t type;
  if (int32_t(Word(kSpoilerShift)) != 0) {
    const uint32_t t = Byte(kType);
    if (t == 0) {
      type = 0;
    } else if (t >= 2 && t <= 5) {
      type = int64_t(t) - 1;
    } else if (t == 1) {
      type = -1;
    } else {
      type = 0;
    }
  } else {
    type = Byte(kType);
  }
  const auto get = [&](uint32_t function, uint32_t out, uint64_t level, uint32_t lr,
                       bool (*plain)(uint8_t*, uint32_t, const uint32_t[9], int32_t)) {
    uint32_t loaded[9];
    for (uint32_t i = 0; i < 9; i++) {
      loaded[i] = guest::LoadU32(base, parts + 4 * i);
    }
    if (plain(base, out, loaded, int32_t(level))) {
      return out;
    }
    return uint32_t(Call(function, {out, parts, level}, lr));
  };
  const uint32_t kit77 = get(kGet77, frame + 144, uint64_t(type), 0x821E5B70, car_specs::AeroPart77);
  const uint32_t a = guest::LoadU32(base, kit77 + 24);
  const uint32_t kit82 =
      get(kGet82, frame + 224, Word(kBodyKit), 0x821E5B90, car_specs::AeroPart82);
  const uint32_t b = guest::LoadU32(base, kit82 + 24);
  if (Word(kBodyKit) == 0) {
    if (int32_t(type) == -1) {
      return 865;
    }
    return (a & 1) != 0 ? 629 : 630;
  }
  if ((b & 2) != 0) {
    return (a & 1) != 0 ? 629 : 630;
  }
  return (b & 4) != 0 ? 629 : 630;
}

}  // namespace tuning_screen::refresh
