# Known functions: the minimap

The race's minimap: drawn by `Minimap::Draw` (`sub_820F5740`) from the
course scenery's draw (`Scenery::Draw`, `sub_820F6068`, in
[`course-scenery.md`](course-scenery.md)) into a camera pass of its own
(the pass description's +20; the minimap's road outline is the 256 × 256
target in `../native-renderer.md`'s list of passes), after that pass's clear (`RecordClear`,
key 19) and `Scenery::RecordHeadlightProjectors`. It draws three
things: the **map**, the first few models of the course set's list at
+1064 (how many depends on the race data's flags), through the lists of
models' draw; the **point markers**, a 22-triangle shape at each point
of the course set's list at +3180 that the race mode shows; and the
**car markers**, a 7-triangle arrow (one triangle in the marker's color,
six black) for the player and each racer on a branch of the route the
player's branch shows, in one of five colors by type (0 the player,
yellow; 1 a human car in mode 6, by `[0x826C7824]`, or a racer
without a rival record, orange-red; 2 to 4 the rivals, by their record in player 0's
save: cyan, pink, green). The car markers are sorted into five stack
pools (80-slot lists of 64-byte matrices) by type plus a phase, then
drawn pool by pool; the phase (`0x826C5AA8`) steps every five calls
(`0x826C5AA4`), so which type is drawn last, on top, changes: the
player's triangle flashing where it overlaps other cars'
(`../frame-interpolation.md`, "Per-frame logic": `Minimap::Draw` is
Wrapped to keep that clock at the tick's rate, `--per_tick_minimap_blink`,
and a migration has to keep the wrap). Each triangle is a 176-byte
primitive (`Triangle::`, the layout in `../known-data.md`) recorded by
`Triangle::RecordFlat` on the command list at `[0x826D83D4]`, the
color rectangles' (`mode-tick.md`), as an inline 3-vertex draw: the
minimap's triangle in `../gpu-functions.md` ("The helper object",
command 15).

Shared, with rows in other files: `Scenery::Draw` (`sub_820F6068`) and
`Scenery::RecordHeadlightProjectors` (`sub_820F34C8`) in
[`course-scenery.md`](course-scenery.md); `Matrix::Copy`,
`Matrix::Multiply`, `Matrix::RotateY` (`sub_820A0AC0`, `sub_8209BD50`,
`sub_820A67F8`) and `sub_8209B038` (the pools' element constructor,
passed by `Minimap::Draw` to the constructor iterator: the pointer that
row says reaches it) in [`matrix-math.md`](matrix-math.md);
`Matrix::MultiplyTranslation` (`sub_820B0D68`) in
[`car-physics.md`](car-physics.md); `Vehicle::Get` (`sub_820C0468`),
`Stages::Copy` (`sub_820FFD88`) and `SaveData::Of` (`sub_82105620`) in
[`vehicle-accessors.md`](vehicle-accessors.md); `ItemRecords::Item`
(`sub_82116DA8`) in [`data-tables.md`](data-tables.md); `memcpy`,
`atan2` and the vector constructor and destructor iterators
(`sub_825A7410`, `sub_825A8770`, `sub_820911A8`, `sub_820900C0`) in
[`c-runtime.md`](c-runtime.md); the empty destructor `sub_82106540` in
[`audio.md`](audio.md); `CommandList::OpenChunk`, `AllocateSlots`,
`AllocateVertices`, `CloseChunk` and `ChunkSlots` (`sub_8258C9D0`,
`sub_8258CAA0`, `sub_8258CB08`, `sub_8258CA50`, `sub_8258CA88`) in
[`command-list.md`](command-list.md); `atexit` (`sub_825A7068`, the
core's, `cameras.md`'s intro), which the three static tables' first use
registers their clean-up with (`0x825DAEF0`, `0x825DAEF8`,
`0x825DAF00`, never run). Without rows anywhere, in
`../loops/leads.md`: the lists of models' draw `sub_82147D88` (with the
recorders under it) and the race mode's test of the +3180 list's entries
`sub_82147B18` (the course set's lists' lead). Not seen in play, so
without rows: `sub_820F2CA8` (a rival's chained record, below
`Minimap::RacerMarkerType`), `sub_820F2E68` (not a function: case 1 of
`Minimap::RacerMarkerType`'s switch, which the recompiler made one; a
migration ports it with that function: ported with it, 2026-10-25) and `sub_8217C4C0` (the
triangle's other kind, as `Triangle::RecordFlat` with four vertices,
probably a quad; not read in detail).

