// TEMPORARY (rexglue-removal B5 and B6, "Left to do" in
// docs/rexglue-removal.md): counts the functions still calling B5's and B6's
// imports (and main and exit, for xstart's quit path), to find which run, how
// often and from where, before each is marked "never called" or given what
// the game needs. Removed when the survey is done (a kept wrapper would block
// a real override).
//
// --log_leftovers logs each function's first calls and every power of two
// (caller, r3-r6 in, r3 out) and every 10 s the counts of those called. exit
// is logged on entry (it may not return), the others on return.

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <atomic>
#include <bit>
#include <chrono>
#include <mutex>
#include <string>

REXCVAR_DEFINE_BOOL(log_leftovers, false, "ITC",
                    "Log calls to the functions left calling B5's and B6's imports (temporary, "
                    "for rexglue-removal B5 and B6)");

namespace {

struct Function {
  uint32_t address;
  const char* name;
  std::atomic<uint64_t> calls{0};
};

#define LEFTOVERS(X)                                     \
  X(82459C20, "socket")                                  \
  X(82459C48, "ioctlsocket")                             \
  X(82459C60, "bind")                                    \
  X(822952C0, "XShowSigninUI")                           \
  X(8257B7A8, "Voice::Init")                             \
  X(8257E568, "voice profile read")                      \
  X(8257AF30, "Voice::Poll")                             \
  X(820A4610, "SaveDevices::Update")                     \
  X(82149080, "Live connection reader")                  \
  X(8214E268, "Network::Update")                         \
  X(8214F290, "SignIn::Update")                          \
  X(822AC530, "graphics debug print")                    \
  X(82486BC0, "DbgPrint caller 82486BC0")                \
  X(8248FBE0, "not implemented method")                  \
  X(822AC638, "sprintf caller 822AC638")                 \
  X(822B1AC8, "sprintf caller 822B1AC8")                 \
  X(82294DA0, "GlobalMemoryStatus")                      \
  X(82575D78, "XamAlloc/XamFree caller")                 \
  X(82090108, "main")

#define ENTRY(address, name) {0x##address, #address " " name},
Function g_functions[] = {LEFTOVERS(ENTRY) {0x825AA388, "825AA388 exit"}};
#undef ENTRY

#define INDEX(address, name) k##address,
enum Index { LEFTOVERS(INDEX) k825AA388 };
#undef INDEX

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
  for (auto& f : g_functions) {
    if (uint64_t n = f.calls.load()) {
      counts += fmt::format(" [{:08X}: {}]", f.address, n);
    }
  }
  REXLOG_INFO("leftovers: counts:{}", counts);
}

// Counts the call; true when it's one to log (the first 3 and powers of 2).
bool Count(Index i) {
  if (!REXCVAR_GET(log_leftovers)) {
    return false;
  }
  uint64_t n = ++g_functions[i].calls;
  Summary();
  return n <= 3 || std::has_single_bit(n);
}

struct In {
  uint32_t lr, r3, r4, r5, r6;
};

In Capture(PPCContext& ctx) {
  return {uint32_t(ctx.lr), ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32};
}

void Log(Index i, const In& in, const char* result) {
  REXLOG_INFO("leftovers: {} call {} from {:08X}: r3 {:08X} r4 {:08X} r5 {:08X} r6 {:08X} -> {}",
              g_functions[i].name, g_functions[i].calls.load(), in.lr, in.r3, in.r4, in.r5,
              in.r6, result);
}

}  // namespace

#define WRAPPER(address, name)                                    \
  extern "C" REX_FUNC(sub_##address) {                            \
    REX_FUNC_PROLOGUE();                                          \
    bool log = Count(k##address);                                 \
    const In in = Capture(ctx);                                   \
    __imp__sub_##address(ctx, base);                              \
    if (log) {                                                    \
      Log(k##address, in, fmt::format("{:08X}", ctx.r3.u32).c_str()); \
    }                                                             \
  }

LEFTOVERS(WRAPPER)
#undef WRAPPER

// exit: logged before the call, which may not return.
extern "C" REX_FUNC(sub_825AA388) {
  REX_FUNC_PROLOGUE();
  if (Count(k825AA388)) {
    Log(k825AA388, Capture(ctx), "(on entry)");
  }
  __imp__sub_825AA388(ctx, base);
}
