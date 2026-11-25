// PanelSet's leaves (docs/functions/car-list.md): see car_list.h.

#include "car_list/car_list.h"

#include "archive/archive.h"
#include "base/log.h"
#include "guest.h"
#include "schema/car_list.h"
#include "screen_constructors/screen_constructors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace car_list {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::CarListGlobals;
using Base = schema::PanelSetBase;
using Set = schema::PanelSet;
using Pair = schema::PanelSetPair;
using Group = schema::PanelGroupBase;
using Confirm = schema::ConfirmPanel;
using Car = schema::CarPanel;
using Interface = schema::PanelSetInterface;

// PanelSet::Reset's stores on the base `b` (PanelSetPair's Reset and Open
// make them on both bases).
void ResetWords(uint32_t b_address) {
  const Base b(b_address);
  b.set_picked(0);
  b.set_done(0);
  b.set_power_graph_asked(0);
  b.set_show_stand_car(0);
  b.set_stand_model_loaded(0);
  b.set_may_leave(1);
  b.set_fade_with_steps(1);
  b.set_next_page(0xFFFFFFFFu);
  b.set_page(0xFFFFFFFFu);
  b.set_previous_page(0xFFFFFFFFu);
  b.set_step_cleared(0xFFFFFFFFu);
  b.set_step(0xFFFFFFFFu);
  b.set_first_pass(0);
  b.set_current_list(0);
  b.set_tab_shown(0);
  b.set_tab(0);
  b.set_row(0);
  b.set_scroll(0);
  b.set_color(0);
  b.set_color_scroll(0);
}

}  // namespace

uint64_t SetReset(uint64_t set) {
  ITC_NATIVE_RUNNING("sub_822496C8 PanelSet::Reset");
  ResetWords(uint32_t(set) + 4);
  return set;
}

uint64_t SetPicked(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82249848 PanelSet::Picked");
  if (Set(set).done() == 0) return 0;
  return Set(set).picked() != 0 ? 1 : 0;
}

uint64_t SetWentBack(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82249870 PanelSet::WentBack");
  if (Set(set).done() == 0) return 0;
  return Set(set).picked() == 0 ? 1 : 0;
}

uint64_t SetWantsPowerGraph(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82249898 PanelSet::WantsPowerGraph");
  if (Set(set).power_graph_asked() == 0) return 0;
  return Set(set).picked() != 0 ? 1 : 0;
}

uint64_t SetWord(uint64_t set, uint32_t offset, uint64_t value) {
  const Set s{uint32_t(set)};
  switch (offset) {
    case 1932:
      ITC_NATIVE_RUNNING("sub_8224B6D8 PanelSet::SetOpenKind");
      s.set_open_kind(uint32_t(value));
      break;
    case 1884:
      ITC_NATIVE_RUNNING("sub_8224B6E8 PanelSet::SetPreviousTab");
      s.set_previous_tab(uint32_t(value));
      break;
    case 1888:
      ITC_NATIVE_RUNNING("sub_8224B6F0 PanelSet::SetPreviousEntry");
      s.set_previous_entry(uint32_t(value));
      break;
    default:
      ITC_NATIVE_RUNNING("sub_8224B6F8 PanelSet::SetPreviousColor");
      s.set_previous_color(uint32_t(value));
      break;
  }
  return set;
}

uint64_t PickedPage(uint64_t b) {
  ITC_NATIVE_RUNNING("sub_82248EF0 PanelSetBase::PickedPage");
  Base(uint32_t(b)).set_done(1);
  return b;
}

uint64_t SaveData(uint32_t b, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82249668 PanelSetBase::SaveData");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  if (G::two_players() == 0) {
    return vehicle_accessors::SaveDataOf(0);
  }
  // mullw: the low words' signed product, 64 bits; add.
  const int64_t offset = int64_t(int32_t(Base(b).player())) * int64_t(120108);
  return uint64_t(G::pair_save_data().address) + uint64_t(offset);
}

// --- The destructors ---

uint64_t GroupBaseDestruct(uint64_t group) {
  ITC_NATIVE_RUNNING("sub_8224EF08 PanelGroupBase::~PanelGroupBase");
  Group(uint32_t(group)).set_vtable(Group::kVtable);
  archive::NodeDestruct(uint32_t(group));
  return group;
}

