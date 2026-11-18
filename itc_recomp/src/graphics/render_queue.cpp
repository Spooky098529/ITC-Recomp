// The render queue's methods (docs/functions/device-shaders.md): see
// render_queue.h.

#include "graphics/render_queue.h"

#include "guest.h"
#include "schema/device_shaders.h"

#include "base/log.h"
#include "graphics/device_shaders.h"
#include "graphics/resources.h"
#include "models/texture_provider.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstdint>

namespace graphics::render_queue {

namespace {

using G = schema::DeviceShadersGlobals;
using schema::ChunkCommand;
using schema::ChunkItem;
using schema::PassList;
using schema::PassListBucket;
using schema::PassListItem;
using schema::PtrVector;
using schema::PtrVectorSlot;
using schema::RenderQueueEntry;
using schema::RenderQueueState;

constexpr uint32_t kRecordingSlot = 28;  // slot 7
constexpr uint32_t kRecordingFunction = 0x8258E5F8;

RenderQueueState State(uint8_t* base, uint32_t queue) {
  return schema::RenderQueue(base, queue).state();
}

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
  const uint32_t target =
      guest::LoadU32(base, schema::RenderQueue(base, queue).vtable().address + slot);
  if (target == function) {
    return own(base, queue);
  }
  return uint32_t(call(target, frame, return_address, 1, queue, 0, 0).r3);
}

}  // namespace

void Flip(uint8_t* base, uint32_t queue) {
  ITC_NATIVE_RUNNING("sub_8258E5E0 RenderQueue::Flip");
  const RenderQueueState state = State(base, queue);
  state.set_replay_index(state.replay_index() == 0 ? 1 : 0);
}

uint32_t Recording(uint8_t* base, uint32_t queue) {
  ITC_NATIVE_RUNNING("sub_8258E5F8 RenderQueue::Recording");
  const uint32_t index = State(base, queue).replay_index() == 0 ? 1 : 0;
  const schema::RenderQueue q(base, queue);
  return (index != 0 ? q.list_0() : q.list_1()).address;
}

uint32_t Replaying(uint8_t* base, uint32_t queue) {
  ITC_NATIVE_RUNNING("sub_8258E618 RenderQueue::Replaying");
  const uint32_t index = State(base, queue).replay_index();
  const schema::RenderQueue q(base, queue);
  return (index != 0 ? q.list_0() : q.list_1()).address;
}

float Sequence(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_8258E5C0 QueueEntry::Sequence");
  return RenderQueueEntry(base, entry).sequence();
}

uint32_t ClearRecording(uint8_t* base, uint32_t queue, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258ED78 RenderQueue::ClearRecording");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t list = QueueSlot(base, queue, kRecordingSlot, kRecordingFunction, Recording,
                                  frame, 0x8258ED9C, call);
  device_shaders::PassListClear(base, list);
  // lfs, then stfs: the float through a double, as the original.
  const double zero = G::zero(base);
  State(base, queue).set_sequence(float(zero));
  return list;
}

uint32_t Init(uint8_t* base, uint32_t queue, uint32_t device, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258ED00 RenderQueue::Init");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t drawer = State(base, queue).drawer_address();
  call(guest::LoadU32(base, guest::LoadU32(base, drawer)), frame, 0x8258ED2C, 2, drawer, device,
       0);
  for (uint32_t i = 0, offset = 0; offset < 16; i++, offset += 4) {
    const uint32_t at = offset + State(base, queue).render_target_0_address();
    guest::StoreU32(base, at, GetRenderTarget(base, device, i));
  }
  const RenderQueueState state = State(base, queue);
  const uint32_t depth = GetDepthStencilSurface(base, device);
  state.set_depth_surface(depth);
  return depth;
}

