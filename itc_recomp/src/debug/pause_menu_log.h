#pragma once

#include <cstdint>

namespace debug {

// Counts a run of the mappers' update sub_8210B018 from `caller` for
// --log_pause_menu (its override is in hooks/input.cpp).
void PauseMenuLog_NoteMapperUpdate(uint32_t caller);

}  // namespace debug
