// The boot mode and the modes' other methods as plain C++ (boot_mode.h,
// docs/functions/boot-mode.md), read from the generated code.

#include "boot_mode/boot_mode.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "hooks/last_error.h"
#include "mode_tick/mode_tick.h"
#include "movies/movie.h"

namespace boot_mode {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// A movie class slot (a tail call from the caller's frame at `sp`): ours
// where the slot holds it, else the guest's. r3 as the slot leaves it (ours
// leave the object).
uint64_t MovieSlot(uint8_t* base, uint32_t offset, uint32_t ours,
                   void (*native)(uint8_t*, uint32_t), uint32_t sp, uint32_t lr,
                   const Calls& calls) {
  const uint32_t movie = Load(base, kMovieClass);
  const uint32_t target = Load(base, Load(base, movie) + offset);
  if (calls.ffmpeg_movies && target == ours) {
    native(base, movie);
    return movie;
  }
  return calls.call(target, sp, lr, movie, 0);
}

// A message (id, 0, 0) queued in the ring at `ring` (its count +600, its
// start +604): the count first, then the message at (start + count - 1) mod
// 50.
void Queue(uint8_t* base, uint32_t ring, uint32_t id) {
  const uint32_t count = Load(base, ring + kRingCount) + 1;
  const uint32_t at = Load(base, ring + kRingStart) + count - 1;
  Store(base, ring + kRingCount, count);
  const uint32_t slot = ring + (at % kRingSlots) * kRingMessageBytes;
  Store(base, slot, id);
  Store(base, slot + 4, 0);
  Store(base, slot + 8, 0);
}

// BootMachine::Init's stores and copies, in its order (read from the
// generated code): each transition's functor (vtable 0x82011918, the
// machine, the handler) built at a temporary, its six storage words copied
// (the last three never written: the stack's words go into the transitions),
// the key (state, message, 0, 0) at +80, the functor part (a pointer to the
// copy, then the copy, then the next state) and StateMachine::Add.
struct Value {
  enum Base : uint8_t { kConst, kMachine, kFrame } base;
  uint32_t value;
};
constexpr Value C(uint32_t value) { return {Value::kConst, value}; }
constexpr Value M(uint32_t offset) { return {Value::kMachine, offset}; }
constexpr Value F(uint32_t offset) { return {Value::kFrame, offset}; }

// kStore: [a] = b; kCopy6: six words from a to b, word by word; kAdd:
// StateMachine::Add(machine, key a, functor part b).
enum class Op : uint8_t { kStore, kCopy6, kAdd };
struct Step {
  Op op;
  Value a;
  Value b;
};

// (0, 0) -> 1 Boot::StartMovie; (1, 0) stay Boot::WaitForMovie; (1, 1) -> 2
// Boot::EndMovie.
constexpr Step kMachineSteps[] = {
    {Op::kStore, F(168), M(0)},
    {Op::kStore, F(164), C(kMemberFunctorVtable)},
    {Op::kStore, F(172), C(kStartMovie)},
    {Op::kStore, F(160), F(164)},
    {Op::kCopy6, F(164), F(196)},
    {Op::kStore, F(80), C(0)},
    {Op::kStore, F(192), F(196)},
    {Op::kStore, F(84), C(0)},
    {Op::kStore, F(88), C(0)},
    {Op::kStore, F(92), C(0)},
    {Op::kStore, F(220), C(1)},
    {Op::kAdd, F(80), F(192)},
    {Op::kStore, F(132), C(kMemberFunctorVtable)},
    {Op::kStore, F(136), M(0)},
    {Op::kStore, F(140), C(kWaitForMovie)},
    {Op::kStore, F(128), F(132)},
    {Op::kCopy6, F(132), F(228)},
    {Op::kStore, F(252), C(0)},
    {Op::kStore, F(80), C(1)},
    {Op::kStore, F(84), C(0)},
    {Op::kStore, F(88), C(0)},
    {Op::kStore, F(92), C(0)},
    {Op::kStore, F(224), F(228)},
    {Op::kAdd, F(80), F(224)},
    {Op::kStore, F(100), C(kMemberFunctorVtable)},
    {Op::kStore, F(104), M(0)},
    {Op::kStore, F(108), C(kEndMovie)},
    {Op::kStore, F(96), F(100)},
    {Op::kCopy6, F(100), F(260)},
    {Op::kStore, F(80), C(1)},
    {Op::kStore, F(256), F(260)},
    {Op::kStore, F(84), C(1)},
    {Op::kStore, F(88), C(0)},
    {Op::kStore, F(92), C(0)},
    {Op::kStore, F(284), C(2)},
    {Op::kAdd, F(80), F(256)},
};

}  // namespace

