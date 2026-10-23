#pragma once

#include <cstdint>

namespace debug {

// Counts a run of the mappers' update sub_8210B018 from `caller` for
// --log_pause_menu (its override is in hooks/input.cpp).
void PauseMenuLog_NoteMapperUpdate(uint32_t caller);

// For --log_pause_menu, around one mapper's update sub_8210C370 (its
// adapter is in hooks/action_mappers.cpp, and the native mappers' update
// calls these too): Before returns the mapper's fired word (0 when the flag
// is off); After logs the update when a direction is held or fires.
uint32_t PauseMenuLog_BeforeMapperUpdate(uint8_t* base, uint32_t mapper);
void PauseMenuLog_AfterMapperUpdate(uint8_t* base, uint32_t mapper, uint32_t caller,
                                    uint32_t fired_before);

// For --log_pause_menu: a call to one of the mapper's read slots (`slot` its
// vtable slot, the result as `value`). Nothing when the flag is off.
void PauseMenuLog_NoteSlotRead(int slot, uint32_t mapper, uint32_t action, uint32_t caller,
                               double value);

}  // namespace debug
