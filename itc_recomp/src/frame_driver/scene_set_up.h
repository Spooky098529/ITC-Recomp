// The app's scene's set-up as plain C++ (docs/functions/frame-driver.md):
// Scene::Scene and what only it calls (CommandList::Init, the color
// rectangles' targets, the graphics library's draw globals and the
// primitive vertex declaration). The guest calls (the interfaces' slots,
// the render queue's constructor and Init, CreateVertexDeclaration,
// operator new and delete, the memory blocks' makers, the run loop's and
// the frame driver's adds) are made by the adapter
// (src/hooks/frame_driver.cpp) from frames like the originals'; each goes
// through its own adapter there, so it runs as that function's plain C++
// where it has one.

#pragma once

#include <cstdint>
#include <functional>

namespace frame_driver {

struct SceneSetUpCalls {
  // `object`'s vtable slot at `byte` with r3 = `object`, r4 and r5,
  // returning to `return_address`; its r3.
  std::function<uint32_t(uint32_t object, uint32_t byte, uint64_t r4, uint64_t r5,
                         uint32_t return_address)>
      vcall;
  // RenderQueue::RenderQueue (sub_825915E0) on `queue`.
  std::function<void(uint32_t queue)> render_queue_construct;
  // CreateVertexDeclaration (sub_822A8980) on `elements`, returning to
  // `return_address`; the declaration.
  std::function<uint32_t(uint32_t elements, uint32_t return_address)> create_declaration;
  // operator new (sub_825A6940) and operator delete (sub_825A71C0).
  std::function<uint32_t(uint32_t bytes)> operator_new;
  std::function<void(uint32_t block)> operator_delete;
  // Memory::AllocPhysicalEx (sub_8258E098: r4 0, r5 -1, r6 and r7 4) and
  // Memory::AllocVirtual (sub_8258DFF0), returning to `return_address`.
  std::function<uint32_t(uint32_t bytes, uint32_t return_address)> alloc_physical;
  std::function<uint32_t(uint32_t bytes, uint32_t return_address)> alloc_virtual;
  // `body(frame)` in a guest frame of `bytes` (its back chain stored)
  // with the link register `return_address`.
  std::function<void(uint32_t bytes, uint32_t return_address,
                     const std::function<void(uint32_t frame)>& body)>
      in_frame;
  // A nonvolatile register set as the original sets it.
  std::function<void(int reg, uint64_t value)> set_register;
};

// sub_8258CD28 CommandList::Init: the old memory (+20) deleted, `slots`
// rounded up to even, operator new(slots x 16; -1 above 0x0FFFFFFF slots)
// into +20 and +24, +28 the second half, +8 half the slots, and both
// buffers begun (+4 flipped twice, +32, +16, +12) as the original leaves
// them. Returns 1. From a 112-byte frame (its calls').
uint32_t CommandListInit(uint8_t* base, uint32_t list, uint32_t slots,
                         const SceneSetUpCalls& calls);

// sub_82178B28 ColorRect::SetTargets: [0x826D83D0] = `queue`,
// [0x826D83D4] = `list`.
void ColorRectSetTargets(uint8_t* base, uint32_t queue, uint32_t list);

// sub_82589E78 DrawGlobals::Init: from a 368-byte frame, the identity
// matrix at 0x8265ECA0, the float4 defaults (0x8265ECE0 to 0x8265ED0F, the
// lighting constants 0x8296B950 to 0x8296BCDF: image constants, each word
// through a float's load and store as the original), the 77 vertex
// declarations (eleven tables of seven, created in the original's order),
// the work memory (0x8296B780: its two `capacity` buffers, `count` x 64
// bytes twice when `count` isn't 0, the counters 0), the defaults at
// 0x8265ED20 and 0x8265ED30 and the sort keys [0x8296B90C] = 0,
// [0x8296B900] = 1. r22 and r23 as the original keeps them.
void DrawGlobalsInit(uint8_t* base, uint32_t capacity, uint32_t count,
                     const SceneSetUpCalls& calls);

// sub_82591858 PrimitiveDecl::Init: from a 96-byte frame,
// [0x82970BC8] = CreateVertexDeclaration(0x8206CBB4), [0x82970BDC] = 1.
// Returns 1.
uint32_t PrimitiveDeclInit(uint8_t* base, const SceneSetUpCalls& calls);

// sub_82098A98 Scene::Scene: `scene` (80 bytes), `block` (+0 the run
// loop, +4 the framework's +8 interface), its 192-byte frame at `frame`:
// the vtable, the command list (CommandList::CommandList's plain C++) and
// the render queue made, the interface kept, the video output (the
// interface's slot 1, then its slot 3: FrameDriver::GetVideoOutput's plain
// C++ where it is that), the device locked into the frame's +80, the list
// sized (512,000 slots), the queue's Init (slot 0), the color rectangles'
// targets, the draw globals (2 MB, 16,384), the list and queue published
// ([0x8296B918], [0x8296B91C], [0x82970BD8], [0x82970BD4]), the primitive
// declaration, the device unlocked; then Scene::Draw added as the frame
// driver's listener 1, Scene::Update as the run loop's tick handler 18 and
// Scene::OnEvent5 as its listener for event 5, each delegate built in the
// frame as the original builds it. r27 to r31 as the original keeps them.
// Returns `scene`.
uint32_t SceneConstruct(uint8_t* base, uint32_t scene, uint32_t block, uint32_t frame,
                        const SceneSetUpCalls& calls);

}  // namespace frame_driver
