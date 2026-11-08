// The options screen's settings read from and written back to the save data
// (docs/functions/options-screen.md): see options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "guest.h"
#include "native/buffers.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace options_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Settings(uint8_t* base) { return LoadU32(base, kSettings); }

// memcpy(destination, source, 60) from `frame` returning to `lr`: its plain
// C++ where it takes it, else the guest's.
void Copy60(uint8_t* base, uint32_t destination, uint32_t source, uint32_t frame, uint32_t lr,
            const Calls& calls) {
  if (!native::Buffers_Memcpy(base, destination, source, 60, lr)) {
    calls.Call(kMemcpy, frame, lr, destination, source, 60);
  }
}

// A record's words and the settings' slots, in the originals' call order:
// the record's word index and the slot's offset in the settings.
struct Pair {
  uint32_t word;
  uint32_t slot;
};
// The buttons' records (o' +44, o' +164: the slots from +92, +140).
constexpr Pair kButtons[12] = {{0, 0},  {1, 4},  {2, 8},  {5, 12}, {6, 16},  {7, 20},
                               {3, 36}, {9, 28}, {10, 32}, {8, 24}, {12, 40}, {13, 44}};
// The analog records read (o' +104 and o' +224: the slots from +188, +212).
constexpr Pair kAnalogRead[6] = {{5, 0}, {6, 4}, {7, 8}, {3, 16}, {9, 12}, {13, 20}};
// Written back: o' +104 as read; o' +224 with slot 4 (+228) into word 8.
constexpr Pair kAnalogWrite104[6] = {{5, 0}, {6, 4}, {7, 8}, {3, 16}, {9, 12}, {13, 20}};
constexpr Pair kAnalogWrite224[6] = {{5, 0}, {6, 4}, {7, 8}, {8, 16}, {9, 12}, {13, 20}};

}  // namespace

uint64_t SettingsRead(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BBF00 Settings::Read");
  const uint32_t frame = sp - 272;
  StoreU32(base, frame, sp);
  StoreU32(base, frame + 80, 0x826E0000);
  switch (int32_t(LoadU32(base, kModeType))) {
    case 2: StoreU32(base, Settings(base) + 4, 1); break;
    case 4: StoreU32(base, Settings(base) + 4, 0); break;
    case 6: StoreU32(base, Settings(base) + 4, 2); break;
    case 7: StoreU32(base, Settings(base) + 4, 3); break;
    case 8: StoreU32(base, Settings(base) + 4, 4); break;
    default: break;
  }
  const uint32_t shown = (LoadU32(base, vehicle_accessors::SaveDataOf(base, 0) + 6092) >> 5) & 1;
  StoreU32(base, Settings(base) + 8,
           LoadU32(base, vehicle_accessors::SaveDataOf(base, 0) + 4) | shown);
  const uint32_t options = vehicle_accessors::SaveDataOf(base, 0) + kSaveOptions;
  for (uint32_t i = 0; i < 4; i++) {
    StoreU32(base, Settings(base) + 44 + 4 * i, LoadU32(base, options + 4 * i));
  }
  // The volumes and sliders: bytes, sign-extended.
  const auto byte = [&](uint32_t at) {
    return uint32_t(int32_t(int8_t(guest::LoadU8(base, options + at))));
  };
  StoreU32(base, Settings(base) + 60, byte(28));
  StoreU32(base, Settings(base) + 64, byte(29));
  for (uint32_t i = 0; i < 6; i++) {
    StoreU32(base, Settings(base) + 68 + 4 * i, byte(36 + i));
  }
  const uint32_t record = frame + 144;
  const auto read = [&](uint32_t from, uint32_t slots, const Pair* pairs, uint32_t count,
                        uint32_t lr, bool buttons) {
    Copy60(base, record, options + from, frame, lr, calls);
    for (uint32_t i = 0; i < count; i++) {
      const uint32_t code = LoadU32(base, record + 4 * pairs[i].word);
      const uint32_t index = buttons ? ButtonIndex(code) : AnalogIndex(code);
      StoreU32(base, Settings(base) + slots + pairs[i].slot, index);
    }
  };
  read(44, 92, kButtons, 12, 0x821BC084, true);
  read(164, 140, kButtons, 12, 0x821BC184, true);
  read(104, 188, kAnalogRead, 6, 0x821BC284, false);
  read(224, 212, kAnalogRead, 6, 0x821BC30C, false);
  // The defaults' table on the frame (+84: 1, 1, 1, 1, 15, 15, then six 0s
  // and a 1), then the defaults.
  static constexpr uint32_t kTable[13] = {1, 1, 1, 1, 15, 15, 0, 0, 0, 0, 0, 0, 1};
  for (uint32_t i = 0; i < 13; i++) {
    StoreU32(base, frame + 84 + 4 * i, kTable[i]);
  }
  static constexpr uint32_t kDefaults[] = {
      // +428 to +472
      1, 1, 1, 1, 15, 15, 0, 0, 0, 0, 0, 0,
      // +476 to +520 and +524 to +568
      1, 6, 7, 5, 12, 13, 15, 2, 16, 3, 0, 4, 1, 6, 7, 5, 12, 13, 15, 2, 16, 3, 0, 4,
      // +572 to +592 and +596 to +616
      5, 17, 18, 2, 3, 4, 5, 17, 18, 2, 3, 4};
  for (uint32_t i = 0; i < sizeof(kDefaults) / 4; i++) {
    StoreU32(base, Settings(base) + 428 + 4 * i, kDefaults[i]);
  }
  const uint32_t o = Settings(base);
  if (int32_t(LoadU32(base, o + 4)) == 1) {
    const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
    const int32_t day = int32_t(LoadU32(base, save + 6100));
    for (uint32_t i = 0; i < 5; i++) {
      StoreU32(base, o + 12 + 4 * i, LoadU32(base, save + 8 + 4 * i));
    }
    StoreU32(base, o + 32, LoadU32(base, save + 6116));
    StoreU32(base, o + 36, uint32_t(day - (day / 7) * 7));
    StoreU32(base, o + 40, uint32_t(day > 999 ? 999 : day));
  }
  const auto setting = [&](uint32_t field) {
    return uint64_t(int64_t(int8_t(LoadU32(base, Settings(base) + field) & 0xFF)));
  };
  calls.Call(kSetEffectsVolume, frame, 0x821BC648, setting(64));
  return calls.Call(kSetMusicVolume, frame, 0x821BC658, setting(60));
}

