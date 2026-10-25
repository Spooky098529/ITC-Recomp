// The mode manager's tick services natively: adapters in front of
// src/mode_tick/ (docs/functions/mode-tick.md). Each leaves r3 as the
// original does. Functions reached only from native callers have no
// adapters (the flag switches their callers): SignInWatch::CheckIdle
// (sub_8209ED58), from SignInWatch::Update.

#include "guest.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/mode_tick.h"
#include "mode_tick/mode_tick.h"

ITC_FLAG_BOOL(native_mode_tick, true, "ITC",
              "Run the mode manager's tick services (docs/functions/mode-tick.md: "
              "Billboards::FlipRing sub_8209BD18, ScreenFades::Init sub_8209CDE8, FadeIn "
              "sub_8209D038 and FadeOut sub_8209D0C8, Players::SignedInSlot sub_8209F658 and "
              "ActionMappers::SetBlocked sub_8209F318, Modes::SetChecks sub_82106868 (Stages::Copy's "
              "plain C++ called directly), Modes::SetSceneTargetMode sub_82106740 (with "
              "sub_82106778 merged into it), SignInWatch::CheckIdle sub_8209ED58 (no adapter: only "
              "SignInWatch::Update calls it), and "
              "SignInWatch::Update sub_8209EEC8 (with sub_8209F040 merged; wcscpy's and wcscmp's plain "
              "C++ called directly; the prompts' states 1 to 9 the original's), "
              "ScreenFades::Update sub_8209D130 (with ColorRect::Record sub_8217C348 merged; "
              "ScreenFades::RecordScissor sub_8209CE68 and ColorRect::RecordFlat sub_8217A370, no "
              "adapters: only it calls them; their scene getters and render queue submits guest "
              "calls), Music::Play sub_821063B0 (with lstrcmpA sub_82296130 and CompareStringA "
              "sub_82297100, XAPI's, no adapters: only it calls them in play; its player's "
              "slot 4 a guest call), Presence::SetForMode sub_822934B8 Dropped (presence isn't ported: only the "
              "last error its dropped XUserSetContextEx wrapper clears; sub_82293504 merged)) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=mode_tick)");

// Defines an adapter: the body (the arguments after the address) uses `ctx`
// and `base`.
#define ITC_MODE_TICK(address, ...)     \
  extern "C" REX_FUNC(sub_##address) {  \
    REX_FUNC_PROLOGUE();                \
    if (!ITC_FLAG(native_mode_tick)) {  \
      __imp__sub_##address(ctx, base);  \
      return;                           \
    }                                   \
    __VA_ARGS__;                        \
  }

using namespace mode_tick;

// Billboards::FlipRing: no arguments.
ITC_MODE_TICK(8209BD18, BillboardsFlipRing(base))

// ScreenFades::Init: no arguments.
ITC_MODE_TICK(8209CDE8, ScreenFadesInit(base))

// ScreenFades::FadeIn and FadeOut: r3 = the passes, r4, r5, r6 = the color,
// r7 = the layer (r3 left).
ITC_MODE_TICK(8209D038, ScreenFadesFadeIn(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                          ctx.r6.u32, ctx.r7.u32))
ITC_MODE_TICK(8209D0C8, ScreenFadesFadeOut(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                           ctx.r6.u32, ctx.r7.u32))

// Players::SignedInSlot: r3 = a player; r3 = the slot or -1 (sign-extended,
// as the original's li).
ITC_MODE_TICK(8209F658, ctx.r3.s64 = PlayersSignedInSlot(base, ctx.r3.u32))

// ActionMappers::SetBlocked: r3 = the value (left).
ITC_MODE_TICK(8209F318, ActionMappersSetBlocked(base, ctx.r3.u32))

