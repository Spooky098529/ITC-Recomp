// XMP's dropped playback calls (src/hooks/xmp.cpp) against the recompiled
// ones: taking the playback and handing it back, in that order (as the boot
// movie does), each's answer and the last error it leaves; and the
// recompiled pair's state (82957778, 8295777C) back where it started.

#include "harness.h"

#include "base/log.h"

namespace harness {

void Xmp(Context& h) {
  uint8_t* base = h.base;
  const uint32_t pcr = h.ctx.r13.u32;
  const uint32_t slot = guest::LoadU32(base, pcr + 256) + 352;
  const uint32_t saved = guest::LoadU32(base, slot);
  const uint32_t taken = 0x82957778, was_playing = 0x8295777C;
  Results results("XMP take and hand back (sub_825894C8, sub_825895A0)");
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    for (GuestFunction f : {v == 0 ? __imp__sub_825894C8 : sub_825894C8,
                            v == 0 ? __imp__sub_825895A0 : sub_825895A0}) {
      guest::StoreU32(base, slot, 0xCDCDCDCD);
      PPCContext t = CallContext(h);
      f(t, base);
      for (uint32_t word : {t.r3.u32, guest::LoadU32(base, slot)}) {
        for (int i = 0; i < 4; i++) {
          result[v].push_back(uint8_t(word >> (24 - 8 * i)));
        }
      }
    }
  }
  results.Check(result[0], result[1], "take, then hand back");
  guest::StoreU32(base, slot, saved);
  results.Report();
  ITC_LOG_INFO("harness: XMP state after the recompiled pair: {:08X} {:08X} (0 0 expected)",
               guest::LoadU32(base, taken), guest::LoadU32(base, was_playing));
}

}  // namespace harness
