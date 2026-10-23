# Known functions: the camera passes and viewports

The cameras under the game's camera modes: the graphics library's camera
class (`Camera`, vtable `0x82001EC0`: slot 0 `Camera::SetViewportSize`,
1 `Camera::SetFieldOfView`, 2 `Camera::Update`, 4 its destructor
`sub_8209FB80`, never run), the game's 21 camera passes built on it (592
bytes each from `0x826BAC00`, made once at boot by `CameraPasses::Init`;
pass 0 the main view; each pass has a type 0-20 that sets its viewport,
planes, clear and render targets, and the 14 render target textures at
`0x826BABC4`-`0x826BABFC` the passes draw into, 13 ids with two for id 1), their enabling, their
command recording at the start and end of a pass (render targets, clear,
viewport, resolves; `command-list.md`), and the two viewports
(`[0x826C2CE8]`, one a player, 1,872 bytes each) the camera modes write:
twenty 80-byte camera entries each (eye, target, up, roll, zoom and far
plane), filled from the viewport's own eye, target and up, and copied into
a camera pass by `CameraPass::FromViewport`. The fields worked out are in
`../known-data.md` (`0x826BAC00`, `0x826BABC0`, `0x826C2CE8`, "viewport",
"viewport entry"). These are the cameras
frame interpolation blends (`../frame-interpolation.md`): the functions
with overrides (Wrapped) keep their facts, and say what wraps them.