uint32_t PassListAdd(uint8_t* base, uint32_t list, uint64_t chunk_key, uint64_t sequence,
                     uint32_t bucket, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8258EB88 PassList::Add");
  guest::StoreU64(base, sp + 32, chunk_key);
  guest::StoreU64(base, sp + 40, sequence);
  const PassList l(base, list);
  if (bucket >= l.bucket_count()) {
    return 0;
  }
  const uint32_t items = l.item_count();
  const uint32_t used = l.items_used();
  if (used >= items) {
    return 0;
  }
  const uint32_t buckets = l.buckets().address;
  const uint32_t item_block = l.items().address;
  // lfs, then stfs: each float through a double, as the original.
  const double key = guest::LoadFloat(base, sp + 36);
  const double seq = guest::LoadFloat(base, sp + 40);
  const PassListBucket at(base, (bucket << 4) + buckets);
  const PassListItem item(base, ((used + (used << 2)) << 2) + item_block);
  const uint32_t chunk = guest::LoadU32(base, sp + 32);
  item.set_next(0);
  item.set_key(float(key));
  item.set_sequence(float(seq));
  item.set_chunk(chunk);
  if (!at.first()) {
    at.set_first(item);
  } else {
    at.last().set_next(item);
  }
  at.set_last(item);
  at.set_count(at.count() + 1);
  l.set_items_used(l.items_used() + 1);
  return 1;
}

std::optional<uint32_t> Add(uint8_t* base, uint32_t queue, uint32_t chunk, uint32_t bucket,
                            double key, uint32_t sp, const GuestCall& call) {
  if (bucket > 2056 || key > double(G::key_max(base)) || key < double(G::key_min(base))) {
    return std::nullopt;  // the warning (DebugPrintf), never seen in play
  }
  ITC_NATIVE_RUNNING("sub_8258EDC8 RenderQueue::Add");
  if (chunk == 0) {
    return queue;
  }
  const uint32_t frame = Frame(base, sp, 144);
  const RenderQueueState state = State(base, queue);
  guest::StoreFloat(base, frame + 84, float(key));
  guest::StoreU32(base, frame + 80, chunk);
  const double sequence = state.sequence();
  guest::StoreFloat(base, frame + 88, float(sequence));
  const uint32_t list = QueueSlot(base, queue, kRecordingSlot, kRecordingFunction, Recording,
                                  frame, 0x8258EE60, call);
  const uint32_t r3 = PassListAdd(base, list, guest::LoadU64(base, frame + 80),
                                  guest::LoadU64(base, frame + 88), bucket, frame);
  const RenderQueueState now = State(base, queue);
  const double before = now.sequence();
  const double step = G::sequence_step(base);
  now.set_sequence(float(double(float(before + step))));
  return r3;
}

namespace {

// A vector's size or capacity from two of its pointers: the byte difference
// shifted right as a signed word (srawi), as the original.
uint32_t Count(uint32_t from, uint32_t to) { return uint32_t(int32_t(to - from) >> 2); }

}  // namespace

std::optional<uint32_t> PtrVectorInsert(uint8_t* base, uint32_t answer, uint32_t vector,
                                        uint32_t where, uint32_t value) {
  const PtrVector v(base, vector);
  const uint32_t first = v.first().address;
  uint32_t index = 0;
  if (first != 0 && Count(first, v.last().address) != 0) {
    index = Count(first, where);
  }
  if (!models::PointersInsertN(base, vector, where, 1, PtrVectorSlot(base, value).value())) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825911C8 PtrVector::Insert");
  guest::StoreU32(base, answer, (index << 2) + v.first().address);
  return answer;
}

std::optional<uint32_t> PushBack(uint8_t* base, uint32_t vector, uint32_t value, uint32_t sp,
                                 bool may_allocate) {
  const PtrVector v(base, vector);
  const uint32_t first = v.first().address;
  const uint32_t size = first == 0 ? 0 : Count(first, v.last().address);
  if (first != 0 && size < Count(first, v.end().address)) {
    ITC_NATIVE_RUNNING("sub_82591540 PtrVector::PushBack");
    const PtrVectorSlot last = v.last();
    last.set_value(PtrVectorSlot(base, value).value());
    v.set_last(last.address + 4);
    return vector;
  }
  if (!may_allocate) {
    return std::nullopt;
  }
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t last = v.last().address;
  guest::StoreU32(base, frame + 80, last);
  const auto r3 = PtrVectorInsert(base, frame + 80, vector, last, value);
  if (r3) {
    ITC_NATIVE_RUNNING("sub_82591540 PtrVector::PushBack");
  }
  return r3;
}

