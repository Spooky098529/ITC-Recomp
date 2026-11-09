// The tuning screen machine's register steps (src/tuning_machine/,
// docs/functions/tuning-machine.md), shared by the adapters
// (src/hooks/tuning_machine.cpp, behind --native_tuning_machine) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// The machine's handlers (r3 the screen, r4 the mode where read).
void TuningMachineCreate(PPCContext& ctx, uint8_t* base);
void TuningMachineZeroRecord16(PPCContext& ctx, uint8_t* base);
void TuningMachineResetImages(PPCContext& ctx, uint8_t* base);
void TuningMachineOpenMovies(PPCContext& ctx, uint8_t* base);
void TuningMachineWaitForMovies(PPCContext& ctx, uint8_t* base);
void TuningMachineOnDialogClosed(PPCContext& ctx, uint8_t* base);
void TuningMachineUnload(PPCContext& ctx, uint8_t* base);
void TuningMachineClose(PPCContext& ctx, uint8_t* base);
void TuningMachineDestroy(PPCContext& ctx, uint8_t* base);
// The decal images' placeholders (r3 the decal images).
void TuningMachineSavePlaceholders(PPCContext& ctx, uint8_t* base);
void TuningMachineSavePlaceholders14(PPCContext& ctx, uint8_t* base);
// Init (r3 the machine).
void TuningMachineInit(PPCContext& ctx, uint8_t* base);
// The screen's set-up's callees (r3 the screen).
void TuningMachineLoadInteriorSlots(PPCContext& ctx, uint8_t* base);
void TuningMachineLoadCurrentChoices(PPCContext& ctx, uint8_t* base);
void TuningMachineLoadOwnedParts(PPCContext& ctx, uint8_t* base);
void TuningMachineSetPrices(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
