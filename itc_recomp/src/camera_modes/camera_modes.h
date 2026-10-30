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
#include <functional>
#include <optional>
#include <utility>

#include "archive/archive.h"
#include "c_runtime/exit.h"

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


// --- The camera animations' files (src/camera_modes/anims.cpp) ---

// A copy the native layer refuses (c_runtime::Memcpy's or XMemCpy's false),
// made through the guest's memcpy (sub_825A7410) or XMemCpy (sub_82295890)
// by the adapter: destination, source, bytes.
using GuestCopy = std::function<void(uint32_t, uint32_t, uint32_t)>;

// The load callbacks Archive::Load is handed (no direct callers).
constexpr uint32_t kAnimsOnLoaded = 0x82150878;
constexpr uint32_t kPathOnLoaded = 0x82152078;

// sub_82150B10 CameraAnims::Load: a 4-byte record (VirtualAlloc: the
// layer's AllocPages) = `id`, a buffer of Archive::AssetSize(id) bytes, then
// Archive::Load(id, the buffer, CameraAnims::OnLoaded, the record). Returns
// the load's serial (-1 when none was queued).
int64_t AnimsLoad(uint8_t* base, uint32_t id, const archive::LoadCall& load);

// sub_82150100 CameraAnim::ParseObject: a 340-byte copy (AllocPages) of the
// file's object at `object`: +0 to +36 its words, +40 and +44 its floats,
// +48 to +79 0, then each of its +12 blocks (from object + [object + 4],
// each + its +4 + its +8 to the next) as a 64-byte copy (+0 to +8 its words,
// +12 and +16 its floats, +20 to +59 0, +60 its data: AllocPages(+8),
// XMemSet to 0, XMemCpy'd from the block + its +4), appended at +80 (count
// +336). Returns the copy. `xmemcpy` makes the copies XMemCpy refuses.
uint32_t AnimParseObject(uint8_t* base, uint32_t object, const GuestCopy& xmemcpy);

// sub_82150878 CameraAnims::OnLoaded: the file's objects at `data` (each +
// its +4 + its +8 to the next) parsed (AnimParseObject), to the end object
// (kind 14, parsed too), into the table's next entry (+0 the record's id,
// +4 the objects, +260 their count) through the original's stack frame at
// `frame` (its r1 - 672: the pointers at +352, the count at +608, the id at
// +80, copied to +84 and then to the entry with memcpy, so the unused slots
// hold what the stack held, as the original leaves them); the table's
// count up by one; the record and the data freed (VirtualFree MEM_RELEASE:
// FreePages). False, with nothing done, when either isn't the layer's
// pages (the adapter runs the original). `memcpy` and `xmemcpy` make the
// copies the native layer refuses.
bool AnimsOnLoaded(uint8_t* base, uint32_t data, uint32_t record, uint32_t frame,
                   const GuestCopy& memcpy, const GuestCopy& xmemcpy);

// sub_8214FFF0 CameraAnimBlock::Free: the block's data (+60) freed when set
// (FreePages), +60 = 0. sub_82150098 CameraAnim::Free: its blocks (+80,
// count +336 counted down) each freed so and freed. sub_821507D8
// CameraAnims::FreeAll: the table's entries from the last (its count
// counted down and stored before each), each one's objects (+4, count +260)
// from the last freed so and freed. AnimsFreeable says whether every
// address FreeAll frees is the layer's pages (else the adapter runs the
// original). FreeAll returns the original's r3 when it set one: 1 after a
// last entry with objects (VirtualFree's), `memcpy_r3` after one without
// (its memcpy's: the original's frame + 80); nullopt with no entries.
bool AnimsFreeable(uint8_t* base);
void BlockFree(uint8_t* base, uint32_t block);
void AnimFree(uint8_t* base, uint32_t object);
std::optional<uint32_t> AnimsFreeAll(uint8_t* base, uint32_t memcpy_r3);

// sub_82150350 CameraAnim::EvalCamera: a kind 6 object's blocks at frame
// `time` (each its data's float [+60][time]) by their +0: 0 x, 1 y, 2 -z,
// 101 the film's height, 102 the focal length, 109 -roll; into `out`: +16
// x, +20 y, +24 -z, +28 1.0 (through the original's stack), +32 the roll,
// +36 the zoom 0x82002C30's float / tan(atan(length x 0x82002D0C's float /
// (height x 2)) x 2 x 0.5). Returns tan's f1, or nullopt with nothing
// written where tan's native form can't stand in (the rounding mode not
// nearest, or tan's _except1 paths): the original runs.
std::optional<double> AnimEvalCamera(uint8_t* base, uint32_t object, uint32_t out,
                                     uint32_t time);
// sub_821504E0 CameraAnim::EvalTarget: a kind 7 object's blocks at frame
// `time` by their +0: 0 x, 1 y, 2 -z; `out` = (x, y, -z, 1.0).
void AnimEvalTarget(uint8_t* base, uint32_t object, uint32_t out, uint32_t time);
// sub_82150590 CameraAnim::Matrix: `out` = the identity (memcpy from the
// original's stack) turned by RotateY(the float at 0x82001FB4), RotateX(-the
// block 3's value in degrees), RotateY(-block 4's), RotateZ(-block 5's)
// (Multiply'd onto the identity, noted for interpolation as the original's
// call at 0x821507B0), its position (+48 to +56) blocks 0, 1 and -2's.
// `frame` is the original's r1 after its stwu (16-byte aligned: the angle
// at +80, the turned matrix at +160, the turns at -80). `memcpy` makes the
// copy the native layer refuses.
void AnimMatrix(uint8_t* base, uint32_t object, uint32_t out, uint32_t time, uint32_t frame,
                const GuestCopy& memcpy);

