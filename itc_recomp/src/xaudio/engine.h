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

// --- The engine's making (its parts that use no kernel object) ---

constexpr uint32_t kEngineVtable0 = 0x8204A6CC;  // the base's
constexpr uint32_t kEngineVtable = 0x8204A6E0;
constexpr uint32_t kFactoryVtable = 0x8204A6F4;
constexpr uint32_t kDefaultClasses = 0x8204A7B0;  // the factory's class table
constexpr uint32_t kDebugMonitor = 0x820007F0;    // the debug monitor's data
constexpr uint32_t kStatsCallback = 0x8245F4E0;
constexpr uint32_t kKeGetCurrentProcessType = 0x825DB934;
constexpr uint32_t kBufferSize = 0x8245FA28;
constexpr uint32_t kOneFloatBits = 0x3F800000;   // 1.0f

// XAudioEngine::XAudioEngine (sub_8245C098): the 372-byte engine at `engine`
// with the allocator `allocator` (AddRef'd, its slot +0, when set): the
// vtables, the count, the start-pending list (+68), the source voices'
// stage (+80, its two lists at +92 and +104), the categories' volumes (+132,
// +136, 1.0); then [kEngine] = the engine. Returns the engine.
uint32_t EngineConstruct(uint8_t* base, uint32_t engine, uint32_t allocator,
                         const GuestCall& call);

// XAudioFactory::XAudioFactory (sub_8245DA48): the 24-byte factory at
// `factory` for the class table at [table], its entries (8 bytes an id, the
// largest id + 1 of them, at +8) from the allocator's slot +20 into +12, each
// class's two functions (its table entry's +4, +8) at its id's. `frame` is
// this function's (112 bytes). Returns the factory.
uint32_t FactoryConstruct(uint8_t* base, uint32_t factory, uint32_t table, uint32_t allocator,
                          uint32_t frame, const GuestCall& call);

// XAudioFactory::Create (sub_8245DC10): 24 bytes from the allocator's slot
// +20, FactoryConstruct (in a frame below `frame`), the factory's interface
// (+8) to [out]. `frame` is this function's (112 bytes). Returns 0 or
// E_OUTOFMEMORY.
uint32_t FactoryCreate(uint8_t* base, uint32_t table, uint32_t allocator, uint32_t out,
                       uint32_t frame, const GuestCall& call);

// The factory's create (sub_8245D650) and size (sub_8245D6D8) slots: the
// class entry of the descriptor's id (byte +0: below 128 in +8/+12, from
// 128 in +16/+20), its first (`which` 0) or second (`which` 1) function to
// `function`, tail-called by the adapter; E_NOTIMPL (0x80004001), and no
// function, when the id has no entry.
uint32_t FactoryEntry(uint8_t* base, uint32_t factory, uint32_t descriptor, uint32_t which,
                      uint32_t* function);

// XAudio::RegisterStats (sub_8245F588): the request (4 and the callback
// kStatsCallback at the frame's +80) handed to the debug monitor's
// ([[kDebugMonitor]]) slot +24 with 47, when there is one. `frame` is this
// function's (96 bytes). Returns 0.
uint32_t RegisterStats(uint8_t* base, uint32_t frame, const GuestCall& call);

// XAudio::SetUpProcessors (sub_8245A828): the 8 bytes at `out` from the
// processor mask `mask` (its low 6 bits; 0 means 16) and the kernel's
// KeGetCurrentProcessType (through `call` at its import's thunk): for the
// title's process (1), each masked hardware thread's byte (+2 + i) 2 and
// +0, +1 counted up, then the main one 1 (thread 4 when masked, else the
// highest); otherwise +1 = 1, +6 = 4. `frame` is this function's (112
// bytes). Returns the kernel's answer (the original's r3).
uint32_t SetUpProcessors(uint8_t* base, uint32_t mask, uint32_t out, uint32_t frame,
                     const GuestCall& call);

// XAudioEngine::Size (sub_8245B2C0): the engine's size for the parameters
// at `params` to [out]: the factory's (FactorySizeOf), an output buffer's
// for max(+0, 6) channels at 48 kHz (XAudioBuffer::Size's plain C++, its
// original where that refuses) times the processors (SetUpProcessors with
// +2, in a frame below) times 2, + 44 x the stage count (+1) + 4 when there
// are stages. `frame` is this function's (176 bytes). Returns the buffer
// size's r3 (nothing written on a failure).
uint32_t EngineSize(uint8_t* base, uint32_t params, uint32_t out, uint32_t frame,
                    const GuestCall& call);

