# Known functions

Names for the generated `sub_XXXXXXXX` functions, worked out so far. The
address is the function's location in the original `default.xex`, so it also
matches Ghidra or IDA. Names are our own guesses; the game shipped without
symbols.

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

The table is being split by subsystem into `docs/functions/`, one file
per area, each with the same columns, one function a row, and these
status definitions copied at its top (change them in every file).
Rows not yet moved stay in the table below. So far:

- [`system-services.md`](system-services.md):
  sign-in, profile, dialogs, sessions and achievements, Xbox Live,
  the player's music (XMP), network and system link, voice chat,
  notifications, the loader and start-up, console settings (B5 in
  `docs/rexglue-removal.md`).
- [`files-and-saves.md`](files-and-saves.md): the file calls, the
  game's file layer and archives, the C runtime's I/O, the save slots
  (B4).
- [`audio.md`](audio.md): the audio library's render driver client, its
  XMA layer and the voice category volume (B2).
- [`sprites.md`](sprites.md): the camera-facing sprites: the race
  overlays' sprite lists (trails, particles), single sprites and screen
  rectangles, and the light glows' billboards (B7).
- [`race-overlays.md`](race-overlays.md): the race overlays' manager
  and update side: the overlays per car, their views of the vehicles,
  and the trails, smoke, sparks, particles, light sprites and the marker
  above opponents fed from the cars' wheels and bodies (B7; their
  recordings are in `sprites.md`).
- [`command-list.md`](command-list.md): the graphics library's command
  list the recorders write render commands into: its set-up and flip,
  opening and closing chunks, allocating slots, vertices, bytes and
  resolves (B7).
- [`c-runtime.md`](c-runtime.md): the C runtime and math library as seen
  in play: math functions and their floating-point helpers, string,
  memory and character helpers, number parsing and formatting, and the
  pure parts of its start-up, and the compiler's array constructor and
  destructor helpers (B7; the heap and what calls kernel imports aren't
  in it).
- [`ui-movies.md`](ui-movies.md): the UI movie library (the HUD's, the
  menus' and the pause screens' 2D movies): loading and fixing up movie
  files, applying animations' tracks to nodes, and drawing the node tree
  as quads, with the game's movie update and rest pose (B7; the rest of
  the game's `UiMovie::` side is in `ui-movies-game.md`).
- [`ui-movies-game.md`](ui-movies-game.md): the game's side of the UI
  movies (`UiMovie::`): loading and unloading movies and the table of
  them by id, node lookups and setters, the animation queue and the
  looping animations, the movies' texts and the text list's draw (B7).
- [`vehicle-accessors.md`](vehicle-accessors.md): the small accessors on
  the vehicles, their wheels, rigid bodies, collision objects and
  contacts, the scene's object pools, the race data's stages and racer
  records and the players' save data, with the vector library they use
  (B7; the physics itself isn't in it).
- [`matrix-math.md`](matrix-math.md): the matrix, vector and quaternion
  math: the 4x4 matrix copy, multiply, inverse and turns and a vector
  transformed by a matrix (`8209B000`-`820AC000`), and the matrix and
  quaternion half of the physics' vector library (`82293598`-`82293F10`)
  (B7).
- [`car-physics.md`](car-physics.md): the cars' physics: the car data
  derived at load, the vehicle's physics set-up, the physics step
  (ground, suspension, steering, tires, engine, gearbox, drivetrain,
  brakes, aerodynamics), the rigid body's integration, and the
  collision world's objects, pairs, update and surface settings (B7;
  the narrow phase and shapes are in `collision.md`).
- [`heap.md`](heap.md): memory allocation: the C runtime's allocators,
  LocalAlloc and XMemAlloc, the process heap, VirtualAlloc and physical
  memory, routed to the native allocation layer (B7's first step,
  `../native-memory.md`); the C runtime's heap above it read in full by
  the B7 loop (2026-10-22).
- [`collision.md`](collision.md): the collision world's narrow phase and
  shapes: the shape record each collision object carries, the box shape
  (placed, moved, its corners, faces and edges), a pair's next check
  time, its separation, impact and push-out tests and its response (B7;
  the world, its objects and pairs are in `car-physics.md`).
- [`collision-world.md`](collision-world.md): the collision world's
  owner (its load, tick, reset and unload) and the track's collision:
  the course's mesh and route files loaded, the queries the world calls
  (walls, ground) and the ground probe the wheels use, the mesh's
  lookups and the track positions they keep (B7; the course's route
  queries are a lead).
- [`sound-cues.md`](sound-cues.md): the game's sound manager (sound
  bank pairs loaded and released, cues played as sounds with a 3D
  emitter each, stopped and removed, the listener and the categories)
  and its thin wrapper over XACT and X3DAudio, with the libraries'
  entry points it calls (B7; the rest of XACT and X3DAudio is a lead).
- [`xact.md`](xact.md): XACT, the 360's sound library linked into the
  game, as far as the game's wrapper reaches it in play: the engine
  (start-up, DoWork, the audio callback, categories), its global
  settings and variables, sound banks, wave banks, cues, sounds and
  faders, and where it meets XAudio and the kernel (B7; the tracks,
  waves and voices below are a lead).
- [`action-mappers.md`](action-mappers.md): the action mappers (one per
  player: the pads' sticks and buttons turned into 30 actions with held,
  pressed and repeating words and analog values): their set-up at boot,
  the client list they're kept in, binding to the pads, the update and
  the read slots seen in play (B7; the input path below them is B1's,
  in `input.md`).
- [`input.md`](input.md): the input below the action mappers: the
  input manager with its logical and physical pads, input processors
  and listener lists, their constructors and the manager's slots at
  boot, the `std::list` of pointers the lists are, the button tests,
  and B1's native update, pad read and XInput wrappers (B7).
- [`data-tables.md`](data-tables.md): the game's data tables at
  `[0x826C5CA0]`: twelve archive assets loaded once at boot into one
  block, byte-swapped by their load callbacks, and the accessors that
  index them (the 400 3,008-byte items and the 56-, 20- and 40-byte
  tables; B7).
- [`car-specs.md`](car-specs.md): the car parameter tables at
  `[0x826C2E48]` (assets 72-83: the car specs, wheels and aero parts),
  their loads and byte swaps, the accessors, and the car object a car's
  tuned spec is copied into (`Car::Refresh`); from SBX-Recomp's notes,
  checked against our code.
- [`car-models.md`](car-models.md): loading a car's models (the outside
  sets a and b, the cockpit set c, the interior, the wheels) through load
  slots and the shared load buffer, the nodes they're placed at, and the
  vinyl sheet drawn from the decal TGAs on the CPU.
- [`models.md`](models.md): the graphics library's models (part files,
  node files and skeletons, recording a shape's draws), the XPR2 texture
  bundles and the boot's shader pairs.
- [`model-animation.md`](model-animation.md): the graphics library's
  keyframed model animation: a model's track sets for its part file's
  sections, their tracks, curves and keys, the per-draw sampling step,
  and the skeleton class under the node file; with the library's
  primitive drawer beside it (B7).
- [`cameras.md`](cameras.md): the cameras under the game's camera
  modes: the graphics library's camera class, the 21 camera passes
  (their set-up by type, render targets and surfaces, enabling, update
  and the commands recorded at a pass's start and end) and the two
  viewports with their 20 camera entries the camera modes write (B7).
- [`strings-and-font.md`](strings-and-font.md): the game's texts: the
  string table (17 string files loaded by mask, a string id formatted
  with its arguments into UTF-16, pages) and the bitmap font (four pages
  of glyphs in a texture's channels, their shaders, a text's width, and
  drawing a text as quads through the command list) (B7).
- [`std-string.md`](std-string.md): the C++ library's narrow string
  (`std::string`: constructors, `_Tidy` and the destructor, growth,
  assign and append) as seen in play, with the library's wildcard search
  and replace over it and two global strings' static set-ups (B7).
- [`track-route.md`](track-route.md): the course's route at
  `0x826C5B10`: route positions (a section and a slice) normalized and
  moved along its linked sections by lane, branch or path, points and
  lanes on it, distances along it, and following a point along it for
  the track positions (B7).
- [`ai-drivers.md`](ai-drivers.md): the AI drivers: the computer
  racers' table of drivers, filling their cars' input records, the path
  each keeps along the course's route (its line across the road, the
  corners, the room left by other racers), their target speeds, pedals
  and steering, their records of the other racers, and their skill
  blocks (B7).
- [`racer-controllers.md`](racer-controllers.md): the racer
  controllers: the cars' input records at `0x826C5B30`, filled every
  tick from the player's pad (the control map, dead zones and flags) or
  by the racer's AI driver, the input recordings and replays, the race
  flow's handlers that pick which each racer gets, and each AI driver's
  set-up with its profile (B7).
- [`track-zones.md`](track-zones.md): the track zones: 70 stretches
  of the course's route, each a few route ranges (a start and an end
  section and slice, and the sections to take at forks), that the
  player's car is checked against for the HUD, their owner at
  `0x828161EC`, and the fixed-size lists they're kept in (B7).
- [`graphics-resources.md`](graphics-resources.md): the graphics
  library's resources: texture and surface headers and what's worked
  out from them (block sizes, formats, types, a level's size, layout,
  offset and description), making textures, surfaces with their EDRAM
  tiles, surface levels and vertex and index buffers, the device's
  current targets and stream, locks and unlocks, reference counts and
  destruction (B7).
- [`video-output.md`](video-output.md): the game's video output: its
  set-up, the display and the front buffers, present and the vblank
  wait, the frame's unbinding, the device lock (with the device's thread
  word) and the core's sync primitives under it (the semaphore, events,
  critical sections, scoped locks, waits) (B7).
- [`device-state.md`](device-state.md): the graphics device's state
  setters: the render states and sampler states (the device's setter
  tables), the render target, depth surface, viewport and scissor, the
  stream sources, index buffer and textures, the blend controls and
  shader constants, the default state at boot, and the GPU register
  split, the screen scissor and the gamma ramp with the packets they
  write (B7).
- [`course-scenery.md`](course-scenery.md): the course scenery: its
  manager at `0x826C5514`, the areas and their models and lights
  collected around the viewports each update and streamed in and out of
  five fixed pools of physical memory, the course's shared set, the
  lights' nearest search and the scenery's draw with the cars' headlight
  projectors (B7).
- [`post-effects.md`](post-effects.md): the post effects drawn over
  each view: their table and the 15 objects at `0x826C5D80` (the glow
  with its glare and star definitions, the color filter, the motion
  blur, the render target filter), the render texture pool they share,
  their shared command recorders (states, targets, constants, quads,
  resolves), and the race's toggle over them (B7).
- [`screen-objects.md`](screen-objects.md): the race's screen objects
  (one a view, at the race object's +31152 to +31160): their texture,
  vertex declaration and the fan and rings built at their set-up, the
  map of the rings' texture coordinates by position (two `std::map`
  instantiations), the speed update and the draw, probably a screen
  blur while the nitrous runs (B7).
- [`framework.md`](framework.md): the app framework: the framework
  object `main` makes, its set-up and the factories of its services
  (named only as far as the framework makes them), its sixteen
  interfaces' getters, the run loop (`App::Run`, its tick handler and
  listener maps and the requests applied each pass) and the frame start
  and debug text it owns (B7).
- [`game-modes.md`](game-modes.md): the game modes (the object the app
  runs at a time, made by type when the mode changes) and the race's,
  their message dispatcher and the state machines that handle the
  messages (a transition table keyed by state and message), with the
  race overlays' state machine and its handlers (B7).