// --- Movies:: ---

uint64_t MoviesPlay(uint8_t* base, uint32_t id, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A43A8 Movies::Play");
  const uint32_t path = Load(base, kMoviePaths + (id << 2));
  const uint32_t movie = Load(base, kMovieClass);
  const uint32_t target = Load(base, Load(base, movie) + 4);
  if (calls.ffmpeg_movies && target == kMoviePlay) {
    movies::movie::Play(base, movie, path);
    return movie;
  }
  return calls.call(target, sp, lr, movie, path);
}

uint64_t MoviesStart(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A43D8 Movies::Start");
  return MovieSlot(base, 8, kMovieStart, movies::movie::Start, sp, lr, calls);
}

uint64_t MoviesStop(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A43F0 Movies::Stop");
  return MovieSlot(base, 12, kMovieStop, movies::movie::Stop, sp, lr, calls);
}

uint64_t MoviesPause(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A4408 Movies::Pause");
  return MovieSlot(base, 16, kMoviePause, movies::movie::Pause, sp, lr, calls);
}

uint64_t MoviesResume(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A4420 Movies::Resume");
  return MovieSlot(base, 20, kMovieResume, movies::movie::Resume, sp, lr, calls);
}

uint64_t MoviesRelease(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A4438 Movies::Release");
  return MovieSlot(base, 28, kMovieRelease, movies::movie::Release, sp, lr, calls);
}

uint64_t MoviesState(uint8_t* base, uint32_t frame, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A4450 Movies::State");
  if (calls.finished_early && calls.finished_early()) {
    return 1;
  }
  const uint32_t movie = Load(base, kMovieClass);
  const uint32_t target = Load(base, Load(base, movie) + 24);
  uint32_t state;
  if (calls.ffmpeg_movies && target == kMovieGetState) {
    state = movies::movie::GetState(base, movie);
  } else {
    state = uint32_t(calls.call(target, frame, 0x820A4474, movie, 0));
  }
  if (state <= 3) {
    return state;
  }
  return Load(base, frame + 80);  // never written: the stack's word
}

// --- The boot machine's handlers ---

uint64_t StartMovie(uint8_t* base, uint32_t frame, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82107B90 Boot::StartMovie");
  // sub_825894C8, XMP's playback taken: Dropped (system-services.md), only
  // the last error it clears.
  last_error::Set(base, calls.pcr, 0);
  const uint32_t id = Load(base, kMovieTable + (Load(base, kMovieIndex) << 2));
  MoviesPlay(base, id, frame, 0x82107BBC, calls);
  const uint32_t passes = Load(base, kStartFadePasses);
  Store(base, kMovieIndex, Load(base, kMovieIndex) + 1);
  mode_tick::ScreenFadesFadeIn(base, passes, 0, 0, 0, 0);
  return passes;
}

