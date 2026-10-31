// The menu sounds natively (B7 in docs/rexglue-removal.md; the functions are
// in docs/functions/menu-sounds.md): the helpers the game's screens play
// their sounds and set the sound options through, over the sound manager at
// [0x826C5C80] (the AudioManager, docs/functions/sound-cues.md). Plain C++
// over guest memory with the originals' frame words; the manager's slots
// they call (PlayCue2D, GetCategory, SetCategoryVolume) aren't ours yet, so
// they're called through the function table (`Calls`). Adapters:
// src/hooks/menu_sounds.cpp.

#pragma once

#include <cstdint>

namespace menu_sounds {

constexpr uint32_t kSoundManager = 0x826C5C80;  // [it] the AudioManager
// The manager's slots: PlayCue2D (10), GetCategory (29), SetCategoryVolume
// (30).
constexpr uint32_t kPlayCue2DSlot = 40;
constexpr uint32_t kGetCategorySlot = 116;
constexpr uint32_t kSetCategoryVolumeSlot = 120;
// The floats 1.0 and 0.0 the originals load, and 1/15.
constexpr uint32_t kOne = 0x82000A30;
constexpr uint32_t kZero = 0x82000A34;
constexpr uint32_t kFifteenth = 0x82008060;
// The categories' names.
constexpr uint32_t kDefaultName = 0x8200A6A4;  // "Default"
constexpr uint32_t kMusicName = 0x82008348;    // "Music"
// The volume globals: the effects' (two words) and the music's.
constexpr uint32_t kEffectsVolume = 0x82616C38;
constexpr uint32_t kEffectsVolume2 = 0x82616C3C;
constexpr uint32_t kMusicVolume = 0x82616BEC;

// A guest function called through the function table from the frame at
// `r1`, with the first `set` of r3 to r6 given (the others left as they
// are: the original doesn't set them) and the link register `lr` (the
// original's return address); its r3 (64 bits). The adapter's job
// (hooks/menu_sounds.cpp).
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

// MenuSound::Play (sub_821103F0): cue `cue` of bank 0 through the manager's
// slot 10 (PlayCue2D) from the 112-byte frame below `sp` (+80 the float
// 1.0, +84 the cue, +88 the bank 0; r4 to r6 their addresses). The slot's
// r3.
uint64_t PlayMenuSound(uint8_t* base, uint32_t cue, uint32_t sp, const Calls& calls);

// Sound::Play (sub_82175850): the manager's slot 10 with r4, r5 = `bank`,
// `cue` (the indexes' addresses) and r6 = the 96-byte frame's +80 (the
// float 1.0). The slot's r3.
uint64_t PlaySound(uint8_t* base, uint32_t bank, uint32_t cue, uint32_t sp, const Calls& calls);

// SoundOptions::SetEffectsVolume (sub_821B9C20) and SetMusicVolume
// (sub_821B9CF0): `setting` (r3, its low byte sign-extended) as a gain (0.0
// for 0, 1.0 for 15, else the setting times 1/15, in single precision) at
// the 112-byte frame's +80; the category "Default" or "Music" (slot 29) at
// +88 and, unless it's -1, its volume set (slot 30, r4 = +88, r5 = +80);
// then +80 stored to the volume globals. r3 as the last slot leaves it.
uint64_t SetEffectsVolume(uint8_t* base, uint64_t setting, uint32_t sp, const Calls& calls);
uint64_t SetMusicVolume(uint8_t* base, uint64_t setting, uint32_t sp, const Calls& calls);

}  // namespace menu_sounds
