// The data load's adapters (src/hooks/data_load.cpp), for the harness: the
// register steps without the flag (each needs --native_heap and the
// archive's queue native).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// LoadingScreen::Load (sub_82247B98) and sub_82291328: no arguments.
void LoadingScreenLoadSteps(lifted::Regs& ctx);
void OtherMoviesLoadSteps(lifted::Regs& ctx);
// BootData::CreateObjects (sub_82105868) and BootData::Load (sub_82106618),
// their callees through their adapters from frames like the originals'.
void CreateObjectsSteps(lifted::Regs& ctx);
void BootDataLoadSteps(lifted::Regs& ctx);

}  // namespace hooks
