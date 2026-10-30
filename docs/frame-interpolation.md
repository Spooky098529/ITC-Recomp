# Frame interpolation

How the game's frame is structured, how drawing was separated from the
30 Hz game tick, and how frames are blended between ticks: what's blended,
the options and diagnostics, and what didn't work. Moved out of
`docs/functions/functions.md` on 2026-10-20; the functions named here
have their rows in `docs/functions/`.

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

With `src/frame_pacing/`, every main loop pass still
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
  It runs on every pass, including render-only ones. Since the B7 loop
  (2026-10-25) its recording is native (`src/scenery_lists/model_draws.cpp`,
  `--native_scenery_lists`) inside the same wrapper: the multiply and copy
  are noted with those return addresses, and the command list's
  diagnostics (`--log_slot_callers` and the others) run the original.
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

Game logic stays at a fixed 30 Hz; races draw at any
frame rate (`--max_fps`, 0 = unlimited), each frame a blend of the last two
ticks. Menus stay at 30 fps; the pause menu, drawn over a race, runs at
the race's frame rate.

### Passes

`src/frame_pacing/`. After each `Renderer::WaitForFrame`, the pacer decides
what the coming main loop pass is:

- **Logic pass** (a 30 Hz tick is due): runs normally.
- **Render-only pass** (in a race, otherwise): `GameMessages::Dispatch`
  consumes Update messages without delivering them and the input update is
  skipped, so the pass draws the same game state again.

A due tick always wins: when the next tick is due (or overdue), the coming
pass is a logic pass, even if a frame slot is also due. Until 2026-10-14
the frame slot, counted from the last frame's start, could lie in the past
behind an overdue tick after a frame longer than the cap's period, so the
pass was drawn render-only and the tick waited another frame; a tick more
than a whole tick late restarts the 30 Hz grid, and that time is lost. On
the Steam Deck (render thread at ~98% of a core, 20-27 ms frames) races
ran at 13-16 ticks a second while drawing ~40 fps: slow motion. The same
happened here pinned to one CPU (`taskset -c 0`: 36-50 fps, 16-27 ticks);
with the fix, 43-62 fps and 30 ticks. `--log_fps` counts each second's
ticks released over a quarter of a tick late and grid restarts. On the
Deck with the fix (`itc_041.log`, an attract race and some driving): 30
ticks every second in races at 40-66 fps (15-25 ticks a second released
over 8 ms late, no grid restarts), restarts only at loads.

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
| Texture animations: the flashing building lights, the flashing road signs and the chevron over off-ramps (flipbooks: each animation steps a tick counter and picks its frame by it) | `sub_820F11A8` | Skipped on render-only passes (`--per_tick_texture_animations`) |
| The player's triangle on the minimap flashing where it overlaps other cars' (a frame divider: `0x826C5AA4` counts calls and wraps after 4, then steps the phase `0x826C5AA8`, which wraps after 5) | `sub_820F5740` | Both put back after the call on render-only passes (`--per_tick_minimap_blink`) |
| Play time (a frame counter at `0x826C5B90`, which `sub_821109B8` turns into hours, minutes and seconds at 30 a second) | `sub_82107910` | One taken back before the call on render-only passes (`--per_tick_play_time`) |
| UI movies: the HUD, including the race start countdown (a 240-frame movie, a beep at frames 60, 120, 180, 240) | `sub_8211D2F8`, with r4 = 0 (advance a frame and draw); r4 = 1 only draws | Replayed (below) (`--per_tick_ui_movies`) |
| UI movies' adding animation tracks (the pause sub-menus' arrow slid ~5x as far): they write into the movie's nodes, outside the replayed bytes | `sub_825A0548`, inside `sub_8211D2F8` | A workaround for frame rates above 30: on render-only passes, adds whose target the update doesn't put back to the rest pose (`sub_82119B80`) are skipped (`--per_tick_ui_movie_tracks`; `docs/pause-menu-fix.md`, "The sub-menus' arrow"; the track native since 2026-10-25, the override kept around it) |
| The cockpit dials: the turbo gauge reads car + 704, which Draw's per-car update (`sub_820DAC68`) lowers by 1.0 every pass (set again only on ticks), so it fell to its rest angle; and blending the needles' matrices cut across the dials on fast swings | `sub_820E42E8`, `sub_820E45A8` (native, `src/hooks/cockpit_dials.cpp`) | Posed every pass from the tick's gauge values, on the car blended to the frame; replayed as live poses (`--native_cockpit_dials`; "The cockpit dials" under "What gets blended") |
| The rival's logo in the rival intro cutscene (drawn during Update by handler `82159948`) | `sub_821594B8` (native override), `sub_82211A28` | On render-only passes the handler gets Update but only redraws the logo the logic pass drew (`--redraw_rival_logo`; "The rival logo at the race start" below) |

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

