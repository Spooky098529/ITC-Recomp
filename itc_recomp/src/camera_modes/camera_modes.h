// The camera modes as plain C++ (docs/functions/camera-modes.md): the camera
// director (the race mode's member at +10512), its views' cameras, the TV
// camera's paths, the cutscene and the camera animations. They're guest
// objects shared with the recompiled code (the race mode, the viewports,
// frame interpolation's blend), so these work on guest memory, with the
// originals' loads and stores in their order, and their float arithmetic
// step for step as the translated code's doubles (fmuls and the rest
// rounded to single, the fused steps as std::fma), flush-to-zero off (the
// adapters' job) except around the vector unit's work. Built with
// -ffp-model=strict and -ffp-contract=off (CMakeLists.txt). Adapters:
// src/hooks/camera_modes.cpp (behind --native_camera_modes).

#pragma once

#include <cstdint>
#include <optional>

namespace camera_modes {

// The globals read here.
constexpr uint32_t kViewports = 0x826C2CE8;  // the two viewports (a pointer to two)
constexpr uint32_t kRaceData = 0x826C5B2C;   // the race data (a pointer)
// The camera animations' table: 264-byte entries (+0 the asset id, +4 up
// to 64 object pointers, +260 their count) and its count.
constexpr uint32_t kAnims = 0x826C79B8;
constexpr uint32_t kAnimCount = 0x826D81B8;
constexpr uint32_t kAnimBytes = 264;

// The director's fields.
constexpr uint32_t kView = 9628;      // the view shown (0-5)
constexpr uint32_t kViewKept = 9632;  // the view kept for after views 3-5
constexpr uint32_t kFollowed = 9636;  // the player the views follow
constexpr uint32_t kFollowedInput = 0x12E60;  // +77408: the player whose input is read

// --- The director's handlers (no direct callers: its state machine's functors) ---

// sub_82152400 CameraDirector::ToView4: viewport 0's view mode (+1860) = 4;
// in split screen (the viewports' +8 set) the split ended (+8 = 0) and the
// player followed (+9636) = 1 when the race data's last racer
// record (RacerRecords::At(race data + 0x59AE8, -1)) has +3924 = 2, else 0
// (+77408 = 0 either way); +9632 = +9628. Returns the record when it was
// read (the original's r3), else the director.
uint32_t ToView4(uint8_t* base, uint32_t director);
// sub_821524B8 CameraDirector::ToView5: viewport 0's view mode = 5, +9632 =
// +9628.
void ToView5(uint8_t* base, uint32_t director);
// sub_821524D8 CameraDirector::ToView3: viewport 0's view mode = 3.
void ToView3(uint8_t* base);
// sub_82153160 CameraDirector::ToView3IfCourse: with the race's kind
// (race data +0xA3AF0) 1 and the course (+4) not 8, 9 or 10, viewport 0's
// view mode = 3 and, unless +9628 is 3, +9632 = it.
void ToView3IfCourse(uint8_t* base, uint32_t director);

// --- Small helpers ---

// sub_821524F0 Viewports::ResetRoll: Viewport::SetRoll(0.0) on both.
void ResetRoll(uint8_t* base);
// sub_82152548 Camera::ZoomFromFov: 0.5 x `width` x tan(half the angle in
// degrees, in radians, wrapped into [-pi, pi]), each step rounded to
// single. nullopt where tan's native form can't stand in (the rounding mode
// not nearest, or tan's _except1 paths): the adapter runs the original.
std::optional<double> ZoomFromFov(uint8_t* base, double degrees, double width);
// sub_821525F8 Quat::Rotate: out = v turned by the quaternion q (x, y, z,
// w): v (w² − x² − y² − z²) + 2 (q·v) q + 2w (q × v), w of out = 1.0.
void QuatRotate(uint8_t* base, uint32_t out, uint32_t q, uint32_t v);
// sub_82152698 Matrix::FromQuat: `out` = the turn of the quaternion `q`
// (its four words, as the original's r4 and r5 hold them) scaled by 2 /
// |q|² (the vector unit's dot product, flush-to-zero on), row 3 (0, 0, 0,
// 1) and the w words 0. `scratch` is where the original keeps the dot
// product (its r1 - 16, 16-byte aligned down): written as it does.
void MatrixFromQuat(uint8_t* base, uint32_t out, const uint32_t q[4], uint32_t scratch);
// sub_82090000 Matrix::FromFloats: the four w words (+12, +28, +44, +60) =
// 1.0, then the 16 words from the 16 pointers, in order.
void MatrixFromFloats(uint8_t* base, uint32_t out, const uint32_t from[16]);

// --- The fixed-size lists (List16::, List40::, track_zones.h's layout) ---

// sub_82153228 List40::Link: List16::Link for 40 slots (free head +160,
// head +164, count +168).
void List40Link(uint8_t* base, uint32_t list, int16_t position);
// sub_821544E0 List40::Insert: List40::Link at `position`'s low halfword,
// then the node `position` steps from the head has its data word (+172 + 4
// x its slot) = `word`.
void List40Insert(uint8_t* base, uint32_t list, uint32_t word, uint32_t position);
// sub_82154458 List16::Insert20: List16::Link, then the 20 bytes at
// `record` copied word by word to the node's slot (+76 + 20 x its slot).
void List16Insert20(uint8_t* base, uint32_t list, uint32_t record, uint32_t position);
// sub_82154378 List16::Insert32 and sub_821543E8 List16::Insert608:
// List16::Link, then the node's slot (+76 + size x its slot) is where the
// record goes (memcpy). `*copy_to` = it; true when the copy was made here,
// false when c_runtime::Memcpy refused it (the caller copies through the
// guest's memcpy).
bool List16Insert(uint8_t* base, uint32_t list, uint32_t record, uint32_t position,
                  uint32_t size, uint32_t* copy_to);
// sub_82154300 List16::Copy32: the node `position` steps from the head of
// the 16-slot list's 32-byte slot (+76 + 32 x its slot) is copied to `out`
// (memcpy). `*copy_from` = it; false as List16Insert.
bool List16Copy32(uint8_t* base, uint32_t out, uint32_t list, uint32_t position,
                  uint32_t* copy_from);

// --- The camera animations' table ---

// sub_82150078 CameraAnims::Clear: the table's count = 0.
void AnimsClear(uint8_t* base);
// sub_82150300 CameraAnims::Find: the entry with the asset id: its +4 (the
// objects), or 0.
uint32_t AnimsFind(uint8_t* base, uint32_t id);

// --- The color filter and the fade cues ---

// sub_82152108 ColorFilter::SetStyle: on the color filter
// (PostEffects::Get(2), nothing when 0), the style at `style` (0-3; others
// nothing) through its vtable's slots 6 (+8112), 4 (the color) and 7
// (+8116). False, with nothing done, when a slot isn't the color filter's
// own function (the adapter runs the original).
bool ColorFilterSetStyle(uint8_t* base, uint32_t style);
// sub_82152290 FadeCue::Step: a 32-byte cue (+0 the start frame, +4 the
// end, +8 the kind, +12 to +20 the color, +24 the frame, +28 hold): at the
// start frame, kind 0 ScreenFades::FadeIn, 1 FadeOut (the length end -
// start, layer 0); the frame counted up to the end unless held.
void FadeCueStep(uint8_t* base, uint32_t cue);

}  // namespace camera_modes
