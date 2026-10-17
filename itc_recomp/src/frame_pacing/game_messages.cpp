#include "game_messages.h"

#include "debug/frame_state_diff.h"
#include "debug/idle_timer_scan.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "native/renderer.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstring>
#include <vector>
#include <string>
#include <map>
#include <thread>

REXCVAR_DEFINE_BOOL(log_game_messages, false, "ITC",
                    "Log game messages delivered and skipped, once a second");
REXCVAR_DEFINE_STRING(render_only_update_handlers, "", "ITC",
                      "Testing: message handler targets (hex, comma-separated) that still get "
                      "Update on render-only passes");
REXCVAR_DEFINE_BOOL(redraw_race_overlays, true, "ITC",
                    "Record the race overlays (the marker above opponent cars) on render-only "
                    "passes too");
REXCVAR_DEFINE_BOOL(log_recording_handlers, false, "ITC",
                    "Log which message handlers record render commands, by message, once a "
                    "second");
// Testing: main loop tick handlers or message handlers to skip on
// render-only passes, by target (comma-separated hex), to find which one
// counts frames.
REXCVAR_DEFINE_STRING(render_only_skip, "", "ITC",
                      "Testing: tick handler or message handler targets (hex, "
                      "comma-separated) to skip on render-only passes");

namespace frame_pacing {
namespace {
uint32_t g_current_handler = 0;
uint32_t g_current_message = ~0u;
}  // namespace

void GameMessages_CurrentHandler(uint32_t& handler, uint32_t& message) {
  handler = g_current_handler;
  message = g_current_message;
}

namespace {

bool InTargetList(const std::string& list, uint32_t target) {
  size_t start = 0;
  while (start < list.size()) {
    size_t end = list.find(',', start);
    std::string item = list.substr(start, end == std::string::npos ? std::string::npos : end - start);
    if (!item.empty() && uint32_t(std::stoul(item, nullptr, 16)) == target) {
      return true;
    }
    if (end == std::string::npos) {
      break;
    }
    start = end + 1;
  }
  return false;
}

bool SkipOnRenderOnlyPass(uint32_t target) {
  static std::string parsed_from;
  static std::vector<uint32_t> targets;
  std::string list = REXCVAR_GET(render_only_skip);
  if (list != parsed_from) {
    parsed_from = list;
    targets.clear();
    size_t start = 0;
    while (start < list.size()) {
      size_t end = list.find(',', start);
      std::string item = list.substr(start, end == std::string::npos ? std::string::npos : end - start);
      if (!item.empty()) {
        targets.push_back(uint32_t(std::stoul(item, nullptr, 16)));
      }
      if (end == std::string::npos) {
        break;
      }
      start = end + 1;
    }
  }
  return std::find(targets.begin(), targets.end(), target) != targets.end();
}

// Message dispatcher object (r3 of sub_821062E8).
constexpr uint32_t kHandlersOffset = 0;  // array of handler pointers
constexpr uint32_t kHandlerCountOffset = 200;
constexpr uint32_t kQueueOffset = 204;  // ring of messages
constexpr uint32_t kQueueCountOffset = 804;
constexpr uint32_t kQueueReadIndexOffset = 808;
constexpr uint32_t kQueueCapacity = 50;
constexpr uint32_t kMessageBytes = 12;  // id and two arguments

enum MessageId : uint32_t {
  kUpdate = 0,
  kDraw = 1,
};

bool g_render_only_pass = false;
// A render-only pass after the tick's first: Update and Draw aren't
// delivered, the render thread draws the first's stored list again
// (--native_renderer_tick_replay).
bool g_fast_pass = false;
int g_render_only_since_logic = 0;
bool g_race_draw_step_seen = false;  // during the current logic pass
bool g_in_race = false;              // as of the last logic pass

struct Counts {
  int updates = 0;
  int updates_skipped = 0;
  int draws = 0;
  int others = 0;
  std::chrono::steady_clock::time_point last_log{};
};

Counts& GetCounts() {
  static Counts counts;
  return counts;
}

void LogCounts() {
  if (!REXCVAR_GET(log_game_messages)) {
    return;
  }
  Counts& c = GetCounts();
  auto now = std::chrono::steady_clock::now();
  if (now - c.last_log < std::chrono::seconds(1)) {
    return;
  }
  REXLOG_INFO("game messages: {} updates ({} skipped), {} draws, {} others", c.updates,
              c.updates_skipped, c.draws, c.others);
  c = {};
  c.last_log = now;
}

// Diagnostics (--log_recording_handlers): command slots recorded per message
// handler, by message id, once a second.
void LogRecording(uint32_t message, uint32_t handler, int slots) {
  static std::map<std::pair<uint32_t, uint32_t>, std::pair<int, int>> recorded;
  static auto last = std::chrono::steady_clock::now();
  auto& entry = recorded[{message, handler}];
  entry.first++;
  entry.second += slots;
  auto now = std::chrono::steady_clock::now();
  if (now - last < std::chrono::seconds(1)) {
    return;
  }
  last = now;
  for (const auto& [key, value] : recorded) {
    REXLOG_INFO("recording: message {} handler {:08X}: {} calls, {} slots", key.first,
                key.second, value.first, value.second);
  }
  recorded.clear();
}

// Delivers one message to every handler, as the game does.
void Deliver(PPCContext& ctx, uint8_t* base, uint32_t dispatcher, uint32_t message,
             const std::string* only_targets = nullptr) {
  uint32_t handler_count = guest::LoadU32(base, dispatcher + kHandlerCountOffset);
  for (uint32_t i = 0; i < handler_count; i++) {
    // sub_82106280 picks the object that handles this message for handler i;
    // its first virtual method takes it.
    ctx.r3.u64 = guest::LoadU32(base, dispatcher + kHandlersOffset + 4 * i);
    ctx.r4.u64 = message;
    ctx.lr = 0x82106370;
    sub_82106280(ctx, base);
    uint32_t object = guest::LoadU32(base, ctx.r3.u32);
    uint32_t method = guest::LoadU32(base, guest::LoadU32(base, object));
    ctx.r3.u64 = object;
    ctx.r4.u64 = dispatcher;
    ctx.lr = 0x82106388;
    // Delegates (sub_8223A3F0, sub_82233460) keep their real target at +8.
    uint32_t target = (method == 0x8223A3F0 || method == 0x82233460)
                          ? guest::LoadU32(base, object + 8)
                          : method;
    if (g_render_only_pass && SkipOnRenderOnlyPass(target)) {
      continue;
    }
    if (only_targets && !InTargetList(*only_targets, target)) {
      continue;
    }
    if (debug::IsHidden(target)) {
      continue;
    }
    // The command list recorders write to, and its slots used.
    constexpr uint32_t kCommandListPointer = 0x8296B918;
    constexpr uint32_t kCommandListUsedOffset = 12;
    uint32_t list = guest::LoadU32(base, kCommandListPointer);
    uint32_t used_before = list ? guest::LoadU32(base, list + kCommandListUsedOffset) : 0;
    debug::FrameStateDiff_Before(base);
    debug::IdleTimerScan_Before(base);
    interpolation::BlendState_SetHandler(target);
    g_current_handler = target;
    g_current_message = guest::LoadU32(base, message);
    REX_CALL_INDIRECT_FUNC(method);
    g_current_handler = 0;
    g_current_message = ~0u;
    interpolation::BlendState_SetHandler(0);
    debug::IdleTimerScan_After(base, "message handler", target);
    debug::FrameStateDiff_After(base, "message handler", target);
    if (REXCVAR_GET(log_recording_handlers) && list &&
        guest::LoadU32(base, kCommandListPointer) == list) {
      uint32_t used_after = guest::LoadU32(base, list + kCommandListUsedOffset);
      if (used_after != used_before) {
        LogRecording(guest::LoadU32(base, message), target, int(used_after - used_before));
      }
    }
  }
}

void DispatchMessages(PPCContext& ctx, uint8_t* base, uint32_t dispatcher) {
  guest::StackFrame frame(ctx, 144);
  uint32_t message = frame.Local(80);
  Counts& counts = GetCounts();
  while (guest::LoadU32(base, dispatcher + kQueueCountOffset) != 0) {
    // Pop the oldest message into a local copy.
    uint32_t count = guest::LoadU32(base, dispatcher + kQueueCountOffset);
    uint32_t read = guest::LoadU32(base, dispatcher + kQueueReadIndexOffset);
    guest::StoreU32(base, dispatcher + kQueueCountOffset, count - 1);
    guest::StoreU32(base, dispatcher + kQueueReadIndexOffset, read + 1);
    uint32_t slot = dispatcher + kQueueOffset + (read % kQueueCapacity) * kMessageBytes;
    for (uint32_t k = 0; k < kMessageBytes; k += 4) {
      guest::StoreU32(base, message + k, guest::LoadU32(base, slot + k));
    }

    uint32_t id = guest::LoadU32(base, message);
    if (g_fast_pass && (id == kUpdate || id == kDraw)) {
      if (id == kDraw) {
        counts.draws++;
        interpolation::BlendState_PublishFastFrame();
      } else {
        counts.updates++;
        counts.updates_skipped++;
      }
      continue;
    }
    if (id == kUpdate) {
      counts.updates++;
      if (g_render_only_pass) {
        counts.updates_skipped++;
        // The race overlays' Update handler (sub_8215E150) records the
        // marker above opponent cars; on render-only passes it only records
        // (per_frame_logic.cpp).
        std::string only = REXCVAR_GET(render_only_update_handlers);
        if (REXCVAR_GET(redraw_race_overlays)) {
          only += only.empty() ? "8215E150" : ",8215E150";
        }
        if (!only.empty()) {
          Deliver(ctx, base, dispatcher, message, &only);
        }
        continue;
      }
    } else if (id == kDraw) {
      counts.draws++;
      interpolation::BlendState_BeforeDraw(base, g_render_only_pass);
      Deliver(ctx, base, dispatcher, message);
      interpolation::BlendState_AfterDraw(base);
      continue;
    } else {
      counts.others++;
    }
    Deliver(ctx, base, dispatcher, message);
  }
  LogCounts();
}

}  // namespace

void GameMessages_OnPassFinished() {
  if (!g_render_only_pass) {
    // The pass that just ran was a logic pass: it decides whether we're in a race.
    g_in_race = g_race_draw_step_seen;
    g_race_draw_step_seen = false;
  }
}

uint64_t g_passes[2] = {0, 0};  // logic, render-only
// For other threads: the pass number << 1 | render-only, and the main thread.
std::atomic<uint64_t> g_pass_snapshot{0};
std::atomic<std::thread::id> g_main_thread{};

uint64_t GameMessages_PassNumber() {
  return g_passes[0] + g_passes[1];
}

void GameMessages_PassCounts(uint64_t& logic, uint64_t& render_only) {
  logic = g_passes[0];
  render_only = g_passes[1];
}

void GameMessages_SetRenderOnlyPass(bool render_only) {
  g_passes[render_only ? 1 : 0]++;
  g_render_only_pass = render_only;
  g_render_only_since_logic = render_only ? g_render_only_since_logic + 1 : 0;
  g_fast_pass = render_only && g_render_only_since_logic >= 2 && native::Renderer_TickReplayReady();
  g_pass_snapshot.store((GameMessages_PassNumber() << 1) | render_only, std::memory_order_relaxed);
  g_main_thread.store(std::this_thread::get_id(), std::memory_order_relaxed);
}

void GameMessages_PassFromAnyThread(uint64_t& number, bool& render_only, bool& main_thread) {
  uint64_t snapshot = g_pass_snapshot.load(std::memory_order_relaxed);
  number = snapshot >> 1;
  render_only = snapshot & 1;
  main_thread = g_main_thread.load(std::memory_order_relaxed) == std::this_thread::get_id();
}

bool GameMessages_InRace() {
  return g_in_race;
}

bool SkipTickHandlerOnRenderOnlyPass(uint32_t target) {
  return SkipOnRenderOnlyPass(target);
}

bool GameMessages_IsRenderOnlyPass() {
  return g_render_only_pass;
}

int GameMessages_PassKind() {
  return g_fast_pass ? 2 : g_render_only_pass ? 1 : 0;
}

}  // namespace frame_pacing

