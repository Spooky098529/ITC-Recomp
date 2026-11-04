// The race's sounds: the handlers that step the music (the ticks) and the
// events that fade it (docs/functions/race-sounds.md): see race_sounds.h.

#include "race_sounds/race_sounds.h"

#include "base/log.h"
#include "guest.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_sounds {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

void CopySingle(uint8_t* base, uint32_t from, uint32_t to) {
  const double value = double(guest::LoadFloat(base, from));
  guest::StoreFloat(base, to, float(value));
}

uint32_t Flags(uint8_t* base, uint32_t machine) { return Load(base, machine + kFlags); }
void SetFlags(uint8_t* base, uint32_t machine, uint32_t flags) {
  Store(base, machine + kFlags, flags);
}

// The manager's slot `slot` with r4 = `r4` (and r5 = `r5` for set 3).
uint64_t CallSlot(uint8_t* base, const Calls& calls, uint32_t slot, uint32_t frame, uint32_t lr,
                  int set, uint64_t r4, uint64_t r5 = 0) {
  const uint32_t manager = Load(base, kSoundManager);
  return calls.Call(Load(base, Load(base, manager) + slot), frame, lr, set, manager, r4, r5);
}

// The music faded out: the kept volume = the music's, the byte 0x826C5C84
// = 0, flag 0x10 cleared and 0x8000 (with `extra`) set.
void FadeOut(uint8_t* base, uint32_t machine, uint32_t extra) {
  CopySingle(base, kMusicVolume, kKeptMusicVolume);
  guest::StoreU8(base, kMusicStarted, 0);
  SetFlags(base, machine, (Flags(base, machine) & ~0x10u) | 0x8000u | extra);
}

// The words and return addresses of a tick's music steps: the frame's
// offsets of the indexes (the cue 0 and bank 2 words) each slot is given,
// and the calls' return addresses.
struct Steps {
  uint32_t frame_bytes;
  uint32_t stop_cue, stop_bank, playing_cue, playing_bank, release_bank, empty_bank,
      loading_bank, play_cue, play_bank;
  uint32_t fade_lr, stop_lr, playing_lr, release_lr, restore_lr, empty_lr, loading_lr, play_lr;
  bool effects_back;  // the 0x20 step also puts the effects' volume back
};

// The music steps (the intro's): `next(frame)` plays the next music for the
// 0x4 step and returns its r3; `may_play(r3)` decides whether cue 0 is
// played (setting r3 when it makes a call). Returns r3 as the original
// leaves it.
template <typename Next, typename MayPlay>
uint64_t MusicSteps(uint8_t* base, uint32_t machine, uint32_t frame, const Steps& s,
                    uint64_t r3, const Calls& calls, Next next, MayPlay may_play) {
  if (Flags(base, machine) & 0x8000) {
    r3 = FadeMusic(base, frame, calls);
    if (int32_t(uint32_t(r3)) == 0) {
      return r3;
    }
    Store(base, frame + s.stop_cue, 0);
    Store(base, frame + s.stop_bank, 2);
    r3 = CallSlot(base, calls, kStopCueSlot, frame, s.stop_lr, 3, frame + s.stop_bank,
                  frame + s.stop_cue);
    SetFlags(base, machine, (Flags(base, machine) | 0x20) & ~0x8000u);
  }
  if (Flags(base, machine) & 0x20) {
    Store(base, frame + s.playing_cue, 0);
    Store(base, frame + s.playing_bank, 2);
    r3 = CallSlot(base, calls, kIsCuePlayingSlot, frame, s.playing_lr, 3, frame + s.playing_bank,
                  frame + s.playing_cue);
    if (uint8_t(r3) != 0) {
      return r3;
    }
    Store(base, frame + s.release_bank, 2);
    CallSlot(base, calls, kReleaseBankSlot, frame, s.release_lr, 2, frame + s.release_bank);
    r3 = RestoreMusicVolume(base, frame, calls);
    if (s.effects_back) {
      CopySingle(base, kEffectsVolume2, kEffectsVolume);
    }
    SetFlags(base, machine, (Flags(base, machine) | 0x2) & ~0x20u);
  }
  if (Flags(base, machine) & 0x2) {
    Store(base, frame + s.empty_bank, 2);
    r3 = CallSlot(base, calls, kIsBankSlotEmptySlot, frame, s.empty_lr, 2, frame + s.empty_bank);
    if (uint8_t(r3) == 0) {
      return r3;
    }
    SetFlags(base, machine, (Flags(base, machine) | 0x4) & ~0x2u);
  }
  if (Flags(base, machine) & 0x4) {
    r3 = next(frame);
    SetFlags(base, machine, Flags(base, machine) & ~0x4u);
  }
  Store(base, frame + s.loading_bank, 2);
  r3 = CallSlot(base, calls, kIsBankLoadingSlot, frame, s.loading_lr, 2, frame + s.loading_bank);
  if (uint8_t(r3) != 0) {
    return r3;
  }
  const uint32_t flags = Flags(base, machine);
  if ((flags & 0x10) || (flags & 0x2)) {
    return r3;
  }
  if (!may_play(r3)) {
    return r3;
  }
  Store(base, frame + s.play_cue, 0);
  Store(base, frame + s.play_bank, 2);
  r3 = calls.Call(kSoundPlay, frame, s.play_lr, 2, frame + s.play_bank, frame + s.play_cue);
  SetFlags(base, machine, Flags(base, machine) | 0x10);
  return r3;
}

// The ticks' frames (160 bytes) and return addresses, by tick.
constexpr Steps kRaceTick = {160,        80,         84,         88,         92,
                             96,         100,        104,        108,        112,
                             0x82176A14, 0x82176A40, 0x82176A84, 0x82176AA8, 0x82176AAC,
                             0x82176AF8, 0x82176B4C, 0x82176B7C, true};
