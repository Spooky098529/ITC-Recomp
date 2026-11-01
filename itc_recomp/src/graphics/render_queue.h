// The render queue's methods as plain C++ (docs/functions/device-shaders.md,
// "The render queue"): the queue at scene + 64 (vtable 0x8206CB8C), its
// state (+4: +0 the replay index, +4 the primitive drawer, +48 to +64 the
// render targets and depth surface Init keeps, +68 the release list, +84
// the submission sequence) and its two pass lists (+8, +12). Every read and
// write is the original's, in its order. Calls to code still recompiled (the
// drawer's Init, the render command handlers of the tables 0x8265EDE0 and
// 0x8265ED48, the release list's objects, a vtable slot that isn't the
// queue's own) are guest calls through `call`, from frames made where the
// originals make theirs. Adapters: src/hooks/render_queue.cpp
// (--native_render_queue).

#pragma once

#include <cstdint>
#include <functional>

namespace graphics::render_queue {

constexpr uint32_t kVtable = 0x8206CB8C;

// What a guest call leaves: r3 in full and f1.
struct CallResult {
  uint64_t r3;
  double f1;
};
// A guest call to `function` with r1 = `sp`, lr = `return_address`, and
// the first `count` of r3, r4, r5 set (the others as they are).
using GuestCall = std::function<CallResult(uint32_t function, uint32_t sp, uint32_t return_address,
                                           int count, uint64_t r3, uint64_t r4, uint64_t r5)>;

// RenderQueue::Flip (sub_8258E5E0, slot 6): the state's +0 (the replay
// index) = 1 when it was 0, else 0.
void Flip(uint8_t* base, uint32_t queue);
// RenderQueue::Recording (sub_8258E5F8, slot 7): the pass list being
// recorded, +12 when the replay index is 0, else +8.
uint32_t Recording(uint8_t* base, uint32_t queue);
// RenderQueue::Replaying (sub_8258E618, slot 8): the word at +8 + 4 x the
// replay index.
uint32_t Replaying(uint8_t* base, uint32_t queue);
// QueueEntry::Sequence (sub_8258E5C0): the float at the entry's +8.
float Sequence(uint8_t* base, uint32_t entry);

// RenderQueue::ClearRecording (sub_8258ED78, slot 2), from its caller's
// stack pointer `sp`: the list the queue's slot 7 returns emptied
// (PassList::Clear), then the state's +84 = 0.0 (0x82000A34). Returns r3
// as the original leaves it (the list).
uint32_t ClearRecording(uint8_t* base, uint32_t queue, uint32_t sp, const GuestCall& call);

// RenderQueue::Init (sub_8258ED00, slot 0): the drawer's (state + 4) slot
// 0, PrimitiveDrawer::Init, with r4 = the device; state +48 + 4 x i =
// Device::GetRenderTarget(device, i) for i 0 to 3, +64 =
// GetDepthStencilSurface(device). Returns r3 (the depth surface).
uint32_t Init(uint8_t* base, uint32_t queue, uint32_t device, uint32_t sp, const GuestCall& call);

}  // namespace graphics::render_queue
