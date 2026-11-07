// The tuning screen's lists on movies 2 and 13 (docs/functions/tuning-screen.md):
// see tuning_screen.h. The nodes through the UI movies' plain C++
// (UiMovies::Find, UiMovie::FindNode and Child; ShowNode and HideNode as
// their overrides do them), r3 followed through the calls as the originals
// leave it. The float steps as the translated code does them
// (camera_modes/steps.h), with flush-to-zero off (the adapters').

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace tuning_screen {

namespace {

namespace steps = camera_modes::steps;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// std: a doubleword, big-endian.
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

constexpr uint32_t kAll = 0xFFFFFFFFu;  // sub-scene -1: the scene itself
constexpr uint32_t kMovie13 = 13;

// UiMovie::ShowNode sub_8211AC00 and HideNode sub_8211AC48: the node's +80
// set; the node.
uint32_t Show(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  Store(base, node + 80, 1);
  return node;
}
uint32_t Hide(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  Store(base, node + 80, 0);
  return node;
}

// The int-to-float conversion through the frame's +80 (std, lfd, fcfid,
// frsp): the doubleword left there, the single's bits.
uint32_t Convert(uint8_t* base, uint32_t at, int32_t value) {
  Store64(base, at, uint64_t(int64_t(value)));
  return steps::Bits(steps::Single(double(value)));
}

}  // namespace

uint32_t UpdateListDots(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EE9B0 TuningScreen::UpdateListDots");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  auto last = [&] { return int32_t(Load(base, screen + 25224)); };
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 38; i++) {
    const uint32_t node = ui_movies::FindNode(base, movie, 1, 6);
    const uint32_t first = ui_movies::Child(base, node, i);
    const uint32_t second = ui_movies::Child(base, node, i);
    Store(base, first + 80, 0);
    const int32_t n = last();
    uint32_t size = 0;
    if (n >= 0 && n <= 2) {
      size = 3;
    } else if (n >= 3 && n <= 4) {
      size = 2;
    } else if (n >= 6 && n <= 7) {
      size = 1;
    }
    Store(base, second + 108, size);
    r3 = second;
  }
  if (int32_t(uint32_t(last()) + 1) > 0) {
    for (uint32_t j = 0; int32_t(j) < int32_t(uint32_t(last()) + 1); j++) {
      r3 = ui_movies::Child(base, ui_movies::FindNode(base, movie, 1, 6), j);
      Store(base, r3 + 80, 1);
    }
  }
  const int32_t n = last();
  if (n >= 0 && n <= 2) {
    const uint32_t node = ui_movies::FindNode(base, movie, 1, 6);
    r3 = ui_movies::Child(base, node, Load(base, screen + 25220));
    Store(base, r3 + 108, 4);
    return r3;
  }
  // h dots each side of the cursor, wrapped over the n + 1 entries; 4 at
  // the cursor, one less a step away.
  const int32_t h = (n >= 3 && n <= 4) ? 1 : (n >= 6 && n <= 7) ? 2 : 3;
  for (int32_t j = 0; j <= 2 * h; j++) {
    const uint32_t node = ui_movies::FindNode(base, movie, 1, 6);
    const uint32_t cursor = Load(base, screen + 25220);
    const uint32_t count = Load(base, screen + 25224) + 1;
    uint32_t index = cursor + uint32_t(j - h);
    if (int32_t(index) >= int32_t(count)) {
      index -= count;
    } else if (int32_t(index) < 0) {
      index += count;
    }
    r3 = ui_movies::Child(base, node, index);
    Store(base, r3 + 108, uint32_t(4 - (j > h ? j - h : h - j)));
  }
  return r3;
}

uint32_t ShowOptionMarks(uint8_t* base, uint32_t screen, uint32_t show, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82202F30 TuningScreen::ShowOptionMarks");
  const uint32_t entry = MovieEntry(base, Frame(base, sp, 112), kMovie13);
  const uint32_t movie = Load(base, entry);
  if (show == 0) {
    uint32_t r3 = entry;
    for (const uint32_t sub_scene : {28u, 27u}) {
      for (uint32_t k = 26; k <= 31; k++) {
        r3 = Hide(base, movie, sub_scene, k);
      }
    }
    return r3;
  }
  const int32_t variant = int32_t(Load(base, screen + 30724));
  if (variant != 0 && variant != 1) {
    return entry;
  }
  const uint32_t sub_scene = variant == 0 ? 28 : 27;
  auto option = [&] { return Load(base, screen + 30732); };
  switch (int32_t(Load(base, screen + 30756))) {
    case 0:
      return Show(base, movie, sub_scene, option() * 2 + 27);
    case 1:
      Show(base, movie, sub_scene, (option() + 13) * 2);
      return Show(base, movie, sub_scene, option() * 2 + 27);
    case 2:
      return Show(base, movie, sub_scene, (option() + 13) * 2);
    default:
      return entry;
  }
}

uint32_t PlaceStateNodes(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82202C08 TuningScreen::PlaceStateNodes");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = MovieEntry(base, frame, kMovie13);
  const uint32_t movie = Load(base, entry);
  // The row +30724 of the table at 0x8200C860 (56 bytes a row), read again
  // for each word.
  auto t = [&](uint32_t k) {
    return Load(base, 0x8200C860u + 56 * Load(base, screen + 30724) + 4 * k);
  };
  struct Place {
    uint32_t child;
    uint32_t k;  // t[k] + dx, t[k + 1] + dy
    uint32_t dx;
    uint32_t dy;
  };
  auto place = [&](std::initializer_list<Place> places) {
    uint32_t node = entry;
    for (const Place& p : places) {
      node = ui_movies::FindNode(base, movie, 29, p.child);
      Store(base, node + 12, Convert(base, frame + 80, int32_t(t(p.k) + p.dx)));
      Store(base, node + 16, Convert(base, frame + 80, int32_t(t(p.k + 1) + p.dy)));
    }
    return node;
  };
  switch (int32_t(Load(base, screen + kState))) {
    case 29:
      return place({{4, 0, 288, 0}, {5, 2, 0, 224}, {6, 4, 640, 160}, {7, 6, 288, 304}});
    case 30:
      return place({{8, 8, 632, 320}, {9, 10, 0, 80}});
    case 31:
      return place({{10, 12, 208, 272}});
    default:
      return entry;
  }
}

