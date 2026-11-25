// The app's state stack and its layers natively (layers.h,
// docs/functions/app-states.md).

#include "app_states/layers.h"

#include "guest.h"

#include "app_states/containers.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "memory/c_runtime_heap.h"
#include "schema/app_states.h"
#include "schema/boot_mode.h"
#include "schema/game_modes.h"
#include "schema/options_screen.h"
#include "std_list/ptr_list.h"

namespace app_states {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// The original functions, for the paths their plain C++ hands back.
constexpr uint32_t kPtrListConstruct = 0x820AD058;
constexpr uint32_t kPtrListInsert = 0x82095958;
constexpr uint32_t kOpListConstruct = 0x820AD0B8;
constexpr uint32_t kLayerPush = 0x820ADAD0;

}  // namespace

void LayerConstruct(uint32_t layer, uint32_t owner, std::optional<uint32_t> state,
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
  const schema::StateLayer l(layer);
  l.set_state_vtable(kStateBaseVtable);
  l.set_vtable(kLayerVtable);
  l.set_state_vtable(kLayerStateVtable);
  l.set_owner(owner);
  if (!std_list::Construct(l.children_list().address)) {
    calls.call(kPtrListConstruct, l.children_list().address, std::nullopt, std::nullopt,
               children_return);
  }
  if (!std_list::Construct(l.states_list().address)) {
    calls.call(kPtrListConstruct, l.states_list().address, std::nullopt, std::nullopt, states_return);
  }
  if (!OpConstruct(l.ops_list().address)) {
    calls.call(kOpListConstruct, l.ops_list().address, std::nullopt, std::nullopt, ops_return);
  }
  if (state && !LayerPush(layer, *state)) {
    calls.call(kLayerPush, layer, uint64_t(*state), std::nullopt, 0x820ADC40);
  }
}

void LayerAddChild(uint32_t layer, uint32_t child, uint32_t frame,
                   const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820ADA08 StateLayer::AddChild");
  calls.set_register(31, layer);
  const uint32_t value = frame + 112 + 28;  // the caller's r1 + 28
  Store(value, child);
  const uint32_t child_state = schema::StateLayer(child).address + 4;
  calls.call(VirtualTarget(child_state, 4), child_state, uint64_t(layer), std::nullopt,
             0x820ADA38);
  const uint32_t list = schema::StateLayer(layer).children_list().address;
  const uint32_t head = schema::PtrList(list).head().address;
  Store(frame + 80, list);
  Store(frame + 84, head);
  if (!std_list::Insert(list, head, Load(value))) {
    calls.call(kPtrListInsert, list, (uint64_t(list) << 32) | head, uint64_t(value), 0x820ADA58);
  }
}

bool StatesAddLayer(uint32_t states, uint32_t state, uint32_t frame,
                    const AppCalls& calls) {
  (void)frame;
  const auto layer = std::optional<uint32_t>(schema::StateLayer::New().address);  // host memory (E5 step 4)
  if (!layer) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820AE130 AppStates::AddLayer");
  const schema::AppStates s(states);
  calls.set_register(30, states);
  calls.set_register(29, state);
  calls.set_register(31, *layer);
  uint32_t child = 0;
  if (*layer != 0) {
    const uint32_t app = s.app().address;
    const uint32_t owner =
        calls.call(VirtualTarget(app, 8), app, std::nullopt, std::nullopt, 0x820AE168);
    calls.in_frame(112, 0x820AE178, [&](uint32_t inner) {
      LayerConstruct(*layer, owner, state, inner, calls);
    });
    child = *layer;
  }
  calls.in_frame(112, 0x820AE18C, [&](uint32_t inner) {
    LayerAddChild(s.root().address, child, inner, calls);
  });
  return true;
}

