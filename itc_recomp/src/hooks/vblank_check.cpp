// TEMPORARY (B3, the vblank count): four functions read their object's
// +15148 (the device's vblank count's offset) and compare it with -1. Logs
// their object and what they read, to see whether it's the device. Removed
// when B3's interrupt item is done. See B3 in docs/rexglue-removal.md.

#include "guest.h"

#include <rex/logging.h>

#include <atomic>

namespace {

bool ShouldLog(std::atomic<uint32_t>& count) {
  const uint32_t n = ++count;
  return n <= 4 || (n & (n - 1)) == 0;
}

void Log(uint8_t* base, const char* name, std::atomic<uint32_t>& count, PPCContext& ctx) {
  if (ShouldLog(count)) {
    REXLOG_INFO("vblank check: {} #{} from {:08X}, r3 {:08X}: +3852 {:08X} +14776 {:08X} "
                "+15148 {:08X}",
                name, count.load(), ctx.lr, ctx.r3.u32, guest::LoadU32(base, ctx.r3.u32 + 3852),
                guest::LoadU32(base, ctx.r3.u32 + 14776), guest::LoadU32(base, ctx.r3.u32 + 15148));
  }
}

std::atomic<uint32_t> calls[4];

}  // namespace

extern "C" REX_FUNC(sub_825169A0) {
  REX_FUNC_PROLOGUE();
  Log(base, "sub_825169A0", calls[0], ctx);
  __imp__sub_825169A0(ctx, base);
}

extern "C" REX_FUNC(sub_82533010) {
  REX_FUNC_PROLOGUE();
  Log(base, "sub_82533010", calls[1], ctx);
  __imp__sub_82533010(ctx, base);
}

extern "C" REX_FUNC(sub_8253DEB0) {
  REX_FUNC_PROLOGUE();
  Log(base, "sub_8253DEB0", calls[2], ctx);
  __imp__sub_8253DEB0(ctx, base);
}

extern "C" REX_FUNC(sub_8256D470) {
  REX_FUNC_PROLOGUE();
  Log(base, "sub_8256D470", calls[3], ctx);
  __imp__sub_8256D470(ctx, base);
}
