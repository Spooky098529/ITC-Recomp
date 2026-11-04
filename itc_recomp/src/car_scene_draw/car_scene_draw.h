// The car scene's draw as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/car-scene-draw.md, the fields in
// docs/known-data.md). The data is guest data shared with the recompiled
// code, so these work on guest memory with the originals' loads and stores
// in their order: the scalar float work as the translated code's doubles
// rounded to single (fused where the original fuses), the vector work on SSE
// vectors with flush-to-zero on as the translation turns it on (built with
// -ffp-model=strict, CMakeLists.txt). Each takes `stack`, the guest r1 at
// entry, and keeps the original's frame: its back chain written as stwu does
// (not its saved registers), its temporaries at the original's offsets (the
// leaves' below r1, as the original's), its callees run from it. Expect
// flush-to-zero off at entry (the adapters turn it off) and leave it off.
// Adapters: src/hooks/car_scene_draw.cpp; a function that declines (nullopt,
// with nothing done that the original wouldn't redo the same) falls back to
// its original there.
//
// Part 1: the geometry library under TextureProjection::Fit (sub_82598768,
// still recompiled): the frustum, the planes' crossings, the box over a
// std::vector of 16-byte points and the vector's growth. Part 2: the
// projectors' matrices (look-at, perspective, Projector::Build and
// BuildOrtho) and the root's small leaves (the pass's light, the scene
// models' beams, the player's model). Part 3: TextureProjection::Fit
// over them. Part 4 (scene_draw.cpp): the root, CarScene::Draw
// (sub_820C5DE8): 4a its set-up and the records of both paths, 4b the pass
// loop, 4c its wrap and harness. The float helpers they share: ops.h.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace car_scene_draw {

// --- The frustum and the planes (geometry.cpp) ---

// sub_825976C8 Plane::Intersect3: out = the point where the planes a, b and
// c (x, y, z, d) meet, w 1.0; 1, or 0 with out unwritten when their triple
// product is 0, denormal, infinite or NaN. Its temporaries below `stack`.
uint32_t PlaneIntersect3(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t c,
                         uint32_t stack);
// sub_82597878 Frustum::FromMatrix: the 256-byte frustum at `out` from the
// matrix (its six planes, their sign codes and eight corners). Returns out.
uint32_t FrustumFromMatrix(uint8_t* base, uint32_t out, uint32_t matrix, uint32_t stack);
// sub_82597E58 Plane::SegmentCrossing: out = where the line from a through b
// crosses the plane, w 1.0; 1, or 0 with out unwritten when it runs along
// the plane. Its temporaries below `stack`.
uint32_t PlaneSegmentCrossing(uint8_t* base, uint32_t out, uint32_t plane, uint32_t b,
                              uint32_t a, uint32_t stack);

// --- The box (geometry.cpp) ---

// sub_825971E8 Vector16::Size: (last - first) / 16 (srawi), 0 with no storage.
uint32_t Vector16Size(uint8_t* base, uint32_t vector);
// sub_82597150 Box::Extend: the 32-byte box (min, max) grown to the point.
void BoxExtend(uint8_t* base, uint32_t box, uint32_t point);
// sub_82597210 Box::FromPoints: the box over the vector's points. Returns box.
uint32_t BoxFromPoints(uint8_t* base, uint32_t box, uint32_t vector, uint32_t stack);

// --- std::vector of 16-byte elements (vector16.cpp) ---
// The vector: +4 first, +8 last, +12 end of storage. Each returns what the
// original leaves in r3, or nullopt (nothing done) where the original takes
// a path the plain C++ leaves to it: _Xlen (sub_825981C8) and bad_alloc's
// copy (sub_82091200), never run in play, and operator new's and delete's
// error paths (or the native heap off).

// sub_82598158 Vector16::Allocate: operator new(16 x count).
std::optional<uint32_t> Vector16Allocate(uint8_t* base, uint32_t count, uint32_t stack);
// sub_82598288 Vector16::InsertN: n copies of the 16 bytes at `value` at
// `where`.
std::optional<uint32_t> Vector16InsertN(uint8_t* base, uint32_t vector, uint32_t where,
                                        uint32_t n, uint32_t value, uint32_t stack);
// sub_82598648 Vector16::Insert: one copy at `where`; the iterator to it at
// `out`. Returns out.
std::optional<uint32_t> Vector16Insert(uint8_t* base, uint32_t out, uint32_t vector,
                                       uint32_t where, uint32_t value, uint32_t stack);
// sub_825986B8 Vector16::PushBack.
std::optional<uint32_t> Vector16PushBack(uint8_t* base, uint32_t vector, uint32_t value,
                                         uint32_t stack);

// --- The projectors (projectors.cpp) ---
// f-register arguments are the doubles the registers hold.

