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
#include <functional>
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

// XAudioBuffer::Size (sub_8245FA28): the size of a buffer to [out] for
// `format` (+0 a type byte, then an 8-byte voice format at +4: +5 the
// channels, +8 the rate), (256 / (48,000 / the rate)) x the channels x 4 +
// 128 + 36 for type 0, else 36; returns 0. nullopt, with nothing written,
// when the rate is 0 or above 48,000 (the original traps on its
// divisions).
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

// --- Calls into the recompiled objects ---
//
// The voices' methods reach the objects the engine's factory makes (the
// codecs, matrix objects and effects), the allocators and the voices' own
// vtables, which stay recompiled. A call sets only the registers given
// (r3 first, `count` of them, and f1 when asked); the others keep what the
// previous call left, as in the original. `return_address` is the
// original's. Returns the callee's r3.
struct Regs {
  uint64_t r[5] = {};  // r3 to r7
  uint8_t count = 0;
  bool f1_set = false;
  double f1 = 0.0;
  // The stack pointer for the call (0: the adapter's frame). Set by plain
  // C++ called from plain C++, which works in a frame of its own below its
  // caller's, as the original does, so a callee's frame never covers the
  // locals it hands out.
  uint32_t sp = 0;
};
using GuestCall = std::function<uint32_t(uint32_t function, uint32_t return_address, const Regs&)>;

// The lowest frame plain C++ has made below its adapter's on this thread (0:
// none), lowered by each frame a plain C++ function makes for a plain C++
// callee. A call without its own stack pointer (Regs::sp 0) runs below it,
// so a callee's frame never covers a nested function's locals, whichever
// plain C++ made the call. Each adapter's frame saves and clears it, and
// puts it back.
uint32_t& StackFloor();

// A frame of `bytes` below `frame` with its back chain, the stack floor
// lowered to it: for plain C++ outside XAudio (the car sounds) calling an
// entry point's plain C++ from its own frame, as the original's call makes
// the callee's frame there.
uint32_t FrameBelow(uint8_t* base, uint32_t frame, uint32_t bytes);

// A call to function 0 only sets the registers given (and the stack
// pointer is ignored): a plain C++ caller sets them as the original does
// before calling a function whose plain C++ it runs itself, so the
// registers that function's calls don't set are the original's.

// The method at `slot` of the vtable of the object at `object`.
uint32_t Method(uint8_t* base, uint32_t object, uint32_t slot);

// The interface thunks: the object 8 bytes before the interface at `iface`
// (0 stays 0), whose method they tail-call with the other registers as they
// are. Each returns the object; its method to `function`.
// XAudioFactory::Size (sub_8245D610): slot +20.
uint32_t FactorySizeThunk(uint8_t* base, uint32_t iface, uint32_t* function);
// XAudioFactory::Create (sub_8245D630): slot +24.
uint32_t FactoryCreateThunk(uint8_t* base, uint32_t iface, uint32_t* function);
// XAudioBuffer::AddRefThunk (sub_8245F6B0): slot +4.
uint32_t BufferAddRefThunk(uint8_t* base, uint32_t iface, uint32_t* function);
// XAudioBuffer::ReleaseThunk (sub_8245F6D0): slot +8.
uint32_t BufferReleaseThunk(uint8_t* base, uint32_t iface, uint32_t* function);

// XAudioVoice::Release (sub_8245F878): +4 down by one; at 0 the object's
// slot +12 (free) and 0, else the count.
uint32_t Release(uint8_t* base, uint32_t object, const GuestCall& call);

// XAudioObject::Free (sub_8245AAD8): the allocator (+8) AddRef'd (its slot
// +0), the object's slot +0 with 1 (its deleting destructor), the allocator
// released (slot +4). Returns the last call's r3.
uint32_t ObjectFree(uint8_t* base, uint32_t object, const GuestCall& call);

// XAudioSourceVoice::ApplyVolume (sub_8245DFC0): the category's volume
// (the engine's +132 + 4 x +144 for categories 0 and 1, else 1.0 from
// 0x82000A30) times +140, to the codec's (+76) slot +88 in f1 (a tail
// call). Returns its r3.
uint32_t ApplyVolume(uint8_t* base, uint32_t voice, const GuestCall& call);

// XAudioSourceVoice::StopAndReset (sub_8245DD60): slot +60 with 1; when it
// succeeds and there's a codec (+76), the codec's slot +32. Returns the
// last call's r3.
uint32_t StopAndReset(uint8_t* base, uint32_t voice, const GuestCall& call);

// XAudioVoice::SetEffectParameters (sub_82461418): E_INVALIDARG past the
// +60 effects; else the effect's (+36 + 8 x the index) slot +16 with
// `r5`-`r7` as r4-r6. Returns its r3.
uint32_t SetEffectParameters(uint8_t* base, uint32_t voice, uint32_t index, uint64_t r5,
                             uint64_t r6, uint64_t r7, const GuestCall& call);

