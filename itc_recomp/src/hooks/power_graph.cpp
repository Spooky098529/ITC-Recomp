// The power graph natively: the adapters in front of src/power_graph/
// (docs/functions/power-graph.md). Line::RecordFlat (sub_8217B508) has no
// adapter: only PowerGraph::Draw calls it, and its plain C++ is called from
// Draw's. With the command list's diagnostics on (they name who allocates
// slots by the return address), the original runs.

#include "hooks/power_graph.h"
#include "lifted/names.h"
#include "lifted/entries.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "hooks/car_makers.h"
#include "hooks/direct_slots.h"
#include "hooks/front_end_car_draw.h"
#include "hooks/guest_call.h"
#include "power_graph/power_graph.h"
#include "schema/app_start_up.h"
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

constexpr uint32_t kRenderManager = schema::AppStartUpGlobals::render_app_address();

// A guest call made from the frame at `frame` (`bytes` long, its back chain
// written as the original's stwu does), returning to `back`.
template <typename Call>
void From(lifted::Regs& ctx, uint32_t frame, uint32_t bytes, uint32_t back,
          Call call) {
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = frame + bytes;
  guest_call::InFrame(ctx, bytes, back, [&](uint32_t) { call(); });
  ctx.r1.u64 = stack;
  ctx.fpscr.disableFlushMode();
}

// `object`'s vtable slot at `offset` with r3 = `object`, returning to
// `back`; its r3.
uint32_t VtableCall(lifted::Regs& ctx, uint32_t object, uint32_t offset,
                    uint32_t back) {
  ctx.r3.u64 = object;
  const uint32_t function = guest::LoadU32(guest::LoadU32(object) + offset);
  ctx.ctr.u64 = function;
  ctx.lr = back;
  // (The render manager's slot 8 by its plain C++ where it's the game's,
  // hooks::DirectSlotCall.)
  if (!hooks::DirectSlotCall(ctx, function)) {
    lifted::CallFromContext(ctx, function);
  }
  return ctx.r3.u32;
}

// std of a doubleword.
void StoreDoubleword(uint32_t address, uint64_t value) {
  guest::StoreU32(address, uint32_t(value >> 32));
  guest::StoreU32(address + 4, uint32_t(value));
}

}  // namespace

