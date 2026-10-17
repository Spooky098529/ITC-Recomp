# Cockpit tachometer needle: handover

Branch `cockpit-anim-fix` (from `main` at `0c78a01`, v0.4.4), pushed to
`origin`. Not merged. Started 2026-10-17. Read CLAUDE.md first (rules:
logs before guesses, commit and push often, the user drives the races).

## The problem

In cockpit view (the 3D dashboard, not the HUD), the tachometer needle
looks wrong with frame interpolation on. The user's words, in order:

- "the tachometer jumps when braking sharply"
- At the original 30 fps (`./run.sh --no-interpolation`, i.e.
  `--interpolate_draw_state=false --max_fps=30`) it also jumps: "It
  jumps of course, since it's a tachometer". The revs really do drop
  that fast; the jump itself is the game's.
- What's wrong with interpolation on: "the interpolated jumping makes the
  needle look like it's moving out of position", "it still seems to move
  off its center position". The user suspects the interpolation isn't
  precise enough.

So the goal is: during a big swing (hard braking), the needle must stay on
its spindle in the in-between frames. It doesn't need to stop jumping.

Attract races show no cockpit, so every test needs the user to drive:
start a race, switch to cockpit view, rev and brake hard a few times,
quit after ~30 s. Launch the game in the background without a timeout
(the user objected to runs closing under `timeout` while they played).

## Where the needle is drawn

A frame dump in cockpit view (`--native_renderer_dump=<dir>
--native_renderer_dump_every=5`, then `tools/pfm_to_png.py`) shows the
tachometer in the middle of three dials at about (640, 560-610) in a
1280x720 frame. `draw_ids.pfm` there:

- The needle's pixels (640, 560-575) are **draw 2282**: vs
  `113604C3002A6A21`, ps `BDEC67D7B4DF91A1`, 6,323 triangles, vertex
  fetch slot 93 at `192F5F0C` (the same vertex buffer as draws 2280 and
  2281). The same vertex shader also draws 2288 and 2289 (~1,400
  triangles each). So the needle is part of a big cockpit mesh, not a
  separate model.
- Draw 2346 (vs `B357A047F0BB9E48`, ps `4F4860F071E25904`, 228
  triangles, blended) covers the whole cluster afterwards: probably the
  dial glass.

Draw numbers change between frames; match by shader hashes and vertex
buffer. A `pfm` reader for `draw_ids` is a few lines of Python (rows are
stored bottom-up, three floats a pixel).

## Game code found

- `sub_820DAC68` (per car, from Draw, r3 = car): the car's draw. For the
  player's car in cockpit view (`+32` == 2 and `+4300` set) it calls
  `sub_820DC498` → `sub_820DC350` → `sub_820DBD38` (r3 = the cockpit
  object, r4 = car + 4288) instead of the outside model. Its only per-call
  state change is `+4992 -= 1.0`, harmless (see "Per-frame logic" in
  `functions.md`).
- `sub_820DBD38`, the cockpit's draw: draws the cockpit's pieces with
  `sub_820D06F0(piece, camera = 0x826BAC00 + 144, world = car + 4304)`
  (pieces at cockpit +3120, +1200, +720, +1680, +1920, via
  `sub_820DB8C8` +240/+480/+0, `sub_820DBB88` +1200/+1440/+1680 of
  +2160) and two gauge objects:
  - `sub_820DB980` (gauge object at cockpit +720, model at +244) and
    `sub_820DBA38` (cockpit +1440, model at +4). Each calls
    `sub_820D1B38(model, [obj + 468 or + 228], f1 = [car + 4916])`, sets
    the model's `+440` to 1 around its draw and draws it.
  - `[car + 4916]` stayed **0.1 for a whole drive**: it isn't the needle.
- `sub_820D1B38` (many callers; a general helper): writes f1 to `+12` of
  every material entry of type 1002 on one mesh. Materials: model `+40`
  is an array of 544-byte meshes; mesh `+396` points to a material set
  (count at +8 for pass 0, +12 for pass 1; 512-byte entries from +16;
  `sub_82594AD8` counts, `sub_82594B20` returns one); entry `+392` is the
  type.
