// The graphics device's shader setters and the pass lists' Clear
// (docs/functions/device-shaders.md): see device_shaders.h. Every write
// matches the original's; the shader state's doublewords are ORed whole, as
// the originals' ld/or/std.

#include "graphics/device_shaders.h"

#include "guest.h"
#include "schema/device_shaders.h"
#include "schema/device_state.h"
#include "schema/shader_objects.h"

#include "base/log.h"
#include "native/buffers.h"
#include "memory/c_runtime_heap.h"
#include "model_animation/model_animation.h"
#include "native/refcount.h"

#include <cstdint>

namespace graphics::device_shaders {

namespace {

using schema::Device;

constexpr uint64_t kVertexShaderBit = 1ull << 51;
constexpr uint64_t kPixelShaderBits[] = {1ull << 52, 1ull << 49};

constexpr uint32_t kPassListVtable = 0x8206CB88;
constexpr uint32_t kRenderQueueVtable = 0x8206CB8C;
using schema::PassList;
using schema::PassListBucket;
using schema::RenderQueue;
using schema::RenderQueueState;

// Where each shader's file header was copied: right after the object's fields.
constexpr uint32_t kVertexPatches = schema::VertexShader::kSize;  // 592
constexpr uint32_t kPixelPatches = schema::PixelShader::kSize;    // 56

void Or64(uint32_t address, uint64_t bits) {
  guest::StoreU64(address, guest::LoadU64(address) | bits);
}

// The patches at p (header + its +12): the constant mask ORed into the
// device's dirty mask at `mask` (an address), the render state's bit 1, then the records into the
// shadow. `r3` follows the original's: each memcpy leaves its destination.
// `at` is the memcpy call's return address in the original.
void ApplyPatches(const Device& d, uint32_t p, uint32_t mask, uint32_t at,
                  const ShaderCalls& calls, uint32_t& r3) {
  Or64(mask, guest::LoadU64(p + 0));
  if (guest::LoadU64(p + 8) != 0) {
    Or64(d.render_dirty_address(), 2);
  }
  uint32_t record = p + 24;
  const uint32_t end = guest::LoadU32(p + 16) + record;
  const uint32_t shadow = d.fetch_constants_address();  // the register shadow (0x4800 on)
  // The copy records, up to one with a count of 0.
  bool masked = false;
  while (record < end) {
    const uint16_t count = guest::LoadU16(record + 2);
    const uint16_t offset = guest::LoadU16(record + 0);
    record += 4;
    if (count == 0) {
      masked = true;
      break;
    }
    const uint32_t bytes = uint32_t(count) << 2;
    const uint32_t destination = offset + shadow;
    if (!native::Buffers_Memcpy(destination, record, bytes, at)) {
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
    uint16_t count = guest::LoadU16(record + 2);
    const uint16_t offset = guest::LoadU16(record + 0);
    record += 4;
    if (count == 0) {
      return;
    }
    uint32_t word = offset + shadow;
    do {
      const uint32_t value = (guest::LoadU32(record + 0) & guest::LoadU32(word)) |
                             guest::LoadU32(record + 4);
      record += 8;
      guest::StoreU32(word, value);
      word += 4;
      count = uint16_t(count - 2);
    } while (count != 0);
  }
}

}  // namespace

uint32_t SetVertexShader(uint32_t device, uint32_t shader,
                         const ShaderCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822A8808 Device::SetVertexShader");
  const Device d(device);
  uint32_t r3 = device;
  const uint32_t bound = d.vertex_shader();
  const bool same = shader != 0 && shader == bound;
  refcount::NoteSetVertexShader(same);
  if (same) {
    // The rebind skip: AddRef and Release would leave the count as it is;
    // Release's r3 would be that count.
    r3 = schema::VertexShader(shader).reference_count();
  } else {
    if (shader != 0) {
      r3 = refcount::AddRef(shader);
    }
    // Read again after AddRef, as the original does.
    const uint32_t old = d.vertex_shader();
    r3 = old;
    if (old != 0) {
      r3 = calls.release(old);
    }
  }
  const uint8_t flags = d.flags2();
  d.set_vertex_shader(shader);
  d.set_flags2(flags & 0x7F);
  Or64(d.shader_dirty_address(), kVertexShaderBit);
  if (shader == 0) {
    return r3;
  }
  const uint32_t header = shader + kVertexPatches;
  if (header == 0) {
    return r3;  // the original's addic. on the sum
  }
  const uint32_t offset = guest::LoadU32(header + 12);
  if (offset == 0) {
    return r3;
  }
  ApplyPatches(d, offset + header, d.vertex_float_dirty_address(), 0x822A88EC, calls, r3);
  return r3;
}

uint32_t SetVertexShaderEntry(uint32_t device, uint32_t shader,
                              const ShaderCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822A8958 D3DDevice_SetVertexShader");
  Device(device).set_entry_vertex_shader(shader);
  return SetVertexShader(device, shader, calls);
}

uint32_t SetPixelShader(uint32_t device, uint32_t shader,
                        const ShaderCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822A8618 Device::SetPixelShader");
  const Device d(device);
  uint32_t r3 = device;
  d.set_pixel_shader(shader);
  for (uint64_t bit : kPixelShaderBits) {
    Or64(d.shader_dirty_address(), bit);
  }
  if (shader == 0) {
    return r3;
  }
  const uint32_t header = shader + kPixelPatches;
  const uint32_t offset = guest::LoadU32(header + 12);
  if (offset == 0) {
    return r3;
  }
  ApplyPatches(d, offset + header, d.pixel_float_dirty_address(), 0x822A86D4, calls, r3);
  return r3;
}

void SetVertexDeclaration(uint32_t device, uint32_t declaration) {
  ITC_NATIVE_RUNNING("sub_822A8960 Device::SetVertexDeclaration");
  const Device d(device);
  d.set_vertex_declaration(declaration);
  Or64(d.shader_dirty_address(), kVertexShaderBit);
}

void PassListClear(uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8258EB38 PassList::Clear");
  const PassList l(list);
  for (uint32_t i = 0, offset = 0; i < l.bucket_count(); i++, offset += 16) {
    PassListBucket(l.buckets().address + offset).set_first(0);
    PassListBucket(l.buckets().address + offset).set_last(0);
  }
  l.set_items_used(0);
  l.set_cleared_28(0);
  l.set_current_item(0);
}

bool PassListConstruct(uint32_t list, uint32_t items, uint32_t buckets) {
  ITC_NATIVE_RUNNING("sub_82590B38 PassList::PassList");
  const PassList l(list);
  l.set_item_count(items);
  l.set_vtable(kPassListVtable);
  l.set_items_used(0);
  l.set_bucket_done(1);
  l.set_bucket_count(buckets);
  const auto item_block =
      memory::crt::OperatorNewArray(items > 0x0CCCCCCCu ? 0xFFFFFFFFu : items * 20);
  if (!item_block) {
    return false;
  }
  l.set_items(*item_block);
  const uint32_t count = l.bucket_count();
  const auto bucket_block =
      memory::crt::OperatorNewArray(count > 0x0FFFFFFFu ? 0xFFFFFFFFu : count << 4);
  if (!bucket_block) {
    return false;
  }
  l.set_buckets(*bucket_block);
  PassListClear(list);
  if (l.bucket_count() - 1 != 0) {
    uint32_t i = 0, offset = 0;
    do {
      const PassListBucket bucket(l.buckets().address + offset);
      i++;
      bucket.set_next(bucket.address + 16);
      PassListBucket(l.buckets().address + offset).set_count(0);
      offset += 16;
    } while (i < l.bucket_count() - 1);
  }
  // The last bucket's +8 and +12 (the bucket before the end's).
  PassListBucket((l.bucket_count() << 4) + l.buckets().address - 16).set_next(0);
  PassListBucket((l.bucket_count() << 4) + l.buckets().address - 16).set_count(0);
  return true;
}

bool RenderQueueConstruct(uint32_t queue) {
  ITC_NATIVE_RUNNING("sub_825915E0 RenderQueue::RenderQueue");
  const RenderQueue q(queue);
  q.set_vtable(kRenderQueueVtable);
  const auto state_block = memory::crt::OperatorNew(88);
  if (!state_block) {
    return false;
  }
  const RenderQueueState state(*state_block);
  if (state) {
    if (!model_animation::PrimitiveDrawerConstruct(state.drawer_address())) {
      return false;
    }
    state.set_release_first(0);
    state.set_release_last(0);
    state.set_release_end(0);
  }
  q.set_state(state);
  for (uint32_t field : {q.list_1_address(), q.list_0_address()}) {
    const auto block = memory::crt::OperatorNew(36);
    if (!block) {
      return false;
    }
    if (*block != 0 && !PassListConstruct(*block, 4096, 1024)) {
      return false;
    }
    guest::StoreU32(field, *block);  // list_1, then list_0
  }
  PassListClear(q.list_1().address);
  PassListClear(q.list_0().address);
  q.state().set_replay_index(0);
  // lfs, then stfs: the float through a double, as the original.
  const double zero = schema::DeviceShadersGlobals::zero();
  q.state().set_sequence(float(zero));
  q.state().set_render_target_0(0);
  q.state().set_render_target_1(0);
  q.state().set_render_target_2(0);
  q.state().set_render_target_3(0);
  q.state().set_depth_surface(0);
  return true;
}

}  // namespace graphics::device_shaders