constexpr Steps kStageTick = {160,        80,         84,         88,         92,
                              96,         100,        104,        108,        112,
                              0x82176F54, 0x82176F80, 0x82176FC4, 0x82176FE8, 0x82176FEC,
                              0x82177028, 0x8217707C, 0x821770AC, false};
constexpr Steps kRollingStartTick = {160,        80,         84,         88,         92,
                                     96,         100,        104,        108,        112,
                                     0x8217715C, 0x82177188, 0x821771CC, 0x821771F0,
                                     0x821771F4, 0x82177240, 0x82177294, 0x821772C4, true};
constexpr Steps kAfterRaceTick = {176,        84,         88,         92,         96,
                                  100,        104,        112,        108,        116,
                                  0x82176C40, 0x82176C6C, 0x82176CB0, 0x82176CD4, 0x82176CD8,
                                  0x82176D14, 0x82176DEC, 0x82176E38, false};

// PlayRivalMusic as the 0x4 step's music.
auto RivalMusic(uint8_t* base, uint32_t machine, const Calls& calls) {
  return [=, &calls](uint32_t frame) { return PlayRivalMusic(base, machine, frame, calls); };
}
bool Always(uint64_t&) { return true; }

}  // namespace

uint64_t RaceTick(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821769D0 RaceSounds::RaceTick");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, kRaceTick.frame_bytes);
  const uint32_t step = Load(base, machine + kCarsStep);
  if (step == 1) {
    return SetUpCars(base, r3, frame, calls);
  }
  if (step != 2) {
    return r3;
  }
  r3 = UpdateCars(base, r3, frame, calls);
  return MusicSteps(base, machine, frame, kRaceTick, r3, calls, RivalMusic(base, machine, calls),
                    Always);
}

uint64_t StageTick(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82176F28 RaceSounds::StageTick");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, kStageTick.frame_bytes);
  return MusicSteps(base, machine, frame, kStageTick, r3, calls,
                    RivalMusic(base, machine, calls), Always);
}

uint64_t RollingStartTick(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82177130 RaceSounds::RollingStartTick");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, kRollingStartTick.frame_bytes);
  return MusicSteps(base, machine, frame, kRollingStartTick, r3, calls,
                    RivalMusic(base, machine, calls), Always);
}

uint64_t AfterRaceTick(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82176C10 RaceSounds::AfterRaceTick");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, kAfterRaceTick.frame_bytes);
  r3 = UpdateCars(base, r3, frame, calls);
  // The 0x4 step's music by the last racer record's +3924 (probably the
  // result): bgm_tmp_14 (entry 11) or bgm_tmp_08 (entry 12).
  const auto next = [&](uint32_t f) {
    const uint32_t data = Load(base, kRaceData);
    const uint32_t record = vehicle_accessors::RacerRecordsAt(base, data + 0x59AE8, 0xFFFFFFFFu);
    bool win;
    if (int32_t(Load(base, machine + kKind)) != 6) {
      win = int32_t(Load(base, record + 3924)) == 1;
    } else if (int32_t(Load(base, Load(base, kRaceData) + 0xA3AF4)) == 7) {
      win = true;
    } else {
      const uint32_t career = Load(base, kAfterRaceWord);
      const int32_t result = int32_t(Load(base, record + 3924));
      win = career == 0 ? result == 2 : result == 1;
    }
    const uint32_t name = Load(base, kMusicNames + (win ? 44 : 48));
    guest::StoreU8(base, f + 80, 0);
    Store(base, f + 108, 2);
    return calls.Call(kMusicPlay, f, 0x82176DC4, 3, f + 108, name, f + 80);
  };
  // Cue 0 not played while player 0's save data's +6100 is 1 (played above
  // 999 or otherwise).
  return MusicSteps(base, machine, frame, kAfterRaceTick, r3, calls, next,
                    [&](uint64_t& last) {
                      const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
                      last = save;
                      const int32_t value = int32_t(Load(base, save + 6100));
                      return value > 999 || value != 1;
                    });
}

uint64_t OnCountdown(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82176968 RaceSounds::OnCountdown");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kCarSoundUpdate, frame, 0x82176988, 1, Load(base, kCarSounds));
  FadeOut(base, machine, 0);
  return PickRivalMusic(base, machine, frame);
}

uint64_t OnRaceEnd(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82176BA0 RaceSounds::OnRaceEnd");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  // fmuls: the effects' volume times 0.0, rounded to single.
  const double volume = double(guest::LoadFloat(base, kEffectsVolume));
  const double zero = double(guest::LoadFloat(base, kZero));
  guest::StoreFloat(base, kEffectsVolume, float(volume * zero));
  r3 = UpdateCars(base, r3, frame, calls);
  FadeOut(base, machine, 0);
  return r3;
}

uint64_t OnLeaveAfterRace(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82176E60 RaceSounds::OnLeaveAfterRace");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  r3 = UpdateCars(base, r3, frame, calls);
  FadeOut(base, machine, 0);
  CopySingle(base, kEffectsVolume2, kEffectsVolume);
  return r3;
}

uint64_t OnNextStage(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82176EC8 RaceSounds::OnNextStage");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kCarSoundsStopAll, frame, 0x82176EE0, 1, r3);
  FadeOut(base, machine, 0x4000);
  return PickRivalMusic(base, machine, frame);
}

uint64_t OnRollingStart(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821770C0 RaceSounds::OnRollingStart");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  if (Load(base, machine + kCarsStep) == 1) {
    SetUpCars(base, r3, frame, calls);
  }
  FadeOut(base, machine, 0);
  return PickRivalMusic(base, machine, frame);
}

}  // namespace race_sounds
