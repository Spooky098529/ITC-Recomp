// The frame driver and the app's scene (frame_driver.h,
// docs/functions/frame-driver.md): step for step as the recompiled functions,
// the handler map and request vector as the framework's plain C++
// (src/framework/run_loop.h), the guest calls through the adapters.

#include "frame_driver/frame_driver.h"

#include "guest.h"

#include "base/log.h"
#include "command_list/command_list.h"
#include "framework/run_loop.h"
#include "graphics/video_output.h"

namespace frame_driver {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

}  // namespace

uint32_t GetVideoOutput(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_824960E0 FrameDriver::GetVideoOutput");
  return object + 44;
}

std::optional<bool> AddListener(uint8_t* base, uint32_t interface, uint32_t key,
                                uint32_t delegate, uint32_t request,
                                const app_states::AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820922B8 FrameDriver::AddListener");
  Store(base, request + 0, 0);
  Store(base, request + 4, Load(base, key));
  Store(base, request + 8, calls.clone(delegate, 0x820922F0));
  return framework::RequestsPushBack(base, interface + (kDriverRequests - kDriverInterface),
                                     request);
}

std::optional<bool> ApplyRequest(uint8_t* base, uint32_t driver, uint32_t type, uint32_t key,
                                 uint32_t delegate) {
  if (type == 1) {
    return std::nullopt;  // a listener's removal: never seen in play
  }
  if (type == 0 &&
      !framework::MapInsertUniqueOf(base, driver + kDriverListeners, key, delegate)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82091FF0 FrameDriver::ApplyRequest");
  return type == 0;
}

uint32_t CallListener(uint8_t* base, uint32_t driver, uint64_t entry, uint32_t frame,
                      const app_states::AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82091A98 FrameDriver::CallListener");
  Store64(base, frame + 96 + 24, entry);
  const uint32_t interface = driver != 0 ? driver + kDriverInterface : 0;
  const uint32_t device = Load(base, driver + kDriverDevice);
  const uint32_t delegate = Load(base, frame + 96 + 28);
  const uint32_t target = app_states::VirtualTarget(base, delegate, 4);
  return calls.call(target, delegate, interface, uint64_t(device), 0x82091AD4);
}

void DrawFrame(uint8_t* base, uint32_t driver, uint32_t frame,
               const app_states::AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82091E58 FrameDriver::DrawFrame");
  namespace vo = graphics::video_output;
  const uint32_t video_output = driver + kDriverVideoOutput;
  const uint32_t device = vo::LockDevice(base, video_output);
  const uint32_t map = driver + kDriverListeners;
  Store(base, frame + 104, driver);
  Store(base, frame + 84, 0);
  Store(base, frame + 80, kCallListener);
  const uint64_t function = Load64(base, frame + 80);
  Store(base, driver + kDriverDevice, device);
  const uint32_t head = Load(base, map + framework::kMapHead);
  Store(base, frame + 80, map);
  Store64(base, frame + 96, function);
  const uint64_t functor = Load64(base, frame + 96);
  const uint64_t adjust = Load64(base, frame + 104);  // its low word as the stack holds it
  Store(base, frame + 96, map);
  const uint32_t left = Load(base, head + framework::kNodeLeft);
  Store(base, frame + 84, head);
  const uint64_t last = Load64(base, frame + 80);
  Store(base, frame + 100, left);
  const uint64_t first = Load64(base, frame + 96);
  calls.set_register(29, driver + kDriverDevice);
  calls.set_register(30, video_output);
  calls.set_register(31, driver);
  calls.in_frame(112, 0x82091EE4, [&](uint32_t inner) {
    app_states::HandlerMapForEach(base, frame + 96, first, last, functor, adjust, inner, calls);
  });
  vo::UnlockDevice(base, video_output, driver + kDriverDevice);
}

void FlipPrimitiveWord(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82591838 the primitive word's flip");
  Store(base, kPrimitiveFlip, Load(base, kPrimitiveFlip) == 1 ? 0 : 1);
}

void SceneUpdate(uint8_t* base, uint32_t scene, const app_states::AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820988C8 Scene::Update");
  const uint32_t list = scene + kSceneCommandList;
  command_list::Flip(base, list);
  command_list::BeginTick(base, list);
  FlipPrimitiveWord(base);
  const uint32_t queue = scene + kSceneRenderQueue;
  calls.set_register(30, list);
  calls.set_register(31, queue);
  calls.call(app_states::VirtualTarget(base, queue, 24), queue, std::nullopt, std::nullopt,
             0x82098910);
  calls.call(app_states::VirtualTarget(base, queue, 8), queue, std::nullopt, std::nullopt,
             0x82098924);
}

void VideoOutputBeginFrame(uint8_t* base, uint32_t video_output, const SetGpr& set_gpr) {
  ITC_NATIVE_RUNNING("sub_8258D958 VideoOutput::BeginFrame");
  set_gpr(Load(base, video_output + graphics::video_output::kDevice), 32, 96);
  Store(base, kVblankCount, 0);
}

uint32_t SceneDraw(uint8_t* base, uint32_t scene, uint32_t interface, uint32_t device,
                   const app_states::AppCalls& calls, const SetGpr& set_gpr) {
  ITC_NATIVE_RUNNING("sub_82098940 Scene::Draw");
  calls.set_register(29, device);
  calls.set_register(30, scene);
  const uint32_t target = app_states::VirtualTarget(base, interface, 12);
  const uint32_t video_output =
      target == kGetVideoOutput
          ? GetVideoOutput(interface)
          : calls.call(target, interface, std::nullopt, std::nullopt, 0x82098968);
  calls.set_register(31, video_output);
  VideoOutputBeginFrame(base, video_output, set_gpr);
  const uint32_t queue = scene + kSceneRenderQueue;
  calls.call(app_states::VirtualTarget(base, queue, 20), queue, uint64_t(device),
             uint64_t(video_output), 0x82098994);
  return video_output;
}

}  // namespace frame_driver