// Modes::SetChecks: r3 = the holder (not read), r4 = the type; r3 left, but
// for type 1, Stages::Copy's out (the frame's +80, from the original's r1,
// whose high word its stwu leaves). Where Stages::Copy's plain C++ declines,
// the original from the start.
extern "C" REX_FUNC(sub_82106868) {
  REX_FUNC_PROLOGUE();
  const uint32_t frame = ctx.r1.u32 - kSetChecksFrame;
  if (!ITC_FLAG(native_mode_tick) || !ModesSetChecks(base, ctx.r4.u32, frame)) {
    __imp__sub_82106868(ctx, base);
    return;
  }
  if (ctx.r4.s32 == 1) {
    ctx.r3.u64 = ((ctx.r1.u64 & 0xFFFFFFFF00000000ull) | frame) + 80;
  }
}

// Modes::SetSceneTargetMode: r3 = the holder (not read), r4 = the type (r3
// left).
ITC_MODE_TICK(82106740, ModesSetSceneTargetMode(base, ctx.r4.u32))

// Presence::SetForMode: r3 = the type; r3 = 0 where the original's last
// XUserSetContextEx call answers it, else left.
ITC_MODE_TICK(822934B8, if (PresenceSetForMode(base, ctx.r13.u32, ctx.r3.u32)) ctx.r3.u64 = 0)

// SignInWatch::Update: r3 = 0x826C5B9C, r4 = the mode (both only for the
// prompts' handlers); r3 = the last name's address as wcscpy and memset
// leave it (sign-extended, as the original's addi). Where the plain C++
// says so (the prompts' states, CheckIdle's traps), the original.
extern "C" REX_FUNC(sub_8209EEC8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_mode_tick) ||
      !SignInWatchUpdate(base, ctx.r1.u32 - kSignInWatchUpdateFrame)) {
    __imp__sub_8209EEC8(ctx, base);
    return;
  }
  ctx.r3.s64 = int32_t(kLastName);
}

// --- ScreenFades::Update and its recording ---

// The command list's diagnostics (blend_state.cpp) see who allocates slots:
// with any of them on, the original runs.
ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);

namespace {

constexpr uint32_t kScene = 0x826BABA0;

// A call through slot `slot` of r3's vtable, returning to `return_address`.
void VtableCall(PPCContext& ctx, uint8_t* base, uint32_t slot, uint32_t return_address) {
  ctx.lr = return_address;
  REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 4 * slot));
}

// A chunk handed to the render queue `queue` (its slot 4, sub_8258EDC8 in
// play, which adds it to the queue's entries: r4 = the chunk, r5 = the key,
// f1 = the depth's float at `depth`).
void Submit(PPCContext& ctx, uint8_t* base, uint32_t queue, uint32_t chunk, uint32_t key,
            uint32_t depth, uint32_t return_address) {
  ctx.r3.u64 = queue;
  ctx.r4.u64 = chunk;
  ctx.r5.u64 = key;
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(guest::LoadFloat(base, depth));
  VtableCall(ctx, base, 4, return_address);
}

// ScreenFades::RecordScissor (r3 = the key, f1 = -10,000.0, not read) from
// a 160-byte frame like the original's: the scene's command list and render
// queue (four getters), then the two chunks natively, each handed on; where
// they wouldn't fit, the original (whose getters are then called again,
// never seen in play).
void RecordScissor(PPCContext& ctx, uint8_t* base, uint32_t key) {
  bool fit = false;
  guest_call::InFrame(ctx, base, 160, 0x8209CE90, [&](uint32_t) {
    ctx.r3.u64 = guest::LoadU32(base, kScene);
    VtableCall(ctx, base, 8, 0x8209CE90);
    VtableCall(ctx, base, 1, 0x8209CEA0);
    const uint32_t list = ctx.r3.u32;
    ctx.r3.u64 = guest::LoadU32(base, kScene);
    VtableCall(ctx, base, 8, 0x8209CEBC);
    VtableCall(ctx, base, 2, 0x8209CECC);
    const uint32_t queue = ctx.r3.u32;
    if (!mode_tick::ScissorChunksFit(base, list)) {
      return;
    }
    fit = true;
    Submit(ctx, base, queue, mode_tick::RecordScissorChunk(base, list), key, 0x82001C00,
           0x8209CF90);
    Submit(ctx, base, queue, mode_tick::RecordScissorEndChunk(base, list), key, 0x82001BFC,
           0x8209D030);
  });
  if (!fit) {
    ctx.r3.u64 = key;
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = double(guest::LoadFloat(base, 0x82001C00));
    ctx.lr = 0x8209D348;
    __imp__sub_8209CE68(ctx, base);
  }
}

