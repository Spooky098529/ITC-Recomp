# Translating draws once a tick (in progress on `tick-replay`)

Written 2026-10-16 on `shader-lookup`, from a question of the user's: the
game only changes its state on 30 Hz ticks, so why does the renderer
work every draw out again on every frame? Built on `tick-replay`
(2026-10-17): steps 1 and 2 work behind options, off by default; what's
left is in "Handoff: state and what's left" at the end.

## What happens now on an in-between frame

With interpolation, a race frame that isn't a tick is a **render-only
pass** (`functions.md`, "Interpolation", "Passes"): the main loop runs
with Update skipped, so it draws the same game state again. At ~450 fps
uncapped that's ~15 frames a tick, 14 of them render-only. On each:

1. **The main thread runs the game's Draw step again** and records a
   fresh command list from the same tick's state (`Scene::Update` flips
   the list and starts the recording on every pass). The main thread is
   at ~88% of a core in an uncapped attract race (2026-10-16), about as
   busy as the render thread.
2. **The render thread replays that list from scratch**
   (`gpu-functions.md`): each command's handler runs the game's Direct3D
   (the guest share of the render thread, ~37% in "Profile of
   2026-10-16, after `texture-binding`" in `native-renderer.md`), then
   our `Passes::State::Draw` works each draw out again: registers,
   shaders, texture sets, pipeline, constants.
3. **Interpolation substitutes blended values** as the replay uploads
   them (`BlendState_ReplayConstants`: cameras, world matrices, lighting;
   `functions.md`, "Interpolation").

Only step 3 makes anything new. Steps 1 and 2 rebuild what the tick's
first frame already built; the per-draw caches so far (repeated state,
constant blocks, register reads, texture bindings, shader lookups) each
made a part of that rebuilding cheaper, without removing it.

## The idea

On a tick's frame, keep what each draw came to, in our own terms: the
pass and target, pipeline, descriptor sets, vertex and index buffers,
draw parameters, and where each constant block comes from (a fixed
upload, or one interpolation blends). On the tick's render-only frames:

- don't run the game's Draw step (main thread);
- don't replay the command list through the game's Direct3D, and don't
  work the draws out again (render thread);
- emit the Vulkan commands from the kept list, writing only the blended
  constants fresh.

What's left per in-between frame: the blending, the constant uploads,
the Vulkan calls (the driver was ~14% of the render thread) and the
resolves. Both threads' work on 14 of 15 frames could drop by a large
factor, not a few percent; on the Steam Deck, where the render thread is
flat out, it matters most. Menus have no render-only passes, so this is
for races only (where the load is).

Related, in `native-renderer.md` "Ideas for lightening the render load":
item 1 (redraw shadows and reflections once a tick) would skip whole
passes on in-between frames; this keeps every pass but stops working
them out again. Item 4 (splitting the render thread) would be less
needed if this works.

## Why it might not hold

From what's documented; each needs checking:

- **Do render-only recordings match the tick's?** The render-only
  recording numbers its products separately, and interpolation pairs
  them with the logic recording's by order and position (`functions.md`,
  "Interpolation"; wheels are "paired by position"). So the two lists
  may not be the same draws in the same order.
