# Working on this repo

A static recompilation of Import Tuner Challenge (Xbox 360) built on
ReXGlue (`rexglue-sdk/`, a submodule patched from `patches/rexglue-sdk/`).
`git status` showing `rexglue-sdk` as modified (`m`) is expected: those are
our patches applied in the submodule, not something to look into
(patch 0003, Windows SDL as a DLL, only changes Windows builds).
`docs/functions/` is the reference for the game's functions: their
table, split by subsystem into one file per area since 2026-10-20
(`functions.md` holds the rows not yet moved, `system-services.md` B5's,
`files-and-saves.md` B4's, `audio.md` B2's; B7's subsystems in their
own files, such as `c-runtime.md` and `ui-movies.md`).
`docs/known-data.md` lists the globals and object fields worked out.
`docs/frame-interpolation.md` is how frame interpolation works (the
frame's structure, the passes, what's blended), every diagnostic option,
and what didn't work; `docs/title-screen.md` the boot and the attract
race; `docs/known-issues.md` the known issues. `docs/gpu-functions.md` covers how the game drives the GPU (the
render command replay and the graphics library), and
`docs/native-renderer.md` is the plan and progress of the native renderer
that replaced the emulated GPU, and (its last section) the investigation
of the render thread's performance. `docs/native-audio.md` is the same for
the SDK's audio (the Audio Worker's spin, the game's sounds, the plan).
`docs/native-movies.md` is the movies played through our own FFmpeg and
drawn by our renderer (`--play_movies_with_ffmpeg`, on by default, since
2026-10-25), replacing the movie library linked into the game
(`docs/functions/movies.md`; `itc_movie_check` plays one on its own).
`docs/unlocked-res.md` covers drawing at other resolutions (`--width`,
`--height`, `--fullscreen`, black bars) and what's left to try.
`docs/windows.md` is the Windows build: cross-compiled on Linux, tested
under Wine (the user has no Windows machine).
`docs/native-memory.md` is B7's first step (built on `native-heap`,
merged into `rexglue-removal-project` 2026-10-22): one allocation layer of our own
(`src/memory/heap.h`) under every allocation, recompiled and native alike
(the process heap, VirtualAlloc and physical memory, routed at nine
functions by `src/hooks/heap.cpp` behind `--native_heap`, on by default;
harness `heap`), whose backing is swapped at the port's end. Guest
allocation is native: native code allocates through it without `ctx`.
`docs/old-hardware.md` is the plan (not started) for running below
Vulkan 1.3, and why not OpenGL.
`docs/rexglue-removal.md` is the plan for a native port without the
ReXGlue SDK: host-side pieces first (track A), then the game's functions
made native from the SDK boundary inward (track B), the SDK's core
removed last. In progress on the long-lived branch
`rexglue-removal-project`: the plan's steps are merged into it, while
`main` keeps the game in its current playable state with bug fixes
(graphics and the like). A0, A5 and B1 are done: game input no longer
goes through the SDK, and the input path from SDL to the processors is
our own C++ (`src/input/`); the action mappers above it are read but
recompiled. B6 (pure helpers) is done for now (merged from
`pure-helpers`, 2026-10-20): the game's time, clock rate, string and
SetLastError functions are plain C++ in `src/helpers/` behind adapters
in `src/hooks/`, and 10 kernel imports have no recompiled callers left;
what's left of B6 waits on its callers' areas ("Left in B6" in the plan:
since B4, only the core's and `xstart`'s still run). B3 (the graphics boundary) is
done (merged from `graphics-boundary`, 2026-10-20): the emulated GPU
isn't built or loaded (`--native_renderer` and
`--native_renderer_exclusive` are gone), the game's device, Swap, display
and gamma functions that called `Vd*` imports are native in
`src/hooks/graphics_device.cpp`, `video_mode.cpp`, `gamma.cpp` and
`graphics_drops.cpp`, nothing reads the GPU registers, and nothing needs
vblanks (the interrupt callback is never called); the SDK's unused
graphics code goes with the SDK. B5 (system services) is done (merged
from `system-services`, 2026-10-21): step 1 (2026-10-21; the PAL-50 and heap
checks, Device::Init's video flags and the console's time zone read
without their kernel calls, `src/hooks/start_up.cpp` and
`src/helpers/console_settings.h`), and step 2 (the user wrappers from
`src/user/user.h`, XUserSetContextEx dropped) step 3 (XMP
dropped, `src/hooks/xmp.cpp`) and step 4 (the network start-up, the
system link socket not made, `src/hooks/network.cpp`); step 5 (voice) is
mapped with no code; step 6 (notifications: the listener wrapper makes
none, the app's reader builds the users once, `src/hooks/notify.cpp`);
step 7 (the profile read as hardcoded data, `src/user/profile.cpp`). B5's
steps are done; what's left of its imports goes with B4, B7 and the core.
B2 (audio) is done (merged from `native-xma`, 2026-10-21): XMA
sounds are decoded by our own upstream FFmpeg (the submodule
`itc_recomp/thirdparty/FFmpeg`, built with only the `xma2` decoder by
`itc_recomp/cmake/ffmpeg.cmake`) into a flat buffer the game's XMA voices
read by position (`src/audio/xma_stream.h`, `src/hooks/xma.cpp`); the
SDK's XMA decoder isn't set up, and the output's conversion is ours (A6,
`src/audio/output_mix.h`); see `docs/native-audio.md`.
B5's and B6's leftovers are done (merged from `b5-b6-leftovers`,
2026-10-21; under "Left to do" in each): the functions never called in
play marked, GlobalMemoryStatus and XShowSigninUI dropped, voice skipped at
its thread (`src/hooks/voice.cpp`: Voice::Poll no longer runs; Voice::Init
still makes the engine, used without null checks), SaveDevices::Update
without its notification read (`src/hooks/notify.cpp`), and Network::Update
stubbed with the data the game reads (`src/hooks/network.cpp`: one
signed-in local user once, the game's profile read when the front end
asks; SignIn::Update no longer called). The game has no System Link mode:
its code is unreachable and not mapped. 63 imports have a live caller;
outside the core only Voice::Init's (B7), `xstart`'s (never reached:
`main` doesn't return in play, closing the window ends the title) and a
`DbgPrint` in the movie player's stream set-up.
B4 (files and saves) is done (merged from `files-and-saves`, 2026-10-21):
the saves (`src/saves/`), the data files (`src/game_data/`, the
file class and the archive streaming), the movies' reads
(`src/hooks/movie_files.cpp`) and the shader build's debug check are
native or dropped; nothing the game opens goes through the SDK's VFS (the
SDK still reads `default.xex` through it at start-up: E3 in the plan).
A2 (logging, flags, hashes) is done (merged from `own-logging`,
2026-10-21): `src/` logs, defines flags and hashes through our own
`src/base/` (`ITC_LOG_*`, `ITC_FLAG_*` and `ITC_FLAG(name)`,
`base::Hash64`; never the SDK's `REXLOG_*`, `REXCVAR_*` or `rex/hash.h` in
new code). The logger is ours (`src/base/log.cpp`, `std::format` and
stdio, no rotation) and takes over the log file the SDK opens, forwarding
the SDK's own lines into it (`src/base/log_sdk.cpp`, until A4); hashes
use our own xxHash (`itc_recomp/thirdparty/xxHash`, A1's first library).
The flags are still parsed by the SDK until A4, which is deferred until
the code has no guest dependency (the user's call, 2026-10-21; the survey
is under A4 in the plan). B7 (the game logic) is in progress: its
first loop run (`docs/loops/b7-loop.md`, 2026-10-22, on `b7-loop-test`,
merged into `rexglue-removal-project` the same day, its rows not yet
reviewed by the user) made five subsystems native as far as they were ready
(`docs/functions/sprites.md`, `command-list.md`, `c-runtime.md`,
`ui-movies.md`, `ui-movies-game.md`; each behind `--native_*` flags, on by
default); see "B7's loop runs" in the plan and
`docs/loops/b7-progress.md`. Much of what's left in them waited on
allocating guest memory from native code, which is native since
native memory (2026-10-22, `docs/native-memory.md`): native code calls
`memory::Alloc` and the like. Its second run
(on `b7-loop-22-10-26`, merged into `rexglue-removal-project`
2026-10-22, after a drive saw the functions attract races hadn't; summary
`docs/loops/b7-loop-2026-10-22-1047.md`) removed 31 adapters and made the
race overlays native as far as they were ready
(`docs/functions/race-overlays.md`, `--native_race_overlays`). Its third
run (the same branch, 2026-10-22, merged the same day after a drive saw
62 of the 66 functions attract races hadn't, its rows not yet reviewed
by the user; summary
`docs/loops/b7-loop-2026-10-22-1439.md`) made the vehicle accessors, the
matrix math and the car physics native as far as they were ready
(`vehicle-accessors.md`, `matrix-math.md`, `car-physics.md`;
`--native_vehicle_accessors`, `--native_vector_library`,
`--native_matrix_math`, `--native_car_physics`; ours 422 -> 705); native
functions log `native running (game|harness)` once
(`ITC_NATIVE_RUNNING`), and the status Merged is work done inside a
native caller. Its fourth run (`b7-loop-22-10-26-2`, merged into
`rexglue-removal-project` 2026-10-23 after a drive; summary
`docs/loops/b7-loop-2026-10-22-2211.md`) made the C runtime heap, the
collision narrow phase and the collision world native as far as they
were ready (`heap.md`, `collision.md`, `collision-world.md`;
`--native_c_runtime_heap`, `--native_collision`,
`--native_collision_world`; ours 714 -> 837). Its fifth run
(`b7-loop-23-10-26`, merged into
`rexglue-removal-project` 2026-10-23 after a drive; summary
`docs/loops/b7-loop-2026-10-23-0152.md`) made the sound cues and XACT
native as far as they were ready (`sound-cues.md`, `xact.md`;
`--native_sound_cues`, `--native_xact`; ours 837 -> 970): the user's
call is that XACT, the sound library linked into the game, is
translated (no host library plays its data); the rest waits on XACT's
critical section, XAudio and its voices, the tracks and waves, and
GetTickCount (`sub_822956F0`, the user's call). What's for the user's
review is in `docs/loops/review.md`, harness groups expected to differ
in `docs/loops/expected-differences.md`, the functions still to see in a
drive in `docs/loops/to-drive.md`; new harness code compares
floats with `harness::Outputs` (two NaNs match). Its sixth run
(`b7-loop-23-10-26-2`, merged into `rexglue-removal-project`
2026-10-23; summary `docs/loops/b7-loop-2026-10-23-1106.md`) made the
action mappers and the game's data tables native as far as they were
ready (`action-mappers.md`, `data-tables.md`;
`--native_action_mappers`, `--native_data_tables`; ours 971 -> 1,023),
with no change in the game's behavior (the user's call: B7 only moves
the game off the recompiled code); the rest waits on the input
manager's vtable targets, the input processor's constructor and the
archive.
Its seventh run (`b7-loop-23-10-26-3`, merged into
`rexglue-removal-project` 2026-10-23 after a drive; summary
`docs/loops/b7-loop-2026-10-23-1344.md`) made the model animation, the
car specs and the cameras native as far as they were ready
(`model-animation.md`, `car-specs.md`, `cameras.md`;
`--native_model_animation`, `--native_car_specs`, `--native_cameras`;
ours 1,023 -> 1,139), the cameras keeping frame interpolation's hooks
(`BlendState_OnCopy` added); the rest waits on the graphics device's
resource calls, the archive and memset/atexit.
Its eighth run (`b7-loop-23-10-26-4`, 2026-10-23, not yet merged;
summary `docs/loops/b7-loop-2026-10-23-1722.md`) made the string table
and font, `std::string` and the course's route native as far as they
were ready (`strings-and-font.md`, `std-string.md`, `track-route.md`;
`--native_strings_and_font`, `--native_std_string`,
`--native_track_route`; ours 1,139 -> 1,223), with
`TrackPosition::Init` and `Update`; `std::string`'s member functions
are translated over the guest layout (the user's call); the rest waits
on `DataFile::GetEntry`, the archive, the XPR, the shader compiler, the
device's state save and `atexit`.
Its ninth run (`b7-loop-23-10-26-5`, 2026-10-23 to 24, merged into
`rexglue-removal-project` 2026-10-24 before a drive;
summary `docs/loops/b7-loop-2026-10-23-2022.md`) made the AI drivers,
the racer controllers and the track zones native as far as they were
ready (`ai-drivers.md`, `racer-controllers.md`, `track-zones.md`;
`--native_ai_drivers`, `--native_racer_controllers`,
`--native_track_zones`; ours 1,223 -> 1,485); the rest waits on the
array destructor iterator (`sub_820900C0`, native since the eleventh
run), the getter of
`[0x826C6078]` (`sub_821434E8`), the race flow's functors and the
camera modes. Its tenth run (`b7-loop-24-10-26`, 2026-10-24) mapped
the graphics resources (`docs/functions/graphics-resources.md`, 44
rows) and stopped for two of the user's calls, then went on (the
user's calls, 2026-10-24: `Edram::Alloc`, `sub_822B5EF8`, native with a
`std::mutex` for the kernel's critical section; the resource locks'
fence waits and cache flushes Dropped, their lock count, address and
freeing kept; 46 of 46 ours, `--native_graphics_resources`, ours 1,485
-> 1,529), then mapped the video output and the device lock
(`video-output.md`) and stopped again for the user's call on the
device lock's kernel sync, then went on (the user's call, 2026-10-24:
the device lock alone over a `std::binary_semaphore`; the game's
Semaphore class stays recompiled for the archive loader; 7 of 30 ours,
`--native_video_output`, with three camera passes), and made the device's
state setters native (`docs/functions/device-state.md`, 131 of 131,
`--native_device_state`; the packets written straight into the command
buffer, which nothing reads, Dropped by the graphics rule; ours 1,672).
It stopped at its limit and was merged into `rexglue-removal-project`
2026-10-24 after a drive (its rows not yet reviewed by the user);
summary `docs/loops/b7-loop-2026-10-24-0325.md`.
Its eleventh run (`b7-loop-24-10-26-2`, 2026-10-24, merged into
`rexglue-removal-project` 2026-10-24 before a drive;
summary `docs/loops/b7-loop-2026-10-24-1601.md`) made the compiler's
array helpers (`c-runtime.md`), the course scenery
(`docs/functions/course-scenery.md`, `--native_course_scenery`: the
areas' models and lights streamed into pools, 38 of 73) and XAPI's
memset (`XMemSet`, with `Scenery::Init` and `CameraPasses::Init` it
unblocked) native as far as they were ready (ours 1,672 -> 1,714); the
rest waits on `atexit` and the critical section wrappers (the core),
`MmQueryAllocationSize`'s wrapper, `DataFile::GetEntry`, the XPR, the
archive's queue and the texture provider.
Its twelfth run (`b7-loop-24-10-26-3`, 2026-10-24, merged into
`rexglue-removal-project` 2026-10-24;
summary `docs/loops/b7-loop-2026-10-24-1746.md`) made the post effects
(`post-effects.md`, `--native_post_effects`, 44 of 57), the UI movie
walk and the AI drivers' teardown (cleanup leads), and the input
manager and processors (`input.md`, `--native_input`, 29 of 29, with
`ActionMapper::ActionMapper` it unblocked) native (ours 1,714 ->
1,794); it stopped on two input functions whose harness didn't reach
their prints (given plain C++ of their own since; each harness now has
its own random generator); `ActionMappers::Init` tops the leads; the rest waits on the models'
resource relocation and shader pairs, the vertex declaration and the
critical section wrappers.
Its thirteenth run (`b7-loop-24-10-26-4`, 2026-10-24 to 25, ended by the
user before the framework's migration, merged into `rexglue-removal-project`
2026-10-25; summary
`docs/loops/b7-loop-2026-10-24-2133.md`) made `ActionMappers::Init`, three
cleanup leads and the race's screen objects native (`screen-objects.md`,
`--native_screen_objects`, 38 of 42) and mapped the app framework
(`framework.md`, 68 rows, its migration the first lead); beside it, the
user's work replaced two libraries linked into the game: the HLSL
compiler (its 14 shaders stored, `--use_hardcoded_shaders`) and the movie
library (`--play_movies_with_ffmpeg`, `docs/native-movies.md`), whose
~640 and ~580 functions no longer run; replaced libraries aren't mapped
("Rules that still hold" in `docs/loops/b7-loop.md`); ours 1,794 ->
1,847, work left 4,967 -> 4,604 (the coverage list made again twice).
Its fourteenth run (`by-loop-25-10-26`, 2026-10-25, merged into
`rexglue-removal-project` the same day;
summary `docs/loops/b7-loop-2026-10-25-0117.md`) made the app framework
native as far as it was ready (`framework.md`, 56 of 68 ours, with
`main` and `App::Run`; `--native_framework`; ours 1,847 -> 1,903),
ended after it by the user's call; an `ai_drivers` harness case that
read a wild address (exposed when the hardcoded shaders moved the
harness's block) was fixed on the way (dbc8ee0); the rest waits on
`Framework::Init`'s services, `sub_824936E8` and `sub_8258E1D8`'s
critical section.
Its fifteenth run (`b7-loop-25-10-26-2`, 2026-10-25, merged into
`rexglue-removal-project` the same day after a drive;
summary `docs/loops/b7-loop-2026-10-25-0359.md`; 5 subsystems, the
user's limit for it) made the game modes, the model class, the mode
tick, the minimap's parts and `atexit` with what it unblocked native as
far as they were ready (`game-modes.md`, `models.md`, `mode-tick.md`,
`minimap.md`, `c-runtime.md`; `--native_game_modes`, `--native_models`,
`--native_mode_tick`, `--native_minimap`; `atexit` and the UI movies'
animation under `--native_c_runtime` and `--native_ui_movies`; ours
1,903 -> 2,013). Two kernel critical sections became host mutexes by
the user's call (the memory blocks' lock `0x8296BD38`; the C runtime's
exit lock), taken by every user, as `Edram::Alloc`'s; the loop now
follows the user's earlier calls without asking again ("Precedent
first" in `docs/loops/b7-loop.md`); `scripted_run.sh`'s boot marker
was fixed (it waited for the movie library's draw, gone since the
FFmpeg movies). The rest waits on the race mode's set-ups, the course
set's lists, the save data's creation and the kernel objects' set-ups.
Its sixteenth run (`b7-loop-25-10-26-3`, 2026-10-25, merged into
`rexglue-removal-project` the same day after a drive;
summary `docs/loops/b7-loop-2026-10-25-1638.md`; 3 subsystems) made
the scene models' set-up side, the scenery lists with the model draws
and the shader objects up to `Model::Load` native as far as they were
ready, with two leads without mapping (the UI movies' text draw, update
and objects; the race HUD's movie picker and the rival logo's draw)
and what `DataFile::GetEntry` unblocked (`scene-models.md`,
`race-hud.md`, `scenery-lists.md`, `shader-objects.md`;
`--native_scene_models`, `--native_race_hud`, `--native_scenery_lists`,
`--native_shader_objects`, `--native_data_files`; ours 2,013 -> 2,152).
The user's calls: `_mtinit` (kernel TLS) waits until it blocks
something; native code reaching `generated/` (fallbacks, guest calls)
is left to investigate later; one replay with the menus unpaced under
`--skip_intro` wasn't reproduced and is left to debug later
(`docs/loops/review.md`). The rest waits on the generic critical
section wrappers, the video output's events and semaphores and the
archive.
Its seventeenth run (`b7-loop-25-10-26-4`, 2026-10-25 to 27, merged
into `rexglue-removal-project` 2026-10-27 before a drive; summary `docs/loops/b7-loop-2026-10-25-2342.md`; 2 subsystems,
the user's limit for it) made the race's set-up (`race-setup.md`,
`--native_race_setup`, 52 of 52), the rest of the car physics (a lead
without mapping, `car-physics.md` 134 of 134) and the vehicle code
above it (`vehicle.md`, `--native_vehicle`, 25 of 25) native (ours
2,152 -> 2,255); one harness group is for the user's review
(`Race::SetUp`'s traffic records' unwritten bytes,
`docs/loops/expected-differences.md`).
Its eighteenth run (`b7-loop-27-10-26`, 2026-10-27 to 28, merged
into `rexglue-removal-project` 2026-10-28 before a drive; summary `docs/loops/b7-loop-2026-10-27-2231.md`; 3 subsystems)
made the app's update and its state stack (`app-states.md`,
`--native_app_states`, 56 of 56), the scene's loading objects with
their small blockers in `car-models.md` and `models.md`
(`scene-loaders.md`, `--native_scene_loaders`, 60 of 78;
`--native_car_models`; a lead without mapping) and the players' save
data in memory (`save-data.md`, `--native_save_data`, 19 of 19 in play)
native (ours 2,255 -> 2,400); the rest of the scene loaders waits on
the material passes, the accessories, `Skeleton`'s local matrices,
`XPR::GetTexture` and sprintf.
Its nineteenth run (`b7-loop-28-10-26`, 2026-10-28, merged into
`rexglue-removal-project` the same day before a drive;
summary `docs/loops/b7-loop-2026-10-28-0836.md`; 3 subsystems; stopped
once by a harness gap, fixed and resumed) made texture compression
(`texture-compression.md`, XGraphics' `XGCompressSurface` over D3DX's
blitter, `--native_texture_compression`, 24 of 24 in play), the vinyl
sheet above it (`Vinyl::Compress` and the decal path with its software
rasterizer, two leads without mapping in `car-models.md`, under
`--native_car_models`), the race world (`race-world.md`,
`--native_race_world`, 49 of 50: the race mode's state machine, the
racers' manager and its vehicle streaming, the start placements) and
the race's states (`race-states.md`, `--native_race_states`, 43 of 63:
the second state machine and the race's progress) native (ours 2,400
-> 2,540); the rest waits on the archive, the loading screen, the cars
put back, the input recordings and the race event's lookups. The
harnesses now take ~30 s of `attract_race.sh 40`, leaving ~8 s of race.
Its twentieth run (`b7-loop-28-10-26-2`, 2026-10-28, not yet merged;
summary `docs/loops/b7-loop-2026-10-28-1845.md`; 3 subsystems) made
the archive (`archive.md`, `--native_archive`: the TOC, `Archive::Load`,
the queue and the loader; its four semaphores host counting semaphores,
the user's call, routed by `Semaphore::Wait` and `Signal`), the file
layer with sprintf and the loose read it unblocked (`file-layer.md`,
`--native_file_layer`; sprintf under `--native_c_runtime`) and the save
system's interface (`save-interface.md`, `--native_save_interface`, 41
of 41 in play) native (ours 2,540 -> 2,628); the rest waits on the
loose read's and opens' guest calls to B4's `File::` adapters, the
game's `Semaphore::Init` and the leads in `docs/loops/leads.md`.
Harness runs use `attract_race.sh 40` (the user's call,
2026-10-23); a race short only because the harnesses took longer is
no halt. B7 loop migration agents run only their subsystem's harnesses
(and those of files they touched); the validation agent runs them all
once after each migration (the user's call, 2026-10-28).
After each plan item, run `itc_recomp/tools/boundary_status.py` and record
its output in "The boundary over time" at the end of
`docs/rexglue-removal.md`. Plan steps are done on their
own branches and merged into `rexglue-removal-project` (no force pushes:
GitHub refuses them on that branch).
`docs/coverage.md` is which recompiled functions actually run (the
`itc_coverage` build, `itc_recomp/coverage/seen_in_play.txt`) and how that
measures B7; functions not seen in play still get rows, marked "Not seen in play"
(the user's call, 2026-10-23: a session may not have taken their path).
`docs/pad-replay.md` is recording the user's pad and replaying it into the
game through a virtual pad (`pad_record.py`, `scripted_run.sh`), for test
runs past the title screen.
`docs/tick-replay.md` is a lead (not started): translating draws once a
tick and replaying our own list on in-between frames.
`docs/pause-menu-fix.md` is the pause menu's too-fast input repeat: fixed
(the action mappers updated once a tick, `--input_mappers_by_tick`), and a
native Input::Update for later. Its last section, "The sub-menus' arrow",
is fixed (merged from `pause-menu-arrow`): UI movie tracks
that add to values the update doesn't put back were re-added on every
render-only pass; now skipped there (`--per_tick_ui_movie_tracks`, a
workaround for frame rates above 30).
`docs/cockpit-dial-interpolation.md` is the cockpit dials: native dial
updates, the needles' angles from the tick and their position on the
blended car (`--native_cockpit_dials`), and the TODO for smooth needles.
Read the relevant section before changing anything, and keep it up to
date as you learn things.

## Rules from the user

- Only search inside this repo. Ask for the location of anything outside it.
- SBX-Recomp (the Japanese release's recomp, in a folder named `SBX-Recomp`
  beside this repo's folder) is a reference, but read it only when the user
  says so. How to reference it: `docs/referencing-sbx-recomp.md`.
- Native code replaces the game's use of the Xbox, never the Xbox or the
  SDK behind it (don't port ReXGlue/Xenia into `src/`):
  - Plug in data, don't mimic: when replacing an Xbox system service, find
    what the game's callers use from the answer and hardcode that (fixed
    values, a one-time action), with no 360-style queues, profiles or
    notification machinery. Check the function before sizing the work.
  - Map Xbox features, don't port: voice (XHV), system link, Xbox Live,
    XMP and presence get rows in `docs/functions/`, but no C++ stand-ins
    imitating their interfaces; skip them at the highest safe game-level
    point (Status Dropped, with the reason).
  - Not emulating: play the game's data through host APIs and upstream
    libraries at the level the data needs (FFmpeg's `xma2` for XMA), not
    Xenia's forks or code imitating the hardware (contexts, registers).
  - No System Link mode: the game has none; its code is unreachable, gets
    no rows and no preserved behavior (mention it at most once where it's
    left out).
- The repo is public: never commit absolute local paths (anything under
  `/home/`) in files or commit messages; use paths relative to the repo root.
  `.githooks/` refuses them; enable it once per clone with
  `git config core.hooksPath .githooks`.
- Commit often. Never revert or `git checkout` uncommitted work without
  committing it first.
- Don't create branches (local or pushed) without asking the user first;
  work on the branch you're on unless they say otherwise.
- Push work branches to `origin` (github.com/Spooky098529/ITC-Recomp) after
  each commit. Ask before pushing to `main` or pushing tags.
- No `-march=native`.
- Use American English only (spelling and wording) in replies, code,
  comments, docs and commit messages.
- Find problems with logs, not guesses: add logging, have the user run the
  game, read the log, then fix what it shows. Guessing at causes cost many
  runs on the marker above opponents (see "World-space sprites").
- Keep track of what you learn about the game's functions, without being
  asked: every function you read in `generated/`, override, or learn
  something about (what it does, its arguments, what it calls, who calls
  it, how often), including those only met along the way (callees,
  callers, helpers). When a piece of work is done, or before ending a
  session, show the user the rows you'd add or change in
  `docs/functions/` (the subsystem's file; `functions.md` when none
  fits yet), in the table format (`| Address | Name | Status | Notes |`,
  four cells, one function a row, each row exactly as it would be written;
  for a changed row, say what changed), and ask whether to add them;
  write them only after the user has reviewed and agreed. In
  the rows, say how much is known: "Not read" or "Not read in detail"
  for what's inferred from calls, "probably" for guesses. Set the Status
  column by the definitions at the top of the file and run
  `itc_recomp/tools/check_function_statuses.py` after changing it.

## Build and run

From the repo root:

```sh
cmake --build itc_recomp/out/build/linux-amd64-release -j$(nproc)
G="$PWD/itc/Import Tuner Challenge (USA)"; B=itc_recomp/out/build/linux-amd64-release
XDG_DATA_DIRS=/usr/local/share:/usr/share LD_LIBRARY_PATH=$B:rexglue-sdk/out/linux-amd64 \
  $B/itc --native_audio=true \
  --native_renderer_mirror_by_tick=true --native_renderer_mirror_to_both=true \
  --game_data_root="$G" --max_fps=144 --fullscreen=false \
  --log_fps=true --interpolate_draw_state=true <diagnostic flags>
find /dev/shm -maxdepth 1 -user "$USER" -name 'xenia_memory_*' -delete
```

- Test runs use the command above (the docs' "standard flags"), which
  matches what players get
  (`itc.toml`'s settings and the defaults; the flags it spells out are on
  by default now): always `--native_audio=true` (without it the SDK's
  Audio Worker spins on a core and frame rates come out lower than players
  get) and the mirrors updated once a frame
  (`--native_renderer_mirror_by_tick=true
  --native_renderer_mirror_to_both=true`; `=false` on the first for the
  old per-draw mirror). `profile_render_thread.sh` does the same.
- For logs to read (frame rate, where each frame's time goes, each busy
  thread's CPU use), add `--log_level=info --log_fps=true
  --log_frame_times=true --log_thread_cpu=true`.

- If the build fails with "file ... has been modified since the precompiled
  header ... was built" (clang was updated), delete the stale headers:
  `find itc_recomp/out/build/linux-amd64-release -name 'cmake_pch*.pch' -delete`.
- Compare frame rates uncapped (`--max_fps=0`), alternating runs: this PC
  holds the 144 cap, which hides differences. An attract race under
  `timeout 20` is enough: the race starts ~2 s after launch (the first
  `fps` line with `ticks/s 30`); take the mean over its seconds 3-18.
  Runs vary ~±1.5%, so for changes smaller than a few percent compare
  profiles instead (the user's call): `itc_recomp/tools/profile_render_thread.sh
  <dir holding itc> <name> [symbol regex]` gives the render thread's and
  each matching function's cycles a frame; two or three runs per build,
  alternating. A run pinned to one CPU
  (`taskset -c 0`) stands in for the Steam Deck but varies by ~15% from run
  to run.
- The release preset builds with ThinLTO (`ITC_LTO`, game and SDK), which
  needs lld (`ld.lld`). Measured 2026-10-14, two alternating uncapped attract
  races each: 236 and 237 fps mean without, 246 and 239 with; the user kept
  it. `-DITC_LTO=OFF` turns it off.
- That's what players run: our own renderer, no GPU plugin.

- Launch from the repo root; other directories fail (exit 127).
- `--skip_intro --attract_after_s=1` starts an attract race (AI cars) about
  1 s after launch with no input, so races can be checked without the user:
  run under `timeout`, read the log, and look at frames with the
  renderer's dump (`--native_renderer_dump=<dir>`, 10 s into a race or
  when `<dir>/now` appears). The attract mode puts a random sepia or
  grayscale filter over the screen: a gray or brown frame from an attract
  race is the game's, not a rendering bug. Never screenshot the user's desktop. Without the GPU plugin, `--skip_intro` sometimes
  stalls the boot (no "Resolve (822AFA58) called from 8258D9E8" in the
  log; "called from 8248466C" before the movies moved to our FFmpeg):
  restart it.
- `itc_recomp/tools/attract_race.sh <seconds> [flags]` runs such a race
  with the standard flags and prints the log's path, the seconds raced,
  error lines and harness results. `--run_harnesses=all` (or a list)
  compares native functions with the recompiled ones on the same inputs
  (`src/harnesses/`); every function made native gets harness cases there
  in the same change (see "Checking with a harness" in
  `docs/rexglue-removal.md`).
- Run the game in the background. The user plays: they start a race, drive
  near an opponent and quit, then say what they saw.
- Logs go to `itc_recomp/out/build/linux-amd64-release/logs/itc_<n>.log`,
  newest first by `ls -t` (one file a run, no rotation since A2's logger;
  older logs may have an `itc_<n>.1.log`). The race starts where `fps` jumps above 30.
- `generated/` is the recompiled code. Find a function with
  `grep -n "DEFINE_REX_FUNC(sub_XXXXXXXX)" itc_recomp/generated/default/*.cpp`,
  or a call site with `grep -ln "lr = 0xXXXXXXXX;"`. Override a function by
  defining `extern "C" REX_FUNC(sub_XXXXXXXX)` in `src/` and calling
  `__imp__sub_XXXXXXXX`. Only one override per function can exist (the link
  fails with "multiple definition" otherwise), so `grep -rn sub_XXXXXXXX
  itc_recomp/src` first.

## Test builds for the user

Package them into `itc_recomp/out/dist` with Python's zipfile:

```sh
name=itc-<label>-linux-amd64; D=itc_recomp/out/dist/$name
rm -rf $D $D.zip; mkdir -p $D
cp itc_recomp/out/build/linux-amd64-release/itc rexglue-sdk/out/linux-amd64/librexruntime.so $D/
cp itc_recomp/itc.toml $D/
(cd $D && python3 -I -m zipfile -c ../$name.zip itc librexruntime.so itc.toml)
```

Release and test zips ship `itc.toml` (the player settings, with comments;
command-line options win over it). Since 2026-10-20 the once-a-frame mirrors
are on by default (and the native renderer always draws: its switches,
`--native_renderer` and `--native_renderer_exclusive`, went in B3), and without
`--game_data_root` the game's files are looked for next to the executable
(`ItcApp::OnConfigurePaths`; the SDK reads that flag before `itc.toml`
loads), so a bare `./itc` plays as released.

## Status (`main`, 2026-10-20, v0.4.7.1)

Frame interpolation for races is working: logic runs at 30 Hz and frames are
blended between ticks. The world, cameras, wheels (other cars' too, paired
by position), lights, shadows, headlights (including their light on other
cars' bodies), other cars' lighting passing street lamps, tunnels, the race
start countdown, the HUD, the marker above opponents, the light trails, the
texture animations (flashing lights, signs, the off-ramp chevron), the
minimap triangle's flashing, the car's reflections, buildings' lighting,
play time and the glow (bloom; `src/interpolation/glow.cpp`, v0.3.9) are all done. Interpolation is
on by default (marked experimental in `itc.toml`, which also turns logging
off); `interpolate_draw_state = false` and `max_fps = 30` there give the
original 30 fps.
Our renderer always draws the glow (`--glow` only affects the emulated
GPU); `--motion_blur=false` leaves out the motion blur (on by default;
defined in `src/native/renderer.cpp` since SDK patch 0002 was removed). Known issues are in
`docs/known-issues.md`.

Other resolutions (v0.4.0, merged from `unlocked-resolution`):
`--width`/`--height` and `--fullscreen`, off by default (a 1280x720 window
at the game's own resolution out of the box); at other sizes every pass is
drawn scaled and the 3D view at the full resolution, with black bars on
shapes other than 16:9. See `docs/unlocked-res.md` (a more native frame
without the game's upscale draw is for later).

Pause menu (merged from `pause-menu`, 2026-10-18, released in v0.4.5): the
action mappers' update (`sub_8210B018`) runs on logic passes only
(`--input_mappers_by_tick`, on by default, `src/hooks/input.cpp`), so the
pause menu's input repeat counts ticks at any frame rate; the old
workaround (`--pause_workaround`, the pause screen held at 30 fps) is
removed. See `docs/pause-menu-fix.md`. The pause sub-menus' arrow (merged
from `pause-menu-arrow`, 2026-10-18, released in v0.4.5): at 144 fps it slid
too far, because UI movie tracks that add to values the update doesn't put
back were applied again on render-only passes; now skipped there
(`--per_tick_ui_movie_tracks`, on by default). Its trail, and its snap to a
front-end menu's first item, are the game's own (seen on a 360 video).

The rival logo (merged from `rival-logo-flicker`, 2026-10-18, released in v0.4.5):
the rival intro cutscene's logo flickered above 30 fps, drawn during its
Update handler (`82159948`, `sub_821594B8`) on logic passes only. A native
override runs the cutscene on logic passes and only the logo's draw
(`sub_82211A28`) on render-only passes (`--redraw_rival_logo`, on by
default). See "The rival logo at the race start" in `docs/frame-interpolation.md`.

Cockpit dials (merged from `cockpit-dial-interpolation`, 2026-10-19,
released in v0.4.5): the dial updates (`sub_820E42E8`, `sub_820E45A8`) are native
(`src/hooks/cockpit_dials.cpp`, `--native_cockpit_dials`, on by default).
The needles' angles come from the tick's gauge values (30 Hz steps, as on
the 360) and their position from the car blended to the frame; the game
draws the tick pose (it culls with the tick's camera) and the blend replays
the frame pose as a live pose (`BlendState_SetLivePose`: world kept, camera
blended). Before, blending the needles' matrices cut across the dials, and
the turbo gauge fell to its rest angle between ticks. TODO: blend the
angles, or run the game logic at a higher tick rate. See
`docs/cockpit-dial-interpolation.md`; the functions are in
`docs/functions/functions.md`.

Headlight beam far from the origin (merged from `headlight-flicker`,
2026-10-19): far out (x ≈ -8,000 m), the beam lagged the car or flickered,
as the headlight projector's depth offset, read back from its matrix,
drifts with distance and failed the cut check's 1% (and renumbered it).
Camera numbers and the cut check no longer use it
(`--camera_ids_ignore_depth_offset`), and numbers stay with their camera
address while a projection drifts (`--sticky_camera_ids`); both on.
`--double_precision_blending` is kept off (nothing visible). The cockpit
judder far out is the game's own (Known issues). See "The headlight beam
far from the origin" in `docs/frame-interpolation.md`.

Motion blur (merged from `beam-stepping`, 2026-10-19): the headlight beam
on the road sometimes looked like it updated at 30 Hz. It was the motion
blur over it: its previous view-projection (vertex c100 x4, copied by
`sub_821363F0`) is the same matrix all tick, but copied from the view's
+8080 on logic passes and +8144 on others, and copies pair by address, so
only logic frames blended it and the blur's length jumped once a tick.
Now noted under the logic pass's address and blended on every frame
(`--motion_blur_by_tick`, on); the user judged it fixed at 4K. The motion
blur is also optional now: `--motion_blur=false` (on by default, in
`itc.toml`), defined in `src/native/renderer.cpp`; SDK patch 0002
(the emulated GPU's toggle) is removed. The cameras refused far from the
origin were ruled out: a temporary in `sub_82598768`, never drawn
(`--log_blend_state` now names each refused camera's source). See
"Motion blur reads the 30 Hz tick" in `docs/native-renderer.md`.

Graphics memory (merged from `memory-leak`, 2026-10-19, released in
v0.4.6): textures were never freed,
so graphics memory grew by ~60-130 MB a race load and ~1 MB/s while
driving (seen on the Steam Deck, 1 GB of VRAM). Now textures loaded from
guest memory are freed when a race ends (`TextureCache_RaceLeft`, from
`GameMessages_OnPassFinished`) and when the game loads a new one over
their memory after writing there (its texture streaming), and a resolve
destination when a new one is made over its memory at another address
(the race's glow chain, made again per race); all destroyed after the
frames in flight. No flags (measured free: 486.0 against 486.4 fps).
VRAM peaks at ~380-510 MB instead of growing without end; a 6.5-minute
race levels off at ~155 MB of textures. `--log_gpu_memory` logs what the
renderer holds. See "Textures are never freed" in `docs/native-renderer.md`.

Xbox 360 gamma (merged from `xbox-gamma`, 2026-10-16; released in v0.4.3):
`--xbox_gamma=off|hdtv|power`, off by default (`itc.toml` sets `off` and
lists the choices). The display gamma tables the game writes at boot,
copied into `src/native/xbox_gamma_table.h` (not read from the SDK), are
applied to the frame by a compute pass; `hdtv` looked pretty close to the
360 to the user. See "Gamma" in `docs/native-renderer.md`'s known issues.

Windows (merged from `windows`, 2026-10-16; released in v0.4.1, experimental):
cross-compiled on Linux with xwin, ThinLTO (`ITC_WIN_LTO`), `itc.toml`
for settings, the Visual C++ runtime DLLs in the zip, and a
CI job beside Linux's. Tested only under Wine: ~26% slower than Linux
there before ThinLTO, which then gave +3.1%. See `docs/windows.md`.

The game draws with our own Vulkan renderer (the native renderer, exclusive
mode; merged from `native-renderer`): the emulated GPU plugin isn't loaded
or shipped. Uncapped attract race: ~330 fps mean against the emulated GPU's
155 (~320 before `replay-allocations`: interpolation's constant replay
no longer allocates per upload, finds products in a flat table and
blends lighting vectorized, see "Profile of 2026-10-15, after v0.3.7" in
`docs/native-renderer.md`; 258 before `redundant-state`: draws no longer record state calls that
repeat the previous draw's, and the constant replay's diagnostics run only
with their flags; see "Performance: repeated state calls" in
`docs/native-renderer.md`; ~285 before `descriptor-reuse`: constant blocks
the same as the last written are reused, and constants are copied from
guest memory run by run with a vectorized byte swap, see "Performance:
reading constants from guest memory"). Since then (`blend-math`, `read-registers`): blends planned once a tick,
cameras blended once a frame, guest words byte-swapped with `pshufb`, and
the register shadow read only where the library's dirty masks say
("Performance: the render thread" in `docs/native-renderer.md`: "Done on
`blend-math`", "Register reads", "Against v0.3.7"); then (`mirrors-fix`)
the guest memory mirrors given memcpy's writes once a frame instead of
checked draw by draw (on by default; "Hypothesis to test: the
mirrors updated once a tick"). All released in v0.3.8: ~430 fps uncapped
in an attract race with the standard flags (2026-10-15). Translated shaders and pipelines are kept on disk (`cache/` next to
the executable), and textures are reloaded when the game's memcpy writes
them instead of being hashed (`docs/texture-handling-optimization.md`);
merged from `perf-debug` for Steam Deck hitches. Its known issues (boot movie, gamma, textures, window resizing,
remaining packet writers, speed) are milestone 7 in
`docs/native-renderer.md`.

Sound goes through our own audio output (`--native_audio`, on by default,
since v0.3.2): our thread runs the game's mixer, replacing the
SDK's Audio Worker, whose polling took ~80% of a core. The SDK's XMA
decoder is kept. See `docs/native-audio.md`.

Releases are made by pushing a `v*` tag, which must be `vX.Y`, `vX.Y.Z`
or `vX.Y.Z.W` (the SDK's `cmake/rex_version.cmake` parses it: anything
else, like `v0.4.7b`, fails to configure, and so does every later
untagged build while it's the nearest tag; a release without
improvements gets a fourth number, like `v0.4.7.1`): CI builds the zip and publishes a
GitHub release with generated notes, which are then replaced with notes
written for players: title "vX.Y.Z: <summary>", and just a couple of
bullet points naming the changes (not the long layout of v0.3.5 and
earlier). `build-linux.yml` configures with `clang-20`/`clang++-20` by
name: since 2026-10-16 its build dependencies pull in Ubuntu 22.04's clang
14 package, which replaces `/usr/bin/clang` (the SDK needs 18 or newer).
A tag runs both `build-linux.yml` and `build-windows.yml`, and
both zips go on the release (see "CI" in `docs/windows.md`). Linux
release zips (`v*` tags only) are fully stripped (`strip --strip-all`: no
debug info or symbol table, nothing for gdb to go on, as the user wants;
the dynamic symbols stay, as loading needs them); CI artifacts from other
runs, local builds and test zips keep both for debugging.
Stripping doesn't change speed (2026-10-16: 431.5 against 432.0 fps mean,
two alternating uncapped attract races each).

Not done yet:

- Menus still run at 30 fps (the front end's; the pause menu runs at the
  race's rate; see Known issues).
- Other per-object draws (caller `0x82596D9C`) use projective matrices that
  aren't blended, so they step at 30 Hz; what they draw isn't known yet
  (see Known issues; `--log_moving_products` lists them).
- General renderer load: the render thread is the limit (flat out on the
  Steam Deck, at ~7 µs a draw), so any cut in the cost of a draw helps.
  Since v0.3.7, `blend-math` and `read-registers` cut
  ~12% of the render thread's cycles a draw (by profile, attract race;
  not yet checked on the Deck). Where the render thread's time goes, what's
  done and what's next: "Performance: the render thread" in
  `docs/native-renderer.md`. Since `mirrors-fix` (merged): the
  mirrors are updated at frame starts instead of checked draw by draw
  (`--native_renderer_mirror_by_tick` and `--native_renderer_mirror_to_both`,
  on by default): -14.7% render thread cycles a frame,
  checked in attract races and a race the user drove ("Hypothesis to test:
  the mirrors updated once a tick" and what follows it). Then (2026-10-16)
  `Buffers_OnCommand`, `ReplayStats_OnCommand` and `GetRenderer` stopped
  setting up big stack frames on every call (ThinLTO had inlined their
  logging or start-up into them; ~-2% and ~150k cycles a frame, see
  "`Buffers_OnCommand`'s cost"). Merged from `texture-binding`
  (2026-10-16): texture views and samplers answered from a table by the
  fetch constant's words, and each stage's last texture set reused when
  unchanged: -6.9% render thread cycles a frame and +2.8% uncapped fps
  against the `main` before it ("Texture binding" in
  `docs/native-renderer.md`). Merged from `shader-lookup` (2026-10-16):
  shader lookups kept by what draws read from the shader objects instead
  of hashing the microcode every draw: -5.2% render thread cycles a
  frame; the main thread is now about as busy as the render thread on
  this PC, so `profile_render_thread.sh` skips `Main XThread` ("Shader
  lookup" in `docs/native-renderer.md`). Merged from `allocate-slots`
  (2026-10-16): the main thread had become the limit on this PC (99% of
  a core, the render thread 87%); `CommandList::AllocateSlots` (~51,000
  calls a frame) done natively with its diagnostics out of line: -14.4%
  main thread cycles a frame, +3.6% uncapped fps ("The main thread" in
  `docs/native-renderer.md`; `ITC_THREAD=main profile_render_thread.sh`
  profiles it). Merged from `perf-investigate` (2026-10-16): the lights
  nearest a point (`sub_820F4128`, ~6,240 lights tested a call, ~3 calls
  a frame) found natively, four lights tested at once with the same
  results: -14.1% main thread cycles a frame, +0.9% fps; the render
  thread (94%) limits again, the main thread at 84% ("Lights::FindNearest"
  in `docs/native-renderer.md`). Merged from `camera-ids` (2026-10-17):
  `CameraIds::Of` (~3,300 calls a frame, ~250 distinct matrices) keeps
  each matrix's camera number in a 1,024-entry table by its bytes
  (`--cache_camera_ids`), 98% hits, same numbers: -16.7% main thread
  cycles a frame, +0.7% fps (render thread limits; "CameraIds::Of" in
  `docs/native-renderer.md`). Merged from `drawstate-investigate`
  (2026-10-17, released in v0.4.4 with the three before it: main thread
  -41.7% cycles a frame against v0.4.3): the tick's products worked out once a tick and shared by
  its frames (`--publish_by_tick`), `Note`'s per-frame maps as flat tables
  and its lookups in the order noted, the billboards' cameras blended once a
  frame (`--cache_billboard_cameras`), guest matrices loaded and stored four
  words at a time: -22.0% main thread cycles a frame, render thread
  unchanged ("DrawState::Note and AfterDraw" in `docs/native-renderer.md`;
  next leads there: the game's `sub_821298C0` and `sub_8209C6E0`). A larger lead, translating
  draws once a tick: `docs/tick-replay.md`. Still to decide: the old
  per-draw path's future, and a Steam Deck test.
