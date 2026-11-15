// AudioManager::Update's callees as plain C++ (docs/functions/sound-cues.md;
// src/sound_cues/manager_update.cpp). Each takes the stack pointer and r13
// it's called with (its calls go through the dispatch table from a frame
// made as the original's) and returns the original's r3.

#pragma once

#include <cstdint>

namespace sound_cues {

// AudioEngine::DoWork (sub_8259D1A8): the XACT engine's (+8) slot +16, a
// tail call; returns its r3.
uint64_t AudioEngineDoWork(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t engine);
// AudioManager::FreeReleasedBanks (sub_821184E8): each bank pair in the
// vector at +1516 that BankPair::CanFree (sub_821176E8) accepts deleted
// (sub_82118438), with the original's bounds checks (traps, never hit).
uint64_t ManagerFreeReleasedBanks(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t manager);
// AudioManager::RemoveStoppedSounds (sub_82118C18): the list at +1528's
// remove_if (sub_82118850) with DeleteIfStopped (sub_821185C8).
uint64_t ManagerRemoveStoppedSounds(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t manager);
// AudioManager::UpdateSounds (sub_82118FB0): std::for_each (sub_82118F50)
// of Sound::Update (sub_82117438) over the list at +1528.
uint64_t ManagerUpdateSounds(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t manager);

}  // namespace sound_cues
