// The car dealer's menu openers (docs/functions/car-dealer.md): see
// car_dealer.h.

#include "car_dealer/car_dealer.h"

#include "base/log.h"
#include "car_dealer/internal.h"
#include "guest.h"
#include "ui_movies/game.h"

#include "base/float_mode.h"

#include <climits>
#include <cmath>

namespace car_dealer {

namespace {

constexpr uint32_t kAll = 0xFFFFFFFFu;  // -1 (the sub-scene of the root)

// MenuList::Init over `list` with the menu actions the dealer passes: r7 to
// r10 given, 159 and 158 stored at the frame's +84 and +92 first.
void InitList(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr, uint32_t list,
              uint64_t count, uint64_t shown, uint32_t r7, uint32_t r8, uint32_t r9,
              uint32_t r10) {
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  const uint64_t args[8] = {list, count, shown, 1, r7, r8, r9, r10};
  calls.CallArgs(kListInit, frame, lr, args);
}

// fctiwz as the translated code does it: NaN and below INT_MIN 0x80000000,
// INT_MAX and above INT_MAX, else truncated; the low word.
uint32_t Fctiwz(double value) {
  if (std::isnan(value)) return 0x80000000u;
  if (value >= double(INT32_MAX)) return uint32_t(INT32_MAX);
  if (value < double(INT32_MIN)) return 0x80000000u;
  return uint32_t(int32_t(value));
}

// A 64-bit integer stored big-endian at `at` (the original's std before
// each conversion).
void StoreWide(uint8_t* base, uint32_t at, int64_t value) {
  Store(base, at, uint32_t(uint64_t(value) >> 32));
  Store(base, at + 4, uint32_t(uint64_t(value)));
}

uint64_t SetText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                 uint64_t sub_scene, uint64_t child, uint64_t text) {
  return calls.Call(kSetText, frame, lr, movie, sub_scene, child, text);
}

}  // namespace

uint64_t OpenListMenu(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184338 CarDealer::OpenListMenu");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = FindMovie(base, 4, frame + 96);
  InitList(base, calls, frame, 0x8218439C, G::list_menu(base).address, 2, 2, 154, 153, 155, 156);
  calls.Call(kListSetSelection, frame, 0x821843B0, G::list_menu(base).address,
             G::data(base).kind_selection());
  SetKindBar(base, uint64_t(G::data(base).kind_selection()) * 40, frame);
  SetText(calls, frame, 0x821843DC, movie, 8, 5, 124);
  SetText(calls, frame, 0x821843DC, movie, 8, 6, 125);
  const uint32_t second = ui_movies::FindNode(base, movie, 11, 1);
  const uint32_t first = ui_movies::FindNode(base, movie, 11, 0);
  const uint32_t picked = uint32_t(calls.Call(kListSelection, frame, 0x82184418, G::list_menu(base).address));
  if (picked <= 1) {
    Node(base, second).set_texture_entry(picked);
    Node(base, first).set_texture_entry(picked);
  }
  const uint64_t selection = calls.Call(kListSelection, frame, 0x82184440, G::list_menu(base).address);
  uint64_t text = selection + 550;
  if (uint32_t(selection) <= 1) {
    const Data data = G::data(base);
    const uint32_t count =
        (uint32_t(selection) == 0 ? data.first_list() : data.second_list()).count();
    if (count == 0) {
      text = 583;  // the list empty
    }
  }
  return HelpLineSet(base, text, frame, calls);
}