uint64_t WaitForMovie(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t frame,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82107C58 Boot::WaitForMovie");
  bool end = false;
  if (Load(base, kSkippable) != 0) {
    const uint32_t entry = kActionMappers + (Load(base, kCurrentPlayer) << 3);
    if (Load(base, entry + 4) != 1) {
      const uint32_t mapper = Load(base, entry);
      const uint32_t target = Load(base, Load(base, mapper) + 40);
      if ((calls.call(target, frame, 0x82107CC0, mapper, kSkipAction) & 0xFF) != 0) {
        end = true;
      }
    }
  }
  const uint32_t state_frame = frame - kStateFrame;
  if (int32_t(MoviesState(base, state_frame, calls)) == 1 &&
      Load(base, machine + kMovieStarted) == 1) {
    end = true;
  }
  if (end) {
    mode_tick::ScreenFadesFadeOut(base, 1, 0, 0, 0, 0);
    Queue(base, dispatcher + kRing, 1);
  }
  uint64_t r3 = MoviesState(base, state_frame, calls);
  if (int32_t(r3) == 1 && Load(base, machine + kMovieStarted) == 0) {
    r3 = MoviesStart(base, frame, 0x82107D70, calls);
    Store(base, machine + kMovieStarted, 1);
  }
  return r3;
}

uint64_t EndMovie(uint8_t* base, uint32_t frame, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82107BF8 Boot::EndMovie");
  MoviesStop(base, frame, 0x82107C08, calls);
  MoviesRelease(base, frame, 0x82107C0C, calls);
  // sub_825895A0, XMP's playback handed back: Dropped (system-services.md),
  // only the last error it clears and its answer, 0.
  last_error::Set(base, calls.pcr, 0);
  return 0;
}

// --- The boot mode ---

uint32_t MachineInit(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82107DD0 BootMachine::Init");
  auto at = [&](const Value& v) {
    switch (v.base) {
      case Value::kConst:
        return v.value;
      case Value::kMachine:
        return machine + v.value;
      case Value::kFrame:
        break;
    }
    return frame + v.value;
  };
  uint32_t r3 = 0;
  for (const Step& step : kMachineSteps) {
    switch (step.op) {
      case Op::kStore:
        Store(base, at(step.a), at(step.b));
        break;
      case Op::kCopy6:
        for (uint32_t i = 0; i < 6; i++) {
          Store(base, at(step.b) + 4 * i, Load(base, at(step.a) + 4 * i));
        }
        break;
      case Op::kAdd:
        r3 = game_modes::StateMachineAdd(base, machine, at(step.a), at(step.b), replaced);
        break;
    }
  }
  Store(base, machine + kMovieStarted, 0);
  return r3;
}

uint32_t Init(uint8_t* base, uint32_t mode, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82107F68 BootMode::Init");
  const uint32_t machine = mode + kMachine;
  const uint32_t r3 = MachineInit(base, machine, frame - kMachineInitFrame, replaced);
  // The delegate's function and adjustment, built at the frame's +80.
  Store(base, frame + 84, 0);
  const uint32_t dispatcher = mode + kDispatcher;
  const uint32_t count = Load(base, dispatcher + kHandlerCount);
  Store(base, frame + 80, kNextModeThunk);
  Store(base, dispatcher + kHandlerCount, count + 1);
  Store(base, dispatcher + (count << 2), machine);
  const uint32_t delegate = mode + 8;
  if (delegate != 0) {
    const uint32_t function = Load(base, frame + 80), adjustment = Load(base, frame + 84);
    Store(base, delegate + 4, mode);
    Store(base, delegate + 8, function);
    Store(base, delegate + 12, adjustment);
    Store(base, delegate, kDelegateVtable);
  }
  Store(base, mode + 4, delegate);
  Store(base, kModeAsked, kNoModeAsked);
  Store(base, kPaused, 0);
  return r3;
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t frame, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82108018 BootMode::NextMode");
  const uint32_t blocked0 = Load(base, kPadBlocked0);
  Store(base, answer + 4, 0);
  const uint32_t was_paused = Load(base, kPaused);
  guest::StoreU8(base, answer, 0);
  uint32_t paused;
  if ((blocked0 != 0 || Load(base, kPadBlocked1) != 0) && Load(base, kSkippable) != 0) {
    paused = 1;
  } else {
    paused = Load(base, kModeAsked) == kNoModeAsked ? 0 : 1;
  }
  Store(base, kPaused, paused);
  if (was_paused != paused) {
    if (paused != 0) {
      MoviesPause(base, frame, 0x821080AC, calls);
    } else {
      MoviesResume(base, frame, 0x821080B4, calls);
    }
  }
  Queue(base, mode + 240, 0);
  // GameMessages::Dispatch (its override, src/frame_pacing/game_messages.cpp).
  calls.call(0x821062E8, frame, 0x82108108, mode + kDispatcher, mode);
  if (Load(base, mode + kMachineState) != 2) {
    return;
  }
  uint32_t type = Load(base, kModeAsked);
  if (type == kNoModeAsked) {
    const uint32_t index = Load(base, kMovieIndex);
    if (index >= 5 || Load(base, kMovieTable + (index << 2)) == kEndOfList) {
      type = Load(base, kNextType);
    } else {
      type = 3;
    }
  }
  // The answer built at the frame's +80 (bytes +81 to +83 not written: the
  // stack's), then its 8 bytes copied.
  Store(base, frame + 84, type);
  guest::StoreU8(base, frame + 80, 1);
  const uint32_t w0 = Load(base, frame + 80), w1 = Load(base, frame + 84);
  Store(base, answer, w0);
  Store(base, answer + 4, w1);
}