Blocked, for the migration: `Minimap::Draw` by `atexit` (the core) and
the course set's lists' `sub_82147D88` and `sub_82147B18`; the marker
draws by `atexit`. The rest is ready from the bottom up. Made native by the B7 loop's migration, 2026-10-25 (`src/minimap/`, `--native_minimap`, harness `minimap`): the manager's lookups, `SetList::Point`, `Minimap::RacerMarkerType`, `Minimap::Scaling`, `Triangle::Init`, the `List80` functions, `Primitive::RecordTransform` and `Triangle::Record` with `Triangle::RecordFlat`; a frame dump's minimap (the 256 × 256 resolve) the same with the flag off, its car marker included. The fields and
globals are in `../known-data.md` (`0x826C5AA4`-`0x826C5B00`, the
markers' tables `0x826C5520` and `0x826C5680`, "triangle", and additions
to `0x826D82F4` and the course set); the floats read are from the image
(`0x820067BC`-`0x82006808`, `0x82002814`, `0x82002C1C`, `0x82001FB4`
π/2). Mapped by the B7 loop's mapping agent (`../loops/b7-loop.md`),
2026-10-25, from the generated code and the image only (no runs): call
counts aren't known. Names are ours.

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
| `sub_820F5740` | `Minimap::Draw` | Wrapped | From the draw step part `sub_820F6068`; steps the minimap blink clock at `0x826C5AA4`/`0x826C5AA8` after drawing (see "Per-frame logic" in `../frame-interpolation.md`). Wrapped in `src/frame_pacing/per_frame_logic.cpp` (`--per_tick_minimap_blink`, on by default): on render-only passes both words are put back after the call, so the clock steps once a tick; the migration has to keep that. r3 = the course scenery's manager, r4 = a pass description (+20 the camera pass, `0x826BAC00` + 592 × +20; +24 and +28 two keys). Only caller `Scenery::Draw` (`course-scenery.md`), when the pass is on (+588), after `RecordClear` and `Scenery::RecordHeadlightProjectors`. A 28,336-byte frame. Nothing at all (no blink step either) when the word +0 of the course set's (+112) list at +1064 is 0. Otherwise: the keys to `0x8296B90C` and `0x8296B900` (as `Scenery::DrawArea`), the pass's view-projection (+144) copied to the stack and its view (+16) to `0x8265ECA0` (`Matrix::Copy`). **The map**: the first n entries of the +1064 list in list order (n its word +0, or, when the race data `[0x826C5B2C]` is set with +0xA3AF0 = 1, by its +20 flags: 4 with 0x20, else 3 with 0x4 and 0x8, else 2 with 0x10, else 1): each entry's +48 = 0 and its shapes 0 to +412 − 1 drawn by `sub_82147D88` (lead) with the view-projection copy, an identity, (10000.0, 50000.0, 0.0, 0.0) (`0x82002814`, `0x82006804`) and zeros. **The points**: for each entry of the set's list at +3180 (count +620) that `sub_82147B18` (lead) accepts, its points 0 to 3 (`sub_82147D18`); each with y at least -1000.0 (`0x82006808`) gets y = 0.0 and a `Minimap::DrawPointMarker` scaled (100, 1, 100) (`Minimap::Scaling`) and moved there (`Matrix::MultiplyTranslation`), colored (168, 124, 255, 255) for points 0 to 2 and (103, 252, 255, 255) for 3. **The car markers**: the five type colors at `0x826C5AB0` made once (guard bit 0x1 at `0x826C5B00`, clean-up to `atexit`); five stack pools of 5,488 bytes (+0 the type, -1 none; +16 the color; +32 an 80-slot `List80` of 64-byte matrices, its data from +368, made by the vector constructor iterator with `sub_8209B038` and reset inline); with `Vehicle::Get(0)`: the player's branch number (byte +3 of the route section (`[0x826C5B10]` + 12) at halfword +8 of the race manager's first entry with flag 0x4, `Race::FindPlayerEntry(1)`, its result not checked for 0); then for each of the manager's entries (`[0x826D82F4]` + 4 + 28 × i, count +2368) a type: flag 0x4 gives 0, or 1 when the race data's +0xA3AF0 is 6, the current stage (`Stages::Copy(-1)`) has +0 other than 1 and `[0x826C7824]` is 0; flag 0x8 the same but 1 when `[0x826C7824]` is not 0; else flag 0x1 without 0x20 `Minimap::RacerMarkerType` (-1 skips); else skipped. Its pool is (type + `[0x826C5AA8]`) mod 5; the pool's first marker sets its type and copies its color (`0x826C5AB0` + 16 × type). The marker's matrix: for 0x4 and 0x8 entries with a vehicle (`Race::RacerVehicle(i)`), scale (100, 1, 100), `Matrix::RotateY` by π/2 (`0x82001FB4`) and times the vehicle's matrix (+3312, its y translation +52 set to 0.0); else from the route, the entry's slice (halfwords +8 the section, +10 the slice; the slice's center x and z with y 0.0, turned by `atan2` of its forward x and z). Added (`List80::Insert` at the count) while the pool holds under 80. Then each pool in order 0 to 4, each marker (`List80::Copy`) drawn by `Minimap::DrawCarMarker` with the pool's color. Last the blink clock: `0x826C5AA4` + 1, above 4 back to 0 and `0x826C5AA8` + 1, above 5 back to 0; and the pools' destructor iterator (`sub_82106540`, empty). The first part's dead stores (1.0 at the stack's +364 to +476, overwritten) are the compiler's. Read in full 2026-10-25 (the B7 loop). |
| `sub_820F2D28` | `Minimap::RacerMarkerType` | Overridden | r3 = a race manager entry (28 bytes), r4 = the player's branch number: the entry's marker type, or -1 for none. 1 when the entry's word +0 has 0x10 or its racer record (`[0x826C5B2C]` + 48 + 3,936 × the entry's +24) has the racer id (+3912) -1. Otherwise by the id's record in player 0's save data (`SaveData::Of(0)` + 0x199F8 + 12 × id, three halfwords; `ItemRecords::Item` gives the id's item): 2 when +0 + +2 + +4 (as a halfword) is 0 or less, else 3 when +2 is 0, else 4, or, when the item's halfword +88 is 3, `sub_820F2CA8`'s answer unless that's -1 (never ran: from the item's +92, 3 when that id's +2 is 0, else itself again). Then -1 when the entry's section (+8) or slice (+10) is negative; type 1 stays on any branch; the others only on a branch the player's shows: the entry's section's branch number (byte +3, `[0x826C5B10]` + 12) by a switch on the player's (the byte offsets at `0x820067A0`): 1 shows 1 and 5, 2 shows 2 and 6, 3 shows 3 and 6, 4 shows 4 and 5, 5 shows 1, 4 and 5, 6 shows 2, 3 and 6, 7 shows 7 and 14, 8 shows 8 and 13, 13 shows 3, 8 and 13, 14 shows 4, 7 and 14, others only their own. Case 1 is `sub_820F2E68` in the generated code (never seen in play). Only caller `Minimap::Draw`. Read in full. Native since 2026-10-25: `src/minimap/minimap.cpp` behind `--native_minimap` (adapter `src/hooks/minimap.cpp`, harness `minimap`), with case 1 (`sub_820F2E68`) in its switch; an id whose item's +88 is 3 (`sub_820F2CA8`, never seen in play) hands the call to the original (the harness leaves that case out: its chained records follow made-up data). Not seen in the attract race. |
| `sub_820F2EF8` | `Minimap::Scaling` | Overridden | r3 = out, r4 = a vector: out = an identity (made on the stack, `memcpy`) with x, y, z at +0, +20, +40. Leaf apart from `memcpy`. Only caller `Minimap::Draw` (five call sites, always (100.0, 1.0, 100.0): `0x82002A0C`, `0x82000A30`). Read in full. Native since 2026-10-25: `src/minimap/minimap.cpp` behind `--native_minimap` (adapter `src/hooks/minimap.cpp`, harness `minimap`); the identity written straight to out (the original's stack copy and `memcpy` left out), the original for an out in the physical views (`memcpy`'s mirrors); r3 = out, as `memcpy` leaves it. |
| `sub_820F4888` | `Minimap::DrawCarMarker` | | r3 = the camera pass, r4 = a color (four words: red, green, blue, alpha), r5 = the marker's matrix. Seven triangles from the table at `0x826C5520` (336 bytes: 3 vertices of (x, 0.0, z, 1.0) each, made once from the floats at `0x820067BC`-`0x820067D8`, guard bit 0x1 at `0x826C5670`, clean-up `0x825DAEF8` to `atexit`): triangle 0, (1.918, -2.157), (-1.918, -2.157), (0, 4.313) in x and z, an arrow along +z, in the color; 1 to 6 black (0, 0, 0, 255), reaching ±2.66 and 6.07 (probably its outline). Each one a stack triangle (`Triangle::Init`), its vertices and its three colors set, the matrix copied in (`memcpy`, 64 bytes), recorded with `Triangle::Record` (key 104). Only caller `Minimap::Draw`, once a car marker. Read in full. |
| `sub_820F4BA8` | `Minimap::DrawPointMarker` | | As `Minimap::DrawCarMarker` with 22 triangles: the table at `0x826C5680` (1,056 bytes, from the floats at `0x820067DC`-`0x82006800` and `0x82002C1C` (1.65), guard bit 0x1 at `0x826C5AA0`, clean-up `0x825DAF00` to `atexit`); triangles 0 to 5 in the color, 6 to 21 black. Only caller `Minimap::Draw`, once a shown point. Read for its outline (the table's 264 stores not checked one by one). |
| `sub_820F4040` | `Triangle::Init` | Overridden | r3 = a 176-byte triangle (`../known-data.md`): an identity at +0, +64 (the kind) = 0, +68 = 0, +72 = 2, and 1.0 at +92, +108 and +124 (the w of its three vertices). Leaf. As `Sprite::ResetStyle2` (`race-overlays.md`) does for a sprite. From `Minimap::DrawCarMarker` and `Minimap::DrawPointMarker`, once a triangle. Read in full. Native since 2026-10-25: `src/minimap/minimap.cpp` behind `--native_minimap` (adapter `src/hooks/minimap.cpp`, harness `minimap`); the rows stored at their 16-byte boundaries, as the original's `stvx`. |
| `sub_8217DAA0` | `Triangle::Record` | Overridden | r3 = a triangle, r4 = a camera pass, r5 = a sort key: kind +64 0 tail-calls `Triangle::RecordFlat`, 1 `sub_8217C4C0` (never ran), above 1 nothing. As `ColorRect::Record` (`mode-tick.md`). From the two marker draws only. Read in full. Native since 2026-10-25: `src/minimap/minimap.cpp` behind `--native_minimap` (adapter `src/hooks/minimap.cpp`, harness `minimap`): kind 0 to `Triangle::RecordFlat`'s step, kind 1 the original `sub_8217C4C0` (never seen in play), above 1 nothing. |
| `sub_8217C370` | `Triangle::RecordFlat` | Native | r3 = a triangle, r4 = a camera pass, r5 = a sort key: on the command list at `[0x826D83D0 + 4]` (the render queue at +0; the color rectangles', `mode-tick.md`) a chunk: `CommandList::OpenChunk`, a header slot; `Primitive::RecordTransform(pass, triangle)`; [13, 4, 0, 0]; three 24-byte vertices with their header (`AllocateVertices(3, 1)`: command 5): +0 x, +4 y, +8 z from the vertices at +80, +96 and +112, +12 the color as ARGB (the low bytes of the color words +140, +128, +132, +136 for the first, 16 bytes on for the others), +16 to +23 not written; a slot with +0 = 15 (the helper object's draw, `../gpu-functions.md`); `CloseChunk`; the header's halfwords +0 and +2 = 0, +4 and +12 = 0, +8 = `ChunkSlots` − 1; queued through the queue's slot 4 with the header, the key and 0.0 (`0x82000A34`). Only caller `Triangle::Record`. Read in full. Native since 2026-10-25: `src/minimap/minimap.cpp`, called by `Triangle::Record`'s step, no adapter (`--native_minimap` switches its caller; harness `minimap`); `Primitive::RecordTransform`'s plain C++ called with its product at the original's place (r1 − 128 − 80, noted for interpolation) and the command list's plain C++; the render queue's slot 4 (`sub_8258EDC8` in play) a guest call from a 128-byte frame, as `ColorRect::RecordFlat`'s (`mode-tick.md`); the original where a chunk is open on any list, the list hasn't room for its 14 slots, or with the command list's diagnostics on. A frame dump's minimap (its car marker drawn through it) the same with the flag off. |
| `sub_8217AEA8` | `Primitive::RecordTransform` (probably) | Overridden | r3 = a camera pass, r4 = a primitive whose +0 is its world matrix: `Matrix::Multiply(stack, the matrix, the pass's view-projection +144)`, then on the command list at `[0x826D83D4]` a slot of command 7 (+4 = 0, +8 = 1, bytes +12 = 0 and +13 = 4: probably vertex constants c0 to c3) and four slots with the product's rows. The 1.0 stores before the multiply are overwritten. Callers seen: `Triangle::RecordFlat` and `sub_8217B508` (named in `command-list.md`'s intro, from `sub_8217C708`, a lead); also `sub_8217C4C0` (never ran). Read in full. Native since 2026-10-25: `src/minimap/minimap.cpp` behind `--native_minimap` (adapter `src/hooks/minimap.cpp`, harness `minimap`); the product made at the original's frame (r1 − 80) and noted for frame interpolation as the multiply's wrapper does (`BlendState_OnMultiply`, return address `0x8217AEDC`); `AllocateSlots`' plain C++; the original where the list hasn't room for the five slots, or with the command list's diagnostics on (`--log_slot_callers` and the like). r3 = the last slot. The adapter stays for `sub_8217B508`'s calls. |
| `sub_820F5238` | `List80::Insert` | Overridden | r3 = an 80-slot list of 64-byte entries (`List64`'s layout from `track-zones.md` for 80 slots: nodes from +0, the free head +320, the list's head +324, the count +328, the data from +336), r4 = a matrix, r5 = a position: `List80::Link`, then `Matrix::Copy` of r4 into the data slot of the node now at the position (+336 + 64 × its +2). Only caller `Minimap::Draw` (at the count: appended). Read in full. Native since 2026-10-25: `src/minimap/minimap.cpp` behind `--native_minimap` (adapter `src/hooks/minimap.cpp`, harness `minimap`); `Matrix::Copy`'s plain C++ noted for interpolation as its wrapper does (`BlendState_OnCopy`, `0x820F529C`); r3 = the data slot. |
| `sub_820F40C0` | `List80::Link` | Native | `List128::Link` (`track-zones.md`) for 80 slots: r3 = the list, r4 = a position: the free head (+320) taken and linked in at the position (after the head +324 for 0, else after the node at position − 1), count +328 up by 1. No check for a full list (its caller checks for under 80). Leaf. Only caller `List80::Insert`. Read in full. Native since 2026-10-25: `src/minimap/minimap.cpp`, called by `List80::Insert`'s plain C++, no adapter (`--native_minimap` switches its caller; harness `minimap`). |
| `sub_820F51A8` | `List80::Copy` | Overridden | r3 = out, r4 = an 80-slot list, r5 = a position: the 64-byte entry of the node at the position copied to out (`memcpy`; the 1.0 stores to out's +12, +28, +44 and +60 before it are overwritten). Returns out. Only caller `Minimap::Draw`. Read in full. Native since 2026-10-25: `src/minimap/minimap.cpp` behind `--native_minimap` (adapter `src/hooks/minimap.cpp`, harness `minimap`); the copy through `c_runtime::Memcpy`, the original where it declines or for an out in the physical views. |
| `sub_82147D18` | `SetList::Point` (probably) | Overridden | r3 = the course set's list at +3180, r4 = out, r5 = an entry, r6, r7: the x, y and z at `[list + 528 + 4 × r5]` + 64 × r6 + 16 × r7 copied to out with w = 1.0 (`0x82000A30`). Leaf. Only caller `Minimap::Draw` (r6 0 to 3, r7 0). In the range of the course set's lists (`82146AA8`-`82148650`, a lead), here as only the minimap calls it. Read in full. Native since 2026-10-25: `src/minimap/minimap.cpp` behind `--native_minimap` (adapter `src/hooks/minimap.cpp`, harness `minimap`). Not seen in the attract race. |
| `sub_82170578` | `Race::RacerVehicle` | Overridden | r3 = a racer index: 0 when `[0x826D82F4]` (the race's manager) is 0, else `Vehicle::Get` (tail call) of the racer's (`[.. + 2308 + 4 × r3]`) +20. No bounds or null check of the racer, and no flag test (unlike `Racers::Vehicle`, `track-zones.md`). Callers seen: `Minimap::Draw` and `sub_820C2248` (the vehicle code, a lead). Read in full. Native since 2026-10-25: `src/minimap/minimap.cpp` behind `--native_minimap` (adapter `src/hooks/minimap.cpp`, harness `minimap`); `Vehicle::Get`'s plain C++. |
| `sub_82170608` | `Race::FindPlayerEntry` | Overridden | r3 = the race's manager, r4 = which: the first of its 28-byte entries (from +4, count +2368) whose word +0 has 0x4 (r4 not 0) or 0x8 (r4 0); returns its address, or 0. Leaf. Callers seen: `Minimap::Draw` (r4 = 1) and `sub_82164B50` (not read). Read in full. Native since 2026-10-25: `src/minimap/minimap.cpp` behind `--native_minimap` (adapter `src/hooks/minimap.cpp`, harness `minimap`). |
