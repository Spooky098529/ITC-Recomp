// The SDK's own log lines (its shader translator's, until step 5 of E4 in
// docs/rexglue-removal.md) forwarded into our log (log_sdk.cpp).

#pragma once

#include <string_view>

namespace base {

// Sets up the SDK's logging with no file of its own, at `level` (the
// --log_level spelling), its lines going into ours.
void ForwardSdkLog(std::string_view level);

}  // namespace base
