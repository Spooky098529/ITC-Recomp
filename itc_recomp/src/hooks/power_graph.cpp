// The power graph natively: the adapters in front of src/power_graph/
// (docs/functions/power-graph.md). Line::RecordFlat (sub_8217B508) has no
// adapter: only PowerGraph::Draw calls it, and its plain C++ is called from
// Draw's. With the command list's diagnostics on (they name who allocates
// slots by the return address), the original runs.

#include "hooks/power_graph.h"

#include "base/flags.h"
#include "hooks/car_makers.h"
#include "hooks/front_end_car_draw.h"
#include "hooks/guest_call.h"
#include "power_graph/power_graph.h"

ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);
ITC_FLAG_DECLARE(bool, native_car_makers);
ITC_FLAG_DECLARE(bool, native_career);

ITC_FLAG_BOOL(native_power_graph, true, "ITC",
              "Run the front end's power and torque graph (docs/functions/power-graph.md: "
              "TuningScreen::DrawPowerGraph sub_821DF350 and CarList::DrawEntryPowerGraph "
              "sub_82250670 and CarList::DrawCarPowerGraph sub_82250700 (PowerGraph::Draw through its "
              "adapter; the latter two with --native_car_makers and its callees' flags, the "
              "former with --native_career too), PowerGraph::Draw sub_8217C708 with Line::RecordFlat sub_8217B508, no adapter: "
              "only it calls it; the render manager's getters, the render queue's Add and the "
              "original Line::RecordFlat where its plain C++ declines are guest calls) natively "
              "instead of through the translated code (the same writes; the original with the "
              "command list's diagnostics on or a segment count above 32; checked by "
              "--run_harnesses=power_graph)");

namespace {

namespace pg = power_graph;

constexpr uint32_t kRenderManager = 0x826BABA0;

// A guest call made from the frame at `frame` (`bytes` long, its back chain
// written as the original's stwu does), returning to `back`.
template <typename Call>
void From(PPCContext& ctx, uint8_t* base, uint32_t frame, uint32_t bytes, uint32_t back,
          Call call) {
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = frame + bytes;
  guest_call::InFrame(ctx, base, bytes, back, [&](uint32_t) { call(); });
  ctx.r1.u64 = stack;
  ctx.fpscr.disableFlushMode();
}

// `object`'s vtable slot at `offset` with r3 = `object`, returning to
// `back`; its r3.
uint32_t VtableCall(PPCContext& ctx, uint8_t* base, uint32_t object, uint32_t offset,
                    uint32_t back) {
  ctx.r3.u64 = object;
  const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, object) + offset);
  ctx.ctr.u64 = function;
  ctx.lr = back;
  REX_CALL_INDIRECT_FUNC(function);
  return ctx.r3.u32;
}

// std of a doubleword.
void StoreDoubleword(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

}  // namespace

