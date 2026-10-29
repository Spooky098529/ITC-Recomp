// XAudio's factory objects as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/xaudio-objects.md): the source voices'
// codecs (PCM and XMA) with their packet queue, the output matrix objects,
// the reverb and the volume meter, translated as the game's code (the
// user's call for XACT, 2026-10-23, followed for XAudio). The objects are
// guest objects shared with the recompiled code (the voices' vtable calls,
// the mixer's passes), so these work on guest memory, in the original's
// order of loads and stores. Each returns r3 as the original leaves it, in
// full (sign-extended error codes, a callee's r3 through LastCallR3). The
// XMA codec's functions that reach its XMA object stay with B2's rows
// (docs/functions/audio.md): they're called through `call`. Built with
// strict floating point and no contraction (CMakeLists.txt). Adapters:
// src/hooks/xaudio_objects.cpp (behind --native_xaudio_objects).

#pragma once

#include "xaudio/voices.h"

#include <cstdint>
#include <optional>

namespace xaudio {

// The vtables these objects are given (docs/functions/xaudio-objects.md).
constexpr uint32_t kObjectBaseVtable = 0x8204AA58;   // the inner objects' shared base
constexpr uint32_t kPcmCodecVtable = 0x8204BB48;
constexpr uint32_t kPcmCodecInnerVtable = 0x8204BB20;
constexpr uint32_t kPcmCodecBaseVtable = 0x8204BAC0;
constexpr uint32_t kXmaCodecVtable = 0x8204BA40;
constexpr uint32_t kXmaCodecInnerVtable = 0x8204BA18;
constexpr uint32_t kXmaCodecBaseVtable = 0x8204B9B8;
constexpr uint32_t kCodecInnerVtable = 0x8204ABD0;   // XAudioCodec's inner
constexpr uint32_t kCodecInnerBaseVtable = 0x8204AB70;  // at the inner's +12
constexpr uint32_t kCodecBaseVtable = 0x8204B958;    // the codec base's own
constexpr uint32_t kMatrixVtable = 0x8204AB30;
constexpr uint32_t kMatrixInnerVtable = 0x8204AB14;
constexpr uint32_t kMeterVtable = 0x8207229C;
constexpr uint32_t kCodecInfoWord = 0x8204B950;      // the codecs' info half-word (10)
constexpr uint32_t kSampleBytes = 0x8204BBC0;        // bytes a sample, by the format's type
constexpr uint32_t kXmaRelease = 0x8246A758;         // the XMA object's release (audio.md)
constexpr uint32_t kMemset = 0x825A71D0;

constexpr uint64_t kENotImpl = 0xFFFFFFFF80004001ull;     // as li/oris leave it
constexpr uint64_t kEOutOfMemory = 0xFFFFFFFF8007000Eull;

// --- Sizes (the class tables' size functions) ---

// XAudioCodec::Size (sub_82466B78): by the format's type (byte +8): up to 3
// PcmCodecSize, 4 XmaCodecSize, else E_NOTIMPL with nothing written.
uint64_t CodecSize(uint8_t* base, uint32_t descriptor, uint32_t out);
// PcmCodec::Size (sub_8246D720): 272 + 120 x the packet count (byte +68).
uint64_t PcmCodecSize(uint8_t* base, uint32_t descriptor, uint32_t out);
// XmaCodec::Size (sub_8246B458): 284 + 120 x the packets + 88 x the streams
// (byte +12).
uint64_t XmaCodecSize(uint8_t* base, uint32_t descriptor, uint32_t out);
// XAudioMatrix::Size (sub_82466840): 28 + 12 x the entries (byte +8, at most
// the default matrix's count byte at [+12] when there is one).
uint64_t MatrixSize(uint8_t* base, uint32_t descriptor, uint32_t out);
// Reverb::Size (sub_82463B78): 309,936.
uint64_t ReverbSize(uint8_t* base, uint32_t out);
// VolumeMeter::Size (sub_825A60E0): 12 x (+8 x +12, signed) + 52.
uint64_t MeterSize(uint8_t* base, uint32_t descriptor, uint32_t out);

// --- Small accessors ---

// XAudioCodec::GetInfo (sub_82466E80): byte 4 and the half-word at
// 0x8204B950. XAudioMatrix::GetInfo (sub_82465BE0): 6 and 0.
// VolumeMeter::GetInfo (sub_825A5778): 1 and 1. Each returns 0.
uint64_t CodecGetInfo(uint8_t* base, uint32_t out);
uint64_t MatrixGetInfo(uint8_t* base, uint32_t out);
uint64_t MeterGetInfo(uint8_t* base, uint32_t out);
// XAudioCodec::GetState (sub_8246A920): the state byte (+172) to [out]; 0.
uint64_t CodecGetState(uint8_t* base, uint32_t codec, uint32_t out);
// XAudioMatrix::SetOverwrite (sub_82465C08): +26 = r4's low byte; 0.
uint64_t MatrixSetOverwrite(uint8_t* base, uint32_t matrix, uint32_t overwrite);
// ReverbCore::SetStereo (sub_824679F8): +100 = r4 (r3 left as it is).
void ReverbSetStereo(uint8_t* base, uint32_t core, uint32_t stereo);

// XAudioObject::Release (sub_8246CE70): the inner's count (the object's +8)
// less one (as 64 bits from the zero-extended word); at 0 the inner's
// (+4) slot +12 and 0. `frame` is this function's (96 bytes).
uint64_t ObjectRelease(uint8_t* base, uint32_t object, uint32_t frame, const GuestCall& call);

// --- The codecs' construction ---

// PacketLists::Init (sub_8246E1E0): the two list heads at `lists` (12 bytes
// each: next, prev, the node's offset) empty, offsets 0.
void PacketListsInit(uint8_t* base, uint32_t lists);
// CodecBase::CodecBase (sub_82469920): vtable 0x8204B958, its lists (+4)
// made empty. Returns the codec base.
uint64_t CodecBaseConstruct(uint8_t* base, uint64_t codec_base);
// XAudioCodec::Construct (sub_82466BC8): the codec's inner at `inner`:
// vtable 0x8204AA58, +4 = 1, +8 the descriptor's +4; CodecBase::CodecBase
// on +12; vtables 0x8204ABD0 (+0) and 0x8204AB70 (+12). Returns `inner`.
uint64_t CodecConstruct(uint8_t* base, uint64_t inner, uint32_t descriptor);
// PacketLists::Alloc (sub_8246E120): `count` (its low byte) x 120 bytes from
// the allocator's slot +20 into +24, each entry put on the free list (+12).
// Returns the allocation (`lists` when the count is 0). `frame` is this
// function's (112 bytes).
uint64_t PacketListsAlloc(uint8_t* base, uint64_t lists, uint32_t count, uint32_t allocator,
                          uint32_t frame, const GuestCall& call);
// CodecBase::Init (sub_82469978): the 56-byte format to +36 (memcpy), +96
// and +100 = 1.0, the state +156 = 32, +92 and +94 = 1, +152 = 0, then
// PacketLists::Alloc on +4. Returns its r3. `frame` is this function's (112
// bytes).
uint64_t CodecBaseInit(uint8_t* base, uint32_t codec_base, uint32_t format, uint32_t count,
                       uint32_t allocator, uint32_t frame, const GuestCall& call);
// XAudioCodec::Init (sub_82466960): the callbacks (+172, +176) from the
// descriptor's +72 and +76, then CodecBase::Init on +12 (a tail call: `sp`
// is the caller's stack pointer).
uint64_t CodecInit(uint8_t* base, uint32_t inner, uint32_t descriptor, uint32_t allocator,
                   uint32_t sp, const GuestCall& call);
// PcmCodec::PcmCodec (sub_8246D750): see the row. `frame` is this
// function's (144 bytes). Returns the object.
uint64_t PcmCodecConstruct(uint8_t* base, uint64_t object, uint32_t descriptor,
                           uint32_t allocator, uint32_t frame, const GuestCall& call);
// XmaCodec::XmaCodec (sub_8246C1D0): see the row. `frame` is this
// function's (160 bytes). Returns the object.
uint64_t XmaCodecConstruct(uint8_t* base, uint64_t object, uint32_t descriptor,
                           uint32_t allocator, uint32_t frame, const GuestCall& call);
// PcmCodec::Create (sub_8246DF78) and XmaCodec::Create (sub_8246CDB0): the
// object from the allocator's slot +20, constructed (the XMA codec's slot
// +120 then sets up its XMA object, audio.md), to [out]; 0 or the error.
// `frame` is the function's (112 bytes).
uint64_t PcmCodecCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                        uint32_t frame, const GuestCall& call);
uint64_t XmaCodecCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                        uint32_t frame, const GuestCall& call);
// XAudioCodec::Create (sub_82466BA0): by the format's type, PcmCodecCreate
// or XmaCodecCreate (tail calls: their frames below `sp`), else E_NOTIMPL.
uint64_t CodecCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                     uint32_t sp, const GuestCall& call);

