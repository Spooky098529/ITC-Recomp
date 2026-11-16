// The alternate HUD's register steps (src/alt_hud/,
// docs/functions/alt-hud.md), shared by the adapters (src/hooks/alt_hud.cpp,
// behind --native_alt_hud) and the harness (without the flag); those of
// functions only native callers reach have no adapter, and only the harness
// calls them. Each leaves r3 (and f1) as the original does; the ones over
// the allocation layer (MenuList::Release, AltHud::Destroy) run the original
// where it isn't active or the block isn't the layer's.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// MenuList::Release sub_8220BBB0 and MenuList::PressedX sub_8220BF00: r3 =
// the list.
void AltHudMenuListRelease(lifted::Regs& ctx, uint8_t* base);
void AltHudMenuListPressedX(lifted::Regs& ctx, uint8_t* base);
// AltHud::PowerClass sub_82216B08: r4 = the car object.
void AltHudPowerClass(lifted::Regs& ctx, uint8_t* base);
// AltHud::SetUpGauges sub_82216C40, SetUpExtraGauges sub_822178A0,
// ShowPanels sub_82216D78: r3 = the holder's +1004. ShowGauge sub_82217380:
// r4 = the slot, r5 = shown. SetUp sub_82217C70: r3 = the holder's +1004,
// r4 = the player.
void AltHudSetUpGauges(lifted::Regs& ctx, uint8_t* base);
void AltHudSetUpExtraGauges(lifted::Regs& ctx, uint8_t* base);
void AltHudShowPanels(lifted::Regs& ctx, uint8_t* base);
void AltHudShowGauge(lifted::Regs& ctx, uint8_t* base);
void AltHudSetUp(lifted::Regs& ctx, uint8_t* base);
// AltHud::SetSpeed sub_82216E00: r4 = the speed. UpdateExtraGauges
// sub_82217A30: r3 = the holder's +1004, f1, r5, r6. SetGear sub_82217460
// and SetNode25 sub_822174E0: r4. SetTach sub_822173E8, SetBar sub_822175A0,
// SetBoost sub_82217538: r3 = the holder's +1004, f1. SetThrottle
// sub_82217638, SetBrake sub_82217690, SetSteering sub_822176E8: f1.
void AltHudSetSpeed(lifted::Regs& ctx, uint8_t* base);
void AltHudUpdateExtraGauges(lifted::Regs& ctx, uint8_t* base);
void AltHudSetGear(lifted::Regs& ctx, uint8_t* base);
void AltHudSetNode25(lifted::Regs& ctx, uint8_t* base);
void AltHudSetTach(lifted::Regs& ctx, uint8_t* base);
void AltHudSetBar(lifted::Regs& ctx, uint8_t* base);
void AltHudSetBoost(lifted::Regs& ctx, uint8_t* base);
void AltHudSetThrottle(lifted::Regs& ctx, uint8_t* base);
void AltHudSetBrake(lifted::Regs& ctx, uint8_t* base);
void AltHudSetSteering(lifted::Regs& ctx, uint8_t* base);
// AltHud::Tick sub_82217AC8 and Destroy sub_82216BA8: r3 = the holder's
// +1004. RunMovie sub_82216C00: no arguments.
void AltHudTick(lifted::Regs& ctx, uint8_t* base);
void AltHudDestroy(lifted::Regs& ctx, uint8_t* base);
void AltHudRunMovie(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