namespace {

constexpr uint32_t kReplayingSlot = 32;  // slot 8
constexpr uint32_t kReplayingFunction = 0x8258E618;

constexpr uint32_t kMassGetter = 0x820B6708;      // RigidBody::Mass: the float at +4
constexpr uint32_t kSequenceGetter = 0x8258E5C0;  // QueueEntry::Sequence: at +8

constexpr uint32_t kSingleCommands = G::single_commands_address();
constexpr uint32_t kCommands = G::commands_address();
constexpr uint32_t kEndCommand = 36;

constexpr uint32_t kEntries = G::replay_entries_address();  // room for 5,000 before the next

// A sort key: the getter's f1.
double Key(uint8_t* base, uint32_t getter, uint32_t element, uint32_t frame,
           uint32_t return_address, const GuestCall& call) {
  if (getter == kMassGetter) {
    return double(vehicle_accessors::RigidBodyMass(base, element));
  }
  if (getter == kSequenceGetter) {
    return double(Sequence(base, element));
  }
  return call(getter, frame, return_address, 1, element, 0, 0).f1;
}

uint32_t Element(uint8_t* base, uint32_t array, int32_t index) {
  return guest::LoadU32(base, (uint32_t(index) << 2) + array);
}

// The commands from `command` through the table 0x8265EDE0, until id 36.
void RunCommands(uint8_t* base, uint32_t command, uint32_t frame, uint32_t return_address,
                 const GuestCall& call) {
  uint32_t id = ChunkCommand(base, command).id();
  while (int32_t(id) != int32_t(kEndCommand)) {
    const ChunkCommand c(base, command);
    const uint32_t r5 = c.arg1();
    const uint32_t r4 = c.arg0();
    const uint32_t handler = guest::LoadU32(base, (id << 2) + kCommands);
    const uint64_t r3 = call(handler, frame, return_address, 3, command + 16, r4, r5).r3;
    command += uint32_t(r3 + 1) << 4;
    id = ChunkCommand(base, command).id();
  }
}

// ChunkChain::RunSingles (sub_8258E630, kind 0), in its 128-byte frame.
uint32_t RunSingles(uint8_t* base, uint32_t item, uint32_t sp, const GuestCall& call,
                    const SingleCommand& single) {
  ITC_NATIVE_RUNNING("sub_8258E630 ChunkChain::RunSingles");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t count = ChunkItem(base, item).single_count();
  if (count != 0) {
    uint32_t i = 0;
    do {
      const uint32_t slot = ((i + 1) << 4) + item;
      const uint32_t handler =
          guest::LoadU32(base, (ChunkCommand(base, slot).id() << 2) + kSingleCommands);
      const std::optional<uint32_t> native = single(handler, slot, frame);
      const uint64_t r3 = native ? *native : call(handler, frame, 0x8258E678, 1, slot, 0, 0).r3;
      i = uint32_t(r3 + i) + 1;
    } while (i < count);
  }
  return 0;
}

// ChunkChain::RunOutOfLine (sub_8258E780, kind 3), in its 112-byte frame.
uint32_t RunOutOfLine(uint8_t* base, uint32_t item, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258E780 ChunkChain::RunOutOfLine");
  RunCommands(base, ChunkItem(base, item).link(), Frame(base, sp, 112), 0x8258E7C4, call);
  return item + 16;
}

// ChunkChain::RunOutOfLineLast (sub_8258E7E8, kind 4).
uint32_t RunOutOfLineLast(uint8_t* base, uint32_t item, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258E7E8 ChunkChain::RunOutOfLineLast");
  RunCommands(base, ChunkItem(base, item).link(), Frame(base, sp, 112), 0x8258E830, call);
  return 0;
}

// ChunkChain::RunInlineLast (sub_8258E868, kind 5).
uint32_t RunInlineLast(uint8_t* base, uint32_t item, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258E868 ChunkChain::RunInlineLast");
  RunCommands(base, ChunkItem(base, item).command_id_address(), Frame(base, sp, 112), 0x8258E8B0,
              call);
  return 0;
}

}  // namespace