// sub_82597378 Matrix::LookAt: out = the row-vector view matrix from eye
// to target with `up`, eye and up read whole (lvx: their aligned 16 bytes)
// as well as eye's x, y, z. Its temporaries below `stack` (- 32 to - 4).
void LookAt(uint8_t* base, uint32_t out, uint32_t eye, uint32_t target, uint32_t up,
            uint32_t stack);
// sub_825972A8 Matrix::Perspective: out = the perspective for the field of
// view, aspect, near and far, its back chain at stack - 128. False, with
// nothing written, where tan (sub_825A8A50) would go to its error paths or
// the rounding mode isn't nearest (c_runtime::Tan can't stand in): the
// original then runs. r3 isn't given (the original leaves tan's _ctrlfp
// word, which no caller reads).
bool Perspective(uint8_t* base, uint32_t out, double fov, double aspect, double near_plane,
                 double far_plane, uint32_t stack);
// Whether Perspective would decline for this field of view (checked by
// callers before they write anything).
bool PerspectiveDeclines(uint8_t* base, double fov);
// sub_82597610 Projector::Build: out = LookAt (its frame's +144) x
// Perspective (+80) x the bias matrix at 0x8265EED0, in its 256-byte frame
// below `stack`, both multiplies noted for frame interpolation by their
// return addresses. False, with nothing written, where Perspective would
// decline.
bool ProjectorBuild(uint8_t* base, uint32_t out, uint32_t eye, uint32_t target, uint32_t up,
                    double fov, double aspect, double near_plane, double far_plane,
                    uint32_t stack);
// sub_82597518 Projector::BuildOrtho: out = LookAt (its frame's +144) x an
// orthographic matrix (+80) for the width, height, near and far x the bias
// matrix, in its 272-byte frame below `stack`, noted as Build's.
void ProjectorBuildOrtho(uint8_t* base, uint32_t out, uint32_t eye, uint32_t target,
                         uint32_t up, double width, double height, double near_plane,
                         double far_plane, uint32_t stack);

// --- The fit (texture_projection.cpp) ---

// Vector16::PushBack's original (sub_825986B8) from the frame at `sp` with
// `lr`, where its plain C++ declines; leaves flush-to-zero off.
using PushBackCall =
    std::function<void(uint32_t vector, uint32_t value, uint32_t sp, uint32_t lr)>;

// Whether TextureProjectionFit would decline for this camera pass (its
// Matrix::Perspective would: PerspectiveDeclines on the pass's +368).
bool TextureProjectionFitDeclines(uint8_t* base, uint32_t pass);
// sub_82598768 TextureProjection::Fit: out = the texture projection fitted
// to the camera pass's view between the heights `low` and `high` along the
// direction, its far plane cut to far_cut, the eye's distance far x
// distance_scale or at least min_distance (f1 to f5), in its 1,248-byte
// frame below `stack` (the back chain, the 256-byte frustum and the
// std::vector of the frustum's corners on it, its matrices at the
// original's offsets, every Matrix::Multiply noted for frame interpolation
// by its return address). r3 = what the original leaves (the vector's
// storage, freed, or out). False, with nothing written, where
// Matrix::Perspective would decline: the original then runs. Leaves
// flush-to-zero off (the original leaves it on, Matrix::Multiply's last
// step: the adapter turns it on).
bool TextureProjectionFit(uint8_t* base, uint32_t out, uint32_t pass, uint32_t direction,
                          double high, double low, double far_cut, double min_distance,
                          double distance_scale, uint32_t stack, const PushBackCall& push_back,
                          uint32_t* r3);

// --- The root's leaves (lights.cpp) ---

// memcpy's original (sub_825A7410) from the frame at `sp` with `lr`, where
// native::Buffers_Memcpy declines (--native_memcpy off).
using MemcpyCall =
    std::function<void(uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp, uint32_t lr)>;

// sub_820C4EB8 ScenePass::SetLight: the pass description's +96 = the
// direction (its aligned 16 bytes) scaled by 1 / its x, y, z length (0 for
// a zero length), +112 = the identity (built in its 192-byte frame below
// `stack`, copied with memcpy), its last row +160 = (-distance x the
// scaled x, y, z, 1.0). Returns what memcpy leaves in r3 (pass + 112).
uint32_t ScenePassSetLight(uint8_t* base, uint32_t pass, uint32_t direction, double distance,
                           uint32_t stack, const MemcpyCall& memcpy);
