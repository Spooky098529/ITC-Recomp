// The sound cues natively: adapters in front of src/sound_cues/sound_cues.cpp
// (docs/functions/sound-cues.md). Most are reached through the manager's,
// the Sounds' and the bank pairs' vtables, so their adapters stay until
// their callers are native.

#include "guest.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "sound_cues/sound_cues.h"

ITC_FLAG_BOOL(native_sound_cues, true, "ITC",
              "Run the sound cues' bookkeeping (docs/functions/sound-cues.md: the listener, "
              "sub_8259D478, sub_8259D550, sub_8259D588 and sub_8259CC80, and the manager's "
              "listener setters, sub_82117E08 to sub_82117E18; AudioManager::GetEngine, "
              "sub_82117EA8; the bank pairs' and the manager's bank slots' accessors, "
              "sub_82117670 to sub_82117770 and sub_82118190 to sub_82118400; the Sounds' "
              "accessors, sub_821173F0 to sub_821175B8; the emitter's and the bank holders' "
              "constructors, sub_8259C9B0, sub_8259D3D8 and sub_8259D418; AudioManager::IsCuePlaying, sub_82118B58; the delegate clone, sub_82118720; X3DAudioInitialize and AudioEngine::Start3D, sub_82480710 and sub_8259CE10) natively instead of "
              "through the translated code (same results; checked by "
              "--run_harnesses=sound_cues)");

// The flag's fallback; the floats moved as the original's lfs and stfs
// (no flush to zero).
#define ITC_SOUND_FALLBACK(address)        \
  REX_FUNC_PROLOGUE();                     \
  if (!ITC_FLAG(native_sound_cues)) {      \
    __imp__sub_##address(ctx, base);       \
    return;                                \
  }                                        \
  ctx.fpscr.disableFlushMode();

// An adapter leaving `r3_expr` in r3 after `call`.
#define ITC_SOUND_VOID(address, call, r3_expr) \
  extern "C" REX_FUNC(sub_##address) {        \
    ITC_SOUND_FALLBACK(address)               \
    const uint32_t itc_r3_ = (r3_expr);       \
    call;                                     \
    ctx.r3.u64 = itc_r3_;                     \
  }

// An adapter returning a word in r3.
#define ITC_SOUND_WORD(address, expr)    \
  extern "C" REX_FUNC(sub_##address) {  \
    ITC_SOUND_FALLBACK(address)         \
    ctx.r3.u64 = uint32_t(expr);        \
  }

// An adapter for a bank slot's function: the original where it traps.
#define ITC_SOUND_SLOT(address, function)                                 \
  extern "C" REX_FUNC(sub_##address) {                                   \
    ITC_SOUND_FALLBACK(address)                                          \
    uint32_t result = 0;                                                 \
    if (!function(base, ctx.r3.u32, ctx.r4.u32, &result)) {              \
      __imp__sub_##address(ctx, base);                                   \
      return;                                                            \
    }                                                                    \
    ctx.r3.u64 = result;                                                 \
  }

using namespace sound_cues;

ITC_SOUND_VOID(8259D478, ListenerConstruct(base, ctx.r3.u32), ctx.r3.u32)
// Listener::SetOrientation, Listener::SetPosition and sub_8259CC80
// (sub_8259D550, sub_8259D588, sub_8259CC80) have no adapters: their
// callers in play (the manager's listener setters and Sound::SetPosition)
// are native.
ITC_SOUND_VOID(82117E08, ManagerSetListenerPosition(base, ctx.r3.u32, ctx.r4.u32),
               ctx.r3.u32 + 1456)
ITC_SOUND_VOID(82117E10,
               ManagerSetListenerOrientation(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32),
               ctx.r3.u32 + 1456)
ITC_SOUND_VOID(82117E18, ManagerSetListenerVelocity(base, ctx.r3.u32, ctx.r4.u32),
               ctx.r3.u32 + 1456)
ITC_SOUND_WORD(82117EA8, ManagerGetEngine(ctx.r3.u32))

// BankPair::Release, SoundBank and IsLoading (sub_82117760, sub_82117770,
// sub_82117670) have no adapters: their callers in play
// (AudioManager::ReleaseBank, GetSoundBank and IsBankLoading) are native.
ITC_SOUND_SLOT(82118190, ManagerReleaseBank)
ITC_SOUND_SLOT(82118200, ManagerIsBankLoading)
ITC_SOUND_SLOT(821182E0, ManagerIsBankSlotEmpty)
ITC_SOUND_SLOT(82118400, ManagerGetSoundBank)

ITC_SOUND_VOID(821173F0, SoundStart(base, ctx.r3.u32), ctx.r3.u32)
ITC_SOUND_VOID(82117568, SoundSet3D(base, ctx.r3.u32, ctx.r4.u32), ctx.r3.u32)
ITC_SOUND_WORD(82117560, SoundSetPosition(base, ctx.r3.u32, ctx.r4.u32))
ITC_SOUND_WORD(821175B8, SoundIsBank(base, ctx.r3.u32, ctx.r4.u32))
ITC_SOUND_WORD(82117578, SoundIsCue(base, ctx.r3.u32, ctx.r4.u32))

ITC_SOUND_VOID(8259C9B0, EmitterConstruct(base, ctx.r3.u32), ctx.r3.u32)
ITC_SOUND_VOID(8259D3D8, SoundBankHolderConstruct(base, ctx.r3.u32), ctx.r3.u32)
ITC_SOUND_VOID(8259D418, WaveBankHolderConstruct(base, ctx.r3.u32), ctx.r3.u32)

ITC_SOUND_WORD(82118B58, ManagerIsCuePlaying(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))

// The delegate clone allocates through the native heap: the original runs
// unless --native_heap routes the game's heap there, and where operator new
// fails.
extern "C" REX_FUNC(sub_82118720) {
  ITC_SOUND_FALLBACK(82118720)
  uint32_t copy = 0;
  if (!hooks::NativeHeapActive() || !DelegateClone(base, ctx.r3.u32, &copy)) {
    __imp__sub_82118720(ctx, base);
    return;
  }
  ctx.r3.u64 = copy;
}

ITC_SOUND_WORD(82480710, X3DAudioInitialize(base, ctx.r3.u32, ctx.f1.f64, ctx.r5.u32))
ITC_SOUND_WORD(8259CE10, AudioEngineStart3D(base, ctx.r3.u32))
