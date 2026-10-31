// The race HUD object's register steps (src/race_hud_object/,
// docs/functions/race-hud-object.md), shared by the adapters
// (src/hooks/race_hud_object.cpp, behind --native_race_hud_object) and the
// harness (without the flag). Each leaves r3 (and f1) as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// HudCurve::Eval sub_82212118: r3 = the curve, f1 = x (r3 left).
void RaceHudObjectCurveEval(PPCContext& ctx, uint8_t* base);
// PowInt sub_82213C40: f1 = x, r4 = n; f1 = x to the n.
void RaceHudObjectPowInt(PPCContext& ctx, uint8_t* base);
// HudBar::Step sub_82211FB8: r3 = the bar, r4 = the target (r3 left).
void RaceHudObjectBarStep(PPCContext& ctx, uint8_t* base);
// RaceHud::SetGear sub_82212DB0: r3 = the HUD, r4 = the gear; r3 = the node.
void RaceHudObjectSetGear(PPCContext& ctx, uint8_t* base);
// RaceHud::SetBar sub_82214698: r3 = the HUD, f1 = the value; r3 = the node.
void RaceHudObjectSetBar(PPCContext& ctx, uint8_t* base);
// sub_82212668 and sub_822122A0: r3 = the HUD (left).
void RaceHudObjectResetBar(PPCContext& ctx, uint8_t* base);
void RaceHudObjectClearWarning(PPCContext& ctx, uint8_t* base);
// RaceHud::SetPlayer sub_82212690: r3 = the HUD, r4 = the player; r3 = the
// vehicle.
void RaceHudObjectSetPlayer(PPCContext& ctx, uint8_t* base);
// RaceHud::Touching sub_82215820: r4 = the player; r3 = 1 or 0.
void RaceHudObjectTouching(PPCContext& ctx, uint8_t* base);

// RaceHud::SetTach sub_82212D58 and SetBoost sub_82212E18: r3 = the HUD,
// f1 = the value; r3 = the node.
void RaceHudObjectSetTach(PPCContext& ctx, uint8_t* base);
void RaceHudObjectSetBoost(PPCContext& ctx, uint8_t* base);
// RaceHud::SetSpeed sub_82214260: r3 = the HUD, r4 = the speed.
void RaceHudObjectSetSpeed(PPCContext& ctx, uint8_t* base);
// RaceHud::SetTime sub_82213750: r3 = the HUD, r4 = the time's address.
void RaceHudObjectSetTime(PPCContext& ctx, uint8_t* base);
// RaceHud::SetDistance sub_82214A50: r3 = the HUD, f1 = the value.
void RaceHudObjectSetDistance(PPCContext& ctx, uint8_t* base);
// RaceHud::Reset sub_822121C0, SetUpExtraGauges sub_82213DB0, ApplyOptions
// sub_82212768, UpdateView sub_82212B68: r3 = the HUD.
void RaceHudObjectReset(PPCContext& ctx, uint8_t* base);
void RaceHudObjectSetUpExtraGauges(PPCContext& ctx, uint8_t* base);
void RaceHudObjectApplyOptions(PPCContext& ctx, uint8_t* base);
void RaceHudObjectUpdateView(PPCContext& ctx, uint8_t* base);
// RaceHud::ShowGauge sub_82212D08: r3 = the HUD, r4 = the slot, r5 = shown.
void RaceHudObjectShowGauge(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
