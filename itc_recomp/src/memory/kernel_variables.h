// The kernel's variables the game's image imports, plugged in (E2 step 2,
// docs/rexglue-removal.md "E2: the plan"): see kernel_variables.cpp.

#pragma once

#include <cstdint>

namespace memory {

// Points the image's import slots of the variables the game reads at
// blocks of ours (from the allocation layer) holding what the game needs
// of them. After the image is loaded, before any game code runs.
void BindKernelVariables();

}  // namespace memory