// --- The codecs' destruction ---

// PacketList::Next (sub_82466EA0): the neighbor of `entry` (0: the head) in
// the list at `list`, next when `direction` (as a signed word) isn't 0, as
// an entry (the node less the head's offset, +8), 0 at the head.
uint64_t PacketListNext(uint8_t* base, uint64_t list, uint64_t entry, uint32_t direction);
// PacketList::Clear (sub_8246E060): every node unlinked, each pointed at
// itself. Returns the last Next's r3 (0).
uint64_t PacketListClear(uint8_t* base, uint32_t list);
// PacketLists::Clear (sub_8246E0E8): the second list (+12), then the first.
uint64_t PacketListsClear(uint8_t* base, uint32_t lists);
// CodecBase::~CodecBase (sub_82469960): vtable 0x8204B958, PacketLists::Clear
// on +4.
uint64_t CodecBaseDestruct(uint8_t* base, uint32_t codec_base);
// XAudioCodec::Destruct (sub_82466EE8): vtables 0x8204ABD0 and 0x8204AB70
// (+12), CodecBase::~CodecBase on +12, the root vtable (0x8204A6B8).
uint64_t CodecDestruct(uint8_t* base, uint32_t inner);
// PcmCodec::~PcmCodec (sub_8246CF50): the PCM codec's three vtables, then
// XAudioCodec::Destruct on +4. Returns the object.
uint64_t PcmCodecDestroy(uint8_t* base, uint64_t object);
// XmaCodec::~XmaCodec (sub_8246AAD0): the XMA object's release (sub_8246A758,
// audio.md, through `call`). Returns the object. `frame` is its (96 bytes).
uint64_t XmaCodecDestroy(uint8_t* base, uint64_t object, uint32_t frame, const GuestCall& call);
// XAudioMatrix::Destruct (sub_82466418): vtables 0x8204AB30 (+0), then
// 0x8204AB14 and the root's at +4.
void MatrixDestruct(uint8_t* base, uint32_t matrix);

