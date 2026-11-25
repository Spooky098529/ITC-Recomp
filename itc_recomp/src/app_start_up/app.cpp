// The app's constructor natively (start_up.h, docs/functions/app-start-up.md:
// App::App).

#include "app_start_up/start_up.h"

#include "base/flags.h"
#include "base/log.h"
#include "framework/framework.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "schema/app_start_up.h"
#include "schema/app_states.h"
#include "schema/cameras.h"
#include "schema/movies.h"

ITC_FLAG_DECLARE(bool, native_app_start_up);
ITC_FLAG_DECLARE(bool, native_framework);

namespace app_start_up {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

constexpr uint32_t kHandlerMap = 0x82099FB0;   // HandlerMap::HandlerMap
constexpr uint32_t kStatesCreate = 0x820AE498;  // AppStates::Create
constexpr uint32_t kAppCamera = 0x820A8B88;    // AppCamera::AppCamera
constexpr uint32_t kMovie = 0x820AEA88;        // Movie::Movie
constexpr uint32_t kAudioManagerCreate = 0x82119B38;
constexpr uint32_t kStartUp = 0x82106548;      // App::StartUp
constexpr uint32_t kMemberDelegateVtable = 0x8200833C;
constexpr uint32_t kSoundName = 0x82000EAC;    // "txr_xact"

}  // namespace

uint32_t AppConstruct(uint32_t app, uint32_t block, uint32_t frame,
                      const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82094590 App::App");
  const schema::App self(app);
  // The base classes' vtables, then the app's own.
  self.set_vtable_4(0x82000ABC);
  self.set_vtable_8(0x82000A74);
  self.set_vtable_12(0x82000D48);
  self.set_vtable_16(0x82000DD0);
  self.set_interface_vtable(0x82000DA0);
  self.set_vtable_12(0x82000E70);
  self.set_vtable(0x82000EA0);
  self.set_vtable_4(0x82000E94);
  self.set_vtable_8(0x82000E80);
  self.set_vtable_16(0x82000E38);
  self.set_interface_vtable(0x82000E08);
  // The four handler maps (r30 still the +12 vtable).
  calls.call(kHandlerMap, self.handlers_map().address, 0x82000E80, 0x82094644, 0x82000E70,
             {});  // r4 as left
  calls.call(kHandlerMap, self.toggled_handlers_map().address, {}, 0x8209464C, 0x82000E70, {});
  calls.call(kHandlerMap, self.always_handlers_map().address, {}, 0x82094654, 0x82000E70, {});
  calls.call(kHandlerMap, self.listeners_map().address, {}, 0x8209465C, 0x82000E70, {});
  // The tick handler (key 8, App::Update) on the run loop's slot 2.
  Store(frame + 116, app);
  Store(frame + 84, 0);
  Store(frame + 112, kMemberDelegateVtable);
  Store(frame + 80, 0x82093DC0);
  self.set_messages_first(0u);
  self.set_messages_last(0u);
  self.set_messages_end(0u);
  self.set_requests_first(0);
  self.set_requests_last(0);
  self.set_requests_end(0);
  Store(frame + 120, 0x82093DC0);
  Store(frame + 124, 0);
  Store(frame + 80, 8);
  self.set_toggled(0);
  self.set_unknown_114(0);
  self.set_step(0);
  const schema::AppSetUpBlock set_up(block);
  self.set_framework_input(set_up.framework_interface().address);
  calls.virtual_call(set_up.run_loop().address, 8, frame + 80, frame + 112, 0x820946D8);
  // The run-loop listener (key 9, App::OnRunLoopEvent) on its slot 1.
  Store(frame + 84, 0);
  Store(frame + 132, app);
  Store(frame + 80, 0x82092DE0);
  Store(frame + 112, 0x82000ABC);
  Store(frame + 128, kMemberDelegateVtable);
  Store(frame + 136, 0x82092DE0);
  Store(frame + 140, 0);
  Store(frame + 80, 9);
  calls.virtual_call(set_up.run_loop().address, 4, frame + 80, frame + 128, 0x82094730);
  // The state stack, from {the app} on the frame.
  Store(frame + 88, app);
  Store(frame + 128, 0x82000ABC);
  const uint32_t states = calls.call(kStatesCreate, frame + 88, {}, 0x82094748, 0, {});
  // The app's interfaces (app + N).
  Store(frame + 96, app + 20);
  Store(frame + 100, app + 4);
  self.set_states(states);
  // The second camera class, from {app + 20, app + 4}.
  uint32_t camera = 0;
  // In host memory (E5 step 4; operator new's block before, 0x82094760).
  static_assert(schema::AppCamera::kSize == kAppCameraBytes);
  const uint32_t camera_at = schema::AppCamera::New().address;
  if (camera_at != 0) {
    camera = calls.call(kAppCamera, camera_at, frame + 96, 0x82094770, 0, {});
  }
  // The movie class, from {app + 20, app + 8}.
  Store(frame + 104, app + 20);
  Store(frame + 108, app + 8);
  self.set_camera(camera);
  uint32_t movie = 0;
  // In host memory (E5 step 4; operator new's block before, 0x82094790).
  static_assert(schema::Movie::kSize == kMovieBytes);
  const uint32_t movie_at = schema::Movie::New().address;
  if (movie_at != 0) {
    movie = calls.call(kMovie, movie_at, frame + 104, 0x820947A0, 0, {});
  }
  self.set_movie(movie);
  // The sound manager, "txr_xact" with 3 banks (the count on the frame).
  Store(frame + 80, 3);
  const uint32_t sound =
      calls.call(kAudioManagerCreate, app + 12, kSoundName, 0x820947C4, movie,
                 frame + 80);
  self.set_sound_manager(sound);
  calls.call(kStartUp, app + 16, {}, 0x820947D4, movie, {});
  return app;
}

