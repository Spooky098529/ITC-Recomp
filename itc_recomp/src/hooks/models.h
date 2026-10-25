// The model class's adapters (hooks/models.cpp): what the harness shares.

#pragma once

namespace hooks {

// Whether the memory blocks' lock is the host mutex (models/memory_blocks.h)
// for this run: --native_models as it was the first time this was asked
// (fixed for the run, so no user takes the game's critical section while
// another takes the mutex, even while a harness switches the flag).
bool ModelsHostLock();

}  // namespace hooks
