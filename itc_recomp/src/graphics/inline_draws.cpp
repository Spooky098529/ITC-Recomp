// The graphics library's inline and non-indexed draws: see inline_draws.h.
// Every kept read and write is the originals', in their order; the masks
// and shifts are their rldicr and rlwinm on the dirty words.

#include "graphics/inline_draws.h"

#include "guest.h"

#include "base/log.h"
#include "hooks/lifted_call.h"
#include "native/state.h"

namespace graphics::inline_draws {

namespace {

constexpr uint32_t kLoadShaders = 0x822AED18;  // Device::LoadShaders (Lifted)
constexpr uint32_t kEndSegment = 0x8229EF48;   // Device::EndSegment (its override)
constexpr uint32_t kReserve = 0x8229E308;      // Device::AllocateFromCommandBuffer
constexpr uint32_t kTypeCounts = 0x82000950;   // a primitive type's multiplier and addend

// The device's fields.
constexpr uint32_t kVertexConstantsDirty = 0;
constexpr uint32_t kPixelConstantsDirty = 8;
constexpr uint32_t kShaderStateDirty = 16;
constexpr uint32_t kRenderStateDirty = 24;
constexpr uint32_t kOtherStateDirty = 32;
constexpr uint32_t kWritePosition = 40;
constexpr uint32_t kSoftEnd = 48;
constexpr uint32_t kHizState = 11584;
constexpr uint32_t kShaderStride = 11568;   // byte: the bound stride in words
constexpr uint32_t kInlineStride = 12272;   // byte: the inline draw's stride in words
constexpr uint32_t kInlinePosition = 13092;  // where End puts the write position
constexpr uint32_t kInlineVertices = 13096;
constexpr uint32_t kInlineIndices = 13100;
constexpr uint32_t kInlineVertexWords = 13104;
constexpr uint32_t kInlineIndexWords = 13108;

uint64_t Load64(uint32_t address) {
  return (uint64_t(guest::LoadU32(address)) << 32) | guest::LoadU32(address + 4);
}
void Store64(uint32_t address, uint64_t value) {
  guest::StoreU32(address, uint32_t(value >> 32));
  guest::StoreU32(address + 4, uint32_t(value));
}

// The dirty state's flush every draw begins with: the masks cleared, the
// shaders loaded and the hi-Z state reset. The packet writers it calls are
// Dropped (sub_822AE1D0, sub_822ADE30, sub_822ADEF0, sub_822AE070,
// sub_822ADC08); the renderer reads the device's shadow at the draw
// (src/native/draw_registers.h), so nothing is marked for it.
void Flush(uint32_t frame, uint64_t r13, uint32_t device) {
  // +0 and +8: the constants' packets (sub_822AE1D0).
  Store64(device + kVertexConstantsDirty, 0);
  Store64(device + kPixelConstantsDirty, 0);
  if (const uint64_t shader = Load64(device + kShaderStateDirty); shader != 0) {
    if (shader & (0xFull << 49)) {
      lifted_call::Call(kLoadShaders, frame, r13, {device, shader});
    }
    Store64(device + kShaderStateDirty, 0);
  }
  if (const uint64_t render = Load64(device + kRenderStateDirty); render != 0) {
    if (render & 0x2) {
      Store64(device + kHizState, 0xFFFFFFFFFF000000ull);
    }
    Store64(device + kRenderStateDirty, 0);
  }
  // sub_822ADEF0's register blocks and the rest of the other state: packets.
  Store64(device + kOtherStateDirty, 0);
}

// The inline stride set for the flush (the shader load patches the vertex
// fetch with it), the shader state marked dirty when it differs from the
// bound one; returns the stride byte to put back after.
uint8_t SetInlineStride(uint32_t device, uint32_t stride) {
  const uint8_t words = uint8_t(stride >> 2);
  const uint8_t bound = guest::LoadU8(device + kShaderStride);
  const uint8_t saved = guest::LoadU8(device + kInlineStride);
  guest::StoreU8(device + kInlineStride, words);
  if (words != bound) {
    Store64(device + kShaderStateDirty,
            Load64(device + kShaderStateDirty) | (1ull << 51));
  }
  return saved;
}

// The write position, after ending the segment when it has passed the soft
// end (the original's check before its packets).
uint32_t Position(uint32_t frame, uint64_t r13, uint32_t device) {
  const uint32_t position = guest::LoadU32(device + kWritePosition);
  if (position > guest::LoadU32(device + kSoftEnd)) {
    return uint32_t(lifted_call::Call(kEndSegment, frame, r13, {device}).r3);
  }
  return position;
}

uint32_t Reserve(uint32_t frame, uint64_t r13, uint32_t device, uint32_t words) {
  return uint32_t(lifted_call::Call(kReserve, frame, r13, {device, words, 16}).r3);
}

}  // namespace

uint32_t BeginVertices(uint32_t sp, uint64_t r13, uint32_t device, uint32_t count,
                       uint32_t stride) {
  ITC_NATIVE_RUNNING("sub_825BD8F0 BeginVertices");
  const uint32_t frame = lifted_call::PushFrame(sp, 176);
  const uint32_t words = (count * stride) >> 2;
  const uint8_t saved = SetInlineStride(device, stride);
  Flush(frame, r13, device);
  const uint32_t soft_end = guest::LoadU32(device + kSoftEnd);
  uint32_t position = guest::LoadU32(device + kWritePosition);
  guest::StoreU8(device + kInlineStride, saved);
  if (position > soft_end) {
    position = uint32_t(lifted_call::Call(kEndSegment, frame, r13, {device}).r3);
  }
  const uint32_t data = Reserve(frame, r13, device, words);
  if (data == 0) {
    guest::StoreU32(device + kWritePosition, position);
    return 0;
  }
  // The vertex fetch constant and draw packets (or, under predicated
  // tiling, the draw chain's record): Dropped.
  Store64(device + kShaderStateDirty, Load64(device + kShaderStateDirty) | 4096);
  guest::StoreU32(device + kInlinePosition, position);
  guest::StoreU32(device + kInlineVertices, data);
  guest::StoreU32(device + kInlineVertexWords, words);
  return data;
}

void EndVertices(uint32_t device) {
  ITC_NATIVE_RUNNING("sub_825BDDA0 EndVertices");
  guest::StoreU32(device + kWritePosition, guest::LoadU32(device + kInlinePosition));
}

uint32_t BeginIndexedVertices(uint32_t sp, uint64_t r13, uint32_t device,
                              uint32_t vertex_count, uint32_t index_count, uint32_t format,
                              uint32_t stride, uint32_t index_out) {
  ITC_NATIVE_RUNNING("sub_825BDDC0 BeginIndexedVertices");
  const uint32_t frame = lifted_call::PushFrame(sp, 240);
  const uint32_t vertex_words = (vertex_count * stride) >> 2;
  const uint32_t index_words = (format & 0x4) ? index_count : (index_count + 1) >> 1;
  const uint8_t saved = SetInlineStride(device, stride);
  Flush(frame, r13, device);
  const uint32_t soft_end = guest::LoadU32(device + kSoftEnd);
  uint32_t position = guest::LoadU32(device + kWritePosition);
  guest::StoreU8(device + kInlineStride, saved);
  if (position > soft_end) {
    position = uint32_t(lifted_call::Call(kEndSegment, frame, r13, {device}).r3);
  }
  const uint32_t vertices = Reserve(frame, r13, device, vertex_words);
  const uint32_t indices = Reserve(frame, r13, device, index_words);
  const uint32_t vertex_out = guest::LoadU32(sp + 84);
  if (vertices == 0 || indices == 0) {
    guest::StoreU32(device + kWritePosition, position);
    guest::StoreU32(index_out, 0);
    guest::StoreU32(vertex_out, 0);
    return 0x8007000E;
  }
  // The index and vertex fetch packets and DRAW_INDX (or the draw chain's
  // record): Dropped.
  Store64(device + kShaderStateDirty, Load64(device + kShaderStateDirty) | 4096);
  guest::StoreU32(device + kInlinePosition, position);
  guest::StoreU32(device + kInlineVertices, vertices);
  guest::StoreU32(device + kInlineIndices, indices);
  guest::StoreU32(device + kInlineVertexWords, vertex_words);
  guest::StoreU32(device + kInlineIndexWords, index_words);
  guest::StoreU32(vertex_out, vertices);
  guest::StoreU32(index_out, indices);
  return 0;
}

void DrawVertices(uint32_t sp, uint64_t r13, uint32_t device, uint32_t type,
                  uint32_t start, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_825BE300 DrawVertices");
  const uint32_t frame = lifted_call::PushFrame(sp, 208);
  Flush(frame, r13, device);
  const uint32_t multiplier = guest::LoadU32(kTypeCounts + 8 * type);
  const uint32_t addend = guest::LoadU32(kTypeCounts + 8 * type + 4);
  for (;;) {
    const uint32_t position = Position(frame, r13, device);
    uint32_t vertices = count;
    if (count > 65535) {
      vertices = ((65535 / multiplier) & ~1u) * multiplier;
    }
    // The draw's packets (or the draw chain's record): Dropped.
    guest::StoreU32(device + kWritePosition, position);
    const uint32_t left = count - vertices;
    if (left == 0) {
      return;
    }
    count = addend + left;
    start += vertices - addend;
  }
}

}  // namespace graphics::inline_draws
