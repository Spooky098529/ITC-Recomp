// The game's entry and its main loop natively (docs/functions/framework.md):
// `main` (sub_82090108), the framework's run (sub_82090378, sub_8209ABE8),
// App::Run (sub_8209A760) and its helpers ForEachTickHandler (sub_82099D50)
// and ForEachRequest (sub_82099EE8). Plain C++ over the guest layout; every
// virtual call and functor stays a guest call (the tick handlers, the
// framework's slots and the run loop's), made through MainLoopCalls by the
// adapters (src/hooks/framework.cpp), from frames like the originals' (the
// stack words the originals store are stored there too).
//
// Each sets the nonvolatile registers it keeps its values in as the
// original does (r26 to r31 for App::Run), so the guest code it calls sees
// the same registers (a recompiled device setter reads r27's high word).
//
// Where an original traps (a checked iterator's tw 22, which the
// recompiled code only logs before going on), these log a warning and go on
// the same way. None was seen in play.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace framework {

struct MainLoopCalls {
  // A guest call of `target` with r3 = `r3`, r4 and r5 when given (else left
  // as they are) and the link register `return_address`; its r3.
  std::function<uint32_t(uint32_t target, uint32_t r3, std::optional<uint64_t> r4,
                         std::optional<uint64_t> r5, uint32_t return_address)>
      call;
  // `body(frame)` run in a guest frame of `bytes` below the stack pointer
  // (its back chain stored); the nonvolatile registers (r14 to r31) put
  // back after it, as the original's epilogue does.
  std::function<void(uint32_t bytes, const std::function<void(uint32_t frame)>& body)> in_frame;
  // A nonvolatile register set as the original sets it (its callees see
  // the original's values there).
  std::function<void(int reg, uint64_t value)> set_register;
  // The map iterator's operator++ (sub_82099B18) on the iterator at
  // `iterator`, where MapIncrement hands back (the original's trap).
  std::function<void(uint32_t iterator)> increment;
  // RequestVector::Clear (sub_8209A080) where RequestsClear hands back.
  std::function<void(uint32_t vector)> clear;
  // RunLoop::RunLoop (sub_8209A9F0) and Framework::Create (sub_820909B8):
  // their plain C++ where the heap allows (their adapters' condition), else
  // the originals.
  std::function<void(uint32_t loop)> construct_run_loop;
  std::function<void(uint32_t holder)> create_framework;
  // The run loop's destructor (sub_8209A6E0) and the framework's deletion
  // (sub_82090350): never ran (App::Run doesn't return in play), the
  // recompiled functions.
  std::function<void(uint32_t loop)> destroy_run_loop;
  std::function<void(uint32_t holder)> delete_framework;
};

// sub_82099D50 ForEachTickHandler (std::for_each over a handler map with a
// member functor): `first` and `last` the checked iterators (map, node),
// `functor` its function (high word) and object; each node's key (high
// word) and delegate passed in r4 to the function (r3 the object), the
// iterator stepped in key order. `frame` is the original's 128-byte frame
// (the iterators and the functor stored there and in the caller's frame
// above it). The functor stored at `result`; returns `result`.
uint32_t ForEachTickHandler(uint8_t* base, uint32_t result, uint64_t first, uint64_t last,
                            uint64_t functor, uint32_t frame, const MainLoopCalls& calls);
// sub_82099EE8 ForEachRequest (std::for_each over the request vector):
// `first` and `last` the checked iterators (vector, element), 12-byte
// steps; each request's type and key in r4 and its delegate in r5's high
// word passed to the functor's function (r3 its object). `frame` is the
// original's 128-byte frame. The functor stored at `result`; returns it.
uint32_t ForEachRequest(uint8_t* base, uint32_t result, uint64_t first, uint64_t last,
                        uint64_t functor, uint32_t frame, const MainLoopCalls& calls);
// sub_8209A760 App::Run: the framework's slot 1 (Framework::Init; 0
// answers 0), slot 2 (Framework::Start), then each pass the tick handlers
// (TickHandler::Invoke on the run loop), the requests applied
// (RunLoop::ApplyRequest) and cleared, slot 5 (an event of 1 through the
// run loop's slot 7) or slot 4 (an event of 0, then slot 2 again), while
// the tick handler map isn't empty; then slot 3. `frame` is the original's
// 208-byte frame. Returns 0.
uint32_t AppRun(uint8_t* base, uint32_t loop, uint32_t framework, uint32_t frame,
                const MainLoopCalls& calls);
// sub_8209ABE8: the run loop built at `frame` + 80 (the original's 160-byte
// frame), App::Run with the framework, the run loop destroyed; App::Run's
// answer.
uint32_t RunFramework(uint8_t* base, uint32_t framework, uint32_t frame,
                      const MainLoopCalls& calls);
// sub_82090378: the framework at `holder`, run by sub_8209ABE8 (a tail
// call: its frame directly below the caller's).
uint32_t RunFrameworkAt(uint8_t* base, uint32_t holder, const MainLoopCalls& calls);
// sub_82090108 main: the framework made into the holder at `frame` + 80
// (the original's 112-byte frame), run, then deleted; the run's answer.
uint32_t Main(uint8_t* base, uint32_t frame, const MainLoopCalls& calls);

}  // namespace framework
