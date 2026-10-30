// The app's constructor natively (start_up.h, docs/functions/app-start-up.md:
// App::App).

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

namespace app_start_up {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kHandlerMap = 0x82099FB0;   // HandlerMap::HandlerMap
constexpr uint32_t kStatesCreate = 0x820AE498;  // AppStates::Create
constexpr uint32_t kOperatorNew = 0x825A6880;
constexpr uint32_t kAppCamera = 0x820A8B88;    // AppCamera::AppCamera
constexpr uint32_t kMovie = 0x820AEA88;        // Movie::Movie
constexpr uint32_t kAudioManagerCreate = 0x82119B38;
constexpr uint32_t kStartUp = 0x82106548;      // App::StartUp
constexpr uint32_t kMemberDelegateVtable = 0x8200833C;
constexpr uint32_t kSoundName = 0x82000EAC;    // "txr_xact"

}  // namespace

uint32_t AppConstruct(uint8_t* base, uint32_t app, uint32_t block, uint32_t frame,
                      const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82094590 App::App");
  // The base classes' vtables, then the app's own.
  Store(base, app + 4, 0x82000ABC);
  Store(base, app + 8, 0x82000A74);
  Store(base, app + 12, 0x82000D48);
  Store(base, app + 16, 0x82000DD0);
  Store(base, app + 20, 0x82000DA0);
  Store(base, app + 12, 0x82000E70);
  Store(base, app + 0, 0x82000EA0);
  Store(base, app + 4, 0x82000E94);
  Store(base, app + 8, 0x82000E80);
  Store(base, app + 16, 0x82000E38);
  Store(base, app + 20, 0x82000E08);
  // The four handler maps (r30 still the +12 vtable).
  calls.call(kHandlerMap, app + 28, 0x82000E80, 0x82094644, 0x82000E70, {});  // r4 as left
  calls.call(kHandlerMap, app + 40, {}, 0x8209464C, 0x82000E70, {});
  calls.call(kHandlerMap, app + 52, {}, 0x82094654, 0x82000E70, {});
  calls.call(kHandlerMap, app + 64, {}, 0x8209465C, 0x82000E70, {});
  // The tick handler (key 8, App::Update) on the run loop's slot 2.
  Store(base, frame + 116, app);
  Store(base, frame + 84, 0);
  Store(base, frame + 112, kMemberDelegateVtable);
  Store(base, frame + 80, 0x82093DC0);
  for (const uint32_t at : {80u, 84u, 88u, 100u, 104u, 108u}) {
    Store(base, app + at, 0);
  }
  Store(base, frame + 120, 0x82093DC0);
  Store(base, frame + 124, 0);
  Store(base, frame + 80, 8);
  guest::StoreU8(base, app + 112, 0);
  guest::StoreU8(base, app + 114, 0);
  guest::StoreU8(base, app + 113, 0);
  Store(base, app + 24, Load(base, block + 4));
  calls.virtual_call(Load(base, block + 0), 8, frame + 80, frame + 112, 0x820946D8);
  // The run-loop listener (key 9, App::OnRunLoopEvent) on its slot 1.
  Store(base, frame + 84, 0);
  Store(base, frame + 132, app);
  Store(base, frame + 80, 0x82092DE0);
  Store(base, frame + 112, 0x82000ABC);
  Store(base, frame + 128, kMemberDelegateVtable);
  Store(base, frame + 136, 0x82092DE0);
  Store(base, frame + 140, 0);
  Store(base, frame + 80, 9);
  calls.virtual_call(Load(base, block + 0), 4, frame + 80, frame + 128, 0x82094730);
  // The state stack, from {the app} on the frame.
  Store(base, frame + 88, app);
  Store(base, frame + 128, 0x82000ABC);
  const uint32_t states = calls.call(kStatesCreate, frame + 88, {}, 0x82094748, 0, {});
  Store(base, frame + 96, app + 20);
  Store(base, frame + 100, app + 4);
  Store(base, app + 116, states);
  // The second camera class, from {app + 20, app + 4}.
  uint32_t camera = 0;
  const auto camera_block = memory::crt::OperatorNew(base, kAppCameraBytes);
  const uint32_t camera_at =
      camera_block ? *camera_block : calls.call(kOperatorNew, kAppCameraBytes, {}, 0x82094760, 0, {});
  if (camera_at != 0) {
    camera = calls.call(kAppCamera, camera_at, frame + 96, 0x82094770, 0, {});
  }
  // The movie class, from {app + 20, app + 8}.
  Store(base, frame + 104, app + 20);
  Store(base, frame + 108, app + 8);
  Store(base, app + 120, camera);
  uint32_t movie = 0;
  const auto movie_block = memory::crt::OperatorNew(base, kMovieBytes);
  const uint32_t movie_at =
      movie_block ? *movie_block : calls.call(kOperatorNew, kMovieBytes, {}, 0x82094790, 0, {});
  if (movie_at != 0) {
    movie = calls.call(kMovie, movie_at, frame + 104, 0x820947A0, 0, {});
  }
  Store(base, app + 128, movie);
  // The sound manager, "txr_xact" with 3 banks (the count on the frame).
  Store(base, frame + 80, 3);
  const uint32_t sound =
      calls.call(kAudioManagerCreate, app + 12, kSoundName, 0x820947C4, movie, frame + 80);
  Store(base, app + 124, sound);
  calls.call(kStartUp, app + 16, {}, 0x820947D4, movie, {});
  return app;
}

}  // namespace app_start_up
