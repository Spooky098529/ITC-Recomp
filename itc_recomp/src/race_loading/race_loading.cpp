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

void SetFrames(uint8_t* base, uint32_t movie, uint32_t other, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82290B30 UiMovie::SetFrames");
  constexpr uint32_t kTrm = 0x54524D00, kTxt = 0x54585400, kPrm = 0x50524D00,
                     kPic = 0x50494300;
  uint32_t chunk = Load(base, movie + 16);
  for (uint32_t tag = Load(base, chunk); tag != kTrm; tag = Load(base, chunk)) {
    if (tag == kTxt) {
      Store(base, chunk + 72, text);
    } else if (tag == kPrm || tag == kPic) {
      Store(base, chunk + 72, other);
    }
    chunk += Load(base, chunk + 4);
  }
}

uint64_t Show(uint8_t* base, uint64_t tip, uint32_t which, uint32_t frames, uint32_t sp,
              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822478D0 LoadingScreen::Show");
  const uint32_t frame = Frame(base, sp, 144);  // made before the test
  if (which >= 3) {
    return tip;
  }
  constexpr uint64_t kNone = ~0ull;  // li -1
  const uint32_t movie = Load(base, kLoadingMovies + 4 * which);
  Store(base, kLoadingCurrent, which);
  calls.Call(kResetPose, frame, 0x8224790C, 1, movie);
  if (int32_t(frames) != -1) {
    SetFrames(base, movie, frames, frames);
  }
  uint32_t current = Load(base, kLoadingCurrent);
  if (current == 0 || current == 2) {
    calls.Call(kSetText, frame, 0x8224795C, 8, movie, kNone, current == 0 ? 3 : 2, 1200);
    current = Load(base, kLoadingCurrent);
  }
  if (current == 0) {
    if (int32_t(tip) != 0) {
      calls.Call(kGetLocalTime, frame, 0x82247978, 1, frame + 80);
      const int32_t milliseconds = guest::LoadU16(base, frame + 94);
      const uint32_t id = uint32_t(milliseconds % 14) * 2 + 1201;
      calls.Call(kShowNode, frame, 0x822479A4, 3, movie, kNone, 9);
      calls.Call(kShowNode, frame, 0x822479B4, 3, movie, kNone, 10);
      calls.Call(kSetText, frame, 0x822479C8, 8, movie, kNone, 9, id);
      calls.Call(kSetText, frame, 0x822479DC, 8, movie, kNone, 10, id + 1);
    } else {
      calls.Call(kHideNode, frame, 0x822479F0, 3, movie, kNone, 9);
      calls.Call(kHideNode, frame, 0x82247A00, 3, movie, kNone, 10);
    }
    current = Load(base, kLoadingCurrent);
  }
  calls.Call(kFinishNamed, frame, 0x82247A1C, 2, movie,
             Load(base, kLoadingAnimations + (current << 2)));
  current = Load(base, kLoadingCurrent);
  calls.Call(kPlayNamed, frame, 0x82247A40, 6, movie,
             Load(base, kLoadingAnimations + (current << 2)), 0, 0, 0, 0);
  const uint64_t r3 = calls.Call(kLoopLast, frame, 0x82247A48, 1, movie);
  Store(base, kLoadingShown, 1);
  return r3;
}

namespace {

// Run's and Finish's movie index: r3, or the current one for -1 (not
// checked again); nullopt (nothing done) for 3 or more, signed.
bool Which(uint8_t* base, uint64_t r3, uint32_t* which) {
  const int32_t index = int32_t(uint32_t(r3));
  if (index >= 3) {
    return false;
  }
  *which = index == -1 ? Load(base, kLoadingCurrent) : uint32_t(index);
  return true;
}

}  // namespace

uint64_t Run(uint8_t* base, uint64_t which, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82247898 LoadingScreen::Run");
  uint32_t index;
  if (!Which(base, which, &index)) {
    return which;
  }
  return calls.Call(kRun, sp, lr, 2, Load(base, kLoadingMovies + (index << 2)), 0);
}

uint64_t Finish(uint8_t* base, uint64_t which, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82247A60 LoadingScreen::Finish");
  uint32_t index;
  if (!Which(base, which, &index)) {
    return which;
  }
  return calls.Call(kFinishNamed, sp, lr, 2, Load(base, kLoadingMovies + (index << 2)),
                    Load(base, kLoadingAnimations + (index << 2)));
}

uint64_t CheckSystemRequest(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82293460 CheckSystemRequest");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t request = Load(base, kSystemRequest);
  if (request == 0) {
    return 0;
  }
  if (int32_t(uint32_t(calls.Call(kSystemRequestCheck, frame, 0x8229348C, 1, request))) == 0) {
    return 0;
  }
  calls.Call(kSystemRequestRun, frame, 0x8229349C, 1, Load(base, kSystemRequest));
  return 1;
}

}  // namespace race_loading
