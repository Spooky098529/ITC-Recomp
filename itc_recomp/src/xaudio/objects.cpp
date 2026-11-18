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
#include "helpers/clock.h"
#include "schema/xaudio_objects.h"
#include "xaudio/engine.h"
#include "xaudio/resampler.h"
#include "xaudio/reverb_core.h"

#include "base/float_mode.h"
#include "lifted/vector_helpers.h"
#include <simde/x86/sse4.1.h>

#include <bit>
#include <climits>
#include <cmath>
#include <cstring>
#include <utility>

namespace xaudio {

using schema::CodecBase;
using schema::CodecStateOut;
using schema::ConverterState;
using schema::FactoryInfoOut;
using schema::FactoryObjectOut;
using schema::FactorySizeOut;
using schema::I3dl2Reverb;
using schema::MatrixEntry;
using schema::MatrixLevel;
using schema::MatrixLevels;
using schema::MatrixVtable;
using schema::NativeReverbParams;
using schema::PacketEntry;
using schema::PacketList;
using schema::PacketLists;
using schema::PacketNode;
using schema::PcmCodec;
using schema::Reverb;
using schema::ReverbCore;
using schema::ReverbParameterData;
using schema::VolumeMeter;
using schema::XAudioCodec;
using schema::XAudioMatrix;
using schema::XAudioObject;
using schema::XAudioObjectDescriptor;
using schema::XAudioPacket;
using schema::XAudioRegionPacket;
using schema::XaudioObjectsGlobals;
using schema::XmaCodec;

namespace {

using Fpscr = ::base::FloatMode;

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
  FactorySizeOut(base, out).set_size(
      XAudioObjectDescriptor(base, descriptor).packet_count() * 120u + 272u);
  return 0;
}

uint64_t XmaCodecSize(uint8_t* base, uint32_t descriptor, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8246B458 XmaCodec::Size");
  const XAudioObjectDescriptor d(base, descriptor);
  const uint32_t packets = d.packet_count();
  const uint32_t streams = d.format_channels();
  FactorySizeOut(base, out).set_size(packets * 120u + streams * 88u + 284u);
  return 0;
}

uint64_t CodecSize(uint8_t* base, uint32_t descriptor, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82466B78 XAudioCodec::Size");
  const uint8_t type = XAudioObjectDescriptor(base, descriptor).format_type();
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
  const XAudioObjectDescriptor d(base, descriptor);
  const MatrixLevels matrix = d.matrix_default();
  uint32_t entries = d.matrix_entries();
  if (matrix) {
    // The larger of the two (the original keeps its own count only when
    // it's above the matrix's).
    const uint32_t count = matrix.count();
    if (entries <= count) {
      entries = count;
    }
  }
  FactorySizeOut(base, out).set_size(entries * 12u + 28u);
  return 0;
}

uint64_t ReverbSize(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82463B78 Reverb::Size");
  FactorySizeOut(base, out).set_size(Reverb::kSize);
  return 0;
}

uint64_t MeterSize(uint8_t* base, uint32_t descriptor, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_825A60E0 VolumeMeter::Size");
  const XAudioObjectDescriptor d(base, descriptor);
  const int32_t a = int32_t(d.meter_history_length());
  const int32_t b = int32_t(d.meter_channels());
  const uint32_t product = uint32_t(int64_t(a) * int64_t(b));
  FactorySizeOut(base, out).set_size(product * 12u + 52u);
  return 0;
}

// --- Small accessors ---

uint64_t CodecGetInfo(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82466E80 XAudioCodec::GetInfo");
  const FactoryInfoOut info(base, out);
  info.set_info_byte(4);
  info.set_info_halfword(XaudioObjectsGlobals::codec_info_halfword(base));
  return 0;
}

uint64_t MatrixGetInfo(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82465BE0 XAudioMatrix::GetInfo");
  const FactoryInfoOut info(base, out);
  info.set_info_byte(6);
  info.set_info_halfword(0);
  return 0;
}

uint64_t MeterGetInfo(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_825A5778 VolumeMeter::GetInfo");
  const FactoryInfoOut info(base, out);
  info.set_info_byte(1);
  info.set_info_halfword(1);
  return 0;
}

uint64_t CodecGetState(uint8_t* base, uint32_t codec, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8246A920 XAudioCodec::GetState");
  CodecStateOut(base, out).set_state(CodecBase(base, codec + 16).state());
  return 0;
}

uint64_t MatrixSetOverwrite(uint8_t* base, uint32_t matrix, uint32_t overwrite) {
  ITC_NATIVE_RUNNING("sub_82465C08 XAudioMatrix::SetOverwrite");
  XAudioMatrix(base, matrix).set_overwrite(uint8_t(overwrite));
  return 0;
}

void ReverbSetStereo(uint8_t* base, uint32_t core, uint32_t stereo) {
  ITC_NATIVE_RUNNING("sub_824679F8 ReverbCore::SetStereo");
  ReverbCore(base, core).set_stereo(stereo);
}

uint64_t ObjectRelease(uint8_t* base, uint32_t object, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246CE70 XAudioObject::Release");
  const uint32_t inner = object + 4;
  const XAudioObject o(base, object);
  const uint64_t count = uint64_t(o.ref_count()) - 1;
  o.set_ref_count(uint32_t(count));
  if (uint32_t(count) != 0) {
    return count;
  }
  call(Method(base, inner, 12), 0x8246CEA4, At(frame, Args({inner})));
  return 0;
}

// --- The codecs' construction ---

void PacketListsInit(uint8_t* base, uint32_t lists) {
  ITC_NATIVE_RUNNING("sub_8246E1E0 PacketLists::Init");
  const PacketLists l(base, lists);
  l.set_queued_next(lists);
  l.set_queued_prev(lists);
  l.set_queued_node_offset(0);
  l.set_free_node_offset(0);
  l.set_free_next(l.free_next_address());
  l.set_free_prev(l.free_next_address());
}

uint64_t CodecBaseConstruct(uint8_t* base, uint64_t codec_base) {
  ITC_NATIVE_RUNNING("sub_82469920 CodecBase::CodecBase");
  const CodecBase b(base, uint32_t(codec_base));
  b.set_vtable(kCodecBaseVtable);
  PacketListsInit(base, b.queued_next_address());
  return codec_base;
}

uint64_t CodecConstruct(uint8_t* base, uint64_t inner, uint32_t descriptor) {
  ITC_NATIVE_RUNNING("sub_82466BC8 XAudioCodec::Construct");
  const XAudioCodec c(base, uint32_t(inner));
  c.set_vtable(kObjectBaseVtable);
  c.set_ref_count(1);
  c.set_owner(XAudioObjectDescriptor(base, descriptor).owner());
  CodecBaseConstruct(base, inner + 12);
  c.set_vtable(kCodecInnerVtable);
  c.set_base_vtable(kCodecInnerBaseVtable);
  return inner;
}

uint64_t PacketListsAlloc(uint8_t* base, uint64_t lists, uint32_t count, uint32_t allocator,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246E120 PacketLists::Alloc");
  const uint32_t n = count & 0xFF;
  if (n == 0) {
    return lists;
  }
  const PacketLists l(base, uint32_t(lists));
  const uint64_t entries =
      Allocate(base, allocator, uint64_t(n) * PacketEntry::kSize, 0x8246E15C, frame, call);
  l.set_entries(uint32_t(entries));
  const uint32_t free_list = l.free_next_address();
  for (uint32_t i = 0, offset = 0; i < n; i++, offset += PacketEntry::kSize) {
    PacketNode node(base, l.entries().address + offset);
    node.set_next(node.address);
    node = PacketNode(base, l.entries().address + offset);
    node.set_prev(node.next());
    // The node at the list's offset (+8) into the entry, at the free list's
    // end.
    node = PacketNode(base, offset + l.free_node_offset() + l.entries().address);
    node.set_next(free_list);
    node.set_prev(l.free_prev());
    l.set_free_prev(node.address);
    PacketNode(base, node.prev()).set_next(node.address);
  }
  return entries;
}

uint64_t CodecBaseInit(uint8_t* base, uint32_t codec_base, uint32_t format, uint32_t count,
                       uint32_t allocator, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82469978 CodecBase::Init");
  const CodecBase b(base, codec_base);
  Copy(base, b.format_address(), format, 56, 0x8246999C, frame, call);
  CopyFloat(base, b.volume_address(), 0x82000A30);
  CopyFloat(base, b.frequency_ratio_address(), 0x82000A30);
  b.set_state(32);
  b.set_block_size(1);
  b.set_block_size_2(1);
  b.set_frames_played(0);
  SetRegs(call, Args({b.queued_next_address(), count, allocator}));
  return PacketListsAlloc(base, b.queued_next_address(), count, allocator,
                          Below(base, frame, 112), call);
}

uint64_t CodecInit(uint8_t* base, uint32_t inner, uint32_t descriptor, uint32_t allocator,
                   uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82466960 XAudioCodec::Init");
  const XAudioCodec c(base, inner);
  const XAudioObjectDescriptor d(base, descriptor);
  c.set_packet_done_callback(d.packet_done_callback());
  c.set_loop_end_callback(d.loop_end_callback());
  const uint32_t extra = d.converter_flags();
  const uint32_t count = d.packet_count();
  SetRegs(call, Args({c.base_vtable_address(), d.format_type_address(), count, extra, allocator}));
  return CodecBaseInit(base, c.base_vtable_address(), d.format_type_address(), count, allocator,
                       Below(base, sp, 112), call);
}

uint64_t PcmCodecConstruct(uint8_t* base, uint64_t object, uint32_t descriptor,
                           uint32_t allocator, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D750 PcmCodec::PcmCodec");
  const PcmCodec c(base, uint32_t(object));
  CodecConstruct(base, object + 4, descriptor);
  const ConverterState converter(base, c.converter_input_address());
  c.set_vtable(kPcmCodecVtable);
  c.set_inner_vtable(kPcmCodecInnerVtable);
  c.set_base_vtable(kPcmCodecBaseVtable);
  Clear(base, converter.address, 76);
  converter.set_kernel(0);
  SetRegs(call, Args({c.address + 4, descriptor, allocator}));
  CodecInit(base, c.address + 4, descriptor, allocator, frame, call);
  const uint32_t rate = c.sample_rate();
  const double ratio = double(c.frequency_ratio());
  const uint8_t extra = XAudioObjectDescriptor(base, descriptor).converter_flags();
  const uint8_t channels = c.channels();
  const uint8_t type = c.format_type();
  // The rate's word goes through the frame (std, lfd, fcfid).
  guest::StoreU32(base, frame + 80, 0);
  guest::StoreU32(base, frame + 84, rate);
  converter.set_flags(extra);
  uint32_t changed = converter.changed();
  converter.set_format(type);
  changed |= 1;
  converter.set_channels(channels);
  converter.set_changed(changed);
  changed |= 2;
  const double rate_float = double(float(double(int64_t(uint64_t(rate)))));
  const double product = double(float(rate_float * ratio));
  const int64_t fixed = std::isnan(product) ? INT64_MIN
                        : product > double(LLONG_MAX)
                            ? LLONG_MAX
                            : simde_mm_cvttsd_si64(simde_mm_load_sd(&product));
  converter.set_input_rate(uint32_t(fixed));
  converter.set_changed(changed);
  converter.set_output_rate(kSampleRate);
  const uint32_t table_type = converter.format();
  const uint32_t table_channels = converter.channels();
  const uint32_t bytes =
      guest::LoadU32(base, XaudioObjectsGlobals::sample_bytes_address() + table_type * 4);
  c.set_block_size_2(1);
  c.set_block_size(uint16_t(int64_t(int32_t(bytes)) * int64_t(int32_t(table_channels))));
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
      reinterpret_cast<const simde__m128i*>(guest::Pointer(base, address & ~0xFu)));
  const simde__m128i mask = simde_mm_load_si128(
      reinterpret_cast<const simde__m128i*>(&vector_helpers::VectorMaskL[(address & 0xF) * 16]));
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
  const simde__m128i fixed = vector_helpers::simde_mm_vctsxs(scaled);
  Fpscr::setcsr(csr);
  v0 = vector_helpers::simde_mm_adds_epu32(fixed, v0);
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
  const XmaCodec c(base, uint32_t(object));
  const XAudioObjectDescriptor d(base, descriptor);
  CodecConstruct(base, object + 4, descriptor);
  c.set_xma_object(0);
  c.set_vtable(kXmaCodecVtable);
  c.set_inner_vtable(kXmaCodecInnerVtable);
  c.set_base_vtable(kXmaCodecBaseVtable);
  const uint8_t extra = d.converter_flags();
  c.set_submit_mode(0);
  c.set_converter_flags(extra);
  c.set_unknown_272(0);
  c.set_unknown_276(0);
  c.set_unknown_280(0);
  c.set_unknown_268(0);
  SetRegs(call, Args({c.address + 4, descriptor, allocator}));
  CodecInit(base, c.address + 4, descriptor, allocator, frame, call);
  const uint32_t streams = c.channels();
  // never above 0x2E8BA2E streams: a byte
  const uint64_t bytes = uint64_t(streams) * ConverterState::kSize;
  const uint64_t array = Allocate(base, allocator, bytes, 0x8246C27C, frame, call);
  uint32_t first = 0;
  if (uint32_t(array) != 0) {
    for (uint32_t i = 0, entry = uint32_t(array); i < streams; i++, entry += ConverterState::kSize) {
      SetRegs(call, Args({entry, 0, 76}));
      Clear(base, entry, 76);
      ConverterState(base, entry).set_kernel(0);
    }
    first = uint32_t(array);
  }
  c.set_streams(first);
  c.set_xma_block_size(XmaBlockSize(base, d.xma_block_scale_address()));
  Copy(base, c.format_copy_address(), c.format_address(), 56, 0x8246C308, frame, call);
  c.set_block_size(1);
  c.set_block_size_2(1);
  return object;
}

