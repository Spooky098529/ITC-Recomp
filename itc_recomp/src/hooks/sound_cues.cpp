// The sound cues natively: adapters in front of src/sound_cues/sound_cues.cpp
// (docs/functions/sound-cues.md). Most are reached through the manager's,
// the Sounds' and the bank pairs' vtables, so their adapters stay until
// their callers are native.

#include "guest.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

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
              "constructors, sub_8259C9B0, sub_8259D3D8 and sub_8259D418; AudioManager::IsCuePlaying, sub_82118B58; the delegate clone, sub_82118720; X3DAudioInitialize and AudioEngine::Start3D, sub_82480710 and sub_8259CE10; the bank holders' destructor and deletes, sub_8259D438, sub_82117AD8 and sub_82117B30; with --native_heap, the bank vector's constructor sub_821194A0 with its allocation sub_821193C0, no adapter of its own) natively instead of "
              "through the translated code (same results; checked by "
              "--run_harnesses=sound_cues)");

// The flag's fallback; the floats moved as the original's lfs and stfs
// (no flush to zero).
#define ITC_SOUND_FALLBACK(address)        \
  REX_FUNC_PROLOGUE();                     \
  if (!ITC_FLAG(native_sound_cues)) {      \
    lifted::original::c##address(ctx);       \
    return;                                \
  }                                        \
  ctx.fpscr.disableFlushMode();

// An adapter leaving `r3_expr` in r3 after `call`.
#define ITC_SOUND_VOID(address, call, r3_expr) \
  ITC_NATIVE_ENTRY_ALWAYS(address) {        \
    ITC_SOUND_FALLBACK(address)               \
    const uint32_t itc_r3_ = (r3_expr);       \
    call;                                     \
    ctx.r3.u64 = itc_r3_;                     \
  }

// An adapter returning a word in r3.
#define ITC_SOUND_WORD(address, expr)    \
  ITC_NATIVE_ENTRY_ALWAYS(address) {  \
    ITC_SOUND_FALLBACK(address)         \
    ctx.r3.u64 = uint32_t(expr);        \
  }

// An adapter for a bank slot's function: the original where it traps.
#define ITC_SOUND_SLOT(address, function)                                 \
  ITC_NATIVE_ENTRY_ALWAYS(address) {                                   \
    ITC_SOUND_FALLBACK(address)                                          \
    uint32_t result = 0;                                                 \
    if (!function(ctx.r3.u32, ctx.r4.u32, &result)) {              \
      lifted::original::c##address(ctx);                                   \
      return;                                                            \
    }                                                                    \
    ctx.r3.u64 = result;                                                 \
  }

using namespace sound_cues;

ITC_SOUND_VOID(8259D478, ListenerConstruct(ctx.r3.u32), ctx.r3.u32)
// Listener::SetOrientation, Listener::SetPosition and sub_8259CC80
// (sub_8259D550, sub_8259D588, sub_8259CC80) have no adapters: their
// callers in play (the manager's listener setters and Sound::SetPosition)
// are native.
ITC_SOUND_VOID(82117E08, ManagerSetListenerPosition(ctx.r3.u32, ctx.r4.u32),
               ctx.r3.u32 + 1456)
ITC_SOUND_VOID(82117E10,
               ManagerSetListenerOrientation(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32),
               ctx.r3.u32 + 1456)
ITC_SOUND_VOID(82117E18, ManagerSetListenerVelocity(ctx.r3.u32, ctx.r4.u32),
               ctx.r3.u32 + 1456)
ITC_SOUND_WORD(82117EA8, ManagerGetEngine(ctx.r3.u32))

// BankPair::Release, SoundBank and IsLoading (sub_82117760, sub_82117770,
// sub_82117670) have no adapters: their callers in play
// (AudioManager::ReleaseBank, GetSoundBank and IsBankLoading) are native.
ITC_SOUND_SLOT(82118190, ManagerReleaseBank)
ITC_SOUND_SLOT(82118200, ManagerIsBankLoading)
ITC_SOUND_SLOT(821182E0, ManagerIsBankSlotEmpty)
ITC_SOUND_SLOT(82118400, ManagerGetSoundBank)

ITC_SOUND_VOID(821173F0, SoundStart(ctx.r3.u32), ctx.r3.u32)
ITC_SOUND_VOID(82117568, SoundSet3D(ctx.r3.u32, ctx.r4.u32), ctx.r3.u32)
ITC_SOUND_WORD(82117560, SoundSetPosition(ctx.r3.u32, ctx.r4.u32))
ITC_SOUND_WORD(821175B8, SoundIsBank(ctx.r3.u32, ctx.r4.u32))
ITC_SOUND_WORD(82117578, SoundIsCue(ctx.r3.u32, ctx.r4.u32))

