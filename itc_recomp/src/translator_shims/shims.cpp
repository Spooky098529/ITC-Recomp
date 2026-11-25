// Definitions for the shims (see README.md): the flags the SDK's logging
// headers declare, at the SDK's defaults.

#include <rex/cvar.h>

#include <cstdint>
#include <string>

REXCVAR_DEFINE_BOOL(log_noisy, false, "Log", "");
