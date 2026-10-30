// XAudio's engine and mixer as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/xaudio-engine.md): the part of XAudio, the
// Xbox 360's low-level audio library linked into the game, under its voices
// (voices.h), translated as the game's code (the user's call for XACT,
// 2026-10-23, followed for XAudio). The engine, its factory, allocators and
// mastering voice are guest objects shared with the recompiled code (the
// engine's set-up, which stays recompiled), so these work on guest memory. Calls into the
// recompiled objects go through `call` (voices.h's GuestCall). Adapters:
// src/hooks/xaudio_engine.cpp.

#pragma once

#include "xaudio/voices.h"

#include <cstdint>
#include <optional>

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

// --- Under the voice lists' spin lock (taken as the original does) ---
//
// Each takes the game's spin lock (voices.h's SpinTake and SpinRelease, the
// kernel's calls through `call`) as its inline code does; where the
// original releases with the depth, owner and IRQL it kept in registers
// from its take, those are the lock's words while it's held, so the release
// reads them.

// XAudioStage::Swap (sub_8245B208): under the spin lock, the stage's +36
// (the list to mix) and +40 (the list mixed) swapped.
void StageSwap(uint8_t* base, uint32_t stage, uint32_t thread, const GuestCall& call);

// XAudioEngine::SwapStageLists (sub_8245B720): under the spin lock,
// StageSwap on the source voices' stage (+80) and each of the +128 submix
// stages (+124, 44 bytes each; the count read again each time).
void SwapStageLists(uint8_t* base, uint32_t engine, uint32_t thread, const GuestCall& call);

// XAudioEngine::RunStage (sub_8245B480): until the stage's +36 list is
// empty, under the spin lock its first voice (by the list's node offset,
// +8) moved to the end of +40 and recorded at the engine's +144 + 4 x
// `processor`; out of it, the voice's slot +68 (its pass) and AddStat(0, 1)
// on success. Then the processor's entry cleared.
void RunStage(uint8_t* base, uint32_t engine, uint32_t stage, uint32_t processor,
              uint32_t thread, const GuestCall& call);

// XAudioEngine::RunCallbacks (sub_8245ACE0): under the spin lock, for each
// of the stage's (`stage`'s low byte) 8 callback slots (+168 + 64 x the
// stage + 8i: the function, its context), +296 = the slot; when set, the
// lock left, the function called with its context, the lock taken again.
// +296 = 0 at the end.
void RunCallbacks(uint8_t* base, uint32_t engine, uint32_t stage, uint32_t thread,
                  const GuestCall& call);

// XAudioEngine::RegisterCallback (sub_8245AB60): under the spin lock, the
// stage's first free slot of 8 set to `function` and `context`, the slot to
// [out] (unless 0). Returns 0, or E_OUTOFMEMORY when full.
uint32_t RegisterCallback(uint8_t* base, uint32_t engine, uint32_t stage, uint32_t function,
                          uint32_t context, uint32_t out, uint32_t thread, const GuestCall& call);

// XAudioEngine::Commit (sub_8245B638): under the spin lock, while the
// start-pending list (+68, its node offset at +76) isn't empty,
// XAudioSourceVoice::Start's plain C++ (flags 0) on its first voice, until
// one fails. Returns the last result (0 with none).
uint32_t Commit(uint8_t* base, uint32_t engine, const Thread& thread, const GuestCall& call);

// XAudioEngine::StageOf (sub_8245FBF0): the voice's type (its slot +52, at
// the frame's +80): 0 the source voices' stage (+80), 1 its submix stage
// ([+124] + 44 x the voice's +76), else 0. `frame` is this function's (112
// bytes).
uint32_t StageOf(uint8_t* base, uint32_t engine, uint32_t voice, uint32_t frame,
                 const GuestCall& call);

