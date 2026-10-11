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
