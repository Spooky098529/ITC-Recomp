// XAudio's engine and mixer as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/xaudio-engine.md): the part of XAudio, the
// Xbox 360's low-level audio library linked into the game, under its voices
// (voices.h), translated as the game's code (the user's call for XACT,
// 2026-10-23, followed for XAudio). The engine, its factory, allocators and
// mastering voice are guest objects shared with the recompiled code (the
// engine's set-up, the mixer's callback and XAudio's processor threads,
// which stay recompiled: they make, set and wait on kernel objects with no
// precedent of their own), so these work on guest memory. Calls into the
// recompiled objects go through `call` (voices.h's GuestCall). Adapters:
// src/hooks/xaudio_engine.cpp.

#pragma once

#include "xaudio/voices.h"

#include <cstdint>

namespace xaudio {

constexpr uint32_t kStatsCopy = 0x82954F40;  // the counters' copies (TakeStats)
constexpr uint32_t kStatCount = 7;

// --- Leaves ---

// XAudioFactory::TableSize (sub_8245D9E8): for the class table at [table]
// (+0 the count byte, +4 the 12-byte entries, +0 each the class id), 8 x
// (the largest id + 1); 0 with no pointer or no table.
uint32_t FactoryTableSize(uint8_t* base, uint32_t table);

// XAudioFactory::SizeOf (sub_8245DBD0): FactoryTableSize + 24 to [out];
// returns 0.
uint32_t FactorySizeOf(uint8_t* base, uint32_t table, uint32_t out);

// XAudioBuffer::GetFormat (sub_8245F6F0): the buffer's +20, +24 and +28 to
// the 12 bytes at `out`; returns 0.
uint32_t BufferGetFormat(uint8_t* base, uint32_t buffer, uint32_t out);

// XAudioBuffer::GetFormatThunk (sub_8245F718): BufferGetFormat on the
// object 8 bytes before the interface at `iface` (0 stays 0).
uint32_t BufferGetFormatThunk(uint8_t* base, uint32_t iface, uint32_t out);

// sub_8245F8D0: the buffer's interface (`buffer` + 8, or 0) to [out]. The
// original leaves r3 as it was.
void BufferInterface(uint8_t* base, uint32_t buffer, uint32_t out);

// XAudioAllocator::Remaining (sub_82460388): +16 less +20, as the original's
// 64-bit subtraction of the two words leaves r3.
uint64_t AllocatorRemaining(uint8_t* base, uint32_t allocator);

// sub_82460370: the +4 interface's AddRef: the word at `iface` + 8 up by
// one; returns it (the original's 64-bit sum).
uint64_t InterfaceAddRef(uint8_t* base, uint32_t iface);

// sub_82461EF8: byte +0 = 6, half-word +2 = 3,750 at `out`; returns 0.
uint32_t ObjectInfo(uint8_t* base, uint32_t out);

// XAudioEngine::TakeStats (sub_8245F678): the 7 statistics counters
// (kStats) copied to kStatsCopy and cleared, one at a time; returns 0.
uint32_t TakeStats(uint8_t* base);

// XAudio::Free (sub_8245EF90): XMemFree (sub_82297790)'s plain C++ with
// `block` and `attributes`. Only while hooks::NativeHeapActive().
uint32_t Free(uint32_t block, uint32_t attributes);

// XAudioAllocator::Alloc (sub_82460288): `size` rounded up to 4; 0 when
// more than what's left (the allocator's slot +16), else the block (+24)
// plus what's used (+20), the used raised. Returns the original's r3 (the
// 64-bit sum of the two words).
uint64_t AllocatorAlloc(uint8_t* base, uint32_t allocator, uint32_t size, const GuestCall& call);

// sub_8245AAB8: an object's deleting destructor (its slot +0, with r4 1),
// tail-called: the method to `function`, 0 for no object (nothing called).
void DeleteObject(uint8_t* base, uint32_t object, uint32_t* function);

// XAudioEngine::Barrier (sub_8245A940): nothing without processor threads
// (+304); else this processor's byte (`processor`, r13's +268) of the
// 8-byte barrier at `barrier` set to 1, then a spin until the word holds a
// 1 for each processor with a thread (+308 + 4i), cleared then, or until
// another processor has cleared it.
void Barrier(uint8_t* base, uint32_t engine, uint32_t barrier, uint32_t processor);

}  // namespace xaudio