bool StatesConstruct(uint32_t states, uint32_t setup, uint32_t frame,
                     const AppCalls& calls) {
  const auto layer = std::optional<uint32_t>(schema::StateLayer::New().address);  // host memory (E5 step 4)
  if (!layer) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820AE348 AppStates::AppStates");
  calls.set_register(31, states);
  const schema::AppStates s(states);
  s.set_vtable(kStatesVtable);
  s.set_app(schema::AppStatesSetup(setup).app());
  calls.set_register(30, *layer);
  uint32_t root = 0;
  if (*layer != 0) {
    const uint32_t app = s.app().address;
    const uint32_t owner =
        calls.call(VirtualTarget(app, 8), app, std::nullopt, std::nullopt, 0x820AE390);
    calls.in_frame(96, 0x820AE39C, [&](uint32_t inner) {
      LayerConstruct(*layer, owner, std::nullopt, inner, calls);
    });
    root = *layer;
  }
  const uint32_t app = s.app().address;
  s.set_root(root);
  const uint32_t interface =
      calls.call(VirtualTarget(app, 4), app, std::nullopt, std::nullopt, 0x820AE3C0);
  calls.set_register(30, interface);
  // Key 8 (the map +28): AppStates::OnUpdate.
  Store(frame + 92, states);
  calls.set_register(29, kDelegateVtable);
  Store(frame + 88, kDelegateVtable);
  Store(frame + 96, kStatesOnUpdate);
  Store(frame + 80, 8);
  calls.call(VirtualTarget(interface, 16), interface, uint64_t(frame + 80),
             uint64_t(frame + 88), 0x820AE400);
  // Key 2 (the map +40): AppStates::OnUpdateToggled; the first delegate's
  // vtable left as the base's (its destructor).
  Store(frame + 104, kDelegateVtable);
  calls.set_register(28, kDelegateBaseVtable);
  Store(frame + 108, states);
  Store(frame + 80, 2);
  Store(frame + 88, kDelegateBaseVtable);
  Store(frame + 112, kStatesOnUpdateToggled);
  calls.call(VirtualTarget(interface, 12), interface, uint64_t(frame + 80),
             uint64_t(frame + 104), 0x820AE444);
  // Listener 9 (the map +64): AppStates::OnMessage.
  Store(frame + 104, kDelegateBaseVtable);
  Store(frame + 124, states);
  Store(frame + 120, kDelegateVtable);
  Store(frame + 128, kStatesOnMessage);
  Store(frame + 80, 9);
  calls.call(VirtualTarget(interface, 4), interface, uint64_t(frame + 80),
             uint64_t(frame + 120), 0x820AE488);
  return true;
}

std::optional<uint32_t> StatesCreate(uint32_t setup, uint32_t frame,
                                     const AppCalls& calls) {
  (void)frame;
  const auto block = std::optional<uint32_t>(schema::AppStates::New().address);  // host memory (E5 step 4)
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
    ours = StatesConstruct(*block, setup, inner, calls);
  });
  if (!ours) {
    calls.call(0x820AE348, *block, uint64_t(setup), std::nullopt, 0x820AE4C4);
  }
  return *block;
}

std::optional<uint32_t> AppStart(uint32_t interface, uint32_t frame,
                                 const AppCalls& calls) {
  (void)frame;
  const auto holder = std::optional<uint32_t>(schema::ModeHolder::New().address);  // host memory (E5 step 4)
  if (!holder) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82092878 App::Start");
  calls.set_register(31, interface);
  using Boot = schema::BootModeGlobals;
  for (uint32_t at = 0; at <= 16; at += 4) {
    Store(Boot::movie_ids_address() + at, 11);
  }
  Boot::set_movie_index(0);
  Store(Boot::movie_ids_address() + 0, 10);
  Boot::set_next_type(4);
  Boot::set_skippable(0);
  uint32_t state = 0;
  if (*holder != 0) {
    game_modes::ModesConstruct(*holder, 3);
    state = *holder;
  }
  const uint32_t states = schema::AppInterface(interface).states().address;
  const uint32_t target = VirtualTarget(states, 4);
  bool ours = false;
  if (target == kAddLayer) {
    calls.in_frame(112, 0x82092908, [&](uint32_t inner) {
      ours = StatesAddLayer(states, state, inner, calls);
    });
  }
  // r3 as the slot leaves it (AddLayer's: AddChild's, the root's children).
  uint32_t r3 = 0;
  if (ours) {
    r3 = schema::AppStates(states).root().children_list().address;
  } else {
    r3 = calls.call(target, states, uint64_t(state), std::nullopt, 0x82092908);
  }
  schema::OptionsScreenGlobals::set_return_menu(2);
  return r3;
}

