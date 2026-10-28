// The AI drivers' calls for native callers that still hand them the guest
// context (src/hooks/ai_drivers.cpp): the racer controllers
// (src/hooks/racer_controllers.cpp) and the race world's start
// (src/hooks/race_world.cpp).

#pragma once

#include "guest.h"

namespace hooks {

// AiDriver::DriveCar (sub_8215F678) as its adapter runs it: r3 = the car's
// input record, r4 = the racer's index, r5 = the flag, r1 the caller's
// stack pointer; the plain C++ (src/ai_drivers/) with --native_ai_drivers,
// the original where it can't be native or with the flag off. r3 and f1 as
// the original leaves them.
void AiDriveCar(PPCContext& ctx, uint8_t* base);
// AiDriver::UpdateIdle (sub_821399A8) as its adapter runs it: r3 = the
// driver, r1 the caller's stack pointer.
void AiUpdateIdle(PPCContext& ctx, uint8_t* base);

// AiDrivers::UpdateAll (sub_82139628) as its adapter runs it: r1 the
// caller's stack pointer; the plain C++ with --native_ai_drivers (the
// drivers' updates through their +6148), the original with the flag off. r3
// as the original leaves it.
void AiUpdateAll(PPCContext& ctx, uint8_t* base);

// For harnesses: while set, these run instead (from the adapters and the
// functions above), answering with made-up data.
using GuestStandIn = void (*)(PPCContext&, uint8_t*);
extern GuestStandIn g_drive_car_stand_in;
extern GuestStandIn g_update_idle_stand_in;
extern GuestStandIn g_update_all_stand_in;

}  // namespace hooks