uint64_t PcmCodecCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                        uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246DF78 PcmCodec::Create");
  const uint64_t object = Allocate(base, allocator, PcmCodec::kSize, 0x8246DFA8, frame, call);
  if (uint32_t(object) == 0) {
    return kEOutOfMemory;
  }
  SetRegs(call, Args({object, descriptor, allocator}));
  const uint64_t made =
      PcmCodecConstruct(base, object, descriptor, allocator, Below(base, frame, 144), call);
  if (uint32_t(made) == 0) {
    return kEOutOfMemory;
  }
  FactoryObjectOut(base, out).set_object(uint32_t(made));
  return 0;
}

uint64_t XmaCodecCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                        uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246CDB0 XmaCodec::Create");
  const uint64_t object = Allocate(base, allocator, XmaCodec::kSize, 0x8246CDE0, frame, call);
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
    FactoryObjectOut(base, out).set_object(codec);
    return r3;
  }
  call(Method(base, codec + 4, 12), 0x8246CE50, At(frame, Args({made + 4})));
  return r3;
}

uint64_t CodecCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                     uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82466BA0 XAudioCodec::Create");
  const uint8_t type = XAudioObjectDescriptor(base, descriptor).format_type();
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
  const PacketList head(base, uint32_t(list));
  const PacketNode node(base, uint32_t(entry) != 0 ? uint32_t(head.node_offset() + entry)
                                                   : head.address);
  const uint32_t next = int32_t(direction) != 0 ? node.next() : node.prev();
  if (head.address == next) {
    return 0;
  }
  return uint64_t(next) - uint64_t(head.node_offset());
}

uint64_t PacketListClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8246E060 PacketList::Clear");
  const PacketList head(base, list);
  uint64_t entry = PacketListNext(base, list, 0, 1);
  while (uint32_t(entry) != 0) {
    const PacketNode node(base, uint32_t(head.node_offset() + entry));
    const uint32_t next = node.next();
    if (next != node.address) {
      PacketNode(base, next).set_prev(node.prev());
      PacketNode(base, node.prev()).set_next(node.next());
      node.set_prev(node.address);
      node.set_next(node.address);
    }
    entry = PacketListNext(base, list, 0, 1);
  }
  return entry;
}

uint64_t PacketListsClear(uint8_t* base, uint32_t lists) {
  ITC_NATIVE_RUNNING("sub_8246E0E8 PacketLists::Clear");
  const PacketLists l(base, lists);
  PacketListClear(base, l.free_next_address());
  return PacketListClear(base, l.queued_next_address());
}

uint64_t CodecBaseDestruct(uint8_t* base, uint32_t codec_base) {
  ITC_NATIVE_RUNNING("sub_82469960 CodecBase::~CodecBase");
  const CodecBase b(base, codec_base);
  b.set_vtable(kCodecBaseVtable);
  return PacketListsClear(base, b.queued_next_address());
}

uint64_t CodecDestruct(uint8_t* base, uint32_t inner) {
  ITC_NATIVE_RUNNING("sub_82466EE8 XAudioCodec::Destruct");
  const XAudioCodec c(base, inner);
  c.set_vtable(kCodecInnerVtable);
  c.set_base_vtable(kCodecInnerBaseVtable);
  const uint64_t r3 = CodecBaseDestruct(base, c.base_vtable_address());
  c.set_vtable(kRootVtable);
  return r3;
}

uint64_t PcmCodecDestroy(uint8_t* base, uint64_t object) {
  ITC_NATIVE_RUNNING("sub_8246CF50 PcmCodec::~PcmCodec");
  const PcmCodec c(base, uint32_t(object));
  c.set_vtable(kPcmCodecVtable);
  c.set_inner_vtable(kPcmCodecInnerVtable);
  c.set_base_vtable(kPcmCodecBaseVtable);
  CodecDestruct(base, c.inner_vtable_address());
  return object;
}

