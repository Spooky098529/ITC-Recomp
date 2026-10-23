# Known functions: the data tables

The game's data tables at `[0x826C5CA0]` (a 64-byte object, beside the
sound manager's code at `82117348` but not part of it): twelve assets
from the archive, loaded once at boot into one block and read by index
all through the game. `DataTables::Init` (`sub_82116DB0`) makes the
object and lays the block out from the assets' sizes in the archive's
TOC; `DataTables::Load` (`sub_82117240`) asks for the twelve loads
(`Archive::Load`); each load's callback byte-swaps its table's entries
in place (the files are little-endian; asset 94's and 84's aren't
swapped) and counts the pending loads down. Both run once, from the
game's data load at boot (`sub_82105868`, from `sub_82106618`; a lead).
The tables (what's known of them, `../known-data.md`):

- asset 94 at +16: 400 entries of 3,008 bytes (the count, +8, is set to
  400 by its callback, not read from the file), indexed by the same ids
  as the save data's 12-byte records at +0x199F8 (`SaveData::Of`, in
  `vehicle-accessors.md`): probably the game's items (cars or parts);
  `DataTables::Item` (`sub_82117178`) has 62 callers.
- asset 84 at +60: no reader found besides the load.
- eight assets at +20 to +48 (their ids the eight words at
  `0x82616C64`, not read: the image isn't dumped): 56-byte entries
  ending with one whose +0 is -1; `DataTables::Entry56` (`sub_82117190`)
  with the table (0-7) and the entry.
- asset 95 at +52: 20-byte entries ending with -1 at +0, searched by
  their +8 and +0 (`sub_821171B0`), read by index (`sub_82117210`).
- asset 96 at +56: 40-byte entries ending with -1 at +4 (the callback;
  `sub_82116588` stops at -1 at +16), read by index (`sub_82117228`).

Not here: `Archive::Load` (`sub_8209DE48`, `files-and-saves.md`,
shared); the archive's TOC base by id (`sub_8209D868`, below 23011
`[0x826A5120]`, else `[0x826A5128]`) and an asset's size
(`sub_820A0678`), both under the asset size's lead; operator new[]
(`sub_825A6940`, `heap.md`). The callers of the accessors are many and
in other subsystems (named in the rows; leads for the save data's items
at `82110B40`-`82116588`, the code at `82140340`-`82144E58` and the
front end at `822271F8`-`8222F0B0`). The table's free (`sub_82116FB0`:
the block +12 and the object, `sub_825A71C0`, `sub_825A70E8`) never ran
and has no row. The rows were drafted by the B7 loop's mapping agent
(2026-10-23) from the generated code; since then the B7 loop's migration agent has made them native as far as they're ready (`src/data_tables/`, `--native_data_tables`).

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
| `sub_82116DB0` | `DataTables::Init` | | No arguments; once at boot (only caller `sub_82105868`, just before `DataTables::Load`). Makes the 64-byte table object (`sub_825A6880`, operator new) into `0x826C5CA0`: +12 the block, +4 the loads pending, +8 the item count, all 0. Then two passes over the twelve assets: each sets a table pointer to the block (+12) plus the sizes so far (0 while the block isn't made) and counts +4 up: +16 asset 94, +60 asset 84, +20 to +48 the eight ids at `0x82616C64`, +52 asset 95, +56 asset 96. The sizes come from the archive's TOC (`sub_8209D868(id)` + 16 × id + 24, the unpacked size, or + 20, the stored size, when 0; `sub_820A0678(id)` for the eight). After the first pass the block is allocated with the total (`sub_825A6940`, operator new[]); +4 ends at 12. Allocates guest memory. |
| `sub_82117240` | `DataTables::Load` | | No arguments; once at boot (only caller `sub_82105868`, after `DataTables::Init`). Twelve `Archive::Load`s (`sub_8209DE48`, `files-and-saves.md`; r7, r8 = 0) into the table pointers `DataTables::Init` set: asset 94 into +16 with `sub_821170D8` and the table object as context; 84 into +60 with `sub_821170C0` (the object); the eight ids at `0x82616C64` into +20 to +48 with `sub_821170F0` (the table as context); 95 into +52 with `sub_82116FF0` and 96 into +56 with `sub_82117058` (the table). Each returned serial is stored at the object's +0 (overwritten; nothing reads it). |
| `sub_821170D8` | `DataTables::OnItemsLoaded` | Overridden | `Archive::Load`'s callback for asset 94 (no direct callers; its address is passed by `DataTables::Load`): r6 = the table object: +8 = 400 (the item count), +4 (the loads pending) down by one. The entries aren't byte-swapped. Leaf. Native (2026-10-23, `OnItemsLoaded` in `src/data_tables/data_tables.cpp`, `--native_data_tables`; harness `data_tables`); runs at boot. Its adapter stays (a function pointer). |
| `sub_821170C0` | `DataTables::OnLoaded84` | Overridden | `Archive::Load`'s callback for asset 84 (no direct callers): `[0x826C5CA0]` +4 down by one. Leaf. Native (2026-10-23, `OnLoaded84` in `src/data_tables/data_tables.cpp`, `--native_data_tables`; harness `data_tables`); runs at boot. Its adapter stays (a function pointer). |
| `sub_821170F0` | `DataTables::On56Loaded` | Overridden | `Archive::Load`'s callback for the eight 56-byte tables (no direct callers): r6 = the table: `Entry56::Swap` (`sub_82111B30`) on each entry, up to and including the first whose +0 is -1, then `[0x826C5CA0]` +4 down by one. Native (2026-10-23, `On56Loaded` in `src/data_tables/data_tables.cpp`, `--native_data_tables`; harness `data_tables`, tables of 0 to 39 entries and up to the scratch area's end); runs at boot. Its adapter stays (reached through `Archive::Load`'s function pointer). |
| `sub_82116FF0` | `DataTables::On20Loaded` | Overridden | `Archive::Load`'s callback for asset 95 (no direct callers): r6 = the table: `Entry20::Swap` (`sub_82111AE0`) on each 20-byte entry, up to and including the first whose +0 is -1, then `[0x826C5CA0]` +4 down by one. Native (2026-10-23, `On20Loaded` in `src/data_tables/data_tables.cpp`, `--native_data_tables`; harness `data_tables`, tables of 0 to 39 entries and up to the scratch area's end); runs at boot. Its adapter stays (reached through `Archive::Load`'s function pointer). |
| `sub_82117058` | `DataTables::On40Loaded` | Overridden | `Archive::Load`'s callback for asset 96 (no direct callers): r6 = the table: `Entry40::Swap` (`sub_821128E0`) on each 40-byte entry, up to and including the first whose +4 is -1, then `[0x826C5CA0]` +4 down by one. Native (2026-10-23, `On40Loaded` in `src/data_tables/data_tables.cpp`, `--native_data_tables`; harness `data_tables`, tables of 0 to 39 entries and up to the scratch area's end); runs at boot. Its adapter stays (reached through `Archive::Load`'s function pointer). |
| `sub_82111AE0` | `Entry20::Swap` | Overridden | r3 = a 20-byte entry: `SwapWord` (`sub_820C97D8`) on its five words. Only caller `sub_82116FF0`. Native (2026-10-23, `SwapEntry20` in `src/data_tables/data_tables.cpp`, `--native_data_tables`; harness `data_tables`): calls the native `SwapWord`; r3 left at the last word swapped, as the original's. |
| `sub_82111B30` | `Entry56::Swap` | Overridden | r3 = a 56-byte entry: `SwapWord` on its 14 words (+0 to +20, three pairs at +24 to +44, +48, +52). Only caller `sub_821170F0`. Native (2026-10-23, `SwapEntry56` in `src/data_tables/data_tables.cpp`, `--native_data_tables`; harness `data_tables`): calls the native `SwapWord`; r3 left at the last word swapped, as the original's. |
| `sub_821128E0` | `Entry40::Swap` | Overridden | r3 = a 40-byte entry: `SwapWord` on its ten words (+0 to +12, three pairs at +16 to +36). Only caller `sub_82117058`. Native (2026-10-23, `SwapEntry40` in `src/data_tables/data_tables.cpp`, `--native_data_tables`; harness `data_tables`): calls the native `SwapWord`; r3 left at the last word swapped, as the original's. |
| `sub_820C97D8` | `SwapWord` | Overridden | r3 = a word in guest memory: reverses its four bytes in place, through a float load and store (`lfs`, `stfs` on the stack, flush mode off). Leaf. Callers: the three `Swap`s above only. Native (2026-10-23, `SwapWord` in `src/data_tables/data_tables.cpp`, `--native_data_tables`): the translated code's steps (the single widened to double and narrowed back, on the default floating-point model `generated/` is built with, then the bytes stored reversed). In the release build the compiler folds the translated code's round trip (its disassembly stores the word as loaded: a plain byte swap, signaling NaNs unchanged), and folds ours the same way; harness `data_tables`, 600 cases (256 signaling-NaN patterns over their payloads and both signs, then random words and float edge values), compared exactly. f0 (volatile, read by no caller) isn't set; the adapter turns flush-to-zero off as the original does. |
| `sub_82117158` | `DataTables::IsLoading` | | 1 while loads are pending (`[0x826C5CA0]` +4 nonzero), else 0. Leaf. From `sub_82144E58` and `sub_821514A0` (both skip their work while it's 1). |
| `sub_82117178` | `DataTables::Item` | | r3 = an item id (0-399): `[[0x826C5CA0] + 16] + 3,008 × id`. No bounds check. Leaf. 62 callers (43 seen), among them `sub_82111780`, `sub_82112950`, `sub_821129E0`, `sub_82115190`, `sub_82112DB0`, `sub_82112E2C`, `sub_82115D98`, `sub_82140340` to `sub_82144A50`, `sub_82156CC0`, `sub_82158F70`, `sub_82164B50`, `sub_821654F8`, `sub_82165E20`, `sub_8216C820`, `sub_82170FF8`, `sub_82173050`, `sub_821737F8`, `sub_82173E28`, `sub_821764F0`, `sub_822123C8`, `sub_82213638`, `sub_82213F20`, `sub_822271F8` to `sub_8222F0B0`; and `sub_82116DA8`. What they read: bytes +34, +35, +36, +46, +91, +6 and halfwords +42, +88 (`../known-data.md`). |
| `sub_82116DA8` | `ItemRecords::Item` (probably) | | r3 not read, r4 = an item id: tail call to `DataTables::Item(r4)`. r3 is the save data's item records (`SaveData::Of(i)` + 0x199F8) where seen, so probably a method on them. Callers: `sub_820F2D28` and `sub_82114B88` seen (`sub_82114B88`: the record's halfword +2 up by one, at most 999), `sub_820F2CA8`, `sub_821116E0`, `sub_82114A88`, `sub_82114B08` not. |
| `sub_82117190` | `DataTables::Entry56` | | r3 = a table (0-7), r4 = an entry: `[[0x826C5CA0] + 20 + 4 × r3] + 56 × r4`. No bounds check. Leaf. 17 callers (9 seen): `sub_82114268`, `sub_82116588` (walks table 0 up to the -1 entry), `sub_8218DBD0`, `sub_8218DCC0`, `sub_82227618`, `sub_8222A978`, `sub_8222AE00`, `sub_8222B048`, `sub_8222C300`. They read +0, +8, +16. |
| `sub_821171B0` | `DataTables::Find20` | | r3, r4: the index of the first 20-byte entry of `[[0x826C5CA0] + 52]` whose +8 is r3 and +0 is r4, or -1 (the search stops at the entry whose +0 is -1). Leaf. 9 callers (6 seen), the front end's at `822271F8`-`8222C300` (`sub_822271F8`, `sub_82227430`, `sub_8222A470`, `sub_8222A718`, `sub_8222B8A8`, `sub_8222C300`); most add the index to `[[x + 20048] + 872]`. |
| `sub_82117210` | `DataTables::Entry20` | | r3 = an index: `[[0x826C5CA0] + 52] + 20 × r3`. Leaf. The same callers as `DataTables::Find20`; they read +4. |
| `sub_82117228` | `DataTables::Entry40` | | r3 = an index: `[[0x826C5CA0] + 56] + 40 × r3`. Leaf. Callers `sub_82116588` (seen; walks the table up to the entry whose +16 is -1) and `sub_821162B0` (not seen). |