uint64_t SetBaseDestruct(uint64_t b, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224AE60 PanelSetBase::~PanelSetBase");
  // The original's 96-byte frame (only its back chain) left out: the guest
  // calls run from `sp` (E5 step 6).
  Base(uint32_t(b)).set_vtable(Base::kVtable);
  calls.Call(kMenuListRelease, sp, 0x8224AE88, b + 132);
  return calls.Call(kMenuListRelease, sp, 0x8224AE90, b + 60);
}

uint64_t SetDestruct(uint64_t set, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224B708 PanelSet::~PanelSet");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): the base's destructor runs from `sp`.
  Set(uint32_t(set)).set_vtable(Set::kVtable);
  const uint64_t r3 = SetBaseDestruct(set + 4, sp, calls);
  Interface(uint32_t(set)).set_vtable(Interface::kVtable);
  return r3;
}

uint64_t SetInterfaceDeletingDestruct(uint64_t set, uint64_t flags, uint32_t sp,
                                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224AC98 PanelSetInterface::DeletingDestructor");
  // The original's 96-byte frame (only its back chain) left out: the guest
  // call runs from `sp` (E5 step 6).
  Interface(uint32_t(set)).set_vtable(Interface::kVtable);
  if (flags & 1) {
    calls.Call(kOperatorDelete, sp, 0x8224ACC4, set, flags);
  }
  return set;
}

// --- PanelSetPair ---

uint64_t PairConstruct(uint64_t pair_r3, uint32_t f0, uint32_t f1, uint32_t e0,
                       uint32_t e1, uint32_t d0, uint32_t d1, uint32_t c0, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224B750 PanelSetPair::PanelSetPair");
  const uint32_t pair = uint32_t(pair_r3);
  // The original's 144-byte frame (only its back chain) left out (E5 step
  // 6): PanelSetBase makes no call and reads no stack. The stack arguments
  // below are the caller's.
  const Pair p(pair);
  p.set_vtable(Pair::kVtable);
  screen_constructors::PanelSetBase(pair + 4, sp);
  screen_constructors::PanelSetBase(Pair(uint32_t(pair)).second_base_address(), sp);
  const uint32_t confirm0 = LoadU32(sp + 92);
  const uint32_t c1 = LoadU32(sp + 84);
  const uint32_t confirm1 = LoadU32(sp + 100);
  G::set_two_players(1);
  const uint32_t group_b = LoadU32(sp + 108);
  // The panels get the word as it reads after each store (not checked for 0).
  p.set_first_tabs(f0);
  schema::TabPanel(f0).set_two_players(G::two_players());
  p.set_first_rows(e0);
  schema::RowPanel(e0).set_two_players(G::two_players());
  p.set_first_color_panel(d0);
  schema::ColorPanel(d0).set_two_players(G::two_players());
  p.set_first_car_panel(c0);
  Car(c0).set_two_players(G::two_players());
  p.set_first_confirm_panel(confirm0);
  p.set_first_group_b(group_b);
  p.set_second_tabs(f1);
  schema::TabPanel(f1).set_two_players(G::two_players());
  p.set_second_rows(e1);
  schema::RowPanel(e1).set_two_players(G::two_players());
  p.set_second_color_panel(d1);
  schema::ColorPanel(d1).set_two_players(G::two_players());
  p.set_second_car_panel(c1);
  Car(c1).set_two_players(G::two_players());
  p.set_second_confirm_panel(confirm1);
  p.set_second_group_b(group_b);
  return pair_r3;
}

uint64_t PairDestruct(uint64_t pair, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224B830 PanelSetPair::~PanelSetPair");
  // The original's 112-byte frame (only its back chain) left out (E5 step
  // 6): the bases' destructors run from `sp`.
  Pair(uint32_t(pair)).set_vtable(Pair::kVtable);
  SetBaseDestruct(Pair(uint32_t(pair)).second_base_address(), sp, calls);
  const uint64_t r3 = SetBaseDestruct(pair + 4, sp, calls);
  Interface(uint32_t(pair)).set_vtable(Interface::kVtable);
  return r3;
}

