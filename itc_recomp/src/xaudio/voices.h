// XAudio's voices as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/xaudio.md): the Xbox 360's low-level audio
// library linked into the game, translated from the leaves up as the game's
// code (the user's call for XACT, 2026-10-23, followed for XAudio). The
// voices, their buffers and the engine are guest objects shared with the
// recompiled code (XACT, the car sounds, the mixer on our output thread), so
// these work on guest memory. Only work that takes neither of XAudio's two
// locks (the API lock 0x8264D518, the voice lists' spin lock 0x82954F10) is
// here so far: see the intro of docs/functions/xaudio.md. Adapters:
// src/hooks/xaudio.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace xaudio {

constexpr uint32_t kEngine = 0x82954F08;         // the engine's pointer
constexpr uint32_t kStats = 0x82954F24;          // the statistics counters
constexpr uint32_t kDefaultMatrices = 0x8204A7B8;  // the default output matrices
constexpr uint32_t kRootVtable = 0x8204A6B8;     // the voices' root vtable
constexpr uint32_t kSampleRate = 48000;

// The PCR's processor number (r13 + 268), as EngineBuffer reads it.
constexpr uint32_t kPcrProcessor = 268;

// --- Formats and descriptions ---

// XAudio::SourceOutputFormat (sub_8245DC88): the 8-byte voice format at
// `out` for the source format at `format`: +0 = 0, +1 the channels (for type
// 4, XMA, the sum of its +4 streams' channel bytes at +12 + 8i; else byte
// +4), +4 = 48,000.
void SourceOutputFormat(uint8_t* base, uint32_t format, uint32_t out);

// XAudioSubmixVoice::Describe (sub_8245F010): the 32-byte voice description
// at `out` (cleared) for the submix description at `description`.
void SubmixDescribe(uint8_t* base, uint32_t description, uint32_t out);

// XAudio::DefaultMatrix (sub_8245EC40): the default matrix for `inputs` to
// `outputs` channels (their low bytes) written to the 8-byte entry at `out`
// (+0 the count, +4 the coefficients in the table at 0x8204A7B8); returns
// `out`, or 0 (nothing written) for pairs it doesn't cover.
uint32_t DefaultMatrix(uint8_t* base, uint32_t inputs, uint32_t outputs, uint32_t out);

// XAudioBuffer::Size (sub_8245FA28): the size of a buffer for the 8-byte
// voice format at `format` to [out]; returns 0. nullopt, with nothing
// written, when the format's rate is 0 or above 48,000 (the original
// traps on its divisions).
std::optional<uint32_t> BufferSize(uint8_t* base, uint32_t format, uint32_t out);

// XAudioBuffer::Clear (sub_8245F8E8): checks the format at `format` (0: the
// buffer's own, +12) against the buffer's capacity and current channels,
// clears its samples (+28) as `flags` asks, sets +21 to the format's
// channels; returns the original's r3 (0, E_OUTOFMEMORY or E_UNEXPECTED).
// nullopt, with nothing done, where the original traps (a rate of 0 or
// above 48,000 among the format's, +16 and +24).
std::optional<uint32_t> BufferClear(uint8_t* base, uint32_t buffer, uint32_t format,
                                    uint32_t flags);

// --- Small accessors ---

// XAudioVoice::EngineBuffer (sub_8245DEB0): the engine's buffer for
// processor `processor` (the engine's +12 + 8 x the processor).
uint32_t EngineBuffer(uint8_t* base, uint32_t processor);

// XAudioVoice::GetType (sub_8245E318): byte +12 to [out]; returns 0.
uint32_t GetType(uint8_t* base, uint32_t voice, uint32_t out);
// XAudioVoice::GetFormat (sub_8245E328): the 8 bytes at +52 to [out];
// returns 0.
uint32_t GetFormat(uint8_t* base, uint32_t voice, uint32_t out);
// XAudioVoice::SetFormatWords (sub_82461140): the 8 bytes at `format` to
// +52.
void SetFormatWords(uint8_t* base, uint32_t voice, uint32_t format);
// XAudioVoice::AddRef (sub_8245F860): +4 up by one; returns it.
uint32_t AddRef(uint8_t* base, uint32_t object);
// sub_8245FB00: the root vtable (0x8204A6B8) to +0.
void SetRootVtable(uint8_t* base, uint32_t object);

// XAudio::AddStat (sub_8245F640): `amount` added atomically to the counter
// `index` (0x82954F24 + 4 x the index); returns 0.
uint32_t AddStat(uint8_t* base, uint32_t index, uint32_t amount);

}  // namespace xaudio
