// The camera passes and viewports as plain C++ (docs/functions/cameras.md):
// the graphics library's camera class (`Camera`, vtable 0x82001EC0), the
// game's 21 camera passes built on it (592 bytes each from 0x826BAC00) and
// the two viewports at [0x826C2CE8]. They're guest objects shared with the
// recompiled code (the camera modes, the recorders, frame interpolation's
// blend), so these work on guest memory, with the originals' loads and
// stores in their order where an output may be one of the inputs, and their
// arithmetic step for step so the results are the translated code's bit for
// bit (as src/matrix_math/matrix_math.h describes: the vector unit's work as
// the translation's SSE steps, word 0 in the top lane and flush-to-zero on;
// the scalar work as its doubles, flush-to-zero off: the adapters' job).
// Built with -ffp-model=strict and -ffp-contract=off (CMakeLists.txt).
// Adapters: src/hooks/cameras.cpp.
//
// A camera: +0 the vtable, +16 the view, +80 the projection, +144 the
// view-projection (4x4 floats, row-major, row vectors), +368 the field of
// view (radians), +376 and +380 the viewport's width and height (floats),
// +384 the near plane, +388 the far plane, +392 the aspect.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace cameras {

// Camera::SetFieldOfView (sub_82593278, vtable slot 1): +368 = `degrees`
// (f1, a double) times pi/180 (0x82002948), wrapped into [-pi, pi] as
// Matrix::RotateZ does.
void SetFieldOfView(uint8_t* base, uint32_t camera, double degrees);

// Camera::BuildProjection (sub_825932E8): the projection at camera + 80
// from the floats at `fov`, `aspect`, `far` and `near`: the half angle
// wrapped, its sine and cosine from the game's polynomials (0x820009C0 to
// 0x82000A1F), the cotangent as y's scale and that over the aspect as x's,
// near / (near - far) as z's and -far times that as w's. The rows stored
// aligned (stvx), the first, second and fourth zeroed and the third set to
// (0, 0, 0, 1) before any input is read, as the original does. Its stack
// words (r1 - 112 to r1 - 16) are held here instead: the same values when
// the stack pointer is 16-byte aligned (the adapter checks).
void BuildProjection(uint8_t* base, uint32_t camera, uint32_t fov, uint32_t aspect, uint32_t far,
                     uint32_t near);

// Camera::SetViewportSize (sub_82593500, vtable slot 0): +376 and +380 =
// the width and height (unsigned 32-bit integers) as floats, +392 = width /
// height, then BuildProjection(camera, +368, +392, +388, +384).
void SetViewportSize(uint8_t* base, uint32_t camera, uint32_t width, uint32_t height);

// --- The camera passes (592 bytes each from kPasses) ---
//
// A pass is a camera (above) and: +400, +416, +432 the eye, target and up,
// +448 the roll, +496 to +508 the viewport (x, y, width, height), +516 the
// zoom, +564 the clear color, +572 the clear depth, +576 and +580 its
// surfaces, +584 its type (0-20), +588 whether it's enabled.

constexpr uint32_t kPasses = 0x826BAC00;
constexpr uint32_t kPassBytes = 592;
constexpr uint32_t kPassCount = 21;
// The mode (CameraPasses::SetMode), the render targets' textures, the
// viewports' block.
constexpr uint32_t kMode = 0x826BABC0;
constexpr uint32_t kViewports = 0x826C2CE8;

constexpr uint32_t Pass(uint32_t i) {
  return kPasses + kPassBytes * i;
}

// CameraPasses::Mode (sub_8209F718): the mode when `which` is 1, else -1
// (as a word).
uint32_t Mode(uint8_t* base, int32_t which);

// CameraPasses::SetMode (sub_8209F738): nothing unless `which` is 1; then
// the mode = `mode`, and mode 0 sets pass 4's field of view to the float at
// 0x82001E8C (80 degrees), mode 1 to 0x82001E88's (90), through the camera's
// vtable slot 1 (a tail call): Camera::SetFieldOfView's plain C++ here. False,
// with nothing done, when that slot isn't Camera::SetFieldOfView (the
// original then makes the call; never so in play).
bool SetMode(uint8_t* base, int32_t which, uint32_t mode);

// CameraPasses::RenderTarget (sub_8209F7A8, with its case 0, sub_8209F7DC,
// merged): render target `id`'s texture (0 above 12, unsigned), id 1's by
// the mode (0 for modes other than 0 and 1).
uint32_t RenderTarget(uint8_t* base, uint32_t id);

// CameraPass::Enable (sub_820A0968) and CameraPass::Disable (sub_820A0958):
// +588 = 1 or 0.
void Enable(uint8_t* base, uint32_t pass);
void Disable(uint8_t* base, uint32_t pass);

// CameraPasses::EnableForView (sub_820AA1A8): the passes the view needs
// enabled (Enable): with one viewport ([kViewports] +8 = 0) passes 0, 10,
// 11, 12, 19 and 20, and 3 when viewport 0's view mode (+1860) is 1 or 2;
// in split screen 1, 2, 13 to 18 and 20; then pass 4, and 5 to 9 too when
// the mode (Mode(1)) is 1. Returns the last pass enabled (4 or 9; the
// original leaves it in r3).
uint32_t EnableForView(uint8_t* base);

