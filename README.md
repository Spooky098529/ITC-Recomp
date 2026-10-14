# Import Tuner Challenge Static Recompilation

A native Linux port of *Import Tuner Challenge* (Xbox 360, 2006), made by
statically recompiling the game's PowerPC code to x86-64 with
[ReXGlue](https://github.com/rexglue/rexglue-sdk). It's not an emulator: the game's own
code runs natively, and it draws with its own Vulkan renderer instead of an
emulated Xbox 360 GPU.

Built with AI assistance (Claude Code).

## Features

- **Native Vulkan renderer.** The game's Direct3D calls are drawn directly
  with Vulkan; no GPU emulation. Motion blur and the light bloom are drawn
  as on the console.
- **Frame interpolation in races.** Game logic still runs at its original
  30 Hz, and frames are blended between ticks, so races play smoothly at up
  to 144 fps (or uncapped). `./run.sh --no-interpolation` gives the original
  30 fps game.
  
## Requirements

- Linux on a 64-bit x86 CPU with SSE4.2 (most CPUs from 2009 on).
- A GPU with Vulkan support.
- Your own copy of the game, extracted from a backup.

(Windows will come later, just trying to get this to work first)

## Install

1. Extract the game's files from your backup into a folder.
2. Download the latest zip from
   [Releases](https://github.com/Spooky098529/ITC-Recomp/releases) and
   extract its files into that folder, next to `default.xex`.
3. Run `./run.sh`.

## Running it

| Command | What it does |
|---|---|
| `./run.sh` | Frame interpolation on, up to 144 fps in races, no logging. |
| `./run.sh --max_fps=N` | A different frame cap; `--max_fps=0` is unlimited. |
| `./run.sh --no-interpolation` | The original 30 fps game. |
| `./run-debug.sh` | The same as `run.sh`, with logging on (written to `logs/`). |

## Known limitations

- Only races are interpolated; menus run at 30 fps, and HUD animations
  advance at 30 Hz.
- In game movies are not shown.
- Colours are a little lighter than on the console (the console's output
  gamma isn't applied yet).
- On Wayland, resizing the window doesn't change the rendered image's size.
- Tested on Linux on a small number of machines so far.

## Reporting problems

Play with `./run-debug.sh`, then open an
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

## Documentation

- [docs/functions.md](docs/functions.md): what's known about the game's
  code, how frame interpolation works, and the diagnostic options.
- [docs/gpu-functions.md](docs/gpu-functions.md): how the game drives the
  GPU.
- [docs/native-renderer.md](docs/native-renderer.md): the native renderer's
  design and progress.
