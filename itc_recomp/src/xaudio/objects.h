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

}  // namespace xaudio
