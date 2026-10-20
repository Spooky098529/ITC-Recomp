// OutputDebugStringA (sub_82296270) natively: an adapter in front of
// helpers::DebugOutput (src/helpers/debug_output.cpp). The text isn't read: the
// game's own discards it.

#include "guest.h"

#include "helpers/debug_output.h"

// r3 = the text.
extern "C" REX_FUNC(sub_82296270) {
  REX_FUNC_PROLOGUE();
  helpers::DebugOutput(nullptr);
}
