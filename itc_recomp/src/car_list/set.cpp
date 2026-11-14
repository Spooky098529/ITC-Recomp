// PanelSet's leaves (docs/functions/car-list.md): see car_list.h.

#include "car_list/car_list.h"

#include "archive/archive.h"
#include "base/log.h"
#include "guest.h"
#include "screen_constructors/screen_constructors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace car_list {

namespace {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// PanelSet::Reset's stores on the base `b` (PanelSetPair's Reset and Open
// make them on both bases).
void ResetWords(uint8_t* base, uint32_t b) {
  for (const uint32_t at : {8u, 12u, 16u, 20u, 24u}) StoreU32(base, b + at, 0);
  StoreU32(base, b + 1940, 1);
  StoreU32(base, b + 1948, 1);
  for (const uint32_t at : {36u, 32u, 40u, 52u, 44u}) StoreU32(base, b + at, 0xFFFFFFFFu);
  StoreU32(base, b + 56, 0);
  StoreU32(base, b + 1820, 0);
  for (const uint32_t at : {228u, 232u, 236u, 240u, 244u, 248u}) StoreU32(base, b + at, 0);
}

}  // namespace

uint64_t SetReset(uint8_t* base, uint64_t set) {
  ITC_NATIVE_RUNNING("sub_822496C8 PanelSet::Reset");
  ResetWords(base, uint32_t(set) + 4);
  return set;
}

uint64_t SetPicked(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82249848 PanelSet::Picked");
  if (LoadU32(base, set + 16) == 0) return 0;
  return LoadU32(base, set + 12) != 0 ? 1 : 0;
}

uint64_t SetWentBack(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82249870 PanelSet::WentBack");
  if (LoadU32(base, set + 16) == 0) return 0;
  return LoadU32(base, set + 12) == 0 ? 1 : 0;
}

uint64_t SetWantsPowerGraph(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82249898 PanelSet::WantsPowerGraph");
  if (LoadU32(base, set + 20) == 0) return 0;
  return LoadU32(base, set + 12) != 0 ? 1 : 0;
}

uint64_t SetWord(uint8_t* base, uint64_t set, uint32_t offset, uint64_t value) {
  switch (offset) {
    case 1932:
      ITC_NATIVE_RUNNING("sub_8224B6D8 PanelSet::SetOpenKind");
      break;
    case 1884:
      ITC_NATIVE_RUNNING("sub_8224B6E8 PanelSet::SetPreviousTab");
      break;
    case 1888:
      ITC_NATIVE_RUNNING("sub_8224B6F0 PanelSet::SetPreviousEntry");
      break;
    default:
      ITC_NATIVE_RUNNING("sub_8224B6F8 PanelSet::SetPreviousColor");
      break;
  }
  StoreU32(base, uint32_t(set) + offset, uint32_t(value));
  return set;
}

uint64_t PickedPage(uint8_t* base, uint64_t b) {
  ITC_NATIVE_RUNNING("sub_82248EF0 PanelSetBase::PickedPage");
  StoreU32(base, uint32_t(b) + 12, 1);
  return b;
}

uint64_t SaveData(uint8_t* base, uint32_t b, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82249668 PanelSetBase::SaveData");
  StoreU32(base, sp - 96, sp);
  if (LoadU32(base, kTwoPlayers) == 0) {
    return vehicle_accessors::SaveDataOf(base, 0);
  }
  // mullw: the low words' signed product, 64 bits; add.
  const int64_t offset = int64_t(int32_t(LoadU32(base, b + 1952))) * int64_t(120108);
  return uint64_t(LoadU32(base, kPairSaveData)) + uint64_t(offset);
}

// --- The destructors ---

uint64_t GroupBaseDestruct(uint8_t* base, uint64_t group) {
  ITC_NATIVE_RUNNING("sub_8224EF08 PanelGroupBase::~PanelGroupBase");
  StoreU32(base, uint32_t(group), kPanelGroupBaseVtable);
  archive::NodeDestruct(base, uint32_t(group));
  return group;
}

uint64_t SetBaseDestruct(uint8_t* base, uint64_t b, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224AE60 PanelSetBase::~PanelSetBase");
  const uint32_t frame = sp - 96;
  StoreU32(base, frame, sp);
  StoreU32(base, uint32_t(b), kPanelSetBaseVtable);
  calls.Call(kMenuListRelease, frame, 0x8224AE88, b + 132);
  return calls.Call(kMenuListRelease, frame, 0x8224AE90, b + 60);
}

uint64_t SetDestruct(uint8_t* base, uint64_t set, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224B708 PanelSet::~PanelSet");
  const uint32_t frame = sp - 96;
  StoreU32(base, frame, sp);
  StoreU32(base, uint32_t(set), kPanelSetVtable);
  const uint64_t r3 = SetBaseDestruct(base, set + 4, frame, calls);
  StoreU32(base, uint32_t(set), kPanelSetInterfaceVtable);
  return r3;
}

