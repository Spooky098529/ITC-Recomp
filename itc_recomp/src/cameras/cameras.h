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

}  // namespace cameras
