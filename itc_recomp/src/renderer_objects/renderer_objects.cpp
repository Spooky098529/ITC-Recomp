// The renderer and the app's scene (renderer_objects.h,
// docs/functions/renderer.md): step for step as the recompiled functions,
// the handler map and request vector as the framework's plain C++
// (src/framework/run_loop.h), the guest calls through the adapters.

#include "renderer_objects/renderer_objects.h"

#include "guest.h"

#include "base/log.h"
#include "framework/run_loop.h"
#include "graphics/video_output.h"

namespace renderer_objects {

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
  ITC_NATIVE_RUNNING("sub_824960E0 Renderer::GetVideoOutput");
  return object + 44;
}

std::optional<bool> AddListener(uint8_t* base, uint32_t interface, uint32_t key,
                                uint32_t delegate, uint32_t request,
                                const app_states::AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820922B8 Renderer::AddListener");
  Store(base, request + 0, 0);
  Store(base, request + 4, Load(base, key));
  Store(base, request + 8, calls.clone(delegate, 0x820922F0));
  return framework::RequestsPushBack(base, interface + (kRendererRequests - kRendererInterface),
                                     request);
}

std::optional<bool> ApplyRequest(uint8_t* base, uint32_t renderer, uint32_t type, uint32_t key,
                                 uint32_t delegate) {
  if (type == 1) {
    return std::nullopt;  // a listener's removal: never seen in play
  }
  if (type == 0 &&
      !framework::MapInsertUniqueOf(base, renderer + kRendererListeners, key, delegate)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82091FF0 Renderer::ApplyRequest");
  return type == 0;
}

uint32_t CallListener(uint8_t* base, uint32_t renderer, uint64_t entry, uint32_t frame,
                      const app_states::AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82091A98 Renderer::CallListener");
  Store64(base, frame + 96 + 24, entry);
  const uint32_t interface = renderer != 0 ? renderer + kRendererInterface : 0;
  const uint32_t device = Load(base, renderer + kRendererDevice);
  const uint32_t delegate = Load(base, frame + 96 + 28);
  const uint32_t target = app_states::VirtualTarget(base, delegate, 4);
  return calls.call(target, delegate, interface, uint64_t(device), 0x82091AD4);
}

void DrawFrame(uint8_t* base, uint32_t renderer, uint32_t frame,
               const app_states::AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82091E58 Renderer::DrawFrame");
  namespace vo = graphics::video_output;
  const uint32_t video_output = renderer + kRendererVideoOutput;
  const uint32_t device = vo::LockDevice(base, video_output);
  const uint32_t map = renderer + kRendererListeners;
  Store(base, frame + 104, renderer);
  Store(base, frame + 84, 0);
  Store(base, frame + 80, kCallListener);
  const uint64_t function = Load64(base, frame + 80);
  Store(base, renderer + kRendererDevice, device);
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
  calls.set_register(29, renderer + kRendererDevice);
  calls.set_register(30, video_output);
  calls.set_register(31, renderer);
  calls.in_frame(112, 0x82091EE4, [&](uint32_t inner) {
    app_states::HandlerMapForEach(base, frame + 96, first, last, functor, adjust, inner, calls);
  });
  vo::UnlockDevice(base, video_output, renderer + kRendererDevice);
}

}  // namespace renderer_objects
