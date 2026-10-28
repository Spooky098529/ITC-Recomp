// What the car physics' adapters (src/hooks/car_physics.cpp) share with
// native callers of their plain C++.

#pragma once

#include "car_physics/car_physics.h"
#include "guest.h"

namespace hooks {

// The guest calls CollisionWorld::Update's plain C++ makes (the originals
// where a native callee refuses, and the queries' targets other than the
// track's), as its adapter runs them.
car_physics::CollisionGuestCall CollisionWorldUpdateCalls(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
