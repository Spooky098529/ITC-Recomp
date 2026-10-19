#include "game_messages.h"

#include "debug/frame_state_diff.h"
#include "debug/idle_timer_scan.h"
#include "debug/menu_selection_scan.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "native/textures.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <bit>
#include <atomic>
#include <chrono>
#include <cstring>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <thread>

REXCVAR_DEFINE_BOOL(log_race_changes, false, "ITC",
                    "Log when the game enters and leaves a race (the race's draw step starts or "
                    "stops running)");
REXCVAR_DEFINE_BOOL(log_game_messages, false, "ITC",
                    "Log game messages delivered and skipped, once a second");
REXCVAR_DEFINE_STRING(render_only_update_handlers, "", "ITC",
                      "Testing: message handler targets (hex, comma-separated) that still get "
                      "Update on render-only passes");
REXCVAR_DEFINE_BOOL(redraw_race_overlays, true, "ITC",
                    "Record the race overlays (the marker above opponent cars) on render-only "
                    "passes too");
REXCVAR_DEFINE_BOOL(redraw_rival_logo, true, "ITC",
                    "Draw the rival's logo in the rival intro cutscene on render-only passes "
                    "too (the cutscene still steps once a tick)");
REXCVAR_DEFINE_BOOL(log_recording_handlers, false, "ITC",
                    "Log which message handlers record render commands, by message, once a "
                    "second");
// Testing: main loop tick handlers or message handlers to skip on
// render-only passes, by target (comma-separated hex), to find which one
// counts frames.
REXCVAR_DEFINE_STRING(render_only_skip, "", "ITC",
                      "Testing: tick handler or message handler targets (hex, "
                      "comma-separated) to skip on render-only passes");
// docs/pause-menu-fix.md, step 0.
REXCVAR_DEFINE_BOOL(log_input, false, "ITC",
                    "Log the input manager's objects once a race, the pause signs on change, "
                    "and the handlers run by pass type and pause sign, once a second");
REXCVAR_DEFINE_BOOL(log_input_writes, false, "ITC",
                    "Log which tick and message handlers write to the input manager and its "
                    "listener objects, by pass type and pause sign, once a second");
REXCVAR_DEFINE_BOOL(log_input_reads, false, "ITC",
                    "Log calls to the input processors' button tests by caller, pass type and "
                    "pause, once a second");

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
bool g_race_draw_step_seen = false;  // during the current logic pass
bool g_in_race = false;              // as of the last logic pass
// --log_input_reads: the pause screen's Draw handler (8216CF08) delivered on
// this pass, and on the pass before (taken as paused).
constexpr uint32_t kPauseScreenDraw = 0x8216CF08;
bool g_pause_drawn_this_pass = false;
bool g_paused = false;
uint32_t g_current_tick = 0;  // the tick handler running (its target), or 0
uint32_t g_input_listener0 = 0;

// Diagnostics (--log_input): the pause signs as of the race draw step's last
// entry, and the message and tick handlers run, by pass type and those signs.
struct PauseSigns {
  uint32_t records[3] = {};  // LoadU32(0x826BAC00 + 588 + 592k)
  uint32_t mode = 0;         // LoadU32([0x826C5B2C] + 0xA3AF0)
  uint32_t r31 = 0;
  uint32_t r31_value = 0;  // LoadU32(r31 + 588)
  bool operator==(const PauseSigns&) const = default;
};
PauseSigns g_pause_signs;

// Bits: 1 render-only, 2 in a race, 4 r31 + 588 non-zero, 8 record 0's +588
// non-zero.
int InputLogCombo() {
  return (g_render_only_pass ? 1 : 0) | (g_in_race ? 2 : 0) | (g_pause_signs.r31_value ? 4 : 0) |
         (g_pause_signs.records[0] ? 8 : 0);
}

constexpr int kMessageRun = 0;
constexpr int kTickRun = 1;
// (combo, kind, message id or tick function, target) -> calls.
std::map<std::tuple<int, int, uint32_t, uint32_t>, int> g_input_log_runs;
uint64_t g_input_log_passes[16] = {};
std::chrono::steady_clock::time_point g_input_log_last{};

