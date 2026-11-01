// Lights::FindNearest (sub_820F4128) as plain C++ for native callers
// (CarLighting::Gather, src/car_draw/); its adapter and the search are in
// hooks/lights.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace lights {

// The lights of the sets under `owner` nearest `point` (most `most`, at
// most 32), within the cone about `direction` (16-byte aligned; f3 the
// least dot product) and the farthest squared distance f2, merged within f1,
// written at `output` (32 bytes a light: the position, the w at
// [0x82000A30], the light's bytes 16 to 31), nearest first. Returns the
// count (the original's r3); nullopt, with nothing done, where the original
// must run (--native_nearest_lights off, f2 or f3 not single-precision
// values).
std::optional<uint32_t> FindNearest(uint8_t* base, uint32_t owner, uint32_t output,
                                    uint32_t point, int32_t most, uint32_t direction, double f1,
                                    double f2, double f3);

}  // namespace lights
