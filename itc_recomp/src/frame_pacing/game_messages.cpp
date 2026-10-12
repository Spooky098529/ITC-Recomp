#include "game_messages.h"

#include "guest.h"
#include "interpolation/blend_state.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <chrono>
#include <map>

REXCVAR_DEFINE_BOOL(log_game_messages, false, "ITC",
                    "Log game messages delivered and skipped, once a second");

namespace frame_pacing {
namespace {

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

// Delivers one message to every handler, as the game does.
void Deliver(PPCContext& ctx, uint8_t* base, uint32_t dispatcher, uint32_t message) {
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
    REX_CALL_INDIRECT_FUNC(method);
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
    if (id == kUpdate) {
      counts.updates++;
      if (g_render_only_pass) {
        counts.updates_skipped++;
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

void GameMessages_SetRenderOnlyPass(bool render_only) {
  g_render_only_pass = render_only;
}

bool GameMessages_InRace() {
  return g_in_race;
}

bool GameMessages_IsRenderOnlyPass() {
  return g_render_only_pass;
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
  __imp__sub_82099A98(ctx, base);
}
