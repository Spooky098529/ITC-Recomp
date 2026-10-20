# Removing ReXGlue: toward a native port

The plan (written 2026-10-19 on branch `remove-rexglue`, restructured the
same day; A0, A5 and B1 done, B6 mostly done on `pure-helpers`) for taking the game off the ReXGlue SDK
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
  `docs/functions.md`; write it down there as it's understood.
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
- Check: log both versions' results on the same inputs where that's
  possible (pure functions: run both, compare, log differences);
  otherwise the play checklist below. `src/debug/state_capture.cpp` and
  `frame_state_diff.cpp` may help.
- Speed: uncapped A/B, or profiles for small differences (CLAUDE.md).

## What the game uses from the SDK

Measured 2026-10-19 by grepping `itc_recomp/src` and `generated/`.

| Area | What we use from the SDK | Where | Track |
|---|---|---|---|
| Logging, flags, hashes | `REXLOG_*` (~280 uses), `REXCVAR_*` (~370), `rex/hash.h` (xxHash) | almost every file in `src/` | A |
| App shell | `rex::ReXApp` (`src/itc_app.h`), `REX_DEFINE_APP` (`src/main.cpp`), `WindowedApp`, imgui (include path only), command line, log files, `PathConfig`, the SDK's hidden window (`src/native/app_window.cpp`) | `src/itc_app.h`, `src/main.cpp` | A |
| Renderer leftovers | The Xenos to SPIR-V shader translator built from SDK sources (`itc_xenos_shaders` in `itc_recomp/CMakeLists.txt`), `rex::graphics::RegisterFile` (`src/native/state.h`, `passes.cpp`), `registers.h`, `xenos.h`, `util/draw.h`, `ui/vulkan/api.h`; SDL3 and Vulkan headers from `rexglue-sdk/thirdparty/`; the xenos plugin still built (`GPU_PLUGINS xenos`) for comparisons | `src/native/` | A |
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
awk '/DEFINE_REX_FUNC\(sub_/ {match($0,/sub_[0-9A-F]+/); f=substr($0,RSTART,RLENGTH)}
     /__imp__[A-Za-z]/ && !/__imp__sub_/ && !/DEFINE_REX_FUNC/ {
       match($0,/__imp__[A-Za-z][A-Za-z0-9_]+/); print f, substr($0,RSTART+7,RLENGTH-7)}' \
  itc_recomp.*.cpp | sort -u
