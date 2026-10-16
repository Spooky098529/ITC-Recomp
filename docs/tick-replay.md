# Translating draws once a tick (a lead, not started)

Written 2026-10-16 on `shader-lookup`, from a question of the user's: the
game only changes its state on 30 Hz ticks, so why does the renderer
work every draw out again on every frame? Nothing here is measured yet;
the first step is the diagnostic below.

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
