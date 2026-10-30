// XAudio's factory objects natively: adapters in front of
// src/xaudio/objects.h (docs/functions/xaudio-objects.md), with the register
// steps the harness shares (hooks/xaudio_objects.h). Most are reached
// through the objects' vtables and the class tables, so these adapters stay
// until their callers are native.

#include "hooks/xaudio_objects.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/guest_call.h"
#include "hooks/xaudio.h"
#include "xaudio/objects.h"
#include "xaudio/resampler.h"
#include "xaudio/reverb_core.h"
#include "xaudio/reverb_stages.h"

ITC_FLAG_BOOL(native_xaudio_objects, true, "ITC",
              "Run the parts of XAudio's factory objects made native "
              "(docs/functions/xaudio-objects.md: the class tables' sizes sub_82466B78, "
              "sub_82466840, sub_82463B78 and sub_825A60E0; the GetInfo slots sub_82466E80, "
              "sub_82465BE0 and sub_825A5778, XAudioCodec::GetState sub_8246A920, "
              "XAudioMatrix::SetOverwrite sub_82465C08, ReverbCore::SetStereo sub_824679F8, "
              "XAudioObject::Release sub_8246CE70; the codecs' construction XAudioCodec::Create "
              "sub_82466BA0 and CodecBase::Init sub_82469978, their destruction "
              "XAudioCodec::Destruct sub_82466EE8 and the inner destructors sub_8246CE68, "
              "sub_8246A918 and sub_82465BD8 (the XMA object's release stays the translated "
              "code's); XAudioMatrix::Create sub_82466888, VolumeMeter::Create sub_825A6460 and "
              "Init sub_825A6138; under the voice lists' spin lock (taken as the original does) "
              "the packet queue's SetFlags sub_82467078, RetirePacket sub_8246A430, "
              "CompletePacket sub_824674D8, FlushPackets sub_824699E0 (and sub_8246CEC8), "
              "SetFormat sub_82469E58, SubmitPacket sub_82469EF8 (and sub_8246CEB8) and "
              "SubmitRegion sub_8246A1C0, the PCM codec's SetFormat sub_8246CFA8, Start "
              "sub_8246D410, Stop sub_8246D500, SetVolume sub_8246D008, SetPitch sub_8246D0C8 and "
              "SetFrequencyRatio sub_8246D1E0 (and sub_8246D320), XAudioCodec::EndOfStream "
              "sub_8246D630, the XMA codec's SubmitRegion sub_8246A860, SetVolume sub_8246AB48 "
              "and SetPitch sub_8246AC40; the output matrix's Init sub_82465C30, SetMatrix "
              "sub_82465E50, SetLevels sub_82466188 and Mix sub_82466440; VolumeMeter::Process "
              "sub_825A6210; the render driver's set-up sub_824657C8; the reverb's destructor "
              "sub_82465410 and sub_824656B8, ReverbCore::Defaults sub_82467538, "
              "LargestPrimeBelow sub_8246DFE8, FilterCoefficient sub_824675B8 and the delay "
              "lines' fills sub_82467720 to sub_82467990; the resampler's Resampler::Process "
              "sub_8246F140 and its kernels sub_8247E188, sub_8247E260 (with their vector paths "
              "sub_8246E208 and sub_8246E4C8), sub_82473E48, sub_82474188, sub_8247F558 and "
              "sub_82478E60; ReverbCore::Process sub_82463B90 with its stages, the native ones "
              "reached only from it: sub_82462BC8, sub_82462CF8, sub_82463060 and sub_82463578, "
              "the all-passes sub_82462DC8, sub_82463130, sub_824632A0 and sub_82463410, and the "
              "crossfading delay lines sub_824624F8 and sub_82463620; Reverb::SetParameters "
              "sub_82462148 under the voice lists' spin lock, taken as the original does, and "
              "I3dl2::ToNative sub_82469418; PcmCodec::Process sub_8246D860 under the voice lists' "
              "spin lock, taken as the original does; ReverbCore::Apply sub_82467A18 and Init sub_82468848; the reverb's construction "
              "I3dl2Params::I3dl2Params sub_82469848, Reverb::Reverb sub_82465608 and "
              "Reverb::Create sub_82465710) natively instead of through the "
              "translated code (the same reads and writes; checked by "
              "--run_harnesses=xaudio_objects)");

