// The menu sounds' register steps (src/menu_sounds/,
// docs/functions/menu-sounds.md), shared by the adapters
// (src/hooks/menu_sounds.cpp, behind --native_menu_sounds) and the harness
// (without the flag). Each leaves r3 as the original does (the last slot's).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// MenuSound::Play sub_821103F0: r3 = the cue.
void MenuSoundPlay(lifted::Regs& ctx, uint8_t* base);
// Sound::Play sub_82175850: r3, r4 = the bank and cue indexes' addresses.
void SoundPlay(lifted::Regs& ctx, uint8_t* base);
// SoundOptions::SetEffectsVolume sub_821B9C20 and SetMusicVolume
// sub_821B9CF0: r3 = the setting (its low byte).
void SoundOptionsSetEffectsVolume(lifted::Regs& ctx, uint8_t* base);
void SoundOptionsSetMusicVolume(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
