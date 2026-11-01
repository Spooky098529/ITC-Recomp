// The render queue's methods (docs/functions/device-shaders.md): see
// render_queue.h.

#include "graphics/render_queue.h"

#include "guest.h"

#include "base/log.h"
#include "graphics/device_shaders.h"
#include "graphics/resources.h"
#include "models/texture_provider.h"

#include <cstdint>

namespace graphics::render_queue {

namespace {

constexpr uint32_t kZero = 0x82000A34;          // 0.0
constexpr uint32_t kKeyMax = 0x82002814;        // 10,000.0
constexpr uint32_t kKeyMin = 0x82001C00;        // -10,000.0
constexpr uint32_t kSequenceStep = 0x82001FBC;  // 0.1

constexpr uint32_t kRecordingSlot = 28;  // slot 7
constexpr uint32_t kRecordingFunction = 0x8258E5F8;

uint32_t State(uint8_t* base, uint32_t queue) { return guest::LoadU32(base, queue + 4); }

// A frame of `bytes` below `sp`, with its back chain.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  guest::StoreU32(base, frame, sp);
  return frame;
}

// The queue's slot at `slot` (bytes into its vtable) with r3 = the queue:
// its plain C++ when it's the queue's own `function`, else a guest call.
uint32_t QueueSlot(uint8_t* base, uint32_t queue, uint32_t slot, uint32_t function,
                   uint32_t (*own)(uint8_t*, uint32_t), uint32_t frame, uint32_t return_address,
                   const GuestCall& call) {
  const uint32_t target = guest::LoadU32(base, guest::LoadU32(base, queue) + slot);
  if (target == function) {
    return own(base, queue);
  }
  return uint32_t(call(target, frame, return_address, 1, queue, 0, 0).r3);
}

}  // namespace

void Flip(uint8_t* base, uint32_t queue) {
  ITC_NATIVE_RUNNING("sub_8258E5E0 RenderQueue::Flip");
  const uint32_t state = State(base, queue);
  guest::StoreU32(base, state, guest::LoadU32(base, state) == 0 ? 1 : 0);
}

uint32_t Recording(uint8_t* base, uint32_t queue) {
  ITC_NATIVE_RUNNING("sub_8258E5F8 RenderQueue::Recording");
  const uint32_t index = guest::LoadU32(base, State(base, queue)) == 0 ? 1 : 0;
  return guest::LoadU32(base, queue + ((index + 2) << 2));
}

uint32_t Replaying(uint8_t* base, uint32_t queue) {
  ITC_NATIVE_RUNNING("sub_8258E618 RenderQueue::Replaying");
  const uint32_t index = guest::LoadU32(base, State(base, queue));
  return guest::LoadU32(base, queue + ((index + 2) << 2));
}

float Sequence(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_8258E5C0 QueueEntry::Sequence");
  return guest::LoadFloat(base, entry + 8);
}

uint32_t ClearRecording(uint8_t* base, uint32_t queue, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258ED78 RenderQueue::ClearRecording");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t list = QueueSlot(base, queue, kRecordingSlot, kRecordingFunction, Recording,
                                  frame, 0x8258ED9C, call);
  device_shaders::PassListClear(base, list);
  // lfs, then stfs: the float through a double, as the original.
  const double zero = guest::LoadFloat(base, kZero);
  guest::StoreFloat(base, State(base, queue) + 84, float(zero));
  return list;
}

uint32_t Init(uint8_t* base, uint32_t queue, uint32_t device, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258ED00 RenderQueue::Init");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t drawer = State(base, queue) + 4;
  call(guest::LoadU32(base, guest::LoadU32(base, drawer)), frame, 0x8258ED2C, 2, drawer, device,
       0);
  for (uint32_t i = 0, offset = 48; offset < 64; i++, offset += 4) {
    const uint32_t at = offset + State(base, queue);
    guest::StoreU32(base, at, GetRenderTarget(base, device, i));
  }
  const uint32_t state = State(base, queue);
  const uint32_t depth = GetDepthStencilSurface(base, device);
  guest::StoreU32(base, state + 64, depth);
  return depth;
}

