// The morphed draws' per-draw morph as plain C++ (docs/functions/car-draw.md:
// XGSetVertexBufferHeader, WorkMem::AllocVertexBuffer,
// Model::QueueJobRelease and Model::MorphShape); see car_draw.h. Built with
// -ffp-model=strict (CMakeLists.txt): the release's key is the one float.

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "command_list/command_list.h"
#include "guest.h"
#include "models/xpr.h"
#include "schema/car_draw.h"
#include "schema/frame_driver.h"
#include "schema/graphics_resources.h"
#include "schema/models.h"
#include "schema/second_set.h"
#include "scenery_lists/scenery_lists.h"
#include "second_set/morph.h"

#include <cstdlib>

namespace car_draw {

namespace {

namespace sl = scenery_lists;

// The work memory: the current buffer's index, and per buffer (4i) the
// headers taken, the bytes used and the start; the capacity; the headers.
constexpr uint32_t kWorkIndex = schema::FrameDriverGlobals::work_buffer_address();
constexpr uint32_t kWorkHeaders = schema::FrameDriverGlobals::work_headers_used_address();
constexpr uint32_t kWorkUsed = schema::FrameDriverGlobals::work_bytes_used_address();
constexpr uint32_t kWorkStart = schema::FrameDriverGlobals::work_start_address();
constexpr uint32_t kWorkCapacity = schema::FrameDriverGlobals::work_capacity_address();
constexpr uint32_t kHeaderTable = 0x82957780;
constexpr uint32_t kHeadersPerBuffer = 1280;

constexpr uint32_t kRecordList = schema::FrameDriverGlobals::scene_list_address();
constexpr uint32_t kReleaseBucket = 0x8296B904;
constexpr uint32_t kMakeMorphJob = 0x820AC7F0;  // the second pool maker's slot 1
constexpr uint32_t kZero = 0x82000A34;  // 0.0

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(frame, stack);
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
bool Fits(uint32_t bytes, uint32_t used, uint32_t headers) {
  return used + bytes < Load(kWorkCapacity) && headers < kHeadersPerBuffer;
}

uint32_t Take(uint32_t list) {
  const uint32_t slot = command_list::AllocateSlots(list, 1);
  if (slot == 0) {
    ITC_LOG_ERROR("car_draw: the command list refused a release slot after its checks");
    std::abort();
  }
  return slot;
}

}  // namespace

// --- XGSetVertexBufferHeader ---

uint32_t XgSetVertexBufferHeader(uint32_t length, uint32_t usage, uint32_t offset,
                                 uint32_t header, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8235A688 XGSetVertexBufferHeader");
  Frame(stack, kXgVertexBufferHeaderFrame);
  // memset (sub_825A71D0) with 0: the same bytes.
  for (uint32_t i = 0; i < 32; i += 4) {
    Store(header + i, 0);
  }
  const schema::BufferHeader h(header);
  h.set_common(1);
  if ((usage & 0x4) != 0) {
    h.set_common(0x00200001);
  }
  if ((usage & 0x200) != 0) {
    h.set_common(h.common() | 0x00400000);
  }
  h.set_ref_count(1);
  const uint32_t kept = h.size() & 0x08000000;
  h.set_locked_base(0xFFFF0000);
  h.set_data(offset | 3);
  h.set_size((length & 0x03FFFFFC) | kept | 0x10000000 | 2);
  return header;
}

// --- WorkMem::AllocVertexBuffer ---

std::optional<uint32_t> WorkMemAllocVertexBuffer(uint32_t header_out,
                                                 uint32_t bytes, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8258A9E8 WorkMem::AllocVertexBuffer");
  const uint32_t i4 = Load(kWorkIndex) << 2;
  const uint32_t headers = Load(kWorkHeaders + i4);
  const uint32_t used = Load(kWorkUsed + i4);
  if (!Fits(bytes, used, headers)) {
    return std::nullopt;
  }
  const uint32_t frame = Frame(stack, kAllocVertexBufferFrame);
  const uint32_t header =
      kHeaderTable + ((((Load(kWorkIndex) * 5) << 8) + headers) << 5);
  XgSetVertexBufferHeader(bytes, 0, used, header, frame);
  models::RelocateResource(header, Load(kWorkStart + i4));
  Store(kWorkHeaders + i4, Load(kWorkHeaders + i4) + 1);
  const uint32_t unrounded = bytes + Load(kWorkUsed + i4);
  Store(kWorkUsed + i4, unrounded);
  Store(kWorkUsed + i4, UsedAfter(unrounded, 0));
  Store(header_out, header);
  return Load(kWorkStart + i4) + used;
}

// --- Model::QueueJobRelease ---

std::optional<Left> ModelQueueJobRelease(const Calls& calls, uint32_t job,
                                         uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82594E88 Model::QueueJobRelease");
  const uint32_t list = Load(kRecordList);
  if (!sl::CommandListHasRoom(list, 4)) {
    return std::nullopt;
  }
  const uint32_t frame = Frame(stack, kQueueJobReleaseFrame);
  command_list::OpenChunk(list);
  const uint32_t head = Take(list);
  const uint32_t command = Take(list);
  Store(command + 0, 22);
  Store(command + 4, 1);
  Store(command + 8, 0);
  const uint32_t argument = Take(list);
  Store(argument + 4, job);
  Store(argument + 8, 1);
  const uint32_t end = Take(list);
  Store(end + 4, 0);
  Store(end + 8, 0);
  Store(end + 0, 36);
  command_list::CloseChunk(list);
  Store(head + 4, command);
  const double key = double(guest::LoadFloat(kZero));
  guest::StoreU16(head + 2, 4);
  calls.draw.submit(calls.draw.user, head, Load(kReleaseBucket), key, frame, 0x82594F4C);
  Left left;
  left.in_ctx = true;
  return left;
}

// --- Model::MorphShape ---

std::optional<Left> ModelMorphShape(const Calls& calls, uint32_t model,
                                    uint32_t morph, uint64_t shape_index, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82595110 Model::MorphShape");
  const uint32_t shape = uint32_t(shape_index * schema::ModelShape::kSize +
                                  schema::Model(model).shapes().address);
  const schema::ModelShape s(shape);
  const uint32_t stream0 = s.skin_array_a();
  const uint32_t stream1 = s.skin_array_b();
  const uint32_t weights = s.skin().address;
  // Both allocations and the release checked before anything is done.
  {
    const uint32_t i4 = Load(kWorkIndex) << 2;
    const uint32_t headers = Load(kWorkHeaders + i4);
    const uint32_t used = Load(kWorkUsed + i4);
    const uint32_t bytes0 = Load(stream0 + 12);
    if (!Fits(bytes0, used, headers)) {
      return std::nullopt;
    }
    const uint32_t bytes1 = Load(stream1 + 12);
    if (bytes1 != 0 && !Fits(bytes1, UsedAfter(used, bytes0), headers + 1)) {
      return std::nullopt;
    }
    if (schema::CarDrawGlobals::pool_maker() != 0 &&
        !sl::CommandListHasRoom(Load(kRecordList), 4)) {
      return std::nullopt;
    }
  }
  const uint32_t frame = Frame(stack, kMorphShapeFrame);
  auto alloc = [&](uint32_t header_out, uint32_t bytes) {
    const auto memory = WorkMemAllocVertexBuffer(header_out, bytes, frame);
    if (!memory) {
      ITC_LOG_ERROR("car_draw: WorkMem::AllocVertexBuffer declined after its checks");
      std::abort();
    }
    return *memory;
  };
  Store(shape + 416, alloc(s.skinned_stream1_address(), Load(stream0 + 12)));
  const uint32_t bytes1 = Load(stream1 + 12);
  if (bytes1 == 0) {
    Store(shape + 424, 0);
  } else {
    Store(shape + 424, alloc(s.skinned_stream2_address(), bytes1));
  }
  const schema::Morph m(morph);
  m.set_out1(Load(shape + 416));
  m.set_out2(Load(shape + 424));
  const uint32_t maker = schema::CarDrawGlobals::pool_maker();
  Left left;
  if (maker != 0) {
    // The maker's slot 1 (sub_820AC7F0 in play: a job blending on a worker
    // thread), its plain C++ where it runs natively.
    const uint32_t slot = Load(guest::VtableWord(maker) + 4);  // the maker may be in host memory
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
    schema::SkinBlock(weights).set_object(job);
    const auto release = ModelQueueJobRelease(calls, job, frame);
    if (!release) {
      ITC_LOG_ERROR("car_draw: Model::QueueJobRelease declined after its checks");
      std::abort();
    }
    left = *release;
  } else {
    const auto blend = second_set::MorphBlend(morph, frame);
    if (blend) {
      left.r3 = blend->r3;
    } else {
      const uint64_t regs[1] = {morph};
      calls.call(0x8258BD28, regs, 1, frame, 0x825951CC);
      left.in_ctx = true;
    }
    schema::SkinBlock(weights).set_object(0);
  }
  s.set_skinned(1);
  return left;
}

}  // namespace car_draw
