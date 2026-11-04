# Known functions: the car scene's draw

The car scene's draw (`sub_820C5DE8(scene, matrix)`, the scene at
`[0x826C2E44]`, `scene.md`): once a frame wherever cars are shown (the
race's draw step `Race::DrawStep`, the parking area's `LocationScreen::PollLoad`
and `Update` in `location-screen.md`, the front end's car draw
`sub_8217FE60` and its scene owner `sub_82179AE8`, both leads), always
right after `CarScene::Update`. It records, on the scene's command list,
the chunks that set up the projected textures the cars' materials sample
(a projection matrix as vertex constants, the sampler states, and a
camera pass's render target or a shared texture as the texture), each
handed to the render queue under a sort key from a table, and then, for
each pass description of a second table, draws every scene model
(`Car::DrawUpdate`, `car-draw.md`) with the description copied to the
scene's +5072 (`../known-data.md`, "scene pass description") and the
render queue's sort keys `0x8296B90C`, `0x8296B900`, `0x8296B908` set
from it (put back after each car). Two paths by the scene's +4 (the
set-up record's first word): with 0 (probably the front end) it sets the
camera passes up itself (all 21 disabled, `EnableForView`, the begins
and ends recorded) and records only the light's projection for each of
three records (an orthographic view along the light,
`Projector::BuildOrtho`); otherwise (the race, by `Car::DrawUpdate`'s
row, and the parking area, which sets the +5052 only this path reads) it
records four projections a record: the player's headlight projector
(`Projector::Build`), the shared textures on samplers 11 to 15, and three
projections fitted to the car's view frustum (`TextureProjection::Fit`:
two straight down between heights around the car, one along the light),
and between the scene models it fills the scene lights' slots 2 to 7 with
the other cars' headlights and hides or shows the player's car for the
pass by the view mode. `TextureProjection::Fit` and its helpers are a
small geometry library (the frustum's planes and corners, a plane's
crossing with a segment, a box over a `std::vector` of points); the
projectors' matrices (`Matrix::LookAt`, `Matrix::Perspective`) are
shared with the course scenery's `Scenery::RecordHeadlightProjectors`
(`sub_820F34C8`, `course-scenery.md`), which also calls the scene
model's beam vectors `sub_820DAA38`, `sub_820DAAC8` and `sub_820DAB10`.

Read by the B7 loop's mapping agent (2026-11-04) from the lead of the
scene's draw, with no subagents: the root read through for its control
flow, calls, globals and command list chunks (the stack's float
arithmetic between them only in outline), the small functions in full,
`TextureProjection::Fit` and `std::vector`'s insert in outline (calls,
loops, arguments). Shared, with rows elsewhere: the camera passes'
`CameraPass::Disable`, `CameraPasses::EnableForView`, `RecordBegins`,
`RecordEnds` and `RenderTarget` (`cameras.md`); `Matrix::Multiply`,
`Matrix::Copy`, `Matrix::Inverse` and `Vec4::Transform`
(`matrix-math.md`); `CommandList::OpenChunk`, `AllocateSlots` and
`CloseChunk` (`command-list.md`); `List::At` and `Stages::Copy`
(`vehicle-accessors.md`); `XPR::GetTexture` (`functions.md`); `memcpy`
and `tan` (`c-runtime.md`); `operator new` and `operator delete`
(`heap.md`); `Car::DrawUpdate` (`car-draw.md`); the beam color table
`sub_820D35D0` (`material-passes.md`). Reached through vtables, without
rows: the scene's render manager `[0x826BABA0]` (slot 8 `sub_820944D8`,
then the command list from its answer's slot 1 and the render queue from
slot 2, whose slot 4 takes each chunk; `RenderQueue::Add` in
`device-shaders.md`), and the shared assets' texture provider at the
scene's +432 (its slot 3, probably `sub_825946B8`, a getter; both leads).
Never seen in play, without rows (as in `models.md`): `std::vector`'s
`_Xlen` (`sub_825981C8`) and `bad_alloc`'s copy (`sub_82091200`,
`std-string.md`). Fields: `../known-data.md` ("scene pass description",
"car scene draw tables", "frustum", the scene's and the scene model's
rows).