- `sub_820D06F0` → `sub_820D04E8(model, camera, world, mesh, ...)`, the
  model draw: per-object constants (`sub_825953D0`, vertex c0 x9 = world
  * camera, the world matrix, the eye), lighting (`sub_82595490`, pixel
  c0 x63), then per material pass `sub_820CECA0`, then submits.
- `sub_820CECA0` (r3 = model, r4 = mesh, r5 = material pass): per
  material, picks shaders by type (`sub_825896D8` vertex, `sub_82589700`
  pixel; with the model's `+440` set, type-0 materials use vertex shader
  1005) and records two constant uploads (the only two in it):
  - vertex **c9 x5** (header `0x00010905`): c9-c12 from the model's vec4
    array at `[model + 320]` by the indices at material +208 (count +128,
    up to 4; -1 gives (0, 0, 0, 1)), w from material +240; c13 = the
    material's first 16 bytes (+12 is what `sub_820D1B38` writes).
  - pixel **c63 x5** (header `0x01013F05`).
- `sub_820D0D60` (listed under "Fixed time step" as using 1/60): it's
  HSV to RGB (60 degrees of hue), not a time step.

## What was ruled out, with the evidence

Each was a guess tested by a drive; the logs are in
`itc_recomp/out/build/linux-amd64-release/logs/` (`itc_926` to `itc_937`).

1. **Per-frame logic (the needle advancing on render-only passes).**
   `--log_cockpit_changes` (itc_928): nothing in the cockpit object or the
   gauge models changes on render-only passes, only the car's `+4992`
   counter. The 30 fps comparison agrees: the jump is the game's.
2. **The blended world matrix: needle mistaken for a wheel.**
   `--log_moving_products` spinning summary (itc_929): only wheels
   (caller `0x820D0564`) are ever "spinning".
3. **The blended world matrix: needle turning off its pivot.**
   `--blend_about_pivot` was written for this (see below), but
   `--log_parented_products` (itc_933): 16 matrix multiplies a frame in
   the cockpit's draw, all paired between ticks, **largest turn in the
   car's frame 0.000 rad**. Every cockpit piece is placed with exactly
   the car's matrix; the needle's turn isn't in any world matrix.
4. **A constant stuck at 30 Hz, by the old detector** (itc_930): nothing
   new. But that detector only compared each upload's first 4 registers
   and wanted a steady 15-45 changes/s; fixed since (below).
5. **The gauge models' c9-c12** (`--log_gauge_constants`, itc_935): the
   two flagged gauge models have one type-1002 material each (mesh 1,
   pass 1), no `[model + 320]` array, c13.w = 0.1, never changing. The
   models at cockpit +720+244 and +1440+4 aren't where the needle turns.

## Where it probably turns, and what to do next

The needle is inside the cockpit mesh of draw 2282 and its turn is in
neither its world matrix nor (as far as logged) its material constants.
Candidates, most likely first:

1. **Skinning: a bone palette in vertex constants.** Vertex shader
   `113604C3002A6A21` may be skinned (steering wheel and needles as
   bones). The fixed detector (`--log_stepping_constants`, "tick-stepped
   constants" lines: uploads whose whole value changes only on a tick's
   first frame) ran once in cockpit view (itc_937), but the race was only
   a few seconds: it saw c9 x5 from `0x820CF148` (in `sub_820CECA0`)
   change at c13 on 2 ticks (values like 0.902 → 0.5, 1 → 0: material
   colours, maybe the brake lights) and pixel c43 x1 (lighting). No bone
   palette yet. **Next: one longer drive with it** (cockpit view, revving
   for 30 s), then look for a large vertex upload, then find its recorder
   with `--log_slot_callers` / `--log_upload_owners` and
   `--debug_zero_owners` (see "World-space sprites" in `functions.md`).
   Read vertex shader `113604C3002A6A21` (translated shaders are cached in
   `cache/`; see `native-renderer.md`) to see which constants it reads:
   that answers skinned or not directly.
2. **Vertices written by the CPU.** If the needle's vertices in the
   buffer at `192F5F0C` are rewritten each tick (CPU skinning), the
   texture/memcpy write tracking in the native renderer
   (`texture-handling-optimisation.md`) or a watch on that buffer would
   show it. Then the fix is blending the written vertices (like
   `PerFrameLogic_RecordWorldSprites` blends sprite corners).