// sub_82150960 CameraAnims::Evaluate: the objects of the animation `id`
// (CameraAnims::Find; 0 when not loaded) to the end object (kind 14): kind 6
// AnimEvalCamera, kind 7 AnimEvalTarget, into `out` at frame `time`.
// `camera_original` runs the original EvalCamera where AnimEvalCamera
// can't. Returns 1, or 0; `*f1` = the last EvalCamera's f1 when one ran.
uint32_t AnimsEvaluate(uint8_t* base, uint32_t out, uint32_t id, uint32_t time,
                       const std::function<void(uint32_t object)>& camera_original,
                       std::optional<double>* f1);
// sub_82150A98 CameraAnims::Duration: the first object of kind 1 or 6
// (before the end object) of the animation `id`: its first block's float
// +16 ([+80] + 16); else 0.0. No check that the animation is loaded (the
// count read at 0 + 256, as the original does). `*r3` = the original's r3
// (the objects' slot it stopped at).
double AnimsDuration(uint8_t* base, uint32_t id, uint32_t* r3);
// sub_82150A10 CameraAnims::ObjectMatrix: the animation `id`'s first kind
// 1 object (before the end object) at frame `time` into `out`
// (AnimMatrix, its frame `frame`). 1, or 0 when not loaded.
uint32_t AnimsObjectMatrix(uint8_t* base, uint32_t out, uint32_t id, uint32_t time,
                           uint32_t frame, const GuestCopy& memcpy);

// --- The camera paths (src/camera_modes/anims.cpp) ---

// The camera paths' asset ids: [[0x82618D30 + 4 x index]].
constexpr uint32_t kPathAssets = 0x82618D30;

// sub_82152E08 CameraPath::Request: for `index` (not -1), with the path
// buffer (a Buffer28) empty (+24 0) or loaded (3): its block freed and the
// buffer reset (Buffer28::Free, Reset), +12 = index, then sub_82152D80
// CameraPath::Load of the index's asset: +24 = 1, +20 = AllocPages(
// Archive::AssetSize(id)), +16 = Archive::Load(id, +20,
// CameraPath::OnLoaded, the buffer), +24 = 2. The serial, or nullopt when
// nothing was loaded. Needs game_modes::Buffer28Freeable (the adapter
// checks).
std::optional<int64_t> PathRequest(uint8_t* base, uint32_t buffer, uint32_t index,
                                   const archive::LoadCall& load);
// sub_82152078 CameraPath::OnLoaded: the file at `data` (+0 the paths'
// count, +4 the keys', the 20-byte paths at +16, the 216-byte keys after
// them, the 24-byte curves after those): each path's keys fixed up
// (sub_820AC000 CameraPath::FixKeys: +16 made a pointer, key index x 216 +
// the keys; each key's sub_820ABF80 FixKey, sub_820ABED8 FixGroup and
// sub_820ABE68 FixTriple: each (kind, value) pair of kind 2 made kind 1 with
// value x 24 + the curves); the buffer's +4 = the first path, +24 = 3, +8 =
// the last path.
void PathOnLoaded(uint8_t* base, uint32_t data, uint32_t buffer);
// Its pieces (no adapters: only PathOnLoaded and each other call them):
// FixKeys (r3 the path, r4 the keys, r5 the curves), FixKey (r3 a key, r4
// the curves), FixGroup (r3 a group, r4) and FixTriple (r3 three pairs, r4).
void FixKeys(uint8_t* base, uint32_t path, uint32_t keys, uint32_t curves);
void FixKey(uint8_t* base, uint32_t key, uint32_t curves);
void FixGroup(uint8_t* base, uint32_t group, uint32_t curves);
void FixTriple(uint8_t* base, uint32_t triple, uint32_t curves);

// --- The rival logo (src/camera_modes/anims.cpp) ---

// sub_82211848 RivalLogo::RivalLogo: the entry's +8 = 0, +0 = -1, +4 = 2,
// +12, +16 = 0, then sub_82211708 RivalLogo::InitXpr on the holder at +20:
// +0 = -1, XPR::XPR(+4), +28 = 2, +56, +60 = 0, +64 = 3, +68 = 4.
void RivalLogoConstruct(uint8_t* base, uint32_t entry);
// sub_82211AB0 RivalLogo::Play: with +4's bit 0x4, logo 0 (+0) once its XPR
// is loaded (+48's bit 0x10), others once their movie (+8) is loaded
// (UiMovies::IsLoaded): the movie (UiMovies::Find), for logo 0 its
// textures +84 and +88 swapped for +52 and +64 (the old ones into +76 and
// +80; UiMovie::SwapTexture), the named animation +12 played
// (UiMovie::PlayNamed), +4's bit 0x4 off and 0x8 on. Returns the original's
// r3: the entry when the bit is off, IsLoaded's answer when it waits, else
// PlayNamed's.
uint32_t RivalLogoPlay(uint8_t* base, uint32_t entry, uint32_t key_slot);
// sub_822118E0 RivalLogo::Open: logo -1 sets +4 = 1 only; else, with +4's
// bit 0x2, +0 = the logo, +4 |= 0x4, +8, +12, +16 = its 12-byte record
// (0x8200CD70 + 12 x logo: the movie's id, its name, its asset), then
// open(+8, +16) (UiMovies::Open); for logo 0 (+0 read again) load_xpr(the
// holder +20, the word at 0x8200CEA0 + 4 x texture) (RivalLogo::LoadXpr,
// never run in play). Returns r3 as the original leaves it (the entry, or
// the last call's).
uint32_t RivalLogoOpen(uint8_t* base, uint32_t entry, uint32_t logo, uint32_t texture,
                       const std::function<uint32_t(uint32_t id, uint32_t asset)>& open,
                       const std::function<uint32_t(uint32_t holder, uint32_t id)>& load_xpr);
