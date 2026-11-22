// The race's sounds natively (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/race-sounds.md): one of the race mode's state
// machines (race mode + 138284, RaceSounds::), whose handlers run the car
// sounds and step the race's music over the sound manager ([0x826C5C80],
// docs/functions/sound-cues.md). Plain C++ over guest memory (the machine
// and the globals are shared with the recompiled code), with the originals'
// loads and stores in their order and their frames' words. `sp` is the
// caller's stack pointer: the back chain of the original's frame and its
// locals are written below it as the original leaves them; its register
// saves aren't. Adapters: src/hooks/race_sounds.cpp.
//
// The guest calls go through `Calls` by address, with the registers the
// original sets (the first `set` of r3 to r6; the others left as the
// previous call left them, as the original leaves them), its frame and
// return address: the car sounds (car-sounds.md) run as their register
// steps' plain C++; the sound manager's slots (by the user's call for the
// menu sounds, 2026-10-31) and the other callees through the function table
// (their overrides).

#pragma once

#include <cstdint>

#include "guest.h"
#include "schema/ai_drivers.h"        // the float 0.1
#include "schema/archive.h"           // ArchiveTocEntry
#include "schema/boot_mode.h"         // the pad blocked words (the system UI)
#include "schema/car_sounds.h"        // CarSound, CarSoundWaveBank (the effects' bank)
#include "schema/career.h"            // CareerSave (the save data)
#include "schema/menu_sounds.h"       // AudioManager (the sound manager)
#include "schema/minimap.h"           // MinimapRacerEntry (the racers' entries)
#include "schema/race_hud_machine.h"  // RacerResultsRecord
#include "schema/race_sounds.h"
#include "schema/race_states.h"       // RaceData, RaceProgress, the floats 0.0 and 1.0
#include "schema/race_world.h"        // RaceManager

namespace race_sounds {

// The sound manager ([0x826C5C80], menu-sounds.toml's AudioManager) and its
// vtable's slots (bytes into it; slots 30 to 32 aren't AudioManagerVtable's
// fields yet).
inline schema::AudioManager SoundManager(uint8_t* base) {
  return schema::MenuSoundsGlobals::sound_manager(base);
}
// The manager's function at `slot`.
inline uint32_t ManagerSlot(uint8_t* base, schema::AudioManager manager, uint32_t slot) {
  return guest::LoadU32(base, manager.vtable().address + slot);
}
constexpr uint32_t kReleaseBankSlot = 20;              // 5
constexpr uint32_t kIsBankLoadingSlot = 24;            // 6
constexpr uint32_t kIsBankSlotEmptySlot = 32;          // 8
constexpr uint32_t kStopCueSlot = 52;                  // 13
constexpr uint32_t kIsCuePlayingSlot = 56;             // 14
constexpr uint32_t kSetListenerPositionSlot = 100;     // 25
constexpr uint32_t kSetListenerOrientationSlot = 104;  // 26
constexpr uint32_t kSetListenerVelocitySlot = 108;     // 27
constexpr uint32_t kGetCategorySlot = 116;             // 29
constexpr uint32_t kSetCategoryVolumeSlot = 120;       // 30
constexpr uint32_t kPauseCategorySlot = 124;           // 31
constexpr uint32_t kResumeCategorySlot = 128;          // 32

// The car sounds' array (seven of 1,264 bytes, car-sounds.toml's CarSound;
// 0 before CarSounds::Init).
constexpr uint32_t kCarSoundCount = 7;
// Car sound i's address as the loops form it: 0 (each time) while the array
// is 0.
inline uint32_t CarSoundAt(uint8_t* base, uint32_t i) {
  const uint32_t array = schema::CarSoundsGlobals::sounds(base).address;
  return array == 0 ? 0 : array + schema::CarSound::kSize * i;
}

// Other areas' globals: the race data and the race's progress
// (race-states.toml), the racers' manager (race-world.toml), the floats 0.0
// and 1.0 (race-states.toml's) and 0.1 (ai-drivers.toml's), "Music"
// (menu-sounds.toml), the pad blocked words (boot-mode.toml), the music's
// byte (car-sounds.toml). The volumes, the effects' bank arrived word and
// the scene models' sound slots are schema::RaceSoundsGlobals'.
using RaceStates = schema::RaceStatesGlobals;
using CarSounds = schema::CarSoundsGlobals;

// The machine (itc_recomp/schema/race-sounds.toml) and the area's globals.
using Globals = schema::RaceSoundsGlobals;
inline schema::RaceSounds Sounds(uint8_t* base, uint32_t machine) {
  return schema::RaceSounds(base, machine);
}

// The callees by address.
constexpr uint32_t kCarSoundPauseAll = 0x8210E2B8;
constexpr uint32_t kCarSoundResumeAll = 0x8210E350;
constexpr uint32_t kCarSoundsStopAll = 0x8210F7C8;
constexpr uint32_t kCarSoundsRelease = 0x8210F750;
constexpr uint32_t kCarSoundStop = 0x8210E218;
constexpr uint32_t kCarSoundStart = 0x8210E030;
constexpr uint32_t kCarSoundUpdate = 0x82110048;
constexpr uint32_t kMusicPlay = 0x821063B0;          // Music::Play (mode-tick.md)
constexpr uint32_t kSoundPlay = 0x82175850;
constexpr uint32_t kXPhysicalAlloc = 0x822964C8;
constexpr uint32_t kXPhysicalFree = 0x822965C8;
constexpr uint32_t kSoundHolderDelete = 0x82117B30;  // sound-cues.md
constexpr uint32_t kArchiveLoad = 0x8209DE48;
constexpr uint32_t kOperatorNew = 0x825A6880;
constexpr uint32_t kXprConstruct = 0x82596E68;    // XPR::XPR (models.md)
constexpr uint32_t kOnEffectsBankLoaded = 0x82175ED0;
constexpr uint32_t kEffectsBankId = 22889;          // Sound::Play (menu-sounds.md)
constexpr uint32_t kRaceSoundsPauseCars = 0x82175C00;
constexpr uint32_t kRaceSoundsResumeCars = 0x82175C58;
constexpr uint32_t kCarSoundsInit = 0x8210FE00;

// A guest function called from the frame at `r1`, with the first `set` of
// r3 to r6 given (the others left as they are) and the link register `lr`
// (the original's return address); its r3 (64 bits). The adapter's job
// (hooks/race_sounds.cpp).
struct Calls {
  void* user;
  uint64_t (*call)(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                   uint32_t lr);
  // r4 and r5 as a native step the original makes as a call leaves them
  // (CarSounds::SetBanks' r4 and r5), given to the next call that doesn't
  // set them (the original's leftovers), then forgotten.
  mutable bool left = false;
  mutable uint64_t left_r4 = 0, left_r5 = 0;

