// The achievements' register steps (src/achievements/,
// docs/functions/achievements.md), shared by the adapters
// (src/hooks/achievements.cpp, behind --native_achievements) and the
// harness (without the flag).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// Time::Less sub_820FF708: r3, r4 = &the two packed times.
void TimeLessSteps(lifted::Regs& ctx, uint8_t* base);
// Achievements::Check sub_82290788: r3 = the kind, r4 = the 27 words out.
void AchievementsCheckSteps(lifted::Regs& ctx, uint8_t* base);
// Achievements::Write sub_82290A30: r3 = the 27 words; XUserWriteAchievements
// through the function table.
void AchievementsWriteSteps(lifted::Regs& ctx, uint8_t* base);
// XUserWriteAchievements sub_82294E58 (r3 the count, r4 the list, r5 the
// overlapped): plugged in (nothing recorded), entered through lifted::Enter.
void WriteAchievementsSteps(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
