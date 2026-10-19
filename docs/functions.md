# Known functions

Names for the generated `sub_XXXXXXXX` functions, worked out so far. The
address is the function's location in the original `default.xex`, so it also
matches Ghidra or IDA. Names are our own guesses; the game shipped without
symbols.

Status:

- **Native**: a version we wrote lives in `itc_recomp/src/` and works
  without the original code (it may still call the original when its flag
  is off, or to check itself against it).
- **Wrapped**: a version in `itc_recomp/src/` does something of its own
  and calls the original (`__imp__sub_XXXXXXXX`).
- **Patched**: the generated code was edited in place (search for
  `PATCH:`).

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_82090108` | `main` | | Guest entry point on the main thread. |
| `sub_8258DE50` | `Thread::Start` | | Entry point of the game's threads: calls vtable slot 1 of the object at start context + 8. |
| `sub_82095AF0` | | | A worker thread's loop (started through `sub_8258DE50`): waits on an event, runs jobs through function pointers, signals events; it opens the game's data files at boot. On the Steam Deck it spiked to 66-82% of a core with 15-32% in the kernel around race loads; here (2026-10-14) ~29% with ~68,000 minor page faults in 2 s while a race loads. On the Deck (`itc_041.log`) the kernel time is page faults: 37-69% of a core (19-24% kernel) with 27,000-52,000 minor faults and no major ones per 2 s, its system call samples all `running` (in a fault, not a call) but once `202` (futex: waiting). Only around loads (2-4 s each); in a race it's at 0-15%. The fix, if worth it: populate the guest memory's pages in advance (e.g. `MADV_POPULATE_WRITE` on the views), moving the cost to startup. |
| `sub_8209A760` | `App::Run` | | Main loop: calls every handler in the tick list at this+4, repeats until the quit flag at this+12 is set. One logic tick per pass, with no measured time. |
| `sub_82099D50` | `ForEachTickHandler` | | Calls each handler in a list through a member function pointer. |
| `sub_82092358` | `Renderer::Init` | | Sets the vblank interval (2 = 30 fps) via `sub_8258D568`, then subscribes renderer callbacks to numbered events (2, 3, 5, 6, 16). |
| `sub_820920D8` | `Renderer::WaitForFrame` | Wrapped | Tick handler on the main thread. Blocks until the render thread signals the frame is done (event at this+392), swaps, and starts the next frame. Frame pacing runs after it (`src/frame_pacing/renderer_hooks.cpp`). |
| `sub_82091EF8` | `Renderer::ThreadLoop` | Native | Render thread loop; started through `sub_8258DE50`. Draws one frame each time the main thread asks, as the game did, without the 2-vblank wait. |
| `sub_82091A60` | `Renderer::Present` | Wrapped | Flushes, waits for the vblank interval (`sub_8258DC58` → `sub_8258DAA8`), then swaps buffers. Event 5. Wrapped only to time it (`--log_frame_times`). |
| `sub_82091E58` | `Renderer::DrawFrame` | | Render thread: calls each draw listener in the list at renderer+28. |
| `sub_82091BC0` | | | Draw listener between the scene draw and present: resolves the frame into a front buffer (`sub_8258D998`) and waits for the GPU to go idle (`sub_8229F1B0`). See "The end of a frame" in `gpu-functions.md`. |
| `sub_820988C8` | `Scene::Update` | Wrapped | Tick handler on the scene object (0x40026FE0 at the menu). Calls the current mode object at +64 (vtable slots 6 and 2). Wrapped for `--log_boot_modes` (logs the front end's state objects at `[0x82814E2C]` and `[0x82814E34]` as they change). |
| `sub_82098940` | `Scene::Draw` | | Draw listener on the same object. Calls the mode object's draw (vtable slot 5), reading live game state. |
| `sub_8223A3F0` | `Delegate::Invoke` | | Calls the function at +8 with the argument at +4. Used for tick and draw listeners. |
| `sub_82233460` | `MemberDelegate::Invoke` | | Calls the member function at +8 on the object at +4, adjusted by +12. |
| `sub_8258D568` | | | Receives the frame interval from `sub_82092358`. |
| `sub_8258DAA8` | `VideoOutput::WaitForVblank` | Native | Waited for the 2-vblank interval; now only makes the preceding device call (`sub_822A0718`). |
| `sub_8258DD90` | `Event::Wait` | | r3 = event, r4 = pointer to timeout (-1 = forever). |
| `sub_8258DD78` | `Event::Signal` | | r3 = event. |
| `sub_822B20D8` | | | Graphics driver present bookkeeping (frame counter at +20024). Library code, not game timing. |
| `sub_825BA0C8` | `QueryPerformanceCounter` | | System library. |
| `sub_820C0128` | `Vehicle::GetSpeedKmh` | Native | Returns the float at vehicle+48 (m/s) times 3.6. |
| `sub_82093DC0` | | | First tick handler: polls input, then walks listener lists that run both game logic and the draw step (see "Separating drawing from the game tick"). |
| `sub_821062E8` | `GameMessages::Dispatch` | Native | Delivers the game's queued messages to its handlers: 0 = Update (all game logic), 1 = Draw. Skips Update on render-only passes (`src/frame_pacing/game_messages.cpp`). |
| `sub_82106280` | | | Picks the handler object a message goes to; its method 0 handles it. |
| `sub_820990F0` | `Input::Update` | Wrapped | Input manager update; skipped on render-only passes so presses aren't seen several times. |
| `sub_8210B018` | `ActionMappers::Update` | Wrapped | Updates every action mapper (`sub_8210C370` each: held, pressed, fired words and the repeat objects). Skipped on render-only passes (`--input_mappers_by_tick`, on by default), so menu input repeats count ticks; see `docs/pause-menu-fix.md`. |
| `sub_8215B4F8` | `Race::DrawStep` | Wrapped | The race's draw step (Draw message). Its running marks the pass as in a race. |
| `sub_820A3978` | `CameraPass::Update` | Wrapped | Rebuilds a camera pass's view (+0x10) from its eye, target and up vector (+0x190, +0x1A0, +0x1B0), and its view-projection (+0x90 = view * projection at +0x50). Called several times per Draw for pass 0. Wrapped for `--log_camera_pass_records`. |
| `sub_8209BD50` | `Matrix::Multiply` | Wrapped | out (r3) = a (r4) * b (r5), 4x4. Interpolation notes every multiply into a camera (see "Interpolation"). |
| `sub_825953D0` | `Recorder::RecordObject` | Wrapped | The bulk per-object recorder, ~150 calls per pass in a race: records command 7 with vertex constants 0-8: world (r5) * camera view-projection (r4) in c0-c3, the world matrix in c4-c7 (for some objects a projective matrix instead), and the camera's world position in c8. The camera is often a stack copy, not a camera pass. |
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
| `sub_820F5740` | `Minimap::Draw` | Wrapped | From the draw step part `sub_820F6068`; steps the minimap blink clock at `0x826C5AA4`/`0x826C5AA8` after drawing (see "Per-frame logic"). |
| `sub_82107910` | | Wrapped | From Draw; adds one to the play time frame counter at `0x826C5B90` (see "Per-frame logic"). |
| `sub_821109B8` | | | Turns the play time frame counter into hours, minutes and seconds (30, 1800, 108000 frames). |
| `sub_82107B90` | `Boot::StartMovie` | | Message handler: plays the next boot movie, id from the table at `0x826C5BAC` (index at `0x826C5BC8`), via `sub_820A43A8` (name table at `0x82340BEC`). See "The title screen". |
| `sub_82107C58` | `Boot::WaitForMovie` | Wrapped | Message handler, every tick while a boot movie plays; ends the step on a button press if skippable (`0x826C5BC4`) or when the movie has finished. `--skip_intro` makes it finish at once. |
| `sub_82107BF8` | `Boot::EndMovie` | | Message handler: stops the movie; the front end (Genki logo, title) follows. |
| `sub_820A4450` | `Movie::IsFinished` | Wrapped | 1 when the playing movie has finished. |
| `sub_82295508` | `CreateFile` | Wrapped | The only caller of `NtCreateFile`; r3 = path. `--log_file_opens` logs every path. |
| `sub_822221A0` | `TitleScreen::Update` | Wrapped | Counts idle ticks and starts the attract race at 600 (see "The title screen"). `--attract_after_s` shortens the wait. |
| `sub_820F4128` | `Lights::FindNearest` | Native | `src/hooks/lights.cpp` (`--native_nearest_lights`, "Lights::FindNearest" in `native-renderer.md`). Collects the lights nearest a point, nearest first; 13.1% of the main thread in an uncapped attract race (2026-10-16, "The main thread" in `native-renderer.md`). Arguments: r3 the light set owner (list at r3+216: count +0, array +8 of 272-byte entries, each's +12 → +80 a light set), r4 the output, r5 the point, r6 the most lights to keep (at most 32; 0 returned above), r9 a direction vector, f1 a merge radius, f2 the farthest squared distance, f3 the least dot product. For each light (48 bytes: position xyz at +0, 32 bytes copied out): skipped beyond f2, or when the direction to it dotted with r9 is below f3 (a cone); within f1 of a light already kept, it replaces that one if nearer, else is dropped; otherwise insertion-sorted by squared distance into a 33-entry array on its ~2 KB guest stack frame. Writes 32 bytes a light to r4 (position with w = the float at `0x82000A30`, then the light's +16..+31) and returns the count. A light set's count (+0) is little-endian (byte-swapped after `lwz`); `sub_82294730` gives each of its entries, whose +0 count says how many times the entry's one light group (count +60, lights from +76) is scanned: the group pointer isn't advanced, so a count above 1 scans the same lights again (the merge then drops the copies). It happens: in an attract race ~63 repeated scans a call, ~1,465 of the ~6,240 lights it tests a call. Hot spot: three `stfs` of the difference to r1+80, then a 16-byte `lvx` of the same spot (a failed store forward) for every light tested. Only caller: `sub_820C9170`, ~3 calls a frame, 93% of them on render-only passes. |
| `sub_820C9170` | | | Asks `sub_820F4128` for up to 10 lights near a point (r1+112) in a cone (cosine-like value from `sub_825A8978` of a setting at `[r27+11844]+5500` times a constant) of the light set at `[0x826C551C]`. Probably feeds the 8 per-object light slots of the pixel constants (see "Per-object pixel constants"; not checked). Called from `0x820E1DF0`, `0x820E6B00` and `0x820DC3B8`. |
| `sub_82294730` | | | r3 a light set, r4 an index: returns r3 plus the r4-th of the little-endian offsets at r3+4 (the count at +0 bounds r4); with r5's low byte set, 0 for an entry whose first word is 0. `sub_820F4128` passes r5 = 0. |
| `sub_8218E420` | | Patched | Post-race check; the comparison against 60 was changed to 1 to fix the black screen after quest races. |
| `sub_8211D2F8` | `UiMovie::Update` | Wrapped | r3 = movie, r4 = 0 advance a frame and draw, 1 draw only; reached through `sub_8211E1B8` from 634 call sites (the HUD, the race start countdown, the pause menus). Replayed on render-only passes, and its adding tracks handled per tick (see "Per-frame logic"; `docs/pause-menu-fix.md`, "The sub-menus' arrow"). |
| `sub_825A08E0` | `UiMovie::ApplyAnimation` | | Run by each movie update for every playing animation: finds the keys around the animation's time and applies each track with t = (time - k0) / (k1 - k0), clamped, through `sub_825A0548`. Sets values from the time, so it doesn't step by itself. |
| `sub_825A0548` | `UiMovie::ApplyTrack` | Wrapped | r3 = node, r4 = track (word +0 its property id), f1 = t, r5/r6 = the keys. Writes one word, at node + the descriptor's offset (plus `sub_8259D600` when indirect): sets it (descriptor byte +8 non-zero) or adds to it (0); nothing for types 3 and up. Adds the update doesn't put back are skipped on render-only passes (`--per_tick_ui_movie_tracks`). |
| `sub_8259FE68` | `UiMovie::TrackProperty` | | r3 = property id; returns its 12-byte descriptor in the table at `0x82970C28` (64 entries, filled on first use, byte `0x82970C20` set): +0 node offset, +4 type (0 float, 1 and 2 integer), byte +8 set/add, byte +9 indirect. |
| `sub_8259D600` | | | r3, r4: returns 16 * (r3 + 2 * (r4 + 4)); the indirect tracks' extra offset (r3 = [node + 112], r4 = [track + 12]). |
| `sub_8259D658` | `UiMovie::Child` | | r3 = node, r4 = index: returns that child (the count is at node + 112). |
| `sub_82119B80` | `UiMovie::RestorePose` | Wrapped | r3 = movie, r4 = live tree, r5 = rest tree: for each child of six tagged types (GRP recursed into; PIC, PRM, SCE, TFM, TXT) copies the rest node's fields (+12..+80, and per type +100..+136) over the live one. Called at the end of each movie update (from `8211D7CC`), so tracks' adds start from the rest pose every update. Wrapped only for `--log_pause_movie` (`WATCH_MOVIE_CALL`). |
| `sub_820DAC68` | `Car::DrawUpdate` | Wrapped | Draw's per-car update (r3 = car - 4288; it passes r3 + 4288 on as the car), on every pass: first takes `[0x82000A30]` (1.0) off car + 704 (the boost; the tick sets it again), then poses and draws the car, the cockpit dials among it (`sub_820E51D0`, `sub_820E45A8`). The player's car in cockpit view runs it twice a pass; only the first poses the dials. Wrapped only for `--find_per_frame_state` (`ITC_FRAME_STATE_WRAP`). |
| `sub_8215C780` | `Car::TickUpdate` | | The tick's car record update (logic passes; r31 = the car record, r26 = the vehicle): copies the car's world matrix into car + 16 and stores the gauge values at car + 696 (`sub_820C0128`, the speed in km/h), + 700 (`sub_820C0140`, the revs), + 704 (`[r26 + 2944]`, the boost) and + 712 (`[r26 + 6260]`). |
| `sub_820E51D0` | | | Calls `sub_820E42E8` when `[dash + 20]` is set, then `sub_820E39F8` when `[dash + 2180]` is. |
| `sub_820E42E8` | `Dash::PoseTachometer` | Native | `src/hooks/cockpit_dials.cpp` (`--native_cockpit_dials`; `docs/cockpit-dial-interpolation.md`). r3 = the dash, r4 = the car, r5 = the camera passes. Gets two angles from `sub_820D3740([dash + 2896], car, [dash])`: `a` the speedometer, `b` the tachometer. Each needle's world matrix is `(B * RotZ(angle)) * A * car + 16`, with A, B at dash + 32/+96 for `a` and + 272/+336 for `b`; drawn with `sub_820D06F0(dash + 496 / + 736, r5 + 144, world, 0, 0)`, both part `...06A0`. |
| `sub_820E45A8` | `Dash::PoseDials` | Native | Same file. r3 = the dash, r4 = the car, r5 = the camera passes. Seven needle slots k = 0-6, each drawn if `[[dash] + 8 + 12k]` and its model (`[dash + 1700]`, `1940`, `2660`, `2900`, `3620`, `3860`, `4100`) are set: stores `[dash + 5296 + 4k]` into the model + 48, angle `sub_820D3688([dash + 5328 + 4k], [[dash] + 12 + 12k], car)`, world `(B * RotZ(angle)) * A * car + 16` with A at dash + 32 + 240k and B 64 bytes on, drawn with `sub_820D06F0` (dash + 2176, 2416, 3136, 3376, 4336, 4576, 4816). In mode 5 (`[dash + 4]`) slot 1 is the turbo gauge: `[dash + 5324]` into the model, angle from car + 704: above 0, `[0x820062E0]` times it, capped at `[0x820062E0]` above 1; below the double at `0x820062D8`, `[0x820062D0]`; else value * `[0x820062CC]` * `[0x820062D0]`. |
| `sub_820D3688` | `Dial::Angle` | Native | `DialAngle` in `src/hooks/cockpit_dials.cpp`, called by the native `sub_820E45A8` (its only caller; the translated one runs only with `--native_cockpit_dials=false` or for `--check_native_cockpit_dials`). r3 = range, r4 = gauge (0-3: car + 704, 712, 716, 720), r5 = the car; returns f1, no state. `[0x82000A34]` with no range. Else from = range + 0, to = + 4, angles + 8, + 12: below from the first angle, above to the second, between `(v - from) / (to - from) * (a1 - a0)` (without `+ a0`, the game's own), all times `[0x82002948]`. |
| `sub_820D3740` | `Dial::TachometerAngles` | Native | `TachAngles` in `src/hooks/cockpit_dials.cpp`, called by the native `sub_820E42E8` (its only caller; the translated one runs only with `--native_cockpit_dials=false` or for `--check_native_cockpit_dials`). r3 = table, r4 = the car, r5 = [dash], r6 = &a, r7 = &b; no state. `a` from car + 696: `[0x82000A34]` below it; up to from (table + 0, or + 8 when `[[dash]]` isn't 0) value / from * table[16]; up to to (+ 4 or + 12) linear to table[20]; above, table[20]. `b` from car + 700: table[28] below table[24], linear over table[24..36] then table[32..44], clamped at table[44]. |
| `sub_820A2D30` | `Matrix::RotateZ` | | r3 = out, r4 = in, r5 = &angle: out = in * RotZ, rows `(c, s, 0, 0)`, `(-s, c, 0, 0)`, sine and cosine from a polynomial. |
| `sub_820A0AC0` | `Matrix::Copy` | Wrapped | r3 = destination, r4 = source, 4x4. Interpolation follows copies of cameras and products. |
| `sub_82597610` | `Projector::Build` | | r3 = out, r4 = eye, r5 = target, r6 = up (16-byte vectors), f1-f4 = field of view, aspect, near, far: out = look-at (`sub_82597378`) * perspective (`sub_825972A8`) * a fixed bias matrix at `0x8265EED0` (two `sub_8209BD50` multiplies). Called by the render orchestrator `sub_820C5DE8` (return `0x820C6A2C`) for the player's headlight once a view, and by `sub_820F34C8` (return `0x820F3944`) in a loop of up to 5. See "The headlight beam far from the origin". |
| `sub_82597378` | `Matrix::LookAt` | | r3 = out, r4 = eye, r5 = target, r6 = up: a row-vector view matrix, forward = target - eye. |
| `sub_825972A8` | `Matrix::Perspective` | | r3 = out, f1 = field of view, f2 = aspect, f3 = near, f4 = far: y scale = 1 / tan(f1 * the constant at `0x82001BE4`), x scale = y scale / aspect, depth = far / (far - near), offset -near * far / (far - near). |
| `sub_820D06F0` | `CarPart::Draw` | | r3 = the part (returns if `[part + 4]`, its model, is 0), r4 = the camera, r5 = the world matrix, r6, r7: with r7 = 0, `sub_820D04E8([part + 4], camera, world, [part + 228], r6)`. |
| `sub_820D04E8` | `Model::Draw` | | Draws a model (r31 = r3): a visibility test (`sub_825951E0`), then the per-object recorder `sub_825953D0` (return address `0x820D0564`, which pairs products by r31). Culls with the camera it's given, so a world matrix away from the tick's can be culled. |
| `sub_82099A98` | `TickHandler::Invoke` | Wrapped | r3 = the app, r4 = the handler (target at +8). Skips some tick handlers on render-only passes (`SkipTickHandlerOnRenderOnlyPass`, `src/frame_pacing/game_messages.cpp`); the diagnostics' before and after hooks (`--log_tick_handlers`, `--find_per_frame_state` and others). |
| `sub_820DC958` | `BlinkTimers::Advance` | Wrapped | Skipped on render-only passes (`--per_tick_blink_timers`; "Per-frame logic"). |
| `sub_8215DEA0` | `RaceOverlays::Run` | Native | The race overlays (25 objects at r3; the marker above opponents): their update (vtable slot 4) on logic passes only, their recording (slot 5) every pass with the logic pass's cameras put back, in the original's stack frame so their products pair. See "World-space sprites". |
| `sub_821594B8` | | Wrapped | The rival intro cutscene's Update handler: runs on logic passes; on render-only passes only its logo's draw (`sub_82211A28`) is repeated (`--redraw_rival_logo`; "The rival logo at the race start"). |
| `sub_82211A28` | | Wrapped | Draws the rival's logo; noted during the cutscene for the render-only redraw. |
| `sub_82128750` | | Wrapped | World-space sprite recorder (r3 = sprite list, r4 = camera; the light trails): corners blended around the original ("World-space sprites"). |
| `sub_821281A8` | | Wrapped | A single world-space sprite's recorder: its corners blended between ticks (`--blend_world_sprites`; `--log_single_sprites`). |
| `sub_821379D8` | `Glow::Record` | Wrapped | The glow (bloom) recorder in races: strength and history weight blended between ticks, history written every frame (`--blend_glow`, `--glow_history_every_frame`; `src/interpolation/glow.cpp`). |
| `sub_8209C6E0` | `Billboards::BuildVertices` | Wrapped | r3 = billboard set (+0 the camera pass it faces), r4 = command list, r5 = the sprites: each light's quad turned to face that camera; the camera blended once a frame (`--cache_billboard_cameras`). |
| `sub_82487C48` | `MoviePlayer::OpenFile` | Wrapped | The movie player's file open (r3 = player, through a vtable); logged with `--log_file_opens`. |
| `sub_820AE958` | | Wrapped | The render thread's command that starts the boot movie; `--skip_intro` waits for it to return (ending the movie earlier stalled the boot; "The title screen"). |
| `sub_824657E0` | `AudioRenderDriver::Register` | Native | `src/audio/native_audio.cpp` (`--native_audio`; `docs/native-audio.md`). r3 = the driver object (callback argument +12, handle +16), r4 = the mixer callback (0 only unregisters): registered with our audio output instead of the kernel's. |
| `sub_82465870` | `AudioRenderDriver::Submit` | Native | Same file. r4 = the mixed buffer (`sub_8245F718` gives its descriptor; samples at +8), handed to our audio output. |
| `sub_824658C8` | `AudioRenderDriver::Destroy` | Wrapped | Same file. Unregisters our handle first, so the original doesn't pass it to `XAudioUnregisterRenderDriverClient`. |
| `sub_825A7410` | `memcpy` | Wrapped | The library's memcpy (r3 destination, r4 source, r5 size): copies into the physical views recorded for the native renderer's guest memory mirrors (`src/native/buffers.cpp`). |
| `sub_8229E3F0` | `Device::Kick` | Native | `src/native/gpu_link.cpp`. r3 device, r4 (size in words, address) pairs, r5 their count. In exclusive mode the kicked buffers are applied directly instead of written to the ring; the original otherwise. `--native_renderer_log_gpu` logs. |
| `sub_8229E848` | `Device::DeferredKick` | Native | Same file. Queued kicks while GPU interrupts were outstanding; in exclusive mode every kick completes at once, so it kicks straight away. |
| `sub_8229DEA0` | `Device::OnInterrupt` | Wrapped | The library's interrupt callback (r3 source: 0 vblank, 1 the command stream's INTERRUPT); logged with `--native_renderer_log_gpu`. |
| `sub_822B6BF8`, `sub_822B6A78`, `sub_822B6780` | | Wrapped | The tiling worker's callbacks and job; logged with `--native_renderer_log_gpu`. |
| `sub_8229EF48` | | Wrapped | Ends the command segment when the buffer is full; counted for `--native_renderer_check_draw_writes` (`src/native/draw_packets.cpp`). |
| `sub_825BE708` | `DrawIndexedVertices` | Native | Exclusive mode, without its packets (`src/native/draw_packets.cpp`). |
| `sub_822A6738`, `sub_822A67B0` | `Resource::AddRef`, `Resource::Release` | Native | `src/native/refcount.cpp`: reference counts without the library's global lock, skipped for the vertex shader already bound; the original with `--refcount_global_lock`. |
| `sub_822A8808` | `Device::SetVertexShader` | Wrapped | Same file: marks a rebind of the bound shader, so `AddRef` and `Release` skip it. |
| `sub_822AE340` | | Wrapped | `src/native/replay_hooks.cpp`: keeps the patched copy of a vertex shader's microcode (made when the GPU may still use the original) for the native renderer. |
| `sub_825BD8F0`, `sub_825BDDA0` | `BeginVertices`, `EndVertices` | Wrapped | Same file: an inline vertex draw noted, then handed to the native renderer (in commands 15 and 19). |
| `sub_8258F5C0` | | Wrapped | Same file: the single-command items' command 15 (the helper object's draw), marked as command 15 so the native renderer draws its inline vertices. |
| `sub_8258D998` | `VideoOutput::ResolveFrontBuffer` | Wrapped | Same file: resolves render target 0 into a front buffer at the frame's end; the native renderer shows that target ("The end of a frame" in `gpu-functions.md`). |
| `sub_822AFA58` | `Device::Resolve` | Wrapped | Same file: resolves made outside command 25 (command 33's, one per tile) passed to the native renderer. |
| `sub_8209CC38` | | Wrapped | The light billboards' batch (calls `sub_8209C1A0`, `sub_8209C6E0`; r4 the light list); for `--find_per_frame_state` and `--watch_lights`. |
| `sub_82133960` | | Native | A draw step part, rewritten as the original to name each view's draw for `--find_per_frame_state`: for the 15 per-view objects at `0x826C5D80` with an active camera pass and +52 clear, calls vtable slot 0, clears +8056 and +48, flips +8060. |
| `sub_82096340` | | Wrapped | A tick handler thunk (vtable slot 1 of r3); names its target for `--find_per_frame_state`. |
| `sub_82092968` | | Wrapped | Runs the app's component lists (r4's low word a component; its vtable slot 1); named for `--find_menu_selection`. |
| `sub_8220C1E0`, `sub_8210C370` | `MenuList::Update`, `ActionMapper::Update` | Wrapped | Logged for `--log_pause_menu` (`src/debug/pause_menu_log.cpp`). |
| `sub_8211C7E8`, `sub_821BE5A8`, `sub_8220D040`, `sub_8211DAC8`, `sub_8211D978`, `sub_8211DE38`, `sub_8211DDD8`, `sub_8211DF20`, `sub_8220BA68` | | Wrapped | The pause sub-menus' calls on their UI movie (setting animation targets and the children's +80 flags, starting animations, switching sub-screens); logged for `--log_pause_movie` (`src/debug/pause_movie_log.cpp`). |
| `sub_8258FBD0` (1), `sub_8258FC30` (2), `sub_8258FC90` (3), `sub_8258FCE0` (4), `sub_8258E9F8` (5), `sub_8258EA20` (6), `sub_8258FF50` (8), `sub_8258FFA0` (9), `sub_8258FFF0` (10), `sub_82590040` (11), `sub_825900B8` (12), `sub_8258EA58` (13), `sub_82590108` (15), `sub_825901B0` (16), `sub_82590268` (17), `sub_825902E0` (18), `sub_82590360` (19), `sub_825903D8` (20), `sub_8258EAD0` (21), `sub_82591798` (22), `sub_82590428` (23), `sub_82590478` (24), `sub_825904C8` (25), `sub_82590620` (26), `sub_82590698` (27), `sub_8258EAE0` (28), `sub_82590768` (29), `sub_82590860` (30), `sub_825908E0` (31), `sub_82590968` (32), `sub_825909E8` (33), `sub_82590A68` (34), `sub_82590AD0` (35) | `Command::*` | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" here and `gpu-functions.md`. |
| `sub_822AE1D0`, `sub_822ADE30`, `sub_822ADEF0`, `sub_822AE070`, `sub_822ADC08` | | Wrapped | The draw's packet writers (r3 the device: shader constants, register blocks, fetch constants, HiZ and surface set-up; `NATIVE_CHECKED_HELPER`, `src/native/draw_packets.cpp`): what they change marked for the native renderer; checked with `--native_renderer_check_draw_writes`. |
| `sub_825BE300`, `sub_825BDDC0` | `DrawVertices`, `BeginIndexedVertices` | Wrapped | Logged when called outside the commands known to make them (`LOG_OUTSIDE_WRAPPER`). |
| `sub_825924E0`, `sub_82592500`, `sub_82592520`, `sub_8258CF10`, `sub_8258CEB0`, `sub_8258CEE0`; `sub_82591B30`, `sub_820988B8` | | Wrapped | The input processors' button tests, and the pads' values (`sub_82591B30` a logical pad's, `sub_820988B8` a physical pad's): logged by caller and pass type for `--log_input_reads` (`ITC_INPUT_READ`, `ITC_INPUT_VALUES`). |
| `sub_8210BA68`, `sub_8210BE70`, `sub_8210BED8`, `sub_8210BF40`, `sub_8210BFA8`, `sub_8210C010`, `sub_8210BA28` | | Wrapped | The action mappers' read slots (vtable `0x82007F78`; r3 the mapper, r4 the action id 151-180); logged for `--log_pause_menu` (`PAUSE_MENU_READ_SLOT`). |
| `sub_8259FA00`, `sub_8211CD90`, `sub_8259DAC0`, `sub_8211B4D0`, `sub_8211B080` | | Wrapped | The UI movie update's callees besides its tracks (and `sub_82119B80` above), watched for writes for `--log_pause_movie` (`WATCH_MOVIE_CALL`). |
| `sub_8258E5E0`, `sub_8258ED78`, `sub_8259D1A8`, `sub_82094B10`, `sub_821184E8`, `sub_82118FB0`, `sub_82118C18` | | Wrapped | Named in `--find_menu_selection`'s log (`ITC_MENU_SCAN_WRAP`): `Scene::Update`'s two screen calls and others. |
| `sub_8209F8A0`, `sub_8209F900`, `sub_820C5DE8`, `sub_820F6068`, `sub_82130978`, `sub_8209F7A8`, `sub_820A0958`, `sub_820AA1A8`, `sub_820AB560`, `sub_820C4EB8`, `sub_820C4FB8`, `sub_820DAA38`, `sub_820DAAC8`, `sub_820DAB10`, `sub_820FFD88`, `sub_82593FC0`, `sub_82597118`, `sub_82597518`, `sub_82597610`, `sub_82598768`, `sub_820C5C88`, `sub_820D4048`, `sub_820D72D8`, `sub_820D92A0`, `sub_820D9B68`, `sub_820D9C50`, `sub_820D9EA0`, `sub_820DAB58`, `sub_820DBFF8`, `sub_820DC498`, `sub_820DCBC0`, `sub_820DDA60`, `sub_820DDE58`, `sub_820E2E68`, `sub_820E2EE8`, `sub_820E4140`, `sub_820E53F0`, `sub_820E5A68`, `sub_820E6A40`, `sub_820E7328`, `sub_820E8908`, `sub_820ECE28` (and `sub_820DAC68` above) | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |

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

## Known data

| Address | What | Notes |
|---|---|---|
| vehicle+0x30 | Speed (float, m/s) | Read by `Vehicle::GetSpeedKmh`. |
| vehicle+0xD0 | Vehicle world matrix | 4x4 row-major: rotation rows, then position row (w = 1). The car's forward axis is -row 0. |
| vehicle+0xCF0 | Copy of the world matrix | Identical every tick; not yet known which one drawing reads. |
| `0x826BAC10` | Camera view matrix | Pass 0's view (+0x10). The camera position is -t·Rᵀ. In the car's frame the views sit at (-5.48, 1.34, 0) for chase, (-0.44, 0.67, 0) for bumper and (-0.26, 0.85, -0.38) for cockpit. Writing it has no effect while driving: Draw rebuilds it from the eye and target. |
| `0x826BAC00` | Camera pass array | 21 objects of 592 bytes: view +0x10, projection +0x50, view-projection +0x90, eye +0x190, look-at target +0x1A0, up +0x1B0. Pass settings at +564 to +588, active flag at +588. Pass 0 is the main 1280x720 view; pass 10 a 1024x1024 reflection and 11 a 512x512 shadow map, both rebuilt in Draw. |
| `[0x82814E34]` | Title screen object | +4 state (5 = start the attract race), +36 idle ticks. See "The title screen". |
| car + 16 | The car's world matrix, for drawing | Copied in by `sub_8215C780` each tick; the cockpit dials' parent. |
| car + 696, + 700, + 704 | Speed (km/h), revs, boost | Set each tick by `sub_8215C780`; the cockpit dials read them (`sub_820E42E8`, `sub_820E45A8`). `sub_820DAC68` takes 1.0 off + 704 every pass. |
| `0x70180000` | Main thread stack | During Draw, holds temporary world matrices and camera copies passed to the recorders (for example 0x7018E640). Not render state. |

## The title screen

From launch, with no input (times as measured here):

| Time | What | Driven by |
|---|---|---|
| 0-9.5 s | Ubisoft logo movie (`UBI_LOGO.wmv`) | The boot sequence's message handlers `sub_82107B90` (start), `sub_82107C58` (wait, every tick), `sub_82107BF8` (end). The movie decodes on a worker thread (`sub_820AE5D0`) and plays at its own rate. |
| 9.5-15.7 s | Genki logo (an image) | UI animation counters, ~190 ticks. |
| 15.7-18.4 s | The title logo animating in | UI animation counters, ~80 ticks. |
| 18.4-38.4 s | Title screen, "press start" | `sub_822221A0`: idle ticks at +36 of `[0x82814E34]`, attract race at 600. |

`sub_822221A0` (the title screen's update) checks four controllers for a
button (152) and otherwise adds one to +36 of the object at `[0x82814E34]`.
At 600 ticks (20 s) it sets the state at +4 to 5 (via 8), which starts an
attract-mode race (AI cars, no input). After the race (~60 s) the game goes
back to the title screen, and the next race follows ~22 s later. The front
end's state objects are at `[0x82814E2C]`, `[0x82814E30]` and
`[0x82814E34]`, created when the movie ends.

Options for automated runs (both off by default):

- `--skip_intro`: the movie step reports the movie finished at once
  (`sub_820A4450` returns 1 inside `sub_82107C58`), and menus run without
  the 30 Hz tick wait until the first race, so the tick-counted Genki logo
  and title animation pass in a fraction of a second.
  Failing the movie file's open instead makes the game show the dirty disc
  error.
- `--attract_after_s=<n>`: the title screen's wait becomes n seconds: after
  a tick that counted, the counter jumps to 599.

With `--skip_intro --attract_after_s=1` the attract race starts about 1 s
after launch (21 s with only `--attract_after_s=1`, 39 s with neither).

How these were found: `--find_idle_timer` (`src/debug/idle_timer_scan.cpp`)
compares guest memory every 30 logic passes outside races, logs words that
step steadily like a timer (an integer by 30 or 60 ticks or about 1000 ms,
a float by about 1 s or 30 or 60), and then logs which tick handler or
message handler changes them; the counter at `01A70024` counted to 600.
`--log_boot_modes` logs the front end's state objects as they change and
each tick handler and message handler the first time it runs, which showed
the boot sequence's handlers. `--log_file_opens` and screenshots
(`spectacle -b -n -f -o <file>`) named the phases.

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
| UI movies' adding animation tracks (the pause sub-menus' arrow slid ~5x as far): they write into the movie's nodes, outside the replayed bytes | `sub_825A0548`, inside `sub_8211D2F8` | A workaround for frame rates above 30: on render-only passes, adds whose target the update doesn't put back to the rest pose (`sub_82119B80`) are skipped (`--per_tick_ui_movie_tracks`; `docs/pause-menu-fix.md`, "The sub-menus' arrow") |
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
logo (drawn from Draw) doesn't. Found
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
  overlays). On logic passes the override runs the original, and a
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
| `--motion_blur` | The game's velocity-based motion blur (pixel shader `7EB16456AAD56CBA`), skipped by the emulated GPU when false (`run.sh` did so until the native renderer, which always draws it and ignores this flag). Its velocities come from the tick, so it smears a whole tick of motion at high frame rates. Patch `patches/rexglue-sdk/0002`. |
| `--glow` | Emulated GPU only (the native renderer always draws the glow and ignores this flag): the game's light bloom (glow intensity c33.y of pixel shader `9750599DF5ED3B39`, the bloom combine), off by default there, as it leaves smears after camera switches. Patch `patches/rexglue-sdk/0001`. Turned on (with `--motion_blur=true`) when comparing the native renderer with the emulated GPU. |
| `--blend_pixel_constants` | Blend per-object lighting constants (default on). |
| `--blend_glow`, `--glow_history_every_frame` | The glow between ticks (default on; `src/interpolation/glow.cpp`): its strength and history weight blended between ticks, and its history written every frame with the weight raised to the frame's share of a tick. See the bloom under Known issues in `native-renderer.md`. |
| `--log_glow_recorder` | Every 2 s: the glow recorder's passes, history writes (the game's and added), the tick's history weight and the weights used. |
| `--log_fps` | Frames and ticks per second. |
| `--switch_light_slots`, `--blend_lighting_camera` | How the per-object lighting's light slots and camera blend (default on); see "What gets blended". |
| `--blend_only_light_slots` | Blend only the per-object lighting's light positions and directions (default off). |
| `--log_pixel_outcomes` | Every 2 s: what the lighting uploads got (blended, unpaired, not as recorded, rejected, unchanged) per frame for moving and still objects, which components differ or were rejected (with the object and its callers), frame-to-frame jumps in moving objects' uploaded lighting, an object followed for 10 frames after its lighting's camera jumps, and every 5 s one moving object's light slots traced for 4 ticks. |
| `--pixel_blend_from_latest` | Blend per-object lighting from the previous tick's last value (default on); see "What gets blended". |
| `--log_blend_state` | Per second: products published, blended, unpaired, rejected cameras with the reason, and uploads left alone. Unpaired or rejected counts above zero in steady driving mean something is stepping at 30 Hz. |
| `--log_replayed_constants` | One frame per second: every vertex constant upload replayed, whether it changed since the last frame, and whether it was substituted. |
| `--log_stepping_constants` | With `--log_slot_callers`: every 2 s, the constant uploads left as recorded that change about 30 times a second (stuck to the tick) rather than every frame, with the code that recorded them. The quickest way to find what still steps. |
| `--per_tick_blink_timers`, `--per_tick_ui_movies`, `--per_tick_play_time`, `--per_tick_texture_animations`, `--per_tick_minimap_blink` | The per-frame logic fixes (default on); see "Per-frame logic". |
| `--find_per_frame_state`, `--render_only_skip`, `--log_recording_by_pass` and others | Finding per-frame logic; see "Per-frame logic". |
| `--skip_intro`, `--attract_after_s` | Skip the logos and shorten the title screen's wait, so an attract race starts ~1 s after launch. See "The title screen". |
| `--log_race_changes` | When the game enters and leaves a race (the race's draw step starts or stops running; the race end frees the textures, see "Textures are never freed" in `native-renderer.md`). |
| `--find_idle_timer`, `--log_boot_modes`, `--log_file_opens` | Finding timers in menus; the boot sequence; files opened. See "The title screen". |
| `--log_game_messages`, `--log_slot_callers` | Message delivery per pass; which code allocates constant slots. |
| `--log_upload_owners`, `--debug_zero_owners` | Which code recorded each vertex constant upload; replay a recorder's vertex or pixel constant uploads as zeros (with `--log_slot_callers`) to see what it draws. The fastest way to identify a draw; see "World-space sprites". |
| `--log_overlay_products`, `--log_overlay_uploads` | The race overlays' camera products and uploads, as replayed. |
| `--log_world_sprites`, `--log_single_sprites` | The world-space sprites as recorded. |
| `--pair_by_object` | Pair the per-object recorder's products by position within groups alike but for their order (default on); see "What gets blended". |
| `--log_moving_products` | Moving objects drawn out of step, event by event; see "What gets blended". |
| `--camera_ids_ignore_depth_offset`, `--sticky_camera_ids` | Camera numbers and the cut check without the depth offset, and numbers kept by camera address while a projection drifts (default on); see "The headlight beam far from the origin". |
| `--double_precision_blending` | Blend objects' and cameras' positions in double and multiply only their difference (default off): the blend's own jitter far from the origin from ~1 mm to ~0.01 mm, nothing visible. |
| `--log_camera_copies` | Every 2 s: copies of cameras left unpaired between ticks, what the previous tick held at the same address (with the eye's movement, turn and scales for a renumbered one), new camera numbers and how many exist. |
| `--log_cockpit_judder` | Every 2 s: where the camera is in the frame of each part within 3 m of it (the cockpit), frame by frame, its jerk, and the ticks' own camera offsets. |
| `--log_thread_cpu` | Every 2 s: the process's and the machine's CPU use, and each thread using 5% of a core or more, by name: its share of a core (system time in brackets), the CPU it last ran on, its voluntary/involuntary context switches (many involuntary ones mean it's waiting for a core) and its minor/major page faults. Game threads are labeled with their entry point (`8258DE50 -> <the thread's own function>`). A thread with 10% or more of a core in the kernel gets its system calls sampled (25 reads of `/proc/.../syscall`, 4 ms apart: numbers, `running`, or -1 outside a call). On in `run-debug.sh`; Linux only. For finding a thread that pegs a core on slower machines (the Steam Deck). |
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
  (`821364D0`, each view's previous view-projection) and c14 x4
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

## Performance

**Historical: the emulated GPU (2026-10-13).** This is what limited the
frame rate before the native renderer and native audio. Neither the
emulated GPU nor the Audio Worker runs any more by default, so none of it
describes the game as `run.sh` runs it now. For the current limit (the
render thread's own work), see "Performance: the render thread" in
`native-renderer.md`.

Measured 2026-10-13 in a race (9800X3D, RX 7900), with `--log_frame_times`
and `perf record -e cycles:u -g` over the whole run, cut to the race by
the samples in our blending code.

- **The render thread is the limit**: in busy areas it takes 7.5-7.8 ms a
  frame (129-134 fps) while the main thread's pass takes ~3.3 ms and then
  waits. Its CPU time is within 0.1 ms of its wall time, so it never
  sleeps.
- **About half of the render thread's time is a busy-wait on the emulated
  GPU.** `sub_8229D8A0` (46% of its samples) and `sub_825B9A60` (7%, it
  only reads a tick count from the processor block, r13+256 → +332) are
  the game's graphics library waiting on the GPU. `sub_8229D8A0` spins a
  few `db16cyc`, reads the tick count, and gives up after 5000 ticks
  (`sub_822ACAA8`). It's called in loops from four functions; the one in
  `sub_8229E908` waits for space in the ring buffer (see "Waits on the
  GPU" in `gpu-functions.md`). `perf` can't see callers, since the
  recompiled code has no frame pointers.
- **The emulated GPU's thread ("GPU Commands") is busy, not idle**: ~70% of
  a core in user code during the race, nearly all of it command processing
  rather than its idle spin. Its biggest costs are per-register work
  (`VulkanCommandProcessor::WriteRegister` 7.5%,
  `CommandProcessor::WriteRegister` 7.3%, `RegisterFile::GetRegisterInfo`
  4%, `ExecutePacketType0` 3.7%), `pthread_mutex_lock` 3.9%, binding
  updates, and ~3% looking up cvars by name (`cvar::HasNonDefaultValue`,
  a string-keyed hash table, in the draw path). The Vulkan driver itself is
  ~12% of it.
- Our blending is ~4-10% of the render thread (`BlendState_ReplayConstants`
  and the upload wrapper).
- The "Audio Worker" thread spins at ~80% of a core
  (`pthread_mutex_trylock`) the whole time: the SDK's polling wait, not
  audio work. Measured since: it didn't cost frame rate on this PC, and
  our own audio output replaced it (see `native-audio.md`).

So the frame rate in busy areas is set by how fast the emulated GPU
processes the game's commands; the render thread mostly waits for it.

## Known issues

- **Menus run at 30 fps** (the front end's; the pause menu in a race runs
  at the race's frame rate since `pause-menu`, its input repeat fixed by
  updating the action mappers once a tick, see `docs/pause-menu-fix.md`):
  they record their UI and read input during Update, so render-only passes
  made them flicker and repeat presses. Unlocking them would mean skipping
  Update only for the handlers that run game logic.
- **The race HUD updates at 30 fps.** Accepted for now; its animations
  advance once per tick (see "Per-frame logic").
- **More per-frame logic may exist.** Anything that runs too fast or
  flickers only with frames uncapped is probably another case; see
  "Per-frame logic" for how they were found.
- **Some uploads still step on purpose**: the shadow lookup matrices (c70 and
  c74, orthographic, kept at the tick to match the shadow map), c14 x4 (a
  texture projection) and c100 x4 (zeroing either changes nothing visible).
  Nothing visible steps in testing; see
  "Finding what still steps" if something does.
- **A piece of geometry flickers while the car moves**, at native 30 fps
  too, so not interpolation: probably the game's own effect (a broken light).
- **The cockpit judders left to right far from the map's origin**
  (x ≈ −8,000 to −10,000 m), with interpolation off too (seen under Wine,
  uncapped, `--interpolate_draw_state=false`, 2026-10-19), so it's the
  game's own: its per-tick camera offset from the car wobbles by 1–4 mm a
  tick out there (`--log_cockpit_judder`), the recurring 3.9 mm maximum
  being 2⁻⁸ m, a float step at that distance. Our blend's own jerk is
  ~0.01 mm with `--double_precision_blending`. Left as is: the eye/target/up
  the game's camera pass stores are float too, and smoothing the camera's
  offset from the car across ticks would make head movement lag the car.
- **The player's headlight beam on the road can jump in very sharp turns.**
  Reported before the light slot fixes (frame lag, switching, 4 m rule) and
  not rechecked since; the release notes still list it.
- **Projective matrices from `0x82596D9C`** (the per-object recorder's
  other caller seen moving in races) can't be split into position and
  rotation, so they're drawn at the tick (`--log_moving_products` lists them
  as not blended, moving 1-8 m a tick). What they draw isn't known yet.
- **One tick of latency**: drawing shows between the previous and the
  current tick, as usual for this kind of interpolation.
