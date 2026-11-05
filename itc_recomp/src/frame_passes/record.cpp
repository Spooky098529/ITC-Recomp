// FramePasses::Record (record.h, frame_passes.h,
// docs/functions/frame-passes.md): the getters and the render queue as
// guest calls through the adapter, the chunks recorded by the three parts,
// then the render queue's 58 adds, the scene target's passes and the
// primitive reset.

#include "frame_passes/record.h"

#include "base/log.h"
#include "command_list/command_list.h"

namespace frame_passes {

namespace record {

uint32_t Recorder::Slot() const { return command_list::AllocateSlots(base, list, 1); }

uint32_t Recorder::Slot3(uint32_t a, uint32_t b, uint32_t c) const {
  const uint32_t s = Slot();
  Store(s + 0, a);
  Store(s + 4, b);
  Store(s + 8, c);
  return s;
}

uint32_t Recorder::Slot2(uint32_t a, uint32_t b) const {
  const uint32_t s = Slot();
  Store(s + 0, a);
  Store(s + 4, b);
  return s;
}

uint32_t Recorder::Slot1(uint32_t a) const {
  const uint32_t s = Slot();
  Store(s, a);
  return s;
}

uint32_t Recorder::Head() {
  const uint32_t s = Slot();
  guest::StoreU16(base, s + 2, 5);
  Store(s + 4, 0);
  heads.push_back(s);
  return s;
}

uint32_t Recorder::Bytes(uint32_t bytes) const {
  return command_list::AllocateBytes(base, list, bytes, false);
}

void Recorder::Constants() const { RecordConstants(base, list); }
void Recorder::Samplers() const { RecordSamplers(base, list); }

}  // namespace record

namespace {

constexpr uint32_t kRecordConstants = 0x82096360;
constexpr uint32_t kRecordSamplers = 0x82096030;

// The render queue's adds after the chunk closes: the head (by its index
// in Recorder::heads), the bucket, the call's return address. Head 0 (the
// first chunk's) isn't added.
struct Add {
  uint32_t head;
  uint32_t bucket;
  uint32_t lr;
};
constexpr Add kAdds[] = {
    {16, 157, 0x82097EB0}, {3, 162, 0x82097ED4}, {4, 162, 0x82097EF8}, {1, 50, 0x82097F1C},
    {1, 56, 0x82097F3C}, {1, 62, 0x82097F5C}, {1, 68, 0x82097F7C}, {1, 74, 0x82097F9C},
    {1, 80, 0x82097FBC}, {1, 86, 0x82097FDC}, {1, 92, 0x82097FFC}, {1, 98, 0x8209801C},
    {17, 103, 0x8209803C}, {1, 161, 0x8209805C}, {1, 167, 0x8209807C}, {1, 151, 0x8209809C},
    {2, 153, 0x820980BC}, {6, 155, 0x820980E0}, {2, 52, 0x82098100}, {2, 58, 0x82098120},
    {2, 64, 0x82098140}, {2, 70, 0x82098160}, {2, 76, 0x82098180}, {2, 82, 0x820981A0},
    {2, 88, 0x820981C0}, {2, 94, 0x820981E0}, {2, 100, 0x82098200}, {2, 269, 0x82098220},
    {5, 597, 0x82098244}, {5, 123, 0x82098264}, {5, 603, 0x82098284}, {7, 16, 0x820982A8},
    {8, 20, 0x820982CC}, {9, 21, 0x820982F0}, {10, 4, 0x82098314}, {11, 6, 0x82098338},
    {12, 8, 0x8209835C}, {13, 10, 0x82098380}, {14, 12, 0x820983A4}, {15, 13, 0x820983C4},
    {7, 41, 0x820983E4}, {8, 45, 0x82098404}, {9, 46, 0x82098424}, {10, 29, 0x82098444},
    {11, 31, 0x82098464}, {12, 33, 0x82098484}, {13, 35, 0x820984A4}, {14, 37, 0x820984C4},
    {15, 38, 0x820984E4}, {16, 378, 0x82098504}, {3, 383, 0x82098524}, {4, 385, 0x82098544},
    {1, 382, 0x82098564}, {1, 388, 0x82098584}, {1, 372, 0x820985A4}, {2, 374, 0x820985C4},
    {6, 376, 0x820985E4}, {2, 490, 0x82098604},
};

}  // namespace

bool Record(uint8_t* base, uint32_t passes, uint32_t frame, const RecordCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82096418 FramePasses::Record");
  const uint32_t scene = calls.vcall(guest::LoadU32(base, passes + 4), 12, 0x82096440);
  const uint32_t list = calls.vcall(scene, 4, 0x82096454);
  const uint32_t queue = calls.vcall(scene, 8, 0x8209646C);
  // Declined (the original then) where a chunk is open, the list hasn't
  // room for every slot, or this's slots 5 and 6 aren't the recorders.
  const uint32_t vtable = guest::LoadU32(base, passes);
  if (guest::LoadU8(base, list + 52) != 0 ||
      guest::LoadU32(base, command_list::kOpenChunks) != 0 ||
      uint64_t(guest::LoadU32(base, list + 12)) + record::kSlots >= guest::LoadU32(base, list + 8) ||
      guest::LoadU32(base, vtable + 20) != kRecordConstants ||
      guest::LoadU32(base, vtable + 24) != kRecordSamplers) {
    return false;
  }
  // The render queue's sort keys.
  guest::StoreU32(base, 0x8296B90C, 168);
  guest::StoreU32(base, 0x8296B900, 270);
  guest::StoreU32(base, 0x8296B908, 163);
  command_list::OpenChunk(base, list);
  record::Recorder r{base, list, passes, frame, {}};
  r.heads.reserve(18);
  record::RecordPart1(r);
  record::RecordPart2(r);
  record::RecordPart3(r);
  command_list::CloseChunk(base, list);
  for (const Add& add : kAdds) {
    calls.add(queue, add.head < r.heads.size() ? r.heads[add.head] : 0, add.bucket, add.lr);
  }
  calls.record_passes(list);
  calls.primitive_reset();
  return true;
}

}  // namespace frame_passes
