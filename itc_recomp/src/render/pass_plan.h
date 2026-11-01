// The frame's pass plan (docs/conventional-renderer.md, "Step 3"): the
// command list scanned once before it's replayed, so a pass can draw
// straight into the texture it will be resolved into. Each target's draws
// are cut into segments, each ending at a resolve of that target, and each
// resolve gets a kind: drawn into (its segment draws into the
// destination), a multisample resolve (at the pass's end), or a copy.
//
// The scan decodes only what the plan needs: target binds (commands 23,
// 24, 29, 34, 35), resolves (25) and tiling (32, 33); every other command
// is skipped by its size (Commands::Slots).

#pragma once

#include <cstdint>
#include <vector>

namespace render {

// A target as the 360's EDRAM holds it: the surfaces drawing into the same
// one share it. Formats drawn the same way share a class.
struct TargetId {
  bool depth = false;
  bool tiled = false;  // drawn while tiling (a whole image of the tiles)
  uint32_t edram_base = 0, pitch = 0, msaa = 0, format_class = 0;

  bool operator==(const TargetId&) const = default;
};

// The class of a color surface's format (RB_COLOR_INFO bits 16-19):
// formats the renderer draws in one host format share it.
uint32_t TargetFormatClass(uint32_t format);
// A surface object's target (its +24 RB_SURFACE_INFO, +28 RB_COLOR_INFO or
// RB_DEPTH_INFO).
TargetId SurfaceTarget(uint8_t* base, uint32_t surface, bool depth, bool tiled);

enum class ResolveKind : uint8_t {
  kSkipped,      // no source or destination: nothing to resolve
  kDrawnInto,    // its segment draws straight into the destination
  kMultisample,  // an MSAA source: resolved at the pass's end
  kCopy,         // a part of the target, a level or slice, a tile
};
const char* ResolveKindName(ResolveKind kind);

struct PlannedResolve {
  TargetId source;
  uint32_t destination = 0;  // the texture's header
  uint32_t flags = 0;
  // Its source's resolves in the frame before it: its segment's number.
  uint32_t segment = 0;
  ResolveKind kind = ResolveKind::kSkipped;
};

struct PassPlan {
  // False when the list held something the scan can't walk (chain kinds 1
  // and 2, never seen in play): every resolve is then a copy.
  bool complete = false;
  std::vector<PlannedResolve> resolves;  // in the replay's order
};

}  // namespace render