uint64_t SetInterfaceDeletingDestruct(uint8_t* base, uint64_t set, uint64_t flags, uint32_t sp,
                                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224AC98 PanelSetInterface::DeletingDestructor");
  const uint32_t frame = sp - 96;
  StoreU32(base, frame, sp);
  StoreU32(base, uint32_t(set), kPanelSetInterfaceVtable);
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
  StoreU32(base, pair + 0, kPanelSetPairVtable);
  screen_constructors::PanelSetBase(base, pair + 4, frame);
  screen_constructors::PanelSetBase(base, pair + kPairSecondBase, frame);
  const uint32_t confirm0 = LoadU32(base, sp + 92);
  const uint32_t c1 = LoadU32(base, sp + 84);
  const uint32_t confirm1 = LoadU32(base, sp + 100);
  StoreU32(base, kTwoPlayers, 1);
  const uint32_t group_b = LoadU32(base, sp + 108);
  // The panels get the word as it reads after each store (not checked for 0).
  const auto panel = [&](uint32_t at, uint32_t object, uint32_t word_at) {
    StoreU32(base, pair + at, object);
    StoreU32(base, object + word_at, LoadU32(base, kTwoPlayers));
  };
  panel(1900, f0, 140);
  panel(1904, e0, 156);
  panel(1908, d0, 88);
  panel(1912, c0, 316);
  StoreU32(base, pair + 1916, confirm0);
  StoreU32(base, pair + 1960, group_b);
  panel(3872, f1, 140);
  panel(3876, e1, 156);
  panel(3880, d1, 88);
  panel(3884, c1, 316);
  StoreU32(base, pair + 3888, confirm1);
  StoreU32(base, pair + 3932, group_b);
  return pair_r3;
}

uint64_t PairDestruct(uint8_t* base, uint64_t pair, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224B830 PanelSetPair::~PanelSetPair");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, uint32_t(pair), kPanelSetPairVtable);
  SetBaseDestruct(base, pair + kPairSecondBase, frame, calls);
  const uint64_t r3 = SetBaseDestruct(base, pair + 4, frame, calls);
  StoreU32(base, uint32_t(pair), kPanelSetInterfaceVtable);
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
  StoreU32(base, sp - 12, uint32_t(pair) + kPairSecondBase);
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
  StoreU32(base, frame + 128, uint32_t(pair_r3) + kPairSecondBase);
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
  const uint32_t pair = LoadU32(base, frame + 196);
  StoreU32(base, pair + 1956, 0);
  StoreU32(base, pair + 3928, 1);
  for (const uint32_t at : {1944u, 3916u, 1952u, 3924u, 1948u, 3920u}) {
    StoreU32(base, pair + at, 0);
  }
  return r3;
}

uint64_t PairHandleInput(uint8_t* base, uint64_t pair, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CA80 PanelSetPair::HandleInput");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, kStandWait, 1);
  HandleInput(base, pair + 4, frame, calls);
  return HandleInput(base, pair + kPairSecondBase, frame, calls);
}

uint64_t PairUpdate(uint8_t* base, uint64_t pair, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CF20 PanelSetPair::Update");
  const uint32_t frame = Frame(base, sp, 96);
  Update(base, pair + 4, frame, calls);
  return Update(base, pair + kPairSecondBase, frame, calls);
}

uint64_t PairBuild(uint8_t* base, uint64_t pair, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CAE0 PanelSetPair::Build");
  const uint32_t frame = Frame(base, sp, 96);
  Build(base, pair + 4, frame, calls);
  return Build(base, pair + kPairSecondBase, frame, calls);
}

uint64_t PairPicked(uint8_t* base, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_82249B60 PanelSetPair::Picked");
  if (LoadU32(base, pair + 16) == 0 || LoadU32(base, pair + 1988) == 0) return 0;
  return LoadU32(base, pair + 12) != 0 && LoadU32(base, pair + 1984) != 0 ? 1 : 0;
}

// --- The C panels' base and the confirm panel ---

uint64_t CarPanelBaseDestruct(uint8_t* base, uint64_t panel) {
  ITC_NATIVE_RUNNING("sub_8224D7E0 CarPanelBase::~CarPanelBase");
  StoreU32(base, uint32_t(panel), kCarPanelBaseVtable);
  return panel;
}

uint64_t ConfirmPanelConstruct(uint8_t* base, uint64_t panel_r3) {
  ITC_NATIVE_RUNNING("sub_8224EBB0 ConfirmPanel::ConfirmPanel");
  const uint32_t panel = uint32_t(panel_r3);
  StoreU32(base, panel + 4, 0);
  StoreU32(base, panel + 0, kConfirmPanelVtable);
  StoreU32(base, panel + 12, 0xFFFFFFFFu);
  StoreU32(base, panel + 8, 2);
  for (uint32_t at = 16; at <= 36; at += 4) StoreU32(base, panel + at, 0xFFFFFFFFu);
  return panel_r3;
}

}  // namespace car_list
