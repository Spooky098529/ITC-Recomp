// The race's sounds' handlers and helpers (docs/functions/race-sounds.md):
// see race_sounds.h.

#include "race_sounds/race_sounds.h"

#include "base/log.h"
#include "guest.h"

#include <cmath>

namespace race_sounds {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// lfs then stfs: the float through a double, as the original's registers
// hold it.
float LoadSingle(uint32_t address) {
  const double value = double(guest::LoadFloat(address));
  return float(value);
}
void CopySingle(uint32_t from, uint32_t to) {
  guest::StoreFloat(to, LoadSingle(from));
}

// Slot `slot` of the manager ([0x826C5C80] loaded again) with r4 = `r4`
// (and the first `set` registers).
uint64_t CallSlot(const Calls& calls, uint32_t slot, uint32_t frame, uint32_t lr,
                  int set, uint64_t r4 = 0, uint64_t r5 = 0) {
  const schema::AudioManager manager = SoundManager();
  return calls.Call(ManagerSlot(manager, slot), frame, lr, set, manager.address, r4, r5);
}

// The original's frame below `sp`: its back chain.
uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(frame, sp);
  return frame;
}

// CarSound::PauseAll on each of the seven (the loop's bl at `lr`).
uint64_t PauseEach(uint32_t frame, uint32_t lr, const Calls& calls) {
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < kCarSoundCount; i++) {
    r3 = calls.Call(kCarSoundPauseAll, frame, lr, 1, CarSoundAt(i));
  }
  return r3;
}

// CarSound::ResumeAll seven times on the first car sound (the pointer not
// advanced, as the original).
uint64_t ResumeFirst(uint32_t frame, uint32_t lr, const Calls& calls) {
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < kCarSoundCount; i++) {
    r3 = calls.Call(kCarSoundResumeAll, frame, lr, 1, CarSounds::sounds().address);
  }
  return r3;
}

// Slot 29 with "Music" into the frame's +80, then slot `slot` with r4 =
// +80.
uint64_t MusicCategory(uint32_t frame, uint32_t slot, uint32_t get_lr,
                       uint32_t lr, const Calls& calls) {
  const uint64_t category = CallSlot(calls, kGetCategorySlot, frame, get_lr, 2, schema::MenuSoundsGlobals::music_category_name_address());
  Store(frame + 80, uint32_t(category));
  return CallSlot(calls, slot, frame, lr, 2, frame + 80);
}

}  // namespace

uint64_t SetListener(uint64_t position, uint64_t front, uint64_t top,
                     uint64_t velocity, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821758A0 RaceSounds::SetListener");
  // No slots (E5 step 6): the manager's calls made from `sp` (their
  // arguments are the caller's vectors, in its frame above `sp`).
  const uint32_t frame = sp;
  schema::AudioManager manager = SoundManager();
  calls.Call(manager.vtable().set_listener_orientation(), frame, 0x821758CC, 4, manager.address,
             front, top, velocity);
  manager = SoundManager();
  calls.Call(manager.vtable().set_listener_position(), frame, 0x821758E4, 2, manager.address,
             position);
  manager = SoundManager();
  return calls.Call(manager.vtable().set_listener_velocity(), frame, 0x821758FC, 2,
                    manager.address, velocity);
}

uint64_t FadeMusic(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175908 RaceSounds::FadeMusic");
  const uint32_t frame = Frame(sp, 112);
  const uint64_t category = CallSlot(calls, kGetCategorySlot, frame, 0x82175938, 2,
                                     schema::MenuSoundsGlobals::music_category_name_address());
  Store(frame + 80, uint32_t(category));
  const double kept = double(Globals::kept_music_volume());
  const double zero = double(RaceStates::zero());
  if (kept == zero) {
    return 1;
  }
  const schema::AudioManager manager = SoundManager();
  const double tenth = double(schema::AiDriversGlobals::tenth());
  const double volume = double(Globals::music_volume());
  // fnmsubs: -(kept * tenth - volume), fused, rounded to single.
  const double faded = double(float(-std::fma(kept, tenth, -volume)));
  Globals::set_music_volume(float(faded));
  if (faded > zero) {
    calls.Call(ManagerSlot(manager, kSetCategoryVolumeSlot), frame, 0x821759BC, 3,
               manager.address,
               frame + 80, Globals::music_volume_address());
    return 0;
  }
  Globals::set_music_volume(float(zero));
  calls.Call(ManagerSlot(manager, kSetCategoryVolumeSlot), frame, 0x82175994, 3,
             manager.address,
             frame + 80, Globals::music_volume_address());
  return 1;
}