// XAudioVoice::ReleaseOutputs (sub_82460A58): each output's target (+72 +
// 12i, +69 of them, the count read again each time) released (slot +8);
// +69 = 0. Returns the last call's r3, or `voice` when there were none (the
// original's r3 then).
uint32_t ReleaseOutputs(uint8_t* base, uint32_t voice, const GuestCall& call);

// XAudioVoice::SetOutputMatrices (sub_82460AB8): for each of the matrices'
// entries (count byte +0, +4 the 8-byte entries) until a call fails, the
// output's matrix object (+72 + 12 x the index byte, +4) gets its slot +56
// with the matrix and whether the voice isn't playing. Returns the last
// call's r3 (0 with no entries).
uint32_t SetOutputMatrices(uint8_t* base, uint32_t voice, uint32_t matrices,
                           const GuestCall& call);

// XAudioVoice::SetFormat (sub_82460778): SetFormatWords, then slot +36
// (set output voices) with `outputs`. Returns its r3.
uint32_t SetFormat(uint8_t* base, uint32_t voice, uint32_t format, uint32_t outputs,
                   const GuestCall& call);

// XAudio::Alloc (sub_8245EF80): XMemAlloc (sub_822976F8)'s plain C++ with
// `size` and `attributes`. Only while hooks::NativeHeapActive().
uint32_t Alloc(uint8_t* base, uint32_t size, uint32_t attributes);

// XAudioBuffer::ClearThunk (sub_8245FA10): BufferClear on the object 8 bytes
// before the interface at `iface` (0 stays 0); nullopt where it refuses.
std::optional<uint32_t> BufferClearThunk(uint8_t* base, uint32_t iface, uint32_t format,
                                         uint32_t flags);

// XAudioSourceVoice::Describe (sub_8245E010): the 32-byte voice
// description at `out` (cleared) for the source description at
// `description`: +0 the output format (SourceOutputFormat), +8 the effect
// chain (+68), +12 the engine's first buffer of the six (+12 + 8i) that's
// set, +16 the callback (+76), +20 its context (+88), +24 the output count
// (+57, 1 when 0), +25 the outputs' channels (+58, 6 when 0), +28 the
// outputs (+72).
void SourceDescribe(uint8_t* base, uint32_t description, uint32_t out);

// XAudioSourceVoice::Reformat (sub_8245DCF0): the 56-byte format at
// `format` copied to +84, the codec's (+76) slot +112 with the format and
// `r6`; when that succeeds, its output format (SourceOutputFormat, at the
// frame's +80) set with `outputs` (SetFormat). Returns the last call's r3.
// nullopt, with nothing done, where memcpy's plain C++ refuses the copy.
std::optional<uint32_t> Reformat(uint8_t* base, uint32_t voice, uint32_t format,
                                 uint32_t outputs, uint64_t r6, uint32_t frame,
                                 const GuestCall& call);

// XAudioVoice::BaseSize (sub_82461158): the size of a voice's base part for
// the voice description at `description` to [out]: an output buffer's
// (XAudioBuffer::Size for a format of its +1 channels at 48 kHz, at the
// frame's +96) when none is given (+12), plus each of the effect chain's
// (+8: count byte +0, +4 the ids) objects' sizes from the engine's factory
// (its slot +20 through XAudioFactory::Size's thunk, the size at the
// frame's +80) and 8 bytes each. Returns the last call's r3 (on a failure,
// with nothing written).
uint32_t BaseSize(uint8_t* base, uint32_t description, uint32_t out, uint32_t frame,
                  const GuestCall& call);

// --- Buffers, allocators and the base voice ---

constexpr uint32_t kBufferVtable = 0x8204A8CC;
constexpr uint32_t kAllocatorVtable = 0x8204A954;   // +0 of the default allocator
constexpr uint32_t kAllocatorVtable2 = 0x8204A980;  // its +4
constexpr uint32_t kVoiceVtable0 = 0x8204A6CC;      // the base voice's first
constexpr uint32_t kVoiceVtable = 0x8204AA00;       // the base voice's
constexpr uint32_t kBufferConstruct = 0x8245F750;
constexpr uint32_t kAllocatorUnseen = 0x82460538;  // GetAllocator's for a size of 0

// XAudioBuffer::XAudioBuffer (sub_8245F750): the 36-byte buffer at
// `buffer` for `format` (+0 the type; type 0: +5 the channels, +8 the rate,
// the samples from the allocator's slot +20, 128-byte aligned; else +12 the
// samples), the allocator `allocator`. Returns the buffer. nullopt, with
// nothing done, where the original traps (type 0 with a rate of 0 or above
// 48,000).
std::optional<uint32_t> BufferConstruct(uint8_t* base, uint32_t buffer, uint32_t format,
                                        uint32_t allocator, const GuestCall& call);

