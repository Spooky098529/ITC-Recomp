// Resources made, written and freed through our interface
// (docs/conventional-renderer.md, "Step 2"): called where the game makes,
// fills and frees them, on any thread. The events are queued and handed
// to the backend on the render thread before the next list it replays, so
// a list never sees a texture freed after it was recorded, and always sees
// those made before.
//
// A texture is named by its header's guest address (what the command list
// binds), as everywhere in the interface until resources leave guest
// memory (step 2b).

#pragma once

#include "render/device.h"

namespace render {

// The header is filled (its data may still be to come: the backend reads it
// when the texture is first drawn). Again for the same handle: the texture
// is made again (its header changed).
void TextureCreated(Texture texture);
// Its data was written after it was made (the backend reloads it).
void TextureWritten(Texture texture);
// Its header or data is freed: the handle names nothing until made again.
void TextureDestroyed(Texture texture);

// Main thread: a list was handed over to the render thread; the events
// queued so far go before it.
void ResourceEvents_OnListHandedOver();
// Render thread, at a list's replay start (once per hand-over, in order).
void ResourceEvents_OnReplayStart();
// Render thread: hands the events due before the list being replayed to
// `backend`, in the order they were queued.
void ResourceEvents_Apply(Backend& backend);

}  // namespace render
