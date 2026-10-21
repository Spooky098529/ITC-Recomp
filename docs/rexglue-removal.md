# Removing ReXGlue: toward a native port

The plan (written 2026-10-19 on branch `remove-rexglue`, restructured the
same day; A0, A5, B1 and B3 done, B6 done for now) for taking the game off the ReXGlue SDK
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
itc_recomp/tools/attract_race.sh 25 --run_harnesses=all
```

- `harness.h` has what they share: `Both` (one case through both
  versions: the scratch area filled with 0xCD, inputs written, registers
  set, the function run, the output bytes and r3 kept), `Results`
  (counts and logs), a seeded `Random` (the same cases each run) and
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

Measured 2026-10-19 by grepping `itc_recomp/src` and `generated/`.

| Area | What we use from the SDK | Where | Track |
|---|---|---|---|
| Logging, flags, hashes | `REXLOG_*` (~280 uses), `REXCVAR_*` (~370), `rex/hash.h` (xxHash) | almost every file in `src/` | A |
| App shell | `rex::ReXApp` (`src/itc_app.h`), `REX_DEFINE_APP` (`src/main.cpp`), `WindowedApp`, imgui (include path only), command line, log files, `PathConfig`, the SDK's hidden window (`src/native/app_window.cpp`) | `src/itc_app.h`, `src/main.cpp` | A |
| Renderer leftovers | The Xenos to SPIR-V shader translator built from SDK sources (`itc_xenos_shaders` in `itc_recomp/CMakeLists.txt`), `rex::graphics::RegisterFile` (`src/native/state.h`, `passes.cpp`), `registers.h`, `xenos.h`, `util/draw.h`, `ui/vulkan/api.h`; SDL3 and Vulkan headers from `rexglue-sdk/thirdparty/`; the xenos plugin, no longer built for us or loaded since B3 (the SDK's own tree still builds it) | `src/native/` | A |
| Graphics boundary | `Vd*` (25 imports; 17 game functions). In exclusive mode the GPU's job is already ours (`src/native/gpu_link.cpp`) | SDK `src/kernel/` | B |
| Audio | Output: `AudioDriver`, `REX_AUDIO_BACKEND`, `conversion.h`, `downmix.h` (A). The game side: `XAudio*`, `XMA*` and the SDK's XMA decoder, which emulates the XMA hardware's contexts on FFmpeg (B; 6 game functions). Our mixer (`src/audio/native_audio.cpp`) still runs on an `XHostThread` and allocates from the guest heap | `src/audio/` | A and B |
| Input | Reading SDL gamepads (A; the `--mnk_mode` keyboard is dropped first, A0); `XamInput*` (B; 3 game functions) | SDK `src/input/` | A and B |
| Files, saves | `Nt*File` and friends, `Ob*SymbolicLink`, `XamContent*` through the SDK's VFS and `--game_data_root` (22 game functions; 12 for the file calls) | SDK `src/filesystem/`, `src/kernel/` | B |
| System services | `Xam` UI, user, profile, sessions, voice, notifications, `XMsg*`, `XamLoader*`, `NetDll_*` (17), `Xex*` (78 game functions) | SDK `src/kernel/xam/` | B |
| Pure helpers | `Rtl*` strings, time, fills, `sprintf`, `vswprintf`, `KeQuery*`, `XGet*`, `DbgPrint` (55 game functions) | SDK `src/kernel/` | B |
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

A1. **Third-party libraries from our own copies** (submodules or
    FetchContent, pinned versions) instead of `rexglue-sdk/thirdparty/`:
    SDL3, Vulkan headers, fmt, spdlog, xxHash, glslang. While
    `librexruntime` exists it links its own; only one SDL may be loaded
    (see patch 0003 and `docs/windows.md`), so SDL probably stays the
    SDK's until A4.

A2. **Logging, flags, hashes**: our own headers (`src/base/log.h`,
    `cvar.h`, `hash.h`, names to taste) with our own macro names, first
    forwarding to the SDK's, and every file in `src/` switched over
    (mechanical; one commit per area). Then our own implementations for
    logging and hashing. Our own flag parser has to wait for A4: until
    then the SDK's flags (`--game_data_root`,
    `--fullscreen`, ...) are parsed from the same command line by the SDK.
    Log file names and rotation must stay (`logs/itc_<n>.log`,
    `itc_<n>.1.log`): CLAUDE.md and tools rely on them.

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
    or A6 with B2, would remove visible SDK use).

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
