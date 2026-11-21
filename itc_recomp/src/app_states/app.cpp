// The app's update, its interface's slots and its state stack natively
// (app.h, docs/functions/app-states.md).

#include "app_states/app.h"

#include "guest.h"

#include "app_states/containers.h"
#include "base/log.h"
#include "framework/run_loop.h"
#include "schema/app_states.h"
#include "schema/framework.h"

namespace app_states {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// A doubleword as the PowerPC stores it (the high word first).
uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
void Copy16(uint8_t* base, uint32_t to, uint32_t from) {
  const uint64_t a = Load64(base, from), b = Load64(base, from + 8);
  Store64(base, to, a);
  Store64(base, to + 8, b);
}

// The original's tw 22 (a failed check), which the recompiled code logs and
// goes on from.
void Trap(const char* function) { ITC_LOG_WARN("{}: tw/td trap hit (type 22)", function); }

// std::for_each over a vector with a member functor (ForEachMessage and
// ForEachRequest): `step` bytes an element, `call(cursor, object)` the
// functor's call.
template <typename Call>
void ForEachIn(uint8_t* base, uint32_t result, uint64_t first, uint64_t last, uint64_t functor,
               uint64_t adjust, uint32_t frame, const AppCalls& calls, uint32_t step, Call call) {
  const uint32_t caller = frame + 128;
  Store64(base, caller + 24, first);
  calls.set_register(28, result);
  Store64(base, caller + 32, last);
  Store64(base, caller + 48, functor);
  Store64(base, caller + 56, adjust);
  uint32_t cursor = Load(base, caller + 28);
  const uint32_t end = Load(base, caller + 36);
  calls.set_register(31, cursor);
  calls.set_register(29, end);
  if (cursor != end) {
    const uint32_t object = Load(base, caller + 52) + Load(base, caller + 56);
    calls.set_register(30, object);
    do {
      call(cursor, object, Load(base, caller + 48));
      cursor += step;
      calls.set_register(31, cursor);
    } while (cursor != end);
  }
  Copy16(base, result, caller + 48);
}

}  // namespace

uint32_t VirtualTarget(uint8_t* base, uint32_t object, uint32_t offset) {
  return Load(base, Load(base, object) + offset);
}

uint32_t StatesIsEmpty(uint8_t* base, uint32_t states, const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820AE1A0 AppStates::IsEmpty");
  const uint32_t root = schema::AppStates(base, states).root().state_vtable_address();
  const uint32_t target = VirtualTarget(base, root, 44);
  if (target == kLayerIsEmpty) {
    return LayerIsEmpty(base, root);
  }
  return calls.call(target, root, std::nullopt, std::nullopt, 0);
}

