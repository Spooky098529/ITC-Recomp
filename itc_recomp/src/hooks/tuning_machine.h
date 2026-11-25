// The tuning screen machine's register steps (src/tuning_machine/,
// docs/functions/tuning-machine.md), shared by the adapters
// (src/hooks/tuning_machine.cpp, behind --native_tuning_machine) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The machine's handlers (r3 the screen, r4 the mode where read).
void TuningMachineCreate(lifted::Regs& ctx);
void TuningMachineZeroRecord16(lifted::Regs& ctx);
void TuningMachineResetImages(lifted::Regs& ctx);
void TuningMachineOpenMovies(lifted::Regs& ctx);
void TuningMachineWaitForMovies(lifted::Regs& ctx);
void TuningMachineOnDialogClosed(lifted::Regs& ctx);
void TuningMachineUnload(lifted::Regs& ctx);
void TuningMachineClose(lifted::Regs& ctx);
// The no answers and the notice's close (r3 the screen); StickerDialogNo's
// step is the harness's (its only caller, OnDialogNo, calls its plain C++).
void TuningMachineOnDialogNo(lifted::Regs& ctx);
void TuningMachineStickerDialogNo(lifted::Regs& ctx);
void TuningMachineOnNoticeClosed(lifted::Regs& ctx);
void TuningMachineDestroy(lifted::Regs& ctx);
// The decal images' placeholders (r3 the decal images).
void TuningMachineSavePlaceholders(lifted::Regs& ctx);
void TuningMachineSavePlaceholders14(lifted::Regs& ctx);
// Init (r3 the machine).
void TuningMachineInit(lifted::Regs& ctx);
// The screen's set-up's callees (r3 the screen).
void TuningMachineLoadInteriorSlots(lifted::Regs& ctx);
void TuningMachineLoadCurrentChoices(lifted::Regs& ctx);
void TuningMachineLoadOwnedParts(lifted::Regs& ctx);
void TuningMachineSetPrices(lifted::Regs& ctx);
void TuningMachineLoadUnlockedParts(lifted::Regs& ctx);
// The screen's set-up (r3 the screen).
void TuningMachineSetUp(lifted::Regs& ctx);
// Start (r3 the screen; r3 FadeIn's, f1 as the refresh leaves it), with
// TuningMachineStartNative: --native_tuning_screen, the tuning screen's stand
// and car stats callees (TuningStandCalleesNative,
// TuningCarStatsCalleesNative) and --native_car_specs, the refresh's
// conditions.
bool TuningMachineStartNative();
void TuningMachineStart(lifted::Regs& ctx);

}  // namespace hooks
