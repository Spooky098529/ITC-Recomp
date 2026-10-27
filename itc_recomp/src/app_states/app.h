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

}  // namespace app_states