What wraps or reads these functions, kept around native bodies by the
migration (the precedent of the car draw and `XPR::GetTexture`): the
root, `sub_820C4EB8`, `sub_820C4FB8`, `sub_820DAA38`, `sub_820DAAC8`,
`sub_820DAB10`, `Projector::BuildOrtho`, `Projector::Build` and
`TextureProjection::Fit` are Wrapped by the diagnostics
(`ITC_FRAME_STATE_WRAP` in `src/debug/frame_state_diff.cpp`,
`--find_per_frame_state`, `--debug_hide`), and the root's callees
`CameraPasses::RenderTarget`, `XPR::GetTexture` and `Vec4::Transform`
have their adapters there too. Frame interpolation reads what the root
records, not the root itself: it follows `Matrix::Copy` and
`Matrix::Multiply` into cameras (`matrix-math.md`) and replays the
command 7 uploads (`../frame-interpolation.md` lists the root's
four-register uploads by return address, `0x820C6B14`, `0x820C725C`,
`0x820C7640`, `0x820C7A38`; nothing in `src/` keys on those or on the
root's other return addresses, checked 2026-11-04); the headlight
projector's matrix and `TextureProjection::Fit`'s temporary camera are
in "The headlight beam far from the origin" there. The scene's draw
writes things that outlive it, to keep as the original does: the
camera passes' view-projections (+144) of the projectors' passes, the
scene lights' arrays at `0x8296B950` to `0x8296BCDF` and `0x8265ECA0`,
`0x8265ECE0`, `0x8265ED00` (the scenery's draws read them), the scene
models' +4296, `[0x8296B8F0]`, and the sort keys `0x8296B900`,
`0x8296B90C` from `[0x82601858]` and `[0x82601854]`; pass 0's
view-projection is put back from a copy at the end (it's multiplied by
the optional matrix for the draw).

Constants read (not read from the image here): `[0x82000A30]` 1.0,
`[0x82000A34]` 0.0 (as in `car-draw.md`); the others by address in the
rows.

Status:

- **Native**: plain C++ in `itc_recomp/src/` that doesn't use the
  PowerPC context (`ctx`): normal arguments and host structs (it may
  still read and write guest memory through `src/guest.h` while
  recompiled code shares the data). The game reaches it without `ctx`,
  from native or overridden callers, and the recompiled original is no
  longer called (except behind a fallback flag or to check against it).
  No `REX_FUNC` override of it exists, not even an adapter kept only for
  a harness (that's Overridden until the adapter goes).
  The end state for every function.
- **Overridden**: an `extern "C" REX_FUNC(sub_XXXXXXXX)` override in
  `itc_recomp/src/` (or a macro defining one) does the work instead of
  the original: a thin adapter in front of native code, or a body that
  still uses `ctx` (calling guest code, reading registers). It may call
  the original when its flag is off, or to check itself against it.
- **Wrapped**: an override that does something of its own (skipping
  render-only passes, logging, timing) and still calls the original
  (`__imp__sub_XXXXXXXX`) for the work.
- **Dropped**: its work isn't needed on PC (hardware set-up, kernel calls
  the SDK already stubbed), so it no longer runs: an override in
  `itc_recomp/src/` replaces it with nothing (or a fixed return value), or
  its callers, made native, leave the call out (no override then; the row
  says which caller). Logs, a harness or the renderer's frame dumps have
  shown that nothing reads what it did. The original is no longer called,
  except behind a fallback flag while the drop is being checked. A
  function the game never calls in the first place isn't Dropped: its
  status stays blank and its row says it's never called.
- **Merged**: its work is done inside a native caller's plain C++ (a
  few lines only that caller needs), so it has no function or override of
  its own in `itc_recomp/src/`. The row names the caller(s) and the
  harness that covers the work. The original is no longer called, except
  behind the caller's fallback flag.
- **Patched**: the generated code was edited in place (search for
  `PATCH:`).

A function usually moves from recompiled or Wrapped to Overridden to
Native as the `ctx` edge moves up through its callers, or straight to
Dropped when its work isn't needed.
`itc_recomp/tools/check_function_statuses.py` checks this column, in
every file of the tables, against `src/` (an override marked Native, a
status with no override, an override with no row, a row in two files);
run it after changing either, on any branch.

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_820C5DE8` | `CarScene::Draw` | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). Read through (B7 loop, 2026-11-04; 2,470 instructions, 177 indirect calls; the frame's float arithmetic between the calls in outline): r3 = the scene (`[0x826C2E44]`), r4 = a matrix or 0 (only `sub_8217FE60`'s first call passes one, its frame's +80; the five other call sites pass 0). Nothing when the scene's +0 is 4 (tearing down). Pass 0's view-projection (`0x826BAC00` + 144) is copied to the frame and put back at the end (`Matrix::Copy`); with r4 it is r4 × it (`Matrix::Multiply`) for the draw. **Scene +4 = 0** (probably the front end): `[0x8296B8F0]` = `XPR::GetTexture(scene + 88, 0)` when the shared assets' slot 0 is done (+84 = 3), else 0; `CameraPass::Disable` on the 21 passes, `CameraPasses::EnableForView`, `RecordBegins`, `RecordEnds`; the light's direction is `0x8296B950`'s first vector, normalized. Then for each of the three records at `0x82002D40` (`../known-data.md`, "car scene draw tables": w0 to w14), when pass w0 is active (+588) and the record's scene model exists (`List::At(scene + 5028, sub_820C4FB8(w9))`): `sub_820C4EB8(scene + 5072, the direction, [0x82002FB0])`; the model's position (its matrix +4304 copied with `memcpy`, times (0, 0, 0, 1) by `Vec4::Transform`); `Projector::BuildOrtho` (eye = the position plus the direction times `[0x820021E4]`, target the position, up (0, 1, 0), width and height `[0x820021F4]`, near 0.0, far `[0x82000C34]`) into pass w4's view-projection (+144, `Matrix::Copy`); and a chunk on the scene's command list (`[0x826BABA0]`'s slot 8, then slot 1, before each allocation; `CommandList::OpenChunk`, `AllocateSlots`, `CloseChunk`): a header (halfword +2 = 5, +4 = 0), command 7 vertex c14 x4 = the matrix, command 2 with sampler 7's states 20 and 16 = 0 and 24 = 2, command 4 with sampler 7 = `CameraPasses::RenderTarget(w8)`, command 7 pixel c77 = (pass w4's +504 and +508 as floats, 0.0, 0.0), command 36; the chunk handed to the render queue (slot 8, then slot 2; its slot 4) with key w14 and f1 = 0.0. **Scene +4 not 0** (the race, by `Car::DrawUpdate`'s row in `car-draw.md`; the parking area): `[0x8296B8F0]` = `XPR::GetTexture(scene + 88, 0)` when the scene's +5044 is set, else `CameraPasses::RenderTarget(1)`; the direction is (`[0x82001BE8]`, 1.0, 0.0, 0.0) normalized, or with the scene's +5052 set (the parking area, `LocationScreen::Update`) `0x8296B950`'s first vector normalized. With +5052 0 it sets the scene lights' arrays: the headlight projector slots (8 at `0x8296BAE0` and 8 at `0x8296BB60`) and the three vectors at `0x8296B980` and `0x8296B9B0` cleared to 0.0, then the three at `0x8296B950`, `0x8296B980` and `0x8296B9B0` from the direction and constants (`[0x82001BE4]`, `[0x8200296C]`, 1.0; `0x8296B9D0` = (0, 0, 0, 1)) and `0x8265ED00` = the scene's +5280 (16 bytes). Then for each of the three records, when pass w0 is active and the model exists: (1) the headlight projector: `sub_820DAAC8` of the model as the beam, the eye = the scene's +5376 through the model's matrix, the target along minus the beam through it (normalized), `Projector::Build` (field of view the scene's +5436, aspect and near 1.0, far `[0x82002FA8]`) into pass w1's +144, and its chunk: c87 x4, sampler 8's states (20, 16 = 0; 24 = 2), sampler 8 = `RenderTarget(w5)`, pixel c76 = pass w1's size, command 36, key w10; (2) a chunk for samplers 11 to 15 (states 20, 16 and 24 = 1 each) and, when the texture provider at the scene's +432, its slot 3's answer (r4 = 0) and that XPR's texture 0 (`XPR::GetTexture`) are there, each sampler = that texture; key w11; (3) `TextureProjection::Fit(out, pass w0, (0, 1, 0, 0))` with f1 = the model's height + `[0x82001FD8]`, f2 = the height − `[0x820021F4]`, f3 and f4 = `[0x82002C38]` and `[0x820021F4]` times `[0x82002FAC]` / pass w0's +368, f5 = `[0x82002204]`, into pass w2's +144, and its chunk: c70 x4, sampler 9 (20, 16 = 1; 24 = 2), sampler 9 = `RenderTarget(w6)`, pixel c68 = (0, 1, 0, the height), key w12; (4) the same with f1 = the height + `[0x820021F4]`, f3, f4 = `[0x82002A0C]`, `[0x82002CAC]` times the ratio, f5 = `[0x82002A1C]` into pass w3 (c74 x4, sampler 10, `RenderTarget(w7)`, pixel c69), key w13; (5) `sub_820C4EB8(scene + 5072, the direction, [0x82002204])` and `TextureProjection::Fit` along the direction (f3, f4 = `[0x82000D44]`, `[0x82002674]` times the ratio, f5 = `[0x8200222C]`) into pass w4's +144, and its chunk: c14 x4, sampler 7 (20, 16 = 0; 24 = 2), sampler 7 = `RenderTarget(w8)`, no pixel constant, key w14. Every chunk goes to the queue with f1 = 0.0. **Then, both paths:** `[0x8296B90C]` = `[0x82601854]`, `[0x8296B900]` = `[0x82601858]`; the player's model p = `sub_820C4FB8(0)` (inline); for each pass description (the first path one, `0x82002F38` when the mode type `[0x826032DC]` is 8, never made in play, else `0x82002EF8`; the second path four, from `0x82002DF8`), its 64 bytes copied to the scene's +5072 (`memcpy`); skipped when its pass (+5072) isn't active; `[0x8265ECE0]` = the pass's eye (+400) and `Matrix::Copy(0x8265ECA0, the pass's view +16)`; in the second path, by the pass's type (+584): 0, unless the race kind (`[0x826C5B2C]` + 0xA3AF0) is 6 with the current stage's word +0 (`Stages::Copy` of +0x59620, stage -1) not 1, or the race data's +0x59AE4 has 0x80: model p's +4296 = 0 when viewport 0 (`[0x826C2CE8]` + 0) exists in view mode 2 (+1860, probably the cockpit), else 1; 1: model 1's +4296 = 1, then model p's = 0 when viewport 0 is in view mode 2; 2: model p's = 1, then model 1's = 0 when viewport 1 (+4) is in view mode 2; 3: model p's = 0; above 3 nothing (each only when the model exists). Then for each scene model i (`List::At(scene + 5028, i)`, i below the scene's +5036) in use (+16 = 3) with a car model (+5048): in the second path with +5052 0, the 16-byte slots of the six arrays at `0x8296B9E0` + 128k (k 0 to 5: `0x8296BAE0` and `0x8296BB60`'s 8 slots first set to 0.0) from slot 2 on get the other models in use (not i, with +5048, kind (+32) not 3, traffic; up to six) as lights: the position (the scene's +5376 through its matrix), minus the beam (`sub_820DAAC8`) through it, normalized, `sub_820DAB10`'s vector, `sub_820DAA38`'s color times the scene's +5248 to +5256, the second `sub_820DAA38` times +5264 to +5272, and the scene's +5440. Then the description's +64, +68, +72 = `[0x8296B90C]`, `[0x8296B900]`, `[0x8296B908]`, and those = its +20, +24, +28; the mask (+5132, the description's +60) for the call: 0x4 cleared for type 0 or 1 with i = p and type 2 with i = 1; for type 0 with the scene's +5052 set, also 0x8 and 0xE0 (any i); `Car::DrawUpdate(model)`; then the mask, `[0x8296B908]` (from the scene's +5144), `[0x8296B900]` and `[0x8296B90C]` put back. Callers: `Race::DrawStep` (`functions.md`), `LocationScreen::PollLoad` and `Update` (`location-screen.md`), `sub_8217FE60` (twice) and `sub_82179AE8` (leads), each right after `CarScene::Update`. Its render manager, command list, render queue and texture provider calls are the 177 indirect calls (intro). Name ours. |
| `sub_820C4FB8` | `CarScene::PlayerModel` (probably) | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). Read in full (2026-11-04): r3 = an index, returned as it is unless 0; for 0, when the race data's (`[0x826C5B2C]`) +0xA3AF0 (the race kind) is 6 and its +0x59614 is 2, returns 1 when `[0x826C7824]` is 0, else 0 (the word `Minimap::Draw` reads to pick the human car); otherwise 0. Leaf. Only caller the scene's draw (each record's w9, and inlined for the player's model in the pass loop). Probably the race kind 6's second human car. |
| `sub_820C4EB8` | `ScenePass::SetLight` (probably) | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). Read in full (2026-11-04): r3 = the pass description (the scene's +5072), r4 = a direction (16 bytes), f1 = a distance: +96 = r4 scaled by 1 / the length of its x, y, z (0 when that length is 0); +112 = the identity (64 bytes, built in the frame, `memcpy`), then its last row (+160) = (-f1 × the scaled x, y, z, 1.0): a translation by f1 against the light. `Car::DrawUpdate` reads +96 and +112 (probably the planar shadows'). Only caller the scene's draw (f1 = `[0x82002FB0]` in the first path, `[0x82002204]` in the second). |
| `sub_820DAAC8` | `SceneModel::Beam` (probably) | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). Read in full (2026-11-04): r3 = out (16 bytes), r4 = a scene model: the scene's (`[0x826C2E44]`) +5408 when the model's light bits (its block's +4, the model's +4292, read with a doubleword load) have 0x40 (the high beams, probably), else +5392. Leaf. From the scene's draw (the headlight projector's direction and the other cars' lights) and `Scenery::RecordHeadlightProjectors` (`course-scenery.md`). |
| `sub_820DAB10` | `SceneModel::BeamShape` (probably) | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). Read in full (2026-11-04): as `sub_820DAAC8` with the scene's +5472 (high beams) or +5456: the third vector of each light slot the scene's draw fills for the other cars (probably the beam's cone or reach). Leaf. From the scene's draw and `Scenery::RecordHeadlightProjectors` (`course-scenery.md`). |
| `sub_820DAA38` | `SceneModel::BeamColor` (probably) | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). Read in full (2026-11-04): r3 = out (16 bytes), r4 = a scene model: `sub_820D35D0` (`material-passes.md`: the table at `0x82601A00`) with the model's +48 (the car record's +0, the block) and +3776 (the parts record's +32, 0 to 3; probably the headlights' color); with the high beams (+4292 & 0x40, as `sub_820DAAC8`) into its frame, then out's x, y, z = those times `[0x820029D4]` and w = 1.0. Returns out. From the scene's draw (twice for each other car's light) and `Scenery::RecordHeadlightProjectors` (`course-scenery.md`). |
| `sub_82597518` | `Projector::BuildOrtho` | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). Read in full (2026-11-04): r3 = out, r4 = eye, r5 = target, r6 = up, f1 = width, f2 = height, f3 = near, f4 = far: `Matrix::LookAt` into its frame (whose w column it sets to 1.0 first), times an orthographic matrix (`[0x820021E8]` / width, `[0x820021E8]` / height, 1 / (far − near), offset near / (near − far), w 1.0, the rest `[0x82000A34]`, 0.0), then times the bias matrix at `0x8265EED0` (two `Matrix::Multiply`), as `Projector::Build`. `[0x820021E8]` probably 2.0. Only caller the scene's draw (the first path's light projection). |
| `sub_82597610` | `Projector::Build` | Wrapped | r3 = out, r4 = eye, r5 = target, r6 = up (16-byte vectors), f1-f4 = field of view, aspect, near, far: out = look-at (`sub_82597378`) * perspective (`sub_825972A8`) * a fixed bias matrix at `0x8265EED0` (two `sub_8209BD50` multiplies). Called by the render orchestrator `sub_820C5DE8` (return `0x820C6A2C`) for the player's headlight once a view, and by `sub_820F34C8` (return `0x820F3944`) in a loop of up to 5. See "The headlight beam far from the origin" in `../frame-interpolation.md`. Also one of the race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). In the scene's draw (2026-11-04): the second path's headlight projector, once for each record whose pass and model are there (field of view the scene's +5436, aspect and near 1.0, far `[0x82002FA8]`). The two rows `functions.md` had for it merged here. |
| `sub_82597378` | `Matrix::LookAt` | | r3 = out, r4 = eye, r5 = target, r6 = up: a row-vector view matrix, forward = target - eye. Leaf. From `Projector::Build` and `Projector::BuildOrtho` (2026-11-04). |
| `sub_825972A8` | `Matrix::Perspective` | | r3 = out, f1 = field of view, f2 = aspect, f3 = near, f4 = far: y scale = 1 / tan(f1 * the constant at `0x82001BE4`), x scale = y scale / aspect, depth = far / (far - near), offset -near * far / (far - near). Calls `tan` (`c-runtime.md`). From `Projector::Build` and `TextureProjection::Fit` (2026-11-04). |
| `sub_82598768` | `TextureProjection::Fit` | Wrapped | Builds the c14 x4 texture projection, fitted to a camera object's view; called three times by `sub_820C5DE8` (returns `820C7174`, `820C7558`, `820C7950`; the third's result copied to c14 at `820C79F8`). Its multiply at `82599644` (stack temporary sp+352 = sp+352 * sp+208) reads back as a camera, w 1 with the eye at the origin, and was the group of cameras refused far from the origin; overwritten at `825996FC`, never drawn. See "The headlight beam far from the origin" in `../frame-interpolation.md`. Also one of the race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). Read in outline (2026-11-04: its calls, loops and arguments; 1,020 instructions): r3 = out, r4 = a camera pass (the record's car view, w0), r5 = a direction (normalized first: (0, 1, 0) for the first two calls, which also give c70 and c74, the light's for the third), f1 and f2 = a height range (the car's height plus and minus constants), f3 = a far cut, f4, f5 constants (the scene's draw's row). It copies the pass's view (+16) and projection (+80), takes its near and far (+388, +384, ordered) with the far cut to f3, builds the perspective again (`Matrix::Perspective` with the pass's +368 and +376 / +380) and its frustum (`Frustum::FromMatrix`, a 256-byte frame record), inverts the view (`Matrix::Inverse`), crosses the frustum's edges with the two height planes (`Plane::SegmentCrossing`, four call sites) and pushes points into a `std::vector` of 16-byte points on its frame (`Vector16::PushBack`; probably the crossings and the corners between the heights), takes their box (`Box::FromPoints`), then `Matrix::Multiply`, `Matrix::Inverse`, two more `Plane::SegmentCrossing` and five more `Matrix::Multiply` into out; the vector's storage freed (`operator delete`) at the end. Its float work not read. Only caller the scene's draw. |
| `sub_82597878` | `Frustum::FromMatrix` | | r3 = a 256-byte frustum, r4 = a matrix (a projection, from `TextureProjection::Fit`): the six planes at +0 to +95 from the sums and differences of the matrix's w column and its x, y, z columns, each scaled by 1 / the length of its x, y, z; +96 to +116 each plane's 3-bit code of its normal's negative components (1 x, 2 y, 4 z; probably for box tests); +128 to +255 the eight corners (w 1.0), corner k from three planes picked by k's bits (`Plane::Intersect3`). Returns the frustum. Only caller `TextureProjection::Fit`. Read in outline (stores, comparisons and calls, 2026-11-04). |
| `sub_825976C8` | `Plane::Intersect3` | | r3 = out, r4, r5, r6 = three planes (x, y, z, d): the point where they meet (the cross products of the normals over their triple product, with the d's at +12), w 1.0; returns 1, or 0 with out unwritten when the triple product is 0, denormal, infinite or NaN (its exponent bits all 0 or all 1). Leaf. Only caller `Frustum::FromMatrix`. Read in full (2026-11-04). |
| `sub_82597E58` | `Plane::SegmentCrossing` (probably) | | r3 = out, r4 = a plane (x, y, z, d), r5 = a point b, r6 = a point a: dir = b − a normalized; when the absolute plane · dir (x, y, z) is below `[0x8206CE48]` returns 0 with out unwritten; else t = −(plane · a + d) / (plane · dir), out = a + dir × t with w 1.0, returns 1 (t not limited to the segment). Leaf. Only caller `TextureProjection::Fit` (six call sites). Read in full (2026-11-04). |
| `sub_82597210` | `Box::FromPoints` | | r3 = a 32-byte box (min at +0, max at +16), r4 = a `std::vector` of 16-byte points: min x, y, z = `[0x8206CE44]` and max = `[0x8206CE40]` (probably the largest float and its negative), both w 1.0; then `Box::Extend` with each point (`Vector16::Size` read again each time). Only caller `TextureProjection::Fit`. Read in full (2026-11-04). |
| `sub_82597150` | `Box::Extend` | | r3 = a box, r4 = a point: per axis (x, y, z) min = the point's unless min is below it, max = the point's unless max is above it (a NaN coordinate goes into min, not max). Leaf. Only caller `Box::FromPoints`. Read in full (2026-11-04). |
| `sub_825971E8` | `Vector16::Size` | | r3 = a `std::vector` of 16-byte elements (+4 first, +8 last): (last − first) / 16 (`srawi`), 0 when first is 0. Leaf. Only caller `Box::FromPoints`. Read in full (2026-11-04). |
| `sub_825986B8` | `Vector16::PushBack` | | `std::vector<16 bytes>::push_back`: r3 = the vector (+4 first, +8 last, +12 end of storage), r4 = the value's address: with storage and the size below the capacity, the 16 bytes stored at last (unless last is 0) and +8 moved on by 16; else `Vector16::Insert(its frame, the vector, last, the value)`. Only caller `TextureProjection::Fit`. Read in full (2026-11-04). |
| `sub_82598648` | `Vector16::Insert` | | `std::vector<16 bytes>::insert` of one: r3 = the iterator out, r4 = the vector, r5 = where, r6 = the value: `Vector16::InsertN(vector, where, 1, value)`, then out = first + 16 × where's index (where − first; 0 for a vector with no storage or no elements). Returns r3. Only caller `Vector16::PushBack`. Read in full (2026-11-04). |
| `sub_82598288` | `Vector16::InsertN` | | `std::vector<16 bytes>::_Insert_n`: r3 = the vector, r4 = where, r5 = n, r6 = the value (copied to the frame first): nothing for n = 0; `_Xlen` (`sub_825981C8`, never ran) when 0x0FFFFFFF minus the size is below n; when the capacity is short, new storage (`Vector16::Allocate`) of the capacity plus half of it (or the size plus n when that's more), the elements and the n copies placed, the old storage freed (`operator delete`); otherwise in place. Only caller `Vector16::Insert`. Not read in detail (its calls and the growth; 239 instructions, 2026-11-04). |
| `sub_82598158` | `Vector16::Allocate` | | `std::allocator<16 bytes>::allocate`: r4 = the count (r3 not read): `operator new(16 × count)` (`heap.md`), returned; when the count isn't 0 and 0xFFFFFFFF / count is below 16, a `std::bad_alloc` built on the stack first (`sub_82091200`, never ran; vtables `0x82000CCC`, then `0x82000CC0`), as the other allocators (`app-states.md`). Only caller `Vector16::InsertN`. Read in full (2026-11-04). |