// sub_82460000: the voice's stage in the live engine ([kEngine], StageOf in
// a frame below); with one, under the spin lock, the voice appended to its
// +40 list (by the list's node offset). `frame` is this function's (128
// bytes). Returns 0.
uint32_t JoinMix(uint8_t* base, uint32_t voice, uint32_t thread, uint32_t frame,
                 const GuestCall& call);

// --- The allocators' destructors ---

constexpr uint32_t kAllocatorBaseVtable = 0x8204A940;  // +4 while the base destroys
constexpr uint32_t kFreeHeap = 0x82954F20;             // XAudio::Free's r3 (unused)
constexpr uint32_t kFree = 0x8245EF90;

// XAudioAllocator::~XAudioAllocator (sub_824602F8): its vtables
// (0x8204A954, 0x8204A940); its block (+24) freed (XAudio::Free with the
// attributes +12; its plain C++ when `native_heap`, else through `call`)
// and cleared; +4 the root vtable. `frame` is this function's (96 bytes).
// Returns the free's r3, or the allocator (the original's r3 untouched).
uint32_t AllocatorDestroy(uint8_t* base, uint32_t allocator, bool native_heap, uint32_t frame,
                          const GuestCall& call);

// sub_824604E8: the default allocator's destructor: its vtables
// (0x8204A954, 0x8204A980), +24 = 0 (its block is itself), then
// AllocatorDestroy (in a frame below). `frame` is this function's (96
// bytes). Returns the allocator.
uint32_t DefaultAllocatorDestroy(uint8_t* base, uint32_t allocator, bool native_heap,
                                 uint32_t frame, const GuestCall& call);

// sub_824603A8: the +4 interface's free: its slot +0 (the destructor, r4 1)
// through `call`, then the object (`iface` - 4) freed (XAudio::Free with its
// attributes, `iface` + 8) unless 0. `frame` is this function's (112
// bytes). Returns the last r3.
uint32_t InterfaceFree(uint8_t* base, uint32_t iface, bool native_heap, uint32_t frame,
                       const GuestCall& call);

// --- The mix and XAudio's threads ---
//
// Their kernel objects (the events and semaphore XAudioEngine::Init makes at
// fixed addresses) are used through the kernel's imports exactly as the
// original does, through `call` at the imports' thunks (the user's call,
// 2026-10-29: host objects later, once every user is native). The mixer's
// lock, a kernel critical section taken only here and by the engine's
// teardown (recompiled, never run), is entered through its import as the
// original does.

constexpr uint32_t kMixerLock = 0x8264D4F8;       // the mixer's critical section
constexpr uint32_t kMixNow = 0x82954EF8;          // a synchronization event
constexpr uint32_t kMixDone = 0x82954EE8;         // a synchronization event
constexpr uint32_t kTerminating = 0x82954EC4;     // a notification event
constexpr uint32_t kWorkerSemaphore = 0x82954ED4; // the workers' wake-up
constexpr uint32_t kKeWaitForSingleObject = 0x825DBBB4;
constexpr uint32_t kKeSetEvent = 0x825DBBD4;
constexpr uint32_t kKeReleaseSemaphore = 0x825DBE64;
constexpr uint32_t kKeWaitForMultipleObjects = 0x825DBE94;
constexpr uint32_t kMixPasses = 0x8245B808;  // audio.md's (an override of its own)

// XAudioEngine::Mix (sub_8245BF80): DebugSwitchOff's plain C++ (sub_825C8388),
// the mixer's lock entered, +300 = the thread (`thread.mixer`). With
// processor threads (+304): KeSetEvent(kMixNow, 1, 0), then
// KeWaitForMultipleObjects on kMixDone and kTerminating (wait any, reason 3,
// user mode, no timeout; the objects at the frame's +80, the wait block at
// +96, cleared as memset's 48 bytes); index 1 skips the mastering voice.
// Without: SwapStageLists' plain C++ (a frame below) and kMixPasses with 1.
// Then the mastering voice's (+64) slot +68 and AddStat(0, 1) on success; +300
// = 0; TakeStats; the lock left. `frame` is this function's (192 bytes).
// Returns 0.
uint32_t Mix(uint8_t* base, uint32_t engine, uint32_t frame, const Thread& thread,
             const GuestCall& call);

