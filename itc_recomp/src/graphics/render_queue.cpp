// The render queue's methods (docs/functions/device-shaders.md): see
// render_queue.h.

#include "graphics/render_queue.h"

#include "guest.h"
#include "debug/render_watch.h"

#include "base/log.h"
#include "debug/replay_stats.h"
#include "native/renderer.h"
#include "render/commands.h"
#include "graphics/device_shaders.h"
#include "graphics/resources.h"
#include "models/texture_provider.h"
#include "vehicle_accessors/vehicle_accessors.h"

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

namespace {

constexpr uint32_t kReplayingSlot = 32;  // slot 8
constexpr uint32_t kReplayingFunction = 0x8258E618;

constexpr uint32_t kMassGetter = 0x820B6708;      // RigidBody::Mass: the float at +4
constexpr uint32_t kSequenceGetter = 0x8258E5C0;  // QueueEntry::Sequence: at +8

constexpr uint32_t kSingleCommands = 0x8265ED48;
constexpr uint32_t kCommands = 0x8265EDE0;
constexpr uint32_t kEndCommand = 36;

constexpr uint32_t kReleaseList = 0x8296BD8C;
constexpr uint32_t kDevice = 0x8296BD90;
constexpr uint32_t kEntries = 0x8296BD98;  // room for 5,000 before the next
constexpr uint32_t kDrawer = 0x82970BB8;
constexpr uint32_t kState12 = 0x82970BBC;
constexpr uint32_t kRenderTargets = 0x82970BC0;

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
// Our handlers' guest calls, from the walker's frame.
render::GameCall GameCall(uint32_t frame, uint32_t return_address, const GuestCall& call) {
  return [&call, frame, return_address](uint32_t function, int count, uint64_t r3, uint64_t r4,
                                        uint64_t r5) {
    return call(function, frame, return_address, count, r3, r4, r5).r3;
  };
}

// The slots our handler and the game's answered differently (the check).
void CheckSlots(uint32_t id, uint64_t ours, uint64_t theirs) {
  if (uint32_t(ours) != uint32_t(theirs)) {
    static uint32_t logged = 0;
    if (logged++ < 20) {
      ITC_LOG_INFO("conventional renderer check: command {} took {} slots by ours, {} by the "
                   "game's",
                   id, uint32_t(ours), uint32_t(theirs));
    }
  }
}

// The commands from `command` through the table 0x8265EDE0, until id 36:
// with --conventional_renderer through our handlers (render/commands.h),
// with its check through both.
void RunCommands(uint8_t* base, uint32_t command, uint32_t frame, uint32_t return_address,
                 const GuestCall& call) {
  render::Commands* native = render::GetCommands();
  const bool conventional = native && render::ConventionalRenderer();
  uint32_t id = guest::LoadU32(base, command);
  while (int32_t(id) != int32_t(kEndCommand)) {
    const uint32_t r5 = guest::LoadU32(base, command + 8);
    const uint32_t r4 = guest::LoadU32(base, command + 4);
    uint64_t r3;
    if (conventional) {
      debug::ReplayStats_OnCommand(base, id, command + 16, r4, r5);
      native::Renderer_OnCommand(base, id, command + 16, r4, r5);
      r3 = native->Run(base, id, command + 16, r4, r5, GameCall(frame, return_address, call));
    } else {
      const uint32_t ours =
          native ? native->Run(base, id, command + 16, r4, r5, GameCall(frame, return_address, call))
                 : 0;
      const uint32_t handler = guest::LoadU32(base, (id << 2) + kCommands);
      r3 = call(handler, frame, return_address, 3, command + 16, r4, r5).r3;
      if (native && id != 7) {
        CheckSlots(id, ours, r3);
      }
    }
    command += uint32_t(r3 + 1) << 4;
    id = guest::LoadU32(base, command);
  }
}

// ChunkChain::RunSingles (sub_8258E630, kind 0), in its 128-byte frame.
uint32_t RunSingles(uint8_t* base, uint32_t item, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258E630 ChunkChain::RunSingles");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t count = guest::LoadU32(base, item + 8);
  if (count != 0) {
    uint32_t i = 0;
    do {
      const uint32_t slot = ((i + 1) << 4) + item;
      render::Commands* native = render::GetCommands();
      uint64_t r3;
      if (native && render::ConventionalRenderer()) {
        r3 = native->RunSingle(base, slot, GameCall(frame, 0x8258E678, call));
      } else {
        const uint32_t ours = native ? native->RunSingle(base, slot, GameCall(frame, 0x8258E678, call)) : 0;
        const uint32_t handler =
            guest::LoadU32(base, (guest::LoadU32(base, slot) << 2) + kSingleCommands);
        r3 = call(handler, frame, 0x8258E678, 1, slot, 0, 0).r3;
        if (native) {
          CheckSlots(100 + guest::LoadU32(base, slot), ours, r3);
        }
      }
      i = uint32_t(r3 + i) + 1;
    } while (i < count);
  }
  return 0;
}

// ChunkChain::RunOutOfLine (sub_8258E780, kind 3), in its 112-byte frame.
uint32_t RunOutOfLine(uint8_t* base, uint32_t item, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258E780 ChunkChain::RunOutOfLine");
  RunCommands(base, guest::LoadU32(base, item + 4), Frame(base, sp, 112), 0x8258E7C4, call);
  return item + 16;
}

// ChunkChain::RunOutOfLineLast (sub_8258E7E8, kind 4).
uint32_t RunOutOfLineLast(uint8_t* base, uint32_t item, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258E7E8 ChunkChain::RunOutOfLineLast");
  RunCommands(base, guest::LoadU32(base, item + 4), Frame(base, sp, 112), 0x8258E830, call);
  return 0;
}

// ChunkChain::RunInlineLast (sub_8258E868, kind 5).
uint32_t RunInlineLast(uint8_t* base, uint32_t item, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258E868 ChunkChain::RunInlineLast");
  RunCommands(base, item + 16, Frame(base, sp, 112), 0x8258E8B0, call);
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
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258EE98 ChunkChain::Run");
  const uint32_t frame = Frame(base, sp, 96);
  while (item != 0) {
    const uint32_t kind = uint32_t(int32_t(int16_t(guest::LoadU32(base, item) & 0xFFFF)));
    switch (kind) {
      case 0:
        item = RunSingles(base, item, frame, call);
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
            const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82591238 RenderQueue::Replay");
  const uint32_t frame = Frame(base, sp, 192);
  if (render::Commands* native = render::GetCommands()) {
    native->BeginReplay(base);
  }
  debug::RenderWatch_OnReplay(device);
  guest::StoreU32(base, kDevice, device);
  guest::StoreU32(base, State(base, queue) + 44, video_output);
  for (uint32_t field : {24u, 28u, 32u, 36u}) {
    guest::StoreU32(base, State(base, queue) + field, 0);
  }
  const uint32_t state = State(base, queue);
  guest::StoreU32(base, kDrawer, state + 4);
  guest::StoreU32(base, kReleaseList, state + 68);
  guest::StoreU32(base, kState12, state + 12);
  guest::StoreU32(base, kRenderTargets, state + 48);
  models::PointersClear(base, state + 68);
  const uint32_t list = QueueSlot(base, queue, kReplayingSlot, kReplayingFunction, Replaying,
                                  frame, 0x825912BC, call);
  uint32_t buckets_left = guest::LoadU32(base, list + 12);
  if (buckets_left != 0) {
    uint32_t bucket = guest::LoadU32(base, list + 24);
    do {
      const uint32_t first = guest::LoadU32(base, bucket + 0);
      uint32_t n = 0;
      guest::StoreU32(base, list + 28, 0);
      guest::StoreU32(base, list + 32, 0);
      guest::StoreU8(base, list + 4, 1);
      if (first != 0) {
        guest::StoreU32(base, list + 32, first);
        guest::StoreU8(base, list + 4, 0);
      }
      if (guest::LoadU8(base, list + 4) == 0) {
        uint32_t item = guest::LoadU32(base, list + 32);
        uint32_t at = kEntries;
        do {
          guest::StoreU32(base, at, item + 4);
          n++;
          at += 4;
          item = guest::LoadU32(base, item + 0);
          guest::StoreU32(base, list + 32, item);
          if (item == 0) {
            guest::StoreU8(base, list + 4, 1);
          }
        } while (guest::LoadU8(base, list + 4) == 0);
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
            const double key = guest::LoadFloat(base, guest::LoadU32(base, run) + 4);
            uint32_t equal = 0;
            if (start < n) {
              uint32_t k = start, at_k = run;
              do {
                const double other = guest::LoadFloat(base, guest::LoadU32(base, at_k) + 4);
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
            const uint32_t entry = guest::LoadU32(base, at_entry);
            ChunkChainRun(base, queue, guest::LoadU32(base, entry + 0), frame, call);
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
  uint32_t vector = guest::LoadU32(base, kReleaseList);
  const uint32_t first = guest::LoadU32(base, vector + 4);
  const uint32_t size = first == 0 ? 0 : Count(first, guest::LoadU32(base, vector + 8));
  for (uint32_t i = 0, offset = 0; i < size; i++, offset += 4) {
    const uint32_t first_at = vector + 4;
    const uint32_t now = guest::LoadU32(base, first_at);
    if (now == 0 || !(Count(now, guest::LoadU32(base, vector + 8)) > i)) {
      call(0x825944B0, frame, 0x825914EC, 0, 0, 0, 0);  // std::_Xran, never seen in play
      vector = guest::LoadU32(base, kReleaseList);
    }
    const uint32_t elements = guest::LoadU32(base, first_at);
    const uint32_t object = guest::LoadU32(base, offset + elements);
    if (object != 0) {
      call(guest::LoadU32(base, guest::LoadU32(base, object)), frame, 0x82591514, 2, object, 1,
           0);
      vector = guest::LoadU32(base, kReleaseList);
      guest::StoreU32(base, offset + elements, 0);
    }
  }
  guest::StoreU32(base, kDevice, 0);
}

}  // namespace graphics::render_queue
