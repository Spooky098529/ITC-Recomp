// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 21's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"

#include "accessories/accessories.h"
#include "app_start_up/start_up.h"
#include "hooks/accessories.h"
#include "hooks/app_states.h"
#include "hooks/last_error.h"
#include "hooks/models.h"
#include "hooks/post_effects.h"
#include "hooks/race_loading.h"
#include "hooks/xaudio.h"
#include "hooks/xaudio_engine.h"
#include "hooks/xaudio_objects.h"
#include "lifted/originals.h"
#include "post_effects/post_effects.h"
#include "race_loading/race_loading.h"
#include "race_overlays/race_overlays.h"
#include "sound_cues/sound_cues.h"
#include "xaudio/objects.h"

// --- XAudio: the register steps (hooks/xaudio.h, xaudio_engine.h,
// xaudio_objects.h) ---

// XAudioVoice::SetEffectParameters, the API's.
ITC_DIRECT_ENTRY(82459E80) { hooks::XAudioApiSetEffectParameters(ctx); }
// XAudioSourceVoice::SubmitPacket.
ITC_DIRECT_ENTRY(8245A050) { hooks::XAudioApiSubmitPacket(ctx); }
// XAudioVoice::Start, the API's.
ITC_DIRECT_ENTRY(8245A168) { hooks::XAudioApiStart(ctx); }
// XAudioVoice::Stop, the API's.
ITC_DIRECT_ENTRY(8245A1B0) { hooks::XAudioApiStop(ctx); }
// XAudioSourceVoice::FlushPackets.
ITC_DIRECT_ENTRY(8245A208) { hooks::XAudioApiFlushPackets(ctx); }
// XAudio::Alloc: r4 = the size, r5 = the attributes (the original without
// the native heap).
ITC_DIRECT_ENTRY(8245EF80) { hooks::XAudioAlloc(ctx); }
// XAudio::Free: r4 = the block, r5 = the attributes (the original without
// the native heap).
ITC_DIRECT_ENTRY(8245EF90) { hooks::XAudioFree(ctx); }
// XAudioBuffer::ReleaseThunk.
ITC_DIRECT_ENTRY(8245F6D0) { hooks::XAudioBufferReleaseThunk(ctx); }
// XAudio's allocator's destructor.
ITC_DIRECT_ENTRY(824602F8) { hooks::XAudioAllocatorDestroy(ctx); }
// XAudioVoice::SetFormat: r3 = the voice, r4 = the format, r5 = the outputs.
ITC_DIRECT_ENTRY(82460778) { hooks::XAudioSetFormat(ctx); }
// The voice's outputs destroyed.
ITC_DIRECT_ENTRY(82460B50) { hooks::XAudioOutputsDestroy(ctx); }
// XAudioVoice::Destroy.
ITC_DIRECT_ENTRY(82461240) { hooks::XAudioDestroy(ctx); }
// PacketList::Next: r3 = the list, r4 = the entry, r5 = the direction.
ITC_DIRECT_ENTRY(82466EA0) { hooks::XAudioPacketListNext(ctx); }
// CodecBase::~CodecBase: r3 = the codec base; r3 as PacketLists::Clear
// leaves it.
ITC_DIRECT_ENTRY(82469960) { ctx.r3.u64 = xaudio::CodecBaseDestruct(ctx.r3.u32); }
// CodecBase::SetFormat.
ITC_DIRECT_ENTRY(82469E58) { hooks::XAudioCodecBaseSetFormat(ctx); }
// CodecBase::SubmitPacket (the original where the port declines).
ITC_DIRECT_ENTRY(82469EF8) { hooks::XAudioCodecSubmitPacket(ctx); }
// CodecBase::SubmitRegion (the original where the port declines).
ITC_DIRECT_ENTRY(8246A1C0) { hooks::XAudioCodecSubmitRegion(ctx); }
// XmaCodec::~XmaCodec: r3 = the object (left in r3, in full); the release
// call from the original's 96-byte frame.
ITC_DIRECT_ENTRY(8246AAD0) {
  const uint64_t object = ctx.r3.u64;
  uint64_t r3 = object;
  hooks::XAudioFramed(ctx, 96, [&](const xaudio::GuestCall& call, uint32_t frame) {
    r3 = xaudio::XmaCodecDestroy(object, frame, call);
    return uint32_t(r3);
  });
  ctx.r3.u64 = r3;
}
// PcmCodec::~PcmCodec: r3 = the object (left in r3, in full).
ITC_DIRECT_ENTRY(8246CF50) { ctx.r3.u64 = xaudio::PcmCodecDestroy(ctx.r3.u64); }
// PacketList::Clear.
ITC_DIRECT_ENTRY(8246E060) { hooks::XAudioPacketListClear(ctx); }
// PacketLists::Clear: r3 = the lists; r3 as the second Clear leaves it.
ITC_DIRECT_ENTRY(8246E0E8) { ctx.r3.u64 = xaudio::PacketListsClear(ctx.r3.u32); }

// --- The app's states: the register steps (hooks/app_states.h) ---