// --- The matrix and the meter's construction ---

// XAudioMatrix::Create (sub_82466888): 28 bytes from the allocator's slot
// +20, its vtables and count, +12 the descriptor's +4, its slot +52
// (XAudioMatrix::Init); the object to [out], or the inner's slot +12 and the
// error; E_OUTOFMEMORY. `frame` is its (128 bytes).
uint64_t MatrixCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                      uint32_t frame, const GuestCall& call);
// VolumeMeter::Create (sub_825A6460): 52 bytes from the allocator's slot
// +20 (no check), its slot +28 (VolumeMeter::Init); the object to [out]; 0.
// `frame` is its (128 bytes).
uint64_t MeterCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                     uint32_t frame, const GuestCall& call);
// VolumeMeter::Init (sub_825A6138): the descriptor's 16 bytes to +8, the
// allocator to +48, three arrays of 4 x (+8 x +12) bytes from its slot +20
// into +28, +32 and +36, each cleared (memset); 0. `frame` is its (128
// bytes).
uint64_t MeterInit(uint8_t* base, uint32_t meter, uint32_t descriptor, uint32_t allocator,
                   uint32_t frame, const GuestCall& call);

// --- The packet queue and the codecs' slots (under the voice lists' spin
// lock, taken as the original does: SpinTake and SpinRelease with the
// thread's r13) ---

// CodecBase::SetFlags (sub_82467078): under the spin lock, +156 = (+156 &
// ~mask) | bits (their low bytes). Returns r3 as the last kernel call left
// it (LastCallR3). `frame` is its (144 bytes).
uint64_t CodecSetFlags(uint8_t* base, uint32_t codec_base, uint32_t mask, uint32_t bits,
                       uint32_t thread, const GuestCall& call);
// CodecBase::RetirePacket (sub_8246A430): under the spin lock, `entry` (0:
// the first queued, when there is one; else 0 is returned) moved from the
// queued list (+4) to the end of the free list (+16), +104 cleared (48
// bytes), the entry's +116 = 0, slot +84 (32, 32). Returns the entry.
// `frame` is its (128 bytes).
uint64_t CodecRetirePacket(uint8_t* base, uint32_t codec_base, uint64_t entry, uint32_t thread,
                           uint32_t frame, const GuestCall& call);