// Guest entry points. These keep the generated names so the game's calls and
// the function table reach them; they only unpack registers and forward.

// Input manager update (a main-loop handler; reads the controllers through
// sub_825BA3F8, XInputGetState): r3 = input manager. Skipped on render-only
// passes, so input is read and acted on once per 30 Hz tick as the game
// expects. Otherwise presses read on a render-only pass were lost before
// game logic saw them, and menu auto-repeat (counted in reads) ran 5x fast.
extern "C" REX_FUNC(sub_820990F0) {
  REX_FUNC_PROLOGUE();
  if (frame_pacing::GameMessages_IsRenderOnlyPass()) {
    return;
  }
  __imp__sub_820990F0(ctx, base);
}

// The race's draw step: r3 = its argument. Noted to tell races from menus.
extern "C" REX_FUNC(sub_8215B4F8) {
  REX_FUNC_PROLOGUE();
  frame_pacing::g_race_draw_step_seen = true;
  __imp__sub_8215B4F8(ctx, base);
}

// Message dispatcher: r3 = dispatcher object.
extern "C" REX_FUNC(sub_821062E8) {
  REX_FUNC_PROLOGUE();
  frame_pacing::DispatchMessages(ctx, base, ctx.r3.u32);
}

// Diagnostics (--log_tick_handlers): the main loop's tick handlers that run
// on render-only passes, once a second. Each handler is an object whose
// vtable slot 1 is its tick; delegates keep their target at +8.
REXCVAR_DEFINE_BOOL(log_tick_handlers, false, "ITC",
                    "Log the main loop tick handlers run on render-only passes, once a second");

