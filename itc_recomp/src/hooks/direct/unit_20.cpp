// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 20's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "hooks/heap.h"
#include "hooks/title_screen_native.h"
#include "hooks/type67_modes.h"
#include "hooks/xact_tracks.h"
#include "hooks/xact_waves.h"
#include "lifted/originals.h"
#include "ui_movies/game.h"
#include "xact/xact.h"

// --- XACT (src/xact/xact.cpp, docs/functions/xact.md): the mappings of the
// deleted adapters in hooks/xact.cpp, the floats moved as the original's lfs
// and stfs (no flush to zero). Ports that can decline run the original. ---

// The wave entry's leaves: r3 = the entry; r3 = the word.
ITC_DIRECT_ENTRY(820A8958) { ctx.r3.u64 = xact::WaveEntryGetLoopStart(ctx.r3.u32); }
ITC_DIRECT_ENTRY(825CEE00) { ctx.r3.u64 = xact::WaveEntryGetDuration(ctx.r3.u32); }
ITC_DIRECT_ENTRY(825CEE10) { ctx.r3.u64 = xact::WaveEntryHasLoopRegion(ctx.r3.u32); }
ITC_DIRECT_ENTRY(825CEE28) { ctx.r3.u64 = xact::WaveEntryGetFormatTag(ctx.r3.u32); }
// XACTWaveEntry::XACTWaveEntry: r3 left as the entry.
ITC_DIRECT_ENTRY(825CED88) { xact::WaveEntryConstruct(ctx.r3.u32); }
// XACTWaveEntry::Set: r3 = the entry, r4 = the index, r5 = the bank; r3 =
// memcpy's destination (entry + 12).
ITC_DIRECT_ENTRY(825CED98) {
  ctx.fpscr.disableFlushMode();
  const uint32_t entry = ctx.r3.u32;
  if (!xact::WaveEntrySet(entry, ctx.r4.u32, ctx.r5.u32)) {
    lifted::original::c825CED98(ctx);
    return;
  }
  ctx.r3.u64 = uint32_t(entry + 12);
}
// XACTWaveEntry::SamplesToBytes: r3 = the entry, r4 = the position, r5 = the
// out word (or 0).
ITC_DIRECT_ENTRY(825CF080) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = xact::WaveEntrySamplesToBytes(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  if (!r3) {
    lifted::original::c825CF080(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// GetLoopStartBytes (r4 the out word) and GetLoopLengthBytes.
ITC_DIRECT_ENTRY(825CF210) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = xact::WaveEntryGetLoopStartBytes(ctx.r3.u32, ctx.r4.u32);
  if (!r3) {
    lifted::original::c825CF210(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
ITC_DIRECT_ENTRY(825CF288) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = xact::WaveEntryGetLoopLengthBytes(ctx.r3.u32);
  if (!r3) {
    lifted::original::c825CF288(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// The engine's GetCategoryFlag and variable record: r3 = the engine, r4 = the
// index.
ITC_DIRECT_ENTRY(825C0708) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = xact::EngineGetCategoryFlag(ctx.r3.u32, ctx.r4.u32);
  if (!r3) {
    lifted::original::c825C0708(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
ITC_DIRECT_ENTRY(825C0748) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = xact::EngineGetVariableRecord(ctx.r3.u32, ctx.r4.u32);
  if (!r3) {
    lifted::original::c825C0748(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// XACTCue::GetVariable: r3 = the cue, r4 = the index, r5 = the out word (or
// 0); r3 = the HRESULT, sign-extended (li, or lis and ori). Allocates: the
// original unless --native_heap routes the game's heap to the native layer.
ITC_DIRECT_ENTRY(825C5960) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = hooks::NativeHeapActive()
                      ? xact::CueGetVariable(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)
                      : std::nullopt;
  if (!r3) {
    lifted::original::c825C5960(ctx);
    return;
  }
  ctx.r3.s64 = int32_t(*r3);
}
// XACTCueBase::XACTCueBase: r3 = the cue, r4 = the engine, r5 = the sound
// bank, r6 = the record, r7 = the index; r3 left as the cue. Registers with
// the bank (AddListener allocates): the original unless --native_heap, or
// where the bank's slot isn't AddListener.
ITC_DIRECT_ENTRY(825C5E58) {
  ctx.fpscr.disableFlushMode();
  const uint32_t cue = ctx.r3.u32;
  if (!hooks::NativeHeapActive() ||
      !xact::CueBaseConstruct(cue, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32)) {
    lifted::original::c825C5E58(ctx);
    return;
  }
  ctx.r3.u64 = cue;
}
// XACTNodePool::PushBack: r3 = the pool, r4 = the value; r3 = the node (0 when
// out of memory). Native layer only, as the deleted adapter.
ITC_DIRECT_ENTRY(825C60C0) {
  ctx.fpscr.disableFlushMode();
  if (!hooks::NativeHeapActive()) {
    lifted::original::c825C60C0(ctx);
    return;
  }
  ctx.r3.u64 = xact::NodePoolPushBack(ctx.r3.u32, ctx.r4.u32);
}
// XACTSound::GetPitch: r3 = the sound; f1 = the pitch.
ITC_DIRECT_ENTRY(825CAC48) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = xact::SoundGetPitch(ctx.r3.u32);
}
// XACTWaveBankRoot::XACTWaveBankRoot: r3 = the bank, r4 = the engine; r3 left
// as the bank.
ITC_DIRECT_ENTRY(825CC620) {
  ctx.fpscr.disableFlushMode();
  const uint32_t bank = ctx.r3.u32;
  xact::WaveBankRootConstruct(bank, ctx.r4.u32);
  ctx.r3.u64 = bank;
}

// --- XACT's tracks, events and waves (src/xact/tracks.cpp, waves.cpp): their
// register steps (hooks/xact_tracks.h, hooks/xact_waves.h), which build the
// guest call and frame and run the original where the port declines. ---

ITC_DIRECT_ENTRY(825CB0C8) { hooks::XactSoundScheduleEvent(ctx); }
ITC_DIRECT_ENTRY(825D0268) { hooks::XactTrackGetVolume(ctx); }
ITC_DIRECT_ENTRY(825D02B0) { hooks::XactTrackGetPitch(ctx); }
ITC_DIRECT_ENTRY(825D02F8) { hooks::XactTrackCreateInstance(ctx); }
ITC_DIRECT_ENTRY(825D0BF8) { hooks::XactTrackSetVolume(ctx); }
ITC_DIRECT_ENTRY(825D0EF0) { hooks::XactEventDestruct(ctx); }
ITC_DIRECT_ENTRY(825D1358) { hooks::XactEventFire(ctx); }
ITC_DIRECT_ENTRY(825D13F0) { hooks::XactEventDone(ctx); }
ITC_DIRECT_ENTRY(825D1760) { hooks::XactEventConstruct(ctx); }
ITC_DIRECT_ENTRY(825D1C78) { hooks::XactEngineRequestCommit(ctx); }
ITC_DIRECT_ENTRY(825D1DF8) { hooks::XactWaveSetVoice(ctx); }
ITC_DIRECT_ENTRY(825D2170) { hooks::XactWaveGetStartPosition(ctx); }
ITC_DIRECT_ENTRY(825D23F0) { hooks::XactWaveConstruct(ctx); }
ITC_DIRECT_ENTRY(825D24D8) { hooks::XactWaveSetState(ctx); }
ITC_DIRECT_ENTRY(825D2778) { hooks::XactWavePrepareNext(ctx); }
ITC_DIRECT_ENTRY(825D28D0) { hooks::XactWaveRelease(ctx); }
ITC_DIRECT_ENTRY(825D29E8) { hooks::XactWaveDestruct(ctx); }
ITC_DIRECT_ENTRY(825D55E0) { hooks::XactTrackInstanceCheckDone(ctx); }
ITC_DIRECT_ENTRY(825D5678) { hooks::XactTrackInstanceSetVolume(ctx); }
ITC_DIRECT_ENTRY(825D5980) { hooks::XactTrackInstancePlay(ctx); }
ITC_DIRECT_ENTRY(825D5D18) { hooks::XactTrackInstanceStop(ctx); }

// --- The title screen, the UI movies, the type 6 mode ---

// Players::AssignController: r3 = the player, r4 = the controller (its step).
ITC_DIRECT_ENTRY(8209F6B8) { hooks::PlayersAssignController(ctx); }
// UiMovie::IsNamedActive: r3 = the movie, r4 = the animation's index; r3 = 1 or
// 0 (li).
ITC_DIRECT_ENTRY(8211BC18) { ctx.r3.u64 = ui_movies::IsNamedActive(ctx.r3.u32, ctx.r4.u32); }
// Type6Screen0::ShowPage: r4 = the page, the stack pointer; r3 the last node
// (its step).
ITC_DIRECT_ENTRY(822344A8) { hooks::Type6Screen0ShowPage(ctx); }