// List::Clear (the original without the native heap).
ITC_DIRECT_ENTRY(820ACF88) { hooks::AppListClear(ctx); }
// The operation list's node bought (the original where the port declines).
ITC_DIRECT_ENTRY(820ACFF8) { hooks::AppOpBuyNode(ctx); }
// PtrList::ForEach.
ITC_DIRECT_ENTRY(820AD3A0) { hooks::AppPtrListForEach(ctx); }
// StateLayer::OnToggleOn.
ITC_DIRECT_ENTRY(820AD4E0) { hooks::AppLayerToggleOn(ctx); }
// The operation list's size increased (the original where the port declines).
ITC_DIRECT_ENTRY(820AD768) { hooks::AppOpIncSize(ctx); }

// --- The accessories' destructors: the removed adapters' checks, else the
// originals ---

// Accessory::~Accessory: r3 = the accessory.
ITC_DIRECT_ENTRY(820EF310) {
  if (!hooks::AccessoriesNativeLoads() || !hooks::ModelsNativeLifetime() ||
      !accessories::AccessoryDestructible(ctx.r3.u32)) {
    lifted::original::c820EF310(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = accessories::AccessoryDestruct(ctx.r3.u32);
}
// AccessorySkeleton::~AccessorySkeleton: r3 = the skeleton.
ITC_DIRECT_ENTRY(820EFFD8) {
  if (!hooks::AccessoriesNativeLoads() || !hooks::ModelsNativeLifetime() ||
      !accessories::SkeletonDestructible(ctx.r3.u32)) {
    lifted::original::c820EFFD8(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();  // as SkeletonBase::~SkeletonBase's adapter
  ctx.r3.u64 = accessories::SkeletonDestruct(ctx.r3.u32);
}
// AccessoriesBase::~AccessoriesBase: r3 = the set.
ITC_DIRECT_ENTRY(820F0B58) {
  if (!hooks::AccessoriesNativeLoads() || !hooks::ModelsNativeLifetime() ||
      !accessories::SetReleasable(ctx.r3.u32)) {
    lifted::original::c820F0B58(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = accessories::SetBaseDestruct(ctx.r3.u32);
}
// AccessoryFile::~AccessoryFile: r3 = the file.
ITC_DIRECT_ENTRY(820F0DB8) {
  if (!hooks::AccessoriesNativeLoads() || !accessories::FileDestructible(ctx.r3.u32)) {
    lifted::original::c820F0DB8(ctx);
    return;
  }
  ctx.r3.u64 = accessories::FileDestruct(ctx.r3.u32);
}

// --- The post effects ---

// RenderTextures::Acquire: r3 = the id, r4 = the format, r5, r6 = the size,
// r7, r8 = the key (r1 = the stack).
ITC_DIRECT_ENTRY(82133190) {
  ctx.r3.u64 = post_effects::RenderTexturesAcquire(ctx.r1.u32, ctx.r3.u32, ctx.r4.u32,
                                                   ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                                                   ctx.r8.u32);
}
// RenderTextures::Release (the original where the frees aren't native).
ITC_DIRECT_ENTRY(821332F0) { hooks::PostEffectsReleaseTexture(ctx); }
// PostEffect::ReleaseSurface (the original without the native device lock).
ITC_DIRECT_ENTRY(82133C60) { hooks::PostEffectsReleaseSurface(ctx); }

// --- The sound cues' bank pairs ---

// BankPair::IsLoading: r3 = the pair.
ITC_DIRECT_ENTRY(82117670) { ctx.r3.u64 = sound_cues::BankPairIsLoading(ctx.r3.u32); }
// BankPair::SoundBank: r3 = the pair; its +76.
ITC_DIRECT_ENTRY(82117770) { ctx.r3.u64 = sound_cues::BankPairSoundBank(ctx.r3.u32); }

// --- The race's loading ---

// UiMovie::SetFrames: r3 = the movie (left in r3), r4 = the PRM and PIC
// chunks' word, r5 = the TXT chunks'.
ITC_DIRECT_ENTRY(82290B30) { race_loading::SetFrames(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32); }
// The system request's check (its step: r1 the stack).
ITC_DIRECT_ENTRY(82293460) { hooks::CheckSystemRequest(ctx); }

// --- The app's start-up ---

// The app camera's 16-byte entry constructed: r3 = the entry (returned).
ITC_DIRECT_ENTRY(820A75E8) { ctx.r3.u64 = app_start_up::AppCameraEntryConstruct(ctx.r3.u32); }

// --- The race overlays ---

// RandomRange: f1 = low, f2 = high; f1 the result (single), r3 rand()'s.
ITC_DIRECT_ENTRY(8211F118) {
  ctx.fpscr.disableFlushMode();
  uint32_t drawn = 0;
  const float value = race_overlays::RandomRange(ctx.f1.f64, ctx.f2.f64, &drawn);
  ctx.f1.f64 = double(value);
  ctx.r3.u64 = drawn;
}

// --- GetLastError ---

// GetLastError itself (r13 the PCR): r3 the error.
ITC_DIRECT_ENTRY(82296EC0) { ctx.r3.u64 = last_error::Get(ctx.r13.u32); }
