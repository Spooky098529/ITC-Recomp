# Known functions

Names for the generated `sub_XXXXXXXX` functions, worked out so far. The
address is the function's location in the original `default.xex`, so it also
matches Ghidra or IDA. Names are our own guesses; the game shipped without
symbols.

"Rewritten" means a readable replacement lives in `itc_recomp/src/` and
overrides the generated code; "Wrapped" means a replacement there runs extra
code around the original (`__imp__sub_XXXXXXXX`). "Patched" means the generated code was edited in
place (search for `PATCH:`).

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_82090108` | `main` | | Guest entry point on the main thread. |
| `sub_8209A760` | `App::Run` | | Main loop: calls every handler in the tick list at this+4, repeats until the quit flag at this+12 is set. One logic tick per pass, with no measured time. |
| `sub_82099D50` | `ForEachTickHandler` | | Calls each handler in a list through a member function pointer. |
| `sub_82092358` | `Renderer::Init` | | Sets the vblank interval (2 = 30 fps) via `sub_8258D568`, then subscribes renderer callbacks to numbered events (2, 3, 5, 6, 16). |
| `sub_820920D8` | `Renderer::WaitForFrame` | Wrapped | Tick handler on the main thread. Blocks until the render thread signals the frame is done (event at this+392), swaps, and starts the next frame. Frame pacing runs after it (`src/frame_pacing/renderer_hooks.cpp`). |
| `sub_82091EF8` | `Renderer::ThreadLoop` | Rewritten | Render thread loop; started through `sub_8258DE50`. Draws one frame each time the main thread asks, as the game did, without the 2-vblank wait. |
| `sub_82091A60` | `Renderer::Present` | | Flushes, waits for the vblank interval (`sub_8258DC58` → `sub_8258DAA8`), then swaps buffers. Event 5. |
| `sub_82091E58` | `Renderer::DrawFrame` | | Render thread: calls each draw listener in the list at renderer+28. |
| `sub_82091BC0` | | | Draw listener between the scene draw and present; calls `sub_8258D998` and `sub_8229F1B0`. |
| `sub_820988C8` | `Scene::Update` | | Tick handler on the scene object (0x40026FE0 at the menu). Calls the current mode object at +64 (vtable slots 6 and 2). |
| `sub_82098940` | `Scene::Draw` | | Draw listener on the same object. Calls the mode object's draw (vtable slot 5), reading live game state. |
| `sub_8223A3F0` | `Delegate::Invoke` | | Calls the function at +8 with the argument at +4. Used for tick and draw listeners. |
| `sub_82233460` | `MemberDelegate::Invoke` | | Calls the member function at +8 on the object at +4, adjusted by +12. |
| `sub_8258D568` | | | Receives the frame interval from `sub_82092358`. |
| `sub_8258DAA8` | `VideoOutput::WaitForVblank` | Rewritten | Waited for the 2-vblank interval; now only makes the preceding device call (`sub_822A0718`). |
| `sub_8258DD90` | `Event::Wait` | | r3 = event, r4 = pointer to timeout (-1 = forever). |
| `sub_8258DD78` | `Event::Signal` | | r3 = event. |
| `sub_822B20D8` | | | Graphics driver present bookkeeping (frame counter at +20024). Library code, not game timing. |
| `sub_825BA0C8` | `QueryPerformanceCounter` | | System library. |
| `sub_820C0128` | `Vehicle::GetSpeedKmh` | Rewritten | Returns the float at vehicle+48 (m/s) times 3.6. |
| `sub_82093DC0` | | | First tick handler: polls input, then walks listener lists that run both game logic and the draw step (see "Separating drawing from the game tick"). |
| `sub_821062E8` | `GameMessages::Dispatch` | Rewritten | Delivers the game's queued messages to its handlers: 0 = Update (all game logic), 1 = Draw. Skips Update on render-only passes (`src/frame_pacing/game_messages.cpp`). |
| `sub_82106280` | | | Picks the handler object a message goes to; its method 0 handles it. |
| `sub_820990F0` | `Input::Update` | Wrapped | Input manager update; skipped on render-only passes so presses aren't seen several times. |
| `sub_8215B4F8` | `Race::DrawStep` | Wrapped | The race's draw step (Draw message). Its running marks the pass as in a race. |
| `sub_820A3978` | `CameraPass::Update` | | Rebuilds a camera pass's view (+0x10) from its eye, target and up vector (+0x190, +0x1A0, +0x1B0), and its view-projection (+0x90 = view * projection at +0x50). Called several times per Draw for pass 0. |
| `sub_8209BD50` | `Matrix::Multiply` | Wrapped | out (r3) = a (r4) * b (r5), 4x4. Interpolation notes every multiply into a camera (see "Interpolation"). |
| `sub_825953D0` | `Recorder::RecordObject` | Wrapped | The bulk per-object recorder, ~150 calls per pass in a race: records command 7 with vertex constants 0-8: world (r5) * camera view-projection (r4), the world matrix, and a vector. The camera is often a stack copy, not a camera pass. |
| `sub_825951E0` | | | Multiplies a world matrix and camera into a stack temporary; not recorded (likely a visibility test). |
| `sub_8258FD48` | `Command::SetShaderConstants` | Wrapped | Render thread, command 7: r3 = payload, r4 = register count, r5 = stage << 24 \| type << 16 \| start << 8 \| count. Interpolation substitutes blended values here. |
| `sub_8258CAA0` | `CommandList::AllocateSlots` | Wrapped | r3 = list, r4 = slot count; returns the first 16-byte slot. Wrapped only for diagnostics (`--log_slot_callers`). |
| `sub_820C0150` | `Vehicle::GetDistancePerFrame` | | Speed (m/s) times a hard-coded 1/30 s. |
| `sub_8210EDB0` | | | Wind sound; scales with speed. |
| `sub_82152CD8` | | | Camera shake; scales with speed. |
| `sub_8213F530` | | | Parent of the speed trail object (switches mode at about 88 km/h). Calls `sub_8213F2E0`, `sub_8213F140` and `sub_8213F4B0`. |
| `sub_8213D438` | | | Resets the trail history; runs on a camera switch. |
| `sub_820F11A8` | `TextureAnimations::Advance` | Wrapped | From the draw step part `sub_820F6068`: advances the flipbook animation set at r3+1060 (`sub_82146B20`). Skipped on render-only passes (see "Per-frame logic"). |
| `sub_82146B20` | | | Calls `sub_82146AC0` on every animation in a set: count at +0, then offsets to each animation. |
| `sub_82146AC0` | | | Advances one flipbook animation: frame count +0, current frame +4, tick counter +8 (wraps at the length at +12), frame start ticks from +16. |
| `sub_82146B80` | | | Returns the current frame's entry of animation r4 in set r3 (texture id at +4). |
| `sub_82146A38` | | | Finds the animation in set r3 whose texture is r4; -1 if none. |
| `sub_82148658` | | | Draws the light groups: for each group, builds a light list on the stack (count +32, 48-byte lights at +36, from static level data) and picks its texture's current animation frame. Calls `sub_8209CDB0` → `sub_8209CC38`. |
| `sub_82107910` | | Wrapped | From Draw; adds one to the play time frame counter at `0x826C5B90` (see "Per-frame logic"). |
| `sub_821109B8` | | | Turns the play time frame counter into hours, minutes and seconds (30, 1800, 108000 frames). |
| `sub_8218E420` | | Patched | Post-race check; the comparison against 60 was changed to 1 to fix the black screen after quest races. |

## Frame structure

The main thread and the render thread run in lockstep:

1. The main thread runs the tick handlers in order. At the menu these are:
   `sub_82093DC0`, `sub_820920D8` (wait for the previous frame),
   `sub_820988C8` (scene update), `sub_82096350`, `sub_82099828`,
   `sub_820A4610`, `sub_82091A58`, `sub_82096340`, `sub_82096330`,
   `sub_820990F0`, `sub_820902C8`.
2. `Renderer::WaitForFrame` signals the render thread (event at
   renderer+388) and blocks until it signals back (renderer+392).
3. The render thread runs the draw listeners: `Scene::Draw`,
   `sub_82091BC0`, `Renderer::Present`. Present waits 2 vblanks.

Rendering reads live game objects while the main thread is blocked. There is
no snapshot of the scene.

With `src/frame_pacing/` (branch `unlock-fps`), every main loop pass still
draws one frame, but in a race only some passes run game logic; see
"Interpolation". `--log_fps=true` logs frames and ticks per second.

## Fixed time step

The game never measures elapsed time. Each pass of `App::Run` is assumed to
be 1/30 s, so running the loop at 60 fps doubles game speed. Functions that
load the step as a float constant from `.rdata` (`1/30` at `0x820029EC`,
`1/60` at `0x820029E8`):

- 1/30: `sub_820B1790`, `sub_820B8D30`, `sub_820C0150`, `sub_820C23E8`,
  `sub_8210D890`, `sub_8210EEE0`, `sub_8211F530`, `sub_8212A350`
- 1/60: `sub_820B0EE8`, `sub_820D0D60`, `sub_82155E58`, `sub_8224AB50`

Also 48 functions load 30.0 and 12 load 60.0. These are likely
frames-per-second conversions or timers counted in frames, but each one needs
checking. Integer frame counters (for example `cmpwi r11,60`) aren't
included in these counts.

## Known data

| Address | What | Notes |
|---|---|---|
| vehicle+0x30 | Speed (float, m/s) | Read by `Vehicle::GetSpeedKmh`. |
| vehicle+0xD0 | Vehicle world matrix | 4x4 row-major: rotation rows, then position row (w = 1). The car's forward axis is -row 0. |
| vehicle+0xCF0 | Copy of the world matrix | Identical every tick; not yet known which one drawing reads. |
| `0x826BAC10` | Camera view matrix | Pass 0's view (+0x10). The camera position is -t·Rᵀ. In the car's frame the views sit at (-5.48, 1.34, 0) for chase, (-0.44, 0.67, 0) for bumper and (-0.26, 0.85, -0.38) for cockpit. Writing it has no effect while driving: Draw rebuilds it from the eye and target. |
| `0x826BAC00` | Camera pass array | 21 objects of 592 bytes: view +0x10, projection +0x50, view-projection +0x90, eye +0x190, look-at target +0x1A0, up +0x1B0. Pass settings at +564 to +588, active flag at +588. Pass 0 is the main 1280x720 view; pass 10 a 1024x1024 reflection and 11 a 512x512 shadow map, both rebuilt in Draw. |
| `0x70180000` | Main thread stack | During Draw, holds temporary world matrices and camera copies passed to the recorders (for example 0x7018E640). Not render state. |

## Render command recording

The game records render commands on the main thread, during Update and Draw,
and the render thread only replays them. Matrices are copied into the
commands at record time, so changing them between ticks doesn't affect what
is drawn. Recording happens inside `sub_820C5DE8` (5,600 lines, also game
logic), which calls `sub_8209F8A0` and `sub_8209F900`. Those loop over the
active camera passes and record them with `sub_820A0CC0` and `sub_820A1C30`,
which allocate command packets with `sub_8258CAA0`.

Interpolation (`src/interpolation/`) therefore substitutes values while the
recorded commands are replayed.

### Command list format

- Scene object +8 is the command list. It's double-buffered: buffer pointers
  at +24 and +28, the index being recorded at +4 (flipped by `sub_8258C9C0`
  each tick), slots used at +12, and the previous recording's slot count at
  +16 (saved by `sub_8258C998`). The render thread replays the other buffer,
  so drawing is one tick behind logic.
- Recorders write through the list pointer at `0x8296B918`. `sub_8258C9D0`
  opens a chunk, `sub_8258CAA0` allocates n 16-byte slots (write pointer at
  +44) and `sub_8258CA50` closes the chunk.
- Commands are 16-byte slots: header [id, arg1, arg2, 0] then payload slots.
  `sub_8258EE98` walks item chains (types 0-6), and `sub_8258E698` and others
  dispatch each command through the table at `0x8265EDE0` (38 ids).
- Command 7 (`sub_8258FD48`) sets shader constants. arg1 is the register
  count; arg2 is stage << 24 | type << 16 | start << 8 | count. The payload
  follows the header. In a race a frame replays about 1,200-1,800 vertex
  constant uploads of 4+ registers. Vertex constants 0 with 9 registers are
  the per-object constants from `sub_825953D0` (world * view-projection
  first); 9 with 5 registers come from other recorders.

### Recorders

- `sub_825953D0` is the bulk per-object recorder (vertex constants 0, 9
  registers); it allocates the payload with the call returning to
  `0x82595428` and fills it with the multiply returning to `0x82595438`.
  It runs on every pass, including render-only ones.
- `sub_82595550`, `sub_82146BD0` and `sub_820CECA0` record draws with a
  command 7 (vertex constants 9, 5 registers). `sub_82595490` records pixel
  constants 0, 63 registers.
- `sub_82128750` (from `sub_821292B0`) multiplies by camera passes 0 and 3
  during Update, about 20 times a tick each.
- Other 4-register constant uploads come from `0x820C6B14`, `0x820C725C`,
  `0x820C7640`, `0x820C7A38` and `0x82136518`, once per pass each.

## Separating drawing from the game tick

Measured in a race (temporary diagnostics, since removed):

- The main loop's first tick handler, `sub_82093DC0`, polls input and then
  walks listener lists (at +28 or +40 by game state, then +52, +76 and +96).
  Both physics and render recording run in the +40 list (+28 in the other
  state).
- Within it, `sub_8215B4F8` is the draw step: it updates the 21 camera passes
  (`sub_820A0958` each), records the passes (`sub_8209F8A0`,
  `sub_8209F900`), runs the render orchestrator `sub_820C5DE8` (about six
  near-identical blocks of recording, one per pass), then `sub_820F6068`,
  `sub_82130978` and `sub_82133960`. No physics runs inside it: the speed
  getter was called 0 times inside `sub_820C5DE8` and ~90 times a second
  outside.
- Running the draw step twice per tick, rewinding the command list header
  (64 bytes at the list object) before the second run, crashed at race
  start: the render thread jumped to a garbage address while replaying. The
  draw step registers what it records somewhere besides the list header
  (pass item lists or the replay's sort queue), and those kept pointing at
  overwritten data.

Recordings are registered in the game mode object's pass lists. The mode
object (scene+64, vtable `0x8206CB8C`) holds two pass lists at +8 and +12;
its state (+4) holds the replay index at +0 and a float submission sequence
at +84. Method 6 (`sub_8258E5E0`) flips the replay index, method 7
(`sub_8258E5F8`) returns the list being recorded, method 8 (`sub_8258E618`)
the list being replayed, and method 4 (`sub_8258EDC8`) submits an item into
a bucket (1,024 buckets of 16 bytes at list+24, count at +12) and advances
the sequence. `sub_8258EB38` empties a pass list.

Restoring the command list header, the recording pass list (header and
buckets) and the sequence before a second run made re-running the draw step
safe (an experiment since removed). The approach used instead is below.

## Interpolation

Branch `unlock-fps`. Game logic stays at a fixed 30 Hz; races draw at any
frame rate (`--max_fps`, 0 = unlimited), each frame a blend of the last two
ticks. Menus stay at 30 fps.

### Passes

`src/frame_pacing/`. After each `Renderer::WaitForFrame`, the pacer decides
what the coming main loop pass is:

- **Logic pass** (a 30 Hz tick is due): runs normally.
- **Render-only pass** (in a race, otherwise): `GameMessages::Dispatch`
  consumes Update messages without delivering them and the input update is
  skipped, so the pass draws the same game state again.

Only races get render-only passes: menus record their UI and act on input
during Update, so they flickered and repeated button presses. A pass is "in a
race" when the last logic pass ran the race draw step `sub_8215B4F8`.

Every other main loop tick handler still runs on every pass, and they must:
`Scene::Update` (`sub_820988C8`) flips the command list and starts each
pass's recording (skipping it froze race loading), `0x82091A58` kicks the
render thread, and `0x82096340` is needed for drawing (skipping it left a
black screen with red blocks). See "Per-frame logic" for what counts frames
anyway.

### Per-frame logic

Some game logic runs from Draw (or the per-pass handlers) and counts frames.
On hardware every pass was a 30 Hz tick, so it counted ticks; with
render-only passes it ran ~5x fast. Each case found is fixed in
`src/frame_pacing/per_frame_logic.cpp`, without changing what's drawn:

| What | Function | Fix |
|---|---|---|
| Blink timers of the car's sprites (four 64-bit frame counters, called from the car's draw `sub_820DDE58`) | `sub_820DC958` | Skipped on render-only passes (`--per_tick_blink_timers`) |
| Texture animations: the flashing building lights, the flashing road signs, the chevron over off-ramps and the player's triangle on the minimap (flipbooks: each animation steps a tick counter and picks its frame by it) | `sub_820F11A8` | Skipped on render-only passes (`--per_tick_texture_animations`) |
| Play time (a frame counter at `0x826C5B90`, which `sub_821109B8` turns into hours, minutes and seconds at 30 a second) | `sub_82107910` | One taken back before the call on render-only passes (`--per_tick_play_time`) |
| UI movies: the HUD, including the race start countdown (a 240-frame movie, a beep at frames 60, 120, 180, 240) | `sub_8211D2F8`, with r4 = 0 (advance a frame and draw); r4 = 1 only draws | Replayed (below) (`--per_tick_ui_movies`) |

**How the texture animations were found.** `--find_per_frame_state`
finds only ±1 steps, and the counters it found first (at 0x4005xxxx, half
the pass rate) were the command lists' per-buffer bookkeeping, not
animations. `--debug_hide=8209CC38` hid the building lights, so they were
the light billboards. `--watch_lights` showed the lights' data never changes
(it's static level data), so the flashing came from the texture each group
picks, which led to the animation set. `--find_any_steps` and
`--watch_lights` together made the game too slow to play (and juddery in
attract mode); run them separately.

**Race overlays** (the marker above opponents, the light trails). Update
handler `sub_8215E150` calls `sub_8215DEA0` with a list of 25 overlay
objects: each is updated (vtable slot 4, +16) then records its drawing
(slot 5, +20), during Update. Render-only passes skip Update, so the
overlays showed only on logic passes (flicker). Now:

- Render-only passes still deliver Update to this one handler
  (`--redraw_race_overlays`, `game_messages.cpp`), and the
  `sub_8215DEA0` wrapper (`per_frame_logic.cpp`) runs only the recording
  loop there, so the overlays draw every frame and advance once a tick.
- The overlays place themselves with the camera passes (`0x826BAC00`, 21 x
  592 bytes) as game logic left them; by a render-only pass's Update, Draw
  has reused them for other views. The wrapper saves them on the logic pass
  and puts them back around the render-only recording.
- The wrapper sets numbering contexts for camera products
  (`BlendState_SetHandler`: 0x8215E151 for updates, 0x8215E150 for
  recordings), so the render-only recording's products pair with the logic
  pass's. They're also matched by value (`overlay_keys_` in
  `blend_state.cpp`), since the render-only recording numbers them
  differently.
- The sprites' own corners are blended separately; see "World-space
  sprites".

Dead ends on the marker, all before it was found to be its corners: blending
vehicle poses for the overlays, lifting vehicle +0xD0/+0xCF0 (the marker
didn't move), blending the overlay objects' first 0x1000 bytes
(since removed), carrying logic-pass products
through render-only copies (`OnCopy`, kept, harmless), restoring the camera
passes before Draw (since removed), and hiding
`sub_821225D8`, `sub_820DDE58`, `820F6068` and HUD handlers.

**Replaying** (`ReplayFromLogicPass`): when a call both steps an object's
state and draws it, skipping it would stop the drawing too (the HUD
flickered). Instead, on the logic pass the object's memory is saved before and
after the call; on a render-only pass the call runs from the "before" copy
(drawing exactly what the logic pass drew) and the "after" copy is put back,
so the state advances once per tick. Calls are matched by object and order
within the pass. If the object changed since the logic pass, the call just
runs.

**Finding them.** The race start countdown runs at the main loop pass rate,
so with frames uncapped it's fast; at `--max_fps=30` it's right.

- `--find_per_frame_state` (`src/debug/frame_state_diff.cpp`) compares guest
  memory before and after a render-only pass every 2 s. Words that step like
  a timer (floats by ±1/30 or ±1/60, small integers by ±1), in more than one
  sample, are watched; each change is attributed to the tick handler,
  message handler (delegates by their target) or wrapped draw step part that
  made it. `--find_countdowns` looks for values counting down instead;
  `--find_any_steps` for any steady step (a float by under 1, an integer
  by up to 4, the same amount in two samples), for animations that don't
  count in frames;
  `--find_per_frame_state_interval_ms=0` samples every pass (very slow);
  `--watch_addresses` watches given addresses from the start.
  `--watch_lights` watches the light billboards' lights (the lists drawn by
  `sub_8209CC38`: count at +32, 48-byte lights at +36, sizes at +32/+36).
- `--render_only_skip=<hex,...>` skips tick handlers or message handlers (by
  target) on render-only passes, to bisect. Skipping the UI handler
  `0x8216C330` fixed the countdown's speed but hid the HUD on render-only
  frames, which led to the movie call inside it.
- `--log_recording_by_pass` lists the code that records render commands more
  often on logic passes than on render-only ones: something drawn only on
  logic passes, or gated by per-frame state. It points at `sub_821281A8`
  (reached from `sub_821225D8`, which steps a counter at +0x150 and draws
  once per view during Update), but hiding `sub_821225D8` entirely leaves
  the marker above opponents on screen: the marker reaches it from the race
  overlays instead (see "World-space sprites").
- `--debug_hide=<hex,...>` never runs the given wrapped draw functions, to
  see what each draws.
- `--log_recording_handlers` lists which message handlers record commands,
  by message. `--render_only_update_handlers=<hex,...>` still delivers Update
  to given handlers on render-only passes (a test).

What didn't work: skipping all non-drawing tick handlers on render-only
passes (froze loading: `Scene::Update`); skipping `0x82096340`, first as a
function (a generic "call vtable slot 1" thunk) and then as a tick handler
(both broke rendering); skipping `sub_822128A8` in the HUD (it's part of
drawing).

### World-space sprites

Some sprites are drawn in world space: game logic writes each sprite's four
corners (x, y, z, pad, 16 floats at +80) once a tick, the recorder writes
them into the command list as vertices, and c0 x4 is the sprite's matrix
(+0, identity here) times the camera. The camera product is blended at
replay, but the corners stayed at the newest tick, so the sprite ran ahead
of its blended car early in each tick and caught up by the end. Both
recorders blend the corners between their last two ticks while recording
and then put them back (`--blend_world_sprites`, in `per_frame_logic.cpp`;
a corner that moved more than 10 units in a tick is a new sprite):

| What | Recorder | Matched by |
|---|---|---|
| The marker above opponents (one sprite, from the race overlays' recording, once a pass) | `sub_821281A8`: r3 = sprite, r4 = camera | Sprite, caller and order in the pass |
| Light trails behind headlights and tail lights (up to 500 sprites; count at +0x109F0, sprite i at +80 + 64 * i) | `sub_82128750`: r3 = list, r4 = camera | Order in the pass (the list is shared: each overlay fills and records it in turn) |

The marker was found by elimination: `--log_upload_owners` lists the code
that allocated each vertex constant upload's header (with
`--log_slot_callers`), and `--debug_zero_owners=<hex,...>` replays those
uploads as zeros so their draws vanish. Use the addresses
`--log_upload_owners` prints (the header's allocation), not the payload
allocation that `--log_slot_callers` lists: those never match, so nothing
is zeroed. Halving the list found 0x82128468,
in `sub_821281A8`. Before that, a long run of guesses (vehicle poses,
overlay object state, camera products, the camera passes as Draw sees them)
changed nothing, because none of them was the corners. `--log_single_sprites`
and `--log_world_sprites` log the sprites as recorded.

### What gets blended

`src/interpolation/blend_state.cpp`. Drawing runs a tick behind: a logic
pass's frame shows the previous tick (amount 0) and the following frames move
towards the current one (amount = time since the tick / 1/30 s).

Almost everything drawn in 3D gets its transform through the matrix multiply
`sub_8209BD50`: out = world * a camera's view-projection, written straight
into a command 7 payload. So blending happens there, in two halves:

1. **Main thread, recording.** Every multiply by a camera is noted, on every
   pass: the per-object recorder's (return address `0x82595438`), any
   multiply whose second operand is a camera pass's view-projection (the
   scenery's, during Update), and any multiply whose second operand is a
   perspective view-projection wherever it's kept (the light billboards'
   recorder `sub_8209C1A0` builds one on its stack). Matrix copies
   (`sub_820A0AC0`) carry on: a copy of a camera counts as identity * camera
   (the headlight projector's matrix, built by `sub_82597610`, arrives this
   way), and a copy of a noted product as that product. On a logic pass a
   copy of both inputs (world, view-projection) is kept, keyed by:
   - the operands' addresses (often the same stack temporaries for many
     objects),
   - which camera it is: a permanent id per distinct projection (aspect
     ratio and depth mapping, matched within 1% through log-scale buckets),
     because the main view, mirror and reflection cameras share stack copies
     and their order varies between ticks. Ids must never be renumbered:
     a table that once reset when full unpaired everything,
   - how many times that combination was multiplied earlier in the pass.

   Each key is paired with the same key from the previous tick. Render-only
   passes draw unchanged state, so their multiplies must have the same inputs
   as the logic pass's under the same key; a mismatch is left alone.
2. **Render thread, replay.** After each Draw the main thread publishes, by
   payload address, the products recorded and their two ticks' inputs, plus
   the frame's amount. As `sub_8258FD48` replays a constant upload from a
   published address that still holds the recorded value, it uploads the
   blend instead and puts the command list back afterwards.

Blending a product:

- **World matrix**: position, rotation and per-axis scale. One that turns
  more than 0.5 rad in a tick about mostly one local axis is a wheel: its
  turn is split into a twist about that axis and the rest (steering, the car
  turning); position and the rest blend, and the twist (the spin, which
  can't be interpolated at speed) is the newer tick's.
- **Camera**: split into placement (blended rigidly: position straight,
  rotation along the shortest arc) and a symmetric perspective projection
  (scales and depth mapping, blended linearly), then multiplied back
  together. Splitting keeps field-of-view changes in fast turns blending.
  A mirrored camera (a reflection) has the mirror moved into a negative x
  scale. Non-perspective cameras (the orthographic shadow map's, refitted
  around the car each tick) aren't blended: everything using one must agree
  within a frame, and only the objects move.
- A pair is drawn unblended if the world or camera moves more than 8 m or
  turns more than 0.5 rad in a tick (and isn't a wheel), or the projection
  changes by more than a few percent (a cut, or two different cameras).

Two more things are blended, outside the products:

- **Light billboards' facing.** `sub_8209C6E0` builds the light glows' quads
  on the CPU in Draw, turned to face its camera pass. For that call alone the
  pass gets the frame's blended eye, target, up and view, so the quads face
  the camera they're drawn from (otherwise their lower edge, cut by the
  road's depth, wobbles).
- **Per-object pixel constants** (`sub_82595490`, 63 registers at c0, and the
  same layout at c63: lighting, including the headlights). They hold no
  object identity, so each is paired by the camera product replayed just
  before it, and uploaded as the frame's linear blend of the two ticks.
  Components c1.x, c3.xyz and c4.x of an upload are per-object light switches
  that legitimately jump between ticks: they keep the newer tick's value. A
  jump anywhere else means a wrong pairing, and the upload is left as
  recorded (blending two objects' lighting flashes headlight-colored light
  onto random geometry). `--blend_pixel_constants=false` turns this off.

Logic and render-only passes both draw the tick's real state; nothing is
blended while drawing. Blending the camera inside Draw changed what the game
culled, so the order-based pairing slipped and bits of the scene stepped at
30 Hz. `--blend_while_drawing` brings back the in-Draw blending (camera eye
and target, and recorder world matrices), mostly for comparison.

### Options and diagnostics

| Option | What |
|---|---|
| `--max_fps` | Frame rate cap in races; 0 = unlimited. |
| `--interpolate_draw_state` | Blending on (default) or off. |
| `--motion_blur` | The game's velocity-based motion blur (pixel shader `7EB16456AAD56CBA`), skipped when false (as `run.sh` does, except with `--no-interpolation`). Its velocities come from the tick, so it smears a whole tick of motion at high frame rates. Patch `patches/rexglue-sdk/0002`. |
| `--blend_pixel_constants` | Blend per-object lighting constants (default on). |
| `--log_fps` | Frames and ticks per second. |
| `--log_blend_state` | Per second: products published, blended, unpaired, rejected cameras with the reason, and uploads left alone. Unpaired or rejected counts above zero in steady driving mean something is stepping at 30 Hz. |
| `--log_replayed_constants` | One frame per second: every vertex constant upload replayed, whether it changed since the last frame, and whether it was substituted. |
| `--log_stepping_constants` | With `--log_slot_callers`: every 2 s, the constant uploads left as recorded that change about 30 times a second (stuck to the tick) rather than every frame, with the code that recorded them. The quickest way to find what still steps. |
| `--per_tick_blink_timers`, `--per_tick_ui_movies`, `--per_tick_play_time`, `--per_tick_texture_animations` | The per-frame logic fixes (default on); see "Per-frame logic". |
| `--find_per_frame_state`, `--render_only_skip`, `--log_recording_by_pass` and others | Finding per-frame logic; see "Per-frame logic". |
| `--log_game_messages`, `--log_slot_callers` | Message delivery per pass; which code allocates constant slots. |
| `--log_upload_owners`, `--debug_zero_owners` | Which code recorded each vertex constant upload; replay a recorder's uploads as zeros to see what it draws. The fastest way to identify a draw; see "World-space sprites". |
| `--log_overlay_products`, `--log_overlay_uploads` | The race overlays' camera products and uploads, as replayed. |
| `--log_world_sprites`, `--log_single_sprites` | The world-space sprites as recorded. |
| `--log_camera_pass_records` | Which camera passes are recorded on logic and render-only passes, by message handler. |

### What didn't work, and why

- **Blending matrices in memory before Draw** (camera passes, the
  0x7018xxxx block). That block is the stack. Draw rebuilds the cameras from
  eye and target, and the scenery's camera is a stack copy that only changes
  on ticks.
- **Matching uploads by value** (find a camera's matrix in the command list,
  or test whether an upload is "some world * this camera"). Several cameras
  are similar enough to be confused, which put road in the reflections.
  Pairing by the multiply's operands is exact.
- **Rebuilding a product from the camera as Draw left it.** Draw reuses
  pass 0 for several views within one Draw, so that gave the wrong camera
  (the car juddered).
- **Rigid-motion blending of view-projections** (`FindRigidMotion`). It fails
  when the field of view changes during a turn; the placement/projection
  split replaced it for perspective cameras.
- **The command list shim** (`--interpolate`, `interpolator.cpp`,
  `command_list.cpp`, `draw_tags.cpp`): blending recorded constants matched
  between ticks by draw order. Left in but off.
- **Pairing pixel constants by order in the frame.** Objects coming into or
  leaving view shifted the order; pairing by the preceding camera product
  fixed it.

### Finding what still steps

The loop that found the light billboards, headlight projector and lighting
constants:

1. Run with `--log_stepping_constants --log_slot_callers` and drive. Uploads
   still at 30 Hz are listed with their stage, register, size and recording
   code (the return address of the slot allocation for the command's
   header).
2. Disassemble around that address in `generated/` to find where the
   payload comes from (a multiply, a copy, or something else).
3. Keep per-tick copies of that source's inputs and blend them at replay,
   keyed by something stable across ticks; check with the log again.

## Known issues

- **Menus run at 30 fps** (`unlock-fps`): they record their UI and read
  input during Update, so render-only passes made them flicker and repeat
  presses. Unlocking them would mean skipping Update only for the handlers
  that run game logic.
- **The race HUD updates at 30 fps.** Accepted for now; its animations
  advance once per tick (see "Per-frame logic").
- **More per-frame logic may exist.** Anything that runs too fast or
  flickers only with frames uncapped is probably another case; see
  "Per-frame logic" for how they were found.
- **Some uploads still step on purpose**: the shadow lookup matrices (c70 and
  c74, orthographic, kept at the tick to match the shadow map), c14 x4 (a
  texture projection) and c100 x4. Nothing visible steps in testing; see
  "Finding what still steps" if something does.
- **A piece of geometry flickers while the car moves**, at native 30 fps
  too, so not interpolation: probably the game's own effect (a broken light).
- **One tick of latency**: drawing shows between the previous and the
  current tick, as usual for this kind of interpolation.
