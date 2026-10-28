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

namespace {

uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

}  // namespace

void ListForEach(uint8_t* base, uint32_t result, uint64_t first, uint64_t last, uint64_t functor,
                 uint64_t adjust, bool ops, uint32_t frame, const AppCalls& calls) {
  if (ops) {
    ITC_NATIVE_RUNNING("sub_820AD440 OpList::ForEach");
  } else {
    ITC_NATIVE_RUNNING("sub_820AD3A0 PtrList::ForEach");
  }
  const uint32_t caller = frame + 112;
  const uint32_t return_address = ops ? 0x820AD4A4 : 0x820AD404;
  Store64(base, caller + 24, first);
  calls.set_register(30, result);
  Store64(base, caller + 32, last);
  Store64(base, caller + 48, functor);
  Store64(base, caller + 56, adjust);
  uint32_t node = Load(base, caller + 28);
  calls.set_register(31, node);
  while (Load(base, caller + 36) != node) {
    const uint32_t object = Load(base, caller + 52) + Load(base, caller + 56);
    const uint32_t function = Load(base, caller + 48);
    if (ops) {
      calls.call(function, object, Load64(base, node + 8), std::nullopt, return_address);
    } else {
      const uint32_t value = Load(base, node + 8);
      if (function == kUpdateChild || function == kUpdateChildToggled) {
        calls.in_frame(112, return_address, [&](uint32_t inner) {
          UpdateChild(base, object, value, function == kUpdateChildToggled, inner, calls);
        });
      } else if (function == kToggleOffChild) {
        ToggleOffChild(base, object, value, return_address, calls);
      } else {
        calls.call(function, object, uint64_t(value), std::nullopt, return_address);
      }
    }
    node = Load(base, node + 0);
    calls.set_register(31, node);
  }
  const uint64_t a = Load64(base, caller + 48), b = Load64(base, caller + 56);
  Store64(base, result, a);
  Store64(base, result + 8, b);
}

uint32_t UpdateChild(uint8_t* base, uint32_t layer, uint32_t child, bool toggled, uint32_t frame,
                     const AppCalls& calls) {
  (void)frame;
  if (toggled) {
    ITC_NATIVE_RUNNING("sub_820ACDE8 StateLayer::UpdateToggled's functor");
  } else {
    ITC_NATIVE_RUNNING("sub_820ACD80 StateLayer::Update's functor");
  }
  const uint32_t state = child + 4;
  calls.set_register(31, state);
  calls.set_register(30, layer);
  const uint32_t target = VirtualTarget(base, state, toggled ? 36 : 32);
  const uint32_t return_address = toggled ? 0x820ACE1C : 0x820ACDB4;
  if (calls.native_heap && target == (toggled ? kLayerStateUpdateToggled : kLayerStateUpdate)) {
    // A child layer's update (a tail call: its frame below this one).
    calls.in_frame(144, return_address, [&](uint32_t inner) {
      LayerStateUpdate(base, state, toggled, inner, calls);
    });
  } else {
    calls.call(target, state, uint64_t(layer), std::nullopt, return_address);
  }
  return calls.call(VirtualTarget(base, state, 40), state, uint64_t(layer), std::nullopt,
                    toggled ? 0x820ACE34 : 0x820ACDCC);
}

uint32_t ToggleOffChild(uint8_t* base, uint32_t layer, uint32_t child, uint32_t return_address,
                        const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820ACD60 StateLayer::OnToggleOff's functor");
  const uint32_t state = child + 4;
  calls.set_register(11, state);
  return calls.call(VirtualTarget(base, state, 20), state, uint64_t(layer), std::nullopt,
                    return_address);
}

uint32_t LayerApplyOps(uint8_t* base, uint32_t layer, uint32_t frame, const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820ADE68 StateLayer::ApplyOps");
  const uint32_t list = layer + kLayerOps;
  calls.set_register(31, list);
  Store(base, frame + 104, layer);
  Store(base, frame + 80, kApplyOp);
  Store(base, frame + 84, 0);
  const uint64_t function = Load64(base, frame + 80);
  const uint32_t head = Load(base, list + 4);
  Store(base, frame + 80, list);
  Store64(base, frame + 96, function);
  const uint64_t functor = Load64(base, frame + 96);
  const uint64_t adjust = Load64(base, frame + 104);  // its low word as the stack holds it
  Store(base, frame + 96, list);
  Store(base, frame + 84, head);
  Store(base, frame + 100, Load(base, head + 0));
  const uint64_t last = Load64(base, frame + 80);
  const uint64_t first = Load64(base, frame + 96);
  calls.in_frame(112, 0x820ADEE0, [&](uint32_t inner) {
    ListForEach(base, frame + 96, first, last, functor, adjust, true, inner, calls);
  });
  return ListClear(base, list);
}

