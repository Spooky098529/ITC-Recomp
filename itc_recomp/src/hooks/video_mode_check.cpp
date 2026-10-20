// TEMPORARY (B3, XGetVideoMode's two callers): logs what each caller keeps
// of the SDK's answer, and --poison_video_mode_answer overwrites the answer
// sub_8258D568 leaves in its object, to see whether anything reads it.
// Removed when the callers are native. See B3 in docs/rexglue-removal.md.

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <atomic>
#include <cstring>
#include <string>

REXCVAR_DEFINE_BOOL(poison_video_mode_answer, false, "ITC",
                    "TEMPORARY: fill the XGetVideoMode answer sub_8258D568 keeps (+152..+199) "
                    "with 0xCD after it returns");

namespace {

bool ShouldLog(std::atomic<uint32_t>& count) {
  const uint32_t n = ++count;
  return n <= 4 || (n & (n - 1)) == 0;
}

std::string Words(uint8_t* base, uint32_t address, uint32_t count) {
  std::string s;
  for (uint32_t i = 0; i < count; i++) {
    char buffer[12];
    std::snprintf(buffer, sizeof(buffer), "%s%08X", i ? " " : "",
                  guest::LoadU32(base, address + 4 * i));
    s += buffer;
  }
  return s;
}

std::atomic<uint32_t> display_setup_calls{0};
std::atomic<uint32_t> aspect_calls{0};

}  // namespace

// The display set-up: r3 = the video output (Renderer + 64), r6 = mode.
extern "C" REX_FUNC(sub_8258D568) {
  REX_FUNC_PROLOGUE();
  const uint32_t object = ctx.r3.u32, r6 = ctx.r6.u32, lr = ctx.lr;
  __imp__sub_8258D568(ctx, base);
  if (ShouldLog(display_setup_calls)) {
    REXLOG_INFO("video mode check: sub_8258D568 #{} from {:08X}, object {:08X}, r6 {}, "
                "+152..+199: {}",
                display_setup_calls.load(), lr, object, r6, Words(base, object + 152, 12));
  }
  if (REXCVAR_GET(poison_video_mode_answer)) {
    std::memset(base + object + 152, 0xCD, 48);
    REXLOG_INFO("video mode check: +152..+199 poisoned");
  }
}

// After sub_82482B18 (src/hooks/video_mode.cpp), native or recompiled.
void VideoModeCheck_AfterAspect(uint8_t* base, uint32_t object, uint32_t lr, uint32_t result) {
  if (!ShouldLog(aspect_calls)) {
    return;
  }
  const uint32_t indices = guest::LoadU32(base, object + 60);
  REXLOG_INFO("video mode check: sub_82482B18 #{} from {:08X}, object {:08X}, r3 {:08X}, "
              "+132..+140 {}, +56..+68 {}, index buffer {}, its data {}",
              aspect_calls.load(), lr, object, result, Words(base, object + 132, 3),
              Words(base, object + 56, 4), indices ? Words(base, indices, 8) : "none",
              indices ? Words(base, guest::LoadU32(base, indices + 24), 3) : "none");
}