// CameraPasses::SetNear (sub_820AA368): pass 0's near plane (+384) = `near`
// (f1) as a float.
void SetNear(uint8_t* base, double near);

// CameraPass::ReleaseSurfaces (sub_820A0C40): for types 3, 4 and 10 to 20
// (+584), each surface (+576, then +580) that's set is handed to `release`
// with its field's offset (Resource::Release, sub_822A67B0: a guest call,
// made by the adapter) and cleared. Returns whether the type has surfaces.
bool ReleaseSurfaces(uint8_t* base, uint32_t pass,
                     const std::function<void(uint32_t surface, uint32_t field)>& release);

// CameraPass::RecordViewport (sub_820A0978)'s chunk on the command list
// `list` (the scene's, [0x826BABA0]'s vtable slot 8 then slot 1: the
// adapter's guest calls): a header (+2 = 5), command 30 (1), the viewport
// (+496, +500, +504, +508 as halfwords, depths 0.0 and 1.0) and command 36
// in one chunk. Returns its first slot, which the adapter hands to the
// render queue (slot 8 then 2; the queue's slot 4 with the sort key); nothing
// done (nullopt) where the original's command list calls would print a
// message (another chunk open, the list full).
std::optional<uint32_t> RecordViewport(uint8_t* base, uint32_t list, uint32_t pass);

// CameraPass::Update (sub_820A3978): types 0 to 2 (+584, unsigned) first
// have their viewport recorded (`record_viewport(key)`: RecordViewport with
// the adapter's guest calls; key 150 for types 0 and 1, 371 for 2); then the
// camera's vtable slot 0 (Camera::SetViewportSize with +504, +508: its plain
// C++ here). Types 10 to 18 and 20 stop there. The others: the view from the
// eye, target and up (+400, +416, +432): the forward (target - eye, w 1.0)
// kept normalized at +464 (w then 1.0), the right (up x forward) and the up
// again (forward x right), all normalized as the vector unit does it
// (vrsqrtefp and a Newton step); the view (their columns and -eye's dots)
// built at `frame` + 128 (the original's stack: `frame` is its 272-byte
// frame, 16-byte aligned) and copied to +16 with Matrix::Copy, then turned
// by the roll (+448, Matrix::RotateZ, its turn at `frame` - 80); +480 the
// up recomputed (the forward at +464 crossed with +432, normalized); the
// view-projection +144 = view (+16) x projection (+80) (Matrix::Multiply).
// The copy and the multiply are noted for frame interpolation as the
// original's calls (interpolation::BlendState_OnCopy and OnMultiply, with
// their return addresses 0x820A3D18 and 0x820A3D90; the turn notes its own).
// False, with nothing done, where slot 0 isn't Camera::SetViewportSize
// (never so in play).
bool PassUpdate(uint8_t* base, uint32_t pass, uint32_t frame,
                const std::function<void(uint32_t key)>& record_viewport);

// CameraPass::FromViewport (sub_820AA4A8)'s work before its tail:
// viewport `player`'s ([[kViewports] + 4 x player]) entry `entry` (80
// bytes each): its eye, target and up (+16 to +63, 8 bytes at a time) into
// the pass's +400 to +447, PassSetZoom(pass, entry +68), the far plane +388
// = entry +72, the roll +448 = entry +64. The adapter then calls
// CameraPass::Update (through its wrapper, which keeps its diagnostics).
void FromViewport(uint8_t* base, uint32_t pass, uint32_t player, uint32_t entry);

// CameraPass::SetZoom (sub_820AA378): +516 = `zoom` (f1) as a float; the
// field of view +368 = atan(height (+508, a signed integer) x 0.5 / zoom),
// wrapped into [-pi, pi]; atan the C runtime's (src/c_runtime/math.h).
void PassSetZoom(uint8_t* base, uint32_t pass, double zoom);

// --- The viewports ([kViewports]: a 12-byte block, +0 and +4 two 1,872-byte
// viewports, +8 whether both are in use) ---
//
// A viewport has twenty 80-byte entries from +0 (+16, +32, +48 the eye,
// target and up, +64 the roll, +68 the zoom, +72 the far plane), +1856 a
// distance, +1860 its view mode.

// Viewports::Release (sub_820AA430): the two viewports and the block freed
// (VirtualFree MEM_RELEASE: memory::FreePages), [kViewports] = 0. False,
// with nothing done, where one isn't the allocation layer's pages (the
// original frees it).
bool ReleaseViewports(uint8_t* base);

// Viewport::SetZoom (sub_820AAE90): the entries' zoom (+68): entries 0, 1,
// 8 to 17 and 19 = `zoom` (f1) as a float; 2 to 7 = atan(the integer at
// 0x82001EF0 (256) x 0.5) times the float at 0x82002808; 18 = 1 / +1856
// times the float at 0x82002800.
void ViewportSetZoom(uint8_t* base, uint32_t viewport, double zoom);

// Viewport::SetRoll (sub_820AA2D0): `angle` (f1) wrapped into [-pi, pi] by
// adding pi, taking off 2 pi while above 2 pi, adding it while below 0 and
// taking off pi; stored as the roll (+64) of entries 0, 2 to 17 and 19,
// negated for entry 1. An infinite angle never leaves the first loop, as
// in the original (the game's are numbers).
void ViewportSetRoll(uint8_t* base, uint32_t viewport, double angle);

}  // namespace cameras
