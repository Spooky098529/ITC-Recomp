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

void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  Store(sp - bytes, sp);
  return sp - bytes;
}

constexpr uint32_t kAll = 0xFFFFFFFFu;  // sub-scene -1: the scene itself

}  // namespace

uint32_t ShowInteriorPicture(uint32_t screen, uint32_t type, uint32_t index,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DA3F8 TuningScreen::ShowInteriorPicture");
  const uint32_t movie = Movie(Frame(sp, 128));
  const uint32_t node = ui_movies::FindNode(movie, kAll, 0);
  if (type == 0) {
    UiNode(node).set_visible(0);
    return node;
  }
  UiNode(node).set_visible(1);
  const uint32_t picture = ui_movies::Child(ui_movies::FindNode(movie, kAll, 0), 0);
  switch (uint32_t(TS(screen).type())) {
    case 1: UiNode(picture).set_texture_entry(index); break;
    case 2: UiNode(picture).set_texture_entry(index + 4); break;
    case 3: UiNode(picture).set_texture_entry(index + 8); break;
    case 4: UiNode(picture).set_texture_entry(index + 11); break;
    default: break;
  }
  return picture;
}

uint32_t HideTireMarks(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DFB20 TuningScreen::HideTireMarks");
  const uint32_t movie = Movie(Frame(sp, 128));
  uint32_t child = 0;
  for (const uint32_t sub_scene : {30u, 33u}) {
    const uint32_t node = ui_movies::FindNode(movie, sub_scene, 0);
    for (const uint32_t k : {11u, 10u, 1u}) {
      child = ui_movies::Child(node, k);
      UiNode(child).set_visible(0);
    }
  }
  return child;
}

uint64_t HideNode19(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DFBE8 TuningScreen::HideNode19");
  (void)r3;
  const uint32_t screen = uint32_t(r3);
  const uint32_t movie = Movie(Frame(sp, 112));
  const int32_t state = int32_t(TS(screen).state());
  const int32_t category = int32_t(TS(screen).category());
  const uint32_t item = uint32_t(TS(screen).item());
  bool hide = false;
  if (state == 5) {
    hide = category == 3 || uint32_t(TS(screen).level()) == 0;
  } else if (state == 6) {
    if (category == 4 && item == 3) {
      const bool set = int32_t(TS(screen).aero3_type_remap()) != 0;
      const uint32_t type = uint32_t(TS(screen).type());
      hide = type == 0 || (set && type == 1);
    } else if (category == 0 && item == 2) {
      hide = uint32_t(TS(screen).level()) == 0;
    } else {
      hide = uint32_t(TS(screen).type()) == 0;
    }
  } else if (state == 8) {
    hide = category == 5 && item == 1 && uint32_t(TS(screen).option()) == 0;
  } else if (state == 54) {
    hide = int32_t(TS(screen).interior_pick()) == 0;
  } else if (state == 56) {
    hide = int32_t(TS(screen).interior_page_mode()) == 4;
  } else {
    hide = state == 9 || state == 10 || state == 11 || state == 14 || state == 15;
  }
  if (!hide) {
    return movie;
  }
  const uint32_t node = ui_movies::FindNode(movie, kAll, 19);
  UiNode(node).set_visible(0);
  return node;
}

uint32_t ShowTireMarks(uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E22E8 TuningScreen::ShowTireMarks");
  const uint32_t frame = Frame(sp, 176);
  const uint32_t movie = Movie(frame);
  auto child = [&](uint32_t sub_scene, uint32_t k) {
    return ui_movies::Child(ui_movies::FindNode(movie, sub_scene, 0), k);
  };
  auto tire_state = [&](bool with_5) {
    const int32_t s = int32_t(TS(screen).state());
    return ((with_5 && s == 5) || s == 8 || s == 9 || s == 10 || s == 11) &&
           int32_t(TS(screen).category()) == 3;
  };
  uint32_t r3 = child(30, 2);
  UiNode(r3).set_visible(1);
  if (tire_state(true)) {
    if (uint32_t(TS(screen).item()) == 0) {
      r3 = child(30, 1);
      UiNode(r3).set_visible(int32_t(TS(screen).tire_front_choice()) ==
                    int32_t(TS(screen).tire_rear_choice())
                ? 0
                : 1);
      r3 = child(33, 1);
      const bool differs = int32_t(TS(screen).tire_front_option()) !=
                               int32_t(TS(screen).tire_rear_option()) ||
                           int32_t(TS(screen).tire_front_choice()) !=
                               int32_t(TS(screen).tire_rear_choice());
      UiNode(r3).set_visible(differs ? 1 : 0);
    } else {
      r3 = child(30, 1);
      UiNode(r3).set_visible(0);
      r3 = child(33, 1);
      UiNode(r3).set_visible(0);
    }
  } else {
    r3 = HideTireMarks(frame);
  }
  if (!tire_state(false)) {
    return r3;
  }
  if (uint32_t(TS(screen).item()) == 0) {
    const uint32_t a = ui_movies::FindNode(movie, 30, 0);
    r3 = ui_movies::Child(a, 2);
    UiNode(r3).set_visible(0);
    r3 = ui_movies::Child(a, 1);
    UiNode(r3).set_visible(0);
  }
  const uint32_t item = uint32_t(TS(screen).item());
  if (item == 0) {
    r3 = child(33, 2);
    UiNode(r3).set_visible(int32_t(uint32_t(TS(screen).level())) == int32_t(TS(screen).tire_front_choice())
            ? 1
            : 0);
    r3 = child(33, 1);
    const int32_t level = int32_t(uint32_t(TS(screen).level()));
    const int32_t rear = int32_t(TS(screen).tire_rear_choice());
    bool shown = false;
    if (level == rear) {
      shown = int32_t(TS(screen).tire_front_option()) !=
                  int32_t(TS(screen).tire_rear_option()) ||
              int32_t(TS(screen).tire_front_choice()) != rear;
    }
    UiNode(r3).set_visible(shown ? 1 : 0);
  } else if (item == 1 || item == 2) {
    const uint32_t node = child(33, 2);
    r3 = GetCurrentChoice(screen);
    UiNode(node)
        .set_visible(int32_t(uint32_t(TS(screen).level())) == int32_t(r3) ? 1 : 0);
  }
  return r3;
}

void SetRgb(uint32_t node, uint32_t r, uint32_t g, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_821E7DA0 UiNode::SetRgb");
  UiNode(node).set_red(r);
  UiNode(node).set_green(g);
  UiNode(node).set_blue(b);
}

}  // namespace tuning_screen