// --- The mastering voice ---

constexpr uint32_t kMasteringVtable = 0x8204A8E8;
constexpr uint32_t kMasteringAttributes = 0x61820006;
constexpr uint32_t kDriverVtable = 0x8204AAEC;
constexpr uint32_t kDriverVtable2 = 0x8204AAD0;  // its +4 interface
constexpr uint32_t kDriverVtableFirst = 0x8204AA58;  // +4's first, written over
constexpr uint32_t kBufferClearThunk = 0x8245FA10;

// XAudioMasteringVoice::Size (sub_8245FF68): the mastering voice's size for
// the description at `description` (3 words, or 0) to [out]: a 32-byte
// voice description at the frame's +96 (6 channels at 48,000 Hz, +8, +16
// and +20 the three words; +24 on left as the stack had them), the base's
// size (XAudioVoice::BaseSize's plain C++, in a frame below), + 20. `frame`
// is this function's (144 bytes). Returns BaseSize's r3.
uint32_t MasteringSize(uint8_t* base, uint32_t description, uint32_t out, uint32_t frame,
                       const GuestCall& call);

// AudioRenderDriver::AudioRenderDriver (sub_82465938): the 20-byte driver
// object at `driver` (+0 and +4 the vtables, +8 = 1, +12 the description's
// +4, the voice). The original leaves r3 as it was.
void RenderDriverConstruct(uint8_t* base, uint32_t driver, uint32_t description);

// XAudioMasteringVoice::Init (sub_8245FCA0): the voice description as
// MasteringSize's at the frame's +96, XAudioVoice::InitBase (in a frame
// below); then a driver description at the frame's +80 (type 2, +4 the
// voice), 20 bytes from the voice's allocator (+8) slot +20, the driver
// constructed into +68 (E_OUTOFMEMORY without), its slot +32 (r4 the
// description, r5 the allocator) and the voice's slot +56 (its start).
// `frame` is this function's (144 bytes). Returns the last r3.
uint32_t MasteringInit(uint8_t* base, uint32_t voice, uint32_t description, uint32_t frame,
                       const GuestCall& call);

// XAudioMasteringVoice::Create (sub_824600F0): the size (MasteringSize, +
// 72), the allocator (XAudio::GetAllocator's plain C++ with
// kMasteringAttributes into the frame's +80; its original where that
// can't), 72 bytes from it (slot +20), the base voice (XAudioVoice::
// Construct, type 2) with the mastering vtable, MasteringInit; on success the
// voice to [out], else its slot +12; the allocator released (slot +4).
// `frame` is this function's (128 bytes); `native_heap` whether the default
// allocator may run. Returns 0 or the failure.
uint32_t MasteringCreate(uint8_t* base, uint32_t description, uint32_t out, bool native_heap,
                         uint32_t frame, const GuestCall& call);

// XAudioMasteringVoice::Start (sub_8245FE20): when not playing (+61 bit 0),
// its buffer (+32) cleared (XAudioBuffer::ClearThunk, flags 3; its original
// where the plain C++ refuses), then XAudioVoice::Play's plain C++. `frame`
// is this function's (96 bytes). Returns 0 (Play's).
uint32_t MasteringStart(uint8_t* base, uint32_t voice, uint32_t frame, const Thread& thread,
                        const GuestCall& call);

// XAudioMasteringVoice::Pass (sub_8245FDC8): XAudioVoice::Pass's plain C++
// (in a frame below), then on success its buffer (+32) cleared as Start's.
// `frame` is this function's (112 bytes). Returns Pass' r3.
uint32_t MasteringPass(uint8_t* base, uint32_t voice, uint32_t frame, const GuestCall& call);

// XAudioMasteringVoice::Process (sub_8245FB80): XAudioVoice::
// ProcessEffects' plain C++ over [inout] (in a frame below; `processor`
// r13's +268), then the driver's (+68) slot +24 with [inout] and 0 (the
// frame to our audio output). `frame` is this function's (112 bytes).
// Returns the last r3.
uint32_t MasteringProcess(uint8_t* base, uint32_t voice, uint32_t inout, uint32_t processor,
                          uint32_t frame, const GuestCall& call);

}  // namespace xaudio