int32_t Partition(uint8_t* base, uint32_t array, int32_t lo, int32_t hi, uint32_t getter,
                  uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258EC38 RenderQueue::Partition");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t pivot = Element(base, array, hi);
  int32_t i = lo - 1;
  int32_t j = hi;
  for (;;) {
    const double pivot_key = Key(base, getter, pivot, frame, 0x8258EC70, call);
    do {
      i++;
    } while (Key(base, getter, Element(base, array, i), frame, 0x8258EC90, call) < pivot_key);
    for (;;) {
      j--;
      if (!(i < j)) {
        const uint32_t at_i = (uint32_t(i) << 2) + array;
        const uint32_t at_hi = (uint32_t(hi) << 2) + array;
        const uint32_t high = guest::LoadU32(base, at_hi);
        const uint32_t low = guest::LoadU32(base, at_i);
        guest::StoreU32(base, at_i, high);
        guest::StoreU32(base, at_hi, low);
        return i;
      }
      if (!(pivot_key < Key(base, getter, Element(base, array, j), frame, 0x8258ECB8, call))) {
        break;
      }
    }
    const uint32_t at_j = (uint32_t(j) << 2) + array;
    const uint32_t at_i = (uint32_t(i) << 2) + array;
    const uint32_t high = guest::LoadU32(base, at_j);
    const uint32_t low = guest::LoadU32(base, at_i);
    guest::StoreU32(base, at_i, high);
    guest::StoreU32(base, at_j, low);
  }
}

void QuickSort(uint8_t* base, uint32_t array, int32_t lo, int32_t hi, uint32_t getter,
               uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82590CB0 RenderQueue::QuickSort");
  const uint32_t frame = Frame(base, sp, 128);
  while (lo < hi) {
    const int32_t p = Partition(base, array, lo, hi, getter, frame, call);
    QuickSort(base, array, lo, p - 1, getter, frame, call);
    lo = p + 1;
  }
}

void ChunkChainRun(uint8_t* base, uint32_t queue, uint32_t item, uint32_t sp,
                   const GuestCall& call, const SingleCommand& single) {
  ITC_NATIVE_RUNNING("sub_8258EE98 ChunkChain::Run");
  const uint32_t frame = Frame(base, sp, 96);
  while (item != 0) {
    const uint32_t kind = uint32_t(int32_t(int16_t(ChunkItem(base, item).kind() & 0xFFFF)));
    switch (kind) {
      case 0:
        item = RunSingles(base, item, frame, call, single);
        break;
      case 1:  // ChunkChain::RunInline, never seen in play
        item = uint32_t(call(0x8258E698, frame, 0x8258EF0C, 2, queue, item, 0).r3);
        break;
      case 2:  // ChunkChain::RunInlineLinked, never seen in play
        item = uint32_t(call(0x8258E718, frame, 0x8258EF18, 2, queue, item, 0).r3);
        break;
      case 3:
        item = RunOutOfLine(base, item, frame, call);
        break;
      case 4:
        item = RunOutOfLineLast(base, item, frame, call);
        break;
      case 5:
        item = RunInlineLast(base, item, frame, call);
        break;
      case 6:
        return;
      default:
        break;  // the same item again, as the original (it never ends)
    }
  }
}