ITC_SOUND_VOID(8259C9B0, EmitterConstruct(ctx.r3.u32), ctx.r3.u32)
ITC_SOUND_VOID(8259D3D8, SoundBankHolderConstruct(ctx.r3.u32), ctx.r3.u32)
ITC_SOUND_VOID(8259D418, WaveBankHolderConstruct(ctx.r3.u32), ctx.r3.u32)

ITC_SOUND_WORD(82118B58, ManagerIsCuePlaying(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))

// The delegate clone allocates through the native heap: the original runs
// unless --native_heap routes the game's heap there, and where operator new
// fails.
ITC_NATIVE_ENTRY_ALWAYS(82118720) {
  ITC_SOUND_FALLBACK(82118720)
  uint32_t copy = 0;
  if (!hooks::NativeHeapActive() || !DelegateClone(ctx.r3.u32, &copy)) {
    lifted::original::c82118720(ctx);
    return;
  }
  ctx.r3.u64 = copy;
}

// The bank vector's constructor: r3 = the vector, r4 = the count, r5 = the
// value's address (sub_821193C0, its allocation, has no adapter: only this
// one's plain C++ calls it in play). The original without --native_heap or
// where the allocation hands back.
ITC_NATIVE_ENTRY_ALWAYS(821194A0) {
  ITC_SOUND_FALLBACK(821194A0)
  if (hooks::NativeHeapActive()) {
    if (const auto r3 = WordVectorFill(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  lifted::original::c821194A0(ctx);
}

ITC_SOUND_WORD(82480710, X3DAudioInitialize(ctx.r3.u32, ctx.f1.f64, ctx.r5.u32))
ITC_SOUND_WORD(8259CE10, AudioEngineStart3D(ctx.r3.u32))

// The bank holders' deletes, which free through the native heap: the
// original unless --native_heap routes the game's heap there, with a handle
// to close, or where operator delete refuses the block.
// WaveBankHolder::~WaveBankHolder (sub_8259D438) has no adapter: its caller
// in play, sub_82117AD8, is native.
#define ITC_SOUND_HOLDER_DELETE(address, function)                         \
  ITC_NATIVE_ENTRY_ALWAYS(address) {                                    \
    ITC_SOUND_FALLBACK(address)                                           \
    uint32_t r3 = 0;                                                      \
    if (!hooks::NativeHeapActive() || !function(ctx.r3.u32, &r3)) { \
      lifted::original::c##address(ctx);                                    \
      return;                                                             \
    }                                                                     \
    ctx.r3.u64 = r3;                                                      \
  }
ITC_SOUND_HOLDER_DELETE(82117AD8, WaveBankHolderDelete)
ITC_SOUND_HOLDER_DELETE(82117B30, SoundBankHolderDelete)

// For the fallback originals (docs/missing-functions.md): lifted code calls
// these hand ports by address. Each leaves r3 as the original does; the
// floats moved as the original's lfs and stfs.
// BankPair::Release: r3 = the pair (left).
ITC_DIRECT_ENTRY(82117760) { BankPairRelease(ctx.r3.u32); }
// The bank vector's allocation: r3 = the vector, r4 = the count; r3 = 1 or 0.
// The original without --native_heap or where it hands back (its writes made
// again there).
ITC_DIRECT_ENTRY(821193C0) {
  if (hooks::NativeHeapActive()) {
    if (const auto r3 = WordVectorAllocate(ctx.r3.u32, ctx.r4.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  lifted::original::c821193C0(ctx);
}
// sub_8259CC80: r3 = the object (left), r4 = the vector.
ITC_DIRECT_ENTRY(8259CC80) {
  ctx.fpscr.disableFlushMode();
  SetVector36(ctx.r3.u32, ctx.r4.u32);
}
// WaveBankHolder::~WaveBankHolder: r3 = the holder; r3 = the handle (-1); the
// original where there's a handle to close.
ITC_DIRECT_ENTRY(8259D438) {
  uint32_t r3 = 0;
  if (!WaveBankHolderDestroy(ctx.r3.u32, &r3)) {
    lifted::original::c8259D438(ctx);
    return;
  }
  ctx.r3.u64 = r3;
}
// Listener::SetOrientation: r3 = the listener (left), r4 = front, r5 = top.
ITC_DIRECT_ENTRY(8259D550) {
  ctx.fpscr.disableFlushMode();
  ListenerSetOrientation(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
// Listener::SetPosition: r3 = the listener (left), r4 = the position.
ITC_DIRECT_ENTRY(8259D588) {
  ctx.fpscr.disableFlushMode();
  ListenerSetPosition(ctx.r3.u32, ctx.r4.u32);
}
