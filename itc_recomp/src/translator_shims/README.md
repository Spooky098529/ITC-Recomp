# Shims for the SDK's shader translator

The SDK's Xenos-to-SPIR-V translator is our one static dependency on the
ReXGlue SDK (E4 in `docs/rexglue-removal.md`, step 5): its sources and
headers are built from the `rexglue-sdk` submodule, but not the SDK's
logging (spdlog) or flag registry. These headers come before the SDK's on
the include path and stand in for those:

- `rex/logging/types.h`, `api.h` and `format.h`: the translator's log lines
  go into our log (`src/base/log.h`), formatted with our fmt copy.
- `rex/cvar.h`: its flags are plain values at their defaults (none of ours
  sets them; they were never set before either).
