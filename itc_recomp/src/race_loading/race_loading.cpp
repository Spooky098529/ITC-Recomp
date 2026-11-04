// The race's loading's handlers (docs/functions/race-loading.md): see
// race_loading.h.

#include "race_loading/race_loading.h"

#include "archive/archive.h"
#include "base/log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"

namespace race_loading {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The original's frame below `sp`: its back chain.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// A message posted into the dispatcher's ring, as the handlers inline it:
// the count + 1 stored, then the message's word and two zero words at (start
// + count - 1) mod 50 (unsigned).
void Post(uint8_t* base, uint32_t dispatcher, uint32_t message) {
  const uint32_t ring = dispatcher + kRing;
  const uint32_t count = Load(base, ring + kRingCount) + 1;
  const uint32_t at = Load(base, ring + kRingStart) + count - 1;
  Store(base, ring + kRingCount, count);
  const uint32_t slot = ring + (at % kRingSlots) * kRingMessageBytes;
  Store(base, slot, message);
  Store(base, slot + 4, 0);
  Store(base, slot + 8, 0);
}

}  // namespace

uint64_t Start(uint8_t* base, uint32_t dispatcher, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82151408 CourseSetup::Start");
  Frame(base, sp, 112);
  Post(base, dispatcher, 2);
  const uint32_t passes = Load(base, kFadeInPasses);
  mode_tick::ScreenFadesFadeIn(base, passes, 0, 0, 0, 0);
  return passes;
}

void Begin(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82150DE0 CourseSetup::Begin");
  for (const uint32_t field : {9616u, 9620u, 9608u, 9632u, 9636u, 9628u}) {
    Store(base, machine + field, 0);
  }
}

void OnCourseReady(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82150DD0 CourseSetup::OnCourseReady");
  Store(base, machine + kReady, 0);
}

uint64_t WaitDraw(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821515A0 CourseSetup::WaitDraw");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  if (Load(base, machine + kDrawStep) == 0) {
    const uint32_t queued = calls.native_queue
                                ? archive::QueueLength(base)
                                : uint32_t(calls.Call(kQueueLength, frame, 0x821515C4, 0));
    if (queued == 0) {
      if (int32_t(Load(base, kLayer0Alpha)) != 255) {
        mode_tick::ScreenFadesFadeOut(base, Load(base, kFadeOutPasses), 0, 0, 0, 0);
      }
      Store(base, machine + kDrawStep, 1);
    }
  } else if (int32_t(Load(base, kLayer0Alpha)) == 255) {
    Post(base, dispatcher, 4);
    Store(base, machine + kCleared, 0);
    Store(base, machine + kDrawStep, 0);
    return r3;
  }
  // "DRAW SETUP WAITING." to the empty sub_82106540 (r3 = 500, r4 = 100, r5
  // = 0, r6 the string): nothing done.
  return 500;
}

}  // namespace race_loading
