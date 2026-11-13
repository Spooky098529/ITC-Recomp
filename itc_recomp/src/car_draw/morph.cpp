// The morphed draws' per-draw morph as plain C++ (docs/functions/car-draw.md:
// XGSetVertexBufferHeader, WorkMem::AllocVertexBuffer,
// Model::QueueJobRelease and Model::MorphShape); see car_draw.h. Built with
// -ffp-model=strict (CMakeLists.txt): the release's key is the one float.

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "command_list/command_list.h"
#include "guest.h"
#include "models/xpr.h"
#include "scenery_lists/scenery_lists.h"
#include "second_set/morph.h"

#include <cstdlib>

namespace car_draw {

namespace {

namespace sl = scenery_lists;

// The work memory: the current buffer's index, and per buffer (4i) the
// headers taken, the bytes used and the start; the capacity; the headers.
constexpr uint32_t kWorkIndex = 0x8296B780;
constexpr uint32_t kWorkHeaders = 0x8296B798;
constexpr uint32_t kWorkUsed = 0x8296B7A0;
constexpr uint32_t kWorkStart = 0x8296B7A8;
constexpr uint32_t kWorkCapacity = 0x8296B7B0;
constexpr uint32_t kHeaderTable = 0x82957780;
constexpr uint32_t kHeadersPerBuffer = 1280;

constexpr uint32_t kRecordList = 0x8296B918;
constexpr uint32_t kReleaseBucket = 0x8296B904;
constexpr uint32_t kPoolMaker = 0x8296B914;
constexpr uint32_t kMakeMorphJob = 0x820AC7F0;  // the second pool maker's slot 1
constexpr uint32_t kZero = 0x82000A34;  // 0.0

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// The bytes used once `bytes` more are taken: up by them, rounded up to 128.
uint32_t UsedAfter(uint32_t used, uint32_t bytes) {
  uint32_t after = used + bytes;
  const uint32_t over = after & 0x7F;
  if (over != 0) {
    after += 128 - over;
  }
  return after;
}

// Whether `bytes` and a header fit with `used` bytes and `headers` headers
// taken from the current buffer (the original's unsigned compares).
bool Fits(uint8_t* base, uint32_t bytes, uint32_t used, uint32_t headers) {
  return used + bytes < Load(base, kWorkCapacity) && headers < kHeadersPerBuffer;
}

uint32_t Take(uint8_t* base, uint32_t list) {
  const uint32_t slot = command_list::AllocateSlots(base, list, 1);
  if (slot == 0) {
    ITC_LOG_ERROR("car_draw: the command list refused a release slot after its checks");
    std::abort();
  }
  return slot;
}

}  // namespace

// --- XGSetVertexBufferHeader ---

uint32_t XgSetVertexBufferHeader(uint8_t* base, uint32_t length, uint32_t usage, uint32_t offset,
                                 uint32_t header, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8235A688 XGSetVertexBufferHeader");
  Frame(base, stack, kXgVertexBufferHeaderFrame);
  // memset (sub_825A71D0) with 0: the same bytes.
  for (uint32_t i = 0; i < 32; i += 4) {
    Store(base, header + i, 0);
  }
  Store(base, header + 0, 1);
  if ((usage & 0x4) != 0) {
    Store(base, header + 0, 0x00200001);
  }
  if ((usage & 0x200) != 0) {
    Store(base, header + 0, Load(base, header + 0) | 0x00400000);
  }
  Store(base, header + 4, 1);
  const uint32_t kept = Load(base, header + 28) & 0x08000000;
  Store(base, header + 20, 0xFFFF0000);
  Store(base, header + 24, offset | 3);
  Store(base, header + 28, (length & 0x03FFFFFC) | kept | 0x10000000 | 2);
  return header;
}

// --- WorkMem::AllocVertexBuffer ---

std::optional<uint32_t> WorkMemAllocVertexBuffer(uint8_t* base, uint32_t header_out,
                                                 uint32_t bytes, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8258A9E8 WorkMem::AllocVertexBuffer");
  const uint32_t i4 = Load(base, kWorkIndex) << 2;
  const uint32_t headers = Load(base, kWorkHeaders + i4);
  const uint32_t used = Load(base, kWorkUsed + i4);
  if (!Fits(base, bytes, used, headers)) {
    return std::nullopt;
  }
  const uint32_t frame = Frame(base, stack, kAllocVertexBufferFrame);
  const uint32_t header =
      kHeaderTable + ((((Load(base, kWorkIndex) * 5) << 8) + headers) << 5);
  XgSetVertexBufferHeader(base, bytes, 0, used, header, frame);
  models::RelocateResource(base, header, Load(base, kWorkStart + i4));
  Store(base, kWorkHeaders + i4, Load(base, kWorkHeaders + i4) + 1);
  const uint32_t unrounded = bytes + Load(base, kWorkUsed + i4);
  Store(base, kWorkUsed + i4, unrounded);
  Store(base, kWorkUsed + i4, UsedAfter(unrounded, 0));
  Store(base, header_out, header);
  return Load(base, kWorkStart + i4) + used;
}

// --- Model::QueueJobRelease ---

std::optional<Left> ModelQueueJobRelease(uint8_t* base, const Calls& calls, uint32_t job,
                                         uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82594E88 Model::QueueJobRelease");
  const uint32_t list = Load(base, kRecordList);
  if (!sl::CommandListHasRoom(base, list, 4)) {
    return std::nullopt;
  }
  const uint32_t frame = Frame(base, stack, kQueueJobReleaseFrame);
  command_list::OpenChunk(base, list);
  const uint32_t head = Take(base, list);
  const uint32_t command = Take(base, list);
  Store(base, command + 0, 22);
  Store(base, command + 4, 1);
  Store(base, command + 8, 0);
  const uint32_t argument = Take(base, list);
  Store(base, argument + 4, job);
  Store(base, argument + 8, 1);
  const uint32_t end = Take(base, list);
  Store(base, end + 4, 0);
  Store(base, end + 8, 0);
  Store(base, end + 0, 36);
  command_list::CloseChunk(base, list);
  Store(base, head + 4, command);
  const double key = double(guest::LoadFloat(base, kZero));
  guest::StoreU16(base, head + 2, 4);
  calls.draw.submit(calls.draw.user, head, Load(base, kReleaseBucket), key, frame, 0x82594F4C);
  Left left;
  left.in_ctx = true;
  return left;
}

// --- Model::MorphShape ---

std::optional<Left> ModelMorphShape(uint8_t* base, const Calls& calls, uint32_t model,
                                    uint32_t morph, uint64_t shape_index, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82595110 Model::MorphShape");
  const uint32_t shape = uint32_t(shape_index * 544 + Load(base, model + 40));
  const uint32_t stream0 = Load(base, shape + 400);
  const uint32_t stream1 = Load(base, shape + 404);
  const uint32_t weights = Load(base, shape + 408);
  // Both allocations and the release checked before anything is done.
  {
    const uint32_t i4 = Load(base, kWorkIndex) << 2;
    const uint32_t headers = Load(base, kWorkHeaders + i4);
    const uint32_t used = Load(base, kWorkUsed + i4);
    const uint32_t bytes0 = Load(base, stream0 + 12);
    if (!Fits(base, bytes0, used, headers)) {
      return std::nullopt;
    }
    const uint32_t bytes1 = Load(base, stream1 + 12);
    if (bytes1 != 0 && !Fits(base, bytes1, UsedAfter(used, bytes0), headers + 1)) {
      return std::nullopt;
    }
    if (Load(base, kPoolMaker) != 0 &&
        !sl::CommandListHasRoom(base, Load(base, kRecordList), 4)) {
      return std::nullopt;
    }
  }
  const uint32_t frame = Frame(base, stack, kMorphShapeFrame);
  auto alloc = [&](uint32_t header_out, uint32_t bytes) {
    const auto memory = WorkMemAllocVertexBuffer(base, header_out, bytes, frame);
    if (!memory) {
      ITC_LOG_ERROR("car_draw: WorkMem::AllocVertexBuffer declined after its checks");
      std::abort();
    }
    return *memory;
  };
  Store(base, shape + 416, alloc(shape + 412, Load(base, stream0 + 12)));
  const uint32_t bytes1 = Load(base, stream1 + 12);
  if (bytes1 == 0) {
    Store(base, shape + 424, 0);
  } else {
    Store(base, shape + 424, alloc(shape + 420, bytes1));
  }
  Store(base, morph + 8, Load(base, shape + 416));
  Store(base, morph + 12, Load(base, shape + 424));
  const uint32_t maker = Load(base, kPoolMaker);
  Left left;
  if (maker != 0) {
    // The maker's slot 1 (sub_820AC7F0 in play: a job blending on a worker
    // thread), its plain C++ where it runs natively.
    const uint32_t slot = Load(base, Load(base, maker) + 4);
    std::optional<uint32_t> made;
    if (slot == kMakeMorphJob && calls.make_morph_job) {
      made = calls.make_morph_job(maker, morph, frame);
    }
    uint32_t job;
    if (made) {
      job = *made;
    } else {
      const uint64_t regs[2] = {maker, morph};
      job = uint32_t(calls.call(slot, regs, 2, frame, 0x825951A4));
    }
    Store(base, weights + 20, job);
    const auto release = ModelQueueJobRelease(base, calls, job, frame);
    if (!release) {
      ITC_LOG_ERROR("car_draw: Model::QueueJobRelease declined after its checks");
      std::abort();
    }
    left = *release;
  } else {
    const auto blend = second_set::MorphBlend(base, morph, frame);
    if (blend) {
      left.r3 = blend->r3;
    } else {
      const uint64_t regs[1] = {morph};
      calls.call(0x8258BD28, regs, 1, frame, 0x825951CC);
      left.in_ctx = true;
    }
    Store(base, weights + 20, 0);
  }
  Store(base, shape + 428, 1);
  return left;
}

}  // namespace car_draw