// sub_82211980 RivalLogo::Close: +8 stored at the frame's +80 (`frame`, the
// original's r1, 128 bytes) and its movie found (UiMovies::Find); with +4's
// bit 0x8: finish_all(the movie) (UiMovie::FinishAll), for logo 0 the two
// textures put back (UiMovie::SwapTexture(movie, [+84 + 4i], [+76 + 4i]))
// and release_xpr(+20) (RivalLogo::ReleaseXpr, never run in play), then
// remove(+8) (UiMovies::Delete). Then +8 = 0, +0 = -1, +4 = 1, +12, +16 = 0.
// Returns r3 as the original leaves it (Find's, or Delete's).
struct RivalLogoCloseCalls {
  std::function<void(uint32_t movie)> finish_all;
  std::function<void(uint32_t holder)> release_xpr;
  std::function<uint32_t(uint32_t id)> remove;
};
uint32_t RivalLogoClose(uint8_t* base, uint32_t entry, uint32_t frame,
                        const RivalLogoCloseCalls& calls);

// --- The views (src/camera_modes/views.cpp) ---

// sub_82153290 CameraTarget::Track: a camera's target tracker (camera + 32)
// following `vehicle` (nothing when 0): the last values kept (+16 = +0, +48
// = +32, +128 = the turn +64 (Matrix::Copy, noted for interpolation as the
// original's call at 0x821532E4), +208 = +192); +0 to +8 the body's
// position (its matrix's fourth row), +12 1.0; +32 to +40 the mean of the
// vehicle's +1216 and +1584 (Vec4::Copy), +44 1.0; +192 the body's
// rotation (+80, a quaternion) and +64 its turn (Matrix::FromQuat over the
// identity); +224 the vehicle's track position (TrackPosition::Copy of
// Vehicle::Identity). `frame` is the original's r1 after its stwu (256
// bytes, 16-byte aligned: FromQuat's scratch below it).
void TargetTrack(uint8_t* base, uint32_t tracker, uint32_t vehicle, uint32_t frame);

// sub_821529B0 Viewport::Offset and sub_82152788 Viewport::PullBack: the
// amount clamped to 0-1; the view's axes from the target less the eye and
// up (0, 1, 0) (the vector unit's cross products and normalizes, through
// the stack below `sp`, the caller's r1, as the originals'); the offset
// (+0, +4, +8) times the amount along right, up and forward added to the
// target (+1616, Offset) or the eye (+1600, PullBack).
void ViewportOffset(uint8_t* base, uint32_t viewport, double amount, uint32_t offset,
                    uint32_t sp);
void ViewportPullBack(uint8_t* base, uint32_t viewport, double amount, uint32_t offset,
                      uint32_t sp);
// sub_82152BD8 Camera::ShakeRandom: the step 0x826D81C0 made once (guard bit
// 1 of 0x826D81C4, from 0x8200939C); the amount clamped to 0-1; the offset
// (rand() % 100 x the step, rand() % 100 x the step, 0, 1) at `frame` + 96
// (the original's r1 after its stwu, 160 bytes); Viewport::Offset.
void ShakeRandom(uint8_t* base, uint32_t viewport, double amount, uint32_t frame);
// sub_82152CD8 Camera::Shake: (Vehicle::GetSpeedKmh(+16) - the float at
// 0x82001FB8) x the float at 0x8200222C, ShakeRandom on the viewport (+20).
// `frame` is the original's r1 after its stwu (96 bytes).
void Shake(uint8_t* base, uint32_t camera, uint32_t frame);

// sub_82152E78 IntroCamera::StepShot: a 20-byte shot (+0 the viewport, +4
// the vehicle, +8 its frame count, +16 its phase): the viewport's eye
// (+1600) and target (+1616) = its +1680 and +1696 turned by the body's
// matrix (Vec4::Transform), its up (+1632) = +1712, +1664 = +1744; the
// phase set to 3 once the count reaches 29 (phase 0); then 0 at phase 3,
// else 1 (the count up by one unless [0x826A50D4] is set). `frame` is the
// original's r1 after its stwu (272 bytes).
uint32_t IntroStepShot(uint8_t* base, uint32_t shot, uint32_t frame);
// sub_82154108 IntroCamera::MakeShot: shot +0 = the viewport, +4 the
// vehicle, +8, +16 0; the car's spec (sub_820C4550 of its +5712) sizes the
// viewport's +1680 (an orbit: sin and cos of the double at 0x820093A8) and
// +1696; +1712 = (0, 1, 0, 1) and +1744 = (0x8200295C's, 0x82008290's, 0,
// 1), statics made once (guard 0x826D81F0, bits 1 and 2, each with an
// atexit: 0x825DB150, 0x825DB148); shot +12 = the spec's +88 x the float at
// 0x82002CCC. `frame` is the original's r1 after its stwu (192 bytes).
void IntroMakeShot(uint8_t* base, uint32_t shot, uint32_t viewport, uint32_t vehicle,
                   uint32_t frame, const c_runtime::AtexitCall& atexit);
