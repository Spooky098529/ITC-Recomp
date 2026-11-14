// The type 8 mode (src/type8_mode/, docs/functions/type8-mode.md) against
// the recompiled functions: the originals run with --native_type8_mode off,
// the plain C++ through the register steps (hooks/type8_mode.h). The
// dialog's cases are laid in type67_modes.cpp (Type8Dialogs), over the type
// 6 and 7 dialogs' worlds.

#include "harness.h"

namespace harness {

void Type8Dialogs(Context& h);

void Type8Mode(Context& h) { Type8Dialogs(h); }

}  // namespace harness
