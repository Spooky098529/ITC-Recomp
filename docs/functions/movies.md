# Known functions: the movies

The game's movies (the 12 `.wmv` files in `DATA/TXR/MOVIE`: WMV3 video,
1280x720 at 29.97 fps, and WMA v2 stereo at 48 kHz) and the game's side
of their playback. The game reaches them only through `Movies::*`, one-line
calls of the movie class (`Movie`, vtable `0x82002910`) that the app's
start-up keeps at `0x826C1CDC`, from the boot sequence's handlers
(`Boot::StartMovie`, `Boot::WaitForMovie`, `Boot::EndMovie` in
`functions.md`; the stage openings come through them too). The class
keeps its state at +308 (0 none, 1 loaded or ended, 2 playing, 3 paused)
and reaches the movie library, linked into the game (`82481A70`-
`8257FFFF`, ~615 functions seen in play: the player, its threads, the
WMV and WMA decoders and its own YUV draw; with the `.embsec_` sections
`825DC200`-`825FC1B8`, 120 functions of its decoder called only from
`sub_824D3148`, `sub_8251BB78` and `sub_825169A0`, see
`../coverage.md`), only through slots 0, 3, 4,
5, 7 and 8, by an 8-byte wrapper at +8 (`MovieWrapper`, vtable
`0x8206CC90`: slot 0 opens, 5 returns the player, 6 releases) and the
player's methods at +80, +88, +92 and +96. The plan to play them through
FFmpeg instead, from the movie class's methods: `../native-movies.md`.
Mapped 2026-10-25. Since 2026-10-25 the class's slots 0-7 are ours
(`--play_movies_with_ffmpeg`, on by default, `src/hooks/movies.cpp`), so the
library and the wrapper's slots no longer run.

