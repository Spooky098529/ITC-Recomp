# Windows build

The Windows build, cross-compiled on Linux and tested under Wine (there's
no Windows machine to test on yet). Started 2026-10-14 on the `windows`
branch.

## Status

- **Runs under Wine** (wine-staging 11.19, RX 7900 XT, 2026-10-14): boot,
  title screen, attract races, our renderer (exclusive mode, no GPU
  plugin) and our audio (WASAPI through Wine's PulseAudio output, 0
  silence fills). The log looks as on Linux.
- **Frame rate under Wine**, attract race, 60 s each:

  | Cap | Mean | 10th pct | Median | Min | Max |
  |---|---|---|---|---|---|
  | 144 | 145 | 139 | 147 | 106 | 151 |
  | none | 149 | 123 | 146 | 117 | 197 |

  Uncapped, that's well under native Linux's ~235 (see `CLAUDE.md`). Not
  looked into: Wine's overhead, the MSVC runtime (its mutexes and
  atomics) and ThinLTO being off (~2% on Linux) are all candidates.
  Profile before changing anything. On real Windows it's unmeasured.
- Not tested yet: playing with keyboard and gamepad, the menus, real
  Windows.

## What differs from Linux

- **SDL is a DLL** (`SDL3.dll`, SDK patch `0003`): a DLL only exports its
  own code, so the SDK's static SDL ended up in both `rexruntime.dll` and
  `itc.exe`, and our window and audio would have used another SDL than the
  SDK's event loop (no input, `SDL_RunOnMainThread` never running). The
  patch also turns on SDL's Vulkan support on Windows (the SDK's presenters
  make surfaces from the HWND; ours goes through SDL).
- **Vulkan, not D3D12**: the SDK defaults to D3D12 on Windows, which leaves
  out the Vulkan headers our renderer and the shader translator need. The
  preset sets `REXGLUE_USE_VULKAN=ON`, `REXGLUE_USE_D3D12=OFF`.
- **The frame cap's sleep**: Windows rounds sleeps up to the system timer
  (often 15.6 ms), so `frame_pacer.cpp` sleeps on a high-resolution
  waitable timer until 1 ms before and spins the rest.
- **The disk cache** (`cache/` next to the executable, `disk_cache.cpp`):
  the executable comes from `GetModuleFileNameW` instead of
  `/proc/self/exe`, and the cache files are opened with `native::OpenFile`,
  which uses `_wfopen` on Windows (a `std::filesystem::path` is a wide
  string there, so `std::fopen(path.c_str())` doesn't compile). Checked
  under Wine 2026-10-16: written on the first run, loaded on the second.
- **Stubbed on Windows** (debug tools only): the guest memory scans
  (`--find_per_frame_state` and friends, the state capture), which read
  `/proc/self/maps`, and `--log_frame_times`' thread CPU time (reads 0).
- **`run.bat` sets its defaults as environment variables** (`REX_<OPTION>`,
  e.g. `REX_MAX_FPS`): the SDK reads them after the command line, so an
  option given there overrides its default. (`run.sh` leaves a default out
  instead, as the parser rejects an option given twice; batch can't do
  that easily, since it splits arguments at `=`.) `--no-interpolation` is
  handled by the script and also reaches the game, which ignores it.
- Players need the Visual C++ runtime (`MSVCP140.dll`, `VCRUNTIME140.dll`;
  Wine has its own). Not shipped yet.

## Building (on Linux)

Once, the Microsoft CRT and Windows SDK, with
[xwin](https://github.com/Jake-Shadle/xwin) (accepts Microsoft's license;
~1 GB, kept in `itc_recomp/out/`):

```sh
X=itc_recomp/out/tools/xwin-dl/xwin-0.10.0-x86_64-unknown-linux-musl/xwin  # from its GitHub release
$X --accept-license --arch x86_64 --cache-dir itc_recomp/out/tools/xwin-cache \
  splat --use-winsysroot-style --preserve-ms-arch-notation --output itc_recomp/out/xwin-sdk
python3 -I itc_recomp/tools/xwin_case_links.py itc_recomp/out/xwin-sdk rexglue-sdk itc_recomp/src
```

The second step adds links for headers the code includes with another case
than the SDK's files (`ObjBase.h` for `objbase.h`): Windows' filesystem
ignores case. Run it again if a build fails with a Windows header "not
found".

Then:

```sh
cd itc_recomp && cmake --preset win-amd64-cross-release && cd ..
cmake --build itc_recomp/out/build/win-amd64-cross-release -j$(nproc)
```

`cmake/windows-cross.cmake` is the toolchain: `clang`/`clang++` for
`x86_64-pc-windows-msvc` with `-Xmicrosoft-windows-sys-root` (clang-cl's
`/winsysroot`: the SDK's headers after clang's own, and its libraries),
`lld-link`, `llvm-ar` (CMake gives the archiver `ar` arguments) and
`llvm-rc` (resources are preprocessed with `clang++`, so `-I`, not `/I`).
A changed `CMAKE_*_FLAGS_INIT` needs `-U CMAKE_<LANG>_FLAGS` (or a fresh
build directory): the old value stays cached.

The output is `itc.exe`, `rexruntime.dll` and `SDL3.dll` in the build
directory (`rexgpu-xenos.dll` is built but not needed).

## Testing under Wine

A Wine prefix of our own, so `~/.wine` is left alone; Mono and Gecko
aren't needed:

```sh
export WINEPREFIX=$PWD/itc_recomp/out/wineprefix WINEDLLOVERRIDES="mscoree,mshtml=" \
  XDG_DATA_DIRS=/usr/local/share:/usr/share
B=itc_recomp/out/build/win-amd64-cross-release
G='Z:\home\sam\repos\recomptest\itc\Import Tuner Challenge (USA)'
timeout 90 wine $B/itc.exe --native_renderer=true --native_renderer_exclusive=true \
  --native_audio=true --game_data_root="$G" --mnk_mode=true --max_fps=144 \
  --log_fps=true --interpolate_draw_state=true --skip_intro --attract_after_s=1
```

Logs go to `logs/` next to `itc.exe`. Wine's own messages (stderr) are
mostly harmless `fixme` lines. To test the scripts, package as below and
run `wine cmd /c run-debug.bat` in the package's folder (with
`--game_data_root=...` if it isn't in the game folder).

Test zips, as for Linux:

```sh
name=itc-<label>-win-amd64; D=itc_recomp/out/dist/$name; B=itc_recomp/out/build/win-amd64-cross-release
rm -rf $D $D.zip; mkdir -p $D
cp $B/itc.exe $B/rexruntime.dll $B/SDL3.dll itc_recomp/run.bat itc_recomp/run-debug.bat $D/
(cd $D && python3 -I -m zipfile -c ../$name.zip itc.exe rexruntime.dll SDL3.dll run.bat run-debug.bat)
```

## To do

- The user plays under Wine (keyboard, gamepad, menus, a driven race).
- A Windows job in CI (`.github/workflows`), so tagged releases get a
  Windows zip too; the SDK's own `_build-platform.yaml` builds Windows on a
  `windows-latest` runner with clang from Chocolatey.
- The frame rate under Wine (above).
- ThinLTO on Windows (`ITC_LTO`'s link options are written for ELF lld).
- The Visual C++ runtime for players (ship the DLLs or point to Microsoft's
  installer).
- README: Windows install and requirements.
