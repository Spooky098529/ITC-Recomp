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
              "those functions) natively "
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

ITC_XACT_VOID(825C1DE0,
              VariableInit(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                           ctx.r8.u32),
              ctx.r3.u32)
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
ITC_XACT_VOID(825CED88, WaveEntryConstruct(base, ctx.r3.u32), ctx.r3.u32)
extern "C" REX_FUNC(sub_825CED98) {
  ITC_XACT_FALLBACK(825CED98)
  if (!WaveEntrySet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    __imp__sub_825CED98(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + 12;  // memcpy's r3, the destination
}
ITC_XACT_WORD(825CEDF8, WaveEntryGetLoopRegion(ctx.r3.u32))
ITC_XACT_WORD(825CEE00, WaveEntryGetDuration(base, ctx.r3.u32))
extern "C" REX_FUNC(sub_825CEE70) {
  ITC_XACT_FALLBACK(825CEE70)
  uint32_t ms = 0;
  if (!WaveEntrySamplesToMs(base, ctx.r3.u32, ctx.r4.u32, &ms)) {
    __imp__sub_825CEE70(ctx, base);
    return;
  }
  ctx.r3.u64 = ms;
}
ITC_XACT_OPTIONAL(825CF080, WaveEntrySamplesToBytes(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))

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
ITC_XACT_VOID(825C20B0, VariableConstruct(base, ctx.r3.u32), ctx.r3.u32)
ITC_XACT_HEAP_WORD(825C60C0, NodePoolPushBack(base, ctx.r3.u32, ctx.r4.u32))
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
ITC_XACT_OPTIONAL(825CF210, WaveEntryGetLoopStartBytes(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825CF288, WaveEntryGetLoopLengthBytes(base, ctx.r3.u32))

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
ITC_XACT_OPTIONAL(825C0708, EngineGetCategoryFlag(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C0598, EngineAddInstance(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C05B0, EngineRemoveInstance(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_OPTIONAL(825C0748, EngineGetVariableRecord(base, ctx.r3.u32, ctx.r4.u32))
extern "C" REX_FUNC(sub_825C04D8) {
  ITC_XACT_FALLBACK(825C04D8)
  const auto result = EngineGetCategoryVolume(base, ctx.r3.u32, ctx.r4.u32);
  if (!result) {
    __imp__sub_825C04D8(ctx, base);
    return;
  }
  ctx.f1.f64 = result->f1;
  ctx.r3.u64 = result->r3;
}
