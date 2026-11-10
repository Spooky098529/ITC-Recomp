// The race menu's two event tables (docs/functions/race-menu.md,
// RaceMenu::SetUpEvents): see race_menu.h.

#include "race_menu/race_menu.h"

#include <initializer_list>
#include <utility>

#include "base/flags.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "race_menu/internal.h"

ITC_FLAG_DECLARE(bool, native_c_runtime);

namespace race_menu {

namespace {

// The second memcpy (sub_825A7830): its plain C++ under --native_c_runtime
// (as its override), else the guest call. The destination (its r3).
uint64_t CopyForward(uint8_t* base, const Calls& calls, uint64_t to, uint32_t from,
                     uint32_t bytes, uint32_t frame, uint32_t lr) {
  if (ITC_FLAG(native_c_runtime)) {
    c_runtime::CopyForward(base, uint32_t(to), from, bytes);
    return to;
  }
  return calls.Call(kCopyForward, frame, lr, to, from, bytes);
}

using Words = std::initializer_list<std::pair<uint32_t, int32_t>>;

void StoreAll(uint8_t* base, uint32_t frame, Words words) {
  for (const auto& [offset, value] : words) {
    Store(base, frame + offset, uint32_t(value));
  }
}

}  // namespace

uint64_t SetUpEvents(uint8_t* base, uint64_t machine_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821891C8 RaceMenu::SetUpEvents");
  const uint32_t machine = uint32_t(machine_r3);
  const uint32_t frame = Frame(base, sp, 288);
  const uint32_t table = frame + 80;
  const uint32_t bits = Load(base, machine + 10148);
  // The first table (+9980) built in the frame, copied, then the frame's
  // words changed into the second (+9852), with the stores the original
  // makes in its order (some overwrite others).
  if ((bits & 0x20) != 0) {
    StoreAll(base, frame,
             {{108, -1}, {80, 0},   {84, 1},   {88, 0},   {92, 1},   {96, 2},   {100, 3},
              {104, 2},  {112, 3},  {116, -1}, {120, 0},  {124, 1},  {128, 0},  {132, 1},
              {136, 4},  {140, -1}, {144, 5},  {148, -1}, {152, 0},  {156, -1}, {160, 1},
              {164, -1}, {168, 2},  {172, -1}, {176, 3},  {180, -1}, {184, 10}, {188, -1},
              {192, 11}, {196, -1}});
    CopyForward(base, calls, machine_r3 + 9980, table, 120, frame, 0x82189290);
    StoreAll(base, frame,
             {{120, 5},  {148, 0},  {160, 10}, {164, 1}, {168, 11}, {128, 6},  {180, 2},
              {188, 0},  {196, 0},  {136, 7},  {144, 8}, {152, 9},  {176, 12}, {184, 13},
              {192, 14}});
  } else if ((bits & 0x4) != 0 || (bits & 0x8) != 0) {
    StoreAll(base, frame,
             {{108, -1}, {80, 0},   {84, 1},   {88, 0},   {92, 1},   {96, 2},   {100, 3},
              {104, 2},  {112, 3},  {116, -1}, {120, 0},  {124, 1},  {128, 0},  {132, 1},
              {136, 4},  {140, -1}, {144, 0},  {148, -1}, {152, 1},  {156, -1}, {160, 2},
              {164, -1}, {168, 3},  {172, -1}, {176, 10}, {180, -1}, {184, -1}, {188, -1},
              {192, -1}, {196, -1}});
    CopyForward(base, calls, machine_r3 + 9980, table, 120, frame, 0x82189544);
    StoreAll(base, frame,
             {{148, 1},  {152, 10}, {164, 2}, {180, 0}, {184, -1}, {120, 5},  {188, -1},
              {192, -1}, {196, -1}, {128, 6}, {136, 7}, {144, 9},  {160, 11}, {168, 12},
              {176, 13}});
  } else {
    StoreAll(base, frame,
             {{116, -1}, {80, 0},   {88, 0},   {96, 0},   {104, 0},  {84, 1},   {92, 1},
              {100, 1},  {108, 1},  {124, -1}, {132, -1}, {140, -1}, {144, -1}, {148, -1},
              {152, -1}, {156, -1}, {160, -1}, {164, -1}, {168, -1}, {172, -1}, {176, -1},
              {180, -1}, {184, -1}, {188, -1}, {192, -1}, {196, -1}});
    if ((bits & 0x10) != 0) {
      StoreAll(base, frame, {{120, 0}, {128, 1}, {112, 4}, {136, 10}});
      CopyForward(base, calls, machine_r3 + 9980, table, 120, frame, 0x821893A0);
      StoreAll(base, frame,
               {{100, 1},  {108, 1},  {116, 0},  {128, 10}, {144, -1}, {96, 5},   {148, -1},
                {152, -1}, {156, -1}, {160, -1}, {104, 6},  {164, -1}, {168, -1}, {172, -1},
                {176, -1}, {112, 7},  {180, -1}, {184, -1}, {188, -1}, {192, -1}, {120, 9},
                {196, -1}, {136, 13}});
      // The original's join after the second table's own words.
      StoreAll(base, frame, {{132, 1}, {140, 0}});
    } else {
      StoreAll(base, frame, {{112, 0}, {120, 1}, {128, -1}, {136, -1}});
      CopyForward(base, calls, machine_r3 + 9980, table, 120, frame, 0x82189428);
      StoreAll(base, frame,
               {{100, 1},  {108, 1},  {116, 1},  {128, -1}, {132, -1}, {96, 5},   {136, -1},
                {140, -1}, {144, -1}, {148, -1}, {104, 6},  {152, -1}, {156, -1}, {160, -1},
                {164, -1}, {112, 9},  {168, -1}, {172, -1}, {176, -1}, {180, -1}, {120, 10},
                {184, -1}, {188, -1}, {192, -1}, {196, -1}});
    }
  }
  if ((bits & 0x20) != 0 || (bits & 0x4) != 0 || (bits & 0x8) != 0) {
    // The two paths' shared tail (loc_8218959C, then loc_821895BC).
    StoreAll(base, frame,
             {{108, 2}, {104, 3}, {100, 2}, {96, 2}, {112, 4}, {116, 2}, {156, 1}, {172, 2}});
    StoreAll(base, frame, {{132, 1}, {140, 0}});
  }
  StoreAll(base, frame, {{80, 0}, {84, 1}, {88, 1}, {92, 1}, {124, 1}});
  return CopyForward(base, calls, machine_r3 + 9852, table, 120, frame, 0x821895E8);
}

}  // namespace race_menu
