// The game's system notifications (XNotify) without listeners: what the game
// acts on is plugged in directly instead. Of the readers the listener wrapper
// served, only the app's ever acted on one: the sign-in changed the SDK sent
// at start-up, which built the user table. The others (the storage devices,
// Live connection, Network::Update's 0x11 and, around the dialogs, the
// sign-in helper's system UI and sign-in changes, which re-polled the same
// users) get none, as an empty queue. See B5 in docs/rexglue-removal.md.

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
