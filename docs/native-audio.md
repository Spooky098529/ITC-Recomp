# Native audio

The plan and progress of replacing the SDK's audio output (Xenia's audio
system, in `rexglue-sdk/src/audio/` and
`rexglue-sdk/src/kernel/xboxkrnl/xboxkrnl_audio.cpp`) with our own, as
the native renderer replaced the emulated GPU (`native-renderer.md`). Why:
the SDK's "Audio Worker" thread takes ~80% of a core for the whole game
(see "Measured" below). As with rendering, the game's own audio library is
kept and only what's under it is replaced.

## How the game's audio works

From the SDK's source and the generated code (2026-10-14):

1. The game links the Xbox 360's **XACT** and **XAudio** libraries
   (recompiled guest code, like its Direct3D). XACT plays cues from the
   game's sound banks; XAudio mixes every playing voice on the CPU
   (resampling, volume, pitch, 3D panning) into 6-channel frames.
2. **The render driver** (the kernel's XAudio functions, in the SDK):
   - The game's XAudio registers its mixer with
     `XAudioRegisterRenderDriverClient` (called from `sub_824657E0`: r3 =
     the driver object, r4 = the callback; the callback's argument is
     read from the object's +12, and the driver handle kept at +16) and hands back each mixed frame with
     `XAudioSubmitRenderDriverFrame` (from `sub_82465870`). Both are
     reached only through virtual calls (no direct callers in
     `generated/`).
   - The SDK's **Audio Worker** thread (`AudioSystem::WorkerThreadMain`)
     calls the game's mixer callback whenever a client's semaphore is
     released; each client starts with 8 frames' worth
     (`--audio_maxqframes`). A frame is 256 samples a channel, 6 channels
     of big-endian floats, one after another (about 5.3 ms at 48 kHz).
   - The SDL driver (`sdl_audio_driver.cpp`) queues the frames; SDL's
     audio thread converts each to interleaved stereo or 5.1 (with the
     SDK's downmix) and releases the semaphore, so the worker runs the
     mixer again.
3. **The XMA hardware** (the 360's compressed audio decoder), in the SDK:
   registers at `0x7FEA0000` (mapped with `AddVirtualMappedRange`), 64-byte
   contexts in guest memory, decoded with FFmpeg on an **XMA Decoder**
   thread, and also right away on the guest thread that writes a context's
   kick register. The game imports only `XMACreateContext` and
   `XMAReleaseContext`; XAudio drives the contexts through their memory
   and the registers.
4. Other kernel audio functions the game imports:
   `XAudioUnregisterRenderDriverClient`, `XAudioGetVoiceCategoryVolume`
   (the SDK returns 1.0) and `XAudioGetVoiceCategoryVolumeChangeMask` (the
   SDK sleeps 1 µs and returns 0).

The SDK picks the SDL backend in `ReXApp::SetupPresentation`
(`config_.audio_factory`); the app can change it in `OnPreSetup`.

## The game's sounds

`DATA/AUDIO`: one global settings file (`TXR_XACT.xgs`), 19 sound banks
(`.xsb`) and 102 wave banks (`.xwb`, 71 MB). The wave banks are big-endian
(`DNBW`), version 39 (XACT2 era), with 24-byte entries and 2048-byte
alignment. Of 390 waves:

| Format | Waves | What |
|---|---|---|
| PCM, 16-bit, mono, 44.1 kHz | 349 | Every car's engine sounds (`<car>_on`/`_off` banks), `car_se` and the other effects |
| XMA, stereo, 44.1 kHz | 21 | The 17 music tracks (`bgm_tmp_01`-`17`, one wave each, about a minute) and `BGM_SYSTEM` (4) |
| XMA, mono, 44.1 kHz | 20 | `SB_SYSTEM_SE`: the menus' sounds |

All the XMA banks are streaming banks (flags `0x80000`). Together the XMA
waves are 41 MB, about 261 MB as 16-bit PCM. Loop regions are stored in
samples (`bgm_tmp_01`: 68.6 s, looping from 13.7 s for 54.9 s), so they'd
carry over to PCM unchanged. So in a race the XMA decoder only plays the
music.

## Measured

2026-10-14, `main` at `3d44b95`, three uncapped attract races
(`--skip_intro --attract_after_s=1 --max_fps=0`, 205-275 fps), profiled
12-14 s after launch: `perf record -e cycles:u -p <pid>` for 15 s, then
`--call-graph dwarf` on the Audio Worker alone (`-t <tid>`) for 4 s, with
`top -H` for each thread's CPU use. (gdb can't attach here:
`ptrace_scope` is 1.)

| Thread | CPU | Share of the process's samples |
|---|---|---|
| Render thread (`XThread6FFE6C0`) | ~99% | 40% |
| **Audio Worker** | **~80%** | **33%** |
| Main XThread | ~55% | 23% |
| SDL's audio thread (`SDLAudioP23`) | ~0% | 0.01% |
| XMA Decoder | 0% | 0% |

Within the Audio Worker:

- **~96% is the SDK's wait**: `pthread_mutex_trylock` (52%) and an
  unnamed libc address next to it (`0x9c77c`, 43%; libc has no symbols
  here), both called from `rex::thread::PosixConditionBase::WaitMultiple`
  (`rexglue-sdk/src/core/threading_posix.cpp`). The worker waits on 9
  handles at once (8 client semaphores and a shutdown event, `WaitAny`),
  and on Linux that wait polls: it trylocks every handle's mutex, yields if
  one is held, otherwise checks them and sleeps 1 ms, and loops.
- ~3% is the same loop's clock reads, `malloc` and `new`/`delete`.
- **~0.3% is the game's own code** (the mixer callback): mixing costs
  next to nothing.
- No other thread spends measurable time in that wait.

Not known yet:

- **Why the loop never sleeps**: `nanosleep` is ~0% of the samples and the
  process's system time stayed under 1%, so it's spinning in user space
  rather than sleeping 1 ms a pass or yielding. Not looked into, since
  step 1 replaces the thread.
- **Whether it costs frame rate.** It runs on a core of its own; with few
  cores it would compete with the render and main threads. To be measured
  before and after step 1.

## Decisions

- **Keep the game's XACT and XAudio**; replace the SDK's render driver.
  Replacing XACT itself (e.g. with FAudio) isn't planned: the banks are XACT2-era big-endian files, and the engine
  sounds depend on XACT's runtime variables.
- **Keep the SDK's XMA decoder** (the user's call, 2026-10-14): it costs
  nothing measurable, so replacing it gains no speed. (Considered:
  converting the XMA wave banks to PCM banks on first launch, decoded once
  with FFmpeg and opened instead of the originals through the `CreateFile`
  wrapper `sub_82295508`, so that XACT plays music and menu sounds through
  its PCM path and no XMA context is made. Its open question was whether
  this XACT streams PCM from banks laid out for XMA.)
  **Changed (the user's call, 2026-10-21, for B2 in
  `docs/rexglue-removal.md`):** XMA is decoded by our own code calling
  FFmpeg, in place of the SDK's XMA decoder and its emulated hardware
  (contexts, the registers at `7FEA0000`), so the SDK's audio system can
  go. The PCM bank conversion isn't the plan.
- **No SDK patch**: override the game's functions in `itc_recomp/src/`,
  as the native renderer does.

## Plan

1. **Our own render driver** (`itc_recomp/src/audio/`). Override the
   game's driver object's register (`sub_824657E0`), submit
   (`sub_82465870`) and destructor (`sub_824658C8`, which unregisters) so
   the SDK's `XAudio*RenderDriver*` functions are never called, and give
   the runtime our own audio system (a subclass of the SDK's, set in
   `OnPreSetup`) that sets up only the XMA decoder, so the Audio Worker
   never starts. Our
   own thread runs the game's mixer callback paced by an SDL3 audio
   stream, with a blocking wait (no polling), and converts the 6-channel
   big-endian frames itself. Open: the callback needs a guest thread
   context; the SDK's `XHostThread`, as its worker uses, is the likely
   answer. **Done when** sound plays as before, the Audio Worker is gone,
   and CPU use and frame rate are measured against the SDK's driver.
2. **Only if a later profile shows it**: the game's mixer (native mixing
   loops) or locks the audio thread shares with the render thread. Neither
   showed up in the profile above.

## Progress on step 1

- **Done in code** (2026-10-14, `itc_recomp/src/audio/native_audio.cpp`):
  - `ItcAudioSystem`, a subclass of the SDK's `AudioSystem`, put in the
    runtime's config by `ItcApp::OnPreSetup`. Its `Setup` starts only the
    XMA decoder and our thread ("ITC Audio", an `XHostThread`, as the
    SDK's worker was); `CreateDriver` (the SDK's driver) logs an error if
    anything asks for it.
  - The game's driver object calls ours: `sub_824657E0` (register: unregisters
    any earlier handle, then registers r4 with the argument at +12; handles
    are `0x4954000n`), `sub_82465870` (submit: the samples' address is the
    third word of the descriptor `sub_8245F718` fills) and `sub_824658C8`
    (the destructor: unregisters our handle and clears +16, so the
    original calls nothing).
  - Pacing as the SDK's: each client starts with `--audio_maxqframes`
    credits (8); a mixer call takes one, a frame SDL plays gives one back.
    The thread runs a client's mixer (`FunctionDispatcher::Execute` with
    the address of a guest word holding the argument, as the SDK did)
    while it has credits, and otherwise waits on one condition variable,
    which SDL's callback notifies after playing a frame.
  - Output as the SDK's driver: an SDL3 stream per client on the default
    device, 6 channels unless the device has 2 or fewer (then the stereo
    fold), converted with the SDK's `conversion.h` and its downmix
    settings and `--audio_mute`.
  - `--native_audio=false` gives the runtime the SDK's audio system again,
    and the three overrides then call the originals.