namespace hooks {

namespace xa = xaudio;

namespace {

// A body with calls, run in a frame of the original's size (0: none, the
// body given the stack pointer); its r3, in full, to r3.
template <typename Body>
void Framed(PPCContext& ctx, uint8_t* base, uint32_t frame_bytes, Body body) {
  const uint64_t lr = ctx.lr;
  uint64_t r3 = 0;
  const uint32_t floor = xa::StackFloor();
  xa::StackFloor() = 0;
  if (frame_bytes == 0) {
    r3 = body(XAudioCaller(ctx, base), ctx.r1.u32);
  } else {
    guest_call::InFrame(ctx, base, frame_bytes, uint32_t(lr),
                        [&](uint32_t frame) { r3 = body(XAudioCaller(ctx, base), frame); });
  }
  xa::StackFloor() = floor;
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

}  // namespace

void XAudioCodecSize(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::CodecSize(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioMatrixSize(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::MatrixSize(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioReverbSize(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::ReverbSize(base, ctx.r4.u32);
}

void XAudioMeterSize(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::MeterSize(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioCodecGetInfo(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::CodecGetInfo(base, ctx.r4.u32);
}

void XAudioMatrixGetInfo(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::MatrixGetInfo(base, ctx.r4.u32);
}

void XAudioMeterGetInfo(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::MeterGetInfo(base, ctx.r4.u32);
}

void XAudioCodecGetState(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::CodecGetState(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioMatrixSetOverwrite(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::MatrixSetOverwrite(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioReverbSetStereo(PPCContext& ctx, uint8_t* base) {
  xa::ReverbSetStereo(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioObjectRelease(PPCContext& ctx, uint8_t* base) {
  const uint32_t object = ctx.r3.u32;
  Framed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::ObjectRelease(base, object, frame, call);
  });
}

void XAudioCodecCreate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t descriptor = ctx.r3.u32, allocator = ctx.r4.u32, out = ctx.r5.u32;
  Framed(ctx, base, 0, [&](const xa::GuestCall& call, uint32_t sp) {
    return xa::CodecCreate(base, descriptor, allocator, out, sp, call);
  });
}

void XAudioCodecBaseInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t codec_base = ctx.r3.u32, format = ctx.r4.u32, count = ctx.r5.u32,
                 allocator = ctx.r7.u32;
  Framed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CodecBaseInit(base, codec_base, format, count, allocator, frame, call);
  });
}

void XAudioCodecDestruct(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::CodecDestruct(base, ctx.r3.u32);
}

void XAudioPcmCodecInnerDestroy(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8246CE68 PcmCodec inner destructor");
  ctx.r3.u64 = xa::PcmCodecDestroy(base, ctx.r3.u64 - 4);
}

void XAudioXmaCodecInnerDestroy(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8246A918 XmaCodec inner destructor");
  const uint64_t object = ctx.r3.u64 - 4;
  Framed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::XmaCodecDestroy(base, object, frame, call);
  });
}

void XAudioMatrixInnerDestroy(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82465BD8 XAudioMatrix inner destructor");
  ctx.r3.u64 = ctx.r3.u64 - 4;
  xa::MatrixDestruct(base, ctx.r3.u32);
}

void XAudioMatrixCreate(PPCContext& ctx, uint8_t* base) {
  const uint32_t descriptor = ctx.r3.u32, allocator = ctx.r4.u32, out = ctx.r5.u32;
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MatrixCreate(base, descriptor, allocator, out, frame, call);
  });
}

void XAudioMeterCreate(PPCContext& ctx, uint8_t* base) {
  const uint32_t descriptor = ctx.r3.u32, allocator = ctx.r4.u32, out = ctx.r5.u32;
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MeterCreate(base, descriptor, allocator, out, frame, call);
  });
}

void XAudioMeterInit(PPCContext& ctx, uint8_t* base) {
  const uint32_t meter = ctx.r3.u32, descriptor = ctx.r4.u32, allocator = ctx.r5.u32;
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MeterInit(base, meter, descriptor, allocator, frame, call);
  });
}

