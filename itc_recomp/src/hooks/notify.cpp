// The game's system notifications (XNotify) without listeners: what the game
// acts on is plugged in directly instead. Of the readers the listener wrapper
// served, only the app's ever acted on one: the sign-in changed the SDK sent
// at start-up, which built the user table. The others (the storage devices,
// Live connection, Network::Update's 0x11 and, around the dialogs, the
// sign-in helper's system UI and sign-in changes, which re-polled the same
// users) get none, as an empty queue. Of those, the storage devices' update
// runs without its read (the others went with Network::Update's stub,
// src/hooks/network.cpp). See B5 in docs/rexglue-removal.md.

#include "guest.h"
#include "lifted/functions.h"
#include "hooks/native_entry.h"
#include "guest_call.h"
#include "base/flags.h"
#include "hooks/save_interface.h"
#include "schema/system_services.h"
#include "schema/save_interface.h"
#include "schema/files_and_saves.h"
#include "user/network.h"
#include "lifted/sdk.h"

using guest_call::InFrame;

ITC_FLAG_DECLARE(bool, native_save_interface);

namespace {

// What the listener wrapper answers (user::kNoListener): no listener, but
// not 0 (the readers test for 0, and the storage devices' update runs only
// with one). Not an SDK handle (those start at 0xF8000000), so
// XNotifyGetNext, still called by the readers left recompiled, finds no
// queue and answers nothing.
constexpr uint32_t kNoListener = user::kNoListener;

bool g_users_built = false;

}  // namespace

namespace notify_hooks {

// For the harness: whether the app's reader has built the users.
void SetUsersBuilt(bool built) {
  g_users_built = built;
}

}  // namespace notify_hooks

// XNotifyCreateListener: r3 = the areas (mask). No listener made.
ITC_NATIVE_ENTRY_ALWAYS(822952B8) {
  
  ctx.r3.u64 = kNoListener;
}

// App::PollSignIn (every frame): r3 = the app. Builds the user table
// (sub_82099600) and flags it changed (+244) once, on the first call with a
// listener (+240), as the start-up sign-in changed did; returns 0 otherwise,
// as an empty queue did.
ITC_NATIVE_ENTRY_ALWAYS(82099828) {
  
  const uint32_t app = ctx.r3.u32;
  const schema::SysUsers users(base, app);
  if (g_users_built || !users.listener()) {
    ctx.r3.u64 = 0;
    return;
  }
  g_users_built = true;
  users.set_users_changed(1);
  InFrame(ctx, base, 112, 0x820998A0, [&](uint32_t) {
    ctx.r3.u64 = app;
    lifted::Enter(ctx, base, lifted::e82099600);
  });
}

namespace {

// Calls guest code at `function` with the link register the recompiled call
// had.
void CallAt(lifted::Regs& ctx, uint8_t* base, uint32_t function, uint32_t return_address) {
  ctx.lr = return_address;
  lifted::CallFromContext(ctx, base, function);
}

// An object's method: its vtable's word at `offset`.
uint32_t Method(uint8_t* base, uint32_t object, uint32_t offset) {
  return guest::LoadU32(base, guest::LoadU32(base, object) + offset);
}

}  // namespace

// SaveDevices::Update (every frame, through a vtable): r3 = the save devices.
// The game's slot state machines, without the storage devices changed
// notification (never sent) that re-checked each slot's device. Each slot
// (1,000 bytes at +44, count at +48) gets its device's state (+956) from the
// device list (+12's +4, then +8 per slot); a slot whose state isn't 0-3 is
// asked about (+28) and cleared under 5; then the slot's handler (the object's
// +76 + 4 x its +988) gives its next state. Runs only with a listener (+56).
// With --native_save_interface, its plain C++ instead (B7 loop,
// save_interface/slot_handlers.h: the handlers, the device list and
// SaveSlot::State native; its prologue written as the original's).
ITC_NATIVE_ENTRY_ALWAYS(820A4610) {
  
  if (ITC_FLAG(native_save_interface)) {
    hooks::SaveDevicesUpdateSteps(ctx, base);
    return;
  }
  // The save devices (save-interface.toml's SaveDevices) and their slots
  // (files-and-saves.toml's SaveSlot).
  const schema::SaveDevices devices(base, ctx.r3.u32);
  if (!devices.listener()) {
    return;
  }
  auto slot = [&](uint32_t i) {
    return schema::SaveSlot(base, devices.slots<schema::View>().address + 1000 * i);
  };
  auto count = [&] { return int32_t(devices.slot_count()); };
  InFrame(ctx, base, 144, 0x820A4640, [&](uint32_t) {
    const uint32_t source = devices.device_list();
    ctx.r3.u64 = source;  // r4 as the caller left it
    CallAt(ctx, base, Method(base, source, 4), 0x820A4640);
    const uint32_t list = ctx.r3.u32;
    for (int32_t i = 0; i < count(); i++) {
      ctx.r3.u64 = list;
      ctx.r4.u64 = uint32_t(i);
      CallAt(ctx, base, Method(base, list, 8), 0x820A4674);
      slot(i).set_user(ctx.r3.u32);
    }
    ctx.r3.u64 = 0;  // XNotifyGetNext's answer: nothing
    for (int32_t i = 0; i < count(); i++) {
      const int32_t state = int32_t(slot(i).user());
      if (state < 0 || state > 3) {
        ctx.r3.u64 = devices.address;
        ctx.r4.u64 = uint32_t(i);
        CallAt(ctx, base, Method(base, devices.address, 28), 0x820A4758);
        if (ctx.r3.u32 < 5) {
          // +36, +988, +984, +960, +964, +968, +972, in the original's order.
          slot(i).set_device(0);
          slot(i).set_state(0);
          slot(i).set_enumerated_saves(0);
          slot(i).set_request_device_selector(0);
          slot(i).set_request_enumeration(0);
          slot(i).set_request_save(0);
          slot(i).set_request_load(0);
        }
      }
      const schema::SaveSlot s = slot(i);
      // The handlers' table from +76 (wait_for_user), by the slot's state.
      const uint32_t handler =
          guest::LoadU32(base, devices.wait_for_user_address() + 4 * s.state());
      ctx.r3.u64 = devices.address;
      ctx.r4.u64 = s.address;
      CallAt(ctx, base, handler, 0x820A47D8);
      slot(i).set_state(ctx.r3.u32);
    }
  });
}
