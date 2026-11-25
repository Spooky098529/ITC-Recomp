// The game's entry and its main loop natively (docs/functions/framework.md).

#include "framework/main_loop.h"

#include "guest.h"

#include "base/log.h"
#include "framework/framework.h"
#include "framework/run_loop.h"
#include "schema/framework.h"

namespace framework {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
// A doubleword as the PowerPC stores it (the high word first).
uint64_t Load64(uint32_t address) {
  return (uint64_t(Load(address)) << 32) | Load(address + 4);
}
void Store64(uint32_t address, uint64_t value) {
  Store(address, uint32_t(value >> 32));
  Store(address + 4, uint32_t(value));
}
// The functor std::for_each returns, stored at `result` a word at a time.
void StoreFunctor(uint32_t result, uint64_t functor) {
  const schema::FrameworkMemberFunctor stored(result);
  stored.set_function(uint32_t(functor >> 32));
  stored.set_object(uint32_t(functor));
}

// The original's tw 22 (a failed check), which the recompiled code logs and
// goes on from.
void Trap(const char* function) {
  ITC_LOG_WARN("{}: tw/td trap hit (type 22)", function);
}

// The functions App::Run hands ForEachTickHandler and ForEachRequest.
constexpr uint32_t kTickHandlerInvoke = 0x82099A98;
constexpr uint32_t kApplyRequest = 0x8209A458;

}  // namespace

uint32_t ForEachTickHandler(uint32_t result, uint64_t first, uint64_t last,
                            uint64_t functor, uint32_t frame, const MainLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82099D50 ForEachTickHandler");
  calls.set_register(30, functor);
  calls.set_register(31, result);
  Store64(frame + 160, last);
  Store64(frame + 80, first);
  Store64(frame + 168, functor);
  for (;;) {
    const uint32_t map = Load(frame + 80);
    if (map == 0 || map != Load(frame + 160)) {
      Trap("sub_82099D50");
    }
    const uint32_t node = Load(frame + 84);
    if (node == Load(frame + 164)) {
      break;
    }
    if (map == 0) {
      Trap("sub_82099D50");
    }
    if (node == schema::HandlerMap(map).head().address) {
      Trap("sub_82099D50");
    }
    const schema::HandlerMapNode handler(node);
    const uint32_t key = handler.key();
    const uint32_t delegate = handler.delegate().address;
    Store(frame + 88, key);
    Store(frame + 92, delegate);
    calls.call(Load(frame + 168), Load(frame + 172), Load64(frame + 88),
               std::nullopt, 0x82099DF4);
    Iterator it{Load(frame + 80), Load(frame + 84)};
    if (MapIncrement(it)) {
      Store(frame + 84, it.node);
    } else {
      calls.increment(frame + 80);
    }
  }
  StoreFunctor(result, functor);
  return result;
}

uint32_t ForEachRequest(uint32_t result, uint64_t first, uint64_t last,
                        uint64_t functor, uint32_t frame, const MainLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82099EE8 ForEachRequest");
  Store64(frame + 152, first);
  calls.set_register(28, functor);
  Store64(frame + 160, last);
  calls.set_register(29, result);
  Store64(frame + 168, functor);
  const uint32_t end = Load(frame + 164);
  calls.set_register(30, end);
  for (uint32_t at = Load(frame + 156); at != end; at += kRequestBytes) {
    calls.set_register(31, at);
    const schema::Request request(at);
    const uint64_t delegate = uint64_t(request.delegate().address) << 32;
    calls.call(Load(frame + 168), Load(frame + 172),
               (uint64_t(request.type()) << 32) | request.key(), delegate, 0x82099F30);
  }
  StoreFunctor(result, functor);
  return result;
}

