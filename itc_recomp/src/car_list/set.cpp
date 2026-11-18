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

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// PanelSet::Reset's stores on the base `b` (PanelSetPair's Reset and Open
// make them on both bases).
void ResetWords(uint8_t* base, uint32_t b_address) {
  const Base b(base, b_address);
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

uint64_t SetReset(uint8_t* base, uint64_t set) {
  ITC_NATIVE_RUNNING("sub_822496C8 PanelSet::Reset");
  ResetWords(base, uint32_t(set) + 4);
  return set;
}

uint64_t SetPicked(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82249848 PanelSet::Picked");
  if (Set(base, set).done() == 0) return 0;
  return Set(base, set).picked() != 0 ? 1 : 0;
}

uint64_t SetWentBack(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82249870 PanelSet::WentBack");
  if (Set(base, set).done() == 0) return 0;
  return Set(base, set).picked() == 0 ? 1 : 0;
}

uint64_t SetWantsPowerGraph(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82249898 PanelSet::WantsPowerGraph");
  if (Set(base, set).power_graph_asked() == 0) return 0;
  return Set(base, set).picked() != 0 ? 1 : 0;
}

uint64_t SetWord(uint8_t* base, uint64_t set, uint32_t offset, uint64_t value) {
  const Set s(base, uint32_t(set));
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

uint64_t PickedPage(uint8_t* base, uint64_t b) {
  ITC_NATIVE_RUNNING("sub_82248EF0 PanelSetBase::PickedPage");
  Base(base, uint32_t(b)).set_done(1);
  return b;
}

uint64_t SaveData(uint8_t* base, uint32_t b, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82249668 PanelSetBase::SaveData");
  StoreU32(base, sp - 96, sp);
  if (G::two_players(base) == 0) {
    return vehicle_accessors::SaveDataOf(base, 0);
  }
  // mullw: the low words' signed product, 64 bits; add.
  const int64_t offset = int64_t(int32_t(Base(base, b).player())) * int64_t(120108);
  return uint64_t(G::pair_save_data(base).address) + uint64_t(offset);
}

// --- The destructors ---

uint64_t GroupBaseDestruct(uint8_t* base, uint64_t group) {
  ITC_NATIVE_RUNNING("sub_8224EF08 PanelGroupBase::~PanelGroupBase");
  Group(base, uint32_t(group)).set_vtable(Group::kVtable);
  archive::NodeDestruct(base, uint32_t(group));
  return group;
}

uint64_t SetBaseDestruct(uint8_t* base, uint64_t b, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224AE60 PanelSetBase::~PanelSetBase");
  const uint32_t frame = sp - 96;
  StoreU32(base, frame, sp);
  Base(base, uint32_t(b)).set_vtable(Base::kVtable);
  calls.Call(kMenuListRelease, frame, 0x8224AE88, b + 132);
  return calls.Call(kMenuListRelease, frame, 0x8224AE90, b + 60);
}

uint64_t SetDestruct(uint8_t* base, uint64_t set, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224B708 PanelSet::~PanelSet");
  const uint32_t frame = sp - 96;
  StoreU32(base, frame, sp);
  Set(base, uint32_t(set)).set_vtable(Set::kVtable);
  const uint64_t r3 = SetBaseDestruct(base, set + 4, frame, calls);
  Interface(base, uint32_t(set)).set_vtable(Interface::kVtable);
  return r3;
}

uint64_t SetInterfaceDeletingDestruct(uint8_t* base, uint64_t set, uint64_t flags, uint32_t sp,
                                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224AC98 PanelSetInterface::DeletingDestructor");
  const uint32_t frame = sp - 96;
  StoreU32(base, frame, sp);
  Interface(base, uint32_t(set)).set_vtable(Interface::kVtable);
  if (flags & 1) {
    calls.Call(kOperatorDelete, frame, 0x8224ACC4, set, flags);
  }
  return set;
}

// --- PanelSetPair ---

uint64_t PairConstruct(uint8_t* base, uint64_t pair_r3, uint32_t f0, uint32_t f1, uint32_t e0,
                       uint32_t e1, uint32_t d0, uint32_t d1, uint32_t c0, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224B750 PanelSetPair::PanelSetPair");
  const uint32_t pair = uint32_t(pair_r3);
  const uint32_t frame = Frame(base, sp, 144);
  const Pair p(base, pair);
  p.set_vtable(Pair::kVtable);
  screen_constructors::PanelSetBase(base, pair + 4, frame);
  screen_constructors::PanelSetBase(base, Pair(base, uint32_t(pair)).second_base_address(), frame);
  const uint32_t confirm0 = LoadU32(base, sp + 92);
  const uint32_t c1 = LoadU32(base, sp + 84);
  const uint32_t confirm1 = LoadU32(base, sp + 100);
  G::set_two_players(base, 1);
  const uint32_t group_b = LoadU32(base, sp + 108);
  // The panels get the word as it reads after each store (not checked for 0).
  p.set_first_tabs(f0);
  schema::TabPanel(base, f0).set_two_players(G::two_players(base));
  p.set_first_rows(e0);
  schema::RowPanel(base, e0).set_two_players(G::two_players(base));
  p.set_first_color_panel(d0);
  schema::ColorPanel(base, d0).set_two_players(G::two_players(base));
  p.set_first_car_panel(c0);
  Car(base, c0).set_two_players(G::two_players(base));
  p.set_first_confirm_panel(confirm0);
  p.set_first_group_b(group_b);
  p.set_second_tabs(f1);
  schema::TabPanel(base, f1).set_two_players(G::two_players(base));
  p.set_second_rows(e1);
  schema::RowPanel(base, e1).set_two_players(G::two_players(base));
  p.set_second_color_panel(d1);
  schema::ColorPanel(base, d1).set_two_players(G::two_players(base));
  p.set_second_car_panel(c1);
  Car(base, c1).set_two_players(G::two_players(base));
  p.set_second_confirm_panel(confirm1);
  p.set_second_group_b(group_b);
  return pair_r3;
}

uint64_t PairDestruct(uint8_t* base, uint64_t pair, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224B830 PanelSetPair::~PanelSetPair");
  const uint32_t frame = Frame(base, sp, 112);
  Pair(base, uint32_t(pair)).set_vtable(Pair::kVtable);
  SetBaseDestruct(base, Pair(base, uint32_t(pair)).second_base_address(), frame, calls);
  const uint64_t r3 = SetBaseDestruct(base, pair + 4, frame, calls);
  Interface(base, uint32_t(pair)).set_vtable(Interface::kVtable);
  return r3;
}

uint64_t PairDeletingDestruct(uint8_t* base, uint64_t pair, uint64_t flags, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CA30 PanelSetPair::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  PairDestruct(base, pair, frame, calls);
  if (flags & 1) {
    // r4 as ~PanelSetPair leaves it (operator delete doesn't read it).
    calls.Call(kOperatorDelete, frame, 0x8224CA60, pair, flags);
  }
  return pair;
}

uint64_t PairReset(uint8_t* base, uint64_t pair, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822498C0 PanelSetPair::Reset");
  // The words the original builds below its r1 (it has no frame) and reads
  // back.
  StoreU32(base, sp - 12, Pair(base, uint32_t(pair)).second_base_address());
  for (const uint32_t at : {64u, 60u, 56u, 52u, 20u, 16u}) StoreU32(base, sp - at, 0);
  for (const uint32_t at : {48u, 44u}) StoreU32(base, sp - at, 1);
  for (const uint32_t at : {40u, 36u, 32u, 28u, 24u}) StoreU32(base, sp - at, 0xFFFFFFFFu);
  ResetWords(base, uint32_t(pair) + 4);
  ResetWords(base, LoadU32(base, sp - 12));
  return pair;
}

uint64_t PairOpen(uint8_t* base, uint64_t pair_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822499F0 PanelSetPair::Open");
  const uint32_t frame = Frame(base, sp, 176);
  // The words on its frame (the bases' reset read through them) and the
  // pair in the caller's area.
  StoreU32(base, frame + 196, uint32_t(pair_r3));
  for (const uint32_t at : {84u, 88u, 92u, 80u, 124u}) StoreU32(base, frame + at, 0);
  StoreU32(base, frame + 128, Pair(base, uint32_t(pair_r3)).second_base_address());
  for (const uint32_t at : {96u, 100u}) StoreU32(base, frame + at, 1);
  for (const uint32_t at : {104u, 108u, 112u, 116u, 120u}) {
    StoreU32(base, frame + at, 0xFFFFFFFFu);
  }
  const uint64_t b1 = pair_r3 + 4;
  const uint32_t b2 = LoadU32(base, frame + 128);
  ResetWords(base, uint32_t(b1));
  ResetWords(base, b2);
  ToTabs(base, b1, frame, calls);
  const uint64_t r3 = ToTabs(base, b2, frame, calls);
  const Pair pair(base, LoadU32(base, frame + 196));
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

uint64_t PairHandleInput(uint8_t* base, uint64_t pair, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CA80 PanelSetPair::HandleInput");
  const uint32_t frame = Frame(base, sp, 112);
  G::set_stand_wait(base, 1);
  HandleInput(base, pair + 4, frame, calls);
  return HandleInput(base, Pair(base, uint32_t(pair)).second_base_address(), frame, calls);
}

uint64_t PairUpdate(uint8_t* base, uint64_t pair, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CF20 PanelSetPair::Update");
  const uint32_t frame = Frame(base, sp, 96);
  Update(base, pair + 4, frame, calls);
  return Update(base, Pair(base, uint32_t(pair)).second_base_address(), frame, calls);
}

uint64_t PairBuild(uint8_t* base, uint64_t pair, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CAE0 PanelSetPair::Build");
  const uint32_t frame = Frame(base, sp, 96);
  Build(base, pair + 4, frame, calls);
  return Build(base, Pair(base, uint32_t(pair)).second_base_address(), frame, calls);
}

uint64_t PairPicked(uint8_t* base, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_82249B60 PanelSetPair::Picked");
  const Pair p(base, pair);
  if (p.first_done() == 0 || p.second_done() == 0) return 0;
  return p.first_picked() != 0 && p.second_picked() != 0 ? 1 : 0;
}

// --- The C panels' base and the confirm panel ---

uint64_t CarPanelBaseDestruct(uint8_t* base, uint64_t panel) {
  ITC_NATIVE_RUNNING("sub_8224D7E0 CarPanelBase::~CarPanelBase");
  Car(base, uint32_t(panel)).set_vtable(kCarPanelBaseVtable);
  return panel;
}

uint64_t ConfirmPanelConstruct(uint8_t* base, uint64_t panel_r3) {
  ITC_NATIVE_RUNNING("sub_8224EBB0 ConfirmPanel::ConfirmPanel");
  const uint32_t panel = uint32_t(panel_r3);
  const Confirm c(base, panel);
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
