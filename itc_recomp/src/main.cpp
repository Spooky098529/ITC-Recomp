// itc - ReXGlue Recompiled Project

#include "generated/default/itc_init.h"

#include "itc_app.h"

#include <cstdlib>

namespace {

// Defaults this game needs, applied before main() as REX_* environment
// variables. The environment reaches every cvar, including ones the GPU plugin
// registers after it loads; command-line flags still override them, and so
// does anything the user already set in their own environment.
struct ItcDefaults {
  ItcDefaults() {
    setenv("REX_GPU_PLUGIN", "xenos", 0);
    // Keyboard controls (a gamepad keeps working alongside).
    setenv("REX_MNK_MODE", "true", 0);
    // The direct resolve path leaves the motion blur's afterimage texture
    // stale: weak blur, and light trails from the previous camera stay on
    // screen after switching view.
    setenv("REX_DIRECT_HOST_RESOLVE", "false", 0);
  }
} itc_defaults;

}  // namespace

REX_DEFINE_APP(itc, ItcApp::Create)
