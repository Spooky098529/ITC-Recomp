// XAudio's factory objects as plain C++ (objects.h;
// docs/functions/xaudio-objects.md). Built with strict floating point and no
// contraction (CMakeLists.txt), so each rounding is the one written; the
// adapters turn the host's flush to zero off before float work, as the
// translated code does.

#include "xaudio/objects.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "xaudio/engine.h"
#include "xaudio/resampler.h"

#include <rex/platform/fpscr.h>
#include <rex/ppc/intrinsics.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <climits>
#include <cmath>
#include <cstring>

namespace xaudio {

namespace {

using Fpscr = rex::platform::FPSCRPlatform;

Regs Args(std::initializer_list<uint64_t> r) {
  Regs regs;
  for (const uint64_t value : r) {
    regs.r[regs.count++] = value;
  }
  return regs;
}

// The call made at `sp` (a plain C++ function's own frame).
Regs At(uint32_t sp, Regs regs) {
  regs.sp = sp;
  return regs;
}

// The registers the original sets before calling a function whose plain
// C++ runs here instead.
void SetRegs(const GuestCall& call, Regs regs) { call(0, 0, regs); }

uint32_t Below(uint8_t* base, uint32_t frame, uint32_t bytes) {
  return FrameBelow(base, frame, bytes);
}

// The allocator's slot +20 (allocate `bytes`): its r3 in full.
uint64_t Allocate(uint8_t* base, uint32_t allocator, uint64_t bytes, uint32_t return_address,
                  uint32_t sp, const GuestCall& call) {
  call(Method(base, allocator, 20), return_address, At(sp, Args({allocator, bytes})));
  return LastCallR3();
}

// memset (sub_825A71D0) with 0: the same bytes.
void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(base, address, bytes, [](uint8_t* host, uint32_t, uint32_t part) {
    std::memset(host, 0, part);
  });
}

// memcpy's plain C++, or its original (at `sp`) where that refuses.
void Copy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size,
          uint32_t return_address, uint32_t sp, const GuestCall& call) {
  SetRegs(call, Args({destination, source, size}));
  if (!c_runtime::Memcpy(base, destination, source, size)) {
    call(kMemcpy, return_address, At(sp, Args({destination, source, size})));
  }
}

// A float word copied through a float register (lfs, then stfs): its bits,
// as the translated code leaves them (its conversions there fold away, a
// signaling NaN kept, seen in SetMatrix's harness cases).
void CopyFloat(uint8_t* base, uint32_t destination, uint32_t source) {
  guest::StoreU32(base, destination, guest::LoadU32(base, source));
}

}  // namespace

// --- Sizes ---

uint64_t PcmCodecSize(uint8_t* base, uint32_t descriptor, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8246D720 PcmCodec::Size");
  guest::StoreU32(base, out, guest::LoadU8(base, descriptor + 68) * 120u + 272u);
  return 0;
}

uint64_t XmaCodecSize(uint8_t* base, uint32_t descriptor, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8246B458 XmaCodec::Size");
  const uint32_t packets = guest::LoadU8(base, descriptor + 68);
  const uint32_t streams = guest::LoadU8(base, descriptor + 12);
  guest::StoreU32(base, out, packets * 120u + streams * 88u + 284u);
  return 0;
}

uint64_t CodecSize(uint8_t* base, uint32_t descriptor, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82466B78 XAudioCodec::Size");
  const uint8_t type = guest::LoadU8(base, descriptor + 8);
  if (type <= 3) {
    return PcmCodecSize(base, descriptor, out);
  }
  if (type == 4) {
    return XmaCodecSize(base, descriptor, out);
  }
  return kENotImpl;
}

uint64_t MatrixSize(uint8_t* base, uint32_t descriptor, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82466840 XAudioMatrix::Size");
  const uint32_t matrix = guest::LoadU32(base, descriptor + 12);
  uint32_t entries = guest::LoadU8(base, descriptor + 8);
  if (matrix != 0) {
    // The larger of the two (the original keeps its own count only when
    // it's above the matrix's).
    const uint32_t count = guest::LoadU8(base, matrix);
    if (entries <= count) {
      entries = count;
    }
  }
  guest::StoreU32(base, out, entries * 12u + 28u);
  return 0;
}

uint64_t ReverbSize(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82463B78 Reverb::Size");
  guest::StoreU32(base, out, 0x4BAB0);
  return 0;
}

uint64_t MeterSize(uint8_t* base, uint32_t descriptor, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_825A60E0 VolumeMeter::Size");
  const int32_t a = int32_t(guest::LoadU32(base, descriptor + 12));
  const int32_t b = int32_t(guest::LoadU32(base, descriptor + 8));
  const uint32_t product = uint32_t(int64_t(a) * int64_t(b));
  guest::StoreU32(base, out, product * 12u + 52u);
  return 0;
}

// --- Small accessors ---

uint64_t CodecGetInfo(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82466E80 XAudioCodec::GetInfo");
  guest::StoreU8(base, out, 4);
  guest::StoreU16(base, out + 2, guest::LoadU16(base, kCodecInfoWord));
  return 0;
}

uint64_t MatrixGetInfo(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82465BE0 XAudioMatrix::GetInfo");
  guest::StoreU8(base, out, 6);
  guest::StoreU16(base, out + 2, 0);
  return 0;
}

uint64_t MeterGetInfo(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_825A5778 VolumeMeter::GetInfo");
  guest::StoreU8(base, out, 1);
  guest::StoreU16(base, out + 2, 1);
  return 0;
}

uint64_t CodecGetState(uint8_t* base, uint32_t codec, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8246A920 XAudioCodec::GetState");
  guest::StoreU8(base, out, guest::LoadU8(base, codec + 172));
  return 0;
}

uint64_t MatrixSetOverwrite(uint8_t* base, uint32_t matrix, uint32_t overwrite) {
  ITC_NATIVE_RUNNING("sub_82465C08 XAudioMatrix::SetOverwrite");
  guest::StoreU8(base, matrix + 26, uint8_t(overwrite));
  return 0;
}

void ReverbSetStereo(uint8_t* base, uint32_t core, uint32_t stereo) {
  ITC_NATIVE_RUNNING("sub_824679F8 ReverbCore::SetStereo");
  guest::StoreU32(base, core + 100, stereo);
}

uint64_t ObjectRelease(uint8_t* base, uint32_t object, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246CE70 XAudioObject::Release");
  const uint32_t inner = object + 4;
  const uint64_t count = uint64_t(guest::LoadU32(base, inner + 4)) - 1;
  guest::StoreU32(base, inner + 4, uint32_t(count));
  if (uint32_t(count) != 0) {
    return count;
  }
  call(Method(base, inner, 12), 0x8246CEA4, At(frame, Args({inner})));
  return 0;
}

// --- The codecs' construction ---

void PacketListsInit(uint8_t* base, uint32_t lists) {
  ITC_NATIVE_RUNNING("sub_8246E1E0 PacketLists::Init");
  guest::StoreU32(base, lists, lists);
  guest::StoreU32(base, lists + 4, lists);
  guest::StoreU32(base, lists + 8, 0);
  guest::StoreU32(base, lists + 12 + 8, 0);
  guest::StoreU32(base, lists + 12, lists + 12);
  guest::StoreU32(base, lists + 12 + 4, lists + 12);
}

uint64_t CodecBaseConstruct(uint8_t* base, uint64_t codec_base) {
  ITC_NATIVE_RUNNING("sub_82469920 CodecBase::CodecBase");
  guest::StoreU32(base, uint32_t(codec_base), kCodecBaseVtable);
  PacketListsInit(base, uint32_t(codec_base) + 4);
  return codec_base;
}

uint64_t CodecConstruct(uint8_t* base, uint64_t inner, uint32_t descriptor) {
  ITC_NATIVE_RUNNING("sub_82466BC8 XAudioCodec::Construct");
  const uint32_t at = uint32_t(inner);
  guest::StoreU32(base, at, kObjectBaseVtable);
  guest::StoreU32(base, at + 4, 1);
  guest::StoreU32(base, at + 8, guest::LoadU32(base, descriptor + 4));
  CodecBaseConstruct(base, inner + 12);
  guest::StoreU32(base, at, kCodecInnerVtable);
  guest::StoreU32(base, at + 12, kCodecInnerBaseVtable);
  return inner;
}

uint64_t PacketListsAlloc(uint8_t* base, uint64_t lists, uint32_t count, uint32_t allocator,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246E120 PacketLists::Alloc");
  const uint32_t n = count & 0xFF;
  if (n == 0) {
    return lists;
  }
  const uint32_t at = uint32_t(lists);
  const uint64_t entries = Allocate(base, allocator, uint64_t(n) * 120, 0x8246E15C, frame, call);
  guest::StoreU32(base, at + 24, uint32_t(entries));
  const uint32_t free_list = at + 12;
  for (uint32_t i = 0, offset = 0; i < n; i++, offset += 120) {
    uint32_t node = guest::LoadU32(base, at + 24) + offset;
    guest::StoreU32(base, node, node);
    node = guest::LoadU32(base, at + 24) + offset;
    guest::StoreU32(base, node + 4, guest::LoadU32(base, node));
    // The node at the list's offset (+8) into the entry, at the free list's
    // end.
    node = offset + guest::LoadU32(base, free_list + 8) + guest::LoadU32(base, at + 24);
    guest::StoreU32(base, node, free_list);
    guest::StoreU32(base, node + 4, guest::LoadU32(base, free_list + 4));
    guest::StoreU32(base, free_list + 4, node);
    guest::StoreU32(base, guest::LoadU32(base, node + 4), node);
  }
  return entries;
}

uint64_t CodecBaseInit(uint8_t* base, uint32_t codec_base, uint32_t format, uint32_t count,
                       uint32_t allocator, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82469978 CodecBase::Init");
  Copy(base, codec_base + 36, format, 56, 0x8246999C, frame, call);
  CopyFloat(base, codec_base + 96, 0x82000A30);
  CopyFloat(base, codec_base + 100, 0x82000A30);
  guest::StoreU8(base, codec_base + 156, 32);
  guest::StoreU16(base, codec_base + 92, 1);
  guest::StoreU16(base, codec_base + 94, 1);
  guest::StoreU32(base, codec_base + 152, 0);
  SetRegs(call, Args({codec_base + 4, count, allocator}));
  return PacketListsAlloc(base, codec_base + 4, count, allocator, Below(base, frame, 112), call);
}

uint64_t CodecInit(uint8_t* base, uint32_t inner, uint32_t descriptor, uint32_t allocator,
                   uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82466960 XAudioCodec::Init");
  guest::StoreU32(base, inner + 172, guest::LoadU32(base, descriptor + 72));
  guest::StoreU32(base, inner + 176, guest::LoadU32(base, descriptor + 76));
  const uint32_t extra = guest::LoadU8(base, descriptor + 69);
  const uint32_t count = guest::LoadU8(base, descriptor + 68);
  SetRegs(call, Args({inner + 12, descriptor + 8, count, extra, allocator}));
  return CodecBaseInit(base, inner + 12, descriptor + 8, count, allocator, Below(base, sp, 112),
                       call);
}

