// The race HUD object's register steps (src/race_hud_object/,
// docs/functions/race-hud-object.md), shared by the adapters
// (src/hooks/race_hud_object.cpp, behind --native_race_hud_object) and the
// harness (without the flag); those of functions only native callers reach
// have no adapter, and only the harness calls them. Each leaves r3 (and f1) as the original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// HudCurve::Eval sub_82212118: r3 = the curve, f1 = x (r3 left).
void RaceHudObjectCurveEval(lifted::Regs& ctx);
// PowInt sub_82213C40: f1 = x, r4 = n; f1 = x to the n.
void RaceHudObjectPowInt(lifted::Regs& ctx);
// HudBar::Step sub_82211FB8: r3 = the bar, r4 = the target (r3 left).
void RaceHudObjectBarStep(lifted::Regs& ctx);
// RaceHud::SetGear sub_82212DB0: r3 = the HUD, r4 = the gear; r3 = the node.
void RaceHudObjectSetGear(lifted::Regs& ctx);
// RaceHud::SetBar sub_82214698: r3 = the HUD, f1 = the value; r3 = the node.
void RaceHudObjectSetBar(lifted::Regs& ctx);
// sub_82212668 and sub_822122A0: r3 = the HUD (left).
void RaceHudObjectResetBar(lifted::Regs& ctx);
void RaceHudObjectClearWarning(lifted::Regs& ctx);
// RaceHud::SetPlayer sub_82212690: r3 = the HUD, r4 = the player; r3 = the
// vehicle.
void RaceHudObjectSetPlayer(lifted::Regs& ctx);
// RaceHud::Touching sub_82215820: r4 = the player; r3 = 1 or 0.
void RaceHudObjectTouching(lifted::Regs& ctx);

// RaceHud::SetTach sub_82212D58 and SetBoost sub_82212E18: r3 = the HUD,
// f1 = the value; r3 = the node.
void RaceHudObjectSetTach(lifted::Regs& ctx);
void RaceHudObjectSetBoost(lifted::Regs& ctx);
// RaceHud::SetSpeed sub_82214260: r3 = the HUD, r4 = the speed.
void RaceHudObjectSetSpeed(lifted::Regs& ctx);
// RaceHud::SetTime sub_82213750: r3 = the HUD, r4 = the time's address.
void RaceHudObjectSetTime(lifted::Regs& ctx);
// RaceHud::SetDistance sub_82214A50: r3 = the HUD, f1 = the value.
void RaceHudObjectSetDistance(lifted::Regs& ctx);
// RaceHud::Reset sub_822121C0, SetUpExtraGauges sub_82213DB0, ApplyOptions
// sub_82212768, UpdateView sub_82212B68: r3 = the HUD.
void RaceHudObjectReset(lifted::Regs& ctx);
void RaceHudObjectSetUpExtraGauges(lifted::Regs& ctx);
void RaceHudObjectApplyOptions(lifted::Regs& ctx);
void RaceHudObjectUpdateView(lifted::Regs& ctx);
// RaceHud::ShowGauge sub_82212D08: r3 = the HUD, r4 = the slot, r5 = shown.
void RaceHudObjectShowGauge(lifted::Regs& ctx);

// RaceHud::SetUpGauges sub_82213C90, SetUpMirror sub_822135B0, TearDown
// sub_822141A0, SetUpSigns sub_822122B0: r3 = the HUD. UpdateExtraGauges
// sub_82215160: r3 = the HUD, f1, r5 and r6 the gauges' values. Their guest
// calls through this context.
void RaceHudObjectSetUpGauges(lifted::Regs& ctx);
void RaceHudObjectSetUpMirror(lifted::Regs& ctx);
void RaceHudObjectTearDown(lifted::Regs& ctx);
void RaceHudObjectSetUpSigns(lifted::Regs& ctx);
void RaceHudObjectUpdateExtraGauges(lifted::Regs& ctx);

// RaceHud::UpdateSigns sub_82214718 and UpdateSign sub_82212F00: r3 = the
// HUD, r4 = the race flow's flag; UpdatePlaceName sub_82213510: r3 = the HUD.
void RaceHudObjectUpdateSigns(lifted::Regs& ctx);
void RaceHudObjectUpdateSign(lifted::Regs& ctx);
void RaceHudObjectUpdatePlaceName(lifted::Regs& ctx);

// RaceHud::Tick sub_822153A0: r3 = the HUD, r4 = the race flow's flag;
// TickTimer sub_82214D78: r3 = the HUD.
void RaceHudObjectTick(lifted::Regs& ctx);
void RaceHudObjectTickTimer(lifted::Regs& ctx);