- [`mode-tick.md`](mode-tick.md): what the mode manager's tick runs
  besides the mode: the per-mode settings (the system checks allowed,
  the scene target's mode, presence), the sign-in watch, the screen
  fades and the color rectangle they draw, the billboards' ring flip,
  the menu music's start and the input unblocked on a mode change (B7).
- [`minimap.md`](minimap.md): the race's minimap: the map's models,
  the point markers and the car markers (by type, in five colors, with
  the blink clock that flashes the player's), the triangles they're
  drawn with, their 80-slot lists, and two of the race manager's
  accessors (B7).
- [`scene-models.md`](scene-models.md): the scene models (the cars as
  the scene holds them, in its second set of pools): their set-up and
  build over the parts' pools, the build deferred to their update, the
  set-up dispatch, the parts' links, the records' defaults and copies,
  the parts' and the traffic body's loaders, and a record re-applied
  (B7; the load poll and the release aren't in it).
- [`scenery-lists.md`](scenery-lists.md): the course set's six lists
  of models (loaded, released, picked by the race and drawn, the +3180
  list at its points), the scenery model's shape draw and record with
  the course's texture animations, and the model draws they share: the
  shape's visibility test, the per-object and scene light constants,
  the depth-only record, the generic shape draws and the texture
  provider's slots 2 and 4 (B7).
- [`shader-objects.md`](shader-objects.md): the shader objects made
  from shader files (vertex and pixel: their fields, their fetch
  instructions, the library's create calls) and the memory blocks with
  an `XLMM` header they are made in (the virtual and physical makers and
  the pending frees' walk at the pass's start); `XMemCpy`'s rows are in
  `c-runtime.md` (B7).
- [`race-hud.md`](race-hud.md): the race HUD's movie picker (a UI
  movie run by the HUD's state) and the rival intro's logo draw (B7;
  the HUD's set-up, update and teardown aren't in it).
- [`race-setup.md`](race-setup.md): the race's set-up (the race data
  reset, its route and the course routes' table, the racers made from
  the garage, the rival's items and the traffic, their start positions,
  the stage list by the rival's race kind), the car object's set-up
  (its parts' resets, from a car spec and from a car build), the race
  data's other records and the event's accessors (B7).
- [`vehicle.md`](vehicle.md): the vehicle code above the car physics:
  the seven vehicles made and destroyed, a vehicle set up from a car
  object, its tick (the car input applied to the controls, two physics
  steps, the track positions, the status record: speed, place on the
  route, wrong way, distance, temperatures), its placement on the route,
  the engine's power curve and a few queries (B7).
- [`app-states.md`](app-states.md): the app's update (its four handler
  maps, their requests, its message queue and the toggle player 1's
  Start flips) and the state stack at the app's +116: a tree of layers,
  each with a stack of states (the game modes' holder the only one in
  play), its children and its queued operations, with the lists and
  vector it instantiates (B7).
- [`scene-loaders.md`](scene-loaders.md): the first set of the scene's
  object pools and its seven loading classes (the car model, parts,
  cockpit body, interior, pool 4's model, wheels and traffic body): the
  set made and filled, each class's constructor, reset, release and
  destructors with the loads it embeds, the cockpit's, interior's and
  traffic body's load callbacks (skeletons bound and searched by node
  name), their loaded queries and walks, and the XPR name lookup and
  model point search the car code shares (B7).
- [`save-data.md`](save-data.md): a player's save data in memory, made
  with its defaults and deleted: the progress (the stats record, the
  garage list, the item records, the unlock bit sets), the course
  records, the options and the default control maps, and the title
  screen's reset that makes player 0's again (B7).
- [`texture-compression.md`](texture-compression.md): XGraphics'
  surface compression and conversion, linked into the game:
  `XGCompressSurface` (the vinyl sheet to DXT5, its only use in play),
  D3DX's image blitter and filters, a codec for each format (rows
  decoded to and encoded from float4s), the DXT block encoders and
  decoders, `XGCopySurface` and the endian swaps (B7).
- [`race-world.md`](race-world.md): the race world, the race mode's
  state machine that loads, settles, ticks and frees the race's cars and
  collision world, and the racers' manager at `[0x826D82F4]`: its
  entries for the opponents and traffic, which of them get one of the
  seven vehicles near the player (streamed every tick), the others moved
  along the route, the racers' list, and the event's start positions
  (B7).
- [`race-states.md`](race-states.md): the race's states, the race
  mode's state machine that runs a race from start to end (each stage's
  set-up, the loading screen waits, the rolling and standing starts, the
  countdown, the end, the replay and the menu after the race) and the
  race's progress at `[0x826D82E8]`: each racer's time, laps, distance
  along the route, hits, spirit points and place (B7).
- [`archive.md`](archive.md): the archive streaming's functions not
  met in B4 (whose rows stay in `files-and-saves.md`): the TOC lookup,
  the queue's length, wait and cancel, the loader thread's functor, the
  request queue's list class and the static set-ups and clean-ups of
  the queue, the request slots and the four semaphores (B7).
- [`file-layer.md`](file-layer.md): the file layer above the file
  calls: the file objects' class chain (the game's file over an
  overlapped file over `File::`), the path objects that build a full
  path with sprintf and the game's path buffer (B7; the `File::` class
  and the open for read by name stay in `files-and-saves.md`).
- [`save-interface.md`](save-interface.md): the save system's
  interface between the front end and the save devices: the interface
  over the app at `0x826BDC90` and the save devices' and users' slots it
  reaches (the slots' request setters and getters), and the front end's
  save layer: the save buffers, the game data's save and load and the
  replays' list, save, load and delete, with the play time update (B7;
  the save slots' steps stay in `files-and-saves.md`).
- [`car-sounds.md`](car-sounds.md): the car sounds, the game's own
  sounds for each racer played straight through XAudio's voices (the
  engine's two layers, the tires, hits, wind, road noise, pass by and
  others, the in-memory wave banks' reader), their 3D positioning, and
  X3DAudio, the 3D audio library linked into the game (B7;
  X3DAudioInitialize stays in `sound-cues.md`).
- [`xaudio.md`](xaudio.md): XAudio's API entry points and its voices
  (the source and submix voices the car sounds, XACT and the voice chat
  call: create, start, stop, state, volume, pitch, outputs, effects,
  release), their set-up, outputs and mixer passes, and XAudio's three
  locks (B7; XAudioInitialize stays in `sound-cues.md`).
- [`xact-waves.md`](xact-waves.md): XACT's waves and voices: the voice
  class over XAudio's source voices (create, state, volume, pitch,
  packets, start, stop, flush, outputs, release) and the reverb submix,
  the waves that play a wave bank's entries through them (in-memory and
  streaming: prepare, play, stop, pause, the game thread's and the audio
  callback's updates, the notifications), their stream entries and the
  sound banks' variation tables (B7; the rest of XACT stays in
  `xact.md`).
- [`xact-tracks.md`](xact-tracks.md): XACT's tracks and events: the
  sounds' tracks (volume, pitch, outputs, play, stop, pause, the game
  thread's and the audio callback's updates, the RPC hooks), their
  instances (one play of a track's events), the clip events made from
  the sound banks' records (the play-wave event that owns each wave,
  the wave-variation, stop and transition events) and the waves'
  forwarders to their voices (B7; the parameter events are a lead).
- [`xaudio-engine.md`](xaudio-engine.md): XAudio's engine and mixer:
  the engine's creation and set-up (its size, processors, threads and
  kernel objects), the object factory, the allocators, the mastering
  voice and its render driver object, the mixer's callback, the
  processor threads, the stages' passes and barrier, the processing
  callbacks and the commit (B7; XAudioInitialize stays in
  `sound-cues.md`, the category volume in `audio.md`).
- [`xaudio-objects.md`](xaudio-objects.md): the objects XAudio's
  factory makes: the class tables, the source voices' codecs (the packet
  queue, PCM and XMA: submit, flush, state, start, stop, volume, pitch,
  format, the pass) and their resampler, the output matrices (set, mix),
  the reverb and the volume meter (B7; the XMA voice's hardware side
  stays in `audio.md`).
- [`app-start-up.md`](app-start-up.md): the app's constructor and its
  start-up at boot: the app's vtables, maps, tick handler and listener,
  the objects it owns, the globals that keep its interface, the scene's
  1024 × 576 target and tile surfaces, the light billboards' shaders and
  vertex declaration, `CreateVertexDeclaration`, and the voice engine's
  set-up parts that had no rows (its talkers, processing objects and
  XAudio voices; the rest of voice stays in `system-services.md`) (B7).
- [`data-load.md`](data-load.md): the game's data load at boot and its
  unload: the objects kept for the session (the race data's, the input
  recordings' and the kept replay's makers, the 5,232-byte records'
  constructors), the save defaults' bit sets, the loading screen's and
  another set of UI movies, and two static copies of the replay (B7;
  the loads it calls stay in their files).
- [`career.md`](career.md): the player's career over the save data:
  a race's result in the stats record and the items' records, the
  unlocks and rewards checked after a race, the stages, the day's
  advance and the day's races picked, and the garage's list (B7; the
  race event it makes has its own lead).
- [`race-event.md`](race-event.md): the race event at `[0x826C6078]`:
  the opponents a career race offers, picked from the items by their
  unlock conditions (68 kinds of test over the save data and the car),
  grouped by location, and the list the race's set-up and the race
  world read (B7).
- [`camera-modes.md`](camera-modes.md): the race's camera director and
  its six views' cameras (chase, two in-car, the start's look at the
  racers, the TV camera with its paths along the route, the scripted
  cutscenes with the rival's logo), the camera animation files and two
  front-end cameras (B7; the viewports below them are in `cameras.md`).
