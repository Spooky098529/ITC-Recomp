// The app's update, its interface's slots and its state stack natively
// (docs/functions/app-states.md): plain C++ over the guest layout. Every
// virtual call whose target isn't one of ours stays a guest call, made
// through AppCalls by the adapters (src/hooks/app_states.cpp); where the
// target is ours (in play), its plain C++ runs instead.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace app_states {

// The app's state stack (AppStates::, 12 bytes): +4 the app, +8 the root
// layer.
constexpr uint32_t kStatesRoot = 8;
// The app's +20 interface: +96 the state stack (the app + 116), +56 the
// message vector (the app + 76).
constexpr uint32_t kInterfaceStates = 96;
constexpr uint32_t kInterfaceMessages = 56;

// The targets ours in play.
constexpr uint32_t kAppStatesIsEmpty = 0x820AE1A0;
constexpr uint32_t kLayerIsEmpty = 0x820ACE50;

struct AppCalls {
  // A guest call of `target` with r3 = `r3`, r4 and r5 when given (else left
  // as they are) and the link register `return_address`; its r3. A tail
  // call when `return_address` is 0 (the link register left as it is).
  std::function<uint32_t(uint32_t target, uint32_t r3, std::optional<uint64_t> r4,
                         std::optional<uint64_t> r5, uint32_t return_address)>
      call;
  // `body(frame)` run in a guest frame of `bytes` below the stack pointer
  // (its back chain stored), as the original's, with the link register
  // `return_address` (the original's call's).
  std::function<void(uint32_t bytes, uint32_t return_address,
                     const std::function<void(uint32_t frame)>& body)>
      in_frame;
  // A nonvolatile register set as the original sets it (its callees see
  // the original's values there; in_frame puts r14 to r31 back after its
  // body, as the original's epilogue does).
  std::function<void(int reg, uint64_t value)> set_register;
  // The map iterator's operator++ (sub_82099B18) on the iterator at
  // `iterator` with the link register `return_address`, where
  // framework::MapIncrement hands back (the original's trap).
  std::function<void(uint32_t iterator, uint32_t return_address)> increment;
  // The delegate's copy (its slot 2) for a handler request: ours where the
  // slot is Delegate::Clone and the heap allows, else a guest call with the
  // link register `return_address`.
  std::function<uint32_t(uint32_t delegate, uint32_t return_address)> clone;
  // Whether --native_heap routes the game's heap to the allocation layer
  // (the plain C++ that allocates or frees runs only then).
  bool native_heap = false;
};

// The target of `object`'s vtable slot at byte `offset`.
uint32_t VirtualTarget(uint8_t* base, uint32_t object, uint32_t offset);

// sub_820AE1A0 AppStates::IsEmpty (the stack's slot 3): the root layer's
// state (+4) slot 11 as a tail call; its r3.
uint32_t StatesIsEmpty(uint8_t* base, uint32_t states, const AppCalls& calls);
// sub_82092928 App::IsRunning (the +20 interface's slot +44): 1 while the
// state stack's slot 3 answers 0 (in its low byte), else 0; the call from
// a 96-byte frame.
uint32_t AppIsRunning(uint8_t* base, uint32_t interface, const AppCalls& calls);
// sub_82094AB0 App::SendMessage (the +20 interface's slot +40): the word at
// `message` pushed on the message vector (interface + 56). Whether it took
// Insert, or nullopt where MessageVector::PushBack hands back.
std::optional<bool> AppSendMessage(uint8_t* base, uint32_t interface, uint32_t message);

// The app's handler maps: +28 (run while the toggle +112 is clear or a
// step was asked), +40 (while it's set), +52 (every pass), +64 (the
// listeners); the request vector at +96, the message vector at +76, the
// message being delivered at +92.
constexpr uint32_t kAppMaps[4] = {28, 40, 52, 64};
constexpr uint32_t kAppRequests = 96;
constexpr uint32_t kAppMessages = 76;
constexpr uint32_t kAppMessage = 92;
constexpr uint32_t kAppInterface = 20;
constexpr uint32_t kInterfaceRequests = 76;  // the app + 96
// The functors App::Update and DeliverMessage hand the walks.
constexpr uint32_t kRunHandler = 0x82092968;
constexpr uint32_t kRunListener = 0x820929B0;
constexpr uint32_t kDeliverMessage = 0x82092D50;
constexpr uint32_t kApplyRequest = 0x82093BE0;