**The rival logo at the race start** (fixed; merged from
`rival-logo-flicker`, `--redraw_rival_logo`). Entering a race against a rival plays the rival
intro cutscene (about 13 s): the rival's cars, with the rival's 2D logo
over them. The logo flickers with frames above 30; the attract race's
logo (drawn from Draw) doesn't. (The logo's draw, `sub_82211A28`, is
native since 2026-10-25, `--native_race_hud`, `functions/race-hud.md`; its
override's noting and the redraw are unchanged.) Found
with logs (2026-10-18, `itc_064.log` to `itc_066.log`):

- `--log_recording_handlers`: Update handler `0x82159948` (a jump to
  `sub_821594B8`) is the rival intro cutscene's (the user). It records
  commands only while the cutscene plays, 30 times a second at 100-150
  fps, so only on logic passes.
- `--log_recording_by_pass`: in the same window, about 30 recorders in
  the UI movie library (`8259D7DC`-`8259E050`) record 2-7 times a logic
  pass and never on render-only passes; nothing else in the race does.
- `--log_pause_movie`: the logo is UI movie `423531F0` (`423531E0` in
  another run), advanced and drawn (r4 = 0) from `0x82211A74` in
  `sub_82211A28`, 31 a second, logic passes only. The HUD's movies
  (`822128C8`, `8221293C`, `822129C8`) run from Draw, so they get about
  120 render-only updates a second, replayed (see Replaying below).
- So it's the race overlays' case again: drawing recorded during Update,
  which render-only passes skip. `--redraw_race_overlays` covers only
  `8215E150`.