// RaceHud::StartCountdown sub_82212270, StartMovie1 sub_82212718,
// FinishMovie1 sub_82212878, DrawWarning sub_82212AC8, RunMovie1
// sub_82212AF8, RunItem sub_82212B08, Init sub_82215260: r3 = the HUD.
// RaceScreens::MovieMask sub_82211BE8: r4 = the race kind. DrawWarning has
// no adapter (its callers in play, the race HUD machine's PauseDraw and
// RacePauseDraw, call its plain C++): the step for the harness.
void RaceHudObjectStartCountdown(lifted::Regs& ctx);
void RaceHudObjectStartMovie1(lifted::Regs& ctx);
void RaceHudObjectFinishMovie1(lifted::Regs& ctx);
void RaceHudObjectDrawWarning(lifted::Regs& ctx);
void RaceHudObjectRunMovie1(lifted::Regs& ctx);
void RaceHudObjectRunItem(lifted::Regs& ctx);
void RaceHudObjectMovieMask(lifted::Regs& ctx);
void RaceHudObjectInit(lifted::Regs& ctx);
// RaceHud::Draw sub_822128A8, UpdateWarning sub_82212C50: r3 = the HUD (Draw
// has no adapter: its callers in play, the race HUD machine's draws, call its
// plain C++; the step for the harness);
// UpdateRecordingFull sub_82212E60: r3 = the HUD, r4 = HasRoom's.
void RaceHudObjectDraw(lifted::Regs& ctx);
void RaceHudObjectUpdateWarning(lifted::Regs& ctx);
void RaceHudObjectUpdateRecordingFull(lifted::Regs& ctx);
// RaceHud::ShowItem sub_82213638: r3 = the HUD, r4 = the item or -1;
// UpdateItem sub_82214238: r3 = the HUD.
void RaceHudObjectShowItem(lifted::Regs& ctx);
void RaceHudObjectUpdateItem(lifted::Regs& ctx);
// RaceHud::RunCountdown sub_822129A8: r3 = the HUD (f1 the time, as the
// original leaves it); UpdateRacerBar sub_82214790: r3 = the HUD, r4 = the
// bar, r5 = the racer.
void RaceHudObjectRunCountdown(lifted::Regs& ctx);
void RaceHudObjectUpdateRacerBar(lifted::Regs& ctx);
// RaceHud::RunWarning sub_82212A48: r3 = the HUD.
void RaceHudObjectRunWarning(lifted::Regs& ctx);
// RaceHud::TickRacers sub_822151F8 and TickPanel sub_82215488: r3 = the HUD.
void RaceHudObjectTickRacers(lifted::Regs& ctx);
void RaceHudObjectTickPanel(lifted::Regs& ctx);
// RaceHud::SetMode sub_822157A8: r3 = the HUD, r4 = the mode (a tail call
// from the caller's frame and lr); SetUpTimer sub_822123C8 and SetUpRacers
// sub_82213F20: r3 = the HUD.
void RaceHudObjectSetMode(lifted::Regs& ctx);
void RaceHudObjectSetUpTimer(lifted::Regs& ctx);
void RaceHudObjectSetUpRacers(lifted::Regs& ctx);
// RaceScreens::Make1004 sub_82216A60: r3 = the holder's +1004;
// RaceScreens::Init sub_82211DA0: r3 = the holder, r4 = the race kind.
void RaceHudObjectMake1004(lifted::Regs& ctx);
void RaceHudObjectScreensInit(lifted::Regs& ctx);
// RaceScreens::Create sub_82211F40: r3 = the race kind; the original where
// the native heap isn't active.
void RaceHudObjectScreensCreate(lifted::Regs& ctx);
// RaceScreens::~RaceScreens sub_82211B98: r3 = the holder; the original
// where the native heap isn't active (AltHud::Destroy's plain C++ frees
// over the allocation layer).
void RaceHudObjectScreensDestruct(lifted::Regs& ctx);
// RaceScreens::Destroy sub_82211D18: no arguments; the original where the
// native heap isn't active.
void RaceHudObjectScreensDestroy(lifted::Regs& ctx);
// sub_82211D08 RaceScreens::DrawPauseMenu and sub_82211D10
// DrawAfterRaceMenu (r3 = the holder; tail calls). Neither has an adapter
// (their callers in play, the race HUD machine's PauseDraw, RacePauseDraw and
// AfterRaceDraw, call their plain C++): the steps for the harness.
void RaceHudObjectDrawPauseMenu(lifted::Regs& ctx);
void RaceHudObjectDrawAfterRaceMenu(lifted::Regs& ctx);

}  // namespace hooks
