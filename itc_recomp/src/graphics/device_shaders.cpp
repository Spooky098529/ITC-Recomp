// The graphics device's shader setters and the pass lists' Clear
// (docs/functions/device-shaders.md): see device_shaders.h. Every write
// matches the original's; the shader state's doublewords are ORed whole, as
// the originals' ld/or/std.

#include "graphics/device_shaders.h"

#include "guest.h"

#include "base/log.h"
#include "native/buffers.h"
#include "memory/c_runtime_heap.h"
#include "model_animation/model_animation.h"
#include "native/refcount.h"

#include <cstdint>

namespace graphics::device_shaders {

namespace {

constexpr uint32_t kVertexConstantMask = 0;   // the vertex float constants' dirty mask
constexpr uint32_t kPixelConstantMask = 8;    // the pixel float constants'
constexpr uint32_t kShaderStateMask = 16;     // the shader state's dirty mask
constexpr uint32_t kRenderStateMask = 24;     // the render state's
constexpr uint32_t kShadow = 1024;            // the register shadow
constexpr uint32_t kFlags10810 = 10810;       // a byte; SetVertexShader clears its top bit
constexpr uint32_t kVertexDeclaration = 11552;
constexpr uint32_t kPixelShader = 12432;
constexpr uint32_t kVertexShader = 12436;
constexpr uint32_t kTitleVertexShader = 13156;  // the entry's copy (read by nothing)

constexpr uint64_t kVertexShaderBit = 1ull << 51;
constexpr uint64_t kPixelShaderBits[] = {1ull << 52, 1ull << 49};

constexpr uint32_t kPassListVtable = 0x8206CB88;
constexpr uint32_t kRenderQueueVtable = 0x8206CB8C;
constexpr uint32_t kZero = 0x82000A34;  // 0.0

constexpr uint32_t kVertexPatches = 592;  // where the vertex shader's file header was copied
constexpr uint32_t kPixelPatches = 56;    // the pixel shader's

void Or64(uint8_t* base, uint32_t address, uint64_t bits) {
  REX_STORE_U64(address, REX_LOAD_U64(address) | bits);
}

// The patches at p (header + its +12): the constant mask ORed into the
// device's `mask`, the render state's bit 1, then the records into the
// shadow. `r3` follows the original's: each memcpy leaves its destination.
// `at` is the memcpy call's return address in the original.
void ApplyPatches(uint8_t* base, uint32_t device, uint32_t p, uint32_t mask, uint32_t at,
                  const ShaderCalls& calls, uint32_t& r3) {
  Or64(base, device + mask, REX_LOAD_U64(p + 0));
  if (REX_LOAD_U64(p + 8) != 0) {
    Or64(base, device + kRenderStateMask, 2);
  }
  uint32_t record = p + 24;
  const uint32_t end = guest::LoadU32(base, p + 16) + record;
  const uint32_t shadow = device + kShadow;
  // The copy records, up to one with a count of 0.
  bool masked = false;
  while (record < end) {
    const uint16_t count = guest::LoadU16(base, record + 2);
    const uint16_t offset = guest::LoadU16(base, record + 0);
    record += 4;
    if (count == 0) {
      masked = true;
      break;
    }
    const uint32_t bytes = uint32_t(count) << 2;
    const uint32_t destination = offset + shadow;
    if (!native::Buffers_Memcpy(base, destination, record, bytes, at)) {
      calls.memcpy(destination, record, bytes, at);
    }
    r3 = destination;
    record += bytes;
  }
  if (!masked) {
    return;
  }
  // The masked records, up to another count of 0 or the end: a pair of
  // words a shadow word, the count going down by 2 (16 bits) until it's 0.
  while (record < end) {
    uint16_t count = guest::LoadU16(base, record + 2);
    const uint16_t offset = guest::LoadU16(base, record + 0);
    record += 4;
    if (count == 0) {
      return;
    }
    uint32_t word = offset + shadow;
    do {
      const uint32_t value = (guest::LoadU32(base, record + 0) & guest::LoadU32(base, word)) |
                             guest::LoadU32(base, record + 4);
      record += 8;
      guest::StoreU32(base, word, value);
      word += 4;
      count = uint16_t(count - 2);
    } while (count != 0);
  }
}

}  // namespace

uint32_t SetVertexShader(uint8_t* base, uint32_t device, uint32_t shader,
                         const ShaderCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822A8808 Device::SetVertexShader");
  uint32_t r3 = device;
  const uint32_t bound = guest::LoadU32(base, device + kVertexShader);
  const bool same = shader != 0 && shader == bound;
  refcount::NoteSetVertexShader(same);
  if (same) {
    // The rebind skip: AddRef and Release would leave the count as it is;
    // Release's r3 would be that count.
    r3 = guest::LoadU32(base, shader + 4);
  } else {
    if (shader != 0) {
      r3 = refcount::AddRef(base, shader);
    }
    // Read again after AddRef, as the original does.
    const uint32_t old = guest::LoadU32(base, device + kVertexShader);
    r3 = old;
    if (old != 0) {
      r3 = calls.release(old);
    }
  }
  const uint8_t flags = guest::LoadU8(base, device + kFlags10810);
  guest::StoreU32(base, device + kVertexShader, shader);
  guest::StoreU8(base, device + kFlags10810, flags & 0x7F);
  Or64(base, device + kShaderStateMask, kVertexShaderBit);
  if (shader == 0) {
    return r3;
  }
  const uint32_t header = shader + kVertexPatches;
  if (header == 0) {
    return r3;  // the original's addic. on the sum
  }
  const uint32_t offset = guest::LoadU32(base, header + 12);
  if (offset == 0) {
    return r3;
  }
  ApplyPatches(base, device, offset + header, kVertexConstantMask, 0x822A88EC, calls, r3);
  return r3;
}

uint32_t SetVertexShaderEntry(uint8_t* base, uint32_t device, uint32_t shader,
                              const ShaderCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822A8958 D3DDevice_SetVertexShader");
  guest::StoreU32(base, device + kTitleVertexShader, shader);
  return SetVertexShader(base, device, shader, calls);
}

uint32_t SetPixelShader(uint8_t* base, uint32_t device, uint32_t shader,
                        const ShaderCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822A8618 Device::SetPixelShader");
  uint32_t r3 = device;
  guest::StoreU32(base, device + kPixelShader, shader);
  for (uint64_t bit : kPixelShaderBits) {
    Or64(base, device + kShaderStateMask, bit);
  }
  if (shader == 0) {
    return r3;
  }
  const uint32_t header = shader + kPixelPatches;
  const uint32_t offset = guest::LoadU32(base, header + 12);
  if (offset == 0) {
    return r3;
  }
  ApplyPatches(base, device, offset + header, kPixelConstantMask, 0x822A86D4, calls, r3);
  return r3;
}

void SetVertexDeclaration(uint8_t* base, uint32_t device, uint32_t declaration) {
  ITC_NATIVE_RUNNING("sub_822A8960 Device::SetVertexDeclaration");
  guest::StoreU32(base, device + kVertexDeclaration, declaration);
  Or64(base, device + kShaderStateMask, kVertexShaderBit);
}

void PassListClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8258EB38 PassList::Clear");
  for (uint32_t i = 0, offset = 0; i < guest::LoadU32(base, list + 12); i++, offset += 16) {
    guest::StoreU32(base, guest::LoadU32(base, list + 24) + offset, 0);
    guest::StoreU32(base, guest::LoadU32(base, list + 24) + offset + 4, 0);
  }
  guest::StoreU32(base, list + 16, 0);
  guest::StoreU32(base, list + 28, 0);
  guest::StoreU32(base, list + 32, 0);
}