uint64_t PairDeletingDestruct(uint64_t pair, uint64_t flags, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CA30 PanelSetPair::DeletingDestructor");
  // The original's 112-byte frame (only its back chain) left out: its
  // calls run from `sp` (E5 step 6).
  PairDestruct(pair, sp, calls);
  if (flags & 1) {
    // r4 as ~PanelSetPair leaves it (operator delete doesn't read it).
    calls.Call(kOperatorDelete, sp, 0x8224CA60, pair, flags);
  }
  return pair;
}

uint64_t PairReset(uint64_t pair, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822498C0 PanelSetPair::Reset");
  // The words the original builds below its r1 (it has no frame; the
  // second base's address, the reset's values) are values since E5 step 6:
  // only it reads them.
  (void)sp;
  const uint32_t second = Pair(uint32_t(pair)).second_base_address();
  ResetWords(uint32_t(pair) + 4);
  ResetWords(second);
  return pair;
}

uint64_t PairOpen(uint64_t pair_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822499F0 PanelSetPair::Open");
  // The words on its 176-byte frame (the bases' reset's values, the second
  // base's address) and the pair it keeps in the caller's area are values
  // since E5 step 6: only it reads them. ToTabs runs from `sp`.
  const uint64_t b1 = pair_r3 + 4;
  const uint32_t b2 = Pair(uint32_t(pair_r3)).second_base_address();
  ResetWords(uint32_t(b1));
  ResetWords(b2);
  ToTabs(b1, sp, calls);
  const uint64_t r3 = ToTabs(b2, sp, calls);
  const Pair pair{uint32_t(pair_r3)};
  pair.set_first_player(0);
  pair.set_second_player(1);
  pair.set_first_may_leave(0);
  pair.set_second_may_leave(0);
  pair.set_first_fade_with_steps(0);
  pair.set_second_fade_with_steps(0);
  pair.set_first_cancelled(0);
  pair.set_second_cancelled(0);
  return r3;
}

uint64_t PairHandleInput(uint64_t pair, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CA80 PanelSetPair::HandleInput");
  // The original's 112-byte frame (only its back chain) left out (E5 step
  // 6): the bases' HandleInput runs from `sp`.
  G::set_stand_wait(1);
  HandleInput(pair + 4, sp, calls);
  return HandleInput(Pair(uint32_t(pair)).second_base_address(), sp, calls);
}

uint64_t PairUpdate(uint64_t pair, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CF20 PanelSetPair::Update");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): the bases' Update runs from `sp`.
  Update(pair + 4, sp, calls);
  return Update(Pair(uint32_t(pair)).second_base_address(), sp, calls);
}

uint64_t PairBuild(uint64_t pair, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CAE0 PanelSetPair::Build");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): the bases' Build runs from `sp`.
  Build(pair + 4, sp, calls);
  return Build(Pair(uint32_t(pair)).second_base_address(), sp, calls);
}

uint64_t PairPicked(uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_82249B60 PanelSetPair::Picked");
  const Pair p(pair);
  if (p.first_done() == 0 || p.second_done() == 0) return 0;
  return p.first_picked() != 0 && p.second_picked() != 0 ? 1 : 0;
}

// --- The C panels' base and the confirm panel ---

uint64_t CarPanelBaseDestruct(uint64_t panel) {
  ITC_NATIVE_RUNNING("sub_8224D7E0 CarPanelBase::~CarPanelBase");
  Car(uint32_t(panel)).set_vtable(kCarPanelBaseVtable);
  return panel;
}

uint64_t ConfirmPanelConstruct(uint64_t panel_r3) {
  ITC_NATIVE_RUNNING("sub_8224EBB0 ConfirmPanel::ConfirmPanel");
  const uint32_t panel = uint32_t(panel_r3);
  const Confirm c(panel);
  c.set_group(0);
  c.set_vtable(Confirm::kVtable);
  c.set_unknown_12(0xFFFFFFFFu);
  c.set_kind(2);
  c.set_node_a(0xFFFFFFFFu);
  c.set_node_b(0xFFFFFFFFu);
  c.set_node_c(0xFFFFFFFFu);
  c.set_show_anim(0xFFFFFFFFu);
  c.set_loop_anim(0xFFFFFFFFu);
  c.set_hide_anim(0xFFFFFFFFu);
  return panel_r3;
}

}  // namespace car_list
