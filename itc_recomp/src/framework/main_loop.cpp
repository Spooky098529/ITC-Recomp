// The game's entry and its main loop natively (docs/functions/framework.md).

#include "framework/main_loop.h"

#include "guest.h"

#include "base/log.h"
#include "framework/framework.h"
#include "framework/run_loop.h"

namespace framework {

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

// The original's tw 22 (a failed check), which the recompiled code logs and
// goes on from.
void Trap(const char* function) {
  ITC_LOG_WARN("{}: tw/td trap hit (type 22)", function);
}

// The functions App::Run hands ForEachTickHandler and ForEachRequest.
constexpr uint32_t kTickHandlerInvoke = 0x82099A98;
constexpr uint32_t kApplyRequest = 0x8209A458;

}  // namespace

uint32_t ForEachTickHandler(uint8_t* base, uint32_t result, uint64_t first, uint64_t last,
                            uint64_t functor, uint32_t frame, const MainLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82099D50 ForEachTickHandler");
  calls.set_register(30, functor);
  calls.set_register(31, result);
  Store64(base, frame + 160, last);
  Store64(base, frame + 80, first);
  Store64(base, frame + 168, functor);
  for (;;) {
    const uint32_t map = Load(base, frame + 80);
    if (map == 0 || map != Load(base, frame + 160)) {
      Trap("sub_82099D50");
    }
    const uint32_t node = Load(base, frame + 84);
    if (node == Load(base, frame + 164)) {
      break;
    }
    if (map == 0) {
      Trap("sub_82099D50");
    }
    if (node == Load(base, map + kMapHead)) {
      Trap("sub_82099D50");
    }
    const uint32_t key = Load(base, node + kNodeKey);
    const uint32_t delegate = Load(base, node + kNodeDelegate);
    Store(base, frame + 88, key);
    Store(base, frame + 92, delegate);
    calls.call(Load(base, frame + 168), Load(base, frame + 172), Load64(base, frame + 88),
               std::nullopt, 0x82099DF4);
    Iterator it{Load(base, frame + 80), Load(base, frame + 84)};
    if (MapIncrement(base, it)) {
      Store(base, frame + 84, it.node);
    } else {
      calls.increment(frame + 80);
    }
  }
  Store64(base, result, functor);
  return result;
}

uint32_t ForEachRequest(uint8_t* base, uint32_t result, uint64_t first, uint64_t last,
                        uint64_t functor, uint32_t frame, const MainLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82099EE8 ForEachRequest");
  Store64(base, frame + 152, first);
  calls.set_register(28, functor);
  Store64(base, frame + 160, last);
  calls.set_register(29, result);
  Store64(base, frame + 168, functor);
  const uint32_t end = Load(base, frame + 164);
  calls.set_register(30, end);
  for (uint32_t at = Load(base, frame + 156); at != end; at += kRequestBytes) {
    calls.set_register(31, at);
    const uint64_t delegate = uint64_t(Load(base, at + 8)) << 32;
    calls.call(Load(base, frame + 168), Load(base, frame + 172), Load64(base, at), delegate,
               0x82099F30);
  }
  Store64(base, result, functor);
  return result;
}