uint64_t PcmCodecConstruct(uint8_t* base, uint64_t object, uint32_t descriptor,
                           uint32_t allocator, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D750 PcmCodec::PcmCodec");
  const uint32_t at = uint32_t(object);
  CodecConstruct(base, object + 4, descriptor);
  const uint32_t converter = at + 184;
  guest::StoreU32(base, at, kPcmCodecVtable);
  guest::StoreU32(base, at + 4, kPcmCodecInnerVtable);
  guest::StoreU32(base, at + 16, kPcmCodecBaseVtable);
  Clear(base, converter, 76);
  guest::StoreU32(base, converter + 76, 0);
  SetRegs(call, Args({at + 4, descriptor, allocator}));
  CodecInit(base, at + 4, descriptor, allocator, frame, call);
  const uint32_t rate = guest::LoadU32(base, at + 60);
  const double ratio = double(guest::LoadFloat(base, at + 116));
  const uint8_t extra = guest::LoadU8(base, descriptor + 69);
  const uint8_t channels = guest::LoadU8(base, at + 56);
  const uint8_t type = guest::LoadU8(base, at + 52);
  // The rate's word goes through the frame (std, lfd, fcfid).
  guest::StoreU32(base, frame + 80, 0);
  guest::StoreU32(base, frame + 84, rate);
  guest::StoreU32(base, at + 268, extra);
  uint32_t changed = guest::LoadU32(base, converter + 80);
  guest::StoreU8(base, converter + 12, type);
  changed |= 1;
  guest::StoreU8(base, converter + 13, channels);
  guest::StoreU32(base, converter + 80, changed);
  changed |= 2;
  const double rate_float = double(float(double(int64_t(uint64_t(rate)))));
  const double product = double(float(rate_float * ratio));
  const int64_t fixed = std::isnan(product) ? INT64_MIN
                        : product > double(LLONG_MAX)
                            ? LLONG_MAX
                            : simde_mm_cvttsd_si64(simde_mm_load_sd(&product));
  guest::StoreU32(base, converter + 16, uint32_t(fixed));
  guest::StoreU32(base, converter + 80, changed);
  guest::StoreU32(base, converter + 32, kSampleRate);
  const uint32_t table_type = guest::LoadU8(base, converter + 12);
  const uint32_t table_channels = guest::LoadU8(base, converter + 13);
  const uint32_t bytes = guest::LoadU32(base, kSampleBytes + table_type * 4);
  guest::StoreU16(base, at + 110, 1);
  guest::StoreU16(base, at + 108,
                  uint16_t(int64_t(int32_t(bytes)) * int64_t(int32_t(table_channels))));
  return object;
}

namespace {

// The XMA codec's block size (+192): max(1, (vctsxs(2^x x 16) + 15,
// saturating unsigned) >> 4) for x the float at `address` as lvlx loads it
// (its first lane), with the vector unit's flush to zero on and vexptefp as
// the translated code computes it.
uint32_t XmaBlockSize(uint8_t* base, uint32_t address) {
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  simde__m128i v0 = simde_mm_set1_epi32(int(0xF));
  const simde__m128i v11 = simde_mm_set1_epi32(int(0x1));
  const simde__m128i line = simde_mm_load_si128(
      reinterpret_cast<const simde__m128i*>(REX_RAW_ADDR(address & ~0xFu)));
  const simde__m128i mask = simde_mm_load_si128(
      reinterpret_cast<const simde__m128i*>(&rex::ppc::VectorMaskL[(address & 0xF) * 16]));
  simde__m128 x = simde_mm_castsi128_ps(simde_mm_shuffle_epi8(line, mask));
  asm volatile("" : "+x"(x));
  simde__m128 n = simde_mm_round_ps(x, SIMDE_MM_FROUND_TO_NEG_INF | SIMDE_MM_FROUND_NO_EXC);
  simde__m128 f = simde_mm_sub_ps(x, n);
  simde__m128 p = simde_mm_set1_ps(1.8775767e-3f);
  p = simde_mm_add_ps(simde_mm_mul_ps(p, f), simde_mm_set1_ps(8.9893397e-3f));
  p = simde_mm_add_ps(simde_mm_mul_ps(p, f), simde_mm_set1_ps(5.5826318e-2f));
  p = simde_mm_add_ps(simde_mm_mul_ps(p, f), simde_mm_set1_ps(2.4015361e-1f));
  p = simde_mm_add_ps(simde_mm_mul_ps(p, f), simde_mm_set1_ps(6.9315308e-1f));
  p = simde_mm_add_ps(simde_mm_mul_ps(p, f), simde_mm_set1_ps(1.0f));
  const simde__m128i exp_bits = simde_mm_slli_epi32(
      simde_mm_add_epi32(simde_mm_cvttps_epi32(n), simde_mm_set1_epi32(127)), 23);
  simde__m128 e = simde_mm_mul_ps(p, simde_mm_castsi128_ps(exp_bits));
  simde__m128 scaled = simde_mm_mul_ps(e, simde_mm_set1_ps(16));
  asm volatile("" : "+x"(scaled));
  const simde__m128i fixed = rex::ppc::simde_mm_vctsxs(scaled);
  Fpscr::setcsr(csr);
  v0 = rex::ppc::simde_mm_adds_epu32(fixed, v0);
  v0 = simde_mm_srai_epi32(v0, 4);
  v0 = simde_mm_max_epi32(v0, v11);
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), v0);
  // The first lane (the host's last: the translated code's lanes run
  // backward), splatted and stored by stvewx.
  return lanes[3];
}

}  // namespace

uint64_t XmaCodecConstruct(uint8_t* base, uint64_t object, uint32_t descriptor,
                           uint32_t allocator, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246C1D0 XmaCodec::XmaCodec");
  const uint32_t at = uint32_t(object);
  CodecConstruct(base, object + 4, descriptor);
  guest::StoreU32(base, at + 184, 0);
  guest::StoreU32(base, at, kXmaCodecVtable);
  guest::StoreU32(base, at + 4, kXmaCodecInnerVtable);
  guest::StoreU32(base, at + 16, kXmaCodecBaseVtable);
  const uint8_t extra = guest::LoadU8(base, descriptor + 69);
  guest::StoreU8(base, at + 266, 0);
  guest::StoreU8(base, at + 252, extra);
  guest::StoreU32(base, at + 272, 0);
  guest::StoreU32(base, at + 276, 0);
  guest::StoreU8(base, at + 280, 0);
  guest::StoreU32(base, at + 268, 0);
  SetRegs(call, Args({at + 4, descriptor, allocator}));
  CodecInit(base, at + 4, descriptor, allocator, frame, call);
  const uint32_t streams = guest::LoadU8(base, at + 56);
  const uint64_t bytes = uint64_t(streams) * 88;  // never above 0x2E8BA2E streams: a byte
  const uint64_t array = Allocate(base, allocator, bytes, 0x8246C27C, frame, call);
  uint32_t first = 0;
  if (uint32_t(array) != 0) {
    for (uint32_t i = 0, entry = uint32_t(array); i < streams; i++, entry += 88) {
      SetRegs(call, Args({entry, 0, 76}));
      Clear(base, entry, 76);
      guest::StoreU32(base, entry + 76, 0);
    }
    first = uint32_t(array);
  }
  guest::StoreU32(base, at + 188, first);
  guest::StoreU32(base, at + 192, XmaBlockSize(base, descriptor + 64));
  Copy(base, at + 196, at + 52, 56, 0x8246C308, frame, call);
  guest::StoreU16(base, at + 108, 1);
  guest::StoreU16(base, at + 110, 1);
  return object;
}

uint64_t PcmCodecCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                        uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246DF78 PcmCodec::Create");
  const uint64_t object = Allocate(base, allocator, 272, 0x8246DFA8, frame, call);
  if (uint32_t(object) == 0) {
    return kEOutOfMemory;
  }
  SetRegs(call, Args({object, descriptor, allocator}));
  const uint64_t made =
      PcmCodecConstruct(base, object, descriptor, allocator, Below(base, frame, 144), call);
  if (uint32_t(made) == 0) {
    return kEOutOfMemory;
  }
  guest::StoreU32(base, out, uint32_t(made));
  return 0;
}

uint64_t XmaCodecCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                        uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246CDB0 XmaCodec::Create");
  const uint64_t object = Allocate(base, allocator, 284, 0x8246CDE0, frame, call);
  if (uint32_t(object) == 0) {
    return kEOutOfMemory;
  }
  SetRegs(call, Args({object, descriptor, allocator}));
  const uint64_t made =
      XmaCodecConstruct(base, object, descriptor, allocator, Below(base, frame, 160), call);
  const uint32_t codec = uint32_t(made);
  if (codec == 0) {
    return kEOutOfMemory;
  }
  // The XMA object's set-up (slot +120, sub_8246AEE0 in audio.md).
  call(Method(base, codec, 120), 0x8246CE24, At(frame, Args({made})));
  const uint64_t r3 = LastCallR3();
  if (int32_t(r3) >= 0) {
    guest::StoreU32(base, out, codec);
    return r3;
  }
  call(Method(base, codec + 4, 12), 0x8246CE50, At(frame, Args({made + 4})));
  return r3;
}

uint64_t CodecCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                     uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82466BA0 XAudioCodec::Create");
  const uint8_t type = guest::LoadU8(base, descriptor + 8);
  if (type <= 3) {
    return PcmCodecCreate(base, descriptor, allocator, out, Below(base, sp, 112), call);
  }
  if (type == 4) {
    return XmaCodecCreate(base, descriptor, allocator, out, Below(base, sp, 112), call);
  }
  return kENotImpl;
}

// --- The codecs' destruction ---

uint64_t PacketListNext(uint8_t* base, uint64_t list, uint64_t entry, uint32_t direction) {
  ITC_NATIVE_RUNNING("sub_82466EA0 PacketList::Next");
  const uint32_t head = uint32_t(list);
  const uint32_t node = uint32_t(entry) != 0
                            ? uint32_t(guest::LoadU32(base, head + 8) + entry)
                            : head;
  const uint32_t next =
      int32_t(direction) != 0 ? guest::LoadU32(base, node) : guest::LoadU32(base, node + 4);
  if (head == next) {
    return 0;
  }
  return uint64_t(next) - uint64_t(guest::LoadU32(base, head + 8));
}

uint64_t PacketListClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8246E060 PacketList::Clear");
  uint64_t entry = PacketListNext(base, list, 0, 1);
  while (uint32_t(entry) != 0) {
    const uint32_t node = uint32_t(guest::LoadU32(base, list + 8) + entry);
    const uint32_t next = guest::LoadU32(base, node);
    if (next != node) {
      guest::StoreU32(base, next + 4, guest::LoadU32(base, node + 4));
      guest::StoreU32(base, guest::LoadU32(base, node + 4), guest::LoadU32(base, node));
      guest::StoreU32(base, node + 4, node);
      guest::StoreU32(base, node, node);
    }
    entry = PacketListNext(base, list, 0, 1);
  }
  return entry;
}

uint64_t PacketListsClear(uint8_t* base, uint32_t lists) {
  ITC_NATIVE_RUNNING("sub_8246E0E8 PacketLists::Clear");
  PacketListClear(base, lists + 12);
  return PacketListClear(base, lists);
}

uint64_t CodecBaseDestruct(uint8_t* base, uint32_t codec_base) {
  ITC_NATIVE_RUNNING("sub_82469960 CodecBase::~CodecBase");
  guest::StoreU32(base, codec_base, kCodecBaseVtable);
  return PacketListsClear(base, codec_base + 4);
}

uint64_t CodecDestruct(uint8_t* base, uint32_t inner) {
  ITC_NATIVE_RUNNING("sub_82466EE8 XAudioCodec::Destruct");
  guest::StoreU32(base, inner, kCodecInnerVtable);
  guest::StoreU32(base, inner + 12, kCodecInnerBaseVtable);
  const uint64_t r3 = CodecBaseDestruct(base, inner + 12);
  guest::StoreU32(base, inner, kRootVtable);
  return r3;
}

uint64_t PcmCodecDestroy(uint8_t* base, uint64_t object) {
  ITC_NATIVE_RUNNING("sub_8246CF50 PcmCodec::~PcmCodec");
  const uint32_t at = uint32_t(object);
  guest::StoreU32(base, at, kPcmCodecVtable);
  guest::StoreU32(base, at + 4, kPcmCodecInnerVtable);
  guest::StoreU32(base, at + 16, kPcmCodecBaseVtable);
  CodecDestruct(base, at + 4);
  return object;
}