3. Once the source is found: if it's a matrix, blend it about the
   needle's pivot like `--blend_about_pivot` does (the pivot code can be
   reused); a plain linear blend of a rotation matrix's elements would
   shrink the needle mid-swing, which may be exactly the "out of
   position" look if something already blends it element by element.

## State of the branch

All committed and pushed; nothing uncommitted. In order:

| Commit | What |
|---|---|
| `fda4e7a`, `84f330f` | `--find_all_changes` (+ `--find_all_changes_dir`, later `--find_all_changes_skip`) in `src/debug/frame_state_diff.cpp`: every word a render-only pass changes, as files or watched for the code changing it. Mostly finds command lists; skip `40000000-41000000`. |
| `372de0a` | `sub_820DAC68` wrapper with `--log_car_counter`; docs ("Checked and left alone: the per-car draw's counter" in `functions.md`). |
| `d2369c2` | `sub_820DBD38` wrapper in `per_frame_logic.cpp`: `--blend_cockpit_gauges` (blends `[car + 4916]`; does nothing, the value is constant 0.1) and `--log_cockpit_gauges`. **Remove** `--blend_cockpit_gauges` before merging. |
| `413026a`, `82ed1b7` | `--log_cockpit_changes` (in the same wrapper): watched ranges are now the two gauge models and their `[model + 320]` arrays. |
| `f93a1f2` | `--log_moving_products` also sums "spinning" products by caller. Keep. |
| `3586178`, `685baf0` | `--blend_about_pivot` (on by default) and `--log_parented_products` in `blend_state.cpp`: the cockpit's draw sets the car's matrix as parent (`BlendState_SetParentWorld`), `ProductInputs` gained `std::optional<Matrix4> parent`, `PlanProduct` solves each part's pivot in the parent's frame and `BlendProduct` moves the blend so the pivot is where it should be. **It never applies** (nothing in the cockpit turns relative to the car). It costs every product 68 more bytes (copied ~1,500 times a frame on the main thread, which recent work trimmed hard). Remove it, or keep only the pivot math for item 3 above and measure with `ITC_THREAD=main itc_recomp/tools/profile_render_thread.sh`. |
| `0143a1d` | `--log_gauge_constants`: a `sub_820CECA0` wrapper logging flagged models' c9-c12. |
| `272e3ed` | `--log_stepping_constants` also lists "tick-stepped constants" (whole upload, changes only on a tick's first frame). Keep; it was quiet in an attract race (only the known c100). |

Before merging: drop what didn't help (`--blend_cockpit_gauges`,
probably `--blend_about_pivot` and the parent field), keep the
diagnostics that are cheap when off, move the findings into
`functions.md` ("Per-frame logic", "What gets blended", Known issues),
and update CLAUDE.md's status.

## Commands

Build and the base command line are in CLAUDE.md. The drives used the
base command plus one diagnostic set, e.g.:

```sh
--log_stepping_constants=true --log_slot_callers=true \
--native_renderer_dump=<dir> --native_renderer_dump_every=10
```

Run it in the background (no `timeout`), tell the user what to do in the
race, wait for them to quit, then read the newest log (`ls -t`).


## New diagnostics from 2026-10-17

- **upload owners** shows that the function at **0x8209637C** uploads data to the vertex buffer that contains the cockpit mesh (which includes the needle).  The count fluctuates each frame, so the buffer is rewritten on every frame.
- **slot callers** is empty – the vertex shader `vs 113604C3002A6A21` has no stream constants (no matrix or bone palette), confirming that the needle is not animated via a matrix.
- **debug_zero_owners** is also empty, indicating the buffer is always written with real vertex data.

**Implication**: the needle’s geometry is updated by a CPU routine (the function at `0x8209637C`) and not by a skinned matrix.  The current interpolation logic, which only blends matrices at tick boundaries, therefore never touches the needle.

We will need to add a vertex‑buffer interpolation step for this mesh (or extend the existing matrix interpolation if we later discover a per‑frame matrix).