```

2026-10-19: 916 pairs, 449 distinct functions. By area (functions):
threads and sync 291, system services 78, pure helpers 55, memory 31,
files and saves 22, graphics 17, audio 6, input 3. Overrides in `src/`
already replace some callers; check each against `grep -rn` before
counting it as open.

These are the direct callers only. Making one native often means making
its close callers native too (a wrapper and the few functions using its
handles), which is fine: the unit of work is a subsystem, not a function.

## Testing

- Attract races without the user: `--skip_intro --attract_after_s=1`
  under `timeout`, read the log, frames with `--native_renderer_dump`
  (CLAUDE.md, "Build and run").
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
    `draw_util` and the Vulkan function list the same way. Ask the user
    whether the xenos plugin keeps being built for comparisons (CLAUDE.md
    uses it) or goes behind an option.

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

A6. **Audio output**: SDL audio straight from our mixer's output, our own
    conversion and downmix (`conversion.h`, `downmix.h` are small), in
    place of `AudioDriver`. Its game side is B2. Read
    `docs/native-audio.md` first.

## Track B: the native port from the boundary inward

Smallest and most self-contained first. Each item: make the game's
callers (and close neighbors) native at level 1, delete or stop building
the SDK piece nothing calls any more, release. Start every item by
listing its functions (the pipeline above), reading them in
`generated/` and writing them up in `docs/functions.md`.

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
    on replugging, and the race resumed fine. Not tried yet: starting with
    no pad connected.

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

B3. **Graphics boundary** (17 functions calling `Vd*`): ring buffer
    setup, the interrupt callback and vblank, swap, display mode and gamma
    queries. `src/native/gpu_link.cpp` already does the GPU's side; move
    it to the game's functions (read "Exclusive mode" and "Plan for
    milestone 6" in `docs/native-renderer.md`). Decide what happens to
    the MMIO range at `0x7FC80000` (`CP_RB_WPTR 0x714`, interrupt status
    `0x6544`, `0x6110`): show with logs that nothing reads it, or make
    its readers native. Then the GPU plugin and the SDK's graphics go.

B4. **Files and saves** (22 functions; the game's `CreateFile`
    `sub_82295508` is the only caller of `NtCreateFile`): the game's file
    functions on host files under `--game_data_root` (log the paths with
    `--log_file_opens`, `src/hooks/create_file.cpp`), with handles that
    are ours, used only by these functions. Check every user of a file
    handle is native (waits on async reads, `NtClose`). Saves
    (`XamContent*`): read and write where the SDK puts them, byte for
    byte; test with a save from the last release before and after. Then
    the SDK's VFS goes.

B5. **System services** (78 functions): sign-in, profile, UI dialogs,
    sessions, voice, network, notifications, the loader. Most should go
    from the game's side entirely (offline, signed in, no voice). Log
    which are called in a full play session first (an opt-in counter
    around the callers), and what the SDK answers today; the game must
    get the same answers.

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
    checked by an attract race and a temporary harness (not committed;
    kept in `git stash`) running the recompiled and native versions on
    the same inputs:

    - Time (done): `KeQuerySystemTime`, `RtlTimeToTimeFields`,
      `RtlTimeFieldsToTime`, through GetLocalTime, the time zone state,
      its cutover dates and the two FILETIME conversions
      (`src/helpers/time.cpp`). Thousands of random cases the same, but
      for times outside 1678-2261, where the SDK's conversions overflow
      `system_clock`'s nanoseconds (ours are right). The zone itself
      still comes from `sub_8229B440` (`ExGetXConfigSetting`, B5) as a
      guest call.
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
    - `XGetVideoMode`: not done, moved to B3. Its two callers are large
      graphics setup functions (`sub_8258D568`, the graphics library's
      display setup; `sub_82482B18`), and the SDK's answer comes from
      its `video_mode_*` settings.

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
    | `RtlNtStatusToDosError` | `sub_822953E8`; `sub_82296E88` (SetLastError from an NTSTATUS, which our time adapters call as guest code: native, it removes those guest calls) | `sub_8229B440`, the console's time zone read (B5) |
    | `DbgPrint` | `sub_822AC530`, `sub_82486BC0`, `sub_8248FBE0` | `sub_822954C8`, also calling `XamLoaderTerminateTitle` (B5) |
    | `sprintf` | `sub_822AC638`, `sub_822B1AC8` | the drive mounting `sub_822B58F0` (B4) |
    | `RtlCompareMemoryUlong` | `sub_82298548`, `sub_82298A88` | `sub_8229A930`, the game's heap (core) |
    | `KeGetCurrentProcessType` | `sub_8245A828`, `sub_825776A8` | five heap and memory functions (core) |
    | `RtlInitAnsiString` | none (its 2 pure callers are done) | the file functions `sub_825B9C20`, `sub_825B9F20`, `sub_825B9FD8`, `sub_822B58F0` (B4); `sub_82295508`, CreateFile, is only wrapped for logging |

    Blocked entirely: `RtlFillMemoryUlong` (its caller also sets up the
    scaler command buffer, B3), `XGetAVPack` and `XGetLanguage` (one
    function also reading the console's settings and the executable's
    privileges, B5), `XGetVideoMode` (B3, above), and `RtlRaiseException`,
    `RtlUnwind`, `RtlCaptureContext` (the core). Re-run the boundary
    pipeline before starting any of these: the callers may have changed.

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
       has to be ours. GetTimeZoneBias (`sub_82297678`) is plain C++ and
       its recompiled version is no longer called: the first function of
       this work to reach Native. GetLocalTime's only guest call left is
       the console settings read (B5). Checked by the harness (all the
       same) and an attract race.

    Suggested next (not decided): with 2 done, B6 counts as done for now;
    merge `pure-helpers` into `rexglue-removal-project` and pick the next
    item (A2, or A6 with B2, would remove visible SDK use).

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
    the statuses in `docs/functions.md` up to date (Wrapped, Overridden,
    Native, defined at its top) so progress can be measured: Native plus
    Overridden is how much of the game is ours, Native alone how much no
    longer needs `ctx`. Keyboard input comes back
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
    modified). The `rexglue codegen` target goes with it.

## Risks

- Saves (B4): players' saves must keep loading.
- Behavior drift: a native function that differs subtly from the
  recompiled one (float rounding, big-endian layout, order of side
  effects). Compare against the recompiled version with a fallback flag
  before it's removed.
- Scale (B7): most of the work is the game logic, not the boundary.
  Track A and B1-B6 are worth doing on their own even if B7 goes slowly.
