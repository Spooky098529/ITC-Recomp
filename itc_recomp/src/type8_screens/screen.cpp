// The type 8 panel screen's constructor, destructors and car pick
// (docs/functions/type8-screens.md): see type8_screens.h. Read from the
// generated code.

#include "type8_screens/type8_screens.h"

#include "archive/archive.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "career/career.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "save_data/save_data.h"
#include "schema/car_list.h"
#include "schema/car_makers.h"
#include "schema/career.h"
#include "schema/type8_screens.h"
#include "screen_constructors/screen_constructors.h"

namespace type8_screens {

namespace {

using guest::LoadU16;
using guest::LoadU32;
using guest::StoreU32;

namespace ts = type67_screens;
using G = schema::Type8ScreensGlobals;

constexpr uint32_t kGarageCarReset = 0x82110448;
constexpr uint32_t kMemcpy = 0x825A7410;
// The two-player save block's halves and a garage car's size.
constexpr uint32_t kHalfBytes = 120108;
constexpr uint32_t kCarBytes = 1968;
constexpr uint32_t kGarageCarBytes = 1972;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

}  // namespace

uint64_t Construct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822386F0 Type8PanelScreen::Type8PanelScreen");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 224);
  const schema::Type8PanelScreen s(base, screen);
  screen_constructors::MachineScreen(base, screen);
  const uint32_t group = s.group_address();
  s.set_vtable(kScreenVtable);
  for (uint32_t player = 0; player < 2; player++) {
    Panel8FConstruct(base, screen + kPanelsF[player], player, group, frame);
    Panel8EConstruct(base, screen + kPanelsE[player], player, group, frame);
    Panel8CConstruct(base, screen + kPanelsC[player], player, group, frame);
    Panel8DConstruct(base, screen + kPanelsD[player], player, group, frame);
    ConfirmPanel8Construct(base, screen + kConfirms[player], player, group, frame);
  }
  PanelGroup8Construct(base, group, 4, frame);
  // PanelSetPair's constructor: r4 to r10, then its stack words.
  const uint32_t pair = s.pair_address();
  StoreU32(base, frame + 108, group);
  StoreU32(base, frame + 100, screen + kConfirms[1]);
  StoreU32(base, frame + 92, screen + kConfirms[0]);
  StoreU32(base, frame + 84, screen + kPanelsC[1]);
  car_list::PairConstruct(base, pair, screen + kPanelsF[0], screen + kPanelsF[1],
                          screen + kPanelsE[0], screen + kPanelsE[1], screen + kPanelsD[0],
                          screen + kPanelsD[1], screen + kPanelsC[0], frame);
  const uint32_t holder = s.group_holder_address();
  ts::SingleGroupConstruct(base, holder, group);
  s.set_player_node(11);
  s.set_player_child(4);
  s.set_car_list(pair);
  s.set_holder(holder);
  s.set_opens_itself(1);
  s.set_open_anim(376);
  s.set_loop_anim(354);
  s.set_list0_page_menu_player(0);
  s.set_list1_page_menu_player(1);
  s.set_list0_rows_shown(0);
  s.set_list1_rows_shown(0);
  s.set_list0_unknown_1920(0);
  s.set_list1_unknown_1920(0);
  s.set_list0_busy_anim_a(360);
  s.set_list0_busy_anim_b(359);
  s.set_list0_busy_anim_done(361);
  s.set_list1_busy_anim_a(363);
  s.set_list1_busy_anim_b(362);
  s.set_list1_busy_anim_done(364);
  s.set_message2_got(0);
  s.set_load_asked(0);
  s.set_opening_played(0);
  s.set_loaded_this_visit(0);
  s.set_draw_mode(0);
  s.set_dialog_answer(0xFFFFFFFFu);
  s.set_unknown_15248(0);
  return r3;
}

