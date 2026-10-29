// XAudio's voices natively: adapters in front of src/xaudio/
// (docs/functions/xaudio.md), with the register steps the harness shares
// (hooks/xaudio.h). Many are reached through the voices' vtables, so these
// adapters stay until their callers are native.

#include "hooks/xaudio.h"

#include "base/flags.h"
#include "xaudio/voices.h"

ITC_FLAG_BOOL(native_xaudio, true, "ITC",
              "Run the parts of XAudio's voices made native (docs/functions/xaudio.md: "
              "XAudio::SourceOutputFormat sub_8245DC88, XAudioSubmixVoice::Describe "
              "sub_8245F010, XAudio::DefaultMatrix sub_8245EC40, XAudioBuffer::Size "
              "sub_8245FA28 and Clear sub_8245F8E8, XAudioVoice::EngineBuffer sub_8245DEB0, "
              "GetType sub_8245E318, GetFormat sub_8245E328, SetFormatWords sub_82461140, "
              "AddRef sub_8245F860, sub_8245FB00 and XAudio::AddStat sub_8245F640) natively "
              "instead of through the translated code (the same reads and writes; XAudio's two "
              "locks stay the game's; checked by --run_harnesses=xaudio)");

namespace hooks {

namespace xa = xaudio;

void XAudioSourceOutputFormat(PPCContext& ctx, uint8_t* base) {
  xa::SourceOutputFormat(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioSubmixDescribe(PPCContext& ctx, uint8_t* base) {
  xa::SubmixDescribe(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioDefaultMatrix(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::DefaultMatrix(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void XAudioBufferSize(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = xa::BufferSize(base, ctx.r3.u32, ctx.r4.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_8245FA28(ctx, base);
}

void XAudioBufferClear(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = xa::BufferClear(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_8245F8E8(ctx, base);
}

void XAudioEngineBuffer(PPCContext& ctx, uint8_t* base) {
  const uint32_t processor = guest::LoadU8(base, ctx.r13.u32 + xa::kPcrProcessor);
  ctx.r3.u64 = xa::EngineBuffer(base, processor);
}

void XAudioGetType(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::GetType(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioGetFormat(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::GetFormat(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioSetFormatWords(PPCContext& ctx, uint8_t* base) {
  xa::SetFormatWords(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioAddRef(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::AddRef(base, ctx.r3.u32);
}

void XAudioSetRootVtable(PPCContext& ctx, uint8_t* base) {
  xa::SetRootVtable(base, ctx.r3.u32);
}

void XAudioAddStat(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::AddStat(base, ctx.r3.u32, ctx.r4.u32);
}

}  // namespace hooks

#define ITC_XAUDIO_ADAPTER(address, step)   \
  extern "C" REX_FUNC(sub_##address) {      \
    if (!ITC_FLAG(native_xaudio)) {         \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

ITC_XAUDIO_ADAPTER(8245DC88, XAudioSourceOutputFormat)
ITC_XAUDIO_ADAPTER(8245F010, XAudioSubmixDescribe)
ITC_XAUDIO_ADAPTER(8245EC40, XAudioDefaultMatrix)
ITC_XAUDIO_ADAPTER(8245FA28, XAudioBufferSize)
ITC_XAUDIO_ADAPTER(8245F8E8, XAudioBufferClear)
ITC_XAUDIO_ADAPTER(8245DEB0, XAudioEngineBuffer)
ITC_XAUDIO_ADAPTER(8245E318, XAudioGetType)
ITC_XAUDIO_ADAPTER(8245E328, XAudioGetFormat)
ITC_XAUDIO_ADAPTER(82461140, XAudioSetFormatWords)
ITC_XAUDIO_ADAPTER(8245F860, XAudioAddRef)
ITC_XAUDIO_ADAPTER(8245FB00, XAudioSetRootVtable)
ITC_XAUDIO_ADAPTER(8245F640, XAudioAddStat)
