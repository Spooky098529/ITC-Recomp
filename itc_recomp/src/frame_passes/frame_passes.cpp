// The frame passes object (frame_passes.h, docs/functions/frame-passes.md):
// step for step as the recompiled functions, the command list as plain C++
// (src/command_list/), the guest calls through the adapter.

#include "frame_passes/frame_passes.h"

#include "guest.h"

#include "base/log.h"
#include "command_list/command_list.h"
#include "graphics/video_output.h"

namespace frame_passes {

namespace {

constexpr uint32_t kWorkMemCurrent = 0x8296B780;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// Whether the list has room for `slots` more (a record inside a chunk
// already open: only the room).
bool Room(uint8_t* base, uint32_t list, uint64_t slots) {
  return uint64_t(Load(base, list + 12)) + slots < Load(base, list + 8);
}
// Whether a record opening its own chunk may go on: no chunk open (on this
// list or another), and room for `slots`.
bool Ready(uint8_t* base, uint32_t list, uint64_t slots) {
  return guest::LoadU8(base, list + 52) == 0 && Load(base, command_list::kOpenChunks) == 0 &&
         Room(base, list, slots);
}

uint32_t Slot(uint8_t* base, uint32_t list) { return command_list::AllocateSlots(base, list, 1); }
// A slot with [a, b, c] (+12 left as it was).
uint32_t Slot3(uint8_t* base, uint32_t list, uint32_t a, uint32_t b, uint32_t c) {
  const uint32_t s = Slot(base, list);
  Store(base, s + 0, a);
  Store(base, s + 4, b);
  Store(base, s + 8, c);
  return s;
}
// A slot with [a, b] (+8, +12 left as they were).
uint32_t Slot2(uint8_t* base, uint32_t list, uint32_t a, uint32_t b) {
  const uint32_t s = Slot(base, list);
  Store(base, s + 0, a);
  Store(base, s + 4, b);
  return s;
}
// A slot with the 16 bytes at `from` (the original's two doubleword copies).
uint32_t Slot16(uint8_t* base, uint32_t list, uint32_t from) {
  const uint32_t s = Slot(base, list);
  for (uint32_t at = 0; at < 16; at += 4) {
    Store(base, s + at, Load(base, from + at));
  }
  return s;
}

}  // namespace

void WorkMemFlip(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82589A78 WorkMem::Flip");
  const uint32_t current = Load(base, kWorkMemCurrent) ^ 1;
  Store(base, kWorkMemCurrent, current);
  const uint32_t at = current << 2;
  Store(base, kWorkMemCurrent + 4 + 28 + at, 0);
  Store(base, kWorkMemCurrent + 4 + 20 + at, 0);
  Store(base, kWorkMemCurrent + 4 + at, 0);
}

void FlipWorkMem(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82096218 FramePasses::FlipWorkMem");
  WorkMemFlip(base);
}

uint32_t DelegateSlot2Target(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82096350 Delegate slot 2 thunk");
  return Load(base, Load(base, object) + 8);
}

std::optional<uint32_t> RecordConstants(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82096360 FramePasses::RecordConstants");
  if (!Room(base, list, 4)) {
    return std::nullopt;
  }
  Slot3(base, list, 7, 1, 0x00015601u);  // vertex, float, c86, 1
  Slot16(base, list, 0x8265ED20);
  Slot3(base, list, 7, 1, 0x01014C01u);  // pixel, float, c76, 1
  return Slot16(base, list, 0x8265ED30);
}

std::optional<uint32_t> RecordSamplers(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82096030 FramePasses::RecordSamplers");
  if (!Room(base, list, 24)) {
    return std::nullopt;
  }
  // Samplers 0-3: address U (0) and V (4) = 2.
  Slot3(base, list, 2, 8, 0);
  for (uint32_t sampler = 0; sampler < 4; sampler++) {
    Slot3(base, list, sampler, 0, 2);
    Slot3(base, list, sampler, 4, 2);
  }
  // Their min (20) and mag (16) filters = 0, mip filter (24) = 2.
  Slot3(base, list, 2, 12, 0);
  for (uint32_t sampler = 0; sampler < 4; sampler++) {
    Slot3(base, list, sampler, 20, 0);
    Slot3(base, list, sampler, 16, 0);
    Slot3(base, list, sampler, 24, 2);
  }
  // Render state 324 = 0.
  Slot3(base, list, 1, 1, 0);
  return Slot2(base, list, 324, 0);
}

std::optional<uint32_t> RecordPrimitiveReset(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82589990 FramePasses::RecordPrimitiveReset");
  const uint32_t list = Load(base, kCommandList);
  if (!Ready(base, list, 5)) {
    return std::nullopt;
  }
  command_list::OpenChunk(base, list);
  const uint32_t chunk = Slot(base, list);
  guest::StoreU16(base, chunk + 2, 5);
  Store(base, chunk + 4, 0);
  Slot3(base, list, 1, 2, 0);
  Slot2(base, list, 328, 1);       // primitive reset enable
  Slot2(base, list, 332, 0xFFFF);  // the reset index
  Slot3(base, list, 36, 0, 0);
  command_list::CloseChunk(base, list);
  return chunk;
}

bool JobWait(uint8_t* base, uint32_t job, const std::function<void(uint32_t object)>& wait) {
  ITC_NATIVE_RUNNING("sub_82094B10 Job::Wait");
  if (guest::LoadU8(base, job + 18) == 0) {
    return false;
  }
  wait(Load(base, job + 20));
  guest::StoreU8(base, job + 18, 0);
  return true;
}

uint32_t Boot(uint8_t* base, uint32_t passes, uint32_t frame, const BootCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820962B8 FramePasses::Boot");
  namespace vo = graphics::video_output;
  const uint32_t video_output = calls.video_output(Load(base, passes + 4));
  const uint32_t device = vo::LockDevice(base, video_output);
  const uint32_t local = frame + 80;
  Store(base, local, device);
  calls.init_boot(device);
  vo::UnlockDevice(base, video_output, local);
  return video_output;
}

uint32_t Construct(uint8_t* base, uint32_t passes, uint32_t block, uint32_t frame,
                   uint32_t stack_word, const ConstructCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82098630 FramePasses::FramePasses");
  constexpr uint32_t kDelegateVtable = 0x82002850;
  constexpr uint32_t kFunctorBase = 0x82000ABC;
  const uint32_t job = passes + 8;
  Store(base, passes + 0, 0x82000F20);
  const uint32_t interface = Load(base, block + 4);
  Store(base, passes + 4, interface);
  // The job, as the original leaves it (its +4 0x82000AB4, then the job's
  // vtable 0x82002908; the fields stored twice once).
  Store(base, job + 0, 0x820066DC);
  Store(base, job + 4, 0x82002908);
  Store(base, job + 8, 0);
  Store(base, job + 12, 0);
  guest::StoreU8(base, job + 16, 0);
  guest::StoreU8(base, job + 17, 1);
  guest::StoreU8(base, job + 18, 0);
  Store(base, job + 20, stack_word);
  Store(base, job + 24, kDelegateVtable);
  Store(base, job + 28, passes);
  Store(base, job + 32, 0x82096330);  // slot 0, FramePasses::Boot
  calls.post(Load(base, passes + 4), job);
  calls.wait(job);

  const uint32_t key = frame + 80;
  // Tick handler 3: slot 1 (FramePasses::Record) through sub_82096340.
  Store(base, frame + 92, passes);
  Store(base, frame + 88, kDelegateVtable);
  Store(base, frame + 96, 0x82096340);
  Store(base, key, 3);
  calls.add(Load(base, block + 0), 8, key, frame + 88, 0x82098750);
  // Tick handler 20: slot 2 (FramePasses::FlipWorkMem) through sub_82096350;
  // the first delegate destroyed.
  Store(base, frame + 104, kDelegateVtable);
  Store(base, frame + 108, passes);
  Store(base, frame + 88, kFunctorBase);
  Store(base, frame + 112, 0x82096350);
  Store(base, key, 20);
  calls.add(Load(base, block + 0), 8, key, frame + 104, 0x82098794);
  // The listener for event 8: slot 3 (FramePasses::OnEvent) through
  // sub_82096320; the second delegate destroyed (the third isn't).
  Store(base, frame + 104, kFunctorBase);
  Store(base, frame + 124, passes);
  Store(base, frame + 120, kDelegateVtable);
  Store(base, frame + 128, 0x82096320);
  Store(base, key, 8);
  calls.add(Load(base, block + 0), 4, key, frame + 120, 0x820987D8);
  return passes;
}

}  // namespace frame_passes
