# Native movies

The game's movies played through upstream FFmpeg instead of the movie
library linked into the game, and their frames drawn with our renderer
(`--play_movies_with_ffmpeg`, on by default since 2026-10-25; "Progress"
below). Part of B7: it moves the game off
~580 recompiled functions (about 11% of the work left by functions,
`docs/coverage.md`), as `--use_hardcoded_shaders` did for the HLSL
compiler. The functions are in `functions/movies.md`.

## The movies

The 12 files in `DATA/TXR/MOVIE`, all the same format (ffprobe,
2026-10-25): an ASF container with WMV3 video (WMV9, 1280x720, 29.97
fps, 8 Mbit/s) and WMA v2 audio (stereo, 48 kHz, 192 kbit/s). From 6.5 s
(`UBI_LOGO_SD`, not in the game's path table) to 107 s (the staff rolls).
FFmpeg's `asf` demuxer and its `wmv3` and `wmav2` decoders read them;
ours is built with only `xma2` (`itc_recomp/cmake/ffmpeg.cmake`).

## How the game plays one

Only through the movie class (`Movie`, vtable `0x82002910`, kept at
`0x826C1CDC`), from the boot sequence's handlers (`Boot::StartMovie`,
`Boot::WaitForMovie`, `Boot::EndMovie`); its state is at +308 (0 none, 1
loaded or ended, 2 playing, 3 paused). The sequence is in
`functions/movies.md`. What the library does under it:

- **Opening** (`Movie::Open`, slot 8, on a worker under the device lock):
  the wrapper at +8 makes the player for the path.
- **Drawing** (`Movie::RenderFrame`, slot 0, on the render thread from
  command 28, which `Movie::OnTick` queues each tick while playing): the
  library draws the next frame outside the command list
  (`gpu-functions.md`, "Movies"): three 8-bit planes (Y 1280x720, the two
  chroma 640x360 with a pitch of 768) in three rotating sets, two
  triangles through `DrawVertices` from `0x8248466C`, with its own
  vertex and pixel shaders (made by `sub_824846D8` from microcode, not
  compiled). Our renderer doesn't see that draw, so the boot movie isn't
  shown (`native-renderer.md`, known issues).
- **Sound:** one XAudio source voice (`sub_82483770`: `XAudioInitialize`,
  then the voice made by `sub_8245A768`; its format set from the stream
  by `sub_82483958` through `sub_82459FF8`; buffers submitted by
  `sub_82483A30` through `sub_8245A050` and started with `sub_8245A168`;
  stopped, flushed and released by `sub_82483BA0` and `sub_824838B8`). No
  volume is set: the voice plays at 1.0, and the category volume the SDK
  gives is 1.0.
- **The end:** the draw returns `0x16660026` at the file's end, and the
  class sets state 1.

## The plan

Replace the movie class's methods with native code at the class (the
user's choice, 2026-10-25: option B, the frames piped to the renderer).
The game's handlers, the state at +308, `Movie::OnTick`'s command 28 and
the pad press that skips stay as they are; the class's constructor and
the wrapper it makes stay, unused. So nothing the game sees changes, and
the library stops running.

1. **FFmpeg:** the `asf` demuxer and the `wmv3` and `wmav2` decoders
   added to the build.
2. **A player** (`src/movies/`, plain C++): opens a file, decodes on a
   host thread a few frames ahead (the YUV planes with their times, and
   the PCM), and keeps a clock that starts with `Movie::Start` and stops
   while paused.
3. **The class's methods** (`src/hooks/movies.cpp`, thin adapters behind
   a flag): `Movie::Play` (slot 1: open, state 1), `Start`, `Stop`,
   `Pause`, `Resume`, `State` and `Release` (slots 2-7: the same state
   changes, done to our player), and `Movie::RenderFrame` (slot 0: the
   frame due at the clock's time handed to the renderer; at the end,
   state 1). `Movie::Open` (slot 8) is no longer called. The
   `--skip_intro` wrappers in `src/hooks/title_screen.cpp` move into
   these: the stall they work around came from the library's player
   loop.
4. **The draw:** the renderer uploads the three planes and draws them
   with a YUV to RGB shader, at command 28's place in the frame (keeps
   the order with anything drawn over a movie; recommended, the user's
   call to confirm). The conversion's numbers come from the library's
   own pixel shader and its constants, so the colors match.
5. **The sound:** the PCM mixed into our output (`src/audio/output_mix.h`)
   at 1.0, as the library's voice was; resampled if the output's rate
   isn't 48 kHz.
6. **Checks:** harness cases for the state changes; the user's boot and a
   stage opening (the picture, its timing and the sound); then a fresh
   coverage session, as after the hardcoded shaders.

## Progress

Done 2026-10-25, on `b7-loop-24-10-26-4`, steps 1-5 of the plan:

- **FFmpeg** (`ecb9968`): `tools/ffmpeg_config.sh` configures the
  `asf` demuxer and the `wmv3` and `wmav2` decoders too, with
  libavformat (`itc_avformat`, no protocols: the bytes come through our
  own AVIOContext); the configs made again for both platforms.
- **The player** (`src/movies/player.h`, the library `itc_movies` on our
  FFmpeg alone, as `itc_xma`): decodes on a thread of its own, six frames
  and two seconds of sound ahead of a steady clock that `Start` starts and
  `Pause` holds; `CurrentFrame` gives the frame due, `Ended` says when the
  last one's time is past, `MixAudio` the sound. `itc_movie_check`
  (`tests/movie_check.cpp`, built on request) plays a movie in real time,
  one frame a 30 Hz tick: the logo ran 9.07 s (the file 9.05), 256
  frames, its frame at 2 s the same as the system `ffmpeg`'s.
- **The draw** (`Passes::DrawMovie`, `glsl/movie.frag`): the planes
  uploaded per frame in flight and drawn over render target 0's top-left
  1280x720 guest pixels (scaled), with the library's BT.709 numbers, in a
  rendering scope of its own like a resolve's; the resolve's pipeline
  set-up shared (`MakeFullscreenPipeline`).
- **The sound** (`audio/output_mix.h`, `native_audio.cpp`): the movie's
  stereo added to the front channels before the fold and the clamp, for
  every frame the output plays (also where the mixer had none).
- **The class** (`src/movies/movie.cpp`, adapters in
  `src/hooks/movies.cpp`): slots 0-7. `--skip_intro`'s wait for the first
  `RenderFrame` moved there (`TitleScreen_MovieRendered`).

Checked by play, not with a harness: the work is FFmpeg's, so nothing
comes out byte for byte as the library's, and the only shared output, the
state at +308, shows in play (a movie that didn't start, end or move on).
The user's boot (2026-10-25): the logo shown in our window for the first
time, 9.09 s, then the front end; "looked fine".

## Open

- A stage opening (before a race, where render-only passes may run
  between ticks: command 28 is queued on ticks only, so a movie drawn once
  a tick might flicker there) and the movies' sound: the user's drive.
- A coverage session afterwards, to take the library's ~580 functions
  out of the work left.

- The library's YUV to RGB math (done): built into its pixel shaders
  (`0x8204D4F8` BT.601 for frames up to 576 lines, `0x8204D6B8` BT.709
  above, picked by `sub_824840C8`), no constants set in `sub_824842C8`.
- Whether anything is drawn over a movie (the stage openings): the
  user's drive will show.
- Timing: the library plays at the file's rate on its own clock; ours
  follows the frames' times from `Movie::Start`. Frames are drawn once a
  tick (command 28), as now; whether to draw on every rendered frame is
  for later.