void InputLog_Run(int kind, uint32_t id, uint32_t target) {
  if (REXCVAR_GET(log_input)) {
    g_input_log_runs[{InputLogCombo(), kind, id, target}]++;
  }
}

// --log_input_writes: guest ranges watched (the input manager and its
// listener objects; set by the sub_820990F0 override), and the words each
// handler changed in them, logged once a second.
struct InputRange {
  uint32_t start;
  uint32_t words;
  const char* name;
  int index;
};
std::vector<InputRange> g_input_ranges;
std::vector<uint32_t> g_input_before, g_input_after;
struct InputWrite {
  int count = 0;
  uint32_t before = 0, after = 0;  // the last change
};
// (combo, kind, target, range, offset) -> writes.
std::map<std::tuple<int, int, uint32_t, int, uint32_t>, InputWrite> g_input_writes;
std::chrono::steady_clock::time_point g_input_writes_last{};

void InputWatch_SetRanges(std::vector<InputRange> ranges) {
  g_input_ranges = std::move(ranges);
}

void InputWatch_Copy(uint8_t* base, std::vector<uint32_t>& out) {
  size_t words = 0;
  for (const InputRange& r : g_input_ranges) {
    words += r.words;
  }
  out.resize(words);
  size_t at = 0;
  for (const InputRange& r : g_input_ranges) {
    guest::CopyU32s(base, r.start, out.data() + at, r.words);
    at += r.words;
  }
}

void InputWatch_Before(uint8_t* base) {
  if (REXCVAR_GET(log_input_writes) && !g_input_ranges.empty()) {
    InputWatch_Copy(base, g_input_before);
  }
}

void InputWatch_After(uint8_t* base, int kind, uint32_t target) {
  if (!REXCVAR_GET(log_input_writes) || g_input_ranges.empty() || g_input_before.empty()) {
    return;
  }
  InputWatch_Copy(base, g_input_after);
  if (g_input_after.size() != g_input_before.size()) {
    return;
  }
  size_t at = 0;
  int combo = InputLogCombo();
  for (size_t r = 0; r < g_input_ranges.size(); r++) {
    for (uint32_t w = 0; w < g_input_ranges[r].words; w++, at++) {
      if (g_input_before[at] != g_input_after[at]) {
        InputWrite& write = g_input_writes[{combo, kind, target, int(r), 4 * w}];
        write.count++;
        write.before = g_input_before[at];
        write.after = g_input_after[at];
      }
    }
  }
}

void InputWatch_Flush() {
  constexpr size_t kMaxLines = 120;
  size_t lines = 0;
  for (const auto& [key, write] : g_input_writes) {
    auto [combo, kind, target, range, offset] = key;
    if (lines++ == kMaxLines) {
      REXLOG_INFO("input writes: {} more", g_input_writes.size() - kMaxLines);
      break;
    }
    const InputRange& r = g_input_ranges[std::min<size_t>(range, g_input_ranges.size() - 1)];
    REXLOG_INFO(
        "input writes: {} in_race {} r31+588 {} record0 {} ({} passes): {} {:08X}: {} {} +{:X} "
        "x{} (last {:08X} -> {:08X})",
        (combo & 1) ? "render-only" : "logic", (combo >> 1) & 1, (combo >> 2) & 1,
        (combo >> 3) & 1, g_input_log_passes[combo], kind == kMessageRun ? "message" : "tick",
        target, r.name, r.index, offset, write.count, write.before, write.after);
  }
  g_input_writes.clear();
}

