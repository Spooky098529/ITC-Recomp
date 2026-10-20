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
    {"822A08A0 swap"},
};
enum Index {
  kSwap,
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

// Swap (sub_822A08A0): what each of its branches tests, read at entry, and
// how often each would run, by caller; logged with the fields Swap leaves
// at its first calls and powers of two.
namespace {

enum SwapEvent {
  kSwapTimestamps,     // +20080 set and +20076 - +20072 < 6: sub_822B1F68's packets
  kSwapGlobal37C0,     // 829537C0 set: the segment may end first
  kSwapCapture,        // sub_822B5C58 does something (82953E1C or +20424 bit 3)
  kSwapObject19980,    // +19980 set: no VdSwap, the stack buffer zeroed
  kSwapSetDisplayMode, // +10809 bit 4: Device::SetDisplayMode, REG_RMW, VdPersistDisplay
  kSwapParameters,     // r5 set: compared with +13284 (sub_822B42D0)
  kSwapScalerFlag,     // +10810 bit 5: Device::SetUpScaler and the 10-word packet
  kSwapGammaFlag,      // +10811 bit 7: sub_822A05A8
  kSwapFrontBuffer,    // +20004 differs from the front buffer: register 0x1844
  kSwapHung,           // +10809 bit 1: the 7FC86110 write
  kSwapCaptureState,   // 82953D10 (sub_822B1788's state) set
  kSwapStats,          // +15168 or +20104 set (sub_822B20D8's debugger and counter paths)
  kSwapFenceAhead,     // the fence sub_8229EC00 waits on is past progress +0
  kSwapEvents,
};
const char* const kSwapEventNames[kSwapEvents] = {
    "timestamps", "829537C0", "capture", "+19980", "set display mode", "r5", "scaler",
    "gamma", "front buffer", "hung", "capture state", "stats", "fence ahead",
};
std::atomic<uint64_t> g_swap_events[kSwapEvents];
// By caller: Renderer::Present, the movies, the tiling worker, sub_8209ACB8's, others.
constexpr uint32_t kSwapCallers[] = {0x8258DCA4, 0, 0, 0};
std::atomic<uint64_t> g_swap_callers[5];
std::mutex g_swap_callers_mutex;
uint32_t g_swap_other_callers[4];

std::string SwapEntryFields(uint8_t* base, uint32_t device, uint32_t front_buffer) {
  return fmt::format(
      "flags {:02X} {:02X} {:02X} {:02X}; [20080] {} [20072] {} [20076] {}; [829537C0] {:08X} "
      "[82953D10] {:08X} [82953E1C] {:02X} [20424] {:02X}; [19980] {:08X}; [20004] {:08X} "
      "front buffer {:08X}; [15168] {:08X} [20104] {} [20440] {:08X}; fences [13540] {} "
      "[13544] {}; progress +0 {}; position {}; write {:08X} end {:08X} soft end {:08X}",
      guest::LoadU8(base, device + 10808), guest::LoadU8(base, device + 10809),
      guest::LoadU8(base, device + 10810), guest::LoadU8(base, device + 10811),
      guest::LoadU32(base, device + 20080), guest::LoadU32(base, device + 20072),
      guest::LoadU32(base, device + 20076), guest::LoadU32(base, 0x829537C0),
      guest::LoadU32(base, 0x82953D10), guest::LoadU8(base, 0x82953E1C),
      guest::LoadU8(base, device + 20424), guest::LoadU32(base, device + 19980),
      guest::LoadU32(base, device + 20004), guest::LoadU32(base, front_buffer + 32) & ~0xFFFu,
      guest::LoadU32(base, device + 15168), guest::LoadU32(base, device + 20104),
      guest::LoadU32(base, device + 20440), guest::LoadU32(base, device + 13540),
      guest::LoadU32(base, device + 13544),
      guest::LoadU32(base, guest::LoadU32(base, device + 10768)),
      guest::LoadU32(base, device + 10780), guest::LoadU32(base, device + 40),
      guest::LoadU32(base, device + 44), guest::LoadU32(base, device + 48));
}

std::string SwapExitFields(uint8_t* base, uint32_t device) {
  std::string dirty;
  for (uint32_t i = 0; i < 40; i += 8) {
    dirty += fmt::format(" {:016X}", uint64_t(REX_LOAD_U64(device + i)));
  }
  return fmt::format(
      "fences [13540] {} [13544] {}; [20120] {:08X} [20432] {:08X} [20004] {:08X}; frames "
      "[20024] {} [20028] {}; segment [13528] {:08X} [13516] {} [13532] {}; write {:08X} end "
      "{:08X} soft end {:08X}; position {}; dirty{}",
      guest::LoadU32(base, device + 13540), guest::LoadU32(base, device + 13544),
      guest::LoadU32(base, device + 20120), guest::LoadU32(base, device + 20432),
      guest::LoadU32(base, device + 20004), guest::LoadU32(base, device + 20024),
      guest::LoadU32(base, device + 20028), guest::LoadU32(base, device + 13528),
      guest::LoadU32(base, device + 13516), guest::LoadU32(base, device + 13532),
      guest::LoadU32(base, device + 40), guest::LoadU32(base, device + 44),
      guest::LoadU32(base, device + 48), guest::LoadU32(base, device + 10780), dirty);
}

void CountSwap(uint8_t* base, uint32_t lr, uint32_t device, uint32_t front_buffer,
               uint32_t parameters) {
  auto event = [](SwapEvent e, bool happened) {
    if (happened) {
      g_swap_events[e]++;
    }
  };
  uint8_t flags1 = guest::LoadU8(base, device + 10809);
  uint32_t fence = guest::LoadU32(base, device + 13540);
  if (guest::LoadU32(base, device + 20440) & 1) {
    fence = guest::LoadU32(base, device + 13544);
  }
  uint32_t progress = guest::LoadU32(base, guest::LoadU32(base, device + 10768));
  event(kSwapTimestamps, guest::LoadU32(base, device + 20080) &&
                             guest::LoadU32(base, device + 20076) -
                                     guest::LoadU32(base, device + 20072) < 6);
  event(kSwapGlobal37C0, guest::LoadU32(base, 0x829537C0));
  event(kSwapCapture, guest::LoadU8(base, 0x82953E1C) || (guest::LoadU8(base, device + 20424) & 8));
  event(kSwapObject19980, guest::LoadU32(base, device + 19980));
  event(kSwapSetDisplayMode, flags1 & 0x10);
  event(kSwapParameters, parameters);
  event(kSwapScalerFlag, guest::LoadU8(base, device + 10810) & 0x20);
  event(kSwapGammaFlag, guest::LoadU8(base, device + 10811) & 0x80);
  event(kSwapFrontBuffer, guest::LoadU32(base, device + 20004) !=
                              (guest::LoadU32(base, front_buffer + 32) & ~0xFFFu));
  event(kSwapHung, flags1 & 0x2);
  event(kSwapCaptureState, guest::LoadU32(base, 0x82953D10));
  event(kSwapStats, guest::LoadU32(base, device + 15168) || guest::LoadU32(base, device + 20104));
  event(kSwapFenceAhead, fence && int32_t(fence - progress) > 0);
  size_t caller = 4;
  if (lr == kSwapCallers[0]) {
    caller = 0;
  } else {
    std::lock_guard lock(g_swap_callers_mutex);
    for (size_t i = 0; i < 4; i++) {
      if (g_swap_other_callers[i] == lr || !g_swap_other_callers[i]) {
        g_swap_other_callers[i] = lr;
        caller = i < 3 ? i + 1 : 4;
        break;
      }
    }
  }
  g_swap_callers[caller]++;
}

void SwapSummary() {
  static std::atomic<uint64_t> last{0};
  uint64_t calls = g_counters[kSwap].calls.load();
  // Every 1,024 swaps (the counts' own summary only runs every 10 s).
  if (calls / 1024 == last.load()) {
    return;
  }
  last = calls / 1024;
  std::string events;
  for (int e = 0; e < kSwapEvents; e++) {
    events += fmt::format(" [{}: {}]", kSwapEventNames[e], g_swap_events[e].load());
  }
  std::string callers = fmt::format(" [8258DCA4: {}]", g_swap_callers[0].load());
  {
    std::lock_guard lock(g_swap_callers_mutex);
    for (size_t i = 0; i < 3; i++) {
      if (g_swap_other_callers[i]) {
        callers += fmt::format(" [{:08X}: {}]", g_swap_other_callers[i],
                               g_swap_callers[i + 1].load());
      }
    }
  }
  callers += fmt::format(" [others: {}]", g_swap_callers[4].load());
  REXLOG_INFO("graphics boundary: swap {}: events:{}; callers:{}", calls, events, callers);
}

}  // namespace

extern "C" REX_FUNC(sub_822A08A0) {
  REX_FUNC_PROLOGUE();
  bool log = Count(kSwap);
  uint32_t lr = uint32_t(ctx.lr), r3 = ctx.r3.u32, r4 = ctx.r4.u32, r5 = ctx.r5.u32;
  std::string entry;
  if (REXCVAR_GET(log_graphics_boundary)) {
    CountSwap(base, lr, r3, r4, r5);
    if (log) {
      entry = SwapEntryFields(base, r3, r4);
    }
  }
  __imp__sub_822A08A0(ctx, base);
  if (log) {
    Log(kSwap, lr, r3, r4, r5, ctx.r3.u32,
        "; at entry: " + entry + "; after: " + SwapExitFields(base, r3) + DeviceFields(base, r3));
  }
  if (REXCVAR_GET(log_graphics_boundary)) {
    SwapSummary();
  }
}

// TEMPORARY (B3, Swap): skip Swap's capture check and frame statistics to
// show nothing the renderer uses reads what they leave.
REXCVAR_DEFINE_BOOL(swap_skip_capture_and_stats, false, "ITC",
                    "Skip sub_822B5C58 and sub_822B20D8 in Swap (temporary, for "
                    "rexglue-removal B3)");

extern "C" REX_FUNC(sub_822B5C58) {
  REX_FUNC_PROLOGUE();
  if (!REXCVAR_GET(swap_skip_capture_and_stats)) {
    __imp__sub_822B5C58(ctx, base);
  }
}

extern "C" REX_FUNC(sub_822B20D8) {
  REX_FUNC_PROLOGUE();
  if (!REXCVAR_GET(swap_skip_capture_and_stats)) {
    __imp__sub_822B20D8(ctx, base);
  }
}