// XAudioBuffer::Create (sub_8245FA80): 36 bytes from the allocator's slot
// +20, the buffer constructed (BufferConstruct; its original through `call`
// where that refuses), its slot +20 with `out`. Returns 0, or E_OUTOFMEMORY
// when the allocation (or the constructor) gives 0.
uint32_t BufferCreate(uint8_t* base, uint32_t format, uint32_t allocator, uint32_t out,
                      const GuestCall& call);

// XAudioVoice::Construct (sub_82461500): the base voice at `voice`: vtable
// 0x8204A6CC, the count 1, the allocator (+8, AddRef'd by its slot +0 when
// set), then vtable 0x8204AA00, +12 = `type`'s low byte, the two empty list
// nodes at +16 and +24, +40 = 0. Returns the voice.
uint32_t VoiceConstruct(uint8_t* base, uint32_t voice, uint32_t allocator, uint32_t type,
                        const GuestCall& call);

// XAudio::DefaultAllocator (sub_824605E8): one block of `size` + 28 from
// XAudio::Alloc's plain C++ with `attributes`, its header written (the
// vtables, the count 1, the attributes, the size, 0 used, the next free
// byte +28) and the block to [out]; 0, or E_OUTOFMEMORY. Only while
// hooks::NativeHeapActive().
uint32_t DefaultAllocator(uint8_t* base, uint32_t attributes, uint32_t size, uint32_t out);

// XAudio::GetAllocator (sub_82460688): with no allocator in [inout], the
// default one (DefaultAllocator for a size, `kAllocatorUnseen` through
// `call` for none, its out at the frame's +80) to [inout] when it succeeds;
// with one, its slots +0, +8 (with `size`) and +12 (with `attributes`).
// Returns the last r3. `native_heap`: DefaultAllocator may run (else
// nullopt, with nothing done, where it would).
std::optional<uint32_t> GetAllocator(uint8_t* base, uint32_t attributes, uint32_t size,
                                     uint32_t inout, uint32_t frame, bool native_heap,
                                     const GuestCall& call);

// --- The voices with outputs (lock-free) ---

constexpr uint32_t kOutputsVtable = 0x8204A998;  // the voice with outputs'
constexpr uint32_t kDefaultOutput = 0x8204A9F4;  // {0, 0}: the mastering voice, its default matrix

// XAudioVoice::ConstructOutputs (sub_82460738): VoiceConstruct, then the
// vtable 0x8204A998. Returns the voice.
uint32_t ConstructOutputs(uint8_t* base, uint32_t voice, uint32_t allocator, uint32_t type,
                          const GuestCall& call);

// XAudioVoice::OutputsSize (sub_824607C8): for the voice description at
// `description`, BaseSize (at the frame's +84, its own frame below), plus
// for each of the +24 outputs 12 bytes and a matrix object's size (the
// factory's slot +20 for a matrix description at the frame's +96: type 1,
// +8 the outputs' channels from +25; the size at the frame's +80), to
// [out]. `frame` is this function's frame (144 bytes). Returns the last
// call's r3 (on a failure, with nothing written).
uint32_t OutputsSize(uint8_t* base, uint32_t description, uint32_t out, uint32_t frame,
                     const GuestCall& call);

// XAudioVoice::SetOutput (sub_82460998): the output entry `entry` (0: the
// default 0x8204A9F4; +0 the target, 0 the mastering voice the engine's +64;
// +4 its matrix) into the voice's output slot `slot` (+0 the target,
// AddRef'd by its slot +4; +4 the matrix object): the matrix object's slot
// +40 with the entry's matrix, or with none, the default one for the
// voice's channels (+53) to the target's (its slot +64 at the frame's +80;
// DefaultMatrix at +88). `frame` is this function's (128 bytes). Returns
// the last call's r3.
uint32_t SetOutput(uint8_t* base, uint32_t voice, uint32_t entry, uint32_t slot, uint32_t frame,
                   const GuestCall& call);

// XAudioVoice::SetOutputVoices (sub_82460BF0, and sub_8245F228 jumps to
// it): ReleaseOutputs, then each of the outputs' entries (count byte +0,
// +4 the 8-byte entries) set (SetOutput, in a frame below `frame`) until
// one fails, and +69 the count; with no outputs (0), the default one when
// the voice has room (+68) and +69 = 1. `frame` is this function's (144
// bytes). Returns the last result.
uint32_t SetOutputVoices(uint8_t* base, uint32_t voice, uint32_t outputs, uint32_t frame,
                         const GuestCall& call);

