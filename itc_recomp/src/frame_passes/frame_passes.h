// The frame passes object as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/frame-passes.md, the fields in
// docs/known-data.md, "frame passes" and "work memory"): the 44-byte object
// the framework keeps at its +108 (vtable 0x82000F20), whose tick handlers
// record the frame's shared pass chunks every pass and flip the graphics
// library's work memory. They record into guest command lists that the
// renderer replays, so they work on guest memory with the originals' loads
// and stores. Their guest calls (the render queue's Add, the scene's and the
// interface's getters) are the adapter's (src/hooks/frame_passes.cpp). Each
// recorder declines (nothing done) where the original's command list calls
// would print a message (the list without room, a chunk open): the original
// runs then.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace frame_passes {

// The command list and render queue the primitive reset is recorded on.
constexpr uint32_t kCommandList = 0x8296B918;
constexpr uint32_t kRenderQueue = 0x8296B91C;

// sub_82589A78 WorkMem::Flip: the work memory's current buffer
// [0x8296B780] ^= 1, then the new one's (i) [0x8296B784 + 4i], headers used
// [0x8296B798 + 4i] and bytes used [0x8296B7A0 + 4i] = 0.
void WorkMemFlip(uint8_t* base);
// sub_82096218 FramePasses::FlipWorkMem (slot 2, tick handler 20): a tail
// call to WorkMem::Flip.
void FlipWorkMem(uint8_t* base);

// sub_82096350: a delegate's target, the object's vtable slot 2 (a tail
// call): the target.
uint32_t DelegateSlot2Target(uint8_t* base, uint32_t object);

// sub_82096360 FramePasses::RecordConstants (slot 5): on `list`, command 7
// with vertex constant c86 (0x00015601) from the 16 bytes at 0x8265ED20,
// command 7 with pixel constant c76 (0x01014C01) from 0x8265ED30. Returns
// the last slot (the original's r3); nullopt where the list hasn't room for
// the 4 slots.
std::optional<uint32_t> RecordConstants(uint8_t* base, uint32_t list);

// sub_82096030 FramePasses::RecordSamplers (slot 6): on `list`, command 2
// with samplers 0-3's address U and V = 2, command 2 with their min and mag
// filters = 0 and mip filter = 2, command 1 with render state 324 = 0 (24
// slots, the words the original doesn't write left as they were). Returns
// the last slot (the original's r3); nullopt where the list hasn't room.
std::optional<uint32_t> RecordSamplers(uint8_t* base, uint32_t list);

// RecordPrimitiveReset's frame (its r1 less this, below its caller's).
constexpr uint32_t kRecordPrimitiveResetFrame = 128;

// sub_82589990 FramePasses::RecordPrimitiveReset: on the list at
// [kCommandList] a chunk with command 1 (render state 328 = 1, 332 =
// 0xFFFF) and command 36. Returns the chunk, which the adapter hands to the
// render queue at [kRenderQueue] (its slot 4, bucket 0, f1 = 0.0); nullopt,
// with nothing done, where a chunk is open or the list hasn't room.
std::optional<uint32_t> RecordPrimitiveReset(uint8_t* base);

// sub_82094B10 Job::Wait: when the job's byte +18 is set (posted,
// probably), `wait(object)` with the object at its +20 (the original calls
// that object's slot 1 with r4 = the job: the pool's wait, probably), then
// +18 = 0. Whether it waited.
bool JobWait(uint8_t* base, uint32_t job, const std::function<void(uint32_t object)>& wait);

// Boot's frame (its r1 less this, below its caller's).
constexpr uint32_t kBootFrame = 112;

// FramePasses::Boot's guest calls, made by the adapter from its frame.
struct BootCalls {
  // The interface's slot 2 (the renderer), that one's slot 3 (the video
  // output), with the original's return addresses.
  std::function<uint32_t(uint32_t interface)> video_output;
  // PostEffects::InitBoot with r3 = `device`.
  std::function<void(uint32_t device)> init_boot;
};

// sub_820962B8 FramePasses::Boot (slot 0, the boot job): the +4
// interface's video output, its device locked (VideoOutput::LockDevice,
// into the frame's +80), PostEffects::InitBoot with the device, the device
// unlocked with the frame's word. Returns the video output (r3 as the
// native UnlockDevice leaves it).
uint32_t Boot(uint8_t* base, uint32_t passes, uint32_t frame, const BootCalls& calls);

}  // namespace frame_passes
