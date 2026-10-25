// Logs the front end's menu lists and the input they act on
// (--log_pause_menu), to find which pass moves the pause menu's selection.
//
// A menu list (the pause menu's is at [0x826D84DC]) is updated by
// sub_8220C1E0: the selection is at +16 (previous +20, scroll +4), and it
// moves when sub_8220C020 says action 153/167 (up) or 154/168 (down) fired.
// That reads bit (action - 151) of the action mapper's "fired" word at
// mapper + 464; the mapper (0x40071140 for pad 0) is
// [0x826BAB80 + 8 * [0x826BABA4 + 4 * [menu + 60]]]. The mapper's input
// update sub_8210C370 rebuilds its words at +448 (held, held before,
// pressed, released, fired).
//
// It also counts the mappers' updates (sub_8210B018) and the calls to the
// mapper's read slots, once a second by caller, slot, action, pass type and
// whether the game is paused, to find any reader on render-only passes
// before the mappers are updated once a tick (docs/pause-menu-fix.md,
// "Step 1").

#include "debug/pause_menu_log.h"
#include "frame_pacing/game_messages.h"
#include "guest.h"

#include "base/flags.h"
#include "base/log.h"

#include <chrono>
#include <map>
#include <mutex>
#include <tuple>

ITC_FLAG_BOOL(log_pause_menu, false, "ITC",
              "Log menu list updates (sub_8220C1E0) and the action mapper's updates "
              "(sub_8210C370) while a direction fires, by pass type; count the mappers' "
              "updates and reads by caller once a second");

namespace {

constexpr uint32_t kDirectionBits = (1u << 2) | (1u << 3) | (1u << 16) | (1u << 17);
constexpr int kMaxLines = 600;  // per 2 s

bool TakeLine() {
  static int lines = 0;
  static auto last = std::chrono::steady_clock::now();
  auto now = std::chrono::steady_clock::now();
  if (now - last >= std::chrono::seconds(2)) {
    last = now;
    lines = 0;
  }
  return lines++ < kMaxLines;
}

const char* PassType() {
  return frame_pacing::GameMessages_IsRenderOnlyPass() ? "render-only" : "logic";
}

uint64_t Ticks() {
  uint64_t logic = 0, render_only = 0;
  frame_pacing::GameMessages_PassCounts(logic, render_only);
  return logic;
}

uint32_t MapperOf(uint8_t* base, uint32_t menu) {
  uint32_t index = guest::LoadU32(base, 0x826BABA4 + 4 * guest::LoadU32(base, menu + 60));
  if (index > 4) {
    return 0;
  }
  return guest::LoadU32(base, 0x826BAB80 + 8 * index);
}

// Read counts, keyed by (slot, action, caller, render-only, paused); slot 0
// is the mappers' update sub_8210B018 (action 0), counted when it runs.
struct ReadCount {
  uint64_t calls = 0;
  uint64_t nonzero = 0;
};
using ReadKey = std::tuple<int, uint32_t, uint32_t, bool, bool>;
std::mutex reads_mutex;
std::map<ReadKey, ReadCount> reads;
auto reads_since = std::chrono::steady_clock::now();

const char* SlotFunction(int slot) {
  switch (slot) {
    case 0: return "8210B018 update";
    case 5: return "8210BA68 analog";
    case 8: return "8210BE70 held";
    case 9: return "8210BED8 held before";
    case 10: return "8210BF40 pressed";
    case 11: return "8210BFA8 released";
    case 12: return "8210C010 fired";
    case 14: return "8210BA28 analog u16";
  }
  return "?";
}

// Logs the counts once a second; reads_mutex held.
void FlushReads() {
  auto now = std::chrono::steady_clock::now();
  if (now - reads_since < std::chrono::seconds(1)) {
    return;
  }
  reads_since = now;
  for (const auto& [key, count] : reads) {
    const auto& [slot, action, caller, render_only, paused] = key;
    ITC_LOG_INFO(
        "pause menu reads: tick {} {} {} slot {} ({}) action {} from {:08X}: calls {} "
        "non-zero {}",
        Ticks(), paused ? "paused" : "running", render_only ? "render-only" : "logic", slot,
        SlotFunction(slot), action, caller, count.calls, count.nonzero);
  }
  reads.clear();
}

// Separate budget from TakeLine's, so a held stick's analog reads can't crowd
// out the menu lines.
bool TakeReadLine() {
  static int lines = 0;
  static auto last = std::chrono::steady_clock::now();
  auto now = std::chrono::steady_clock::now();
  if (now - last >= std::chrono::seconds(2)) {
    last = now;
    lines = 0;
  }
  return lines++ < 200;
}

void NoteRead(int slot, uint32_t mapper, uint32_t action, uint32_t caller, bool nonzero,
              double value) {
  bool render_only = frame_pacing::GameMessages_IsRenderOnlyPass();
  {
    std::lock_guard lock(reads_mutex);
    ReadCount& count =
        reads[ReadKey{slot, action, caller, render_only, frame_pacing::GameMessages_Paused()}];
    count.calls++;
    count.nonzero += nonzero;
    FlushReads();
  }
  if (render_only && nonzero && TakeReadLine()) {
    ITC_LOG_INFO(
        "pause menu read on render-only pass: tick {} pass {} slot {} ({}) mapper {:08X} "
        "action {} from {:08X} = {}",
        Ticks(), frame_pacing::GameMessages_PassNumber(), slot, SlotFunction(slot), mapper,
        action, caller, value);
  }
}

}  // namespace

