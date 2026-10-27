// The app's state stack and its layers natively (layers.h,
// docs/functions/app-states.md).

#include "app_states/layers.h"

#include "guest.h"

#include "app_states/containers.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "memory/c_runtime_heap.h"
#include "std_list/ptr_list.h"

namespace app_states {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The original functions, for the paths their plain C++ hands back.
constexpr uint32_t kPtrListConstruct = 0x820AD058;
constexpr uint32_t kPtrListInsert = 0x82095958;
constexpr uint32_t kOpListConstruct = 0x820AD0B8;
constexpr uint32_t kLayerPush = 0x820ADAD0;

}  // namespace

void LayerConstruct(uint8_t* base, uint32_t layer, uint32_t owner, std::optional<uint32_t> state,
                    uint32_t frame, const AppCalls& calls) {
  (void)frame;
  if (state) {
    ITC_NATIVE_RUNNING("sub_820ADBD8 StateLayer::StateLayer(state)");
  } else {
    ITC_NATIVE_RUNNING("sub_820AD118 StateLayer::StateLayer");
  }
  // The return addresses of the original's calls (with a state, or not).
  const uint32_t children_return = state ? 0x820ADC24 : 0x820AD15C;
  const uint32_t states_return = state ? 0x820ADC2C : 0x820AD164;
  const uint32_t ops_return = state ? 0x820ADC34 : 0x820AD16C;
  calls.set_register(31, layer);
  if (state) {
    calls.set_register(30, *state);
  }
  Store(base, layer + 4, kStateBaseVtable);
  Store(base, layer + 0, kLayerVtable);
  Store(base, layer + 4, kLayerStateVtable);
  Store(base, layer + kLayerOwner, owner);
  if (!std_list::Construct(base, layer + kLayerChildren)) {
    calls.call(kPtrListConstruct, layer + kLayerChildren, std::nullopt, std::nullopt,
               children_return);
  }
  if (!std_list::Construct(base, layer + kLayerStates)) {
    calls.call(kPtrListConstruct, layer + kLayerStates, std::nullopt, std::nullopt,
               states_return);
  }
  if (!OpConstruct(base, layer + kLayerOps)) {
    calls.call(kOpListConstruct, layer + kLayerOps, std::nullopt, std::nullopt, ops_return);
  }
  if (state && !LayerPush(base, layer, *state)) {
    calls.call(kLayerPush, layer, uint64_t(*state), std::nullopt, 0x820ADC40);
  }
}

void LayerAddChild(uint8_t* base, uint32_t layer, uint32_t child, uint32_t frame,
                   const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820ADA08 StateLayer::AddChild");
  calls.set_register(31, layer);
  const uint32_t value = frame + 112 + 28;  // the caller's r1 + 28
  Store(base, value, child);
  const uint32_t child_state = child + 4;
  calls.call(VirtualTarget(base, child_state, 4), child_state, uint64_t(layer), std::nullopt,
             0x820ADA38);
  const uint32_t list = layer + kLayerChildren;
  const uint32_t head = Load(base, list + std_list::kHead);
  Store(base, frame + 80, list);
  Store(base, frame + 84, head);
  if (!std_list::Insert(base, list, head, Load(base, value))) {
    calls.call(kPtrListInsert, list, (uint64_t(list) << 32) | head, uint64_t(value), 0x820ADA58);
  }
}

bool StatesAddLayer(uint8_t* base, uint32_t states, uint32_t state, uint32_t frame,
                    const AppCalls& calls) {
  (void)frame;
  const auto layer = memory::crt::OperatorNew(base, kLayerBytes);
  if (!layer) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820AE130 AppStates::AddLayer");
  calls.set_register(30, states);
  calls.set_register(29, state);
  calls.set_register(31, *layer);
  uint32_t child = 0;
  if (*layer != 0) {
    const uint32_t app = Load(base, states + 4);
    const uint32_t owner =
        calls.call(VirtualTarget(base, app, 8), app, std::nullopt, std::nullopt, 0x820AE168);
    calls.in_frame(112, 0x820AE178, [&](uint32_t inner) {
      LayerConstruct(base, *layer, owner, state, inner, calls);
    });
    child = *layer;
  }
  calls.in_frame(112, 0x820AE18C, [&](uint32_t inner) {
    LayerAddChild(base, Load(base, states + kStatesRoot), child, inner, calls);
  });
  return true;
}

