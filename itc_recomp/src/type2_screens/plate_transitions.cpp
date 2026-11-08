// The license plate editor's transitions (docs/functions/type2-screens.md):
// see type2_screens.h. Each is called by PlateEditor::Update with the
// address of the page object's +24 (the started word) and answers whether
// it's done.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "car_dealer/car_dealer.h"
#include "race_screens/race_screens.h"
#include "type2_screens/internal.h"
#include "ui_movies/game.h"

namespace type2_screens {

namespace {

constexpr uint32_t kAll = 0xFFFFFFFFu;  // -1 (the sub-scene of the root)
// The float the highlights store into a node's +68 (lfs, stfs).
constexpr uint32_t kHighlight = 0x82000A30;

uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id) {
  return calls.Call(kPlayNamed, frame, lr, movie, id, 0, 0);
}
uint64_t Loop(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id) {
  return calls.Call(kLoopNamed, frame, lr, movie, id);
}
uint64_t Finish(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t id) {
  return calls.Call(kFinishNamed, frame, lr, movie, id);
}
uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}
// The selected cell's named animation: `first` + the selection x `per` +
// the column (the data's +52).
uint32_t Cell(uint8_t* base, uint32_t first, uint32_t per) {
  const uint32_t selection = race_screens::ListSelection(base, Load(base, kPlateList));
  return Load(base, Load(base, kPlateData) + 52) + selection * per + first;
}
// A node's +68 = the highlight float, through a single's double as the
// original's lfs and stfs.
void Highlight(uint8_t* base, uint32_t node) {
  guest::StoreFloat(base, node + 68, float(double(guest::LoadFloat(base, kHighlight))));
}
// The plate's bytes from the data's `from` as `count` words at `to`.
void PlateWords(uint8_t* base, uint32_t from, uint32_t count, uint32_t to) {
  const uint32_t data = Load(base, kPlateData);
  for (uint32_t i = 0; i < count; i++) {
    Store(base, to + 4 * i, uint32_t(int32_t(int8_t(guest::LoadU8(base, data + from + i)))));
  }
}

}  // namespace

uint64_t PlateTransition0(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219A3A8 PlateEditor::Transition0");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  if (Load(base, step) == 0) {
    Play(calls, frame, 0x8219A404, movie, 66);
    PlateSetUpRegionMenu(base, frame, calls);
    PlateShowPlate(base, Load(base, kPlateData) + 4, frame);
    Store(base, step, 1);
  }
  return uint32_t(Run(calls, frame, 0x8219A42C, movie)) == 0 ? 1 : 0;
}

uint64_t PlateTransition1(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198A40 PlateEditor::Transition1");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  if (Load(base, step) == 0) {
    const uint32_t cell = Cell(base, 2, 3);
    Play(calls, frame, 0x82198ABC, movie, cell);
    Loop(calls, frame, 0x82198AC8, movie, cell);
    Play(calls, frame, 0x82198AE4, movie, 0);
    Loop(calls, frame, 0x82198AF0, movie, 0);
    Play(calls, frame, 0x82198B0C, movie, 68);
    Loop(calls, frame, 0x82198B18, movie, 68);
    Highlight(base, ui_movies::FindNode(base, movie, 3, 1));
    Store(base, step, 1);
  }
  Run(calls, frame, 0x82198B48, movie);
  return 1;
}

uint64_t PlateTransition4(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198C20 PlateEditor::Transition4");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  if (Load(base, step) == 0) {
    Finish(calls, frame, 0x82198C90, movie, Cell(base, 2, 3));
    Finish(calls, frame, 0x82198C9C, movie, 0);
    Finish(calls, frame, 0x82198CA8, movie, 68);
    calls.Call(kFinishAll, frame, 0x82198CB0, movie);
    Play(calls, frame, 0x82198CCC, movie, 67);
    Store(base, step, 1);
  }
  return uint32_t(Run(calls, frame, 0x82198CE0, movie)) == 0 ? 1 : 0;
}

