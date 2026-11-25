// OutputDebugStringA (sub_82296270) and OutputDebugStringW (sub_825BA4A0)
// natively: adapters in front of helpers::DebugOutput
// (src/helpers/debug_output.cpp). The text isn't read: the game's own
// discards it.

#include "guest.h"
#include "hooks/native_entry.h"

#include "helpers/debug_output.h"

// r3 = the text.
ITC_NATIVE_ENTRY_ALWAYS(82296270) {
  
  helpers::DebugOutput(nullptr);
}

// OutputDebugStringW: r3 = the text (UTF-16). The game's converts it to ANSI
// on the kernel's heap for OutputDebugStringA, which discards it.
ITC_NATIVE_ENTRY_ALWAYS(825BA4A0) {
  
  helpers::DebugOutput(nullptr);
}