  // At a native function's end: what it left isn't passed on (its
  // adapter's calls object ends there too).
  void Forget() const { left = false; }
  void Leave(uint64_t r4, uint64_t r5) const {
    left = true;
    left_r4 = r4;
    left_r5 = r5;
  }
  // With up to r3 to r8 (`set` of them; the adapter reads `args` past r6
  // only when `set` is above 4).
  uint64_t CallArgs(uint32_t function, uint32_t r1, uint32_t lr, int set,
                    const uint64_t (&args)[8]) const {
    left = false;
    return call(user, function, args, set, r1, lr);
  }
  uint64_t Call(uint32_t function, uint32_t r1, uint32_t lr, int set, uint64_t r3,
                uint64_t r4 = 0, uint64_t r5 = 0, uint64_t r6 = 0) const {
    uint64_t args[4] = {r3, r4, r5, r6};
    if (left) {
      if (set < 2) {
        args[1] = left_r4;
      }
      if (set < 3) {
        args[2] = left_r5;
        set = 3;
      }
      left = false;
    }
    return call(user, function, args, set, r1, lr);
  }
};

// --- The handlers and their helpers (race_sounds.cpp) ---
//
// Each returns r3 as the original leaves it (`r3` itself, the machine's
// register, where it calls nothing).

// sub_821758A0 RaceSounds::SetListener (112 bytes): slot 26 with r4, r5
// (and r6 as given), slot 25 with `position`, slot 27 with `velocity` (r5
// and r6 as the slots before leave them).
uint64_t SetListener(uint8_t* base, uint64_t position, uint64_t front, uint64_t top,
                     uint64_t velocity, uint32_t sp, const Calls& calls);
// sub_82175908 RaceSounds::FadeMusic (112 bytes): the music's volume down by
// a tenth of the kept one (slot 30), 1 once silent (or with nothing kept),
// else 0.
uint64_t FadeMusic(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821759C8 RaceSounds::RestoreMusicVolume (112 bytes): the music's
// volume = the kept one (slot 30).
uint64_t RestoreMusicVolume(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82175A40 RaceSounds::EndTick (112 bytes): once (flag 0x20 clear) slot
// 13 on bank 2's cue 0, 0x20 set, CarSound::PauseAll on each of the seven.
uint64_t EndTick(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82175AE8 RaceSounds::Pause and sub_82175B80 Resume (112 bytes): the
// car sounds paused (each) or resumed (the first, seven times), then the
// "Music" category paused or resumed (slots 29 and 31 or 32).
uint64_t Pause(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t Resume(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82175C00 RaceSounds::PauseCars and sub_82175C58 ResumeCars (112
// bytes): the car sounds alone.
uint64_t PauseCars(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t ResumeCars(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82175D20 RaceSounds::OnStageCountdown (leaf): the effects' volume
// back, the cars' step 1, flag 0x4000 cleared.
uint64_t OnStageCountdown(uint8_t* base, uint64_t r3);
// sub_82175D48 RaceSounds::OnRollingStartDone (96 bytes): with 0x20000 set
// and 0x4000 clear, CarSounds::StopAll, 0x4000 set and 0x10000 cleared; the
// cars' step 1 and 0x4000 cleared.
uint64_t OnRollingStartDone(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82175CF8 RaceSounds::OnAbortDone (leaf; not seen in play): the cars'
// step 1, flag 0x4000 cleared, the effects' volume back.
uint64_t OnAbortDone(uint8_t* base, uint64_t r3);
// sub_821772D8 RaceSounds::Unload (144 bytes): CarSounds::Release; each
// slot's two buffers freed (XPhysicalFree, the pointers left) and the car's
// two views deleted (sub_82117B30, the pointers cleared); slot 5
// (ReleaseBank) on bank 2; the effects' bank freed (XPhysicalFree's r3).
uint64_t Unload(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82175ED0 RaceSounds::OnEffectsBankLoaded (leaf): the effects' bank
// arrived (r3 left).
void OnEffectsBankLoaded(uint8_t* base);

// --- The cars' sounds and the music's picks (race_sounds_cars.cpp) ---

// sub_82176188 RaceSounds::SetUpCars (224 bytes): +9656 = 10; each car
// whose files and the effects' bank have arrived stopped, its views set up,
// started, given its banks and its vehicle (the cars' step 2 and 0x20000
// set for each).
uint64_t SetUpCars(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82175F20 RaceSounds::UpdateCars (240 bytes): the wait counted down,
// or each racer's car set up again when asked, each racer's car sound
// updated, the car sounds paused or resumed by the system UI.
uint64_t UpdateCars(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_821764F0 RaceSounds::PickRivalMusic (112 bytes): the next music's flag
// from the last result's item.
uint64_t PickRivalMusic(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_821762F8 RaceSounds::PlayRivalMusic (144 bytes): each music flag set
// played (Music::Play) and cleared.
uint64_t PlayRivalMusic(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);

// --- The music's ticks and the events that fade it (race_sounds_ticks.cpp) ---

// sub_821769D0 RaceSounds::RaceTick (160 bytes): the cars' step 1 SetUpCars,
// 2 UpdateCars and the music steps (the effects' volume put back, the next
// music PlayRivalMusic's).
uint64_t RaceTick(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82176F28 RaceSounds::StageTick and sub_82177130 RollingStartTick (160
// bytes): the music steps only (RollingStartTick's putting the effects'
// volume back).
uint64_t StageTick(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t RollingStartTick(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82176C10 RaceSounds::AfterRaceTick (176 bytes): UpdateCars and the
// music steps, the next music by the race's result, cue 0 not played while
// player 0's save data's +6100 is 1.
uint64_t AfterRaceTick(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// The events (96 bytes): sub_82176968 OnCountdown (CarSound::Update on the
// first car sound, the fade, PickRivalMusic), sub_82176BA0 OnRaceEnd (the
// effects' volume times 0.0, UpdateCars, the fade), sub_82176E60
// OnLeaveAfterRace (UpdateCars, the fade, the effects' volume back),
// sub_82176EC8 OnNextStage (CarSounds::StopAll, the fade with 0x4000,
// PickRivalMusic), sub_821770C0 OnRollingStart (SetUpCars with the cars'
// step 1, the fade, PickRivalMusic).
uint64_t OnCountdown(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t OnRaceEnd(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t OnLeaveAfterRace(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t OnNextStage(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t OnRollingStart(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82175CA0 RaceSounds::OnAbort (96 bytes; not seen in play):
// CarSounds::StopAll and the fade with 0x4000 (as OnNextStage without
// PickRivalMusic); StopAll's r3.
uint64_t OnAbort(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82175DB0 RaceSounds::PlayStartMusic (144 bytes): the start's music by
// the race kind (entry 0, or one of entries 1 to 5 by rand() % 5 for kinds
// 2 and 4 to 6; rand natively, c_runtime::Rand).
uint64_t PlayStartMusic(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_821767A8 RaceSounds::IdleTick (160 bytes): as RaceTick without the
// effects' volume, the next music PlayStartMusic's.
uint64_t IdleTick(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82176618 RaceSounds::Begin (192 bytes): the effects' bank allocated
// (XPhysicalAlloc) and loaded (Archive::Load with OnEffectsBankLoaded), the
// cars' views made (operator new, XPR::XPR), PlayStartMusic, the listener
// at the origin (SetListener), the cars' step 1.
uint64_t Begin(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);

// --- The machine's set-up (race_sounds_init.cpp) ---

// Init's frame (its r1 is `sp` less this).
constexpr uint32_t kInitFrame = 3280;
// sub_82177378 RaceSounds::Init: the race kind (+9652) from the race data,
// the seven sound slots reset, their buffers allocated (XPhysicalAlloc) and
// cleared, CarSounds::Init, the state (+9604) = 1 and the transitions for
// the kind (StateMachine::Add), replayed in the original's frame (see
// race_sounds_init.cpp). Returns the last Add's answer (`replaced` as it
// says).
uint32_t Init(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls, bool* replaced);

}  // namespace race_sounds