namespace {

uint64_t Load64(uint32_t address) {
  return (uint64_t(Load(address)) << 32) | Load(address + 4);
}
void Store64(uint32_t address, uint64_t value) {
  Store(address, uint32_t(value >> 32));
  Store(address + 4, uint32_t(value));
}

}  // namespace

void ListForEach(uint32_t result, uint64_t first, uint64_t last, uint64_t functor,
                 uint64_t adjust, bool ops, uint32_t frame, const AppCalls& calls) {
  if (ops) {
    ITC_NATIVE_RUNNING("sub_820AD440 OpList::ForEach");
  } else {
    ITC_NATIVE_RUNNING("sub_820AD3A0 PtrList::ForEach");
  }
  const uint32_t caller = frame + 112;
  const uint32_t return_address = ops ? 0x820AD4A4 : 0x820AD404;
  Store64(caller + 24, first);
  calls.set_register(30, result);
  Store64(caller + 32, last);
  Store64(caller + 48, functor);
  Store64(caller + 56, adjust);
  uint32_t node = Load(caller + 28);
  calls.set_register(31, node);
  while (Load(caller + 36) != node) {
    const uint32_t object = Load(caller + 52) + Load(caller + 56);
    const uint32_t function = Load(caller + 48);
    if (ops) {
      // The node in host memory (E5 step 4): its two words, type then state.
      const schema::AppOpListNode n(node);
      const uint64_t op = (uint64_t(n.op_type()) << 32) | n.op_state();
      std::optional<uint32_t> ours;
      if (function == kApplyOp) {
        calls.in_frame(128, return_address,
                       [&](uint32_t inner) { ours = LayerApplyOp(object, op, inner, calls); });
      }
      if (!ours) {
        calls.call(function, object, op, std::nullopt, return_address);
      }
    } else {
      const uint32_t value = schema::StateListNode(node).value();
      if (function == kUpdateChild || function == kUpdateChildToggled) {
        calls.in_frame(112, return_address, [&](uint32_t inner) {
          UpdateChild(object, value, function == kUpdateChildToggled, inner, calls);
        });
      } else if (function == kToggleOffChild) {
        ToggleOffChild(object, value, return_address, calls);
      } else if (function == kToggleOnChild) {
        ToggleOnChild(value, return_address, calls);
      } else {
        calls.call(function, object, uint64_t(value), std::nullopt, return_address);
      }
    }
    node = ops ? schema::AppOpListNode(node).next().address
               : schema::StateListNode(node).next().address;
    calls.set_register(31, node);
  }
  const uint64_t a = Load64(caller + 48), b = Load64(caller + 56);
  Store64(result, a);
  Store64(result + 8, b);
}