uint64_t RestoreMusicVolume(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821759C8 RaceSounds::RestoreMusicVolume");
  const uint32_t frame = Frame(sp, 112);
  const uint64_t category = CallSlot(calls, kGetCategorySlot, frame, 0x821759F8, 2,
                                     schema::MenuSoundsGlobals::music_category_name_address());
  Store(frame + 80, uint32_t(category));
  const schema::AudioManager manager = SoundManager();
  CopySingle(Globals::kept_music_volume_address(), Globals::music_volume_address());
  return calls.Call(ManagerSlot(manager, kSetCategoryVolumeSlot), frame, 0x82175A28, 3,
                    manager.address,
                    frame + 80, Globals::music_volume_address());
}

uint64_t EndTick(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175A40 RaceSounds::EndTick");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(sp, 112);
  if (Sounds(machine).flags() & 0x20) {
    return r3;
  }
  Store(frame + 80, 0);
  Store(frame + 84, 2);
  CallSlot(calls, kStopCueSlot, frame, 0x82175A94, 3, frame + 84, frame + 80);
  Sounds(machine).set_flags(Sounds(machine).flags() | 0x20);
  return PauseEach(frame, 0x82175AC0, calls);
}

uint64_t Pause(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175AE8 RaceSounds::Pause");
  const uint32_t frame = Frame(sp, 112);
  PauseEach(frame, 0x82175B1C, calls);
  return MusicCategory(frame, kPauseCategorySlot, 0x82175B48, 0x82175B64, calls);
}

uint64_t Resume(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175B80 RaceSounds::Resume");
  const uint32_t frame = Frame(sp, 112);
  ResumeFirst(frame, 0x82175BA4, calls);
  return MusicCategory(frame, kResumeCategorySlot, 0x82175BCC, 0x82175BE8, calls);
}

uint64_t PauseCars(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175C00 RaceSounds::PauseCars");
  // No frame (E5 step 6): the calls made from `sp`.
  return PauseEach(sp, 0x82175C34, calls);
}

uint64_t ResumeCars(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175C58 RaceSounds::ResumeCars");
  // No frame (E5 step 6): the calls made from `sp`.
  return ResumeFirst(sp, 0x82175C7C, calls);
}

uint64_t OnStageCountdown(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82175D20 RaceSounds::OnStageCountdown");
  const uint32_t machine = uint32_t(r3);
  CopySingle(Globals::effects_volume_2_address(), Globals::effects_volume_address());
  Sounds(machine).set_cars_step(1);
  Sounds(machine).set_flags(Sounds(machine).flags() & ~0x4000u);
  return r3;
}

uint64_t OnRollingStartDone(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175D48 RaceSounds::OnRollingStartDone");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = sp;  // no frame (E5 step 6): the call made from `sp`
  const uint32_t flags = Sounds(machine).flags();
  if ((flags & 0x20000) && !(flags & 0x4000)) {
    r3 = calls.Call(kCarSoundsStopAll, frame, 0x82175D74, 1, r3);
    Sounds(machine).set_flags((Sounds(machine).flags() | 0x4000) & ~0x10000u);
  }
  const uint32_t after = Sounds(machine).flags();
  Sounds(machine).set_cars_step(1);
  Sounds(machine).set_flags(after & ~0x4000u);
  return r3;
}

uint64_t OnAbortDone(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82175CF8 RaceSounds::OnAbortDone");
  const uint32_t machine = uint32_t(r3);
  Sounds(machine).set_cars_step(1);
  Sounds(machine).set_flags(Sounds(machine).flags() & ~0x4000u);
  CopySingle(Globals::effects_volume_2_address(), Globals::effects_volume_address());
  return r3;
}

uint64_t Unload(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821772D8 RaceSounds::Unload");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(sp, 144);
  calls.Call(kCarSoundsRelease, frame, 0x821772EC, 1, r3);
  for (uint32_t i = 0; i < kCarSoundCount; i++) {
    calls.Call(kXPhysicalFree, frame, 0x82177318, 1,
               Load(Globals::slot_buffers_1_address() + 4 * i));
    calls.Call(kXPhysicalFree, frame, 0x82177320, 1,
               Load(Globals::slot_buffers_0_address() + 4 * i));
    calls.Call(kSoundHolderDelete, frame, 0x82177328, 1, Globals::car_views_address() + 8 * i);
    calls.Call(kSoundHolderDelete, frame, 0x82177330, 1, Globals::car_views_address() + 8 * i + 4);
  }
  Store(frame + 80, 2);
  CallSlot(calls, kReleaseBankSlot, frame, 0x82177364, 2, frame + 80);
  return calls.Call(kXPhysicalFree, frame, 0x8217736C, 1,
                    Sounds(machine).effects_bank().address);
}

void OnEffectsBankLoaded() {
  ITC_NATIVE_RUNNING("sub_82175ED0 RaceSounds::OnEffectsBankLoaded");
  Globals::set_effects_bank_arrived(1);
}

}  // namespace race_sounds
