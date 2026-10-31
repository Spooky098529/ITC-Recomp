// The menu sounds (docs/functions/menu-sounds.md): see menu_sounds.h.

#include "menu_sounds/menu_sounds.h"

#include "base/log.h"
#include "guest.h"

namespace menu_sounds {

namespace {

// The function in the manager's vtable at `slot` (bytes into it).
uint32_t Slot(uint8_t* base, uint32_t manager, uint32_t slot) {
  return guest::LoadU32(base, guest::LoadU32(base, manager) + slot);
}

// The two volume setters: the gain from the setting, the category named
// `name`, the globals in `globals` (`count` of them).
uint64_t SetVolume(uint8_t* base, uint64_t setting, uint32_t sp, const Calls& calls,
                   uint32_t name, const uint32_t* globals, int count, uint32_t get_lr,
                   uint32_t set_lr) {
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  const int64_t value = int8_t(uint8_t(setting));
  uint32_t gain;
  if (value == 0) {
    gain = guest::LoadU32(base, kZero);
  } else if (value == 15) {
    gain = guest::LoadU32(base, kOne);
  } else {
    // std, lfd, fcfid, frsp, then fmuls with 1/15: the doubleword stays at
    // +88 (its low word survives the category's store below).
    guest::StoreU32(base, frame + 88, uint32_t(uint64_t(value) >> 32));
    guest::StoreU32(base, frame + 92, uint32_t(uint64_t(value)));
    const float f13 = float(double(value));
    const float fifteenth = guest::LoadFloat(base, kFifteenth);
    const float product = float(double(f13) * double(fifteenth));
    uint32_t bits;
    __builtin_memcpy(&bits, &product, 4);
    gain = bits;
  }
  guest::StoreU32(base, frame + 80, gain);
  const uint32_t manager = guest::LoadU32(base, kSoundManager);
  uint64_t r3 =
      calls.Call(Slot(base, manager, kGetCategorySlot), frame, get_lr, 2, manager, name);
  guest::StoreU32(base, frame + 88, uint32_t(r3));
  if (int32_t(uint32_t(r3)) != -1) {
    const uint32_t again = guest::LoadU32(base, kSoundManager);
    r3 = calls.Call(Slot(base, again, kSetCategoryVolumeSlot), frame, set_lr, 3, again, frame + 88,
                    frame + 80);
  }
  const uint32_t stored = guest::LoadU32(base, frame + 80);
  for (int i = 0; i < count; i++) {
    guest::StoreU32(base, globals[i], stored);
  }
  return r3;
}

}  // namespace

uint64_t PlayMenuSound(uint8_t* base, uint32_t cue, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821103F0 MenuSound::Play");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  guest::StoreU32(base, frame + 84, cue);
  guest::StoreU32(base, frame + 80, guest::LoadU32(base, kOne));
  guest::StoreU32(base, frame + 88, 0);
  const uint32_t manager = guest::LoadU32(base, kSoundManager);
  // PlayCue2D isn't ours yet: the slot through the function table.
  return calls.Call(Slot(base, manager, kPlayCue2DSlot), frame, 0x82110438, 4, manager,
                    frame + 88, frame + 84, frame + 80);
}

uint64_t PlaySound(uint8_t* base, uint32_t bank, uint32_t cue, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175850 Sound::Play");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  guest::StoreU32(base, frame + 80, guest::LoadU32(base, kOne));
  const uint32_t manager = guest::LoadU32(base, kSoundManager);
  return calls.Call(Slot(base, manager, kPlayCue2DSlot), frame, 0x82175890, 4, manager, bank,
                    cue, frame + 80);
}

uint64_t SetEffectsVolume(uint8_t* base, uint64_t setting, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B9C20 SoundOptions::SetEffectsVolume");
  static constexpr uint32_t kGlobals[] = {kEffectsVolume, kEffectsVolume2};
  return SetVolume(base, setting, sp, calls, kDefaultName, kGlobals, 2, 0x821B9C9C, 0x821B9CC4);
}

uint64_t SetMusicVolume(uint8_t* base, uint64_t setting, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B9CF0 SoundOptions::SetMusicVolume");
  static constexpr uint32_t kGlobals[] = {kMusicVolume};
  return SetVolume(base, setting, sp, calls, kMusicName, kGlobals, 1, 0x821B9D6C, 0x821B9D94);
}

}  // namespace menu_sounds
