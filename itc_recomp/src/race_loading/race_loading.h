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
// The UI movie calls, through their overrides (their --log_pause_movie
// lines, Run's frame pacing and SetText's heap checks kept there, as the
// in-race screens' calls, race_screens.h).
constexpr uint32_t kResetPose = 0x8211ABA8;    // UiMovie::ResetPose
constexpr uint32_t kShowNode = 0x8211AC00;     // UiMovie::ShowNode
constexpr uint32_t kHideNode = 0x8211AC48;     // UiMovie::HideNode
constexpr uint32_t kSetText = 0x8211C1F8;      // UiMovie::SetText
constexpr uint32_t kFinishNamed = 0x8211DF20;  // UiMovie::FinishNamed
constexpr uint32_t kPlayNamed = 0x8211DAC8;    // UiMovie::PlayNamed
constexpr uint32_t kLoopLast = 0x8211DDE8;     // UiMovie::LoopLast
constexpr uint32_t kRun = 0x8211E1B8;          // UiMovie::Run
// GetLocalTime, through its override (src/hooks/time.cpp: its time zone
// bias is worked out in a guest frame).
constexpr uint32_t kGetLocalTime = 0x822968B8;
// sub_82293460's callees (not seen in play; not read in detail).
constexpr uint32_t kSystemRequestCheck = 0x82293358;
constexpr uint32_t kSystemRequestRun = 0x82292828;

// The loading screen's globals: shown (1 once Show ran), the current movie,
// the three movies; the animation each plays (a word each, 0, 13 and 0 in
// the image); the system request sub_82293460 tests.
constexpr uint32_t kLoadingShown = 0x82815E60;
constexpr uint32_t kLoadingCurrent = 0x82815E64;
constexpr uint32_t kLoadingMovies = 0x82815E68;
constexpr uint32_t kLoadingAnimations = 0x8263799C;
constexpr uint32_t kSystemRequest = 0x82816E3C;

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

// The globals the course's wait and end read: the course scenery's manager
// (Scenery::IsLoaded's), the collision mesh's and the route's load states
// (2 once loaded; the words +8 into 0x826C5B1C and 0x826C5B10), layer 0's
// fade counts (out and in), and the guest functions they call.
constexpr uint32_t kSceneryManager = 0x826C5514;
constexpr uint32_t kCollisionState = 0x826C5B24;
constexpr uint32_t kRouteState = 0x826C5B18;
constexpr uint32_t kLayer0FadeOut = 0x826A50E4;
constexpr uint32_t kLayer0FadeIn = 0x826A50DC;
constexpr uint32_t kCarSceneUpdate = 0x820C9608;  // CarScene::Update, through its adapter

// sub_821514A0 CourseSetup::WaitCourse ((3, 0); 112-byte frame): when
// Scenery::IsLoaded([0x826C5514]) answers nonzero (its plain C++), the
// collision mesh's and the route's states are both 2, Text::IsLoaded
// answers nonzero and DataTables::IsLoading 0 (their plain C++): message 3
// posted and the draw step (+9648) = 0; returns 0 (IsLoading's r3).
// Otherwise the debug print "CRS SETUP WAITING." to the empty sub_82106540
// (nothing done); returns 500.
uint64_t WaitCourse(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                    const Calls& calls);
// sub_821516F0 CourseSetup::EndTick ((37, 0); 112-byte frame): the end's
// tick count (+9644) up by 1; when Scenery::IsLoaded([0x826C5514]) answers
// nonzero, CarScene::LoadsDone([0x826C2E44]) 1 (their plain C++, LoadsDone's
// finishers guest calls), the archive's queue is empty, the count is above
// 2 (unsigned) and layer 0's fade counts are both 0: message 50 posted,
// returns 0 (the queue length's r3). Otherwise CarScene::Update (a guest
// call with the r3 the failed test left), returning its r3.
uint64_t EndTick(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                 const Calls& calls);

// --- The loading screen (race_loading.cpp) ---

// sub_82290B30 UiMovie::SetFrames (probably; leaf): from the movie's live
// scene ([movie + 16]) chunk by chunk (+4 the size) up to the TRM one, +72
// of each TXT chunk = `text` and of each PRM and PIC chunk = `other`. No
// adapter: only LoadingScreen::Show calls it in play.
void SetFrames(uint8_t* base, uint32_t movie, uint32_t other, uint32_t text);
// sub_822478D0 LoadingScreen::Show (144-byte frame): r3 `tip` (movie 0's
// tip shown when not 0), r4 `which` (0 to 2, unsigned: 3 or more does
// nothing, r3 left), r5 `frames` (-1 for none): the movie reset
// (UiMovie::ResetPose), its frames set (SetFrames), its title text
// (UiMovie::SetText, r7 to r10 passed as 0: the user's call for its unset
// varargs, 2026-10-30), movie 0's tip (GetLocalTime's milliseconds % 14
// picking strings 1201 + 2n and the next into nodes 9 and 10, shown) or
// those nodes hidden, its animation finished, played and looped, and the
// shown word = 1. Returns r3 as the original leaves it (LoopLast's).
uint64_t Show(uint8_t* base, uint64_t tip, uint32_t which, uint32_t frames, uint32_t sp,
              const Calls& calls);
// sub_82247898 LoadingScreen::Run and sub_82247A60 LoadingScreen::Finish
// (leaves but for their tail calls): r3 `which` (-1 for the current; 3 or
// more, signed, does nothing, r3 left): UiMovie::Run(movie, 0) or
// UiMovie::FinishNamed(movie, its animation), from the caller's frame `sp`
// with the caller's return address `lr`. Return the callee's r3.
uint64_t Run(uint8_t* base, uint64_t which, uint32_t sp, uint32_t lr, const Calls& calls);
uint64_t Finish(uint8_t* base, uint64_t which, uint32_t sp, uint32_t lr, const Calls& calls);
// sub_82293460 (96-byte frame): with [0x82816E3C] set and sub_82293358
// answering nonzero, sub_82292828([0x82816E3C]) and 1; else 0.
uint64_t CheckSystemRequest(uint8_t* base, uint32_t sp, const Calls& calls);

}  // namespace race_loading
