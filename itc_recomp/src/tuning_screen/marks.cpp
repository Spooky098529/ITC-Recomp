// The tuning screen's part list marks (docs/functions/tuning-screen.md): see
// tuning_screen.h. Movie 2's nodes through the UI movies' plain C++
// (UiMovies::Find, UiMovie::FindNode and Child).

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace tuning_screen {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  Store(sp - bytes, sp);
  return sp - bytes;
}

}  // namespace

uint32_t Movie(uint32_t frame) {
  Store(frame + 80, kMovieId);
  return Load(ui_movies::PairsFind(ui_movies::kMovieTable, frame + 80));
}

uint32_t HidePartMarks(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EE6D0 TuningScreen::HidePartMarks");
  const uint32_t movie = Movie(Frame(sp, 128));
  uint32_t child = 0;
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t node = ui_movies::FindNode(movie, 1, 14 + i);
    for (uint32_t k = 3; k <= 5; k++) {
      child = ui_movies::Child(node, k);
      UiNode(child).set_visible(0);
    }
  }
  return child;
}

uint64_t ShowPartMarks(uint64_t r3, uint32_t row, uint64_t group, uint64_t index,
                       uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EE760 TuningScreen::ShowPartMarks");
  Frame(sp, 128);
  const uint32_t screen = uint32_t(r3);
  const int32_t which = int32_t(TS(screen).wheel_part_cursor());
  const uint64_t number = WheelNumber(group, index);
  const bool front = int32_t(TS(screen).saved_wheel_front()) == int32_t(number);
  const bool rear = int32_t(TS(screen).saved_wheel_rear()) == int32_t(number);
  // Child 5 always, then 4 for the front wheel's, 3 for the rear's.
  auto mark = [&](uint32_t second) -> uint64_t {
    const uint32_t first = ui_movies::Child(row, 5);
    UiNode(first).set_visible(1);
    if (second == 0) {
      return first;
    }
    const uint32_t other = ui_movies::Child(row, second);
    UiNode(other).set_visible(1);
    return other;
  };
  if (which == 0) {
    if (front && rear) {
      return mark(0);
    }
    if (front) {
      return mark(4);
    }
    return rear ? mark(3) : number;
  }
  if (which == 1) {
    return front ? mark(4) : number;
  }
  if (which == 2) {
    return rear ? mark(3) : number;
  }
  return r3;
}

}  // namespace tuning_screen
