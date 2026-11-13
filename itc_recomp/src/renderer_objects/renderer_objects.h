// The renderer at framework + 76 and the app's scene at framework + 80 as
// plain C++ (B7 in docs/rexglue-removal.md; the functions are in
// docs/functions/renderer.md, the fields in docs/known-data.md, "renderer"
// and "app scene"). Both are guest objects the recompiled code shares
// (Renderer::Init, WaitForFrame and StartFrame stay recompiled, blocked by
// the core's Event:: functions, the user's call 2026-10-24), so these work
// on guest memory with the originals' loads and stores. Their virtual calls
// and functors stay guest calls, made through app_states::AppCalls by the
// adapters (src/hooks/renderer_objects.cpp), from frames like the
// originals'.

#pragma once

#include <cstdint>
#include <optional>

#include "app_states/app.h"

namespace renderer_objects {

// The renderer (400 bytes; `Renderer::Create` keeps renderer + 20).
constexpr uint32_t kRendererInterface = 20;  // vtable 0x82000D6C
constexpr uint32_t kRendererListeners = 28;  // a HandlerMap
constexpr uint32_t kRendererRequests = 40;   // the request vector (interface + 20)
constexpr uint32_t kRendererVideoOutput = 64;
constexpr uint32_t kRendererDevice = 384;    // the device while a frame is drawn
// The functor Renderer::DrawFrame hands HandlerMap::ForEach.
constexpr uint32_t kCallListener = 0x82091A98;

// sub_824960E0 Renderer::GetVideoOutput (the interface's slot 3; folded:
// any `this + 44` getter): `object` + 44.
uint32_t GetVideoOutput(uint32_t object);

// sub_820922B8 Renderer::AddListener (the interface's slot 1): the request
// {0, the key at `key`, the delegate's copy (its slot 2, `calls.clone` with
// the return address 0x820922F0)} built at `request` (the original's r1 +
// 80) and pushed on the request vector (interface + 20). Whether PushBack
// took Insert, or nullopt when it hands back (the request is then built:
// the caller runs the original PushBack on it).
std::optional<bool> AddListener(uint8_t* base, uint32_t interface, uint32_t key,
                                uint32_t delegate, uint32_t request,
                                const app_states::AppCalls& calls);

// sub_82091FF0 Renderer::ApplyRequest (Renderer::WaitForFrame's functor):
// type 0 inserts the delegate under the key in the listener map
// (HandlerMap::Insert); types above 1 do nothing. Whether it inserted, or
// nullopt for type 1 (a listener's removal, never seen in play: the
// original runs, as RunLoop::ApplyRequest's and App::ApplyRequest's
// removals) or an insert handed back.
std::optional<bool> ApplyRequest(uint8_t* base, uint32_t renderer, uint32_t type, uint32_t key,
                                 uint32_t delegate);

// sub_82091A98 Renderer::CallListener (DrawFrame's functor): `entry` (the
// map node's key in the high word, its delegate in the low) stored at the
// caller's r1 + 24 (`frame` + 120, `frame` the original's 96-byte frame),
// then the delegate's slot 1 with r4 = renderer + 20 (0 for no renderer)
// and r5 = the device (+384), returning to 0x82091AD4. Its r3.
uint32_t CallListener(uint8_t* base, uint32_t renderer, uint64_t entry, uint32_t frame,
                      const app_states::AppCalls& calls);

// sub_82091E58 Renderer::DrawFrame (the render thread's frame): the device
// locked (VideoOutput::LockDevice on +64) and kept at +384,
// HandlerMap::ForEach over the listener map with Renderer::CallListener
// (the iterators and the functor stored in `frame`, the original's 144-byte
// frame, as it stores them; r29 to r31 as it sets them), then the device
// unlocked with the word at +384.
void DrawFrame(uint8_t* base, uint32_t renderer, uint32_t frame,
               const app_states::AppCalls& calls);

}  // namespace renderer_objects
