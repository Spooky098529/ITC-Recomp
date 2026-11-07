// The tuning screen's help texts by state and the part list's rows
// (docs/functions/tuning-screen.md): see tuning_screen.h. Movie 2's nodes
// through the UI movies' plain C++ (FindNode, Child, SetNodeTexture,
// NamedAnimationTime, ShowNode's and HideNode's stores); SetText,
// SetNodeText, ScrollText and sub_8211C5F8 through their overrides from each
// original's frame with its return addresses, the varargs they leave unset
// passed as 0 (the user's call, 2026-10-30).

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace tuning_screen {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

constexpr uint32_t kSetNodeText = 0x8211C7E8;  // UiMovie::SetNodeText(movie, node, id, ...)
constexpr uint32_t kNotPlaying = 0x82001BE0;   // -1.0: NamedAnimationTime's answer for none

// UiMovie::ShowNode and HideNode (sub_8211AC00, sub_8211AC48): the node's
// +80 set; the node.
uint32_t SetShown(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
                  uint32_t shown) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  Store(base, node + 80, shown);
  return node;
}

// The text's scroll reset, SetText(movie, 10, 0, id) and ScrollText(movie,
// 10, 0, 1) from the return addresses `lr`; ScrollText's r3.
uint64_t HelpText(const GuestCall& call, uint32_t frame, uint32_t movie, uint64_t id,
                  const uint32_t (&lr)[3]) {
  call(kResetTextScroll, {movie, 10, 0}, frame, lr[0]);
  call(kSetText, {movie, 10, 0, id, 0, 0, 0, 0}, frame, lr[1]);
  return call(kScrollText, {movie, 10, 0, 1}, frame, lr[2]);
}

}  // namespace

uint64_t SetStateTexts(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp,
                       std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821F1DE0 TuningScreen::SetStateTexts");
  const uint32_t frame = Frame(base, sp, 224);
  const uint32_t entry = MovieEntry(base, frame, kMovieId);
  const int32_t state = int32_t(Load(base, screen + kState));
  const uint32_t movie = Load(base, entry);
  if (state == 17) {
    call(kResetTextScroll, {movie, 10, 0}, frame, 0x821F1E3C);
    const uint64_t id = uint64_t(Load(base, screen + 25228)) + 785;
    call(kSetText, {movie, 10, 0, id, 0, 0, 0, 0}, frame, 0x821F1E54);
    call(kScrollText, {movie, 10, 0, 1}, frame, 0x821F1E68);
    const bool first = Load(base, screen + 25204) == 0;
    return SetShown(base, movie, uint32_t(kAllScenes), 17, first ? 0 : 1);
  }
  if (state == 16) {
    // The group tabs, when neither 1007 nor 62 plays and +9716 is 0.
    const double none = camera_modes::steps::Lfs(base, kNotPlaying);
    auto time = [&](uint32_t index) {
      const double t = double(std::bit_cast<float>(ui_movies::NamedAnimationTime(base, movie, index)));
      *f1 = t;
      return t;
    };
    if (time(1007) == none && time(62) == none && Load(base, screen + 9716) == 0) {
      for (uint32_t i = 0; i < 10; i++) {
        const uint32_t tab = ui_movies::Child(base, ui_movies::FindNode(base, movie, 1, 9), i + 5);
        Store(base, tab + 80, 0);
      }
      const uint32_t table = frame + 96;
      Store(base, table, 376);
      for (uint32_t i = 1; i < 17; i++) {
        Store(base, table + 4 * i, 1183 + i);
      }
      for (uint32_t j = 0; j < 4; j++) {
        const uint32_t tab = ui_movies::Child(base, ui_movies::FindNode(base, movie, 1, 9), j + 5);
        Store(base, tab + 80, 1);
        const uint32_t k = Load(base, screen + 25204) - Load(base, screen + 25212) + j;
        const uint64_t id = Load(base, table + 4 * k);
        call(kSetNodeText, {movie, tab, id, 0, 0, 0, 0, 0}, frame, 0x821F1F70);
      }
    }
    SetShown(base, movie, 1, 26, 1);
    SetShown(base, movie, 1, 27, 1);
    const int32_t group = int32_t(Load(base, screen + 25204));
    if (group == 0) {
      SetShown(base, movie, 1, 26, 0);
    } else if (group == 16) {
      SetShown(base, movie, 1, 27, 0);
    }
    return HelpText(call, frame, movie, 784, {0x821F1FE8, 0x821F2048, 0x821F205C});
  }
  if (state == 15) {
    if (Load(base, screen + 9716) != 0) {
      ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, 3, 1), 6);
      ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, 3, 2), 30);
    }
    return HelpText(call, frame, movie, 637, {0x821F2034, 0x821F2048, 0x821F205C});
  }
  return entry;
}

