// The scene target's passes as plain C++ (B7 in docs/rexglue-removal.md;
// the functions are in docs/functions/scene-target.md, the scene target's
// fields in docs/known-data.md, 0x8266D08C): the chunks that bind the scene
// target's surfaces, clear them, resolve them into its 1280 x 720 texture
// and draw that texture back over the screen, and the textured screen
// rectangle that draw goes through. They record into guest command lists
// that the renderer replays, so they work on guest memory with the
// originals' loads and stores. Their guest calls (the render manager's
// getters, the render queue's Add, TexturedRect::Record from
// RecordPasses) are the adapter's (src/hooks/scene_target.cpp). Each
// declines (nothing done) where the original's command list calls would
// print a message (the list without room, a chunk open): the original runs
// then.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace scene_target {

// The scene target, made once at boot (src/app_start_up/scene_target.cpp).
constexpr uint32_t kSceneTarget = 0x8266D08C;
// The textured and color rectangles' render queue (+0) and command list
// (+4).
constexpr uint32_t kRectLists = 0x826D83D0;

// sub_8209AE48 SceneTarget::RecordTargets: in the open chunk of `list`,
// command 23 with render targets 0-3 all the scene target's +4, then
// command 24 with its +0 (the items' unread words left as they were).
// Returns the last slot (the original's r3); nullopt where the list hasn't
// room for the 7 slots.
std::optional<uint32_t> RecordTargets(uint8_t* base, uint32_t scene, uint32_t list);
// sub_8209AF40 SceneTarget::RecordTileTargets: the same with +12 and +8
// (the tile-sized pair).
std::optional<uint32_t> RecordTileTargets(uint8_t* base, uint32_t scene, uint32_t list);
// sub_8209B058 SceneTarget::RecordBoundTargets: RecordTargets on
// [kSceneTarget].
std::optional<uint32_t> RecordBoundTargets(uint8_t* base, uint32_t list);

// TexturedRect::RecordFlat's frame (its r1 less this, below its caller's).
constexpr uint32_t kRecordFlatFrame = 272;

// sub_8217AA98 TexturedRect::RecordFlat's chunk on the list at
// [kRectLists + 4]: the texture (+112), the render states by +116 (blending
// on or off), sampler 0's filters, the helper object's draw and four 24-byte
// vertices of the rectangle `rect` (+80 x, +84 y, +88 width, +92 height,
// +96 to +108 u0, v0, u1, v1); the words the original leaves in its frame
// at `frame` (+80 to +111, +124 to +172) written as it does. Returns the
// chunk's first slot, which the adapter hands to the render queue at
// [kRectLists] (its slot 4); nullopt, with nothing done, where the
// original's command list calls would print a message.
std::optional<uint32_t> TexturedRectRecordFlat(uint8_t* base, uint32_t rect, uint32_t frame);

// SceneTarget::RecordPasses's frame (its r1 less this, below its caller's).
constexpr uint32_t kRecordPassesFrame = 656;

// RecordPasses's guest calls, made by the adapter from its frame.
struct PassCalls {
  // The render manager [0x826BABA0]'s slot 8, that answer's slot 2 (the
  // render queue), and the queue's slot 4 with r4 = `chunk`, r5 =
  // `bucket`, f1 = 0.0; the three calls' return addresses.
  std::function<void(uint32_t chunk, uint32_t bucket, uint32_t lr1, uint32_t lr2, uint32_t lr3)>
      submit;
  // TexturedRect::Record (r3 = `rect`, r4 = 0, r5 = `bucket`), returning to
  // `lr`.
  std::function<void(uint32_t rect, uint32_t bucket, uint32_t lr)> rect;
};

// sub_8209B3D0 SceneTarget::RecordPasses: r3 = the scene target `scene`,
// r4 = the command list, r5, r6, r7 the three render queue buckets, its
// frame at `frame` (the caller's stack pointer less kRecordPassesFrame; r5
// and r6 also stored into the caller's frame at +36 and +44, as the
// original does). The scene target's mode (+28) gives the passes: the
// menus' predicated tiling (bit 8), the clear (bit 1), the race's binds,
// clear and shrink (bits 2 and 4), the upscale draw (bit 4), each a chunk
// handed on through `calls`, and the screen rectangles built in the frame
// as the original builds them. False, with nothing done, where the list
// hasn't room for all its slots or a chunk is open (the original's
// messages): the original runs then.
bool RecordPasses(uint8_t* base, uint32_t frame, uint32_t scene, uint32_t list, uint32_t bucket5,
                  uint32_t bucket6, uint32_t bucket7, const PassCalls& calls);

}  // namespace scene_target