uint64_t Destruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238368 Type8PanelScreen::~Type8PanelScreen");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  const schema::Type8PanelScreen s(base, screen);
  s.set_vtable(kScreenVtable);
  ts::GroupPairDestruct(base, r3 + kHolder);
  car_list::PairDestruct(base, r3 + kCarList, frame, CarListCalls(calls));
  car_list::GroupBaseDestruct(base, r3 + kGroup);
  s.set_confirm1_vtable(kEmptyBaseVtable);
  archive::NodeDestruct(base, screen + kPanelsD[1]);
  car_list::CarPanelBaseDestruct(base, r3 + kPanelsC[1]);
  // The E panels' destructor is the empty sub_82106540.
  archive::NodeDestruct(base, screen + kPanelsF[1]);
  s.set_confirm0_vtable(kEmptyBaseVtable);
  archive::NodeDestruct(base, screen + kPanelsD[0]);
  car_list::CarPanelBaseDestruct(base, r3 + kPanelsC[0]);
  archive::NodeDestruct(base, screen + kPanelsF[0]);
  return ts::MachineScreenDestruct(base, r3);
}

uint64_t DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822388D8 Type8PanelScreen::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  Destruct(base, r3, frame, calls);
  if (flags & 1) {
    const uint64_t args[6] = {r3, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x82238908);
  }
  return r3;
}

uint64_t PickCar(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238C88 Type8PanelScreen::PickCar");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 160);
  const schema::Type8PanelScreen s(base, screen);
  for (uint32_t player = 0; player < 2; player++) {
    // The player's car list base (r30 in the original its +236).
    const schema::PanelSetBase list(base, s.pair_address() + 4 + kGarageCarBytes * player);
    const uint32_t car = G::garage_cars_address() + kGarageCarBytes * player;
    const uint32_t step = list.scroll() + list.row();
    const uint32_t entries = list.current_list().address;
    const uint32_t id = schema::CarListEntry(base, LoadU32(base, entries + (step << 2))).car_id();
    // GarageCar::Reset sets r4 before reading it: only r3 passed.
    const uint64_t reset[6] = {car, 0, 0, 0, 0, 0};
    calls.call_set(kGarageCarReset, reset, 1, frame, 0x82238CD4);
    const uint32_t tab = list.tab();
    if (tab < 2) {
      const uint32_t index = list.color_scroll() + list.color();
      career::CarSetModelAndClass(base, car, id, frame);
      const uint16_t half = uint16_t(index);
      const schema::CarMakersCar head(base, car);
      if (head.body_class() == 0xFFFF) {
        head.set_body_class(half);
      }
      const uint16_t old = head.paint_pick();
      head.set_paint_pick(half);
      schema::GarageCarSlot(base, car).set_unknown_1968(0);
      head.set_unknown_2(old);
    } else if (tab == 2) {
      // The player's half of the two-player save block: its garage's car
      // `step` links on from the first.
      const schema::CareerGarageList garage_list(
          base, save_data::TwoPlayerBlock(base) + kHalfBytes * player + 6120);
      const uint32_t garage = garage_list.address;
      uint32_t node = garage + (uint32_t(int32_t(int16_t(garage_list.head()))) << 2);
      for (uint32_t n = step; n != 0; n--) {
        node = garage + (uint32_t(int32_t(int16_t(LoadU16(base, node)))) << 2);
      }
      const uint32_t slot = uint32_t(int32_t(int16_t(LoadU16(base, node + 2))));
      const uint32_t from = slot * kCarBytes + garage_list.cars_address();
      if (!c_runtime::Memcpy(base, car, from, kCarBytes)) {
        const uint64_t args[6] = {car, from, kCarBytes, 0, 0, 0};
        calls.call_set(kMemcpy, args, 3, frame, 0x82238D38);
      }
      schema::GarageCarSlot(base, car).set_unknown_1968(1);
    }
  }
  const uint32_t passes = G::fade_out_passes(base);
  mode_tick::ScreenFadesFadeOut(base, passes, 0, 0, 0, 0);
  calls.set_r4(0);
  s.set_loaded_this_visit(0);
  s.set_draw_mode(2);
  return passes;
}

}  // namespace type8_screens