uint64_t XmaCodecDestroy(uint8_t* base, uint64_t object, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246AAD0 XmaCodec::~XmaCodec");
  call(kXmaRelease, 0x8246AAE8, At(frame, Args({object})));
  return object;
}

void MatrixDestruct(uint8_t* base, uint32_t matrix) {
  ITC_NATIVE_RUNNING("sub_82466418 XAudioMatrix::Destruct");
  guest::StoreU32(base, matrix, kMatrixVtable);
  guest::StoreU32(base, matrix + 4, kMatrixInnerVtable);
  guest::StoreU32(base, matrix + 4, kRootVtable);
}

// --- The matrix and the meter's construction ---

uint64_t MatrixCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                      uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82466888 XAudioMatrix::Create");
  const uint64_t object = Allocate(base, allocator, 28, 0x824668B8, frame, call);
  const uint32_t at = uint32_t(object);
  if (at == 0) {
    return kEOutOfMemory;
  }
  const uint32_t inner = at + 4;
  guest::StoreU32(base, inner, kObjectBaseVtable);
  guest::StoreU32(base, inner + 4, 1);
  guest::StoreU32(base, inner + 8, guest::LoadU32(base, descriptor + 4));
  guest::StoreU32(base, at, kMatrixVtable);
  guest::StoreU32(base, inner, kMatrixInnerVtable);
  call(guest::LoadU32(base, kMatrixVtable + 52), 0x82466914,
       At(frame, Args({object, descriptor, allocator})));
  const uint64_t r3 = LastCallR3();
  if (int32_t(r3) >= 0) {
    guest::StoreU32(base, out, at);
    return r3;
  }
  call(Method(base, inner, 12), 0x82466950, At(frame, Args({uint64_t(inner)})));
  return r3;
}

uint64_t MeterCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                     uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825A6460 VolumeMeter::Create");
  const uint64_t object = Allocate(base, allocator, 52, 0x825A6490, frame, call);
  uint64_t meter = 0;
  if (uint32_t(object) != 0) {
    const uint32_t at = uint32_t(object);
    meter = object;
    guest::StoreU32(base, at + 4, 1);
    guest::StoreU32(base, at, kMeterVtable);
    for (const uint32_t offset : {48u, 24u, 28u, 44u, 8u, 12u, 16u, 20u}) {
      guest::StoreU32(base, at + offset, 0);
    }
  }
  const uint32_t at = uint32_t(meter);
  call(Method(base, at, 28), 0x825A64F8, At(frame, Args({meter, descriptor, allocator})));
  guest::StoreU32(base, out, at);
  return 0;
}

uint64_t MeterInit(uint8_t* base, uint32_t meter, uint32_t descriptor, uint32_t allocator,
                   uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825A6138 VolumeMeter::Init");
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, meter + 8 + i, guest::LoadU32(base, descriptor + i));
  }
  guest::StoreU32(base, meter + 48, allocator);
  const int32_t a = int32_t(guest::LoadU32(base, descriptor + 8));
  const int32_t b = int32_t(guest::LoadU32(base, descriptor + 12));
  const uint32_t bytes = uint32_t(int64_t(b) * int64_t(a)) << 2;
  static constexpr uint32_t kArrays[] = {28, 32, 36};
  static constexpr uint32_t kReturns[] = {0x825A61A0, 0x825A61C8, 0x825A61F0};
  for (int i = 0; i < 3; i++) {
    const uint64_t array = Allocate(base, allocator, bytes, kReturns[i], frame, call);
    guest::StoreU32(base, meter + kArrays[i], uint32_t(array));
    SetRegs(call, Args({array, 0, bytes}));
    Clear(base, uint32_t(array), bytes);
  }
  return 0;
}

// --- The packet queue and the codecs' slots ---

namespace {

// The first entry of the list at `list` (its node less the head's offset),
// 0 when it's empty or the first entry is 0.
uint64_t FirstEntry(uint8_t* base, uint32_t list) {
  const uint32_t next = guest::LoadU32(base, list);
  if (next == list) {
    return 0;
  }
  return uint64_t(next) - uint64_t(guest::LoadU32(base, list + 8));
}

// The node of `entry` (the list's offset into it) out of its list when it's
// in one, left pointing at itself.
void UnlinkEntry(uint8_t* base, uint32_t list, uint64_t entry) {
  const uint32_t node = uint32_t(guest::LoadU32(base, list + 8) + entry);
  const uint32_t next = guest::LoadU32(base, node);
  if (next == node) {
    return;
  }
  guest::StoreU32(base, next + 4, guest::LoadU32(base, node + 4));
  guest::StoreU32(base, guest::LoadU32(base, node + 4), guest::LoadU32(base, node));
  guest::StoreU32(base, node + 4, node);
  guest::StoreU32(base, node, node);
}

// The node of `entry` added at the end of the list at `list`.
void LinkEntryAtEnd(uint8_t* base, uint32_t list, uint64_t entry) {
  const uint32_t node = uint32_t(guest::LoadU32(base, list + 8) + entry);
  guest::StoreU32(base, node, list);
  guest::StoreU32(base, node + 4, guest::LoadU32(base, list + 4));
  guest::StoreU32(base, list + 4, node);
  guest::StoreU32(base, guest::LoadU32(base, node + 4), node);
}

uint64_t SetFlagsCall(uint8_t* base, uint32_t codec_base, uint32_t mask, uint32_t bits,
                      uint32_t return_address, uint32_t sp, const GuestCall& call) {
  call(Method(base, codec_base, 84), return_address, At(sp, Args({codec_base, mask, bits})));
  return LastCallR3();
}

// fctidz of a double, as the translated code computes it.
int64_t ToFixed(double value) {
  return std::isnan(value)             ? INT64_MIN
         : value > double(LLONG_MAX) ? LLONG_MAX
                                     : simde_mm_cvttsd_si64(simde_mm_load_sd(&value));
}

// The converter at `converter` given a rate times a ratio (both as the
// translated code has them: the rate's word through the frame at `frame`
// + 80, converted and rounded to float, the product rounded to float),
// stored by fctidz to +16.
void SetConverterRate(uint8_t* base, uint32_t converter, uint32_t frame, uint32_t rate,
                      double ratio) {
  guest::StoreU32(base, frame + 80, 0);
  guest::StoreU32(base, frame + 84, rate);
  const double rate_float = double(float(double(int64_t(uint64_t(rate)))));
  const double product = double(float(rate_float * ratio));
  guest::StoreU32(base, converter + 16, uint32_t(ToFixed(product)));
}

double TwoToThe(uint8_t* base, double octaves) {
  return c_runtime::Pow(std::bit_cast<double>(uint64_t(REX_LOAD_U64(0x820056D8))), octaves);
}

}  // namespace

uint64_t CodecSetFlags(uint8_t* base, uint32_t codec_base, uint32_t mask, uint32_t bits,
                       uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82467078 CodecBase::SetFlags");
  SpinTake(base, thread, call);
  const uint8_t state = guest::LoadU8(base, codec_base + 156);
  guest::StoreU8(base, codec_base + 156, uint8_t((state & ~(mask & 0xFF)) | (bits & 0xFF)));
  SpinRelease(base, thread, call);
  return LastCallR3();
}

uint64_t CodecRetirePacket(uint8_t* base, uint32_t codec_base, uint64_t entry, uint32_t thread,
                           uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246A430 CodecBase::RetirePacket");
  SpinTake(base, thread, call);
  if (uint32_t(entry) == 0) {
    entry = FirstEntry(base, codec_base + 4);
    if (uint32_t(entry) == 0) {
      SpinRelease(base, thread, call);
      return 0;
    }
  }
  UnlinkEntry(base, codec_base + 4, entry);
  LinkEntryAtEnd(base, codec_base + 16, entry);
  SetRegs(call, Args({codec_base + 104, 0, 48}));
  Clear(base, codec_base + 104, 48);
  guest::StoreU8(base, uint32_t(entry) + 116, 0);
  SetFlagsCall(base, codec_base, 32, 32, 0x8246A5A0, frame, call);
  SpinRelease(base, thread, call);
  return entry;
}

uint64_t CodecCompletePacket(uint8_t* base, uint32_t codec_base, uint64_t entry, uint64_t result,
                             uint32_t thread, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824674D8 CodecBase::CompletePacket");
  SetRegs(call, Args({codec_base, entry}));
  const uint64_t done =
      CodecRetirePacket(base, codec_base, entry, thread, Below(base, frame, 128), call);
  const uint32_t callback = guest::LoadU32(base, codec_base + 160);
  if (uint32_t(done) != 0 && callback != 0) {
    guest::StoreU32(base, frame + 88, uint32_t(result));
    guest::StoreU32(base, frame + 80, guest::LoadU32(base, codec_base - 4));
    guest::StoreU32(base, frame + 84, guest::LoadU32(base, uint32_t(done) + 36));
    call(callback, 0x82467528, At(frame, Args({frame + 80})));
  }
  return done;
}

uint64_t CodecFlushPackets(uint8_t* base, uint32_t codec_base, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824699E0 CodecBase::FlushPackets");
  if ((guest::LoadU8(base, codec_base + 156) & 1) != 0) {
    return 0xFFFFFFFF8000FFFFull;  // E_UNEXPECTED
  }
  while (call(Method(base, codec_base, 92), 0x82469A40,
              At(frame, Args({codec_base, 0, 0xFFFFFFFF80004004ull}))) != 0) {
  }
  SetFlagsCall(base, codec_base, 24, 0, 0x82469A64, frame, call);
  guest::StoreU32(base, codec_base + 152, 0);
  return 0;
}

uint64_t CodecBaseSetFormat(uint8_t* base, uint32_t codec_base, uint32_t format, uint32_t extra,
                            uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82469E58 CodecBase::SetFormat");
  if ((guest::LoadU8(base, codec_base + 156) & 1) != 0 ||
      uint32_t(FirstEntry(base, codec_base + 4)) != 0) {
    return 0xFFFFFFFF8000FFFFull;  // E_UNEXPECTED
  }
  Copy(base, codec_base + 36, format, 56, 0x82469EB0, frame, call);
  guest::StoreU8(base, codec_base + 157, uint8_t(extra));
  Regs regs = At(frame, Args({codec_base}));
  regs.f1_set = true;
  regs.f1 = double(guest::LoadFloat(base, 0x82000A30));
  call(Method(base, codec_base, 80), 0x82469ED0, regs);
  return LastCallR3();
}

uint64_t PcmCodecSetFormat(uint8_t* base, uint32_t codec, uint32_t format, uint32_t extra,
                           uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246CFA8 PcmCodec::SetFormat");
  SetRegs(call, Args({codec + 16}));
  const uint64_t r3 =
      CodecBaseSetFormat(base, codec + 16, format, extra, Below(base, frame, 112), call);
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t type = guest::LoadU8(base, codec + 196);
  const uint32_t channels = guest::LoadU8(base, codec + 197);
  const uint32_t bytes = guest::LoadU32(base, kSampleBytes + type * 4);
  guest::StoreU16(base, codec + 110, 1);
  guest::StoreU16(base, codec + 108, uint16_t(int64_t(int32_t(bytes)) * int64_t(int32_t(channels))));
  return r3;
}

