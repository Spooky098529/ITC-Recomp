// Tags shader constant uploads with the draw that recorded them, so the same
// object's uploads can be found again in the next tick's command list.
//
// Each recorder that writes shader constant uploads (see "Recorders" in
// docs/functions.md) is wrapped. Uploads recorded during a call are tagged
// with: the recorder, its arguments (what it draws), which call with those
// arguments this is in the tick (a model drawn several times), the upload's
// position in the call, and how many uploads the call made (so a batch that
// changed size doesn't pair with the old one).
//
// Tags are stored by the upload's address in the command list and cleared
// when the game starts recording into that buffer again, so they're always
// those of the list currently in the buffer, whenever it was recorded.

#pragma once

#include <cstdint>
#include <optional>

namespace interpolation {

using DrawTag = uint64_t;

struct DrawTagInfo {
  DrawTag tag;
  uint32_t recorder;  // guest address of the recorder function
};

// Render thread: the tag of the upload whose payload is at `payload`, if it
// was recorded by a tagged recorder.
std::optional<DrawTagInfo> DrawTags_Find(uint32_t payload);

}  // namespace interpolation
