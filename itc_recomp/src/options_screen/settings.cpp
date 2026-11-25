// The options screen's settings read from and written back to the save data
// (docs/functions/options-screen.md): see options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "guest.h"
#include "native/buffers.h"
#include "schema/options_screen.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace options_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;

using G = schema::OptionsScreenGlobals;
using S = schema::OptionsSettings;
using SaveData = schema::SaveData;

S Settings() { return G::settings(); }

// memcpy(destination, source, 60) from `frame` returning to `lr`: its plain
// C++ where it takes it, else the guest's.
void Copy60(uint32_t destination, uint32_t source, uint32_t frame, uint32_t lr,
            const Calls& calls) {
  if (!native::Buffers_Memcpy(destination, source, 60, lr)) {
    calls.Call(kMemcpy, frame, lr, destination, source, 60);
  }
}

// A record's words and the settings' slots, in the originals' call order:
// the record's word index and the slot's offset from the slots' first.
struct Pair {
  uint32_t word;
  uint32_t slot;
};
// The buttons' records (o' +44, o' +164: the slots from current_button_0,
// current_p1_button_0).
constexpr Pair kButtons[12] = {{0, 0},  {1, 4},  {2, 8},  {5, 12}, {6, 16},  {7, 20},
                               {3, 36}, {9, 28}, {10, 32}, {8, 24}, {12, 40}, {13, 44}};
// The analog records read (o' +104 and o' +224: the slots from
// current_analog_0, current_p1_analog_0).
constexpr Pair kAnalogRead[6] = {{5, 0}, {6, 4}, {7, 8}, {3, 16}, {9, 12}, {13, 20}};
// Written back: o' +104 as read; o' +224 with slot 4 (current_p1_analog_4)
// into word 8.
constexpr Pair kAnalogWrite104[6] = {{5, 0}, {6, 4}, {7, 8}, {3, 16}, {9, 12}, {13, 20}};
constexpr Pair kAnalogWrite224[6] = {{5, 0}, {6, 4}, {7, 8}, {8, 16}, {9, 12}, {13, 20}};

}  // namespace

