# Known functions: matrix math

The game's matrix, vector and quaternion math: two libraries of plain
math functions with no state of their own. The first, around
`8209B000`-`820AC000` beside the camera passes and the billboards, has
the 4x4 matrix copy, multiply and inverse, the turns about x, y and z,
and a vector transformed by a matrix; the cameras, the cars' drawing,
the cockpit dials, the race overlays, the UI movies and the graphics
library call it (the first three are wrapped for frame interpolation:
see "Interpolation" in `../frame-interpolation.md` and the rows). The
second is the 4x4 matrix and quaternion half of the physics' vector
library (`82293598`-`82293F10`, with the two functions it reaches at
`822B8F78` and `822B9158` and the sine and cosine pair `820B0E10`); its
3D vector half and three matrix functions (`Matrix::SetTranslation`
`sub_82293818`, `Matrix::Rotate` `sub_82293880`,
`Matrix::TransformPoint` `sub_822938F0`) are in
[`vehicle-accessors.md`](vehicle-accessors.md) (shared, already
native behind `--native_vector_library`), as is `sqrtf`
(`sub_82293590`) and `Vec4::Copy` (`sub_822940E0`). `sin` and `cos`
(`sub_825A88A0`, `sub_825A8978`) are in [`c-runtime.md`](c-runtime.md).
Matrices are 4x4 floats, row-major, used with row vectors (v * M: the
fourth row is the translation); quaternions are (x, y, z, w).
Constants: the float at `0x82000A30` is 1.0 and `0x82000A34` probably
0.0, `0x82001BE4` probably 0.5; probably the identity at `0x82953740`
(`../known-data.md`; none of these read from guest memory here). Mapped by the B7 loop (`../loops/b7-loop.md`),
2026-10-22, from the generated code only (no runs): call counts aren't
known. Plain C++ already doing some of this: `Multiply` and `RotationZ`
in `src/hooks/cockpit_dials.cpp` (`RotationZ` with `std::cos` and
`std::sin`, not the game's polynomial) and `interpolation::Multiply`
and `Inverse` in `src/interpolation/transform.h`. Native versions of
the vector-unit functions (`vmsum4fp128`, `vrefp`) must give the
translation's results (the dot products as `dpps` does them, as
`cockpit_dials.cpp`'s `Multiply` does): check them with a harness.
Left out: the camera code beside the first library (`sub_820A3978`
`CameraPass::Update` in `functions.md`, the camera objects at
`820AA2D0`-`820AB558`) and the other functions in that range (files,
saves, billboards, the graphics library's objects): not math.

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
| `sub_8209BD50` | `Matrix::Multiply` | Wrapped | out (r3) = a (r4) * b (r5), 4x4. Interpolation notes every multiply into a camera (see "Interpolation" in `../frame-interpolation.md`). Both inputs are copied to the stack first (8 bytes at a time, any alignment), so out may be a or b; each element a row of a dotted with a column of b (`vmsum4fp128`), out stored aligned (`stvx`). Leaf. 59 callers (45 seen): the cameras (`CameraPass::Update`, `Projector::Build`, `sub_820C5DE8`), the cars' and scene models' drawing (`820D0000`-`820F6000`), the cockpit dials, the billboards (`sub_8209C1A0`, `sprites.md`), the sprite recorders (`sub_82128750`, `sub_821281A8`), the per-object recorder `sub_825953D0`, `UiMovie::ApplyTransform` (`ui-movies.md`), the turns below. The wrapper (`src/interpolation/blend_state.cpp`) calls the original, then `interpolation::GetDrawState().OnMultiply(base, out, a, b, caller)` with `caller` the call's guest return address (`0x82595438`, the per-object recorder's, is told apart by it; only main-thread calls are noted): a native version keeps that call, and native callers pass the return address of the call they replace. `src/hooks/cockpit_dials.cpp` calls `__imp__sub_8209BD50` on purpose (not noted) and has a plain C++ `Multiply` with the same results (`_mm_dp_ps`). Moved from `functions.md`. |
| `sub_820A0AC0` | `Matrix::Copy` | Wrapped | r3 = destination, r4 = source, 4x4. Interpolation follows copies of cameras and products. Copies the 16 floats one at a time as floats (`lfs`/`stfs`, any alignment). Leaf. 52 callers (38 seen): the cameras, the cars' drawing, `View::UploadPreviousViewProjection` (`sub_821363F0`), the UI movies' draw contexts (`sub_8259D9F8`, `ui-movies.md`) and others. The wrapper (`src/interpolation/blend_state.cpp`): for the copy returning to `0x8213653C` (the motion blur's previous view-projection, r29 the view) the source noted is `interpolation::BlendState_MotionBlurNotedSource(base, r29, src)` (and logged with `--log_motion_blur_velocity`); after the copy, `interpolation::GetDrawState().OnCopy(base, dst, noted_src, caller)` (main thread only). A native version keeps both, with the replaced call's return address as `caller`. Moved from `functions.md`. |
| `sub_820A2D30` | `Matrix::RotateZ` | | r3 = out, r4 = in, r5 = &angle: out = in * RotZ, rows `(c, s, 0, 0)`, `(-s, c, 0, 0)`, sine and cosine from a polynomial. The angle is first brought into [-π, π] (adds π with its sign, times the float at `0x82001FA4`, probably 1/(2π), truncated, that many times the float at `0x82001FA8`, probably 2π, taken off; π built from its bits), then sine and cosine from polynomials in (1, x, x², x³) with coefficients at `0x820009C0`-`0x82000A1F` (`vmsum4fp128`); the turn is built on the stack and multiplied with `Matrix::Multiply` (`sub_8209BD50`, so it's noted as any multiply). 17 callers (10 seen): `CameraPass::Update`, `Car::TickUpdate` (`sub_8215C780`), `UiMovie::ApplyTransform` (`ui-movies.md`), the cockpit dials (native in `src/hooks/cockpit_dials.cpp`, which builds the turn with `std::cos` and `std::sin` instead) and others. Moved from `functions.md`. |
| `sub_820A65F0` | `Matrix::RotateX` | | As `Matrix::RotateZ` (`sub_820A2D30`, the same code and constants) about x: out = in * RotX, rows `(1, 0, 0, 0)`, `(0, c, s, 0)`, `(0, -s, c, 0)`, `(0, 0, 0, 1)`. Calls `sub_8209BD50`. 19 callers (10 seen): the cars' drawing (`820E0998`-`820EF660`), `Car::TickUpdate`, `sub_82150590`, `sub_8217CAB8`, `sub_82228588`, `sub_82593568`, `sub_82599C58`. |
| `sub_820A67F8` | `Matrix::RotateY` | | As `Matrix::RotateZ` about y: out = in * RotY, rows `(c, 0, -s, 0)`, `(0, 1, 0, 0)`, `(s, 0, c, 0)`, `(0, 0, 0, 1)`. Calls `sub_8209BD50`. 34 callers (21 seen): the cars' and scene models' drawing (`820D02D0`-`820F5740`), `Car::TickUpdate`, `sub_82148058`, `sub_82150590`, `sub_82153A50`, `sub_8217CAB8`, `sub_82228588`, `sub_82593568`, `sub_82599C58` and others. |
| `sub_820A9988` | `Matrix::Inverse` | | r3 = out (stored aligned), r4 = in (copied to the stack first, any alignment): the general 4x4 inverse by cofactors (vector unit), each divided by the determinant: its reciprocal from `vrefp` refined by two Newton steps, the estimate kept when the refined one is NaN. No check for a singular matrix. Leaf. 10 callers (8 seen): `sub_820D02D0`, `sub_820D1D70`, `sub_820D1EB8`, `sub_82136FA8`, `sub_8217CAB8`, `sub_82228588`, `TextureProjection::Fit` (`sub_82598768`), `sub_820A9BC8`. |
| `sub_820A9BC8` | | | r3 = an object: its +560 = `Matrix::Inverse` of its +208 (tail call). No direct callers: reached through a pointer (probably a vtable; whose isn't known). |
| `sub_820AB560` | `Vec4::Transform` | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). What it does: r3 = out (stored aligned), r4 = a 4x4 matrix (copied to the stack first, any alignment), r5 = a 16-byte vector (aligned): out = v * M, all four components (w included as given; each a dot product of v with a column, `vmsum4fp128`). Leaf. 39 callers (27 seen): the scene models' and cars' drawing (`sub_820C5DE8`, `sub_820C9170`, `820D1EB8`-`820DAC68`, `sub_820F34C8`), the race overlays (`CarOverlays::UpdateModelSprites` `sub_82123E48`, `UpdateSparks` `sub_82124728`, `UpdateLightSprites` `sub_821251C0`, `UpdateParticlesB` `sub_821252D0`, `UpdateParticlesC` `sub_82125798`: six groups in [`race-overlays.md`](race-overlays.md) wait on it, by `b7_groups.py`), the UI movies (`UiMovie::DrawPicture` `sub_8259ED40` and `DrawPrimitive` `sub_8259F348` in `ui-movies.md`, `UiMovie::DrawTexts` `sub_8211CD90` and `sub_8211BEE8` in `ui-movies-game.md`), `sub_82136FA8`, `sub_82152E78`-`sub_82155E58`, `sub_8217BE98`. A native version keeps the wrap's calls around it (skipped when `debug::IsHidden(0x820AB560)`, `debug::FrameStateDiff_Before` and `_After(base, "draw step part", 0x820AB560)`) while the wrap exists. Moved from `functions.md`. |
| `sub_820AB688` | `Vec3::SwapXZ` | | r3 = a vector: swaps its floats at +0 and +8. Leaf. 2 callers (1 seen): `sub_82155800` (`sub_820AC248` never ran). |
| `sub_8209B038` | | | r3: the floats at +12, +28, +44 and +60 = 1.0 (`0x82000A30`): the w of four 16-byte vectors (or a matrix's fourth column). Leaf. No direct callers: reached through a pointer (probably a vtable; whose isn't known). |
| `sub_82293598` | `Matrix::MultiplyTo` | | Tail-calls `sub_822B8F78` (r3 = out, r4 = a, r5 = b: out = a * b). From `sub_82293988` only. |
| `sub_822B8F78` | `Matrix::MultiplyUnaligned` | | r3 = out, r4 = a, r5 = b, any alignment (loaded with `lvsl`/`vperm`, stored a word at a time with `stvewx`): out = a * b, each element a row of a dotted with a column of b (`vmsum4fp128`, as `Matrix::Multiply`); returns r3. Both inputs are read before out is written. Saves and restores registers with `sub_825A6E88`. From `sub_82293598` only. Not wrapped: interpolation doesn't see these multiplies. |
| `sub_82293988` | `Matrix::MultiplyReversed` | | r3 = out, r4, r5: out = r5 * r4 (swaps them for `sub_82293598`). From `sub_820B6F38` (the car physics, a lead in `../loops/leads.md`). |
| `sub_822935A0` | `Matrix::Set` | | r3 = out, r4 = a 4x4 matrix: copies its four rows with `Vec4::Copy` (`sub_822940E0`, as floats). From `sub_822936E0` only. |
| `sub_822936E0` | `Matrix::SetIdentity` | | r3 = out: `sub_822935A0(r3, 0x82953740)`, probably the identity (its name from what its callers do with it; not read from guest memory, `../known-data.md`). 7 callers (7 seen): the car physics (`sub_820AFE90`, `sub_820B0EE8`, `sub_820B6F38`, `sub_820B77D0`, `sub_820BB580`), `sub_82293DF0`, `sub_825DA050`. |
| `sub_822936D8` | `Matrix::Transpose` | | Tail-calls `sub_822B9158` (r3 = out, r4 = in). From `sub_82293998` only. |
| `sub_822B9158` | `Matrix::TransposeUnaligned` | | r3 = out, r4 = in, any alignment (`lvsl`/`vperm` in, `stvewx` out): out = the transpose of in (read before out is written); returns r3. Saves and restores registers with `sub_825A6E84`. From `sub_822936D8` only. |
| `sub_82293600` | `Matrix::ProjectPoint` | | r3 = out, r4 = a 4x4 matrix, r5 = a point (x, y, z): (x × row 0 + y × row 1 + z × row 2 + row 3) divided by its w (one division, 1.0 / w, then three products), out w 1.0. out may be r5 (read first). Saves f26-f31 with `sub_825A7910`/`sub_825A795C`. From `sub_82293DF0` only. |
| `sub_82293998` | `Matrix::InverseRotate` | | r3 = out, r4 = a 4x4 matrix, r5 = a vector: the matrix transposed on the stack (`sub_822936D8`), then out = (x, y, z) × its upper 3x3 (so out.x = row 0 · v and so on: the inverse turn for a pure rotation), out w 0.0 (`0x82000A34`). 3 callers (3 seen): the car physics (`sub_820B2CC0`, `sub_820B4420`, `sub_820B7500`). |
| `sub_822936F0` | `Matrix::FromQuaternion` | | r3 = out, r4 = a quaternion (x, y, z, w): s = the float at `0x820021E8` (probably 2.0) / `Quaternion::LengthSquared` (`sub_82293CB0`), then the upper 3x3: `(1 - s(y² + z²), s(xy + wz), s(xz - wy))`, `(s(xy - wz), 1 - s(x² + z²), s(yz + wx))`, `(s(xz + wy), s(yz - wx), 1 - s(x² + y²))` (D3D's rotation from a quaternion). Writes +0-+8, +16-+24, +32-+40 only: the fourth column and row are left as they were. 2 callers (2 seen): `sub_820B6F38` (the car physics), `sub_822937D8`. |
| `sub_822937D8` | `Matrix::FromQuaternionAndRow` | | r3 = out, r4 = a quaternion: `sub_822936F0`, then the fourth row (+48) copied from `0x826382D0` (`Vec4::Copy`; probably (0, 0, 0, 1)). The fourth column is left as it was (its caller starts from the identity). From `sub_82293DF0` only. |
| `sub_82293AD8` | `Quaternion::FromMatrix` | | r3 = out, r4 = a 4x4 matrix (its 3x3): when the trace (+0 + +20 + +40) is above 0.0: s = `sqrtf`(trace + 1.0), w = s × 0.5 (`0x82001BE4`), and with t = 0.5 / s: x = (m12 - m21) t, y = (m20 - m02) t, z = (m01 - m10) t. Otherwise the largest diagonal element i (j, k the next two, from a table {1, 2, 0} on the stack): s = `sqrtf`(m_ii - (m_jj + m_kk) + 1.0), q_i = s × 0.5, t = 0.5 / s (left 0.0 when s is 0.0), w = (m_jk - m_kj) t, q_j = (m_ij + m_ji) t, q_k = (m_ik + m_ki) t. Calls `sqrtf` (`sub_82293590`); saves registers with `sub_825A6E7C`. 2 callers (2 seen): `sub_820AFE90`, `sub_820C3608` (the car physics and its set-up). |
| `sub_82293CB0` | `Quaternion::LengthSquared` | | r3 = a quaternion: x² + y² + z² + w² in f1. Leaf. From `sub_822936F0` only. |
| `sub_82293D78` | `Quaternion::Multiply` | | r3 = out, r4 = a, r5 = b: the Hamilton product a b (x = aw bx + ax bw + ay bz - az by, ..., w = aw bw - ax bx - ay by - az bz; `fmadds`/`fnmsubs` in that order). All inputs read before out is written. Leaf. From `sub_820B7658` (the car physics) only. |
| `sub_82293DF0` | `Quaternion::XAxis` | | r3 = out, r4 = a quaternion: out = (1, 0, 0, 1), a matrix on the stack set to the identity (`sub_822936E0`) and to the quaternion's turn (`sub_822937D8`), then out = `Matrix::ProjectPoint`(that matrix, out): the x axis turned by the quaternion. 2 callers (2 seen): `sub_820C23E8`, `sub_82166658`. |
| `sub_82293E68` | `Quaternion::FromAxisAngle` | | r3 = out, r4 = an axis, f1 = an angle: (0, 0, 0, 1) when the angle is exactly 0.0; otherwise `SinCos`(angle × 0.5) (`sub_820B0E10`, the float at `0x82001BE4`) and out = (axis.x s, axis.y s, axis.z s, c). The axis isn't normalized here. 4 callers (4 seen): the car physics (`sub_820AF1C0`, `sub_820B0EE8`, `sub_820B1B00`, `sub_820B7658`). |
| `sub_82293F10` | `Quaternion::NormalizeSelf` | | r3 = a quaternion: all four floats × 1.0 / `fsqrts`(x² + y² + z² + w²) (no check for 0.0). Leaf. From `sub_820B71E8` (the car physics) only. |
| `sub_820B0E10` | `SinCos` | | f1 = an angle, r4 = &sine, r5 = &cosine: `sin` (`sub_825A88A0`) and `cos` (`sub_825A8978`, both in `c-runtime.md`) of it, each rounded to a float. 2 callers (2 seen): `sub_820B1B00` (the car physics), `sub_82293E68`. In the car physics' address range, but math. |