Not here: the camera modes above (`82152028`-`82156CC0`, ~41 functions
seen in play, `sub_82156CC0` 4,907 lines; with `sub_8217BE98` and
`sub_822282E8`: a lead), the second camera class made over
`Camera::Camera` at boot (`sub_820A9E28`, from `sub_820A8B88`; a lead),
and below: the matrix math (`matrix-math.md`: `Matrix::Multiply`,
`Copy`, `RotateX`, `RotateY`, `RotateZ`, shared), `atan` and `tan`
(`c-runtime.md`, shared), the command list (`command-list.md`, shared),
`VirtualAlloc`, `VirtualFree` (`heap.md`), `Resource::Release`
(`sub_822A67B0`) and the graphics device's texture and surface calls
(`sub_822A5D08`, `sub_822A5E28`, `sub_822A5B80`, `sub_822A5C50`,
`sub_822A3998`, `sub_822A39E0`, `sub_8258D8C8`, `sub_8258D908`; in
`functions.md` or without rows; a lead), memset (`sub_822962B0`) and
atexit (`sub_825A7068`, over `sub_825A6F60`), neither with rows (a
lead). Two functions
in the viewports' address range aren't cameras: `sub_820AB408` and
`sub_820AB498` convert track positions over the course's route
(`[0x826C5B10]`, from `sub_820F8420`; the route's lead). The rows were
drafted by the B7 loop's mapping agent (2026-10-23) from the generated
code; float constants read from the image (`../known-data.md`, "Values
from the image").

Made native by the B7 loop (2026-10-23, run `b7-loop-23-10-26-3`):
`src/cameras/cameras.cpp` behind `--native_cameras` (on by default;
adapters in `src/hooks/cameras.cpp`), checked by
`--run_harnesses=cameras`. As the matrix math's (`matrix-math.md`), the
vector unit's work is the translation's own SSE steps (word 0 in the top
lane, flush-to-zero on), the scalar work its doubles step for step (built
with `-ffp-model=strict -ffp-contract=off`); the angle wrap into [-π, π]
is the one the turns use. Frame dumps of an attract race with the flag on
and off show the same frame.

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
| `sub_82593908` | `Camera::Camera` | | r3 = a camera: the library camera's constructor. +0 = the vtable `0x82001EC0`; identity matrices at +16 on (the view, the projection at +80, the view-projection at +144 and the others to +364); +368 the field of view (π/4, `0x82001FC4`, wrapped into [-π, π] as `Camera::SetFieldOfView` does), +376 and +380 the viewport's size (640 and 480), +384 the near plane (0.1), +388 the far plane (3,000.0), +392 the aspect (4/3), +288 to +296 and +272 the angles and position (from stack vectors); then `Camera::BuildProjection` and `Camera::Update`. Returns r3. Not read in detail (the matrices' stores). From `CameraPass::Init` (each pass, at boot) and `sub_820A9E28` (another camera class, a lead); `sub_825D9310` (never ran). |
| `sub_82593500` | `Camera::SetViewportSize` | Overridden | Vtable slot 0 (no direct callers; `CameraPass::Update` calls it): r3 = the camera, r4 = width, r5 = height (integers): +376 and +380 = them as floats, +392 = width / height; then `Camera::BuildProjection(camera, +368, +392, +388, +384)`. The integers are zero-extended (`clrldi`) and converted as signed 64-bit, so they're taken as unsigned. Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`); the adapter stays (reached through the vtable). |
| `sub_82593278` | `Camera::SetFieldOfView` | Overridden | Vtable slot 1 (no direct callers; `CameraPasses::SetMode` calls it on pass 4): r3 = the camera, f1 = degrees: +368 = f1 × π/180 (`0x82002948`), wrapped into [-π, π] (π with the angle's sign added, times 1/(2π), truncated, that many 2π taken off, as `Matrix::RotateZ` does). Leaf. Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`); the adapter stays (reached through the vtable). |
| `sub_825932E8` | `Camera::BuildProjection` | Overridden | r3 = the camera, r4 = &field of view, r5 = &aspect, r6 = &far, r7 = &near: the projection at +80 (its rows zeroed first, +112 a constant row): the half angle (field of view × 0.5, wrapped into [-π, π]) through sine and cosine polynomials (the vectors at `0x820009C0`-`0x82000A1F`), its cotangent as y's scale and that over the aspect as x's, z's near / (near − far) and w's −far × that: a reversed depth range (1 at the near plane, 0 at the far one). Leaf. Not read in detail. From `Camera::Camera`, `Camera::SetViewportSize` and `Camera::Update`. Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`): its stack words (r1 - 112 to r1 - 16: the polynomial's (1, x, x², x³) and the dot products, read back with `lvx`) held in registers, the same with a 16-byte aligned stack pointer (the adapter runs the original otherwise); the rows' first stores (zeros and (0, 0, 0, 1)) kept before the inputs are read. `Camera::SetViewportSize`'s plain C++ calls it directly; the adapter stays for `Camera::Camera` and `Camera::Update` (recompiled). |
| `sub_82593568` | `Camera::Update` | | Vtable slot 2 (its direct caller `Camera::Camera`): r3 = the camera: the view from Euler angles (+288, +292, +296 through `Matrix::RotateY`, `RotateX`, `RotateZ`) and the position at +272, kept at +16 (`Matrix::Copy`); +392 = +376 / +380 (the aspect); a vertical angle from the constant at `0x8206CC08` (wrapped, halved, `tan`, over the aspect, `atan`), `Camera::BuildProjection(camera, +368, +392, +388, +384)`, the view-projection +144 = view × projection (`Matrix::Multiply`); +372 = +380 × 0.5 over a sum of the stack's matrix entries (probably a pixel scale). Not read in detail. Probably the library's own update; the game's passes use `CameraPass::Update`. |
| `sub_8209F9B8` | `CameraPasses::Init` | | No arguments; once at boot (from the app's start-up `sub_82106548`): clears 15 words at `0x826BABC0`-`0x826BABFC`: the mode (`0x826BABC0`), `0x826BABE4` and 13 of the 14 render targets (not `0x826BABC4`, id 12's) and runs `CameraPasses::InitPasses` (a tail call). |
| `sub_8209F960` | `CameraPasses::InitPasses` | | From `CameraPasses::Init`: `CameraPass::Init(0x826BAC00 + 592 × i, i, [0x826BABE4])` for i = 0 to 20 (pass i gets type i; r5 isn't read), then `CameraPasses::SetMode(1, [0x826BABC0])`. |
| `sub_8209F738` | `CameraPasses::SetMode` | Overridden | r3 = 1 (nothing otherwise), r4 = a mode: `[0x826BABC0]` = r4; mode 0 sets pass 4's field of view to 80 degrees, mode 1 to 90 (`Camera::SetFieldOfView` through its vtable slot 1; the floats at `0x82001E8C`, `0x82001E88`). From `CameraPasses::InitPasses` (mode 0). Render target 1 and `CameraPasses::EnableForView` follow the mode. Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`): the call through pass 4's vtable slot 1 made to `Camera::SetFieldOfView`'s plain C++ when the slot holds it (always in play; the original runs otherwise). From `CameraPasses::InitPasses` (recompiled), so the adapter stays. |
| `sub_8209F718` | `CameraPasses::Mode` | Overridden | r3 = 1: returns `[0x826BABC0]` (the mode, `CameraPasses::SetMode`); anything else returns -1. Leaf. From `CameraPass::SetUpSurfaces` (6 calls), `CameraPass::Init`, `CameraPasses::EnableForView` and `Viewport::UpdateEntries` (all with 1). Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`); `CameraPasses::EnableForView`'s plain C++ calls it directly; the adapter stays for `CameraPass::SetUpSurfaces`, `CameraPass::Init` and `Viewport::UpdateEntries` (recompiled). |
| `sub_8209F7A8` | `CameraPasses::RenderTarget` | Overridden | Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`), with its case 0 (`sub_8209F7DC`) merged; its adapter is the diagnostics' wrapper in `src/debug/frame_state_diff.cpp` (written out there instead of `ITC_FRAME_STATE_WRAP`), which still names which draw step part advances per-frame state (`--find_per_frame_state`) and skips it with `--debug_hide`. What it does (B7 loop, camera map): r3 = an id 0-12: returns that render target's texture (or 0 above 12): 0 `[0x826BABE8]` (the chunk `sub_8209F7DC`), 1 `[0x826BABEC]` with `[0x826BABC0]` 0 or `[0x826BABCC]` with 1 (0 otherwise), 2 `[0x826BABDC]`, 3 `[0x826BABC8]`, 4 `[0x826BABD4]`, 5 `[0x826BABFC]`, 6 `[0x826BABF8]`, 7 `[0x826BABF4]`, 8 `[0x826BABF0]`, 9 `[0x826BABD0]`, 10 `[0x826BABD8]`, 11 `[0x826BABE0]`, 12 `[0x826BABC4]`. Leaf. From `CameraPass::SetUpSurfaces` (19 calls), `CameraPass::RecordEnd` and its part, `CameraPasses::MakeRenderTarget`, and the UI movies' full-screen effect (`sub_8211BEE8`, `ui-movies-game.md`, with id 0). |
| `sub_8209F7DC` | | Merged | Case 0 of `CameraPasses::RenderTarget` (the recompiler split it off): returns `[0x826BABE8]` (render target 0). Leaf. Merged into `CameraPasses::RenderTarget`'s plain C++ (`src/cameras/cameras.cpp`, harness `cameras`). |
| `sub_8209FBF0` | `CameraPasses::MakeRenderTarget` | | r3 = an id 0-12: makes that render target's texture, releasing the old one first (`CameraPasses::RenderTarget`, `Resource::Release`): with the device's current state saved (`sub_8258D8C8` on the object `[0x8266D088]`'s slot 7 then 3 return) and restored after (`sub_8258D908`), one `sub_822A5D08` (probably CreateTexture: width, height, 1, 1, 1, a format, 0, 3) into its global: id 0 512 × 128 (`[0x826BABE8]`, the chunk `sub_8209FC7C`), then 1024 × 1024, 512 × 512 and 256 × 256 ones (see `CameraPasses::RenderTarget` for the globals). From `CameraPass::SetUpSurfaces` (13 calls). Not read in detail (the formats). |
| `sub_8209FC7C` | | | Case 0 of `CameraPasses::MakeRenderTarget` (split off): `sub_822A5D08(512, 128, 1, 1, 1, 0x182801B6, 0, 3)` into `[0x826BABE8]`, then the device's state restored (`sub_8258D908`) and the original's epilogue. |
| `sub_820A3DB0` | `CameraPass::Init` | | r3 = a camera pass, r4 = its type (0-20; r5 not read): +576 and +580 (its surfaces) = 0, `Camera::Camera`, then by type (a switch; type 0 is the chunk `sub_820A3E04`) the viewport (+496 x, +500 y, +504 width, +508 height; from the display for the screen's types, `[0x8266D088]`'s slot 7 then 3 object's +296 and +300), +512, the zoom +516, +520 to +540, the near and far planes' defaults +544 and +548, +552 to +572 (+564 the clear color, +572 the clear depth), then for all: +384 = +544 (near), +388 = +548 (far), +584 = the type, +588 = 0 (disabled), +376 and +380 = the size as floats, `CameraPass::Update`, `CameraPass::SetUpSurfaces`; returns 0. Once a pass at boot (`CameraPasses::InitPasses`). Not read in detail (types 1-20's values; from `../known-data.md`, pass 10 is a 1024 × 1024 reflection and 11 a 512 × 512 shadow map). |
| `sub_820A3E04` | | | Type 0 of `CameraPass::Init` (split off; r31 = the pass, r30 = 0, r28 = the type): the display's width and height into +504, +508 (x, y 0), zoom +516 = 666.33 (`0x82001FDC`), +520 = +544 = 1.0 (near), +548 = 5,000.0 (far), +524 = 0.733, +528 = 0.0, +532 = 10.0, +536 = +540 = 2,000.0, +564 = `0xFF000000` (opaque black), +560 = +568 = 1, +512, +552, +556, +572 = 0; then the common tail (`CameraPass::Update`, `CameraPass::SetUpSurfaces`). |
| `sub_820A2F40` | `CameraPass::SetUpSurfaces` | | r3 = a camera pass: `CameraPass::ReleaseSurfaces`, then by type (+584, a switch): types 0-2 the device's own (the chunk `sub_820A2F94`); the others their render target's texture (`CameraPasses::RenderTarget`, made with `CameraPasses::MakeRenderTarget` when missing, by `CameraPasses::Mode` for some) and its surfaces (`sub_822A5B80`, `sub_822A5E28`, `sub_822A5C50`: probably a texture's surface level, a render target and a depth surface) into +576 and +580, with the device's state saved and restored (`sub_8258D8C8`, `sub_8258D908`). From `CameraPass::Init` and its type 0 chunk. Not read in detail. |
| `sub_820A2F94` | | | Types 0-2 of `CameraPass::SetUpSurfaces` (split off): with the device's state saved (`sub_8258D8C8`), `sub_822A3998(state, 0)` and `sub_822A39E0(state)` (probably its render target 0 and depth surface, referenced) into +576 and +580, then restored (`sub_8258D908`). Not read in detail. |
| `sub_820A0C40` | `CameraPass::ReleaseSurfaces` | Overridden | r3 = a camera pass: for types 3, 4 and 10-20 (+584), releases +576 and +580 when set (`Resource::Release`) and clears them. From `CameraPass::SetUpSurfaces` (`sub_820A2F38` never ran). Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`): the type check and the stores; `Resource::Release` (Overridden, `src/native/refcount.cpp`, which still makes guest calls) is called by the adapter as a guest call from a frame like the original's, so the adapter stays (it also stays for `CameraPass::SetUpSurfaces`, recompiled). |
| `sub_820A3978` | `CameraPass::Update` | Wrapped | Rebuilds a camera pass's view (+0x10) from its eye, target and up vector (+0x190, +0x1A0, +0x1B0), and its view-projection (+0x90 = view * projection at +0x50). Called several times per Draw for pass 0. Wrapped for `--log_camera_pass_records`. Read in part (B7 loop, camera map): r3 = the pass; types 0-2 (+584) first record the viewport (`CameraPass::RecordViewport`, r4 = 150 for types 0 and 1, 371 for type 2); then the vtable's slot 0 (`Camera::SetViewportSize` with +504, +508); type 10 skips part of the rest; the view turned by the roll at +448 (`Matrix::RotateZ`), with `Matrix::Copy` and `Matrix::Multiply`. From `CameraPass::Init`, its type 0 chunk and `CameraPass::FromViewport`. The wrapper is in `src/frame_pacing/per_frame_logic.cpp`. |
| `sub_820A0978` | `CameraPass::RecordViewport` | | r3 = a camera pass, r4 = a sort key: on the renderer's command list (`[0x826BAB60]`'s slot 8 then 1), a chunk (`CommandList::OpenChunk`, `AllocateSlots`, `CloseChunk`) with a header (5), command 30 (1) and the viewport (+496, +500, +504, +508 as halfwords, depths 0.0 to 1.0), then command 36; the chunk handed to the render queue (slot 8 then 2, its slot 4) with r4. From `CameraPass::Update` only. Not read in detail (the commands' meanings, `command-list.md`). |
| `sub_820A0958` | `CameraPass::Disable` | Overridden | Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`); its adapter is the diagnostics' wrapper in `src/debug/frame_state_diff.cpp` (written out there instead of `ITC_FRAME_STATE_WRAP`), which still names which draw step part advances per-frame state (`--find_per_frame_state`) and skips it with `--debug_hide`. What it does (B7 loop, camera map): r3 = a camera pass: +588 = 0. Leaf. From `sub_820C5DE8` and `Race::DrawStep` (`sub_8215B4F8`). |
| `sub_820A0968` | `CameraPass::Enable` | Overridden | r3 = a camera pass: +588 = 1 (the passes `CameraPasses::RecordBegins` and `RecordEnds` record). Leaf. From `CameraPasses::EnableForView` (21 calls), `sub_8217BE98` and `sub_822282E8`. Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`); `CameraPasses::EnableForView`'s plain C++ calls it directly; the adapter stays for `sub_8217BE98` and `sub_822282E8` (recompiled). |
| `sub_820AA1A8` | `CameraPasses::EnableForView` | Overridden | Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`) (r3 left as the last pass enabled, 4 or 9, as the original's); its adapter is the diagnostics' wrapper in `src/debug/frame_state_diff.cpp` (written out there instead of `ITC_FRAME_STATE_WRAP`), which still names which draw step part advances per-frame state (`--find_per_frame_state`) and skips it with `--debug_hide`. What it does (B7 loop, camera map): no arguments: enables the passes the view needs (`CameraPass::Enable`): with one viewport (`[0x826C2CE8]` +8 = 0) passes 0, 10, 11, 12, 19 and 20, and pass 3 when viewport 0's view mode (+1860) is 1 or 2; in split screen (+8 not 0) passes 1, 2, 13 to 18 and 20 instead; then pass 4, and passes 5 to 9 too when `CameraPasses::Mode(1)` is 1 (probably a cube map's six faces: mode 1 sets pass 4's field of view to 90 degrees, `CameraPasses::SetMode`). From `sub_820C5DE8`, `Race::DrawStep` (`sub_8215B4F8`) and `sub_82227A50`. |
| `sub_8209F8A0` | `CameraPasses::RecordBegins` | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). What it does (B7 loop, camera map): no arguments: `CameraPass::RecordBegin` for each enabled pass (+588) of the 21. From `sub_820C5DE8`, `Race::DrawStep` and `sub_822282E8`. |
| `sub_8209F900` | `CameraPasses::RecordEnds` | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). What it does (B7 loop, camera map): no arguments: `CameraPass::RecordEnd` for each enabled pass (+588) of the 21. From `sub_820C5DE8`, `Race::DrawStep` and `sub_822282E8`. |
| `sub_820A0CC0` | `CameraPass::RecordBegin` | | r3 = a camera pass: for types 3-20 (+584, a switch; part in the chunk `sub_820A0D38`), on the renderer's command list a chunk with the pass's render targets (command 23 with four slots of +576, command 24 with +580), a clear (command 26 in a 280-byte item: flags 49 or another by type, the color +564, the depth +572 as a float) and command 36, handed to the render queue with a key (55, 49 and others by type). Not read in detail (1,092 lines). From `CameraPasses::RecordBegins`. |
| `sub_820A0D38` | | | Part of `CameraPass::RecordBegin` (split off): the same chunk (render targets, depth, clear with flags 49, command 36) handed on with key 49. Not read in detail. |
| `sub_820A1C30` | `CameraPass::RecordEnd` | | r3 = a camera pass: for types 3-20 (a switch; part in the chunk `sub_820A1CA8`), a chunk with resolves (`CommandList::AllocateResolve`, command 25) into the pass's render target textures (`CameraPasses::RenderTarget`, 11 calls) and command 36, handed to the render queue. Not read in detail (1,266 lines). From `CameraPasses::RecordEnds`. |
| `sub_820A1CA8` | | | Part of `CameraPass::RecordEnd` (split off): one resolve chunk into a render target (`CameraPasses::RenderTarget`). Not read in detail. |
| `sub_8209FA38` | `RecordClear` | | r4 = a sort key, r5 = flags, r6 = a color, r7 = a depth (an integer, stored as a float): a chunk on the renderer's command list with a clear (command 26 in a 280-byte item, as `CameraPass::RecordBegin`'s) and command 36, handed to the render queue with r4 (f1 0.0). From `sub_820F6068` (a race draw step part) and `sub_8215AA88`. |
| `sub_820AB358` | `Viewports::Init` | | No arguments: `[0x826C2CE8]` = a 12-byte block (`VirtualAlloc(0, 12, 0x1000, 4)`, cleared with memset `sub_822962B0`), its +0 and +4 two 1,872-byte viewports (the same, cleared), each set up by `Viewport::Init`; +8 = 0 (one viewport in use). From `sub_82152338` (a camera mode), `sub_82180B00` and `sub_82227A50` (not read). |
| `sub_820AA430` | `Viewports::Release` | Overridden | No arguments: frees `[0x826C2CE8]`'s two viewports and the block (`VirtualFree(address, 0, 0x8000)`) and clears `[0x826C2CE8]`. From `sub_82153098`, `sub_82180738` and `sub_82228548`. Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`) while `--native_heap` routes `VirtualFree` to the allocation layer (`memory::FreePages`; the original where a block isn't the layer's). Not seen in an attract race's first 25 seconds (it runs when a race ends). |
| `sub_820AAFF0` | `Viewport::Init` | | r3 = a viewport: `Viewport::SetZoom(the display's width × tan(1.0908) × 0.5)` (the double at `0x82002818`, `tan`; the width from `[0x8266D088]`'s slot 7 then 3 object's +296), `Viewport::SetRoll(0.0)`; camera pass 0's near plane (+384) = 1.0; +1860 (the view mode) = 0; each entry's far plane (+72) from a pass's (+388): entry 0 from pass 0, entries 1-16 from passes 3-18, 17 = 10,000.0, 18 and 19 from passes 19 and 20; each entry's +0 = 0; three vectors made once (guard bits 1, 2, 4 in `0x826C2D20`, each with an atexit, `sub_825A7068`): `0x826C2D10` (0, 0, 0, 1), `0x826C2D00` (10, 0, 0, 1), `0x826C2CF0` (0, 1, 0, 1), copied to +1680, +1696, +1712; +1728 to +1839 seven (0, 0, 0, 1); +1856 = 8,000.0, +1864 = entry 0's zoom, +1868 = pass 0's near plane, entry 18's zoom (+1508) = 4.096 and far plane = 8,000.0. From `Viewports::Init`. |
| `sub_820AAE90` | `Viewport::SetZoom` | Overridden | r3 = a viewport, f1 = a zoom: the entries' +68: entries 0, 1, 8-17 and 19 = f1; 2-7 = atan(256 × 0.5) × 2/π (`0x82001EF0`, `0x82002808`; ≈ 0.995); 18 = 32,768 / +1856. 14 callers seen: the camera modes (`sub_82152338` twice, `sub_82153640`, `sub_82153A50`, `sub_82153E58`, `sub_82154548`, `sub_82154A48`, `sub_82154D38`, `sub_82155A28`, `sub_82156CC0`), `sub_8217BE98`, `sub_822282E8` and `Viewport::Init` (`sub_82155288`, `sub_821562F8` never ran). The integer at `0x82001EF0` (256) is read as a word and converted. Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`), `atan` the C runtime's plain C++. |
| `sub_820AA2D0` | `Viewport::SetRoll` | Overridden | r3 = a viewport, f1 = an angle: wrapped into [-π, π] (π added, 2π taken off while above 2π, added while below 0.0, π taken off), stored as the entries' roll (+64) for entries 0 and 2-17 and 19, negated for entry 1; entry 18 left as it was. Leaf. From the camera modes (`sub_821524F0` twice, `sub_82153640` to `sub_82155A28`), `sub_8217BE98`, `sub_822282E8` and `Viewport::Init`. An infinite or huge angle never leaves the first loop (the game's are numbers). Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`). |
| `sub_820AA368` | `CameraPasses::SetNear` | Overridden | f1 = a distance: camera pass 0's near plane (`0x826BAD80`, +384) = f1. Leaf. From the camera modes (`sub_82152338` twice, `sub_82153640` to `sub_82155A28`, `sub_82156CC0`), `sub_8217BE98` and `sub_822282E8` (`sub_82155288`, `sub_821562F8` never ran). Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`). |
| `sub_820AA378` | `CameraPass::SetZoom` | Overridden | r3 = a camera pass, f1 = a zoom (probably a focal length in pixels): +516 = f1; +368 (the field of view) = atan(height (+508) × 0.5 / f1), wrapped into [-π, π] as `Camera::SetFieldOfView` does. From `CameraPass::FromViewport` only. The height is read as a signed integer (`lwa`) and the ratio divided by f1 as it came (a double). Native in `src/cameras/cameras.cpp` (`--native_cameras`, harness `cameras`), `atan` the C runtime's plain C++; the adapter stays for `CameraPass::FromViewport` (recompiled). |
| `sub_820AA4A8` | `CameraPass::FromViewport` | | r3 = a camera pass, r4 = a player (viewport `[[0x826C2CE8] + 4 × r4]`), r5 = an entry (80 × r5 into it): the entry's eye, target and up (+16 to +63) into the pass's +400 to +447 (8 bytes at a time), `CameraPass::SetZoom(pass, entry +68)`, +388 (far) = entry +72, +448 (roll) = entry +64, then `CameraPass::Update`. From `sub_820F34C8`, `sub_820F4558` and `sub_8217BE98`. |
| `sub_820AA550` | `Viewport::UpdateEntries` | | r3 = a viewport: entry 0's eye, target and up (+16 to +63) kept at +1760 to +1807, then the 20 entries' vectors set from the viewport's eye (+1600), target (+1616; +1664 in view mode 1, +1860), up (+1632) and fourth vector (+1648), entry 1's by `CameraPasses::Mode(1)`, others with offsets and cross products (vector math). Not read in detail (1,022 lines). From `Viewport::Update`, `Viewport::Set` and `Viewport::LookAt`. |
| `sub_820AAE40` | `Viewport::Update` | | r3 = a viewport: `Viewport::UpdateEntries` (a tail call). From `sub_82152338` (twice) and the rival intro's update (`sub_821594B8`, twice). |
| `sub_820AAE48` | `Viewport::Set` | | r3 = a viewport, r4 to r7 = four vectors: copied to +1600, +1616, +1632, +1648 (the eye, target, up and fourth vector), then `Viewport::UpdateEntries` (a tail call). From `sub_822282E8` only. |
| `sub_820AAF68` | `Viewport::LookAt` | | r3 = a viewport, r4 = the eye, r5 = the target: +1600 = the eye, +1616 and +1648 = the target, +1632 = (0, 1, 0, 1), then `Viewport::UpdateEntries`. From `sub_8217BE98` only. |