uint64_t PcmCodecStart(uint8_t* base, uint32_t codec, uint32_t thread, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D410 PcmCodec::Start");
  const uint32_t codec_base = codec + 16;
  SpinTake(base, thread, call);
  const uint8_t state = guest::LoadU8(base, codec_base + 156);
  if ((state & 1) == 0) {
    SetFlagsCall(base, codec_base, 3, 3, 0x8246D4AC, frame, call);
  } else if ((state & 4) != 0) {
    SetFlagsCall(base, codec_base, 6, 2, 0x8246D4AC, frame, call);
  }
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t PcmCodecStop(uint8_t* base, uint32_t codec, uint32_t flags, uint32_t thread,
                      uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D500 PcmCodec::Stop");
  const uint32_t codec_base = codec + 16;
  SpinTake(base, thread, call);
  const uint8_t state = guest::LoadU8(base, codec_base + 156);
  if ((state & 1) != 0) {
    if ((flags & 1) != 0 || (state & 18) != 0) {
      SetFlagsCall(base, codec_base, 87, 0, 0x8246D5D8, frame, call);
    } else if ((state & 4) == 0) {
      SetFlagsCall(base, codec_base, 4, 4, 0x8246D5AC, frame, call);
      guest::StoreU16(base, codec_base + 158, guest::LoadU16(base, kCodecInfoWord));
    }
  }
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t CodecEndOfStream(uint8_t* base, uint32_t codec, uint32_t thread, uint32_t frame,
                          const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D630 XAudioCodec::EndOfStream");
  const uint32_t codec_base = codec + 16;
  SpinTake(base, thread, call);
  if (uint32_t(FirstEntry(base, codec_base + 4)) != 0) {
    SetFlagsCall(base, codec_base, 8, 8, 0x8246D6C8, frame, call);
  }
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t PcmCodecSetVolume(uint8_t* base, uint32_t codec, double volume, uint32_t thread,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D008 PcmCodec::SetVolume");
  SpinTake(base, thread, call);
  guest::StoreFloat(base, codec + 112, float(volume));
  guest::StoreFloat(base, codec + 224, float(volume));
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t PcmCodecSetPitch(uint8_t* base, uint32_t codec, double octaves, uint32_t thread,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D0C8 PcmCodec::SetPitch");
  SpinTake(base, thread, call);
  const double ratio = double(float(TwoToThe(base, octaves)));
  const uint32_t rate = guest::LoadU32(base, codec + 60);
  const uint32_t converter = codec + 184;
  guest::StoreFloat(base, codec + 116, float(ratio));
  const uint8_t channels = guest::LoadU8(base, codec + 56);
  const uint8_t type = guest::LoadU8(base, codec + 52);
  const uint32_t changed = guest::LoadU32(base, converter + 80);
  guest::StoreU8(base, converter + 13, channels);
  guest::StoreU8(base, converter + 12, type);
  guest::StoreU32(base, converter + 80, changed | 1);
  SetConverterRate(base, converter, frame, rate, ratio);
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t PcmCodecSetFrequencyRatio(uint8_t* base, uint32_t codec, double ratio, uint32_t thread,
                                   uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D1E0 PcmCodec::SetFrequencyRatio");
  SpinTake(base, thread, call);
  const uint32_t converter = codec + 184;
  const uint32_t rate = guest::LoadU32(base, codec + 60);
  guest::StoreFloat(base, codec + 116, float(ratio));
  const uint8_t type = guest::LoadU8(base, codec + 52);
  const uint8_t channels = guest::LoadU8(base, codec + 56);
  const uint32_t changed = guest::LoadU32(base, converter + 80);
  guest::StoreU8(base, converter + 12, type);
  guest::StoreU8(base, converter + 13, channels);
  guest::StoreU32(base, converter + 80, changed | 1);
  SetConverterRate(base, converter, frame, rate, ratio);
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t XmaCodecSetVolume(uint8_t* base, uint32_t codec, double volume, uint32_t thread,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246AB48 XmaCodec::SetVolume");
  SpinTake(base, thread, call);
  guest::StoreFloat(base, codec + 112, float(volume));
  for (uint32_t i = 0; i < guest::LoadU8(base, codec + 56); i = (i + 1) & 0xFF) {
    CopyFloat(base, guest::LoadU32(base, codec + 188) + 88 * i + 40, codec + 112);
  }
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t XmaCodecSetPitch(uint8_t* base, uint32_t codec, double octaves, uint32_t thread,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246AC40 XmaCodec::SetPitch");
  SpinTake(base, thread, call);
  guest::StoreFloat(base, codec + 116, float(double(float(TwoToThe(base, octaves)))));
  for (uint32_t i = 0; i < guest::LoadU8(base, codec + 56); i = (i + 1) & 0xFF) {
    const double ratio = double(guest::LoadFloat(base, codec + 116));
    const uint32_t converter = guest::LoadU32(base, codec + 188) + 88 * i;
    const uint32_t rate = guest::LoadU32(base, codec + 8 * i + 60);
    const uint32_t changed = guest::LoadU32(base, converter + 80);
    const uint8_t channels = guest::LoadU8(base, codec + 8 * (i + 8));
    guest::StoreU8(base, converter + 12, 1);
    guest::StoreU8(base, converter + 13, channels);
    guest::StoreU32(base, converter + 80, changed | 1);
    SetConverterRate(base, converter, frame, rate, ratio);
  }
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t CodecOnSubmit(uint8_t* base, uint32_t codec_base, uint32_t entry, uint32_t flags,
                       uint32_t thread, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82469AF8 CodecBase::OnSubmit");
  const uint32_t mode = flags & 0xFF;
  const auto word = [&](uint32_t offset) { return guest::LoadU32(base, entry + offset); };
  // Each failed check releases one level of the spin lock (an assertion's
  // path, probably) and goes on.
  const auto fail = [&]() { SpinRelease(base, thread, call); };
  if ((mode & 0x10) == 0) {
    if (word(32) == 0) {
      guest::StoreU32(base, entry + 32, word(12));
    }
    if (word(16) != 0) {
      if (word(20) >= word(12)) {
        fail();
      }
      if (word(24) > word(12)) {
        fail();
      }
    }
    if (word(28) >= word(12)) {
      fail();
    }
    if (word(32) > word(12)) {
      fail();
    }
  }
  if (word(16) != 0 && (mode & 0x20) == 0) {
    if (word(24) <= word(20)) {
      fail();
    }
    if (word(28) >= word(24)) {
      fail();
    }
  }
  if (word(32) != 0) {
    const uint32_t limit = word(16) != 0 ? word(20) : word(28);
    if (word(32) <= limit) {
      fail();
    }
  }
  SetFlagsCall(base, codec_base, 24, (mode & 1) != 0 ? 8 : 0, 0x82469E10, frame, call);
  if (uint32_t(FirstEntry(base, codec_base + 16)) == 0) {
    SetFlagsCall(base, codec_base, 32, 0, 0x82469E4C, frame, call);
  }
  return 0;
}

namespace {

// The first free entry (+16) moved to the end of the queued list (+4) and
// given the next sequence number (+32, never 0), +117 to +119 cleared; 0
// when there's none.
uint32_t TakeFreeEntry(uint8_t* base, uint32_t codec_base) {
  const uint32_t entry = uint32_t(FirstEntry(base, codec_base + 16));
  if (entry == 0) {
    return 0;
  }
  UnlinkEntry(base, codec_base + 16, entry);
  LinkEntryAtEnd(base, codec_base + 4, entry);
  uint32_t sequence = guest::LoadU32(base, codec_base + 32) + 1;
  guest::StoreU32(base, codec_base + 32, sequence);
  guest::StoreU32(base, entry + 112, sequence);
  if (sequence == 0) {
    sequence = guest::LoadU32(base, codec_base + 32) + 1;
    guest::StoreU32(base, codec_base + 32, sequence);
    guest::StoreU32(base, entry + 112, sequence);
  }
  guest::StoreU8(base, entry + 117, 0);
  guest::StoreU8(base, entry + 118, 0);
  guest::StoreU8(base, entry + 119, 0);
  return entry;
}

// No free entry: two levels of the spin lock released (the original's
// path), E_OUTOFMEMORY.
uint64_t NoFreeEntry(uint8_t* base, uint32_t thread, const GuestCall& call) {
  SpinRelease(base, thread, call);
  SpinRelease(base, thread, call);
  return kEOutOfMemory;
}

}  // namespace

std::optional<uint64_t> CodecSubmitPacket(uint8_t* base, uint32_t codec_base, uint32_t packet,
                                          uint32_t flags, uint32_t thread, uint32_t frame,
                                          const GuestCall& call) {
  if (guest::LoadU16(base, codec_base + 92) == 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82469EF8 CodecBase::SubmitPacket");
  SpinTake(base, thread, call);
  const uint32_t entry = TakeFreeEntry(base, codec_base);
  if (entry == 0) {
    return NoFreeEntry(base, thread, call);
  }
  guest::StoreU32(base, entry + 8, guest::LoadU32(base, packet));
  const uint32_t block = guest::LoadU16(base, codec_base + 92);
  guest::StoreU32(base, entry + 12, guest::LoadU32(base, packet + 4) / block);
  guest::StoreU32(base, entry + 16, guest::LoadU32(base, packet + 8));
  if ((flags & 0x20) != 0) {
    guest::StoreU32(base, entry + 20, 0);
    guest::StoreU32(base, entry + 24, 0);
    Copy(base, entry + 40, packet + 12, 72, 0x8246A054, frame, call);
  } else {
    guest::StoreU32(base, entry + 20,
                    guest::LoadU32(base, packet + 12) / guest::LoadU16(base, codec_base + 92));
    const uint32_t length = guest::LoadU32(base, packet + 16);
    if (length != 0) {
      const uint32_t blocks = length / guest::LoadU16(base, codec_base + 92);
      guest::StoreU32(base, entry + 24, blocks + guest::LoadU32(base, entry + 20));
    } else {
      guest::StoreU32(base, entry + 24, guest::LoadU32(base, entry + 12));
    }
  }
  guest::StoreU32(base, entry + 28, 0);
  guest::StoreU32(base, entry + 32, 0);
  guest::StoreU32(base, entry + 36, guest::LoadU32(base, packet + 84));
  SetRegs(call, Args({codec_base, entry, flags}));
  const uint64_t r3 =
      CodecOnSubmit(base, codec_base, entry, flags, thread, Below(base, frame, 128), call);
  SpinRelease(base, thread, call);
  return r3;
}

std::optional<uint64_t> CodecSubmitRegion(uint8_t* base, uint32_t codec_base, uint32_t region,
                                          uint32_t flags, uint32_t thread, uint32_t frame,
                                          const GuestCall& call) {
  if (guest::LoadU16(base, codec_base + 92) == 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8246A1C0 CodecBase::SubmitRegion");
  SpinTake(base, thread, call);
  const uint32_t entry = TakeFreeEntry(base, codec_base);
  if (entry == 0) {
    return NoFreeEntry(base, thread, call);
  }
  guest::StoreU32(base, entry + 8, guest::LoadU32(base, region));
  const uint32_t block = guest::LoadU16(base, codec_base + 92);
  guest::StoreU32(base, entry + 12, guest::LoadU32(base, region + 4) / block);
  for (uint32_t i = 8; i <= 28; i += 4) {
    guest::StoreU32(base, entry + 8 + i, guest::LoadU32(base, region + i));
  }
  SetRegs(call, Args({codec_base, entry, flags}));
  const uint64_t r3 =
      CodecOnSubmit(base, codec_base, entry, flags, thread, Below(base, frame, 128), call);
  SpinRelease(base, thread, call);
  return r3;
}

std::optional<uint64_t> XmaCodecSubmitRegion(uint8_t* base, uint32_t codec, uint32_t region,
                                             uint32_t flags, uint32_t thread, uint32_t frame,
                                             const GuestCall& call) {
  if (guest::LoadU8(base, codec + 266) == 1) {
    ITC_NATIVE_RUNNING("sub_8246A860 XmaCodec::SubmitRegion");
    return kENotImpl;
  }
  if (guest::LoadU16(base, codec + 16 + 92) == 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8246A860 XmaCodec::SubmitRegion");
  guest::StoreU8(base, codec + 266, 2);
  const uint32_t loops = guest::LoadU32(base, region + 8);
  uint32_t from = region;
  if (loops > 255 && int32_t(loops) != -1) {
    for (uint32_t i = 0; i < 32; i += 4) {
      guest::StoreU32(base, frame + 80 + i, guest::LoadU32(base, region + i));
    }
    guest::StoreU32(base, frame + 88, 0xFFFFFFFF);
    from = frame + 80;
  }
  const uint32_t mode = (flags & 0xFF) | 16;
  SetRegs(call, Args({codec + 16, from, mode}));
  return CodecSubmitRegion(base, codec + 16, from, mode, thread, Below(base, frame, 144), call);
}

// --- The output matrix ---

namespace {

using Vec = simde__m128i;

// lvx and stvx (aligned down, the words byte-swapped and the lanes in the
// translated code's order) and lvlx, as the translated code does them.
Vec LoadAligned(uint8_t* base, uint32_t address) {
  return simde_mm_shuffle_epi8(
      simde_mm_load_si128(reinterpret_cast<const Vec*>(REX_RAW_ADDR(address & ~0xFu))),
      simde_mm_load_si128(reinterpret_cast<const Vec*>(rex::ppc::VectorMaskL)));
}
Vec LoadLeft(uint8_t* base, uint32_t address) {
  return simde_mm_shuffle_epi8(
      simde_mm_load_si128(reinterpret_cast<const Vec*>(REX_RAW_ADDR(address & ~0xFu))),
      simde_mm_load_si128(
          reinterpret_cast<const Vec*>(&rex::ppc::VectorMaskL[(address & 0xF) * 16])));
}
void StoreAligned(uint8_t* base, uint32_t address, Vec value) {
  simde_mm_store_si128(
      reinterpret_cast<Vec*>(REX_RAW_ADDR(address & ~0xFu)),
      simde_mm_shuffle_epi8(value,
                            simde_mm_load_si128(reinterpret_cast<const Vec*>(rex::ppc::VectorMaskL))));
}
// vspltw of the first lane.
Vec SplatFirst(Vec value) { return simde_mm_shuffle_epi32(value, 0xFF); }

// The vector unit's float operations with flush to zero on (the translated
// code's enableFlushMode before them); the operands in its order, each
// passed through an empty asm so nothing moves across the control word.
struct FlushToZero {
  FlushToZero() : csr(Fpscr::getcsr()) { Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask)); }
  ~FlushToZero() { Fpscr::setcsr(csr); }
  uint32_t csr;
};
Vec Pin(Vec value) {
  asm volatile("" : "+x"(value));
  return value;
}
Vec Add(Vec a, Vec b) {
  return Pin(simde_mm_castps_si128(
      simde_mm_add_ps(simde_mm_castsi128_ps(Pin(a)), simde_mm_castsi128_ps(Pin(b)))));
}
Vec Mul(Vec a, Vec b) {
  return Pin(simde_mm_castps_si128(
      simde_mm_mul_ps(simde_mm_castsi128_ps(Pin(a)), simde_mm_castsi128_ps(Pin(b)))));
}
// vmaddfp as the translated code computes it: a x b, then + c.
Vec Madd(Vec a, Vec b, Vec c) { return Add(Mul(a, b), c); }

// The float word at `address` as lfs loads it.
double LoadSingle(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}

}  // namespace

uint64_t MatrixInit(uint8_t* base, uint32_t matrix, uint32_t descriptor, uint32_t allocator,
                    uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82465C30 XAudioMatrix::Init");
  const uint32_t levels = guest::LoadU32(base, descriptor + 12);
  uint32_t entries = guest::LoadU8(base, descriptor + 8);
  if (levels != 0) {
    const uint32_t count = guest::LoadU8(base, levels);
    if (entries <= count) {
      entries = count;
    }
  }
  guest::StoreU8(base, matrix + 24, uint8_t(entries));
  if (entries != 0) {
    const uint64_t array = Allocate(base, allocator, entries * 12, 0x82465CA4, frame, call);
    guest::StoreU32(base, matrix + 20, uint32_t(array));
  }
  call(Method(base, matrix, 40), 0x82465CC0,
       At(frame, Args({matrix, guest::LoadU32(base, descriptor + 12)})));
  if (int32_t(LastCallR3()) < 0) {
    return LastCallR3();
  }
  call(Method(base, matrix, 32), 0x82465CE0,
       At(frame, Args({matrix, guest::LoadU8(base, descriptor + 9)})));
  return LastCallR3();
}

uint64_t MatrixSetMatrix(uint8_t* base, uint32_t matrix, uint32_t levels, uint32_t thread,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82465E50 XAudioMatrix::SetMatrix");
  SpinTake(base, thread, call);
  if (levels == 0) {
    guest::StoreU8(base, matrix + 16, 0);
    guest::StoreU8(base, matrix + 25, 0);
    SpinRelease(base, thread, call);
    return 0;
  }
  const uint8_t count = guest::LoadU8(base, levels);
  if (count > guest::LoadU8(base, matrix + 24)) {
    SpinRelease(base, thread, call);
    return 0xFFFFFFFF80070057ull;  // E_INVALIDARG
  }
  guest::StoreU8(base, matrix + 16, count);
  if (count != 0) {
    for (uint32_t i = 0; i < guest::LoadU8(base, matrix + 16); i++) {
      const uint32_t in = guest::LoadU32(base, levels + 4) + 8 * i;
      const uint32_t out = guest::LoadU32(base, matrix + 20) + 12 * i;
      guest::StoreU8(base, out, guest::LoadU8(base, in));
      guest::StoreU8(base, guest::LoadU32(base, matrix + 20) + 12 * i + 1,
                     guest::LoadU8(base, guest::LoadU32(base, levels + 4) + 8 * i + 1));
      CopyFloat(base, guest::LoadU32(base, matrix + 20) + 12 * i + 4,
                guest::LoadU32(base, levels + 4) + 8 * i + 4);
      CopyFloat(base, guest::LoadU32(base, matrix + 20) + 12 * i + 8,
                guest::LoadU32(base, levels + 4) + 8 * i + 4);
    }
  }
  uint32_t highest = 0;
  const uint32_t set = guest::LoadU8(base, matrix + 16);
  if (set != 0) {
    const uint32_t entries = guest::LoadU32(base, matrix + 20);
    for (uint32_t i = 0; i < set; i++) {
      const uint32_t channel = guest::LoadU8(base, entries + 12 * i + 1) + 1u;
      if (channel > highest) {
        highest = channel;
      }
    }
  }
  guest::StoreU8(base, matrix + 25, uint8_t(highest));
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t MatrixSetLevels(uint8_t* base, uint32_t matrix, uint32_t levels, uint32_t now,
                         uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82466188 XAudioMatrix::SetLevels");
  SpinTake(base, thread, call);
  if (guest::LoadU8(base, levels) != 0) {
    for (uint32_t i = 0; i < guest::LoadU8(base, levels); i = (i + 1) & 0xFF) {
      const uint32_t index = guest::LoadU8(base, guest::LoadU32(base, levels + 4) + 8 * i);
      if (index >= guest::LoadU8(base, matrix + 16)) {
        SpinRelease(base, thread, call);
        return 0xFFFFFFFF80070057ull;  // E_INVALIDARG
      }
      const uint32_t level = guest::LoadU32(base, levels + 4) + 8 * i;
      const uint32_t entry = guest::LoadU32(base, matrix + 20) + 12 * guest::LoadU8(base, level);
      CopyFloat(base, entry + 8, level + 4);
      if ((now & 0xFF) != 0) {
        CopyFloat(base, entry + 4, level + 4);
      }
    }
  }
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t MatrixMixRamp(uint8_t* base, uint64_t out, uint64_t in, uint32_t start, uint32_t step,
                       uint32_t frames) {
  ITC_NATIVE_RUNNING("sub_82466310 XAudioMatrix::MixRamp");
  Vec v0 = LoadAligned(base, step);
  const uint32_t vectors = frames >> 2;
  FlushToZero flush;
  Vec v11 = Add(v0, v0);
  Vec v13 = LoadAligned(base, start);
  Vec v12 = Add(v13, v0);
  Vec v10 = Add(v11, v0);
  v0 = Add(v11, v11);
  v11 = Add(v13, v11);
  v10 = Add(v13, v10);
  if (vectors == 0) {
    return out;
  }
  uint32_t loops = ((vectors - 1) >> 2) + 1;
  uint32_t r10 = uint32_t(out) + 48;
  uint64_t r11 = in + 32;  // r3 is left at its last value less 32, in full
  const uint32_t r5 = uint32_t(out) - uint32_t(in);
  uint64_t r3_left = out;
  while (loops-- != 0) {
    const uint32_t r4 = uint32_t(r11) + 16;
    const Vec v1 = v10, v31 = v13, v30 = v12, v29 = v11;
    r3_left = r11 - 32;
    const uint32_t r8 = r10 - 48, r3 = uint32_t(r3_left), r7 = r10 - 32,
                   r6 = r5 + uint32_t(r11), r31 = uint32_t(r11) - 16;
    Vec v9 = LoadAligned(base, r10);
    Vec v7 = LoadAligned(base, r4);
    const Vec v4 = LoadAligned(base, r8);
    v9 = Madd(v7, v1, v9);
    Vec v6 = LoadAligned(base, r3);
    Vec v8 = LoadAligned(base, uint32_t(r11));
    v7 = Madd(v6, v31, v4);
    const Vec v3 = LoadAligned(base, r7);
    r11 += 64;
    const Vec v2 = LoadAligned(base, r6);
    v13 = Add(v13, v0);
    const Vec v5 = LoadAligned(base, r31);
    v8 = Madd(v8, v29, v2);
    v6 = Madd(v5, v30, v3);
    v12 = Add(v12, v0);
    v11 = Add(v11, v0);
    v10 = Add(v10, v0);
    StoreAligned(base, r10, v9);
    r10 += 64;
    StoreAligned(base, r8, v7);
    StoreAligned(base, r6, v8);
    StoreAligned(base, r7, v6);
  }
  return r3_left;
}

namespace {

// The 4-float ramp vectors at the frame's +128 (the start: `first` x the
// constants at +160 + `level`) and +144 (the step, splatted), from the
// words at the frame's +84, +80 and +88, as the translated code makes them
// (the step's first splat stored at +144 and the level's at +128 before
// they're overwritten).
Vec RampVectors(uint8_t* base, uint32_t frame) {
  const Vec constants = LoadAligned(base, frame + 160);
  Vec v0 = SplatFirst(LoadLeft(base, frame + 84));
  StoreAligned(base, frame + 144, v0);
  const Vec v13 = SplatFirst(LoadLeft(base, frame + 80));
  {
    FlushToZero flush;
    v0 = Madd(v0, constants, v13);
  }
  StoreAligned(base, frame + 128, v13);
  StoreAligned(base, frame + 128, v0);
  StoreAligned(base, frame + 144, SplatFirst(LoadLeft(base, frame + 88)));
  return v0;
}

// The four constants (0x82000A34, 0x82000A30, 0x820021E8, 0x820021E4: the
// lanes' frame offsets 0 to 3, probably) to the frame's +160.
void RampConstants(uint8_t* base, uint32_t frame) {
  CopyFloat(base, frame + 160, 0x82000A34);
  CopyFloat(base, frame + 164, 0x82000A30);
  CopyFloat(base, frame + 168, 0x820021E8);
  CopyFloat(base, frame + 172, 0x820021E4);
}

void XMemSetCall(uint8_t* base, uint32_t destination, uint32_t bytes, uint32_t return_address,
                 uint32_t sp, const GuestCall& call) {
  SetRegs(call, Args({destination, 0, bytes}));
  c_runtime::XMemSet(base, destination, 0, bytes);
  (void)return_address;
  (void)sp;
}

void XMemCpyCall(uint8_t* base, uint32_t destination, uint32_t source, uint32_t bytes,
                 uint32_t return_address, uint32_t sp, const GuestCall& call) {
  SetRegs(call, Args({destination, source, bytes}));
  if (!c_runtime::XMemCpy(base, destination, source, bytes)) {
    call(0x82295890, return_address, At(sp, Args({destination, source, bytes})));
  }
}

}  // namespace

uint64_t MatrixMix(uint8_t* base, uint32_t matrix, uint32_t source, uint32_t target,
                   uint32_t thread, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82466440 XAudioMatrix::Mix");
  const uint32_t source_format = frame + 112, target_format = frame + 96;
  SetRegs(call, Args({source != 0 ? source - 8 : 0, source_format}));
  BufferGetFormat(base, source != 0 ? source - 8 : 0, source_format);
  const uint32_t target_buffer = target != 0 ? target - 8 : 0;
  SetRegs(call, Args({target_buffer, target_format}));
  BufferGetFormat(base, target_buffer, target_format);
  if (guest::LoadU8(base, matrix + 26) == 1) {
    guest::StoreU8(base, frame + 97, guest::LoadU8(base, matrix + 25));
    SetRegs(call, Args({target_buffer, target_format, 1}));
    uint64_t r3;
    if (const auto cleared = BufferClear(base, target_buffer, target_format, 1)) {
      r3 = uint64_t(int64_t(int32_t(*cleared)));  // as lis/ori leave the errors
    } else {
      call(0x8245F8E8, 0x824664C8, At(frame, Args({target_buffer, target_format, 1})));
      r3 = LastCallR3();
    }
    if (int32_t(r3) < 0) {
      return r3;
    }
  }
  SpinTake(base, thread, call);
  const uint32_t entries = guest::LoadU8(base, matrix + 16);
  if (entries != 0) {
    for (uint32_t i = 0; i < guest::LoadU8(base, matrix + 16); i++) {
      const uint32_t entry = guest::LoadU32(base, matrix + 20) + 12 * i;
      const uint32_t target_samples = guest::LoadU32(base, frame + 104);
      const uint32_t target_channels = guest::LoadU8(base, frame + 97);
      const uint32_t source_channels = guest::LoadU8(base, frame + 113);
      const uint32_t out_channel = guest::LoadU8(base, entry + 1);
      const double goal = LoadSingle(base, entry + 8);
      const uint32_t in_channel = guest::LoadU8(base, entry);
      const uint32_t out = (out_channel << 10) + target_samples;
      const uint32_t in = (in_channel << 10) + guest::LoadU32(base, frame + 120);
      if (out_channel >= target_channels) {
        continue;
      }
      if (in_channel >= source_channels) {
        if (guest::LoadU8(base, matrix + 26) != 0) {
          XMemSetCall(base, out, 1024, 0x824666A4, frame, call);
        }
        continue;
      }
      const double level = LoadSingle(base, entry + 4);
      guest::StoreFloat(base, frame + 80, float(level));
      const double difference = double(float(goal - level));
      RampConstants(base, frame);
      const bool overwrite = guest::LoadU8(base, matrix + 26) != 0;
      const double first = double(float(difference * LoadSingle(base, 0x82008058)));
      guest::StoreFloat(base, frame + 84, float(first));
      const double each = double(float(first * LoadSingle(base, 0x820021F8)));
      guest::StoreFloat(base, frame + 88, float(each));
      Vec v0 = RampVectors(base, frame);
      if (!overwrite) {
        SetRegs(call, Args({out, in, frame + 128, frame + 144, 256}));
        MatrixMixRamp(base, out, in, frame + 128, frame + 144, 256);
        guest::StoreFloat(base, entry + 4, float(goal));
        continue;
      }
      {
        FlushToZero flush;
        for (uint32_t k = 0; k < 64; k++) {
          const Vec v12 = v0;
          Vec v13 = LoadAligned(base, in + 16 * k);
          v13 = Mul(v13, v12);
          const Vec v11 = LoadAligned(base, frame + 144);
          v0 = Add(v0, v11);
          StoreAligned(base, out + 16 * k, v13);
        }
      }
      guest::StoreFloat(base, entry + 4, float(goal));
      StoreAligned(base, frame + 128, v0);
    }
  } else if (source != target && guest::LoadU8(base, frame + 97) != 0) {
    uint32_t offset = 0;
    for (uint32_t i = 0; i < guest::LoadU8(base, frame + 97); i++, offset += 1024) {
      const uint32_t out = offset + guest::LoadU32(base, frame + 104);
      const uint32_t in = offset + guest::LoadU32(base, frame + 120);
      const bool overwrite = guest::LoadU8(base, matrix + 26) != 0;
      if (i >= guest::LoadU8(base, frame + 113)) {
        if (overwrite) {
          XMemSetCall(base, out, 1024, 0x824667B8, frame, call);
          continue;
        }
        break;
      }
      if (overwrite) {
        XMemCpyCall(base, out, in, 1024, 0x824667A0, frame, call);
        continue;
      }
      // A ramp from 1.0 by 0: each channel added as it is.
      RampConstants(base, frame);
      CopyFloat(base, frame + 88, 0x82000A30);
      CopyFloat(base, frame + 84, 0x82000A34);
      CopyFloat(base, frame + 80, 0x82000A34);
      const Vec constants = LoadAligned(base, frame + 160);
      Vec v0 = SplatFirst(LoadLeft(base, frame + 84));
      StoreAligned(base, frame + 144, v0);
      const Vec v13 = SplatFirst(LoadLeft(base, frame + 88));
      {
        FlushToZero flush;
        v0 = Madd(v0, constants, v13);
      }
      StoreAligned(base, frame + 128, v13);
      StoreAligned(base, frame + 128, v0);
      StoreAligned(base, frame + 144, SplatFirst(LoadLeft(base, frame + 80)));
      SetRegs(call, Args({out, in, frame + 128, frame + 144, 256}));
      MatrixMixRamp(base, out, in, frame + 128, frame + 144, 256);
    }
  }
  SpinRelease(base, thread, call);
  return 0;
}

// --- The volume meter ---

uint64_t MeterProcess(uint8_t* base, uint32_t meter, uint32_t buffer, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_825A6210 VolumeMeter::Process");
  BufferGetFormatThunk(base, buffer, frame + 80);
  const uint32_t meter_channels = guest::LoadU32(base, meter + 16);
  uint32_t channels = guest::LoadU8(base, frame + 81);
  if (channels >= meter_channels) {
    channels = meter_channels;
  }
  const double zero = LoadSingle(base, 0x82000A34);
  const double threshold = LoadSingle(base, 0x82072298);
  double counted = zero;  // f10: also the second fold-down's sum
  uint32_t sample = 0;    // r8: runs on across the channels
  if (meter_channels != 0) {
    const double scale = LoadSingle(base, 0x82008058);
    for (uint32_t c = 0; c < guest::LoadU32(base, meter + 16); c = (c + 1) & 0xFF) {
      const uint32_t history = guest::LoadU32(base, meter + 24);
      const uint32_t length = guest::LoadU32(base, meter + 20);
      double peak = counted, sum = counted, squares = counted;
      const uint32_t quotient = length != 0 ? history / length : 0;
      const uint32_t slot = history - uint32_t(int64_t(int32_t(quotient)) * int64_t(int32_t(length)));
      const uint32_t index =
          uint32_t(int64_t(int32_t(slot)) * int64_t(int32_t(guest::LoadU32(base, meter + 16)))) + c;
      if (c < channels) {
        const uint32_t samples = guest::LoadU32(base, frame + 88);
        for (uint32_t k = 0; k < 256; k++) {
          const uint32_t at = (sample & 0xFFFF) * 4 + samples;
          const double x = LoadSingle(base, at);
          const double magnitude = std::bit_cast<double>(std::bit_cast<uint64_t>(x) & ~(1ull << 63));
          squares = double(float(std::fma(x, x, squares)));
          if (!(peak > magnitude)) {
            peak = magnitude;
          }
          const double again = std::bit_cast<double>(
              std::bit_cast<uint64_t>(LoadSingle(base, at)) & ~(1ull << 63));
          sample = (sample + 1) & 0xFFFF;
          if (again > threshold) {
            sum = double(float(sum + threshold));
          }
        }
        squares = double(float(std::sqrt(double(float(squares * scale)))));
      }
      guest::StoreFloat(base, guest::LoadU32(base, meter + 36) + index * 4, float(squares));
      guest::StoreFloat(base, index * 4 + guest::LoadU32(base, meter + 28), float(sum));
      guest::StoreFloat(base, index * 4 + guest::LoadU32(base, meter + 32), float(peak));
    }
  }
  if (guest::LoadU32(base, meter + 16) == 8 && channels == 6) {
    const uint32_t samples = guest::LoadU32(base, frame + 88);
    double peak_left = counted, peak_right = counted, sum_left = counted;
    const double side = LoadSingle(base, 0x82001BE4);
    const double center = LoadSingle(base, 0x8200295C);
    const auto at = [&](uint32_t k) { return LoadSingle(base, samples + 4 * k); };
    const auto magnitude = [](double x) {
      return std::bit_cast<double>(std::bit_cast<uint64_t>(x) & ~(1ull << 63));
    };
    for (uint32_t i = 0; i < 256; i++) {
      const double c = double(float(at(i + 512) * center));
      double left = double(float(std::fma(at(i + 1024), side, at(i))));
      double right = double(float(std::fma(at(i + 1280), side, at(i + 256))));
      left = double(float(left + c));
      right = double(float(right + c));
      if (!(peak_left > magnitude(left))) {
        peak_left = magnitude(left);
      }
      if (!(peak_right > magnitude(right))) {
        peak_right = magnitude(right);
      }
      if (magnitude(left) > threshold) {
        sum_left = double(float(sum_left + threshold));
      }
      if (magnitude(right) > threshold) {
        counted = double(float(counted + threshold));
      }
    }
    guest::StoreFloat(base, guest::LoadU32(base, meter + 28) + 24, float(sum_left));
    guest::StoreFloat(base, guest::LoadU32(base, meter + 28) + 28, float(counted));
    guest::StoreFloat(base, guest::LoadU32(base, meter + 32) + 24, float(peak_left));
    guest::StoreFloat(base, guest::LoadU32(base, meter + 32) + 28, float(peak_right));
  }
  const uint32_t history = guest::LoadU32(base, meter + 24) + 1;
  guest::StoreU32(base, meter + 24, history);
  if (history == guest::LoadU32(base, meter + 16)) {
    guest::StoreU32(base, meter + 44, 1);
  }
  return 0;
}

// --- The render driver's set-up and the reverb's small parts ---

uint32_t RenderDriverSetUpThunk(uint8_t* base, uint32_t driver, uint32_t descriptor, uint32_t* r4) {
  ITC_NATIVE_RUNNING("sub_824657C8 the render driver's set-up");
  *r4 = guest::LoadU32(base, descriptor + 8);
  return Method(base, driver, 28);
}

void ReverbDestruct(uint8_t* base, uint32_t reverb) {
  ITC_NATIVE_RUNNING("sub_824656B8 Reverb::Destruct");
  guest::StoreU32(base, reverb, 0x8204AAB0);
  guest::StoreU32(base, reverb + 4, 0x8204AA94);
  const uint32_t offset = guest::LoadU32(base, guest::LoadU32(base, reverb + 16) + 4);
  guest::StoreU32(base, offset + reverb + 68 - 52, 0x8204AA74);
  guest::StoreU32(base, reverb + 16 + 192, 0x8204A868);
  guest::StoreU32(base, reverb + 4, kRootVtable);
}

void ReverbDefaults(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82467538 ReverbCore::Defaults");
  CopyFloat(base, out + 52, 0x8204AC10);
  guest::StoreU32(base, out + 0, 5);
  CopyFloat(base, out + 72, 0x8204AC18);
  guest::StoreU32(base, out + 4, 5);
  guest::StoreU32(base, out + 8, 6);
  CopyFloat(base, out + 56, 0x8204AC14);
  guest::StoreU32(base, out + 12, 6);
  CopyFloat(base, out + 60, 0x8204AC14);
  guest::StoreU32(base, out + 48, 5);
  CopyFloat(base, out + 64, 0x8204AC14);
  CopyFloat(base, out + 68, 0x8204AC14);
  guest::StoreU32(base, out + 24, 8);
  CopyFloat(base, out + 76, 0x8204AC1C);
  guest::StoreU32(base, out + 28, 8);
  CopyFloat(base, out + 80, 0x8204AC1C);
  guest::StoreU32(base, out + 32, 8);
  guest::StoreU32(base, out + 36, 4);
  guest::StoreU32(base, out + 40, 8);
  guest::StoreU32(base, out + 44, 4);
}

uint32_t LargestPrimeBelow(uint8_t* base, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_8246DFE8 LargestPrimeBelow");
  if (n <= 1) {
    return 1;
  }
  if (n >= 9973) {
    return 9973;
  }
  constexpr uint32_t kPrimes = 0x8204BBD0;
  uint32_t low = 0, high = 1229, middle = 1;
  for (;;) {
    const uint32_t previous = middle;
    middle = (high + low) >> 1;
    const uint32_t prime = guest::LoadU32(base, kPrimes + 4 * middle);
    if (prime == n) {
      break;
    }
    if (prime > n) {
      high = middle - 1;
    } else {
      low = middle + 1;
    }
    if (middle == previous) {
      break;
    }
  }
  return guest::LoadU32(base, kPrimes + 4 * middle);
}

void FillDelayLine(uint8_t* base, uint32_t line, uint32_t groups) {
  switch (groups) {
    case 2048: ITC_NATIVE_RUNNING("sub_82467720 the reverb's delay line fill (2,048)"); break;
    case 64: ITC_NATIVE_RUNNING("sub_82467788 the reverb's delay line fill (64)"); break;
    case 16: ITC_NATIVE_RUNNING("sub_824677F0 the reverb's delay line fill (16)"); break;
    case 256: ITC_NATIVE_RUNNING("sub_82467858 the reverb's delay line fill (256)"); break;
    case 32: ITC_NATIVE_RUNNING("sub_824678C0 the reverb's delay line fill (32)"); break;
    case 128: ITC_NATIVE_RUNNING("sub_82467928 the reverb's delay line fill (128)"); break;
    case 512: ITC_NATIVE_RUNNING("sub_82467990 the reverb's delay line fill (512)"); break;
  }
  uint32_t at = line + 12;
  for (uint32_t i = 0; i < groups; i++, at += 32) {
    for (uint32_t k = 0; k < 8; k++) {
      CopyFloat(base, at - 4 + 4 * k, line + 4);
    }
  }
  guest::StoreU32(base, line, 0);
}

void FilterCoefficient(uint8_t* base, uint32_t filter) {
  ITC_NATIVE_RUNNING("sub_824675B8 ReverbCore::FilterCoefficient");
  const auto single = [&](uint32_t address) { return LoadSingle(base, address); };
  const auto dbl = [&](uint32_t address) {
    return std::bit_cast<double>(uint64_t(REX_LOAD_U64(address)));
  };
  const auto f = [](double x) { return double(float(x)); };
  double f0 = single(filter + 12);
  double f13 = single(filter + 8);
  const double f29 = f(f0 + f13);
  const double f26 = single(0x8204AC14);
  f0 = single(0x8204AC18);
  const double f28 = f29 < f26 ? f(f13 / f29) : f0;
  double f12 = f(f0 - f28);
  f13 = single(filter + 4);
  f0 = f13 * dbl(0x8204B8B8);
  f13 = single(filter);
  f12 = f(f12 * f29);
  double f31 = dbl(0x8200F728);
  f13 = f0 / f13;
  f0 = single(0x82001FBC);
  double f2 = f(f12 * f0);
  const double f27 = f(f13);
  double f1 = c_runtime::Pow(f31, f2);
  f1 = f(f1);
  const double f30 = dbl(0x820022B8);
  f1 = c_runtime::Pow(f1, f30);
  f0 = f1;
  f12 = f(f28 * f29);
  f1 = f31;
  f13 = single(0x82002204);
  f31 = f(f0);
  f2 = f(f12 * f13);
  f1 = c_runtime::Pow(f1, f2);
  f1 = f(f1);
  guest::StoreFloat(base, filter + 24, float(f1));
  f1 = c_runtime::Pow(f1, f30);
  const double limit = single(0x8204B8B0);
  f0 = f(f1);
  guest::StoreFloat(base, filter + 24, float(f0));
  if (!(f31 < limit)) {
    guest::StoreFloat(base, filter + 28, float(f26));
    return;
  }
  f1 = c_runtime::Cos(f27);
  double f11 = f(f31 * f31);
  const double two = dbl(0x820056D8);
  f0 = dbl(0x82002250);
  f13 = f0 - f31;
  const double f10 = f(f13);
  f13 = f(f1);
  double f9 = f(f13 * f13);
  const double f8 = f(f13 * f31);
  f13 = f0 - f13;
  f9 = f0 - f9;
  f0 = f0 - f8;
  f13 = f13 * f31;
  f11 = f9 * f11;
  f13 = std::fma(f13, two, -f11);
  f13 = std::sqrt(f13);
  f0 = f0 - f13;
  f0 = f(f0);
  f0 = f(f0 / f10);
  guest::StoreFloat(base, filter + 28, float(f0));
}

uint64_t ReverbSetParameters(uint8_t* base, uint32_t reverb, uint32_t kind, uint32_t data,
                             uint32_t thread, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82462148 Reverb::SetParameters");
  const uint32_t which = kind & 0xFF;
  if (which >= 3) {
    return 0xFFFFFFFF80070057ull;  // E_INVALIDARG
  }
  const uint32_t core = reverb + 16;
  if (which == 2) {
    const double gain = LoadSingle(base, data);
    SpinTake(base, thread, call);
    SpinTake(base, thread, call);
    guest::StoreFloat(base, reverb + 200,
                      float(double(float(gain * LoadSingle(base, 0x82002A0C)))));
    guest::StoreU8(base, reverb + 204, 1);
  } else if (which == 1) {
    const uint32_t source = guest::LoadU32(base, data);
    SpinTake(base, thread, call);
    SpinTake(base, thread, call);
    Copy(base, core + 136, source, 48, 0x824622D8, frame, call);
    guest::StoreU8(base, core + 190, 1);
  } else {
    // The native parameters read into the frame over the defaults.
    const uint32_t source = guest::LoadU32(base, data);
    const uint32_t local = frame + 80;
    SetRegs(call, Args({local}));
    ReverbDefaults(base, local);
    guest::StoreU32(base, local, guest::LoadU32(base, source));
    static constexpr uint8_t kBytes[] = {4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 5};
    for (uint32_t i = 0; i < 12; i++) {
      guest::StoreU32(base, local + 4 + 4 * i, guest::LoadU8(base, source + kBytes[i]));
    }
    for (uint32_t i = 0; i < 6; i++) {
      CopyFloat(base, local + 52 + 4 * i, source + 16 + 4 * i);
    }
    guest::StoreFloat(base, local + 76,
                      float(double(float(LoadSingle(base, source + 40) *
                                         LoadSingle(base, 0x82002A0C)))));
    guest::StoreFloat(base, local + 80,
                      float(double(float(LoadSingle(base, source + 44) *
                                         LoadSingle(base, 0x8204AA80)))));
    SpinTake(base, thread, call);
    SpinTake(base, thread, call);
    Copy(base, core + 52, local, 84, 0x8246244C, frame, call);
    guest::StoreU8(base, core + 189, 1);
  }
  SpinRelease(base, thread, call);
  SpinRelease(base, thread, call);
  return 0;
}

namespace {

// fctiwz as the translated code computes it (the word stfiwx stores).
uint32_t ToWord(double value) {
  return std::isnan(value)            ? 0x80000000u
         : value >= double(INT_MAX) ? uint32_t(INT_MAX)
                                    : uint32_t(simde_mm_cvttsd_si32(simde_mm_load_sd(&value)));
}

// The decay HF ratio's log10 times the double at `scale`, by fctiwz through
// the frame's word at +80, to a step: 0 below -8, the step + 8 below 0,
// else 8.
uint32_t DecayStep(uint8_t* base, double log, uint32_t scale, uint32_t frame) {
  const double product =
      double(float(log)) * std::bit_cast<double>(uint64_t(REX_LOAD_U64(scale)));
  guest::StoreU32(base, frame + 80, ToWord(product));
  const int32_t step = int32_t(guest::LoadU32(base, frame + 80));
  return step < -8 ? 0u : step < 0 ? uint32_t(step + 8) : 8u;
}

}  // namespace

double I3dl2ToNative(uint8_t* base, uint32_t in, uint32_t out, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82469418 I3dl2::ToNative");
  const auto f = [](double x) { return double(float(x)); };
  // A word through the frame (std, lfd, fcfid) and frsp: `bits` the 64 bits
  // stored.
  const auto integer = [&](uint64_t bits) {
    REX_STORE_U64(frame + 80, bits);
    return f(double(int64_t(bits)));
  };
  const auto level = [&](uint32_t address) {
    return uint64_t(int64_t(int32_t(guest::LoadU32(base, address))));
  };
  guest::StoreU32(base, out + 48, guest::LoadU32(base, 0x8204AC0C));
  guest::StoreU32(base, out + 8, guest::LoadU32(base, 0x8204AC04));
  guest::StoreU32(base, out + 12, guest::LoadU32(base, 0x8204AC04));
  guest::StoreU32(base, out + 16, guest::LoadU32(base, 0x8204AC08));
  guest::StoreU32(base, out + 20, guest::LoadU32(base, 0x8204AC08));
  CopyFloat(base, out + 80, 0x8204AC1C);
  double f0 = integer(level(in));
  const double f31 = LoadSingle(base, 0x8200222C);
  guest::StoreFloat(base, out + 56, float(f(f0 * f31)));
  const uint64_t room_hf = level(in + 4);
  guest::StoreU32(base, out + 36, 4);
  guest::StoreU32(base, out + 44, 6);
  f0 = integer(room_hf);
  const double f30 = LoadSingle(base, 0x82000A30);
  guest::StoreFloat(base, out + 60, float(f(f0 * f31)));
  double f1;
  if (!(LoadSingle(base, in + 16) < f30)) {
    guest::StoreU32(base, out + 40, 8);
    f1 = c_runtime::Log10(LoadSingle(base, in + 16));
    guest::StoreU32(base, out + 32, DecayStep(base, f1, 0x8204B940, frame));
    f0 = f(LoadSingle(base, in + 12) * LoadSingle(base, in + 16));
    guest::StoreFloat(base, out + 72, float(f0));
  } else {
    guest::StoreU32(base, out + 32, 8);
    f1 = c_runtime::Log10(LoadSingle(base, in + 16));
    guest::StoreU32(base, out + 40, DecayStep(base, f1, 0x8204B938, frame));
    CopyFloat(base, out + 72, in + 12);
  }
  f0 = integer(level(in + 20));
  guest::StoreFloat(base, out + 64, float(f(f0 * f31)));
  // The reflections delay, clamped below the table's word less 1 and to at
  // least 1.
  const uint32_t reflections_limit = guest::LoadU32(base, 0x8204ABFC);
  f0 = LoadSingle(base, in + 24);
  double f13 = integer(reflections_limit);
  const double f12 = LoadSingle(base, 0x82001BDC);
  f0 = f(f0 * f12);
  if (!(f0 < f13)) {
    f0 = f(f13 - f30);
  }
  if (!(f0 > f30)) {
    f0 = f30;
  }
  guest::StoreU32(base, out + 0, uint32_t(ToFixed(f0)));
  f0 = integer(level(in + 28));
  guest::StoreFloat(base, out + 68, float(f(f0 * f31)));
  // The reverb delay, clamped below the table's word less 1 only.
  const uint32_t reverb_limit = guest::LoadU32(base, 0x8204AC00);
  f0 = f(LoadSingle(base, in + 32) * f12);
  f13 = integer(reverb_limit);
  if (!(f0 < f13)) {
    f0 = f(f13 - f30);
  }
  guest::StoreU32(base, out + 4, uint32_t(ToFixed(f0)));
  // The diffusion, scaled.
  f0 = f(LoadSingle(base, in + 36) * LoadSingle(base, 0x82002C4C));
  guest::StoreU32(base, frame + 80, uint32_t(ToFixed(f0)));
  const uint32_t diffusion = guest::LoadU32(base, frame + 80);
  guest::StoreU32(base, out + 24, diffusion);
  guest::StoreU32(base, out + 28, diffusion);
  CopyFloat(base, out + 76, in + 40);
  CopyFloat(base, out + 52, in + 44);
  return f1;
}

// --- The PCM codec's pass ---

namespace {

constexpr uint32_t kSetFlagsFunction = 0x82467078;       // CodecBase::SetFlags
constexpr uint32_t kCompletePacketFunction = 0x824674D8;  // CodecBase::CompletePacket
constexpr uint32_t kPcmCodecStopFunction = 0x8246D500;   // PcmCodec::Stop
constexpr uint32_t kBufferClearFunction = 0x8245F8E8;    // XAudioBuffer::Clear
constexpr uint32_t kFloatZero = 0x82000A34;              // the f31 constant
constexpr uint32_t kFadeStep = 0x8204B954;               // 0.5

// The codec base's slot +84 (SetFlags): its plain C++ when it's the game's
// (in a frame of its own below `frame`), else the function through `call`.
void ProcessSetFlags(uint8_t* base, uint32_t codec_base, uint32_t mask, uint32_t bits,
                     uint32_t return_address, uint32_t thread, uint32_t frame,
                     const GuestCall& call) {
  const uint32_t target = Method(base, codec_base, 84);
  if (target == kSetFlagsFunction) {
    SetRegs(call, Args({codec_base, mask, bits}));
    Below(base, frame, 144);
    CodecSetFlags(base, codec_base, mask, bits, thread, call);
  } else {
    call(target, return_address, At(frame, Args({codec_base, mask, bits})));
  }
}

// The codec's slot +52 (Stop) with 1 (at once): as ProcessSetFlags.
void ProcessStop(uint8_t* base, uint32_t codec, uint32_t return_address, uint32_t thread,
                 uint32_t frame, const GuestCall& call) {
  const uint32_t target = Method(base, codec, 52);
  if (target == kPcmCodecStopFunction) {
    SetRegs(call, Args({codec, 1}));
    PcmCodecStop(base, codec, 1, thread, Below(base, frame, 128), call);
  } else {
    call(target, return_address, At(frame, Args({codec, 1})));
  }
}

// The rest of the converter's output zeroed (memset a channel every 1,024
// bytes from the frames written), as the original's loops.
void ZeroRest(uint8_t* base, uint32_t converter, const GuestCall& call) {
  const uint32_t written = guest::LoadU32(base, converter + 28);
  const uint32_t left = guest::LoadU32(base, converter + 24) - written;
  uint32_t channels = guest::LoadU8(base, converter + 13);
  if (left == 0 || channels == 0) {
    return;
  }
  const uint32_t bytes = left << 2;
  uint32_t offset = written << 2;
  while (channels != 0) {
    const uint32_t at = guest::LoadU32(base, converter + 20) + offset;
    SetRegs(call, Args({at, 0, bytes}));
    Clear(base, at, bytes);
    channels--;
    offset += 1024;
  }
}

}  // namespace

uint64_t PcmCodecProcess(uint8_t* base, uint32_t codec, uint64_t iface, uint32_t thread,
                         uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D860 PcmCodec::Process");
  const uint32_t codec_base = codec + 16;
  const uint32_t converter = codec + 184;
  const uint32_t format = frame + 80;
  const uint64_t buffer = uint32_t(iface) != 0 ? iface - 8 : 0;
  SpinTake(base, thread, call);
  SetRegs(call, Args({buffer, format}));
  uint64_t r29 = BufferGetFormat(base, uint32_t(buffer), format);
  if (int32_t(r29) >= 0) {
    guest::StoreU8(base, frame + 81, guest::LoadU8(base, codec + 56));
    SetRegs(call, Args({buffer, format, 1}));
    if (const auto cleared = BufferClear(base, uint32_t(buffer), format, 1)) {
      r29 = uint64_t(int64_t(int32_t(*cleared)));  // as lis/ori leave the errors
    } else {
      call(kBufferClearFunction, 0x8246D904, At(frame, Args({buffer, format, 1})));
      r29 = LastCallR3();
    }
  }
  if (int32_t(r29) < 0) {
    SpinRelease(base, thread, call);
    if (r29 != 0) {
      return r29;
    }
  } else {
    guest::StoreU32(base, converter + 20, guest::LoadU32(base, frame + 88));
    guest::StoreU32(base, converter + 24, 256);
    guest::StoreU32(base, converter + 28, 0);
    const uint8_t state = guest::LoadU8(base, codec + 172);
    if ((state & 1) == 0 || (state & 0x10) != 0) {
      // Not playing, or starved: the output zeroed, 1.
      ZeroRest(base, converter, call);
      SpinRelease(base, thread, call);
      return 1;
    }
    if ((state & 2) != 0) {
      // Just started: the volume at once, the converter's ramp and kept
      // frames reset, bit 1 cleared.
      CopyFloat(base, codec + 224, codec + 112);
      CopyFloat(base, converter + 36, converter + 40);
      for (uint32_t k = 0; k < 6; k++) {
        guest::StoreU32(base, converter + 52 + 4 * k, 0);
      }
      CopyFloat(base, converter + 48, kFloatZero);
      ProcessSetFlags(base, codec_base, 2, 0, 0x8246DA7C, thread, frame, call);
    }
    if ((guest::LoadU8(base, codec + 172) & 4) != 0) {
      // Stopping: the volume halved a pass until the count runs out.
      const double volume = LoadSingle(base, codec + 224);
      const uint32_t count = (uint32_t(guest::LoadU16(base, codec + 174)) + 0xFFFF) & 0xFFFF;
      guest::StoreU16(base, codec + 174, uint16_t(count));
      if (count != 0) {
        const double f0 = double(float(volume * LoadSingle(base, kFadeStep)));
        guest::StoreFloat(base, codec + 224, float(f0));
      } else {
        CopyFloat(base, codec + 224, kFloatZero);
      }
    }
    SpinRelease(base, thread, call);
  }
  SetRegs(call, Args({6, 1}));
  AddStat(base, 6, 1);
  uint64_t entry = 0;
  for (;;) {
    uint64_t current = entry;
    if (uint32_t(entry) != 0 &&
        guest::LoadU32(base, converter + 4) == guest::LoadU32(base, converter + 8)) {
      // The packet's region played.
      const uint32_t loops = guest::LoadU32(base, uint32_t(entry) + 16);
      if (loops == 0) {
        const uint32_t target = Method(base, codec_base, 92);
        if (target == kCompletePacketFunction) {
          SetRegs(call, Args({codec_base, entry, 0}));
          CodecCompletePacket(base, codec_base, entry, 0, thread, Below(base, frame, 128), call);
        } else {
          call(target, 0x8246DBDC, At(frame, Args({codec_base, entry, 0})));
        }
        current = 0;
      } else {
        if (loops != 0xFFFFFFFFu) {
          guest::StoreU32(base, uint32_t(entry) + 16, loops - 1);
        }
        guest::StoreU32(base, codec + 120, guest::LoadU32(base, uint32_t(entry) + 20));
        const uint32_t callback = guest::LoadU32(base, codec + 180);
        if (callback != 0) {
          guest::StoreU32(base, frame + 88, 0);
          guest::StoreU32(base, frame + 80, guest::LoadU32(base, codec + 12));
          guest::StoreU32(base, frame + 84, guest::LoadU32(base, uint32_t(entry) + 36));
          call(callback, 0x8246D9E4, At(frame, Args({frame + 80})));
        }
      }
    }
    if (guest::LoadU32(base, converter + 24) == guest::LoadU32(base, converter + 28)) {
      entry = 0;
    } else if (uint32_t(current) != 0) {
      entry = current;
    } else {
      // The first queued packet, started on its first pass.
      SpinTake(base, thread, call);
      const uint64_t first = FirstEntry(base, codec + 20);
      if (uint32_t(first) != 0 && guest::LoadU8(base, uint32_t(first) + 117) == 0) {
        guest::StoreU32(base, codec + 120, guest::LoadU32(base, uint32_t(first) + 28));
        guest::StoreU8(base, uint32_t(first) + 117, 1);
      }
      SpinRelease(base, thread, call);
      entry = first;
    }
    if (uint32_t(entry) == 0) {
      break;
    }
    const uint32_t packet = uint32_t(entry);
    const uint32_t end = guest::LoadU32(base, packet + 16) != 0
                             ? guest::LoadU32(base, packet + 24)
                             : guest::LoadU32(base, packet + 32);
    const uint32_t data = guest::LoadU32(base, packet + 8);
    const uint32_t position = guest::LoadU32(base, codec + 120);
    guest::StoreU32(base, converter + 4, end);
    guest::StoreU32(base, converter + 0, data);
    guest::StoreU32(base, converter + 8, position < end ? position : end);
    SetRegs(call, Args({converter}));
    const uint64_t used = ResamplerProcess(base, converter, Below(base, frame, 128), call);
    SpinTake(base, thread, call);
    if (position == guest::LoadU32(base, codec + 120)) {
      guest::StoreU32(base, codec + 120, uint32_t(used + position));
    }
    guest::StoreU32(base, codec + 168, uint32_t(guest::LoadU32(base, codec + 168) + used));
    SpinRelease(base, thread, call);
  }
  // Out of packets, or the output full: the rest zeroed; out of packets
  // with nothing queued, the end of stream's stop or starved.
  SpinTake(base, thread, call);
  CopyFloat(base, converter + 36, converter + 40);
  const uint32_t written = guest::LoadU32(base, converter + 28);
  if (guest::LoadU32(base, converter + 24) - written != 0) {
    ZeroRest(base, converter, call);
    if ((guest::LoadU8(base, codec + 172) & 8) == 0) {
      const uint32_t head = codec + 20;
      const uint32_t next = guest::LoadU32(base, head);
      if (next == head || next - guest::LoadU32(base, head + 8) == 0) {
        ProcessSetFlags(base, codec_base, 16, 16, 0x8246DEC0, thread, frame, call);
      }
    } else {
      ProcessStop(base, codec, 0x8246DEDC, thread, frame, call);
    }
  }
  const uint8_t state = guest::LoadU8(base, codec + 172);
  if ((state & 4) != 0 &&
      (guest::LoadU16(base, codec + 174) == 0 || (state & 0x10) != 0)) {
    ProcessStop(base, codec, 0x8246DF1C, thread, frame, call);
  }
  SpinRelease(base, thread, call);
  return 0;
}

}  // namespace xaudio
