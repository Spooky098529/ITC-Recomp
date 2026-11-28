// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 34's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "hooks/xaudio.h"
#include "hooks/xaudio_engine.h"
#include "hooks/xaudio_objects.h"

// The stale originals round: XAudio's ports through their register steps
// (each maps its registers and frames, and runs the original where its port
// declines).
ITC_DIRECT_ENTRY(8245B720) { hooks::XAudioSwapStageLists(ctx); }
ITC_DIRECT_ENTRY(8245DC88) { hooks::XAudioSourceOutputFormat(ctx); }
ITC_DIRECT_ENTRY(8245F8E8) { hooks::XAudioBufferClear(ctx); }
ITC_DIRECT_ENTRY(824605E8) { hooks::XAudioDefaultAllocator(ctx); }
ITC_DIRECT_ENTRY(82469AF8) { hooks::XAudioCodecOnSubmit(ctx); }
