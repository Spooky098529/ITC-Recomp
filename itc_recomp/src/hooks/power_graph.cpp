// The power graph natively: the adapters in front of src/power_graph/
// (docs/functions/power-graph.md). Line::RecordFlat (sub_8217B508) has no
// adapter: only PowerGraph::Draw calls it, and its plain C++ is called from
// Draw's. With the command list's diagnostics on (they name who allocates
// slots by the return address), the original runs.

#include "hooks/power_graph.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "hooks/car_makers.h"
#include "hooks/direct_slots.h"
#include "hooks/front_end_car_draw.h"
#include "hooks/guest_call.h"
#include "power_graph/power_graph.h"
#include "lifted/sdk.h"

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
void From(lifted::Regs& ctx, uint8_t* base, uint32_t frame, uint32_t bytes, uint32_t back,
          Call call) {
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = frame + bytes;
  guest_call::InFrame(ctx, base, bytes, back, [&](uint32_t) { call(); });
  ctx.r1.u64 = stack;
  ctx.fpscr.disableFlushMode();
}

// `object`'s vtable slot at `offset` with r3 = `object`, returning to
// `back`; its r3.
uint32_t VtableCall(lifted::Regs& ctx, uint8_t* base, uint32_t object, uint32_t offset,
                    uint32_t back) {
  ctx.r3.u64 = object;
  const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, object) + offset);
  ctx.ctr.u64 = function;
  ctx.lr = back;
  // (The render manager's slot 8 by its plain C++ where it's the game's,
  // hooks::DirectSlotCall.)
  if (!hooks::DirectSlotCall(ctx, base, function)) {
    lifted::CallFromContext(ctx, base, function);
  }
  return ctx.r3.u32;
}

// std of a doubleword.
void StoreDoubleword(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

}  // namespace

void hooks::PowerGraphDraw(lifted::Regs& ctx, uint8_t* base) {
  if (!pg::DrawReady(base, ctx.r6.u32)) {
    lifted::Sdk(ctx, base, __imp__sub_8217C708);
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
      lifted::CallFromContext(ctx, base, 0x8217B508);
      ctx.fpscr.disableFlushMode();
    };
    pg::Draw(base, frame, x, y, car, calls);
  });
  ctx.lr = lr;
  ctx.r3.u64 = draw_frame + pg::kDrawR3;
}

void hooks::TuningScreenDrawPowerGraph(lifted::Regs& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  // The original's saves in its caller's frame (a segment count above 32
  // has PowerGraph::Draw's lines read them).
  guest::StoreU32(base, ctx.r1.u32 - 8, uint32_t(lr));
  StoreDoubleword(base, ctx.r1.u32 - 24, 0);  // (the caller's r30: not carried by the view)
  StoreDoubleword(base, ctx.r1.u32 - 16, 0);  // (the caller's r31: not carried by the view)
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
    // (The original holds r30 = the movie and r31 = the screen here, which
    // PowerGraph::Draw saves in its frame: not carried by the view.)
    (void)movie;
    ctx.lr = pg::kTuningDrawReturn;
    lifted::CallFromContext(ctx, base, 0x8217C708);
  });
  ctx.lr = lr;
}

bool hooks::CarListDrawEntryCalleesNative() {
  return ITC_FLAG(native_car_makers) && hooks::CarMakersGarageCalleesNative() &&
         ITC_FLAG(native_career);
}

void hooks::CarListDrawEntryPowerGraph(lifted::Regs& ctx, uint8_t* base) {
  if (!CarListDrawEntryCalleesNative()) {
    lifted::Sdk(ctx, base, __imp__sub_82250670);
    return;
  }
  const uint32_t entry = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  // The original's saves in its caller's frame.
  guest::StoreU32(base, ctx.r1.u32 - 8, uint32_t(lr));
  StoreDoubleword(base, ctx.r1.u32 - 16, 0);  // (the caller's r31: not carried by the view)
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
    // (The original holds r31 = kEntryR31 here, sign-extended, which
    // PowerGraph::Draw saves in its frame, where a count above 32 reads it
    // back: not carried by the view.)
    ctx.lr = pg::kEntryDrawReturn;
    lifted::CallFromContext(ctx, base, 0x8217C708);
  });
  ctx.lr = lr;
}

void hooks::CarListDrawCarPowerGraph(lifted::Regs& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_car_makers) || !hooks::CarMakersGarageCalleesNative()) {
    lifted::Sdk(ctx, base, __imp__sub_82250700);
    return;
  }
  const uint32_t car = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  // The original's saves in its caller's frame.
  guest::StoreU32(base, ctx.r1.u32 - 8, uint32_t(lr));
  StoreDoubleword(base, ctx.r1.u32 - 16, 0);  // (the caller's r31: not carried by the view)
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
    // (The original holds r31 = kEntryR31 here, sign-extended, which
    // PowerGraph::Draw saves in its frame, where a count above 32 reads it
    // back: not carried by the view.)
    ctx.lr = pg::kCarDrawReturn;
    lifted::CallFromContext(ctx, base, 0x8217C708);
  });
  ctx.lr = lr;
}

ITC_NATIVE_ENTRY_ALWAYS(8217C708) {
  
  if (!ITC_FLAG(native_power_graph) || ITC_FLAG(log_overlay_uploads) ||
      ITC_FLAG(log_slot_callers) || ITC_FLAG(log_recording_by_pass)) {
    lifted::Sdk(ctx, base, __imp__sub_8217C708);
    return;
  }
  hooks::PowerGraphDraw(ctx, base);
}

ITC_NATIVE_ENTRY(821DF350, ITC_FLAG(native_power_graph)) {
  hooks::TuningScreenDrawPowerGraph(ctx, base);
}

ITC_NATIVE_ENTRY(82250670, ITC_FLAG(native_power_graph)) {
  hooks::CarListDrawEntryPowerGraph(ctx, base);
}

ITC_NATIVE_ENTRY(82250700, ITC_FLAG(native_power_graph)) {
  hooks::CarListDrawCarPowerGraph(ctx, base);
}
