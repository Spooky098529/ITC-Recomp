// The user wrappers (src/hooks/user.cpp) against the recompiled ones, which
// ask the SDK: the name, sign-in state and XUID for users 0-5, the title id
// check, XUserSetContextEx's answer and the last error it leaves, and
// XShowSigninUI's answer.

#include "harness.h"

#include <rex/logging.h>

namespace harness {

void User(Context& h) {
  uint8_t* base = h.base;
  const uint32_t data = h.data;
  Random& r = h.random;
  {
    Results results("XUserGetName (sub_82295020)");
    for (uint32_t index = 0; index < 6; index++) {
      for (uint32_t size = 0; size <= 20; size++) {
        Both(h, results, [] {},
             [&](PPCContext& t) {
               t.r3.u64 = index;
               t.r4.u64 = data;
               t.r5.u64 = size;
             },
             __imp__sub_82295020, sub_82295020, data, 24,
             fmt::format("user {} size {}", index, size));
      }
    }
    results.Report();
  }
  {
    Results results("XUserGetSigninState (sub_82295028)");
    for (uint32_t index = 0; index < 6; index++) {
      Both(h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = index; }, __imp__sub_82295028,
           sub_82295028, data, 0, fmt::format("user {}", index));
    }
    results.Report();
  }
  {
    Results results("XUserGetXUID (sub_82295268)");
    for (uint32_t index = 0; index < 6; index++) {
      for (int null_out = 0; null_out < 2; null_out++) {
        Both(h, results, [] {},
             [&](PPCContext& t) {
               t.r3.u64 = index;
               t.r4.u64 = null_out ? 0 : data;
             },
             __imp__sub_82295268, sub_82295268, data, 8,
             fmt::format("user {}{}", index, null_out ? ", no out" : ""));
      }
    }
    results.Report();
  }
  {
    Results results("CheckTitleId (sub_82296EE0)");
    for (int i = 0; i < 200; i++) {
      uint32_t title_id;
      switch (i < 3 ? i : r.Below(3)) {
        case 0: title_id = 0; break;
        case 1: title_id = 0x55530000 | r.Below(0x10000); break;
        default: title_id = uint32_t(r.Next()); break;
      }
      Both(h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = title_id; }, __imp__sub_82296EE0,
           sub_82296EE0, data, 0, fmt::format("{:08X}", title_id));
    }
    results.Report();
  }
  {
    // The last error slot is compared, then put back.
    Results results("XUserSetContextEx (sub_82294F70)");
    const uint32_t pcr = h.ctx.r13.u32;
    const uint32_t slot = guest::LoadU32(base, pcr + 256) + 352;
    const uint32_t saved = guest::LoadU32(base, slot);
    for (int i = 0; i < 20; i++) {
      const uint32_t index = r.Below(4), context = i < 10 ? 0x8001 : r.Below(0x10000);
      const uint32_t value = r.Below(4);
      Both(h, results, [&] { guest::StoreU32(base, slot, 0xCDCDCDCD); },
           [&](PPCContext& t) {
             t.r3.u64 = index;
             t.r4.u64 = context;
             t.r5.u64 = value;
           },
           __imp__sub_82294F70, sub_82294F70, slot, 4,
           fmt::format("user {} context {:X} value {}", index, context, value));
    }
    guest::StoreU32(base, slot, saved);
    results.Report();
  }
  {
    // The recompiled one's SDK call also broadcasts sign-in changed and
    // system UI off, which reach only voice's listener.
    Results results("XShowSigninUI (sub_822952C0)");
    for (uint32_t panes : {1u, 2u, 4u}) {
      for (uint32_t flags : {0u, 1u, 2u}) {
        Both(h, results, [] {},
             [&](PPCContext& t) {
               t.r3.u64 = panes;
               t.r4.u64 = flags;
             },
             __imp__sub_822952C0, sub_822952C0, h.data, 0,
             fmt::format("panes {} flags {}", panes, flags));
      }
    }
    results.Report();
  }
}

}  // namespace harness