uint32_t AppRun(uint8_t* base, uint32_t loop, uint32_t framework, uint32_t frame,
                const MainLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209A760 App::Run");
  // The framework's slots (its vtable read for each call).
  const auto slot = [&](uint32_t offset, std::optional<uint64_t> r4, uint32_t return_address) {
    return calls.call(VirtualTarget(base, framework, offset), framework, r4, std::nullopt,
                      return_address);
  };
  // The run loop's slot 7 (an event: the word at frame + 80).
  const auto event = [&](uint32_t value, uint32_t return_address) {
    Store(base, frame + 80, value);
    calls.call(VirtualTarget(base, loop, 28), loop, frame + 80, std::nullopt, return_address);
  };
  calls.set_register(30, framework);
  calls.set_register(31, loop);
  if ((slot(4, loop, 0x8209A78C) & 0xFF) == 0) {
    return 0;
  }
  slot(8, loop, 0x8209A7AC);
  const uint32_t map = loop + kTickHandlers;
  const uint32_t vector = loop + kRequests;
  Store(base, frame + 100, loop);
  Store(base, frame + 92, loop);
  Store(base, frame + 112, map);
  Store(base, frame + 96, kTickHandlerInvoke);
  Store(base, frame + 88, kApplyRequest);
  calls.set_register(28, map);
  calls.set_register(27, frame + 88);
  calls.set_register(26, frame + 120);
  calls.set_register(29, vector);
  do {
    // The tick handlers: begin (the head's left) to end (the head).
    const uint32_t head = Load(base, map + kMapHead);
    Store(base, frame + 104, map);
    Store(base, frame + 108, head);
    Store(base, frame + 116, Load(base, head + kNodeLeft));
    {
      const uint64_t functor = Load64(base, frame + 96);
      const uint64_t last = Load64(base, frame + 104);
      const uint64_t first = Load64(base, frame + 112);
      calls.in_frame(128, [&](uint32_t inner) {
        ForEachTickHandler(base, frame + 136, first, last, functor, inner, calls);
      });
    }
    // The requests: their checked iterators (first past last traps).
    if (Load(base, vector + kVectorFirst) > Load(base, vector + kVectorLast)) {
      Trap("sub_8209A760");
    }
    Store(base, frame + 124, Load(base, vector + kVectorLast));
    const uint32_t first_request = Load(base, vector + kVectorFirst);
    Store(base, frame + 120, vector);
    if (first_request > Load(base, vector + kVectorLast)) {
      Trap("sub_8209A760");
    }
    Store(base, frame + 132, first_request);
    Store(base, frame + 128, vector);
    {
      const uint64_t functor = Load64(base, frame + 88);
      const uint64_t last = Load64(base, frame + 120);
      const uint64_t first = Load64(base, frame + 128);
      calls.in_frame(128, [&](uint32_t inner) {
        ForEachRequest(base, frame + 144, first, last, functor, inner, calls);
      });
    }
    if (!RequestsClear(base, vector)) {
      calls.clear(vector);
    }
    if ((slot(20, std::nullopt, 0x8209A884) & 0xFF) != 0) {
      event(1, 0x8209A8FC);
    } else if ((slot(16, std::nullopt, 0x8209A8BC) & 0xFF) != 0) {
      event(0, 0x8209A8E4);
      slot(8, loop, 0x8209A8FC);
    }
  } while (Load(base, loop + kTickHandlers + kMapSize) != 0);
  slot(12, loop, 0x8209A920);
  return 0;
}

uint32_t RunFramework(uint8_t* base, uint32_t framework, uint32_t frame,
                      const MainLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209ABE8 RunFramework");
  const uint32_t loop = frame + 80;
  calls.set_register(31, framework);
  calls.construct_run_loop(loop);
  uint32_t answer = 0;
  calls.in_frame(208, [&](uint32_t inner) { answer = AppRun(base, loop, framework, inner, calls); });
  calls.set_register(31, answer);
  calls.destroy_run_loop(loop);
  return answer;
}

uint32_t RunFrameworkAt(uint8_t* base, uint32_t holder, const MainLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82090378 RunFrameworkAt");
  const uint32_t framework = Load(base, holder);
  uint32_t answer = 0;
  calls.in_frame(160, [&](uint32_t frame) { answer = RunFramework(base, framework, frame, calls); });
  return answer;
}

uint32_t Main(uint8_t* base, uint32_t frame, const MainLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82090108 main");
  const uint32_t holder = frame + 80;
  calls.create_framework(holder);
  const uint32_t answer = RunFrameworkAt(base, holder, calls);
  calls.set_register(31, answer);
  calls.delete_framework(holder);
  return answer;
}

}  // namespace framework
