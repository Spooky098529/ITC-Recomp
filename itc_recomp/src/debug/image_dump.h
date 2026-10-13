// Diagnostics (--dump_image): writes the game's loaded executable image to a
// file, for tools that read its data (tools/scan_ptrs.py, dispatch tables).

#pragma once

#include <cstdint>

namespace debug {

// Main thread, once per main loop pass. On the first call with --dump_image
// set, writes the image and exits.
void ImageDump_OnPass(uint8_t* base);

}  // namespace debug