uint64_t PlateTransition5(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219A5E8 PlateEditor::Transition5");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  if (Load(base, step) == 0) {
    PlateFinishRegionAnim(base, frame, calls);
    Play(calls, frame, 0x8219A640, movie, 69);
    Store(base, Load(base, kPlateData) + 56, 0);
    PlateSetUpNumberMenu(base, frame, calls);
    car_dealer::HelpLineSet(base, 569, frame, calls);
    PlateWords(base, 6, 3, frame + 88);
    PlateShowDigits(base, frame + 88, frame);
    Store(base, step, 1);
  }
  return uint32_t(Run(calls, frame, 0x8219A6A4, movie)) == 0 ? 1 : 0;
}

uint64_t PlateTransition6(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198D00 PlateEditor::Transition6");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  if (Load(base, step) == 0) {
    const uint32_t cell = Cell(base, 15, 4);
    Play(calls, frame, 0x82198D7C, movie, cell);
    Loop(calls, frame, 0x82198D88, movie, cell);
    Highlight(base, ui_movies::FindNode(base, movie, 4, 1));
    Store(base, step, 1);
  }
  Run(calls, frame, 0x82198DB8, movie);
  return 1;
}

uint64_t PlateTransition8(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219A820 PlateEditor::Transition8");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  if (Load(base, step) == 0) {
    Finish(calls, frame, 0x8219A888, movie, Cell(base, 15, 4));
    PlateSetUpNumberMenu(base, frame, calls);
    PlateWords(base, 6, 3, frame + 88);
    PlateShowDigits(base, frame + 88, frame);
    Store(base, step, 1);
  }
  Run(calls, frame, 0x8219A8D4, movie);
  return 1;
}

uint64_t PlateTransition10(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219AA00 PlateEditor::Transition10");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  if (Load(base, step) == 0) {
    PlateFinishDigitAnim(base, frame, calls);
    Store(base, Load(base, kPlateData) + 56, 0);
    PlateSetUpCharsMenu(base, frame, calls);
    Play(calls, frame, 0x8219AA6C, movie, 71);
    car_dealer::HelpLineSet(base, 571, frame, calls);
    Store(base, step, 1);
  }
  const uint32_t done = uint32_t(Run(calls, frame, 0x8219AA88, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    PlateShowCharLine(base, Load(base, Load(base, kPlateData) + 60), frame);
  }
  return done;
}

uint64_t PlateTransition11(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198DC8 PlateEditor::Transition11");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  if (Load(base, step) == 0) {
    const uint32_t cell = Cell(base, 27, 10);
    Play(calls, frame, 0x82198E44, movie, cell);
    Loop(calls, frame, 0x82198E50, movie, cell);
    Store(base, step, 1);
  }
  Run(calls, frame, 0x82198E64, movie);
  return 1;
}

uint64_t PlateTransition13(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219AC88 PlateEditor::Transition13");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  if (Load(base, step) == 0) {
    Finish(calls, frame, 0x8219ACF0, movie, Cell(base, 27, 10));
    PlateSetUpCharsMenu(base, frame, calls);
    const uint32_t data = Load(base, kPlateData);
    PlateWords(base, 9, 7, frame + 96);
    Store(base, data + 60, PlateCharLineKind(base, frame + 96));
    PlateShowChars(base, frame + 96, frame);
    PlateShowCharLine(base, Load(base, Load(base, kPlateData) + 60), frame);
    Store(base, step, 1);
  }
  Run(calls, frame, 0x8219AD58, movie);
  return 1;
}

uint64_t PlateTransition19(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198FB8 PlateEditor::Transition19");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  if (Load(base, step) == 0) {
    race_screens::ListSelection(base, Load(base, kPlateList));
    calls.Call(kFinishAll, frame, 0x82199004, movie);
    uint32_t nodes[7];
    const uint32_t children[7] = {7, 6, 5, 3, 4, 8, 9};
    for (uint32_t i = 0; i < 7; i++) {
      nodes[i] = ui_movies::FindNode(base, movie, kAll, children[i]);
    }
    for (const uint32_t node : nodes) {
      Store(base, node + 80, 0);
    }
    Store(base, step, 1);
  }
  Run(calls, frame, 0x821990C0, movie);
  return 1;
}

}  // namespace type2_screens
