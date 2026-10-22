// Vehicle functions made native (src/hooks/vehicle.cpp), for native callers.

#pragma once

#include <cstdint>

// Vehicle::GetSpeedKmh (sub_820C0128): the float at vehicle + 48 (m/s) times
// 3.6. Native callers call this directly; the adapter also tells the state
// capture (--capture_dir) about the vehicle, which this doesn't.
float Vehicle_GetSpeedKmh(uint8_t* base, uint32_t vehicle);