uint32_t PassListAdd(uint8_t* base, uint32_t list, uint64_t chunk_key, uint64_t sequence,
                     uint32_t bucket, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8258EB88 PassList::Add");
  REX_STORE_U64(sp + 32, chunk_key);
  REX_STORE_U64(sp + 40, sequence);
  if (bucket >= guest::LoadU32(base, list + 12)) {
    return 0;
  }
  const uint32_t items = guest::LoadU32(base, list + 8);
  const uint32_t used = guest::LoadU32(base, list + 16);
  if (used >= items) {
    return 0;
  }
  const uint32_t buckets = guest::LoadU32(base, list + 24);
  const uint32_t item_block = guest::LoadU32(base, list + 20);
  // lfs, then stfs: each float through a double, as the original.
  const double key = guest::LoadFloat(base, sp + 36);
  const double seq = guest::LoadFloat(base, sp + 40);
  const uint32_t at = (bucket << 4) + buckets;
  const uint32_t item = ((used + (used << 2)) << 2) + item_block;
  const uint32_t chunk = guest::LoadU32(base, sp + 32);
  guest::StoreU32(base, item + 0, 0);
  guest::StoreFloat(base, item + 8, float(key));
  guest::StoreFloat(base, item + 12, float(seq));
  guest::StoreU32(base, item + 4, chunk);
  if (guest::LoadU32(base, at + 0) == 0) {
    guest::StoreU32(base, at + 0, item);
  } else {
    guest::StoreU32(base, guest::LoadU32(base, at + 4), item);
  }
  guest::StoreU32(base, at + 4, item);
  guest::StoreU32(base, at + 12, guest::LoadU32(base, at + 12) + 1);
  guest::StoreU32(base, list + 16, guest::LoadU32(base, list + 16) + 1);
  return 1;
}

std::optional<uint32_t> Add(uint8_t* base, uint32_t queue, uint32_t chunk, uint32_t bucket,
                            double key, uint32_t sp, const GuestCall& call) {
  if (bucket > 2056 || key > double(guest::LoadFloat(base, kKeyMax)) ||
      key < double(guest::LoadFloat(base, kKeyMin))) {
    return std::nullopt;  // the warning (DebugPrintf), never seen in play
  }
  ITC_NATIVE_RUNNING("sub_8258EDC8 RenderQueue::Add");
  if (chunk == 0) {
    return queue;
  }
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t state = State(base, queue);
  guest::StoreFloat(base, frame + 84, float(key));
  guest::StoreU32(base, frame + 80, chunk);
  const double sequence = guest::LoadFloat(base, state + 84);
  guest::StoreFloat(base, frame + 88, float(sequence));
  const uint32_t list = QueueSlot(base, queue, kRecordingSlot, kRecordingFunction, Recording,
                                  frame, 0x8258EE60, call);
  const uint32_t r3 = PassListAdd(base, list, REX_LOAD_U64(frame + 80),
                                  REX_LOAD_U64(frame + 88), bucket, frame);
  const uint32_t now = State(base, queue);
  const double before = guest::LoadFloat(base, now + 84);
  const double step = guest::LoadFloat(base, kSequenceStep);
  guest::StoreFloat(base, now + 84, float(double(float(before + step))));
  return r3;
}

namespace {

// A vector's size or capacity from two of its pointers: the byte difference
// shifted right as a signed word (srawi), as the original.
uint32_t Count(uint32_t from, uint32_t to) { return uint32_t(int32_t(to - from) >> 2); }

}  // namespace

std::optional<uint32_t> PtrVectorInsert(uint8_t* base, uint32_t answer, uint32_t vector,
                                        uint32_t where, uint32_t value) {
  const uint32_t first = guest::LoadU32(base, vector + 4);
  uint32_t index = 0;
  if (first != 0 && Count(first, guest::LoadU32(base, vector + 8)) != 0) {
    index = Count(first, where);
  }
  if (!models::PointersInsertN(base, vector, where, 1, guest::LoadU32(base, value))) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825911C8 PtrVector::Insert");
  guest::StoreU32(base, answer, (index << 2) + guest::LoadU32(base, vector + 4));
  return answer;
}

std::optional<uint32_t> PushBack(uint8_t* base, uint32_t vector, uint32_t value, uint32_t sp,
                                 bool may_allocate) {
  const uint32_t first = guest::LoadU32(base, vector + 4);
  const uint32_t size = first == 0 ? 0 : Count(first, guest::LoadU32(base, vector + 8));
  if (first != 0 && size < Count(first, guest::LoadU32(base, vector + 12))) {
    ITC_NATIVE_RUNNING("sub_82591540 PtrVector::PushBack");
    const uint32_t last = guest::LoadU32(base, vector + 8);
    guest::StoreU32(base, last, guest::LoadU32(base, value));
    guest::StoreU32(base, vector + 8, last + 4);
    return vector;
  }
  if (!may_allocate) {
    return std::nullopt;
  }
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t last = guest::LoadU32(base, vector + 8);
  guest::StoreU32(base, frame + 80, last);
  const auto r3 = PtrVectorInsert(base, frame + 80, vector, last, value);
  if (r3) {
    ITC_NATIVE_RUNNING("sub_82591540 PtrVector::PushBack");
  }
  return r3;
}

}  // namespace graphics::render_queue