// At a pass's start: counts the pass, and logs what ran once a second.
void InputLog_OnPassStart() {
  if (!REXCVAR_GET(log_input)) {
    return;
  }
  auto now = std::chrono::steady_clock::now();
  if (now - g_input_log_last >= std::chrono::seconds(1)) {
    g_input_log_last = now;
    std::string line;
    auto flush = [&] {
      if (!line.empty()) {
        REXLOG_INFO("{}", line);
        line.clear();
      }
    };
    int last_combo = -1, last_kind = -1;
    uint32_t last_id = 0;
    for (const auto& [key, n] : g_input_log_runs) {
      auto [combo, kind, id, target] = key;
      if (combo != last_combo || kind != last_kind || id != last_id) {
        flush();
        line = fmt::format("input runs: {} in_race {} r31+588 {} record0 {} ({} passes): {} {:X}:",
                           (combo & 1) ? "render-only" : "logic", (combo >> 1) & 1,
                           (combo >> 2) & 1, (combo >> 3) & 1, g_input_log_passes[combo],
                           kind == kMessageRun ? "message" : "tick", id);
        last_combo = combo;
        last_kind = kind;
        last_id = id;
      }
      line += fmt::format(" {:08X}x{}", target, n);
    }
    flush();
    g_input_log_runs.clear();
    InputWatch_Flush();
    std::fill(std::begin(g_input_log_passes), std::end(g_input_log_passes), 0);
  }
  g_input_log_passes[InputLogCombo()]++;
}

// As the race draw step is entered: logs the pause signs when any changed.
void InputLog_PauseSigns(uint8_t* base, uint32_t r31, uint32_t r31_value);

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
    debug::MenuSelectionScan_Before(base);
    interpolation::BlendState_SetHandler(target);
    g_current_handler = target;
    g_current_message = guest::LoadU32(base, message);
    InputLog_Run(kMessageRun, g_current_message, target);
    if (target == kPauseScreenDraw) {
      g_pause_drawn_this_pass = true;
    }
    InputWatch_Before(base);
    REX_CALL_INDIRECT_FUNC(method);
    InputWatch_After(base, kMessageRun, target);
    g_current_handler = 0;
    g_current_message = ~0u;
    interpolation::BlendState_SetHandler(0);
    debug::MenuSelectionScan_After(base, "message handler", target);
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
        // The rival intro cutscene's handler (82159948, sub_821594B8): on
        // render-only passes it only draws the rival's logo
        // (per_frame_logic.cpp).
        if (REXCVAR_GET(redraw_rival_logo)) {
          only += only.empty() ? "82159948" : ",82159948";
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
    if (g_in_race != g_race_draw_step_seen) {
      if (REXCVAR_GET(log_race_changes)) {
        REXLOG_INFO("game messages: {} a race", g_race_draw_step_seen ? "entered" : "left");
      }
      if (!g_race_draw_step_seen) {
        native::TextureCache_RaceLeft();
      }
    }
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
  g_paused = g_pause_drawn_this_pass;
  g_pause_drawn_this_pass = false;
  InputLog_OnPassStart();
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

bool GameMessages_Paused() {
  return g_paused;
}

uint32_t GameMessages_InputListener0() {
  return g_input_listener0;
}

bool SkipTickHandlerOnRenderOnlyPass(uint32_t target) {
  return SkipOnRenderOnlyPass(target);
}

bool GameMessages_IsRenderOnlyPass() {
  return g_render_only_pass;
}

namespace {

void InputLog_PauseSigns(uint8_t* base, uint32_t r31, uint32_t r31_value) {
  if (!REXCVAR_GET(log_input)) {
    return;
  }
  // sub_8215B4F8 reads +588 of record 0 of 0x826BAC00 (21 records of 592
  // bytes), or of records 1 and 2 when the mode word is 4.
  constexpr uint32_t kRecords = 0x826BAC00;
  constexpr uint32_t kRecordBytes = 592;
  PauseSigns signs;
  for (uint32_t k = 0; k < 3; k++) {
    signs.records[k] = guest::LoadU32(base, kRecords + k * kRecordBytes + 588);
  }
  uint32_t mode_object = guest::LoadU32(base, 0x826C5B2C);
  signs.mode = mode_object ? guest::LoadU32(base, mode_object + 0xA3AF0) : ~0u;
  signs.r31 = r31;
  signs.r31_value = r31_value;
  if (signs == g_pause_signs) {
    return;
  }
  g_pause_signs = signs;
  REXLOG_INFO(
      "input pause signs: pass {} {}: record0+588 {:08X} record1 {:08X} record2 {:08X} mode {:X} "
      "r31 {:08X} r31+588 {:08X}",
      GameMessages_PassNumber(), g_render_only_pass ? "render-only" : "logic", signs.records[0],
      signs.records[1], signs.records[2], signs.mode, signs.r31, signs.r31_value);
}

}  // namespace

}  // namespace frame_pacing

