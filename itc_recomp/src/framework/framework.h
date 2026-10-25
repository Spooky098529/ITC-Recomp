// The app framework natively (docs/functions/framework.md): the framework
// object's getters and its interfaces' this-adjusting thunks, the debug
// text's position. Plain C++ over the guest layout; the adapters are in
// src/hooks/framework.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace framework {

// The framework's flags read by App::Run each pass: the byte at +133
// (sub_82090760) and at +134 (sub_82090768).
uint32_t Flag133(uint8_t* base, uint32_t object);
uint32_t Flag134(uint8_t* base, uint32_t object);

// Getters the linker folded (one function for every class with a word at
// that offset): +60 (sub_82090770), +64 (sub_82090778), +88 (sub_82090788),
// +68 (sub_8248F030), +44 (sub_824892F8).
uint32_t Word60(uint8_t* base, uint32_t object);
uint32_t Word64(uint8_t* base, uint32_t object);
uint32_t Word88(uint8_t* base, uint32_t object);
uint32_t Word68(uint8_t* base, uint32_t object);
uint32_t Word44(uint8_t* base, uint32_t object);

// The interfaces' thunks: `self` is the interface; each adjusts it to the
// framework's sub-object and calls one of the getters above.
uint32_t InputManagerFrom36(uint8_t* base, uint32_t self);    // sub_82090830: -12, Word64
uint32_t RendererFrom52(uint8_t* base, uint32_t self);        // sub_82090828: -44, Word68
uint32_t WorkerPoolFrom52(uint8_t* base, uint32_t self);      // sub_82090838: -28, Word44
uint32_t RendererOrTextFrom24(uint8_t* base, uint32_t self);  // sub_82090860: -16, Word68
uint32_t SceneFrom52(uint8_t* base, uint32_t self);           // sub_82090868: -32, Word60
uint32_t RendererFrom36(uint8_t* base, uint32_t self);        // sub_820908E0: -28, Word68
uint32_t WorkerPoolFrom36(uint8_t* base, uint32_t self);      // sub_820908E8: -12, Word44
uint32_t SceneFrom36(uint8_t* base, uint32_t self);           // sub_82090908: -16, Word60

// The debug font's cursor and line start (0x8296BCE0: +0, +4 and +8, +12)
// set to (x, y) (sub_8258BE08, DebugFont::SetPosition).
constexpr uint32_t kDebugFontPosition = 0x8296BCE0;
void DebugFontSetPosition(uint8_t* base, uint32_t x, uint32_t y);
// DebugText::SetPosition (sub_82091050, vtable slot 2): `x` and `y` point to
// the words.
void DebugTextSetPosition(uint8_t* base, uint32_t x, uint32_t y);

// The word in `object`'s vtable at `offset`: the target of a virtual call
// (the thunks and dispatchers below make it as a guest call).
uint32_t VirtualTarget(uint8_t* base, uint32_t object, uint32_t offset);

// sub_82096330 and sub_82096340: a delegate's target, the object's vtable
// slot 0 or 1 (tail calls).
uint32_t DelegateSlot0Target(uint8_t* base, uint32_t object);
uint32_t DelegateSlot1Target(uint8_t* base, uint32_t object);
// sub_82099A98 TickHandler::Invoke: the delegate (the map node's low word)
// and its slot 1, called with r3 = the delegate, r4 = the run loop.
uint32_t TickHandlerTarget(uint8_t* base, uint32_t delegate);
// sub_820902B0 Framework::Start: the app (+120) and its slot +36 (a tail
// call): {object, target}.
struct VirtualCall {
  uint32_t object;
  uint32_t target;
};
VirtualCall FrameworkStartCall(uint8_t* base, uint32_t framework);
// sub_82094350: the app's input source: the word at self + 20 (the
// framework's +36 interface) and its slot 1 (a tail call; in play
// InputManagerFrom36, sub_82090830).
VirtualCall AppInputSourceCall(uint8_t* base, uint32_t self);
// sub_820944C8: self - 12, then sub_82094350.
VirtualCall AppInputSourceFrom16Call(uint8_t* base, uint32_t self);

