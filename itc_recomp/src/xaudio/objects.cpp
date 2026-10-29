// XAudio's factory objects as plain C++ (objects.h;
// docs/functions/xaudio-objects.md). Built with strict floating point and no
// contraction (CMakeLists.txt), so each rounding is the one written; the
// adapters turn the host's flush to zero off before float work, as the
// translated code does.

#include "xaudio/objects.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"

#include <rex/platform/fpscr.h>
#include <rex/ppc/intrinsics.h>
#include <simde/x86/sse4.1.h>

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

// A float word copied through a float register (lfs, then stfs).
void CopyFloat(uint8_t* base, uint32_t destination, uint32_t source) {
  volatile double value = guest::LoadFloat(base, source);
  guest::StoreFloat(base, destination, float(value));
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

}  // namespace xaudio
