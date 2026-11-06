// The command buffer's adapter steps other overrides and the harness share
// (src/hooks/command_buffer.cpp, docs/functions/command-buffer.md).

#pragma once

#include "guest.h"

#include "graphics/command_buffer.h"

namespace hooks {

// The segment flow's kick (graphics::command_buffer::Kick): Device::
// DeferredKick (sub_8229E848) as a guest call through `ctx`, from a frame
// like sub_8229EDA8's.
graphics::command_buffer::Kick CommandBufferKick(PPCContext& ctx, uint8_t* base);

// The segment's end (sub_8229EF48), r3 the device: the plain C++ with
// --native_command_buffer (the original with it off, or with a recording
// command buffer). For its Wrapped override in src/native/draw_packets.cpp,
// which counts the calls.
void EndCommandSegment(PPCContext& ctx, uint8_t* base);

// The next segment (sub_8229EC00), r3 the device, r4 the words: the plain
// C++ with --native_command_buffer (the original with it off, or with a
// recording command buffer); r3 the segment. For Device::Swap's override
// (src/hooks/graphics_device.cpp).
void NextCommandSegment(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