// The mapper read slots (vtable 0x82007F78; r3 the mapper, r4 the action id
// 151-180). Bits return 0 or 1 in r3, analog values in f1 or r3.
#define PAUSE_MENU_READ_SLOT(address, slot, result)                        \
  extern "C" REX_FUNC(sub_##address) {                                     \
    REX_FUNC_PROLOGUE();                                                   \
    if (!ITC_FLAG(log_pause_menu)) {                                    \
      __imp__sub_##address(ctx, base);                                     \
      return;                                                              \
    }                                                                      \
    uint32_t mapper = ctx.r3.u32;                                          \
    uint32_t action = ctx.r4.u32;                                          \
    uint32_t caller = uint32_t(ctx.lr);                                    \
    __imp__sub_##address(ctx, base);                                       \
    double value = (result);                                               \
    NoteRead(slot, mapper, action, caller, value != 0, value);             \
  }

PAUSE_MENU_READ_SLOT(8210BA68, 5, ctx.f1.f64)
PAUSE_MENU_READ_SLOT(8210BED8, 9, double(ctx.r3.u32))
PAUSE_MENU_READ_SLOT(8210BFA8, 11, double(ctx.r3.u32))

#undef PAUSE_MENU_READ_SLOT

// Slots 8, 10, 12 and 14 are native (hooks/action_mappers.cpp); their
// adapters log through this.
void debug::PauseMenuLog_NoteSlotRead(int slot, uint32_t mapper, uint32_t action,
                                      uint32_t caller, double value) {
  if (ITC_FLAG(log_pause_menu)) {
    NoteRead(slot, mapper, action, caller, value != 0, value);
  }
}

void debug::PauseMenuLog_NoteMapperUpdate(uint32_t caller) {
  if (ITC_FLAG(log_pause_menu)) {
    NoteRead(0, 0, 0, caller, false, 0);
  }
}

// The menu list update.
extern "C" REX_FUNC(sub_8220C1E0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(log_pause_menu)) {
    __imp__sub_8220C1E0(ctx, base);
    return;
  }
  uint32_t menu = ctx.r3.u32;
  uint32_t caller = uint32_t(ctx.lr);
  uint32_t mapper = MapperOf(base, menu);
  uint32_t fired = mapper ? guest::LoadU32(base, mapper + 464) : 0;
  uint32_t held = mapper ? guest::LoadU32(base, mapper + 448) : 0;
  uint32_t before = guest::LoadU32(base, menu + 16);
  uint32_t scroll = guest::LoadU32(base, menu + 4);
  __imp__sub_8220C1E0(ctx, base);
  uint32_t after = guest::LoadU32(base, menu + 16);
  bool pause_menu = menu == guest::LoadU32(base, 0x826D84DC);
  if ((after != before || (fired & kDirectionBits) || (held & kDirectionBits)) && TakeLine()) {
    ITC_LOG_INFO(
        "pause menu: tick {} pass {} {} list {:08X}{} from {:08X}: selection {} -> {} "
        "(scroll {} -> {}), mapper {:08X} held {:08X} fired {:08X}",
        Ticks(), frame_pacing::GameMessages_PassNumber(), PassType(), menu,
        pause_menu ? " (pause)" : "", caller, int32_t(before), int32_t(after), int32_t(scroll),
        int32_t(guest::LoadU32(base, menu + 4)), mapper, held, fired);
  }
}

// The action mapper's input update (sub_8210C370; its adapter is in
// hooks/action_mappers.cpp).
uint32_t debug::PauseMenuLog_BeforeMapperUpdate(uint8_t* base, uint32_t mapper) {
  if (!ITC_FLAG(log_pause_menu)) {
    return 0;
  }
  return guest::LoadU32(base, mapper + 464);
}

void debug::PauseMenuLog_AfterMapperUpdate(uint8_t* base, uint32_t mapper, uint32_t caller,
                                           uint32_t fired_before) {
  if (!ITC_FLAG(log_pause_menu)) {
    return;
  }
  uint32_t held = guest::LoadU32(base, mapper + 448);
  uint32_t fired = guest::LoadU32(base, mapper + 464);
  if (((held | fired | fired_before) & kDirectionBits) && TakeLine()) {
    ITC_LOG_INFO(
        "pause menu: tick {} pass {} {} mapper {:08X} update from {:08X}: held {:08X} "
        "pressed {:08X} fired {:08X} -> {:08X}",
        Ticks(), frame_pacing::GameMessages_PassNumber(), PassType(), mapper, caller, held,
        guest::LoadU32(base, mapper + 456), fired_before, fired);
  }
}
