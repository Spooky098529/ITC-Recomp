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

namespace race_sounds {

// The sound manager and its vtable's slots (bytes into it).
constexpr uint32_t kSoundManager = 0x826C5C80;
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

// The car sounds' array (seven of 1,264 bytes; 0 before CarSounds::Init).
constexpr uint32_t kCarSounds = 0x826C5C3C;
constexpr uint32_t kCarSoundBytes = 1264;
constexpr uint32_t kCarSoundCount = 7;

// The globals: "Music", the music's volume and the one kept before a fade,
// the effects' two volume words, the floats 0.0 and 0.1, the effects' bank
// arrived word.
constexpr uint32_t kMusicName = 0x82008348;
constexpr uint32_t kMusicVolume = 0x82616BEC;
constexpr uint32_t kKeptMusicVolume = 0x82616C34;
constexpr uint32_t kEffectsVolume = 0x82616C38;
constexpr uint32_t kEffectsVolume2 = 0x82616C3C;
constexpr uint32_t kZero = 0x82000A34;
constexpr uint32_t kTenth = 0x82001FBC;
constexpr uint32_t kEffectsBankArrived = 0x826D83C8;

// The machine's words.
constexpr uint32_t kFlags = 9644;
constexpr uint32_t kCarsStep = 9648;

// The callees by address.
constexpr uint32_t kCarSoundPauseAll = 0x8210E2B8;
constexpr uint32_t kCarSoundResumeAll = 0x8210E350;
constexpr uint32_t kCarSoundsStopAll = 0x8210F7C8;
constexpr uint32_t kCarSoundsRelease = 0x8210F750;

// A guest function called from the frame at `r1`, with the first `set` of
// r3 to r6 given (the others left as they are) and the link register `lr`
// (the original's return address); its r3 (64 bits). The adapter's job
// (hooks/race_sounds.cpp).
struct Calls {
  void* user;
  uint64_t (*call)(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                   uint32_t lr);

  uint64_t Call(uint32_t function, uint32_t r1, uint32_t lr, int set, uint64_t r3,
                uint64_t r4 = 0, uint64_t r5 = 0, uint64_t r6 = 0) const {
    const uint64_t args[4] = {r3, r4, r5, r6};
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
// sub_82175ED0 RaceSounds::OnEffectsBankLoaded (leaf): the effects' bank
// arrived (r3 left).
void OnEffectsBankLoaded(uint8_t* base);

}  // namespace race_sounds
