# Windows build

The Windows build, cross-compiled on Linux and tested under Wine (there's
no Windows machine to test on yet). Started 2026-10-14 on the `windows`
branch, merged into `main` 2026-10-16.

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
- **Against Linux after merging v0.4.0** (2026-10-16, same PC, both
  built from `windows` at `46a85fe`): uncapped attract races at the
  default 1280x720 with the standard flags (`--max_fps=0`), `timeout 25`,
  Linux and Windows alternating, mean fps over the race's seconds 3-18:

  | Run | Linux | Windows (Wine) |
  |---|---|---|
  | 1 | 439.6 | 326.1 |
  | 2 | 439.9 | 324.7 |
  | Mean | 439.8 | 325.4 |

  Windows under Wine is ~26% slower. The per-second rates go up and down
  at the same points in the race on both, so the gap is spread evenly,
  not in a few spots. Where it comes from (Wine, the MSVC runtime,
  ThinLTO being off) is still unmeasured; only real Windows would say
  what players there get. (ThinLTO has since been turned on, below.)
- **ThinLTO** (`ITC_WIN_LTO`, on in the `win-amd64-cross-release` preset;
  2026-10-16): a separate option from Linux's `ITC_LTO`, whose link
  options are for ELF `ld.lld`; this one gives `lld-link`
  `/lldltocache:<dir>`, and only applies when `WIN32`. `rexruntime.dll`'s
  exports (the SDK's `WINDOWS_EXPORT_ALL_SYMBOLS`) are then read from
  bitcode objects: ~6900 instead of ~10600, the difference inline and
  template code (the standard library, CLI11, spdlog). The import library
  comes from the same list and `itc.exe` links and runs, so nothing it
  uses is missing. Same PC and method as above, under Wine, alternating,
  four runs each: 329.9, 324.6, 321.6, 326.1 without (mean 325.6);
  335.5, 332.8, 335.6, 338.3 with (mean 335.6, +3.1%). Every run with it
  was faster than every run without.
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
- **Settings come from `itc.toml`** next to `itc.exe`, as on Linux.
- **The MSVC runtime's `<chrono>`** has no `file_clock::to_sys`: a
  save's time (`src/saves/saves.cpp`) goes through
  `clock_cast<system_clock>`, which both have (the Windows build failed
  from B4 until 2026-10-21, on `rexglue-removal-project` only).
- **The log** (A2, `src/base/log.cpp`): thread ids from
  `GetCurrentThreadId`, local time from `localtime_s`; the take-over of
  the SDK's log file finds its sink across the DLL boundary
  (`dynamic_pointer_cast`). Checked under Wine 2026-10-21: one log with
  ours and the SDK's lines, no duplicates.
- **The Visual C++ runtime ships in the zip** (`MSVCP140.dll`,
  `MSVCP140_ATOMIC_WAIT.dll`, `VCRUNTIME140.dll`, `VCRUNTIME140_1.dll`:
  what the three binaries import; Windows 10 and 11 have the Universal
  CRT, `api-ms-win-crt-*`, but not these). xwin doesn't splat them, so
  `itc_recomp/tools/fetch_vc_redist.py` takes them from the same Visual
  Studio manifest xwin reads: the redist package of the MSVC version xwin
  splatted (`Microsoft.VC.<version>.CRT.Redist.X64.base`, 14.44.35211 on
  2026-10-16), SHA-256 checked. Microsoft's license allows shipping them.
  Checked under Wine with them forced over Wine's own
  (`WINEDLLOVERRIDES=...;msvcp140,msvcp140_atomic_wait,vcruntime140,vcruntime140_1=n`,
  `WINEDEBUG=+loaddll` shows them loaded from the zip's folder): boots
  and races.

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
G="Z:$(pwd | tr / '\\')\\itc\\Import Tuner Challenge (USA)"
timeout 90 wine $B/itc.exe \
  --native_audio=true --game_data_root="$G" --max_fps=144 \
  --log_fps=true --interpolate_draw_state=true --skip_intro --attract_after_s=1
```

Logs go to `logs/` next to `itc.exe`. Wine's own messages (stderr) are
mostly harmless `fixme` lines. To test a package as players get it,
package as below, put it in the game folder and run `wine itc.exe` there
(add `--log_level=info` for logs; without `--game_data_root`, the game's
files are looked for next to `itc.exe`).

Test zips, as for Linux:

```sh
name=itc-<label>-win-amd64; D=itc_recomp/out/dist/$name; B=itc_recomp/out/build/win-amd64-cross-release
rm -rf $D $D.zip; mkdir -p $D
cp $B/itc.exe $B/rexruntime.dll $B/SDL3.dll itc_recomp/itc.toml $D/
runtime=$(for f in itc.exe rexruntime.dll SDL3.dll; do llvm-readobj --coff-imports $D/$f | sed -n 's/^ *Name: //p'; done \
  | grep -iE '^(msvcp|vcruntime|concrt|vccorlib)' | sort -fu)
python3 -I itc_recomp/tools/fetch_vc_redist.py itc_recomp/out/xwin-sdk $D $runtime
(cd $D && python3 -I -m zipfile -c ../$name.zip *)
```

## CI

`.github/workflows/build-windows.yml` (2026-10-16) builds as above on
`ubuntu-24.04`: clang 20 from apt.llvm.org (its `bin` directory on `PATH`,
for the plain tool names the toolchain file uses), xwin's splat cached
between runs (~630 MB; the key names xwin's version, bump its `-v1` for a
newer CRT), then the zip with the runtime DLLs. Packaging fails if a
binary imports one of our DLLs or a runtime DLL the zip lacks. It runs on
pushes to `main`, pull requests, by hand, and on `v*` tags, like
`build-linux.yml`; on a tag both zips go on the same release. A build
takes ~15 minutes: sccache is set up but caches nothing, as it takes
`-Xmicrosoft-windows-sys-root`'s directory for a second input file
("multiple input files" for every compile; clang has no joined form of
the option). Passing the CRT and SDK through `INCLUDE`/`LIB` instead might
fix it; the user was fine with the longer builds (2026-10-16). First run
checked 2026-10-16: its zip boots and races under Wine from an empty
folder, the runtime DLLs loaded from it (`WINEDEBUG=+loaddll`).

## To do

- The user plays under Wine (keyboard, gamepad, menus, a driven race).
- The frame rate under Wine (above).
