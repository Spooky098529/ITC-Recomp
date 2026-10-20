// TEMPORARY (rexglue-removal B3, docs/rexglue-removal.md): counts the
// graphics library's functions that call the kernel's Vd* imports, to find
// which run, how often and from where, before any is made native. Removed
// once B3's natives replace these (a kept wrapper would block a real
// override).
//
// --log_graphics_boundary logs each function's first calls and every power
// of two (caller, r3-r5 in, r3 out, and fields the function sets), and every
// 10 s all the counts.

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <atomic>
#include <bit>
#include <chrono>
#include <mutex>
#include <string>

REXCVAR_DEFINE_BOOL(log_graphics_boundary, false, "ITC",
                    "Log calls to the graphics library's functions that call Vd* kernel "
                    "imports (temporary, for rexglue-removal B3)");

namespace {

struct Counter {
  const char* name;
  std::atomic<uint64_t> calls{0};
};

Counter g_counters[] = {
    {"8229B848 clock gating"},     {"8229F280 ring set-up"},     {"822A08A0 swap"},
    {"822ACB68 terminate"},        {"822ACE00 display mode"},    {"822AD0B0 set display mode"},
    {"822AD298 device init"},      {"822AD500 device release"},  {"822B2470 scaler rect"},
    {"822B3A78 scaler filters"},   {"822B4F10 capture?"},        {"822A0688 reads 7FC8xxxx"},
    {"822ACB48 engines callback"},
};
enum Index {
  kClockGating, kRingSetup, kSwap, kTerminate, kDisplayMode, kSetDisplayMode, kDeviceInit,
  kDeviceRelease, kScalerRect, kScalerFilters, kCapture, kRegisterRead, kEnginesCallback,
};

std::mutex g_summary_mutex;
std::chrono::steady_clock::time_point g_last_summary = std::chrono::steady_clock::now();

void Summary() {
  auto now = std::chrono::steady_clock::now();
  {
    std::lock_guard lock(g_summary_mutex);
    if (now - g_last_summary < std::chrono::seconds(10)) {
      return;
    }
    g_last_summary = now;
  }
  std::string counts;
  for (auto& c : g_counters) {
    counts += fmt::format(" [{}: {}]", c.name, c.calls.load());
  }
  REXLOG_INFO("graphics boundary: counts:{}", counts);
}

// Counts the call; true when it's one to log (the first 3 and powers of 2).
bool Count(Index i) {
  if (!REXCVAR_GET(log_graphics_boundary)) {
    return false;
  }
  uint64_t n = ++g_counters[i].calls;
  Summary();
  return n <= 3 || std::has_single_bit(n);
}

void Log(Index i, uint32_t lr, uint32_t r3, uint32_t r4, uint32_t r5, uint32_t result,
         const std::string& extra = {}) {
  REXLOG_INFO("graphics boundary: {} call {} from {:08X}: r3 {:08X} r4 {:08X} r5 {:08X} -> "
              "{:08X}{}",
              g_counters[i].name, g_counters[i].calls.load(), lr, r3, r4, r5, result, extra);
}

// The device's display fields (+19988..+20000), flags (+10808..+10811), the
// surfaces Device::SetUpDisplay stores (+13428, +13432, +13424) and the
// front buffer's format (+13252).
std::string DeviceFields(uint8_t* base, uint32_t device) {
  return fmt::format("; display {}x{} {} format {}; flags {:02X} {:02X} {:02X} {:02X}; "
                     "surfaces {:08X} {:08X} {:08X}, format {:08X}",
                     guest::LoadU32(base, device + 19988), guest::LoadU32(base, device + 19992),
                     guest::LoadU32(base, device + 19996), guest::LoadU32(base, device + 20000),
                     guest::LoadU8(base, device + 10808), guest::LoadU8(base, device + 10809),
                     guest::LoadU8(base, device + 10810), guest::LoadU8(base, device + 10811),
                     guest::LoadU32(base, device + 13428), guest::LoadU32(base, device + 13432),
                     guest::LoadU32(base, device + 13424), guest::LoadU32(base, device + 13252)) +
         fmt::format("; ring {:08X} mask {} write {} alloc {:08X} {:08X}; commands {:08X} last {:08X} "
                     "words {} segment {} [13516] {} [13520] {} [13524] {:08X} [13528] {:08X}; "
                     "blocks {:08X} {:08X}, progress +0 {:08X} +4 {:08X} +60 {:08X}; position {}",
                     guest::LoadU32(base, device + 13496), guest::LoadU32(base, device + 13500),
                     guest::LoadU32(base, device + 10820), guest::LoadU32(base, device + 13436),
                     guest::LoadU32(base, device + 13440), guest::LoadU32(base, device + 13504),
                     guest::LoadU32(base, device + 13508), guest::LoadU32(base, device + 13536),
                     guest::LoadU32(base, device + 13512), guest::LoadU32(base, device + 13516),
                     guest::LoadU32(base, device + 13520), guest::LoadU32(base, device + 13524),
                     guest::LoadU32(base, device + 13528), guest::LoadU32(base, device + 10768),
                     guest::LoadU32(base, device + 10772),
                     guest::LoadU32(base, guest::LoadU32(base, device + 10768)),
                     guest::LoadU32(base, guest::LoadU32(base, device + 10768) + 4),
                     guest::LoadU32(base, guest::LoadU32(base, device + 10768) + 60),
                     guest::LoadU32(base, device + 10780));
}

}  // namespace

// A plain counting wrapper; `extra` (an expression using base and r3)
// adds to the log line.
#define COUNTED(address, index, extra)                                          \
  extern "C" REX_FUNC(sub_##address) {                                          \
    REX_FUNC_PROLOGUE();                                                        \
    bool log = Count(index);                                                    \
    uint32_t lr = uint32_t(ctx.lr), r3 = ctx.r3.u32, r4 = ctx.r4.u32, r5 = ctx.r5.u32; \
    __imp__sub_##address(ctx, base);                                            \
    if (log) {                                                                  \
      Log(index, lr, r3, r4, r5, ctx.r3.u32, extra);                            \
    }                                                                           \
  }

COUNTED(8229B848, kClockGating, DeviceFields(base, r3))
COUNTED(822A08A0, kSwap, DeviceFields(base, r3))
COUNTED(822ACB68, kTerminate, std::string())
COUNTED(822AD500, kDeviceRelease, std::string())
COUNTED(822B2470, kScalerRect,
        fmt::format("; display {}x{}", guest::LoadU32(base, r3 + 19988),
                    guest::LoadU32(base, r3 + 19992)))
COUNTED(822B3A78, kScalerFilters, std::string())
COUNTED(822B4F10, kCapture, std::string())
COUNTED(822A0688, kRegisterRead, std::string())
COUNTED(822ACB48, kEnginesCallback, std::string())
