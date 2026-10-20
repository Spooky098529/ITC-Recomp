// The game's debug output (OutputDebugStringA, sub_82296270). The retail
// game's ends in an empty function (sub_822972D0 → sub_82297DC0), so its
// text goes nowhere, as here. See B6 in docs/rexglue-removal.md.

#pragma once

namespace helpers {

void DebugOutput(const char* text);

}  // namespace helpers
