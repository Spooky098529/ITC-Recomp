// XACT natively: adapters in front of src/xact/xact.cpp
// (docs/functions/xact.md). Most of XACT is reached through its objects'
// vtables, so these adapters stay until their callers are native.

#include "guest.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "xact/xact.h"

ITC_FLAG_BOOL(native_xact, true, "ITC",
              "Run the parts of XACT made native (docs/functions/xact.md: the name hash, "
              "sub_825BFC50; the node pools, sub_825C2060, sub_825C2C60, sub_825C2CC8 and "
              "sub_825C1F30; XACTEngine::AddCue, sub_825C1518; the global settings' "
              "constructors, sub_825C21B0 and sub_825C3508; XACTVariable::Init, sub_825C1DE0; "
              "the cue's DetachSound and FindVariable, sub_825C4FC0 and sub_825C5758; the "
              "fader's constructor and destructor, sub_825CC0A0 and sub_825CC0D0; the sound "
              "banks' file readers, sub_825C7AA8 to sub_825C8230 and sub_825C6928, while the bank's "
              "slot +64 is GetHeader; the wave banks' accessors, sub_825C8398 to sub_825C85E8 and "
              "sub_825CC320; the wave entry accessors, sub_825CED88 to sub_825CF080; the cue's "
              "GetStateInternal, sub_825C4D90; the sound banks' ForgetWaveBank and the engine's "
              "loop over them, sub_825C72B0 and sub_825C0408; XACTSound::SetTrackParameter "
              "without tracks, sub_825CA980; XACTFader::Step when idle, sub_825CC1D0; the "
              "variable's, sound banks', wave banks' and cues' constructors, sub_825C20B0, "
              "sub_825C76A0, sub_825C79B8, sub_825CC620, sub_825CEBE0, sub_825C8310, sub_825C5E58 "
              "and sub_825C67E8; the listener lists, sub_825C60C0, sub_825C6140, sub_825C7858, "
              "sub_825C6040 and sub_825C7758; the loop region in bytes, sub_825CF210 and "
              "sub_825CF288; loading a sound bank, sub_825C7E40 and sub_825C7448; the global settings' "
              "accessors, sub_825C2200 to sub_825C2618, and their users, sub_825C3548, "
              "sub_825C3F90, sub_825C4000, sub_825C4070, sub_825C0598, sub_825C05B0, "
              "sub_825C0708, sub_825C0748 and sub_825C04D8, while the settings' slots are "
              "those functions; the wave entries' leaves, sub_825CEE10, sub_825CEE28, "
              "sub_825CEEA0 and the folded getters sub_82099BA0, sub_820A8958 and sub_82379D50; "
              "XACTSound::GetPitch, sub_825CAC48; the wave banks' listeners, sub_825C8690 and "
              "sub_825C8540; the wave bank lookups, sub_825C0360 and sub_825C7218; the "
              "variable's destructors, sub_825C2128 and sub_825C3EE0; XACTSound::GetVolume, "
              "sub_825CB5E8; the cue's variables, sub_825C5960 and sub_825C5E28; the track "
              "record, sub_825CC9C8, and XACTSound::SetUpRpcs, sub_825CB470, as in play; "
              "loading the global settings, sub_825C26E0 and sub_825C3B68, and their DSP "
              "preset and RPC set-up where nothing is to do, sub_825C27C0 and sub_825C2910; the "
              "wave durations, sub_825CC380; letting go of wave banks without listeners to "
              "tell, sub_825C6A20 and sub_825C7798; the wave region, sub_825C8440, and the "
              "folded getter it calls, sub_8220BC28 (the menu list's too: the same load); XACT's "
              "clock, sub_825BFE28; XACTEngine::SendNotification, sub_825C1388, its adapter in "
              "hooks/xact_waves.cpp) "
              "natively "
              "instead of through the translated code (same results; checked by "
              "--run_harnesses=xact)");

// The flag's fallback; the floats moved as the original's lfs and stfs
// (no flush to zero).
#define ITC_XACT_FALLBACK(address)    \
  REX_FUNC_PROLOGUE();                \
  if (!ITC_FLAG(native_xact)) {       \
    __imp__sub_##address(ctx, base);  \
    return;                           \
  }                                   \
  ctx.fpscr.disableFlushMode();

// An adapter leaving `r3_expr` in r3 after `call`.
#define ITC_XACT_VOID(address, call, r3_expr) \
  extern "C" REX_FUNC(sub_##address) {       \
    ITC_XACT_FALLBACK(address)               \
    const uint32_t itc_r3_ = (r3_expr);      \
    call;                                    \
    ctx.r3.u64 = itc_r3_;                    \
  }