uint64_t SettingsWrite(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BAE10 Settings::Write");
  const uint32_t frame = sp - 176;
  StoreU32(base, frame, sp);
  const uint32_t options = vehicle_accessors::SaveDataOf(base, 0) + kSaveOptions;
  for (uint32_t i = 0; i < 4; i++) {
    StoreU32(base, options + 4 * i, LoadU32(base, Settings(base) + 44 + 4 * i));
  }
  guest::StoreU8(base, options + 28, uint8_t(LoadU32(base, Settings(base) + 60)));
  guest::StoreU8(base, options + 29, uint8_t(LoadU32(base, Settings(base) + 64)));
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU8(base, options + 36 + i, uint8_t(LoadU32(base, Settings(base) + 68 + 4 * i)));
  }
  const uint32_t record = frame + 80;
  // Each record: copied to the frame, its words replaced, copied back.
  const auto write = [&](uint32_t to, uint32_t slots, const Pair* pairs, uint32_t count,
                         uint32_t lr_in, uint32_t lr_out) {
    Copy60(base, record, options + to, frame, lr_in, calls);
    const uint32_t o = Settings(base);
    for (uint32_t i = 0; i < count; i++) {
      StoreU32(base, record + 4 * pairs[i].word, ButtonCode(LoadU32(base, o + slots + pairs[i].slot)));
    }
    Copy60(base, options + to, record, frame, lr_out, calls);
  };
  write(44, 92, kButtons, 12, 0x821BAED8, 0x821BAF7C);
  write(164, 140, kButtons, 12, 0x821BAF90, 0x821BB034);
  write(104, 188, kAnalogWrite104, 6, 0x821BB048, 0x821BB0A4);
  write(224, 212, kAnalogWrite224, 6, 0x821BB0B8, 0x821BB114);
  return options + 224;
}

}  // namespace options_screen