// CodecBase::CompletePacket (sub_824674D8): RetirePacket, then the
// packet-done callback (+160) when there's one and an entry, with a pointer
// to {the codec's +12 (the base's -4), the entry's +36, `result`} in its
// frame. Returns the entry. `frame` is its (128 bytes).
uint64_t CodecCompletePacket(uint8_t* base, uint32_t codec_base, uint64_t entry, uint64_t result,
                             uint32_t thread, uint32_t frame, const GuestCall& call);
// CodecBase::FlushPackets (sub_824699E0): E_UNEXPECTED while playing; else
// slot +92 (0, E_ABORT) until it returns 0, slot +84 (24, 0), +152 = 0; 0.
// `frame` is its (96 bytes).
uint64_t CodecFlushPackets(uint8_t* base, uint32_t codec_base, uint32_t frame,
                           const GuestCall& call);
// CodecBase::SetFormat (sub_82469E58): E_UNEXPECTED while playing or with
// packets queued; else the format to +36 (memcpy), +157 = r5, slot +80
// with f1 = 1.0, whose r3 it returns. `frame` is its (112 bytes).
uint64_t CodecBaseSetFormat(uint8_t* base, uint32_t codec_base, uint32_t format, uint32_t extra,
                            uint32_t frame, const GuestCall& call);
// PcmCodec::SetFormat (sub_8246CFA8): CodecBase::SetFormat on +16; on
// success +110 = 1 and +108 = the type's bytes a sample x the channels.
// `frame` is its (96 bytes).
uint64_t PcmCodecSetFormat(uint8_t* base, uint32_t codec, uint32_t format, uint32_t extra,
                           uint32_t frame, const GuestCall& call);
// PcmCodec::Start (sub_8246D410), Stop (sub_8246D500, `flags` r4) and
// XAudioCodec::EndOfStream (sub_8246D630): the state changes under the
// spin lock, through slot +84; 0. `frame` is theirs (128 bytes).
uint64_t PcmCodecStart(uint8_t* base, uint32_t codec, uint32_t thread, uint32_t frame,
                       const GuestCall& call);
uint64_t PcmCodecStop(uint8_t* base, uint32_t codec, uint32_t flags, uint32_t thread,
                      uint32_t frame, const GuestCall& call);
uint64_t CodecEndOfStream(uint8_t* base, uint32_t codec, uint32_t thread, uint32_t frame,
                          const GuestCall& call);
// PcmCodec::SetVolume (sub_8246D008): +112 and +224 = the volume rounded
// to float, under the spin lock; 0.
uint64_t PcmCodecSetVolume(uint8_t* base, uint32_t codec, double volume, uint32_t thread,
                           const GuestCall& call);
// PcmCodec::SetPitch (sub_8246D0C8, the ratio 2^octaves through pow) and
// SetFrequencyRatio (sub_8246D1E0): +116 the ratio as float; the
// converter's +196 and +197 the format's type and channels, +200 the rate
// x the ratio (the pitch's rounded to float, the ratio's in double) by
// fctidz, +264 bit 0; 0. `frame` is theirs (144 bytes).
uint64_t PcmCodecSetPitch(uint8_t* base, uint32_t codec, double octaves, uint32_t thread,
                          uint32_t frame, const GuestCall& call);
uint64_t PcmCodecSetFrequencyRatio(uint8_t* base, uint32_t codec, double ratio, uint32_t thread,
                                   uint32_t frame, const GuestCall& call);
// XmaCodec::SetVolume (sub_8246AB48) and SetPitch (sub_8246AC40): as the
// PCM codec's over each stream (+56 of them, 88 bytes from [+188]). `frame`
// is SetPitch's (144 bytes).
uint64_t XmaCodecSetVolume(uint8_t* base, uint32_t codec, double volume, uint32_t thread,
                           const GuestCall& call);
uint64_t XmaCodecSetPitch(uint8_t* base, uint32_t codec, double octaves, uint32_t thread,
                          uint32_t frame, const GuestCall& call);
// CodecBase::OnSubmit (sub_82469AF8): the new entry's bounds checked
// (unless flags bit 4), each failed check releasing one level of the spin
// lock as the original does; slot +84 (24, 8 for flags bit 0 else 0); slot
// +84 (32, 0) when the free list is empty; 0. `frame` is its (128 bytes).
uint64_t CodecOnSubmit(uint8_t* base, uint32_t codec_base, uint32_t entry, uint32_t flags,
                       uint32_t thread, uint32_t frame, const GuestCall& call);