// XAudioVoice::Pass (sub_82461070): one mixer pass: the voice's slot +72
// (its input buffer, kept at the frame's +84), the callback (+44) with its
// context (+48, at the frame's +80), then when stopping (+61 bit 2) +64
// counted down, slot +60 with 1 (stop at once) at 0; else slot +84 (the
// pass) with the frame's +84. DebugSwitchOff's plain C++ (sub_825C8388)
// where the original calls it. `frame` is this function's (112 bytes).
// Returns the last method's r3.
uint32_t Pass(uint8_t* base, uint32_t voice, uint32_t frame, const GuestCall& call);

// XAudioVoice::Destroy (sub_82461240): the base's destructor: vtable
// 0x8204AA00, each effect (+36 + 8i, +60 of them) released (its slot +4)
// and cleared, the output buffer (+32) released (its interface's thunk) and
// cleared, vtable 0x8204A6CC, the allocator (+8) released (its slot +4) and
// cleared, the root vtable. Returns the allocator's release's r3, or 0.
uint32_t Destroy(uint8_t* base, uint32_t voice, const GuestCall& call);

// XAudioVoice::CreateObject (sub_82461308): the engine's factory (+60)
// makes the object described at `description` with the voice's allocator
// (+8) into [entry] (cleared first); its slot +8 at the frame's +80 (+0 a
// byte, +2 a halfword size): +4 = `flag`'s low byte, +5 = 1, +6 = the
// frame's +80, the voice's +62 raised to the size. `frame` is this
// function's (128 bytes). Returns the last call's r3.
uint32_t CreateObject(uint8_t* base, uint32_t voice, uint32_t entry, uint32_t description,
                      uint32_t flag, uint32_t frame, const GuestCall& call);

// XAudioVoice::ProcessEffect (sub_82461678): an effect entry `entry` (+0
// the object, +5 enabled, +6 flags) over the buffer at [inout]: disabled,
// 0; else the effect's slot +24 with the buffers its flags (+6) pick: for
// both bits 1 and 2 the processor's other engine buffer (`processor`, the
// engine's +12 + 8 x the processor) to [inout] and r4 the old; for bit 0 or
// 1 the buffer and 0; for bit 2, 0 and the buffer; for none, the word at
// the frame's +80 for both (a leftover, as the original). `frame` is this
// function's (96 bytes). Returns its r3.
uint32_t ProcessEffect(uint8_t* base, uint32_t entry, uint32_t inout, uint32_t processor,
                       uint32_t frame, const GuestCall& call);

// --- Making a voice and its passes (lock-free) ---

constexpr uint32_t kMemcpy = 0x825A7410;

// XAudioSourceVoice::Size (sub_8245E0D8): the size of a source voice for the
// source description at `description` to [out]: the voice's description
// (SourceDescribe at the frame's +96), its outputs' (OutputsSize, at +80, in
// its own frame below) and the codec's (the factory's slot +20 for the
// codec's description at +128: +4 the callback context (+88), +8 the
// 56-byte format, +64 the float +60, +68 the bytes +59 and +64, +72 the
// words +80 and +84; the size at +84). `frame` is this function's (240
// bytes). Returns 0, also on a failure (the original's).
uint32_t SourceSize(uint8_t* base, uint32_t description, uint32_t out, uint32_t frame,
                    const GuestCall& call);

// XAudioVoice::CreateEffects (sub_82461600): each of the chain's (count
// byte +0, +4 the descriptions) effects made into +36 + 8i (CreateObject,
// the flag its index, in a frame below `frame`, 112 bytes) until one fails.
// Returns the last r3 (0 with none).
uint32_t CreateEffects(uint8_t* base, uint32_t voice, uint32_t chain, uint32_t frame,
                       const GuestCall& call);

// XAudioVoice::InitBase (sub_82461D50): from the voice description at
// `description`: +44 the callback, +48 its context, +52 the format, +60 the
// effect count; the output buffer given (+12, AddRef'd through its
// interface's thunk) or made (XAudioBuffer::Create for a format of the
// channels at 48 kHz at the frame's +80) into +32; the effects' entries
// from the allocator's slot +20 into +36 and made (CreateEffects).
// `frame` is this function's (144 bytes). Returns the last r3, 0, or
// E_OUTOFMEMORY.
uint32_t InitBase(uint8_t* base, uint32_t voice, uint32_t description, uint32_t frame,
                  const GuestCall& call);

// XAudioVoice::InitOutputs (sub_82460880): +68 the output count (+24),
// InitBase, the outputs' 12-byte slots from the allocator's slot +20 into
// +72, a matrix object made into each (CreateObject for a description at
// the frame's +80: type 1, +4 the voice, +8 the channels +25; the flag
// 255), then the voice's slot +36 (set output voices) with the outputs
// (+28). `frame` is this function's (144 bytes). Returns the last r3, or
// E_OUTOFMEMORY.
uint32_t InitOutputs(uint8_t* base, uint32_t voice, uint32_t description, uint32_t frame,
                     const GuestCall& call);

