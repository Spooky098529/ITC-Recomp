// SetLastError (src/hooks/last_error.cpp) against the recompiled one: the
// error each stores in the thread's last error (put back afterwards) and r3,
// for SetLastError (both entry points) and SetLastError from an NTSTATUS,
// with statuses across every branch of the mapping.

#include "harness.h"

#include <rex/logging.h>

namespace harness {

void LastError(Context& h) {
  uint8_t* base = h.base;
  Random& r = h.random;
  const uint32_t pcr = h.ctx.r13.u32;
  const uint32_t slot = guest::LoadU32(base, pcr + 256) + 352;
  const uint32_t saved = guest::LoadU32(base, slot);
  if (guest::LoadU32(base, pcr + 336)) {
    REXLOG_INFO("harness: PCR+336 is set, so SetLastError stores nothing here");
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
  guest::StoreU32(base, slot, saved);
  set.Report();
  from_status.Report();
}

}  // namespace harness
