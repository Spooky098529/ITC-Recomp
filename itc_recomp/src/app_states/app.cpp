// The app's update, its interface's slots and its state stack natively
// (app.h, docs/functions/app-states.md).

#include "app_states/app.h"

#include "guest.h"

#include "app_states/containers.h"
#include "base/log.h"

namespace app_states {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }

}  // namespace

uint32_t VirtualTarget(uint8_t* base, uint32_t object, uint32_t offset) {
  return Load(base, Load(base, object) + offset);
}

uint32_t StatesIsEmpty(uint8_t* base, uint32_t states, const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820AE1A0 AppStates::IsEmpty");
  const uint32_t root = Load(base, states + kStatesRoot) + 4;
  const uint32_t target = VirtualTarget(base, root, 44);
  if (target == kLayerIsEmpty) {
    return LayerIsEmpty(base, root);
  }
  return calls.call(target, root, std::nullopt, std::nullopt, 0);
}

uint32_t AppIsRunning(uint8_t* base, uint32_t interface, const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82092928 App::IsRunning");
  const uint32_t states = Load(base, interface + kInterfaceStates);
  const uint32_t target = VirtualTarget(base, states, 12);
  uint32_t answer = 0;
  if (target == kAppStatesIsEmpty) {
    // Its guest call, if any, a tail call from this frame.
    const uint32_t root = Load(base, states + kStatesRoot) + 4;
    if (VirtualTarget(base, root, 44) == kLayerIsEmpty) {
      answer = StatesIsEmpty(base, states, calls);
    } else {
      calls.in_frame(96, 0x82092948,
                     [&](uint32_t) { answer = StatesIsEmpty(base, states, calls); });
    }
  } else {
    calls.in_frame(96, 0x82092948, [&](uint32_t) {
      answer = calls.call(target, states, std::nullopt, std::nullopt, 0x82092948);
    });
  }
  return (answer & 0xFF) == 0 ? 1 : 0;
}

std::optional<bool> AppSendMessage(uint8_t* base, uint32_t interface, uint32_t message) {
  const auto inserted = MessagesPushBack(base, interface + kInterfaceMessages, message);
  if (inserted) {
    ITC_NATIVE_RUNNING("sub_82094AB0 App::SendMessage");
  }
  return inserted;
}

}  // namespace app_states