namespace {
std::map<std::pair<uint32_t, uint32_t>, int> g_tick_handlers;
std::chrono::steady_clock::time_point g_tick_handlers_logged{};
}  // namespace

// TickHandler_Invoke: r3 = app, r4 = (handler object in the low word).
extern "C" REX_FUNC(sub_82099A98) {
  REX_FUNC_PROLOGUE();
  if (frame_pacing::GameMessages_IsRenderOnlyPass() &&
      frame_pacing::SkipTickHandlerOnRenderOnlyPass(guest::LoadU32(base, ctx.r4.u32 + 8))) {
    return;
  }
  if (REXCVAR_GET(log_tick_handlers) && frame_pacing::GameMessages_IsRenderOnlyPass()) {
    uint32_t handler = ctx.r4.u32;
    uint32_t tick = guest::LoadU32(base, guest::LoadU32(base, handler) + 4);
    uint32_t target = guest::LoadU32(base, handler + 8);
    g_tick_handlers[{tick, target}]++;
    auto now = std::chrono::steady_clock::now();
    if (now - g_tick_handlers_logged >= std::chrono::seconds(1)) {
      for (const auto& [key, n] : g_tick_handlers) {
        REXLOG_INFO("tick handler on render-only passes: tick {:08X} target {:08X}: {} calls",
                    key.first, key.second, n);
      }
      g_tick_handlers.clear();
      g_tick_handlers_logged = now;
    }
  }
  uint32_t target = guest::LoadU32(base, ctx.r4.u32 + 8);
  debug::FrameStateDiff_Before(base);
  debug::IdleTimerScan_Before(base);
  __imp__sub_82099A98(ctx, base);
  debug::IdleTimerScan_After(base, "tick handler", target);
  debug::FrameStateDiff_After(base, "tick handler", target);
}