void Replay(uint8_t* base, uint32_t queue, uint32_t device, uint32_t video_output, uint32_t sp,
            const GuestCall& call, const SingleCommand& single) {
  ITC_NATIVE_RUNNING("sub_82591238 RenderQueue::Replay");
  const uint32_t frame = Frame(base, sp, 192);
  G::set_replay_device(base, device);
  State(base, queue).set_video_output(video_output);
  State(base, queue).set_cleared_24(0);
  State(base, queue).set_cleared_28(0);
  State(base, queue).set_cleared_32(0);
  State(base, queue).set_cleared_36(0);
  const RenderQueueState state = State(base, queue);
  G::set_replay_drawer(base, state.drawer_address());
  G::set_release_list(base, state.release_list_address());
  G::set_replay_drawer_state(base, state.drawer_state_address());
  G::set_replay_render_targets(base, state.render_target_0_address());
  models::PointersClear(base, state.release_list_address());
  const PassList list(base, QueueSlot(base, queue, kReplayingSlot, kReplayingFunction, Replaying,
                                       frame, 0x825912BC, call));
  uint32_t buckets_left = list.bucket_count();
  if (buckets_left != 0) {
    uint32_t bucket = list.buckets().address;
    do {
      const uint32_t first = PassListBucket(base, bucket).first().address;
      uint32_t n = 0;
      list.set_cleared_28(0);
      list.set_current_item(0);
      list.set_bucket_done(1);
      if (first != 0) {
        list.set_current_item(first);
        list.set_bucket_done(0);
      }
      if (list.bucket_done() == 0) {
        uint32_t item = list.current_item().address;
        uint32_t at = kEntries;
        do {
          guest::StoreU32(base, at, PassListItem(base, item).chunk_address());
          n++;
          at += 4;
          item = PassListItem(base, item).next().address;
          list.set_current_item(item);
          if (item == 0) {
            list.set_bucket_done(1);
          }
        } while (list.bucket_done() == 0);
        if (n != 0) {
          const int32_t last = int32_t(n - 1);
          if (last > 0) {
            const int32_t p = Partition(base, kEntries, 0, last, kMassGetter, frame, call);
            QuickSort(base, kEntries, 0, p - 1, kMassGetter, frame, call);
            QuickSort(base, kEntries, p + 1, last, kMassGetter, frame, call);
          }
          // Each run of equal keys (compared as the original's fcmpu: a NaN
          // key ends a run of none, and the original then loops for good).
          uint32_t start = 0;
          do {
            const uint32_t run = (start << 2) + kEntries;
            const double key = RenderQueueEntry(base, guest::LoadU32(base, run)).key();
            uint32_t equal = 0;
            if (start < n) {
              uint32_t k = start, at_k = run;
              do {
                const double other = RenderQueueEntry(base, guest::LoadU32(base, at_k)).key();
                if (!(key == other)) break;
                k++;
                equal++;
                at_k += 4;
              } while (k < n);
            }
            const int32_t run_last = int32_t(equal - 1);
            if (run_last > 0) {
              const int32_t p = Partition(base, run, 0, run_last, kSequenceGetter, frame, call);
              QuickSort(base, run, 0, p - 1, kSequenceGetter, frame, call);
              QuickSort(base, run, p + 1, run_last, kSequenceGetter, frame, call);
            }
            start += equal;
          } while (start < n);
          uint32_t left = n, at_entry = kEntries;
          do {
            const RenderQueueEntry entry(base, guest::LoadU32(base, at_entry));
            ChunkChainRun(base, queue, entry.chunk().address, frame, call, single);
            left--;
            at_entry += 4;
          } while (left != 0);
        }
      }
      buckets_left--;
      bucket += 16;
    } while (buckets_left != 0);
  }
  // The release list: each object's slot 0 with r4 = 1, the element then 0.
  PtrVector vector = G::release_list(base);
  const uint32_t first = vector.first().address;
  const uint32_t size = first == 0 ? 0 : Count(first, vector.last().address);
  for (uint32_t i = 0, offset = 0; i < size; i++, offset += 4) {
    // The original keeps the address of the first word it read: `vector`
    // read again below isn't used for it.
    const PtrVector at_first = vector;
    const uint32_t now = at_first.first().address;
    if (now == 0 || !(Count(now, at_first.last().address) > i)) {
      call(0x825944B0, frame, 0x825914EC, 0, 0, 0, 0);  // std::_Xran, never seen in play
      vector = G::release_list(base);
    }
    const uint32_t elements = at_first.first().address;
    const PtrVectorSlot element(base, offset + elements);
    const uint32_t object = element.value();
    if (object != 0) {
      call(guest::LoadU32(base, guest::LoadU32(base, object)), frame, 0x82591514, 2, object, 1,
           0);
      vector = G::release_list(base);
      element.set_value(0);
    }
  }
  G::set_replay_device(base, 0);
}

}  // namespace graphics::render_queue
