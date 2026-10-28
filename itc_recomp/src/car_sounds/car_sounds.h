// The car sounds natively (docs/functions/car-sounds.md): the game's own
// sounds for the cars over XAudio's voices, their wave bank views and their
// objects' set-up. Plain C++ over the guest layout; the adapters are in
// src/hooks/car_sounds.cpp (behind --native_car_sounds). XAudio's voice
// methods (8245xxxx) stay recompiled.

#pragma once

#include <cstdint>
#include <optional>

namespace car_sounds {

// The voice holder's vtable (the 44-byte base of every sound part) and the
// engine layer's.
constexpr uint32_t kHolderVtable = 0x82010B5C;
constexpr uint32_t kLayerVtable = 0x82008018;
constexpr uint32_t kCarSoundBytes = 1264;

// --- The wave bank views (24 bytes over an XWB in memory) ---

// sub_8210C9D8 WaveBankView::Init: the view over `bank`; returns 0 (r3).
uint32_t WaveBankViewInit(uint8_t* base, uint32_t view, uint32_t bank);
// sub_8210C850 WaveBankView::Format: wave `wave`'s PCM format into `out`
// (+0 the sample type, +4 the channels, +8 the rate); 1 (nothing written)
// past the count or for a format other than PCM, else 0.
uint32_t WaveBankFormat(uint8_t* base, uint32_t view, uint32_t wave, uint32_t out);
// sub_8210C920 WaveBankView::Length: the entry's +12 into `out`; 1 past the
// count, else 0.
uint32_t WaveBankLength(uint8_t* base, uint32_t view, uint32_t wave, uint32_t out);
// sub_8210C958 WaveBankView::Data: the entry's +8 plus the wave data's
// start into `out`; 1 past the count, else 0.
uint32_t WaveBankData(uint8_t* base, uint32_t view, uint32_t wave, uint32_t out);
// sub_8210C998 WaveBankView::Entry: the wave's 24-byte entry copied into
// `out` (XMemCpy); 0. Nullopt, with nothing copied, where XMemCpy's
// original must run.
std::optional<uint32_t> WaveBankEntry(uint8_t* base, uint32_t view, uint32_t wave, uint32_t out);

// --- The car sound objects ---

// sub_8210CB48 CarSounds::SetBanks: the car's sound parameters
// [[0x82616318 + 16 i] + 4 j] into the car sound's banks.
constexpr uint32_t kSoundParameters = 0x82616318;
void SetBanks(uint8_t* base, uint32_t sound, uint32_t i, uint32_t j);
// sub_8210CA28 TireSound::Amount: the tires' part's +80 and +76 stepped by
// `f1` (a double, as the register holds it).
void TireSoundAmount(uint8_t* base, uint32_t part, double f1);
// sub_8210CBA0 EngineSound::EngineSound: the engine part's two layers.
void EngineSoundConstruct(uint8_t* base, uint32_t engine);
// sub_8210CC18 CarSound::CarSound: the engine and the ten holders; returns
// the car sound.
uint32_t CarSoundConstruct(uint8_t* base, uint32_t sound);

// sub_8210DCA8 CarSounds::Destroy (the vector deleting destructor): the
// vtables put back, for the array (flags bit 1; the count at r3 - 16) or one
// car sound. Returns r3 as the original leaves it (the array's block or the
// object) and, in `to_free`, the block operator delete (`array`: delete[])
// is to free (flags bit 0), or 0.
struct Destroyed {
  uint32_t r3;
  uint32_t to_free;
  bool array;
};
Destroyed CarSoundsDestroy(uint8_t* base, uint32_t sound, uint32_t flags);

}  // namespace car_sounds