// Stages::Copy's original (out, stages, stage), where its plain C++
// refuses.
using StagesCopyCall = std::function<void(uint32_t, uint32_t, uint32_t)>;
// sub_82155E58 IntroCamera::Update (vtable slot 0 of 0x820093C4):
// CameraTarget::Track; with no shots (the list's count +424 0) the race's
// flag word (+0x59AE4) bit 1 set and phase (+756) 2, nothing else; phase 0:
// the current shot (+748 steps along the list) stepped (IntroCamera::
// StepShot, its frame 272 bytes below), +748 on when it ends, phase 1 and
// 0x826D82C4 = the frame count (+752) after the last; phases 1 and 2: an
// orbit about the car from its spec block (+76, +80, +84) over t = (the
// frames since 0x826D82C4) x 1/60 (0x820029E8) clamped to 0-1 (sin and cos
// of the double at 0x820093A8; a quadratic blend), the eye (+1600) and
// target (+1616, from (0, +80 x 0x82002CCC's, 0, 1)) through the body's
// matrix (Vec4::Transform); phase 1 with t at 1: phase 2 and the flag word's
// bit 1; then the frame count up unless [0x826A50D4] is set. `frame`: 320
// bytes.
void IntroUpdate(uint8_t* base, uint32_t camera, uint32_t frame);
// sub_82158F70 IntroCamera::IntroCamera: vtable 0x820093C4, the tracker's
// w words 1.0, +748 to +756 0, +16 the vehicle, +20 the viewport, its shot
// list (+352) reset; the stage's racers (Stages::Copy) whose record's
// +3936 has bit 0x2: their vehicles (Race's racer entries by id) gathered at
// `frame` + 80 (the count at +96), those flagged 0x400 whose item
// (DataTables::Item(+3960)) is of kind 1 or 3 moved to the front; a shot
// made for each (MakeShot) and added (List16::Insert20). Returns the
// camera. `frame` is the original's r1 after its stwu (368 bytes).
uint32_t IntroConstruct(uint8_t* base, uint32_t camera, uint32_t viewport, uint32_t vehicle,
                        uint32_t frame, const StagesCopyCall& stages_copy,
                        const c_runtime::AtexitCall& atexit);

// sub_82152338 CameraDirector::Start: `viewports_init` (Viewports::Init);
// for both viewports Viewport::Update, CameraPasses::SetNear(0x8200295C's),
// Viewport::SetZoom(1.0) and entry 0's far plane (+72) = 0x82001FCC's;
// +9636 = 0, +13348 = 2, +77408 = 0; CameraAnims::Clear.
void DirectorStart(uint8_t* base, uint32_t director, const std::function<void()>& viewports_init);
// sub_82153098 CameraDirector::Release: `free_all` (CameraAnims::FreeAll);
// with the race's kind 1, 2 or 3 the view (+9628, or +9632 in views 4 and
// 5) into +9624 and, for views 0-2, the save data's camera setting
// (SaveData::Of(0) + 0x1F85C + 24: 2, 1, 0); `release_viewports`
// (Viewports::Release).
void DirectorRelease(uint8_t* base, uint32_t director, const std::function<void()>& free_all,
                     const std::function<void()>& release_viewports);

// --- The director's state machine and views (src/camera_modes/director.cpp) ---

constexpr uint32_t kMachine = 16;  // the director's state machine
// sub_82159950 CameraDirector::InitStates: +9620 = 1, the state machine's
// (+16) 34 transitions added as the original does (its stores and copies
// replayed in `frame`, its 2,368 bytes: the words it never writes go into
// the transitions), then +9628, +9632 and +9624 = 0. Returns the last
// StateMachine::Add's plain C++'s answer (`replaced` its flag).
constexpr uint32_t kInitStatesFrame = 2368;
uint32_t DirectorInitStates(uint8_t* base, uint32_t director, uint32_t frame, bool* replaced);

// CutsceneShot::Init's guest calls: the copies List16::Copy32 and
// Insert32 refuse (memcpy), and CameraAnims::Load of the asset id.
struct CutsceneCalls {
  GuestCopy copy32;
  GuestCopy insert32;
  std::function<void(uint32_t id)> load;
};
// sub_82156B68 CutsceneShot::Init: a 608-byte shot from a script entry: +0
// its kind, +4 the asset id 22824 + kind (a table of 16 on the stack at
// `frame` + 80, the original's r1 after its stwu, 192 bytes), +8 its
// vehicle word; the cue list (+12) reset, +600 = 0, +604 = 1; the entry's
// cues (+8, count +80) copied in through the stack (Copy32, Insert32); the
// animation loaded (`calls.load`).
void CutsceneInit(uint8_t* base, uint32_t shot, uint32_t entry, uint32_t frame,
                  const CutsceneCalls& calls);
// sub_82155800 CutsceneShot::Step: the animation (+4) at the time (+600)
// evaluated at `frame` + 144 (CameraAnims::Evaluate; `camera_original`
// runs EvalCamera's original where its native form can't): when not loaded
// +604 = 4; else +604's bits 0x25 off and 0x8 on, the target and eye
// (z flipped by the float at 0x82001BE0, x and z swapped) turned by the
// car's body matrix into `out` + 32 and + 16, the roll (degrees) x pi/180
// into +64, the zoom into +68; the cues stepped (FadeCue::Step); the time
// on unless +604 has 0x10; +604 = 32 once the time reaches the duration
// (CameraAnims::Duration). Returns +604; `*f1` = the last f1 (Duration's,
// or EvalCamera's when not loaded). `frame` is the original's r1 after its
// stwu (304 bytes).
uint32_t CutsceneStep(uint8_t* base, uint32_t shot, uint32_t out, uint32_t frame,
                      const std::function<void(uint32_t object)>& camera_original, double* f1);