uint64_t OpenCarList(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184488 CarDealer::OpenCarList");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = FindMovie(base, 4, frame + 96);
  const uint32_t n = G::data(base).shown_list().count();
  const bool few = int32_t(n) < 4;
  const uint64_t shown = few ? n : 4;
  InitList(base, calls, frame, few ? 0x82184504 : 0x82184548, G::list_menu(base).address, n, shown,
           154, 153, 155, 156);
  InitList(base, calls, frame, few ? 0x82184534 : 0x82184578, G::color_menu(base).address, n, shown,
           162, 161, 153, 154);
  calls.Call(kListSetSelection, frame, 0x82184588, G::list_menu(base).address,
             G::data(base).car_selection());
  G::list_menu(base).set_scroll(G::data(base).car_scroll());
  calls.Call(kListFit, frame, 0x821845A4, G::list_menu(base).address, 1);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t card = ui_movies::FindNode(base, movie, 7, i + 5);
    Node(base, card).set_visible(int32_t(i) < int32_t(n) ? 1 : 0);
  }
  const uint32_t more = ui_movies::FindNode(base, movie, 7, 1);
  if (int32_t(n) > 4) {
    Node(base, more).set_visible(1);
    SetScrollArrows(base, G::list_menu(base).scroll(), n, frame);
  } else {
    Node(base, more).set_visible(0);
  }
  calls.Call(kCarCardsFillAll, frame, 0x82184618);
  const uint64_t selection = calls.Call(kListSelection, frame, 0x82184620, G::list_menu(base).address);
  const uint32_t index = (G::list_menu(base).scroll() + uint32_t(selection)) << 2;
  const Entry entry(base, uint32_t(G::data(base).shown_list().load_at(index, 4)));
  return HelpLineSet(base, uint64_t(entry.car_id()) + 1862, frame, calls);
}

uint64_t OpenCar(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184650 CarDealer::OpenCar");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, 4, frame + 96);
  InitList(base, calls, frame, 0x821846B4, G::list_menu(base).address, 2, 2, 154, 153, 155, 156);
  calls.Call(kListSetSelection, frame, 0x821846CC, G::color_menu(base).address,
             G::data(base).car_selection());
  G::color_menu(base).set_scroll(G::data(base).car_scroll());
  calls.Call(kListFit, frame, 0x821846E8, G::color_menu(base).address, 1);
  const uint64_t selection = calls.Call(kListSelection, frame, 0x821846F0, G::list_menu(base).address);
  SetKindBar(base, selection * 40, frame);
  SetText(calls, frame, 0x8218470C, movie, 8, 5, 127);
  SetText(calls, frame, 0x82184720, movie, 8, 6, 112);
  {
    const Data data = G::data(base);
    ShowCarInfo(base, uint64_t(data.car_scroll()) + data.car_selection(), frame, calls);
  }
  const uint32_t shown = G::data(base).shown_list().count() == 1 ? 0 : 1;
  Node(base, ui_movies::FindNode(base, movie, kAll, 23)).set_visible(shown);
  Node(base, ui_movies::FindNode(base, movie, kAll, 24)).set_visible(shown);
  return calls.Call(kStageSetPlates, frame, 0x82184798, Stage(base), 0);
}