// ColorRect::RecordFlat (r3 = the rectangle, r4 = the camera pass, r5 = the
// key) natively, its chunk handed to the render queue at [0x826D83D0] from a
// 368-byte frame like the original's; where the plain C++ declines, the
// original.
void RecordColorRect(PPCContext& ctx, uint8_t* base, uint32_t rect, uint32_t key) {
  const auto chunk = mode_tick::ColorRectRecordFlat(base, rect);
  if (!chunk) {
    ctx.r3.u64 = rect;
    ctx.r4.u64 = mode_tick::kFadeCameraPass;
    ctx.r5.u64 = key;
    ctx.lr = 0x8209D35C;
    __imp__sub_8217A370(ctx, base);
    return;
  }
  guest_call::InFrame(ctx, base, 368, 0x8217A74C, [&](uint32_t) {
    Submit(ctx, base, guest::LoadU32(base, mode_tick::kColorRectLists), *chunk, key, 0x82000A34,
           0x8217A74C);
  });
}

}  // namespace

void hooks::ModeTickScreenFadesUpdate(PPCContext& ctx, uint8_t* base) {
  if (ITC_FLAG(log_overlay_uploads) || ITC_FLAG(log_slot_callers) ||
      ITC_FLAG(log_recording_by_pass)) {
    __imp__sub_8209D130(ctx, base);
    return;
  }
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, base, kScreenFadesUpdateFrame, 0x8209D348, [&](uint32_t frame) {
    r3 = ScreenFadesUpdate(base, frame, [&](uint32_t key, uint32_t rect) {
      RecordScissor(ctx, base, key);
      RecordColorRect(ctx, base, rect, key + 1);
    });
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

// ScreenFades::Update: r3 not read; r3 = 1 while any count isn't 0, else 0.
ITC_MODE_TICK(8209D130, hooks::ModeTickScreenFadesUpdate(ctx, base))

// Music::Play: r3, r5 = two pointers passed on, r4 = the music's name; r3 as
// the call through [0x826C5C80]'s slot 4 leaves it, made from a 288-byte
// frame like the original's (the ids its r7 and r8 point at in it).
void hooks::ModeTickMusicPlay(PPCContext& ctx, uint8_t* base) {
  const uint32_t name = ctx.r4.u32;
  const uint64_t a = ctx.r3.u64, name64 = ctx.r4.u64, b = ctx.r5.u64;
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, kMusicPlayFrame, 0x82106538, [&](uint32_t frame) {
    const MusicIds ids = MusicPlay(base, ctx.r13.u32, frame, name);
    ctx.r3.u64 = guest::LoadU32(base, kMusicPlayer);
    ctx.r4.u64 = a;
    ctx.r5.u64 = name64;
    ctx.r6.u64 = b;
    // The frame's addresses as the original's addi from r1 (InFrame's r1).
    ctx.r7.u64 = ctx.r1.u64 + (ids.r7 - frame);
    ctx.r8.u64 = ctx.r1.u64 + (ids.r8 - frame);
    VtableCall(ctx, base, 4, 0x82106538);
  });
  ctx.lr = lr;
}

ITC_MODE_TICK(821063B0, hooks::ModeTickMusicPlay(ctx, base))