void hooks::PowerGraphDraw(lifted::Regs& ctx) {
  if (!pg::DrawReady(ctx.r6.u32)) {
    lifted::original::c8217C708(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t r3 = ctx.r3.u32;
  const uint32_t car = ctx.r6.u32;
  const double x = ctx.f1.f64;
  const double y = ctx.f2.f64;
  const uint64_t lr = ctx.lr;
  uint32_t draw_frame = 0;
  guest_call::InFrame(ctx, pg::kDrawFrame, pg::kTorqueLineReturn, [&](uint32_t frame) {
    draw_frame = frame;
    pg::LineCalls calls;
    calls.list_and_queue = [&](uint32_t line_frame) {
      std::pair<uint32_t, uint32_t> answer;
      From(ctx, line_frame, pg::kLineFrame, 0x8217B538, [&] {
        const uint32_t scene =
            VtableCall(ctx, guest::LoadU32(kRenderManager), 32, 0x8217B538);
        answer.first = VtableCall(ctx, scene, 4, 0x8217B548);
        const uint32_t again =
            VtableCall(ctx, guest::LoadU32(kRenderManager), 32, 0x8217B564);
        answer.second = VtableCall(ctx, again, 8, 0x8217B574);
      });
      return answer;
    };
    calls.add = [&](uint32_t line_frame, uint32_t queue, uint32_t chunk, uint32_t key) {
      From(ctx, line_frame, pg::kLineFrame, 0x8217B6B0, [&] {
        ctx.r4.u64 = chunk;
        ctx.r5.u64 = key;
        ctx.f1.f64 = double(guest::LoadFloat(0x82000A34));
        VtableCall(ctx, queue, 16, 0x8217B6B0);
      });
    };
    calls.original = [&](uint32_t sprite, uint32_t back) {
      // From Draw's own frame (r1 is it here).
      ctx.r3.u64 = r3;
      ctx.r4.u64 = sprite;
      ctx.r5.u64 = pg::kPass;
      ctx.r6.u64 = pg::kKey;
      ctx.lr = back;
      lifted::CallFromContext(ctx, 0x8217B508);
      ctx.fpscr.disableFlushMode();
    };
    pg::Draw(frame, x, y, car, calls);
  });
  ctx.lr = lr;
  ctx.r3.u64 = draw_frame + pg::kDrawR3;
}

void hooks::TuningScreenDrawPowerGraph(lifted::Regs& ctx) {
  const uint32_t screen = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  // The original's saves in its caller's frame (a segment count above 32
  // has PowerGraph::Draw's lines read them).
  guest::StoreU32(ctx.r1.u32 - 8, uint32_t(lr));
  StoreDoubleword(ctx.r1.u32 - 24, 0);  // (the caller's r30: not carried by the view)
  StoreDoubleword(ctx.r1.u32 - 16, 0);  // (the caller's r31: not carried by the view)
  ctx.fpscr.disableFlushMode();
  guest_call::InFrame(ctx, pg::kTuningFrame, pg::kTuningDrawReturn, [&](uint32_t frame) {
    uint32_t r3 = 0, movie = 0;
    if (!pg::TuningScreenShowsGraph(screen, frame, &r3, &movie)) {
      ctx.r3.u64 = r3;
      return;
    }
    const pg::TuningDraw draw = pg::TuningScreenDraw(screen);
    ctx.r3.u64 = draw.r3;
    ctx.r6.u64 = draw.car;
    ctx.f1.f64 = draw.x;
    ctx.f2.f64 = draw.y;
    // (The original holds r30 = the movie and r31 = the screen here, which
    // PowerGraph::Draw saves in its frame: not carried by the view.)
    (void)movie;
    ctx.lr = pg::kTuningDrawReturn;
    lifted::Enter(ctx, game::PowerGraph::Draw);
  });
  ctx.lr = lr;
}

bool hooks::CarListDrawEntryCalleesNative() {
  return ITC_FLAG(native_car_makers) && hooks::CarMakersGarageCalleesNative() &&
         ITC_FLAG(native_career);
}

void hooks::CarListDrawEntryPowerGraph(lifted::Regs& ctx) {
  if (!CarListDrawEntryCalleesNative()) {
    lifted::original::c82250670(ctx);
    return;
  }
  const uint32_t entry = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  // The original's saves in its caller's frame.
  guest::StoreU32(ctx.r1.u32 - 8, uint32_t(lr));
  StoreDoubleword(ctx.r1.u32 - 16, 0);  // (the caller's r31: not carried by the view)
  ctx.fpscr.disableFlushMode();
  guest_call::InFrame(ctx, pg::kEntryFrame, pg::kEntryDrawReturn, [&](uint32_t frame) {
    const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
    const bool draws = pg::CarListEntrySetUp(calls, entry, frame);
    ctx.fpscr.disableFlushMode();
    if (!draws) {
      return;
    }
    const pg::TuningDraw draw = pg::CarListEntryDraw(frame);
    ctx.r3.u64 = draw.r3;
    ctx.r6.u64 = draw.car;
    ctx.f1.f64 = draw.x;
    ctx.f2.f64 = draw.y;
    // (The original holds r31 = kEntryR31 here, sign-extended, which
    // PowerGraph::Draw saves in its frame, where a count above 32 reads it
    // back: not carried by the view.)
    ctx.lr = pg::kEntryDrawReturn;
    lifted::Enter(ctx, game::PowerGraph::Draw);
  });
  ctx.lr = lr;
}

void hooks::CarListDrawCarPowerGraph(lifted::Regs& ctx) {
  if (!ITC_FLAG(native_car_makers) || !hooks::CarMakersGarageCalleesNative()) {
    lifted::original::c82250700(ctx);
    return;
  }
  const uint32_t car = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  // The original's saves in its caller's frame.
  guest::StoreU32(ctx.r1.u32 - 8, uint32_t(lr));
  StoreDoubleword(ctx.r1.u32 - 16, 0);  // (the caller's r31: not carried by the view)
  ctx.fpscr.disableFlushMode();
  guest_call::InFrame(ctx, pg::kCarFrame, pg::kCarDrawReturn, [&](uint32_t frame) {
    const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
    const bool draws = pg::CarListCarSetUp(calls, car, frame);
    ctx.fpscr.disableFlushMode();
    if (!draws) {
      return;
    }
    const pg::TuningDraw draw = pg::CarListEntryDraw(frame);
    ctx.r3.u64 = draw.r3;
    ctx.r6.u64 = draw.car;
    ctx.f1.f64 = draw.x;
    ctx.f2.f64 = draw.y;
    // (The original holds r31 = kEntryR31 here, sign-extended, which
    // PowerGraph::Draw saves in its frame, where a count above 32 reads it
    // back: not carried by the view.)
    ctx.lr = pg::kCarDrawReturn;
    lifted::Enter(ctx, game::PowerGraph::Draw);
  });
  ctx.lr = lr;
}

ITC_NATIVE_ENTRY_ALWAYS(8217C708) {
  
  if (!ITC_FLAG(native_power_graph) || ITC_FLAG(log_overlay_uploads) ||
      ITC_FLAG(log_slot_callers) || ITC_FLAG(log_recording_by_pass)) {
    lifted::original::c8217C708(ctx);
    return;
  }
  hooks::PowerGraphDraw(ctx);
}

ITC_NATIVE_ENTRY(821DF350, ITC_FLAG(native_power_graph)) {
  hooks::TuningScreenDrawPowerGraph(ctx);
}

ITC_NATIVE_ENTRY(82250670, ITC_FLAG(native_power_graph)) {
  hooks::CarListDrawEntryPowerGraph(ctx);
}

ITC_NATIVE_ENTRY(82250700, ITC_FLAG(native_power_graph)) {
  hooks::CarListDrawCarPowerGraph(ctx);
}

// For the fallback originals (docs/missing-functions.md).
// Line::RecordFlat: r3 = Draw's (not read), r4 = the sprite, r5 = the pass,
// r6 = the key; its guest calls from its 128-byte frame as PowerGraph::Draw's
// adapter makes them; r3 as RenderQueue::Add leaves it. The original (r3 to
// r6 and lr put back) where the plain C++ hands back.
ITC_DIRECT_ENTRY(8217B508) {
  ctx.fpscr.disableFlushMode();
  const uint64_t r3 = ctx.r3.u64, r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64;
  const uint64_t lr = ctx.lr;
  pg::LineCalls calls;
  calls.list_and_queue = [&](uint32_t line_frame) {
    std::pair<uint32_t, uint32_t> answer;
    From(ctx, line_frame, pg::kLineFrame, 0x8217B538, [&] {
      const uint32_t scene = VtableCall(ctx, guest::LoadU32(kRenderManager), 32, 0x8217B538);
      answer.first = VtableCall(ctx, scene, 4, 0x8217B548);
      const uint32_t again = VtableCall(ctx, guest::LoadU32(kRenderManager), 32, 0x8217B564);
      answer.second = VtableCall(ctx, again, 8, 0x8217B574);
    });
    return answer;
  };
  calls.add = [&](uint32_t line_frame, uint32_t queue, uint32_t chunk, uint32_t key) {
    From(ctx, line_frame, pg::kLineFrame, 0x8217B6B0, [&] {
      ctx.r4.u64 = chunk;
      ctx.r5.u64 = key;
      ctx.f1.f64 = double(guest::LoadFloat(0x82000A34));
      VtableCall(ctx, queue, 16, 0x8217B6B0);
    });
  };
  // (Not called: only PowerGraph::Draw's own lines fall back through it.)
  calls.original = [](uint32_t, uint32_t) {};
  if (pg::LineRecordFlat(ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32 - pg::kLineFrame,
                         calls)) {
    ctx.lr = lr;
    return;
  }
  ctx.r3.u64 = r3;
  ctx.r4.u64 = r4;
  ctx.r5.u64 = r5;
  ctx.r6.u64 = r6;
  ctx.lr = lr;
  lifted::original::c8217B508(ctx);
}
