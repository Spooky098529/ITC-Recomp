// The render command replay's single commands natively: adapters in front of
// src/graphics/single_commands.h (docs/functions/single-commands.md). The
// handlers are reached through the table 0x8265ED48, so these adapters
// stay; ChunkChain::RunSingles' plain C++ (src/graphics/render_queue.cpp)
// calls the plain C++ directly (SingleCommands below). Each leaves r3 as the
// original does.

#include "hooks/single_commands.h"
#include "hooks/native_entry.h"

#include "guest.h"

#include "base/flags.h"
#include "native/state.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_single_commands, true, "ITC",
              "Run the render command replay's single commands "
              "(docs/functions/single-commands.md: Single::InlineVertices sub_8258E8E8, "
              "Single::SetUpDrawer sub_8258E958, Single::SetShaderConstants sub_8258F0F0, "
              "Single::DrawerDraw sub_8258F5C0 inside its command 15 wrap, Single::Callback "
              "sub_8258E9B8) "
              "natively instead of through the translated code (the same writes; guest calls "
              "for the drawer's and the callback object's vtable slots and the shader constant "
              "setters not seen in play; "
              "checked by --run_harnesses=single_commands)");

namespace sc = graphics::single_commands;

sc::GuestCall hooks::SingleCommandsCall(lifted::Regs& ctx) {
  return [&ctx](uint32_t function, uint32_t sp, uint32_t return_address,
                      std::initializer_list<uint64_t> args) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    lifted::Temp* registers[] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6, &ctx.r7, &ctx.r8};
    size_t i = 0;
    for (uint64_t value : args) {
      registers[i++]->u64 = value;
    }
    ctx.lr = return_address;
    lifted::CallFromContext(ctx, function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  };
}

void hooks::NativeSingleInlineVertices(lifted::Regs& ctx) {
  ctx.r3.u64 = sc::InlineVertices(ctx.r3.u32);
}

void hooks::NativeSingleSetUpDrawer(lifted::Regs& ctx) {
  ctx.r3.u64 = sc::SetUpDrawer(ctx.r3.u32, ctx.r1.u32, SingleCommandsCall(ctx));
}

void hooks::NativeSingleSetShaderConstants(lifted::Regs& ctx) {
  ctx.r3.u64 =
      sc::SetShaderConstants(ctx.r3.u32, ctx.r1.u32, SingleCommandsCall(ctx));
}

namespace {

// Command 15: marked as command 15 around it, so the wraps of BeginVertices
// and EndVertices hand PrimitiveDrawer::Draw's inline draw to the renderer
// (as its wrap in src/native/replay_hooks.cpp does around the original).
uint32_t DrawerDrawMarked(lifted::Regs& ctx, uint32_t sp) {
  const uint32_t outer_command = native::g_current_command;
  native::g_current_command = 15;
  const uint32_t r3 = sc::DrawerDraw(sp, hooks::SingleCommandsCall(ctx));
  native::g_current_command = outer_command;
  return r3;
}

}  // namespace

void hooks::NativeSingleDrawerDraw(lifted::Regs& ctx) {
  ctx.r3.u64 = DrawerDrawMarked(ctx, ctx.r1.u32);
}

void hooks::NativeSingleCallback(lifted::Regs& ctx) {
  ctx.r3.u64 = sc::Callback(ctx.r3.u32, ctx.r1.u32, SingleCommandsCall(ctx));
}

graphics::render_queue::SingleCommand hooks::SingleCommands(lifted::Regs& ctx) {
  return [&ctx](uint32_t handler, uint32_t slot, uint32_t sp) -> std::optional<uint32_t> {
    if (!ITC_FLAG(native_single_commands)) {
      return std::nullopt;
    }
    switch (handler) {
      case sc::kInlineVertices:
        return sc::InlineVertices(slot);
      case sc::kSetUpDrawer:
        return sc::SetUpDrawer(slot, sp, SingleCommandsCall(ctx));
      case sc::kSetShaderConstants:
        return sc::SetShaderConstants(slot, sp, SingleCommandsCall(ctx));
      case sc::kDrawerDraw:
        return DrawerDrawMarked(ctx, sp);
      case sc::kCallback:
        return sc::Callback(slot, sp, SingleCommandsCall(ctx));
      default:
        return std::nullopt;
    }
  };
}

// Single::InlineVertices (5): r3 = the slot.
ITC_NATIVE_ENTRY(8258E8E8, ITC_FLAG(native_single_commands)) {
  hooks::NativeSingleInlineVertices(ctx);
}

// Single::SetUpDrawer (13 and 14): r3 = the slot.
ITC_NATIVE_ENTRY(8258E958, ITC_FLAG(native_single_commands)) {
  hooks::NativeSingleSetUpDrawer(ctx);
}

// Single::SetShaderConstants (7): r3 = the slot.
ITC_NATIVE_ENTRY(8258F0F0, ITC_FLAG(native_single_commands)) {
  hooks::NativeSingleSetShaderConstants(ctx);
}

// Single::Callback (28): r3 = the slot.
ITC_NATIVE_ENTRY(8258E9B8, ITC_FLAG(native_single_commands)) {
  hooks::NativeSingleCallback(ctx);
}