void XAudioPacketListNext(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::PacketListNext(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u32);
}

void XAudioPacketListClear(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::PacketListClear(base, ctx.r3.u32);
}

void XAudioPacketListsAlloc(PPCContext& ctx, uint8_t* base) {
  const uint64_t lists = ctx.r3.u64;
  const uint32_t count = ctx.r4.u32, allocator = ctx.r5.u32;
  Framed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::PacketListsAlloc(base, lists, count, allocator, frame, call);
  });
}

void XAudioPcmCodecConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t object = ctx.r3.u64;
  const uint32_t descriptor = ctx.r4.u32, allocator = ctx.r5.u32;
  Framed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::PcmCodecConstruct(base, object, descriptor, allocator, frame, call);
  });
}

void XAudioXmaCodecConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t object = ctx.r3.u64;
  const uint32_t descriptor = ctx.r4.u32, allocator = ctx.r5.u32;
  Framed(ctx, base, 160, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::XmaCodecConstruct(base, object, descriptor, allocator, frame, call);
  });
}

// --- The packet queue and the codecs' slots ---

namespace {

uint32_t Thread(PPCContext& ctx, uint8_t* base) { return XAudioThreadOf(ctx, base).r13; }

// A body whose plain C++ refuses where the original traps: run with the
// original's registers when `refuses`, else framed.
template <typename Body>
void FramedOr(PPCContext& ctx, uint8_t* base, uint32_t frame_bytes, bool refuses,
              void (*original)(PPCContext&, uint8_t*), Body body) {
  if (refuses) {
    original(ctx, base);
    return;
  }
  Framed(ctx, base, frame_bytes, [&](const xa::GuestCall& call, uint32_t frame) {
    return body(call, frame).value_or(0);
  });
}

bool NoBlockSize(uint8_t* base, uint32_t codec_base) {
  return guest::LoadU16(base, codec_base + 92) == 0;
}

}  // namespace

void XAudioCodecSetFlags(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec_base = ctx.r3.u32, mask = ctx.r4.u32, bits = ctx.r5.u32;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t) {
    return xa::CodecSetFlags(base, codec_base, mask, bits, thread, call);
  });
}

void XAudioCodecRetirePacket(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec_base = ctx.r3.u32;
  const uint64_t entry = ctx.r4.u64;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CodecRetirePacket(base, codec_base, entry, thread, frame, call);
  });
}

void XAudioCodecCompletePacket(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec_base = ctx.r3.u32;
  const uint64_t entry = ctx.r4.u64, result = ctx.r5.u64;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CodecCompletePacket(base, codec_base, entry, result, thread, frame, call);
  });
}

void XAudioCodecFlushPackets(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec_base = ctx.r3.u32;
  Framed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CodecFlushPackets(base, codec_base, frame, call);
  });
}

void XAudioCodecFlush(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8246CEC8 the codecs' flush");
  // A tail call: FlushPackets' frame below the caller's.
  const uint32_t codec_base = ctx.r3.u32 + 16;
  ctx.r3.u64 = ctx.r3.u64 + 16;
  Framed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CodecFlushPackets(base, codec_base, frame, call);
  });
}

void XAudioCodecBaseSetFormat(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec_base = ctx.r3.u32, format = ctx.r4.u32, extra = ctx.r5.u32;
  Framed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CodecBaseSetFormat(base, codec_base, format, extra, frame, call);
  });
}

void XAudioPcmCodecSetFormat(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec = ctx.r3.u32, format = ctx.r4.u32, extra = ctx.r5.u32;
  Framed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::PcmCodecSetFormat(base, codec, format, extra, frame, call);
  });
}

void XAudioPcmCodecStart(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec = ctx.r3.u32;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::PcmCodecStart(base, codec, thread, frame, call);
  });
}