// XAudio::MixerCallback (sub_8245FBE0): Mix on [kEngine], tail-called (its
// frame `frame`, at the callback's caller's). Returns 0.
uint32_t MixerCallback(uint8_t* base, uint32_t frame, const Thread& thread,
                       const GuestCall& call);

// XAudio::MasterThread (sub_8245BE08): the thread (`thread.mixer`) at
// [kEngine] + 332 + 4 x its processor; then until the engine's +300 is 0
// when woken: KeWaitForSingleObject(kMixNow) (reason 3, user mode, no
// timeout); +300 set: SwapStageLists' plain C++ (a frame below) and
// kMixPasses with 1 on [kEngine]; else KeReleaseSemaphore(kWorkerSemaphore,
// 1, +304 - 1) unless that's 0; KeSetEvent(kMixDone, 1, 0) each time. `frame`
// is this function's (128 bytes). Returns 0 (in play, never).
uint32_t MasterThread(uint8_t* base, uint32_t frame, const Thread& thread,
                      const GuestCall& call);

// XAudio::WorkerThread (sub_8245BED0): the thread at [kEngine] + 332 + 4 x
// its processor; KeWaitForSingleObject(kWorkerSemaphore); while [kEngine]'s
// +300 is set, kMixPasses with 0 and the wait again. Returns 0 (in play,
// never).
uint32_t WorkerThread(uint8_t* base, const Thread& thread, const GuestCall& call);

// --- The engine's making ---
//
// Its kernel objects through the kernel's imports at their thunks, exactly
// as the original (the user's call, 2026-10-29): the events' headers built
// inline, the semaphore, the title terminate notification and the
// processor threads, whose entry points stay MasterThread's and
// WorkerThread's guest addresses.

constexpr uint32_t kTerminateRegistration = 0x82954EB4;
constexpr uint32_t kTerminateRoutine = 0x8245B0D0;
constexpr uint32_t kTerminatePriority = 0x7D800000;
constexpr uint32_t kThreadType = 0x820007FC;  // [it]: probably PsThreadObjectType
constexpr uint32_t kMasterEntry = 0x8245BE08;
constexpr uint32_t kWorkerEntry = 0x8245BED0;
constexpr uint32_t kMixerCallback = 0x8245FBE0;
constexpr uint32_t kEngineAttributes = 0x61820005;
constexpr uint32_t kObDereferenceObject = 0x825DB884;
constexpr uint32_t kKeSetBasePriorityThread = 0x825DB894;
constexpr uint32_t kObReferenceObjectByHandle = 0x825DB8A4;
constexpr uint32_t kExRegisterTitleTerminateNotification = 0x825DBA84;
constexpr uint32_t kExCreateThread = 0x825DBBE4;
constexpr uint32_t kKeResumeThread = 0x825DBEA4;
constexpr uint32_t kKeInitializeSemaphore = 0x825DBEB4;

// XAudioEngine::Init (sub_8245C198): RegisterStats; +128 the stage count
// (`params` +1) and +124 its stages (a count word, then 44 bytes a stage,
// each its three lists' heads and node offsets) from the allocator's (+8)
// slot +20 (E_OUTOFMEMORY without); FactoryCreate (the class table +4 or
// kDefaultClasses at the frame's +88) into +60; the events' headers
// (kMixNow, kMixDone synchronization, kTerminating notification) written
// inline, KeInitializeSemaphore(kWorkerSemaphore, 0, 6), the registration
// (kTerminateRoutine, kTerminatePriority) at kTerminateRegistration
// registered; +304 = 0; SetUpProcessors (+2, the frame's +96); for each
// processor byte with its low bits set, ExCreateThread (suspended, on that
// hardware thread; MasterThread for 1, WorkerThread else),
// ObReferenceObjectByHandle with [kThreadType], KeSetBasePriorityThread 15,
// KeResumeThread, ObDereferenceObject, +304 counted (E_OUTOFMEMORY when the
// create fails), its handle at +308 + 4i (0 for the others); for each
// nonzero byte two output buffers (BufferCreate, max(+0, 6) channels at
// 48,000 Hz) at +12 + 8i and +16 + 8i; MasteringCreate (+8) into +64; its
// driver's (+68) slot +28 with kMixerCallback. `frame` is this function's
// (208 bytes); `native_heap` whether the default allocators may run.
// Returns r3 in full as the original leaves it.
uint64_t EngineInit(uint8_t* base, uint32_t engine, uint32_t params, bool native_heap,
                    uint32_t frame, const GuestCall& call);

