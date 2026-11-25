// The boot mode and the modes' other methods as plain C++ (boot_mode.h,
// docs/functions/boot-mode.md), read from the generated code.

#include "boot_mode/boot_mode.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "hooks/last_error.h"
#include "mode_tick/mode_tick.h"
#include "movies/movie.h"
#include "schema/action_mappers.h"
#include "schema/boot_mode.h"
#include "schema/movies.h"
#include "schema/racer_controllers.h"
#include "schema/type6_type7_modes.h"

namespace boot_mode {

namespace {

using G = schema::BootModeGlobals;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// A movie class slot (MovieVtable's, a tail call from the caller's frame at
// `sp`): ours where the slot holds it, else the guest's. r3 as the slot
// leaves it (ours leave the object).
using MovieSlotField = uint32_t (schema::MovieVtable::*)() const;
uint64_t MovieSlot(MovieSlotField slot, uint32_t ours,
                   void (*native)(uint32_t), uint32_t sp, uint32_t lr,
                   const Calls& calls) {
  const uint32_t movie = schema::MoviesGlobals::movie_class().address;
  const uint32_t target = (schema::Movie(movie).vtable().*slot)();
  if (calls.ffmpeg_movies && target == ours) {
    native(movie);
    return movie;
  }
  return calls.call(target, sp, lr, movie, 0);
}

// A message (id, 0, 0) queued in the ring of a mode's dispatcher (the mode's
// +36; a dual view, E5 step 4: the 852-byte modes are in host memory, their
// ring's bytes in guest order): the count first, then the message at (start +
// count - 1) mod 50.
void Queue(const schema::ModeDispatcher& owner, uint32_t id) {
  const uint32_t count = owner.ring_count() + 1;
  const uint32_t at = owner.ring_start() + count - 1;
  owner.set_ring_count(count);
  uint8_t* slot = owner.ring() + (at % kRingSlots) * kRingMessageBytes;
  schema::host::StoreU32(slot, id);
  schema::host::StoreU32(slot + 4, 0);
  schema::host::StoreU32(slot + 8, 0);
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

uint64_t MoviesPlay(uint32_t id, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A43A8 Movies::Play");
  const uint32_t path = Load(kMoviePaths + (id << 2));
  const uint32_t movie = schema::MoviesGlobals::movie_class().address;
  const uint32_t target = schema::Movie(movie).vtable().play();
  if (calls.ffmpeg_movies && target == kMoviePlay) {
    movies::movie::Play(movie, path);
    return movie;
  }
  return calls.call(target, sp, lr, movie, path);
}

uint64_t MoviesStart(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A43D8 Movies::Start");
  return MovieSlot(&schema::MovieVtable::start, kMovieStart, movies::movie::Start, sp, lr, calls);
}

uint64_t MoviesStop(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A43F0 Movies::Stop");
  return MovieSlot(&schema::MovieVtable::stop, kMovieStop, movies::movie::Stop, sp, lr, calls);
}

uint64_t MoviesPause(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A4408 Movies::Pause");
  return MovieSlot(&schema::MovieVtable::pause, kMoviePause, movies::movie::Pause, sp, lr, calls);
}

uint64_t MoviesResume(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A4420 Movies::Resume");
  return MovieSlot(&schema::MovieVtable::resume, kMovieResume, movies::movie::Resume, sp, lr, calls);
}

uint64_t MoviesRelease(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A4438 Movies::Release");
  return MovieSlot(&schema::MovieVtable::release, kMovieRelease, movies::movie::Release, sp, lr, calls);
}

uint64_t MoviesState(uint32_t frame, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820A4450 Movies::State");
  if (calls.finished_early && calls.finished_early()) {
    return 1;
  }
  const uint32_t movie = schema::MoviesGlobals::movie_class().address;
  const uint32_t target = schema::Movie(movie).vtable().state();
  uint32_t state;
  if (calls.ffmpeg_movies && target == kMovieGetState) {
    state = movies::movie::GetState(movie);
  } else {
    state = uint32_t(calls.call(target, frame, 0x820A4474, movie, 0));
  }
  if (state <= 3) {
    return state;
  }
  return Load(frame + 80);  // never written: the stack's word
}

// --- The boot machine's handlers ---

uint64_t StartMovie(uint32_t frame, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82107B90 Boot::StartMovie");
  // sub_825894C8, XMP's playback taken: Dropped (system-services.md), only
  // the last error it clears.
  last_error::Set(calls.pcr, 0);
  const uint32_t id = Load(G::movie_ids_address() + (G::movie_index() << 2));
  MoviesPlay(id, frame, 0x82107BBC, calls);
  const uint32_t passes = G::start_fade_passes();
  G::set_movie_index(G::movie_index() + 1);
  mode_tick::ScreenFadesFadeIn(passes, 0, 0, 0, 0);
  return passes;
}

uint64_t WaitForMovie(uint32_t machine, uint32_t dispatcher, uint32_t frame,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82107C58 Boot::WaitForMovie");
  const schema::BootMachine boot(machine);
  bool end = false;
  if (G::skippable() != 0) {
    using RC = schema::RacerControllersGlobals;
    const schema::PlayerMapperEntry entry(RC::mapper_entries_address() + (Load(RC::player_entries_address()) << 3));
    if (entry.unread() != 1) {
      const uint32_t mapper = entry.mapper().address;
      const uint32_t target = Load(Load(mapper) + 40);
      if ((calls.call(target, frame, 0x82107CC0, mapper, kSkipAction) & 0xFF) != 0) {
        end = true;
      }
    }
  }
  const uint32_t state_frame = frame - kStateFrame;
  if (int32_t(MoviesState(state_frame, calls)) == 1 && boot.movie_started() == 1) {
    end = true;
  }
  if (end) {
    mode_tick::ScreenFadesFadeOut(1, 0, 0, 0, 0);
    Queue(schema::ModeDispatcher(dispatcher), 1);
  }
  uint64_t r3 = MoviesState(state_frame, calls);
  if (int32_t(r3) == 1 && boot.movie_started() == 0) {
    r3 = MoviesStart(frame, 0x82107D70, calls);
    boot.set_movie_started(1);
  }
  return r3;
}

uint64_t EndMovie(uint32_t frame, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82107BF8 Boot::EndMovie");
  MoviesStop(frame, 0x82107C08, calls);
  MoviesRelease(frame, 0x82107C0C, calls);
  // sub_825895A0, XMP's playback handed back: Dropped (system-services.md),
  // only the last error it clears and its answer, 0.
  last_error::Set(calls.pcr, 0);
  return 0;
}

// --- The boot mode ---

uint32_t MachineInit(uint32_t machine, uint32_t frame, bool* replaced) {
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
        Store(at(step.a), at(step.b));
        break;
      case Op::kCopy6:
        for (uint32_t i = 0; i < 6; i++) {
          Store(at(step.b) + 4 * i, Load(at(step.a) + 4 * i));
        }
        break;
      case Op::kAdd:
        r3 = game_modes::StateMachineAdd(machine, at(step.a), at(step.b), replaced);
        break;
    }
  }
  schema::BootMachine(machine).set_movie_started(0);
  return r3;
}

uint32_t Init(uint32_t mode, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82107F68 BootMode::Init");
  const schema::BootMode boot(mode);
  const uint32_t machine = boot.machine_address();
  const uint32_t r3 = MachineInit(machine, frame - kMachineInitFrame, replaced);
  // The delegate's function and adjustment, built at the frame's +80.
  Store(frame + 84, 0);
  const uint32_t count = boot.handler_count();
  Store(frame + 80, kNextModeThunk);
  boot.set_handler_count(count + 1);
  Store(boot.handlers_address() + (count << 2), machine);
  const schema::ModeDelegate delegate(mode + 8);
  if (delegate.address != 0) {
    const uint32_t function = Load(frame + 80), adjustment = Load(frame + 84);
    delegate.set_object(mode);
    delegate.set_function(function);
    delegate.set_adjustment(adjustment);
    delegate.set_vtable(kDelegateVtable);
  }
  boot.set_delegate_ptr(delegate);
  G::set_mode_asked(kNoModeAsked);
  G::set_movie_paused(0);
  return r3;
}

void NextMode(uint32_t answer, uint32_t mode, uint32_t frame, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82108018 BootMode::NextMode");
  const schema::ModeAnswer out(answer);
  const schema::BootMode boot(mode);
  const uint32_t blocked0 = G::pad_blocked0();
  out.set_next_type(0);
  const uint32_t was_paused = G::movie_paused();
  out.set_changed(0);
  uint32_t paused;
  if ((blocked0 != 0 || G::pad_blocked1() != 0) && G::skippable() != 0) {
    paused = 1;
  } else {
    paused = G::mode_asked() == kNoModeAsked ? 0 : 1;
  }
  G::set_movie_paused(paused);
  if (was_paused != paused) {
    if (paused != 0) {
      MoviesPause(frame, 0x821080AC, calls);
    } else {
      MoviesResume(frame, 0x821080B4, calls);
    }
  }
  Queue(schema::ModeDispatcher(mode + 36), 0);  // the boot mode's dispatcher
  // GameMessages::Dispatch (its override, src/frame_pacing/game_messages.cpp).
  calls.call(0x821062E8, frame, 0x82108108, boot.handlers_address(), mode);
  if (boot.machine_state() != 2) {
    return;
  }
  uint32_t type = G::mode_asked();
  if (type == kNoModeAsked) {
    const uint32_t index = G::movie_index();
    if (index >= 5 || Load(G::movie_ids_address() + (index << 2)) == kEndOfList) {
      type = G::next_type();
    } else {
      type = 3;
    }
  }
  // The answer built at the frame's +80 (bytes +81 to +83 not written: the
  // stack's), then its 8 bytes copied.
  Store(frame + 84, type);
  guest::StoreU8(frame + 80, 1);
  const uint32_t w0 = Load(frame + 80), w1 = Load(frame + 84);
  out.set_word0(w0);
  out.set_next_type(w1);
}

void Destruct(uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82106D08 BootMode::~BootMode");
  const schema::BootMode boot(mode);
  boot.set_vtable(kBootModeVtable);
  boot.set_vtable32(kBootModeVtable32);
  // sub_82106540 on +848 (the state machine needs nothing): empty.
  boot.set_vtable(kModeVtable);
  boot.set_vtable32(kModeVtable32);
}

void DeletingDestructor(uint32_t mode, uint32_t flags, uint32_t frame,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82106D68 BootMode::DeletingDestructor");
  Destruct(mode);
  if (flags & 1) {
    calls.call(kOperatorDelete, frame, 0x82106D98, mode, flags);
  }
}

void ModeDeletingDestructor(uint32_t mode, uint32_t flags, uint32_t frame,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82106E38 Mode::DeletingDestructor");
  const schema::GameMode game_mode(mode);
  game_mode.set_vtable(kModeVtable);
  game_mode.set_vtable32(kModeVtable32);
  if (flags & 1) {
    calls.call(kOperatorDelete, frame, 0x82106E70, mode, flags);
  }
}

uint64_t NextModeThunk(uint64_t answer, uint64_t mode, uint32_t sp, uint32_t lr,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82235C08 Mode::NextModeThunk");
  const uint32_t target =
      schema::GameModeVtable(schema::GameMode(uint32_t(mode)).vtable()).next_mode();
  if (target == kNextMode) {
    NextMode(uint32_t(answer), uint32_t(mode), sp - kNextModeFrame, calls);
    return answer;
  }
  return calls.call(target, sp, lr, answer, mode);
}

void ModeDelegateInvoke(uint32_t answer, uint32_t delegate, uint32_t frame,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223D440 ModeDelegate::Invoke");
  const schema::ModeDelegate view(delegate);
  const uint32_t adjustment = view.adjustment();
  const uint32_t object = view.object();
  const uint32_t function = view.function();
  const uint64_t r4 = uint64_t(adjustment) + uint64_t(object);
  const uint32_t buffer = frame + 80;
  constexpr uint32_t kReturn = 0x8223D470;
  const uint64_t r3 = function == kNextModeThunk
                          ? NextModeThunk(buffer, r4, frame, kReturn, calls)
                          : calls.call(function, frame, kReturn, buffer, r4);
  const uint32_t w0 = Load(uint32_t(r3)), w1 = Load(uint32_t(r3) + 4);
  const schema::ModeAnswer out(answer);
  out.set_word0(w0);
  out.set_next_type(w1);
}

void ScreenMachineConstruct(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82107358 ScreenMachine::ScreenMachine");
  game_modes::StateMachineConstruct(machine);
  const schema::ScreenMachine screen(machine);
  screen.set_transition_count(0);
  screen.set_state(0);
  screen.set_loaded(0);
  screen.set_reenter(0);
  screen.set_delete_movie(0);
  screen.set_word_9620(0);
  screen.set_word_9624(0);
  screen.set_word_9628(0);
  screen.set_word_9632(0);
  screen.set_word_9636(0);
}

}  // namespace boot_mode
