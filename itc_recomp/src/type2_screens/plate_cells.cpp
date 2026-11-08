// The license plate editor's cells and menus
// (docs/functions/type2-screens.md): see type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "car_dealer/car_dealer.h"
#include "type2_screens/internal.h"
#include "ui_movies/game.h"

namespace type2_screens {

namespace {

constexpr uint32_t kAll = 0xFFFFFFFFu;  // -1 (the sub-scene of the root)

// ShowRegionCells and ShowCharCells: the rows from the list's scroll on
// sub-scene `scene`'s nodes 0 and 2, `per_row` cells a row, `limit` cells
// in all, strings `first_string` +; `set_first` the text set before the
// shown words (ShowCharCells) or after them; the arrows by `up_any` (up
// when the scroll isn't 0, else when it's above 0) and the rows at the
// data's `rows`. The down arrow.
uint32_t ShowCells(uint8_t* base, uint32_t frame, uint32_t scene, uint32_t per_row,
                   int32_t limit, uint32_t first_string, uint32_t lr, bool set_first,
                   bool up_any, uint32_t rows, const Calls& calls) {
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const int32_t scroll = int32_t(Load(base, Load(base, kPlateList) + 4));
  const uint32_t a = ui_movies::FindNode(base, movie, scene, 0);
  const uint32_t b = ui_movies::FindNode(base, movie, scene, 2);
  int32_t k = int32_t(uint32_t(scroll) * per_row);
  for (int32_t i = 0; i < int32_t(Load(base, a + 104)); i++, k++) {
    const uint32_t cell = ui_movies::FindNode(base, movie, scene, Load(base, a + 100) + uint32_t(i));
    const uint32_t other = ui_movies::FindNode(base, movie, scene, Load(base, b + 100) + uint32_t(i));
    if (k < limit) {
      if (set_first) {
        calls.Call(kSetNodeText, frame, lr, movie, cell, uint32_t(k) + first_string);
      }
      Store(base, cell + 80, 1);
      Store(base, other + 80, 1);
      if (!set_first) {
        calls.Call(kSetNodeText, frame, lr, movie, cell, uint32_t(k) + first_string);
      }
    } else {
      Store(base, cell + 80, 0);
      Store(base, other + 80, 0);
    }
  }
  const uint32_t up = ui_movies::FindNode(base, movie, kAll, 8);
  const uint32_t down = ui_movies::FindNode(base, movie, kAll, 9);
  Store(base, up + 80, (up_any ? scroll != 0 : scroll > 0) ? 1 : 0);
  const int32_t last = int32_t(Load(base, Load(base, kPlateData) + rows) - 3);
  Store(base, down + 80, (up_any ? scroll != last : scroll < last) ? 1 : 0);
  return down;
}

// MenuList::Init over the plate editor's list: `rows` rows, 3 shown, the
// actions 154, 153, 155, 156 in r7 to r10 and 159, 158 at the frame's +84
// and +92.
void InitList(uint8_t* base, uint32_t frame, uint32_t lr, uint64_t rows, const Calls& calls) {
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  const uint64_t args[8] = {Load(base, kPlateList), rows, 3, 0, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, lr, args);
}

// The field's row selected (SetSelection with `row` in r4, Fit 1) from the
// frame.
void SelectRow(uint8_t* base, uint32_t frame, uint32_t lr_select, uint32_t lr_fit, uint64_t row,
               const Calls& calls) {
  calls.Call(kListSetSelection, frame, lr_select, Load(base, kPlateList), row);
  calls.Call(kListFit, frame, lr_fit, Load(base, kPlateList), 1);
}

}  // namespace

uint32_t PlateShowRegionCells(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821990D0 PlateEditor::ShowRegionCells");
  const uint32_t frame = Frame(base, sp, 176);
  return ShowCells(base, frame, 3, 3, 14, 5411, 0x82199198, false, false, 64, calls);
}

uint32_t PlateShowCharCells(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199488 PlateEditor::ShowCharCells");
  const uint32_t frame = Frame(base, sp, 176);
  return ShowCells(base, frame, 5, 10, 38, 5425, 0x82199548, true, true, 68, calls);
}

uint32_t PlateShowDigitCells(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199350 PlateEditor::ShowDigitCells");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t parent = ui_movies::FindNode(base, movie, 4, 0);
  for (int32_t i = 0; i < int32_t(Load(base, parent + 104)); i++) {
    const uint32_t child = Load(base, parent + 100) + uint32_t(i);
    if (i < 10) {
      const uint64_t args[8] = {movie, 4, child, 8, uint64_t(i), 0, 0, 0};
      calls.CallArgs(kSetText, frame, 0x821993C4, args);
    } else {
      calls.Call(kClearText, frame, 0x821993CC, movie, 4, child);
    }
  }
  const uint32_t up = ui_movies::FindNode(base, movie, kAll, 8);
  const uint32_t down = ui_movies::FindNode(base, movie, kAll, 9);
  Store(base, up + 80, 0);
  Store(base, down + 80, 0);
  return down;
}

uint32_t PlateShowCharLine(uint8_t* base, uint32_t kind, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82199688 PlateEditor::ShowCharLine");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t parent = ui_movies::FindNode(base, movie, 4, 2);
  if (kind >= 3) {
    return parent;
  }
  uint32_t r3 = parent;
  for (int32_t i = 0; i < int32_t(Load(base, parent + 104)); i++) {
    const uint32_t child = ui_movies::FindNode(base, movie, 4, Load(base, parent + 100) + uint32_t(i));
    const bool marked = kind == 0 ? i == 0 : kind == 1 ? i == 10 : (i == 10 || i == 0);
    ui_movies::SetColor(base, child, marked ? 2 : 0);
    r3 = movie;
  }
  return r3;
}

uint32_t PlateShowPlate(uint8_t* base, uint32_t plate, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82199A20 PlateEditor::ShowPlate");
  const uint32_t frame = Frame(base, sp, 144);
  for (uint32_t i = 0; i < 3; i++) {
    Store(base, frame + 80 + 4 * i, uint32_t(int32_t(int8_t(guest::LoadU8(base, plate + 2 + i)))));
  }
  for (uint32_t i = 0; i < 7; i++) {
    Store(base, frame + 96 + 4 * i, uint32_t(int32_t(int8_t(guest::LoadU8(base, plate + 5 + i)))));
  }
  PlateShowRegion(base, uint32_t(int32_t(int8_t(guest::LoadU8(base, plate + 1)))), frame);
  PlateShowDigits(base, frame + 80, frame);
  return PlateShowChars(base, frame + 96, frame);
}

uint64_t PlateSetUpRegionMenu(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199C10 PlateEditor::SetUpRegionMenu");
  const uint32_t frame = Frame(base, sp, 128);
  InitList(base, frame, 0x82199C64, Load(base, Load(base, kPlateData) + 64), calls);
  const uint32_t data = Load(base, kPlateData);
  const int32_t field = int8_t(guest::LoadU8(base, data + 5));
  const int32_t row = field / 3;
  Store(base, data + 52, uint32_t(field - row * 3));
  SelectRow(base, frame, 0x82199C90, 0x82199C9C, uint32_t(row), calls);
  PlateMarkField(base, 0, frame);
  PlateShowRegionCells(base, frame, calls);
  return car_dealer::HelpLineSet(base, 568, frame, calls);
}

uint64_t PlateSetUpNumberMenu(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199CC8 PlateEditor::SetUpNumberMenu");
  const uint32_t frame = Frame(base, sp, 128);
  InitList(base, frame, 0x82199D14, 3, calls);
  const uint32_t data = Load(base, kPlateData);
  const int32_t field = int8_t(guest::LoadU8(base, data + Load(base, data + 56) + 6));
  const int32_t row = field / 4;
  Store(base, data + 52, uint32_t(field) - (uint32_t(row) << 2));
  // srawi and addze: the row sign-extended in r4.
  SelectRow(base, frame, 0x82199D50, 0x82199D5C, uint64_t(int64_t(row)), calls);
  PlateMarkField(base, Load(base, Load(base, kPlateData) + 56) + 1, frame);
  return PlateShowDigitCells(base, frame, calls);
}

uint64_t PlateSetUpCharsMenu(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199D88 PlateEditor::SetUpCharsMenu");
  const uint32_t frame = Frame(base, sp, 128);
  InitList(base, frame, 0x82199DDC, Load(base, Load(base, kPlateData) + 68), calls);
  const uint32_t data = Load(base, kPlateData);
  const int32_t field = int8_t(guest::LoadU8(base, data + Load(base, data + 56) + 9));
  const int32_t row = field / 10;
  Store(base, data + 52, uint32_t(field - row * 10));
  SelectRow(base, frame, 0x82199E10, 0x82199E1C, uint32_t(row), calls);
  PlateMarkField(base, Load(base, Load(base, kPlateData) + 56) + 4, frame);
  return PlateShowCharCells(base, frame, calls);
}

}  // namespace type2_screens