- **Per-frame data.** Inline vertex draws get fresh data every frame (the
  mirrors give them half their region per frame slot); loader bursts
  landed on render-only frames (`native-renderer.md`, "Hypothesis to
  test: the mirrors updated once a tick"); and the "Per-frame logic"
  exceptions in `functions.md` (blink timers, texture animations, the
  minimap's flashing, play time) run per pass, some of them put back
  around render-only passes.
- **Interpolation is tied to the replay.** It blends values as the game
  uploads them, matching uploads to products. It would have to blend
  into the kept list instead: a rewrite of how it feeds the renderer.
- **Skipping the game's Draw on render-only passes** was avoided before:
  re-running or skipping parts of the main loop broke things (skipping
  `Scene::Update` froze race loading; `functions.md`, "Passes"). The
  render thread's half (replaying our own list instead of the game's)
  can be tried without touching the main thread's.
- **What our draws read that isn't the game's state:** the frame's
  resources (constant rings, descriptor pools reset each frame, the
  texture cache's generations, resolve destinations). The kept list has
  to hold what lasts and rebuild what doesn't.

## First step: measure whether render-only frames repeat the tick

A diagnostic (e.g. `--native_renderer_log_tick_repeats`, every 5 s):
for each draw, hash its non-constant inputs (shader lookup key, the
fetch constants its shaders read, the render state registers, the vertex
and index buffers' addresses and counts, the target), and keep the
frame's sequence of hashes. Compare every render-only frame's sequence
with its tick's first frame:

- frames matching draw for draw;
- for the rest, where they diverge: how many draws, which ones (pass,
  shader, caller), and whether only constants differ;
- the same with constants included, split by whether interpolation
  blended them (expected to differ) or not (shouldn't).

If most frames match exactly and the differences are a known handful
(inline vertices, the per-frame logic exceptions), the design holds and
the render thread's half can be built behind a flag, with a check that
replays the game's list beside it and compares what each draw comes to.
If the sequences diverge often, this lead stops there.

## Measured (2026-10-17, `tick-replay`)

`--native_renderer_log_tick_repeats` (`passes.cpp`, `NoteDrawParts` and
`CompareTickRepeats`): each draw's parts hashed as drawn (target, the
pipeline, the texture fetch constants its shaders read, the vertex fetch
constants it reads, type and indices, viewport/scissor/depth bias/blend
constants, an inline draw's vertices, and each constant block), and each
render-only frame compared draw by draw with its tick's first frame
(`Renderer_ReplayingLogicList`). Every 5 s. An uncapped attract race
(`itc_887.log`), five windows of ~150 ticks and ~1,850-2,300 render-only
frames:

- The tick's draw count: 96-100% of render-only frames (else 1-9 apart).
- Target, pipeline, textures, vertex fetch, indices and dynamic state:
  the tick's for 99.9% of draws (0.01-0.12% differ); for every draw of
  the frame in 76-87% of frames. The differences sampled are all one
  inline draw (pixel shader `9567C79307ACC6F5`, pass 6 or 10) whose
  vertex count changes.
- Inline vertices differ for 3-6% of draws (what the game builds per pass:
  the world sprites and light billboards, blended while recorded).
- Vertex float constants differ for 90-98% of draws and pixel float
  constants for 21-26% (the blended cameras, objects and lighting, as
  expected); system and bool/loop constants ~0%.

So the design holds: within a tick the frame is the same draws. What a
render-only frame has to supply fresh is the blended float constants and
the inline draws' vertices (and their count), and a frame whose draws
don't line up with the tick's (a few percent) has to fall back to the
full replay. The inline vertices come from the main thread's recording of
that pass, so the render thread's half still reads them from the new
list; the main thread's half (not running the game's Draw) would need
them made another way.

First person (2026-10-17, a race the user drove, `itc_885.log`): 26%
fewer fps uncapped than the chase view, from a pass more (the mirror) and
~800 draws a frame more, the render thread's cost per draw unchanged
(see "Tools" in `native-renderer.md`). Tick replay cuts every pass's
draws alike, the mirror's included, which stays drawn every frame.

## Step 1: draws translated once a tick (done, 2026-10-17, `tick-replay`)

The plan agreed with the user, in three steps: (1) our translation of each
draw done once a tick and kept; (2) on in-between frames, neither the
game's Draw step (main thread) nor its Direct3D replay (render thread)
runs, the kept draws issued with interpolation's blends applied to them
directly; (3) the frame's Vulkan command buffers recorded once a tick and
submitted again with only the constants rewritten.

Step 1, `--native_renderer_tick_draws` (off by default; `passes.cpp`,
`TickDrawKey`, `FindTickDraw`, `DrawFromTick`): on a frame replaying a
logic pass's list each draw stores a key of what it was worked out from
(the call, the surfaces' objects and words, the shader objects' fields and
the declaration, the index buffer object, the scissor, a hash of the
register blocks `ReadRegisters` fills, the texture cache's generations)
with what it came to (targets, shaders, system constants, pipeline,
texture sets, viewport, scissor, depth bias, blend constants, index setup,
draw call; `DrawEmit`), and the fetch constants its textures read with
their hash. On the frames after it, a draw whose key and texture words
equal the stored draw's at the same place (`draw_calls`) records the
stored result, packing only its float, bool/loop and fetch constants from
the device as before (the replay still runs, so interpolation's blended
uploads are there); others are worked out in full. Texture sets made on
the tick's frame come from tick rings (`tick_rings`, three, reset in turn
as ticks are captured), as the frames' pools are reset every frame. The
float constant packing (`PlaceFloatConstants`) and the recording
(`RecordDraw`) are shared by both paths. Off with diagnostics that need
every draw worked out (dumps, `--native_renderer_watch_draws`, the glow
log, `--native_renderer_log_tick_repeats`...) and with the per-draw
mirror.

`--native_renderer_check_tick_draws` works out each matched draw in full
as well and compares the two (`CheckTickDraw`). An uncapped attract race
(`itc_888.log`): ~98% of the draws on in-between frames matched (~2,230 a
frame; the rest mostly the texture cache's generation having moved, then
the register blocks, the call and indices: the inline draws), 4.1-4.6
million checked per 5 s, **none different**.

Render thread, `profile_render_thread.sh`, two runs each, alternating, one
binary: off 10.41, 9.54 (9.97), on 8.62, 8.80 (8.71) million cycles a
frame: **-12.6%**. Frames drawn about the same (5,612/6,264 against
5,760/5,972 in the 12 s): the main thread, still running the game's Draw
step every frame, limits instead. What's left of the render thread's work
on in-between frames is mostly the game's Direct3D replay (step 2).

## Step 2: in-between frames drawn from a stored frame (2026-10-17, `tick-replay`)

`--native_renderer_tick_replay` (off by default):

- **Pass kinds.** `GameMessages_PassKind`: 0 logic, 1 render-only, 2 render-only
  and not drawn (a "fast pass": the second render-only pass of a tick on,
  once a stored frame has been made, `Renderer_TickReplayReady`). Handed to
  the render thread with each list (`Renderer_OnListHandedOver`).
- **Main thread, fast passes.** Neither Update (the race overlays included)
  nor Draw is delivered (`DispatchMessages`); `BlendState_PublishFastFrame`
  publishes the frame's amount with the products of the tick's first
  render-only pass, kept by `DrawState` (`kept_outputs_`).
- **Render thread, the tick's first render-only frame** (`capturing_frame`):
  replayed as usual and stored as ops (`StoredFrame`): each float constant
  upload as recorded (before blending, from `Renderer_OnCommand`), each draw
  as worked out (`CaptureDraw`: `TickDraw` with its bool/loop and fetch
  constants and its inline vertices), clears (`ClearTarget`, not
  `GetTarget`'s first one), resolves (`ResolveInto`'s arguments) and the
  front buffer. Texture sets made on it come from the tick rings. Not usable
  with failed draws or tiling.
- **Render thread, fast frames** (`Renderer_DrawStoredFrame`, in place of the
  game's frame, its front buffer resolve and present): the ops again; each
  upload blended (`BlendState_BlendStoredUpload`: products, the per-object
  extras, lighting, as `BlendState_ReplayConstants` does) into
  `float_shadow`, which stands in for the device's float constants (from
  the device as the stored frame left them), each draw's floats packed from
  it. A fast frame with no usable stored frame shows the last frame again.
- **Registers set other than by uploads.** The first version came out with
  wildly wrong lighting (saturated colours, blown-out glow; the user's
  screenshots). A check (kept: it's how they're stored) follows the device's
  float constants as uploads alone would leave them and compares them with
  the device at each draw while storing: pixel c245-c255 (and vertex c0-c3
  once a frame) change without an upload, ~330 draws a frame: the pixel
  shaders' literal constants, loaded as a shader is bound. They're stored
  as plain sets (`StoredKind::kSet`) before the draw that reads them.
  Frames dumped then look right; the user: "it's looking good".

Profiles (`profile_render_thread.sh`, uncapped attract race, two runs each,
off/on alternating, one binary): render thread 10.28, 10.21 against 4.34,
4.28 million cycles a frame (**-58%**); main thread 6.09, 6.06 against 1.26,
1.31 (**-79%**). Frames counted about the same (5,770-5,957 in 12 s): on this
PC the GPU is about the limit at these rates.

### Rebuilt calls: what the main thread builds into the list per frame

What the main thread builds into the list while recording (found by
`--native_renderer_log_tick_repeats`, the "by pixel shader and pass" part of
its line) was at first stored as the tick's first render-only frame had it,
so on fast frames it moved at 30 Hz (the user: light trails "look bad").
Now (`e001686`) those builders run again on fast passes and the stored
draws take their fresh vertices:

- **Main thread** (`game_messages.cpp`): on fast passes the race overlays'
  Update (`8215E150`: the marker above opponents, the light trails) is
  delivered again, and the draw step part `sub_820F6068` (the light groups'
  billboards and the minimap) is called again with the `r3` it had on the
  tick's first render-only pass (noted in its override in
  `debug/frame_state_diff.cpp`, through `GameMessages_BeforeRebuiltCall` /
  `GameMessages_AfterRebuiltCall`), between `BlendState_BeginFastFrame` (so
  billboards face the frame's blended camera, as in Draw) and
  `BlendState_PublishFastFrame`. The command list range each wrote
  (the list's write pointer, `0x8296B918` +44, before and after) is handed
  over with the list (`GameMessages_TakeRebuiltRanges`,
  `Renderer_ReplayingRanges`). Calling the light groups' own draw
  (`sub_82148658`) again crashed the game at the attract race's start (a
  stream of reads of guest `0x50`): its `r3` is an object on
  `sub_820F6068`'s stack, gone by then.
- **Render thread** (`CaptureDraw`, `DrawStored`): while storing a frame,
  an inline draw whose vertices the library's memcpy copied from inside one
  of those ranges (`Buffers_LastCopy`; the helper draw `sub_825A17A0` and
  command 19 both copy the list's vertices verbatim) is tagged with the
  range's index and its offset in it, once a tick. On fast frames a tagged
  draw copies its vertices from the fresh range at that offset if the range
  is the same size as when stored, else uses the stored ones.
- Attract race: 45-80 draws tagged per stored frame, all but ~0.1 a frame
  given fresh vertices. A race the user drove (`itc_913.log`): 180-235
  tagged, all but 0-0.3 a frame fresh, no errors; "trails look ok now".

## Handoff: state and what's left (2026-10-17)

For whoever picks this up. Everything is on the branch `tick-replay`
(pushed), off by default, behind two options:
`--native_renderer_tick_draws` (step 1) and `--native_renderer_tick_replay`
(step 2, independent of step 1; both on is fine). Neither is in `run.sh`
yet. Commits, in order: `e819bc0` (resolve list log), `88bd7d0`
(`--native_renderer_log_tick_repeats`), `440e565` (step 1), the step 2
commits, `e001686` (rebuilt calls).

**How to run it.** The command in `CLAUDE.md` plus
`--native_renderer_tick_replay=true` (and `--native_renderer_tick_draws=true`
for step 1). With logging on, every 5 s: `tick replay: N frames stored (M
not usable...), K frames drawn from them (J with none to draw from)...;
light draws: ... tagged ..., ... with fresh vertices, ... with stored ones;
of D draws stored, ... set other than by uploads (stored as sets): ps c255
...`. M and J should be 0, and "with stored ones" near 0. The two lines
`tick replay: noted call` / `rebuilt call` (the first 20/40) show the
rebuilt call's argument and list addresses; they can go once this settles.
Attract races (`--skip_intro --attract_after_s=1`) exercise all of it
except the HUD and the player's driving. Dumps (`--native_renderer_dump`,
`_every`) work: on a fast frame they show the stored frame (no
`draws.txt`, as no draw is worked out). A log full of `Unhandled guest
access violation` rotates away its start: catch the lines before the first
one with `tail -F <log> | grep -m1 -B40 'access violation'` while it runs.

**Still to do, in order:**

1. **The glow (bloom) on fast frames.** Its cross-fade's weight (the
   vertex colour alpha of the inline quad drawn with pixel shader
   `E0D070FB58A4EA03` in pass 11, ~1 a frame) and its strength (pixel c33,
   uploaded for `9750599DF5ED3B39` in pass 20, recorded by `82133D28` inside
   `sub_821379D8`) are both set per pass by `src/interpolation/glow.cpp`
   while the main thread records, so on fast frames they're the tick's first
   render-only frame's. The history resolve itself is in the stored ops, so
   it's written every frame. Options: call the post-processing objects'
   draw (`sub_821379D8`, recorded for each object by `sub_82133960` from the
   race draw step; check its arguments persist, unlike `sub_82148658`'s)
   again on fast passes, and extend the tagging to **uploads**: a stored
   upload whose payload lies in a rebuilt range takes the fresh values (same
   offset, same range size), as inline draws do. The cross-fade quad is an
   inline draw and would be tagged as is. Or compute the two values natively
   from glow.cpp's state (the weight is the blended `+8460` raised to the
   frame's share of a tick, the strength the blended c33 fields) and patch
   the stored upload and quad. The first keeps the game doing the maths, as
   for the lights.
2. **Two more pixel constants that change between a tick's frames**:
   `0E2F6DFFB4C83835` in pass 11 (2 draws a frame) and `CC253DC700BC8028`
   in pass 23 (1 a frame), from the same diagnostic. Not identified; pass
   11 is the glow chain, so they may come with item 1. Pass 23 is near the
   frame's end (post-processing combine, or the attract mode's filter).
   Find their recorder with `--log_stepping_constants --log_slot_callers`
   (tick replay off), then treat them as item 1.
3. **A check for step 2**, before it ships: on fast passes, let the game
   draw the frame as now and, beside it, work out what the stored frame
   would give each draw (the blended float blocks, the inline vertices),
   and compare. It needs its own `ReplayCameras` and `PixelConstantBlender`
   instances (both keep per-frame and per-tick state, so calling them twice
   a frame would corrupt it). `--native_renderer_check_tick_draws` is the
   model (step 1's). Without it, correctness rests on dumps and the user's
   eyes.
4. **Profile with the rebuilt calls.** The -58%/-79% above was measured
   before them. Calling `sub_820F6068` and the overlays again costs the
   main thread something (the billboards alone were ~0.7 million cycles a
   frame); measure both threads with `profile_render_thread.sh`
   (`ITC_ARGS=--native_renderer_tick_replay=true`, `ITC_THREAD=main`), two
   or three alternating runs each. Compare cycles, not fps: on this PC the
   GPU is about the limit at these rates.
5. **What the user hasn't checked yet**: the light glows on buildings and
   lamps, the marker above opponents, the minimap (its triangle's flashing
   is per-frame logic, put back on render-only passes), first person (the
   mirror pass), camera switches and cuts, the race start countdown, the
   pause menu during a race, the end of a race and the next race's loading
   (stored frames and tick rings across a load), other resolutions
   (`--width`/`--height`), and the Windows build (it should compile; not
   built since).
6. **Edge cases to look at:** a fast frame whose tick's stored frame
   wasn't usable shows the last frame again (the main thread decides a
   pass is fast from the *previous* tick's stored frame, as it can't wait
   for this one's); a texture rewritten mid-tick isn't reloaded until the
   next tick's first frames (fast frames ask the texture cache nothing);
   with the old per-draw mirror (`--native_renderer_mirror_by_tick=false`)
   both steps stay off.
7. **Step 3: the frame's Vulkan commands recorded once a tick.** On fast
   frames the render thread now still records every draw's Vulkan calls
   (`RecordDraw`, ~6% of what's left of it) and packs every draw's
   constants. Record the stored frame into a command buffer once per frame
   in flight per tick (two a tick), with each draw's constant blocks at a
   fixed offset in that frame's ring and its inline vertices in a fixed
   place, then on each fast frame only write the blended blocks and the
   fresh inline vertices and submit it again. Needs: no constant block
   reuse by comparison (`PlaceConstants`) inside the recorded part, the
   present blit and texture uploads kept in their own command buffers
   (`Renderer::EndFrame` records the blit into the frame's buffer today),
   and the stored ops' resolves and clears recorded with it.
8. **Then:** turn it on in `run.sh` and `run.bat` (keep them in step), the
   status in `CLAUDE.md`, a pointer from "Performance: the render thread"
   in `native-renderer.md`, and a merge into `main` (ask the user first).

**Where the code is:** `native/passes.cpp` (`TickDrawKey`/`TickDraw`,
`FindTickDraw`, `DrawFromTick`, `CheckTickDraw` for step 1; `StoredFrame`,
`CaptureDraw`, `NoteUpload`, `CheckShadowFlush`, `DrawStored`,
`DrawStoredFrame`, `TakeStoredFrames` for step 2; `RecordDraw`,
`PlaceFloatConstants` shared), `native/renderer.cpp` (the options, the
lists' kinds and ranges, `Renderer_DrawStoredFrame`),
`native/buffers.cpp` (`Buffers_LastCopy`),
`frame_pacing/game_messages.cpp` (fast passes, rebuilt calls),
`frame_pacing/renderer_hooks.cpp` (the hand-over, the stored frame in place
of the game's), `interpolation/blend_state.cpp`
(`BlendState_BlendStoredUpload`, `BeginFastFrame`, `PublishFastFrame`,
`kept_outputs_`, `PixelConstantBlender::Apply`'s host source),
`debug/frame_state_diff.cpp` (`sub_820F6068`'s override).
