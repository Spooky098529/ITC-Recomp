# Known functions

Names for the generated `sub_XXXXXXXX` functions, worked out so far. The
address is the function's location in the original `default.xex`, so it also
matches Ghidra or IDA. Names are our own guesses; the game shipped without
symbols.

"Rewritten" means a readable replacement lives in `itc_recomp/src/hooks/` and
overrides the generated code. "Patched" means the generated code was edited in
place (search for `PATCH:`).

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_82090108` | `main` | | Guest entry point on the main thread. |
| `sub_8209A760` | `App::Run` | | Main loop: calls every handler in the tick list at this+4, repeats until the quit flag at this+12 is set. One logic tick per pass, with no measured time. |
| `sub_82099D50` | `ForEachTickHandler` | | Calls each handler in a list through a member function pointer. |
| `sub_82092358` | `Renderer::Init` | | Sets the vblank interval (2 = 30 fps) via `sub_8258D568`, then subscribes renderer callbacks to numbered events (2, 3, 5, 6, 16). |
| `sub_820920D8` | `Renderer::WaitForFrame` | | Tick handler on the main thread. Blocks until the render thread signals the frame is done (event at this+392), which locks logic to the frame rate. |
| `sub_82091EF8` | `Renderer::ThreadLoop` | Rewritten | Render thread loop; started through `sub_8258DE50`. Now draws frames until the next 30 Hz tick is due (`src/frame_pacing/`). |
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
| `sub_820C0150` | `Vehicle::GetDistancePerFrame` | | Speed (m/s) times a hard-coded 1/30 s. |
| `sub_8210EDB0` | | | Wind sound; scales with speed. |
| `sub_82152CD8` | | | Camera shake; scales with speed. |
| `sub_8213F530` | | | Parent of the speed trail object (switches mode at about 88 km/h). Calls `sub_8213F2E0`, `sub_8213F140` and `sub_8213F4B0`. |
| `sub_8213D438` | | | Resets the trail history; runs on a camera switch. |
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

With `src/frame_pacing/`, step 3 repeats until the next tick is due (30 Hz,
or capped by `--max_fps`), so ticks and frames are no longer one to one.
`--log_fps=true` logs both rates once a second.

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
| `0x826BAC10` | Camera view matrix | Global in the game's data section; 4x4 row-major view matrix. The camera position is -t·Rᵀ. In the car's frame the views sit at (-5.48, 1.34, 0) for chase, (-0.44, 0.67, 0) for bumper and (-0.26, 0.85, -0.38) for cockpit. Drawing does not read it (raising it 3 m between ticks changed nothing). |
| `0x826BAC00` | Camera pass array | 21 objects of 592 bytes. Pass settings at +564 to +588, active flag at +588. |

## Render command recording

The game records render commands during the logic tick, on the main thread,
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
  count; arg2 is stage << 24 | type << 16 | start << 8 | count. Vertex
  constants starting at 9 with 5 registers are the per-object transforms
  (thousands a second in a race); 0 with 9 registers carries per-pass
  values.

### Recorders

- `sub_82595550`, `sub_82146BD0` and `sub_820CECA0` record draws with a
  command 7 (vertex constants 9, 5 registers). `sub_82595550` runs once a
  tick in a race and its uploads aren't in the replayed list, so the bulk
  per-object recorder is still to be found.
- `sub_825953D0` (vertex constants 0, 9 registers) and `sub_82595490` (pixel
  constants 0, 63 registers) record per-pass constants.

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
buckets) and the sequence before a second run makes re-running the draw
step safe: with both runs every tick, a race looked normal
(`--debug_redraw_twice`, `src/frame_pacing/draw_step.cpp`).
