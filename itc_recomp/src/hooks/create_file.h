// The game's CreateFile's register steps (src/hooks/create_file.cpp).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// CreateFile sub_82295508 (r3 the path, r4 the access, r5 the sharing, r7
// the disposition, r8 the flags): game_data::CreateFile entered through
// lifted::Enter, without the flag (the adapter's and the harness's).
void CreateFileSteps(PPCContext& ctx, uint8_t* base);
void CreateFileSteps(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
