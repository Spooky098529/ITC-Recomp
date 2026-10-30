# Known functions: the data load at boot

The game's data load at boot (`BootData::Load`, `sub_82106618`): once
(its flag `0x826C5B8C`), from a front-end state machine's transition
(`sub_82220C88`, registered by `sub_822213B0` from the screen start
`sub_82224C70`; neither has rows: `../loops/leads.md`). In order: the
shader pairs (`Shaders::LoadAll`, `models.md`), the text
(`Text::Load`, `strings-and-font.md`), the objects the game keeps for
the whole session (`BootData::CreateObjects`, `sub_82105868`: player 0's
save data, `save-data.md`; the race data, `race-setup.md`; the save
defaults' tables; the input recordings; probably the kept replay; the
data tables, `data-tables.md`), the loading screen's three UI movies
and another set of three, the car parameters (`Params::Init`, `Load`,
`car-specs.md`), with `GlobalMemoryStatus` before and after into a frame
buffer it never reads (`functions.md`, Dropped). Those callees keep their
rows in their files (shared: `Shaders::LoadAll` and its
`sub_820A07F0`, `Text::Load`, `Font::Load`, `Strings::Load` in
`files-and-saves.md`, `SaveData::Create`, `RaceData::RaceData`,
`RaceData::ResetGlobal`, `DataTables::Init` and `Load`, `UiMovie::UiMovie`,
`sub_8211A9E8` and `UiMovie::Load` in `ui-movies-game.md`,
`UiMovie::SetRenderer` in `ui-movies.md`, `Params::Init` and `Load`);
this file has the load itself, the objects' makers and constructors, the
save defaults' tables (the bit sets `Defaults::Table*` in `save-data.md`
return: who filled them wasn't read until now), the two UI movie sets,
the two static copies of the replay object (C runtime static set-ups,
`_cinit`'s tables, `c-runtime.md`) and the unload that mirrors the load
(never in play: `main` doesn't return).

What a native port meets (read 2026-10-30, the B7 loop's mapping agent):
no kernel imports, kernel objects or locks of its own. The allocations
are `operator new` (`sub_825A6880`, ours over the native heap: the
user's call, 2026-10-22); the loads are `Archive::Load` and
`Archive::AssetSize` (ours, `archive.md`, the archive's semaphores host
ones by the user's call, 2026-10-28) and `VirtualAlloc` (ours,
`heap.md`); `GlobalMemoryStatus` is already Dropped (a native caller
leaves it out, as `App::StartUp`'s plain C++ does). The only indirect
calls are `sub_82291328`'s four: the app's interface `[0x826BABA0]`'s
slot 8 (`sub_820944D8`) and its answer's slots 1 and 2 (the command list
and the renderer), the same calls `sub_8211AD00`'s override makes
(`ui-movies-game.md`) and the screen fades' adapter (`mode-tick.md`).
Failed allocations are kept as the original leaves them: the makers
store 0 and some still write a word near address 0 (`sub_82105508`,
`sub_82105560`, as `SaveData::Create`); B7 changes no behavior (the
user's call). The float constants are `.rdata`'s `0x82000A30` (1.0)
and `0x82000A34` (0.0). Blockers outside the file: `UiMovie::Load`
(`sub_8211E798`, `ui-movies-game.md`, also called by `UiMovies::Open`)
and `Strings::Load` (`sub_8210A8D0`, `files-and-saves.md`, ten callers);
both are ready groups themselves. `Record5232::Record5232`
(`sub_82104C20`) is shared inside the file by the input recordings, the
replay and the two static set-ups. Nothing here needs a call the user
hasn't made.

Made native by the B7 loop (2026-10-30, `src/data_load/`,
`--native_data_load`; the shared groups under their files' flags): the
loads go through `archive::LoadCall` (`hooks::NativeArchiveLoad`:
`archive::Queue(BuildLoad(...))`, or a harness's recorder when one is set,
so harnesses never queue for real; `harness::ArchiveLoads` records both
versions' calls). `BootData::Load` and `CreateObjects` call their callees
through their adapters (as `App::StartUp`), and those adapters hand their
call to `hooks::StartUpRecorded` first, for the harness. A harness running
a recompiled caller of a native load (`Text::Load`'s `Strings::Load`) turns
the callee's flag off, or its adapter would queue the load for real (found
when `Strings::Release`'s cases saw a loaded flag written into freed
memory).

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

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_82106618` | `BootData::Load` | Overridden | No arguments read (r3 = `[0x826C5B60]` from its caller, unused). Once: when the flag `0x826C5B8C` is 0, sets it to 1 and calls, in order, `GlobalMemoryStatus` (`sub_82294DA0`, Dropped, `functions.md`; a 32-byte buffer at its frame + 80, never read), `Shaders::LoadAll` (`sub_820A08A0`, `models.md`), `Text::Load` (`sub_8210ADF8`, `strings-and-font.md`), `BootData::CreateObjects`, `LoadingScreen::Load` (`sub_82247B98`), `sub_82291328`, `Params::Init` (`sub_820C9960`) and `Params::Load` (`sub_820CC2A8`, `car-specs.md`), then `GlobalMemoryStatus` again into the same buffer; else nothing. Only caller `sub_82220C88` (a front-end state machine's transition, registered by `sub_822213B0`; `../loops/leads.md`), which then sets `[0x82814E28]`'s +4 = 0, +12 = -1, +8 = 12. 40 lines. Read in full (B7 loop's mapping agent, 2026-10-30). Native (B7 loop, 2026-10-30): `BootDataLoad` in `src/data_load/boot.cpp`, the adapter in `src/hooks/data_load.cpp` behind `--native_data_load`, `--native_heap` and the archive's queue native; its callees through their adapters from a frame like its own (as `App::StartUp`'s), Shaders::LoadAll given r3 = its frame + 80 as the original leaves it; both GlobalMemoryStatus calls left out (Dropped). r3 its frame + 80 (as it came when the flag was set). Harness `data_load` (every callee recorded by its adapter, `hooks::StartUpRecorded`). |
| `sub_82105868` | `BootData::CreateObjects` | Overridden | No arguments; from `BootData::Load` only. `SaveData::Create(0)` (`sub_821057C8`, `save-data.md`: player 0's), `RaceData::Create`, `RaceData::ResetGlobal` (`sub_821021A8`, `race-setup.md`), `Defaults::Init`, `InputRecordings::Create`, `Replay::Create`, `DataTables::Init` (`sub_82116DB0`) and `DataTables::Load` (`sub_82117240`, `data-tables.md`). 30 lines. Read in full. Native (B7 loop, 2026-10-30): `CreateObjects` in `src/data_load/boot.cpp`, the adapter behind `--native_data_load`, `--native_heap` and the archive's queue native; its eight callees through their adapters (which hand their call to `hooks::StartUpRecorded` first, for the harness), so those adapters stay. r3 DataTables::Load's. Harness `data_load`. |
| `sub_82102208` | `RaceData::Create` | Overridden | No arguments: 0xA3B00 bytes from `operator new` (`sub_825A6880`); when not null, `RaceData::RaceData` (`sub_821021B8`, `race-setup.md`); the block, or 0, stored at `0x826C5B2C` (the race data, `known-data.md`). From `BootData::CreateObjects` only. Leaf apart from those. 29 lines. Read in full. Native (B7 loop, 2026-10-30): `RaceDataCreate` in `src/data_load/objects.cpp` (operator new's and `RaceData::RaceData`'s plain C++), the adapter in `src/hooks/data_load.cpp` behind `--native_data_load` and `--native_heap`. Harness `data_load`. |
| `sub_820CCDC0` | `Defaults::Init` | Overridden | No arguments; once at boot from `BootData::CreateObjects`. Fills the save defaults' bit sets (bit i is word i / 32's bit i % 32, set by an inlined `bitset::set` that skips negative i, kept as it is), base `0x826C4064`: `0x826C3044` (15 words, `Defaults::Table3044`, `save-data.md`): cleared, then fixed bits in its words 0-5, 12 and 13 and bits 404-409; `0x826C3C24` (7 words, `Defaults::Table3C24`): cleared, bits 0-60; `0x826C50E4` (7 words, `Defaults::Table50E4`): `Bits224::Fill` (bits 0-198); `0x826C50A8` (15 words, `Defaults::Table50A8`): cleared, fixed bits in words 0-5 and 12, bits 189-388, 410-434 and 435-453; the 48 bit sets of 15 words at `0x826C3080` (60 bytes apart): each cleared, then a range of bits by a jump table on the index (0: 0-19, 1: 0-9, 2: 10-19, 3: 20-33, ..., 29: none, 47: 435-453); then `0x826C4064` (15 words) cleared, ORed with entries 17, 35, 38, 41 and 47, filled by `Bits480::Fill` (bits 0-453) and cleared again (`andc` of each word with itself: it ends 0). `0x826C3E34` (`Defaults::Table3E34`) isn't written here. Probably the items or parts a new game offers (`Progress::SetDefaults` copies these into the save data), not checked. No calls besides the two fills. 713 lines (the bits set one by one). Read in full apart from the fixed bits' values and the jump table's middle ranges (in the code at `0x820CD29C`-`0x820CD4E8`). Native (B7 loop, 2026-10-30): `src/data_load/defaults.cpp` (`DefaultsInit`), the adapter in `src/hooks/data_load.cpp` behind `--native_data_load`; the bits each set ends with listed as ranges (every set is cleared first, so they are what the original leaves), the five indices on its frame (+80 to +96) as the original leaves them; r3 = 0x826C4064 sign-extended, as `lis`/`addi` leave it. Harness `data_load`. |
| `sub_820CCD10` | `Bits224::Fill` | Native | r3 = a 7-word bit set: the words cleared, then bits 0-198 set one by one (the same inlined `bitset::set`). Leaf. Only caller `Defaults::Init` (on `0x826C50E4`). 30 lines. Read in full. Native (B7 loop, 2026-10-30): `Bits224Fill` in `src/data_load/defaults.cpp`, no adapter (only `Defaults::Init`'s plain C++ calls it). Harness `data_load`. |
| `sub_820CCD68` | `Bits480::Fill` | Native | r3 = a 15-word bit set: the words cleared, then bits 0-453 set one by one. Leaf. Only caller `Defaults::Init` (on `0x826C4064`). 30 lines. Read in full. Native (B7 loop, 2026-10-30): `Bits480Fill` in `src/data_load/defaults.cpp`, no adapter (only `Defaults::Init`'s plain C++ calls it). Harness `data_load`. |
| `sub_82105508` | `InputRecordings::Create` | Overridden | No arguments: 0x2225C0 bytes from `operator new`; when not null, `InputRecordings::InputRecordings`, else 0; the block stored at `0x826C5B4C` (the input recordings, `known-data.md`, `racer-controllers.md`), then the word at block + 0x2225B0 = 0 (also when the allocation failed: then at address 0x2225B0). From `BootData::CreateObjects` only. 32 lines. Read in full. Native (B7 loop, 2026-10-30): `InputRecordingsCreate` in `src/data_load/objects.cpp` (the store near address 0 after a failed allocation kept), the adapter behind `--native_data_load` and `--native_heap`. Harness `data_load`. |
| `sub_821054A8` | `InputRecordings::InputRecordings` | Native | r3 = the block (0x2225C0 bytes): `Record5232::Record5232` on seven records of 5,232 bytes from +0x20FE90 and seven more from +0x2196A0 (after the two players' recordings, 0x107AC4 bytes each at +0); returns r3. Nothing else is set (the recordings and their counts are left as `operator new` gives them). Only caller `InputRecordings::Create`. 39 lines. Read in full. Native (B7 loop, 2026-10-30): `InputRecordingsConstruct` in `src/data_load/objects.cpp`, no adapter (only `InputRecordings::Create`'s plain C++ calls it). Harness `data_load`. |
| `sub_82105560` | `Replay::Create` (probably) | Overridden | No arguments: 0x13C8F0 bytes from `operator new`; when not null, `Record5232::Record5232` on seven records of 5,232 bytes from +0x2BF10 (inline, no constructor of its own); the block, or 0, stored at `0x826C5B50`, then the word at block + 0x2B600 = 1 (also when the allocation failed: then at address 0x2B600). Probably the replay kept after a race (`sub_82104F68` fills it from the race data and the recordings, `../loops/leads.md`); two static objects of the same layout are made by `sub_825D9E60` and `sub_825D9F68`. From `BootData::CreateObjects` only. 46 lines. Read in full. Native (B7 loop, 2026-10-30): `ReplayCreate` in `src/data_load/objects.cpp` (r3 the last record, as the original leaves it), the adapter behind `--native_data_load` and `--native_heap`. Harness `data_load`. |
| `sub_82104C20` | `Record5232::Record5232` | Overridden | r3 = a 5,232-byte record: 1.0 (`[0x82000A30]`) into +156, +172, +188, +204 and the four words +252, +268, +284, +300 of each of four 96-byte rows (+252 to +588); 0.0 (`[0x82000A34]`) into +636, +652, +684, +716, +748, +764, +796, +812, +828, +972, +988, +1020, +1100, +1116; then `Member1136::Member1136` (+1136) and `Member4080::Member4080` (+4080); returns r3. Probably the w words of vectors and matrices (as `Member56896::Member56896`, `game-modes.md`). Callers: `InputRecordings::InputRecordings`, `Replay::Create`, `sub_825D9E60`, `sub_825D9F68` (seven records each). 85 lines. Read in full. Native (B7 loop, 2026-10-30): `Record5232Construct` in `src/data_load/records.cpp` (the floats copied as lfs/stfs leave them), the adapter in `src/hooks/data_load.cpp` behind `--native_data_load` (the static set-ups' fallbacks still call it). Harness `data_load`. |
| `sub_82102888` | `Member1136::Member1136` | Native | r3 = a record + 1136 (a part of 2,944 bytes up to +4080, probably): 0.0 (`[0x82000A34]`) into +12, +28, +44, +76, +108, +124, +140, +348, +364, +380, +396; four times (368 bytes apart from +492) +476, +492, +524, +540, +556, +572, +748; then +1884, +1900, +1916, +1932, +1948, +1964, +2012 to +2092 by 16, +2428, +2444, +2492, +2508 to +2572 by 16 (five), +2876 to +2924 by 16 (four). Leaf; no return value set (r3 unchanged). Only caller `Record5232::Record5232`. 109 lines. Read in full (moved here from the race data's lead in `../loops/leads.md`). Native (B7 loop, 2026-10-30): `Member1136Construct` in `src/data_load/records.cpp`, no adapter (only `Record5232::Record5232`'s plain C++ calls it). Harness `data_load`. |
| `sub_82104690` | `Member4080::Member4080` | Native | r3 = a record + 4080: `Part464::Part464` on +16 and +480, then 0.0 (`[0x82000A34]`) into +956, +972 (the second part's +476, +492), +1068, +1084, +1116, +1148; returns r3. Only caller `Record5232::Record5232`. 43 lines. Read in full. Native (B7 loop, 2026-10-30): `Member4080Construct` in `src/data_load/records.cpp`, no adapter. Harness `data_load`. |
| `sub_821042A0` | `Part464::Part464` | Native | r3 = a part (464 bytes apart, probably its size): 0.0 (`[0x82000A34]`) into +12, +28, +44 to +156 by 16 (eight), +188 to +348 by 32 (six), +396, +412 to +444 by 16 (three). Leaf. Only caller `Member4080::Member4080` (twice). 51 lines. Read in full. Native (B7 loop, 2026-10-30): `Part464Construct` in `src/data_load/records.cpp`, no adapter. Harness `data_load`. |
| `sub_825D9E60` | `Replay826D8520::StaticInit` | Overridden | No direct caller: a C runtime static set-up (`_cinit`'s initializer tables, `c-runtime.md`), once at boot. `Record5232::Record5232` on seven records of 5,232 bytes from `0x82704430` (a static object at `0x826D8520` with `Replay::Create`'s layout: its records at +0x2BF10), then `atexit(0x825DB170)` (`sub_825A7068`, ours, `c-runtime.md`); r3 = atexit's. What uses the object wasn't looked for. Read in full. Native (B7 loop, 2026-10-30): `StaticReplayAInit` in `src/data_load/objects.cpp` (`Record5232Construct` called directly; atexit through `c_runtime::AtexitFrom`, the game's when the native one can't, as `SignInWatch::StaticInit`, `mode-tick.md`; its frame's saved words not written), the adapter in `src/hooks/data_load.cpp` behind `--native_data_load` with native atexit; it stays (reached only through `_cinit`'s table). Ran natively at boot (itc_2643). Harness `data_load`: on the game's object and exit table, as at boot and random, put back after each side. |
| `sub_825D9F68` | `Replay82816E50::StaticInit` | Overridden | The same for a static object at `0x82816E50` (records from `0x82842D60`), `atexit(0x825DB1B0)`. No direct caller (`_cinit`'s tables), once at boot. Users not looked for. Read in full. Native (B7 loop, 2026-10-30): `StaticReplayBInit` in `src/data_load/objects.cpp`, as `sub_825D9E60`'s (the adapter stays: reached only through `_cinit`'s table). Ran natively at boot (itc_2643). Harness `data_load`, as `sub_825D9E60`'s. |
| `sub_82247B98` | `LoadingScreen::Load` (probably) | Overridden | No arguments; from `BootData::Load` only. The loading screen's movie count (`0x82815EA8`, the list's +64) = 0 and its shown flag (`0x82815E60`) = 0, then `LoadingScreen::AddMovie` with assets 22575, 22574 and 22576 (the list at `0x82815E68`, `known-data.md`; the screen is `sub_822478D0`'s, `../loops/leads.md`). 29 lines. Read in full. Native (B7 loop, 2026-10-30): `LoadingScreenLoad` in `src/data_load/boot.cpp`, the adapter behind `--native_data_load`, `--native_heap` and the archive's queue native; r3 the last serial. Harness `data_load` (the movies made for real, the loads recorded). |
| `sub_82247AA0` | `LoadingScreen::AddMovie` (probably) | Native | r3 = an asset id: a movie from `operator new` (40,976 bytes; `UiMovie::UiMovie` `sub_8211E1C0` when not null, else 0), then `sub_8211A9E8` on it (also on 0) and `UiMovie::Load(movie, id)` (`sub_8211E798`, `ui-movies-game.md`), appended to the list at `0x82815E68` (the count at +64 up by one). From `LoadingScreen::Load` only. 45 lines. Read in full. Native (B7 loop, 2026-10-30): `LoadingScreenAddMovie` in `src/data_load/boot.cpp` (operator new's, `sub_8211A9E8`'s and `UiMovie::Load`'s plain C++; `UiMovie::UiMovie`'s plain C++ with `--native_ui_movies_game` and `--native_c_runtime_heap`, else its adapter), no adapter (only `LoadingScreen::Load`'s plain C++ calls it; the original from its frame where operator new's plain C++ hands back). Harness `data_load`. |
| `sub_82291328` | | Overridden | No arguments; from `BootData::Load` only. `sub_822912A0` with each of the three asset ids at `0x82638134` (22560, 22595, 22559 in the image, `known-data.md`; only this function forms the address, as loads), then nine words at `0x828165D0` = 0, `[0x82816600]` = 0, `[0x82638130]` = 0 (1 in the image); then `UiMovie::SetRenderer` (`sub_8259D798`, `ui-movies.md`) with the app's interface `[0x826BABA0]`'s slot 8 (`sub_820944D8`) answer's slot 2 (the renderer) and slot 1 (the command list): four indirect calls, as `sub_8211AD00` makes. The movies' users (`sub_82290B98`, `sub_82290D48`, `sub_82290D90`, `sub_82290EA0`, `sub_82290F78`, `sub_82290FE0`, `sub_822910F0`) never ran in play, so what the set is isn't known. 71 lines. Read in full. Native (B7 loop, 2026-10-30): `OtherMoviesLoad` in `src/data_load/boot.cpp`, the adapter behind `--native_data_load`, `--native_heap` and the archive's queue native; the four indirect calls guest calls with the original's return addresses (as `sub_8211AD00`'s override), `UiMovie::SetRenderer`'s plain C++; r3 the renderer. Harness `data_load`. |
| `sub_822912A0` | | Native | r3 = an asset id: as `LoadingScreen::AddMovie`, appended to the list at `0x82816610` (the count at `0x82816650`). From `sub_82291328` only. 45 lines. Read in full. Native (B7 loop, 2026-10-30): `OtherMoviesAdd` in `src/data_load/boot.cpp` (as `LoadingScreen::AddMovie`), no adapter (only `sub_82291328`'s plain C++ calls it). Harness `data_load`. |
| `sub_821066D0` | `BootData::Unload` | | The load's mirror: when the flag `0x826C5B8C` is set (it isn't cleared), `Params::Free` (`sub_820C9D90`, `car-specs.md`), `sub_82291408`, `LoadingScreen::Free`, `sub_82105688` (`save-data.md`), `Text::Unload`, then `sub_82106540` (an empty function, `audio.md`). From `sub_82106718` (with `sub_82106678`, the app's shut-down; from `sub_820941F0`), none of which ran. Not seen in play. Read in full. |
| `sub_8210AE38` | `Text::Unload` | | `Text::Load`'s mirror (`strings-and-font.md`): `sub_82109A40` (the font's teardown, `strings-and-font.md`) and `sub_8210A4D8` (not read; probably the string table's). From `BootData::Unload` only. Not seen in play. Read in full. |
| `sub_82247B28` | `LoadingScreen::Free` (probably) | | While the list at `0x82815E68` has movies: the count down by one, and the last movie (0 ends the loop early) destroyed (`UiMovie::~UiMovie` `sub_8211E160`, `ui-movies-game.md`) and deleted (`operator delete` `sub_825A70E8`). From `BootData::Unload` only. Not seen in play. Read in full. |
| `sub_82291408` | | | The same for the list at `0x82816610`. From `BootData::Unload` only. Not seen in play. Read in full. |
| `sub_825DB170` | | | `Replay826D8520::StaticInit`'s clean-up (registered with `atexit`): `sub_82104630` (not read; probably the records' destructor) on `0x826D8520` + 0x2B610. Not seen in play (the exit never runs). Read in full. |
| `sub_825DB1B0` | | | The same for `0x82816E50` (`Replay82816E50::StaticInit`'s). Not seen in play. Read in full. |