- `--render_only_update_handlers=82159948` (deliver its Update on
  render-only passes too): the movie got render-only updates (111-120 a
  second), but the whole cutscene then ran far too fast (the user; over
  in ~3 s instead of 13), as the handler is the cutscene. So its logic must
  stay once a tick and only its drawing (the logo's movie) be repeated, as
  the race overlays do.
- `sub_821594B8`, read from `generated/`: works out a mode from its
  input (the word at `[[0x826C2CE8]] + 1860`, `sub_82156980`,
  `sub_821591F0`), then makes virtual calls (slot 0) on the objects at
  this + 0x10BB0 and (if set) this + 0x10BB4, then calls `sub_820AAE40`
  (a jump to `sub_820AA550`) on `[[0x826C2CE8]]` and on
  `[[0x826C2CE8] + 4]`.
- The logo's draw, read from `generated/`: `sub_82211A28` has one caller,
  `sub_82155A28` (at `0x82155BDC`, r3 = its object + 10760, when bit 8 of
  object + 10764 is set, behind several branches on the cutscene's
  state), which is called only from `sub_821561F8`, which has no direct
  caller (a virtual method; most likely slot 0 of the object at
  this + 0x10BB0, not checked). So the logo's draw is nested inside the
  cutscene's step, not one of `sub_821594B8`'s own calls.
- `sub_82211A28(entry)`: if bit 8 of `entry + 4` (shown) is set, looks up
  the movie by its id at `entry + 8` (`sub_8211B3F0` on `0x826C5CCC`),
  advances and draws it (`sub_8211E1B8`, r4 = 0, which jumps to the UI
  movie update `sub_8211D2F8`; the call at `0x82211A74`); if that returns
  0 (the movie ended), calls `sub_8211DF20(movie, id)` and sets bit 16 of
  `entry + 4`.
- Fix (the user's design, `--redraw_rival_logo`, on by default): a native
  override of `sub_821594B8` (`per_frame_logic.cpp`). Render-only passes
  deliver Update to `82159948` too (`game_messages.cpp`, as for the race
  overlays). On logic passes the override runs the director's update (since
  2026-10-30 natively with `--native_camera_modes`,
  `hooks::CameraModesUpdate`, else the original), and a
  pass-through hook on `sub_82211A28` notes the entry it's called with
  during it; on render-only passes it runs nothing of the cutscene, only
  `sub_82211A28` (the game's) on that entry, if the logic pass drew. The
  UI movie replay keeps the logo advancing once a tick.
- Checked (2026-10-18, `itc_067.log`, 144 fps cap): the user saw no
  flicker. The handler recorded on ~150 passes a second (203 slots each)
  instead of 30; the logo's movie (`42353200`) got 30 logic and ~120
  render-only updates a second from `0x82211A74`, all render-only ones
  "replayed" (~240 per 2 s, none "changed since the logic pass"); the
  cutscene lasted ~13.5 s, as before.

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
     ratio and depth scale over w scale, matched within 1% through
     log-scale buckets; not the depth offset, which drifts far from the
     origin), because the main view, mirror and reflection cameras share
     stack copies and their order varies between ticks. Ids must never be
     renumbered: a table that once reset when full unpaired everything.
     Each camera address keeps the ids it used with their latest
     projection and reuses the most recently used one within 1%
     (`--sticky_camera_ids`), so a projection that drifts (a wide camera's
     aspect, 2.8 to 24 over a drive) keeps its id,
   - how many times that combination was multiplied earlier in the pass.

   Each key is paired with the same key from the previous tick, except the
   per-object recorder's products (`--pair_by_object`, see below). Render-only
   passes draw unchanged state, so their multiplies must have the same inputs
   as the logic pass's under the same key; a mismatch is left alone.

   **Cars' parts are paired by position.** Numbering by order paired a part
   with whatever took its place when something was culled or the cars were
   drawn in another order: a wheel with another wheel of the same car (3.5 m
   away, so it was blended between the two and drawn in the wrong place for
   a moment) or a part with another car's (150 m, drawn unblended). The
   recorder's caller for cars' parts (`0x820D0564`) keeps the part's mesh in
   r31, so the key also holds the caller and r31; but the mesh is shared by
   cars of a model and by a car's four wheels, so within each group of
   products alike but for their order, `MatchTick` matches by position once
   a tick: first the parts alone within 8 m in both ticks (a car's body),
   then the rest at their position moved back by the nearest such part's
   movement (so a front wheel isn't taken for where the rear wheel was at
   speed). Each object keeps an identity along the pairing from tick to
   tick, which the pixel constants are paired by; their entries are dropped
   two ticks after last seen. `--log_moving_products` logs moving objects
   drawn out of step: render-only mismatches, products not blended or
   unpaired, pairings that moved unlike everything within 4 m (another
   wheel), and jumps between frames, with the recorder's caller and its
   r24-r31.
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

Three more things are blended, outside the products:

- **The rest of the per-object upload.** `sub_825953D0`'s upload (c0 x9)
  also holds the world matrix (c4-c7) and the camera's world position (c8),
  which the paint's reflections use. As c0-c3 is substituted, c4-c7 gets the
  blended world matrix if it holds the product's world matrix, and c8 the
  blended camera's position if it's within 5 cm of the tick's camera
  position; otherwise they're left alone. Without this the car's reflections
  stepped at 30 Hz against the blended car ("shifting perspective", most
  visible on the wing mirror housing in first person).
  `--blend_object_extras=false` turns it off; `--log_object_extras` logs
  how often they change and are replaced.

- **Light billboards' facing.** `sub_8209C6E0` builds the light glows' quads
  on the CPU in Draw, turned to face its camera pass. For that call alone the
  pass gets the frame's blended eye, target, up and view, so the quads face
  the camera they're drawn from (otherwise their lower edge, cut by the
  road's depth, wobbles). The blended camera is the same for every set in
  a frame, so it's blended once a Draw (`--cache_billboard_cameras`, since
  v0.4.4). The function itself is ~12% of the main thread, a lead for later
  ("DrawState::Note and AfterDraw" in `native-renderer.md`).
- **Per-object pixel constants** (`sub_82595490`, 63 registers at c0, and the
  same layout at c63: lighting, including the headlights). They hold no
  object identity, so each is paired by the camera product replayed just
  before it, and uploaded as the frame's linear blend of the two ticks.
  The upload's layout (the same at c0 and c63): c0-c3 are the view matrix
  and c4 the position of the camera the lighting is worked out against;
  light slot k (of 8, the lights nearest the object) is registers 8+k
  (position), 16+k, 24+k (direction), 32+k, 47+k and 55+k (color and
  strength among them). For the 63-register uploads:
  - All of it blends; the light slots' positions and directions are what
    move the cars' headlights' light with the cars.
    `--blend_only_light_slots` blends only those and leaves the rest as
    recorded: added while chasing the flash below, before its cause was
    found, and not needed since.
  - A slot whose light moved more than 4 m in a tick, or any of whose
    registers jumped, holds another light (passing a street lamp, the next
    one along takes its slot) and takes the tick's light
    (`--switch_light_slots`), as the game switches it. Judged by the
    relative tolerance alone, a world position may move hundreds of meters,
    so a slot taking the next lamp was blended as one light sliding 30 m
    across the car in a tick: opponents' cars flashed brighter passing
    under lamps. Before slots were switched on their own, any slot change
    rejected the whole upload, so the car's lighting stayed at the tick for
    a tick (a flicker).
  - c0-c4 blend together as a camera, or take the tick's together on a cut
    (`--blend_lighting_camera`;
    treated by component as switches, as they once were, a view matrix
    entry that changed past the tolerance while turning kept the tick's
    value while the rest blended).

  Otherwise (and for other uploads), components c1.x, c3.xyz and c4.x keep
  the newer tick's value when they jump, and a jump anywhere else means a
  wrong pairing, and the upload is left as recorded (blending two objects'
  lighting flashes headlight-colored light onto random geometry). After a vertex upload that isn't a known product
  (world geometry's matrix often isn't), the pixel constants after it
  aren't paired at all (`--strict_pixel_pairing`): pairing them by an
  earlier draw's product blended one piece of world geometry's lighting
  with another's, so buildings' lighting briefly changed.
  `--blend_pixel_constants=false` turns this off.

  Each upload blends from the **last** value recorded in the previous tick
  to this frame's, not from the tick's first. The light slots (c8-c11
  positions, c24-c27 directions; cars' headlights among them) are recorded
  on a tick's logic pass before the lights move, so there they still hold
  the previous tick's lights, and the tick's own on the render-only frames
  after (a car at -1745.33 had its headlight at -1744.61, tick 765's, on
  tick 766's first frame, and -1745.12 from the next). Pairing only uploads
  equal to the tick's first left those later frames as recorded (45-110
  uploads a frame on moving objects), so the headlights' light on car
  bodies stepped once a tick, a frame late, while the road was smooth. Now
  the stale first frame equals the previous tick's value (no change at
  amount about 0) and the frames after blend towards the new one.
  `--pixel_blend_from_latest=false` brings back the old pairing.

  This was the long-standing "headlight lighting on other cars" issue. It
  had been described as the beam's edge flickering on an opponent close and
  diagonal in it, but it was this stepping, easier to see at angles where
  the light is dimmer. Two earlier attempts went after the wrong cause and
  were dropped: matching light slots by the light in them (faded in and
  out; jumps judged per register; c4-c8 held with the lighting) made
  opponents worse, and rerunning the game's light setup with blended cars,
  light records and camera smoothed the road's lighting but not car
  bodies.

**The cockpit dials** (`docs/cockpit-dial-interpolation.md`). A needle's
world matrix is `(B * RotZ(angle)) * A * car`, turning about its spindle,
away from its origin, so blending its matrix between ticks cut across the
dial. The native dial updates hand the game the needle's tick pose (it
culls with the tick's camera) and, around the draw, its pose on the car
blended to the frame (`BlendState_SetLivePose`): the recorder's product is
replayed with that world matrix and only its camera blended. The angle
comes from the tick's gauge values, so the needles step at 30 Hz, as on
the 360; the position moves with the car.

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
| `--motion_blur` | The game's velocity-based motion blur (pixel shader `7EB16456AAD56CBA`), on by default; false leaves its one draw out, and the frame is as drawn, unblurred. Defined in `src/native/renderer.cpp` (2026-10-19, `beam-stepping`); it used to be SDK patch `0002`, for the emulated GPU only (removed: the plugin isn't shipped, and now always draws it). Its velocities come from the tick, so it smears a whole tick of motion at high frame rates; see `--motion_blur_by_tick`. |
| `--glow` | Emulated GPU only (the native renderer always draws the glow and ignores this flag): the game's light bloom (glow intensity c33.y of pixel shader `9750599DF5ED3B39`, the bloom combine), off by default there, as it leaves smears after camera switches. Patch `patches/rexglue-sdk/0001`. Turned on (with `--motion_blur=true`) when comparing the native renderer with the emulated GPU. |
| `--blend_pixel_constants` | Blend per-object lighting constants (default on). |
| `--blend_glow`, `--glow_history_every_frame` | The glow between ticks (default on; `src/interpolation/glow.cpp`): its strength and history weight blended between ticks, and its history written every frame with the weight raised to the frame's share of a tick. See the bloom under Known issues in `native-renderer.md`. |
| `--log_glow_recorder` | Every 2 s: the glow recorder's passes, history writes (the game's and added), the tick's history weight and the weights used. |
| `--log_fps` | Frames and ticks per second. |
| `--switch_light_slots`, `--blend_lighting_camera` | How the per-object lighting's light slots and camera blend (default on); see "What gets blended". |
| `--blend_only_light_slots` | Blend only the per-object lighting's light positions and directions (default off). |
| `--log_pixel_outcomes` | Every 2 s: what the lighting uploads got (blended, unpaired, not as recorded, rejected, unchanged) per frame for moving and still objects, which components differ or were rejected (with the object and its callers), frame-to-frame jumps in moving objects' uploaded lighting, an object followed for 10 frames after its lighting's camera jumps, and every 5 s one moving object's light slots traced for 4 ticks. |
| `--pixel_blend_from_latest` | Blend per-object lighting from the previous tick's last value (default on); see "What gets blended". |
| `--log_blend_state` | Per second: products published, blended, unpaired, rejected cameras with the reason, and uploads left alone. Unpaired or rejected counts above zero in steady driving mean something is stepping at 30 Hz. Each rejected camera names its source: the camera and world addresses, the handler, and the return addresses of the multiplies or copies that made it in the two ticks (`from before->now`); disassemble around those in `generated/` to see what builds it. A rejection alone isn't a visible step: compare the per-frame "camera not blended" count (uploads drawn with a rejected camera). |
| `--log_motion_blur_velocity` | Every 5 s, ~40 passes in a row: the motion blur's previous view-projection (c100 x4) as copied (logic or render-only pass, the view's +44 and +48, which of +8080/+8144 was copied, the camera's motion between the two, their translation rows) and as uploaded (substituted or as recorded, and how far the replay moved it). |
| `--log_replayed_constants` | One frame per second: every vertex constant upload replayed, whether it changed since the last frame, and whether it was substituted. |
| `--log_stepping_constants` | With `--log_slot_callers`: every 2 s, the constant uploads left as recorded that change about 30 times a second (stuck to the tick) rather than every frame, with the code that recorded them. The quickest way to find what still steps. |
| `--per_tick_blink_timers`, `--per_tick_ui_movies`, `--per_tick_play_time`, `--per_tick_texture_animations`, `--per_tick_minimap_blink` | The per-frame logic fixes (default on); see "Per-frame logic". |
| `--find_per_frame_state`, `--render_only_skip`, `--log_recording_by_pass` and others | Finding per-frame logic; see "Per-frame logic". |
| `--skip_intro`, `--attract_after_s` | Skip the logos and shorten the title screen's wait, so an attract race starts ~1 s after launch. See "The title screen" in `title-screen.md`. |
| `--log_race_changes` | When the game enters and leaves a race (the race's draw step starts or stops running; the race end frees the textures, see "Textures are never freed" in `native-renderer.md`). |
| `--find_idle_timer`, `--log_boot_modes`, `--log_file_opens` | Finding timers in menus; the boot sequence; files opened. See "The title screen" in `title-screen.md`. |
| `--log_game_messages`, `--log_slot_callers` | Message delivery per pass; which code allocates constant slots. |
| `--log_upload_owners`, `--debug_zero_owners` | Which code recorded each vertex constant upload; replay a recorder's vertex or pixel constant uploads as zeros (with `--log_slot_callers`) to see what it draws. The fastest way to identify a draw; see "World-space sprites". |
| `--log_overlay_products`, `--log_overlay_uploads` | The race overlays' camera products and uploads, as replayed. |
| `--log_world_sprites`, `--log_single_sprites` | The world-space sprites as recorded. |
| `--pair_by_object` | Pair the per-object recorder's products by position within groups alike but for their order (default on); see "What gets blended". |
| `--log_moving_products` | Moving objects drawn out of step, event by event; see "What gets blended". |
| `--motion_blur_by_tick` | The motion blur's previous view-projection (c100 x4, copied at `8213653C`) blended on render-only frames too (default on): the game copies it from the view's +8080 on logic passes and +8144 on others, the same matrix, and only the logic pass's copy was paired, so the blur's length jumped once a tick. `--log_motion_blur_velocity` logs the copies and uploads. See "Motion blur reads the 30 Hz tick" in `docs/native-renderer.md`. |
| `--camera_ids_ignore_depth_offset`, `--sticky_camera_ids` | Camera numbers and the cut check without the depth offset, and numbers kept by camera address while a projection drifts (default on); see "The headlight beam far from the origin". |
| `--double_precision_blending` | Blend objects' and cameras' positions in double and multiply only their difference (default off): the blend's own jitter far from the origin from ~1 mm to ~0.01 mm, nothing visible. |
| `--log_camera_copies` | Every 2 s: copies of cameras left unpaired between ticks, what the previous tick held at the same address (with the eye's movement, turn and scales for a renumbered one), new camera numbers and how many exist. |
| `--log_cockpit_judder` | Every 2 s: where the camera is in the frame of each part within 3 m of it (the cockpit), frame by frame, its jerk, and the ticks' own camera offsets. |
| `--log_thread_cpu` | Every 2 s: the process's and the machine's CPU use, and each thread using 5% of a core or more, by name: its share of a core (system time in brackets), the CPU it last ran on, its voluntary/involuntary context switches (many involuntary ones mean it's waiting for a core) and its minor/major page faults. Game threads are labeled with their entry point (`8258DE50 -> <the thread's own function>`). A thread with 10% or more of a core in the kernel gets its system calls sampled (25 reads of `/proc/.../syscall`, 4 ms apart: numbers, `running`, or -1 outside a call). Among CLAUDE.md's logging flags; Linux only. For finding a thread that pegs a core on slower machines (the Steam Deck). |
| `--native_cockpit_dials`, `--check_native_cockpit_dials` | The cockpit dials posed natively (default on), and their angles and poses compared with the translated code on logic passes, logged every 2 s. |
| `--log_camera_pass_records` | Which camera passes are recorded on logic and render-only passes, by message handler. |
| `--log_gpu_memory` | Every 5 s (native renderer): the GPU memory it holds by what it's for (textures, resolve destinations, images, staging, mirrors, constant blocks), and its counts of views, samplers, descriptor pools and pipelines; with it, the texture cache logs textures by time since drawn, what new textures' memory overlapped, and the textures and resolve destinations it frees. For the memory that grows from race to race; see "Textures are never freed" in `native-renderer.md`. |

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
- **The command list shim**: blending recorded constants matched between
  ticks by draw order (the `--interpolate` option). It paired the wrong
  objects whenever draws came and went, and showed up in the settings
  overlay as a buggy "interpolate" toggle; removed.
- **Pairing cars' parts by r31 alone.** It's the part's mesh, not the car:
  two cars of a model 152 m apart had the same r31, and the four wheels
  share one, so order still decided within it. Matching by position did it.
- **Pairing pixel constants by order in the frame.** Objects coming into or
  leaving view shifted the order; pairing by the preceding camera product
  fixed it.
- **The car's reflections, before c4-c8 were found.** Zeroing c100 x4
  (`821364D0`, each view's previous view-projection, which turned out to
  be the motion blur's: these tests ran with the blur off) and c14 x4
  (`820C79F8`) changed nothing visible; zeroing `82147074`/`8214717C`
  (`sub_82146BD0`) blacked out the world geometry. Treating more pixel
  constant components as switches (the most-rejected ones, c2.x, c8.y,
  c48.x...) made no visible difference: the rejections moved to the same
  registers' other components.

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

### The headlight beam far from the origin

Far from the map's origin (x ≈ -8,000 to -10,000 m), the player's headlight
beam on the road sometimes lagged the car or flickered, with interpolation
only. Fixed on `headlight-flicker` (2026-10-19): the cut check no longer
compares cameras' depth offsets (`--camera_ids_ignore_depth_offset`, on).
In a 5-minute drive far out, no square projector was refused, against 56
before; the user saw the beam steady. What was learned:

- **The projector.** The player's headlight is a projected texture: the
  render orchestrator `sub_820C5DE8` calls `sub_82597610` (return
  `0x820C6A2C`) once a view, with the eye and the beam's direction worked
  out from the car's matrix at car + 4304 (fixed local offsets), the field
  of view from the light's data (+188), aspect 1 and near 1, and copies the
  result into a per-view array (stride 592, + 144). `sub_820F34C8` (return
  `0x820F3944`) builds up to 5 more square projectors in the lights code
  (which lights isn't known). The interpolation sees a projector's copy as
  identity * camera, and products made with it (the beam on other cars'
  bodies, ~30-170 a draw) as world * camera, and blends it as a camera.
  Read back as a camera it's square: x = y = 0.8391, w 1.
- **Why it stepped.** Its depth offset, read back from the view-projection,
  is the difference of two terms that grow with the distance from the
  origin, so float rounding moves it: 0.1000 near x ≈ -1,700, 0.0991 <->
  0.1002 from tick to tick at x ≈ -8,500. Two things compared it within 1%:
  the camera numbers (CameraIds), which then flipped between two numbers
  (unpaired, drawn at the tick), and the cut check (`IsSameCameraCheck`),
  which refused the pair as a cut (not blended). Either way that frame drew
  the beam a tick ahead of the car. With `--log_blend_state`, refusals show
  as "cameras not blended" with x 0.8391. Both now use the depth scale over
  w scale (from the matrix's rotation part) and not the offset; the
  numbers also stay with their camera address while the projection drifts
  (`--sticky_camera_ids`, on), as a wide camera's aspect does (below).
- **The logic pass's frame.** Drawing runs a tick behind: the logic pass's
  draw records tick N (later frames blend against it) while its frame shows
  N-1, made by the replay blending each product back. So anything built for
  the frame at the source (as the cockpit needles are) still needs the
  replay to be given the frame's version beside the tick's
  (`BlendState_SetLivePose` for the needles' world matrices); drawing it
  only on render-only passes leaves the logic pass's frame to the replay's
  own blend.
- **Other cameras refused far out: a temporary, never drawn.** About
  once a second a group of cameras with w 1, the eye at the origin, x
  0.9-3.4 and y 0.08-0.7 (aspect drifting 2.5 to 24; depth offsets -3 to
  -23) is refused, their y scale changing 1-3% a tick (over the check's 1%
  on aspect). `--log_blend_state` now names each refused pair's camera
  and world addresses, handler and the multiply's or copy's return address
  (2026-10-19, `beam-stepping`): all of these are one multiply inside the
  c14 texture projection's builder `sub_82598768` (return `82599644`,
  sp+352 = sp+352 * sp+208, camera `7010EA90`, world `7010EB20`, handler
  `8215C770`), a temporary the builder overwrites two multiplies later
  (`825996FC`). Its right operand happens to read back as a camera. In an
  attract race, every second where only it was refused had 0 uploads
  drawn with a refused camera; the nonzero seconds came with real cuts
  (the main camera pass `826BAC90`, `7010F1C0` from the per-object
  recorder, `421E6010`). So it isn't the beam and draws nothing; it does
  keep CameraIds growing (thousands of numbers over a long drive).
- **Main camera number switches are real cuts.** The main camera pass
  `826BAC90` switches between two projections (x 0.891, y 1.584, depth
  scale 0.139 and x 1.287, y 2.288, 0.271) with the eye moving 0.5-3.9 m and
  turning 0.06-0.11 rad: view changes, correctly left unblended.
- **What the diagnostics see.** `--log_camera_copies` only covers copies of
  cameras (identity world), not object * camera products; unpaired (no
  match in the previous tick) and refused (paired, failed the cut check) are
  two separate ways to be drawn at the tick, logged by different flags
  (`--log_camera_copies`, `--log_moving_products`; `--log_blend_state`).

- **Afterwards: the beam looked like it updated at 30 Hz, sometimes.**
  Not the beam itself but the motion blur over it: the blur's previous
  view-projection (c100 x4) was blended on logic frames only, so its
  length jumped once a tick (fixed on `beam-stepping`,
  `--motion_blur_by_tick`; see "Motion blur reads the 30 Hz tick" in
  `docs/native-renderer.md`). The cameras refused far out (above) were
  ruled out first: a temporary inside `sub_82598768`, never drawn.

Tried on the way, not kept:

- **Positions blended in double** (`--double_precision_blending`, off): the
  blend's own jitter far out went from ~1 mm to ~0.01 mm, with no visible
  change to the cockpit judder, which is the game's own (Known issues).
  Kept as an option.
- **The projector given per frame**: an override of `sub_82597610` for
  `0x820C6A2C` working out the frame's projector from its placement blended
  between ticks, first built on render-only passes only (the logic pass's
  frame was still refused), then handed to the replay on every pass beside
  the tick's, never blended as a camera. Its refusals went to 0 in logs, but
  `sub_820F34C8`'s projectors were still refused and the user still saw
  flicker; the cut check fix made it unneeded (removed).