void Destruct(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82106D08 BootMode::~BootMode");
  Store(base, mode, kBootModeVtable);
  Store(base, mode + 32, kBootModeVtable32);
  // sub_82106540 on +848 (the state machine needs nothing): empty.
  Store(base, mode, kModeVtable);
  Store(base, mode + 32, kModeVtable32);
}

void DeletingDestructor(uint8_t* base, uint32_t mode, uint32_t flags, uint32_t frame,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82106D68 BootMode::DeletingDestructor");
  Destruct(base, mode);
  if (flags & 1) {
    calls.call(kOperatorDelete, frame, 0x82106D98, mode, flags);
  }
}

void ModeDeletingDestructor(uint8_t* base, uint32_t mode, uint32_t flags, uint32_t frame,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82106E38 Mode::DeletingDestructor");
  Store(base, mode, kModeVtable);
  Store(base, mode + 32, kModeVtable32);
  if (flags & 1) {
    calls.call(kOperatorDelete, frame, 0x82106E70, mode, flags);
  }
}

uint64_t NextModeThunk(uint8_t* base, uint64_t answer, uint64_t mode, uint32_t sp, uint32_t lr,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82235C08 Mode::NextModeThunk");
  const uint32_t target = Load(base, Load(base, uint32_t(mode)) + 24);
  if (target == kNextMode) {
    NextMode(base, uint32_t(answer), uint32_t(mode), sp - kNextModeFrame, calls);
    return answer;
  }
  return calls.call(target, sp, lr, answer, mode);
}

void ModeDelegateInvoke(uint8_t* base, uint32_t answer, uint32_t delegate, uint32_t frame,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223D440 ModeDelegate::Invoke");
  const uint32_t adjustment = Load(base, delegate + 12);
  const uint32_t object = Load(base, delegate + 4);
  const uint32_t function = Load(base, delegate + 8);
  const uint64_t r4 = uint64_t(adjustment) + uint64_t(object);
  const uint32_t buffer = frame + 80;
  constexpr uint32_t kReturn = 0x8223D470;
  const uint64_t r3 = function == kNextModeThunk
                          ? NextModeThunk(base, buffer, r4, frame, kReturn, calls)
                          : calls.call(function, frame, kReturn, buffer, r4);
  const uint32_t w0 = Load(base, uint32_t(r3)), w1 = Load(base, uint32_t(r3) + 4);
  Store(base, answer, w0);
  Store(base, answer + 4, w1);
}

void ScreenMachineConstruct(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82107358 ScreenMachine::ScreenMachine");
  game_modes::StateMachineConstruct(base, machine);
  for (uint32_t offset = 9600; offset <= 9636; offset += 4) {
    Store(base, machine + offset, 0);
  }
}

}  // namespace boot_mode