void XAudioPcmCodecStop(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec = ctx.r3.u32, flags = ctx.r4.u32;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::PcmCodecStop(base, codec, flags, thread, frame, call);
  });
}

void XAudioCodecEndOfStream(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec = ctx.r3.u32;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CodecEndOfStream(base, codec, thread, frame, call);
  });
}

void XAudioPcmCodecSetVolume(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t codec = ctx.r3.u32;
  const double volume = ctx.f1.f64;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t) {
    return xa::PcmCodecSetVolume(base, codec, volume, thread, call);
  });
}

void XAudioPcmCodecSetPitch(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t codec = ctx.r3.u32;
  const double octaves = ctx.f1.f64;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::PcmCodecSetPitch(base, codec, octaves, thread, frame, call);
  });
}

void XAudioPcmCodecSetFrequencyRatio(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t codec = ctx.r3.u32;
  const double ratio = ctx.f1.f64;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::PcmCodecSetFrequencyRatio(base, codec, ratio, thread, frame, call);
  });
}

void XAudioPcmCodecBaseSetFrequencyRatio(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8246D320 PcmCodec base's SetFrequencyRatio");
  ctx.r3.u64 = ctx.r3.u64 - 16;
  XAudioPcmCodecSetFrequencyRatio(ctx, base);
}

void XAudioXmaCodecSetVolume(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t codec = ctx.r3.u32;
  const double volume = ctx.f1.f64;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t) {
    return xa::XmaCodecSetVolume(base, codec, volume, thread, call);
  });
}

void XAudioXmaCodecSetPitch(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t codec = ctx.r3.u32;
  const double octaves = ctx.f1.f64;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::XmaCodecSetPitch(base, codec, octaves, thread, frame, call);
  });
}

void XAudioCodecOnSubmit(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec_base = ctx.r3.u32, entry = ctx.r4.u32, flags = ctx.r5.u32;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CodecOnSubmit(base, codec_base, entry, flags, thread, frame, call);
  });
}

void XAudioCodecSubmitPacket(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec_base = ctx.r3.u32, packet = ctx.r4.u32, flags = ctx.r5.u32;
  const uint32_t thread = Thread(ctx, base);
  FramedOr(ctx, base, 144, NoBlockSize(base, codec_base), __imp__sub_82469EF8,
           [&](const xa::GuestCall& call, uint32_t frame) {
             return xa::CodecSubmitPacket(base, codec_base, packet, flags, thread, frame, call);
           });
}

void XAudioCodecSubmitRegion(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec_base = ctx.r3.u32, region = ctx.r4.u32, flags = ctx.r5.u32;
  const uint32_t thread = Thread(ctx, base);
  FramedOr(ctx, base, 144, NoBlockSize(base, codec_base), __imp__sub_8246A1C0,
           [&](const xa::GuestCall& call, uint32_t frame) {
             return xa::CodecSubmitRegion(base, codec_base, region, flags, thread, frame, call);
           });
}

void XAudioPcmCodecSubmitPacket(PPCContext& ctx, uint8_t* base) {
  if (NoBlockSize(base, ctx.r3.u32 + 16)) {
    __imp__sub_8246CEB8(ctx, base);
    return;
  }
  ITC_NATIVE_RUNNING("sub_8246CEB8 PcmCodec's SubmitPacket");
  // A tail call: SubmitPacket's frame below the caller's.
  ctx.r3.u64 = ctx.r3.u64 + 16;
  XAudioCodecSubmitPacket(ctx, base);
}

void XAudioXmaCodecSubmitRegion(PPCContext& ctx, uint8_t* base) {
  const uint32_t codec = ctx.r3.u32, region = ctx.r4.u32, flags = ctx.r5.u32;
  const uint32_t thread = Thread(ctx, base);
  const bool refuses =
      guest::LoadU8(base, codec + 266) != 1 && NoBlockSize(base, codec + 16);
  FramedOr(ctx, base, 128, refuses, __imp__sub_8246A860,
           [&](const xa::GuestCall& call, uint32_t frame) {
             return xa::XmaCodecSubmitRegion(base, codec, region, flags, thread, frame, call);
           });
}

