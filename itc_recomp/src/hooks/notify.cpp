// The game's system notifications (XNotify) without listeners: what the game
// acts on is plugged in directly instead. Of the readers the listener wrapper
// served, only the app's ever acted on one: the sign-in changed the SDK sent
// at start-up, which built the user table. The others (the storage devices,
// Live connection, Network::Update's 0x11 and, around the dialogs, the
// sign-in helper's system UI and sign-in changes, which re-polled the same
// users) get none, as an empty queue. Of those, the Live connection reader
// answers its fixed result and the storage devices' update runs without its
// read. See B5 in docs/rexglue-removal.md.

#include "guest.h"
#include "guest_call.h"

using guest_call::InFrame;

namespace {

// What the listener wrapper answers: no listener, but not 0 (the readers test
// for 0, and the storage devices' update runs only with one). Not an SDK
// handle (those start at 0xF8000000), so XNotifyGetNext, still called by the
// readers left recompiled, finds no queue and answers nothing.
constexpr uint32_t kNoListener = 0x4E4F4C53;

bool g_users_built = false;

}  // namespace

namespace notify_hooks {

// For the harness: whether the app's reader has built the users.
void SetUsersBuilt(bool built) {
  g_users_built = built;
}

}  // namespace notify_hooks

// XNotifyCreateListener: r3 = the areas (mask). No listener made.
extern "C" REX_FUNC(sub_822952B8) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = kNoListener;
}

// App::PollSignIn (every frame): r3 = the app. Builds the user table
// (sub_82099600) and flags it changed (+244) once, on the first call with a
// listener (+240), as the start-up sign-in changed did; returns 0 otherwise,
// as an empty queue did.
extern "C" REX_FUNC(sub_82099828) {
  REX_FUNC_PROLOGUE();
  const uint32_t app = ctx.r3.u32;
  if (g_users_built || !guest::LoadU32(base, app + 240)) {
    ctx.r3.u64 = 0;
    return;
  }
  g_users_built = true;
  guest::StoreU32(base, app + 244, 1);
  InFrame(ctx, base, 112, 0x820998A0, [&](uint32_t) {
    ctx.r3.u64 = app;
    sub_82099600(ctx, base);
  });
}

namespace {

// Calls guest code at `function` with the link register the recompiled call
// had.
void CallAt(PPCContext& ctx, uint8_t* base, uint32_t function, uint32_t return_address) {
  ctx.lr = return_address;
  REX_CALL_INDIRECT_FUNC(function);
}

// An object's method: its vtable's word at `offset`.
uint32_t Method(uint8_t* base, uint32_t object, uint32_t offset) {
  return guest::LoadU32(base, guest::LoadU32(base, object) + offset);
}

}  // namespace

// The Live connection reader (from Network::Update, every frame): with no
// notification it asked the app for its Live connection state, which copies
// the app's +252 into 826C72F8 and answers its +248; both are 0 (set by the
// app's set-up and written only by App::PollSignIn on a Live connection
// change, which never comes). So it answers 0 and leaves 826C72F8 at 0.
extern "C" REX_FUNC(sub_82149080) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = 0;
}

// SaveDevices::Update (every frame, through a vtable): r3 = the save devices.
// The game's slot state machines, without the storage devices changed
// notification (never sent) that re-checked each slot's device. Each slot
// (1,000 bytes at +44, count at +48) gets its device's state (+956) from the
// device list (+12's +4, then +8 per slot); a slot whose state isn't 0-3 is
// asked about (+28) and cleared under 5; then the slot's handler (the object's
// +76 + 4 x its +988) gives its next state. Runs only with a listener (+56).
extern "C" REX_FUNC(sub_820A4610) {
  REX_FUNC_PROLOGUE();
  const uint32_t devices = ctx.r3.u32;
  if (!guest::LoadU32(base, devices + 56)) {
    return;
  }
  auto slot = [&](uint32_t i) { return guest::LoadU32(base, devices + 44) + 1000 * i; };
  auto count = [&] { return int32_t(guest::LoadU32(base, devices + 48)); };
  InFrame(ctx, base, 144, 0x820A4640, [&](uint32_t) {
    const uint32_t source = guest::LoadU32(base, devices + 12);
    ctx.r3.u64 = source;  // r4 as the caller left it
    CallAt(ctx, base, Method(base, source, 4), 0x820A4640);
    const uint32_t list = ctx.r3.u32;
    for (int32_t i = 0; i < count(); i++) {
      ctx.r3.u64 = list;
      ctx.r4.u64 = uint32_t(i);
      CallAt(ctx, base, Method(base, list, 8), 0x820A4674);
      guest::StoreU32(base, slot(i) + 956, ctx.r3.u32);
    }
    ctx.r3.u64 = 0;  // XNotifyGetNext's answer: nothing
    for (int32_t i = 0; i < count(); i++) {
      const int32_t state = int32_t(guest::LoadU32(base, slot(i) + 956));
      if (state < 0 || state > 3) {
        ctx.r3.u64 = devices;
        ctx.r4.u64 = uint32_t(i);
        CallAt(ctx, base, Method(base, devices, 28), 0x820A4758);
        if (ctx.r3.u32 < 5) {
          for (uint32_t offset : {36, 988, 984, 960, 964, 968, 972}) {
            guest::StoreU32(base, slot(i) + offset, 0);
          }
        }
      }
      const uint32_t s = slot(i);
      const uint32_t handler =
          guest::LoadU32(base, devices + 4 * (guest::LoadU32(base, s + 988) + 19));
      ctx.r3.u64 = devices;
      ctx.r4.u64 = s;
      CallAt(ctx, base, handler, 0x820A47D8);
      guest::StoreU32(base, slot(i) + 988, ctx.r3.u32);
    }
  });
}
