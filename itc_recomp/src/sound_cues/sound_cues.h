// The sound cues as plain C++ (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/sound-cues.md): the game's sound manager
// (AudioManager), its Sounds and bank pairs, and the game's thin wrapper
// over XACT and X3DAudio (the emitters, the bank holders, the listener).
// Only the work that doesn't call into XACT is here: XACT is still the
// recompiled library (its translation is a lead), so the functions whose
// work is a call through an XACT vtable stay recompiled for now. The
// objects are guest objects shared with the recompiled code, so these work
// on guest memory. Adapters: src/hooks/sound_cues.cpp.

#pragma once

#include <cstdint>

namespace sound_cues {

// --- The listener (the manager's +1456, 56 bytes: an X3DAudio listener,
// front +0, top +12, position +24, velocity +36, then two floats) ---

// Listener::Listener (sub_8259D478): front (0, 0, 1), top (0, 1, 0),
// position (0, 0, [0x82001FD8]), velocity 0, +48 = [0x82002A0C], +52 =
// [0x820027A4].
void ListenerConstruct(uint8_t* base, uint32_t listener);
// Listener::SetOrientation (sub_8259D550): the front and top vectors (three
// floats each) into +0 and +12.
void ListenerSetOrientation(uint8_t* base, uint32_t listener, uint32_t front, uint32_t top);
// Listener::SetPosition (sub_8259D588): three floats into +24.
void ListenerSetPosition(uint8_t* base, uint32_t listener, uint32_t position);
// sub_8259CC80: three floats into +36 (an emitter's X3DAudio position, or
// the listener's velocity).
void SetVector36(uint8_t* base, uint32_t object, uint32_t vector);

// AudioManager::SetListenerPosition, SetListenerOrientation and
// SetListenerVelocity (sub_82117E08, sub_82117E10, sub_82117E18): the
// above on the manager's listener (+1456).
void ManagerSetListenerPosition(uint8_t* base, uint32_t manager, uint32_t position);
void ManagerSetListenerOrientation(uint8_t* base, uint32_t manager, uint32_t front, uint32_t top);
void ManagerSetListenerVelocity(uint8_t* base, uint32_t manager, uint32_t velocity);
// AudioManager::GetEngine (sub_82117EA8): r3 + 12 (on the interface at the
// manager's +4, the audio engine at +16).
uint32_t ManagerGetEngine(uint32_t interface);

// --- The bank pairs (112 bytes) and the manager's bank slots ---

// BankPair::Release (sub_82117760): byte +69 = 1.
void BankPairRelease(uint8_t* base, uint32_t pair);
// BankPair::SoundBank (sub_82117770): +76.
uint32_t BankPairSoundBank(uint8_t* base, uint32_t pair);
// BankPair::IsLoading (sub_82117670): 0 when both loaded bytes (+108,
// +109) are set, else 1.
uint32_t BankPairIsLoading(uint8_t* base, uint32_t pair);

// The manager's bank slots: a vector of words at +1516 (begin) and +1520
// (end), indexed by the word at `index_address`. Each returns false where
// the original traps (no vector, or the index past its end: twi 31), so
// the adapter runs the original there.
//
// AudioManager::ReleaseBank (sub_82118190): BankPair::Release on the
// slot's pair when there is one. `r3` gets what the original leaves there
// (the pair, or the manager for an empty slot).
bool ManagerReleaseBank(uint8_t* base, uint32_t manager, uint32_t index_address, uint32_t* r3);
// AudioManager::IsBankLoading (sub_82118200): 0 for an empty slot, else
// BankPair::IsLoading.
bool ManagerIsBankLoading(uint8_t* base, uint32_t manager, uint32_t index_address,
                          uint32_t* result);
// AudioManager::IsBankSlotEmpty (sub_821182E0): 1 when the slot holds no
// pair.
bool ManagerIsBankSlotEmpty(uint8_t* base, uint32_t manager, uint32_t index_address,
                            uint32_t* result);
// AudioManager::GetSoundBank (sub_82118400), on the interface at the
// manager's +8 (its +1508 and +1512 are the vector): BankPair::SoundBank of
// the slot's pair (the pair is read without a null check, as the original).
bool ManagerGetSoundBank(uint8_t* base, uint32_t interface, uint32_t index_address,
                         uint32_t* result);

// --- The Sounds (24 bytes: +0 the owner, +4 the emitter, bytes +8 3D, +9
// loop, +10 was playing, +11 start asked for, +16 the bank index, +20 the
// cue index) ---

// Sound::Start (sub_821173F0): byte +11 = 1.
void SoundStart(uint8_t* base, uint32_t sound);
// Sound::Set3D (sub_82117568): byte +8 = the byte at `value_address`.
void SoundSet3D(uint8_t* base, uint32_t sound, uint32_t value_address);
// Sound::SetPosition (sub_82117560): sub_8259CC80 on the emitter (+4).
// Returns the emitter (the original's r3).
uint32_t SoundSetPosition(uint8_t* base, uint32_t sound, uint32_t position);
// Sound::IsBank and Sound::IsCue (sub_821175B8, sub_82117578): 1 when the
// word at `index_address` equals +16 (+20).
uint32_t SoundIsBank(uint8_t* base, uint32_t sound, uint32_t index_address);
uint32_t SoundIsCue(uint8_t* base, uint32_t sound, uint32_t index_address);

// --- The emitters and the bank holders ---

// Emitter::Emitter (sub_8259C9B0): the 300-byte emitter's defaults (no
// cue, an X3DAudio emitter at +8 with its cone at +100 and azimuths at
// +132, and the cue's 3D settings block from +136); the bytes it doesn't
// set are left.
void EmitterConstruct(uint8_t* base, uint32_t emitter);
// SoundBankHolder::SoundBankHolder (sub_8259D3D8): +0, +4, +8 cleared.
void SoundBankHolderConstruct(uint8_t* base, uint32_t holder);
// WaveBankHolder::WaveBankHolder (sub_8259D418): +0, +4, +8 cleared, +12 =
// -1.
void WaveBankHolderConstruct(uint8_t* base, uint32_t holder);

// --- The manager's Sound list and handlers ---

// AudioManager::IsCuePlaying (sub_82118B58): 1 when a Sound in the list at
// +1528 (a std::list: its sentinel node at +1532, each node's +0 the next
// and +8 the Sound) is for the bank and cue at the two addresses
// (Sound::IsBank and Sound::IsCue), else 0.
uint32_t ManagerIsCuePlaying(uint8_t* base, uint32_t manager, uint32_t bank_address,
                             uint32_t cue_address);
// sub_82118720: a 16-byte copy of the member-function delegate at
// `delegate` (operator new; vtable 0x8200833C, +4 the object, +8 and +12
// the member pointer), or 0 when operator new gives 0. False when operator
// new fails (its new handler and std::bad_alloc are the original's).
bool DelegateClone(uint8_t* base, uint32_t delegate, uint32_t* copy);

// --- The audio engine's X3DAudio ---

// X3DAudioInitialize (sub_82480710, the X3DAudio library's): for the
// speaker mask, the speed of sound and the handle: +0 the channel count
// (the mask's bits; 1 for an empty mask, as the original's loop counts),
// +4 the LFE channel's index (-1 without bit 3, else the count of bits
// 0-2), +8 the speed, +12 the float just below it (its bits less one).
// Returns 0 (the original's r3).
uint32_t X3DAudioInitialize(uint8_t* base, uint32_t speaker_mask, double speed_of_sound,
                            uint32_t handle);
// AudioEngine::Start3D (sub_8259CE10): X3DAudioInitialize for 5.1 (mask
// 63) and the speed of sound at 0x8207145C into the handle at +12; the DSP
// settings at +28: the matrix pointer (+76), one source and six destination
// channels (+36, +40), the matrix six 1.0. Returns 0.
uint32_t AudioEngineStart3D(uint8_t* base, uint32_t engine);

}  // namespace sound_cues