// --- The output matrix and the volume meter ---

void XAudioMatrixInit(PPCContext& ctx, uint8_t* base) {
  const uint32_t matrix = ctx.r3.u32, descriptor = ctx.r4.u32, allocator = ctx.r5.u32;
  Framed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MatrixInit(base, matrix, descriptor, allocator, frame, call);
  });
}

void XAudioMatrixSetMatrix(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t matrix = ctx.r3.u32, levels = ctx.r4.u32;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t) {
    return xa::MatrixSetMatrix(base, matrix, levels, thread, call);
  });
}

void XAudioMatrixSetLevels(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t matrix = ctx.r3.u32, levels = ctx.r4.u32, now = ctx.r5.u32;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t) {
    return xa::MatrixSetLevels(base, matrix, levels, now, thread, call);
  });
}

void XAudioMatrixMix(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t matrix = ctx.r3.u32, source = ctx.r4.u32, target = ctx.r5.u32;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 288, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MatrixMix(base, matrix, source, target, thread, frame, call);
  });
}

void XAudioMatrixMixRamp(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = xa::MatrixMixRamp(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
}

void XAudioMeterProcess(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t meter = ctx.r3.u32, buffer = ctx.r4.u32;
  Framed(ctx, base, 112, [&](const xa::GuestCall&, uint32_t frame) {
    return xa::MeterProcess(base, meter, buffer, frame);
  });
}

// --- The render driver's set-up and the reverb's small parts ---

void XAudioRenderDriverSetUp(PPCContext& ctx, uint8_t* base) {
  uint32_t r4 = 0;
  const uint32_t function = xa::RenderDriverSetUpThunk(base, ctx.r3.u32, ctx.r4.u32, &r4);
  ctx.r4.u64 = r4;
  ctx.r11.u64 = function;
  REX_CALL_INDIRECT_FUNC(function);
}

void XAudioReverbInnerDestroy(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82465410 the reverb's inner destructor");
  ctx.r3.u64 = ctx.r3.u64 - 4;
  xa::ReverbDestruct(base, ctx.r3.u32);
}

void XAudioReverbDestruct(PPCContext& ctx, uint8_t* base) { xa::ReverbDestruct(base, ctx.r3.u32); }

void XAudioReverbDefaults(PPCContext& ctx, uint8_t* base) { xa::ReverbDefaults(base, ctx.r3.u32); }

void XAudioLargestPrimeBelow(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::LargestPrimeBelow(base, ctx.r3.u32);
}

void XAudioFilterCoefficient(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  xa::FilterCoefficient(base, ctx.r3.u32);
}

void XAudioReverbSetParameters(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t reverb = ctx.r3.u32, kind = ctx.r4.u32, data = ctx.r6.u32;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 240, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::ReverbSetParameters(base, reverb, kind, data, thread, frame, call);
  });
}

void XAudioI3dl2ToNative(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = xa::I3dl2ToNative(base, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 128);
}

void XAudioPcmCodecProcess(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t codec = ctx.r3.u32;
  const uint64_t iface = ctx.r5.u64;
  const uint32_t thread = Thread(ctx, base);
  Framed(ctx, base, 192, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::PcmCodecProcess(base, codec, iface, thread, frame, call);
  });
}

void XAudioResamplerProcess(PPCContext& ctx, uint8_t* base) {
  const uint32_t state = ctx.r3.u32;
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::ResamplerProcess(base, state, frame, call);
  });
}

