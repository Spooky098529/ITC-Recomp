// The type 2 mode's block helpers (docs/functions/type2-mode.md): see
// type2_mode.h.

#include "type2_mode/type2_mode.h"

#include "base/log.h"
#include "guest.h"
#include "schema/type2_mode.h"

namespace type2_mode {

uint64_t MachineOf(uint64_t block, uint64_t index) {
  ITC_NATIVE_RUNNING("sub_82196FC0 ModeType2::MachineOf");
  const int32_t signed_index = int32_t(uint32_t(index));
  if (signed_index > 16) {
    if (signed_index == 26) {
      return block + 0x20A70;
    }
    if (signed_index == 44) {
      return block + 0x255C0;
    }
    return 0;
  }
  if (signed_index == 16) {
    return block + 0x1E4C8;
  }
  switch (uint32_t(index)) {
    case 0:  // sub_82197000
      return block;
    case 1:
      return block + 9640;
    case 2:
      return block + 19792;
    case 3:
      return block + 0x173D0;
    case 4:
      return block + 0x19978;
    case 5:
      return block + 0x23018;
    case 6:
      return block + 0x1BF20;
    case 7:
      return block + 0x12880;
    case 9:
      return block + 56624;
    case 10:
      return block + 0x102D8;
    case 12:
      return block + 0x14E28;
    default:  // 8, 11, and above 12 (negative ones too)
      return 0;
  }
}

uint32_t AllLoaded(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_82197130 ModeType2::AllLoaded");
  // The loaded words (+9608) of the hub, the race menu, the tuning screen,
  // the car settings, the car dealer, the garage screen, the build screen,
  // the options, the spec sheet, the user info, the plate editor and the
  // save screen (the block's +0x27B48).
  static constexpr uint32_t kMachines[] = {0,       9640,    19792,   0x173D0, 0x19978, 0x23018,
                                           0x1BF20, 0x12880, 56624,   0x102D8, 0x20A70, 0x255C0};
  for (const uint32_t at : kMachines) {
    if (schema::Type2ScreenMachine(base, block + at).loaded() == 0) {
      return 0;
    }
  }
  return 1;
}

uint64_t FindShown(uint8_t* base, uint64_t block, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82197218 ModeType2::FindShown");
  guest::StoreU32(base, sp - 96, sp);
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 8; i++) {
    r3 = MachineOf(block, i);
    if (uint32_t(r3) == 0) {
      continue;
    }
    const int32_t state = int32_t(schema::Type2ScreenMachine(base, uint32_t(r3)).state());
    if (state == 5 || state == 4) {
      schema::Type2ModeBlock(base, uint32_t(block)).set_shown(i);
    }
  }
  return r3;
}

}  // namespace type2_mode