uint64_t SettingsRead(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BBF00 Settings::Read");
  const uint32_t frame = sp - 272;
  StoreU32(frame, sp);
  // The frame's +80 (0x826E0000) and +84 (the defaults' 13-word table): the
  // original's slots nothing reads back, dropped (E5 step 6); the record at
  // +144 stays on the stack (memcpy's target and source).
  switch (int32_t(G::mode_type())) {
    case 2: Settings().set_context(1); break;
    case 4: Settings().set_context(0); break;
    case 6: Settings().set_context(2); break;
    case 7: Settings().set_context(3); break;
    case 8: Settings().set_context(4); break;
    default: break;
  }
  const uint32_t shown =
      (SaveData(vehicle_accessors::SaveDataOf(0)).stages() >> 5) & 1;
  Settings().set_music_preview_shown(
      SaveData(vehicle_accessors::SaveDataOf(0)).shown_items() | shown);
  const uint32_t options =
      SaveData(vehicle_accessors::SaveDataOf(0)).options_address();
  for (uint32_t i = 0; i < 4; i++) {
    StoreU32(Settings().current_hud_0_address() + 4 * i, LoadU32(options + 4 * i));
  }
  // The volumes and sliders: bytes, sign-extended.
  const auto byte = [&](uint32_t at) {
    return uint32_t(int32_t(int8_t(guest::LoadU8(options + at))));
  };
  Settings().set_current_music_volume(byte(28));
  Settings().set_current_effects_volume(byte(29));
  // The sliders and player 1's, six words from current_slider_0.
  for (uint32_t i = 0; i < 6; i++) {
    StoreU32(Settings().current_slider_0_address() + 4 * i, byte(36 + i));
  }
  const uint32_t record = frame + 144;
  const auto read = [&](uint32_t from, uint32_t (S::*slots)() const, const Pair* pairs,
                        uint32_t count, uint32_t lr, bool buttons) {
    Copy60(record, options + from, frame, lr, calls);
    for (uint32_t i = 0; i < count; i++) {
      const uint32_t code = LoadU32(record + 4 * pairs[i].word);
      const uint32_t index = buttons ? ButtonIndex(code) : AnalogIndex(code);
      const S o = Settings();
      StoreU32((o.*slots)() + pairs[i].slot, index);
    }
  };
  read(44, &S::current_button_0_address, kButtons, 12, 0x821BC084, true);
  read(164, &S::current_p1_button_0_address, kButtons, 12, 0x821BC184, true);
  read(104, &S::current_analog_0_address, kAnalogRead, 6, 0x821BC284, false);
  read(224, &S::current_p1_analog_0_address, kAnalogRead, 6, 0x821BC30C, false);
  // The defaults.
  static constexpr uint32_t kDefaults[] = {
      // default_hud_0 to default_p1_slider_2
      1, 1, 1, 1, 15, 15, 0, 0, 0, 0, 0, 0,
      // default_button_0 to _11 and default_p1_button_0 to _11
      1, 6, 7, 5, 12, 13, 15, 2, 16, 3, 0, 4, 1, 6, 7, 5, 12, 13, 15, 2, 16, 3, 0, 4,
      // default_analog_0 to _5 and default_p1_analog_0 to _5
      5, 17, 18, 2, 3, 4, 5, 17, 18, 2, 3, 4};
  for (uint32_t i = 0; i < sizeof(kDefaults) / 4; i++) {
    StoreU32(Settings().default_hud_0_address() + 4 * i, kDefaults[i]);
  }
  const S o = Settings();
  if (int32_t(o.context()) == 1) {
    const SaveData save(vehicle_accessors::SaveDataOf(0));
    const int32_t day = int32_t(save.day());
    for (uint32_t i = 0; i < 5; i++) {
      StoreU32(o.name_0_address() + 4 * i, LoadU32(save.pick_head_address() + 4 * i));
    }
    o.set_money(save.money());
    o.set_day_of_week(uint32_t(day - (day / 7) * 7));
    o.set_day(uint32_t(day > 999 ? 999 : day));
  }
  const auto setting = [&](uint32_t (S::*field)() const) {
    return uint64_t(int64_t(int8_t((Settings().*field)() & 0xFF)));
  };
  calls.Call(kSetEffectsVolume, frame, 0x821BC648, setting(&S::current_effects_volume));
  return calls.Call(kSetMusicVolume, frame, 0x821BC658, setting(&S::current_music_volume));
}

uint64_t SettingsWrite(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BAE10 Settings::Write");
  const uint32_t frame = sp - 176;
  StoreU32(frame, sp);
  const uint32_t options =
      SaveData(vehicle_accessors::SaveDataOf(0)).options_address();
  for (uint32_t i = 0; i < 4; i++) {
    StoreU32(options + 4 * i, LoadU32(Settings().current_hud_0_address() + 4 * i));
  }
  guest::StoreU8(options + 28, uint8_t(Settings().current_music_volume()));
  guest::StoreU8(options + 29, uint8_t(Settings().current_effects_volume()));
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU8(options + 36 + i,
                   uint8_t(LoadU32(Settings().current_slider_0_address() + 4 * i)));
  }
  const uint32_t record = frame + 80;
  // Each record: copied to the frame, its words replaced, copied back.
  const auto write = [&](uint32_t to, uint32_t (S::*slots)() const, const Pair* pairs,
                         uint32_t count, uint32_t lr_in, uint32_t lr_out) {
    Copy60(record, options + to, frame, lr_in, calls);
    const S o = Settings();
    for (uint32_t i = 0; i < count; i++) {
      StoreU32(record + 4 * pairs[i].word,
               ButtonCode(LoadU32((o.*slots)() + pairs[i].slot)));
    }
    Copy60(options + to, record, frame, lr_out, calls);
  };
  write(44, &S::current_button_0_address, kButtons, 12, 0x821BAED8, 0x821BAF7C);
  write(164, &S::current_p1_button_0_address, kButtons, 12, 0x821BAF90, 0x821BB034);
  write(104, &S::current_analog_0_address, kAnalogWrite104, 6, 0x821BB048, 0x821BB0A4);
  write(224, &S::current_p1_analog_0_address, kAnalogWrite224, 6, 0x821BB0B8, 0x821BB114);
  return options + 224;
}

}  // namespace options_screen
