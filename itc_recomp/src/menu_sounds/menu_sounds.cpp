// The menu sounds (docs/functions/menu-sounds.md): see menu_sounds.h.

#include "menu_sounds/menu_sounds.h"

#include "base/log.h"
#include "guest.h"
#include "schema/menu_sounds.h"

namespace menu_sounds {

namespace {

using G = schema::MenuSoundsGlobals;

// The two volume setters: the gain from the setting, the category named
// `name`, then the gain stored to the music's volume (`music`) or the
// effects' two.
uint64_t SetVolume(uint8_t* base, uint64_t setting, uint32_t sp, const Calls& calls,
                   uint32_t name, bool music, uint32_t get_lr, uint32_t set_lr) {
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  const int64_t value = int8_t(uint8_t(setting));
  uint32_t gain;
  if (value == 0) {
    gain = G::zero(base);
  } else if (value == 15) {
    gain = G::one(base);
  } else {
    // std, lfd, fcfid, frsp, then fmuls with 1/15: the doubleword stays at
    // +88 (its low word survives the category's store below).
    guest::StoreU32(base, frame + 88, uint32_t(uint64_t(value) >> 32));
    guest::StoreU32(base, frame + 92, uint32_t(uint64_t(value)));
    const float f13 = float(double(value));
    const float fifteenth = G::fifteenth(base);
    const float product = float(double(f13) * double(fifteenth));
    uint32_t bits;
    __builtin_memcpy(&bits, &product, 4);
    gain = bits;
  }
  guest::StoreU32(base, frame + 80, gain);
  const schema::AudioManager manager = G::sound_manager(base);
  uint64_t r3 = calls.Call(manager.vtable().get_category(), frame, get_lr, 2, manager.address,
                           name);
  guest::StoreU32(base, frame + 88, uint32_t(r3));
  if (int32_t(uint32_t(r3)) != -1) {
    const schema::AudioManager again = G::sound_manager(base);
    r3 = calls.Call(again.vtable().set_category_volume(), frame, set_lr, 3, again.address,
                    frame + 88, frame + 80);
  }
  const uint32_t stored = guest::LoadU32(base, frame + 80);
  if (music) {
    G::set_music_volume(base, stored);
  } else {
    G::set_effects_volume(base, stored);
    G::set_effects_volume_2(base, stored);
  }
  return r3;
}

}  // namespace

uint64_t PlayMenuSound(uint8_t* base, uint32_t cue, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821103F0 MenuSound::Play");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  guest::StoreU32(base, frame + 84, cue);
  guest::StoreU32(base, frame + 80, G::one(base));
  guest::StoreU32(base, frame + 88, 0);
  const schema::AudioManager manager = G::sound_manager(base);
  // PlayCue2D isn't ours yet: the slot through the function table.
  return calls.Call(manager.vtable().play_cue_2d(), frame, 0x82110438, 4, manager.address,
                    frame + 88, frame + 84, frame + 80);
}

uint64_t PlaySound(uint8_t* base, uint32_t bank, uint32_t cue, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175850 Sound::Play");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  guest::StoreU32(base, frame + 80, G::one(base));
  const schema::AudioManager manager = G::sound_manager(base);
  return calls.Call(manager.vtable().play_cue_2d(), frame, 0x82175890, 4, manager.address, bank,
                    cue, frame + 80);
}

uint64_t SetEffectsVolume(uint8_t* base, uint64_t setting, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B9C20 SoundOptions::SetEffectsVolume");
  return SetVolume(base, setting, sp, calls, G::default_category_name_address(), false,
                   0x821B9C9C, 0x821B9CC4);
}

uint64_t SetMusicVolume(uint8_t* base, uint64_t setting, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B9CF0 SoundOptions::SetMusicVolume");
  return SetVolume(base, setting, sp, calls, G::music_category_name_address(), true, 0x821B9D6C,
                   0x821B9D94);
}

}  // namespace menu_sounds
