#include "replay_stats.h"

#include "guest.h"
#include <array>
#include <chrono>
#include <string>
#include <rex/logging.h>
#include <rex/cvar.h>

REXCVAR_DEFINE_BOOL(log_replay_commands, false, "ITC",
                    "Log how often each render command is replayed per frame, and sample "
                    "arguments of the unidentified ones, every 2 s");

namespace {

constexpr uint32_t kCommandCount = 38;

// Commands not identified from the code yet (docs/gpu-functions.md).
constexpr bool IsUnknown(uint32_t id) {
  switch (id) {
    case 3: case 8: case 9: case 10: case 12: case 20: case 22: case 28: case 32: case 33:
      return true;
    default:
      return false;
  }
}

struct Stats {
  std::array<uint64_t, kCommandCount> commands{};  // calls
  std::array<uint64_t, kCommandCount> items{};     // sum of arg1
  std::array<std::string, kCommandCount> sample;   // last arguments seen
  uint32_t frames = 0;
  std::chrono::steady_clock::time_point last_log = std::chrono::steady_clock::now();
};

// Render thread only.
Stats& GetStats() {
  static Stats stats;
  return stats;
}

}  // namespace

void debug::ReplayStats_OnCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t arg1,
                                  uint32_t arg2) {
  if (!REXCVAR_GET(log_replay_commands) || id >= kCommandCount) {
    return;
  }
  Stats& s = GetStats();
  s.commands[id]++;
  s.items[id] += arg1;
  if (IsUnknown(id)) {
    std::string words;
    for (uint32_t i = 0; i < 8; i++) {
      words += fmt::format(" {:08X}", guest::LoadU32(base, payload + 4 * i));
    }
    s.sample[id] = fmt::format("arg1 {} arg2 {:08X} payload{}", arg1, arg2, words);
  }
}

void debug::ReplayStats_OnFrame() {
  if (!REXCVAR_GET(log_replay_commands)) {
    return;
  }
  Stats& s = GetStats();
  s.frames++;
  auto now = std::chrono::steady_clock::now();
  if (now - s.last_log < std::chrono::seconds(2)) {
    return;
  }
  std::string line;
  for (uint32_t id = 0; id < kCommandCount; id++) {
    if (s.commands[id]) {
      line += fmt::format(" | {}: {:.1f} ({:.1f} items)", id, double(s.commands[id]) / s.frames,
                          double(s.items[id]) / s.frames);
    }
  }
  REXLOG_INFO("replay commands per frame ({} frames):{}", s.frames, line);
  for (uint32_t id = 0; id < kCommandCount; id++) {
    if (!s.sample[id].empty()) {
      REXLOG_INFO("replay command {} sample: {}", id, s.sample[id]);
    }
  }
  s = {};
  s.last_log = now;
}

// Command handlers, wrapped to count them. Command 7 (sub_8258FD48) is
// counted in its wrapper in blend_state.cpp. Ids 0, 36 and 37
// (sub_82304258) aren't counted: that function is also used elsewhere.
#define REPLAY_COUNTED_HANDLER(address, id)                                         \
  extern "C" REX_FUNC(sub_##address) {                                              \
    REX_FUNC_PROLOGUE();                                                            \
    debug::ReplayStats_OnCommand(base, id, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);     \
    __imp__sub_##address(ctx, base);                                                \
  }

REPLAY_COUNTED_HANDLER(8258FBD0, 1)
REPLAY_COUNTED_HANDLER(8258FC30, 2)
REPLAY_COUNTED_HANDLER(8258FC90, 3)
REPLAY_COUNTED_HANDLER(8258FCE0, 4)
REPLAY_COUNTED_HANDLER(8258E9F8, 5)
REPLAY_COUNTED_HANDLER(8258EA20, 6)
REPLAY_COUNTED_HANDLER(8258FF50, 8)
REPLAY_COUNTED_HANDLER(8258FFA0, 9)
REPLAY_COUNTED_HANDLER(8258FFF0, 10)
REPLAY_COUNTED_HANDLER(82590040, 11)
REPLAY_COUNTED_HANDLER(825900B8, 12)
REPLAY_COUNTED_HANDLER(8258EA58, 13)  // also 14: one handler
REPLAY_COUNTED_HANDLER(82590108, 15)
REPLAY_COUNTED_HANDLER(825901B0, 16)
REPLAY_COUNTED_HANDLER(82590268, 17)
REPLAY_COUNTED_HANDLER(825902E0, 18)
REPLAY_COUNTED_HANDLER(82590360, 19)
REPLAY_COUNTED_HANDLER(825903D8, 20)
REPLAY_COUNTED_HANDLER(8258EAD0, 21)
REPLAY_COUNTED_HANDLER(82591798, 22)
REPLAY_COUNTED_HANDLER(82590428, 23)
REPLAY_COUNTED_HANDLER(82590478, 24)
REPLAY_COUNTED_HANDLER(825904C8, 25)
REPLAY_COUNTED_HANDLER(82590620, 26)
REPLAY_COUNTED_HANDLER(82590698, 27)
REPLAY_COUNTED_HANDLER(8258EAE0, 28)
REPLAY_COUNTED_HANDLER(82590768, 29)
REPLAY_COUNTED_HANDLER(82590860, 30)
REPLAY_COUNTED_HANDLER(825908E0, 31)
REPLAY_COUNTED_HANDLER(82590968, 32)
REPLAY_COUNTED_HANDLER(825909E8, 33)
REPLAY_COUNTED_HANDLER(82590A68, 34)
REPLAY_COUNTED_HANDLER(82590AD0, 35)
