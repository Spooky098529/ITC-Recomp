// TEMPORARY (B3, XGetVideoMode's two callers): logs what each caller leaves
// in its object, native or recompiled (--video_mode_original), to compare.
// Removed when the check is done. See B3 in docs/rexglue-removal.md.

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <atomic>
#include <cstring>
#include <string>

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

// After sub_8258D568 (src/hooks/video_mode.cpp), native or recompiled.
void VideoModeCheck_AfterDisplaySetup(uint8_t* base, uint32_t object, uint32_t lr, uint32_t r6,
                                      uint32_t result) {
  if (!ShouldLog(display_setup_calls)) {
    return;
  }
  REXLOG_INFO("video mode check: sub_8258D568 #{} from {:08X}, object {:08X}, r6 {}, r3 {:08X}",
              display_setup_calls.load(), lr, object, r6, result);
  for (uint32_t offset = 16; offset < 312; offset += 64) {
    REXLOG_INFO("video mode check:   +{}: {}", offset, Words(base, object + offset, std::min(16u, (312 - offset) / 4)));
  }
  if (const uint32_t device = guest::LoadU32(base, object + 140)) {
    REXLOG_INFO("video mode check:   device +11608..+11616 {}, +15144 {}, +10332 {}, +24 {}",
                Words(base, device + 11608, 3), Words(base, device + 15144, 1),
                Words(base, device + 10332, 1), Words(base, device + 24, 2));
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
