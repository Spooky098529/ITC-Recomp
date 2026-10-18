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

#include "frame_pacing/game_messages.h"
#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <chrono>

REXCVAR_DEFINE_BOOL(log_pause_menu, false, "ITC",
                    "Log menu list updates (sub_8220C1E0) and the action mapper's updates "
                    "(sub_8210C370) while a direction fires, by pass type");

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

}  // namespace

// The menu list update.
extern "C" REX_FUNC(sub_8220C1E0) {
  REX_FUNC_PROLOGUE();
  if (!REXCVAR_GET(log_pause_menu)) {
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
    REXLOG_INFO(
        "pause menu: tick {} pass {} {} list {:08X}{} from {:08X}: selection {} -> {} "
        "(scroll {} -> {}), mapper {:08X} held {:08X} fired {:08X}",
        Ticks(), frame_pacing::GameMessages_PassNumber(), PassType(), menu,
        pause_menu ? " (pause)" : "", caller, int32_t(before), int32_t(after), int32_t(scroll),
        int32_t(guest::LoadU32(base, menu + 4)), mapper, held, fired);
  }
}

// The action mapper's input update.
extern "C" REX_FUNC(sub_8210C370) {
  REX_FUNC_PROLOGUE();
  if (!REXCVAR_GET(log_pause_menu)) {
    __imp__sub_8210C370(ctx, base);
    return;
  }
  uint32_t mapper = ctx.r3.u32;
  uint32_t caller = uint32_t(ctx.lr);
  uint32_t fired_before = guest::LoadU32(base, mapper + 464);
  __imp__sub_8210C370(ctx, base);
  uint32_t held = guest::LoadU32(base, mapper + 448);
  uint32_t fired = guest::LoadU32(base, mapper + 464);
  if (((held | fired | fired_before) & kDirectionBits) && TakeLine()) {
    REXLOG_INFO(
        "pause menu: tick {} pass {} {} mapper {:08X} update from {:08X}: held {:08X} "
        "pressed {:08X} fired {:08X} -> {:08X}",
        Ticks(), frame_pacing::GameMessages_PassNumber(), PassType(), mapper, caller, held,
        guest::LoadU32(base, mapper + 456), fired_before, fired);
  }
}