bool PassListConstruct(uint8_t* base, uint32_t list, uint32_t items, uint32_t buckets) {
  ITC_NATIVE_RUNNING("sub_82590B38 PassList::PassList");
  guest::StoreU32(base, list + 8, items);
  guest::StoreU32(base, list + 0, kPassListVtable);
  guest::StoreU32(base, list + 16, 0);
  guest::StoreU8(base, list + 4, 1);
  guest::StoreU32(base, list + 12, buckets);
  const auto item_block =
      memory::crt::OperatorNewArray(base, items > 0x0CCCCCCCu ? 0xFFFFFFFFu : items * 20);
  if (!item_block) {
    return false;
  }
  guest::StoreU32(base, list + 20, *item_block);
  const uint32_t count = guest::LoadU32(base, list + 12);
  const auto bucket_block =
      memory::crt::OperatorNewArray(base, count > 0x0FFFFFFFu ? 0xFFFFFFFFu : count << 4);
  if (!bucket_block) {
    return false;
  }
  guest::StoreU32(base, list + 24, *bucket_block);
  PassListClear(base, list);
  if (guest::LoadU32(base, list + 12) - 1 != 0) {
    uint32_t i = 0, offset = 0;
    do {
      const uint32_t bucket = guest::LoadU32(base, list + 24) + offset;
      i++;
      guest::StoreU32(base, bucket + 8, bucket + 16);
      guest::StoreU32(base, guest::LoadU32(base, list + 24) + offset + 12, 0);
      offset += 16;
    } while (i < guest::LoadU32(base, list + 12) - 1);
  }
  guest::StoreU32(base, (guest::LoadU32(base, list + 12) << 4) + guest::LoadU32(base, list + 24) - 8,
                  0);
  guest::StoreU32(base, (guest::LoadU32(base, list + 12) << 4) + guest::LoadU32(base, list + 24) - 4,
                  0);
  return true;
}

bool RenderQueueConstruct(uint8_t* base, uint32_t queue) {
  ITC_NATIVE_RUNNING("sub_825915E0 RenderQueue::RenderQueue");
  guest::StoreU32(base, queue + 0, kRenderQueueVtable);
  const auto state_block = memory::crt::OperatorNew(base, 88);
  if (!state_block) {
    return false;
  }
  const uint32_t state = *state_block;
  if (state != 0) {
    if (!model_animation::PrimitiveDrawerConstruct(base, state + 4)) {
      return false;
    }
    guest::StoreU32(base, state + 72, 0);
    guest::StoreU32(base, state + 76, 0);
    guest::StoreU32(base, state + 80, 0);
  }
  guest::StoreU32(base, queue + 4, state);
  for (uint32_t field : {8u, 12u}) {
    const auto block = memory::crt::OperatorNew(base, 36);
    if (!block) {
      return false;
    }
    if (*block != 0 && !PassListConstruct(base, *block, 4096, 1024)) {
      return false;
    }
    guest::StoreU32(base, queue + field, *block);
  }
  PassListClear(base, guest::LoadU32(base, queue + 8));
  PassListClear(base, guest::LoadU32(base, queue + 12));
  guest::StoreU32(base, guest::LoadU32(base, queue + 4) + 0, 0);
  // lfs, then stfs: the float through a double, as the original.
  const double zero = guest::LoadFloat(base, kZero);
  guest::StoreFloat(base, guest::LoadU32(base, queue + 4) + 84, float(zero));
  for (uint32_t field : {48u, 52u, 56u, 60u, 64u}) {
    guest::StoreU32(base, guest::LoadU32(base, queue + 4) + field, 0);
  }
  return true;
}

}  // namespace graphics::device_shaders