void hooks::PowerGraphDraw(PPCContext& ctx, uint8_t* base) {
  if (!pg::DrawReady(base, ctx.r6.u32)) {
    __imp__sub_8217C708(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t r3 = ctx.r3.u32;
  const uint32_t car = ctx.r6.u32;
  const double x = ctx.f1.f64;
  const double y = ctx.f2.f64;
  const uint64_t lr = ctx.lr;
  uint32_t draw_frame = 0;
  guest_call::InFrame(ctx, base, pg::kDrawFrame, pg::kTorqueLineReturn, [&](uint32_t frame) {
    draw_frame = frame;
    pg::LineCalls calls;
    calls.list_and_queue = [&](uint32_t line_frame) {
      std::pair<uint32_t, uint32_t> answer;
      From(ctx, base, line_frame, pg::kLineFrame, 0x8217B538, [&] {
        const uint32_t scene =
            VtableCall(ctx, base, guest::LoadU32(base, kRenderManager), 32, 0x8217B538);
        answer.first = VtableCall(ctx, base, scene, 4, 0x8217B548);
        const uint32_t again =
            VtableCall(ctx, base, guest::LoadU32(base, kRenderManager), 32, 0x8217B564);
        answer.second = VtableCall(ctx, base, again, 8, 0x8217B574);
      });
      return answer;
    };
    calls.add = [&](uint32_t line_frame, uint32_t queue, uint32_t chunk, uint32_t key) {
      From(ctx, base, line_frame, pg::kLineFrame, 0x8217B6B0, [&] {
        ctx.r4.u64 = chunk;
        ctx.r5.u64 = key;
        ctx.f1.f64 = double(guest::LoadFloat(base, 0x82000A34));
        VtableCall(ctx, base, queue, 16, 0x8217B6B0);
      });
    };
    calls.original = [&](uint32_t sprite, uint32_t back) {
      // From Draw's own frame (r1 is it here).
      ctx.r3.u64 = r3;
      ctx.r4.u64 = sprite;
      ctx.r5.u64 = pg::kPass;
      ctx.r6.u64 = pg::kKey;
      ctx.lr = back;
      sub_8217B508(ctx, base);
      ctx.fpscr.disableFlushMode();
    };
    pg::Draw(base, frame, x, y, car, calls);
  });
  ctx.lr = lr;
  ctx.r3.u64 = draw_frame + pg::kDrawR3;
}

void hooks::TuningScreenDrawPowerGraph(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  const uint64_t r30 = ctx.r30.u64;
  const uint64_t r31 = ctx.r31.u64;
  // The original's saves in its caller's frame (a segment count above 32
  // has PowerGraph::Draw's lines read them).
  guest::StoreU32(base, ctx.r1.u32 - 8, uint32_t(lr));
  StoreDoubleword(base, ctx.r1.u32 - 24, r30);
  StoreDoubleword(base, ctx.r1.u32 - 16, r31);
  ctx.fpscr.disableFlushMode();
  guest_call::InFrame(ctx, base, pg::kTuningFrame, pg::kTuningDrawReturn, [&](uint32_t frame) {
    uint32_t r3 = 0, movie = 0;
    if (!pg::TuningScreenShowsGraph(base, screen, frame, &r3, &movie)) {
      ctx.r3.u64 = r3;
      return;
    }
    const pg::TuningDraw draw = pg::TuningScreenDraw(base, screen);
    ctx.r3.u64 = draw.r3;
    ctx.r6.u64 = draw.car;
    ctx.f1.f64 = draw.x;
    ctx.f2.f64 = draw.y;
    // r30 and r31 as the original holds them (PowerGraph::Draw saves them
    // in its frame).
    ctx.r30.u64 = movie;
    ctx.r31.u64 = screen;
    ctx.lr = pg::kTuningDrawReturn;
    sub_8217C708(ctx, base);
  });
  ctx.r30.u64 = r30;
  ctx.r31.u64 = r31;
  ctx.lr = lr;
}

bool hooks::CarListDrawEntryCalleesNative() {
  return ITC_FLAG(native_car_makers) && hooks::CarMakersGarageCalleesNative() &&
         ITC_FLAG(native_career);
}

void hooks::CarListDrawEntryPowerGraph(PPCContext& ctx, uint8_t* base) {
  if (!CarListDrawEntryCalleesNative()) {
    __imp__sub_82250670(ctx, base);
    return;
  }
  const uint32_t entry = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  const uint64_t r31 = ctx.r31.u64;
  // The original's saves in its caller's frame.
  guest::StoreU32(base, ctx.r1.u32 - 8, uint32_t(lr));
  StoreDoubleword(base, ctx.r1.u32 - 16, r31);
  ctx.fpscr.disableFlushMode();
  guest_call::InFrame(ctx, base, pg::kEntryFrame, pg::kEntryDrawReturn, [&](uint32_t frame) {
    const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
    const bool draws = pg::CarListEntrySetUp(base, calls, entry, frame);
    ctx.fpscr.disableFlushMode();
    if (!draws) {
      return;
    }
    const pg::TuningDraw draw = pg::CarListEntryDraw(base, frame);
    ctx.r3.u64 = draw.r3;
    ctx.r6.u64 = draw.car;
    ctx.f1.f64 = draw.x;
    ctx.f2.f64 = draw.y;
    // r31 as the original holds it (lis: sign-extended; PowerGraph::Draw
    // saves it in its frame, where a count above 32 reads it back).
    ctx.r31.u64 = 0xFFFFFFFF00000000ull | pg::kEntryR31;
    ctx.lr = pg::kEntryDrawReturn;
    sub_8217C708(ctx, base);
  });
  ctx.r31.u64 = r31;
  ctx.lr = lr;
}

void hooks::CarListDrawCarPowerGraph(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_car_makers) || !hooks::CarMakersGarageCalleesNative()) {
    __imp__sub_82250700(ctx, base);
    return;
  }
  const uint32_t car = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  const uint64_t r31 = ctx.r31.u64;
  // The original's saves in its caller's frame.
  guest::StoreU32(base, ctx.r1.u32 - 8, uint32_t(lr));
  StoreDoubleword(base, ctx.r1.u32 - 16, r31);
  ctx.fpscr.disableFlushMode();
  guest_call::InFrame(ctx, base, pg::kCarFrame, pg::kCarDrawReturn, [&](uint32_t frame) {
    const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
    const bool draws = pg::CarListCarSetUp(base, calls, car, frame);
    ctx.fpscr.disableFlushMode();
    if (!draws) {
      return;
    }
    const pg::TuningDraw draw = pg::CarListEntryDraw(base, frame);
    ctx.r3.u64 = draw.r3;
    ctx.r6.u64 = draw.car;
    ctx.f1.f64 = draw.x;
    ctx.f2.f64 = draw.y;
    // r31 as the original holds it (lis: sign-extended; PowerGraph::Draw
    // saves it in its frame, where a count above 32 reads it back).
    ctx.r31.u64 = 0xFFFFFFFF00000000ull | pg::kEntryR31;
    ctx.lr = pg::kCarDrawReturn;
    sub_8217C708(ctx, base);
  });
  ctx.r31.u64 = r31;
  ctx.lr = lr;
}

extern "C" REX_FUNC(sub_8217C708) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_power_graph) || ITC_FLAG(log_overlay_uploads) ||
      ITC_FLAG(log_slot_callers) || ITC_FLAG(log_recording_by_pass)) {
    __imp__sub_8217C708(ctx, base);
    return;
  }
  hooks::PowerGraphDraw(ctx, base);
}

extern "C" REX_FUNC(sub_821DF350) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_power_graph)) {
    __imp__sub_821DF350(ctx, base);
    return;
  }
  hooks::TuningScreenDrawPowerGraph(ctx, base);
}

extern "C" REX_FUNC(sub_82250670) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_power_graph)) {
    __imp__sub_82250670(ctx, base);
    return;
  }
  hooks::CarListDrawEntryPowerGraph(ctx, base);
}

extern "C" REX_FUNC(sub_82250700) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_power_graph)) {
    __imp__sub_82250700(ctx, base);
    return;
  }
  hooks::CarListDrawCarPowerGraph(ctx, base);
}