uint32_t AppIsRunning(uint8_t* base, uint32_t interface, const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82092928 App::IsRunning");
  const uint32_t states = schema::AppInterface(base, interface).states().address;
  const uint32_t target = VirtualTarget(base, states, 12);
  uint32_t answer = 0;
  if (target == kAppStatesIsEmpty) {
    // Its guest call, if any, a tail call from this frame.
    const uint32_t root = schema::AppStates(base, states).root().state_vtable_address();
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

void HandlerMapForEach(uint8_t* base, uint32_t result, uint64_t first, uint64_t last,
                       uint64_t functor, uint64_t adjust, uint32_t frame, const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82092BF0 HandlerMap::ForEach");
  const uint32_t caller = frame + 112;
  calls.set_register(31, result);
  Store64(base, caller + 32, last);
  Store64(base, caller + 48, functor);
  Store64(base, caller + 56, adjust);
  Store64(base, frame + 80, first);
  for (;;) {
    const uint32_t map = Load(base, frame + 80);
    if (map == 0 || map != Load(base, caller + 32)) {
      Trap("sub_82092BF0");
    }
    const uint32_t node = Load(base, frame + 84);
    if (node == Load(base, caller + 36)) {
      break;
    }
    if (map == 0) {
      Trap("sub_82092BF0");
    }
    if (node == schema::HandlerMap(base, map).head().address) {
      Trap("sub_82092BF0");
    }
    const uint32_t object = Load(base, caller + 52) + Load(base, caller + 56);
    const schema::HandlerMapNode entry(base, node);
    Store(base, frame + 88, entry.key());
    Store(base, frame + 92, entry.delegate().address);
    calls.call(Load(base, caller + 48), object, Load64(base, frame + 88), std::nullopt, 0x82092C98);
    framework::Iterator it{Load(base, frame + 80), Load(base, frame + 84)};
    if (framework::MapIncrement(base, it)) {
      Store(base, frame + 84, it.node);
    } else {
      calls.increment(frame + 80, 0x82092CA0);
    }
  }
  Copy16(base, result, caller + 48);
}

void ForEachMessage(uint8_t* base, uint32_t result, uint64_t first, uint64_t last,
                    uint64_t functor, uint64_t adjust, uint32_t frame, const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82092CD0 App::ForEachMessage");
  ForEachIn(base, result, first, last, functor, adjust, frame, calls, 4,
            [&](uint32_t cursor, uint32_t object, uint32_t function) {
              const uint32_t message = schema::AppMessageWord(base, cursor).value();
              calls.call(function, object, message, std::nullopt, 0x82092D20);
            });
}

void ForEachRequest(uint8_t* base, uint32_t result, uint64_t first, uint64_t last,
                    uint64_t functor, uint64_t adjust, uint32_t frame, const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82091C48 App::ForEachRequest");
  ForEachIn(base, result, first, last, functor, adjust, frame, calls, schema::Request::kSize,
            [&](uint32_t cursor, uint32_t object, uint32_t function) {
              const schema::Request request(base, cursor);
              const uint32_t type = request.type();
              const uint64_t type_and_key = (uint64_t(type) << 32) | request.key();
              calls.call(function, object, type_and_key,
                         uint64_t(request.delegate().address) << 32, 0x82091CA0);
            });
}

uint32_t RunHandler(uint8_t* base, uint32_t app, uint64_t entry, bool listener, uint32_t frame,
                    const AppCalls& calls) {
  if (listener) {
    ITC_NATIVE_RUNNING("sub_820929B0 App::RunListener");
  } else {
    ITC_NATIVE_RUNNING("sub_82092968 App::RunHandler");
  }
  const uint32_t caller = frame + 96;
  Store64(base, caller + 24, entry);
  const schema::App a(base, app);
  const uint32_t interface = app != 0 ? a.interface_vtable_address() : 0;
  const uint32_t delegate = Load(base, caller + 28);
  const uint32_t target = VirtualTarget(base, delegate, 4);
  if (listener) {
    return calls.call(target, delegate, interface, uint64_t(a.message_address()), 0x820929EC);
  }
  return calls.call(target, delegate, interface, std::nullopt, 0x8209299C);
}

void DeliverMessage(uint8_t* base, uint32_t app, uint32_t message, uint32_t frame,
                    const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82092D50 App::DeliverMessage");
  const schema::App a(base, app);
  const uint32_t map = a.listeners_address();
  Store(base, frame + 84, 0);
  Store(base, frame + 80, kRunListener);
  const uint64_t function = Load64(base, frame + 80);
  Store(base, frame + 104, app);
  a.set_message(message);
  const uint32_t head = a.listeners_head().address;
  Store(base, frame + 80, map);
  Store64(base, frame + 96, function);
  const uint64_t functor = Load64(base, frame + 96);
  const uint64_t adjust = Load64(base, frame + 104);  // its low word as the stack holds it
  Store(base, frame + 96, map);
  const uint32_t left = schema::AppHandlerMapHead(base, head).left();
  Store(base, frame + 84, head);
  const uint64_t last = Load64(base, frame + 80);
  Store(base, frame + 100, left);
  const uint64_t first = Load64(base, frame + 96);
  calls.set_register(31, frame + 96);
  calls.in_frame(112, 0x82092DCC, [&](uint32_t inner) {
    HandlerMapForEach(base, frame + 96, first, last, functor, adjust, inner, calls);
  });
}

std::optional<bool> AppApplyRequest(uint8_t* base, uint32_t app, uint32_t type, uint32_t key,
                                    uint32_t delegate) {
  if (type > 7) {
    ITC_NATIVE_RUNNING("sub_82093BE0 App::ApplyRequest");
    return false;
  }
  if (type % 2 == 1) {
    return std::nullopt;  // a handler's removal: never seen in play
  }
  const schema::App a(base, app);
  const uint32_t maps[4] = {a.handlers_address(), a.toggled_handlers_address(),
                            a.always_handlers_address(), a.listeners_address()};
  if (!framework::MapInsertUniqueOf(base, maps[type / 2], key, delegate)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82093BE0 App::ApplyRequest");
  return true;
}

std::optional<bool> AppAddRequest(uint8_t* base, uint32_t interface, uint32_t type, uint32_t key,
                                  uint32_t delegate, uint32_t request, uint32_t clone_return,
                                  const AppCalls& calls) {
  switch (type) {
    case 0:
      ITC_NATIVE_RUNNING("sub_82094830 App::AddHandler");
      break;
    case 2:
      ITC_NATIVE_RUNNING("sub_820948D0 App::AddToggledHandler");
      break;
    case 4:
      ITC_NATIVE_RUNNING("sub_82094970 App::AddAlwaysHandler");
      break;
    default:
      ITC_NATIVE_RUNNING("sub_82094A10 App::AddListener");
      break;
  }
  const schema::Request r(base, request);
  r.set_type(type);
  r.set_key(schema::AppHandlerKey(base, key).key());
  r.set_delegate(calls.clone(delegate, clone_return));
  return framework::RequestsPushBack(base, schema::AppInterface(base, interface).requests_address(),
                                     request);
}

namespace {

// App::Update's walk of a handler map with App::RunHandler (`frame` its
// frame): the words stored as the original's.
void WalkHandlers(uint8_t* base, uint32_t app, uint32_t map, uint32_t frame,
                  uint32_t return_address, const AppCalls& calls) {
  Store(base, frame + 100, 0);
  Store(base, frame + 104, app);
  Store(base, frame + 80, map);
  Store(base, frame + 96, kRunHandler);
  const uint32_t head = schema::HandlerMap(base, map).head().address;
  const uint64_t functor = Load64(base, frame + 96);
  const uint64_t adjust = Load64(base, frame + 104);  // its low word as the stack holds it
  Store(base, frame + 96, map);
  const uint32_t left = schema::AppHandlerMapHead(base, head).left();
  Store(base, frame + 100, head);
  const uint64_t last = Load64(base, frame + 96);
  Store(base, frame + 84, left);
  const uint64_t first = Load64(base, frame + 80);
  calls.in_frame(112, return_address, [&](uint32_t inner) {
    HandlerMapForEach(base, frame + 96, first, last, functor, adjust, inner, calls);
  });
}

// App::Update's walk of a vector (`vec`, a MessageVector or a
// RequestVector) with `function`: the words stored as the original's; the
// range and functor handed to `walk`.
template <typename Vector, typename Walk>
void WalkVector(uint8_t* base, uint32_t app, Vector vec, uint32_t function, uint32_t frame,
                Walk walk) {
  const uint32_t vector = vec.address;
  Store(base, frame + 100, 0);
  Store(base, frame + 88, app);
  Store(base, frame + 96, function);
  const uint64_t functor_word = Load64(base, frame + 96);
  Store64(base, frame + 80, functor_word);
  if (vec.first().address > vec.last().address) {
    Trap("sub_82093DC0");
  }
  Store(base, frame + 100, vec.last().address);
  const uint32_t first = vec.first().address;
  Store(base, frame + 96, vector);
  if (first > vec.last().address) {
    Trap("sub_82093DC0");
  }
  const uint64_t functor = Load64(base, frame + 80);
  const uint64_t adjust = Load64(base, frame + 88);  // its low word as the stack holds it
  Store(base, frame + 84, first);
  Store(base, frame + 80, vector);
  walk(Load64(base, frame + 80), Load64(base, frame + 96), functor, adjust);
}

}  // namespace

bool AppUpdate(uint8_t* base, uint32_t app, uint32_t frame, const AppCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82093DC0 App::Update");
  calls.set_register(31, app);
  calls.set_register(29, 0);
  const schema::App a(base, app);
  // Player 1's processor.
  const uint32_t source = a.framework_input().address;
  Store(base, frame + 80, 0);
  const uint32_t manager =
      calls.call(VirtualTarget(base, source, 4), source, std::nullopt, std::nullopt, 0x82093DEC);
  const uint32_t processor = calls.call(VirtualTarget(base, manager, 4), manager,
                                        uint64_t(frame + 80), std::nullopt, 0x82093E00);
  calls.set_register(30, processor);
  // The toggle (Start) and the step (Back held or repeating while it's set).
  const uint32_t interface = a.interface_vtable_address();
  const auto send = [&](uint32_t message, uint32_t return_address) {
    Store(base, frame + 96, message);
    calls.call(VirtualTarget(base, interface, 40), interface, uint64_t(frame + 96), std::nullopt,
               return_address);
  };
  Store(base, frame + 96, 0x100000);
  if (a.toggled() != 0) {
    if ((calls.call(0x8258CEB0, processor, uint64_t(frame + 96), std::nullopt, 0x82093E20) &
         0xFF) != 0) {
      send(2, 0x82093E48);
      a.set_toggled(0);
    }
    Store(base, frame + 96, 0x200000);
    if ((calls.call(0x8258CEE0, processor, uint64_t(frame + 96), std::nullopt, 0x82093E60) &
         0xFF) != 0) {
      calls.set_register(30, 1);
      send(1, 0x82093E88);
      a.set_step(1);
    }
  } else if ((calls.call(0x8258CEB0, processor, uint64_t(frame + 96), std::nullopt,
                         0x82093E94) &
              0xFF) != 0) {
    send(0, 0x82093EB8);
    a.set_toggled(1);
  }
  // The handler maps.
  if (a.toggled() != 0 && a.step() == 0) {
    WalkHandlers(base, app, a.toggled_handlers_address(), frame, 0x82093F38, calls);
  } else {
    WalkHandlers(base, app, a.handlers_address(), frame, 0x82093F9C, calls);
    a.set_step(0);
  }
  WalkHandlers(base, app, a.always_handlers_address(), frame, 0x82094000, calls);
  // The messages.
  const uint32_t messages = a.messages_address();
  calls.set_register(30, messages);
  WalkVector(base, app, schema::MessageVector(base, messages), kDeliverMessage, frame,
             [&](uint64_t first, uint64_t last, uint64_t functor, uint64_t adjust) {
               calls.in_frame(128, 0x8209407C, [&](uint32_t inner) {
                 ForEachMessage(base, frame + 96, first, last, functor, adjust, inner, calls);
               });
             });
  if (!MessagesClear(base, messages)) {
    calls.call(0x82092F98, messages, std::nullopt, std::nullopt, 0x82094084);
  }
  // The requests.
  const uint32_t requests = a.requests_address();
  calls.set_register(30, requests);
  WalkVector(base, app, schema::RequestVector(base, requests), kApplyRequest, frame,
             [&](uint64_t first, uint64_t last, uint64_t functor, uint64_t adjust) {
               calls.in_frame(128, 0x82094100, [&](uint32_t inner) {
                 ForEachRequest(base, frame + 96, first, last, functor, adjust, inner, calls);
               });
             });
  if (framework::RequestsClear(base, requests)) {
    return true;
  }
  calls.call(0x8209A080, requests, std::nullopt, std::nullopt, 0x82094108);
  return false;
}

std::optional<bool> AppSendMessage(uint8_t* base, uint32_t interface, uint32_t message) {
  const auto inserted =
      MessagesPushBack(base, schema::AppInterface(base, interface).messages_address(), message);
  if (inserted) {
    ITC_NATIVE_RUNNING("sub_82094AB0 App::SendMessage");
  }
  return inserted;
}

}  // namespace app_states
