// The game's XAudio render driver object (docs/functions/audio.md) as plain
// C++: what the wrappers in src/audio/native_audio.cpp run inside them.

#pragma once

#include <cstdint>

namespace audio {

// AudioRenderDriver::Destroy (sub_824658C8): r3 = the driver. Its vtables
// put back, the handle at +16 unregistered (XAudioUnregisterRenderDriverClient
// through its import, as the original calls it; +16 cleared when that
// succeeds) when it isn't 0. Takes the stack pointer and r13 it's called
// with; returns the original's r3 (the driver).
uint64_t RenderDriverDestroy(uint32_t sp, uint64_t r13, uint32_t driver);

}  // namespace audio