// sub_82092BF0 HandlerMap::ForEach (std::for_each over a handler map with a
// member functor): `first` and `last` the checked iterators (map, node),
// `functor` its function (high word) and object, `adjust` its adjustment
// (high word; the low word as the caller leaves it); each node's key (high
// word) and delegate passed in r4 to the function (r3 the object plus the
// adjustment), stepped in key order. `frame` is the original's 112-byte
// frame (the iterator and the key and delegate there, the range and the
// functor in the caller's above it, as the original stores them). The
// functor's 16 bytes stored at `result`. Where the original traps (a
// checked iterator's tw 22, which the recompiled code only logs), this logs
// a warning and goes on the same way.
void HandlerMapForEach(uint8_t* base, uint32_t result, uint64_t first, uint64_t last,
                       uint64_t functor, uint64_t adjust, uint32_t frame, const AppCalls& calls);
// sub_82092CD0 App::ForEachMessage and sub_82091C48 App::ForEachRequest
// (std::for_each over the message vector, 4-byte steps, and the request
// vector, 12-byte steps): r4 each word (or each request's type and key, its
// delegate in r5's high word), r3 the object plus the adjustment; r28 to r31
// as the original keeps them. `frame` is the original's 128-byte frame.
void ForEachMessage(uint8_t* base, uint32_t result, uint64_t first, uint64_t last,
                    uint64_t functor, uint64_t adjust, uint32_t frame, const AppCalls& calls);
void ForEachRequest(uint8_t* base, uint32_t result, uint64_t first, uint64_t last,
                    uint64_t functor, uint64_t adjust, uint32_t frame, const AppCalls& calls);
// sub_82092968 App::RunHandler and sub_820929B0 App::RunListener: the
// delegate (`entry`'s low word) gets its slot 1 with r4 = the app's +20
// interface (0 for no app) and, for a listener, r5 = the app + 92, from a
// 96-byte frame (`entry` stored at the caller's r1 + 24). Its r3.
uint32_t RunHandler(uint8_t* base, uint32_t app, uint64_t entry, bool listener, uint32_t frame,
                    const AppCalls& calls);
// sub_82092D50 App::DeliverMessage: the app + 92 = `message`, then
// HandlerMapForEach over the listener map (+64) with App::RunListener, from
// the original's 128-byte frame (its words stored there as it does).
void DeliverMessage(uint8_t* base, uint32_t app, uint32_t message, uint32_t frame,
                    const AppCalls& calls);
// sub_82093BE0 App::ApplyRequest: types 0, 2, 4 and 6 insert the delegate
// under the key in the maps +28, +40, +52, +64 (HandlerMap::Insert; the
// split-off type 0 piece sub_82093C2C merged); others above 7 do nothing.
// Whether it inserted, or nullopt for the odd types (a removal, never seen
// in play) or an insert handed back.
std::optional<bool> AppApplyRequest(uint8_t* base, uint32_t app, uint32_t type, uint32_t key,
                                    uint32_t delegate);
// sub_82094830 App::AddHandler (type 0), sub_820948D0 AddToggledHandler
// (2), sub_82094970 AddAlwaysHandler (4), sub_82094A10 AddListener (6), the
// +20 interface's slots: the request {type, the key at `key`, the
// delegate's copy} built at `request` (the original's r1 + 80) and pushed on
// the request vector (interface + 76). Whether PushBack took Insert, or
// nullopt when it hands back (the request is then built: the caller runs
// the original PushBack on it).
std::optional<bool> AppAddRequest(uint8_t* base, uint32_t interface, uint32_t type, uint32_t key,
                                  uint32_t delegate, uint32_t request, uint32_t clone_return,
                                  const AppCalls& calls);

// sub_82093DC0 App::Update (the run loop's tick handler 8): player 1's
// processor (the app's +24 object's slot 1, then its slot 1 with index 0),
// the toggle +112 and the step +113 from Start and Back (the button tests
// sub_8258CEB0 and sub_8258CEE0, and the +20 interface's slot +40 for the
// messages, as guest calls), the handler map +40 or +28 then +52 walked with
// App::RunHandler, the messages walked with App::DeliverMessage and cleared,
// the requests walked with App::ApplyRequest and cleared. `frame` is the
// original's 144-byte frame (its words stored as the original stores them;
// r29 to r31 as it keeps them). Whether the request vector's clear was ours
// (its r3 then in its frame, `frame` - 16; else the original's).
bool AppUpdate(uint8_t* base, uint32_t app, uint32_t frame, const AppCalls& calls);

}  // namespace app_states
