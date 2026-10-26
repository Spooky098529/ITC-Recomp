# Known functions: the race's set-up

The **race's set-up** (`Race::SetUp`, `sub_82115D98`, once a race
start, from the front end with player 0's save data; it runs in attract
races) and what it builds: the race data at `[0x826C5B2C]` reset
(`RaceData::Reset`), its route (a course route from a table of 23
built on the stack, `CourseRoutes::Get`, and the race's location), the
racers (3,936-byte racer records in the race data's +48: the player's
car copied from the save data's garage, the opponents built from items
of the data tables (`DataTables::Item`, `data-tables.md`: an item's +160
is a car build), 16 more cars (ids 55 to 70, probably the traffic) and
their start positions), and the stage list at the race data's +0x59620
(stages with their racers and routes, by the rival item's race kind:
`Race::SetUpRival`). Below it, the **car object**'s own set-up at
`820CC450`-`820CDCE0` (`Car::Init`, `Car::Clear` and the parts' resets:
the settings, the decals, the tuning; `Car::SetModel` from a car spec,
`Car::FromBuild` from a car build), which every racer record starts
with (a racer record is a car object, `car-specs.md`, followed by the
race's words at +3904 to +3924), with 20 callers seen outside the race
too (the garage, the front end). Also here: the race data's 77 other
records (`sub_82101FD0`; their own layout) and the save data's 408-byte
course records (`CourseRecord::Reset`), whose resets the race data's
reset calls; and four functions of the object at `[0x826C6078]`
(probably the race's event: its opponent list), which the set-up reads.
The fields are in `../known-data.md` ("car object", "the race data",
"stage list", "course route", "route", "course record", "event").

Shared with other files, which keep their rows: `RacerRecord::Init`
(`sub_820FF808`) and the stage list's and racer records' accessors
(`Stages::SetType`, the +808 setter `sub_820FFAB0` and the like,
`Racers::IndexOf`, the race data's word setters `sub_820FF918`,
`sub_820FF938`, `sub_820FF968`, and `sub_820FFEB0`, a route's last
position) in `vehicle-accessors.md`;
`Car::Refresh` (`sub_820C4998`), `Params::GetCarSpec`, `Params::Exists`,
`Params::FirstWheelOfModel` in `car-specs.md`; `Vehicle::SetupFromCar`
(`sub_820C27E8`) in `car-physics.md`; `DataTables::Item` in
`data-tables.md`; `SaveData::Of` in `vehicle-accessors.md`; `rand`
(`sub_82293568`) in `race-overlays.md`; `memset`, `memcpy` and the array
constructor helper `sub_820911A8` in `c-runtime.md`. Not here: the
set-up's callers (the front end, `../loops/leads.md`), the race code
that sets up later stages (`sub_821634D8`, `sub_82164B50`,
`sub_82166658`, in the race states' lead), the save data's own set-up
(`SaveData::Create`'s lead) and a front-end screen's own race set-up
(`sub_8227B930`, a lead). Two functions here are pieces of a switch the
recompiler split off as functions of their own (`sub_82101ADC`,
`sub_82112E2C`): they run in their parent's frame and go with it.

Floats named by address are in `.rdata`: `0x82000A30` is 1.0,
`0x82000A34` 0.0, `0x82002A0C` 100.0, `0x82002568` 20.0 (as
`vehicle-accessors.md` and `track-route.md` give them); the others
(`0x8200222C`, `0x82002204`, `0x8200692C`, `0x82006960` to
`0x82006968`, `0x82002810`) weren't read.

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
| `sub_82115D98` | `Race::SetUp` | | r3 = player 0's save data (`SaveData::Of(0)` at four of its five callers), r4 = a rival's item id or -1. Save +6112 = 48; `RaceData::Reset` (`sub_821021A8`); race data +0xA3AE0 = `rand()` % 10, +0xA3AF0 = 1, +0xA3AF4 = 2; the race data's +32 = save +6104 (the mode, `sub_820FF968`). The player's racer record on its stack: `Car::Init`, then 1,884 bytes of the garage car picked (the save's +0x19920-th node of the list at save +6120: 4-byte nodes {+0 next, +2 car}, the head halfword at +6324; cars of 1,968 bytes at save +6332) copied in, its +3892 = save +0x1F870, `Car::Refresh`, and `Race::AddRacer` (r5 = 0, f1 = the garage car's float +1892, r6 = save byte +8). Race data +0x59618 = 2 for mode 1, 1 for mode 2, else 3 (`sub_820FF918`); +0x5961C = 5 (`sub_820FF938`); `Race::SetRoute(0, save +6108)` (the location). Then: for event 1 (save +6100): `Stages::AddRacer(list, 0, 2, -1)`, a rival made on the stack (`RacerRecord::Init`, `Car::SetModel` car 11, a fixed paint (+0 = -2, +6 = 1, +8 = 234, +10 = 92, +11 = 64), the +1568 bytes 0, 3, 7, 4, 14, 22, 25, 14, 27, 24, 27 from +1569 and +1584 = 0, flag 0x2), `Car::Refresh`, appended to the racer records, `Stages::AddRacer` for it, the stage list's +808 = 1 (`sub_820FFAB0`), race data +0x59AE4 \|= 0x8 and `Race::AddStage(0)`; else with r4 a rival: `Race::SetUpRival(location, r4, 1)`, +808 = 1, +0x59AE4 \|= 0x10; else: `Race::SetRoute(0, location)` again, +0x59AE4 \|= 0x8, and one racer record for each entry of the event's list (`[0x826C6078]` +156, +20 entries of 12 bytes): `RacerRecord::Init`, `Car::FromBuild(record, item +160)`, +3912 = the item's id (halfword +0), +3916 = the entry's index, +3924 = the item's byte +37, flag 0x2 (and 0x300 when the entry's +8 is 0), +3904 = the entry's word +4 (a start position), `Car::Refresh`, appended; then `Race::SetUpRival(location, the first owned item, 0)` (`RaceEvent::CurrentFirstOwned`), or `Race::AddStage(4)` (which adds nothing) when there's none. Then race data +20 = save +0x199E0, +24 = 0 with the bits of two halfwords (a bit number each, below 0 none) from the table at `0x82008114` + 4 × (event % 7); `Race::PlaceRacers` (`sub_82102048`); a count at save +5886 + 2 × (location - 14) for locations 14 to 19, or + 2 × location for 0 to 13, up to 9999 (none above 19); `sub_82110AC0(save + 48, mode)`. 400 lines, an 11,984-byte frame. Callers: `sub_82180CE8`, `sub_82188CE8`, `sub_82188D44`, `sub_822280D8`, `sub_82228138` (front end; the last passes an item's id). Read in detail. |
| `sub_82112DB0` | `Race::SetUpRival` | | r3 = the location, r4 = a rival's item id, r5 = 1 to add its racer records (0 to find those already added, `Racers::IndexOf`, marking them with flag 0x800). Item = `DataTables::Item(r4)`; location 48 is replaced by item byte +36 (0 to 9, a switch: 20 (`sub_82112E2C`), 22, 24, 25, 31, 33, 35, 37, 39, 41). With r5 the rival's record is made (`RacerRecord::Init`, `Car::FromBuild(item +160)`, +3912 = the item id, +3924 = item byte +37, flag 0x2, `Car::Refresh`, appended to the racer records). `Stages::AddRacer(list, 0, 2, -1)` (the player) and for the rival. Then by item halfword +88 (the race kind): 0, the route (`Race::SetRoute(0, location)` with r5; `Stages::SetRoute(list, 0, location, -1)`) and a stage of type 0 (`Stages::SetType(list, 0, -1)`); 1, the route, with r5 up to three more racers from the item's halfwords +92 (-1 ends; flags 0x302), `sub_820FFA88(list, 1, -1)` (never seen in play), a stage of type 0; 2 and 5, up to three more racers from +92 (flags 0x202, or found and marked) each added to the stage, then for 2 the route and a type 0 stage, for 5 as kind 4; 3, the route and a type 0 stage, then a chain of stages: the item's +92 list gives the next stage's racers (the player and up to three, flags 0x302, or found and marked), its last racer's item the next list, until one is empty (0xFFFF), each stage of type 0; 4, race data +0xA3AF4 = 3, the location `sub_82101630(location, item byte +90)` with `Race::SetRoute(it, 48)` (r5) and `Stages::SetRoute(list, it, 48, -1)`, a stage of type 1. Returns 1. 499 lines with `sub_82112E2C`. Callers: `Race::SetUp`, `sub_821634D8`, `sub_82164B50`, `sub_82166658` (the race states). Read in detail. |
| `sub_82112E2C` | | | Not a function of its own: the case of `Race::SetUpRival`'s switch on item byte +36 for location 20 (r26 = 20), with a copy of the rest of `Race::SetUpRival` after it, in its frame; the recompiler made it a function (its only caller the switch). Goes with `Race::SetUpRival`. |
| `sub_82101630` | | | r3 = a location, r4 = a byte: r4 + 11, 13, 15, 17, 19 or 21 for locations 14 to 19, else 8. Leaf. From `Race::SetUpRival` (kinds 4 and 5, with the item's byte +90). Read in full. |
| `sub_82102048` | `Race::PlaceRacers` | | With race data +0xA3AF4 0 or 4: copies 408 bytes into the stage list's +812 from the save data's course record (`sub_82105BF8(SaveData::Of(0) + 0x1D52C, {race data +4, race data +0x59618 == 0})`, not seen in play: the set-up sets +0xA3AF4 to 2 or 3); then `Racers::SetStart(race data +48, race data +8, the current stage's type)`. Callers seen: `Race::SetUp`, `sub_8227B930`. Read in full. |
| `sub_82101D50` | `Racers::SetStart` | | r3 = the racer records (race data +48), r4 = the location, r5 = the stage's type. Position = `Race::StartPosition(r4)`. When the race data's +0x59618 isn't 0, 16 cars are added (probably the traffic): for ids 55 to 70, a record on the stack (`RacerRecord::Init`, `Car::SetModel`, flag 0x4, `Car::Refresh`, +3908 = `[0x8200692C]`, +3904 and +3906 = 0) copied to the end (the count at +0x595E0). Then each record with flag 0x1 or 0x80 (the players) gets +3904 = the position (section, slice) and +3908 = 0.0 for stage types 2 and 3, else `[0x8200692C]`; with flag 0x2 only +3908. Returns 1. Only caller `Race::PlaceRacers`. Read in full. |
| `sub_82101A98` | `Race::StartPosition` | | r3 = a location (1 to 46, a switch through the byte table at `0x82006930`): a route position (section in the upper halfword, slice in the lower): for most, the first position of a course route (`CourseRoutes::Get`, entries 0 to 22, and 11 to 21 again); for others fixed ones: (132, 0), (292, 0), (229, 0), (128, 0) to (137, 0), (42, 30), (68, 50), (40, 40), (40, 400), (0, 10), (0, 0); outside 1 to 46, (135, 0). A 5,856-byte frame (a 184-byte copy per case). Only caller `Racers::SetStart`. Read in detail. |
| `sub_82101ADC` | | | Not a function of its own: a case of `Race::StartPosition`'s switch (course route 11), in its frame, returning through its epilogue (`0x82101D0C`); the recompiler made it a function. Goes with `Race::StartPosition`. |
| `sub_821017C0` | `Race::AddRacer` | | r3 = a racer record, r5, f1, r6: with the race data's +0x59614 (racers added this way) 0 the record's flags +3888 \|= 0x1, with 1 \|= 0x80 (the two players, probably); r5 nonzero \|= 0x400; the count up; then `Racers::Add(race data +48, r3, f1, r6)` (tail call). Callers seen: `Race::SetUp`, `sub_82179A60`, `sub_8227B930`. Read in full. |
| `sub_820FF860` | `Racers::Add` | | r3 = the racer records, r4 = a car object, f1, r6: when r4's car id (+12) is 0 to 71, the record at the count (+0x595E0): `RacerRecord::Init`, 3,904 bytes from r4 (`memcpy`), +3920 = f1, +3924 = r6, the count up; returns its index, else -1. Only caller `Race::AddRacer`. Read in full. |
| `sub_82101828` | `Race::AddStage` | | r3 = a type: unless 4, the race data's stage list gets a stage of that type (`[count] +0 = r3`, the count +800 up). Leaf. Callers seen: `Race::SetUp`, `sub_8227B930`. Read in full. |
| `sub_820FFAB8` | `Stages::AddRacer` | | r3 = a stage list, r4 = a racer index, r5 = a word, r6 = a stage or -1 (-1 is the stage at the count +800, the one being built, not the current): the stage's slot at its +76 gets +44 + 8 × slot = r4 and +48 + 8 × slot = r5 (2 at every call seen), and +76 up. Leaf. Callers seen: `Race::SetUp`, `Race::SetUpRival`, `sub_821634D8`, `sub_8227B930`. Read in full. |
| `sub_82101D38` | `Race::SetRoute` | | r3, r4: `Route::Set(race data +4, r3, r4)` (tail call). Callers seen: `Race::SetUp`, `Race::SetUpRival`, `sub_8227B930`. Read in full. |
| `sub_82101E90` | `Stages::SetRoute` | | r3 = a stage list, r4, r5, r6 = a stage or -1 (the current, +804): `Route::Set(the stage +24, r4, r5)` (tail call). Callers seen: `Race::SetUpRival`, `sub_8227B930`. Read in full. |
| `sub_82101A10` | `Route::Set` | | r3 = a 16-byte route, r4 = a course route's index, r5 = a location: +0 = r4, +4 = r5 (r4 + 20 when r5 is 48); `CourseRoutes::Get(r4)` into its frame, copied (`memcpy`, 184 bytes); +8 = its first position (word +0), +12 = its last (`sub_820FFEB0`). Callers: `Race::SetRoute`, `Stages::SetRoute`. Read in full. |
| `sub_82101990` | `Route::Reset` | | r3 = a 16-byte route: +0 = 0, +4 = 20, +8 and +12 the first and last positions of course route 0 (as `Route::Set`). Callers: `RaceData::Reset`, `sub_82101F68`. Read in full. |
| `sub_820FFF50` | `CourseRoutes::Get` | | r3 = out (184 bytes), r4 = an index (0 to 22, not checked): writes a table of 23 184-byte entries into its 4,480-byte frame (each: +0 five route positions, section and slice halfwords; +20 40 words, numbers then -1; +180 a float, from `0x82006960` to `0x82006968` and `0x82002810`), copies entry r4 to out (`memcpy`); returns r3. Entry 0 starts (248, 0), (258, 79), (269, 132), (278, 171), (247, 0). Probably the courses' routes. 1,503 lines (the stores). Callers: `Route::Set`, `Route::Reset`, `Race::StartPosition` and its case `sub_82101ADC`, `sub_82162330`, `sub_821635D0`, `sub_82164090` (race code). Read in detail (the entries' values not listed). |
| `sub_821021A8` | `RaceData::ResetGlobal` | | `RaceData::Reset([0x826C5B2C])` (tail call). Callers seen: `sub_82105868` (the data load at boot), `sub_82114268`, `Race::SetUp`, `sub_8227B930`. Read in full. |
| `sub_821020F0` | `RaceData::Reset` | | r3 not read (the race data from `[0x826C5B2C]`): `Route::Reset(+4)`, +20, +24, +28, +32 = 0; `Racers::Reset(+48)`; `Stages::Reset(+0x59620)`; `sub_82101FD0(+0x59AE8)`; +0xA3AD0 = 1, +0xA3AD4 to +0xA3AE0 = 0, +0xA3AE4 to +0xA3AEC = 0, +0xA3AF0 = 7, +0xA3AF4 = 2. Only caller `RaceData::ResetGlobal`. Read in full. |
| `sub_821021B8` | `RaceData::RaceData` | | r3 = the race data: the 77 records' four 972-byte entries each (from +0x59AEC, 3,936 apart) constructed by the array constructor helper (`sub_820911A8`, 972 bytes, 4, `sub_82101D20`); returns r3. Only caller `sub_82102208` (the data load). Read in full. |
| `sub_82101D20` | | | r3 = a 972-byte entry: +792 = 0.0, +796 = 0. Leaf. No direct callers: the element constructor `RaceData::RaceData` passes to the array constructor helper (`c-runtime.md`; its native code calls this one through its pointer, `src/hooks/c_runtime.cpp`). Read in full. |
| `sub_820FF978` | `Racers::Reset` | | r3 = the racer records (race data +48): +0x595E0 to +0x595EC = 0 (the count, the players' count, and the race data's +0x59618 and +0x5961C), then `RacerRecord::Init` on all 93. Only caller `RaceData::Reset`. Read in full. |
| `sub_82101860` | `Stages::Reset` | | r3 = a stage list (1,224 bytes): cleared (`memset`), then ten stages of type 4 with +4, +8, +76 = 0 and their four slots -1 (racer) and 2 (word); +800, +804, +808, +1220 = 0; `CourseRecord::Reset(+812)`. Callers: `Stages::Clear`, `RaceData::Reset`. Read in full. |
| `sub_82101900` | `Stages::Clear` | | r3 = a stage list: `Stages::Reset` keeping its word +1220. 10 callers (2 seen: `sub_821634D8`, `sub_82166658`, the race states). Read in full. |
| `sub_82105C90` | `CourseRecord::Reset` | | r3 = a 408-byte course record: `sub_820FF558(+332)`; halfwords +0 to +39 ten pairs (i + 1, i), then +36 = -1, +40 = 0, +42, +44, +46 = -1, +48 = 0. Callers: `Stages::Reset` (the stage list's +812), `sub_82105D08` (the save data's 22 at +0x1D52C, `SaveData::Create`'s lead). Read in full. |
| `sub_820FF558` | | | r3 = a 76-byte record: cleared (`memset` 64 bytes, then +60 to +72 = 0). Callers: `sub_82101EB0` (ten in a 972-byte entry), `CourseRecord::Reset`. Probably a lap's or a time's record. Read in full. |
| `sub_82101FD0` | | | r3 = the race data's 77 records (+0x59AE8): cleared (`memset` 0x49FE8 bytes), the count +0x49FE0 and +0x49FE4 = 0, then `sub_82101F68` on each (3,936 bytes apart). Only caller `RaceData::Reset`. The array `RacerRecords::Add` and `RacerRecords::At` work on (`vehicle-accessors.md`). Read in full. |
| `sub_82101F68` | | | r3 = one of the 77 records: +3912, +0, +3924, +3920, +3932 = 0, +3908 = 1; its four 972-byte entries at +4 reset (`sub_82101EB0`) with their +784 and +788 = 12000; `Route::Reset(+3892)`. Callers: `sub_82101FD0`, `sub_821648E0`. Read in full. |
| `sub_82101EB0` | | | r3 = a 972-byte entry: +0, +4, +8 = -1, +12, +16 = 0; ten 76-byte records from +20 (`sub_820FF558`); +780, +784 = 0; +792 a running mean set to (0.0, 1) (`RunningMean::Add` of 0.0 once on its stack); +800, +816 to +828 = 0.0; +804, +808, +812, +832, +968 = 0, halfword +964 = 0. Only caller `sub_82101F68`. Read in full. |
| `sub_82101698` | `RunningMean::Add` | | r3 = {+0 a float mean, +4 a count}, f1 = a value, r5 = a weight: the count += r5; unless 0, mean = (mean × old count + f1 × r5) / new count. Leaf. Callers: `sub_82101EB0`, `sub_82113898`, `sub_82115190`, `sub_82163C50`. Read in full. |
| `sub_82101700` | `Time::Pack` | | r3 = milliseconds (signed): the sign in bit 31, minutes in bits 16-30, seconds in 10-15, milliseconds in 0-9. Leaf. 3 callers (1 seen: `sub_82163140`). Read in full. |
| `sub_82110AC0` | | | r3 = save data +48, r4 = the mode: halfword +5796 + 14 × r4 up (to 9999); when +6008 isn't r4, +6008 = r4 and halfword +6012 = 1, else +6012 up (to 9999). Probably counts of races in each mode and the streak in one. Leaf. Only caller `Race::SetUp`. Read in full. |
| `sub_821430A0` | `RaceEvent::FirstOwned` | | r3 = the event (`[0x826C6078]`): the id (halfword +0) of the first item in its list (+156, +20 entries of 12 bytes: +0 an item id, +8 nonzero) whose byte +91 is 1 and whose save data record (`SaveData::Of(0)` + 0x199F8 + 12 × id: halfwords +0, +2, +4 summed) is above 0; else -1. Callers: `RaceEvent::CurrentFirstOwned`, `sub_82143740`, `sub_82144300`, `sub_82144A50` (the items' lead). Read in full. |
| `sub_821434C8` | `RaceEvent::CurrentFirstOwned` | | `RaceEvent::FirstOwned([0x826C6078])`, or -1 when it's 0. Only caller `Race::SetUp`. Read in full. |
| `sub_821434E8` | `RaceEvent::Current` | | Returns `[0x826C6078]`. Leaf. 21 callers (18 seen: `Race::SetUp`, `AiPath::NoteFixedSections`, the race states and front-end menus). Read in full. |
| `sub_821434F8` | `RaceEvent::Count` | | Returns `[0x826C6078]`'s halfword +20 (its list's count). Leaf. Only caller `Race::SetUp`. Read in full. |
| `sub_820CD5D8` | `Car::Init` | Overridden | r3 = a car object: `Car::Clear`, then +3888 = 0 (the flags), +3892 = 1. 49 callers (20 seen), among them `RacerRecord::Init`, `Race::SetUp` and the front end (`sub_82179A60`, `sub_8217DAC0`, `sub_82182C90`, `sub_82182D30`, `sub_8218DB60`, `sub_821A1380`, `sub_821DFA78`, `sub_821E1B48`, `sub_822042D8`, `sub_82228588`, `sub_8222C300`, `sub_82233B70`, `sub_82234F08`, `sub_822440D8`, `sub_82245200`, `sub_8224A490`, `sub_8224C008`, `sub_8227B930`). Read in full.  Native in `race_setup::CarInit` (`src/race_setup/car.cpp`), adapter in `src/hooks/race_setup.cpp` (`--native_race_setup`), harness `race_setup`. Returns Car::Clear's r3. |
| `sub_820CD7F0` | `Car::Clear` | Overridden | r3 = a car object: +12 = 0; `CarTuning::Reset(+1648, 0)`; halfwords +0, +2, +6, +8 = 0, +4 = -1, bytes +10, +11 = 0; `Car::ResetSettings(+16)`; `Car::ResetDecals40(+288)`; `Car::ResetDecals6(+96)`; `sub_820CCC70(+1568)`; three 20-byte entries from +1588: +0, +4 = 0, +8 to +16 = 0.0. Callers seen: `Car::Init`, `Car::FromBuild`, `sub_82110448`. Read in full.  Native in `race_setup::CarClear` (`src/race_setup/car.cpp`), adapter in `src/hooks/race_setup.cpp` (`--native_race_setup`), harness `race_setup`. Calls the parts' plain C++. Returns r3 = the car + 1568 (the original leaves `sub_820CCC70`'s argument there). |
| `sub_820CD760` | `Car::ResetSettings` | Overridden | r3 = a car object +16: nine halfwords from +0 = 0; bytes +56 to +59 = 2, +60 = 3, +61 = 7, +62 to +76 = 0, +77 = 10, +78 = 0 (the car's setting bytes +72 to +94). Leaf. Only caller `Car::Clear`. Read in full.  Native in `race_setup::ResetSettings` (`src/race_setup/car.cpp`), adapter in `src/hooks/race_setup.cpp` (`--native_race_setup`), harness `race_setup`. |
| `sub_820CCBF0` | `Car::ResetDecals6` | Overridden | r3 = a car object +96: six 32-byte decal entries (as `Car::ResetDecals40`). Leaf. Only caller `Car::Clear`. Read in full.  Native in `race_setup::ResetDecals6` (`src/race_setup/car.cpp`), adapter in `src/hooks/race_setup.cpp` (`--native_race_setup`), harness `race_setup`. |
| `sub_820CC4A0` | `Car::ResetDecals40` | Overridden | r3 = a car object +288: 40 32-byte decal entries (8 groups of 5): +0, +4 = 1.0, +8, +12, +16 = 0.0, halfword +20 and bytes +22, +23 = 0, +24 = -1, +28 = 0. Probably the stickers (and `Car::ResetDecals6`'s the vinyls). Leaf. Only caller `Car::Clear`. Read in full.  Native in `race_setup::ResetDecals40` (`src/race_setup/car.cpp`), adapter in `src/hooks/race_setup.cpp` (`--native_race_setup`), harness `race_setup`. |
| `sub_820CCC70` | | Overridden | r3 = a car object +1568 (20 bytes): bytes +0, +1, +3, +4, +12 = 0, +2 = 5, +5 = 1, +6 = 2, +7 = 1, +8 to +11 = 2, word +16 = 0. What it is isn't known (`Car::SetModel` sets +1 and +5 to +8 at random). Leaf. Only caller `Car::Clear`. Read in full.  Native in `race_setup::Reset1568` (`src/race_setup/car.cpp`), adapter in `src/hooks/race_setup.cpp` (`--native_race_setup`), harness `race_setup`. |
| `sub_820CD618` | `CarTuning::Reset` | Overridden | r3 = a car object +1648 (the tuning, 240 bytes), r4 = a flag: +0 = 0, +4 = (r4 != 0), +12 = (+4 == 1), +8 and +16 to +164 = 0, +168 = 3, +172 = 0, five 12-byte entries from +176 = (1, 0, 0). Leaf. Callers: `Car::Clear` (0), `Car::SetModel` (the spec's byte +709). Read in full.  Native in `race_setup::TuningReset` (`src/race_setup/car.cpp`), adapter in `src/hooks/race_setup.cpp` (`--native_race_setup`), harness `race_setup`. The seven zero words it passes through the red zone below the stack pointer aren't written (nothing reads them; as the post effects' scratch). |
| `sub_820CD8A8` | `Car::SetModel` | | r3 = a car object, r4 = a car id: +12 = r4; unless `Params::Exists` is 0, from its spec (`Params::GetCarSpec`): `CarTuning::Reset(+1648, spec byte +709 != 0)`; +36 = the final drive (block 1 +372), +40 to +68 eight gear ratios (+388); setting bytes +76, +77 = block 1 +632, +636, +74, +75 = block 2 +160, +168, +72 = block 2 +156 then +164 (+72 written twice, +73 left as it was); with the drive type (block 1 +356) neither 0 nor 1, +92 = 1 and +93 = (1.0 - block 1 +364) × 100.0 × 20.0 × `[0x8200222C]` (probably the rear's torque share in percent), else +92 = 0; the stock parts from block 2 into the tuning words (+1676, +1680, +1684, +1700, +1704, +1712 to +1724, +1796; the three +1588 entries from block 2 +172, their +8 to +16 converted to floats; the five +1824 entries from block 2 +260: +8 = the word, +0 = the next / 3 + 1, +4 = it % 3); +1570, +1571 = 5, 0 when block 2 +8 (the displacement) ≤ 2000, +76 ≤ 4700, +80 ≤ 2000 and +84 ≤ 1700 (unsigned), else 3, 3; +1572 = 0; at random (`rand`): +1569 = 0 to 13, +1573 = 1 to 10, +1574 and +1575 = 1 to 10 (each after a 10, else 1 to 9), +1576 = 1 to 9; +1580 = 1 for cars 56, 64 to 68 and 70. 9 callers seen: `Car::FromBuild`, `Racers::SetStart`, `Race::SetUp`, `sub_821104D8`, `sub_82182C90`, `sub_82182D30`, `sub_82233B70`, `sub_8224A490`, `sub_8224C008`. Read in detail. |
| `sub_820CDCE0` | `Car::FromBuild` | | r3 = a car object, r4 = a car build (an item's +160, at least 2,812 bytes): `Car::Clear`; `Car::SetModel(build +2788)`; the paint: halfwords +0, +2 = build +2792, +6 = +2796, +8 = +2800, bytes +10, +11 = +2804, +2808; the decals (`CarDecal::FromBuild`): six at +96 from 36-byte entries at +392 and +500, 40 at +288 from +608, +968, +1328, +1688, +1868, +2048, +2408, +2588 (five each); the final drive and the first six gear ratios (+36 to +60) each × (build +4i × `[0x82002204]` + 1.0) unless the build's float is 0.0; setting bytes +72 to +94 from build words +28 to +116; +1568 (20 bytes) from +2768; the three +1588 entries from +332 (+8 to +16 converted to floats); the tuning words +1656 to +1812 from +120 to +268; the wheel numbers +1728, +1736 = `Params::FirstWheelOfModel(build +232 / 10)` + build +232 % 10 (and +240). Callers seen: `Race::SetUp`, `Race::SetUpRival` (and its case), `sub_821A1380`, `sub_82228588`, `sub_8222C300`. Read in detail. |
| `sub_820CDC18` | `CarDecal::FromBuild` | Overridden | r3 = a 32-byte decal entry, r4 = a 36-byte build entry: +20 = (r4 +20 as a halfword) << 16 \| r4 byte +24 << 8 \| r4 byte +28 (`sub_820CC470`); +8, +12 = r4 +8, +12; +0 = -(r4 +0) and +28 = 1 when r4 +0 is below 0.0 (`sub_820CC450`, never seen in play, and `sub_820CC478`), else +28 = 0 (+0 not set); +4 = r4 +4; +16 = r4 +16 (`sub_820B9660`, `vehicle-accessors.md`); +24 = r4 +32 (`sub_820E70B8`, `scene-models.md`). So +0 a scale, mirrored when negative. Only caller `Car::FromBuild`. Read in full.  Native in `race_setup::DecalFromBuild` (`src/race_setup/car.cpp`), adapter in `src/hooks/race_setup.cpp` (`--native_race_setup`), harness `race_setup`. Its setters `sub_820CC458` to `sub_820CC478` are merged into it, and so is `sub_820CC450` (never seen in play: ported, the build's +0 below 0.0); `sub_820B9660` and `sub_820E70B8` through their plain C++ (`vehicle_accessors::Object344SetFloat16`, `scene_models::SetCarModel`). The halfword it builds in its frame (+80) isn't written. |
| `sub_820CC458` | | Merged | r3 = a decal entry, f1: +4 = f1. Leaf. Only caller `CarDecal::FromBuild`. Read in full.  Merged into `CarDecal::FromBuild`'s plain C++ (`race_setup::DecalFromBuild`); harness `race_setup` (CarDecal::FromBuild's cases). |
| `sub_820CC460` | | Merged | As `sub_820CC458`, +8. Leaf. Only caller `CarDecal::FromBuild`. Read in full.  Merged into `CarDecal::FromBuild`'s plain C++ (`race_setup::DecalFromBuild`); harness `race_setup` (CarDecal::FromBuild's cases). |
| `sub_820CC468` | | Merged | As `sub_820CC458`, +12. Leaf. Only caller `CarDecal::FromBuild`. Read in full.  Merged into `CarDecal::FromBuild`'s plain C++ (`race_setup::DecalFromBuild`); harness `race_setup` (CarDecal::FromBuild's cases). |
| `sub_820CC470` | | Merged | r3 = a decal entry, r4: +20 = r4. Leaf. Only caller `CarDecal::FromBuild`. Read in full.  Merged into `CarDecal::FromBuild`'s plain C++ (`race_setup::DecalFromBuild`); harness `race_setup` (CarDecal::FromBuild's cases). |
| `sub_820CC478` | | Merged | r3 = a decal entry, r4: +28 = r4. Leaf. Only caller `CarDecal::FromBuild`. Read in full.  Merged into `CarDecal::FromBuild`'s plain C++ (`race_setup::DecalFromBuild`); harness `race_setup` (CarDecal::FromBuild's cases). |
| `sub_820CD5C8` | `Car::SetupVehicle` | | r3 = a car object, r4 = a car data: `Vehicle::SetupFromCar(r4, r3)` (`car-physics.md`; tail call, the arguments swapped). 4 callers (3 seen: `sub_82178B98`, `sub_82179A60`, `sub_82216B08`). Read in full. |