// XAudioSourceVoice::Init (sub_8245E348): the 56-byte format to +84, +144
// the category (+56), the voice's description (SourceDescribe at the
// frame's +80), InitOutputs, then the codec made into +76 (CreateObject for
// the codec's description at +112, as SourceSize's; the flag 255).
// memcpy's plain C++, or its original where that refuses. `frame` is this
// function's (224 bytes). Returns the last r3.
uint32_t SourceInit(uint8_t* base, uint32_t voice, uint32_t description, uint32_t frame,
                    const GuestCall& call);

// XAudioSubmixVoice::Init (sub_8245F230): the voice's description
// (SubmixDescribe at the frame's +80), InitOutputs, and when that succeeds
// its buffer cleared (ClearThunk with flags 3, its original where it
// refuses). `frame` is this function's (144 bytes). Returns InitOutputs'
// r3.
uint32_t SubmixInit(uint8_t* base, uint32_t voice, uint32_t description, uint32_t frame,
                    const GuestCall& call);

// XAudioVoice::ProcessEffects (sub_82461E88): ProcessEffect for each effect
// (+36 + 8i, +60 of them) over [inout] until one fails. `frame` is this
// function's (112 bytes). Returns the last r3 (0 with none).
uint32_t ProcessEffects(uint8_t* base, uint32_t voice, uint32_t inout, uint32_t processor,
                        uint32_t frame, const GuestCall& call);

// XAudioVoice::MixToOutputs (sub_82460CB8): ProcessEffects, then for each
// output (+69) until one fails, the target's slot +72 (its input buffer)
// and the matrix object's slot +24 with [inout] and that buffer. `frame` is
// this function's (128 bytes). Returns the last r3.
uint32_t MixToOutputs(uint8_t* base, uint32_t voice, uint32_t inout, uint32_t processor,
                      uint32_t frame, const GuestCall& call);

// XAudioSubmixVoice::Pass (sub_8245EFA0): Pass; when it succeeds, its
// buffer (+32, the interface) cleared for its format (+52) with flags 3
// (its original where the plain C++ refuses) and the counter 1 up
// (AddStat). `frame` is this function's (112 bytes). Returns Pass' r3.
uint32_t SubmixPass(uint8_t* base, uint32_t voice, uint32_t frame, const GuestCall& call);

// --- The game's locks, taken as the original does ---
//
// The voice lists' spin lock (0x82954F10: +0 the kernel's spin lock word,
// +4 the depth, +8 the owning thread, r13; +12 the saved IRQL), taken
// inline by its users, native and recompiled, so native code takes it with
// the same reads and writes and the same kernel calls the recompiled code
// makes (the user's call, 2026-10-29). XAudio's API lock, the kernel
// critical section at 0x8264D518 (its recursion count at +20), likewise
// until every user of it is native. The kernel calls go through `call` at
// the imports' guest addresses (the thunks the original's `bl` reaches),
// which the adapters' callers run as those imports.

constexpr uint32_t kSpinLock = 0x82954F10;
constexpr uint32_t kApiLockObject = 0x8264D514;
constexpr uint32_t kApiLock = 0x8264D518;
constexpr uint32_t kKeRaiseIrqlToDpcLevel = 0x825DBE44;
constexpr uint32_t kKeAcquireSpinLockAtRaisedIrql = 0x825DB9A4;
constexpr uint32_t kKeReleaseSpinLockFromRaisedIrql = 0x825DB994;
constexpr uint32_t kKfLowerIrql = 0x825DBE54;
constexpr uint32_t kRtlEnterCriticalSection = 0x825DB8E4;
constexpr uint32_t kRtlLeaveCriticalSection = 0x825DB8D4;

// The spin lock taken by `thread` (r13): the IRQL raised; unless it already
// owns it, the kernel's spin lock acquired and the owner and saved IRQL
// set; the depth up by one.
void SpinTake(uint8_t* base, uint32_t thread, const GuestCall& call);
// One level released when `thread` owns it: the depth down by one; at 0,
// the owner and saved IRQL cleared, the kernel's spin lock released and the
// IRQL lowered to the saved one.
void SpinRelease(uint8_t* base, uint32_t thread, const GuestCall& call);

// --- Under the voice lists' spin lock ---

// XAudioSourceVoice::GetState (sub_8245DDC8): under the spin lock, the
// codec's (+76) slot +44 (its state at the frame's +80); when it succeeds,
// that or'ed with the voice's flags (+61) to [out]. `frame` is this
// function's (144 bytes). Returns the codec's r3.
uint32_t SourceGetState(uint8_t* base, uint32_t voice, uint32_t out, uint32_t thread,
                        uint32_t frame, const GuestCall& call);