bool StatesConstruct(uint8_t* base, uint32_t states, uint32_t setup, uint32_t frame,
                     const AppCalls& calls) {
  const auto layer = memory::crt::OperatorNew(base, kLayerBytes);
  if (!layer) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820AE348 AppStates::AppStates");
  calls.set_register(31, states);
  Store(base, states + 0, kStatesVtable);
  Store(base, states + 4, Load(base, setup + 0));
  calls.set_register(30, *layer);
  uint32_t root = 0;
  if (*layer != 0) {
    const uint32_t app = Load(base, states + 4);
    const uint32_t owner =
        calls.call(VirtualTarget(base, app, 8), app, std::nullopt, std::nullopt, 0x820AE390);
    calls.in_frame(96, 0x820AE39C, [&](uint32_t inner) {
      LayerConstruct(base, *layer, owner, std::nullopt, inner, calls);
    });
    root = *layer;
  }
  const uint32_t app = Load(base, states + 4);
  Store(base, states + kStatesRoot, root);
  const uint32_t interface =
      calls.call(VirtualTarget(base, app, 4), app, std::nullopt, std::nullopt, 0x820AE3C0);
  calls.set_register(30, interface);
  // Key 8 (the map +28): AppStates::OnUpdate.
  Store(base, frame + 92, states);
  calls.set_register(29, kDelegateVtable);
  Store(base, frame + 88, kDelegateVtable);
  Store(base, frame + 96, kStatesOnUpdate);
  Store(base, frame + 80, 8);
  calls.call(VirtualTarget(base, interface, 16), interface, uint64_t(frame + 80),
             uint64_t(frame + 88), 0x820AE400);
  // Key 2 (the map +40): AppStates::OnUpdateToggled; the first delegate's
  // vtable left as the base's (its destructor).
  Store(base, frame + 104, kDelegateVtable);
  calls.set_register(28, kDelegateBaseVtable);
  Store(base, frame + 108, states);
  Store(base, frame + 80, 2);
  Store(base, frame + 88, kDelegateBaseVtable);
  Store(base, frame + 112, kStatesOnUpdateToggled);
  calls.call(VirtualTarget(base, interface, 12), interface, uint64_t(frame + 80),
             uint64_t(frame + 104), 0x820AE444);
  // Listener 9 (the map +64): AppStates::OnMessage.
  Store(base, frame + 104, kDelegateBaseVtable);
  Store(base, frame + 124, states);
  Store(base, frame + 120, kDelegateVtable);
  Store(base, frame + 128, kStatesOnMessage);
  Store(base, frame + 80, 9);
  calls.call(VirtualTarget(base, interface, 4), interface, uint64_t(frame + 80),
             uint64_t(frame + 120), 0x820AE488);
  return true;
}

std::optional<uint32_t> StatesCreate(uint8_t* base, uint32_t setup, uint32_t frame,
                                     const AppCalls& calls) {
  (void)frame;
  const auto block = memory::crt::OperatorNew(base, kStatesBytes);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820AE498 AppStates::Create");
  calls.set_register(31, setup);
  if (*block == 0) {
    return 0;
  }
  bool ours = false;
  calls.in_frame(176, 0x820AE4C4, [&](uint32_t inner) {
    ours = StatesConstruct(base, *block, setup, inner, calls);
  });
  if (!ours) {
    calls.call(0x820AE348, *block, uint64_t(setup), std::nullopt, 0x820AE4C4);
  }
  return *block;
}

std::optional<uint32_t> AppStart(uint8_t* base, uint32_t interface, uint32_t frame,
                                 const AppCalls& calls) {
  (void)frame;
  const auto holder = memory::crt::OperatorNew(base, 8);
  if (!holder) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82092878 App::Start");
  calls.set_register(31, interface);
  for (uint32_t at = 0; at <= 16; at += 4) {
    Store(base, kBootMovies + at, 11);
  }
  Store(base, kBootMovies + 28, 0);
  Store(base, kBootMovies + 0, 10);
  Store(base, kBootMovies + 20, 4);
  Store(base, kBootMovies + 24, 0);
  uint32_t state = 0;
  if (*holder != 0) {
    game_modes::ModesConstruct(base, *holder, 3);
    state = *holder;
  }
  const uint32_t states = Load(base, interface + kInterfaceStates);
  const uint32_t target = VirtualTarget(base, states, 4);
  bool ours = false;
  if (target == kAddLayer) {
    calls.in_frame(112, 0x82092908, [&](uint32_t inner) {
      ours = StatesAddLayer(base, states, state, inner, calls);
    });
  }
  // r3 as the slot leaves it (AddLayer's: AddChild's, the root's children).
  uint32_t r3 = 0;
  if (ours) {
    r3 = Load(base, states + kStatesRoot) + kLayerChildren;
  } else {
    r3 = calls.call(target, states, uint64_t(state), std::nullopt, 0x82092908);
  }
  Store(base, kStartFlag, 2);
  return r3;
}

}  // namespace app_states
