// The race's loading's handlers (docs/functions/race-loading.md): see
// race_loading.h.

#include "race_loading/race_loading.h"

#include "archive/archive.h"
#include "base/log.h"
#include "guest.h"
#include "course_scenery/course_scenery.h"
#include "data_tables/data_tables.h"
#include "debug/state_digest.h"
#include "game_modes/game_modes.h"
#include "input/pad_ticks.h"
#include "mode_tick/mode_tick.h"
#include "schema/boot_mode.h"
#include "schema/car_draw.h"
#include "schema/course_scenery.h"
#include "schema/game_modes.h"
#include "schema/race_loading.h"
#include "schema/scene.h"
#include "schema/ui_movies_game.h"
#include "scene/scene.h"
#include "strings_and_font/strings_and_font.h"

namespace race_loading {

namespace {

using G = schema::RaceLoadingGlobals;
using Machine = schema::CourseSetup;
using Chunk = schema::UiMovieSceneChunk;

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
  const schema::ModeDispatcher d(base, dispatcher);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t at = d.ring_start() + count - 1;
  d.set_ring_count(count);
  const game_modes::RingSlot slot{d.ring() + (at % kRingSlots) * schema::ModeMessage::kSize};
  slot.set_id(message);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

// The course scenery's calls (course_scenery::Calls) through these.
course_scenery::Calls SceneryCalls(const Calls& calls) {
  return {const_cast<Calls*>(&calls),
          [](void* user, uint32_t function, const uint32_t* args, int count, uint32_t frame,
             uint32_t lr) {
            const Calls& c = *static_cast<const Calls*>(user);
            uint64_t registers[8] = {};
            for (int i = 0; i < count && i < 8; i++) {
              registers[i] = args[i];
            }
            return uint32_t(c.call(c.user, function, registers, count, frame, lr));
          }};
}

// Scenery::IsLoaded([0x826C5514]) from the handler's frame.
uint32_t SceneryLoaded(uint8_t* base, uint32_t frame, const Calls& calls) {
  return course_scenery::SceneryIsLoaded(base, SceneryCalls(calls), frame,
                                         schema::CourseSceneryGlobals::manager(base).address,
                                         calls.native_queue);
}

}  // namespace

uint64_t WaitCourse(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821514A0 CourseSetup::WaitCourse");
  const uint32_t frame = Frame(base, sp, 112);
  if (SceneryLoaded(base, frame, calls) != 0 && int32_t(G::collision_state(base)) == 2 &&
      int32_t(G::route_state(base)) == 2 && strings_and_font::TextIsLoaded(base) != 0 &&
      data_tables::IsLoading(base) == 0) {
    Post(base, dispatcher, 3);
    Machine(base, machine).set_draw_step(0);
    return 0;
  }
  // "CRS SETUP WAITING." to the empty sub_82106540 (r3 = 500, r4 = 100, r5
  // = 0, r6 the string): nothing done.
  return 500;
}

uint64_t EndTick(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821516F0 CourseSetup::EndTick");
  const uint32_t frame = Frame(base, sp, 112);
  const Machine m(base, machine);
  m.set_end_ticks(m.end_ticks() + 1);
  uint64_t r3 = SceneryLoaded(base, frame, calls);
  if (r3 != 0) {
    r3 = scene::LoadsDone(
        base, schema::SceneGlobals::scene(base).address, frame,
        [&](uint32_t function, uint32_t at, uint32_t lr, uint32_t f3, uint32_t f4) {
          return calls.Call(function, at, lr, 2, f3, f4);
        });
    if (r3 == 1) {
      r3 = calls.native_queue ? archive::QueueLength(base)
                              : uint32_t(calls.Call(kQueueLength, frame, 0x82151744, 0));
      if (r3 == 0 && m.end_ticks() > 2 &&
          (G::layer0_fade_out(base) | G::layer0_fade_in(base)) == 0) {
        Post(base, dispatcher, 50);
        return 0;
      }
    }
  }
  return calls.Call(kCarSceneUpdate, frame, 0x821517C0, 1, r3);
}

uint64_t Start(uint8_t* base, uint32_t dispatcher, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82151408 CourseSetup::Start");
  // The original's 112-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  Post(base, dispatcher, 2);
  const uint32_t passes = G::fade_in_passes(base);
  mode_tick::ScreenFadesFadeIn(base, passes, 0, 0, 0, 0);
  return passes;
}

void Begin(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82150DE0 CourseSetup::Begin");
  const Machine m(base, machine);
  m.set_unknown_9616(0);
  m.set_unknown_9620(0);
  m.set_unknown_9608(0);
  m.set_unknown_9632(0);
  m.set_unknown_9636(0);
  m.set_unknown_9628(0);
}

void OnCourseReady(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82150DD0 CourseSetup::OnCourseReady");
  Machine(base, machine).set_unknown_9640(0);
}

uint64_t WaitDraw(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821515A0 CourseSetup::WaitDraw");
  const Machine m(base, uint32_t(r3));
  // The original's 96-byte frame (only its back chain) made only for the
  // guest call that runs from it (E5 step 6).
  if (m.draw_step() == 0) {
    const uint32_t queued =
        calls.native_queue
            ? archive::QueueLength(base)
            : uint32_t(calls.Call(kQueueLength, Frame(base, sp, 96), 0x821515C4, 0));
    if (queued == 0) {
      if (int32_t(G::layer0_alpha(base)) != 255) {
        mode_tick::ScreenFadesFadeOut(base, G::fade_out_passes(base), 0, 0, 0, 0);
      }
      m.set_draw_step(1);
    }
  } else if (int32_t(G::layer0_alpha(base)) == 255) {
    Post(base, dispatcher, 4);
    // The race starts (the pad replay's and state digest's mark,
    // docs/pad-replay.md "By tick").
    input::PadTicks_OnRaceStarted();
    debug::StateDigest_OnRaceStarted();
    m.set_unknown_9608(0);
    m.set_draw_step(0);
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
  uint32_t chunk = schema::UiMovie(base, movie).live_scene();
  for (uint32_t tag = Chunk(base, chunk).tag(); tag != kTrm; tag = Chunk(base, chunk).tag()) {
    if (tag == kTxt) {
      Chunk(base, chunk).set_frame(text);
    } else if (tag == kPrm || tag == kPic) {
      Chunk(base, chunk).set_frame(other);
    }
    chunk += Chunk(base, chunk).size();
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
  const uint32_t movie = Load(base, G::loading_movies_address() + 4 * which);
  G::set_loading_current(base, which);
  calls.Call(kResetPose, frame, 0x8224790C, 1, movie);
  if (int32_t(frames) != -1) {
    SetFrames(base, movie, frames, frames);
  }
  uint32_t current = G::loading_current(base);
  if (current == 0 || current == 2) {
    calls.Call(kSetText, frame, 0x8224795C, 8, movie, kNone, current == 0 ? 3 : 2, 1200);
    current = G::loading_current(base);
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
    current = G::loading_current(base);
  }
  calls.Call(kFinishNamed, frame, 0x82247A1C, 2, movie,
             Load(base, G::loading_animations_address() + (current << 2)));
  current = G::loading_current(base);
  calls.Call(kPlayNamed, frame, 0x82247A40, 6, movie,
             Load(base, G::loading_animations_address() + (current << 2)), 0, 0, 0, 0);
  const uint64_t r3 = calls.Call(kLoopLast, frame, 0x82247A48, 1, movie);
  G::set_loading_shown(base, 1);
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
  *which = index == -1 ? G::loading_current(base) : uint32_t(index);
  return true;
}

}  // namespace

uint64_t Run(uint8_t* base, uint64_t which, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82247898 LoadingScreen::Run");
  uint32_t index;
  if (!Which(base, which, &index)) {
    return which;
  }
  return calls.Call(kRun, sp, lr, 2, Load(base, G::loading_movies_address() + (index << 2)), 0);
}

uint64_t Finish(uint8_t* base, uint64_t which, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82247A60 LoadingScreen::Finish");
  uint32_t index;
  if (!Which(base, which, &index)) {
    return which;
  }
  return calls.Call(kFinishNamed, sp, lr, 2, Load(base, G::loading_movies_address() + (index << 2)),
                    Load(base, G::loading_animations_address() + (index << 2)));
}

uint64_t CheckSystemRequest(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82293460 CheckSystemRequest");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t request = G::system_request(base);
  if (request == 0) {
    return 0;
  }
  if (int32_t(uint32_t(calls.Call(kSystemRequestCheck, frame, 0x8229348C, 1, request))) == 0) {
    return 0;
  }
  calls.Call(kSystemRequestRun, frame, 0x8229349C, 1, G::system_request(base));
  return 1;
}

}  // namespace race_loading
