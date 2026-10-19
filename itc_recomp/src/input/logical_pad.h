// The game's logical pad build (sub_82591918) as plain C++: a logical pad's
// 40 values are the sum of its bound devices' values (physical pads, or for
// the "any pad" logical pad 4, logical pads 0-3), clamped. The pad lives in
// guest memory; src/hooks/input_update.cpp gathers the devices' values and
// stores the result. See "Logical pad" in docs/pause-menu-fix.md.

#pragma once

namespace input {

constexpr int kPadValues = 40;

// Adds a device's values, as each enabled binding is added in turn (float
// adds, rounded each time). The caller clears `values` first.
void LogicalPad_Add(float values[kPadValues], const float device[kPadValues]);
// Then clamps: values 0-7 to [-1, 1], values 8-39 to at most 1. NaN is kept.
void LogicalPad_Clamp(float values[kPadValues]);

}  // namespace input
