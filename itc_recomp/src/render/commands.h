// The game's render commands (docs/functions/render-commands.md) decoded
// into our interface (render/device.h): the conventional renderer's seam
// (docs/conventional-renderer.md, step 1). The command list's format stays
// the game's; what each command did through the game's Direct3D is done
// here to the interface's state, and draws, clears, resolves and tiling go
// to the backend.
//
// The game's behavior the handlers keep (its Direct3D's, at the level of
// what a frame looks like): setting render target 0 (or a depth surface
// with none) resets the viewport and scissor rectangle to the whole
// surface; viewports are clipped to the surface (or the tiling); the blend
// render states set all four targets' blending, command 3 one target's;
// depth and stencil tests apply only with a depth surface (the backend's
// part). Calls into game code that isn't Direct3D (a resource's Release,
// the retire list's objects, callbacks, a drawer other than the primitive
// drawer) are guest calls through `GameCall`.
//
// The render queue's replay (src/graphics/render_queue.cpp) runs these
// instead of the recompiled handlers, and the Vulkan renderer draws from
// the interface (its backend, src/native/renderer.cpp).

#pragma once

#include "render/device.h"

#include <cstdint>
#include <functional>

namespace render {

// A guest call to `function` with the first `count` of r3, r4, r5 set;
// returns r3.
using GameCall = std::function<uint64_t(uint32_t function, int count, uint64_t r3, uint64_t r4,
                                        uint64_t r5)>;

class Commands {
 public:
  // Command `id` of the table at 0x8265EDE0 with its payload, count (arg1)
  // and arg2; returns the slots its payload took (the handler's r3).
  uint32_t Run(uint8_t* base, uint32_t id, uint32_t payload, uint32_t count, uint32_t arg2,
               const GameCall& call);
  // A single command (the table at 0x8265ED48) at `slot`; returns the
  // slots after it its items took.
  uint32_t RunSingle(uint8_t* base, uint32_t slot, const GameCall& call);
  // Command 7's upload, from the payload as interpolation leaves it
  // (BlendState_ReplayConstants's upload).
  void SetShaderConstants(uint8_t* base, uint32_t payload, uint32_t count, uint32_t packed);

  State& state() { return state_; }
  // Null: the state alone (the check).
  void set_backend(Backend* backend) { backend_ = backend; }
  // Called at the replay's start: the game's defaults the first time.
  void BeginReplay(uint8_t* base);

  // What the check reads: the game's shader patches applied to constants
  // the interface doesn't hold (logged once each).
  uint64_t unhandled_patch_records() const { return unhandled_patch_records_; }

 private:
  void SetRenderState(uint8_t* base, uint32_t state, uint32_t value);
  void SetSamplerState(uint8_t* base, uint32_t sampler, uint32_t state, uint32_t value);
  void SetRenderTarget(uint8_t* base, uint32_t index, uint32_t surface);
  void SetDepthStencilSurface(uint8_t* base, uint32_t surface);
  void SetSurfaceInfo(uint8_t* base);
  // The viewport (x, y, width, height, min Z, max Z) clipped as the game's
  // SetViewport does.
  void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height, float min_z,
                   float max_z, uint8_t* base);
  void SetVertexShader(uint8_t* base, uint32_t shader);
  void SetPixelShader(uint8_t* base, uint32_t shader);
  void ApplyShaderPatches(uint8_t* base, uint32_t patches, bool pixel);
  void UpdateBlend();
  void SetVertexConstants(uint8_t* base, uint32_t first, uint32_t data, uint32_t count,
                          bool pixel);
  void SetBoolConstants(uint32_t first, const uint32_t* values, uint32_t count, bool pixel);
  void SetLoopConstants(uint8_t* base, uint32_t first, uint32_t data, uint32_t count,
                        bool pixel);
  void DrawDrawer(uint8_t* base, const GameCall& call, bool single);
  void InlineDraw(uint8_t* base, uint32_t type, uint32_t data, uint32_t count, uint32_t stride);
  void Resolve(uint8_t* base, uint32_t item);

  State state_;
  Backend* backend_ = nullptr;
  bool defaults_set_ = false;
  // The game's blend state: the software copy of the blend control (its
  // render states' fields) and whether blending and separate alpha are on;
  // the four targets' controls as last set (command 3 sets one).
  uint32_t blend_control_ = 0;
  bool blending_ = false, separate_alpha_ = false;
  uint32_t target_blend_[kTargets] = {0x00010001, 0x00010001, 0x00010001, 0x00010001};
  // Commands 34 and 35's saved set.
  Surface saved_targets_[kTargets], saved_depth_;
  Viewport saved_viewport_;
  // Predicated tiling (between commands 32 and 33): the tiles' extent.
  bool tiling_ = false;
  uint32_t tiling_width_ = 0, tiling_height_ = 0;
  uint32_t tile_rects_ = 0, tile_count_ = 0;
  uint64_t unhandled_patch_records_ = 0;
};

// The handlers (render thread).
Commands* GetCommands();

}  // namespace render