uint32_t LayerUpdate(uint8_t* base, uint32_t layer, bool toggled, uint32_t frame,
                     const AppCalls& calls) {
  if (toggled) {
    ITC_NATIVE_RUNNING("sub_820AE010 StateLayer::UpdateToggled");
  } else {
    ITC_NATIVE_RUNNING("sub_820ADF00 StateLayer::Update");
  }
  calls.set_register(31, layer);
  if (Load(base, layer + 32) != 0) {
    const uint32_t states = layer + kLayerStates;
    calls.set_register(30, states);
    const uint32_t returns[2][2] = {{0x820ADF5C, 0x820ADF8C}, {0x820AE06C, 0x820AE09C}};
    const uint32_t slots[2] = {toggled ? 36u : 32u, 40u};
    for (int i = 0; i < 2; i++) {
      const uint32_t head = Load(base, states + std_list::kHead);
      const uint32_t top = Load(base, head + std_list::kNext);
      if (top == head) {
        ITC_LOG_WARN("{}: tw/td trap hit (type 22)", toggled ? "sub_820AE010" : "sub_820ADF00");
      }
      const uint32_t state = Load(base, top + std_list::kValue);
      calls.call(VirtualTarget(base, state, slots[i]), state, uint64_t(layer), std::nullopt,
                 returns[toggled][i]);
    }
  }
  const uint32_t list = layer + kLayerChildren;
  Store(base, frame + 104, layer);
  Store(base, frame + 84, 0);
  Store(base, frame + 80, toggled ? kUpdateChildToggled : kUpdateChild);
  const uint64_t function = Load64(base, frame + 80);
  Store(base, frame + 80, list);
  Store64(base, frame + 96, function);
  const uint32_t head = Load(base, list + std_list::kHead);
  const uint64_t functor = Load64(base, frame + 96);
  const uint64_t adjust = Load64(base, frame + 104);  // its low word as the stack holds it
  Store(base, frame + 96, list);
  const uint32_t first_node = Load(base, head + std_list::kNext);
  Store(base, frame + 84, head);
  const uint64_t last = Load64(base, frame + 80);
  Store(base, frame + 100, first_node);
  const uint64_t first = Load64(base, frame + 96);
  calls.in_frame(112, toggled ? 0x820AE100 : 0x820ADFF0, [&](uint32_t inner) {
    ListForEach(base, frame + 96, first, last, functor, adjust, false, inner, calls);
  });
  const uint32_t return_address = toggled ? 0x820AE108 : 0x820ADFF8;
  if (!calls.native_heap) {
    return calls.call(0x820ADE68, layer, std::nullopt, std::nullopt, return_address);
  }
  uint32_t answer = 0;
  calls.in_frame(128, return_address,
                 [&](uint32_t inner) { answer = LayerApplyOps(base, layer, inner, calls); });
  return answer;
}

uint32_t LayerStateUpdate(uint8_t* base, uint32_t state, bool toggled, uint32_t frame,
                          const AppCalls& calls) {
  if (toggled) {
    ITC_NATIVE_RUNNING("sub_820AE128 StateLayer state slot 9");
  } else {
    ITC_NATIVE_RUNNING("sub_820AE120 StateLayer state slot 8");
  }
  return LayerUpdate(base, state - 4, toggled, frame, calls);
}

uint32_t StatesOnUpdate(uint8_t* base, uint32_t states, bool toggled, uint32_t frame,
                        const AppCalls& calls) {
  if (toggled) {
    ITC_NATIVE_RUNNING("sub_820AE1C0 AppStates::OnUpdateToggled");
  } else {
    ITC_NATIVE_RUNNING("sub_820AE1B8 AppStates::OnUpdate");
  }
  return LayerUpdate(base, Load(base, states + kStatesRoot), toggled, frame, calls);
}

}  // namespace app_states
