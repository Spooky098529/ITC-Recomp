// The tuning screen's node setters on movie 2 (docs/functions/tuning-screen.md):
// see tuning_screen.h. The nodes through the UI movies' plain C++
// (UiMovies::Find, UiMovie::FindNode, Child), r3 followed through the calls
// as the originals leave it.

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace tuning_screen {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
uint32_t Load8(uint8_t* base, uint32_t address) { return guest::LoadU8(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

constexpr uint32_t kAll = 0xFFFFFFFFu;  // sub-scene -1: the scene itself

}  // namespace

uint32_t ShowInteriorPicture(uint8_t* base, uint32_t screen, uint32_t type, uint32_t index,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DA3F8 TuningScreen::ShowInteriorPicture");
  const uint32_t movie = Movie(base, Frame(base, sp, 128));
  const uint32_t node = ui_movies::FindNode(base, movie, kAll, 0);
  if (type == 0) {
    Store(base, node + 80, 0);
    return node;
  }
  Store(base, node + 80, 1);
  const uint32_t picture = ui_movies::Child(base, ui_movies::FindNode(base, movie, kAll, 0), 0);
  switch (Load8(base, screen + kType)) {
    case 1: Store(base, picture + 108, index); break;
    case 2: Store(base, picture + 108, index + 4); break;
    case 3: Store(base, picture + 108, index + 8); break;
    case 4: Store(base, picture + 108, index + 11); break;
    default: break;
  }
  return picture;
}

uint32_t HideTireMarks(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DFB20 TuningScreen::HideTireMarks");
  const uint32_t movie = Movie(base, Frame(base, sp, 128));
  uint32_t child = 0;
  for (const uint32_t sub_scene : {30u, 33u}) {
    const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, 0);
    for (const uint32_t k : {11u, 10u, 1u}) {
      child = ui_movies::Child(base, node, k);
      Store(base, child + 80, 0);
    }
  }
  return child;
}

uint64_t HideNode19(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DFBE8 TuningScreen::HideNode19");
  (void)r3;
  const uint32_t screen = uint32_t(r3);
  const uint32_t movie = Movie(base, Frame(base, sp, 112));
  const int32_t state = int32_t(Load(base, screen + kState));
  const int32_t category = int32_t(Load(base, screen + kCategory));
  const uint32_t item = Load8(base, screen + kItem);
  bool hide = false;
  if (state == 5) {
    hide = category == 3 || Load8(base, screen + kLevel) == 0;
  } else if (state == 6) {
    if (category == 4 && item == 3) {
      const bool set = int32_t(Load(base, screen + 25172)) != 0;
      const uint32_t type = Load8(base, screen + kType);
      hide = type == 0 || (set && type == 1);
    } else if (category == 0 && item == 2) {
      hide = Load8(base, screen + kLevel) == 0;
    } else {
      hide = Load8(base, screen + kType) == 0;
    }
  } else if (state == 8) {
    hide = category == 5 && item == 1 && Load8(base, screen + kOption) == 0;
  } else if (state == 54) {
    hide = int32_t(Load(base, screen + 33548)) == 0;
  } else if (state == 56) {
    hide = int32_t(Load(base, screen + 33564)) == 4;
  } else {
    hide = state == 9 || state == 10 || state == 11 || state == 14 || state == 15;
  }
  if (!hide) {
    return movie;
  }
  const uint32_t node = ui_movies::FindNode(base, movie, kAll, 19);
  Store(base, node + 80, 0);
  return node;
}

uint32_t ShowTireMarks(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E22E8 TuningScreen::ShowTireMarks");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = Movie(base, frame);
  auto at = [&](uint32_t offset) { return int32_t(Load(base, screen + offset)); };
  auto child = [&](uint32_t sub_scene, uint32_t k) {
    return ui_movies::Child(base, ui_movies::FindNode(base, movie, sub_scene, 0), k);
  };
  auto tire_state = [&](bool with_5) {
    const int32_t s = at(kState);
    return ((with_5 && s == 5) || s == 8 || s == 9 || s == 10 || s == 11) && at(kCategory) == 3;
  };
  uint32_t r3 = child(30, 2);
  Store(base, r3 + 80, 1);
  if (tire_state(true)) {
    if (Load8(base, screen + kItem) == 0) {
      r3 = child(30, 1);
      Store(base, r3 + 80, at(35948) == at(36076) ? 0 : 1);
      r3 = child(33, 1);
      const bool differs = at(35952) != at(36080) || at(35948) != at(36076);
      Store(base, r3 + 80, differs ? 1 : 0);
    } else {
      r3 = child(30, 1);
      Store(base, r3 + 80, 0);
      r3 = child(33, 1);
      Store(base, r3 + 80, 0);
    }
  } else {
    r3 = HideTireMarks(base, frame);
  }
  if (!tire_state(false)) {
    return r3;
  }
  if (Load8(base, screen + kItem) == 0) {
    const uint32_t a = ui_movies::FindNode(base, movie, 30, 0);
    r3 = ui_movies::Child(base, a, 2);
    Store(base, r3 + 80, 0);
    r3 = ui_movies::Child(base, a, 1);
    Store(base, r3 + 80, 0);
  }
  const uint32_t item = Load8(base, screen + kItem);
  if (item == 0) {
    r3 = child(33, 2);
    Store(base, r3 + 80, int32_t(Load8(base, screen + kLevel)) == at(35948) ? 1 : 0);
    r3 = child(33, 1);
    const int32_t level = int32_t(Load8(base, screen + kLevel));
    const int32_t rear = at(36076);
    bool shown = false;
    if (level == rear) {
      shown = at(35952) != at(36080) || at(35948) != rear;
    }
    Store(base, r3 + 80, shown ? 1 : 0);
  } else if (item == 1 || item == 2) {
    const uint32_t node = child(33, 2);
    r3 = GetCurrentChoice(base, screen);
    Store(base, node + 80, int32_t(Load8(base, screen + kLevel)) == int32_t(r3) ? 1 : 0);
  }
  return r3;
}

void SetRgb(uint8_t* base, uint32_t node, uint32_t r, uint32_t g, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_821E7DA0 UiNode::SetRgb");
  Store(base, node + 56, r);
  Store(base, node + 60, g);
  Store(base, node + 64, b);
}

}  // namespace tuning_screen