// sub_820902C8 Framework::OnTick: the input manager's (+88) slot 1 with a
// key of 0 at `key` (InputManager::GetProcessor, the result not used), +133
// cleared, the app's (+120) slot +44; when its low byte is 0, tick handler
// 7 removed through the run loop's slot 4 (the key 7 at `key`) and +134
// set. `key` is a word in the caller's frame (the original's r1 + 80).
struct OnTickCalls {
  std::function<void(uint32_t manager, uint32_t key)> get_processor;
  std::function<uint32_t(uint32_t app)> app_continues;
  std::function<void(uint32_t loop, uint32_t key)> remove_tick_handler;
};
void FrameworkOnTick(uint8_t* base, uint32_t framework, uint32_t loop, uint32_t key,
                     const OnTickCalls& calls);

// sub_8209AAA8 RunLoop::AddTickHandler (type 0) and sub_8209AB48
// AddListener (type 2): the request {type, the key at `key`, the delegate's
// copy (its slot 2, `clone`)} built at `request` (the original's r1 + 80) and
// pushed on the request vector (+28; RequestVector::PushBack). Whether
// PushBack took Insert, or nullopt when PushBack hands back (the request is
// then built: the caller runs the original PushBack on it).
std::optional<bool> RunLoopAddRequest(uint8_t* base, uint32_t loop, uint32_t type, uint32_t key,
                                      uint32_t delegate, uint32_t request,
                                      const std::function<uint32_t(uint32_t)>& clone);

// sub_82090420 Framework::Framework: the sixteen vtables (+0 to +60, the
// base classes' first, then the framework's own: only the last stay), the
// services' words and the flags cleared. +84 and +108 to +120 are left.
void FrameworkConstruct(uint8_t* base, uint32_t framework);
// sub_820909B8 Framework::Create: operator new(136) and the framework built
// there, stored (or 0) at `holder`. nullopt where operator new fails (the
// original's handler path) or without a heap.
std::optional<uint32_t> FrameworkCreate(uint8_t* base, uint32_t holder);

// sub_820AC8F8 PoolMaker::PoolMaker and sub_820AC848: the vtable
// (0x82002848 or 0x82002840) and at +4 the interface's slot 1 (the second
// worker pool, `pool`, a guest call). Returns the object.
uint32_t PoolMakerConstruct(uint8_t* base, uint32_t maker, uint32_t vtable, uint32_t interface,
                            const std::function<uint32_t(uint32_t)>& pool);

// sub_82092710 FrameStart::FrameStart: the vtable and the block's interface,
// then tick handler 4 (a delegate calling sub_82096330 with it, built at
// `frame` + 88) and a listener for event 2 (sub_82096340, at `frame` + 104)
// added through the run loop's slots 2 and 1 (`add`, guest calls; the key
// at `frame` + 80). `frame` is the original's 144-byte frame. Returns it.
uint32_t FrameStartConstruct(uint8_t* base, uint32_t frame_start, uint32_t block, uint32_t frame,
                             const std::function<void(uint32_t loop, uint32_t offset,
                                                      uint32_t key, uint32_t delegate)>& add);

// sub_82090EA8 DebugText::DebugText: the vtable, the block's interface, the
// debug font's load job at +8 (its state, +20, the word the original reads
// from its own frame, `stack_word`, never set: given here), +48 cleared,
// itself into 0x8266D080, and the job posted to the worker pool (the block's
// +4 interface's slot 1, `post`, a guest call with r5 = 1).
constexpr uint32_t kDebugTextGlobal = 0x8266D080;
uint32_t DebugTextConstruct(uint8_t* base, uint32_t text, uint32_t block, uint32_t stack_word,
                            const std::function<void(uint32_t interface, uint32_t job)>& post);
// sub_82091160 DebugText::Create: operator new(52) and the debug text built
// there (or 0). nullopt without a heap.
std::optional<uint32_t> DebugTextCreate(
    uint8_t* base, uint32_t block, uint32_t stack_word,
    const std::function<void(uint32_t interface, uint32_t job)>& post);

}  // namespace framework
