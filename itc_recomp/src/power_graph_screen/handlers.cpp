// The power graph screen's machine handlers and the members' slots
// (docs/functions/power-graph-screen.md): OnEnter and OnLeft make and free
// the three blocks, WaitForLoad, Shown, Open and Leave move the machine, the
// slots post its replies on the mode's ring.

#include "power_graph_screen/power_graph_screen.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace power_graph_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;

// The original's frame below `sp`, its back chain written.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

}  // namespace

void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const uint32_t ring = mode + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
  StoreU32(base, slot + 0, id);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
}

void SlotPost(uint8_t* base, uint32_t mode, uint32_t slot) {
  uint32_t id = 0;
  switch (slot) {
    case 0x82237A88:
      ITC_NATIVE_RUNNING("sub_82237A88 PowerGraphScreen6::PostLeave");
      id = 4;
      break;
    case 0x82237AD8:
      ITC_NATIVE_RUNNING("sub_82237AD8 PowerGraphScreen6::PostClosing");
      id = 23;
      break;
    case 0x82237B28:
      ITC_NATIVE_RUNNING("sub_82237B28 PowerGraphScreen6::PostClosed");
      id = 24;
      break;
    case 0x82246FB8:
      ITC_NATIVE_RUNNING("sub_82246FB8 PowerGraphScreen7::PostLeave");
      id = 3;
      break;
    default:
      ITC_NATIVE_RUNNING("sub_82247008 PowerGraphScreen7::PostClosing");
      id = 20;
      break;
  }
  Post(base, mode, id);
}

uint64_t OnEnter(uint8_t* base, uint32_t member, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250778 PowerGraphScreen67::OnEnter");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, member + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(calls.Call(kVirtualAlloc, frame, 0x822507AC, 0, 28, 0x1000, 4));
  StoreU32(base, kPages, pages);
  StoreU32(base, pages + 4, 0);
  StoreU32(base, pages + 0, 0);
  StoreU32(base, pages + 8, 0);
  StoreU32(base, pages + 24, 0);
  StoreU32(base, pages + 20, 0xFFFFFFFFu);
  StoreU32(base, pages + 12, 0xFFFFFFFFu);
  const uint32_t menu = uint32_t(calls.Call(kVirtualAlloc, frame, 0x822507E8, 0, 68, 0x1000, 4));
  StoreU32(base, kMenu, menu);
  StoreU32(base, menu + 12, 0);
  StoreU32(base, menu + 20, 0);
  StoreU32(base, menu + 0, 0);
  StoreU32(base, menu + 4, 0);
  StoreU32(base, menu + 16, 7);
  for (const uint32_t field : {8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    StoreU32(base, menu + field, 0);
  }
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x82250844, 0, 92, 0x1000, 4);
  const uint32_t data = uint32_t(r3);
  StoreU32(base, kData, data);
  StoreU32(base, data + 0, 0);
  StoreU32(base, data + 4, 21);
  StoreU32(base, member + kMachineState, LoadU32(base, member + kMachineReenter) != 0 ? 1 : 9);
  StoreU32(base, kShown, 0);
  return r3;
}

uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82250D70 PowerGraphScreen67::WaitForLoad");
  Frame(base, sp, 96);
  const uint32_t member = uint32_t(r3);
  if (LoadU32(base, member + kMachineLoaded) == 0) {
    const uint32_t loaded = ui_movies::IsLoaded(base, kMovie);
    if (int32_t(loaded) != 0) {
      StoreU32(base, member + kMachineLoaded, 1);
    }
    return loaded;
  }
  StoreU32(base, LoadU32(base, kData) + 0, 1);
  StoreU32(base, member + kMachineState, 9);
  return r3;
}

void Shown(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82250338 PowerGraphScreen67::Shown");
  StoreU32(base, kShown, 0);
}

void Open(uint8_t* base, uint32_t member) {
  ITC_NATIVE_RUNNING("sub_82250890 PowerGraphScreen67::Open");
  if (LoadU32(base, member + kMachineLoaded) != 0) {
    const uint32_t pages = LoadU32(base, kPages);
    const uint32_t before = LoadU32(base, pages + 0);
    if (before != 0) {
      StoreU32(base, pages + 8, before);
    }
    StoreU32(base, pages + 12, 0);
    StoreU32(base, pages + 4, 0);
    StoreU32(base, pages + 16, 0xFFFFFFFFu);
    StoreU32(base, pages + 0, 0xFFFFFFFFu);
    StoreU32(base, member + kMachineState, 4);
  } else {
    StoreU32(base, member + kMachineState, 1);
  }
  StoreU32(base, kShown, 0);
}

uint64_t Leave(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250900 PowerGraphScreen67::Leave");
  const uint32_t member = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, member + kMachineState, 7);
  calls.Call(kMoviesDelete, frame, 0x82250924, kMovie);
  // The member's slot 1 (its leave message).
  const uint32_t slot = LoadU32(base, LoadU32(base, member + 0) + 4);
  const uint64_t answer = calls.Call(slot, frame, 0x8225093C, r3, mode);
  StoreU32(base, member + kMachineState, 7);
  return answer;
}

uint64_t OnLeft(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822502C0 PowerGraphScreen67::OnLeft");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x822502E8, LoadU32(base, kPages), 0, 0x8000);
  calls.Call(kVirtualFree, frame, 0x822502FC, LoadU32(base, kMenu), 0, 0x8000);
  const uint64_t r3 = calls.Call(kVirtualFree, frame, 0x82250310, LoadU32(base, kData), 0, 0x8000);
  StoreU32(base, kPages, 0);
  StoreU32(base, kMenu, 0);
  StoreU32(base, kData, 0);
  return r3;
}

}  // namespace power_graph_screen