// sub_82155A28 Cutscene::Step and sub_821561F8 Cutscene::Update's calls
// that go through the translated code (the adapter's): EvalCamera's
// original for the shot's step (as CutsceneStep's `camera_original`, with
// the shot), RivalLogo::Close's calls, RivalLogo::Play's original when set
// (else RivalLogoPlay natively) and RivalLogo::Draw (its override, from a
// frame like Step's with the return address 0x82155BE0, for frame
// interpolation's noting).
struct CutsceneTickCalls {
  std::function<void(uint32_t shot, uint32_t object)> camera_original;
  RivalLogoCloseCalls close;
  std::function<void(uint32_t entry)> play_original;
  std::function<void(uint32_t entry)> draw;
};
// sub_82155A28 Cutscene::Step: r3 = the cutscene (the camera director's
// +56896): 1.0 at the frame's +108, +124 and +140; without +10764's bit 0x1
// the logo (+10760) played (RivalLogo::Play), returning 1 unless +10764 has
// 0x8 then. With a shot left (+360 below the count +1024, unsigned) and
// +10756 0, the shot +360 of the list at +952 stepped
// (CutsceneShot::Step(shot, frame + 80)); its 0x20 moves +360 on, and at
// the last the logo is closed (RivalLogo::Close) and 0 returned; unless its
// 0x4, the viewport (+20) gets the zoom (Viewport::SetZoom, the frame's
// +148), the near plane (CameraPasses::SetNear: +1868 / +1864 x the zoom),
// the roll (Viewport::SetRoll, +144), +1600 the frame's +96 (8 words),
// +1632 its own +1712 (4) and +1648 the cutscene's +32 (4); the logo drawn
// with +10764's 0x8 (`calls.draw`), +356 counted on. Returns 1 while
// running, 0 at the last shot. `frame` is the original's r1 after its stwu
// (192 bytes).
uint32_t CutsceneTick(uint8_t* base, uint32_t cutscene, uint32_t frame,
                      const CutsceneTickCalls& calls);
// sub_821561F8 Cutscene::Update (vtable 0x82006B3C's slot 0): with a shot
// (+352 not -1) Cutscene::Step (from `frame` - 192), returning its answer
// when its low byte isn't 0; else the race data's word +0x59AE4 gets bit
// 0 (the cutscene done, probably). Returns r3 as the original leaves it
// (Step's, or the cutscene). `frame` is the original's r1 (96 bytes).
uint32_t CutsceneUpdate(uint8_t* base, uint32_t cutscene, uint32_t frame,
                        const CutsceneTickCalls& calls);

// --- The views' cameras (src/camera_modes/chase.cpp) ---
// `frame` is each original's r1 after its stwu; the callees' frames below
// it, as the originals make them.

// The video output (the display's width at +296): [0x8266D088]'s vtable
// slot 7, then that object's slot 3, guest calls the adapter makes from the
// calling original's `frame` (its r1), with return addresses `back` and
// `back` + 16.
using VideoOutputCall = std::function<uint32_t(uint32_t frame, uint32_t back)>;

// Whether Camera::ZoomFromFov's native form stands in for the chase
// camera's two angles (0x82009300's and 0x82000D44's): its tan's checks.
bool ChaseReady(uint8_t* base);
// sub_82153478 ChaseCamera::Place: the viewport's +1680 = `eye`, +1696 =
// `target`, +1712 = `up`, +1744 and +1664 = `look` (four words each); +368 =
// 0.0; with a vehicle (+16): CameraTarget::Track, the tracker's rotation
// (+224) normalized (the vector unit) and kept at +240, the viewport's
// +1696 = (0, 0, 0, 1) and its y += the float at 0x820029F0 (1.3), the track
// position's section and slice (+260, +262) into +376 and +378. `frame`:
// 112 bytes.
void ChasePlace(uint8_t* base, uint32_t camera, uint32_t eye, uint32_t target, uint32_t up,
                uint32_t look, uint32_t frame);
// sub_82153640 ChaseCamera::Follow: with a vehicle (+16): CameraTarget::Track;
// zoom from 55 degrees (0x82000D44) at the display's width, SetZoom,
// SetNear(+1868 / +1864 x zoom), SetRoll(0.0); the turn's change since the
// last update (x through +160 and through +96, their dot product) picks the
// rate (1.0 or the float at 0x82001BE0 by whether (dot < 0) dot^4 is 0) x
// `rate`, at least `least`; the rotation (+224) blended by it from the last
// (+240) and normalized; the eye = the target's offset (+1664) about the
// tracked point (+64, Matrix::MultiplyTranslation of the turn +96) plus
// +1680 less +1664 turned by the rotation (Quat::Rotate), w 1.0; the target
// = +1696 through the same matrix; up (+1632) = +1712, +1648 = the
// tracker's position. `frame`: 320 bytes.
void ChaseFollow(uint8_t* base, uint32_t camera, double rate, double least, uint32_t frame,
                 const VideoOutputCall& video_output);
// sub_82154548 ChaseCamera::ChaseCamera: the tracker's w words 1.0, vtable
// 0x82006B24, +364 1.0, +16 the vehicle, +20 the viewport; the eye's and
// target's offsets from the car's spec block (sub_820C4550 of +5712: its
// +76 and +80 halved, x the float at 0x820029D8, the angle from the table
// at 0x8200930C by the word at +372, in degrees: sin and cos) on the stack;
// the statics (guard 0x826D8220: 0x826D8210 = (0, 1, 0, 1), atexit
// 0x825DB100; 0x826D8200 = (0x8200295C's, 0x82008290's, 0, 1), atexit
// 0x825DB0F8); zoom from 0x82009300's angle, SetZoom, SetNear, SetRoll;
// ChasePlace, CameraTarget::Track, ChaseFollow(0x82002CBC's, 0x82001FBC's);
// +352 = the eye. Returns the camera. `frame`: 224 bytes.
uint32_t ChaseConstruct(uint8_t* base, uint32_t camera, uint32_t viewport, uint32_t vehicle,
                        uint32_t frame, const VideoOutputCall& video_output,
                        const c_runtime::AtexitCall& atexit);