// XAudioVoice::Play (sub_82461798): under the spin lock, a playing voice's
// stopping bit (+61 bit 2) cleared (the lock taken again for it); else bits
// 0 and 6 cleared and bit 0 set (taken again) and slot +76 (Activate).
// DebugSwitchOff's plain C++ where the original calls sub_825C8388. Returns
// 0.
uint32_t Play(uint8_t* base, uint32_t voice, uint32_t thread, const GuestCall& call);

// XAudioVoice::Activate (sub_82460EF0): each output's target (+72 + 12i,
// +69 of them) has its use count (+40) raised and its slot +56 called until
// one fails; then by the voice's type (slot +52, at the frame's +80): 0 the
// engine's source active list (its +80 + 40), 1 its stage's ([+124] + 44 x
// +76, + 40), the voice added at its end under the spin lock (its node at
// the list's +8 offset). `frame` is this function's (144 bytes). Returns 0,
// or the failing target's r3.
uint32_t Activate(uint8_t* base, uint32_t voice, uint32_t thread, uint32_t frame,
                  const GuestCall& call);

// XAudioVoice::Deactivate (sub_82460D50): under the spin lock, the voice
// out of its active list (the node at +24, left pointing at itself); then
// each output's target's use count (+40) lowered, its slot +60 with 0 (stop
// at once) at 0, until one fails. Returns the last r3 (0 when not called).
uint32_t Deactivate(uint8_t* base, uint32_t voice, uint32_t thread, const GuestCall& call);

// XAudioSourceVoice::StartLater (sub_8245E7D0): under the spin lock:
// E_UNEXPECTED when playing (+61 bit 0); else unless pending (bit 6), bit 6
// set and the voice added at the end of the engine's pending list (+68, its
// node at the list's +8 offset), each under the lock taken again. Returns 0
// or E_UNEXPECTED.
uint32_t StartLater(uint8_t* base, uint32_t voice, uint32_t thread, const GuestCall& call);

// XAudio::WaitForVoice (sub_8245AE60): unless the caller is the mixer's
// thread (the engine's +300, or its +332 + 4 x the processor), while the
// voice is one of the six the mixer is working on (+144), the spin lock
// released one level, the API lock left as many times as its recursion
// count (+20) and entered again as many, the spin lock taken again.
// `thread` is r13, `mixer_thread` its +256, `processor` its +268.
void WaitForVoice(uint8_t* base, uint32_t engine, uint32_t voice, uint32_t thread,
                  uint32_t mixer_thread, uint32_t processor, const GuestCall& call);

// XAudioSourceVoice::CancelPendingStart (sub_8245E428): under the spin
// lock: 0x80004004 when no start is pending (+61 bit 6 clear); else the
// mixer waited for (WaitForVoice), the voice out of the pending list (its
// node at the engine's +76 offset, left pointing at itself) and bit 6
// cleared, each under the lock taken again. Returns 0 or 0x80004004.
uint32_t CancelPendingStart(uint8_t* base, uint32_t voice, uint32_t thread,
                            uint32_t mixer_thread, uint32_t processor, const GuestCall& call);

// XAudioVoice::StopOrTail (sub_824619B0): with `flags` bit 0 (at once),
// under the spin lock, the mixer waited for; a playing voice's bits 0, 2 and
// 6 cleared (the lock taken again) and slot +80 (Deactivate). Without, a
// playing voice's stopping bit (+61 bit 2) set (taken again) and the
// tail's passes (+64) from +62. DebugSwitchOff's plain C++ where the
// original calls sub_825C8388. Returns 0.
uint32_t StopOrTail(uint8_t* base, uint32_t voice, uint32_t flags, uint32_t thread,
                    uint32_t mixer_thread, uint32_t processor, const GuestCall& call);

// --- Starting, stopping, passes, making and destroying voices ---

constexpr uint32_t kSourceVtable = 0x8204A720;
constexpr uint32_t kSubmixVtable = 0x8204A870;
constexpr uint32_t kGetAllocator = 0x82460688;
constexpr uint32_t kVoiceAttributes = 0x61820006;

// The thread a voice method runs on: r13 (the spin lock's owner), its +256
// (WaitForVoice's mixer check) and +268 (the processor).
struct Thread {
  uint32_t r13 = 0;
  uint32_t mixer = 0;
  uint32_t processor = 0;
};

// XAudioSourceVoice::Start (sub_8245EBA0): with `flags` bit 0, StartLater;
// else the pending start cancelled (CancelPendingStart; any failure but
// 0x80004004 returned), slot +92 (the volume), the codec's (+76) slot +48
// (start) and when it succeeds Play. Returns the last r3.
uint32_t SourceStart(uint8_t* base, uint32_t voice, uint32_t flags, const Thread& thread,
                     const GuestCall& call);

