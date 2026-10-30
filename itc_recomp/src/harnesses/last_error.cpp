// SetLastError (src/hooks/last_error.cpp) against the recompiled one: the
// error each stores in the thread's last error (put back afterwards) and r3,
// for SetLastError (both entry points) and SetLastError from an NTSTATUS,
// with statuses across every branch of the mapping; and GetLastError (both
// entry points against the recompiled sub_82296EC0, which the jump reaches):
// r3 and r11, with the thread's error random and [PCR+336] now and then set
// (put back afterwards).

#include "harness.h"

#include "base/log.h"

#include "hooks/last_error.h"

namespace harness {

namespace {

// GetLastError's register steps (sub_82296EC0 has no adapter: its only
// caller in play is sub_82295F18's).
void NativeGetLastError(PPCContext& ctx, uint8_t* base) {
  const uint32_t in_dpc = guest::LoadU32(base, ctx.r13.u32 + 336);
  ctx.r11.u64 = in_dpc ? in_dpc : guest::LoadU32(base, ctx.r13.u32 + 256);
  ctx.r3.u64 = last_error::Get(base, ctx.r13.u32);
}

}  // namespace

void LastError(Context& h) {
  uint8_t* base = h.base;
  Random& r = h.random;
  const uint32_t pcr = h.ctx.r13.u32;
  const uint32_t slot = guest::LoadU32(base, pcr + 256) + 352;
  const uint32_t saved = guest::LoadU32(base, slot);
  if (guest::LoadU32(base, pcr + 336)) {
    ITC_LOG_INFO("harness: PCR+336 is set, so SetLastError stores nothing here");
  }
  Results set("SetLastError (sub_82296E70, sub_82296FD0)");
  Results from_status("SetLastErrorFromStatus (sub_82296E88)");
  for (int i = 0; i < 3000; i++) {
    const int which = i % 3;
    uint32_t value;
    switch (r.Below(8)) {
      case 0: value = 0; break;
      case 1: value = 0xC0000000 | r.Below(0x400); break;
      case 2: value = 0x80000000 | r.Below(0x40); break;
      case 3: value = 0x80070000 | r.Below(0x10000); break;
      case 4: value = 0xD0000000 | r.Below(0x400); break;
      case 5: value = 0xC0010000 | r.Below(0x100); break;
      case 6: value = 0x40000000 | r.Below(0x100); break;
      default: value = uint32_t(r.Next()); break;
    }
    const GuestFunction recompiled =
        which == 0 ? __imp__sub_82296E70 : which == 1 ? __imp__sub_82296FD0 : __imp__sub_82296E88;
    const GuestFunction native = which == 0 ? sub_82296E70 : which == 1 ? sub_82296FD0 : sub_82296E88;
    Both(h, which == 2 ? from_status : set, [&] { guest::StoreU32(base, slot, 0xCDCDCDCD); },
         [&](PPCContext& t) { t.r3.u64 = value; }, recompiled, native, slot, 4,
         fmt::format("entry {} value {:08X}", which, value));
  }
  Results get("GetLastError (sub_82296EC0, sub_82295F18)");
  const uint32_t saved_dpc = guest::LoadU32(base, pcr + 336);
  for (int i = 0; i < 64; i++) {
    const uint32_t error = i % 8 == 0 ? 0 : uint32_t(r.Next());
    const uint32_t dpc = i % 4 == 3 ? (i % 8 == 3 ? 1 : uint32_t(r.Next()) | 1) : 0;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32(base, slot, error);
      guest::StoreU32(base, pcr + 336, dpc);
      PPCContext t = CallContext(h);
      t.r3.u64 = 0xCDCDCDCDCDCDCDCDull;
      t.r11.u64 = 0xCDCDCDCDCDCDCDCDull;
      (v == 0 ? __imp__sub_82296EC0 : i % 2 ? sub_82295F18 : NativeGetLastError)(t, base);
      guest::StoreU32(base, pcr + 336, saved_dpc);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r11.u32);
      out[v].Word(uint32_t(t.r11.u64 >> 32));
      out[v].Word(guest::LoadU32(base, slot));
    }
    get.Check(out[0], out[1], fmt::format("case {} entry {} error {:08X} dpc {:08X}", i, i % 2,
                                          error, dpc));
  }
  guest::StoreU32(base, slot, saved);
  set.Report();
  from_status.Report();
  get.Report();
}

}  // namespace harness