// Guest entry points. These keep the generated names so the game's calls and
// the function table reach them; they only unpack registers and forward.

// Input manager update (a main-loop handler; reads the controllers through
// sub_825BA3F8, XInputGetState): r3 = input manager. Skipped on render-only
// passes, so input is read and acted on once per 30 Hz tick as the game
// expects. Otherwise presses read on a render-only pass were lost before
// game logic saw them, and menu auto-repeat (counted in reads) ran 5x fast.
namespace {

// --log_input: the input manager's objects (layout in
// docs/pause-menu-fix.md, "Input::Update"), once a race.
void LogInputManager(PPCContext& ctx, uint8_t* base, uint32_t manager) {
  constexpr uint32_t kLogicalPads = 8, kLogicalPadBytes = 196;
  constexpr uint32_t kPhysicalPads = 988, kPhysicalPadBytes = 212;
  constexpr uint32_t kProcessors = 1840, kProcessorBytes = 416;
  constexpr uint32_t kListeners = 3924, kListenerBytes = 12;
  auto slot1 = [&](uint32_t object) {
    return guest::LoadU32(base, guest::LoadU32(base, object) + 4);
  };
  REXLOG_INFO("input manager {:08X}", manager);
  for (uint32_t i = 0; i < 4; i++) {
    uint32_t pad = manager + kPhysicalPads + kPhysicalPadBytes * i;
    REXLOG_INFO("input physical pad {} at {:08X}: vtable {:08X} slot 1 {:08X} flags {:X}", i, pad,
                guest::LoadU32(base, pad), slot1(pad), guest::LoadU32(base, pad + 4));
  }
  for (uint32_t j = 0; j < 5; j++) {
    uint32_t processor = manager + kProcessors + kProcessorBytes * j;
    REXLOG_INFO("input processor {} at {:08X}: vtable {:08X} slot 1 {:08X}", j, processor,
                guest::LoadU32(base, processor), slot1(processor));
  }
  for (uint32_t j = 0; j < 5; j++) {
    uint32_t s = manager + kListeners + kListenerBytes * j;
    uint32_t count = guest::LoadU32(base, s + 4);
    uint32_t head = guest::LoadU32(base, s);
    if (!count || !head) {
      REXLOG_INFO("input listener slot {}: count {} head {:08X}", j, count, head);
      continue;
    }
    uint32_t node = guest::LoadU32(base, head + 4);
    if (node == head) {
      REXLOG_INFO("input listener slot {}: count {} head {:08X}, empty list", j, count, head);
      continue;
    }
    uint32_t object = guest::LoadU32(base, node + 8);
    REXLOG_INFO("input listener slot {}: count {} object {:08X} vtable {:08X} slot 1 {:08X}", j,
                count, object, guest::LoadU32(base, object), slot1(object));
  }
  // Each binding's slot 1 is called as sub_82591918 calls it (r3 = device,
  // returns the values' address in r3); the caller's r3 and lr are kept.
  uint64_t saved_r3 = ctx.r3.u64;
  uint64_t saved_lr = ctx.lr;
  for (uint32_t j = 0; j < 5; j++) {
    uint32_t pad = manager + kLogicalPads + kLogicalPadBytes * j;
    for (uint32_t k = 0; k < 4; k++) {
      uint32_t enabled = guest::LoadU8(base, pad + 4 + 8 * k);
      uint32_t device = guest::LoadU32(base, pad + 8 + 8 * k);
      if (!device) {
        REXLOG_INFO("input logical pad {} binding {}: enabled {} device 0", j, k, enabled);
        continue;
      }
      int physical = -1;
      if (device >= manager + kPhysicalPads &&
          (device - manager - kPhysicalPads) % kPhysicalPadBytes == 0 &&
          (device - manager - kPhysicalPads) / kPhysicalPadBytes < 4) {
        physical = int((device - manager - kPhysicalPads) / kPhysicalPadBytes);
      }
      uint32_t function = slot1(device);
      std::string values = "not called (disabled)";
      if (enabled) {
        ctx.r3.u64 = device;
        ctx.lr = 0x82591970;
        REX_CALL_INDIRECT_FUNC(function);
        uint32_t v = ctx.r3.u32;
        values = fmt::format("returns {:08X} (device + 8: {})", v, v == device + 8);
      }
      REXLOG_INFO(
          "input logical pad {} binding {}: enabled {} device {:08X} (physical pad {}) vtable "
          "{:08X} slot 1 {:08X} {}",
          j, k, enabled, device, physical, guest::LoadU32(base, device), function, values);
    }
  }
  ctx.r3.u64 = saved_r3;
  ctx.lr = saved_lr;
  for (uint32_t address :
       {0x82000A30u, 0x82000A34u, 0x82001BE0u, 0x82002200u, 0x8200223Cu, 0x82002BD4u,
        0x8206CBFCu}) {
    uint32_t bits = guest::LoadU32(base, address);
    REXLOG_INFO("input constant {:08X}: {:08X} = {}", address, bits, std::bit_cast<float>(bits));
  }
}

bool g_input_manager_logged = false;  // this race

}  // namespace