uint32_t UpdateChild(uint32_t layer, uint32_t child, bool toggled, uint32_t frame,
                     const AppCalls& calls) {
  (void)frame;
  if (toggled) {
    ITC_NATIVE_RUNNING("sub_820ACDE8 StateLayer::UpdateToggled's functor");
  } else {
    ITC_NATIVE_RUNNING("sub_820ACD80 StateLayer::Update's functor");
  }
  const uint32_t state = schema::StateLayer(child).address + 4;
  calls.set_register(31, state);
  calls.set_register(30, layer);
  const uint32_t target = VirtualTarget(state, toggled ? 36 : 32);
  const uint32_t return_address = toggled ? 0x820ACE1C : 0x820ACDB4;
  if (calls.native_heap && target == (toggled ? kLayerStateUpdateToggled : kLayerStateUpdate)) {
    // A child layer's update (a tail call: its frame below this one).
    calls.in_frame(144, return_address, [&](uint32_t inner) {
      LayerStateUpdate(state, toggled, inner, calls);
    });
  } else {
    calls.call(target, state, uint64_t(layer), std::nullopt, return_address);
  }
  return calls.call(VirtualTarget(state, 40), state, uint64_t(layer), std::nullopt,
                    toggled ? 0x820ACE34 : 0x820ACDCC);
}

uint32_t ToggleOffChild(uint32_t layer, uint32_t child, uint32_t return_address,
                        const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820ACD60 StateLayer::OnToggleOff's functor");
  const uint32_t state = schema::StateLayer(child).address + 4;
  calls.set_register(11, state);
  const uint32_t target = VirtualTarget(state, 20);
  if (target == kLayerStateToggleOff) {
    return LayerStateToggleOff(state, return_address, calls);
  }
  return calls.call(target, state, uint64_t(layer), std::nullopt, return_address);
}

uint32_t LayerApplyOps(uint32_t layer, uint32_t frame, const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820ADE68 StateLayer::ApplyOps");
  const uint32_t list = schema::StateLayer(layer).ops_list().address;
  calls.set_register(31, list);
  Store(frame + 104, layer);
  Store(frame + 80, kApplyOp);
  Store(frame + 84, 0);
  const uint64_t function = Load64(frame + 80);
  const auto head = schema::OpList(list).head();
  Store(frame + 80, list);
  Store64(frame + 96, function);
  const uint64_t functor = Load64(frame + 96);
  const uint64_t adjust = Load64(frame + 104);  // its low word as the stack holds it
  Store(frame + 96, list);
  Store(frame + 84, head.address);
  Store(frame + 100, head.next().address);
  const uint64_t last = Load64(frame + 80);
  const uint64_t first = Load64(frame + 96);
  calls.in_frame(112, 0x820ADEE0, [&](uint32_t inner) {
    ListForEach(frame + 96, first, last, functor, adjust, true, inner, calls);
  });
  return ListClear(list);
}

uint32_t LayerUpdate(uint32_t layer, bool toggled, uint32_t frame,
                     const AppCalls& calls) {
  if (toggled) {
    ITC_NATIVE_RUNNING("sub_820AE010 StateLayer::UpdateToggled");
  } else {
    ITC_NATIVE_RUNNING("sub_820ADF00 StateLayer::Update");
  }
  calls.set_register(31, layer);
  const schema::StateLayer l(layer);
  if (l.states_size() != 0) {
    const uint32_t states = l.states_list().address;
    calls.set_register(30, states);
    const uint32_t returns[2][2] = {{0x820ADF5C, 0x820ADF8C}, {0x820AE06C, 0x820AE09C}};
    const uint32_t slots[2] = {toggled ? 36u : 32u, 40u};
    for (int i = 0; i < 2; i++) {
      const auto head = schema::PtrList(states).head();
      const auto top = head.next();
      if (top.address == head.address) {
        ITC_LOG_WARN("{}: tw/td trap hit (type 22)", toggled ? "sub_820AE010" : "sub_820ADF00");
      }
      const uint32_t state = top.value();
      calls.call(VirtualTarget(state, slots[i]), state, uint64_t(layer), std::nullopt,
                 returns[toggled][i]);
    }
  }
  const uint32_t list = l.children_list().address;
  Store(frame + 104, layer);
  Store(frame + 84, 0);
  Store(frame + 80, toggled ? kUpdateChildToggled : kUpdateChild);
  const uint64_t function = Load64(frame + 80);
  Store(frame + 80, list);
  Store64(frame + 96, function);
  const auto head = schema::PtrList(list).head();
  const uint64_t functor = Load64(frame + 96);
  const uint64_t adjust = Load64(frame + 104);  // its low word as the stack holds it
  Store(frame + 96, list);
  const uint32_t first_node = head.next().address;
  Store(frame + 84, head.address);
  const uint64_t last = Load64(frame + 80);
  Store(frame + 100, first_node);
  const uint64_t first = Load64(frame + 96);
  calls.in_frame(112, toggled ? 0x820AE100 : 0x820ADFF0, [&](uint32_t inner) {
    ListForEach(frame + 96, first, last, functor, adjust, false, inner, calls);
  });
  const uint32_t return_address = toggled ? 0x820AE108 : 0x820ADFF8;
  if (!calls.native_heap) {
    return calls.call(0x820ADE68, layer, std::nullopt, std::nullopt, return_address);
  }
  uint32_t answer = 0;
  calls.in_frame(128, return_address,
                 [&](uint32_t inner) { answer = LayerApplyOps(layer, inner, calls); });
  return answer;
}

