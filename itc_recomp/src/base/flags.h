// Command-line flags for our code. Forwards to the SDK's flags for now: its
// parser reads the command line and itc.toml until our own app shell (A4 in
// docs/rexglue-removal.md). Flags are defined at file scope, outside any
// namespace, and a flag defined in another file is declared before use.

#pragma once

#include <rex/cvar.h>

#define ITC_FLAG_BOOL(name, default_val, category, desc) \
  REXCVAR_DEFINE_BOOL(name, default_val, category, desc)
#define ITC_FLAG_INT32(name, default_val, category, desc) \
  REXCVAR_DEFINE_INT32(name, default_val, category, desc)
#define ITC_FLAG_STRING(name, default_val, category, desc) \
  REXCVAR_DEFINE_STRING(name, default_val, category, desc)
#define ITC_FLAG_DECLARE(type, name) REXCVAR_DECLARE(type, name)

// A flag's value.
#define ITC_FLAG(name) REXCVAR_GET(name)
