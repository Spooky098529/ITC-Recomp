// The app's sign-in reader (src/hooks/notify.cpp) against the recompiled one:
// the recompiled reads an SDK listener with a sign-in changed queued (as at
// start-up) or nothing; ours builds the users the first time and does nothing
// after. Compared: the app object (its users, lists and flags; not the
// listener handle at +240, an SDK one or ours) and r3.

#include "harness.h"

#include <rex/logging.h>
#include <rex/system/kernel_state.h>
#include <rex/system/xnotifylistener.h>

namespace notify_hooks {
void SetUsersBuilt(bool built);
}

namespace harness {

void Notify(Context& h) {
  uint8_t* base = h.base;
  const uint32_t app = h.data;
  // An SDK listener for the recompiled reader (the app's areas, 3).
  PPCContext t = CallContext(h);
  t.r3.u64 = 3;
  t.r4.u64 = 0;
  __imp__XamNotifyCreateListener(t, base);
  const uint32_t listener = t.r3.u32;
  auto sdk_listener =
      REX_KERNEL_OBJECTS()->LookupObject<rex::system::XNotifyListener>(listener);
  if (!sdk_listener) {
    REXLOG_INFO("harness: notify: no SDK listener made");
    return;
  }
  Results results("App::PollSignIn (sub_82099828)");
  for (int queued = 1; queued >= 0; queued--) {
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      Fill(base, app, 512);
      guest::StoreU32(base, app + 240, v == 0 ? listener : 0x4E4F4C53);
      if (v == 0 && queued) {
        sdk_listener->EnqueueNotification(0xA, 1);
      }
      notify_hooks::SetUsersBuilt(!queued);
      PPCContext c = CallContext(h);
      c.r3.u64 = app;
      (v == 0 ? __imp__sub_82099828 : sub_82099828)(c, base);
      guest::StoreU32(base, app + 240, 0);  // the handles differ by design
      result[v] = Read(base, app, 256);
      for (int i = 0; i < 4; i++) {
        result[v].push_back(uint8_t(c.r3.u32 >> (24 - 8 * i)));
      }
    }
    results.Check(result[0], result[1],
                  queued ? "sign-in changed / first call" : "nothing / later calls");
  }
  notify_hooks::SetUsersBuilt(true);
  results.Report();
}

}  // namespace harness