// CodecBase::SubmitPacket (sub_82469EF8) and SubmitRegion (sub_8246A1C0):
// under the spin lock, the first free entry to the end of the queued list,
// its fields from the packet (`packet`), OnSubmit; E_OUTOFMEMORY with no
// free entry (the lock released twice, as the original does). nullopt, with
// nothing done, when the block size (+92) is 0 (the original traps on its
// division). `frame` is theirs (144 bytes).
std::optional<uint64_t> CodecSubmitPacket(uint8_t* base, uint32_t codec_base, uint32_t packet,
                                          uint32_t flags, uint32_t thread, uint32_t frame,
                                          const GuestCall& call);
std::optional<uint64_t> CodecSubmitRegion(uint8_t* base, uint32_t codec_base, uint32_t region,
                                          uint32_t flags, uint32_t thread, uint32_t frame,
                                          const GuestCall& call);
// XmaCodec::SubmitRegion (sub_8246A860): E_NOTIMPL when +266 is 1; else
// +266 = 2, a loop count above 255 other than -1 made -1 on a copy of the
// 32 bytes in its frame, CodecBase::SubmitRegion on +16 with flags | 16.
// nullopt as SubmitRegion's (before anything is written). `frame` is its
// (128 bytes).
std::optional<uint64_t> XmaCodecSubmitRegion(uint8_t* base, uint32_t codec, uint32_t region,
                                             uint32_t flags, uint32_t thread, uint32_t frame,
                                             const GuestCall& call);

// --- The output matrix (under the voice lists' spin lock) ---

// XAudioMatrix::Init (sub_82465C30): +24 the entries (the larger of the
// descriptor's +8 and its default matrix's count byte), +20 their 12-byte
// entries from the allocator's slot +20 (when there are any), slot +40
// (SetMatrix) with the default matrix, then on success slot +32
// (SetOverwrite) with the descriptor's +9. Returns the last call's r3.
// `frame` is its (112 bytes).
uint64_t MatrixInit(uint8_t* base, uint32_t matrix, uint32_t descriptor, uint32_t allocator,
                    uint32_t frame, const GuestCall& call);
// XAudioMatrix::SetMatrix (sub_82465E50): under the spin lock, the entries
// from `levels` (0: none), +25 the highest output channel + 1; 0, or
// E_INVALIDARG past +24.
uint64_t MatrixSetMatrix(uint8_t* base, uint32_t matrix, uint32_t levels, uint32_t thread,
                         const GuestCall& call);
// XAudioMatrix::SetLevels (sub_82466188): under the spin lock, each given
// entry's target level, and its current one too when `now`'s low byte
// isn't 0; E_INVALIDARG at the first index past +16 (the entries before it
// set).
uint64_t MatrixSetLevels(uint8_t* base, uint32_t matrix, uint32_t levels, uint32_t now,
                         uint32_t thread, const GuestCall& call);
// XAudioMatrix::MixRamp (sub_82466310): out += in x the level, 16 frames a
// loop of 4-frame vectors (aligned down, as lvx and stvx), the level from
// `start` stepping by `step` (4 floats each) every 4 frames; vmaddfp as
// the translated code computes it (a product, then a sum), with the vector
// unit's flush to zero on. Returns r3 as the original leaves it (the last
// loop's input less 32 bytes, or `out` with no loop).
uint64_t MatrixMixRamp(uint8_t* base, uint64_t out, uint64_t in, uint32_t start, uint32_t step,
                       uint32_t frames);
// XAudioMatrix::Mix (sub_82466440): the source's channels into the
// target's by the entries (each ramped from its current level to its
// target over the pass, then current = target), or with none each channel
// as it is; overwriting (+26) clears the target first and stores instead
// of adding. Under the spin lock; its locals in its frame as the original
// (`frame`, 288 bytes). Returns 0, or a format's or the clear's error.
uint64_t MatrixMix(uint8_t* base, uint32_t matrix, uint32_t source, uint32_t target,
                   uint32_t thread, uint32_t frame, const GuestCall& call);

// --- The volume meter ---

// VolumeMeter::Process (sub_825A6210): for each of its channels (+16), the
// pass's RMS, peak and the threshold sum (a sample past the float at
// 0x82072298 adds it) to the history slot ((+24 mod +20) x +16 + the
// channel) of +36, +32 and +28; with 8 meter channels over a 6-channel
// buffer, the two fold-downs to slots 6 and 7 of the arrays' first words;
// +24 up by one, +44 = 1 when it reaches +16; 0. `frame` is its (112
// bytes).
uint64_t MeterProcess(uint8_t* base, uint32_t meter, uint32_t buffer, uint32_t frame);

}  // namespace xaudio