// XAudioEngine::Create (sub_8245C538): EngineSize (the frame's +80), the
// allocator (GetAllocator with kEngineAttributes and the size + 372, or the
// size word alone when that failed, `allocator` at the frame's +156, the
// caller's parameter word; its original where the plain C++ can't), 372
// bytes from it (slot +20), EngineConstruct, EngineInit; on success the
// engine to [out], else its slot +12; E_OUTOFMEMORY without memory or an
// engine; the allocator released (slot +4). `frame` is this function's (128
// bytes). Returns r3 in full as the original leaves it.
uint64_t EngineCreate(uint8_t* base, uint32_t params, uint32_t allocator, uint32_t out,
                      bool native_heap, uint32_t frame, const GuestCall& call);

// XAudio::CreateEngine (sub_82459D60): the XMA set-up (sub_8245C638,
// Dropped: nothing), then EngineCreate (`params`, `allocator`, kEngine).
// `frame` is this function's (112 bytes). Returns its r3.
uint64_t CreateEngine(uint8_t* base, uint32_t params, uint32_t allocator, bool native_heap,
                      uint32_t frame, const GuestCall& call);

// The harness's stand-ins for the engine's making, on its thread only
// (nullopt: none): SetUpProcessors' answer (the 8 bytes; r3 1), and
// MasteringCreate's (`mastering_voice` to [out] when `mastering_r3` isn't
// an error; r3 `mastering_r3`), so a case makes no thread and registers no
// render driver.
struct MakingStandIn {
  uint8_t processors[8] = {};
  uint32_t mastering_voice = 0;
  uint32_t mastering_r3 = 0;
};
void SetMakingForHarness(std::optional<MakingStandIn> stand_in);

// --- The API entry points over the engine (under the API lock, the game's
// critical section entered and left through the imports as the original
// does; docs/functions/xaudio.md) ---

// XAudioRegisterProcessingCallback (sub_8245A5E8): under the API lock,
// E_FAIL without an engine, else RegisterCallback's plain C++ with r3-r6 as
// the stage, function, context and out. Returns r3 in full (sign-extended).
uint64_t ApiRegisterCallback(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6,
                             uint32_t thread, const GuestCall& call);
// XAudio::Commit (sub_8245A6E8): under the API lock, E_FAIL without an
// engine (sign-extended), else Commit's plain C++, whose r3 it returns.
uint64_t ApiCommit(uint8_t* base, const Thread& thread, const GuestCall& call);
// XAudioInitialize (sub_8245A760: r4 = 0, a tail call to sub_8245A410):
// under the API lock, with an engine its slot +4 (AddRef) and 0; without,
// the creation function at the first nonzero word of the one-word table at
// 0x826002AC (XAudio::CreateEngine in play), called with r3 that word's
// address, r4 the table's end, r5 the parameters (a cleared 12-byte block at
// `frame` + 80 when `params` is 0) and r6 `allocator`; returns its r3 in
// full. `frame` is sub_8245A410's (128 bytes).
uint64_t ApiInitialize(uint8_t* base, uint32_t params, uint32_t allocator, uint32_t frame,
                       const GuestCall& call);

}  // namespace xaudio