uint32_t LayerStateUpdate(uint32_t state, bool toggled, uint32_t frame,
                          const AppCalls& calls) {
  if (toggled) {
    ITC_NATIVE_RUNNING("sub_820AE128 StateLayer state slot 9");
  } else {
    ITC_NATIVE_RUNNING("sub_820AE120 StateLayer state slot 8");
  }
  return LayerUpdate(state - 4, toggled, frame, calls);
}

uint32_t StatesOnUpdate(uint32_t states, bool toggled, uint32_t frame,
                        const AppCalls& calls) {
  if (toggled) {
    ITC_NATIVE_RUNNING("sub_820AE1C0 AppStates::OnUpdateToggled");
  } else {
    ITC_NATIVE_RUNNING("sub_820AE1B8 AppStates::OnUpdate");
  }
  return LayerUpdate(schema::AppStates(states).root().address, toggled, frame, calls);
}

std::optional<uint32_t> LayerApplyOp(uint32_t layer, uint64_t op, uint32_t frame,
                                     const AppCalls& calls) {
  const uint32_t type = uint32_t(op >> 32);
  const uint32_t state = uint32_t(op);
  if (type == 1 || type == 3 || !calls.native_heap) {
    return std::nullopt;
  }
  const uint32_t caller = frame + 128;
  if (type == 0) {
    const auto added = std::optional<uint32_t>(schema::StateLayer::New().address);  // host memory (E5 step 4)
    if (!added) {
      return std::nullopt;
    }
    ITC_NATIVE_RUNNING("sub_820ADC60 StateLayer::ApplyOp");
    Store64(caller + 24, op);
    calls.set_register(30, layer);
    uint32_t child = 0;
    if (*added != 0) {
      const uint32_t owner = schema::StateLayer(layer).owner();
      calls.in_frame(112, 0x820ADE48, [&](uint32_t inner) {
        LayerConstruct(*added, owner, state, inner, calls);
      });
      child = *added;
    }
    calls.in_frame(112, 0x820ADE5C,
                   [&](uint32_t inner) { LayerAddChild(layer, child, inner, calls); });
    return schema::StateLayer(layer).children_list().address;
  }
  ITC_NATIVE_RUNNING("sub_820ADC60 StateLayer::ApplyOp");
  Store64(caller + 24, op);
  calls.set_register(30, layer);
  if (type != 2) {
    return layer;
  }
  const schema::StateLayer l(layer);
  const uint32_t list = l.states_list().address;
  if (l.states_size() != 0) {
    const auto head = schema::PtrList(list).head();
    const auto top = head.next();
    if (top.address == head.address) {
      ITC_LOG_WARN("sub_820ADC60: tw/td trap hit (type 22)");
    }
    const uint32_t covered = top.value();
    calls.call(VirtualTarget(covered, 24), covered, uint64_t(layer), std::nullopt,
               0x820ADD70);
  }
  const auto head = schema::PtrList(list).head();
  Store(frame + 88, list);
  const uint32_t first = head.next().address;
  Store(frame + 92, first);
  if (!std_list::Insert(list, first, Load(caller + 28))) {
    calls.call(kPtrListInsert, list, (uint64_t(list) << 32) | first, uint64_t(caller + 28),
               0x820ADD1C);
  }
  const uint32_t entered = Load(caller + 28);
  return calls.call(VirtualTarget(entered, 4), entered, uint64_t(layer), std::nullopt,
                    0x820ADE28);
}