uint64_t OpenColors(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821847A0 CarDealer::OpenColors");
  using Fpscr = ::base::FloatMode;
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
  const uint32_t frame = Frame(base, sp, 240);
  const uint32_t movie = FindMovie(base, 4, frame + 96);
  Entry entry;
  {
    const Data data = G::data(base);
    const uint32_t index = (data.car_scroll() + data.car_selection()) << 2;
    entry = Entry(base, uint32_t(data.shown_list().load_at(index, 4)));
  }
  const uint32_t n = entry.color_count();
  const bool over = int32_t(n) > 9;
  InitList(base, calls, frame, over ? 0x82184838 : 0x82184858, G::list_menu(base).address, n,
           over ? 9 : n, 154, 153, 155, 156);
  calls.Call(kListSetSelection, frame, 0x82184868, G::list_menu(base).address,
             G::data(base).color_selection());
  G::list_menu(base).set_scroll(G::data(base).color_scroll());
  calls.Call(kListFit, frame, 0x82184884, G::list_menu(base).address, 1);
  const uint64_t selection = calls.Call(kListSelection, frame, 0x8218488C, G::list_menu(base).address);
  SetColorBar(base, uint64_t(uint32_t(selection) << 5) + 3, frame);
  Node(base, ui_movies::FindNode(base, movie, kAll, 25)).set_visible(over ? 1 : 0);
  // The old color records freed, from the last.
  while (G::data(base).colors().count() != 0) {
    const Colors colors = G::data(base).colors();
    const uint32_t last = colors.count() - 1;
    colors.set_count(last);
    calls.Call(kVirtualFree, frame, 0x821848F0, uint32_t(colors.load_at(last << 2, 4)), 0,
               0x8000);
  }
  if (int32_t(n) > 0) {
    // Each of the entry's colors (CarDealerSpecColor) into a 32-byte record:
    // +16 to +24 the HSV floats, +0 to +8 the RGB integers.
    const double scale = double(guest::LoadFloat(base, 0x82000C44));
    const double unit = double(guest::LoadFloat(base, 0x82002200));
    SpecColor source = entry.color_records(0);
    for (uint32_t left = n; left != 0; left--, source.address += SpecColor::kSize) {
      // A host object (E5 step 4) in place of the VirtualAlloc(32) at
      // 0x8218493C.
      const Swatch record = Swatch::New(base);
      const int64_t hue = int64_t(int32_t(source.hue()));
      StoreWide(base, frame + 96, hue);
      record.set_hue(float(double(float(double(hue)))));
      const int64_t saturation = source.saturation();
      StoreWide(base, frame + 104, saturation);
      const double s = double(float(double(saturation)));
      record.set_saturation(float(double(float(s * unit))));
      const uint32_t kind = source.kind();
      const uint32_t add = Load(base, kind == 1 ? 0x82002FF4 : 0x82002FF8);
      int32_t value = int32_t(source.value() + add);
      if (value < 0) {
        value = 0;
      } else if (value > 255) {
        value = 255;
      }
      StoreWide(base, kind == 1 ? frame + 112 : frame + 120, value);
      const double v = double(float(double(int64_t(value))));
      record.set_value(float(double(float(v * unit))));
      // Color::HsvToRgb reads its input (the record's +16 to +31) from guest
      // memory: staged below `frame` (its r1), where it copies the 16 bytes
      // itself.
      Store(base, frame - 16, record.hue_bits());
      Store(base, frame - 12, record.saturation_bits());
      Store(base, frame - 8, record.value_bits());
      Store(base, frame - 4, 0);
      calls.Call(kHsvToRgb, frame, 0x82184A00, frame + 128, frame - 16);
      const double red = double(float(double(guest::LoadFloat(base, frame + 128)) * scale));
      const double green = double(float(double(guest::LoadFloat(base, frame + 132)) * scale));
      const double blue = double(float(double(guest::LoadFloat(base, frame + 136)) * scale));
      record.set_red(Fctiwz(red));
      record.set_green(Fctiwz(green));
      record.set_blue(Fctiwz(blue));
      const Colors colors = G::data(base).colors();
      const uint32_t count = colors.count();
      colors.set_count(count + 1);
      colors.store_at(count << 2, 4, record.address);
    }
  }
  // Sub-scene 26's swatches shown for those below n.
  const uint32_t swatches = ui_movies::FindNode(base, movie, 26, 1);
  // The group's +104 and +100 read as words (its child count, first child).
  const Node group(base, swatches);
  for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, group.height_address())); i++) {
    const uint32_t swatch =
        ui_movies::FindNode(base, movie, 26, i + Load(base, group.width_address()));
    Node(base, swatch).set_visible(int32_t(i) < int32_t(n) ? 1 : 0);
  }
  ShowSwatches(base, frame);
  {
    const Data data = G::data(base);
    calls.Call(kShowCarById, frame, 0x82184AD4,
               uint64_t(data.color_scroll()) + data.color_selection());
  }
  return HelpLineSet(base, 552, frame, calls);
}

}  // namespace car_dealer