void XAudioReverbLowPass(PPCContext& ctx, uint8_t* base) {
  xa::ReverbLowPass(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

#define ITC_XAUDIO_DELAY_STEP(name, function)                                      \
  void name(PPCContext& ctx, uint8_t* base) {                                      \
    xa::function(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);            \
  }
ITC_XAUDIO_DELAY_STEP(XAudioReverbDelay512, ReverbDelay512)
ITC_XAUDIO_DELAY_STEP(XAudioReverbDelay2048, ReverbDelay2048)
ITC_XAUDIO_DELAY_STEP(XAudioReverbDelay1024, ReverbDelay1024)
#undef ITC_XAUDIO_DELAY_STEP

void XAudioReverbAllPass128(PPCContext& ctx, uint8_t* base) {
  xa::ReverbAllPass128(base, ctx.r3.u32, ctx.r4.u32);
}
void XAudioReverbAllPass256(PPCContext& ctx, uint8_t* base) {
  xa::ReverbAllPass256(base, ctx.r3.u32, ctx.r4.u32);
}
void XAudioReverbAllPass512(PPCContext& ctx, uint8_t* base) {
  xa::ReverbAllPass512(base, ctx.r3.u32, ctx.r4.u32);
}
void XAudioReverbAllPass512To(PPCContext& ctx, uint8_t* base) {
  xa::ReverbAllPass512To(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
void XAudioReverbCrossfadeDelay(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::ReverbCrossfadeDelay(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}
void XAudioReverbCrossfadeDelay256(PPCContext& ctx, uint8_t* base) {
  xa::ReverbCrossfadeDelay256(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}
// The frame below r1 (the original's 17,888 bytes) is written as the
// original leaves it; r3 as it does.
void XAudioReverbCoreProcess(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::ReverbCoreProcess(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

// In the original's frame (432 bytes; memcpy's original, where its plain
// C++ refuses, runs below it); r3 as the original leaves it.
void XAudioReverbCoreApply(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t core = ctx.r3.u32, params = ctx.r4.u32;
  Framed(ctx, base, 432, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::ReverbCoreApply(base, core, params, frame, call);
  });
}

void XAudioReverbCoreInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = xa::ReverbCoreInit(base, ctx.r3.u32);
}

// In the original's frames (112, 128 and 128 bytes), with their back chains.
void XAudioI3dl2ParamsConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t object = ctx.r3.u32, flag = ctx.r4.u32;
  Framed(ctx, base, 112, [&](const xa::GuestCall&, uint32_t frame) {
    return xa::I3dl2ParamsConstruct(base, object, flag, frame);
  });
}

void XAudioReverbConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t reverb = ctx.r3.u64;
  const uint32_t descriptor = ctx.r4.u32;
  Framed(ctx, base, 128, [&](const xa::GuestCall&, uint32_t frame) {
    xa::ReverbConstruct(base, uint32_t(reverb), descriptor, frame);
    return reverb;
  });
}

void XAudioReverbCreate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t descriptor = ctx.r3.u32, allocator = ctx.r4.u32, out = ctx.r5.u32;
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::ReverbCreate(base, descriptor, allocator, out, frame, call);
  });
}

#define ITC_XAUDIO_KERNEL_STEP(name, function)                     \
  void name(PPCContext& ctx, uint8_t* base) {                      \
    ctx.r3.u64 = xa::function(base, ctx.r3.u32, ctx.r1.u32);       \
  }
ITC_XAUDIO_KERNEL_STEP(XAudioResampleMono, ResampleMono)
ITC_XAUDIO_KERNEL_STEP(XAudioResampleStereo, ResampleStereo)
ITC_XAUDIO_KERNEL_STEP(XAudioResampleMonoScalar, ResampleMonoScalar)
ITC_XAUDIO_KERNEL_STEP(XAudioResampleStereoScalar, ResampleStereoScalar)
ITC_XAUDIO_KERNEL_STEP(XAudioCopyStereo, CopyStereo)
ITC_XAUDIO_KERNEL_STEP(XAudioCopyStereoScalar, CopyStereoScalar)
#undef ITC_XAUDIO_KERNEL_STEP

#define ITC_XAUDIO_FILL_STEP(name, groups)                       \
  void name(PPCContext& ctx, uint8_t* base) {                    \
    xa::FillDelayLine(base, ctx.r3.u32, groups);                 \
    ctx.r11.u64 = 0;                                             \
  }
ITC_XAUDIO_FILL_STEP(XAudioFillDelayLine2048, 2048)
ITC_XAUDIO_FILL_STEP(XAudioFillDelayLine64, 64)
ITC_XAUDIO_FILL_STEP(XAudioFillDelayLine16, 16)
ITC_XAUDIO_FILL_STEP(XAudioFillDelayLine256, 256)
ITC_XAUDIO_FILL_STEP(XAudioFillDelayLine32, 32)
ITC_XAUDIO_FILL_STEP(XAudioFillDelayLine128, 128)
ITC_XAUDIO_FILL_STEP(XAudioFillDelayLine512, 512)
#undef ITC_XAUDIO_FILL_STEP

}  // namespace hooks

