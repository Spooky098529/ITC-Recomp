// The race's sounds: the handlers that step the music (the ticks) and the
// events that fade it (docs/functions/race-sounds.md): see race_sounds.h.

#include "race_sounds/race_sounds.h"

#include "base/log.h"
#include "c_runtime/random.h"
#include "guest.h"
#include "archive/archive.h"
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

constexpr Steps kIdleTick = {160,        80,         84,         88,         92,
                             96,         100,        104,        108,        112,
                             0x821767EC, 0x82176818, 0x8217685C, 0x82176880, 0x82176884,
                             0x821768C0, 0x82176914, 0x82176944, false};

// The C runtime's per-thread data lookup (rand's) through `calls`.
uint64_t ThreadDataGuest(void* user, uint32_t function, uint32_t r1, uint32_t lr, int set,
                         uint64_t r3, uint64_t r4) {
  return static_cast<const Calls*>(user)->Call(function, r1, lr, set, r3, r4);
}

// PlayRivalMusic as the 0x4 step's music.
auto RivalMusic(uint8_t* base, uint32_t machine, const Calls& calls) {
  return [=, &calls](uint32_t frame) { return PlayRivalMusic(base, machine, frame, calls); };
}
bool Always(uint64_t&) { return true; }

}  // namespace

uint64_t PlayStartMusic(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175DB0 RaceSounds::PlayStartMusic");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  const int32_t kind = int32_t(Load(base, machine + kKind));
  uint32_t byte = 80, word = 84, name;
  if (kind == 2 || (kind > 3 && kind <= 6)) {
    // One of entries 1 to 5 by rand() % 5 (the indexes from a stack array).
    const c_runtime::ThreadDataCall thread{&ThreadDataGuest, const_cast<Calls*>(&calls)};
    const int32_t number = int32_t(uint32_t(c_runtime::Rand(base, frame, thread)));
    Store(base, frame + 128, 5);
    Store(base, frame + 112, 1);
    Store(base, frame + 116, 2);
    Store(base, frame + 92, 2);
    Store(base, frame + 120, 3);
    Store(base, frame + 124, 4);
    const int32_t index = number - (number / 5) * 5;
    const uint32_t entry = Load(base, frame + 112 + 4 * uint32_t(index));
    guest::StoreU8(base, frame + 82, 0);
    name = Load(base, kMusicNames + 4 * entry);
    return calls.Call(kMusicPlay, frame, 0x82175EC0, 3, frame + 92, name, frame + 82);
  }
  if (kind == 1) {
    byte = 83;
    word = 96;
  } else if (kind == 3) {
    byte = 81;
    word = 88;
  }
  name = Load(base, kMusicNames);
  guest::StoreU8(base, frame + byte, 0);
  Store(base, frame + word, 2);
  return calls.Call(kMusicPlay, frame, 0x82175EC0, 3, frame + word, name, frame + byte);
}

uint64_t IdleTick(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821767A8 RaceSounds::IdleTick");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, kIdleTick.frame_bytes);
  const uint32_t step = Load(base, machine + kCarsStep);
  if (step == 1) {
    return SetUpCars(base, r3, frame, calls);
  }
  if (step != 2) {
    return r3;
  }
  r3 = UpdateCars(base, r3, frame, calls);
  return MusicSteps(base, machine, frame, kIdleTick, r3, calls,
                    [&](uint32_t f) { return PlayStartMusic(base, machine, f, calls); }, Always);
}

uint64_t Begin(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82176618 RaceSounds::Begin");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 192);
  Store(base, machine + kFlags, 0);
  CopySingle(base, kEffectsVolume2, kEffectsVolume);
  Store(base, machine + kCarsPaused, 0);
  // The effects' wave bank (asset 22889): its TOC entry's unpacked size, or
  // the stored one.
  const uint32_t entry = archive::Toc(base, kEffectsBankId) + 0x596A0;
  uint32_t size = Load(base, entry + 8);
  if (size == 0) {
    size = Load(base, entry + 4);
  }
  Store(base, machine + kEffectsBank, 0);
  const uint64_t buffer =
      calls.Call(kXPhysicalAlloc, frame, 0x8217667C, 4, size, 0xFFFFFFFFFFFFFFFFull, 2048, 4);
  Store(base, machine + kEffectsBank, uint32_t(buffer));
  Store(base, kEffectsBankArrived, 0);
  const uint32_t destination = Load(base, machine + kEffectsBank);
  Store(base, machine + 9636, 0);
  Store(base, machine + kSetUpWait, 0);
  {
    const uint64_t args[8] = {kEffectsBankId, destination, kOnEffectsBankLoaded, 0, 0, 0, 0, 0};
    calls.CallArgs(kArchiveLoad, frame, 0x821766B4, 6, args);
  }
  // Each car's two wave bank views: 24-byte objects (operator new, XPR::XPR,
  // 0 when new fails), cleared either way.
  for (uint32_t i = 0; i < kCarSoundCount; i++) {
    for (uint32_t k = 0; k < 2; k++) {
      const uint32_t lr_new = k == 0 ? 0x821766CC : 0x821766F8;
      const uint32_t lr_xpr = k == 0 ? 0x821766D8 : 0x82176704;
      uint64_t view = calls.Call(kOperatorNew, frame, lr_new, 1, 24);
      if (uint32_t(view) != 0) {
        view = calls.Call(kXprConstruct, frame, lr_xpr, 1, view);
      } else {
        view = 0;
      }
      Store(base, kCarViews + 8 * i + 4 * k, uint32_t(view));
      for (uint32_t b = 0; b < 24; b++) {
        guest::StoreU8(base, uint32_t(view) + b, 0);
      }
    }
  }
  PlayStartMusic(base, machine, frame, calls);
  // The listener at the origin: the position at +128, the front (1, 0, 0)
  // at +112, the top (0, 1, 0) at +96, the velocity at +80 (fourth lanes
  // 1.0).
  const float zero = float(double(guest::LoadFloat(base, kZero)));
  const float one = float(double(guest::LoadFloat(base, kOne)));
  const auto put = [&](uint32_t offset, float value) {
    guest::StoreFloat(base, frame + offset, value);
  };
  put(80, zero);
  put(84, zero);
  put(88, zero);
  put(96, zero);
  put(92, one);
  put(100, one);
  put(104, zero);
  put(108, one);
  put(112, one);
  put(116, zero);
  put(120, zero);
  put(124, one);
  put(128, zero);
  put(132, zero);
  put(136, zero);
  put(140, one);
  r3 = SetListener(base, frame + 128, frame + 112, frame + 96, frame + 80, frame, calls);
  Store(base, machine + kCarsStep, 1);
  return r3;
}

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