- The game registers one client (callback `8245FBE0`, argument
  `4006DF6C`) at boot and never unregisters it in an attract race.
- **Measured** (2026-10-14, four alternating uncapped attract races, 45 s
  each from the race's third second, no profiler attached; thread CPU from
  `top -H` 25 s in):

  | Audio | Audio thread CPU | Mean fps | 10th pct | Median | Min |
  |---|---|---|---|---|---|
  | Ours, run 1 | 0.4% | 245 | 194 | 238 | 188 |
  | SDK's (`--native_audio=false`), run 1 | 80.1% | 244 | 198 | 245 | 188 |
  | Ours, run 2 | 0.2% | 243 | 197 | 234 | 189 |
  | SDK's, run 2 | 79.7% | 244 | 197 | 241 | 189 |

  So the core the Audio Worker took is free, and frame rate is unchanged
  on this CPU (16 threads): the spin never held up the render or main
  thread. The log's 10 s counts (`native audio: client 0: ...`) show 1,876
  frames played every 10 s (48,000 / 256 = 187.5 a second) and 0 silence
  fills in the race, 4 frames queued; the only fills were in the last
  window of one run, when `timeout` ended the game.
- **The user listened** (2026-10-14, boot to a race, ~110 s logged): it
  sounds fine; 0 silence fills. `--native_audio` is on by default, so
  release zips use it.
- **Still to do**: a stereo device as well as the 6-channel one here (a
  HyperX headset that SDL reports as 6 channels, through PipeWire).

## Tools

- `--native_audio=false`: the SDK's audio system and render driver, for
  comparison.
- With logging on, our audio logs its endpoint, each client registered,
  and every 10 s each client's frames played, silence fills (frames the
  mixer didn't have ready in time) and frames queued.

- The profile above: launch an attract race in the background, take the
  `itc` process's pid (`pgrep -n -x itc`; `$!` is `timeout`'s), then
  `perf record -e cycles:u -p <pid> -- sleep 15` and
  `perf report --sort comm,sym`; for one thread's call stacks,
  `perf record -e cycles:u --call-graph dwarf,16384 -t <tid>`.
- The SDK logs its audio at debug level (`REXAPU_DEBUG`): the client's
  callback and argument when it registers, the first 10 callbacks and
  frames submitted.