uint64_t XmaCodecDestroy(uint8_t* base, uint64_t object, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246AAD0 XmaCodec::~XmaCodec");
  call(kXmaRelease, 0x8246AAE8, At(frame, Args({object})));
  return object;
}

void MatrixDestruct(uint8_t* base, uint32_t matrix) {
  ITC_NATIVE_RUNNING("sub_82466418 XAudioMatrix::Destruct");
  const XAudioMatrix m(base, matrix);
  m.set_vtable(kMatrixVtable);
  m.set_inner_vtable(kMatrixInnerVtable);
  m.set_inner_vtable(kRootVtable);
}

// --- The matrix and the meter's construction ---

uint64_t MatrixCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                      uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82466888 XAudioMatrix::Create");
  const uint64_t object = Allocate(base, allocator, XAudioMatrix::kSize, 0x824668B8, frame, call);
  const uint32_t at = uint32_t(object);
  if (at == 0) {
    return kEOutOfMemory;
  }
  const XAudioMatrix m(base, at);
  const uint32_t inner = m.inner_vtable_address();
  m.set_inner_vtable(kObjectBaseVtable);
  m.set_ref_count(1);
  m.set_owner(XAudioObjectDescriptor(base, descriptor).owner());
  m.set_vtable(kMatrixVtable);
  m.set_inner_vtable(kMatrixInnerVtable);
  call(MatrixVtable(base, kMatrixVtable).init(), 0x82466914,
       At(frame, Args({object, descriptor, allocator})));
  const uint64_t r3 = LastCallR3();
  if (int32_t(r3) >= 0) {
    FactoryObjectOut(base, out).set_object(at);
    return r3;
  }
  call(Method(base, inner, 12), 0x82466950, At(frame, Args({uint64_t(inner)})));
  return r3;
}

uint64_t MeterCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                     uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825A6460 VolumeMeter::Create");
  const uint64_t object = Allocate(base, allocator, VolumeMeter::kSize, 0x825A6490, frame, call);
  uint64_t meter = 0;
  if (uint32_t(object) != 0) {
    const VolumeMeter m(base, uint32_t(object));
    meter = object;
    m.set_ref_count(1);
    m.set_vtable(kMeterVtable);
    m.set_allocator(0);
    m.set_history_slot(0);
    m.set_threshold_sums(0);
    m.set_filled(0);
    m.set_class_id(0);
    m.set_owner(0);
    m.set_channels(0);
    m.set_history_length(0);
  }
  const uint32_t at = uint32_t(meter);
  call(Method(base, at, 28), 0x825A64F8, At(frame, Args({meter, descriptor, allocator})));
  FactoryObjectOut(base, out).set_object(at);
  return 0;
}

uint64_t MeterInit(uint8_t* base, uint32_t meter, uint32_t descriptor, uint32_t allocator,
                   uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825A6138 VolumeMeter::Init");
  const VolumeMeter m(base, meter);
  const XAudioObjectDescriptor d(base, descriptor);
  m.set_class_id(d.class_id());
  m.set_owner(d.owner());
  m.set_channels(d.meter_channels());
  m.set_history_length(d.meter_history_length());
  m.set_allocator(allocator);
  const int32_t a = int32_t(d.meter_channels());
  const int32_t b = int32_t(d.meter_history_length());
  const uint32_t bytes = uint32_t(int64_t(b) * int64_t(a)) << 2;
  const uint32_t arrays[] = {m.threshold_sums_address(), m.peaks_address(), m.rms_address()};
  static constexpr uint32_t kReturns[] = {0x825A61A0, 0x825A61C8, 0x825A61F0};
  for (int i = 0; i < 3; i++) {
    const uint64_t array = Allocate(base, allocator, bytes, kReturns[i], frame, call);
    guest::StoreU32(base, arrays[i], uint32_t(array));
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
  const PacketList head(base, list);
  const uint32_t next = head.next().address;
  if (next == list) {
    return 0;
  }
  return uint64_t(next) - uint64_t(head.node_offset());
}

// The node of `entry` (the list's offset into it) out of its list when it's
// in one, left pointing at itself.
void UnlinkEntry(uint8_t* base, uint32_t list, uint64_t entry) {
  const PacketNode node(base, uint32_t(PacketList(base, list).node_offset() + entry));
  const uint32_t next = node.next();
  if (next == node.address) {
    return;
  }
  PacketNode(base, next).set_prev(node.prev());
  PacketNode(base, node.prev()).set_next(node.next());
  node.set_prev(node.address);
  node.set_next(node.address);
}

// The node of `entry` added at the end of the list at `list`.
void LinkEntryAtEnd(uint8_t* base, uint32_t list, uint64_t entry) {
  const PacketList head(base, list);
  const PacketNode node(base, uint32_t(head.node_offset() + entry));
  node.set_next(list);
  node.set_prev(head.prev());
  head.set_prev(node.address);
  PacketNode(base, node.prev()).set_next(node.address);
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
  ConverterState(base, converter).set_input_rate(uint32_t(ToFixed(product)));
}

double TwoToThe(uint8_t* base, double octaves) {
  return c_runtime::Pow(std::bit_cast<double>(uint64_t(guest::LoadU64(base, 0x820056D8))), octaves);
}

}  // namespace

uint64_t CodecSetFlags(uint8_t* base, uint32_t codec_base, uint32_t mask, uint32_t bits,
                       uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82467078 CodecBase::SetFlags");
  const CodecBase b(base, codec_base);
  SpinTake(base, thread, call);
  const uint8_t state = b.state();
  b.set_state(uint8_t((state & ~(mask & 0xFF)) | (bits & 0xFF)));
  SpinRelease(base, thread, call);
  return LastCallR3();
}

uint64_t CodecRetirePacket(uint8_t* base, uint32_t codec_base, uint64_t entry, uint32_t thread,
                           uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246A430 CodecBase::RetirePacket");
  const CodecBase b(base, codec_base);
  SpinTake(base, thread, call);
  if (uint32_t(entry) == 0) {
    entry = FirstEntry(base, b.queued_next_address());
    if (uint32_t(entry) == 0) {
      SpinRelease(base, thread, call);
      return 0;
    }
  }
  UnlinkEntry(base, b.queued_next_address(), entry);
  LinkEntryAtEnd(base, b.free_next_address(), entry);
  SetRegs(call, Args({b.packet_state_address(), 0, 48}));
  Clear(base, b.packet_state_address(), 48);
  PacketEntry(base, uint32_t(entry)).set_retired(0);
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
  const CodecBase b(base, codec_base);
  const uint32_t callback = b.packet_done_callback();
  if (uint32_t(done) != 0 && callback != 0) {
    guest::StoreU32(base, frame + 88, uint32_t(result));
    guest::StoreU32(base, frame + 80, b.codec_owner());
    guest::StoreU32(base, frame + 84, PacketEntry(base, uint32_t(done)).context());
    call(callback, 0x82467528, At(frame, Args({frame + 80})));
  }
  return done;
}

uint64_t CodecFlushPackets(uint8_t* base, uint32_t codec_base, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824699E0 CodecBase::FlushPackets");
  const CodecBase b(base, codec_base);
  if ((b.state() & 1) != 0) {
    return 0xFFFFFFFF8000FFFFull;  // E_UNEXPECTED
  }
  while (call(Method(base, codec_base, 92), 0x82469A40,
              At(frame, Args({codec_base, 0, 0xFFFFFFFF80004004ull}))) != 0) {
  }
  SetFlagsCall(base, codec_base, 24, 0, 0x82469A64, frame, call);
  b.set_frames_played(0);
  return 0;
}

uint64_t CodecBaseSetFormat(uint8_t* base, uint32_t codec_base, uint32_t format, uint32_t extra,
                            uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82469E58 CodecBase::SetFormat");
  const CodecBase b(base, codec_base);
  if ((b.state() & 1) != 0 || uint32_t(FirstEntry(base, b.queued_next_address())) != 0) {
    return 0xFFFFFFFF8000FFFFull;  // E_UNEXPECTED
  }
  Copy(base, b.format_address(), format, 56, 0x82469EB0, frame, call);
  b.set_format_extra(uint8_t(extra));
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
  const PcmCodec c(base, codec);
  const uint32_t type = c.converter_format();
  const uint32_t channels = c.converter_channels();
  const uint32_t bytes =
      guest::LoadU32(base, XaudioObjectsGlobals::sample_bytes_address() + type * 4);
  c.set_block_size_2(1);
  c.set_block_size(uint16_t(int64_t(int32_t(bytes)) * int64_t(int32_t(channels))));
  return r3;
}