A movie, as played: `Boot::StartMovie` calls `Movies::Play` (slot 1: the
player opened on a worker, slot 8, under the device lock; state 1);
`Boot::WaitForMovie`, every tick, starts it (slot 2, state 2); while
playing, `Movie::OnTick` (slot 9) queues command 28 in the command list
each tick, whose handler on the render thread calls `Movie::RenderFrame`
(slot 0: the library draws the next frame, outside the command list,
`../gpu-functions.md` "Movies"); at the file's end it sets state 1, and
`Boot::WaitForMovie` (or a pad press) moves on to `Boot::EndMovie` (slot
3 stops, slot 7 releases).

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

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_820A4360` | `Movies::Set` | Native | r3 = the app: stores its vtable slot 11's result (the movie class) at `0x826C1CDC`. Once at boot, from the app's start-up `sub_82106548`. Read 2026-10-30: the slot is `sub_82379350` (the interface's +112, the app + 128). Ours (`src/app_start_up/start_up.cpp`, `--native_app_start_up`, `app-start-up.md`): the interface's +112 stored when slot 11 is that getter, else the original. Harness `app_start_up`: 8 cases (two through another slot), 0 differ. Native since 2026-10-30: its adapter removed; reached from `App::StartUp`'s plain C++ (`--native_app_start_up` now switches only the caller; its original where the plain C++ hands back, never in play); no call to the adapter in an attract race (`itc_2612`). |
| `sub_820A4398` | `Movies::Clear` | | Stores 0 at `0x826C1CDC`. From `sub_82106678`. Never called in play. |
| `sub_820A43A8` | `Movies::Play` | | r3 = a movie number: the path from the table at `0x82600BEC` (0 `OPENING`, 1 `STAGE1_OPENING`, 2 `SHIBUYA_OPEN`, 3 `STAGE2_OPENING`, 4 `SHINJUKU_OPEN`, 5 `STAGE3_OPENING`, 6 `ENDING`, 7 `STAFFROLL1`, 8 `PERFECT_ENDING`, 9 `STAFFROLL2`, 10 `UBI_LOGO`; `UBI_LOGO_SD.wmv` isn't in it) to the movie class's slot 1. From `Boot::StartMovie`. |
| `sub_820A43D8` | `Movies::Start` | | The movie class's slot 2. From `Boot::WaitForMovie`. |
| `sub_820A43F0` | `Movies::Stop` | | Slot 3. From `Boot::EndMovie`. |
| `sub_820A4408` | `Movies::Pause` | | Slot 4. From `sub_82108018`. Never called in play. |
| `sub_820A4420` | `Movies::Resume` | | Slot 5. From `sub_82108018`. Never called in play. |
| `sub_820A4450` | `Movies::State` | Wrapped | Slot 6 (the state: 1 loaded or ended; named `Movie::IsFinished` before). From `Boot::WaitForMovie`, twice a tick. `src/hooks/title_screen.cpp`: with `--skip_intro`, 1 from `Boot::WaitForMovie` once `Movie::RenderFrame` has returned. |
| `sub_820A4438` | `Movies::Release` | | Slot 7. From `Boot::EndMovie`. |
| `sub_820AEA88` | `Movie::Movie` | Overridden | r3 = it (312 bytes, `operator new` in `App::App`), r4 = the set-up block (+0 the run loop, +4 a word kept at +4; `App::App`'s {app + 20, app + 8}). Read 2026-10-30 (B7 loop, the app's start-up): +0 the vtable `0x82002910`, +4 the block's +4; the command at +12 (+12 `0x820066DC`, +16 `0x82000AB4` then `0x82002908`, +20 and +24 0, bytes +28 0, +29 1, +30 0, +36 `0x82002850` (a delegate's vtable), +40 and +44 0; byte +31 not written); +8 an 8-byte `MovieWrapper` (`operator new`, `sub_82593DC0`) or 0; the run loop's slot 2 (`AddTickHandler`, key 1, a 12-byte delegate {`0x82002850`, it, `0x820AE808`} at its frame's +88) and slot 1 (`AddListener`, key 1, {`0x82002850`, it, `0x820AE818`} at +104; +88 left `0x82000ABC`); +308 (the state) = 0. Returns it. 160-byte frame, `__savegprlr_28`. Once at boot. Ours (B7 loop, 2026-10-30; `app_start_up::MovieConstruct`, `src/app_start_up/movie.cpp`, adapter in `src/hooks/app_start_up.cpp`, `--native_app_start_up` with `--native_heap`): the same stores, the wrapper's plain C++, the run loop's slots guest calls from a frame like the original's (the prologue's saves written); operator new's plain C++ (the original from the start where it can't answer). Harness `app_start_up`: `Movie::Movie (sub_820AEA88) with MovieWrapper::MovieWrapper (sub_82593DC0)`, a fake run loop whose slots 2 and 1 are a counter and a recorder. Ran in the attract race (`itc_2618`). Called by `App::App` (recompiled), so its adapter stays. |
| `sub_820AE958` | `Movie::RenderFrame` | Overridden | Slot 0, on the render thread from command 28 (`sub_8258E9B8`). The player from the wrapper (+8, slot 5); its rectangle 0, 0, 1280, 720 (`sub_82481A80`); the next frame drawn (`sub_82481A70`, player +80); `0x16660026` (the end of the file) sets the state to 1. `src/hooks/title_screen.cpp`: notes that it returned, for `--skip_intro`, which waits for it (ending the movie earlier stalled the boot; "The title screen" in `../title-screen.md`). Ours (`src/hooks/movies.cpp`): the frame due drawn by our renderer at this point (`Passes::DrawMovie`), state 1 once the movie has ended; `--skip_intro`'s note made after either path (`TitleScreen_MovieRendered`). |
| `sub_820AEC10` | `Movie::Play` | Overridden | Slot 1: r4 = the path, copied to +48 (260 bytes). Runs `Movie::Open` through its command (+12: +28 it, +32 `sub_820AE7F8`) on a worker (+4's slot 2, then that slot 1), waits for it (`sub_82094B10`), state 1. Not read in detail. `src/hooks/movies.cpp` (`--play_movies_with_ffmpeg`, on): `movies::movie::Play` copies the path as strncpy does, opens our player (`src/movies/player.h`) and sets +24 to 3 (done) and the state to 1; the command and the worker aren't used. |
| `sub_820AE4E0` | `Movie::Start` | Overridden | Slot 2: when the service at +24 is 3 and the state isn't 0: state 2; returns the wrapper's slot 5 (the player). Ours (`src/hooks/movies.cpp`): the same test and state; our player's clock started and its sound on; r3 left as it came (the caller reads none). |
| `sub_820AE828` | `Movie::Stop` | Overridden | Slot 3: in state 2 or 3: state 1, the player's +96 (`sub_82481B78`, r4 = 0). Ours: the same; our player paused and its sound off. |
| `sub_820AE898` | `Movie::Pause` | Overridden | Slot 4: in state 2: state 3, the player's +88 (`sub_82481B58`). Never called in play. Ours: the same; our player's clock held. |
| `sub_820AE8F8` | `Movie::Resume` | Overridden | Slot 5: in state 3: state 2, the player's +92 (`sub_82481B68`). Never called in play. Ours: the same; our player's clock run on. |
| `sub_820AEA80` | `Movie::State` | Overridden | Slot 6: returns +308 (0 none, 1 loaded or ended, 2 playing, 3 paused). Ours: returns +308, as the original. |
| `sub_820AE528` | `Movie::Release` | Overridden | Slot 7: unless playing (state 2), the wrapper's slot 6 under the device lock (`VideoOutput::LockDevice`, `UnlockDevice`). Ours: the same test; our player closed. No device lock: our player isn't the device's. |
| `sub_820AE5D0` | `Movie::Open` | | Slot 8, on the worker: under the device lock, the wrapper's slot 0 with the path at +48 (through `sub_820A6BF0`); nonzero calls `sub_8209AC78`. Not read in detail. No longer called with `--play_movies_with_ffmpeg`. |
| `sub_820AE688` | `Movie::OnTick` | | Slot 9, the run loop's tick handler: in state 2 or 3, queues command 28 (it at +4) in the command list (`sub_8258CAA0`, `sub_8258CA50`, `sub_8258CA88`). Not read in detail. |
| `sub_820AE780` | `Movie::OnMessage` | | Slot 10, the listener: on message 6 (r5's +0), r4's slots 6 and 5 with 1. Not read. |
| `sub_820AE7F8` | | | Calls r3's slot 8 (+32). `Movie::Play`'s command callback. |
| `sub_820AE808` | | | Calls r3's slot 9 (+36). The tick handler's thunk. |
| `sub_820AE818` | | | Calls r3's slot 10 (+40). The listener's thunk. |
| `sub_82593DC0` | `MovieWrapper::MovieWrapper` | Overridden | r3 = it (8 bytes, `operator new` in `Movie::Movie`): vtable `0x8206CC90`; +4 a 32-byte block (`operator new`): its +0 a critical section (`InitializeCriticalSection`, `sub_8258E548`), its +28 the player (0); returns r3. With no block it stores 0 at address 28 (never in play). Its slots: 0 `sub_82593E78` (open), 1 `sub_82106540` (empty), 2 to 4 `sub_82593D88`, 5 `sub_82593D78` (the player), 6 `sub_82593E30` (release), 7 `sub_82593F60` (the deleting destructor); the movie class's way into the library. Read 2026-10-30 (B7 loop, the app's start-up). Ours (`app_start_up::MovieWrapperConstruct`, `src/app_start_up/movie.cpp`, adapter in `src/hooks/app_start_up.cpp`, `--native_app_start_up` with `--native_heap`): the same, without `InitializeCriticalSection`: the section is a host mutex (`app_start_up::MovieWrapperMutex`, a `std::recursive_mutex`; the user's call for a subsystem's critical section, `Edram::Alloc` 2026-10-24 and the memory blocks' lock 2026-10-25), taken by its every user (slot 0 and slots 2 to 4, through `ScopedLock::ScopedLock` and `~ScopedLock`, whose adapters in `src/hooks/models.cpp` take the mutex for the block the native constructor made; nothing else enters it: greps over `generated/` for `EnterCriticalSection`'s and `ScopedLock`'s callers, and the library is handed only the wrapper); the section's 28 bytes left as `operator new` leaves them (as `Edram::Alloc`'s). Nothing reads its depth. With `--play_movies_with_ffmpeg` (on) no user runs; with it off, slot 0 took the mutex (`itc_2619`). Harness `app_start_up`: `MovieWrapper::MovieWrapper (sub_82593DC0)` (the section's bytes not compared) and `ScopedLock (sub_8258E580, sub_8258E5B8, the movie wrapper's critical section)`. Ran in the attract race (`itc_2618`). Called by `Movie::Movie`'s plain C++ (the adapter's steps, `hooks::MovieWrapperSteps`) and `Movie::Movie`'s original with the flag off. |
| `sub_82593E78` | `MovieWrapper::Open` (probably) | | Slot 0: r3 = the wrapper, r4 and r5 passed on. Under `ScopedLock` on the wrapper's section (`[+4]`): the library's open (`sub_82482730`, r3 = r4, r4 = r5, r5 = the block's +28, the player out); when that answers 0, `sub_82481B88(the player, 1, 0x82106540, the wrapper)` (probably a callback set: `0x82106540` is the empty function). Returns the open's answer (or the second call's). 128-byte frame. From `Movie::Open` (`sub_820AE5D0`, through `sub_820A6BF0`). No longer called with `--play_movies_with_ffmpeg`. Read 2026-10-30 (B7 loop, the app's start-up). |
| `sub_82593D88` | `MovieWrapper::Sync` (probably) | | Slots 2, 3 and 4: takes and drops the wrapper's section at once (`ScopedLock::ScopedLock`, `~ScopedLock`). Not seen in play. Read 2026-10-30 (B7 loop, the app's start-up); since then the host mutex's (`MovieWrapper::MovieWrapper`). |
| `sub_82593D78` | `MovieWrapper::Player` (probably) | | Slot 5: returns `[[r3 + 4] + 28]` (the player). From `Movie::Start` and `Movie::RenderFrame` (their originals). No longer called with `--play_movies_with_ffmpeg`. Read 2026-10-30. |
| `sub_82593E30` | `MovieWrapper::Release` (probably) | | Slot 6: the player (`[[r3 + 4] + 28]`), when set, released (`sub_820A7BD0`) and the word cleared. From `Movie::Release`'s original. No longer called with `--play_movies_with_ffmpeg`. Read 2026-10-30. |
| `sub_82593EF0` | `MovieWrapper::~MovieWrapper` | | The vtable put back; the player released as slot 6 does; the block's empty destructor (`sub_82106540`: the section is never deleted) and `operator delete` (`sub_825A70E8`). From slot 7. Not seen in play. Read 2026-10-30. |
| `sub_82593F60` | | | Slot 7, the deleting destructor: `sub_82593EF0`, then `operator delete` when r4's bit 0 is set. Not seen in play. Read 2026-10-30. |
| `sub_82481A70` | `MoviePlayer::RenderNextFrame` | | Probably: the player's +80, r4 and r5 0 from `Movie::RenderFrame`; `0x16660026` at the end of the file. A thunk. No longer called with `--play_movies_with_ffmpeg`. |
| `sub_82481A80` | `MoviePlayer::SetRect` | | Probably: r4 = a rectangle (0, 0, 1280, 720); calls `sub_82482DA8`. Not read in detail. No longer called with `--play_movies_with_ffmpeg`. |
| `sub_82481B58` | `MoviePlayer::Pause` | | Probably: the player's +88. A thunk. No longer called with `--play_movies_with_ffmpeg`. |
| `sub_82481B68` | `MoviePlayer::Resume` | | Probably: the player's +92. A thunk. No longer called with `--play_movies_with_ffmpeg`. |
| `sub_82481B78` | `MoviePlayer::Stop` | | Probably: the player's +96. A thunk. No longer called with `--play_movies_with_ffmpeg`. |
| `sub_82108018` | | | Pauses (`Movies::Pause`) or resumes the movie when the flags at `0x826C7960`/`64` change, probably the system menu; more not read. Never called in play. |
| `sub_82486BC0` | | | The movie player's stream set-up (from `sub_82481B98`): r3 the player, r4 and r5 two streams' formats, set up through the player's +56, +60 and +64 objects (buffers by `sub_8248F2B8`); a third (r6) only with a `DbgPrint`. Twice a session, r6 0 each time, so the print never runs. Not read in detail. No longer called with `--play_movies_with_ffmpeg`. |
| `sub_824E1750` | | | Movie code (4,497 lines): r3 = the movie player's object, whose +15144, +15156 and +14724 are its own (not the device's, though the offsets match). Not read in detail. No longer called with `--play_movies_with_ffmpeg`. |
| `sub_825169A0` | | | Movie decoder: reads its object's +3852, +14776 and +15148 (compared with -1; not the device's vblank count). Called through a pointer from `824D40F8`, about once a frame during the boot movie (r3 = `40B0AB40`, +15148 = -1). Not read in detail. No longer called with `--play_movies_with_ffmpeg`. |
| `sub_82533010` | | | Movie decoder, like `sub_825169A0` (the same +3852/+14776/+15148 checks); from `sub_8251BB78`. Not called during the boot movie. Not read in detail. No longer called with `--play_movies_with_ffmpeg`. |
| `sub_8253DEB0` | | | Movie decoder, like `sub_825169A0`; called through a pointer. Not called during the boot movie. Not read in detail. No longer called with `--play_movies_with_ffmpeg`. |
| `sub_8256D470` | | | Movie decoder, like `sub_825169A0`; from `sub_8251BB78`. Not called during the boot movie. Not read in detail. No longer called with `--play_movies_with_ffmpeg`. |
| `sub_8251BB78` | | | Movie decoder: calls `sub_82533010` and `sub_8256D470`. Not read. No longer called with `--play_movies_with_ffmpeg`. |
