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

ITC_FLAG_BOOL(native_xaudio_objects, true, "ITC",
              "Run the parts of XAudio's factory objects made native "
              "(docs/functions/xaudio-objects.md: the class tables' sizes XAudioCodec::Size "
              "sub_82466B78 with PcmCodec::Size and XmaCodec::Size, XAudioMatrix::Size "
              "sub_82466840, Reverb::Size sub_82463B78 and VolumeMeter::Size sub_825A60E0; the "
              "GetInfo slots sub_82466E80, sub_82465BE0 and sub_825A5778, XAudioCodec::GetState "
              "sub_8246A920, XAudioMatrix::SetOverwrite sub_82465C08, ReverbCore::SetStereo "
              "sub_824679F8 and XAudioObject::Release sub_8246CE70; the codecs' construction "
              "XAudioCodec::Create sub_82466BA0 with the PCM and XMA codecs' Create and "
              "constructors, XAudioCodec::Construct and Init, CodecBase::CodecBase and Init "
              "sub_82469978 and the packet lists' Init and Alloc; their destruction "
              "XAudioCodec::Destruct sub_82466EE8, the inner objects' destructors sub_8246CE68, "
              "sub_8246A918 and sub_82465BD8 with PcmCodec::~PcmCodec, XmaCodec::~XmaCodec (the "
              "XMA object's release stays the translated code's) and XAudioMatrix::Destruct, "
              "the packet lists' Clear and Next; XAudioMatrix::Create sub_82466888, "
              "VolumeMeter::Create sub_825A6460 and Init sub_825A6138) natively instead of through "
              "the translated code (the same reads and writes; checked by "
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
ITC_XAUDIO_OBJECTS_ADAPTER(82469978, XAudioCodecBaseInit)
ITC_XAUDIO_OBJECTS_ADAPTER(82466EE8, XAudioCodecDestruct)
ITC_XAUDIO_OBJECTS_ADAPTER(8246CE68, XAudioPcmCodecInnerDestroy)
ITC_XAUDIO_OBJECTS_ADAPTER(8246A918, XAudioXmaCodecInnerDestroy)
ITC_XAUDIO_OBJECTS_ADAPTER(82465BD8, XAudioMatrixInnerDestroy)
ITC_XAUDIO_OBJECTS_ADAPTER(82466888, XAudioMatrixCreate)
ITC_XAUDIO_OBJECTS_ADAPTER(825A6460, XAudioMeterCreate)
ITC_XAUDIO_OBJECTS_ADAPTER(825A6138, XAudioMeterInit)
