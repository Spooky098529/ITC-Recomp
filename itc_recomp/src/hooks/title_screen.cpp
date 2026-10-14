// The title screen's idle wait before the attract race (--attract_after_s).
//
// The title screen's update (sub_822221A0) counts idle ticks in +36 of the
// object at [0x82814E34] and, at 600 (20 s at 30 Hz), sets the object's
// state (+4) to 5, which loads an attract-mode race. See "The title screen"
// in docs/functions.md.

#include "guest.h"

#include <rex/cvar.h>

REXCVAR_DEFINE_INT32(attract_after_s, 0, "ITC",
                     "Testing: seconds the title screen waits before the attract race "
                     "(0 = the game's 20)");

namespace {

constexpr uint32_t kTitleScreenPointer = 0x82814E34;
constexpr uint32_t kIdleTicksOffset = 36;
constexpr int32_t kAttractTicks = 600;
constexpr int32_t kTicksPerSecond = 30;

}  // namespace

// TitleScreen_Update.
extern "C" REX_FUNC(sub_822221A0) {
  REX_FUNC_PROLOGUE();
  int32_t after = REXCVAR_GET(attract_after_s) * kTicksPerSecond;
  uint32_t title = guest::LoadU32(base, kTitleScreenPointer);
  int32_t before = title ? int32_t(guest::LoadU32(base, title + kIdleTicksOffset)) : 0;
  __imp__sub_822221A0(ctx, base);
  if (after <= 0 || !title || guest::LoadU32(base, kTitleScreenPointer) != title) {
    return;
  }
  // Only after a tick that counted (the counter went up by one): the next
  // one reaches the game's limit.
  int32_t now = int32_t(guest::LoadU32(base, title + kIdleTicksOffset));
  if (now == before + 1 && now >= after && now < kAttractTicks - 1) {
    guest::StoreU32(base, title + kIdleTicksOffset, uint32_t(kAttractTicks - 1));
  }
}
