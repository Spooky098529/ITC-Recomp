// The race's sounds' handlers and helpers (docs/functions/race-sounds.md):
// see race_sounds.h.

#include "race_sounds/race_sounds.h"

#include "base/log.h"
#include "guest.h"

#include <cmath>

namespace race_sounds {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// lfs then stfs: the float through a double, as the original's registers
// hold it.
float LoadSingle(uint8_t* base, uint32_t address) {
  const double value = double(guest::LoadFloat(base, address));
  return float(value);
}
void CopySingle(uint8_t* base, uint32_t from, uint32_t to) {
  guest::StoreFloat(base, to, LoadSingle(base, from));
}

// The sound manager's function at `slot` (bytes into its vtable).
uint32_t Slot(uint8_t* base, uint32_t manager, uint32_t slot) {
  return Load(base, Load(base, manager) + slot);
}

// Slot `slot` of the manager ([0x826C5C80] loaded again) with r4 = `r4`
// (and the first `set` registers).
uint64_t CallSlot(uint8_t* base, const Calls& calls, uint32_t slot, uint32_t frame, uint32_t lr,
                  int set, uint64_t r4 = 0, uint64_t r5 = 0) {
  const uint32_t manager = Load(base, kSoundManager);
  return calls.Call(Slot(base, manager, slot), frame, lr, set, manager, r4, r5);
}

// The original's frame below `sp`: its back chain.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// The car sound i's address as the loops pass it: 0 (each time) while the
// array is 0.
uint32_t CarSound(uint8_t* base, uint32_t i) {
  const uint32_t array = Load(base, kCarSounds);
  return array == 0 ? 0 : array + kCarSoundBytes * i;
}

// CarSound::PauseAll on each of the seven (the loop's bl at `lr`).
uint64_t PauseEach(uint8_t* base, uint32_t frame, uint32_t lr, const Calls& calls) {
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < kCarSoundCount; i++) {
    r3 = calls.Call(kCarSoundPauseAll, frame, lr, 1, CarSound(base, i));
  }
  return r3;
}

// CarSound::ResumeAll seven times on the first car sound (the pointer not
// advanced, as the original).
uint64_t ResumeFirst(uint8_t* base, uint32_t frame, uint32_t lr, const Calls& calls) {
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < kCarSoundCount; i++) {
    r3 = calls.Call(kCarSoundResumeAll, frame, lr, 1, Load(base, kCarSounds));
  }
  return r3;
}

// Slot 29 with "Music" into the frame's +80, then slot `slot` with r4 =
// +80.
uint64_t MusicCategory(uint8_t* base, uint32_t frame, uint32_t slot, uint32_t get_lr,
                       uint32_t lr, const Calls& calls) {
  const uint64_t category = CallSlot(base, calls, kGetCategorySlot, frame, get_lr, 2, kMusicName);
  Store(base, frame + 80, uint32_t(category));
  return CallSlot(base, calls, slot, frame, lr, 2, frame + 80);
}

}  // namespace

uint64_t SetListener(uint8_t* base, uint64_t position, uint64_t front, uint64_t top,
                     uint64_t velocity, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821758A0 RaceSounds::SetListener");
  const uint32_t frame = Frame(base, sp, 112);
  uint32_t manager = Load(base, kSoundManager);
  calls.Call(Slot(base, manager, kSetListenerOrientationSlot), frame, 0x821758CC, 4, manager,
             front, top, velocity);
  manager = Load(base, kSoundManager);
  calls.Call(Slot(base, manager, kSetListenerPositionSlot), frame, 0x821758E4, 2, manager,
             position);
  manager = Load(base, kSoundManager);
  return calls.Call(Slot(base, manager, kSetListenerVelocitySlot), frame, 0x821758FC, 2, manager,
                    velocity);
}

uint64_t FadeMusic(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175908 RaceSounds::FadeMusic");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t category = CallSlot(base, calls, kGetCategorySlot, frame, 0x82175938, 2,
                                     kMusicName);
  Store(base, frame + 80, uint32_t(category));
  const double kept = double(guest::LoadFloat(base, kKeptMusicVolume));
  const double zero = double(guest::LoadFloat(base, kZero));
  if (kept == zero) {
    return 1;
  }
  const uint32_t manager = Load(base, kSoundManager);
  const double tenth = double(guest::LoadFloat(base, kTenth));
  const double volume = double(guest::LoadFloat(base, kMusicVolume));
  // fnmsubs: -(kept * tenth - volume), fused, rounded to single.
  const double faded = double(float(-std::fma(kept, tenth, -volume)));
  guest::StoreFloat(base, kMusicVolume, float(faded));
  if (faded > zero) {
    calls.Call(Slot(base, manager, kSetCategoryVolumeSlot), frame, 0x821759BC, 3, manager,
               frame + 80, kMusicVolume);
    return 0;
  }
  guest::StoreFloat(base, kMusicVolume, float(zero));
  calls.Call(Slot(base, manager, kSetCategoryVolumeSlot), frame, 0x82175994, 3, manager,
             frame + 80, kMusicVolume);
  return 1;
}