uint64_t PcmCodecStart(uint8_t* base, uint32_t codec, uint32_t thread, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D410 PcmCodec::Start");
  const uint32_t codec_base = codec + 16;
  SpinTake(base, thread, call);
  const uint8_t state = CodecBase(base, codec_base).state();
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
  const CodecBase b(base, codec_base);
  SpinTake(base, thread, call);
  const uint8_t state = b.state();
  if ((state & 1) != 0) {
    if ((flags & 1) != 0 || (state & 18) != 0) {
      SetFlagsCall(base, codec_base, 87, 0, 0x8246D5D8, frame, call);
    } else if ((state & 4) == 0) {
      SetFlagsCall(base, codec_base, 4, 4, 0x8246D5AC, frame, call);
      b.set_fade_passes(XaudioObjectsGlobals::codec_info_halfword(base));
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
  if (uint32_t(FirstEntry(base, CodecBase(base, codec_base).queued_next_address())) != 0) {
    SetFlagsCall(base, codec_base, 8, 8, 0x8246D6C8, frame, call);
  }
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t PcmCodecSetVolume(uint8_t* base, uint32_t codec, double volume, uint32_t thread,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D008 PcmCodec::SetVolume");
  const PcmCodec c(base, codec);
  SpinTake(base, thread, call);
  c.set_volume(float(volume));
  c.set_converter_target_volume(float(volume));
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t PcmCodecSetPitch(uint8_t* base, uint32_t codec, double octaves, uint32_t thread,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D0C8 PcmCodec::SetPitch");
  const PcmCodec c(base, codec);
  SpinTake(base, thread, call);
  const double ratio = double(float(TwoToThe(base, octaves)));
  const uint32_t rate = c.sample_rate();
  const ConverterState converter(base, c.converter_input_address());
  c.set_frequency_ratio(float(ratio));
  const uint8_t channels = c.channels();
  const uint8_t type = c.format_type();
  const uint32_t changed = converter.changed();
  converter.set_channels(channels);
  converter.set_format(type);
  converter.set_changed(changed | 1);
  SetConverterRate(base, converter.address, frame, rate, ratio);
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t PcmCodecSetFrequencyRatio(uint8_t* base, uint32_t codec, double ratio, uint32_t thread,
                                   uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246D1E0 PcmCodec::SetFrequencyRatio");
  const PcmCodec c(base, codec);
  SpinTake(base, thread, call);
  const ConverterState converter(base, c.converter_input_address());
  const uint32_t rate = c.sample_rate();
  c.set_frequency_ratio(float(ratio));
  const uint8_t type = c.format_type();
  const uint8_t channels = c.channels();
  const uint32_t changed = converter.changed();
  converter.set_format(type);
  converter.set_channels(channels);
  converter.set_changed(changed | 1);
  SetConverterRate(base, converter.address, frame, rate, ratio);
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t XmaCodecSetVolume(uint8_t* base, uint32_t codec, double volume, uint32_t thread,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246AB48 XmaCodec::SetVolume");
  const XmaCodec c(base, codec);
  SpinTake(base, thread, call);
  c.set_volume(float(volume));
  for (uint32_t i = 0; i < c.channels(); i = (i + 1) & 0xFF) {
    CopyFloat(base,
              ConverterState(base, c.streams().address + ConverterState::kSize * i)
                  .target_volume_address(),
              c.volume_address());
  }
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t XmaCodecSetPitch(uint8_t* base, uint32_t codec, double octaves, uint32_t thread,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8246AC40 XmaCodec::SetPitch");
  const XmaCodec c(base, codec);
  SpinTake(base, thread, call);
  c.set_frequency_ratio(float(double(float(TwoToThe(base, octaves)))));
  for (uint32_t i = 0; i < c.channels(); i = (i + 1) & 0xFF) {
    const double ratio = double(c.frequency_ratio());
    const ConverterState converter(base, c.streams().address + ConverterState::kSize * i);
    // The format's streams, 8 bytes each: the rate, then the channels.
    const uint32_t rate = guest::LoadU32(base, c.sample_rate_address() + 8 * i);
    const uint32_t changed = converter.changed();
    const uint8_t channels = guest::LoadU8(base, c.stream_channels_address() + 8 * i);
    converter.set_format(1);
    converter.set_channels(channels);
    converter.set_changed(changed | 1);
    SetConverterRate(base, converter.address, frame, rate, ratio);
  }
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t CodecOnSubmit(uint8_t* base, uint32_t codec_base, uint32_t entry, uint32_t flags,
                       uint32_t thread, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82469AF8 CodecBase::OnSubmit");
  const uint32_t mode = flags & 0xFF;
  const PacketEntry e(base, entry);
  // Each failed check releases one level of the spin lock (an assertion's
  // path, probably) and goes on.
  const auto fail = [&]() { SpinRelease(base, thread, call); };
  if ((mode & 0x10) == 0) {
    if (e.play_end() == 0) {
      e.set_play_end(e.length());
    }
    if (e.loop_count() != 0) {
      if (e.loop_start() >= e.length()) {
        fail();
      }
      if (e.loop_end() > e.length()) {
        fail();
      }
    }
    if (e.play_begin() >= e.length()) {
      fail();
    }
    if (e.play_end() > e.length()) {
      fail();
    }
  }
  if (e.loop_count() != 0 && (mode & 0x20) == 0) {
    if (e.loop_end() <= e.loop_start()) {
      fail();
    }
    if (e.play_begin() >= e.loop_end()) {
      fail();
    }
  }
  if (e.play_end() != 0) {
    const uint32_t limit = e.loop_count() != 0 ? e.loop_start() : e.play_begin();
    if (e.play_end() <= limit) {
      fail();
    }
  }
  SetFlagsCall(base, codec_base, 24, (mode & 1) != 0 ? 8 : 0, 0x82469E10, frame, call);
  if (uint32_t(FirstEntry(base, CodecBase(base, codec_base).free_next_address())) == 0) {
    SetFlagsCall(base, codec_base, 32, 0, 0x82469E4C, frame, call);
  }
  return 0;
}

namespace {

// The first free entry (+16) moved to the end of the queued list (+4) and
// given the next sequence number (+32, never 0), +117 to +119 cleared; 0
// when there's none.
uint32_t TakeFreeEntry(uint8_t* base, uint32_t codec_base) {
  const CodecBase b(base, codec_base);
  const uint32_t entry = uint32_t(FirstEntry(base, b.free_next_address()));
  if (entry == 0) {
    return 0;
  }
  const PacketEntry e(base, entry);
  UnlinkEntry(base, b.free_next_address(), entry);
  LinkEntryAtEnd(base, b.queued_next_address(), entry);
  uint32_t sequence = b.next_sequence() + 1;
  b.set_next_sequence(sequence);
  e.set_sequence(sequence);
  if (sequence == 0) {
    sequence = b.next_sequence() + 1;
    b.set_next_sequence(sequence);
    e.set_sequence(sequence);
  }
  e.set_begun(0);
  e.set_unknown_118(0);
  e.set_unknown_119(0);
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
  const CodecBase b(base, codec_base);
  if (b.block_size() == 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82469EF8 CodecBase::SubmitPacket");
  const XAudioPacket p(base, packet);
  SpinTake(base, thread, call);
  const uint32_t entry = TakeFreeEntry(base, codec_base);
  if (entry == 0) {
    return NoFreeEntry(base, thread, call);
  }
  const PacketEntry e(base, entry);
  e.set_data(p.data());
  const uint32_t block = b.block_size();
  e.set_length(p.size() / block);
  e.set_loop_count(p.loop_count());
  if ((flags & 0x20) != 0) {
    e.set_loop_start(0);
    e.set_loop_end(0);
    Copy(base, e.xma_region_address(), p.xma_region_address(), 72, 0x8246A054, frame, call);
  } else {
    e.set_loop_start(p.loop_start() / b.block_size());
    const uint32_t length = p.loop_length();
    if (length != 0) {
      const uint32_t blocks = length / b.block_size();
      e.set_loop_end(blocks + e.loop_start());
    } else {
      e.set_loop_end(e.length());
    }
  }
  e.set_play_begin(0);
  e.set_play_end(0);
  e.set_context(p.context());
  SetRegs(call, Args({codec_base, entry, flags}));
  const uint64_t r3 =
      CodecOnSubmit(base, codec_base, entry, flags, thread, Below(base, frame, 128), call);
  SpinRelease(base, thread, call);
  return r3;
}

std::optional<uint64_t> CodecSubmitRegion(uint8_t* base, uint32_t codec_base, uint32_t region,
                                          uint32_t flags, uint32_t thread, uint32_t frame,
                                          const GuestCall& call) {
  const CodecBase b(base, codec_base);
  if (b.block_size() == 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8246A1C0 CodecBase::SubmitRegion");
  const XAudioRegionPacket r(base, region);
  SpinTake(base, thread, call);
  const uint32_t entry = TakeFreeEntry(base, codec_base);
  if (entry == 0) {
    return NoFreeEntry(base, thread, call);
  }
  const PacketEntry e(base, entry);
  e.set_data(r.data());
  const uint32_t block = b.block_size();
  e.set_length(r.size() / block);
  e.set_loop_count(r.loop_count());
  e.set_loop_start(r.loop_start());
  e.set_loop_end(r.loop_end());
  e.set_play_begin(r.play_begin());
  e.set_play_end(r.play_end());
  e.set_context(r.context());
  SetRegs(call, Args({codec_base, entry, flags}));
  const uint64_t r3 =
      CodecOnSubmit(base, codec_base, entry, flags, thread, Below(base, frame, 128), call);
  SpinRelease(base, thread, call);
  return r3;
}

std::optional<uint64_t> XmaCodecSubmitRegion(uint8_t* base, uint32_t codec, uint32_t region,
                                             uint32_t flags, uint32_t thread, uint32_t frame,
                                             const GuestCall& call) {
  const XmaCodec c(base, codec);
  if (c.submit_mode() == 1) {
    ITC_NATIVE_RUNNING("sub_8246A860 XmaCodec::SubmitRegion");
    return kENotImpl;
  }
  if (c.block_size() == 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8246A860 XmaCodec::SubmitRegion");
  c.set_submit_mode(2);
  const uint32_t loops = XAudioRegionPacket(base, region).loop_count();
  uint32_t from = region;
  if (loops > 255 && int32_t(loops) != -1) {
    for (uint32_t i = 0; i < 32; i += 4) {
      guest::StoreU32(base, frame + 80 + i, guest::LoadU32(base, region + i));
    }
    guest::StoreU32(base, frame + 88, 0xFFFFFFFF);
    from = frame + 80;
  }
  const uint32_t mode = (flags & 0xFF) | 16;
  SetRegs(call, Args({c.base_vtable_address(), from, mode}));
  return CodecSubmitRegion(base, c.base_vtable_address(), from, mode, thread,
                           Below(base, frame, 144), call);
}

// --- The output matrix ---

namespace {

using Vec = simde__m128i;

// lvx and stvx (aligned down, the words byte-swapped and the lanes in the
// translated code's order) and lvlx, as the translated code does them.
Vec LoadAligned(uint8_t* base, uint32_t address) {
  return simde_mm_shuffle_epi8(
      simde_mm_load_si128(reinterpret_cast<const Vec*>(guest::Pointer(base, address & ~0xFu))),
      simde_mm_load_si128(reinterpret_cast<const Vec*>(vector_helpers::VectorMaskL)));
}
Vec LoadLeft(uint8_t* base, uint32_t address) {
  return simde_mm_shuffle_epi8(
      simde_mm_load_si128(reinterpret_cast<const Vec*>(guest::Pointer(base, address & ~0xFu))),
      simde_mm_load_si128(
          reinterpret_cast<const Vec*>(&vector_helpers::VectorMaskL[(address & 0xF) * 16])));
}
void StoreAligned(uint8_t* base, uint32_t address, Vec value) {
  simde_mm_store_si128(
      reinterpret_cast<Vec*>(guest::Pointer(base, address & ~0xFu)),
      simde_mm_shuffle_epi8(value,
                            simde_mm_load_si128(reinterpret_cast<const Vec*>(vector_helpers::VectorMaskL))));
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
  const XAudioMatrix m(base, matrix);
  const XAudioObjectDescriptor d(base, descriptor);
  const MatrixLevels levels = d.matrix_default();
  uint32_t entries = d.matrix_entries();
  if (levels) {
    const uint32_t count = levels.count();
    if (entries <= count) {
      entries = count;
    }
  }
  m.set_entries_allowed(uint8_t(entries));
  if (entries != 0) {
    const uint64_t array =
        Allocate(base, allocator, entries * MatrixEntry::kSize, 0x82465CA4, frame, call);
    m.set_entries(uint32_t(array));
  }
  call(Method(base, matrix, 40), 0x82465CC0,
       At(frame, Args({matrix, d.matrix_default().address})));
  if (int32_t(LastCallR3()) < 0) {
    return LastCallR3();
  }
  call(Method(base, matrix, 32), 0x82465CE0, At(frame, Args({matrix, d.matrix_overwrite()})));
  return LastCallR3();
}

uint64_t MatrixSetMatrix(uint8_t* base, uint32_t matrix, uint32_t levels, uint32_t thread,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82465E50 XAudioMatrix::SetMatrix");
  const XAudioMatrix m(base, matrix);
  const MatrixLevels given(base, levels);
  // The i-th level given and the i-th entry, each through its pointer as
  // the original reloads it.
  const auto level = [&](uint32_t i) {
    return MatrixLevel(base, given.levels().address + MatrixLevel::kSize * i);
  };
  const auto entry = [&](uint32_t i) {
    return MatrixEntry(base, m.entries().address + MatrixEntry::kSize * i);
  };
  SpinTake(base, thread, call);
  if (levels == 0) {
    m.set_entries_set(0);
    m.set_output_channels(0);
    SpinRelease(base, thread, call);
    return 0;
  }
  const uint8_t count = given.count();
  if (count > m.entries_allowed()) {
    SpinRelease(base, thread, call);
    return 0xFFFFFFFF80070057ull;  // E_INVALIDARG
  }
  m.set_entries_set(count);
  if (count != 0) {
    for (uint32_t i = 0; i < m.entries_set(); i++) {
      const MatrixLevel in = level(i);
      const MatrixEntry out = entry(i);
      out.set_input_channel(in.input_channel());
      entry(i).set_output_channel(level(i).output_channel());
      CopyFloat(base, entry(i).level_address(), level(i).level_address());
      CopyFloat(base, entry(i).target_level_address(), level(i).level_address());
    }
  }
  uint32_t highest = 0;
  const uint32_t set = m.entries_set();
  if (set != 0) {
    const uint32_t entries = m.entries().address;
    for (uint32_t i = 0; i < set; i++) {
      const uint32_t channel =
          MatrixEntry(base, entries + MatrixEntry::kSize * i).output_channel() + 1u;
      if (channel > highest) {
        highest = channel;
      }
    }
  }
  m.set_output_channels(uint8_t(highest));
  SpinRelease(base, thread, call);
  return 0;
}

uint64_t MatrixSetLevels(uint8_t* base, uint32_t matrix, uint32_t levels, uint32_t now,
                         uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82466188 XAudioMatrix::SetLevels");
  const XAudioMatrix m(base, matrix);
  const MatrixLevels given(base, levels);
  SpinTake(base, thread, call);
  if (given.count() != 0) {
    for (uint32_t i = 0; i < given.count(); i = (i + 1) & 0xFF) {
      const uint32_t index =
          MatrixLevel(base, given.levels().address + MatrixLevel::kSize * i).input_channel();
      if (index >= m.entries_set()) {
        SpinRelease(base, thread, call);
        return 0xFFFFFFFF80070057ull;  // E_INVALIDARG
      }
      const MatrixLevel level(base, given.levels().address + MatrixLevel::kSize * i);
      const MatrixEntry entry(base,
                              m.entries().address + MatrixEntry::kSize * level.input_channel());
      CopyFloat(base, entry.target_level_address(), level.level_address());
      if ((now & 0xFF) != 0) {
        CopyFloat(base, entry.level_address(), level.level_address());
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
  const XAudioMatrix m(base, matrix);
  const uint32_t source_format = frame + 112, target_format = frame + 96;
  SetRegs(call, Args({source != 0 ? source - 8 : 0, source_format}));
  BufferGetFormat(base, source != 0 ? source - 8 : 0, source_format);
  const uint32_t target_buffer = target != 0 ? target - 8 : 0;
  SetRegs(call, Args({target_buffer, target_format}));
  BufferGetFormat(base, target_buffer, target_format);
  if (m.overwrite() == 1) {
    guest::StoreU8(base, frame + 97, m.output_channels());
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
  const uint32_t entries = m.entries_set();
  if (entries != 0) {
    for (uint32_t i = 0; i < m.entries_set(); i++) {
      const MatrixEntry entry(base, m.entries().address + MatrixEntry::kSize * i);
      const uint32_t target_samples = guest::LoadU32(base, frame + 104);
      const uint32_t target_channels = guest::LoadU8(base, frame + 97);
      const uint32_t source_channels = guest::LoadU8(base, frame + 113);
      const uint32_t out_channel = entry.output_channel();
      const double goal = double(entry.target_level());
      const uint32_t in_channel = entry.input_channel();
      const uint32_t out = (out_channel << 10) + target_samples;
      const uint32_t in = (in_channel << 10) + guest::LoadU32(base, frame + 120);
      if (out_channel >= target_channels) {
        continue;
      }
      if (in_channel >= source_channels) {
        if (m.overwrite() != 0) {
          XMemSetCall(base, out, 1024, 0x824666A4, frame, call);
        }
        continue;
      }
      const double level = double(entry.level());
      guest::StoreFloat(base, frame + 80, float(level));
      const double difference = double(float(goal - level));
      RampConstants(base, frame);
      const bool overwrite = m.overwrite() != 0;
      const double first = double(float(difference * LoadSingle(base, 0x82008058)));
      guest::StoreFloat(base, frame + 84, float(first));
      const double each = double(float(first * LoadSingle(base, 0x820021F8)));
      guest::StoreFloat(base, frame + 88, float(each));
      Vec v0 = RampVectors(base, frame);
      if (!overwrite) {
        SetRegs(call, Args({out, in, frame + 128, frame + 144, 256}));
        MatrixMixRamp(base, out, in, frame + 128, frame + 144, 256);
        entry.set_level(float(goal));
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
      entry.set_level(float(goal));
      StoreAligned(base, frame + 128, v0);
    }
  } else if (source != target && guest::LoadU8(base, frame + 97) != 0) {
    uint32_t offset = 0;
    for (uint32_t i = 0; i < guest::LoadU8(base, frame + 97); i++, offset += 1024) {
      const uint32_t out = offset + guest::LoadU32(base, frame + 104);
      const uint32_t in = offset + guest::LoadU32(base, frame + 120);
      const bool overwrite = m.overwrite() != 0;
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
  const VolumeMeter m(base, meter);
  BufferGetFormatThunk(base, buffer, frame + 80);
  const uint32_t meter_channels = m.channels();
  uint32_t channels = guest::LoadU8(base, frame + 81);
  if (channels >= meter_channels) {
    channels = meter_channels;
  }
  const double zero = LoadSingle(base, 0x82000A34);
  const double threshold = double(XaudioObjectsGlobals::meter_threshold(base));
  double counted = zero;  // f10: also the second fold-down's sum
  uint32_t sample = 0;    // r8: runs on across the channels
  if (meter_channels != 0) {
    const double scale = LoadSingle(base, 0x82008058);
    for (uint32_t c = 0; c < m.channels(); c = (c + 1) & 0xFF) {
      const uint32_t history = m.history_slot();
      const uint32_t length = m.history_length();
      double peak = counted, sum = counted, squares = counted;
      const uint32_t quotient = length != 0 ? history / length : 0;
      const uint32_t slot = history - uint32_t(int64_t(int32_t(quotient)) * int64_t(int32_t(length)));
      const uint32_t index =
          uint32_t(int64_t(int32_t(slot)) * int64_t(int32_t(m.channels()))) + c;
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
      guest::StoreFloat(base, m.rms() + index * 4, float(squares));
      guest::StoreFloat(base, index * 4 + m.threshold_sums().address, float(sum));
      guest::StoreFloat(base, index * 4 + m.peaks().address, float(peak));
    }
  }
  if (m.channels() == 8 && channels == 6) {
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
    // The fold-downs to slots 6 and 7 of the arrays' first words.
    guest::StoreFloat(base, m.threshold_sums().values_address() + 24, float(sum_left));
    guest::StoreFloat(base, m.threshold_sums().values_address() + 28, float(counted));
    guest::StoreFloat(base, m.peaks().values_address() + 24, float(peak_left));
    guest::StoreFloat(base, m.peaks().values_address() + 28, float(peak_right));
  }
  const uint32_t history = m.history_slot() + 1;
  m.set_history_slot(history);
  if (history == m.channels()) {
    m.set_filled(1);
  }
  return 0;
}

// --- The render driver's set-up and the reverb's small parts ---

uint32_t RenderDriverSetUpThunk(uint8_t* base, uint32_t driver, uint32_t descriptor, uint32_t* r4) {
  ITC_NATIVE_RUNNING("sub_824657C8 the render driver's set-up");
  *r4 = XAudioObjectDescriptor(base, descriptor).render_device();
  return Method(base, driver, 28);
}

void ReverbDestruct(uint8_t* base, uint32_t reverb) {
  ITC_NATIVE_RUNNING("sub_824656B8 Reverb::Destruct");
  const Reverb r(base, reverb);
  r.set_vtable(0x8204AAB0);
  r.set_inner_vtable(0x8204AA94);
  const uint32_t offset = r.core_vtable().base_offset();
  guest::StoreU32(base, offset + r.core_vtable_address(), 0x8204AA74);
  r.set_lines_vtable(0x8204A868);
  r.set_inner_vtable(kRootVtable);
}

void ReverbDefaults(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82467538 ReverbCore::Defaults");
  using G = XaudioObjectsGlobals;
  const NativeReverbParams p(base, out);
  CopyFloat(base, p.room_filter_freq_address(), G::reverb_default_room_filter_freq_address());
  p.set_reflections_delay(5);
  CopyFloat(base, p.decay_time_address(), G::reverb_default_decay_time_address());
  p.set_reverb_delay(5);
  p.set_position_left(6);
  CopyFloat(base, p.room_filter_main_address(), G::reverb_default_gain_address());
  p.set_position_right(6);
  CopyFloat(base, p.room_filter_hf_address(), G::reverb_default_gain_address());
  p.set_rear_delay(5);
  CopyFloat(base, p.reflections_gain_address(), G::reverb_default_gain_address());
  CopyFloat(base, p.reverb_gain_address(), G::reverb_default_gain_address());
  p.set_early_diffusion(8);
  CopyFloat(base, p.density_address(), G::reverb_default_density_address());
  p.set_late_diffusion(8);
  CopyFloat(base, p.room_size_address(), G::reverb_default_density_address());
  p.set_low_eq_gain(8);
  p.set_low_eq_cutoff(4);
  p.set_high_eq_gain(8);
  p.set_high_eq_cutoff(4);
}

uint32_t LargestPrimeBelow(uint8_t* base, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_8246DFE8 LargestPrimeBelow");
  if (n <= 1) {
    return 1;
  }
  if (n >= 9973) {
    return 9973;
  }
  const uint32_t kPrimes = XaudioObjectsGlobals::primes_address();
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
    return std::bit_cast<double>(uint64_t(guest::LoadU64(base, address)));
  };
  const auto f = [](double x) { return double(float(x)); };
  using G = XaudioObjectsGlobals;
  const schema::ReverbLowPass lp(base, filter);
  double f0 = double(lp.level_b());
  double f13 = double(lp.level_a());
  const double f29 = f(f0 + f13);
  const double f26 = double(G::reverb_default_gain(base));
  f0 = double(G::reverb_default_decay_time(base));
  const double f28 = f29 < f26 ? f(f13 / f29) : f0;
  double f12 = f(f0 - f28);
  f13 = double(lp.frequency());
  f0 = f13 * G::two_pi(base);
  f13 = double(lp.rate());
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
  lp.set_gain(float(f1));
  f1 = c_runtime::Pow(f1, f30);
  const double limit = double(G::filter_limit(base));
  f0 = f(f1);
  lp.set_gain(float(f0));
  if (!(f31 < limit)) {
    lp.set_coefficient(float(f26));
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
  lp.set_coefficient(float(f0));
}

uint64_t ReverbSetParameters(uint8_t* base, uint32_t reverb, uint32_t kind, uint32_t data,
                             uint32_t thread, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82462148 Reverb::SetParameters");
  const uint32_t which = kind & 0xFF;
  if (which >= 3) {
    return 0xFFFFFFFF80070057ull;  // E_INVALIDARG
  }
  const Reverb r(base, reverb);
  const I3dl2Reverb core(base, r.core_vtable_address());
  const ReverbParameterData given(base, data);
  if (which == 2) {
    const double gain = LoadSingle(base, given.value_address());
    SpinTake(base, thread, call);
    SpinTake(base, thread, call);
    core.set_pending_gain(float(double(float(gain * LoadSingle(base, 0x82002A0C)))));
    core.set_gain_changed(1);
  } else if (which == 1) {
    const uint32_t source = given.value();
    SpinTake(base, thread, call);
    SpinTake(base, thread, call);
    Copy(base, core.pending_i3dl2_address(), source, 48, 0x824622D8, frame, call);
    core.set_i3dl2_changed(1);
  } else {
    // The native parameters read into the frame over the defaults.
    const uint32_t source = given.value();
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
                                         double(XaudioObjectsGlobals::reverb_room_size_scale(base))))));
    SpinTake(base, thread, call);
    SpinTake(base, thread, call);
    Copy(base, core.pending_reflections_delay_address(), local, NativeReverbParams::kSize,
         0x8246244C, frame, call);
    core.set_parameters_changed(1);
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
      double(float(log)) * std::bit_cast<double>(uint64_t(guest::LoadU64(base, scale)));
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
    guest::StoreU64(base, frame + 80, bits);
    return f(double(int64_t(bits)));
  };
  const auto level = [&](uint32_t address) {
    return uint64_t(int64_t(int32_t(guest::LoadU32(base, address))));
  };
  using G = XaudioObjectsGlobals;
  // The 12 I3DL2 parameters at `in` (I3dl2Reverb's +4 on).
  const I3dl2Reverb i3dl2(base, in - 4);
  const NativeReverbParams o(base, out);
  o.set_rear_delay(G::reverb_default_rear_delay(base));
  o.set_position_left(G::reverb_default_position(base));
  o.set_position_right(G::reverb_default_position(base));
  o.set_position_matrix_left(G::reverb_default_position_matrix(base));
  o.set_position_matrix_right(G::reverb_default_position_matrix(base));
  CopyFloat(base, o.room_size_address(), G::reverb_default_density_address());
  double f0 = integer(level(i3dl2.room_address()));
  const double f31 = LoadSingle(base, 0x8200222C);
  o.set_room_filter_main(float(f(f0 * f31)));
  const uint64_t room_hf = level(i3dl2.room_hf_address());
  o.set_low_eq_cutoff(4);
  o.set_high_eq_cutoff(6);
  f0 = integer(room_hf);
  const double f30 = LoadSingle(base, 0x82000A30);
  o.set_room_filter_hf(float(f(f0 * f31)));
  double f1;
  if (!(double(i3dl2.decay_hf_ratio()) < f30)) {
    o.set_high_eq_gain(8);
    f1 = c_runtime::Log10(double(i3dl2.decay_hf_ratio()));
    o.set_low_eq_gain(DecayStep(base, f1, G::decay_scale_high_address(), frame));
    f0 = f(double(i3dl2.decay_time()) * double(i3dl2.decay_hf_ratio()));
    o.set_decay_time(float(f0));
  } else {
    o.set_low_eq_gain(8);
    f1 = c_runtime::Log10(double(i3dl2.decay_hf_ratio()));
    o.set_high_eq_gain(DecayStep(base, f1, G::decay_scale_low_address(), frame));
    CopyFloat(base, o.decay_time_address(), i3dl2.decay_time_address());
  }
  f0 = integer(level(i3dl2.reflections_address()));
  o.set_reflections_gain(float(f(f0 * f31)));
  // The reflections delay, clamped below the table's word less 1 and to at
  // least 1.
  const uint32_t reflections_limit = G::reverb_reflections_delay_limit(base);
  f0 = double(i3dl2.reflections_delay());
  double f13 = integer(reflections_limit);
  const double f12 = LoadSingle(base, 0x82001BDC);
  f0 = f(f0 * f12);
  if (!(f0 < f13)) {
    f0 = f(f13 - f30);
  }
  if (!(f0 > f30)) {
    f0 = f30;
  }
  o.set_reflections_delay(uint32_t(ToFixed(f0)));
  f0 = integer(level(i3dl2.reverb_address()));
  o.set_reverb_gain(float(f(f0 * f31)));
  // The reverb delay, clamped below the table's word less 1 only.
  const uint32_t reverb_limit = G::reverb_reverb_delay_limit(base);
  f0 = f(double(i3dl2.reverb_delay()) * f12);
  f13 = integer(reverb_limit);
  if (!(f0 < f13)) {
    f0 = f(f13 - f30);
  }
  o.set_reverb_delay(uint32_t(ToFixed(f0)));
  // The diffusion, scaled.
  f0 = f(double(i3dl2.diffusion()) * LoadSingle(base, 0x82002C4C));
  guest::StoreU32(base, frame + 80, uint32_t(ToFixed(f0)));
  const uint32_t diffusion = guest::LoadU32(base, frame + 80);
  o.set_early_diffusion(diffusion);
  o.set_late_diffusion(diffusion);
  CopyFloat(base, o.density_address(), i3dl2.density_address());
  CopyFloat(base, o.room_filter_freq_address(), i3dl2.hf_reference_address());
  return f1;
}

// --- The reverb's construction ---

uint64_t I3dl2ParamsConstruct(uint8_t* base, uint32_t object, uint32_t flag, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82469848 I3dl2Params::I3dl2Params");
  const I3dl2Reverb o(base, object);
  guest::StoreU32(base, frame + 80, 0);
  if (flag != 0) {
    o.set_vtable(0x8204B948);
    // Standalone, its lines at +52.
    ReverbCoreInit(base, o.pending_reflections_delay_address());
  }
  const uint32_t offset = o.vtable().base_offset();
  guest::StoreU32(base, offset + object, 0x8204AA74);
  CopyFloat(base, o.room_rolloff_factor_address(), 0x82000A34);
  CopyFloat(base, o.decay_time_address(), 0x82000A30);
  o.set_room(-10000);
  o.set_room_hf(0);
  o.set_reflections(-10000);
  CopyFloat(base, o.decay_hf_ratio_address(), 0x82001BE4);
  o.set_reverb(-10000);
  CopyFloat(base, o.reflections_delay_address(), 0x82002FB0);
  CopyFloat(base, o.reverb_delay_address(), 0x8200256C);
  CopyFloat(base, o.diffusion_address(), 0x82002A0C);
  CopyFloat(base, o.density_address(), 0x82002A0C);
  CopyFloat(base, o.hf_reference_address(), 0x82001FCC);
  return object;
}

uint64_t ReverbConstruct(uint8_t* base, uint32_t reverb, uint32_t descriptor, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82465608 Reverb::Reverb");
  const Reverb r(base, reverb);
  const I3dl2Reverb core(base, r.core_vtable_address());
  r.set_inner_vtable(kObjectBaseVtable);
  r.set_ref_count(1);
  r.set_owner(XAudioObjectDescriptor(base, descriptor).owner());
  r.set_vtable(0x8204AAB0);
  r.set_inner_vtable(0x8204AA94);
  guest::StoreU32(base, frame + 80, 0);
  core.set_vtable(0x8204AA8C);
  ReverbCoreInit(base, core.core_vtable_address());
  I3dl2ParamsConstruct(base, core.address, 0, Below(base, frame, 112));
  const uint32_t offset = core.vtable().base_offset();
  guest::StoreU32(base, offset + core.address, 0x8204AA88);
  ReverbDefaults(base, core.pending_reflections_delay_address());
  r.set_time_spent(0);
  r.set_passes(0);
  return reverb;
}

uint64_t ReverbCreate(uint8_t* base, uint32_t descriptor, uint32_t allocator, uint32_t out,
                      uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82465710 Reverb::Create");
  const uint64_t object = Allocate(base, allocator, Reverb::kSize, 0x82465744, frame, call);
  if (uint32_t(object) == 0) {
    return kEOutOfMemory;
  }
  // Reverb::Reverb returns its r3 (the allocation's, all 64 bits).
  ReverbConstruct(base, uint32_t(object), descriptor, Below(base, frame, 128));
  const uint32_t reverb = uint32_t(object);
  call(Method(base, reverb, 28), 0x82465790, At(frame, Args({object, descriptor, allocator})));
  const uint64_t r3 = LastCallR3();
  if (int32_t(r3) >= 0) {
    FactoryObjectOut(base, out).set_object(reverb);
    return r3;
  }
  call(Method(base, reverb + 4, 12), 0x824657BC, At(frame, Args({object + 4})));
  return r3;
}

// --- The reverb's pass ---

uint64_t I3dl2SetParameter(uint8_t* base, uint32_t object, uint32_t index, uint32_t value,
                           uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824696F0 I3dl2::SetParameter");
  const I3dl2Reverb o(base, object);
  if (index == 0) {
    Copy(base, o.room_address(), value, 48, 0x8246976C, frame, call);
  } else if (index <= 12) {
    // Words for room, room HF, reflections and reverb (1, 2, 6, 8); the
    // floats copied through f0 (lfs, stfs): their bits.
    guest::StoreU32(base, o.room_address() + 4 * (index - 1),
                    guest::LoadU32(base, value + 4 * (index - 1)));
  }
  const uint32_t native = frame + 80;
  ReverbDefaults(base, native);
  I3dl2ToNative(base, o.room_address(), native, Below(base, frame, 128));
  const uint32_t core = o.vtable().base_offset() + object;
  return ReverbCoreApply(base, core, native, Below(base, frame, 432), call);
}

uint64_t ReverbRun(uint8_t* base, uint64_t core64, uint32_t inputs, uint32_t outputs,
                   uint32_t thread, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82465258 Reverb::Run");
  const I3dl2Reverb core(base, uint32_t(core64));
  const auto proper = [&] { return core.vtable().base_offset() + core64; };
  SpinTake(base, thread, call);
  if (core.gain_changed() != 0) {
    // ReverbCore::SetGain (sub_82467A00), merged: fmuls, stfs.
    const double gain =
        double(float(double(core.pending_gain()) * LoadSingle(base, 0x8200222C)));
    ReverbCore(base, uint32_t(proper())).set_gain(float(gain));
  }
  if (core.parameters_changed() != 0) {
    ReverbCoreApply(base, uint32_t(proper()), core.pending_reflections_delay_address(),
                    Below(base, frame, 432), call);
  } else if (core.i3dl2_changed() != 0) {
    I3dl2SetParameter(base, core.address, 0, core.pending_i3dl2_address(), Below(base, frame, 192),
                      call);
  }
  core.set_gain_changed(0);
  core.set_parameters_changed(0);
  core.set_i3dl2_changed(0);
  SpinRelease(base, thread, call);
  return ReverbCoreProcess(base, proper(), inputs, outputs, frame);
}

uint64_t ReverbProcess(uint8_t* base, uint32_t reverb, uint32_t input, uint32_t output,
                       uint32_t thread, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82465428 Reverb::Process");
  // GetFormat's r3 (always 0: these errors aren't reached).
  for (const auto& [iface, out] : {std::pair{input, frame + 120}, std::pair{output, frame + 104}}) {
    const int32_t result = int32_t(BufferGetFormatThunk(base, iface, out));
    if (result < 0) {
      return uint64_t(int64_t(result));
    }
  }
  guest::StoreU8(base, frame + 104, 0);
  guest::StoreU8(base, frame + 105, 6);
  guest::StoreU32(base, frame + 108, 48000);
  uint64_t cleared;
  if (const auto result = BufferClearThunk(base, output, frame + 104, 3)) {
    cleared = uint64_t(int64_t(int32_t(*result)));
  } else {
    call(kBufferClearThunk, 0x82465498, At(frame, Args({output, frame + 104, 3})));
    cleared = LastCallR3();
  }
  if (int32_t(cleared) < 0) {
    return cleared;
  }
  const uint32_t channels = guest::LoadU8(base, frame + 121);
  if (channels < 1) {
    return 0;
  }
  if (channels >= 3) {
    return 0xFFFFFFFF80070057ull;  // E_INVALIDARG
  }
  const Reverb r(base, reverb);
  const uint32_t core = r.core_vtable_address();
  ReverbSetStereo(base, r.core_vtable().base_offset() + reverb + 16, channels == 1 ? 1 : 0);
  const uint32_t in = guest::LoadU32(base, frame + 128);
  guest::StoreU32(base, frame + 80, in);
  guest::StoreU32(base, frame + 84, in + 1024);
  const uint32_t out = guest::LoadU32(base, frame + 112);
  guest::StoreU32(base, frame + 144, out);
  guest::StoreU32(base, frame + 148, out + 1024);
  guest::StoreU32(base, frame + 152, out + 2048);
  guest::StoreU32(base, frame + 156, out + 4096);
  guest::StoreU32(base, frame + 160, out + 5120);
  const auto counter = [&](uint32_t at) {
    const uint64_t count = helpers::PerformanceCounter();
    guest::StoreU64(base, at, count);
  };
  counter(frame + 88);
  ReverbRun(base, core, frame + 80, frame + 144, thread, Below(base, frame, 144), call);
  counter(frame + 96);
  const uint64_t total = uint64_t(r.time_spent()) - uint64_t(guest::LoadU64(base, frame + 88));
  const uint64_t runs = uint64_t(r.passes()) + 1;
  r.set_passes(runs);
  r.set_time_spent(total + uint64_t(guest::LoadU64(base, frame + 96)));
  return 0;
}

// --- The PCM codec's pass ---

namespace {

constexpr uint32_t kSetFlagsFunction = 0x82467078;       // CodecBase::SetFlags
constexpr uint32_t kCompletePacketFunction = 0x824674D8;  // CodecBase::CompletePacket
constexpr uint32_t kPcmCodecStopFunction = 0x8246D500;   // PcmCodec::Stop
constexpr uint32_t kBufferClearFunction = 0x8245F8E8;    // XAudioBuffer::Clear
constexpr uint32_t kFloatZero = 0x82000A34;              // the f31 constant

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
  const ConverterState c(base, converter);
  const uint32_t written = c.frames_written();
  const uint32_t left = c.output_frames() - written;
  uint32_t channels = c.channels();
  if (left == 0 || channels == 0) {
    return;
  }
  const uint32_t bytes = left << 2;
  uint32_t offset = written << 2;
  while (channels != 0) {
    const uint32_t at = c.output() + offset;
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
  const PcmCodec c(base, codec);
  const uint32_t codec_base = c.base_vtable_address();
  const ConverterState converter(base, c.converter_input_address());
  const uint32_t format = frame + 80;
  const uint64_t buffer = uint32_t(iface) != 0 ? iface - 8 : 0;
  SpinTake(base, thread, call);
  SetRegs(call, Args({buffer, format}));
  uint64_t r29 = BufferGetFormat(base, uint32_t(buffer), format);
  if (int32_t(r29) >= 0) {
    guest::StoreU8(base, frame + 81, c.channels());
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
    converter.set_output(guest::LoadU32(base, frame + 88));
    converter.set_output_frames(256);
    converter.set_frames_written(0);
    const uint8_t state = c.state();
    if ((state & 1) == 0 || (state & 0x10) != 0) {
      // Not playing, or starved: the output zeroed, 1.
      ZeroRest(base, converter.address, call);
      SpinRelease(base, thread, call);
      return 1;
    }
    if ((state & 2) != 0) {
      // Just started: the volume at once, the converter's ramp and kept
      // frames reset, bit 1 cleared.
      CopyFloat(base, converter.target_volume_address(), c.volume_address());
      CopyFloat(base, converter.volume_address(), converter.target_volume_address());
      for (uint32_t k = 0; k < 6; k++) {
        guest::StoreU32(base, converter.last_sample_0_address() + 4 * k, 0);
      }
      CopyFloat(base, converter.fraction_address(), kFloatZero);
      ProcessSetFlags(base, codec_base, 2, 0, 0x8246DA7C, thread, frame, call);
    }
    if ((c.state() & 4) != 0) {
      // Stopping: the volume halved a pass until the count runs out.
      const double volume = double(converter.target_volume());
      const uint32_t count = (uint32_t(c.fade_passes()) + 0xFFFF) & 0xFFFF;
      c.set_fade_passes(uint16_t(count));
      if (count != 0) {
        const double f0 =
            double(float(volume * double(XaudioObjectsGlobals::fade_step(base))));
        converter.set_target_volume(float(f0));
      } else {
        CopyFloat(base, converter.target_volume_address(), kFloatZero);
      }
    }
    SpinRelease(base, thread, call);
  }
  SetRegs(call, Args({6, 1}));
  AddStat(base, 6, 1);
  uint64_t entry = 0;
  for (;;) {
    uint64_t current = entry;
    if (uint32_t(entry) != 0 && converter.input_end() == converter.position()) {
      // The packet's region played.
      const PacketEntry e(base, uint32_t(entry));
      const uint32_t loops = e.loop_count();
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
          e.set_loop_count(loops - 1);
        }
        c.set_position(e.loop_start());
        const uint32_t callback = c.loop_end_callback();
        if (callback != 0) {
          guest::StoreU32(base, frame + 88, 0);
          guest::StoreU32(base, frame + 80, c.owner());
          guest::StoreU32(base, frame + 84, e.context());
          call(callback, 0x8246D9E4, At(frame, Args({frame + 80})));
        }
      }
    }
    if (converter.output_frames() == converter.frames_written()) {
      entry = 0;
    } else if (uint32_t(current) != 0) {
      entry = current;
    } else {
      // The first queued packet, started on its first pass.
      SpinTake(base, thread, call);
      const uint64_t first = FirstEntry(base, c.queued_next_address());
      const PacketEntry e(base, uint32_t(first));
      if (uint32_t(first) != 0 && e.begun() == 0) {
        c.set_position(e.play_begin());
        e.set_begun(1);
      }
      SpinRelease(base, thread, call);
      entry = first;
    }
    if (uint32_t(entry) == 0) {
      break;
    }
    const PacketEntry packet(base, uint32_t(entry));
    const uint32_t end = packet.loop_count() != 0 ? packet.loop_end() : packet.play_end();
    const uint32_t data = packet.data();
    const uint32_t position = c.position();
    converter.set_input_end(end);
    converter.set_input(data);
    converter.set_position(position < end ? position : end);
    SetRegs(call, Args({converter.address}));
    const uint64_t used = ResamplerProcess(base, converter.address, Below(base, frame, 128), call);
    SpinTake(base, thread, call);
    if (position == c.position()) {
      c.set_position(uint32_t(used + position));
    }
    c.set_frames_played(uint32_t(c.frames_played() + used));
    SpinRelease(base, thread, call);
  }
  // Out of packets, or the output full: the rest zeroed; out of packets
  // with nothing queued, the end of stream's stop or starved.
  SpinTake(base, thread, call);
  CopyFloat(base, converter.volume_address(), converter.target_volume_address());
  const uint32_t written = converter.frames_written();
  if (converter.output_frames() - written != 0) {
    ZeroRest(base, converter.address, call);
    if ((c.state() & 8) == 0) {
      const PacketList head(base, c.queued_next_address());
      const uint32_t next = head.next().address;
      if (next == head.address || next - head.node_offset() == 0) {
        ProcessSetFlags(base, codec_base, 16, 16, 0x8246DEC0, thread, frame, call);
      }
    } else {
      ProcessStop(base, codec, 0x8246DEDC, thread, frame, call);
    }
  }
  const uint8_t state = c.state();
  if ((state & 4) != 0 && (c.fade_passes() == 0 || (state & 0x10) != 0)) {
    ProcessStop(base, codec, 0x8246DF1C, thread, frame, call);
  }
  SpinRelease(base, thread, call);
  return 0;
}

}  // namespace xaudio