// sub_820DAA38 SceneModel::BeamColor: out = the headlights' color from the
// table (sub_820D35D0's plain C++) by the model's +48 and +3776, with the
// high beams (+4292 & 0x40) scaled by [0x820029D4] (w 1.0) through its
// 112-byte frame below `stack`. Returns out.
uint32_t SceneModelBeamColor(uint8_t* base, uint32_t out, uint32_t model, uint32_t stack);
// sub_820DAAC8 SceneModel::Beam and sub_820DAB10 SceneModel::BeamShape:
// out = the scene's (`[0x826C2E44]`) +5408 or +5392 (Beam) and +5472 or
// +5456 (BeamShape), the first with the high beams.
void SceneModelBeam(uint8_t* base, uint32_t out, uint32_t model);
void SceneModelBeamShape(uint8_t* base, uint32_t out, uint32_t model);
// sub_820C4FB8 CarScene::PlayerModel: the index, or for 0, 1 when the race
// kind is 6 with its +0x59614 2 and [0x826C7824] 0, else 0.
uint32_t CarScenePlayerModel(uint8_t* base, uint32_t index);

// --- The root (scene_draw.cpp) ---

// CarScene::Draw's frame (its caller's r1 less this).
constexpr uint32_t kSceneDrawFrame = 1696;

// What the root's plain C++ can't do itself: its guest calls, made from the
// root's frame with the original's return addresses.
struct SceneDrawCalls {
  // Any guest function `function` from the frame at `sp` with `lr`: r3 on
  // from `regs` (`count`, all 64 bits), f1 on from `fregs` (`fcount`); its
  // r3 (64 bits). Leaves flush-to-zero off. The render manager's, the
  // command list's, the render queue's and the texture provider's vtable
  // calls, CameraPasses::RecordBegins and RecordEnds (their wraps), the
  // command list's originals where the plain C++ declines, and the wraps of
  // Projector::Build and TextureProjection::Fit where theirs would.
  std::function<uint64_t(uint32_t function, const uint64_t* regs, int count, const double* fregs,
                         int fcount, uint32_t sp, uint32_t lr)>
      call;
  // memcpy's original where native::Buffers_Memcpy declines.
  MemcpyCall memcpy;
  // Vector16::PushBack's original, for TextureProjection::Fit.
  PushBackCall push_back;
  // Car::DrawUpdate (sub_820DAC68) from the frame at `sp` with `lr`: r3 =
  // the scene model, r24 = the root's r24 (its callees get it): its plain
  // C++ under --native_car_draw (hooks::CarDrawUpdate), else the original;
  // the caller makes the wrap's checks (--debug_hide,
  // --find_per_frame_state) around it. Leaves flush-to-zero off.
  std::function<void(uint64_t model, uint64_t r24, uint32_t sp, uint32_t lr)> car_draw_update;
};

// The root's state shared between its halves (4a's set-up and records, 4b's
// pass loop and end). The rest is on the frame, at the original's offsets:
//   f + 80   the records' counter (path +4 = 0; 4b reuses it)
//   f + 88   0x826BAC00 (the camera passes)
//   f + 160  the records' counter (path +4 != 0)
//   f + 192  0 for path +4 = 0, 1 for the other
//   f + 224, f + 228  0x8296BAE0, 0x8296BB60 (the headlight slots)
//   f + 1328 pass 0's view-projection as it was (put back at the end)
//   f + 1716 the scene (the caller's frame's +20)
// and the original's registers at loc_820C7C50, where 4b goes on: r21 = the
// scene, r27 = 1, r30 = 0, r26 = 4 (path +4 = 0: 4b sets 1), f30 = 1.0
// ([0x82000A30]), f31 = 0.0 ([0x82000A34]).
struct SceneDrawFrame {
  uint32_t f = 0;      // the root's r1
  uint32_t scene = 0;  // r3 at entry
  bool second_path = false;  // the scene's +4 not 0
};

// sub_820C5DE8 CarScene::Draw: r3 = the scene ([0x826C2E44]), r4 = a matrix
// or 0; `stack` the caller's r1. Its frame (back chain as stwu writes it),
// nothing more when the scene's +0 is 4; pass 0's view-projection saved (and
// times the matrix), then the path's set-up and its three records' chunks
// (docs/functions/car-scene-draw.md), its callees' plain C++ inside their
// diagnostics' wrap checks (--debug_hide, --find_per_frame_state), each
// Matrix::Multiply and Copy noted for frame interpolation by the original's
// return address. Then both paths' pass descriptions (each copied to the
// scene's +5072; the player's and model 1's +4296 by its type in the second
// path; for each scene model in use with a car model, the other cars'
// headlight slots in the second path with the scene's +5052 0, the render
// queue's keys and the mask set around Car::DrawUpdate), and pass 0's
// view-projection put back. Leaves flush-to-zero off.
void CarSceneDraw(uint8_t* base, const SceneDrawCalls& calls, uint32_t stack, uint32_t scene,
                  uint32_t matrix);

}  // namespace car_scene_draw