extern "C" REX_FUNC(sub_820990F0) {
  REX_FUNC_PROLOGUE();
  if (frame_pacing::GameMessages_IsRenderOnlyPass()) {
    return;
  }
  if (!frame_pacing::GameMessages_InRace()) {
    g_input_manager_logged = false;
  } else if (REXCVAR_GET(log_input) && !g_input_manager_logged) {
    g_input_manager_logged = true;
    LogInputManager(ctx, base, ctx.r3.u32);
  }
  {
    // Listener 0's object (first node of listener slot 0), for diagnostics.
    uint32_t s = ctx.r3.u32 + 3924;
    uint32_t head = guest::LoadU32(base, s);
    uint32_t node = head && guest::LoadU32(base, s + 4) ? guest::LoadU32(base, head + 4) : 0;
    frame_pacing::g_input_listener0 = node && node != head ? guest::LoadU32(base, node + 8) : 0;
  }
  if (REXCVAR_GET(log_input_writes)) {
    // The manager (its last part, the listener slots, ends at +3984) and
    // each listener's first object, 416 bytes (the processors' size: they
    // share the processors' vtable).
    uint32_t manager = ctx.r3.u32;
    std::vector<frame_pacing::InputRange> ranges{{manager, 3984 / 4, "manager", 0}};
    for (uint32_t j = 0; j < 5; j++) {
      uint32_t s = manager + 3924 + 12 * j;
      uint32_t head = guest::LoadU32(base, s);
      if (!guest::LoadU32(base, s + 4) || !head) {
        continue;
      }
      uint32_t node = guest::LoadU32(base, head + 4);
      if (node != head) {
        ranges.push_back({guest::LoadU32(base, node + 8), 416 / 4, "listener", int(j)});
      }
    }
    frame_pacing::InputWatch_SetRanges(std::move(ranges));
  }
  __imp__sub_820990F0(ctx, base);
}

// The race's draw step: r3 = its argument. Noted to tell races from menus.
// r31 isn't an argument: it's what the caller (sub_8215C770) left in it.
extern "C" REX_FUNC(sub_8215B4F8) {
  REX_FUNC_PROLOGUE();
  uint32_t r31_value = guest::LoadU32(base, ctx.r31.u32 + 588);
  frame_pacing::InputLog_PauseSigns(base, ctx.r31.u32, r31_value);
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
  frame_pacing::InputLog_Run(frame_pacing::kTickRun,
                             guest::LoadU32(base, guest::LoadU32(base, ctx.r4.u32) + 4), target);
  debug::FrameStateDiff_Before(base);
  debug::IdleTimerScan_Before(base);
  debug::MenuSelectionScan_Before(base);
  frame_pacing::InputWatch_Before(base);
  frame_pacing::g_current_tick = target;
  __imp__sub_82099A98(ctx, base);
  frame_pacing::g_current_tick = 0;
  frame_pacing::InputWatch_After(base, frame_pacing::kTickRun, target);
  debug::MenuSelectionScan_After(base, "tick handler", target);
  debug::IdleTimerScan_After(base, "tick handler", target);
  debug::FrameStateDiff_After(base, "tick handler", target);
}

