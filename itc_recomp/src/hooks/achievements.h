// The achievements' register steps (src/achievements/,
// docs/functions/achievements.md), shared by the adapters
// (src/hooks/achievements.cpp, behind --native_achievements) and the
// harness (without the flag).

#pragma once

#include "guest.h"

namespace hooks {

// Time::Less sub_820FF708: r3, r4 = &the two packed times.
void TimeLessSteps(PPCContext& ctx, uint8_t* base);
// Achievements::Check sub_82290788: r3 = the kind, r4 = the 27 words out.
void AchievementsCheckSteps(PPCContext& ctx, uint8_t* base);
// Achievements::Write sub_82290A30: r3 = the 27 words; XUserWriteAchievements
// through the function table.
void AchievementsWriteSteps(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
