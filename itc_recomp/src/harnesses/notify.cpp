// The app's sign-in reader (src/hooks/notify.cpp) against the recompiled one:
// the recompiled reads an SDK listener with a sign-in changed queued (as at
// start-up) or nothing; ours builds the users the first time and does nothing
// after. Compared: the app object (its users, lists and flags; not the
// listener handle at +240, an SDK one or ours) and r3.
//
// Also SaveDevices::Update (ours without its notification read) on made-up save
// devices whose methods are recompiled getters and constants, against the
// recompiled one reading a listener handle the SDK doesn't know (no
// notification, as in play).

#include "harness.h"

#include "base/log.h"
#include <rex/system/kernel_state.h>
#include <rex/system/xnotifylistener.h>

namespace notify_hooks {
void SetUsersBuilt(bool built);
}

namespace harness {

namespace {

// Recompiled functions used as made-up methods.
constexpr uint32_t kReturnsWord24 = 0x82099BA0;  // lwz r3,24(r3)
constexpr uint32_t kReturns0 = 0x8210B174;
constexpr uint32_t kReturns1 = 0x824457CC;
constexpr uint32_t kReturns10 = 0x820A7178;
constexpr uint32_t kReturns11 = 0x820A7B70;
constexpr uint32_t kReturnsMinus1 = 0x824F65C0;

void SaveDevicesUpdate(Context& h) {
  uint8_t* base = h.base;
  Random& r = h.random;
  const uint32_t devices = h.data, devices_vtable = h.data + 512;
  const uint32_t source = h.data + 256, source_vtable = h.data + 576;
  const uint32_t list = h.data + 320, list_vtable = h.data + 640;
  const uint32_t slots = h.data + 1024;  // three of 1,000 bytes
  const uint32_t kStates[] = {kReturnsMinus1, kReturns1, kReturns10};
  const uint32_t kAnswers[] = {kReturns1, kReturns10};
  const uint32_t kHandlers[] = {kReturns0, kReturns1, kReturns11, kReturnsMinus1};
  Results results("SaveDevices::Update (sub_820A4610)");
  for (int i = 0; i < 60; i++) {
    const uint32_t count = i < 4 ? i : 1 + r.Below(3);
    const uint32_t state = kStates[r.Below(3)], answer = kAnswers[r.Below(2)];
    uint32_t handlers[4], next[3];
    for (auto& f : handlers) f = kHandlers[r.Below(4)];
    for (auto& n : next) n = r.Below(4);
    const bool listener = i != 4;
    const uint32_t r4 = r.Below(8);
    Both(h, results,
         [&] {
           guest::StoreU32(base, devices, devices_vtable);
           guest::StoreU32(base, devices + 12, source);
           guest::StoreU32(base, devices + 44, slots);
           guest::StoreU32(base, devices + 48, count);
           guest::StoreU32(base, devices + 56, listener ? 0x4E4F4C53 : 0);
           for (int k = 0; k < 4; k++) {
             guest::StoreU32(base, devices + 76 + 4 * k, handlers[k]);
           }
           guest::StoreU32(base, devices_vtable + 28, answer);
           guest::StoreU32(base, source, source_vtable);
           guest::StoreU32(base, source_vtable + 4, kReturnsWord24);
           guest::StoreU32(base, source + 24, list);
           guest::StoreU32(base, list, list_vtable);
           guest::StoreU32(base, list_vtable + 8, state);
           for (uint32_t k = 0; k < 3; k++) {
             guest::StoreU32(base, slots + 1000 * k + 988, next[k]);
           }
         },
         [&](PPCContext& t) {
           t.r3.u64 = devices;
           t.r4.u64 = r4;
         },
         __imp__sub_820A4610, sub_820A4610, h.data, kScratchBytes,
         fmt::format("{} slots, listener {}", count, listener));
  }
  results.Report();
}

}  // namespace

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
    ITC_LOG_INFO("harness: notify: no SDK listener made");
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
  SaveDevicesUpdate(h);
}

}  // namespace harness