#define ITC_XAUDIO_OBJECTS_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {            \
    if (!ITC_FLAG(native_xaudio_objects)) {       \
      __imp__sub_##address(ctx, base);            \
      return;                                     \
    }                                             \
    hooks::step(ctx, base);                       \
  }

ITC_XAUDIO_OBJECTS_ADAPTER(82466B78, XAudioCodecSize)
ITC_XAUDIO_OBJECTS_ADAPTER(82466840, XAudioMatrixSize)
ITC_XAUDIO_OBJECTS_ADAPTER(82463B78, XAudioReverbSize)
ITC_XAUDIO_OBJECTS_ADAPTER(825A60E0, XAudioMeterSize)
ITC_XAUDIO_OBJECTS_ADAPTER(82466E80, XAudioCodecGetInfo)
ITC_XAUDIO_OBJECTS_ADAPTER(82465BE0, XAudioMatrixGetInfo)
ITC_XAUDIO_OBJECTS_ADAPTER(825A5778, XAudioMeterGetInfo)
ITC_XAUDIO_OBJECTS_ADAPTER(8246A920, XAudioCodecGetState)
ITC_XAUDIO_OBJECTS_ADAPTER(82465C08, XAudioMatrixSetOverwrite)
ITC_XAUDIO_OBJECTS_ADAPTER(824679F8, XAudioReverbSetStereo)
ITC_XAUDIO_OBJECTS_ADAPTER(8246CE70, XAudioObjectRelease)
ITC_XAUDIO_OBJECTS_ADAPTER(82466BA0, XAudioCodecCreate)
ITC_XAUDIO_OBJECTS_ADAPTER(82466EE8, XAudioCodecDestruct)
ITC_XAUDIO_OBJECTS_ADAPTER(8246CE68, XAudioPcmCodecInnerDestroy)
ITC_XAUDIO_OBJECTS_ADAPTER(8246A918, XAudioXmaCodecInnerDestroy)
ITC_XAUDIO_OBJECTS_ADAPTER(82465BD8, XAudioMatrixInnerDestroy)
ITC_XAUDIO_OBJECTS_ADAPTER(82466888, XAudioMatrixCreate)
ITC_XAUDIO_OBJECTS_ADAPTER(825A6460, XAudioMeterCreate)
ITC_XAUDIO_OBJECTS_ADAPTER(825A6138, XAudioMeterInit)
ITC_XAUDIO_OBJECTS_ADAPTER(82467078, XAudioCodecSetFlags)
ITC_XAUDIO_OBJECTS_ADAPTER(824674D8, XAudioCodecCompletePacket)
ITC_XAUDIO_OBJECTS_ADAPTER(824699E0, XAudioCodecFlushPackets)
ITC_XAUDIO_OBJECTS_ADAPTER(8246CEC8, XAudioCodecFlush)
ITC_XAUDIO_OBJECTS_ADAPTER(8246CFA8, XAudioPcmCodecSetFormat)
ITC_XAUDIO_OBJECTS_ADAPTER(8246D410, XAudioPcmCodecStart)
ITC_XAUDIO_OBJECTS_ADAPTER(8246D500, XAudioPcmCodecStop)
ITC_XAUDIO_OBJECTS_ADAPTER(8246D630, XAudioCodecEndOfStream)
ITC_XAUDIO_OBJECTS_ADAPTER(8246D008, XAudioPcmCodecSetVolume)
ITC_XAUDIO_OBJECTS_ADAPTER(8246D0C8, XAudioPcmCodecSetPitch)
ITC_XAUDIO_OBJECTS_ADAPTER(8246D320, XAudioPcmCodecBaseSetFrequencyRatio)
ITC_XAUDIO_OBJECTS_ADAPTER(8246AB48, XAudioXmaCodecSetVolume)
ITC_XAUDIO_OBJECTS_ADAPTER(8246AC40, XAudioXmaCodecSetPitch)
ITC_XAUDIO_OBJECTS_ADAPTER(8246CEB8, XAudioPcmCodecSubmitPacket)
ITC_XAUDIO_OBJECTS_ADAPTER(8246A860, XAudioXmaCodecSubmitRegion)
ITC_XAUDIO_OBJECTS_ADAPTER(82465C30, XAudioMatrixInit)
ITC_XAUDIO_OBJECTS_ADAPTER(82465E50, XAudioMatrixSetMatrix)
ITC_XAUDIO_OBJECTS_ADAPTER(82466188, XAudioMatrixSetLevels)
ITC_XAUDIO_OBJECTS_ADAPTER(82466440, XAudioMatrixMix)
ITC_XAUDIO_OBJECTS_ADAPTER(825A6210, XAudioMeterProcess)
ITC_XAUDIO_OBJECTS_ADAPTER(824657C8, XAudioRenderDriverSetUp)
ITC_XAUDIO_OBJECTS_ADAPTER(82465410, XAudioReverbInnerDestroy)
ITC_XAUDIO_OBJECTS_ADAPTER(82467538, XAudioReverbDefaults)
ITC_XAUDIO_OBJECTS_ADAPTER(8246DFE8, XAudioLargestPrimeBelow)
ITC_XAUDIO_OBJECTS_ADAPTER(824675B8, XAudioFilterCoefficient)
ITC_XAUDIO_OBJECTS_ADAPTER(82467720, XAudioFillDelayLine2048)
ITC_XAUDIO_OBJECTS_ADAPTER(82467788, XAudioFillDelayLine64)
ITC_XAUDIO_OBJECTS_ADAPTER(824677F0, XAudioFillDelayLine16)
ITC_XAUDIO_OBJECTS_ADAPTER(82467858, XAudioFillDelayLine256)
ITC_XAUDIO_OBJECTS_ADAPTER(824678C0, XAudioFillDelayLine32)
ITC_XAUDIO_OBJECTS_ADAPTER(82467928, XAudioFillDelayLine128)
ITC_XAUDIO_OBJECTS_ADAPTER(82467990, XAudioFillDelayLine512)
ITC_XAUDIO_OBJECTS_ADAPTER(8246F140, XAudioResamplerProcess)
ITC_XAUDIO_OBJECTS_ADAPTER(8247E188, XAudioResampleMono)
ITC_XAUDIO_OBJECTS_ADAPTER(8247E260, XAudioResampleStereo)
ITC_XAUDIO_OBJECTS_ADAPTER(8247F558, XAudioCopyStereo)
ITC_XAUDIO_OBJECTS_ADAPTER(82463B90, XAudioReverbCoreProcess)
ITC_XAUDIO_OBJECTS_ADAPTER(82462148, XAudioReverbSetParameters)
ITC_XAUDIO_OBJECTS_ADAPTER(82469418, XAudioI3dl2ToNative)
ITC_XAUDIO_OBJECTS_ADAPTER(8246D860, XAudioPcmCodecProcess)
ITC_XAUDIO_OBJECTS_ADAPTER(82467A18, XAudioReverbCoreApply)
ITC_XAUDIO_OBJECTS_ADAPTER(82468848, XAudioReverbCoreInit)
ITC_XAUDIO_OBJECTS_ADAPTER(82469848, XAudioI3dl2ParamsConstruct)
ITC_XAUDIO_OBJECTS_ADAPTER(82465608, XAudioReverbConstruct)
ITC_XAUDIO_OBJECTS_ADAPTER(82465710, XAudioReverbCreate)