uint32_t LayOutItems(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82203110 TuningScreen::LayOutItems");
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t entry = MovieEntry(base, frame, kMovie13);
  const uint32_t movie = Load(base, entry);
  const int32_t state = int32_t(Load(base, screen + kState));
  if (state == 26 || state == 29 || state == 30 || state == 31 || state == 34) {
    return entry;
  }
  const int32_t variant = int32_t(Load(base, screen + 30724));
  if (variant != 0 && variant != 1) {
    return entry;
  }
  const bool second_set = variant == 1;
  const uint32_t first = second_set ? 22 : 19;
  const uint32_t records = screen + (second_set ? 31096 : 31024);
  const uint32_t origin_x = screen + (second_set ? 30816 : 30800);
  const uint32_t origin_y = screen + (second_set ? 30820 : 30804);
  const uint32_t scale = screen + (second_set ? 30828 : 30812);
  const uint32_t size = screen + (second_set ? 30824 : 30808);
  const double add_y = steps::Lfs(base, second_set ? 0x8200CAE8u : 0x8200CAF0u);
  const double add_x = steps::Lfs(base, second_set ? 0x8200CAECu : 0x8200CAF4u);
  const double half = steps::Lfs(base, 0x82001BE4u);
  // The conversions' doublewords: x, the origin's x, y, the origin's y.
  const uint32_t at_x = frame + (second_set ? 104 : 80);
  const uint32_t at_ox = frame + (second_set ? 96 : 88);
  const uint32_t at_y = frame + (second_set ? 88 : 96);
  const uint32_t at_oy = frame + (second_set ? 80 : 104);
  auto lwa = [&](uint32_t address) { return int32_t(Load(base, address)); };
  auto single = [&](uint32_t at, int32_t value) {
    Store64(base, at, uint64_t(int64_t(value)));
    return steps::Single(double(value));
  };
  uint32_t r3 = entry;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t record = records + 24 * i;
    const uint32_t looked = ui_movies::FindNode(base, movie, kAll, first + i);
    const uint32_t node = ui_movies::FindNode(base, movie, kAll, first + i);
    {
      const double s = steps::Lfs(base, scale);
      const double ox = single(at_ox, lwa(origin_x));
      const double x = single(at_x, lwa(record + 0));
      steps::Stfs(base, node + 12, steps::Fadds(steps::Fmadds(x, s, ox), add_x));
    }
    {
      const int32_t y_word = lwa(record + 4);
      const double s = steps::Lfs(base, scale);
      const int32_t oy_word = lwa(origin_y);
      const double y = single(at_y, y_word);
      const double oy = single(at_oy, oy_word);
      steps::Stfs(base, node + 16, steps::Fadds(steps::Fmadds(y, s, oy), add_y));
    }
    Store(base, node + 20,
          uint32_t(steps::Fctiwz(steps::Fmuls(steps::Lfs(base, looked + 100), half))));
    Store(base, node + 24,
          uint32_t(steps::Fctiwz(steps::Fmuls(steps::Lfs(base, looked + 104), half))));
    steps::Stfs(base, node + 36, steps::Lfs(base, record + 8));
    for (const uint32_t field : {28u, 32u}) {
      const double record_scale = steps::Lfs(base, record + 12);
      const double sz = steps::Lfs(base, size);
      steps::Stfs(base, node + field,
                  second_set ? steps::Fmuls(record_scale, sz) : steps::Fmuls(sz, record_scale));
    }
    Store(base, looked + 132, int32_t(Load(base, record + 16)) != 0 ? 1 : 0);
    r3 = node;
  }
  return r3;
}

uint32_t ShowItems(uint8_t* base, uint32_t screen, uint32_t show, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82203B00 TuningScreen::ShowItems");
  const uint32_t entry = MovieEntry(base, Frame(base, sp, 144), kMovie13);
  const uint32_t movie = Load(base, entry);
  if (show == 0) {
    uint32_t r3 = entry;
    for (uint32_t k = 19; k <= 24; k++) {
      r3 = Hide(base, movie, kAll, k);
    }
    return r3;
  }
  const int32_t variant = int32_t(Load(base, screen + 30724));
  if (variant != 0 && variant != 1) {
    return entry;
  }
  const uint32_t first = variant == 0 ? 19 : 22;
  const uint32_t marks = variant == 0 ? 28 : 27;
  const uint32_t words = screen + (variant == 0 ? 31192 : 31204);
  uint32_t r3 = entry;
  for (uint32_t i = 0; i < 3; i++) {
    r3 = Show(base, movie, kAll, first + i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    const bool none = int32_t(Load(base, words + 4 * i)) == -1;
    const uint32_t child =
        ui_movies::Child(base, ui_movies::FindNode(base, movie, kAll, marks), 10 + 4 * i);
    Store(base, child + 80, none ? 0 : 1);
    r3 = none ? Hide(base, movie, kAll, first + i) : Show(base, movie, kAll, first + i);
  }
  return r3;
}

}  // namespace tuning_screen
