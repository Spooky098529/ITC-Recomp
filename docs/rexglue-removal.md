# Removing ReXGlue: toward a native port

The plan (written 2026-10-19 on branch `remove-rexglue`, restructured the
same day; A0, A5, A6 and B1-B6 done, see "Where things stand") for taking the game off the ReXGlue SDK
(`rexglue-sdk/`) piece by piece, with releases along the way. The goal
is a native PC port, not a smaller emulator: in the end no `PPCContext`,
no guest memory layout, no Xbox kernel calls, no `default.xex`.

Branches (the user's call, 2026-10-20): the work lives on the
long-lived branch `rexglue-removal-project` (first named
`rexglue-remove-1`), and the plan's steps are merged into it. `main`
keeps the game in its current playable state, with bug fixes (graphics
and the like); the project isn't merged into `main` without the user
saying so.

How to use this doc: pick the next item from a track, do it, mark it
`Done (<date>, <branch/commit>)`, and write what was found below it (the
way `docs/native-renderer.md` records its milestones). If something here
turns out wrong, say so instead of deleting it. CLAUDE.md's rules hold
throughout: logs before guesses, commit often and push the work branch,
ask before touching `main` or tags, never edit `generated/` (native code
goes in `src/` as overrides), American English.

## Where things stand (2026-10-21, after B5's and B6's leftovers)

- **Track B's boundary is done** (B1-B6). Of the 295 boundary functions
  still live, all but three belong to the core (threads and sync 264,
  memory 18, the heap's helpers counted under pure helpers 22; "The
  boundary over time"). The three: Voice::Init and its talker set-up
  (`XamNotifyCreateListener`, `XamUserReadProfileSettings`), `xstart`
  (never reached in play) and the movie player's stream set-up
  (`DbgPrint`); all go with B7. From here `boundary_status.py` mostly
  measures the core, so B7 needs its own measure: per-subsystem counts of
  the statuses in `docs/functions/`.
- **Track A**: A0, A2, A5 and A6 done (A2's flag parser waits for A4);
  A1 started (xxHash); A3 not started; A4 deferred until the code has no
  guest dependency (the user's call, 2026-10-21; see A4). What `src/`
  still takes from the SDK is in the table below.
- **Next (2026-10-21, after A4 was deferred):** B7, starting with its
  measure (per-subsystem counts of the statuses in `docs/functions/`) and
  a first subsystem: the leftovers above, or the main thread's
  performance leads. A3 alongside. A1's SDL and A2's flag parser wait for
  A4, at the end.
- **The order proposed before (2026-10-21, the user agreed to start with
  A2; A2 done the same day, merged from `own-logging`; A4 since
  deferred):** A2 (logging, flags, hashes: our own headers forwarding to the
  SDK's, then `src/` switched over area by area, then our own logger and
  hashing; the flag parser waits for A4), A4 (the app shell: our own
  `main`, loop, window and flag parser; it unblocks A1's SDL and finishes
  A2's flags), A3 alongside (the shader translator and our own register
  array), A1 after A4. B7 starts with its measure and a first subsystem:
  the leftovers above (the movie player, Voice::Init skipped as high as
  is safe), or the main thread's performance leads
  (`sub_821298C0`, `sub_8209C6E0`, in `docs/native-renderer.md`).
- **B7 in progress (2026-10-28):** its measure is `b7_status.py`
  (`docs/coverage.md`), and the B7 loop (`docs/loops/b7-loop.md`, a
  `/loop` session handing each subsystem to a mapping agent, then
  migration agents) did its first run on the branch `b7-loop-test` (merged 2026-10-22): five
  subsystems mapped and made native as far as they were ready (ours 162
  -> 335); its second run (branch `b7-loop-22-10-26`) removed
  31 adapters and made the race overlays native as far as they were
  ready (ours 422); its third run (the same branch; both merged
  2026-10-22) made the vehicle accessors, the matrix math and the
  car physics native as far as they were ready (ours 705); after native
  memory (ours 714), its fourth run (branch `b7-loop-22-10-26-2`, merged
  2026-10-23) made the C runtime heap, the collision narrow phase and
  the collision world native as far as they were ready (ours 837); its
  fifth run (branch `b7-loop-23-10-26`, merged 2026-10-23) made the sound
  cues and XACT (the user's call: translated, no host library plays its
  data) native as far as they were ready (ours 970); after GetTickCount
  from the host clock (ours 971), its sixth run (branch
  `b7-loop-23-10-26-2`, merged 2026-10-23) made
  the action mappers and the data tables native as far as they were
  ready, with no change in the game's behavior (ours 1,023); its
  seventh run (branch `b7-loop-23-10-26-3`, merged 2026-10-23 after a
  drive) made the
  model animation, the car specs and the cameras native as far as they
  were ready (ours 1,139; the cameras keep frame interpolation's hooks,
  with a new `BlendState_OnCopy`); its eighth run (branch
  `b7-loop-23-10-26-4`) made the string table and font, `std::string`
  (the user's call: translated over the guest layout) and the course's
  route native as far as they were ready (ours 1,223); its ninth run
  (branch `b7-loop-23-10-26-5`) made the AI drivers, the racer
  controllers and the track zones native as far as they were ready
  (ours 1,485); its tenth run (branch `b7-loop-24-10-26`) mapped the
  graphics resources (`graphics-resources.md`, 44 rows) and stopped
  for two of the user's calls before migrating: `Edram::Alloc` native
  with a `std::mutex` for the kernel's critical section, and the
  resource locks' fence waits and cache flushes Dropped (2026-10-24);
  46 of 46 ours (ours 1,529). It then mapped the video output and the
  device lock (`video-output.md`) and stopped for the user's call on
  the device lock's kernel sync: the lock alone over a
  `std::binary_semaphore` (2026-10-24; 7 of 30 ours, with three camera
  passes), and made the device's state setters native (`device-state.md`,
  131 of 131; the packets nothing reads Dropped by the graphics rule;
  ours 1,672; merged 2026-10-24 after a drive); its eleventh run (branch
  `b7-loop-24-10-26-2`) made the compiler's array helpers, the course
  scenery (`course-scenery.md`, 38 of 73) and XAPI's memset with what
  it unblocked native as far as they were ready (ours 1,714); its
  twelfth run (branch `b7-loop-24-10-26-3`) made the post effects
  (`post-effects.md`, 44 of 57), the UI movie walk and the AI drivers'
  teardown, and the input manager (`input.md`, 29 of 29) with the action
  mappers' constructor native (ours 1,794), stopped on two harness lines
  missing; its thirteenth run (branch `b7-loop-24-10-26-4`) made the
  race's screen objects native (`screen-objects.md`, 38 of 42) and
  mapped the app framework, beside the hardcoded shaders and the movies
  through FFmpeg (ours 1,847); its fourteenth run (branch
  `by-loop-25-10-26`) made the app framework native as far as
  it was ready (`framework.md`, 56 of 68 ours, with `main` and
  `App::Run`; `--native_framework`; ours 1,903), after fixing an
  `ai_drivers` harness case that read a wild address; its fifteenth
  run (branch `b7-loop-25-10-26-2`) made the game modes, the model
  class (the memory blocks' lock a host mutex), the mode tick, the
  minimap's parts and `atexit` (the exit lock a host mutex) with what
  it unblocked native as far as they were ready (ours 2,013); its
  sixteenth run (branch `b7-loop-25-10-26-3`) made the scene models'
  set-up side, the UI movies' text draw, update and objects with the
  race HUD's movie picker (two leads without mapping), the scenery
  lists and the model draws, and the shader objects up to `Model::Load`
  with the course set and string table it unblocked native as far as
  they were ready (ours 2,152); its seventeenth run (branch
  `b7-loop-25-10-26-4`, 2 subsystems by the user's call) made the
  race's set-up, the rest of the car physics (a lead without mapping)
  and the vehicle code above it native in full (ours 2,255); its
  eighteenth run (branch `b7-loop-27-10-26`, merged 2026-10-28) made the app's update and
  its state stack native in full (`app-states.md`), the scene's loading
  objects as far as they were ready with their blockers in
  `car-models.md` and `models.md` (`scene-loaders.md`, 60 of 78; a lead
  without mapping) and the players' save data in memory in full
  (`save-data.md`; ours 2,400); its nineteenth run (branch
  `b7-loop-28-10-26`, merged 2026-10-28, 3 subsystems) made
  texture compression (`texture-compression.md`) and the vinyl sheet
  above it (`car-models.md`, two leads without mapping), the race world
  (`race-world.md`, 49 of 50) and the race's states (`race-states.md`,
  43 of 63) native (ours 2,540); its twentieth run (branch
  `b7-loop-28-10-26-2`, merged 2026-10-28, 3 subsystems) made the archive (`archive.md`,
  its four semaphores host ones by the user's call), the file layer
  with sprintf (`file-layer.md`) and the save system's interface
  (`save-interface.md`, 41 of 41) native (ours 2,628); its
  twenty-first run (branch `b7-loop-28-10-26-3`, 2026-10-28 to 29, 3
  subsystems, not yet merged) made the car sounds with X3DAudio
  (`car-sounds.md`, 62 of 62), XAudio's voices (`xaudio.md`, 92 of 96;
  its locks the game's, taken as the original does, by the user's
  calls) and XACT's waves and voices (`xact-waves.md`, 22 of 30) native
  (ours 2,804). The
  runs' rows wait for the user's review (`docs/loops/review.md`). See
  "B7's loop runs" under B7, and `docs/loops/b7-progress.md`. Guest
  allocation is no longer a block (`docs/native-memory.md`); what the
  rest waits on is in `docs/loops/leads.md` (the archive loader, the
  route, the scene and race set-up, guest vtable calls, the graphics
  device's resource calls).

## The approach (agreed with the user, 2026-10-19)

- **Don't rebuild what the SDK does.** Writing our own Xbox kernel calls
  with Xbox behavior (file handles, `KeWaitForSingleObject`, the handle
  table, heaps) is a second ReXGlue and gets thrown away later. Instead,
  make the *game's own functions* that call the SDK native; once nothing
  calls an SDK piece, delete it.
- **Work from the SDK boundary inward.** Only 449 of the 16,552
  recompiled functions call kernel imports (see "The boundary" below).
  Making those native removes the SDK subsystem by subsystem, long before
  the game logic inside is native.
- **Two tracks**, independent of each other:
  - **Track A**, host-side pieces a native port needs anyway (logging,
    flags, app shell, window, SDL input and audio output, the renderer's
    leftovers, third-party libraries). Can start now, in any order.
  - **Track B**, the native port from the boundary inward, subsystem by
    subsystem; each one deletes its SDK piece when done.
- **Map Xbox-only features, don't port them** (the user's rule,
  2026-10-20, from B5). Features the PC port won't use (voice chat,
  system link, Xbox Live, XMP, presence, and the like) are still read and
  written up in `docs/functions/`, but they aren't brought into C++:
  no stand-ins or shims imitating their Xbox interfaces. They're skipped
  at the highest point in the game's own code where that's safe (status
  Dropped, with the reason), or left out when that caller is made native.
  What the game does need from them (one signed-in local user, the
  profile's answers) becomes plain C++ with fixed answers.
- **Plug in the data the game uses, don't mimic the 360** (the user's
  rule, 2026-10-21, from B5). When replacing a system service, first
  read what the game's callers actually do with its answer (often one
  field, or one effect), and plug that in directly: hardcoded values, or
  the one action it led to. Don't rebuild the 360 mechanism behind it.
  In B5, notifications became "the app's reader builds the user table
  once" (no queue, listener masks or start-up set), and the profile
  read became a fixed table of the 26 settings the game asks for (no
  profile code). Harness cases still check what the game reads against
  the recompiled function. And read the game function before sizing a
  job: notifications looked big until the readers were read.
- **The core stays the SDK's, untouched, until the end**: guest memory,
  the threads that run recompiled code, the image load, `PPCContext`,
  the function table, the sync objects shared with recompiled code. These
  go only when the last recompiled function is gone ("The end state").

### The boundary rule

While any recompiled function remains, data it touches has to stay in
guest memory in the Xbox layout (big-endian, 32-bit guest pointers). So
a native function comes in two levels:

1. **Native, guest data**: native C++ (no `ctx` registers inside, no
   instruction-by-instruction translation) that still reads and writes
   guest memory through `src/guest.h`, called from and calling recompiled
   code through `REX_FUNC` overrides. This is how `src/native/`,
   `src/hooks/lights.cpp` and `src/hooks/cockpit_dials.cpp` work today.
2. **Native, native data**: real structs and pointers. Possible for a
   struct only when every function touching it is native.

Track B functions start at level 1. A subsystem moves its data to level 2
when no recompiled code touches it any more. Shared sync objects (a
critical section in guest memory locked by both native and recompiled
code) must keep one implementation for both, which is why threads and
sync are part of the core, not the boundary.

### How a native override is written and checked

Existing practice (CLAUDE.md, "generated/ is the recompiled code"):
`extern "C" REX_FUNC(sub_XXXXXXXX)` in `src/`, one override per
function (`grep -rn sub_XXXXXXXX itc_recomp/src` first). For each:

- Read the function in `generated/` and what's known in
  `docs/functions/` (the subsystem's file); write it down there as it's
  understood.
- **Plain C++ behind thin adapters** (the user's precedent, 2026-10-19,
  first done for B1): the logic is a plain C++ function with normal
  arguments and host structs, no `ctx` and no guest memory, in its
  subsystem's directory (e.g. `src/input/xinput.cpp`). The `REX_FUNC`
  override (e.g. `src/hooks/input_update.cpp`) only reads the registers, calls
  it, stores results big-endian in guest memory and sets `r3`. When the
  caller is made native later, it calls the plain function directly and
  the adapter is deleted, so the `ctx` edge moves up without rewriting
  the logic.
- A flag to fall back to the recompiled version (`__imp__sub_XXXXXXXX`),
  on by default once checked, in the repo's style
  (`--native_<subsystem>`), until the recompiled version is dead.
- Check: harness cases comparing both versions on the same inputs
  where that's possible (below), an attract race, and the play
  checklist (Testing) for what neither reaches.
  `src/debug/state_capture.cpp` and `frame_state_diff.cpp` may help.
- Speed: uncapped A/B, or profiles for small differences (CLAUDE.md).

### Checking with a harness

`itc_recomp/src/harnesses/` (since 2026-10-20) keeps the checks: each
harness runs native functions and the recompiled ones they replace
(`__imp__sub_XXXXXXXX`) on the same inputs and compares, byte for
byte, what each leaves in guest memory and r3. They're off unless
`--run_harnesses` names them (`all`, or a list such as
`time,strings`); they then run once, from the first Input::Update, log
`harness: <function>: N cases, M differ` (and the first ten
differences), and the game carries on. So:

```sh
itc_recomp/tools/attract_race.sh 30 --run_harnesses=all
```

- `harness.h` has what they share: `Both` (one case through both
  versions: the scratch area filled with 0xCD, inputs written, registers
  set, the function run, the output bytes and r3 kept), `Results`
  (counts and logs), `Random` (the same cases each run; each harness
  starts its own, seeded from its name, since 2026-10-24) and
  guest memory helpers. The scratch area is 512 bytes below the stack
  pointer, and calls run with their stack below it.
- One file per area, registered in `harness.cpp`'s list (and the flag's
  help text). A new native function gets its cases there, in the same
  change: random inputs over its branches, edge cases (empty, none,
  too small, overflow), and the game's real inputs when known.
- Inputs that come from elsewhere are fed by a stand-in override that
  answers with made-up data while a harness asks and calls the
  recompiled function otherwise (the time harness's console time zone
  read, `sub_8229B440`, Wrapped in `functions/system-services.md`). When that function
  is made native, the stand-in moves into its native version.
- Where ours deliberately differs, the cases go in a group of their own
  named "(expected to differ)" (the time conversions outside 1678-2261,
  where the SDK's overflow), so the others stay at 0.
- What an attract race uses: a temporary counter override (log the
  first calls and powers of two, with `ctx.lr`) on the functions in
  question, removed afterwards (a kept one would block a real override
  of the same function). Functions the race never calls rely on the
  harness alone.
- Not everything fits: functions that wait on hardware or time (the
  audio library's wait, `sub_8245D288`) are checked by attract races
  and the user's play.
- Harnesses need the recompiled functions, so they go when `generated/`
  does (E4).

## What the game uses from the SDK

Measured 2026-10-19 by grepping `itc_recomp/src` and `generated/`;
updated 2026-10-21, after B1-B6 (the Track B rows say what was there
first and what's left).

| Area | What we use from the SDK | Where | Track |
|---|---|---|---|
| Logging, flags, hashes | `REXLOG_*` (~280 uses; 327 on 2026-10-21), `REXCVAR_*` (~370; 408), `rex/hash.h` (xxHash); about 87 includes of `rex/logging.h`, `rex/cvar.h` and `rex/hash.h` | almost every file in `src/` | A |
| App shell | `rex::ReXApp` (`src/itc_app.h`), `REX_DEFINE_APP` (`src/main.cpp`), `WindowedApp`, imgui (include path only), command line, log files, `PathConfig`, the SDK's hidden window (`src/native/app_window.cpp`) | `src/itc_app.h`, `src/main.cpp` | A |
| Renderer leftovers | The Xenos to SPIR-V shader translator built from SDK sources (`itc_xenos_shaders` in `itc_recomp/CMakeLists.txt`), `rex::graphics::RegisterFile` (`src/native/state.h`, `passes.cpp`), `registers.h`, `xenos.h`, `util/draw.h`, `ui/vulkan/api.h`; SDL3 and Vulkan headers from `rexglue-sdk/thirdparty/`; the xenos plugin, no longer built for us or loaded since B3 (the SDK's own tree still builds it) | `src/native/` | A |
| Graphics boundary | Was `Vd*` (25 imports; 17 game functions). Done (B3): native or Dropped, the emulated GPU not built or loaded | — | B |
| Audio | Output: done (A6, `src/audio/output_mix.h`); left: the `AudioSystem` base class (the runtime's audio slot, A4) and the `audio_mute`, `audio_maxqframes` flags (A2). The game side (was `XAudio*`, `XMA*` and the SDK's XMA decoder): done (B2, our own FFmpeg). Our mixer (`src/audio/native_audio.cpp`) still runs on an `XHostThread` and allocates from the guest heap (the core) | `src/audio/` | A, then end |
| Input | Done (A5, B1): `src/input/`; it pumps SDL through the SDK's UI thread queue until A4 | — | A4 |
| Files, saves | Was `Nt*File` and friends, `Ob*SymbolicLink`, `XamContent*` through the SDK's VFS (22 game functions). Done (B4): nothing the game opens goes through the VFS; the SDK still reads `default.xex` through it at start-up (E3) | — | end |
| System services | Was `Xam` UI, user, profile, sessions, voice, notifications, `XMsg*`, `XamLoader*`, `NetDll_*` (78 game functions). Done (B5); left: Voice::Init and its talker set-up (B7), `xstart`'s `XamLoaderTerminateTitle` (never reached) | — | B7 |
| Pure helpers | Was `Rtl*` strings, time, fills, `sprintf`, `vswprintf`, `KeQuery*`, `XGet*`, `DbgPrint` (55 game functions). Done (B6); left: the heap's helpers (the core) and `DbgPrint` in `xstart` and the movie player (B7) | — | B7, end |
| Core | Guest memory (4 GB mapping plus the physical view at +4 GB, `src/native/state.h`; `/dev/shm/xenia_memory_*`), allocators (`NtAllocateVirtualMemory`, `Mm*`, `XamAlloc`: 31 game functions), threads and sync (`ExCreateThread`, `Ke*`, `Rtl*CriticalSection`, events, waits, TLS, the handle table: 291 game functions), the PCR, the XEX image load (`Runtime::LoadXexImage`), `rex/ppc.h` and the other headers `generated/default/itc_pch.h` includes, the function table `itc_register.cpp` fills | SDK `src/system/`, `src/kernel/`, `include/rex/` | end |
| Build, packaging | `generated/rexglue.cmake`, `patches/rexglue-sdk/`, `librexruntime.so`/`rexruntime.dll`, third-party libraries through the SDK (SDL3, fmt, spdlog, FFmpeg, glslang, xxHash, imgui, cli11, tomlplusplus) | CMake, CI | A, then end |

## The boundary

Every game function that calls a kernel import directly, made by this
pipeline (re-run it as things change; it prints `<function> <import>`):

```sh
cd itc_recomp/generated/default
awk '/DEFINE_REX_FUNC\(/ {match($0,/DEFINE_REX_FUNC\([A-Za-z_][A-Za-z0-9_]*/); f=substr($0,RSTART+16,RLENGTH-16)}
     /__imp__[A-Za-z]/ && !/__imp__sub_/ && !/DEFINE_REX_FUNC/ {
       match($0,/__imp__[A-Za-z][A-Za-z0-9_]+/); print f, substr($0,RSTART+7,RLENGTH-7)}' \
  itc_recomp.*.cpp | sort -u
```

Corrected 2026-10-20 (B5): it first matched only `sub_` names, so the
imports of a function with another name were credited to the `sub_`
function before it in the file. The only such caller is `xstart` (the C
runtime's entry point, `0x82296C80`): its `DbgPrint` and
`XamLoaderTerminateTitle` were listed under `sub_822954C8`, a small wait
wrapper. The totals didn't change.

2026-10-19: 916 pairs, 449 distinct functions. By area (functions):
threads and sync 291, system services 78, pure helpers 55, memory 31,
files and saves 22, graphics 17, audio 6, input 3. Overrides in `src/`
already replace some callers; check each against `grep -rn` before
counting it as open.

Where each boundary function stands (overridden, Native, Dropped,
never called, or still live), by plan item:
`itc_recomp/tools/boundary_status.py` (it runs this pipeline and reads
`docs/functions/` with the status check's code). Record its output in
"The boundary over time" at the end of this doc after each plan item.

These are the direct callers only. Making one native often means making
its close callers native too (a wrapper and the few functions using its
handles), which is fine: the unit of work is a subsystem, not a function.

## Testing

- Attract races without the user: `itc_recomp/tools/attract_race.sh
  <seconds> [flags]` (`--skip_intro --attract_after_s=1` under
  `timeout`; prints the log, seconds raced, errors and harness results),
  frames with `--native_renderer_dump` (CLAUDE.md, "Build and run").
- Harnesses: `--run_harnesses=all` (above).
- Speed: uncapped (`--max_fps=0`) alternating runs; for changes under a
  few percent, `itc_recomp/tools/profile_render_thread.sh` (also
  `ITC_THREAD=main`).
- The user's play checklist before a release: boot to the title, the
  menus, load and save a career save made by the last released build, a
  race (opponents, pause menu, rival intro), audio throughout, gamepad,
  quit cleanly; the Windows build under Wine
  (`docs/windows.md`).
- Test zips: CLAUDE.md, "Test builds for the user".

## Track A: host-side pieces

Independent of each other; each is releasable alone, except A0, which
comes before everything else.

A0. Done (2026-10-19, `remove-rexglue`): `--mnk_mode=true` removed from
    `tools/profile_render_thread.sh` and the run lines
    in CLAUDE.md and `docs/windows.md`. The SDK's flag defaults to off, which
    only turns its keyboard driver off (`rexglue-sdk/src/input/mnk/`);
    gamepads go through its SDL driver as before. `app_window.cpp`'s
    forwarding stays: our window's focus and close events need it (only
    its comments and log line changed).
    **Drop keyboard input** (the user's call, 2026-10-19): no keyboard in
    native code until the game is fully native (after B7). Remove
    the keyboard from `app_window.cpp`'s forwarding and its comments in
    `app_window.h`; say in the release notes that keyboard play is gone
    for now (gamepad only). From then on, input in this plan means
    gamepads only, and nothing needs the SDK window's listener for keys.

A1. Started (2026-10-21, xxHash, below). **Third-party libraries from
    our own copies** (submodules or FetchContent, pinned versions) instead of `rexglue-sdk/thirdparty/`:
    SDL3, Vulkan headers, fmt, spdlog, xxHash, glslang. While
    `librexruntime` exists it links its own; only one SDL may be loaded
    (see patch 0003 and `docs/windows.md`), so SDL probably stays the
    SDK's until A4.

    xxHash done (2026-10-21, `own-logging`, with A2): the submodule
    `itc_recomp/thirdparty/xxHash` (v0.8.3), header-only, on the include
    path as `itc_recomp/thirdparty/` (included with its folder).

A2. Done (2026-10-21, `own-logging`; the flag parser waits for A4).
    **Logging, flags, hashes**: our own headers (`src/base/log.h`,
    `cvar.h`, `hash.h`, names to taste) with our own macro names, first
    forwarding to the SDK's, and every file in `src/` switched over
    (mechanical; one commit per area). Then our own implementations for
    logging and hashing. Our own flag parser has to wait for A4: until
    then the SDK's flags (`--game_data_root`,
    `--fullscreen`, ...) are parsed from the same command line by the SDK.
    Log file names must stay (`logs/itc_<n>.log`): CLAUDE.md and tools
    rely on them. (Rotation into `itc_<n>.1.log` was dropped with our own
    logger.)

    Steps 1 and 2 done (2026-10-21, `own-logging`): `src/base/log.h`
    (`ITC_LOG_DEBUG`, `_INFO`, `_WARN`, `_ERROR`, and
    `base::InfoLogEnabled`, moved from `native/log_enabled.h`),
    `src/base/flags.h` (`ITC_FLAG_BOOL`, `_INT32`, `_STRING`,
    `ITC_FLAG_DECLARE`, and `ITC_FLAG(name)` for a value) and
    `src/base/hash.h` (`base::Hash64`, XXH3-64 inlined, with or without a
    seed), all forwarding to the SDK's. Every file in `src/` switched over,
    one commit per directory; nothing outside `src/base/` uses the SDK's
    log, flag or hash macros or includes `rex/logging.h`, `rex/cvar.h` or
    `rex/hash.h`. Flag names and spellings are unchanged (the SDK's
    integer flags are `INT32`; there's no plain `INT`). Files that used a
    macro now include its header: `native/shaders.cpp` had the SDK's
    flags only through `rex/hash.h`. Checked: an attract race with every
    harness (no errors, 16 s raced, all the same but the groups expected
    to differ). Step 3 next: our own logger and hashing.

    Hashing done (2026-10-21, `own-logging`): `base/hash.h` includes our
    own xxHash (A1: the submodule `itc_recomp/thirdparty/xxHash`, v0.8.3,
    the commit the SDK pins), as `<xxHash/xxhash.h>` from
    `itc_recomp/thirdparty/` so the SDK's copy on the include path can't
    stand in for it. Ninja's dependencies show our four hashing files
    (`passes.cpp`, `shaders.cpp`, `textures.cpp`, `replay_stats.cpp`) on
    ours; the SDK's copy is only in SDK sources (`itc_xenos_shaders`'s
    `draw.cpp` among them, until A3). Same hashes: an attract race loaded
    every shader from the disk cache (keyed by the microcode's hash), none
    translated. Left of A2: our own logger, and the flag parser (A4).

    Our own logger done (2026-10-21, `own-logging`; the user's call: plain
    stdio, not a copy of the SDK's spdlog set-up). `src/base/log.cpp`:
    lines formatted with `std::format` (our format specs need no changes),
    written as one `fwrite` under a mutex and flushed (as the SDK's file
    sink flushed at info), in the SDK's layout with `[itc]` as our
    category: `[2026-10-21 23:03:57.966] [info] [itc] [t13876] ...`. The
    macros check the level before evaluating their arguments, as the SDK's
    did; `base::InfoLogEnabled` is an atomic load (it was a call into the
    runtime). Lines from before the file opens are kept and written then.
    No rotation (proposed; a size cap is easy to add if wanted): one file
    a run, where the SDK rotated at 5 MB into `itc_<n>.1.log`. With `log_level = "off"` (as released)
    nothing is written; the SDK still creates the empty file, as before.

    The SDK's side until A4 (`src/base/log_sdk.cpp`, from
    `ItcApp::OnPostInitLogging`, no SDK patch): the SDK still names and
    creates the file (`itc_<n>.log`, one more than the highest there).
    Ours finds its rotating file sink on the `core` logger, takes its file
    name, sets it to off and removes it from every logger, opens the same
    path, and adds a sink forwarding the SDK's own lines (`[core]`,
    `[sys]`, ...; already filtered by `--log_level`) into ours. Setting it
    off matters: a category the SDK registers later (on first use; `sys`
    for the guest memory's line) is built with the SDK's file sink again,
    and wrote its lines twice before. At A4 our `main` names the file and
    sets up the SDK's logging without one. Checked: attract races with
    every harness (no errors, no duplicate lines, 17 s raced), and one
    with `--log_level=off` (empty file). Left of A2: the flag parser
    (A4); the 158 `fmt::format` calls in `src/` can move to `std::format`
    with A1's fmt.

A3. **Renderer leftovers**: bring the shader translator into the repo
    (`src/native/xenos/`; the files `itc_xenos_shaders` builds, listed in
    `CMakeLists.txt`, and the headers they need). The game's shaders are
    Xenos microcode in its data, so a native port still translates them;
    this isn't emulation. Read `rexglue-sdk/LICENSE` first (Tom Clay's
    work, partly derived from Xenia), keep its notices on copied files,
    ask the user if the terms are unclear. Our own register array in
    place of `RegisterFile`; the formats from `xenos.h`/`registers.h`,
    `draw_util` and the Vulkan function list the same way. The xenos
    plugin isn't kept for comparisons (the user's call, 2026-10-20; see
    B3): our build no longer uses it since B3 (the SDK still builds it
    in its own tree until the SDK goes).

A4. **App shell**: our own `main`: SDL init, our window as the only one
    (no hidden SDK window, no event forwarding in `app_window.cpp`), our
    own flag parser (all flag names kept, including the SDK's that the
    game still needs while they exist), the `cache/` directory, the close
    path. Remove `ItcApp`, `REX_DEFINE_APP`, imgui. It still starts the
    SDK's runtime (memory, kernel, image load) through its API: that's the
    core and stays. Find the minimal setup in
    `rexglue-sdk/src/ui/rex_app.cpp` and `src/system/runtime.cpp`.

    **Deferred until the code has no guest dependency** (the user's call,
    2026-10-21, after the survey below). Nothing in Track B or A3 needs it:
    the SDK's flag parser, its hidden window and the log file takeover
    (`src/base/log_sdk.cpp`) all work. Done now, our `main` would still start
    the SDK's `Runtime` (memory, kernel, image load) through its API, and
    that start-up is replaced anyway when the core goes; done last, it's
    written once. A1's SDL and A2's flag parser wait with it.

    The survey (2026-10-21), for when it's picked up again. From process
    start to the first frame, all on the process's main thread (the SDK's
    "UI thread"), as `windowed_app_main_sdl.cpp` and `rex_app.cpp` (both
    compiled into our executable by `rexglue_configure_target`) run it:
    1. `rex::cvar::Init(argc, argv)`: CLI11 parses the command line against
       the SDK's flag registry (ours are in it too, through
       `ITC_FLAG_*`); unknown `--name=value` kept for late registrations.
       `ApplyEnvironment()` (`REX_<NAME>` variables; nothing of ours uses
       them), `InitLoggingEarly()` (a stdout sink).
    2. `SDLWindowedAppContext::Initialize`: `SDL_InitSubSystem(VIDEO)`
       (honors `--video_driver`), two registered event types (wake-up,
       paint), an event watch; `CoInitializeEx` on Windows.
    3. `ReXApp::OnInitialize`:
       - `SetupEnvironment`: paths from `--game_data_root` (our
         `OnConfigurePaths`: the executable's folder when empty),
         `--user_data_root` (default the platform's user folder + `itc`),
         `--cache_root` (default `<user>/cache`; only the emulated GPU's
         shader storage used it; our `cache/` is `native/disk_cache.cpp`'s),
         update and metadata roots. Only then is `itc.toml` (next to the
         executable) loaded, which is why it can't set `game_data_root`.
         Logging: `BuildLogConfig(--log_level, [log.levels] in the toml)`,
         `InitLogging` makes `logs/itc_NNN.log` in the executable's folder
         (`{:03d}`, one more than the highest there; `--log_file` overrides);
         our `OnPostInitLogging` takes the file over.
       - `SetupPresentation`: the `RuntimeConfig` (audio: the SDK's SDL
         audio, ours through `native_audio::Configure`; input: nulled by
         us; `kernel_init = rex::kernel::InitializeKernel`; no GPU plugin);
         a 1280x720 SDK window (we hide it) titled
         `itc [rexglue-v<version>-<config>]`, its listeners, imgui, the
         overlays and their keybinds; our `Gamepads_Start`, which queues
         its work through `CallInUIThreadDeferred`.
       - `ConstructRuntime`: `Runtime(paths)`, `set_app_context` (used only
         by Xam's dialogs, none live since B5; nothing reads
         `display_window`), `Runtime::Setup(PPCImageConfig, config)`
         (memory, exports, dispatcher, VFS, KernelState, kernel modules,
         audio, the function table), `LoadXexImage("game:\\default.xex")`;
         no CRT heap (`REXCRT_HEAP 0`); `OnPostSetup` (thread CPU logging).
    4. The loop (`SDL_WaitEvent`); its first queued function launches the
       module: `PrepareModuleLaunch()`, `Resume()` on the guest's main
       thread, and a `std::thread` that waits for it and quits the loop
       (it never returns in play).

    Elsewhere: our renderer makes its window on the main thread
    (`SDL_RunOnMainThread` and a pushed wake event) and registers it in the
    SDK's event table (`native/app_window.cpp`); gamepads are watched on
    whichever thread pumps, opened and pumped on the main thread. Close:
    our window's close request (or `SDL_EVENT_QUIT`, so SIGINT and
    `timeout`'s SIGTERM too) reaches `ReXApp::OnClosing`:
    `kernel_state()->TerminateTitle()`, flush, `std::_Exit(0)`.
    `OnShutdown` and `OnDestroy` only run if the guest's main returns, so
    never in play. Unused by us: imgui, the overlays and keybinds,
    achievements, the SDK window itself.

    The plan proposed then (not agreed, revisit with the core's removal):
    our own flag registry and parser (`itc.toml` then the command line;
    names the SDK registers forwarded with `rex::cvar::SetFlagByName`;
    `fullscreen`, `log_level`, `audio_mute`, `audio_maxqframes` and
    `video_driver` ours); a `--log_flags` flag landed first on the old
    shell so flag values can be compared before and after; our `main`
    naming the log file as the SDK did and calling `rex::InitLogging` with
    no file; `SDL_main.h` for Windows' entry point; the gamepads' queue on
    `SDL_RunOnMainThread`; the two SDK app sources taken off our target's
    source list in our `CMakeLists.txt`. Open questions then: drop the
    `REX_*` environment variables, no log file at all with
    `--log_level=off`, `itc.toml` able to set `game_data_root`.

A5. **Input reading**: SDL gamepads (rumble too) as a native module with
    no Xbox types. No keyboard (A0). Its game side is B1; do the two
    together.

    It doesn't need our own SDL loop first (checked 2026-10-19). The SDK
    owns the loop today (`SDL_WaitEvent` on the UI thread,
    `RunMainMessageLoop` in `rexglue-sdk/src/ui/windowed_app_context_sdl.cpp`),
    and its gamepad driver gets events through `SDL_AddEventWatch`
    (`src/input/sdl/sdl_input_driver.cpp`, which also asks the UI thread
    to pump when the game reads input). Ours: our own event watch (called
    for every event whoever pumps, so it keeps working when A4 brings our
    own loop). The watch runs on the pumping thread: guard the state the
    game thread reads. To avoid clashing with the SDK's input while both
    exist:
    - Only one answers the game at a time (B1's fallback flag). Opening
      a gamepad twice is fine in SDL; rumble only from the active path.
    - The same pad as player 1 as the SDK picks
      (`src/input/device_assignment.cpp`), or the user's controller
      changes when the flag flips.

    Step 1 done (2026-10-19, `rexglue-removal-project`): `src/input/gamepads.{h,cpp}`,
    not yet answering the game. Started from `ItcApp::SetupPresentation`
    (UI thread). Our event watch only updates four slots (lowest free slot
    on connect, kept until unplugged, as the SDK does) under our mutex and
    never calls SDL; pads are opened and closed on the UI thread, queued
    by the watch (`CallInUIThreadDeferred`), and pads already connected are
    taken from `SDL_GetGamepads()`. Reads and rumble queue an
    `SDL_PumpEvents()` on the UI thread, as the SDK's driver does (it
    queues one on every `XamInputGetState`, capabilities and vibration
    call, `QueueControllerUpdate`). State is SDL's: buttons as bits by
    `SDL_GamepadButton`, axes as SDL gives them; B1 converts. Checked in an
    attract race: the pad found and opened at boot, button presses seen
    with `--log_gamepads`, clean shutdown. Rumble is untested until B1.

    Done (2026-10-19, with B1): since B1 the game's input comes only from
    this module (the SDK's input system isn't created), and the user
    checked every button, both sticks and triggers, rumble and unplugs
    while playing. What still ties it to the SDK is covered elsewhere: it
    is started from `ItcApp` and pumps SDL's events through the SDK's UI
    thread queue, as the SDK owns the event loop until A4 (it watches
    events whoever pumps them, so it keeps working then), and it uses the
    SDK's logging and flags like the rest of `src/` (A2).

A6. **Audio output**: SDL audio straight from our mixer's output, our own
    conversion and downmix (`conversion.h`, `downmix.h` are small), in
    place of `AudioDriver`. Its game side is B2. Read
    `docs/native-audio.md` first.

    Done (2026-10-21, `native-xma`, with B2's step 4): the conversion and
    stereo fold are ours (`src/audio/output_mix.h`), with the numbers the
    SDK's defaults gave (nothing ever changed them: center and back at
    -3 dB into both sides, LFE dropped, scale 1 / (1 + 0.707); 5.1
    passed through). What's left of the SDK in the output is its
    `AudioSystem` base class (the runtime's audio slot: A4) and its
    `audio_mute` and `audio_maxqframes` flags (A2).

## Track B: the native port from the boundary inward

Smallest and most self-contained first. Each item: make the game's
callers (and close neighbors) native at level 1, delete or stop building
the SDK piece nothing calls any more, release. Start every item by
listing its functions (the pipeline above), reading them in
`generated/` and writing them up in `docs/functions/`.

B1. **Input** (3 functions calling `XamInput*`): native, reading A5.
    Then the SDK's input system goes. The user checks gamepad, rumble, the pause menu's repeat (`docs/pause-menu-fix.md`; the action
    mappers' override in `src/hooks/input.cpp` stays as it is).

    Done (2026-10-19, `rexglue-removal-project`), with no fallback flag (the
    user's call: no SDK for game input). `src/hooks/xinput.cpp`
    overrides the three wrappers (`sub_825BA3F0`, `sub_825BA3F8`,
    `sub_825BA408`; only `sub_82591C18` calls them) with the SDK's
    structs, return codes and SDL mapping (stick y as `~value`, triggers
    as `value >> 7`, the guide button left out, packet numbers one more
    per changed read). Capabilities are fixed (a wired gamepad, every
    button but the guide): the game stores them on connect (pad +192)
    and nothing known reads them; the SDK also flagged wireless pads and
    pads with no d-pad. `ItcApp::OnPreSetup` clears the runtime's
    `input_factory`, so the SDK's input system (InputSystem, its SDL,
    XInput, keyboard and nop drivers) isn't created; the log no longer
    has "SDL input driver initialized". Dropped with it:
    `--input_backend`, `--guide_button`, `--hid_mappings_file` (a
    `gamecontrollerdb.txt` we never shipped).

    **Behavior change: an unplugged pad pauses the race.** The SDK's nop
    driver spoofed a connected, idle pad on user 0 at all times
    (`rexglue-sdk/src/input/nop/nop_input_driver.cpp`), so the game never
    saw a pad leave. Now it does, as on a 360 (pausing on a lost
    controller was a certification rule), and the race pauses. Checked by
    the user: four unplugs, in a menu and in a race; each seen by the game
    within ~30 ms (`xinput: user 0 disconnected`), the pad back in slot 0
    on replugging, and the race resumed fine.

    Then (2026-10-19) the `ctx` edge moved up one level: the pad read
    `sub_82591C18` is plain C++ (`src/input/physical_pad.cpp`, the
    constants as literals) calling `input::XInput_*` directly, behind an
    adapter (first `src/hooks/physical_pad.cpp`) that copies the pad's fields
    in and out of guest memory (the recompiled logical pads read its
    values there). The three XInput adapters (first in
    `src/hooks/xinput.cpp`) were deleted: their only caller is native.
    Checked against the recompiled read with a temporary byte-for-byte
    comparison of the 212-byte pad every read (not committed): 7,800
    reads in an attract race and 17,400 while the user played (every
    button, both sticks and triggers, rumble, an unplug), all equal but
    one, where the right stick moved between the two reads.

    Then (2026-10-19) up one more: Input::Update (`sub_820990F0`) is
    native (`src/hooks/input_update.cpp`, called from its override in
    `src/frame_pacing/game_messages.cpp`, which keeps the render-only
    skip). It reads the physical pads with `PhysicalPad_Read` directly
    (the pad read's adapter is deleted) and builds the logical pads
    (`sub_82591918`, now `src/input/logical_pad.cpp`, plain C++); the
    devices' values getters of the two kinds bound in every race are done
    natively, any other is called. The processors and listeners (vtable
    slot 1, `0x82592188`, not read) are still guest calls, inside the
    recompiled function's 144-byte frame. So the `ctx` edge is now
    Input::Update itself: native, calling out to recompiled code for those
    two steps, and the input manager stays in guest memory (they and the
    action mappers read it). Checked with a temporary comparison of each
    logical pad against the recompiled `sub_82591918` (not committed):
    9,000 builds in an attract race and 12,000 while the user played
    (menus, a race, the pause menu), all equal; the user noticed no
    difference.

    Then (2026-10-19) Input::Update's last guest calls: the processors'
    and listeners' update (`sub_82592188`, read and written up in
    "The input processors" in `docs/pause-menu-fix.md`) is plain C++
    (`src/input/input_processor.cpp`), called directly by Input::Update
    for their class, with an adapter override for any recompiled caller
    (it's reached through a vtable). Input::Update calls guest code only
    for an object or device of a class it doesn't know, which no race
    has. Checked with a temporary byte-for-byte comparison of each
    416-byte object against the recompiled update (not committed):
    16,200 updates in an attract race and 13,500 while the user played
    (held directions in menus and the pause menu, a race), all equal;
    the user noticed no difference. The input path is ours from SDL to
    the hold counters; the action mappers (`sub_8210B018`, wrapped) read
    the processors next.

    Who else reads that data (2026-10-19 and 20; "Who reads the input
    manager" in `docs/pause-menu-fix.md`, from unattended runs and a
    session the user played through the front end, a race and the pause
    menu): besides the mappers (logical pads 0-3's values 0-7 through
    mapper +2880, and the listeners' held words), the app's update
    `sub_82093DC0` tests processor 0's pressed, held and repeat state
    (`sub_8258CEB0`, `sub_8258CEE0`), and in a race `sub_8215FCA8` and
    `sub_822231B0` read a physical pad's connected flag and capabilities
    subtype (`sub_8209F340`, `sub_8209F3B8`). So native mappers alone
    wouldn't let the input data leave guest memory: processor 0's +264,
    +272, +276 and counters, and the physical pads' +4 and +192..+211
    would keep a guest copy until those readers are native too.

    The action mappers (2026-10-20): read and written up ("The action
    mappers' update" in `docs/pause-menu-fix.md`: `sub_8210C370` and the
    five functions below it), not made native. The user's call: probably
    more than input needs for now, since the mappers are game logic (the
    actions, their analog curves, thresholds and repeats), and the data
    below them can't leave guest memory yet anyway. What's left to read
    before doing it: the repeat and curve objects' classes, four
    constants and the config tables. Input stops at the processors for
    now: from SDL to the hold counters is ours (B1), the mappers up are
    recompiled.

B2. **Audio** (6 functions calling `XAudio*`/`XMA*`, plus what's around
    them): the game's render driver client feeding A6 directly, and its
    XMA users decoding with FFmpeg directly instead of through emulated
    XMA contexts. Keep the existing overrides (`sub_824657E0`,
    `sub_82465870`, `sub_824658C8`) working. The mixer thread stays an
    SDK thread until the core goes (it runs recompiled code). The user
    listens through menus, a race, the rival intro.

    Next (chosen 2026-10-21: the last boundary subsystem outside the
    core; A2 waits, as its churn over `src/` would make every merge from
    `main` conflict). XMA goes through our own code calling FFmpeg (the
    user's call, 2026-10-21), replacing the user's 2026-10-14 call to keep
    the SDK's decoder (`docs/native-audio.md`, "Decisions"). Live at the
    start: `sub_8245D138` (`XMACreateContext`), `sub_8245C8D0`
    (`XMAReleaseContext`) and `sub_8245B808`
    (`XAudioGetVoiceCategoryVolume` and `...ChangeMask`); the game's
    audio library also reads and writes the XMA registers itself
    (`sub_8245C638`, `sub_8245D138`, `sub_8245D208`, `sub_8245D288`,
    `sub_8245D350`, `sub_8245D4D0`), so the level to replace is the
    library's XMA layer, not just the two imports.

    **Survey (2026-10-21, branch `native-xma`)**, from `generated/` and
    a temporary counter (`--log_xma`, `src/hooks/xma_log.cpp`) in an
    attract race and a session the user played (boot, menus, a race;
    `itc_281.log`):

    - The source voices that play XMA (an XAudio "source voice" is one
      sound playing, not voice chat) each hold an XMA object at voice
      +184: +0 the stream count, +4 flags, +8 an array of 96-byte
      entries, one a stream. An entry is a copy of the stream's XMA
      context (64 bytes, the layout of the SDK's `XMA_CONTEXT_DATA`),
      then +64 the hardware context, +68 the output buffer, +72 the work
      buffer, +76 samples consumed, +80 the context id, +82 the position
      within the current 256-byte block, +84 and +88 the two input
      buffers' addresses. Flags: 0x10000 lock asked, 0x20000 the copies
      are current, 0x40000 contexts made.
    - Every update (`sub_8246C320`, from the mixer callback, ~118 a
      second a voice) does lock (`sub_8245D208`: writes the Lock
      registers `7FEA1A40`), wait (`sub_8245D288` polling `sub_8245D350`:
      `CurrentContextIndex` `7FEA1818` isn't one of its contexts, then
      copies each hardware context into its entry), then works on the
      copies (`sub_8246BB98`, `sub_8246B480` and the helpers: samples
      ready, read them, give input buffers), then unlock (`sub_8245D4D0`:
      the copies written back and the Kick registers `7FEA1940`).
      Between unlock and the next lock, the hardware decodes. The voice
      reaches the XMA object only through these helpers (every load of
      +184 goes to one as r3); it reads the decoded samples straight from
      the output buffer, at the address the helpers give.
    - What plays: one stereo 44.1 kHz voice a music track and a mono
      voice a menu sound, each with its context made when it starts and
      released when it ends. A sound's whole data is one input buffer,
      given once at set-up (`sub_8245C958`; 0.45-1.4 MB for music). 23
      contexts made in the user's session, never more than a few at
      once, one stream an object every time.
    - Music loops in the hardware: `sub_8245CF98` sets the loop data
      (`XMASetLoopData`: loop count 255, forever; loop start and end as
      bit offsets into the input buffer, e.g. `1FECB7` and `5D5993`;
      the subframes to skip at the start and play at the end), called
      once a music voice (4 times in the session). First written here as
      "no hardware loops" and "later input buffers by `sub_8245CF98`",
      both wrong: the loop count was read before that call.

    **Plan (agreed with the user, 2026-10-21):** we aren't emulating an
    Xbox; we take the game's audio and play it through SDL. So:

    1. **Our own FFmpeg from upstream**, pinned to a release in the
       repo and built with only the `xma2` decoder (A1's FFmpeg part,
       done here). Not the SDK's: it pins Xenia's fork
       (`wmarti/FFmpeg`), whose build has only `mp3`, `wmapro`, `wmav2`
       and `xmaframes`, the fork's frame-at-a-time decoder made for
       emulating the hardware (bit read offsets, mid-packet starts and
       loops). Upstream's `xma2` decodes whole 2 KB packets, which is
       what we feed. Loops: every XMA frame decodes to 512 samples a
       channel and starts with its length (15 bits), so our code walks
       the frame headers to turn the loop's bit offsets into sample
       positions and loops the decoded samples there (parsing, not the
       hardware's behavior). Both can be loaded at once:
       `librexruntime.so` links its FFmpeg statically and exports none
       of its symbols. Linux and the Windows cross-compile both build it.

       Done (2026-10-21, `native-xma`): the submodule
       `itc_recomp/thirdparty/FFmpeg` at `n9.0.2` (shallow), built by
       `itc_recomp/cmake/ffmpeg.cmake` as `itc_avutil` and `itc_avcodec`
       (static, hidden symbols; ~150 files, plain C, threads on: pthreads
       or Windows threads) from the files configure made, kept in
       `itc_recomp/thirdparty/ffmpeg-config/<platform>/` (local paths
       scrubbed) by `itc_recomp/tools/ffmpeg_config.sh`; run it again
       after moving the submodule. Not linked into `itc` yet.
       `itc_xma_check` (`itc_recomp/tests/xma_check.cpp`, not built by
       default) decodes a bank's wave and writes a WAV. On `bgm_tmp_01`
       and four `SB_SYSTEM_SE` sounds, Linux and Windows (Wine) alike:
       every packet decoded, and the packet headers' frames × 512
       (`src/audio/xma_packets.h`) equal to the bank's duration
       (3,024,896 for the music). The decoder's output runs exactly 5,184
       samples behind the frames × 512 timeline the whole way: 64 dropped
       at the start (`skip_samples`) and 10 frames held inside, given out
       at the flush. So decoded sample n is timeline sample n + 64, and
       loop points worked out on the game's timeline (`sub_8246A5F8`'s)
       carry over with that shift. The end loses 807 samples more (the
       last frame's trim in the data); the music's loop end (3,024,000)
       comes before it.
    2. **The XMA layer native**: the helpers above (set-up and release,
       input and output buffers, samples ready, read, reset, lock, wait,
       unlock) become adapters over a plain C++ stream (input buffers,
       an FFmpeg decoder, the output ring in guest memory where the voice
       reads it). Decoding happens at unlock, on the mixer thread, filling
       the output ring's free blocks; lock and wait have nothing to wait
       for. No contexts, no registers, no `XMACreateContext`.
    3. `sub_8245B808`'s category volume: the fixed answers (1.0, no
       change) without the imports.
    4. The SDK's audio system goes (`ItcAudioSystem` no longer starts
       its XMA decoder), and A6 (our own conversion and downmix).

    Read since (2026-10-21), and what step 2 needs from it:

    - The voice's queue of buffers is at voice +20 (address +8, size
      +12, loop count +16, state +116: 0 new, 3 handed over, 4 done).
      `sub_8246B480` hands a new buffer over (`sub_8245C958`) and, with a
      loop count, sets the loop data (`sub_8245CF98`): bit offsets the
      buffer carries (+40, +44), or with voice +266 = 2 worked out from
      sample positions by `sub_8246A5F8`. A buffer is done (the
      buffer-end callback, voice +16's vtable +92) once its address is
      no longer a valid input buffer (`sub_8245CA30`), or the error
      status (`sub_8245CF60`) has bit 8. So our stream clears its valid
      bit when decoding passes the data's end after its loops.
    - `sub_8246A5F8` is the game's own frame walk, sample to bit offset:
      2 KB packets, each header with the frame count (top 6 bits), the
      first frame's bit offset (15 bits, + 32 for the header) and the
      packets of other streams to skip (low 8 bits); each frame starts
      with its length (15 bits) and decodes to 512 samples a channel.
      `sub_8246A698` gives the last frame's offset (the data's end). Our
      loop handling is this walk the other way; subframes are 128
      samples, so the loop's skip and end counts place the loop points
      within a frame. Check the exact meaning of the skip and end
      against the SDK's decoder (`rexglue-sdk/src/audio/xma_context.cpp`)
      while writing it.
    - Never called in the user's session: `sub_8246BB98`'s second loop
      (from buffer +32 to the end, once, with `sub_8245CE10` moving the
      output by 128 samples), `sub_8246B1C8` and `sub_8245D070` (the input
      read offset). Step 2 can leave them out (logging if one is ever
      reached).

    **Done (2026-10-21, `native-xma`).** The XMA work went another way
    than step 2 above, for the rule "don't rebuild what the SDK does"
    (the user stopped the ring version: blocks, valid bits, subframes and
    decoding at unlock were the SDK's XMA emulation again):

    - Step 2: each sound is decoded with our FFmpeg into one flat buffer
      of big-endian 16-bit samples in guest memory (system heap; up to
      ~12 MB for a music track), from the game's data where it lies
      (`src/audio/xma_stream.h`): a second's worth when it starts, then
      kept a second ahead at each unlock (a whole track at once took
      ~60 ms, more than the ~42 ms of output queued). The helpers answer
      the voice from a position (`src/hooks/xma.cpp`): ready, where,
      taken; "ready" capped at the voice's own ring size, so the voice
      never sees a count it couldn't on the 360. Loops: the loop's bit
      offsets turned into sample positions by the frame walk
      (`FrameAtBitOffset`), the position jumps back at the loop end.
      Lock, wait and the error status answer at once. The game's frame
      walk (`sub_8246A5F8`) is ported (`BitOffsetOfSample`); harness
      `xma`: 512 cases on made-up packets, 0 differ. No fallback flag (the
      user's call): the harness covers the walk, the release build is the
      listening comparison.
    - Checked: attract races (five minutes; the user listened as well):
      0 silence fills (but the window `timeout` ends in, and once 1 at a
      race load that didn't come back in two more runs); the loop points
      equal to the banks' loop regions less the decoder's 64
      (`bgm_tmp_04`: 458,496 to 1,305,216 in the bank, 458,432 to
      1,305,152 ours; `bgm_tmp_06` likewise); each sound ends at its
      decoded length (`SB_SYSTEM_SE`'s 18,368 samples as
      `itc_xma_check`). Not yet seen: the loop jump itself (no attract
      race lasts to a track's loop end; `--log_xma` logs `loop:` when it
      happens).
    - Step 3: the mixer's category volume pass (`sub_8245B808`,
      `src/hooks/audio_volume.cpp`) without its two imports: the change
      mask 0, as the SDK answered every time, so no volume is asked for.
    - Step 4: the SDK's XMA decoder isn't set up (no "XMA Decoder" thread,
      no register mapping at `7FEA0000`); the hardware wait
      (`sub_8245D288`) answers at once (`hooks/xma.cpp`; its register
      writes are gone from `hooks/clock.cpp`). A6 with it (above).
    - `--log_xma` logs each sound's start, loop data, loop jumps and end.

    Left: the SDK's audio pieces in our output (A6 above: `AudioSystem`,
    two flags), and the guest memory the decoded samples live in (level 1:
    the voice's update is recompiled; with B7 they move to host memory).

B3. **Graphics boundary** (17 functions calling `Vd*`): ring buffer
    setup, the interrupt callback and vblank, swap, display mode and gamma
    queries. `src/native/gpu_link.cpp` already does the GPU's side; move
    it to the game's functions (read "Exclusive mode" and "Plan for
    milestone 6" in `docs/native-renderer.md`). Decide what happens to
    the MMIO range at `0x7FC80000` (`CP_RB_WPTR 0x714`, interrupt status
    `0x6544`, `0x6110`): show with logs that nothing reads it, or make
    its readers native. Then the GPU plugin and the SDK's graphics go.

    No comparison with the emulated GPU (the user's call, 2026-10-20):
    our renderer is checked against the 360 itself (the user's play,
    videos), and the emulated GPU hasn't been used to compare for some
    time. So B3's native functions replace the recompiled ones with no
    fallback to the plugin (as B1 did for input), and the plugin's build
    (`GPU_PLUGINS xenos`) and `--native_renderer=false` go with B3. `main`
    still has them. Worked on in branch `graphics-boundary`.

    The functions (read 2026-10-20 in `generated/`; the `Vd*` answers are
    the SDK's, `rexglue-sdk/src/kernel/xboxkrnl/xboxkrnl_video.cpp`). All
    are the game's graphics library (its Direct3D); r3 is the device
    unless said otherwise:

    - **Device start**: `sub_822AD298` (device init, called by
      `sub_8229DB38`, itself reached from the display set-up
      `sub_8258D568`) initializes two critical sections, calls
      `sub_822AD190`, reads a console setting (`ExGetXConfigSetting(3,
      10)` into +15164, B5), sets up the ring with `sub_8229F280`, the
      display mode with `sub_822ACE00`, retrains EDRAM (`sub_822B2388`)
      and checks `VdIsHSIOTrainingSucceeded` (1). `sub_822AD190` calls
      `VdInitializeEngines` (twice, with a `VdShutdownEngines` between;
      the second time with the PFP and ME microcode at `82016148` and
      `820165C8`), registers the title terminate notification at
      `82639098` and the interrupt callback `sub_8229DEA0`
      (`VdSetGraphicsInterruptCallback`). `sub_8229F280` (also called with
      no parameters by the device's release, to free) waits for the GPU,
      frees the old buffers, allocates the primary ring (default 32 KB),
      the command buffer (default 2 MB), the progress block (96 bytes,
      device +10768) and the callback block (32 bytes, +10772), calls
      `VdInitializeRingBuffer` and `VdEnableRingBufferRPtrWriteBack`
      (progress block +60; `docs/native-renderer.md` put it in
      `sub_822AD190`, wrongly), fills the ring fields (+13496..+13536),
      writes ME_INIT through `sub_8229DD70` and the scratch registers'
      set-up (SCRATCH_UMSK, SCRATCH_ADDR = the callback block), and kicks
      (`sub_8229F0E0`). Its import named `KiApcNormalRoutineNop`, called
      with the ME_INIT packet and 19, is probably
      `VdVerifyMEInitCommand` under a wrong name. Without the plugin the
      SDK ignores all three `Vd*` set-up calls, `VdInitializeEngines`
      returns 1 and never calls its callback `sub_822ACB48` (which writes
      GPU registers `7FC83214` and `7FC83408`).
    - **Device release**: `sub_822AD500` (called by `sub_8229D978`):
      waits, frees, calls `sub_822ACFB8` and `sub_822B58C0` (both reach
      `sub_822B4F10`), clears the interrupt callback, frees the ring
      (`sub_8229F280` with no parameters) and shuts the engines down.
      `sub_822ACB68` (unregister and `VdShutdownEngines`) has no callers
      in code: probably the terminate notification's routine, its
      address only in the image's data.
    - **Every frame**: Swap `sub_822A08A0` (called by `Renderer::Present`'s
      `sub_8258DC58`, the movie code's `sub_822A0E50`, the tiling worker
      `sub_822B69A8`; `sub_8209ACB8`, which calls it, has no direct callers) reserves 64
      words, calls `VdGetSystemCommandBuffer` and `VdSwap` (which writes a
      fetch constant and an XE_SWAP packet there; our kick ignores both),
      updates the display mode (`sub_822AD0B0`) and scaler
      (`sub_822B3FE0`) when they change, retrains EDRAM (`sub_822B2388`,
      `VdRetrainEDRAM` returns 0) and calls `sub_822B20D8` (which reaches
      the clock gating `sub_8229B848`: `VdEnableDisableClockGating`, and
      two REG_RMW packets). It writes the GPU register `7FC86110` only
      with device +10809 bit 1 set (the library's "GPU hung" path).
      `VdPersistDisplay` and `MmFreePhysicalMemory` only with +10809 bit
      4.
    - **Display mode and scaler**: `sub_822ACE00` (from device init)
      reads `VdQueryVideoMode` into +19988..+20000 and creates the front
      buffer and other surfaces; `sub_822AD0B0` sets `VdSetDisplayMode`
      (flags from the format and 720x480/576) and reads
      `VdGetCurrentDisplayInformation` into the same fields;
      `sub_822B2470` works out the scaler's rectangle and output height
      (`VdQueryVideoFlags`); `sub_822B2610` passes the front buffer and
      display sizes to `VdCallGraphicsNotificationRoutines` (returns 0);
      `sub_822B3A78` (`VdQueryVideoMode`, filter set-up through
      `sub_822B3798`) and `sub_822B3FE0` build the scaler's command
      buffer (`VdInitializeScalerCommandBuffer`, which the SDK fills with
      NOPs, and `RtlFillMemoryUlong`, B6's blocked import). The SDK's
      answers come from its `video_mode_*` settings.
    - **Gamma**: `sub_822B0910` (a 256-entry ramp) and `sub_822B0AB0`
      (a 128-entry piecewise-linear one) build a 1024-entry 10-bit curve
      from `VdGetCurrentDisplayGamma` (the SDK: always 2, BT.709) and run
      the ramp through it: the tables `--xbox_gamma` copied
      (`src/native/xbox_gamma_table.h`).
    - **`sub_822B4F10`**: writes a file with a 2048-byte header (display
      information and the gamma ramp in it; `sub_82295D50`, handles
      closed with `sub_82294E20`) and deletes a symbolic link
      (`ObDeleteSymbolicLink`). Called by the mounting `sub_822B58F0`,
      `sub_822B5C58` (from Swap), `sub_822ACFB8` and `sub_822B58C0`.
      Probably a debug capture; not read in detail. With its file calls
      it may belong to B4.

    Not needed already: no vblank interrupts and no GPU registers at
    `0x7FC80000` have been supplied since the plugin went (milestone 6),
    and boot, menus and races work. So B3 is mostly removing calls that do
    nothing today, not doing the GPU's work.

    Plan: (1) a temporary opt-in counter on these functions (which run,
    how often, from where, what each `Vd*` answers), an attract race and
    a session the user plays; (2) native as plain C++ behind adapters,
    smallest first: the display mode, flags, information and gamma (our
    own answers, the SDK's values; harness cases for the ramps); device
    start and release (the library's device fields kept, the `Vd*` calls
    dropped); the scaler (clearing `RtlFillMemoryUlong`); Swap, EDRAM
    retraining and clock gating; `XGetVideoMode`'s two callers (from B6).

    Counted (2026-10-20, `graphics-boundary`): a temporary wrapper on each
    (`src/hooks/graphics_boundary_log.cpp`, `--log_graphics_boundary`;
    removed when the natives replace them). An attract race (30 s): once
    at boot, device init, engines, ring set-up, display mode, set display
    mode (from the first Swap), the 256-entry gamma ramp, the scaler
    (rectangle twice, filters, buffer) and the notification, all at
    1280x720; Swap and EDRAM retraining once a frame (4,198 each, from
    `Renderer::Present`'s `8258DCA4`), Swap's device flags +10809 `04`
    (no "GPU hung" bit, so no `7FC86110` write). Never: clock gating, the
    piecewise-linear ramp, device release, the terminate routine,
    `sub_822B4F10`, `sub_822A0688` (reads `7FC8xxxx`) and the engines'
    callback. A session the user played (2026-10-20, 88 s: the front end,
    a race, the pause menu, closing the window) gave the same: every
    function counted once at boot or once a frame (6,530 Swaps, all from
    `8258DCA4`), the same seven never called. Closing the window
    hard-exits ("Title terminated; hard-exiting process"), so device
    release and the terminate routine don't run at quit either.

    The gamma ramps (done 2026-10-20, `graphics-boundary`): `sub_822B0910`
    and `sub_822B0AB0` are plain C++ (`src/display/gamma.cpp`) behind
    adapters (`src/hooks/gamma.cpp`), with the display's gamma ours
    (`src/display/display.h`: type 2, BT.709, power 2.2222223, the SDK's
    answer). Their helpers, the sRGB and BT.709 curves (`sub_822B07B0`,
    `sub_822B0860`), are plain C++ too and no longer called; `pow` is
    `std::pow`. The constants were logged from the image and written as
    hex floats (one, 1/1.055, doesn't round-trip as a decimal literal);
    the recompiler's fused multiply-subtract (`fma` in double, then
    rounded to float) is kept. The harness `gamma` (`--run_harnesses`)
    compares the two ramps (random, identity and climbing pairs, both
    directions, in place and not), the two curves (all 1,024 curve inputs,
    random values, the knees) and `pow` rounded to float (the curves'
    exponents and random ones): all the same. Only type 2 reaches the
    whole functions (the import can't be stood in for); the helpers'
    cases cover types 1 and 3. The harnesses' scratch area grew to 4 KB
    for the ramps. No game code calls `VdGetCurrentDisplayGamma` any more.

    Kept, though not needed to draw today (the user's call, 2026-10-20):
    `--xbox_gamma` applies fixed tables captured from the game
    (`src/native/xbox_gamma_table.h`), not the ramps' output, which only
    reaches the display's gamma registers. Keeping the native ramps leaves
    room for `--xbox_gamma` to follow the game's own ramp later. So the
    set display mode `sub_822AD0B0`, which calls them, isn't dropped
    either.

    The rule for the rest (the user's, 2026-10-20): we're working toward
    native graphics code, and the Vulkan renderer is it. What the renderer
    needs from a function is kept (made native without its `Vd*` calls);
    what it doesn't need is dropped (status Dropped in
    `docs/functions/functions.md`, with the reason in its row).

    Dropped (2026-10-20, `src/hooks/graphics_drops.cpp`, no fallback
    flag): the engines' set-up `sub_822AD190` (returns 1; only `Vd*`
    calls the SDK ignores and the terminate notification), EDRAM
    retraining `sub_822B2388` (once a frame; only a lock, with the SDK's
    `VdRetrainEDRAM` returning 0), and the display scaler's buffer
    `sub_822B3FE0` and notification `sub_822B2610` (packets for display
    registers our renderer doesn't read). That leaves `sub_822B2470` and
    `sub_822B3A78` with no callers. Checked by an attract race (the same
    device fields at boot and Swap, no errors, the dumped frame drawn
    right) and the user watching it. No game code calls
    `VdInitializeEngines`, `VdRetrainEDRAM`, `VdRetrainEDRAMWorker`,
    `VdInitializeScalerCommandBuffer`, `VdCallGraphicsNotificationRoutines`
    or `RtlFillMemoryUlong` (B6's blocked import) any more.

    Left (2026-10-20): the functions the renderer needs, still calling
    `Vd*`: device init (`VdIsHSIOTrainingSucceeded`), the ring set-up
    (`VdInitializeRingBuffer`, `VdEnableRingBufferRPtrWriteBack`,
    `VdSetSystemCommandBufferGpuIdentifierAddress`), the display mode
    (`VdQueryVideoMode`), the set display mode (`VdSetDisplayMode`,
    `VdGetCurrentDisplayInformation`) and Swap (`VdSwap`,
    `VdGetSystemCommandBuffer`, `VdPersistDisplay`). Each gets a native
    version keeping only what the renderer uses. All of them are native
    now (Swap last, below). The functions that still call `Vd*` in
    `generated/` are the never-called ones that follow,
    `Device::InitEngines` and the two no longer called, so none has been
    seen to run. The counter (`--log_graphics_boundary`) was removed once
    the user's session (below) had been read. The never-called functions
    (clock gating, device release, the terminate routine, `sub_822B4F10`, `sub_822A0688`, the
    engines' callback) keep a blank status; `sub_822B2470` and
    `sub_822B3A78`, no longer called since their callers were dropped, are
    Dropped. None has an override (the counter's wrappers went), and they
    still name `Vd*` imports in `generated/` until it goes.

    Made native (2026-10-20, `src/hooks/graphics_device.cpp`; level 1:
    the library's own steps stay guest calls, the `Vd*` calls are gone):
    - `Device::SetUpDisplay` (`sub_822ACE00`): the display mode is ours
      (`src/display/display.h`, 1280x720) instead of `VdQueryVideoMode`.
      The game passes +60 set, so no front buffer is made here; the
      render target and depth surface are.
    - `Device::Init` (`sub_822AD298`): without `VdIsHSIOTrainingSucceeded`
      (the "training failed" flag at `829537BC` stays 0), the engines'
      set-up (so `sub_822AD190` has no callers and its drop went) and the
      EDRAM retraining (dropped), and without handing two fields to the
      kernel's debug monitor data (`820007F0` points to 0: no debugger).
      Kept: storing the device in `VdGlobalDevice` (`820007B0`, other
      game code may read it), the console setting read (B5) and the
      critical sections (the core). The counter's rate comes from
      `src/helpers/clock.h`.
    - `Device::SetUpRing` (`sub_8229F280`): without its five kernel calls
      and the ME_INIT packet in the primary ring, which nothing reads. The
      game passes its own ring and command buffer (nothing is allocated
      for them). The scratch registers' set-up stays: our kick needs it
      for the boot callback.
    - `Device::SetDisplayMode` (`sub_822AD0B0`): the gamma ramp set as
      before; the display fields from our display mode instead of
      `VdGetCurrentDisplayInformation`; `VdSetDisplayMode` gone.

    Checked by the counter's log of the device's fields (display, surfaces,
    flags, ring, command buffer, progress block, fences) after each step
    and at the first Swaps, against the recompiled versions: the same, but
    the ring's write position (0, no ME_INIT). For the ring set-up the
    original ran in the same build behind a temporary switch. Frames dumped
    in attract races look right (frames always differ between runs, by
    timing), the boot callback is still made, and all harnesses match.

    Swap, step by step (2026-10-20, `graphics-boundary`; `sub_822A08A0`
    read in `generated/`, then counted: the Swap wrapper in
    `src/hooks/graphics_boundary_log.cpp` counts, at entry, the state each
    branch tests, by caller, and logs the fields before and after at its
    first calls and powers of two). Attract races (30 s, 5,120 Swaps): all
    from `Renderer::Present`'s `8258DCA4`, r5 0, device +20440 bit 0
    clear. In the order Swap runs them:

    1. **Copies the front buffer's 52-byte header to +13444. Needed.**
       `sub_822A0718` (from `VideoOutput::WaitForVblank` before every
       Swap; it sets +10809 bit 5, which Swap clears) reads its address
       word (+13476): only once it's non-zero (from the second frame) does
       it write its packets, among them scratch register 1 = 1, which our
       kick writes back to callback block +4. The tiling worker
       `sub_822B69A8` would show the copy again (Swap with +13444) when
       +10810 bit 2 is set, but no code sets that bit.
    2. **GPU timestamps** (`sub_822B1F68` before and after, +20076 += 2)
       when +20080 is set: an EVENT_WRITE_SHD with initiator bit 31 (the
       GPU's swap counter) into progress block +64..+92. Never: +20080 is
       set only by `sub_822B1CB0`, the library's debugger command handler,
       which `sub_822B1E70` hands to the debug monitor only when
       `820007B4`/`820007F0` point somewhere (they don't). Not needed.
    3. **Ends the segment when `829537C0` is set.** Never: no code writes
       it (0 in every run). Not needed.
    4. **`sub_822B5C58(device, front buffer)`: the capture.** Tail-calls
       the mounting `sub_822B58F0` or `sub_822B5208` when the byte at
       `82953E1C` is set, `sub_822B4F10` when device +20424 bit 3 is;
       otherwise returns. Neither is written by code (0 in every run).
       Not needed (skipped behind a temporary switch: the same device
       fields, frames right).
    5. **`VdGetSystemCommandBuffer` and `VdSwap`**, unless +19980 (an
       object only the capture's state machine `sub_822B1788` makes; 0 in
       every run) is set. The SDK zeroes the 148-byte buffer, so +20120 is
       stored as 0 and +20432 never written; `VdSwap` fills the 64 words
       reserved (`sub_8229F148`) with a fetch constant (register `0x4800`,
       above the 0x2000 our kick keeps), XE_SWAP (ignored) and NOPs.
       +20120's only reader is the debugger's statistics query
       `sub_822B1088`, +20432 has none. Not needed, nor the reservation.
    6. **`Device::SetDisplayMode`** when +10809 bit 4 is set: once, at
       the first Swap (`Device::SetUpDisplay` sets the bit). Kept (native
       already, for the gamma ramps).
    7. **The parameters (r5)**: copied to +13284 and `sub_822B42D0` called
       when they differ from it. `Renderer::Present`, the tiling worker and
       `sub_8209ACB8` pass 0; the movies' `sub_822A0E50` passes +13284
       itself. Never runs.
    8. **The scaler.** With the zeroed buffer (its +8 is 0) the
       "nothing from the kernel" branch: when +10810 bit 5 is set (once,
       the first Swap), clears +13312 (56 bytes), clears the bit and calls
       `Device::SetUpScaler` (Dropped), then writes 10 words (register
       `0x1838` = 0, a WAIT_REG_MEM on `0x1973`, `0x1852` = 1: display
       registers). When +10811 bit 7 is set, `sub_822A05A8` writes the
       display's gamma packets (REG_RMW on `0x1841`, `0x1930`, a ramp
       through `sub_822B0D18` or `sub_822B0C38`); no code sets that bit
       (`EndTiling` clears bit 6), never seen. The packets are display
       registers our renderer doesn't read. Not needed, but clearing the
       flags and +13312 (library state).
    9. **The front buffer register.** The game flips between two front
       buffers (`FF7F7000`, `FF45F000`), so every Swap finds +20004 changed
       and writes register `0x1844` (the address) into the command buffer;
       our kick only keeps it in its register table, which nothing reads
       for `0x1844`. The GPU register `7FC86110` is written as well only
       with +10809 bit 1 ("GPU hung", set by `sub_822ACAA8` after a timed
       out wait, or by the segment end when the "training failed" flag
       `829537BC` is set; neither in exclusive mode). +20004 has no other
       reader in the library. Not needed.
    10. **Display persistence** with +10809 bit 4 (the first Swap): a
        REG_RMW on `0x1E4E`, `VdPersistDisplay` (the SDK allocates 64
        bytes) and `MmFreePhysicalMemory` (frees them), then the bit
        cleared. Not needed, but clearing the bit.
    11. **Ends the segment** (`sub_8229EF48`: the kick our renderer reads).
        Needed.
    12. `RetrainEdram`: Dropped already.
    13. **`sub_8229EC00(device, +13512 >> 2)`: the next segment and the
        frame throttle.** Takes the next segment of the command buffer
        (`sub_8229E100`, or through the object at +13160), sets the write
        position and its limits (+40, +44, +48 = +44 − 160) and the
        segment's fields (+13528, +13516, +13532), then waits
        (`sub_8229E908`) for progress block +0 to reach the fence of the
        last Swap (+13540; with +20440 bit 0, the one before, +13544) and
        zeroes it. In exclusive mode the fence is always passed (never
        ahead in the runs). Needed: the next frame's commands go into that
        segment.
    14. **Clears +10809 bit 5 and records the fences**: +13540 = the
        command position before the kick, +13544 = the old +13540 (0 with
        +20440 bit 0 clear, as `sub_8229EC00` has just zeroed it). Only
        Swap and `sub_8229EC00` read them. Needed (the throttle's fences).
    15. **`sub_822B20D8`: frame statistics.** The frame counter (+20024 +=
        1), the time since the last Swap from the timebase (+20028, with
        +20032/+20036), +20048/+20056 moved to +20064/+20068 and zeroed,
        the debugger's requests (+15168 bits through `820007F0`), the GPU
        performance counters (+20104 = 1 or 2, set only by `sub_822B0F70`,
        itself only from the debugger's query `sub_822B1088`; clock gating
        on that path), and `sub_822B1788` (the capture's state machine at
        `82953D10`, 0 in every run; otherwise it hands the frame count to
        the debugger through `820007B4`). Readers of what it leaves: the
        debugger's query and the "GPU hung" handler. Not needed (skipped
        behind the temporary switch: only +20024 and +20028 differ, 0
        instead of counting; frames right).
    16. **`sub_822A3B90(device, 0)`**: the binning packets (opcodes
        `0x60`-`0x63` from +12448/+12452), `sub_8229B7D8` (WAIT_UNTIL and
        register `0xF01` from +10788, with the bits it already holds), the
        screen scissor `sub_822A0F80` (from +10308/+10312; per tile when
        tiling), then **all ones in the dirty masks at device +0..+39**.
        The renderer takes those masks at each draw (`MarkDeviceDirty` in
        `src/native/state.h`), so the next frame's first draw copies every
        register block. Needed (a library step without `Vd*` calls, also
        called by `Device::Init`, `sub_825BED08` and `EndTiling`
        `sub_825BF200`).

    Seen every frame, then: 1, 4, 5, 9, 11, 13-16; once: 6, 8, 10. Never:
    2, 3, 7, the gamma half of 8, the "GPU hung" write. What the
    renderer needs: 1, 11, 13, 14, 16, and 6 (kept for the gamma ramps).

    Proposed native Swap (level 1, in `src/hooks/graphics_device.cpp`):
    copy the 52 bytes; `Device::SetDisplayMode` when +10809 bit 4 is set,
    clearing it; when +10810 bit 5 is set, clear +13312 and the bit; clear
    +10811 bit 7; then the guest calls `sub_8229EF48`, `sub_8229EC00`
    (+13512 >> 2), clear +10809 bit 5, the fences, and `sub_822A3B90(0)`.
    Left out: the timestamps, `829537C0`'s segment end, the capture, the
    `Vd*` calls and their reservation (+20120, +20432), the parameters'
    compare, the scaler, gamma and front buffer packets (+20004), the
    `7FC86110` write, the display persistence, `RetrainEdram` and the
    frame statistics. Swap is the only caller of `sub_822B5C58`,
    `sub_822B20D8`, `sub_822B1F68` and `sub_822A05A8`, which become
    Dropped (and `sub_822B1788`, `sub_822B5208`, `sub_822B58F0`, called
    only from those, are no longer called). Checked as the other device
    functions were: the counter's fields at the first Swaps and powers of
    two the same as the recompiled Swap's, but for those left out
    (+20004, +20024, +20028, and the write position, 256 bytes less a
    frame), attract races with frame dumps, all harnesses, and a session
    the user plays (front end, a race, the pause menu, a movie, loading
    screens).

    Swap made native (2026-10-20, `src/hooks/graphics_device.cpp`, level 1
    as proposed). Swap's listing, read again against the proposal, matched
    it. Its r3 comes back as `sub_822A3B90` leaves it (the last guest call,
    as in the original): `Renderer::Present` and `sub_8209ACB8` ignore it,
    and the movies' `sub_822A0E50` returns it to its own callers. The
    52-byte copy stays a call to the game's memcpy (wrapped for the
    memory mirrors). For the check, the counter's Swap logging moved into
    functions the native Swap called; the counter went after the user's
    session. No game code calls `VdGetSystemCommandBuffer`, `VdSwap` or
    `VdPersistDisplay` any more (`MmFreePhysicalMemory` still has two
    other callers, `sub_822965C8` and `sub_822B4680`).

    Checked against the recompiled Swap in the same build (a temporary
    switch, removed), 30 s attract races with `--log_graphics_boundary`:
    at the first Swaps and powers of two, the same flags (+10808..+10811),
    display, surfaces, ring and command buffer fields, fences (+13544
    always 0, +13540 the position before the kick, progress +0 two behind:
    the fence always passed), dirty masks (all ones) and branch counts
    (5,120 Swaps, all from `8258DCA4`; display mode and scaler once, front
    buffer every frame, the rest never). What differs: +20004 (0, no
    longer set), +20024 and +20028 (0, no frame statistics), and the
    segments: the original reserved `VdSwap`'s 64 words before
    `Device::SetDisplayMode` wrote the gamma ramp's packets, so at the
    first Swap the ramp no longer fit in the segment (3,760 bytes left)
    and one more segment was ended. The native one ends one fewer there:
    command positions 2 lower and segments 4,160 bytes earlier from then
    on, until the runs' own timing takes over. +13520 (probably the
    command buffer's wrap count) is the same until the race loads and
    then follows each run's command positions. The dumped frames look
    right, all harnesses match, there are no error lines, and the boot
    callback is still made. The kick's summary is the same in both (no
    packets unhandled, 0 scratch write-backs, 0 callbacks).

    A session the user played on the native Swap (2026-10-20; front end,
    a race, the pause menu, a movie, loading screens): the game looked
    fine. 9,330 Swaps, all from `Renderer::Present`'s `8258DCA4` with r5
    0; the display mode and scaler branches once, the front buffer every
    Swap, the others never (the same as attract races; the movies'
    `sub_822A0E50` and `sub_8209ACB8` never called Swap). No error lines,
    no packets unhandled. Unlike the attract races, some of the kick's
    10 s summaries count scratch write-backs (240-604, with 125 and 147
    callbacks in two of them; 0 in the race's), so scratch register 1's
    write-back from `sub_822A0718` happens in some phases only. Not looked
    into yet.

    The emulated GPU left out of our build (2026-10-20,
    `graphics-boundary`): the game no longer builds against the plugin or
    copies it next to the executable (`rexglue_setup_target(itc)` without
    `GPU_PLUGINS xenos`; the SDK still builds it in its own tree, which
    goes with the core). `--native_renderer` and
    `--native_renderer_exclusive` are gone, with the paths they guarded:
    the kick (`sub_8229E3F0`) and the deferred kick (`sub_8229E848`) no
    longer fall back to the recompiled ones that handed the GPU its work,
    the SDK's window is always hidden, and the renderer's hooks always
    run. The scripts and the standard command lose the two flags. Checked
    by an attract race: no error lines, the boot callback made, nothing
    unhandled by the kick, the dumped frame right, all harnesses the same
    (one run's `GetLocalTime` case differed by a second, run as the
    second ticked over; the next run matched).

    `XGetVideoMode`'s two callers made native (2026-10-20,
    `src/hooks/video_mode.cpp`, level 1; from B6). Both run once at boot.
    `sub_82482B18` (from `sub_82483130`) read only `is_widescreen` (+12),
    to pick 16/9 or 4/3 (the game's constants at `8204D300`, `8204D2FC`)
    over the back buffer's aspect; ours takes `display::kWidescreen` (1 at
    1280x720) and stores the same +132/+136/+140 (1280, 720, 1.0), then
    makes the same index buffer (indices 0-5), vertex buffer and vertex
    declaration and calls the object's vtable +96. The display set-up
    `sub_8258D568` (from `Renderer::Init`, r6 = 2) had the answer written
    into its object (the video output, `Renderer + 64`, +152..+199) and
    never read it back; nothing else in the video output's or the
    renderer's functions reads those bytes either. Shown by logs, not
    only by search: a temporary switch overwrote them with 0xCD after the
    set-up, and attract races (the same log, harnesses and frame) and a
    session the user played (front end, race, pause menu, movie, loading
    screens: fine, no error lines) didn't change. Ours does the rest the
    same (the parameters, the two physical blocks, the device, the two
    front buffers, the viewport, the states, the vblank callback, the safe
    area) and leaves the 48 bytes zeroed. Checked against the recompiled
    ones in the same build (a temporary switch, removed): attract races
    with the objects logged word by word (the video output's +16..+311,
    the device's states and +15144, the other object's fields, its index
    data, r3) the same but for the answer's bytes; the dumped frames, the
    harnesses, no error lines, the boot callback made. The user judged a
    further session unneeded (both run once at boot, before anything an
    attract race skips). No recompiled code calls `XGetVideoMode` now.

    Met on the way, for the interrupt callback below: the set-up gives the
    device its vblank callback (`sub_8229DA20` stores `sub_8258D4D8` at
    +15144), which counts vblanks (`8296BD2C`, `8296BD30`) and signals an
    event (`8296BD34`); only `sub_8229DEA0` calls it, on vblank
    interrupts, which we don't make. The movie code's `sub_824E1750` also
    reads +15144.

    The GPU registers at `0x7FC80000` (2026-10-20): nothing answers them
    now. Only the SDK's graphics system registered that range's handler
    (`rexglue-sdk/src/graphics/graphics_system.cpp`), and it exists only
    with a GPU plugin; without a handler the recompiled code's
    `REX_MM_LOAD_U32` returns an uninitialized value and its stores are
    dropped. (The SDK's vblank thread lived there too: there are no
    vblanks at all now.) So every function touching them was looked for
    and counted. In `generated/`, the `REX_MM_*` accesses are in the kick
    `sub_8229E3F0` and Swap `sub_822A08A0` (native, without them),
    `sub_822A0688` and the engines' callback `sub_822ACB48` (never called),
    the interrupt callback `sub_8229DEA0` (interrupt status `0x6544`),
    `sub_8229DFF8` (a store through a pointer; its only caller is the
    deferred kick, whose native version doesn't call it), and
    `sub_8245C638`, `sub_8245D138`, `sub_8245D208`, `sub_8245D288`,
    `sub_8245D350`, `sub_8245D4D0`: those are the XMA decoder's registers
    at `0x7FEA18xx` (audio), which the SDK's XMA decoder, kept, still
    answers. One more reads them with a plain load, which the macro
    search misses: `sub_822AC590` (register r3), with `sub_822AC5A0` (its
    write); both are reached only through pointers in `sub_822AC918`,
    from the "GPU hung" handler `sub_822ACAA8`. Nothing else builds
    `0x7FC80000` or the register-index form (`+0x1FF20000`). A temporary
    counter on all of them (removed): none called in a 30 s attract race
    or in a session the user played (front end, a race, the pause menu,
    loading screens: fine, no error lines, nothing unhandled by the
    kick), `sub_8229DEA0` included. So nothing reads or writes the GPU
    registers, and no code changes were needed; the rows say never
    called.

    The interrupt callback and vblank (2026-10-20): `sub_8229DEA0` is no
    longer called, and its override (which only logged, then called it)
    is removed. Nothing registers it: only the engines' set-up
    `sub_822AD190` did (`VdSetGraphicsInterruptCallback`), and native
    `Device::Init` leaves that out; our kick's `Interrupt()` calls the
    callback block's functions itself; and the SDK's vblank thread went
    with the GPU plugin, so there are no vblanks at all. The register
    item's counter showed it: never called in an attract race or the
    user's session. What vblanks fed is unused too: the device's vblank
    count (+15148) is read only by `sub_8229DEA0` and `sub_822A0688`
    (never called); the display set-up's vblank callback `sub_8258D4D8`
    (device +15144) is called only by `sub_8229DEA0`; its counts
    (`8296BD2C`, `8296BD30`) and event (`8296BD34`) were read and waited
    on only by the original `VideoOutput::WaitForVblank` (`sub_8258DAA8`),
    which our override replaced (it makes the device call before the
    wait, not the wait); the event is otherwise only made at boot
    (`sub_825DA470`) and closed at exit (`sub_825DB2C0`). Four movie
    decoder functions read +15148 of their object too (`sub_825169A0`,
    `sub_82533010`, `sub_8253DEB0`, `sub_8256D470`, comparing it with -1):
    a temporary counter (removed) showed the boot movie's `sub_825169A0`
    on its own object (`40B0AB40`, +15148 = -1), not the device
    (`40021B00`); the other three are its variants, reached the same way.
    Likewise the movie player's +15144 and +15156 (`sub_824E1750` and
    others) are its own object's, not the device's: the lead in
    `native-renderer.md` that the movie code sets a vblank callback on the
    device was wrong (corrected there).

    One consequence, noted and not changed (the user's call):
    `sub_822A0718` writes scratch register 1 = 1 every frame, which our
    kick writes back to callback block +4; on the 360 the vblank
    countdown in `sub_8229DEA0` (+15156) cleared it, and that never runs
    now. It fits the scratch write-backs seen outside races in the user's
    sessions (452-604 an interval) and their absence in races.

    B3 done (2026-10-20, merged from `graphics-boundary`): no code that
    runs calls a `Vd*` import or `XGetVideoMode` any more. Each recompiled
    caller is overridden natively without it (device start, the ring,
    Swap, the display mode, the display set-up, the gamma ramps), Dropped
    (engines, EDRAM retraining, the scaler, notifications), or never
    called (clock gating, `Device::Release`, the debug capture), and our
    code calls none. The GPU plugin isn't built or loaded, nothing reads
    the GPU registers, and nothing needs vblanks. The SDK's graphics code
    (`rexglue-sdk/src/graphics/`, the `Vd*` exports) is still compiled
    into `librexruntime`, unused; like all SDK code it's deleted with the
    SDK (E4), not before.

    For later (the user's call, 2026-10-20): return to the scratch
    register 1 lead above. Nothing seen so far says it needs looking
    into now (the game behaves in the user's sessions, with nothing
    unhandled by the kick).

B4. **Files and saves** (22 functions; the game's `CreateFile`
    `sub_82295508` is the only caller of `NtCreateFile`): the game's file
    functions on host files under `--game_data_root` (log the paths with
    `--log_file_opens`, `src/hooks/create_file.cpp`), with handles that
    are ours, used only by these functions. Check every user of a file
    handle is native (waits on async reads, `NtClose`). Saves
    (`XamContent*`): read and write where the SDK puts them, byte for
    byte; test with a save from the last release before and after. Then
    the SDK's VFS goes. (Superseded for saves by the user's call below:
    our own `saves` folder, no migration.)

    Worked on in branch `files-and-saves`, merged into
    `rexglue-removal-project` 2026-10-21. Surveyed 2026-10-21. The
    functions are in `docs/functions/files-and-saves.md`, the save slot's
    fields in `docs/known-data.md`.

    **The functions.** The boundary pipeline filtered to the file and
    save imports gives 23 functions (22, and the debug capture's
    `sub_822B4F10`, B3's). Nearly all are the XDK's Win32-style wrappers:
    CreateFile `sub_82295508` (the only `NtCreateFile` caller), ReadFile,
    ReadFileEx (with its APC `sub_822953E8`), WriteFile, SetFilePointer,
    GetFileSize and GetFileSizeEx, SetEndOfFile, GetFileTime,
    FlushFileBuffers, GetFileAttributes, GetDiskFreeSpaceEx and DeleteFile,
    and the XContent wrappers (create, delete, close, thumbnail, creator,
    enumerator, device data), the device selector and XEnumerate. Above
    them: the game's file layer (`File::OpenRead` `sub_8258C688`,
    `File::Read`, `File::Seek`, `File::OpenWrite`, `File::Write`; the
    handle at +4, the position at +8), the C runtime's low-level I/O
    (`_read`, `_write`, `_lseek`, `_commit`), the archive streaming
    (`sub_8209E210` opens, `sub_8209DD00` reads with ReadFileEx), the
    movie player (`MovieSource::OpenFile` `sub_82487C48`, first named
    `MoviePlayer::OpenFile`, reads in `sub_82488598`), and the save slots (`SaveSlot::*`, `sub_820A45C0` to
    `sub_820A5E68`: a 1,000-byte slot, each step started through an
    overlapped and polled with XGetOverlappedResult `sub_82295C98`). Two
    old guesses were wrong: `sub_82295C10` is XContentGetDeviceData (not
    XGetOverlappedResult), `sub_825B9F20` is DeleteFile (not
    SetFileAttributes).

    **What runs** (a temporary counter, `--log_files_and_saves`,
    `src/hooks/files_and_saves_log.cpp`, with `--log_file_opens`; removed
    after the survey). A 30 s attract race and a session the user played
    (load the save, save, load a race; 2026-10-21):

    - Files opened, the whole session: 15. The two archives
      (`game:\DATA\BUILD\TXR\BUILD.DAT` and `BUILD_CRS.DAT`, opened once
      by `sub_8209E210` and streamed with ReadFileEx: 1,134 reads, each
      done at once under the SDK, the APC called straight after with the
      routine `sub_8209D808`), their TOCs, the audio banks
      (`txr_xact.xgs`, `SB_SYSTEM_SE.XWB`/`.XSB`, `CAR_SE.XWB` three
      times), the boot movie (`UBI_LOGO.wmv`, twice), `DEBUG\DEBFONT.XPR`,
      and the save (`TXR360:\TXR_DATA_1`, once read, once written). The
      file layer reads each whole (open, GetFileSize, one ReadFile).
    - The graphics library asks GetFileAttributes for
      `ShaderDumpxe:\CompareBackEnds` (fails, -1) through `sub_822B8868`,
      14 times: a debug check, can be answered "not there".
    - Never called in play: GetDiskFreeSpaceEx, SetEndOfFile, DeleteFile,
      the debug capture's mounting (`sub_822B58F0`, `sub_822B4760`),
      GetFileTime and its caller, `_read`, `_write`, `_lseek`, `_commit`,
      FlushFileBuffers, XContentDelete, XContentSetThumbnail,
      XContentGetCreator, `File::OpenWrite`, `File::Write`.
    - The save, at boot: the device selector (997, then device 1 through
      the overlapped), XContentGetDeviceData (type 1, 20 GB, 3 GB free),
      the enumerator (`0x7850` bytes: 100 items of 308) and XEnumerate
      (997; one save found: "GAME DATA   2026/10/20 23:28",
      `SystemData.bin`), XContentCreate with open existing on root
      `TXR360` and XContentClose (the check that it opens), then
      `SaveSlot::Create` (mode 0: open existing), `SaveSlot::ReadFile`
      (`TXR360:\TXR_DATA_1`, 129,400 bytes) and XContentClose. The
      front end (`sub_8228FC50`) works out the save's size
      (XContentCalculateSize of 129,400: 188,416) and looks for
      `SystemData.bin` among the enumerated saves.
    - The save, saving: the selector, enumerate and open check again,
      then `SaveSlot::Create` (mode 1, create always, display name
      "GAME DATA   " and the date), `SaveSlot::WriteFile` (the same path
      and size; WriteFile done at once) and XContentClose. No thumbnail
      set. Loading a race enumerated the saves once more.
    - The SDK keeps it as
      `<user folder>/B13EBABEBABEBABE/555307EE/00000001/SystemData.bin/TXR_DATA_1`
      (`~/.local/share/itc/...` on Linux) with a header file for the
      display name.

    **The user's calls (2026-10-21):** the save goes in a `saves` folder
    next to the executable, laid out our own way (the SDK's isn't a
    guide), and old saves in the SDK's folder aren't migrated or read.

    **Proposed for the saves** (plug in the data): cut at the save slot
    steps. The device selector, device data and space check answered at
    once (one device, room enough, no UI); the enumeration one item
    ("GAME DATA   " and the file's date, `SystemData.bin`) when
    `saves/TXR_DATA_1` exists, else none; open, create, close and the
    thumbnail done with nothing to do; the read and write steps plain
    host file I/O on `saves/TXR_DATA_1` (written to a temporary file and
    renamed over the old one), each step's overlapped marked done so the
    game's polls pass. The XContent wrappers and XGetOverlappedResult are
    then never called. Then the game data files (`game:\` under
    `--game_data_root`, through the file layer and the archive
    streaming), the debug checks, and what's never called.

    **Step 1, the saves: done (2026-10-21, `files-and-saves`).** The save
    is plain C++ in `src/saves/saves.{h,cpp}` (`std::filesystem` and
    streams, the same on Linux and Windows): files in `saves/` next to
    the executable (found by `helpers::ExecutableFolder`, which
    `native/disk_cache.cpp` now uses too), written to a temporary file
    and renamed over the old one. `src/hooks/saves.cpp` has the adapters
    over the 12 save slot steps that called imports (CheckDevice,
    Show/WaitDeviceSelector, Enumerate and its wait `sub_820A4A88`,
    OpenItem and its wait, Create, ReadFile, WriteFile, SetThumbnail,
    Delete): each finishes at once and marks its overlapped done, so the
    recompiled waits pass on their next poll without a kernel call; the
    read and write steps make no file or event handles (+496 = -1, +492
    = 0), so the close step's wait has none to close. One device (1),
    the SDK's free bytes (3 GB), one save (`SystemData.bin` holding
    `TXR_DATA_1`, its display name "GAME DATA   " and the file's date in
    UTC). XContentClose, called inline by the save and load steps, is
    Dropped (its overlapped done, 997). XShowDeviceSelectorUI, the
    XContent wrappers and XEnumerate have no callers left (Dropped).
    No fallback flag (as for B3, B5 and B6). Checked: the harness
    (`saves`: CheckDevice, WaitDeviceSelector and the enumeration's
    wait, 13 cases, 0 differ; the other steps would make the SDK create
    or close content in its own folder), an attract race, and the user's
    play: a fresh start with no save, a save (129,400 bytes written), a
    restart that loaded the progress, and a save over it (4 bytes
    changed, at 6086-6092). The temporary counter no longer wraps the
    steps made native.

    **Step 2, the game's data files: surveyed (2026-10-21); done (below).**
    The user's direction: native file loading code at the level where the
    game asks for its data, not a reimplementation of the Xbox's file
    calls (that would emulate the 360); after it, the next level up is
    the functions that read the data (the load callbacks and parsers).
    Names and layouts confirmed with SBX-Recomp's `docs/functions.md`
    (the Japanese build's recomp, beside this repo; its
    `docs/itc_to_sbx_functions.tsv` maps the addresses).

    Where the game asks for files (a trace in the temporary counter, a
    40 s attract race, 2026-10-21):

    - `Archive::Load` (`sub_8209DE48`, about 35 callers, all on the main
      thread): r3 an asset id (from 23011 in `BUILD_CRS`), r4 the
      destination, r5 a callback, r6 its context. It reads the id's TOC
      entry (start sector, stored size, decompressed size), checks
      `Archive::FindLooseFile` (`sub_8209D8D8`: a path for ids served by
      loose files, such as the audio banks), and queues a 40-byte request
      (`sub_8209DC18`); `sub_8209DB20` cancels one still waiting. 476
      requests: 395 at load (120 MB unpacked in a second), then about 10
      every few seconds during the race (3-10 MB a second, from
      `sub_820D08D8`, `sub_820F2454`, `sub_820DB674`/`DB69C`,
      `sub_820D0840`): loads have to stay asynchronous (a synchronous load
      would stall the main thread 10-30 ms a burst). Latency today: median
      12 ms, 90th percentile 241 ms.
    - The loader thread (`sub_8209E118`) does the rest: `sub_8209D490`
      gives each request a state and a target (+56 the byte offset, +60
      sectors, +68/+72 the stored and unpacked sizes, +76 a fresh buffer,
      the caller's, or half of the 10 MB staging ring for a compressed
      entry, its destination then at +88). State 5 (365 + 96 + 12 seen):
      an asynchronous ReadFileEx from `BUILD.DAT` or `BUILD_CRS.DAT`
      (`sub_8209DD00`), then an alertable wait until the APC's routine
      (`sub_8209D808`) releases the loop's lock (`826A(-21912)`); state 7
      (3: the audio banks `SB_SYSTEM_SE.xwb`/`.xsb`, `car_se.xwb`): the
      loose file read through the file class (`sub_8209DFA8`), which
      releases the lock itself; states 1-2: no read, the loop releases
      it. While a read is in flight the previous compressed entry is
      unpacked (`Ucl::Unpack` `sub_82294610`, blocks by `sub_822947F8`,
      about 3 ms a MB); `sub_8209D688` then calls the callback (flag
      0x2000) on the loader thread.
    - `Archive::Init` (`sub_8209E210`, once at boot) allocates the TOC
      and staging buffers, reads `BUILD.TOC` and `BUILD_CRS.TOC` whole
      through the file class, and opens the two `.DAT`s with CreateFile
      (handles at `82600A9C`, `82600AA0`).
    - The game's file class (`File::OpenRead` `sub_8258C688`, `File::Read`
      `sub_8258C820`, `File::Seek` `sub_8258C768`, `File::Close`
      `sub_8258C718`; the object: +4 the handle, -1 closed, +8 the
      position, +12 the size, which `sub_822B80D8` returns): the only
      whole-file loads in play go through it: the TOCs, `DEBFONT.XPR`
      (`sub_82090CC0`), `TXR_XACT.xgs` (opened by `sub_82118638`, read by
      the audio engine's start-up `sub_8259D1F0` through the object's
      vtable) and the loose requests. `File::OpenWrite` and `File::Write`
      are never called.
    - The movies (`UBI_LOGO.wmv`, `OPENING.wmv`) have their own reads in
      the movie player (`MovieSource::OpenFile`, `sub_82488598`): left
      for when the movie decoder is looked at.

    Proposed (each part plain C++ behind thin adapters):

    1. A game data module (`src/game_data/`): reads files and byte
       ranges under the data root (the executable's folder, or
       `--game_data_root`, the flag that already exists) from the game's
       relative paths (`DATA\AUDIO\CAR_SE.XWB`; a `game:\` prefix in the
       game's strings is dropped: no mount points or devices, the user's
       call), matching names whatever their case on Linux (the game asks
       for `CAR_SE.XWB`, the file is `car_se.xwb`); the two archives kept
       open.
    2. The file class native: open (the size from the host file), read
       at the position, seek, close, with handles of our own (a range the
       SDK's `F8000000` ones can't reach); `File::Close` hands any other
       handle to the recompiled CloseHandle.
    3. The loader thread's loop native (level 1: it calls the game's
       request queue, `Ucl::Unpack` and the finish with its callbacks, as
       now): state 5's bytes copied from the archive into +76 directly,
       then the lock released as the APC's routine did, with no wait;
       state 7 read directly too. No ReadFileEx, APC or alertable wait.
    4. `Archive::Init` native (level 1): the TOCs read straight into
       their buffers, the archives opened by the game data module, no
       CreateFile; the allocations, locks and the thread's start called
       as now. `Archive::Shutdown` (`sub_8209D3B0`) closes nothing of the
       SDK's then.
    5. Checks: harnesses for the file class against the recompiled one
       on real files (bytes, positions, sizes, r3) and for the archive
       reads (the same bytes as a ReadFileEx of the same request); an
       attract race with the trace (requests, latency); the user's play.

    Afterwards ReadFileEx and its APC, ReadFile, GetFileSize(Ex),
    SetFilePointer and CreateFile have no callers in play but the movie
    player's; the SDK's VFS serves only the movies and the boot.

    **Step 2: done (2026-10-21, `files-and-saves`).** As proposed, in
    four commits:

    1. `src/game_data/game_data.{h,cpp}`: files under the data root
       (ItcApp's: `--game_data_root` or the executable's folder, set in
       `OnConfigurePaths`) from the game's relative paths, `game:\`
       dropped, names matched whatever their case on Linux (exact first,
       then each folder's entries, cached); our own handles
       (`47440000` up), reads at a position or an offset, straight into
       guest memory (`guest::ForEachHostPart`, split at `0xE0000000`).
    2. The file class (`src/hooks/file_class.cpp`): open, read, seek and
       close on the module; other devices (none in play) go to the
       recompiled class. Harness `file_class`: the game's own use (open,
       read, seek to 0, read, close) the same as the recompiled one on
       three files; seeks elsewhere expected to differ: ours reads at the
       position set (checked against the files), a read through the SDK
       after its SetFilePointer doesn't (not followed: the game only
       seeks to 0; the user's call to check this by play).
    3. The loader thread (`src/hooks/archive_streaming.cpp`): archive
       bytes copied straight into the request's buffer, the request-done
       lock signaled after the read as the APC did; the queue, loose
       reads, `Ucl::Unpack` and callbacks called as before. Attract race:
       the same requests, every unpack the right size, latency the same
       (median 11.7 ms against 12.2, 90th percentile 240 against 241).
    4. `Archive::Init`: the TOCs read straight into their buffers, the
       archives opened by the module (their paths read from the image);
       a word the recompiled Init copies unset from its frame copied from
       the same guest stack slot.

    No fallback flags (as for B3, B5, B6 and step 1). The user's play
    (2026-10-21): boot, menus and sound right, and a 3-minute drive far
    along the track with no missing scenery: 2,287 requests, 1 GB
    unpacked, none wrong; the only files the SDK opened were the movies
    (`UBI_LOGO.wmv`, `STAGE1_OPENING.wmv`). The streaming trace in the
    temporary counter is removed; the counter itself stays until B4 is
    done.

    Left in B4: the movie player's reads (with the movie decoder), the
    debug checks (the graphics library's `ShaderDumpxe:` GetFileAttributes,
    the debug capture's file functions, never called), and the functions
    never called in play (the C runtime's I/O, `File::OpenWrite` and
    `Write`, GetDiskFreeSpaceEx, SetEndOfFile, DeleteFile, GetFileTime,
    FlushFileBuffers), to be marked so the boundary counts them.

    **The debug check: surveyed (2026-10-21).** `sub_822B8868` wraps the
    shader build `sub_822B8230` (14 calls at boot, main thread, all from
    `sub_822B8D28`). On its first call only (the byte `82953E29`, set
    after) it asks GetFileAttributes for `ShaderDumpxe:\CompareBackEnds`;
    had the file been there, it would set `82953E28`, print a debug line,
    and build each shader a second way to compare the two. Nothing else
    reads or writes either byte. So GetFileAttributes runs once (the
    earlier "14 calls" were `sub_822B8868`'s), and with the file absent
    the function is `sub_822B8230` with the same arguments.
    `sub_822BC588`, the other GetFileAttributes caller, is the HLSL
    compiler's `#include` (a folder check on the path, then a file
    object), from the grammar's actions `sub_822BC8A8`: never called in
    play (the game compiles no HLSL source).

    Proposed: `sub_822B8868` overridden as a call to `sub_822B8230` with
    the registers as they came (the compare mode off, no file check);
    GetFileAttributes then has no caller in play (Dropped, no override).
    No harness (the override is the original's own path with the flag
    off; running both would build every shader twice); checked by play
    (the boot, and no "VFS: ... [no device]" line).

    **The movies: surveyed (2026-10-21).** The player's file code is the
    XDK media library's file data source (vtable `8204DD50`; +0 the
    destructor `sub_82488FD8`, +48 Close `sub_824880D8`, +52 Skip
    `sub_82488508`, +56 Seek `sub_82488548`, +60 Read `sub_82488598`,
    +108 OpenFile `sub_82487C48`, +112 Reset `sub_82487BF8`, +116 the
    blocks' set-up `sub_82487F68`, +120 Prime `sub_824881D0`, +124
    Cancel `sub_824883C8`, +128 the looping Read `sub_82488A10`). Its
    fields: +44 the handle, +48 the end flag, +56 the start, +64 the
    size, +72/+80 the blocks' window, +88 the position, +104 the block
    size, +108 the block count, +112 the current block's index, +116 the
    blocks to keep ahead, +120 flags (1 opened by name, 4 the handle was
    the caller's: not closed, 8 loop), +124 the blocks' memory, +128 the
    current block, +132 the block list. OpenFile: CreateFile and
    GetFileSizeEx, then the set-up (8 blocks of 128 KB, each a buffer
    and an overlapped with an event), which primes them: an overlapped
    ReadFile each from the position rounded down to 2 KB. Read (r4 the
    destination, r5 the size, r6 the bytes read): from +88, clamped to
    the end (+56 + +64; +48 = 1 when short or at the end), copied out of
    the blocks, each waited for with GetOverlappedResult, and each block
    passed refilled with the next read; a position outside the window
    cancels (CancelIo `sub_825BA0E8`, the waits) and primes again.
    Close: Cancel, CloseHandle (unless flag 4), the blocks freed, Reset.

    What runs (the temporary counter with the source's fields, a plain
    boot and a session the user played, 2026-10-21): each movie opens
    two sources on its file (one per stream), on the thread that starts
    the movie (`sub_820AE64C`'s), through `sub_82487E68` (by name, flags
    1); each is read on its own decoder thread, 64 KB a Read (from
    `824864A4` and `82485204`), with a Seek and a Skip before each; once
    a Read came from the render thread's interrupt callback (the window
    missed: Cancel and Prime). Every ReadFile finished at once under the
    SDK (overlapped status 0), so no wait ever waited. `UBI_LOGO.wmv`
    (8,094,414 bytes, ~9 s): 359 Reads, 208 ReadFiles, 639
    GetOverlappedResults. `STAGE1_OPENING.wmv` (69,372,600 bytes, skipped
    by the user after 60 s at 56 MB): ~1,790 Reads, ~910 ReadFiles. Both
    sources closed by the destructor on the main thread. Never called:
    the looping Read, the open by handle (`sub_82487DD8`) and by wide
    path (`sub_82487D58`).

    Proposed (where the decoder asks for its data: the source's Read),
    plain C++ behind adapters in a new `src/hooks/movie_files.cpp`:

    1. OpenFile native: the path through the game data module, our
       handle at +44 and the host file's size at +64, then the set-up
       (+116) called as now; a missing file answered as the SDK's
       failure was (the error through +32).
    2. The set-up native without blocks: +48, +104 and +116 set as now,
       no buffers, overlappeds or events, no Prime (+108 and +124 stay 0).
    3. Read and the looping Read native: `ReadAt` at +88 straight into
       guest memory (`guest::ForEachHostPart`), clamped at the end, +88
       moved on, r6's bytes, +48 as the original sets it; 0 in r3. The
       two decoder threads read their own handles (the module locks per
       file).
    4. Cancel (nothing in flight: its fields cleared) and Close (our
       handle closed; a caller's handle, flag 4, never in play, left to
       the recompiled CloseHandle) native.
    5. Checks: a harness for Read against the recompiled source (opened
       through the SDK on the same movie, with the same Seek, Skip and
       Read sequence the decoder makes, reads across a block and at the
       end) comparing bytes, +88, +48 and r6; a plain boot with the
       logo's picture and sound; the user's play with `STAGE1_OPENING`.

    Afterwards ReadFile, GetFileSizeEx, GetOverlappedResult, CancelIo,
    CreateFile and Prime have no callers in play (Dropped), and the
    SDK's VFS serves nothing the game asks for.

    **The debug check and the movies: done (2026-10-21,
    `files-and-saves`).** As proposed. `sub_822B8868` calls the shader
    build alone (`src/hooks/graphics_drops.cpp`). The movie source's
    OpenFile, set-up, Read, looping Read and Close are native for our
    handles (`src/hooks/movie_files.cpp`); a handle that isn't ours goes
    to the recompiled methods, so Cancel and Prime are left only to them
    (Dropped). Harness `movie_files` (the recompiled source opened through
    the SDK on `UBI_LOGO.wmv`, the decoder's Seek, Skip and Read: 17
    cases, 0 differ). The user's play: the logo, then a race through
    `STAGE1_OPENING.wmv`, both seen; the log shows no file opened through
    the SDK and no error lines. GetOverlappedResult still runs once at
    boot, from the save load's wait (`sub_820A4FF8`) on an overlapped our
    save code marked done: no wait, and it calls no import itself.
    CreateFile keeps its logging wrapper (`--log_file_opens`), which
    would show any open left. The temporary counter is removed.

    **B4: done (2026-10-21; merged from `files-and-saves`).** No live
    boundary function is left in files
    and saves: the saves, the game's data files, the movies and the debug
    check are native or dropped, and the rest is never called in play.
    Nothing the game asks for goes through the SDK's VFS any more; the
    SDK itself still reads `default.xex` through it at start-up
    (`Runtime::LoadXexImage`), which goes with the image load (E3). The
    kernel's file imports stay linked, never called in play, until the
    SDK goes (E4).

B5. **System services** (82 functions; first counted as 78): sign-in, profile, UI dialogs,
    sessions, voice, network, notifications, the loader. Most should go
    from the game's side entirely (offline, signed in, no voice). Log
    which are called in a full play session first (an opt-in counter
    around the callers), and what the SDK answers today; the game must
    get the same answers.

    Worked on in branch `system-services`. Surveyed 2026-10-20; steps
    1-7 done 2026-10-21 (below the survey, after the proposed order).

    **The functions.** The boundary pipeline filtered to `Xam*` (but
    input, content and `XamAlloc`/`XamFree`), `XMsg*`, `XNotify*`,
    `NetDll_*`, `Xex*`, `ExGetXConfigSetting`, `XGetLanguage`,
    `XGetAVPack`, `HalReturnToFirmware` and `RtlImageXexHeaderField`
    gives 82 functions, plus `xstart` (`DbgPrint`,
    `XamLoaderTerminateTitle`). Two have overrides already: the time zone
    read `sub_8229B440` (the time harness's stand-in) and Device::Init
    `sub_822AD298` (B3), which is also the only code in `src/` calling one
    of these imports directly (`ExGetXConfigSetting(3, 10)`). Most are the
    XDK's thin wrappers (1-60 instructions); the big ones are game code
    calling an import inline: the sign-in manager `sub_8214F290` (227
    instructions), the network manager's update `sub_8214E268` (232), the
    save devices' update `sub_820A4610` (124) and the voice code (up to
    224). By service (XMsg ids: app `FA` XMP, `FB` XGI, `FC` XLiveBase):

    - Notifications: listeners (`sub_822952B8`, `sub_8257B7A8`) and
      their readers `sub_82099828` (the app's: sign-in changed rebuilds
      the user table, `sub_82099600`; Live connection changed sets +248),
      `sub_820A4610` (storage devices changed), `sub_82149080` (Live
      connection changed), `sub_8214E268` (`0x11`), `sub_8214F290`
      (XN_SYS_UI), `sub_8257AF30` (voice).
    - Sign-in and user: XUserGetName `sub_82295020`, GetSigninState
      `sub_82295028`, GetXUID `sub_82295268`, CheckPrivilege
      `sub_82295030`, the title id check `sub_82296EE0`
      (`XamGetExecutionId`).
    - Profile: XUserReadProfileSettings `sub_82576E78` (from
      `sub_82149178`, the network manager), and the voice code's reads
      (`sub_8257E568`, `sub_8257D798`).
    - UI: the sign-in, friends, gamercard, player review, device
      selector, message box and dirty disc dialogs (`sub_822952C0`..
      `sub_82295310`), and the PAL-50 message box `sub_82296950`.
    - XGI (sessions, achievements, stats, contexts): SetContextEx
      `sub_82294ED8` (`B0006`), SetPropertyEx `sub_82294F78` (`B0007`),
      WriteAchievements `sub_82294E58` (`B0008`), `sub_822950C0`
      (`B0021`), the sessions `sub_82575E50`..`sub_825766E0` (`B0010`..
      `B0025`), the stats enumerators `sub_822951A8`, `sub_82295208`,
      and XEnumerate `sub_82295D30`.
    - XLiveBase: `sub_82575C90` (`58004`, the logon id), `sub_825767D0`
      (`58023`), `sub_82576900`, `sub_82576BE0`, `sub_82576EE0`
      (`5800E`), `sub_82576F30`, `sub_82576FE8` (`58035`).
    - XMP (the player's own music): `sub_82589360` (`7001A`, set the
      playback controller), `sub_82589408` (`7001B`, get it), from
      `sub_825894C8` (`Boot::StartMovie`) and `sub_825895A0`
      (`Boot::EndMovie`).
    - Network: WSAStartup `sub_82459B68`, 13 socket wrappers
      `sub_82459C20`..`sub_82459D20`, XNet `sub_82459D28`,
      `sub_82459D40`, `sub_82459D50` (system link: `sub_82148958` makes
      the socket, `sub_8214D7A0` receives).
    - Voice: `sub_8257B7A8` (set-up), `sub_8257AF30` (its thread's
      poll), `sub_8257D798`, `sub_8257DDA0`, `sub_8257DFA8`,
      `sub_8257E3B8`, `sub_8257E4B0`, `sub_8257E568`, `sub_825816D8`.
    - Loader and start-up: `xstart`, the heap check `sub_82297CE0` and
      `sub_82297C08` (`RtlImageXexHeaderField` for the heap size,
      `HalReturnToFirmware` if the heap can't be made), the PAL-50 check
      `sub_82296AB0` (`XexCheckExecutablePrivilege(10)`, `XGetAVPack`,
      `ExGetXConfigSetting`, `XGetLanguage`), XLaunchNewImage
      `sub_822967E0`.
    - Console settings: the time zone read `sub_8229B440`, Device::Init.

    **What runs** (a temporary counter, `--log_system_services`,
    `src/hooks/system_services_log.cpp`; removed after the survey). No
    attract race this time (the user's call): one session the user played
    (`itc_243.log`, 2.5 minutes: the boot movie, the front end, loading a
    save, a race, saving, closing the window). 33 of the 82 ran:

    - Every frame (~11,800 calls, from Draw through `sub_82107910` and
      the app's `sub_82099A98`): `sub_82099828`, `sub_820A4610`,
      `sub_8214E268` with `sub_8214F290` and `sub_82149080`. The voice
      thread's `sub_8257AF30` every 10 ms (14,990).
    - Once at boot: the heap check (returns 1, so no
      `HalReturnToFirmware`), the PAL-50 check (returns 0: the SDK's AV
      pack is 6, so no settings are read), five listeners (masks 3, 1, 3,
      2 and the voice code's), Device::Init's config read (`00040000`),
      WSAStartup (0), a UDP socket whose `bind` and `ioctlsocket` fail
      (-1: no system link), XNetGetTitleXnAddr (4) and
      XNetXnAddrToMachineId (1), the voice set-up with four profile reads,
      and the XMP controller (set at the boot movie's start and end).
    - Each front-end visit: XUserSetContextEx (9 calls, context `8001`,
      probably presence, values 0 and 1), the user table (user 0 signed
      in locally, state 1; users 1-3 state 0; name "User", XUID
      `B13EBABEBABEBABE`), the profile read (10 calls), the time zone
      read (4: an all-zero zone, "uses daylight" 1), and around the saves
      the device selector (twice) and XEnumerate (three times), both
      997 (pending), and the sign-in UI once.
    - Never: achievements, SetPropertyEx, `B0021`, the stats
      enumerators, every session and XLiveBase message, connect, listen,
      accept, select, send and receive, the friends, gamercard, player
      review, message box and dirty disc dialogs, the PAL-50 message box,
      XLaunchNewImage, XUserCheckPrivilege, the voice traffic (create,
      close, submit) and its user check.

    **Notifications.** The game drains its queues, and the SDK writes id
    0 when one is empty, so they can't be logged one by one; but the SDK
    sends a fixed set. The first listener whose mask has bit 0 (the
    app's, `sub_820998B8`, mask 3) gets XN_SYS_UI on and off, sign-in
    changed twice, input devices changed twice and input configuration
    changed twice at start-up. After that only the dialogs send any: the
    device selector XN_SYS_UI on and off (with a 100 ms sleep), the
    sign-in UI sign-in changed and XN_SYS_UI off, to every listener with
    the system bit. Each listener takes only its mask's areas (id >> 25),
    so the XMP's notifications (area 5) reach none, and nothing sends Live
    connection changed (`0x02000002`), storage devices changed (`0xB`) or
    `0x11`: those branches never run.

    **Player data.** The profile read asks for 26 settings, first for the
    size (4,192 bytes, `ERROR_INSUFFICIENT_BUFFER`), then the settings
    (997, completed at once): the SDK's built-in defaults (from 1, the
    profile), the gamercard picture key "gamercard_picture_key", and the
    three title-specific settings (`63E83FFF`..`63E83FFD`) unset (from
    0). The SDK would keep those as files in the game's user content
    folder, but only `XamUserWriteProfileSettings` writes them and the
    game doesn't import it. So no player data is in the profile: a fixed
    answer keeps everything. Career saves are `XamContent` (B4); the
    device selector (device 1) and XEnumerate on the content enumerator
    belong with them.

    **What can go and what needs an answer:**

    - Fixed answers, once at boot: the heap check (1), the PAL-50 check
      (0; clears `XGetAVPack`, `XGetLanguage`,
      `XexCheckExecutablePrivilege`, and `ExGetXConfigSetting` once the
      others are done), Device::Init's config value (`00040000`), the
      time zone read (the SDK's all-zero zone; the last caller of
      `RtlNtStatusToDosError` but B6's, so its mapping table has to be
      ours then), `HalReturnToFirmware` and `RtlImageXexHeaderField`.
    - Fixed answers, the user: one local user, signed in, name, XUID;
      XUserSetContextEx dropped (returns 0, nothing reads it back).
    - The profile read: the same 26 settings as the SDK answers.
    - Network: WSAStartup 0, the system link socket made but `bind` and
      `ioctlsocket` -1, the XNet answers; system link stays off, as
      today.
    - XMP: dropped (0; its notifications reach no listener).
    - Voice: its set-up and 10 ms poll; nothing seen that it gives the
      game (no headset). Can't be dropped by failing the engine's
      creation (`sub_82578BF8`, probably XHVCreateEngine): the engine
      pointer (`826C74A0`, the network manager's +0) is used without a
      null check, every frame by `sub_8214FA78` (vtable +72, from
      `sub_8214E268`) and by `sub_8214F790` (+48), `sub_8214FDF8`,
      `sub_8226EEC8` and `sub_8226BCB8` (+52); only the release
      `sub_8214F690` checks. A native stand-in engine was ruled out (the
      user's rule below), so voice goes with the network manager.

    The user's rule (2026-10-20): Xbox-only features the PC port won't
    use (voice chat, system link, Xbox Live, XMP, presence) are mapped
    out (read and written up in `docs/functions/`) but not brought into
    C++: no stand-ins imitating their interfaces. They're skipped at the
    highest game-level point that's safe (Dropped, with the reason), and
    what the game does need from them (one signed-in local user, the
    profile's answers) becomes plain C++ with fixed answers. For voice,
    system link and Live that point is probably the network manager
    (`826C74A0`): its set-up `sub_8214E190` (the voice engine, the system
    link socket, XNet, listeners) and its per-frame update `sub_8214E268`.
    That update also runs the sign-in manager `sub_8214F290` and the
    profile read, so what the rest of the game reads from them must be
    found (and kept) first.
    - Notifications: the startup set and the dialogs' must reach the
      same listeners. Five of their readers are game code calling
      `XNotifyGetNext` inline, so those functions have to be made native
      whole (or the queue ours behind an adapter for the import's
      callers); the biggest work in B5.
    - Never called: rows only ("never called"), no code.
    - To B4: the device selector and XEnumerate.

    Proposed order (smallest and safest first, player data last): (1)
    the boot-time fixed answers (heap check, PAL-50 check, Device::Init's
    value, the time zone read); (2) the user wrappers (name, sign-in
    state, XUID, title id check, SetContextEx); (3) XMP; (4) the network
    start-up; (5) voice, after reading what it does with its answers; (6)
    the notification readers with their queue (the sign-in watcher first,
    then the network manager and sign-in manager); (7) the profile read.

    **Step 1, the boot-time fixed answers: done (2026-10-21,
    `system-services`).** All four run once at boot; no fallback flags (the
    user's call, as for B3 and B6: the harness and runs check them).

    - The PAL-50 check `sub_82296AB0` (`src/hooks/start_up.cpp`): returns
      0, Dropped. The executable's system flags (header `0x30000`) are
      `0x600`, so privilege 10 is set and the check reaches the AV pack,
      which the SDK answers 6: it passes before any setting is read.
    - The heap check `sub_82297CE0` and `sub_82297C08`
      (`src/hooks/start_up.cpp`): the heap is still made by the game's
      `sub_822997B0(2, 0, 1 MB, 4096, 0, attributes)` (the attributes a
      48-byte block, its size then zeros), a guest call in a frame like the
      original's, kept at `8297149C` (`lis -32105` + 5276; the survey
      first wrote `826A549C`, which made the first try skip the heap and
      crash at boot). The size is fixed: `default.xex` has no default heap
      size field (`0x20401`; its optional headers listed from the file), so
      the SDK's `RtlImageXexHeaderField` answered 0 and the game took its
      1 MB default. If the heap can't be made, ours logs an error and exits
      (the SDK's `HalReturnToFirmware` exited too).
    - Device::Init (`src/hooks/graphics_device.cpp`): the video flags
      stored at device + 15164 are `helpers::kVideoFlags` (`00040000`,
      `src/helpers/console_settings.h`) instead of
      `ExGetXConfigSetting(3, 10)`; the size it wrote at frame + 80 had no
      reader.
    - The time zone read `sub_8229B440`: plain C++ (Overridden only
      because of the harness's adapter, below). The SDK's answers (zero for
      settings 1-7, retail flags `0x40` for 12, so "uses daylight" 1) are
      `helpers::kConsoleTimeZone` and `helpers::UsesDaylight`;
      `time_hooks::ReadTimeZoneSettings` (`src/hooks/time.cpp`) writes the
      same bytes as the original (only the first four characters of each
      name and a terminator: the rest is left alone), and
      GetTimeZoneInformation calls it directly, so the time adapters make
      no guest call for it and its -1 path (the read can't fail) is gone.
      The time harness's made-up zone moved into it
      (`time_hooks::SetFakeZone`); the `sub_8229B440` adapter stays only
      for the recompiled time functions the harness runs.

    Checked: harness cases (`start_up`: the PAL-50 and heap checks after
    boot; `time`: the read against the recompiled one, "uses daylight"
    given and not, on a buffer filled with 0xCD), all the same, and an
    attract race (`attract_race.sh 30 --run_harnesses=all`: no errors, 25 s
    raced), then a session the user played (`itc_247.log`, 2026-10-21: the
    boot movie, the front end, a race at ~150 fps, quit): "looks fine", no
    errors in the log, closed cleanly. Imports with no recompiled
    callers left: `XGetAVPack`, `XGetLanguage`, `XexCheckExecutablePrivilege`, `HalReturnToFirmware`,
    `RtlImageXexHeaderField` and `ExGetXConfigSetting`. Not cleared:
    `RtlNtStatusToDosError`, still called by `sub_822953E8` (reached only
    through the function table: maps an error status and calls a routine,
    probably a file I/O completion routine, B4); our `sub_82296E88` keeps
    the SDK's mapping as C++ until then (the user's call: the table waits).

    **Step 2, the user wrappers: done (2026-10-21, `system-services`).**
    The player as plain C++ in `src/user/user.h` (one local user in slot 0,
    signed in, the SDK's name "User" and XUID `B13EBABEBABEBABE`, the
    SDK's result codes, this title's id `555307EE` from the image's
    execution info), with adapters in `src/hooks/user.cpp` (Overridden):

    - XUserGetName `sub_82295020`: users 4 and up `0x80070057`, 1-3
      `0x80070525`, nothing written; user 0 gets 0 and "User" in at most
      `min(size, 16) - 1` characters and a terminator (nothing for size 0).
    - XUserGetSigninState `sub_82295028`: 1 for user 0, else 0.
    - XUserGetXUID `sub_82295268`: user 0 the XUID and 0; users 1-3 an
      XUID of 0 and 1317; 4 and up 0 and 87; a null out 87, nothing written.
    - The title id check `sub_82296EE0`: 0 for 0 or this title's high half
      (`5553`), else 1627.
    - XUserSetContextEx (presence, Xbox-only): every call goes through
      `sub_82294F70` (no overlapped), whose override returns 0 and clears
      the thread's last error, as the SDK's answer did (it set the last
      error to 0, which the wrapper read back with GetLastError,
      `sub_82296EC0`). Dropped; `sub_82294ED8` is no longer called
      (Dropped, no override). Its callers (`sub_82293504` and the others)
      stay recompiled.

    Checked: harness cases (`user`: the name for users 0-5 and sizes 0-20
    on a buffer filled with 0xCD, the sign-in state, the XUID with and
    without an out, 200 title ids, and SetContextEx's answer with the last
    error slot), all the same, an attract race (no errors), and a session
    the user played (`itc_249.log`, 2026-10-21: the boot, the front end,
    their career save loaded, no race; no errors, closed cleanly). Imports
    with no recompiled callers left: `XamUserGetName`, `XamUserGetXUID`
    and `XamGetExecutionId`. Not cleared: `XamUserGetSigninState` (also in
    XUserCheckPrivilege, never called, and the voice code `sub_8257D798`,
    step 5) and `XMsgStartIORequest` (the other XGI and XLiveBase
    wrappers).

    Seen along the way: the time harness's GetLocalTime case ("to the
    second") can differ by one second when a second ticks over between
    the two reads; once the recompiled one (read first) gave :53 and ours
    :52, so the SDK's clock is probably a little ahead of the host's
    `helpers::SystemTimeNow`. Not from B5 (GetLocalTime has been native
    since B6); not looked into. Since 2026-10-21 the case allows a second
    apart (the user's call), named "within a second".

    **Step 3, XMP: done (2026-10-21, `system-services`).** Dropped at the
    highest point: Boot::StartMovie's `sub_825894C8` (takes the playback
    from the player's music: XMP's get and set controller,
    `sub_82589408` and `sub_82589360`) and Boot::EndMovie's `sub_825895A0`
    (hands it back) return 0 and clear the last error
    (`src/hooks/xmp.cpp`), as the SDK's answers left them (its set went
    through `XMsgStartIORequestEx`, which cleared the last error).
    StartMovie ignores the answer, EndMovie returns it; their state
    (`82957778`, `8295777C`) has no other reader, and XMP's notification
    reached no listener. The two wrappers are no longer called (Dropped).
    No notes kept for a music player of our own (the user's call).
    Checked: a harness case (`xmp`: take then hand back, each answer and
    the last error, the same; the recompiled pair leaves its state at 0),
    an attract race (no errors), and the session below step 4's. Clears `XMsgStartIORequestEx`; not
    `XamGetSystemVersion` (WSAStartup, step 4, and XUserCheckPrivilege,
    never called) or `XMsgInProcessCall` (the XLiveBase wrappers).

    **Step 4, the network start-up: done (2026-10-21, `system-services`).**
    All once at boot, from the network manager's set-up `sub_8214E190`
    (`src/hooks/network.cpp`):

    - WSAStartup `sub_82459B68` (through `sub_82575B08` and
      `sub_82459C10`, from `sub_82148868`): returns 0, Dropped. Its
      WSADATA is in `sub_82575B08`'s frame and never read; the SDK's
      system version (0) never took the `XexGetProcedureAddress` path.
    - The system link socket `sub_82148958`: not made, Dropped; its handle
      (`826C6AA0`) is INVALID_SOCKET (-1). The SDK made a socket (UDP,
      protocol 254) whose bind to port 1000 and FIONBIO failed on Linux
      (probably the privileged port; on Windows the bind probably
      succeeded, leaving a live socket). The handle's only readers are the
      system link code's sendto, select and recvfrom (`sub_8214AB48`,
      `sub_8214AA70`, `sub_8214D7A0`), never run in play; if system link
      is opened they get the SDK's "not a socket". The one answer that
      differs from the SDK's (socket: -1 instead of a handle) was the
      user's call (2026-10-21): an exact one would need a stand-in for
      the SDK's socket.
    - XNetGetTitleXnAddr `sub_82459D50` (looped until nonzero): the SDK's
      XNADDR as fixed bytes (127.0.0.1, no online address or port, `CC`
      for the Ethernet address, no online id) and 4. Overridden.
    - XNetXnAddrToMachineId `sub_82459D40`: 1, nothing written (so the
      machine id `sub_8214F6E8` gets stays 0, cleared by `sub_82148868`).
      Dropped.

    Checked: harness cases (`network`: WSAStartup's answer, the XNADDR on
    a buffer filled with 0xCD and its answer, the machine id's answer and
    its output left alone), all the same, an attract race (no errors),
    and a session the user played for steps 3 and 4 (`itc_252.log`,
    2026-10-21: the boot movie, the front end, a race at ~150 fps, back to
    the menus, quit): "looked fine", no errors, closed cleanly. Imports
    with no recompiled callers left: `NetDll_WSAStartup`,
    `NetDll_XNetGetTitleXnAddr`, `NetDll_XNetXnAddrToMachineId`,
    `XexGetModuleHandle` and `XexGetProcedureAddress` (WSAStartup was
    their only caller); `XamGetSystemVersion` is left only in
    XUserCheckPrivilege (`sub_82295030`, never called). Not cleared: `NetDll_socket`, `NetDll_bind`
    and `NetDll_ioctlsocket` (also in `sub_82148A80`, another socket
    set-up never run in play) and the other socket imports (the system
    link code).

    **Step 5, voice: mapped, no code (2026-10-21, `system-services`).**
    In play the voice code reaches three system services, all inline in
    game code and all with callers elsewhere: Voice::Init's listener
    (`XamNotifyCreateListener`, mask 5) and the voice thread's
    `XNotifyGetNext` (every 10 ms) go with step 6, its four profile reads
    (`sub_8257E568`, `XamUserReadProfileSettings`) with step 7. Every
    `XamVoice*` call, the friends and privilege checks are in functions
    never called (no talker is ever registered): rows only. Dropping
    Voice::Init or the thread would mean building the engine without
    them, close to the stand-in ruled out, for nothing visible; voice goes
    with the network manager in B7 (the user agreed, 2026-10-21: the
    fastest route through these Xbox-only parts).

    The engine (rows in `docs/functions/system-services.md`): made by
    `sub_82578BF8` (probably XHVCreateEngine) through Voice::Init, 2,240
    bytes, +0 a 22-entry table at `829576D0` (filled by `sub_8257B688`),
    +4 a reference count, +8 the 48-byte parameters. The game calls +72
    (`GetDataReadyFlags`, the engine's +1268, 0 with no talker) and +76
    (`GetLocalChatData`, only when a flag is set) every frame from
    `sub_8214FA78`; +48, +44 and +80 from system link code; +52 (probably
    headset present) from UI code (`sub_8226EEC8`, `sub_8226BCB8`); +4 to
    release (`sub_8214F690`, the only null check). The table's order
    matches the XDK's IXHVEngine where the code shows it (+32/+36 local
    talkers with `XamVoiceCreate`/`XamVoiceClose`, +40/+44 remote, +48
    the remote talkers, +72/+76); +64 and +68 are unknown.

    **Step 6, notifications: done (2026-10-21, `system-services`).** The
    user's approach: plug in the data the game uses, not a 360-style queue
    (no masks, versions or start-up set). Of the readers the listener
    wrapper (`sub_822952B8`) served, only the app's
    (`sub_82099828`) ever acted on a notification: the start-up sign-in
    changed (sent twice, to the first listener with the system bit),
    which built the user table (`sub_82099600`). The others never got one
    (`sub_820A4610`'s storage devices changed, `sub_82149080`'s Live
    connection changed, Network::Update's `0x11`), or, for the sign-in
    helper `sub_8214F290`, only the dialogs' system UI on/off and the
    sign-in UI's sign-in changed, which re-polled the same users. So
    (`src/hooks/notify.cpp`):

    - The listener wrapper makes no listener: it returns a fixed nonzero
      value (the readers test the handle for 0; the storage devices'
      update runs only with one). It isn't an SDK handle, so the readers
      left recompiled find no queue in `XNotifyGetNext` and get nothing.
      Dropped.
    - The app's reader builds the user table and sets +244 once, on its
      first call with a listener (the SDK's came on the third and fourth
      frames), and does nothing after (the sign-in UI's rebuild gave the
      same table). Overridden.

    Left as they were: the four other readers (still calling
    `XNotifyGetNext`, answered empty; they go native with their
    subsystems: B4 for the storage devices, B7 for the network manager's)
    and voice's own listener (made inside Voice::Init). With the game's
    listeners no longer in the SDK's list, the SDK's start-up set now goes
    to voice's listener (mask 5, the system bit): its poll sets and
    clears its +1316 flag within the same call, with no talker to act on.
    The sign-in helper no longer sees the dialogs' system UI on/off (its
    "system UI showing" flag stays 0) or the sign-in UI's sign-in changed
    (no re-poll, the users the same). Imports: none cleared
    (`XamNotifyCreateListener` and `XNotifyGetNext` are still called by
    voice and the readers left).

    Checked: a harness case (`notify`: the app's reader against the
    recompiled one reading an SDK listener, with a sign-in changed queued
    against our first call, and nothing against a later one; the app
    object and r3 the same, but the handle slot, which differs by design)
    and an attract race with every harness (no errors), then a session
    the user played (`itc_255.log`, 2026-10-21: the boot, the front end,
    their save loaded through the device selector, a race, quit): no
    errors, closed cleanly.

    **Step 7, the profile read: done (2026-10-21, `system-services`).**
    Hardcoded data (the user's call: no profile code). The network
    manager's `sub_82149178` reads the profile twice through
    XUserReadProfileSettings (`sub_82576E78`): the size (4,192 bytes,
    ERROR_INSUFFICIENT_BUFFER), then its 26 settings (the list at
    `82008EA0`) into an allocated buffer with an overlapped at `826C6080`,
    which Network::Update waits on (997) before it runs; it then reads
    setting 3's value (the gamercard region, 0). Ours
    (`src/user/profile.cpp`, adapter `src/hooks/profile.cpp`): the 26
    settings in the game's order with the SDK's values (the gamercard and
    options at their defaults, the picture key "gamercard_picture_key",
    the three title-specific ones unset), written at the buffer's address
    (the header and the picture key point into it), and the overlapped
    completed as the SDK did (result, length, extended error; the game's
    has no event or completion routine, and nothing reads the thread
    handle the SDK put in its context). Overridden.

    Checked: harness cases (`profile`: the size query, the settings with
    and without an overlapped, another user, a short buffer; the buffer,
    the size, the overlapped and r3 the same as the recompiled one's), an
    attract race (no errors), and a session the user played
    (`itc_258.log`, 2026-10-21: the front end, a race at ~150 fps, quit):
    no errors, closed cleanly. Not cleared: `XamUserReadProfileSettings`
    (voice's own reads, `sub_8257E568`, and the never-called
    `sub_8257D798`).

    **State of B5 (2026-10-21):** steps 1-7 done. Imports with no
    recompiled callers left: `XGetAVPack`, `XGetLanguage`,
    `XexCheckExecutablePrivilege`, `HalReturnToFirmware`,
    `RtlImageXexHeaderField`, `ExGetXConfigSetting`, `XamUserGetName`,
    `XamUserGetXUID`, `XamGetExecutionId`, `XMsgStartIORequestEx`,
    `NetDll_WSAStartup`, `NetDll_XNetGetTitleXnAddr`,
    `NetDll_XNetXnAddrToMachineId`, `XexGetModuleHandle`,
    `XexGetProcedureAddress` (15; 17 counted before, wrongly: the steps'
    lists add up to 15). Left, with where they go: the notifications'
    imports, the profile read's and the voice imports (voice and the
    network manager's code, B7), the socket imports (system link code,
    B7), `XamUserGetSigninState` (XUserCheckPrivilege, never called, and
    voice), `XMsgStartIORequest` and `XMsgInProcessCall` (the XGI and
    XLiveBase wrappers, never called but SetContextEx's, B7),
    `XamGetSystemVersion` (XUserCheckPrivilege), the device selector and
    XEnumerate (B4), `xstart`'s `DbgPrint` and `XamLoaderTerminateTitle`
    (the core).

    **Left to do (2026-10-21, after B2):** of the 72 imports still with a
    live caller after B2, about a dozen are B5's and B6's; the rest are
    the core's. For B5:

    - Rows to settle: wrappers whose callers B5 already made native or
      skipped still count as live because their rows don't say "never
      called": the socket wrappers (`sub_82459C20`, `sub_82459C48`,
      `sub_82459C60`), XamShowSigninUI's (`sub_822952C0`), the profile
      read's (`sub_8257E568`) and the listener's (`sub_8257B7A8`). A
      temporary counter over boot, menus and a race the user plays, then
      the rows marked (or the wrappers Dropped).
    - The other notification readers still calling `XNotifyGetNext`
      (`sub_820A4610`, `sub_82149080`, `sub_8214E268`, `sub_8214F290`;
      listed above for B7): read each and answer what the game needs
      ("no notification"). The fifth, `Voice::Poll` (`sub_8257AF30`), is
      voice chat: mapped, not ported, skipped at the game level.
    - `xstart`'s `XamLoaderTerminateTitle` (the quit path): a native exit.

    **The leftovers, batch 1 (2026-10-21, branch `b5-b6-leftovers`).** A
    temporary counter (`--log_leftovers`, `src/hooks/leftovers_log.cpp`,
    removed after) over every function above and B6's below, plus `main`
    and `exit`: two attract races (`itc_294.log`, `itc_299.log`) and two
    sessions the user played (`itc_295.log`, `itc_300.log`). What it
    showed, and what was done (the user's rule, 2026-10-21: where the game
    needs something, feed it the data and do nothing more):

    - Never called: the socket wrappers (their callers are Dropped or
      never called). Rows marked.
    - XamShowSigninUI's wrapper ran in B5's survey (from `sub_8227C210`)
      but not here: Dropped, returns 0 (the SDK's answer; its sign-in
      changed and XN_SYS_UI off reach only voice's listener since step 6).
    - The profile read `sub_8257E568` isn't one: it's voice's local talker
      set-up (4 at boot from Voice::Init, an allocated talker, codec
      buffers, the profile read somewhere inside), and Voice::Init itself
      makes the listener inline. Both stay recompiled with the network
      manager (B7).
    - Voice::Poll: skipped at its thread. The voice library's worker
      `sub_8257B5D8` (made by Voice::Init when its parameters' +8 is 0)
      only ran Poll every 10 ms, and Poll's work went nowhere (its flag
      +1316 read only by never-called code, an audio voice nothing else
      uses, no talkers). Ours sets the "started" event Voice::Init waits
      on and returns (`src/hooks/voice.cpp`); Poll is no longer called.
    - The Live connection reader `sub_82149080`: with no notification it
      asked the app (`sub_82099580`) for its state, copying the app's +252
      into `826C72F8` and answering its +248, both 0 since the app's
      set-up (only App::PollSignIn wrote them). Dropped: returns 0.
    - SaveDevices::Update `sub_820A4610`: the game's save-slot state
      machines, needed; ours is the same without the read (Overridden,
      `src/hooks/notify.cpp`).
    - Network::Update `sub_8214E268` and SignIn::Update `sub_8214F290`,
      the last `XNotifyGetNext` callers: stubbed in batch 2 (below).
    - `xstart`: `main` never returns in play (closing the window ends the
      title's threads and exits), so its exit, `DbgPrint` and
      `XamLoaderTerminateTitle` never run. Left as it is for now.

    Checked: harness cases (`notify`: the Live reader on the game's app
    with its globals set each way, 4 the same; SaveDevices::Update on
    made-up devices whose methods are recompiled getters and constants,
    60 the same; `user`: XShowSigninUI, 9 the same), an attract race with
    every harness (no errors, 25 s raced) and a session the user played
    (`itc_300.log`: no errors, closed cleanly). Imports with no live
    caller left: `NetDll_socket`, `NetDll_bind`, `NetDll_ioctlsocket`,
    `XamShowSigninUI` (and, from B6's, `sprintf`, `MmQueryStatistics`,
    `XamAlloc`, `XamFree`). Left for B5: `XNotifyGetNext` (Network::Update
    and SignIn::Update, batch 2), `XamNotifyCreateListener` and
    `XamUserReadProfileSettings` (Voice::Init and its talker set-up, B7),
    `XamLoaderTerminateTitle` (`xstart`).

    **Batch 2, Network::Update stubbed (2026-10-21, `b5-b6-leftovers`;
    the user's call: stub it).** A temporary logger
    (`--log_network_update`, removed after) diffed the network manager's
    globals (`826C6000`..`826C7FFF`, `82618B7C`..`82618B87`) around each
    call in a session the user played, loading their save (`itc_304.log`;
    the sign-in and profile paths need the user, not an attract race).
    What the game gets from Network::Update and the sign-in helper it runs:
    on the first call one signed-in local user (the sign-in counts and
    masks, the first user, the user table's name and XUID) and a profile
    read; the profile read again whenever the front end's controller
    assignments (`sub_8209F5E0`, `sub_8209F6B8`) set `826C7938`, then the
    gamercard region copied from its results; nothing else (every later
    10 s summary: 0 words changed). The rest is Xbox-only and never did
    anything: Live users' voice registration, XNet, the Live connection
    and `0x11` notifications, voice chat data, the sign-in dialog and the
    "VS MODE" message box (the user bounds are fixed at 0-4 at boot), and
    system link code, unreachable (the game has no System Link mode, the
    user, 2026-10-21).

    Ours (`src/hooks/network.cpp`, Overridden): the first call writes user
    0 from `src/user/user.h`; the first call and each request run the
    game's own profile read (`sub_82149178`, its buffers, answered by our
    XUserReadProfileSettings), and the next call copies the region; nothing
    else. SignIn::Update and the Live connection reader are no longer
    called (Dropped; the reader's override from batch 1 removed). No
    harness: the recompiled one frees and reallocates the profile buffers,
    so both can't run on the same state; instead its writes were diffed
    the same way, word for word the same as the recompiled one's, the
    profile buffers' layout included, in an attract race (`itc_305.log`)
    and a session the user played (`itc_306.log`: the menus, their save, a
    race; "looks good", no errors). Clears `XNotifyGetNext`.

    **State of B5 (2026-10-21, after the leftovers):** of B5's imports,
    only `XamNotifyCreateListener` and `XamUserReadProfileSettings`
    (Voice::Init and its talker set-up, B7 with the network manager's
    code) and `xstart`'s `XamLoaderTerminateTitle` (never reached: `main`
    doesn't return in play) still have a live caller.

B6. **Pure helpers** (55 functions): string, time and formatting
    helpers made native, or their callers made native. Cheap; can be
    done alongside anything else.

    What counts (2026-10-20, the boundary pipeline filtered to these
    imports; calling functions each): strings (`RtlInitAnsiString` 7,
    `RtlInitUnicodeString`, `RtlUnicodeToMultiByteN`,
    `RtlMultiByteToUnicodeN`, `RtlUnicodeStringToAnsiString`,
    `RtlFreeAnsiString`, `sprintf` 3, `vswprintf`), fills and compares
    (`RtlFillMemoryUlong`, `RtlCompareMemoryUlong` 3), time math
    (`RtlTimeToTimeFields` 3, `RtlTimeFieldsToTime` 2), error codes
    (`RtlNtStatusToDosError` 5), `DbgPrint` 4, fixed answers
    (`KeGetCurrentProcessType` 7, `XGetVideoMode` 2, `XGetAVPack`,
    `XGetLanguage`: log what the SDK answers first) and the clock
    (`KeQuerySystemTime` 2, `KeQueryPerformanceFrequency` 2: frame
    pacing depends on it). Not pure, though listed with them:
    `RtlRaiseException`, `RtlUnwind` and `RtlCaptureContext` work on the
    recompiled code's PowerPC state, so they go with the core. The
    natives live in `src/helpers/` (plain C++), their adapters in
    `src/hooks/`. Worked on in branch `pure-helpers`.

    `RtlInitAnsiString` (2026-10-20, `pure-helpers`): of its 7 callers,
    the 2 pure ones are native (Overridden: their callers are still
    recompiled): `OutputDebugStringA` (`sub_82296270`, empty: the retail
    game drops the text) and the named objects' attributes
    (`sub_822970A8`, checked by a temporary self-test against the
    recompiled version, all equal; its callers, named event and timer
    creation, are threads and sync, the core). The other 5 are file
    functions (`CreateFile` `sub_82295508`, `sub_825B9FD8` with
    `NtQueryFullAttributesFile`, `sub_825B9C20` with
    `NtQueryVolumeInformationFile`, `sub_825B9F20` with
    `NtSetInformationFile`, the mounting in `sub_822B58F0` with
    `ObCreateSymbolicLink`), where the string is one line next to the
    `Nt*` calls: they go with B4, and the import with them.

    Which imports B6 alone clears (2026-10-20: no caller also calls an
    import of another area), done as groups on `pure-helpers`, each
    checked by an attract race and a harness (first temporary, kept in
    `src/harnesses/` since; "Checking with a harness") running the
    recompiled and native versions on the same inputs:

    - Time (done): `KeQuerySystemTime`, `RtlTimeToTimeFields`,
      `RtlTimeFieldsToTime`, through GetLocalTime, the time zone state,
      its cutover dates and the two FILETIME conversions
      (`src/helpers/time.cpp`). Thousands of random cases the same, but
      for times outside 1678-2261, where the SDK's conversions overflow
      `system_clock`'s nanoseconds (ours are right). The zone itself
      still comes from `sub_8229B440` (`ExGetXConfigSetting`, B5) as a
      guest call (native since B5 step 1, 2026-10-21: no guest call).
    - The counter's rate (done): `KeQueryPerformanceFrequency`, through
      QueryPerformanceFrequency and the audio library's hardware wait
      `sub_8245D288` (`src/helpers/clock.h`: 50 MHz, which must match
      the SDK's timebase that recompiled `mftb` reads until the core
      goes).
    - Strings (done): `RtlInitUnicodeString`,
      `RtlUnicodeStringToAnsiString`, `RtlFreeAnsiString` (only in
      OutputDebugStringW, now empty), `RtlMultiByteToUnicodeN`,
      `RtlUnicodeToMultiByteN` (MultiByteToWideChar,
      WideCharToMultiByte; the SDK's Latin-1 behavior kept; UTF-8 stays
      with the game's converters) and `vswprintf` (its one caller,
      swprintf, has one call site whose format has no conversions, so
      ours copies it; no printf).
    - `XGetVideoMode`: moved to B3 (its two callers are graphics set-up:
      `sub_8258D568`, the graphics library's display set-up, and
      `sub_82482B18`), and done there (2026-10-20): both callers native,
      the answer ours (`src/display/display.h`).

    So 10 imports have no recompiled callers left; their SDK code can go
    when the SDK's import table is ours. Checked by the user (2026-10-20):
    a 2.5-minute session (menus, a race, quit) with no errors seen; the
    log had no errors, the native audio played steadily with no silence
    fills (so the audio library's hardware wait, now ours, never held it
    up), and the game closed cleanly. Adapters calling guest code use
    `src/hooks/guest_call.h` (a frame like the recompiled function's).

    Left in B6 (2026-10-20): each import below still has a caller in
    another area, so making B6's callers native clears none of them
    yet; it's work saved for when those areas are done.

    | Import | Callers B6 can do now | Callers waiting |
    |---|---|---|
    | `RtlNtStatusToDosError` | `sub_822953E8` (probably a file I/O completion routine, so maybe B4's); `sub_82296E88` done (the SDK's mapping as C++) | none since 2026-10-21: the time zone read `sub_8229B440` is native (B5 step 1) |
    | `DbgPrint` | `sub_822AC530`, `sub_82486BC0`, `sub_8248FBE0` | `xstart`, the C runtime's entry point, also calling `XamLoaderTerminateTitle` (B5; first listed as `sub_822954C8`, the pipeline's mistake, corrected under "The boundary") |
    | `sprintf` | `sub_822AC638`, `sub_822B1AC8` | the drive mounting `sub_822B58F0` (B4) |
    | `RtlCompareMemoryUlong` | `sub_82298548`, `sub_82298A88` | `sub_8229A930`, the game's heap (core) |
    | `KeGetCurrentProcessType` | `sub_8245A828`, `sub_825776A8` | five heap and memory functions (core) |
    | `RtlInitAnsiString` | none (its 2 pure callers are done) | the file functions `sub_825B9C20`, `sub_825B9F20`, `sub_825B9FD8`, `sub_822B58F0` (B4); `sub_82295508`, CreateFile, is only wrapped for logging |

    Since B4 (2026-10-21) the table's B4 callers no longer run in play:
    `sub_825B9FD8` and `sub_822953E8` are Dropped, `sub_825B9C20`,
    `sub_825B9F20` and `sub_822B58F0` are never called. So
    `RtlNtStatusToDosError`, `sprintf` and `RtlInitAnsiString` have no
    caller in play outside B6; they stay imported until those callers are
    gone with the SDK.

    GetTickCount (2026-10-23, the user's call, branch
    `native-tick-count`): `sub_822956F0` read the kernel's
    `KeTimeStampBundle` (a variable import, +16 the tick count in
    milliseconds, which the SDK sets to its uptime every 1 ms on a timer)
    and is the game's only reader of it. Its adapter
    (`src/hooks/clock.cpp`) returns `helpers::TickCount()`
    (`src/helpers/clock.cpp`): milliseconds since start from the host's
    steady clock, wrapping at 2^32. Recompiled callers reach it through
    the adapter and native ones call it directly, so both share one
    clock, as the heap's callers share the allocation layer. No flag
    (like QueryPerformanceFrequency): a fallback would put the two kinds
    of callers on different clocks. Harness `clock`: over sleeps of 0 to
    100 ms, ours advances as the SDK's count does, within 3 ms. The
    boundary tool counts function imports only, so its numbers don't
    change; the SDK's timer goes with the SDK.

    Blocked entirely: `RtlRaiseException`,
    `RtlUnwind`, `RtlCaptureContext` (the core). `XGetAVPack` and
    `XGetLanguage`, first listed here, were cleared by B5 step 1
    (2026-10-21: their one caller, the PAL-50 check, is Dropped). Re-run the boundary
    pipeline before starting any of these: the callers may have changed.
    Cleared by B3 (2026-10-20): `XGetVideoMode` (both callers native) and
    `RtlFillMemoryUlong` (its only caller, the scaler set-up
    `sub_822B3FE0`, is Dropped).

    **State of B6 (2026-10-20):** every import B6 alone can clear is
    cleared (10; `XGetVideoMode` went to B3). What's left doesn't clear
    any import:

    1. The "now" callers in the table above (11 functions): work saved
       for B3, B4, B5 and the core, with nothing removed from the SDK
       until those are done.
    2. Done (2026-10-20, `pure-helpers`): SetLastError (`sub_82296E70`,
       its jump `sub_82296FD0`, and `sub_82296E88`) in
       `src/hooks/last_error.cpp`, called directly by our time and string
       adapters, which make no SetLastError guest calls any more.
       `sub_82296E88`'s NTSTATUS mapping is the SDK's
       (`xeRtlNtStatusToDosError`, called as C++) until `sub_8229B440`
       (B5), the import's last other caller, is native: then the table
       has to be ours. (Corrected 2026-10-21: `sub_8229B440` is native,
       but `sub_822953E8` still calls the import, so the table waits for
       it.) GetTimeZoneBias (`sub_82297678`) is plain C++ and
       its recompiled version is no longer called: the first function of
       this work to reach Native. GetLocalTime's only guest call left is
       the console settings read (B5). Checked by the harness (all the
       same) and an attract race.

    B6 is done for now: `pure-helpers` was merged into
    `rexglue-removal-project` (2026-10-20). Next item not decided (A2,
    or A6 with B2, would remove visible SDK use). (2026-10-21: B2 and A6
    were done next; what follows is under "Where things stand".)

    **Left to do (2026-10-21, after B2):** `DbgPrint` (`sub_822AC530`,
    `sub_82486BC0`, `sub_8248FBE0`, `xstart`: dropped), `sprintf`
    (`sub_822AC638`, `sub_822B1AC8`, the drive mounting's: probably never
    run, then marked), and two counted under memory that may be fixed
    data rather than the core's: `MmQueryStatistics` (`sub_82294DA0`) and
    `XamAlloc`/`XamFree` (`sub_82575D78`). The heap helpers
    (`RtlCompareMemoryUlong`, `KeGetCurrentProcessType`) and
    `RtlInitializeCriticalSection` stay with the core. Check with the same
    counter run as B5's rows. `boundary_status.py` gives counts only; the
    live callers of each import come from its `boundary()` and the status
    check's rows, read the way its `main()` decides "live".

    **Batch 1 (2026-10-21, `b5-b6-leftovers`; the counter under B5's
    batch 1):** never called in two attract races and two sessions:
    `sub_822AC530` (the graphics library's debug print), `sub_8248FBE0` (a
    method that isn't implemented: `DbgPrint` and E_NOTIMPL), the sprintf
    callers `sub_822AC638` (only from the never-called "GPU hung" dump)
    and `sub_822B1AC8` (through a pointer), and `sub_82575D78`
    (`XamAlloc`/`XamFree`, only from two never-called XLiveBase requests):
    rows marked. GlobalMemoryStatus `sub_82294DA0` (`MmQueryStatistics`)
    runs (25 calls a session) but its three callers never read the buffer:
    Dropped, writes nothing (`src/hooks/memory_status.cpp`). `sprintf`,
    `MmQueryStatistics`, `XamAlloc` and `XamFree` have no live caller left.
    `DbgPrint` is left in `xstart` (never reached, under B5) and the movie
    player's stream set-up `sub_82486BC0` (twice a session, its print only
    on a third stream that never comes; it goes with the movie player in
    B7).

B7. **The game logic inside**: everything else, subsystem by subsystem,
    in whatever order the user wants (frame interpolation's hooks,
    vehicles, UI, menus, physics, ...). Each subsystem moves its data to
    level 2 once no recompiled code touches it. A function's constants,
    strings and tables from the image move into the source with it
    (literals and arrays, compiled into the exe); big tables are better
    copied out of the image by a small script than by hand. While
    recompiled code still reads a table, it stays in the image too, and
    the two copies must agree. This is most of the
    work: full decompilations of games this size usually take years. Keep
    the statuses in `docs/functions/` up to date (Wrapped, Overridden,
    Native, Dropped, defined at its top) so progress can be measured:
    Native, Dropped and Overridden are how much of the game is ours,
    Native and Dropped how much no longer needs `ctx`. Keyboard input comes back
    once the game is native (A0).

    **B7's first step, native memory** (`docs/native-memory.md`, designed
    with the user and built 2026-10-22, on branch `native-heap`): one allocation
    layer of our own (`src/memory/`) that every allocation goes through,
    recompiled and native alike: the process heap, VirtualAlloc and
    physical memory, routed at nine functions behind `--native_heap`.
    Native code allocates guest memory through it without `ctx`, and the
    port's end swaps its backing in one place.

    What B7 has to deal with is the functions that run, not all 16,552:
    `docs/coverage.md` (started 2026-10-21, branch `coverage`) records them
    with a coverage build (`itc_coverage`) in
    `itc_recomp/coverage/seen_in_play.txt`; a 20-second attract race ran
    4,975 (30%). Its report, joined with `docs/functions/`, is B7's
    measure (`itc_recomp/tools/b7_status.py`).

    **B7's loop runs:** the B7 loop (`docs/loops/b7-loop.md`; progress
    and what's for the user in `docs/loops/b7-progress.md`) maps
    subsystems into their own function files and makes their ready groups
    native, each behind a `--native_*` flag (on by default, falling back
    to the recompiled function) with harness cases comparing it with the
    recompiled one. The loop adds a row here as each subsystem is
    finished:

    | Finished | Subsystem | Function file | Native | Flags |
    |---|---|---|---|---|
    | 2026-10-22 | Camera-facing sprites and light glows | `sprites.md` | 10 of 26 | `--native_sprites`, `--native_billboards` |
    | 2026-10-22 | The graphics library's command list | `command-list.md` | 10 of 11 | `--native_command_list` |
    | 2026-10-22 | The C runtime and math library | `c-runtime.md` | 79 of 89 | `--native_math`, `--native_c_runtime`, `--native_memcpy` |
    | 2026-10-22 | The UI movie library | `ui-movies.md` | 32 of 45 | `--native_ui_movies` |
    | 2026-10-22 | The game's UI movie side | `ui-movies-game.md` | 42 of 89 | `--native_ui_movies_game` |
    | 2026-10-22 | The race overlays' update side | `race-overlays.md` | 88 of 158 | `--native_race_overlays` |
    | 2026-10-22 | The vehicle, rigid body, collision and race data accessors, and the vector library | `vehicle-accessors.md` | 127 of 132 | `--native_vehicle_accessors`, `--native_vector_library` |
    | 2026-10-22 | The matrix, vector and quaternion math (`Matrix::` and the vector library's 4x4 half) | `matrix-math.md` | 28 of 28 | `--native_matrix_math` |
    | 2026-10-22 | The car physics and the collision world | `car-physics.md` | 106 of 130 | `--native_car_physics` |
| 2026-10-22 | The C runtime heap (operator new and delete, malloc, free, calloc, realloc, `_msize`, XMemAlloc, XMemFree, LocalAlloc, LocalFree, GetProcessHeap; in the existing `heap.md`) | `heap.md` | 24 of 30 | `--native_c_runtime_heap` |
| 2026-10-23 | The collision world's narrow phase and shapes (with CollisionObject::Translate, Vehicle::ApplyForces and two rigid body accessors) | `collision.md` | 64 of 64 | `--native_collision` |
| 2026-10-23 | The collision world's owner and the track's collision (the wall and ground queries, the track mesh and positions, the teardown) | `collision-world.md` | 37 of 43 | `--native_collision_world` |
| 2026-10-23 | The sound cues (the game's sound manager, its wrapper over XACT and X3DAudio, X3DAudioInitialize) | `sound-cues.md` | 30 of 87 | `--native_sound_cues` |
| 2026-10-23 | XACT, the sound library linked into the game (its engine, banks, cues, sounds and faders; the user's call to translate it) | `xact.md` | 102 of 186 (two maps, three migrations) | `--native_xact` |
| 2026-10-23 | The action mappers (the game's input actions over the pads: set-up, client list, update, read slots; no behavior change) | `action-mappers.md` | 36 of 40 | `--native_action_mappers` |
| 2026-10-23 | The game's data tables at `[0x826C5CA0]` (their load callbacks, byte swaps and accessors; no behavior change) | `data-tables.md` | 16 of 18 | `--native_data_tables` |
| 2026-10-23 | The model animation (track sets and their keyframed curves, `Model::Animate`, the skeleton's node values and matrices; with the primitive drawer's constructor) | `model-animation.md` | 36 of 39 (and `models.md`'s `sub_8259C3A8`) | `--native_model_animation` |
| 2026-10-23 | The car specs (the parameter tables' set-up, load callbacks and byte swaps, the getters, `Car::Refresh` and `Tire::InchUp`; no behavior change) | `car-specs.md` | 47 of 48 (`Params::Free` not seen in play) | `--native_car_specs` |
| 2026-10-23 | The cameras below the camera modes (the library's `Camera` class, the 21 camera passes and their record passes, the two viewports; interpolation's hooks kept) | `cameras.md` | 32 of 42 (two migrations) | `--native_cameras` |
| 2026-10-23 | The string table and font (the 17 string files, their pages and formatting with the game's `swprintf`, the font's layout and text draw; the C runtime's wide printf with it) | `strings-and-font.md` | 25 of 36 | `--native_strings_and_font` |
| 2026-10-23 | `std::string` (Dinkumware's `basic_string<char>` over the guest layout: constructors, `_Tidy`, `_Copy`, `_Grow`, assign, append, the allocator; the library's wildcard search, find and replace-all) | `std-string.md` | 14 of 16 | `--native_std_string` |
| 2026-10-23 | The course's route (route positions normalized and moved along linked sections, points, lanes and distances, `TrackRoute::Follow` with its full search; with `collision-world.md`'s `TrackPosition::Init` and `Update`) | `track-route.md` | 40 of 40 (two migrations) | `--native_track_route` |
| 2026-10-24 | The AI drivers (the computer racers' driving: each driver's path along the course's route, its corners and line, the other racers' records, the skill block, the steering, throttle and brake into the car's input record; `AiDriver::DriveCar` and `AiDrivers::UpdateAll`) | `ai-drivers.md` | 143 of 148 (five migrations; 119 Native) | `--native_ai_drivers` |
| 2026-10-24 | The racer controllers (each car's input record filled from the player's pad or its AI driver each tick: the pad reader, the input recording and replay, the race flow's per-state handlers, `AiDriver::Setup`) | `racer-controllers.md` | 27 of 27 (one migration) | `--native_racer_controllers` |
| 2026-10-24 | The track zones (70 hardcoded stretches of the course's route the player's car is checked against each tick for the HUD: their set-ups, route ranges, fixed-size lists and the owner at `[0x828161EC]`) | `track-zones.md` | 92 of 92 (one migration; 9 Native) | `--native_track_zones` |
| 2026-10-24 | The graphics resources (the graphics library's texture, surface and buffer headers and descriptions, their creation, the EDRAM allocator, surface levels, the device's current targets and stream, locks, reference counts and destruction; the user's calls: `Edram::Alloc` with a `std::mutex`, the locks' fence waits and cache flushes Dropped) | `graphics-resources.md` | 46 of 46 (two migrations) | `--native_graphics_resources` |
| 2026-10-24 | The video output and the device lock (the video output's constructor, `LockDevice` and `UnlockDevice` over a host `std::binary_semaphore` by the user's call, the device's thread ownership; with `cameras.md`'s `MakeRenderTarget`, `SetUpSurfaces` and `Init`, which it blocked; the shared sync wrappers kept recompiled) | `video-output.md` | 7 of 30 (and 3 of `cameras.md`) | `--native_video_output` (`--native_cameras`) |
| 2026-10-24 | The graphics device's state (the render and sampler state setters through the device's tables, the stream, indices, textures, shader constants, `SetFVF`, the render target and depth surface, scissor and viewport, `SetDefaultState`, `DirtyAllState`, the GPU register split, gamma; the packets written straight into the command buffer, which nothing reads, Dropped by the graphics rule) | `device-state.md` | 131 of 131 (two migrations) | `--native_device_state` |
| 2026-10-24 | The compiler's array helpers (`vector constructor iterator` and `vector destructor iterator`, probably: the element constructor or destructor called on each; the native targets called as plain C++, the empty destructor skipped, others a guest call; in the existing `c-runtime.md`, with wcslen's, mbtowc's and the wide write_char's adapters removed) | `c-runtime.md` | 84 of 91 (2 new, one migration) | `--native_c_runtime` |
| 2026-10-24 | The course scenery (the course's areas, their models and lights collected around each viewport and streamed into five pools of physical memory: the pools, pool objects and their keys, the scenery lists, `StartLoad`, `ReleaseUnused`, `LimitLargeModels`; with the texture animations' advance, interpolation's render-only skip kept) | `course-scenery.md` | 37 of 73 (one migration) | `--native_course_scenery` |
| 2026-10-24 | XAPI's memset (`XMemSet`: one host memset at the stride the game sets, the translated steps at any other; with what it unblocked: `Scenery::Init`, `CameraPasses::Init` and `InitPasses`; XAPI's `XPhysicalSize` and `XQueryMemoryProtect` mapped in `heap.md`, kept for the core) | `c-runtime.md` | 1 of 1 new (85 of 92 in the file; and `cameras.md` 40 of 42, `course-scenery.md` 38 of 73) | `--native_c_runtime` (`--native_cameras`, `--native_course_scenery`) |
| 2026-10-24 | The post effects (the screen-space effects drawn over each view: the effect list's globals and toggles, the glow's and color filter's setters, the recording helpers (states, targets, constants, quads, resolves), the glare's definitions, the render textures' release, the destructors; frame interpolation's overrides kept; `RenderTextures::Acquire` left on `sub_8235A9F8`) | `post-effects.md` | 44 of 57 (one migration) | `--native_post_effects` |
| 2026-10-24 | The race's screen objects (one 128,000-byte object a view: probably a screen blur while the nitrous runs, its strength from the speed; the two `std::map` instantiations of its ring UVs (`UvMap`, `UvColumn`: double keys), the rings, the strength, the release and teardown; the fan, its set-up and the draw left waiting on `rand` and the shader pairs) | `screen-objects.md` | 38 of 42 (one migration) | `--native_screen_objects` |
| 2026-10-25 | The app framework (the framework object `main` makes, its services' factories and getters, the run loop `App::Run` with its two `std::map`s of handlers and its request vector, the tick handlers' dispatch, the constructors and `main` itself; `Framework::Init` and the services' set-ups left waiting on other subsystems and kernel sync) | `framework.md` | 56 of 68 (two migrations) | `--native_framework` |
| 2026-10-25 | The game modes (the one mode object the game runs at a time; the race mode's message dispatcher and its state machines: transitions keyed by (state, message), `StateMachine::Add`, `Find` and `Handle`; the race overlays' state machine and its handlers; the race mode's members and 28-byte buffers; the race mode's constructor, destructor and set-ups left waiting on `XPR::XPR`, other set-ups and the mode constructors) | `game-modes.md` | 38 of 42 (two migrations, the second after the model class; 9 adapters removed, 3 Merged) | `--native_game_modes` |
| 2026-10-25 | The model class (`Model::Model`, `Release`, `~Model`, the texture provider and its `std::vector` helpers, `XPR::XPR`, `Release` and `~XPR`, the put-off frees; the memory blocks' lock `0x8296BD38` a host lock taken by every user, the user's call; with `memmove_s` in `c-runtime.md`) | `models.md` | 17 of 17 new rows (19 of 50 in the file; one migration) | `--native_models` |
| 2026-10-25 | The mode tick (what `Modes::Tick` calls besides the mode's slots: the per-mode settings, the sign-in watch, the billboards' ring flip, the two layers of screen fades, the menu music with XAPI's `lstrcmpA` and `CompareStringA` as plain C++ by B6's call; presence Dropped by B5's; with `Modes::Modes` in `game-modes.md`) | `mode-tick.md` | 21 of 23 (one migration; 1 Dropped, 4 Merged) | `--native_mode_tick` |
| 2026-10-25 | The minimap's parts (the race manager's lookups, the car marker's type, the scaling, the marker triangles and their 80-slot lists, the transform and the triangles' records; interpolation's hooks kept, `Minimap::Draw`'s blink wrap untouched; the marker draws and `Minimap::Draw` left waiting on `atexit` and the course set's lists) | `minimap.md` | 12 of 15 (one migration) | `--native_minimap` |
| 2026-10-25 | `atexit` and what it unblocked (`atexit`, `_onexit` and `__onexitinit` over the game's exit table, the C runtime's exit lock a host mutex taken by every user through `_lock`/`_unlock` overridden for lock 8 only, the user's call by the recorded lock call; the global strings' static set-ups, `SignInWatch::StaticInit`, `UiMovie::TrackProperty`, `ApplyTrack` and `ApplyAnimation`, the minimap's marker draws, `Viewports::Init` and `Viewport::Init`; no mapping) | `c-runtime.md`, and `ui-movies.md`, `minimap.md`, `cameras.md`, `std-string.md`, `mode-tick.md` | 18 made native, 12 adapters removed (two migrations) | under `--native_c_runtime` and the files' own flags |
| 2026-10-25 | The scene models' set-up side (the cars as the scene holds them, pool 7 of the second pool set: the parts' links, the 4,244-byte records' defaults, copies and constructor, the car records' reset and copy, the accessory, the sound loads' callback, the car model's pages through `memory::AllocPages`; `SceneModel::SetUp`, `Build`, `Update` and the loaders left waiting) | `scene-models.md` | 23 of 48 (one migration; 1 Native with no adapter) | `--native_scene_models` |
| 2026-10-25 | `UiMovie::DrawTexts` and what it unblocked (no mapping, not counted: `DrawTexts`, `ScrollOffset`, `UiMovie::Update`, `Draw`, `Run`, `Finish`, `FinishNamed`, `FinishAll`; `_init_pointers` and the empty destructor `sub_82106540`; the race HUD's movie picker and the rival logo's draw in the new `race-hud.md`) | `ui-movies-game.md`, `ui-movies.md`, `c-runtime.md`, `race-hud.md` | 13 made native, 9 adapters removed, 1 Merged (two migrations) | `--native_ui_movies_game`, `--native_ui_movies`, `--native_c_runtime`, `--native_race_hud` |
| 2026-10-25 | The UI movies' ready groups (no mapping, not counted: the movie's vectors, `UiMovie::UiMovie` and `~UiMovie`, the texts freed, `GetText` and the six text setters over it, the named animations' set-up, the HUD's mirror effect `sub_8211BEE8`) | `ui-movies-game.md` | 26 rows (19 with plain C++, 7 Merged; no adapter removable yet) | `--native_ui_movies_game` |
| 2026-10-25 | The scenery lists (the course set's six model lists: release, the draw picks by race data, course layout and mode, `PointList::Draw`; the scenery model's and the shared model draws: `IsShapeVisible`, `RecordObject` inside its interpolation wrapper, `RecordSceneLights`, the depth draws, `Model::DrawShape` and `RecordShape`; the texture provider's slots 2 and 4; `models.md`'s accessors) | `scenery-lists.md` | 25 of 27, and 6 in `models.md` (two migrations; 1 adapter removed, 2 Native with none, 3 Merged) | `--native_scenery_lists`, `--native_models` |
| 2026-10-25 | The shader objects and what they unblocked (the shader creates and inits, the memory blocks' makers and put-off frees under the host lock, `XMemCpy`, the relocations, `XPR::Create`, `Mesh::LoadShaders`, `Mesh::Parse`, `Model::Load`; then `DataFile::GetEntry`, the scenery lists' loads, the course set's parsers, pool objects, collect and draw, `Strings::Get`, `FormatV` and `Format`) | `shader-objects.md`, with `models.md`, `c-runtime.md`, `files-and-saves.md`, `scenery-lists.md`, `course-scenery.md`, `strings-and-font.md` | 7 of 7, and 38 in the other files (three migrations; 15 adapters removed) | `--native_shader_objects`, `--native_data_files` and the files' flags |
| 2026-10-26 | The race's set-up (`Race::SetUp`, `SetUpRival`, the race data's reset and constructor, the course route table `CourseRoutes::Get`, the racers' adds and start positions, the stage list and routes, the car object's set-up `Car::Init`, `Clear`, `SetModel`, `FromBuild` and its parts' resets, `RacerRecord::Init`; then `Vehicle::SetupFromCar` with `CarData::Derive`, `ApplySettings` and `Vehicle::ApplyTuning`) | `race-setup.md`, with `vehicle-accessors.md`, `car-physics.md` | 52 of 52, and 1 in `vehicle-accessors.md` and 4 in `car-physics.md` (three migrations; 27 adapters removed) | `--native_race_setup`, `--native_car_physics` |
| 2026-10-26 | The car physics' ready groups (no mapping, not counted: `Vehicle::PhysicsStep`, `SetUpPhysics`, `ResetPhysics`, `DestroyPhysics`, `ProbeGround`, `Unflip`, `RefreshFromBody`, the collision objects' init and add, `CollisionWorld::Setup`, `Create`, `SetQueryCapacity` and `Update` with its pairs, collide and respond) | `car-physics.md`, with `vehicle-accessors.md`, `matrix-math.md`, `collision.md` | 134 of 134 (two migrations; 84 adapters removed, 7 Native with none) | `--native_car_physics` |
| 2026-10-27 | The vehicle code above the physics (the seven vehicles' create, constructor and destroy, `Vehicle::SetUp` and `ResetForCar`, the surface grip, `Vehicle::Tick` and `ApplyInput`, the track positions, the status record, `PlaceOnRoute`, `Racers::NearestHumanVehicle`; the engine's power curve and `RoutePosition::Step`) | `vehicle.md` | 25 of 25 (two migrations; 5 adapters removed, 1 Native with none, 1 Merged) | `--native_vehicle` |
| 2026-10-28 | The app's update and its state stack (`App::Update`, `App::Start`, `App::IsRunning`, the handler maps, requests, messages and add slots; the state stack `AppStates` and its 48-byte state layers: their set-up, walks, updates, operations and toggle messages; the containers they use: the message vector, the operation list, `PtrList::Erase` and `ForEach`, the delegate's clone) | `app-states.md` | 56 of 56 (two migrations; 19 adapters removed, 4 Native with none, 1 Merged) | `--native_app_states` |
| 2026-10-28 | The scene's loading objects (the first scene pool set's seven loading classes: car model, parts, cockpit body, interior, pool 4's model, wheels, traffic body; `ScenePools::MakeA`, the pool node, the classes' load checks, `CarParts::Part`, the XPR name lookups, `CarModelLoads`) | `scene-loaders.md` | 20 of 78 (one migration; 5 Native with no adapter) | `--native_scene_loaders` |
| 2026-10-28 | The scene loaders' blockers (no mapping, not counted: the load slots, the deferred release, the part model, `CarModel::Release` and `Reset`; then the loading classes' constructors, releases and destructors, `ScenePools::CreateA`, `Interior::OnPartLoaded`, `Skeleton::ResetPose`, `Model::SetVinylTextures`, `SetSharedShaders`) | `scene-loaders.md`, with `car-models.md`, `models.md` | `scene-loaders.md` 60 of 78, `car-models.md` 4 of 42, `models.md` 45 of 50 (two migrations; 27 adapters removed) | `--native_car_models` (new), `--native_scene_loaders`, `--native_models` |
| 2026-10-28 | The players' save data in memory (`SaveData::Create` and `Delete`, the progress's clear and defaults, the stats record, the course records, the options and control maps, the default tables' getters; `TitleScreen::ResetPlayer`, `MenuList::Init`, `Players::ResetControllers`) | `save-data.md` | 19 of 19 in play (one migration; 11 adapters removed; 4 rows not seen in play) | `--native_save_data` |
| 2026-10-28 | Texture compression (XGraphics' `XGCompressSurface` over D3DX's image blitter, used in play only by `Vinyl::Compress` for the vinyl sheet, A8R8G8B8 to DXT5 premultiplied: the DXT alpha and color block encoders, the DXT4/5 encoders, the A8R8G8B8 and DXT codecs, `ImageCodec::Create`, the blitter) | `texture-compression.md` | 24 of 24 in play (one migration in five batches; 18 adapters removed; 133 rows not seen in play) | `--native_texture_compression` |
| 2026-10-28 | The vinyl sheet above texture compression (no mapping, not counted: `Vinyl::Compress`, `Texture::Create` and `Free`; then the decal path under `CarModel::OnDecalLoaded`: the TGA reader, the software rasterizer, `Vinyl::DrawDecal`, `DrawDecals`, `ClearCorner`) | `car-models.md` | 27 of 46 (two migrations; 2 adapters removed, `XGCompressSurface` Native; `ClearCorner`'s harness doesn't reach it) | `--native_car_models` |
| 2026-10-28 | The race world (the race mode's state machine at +116812: `RaceWorld::Init`, `OnLoaded`, `Tick`, `Unload`, the paused tick; the racers' manager at `[0x826D82F4]`: its tables, `AddRacers`, `ListRacers`, the vehicles streamed to the nearest entries by distance, `Refresh`, `ResetWorld`, `ResetVehicles`; the race event's start placements, `RaceStart::Place` and `PlaceRolling`; the save's distance on unload; with `TrackCollision::Tick`) | `race-world.md` | 49 of 50 in play (three migrations; 25 adapters removed; `OnLoad` waits on the archive) | `--native_race_world` |
| 2026-10-28 | The race's states (`RaceStates::Init`'s machine at +107168, 87 transitions: the stage's start, the rolling and standing starts, the countdown, `RaceTick`, `OnGo`, quit, the end, the replay's and menu's handlers, free run; the race's progress at `[0x826D82E8]`: its checks for laps, finish, hits, damage, the SP drain, rank and time; the route helpers `StartRoute`, `FollowRoute`, `RouteDistance`, `MapToSection`) | `race-states.md` | 43 of 63 in play (two migrations; 20 Native; the rest waits on the loading screen, the archive's queue, the cars put back, the input recordings, the scene models' load poll, the items and achievements, the race event's lookups, the two-player cameras, the core) | `--native_race_states` |
| 2026-10-28 | The archive (`Archive::Toc`, `AssetSize`, `Load`, the request queue and slots, the loader's picker, set-up and next request, WaitIdle, QueueLength, the cancels, the request finish, `LoaderFunctor::Invoke`, loose files, the Ucl/NRV2B unpacker; its four semaphores as host counting semaphores, the user's call) | `archive.md` (with the shared rows in `files-and-saves.md` and `video-output.md`) | 25 (all of `archive.md`'s 11 in play and 14 shared rows; two migrations; 8 adapters removed; the rest waits on the loose read's file object, open by name and sprintf, and the game's `Semaphore::Init`) | `--native_archive` |
| 2026-10-28 | The file layer (the file object `GameFile` over `AsyncFile` and `File::`, the path objects and `Path::Build`, `GamePath::SetGame`, the open by name; with sprintf, `_vsnprintf`, `_vsnprintf_l` and `_output_l` it unblocked, and the archive's loose read) | `file-layer.md` (with rows in `c-runtime.md` and `files-and-saves.md`) | 17 (all 11 in play of `file-layer.md` with the 6 it unblocked; one migration; 7 adapters removed) | `--native_file_layer` (sprintf under `--native_c_runtime`, the loose read under `--native_archive`) |
| 2026-10-28 | The save system's interface (the app's interface at `[0x826BDC90]`, its wrappers over the save devices' slots and the users; the front end's save layer: the save buffers, `GameData::Save`, `Load` and `Check`, the replays' list, `PlayTime`) | `save-interface.md` (with rows in `files-and-saves.md`, `system-services.md`) | 41 of 41 in play, with 5 shared rows (one migration; 17 adapters removed) | `--native_save_interface` |
| 2026-10-28 | The car sounds and X3DAudio (the seven per-racer car sound objects at `[0x826C5C3C]`, eleven parts each, played straight through XAudio voices: the wave bank views, `SetBanks`, `TireSound::Amount`, the constructors, `CarSound::SetUp`, `SoundGlobals::Init`, `CarSounds::Destroy`, `CarSound::Update3D`; X3DAudio's math, translated by the XACT precedent: `X3DAudioCalculate` with its vector helpers, curves, pan, matrix, cone and Doppler) | `car-sounds.md` | 62 of 62 in play (three migrations, the last two after XAudio's voices; 53 adapters removed) | `--native_car_sounds` |
| 2026-10-29 | XAudio's voices (the game's copy of XAudio, translated: the API entry points but the shut-down, the callback's and the commit; the source and submix voices, their outputs, passes, effects and states; the buffers and formats; its API lock the game's critical section and the voice lists' spin lock the game's, taken as the original does, the user's calls) | `xaudio.md` | 92 of 96 in play (two migrations; 37 Native; the rest waits on the engine and mixer) | `--native_xaudio` |
| 2026-10-29 | XACT's waves and voices (XACT's voice class over XAudio's entry points, the waves' constructors, start positions, voices, play, pause, next and update, the tracks' volume and pitch getters; XACT's lock the game's, taken as the original does) | `xact-waves.md` | 22 of 30 in play (one migration; 16 adapters removed, 7 of XAudio's and 5 of `xact.md`'s; the rest waits on `XACTWave::Notify`'s notifications and clock) | `--native_xact_waves` |

    The first run (2026-10-22, branch `b7-loop-test`): every harness at 0 differing, attract races at 20-21 s with no errors,
    and a race the user drove looked right. Ours went from 162 to 335 (work
    left 6,590 -> 6,418). The C runtime's math (and the UI movies' keys)
    build with `-ffp-model=strict -ffp-contract=off` to give the
    translated code's doubles bit for bit, NaN signs included (the intro
    of `docs/functions/c-runtime.md`). What blocks the rest of these
    subsystems: the guest heap and the VirtualAlloc wrappers (the user's
    decision then; since native memory, 2026-10-22, native code allocates
    through the layer), TLS (`_getptd`), the matrix math (`Matrix::Multiply` and
    `Matrix::Copy`, Wrapped), std::string, the string table and font, the
    texture set; each is a lead in `docs/loops/leads.md`.

    The vehicle accessors (2026-10-22, branch `b7-loop-22-10-26`, the
    third run): small getters and setters, nearly all one-function groups,
    ported in batches by object. The vector library's and the rigid
    body's harnesses keep NaN inputs in "(expected to differ)" groups:
    when two NaNs meet, which one comes out depends on the operand order
    the compiler picked, in the translated code and ours alike (a few
    cases differ there, only in a NaN's sign or payload). Left:
    `sub_820B8798` (a tail call through a function pointer, needs `ctx`)
    and four functions behind blockers outside the file (`sub_820B94B0`,
    `sub_820B8AF8`, `sub_820DA180`, `sub_820CD5D8`).

    The matrix math (2026-10-22, the same run): all 28 functions ours in
    two migrations, built with `-ffp-model=strict -ffp-contract=off`.
    `Matrix::Multiply`, `Matrix::Copy` and `Vec4::Transform` were
    Wrapped for interpolation: their overrides now do the math natively
    with the same hooks around it, and native callers of Multiply note
    their multiplies through `interpolation::BlendState_OnMultiply`
    (frame dumps with the flag on and off match). Every function has a
    "NaNs anywhere (expected to differ)" harness group (where two NaNs
    meet, the compiler's operand order picks which comes out); the
    cases with numbers alone or one NaN match. It unblocked four race
    overlays groups and UiMovie::ApplyTransform (leads).

    The car physics (2026-10-22, the same run): 106 of 130 ours in four
    migrations: the rigid body, the physics step's updates (suspension,
    steering, tires, engine, turbo, gearbox, drivetrain, brakes, aero),
    the collision world's objects, pairs and surfaces, the car data and
    the vehicle's constructor; the AI cars of an attract race in the same
    places with the flag on and off. Its harness found the vector
    library's dot products leaving their flush-to-zero window under
    ThinLTO (fixed, ee810a9). Left: the physics step's root (ProbeGround,
    through a function pointer), the collision object's destructor (a
    vtable call), the collision update and narrow phase (a lead), and
    the set-up functions that allocate (the heap).

    The C runtime heap (2026-10-22, branch `b7-loop-22-10-26-2`, the
    fourth run): the 15 C runtime allocators above the native allocation
    layer made native in one migration, plain C++ in
    `src/memory/c_runtime_heap.cpp` calling the layer directly. Their
    adapters run it only while `--native_heap` is on (`hooks::NativeHeapActive()`),
    so the game's own heap and the layer never mix; the error paths
    (errno, the new handler, `bad_alloc`, `_invalid_parameter`) run the
    recompiled function from the start. `_calloc_impl`, LocalAlloc and
    LocalFree are Native; the rest keep adapters for their recompiled
    callers (the core's C runtime set-up, the shader code's
    `sub_8235AAC0`, the game's many callers of new, delete and XMemAlloc).
    The game's own heap internals (`sub_82297FD8`, `sub_822982B8`) no
    longer run with `--native_heap` and aren't ported.

    The collision narrow phase (2026-10-23, the same run): all 64 ours
    in two migrations: the box shape (the only shape seen in play; its
    vtable read at run time), the shape record's placement and motion,
    the box-box separation tests, impacts and push-outs, and the pair's
    solve and response. Any other shape, or an impact table entry that
    isn't the box-box one, runs the original. Attract races never reach
    the impacts; the harness builds boxes that touch, and the pad replay
    (now step 3 of the validation agent) reached them. Respond's and
    Solve's harnesses keep "a fixed body meeting the impulse" as expected
    to differ: the original ContactImpulseWith reads its stack's leftovers
    for a fixed body.

    The collision world (2026-10-23, the same run): 37 of 43 ours in two
    migrations: the ground and wall queries the collision world calls
    through function pointers, the track mesh's lookups and track
    positions, the world's reset and its teardown. The wall hits are
    reached only by the harness in attract races (the AI cars don't touch
    walls); a drive ran the wall query natively. Left: the world's tick
    (with `car-physics.md`'s CollisionWorld::Update group) and set-up,
    the course load (blocked by Archive::Load and the asset size) and the
    route's two functions. During this run the harnesses gained
    `harness::Outputs` (two NaNs match for values marked floats; the
    user's call: NaN bits aren't a goal), and the validation agent a pad
    replay.

    The sound cues (2026-10-23, branch `b7-loop-23-10-26`, the fifth
    run): 30 of 87 ours in one migration: the sound manager's
    bookkeeping (bank slots, the sounds' accessors, the cue list), the
    listener, the bank holders, the emitter's constructor and the
    library entry point X3DAudioInitialize (pure math). The rest waits
    on XACT: the cue and engine calls go through XACT's vtables into the
    recompiled library, and everything above them (PlayCue, the sounds'
    update and stop, AudioManager::Update) waits on those. The user's
    call (2026-10-23): XACT will probably be translated, as no host
    library plays its data (a lead). The bank loads wait on the file
    layer and std::string; the message handler makes guest vtable calls.

    XACT (2026-10-23, the same run): 55 of 149 ours in one migration:
    the name hash, the node pools, the sound and wave bank readers and
    the bank loads, the constructors, the cue state, the listener lists
    and loop regions. A function calling its own object's vtable slot
    runs natively only while the slot holds an already native function;
    some are ported only for the paths seen in play (a sound without
    tracks, an idle fader, simple cues). Left: the engine's critical
    section (no native lock in `src/`: the core's), the global settings'
    unmapped vtable slots (a lead, taken next), calls through vtables
    into recompiled code, XAudio and its voice class (the
    game's recompiled XAudio, whose mixer already feeds our own output;
    the next layer to translate), the
    tracks, events and waves (a lead). A second map (the run's third
    subsystem: the global settings' accessors, the renderer's and the
    sound classes' slots, the wave entry's getters, two shared helpers
    in `functions.md`) and two more migrations made 102 of 186 ours,
    with the folded getter `sub_8220BC28`. What's left needs the
    critical section, XAudio and the voice class, vtable calls into the
    still-recompiled tracks and waves (`825D0000`-`825D9000`, leads), or
    XACT's clock, which waited on GetTickCount (`sub_822956F0`; ours
    since 2026-10-23, from the host clock: "GetTickCount" under B6).

    The action mappers (2026-10-23, branch `b7-loop-23-10-26-2`, the
    sixth run): 36 of 40 ours in one migration, with no change in the
    game's behavior (the user's call: migrate off the recompiled code
    only): every harness matches exactly, `--input_mappers_by_tick`'s
    override of ActionMappers::Update (`sub_8210B018`) keeps skipping
    render-only passes before the native update, and the pause menu's
    diagnostics moved into the adapters. Left: BindAll and Bind
    (`sub_8210AF08`, `sub_8210B270`), whose calls into the input
    manager's vtables aren't identified, and the mapper's constructor and
    ActionMappers::Init, blocked by the input processor's constructor
    (`sub_8258CE60`, a lead). With `--run_harnesses=all` an attract race
    under `attract_race.sh 25` now counts 19 s at 30 ticks/s (the
    harnesses' time at boot; 23 s without them).

    The data tables (2026-10-23, the same run): 16 of 18 ours in one
    migration (the load callbacks, the entries' byte swaps, the
    accessors); SwapWord's float round trip folds to a plain byte swap
    in the release build, in the translated code and ours alike, so
    signaling NaNs come through unchanged. Left: DataTables::Init and
    Load, blocked by the archive (`sub_8209DE48` Archive::Load,
    `sub_8209D868`, `sub_820A0678`). The run stopped at its validation:
    ItemRecords::Item (`sub_82116DA8`, a thunk) has an adapter calling
    `data_tables::Item` directly and so no print of its own; now a
    one-line `data_tables::RecordsItem` with its own print (the user's
    call; the adapter stays for its two recompiled callers).

    The AI drivers (2026-10-23 to 24, branch `b7-loop-23-10-26-5`, the
    ninth run): mapped from the course's route's users, 143 of 148 ours
    in five migrations, with functions never seen in play ported with
    their callers (UpdateSteerInput's kinds 2 and 3, jump targets the
    recompiler made functions; harness cases each, no rows). Left:
    `AiDrivers::Destroy` and `DestroyRings` (the array destructor
    iterator `sub_820900C0`, native since the eleventh run, so both
    are ready now) and `UpdateStarting`,
    `UpdateTrack` and `NoteFixedSections` (the getter of `[0x826C6078]`,
    `sub_821434E8`, in the data tables' range); `UpdateAll` keeps its
    adapter for the +6148 update targets, and 22 adapters stay for the
    race states' and set-up's recompiled callers. Harness runs now use
    `attract_race.sh 40` (the user's call: the harnesses' time grows
    with their cases, and a race short only for that is no halt).

    The racer controllers (2026-10-24, the same run): all 27 ours in
    one migration, with three functions never seen in play; the race
    flow's handlers keep their adapters (reached through the functors
    `sub_82160AE0` registers, a lead), and the three that only jump to
    another (OnEnd, OnHold, OnState7) got one-line functions with their
    own prints, as `data_tables::RecordsItem`. State 34's handler
    (`sub_8215FC38`), which drives the cars in attract races, was
    missing from `seen_in_play.txt`. The input record's bytes +18 and
    +19 are stack leftovers nothing writes (for the user's review).

    The track zones (2026-10-24, the same run; its "path search" lead
    turned out to be them): all 92 ours in one migration, the 70
    set-ups over a table of their ranges read from `generated/`, each
    built at the original's stack offset. The coverage list misses three
    camera-mode functions that run in attract races (`sub_82155288`,
    `sub_821562F8`, `sub_820FE830`), so the route ranges' Init,
    AddBranch, Update and Test keep their adapters for them.

    The graphics resources, the video output and the device state
    (2026-10-24, branch `b7-loop-24-10-26`, the tenth run): the run
    stopped twice for the user's calls on kernel sync and the graphics
    rule. `Edram::Alloc` is native with a `std::mutex` for its critical
    section; the resource locks' fence waits and cache flushes are
    Dropped (their lock count, address and freeing kept); the video
    output's device lock alone is a `std::binary_semaphore` (released by
    another thread than the one that took it), while the game's
    Semaphore class and sync wrappers stay recompiled for the archive
    loader and the core. The device's state setters write the shadow,
    dirty masks and fetch constants the renderer reads; the packets they
    wrote straight into the command buffer, which nothing reads since
    the emulated GPU went, are Dropped, with harness groups for those
    words expected to differ. The setters keep their adapters: the
    device's tables at +56 and +440 point at them, and the command
    replay reaches them.

    The array helpers, the course scenery and XAPI's memset (2026-10-24,
    branch `b7-loop-24-10-26-2`, the eleventh run). Native code calls
    guest function pointers and vtable slots in one way: targets that
    are already native are called as plain C++ by their address; any
    other target is a guest call through a callback the adapter gives,
    from a frame laid out like the original's. The vector constructor
    and destructor iterators work like this, and so do the scenery
    pools' key, load and reset slots. The slots keep their adapters for
    the vtables. `Scenery::LimitLargeModels` reads past its five-entry
    stack table for an area above 4, so the native version keeps that
    table in guest stack memory to give the same values. `XMemSet` steps
    by the word at `[0x8263830C]`, logged as 16 and stored by nothing in
    `generated/`: it is one host memset at 16, and the translated steps
    at any other stride. `atexit` (`sub_825A7068`) stays with the core
    (its critical sections) and blocks `Viewports::Init` and
    `Viewport::Init` (native since 2026-10-25, the fifteenth run: the
    exit lock a host mutex, with both viewports). The rest of the scenery waits on
    `MmQueryAllocationSize`'s wrapper, `DataFile::GetEntry`, the XPR,
    the archive's queue and the texture provider. Harnesses then drew
    from one shared `h.random`, so adding cases shifted the inputs of
    every harness after them, and the `swprintf` harness crashed. Since
    2026-10-24 each harness starts its own generator, seeded from its
    name, and the crash was the harness's format generator
    (`strings-and-font.md`).

    The second run (from 2026-10-22, branch `b7-loop-22-10-26`; its first
    session stopped after one subsystem): the first run's removable
    adapters went (31), and the race overlays were made native as far as
    they were ready. Each native function now logs `native running
    (game|harness)` once (`ITC_NATIVE_RUNNING`, `src/base/log.h`), and a
    validation agent checks that every function made native has a harness
    line; work done inside a native caller is the new status Merged. What
    blocks the rest of the race overlays: guest vtable calls, guest
    allocation (the creation side), memcpy's mirror recording (the
    `sub_825A7410` override has no plain C++ entry), and RaceOverlays::Run.

## The end state: removing the core

Only when no recompiled function is left (or none that touches what's
being removed):

E1. **Threads and sync** (291 boundary functions, plus every recompiled
    function using the game's locks): host threads and locks in native
    code. The graphics library's per-CPU workers read the CPU number from
    the PCR (+268; "Interrupts are made at the library's level" in
    `docs/native-renderer.md`), and timing has mattered before (the
    deferred-kick race there).

E2. **Memory**: native allocations once nothing uses guest addresses.
    No guest mapping, no `/dev/shm` files.

E3. **The image load**: the recompiled code reads strings, constants,
    vtables, tables and initialized globals from the image at
    `0x82000000`, which today comes from the player's `default.xex`
    (encrypted, compressed; `Runtime::LoadXexImage`). This isn't a step
    of its own: native code carries its data as literals and arrays,
    compiled into the exe (B7). Once no recompiled code reads the image,
    the load and `default.xex` are no longer needed; the player's `DATA/`
    still is.

A4 (the app shell, deferred 2026-10-21) comes here, once nothing needs
the guest: our own `main`, loop, window and flag parser, written once
against what's left instead of around the SDK's `Runtime` start-up.

E4. **`PPCContext`, `rex/ppc.h`, the function table, `generated/`,
    and the SDK**: delete the submodule, `patches/rexglue-sdk/`,
    `generated/rexglue.cmake`, `librexruntime` from packaging, both CI workflows and CLAUDE.md (the build line's
    `LD_LIBRARY_PATH`, packaging, the note on the submodule showing
    modified). The `rexglue codegen` target goes with it. The SDK's
    graphics (`src/graphics/`, the `Vd*` and `XGetVideoMode` exports),
    unused since B3, go with it.

## Risks

- Saves (B4): players' saves must keep loading.
- Behavior drift: a native function that differs subtly from the
  recompiled one (float rounding, big-endian layout, order of side
  effects). Compare against the recompiled version with a fallback flag
  before it's removed.
- Scale (B7): most of the work is the game logic, not the boundary.
  Track A and B1-B6 are worth doing on their own even if B7 goes slowly.

## The boundary over time

`itc_recomp/tools/boundary_status.py` after each plan item (functions
calling a kernel import directly; a function calling imports of several
areas counts in each area). "Live" is what's still called by recompiled
code.

**2026-10-21, after B5 (branch `system-services`; A0, A5, B1, B3, B5 and
B6 done):** 449 functions (916 function-import pairs): overridden 48,
Native 3, Dropped 6, never called 54, live 338. Imports: 186 called, 97
with a live caller.

| Area | Overridden | Native | Dropped | Never called | Live |
|---|---|---|---|---|---|
| input (B1) | 0 | 3 | 0 | 0 | 0 |
| audio (B2) | 3 | 0 | 0 | 0 | 3 |
| graphics (B3) | 12 | 0 | 3 | 4 | 0 |
| files and saves (B4) | 1 | 0 | 0 | 1 | 21 |
| memory (core) | 2 | 0 | 0 | 3 | 23 |
| system services (B5) | 17 | 0 | 3 | 48 | 12 |
| pure helpers (B6) | 18 | 0 | 0 | 0 | 31 |
| threads, sync and the rest (core) | 6 | 0 | 1 | 16 | 271 |

What's live: the core (threads, sync, memory: E1, E2, after B7), B4's
file and save functions, B2's audio, B5's leftovers (the notification
readers, voice and the system link code, with B7; the device selector
and XEnumerate, with B4) and B6's callers waiting on those areas.
Overall, 232 overrides in `src/` of 16,552 recompiled functions.

**2026-10-21, B4 step 1, the saves (branch `files-and-saves`; B4 in
progress):** 449 functions (916 pairs): overridden 49, Native 3, Dropped
13, never called 54, live 330. Imports: 186 called, 89 with a live
caller. Files and saves (B4): overridden 2, Dropped 7, never called 1,
live 13 (the game's data files, the debug checks, the C runtime's I/O
and what's never called but not yet marked). The other areas unchanged.
245 overrides in `src/`.

**2026-10-21, B4 step 2, the game's data files (branch
`files-and-saves`; B4 in progress):** 449 functions (916 pairs):
overridden 49, Native 3, Dropped 15, never called 54, live 328. Imports:
186 called, 88 with a live caller. Files and saves (B4): overridden 2,
Dropped 8, never called 1, live 12 (the movie player's file calls, the
debug checks and what's never called in play but not yet marked). The
other areas unchanged. 251 overrides in `src/`.

**2026-10-21, B4: the functions never called in play marked (branch
`files-and-saves`; B4 in progress):** 449 functions (916 pairs):
overridden 49, Native 3, Dropped 17, never called 61, live 319. Imports:
186 called, 81 with a live caller. Files and saves (B4): overridden 2,
Dropped 10 (WriteFile and SetFilePointer added: their callers in play are
native), never called 8 (the debug capture's file functions,
GetDiskFreeSpaceEx, SetEndOfFile, DeleteFile, GetFileTime,
FlushFileBuffers, XContentGetCreator: none called in nine sessions), live
3 (ReadFile and GetFileSizeEx, the movie player's; GetFileAttributes, the
graphics library's debug check). The other areas unchanged.

**2026-10-21, after B4 (branch `files-and-saves`; A0, A5, B1, B3, B4, B5
and B6 done):** 449 functions (916 pairs): overridden 49, Native 3,
Dropped 20, never called 61, live 316. Imports: 186 called, 77 with a
live caller.

| Area | Overridden | Native | Dropped | Never called | Live |
|---|---|---|---|---|---|
| input (B1) | 0 | 3 | 0 | 0 | 0 |
| audio (B2) | 3 | 0 | 0 | 0 | 3 |
| graphics (B3) | 12 | 0 | 3 | 4 | 0 |
| files and saves (B4) | 2 | 0 | 13 | 8 | 0 |
| memory (core) | 2 | 0 | 0 | 3 | 23 |
| system services (B5) | 17 | 0 | 3 | 48 | 12 |
| pure helpers (B6) | 18 | 0 | 2 | 3 | 26 |
| threads, sync and the rest (core) | 6 | 0 | 3 | 18 | 267 |

Files and saves: ReadFile, GetFileSizeEx and GetFileAttributes Dropped
(their callers in play native or overridden without them), none live.
Some functions also call imports of other areas (the pure helpers' and
the core's counts move with them). 256 overrides in `src/`.

**2026-10-21, after B2 (branch `native-xma`; A0, A5, A6, B1, B2, B3, B4,
B5 and B6 done):** 449 functions (916 pairs): overridden 54, Native 3,
Dropped 20, never called 62, live 310. Imports: 186 called, 72 with a
live caller.

| Area | Overridden | Native | Dropped | Never called | Live |
|---|---|---|---|---|---|
| input (B1) | 0 | 3 | 0 | 0 | 0 |
| audio (B2) | 6 | 0 | 0 | 0 | 0 |
| graphics (B3) | 12 | 0 | 3 | 4 | 0 |
| files and saves (B4) | 2 | 0 | 13 | 8 | 0 |
| memory (core) | 5 | 0 | 0 | 3 | 20 |
| system services (B5) | 17 | 0 | 3 | 48 | 12 |
| pure helpers (B6) | 18 | 0 | 2 | 3 | 26 |
| threads, sync and the rest (core) | 7 | 0 | 3 | 19 | 265 |

Audio: none live. The XMA helpers calling `MmGetPhysicalAddress`
(`sub_8245C650`, `sub_8245C958`, `sub_8245D138`) are ours, so memory and
the core's counts moved with them. 282 overrides in `src/`. Outside the
core, what's live is B5's and B6's leftovers waiting on B7 and the core.

**2026-10-21, B5 and B6 leftovers, batch 1 (branch `b5-b6-leftovers`):**
449 functions (916 pairs): overridden 58, Native 3, Dropped 21, never
called 70, live 297. Imports: 186 called, 64 with a live caller.

| Area | Overridden | Native | Dropped | Never called | Live |
|---|---|---|---|---|---|
| input (B1) | 0 | 3 | 0 | 0 | 0 |
| audio (B2) | 6 | 0 | 0 | 0 | 0 |
| graphics (B3) | 12 | 0 | 3 | 4 | 0 |
| files and saves (B4) | 2 | 0 | 13 | 8 | 0 |
| memory (core) | 6 | 0 | 0 | 4 | 18 |
| system services (B5) | 20 | 0 | 4 | 51 | 5 |
| pure helpers (B6) | 18 | 0 | 2 | 7 | 22 |
| threads, sync and the rest (core) | 7 | 0 | 4 | 19 | 264 |

Cleared: the three socket imports, `XamShowSigninUI`, `sprintf`,
`MmQueryStatistics`, `XamAlloc` and `XamFree`. Live outside the core:
Network::Update and SignIn::Update (`XNotifyGetNext`, batch 2), Voice::Init
and its talker set-up (B7), `xstart` and the movie player's stream set-up
(`DbgPrint`). 287 overrides in `src/`.

**2026-10-21, B5 and B6 leftovers, batch 2: Network::Update stubbed
(branch `b5-b6-leftovers`):** 449 functions (916 pairs): overridden 58,
Native 3, Dropped 23, never called 70, live 295. Imports: 186 called, 63
with a live caller.

| Area | Overridden | Native | Dropped | Never called | Live |
|---|---|---|---|---|---|
| input (B1) | 0 | 3 | 0 | 0 | 0 |
| audio (B2) | 6 | 0 | 0 | 0 | 0 |
| graphics (B3) | 12 | 0 | 3 | 4 | 0 |
| files and saves (B4) | 2 | 0 | 13 | 8 | 0 |
| memory (core) | 6 | 0 | 0 | 4 | 18 |
| system services (B5) | 20 | 0 | 6 | 51 | 3 |
| pure helpers (B6) | 18 | 0 | 2 | 7 | 22 |
| threads, sync and the rest (core) | 7 | 0 | 4 | 19 | 264 |

Cleared: `XNotifyGetNext`. Live outside the core: Voice::Init and its
talker set-up (`XamNotifyCreateListener`, `XamUserReadProfileSettings`,
B7), `xstart` (`XamLoaderTerminateTitle`, `DbgPrint`) and the movie
player's stream set-up (`DbgPrint`). 287 overrides in `src/`.

**2026-10-21, A2: our own logging, flags and hashes (branch
`own-logging`):** unchanged, as A2 is host-side: 449 functions (916
pairs): overridden 58, Native 3, Dropped 23, never called 70, live 295.
Imports: 186 called, 63 with a live caller. The areas' counts are as
after B5's and B6's leftovers, batch 2.

**2026-10-22, coverage: B7's measure (branch `coverage`):** the boundary
unchanged: 449 functions (916 pairs): overridden 58, Native 3, Dropped
23, never called 70, live 295. Imports: 186 called, 63 with a live
caller. B7's first count (`itc_recomp/tools/b7_status.py`, after the
user's first coverage session; `docs/coverage.md`): of 16,315 `sub_`
functions, 6,591 seen in play, 6,752 in play (seen, or ours): ours 162,
work left 6,590 (Wrapped 151, with a row 174, no row yet 6,265). From
here, record `b7_status.py`'s totals beside the boundary's.

**2026-10-22, B7's first loop run (branch `b7-loop-test`):** the
boundary unchanged, as the five subsystems were all inside it: 449
functions (916 pairs): overridden 58, Native 3, Dropped 23, never called
70, live 295. Imports: 186 called, 63 with a live caller. B7
(`b7_status.py`): 6,753 in play, ours 335, work left 6,418 (Wrapped 142,
with a row 243, no row yet 6,033); 450 overrides in `src/`.

**2026-10-22, B7's second loop run, first session (branch
`b7-loop-22-10-26`: the adapters removed, the race overlays):** the
boundary unchanged: 449 functions (916 pairs): overridden 58, Native 3,
Dropped 23, never called 70, live 295. Imports: 186 called, 63 with a
live caller. B7 (`b7_status.py`): 6,753 in play, ours 422, work left
6,331 (Wrapped 142, with a row 313, no row yet 5,876); 495 overrides in
`src/`.

**2026-10-22, B7's third loop run (branch `b7-loop-22-10-26`: the
vehicle accessors):** the boundary unchanged: 449 functions (916 pairs):
overridden 58, Native 3, Dropped 23, never called 70, live 295. Imports:
186 called, 63 with a live caller. B7 (`b7_status.py`): 6,753 in play,
ours 549, work left 6,204 (Wrapped 140, with a row 317, no row yet
5,747); 618 overrides in `src/`.

**2026-10-22, B7's third loop run (branch `b7-loop-22-10-26`: the race
overlays' functions the accessors made ready, the matrix math):** the
boundary unchanged: 449 functions (916 pairs): overridden 58, Native 3,
Dropped 23, never called 70, live 295. Imports: 186 called, 63 with a
live caller. B7 (`b7_status.py`): 6,753 in play, ours 591, work left
6,162 (Wrapped 137, with a row 302, no row yet 5,723); 641 overrides in
`src/`.

**2026-10-22, B7's third loop run, done (branch `b7-loop-22-10-26`: the
car physics):** the boundary unchanged: 449 functions (916 pairs):
overridden 58, Native 3, Dropped 23, never called 70, live 295. Imports:
186 called, 63 with a live caller. B7 (`b7_status.py`): 6,753 in play,
ours 705, work left 6,048 (Wrapped 137, with a row 318, no row yet
5,593); 694 overrides in `src/`.

**2026-10-22, B7's first step, native memory (branch `native-heap`):**
the nine allocators routed to the native allocation layer
(`docs/native-memory.md`): 449 functions (916 pairs): overridden 67,
Native 3, Dropped 23, never called 70, live 286. Imports: 186 called, 62
with a live caller.

| Area | Overridden | Native | Dropped | Never called | Live |
|---|---|---|---|---|---|
| input (B1) | 0 | 3 | 0 | 0 | 0 |
| audio (B2) | 6 | 0 | 0 | 0 | 0 |
| graphics (B3) | 12 | 0 | 3 | 4 | 0 |
| files and saves (B4) | 2 | 0 | 13 | 8 | 0 |
| memory (core) | 14 | 0 | 0 | 4 | 10 |
| system services (B5) | 20 | 0 | 6 | 51 | 3 |
| pure helpers (B6) | 22 | 0 | 2 | 7 | 18 |
| threads, sync and the rest (core) | 11 | 0 | 4 | 19 | 260 |

Cleared: `KeBugCheckEx` (the heap's own checks, in HeapAlloc, HeapFree,
HeapReAlloc and HeapSize). B7 (`b7_status.py`): 6,753 in play, ours 714,
work left 6,039 (Wrapped 137, with a row 336, no row yet 5,566); the new
`heap.md` has 9 ours of 30 in play. 703 overrides in `src/`.

**2026-10-22, B7's fourth loop run, the C runtime heap (branch
`b7-loop-22-10-26-2`):** the boundary unchanged: 449 functions (916
pairs): overridden 67, Native 3, Dropped 23, never called 70, live 286.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,753
in play, ours 729, work left 6,024 (Wrapped 137, with a row 321, no row
yet 5,566); `heap.md` 24 ours of 30. 715 overrides in `src/`.

**2026-10-23, B7's fourth loop run, the collision narrow phase (branch
`b7-loop-22-10-26-2`):** the boundary unchanged: 449 functions (916
pairs): overridden 67, Native 3, Dropped 23, never called 70, live 286.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,753
in play, ours 797, work left 5,956 (Wrapped 137, with a row 319, no row
yet 5,500); `collision.md` 64 ours of 64. 747 overrides in `src/`.

**2026-10-23, B7's fourth loop run, done (branch `b7-loop-22-10-26-2`:
the C runtime heap, the collision narrow phase, the collision world):**
the boundary unchanged: 449 functions (916 pairs): overridden 67, Native
3, Dropped 23, never called 70, live 286. Imports: 186 called, 62 with a
live caller. B7 (`b7_status.py`): 6,753 in play, ours 837, work left
5,916 (Wrapped 137, with a row 322, no row yet 5,457); `collision-world.md`
37 ours of 43. 764 overrides in `src/`.

**2026-10-23, B7's fifth loop run, the sound cues (branch
`b7-loop-23-10-26`):** the boundary unchanged: 449 functions (916
pairs): overridden 67, Native 3, Dropped 23, never called 70, live 286.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,753
in play, ours 867, work left 5,886 (Wrapped 137, with a row 375, no row
yet 5,374); `sound-cues.md` 30 ours of 87. 787 overrides in `src/`.

**2026-10-23, B7's fifth loop run, XACT (branch `b7-loop-23-10-26`):**
the boundary unchanged: 449 functions (916 pairs): overridden 67,
Native 3, Dropped 23, never called 70, live 286. Imports: 186 called, 62
with a live caller. B7 (`b7_status.py`): 6,753 in play, ours 922, work
left 5,831 (Wrapped 137, with a row 469, no row yet 5,225); `xact.md`
55 ours of 149. 833 overrides in `src/`.

**2026-10-23, B7's fifth loop run, done (branch `b7-loop-23-10-26`: the
sound cues, XACT, XACT's settings and slots):** the boundary unchanged:
449 functions (916 pairs): overridden 67, Native 3, Dropped 23, never
called 70, live 286. Imports: 186 called, 62 with a live caller. B7
(`b7_status.py`): 6,753 in play, ours 970, work left 5,783 (Wrapped
137, with a row 460, no row yet 5,186); `xact.md` 102 ours of 186. 865
overrides in `src/`.

**2026-10-23, GetTickCount from the host clock (branch
`native-tick-count`):** the boundary unchanged (the tool counts function
imports; `KeTimeStampBundle` is a variable import, with no reader left
in play): 449 functions (916 pairs): overridden 67, Native 3, Dropped
23, never called 70, live 286. Imports: 186 called, 62 with a live
caller. B7 (`b7_status.py`): ours 971.

**2026-10-23, B7's sixth loop run, the action mappers (branch
`b7-loop-23-10-26-2`):** the boundary unchanged: 449 functions (916
pairs): overridden 67, Native 3, Dropped 23, never called 70, live 286.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,753
in play, ours 1,007, work left 5,746 (Wrapped 129, with a row 456, no
row yet 5,161); `action-mappers.md` 36 ours of 40. 883 overrides in
`src/`.

**2026-10-23, B7's sixth loop run, stopped (branch `b7-loop-23-10-26-2`:
the action mappers, the data tables):** the boundary unchanged: 449
functions (916 pairs): overridden 67, Native 3, Dropped 23, never called
70, live 286. Imports: 186 called, 62 with a live caller. B7
(`b7_status.py`): 6,753 in play, ours 1,023, work left 5,730 (Wrapped
129, with a row 458, no row yet 5,143); `data-tables.md` 16 ours of 18.
895 overrides in `src/`.

**2026-10-23, B7's seventh loop run, the model animation (branch
`b7-loop-23-10-26-3`):** the boundary unchanged: 449 functions (916
pairs): overridden 67, Native 3, Dropped 23, never called 70, live 286.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,756
in play, ours 1,060, work left 5,696 (Wrapped 129, with a row 588, no
row yet 4,979); `model-animation.md` 36 ours of 39. 916 overrides in
`src/`. The model animation's sections with tracks in `Setup208` and
node types 8 and 16 stay on the recompiled code (never in play); the
primitive drawer's methods wait on the graphics device.

**2026-10-23, B7's seventh loop run, the car specs (branch
`b7-loop-23-10-26-3`):** the boundary unchanged: 449 functions (916
pairs): overridden 67, Native 3, Dropped 23, never called 70, live 286.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,756
in play, ours 1,107, work left 5,649 (Wrapped 129, with a row 541, no
row yet 4,979); `car-specs.md` 47 ours of 48. 929 overrides in `src/`.
`Params::Load` waits on the archive (its twelve `Archive::Load` calls);
the archive's TOC lookup `sub_8209D868` stays a guest call from the
adapters.

**2026-10-23, B7's seventh loop run, stopped (branch
`b7-loop-23-10-26-3`: the model animation, the car specs, the
cameras):** the boundary unchanged: 449 functions (916 pairs):
overridden 67, Native 3, Dropped 23, never called 70, live 286.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,756
in play, ours 1,139, work left 5,617 (Wrapped 123, with a row 551, no
row yet 4,943); `cameras.md` 32 ours of 42. 949 overrides in `src/`.
The cameras' rest waits on the graphics device's resource calls
(`CameraPass::Init`, `SetUpSurfaces`, `CameraPasses::MakeRenderTarget`)
and memset and atexit (`Viewports::Init`, `Viewport::Init`).

**2026-10-23, B7's eighth loop run, the string table and font (branch
`b7-loop-23-10-26-4`):** the boundary unchanged: 449 functions (916
pairs): overridden 67, Native 3, Dropped 23, never called 70, live 286.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,757
in play, ours 1,167, work left 5,590 (Wrapped 123, with a row 559, no
row yet 4,908); `strings-and-font.md` 25 ours of 36. 962 overrides in
`src/`. The game's `swprintf` runs over a step-by-step translation of
the C runtime's `_woutput_l` (`src/c_runtime/wide_format.cpp`), which
hands back to the recompiled code on paths never seen in play. The rest
waits on `DataFile::GetEntry` (`Strings::Get`, so `FormatV` and
`Format`), the archive (`Font::Load`, `Strings::Load`), the XPR, the
shader compiler and the device's state save and restore (`Font::Font`).

**2026-10-23, B7's eighth loop run, `std::string` (branch
`b7-loop-23-10-26-4`):** the boundary unchanged: 449 functions (916
pairs): overridden 67, Native 3, Dropped 23, never called 70, live 286.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,757
in play, ours 1,181, work left 5,576 (Wrapped 123, with a row 561, no
row yet 4,892); `std-string.md` 14 ours of 16. 971 overrides in
`src/`. The string's member functions are translated over the guest
layout (the user's call: the game's inlined code reads its fields, so
host strings can't stand in; thrown away at the port's end); the two
global strings' static set-ups wait on `atexit`.

**2026-10-23, B7's eighth loop run, stopped (branch
`b7-loop-23-10-26-4`: the string table and font, `std::string`, the
course's route):** the boundary unchanged: 449 functions (916 pairs):
overridden 67, Native 3, Dropped 23, never called 70, live 286.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,757
in play, ours 1,223, work left 5,534 (Wrapped 123, with a row 559, no
row yet 4,852); `track-route.md` 40 ours of 40, `collision-world.md`
39 of 43. 986 overrides in `src/`. Follow's full search
(`sub_820FB2B0`, never seen in play) is ported, so Follow and
`TrackPosition::Update` need no `ctx`; 18 of the route's adapters stay
for the race and vehicle code's recompiled callers. Harness runs now
use `attract_race.sh 30` (the harnesses take ~5 s before the race).

**2026-10-24, B7's ninth loop run, the AI drivers (branch
`b7-loop-23-10-26-5`):** the boundary unchanged: 449 functions (916
pairs): overridden 67, Native 3, Dropped 23, never called 70, live 286.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,757
in play, ours 1,366, work left 5,391 (Wrapped 123, with a row 562, no
row yet 4,706); `ai-drivers.md` 143 ours of 148 (119 Native). 1,010
overrides in `src/`. The rest waits on `sub_820900C0` and
`sub_821434E8`.

**2026-10-24, B7's ninth loop run, the racer controllers (branch
`b7-loop-23-10-26-5`):** the boundary unchanged: 449 functions (916
pairs): overridden 67, Native 3, Dropped 23, never called 70, live 286.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,758
in play, ours 1,393, work left 5,365 (Wrapped 123, with a row 559, no
row yet 4,683); `racer-controllers.md` 27 ours of 27. 1,020 overrides
in `src/`.

**2026-10-24, B7's ninth loop run, stopped (branch
`b7-loop-23-10-26-5`: the AI drivers, the racer controllers, the track
zones):** the boundary unchanged: 449 functions (916 pairs): overridden
67, Native 3, Dropped 23, never called 70, live 286. Imports: 186
called, 62 with a live caller. B7 (`b7_status.py`): 6,758 in play, ours
1,485, work left 5,273 (Wrapped 123, with a row 559, no row yet 4,591);
`track-zones.md` 92 ours of 92. 1,103 overrides in `src/`. What blocks
the rest of the run's subsystems: the array destructor iterator
(`sub_820900C0`), the getter of `[0x826C6078]` (`sub_821434E8`), the
race flow's functors (`sub_82160AE0`) and the camera modes (leads).

**2026-10-24, B7's tenth loop run, the graphics resources (branch
`b7-loop-24-10-26`):** 449 functions (916 pairs): overridden 67, Native
5 (from 3), Dropped 23, never called 70, live 284 (from 286):
`Edram::Alloc`'s critical section calls, replaced by a `std::mutex`
(the user's call). Imports: 186 called, 62 with a live caller. B7
(`b7_status.py`): 6,760 in play, ours 1,529, work left 5,231 (Wrapped
123, with a row 554, no row yet 4,554); `graphics-resources.md` 46
ours of 46. 1,127 overrides in `src/`. `Resource::Lock`'s, `Unlock`'s
and `Destroy`'s fence waits and cache flushes are left out (the user's
call); adapters stay for callers in other files still recompiled
(`Device::Resolve`, the DXT compressor, the game's direct calls).

**2026-10-24, B7's tenth loop run, the video output and the device lock
(branch `b7-loop-24-10-26`):** the boundary unchanged: 449 functions
(916 pairs): overridden 67, Native 5, Dropped 23, never called 70, live
284. Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`):
6,760 in play, ours 1,541, work left 5,219 (Wrapped 123, with a row 566,
no row yet 4,530); `video-output.md` 7 ours of 30 (the rest the game's
sync wrappers, kept recompiled for the core by the user's call, and what
waits on the device's state setters), `cameras.md` 38 of 42. 1,136
overrides in `src/`.

**2026-10-24, B7's tenth loop run, stopped (branch `b7-loop-24-10-26`:
the graphics resources, the video output and the device lock, the
device state):** the boundary unchanged: 449 functions (916 pairs):
overridden 67, Native 5, Dropped 23, never called 70, live 284.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,760
in play, ours 1,672, work left 5,088 (Wrapped 123, with a row 555, no
row yet 4,410); `device-state.md` 131 ours of 131. 1,258 overrides in
`src/`. The setters' adapters stay (the device's tables at +56 and +440
point at them, and the command replay reaches them); what the video
output's rest waits on is now the command replay and the game's sync
wrappers (kept recompiled for the core).

**2026-10-24, B7's eleventh loop run, the compiler's array helpers
(branch `b7-loop-24-10-26-2`):** the boundary unchanged: 449 functions
(916 pairs): overridden 67, Native 5, Dropped 23, never called 70, live
284. Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`):
6,760 in play, ours 1,674, work left 5,086 (Wrapped 123, with a row 555,
no row yet 4,408); `c-runtime.md` 84 ours of 91. 1,257 overrides in
`src/`. The array destructor iterator, which blocked the AI drivers'
and the race overlays' rest, is native; its callers' other blockers
remain (leads).

**2026-10-24, B7's eleventh loop run, the course scenery (branch
`b7-loop-24-10-26-2`):** the boundary unchanged: 449 functions (916
pairs): overridden 67, Native 5, Dropped 23, never called 70, live 284.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,760
in play, ours 1,710, work left 5,050 (Wrapped 122, with a row 588, no
row yet 4,340); `course-scenery.md` 37 ours of 73. 1,284 overrides in
`src/`. The rest of the scenery waits on the critical section
wrappers, `MmQueryAllocationSize`'s wrapper, memset, `DataFile::GetEntry`,
the XPR, the archive's queue and the texture provider; the pools' slots
keep their adapters for the vtables.

**2026-10-24, B7's eleventh loop run, stopped (branch
`b7-loop-24-10-26-2`: the array helpers, the course scenery, XAPI's
memset):** the boundary unchanged: 449 functions (916 pairs):
overridden 67, Native 5, Dropped 23, never called 70, live 284.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,760
in play, ours 1,714, work left 5,046 (Wrapped 122, with a row 587, no
row yet 4,337); `c-runtime.md` 85 ours of 92, `cameras.md` 40 of 42,
`course-scenery.md` 38 of 73. 1,286 overrides in `src/`. `Viewports::Init`
and `Viewport::Init` wait on `atexit` (its critical sections, with the
core).

**2026-10-24, B7's twelfth loop run, the post effects (branch
`b7-loop-24-10-26-3`):** the boundary unchanged: 449 functions (916
pairs): overridden 67, Native 5, Dropped 23, never called 70, live 284.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,760
in play, ours 1,757, work left 5,003 (Wrapped 122, with a row 598, no
row yet 4,283); `post-effects.md` 44 ours of 57. 1,320 overrides in
`src/`. The rest waits on the models' resource relocation
(`sub_8235A9F8`) and shader pairs (`sub_825896D8`, `sub_82589700`), the
vertex declaration (`sub_822A8980`) and `sub_8209AE48`.

**2026-10-24, B7's twelfth loop run, stopped (branch
`b7-loop-24-10-26-3`: the post effects, three cleanup leads, the input
manager):** the boundary unchanged: 449 functions (916 pairs):
overridden 67, Native 5, Dropped 23, never called 70, live 284.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,761
in play, ours 1,794, work left 4,967 (Wrapped 115, with a row 585, no
row yet 4,267); `input.md` 29 ours of 29, `action-mappers.md` 39 of 40,
`post-effects.md` 44 of 57. 1,325 overrides in `src/`. Stopped on two
input functions whose harness doesn't reach their prints; the input's
`ActionMappers::Init` is ready.

**2026-10-24, B7's thirteenth loop run, the screen objects (branch
`b7-loop-24-10-26-4`; with `ActionMappers::Init` and three cleanup
leads before it):** the boundary unchanged: 449 functions (916 pairs):
overridden 67, Native 5, Dropped 23, never called 70, live 284.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,761
in play, ours 1,838, work left 4,923 (Wrapped 115, with a row 582, no
row yet 4,226); `screen-objects.md` 38 ours of 42, `action-mappers.md`
40 of 40. 1,323 overrides in `src/`. The screen objects' rest waits on
`rand` (`sub_825A9BB8`, its thread data through the core) and the
shader pairs (`sub_825896D8`, `sub_82589700`).

**2026-10-25, the hardcoded shaders and a new coverage list (branch
`b7-loop-24-10-26-4`):** the game's HLSL compiler (`sub_822B8F10`) no
longer runs: its 14 compiles at boot are answered with the shaders they
made (`--use_hardcoded_shaders`, `src/hooks/shader_compiles.cpp`). The
boundary unchanged (the compiler calls no imports): 449 functions (916
pairs): overridden 67, Native 5, Dropped 23, never called 70, live 284.
Imports: 186 called, 62 with a live caller. `seen_in_play.txt` made
again from the user's second session ("Running a session" in
`docs/coverage.md`), so the compiler's ~640 functions left it. B7
(`b7_status.py`): 7,039 in play, ours 1,839, work left 5,200 (Wrapped
117, with a row 644, no row yet 4,439): more than before, from the 917
functions the second session reached that the first hadn't.

**2026-10-25, the movies through our FFmpeg and a new coverage list
(branch `b7-loop-24-10-26-4`):** the movie library linked into the game
(`82481A70`-`82575B07`, ~580 functions seen in play) no longer runs: the
movie class's slots play the movies through our own FFmpeg and our
renderer draws them (`--play_movies_with_ffmpeg`, `docs/native-movies.md`;
the boot movie shows in our window for the first time; the user: "movie
playback looks perfect"). The boundary unchanged: 449 functions (916
pairs): overridden 67, Native 5, Dropped 23, never called 70, live 284.
Imports: 186 called, 62 with a live caller. `seen_in_play.txt` made again
from the user's third session (`docs/coverage.md`). B7 (`b7_status.py`):
6,451 in play, ours 1,847, work left 4,604 (Wrapped 113, with a row 651,
no row yet 3,840), from 5,200.

**2026-10-25, the app framework (branch `by-loop-25-10-26`):** the B7
loop's fourteenth run made the app framework native as far as it was
ready (`framework.md`, 56 of 68: the run loop, its maps and request
vector, the dispatch, the constructors, `main` and `App::Run`; 22
adapters removed; `--native_framework`), after fixing an `ai_drivers`
harness case that read a wild address since the hardcoded shaders moved
the harness's block (dbc8ee0). The boundary unchanged: 449 functions
(916 pairs): overridden 67, Native 5, Dropped 23, never called 70, live
284. Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`):
6,451 in play, ours 1,903, work left 4,548 (Wrapped 111, with a row 597,
no row yet 3,840), from 4,604. The rest of the framework waits on
`Framework::Init`'s services (the worker pools' threads, events and
critical sections, the renderer, the scene, the app), `sub_824936E8`
(`files-and-saves.md`) and `sub_8258E1D8`'s critical section.

**2026-10-25, the game modes (branch `b7-loop-25-10-26-2`):** the B7
loop's fifteenth run made the game modes native as far as they were
ready (`game-modes.md`, 21 of 31: the race mode's state machines, the
race overlays' machine and handlers, the race mode's members and 28-byte
buffers; `--native_game_modes`). `RaceOverlaysMachine::Init` replays the
original's stores in the same stack frame, so the transitions keep the
same unwritten stack words. The boundary unchanged: 449 functions (916
pairs): overridden 67, Native 5, Dropped 23, never called 70, live 284.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,451
in play, ours 1,923, work left 4,528 (Wrapped 111, with a row 605, no
row yet 3,812), from 4,548. The rest waits on `XPR::XPR` (`models.md`,
a leaf blocking the race mode's constructor), the race mode's other
set-ups (leads), the other modes' constructors and `Modes::Tick`'s
callees.

**2026-10-25, the model class (branch `b7-loop-25-10-26-2`):** the
fifteenth run made the model class native (`models.md`, all 17 new
rows: `Model::Model`, `Release`, `~Model`, the texture provider, the
XPR's constructor and teardown, the put-off frees; `memmove_s` in
`c-runtime.md`; `--native_models`). The critical section at
`0x8296BD38` (the memory blocks' pending frees) is a host
`std::recursive_mutex` (the user's call, 2026-10-25), taken by the
native frees and, through `ScopedLock` overridden for this critical
section only, by its four recompiled users; the generic wrappers stay
the game's. The boundary unchanged: 449 functions (916 pairs):
overridden 67, Native 5, Dropped 23, never called 70, live 284.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): 6,451
in play, ours 1,944, work left 4,507 (Wrapped 111, with a row 601, no
row yet 3,795), from 4,528. It makes the game modes' `sub_821067A0` and
`sub_82106AF0` ready.

**2026-10-25, the game modes' rest (branch `b7-loop-25-10-26-2`):**
after the model class, the race mode's constructor (`RaceMode::RaceMode`
with its members) and `Mode::Create` with the mode constructors became
ready and were made native (`game-modes.md`, 38 of 42; 6 more adapters
removed). The boundary unchanged: 449 functions (916 pairs). B7
(`b7_status.py`): ours 1,961, work left 4,490 (Wrapped 111, with a row
595, no row yet 3,784), from 4,507. Left: `Modes::Tick`, `RaceMode::Init`'s
set-ups and `RaceMode::~RaceMode` (`RaceOverlays::Destroy`'s chain). The
pad replay works again (d83677c: it waited for the movie library's draw,
gone since the movies play through our FFmpeg).

**2026-10-25, the mode tick (branch `b7-loop-25-10-26-2`):** the
fifteenth run made what `Modes::Tick` calls besides the mode's slots
native (`mode-tick.md`, 21 of 23: the per-mode settings, the sign-in
watch, the ring flip, the screen fades, the menu music; presence
Dropped; `--native_mode_tick`), with XAPI's `lstrcmpA` and
`CompareStringA` as plain C++ in `src/helpers/` (B6's call). The
boundary unchanged: 449 functions (916 pairs): overridden 67, Native 5,
Dropped 23, never called 70, live 284. Imports: 186 called, 62 with a
live caller. B7 (`b7_status.py`): ours 1,983, work left 4,468 (Wrapped
111, with a row 597, no row yet 3,760), from 4,490. Left:
`SignInWatch::OnModeChange` (the save data's creation and deletion, a
lead), `SignInWatch::StaticInit` (`atexit`), and so `Modes::Tick`.

**2026-10-25, the minimap (branch `b7-loop-25-10-26-2`):** the
fifteenth run made the minimap's parts native (`minimap.md`, 12 of 15;
`--native_minimap`), keeping frame interpolation's `BlendState_OnMultiply`
and `OnCopy` calls (with the original return addresses) and
`Minimap::Draw`'s `--per_tick_minimap_blink` wrap; its frame dump is
pixel-identical with the fallback. The boundary unchanged: 449 functions
(916 pairs): overridden 67, Native 5, Dropped 23, never called 70, live
284. Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`):
ours 1,995, work left 4,456 (Wrapped 111, with a row 599, no row yet
3,746), from 4,468. The marker draws and `Minimap::Draw` wait on
`atexit` (next, by the user's go-ahead: the exit lock a host mutex) and
the course set's lists.

**2026-10-25, `atexit` and what it unblocked (branch
`b7-loop-25-10-26-2`):** the fifteenth run's last step (the user's
go-ahead, counted as its fifth subsystem) made `atexit`, `_onexit` and
`__onexitinit` native over the game's exit table, the C runtime's exit
lock (lock 8, critical section `0x82971248`) a host
`std::recursive_mutex` taken by every user (`_lock`/`_unlock`
overridden for lock 8 only; its other users, the exit path, never run:
closing the window hard-exits), then what it unblocked: the global
strings' static set-ups, `SignInWatch::StaticInit`, the UI movies'
`TrackProperty`, `ApplyTrack` and `ApplyAnimation`, the minimap's marker
draws and the viewports (12 adapters removed). The boundary: 449
functions (916 pairs): overridden 69, Native 5, Dropped 23, never called
70, live 282 (the two lock overrides). Imports: 186 called, 62 with a
live caller. B7 (`b7_status.py`): 6,451 in play, ours 2,013, work left
4,438 (Wrapped 110, with a row 590, no row yet 3,738), from 4,456 (from
4,548 at the run's start). `Minimap::Draw` waits on the course set's
lists; the semaphores' and the vblank event's static set-ups on their
kernel objects; `sprintf` and `_vsnprintf_l` on `_output_l` and
`_vsnprintf`.

**2026-10-25, the scene models (branch `b7-loop-25-10-26-3`):** the
sixteenth run's first subsystem mapped the scene models' set-up side
(`scene-models.md`, 48 rows; the rest of their code, ~227 functions, in
six leads) and made 23 native (`--native_scene_models`): the parts'
links, the records' defaults, copies and constructor, the car records'
reset and copy, the accessory, the sound loads' callback and the car
model's pages (VirtualAlloc through `memory::AllocPages`). Validated:
every function ran in its harness and in play (two only in the pad
replay). The boundary unchanged: 449 functions (916 pairs): overridden
69, Native 5, Dropped 23, never called 70, live 282. Imports: 186
called, 62 with a live caller. B7 (`b7_status.py`): 6,451 in play, ours
2,037, work left 4,414 (Wrapped 110, with a row 614, no row yet 3,690),
from 4,438. `SceneModel::SetUp`, `Build` and `Update` wait on the load
slots, `Archive::Load`, `ScenePools::SetUp`, the asset's size,
`sprintf` and `Texture::Create`.

**2026-10-25, `UiMovie::DrawTexts` and what it unblocked (branch
`b7-loop-25-10-26-3`):** a "no mapping" lead (not counted) made the UI
movies' text draw native and what it unblocked: `UiMovie::Update`,
`Draw`, `Run`, `Finish`, `FinishNamed` and `FinishAll`, `_init_pointers`
with the empty destructor, the race HUD's movie picker and the rival
logo's draw (`race-hud.md`, `--native_race_hud`; `--redraw_rival_logo`
calls the native draw); 9 adapters removed; the title screen's text
pixel-identical with the fallback. `_mtinit` waits on the kernel's TLS
(no call yet). The boundary unchanged: 449 functions (916 pairs):
overridden 69, Native 5, Dropped 23, never called 70, live 282.
Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`): ours
2,050, work left 4,401 (Wrapped 105, with a row 608, no row yet 3,688),
from 4,414.

**2026-10-25, the UI movies' ready groups (branch
`b7-loop-25-10-26-3`):** a "no mapping" lead (not counted) made the
rest of `ui-movies-game.md`'s ready groups native (26 rows: the vectors,
the movie's constructor and destructor, the texts freed, `GetText` and
the text setters, the named animations' set-up, the HUD's mirror
effect, whose replay frame dumps match the fallback's). The boundary
unchanged: 449 functions (916 pairs): overridden 69, Native 5, Dropped
23, never called 70, live 282. Imports: 186 called, 62 with a live
caller. B7 (`b7_status.py`): ours 2,076, work left 4,375 (Wrapped 105,
with a row 582, no row yet 3,688), from 4,401. The rest of the file
waits on `sub_82108860`, the string table's formatting, the archive
load's events (`NtWaitForSingleObjectEx`, `NtSetEvent`) and the texture
set.

**2026-10-25, the scenery lists (branch `b7-loop-25-10-26-3`):** the
sixteenth run's second subsystem mapped the course set's model lists and
the model draws (`scenery-lists.md`, 27 rows) and made them native as
far as they were ready (`--native_scenery_lists`; `models.md`'s
accessors and `Model::RecordShape` under `--native_models`): every model
draw in a race now records natively, `Recorder::RecordObject` inside its
interpolation wrapper (native callers run its hooks through
`BlendState_SetRecordContext`); frame dumps and `--log_blend_state`
counts matched the fallback. Cases the ports don't cover (material
animations, more than 4 textures, a full command list, another list's
chunk open) run the original from the start. The boundary unchanged:
449 functions (916 pairs): overridden 69, Native 5, Dropped 23, never
called 70, live 282. Imports: 186 called, 62 with a live caller. B7
(`b7_status.py`): ours 2,107, work left 4,344 (Wrapped 104, with a row
576, no row yet 3,664), from 4,375. The lists' loads wait on
`Model::Load` (the shader objects' set-up, the memory blocks).

**2026-10-25, the shader objects and what they unblocked (branch
`b7-loop-25-10-26-3`):** the sixteenth run's third and last subsystem
mapped the shader objects and the memory blocks (`shader-objects.md`, 7
rows; `XMemCpy` in `c-runtime.md`) and, in three migrations, made the
whole chain up to `Model::Load` native (`--native_shader_objects`;
`models.md`'s under `--native_models`; `XMemCpy`'s step word logged as
1; native copies into physical memory recorded for the renderer through
a new `native::Buffers_Memcpy`), then `DataFile::GetEntry` (new
`--native_data_files`) and what it unblocked: the scenery lists' loads,
the course set's parsers, pool objects, `CollectAreas`, `CollectModels`
and `DrawSetLists`, and the string table's `Get`, `FormatV` and `Format`
(15 adapters removed). Race frame dumps matched the fallback (the
attract race comes in two timing variants, 2,412 or 2,414 draws). The
boundary unchanged: 449 functions (916 pairs): overridden 69, Native 5,
Dropped 23, never called 70, live 282. Imports: 186 called, 62 with a
live caller. B7 (`b7_status.py`): 6,451 in play, ours 2,152, work left
4,299 (Wrapped 104, with a row 540, no row yet 3,655), from 4,344 (from
4,438 at the run's start). What's left in these files waits on the
generic critical section wrappers, the video output's events and
semaphores, `Archive::Load` and the asset's size.

**2026-10-26, the race's set-up (branch `b7-loop-25-10-26-4`):** the
seventeenth run's first subsystem mapped the race's set-up
(`race-setup.md`, 52 rows) and made all of it native in three
migrations (`--native_race_setup`): the car object's set-up and parts,
the race data, the course route table (its 1,175 constant stores
replayed into the original's frame), the racers and stages, up to
`Race::SetUp` and `Race::SetUpRival`; then `RacerRecord::Init`
(`vehicle-accessors.md`) and `Vehicle::SetupFromCar` with its car data
derivation (`car-physics.md`, under `--native_car_physics`). Native
callees run in the frame the original's call gives them, frame
leftovers written as the original leaves them; register saves aren't,
so `Race::SetUp`'s traffic records carry 42 bytes a record the
original fills from them (an expected harness group, for review). The
boundary unchanged: 449 functions (916 pairs): overridden 69, Native 5,
Dropped 23, never called 70, live 282. Imports: 186 called, 62 with a
live caller. B7 (`b7_status.py`): ours 2,209, work left 4,242 (Wrapped
104, with a row 535, no row yet 3,603), from 4,299.

**2026-10-26, the car physics' ready groups (branch
`b7-loop-25-10-26-4`):** a lead without mapping, after the race's
set-up: the rest of `car-physics.md` made native in two migrations,
the vehicle's physics step, set-up, reset and destroy, the ground
probe, and the collision world's set-up and update (134 of 134 ours;
84 adapters removed). The translated `ble` is "not greater", so a NaN
keeps the value so far (`car-physics.md`'s intro); callees refusing on
paths never seen run their original in the caller's frame; the
allocating ones run natively only under `--native_heap`. The boundary
unchanged: 449 functions (916 pairs): overridden 69, Native 5, Dropped
23, never called 70, live 282. Imports: 186 called, 62 with a live
caller. B7 (`b7_status.py`): ours 2,230, work left 4,222 (Wrapped 104,
with a row 516, no row yet 3,602), from 4,242.

**2026-10-27, the vehicle code above the physics (branch
`b7-loop-25-10-26-4`):** the seventeenth run's second and last
subsystem (the user's limit for it) mapped the vehicle code
(`vehicle.md`, 25 rows) and made all of it native in two migrations
(`--native_vehicle`): the seven vehicles' lifetime, the set-ups from a
car object, the tick (two physics steps of 1/60 a tick, the step read
from guest memory), the input, the track positions and status, the
engine's power curve and `PlaceOnRoute`. Never-seen callees (six under
`ApplyInput`, `sub_820FA8F8` under `PlaceOnRoute`) run the whole
original, decided before anything is written. The boundary unchanged:
449 functions (916 pairs): overridden 69, Native 5, Dropped 23, never
called 70, live 282. Imports: 186 called, 62 with a live caller. B7
(`b7_status.py`): ours 2,255, work left 4,197 (Wrapped 104, with a row
516, no row yet 3,577), from 4,222 (from 4,299 at the run's start).

**2026-10-28, the app's update and its state stack (branch
`b7-loop-27-10-26`):** the eighteenth run's first subsystem mapped the
app's update and its state stack (`app-states.md`, 56 rows: the lead
`820AE010`-`820AEC10` turned out to be the state stack's layers) and
made all of it native in two migrations (`--native_app_states`): the
app's update with its handler maps, requests and messages, `App::Start`
(which pushes the game modes' holder, whose slot 10 is how `Modes::Tick`
runs every pass), the state layers and the containers they use. Paths
never seen in play (`StateLayer::ApplyOp`'s pop and replace,
`AppStates::OnMessage`'s step and teardowns, failed allocations) run the
original. The boundary unchanged: 449 functions (916 pairs): overridden
69, Native 5, Dropped 23, never called 70, live 282. Imports: 186
called, 62 with a live caller. B7 (`b7_status.py`): ours 2,311, work
left 4,141 (Wrapped 103, with a row 515, no row yet 3,523), from 4,197.

**2026-10-28, the scene's loading objects (branch `b7-loop-27-10-26`):**
the eighteenth run's second subsystem mapped the lead taken for the car
code's skeletons as the first scene pool set's seven loading classes
(`scene-loaders.md`, 78 rows) and made 20 native in one migration
(`--native_scene_loaders`): the leaves, `ScenePools::MakeA` (over the
allocation layer only), the pool node, the load checks, the XPR name
lookups and `CarModelLoads`. The rest waits on functions in other
files, each small and ready: `LoadSlot::Reset` `sub_820D0730` and
`ReleaseBuffer` `sub_820D0758`, the deferred release `sub_820C3F50`
and `CarModel::Release` `sub_820D4508` (`car-models.md`), the part
model's `sub_820CE928`, `sub_820CE990`, `sub_820CE9C8` (`models.md`),
the accessories' release; taken next as a "no mapping" lead. The
boundary unchanged: 449 functions (916 pairs): overridden 69, Native 5,
Dropped 23, never called 70, live 282. Imports: 186 called, 62 with a
live caller. B7 (`b7_status.py`): ours 2,331, work left 4,121 (Wrapped
103, with a row 573, no row yet 3,445), from 4,141.

**2026-10-28, the scene loaders' blockers (branch `b7-loop-27-10-26`):**
a "no mapping" lead (not counted toward the run's limit) made the scene
loaders' small blockers in `car-models.md` and `models.md` native (the
load slots, the deferred release, `CarModel::Release`, the part model;
`--native_car_models`), then what they unblocked: the seven loading
classes' lifetimes, `ScenePools::CreateA` and `Interior::OnPartLoaded`
(`scene-loaders.md` 60 of 78 ours). Deleting destructors run natively
only without bit 0 (the `operator delete` on `VirtualAlloc` pages
never seen); matrices the originals build on their stacks and memcpy
are written in place, a car model at or above `0xA0000000` running the
original so memcpy's override still records the copies. The rest waits
on the material passes, the accessories, `Skeleton`'s local matrices
`sub_82599C58`, `XPR::GetTexture` and sprintf. The boundary unchanged:
449 functions (916 pairs): overridden 69, Native 5, Dropped 23, never
called 70, live 282. Imports: 186 called, 62 with a live caller. B7
(`b7_status.py`): ours 2,381, work left 4,071 (Wrapped 103, with a row
523, no row yet 3,445), from 4,121.

**2026-10-28, the players' save data (branch `b7-loop-27-10-26`):** the
eighteenth run's third and last subsystem mapped the save data in
memory (`save-data.md`, 23 rows, 4 not seen in play) and made all 19 in
play native in one migration (`--native_save_data`), at boot and the
title screen: `SaveData::Create` and `Delete`, the progress, stats,
course records, options and control maps, `TitleScreen::ResetPlayer`
and `MenuList::Init` (about 75 front-end screens call it, so its
adapter stays). Scratch below r1 isn't written (as earlier ports). The
boundary unchanged: 449 functions (916 pairs): overridden 69, Native 5,
Dropped 23, never called 70, live 282. Imports: 186 called, 62 with a
live caller. B7 (`b7_status.py`): ours 2,400, work left 4,052 (Wrapped
103, with a row 522, no row yet 3,427), from 4,071 (from 4,197 at the
run's start).

**2026-10-28, texture compression (branch `b7-loop-28-10-26`):** the
nineteenth run's first subsystem mapped the top lead, `sub_8235AAC0`, as
XGraphics' `XGCompressSurface` over D3DX's image blitter
(`texture-compression.md`, 157 rows, 24 seen in play; not part of the
replaced HLSL compiler), used in play only by `Vinyl::Compress`, and
made all 24 native in one migration (`--native_texture_compression`,
built with the strict-float options; six subagents one at a time, the
color block encoder alone ~1,500 lines). The format paths never seen in
play (DXN/CTX1, YUV, the filters, sRGB and others) go back to the
recompiled code; a few words the original's callees leave below r1
aren't written (as earlier ports). `XGCompressSurface`'s adapter stays
for its recompiled caller `Vinyl::Compress` (`car-models.md`, which
waits on two small ready groups). The boundary unchanged: 449 functions
(916 pairs): overridden 69, Native 5, Dropped 23, never called 70, live
282. Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`):
ours 2,424, work left 4,028 (Wrapped 103, with a row 522, no row yet
3,403), from 4,052.

**2026-10-28, the vinyl sheet (branch `b7-loop-28-10-26`):** two leads
without mapping in `car-models.md` made `Vinyl::Compress` with
`Texture::Create` and `Free`, then the decal path under
`CarModel::OnDecalLoaded` (the TGA reader, the CPU's software
rasterizer, `Vinyl::DrawDecal` and `DrawDecals`) native under
`--native_car_models` (`car-models.md` 27 of 46 ours; `XGCompressSurface`
and `Vinyl::Compress` Native). Paths never seen (RLE TGAs, region
numbers above 9, memory outside the allocation layer) go to the
originals. The TGA headers' two unwritten bytes in `DrawDecals`' frame
differ in the harness (expected to differ, for review). The run
stopped at the validation: `Vinyl::ClearCorner` `sub_820D3C80` ran in
play but its harness never reaches it. The boundary unchanged: 449
functions (916 pairs): overridden 69, Native 5, Dropped 23, never called
70, live 282. Imports: 186 called, 62 with a live caller. B7
(`b7_status.py`): ours 2,447, work left 4,006 (Wrapped 103, with a row
503, no row yet 3,400), from 4,028 (from 4,052 at the run's start).

**2026-10-28, the race world (branch `b7-loop-28-10-26`):** the
nineteenth run's second subsystem mapped the race's states and manager
(`race-world.md`, 54 rows, 50 in play) and made 49 native in three
migrations (`--native_race_world`; `TrackCollision::Tick` under
`--native_collision_world`): the race mode's state machine, the
racers' manager (80 entries; at most 4 opponents and the allowed traffic
given one of the 7 vehicles, streamed by distance each tick), the start
placements and the save's distance. `StreamTraffic`'s neighbor search
is left out as dead code (its count never goes below 0); two
functions never seen got harness stand-in overrides (Wrapped). One
harness group expected to differ (`AddRacers` with random traffic
numbers: stack leftovers). `RaceWorld::OnLoad` waits on the archive
(`sub_8209DE48`, `sub_820A0678`). The harnesses now take ~29 s of
`attract_race.sh 40` (`race_world`'s ~8 s), leaving ~9 s of race. The
boundary unchanged: 449 functions (916 pairs): overridden 69, Native 5,
Dropped 23, never called 70, live 282. Imports: 186 called, 62 with a
live caller. B7 (`b7_status.py`): ours 2,497, work left 3,961 (Wrapped
103, with a row 503, no row yet 3,355), from 4,006.

**2026-10-28, the race's states (branch `b7-loop-28-10-26`):** the
nineteenth run's third and last subsystem mapped the race's second
state machine (`race-states.md`, 78 rows, 63 in play: `RaceStates::Init`
at +107168, 87 transitions, and the race's progress at `[0x826D82E8]`)
and made 43 native in two migrations (`--native_race_states`), with
`RaceStates::Init` split in two and its transitions checked against the
listing. Float compares follow the translated code's `bge` as `!lt` and
`ble` as `!gt` (a NaN takes them). The System Link case of
`StandingStartTick` runs the original whole. The attract race reaches
only `Init`, `RaceProgress::Create`, `Reset` and `Begin`; the replay's
free-start race most of the rest; 23 functions wait for a drive
(`docs/loops/to-drive.md`). The rest waits on the loading screen, the
archive's queue, the cars put back, the input recordings, the scene
models' load poll, the items and achievements, the race event's
lookups, the two-player cameras and the core. The boundary unchanged:
449 functions (916 pairs): overridden 69, Native 5, Dropped 23, never
called 70, live 282. Imports: 186 called, 62 with a live caller. B7
(`b7_status.py`): ours 2,540, work left 3,920 (Wrapped 103, with a row
523, no row yet 3,294), from 3,961 (from 4,052 at the run's start).

**2026-10-28, the archive (branch `b7-loop-28-10-26-2`):** the
twentieth run's first subsystem mapped the archive (`archive.md`, 27
new rows; its B4 rows stay in `files-and-saves.md`) and made 25
functions native in two migrations (`--native_archive`): the TOC and
asset sizes, the request list and slots, the queue, the loader's
picker, set-up and next request, the waits and cancels, the request
finish and the Ucl unpacker. The loader's four semaphores (queue lock
`0x826BAAE0`, work `0x826BAAB8`, request-done `0x826BAA68`, idle
`0x826BAA90`) are host counting semaphores with a readable count, taken
by every user (the user's call, 2026-10-28): `Semaphore::Wait` and
`Signal` route those four addresses to them and every other semaphore
to the game's code. The rest waits on the loose read's file object
(`sub_820A6A00`, `sub_820A6A40`), the open by name (`sub_820A6AB8`) and
sprintf, and the game's `Semaphore::Init` for the static set-ups. The
boundary unchanged: 449 functions (916 pairs): overridden 69, Native 5,
Dropped 23, never called 70, live 282. Imports: 186 called, 62 with a
live caller. B7 (`b7_status.py`): ours 2,565, work left 3,896 (Wrapped
103, with a row 510, no row yet 3,283), from 3,920.

**2026-10-28, the file layer (branch `b7-loop-28-10-26-2`):** the
twentieth run's second subsystem mapped the game's file layer
(`file-layer.md`, 21 rows, 11 in play: the file object `GameFile` over
`AsyncFile`, `File::` and a base stream; the path object and
`Path::Build`; `GamePath::`, only from the movie library's open) and
made it native with what it unblocked (`--native_file_layer`): sprintf
and `_vsnprintf` with `_output_l` (`--native_c_runtime`,
`src/c_runtime/narrow_format.cpp`, after `_woutput_l`'s precedent: the
original runs for what the native formatter doesn't do), the open by
name and the archive's loose read (`--native_archive`), so nothing
ready is left in the archive or the file layer. The open by name and
the loose read stay adapters calling B4's `File::` adapters through
`ctx`. The boundary unchanged: 449 functions (916 pairs): overridden
69, Native 5, Dropped 23, never called 70, live 282. Imports: 186
called, 62 with a live caller. B7 (`b7_status.py`): ours 2,582, work
left 3,879 (Wrapped 103, with a row 504, no row yet 3,272), from 3,896.

**2026-10-28, the save system's interface (branch
`b7-loop-28-10-26-2`):** the twentieth run's third and last subsystem
mapped the save system's interface (`save-interface.md`, 62 rows, 41 in
play: the app's interface at `[0x826BDC90]`, app + 16, with the save
devices and users behind its slots +12 and +8, the wrappers over the
save slots' getters and request words, and the front end's save layer
at `8228F6B0`-`82290640`) and made all 41 native in one migration
(`--native_save_interface`), with `XContentCalculateSize`, the display
name's copy and the users' two vtable targets. Nothing in it calls an
Xbox service: it sets the slots' request words, which B4's `SaveSlot`
steps answer. The plain C++ checks that each vtable target it calls is
ours and hands back to the original when one isn't. The replays' list
(`BuildList`, `Count`, `ReadEntry`, `Sort`) waits for a drive. The
boundary unchanged: 449 functions (916 pairs): overridden 69, Native 5,
Dropped 23, never called 70, live 282. Imports: 186 called, 62 with a
live caller. B7 (`b7_status.py`): ours 2,628, work left 3,833 (Wrapped
103, with a row 498, no row yet 3,232), from 3,879 (from 3,920 at the
run's start).

**2026-10-28, the car sounds and X3DAudio (branch
`b7-loop-28-10-26-3`):** the twenty-first run's first subsystem mapped
the car sounds with X3DAudio (`car-sounds.md`, 66 rows, 62 in play: the
seven 1,264-byte per-racer car sound objects at `[0x826C5C3C]`, played
straight through XAudio voices into a submix, not through XACT, with
the game's own in-memory wave bank reader; and X3DAudio, 14 functions,
translated as the game's code by the XACT precedent) and made 24 native
in one migration (`--native_car_sounds`, `src/car_sounds/`; X3DAudio's
math and `CarSound::Update3D` built with strict floating point), the
eleven X3DAudio adapters and `EngineSound::EngineSound`'s removed. The
rest (the voices' add, play, pitch, update and release, `CarSounds::Init`,
`Start`, `Stop`) waits on XAudio's voice methods (`8245xxxx`, a lead).
The boundary unchanged: 449 functions (916 pairs): overridden 69,
Native 5, Dropped 23, never called 70, live 282. Imports: 186 called,
62 with a live caller. B7 (`b7_status.py`): ours 2,652, work left
3,809 (Wrapped 103, with a row 535, no row yet 3,171), from 3,833.

**2026-10-29, XAudio's voices (branch `b7-loop-28-10-26-3`):** the
twenty-first run's second subsystem mapped XAudio's voices
(`xaudio.md`, 108 rows, 93 in play: the game's recompiled copy of
XAudio, translated by the XACT precedent; its API lock the critical
section at `0x8264D518`, the mixer's own `0x8264D4F8`, the voice lists'
recursive spin lock `0x82954F10`) and made 92 of 96 in play native in
two migrations (`--native_xaudio`, `src/xaudio/`), 37 Native. Two
user's calls (2026-10-29): locks whose depth the code reads become
host recursive locks with a readable depth, taken by every user; a lock
taken inline whose users aren't all ours is taken natively as the
original does until they are. So both locks are still the game's: the
spin lock is inlined in 57 functions (the engine, mixer and codecs, a
lead), and 8 of the API lock's 29 users wait on the engine lead.
Native code calls the kernel imports through the guest call at their
thunks. The rest (`XAudioShutDown`, the callback's registration, the
commit) waits on the engine and mixer. The boundary: 449 functions
(916 pairs): overridden 94, Native 12, Dropped 23, never called 70,
live 250 (from overridden 69, Native 5, live 282). Imports: 186
called, 62 with a live caller. B7 (`b7_status.py`): ours 2,744, work
left 3,720 (Wrapped 103, with a row 539, no row yet 3,078), from 3,809.

**2026-10-29, the car sounds finished (branch `b7-loop-28-10-26-3`):**
with XAudio's voices native, two more migrations made the rest of the
car sounds native (`car-sounds.md`, 62 of 62 in play,
`--native_car_sounds`): the voices made, started, paused and released,
`CarSounds::Init`, and `CarSound::Update` with every part's update
(engine, tires, roll, hits, wind, pass-by, horn, turbo); the car
sounds' plain C++ calls XAudio's plain C++ directly. `CarSound::Update`'s
adapter stays for its recompiled caller in the race code
(`sub_82175F20`). The boundary: 449 functions (916 pairs): overridden
92, Native 14, Dropped 23, never called 70, live 250. Imports: 186
called, 62 with a live caller. B7 (`b7_status.py`): ours 2,782, work
left 3,682 (Wrapped 103, with a row 501, no row yet 3,078), from 3,720.

**2026-10-29, XACT's waves and voices (branch `b7-loop-28-10-26-3`):**
the twenty-first run's third and last subsystem mapped XACT's voice
class with the waves (`xact-waves.md`, 91 rows, 30 in play: the voice
class over XAudio's entry points, the reverb submix wrapper, the base,
in-memory and streaming waves, the tracks' volume and pitch getters)
and made 22 native in one migration (`--native_xact_waves`,
`src/xact/`), with 16 adapters removed (7 of XAudio's entry points, 5
in `xact.md`). XACT's lock (the engine's critical section, +144, taken
inline in 62 functions) is the game's, taken as the original does (the
2026-10-29 call). The rest waits on `XACTWave::Notify`'s
`XACTEngine::SendNotification` and `XACTGetTimeMs` over
`QueryPerformanceCounter` (a lead). The boundary: 449 functions (916
pairs): overridden 85, Native 22, Dropped 23, never called 70, live
249. Imports: 186 called, 62 with a live caller. B7 (`b7_status.py`):
ours 2,804, work left 3,660 (Wrapped 103, with a row 509, no row yet
3,048), from 3,682 (from 3,833 at the run's start).