namespace {

// The part list's row on node (1, `node`) for the item `index` (all 64
// bits, as the original passes it to WheelNumber and ShowPartMarks; `index`
// recomputed from the screen's words for each of its three uses), as the
// original lays it: the picture (child 2) +108 = WheelNumber(G, i) - 1 (or
// `picture` when given), the owned mark (child 1) +80 by the screen's word
// at +27856 + 152 G + 4 i, ShowPartMarks, the picture clamped to 0..206.
// ShowPartMarks' r3.
template <typename Index>
uint64_t FillRow(uint8_t* base, uint64_t r3, uint32_t frame, uint32_t movie, uint32_t node,
                 const Index& index, const std::optional<uint32_t>* picture_number = nullptr) {
  const uint32_t screen = uint32_t(r3);
  const uint32_t row = ui_movies::FindNode(base, movie, 1, node);
  const uint32_t owned = ui_movies::Child(base, row, 1);
  const uint32_t picture = ui_movies::Child(base, row, 2);
  if (picture_number == nullptr) {
    const uint64_t brand = Load(base, screen + 25204);
    Store(base, picture + 108, uint32_t(WheelNumber(brand, index()) - 1));
  } else if (picture_number->has_value()) {
    Store(base, picture + 108, **picture_number);
  }
  const uint64_t group = Load(base, screen + 25204);
  const uint32_t word = uint32_t(group * 38 + index() + 6964) * 4 + screen;
  Store(base, owned + 80, Load(base, word) != 0 ? 1 : 0);
  const uint64_t marks_index = index();
  const uint64_t marks_r3 =
      ShowPartMarks(base, r3, row, Load(base, screen + 25204), marks_index, frame);
  const int32_t number = int32_t(Load(base, picture + 108));
  if (number > 206) {
    Store(base, picture + 108, 206);
  } else if (number < 0) {
    Store(base, picture + 108, 0);
  }
  return marks_r3;
}

// The selected garage car's slot (the list at [+18904] + 6120 walked
// [[+18904] + 0x19920] steps from its head +204, the last entry's +2).
struct CarSlot {
  uint32_t list;
  int16_t slot;
};
CarSlot SelectedCarSlot(uint8_t* base, uint32_t screen) {
  const uint32_t save = Load(base, screen + kSaveData);
  const uint32_t list = save + 6120;
  uint32_t count = Load(base, save + 0x19920);
  uint32_t at = uint32_t(int32_t(int16_t(guest::LoadU16(base, list + 204)))) * 4 + list;
  while (count != 0) {
    at = uint32_t(int32_t(int16_t(guest::LoadU16(base, at)))) * 4 + list;
    count--;
  }
  return {list, int16_t(guest::LoadU16(base, at + 2))};
}
// Its spec id: the word at list + 1,968 × slot + 224.
uint32_t CarId(uint8_t* base, const CarSlot& car) {
  return Load(base, uint32_t(int32_t(car.slot) * 1968) + car.list + 224);
}

}  // namespace

uint64_t FillPartRow(uint8_t* base, uint64_t r3, uint32_t frame, uint32_t movie, uint32_t node,
                     const std::function<uint64_t()>& index) {
  return FillRow(base, r3, frame, movie, node, index);
}

uint64_t FillPartRows(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F4BF8 TuningScreen::FillPartRows");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t entry = MovieEntry(base, frame, kMovieId);
  const uint32_t movie = Load(base, entry);
  const CarSlot car = SelectedCarSlot(base, screen);
  const int32_t last = int32_t(Load(base, screen + 25224));
  auto cursor = [&] { return uint64_t(Load(base, screen + 25220)); };
  if (last == 0) {
    HidePartMarks(base, frame);
    if (Load(base, screen + 25204) != 0) {
      return FillRow(base, r3, frame, movie, 15, cursor);
    }
    // The car's picture (its piece sub_821F4CF0 for id 0, merged): 184 +
    // id for 0 to 21, 206 for 22, left above.
    const uint32_t id = CarId(base, car);
    std::optional<uint32_t> picture;
    if (id <= 21) {
      picture = 184 + id;
    } else if (id == 22) {
      picture = 206;
    }
    return FillRow(base, r3, frame, movie, 15, cursor, &picture);
  }
  if (last == 1) {
    HidePartMarks(base, frame);
    FillRow(base, r3, frame, movie, 15, cursor);
    return FillRow(base, r3, frame, movie, 16, [&] { return cursor() + 1; });
  }
  if (last == 2) {
    HidePartMarks(base, frame);
    uint64_t marks_r3 = 0;
    for (uint64_t k = 0; k < 3; k++) {
      // c + k - 1 wrapped over 3.
      auto index = [&] {
        const uint64_t i = cursor() + k;
        const uint64_t j = i - 1;
        if (int32_t(j) >= 3) {
          return i - 4;
        }
        if (int32_t(j) < 0) {
          return i + 2;
        }
        return j;
      };
      marks_r3 = FillRow(base, r3, frame, movie, uint32_t(14 + k), index);
    }
    return marks_r3;
  }
  if (last < 4) {
    return entry;
  }
  HidePartMarks(base, frame);
  uint64_t marks_r3 = 0;
  for (uint64_t k = 0; k < 5; k++) {
    // c + k - 1 wrapped over n + 1.
    auto index = [&] {
      const uint64_t c = cursor(), n = Load(base, screen + 25224);
      const uint64_t j = c + k - 1;
      if (int32_t(j) >= int32_t(n + 1)) {
        return c - n + k - 2;
      }
      if (int32_t(j) < 0) {
        return c + n + k;
      }
      return j;
    };
    marks_r3 = FillRow(base, r3, frame, movie, uint32_t(14 + k), index);
  }
  return marks_r3;
}

}  // namespace tuning_screen
