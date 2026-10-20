# Import Tuner Challenge Static Recompilation

A native Linux and Windows port of *Import Tuner Challenge* (Xbox 360, 2006), made by
statically recompiling the game's PowerPC code to x86-64 with
[ReXGlue](https://github.com/rexglue/rexglue-sdk).

Built with AI assistance (Claude Code).

## Features

- **Native Vulkan renderer.** The game's Direct3D calls are drawn directly
  with Vulkan, this avoids relying on the xenia GPU backend of rexglue and should run faster (WIP)
  
- **Frame interpolation in races.** Game logic still runs at its original
  30 Hz, and frames are blended between ticks, so races play smoothly at an uncapped FPS (`--max_fps=0`), or with a cap specified on the command line with (`--max_fps=n`). `./run.sh --no-interpolation` gives the original
  30 fps game. Be aware that the internal game logic is still expecting a consistent 30hz update rate, if your machine cannot keep up, the game will run in slow motion.
  
## In progress: removing ReXGlue

See [`rexglue-removal-project`](https://github.com/Spooky098529/ITC-Recomp/tree/rexglue-removal-project).

The above branch contains the most up to date functions reference, and is where work on mapping out all of the game's code will go from now on.

## Requirements

- Linux, or Windows 10 or 11 (experimental, see below), on a 64-bit x86
  CPU with SSE4.2 (most CPUs from 2009 on).
- A GPU and driver supporting Vulkan 1.3 or newer (the game won't start
  without it). With up-to-date drivers, that should include for example:
  - NVIDIA GeForce GTX 750 Ti, GTX 900 series or newer
  - AMD Radeon RX 400 series or newer
  - Intel Iris Xe (11th-gen Core laptops) or Arc
  - Steam Deck

  Only the Steam Deck and a Radeon RX 7900 XT have been tested so far.
- Your own copy of the game, extracted from a backup.

## Install

1. Extract the game's files from your backup into a folder.
2. Download the latest zip for your system from
   [Releases](https://github.com/Spooky098529/ITC-Recomp/releases)
   (`linux-amd64` or `win-amd64`) and extract its files into that folder,
   next to `default.xex`.
3. Run `./run.sh` on Linux, or `run.bat` on Windows.

## Running it

| Command | What it does |
|---|---|
| `./run.sh` | Frame interpolation on, up to 144 fps in races, no logging. |
| `./run.sh --max_fps=N` | A different frame cap; `--max_fps=0` is unlimited. |
| `./run.sh --no-interpolation` | The original 30 fps game. |
| `./run.sh --width=1920 --height=1080` | A bigger window, drawn at that resolution instead of the game's 1280x720. |
| `./run.sh --fullscreen` | Fills the screen, drawn at the screen's resolution. With `--width`/`--height`, drawn at that resolution and scaled to the screen. |
| `./run.sh --skip_intro` | Skips the Ubisoft logo movie and speeds through the Genki logo and the title screen's intro. Now and then the game hangs at startup with this; if it does, close it and start it again. |
| `./run-debug.sh` | The same as `run.sh`, with logging on (written to `logs/`). |

Options can be combined, e.g. `./run.sh --skip_intro --max_fps=0`. On
Windows, use `run.bat` and `run-debug.bat` the same way.

By default the game runs in a 1280x720 window at its own resolution, as on
the console. Screens and windows that aren't 16:9 get black bars.

## Known limitations

- Only races are interpolated; menus run at 30 fps, and HUD animations
  advance at 30 Hz (including cockpit elements such as the tachometer).
- In game movies are not shown.
- Tested on Linux on my PC (9800x3d, 7900XT) and the Steam Deck.
- The game probably uses more RAM than necessary, currently it maintains several copies of the xbox 360 memory pool for performance reasons. This is going to be an issue until the rexglue runtime is removed (see "In progress: removing ReXGlue" above). RAM usage can hit about 1gb in races.
- Some 2d elements flicker when races are paused when unlocking the framerate, like logos above cars or smoke trails
## Reporting problems

Play with `./run-debug.sh` (`run-debug.bat` on Windows), then open an
[issue](https://github.com/Spooky098529/ITC-Recomp/issues) describing what
you saw, and attach the newest file from the `logs` folder.

## Building from source

You need clang, lld, CMake (3.25 or newer), Ninja and the Vulkan SDK.

```sh
git clone --recursive https://github.com/Spooky098529/ITC-Recomp.git
cd ITC-Recomp/itc_recomp
cmake --preset linux-amd64-release
cmake --build out/build/linux-amd64-release
```

This builds `itc_recomp/out/build/linux-amd64-release/itc` and
`rexglue-sdk/out/linux-amd64/librexruntime.so`. Put both in the game folder
with `itc_recomp/run.sh` and `itc_recomp/run-debug.sh`.

The Windows version is cross-compiled on Linux (clang and Microsoft's
libraries fetched with xwin); see [docs/windows.md](docs/windows.md).

## Documentation (This is written by LLMs primarily for LLMs)

- [docs/functions.md](docs/functions.md): what's known about the game's
  code, how frame interpolation works, and the diagnostic options.
- [docs/gpu-functions.md](docs/gpu-functions.md): how the game drives the
  GPU.
- [docs/native-renderer.md](docs/native-renderer.md): the native renderer's
  design and progress.
- [docs/native-audio.md](docs/native-audio.md): how the game's audio works
  and the plan to replace the emulated audio.
- [docs/unlocked-res.md](docs/unlocked-res.md): drawing at other
  resolutions.
- [docs/windows.md](docs/windows.md): the Windows build, how it's made and
  tested.
- [docs/rexglue-removal.md](https://github.com/Spooky098529/ITC-Recomp/blob/rexglue-removal-project/docs/rexglue-removal.md): the plan for a native port without ReXGlue, and its progress
  (on the `rexglue-removal-project` branch; the copy on `main` is out of
  date).