// sub_82154808 ChaseCamera::Update (vtable slot 0): ChaseFollow, Camera::Shake;
// the pull back (0x826D8224) stepped by the speed's change (Vehicle::
// GetSpeedKmh against 0x826D8228, which keeps it) and clamped,
// Viewport::PullBack by (0, 0, it); with +380 set, the eye moved along the
// view's line (by 0x820093B0's and 0x82002CAC's floats) and +380 = 0; +352
// = the eye. `frame`: 144 bytes.
void ChaseUpdate(uint8_t* base, uint32_t camera, uint32_t frame,
                 const VideoOutputCall& video_output);
// sub_82154A48 ViewCamera1::ViewCamera1: +16, +20 = 0, the tracker's w
// words 1.0, vtable 0x82006B2C, +1100 = 1.0; four statics (guard
// 0x826D8270, bits 1 to 8: 0x826D8260 = (0x820093B8's, 1, 0x82002A00's, 1),
// atexit 0x825DB120; 0x826D8250 = (1, 1, 0x82002A00's, 1), 0x825DB118;
// 0x826D8240 = (0, 1, 0, 1), 0x825DB110; 0x826D8230 = (0, 0, 0, 1),
// 0x825DB108); +16 the vehicle, +20 the viewport; zoom from 0x82009300's
// angle, SetZoom, SetNear(0.0), SetRoll(0.0); the viewport's +1680, +1696,
// +1712, +1744 = the statics; CameraTarget::Track; CameraCurve::Load(+352,
// 0, 30); +1088 = the rotation (+224); +1104 = 0.0. Returns the camera.
// `frame`: 192 bytes.
uint32_t View1Construct(uint8_t* base, uint32_t camera, uint32_t viewport, uint32_t car,
                        uint32_t frame, const VideoOutputCall& video_output,
                        const c_runtime::AtexitCall& atexit);
// sub_82153A50 ViewCamera1::Update (vtable slot 0): CameraTarget::Track;
// zoom, SetZoom, SetNear(0x82001FBC's), SetRoll(0.0); the car's model
// (ScenePools::FindModel([0x826C2E44], 2, n): n 1 in a race of kind 6 with
// +0x59614 2 and [0x826C7824] 0, else 0) picks the eye's and target's
// offsets (its +48 x 32 + 0x82618DC0 and + 0x82618DD0), through the turn
// about the tracked point (Matrix::MultiplyTranslation) into +1600 and
// +1616 (+1664 = +1616), up (+1632) = +1712 through the turn; the target
// moved to the eye + the unit direction turned by +1104 (Matrix::RotateY);
// Racers::NearestHumanVehicle (unused); the look point. `frame`: 352 bytes.
void View1Update(uint8_t* base, uint32_t camera, uint32_t frame,
                 const VideoOutputCall& video_output);
// sub_82154D38 ViewCamera2::ViewCamera2: as View1Construct with vtable
// 0x82006B34 and its statics (guard 0x826D82C0: 0x826D82B0 = (1,
// 0x82002CB0's, 0, 1), atexit 0x825DB140; 0x826D82A0 = (0x82001FD8's,
// 0x82002CB0's, 0, 1), 0x825DB138; 0x826D8290 = (0, 1, 0, 1), 0x825DB130;
// 0x826D8280 = (0x820029C8's, 0x82002CB0's, 0, 1), 0x825DB128); SetNear(the
// ratio x zoom); with a vehicle, the eye (+1680, +1684) and target (+1696,
// +1700) from its spec's +76 / 10 and +76, +80 halved; CameraCurve::Load,
// CameraTarget::Track, +1088 = the rotation. `frame`: 192 bytes.
uint32_t View2Construct(uint8_t* base, uint32_t camera, uint32_t viewport, uint32_t car,
                        uint32_t frame, const VideoOutputCall& video_output,
                        const c_runtime::AtexitCall& atexit);
// sub_82153E58 ViewCamera2::Update (vtable slot 0): CameraTarget::Track;
// zoom, SetZoom, SetNear, SetRoll(0.0); the eye (+1600), target (+1616) and
// +1664 = +1680, +1696, +1744 through the turn about the tracked point, up
// (+1632) = +1712 through the turn; Racers::NearestHumanVehicle (unused);
// the look point (+1648 = the eye + the unit direction). `frame`: 240 bytes.
void View2Update(uint8_t* base, uint32_t camera, uint32_t frame,
                 const VideoOutputCall& video_output);

// The garage camera's guest calls: the scene's render queue and command
// list (the app interface [0x826BABA0]'s slot 8, then that object's slot
// 2; slot 8 again, then slot 1: four guest calls from the garage camera's
// `frame` with its return addresses), and CameraPass::Update on `pass` as
// CameraPass::FromViewport's tail (from FromViewport's 112-byte frame below
// `frame`, through the wrapper that keeps its diagnostics).
struct GarageCalls {
  std::function<std::pair<uint32_t, uint32_t>(uint32_t frame)> queue_and_list;
  std::function<void(uint32_t pass, uint32_t frame)> pass_update;
};
// Whether GarageUpdate's tan (of the camera's +224 times pi/180, the float
// at 0x82002948) stands in: nearest rounding and c_runtime::Tan's checks.
bool GarageReady(uint8_t* base, uint32_t camera);
// sub_8217BE98 GarageCamera::Update (probably; from the front end's car
// draw sub_8217FE60, r3 its object's +112): 0x8296B918 = the command list,
// 0x8296B91C = the render queue, 0x8296B8F4 = 0.0; the eye (`frame` + 80)
// = (sin b cos a d, sin a d, cos b cos a d, +28) with a = +100, b = +176,
// d = +180 (an orbit), the target (`frame` + 96) = +32; +108 = 640.0
// (0x82009EB8) / tan(+224, degrees, in radians), Viewport::SetZoom(viewport 0,
// that), CameraPasses::SetNear(0.01), Viewport::SetRoll(+248); the eye and
// target through the identity with +256 to +264 as its translation
// (Vec4::Transform), Viewport::LookAt; CameraPass::FromViewport(pass 0,
// 0, 0) and CameraPass::Enable(pass 0); 0x8265ECE0 = the eye. `frame` is
// the original's r1 after its stwu (272 bytes, 16-byte aligned).
void GarageUpdate(uint8_t* base, uint32_t camera, uint32_t frame, const GarageCalls& calls);