std::optional<DirectSlot> RenderManagerCall(uint32_t function, uint32_t manager) {
  constexpr uint32_t kSceneFrom36 = 0x82090908, kRendererFrom36 = 0x820908E0;
  if (!ITC_FLAG(native_app_start_up) || !ITC_FLAG(native_framework)) {
    return std::nullopt;
  }
  if (function == kRenderManagerScene) {
    const SlotCall call = SceneSlotFromInterface16(manager);
    if (call.target != kSceneFrom36) {
      return std::nullopt;
    }
    return DirectSlot{framework::SceneFrom36(call.object), call.target};
  }
  if (function == kRenderManagerFrameDriver) {
    const SlotCall call = FrameDriverSlotFromInterface16(manager);
    if (call.target != kRendererFrom36) {
      return std::nullopt;
    }
    return DirectSlot{framework::RendererFrom36(call.object), call.target};
  }
  return std::nullopt;
}

std::optional<uint32_t> RenderManagerScene(uint32_t manager) {
  const auto slot = RenderManagerCall(Load(schema::AppInterface16(manager).vtable() + 32), manager);
  if (!slot) {
    return std::nullopt;
  }
  return slot->r3;
}

std::optional<uint32_t> RenderManagerFrameDriver(uint32_t manager) {
  const auto slot = RenderManagerCall(Load(schema::AppInterface16(manager).vtable() + 28), manager);
  if (!slot) {
    return std::nullopt;
  }
  return slot->r3;
}

}  // namespace app_start_up

namespace app_start_up {

std::optional<uint32_t> AppCreate(uint32_t block,
                                  const std::function<uint32_t(uint32_t app)>& construct) {
  static_assert(schema::App::kSize == kAppBytes);
  const auto app = std::optional<uint32_t>(schema::App::New().address);  // host memory (E5 step 4)
  if (!app) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82094AB8 App::Create");
  (void)block;
  const uint32_t made = *app != 0 ? construct(*app) : 0;
  return made != 0 ? made + 20 : 0;
}

uint32_t InterfaceWord108(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82379338 (the word at +108)");
  return schema::AppInterface16(object).sound_manager();
}

uint32_t InterfaceWord112(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82379350 (the word at +112)");
  return schema::AppInterface16(object).movie();
}

SlotCall AppPoolSlot(uint32_t interface8) {
  ITC_NATIVE_RUNNING("sub_82094368 (the framework interface's slot 4)");
  const auto object = schema::AppInterface8(interface8).framework_interface();
  return {object.address, object.vtable().slot_4_worker_pool()};
}

SlotCall AppPoolSlotFromInterface16(uint32_t interface16) {
  ITC_NATIVE_RUNNING("sub_820944A8 (this - 8, then sub_82094368)");
  return AppPoolSlot(interface16 - 8);
}

SlotCall SceneSlot(uint32_t interface8) {
  ITC_NATIVE_RUNNING("sub_82094338 App::SceneFromInterface8");
  const auto object = schema::AppInterface8(interface8).framework_interface();
  return {object.address, object.vtable().slot_3_scene()};
}

SlotCall SceneSlotFromInterface16(uint32_t interface16) {
  ITC_NATIVE_RUNNING("sub_820944D8 App::SceneFromInterface16");
  return SceneSlot(interface16 - 8);
}

SlotCall FrameDriverSlot(uint32_t interface4) {
  ITC_NATIVE_RUNNING("sub_82094320 App::FrameDriver");
  // interface4 is app + 4: the App's framework_input (app + 24).
  const uint32_t object = schema::App(interface4 - 4).framework_input().address;
  return {object, schema::AppFrameworkInterface(object).vtable().slot_2_frame_driver()};
}

SlotCall FrameDriverSlotFromInterface16(uint32_t interface16) {
  ITC_NATIVE_RUNNING("sub_82094498 App::FrameDriverFromInterface16");
  return FrameDriverSlot(interface16 - 12);
}

SlotCall FrameDriverSlotFromInterface8(uint32_t interface8) {
  ITC_NATIVE_RUNNING("sub_82094480 App::FrameDriverFromInterface8");
  return FrameDriverSlot(interface8 - 4);
}

}  // namespace app_start_up