// XAudioSourceVoice::Stop (sub_8245E9F0): when no start was pending
// (CancelPendingStart's 0x80004004), the codec's (+76) slot +52 with
// `flags`; unless it fails, StopOrTail with `flags`, bit 0 added when
// `flags` hasn't it and the codec's state (slot +44, at the frame's +80)
// hasn't bit 0 (at once when the codec's done). Else CancelPendingStart's
// r3. `frame` is this function's (128 bytes).
uint32_t SourceStop(uint8_t* base, uint32_t voice, uint64_t flags, const Thread& thread,
                    uint32_t frame, const GuestCall& call);

// XAudioSourceVoice::Pass (sub_8245DED0): the volume applied again (slot
// +92) when the voice's category (+144) is in the engine's changed mask
// (+140); the codec as an effect (ProcessEffect on +76) over [inout],
// MixToOutputs, the codec's state (slot +44, at the frame's +80), the
// counter 2 up (AddStat); when the codec isn't playing (bit 0) and the
// voice isn't stopping (+61 bit 2), StopOrTail with 0. `frame` is this
// function's (128 bytes). Returns the first failure, StopOrTail's r3 or
// the state's.
uint32_t SourcePass(uint8_t* base, uint32_t voice, uint32_t inout, const Thread& thread,
                    uint32_t frame, const GuestCall& call);

// XAudioSourceVoice::XAudioSourceVoice (sub_8245E6A8): ConstructOutputs (the
// type `type`), the source vtable, +140 = 1.0 (0x82000A30), the voice added
// at the end of the engine's source list (+80, its node at the list's +8
// offset) under the spin lock. Returns the voice.
uint32_t SourceConstruct(uint8_t* base, uint32_t voice, uint32_t allocator, uint32_t type,
                         const Thread& thread, const GuestCall& call);

// XAudioSubmixVoice::XAudioSubmixVoice (sub_8245F298): ConstructOutputs (type
// 1), the submix vtable, +76 the stage (the description's +10), the voice
// added at the end of its stage's list (the engine's [+124] + 44 x the
// stage) under the spin lock. Returns the voice.
uint32_t SubmixConstruct(uint8_t* base, uint32_t voice, uint32_t description,
                         uint32_t allocator, const Thread& thread, const GuestCall& call);

// XAudioVoice::~XAudioVoiceOutputs (sub_82460B50): its vtable, StopOrTail
// at once, ReleaseOutputs, each output's matrix object (+72 + 12i + 4, +68
// of them) released (slot +4) and cleared, Destroy. Returns Destroy's r3.
uint32_t OutputsDestroy(uint8_t* base, uint32_t voice, const Thread& thread,
                        const GuestCall& call);

// XAudioSourceVoice::~XAudioSourceVoice (sub_8245E1B8): its vtable, slot +60
// (stop at once); when that succeeds, the codec's (+76) slot +32 (reset);
// the voice out of the engine's source list (its node at the engine's +88
// offset) under the spin lock; the codec released (slot +4) and cleared;
// OutputsDestroy. Returns its r3.
uint32_t SourceDestroy(uint8_t* base, uint32_t voice, const Thread& thread,
                       const GuestCall& call);

// XAudioSourceVoice::DeletingDestructor (sub_8245E7A0): SourceDestroy;
// returns the voice (the memory is the allocator's).
uint32_t SourceDeletingDestructor(uint8_t* base, uint32_t voice, const Thread& thread,
                                  const GuestCall& call);

// XAudioSubmixVoice::~XAudioSubmixVoice (sub_8245F108): its vtable,
// StopOrTail at once, the voice out of its stage's list under the spin
// lock, OutputsDestroy. Returns its r3.
uint32_t SubmixDestroy(uint8_t* base, uint32_t voice, const Thread& thread,
                       const GuestCall& call);

// XAudioSubmixVoice::DeletingDestructor (sub_8245F3A0): SubmixDestroy;
// returns the voice.
uint32_t SubmixDeletingDestructor(uint8_t* base, uint32_t voice, const Thread& thread,
                                  const GuestCall& call);

// XAudioVoice::CreateSource (sub_8245EAA0): the size (SourceSize, + 148),
// the allocator (GetAllocator with `allocator` at the frame's +156, the
// caller's parameter word, as the original keeps it; its original where
// the plain C++ can't), 148 bytes from it (slot +20), the voice
// constructed and initialized (SourceInit; on a failure freed through its
// slot +12), the voice to [out]; the allocator released (slot +4). `frame`
// is this function's (128 bytes); `native_heap` whether GetAllocator's
// default allocator may run. Returns 0 or the failure.
uint32_t CreateSource(uint8_t* base, uint32_t description, uint32_t allocator, uint32_t out,
                      bool native_heap, const Thread& thread, uint32_t frame,
                      const GuestCall& call);

