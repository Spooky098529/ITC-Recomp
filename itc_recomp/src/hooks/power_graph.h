// The power graph's register step (src/power_graph/,
// docs/functions/power-graph.md), shared by the adapter
// (src/hooks/power_graph.cpp, behind --native_power_graph) and the harness
// (without the flag).

#pragma once

#include "guest.h"

namespace hooks {

// PowerGraph::Draw (sub_8217C708): r3 = passed on to Line::RecordFlat (not
// read), f1 = the graph's left x, f2 = its baseline y, r6 = a car data. Its
// plain C++ in a frame made as the original's, with Line::RecordFlat's
// guest calls; r3 as the original leaves it. The original where the plain
// C++ declines (a segment count above 32).
void PowerGraphDraw(PPCContext& ctx, uint8_t* base);
// TuningScreen::DrawPowerGraph (sub_821DF350): r3 = the tuning screen. Its
// test's plain C++ in a frame made as the original's, then, when it draws,
// PowerGraph::Draw through its adapter (sub_8217C708, so the flag and the
// diagnostics still pick its version); r3 as the original leaves it.
void TuningScreenDrawPowerGraph(PPCContext& ctx, uint8_t* base);
// CarList::DrawEntryPowerGraph (sub_82250670): r3 = an entry of the car
// list screen's list. Its set-up's plain C++ in a frame made as the
// original's (its lr and r31 saved in its caller's frame), then, when it
// draws, PowerGraph::Draw through its adapter (sub_8217C708) with r31 as
// the original holds it; r3 as the original leaves it. The original unless
// its callees run natively (CarListDrawEntryCalleesNative).
bool CarListDrawEntryCalleesNative();
void CarListDrawEntryPowerGraph(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