// An adapter returning a word in r3.
#define ITC_XACT_WORD(address, expr)    \
  extern "C" REX_FUNC(sub_##address) { \
    ITC_XACT_FALLBACK(address)         \
    ctx.r3.u64 = uint32_t(expr);       \
  }

// An adapter for a function allocating or freeing through the native
// layer: the original unless --native_heap routes the game's heap there.
#define ITC_XACT_HEAP_WORD(address, expr)                       \
  extern "C" REX_FUNC(sub_##address) {                         \
    ITC_XACT_FALLBACK(address)                                 \
    if (!hooks::NativeHeapActive()) {                          \
      __imp__sub_##address(ctx, base);                         \
      return;                                                  \
    }                                                          \
    ctx.r3.u64 = uint32_t(expr);                               \
  }

// An adapter returning a word in r3 when `expr` (an optional) has one, else
// running the original (a vtable slot that isn't the native function, or a
// path the native version leaves to the original).
#define ITC_XACT_OPTIONAL(address, expr)  \
  extern "C" REX_FUNC(sub_##address) {   \
    ITC_XACT_FALLBACK(address)           \
    const auto itc_result_ = (expr);     \
    if (!itc_result_) {                  \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    ctx.r3.u64 = *itc_result_;           \
  }

using namespace xact;

// The name hash: the original where it traps (a table size of 0).
extern "C" REX_FUNC(sub_825BFC50) {
  ITC_XACT_FALLBACK(825BFC50)
  uint32_t hash = 0;
  if (!HashName(base, ctx.r4.u32, ctx.r5.u32, &hash)) {
    __imp__sub_825BFC50(ctx, base);
    return;
  }
  ctx.r3.u64 = hash;
}

ITC_XACT_VOID(825C2060, NodePoolConstruct(base, ctx.r3.u32), ctx.r3.u32)
ITC_XACT_VOID(825C2C60, NodePoolFree(base, ctx.r3.u32, ctx.r4.u32), ctx.r3.u32)
ITC_XACT_HEAP_WORD(825C2CC8, NodePoolAlloc(base, ctx.r3.u32))
ITC_XACT_HEAP_WORD(825C1F30, NodePoolFreeChunks(base, ctx.r3.u32))

ITC_XACT_VOID(825C1518, EngineAddCue(base, ctx.r3.u32, ctx.r4.u32), ctx.r3.u32)

// XACTGlobalSettingsBase::XACTGlobalSettingsBase (sub_825C3508) has no
// adapter: its only caller, sub_825C21B0, is native.
ITC_XACT_VOID(825C21B0, GlobalSettingsConstruct(base, ctx.r3.u32, ctx.r4.u32), ctx.r3.u32)

// XACTVariable::Init (sub_825C1DE0) has no adapter: its callers in play,
// MakeState and XACTCue::GetVariable, are native.
ITC_XACT_VOID(825C4FC0, CueDetachSound(base, ctx.r3.u32), ctx.r3.u32)
ITC_XACT_WORD(825C5758, CueFindVariable(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_VOID(825CC0A0, FaderConstruct(base, ctx.r3.u32), ctx.r3.u32)
ITC_XACT_VOID(825CC0D0, FaderDestruct(base, ctx.r3.u32), ctx.r3.u32)

// The sound banks' file readers. XACTSoundBank::GetSimpleCues
// (sub_825C7AB0) has no adapter: its only caller, GetCueRecord, is native.
ITC_XACT_WORD(825C7AA8, SoundBankGetHeader(base, ctx.r3.u32))
ITC_XACT_OPTIONAL(825C7B70, SoundBankGetWaveBankNames(base, ctx.r3.u32))
ITC_XACT_OPTIONAL(825C7C90, SoundBankGetPointer(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C6928, SoundBankGetCueCount(base, ctx.r3.u32))
ITC_XACT_OPTIONAL(825C7F30, SoundBankFirstEvent(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C7F98, SoundBankNextEvent(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C8230, SoundBankGetCueRecord(base, ctx.r3.u32, ctx.r4.u32))

// The wave banks' accessors.
ITC_XACT_WORD(825C8398, WaveBankGetEntry(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_WORD(825C8580, WaveBankGetSeekTable(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_WORD(825C85B8, WaveBankGetName(base, ctx.r3.u32))
ITC_XACT_WORD(825C85E8, WaveBankGetEntryCount(base, ctx.r3.u32))
extern "C" REX_FUNC(sub_825CC320) {
  ITC_XACT_FALLBACK(825CC320)
  if (!WaveBankGetWaveFormat(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    __imp__sub_825CC320(ctx, base);
    return;
  }
  // The original leaves the entry's address (GetEntry's r3).
  ctx.r3.u64 = WaveBankGetEntry(base, ctx.r3.u32, ctx.r4.u32);
}

// The wave entry accessors. XACTWaveEntry::FindXmaBlock (sub_825CEF50) has
// no adapter: its only caller, SamplesToBytes, is native.
// No adapter (removed 2026-10-29: only XACTInMemoryWave::Prepare's plain C++ calls it in play) for XACTWaveEntry::XACTWaveEntry sub_825CED88
// (folded: its callers outside XACT never run in play).
// No adapter (removed 2026-10-29: only XACTInMemoryWave::Prepare's plain C++ calls it in play) for XACTWaveEntry::Set sub_825CED98.
// XACTWaveEntry::GetLoopRegion (sub_825CEDF8) has no adapter: its only
// caller in play, GetWaveRegion, is native.
// No adapter (removed 2026-10-29: only XACTWave::GetStartPosition's plain C++ calls it in play) for XACTWaveEntry::GetDuration sub_825CEE00.
// XACTWaveEntry::SamplesToMs (sub_825CEE70) has no adapter: its only
// caller in play, GetWaveDurations, is native.
// No adapter (removed 2026-10-29: only XACTInMemoryWave::Prepare's plain C++ calls it in play) for XACTWaveEntry::SamplesToBytes sub_825CF080.

ITC_XACT_WORD(825C4D90, CueGetStateInternal(base, ctx.r3.u32))
// XACTSoundBank::ForgetWaveBank (sub_825C72B0) has no adapter: its only
// caller, sub_825C0408, is native.
ITC_XACT_OPTIONAL(825C0408, EngineForgetWaveBank(base, ctx.r3.u32, ctx.r4.u32))
extern "C" REX_FUNC(sub_825CA980) {
  ITC_XACT_FALLBACK(825CA980)
  if (!SoundSetTrackParameter(base, ctx.r3.u32)) {
    __imp__sub_825CA980(ctx, base);
  }
}
ITC_XACT_OPTIONAL(825CC1D0, FaderStep(base, ctx.r3.u32))

// Constructors and listener lists. XACTSoundBankBase::XACTSoundBankBase
// (sub_825C76A0), XACTWaveBankRoot::XACTWaveBankRoot (sub_825CC620),
// XACTWaveBankBase::XACTWaveBankBase (sub_825CEBE0) and
// XACTCueBase::XACTCueBase (sub_825C5E58) have no adapters: their only
// callers are native constructors.
// XACTVariable::XACTVariable (sub_825C20B0) has no adapter: its callers in
// play, MakeState and XACTCue::GetVariable, are native.
// XACTNodePool::PushBack (sub_825C60C0) has no adapter: its callers in
// play, the listener lists' AddListener (sub_825C6140, sub_825C7858,
// sub_825C8690), are native.
ITC_XACT_HEAP_WORD(825C6140, CueAddListener(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_HEAP_WORD(825C7858, SoundBankAddListener(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_WORD(825C6040, CueRemoveListener(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_WORD(825C7758, SoundBankRemoveListener(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_VOID(825C79B8, SoundBankConstruct(base, ctx.r3.u32, ctx.r4.u32), ctx.r3.u32)
ITC_XACT_VOID(825C8310, WaveBankConstruct(base, ctx.r3.u32, ctx.r4.u32), ctx.r3.u32)
// The cue's constructor registers the cue with its bank (AddListener, which
// allocates): the original unless --native_heap routes the game's heap to
// the native layer, or where the bank's slot isn't AddListener.
extern "C" REX_FUNC(sub_825C67E8) {
  ITC_XACT_FALLBACK(825C67E8)
  const uint32_t cue = ctx.r3.u32;
  if (!hooks::NativeHeapActive() ||
      !CueConstruct(base, cue, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32)) {
    __imp__sub_825C67E8(ctx, base);
    return;
  }
  ctx.r3.u64 = cue;
}
// XACTWaveEntry::GetLoopStartBytes and GetLoopLengthBytes (sub_825CF210,
// sub_825CF288) have no adapters: their only caller in play,
// GetWaveRegion, is native.

// Loading a sound bank, which allocates: the original unless --native_heap
// routes the game's heap to the native layer, or where the bank's slot +64
// isn't GetHeader.
// XACTSoundBank::InitInstanceCounts (sub_825C7448) has no adapter: its only
// caller, Load, is native.
extern "C" REX_FUNC(sub_825C7E40) {
  ITC_XACT_FALLBACK(825C7E40)
  const auto result = hooks::NativeHeapActive()
                          ? SoundBankLoad(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                          ctx.r7.u32)
                          : std::nullopt;
  if (!result) {
    __imp__sub_825C7E40(ctx, base);
    return;
  }
  ctx.r3.u64 = *result;
}

// The global settings' accessors, reached through the settings' vtable
// (0x820753A8): each runs the original where a slot it reaches isn't the
// native function. XACTGlobalSettings::AddInstance and RemoveInstance
// (sub_825C3F90, sub_825C4000) have no adapters: their only callers,
// sub_825C0598 and sub_825C05B0, are native.
ITC_XACT_WORD(825C2200, SettingsGetBuffer(base, ctx.r3.u32))
ITC_XACT_OPTIONAL(825C2208, SettingsGetNameBuckets(base, ctx.r3.u32))
ITC_XACT_OPTIONAL(825C2268, SettingsGetNameEntry(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C22C8, SettingsGetPointer(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C23C8, SettingsGetCategoryRecord(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C2428, SettingsGetVariableRecord(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C2480, SettingsGetMaxInstances(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C24B0, SettingsGetInstanceBehavior(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C24E8, SettingsGetParentCategory(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C2518, SettingsGetVolumeByte(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C2548, SettingsGetNameEntryIndex(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C2618, SettingsGetCategoryFlag(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C3548, SettingsIsInCategory(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_XACT_OPTIONAL(825C4070,
                  SettingsSetCategoryVolume(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
// No adapter (removed 2026-10-29: only XACTInMemoryWave::Prepare's plain C++ calls it in play) for XACTEngine::GetCategoryFlag sub_825C0708.
ITC_XACT_OPTIONAL(825C0598, EngineAddInstance(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C05B0, EngineRemoveInstance(base, ctx.r3.u32, ctx.r4.u32))
// sub_825C0748 (a variable's record) has no adapter: its only caller,
// XACTCue::GetVariable, is native.
// sub_825C04D8 (a category's effective volume) has no adapter: its only
// caller, XACTSound::GetVolume, is native.

// The wave entries' leaves (the folded getters' callers in play are
// XACT's; the same results for the game code folded with them), the
// sound's pitch, the wave banks' listeners and the sound banks' wave bank
// lookup.
// No adapter (removed 2026-10-29: only XACTWave::GetStartPosition's plain C++ calls it in play) for XACTWaveEntry::HasLoopRegion sub_825CEE10.
// No adapter (removed 2026-10-29: only XACTInMemoryWave::Prepare's plain C++ calls it in play) for XACTWaveEntry::GetFormatTag sub_825CEE28.
// No adapter (removed 2026-10-29: only XACTWave::GetStartPosition's plain C++ calls it in play) for XACTWaveEntry::MsToSamples sub_825CEEA0.
// XACTWaveEntry::GetPlayLength (sub_82099BA0, folded) has no adapter: its
// only caller in play, GetWaveRegion, is native.
// No adapter (removed 2026-10-29: only XACTWave::GetStartPosition's plain C++ calls it in play) for the folded getter GetLoopStart sub_820A8958.
ITC_XACT_WORD(82379D50, WaveEntryGetLoopLength(base, ctx.r3.u32))
// No adapter (removed 2026-10-29: only XACTTrack::GetPitch's plain C++ calls it in play) for
// XACTSound::GetPitch sub_825CAC48.
ITC_XACT_HEAP_WORD(825C8690, WaveBankAddListener(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_WORD(825C8540, WaveBankRemoveListener(base, ctx.r3.u32, ctx.r4.u32))
// XACTEngine::FindWaveBank (sub_825C0360) has no adapter: its only caller,
// GetWaveBank, is native.
// GetWaveBank registers with the wave bank (AddListener, which
// allocates): the original unless --native_heap routes the game's heap to
// the native layer.
extern "C" REX_FUNC(sub_825C7218) {
  ITC_XACT_FALLBACK(825C7218)
  const auto result = hooks::NativeHeapActive()
                          ? SoundBankGetWaveBank(base, ctx.r3.u32, ctx.r4.u32)
                          : std::nullopt;
  if (!result) {
    __imp__sub_825C7218(ctx, base);
    return;
  }
  ctx.r3.u64 = *result;
}
// XACTVariable::~XACTVariable (sub_825C2128) has no adapter: its only
// caller, the deleting destructor, is native.
ITC_XACT_HEAP_WORD(825C3EE0, VariableDeletingDestruct(base, ctx.r3.u32, ctx.r4.u32))

// The sound's volume, the cue's variables, the sounds' track records and
// RPC set-up. XACTCue::GetVariable (sub_825C5960) has no adapter: its only
// caller, MakeInstanceVariable, is native.
extern "C" REX_FUNC(sub_825CB5E8) {
  ITC_XACT_FALLBACK(825CB5E8)
  const auto result = SoundGetVolume(base, ctx.r3.u32);
  if (!result) {
    __imp__sub_825CB5E8(ctx, base);
    return;
  }
  ctx.f1.f64 = result->f1;
  ctx.r3.u64 = result->r3;
}
// MakeInstanceVariable may allocate: the original unless --native_heap
// routes the game's heap to the native layer.
extern "C" REX_FUNC(sub_825C5E28) {
  ITC_XACT_FALLBACK(825C5E28)
  const auto result =
      hooks::NativeHeapActive() ? CueMakeInstanceVariable(base, ctx.r3.u32) : std::nullopt;
  if (!result) {
    __imp__sub_825C5E28(ctx, base);
    return;
  }
  ctx.r3.u64 = *result;
}
ITC_XACT_OPTIONAL(825CC9C8, SoundGetTrackRecord(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825CB470, SoundSetUpRpcs(base, ctx.r3.u32))

// Loading the global settings, which allocates: the original unless
// --native_heap routes the game's heap to the native layer.
// XACTGlobalSettings::MakeState (sub_825C3B68) has no adapter: its only
// caller, Load, is native.
extern "C" REX_FUNC(sub_825C26E0) {
  ITC_XACT_FALLBACK(825C26E0)
  const auto result = hooks::NativeHeapActive()
                          ? SettingsLoad(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                         ctx.r7.u32)
                          : std::nullopt;
  if (!result) {
    __imp__sub_825C26E0(ctx, base);
    return;
  }
  ctx.r3.u64 = *result;
}
ITC_XACT_OPTIONAL(825C27C0, SettingsSetUpDspPreset(base, ctx.r3.u32))
ITC_XACT_OPTIONAL(825C2910, SettingsSetUpRpcs(base, ctx.r3.u32))

// The wave durations (stored at r5 and r6, r3 0), letting go of wave banks.
extern "C" REX_FUNC(sub_825CC380) {
  ITC_XACT_FALLBACK(825CC380)
  const auto result = WaveBankGetWaveDurations(base, ctx.r3.u32, ctx.r4.u32);
  if (!result) {
    __imp__sub_825CC380(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r5.u32, result->duration_ms);
  guest::StoreU32(base, ctx.r6.u32, result->loop_length_ms);
  ctx.r3.u64 = 0;
}
// ReleaseWaveBanks frees: the original unless --native_heap routes the
// game's heap to the native layer.
extern "C" REX_FUNC(sub_825C6A20) {
  ITC_XACT_FALLBACK(825C6A20)
  const auto result =
      hooks::NativeHeapActive() ? SoundBankReleaseWaveBanks(base, ctx.r3.u32) : std::nullopt;
  if (!result) {
    __imp__sub_825C6A20(ctx, base);
    return;
  }
  ctx.r3.u64 = *result;
}
ITC_XACT_OPTIONAL(825C7798, SoundBankOnWaveBankGone(base, ctx.r3.u32, ctx.r4.u32))

// The wave region; its entry accessor in the original's stack slot (r1 -
// 80). The folded getter sub_8220BC28 is also the menu list's
// (docs/functions/functions.md): one load, the same for every caller.
ITC_XACT_WORD(8220BC28, WaveEntryGetPlayOffset(base, ctx.r3.u32))
ITC_XACT_OPTIONAL(825C8440, WaveBankGetWaveRegion(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                  ctx.r1.u32 - 80))

// XACT's clock: the original where it traps. r3 as the original leaves it
// (sign-extended from the counter, zero-extended from the tick count).
extern "C" REX_FUNC(sub_825BFE28) {
  ITC_XACT_FALLBACK(825BFE28)
  const auto ms = GetTimeMs(base);
  if (!ms) {
    __imp__sub_825BFE28(ctx, base);
    return;
  }
  ctx.r3.u64 = *ms;
}