// --- The TV camera's route and paths (src/camera_modes/paths.cpp) ---
// `sp` is the caller's r1: each function's frame is below it by the
// original's size, its locals written there as the original writes them.

// sub_820FE830 RouteRange::CameraAt: the range's state +0 set to 1 around
// RouteRange::Test of the position at `position` (the old state put back):
// the word at 0x82603274 (the slices OnPath counted) when it's on the range
// (2), else -1 (the original's r3, 64 bits).
int64_t RangeCameraAt(uint8_t* base, uint32_t range, uint32_t position);
// sub_820FE598 RouteRange::Advance: out (section, slice halfwords) =
// `distance` slices on from the range's start (+4) along the route toward
// its end (+8): each section's slices (its +0) taken off while the distance
// covers them, then its one link, or at a fork the link that's in the
// range's list of sections to take (+12); the end's section reached, or the
// slices left, ends it.
void RangeAdvance(uint8_t* base, uint32_t out, uint32_t range, int32_t distance);
// sub_820F9B48 TrackRoute::SliceFraction (our name): how far the point is
// along the slice at `position` (a packed word): the point's feet on the
// lines through the slice's two edge points and the next slice's (on in
// the section, else its link 0, TrackRoute::Link), the fraction of the
// step between them (dot products on the vector unit). Returns f1.
double SliceFraction(uint8_t* base, uint32_t position, uint32_t point, uint32_t sp);
// sub_82156980 TvCamera::Track: the TV camera's 40 ranges (+456, 1,064
// bytes apart, their count +452) updated with the position at `position`
// (RouteRange::Update), the ones it's on listed (List40::Insert, a list on
// the stack at `frame` + 96; `frame` is the original's r1 after its stwu,
// 496 bytes), and for each listed range no path buffer (+43016, four
// 28-byte buffers, +12 the index) has, the first buffer empty or loaded
// (+24 0 or 3) asked for it (`request`: CameraPath::Request); once one is
// found in a buffer, none after it are asked for (as the original's flag).
void TvTrack(uint8_t* base, uint32_t camera, uint32_t position, uint32_t frame,
             const std::function<void(uint32_t buffer, uint32_t index)>& request);
// sub_82293A48 Hermite::Basis: out (2 floats) = the cubic Hermite blend at t
// of the keys a and b (+0, +4 a value pair, +8, +12 their tangents).
void HermiteBasis(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, double t);
// sub_820AC060 Curve::Evaluate: a curve of 24-byte keys (+0 the time, +4 the
// value, +8 the value it leaves, +16 its tangent, +20 the kind; kind 3 the
// end key) at the end key's time x t: the key before the first not earlier
// by its kind: 0 its +8, 1 a line to the next, 2 Hermite::Basis (the keys on
// the stack); 3 or more the stack's word at +80 (never seen).
double CurveEvaluate(uint8_t* base, uint32_t curve, double t, uint32_t sp);
// sub_820AC178 Key::Value: a (kind, value) pair: 0 the float, 1
// Curve::Evaluate of the curve, else 0.0; times the float at 0x82002834.
double KeyValue(uint8_t* base, uint32_t pair, double t, uint32_t sp);
// sub_820AC1D0 Key::Vec3: three pairs' Key::Value into out's x, y, z; w 1.0.
void KeyVec3(uint8_t* base, uint32_t pairs, uint32_t out, double t, uint32_t sp);
// sub_820AB5F0 Matrix::FromRows3 (our name): the identity at `out` (16-byte
// stores), then rows 0 to 2 copied from x, y and z (four words each).
void MatrixFromRows3(uint8_t* base, uint32_t out, uint32_t x, uint32_t y, uint32_t z);
// sub_820AB6A0 TrackRoute::SplinePoint: the course's route at `position` (a
// packed word, also stored at sp + 36 as the original does) and t: four
// slices' centers (the slice before and the two after, over the links) on
// a Catmull-Rom spline (its basis matrix made once: guard 0x826C2D70 bit 1,
// Matrix::FromFloats into 0x826C2D30 and atexit(0x825DAE08)), the point
// times the float at 0x82001BE4 into `out` (3 floats), and `matrix` its
// frame: the tangent, up and side (from the slices' edge points) normalized
// (Matrix::FromRows3), flipped when up's y is below 0.
void SplinePoint(uint8_t* base, uint32_t out, uint32_t matrix, uint32_t position, double t,
                 uint32_t sp, const c_runtime::AtexitCall& atexit);
// sub_820ABDA8 CameraPath::RoutePoint: out (3 floats) = the route's point at
// the position (TrackRoute::ClampPosition, SplinePoint) plus the offset
// turned by its frame (Vec4::Transform).
void RoutePoint(uint8_t* base, uint32_t out, uint32_t position, uint32_t offset, double t,
                uint32_t sp, const c_runtime::AtexitCall& atexit);