// XAudioVoice::CreateSubmix (sub_8245F3D0): as CreateSource for a submix
// voice: SubmixDescribe (the frame's +96), OutputsSize (+80) + 80 bytes,
// the allocator at the frame's +204, SubmixConstruct and SubmixInit.
// `frame` is this function's (176 bytes).
uint32_t CreateSubmix(uint8_t* base, uint32_t description, uint32_t allocator, uint32_t out,
                      bool native_heap, const Thread& thread, uint32_t frame,
                      const GuestCall& call);

// --- The API entry points (under the API lock) ---
//
// Each enters XAudio's API lock, makes one call and leaves it. The lock is
// the game's critical section (its imports through `call`) until every
// user of it is native.

// The API lock entered and left as the entry points do.
void ApiEnter(uint8_t* base, const GuestCall& call);
void ApiLeave(uint8_t* base, const GuestCall& call);

// The entry points that call one of the voice's methods (the slot of its
// vtable) or its codec's (+76) with r4-r7 as given (as many as the
// original passes; r3 the object), under the API lock; each returns the
// method's r3. XAudioVoice::AddRef (sub_82459DB0, +4), Release
// (sub_82459E18, +8), SetEffectParameters (sub_82459E80, +32, r4-r7),
// SetOutputVoices (sub_82459EF0, +36, r4), SetOutputMatrix (sub_82459F48,
// +40, r4), GetState (sub_82459FA0, +48, r4), Stop (sub_8245A1B0, +60, r4),
// GetOutputFormat (sub_8245A3B8, +64, r4); the codec's: SubmitPacket
// (sub_8245A050, +28, r4-r5), sub_8245A108 (+36, r4-r5), FlushPackets
// (sub_8245A208, +56), sub_8245A270 (+72, r4).
uint32_t ApiAddRef(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call);
uint32_t ApiRelease(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call);
uint32_t ApiSetEffectParameters(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call);
uint32_t ApiSetOutputVoices(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call);
uint32_t ApiSetOutputMatrix(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call);
uint32_t ApiGetState(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call);
uint32_t ApiStop(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call);
uint32_t ApiGetOutputFormat(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call);
uint32_t ApiSubmitPacket(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call);
uint32_t ApiCodec36(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call);
uint32_t ApiFlushPackets(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call);
uint32_t ApiCodec72(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call);

// XAudioSourceVoice::SetFormat (sub_82459FF8): Reformat (its own frame
// below `frame`, 128 bytes; its original where the plain C++ refuses).
uint32_t ApiSetFormat(uint8_t* base, uint32_t voice, uint32_t format, uint32_t outputs,
                      uint64_t r6, uint32_t frame, const GuestCall& call);
// XAudioSourceVoice::Reset (sub_8245A0B0): StopAndReset.
uint32_t ApiReset(uint8_t* base, uint32_t voice, const GuestCall& call);
// XAudioVoice::Start (sub_8245A168): XAudioSourceVoice::Start.
uint32_t ApiStart(uint8_t* base, uint32_t voice, uint32_t flags, const Thread& thread,
                  const GuestCall& call);
// XAudioSourceVoice::SetVolume (sub_8245A2C8): +140 = `volume` (as a
// float), then slot +92 (the volume applied; f1 still `volume`).
uint32_t ApiSetVolume(uint8_t* base, uint32_t voice, double volume, const GuestCall& call);
// XAudioSourceVoice::SetFrequencyRatio (sub_8245A340): the codec's (+76)
// slot +96 with f1 `ratio`.
uint32_t ApiSetFrequencyRatio(uint8_t* base, uint32_t voice, double ratio, const GuestCall& call);
// XAudioGetMasteringVoice (sub_8245A568): the engine's +64 to [out] and 0;
// E_FAIL (0x80004005) without an engine or one.
uint32_t ApiGetMasteringVoice(uint8_t* base, uint32_t out, const GuestCall& call);
// XAudioCreateSourceVoice (sub_8245A768) and XAudioCreateSubmixVoice
// (sub_8245A7C8): CreateSource or CreateSubmix with no allocator (in a
// frame below `frame`, 128 bytes), the voice (at the frame's +80) to [out]
// when it succeeds.
uint32_t ApiCreateSourceVoice(uint8_t* base, uint32_t description, uint32_t out,
                              bool native_heap, const Thread& thread, uint32_t frame,
                              const GuestCall& call);
uint32_t ApiCreateSubmixVoice(uint8_t* base, uint32_t description, uint32_t out,
                              bool native_heap, const Thread& thread, uint32_t frame,
                              const GuestCall& call);

}  // namespace xaudio
