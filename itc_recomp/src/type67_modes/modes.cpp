// The type 6 and 7 modes' delegate functions NextMode and Leaving
// (docs/functions/type6-type7-modes.md) as plain C++: the messages queued
// in the mode's ring as the originals do it inline (no overflow check),
// GameMessages::Dispatch through its override (a guest call), then the
// screens' machines checked in the originals' order and the delegate's
// member functor rewritten, its two words built in the original's frame
// (+80, +84) and copied from there as the original's ld does.

#include "type67_modes/type67_modes.h"

#include "base/log.h"
#include "guest.h"

namespace type67_modes {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kDispatch = 0x821062E8;
constexpr uint32_t kBlock = 848;  // the mode's screen block

constexpr MachineAt kType6Next[] = {{4, 4},         {19432, 19436}, {9672, 9672},
                                    {30396, 30856}, {40948, 40952}, {62336, 62336},
                                    {71976, 71976}, {81616, 81616}};
constexpr MachineAt kType6Leave[] = {{4, 4},         {9672, 9672},   {30396, 30856},
                                     {40948, 40952}, {52692, 52696}, {62336, 62336},
                                     {71976, 71976}, {81616, 81616}};
constexpr MachineAt kType7Next[] = {{4, 8},         {19428, 19432}, {30608, 30612}, {9664, 9664},
                                    {42352, 42356}, {51996, 51996}, {61636, 61636}};
constexpr MachineAt kType7Leave[] = {{4, 8},         {30608, 30612}, {9664, 9664},
                                     {42352, 42356}, {51996, 51996}, {61636, 61636}};

// A machine's state, -1 when its cast gives none.
int32_t State(uint8_t* base, uint32_t block, const MachineAt& at) {
  if (int32_t(block + at.test) == 0) {
    return -1;
  }
  const uint32_t machine = block + at.machine;
  if (machine == 0) {
    return -1;
  }
  return int32_t(LoadU32(base, machine + 9604));
}

// A message (`message`, 0, 0) queued in the mode's ring (+240: the count at
// +840, the start at +844), as the originals do it inline.
void Queue(uint8_t* base, uint32_t mode, uint32_t message) {
  const uint32_t ring = mode + 240;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
  StoreU32(base, slot + 0, message);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
}

void ClearAnswer(uint8_t* base, uint32_t answer) {
  StoreU32(base, answer + 4, 0);
  guest::StoreU8(base, answer + 0, 0);
}

// The delegate (mode +4 = mode +8 when that isn't 0) given a member functor:
// vtable 0x82010FA0, the mode, the function and adjustment from the frame.
void SetDelegate(uint8_t* base, uint32_t mode, uint32_t frame) {
  const uint32_t holder = mode + 4;
  uint32_t delegate = holder + 4;
  if (int32_t(delegate) != 0) {
    StoreU32(base, delegate + 4, mode);
    StoreU32(base, delegate + 8, LoadU32(base, frame + 80));
    StoreU32(base, delegate + 12, LoadU32(base, frame + 84));
    StoreU32(base, delegate + 0, kModeDelegateVtable);
  } else {
    delegate = 0;
  }
  StoreU32(base, holder, delegate);
}

}  // namespace

const ModeLayout kType6Layout = {91264,       91256,       4,  0x82236100, 0x82235C18,
                                 0x82636F9C,  0x82236468,  0x82236174, kType6Next, 8,
                                 kType6Leave, 8,           {19432, 19436}};
const ModeLayout kType7Layout = {71284,       71276,       3,  0x82246590, 0x82246098,
                                 0x82637984,  0x822468B0,  0x82246604, kType7Next, 7,
                                 kType7Leave, 6,           {19428, 19432}};

void ModeNextMode(uint8_t* base, const ModeLayout& layout, uint32_t answer, uint32_t mode,
                  uint32_t frame, const Calls& calls) {
  if (&layout == &kType6Layout) {
    ITC_NATIVE_RUNNING("sub_822363B8 Type6Mode::NextMode");
  } else {
    ITC_NATIVE_RUNNING("sub_82246800 Type7Mode::NextMode");
  }
  Queue(base, mode, 1);
  Queue(base, mode, 0);
  calls.call(kDispatch, frame, layout.next_lr, mode + 36, 0);
  const uint32_t block = LoadU32(base, mode + kBlock);
  ClearAnswer(base, answer);
  bool leave = false;
  for (uint32_t i = 0; i < layout.next_count && !leave; i++) {
    leave = State(base, block, layout.next_checks[i]) == 10;
  }
  if (!leave) {
    return;
  }
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 80, layout.leaving);
  SetDelegate(base, mode, frame);
  StoreU32(base, kLeavingFlag, 0);
  StoreU32(base, LoadU32(base, mode + kBlock) + layout.phase, 4);
}

void ModeLeaving(uint8_t* base, const ModeLayout& layout, uint32_t answer, uint32_t mode,
                 uint32_t frame, const Calls& calls) {
  if (&layout == &kType6Layout) {
    ITC_NATIVE_RUNNING("sub_82236100 Type6Mode::Leaving");
  } else {
    ITC_NATIVE_RUNNING("sub_82246590 Type7Mode::Leaving");
  }
  ClearAnswer(base, answer);
  Queue(base, mode, layout.leave_message);
  calls.call(kDispatch, frame, layout.leaving_lr, mode + 36, mode);
  const uint32_t block = LoadU32(base, mode + kBlock);
  for (uint32_t i = 0; i < layout.leave_count; i++) {
    if (State(base, block, layout.leave_checks[i]) != 6) {
      return;
    }
  }
  if (State(base, block, layout.list) == 6) {
    StoreU32(base, block + layout.phase, 5);
    StoreU32(base, frame + 80, layout.tear_down);
    StoreU32(base, layout.car, 0xFFFFFFFF);
  } else if (State(base, block, layout.list) == 11) {
    StoreU32(base, LoadU32(base, mode + kBlock) + layout.phase, 5);
    guest::StoreU8(base, frame + 80, 1);
    StoreU32(base, frame + 84, 1);
    const uint32_t high = LoadU32(base, frame + 80), low = LoadU32(base, frame + 84);
    const uint32_t to = LoadU32(base, mode + kBlock) + layout.answer;
    StoreU32(base, to, high);
    StoreU32(base, to + 4, low);
    StoreU32(base, frame + 80, layout.tear_down);
  } else {
    return;
  }
  StoreU32(base, frame + 84, 0);
  SetDelegate(base, mode, frame);
}

}  // namespace type67_modes