- [`location-screen.md`](location-screen.md): the career's location
  screen in the front end: its state machine, the course scenery and the
  location's opponents posed as cars, the pictures loaded from the
  archive, its 16 pages (menus, texts, the race's set-up) and 36 page
  transitions over two UI movies (B7).

These files hold functions only. What used to follow the table here moved
out on 2026-10-20: how frame interpolation works, its options and what
didn't work to `../frame-interpolation.md`, the boot and attract race to
`../title-screen.md`, the known issues to `../known-issues.md`, the
globals and object fields to `../known-data.md`, and the emulated GPU's
old performance notes to the end of `../native-renderer.md`.

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_8258DE50` | `Thread::Start` | | Entry point of the game's threads: calls vtable slot 1 of the object at start context + 8. |
| `sub_82095AF0` | | | A worker thread's loop (started through `sub_8258DE50`): waits on an event, runs jobs through function pointers, signals events; it opens the game's data files at boot. On the Steam Deck it spiked to 66-82% of a core with 15-32% in the kernel around race loads; here (2026-10-14) ~29% with ~68,000 minor page faults in 2 s while a race loads. On the Deck (`itc_041.log`) the kernel time is page faults: 37-69% of a core (19-24% kernel) with 27,000-52,000 minor faults and no major ones per 2 s, its system call samples all `running` (in a fault, not a call) but once `202` (futex: waiting). Only around loads (2-4 s each); in a race it's at 0-15%. The fix, if worth it: populate the guest memory's pages in advance (e.g. `MADV_POPULATE_WRITE` on the views), moving the cost to startup. |
| `sub_82092358` | `Renderer::Init` | | Sets the vblank interval (2 = 30 fps) via `sub_8258D568`, then subscribes renderer callbacks to numbered events (2, 3, 5, 6, 16). |
| `sub_820920D8` | `Renderer::WaitForFrame` | Wrapped | Tick handler on the main thread. Blocks until the render thread signals the frame is done (event at this+392), swaps, and starts the next frame. Frame pacing runs after it (`src/frame_pacing/renderer_hooks.cpp`). |
| `sub_82091EF8` | `Renderer::ThreadLoop` | Overridden | Render thread loop; started through `sub_8258DE50`. Draws one frame each time the main thread asks, as the game did, without the 2-vblank wait. |
| `sub_82091A60` | `Renderer::Present` | Wrapped | Flushes, waits for the vblank interval (`sub_8258DC58` → `sub_8258DAA8`), then swaps buffers. Event 5. Wrapped only to time it (`--log_frame_times`). |
| `sub_82091E58` | `Renderer::DrawFrame` | | Render thread: calls each draw listener in the list at renderer+28. |
| `sub_82091BC0` | | | Draw listener between the scene draw and present: resolves the frame into a front buffer (`sub_8258D998`) and waits for the GPU to go idle (`sub_8229F1B0`). See "The end of a frame" in `gpu-functions.md`. |
| `sub_820988C8` | `Scene::Update` | Wrapped | Tick handler on the scene object (0x40026FE0 at the menu). Calls the current mode object at +64 (vtable slots 6 and 2). Wrapped for `--log_boot_modes` (logs the front end's state objects at `[0x82814E2C]` and `[0x82814E34]` as they change). |
| `sub_82098940` | `Scene::Draw` | | Draw listener on the same object. Calls the mode object's draw (vtable slot 5), reading live game state. |
| `sub_8223A3F0` | `Delegate::Invoke` | | Calls the function at +8 with the argument at +4. Used for tick and draw listeners. |
| `sub_82233460` | `MemberDelegate::Invoke` | | Calls the member function at +8 on the object at +4, adjusted by +12. |
| `sub_8258D4D8` | `VideoOutput::OnVblank` | | The vblank callback (device +15144): adds 1 to `8296BD2C` and `8296BD30` and signals the event at `8296BD34` (`Event::Signal`). Only `sub_8229DEA0` calls it, which is never called now. Its counts and event were read and waited on only by the original `VideoOutput::WaitForVblank`, which our override replaced. Never called. |
| `sub_825DA470` | | | Static set-up: makes the vblank event at `8296BD34` (`sub_8258DCC8`) and registers its clean-up (`sub_825A7068`). Not read in detail. |
| `sub_825DB2C0` | | | Static clean-up: closes the vblank event at `8296BD34` (`sub_8258DD60`). |
| `sub_8258DD60` | | | Probably `Event::Close`: r3 = the event. Not read. |
| `sub_822B20D8` | | Dropped | Frame statistics, for a debugger: frame counter (+20024 += 1), time since the last Swap from the timebase (+20028; +20032/+20036 the last), +20048/+20056 moved to +20064/+20068 and zeroed, debugger requests (+15168 through `820007F0`), the GPU performance counters while +20104 is 1 or 2 (`Device::SetClockGating`, `sub_8229B930`, `sub_8229B9C8`, `sub_8229BFE8`, `sub_82106540`, `sub_822976F8`), then `sub_822B1788` and the debugger's callback at `820007B4` +4. Was called once a frame, only from Swap. Readers of what it leaves: `sub_822B1088` and `sub_822ACAA8`. The native Swap leaves the call out (before that, skipped behind a temporary switch: only +20024 and +20028 differed, frames right). |
| `sub_825BA0C8` | `QueryPerformanceCounter` | Overridden | r3 = the 64-bit count (out); returns 1. Read the timebase (`mftb`, the SDK's guest clock: 50 MHz from 0 at its start), again when its low word was 0 (the 360's timebase erratum). Leaf. 23 callers, 5 seen in play: XACT's clock (`sub_825BFE28`) and `XACTSound::AudioUpdate` (`sub_825CBD10`, [`xact.md`](xact.md)), `sub_82465428` and `sub_8258D090`, `sub_8258D0D8`, `sub_8258D170` (not read: probably timing). Nothing else seen in play reads `mftb` (its other readers, `sub_822B20D8` and `sub_8245D288`, are Dropped; `sub_8229D6F0` and `sub_8229D720` never ran), so nothing compares the two. Ours (2026-10-29, the user's call, followed as GetTickCount's): the adapter in `src/hooks/clock.cpp` stores `helpers::PerformanceCounter()` (`src/helpers/clock.cpp`): ticks at QueryPerformanceFrequency's 50 MHz (`sub_825B9A70`) since start from the host's steady clock, read again on a low word of 0 as the original; native callers call it directly, so both share one clock. No flag. Harness `clock` (r3, the bytes past the count, the advance over sleeps within 1 ms of the SDK's timebase). |
| `sub_822956F0` | `GetTickCount` | Overridden | Returned the word at `[0x82000854]` + 16: the kernel's `KeTimeStampBundle` (a variable import; the SDK sets +16 to its uptime in milliseconds every 1 ms), the game's only reader of it. Leaf. 25 callers, 5 seen in play: XACT's clock (`sub_825BFE28`) and `sub_825CBD10` ([`xact.md`](xact.md)), `sub_8214F6E8` ([`system-services.md`](system-services.md)), `824CF448`, `824CF840`. Ours (2026-10-23, the user's call): the adapter in `src/hooks/clock.cpp` returns `helpers::TickCount()` (`src/helpers/clock.cpp`), milliseconds since start from the host's steady clock, wrapping at 2^32; native callers call it directly, so both share one clock. No flag (a fallback would split the callers between two clocks). Harness `clock` (the advance over sleeps, within 3 ms of the SDK's). |
| `sub_8220BC28` | `MenuList::GetPreviousSelection` (probably) | Overridden | Returns the word at +20: the menu list's previous selection ("The menu list" in `../pause-menu-fix.md`). A one-instruction getter the linker folded, so XACT's `sub_825C8440` ([`xact.md`](xact.md)) calls it for an entry accessor's play offset (+20). 52 callers, 7 seen in play: the menus' `8220C7B8`, `8220D898`, `8220DD98`, `8220E1F8`, `82218788`, `82222E88`, and XACT's. Leaf. Native in `src/xact/xact.cpp` (`WaveEntryGetPlayOffset`) behind `--native_xact` (adapter in `src/hooks/xact.cpp`, harness `xact`, its wave entries and lookups group): one load, so the override gives every caller, the menus' too, the same result. Runs in attract races. |
| `sub_820C0128` | `Vehicle::GetSpeedKmh` | Overridden | Returns the float at vehicle+48 (m/s) times 3.6. The plain C++ (`Vehicle_GetSpeedKmh`, `src/hooks/vehicle.h`) is also called directly by the native `OverlayView::SpeedKmh` (`sub_821268C0`, [`race-overlays.md`](race-overlays.md)); the adapter also tells the state capture (`--capture_dir`) about the vehicle. Checked against the original by the `race_overlays` harness. |
| `sub_8215B4F8` | `Race::DrawStep` | Wrapped | The race's draw step (Draw message). Its running marks the pass as in a race. |
| `sub_8258FD48` | `Command::SetShaderConstants` | Wrapped | Render thread, command 7: r3 = payload, r4 = register count, r5 = stage << 24 \| type << 16 \| start << 8 \| count. Interpolation substitutes blended values here. |
| `sub_82146A38` | | | Finds the animation in set r3 whose texture is r4; -1 if none. |
| `sub_82107B90` | `Boot::StartMovie` | | Message handler: plays the next boot movie, id from the table at `0x826C5BAC` (index at `0x826C5BC8`), via `Movies::Play` (`sub_820A43A8`, the path table at `0x82600BEC`; `movies.md`). See "The title screen" in `../title-screen.md`. |
| `sub_82107C58` | `Boot::WaitForMovie` | Wrapped | Message handler, every tick while a boot movie plays; ends the step on a button press if skippable (`0x826C5BC4`) or when the movie has finished. The movie started (`Movies::Start`) when its state (`Movies::State`, twice a tick) is 1 with +9608 = 0 (then 1); finished when the state is 1 again with +9608 = 1. `--skip_intro` makes it finish at once. |
| `sub_82107BF8` | `Boot::EndMovie` | | Message handler: stops the movie (`Movies::Stop`, `Movies::Release`, then `sub_825895A0`); the front end (Genki logo, title) follows. |
| `sub_82296270` | `OutputDebugStringA` | Overridden | r3 = text: puts it in an `ANSI_STRING` (`RtlInitAnsiString`) and passes the text to `sub_822972D0`, which ends in `sub_82297DC0`, an empty function (the retail game drops its debug output). Called from 19 files. Ours (`src/hooks/debug_output.cpp`, in front of `helpers::DebugOutput`, `src/helpers/debug_output.cpp`) does nothing. See B6 in `docs/rexglue-removal.md`. |
| `sub_822970A8` | `InitNamedObjectAttributes` | Overridden | r3 = attributes (12 bytes), r4 = an `ANSI_STRING` (8 bytes), r5 = the name's text; returns r3. Fills the string by `RtlInitAnsiString`'s rule and the attributes as root directory -4 (the named objects' root; -3 is `ObDosDevices`), name, attributes `0x80` (probably `OBJ_OPENIF`). Callers: named event and timer creation (`sub_82295E78`, `sub_825BA2B0`), only when given a name; a boot and an attract race make none. Plain C++ in `src/helpers/object_attributes.cpp` behind `src/hooks/object_attributes.cpp`; a temporary self-test (5 strings: a name, empty, none, 300 characters, one character) gave the same 20 bytes as the recompiled version. See B6 in `docs/rexglue-removal.md`. |
| `sub_825BA4A0` | `OutputDebugStringW` | Overridden | r3 = UTF-16 text: converts it to ANSI on the kernel's heap (`RtlInitUnicodeString`, `RtlUnicodeStringToAnsiString`), passes it to `OutputDebugStringA` (`sub_82296270`, which drops it) and frees it. Ours (`src/hooks/debug_output.cpp`) does nothing. See B6 in `docs/rexglue-removal.md`. |
| `sub_822968B8` | `GetLocalTime` | Overridden | r3 = SYSTEMTIME (out): the time now (`KeQuerySystemTime`) less the bias (`sub_82297678`), as fields. Ours: `src/hooks/time.cpp`, the clock and math in `src/helpers/time.cpp` (the host's UTC time as FILETIME); works out the bias itself; no guest calls (the zone from the native time zone read, `sub_8229B440`, B5). Called once at boot, from `0x82247978`. |
| `sub_82297678` | `GetTimeZoneBias` | Native | r3 = 64-bit bias (out, 100 ns units): the zone's bias plus its standard or daylight bias by `sub_82297DC8`'s state (0 for an error). Native: `helpers::TimeZoneBias` (`src/helpers/time.cpp`), called by GetLocalTime's adapter through `time_hooks::GetTimeZoneBias`; the recompiled one is no longer called (its only caller was GetLocalTime). 6 zones the same, including one whose biases overflow 32 bits. |
| `sub_82297DC8` | `GetTimeZoneInformation` | Overridden | r3 = TIME_ZONE_INFORMATION (172 bytes: bias +0, standard name +4, standard date +68, standard bias +84, daylight name +88, daylight date +152, daylight bias +168). Reads the zone from the console's settings (`sub_8229B440`), clears the daylight fields when it says there's no daylight saving, then returns 1 (standard time) or 2 (daylight time) by the two cutovers (`sub_8229B1E8`), 0 without dates, -1 when the settings can't be read (the last error set). The SDK's zone has no dates, so the game gets 0. Ours: `helpers::TimeZoneState`, the zone from `time_hooks::ReadTimeZoneSettings` (native, no guest call; it can't fail, so no -1); the same bytes and answer as the recompiled one on eleven made-up zones (northern, southern, absolute dates, invalid ones). |
| `sub_8229B1E8` | `GetCutoverTime` | Overridden | r3 = a time zone date (SYSTEMTIME; year 0 = a rule: `wDay` the week, 5 the last, `wDayOfWeek` the weekday), r4 = the cutover (out, FILETIME), r5 = now, r6 = from this year (low byte); returns 1 when found. An absolute date's time is stored even when it's past (0 returned). Ours: `helpers::CutoverTime`; 4,000 random cases the same as the recompiled one. |
| `sub_8229B680` | `SystemTimeToFileTime` | Overridden | r3 = SYSTEMTIME, r4 = FILETIME (out); returns 1, or 0 for an invalid date (`sub_82296E88` sets STATUS_INVALID_PARAMETER). Ours: `helpers::SystemTimeToFileTime`; the same as the recompiled one on 2,000 random dates in 1678-2261. Outside those years the SDK's `RtlTimeFieldsToTime` overflows `system_clock`'s nanoseconds and gives wrong times; ours gives the right ones. |
| `sub_8229B720` | `FileTimeToSystemTime` | Overridden | r3 = FILETIME, r4 = SYSTEMTIME (out); returns 1. Ours: `helpers::FileTimeToSystemTime`; the same on 2,000 random times in 1678-2261 (outside, the SDK's `RtlTimeToTimeFields` overflows, as above). |
| `sub_825B9A70` | `QueryPerformanceFrequency` | Overridden | r3 = 64-bit rate (out); returns 1. Ours stores 50,000,000 (`src/helpers/clock.h`), the rate the SDK's runtime gives the timebase recompiled `mftb` reads. |
| `sub_82296648` | `MultiByteToWideChar` | Overridden | r3 = code page, r5 = source, r6 = its length (-1: terminated, terminator counted), r7 = destination, r8 = its room in characters (0 to measure); returns the characters, 0 with error 122 (`sub_82296FD0`) when the room is short. UTF-8 (65001) goes to the game's converter `sub_82297340` (a guest call); other code pages widen each byte (the SDK's `RtlMultiByteToUnicodeN`). Ours: `src/hooks/strings.cpp`, `src/helpers/strings.cpp`; 3,000 random cases the same. Called once at boot, from `0x8214E44C`. |
| `sub_82296720` | `WideCharToMultiByte` | Overridden | The same layout as `MultiByteToWideChar`, room in bytes, r10 = "default used" (out, 0); characters above 255 become '?' (the SDK's `RtlUnicodeToMultiByteN`), UTF-8 through `sub_82297540`. Ours as above; 3,000 random cases the same. |
| `sub_82296870` | `swprintf` | Overridden | r3 = buffer, r4 = format, then the arguments (to `vswprintf`). One call site (`sub_8214F290`, returning to `0x8214F338`), with a Japanese "VS MODE" format at `0x82009218` that has no conversions; ours copies the format (`%%` made `%`) and logs an error at any other conversion. The same as the recompiled one on that format and 199 made-up ones. |
| `sub_822972D0` | | | OutputDebugStringA's sink: passes the text (`[r3+4]`, the ANSI_STRING's buffer) to `sub_82297DC0`. |
| `sub_82297DC0` | | | Empty (`blr`): the retail game's debug output goes nowhere. |
| `sub_82296E70` | `SetLastError` | Overridden | r3 = the error: stored in the current thread's last error, `[[r13+256]+352]` (r13 = the PCR, +256 its current thread), unless `[r13+336]` is set (probably: in a DPC). Ours: `last_error::Set` (`src/hooks/last_error.cpp`), a thin adapter for the recompiled callers; our adapters call it directly. 2,000 cases the same as the recompiled one (with `sub_82296FD0`). |
| `sub_82296FD0` | `SetLastError` | Overridden | Jumps to `sub_82296E70`. Ours: the same as `sub_82296E70`. |
| `sub_82296EC0` | `GetLastError` | | The thread's last error (`[[r13+256]+352]`), or 0 while `[r13+336]` is set. Called by `sub_82294ED8` after its XMsg call. |
| `sub_82294E20` | `CloseHandle` | | r3 = handle: `NtClose`; 1, or 0 with the last error. Called for files, events and enumerators (the file class's close, the save slot's close wait, the enumeration's wait, `Archive::Shutdown`). |
| `sub_82294DA0` | `GlobalMemoryStatus` | Dropped | r3 = a MEMORYSTATUS (32 bytes) filled from `MmQueryStatistics`. From `sub_82106548`, `sub_82106618` and `sub_82107910` (25 calls in a session), each with a buffer in its frame that it never reads. Ours (`src/hooks/memory_status.cpp`): writes nothing. `BootData::Load`'s plain C++ (`src/data_load/boot.cpp`, 2026-10-30) leaves both its calls out, as `App::StartUp`'s does. |
| `sub_82295F18` | `GetLastError` | | Jumps to `sub_82296EC0`. Called after failed file calls (GetDiskFreeSpaceEx, XGetOverlappedResult, the C runtime's I/O), and on free's and realloc's failure paths (`heap.md`, never ran). |
| `sub_82296E88` | `SetLastErrorFromStatus` | Overridden | r3 = an NTSTATUS: `SetLastError(RtlNtStatusToDosError(r3))` (the same check of `[r13+336]`), returning the DOS error. Called after failed kernel calls (27 files). Ours: `last_error::SetFromStatus`, the mapping from the SDK's `xeRtlNtStatusToDosError` (C++, no guest call) until the import's last recompiled caller, `sub_822953E8`, is native and the table is ours. 1,000 statuses across the mapping's branches the same as the recompiled one. |
| `sub_82297340` | | | UTF-8 to UTF-16, for `MultiByteToWideChar`'s code page 65001: r3 = source, r4 = its length, r5 = destination, r6 = its room. Not read. |
| `sub_82297540` | | | UTF-16 to UTF-8, for `WideCharToMultiByte`'s code page 65001; arguments as `sub_82297340`. Not read. |
| `sub_82295E78` | `CreateEvent` | | r4 = manual reset, r5 = initial state, r6 = name (0 for none): `NtCreateEvent`, with `sub_822970A8`'s object attributes only for a name. None named in a boot and attract race. Threads and sync (core). |
| `sub_825BA2B0` | `CreateTimer` | | `NtCreateTimer`, named the same way. Threads and sync (core). |
| `sub_82295500` | `Sleep` | | r3 = milliseconds (from `sub_8245D288`'s `Sleep(1000)`; not read). |
| `sub_82482B18` | | Overridden | `src/hooks/video_mode.cpp`, without `XGetVideoMode` (it read only `is_widescreen`; ours is `display::kWidescreen`). Once at boot, from `sub_82483130` (`82483174`), r3 = an object (+56 the device). Stores the back buffer's size (`sub_8229B7B0`) as floats at +132/+136 and the display's aspect (16/9 or 4/3, the game's constants at `8204D300`/`8204D2FC`) over the back buffer's at +140 (1.0), makes a 12-byte index buffer (`sub_822A6968`, to +60, filled with 0-5 through `sub_822A6A18`/`sub_822A6A78`), a 120-byte vertex buffer (`sub_822A6830`, to +64) and a vertex declaration (`sub_822A8980` with `8264D550`, to +68), then returns the object's vtable +96 call (0), or `0x8007000E` if one couldn't be made. What the object is isn't known. |
| `sub_82483130` | | | Calls `sub_8229D7E0`, then `sub_82482B18` at boot. Not read. |
| `sub_824846D8` | | | Vtable function (no direct callers), seen in play. Makes a vertex shader (`sub_822AA018`) and pixel shaders (`sub_822A9EE8`, twice) without compiling: probably where the game's precompiled shaders are made. Not read. |
| `sub_8248FBE0` | | | A method that isn't implemented: `DbgPrint` and `0x80004001` (E_NOTIMPL). No direct caller (a vtable). Never called in play (two attract races and two sessions the user played, 2026-10-21). |
| `sub_8229B7B0` | | | Copies four words of the device's presentation parameters (+13188 width, +13192 height, +13236, +13196) to r5. r3 device, r4 unused (0). From `sub_82482B18`. |
| `sub_822221A0` | `TitleScreen::Update` | Wrapped | Counts idle ticks and starts the attract race at 600 (see "The title screen" in `../title-screen.md`). `--attract_after_s` shortens the wait. |
| `sub_820C9170` | | | Asks `sub_820F4128` for up to 10 lights near a point (r1+112) in a cone (cosine-like value from `sub_825A8978` of a setting at `[r27+11844]+5500` times a constant) of the light set at `[0x826C551C]`. Probably feeds the 8 per-object light slots of the pixel constants (see "Per-object pixel constants" in `../frame-interpolation.md`; not checked). Called from `0x820E1DF0`, `0x820E6B00` and `0x820DC3B8`. |
| `sub_8218E420` | | Patched | Post-race check; the comparison against 60 was changed to 1 to fix the black screen after quest races. |
| `sub_820DAC68` | `Car::DrawUpdate` | Wrapped | Draw's per-car update (r3 = car - 4288; it passes r3 + 4288 on as the car), on every pass: first takes `[0x82000A30]` (1.0) off car + 704 (the boost; the tick sets it again), then poses and draws the car, the cockpit dials among it (`sub_820E51D0`, `sub_820E45A8`). The player's car in cockpit view runs it twice a pass; only the first poses the dials. Wrapped only for `--find_per_frame_state` (`ITC_FRAME_STATE_WRAP`). |
| `sub_8215C780` | `Car::TickUpdate` | | The tick's car record update (logic passes; r31 = the car record, r26 = the vehicle): copies the car's world matrix into car + 16 and stores the gauge values at car + 696 (`sub_820C0128`, the speed in km/h), + 700 (`sub_820C0140`, the revs), + 704 (`[r26 + 2944]`, the boost) and + 712 (`[r26 + 6260]`). |
| `sub_820E51D0` | | | Calls `sub_820E42E8` when `[dash + 20]` is set, then `sub_820E39F8` when `[dash + 2180]` is. |
| `sub_820E42E8` | `Dash::PoseTachometer` | Overridden | `src/hooks/cockpit_dials.cpp` (`--native_cockpit_dials`; `docs/cockpit-dial-interpolation.md`). r3 = the dash, r4 = the car, r5 = the camera passes. Gets two angles from `sub_820D3740([dash + 2896], car, [dash])`: `a` the speedometer, `b` the tachometer. Each needle's world matrix is `(B * RotZ(angle)) * A * car + 16`, with A, B at dash + 32/+96 for `a` and + 272/+336 for `b`; drawn with `sub_820D06F0(dash + 496 / + 736, r5 + 144, world, 0, 0)`, both part `...06A0`. |
| `sub_820E45A8` | `Dash::PoseDials` | Overridden | Same file. r3 = the dash, r4 = the car, r5 = the camera passes. Seven needle slots k = 0-6, each drawn if `[[dash] + 8 + 12k]` and its model (`[dash + 1700]`, `1940`, `2660`, `2900`, `3620`, `3860`, `4100`) are set: stores `[dash + 5296 + 4k]` into the model + 48, angle `sub_820D3688([dash + 5328 + 4k], [[dash] + 12 + 12k], car)`, world `(B * RotZ(angle)) * A * car + 16` with A at dash + 32 + 240k and B 64 bytes on, drawn with `sub_820D06F0` (dash + 2176, 2416, 3136, 3376, 4336, 4576, 4816). In mode 5 (`[dash + 4]`) slot 1 is the turbo gauge: `[dash + 5324]` into the model, angle from car + 704: above 0, `[0x820062E0]` times it, capped at `[0x820062E0]` above 1; below the double at `0x820062D8`, `[0x820062D0]`; else value * `[0x820062CC]` * `[0x820062D0]`. |
| `sub_820D3688` | `Dial::Angle` | Native | `DialAngle` in `src/hooks/cockpit_dials.cpp`, called by the native `sub_820E45A8` (its only caller; the translated one runs only with `--native_cockpit_dials=false` or for `--check_native_cockpit_dials`). r3 = range, r4 = gauge (0-3: car + 704, 712, 716, 720), r5 = the car; returns f1, no state. `[0x82000A34]` with no range. Else from = range + 0, to = + 4, angles + 8, + 12: below from the first angle, above to the second, between `(v - from) / (to - from) * (a1 - a0)` (without `+ a0`, the game's own), all times `[0x82002948]`. |
| `sub_820D3740` | `Dial::TachometerAngles` | Native | `TachAngles` in `src/hooks/cockpit_dials.cpp`, called by the native `sub_820E42E8` (its only caller; the translated one runs only with `--native_cockpit_dials=false` or for `--check_native_cockpit_dials`). r3 = table, r4 = the car, r5 = [dash], r6 = &a, r7 = &b; no state. `a` from car + 696: `[0x82000A34]` below it; up to from (table + 0, or + 8 when `[[dash]]` isn't 0) value / from * table[16]; up to to (+ 4 or + 12) linear to table[20]; above, table[20]. `b` from car + 700: table[28] below table[24], linear over table[24..36] then table[32..44], clamped at table[44]. |
| `sub_82597610` | `Projector::Build` | Wrapped | r3 = out, r4 = eye, r5 = target, r6 = up (16-byte vectors), f1-f4 = field of view, aspect, near, far: out = look-at (`sub_82597378`) * perspective (`sub_825972A8`) * a fixed bias matrix at `0x8265EED0` (two `sub_8209BD50` multiplies). Called by the render orchestrator `sub_820C5DE8` (return `0x820C6A2C`) for the player's headlight once a view, and by `sub_820F34C8` (return `0x820F3944`) in a loop of up to 5. See "The headlight beam far from the origin" in `../frame-interpolation.md`. |
| `sub_82598768` | `TextureProjection::Fit` | Wrapped | Builds the c14 x4 texture projection, fitted to a camera object's view; called three times by `sub_820C5DE8` (returns `820C7174`, `820C7558`, `820C7950`; the third's result copied to c14 at `820C79F8`). Its multiply at `82599644` (stack temporary sp+352 = sp+352 * sp+208) reads back as a camera, w 1 with the eye at the origin, and was the group of cameras refused far from the origin; overwritten at `825996FC`, never drawn. See "The headlight beam far from the origin" in `../frame-interpolation.md`. |
| `sub_82597378` | `Matrix::LookAt` | | r3 = out, r4 = eye, r5 = target, r6 = up: a row-vector view matrix, forward = target - eye. |
| `sub_825972A8` | `Matrix::Perspective` | | r3 = out, f1 = field of view, f2 = aspect, f3 = near, f4 = far: y scale = 1 / tan(f1 * the constant at `0x82001BE4`), x scale = y scale / aspect, depth = far / (far - near), offset -near * far / (far - near). |
| `sub_820D06F0` | `CarPart::Draw` | | r3 = the part (returns if `[part + 4]`, its model, is 0), r4 = the camera, r5 = the world matrix, r6, r7: with r7 = 0, `sub_820D04E8([part + 4], camera, world, [part + 228], r6)`. |
| `sub_820D04E8` | `Model::Draw` | | Draws a model (r31 = r3): a visibility test (`sub_825951E0`), then the per-object recorder `sub_825953D0` (return address `0x820D0564`, which pairs products by r31). Culls with the camera it's given, so a world matrix away from the tick's can be culled. For car parts, `Model::RecordMaterials` (`sub_820CECA0`, `models.md`) for groups 0 and 1. |
| `sub_820DC958` | `BlinkTimers::Advance` | Wrapped | Skipped on render-only passes (`--per_tick_blink_timers`; "Per-frame logic" in `../frame-interpolation.md`). |
| `sub_8229E3F0` | `Device::Kick` | Overridden | `src/native/gpu_link.cpp`. r3 device, r4 (size in words, address) pairs, r5 their count. The kicked buffers are applied directly (only their effects outside the GPU) instead of written to the ring; the original is no longer called. `--native_renderer_log_gpu` logs. |
| `sub_8229E848` | `Device::DeferredKick` | Overridden | Same file. Queued kicks while GPU interrupts were outstanding; every kick completes at once now, so it kicks straight away; the original is no longer called. |
| `sub_8229DFF8` | | | Links a block into a list in the command buffer: r3 the list (+4, +8 its last), r4 the block, r5 and r6 stored in it; stores the block's address through the old last's pointer (the MMIO store macro, but not shown to be a register) and calls `sub_822AC470` twice. Only from the deferred kick `sub_8229E848`, whose native version doesn't call it: never called (attract race and play). Not read in detail. |
| `sub_8229DEA0` | `Device::OnInterrupt` | | The library's interrupt callback (r3 source: 0 vblank, 1 the command stream's INTERRUPT; r4 device): on a vblank counts +15148, counts down +15156 (zeroing callback block +4 at 0) and calls the callback at +15144; on an INTERRUPT calls callback block +16 with +20 and clears its CPU's bit. Reads interrupt status `7FC86544`. Never called (attract race and play, 2026-10-20, a counter): only `sub_822AD190` registered it (left out by native `Device::Init`), our kick's interrupts call the callback block's functions themselves, and there are no vblanks without the GPU plugin. Its override (logging only) was removed. |
| `sub_822B6BF8` | | Wrapped | The tiling worker's callbacks and job; logged with `--native_renderer_log_gpu`. |
| `sub_822B6A78` | | Wrapped | The tiling worker's callbacks and job; logged with `--native_renderer_log_gpu`. |
| `sub_822B6780` | | Wrapped | The tiling worker's callbacks and job; logged with `--native_renderer_log_gpu`. |
| `sub_8229EF48` | | Wrapped | Ends the command segment when the buffer is full; counted for `--native_renderer_check_draw_writes` (`src/native/draw_packets.cpp`). |
| `sub_822AD298` | `Device::Init` | Overridden | `src/hooks/graphics_device.cpp`, without its kernel video calls. r3 device, r4 parameters. Once at boot, from `sub_8229DB38`. `sub_822AD228`, two critical sections (+13548, +13576), stores the device in `VdGlobalDevice` (`820007B0`), the console's video flags into +15164 (`00040000`, `helpers::kVideoFlags`, in place of `ExGetXConfigSetting(3, 10)`, B5), `Device::SetUpRing` (r4 + 72), the worker threads (`sub_822B6D70`), `sub_822AC2A0`, `Device::SetUpDisplay`, `sub_822ACB90` (`Device::SetDefaultState`'s plain C++ with `--native_device_state`, `device-state.md`, since 2026-10-24), ends the command segment, the counter's rate and period as floats (+20040, +20044), debug callbacks (`sub_822B1E70`), `sub_822A3B90`, and `sub_822B43C0`; failures printed through `sub_822AC530`. Returns 1, or 0. Left out: `Device::InitEngines`, `RetrainEdram`, `VdIsHSIOTrainingSucceeded` (the "training failed" flag at `829537BC` stays 0), and handing +15168/+15172/+20024/+20028 to the kernel's debug monitor data (`820007F0`, which points to 0 without a debugger). Needed to draw: it creates the device (command buffer, surfaces, the worker threads). |
| `sub_822AD228` | | | From `Device::Init`: device +52 = 1, +10752 from `82638ED0`, +10760 from `sub_825B9A60` (not read), +15176 = `sub_822976F8(4800, B5800000)`; returns 0 if that failed. Not read in detail. |
| `sub_822AD190` | `Device::InitEngines` | Dropped | Left out by native `Device::Init`, its only caller. It only made `VdInitializeEngines` (twice, with a `VdShutdownEngines` between; the PFP and ME microcode at `82016148` and `820165C8`, callback `sub_822ACB48`), `ExRegisterTitleTerminateNotification(82639098, 1)` and `VdSetGraphicsInterruptCallback(sub_8229DEA0, device)` calls, which the SDK ignores without a GPU plugin. |
| `sub_822ACB48` | | | The callback given to `VdInitializeEngines`: writes GPU registers `7FC83214` = 7 and `7FC83408` = 2048. The SDK never calls it. Not needed to draw: never called (attract race and play). |
| `sub_822ACB68` | | | `ExRegisterTitleTerminateNotification(r3, 0)` and `VdShutdownEngines`. No callers in code: probably the terminate notification's routine (`82639098`), its address only in the image's data. Never called in an attract race. Not needed to draw: never called (attract race and play). |
| `sub_8229F280` | `Device::SetUpRing` | Overridden | Same file. r3 device, r4 parameters (+4 ring size, +8 a ring, +12 command buffer size, +16 a command buffer, +20 segments; 0 only frees). Waits (`sub_8229F1B0`), frees the old buffers, allocates any ring or command buffer not given (the game gives both), the progress block (96 bytes, +10768) and the callback block (32 bytes, +10772), sets the ring's (+13496..) and command buffer's (+13504..) fields and the segment (+40, +44, +48), and writes the scratch registers' set-up into the command buffer (`sub_8229F0E0` after it). Returns 0, or `8007000E`. Left out: `VdInitializeRingBuffer`, `VdEnableRingBufferRPtrWriteBack`, `VdSetSystemCommandBufferGpuIdentifierAddress`, `VdVerifyMEInitCommand` (imported as `KiApcNormalRoutineNop`), `MmGetPhysicalAddress`, and ME_INIT written into the primary ring (`sub_8229DD70`), which nothing reads. Needed to draw: the command buffer, progress block (the fences our kick writes) and callback block (the scratch set-up our kick uses for the boot callback). |
| `sub_822ACE00` | `Device::SetUpDisplay` | Overridden | Same file. r3 device, r4 parameters (+0 width, +4 height, +8 render target format, +16 passed to both surfaces, +36 non-zero for a depth surface, +40 its format, +56 and +60 non-zero to skip the render target and the front buffer (the game sets +60), +64 front buffer format, +68 display format, +96 the scaler's). The display mode is ours (`src/display/display.h`, 1280x720) into +19988..+20000 instead of `VdQueryVideoMode`; creates the render target (`sub_822A5E28`, set with `sub_822A3D30`) and depth surface (set with `sub_822A4060`; both through their plain C++ with `--native_device_state`, `device-state.md`, since 2026-10-24); copies the parameters to +13188 (124 bytes); the scaler's parameters (`sub_822B42D0`); sets +10809 bit 4 (display mode to set at the next Swap). Returns 1, or 0. Once at boot. Needed to draw. |
| `sub_822AD0B0` | `Device::SetDisplayMode` | Overridden | Same file. r3 device, r4 parameters, r5 set. Runs the default gamma ramp through the display's gamma (`sub_822B0E08` → `sub_822A3A20`, `Device::SetGammaRamp`'s plain C++ with `--native_device_state` since 2026-10-24, `device-state.md`; or for formats 7 and 54 `sub_822B0E60` → `sub_822A3AA8`), and puts our display size in +19988..+20000 (format 0, as the SDK's answer gave). Left out: `VdSetDisplayMode` and `VdGetCurrentDisplayInformation`. Once, from the first Swap. Not needed to draw, but kept for the gamma ramps (kept native for `--xbox_gamma`). |
| `sub_822B0910` | `ComputeGammaRamp` | Overridden | Plain C++ (`src/display/gamma.cpp`) behind an adapter (`src/hooks/gamma.cpp`); checked by the `gamma` harness. r3 out, r4 in (three channels of 256 16-bit values; may be the same), r5 inverse. A 1024-entry 10-bit curve from the display's gamma (1 sRGB, 2 BT.709, 3 power; ours: 2), then `out = curve[in >> 6] << 6`. With r5 = 0 the ramp's values are decoded from sRGB first; with r5 ≠ 0 the display's curve is inverted and the result encoded to sRGB. The tables `--xbox_gamma` copied. Once at boot, from `sub_822A3A20`. Not needed to draw today (`--xbox_gamma` uses fixed tables captured from the game), but kept native so it can follow the game's ramp later. |
| `sub_822B0AB0` | `ComputeGammaRampPwl` | Overridden | Same files. The same curve for a 128-entry piecewise-linear ramp (three channels of (base, delta) pairs): base `curve[base >> 6] << 6`, delta `(curve[min(base>>6 + delta>>6, 1023)] − curve[base >> 6]) << 6`, wrapped to 16 bits. Never called in play. Not needed to draw today (`--xbox_gamma` uses fixed tables captured from the game), but kept native so it can follow the game's ramp later. |
| `sub_822B07B0` | `SrgbCurve` | Native | `display::Srgb` (`src/display/gamma.cpp`). f1 x, r4 to linear; returns f1 (float). To linear: x > 0.04045 → ((x + 0.055) × (1/1.055))^2.4, else x / 12.92; from linear: x > 0.0031308 → 1.055 × x^(1/2.4) − 0.055 (fused), else x × 12.92. Called only by the two gamma ramps, now native. |
| `sub_822B0860` | `Bt709Curve` | Native | `display::Bt709`, same file. To linear: x ≥ 0.0812 → ((x + 0.099) × (1/1.099))^(1/0.45), else x / 4.5; from linear: x ≥ 0.018 → 1.099 × x^0.45 − 0.099 (fused), else x × 4.5. Called only by the two gamma ramps, now native. |
| `sub_822A3AA8` | | | Calls `ComputeGammaRampPwl`; from `Device::SetDisplayMode` for formats 7 and 54. Not read. |
| `sub_822B0E08` | | | Fill a ramp (r3, r4 = 0) before `Device::SetDisplayMode` runs it through the gamma functions; also called by `sub_822A05A8`. Probably the default (identity) ramps. Not read. |
| `sub_822B0E60` | | | Fill a ramp (r3, r4 = 0) before `Device::SetDisplayMode` runs it through the gamma functions; also called by `sub_822A05A8`. Probably the default (identity) ramps. Not read. |
| `sub_822A08A0` | `Device::Swap` | Overridden | `src/hooks/graphics_device.cpp`, without its kernel video calls. r3 device, r4 the front buffer (a texture header), r5 parameters; r3 returned as `sub_822A3B90` leaves it (`Renderer::Present` and `sub_8209ACB8` ignore it, `sub_822A0E50` returns it). Once a frame, only from `Renderer::Present` (`sub_8258DC58`, `8258DCA4`) in play; also from `sub_822A0E50` (r5 = device +13284), the tiling worker `sub_822B69A8` (r4 = device +13444, never: needs +10810 bit 2) and `sub_8209ACB8` (r5 0). Kept: the header's 52 bytes copied to +13444 (the game's memcpy); `Device::SetDisplayMode` with +10809 bit 4 (first Swap), then the bit cleared; with +10810 bit 5 (first Swap) +13312 (56 bytes) and the bit cleared; +10811 bit 7 cleared; ends the segment (`sub_8229EF48`), `sub_8229EC00(device, +13512 >> 2)`, clears +10809 bit 5, fences +13540 (the position before the segment end) and +13544 (old +13540), `sub_822A3B90(device, 0)`. Left out (each step in "Swap, step by step" under B3 in `docs/rexglue-removal.md`): the timestamps `sub_822B1F68` (+20080, never), the segment end with `829537C0` (never), the capture check `sub_822B5C58`, `VdGetSystemCommandBuffer` and `VdSwap` with their 64 words (`sub_8229F148`; +20120, +20432), `sub_822B42D0` when r5 differs from +13284 (never), `Device::SetUpScaler` and its 10 words, `sub_822A05A8` (never), register `0x1844` and +20004 (every Swap), the `7FC86110` write (+10809 bit 1, never), REG_RMW, `VdPersistDisplay` and `MmFreePhysicalMemory` (first Swap), `RetrainEdram` and the frame statistics `sub_822B20D8`. Checked against the recompiled Swap: the same fields but +20004, +20024 and +20028, and one segment fewer at the first Swap (the gamma ramp's packets fit without `VdSwap`'s words). |
| `sub_822B2388` | `RetrainEdram` | Dropped | `src/hooks/graphics_drops.cpp`: returns 1 (Swap ignores it). r3 device, r4 and r5 (0, 0 from Swap; 1, 1 from `Device::Init`, which no longer calls it). Under a critical section, `VdRetrainEDRAMWorker` (if r5), then `VdRetrainEDRAM` in a loop writing into the command buffer while it returns non-zero; the SDK returns 0, so it only took and released the lock. Once a frame. |
| `sub_8229B848` | `Device::SetClockGating` | | r3 device, r4 on. If changed: waits, `VdEnableDisableClockGating`, and when off two REG_RMW packets (registers 129, 130, from `829537E4` and `829537E8`); +10810 bit 1. From `sub_822B1788` and `sub_822B20D8`. Never called in an attract race. Not needed to draw: never called (attract race and play). |
| `sub_822B2470` | `ComputeScalerRect` | Dropped | r3 device, r4 and r5 front buffer size, r6 parameters, r7 output (56 bytes). The scaler's rectangle and output height (`VdQueryVideoFlags`). Twice at boot. No longer called: their only callers (`sub_822B2610`, `sub_822B3FE0`) are dropped. |
| `sub_822B2610` | | Dropped | Same file: returns 0, as the SDK's `VdCallGraphicsNotificationRoutines` did (its caller `sub_822B4250` passes it on, and that caller's callers ignore it). It passed the front buffer and display sizes to that call after `sub_822B2470`. Once at boot. |
| `sub_822B3A78` | | Dropped | The scaler's filters (`VdQueryVideoMode`, `sub_822B3798` twice). Not read in detail. Once at boot, from `sub_822B3FE0`. No longer called: their only callers (`sub_822B2610`, `sub_822B3FE0`) are dropped. |
| `sub_822B3FE0` | `Device::SetUpScaler` | Dropped | Same file: does nothing (Swap ignores the return). The 360's output scaler: `sub_822B2470`, `sub_822B3A78`, `RtlFillMemoryUlong`, then the scaler's command buffer (`VdInitializeScalerCommandBuffer`, NOPs from the SDK) and display register packets written into the command buffer, which our renderer doesn't read. Once at boot, from Swap. |
| `sub_822B4F10` | | | Writes a file with a 2048-byte header (display information, gamma ramp; `sub_82295D50`, `sub_82294E20`), deletes a symbolic link. Probably a debug capture. From `sub_822B58F0`, `sub_822B5C58`, `sub_822ACFB8` and `sub_822B58C0`. Never called in an attract race. Not read in detail. Not needed to draw: never called (attract race and play). |
| `sub_822B8230` | | | The shader build that `sub_822B8868` wraps: r3-r10 its arguments; r3 < 0 for a failure. No longer called with `--use_hardcoded_shaders` (`sub_822B8F10`). Not read. |
| `sub_822B8D28` | | | Calls `sub_822B8868`, and again with r6 bit 2 set when the first returns `88760B6C` and bit 2 wasn't set. No longer called with `--use_hardcoded_shaders` (`sub_822B8F10`). Not read in detail. |
| `sub_822B8DA0` | | | Probably D3DXCompileShader's body (`sub_822B8F10` passes its arguments on): a profile not starting with `v` or `p` sets flag `0x10000`; calls `sub_822B8D28`, then hands back the outputs, releasing the ones not asked for through their vtables. No longer called with `--use_hardcoded_shaders` (`sub_822B8F10`). Not read in detail. |
| `sub_822B8F10` | `D3DXCompileShader` | Overridden | `src/hooks/shader_compiles.cpp` (`--use_hardcoded_shaders`, on): the stored shader for the source, length and profile (`src/graphics/hardcoded_shaders.h`, `compiled_shaders.h`; made by `tools/compiled_shaders.py` from `--dump_shader_compiles`, which compiles and dumps instead) in a buffer from `D3DXCreateBuffer` (`sub_822BE450`); no error buffer or constant table (0 in their outputs); r3 0. Harness `hardcoded_shaders`: 11 of 12 the same as a live compile, the HSV gradient's (`82071528`) constants in another order when compiled during a race (expected to differ). The font drew right in the user's session (2026-10-25). The D3DX HLSL compiler's entry, linked into the game: r3 the source, r4 its length, r5 the defines, r6 the include handler, r7 the entry point (`main`), r8 the profile (`vs_3_0`, `ps_3_0`, `ps_2_0`), r9 the flags, r10 and the caller's stack words +84 and +92 the outputs (the shader buffer, the errors, the constant table). 14 calls at boot of 12 sources: `PrimitiveDrawer::Init` `sub_825A1438` (7), `Font::CreateShaders` `sub_821088B8` (5), `sub_8209BE88` (2, the same as two of the drawer's); no defines, include handler or flags; no constant table asked for; the only error text the "unused inputs" warning (four of the drawer's, printed through `OutputDebugStringA`, dropped). The sources are HLSL text in the image (textured and plain colored quads, the font's one-channel alpha, a pass-through vertex shader with a WVP matrix, a depth view `(texel.r - 0.1) / 3000`, an HSV gradient). The game's own shaders don't come through here (`sub_824846D8`). |
| `sub_822BA5A0` | | | Probably the HLSL preprocessor's directive reader: branches on the directive's first letter (`d`, `e`, `i`, `l`, `p`, `u`: define, else/endif, if/include, line, pragma, undef). No longer called with `--use_hardcoded_shaders` (`sub_822B8F10`). Not read in detail. |
| `sub_822BE278` | `D3DXBuffer::Release` (probably) | | Vtable `0x82019DC0` slot 2: decrements the count at +4 and, when it was 1, calls slot 5 with r4 = 1 (the deleting destructor, `sub_822BE3E8`); returns the new count. Still runs: `D3DXCompileShader`'s override (`sub_822B8F10`) makes its buffers with `D3DXCreateBuffer` (`sub_822BE450`). Seen in play. |
| `sub_822BE2D0` | | | Vtable `0x82019DC0` slot 6: allocates r4 bytes (`XMemAlloc`, `sub_822976F8`, attributes `0x24810000`) into +12 and puts the size at +8; returns `0x8007000E` when the allocation fails, else 0. From `D3DXCreateBuffer` (`sub_822BE450`). Seen in play. |
| `sub_822BE3E8` | `D3DXBuffer::~D3DXBuffer` | | Vtable `0x82019DC0` slot 5: frees the data (+12, `XMemFree`), and the object when r4 bit 0 is set. |
| `sub_822BE450` | `D3DXCreateBuffer` | | r3 the size, r4 where the buffer goes. A 16-byte object (`XMemAlloc`): +0 the vtable `0x82019DC0`, +4 the reference count (1), +8 the size, +12 the data, allocated through vtable slot 6 (`sub_822BE2D0`); on failure releases it through slot 5 and returns the error (`0x8876086C` for a null r4). Called by `sub_822B8F10` for its shader buffers, and by the compiler. Not read in detail. |
| `sub_8235A688` | | | Probably XGraphics' vertex buffer header set-up: clears the 32-byte header at r7; +0 = 1 (type 1), or `0x00200001` with r4 bit `0x4`, and r4 bit `0x200` adds `0x00400000`; +4 = 1; +20 = `0xFFFF0000`; +24 = r6 \| 3 (the fetch constant's address); +28 = (r3 & `0x03FFFFFC`) \| `0x10000000` \| 2, keeping its bit `0x08000000`. From `sub_8258A9E8` (no row). Seen in play. Not read in detail. |
| `sub_825DA090` | | | A static initializer (from the C runtime's start-up): `sub_82411640(0x82954DD4, 109)`. That global is read only by D3DX functions that never ran (`sub_82372790`, `sub_823B7388` and others). Seen in play. |
| `sub_825DA0A0` | | | A static initializer: `sub_8240FBA0(0x82954DF8, 0x82954DD4)`. That global is read only by D3DX functions that never ran (`sub_82393008`, `sub_823964E0`). Seen in play. |
| `sub_82411640` | | | In D3DX: clears 36 bytes at r3 and puts r4 into +8's bits 7-13, setting bit `0x00400000`. From `sub_825DA090` only. Seen in play. |
| `sub_8240FBA0` | | | In D3DX: clears 20 bytes at r3, links r3 and r4 (r3+16 = r4, r4+4 = r3) and sets +0's top bit. From `sub_825DA0A0` only. Seen in play. |
| `sub_822BC8A8` | | | The HLSL compiler's grammar actions (a switch on r4, 46 rules): one calls `sub_822BC588` (`#include`). No longer called with `--use_hardcoded_shaders` (`sub_822B8F10`). Not read in detail. |
| `sub_822A0688` | | | Reads GPU registers at `7FC8xxxx`. No callers in code. Not read. Never called (attract races and play, 2026-10-20, a counter). |
| `sub_822AD500` | `Device::Release` | | r3 device. Waits, frees, `sub_822ACFB8`, `sub_822B58C0`, clears the interrupt callback, `sub_8229F280(device, 0)`, unregisters the terminate notification, `VdShutdownEngines`. From `sub_8229D978`. Never called in an attract race. Not needed to draw: never called (attract race and play). |
| `sub_8229DB38` | | | Probably Direct3D's CreateDevice: r3 0, r4 1, r5 0, r6 0, r7 the presentation parameters, r8 where the device goes; calls `Device::Init`; returns a negative value on failure. From the display set-up `sub_8258D568` (`8258D6C8`), once at boot. Not read in detail. |
| `sub_8229DA20` | | | Stores r4 at device +15144: the callback the interrupt handler `sub_8229DEA0` calls on vblanks. From the display set-up with `sub_8258D4D8`. |
| `sub_8229D978` | | | Calls `Device::Release`. Called by `sub_8229DB38`, `sub_824829A0`, `sub_82491758`, `sub_82491A00`, `sub_8258DBA8`. Probably the device's reference count drop. Not read. |
| `sub_8229F148` | | | Reserves r4 words in the command buffer: ends the segment if +40 + 4·r4 passes the end (+44), then a new segment (`sub_8229EC00`) if still short; returns the write position (+40; the words are written after it), or 0. From Swap (64 words for `VdSwap`), `sub_8229B9C8`, `sub_8229BFE8`, `sub_822AEBF0`, the gamma packets `sub_822B0C38`/`sub_822B0D18`, `RetrainEdram`, `Device::SetUpScaler`, `sub_822B5208`. |
| `sub_8229DD70` | | | Writes r5 words from r4 into the primary ring at its write position (+10820, wrapping with +13500), after waiting for room (`sub_8229DCC0`); doesn't write CP_RB_WPTR. From `Device::SetUpRing` (ME_INIT, no longer) and `sub_822B6370` (not read). |
| `sub_8229DCC0` | | | Waits for room in the primary ring, reading its read pointer from progress block +60 (see "Plan for milestone 6" in `docs/native-renderer.md`). Not read in detail. |
| `sub_8229F0E0` | | | Writes a WAIT_UNTIL (register `0x5C8` = `0x20000`) and register `0xD04` = 0 into the command buffer, ending the segment first if past +48 (`sub_8229EF48`). Only from `Device::SetUpRing`. |
| `sub_8229F1B0` | | | The full GPU wait (spins on device +10868; see "Plan for milestone 6" in `docs/native-renderer.md`); passes at once in exclusive mode. From `Device::SetUpRing`, `Device::Release`, `Device::SetClockGating`, `RetrainEdram`. Not read in detail. |
| `sub_8229E908` | | | Waits for room in the command buffer, comparing progress block +0 with the command position (+10780) (see "Plan for milestone 6"). From `sub_8229EC00`. Not read in detail. Also a fence wait: r3 the device, r4 a fence (0: none), r5 a code, r6 a resource; passes at once when the fence is at or before the one the GPU is done with (device +10768's word, against +10780), else may end the segment (`sub_8229EF48`) and wait. From `Resource::Lock` and `Resource::Destroy`, whose native code (`graphics-resources.md`) leaves the call out (Dropped, the user's call). |
| `sub_822AC470` | | | The data cache flush over r3 to r4 (128-byte lines, eight a step; returns at once for r3 from `0x7F100000` to `0x86FFFFFF`); its translation writes only its own stack and leaves r3. From `Device::SetUpRing` when a command buffer was already set up (r3 and r4 its first and last physical addresses minus `0x40000000`) and `Resource::Unlock`, whose native code (`graphics-resources.md`) leaves the call out (Dropped, the user's call: no data cache to flush on PC). |
| `sub_822B6D70` | | | From `Device::Init`: creates the library's worker threads (`ExCreateThread`, `KeSetBasePriorityThread`) and ends the command segment; 0 on failure. Not read in detail. |
| `sub_822B6370` | | | Calls `sub_8229DD70` (writes into the primary ring). Not read. |
| `sub_822AC2A0` | | | From `Device::Init` (r3 device); writes into the command buffer (ends the segment through `sub_8229EF48`). Not read. |
| `sub_822B43C0` | | | From `Device::Init` (r3 device); a 0 return makes it print a warning (the string at `82014180`). Not read. |
| `sub_822B1E70` | | | From `Device::Init` (no arguments): passes two callbacks (28: `822B1AC8`; 47: a block holding 2 and `822B1CB0`) to a function from the kernel variable at `820007B4`, or from the debug monitor data (`820007F0`, +24) if that's set (it points to 0 without a debugger), and clears the global at `82953D14`. Not read in detail. |
| `sub_822AC530` | | | The library's debug print (r3 a format): `_vsnprintf` into a 260-byte buffer, then `DbgPrint` (see B6). From `Device::Init`'s failures (ours too), `sub_8229DEA0`, `sub_822AC5B8`, the "GPU hung" handler `sub_822ACAA8`, the capture `sub_822B1788` (Dropped) and mounting `sub_822B58F0` (never called). Never called in play (two attract races and two sessions the user played, 2026-10-21). |
| `sub_825B9A60` | | Overridden | The current thread's id (r13 +256's +332). Leaf. From `sub_822AD228` (its result goes to device +10760; in play +10760 reads 0, the main thread 3), `Resource::Lock` (whose native code leaves it out with the packet it picks a path for: Dropped, `graphics-resources.md`) and `XACTEngine::CheckNotInCallback` (`xact.md`). Native in `src/graphics/video_output.cpp` (`CurrentThreadId`, `--native_video_output`, harness `video_output`): the guest thread's id through the SDK's `XThread::GetCurrentThreadId` (the same word: the thread object's +332); called directly by `Device::AcquireThreadOwnership`'s plain C++ (`video-output.md`). The adapter stays (`sub_822AD228` and XACT are recompiled). |
| `sub_8229DDD8` | | | Widens the device's pending cache flush range (r3 = device +11560: a 64-bit start and end, the start the smaller, the end the larger) to r4-r5, by a compare-and-swap. From `Resource::Lock` off the device's thread (always in play), whose native code (`graphics-resources.md`) leaves the call out (Dropped, the user's call: the renderer skips coherency packets). |
| `sub_822B42D0` | | | The scaler's parameters: r3 device, r4 them (presentation parameters +96); calls `sub_822B2598`, then `sub_822B4250` (which sets +10810 bit 5, the scaler set up at the next Swap). From `Device::SetUpDisplay`; from Swap only when r5 differs from +13284 (never). Not read in detail. |
| `sub_822B4250` | | | Copies 56 bytes (probably the scaler's parameters into the device) and calls `sub_822B2610` (Dropped), returning what it returns. Not read in detail. |
| `sub_822B2598` | | | From `sub_822B42D0`. Not read. |
| `sub_822B3798` | | | From `sub_822B3A78` (twice): the scaler's filters. Not read. |
| `sub_822B0760` | | Native | One piecewise-linear gamma ramp entry (r3 out, r4 in, r5 the curve), from `sub_822B0AB0`: base `curve[in.base >> 6] << 6`, delta the curve's rise to `min(base>>6 + delta>>6, 1023)` `<< 6`. `display::ApplyGammaCurve` does it natively; no longer called. |
| `sub_822B0D18` | | | Called by `sub_822A3A20` and `sub_822A3AA8` after their gamma ramp. Not read. |
| `sub_822A05A8` | | | r3 device, r4 front buffer format, r5 a ramp (or 0). The display's gamma packets: REG_RMW on `0x1841` (bit 8 for format `28280136`), register `0x1930` (3 for that format, else 0), then the ramp (the default from `sub_822B0E08`/`sub_822B0E60` when r5 is 0) through `sub_822B0D18` or `sub_822B0C38`. Only from Swap, with +10811 bit 7, which no code sets: never called. Not needed to draw: display registers. The native Swap leaves the call out: no callers left. |
| `sub_822B5C58` | | Dropped | The capture check: r3 device, r4 the front buffer. With the byte at `82953E1C` set: tail-calls `sub_822B5208` (device +20424 bit 3) or the mounting `sub_822B58F0`; else `sub_822B4F10` with bit 3; else returns. No code writes either (0 in play), so it returns. Was called once a frame, only from Swap. The native Swap leaves the call out (before that, skipped behind a temporary switch: nothing changed). |
| `sub_8229EC00` | | | The next segment and the frame throttle: r3 device, r4 words (Swap: +13512 >> 2). With +10809 bit 6: the write position at +15176. Else the next segment from `sub_8229E100` (or `sub_822AD690`/`sub_822AD8F8` through the object at +13160), +13528 its start, +13516 and +13532 zeroed, write position +40, end +44, soft end +48 (+44 − 160). Then waits (`sub_8229E908`) for the fence +13540 (+13544 with +20440 bit 0) and zeroes it. Returns the segment (0 on failure: bit 6 set). From Swap, `sub_8229F148`, `sub_8229EDA8`. Needed to draw: the next frame's commands go there; the wait passes at once in exclusive mode. |
| `sub_822ACFB8` | | | From `Device::Release` (never called); both reach `sub_822B4F10`. Not read. |
| `sub_822B58C0` | | | From `Device::Release` (never called); both reach `sub_822B4F10`. Not read. |
| `sub_822B1788` | | Dropped | The capture's state machine (`82953D10`, states 11-17: `sub_825B9AA8`/`sub_825B9B30`, `sub_822B1718`, the full wait, the +19980 object released, clock gating, messages through `sub_822AC530`). Otherwise hands the frame count (+20024) to the debugger through `820007B4`/`820007F0` (none). State 0 in play, so nothing. Only from `sub_822B20D8` (Dropped), so no longer called. Not read in detail. |
| `sub_822A0718` | | | Before every Swap, from `VideoOutput::WaitForVblank` (and the movies' `sub_822A0E50`, the tiling worker, `sub_8209ACB8`). r3 device. If +13240 isn't `80000000` and +13476 (the header copied by Swap) holds an address: `sub_822B1FF8(device, 1)`, scratch register 1 = 1 (our kick writes it back to callback block +4), WAIT_REG_MEMs on that word around `sub_8229E9D0` (from +13240 and +11596). Probably the GPU's wait for the vblank interval. Sets +10809 bit 5, which Swap clears. Not read in detail. |
| `sub_822B1F68` | | | GPU timestamp: r3 device, r4 an address (progress block +64..+92). Writes EVENT_WRITE_SHD with initiator `80000003` (bit 31: the GPU's swap counter), that address and `DEADBEEF`. Only from Swap, with +20080 set by the debugger (never). The native Swap leaves the call out: no callers left. |
| `sub_822B1CB0` | | | The library's debugger command handler (r3 command, r4 argument; the device from `VdGlobalDevice`): 0 and 16 turn statistics off, 17 on (with 6: +20080 = 1, the GPU timestamps), 34 stores +20124, others query `sub_822B1088`. No callers in code: handed to the debug monitor by `sub_822B1E70`, which doesn't happen without a debugger. |
| `sub_822B1088` | | | Statistics query for the debugger: r3 device, r4 0-17; floats from +20024, +20028, +20040, +20064, +20068, +20120 and the performance counters (`sub_822B0F70`). Only from `sub_822B1CB0`. Not read in detail. |
| `sub_822B0F70` | | | Samples the GPU performance counters (+20100, +20108, +20112, `sub_8229BFD0`, `sub_8229D688`; +20104 0 → 1 starts them) once per frame count (+20116). Only from `sub_822B1088` (the debugger). Not read in detail. |
| `sub_822B1FF8` | | | With +20104 = 2 (performance counters on; never without a debugger): command buffer packets for the next counter sample. From `sub_822A0718`. Not read in detail. |
| `sub_822B1718` | | | Capture: the full wait, then an object from `sub_822B7AF8` stored in +19980, given the progress block's physical address. From `sub_822B1788` (states 13-15). Not read in detail. |
| `sub_822B1AC8` | | | Two `sprintf` calls, `sub_822B43E8` and one indirect call; no direct caller (through a pointer). Never called in play (two attract races and two sessions the user played, 2026-10-21). Not read. |
| `sub_822B69A8` | | | The library's worker thread (r3 its block). Waits on its event (`KeWaitForSingleObject`, block +32); on a timeout (258), under the device's lock (+13548), when +10810 bit 2 is set (no code sets it): `sub_822A0718` and Swap with the header at +13444 (shows the last frame again); else `sub_822B6780`. Its event is set by the interrupt callback `sub_822B6A78`. Not read in detail. |
| `sub_822B5208` | | | From `sub_822B5C58` (capture, device +20424 bit 3; never), its only caller, now Dropped: no callers left. Reserves words with `sub_8229F148`. Not read. |
| `sub_822A0E50` | | | Movies (per Swap's callers): `sub_822A0718`, `sub_822A3450`, then Swap with +13428 and +13284. Never seen calling Swap. Not read in detail. |
| `sub_8209ACB8` | | | Calls `sub_822A0718`, `sub_8258D4A8`, then Swap (its result as the front buffer, r5 0). No direct callers. Not read in detail. |
| `sub_8229E100` | | | The next command buffer segment, for `sub_8229EC00` (r4 2, r5 the size, r6 32); sets +10810 bit 3 among others. Not read in detail. |
| `sub_822ACAA8` | | | The library's "GPU hung" handler: after a wait times out, sets +10809 bit 1 and the progress to position − 2; reads +20024; calls `sub_822AC918`, which reads and writes GPU registers. Never called (attract race and play, 2026-10-20): our kick completes every wait. Not read in detail. |
| `sub_822AC918` | | | From the "GPU hung" handler `sub_822ACAA8`: reads and writes GPU registers (1488, 3878, 1403, ...) through `sub_822AC590` and `sub_822AC5A0` (or a function given in r5); probably a dump of the GPU's state for a debugger. Never called (attract race and play). Not read in detail. |
| `sub_822AC590` | | | Reads GPU register r3 (`7FC80000` + 4·r3) with a plain load, so not through any MMIO handler. Only through a pointer in `sub_822AC918`. Never called (attract race and play). |
| `sub_822AC5A0` | | | Writes GPU register r3 = r4 (`7FC80000` + 4·r3). Only through a pointer in `sub_822AC918`. Never called (attract race and play). |
| `sub_822AC638` | | | Formats with `sprintf`. Only from `sub_822AC918` (the "GPU hung" dump, never called). Never called in play (two attract races and two sessions the user played, 2026-10-21). Not read. |
| `sub_8229B930` | | | GPU performance counters, from `sub_822B20D8` while +20104 is 1 or 2 (never without a debugger); `sub_8229B9C8` and `sub_8229BFE8` reserve words with `sub_8229F148`. Not read. |
| `sub_8229B9C8` | | | GPU performance counters, from `sub_822B20D8` while +20104 is 1 or 2 (never without a debugger); `sub_8229B9C8` and `sub_8229BFE8` reserve words with `sub_8229F148`. Not read. |
| `sub_8229BFE8` | | | GPU performance counters, from `sub_822B20D8` while +20104 is 1 or 2 (never without a debugger); `sub_8229B9C8` and `sub_8229BFE8` reserve words with `sub_8229F148`. Not read. |
| `sub_8229E9D0` | | | From `sub_822A0718` (r4 the write position, r6 `822A0688`, r7 from +13240 and +11596); writes packets. Not read. |
| `sub_825BED08` | | | Graphics library; calls `sub_822A3B90`, sets +10811 bit 6. Not read. |
| `sub_825BE708` | `DrawIndexedVertices` | Overridden | Exclusive mode, without its packets (`src/native/draw_packets.cpp`). Predicated draws (the device's tiling flags, bit 2) and `--native_renderer_library_packets` still run the original there, so it runs in play (coverage, 2026-10-21). |
| `sub_822A8808` | `Device::SetVertexShader` | Wrapped | Same file: marks a rebind of the bound shader, so `AddRef` and `Release` skip it. |
| `sub_822AE340` | | Wrapped | `src/native/replay_hooks.cpp`: keeps the patched copy of a vertex shader's microcode (made when the GPU may still use the original) for the native renderer. |
| `sub_825BD8F0` | `BeginVertices` | Wrapped | Same file: an inline vertex draw noted, then handed to the native renderer (in commands 15 and 19). |
| `sub_825BDDA0` | `EndVertices` | Wrapped | Same file: an inline vertex draw noted, then handed to the native renderer (in commands 15 and 19). |
| `sub_8258F5C0` | | Wrapped | Same file: the single-command items' command 15 (the helper object's draw), marked as command 15 so the native renderer draws its inline vertices. |
| `sub_822AFA58` | `Device::Resolve` | Wrapped | Same file: resolves made outside command 25 (command 33's, one per tile) passed to the native renderer. |
| `sub_8220C1E0` | `MenuList::Update` | Wrapped | Logged for `--log_pause_menu` (`src/debug/pause_menu_log.cpp`). `sub_8210C370` reads logical pad `j`'s values 0-7 (sticks, triggers) through the pointer at mapper +2880 (= input manager +44 + 196j) and the buttons through the listener object at mapper +2464 (seen by the read watch, 2026-10-19). |
| `sub_821BE5A8` | | Wrapped | The pause sub-menus' calls on their UI movie (setting animation targets and the children's +80 flags, starting animations, switching sub-screens); logged for `--log_pause_movie` (`src/debug/pause_movie_log.cpp`). |
| `sub_8220D040` | | Wrapped | The pause sub-menus' calls on their UI movie (setting animation targets and the children's +80 flags, starting animations, switching sub-screens); logged for `--log_pause_movie` (`src/debug/pause_movie_log.cpp`). |
| `sub_8220BA68` | | Wrapped | The pause sub-menus' calls on their UI movie (setting animation targets and the children's +80 flags, starting animations, switching sub-screens); logged for `--log_pause_movie` (`src/debug/pause_movie_log.cpp`). |
| `sub_8258FBD0` | `Command::*` (1) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_8258FC30` | `Command::*` (2) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_8258FC90` | `Command::*` (3) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_8258FCE0` | `Command::*` (4) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_8258E9F8` | `Command::*` (5) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_8258EA20` | `Command::*` (6) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_8258FF50` | `Command::*` (8) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_8258FFA0` | `Command::*` (9) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_8258FFF0` | `Command::*` (10) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590040` | `Command::*` (11) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_825900B8` | `Command::*` (12) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_8258EA58` | `Command::*` (13) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590108` | `Command::*` (15) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_825901B0` | `Command::*` (16) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590268` | `Command::*` (17) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_825902E0` | `Command::*` (18) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590360` | `Command::*` (19) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_825903D8` | `Command::*` (20) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_8258EAD0` | `Command::*` (21) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82591798` | `Command::*` (22) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590428` | `Command::*` (23) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590478` | `Command::*` (24) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_825904C8` | `Command::*` (25) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590620` | `Command::*` (26) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590698` | `Command::*` (27) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_8258EAE0` | `Command::*` (28) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590768` | `Command::*` (29) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590860` | `Command::*` (30) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_825908E0` | `Command::*` (31) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590968` | `Command::*` (32) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_825909E8` | `Command::*` (33) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590A68` | `Command::*` (34) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_82590AD0` | `Command::*` (35) | Wrapped | The render command handlers, by id (`REPLAY_COUNTED_HANDLER`, `src/native/replay_hooks.cpp`): the native renderer and the stats logger see every command. Command 7 is `sub_8258FD48` above; 0, 36 and 37 aren't wrapped. The commands: "Command list format" in `../frame-interpolation.md` here and `gpu-functions.md`. |
| `sub_822AE1D0` | | Wrapped | The draw's packet writers (r3 the device: shader constants, register blocks, fetch constants, HiZ and surface set-up; `NATIVE_CHECKED_HELPER`, `src/native/draw_packets.cpp`): what they change marked for the native renderer; checked with `--native_renderer_check_draw_writes`. |
| `sub_822ADE30` | | Wrapped | The draw's packet writers (r3 the device: shader constants, register blocks, fetch constants, HiZ and surface set-up; `NATIVE_CHECKED_HELPER`, `src/native/draw_packets.cpp`): what they change marked for the native renderer; checked with `--native_renderer_check_draw_writes`. |
| `sub_822ADEF0` | | Wrapped | The draw's packet writers (r3 the device: shader constants, register blocks, fetch constants, HiZ and surface set-up; `NATIVE_CHECKED_HELPER`, `src/native/draw_packets.cpp`): what they change marked for the native renderer; checked with `--native_renderer_check_draw_writes`. |
| `sub_822AE070` | | Wrapped | The draw's packet writers (r3 the device: shader constants, register blocks, fetch constants, HiZ and surface set-up; `NATIVE_CHECKED_HELPER`, `src/native/draw_packets.cpp`): what they change marked for the native renderer; checked with `--native_renderer_check_draw_writes`. |
| `sub_822ADC08` | | Wrapped | The draw's packet writers (r3 the device: shader constants, register blocks, fetch constants, HiZ and surface set-up; `NATIVE_CHECKED_HELPER`, `src/native/draw_packets.cpp`): what they change marked for the native renderer; checked with `--native_renderer_check_draw_writes`. |
| `sub_825BE300` | `DrawVertices` | Wrapped | Logged when called outside the commands known to make them (`LOG_OUTSIDE_WRAPPER`). |
| `sub_825BDDC0` | `BeginIndexedVertices` | Wrapped | Logged when called outside the commands known to make them (`LOG_OUTSIDE_WRAPPER`). |
| `sub_8209F6B8` | | | Puts controller r4 on player r3 in `826BABA4` (swapping with the player that had it) and asks for the profile read (`826C7938` = 1). From `sub_8209E698`, `sub_822219E0` and `sub_822221A0`. |
| `sub_8210BA68` | | Wrapped | The action mappers' read slots (vtable `0x82007F78`; r3 the mapper, r4 the action id 151-180); logged for `--log_pause_menu` (`PAUSE_MENU_READ_SLOT`). |
| `sub_8210BED8` | | Wrapped | The action mappers' read slots (vtable `0x82007F78`; r3 the mapper, r4 the action id 151-180); logged for `--log_pause_menu` (`PAUSE_MENU_READ_SLOT`). |
| `sub_8210BFA8` | | Wrapped | The action mappers' read slots (vtable `0x82007F78`; r3 the mapper, r4 the action id 151-180); logged for `--log_pause_menu` (`PAUSE_MENU_READ_SLOT`). |
| `sub_8258E5E0` | | Wrapped | Named in `--find_menu_selection`'s log (`ITC_MENU_SCAN_WRAP`): `Scene::Update`'s two screen calls and others. |
| `sub_8258ED78` | | Wrapped | Named in `--find_menu_selection`'s log (`ITC_MENU_SCAN_WRAP`): `Scene::Update`'s two screen calls and others. |
| `sub_82094B10` | | Wrapped | Named in `--find_menu_selection`'s log (`ITC_MENU_SCAN_WRAP`): `Scene::Update`'s two screen calls and others. |
| `sub_820C5DE8` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820C4EB8` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820C4FB8` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820DAA38` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820DAAC8` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820DAB10` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_82597118` | `XPR::GetTexture` | Wrapped | r3 = an XPR object, r4 = an index: the r4-th record's resource header (the header block + the record's +4); an index past the count (+20) gives record 0. Any record type, not only textures. Leaf. 20 call sites. Name from SBX-Recomp. Wrapped with the race draw step's parts and the per-car update's callees, only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_82597518` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_82597610` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_82598768` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820C5C88` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820D4048` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820D72D8` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820D92A0` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820D9B68` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). Calls `CarModel::SetVinylTextures` (`sub_820D5F88`, `car-models.md`). |
| `sub_820D9C50` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820D9EA0` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820DAB58` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820DBFF8` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820DC498` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820DCBC0` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820DDA60` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820DDE58` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820E2E68` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820E2EE8` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820E4140` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820E53F0` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820E5A68` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820E6A40` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820E7328` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820E8908` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
| `sub_820ECE28` | | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). |
