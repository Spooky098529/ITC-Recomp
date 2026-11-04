// The race's loading natively (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/race-loading.md): the race mode's first state
// machine (race mode + 848, CourseSetup::), which starts the race's load and
// posts message 4 once the course and the archive's queue are ready, and the
// loading screen (LoadingScreen::, three UI movies). Plain C++ over guest
// memory (the machine, the dispatcher's ring and the globals are shared with
// the recompiled code), with the originals' loads and stores in their order.
// `sp` is the caller's stack pointer: the back chain of the original's frame
// is written below it; its register saves aren't. Adapters:
// src/hooks/race_loading.cpp.

#pragma once

#include <cstdint>

namespace race_loading {

// The machine's words.
constexpr uint32_t kState = 9604;     // the state (1 at set-up)
constexpr uint32_t kCleared = 9608;   // cleared by Begin and WaitDraw
constexpr uint32_t kReady = 9640;     // cleared by OnCourseReady
constexpr uint32_t kEndTicks = 9644;  // the end's tick count (0 at set-up)
constexpr uint32_t kDrawStep = 9648;  // WaitDraw's step

// The dispatcher's ring (from the dispatcher, the handlers' r4): 50
// messages of 12 bytes from +204, the count at +804 and the start at +808.
constexpr uint32_t kRing = 204;
constexpr uint32_t kRingCount = 600;  // from the ring
constexpr uint32_t kRingStart = 604;  // from the ring
constexpr uint32_t kRingSlots = 50;
constexpr uint32_t kRingMessageBytes = 12;

// The fades' passes (words in the image: 5 and 15) and layer 0's alpha.
constexpr uint32_t kFadeInPasses = 0x82001BEC;
constexpr uint32_t kFadeOutPasses = 0x82001BF0;
constexpr uint32_t kLayer0Alpha = 0x826A50D4;

// The guest functions called.
constexpr uint32_t kQueueLength = 0x8209D7B0;  // Archive::QueueLength

// A guest function called from the frame at `r1`, with r3 to r10 (the first
// `set` of them; the others left as they are) and the link register `lr`
// (the original's return address); its r3 (64 bits). The adapter's job
// (hooks/race_loading.cpp). `native_queue`: Archive::QueueLength's plain C++
// (hooks::NativeArchiveQueue()), else the guest call.
struct Calls {
  void* user;
  uint64_t (*call)(void* user, uint32_t function, const uint64_t args[8], int set, uint32_t r1,
                   uint32_t lr);
  bool native_queue;

  uint64_t Call(uint32_t function, uint32_t r1, uint32_t lr, int set, uint64_t r3 = 0,
                uint64_t r4 = 0, uint64_t r5 = 0, uint64_t r6 = 0, uint64_t r7 = 0,
                uint64_t r8 = 0, uint64_t r9 = 0, uint64_t r10 = 0) const {
    const uint64_t args[8] = {r3, r4, r5, r6, r7, r8, r9, r10};
    return call(user, function, args, set, r1, lr);
  }
};

// --- The +848 machine's set-up (race_loading_init.cpp) ---

// CourseSetup::Init's frame (its r1 less this).
constexpr uint32_t kInitFrame = 864;
// sub_821517D8 CourseSetup::Init: the state = 1, the 11 transitions added
// (StateMachine::Add), built in the original's stack frame at `frame` (its
// r1 less kInitFrame: see race_loading_init.cpp), then the end's tick count
// 0. Returns the last StateMachine::Add's answer (`replaced` as its).
uint32_t Init(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

// --- The machine's handlers (race_loading.cpp) ---
//
// Each returns r3 as the original leaves it.

// sub_82151408 CourseSetup::Start ((1, 0); 112-byte frame): message 2
// posted, then ScreenFades::FadeIn([0x82001BEC] passes, black, layer 0).
// Returns the passes (FadeIn's r3).
uint64_t Start(uint8_t* base, uint32_t dispatcher, uint32_t sp);
// sub_82150DE0 CourseSetup::Begin ((1, 2) -> 3; leaf): +9616, +9620, +9608,
// +9632, +9636 and +9628 = 0.
void Begin(uint8_t* base, uint32_t machine);
// sub_82150DD0 CourseSetup::OnCourseReady ((3, 3) -> 4; leaf): +9640 = 0.
void OnCourseReady(uint8_t* base, uint32_t machine);
// sub_821515A0 CourseSetup::WaitDraw ((4, 0); 96-byte frame): with step 0,
// once the archive's queue is empty, the fade-out (unless layer 0's alpha is
// already 255) and step 1; with step 1, once the alpha is 255, message 4
// posted and +9608, the step = 0. Otherwise (and after step 0's fade) the
// debug print to the empty sub_82106540 (nothing done). Returns r3: 500 (the
// print's) or the machine (`r3`, when message 4 is posted).
uint64_t WaitDraw(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                  const Calls& calls);

}  // namespace race_loading
