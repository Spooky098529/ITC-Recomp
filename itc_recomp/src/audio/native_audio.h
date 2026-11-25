// Our own audio output, replacing the SDK's render driver and Audio Worker
// (see docs/native-audio.md).

#pragma once

namespace native_audio {

// Starts our audio output's thread (once, from main(), after guest memory
// is mapped); the game's render driver reaches it through the hooks in
// native_audio.cpp.
void Start();

}  // namespace native_audio
