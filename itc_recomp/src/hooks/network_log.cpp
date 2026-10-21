// TEMPORARY (rexglue-removal B5, batch 2 of the leftovers in
// docs/rexglue-removal.md): what Network::Update (sub_8214E268, which runs
// SignIn::Update, sub_8214F290) writes in play, to stub both with just the
// data the game reads. Removed when the survey is done (a kept wrapper would
// block a real override).
//
// --log_network_update snapshots the network manager's globals (826C6000-
// 826C7FFF) and 82618B7C-82618B87 around each call (since the stub, called
// from it, to compare with what the recompiled one wrote), and logs each word that
// changed: "in" during the call, "outside" between calls (other code). The
// first 1,000 changes in full, then every 10 s the words changed since the last
// summary (count and value). Also the first calls (and every power of two) of
// the functions it calls (only from inside it), with the caller and r3-r6.

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <array>
#include <atomic>
#include <bit>
#include <chrono>
#include <cstring>
#include <map>
#include <string>

REXCVAR_DEFINE_BOOL(log_network_update, false, "ITC",
                    "Log what Network::Update writes (temporary, for rexglue-removal B5)");

namespace {

constexpr uint32_t kRegion = 0x826C6000, kRegionBytes = 0x2000;
constexpr uint32_t kUsers = 0x82618B7C, kUsersBytes = 12;
constexpr uint32_t kWords = (kRegionBytes + kUsersBytes) / 4;

using Snapshot = std::array<uint32_t, kWords>;

uint32_t AddressOf(uint32_t i) {
  return i < kRegionBytes / 4 ? kRegion + 4 * i : kUsers + 4 * (i - kRegionBytes / 4);
}

void Take(uint8_t* base, Snapshot& s) {
  for (uint32_t i = 0; i < kWords; i++) {
    s[i] = guest::LoadU32(base, AddressOf(i));
  }
}

struct Change {
  uint64_t count = 0;
  uint32_t value = 0;
};

Snapshot g_after;
bool g_have_after = false;
thread_local bool g_inside = false;  // callees counted only from inside a call (this thread)
uint64_t g_calls = 0;
int g_logged = 0;
std::map<uint32_t, Change> g_since_summary;
std::chrono::steady_clock::time_point g_last_summary = std::chrono::steady_clock::now();

void Compare(const Snapshot& before, const Snapshot& after, const char* where) {
  for (uint32_t i = 0; i < kWords; i++) {
    if (before[i] == after[i]) {
      continue;
    }
    const uint32_t address = AddressOf(i);
    Change& c = g_since_summary[address];
    c.count++;
    c.value = after[i];
    if (g_logged < 1000) {
      g_logged++;
      REXLOG_INFO("network update: call {} {}: {:08X} {:08X} -> {:08X}", g_calls, where, address,
                  before[i], after[i]);
    }
  }
}

void Summary() {
  auto now = std::chrono::steady_clock::now();
  if (now - g_last_summary < std::chrono::seconds(10)) {
    return;
  }
  g_last_summary = now;
  std::string s;
  int n = 0;
  for (auto& [address, c] : g_since_summary) {
    if (n++ < 60) {
      s += fmt::format(" [{:08X}: {}x, {:08X}]", address, c.count, c.value);
    }
  }
  REXLOG_INFO("network update: {} calls; changed since the last summary ({} words):{}", g_calls,
              g_since_summary.size(), s);
  g_since_summary.clear();
}

struct Callee {
  uint32_t address;
  const char* name;
  std::atomic<uint64_t> calls{0};
};

#define CALLEES(X)                               \
  X(82149178, "profile read")                    \
  X(8214F810, "8214F810 (voice manager)")        \
  X(8214D7A0, "system link receive")             \
  X(8214ADC0, "8214ADC0 (state machine)")        \
  X(82106540, "82106540 (500, ...)")                \
  X(8214AC68, "notification 0x11 handler")       \
  X(8214AEA8, "voice data send")                 \
  X(8214F160, "8214F160")                        \
  X(82260908, "82260908 (per user)")             \
  X(822952E8, "XShowMessageBoxUI")               \
  X(822967E0, "XLaunchNewImage")

#define ENTRY(address, name) {0x##address, name},
Callee g_callees[] = {CALLEES(ENTRY)};
#undef ENTRY

#define INDEX(address, name) k##address,
enum Index { CALLEES(INDEX) };
#undef INDEX

Snapshot g_before;

}  // namespace

// Called by the Network::Update stub (src/hooks/network.cpp) around its work.
extern "C" void NetworkLog_Before(uint8_t* base) {
  if (!REXCVAR_GET(log_network_update)) {
    return;
  }
  Take(base, g_before);
  if (g_have_after) {
    Compare(g_after, g_before, "outside");
  }
  g_calls++;
  g_inside = true;
}

extern "C" void NetworkLog_After(uint8_t* base) {
  if (!REXCVAR_GET(log_network_update)) {
    return;
  }
  g_inside = false;
  Take(base, g_after);
  g_have_after = true;
  Compare(g_before, g_after, "in");
  Summary();
}

#define WRAPPER(address, label)                                                              \
  extern "C" REX_FUNC(sub_##address) {                                                      \
    REX_FUNC_PROLOGUE();                                                                    \
    const uint32_t lr = uint32_t(ctx.lr), r3 = ctx.r3.u32, r4 = ctx.r4.u32, r5 = ctx.r5.u32, \
                   r6 = ctx.r6.u32;                                                         \
    uint64_t n = REXCVAR_GET(log_network_update) && g_inside ? ++g_callees[k##address].calls : 0;       \
    __imp__sub_##address(ctx, base);                                                        \
    if (n && (n <= 3 || std::has_single_bit(n))) {                                          \
      REXLOG_INFO("network update: {} call {} (update call {}) from {:08X}: r3 {:08X} "     \
                  "r4 {:08X} r5 {:08X} r6 {:08X} -> {:08X}",                                \
                  g_callees[k##address].name, n, g_calls, lr, r3, r4, r5, r6, ctx.r3.u32);  \
    }                                                                                       \
  }

CALLEES(WRAPPER)
#undef WRAPPER
