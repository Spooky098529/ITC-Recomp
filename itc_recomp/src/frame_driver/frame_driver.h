// The frame driver at framework + 76 and the app's scene at framework + 80 as
// plain C++ (B7 in docs/rexglue-removal.md; the functions are in
// docs/functions/frame-driver.md, the fields in docs/known-data.md, "driver"
// and "app scene"). Both are guest objects the recompiled code shares
// (FrameDriver::Init, WaitForFrame and StartFrame stay recompiled, blocked by
// the core's Event:: functions, the user's call 2026-10-24), so these work
// on guest memory with the originals' loads and stores. Their virtual calls
// and functors stay guest calls, made through app_states::AppCalls by the
// adapters (src/hooks/frame_driver.cpp), from frames like the
// originals'.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "app_states/app.h"

namespace frame_driver {

// The frame driver (400 bytes; `FrameDriver::Create` keeps driver + 20).
constexpr uint32_t kDriverInterface = 20;  // vtable 0x82000D6C
constexpr uint32_t kDriverListeners = 28;  // a HandlerMap
constexpr uint32_t kDriverRequests = 40;   // the request vector (interface + 20)
constexpr uint32_t kDriverVideoOutput = 64;
constexpr uint32_t kDriverDevice = 384;    // the device while a frame is drawn
// The functor FrameDriver::DrawFrame hands HandlerMap::ForEach.
constexpr uint32_t kCallListener = 0x82091A98;

// sub_824960E0 FrameDriver::GetVideoOutput (the interface's slot 3; folded:
// any `this + 44` getter): `object` + 44.
uint32_t GetVideoOutput(uint32_t object);

// sub_820922B8 FrameDriver::AddListener (the interface's slot 1): the request
// {0, the key at `key`, the delegate's copy (its slot 2, `calls.clone` with
// the return address 0x820922F0)} built at `request` (the original's r1 +
// 80) and pushed on the request vector (interface + 20). Whether PushBack
// took Insert, or nullopt when it hands back (the request is then built:
// the caller runs the original PushBack on it).
std::optional<bool> AddListener(uint8_t* base, uint32_t interface, uint32_t key,
                                uint32_t delegate, uint32_t request,
                                const app_states::AppCalls& calls);

// sub_82091FF0 FrameDriver::ApplyRequest (FrameDriver::WaitForFrame's functor):
// type 0 inserts the delegate under the key in the listener map
// (HandlerMap::Insert); types above 1 do nothing. Whether it inserted, or
// nullopt for type 1 (a listener's removal, never seen in play: the
// original runs, as RunLoop::ApplyRequest's and App::ApplyRequest's
// removals) or an insert handed back.
std::optional<bool> ApplyRequest(uint8_t* base, uint32_t driver, uint32_t type, uint32_t key,
                                 uint32_t delegate);

// sub_82091A98 FrameDriver::CallListener (DrawFrame's functor): `entry` (the
// map node's key in the high word, its delegate in the low) stored at the
// caller's r1 + 24 (`frame` + 120, `frame` the original's 96-byte frame),
// then the delegate's slot 1 with r4 = driver + 20 (0 for no driver)
// and r5 = the device (+384), returning to 0x82091AD4. Its r3.
uint32_t CallListener(uint8_t* base, uint32_t driver, uint64_t entry, uint32_t frame,
                      const app_states::AppCalls& calls);

// sub_82091E58 FrameDriver::DrawFrame (the render thread's frame): the device
// locked (VideoOutput::LockDevice on +64) and kept at +384,
// HandlerMap::ForEach over the listener map with FrameDriver::CallListener
// (the iterators and the functor stored in `frame`, the original's 144-byte
// frame, as it stores them; r29 to r31 as it sets them), then the device
// unlocked with the word at +384.
void DrawFrame(uint8_t* base, uint32_t driver, uint32_t frame,
               const app_states::AppCalls& calls);

// The app's scene (80 bytes, vtable 0x82000F3C).
constexpr uint32_t kSceneCommandList = 8;
constexpr uint32_t kSceneRenderQueue = 64;
// The word sub_82591838 flips once a tick.
constexpr uint32_t kPrimitiveFlip = 0x82970BC4;
// The vblank count VideoOutput::BeginFrame clears.
constexpr uint32_t kVblankCount = 0x8296BD30;
constexpr uint32_t kGetVideoOutput = 0x824960E0;

// Device::SetShaderGprAllocation on a device (its plain C++ or original, in
// the frames the original call makes), for VideoOutput::BeginFrame.
using SetGpr = std::function<void(uint32_t device, uint32_t vertex, uint32_t pixel)>;

// sub_82591838: [0x82970BC4] = 0 when it was 1, else 1.
void FlipPrimitiveWord(uint8_t* base);

// sub_820988C8 Scene::Update (tick handler 18): the command list (+8)
// flipped and its tick begun (CommandList::Flip and BeginTick's plain C++),
// sub_82591838, then the render queue's (+64) slots 6 (RenderQueue::Flip,
// returning to 0x82098910) and 2 (ClearRecording, 0x82098924) as guest
// calls, r30 and r31 as the original sets them.
void SceneUpdate(uint8_t* base, uint32_t scene, const app_states::AppCalls& calls);

// sub_8258D958 VideoOutput::BeginFrame (probably): `set_gpr` on the device
// (+140) with 32 vertex and 96 pixel registers, then the vblank count
// [0x8296BD30] = 0.
void VideoOutputBeginFrame(uint8_t* base, uint32_t video_output, const SetGpr& set_gpr);

// sub_82098940 Scene::Draw (the frame driver's listener 1): the video output
// from `interface`'s slot 3 (FrameDriver::GetVideoOutput's plain C++ where the
// slot is it, else a guest call returning to 0x82098968),
// VideoOutput::BeginFrame on it (from a 96-byte frame, through `set_gpr`),
// the empty sub_82106540 left out, then the render queue's slot 5
// (RenderQueue::Replay with the device and the video output, a guest call
// returning to 0x82098994: its per-frame replay wrap kept). r29 to r31 as
// the original sets them. Returns the video output (the original's r3, as
// the empty call leaves it).
uint32_t SceneDraw(uint8_t* base, uint32_t scene, uint32_t interface, uint32_t device,
                   const app_states::AppCalls& calls, const SetGpr& set_gpr);

// VideoOutput::UnbindAll's guest calls: the device's setters through their
// adapters (their plain C++ and wraps), returning where the original's
// calls return.
struct UnbindCalls {
  // Device::SetTexture (sub_822A6008): r4 the stage, r5 0, r6 the mask.
  std::function<void(uint32_t device, uint32_t stage, uint64_t mask)> set_texture;
  // Device::SetStreamSource (sub_822A3758): r4 the stream, r5 to r7 0, r8
  // the mask.
  std::function<void(uint32_t device, uint32_t stream, uint64_t mask)> set_stream_source;
  // Device::SetIndices (sub_822A3908), SetPixelShader (sub_822A8618) and
  // D3DDevice_SetVertexShader (sub_822A8958) with r4 = 0.
  std::function<void(uint32_t device)> set_indices;
  std::function<void(uint32_t device)> set_pixel_shader;
  std::function<void(uint32_t device)> set_vertex_shader;
  // A nonvolatile register set as the original sets it.
  std::function<void(int reg, uint64_t value)> set_register;
};

// sub_8258D9F8 VideoOutput::UnbindAll: on the device (+140, read again for
// each call), every texture stage 0-15 unset (the mask bit 63 >> (stage +
// 20)), every stream 0-3 (bit 63 >> 51, 51, 51, 50: the original's
// (95 - stream) x 21846 >> 16, plus 20), the index buffer and both shaders;
// r29 to r31 as the original keeps them.
void VideoOutputUnbindAll(uint8_t* base, uint32_t video_output, const UnbindCalls& calls);

}  // namespace frame_driver
