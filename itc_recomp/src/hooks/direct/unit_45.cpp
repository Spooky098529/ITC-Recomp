// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 45's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "accessories/accessories.h"
#include "guest.h"
#include "hooks/accessories.h"
#include "hooks/archive.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "hooks/race_world.h"
#include "hooks/scene_target.h"
#include "hooks/xact_tracks.h"
#include "hooks/xaudio.h"
#include "native/buffers.h"
#include "scene_target/scene_target.h"
#include "xact/waves.h"
#include "xact/xact.h"

// --- xact ---
// As the area's adapters: flush to zero off (its floats moved as the
// original's lfs and stfs); ports that can decline run the original.

// XACTEngine::FindWaveBank: r3 = the engine, r4 = the name; r3 the bank or 0.
ITC_DIRECT_ENTRY(825C0360) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = xact::EngineFindWaveBank(ctx.r3.u32, ctx.r4.u32);
  if (!r3) {
    lifted::original::c825C0360(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// A category's effective volume: r3 = the engine, r4 = the index; f1 the
// volume, r3 as the original leaves it.
ITC_DIRECT_ENTRY(825C04D8) {
  ctx.fpscr.disableFlushMode();
  const auto result = xact::EngineGetCategoryVolume(ctx.r3.u32, ctx.r4.u32);
  if (!result) {
    lifted::original::c825C04D8(ctx);
    return;
  }
  ctx.f1.f64 = result->f1;
  ctx.r3.u64 = result->r3;
}
// XACTVariable::~XACTVariable: r3 = the variable; r3 FreeChunks'. Frees:
// the original unless the game's heap is the native layer.
ITC_DIRECT_ENTRY(825C2128) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c825C2128(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = xact::VariableDestruct(ctx.r3.u32);
}
// XACTGlobalSettingsBase::XACTGlobalSettingsBase: r3 = the settings, r4 =
// the engine; r3 left.
ITC_DIRECT_ENTRY(825C3508) {
  ctx.fpscr.disableFlushMode();
  xact::GlobalSettingsBaseConstruct(ctx.r3.u32, ctx.r4.u32);
}
// XACTGlobalSettings::MakeState: r3 = the settings; r3 0 or E_OUTOFMEMORY.
// Allocates: the original unless the game's heap is the native layer.
ITC_DIRECT_ENTRY(825C3B68) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = hooks::NativeHeapActive() ? xact::SettingsMakeState(ctx.r3.u32)
                                            : std::nullopt;
  if (!r3) {
    lifted::original::c825C3B68(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// XACTGlobalSettings::AddInstance and RemoveInstance: r3 = the settings, r4
// = the category; r3 as the original leaves it.
ITC_DIRECT_ENTRY(825C3F90) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = xact::SettingsAddInstance(ctx.r3.u32, ctx.r4.u32);
  if (!r3) {
    lifted::original::c825C3F90(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
ITC_DIRECT_ENTRY(825C4000) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = xact::SettingsRemoveInstance(ctx.r3.u32, ctx.r4.u32);
  if (!r3) {
    lifted::original::c825C4000(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// XACTSoundBank::ForgetWaveBank: r3 = the sound bank, r4 = the wave bank; r3
// the bank (no array) or the header.
ITC_DIRECT_ENTRY(825C72B0) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = xact::SoundBankForgetWaveBank(ctx.r3.u32, ctx.r4.u32);
  if (!r3) {
    lifted::original::c825C72B0(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// XACTSoundBank::InitInstanceCounts: r3 = the bank; r3 0 or E_OUTOFMEMORY.
// Allocates: the original unless the game's heap is the native layer.
ITC_DIRECT_ENTRY(825C7448) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = hooks::NativeHeapActive() ? xact::SoundBankInitInstanceCounts(ctx.r3.u32)
                                            : std::nullopt;
  if (!r3) {
    lifted::original::c825C7448(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// XACTSoundBankBase::XACTSoundBankBase and XACTWaveBankBase::
// XACTWaveBankBase: r3 = the bank, r4 = the engine; r3 the bank.
ITC_DIRECT_ENTRY(825C76A0) {
  ctx.fpscr.disableFlushMode();
  const uint32_t bank = ctx.r3.u32;
  xact::SoundBankBaseConstruct(bank, ctx.r4.u32);
  ctx.r3.u64 = bank;
}
ITC_DIRECT_ENTRY(825CEBE0) {
  ctx.fpscr.disableFlushMode();
  const uint32_t bank = ctx.r3.u32;
  xact::WaveBankBaseConstruct(bank, ctx.r4.u32);
  ctx.r3.u64 = bank;
}
// XACTSoundBank::GetSimpleCues: r3 = the bank; r3 the records.
ITC_DIRECT_ENTRY(825C7AB0) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = xact::SoundBankGetSimpleCues(ctx.r3.u32);
  if (!r3) {
    lifted::original::c825C7AB0(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// XACTWaveEntry::GetLoopRegion: r3 = the entry; r3 = entry + 28.
ITC_DIRECT_ENTRY(825CEDF8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = xact::WaveEntryGetLoopRegion(ctx.r3.u32);
}
// XACTWaveEntry::SamplesToMs: r3 = the entry, r4 = the samples; r3 the
// milliseconds. The original where the rate is 0 (it traps).
ITC_DIRECT_ENTRY(825CEE70) {
  ctx.fpscr.disableFlushMode();
  uint32_t ms = 0;
  if (!xact::WaveEntrySamplesToMs(ctx.r3.u32, ctx.r4.u32, &ms)) {
    lifted::original::c825CEE70(ctx);
    return;
  }
  ctx.r3.u64 = ms;
}
// XACTWaveEntry::MsToSamples: r3 = the entry, r4 = the milliseconds; r3 the
// samples.
ITC_DIRECT_ENTRY(825CEEA0) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = xact::WaveEntryMsToSamples(ctx.r3.u32, ctx.r4.u32);
}
// XACTVoice::Init: r3 = the voice, r4 = the source voice's description, r5
// = the reverb submix's holder, r6 = &the reverb send level, r7 = the reverb
// flag; r3 the last call's. In its 128-byte frame, as XACTVoice::Create's
// plain C++ calls it.
ITC_DIRECT_ENTRY(825C9B60) {
  const uint32_t voice = ctx.r3.u32, description = ctx.r4.u32, holder = ctx.r5.u32,
                 level = ctx.r6.u32, reverb = ctx.r7.u32;
  const xaudio::Thread thread = hooks::XAudioThreadOf(ctx);
  hooks::XAudioFramed(ctx, 128, [&](const xaudio::GuestCall& call, uint32_t frame) {
    return xact::VoiceInit(voice, description, holder, level, reverb, thread, frame, call);
  });
}
// The tracks' register steps (hooks/xact_tracks.h).
ITC_DIRECT_ENTRY(825D0110) { hooks::XactTrackDestruct(ctx); }
ITC_DIRECT_ENTRY(825D53D8) { hooks::XactTrackInstancePrepareSimple(ctx); }
ITC_DIRECT_ENTRY(825D5838) { hooks::XactTrackInstanceDestruct(ctx); }
ITC_DIRECT_ENTRY(825D82E8) { hooks::XactPlayWaveConstruct(ctx); }

// --- accessories ---
// As the deleted adapters, each with the original where its plain C++
// can't run (hooks/accessories.h).

// Accessory::Finish (its register step).
ITC_DIRECT_ENTRY(820EF368) { hooks::AccessoryFinish(ctx); }
// AccessorySkeleton::Reset: r3 = the skeleton, its 192-byte frame below r1;
// r3 memcpy's.
ITC_DIRECT_ENTRY(820EFB30) {
  if (!native::Buffers_NativeMemcpy()) {
    lifted::original::c820EFB30(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = accessories::SkeletonReset(ctx.r3.u32, ctx.r1.u32);
}
// AccessorySkeleton::SetUp: r3 = the skeleton; r3 = +260.
ITC_DIRECT_ENTRY(820F0070) {
  if (!hooks::ModelsNativeLifetime() || !native::Buffers_NativeMemcpy()) {
    lifted::original::c820F0070(ctx);
    return;
  }
  const auto r3 = accessories::SkeletonSetUp(ctx.r3.u32, ctx.r1.u32);
  if (!r3) {
    lifted::original::c820F0070(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *r3;
}
// AccessoryEntry::Destroy: r3 = the entry's slot; r3 FreeData's, or the slot.
ITC_DIRECT_ENTRY(820F0988) {
  if (!hooks::AccessoriesNativeLoads() || !hooks::ModelsNativeLifetime() ||
      !accessories::EntryDestroyable(ctx.r3.u32)) {
    lifted::original::c820F0988(ctx);
    return;
  }
  if (guest::LoadU32(ctx.r3.u32) != 0) {
    ctx.fpscr.disableFlushMode();
  }
  ctx.r3.u64 = accessories::EntryDestroy(ctx.r3.u32);
}
// Accessories::AcquireEntry: r3 = the set, r4 = the index, r5 = the texture
// index; r3 the accessory or 0. Under the same conditions as Acquire's step.
ITC_DIRECT_ENTRY(820F09E0) {
  if (!hooks::AccessoriesAcquireNative()) {
    lifted::original::c820F09E0(ctx);
    return;
  }
  const uint32_t set = ctx.r3.u32;
  const int32_t index = ctx.r4.s32;
  if (index != 3 && guest::LoadU32(set + ((uint32_t(index + 1) << 2) & 0xFFFFFFFCu)) == 0) {
    ctx.fpscr.disableFlushMode();  // as AccessoryEntry::New's register step leaves it
  }
  ctx.r3.u64 = accessories::SetAcquireEntry(set, index, ctx.r5.u32, ctx.r1.u32,
                                            hooks::NativeArchiveLoad(),
                                            hooks::AccessoryTextureSet(ctx));
}
// Accessories::Release (its register step).
ITC_DIRECT_ENTRY(820F0A90) { hooks::AccessoriesRelease(ctx); }
// AccessoryFile::SetData: r3 = the file, r4 = the data, r5 = owned; r3 left.
ITC_DIRECT_ENTRY(820F0BF0) { accessories::FileSetData(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32); }
// AccessoryFile::Reset: r3 = the file; r3 Cancel's or FreeData's, or left.
ITC_DIRECT_ENTRY(820F0D30) {
  if (!hooks::AccessoriesNativeLoads() || !accessories::FileResetNative(ctx.r3.u32)) {
    lifted::original::c820F0D30(ctx);
    return;
  }
  if (const auto r3 = accessories::FileReset(ctx.r3.u32)) {
    ctx.r3.u64 = *r3;
  }
}
// Accessories::New: no arguments; r3 the set or 0.
ITC_DIRECT_ENTRY(820F0F48) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c820F0F48(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = accessories::SetNew();
}
// Accessories::Free: r3 = the set's slot; r3 0 or FreeData's. The set's
// destructor a guest call from the frame below r1.
ITC_DIRECT_ENTRY(820F0F90) {
  if (!hooks::NativeHeapActive() || !hooks::AccessoriesSetFreeable(ctx.r3.u32)) {
    lifted::original::c820F0F90(ctx);
    return;
  }
  ctx.r3.u64 =
      accessories::SetFree(ctx.r3.u32, ctx.r1.u32, hooks::AccessoriesSetDestructor(ctx));
}

// --- scene_target ---
// (TexturedRect::RecordFlat sub_8217AA98's is in hooks/scene_target.cpp.)

// SceneTarget::RecordTargets and RecordTileTargets: r3 = the scene target,
// r4 = the command list; r3 the last slot.
ITC_DIRECT_ENTRY(8209AE48) {
  const auto r3 = scene_target::RecordTargets(ctx.r3.u32, ctx.r4.u32);
  if (!r3) {
    lifted::original::c8209AE48(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
ITC_DIRECT_ENTRY(8209AF40) {
  const auto r3 = scene_target::RecordTileTargets(ctx.r3.u32, ctx.r4.u32);
  if (!r3) {
    lifted::original::c8209AF40(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// SceneTarget::RecordBoundTargets: r3 = the command list; r3 the last slot.
ITC_DIRECT_ENTRY(8209B058) {
  const auto r3 = scene_target::RecordBoundTargets(ctx.r3.u32);
  if (!r3) {
    lifted::original::c8209B058(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// SceneTarget::RecordPasses (its register step, the original with the
// command list's diagnostics on).
ITC_DIRECT_ENTRY(8209B3D0) { hooks::SceneTargetRecordPassesOrOriginal(ctx); }

// --- race_world (their register steps, hooks/race_world.h) ---

ITC_DIRECT_ENTRY(82116C38) { hooks::RaceWorldAddDistance(ctx); }
ITC_DIRECT_ENTRY(821715B8) { hooks::RaceWorldCreate(ctx); }
ITC_DIRECT_ENTRY(82172F90) { hooks::RaceWorldNoteMarkedRacer(ctx); }
ITC_DIRECT_ENTRY(82173050) { hooks::RaceWorldAddRacers(ctx); }