uint32_t AppRun(uint32_t loop, uint32_t framework, uint32_t frame,
                const MainLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209A760 App::Run");
  // The framework's slots (its vtable read for each call).
  using Slot = uint32_t (schema::FrameworkVtable::*)() const;
  const auto slot = [&](Slot target, std::optional<uint64_t> r4, uint32_t return_address) {
    const schema::FrameworkVtable vtable = schema::Framework(framework).vtable();
    return calls.call((vtable.*target)(), framework, r4, std::nullopt, return_address);
  };
  // The run loop's slot 7 (an event: the word at frame + 80).
  const schema::RunLoop run_loop(loop);
  const auto event = [&](uint32_t value, uint32_t return_address) {
    Store(frame + 80, value);
    calls.call(run_loop.vtable().send_event(), loop, frame + 80, std::nullopt, return_address);
  };
  calls.set_register(30, framework);
  calls.set_register(31, loop);
  if ((slot(&schema::FrameworkVtable::init, loop, 0x8209A78C) & 0xFF) == 0) {
    return 0;
  }
  slot(&schema::FrameworkVtable::start, loop, 0x8209A7AC);
  const uint32_t map = run_loop.tick_handlers_address();
  const uint32_t vector = run_loop.requests_address();
  const schema::HandlerMap handlers(map);
  const schema::RequestVector requests(vector);
  Store(frame + 100, loop);
  Store(frame + 92, loop);
  Store(frame + 112, map);
  Store(frame + 96, kTickHandlerInvoke);
  Store(frame + 88, kApplyRequest);
  calls.set_register(28, map);
  calls.set_register(27, frame + 88);
  calls.set_register(26, frame + 120);
  calls.set_register(29, vector);
  do {
    // The tick handlers: begin (the head's left) to end (the head).
    const uint32_t head = handlers.head().address;
    Store(frame + 104, map);
    Store(frame + 108, head);
    Store(frame + 116, schema::HandlerMapNode(head).left().address);
    {
      const uint64_t functor = Load64(frame + 96);
      const uint64_t last = Load64(frame + 104);
      const uint64_t first = Load64(frame + 112);
      calls.in_frame(128, [&](uint32_t inner) {
        ForEachTickHandler(frame + 136, first, last, functor, inner, calls);
      });
    }
    // The requests: their checked iterators (first past last traps).
    if (requests.first().address > requests.last().address) {
      Trap("sub_8209A760");
    }
    Store(frame + 124, requests.last().address);
    const uint32_t first_request = requests.first().address;
    Store(frame + 120, vector);
    if (first_request > requests.last().address) {
      Trap("sub_8209A760");
    }
    Store(frame + 132, first_request);
    Store(frame + 128, vector);
    {
      const uint64_t functor = Load64(frame + 88);
      const uint64_t last = Load64(frame + 120);
      const uint64_t first = Load64(frame + 128);
      calls.in_frame(128, [&](uint32_t inner) {
        ForEachRequest(frame + 144, first, last, functor, inner, calls);
      });
    }
    if (!RequestsClear(vector)) {
      calls.clear(vector);
    }
    if ((slot(&schema::FrameworkVtable::app_finished, std::nullopt, 0x8209A884) & 0xFF) != 0) {
      event(1, 0x8209A8FC);
    } else if ((slot(&schema::FrameworkVtable::restart_requested, std::nullopt, 0x8209A8BC) &
                0xFF) != 0) {
      event(0, 0x8209A8E4);
      slot(&schema::FrameworkVtable::start, loop, 0x8209A8FC);
    }
  } while (handlers.size() != 0);
  slot(&schema::FrameworkVtable::release, loop, 0x8209A920);
  return 0;
}

uint32_t RunFramework(uint32_t framework, uint32_t frame,
                      const MainLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209ABE8 RunFramework");
  const uint32_t loop = frame + 80;
  calls.set_register(31, framework);
  calls.construct_run_loop(loop);
  uint32_t answer = 0;
  calls.in_frame(208, [&](uint32_t inner) { answer = AppRun(loop, framework, inner, calls); });
  calls.set_register(31, answer);
  calls.destroy_run_loop(loop);
  return answer;
}

uint32_t RunFrameworkAt(uint32_t holder, const MainLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82090378 RunFrameworkAt");
  const uint32_t framework = schema::FrameworkHolder(holder).framework().address;
  uint32_t answer = 0;
  calls.in_frame(160, [&](uint32_t frame) { answer = RunFramework(framework, frame, calls); });
  return answer;
}

uint32_t Main(uint32_t frame, const MainLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82090108 main");
  const uint32_t holder = frame + 80;
  calls.create_framework(holder);
  const uint32_t answer = RunFrameworkAt(holder, calls);
  calls.set_register(31, answer);
  calls.delete_framework(holder);
  return answer;
}

}  // namespace framework