uint64_t RestoreMusicVolume(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821759C8 RaceSounds::RestoreMusicVolume");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t category = CallSlot(base, calls, kGetCategorySlot, frame, 0x821759F8, 2,
                                     kMusicName);
  Store(base, frame + 80, uint32_t(category));
  const uint32_t manager = Load(base, kSoundManager);
  CopySingle(base, kKeptMusicVolume, kMusicVolume);
  return calls.Call(Slot(base, manager, kSetCategoryVolumeSlot), frame, 0x82175A28, 3, manager,
                    frame + 80, kMusicVolume);
}

uint64_t EndTick(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175A40 RaceSounds::EndTick");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (Load(base, machine + kFlags) & 0x20) {
    return r3;
  }
  Store(base, frame + 80, 0);
  Store(base, frame + 84, 2);
  CallSlot(base, calls, kStopCueSlot, frame, 0x82175A94, 3, frame + 84, frame + 80);
  Store(base, machine + kFlags, Load(base, machine + kFlags) | 0x20);
  return PauseEach(base, frame, 0x82175AC0, calls);
}

uint64_t Pause(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175AE8 RaceSounds::Pause");
  const uint32_t frame = Frame(base, sp, 112);
  PauseEach(base, frame, 0x82175B1C, calls);
  return MusicCategory(base, frame, kPauseCategorySlot, 0x82175B48, 0x82175B64, calls);
}

uint64_t Resume(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175B80 RaceSounds::Resume");
  const uint32_t frame = Frame(base, sp, 112);
  ResumeFirst(base, frame, 0x82175BA4, calls);
  return MusicCategory(base, frame, kResumeCategorySlot, 0x82175BCC, 0x82175BE8, calls);
}

uint64_t PauseCars(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175C00 RaceSounds::PauseCars");
  return PauseEach(base, Frame(base, sp, 112), 0x82175C34, calls);
}

uint64_t ResumeCars(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175C58 RaceSounds::ResumeCars");
  return ResumeFirst(base, Frame(base, sp, 112), 0x82175C7C, calls);
}

uint64_t OnStageCountdown(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82175D20 RaceSounds::OnStageCountdown");
  const uint32_t machine = uint32_t(r3);
  CopySingle(base, kEffectsVolume2, kEffectsVolume);
  Store(base, machine + kCarsStep, 1);
  Store(base, machine + kFlags, Load(base, machine + kFlags) & ~0x4000u);
  return r3;
}

uint64_t OnRollingStartDone(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175D48 RaceSounds::OnRollingStartDone");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t flags = Load(base, machine + kFlags);
  if ((flags & 0x20000) && !(flags & 0x4000)) {
    r3 = calls.Call(kCarSoundsStopAll, frame, 0x82175D74, 1, r3);
    Store(base, machine + kFlags, (Load(base, machine + kFlags) | 0x4000) & ~0x10000u);
  }
  const uint32_t after = Load(base, machine + kFlags);
  Store(base, machine + kCarsStep, 1);
  Store(base, machine + kFlags, after & ~0x4000u);
  return r3;
}

uint64_t OnAbortDone(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82175CF8 RaceSounds::OnAbortDone");
  const uint32_t machine = uint32_t(r3);
  Store(base, machine + kCarsStep, 1);
  Store(base, machine + kFlags, Load(base, machine + kFlags) & ~0x4000u);
  CopySingle(base, kEffectsVolume2, kEffectsVolume);
  return r3;
}

uint64_t Unload(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821772D8 RaceSounds::Unload");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  calls.Call(kCarSoundsRelease, frame, 0x821772EC, 1, r3);
  for (uint32_t i = 0; i < kCarSoundCount; i++) {
    calls.Call(kXPhysicalFree, frame, 0x82177318, 1, Load(base, kSlotBuffer1 + 4 * i));
    calls.Call(kXPhysicalFree, frame, 0x82177320, 1, Load(base, kSlotBuffer0 + 4 * i));
    calls.Call(kSoundHolderDelete, frame, 0x82177328, 1, kCarViews + 8 * i);
    calls.Call(kSoundHolderDelete, frame, 0x82177330, 1, kCarViews + 8 * i + 4);
  }
  Store(base, frame + 80, 2);
  CallSlot(base, calls, kReleaseBankSlot, frame, 0x82177364, 2, frame + 80);
  return calls.Call(kXPhysicalFree, frame, 0x8217736C, 1, Load(base, machine + kEffectsBank));
}

void OnEffectsBankLoaded(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82175ED0 RaceSounds::OnEffectsBankLoaded");
  Store(base, kEffectsBankArrived, 1);
}

}  // namespace race_sounds