// sub_820AC248 CameraPath::Evaluate: a 216-byte key at t (clamped to 0-1)
// into the camera's `out`: +64 the field of view (+8, x the float at
// 0x8200283C), +68 the float at 0x82002838 / the zoom (+16), the eye +16
// (+24) and target +32 (+120); the eye and target blended by their weights
// (+72, +168) toward +48's and +144's offsets (x and z swapped) turned by
// `transform`'s matrix (+16) plus its position, and by +104's and +200's
// toward the route's point (RoutePoint) +112's and +208's slices along the
// range (RangeAdvance) with +80's and +176's offsets; z flipped (the float
// at 0x82001BE0) where neither moved them.
void PathEvaluate(uint8_t* base, uint32_t key, uint32_t out, uint32_t range, uint32_t transform,
                  double t, uint32_t sp, const c_runtime::AtexitCall& atexit);

// --- The TV camera (src/camera_modes/chase.cpp, paths.cpp) ---

// sub_82155288 TvCamera::TvCamera (view 4's camera, made in place by
// CameraDirector::SetMode): +16, +20 = 0, the tracker's w words 1.0, vtable
// 0x820093BC, +396, +412, +428 = 1.0; the four path buffers (+43016,
// 28 bytes apart: vtable 0x82009390, +12 and +16 -1, +20 and +24 0); the
// fallback chase camera's (+43136) +16, +20 0, its tracker's w words 1.0,
// vtable 0x82006B24, +364 1.0; +16 the vehicle, +20 the viewport; zoom from
// 0x82009300's angle, SetZoom, SetNear(+1868 / +1864 x zoom);
// CameraTarget::Track; +448 = 0, +43508 (the chase camera's +372) = 2;
// ChaseCamera::ChaseCamera(+43136, its frame 224 bytes below); then the
// +452 route ranges (+456, 1,064 bytes apart; RouteRange::Init from the
// camera path's record [0x82618D30 + 4 x i]: its +4 section, slice 0, to
// its +8 section's last slice, on the stack at `frame` + 80) and twelve of
// them's forks (RouteRange::AddBranch); +43520 = 0. Returns the camera.
// `frame` is the original's r1 after its stwu (160 bytes); `atexit` the
// chase camera's.
uint32_t TvConstruct(uint8_t* base, uint32_t camera, uint32_t viewport, uint32_t vehicle,
                     uint32_t frame, const VideoOutputCall& video_output,
                     const c_runtime::AtexitCall& atexit);
// TvCamera::Step's guest calls: the copy c_runtime::Memcpy refuses (memcpy
// from Step's frame, return address 0x8215634C), and CameraPath::Request
// (the camera, a buffer, an index) from Step's frame with the return
// address `back`.
struct TvStepCalls {
  GuestCopy copy;
  std::function<void(uint32_t buffer, uint32_t index, uint32_t back)> request;
};
// sub_821562F8 TvCamera::Step: CameraTarget::Track; the track position
// (+260) at `frame` + 80, the tracked point (+32, 16 bytes) and turn (+96,
// 64 bytes, memcpy) at + 176 (the paths' transform); the +452 ranges updated
// (RouteRange::Update), those the position is on listed (List40::Insert, a
// list at `frame` + 256); none: nothing more. The spot (+360) kept when
// listed and more than one is, else the first listed; its range's camera
// slice (RouteRange::CameraAt). No path buffer (+43016) holding the spot:
// each buffer empty or loaded (+24 0 or 3) asked for it unless its index is
// the spot (CameraPath::Request, back 0x82156544). A buffer holding it and
// loaded (3): its first 20-byte path record (+4 to +8) covering the slice
// (+0 and +4 its two ends, either order); the slice's fraction (TrackRoute::
// SliceFraction) and its place between the ends, clamped to 0-1;
// +43520's state stepped (0 to 1, 1 to 2 near the far end, 2 to 0 back at the
// near end); the record's key (+16) evaluated (CameraPath::Evaluate) into
// +368, the viewport's zoom (+436), near plane, roll (+432), eye (+384),
// target (+400), up (+1712) and look point (the tracked point) set. With
// the slice changed from +352: +356 one past the record's far end (or
// before its near end, going back), the record covering that (evaluated at
// t 1.0 or 0.0 into `frame` + 96, its eye into the viewport's +1840), or,
// none, the route's twelve links from the position's section: each on a
// range (RouteRange::Test) not held by a buffer asked for (back 0x821568D8),
// and +1840 the eye; +352 = the slice. `frame` is the original's r1 after
// its stwu (720 bytes).
void TvStep(uint8_t* base, uint32_t camera, uint32_t frame, const TvStepCalls& calls,
            const c_runtime::AtexitCall& atexit);
// TvCamera::Update's guest calls: Step's, the video output, a fallback
// camera's slot 0 that isn't ChaseCamera::Update's (the function, the
// camera), and ColorFilter::SetStyle's original where its plain C++ refuses.
struct TvUpdateCalls {
  TvStepCalls step;
  VideoOutputCall video_output;
  std::function<void(uint32_t function, uint32_t camera)> slot0;
  std::function<void(uint32_t style)> set_style_original;
};
// sub_821593D8 TvCamera::Update (slot 0 of 0x820093BC): the fallback chase
// camera's (+43136) slot 0 (ChaseCamera::Update, its frame 144 bytes
// below); TvCamera::Step (its frame 720 bytes below) when +448 is 0. The
// attract race's color filter: the style (0x826D82C8) = 0 once (guard bit
// 1 of 0x826D82CC); while +43520 is 0, the style = (0, 1, 2, 3) at `frame`
// + 80 by rand() % 4 (the signed remainder); ColorFilter::SetStyle(the
// style). `frame` is the original's r1 after its stwu (128 bytes).
void TvUpdate(uint8_t* base, uint32_t camera, uint32_t frame, const TvUpdateCalls& calls,
              const c_runtime::AtexitCall& atexit);

}  // namespace camera_modes