// --log_input_reads: the input processors' (and listeners') button tests,
// each r3 = processor, r4 = pointer to a button mask, returning a bool:
// 825924E0 all held (+272), 82592500 any set in +264, 82592520 pressed or
// repeating (a held button's counter at +284 + 4 * bit is 1 or has reached
// +276). Their only direct callers are the wrappers 8258CF10, 8258CEB0 and
// 8258CEE0 (same arguments). They read the state Input::Update left, which
// changes once a tick, so a caller that acts on them on every pass acts
// several times a tick.
namespace {

struct InputReadKey {
  bool render_only, paused;
  uint32_t message, handler, tick, function, caller, object, mask, result;
  auto operator<=>(const InputReadKey&) const = default;
};
std::map<InputReadKey, int> g_input_reads;
std::chrono::steady_clock::time_point g_input_reads_logged{};

void InputRead_Note(uint32_t function, uint32_t caller, uint32_t object, uint32_t mask,
                    uint32_t result) {
  using namespace frame_pacing;
  g_input_reads[{g_render_only_pass, g_paused, g_current_message, g_current_handler,
                 g_current_tick, function, caller, object, mask, result}]++;
  auto now = std::chrono::steady_clock::now();
  if (now - g_input_reads_logged < std::chrono::seconds(1)) {
    return;
  }
  g_input_reads_logged = now;
  constexpr size_t kMaxLines = 400;
  size_t lines = 0;
  for (const auto& [k, n] : g_input_reads) {
    if (lines++ == kMaxLines) {
      REXLOG_INFO("input reads: {} more", g_input_reads.size() - kMaxLines);
      break;
    }
    REXLOG_INFO(
        "input reads: {} paused {} message {:X} handler {:08X} tick {:08X}: {:08X} from {:08X} "
        "object {:08X} mask {:08X} -> {} x{}",
        k.render_only ? "render-only" : "logic", int(k.paused), k.message, k.handler, k.tick,
        k.function, k.caller, k.object, k.mask, k.result, n);
  }
  g_input_reads.clear();
}

}  // namespace

#define ITC_INPUT_READ(address)                                                    \
  extern "C" REX_FUNC(sub_##address) {                                             \
    REX_FUNC_PROLOGUE();                                                           \
    if (!REXCVAR_GET(log_input_reads)) {                                           \
      __imp__sub_##address(ctx, base);                                             \
      return;                                                                      \
    }                                                                              \
    uint32_t caller = uint32_t(ctx.lr);                                            \
    uint32_t object = ctx.r3.u32;                                                  \
    uint32_t mask = guest::LoadU32(base, ctx.r4.u32);                              \
    __imp__sub_##address(ctx, base);                                               \
    InputRead_Note(0x##address, caller, object, mask, ctx.r3.u32 & 0xFF);          \
  }
ITC_INPUT_READ(825924E0)
ITC_INPUT_READ(82592500)
ITC_INPUT_READ(82592520)
ITC_INPUT_READ(8258CF10)
ITC_INPUT_READ(8258CEB0)
ITC_INPUT_READ(8258CEE0)
#undef ITC_INPUT_READ

// The pads' values (vtable slot 1, r3 = pad, returns a pointer to its 40
// floats, the stick's among them): 82591B30 a logical pad's (+36), 820988B8
// a physical pad's (+8). Input::Update calls them itself; any other caller
// reads the stick. Logged like the button tests, with mask and result 0.
#define ITC_INPUT_VALUES(address)                                                  \
  extern "C" REX_FUNC(sub_##address) {                                             \
    REX_FUNC_PROLOGUE();                                                           \
    uint32_t caller = uint32_t(ctx.lr);                                            \
    uint32_t object = ctx.r3.u32;                                                  \
    __imp__sub_##address(ctx, base);                                               \
    if (REXCVAR_GET(log_input_reads)) {                                            \
      InputRead_Note(0x##address, caller, object, 0, 0);                           \
    }                                                                              \
  }
ITC_INPUT_VALUES(82591B30)
ITC_INPUT_VALUES(820988B8)
#undef ITC_INPUT_VALUES