uint32_t LayerToggle(uint32_t layer, bool off, uint32_t frame,
                     const AppCalls& calls) {
  if (off) {
    ITC_NATIVE_RUNNING("sub_820AD680 StateLayer::OnToggleOff");
  } else {
    ITC_NATIVE_RUNNING("sub_820AD4E0 StateLayer::OnToggleOn");
  }
  calls.set_register(31, layer);
  const schema::StateLayer l(layer);
  if (l.states_size() != 0) {
    const auto head = schema::PtrList(l.states_list().address).head();
    const auto top = head.next();
    if (top.address == head.address) {
      ITC_LOG_WARN("{}: tw/td trap hit (type 22)", off ? "sub_820AD680" : "sub_820AD4E0");
    }
    const uint32_t state = top.value();
    calls.call(VirtualTarget(state, off ? 20 : 12), state, uint64_t(layer), std::nullopt,
               off ? 0x820AD6D8 : 0x820AD538);
  }
  const uint32_t list = l.children_list().address;
  Store(frame + 104, layer);
  Store(frame + 84, 0);
  Store(frame + 80, off ? kToggleOffChild : kToggleOnChild);
  const uint64_t function = Load64(frame + 80);
  Store(frame + 80, list);
  Store64(frame + 96, function);
  const auto head = schema::PtrList(list).head();
  const uint64_t functor = Load64(frame + 96);
  const uint64_t adjust = Load64(frame + 104);  // its low word as the stack holds it
  Store(frame + 96, list);
  const uint32_t first_node = head.next().address;
  Store(frame + 84, head.address);
  const uint64_t last = Load64(frame + 80);
  Store(frame + 100, first_node);
  const uint64_t first = Load64(frame + 96);
  calls.in_frame(112, off ? 0x820AD73C : 0x820AD59C, [&](uint32_t inner) {
    ListForEach(frame + 96, first, last, functor, adjust, false, inner, calls);
  });
  return frame + 96;
}

uint32_t ToggleOnChild(uint32_t child, uint32_t return_address,
                       const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820ACD38 StateLayer::OnToggleOn's functor");
  uint32_t answer = 0;
  calls.in_frame(128, return_address,
                 [&](uint32_t inner) { answer = LayerToggle(child, false, inner, calls); });
  return answer;
}

uint32_t LayerStateToggleOff(uint32_t state, uint32_t return_address,
                             const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820AD760 StateLayer state slot 5");
  uint32_t answer = 0;
  calls.in_frame(128, return_address,
                 [&](uint32_t inner) { answer = LayerToggle(state - 4, true, inner, calls); });
  return answer;
}

std::optional<uint32_t> StatesOnMessage(uint32_t states, uint32_t message,
                                        uint32_t frame, const AppCalls& calls) {
  (void)frame;
  const uint32_t which = schema::AppMessageWord(message).value();
  if (which == 1 || which == 5 || which == 6) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820AE1C8 AppStates::OnMessage");
  if (which != 0 && which != 2) {
    return states;
  }
  const uint32_t root = schema::AppStates(states).root().address;
  uint32_t answer = 0;
  calls.in_frame(128, which == 0 ? 0x820AE29C : 0x820AE284, [&](uint32_t inner) {
    answer = LayerToggle(root, which == 2, inner, calls);
  });
  return answer;
}

}  // namespace app_states
