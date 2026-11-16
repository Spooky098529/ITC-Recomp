// The boot mode's register steps (src/boot_mode/,
// docs/functions/boot-mode.md), shared by the adapters (src/hooks/boot_mode.cpp
// and the wraps in src/hooks/title_screen.cpp, behind --native_boot_mode) and
// the harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "boot_mode/boot_mode.h"
#include "hooks/native_entry.h"
#include "guest.h"

namespace hooks {

// --native_boot_mode.
bool BootModeNative();
// The plain C++'s guest calls through `ctx` (r1 and lr put back after), r13,
// --play_movies_with_ffmpeg and `finished_early` (Movies::State's --skip_intro
// answer; may be empty).
boot_mode::Calls BootModeCalls(lifted::Regs& ctx, uint8_t* base,
                               std::function<bool()> finished_early = {});

// BootMode::Init sub_82107F68 and BootMachine::Init sub_82107DD0 (r3 the mode
// and the machine).
void BootModeInit(lifted::Regs& ctx, uint8_t* base);
void BootMachineInit(lifted::Regs& ctx, uint8_t* base);
// Boot::StartMovie sub_82107B90, WaitForMovie sub_82107C58 (r3 the machine,
// r4 the dispatcher; `finished_early` from the wrap) and EndMovie
// sub_82107BF8.
void BootStartMovie(lifted::Regs& ctx, uint8_t* base);
void BootWaitForMovie(lifted::Regs& ctx, uint8_t* base, std::function<bool()> finished_early);
void BootEndMovie(lifted::Regs& ctx, uint8_t* base);
// Movies::State sub_820A4450 (the wrap's; `finished_early` its --skip_intro
// answer).
void BootMoviesState(lifted::Regs& ctx, uint8_t* base, std::function<bool()> finished_early);
// BootMode::NextMode sub_82108018 (r3 the answer, r4 the mode).
void BootModeNextMode(lifted::Regs& ctx, uint8_t* base);
// BootMode::~BootMode sub_82106D08, BootMode::DeletingDestructor sub_82106D68
// and Mode::DeletingDestructor sub_82106E38 (r3 the mode, r4 the flags).
void BootModeDestruct(lifted::Regs& ctx, uint8_t* base);
void BootModeDeletingDestructor(lifted::Regs& ctx, uint8_t* base);
void ModeDeletingDestructor(lifted::Regs& ctx, uint8_t* base);
// Mode::NextModeThunk sub_82235C08 (r3 the answer, r4 the mode) and
// ModeDelegate::Invoke sub_8223D440 (r3 the answer, r4 the delegate).
void ModeNextModeThunk(lifted::Regs& ctx, uint8_t* base);
void ModeDelegateInvoke(lifted::Regs& ctx, uint8_t* base);
// ScreenMachine::ScreenMachine sub_82107358 (r3 the machine).
void ScreenMachineConstruct(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
