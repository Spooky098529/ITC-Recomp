# Known functions: the replay's snapshots

The race's snapshots for its replay (`82102AA8`-`82105230`): the state
of the seven vehicles and of the racers' manager (`[0x826D82F4]`,
`race-world.md`) saved into blocks of the input recordings
(`[0x826C5B4C]`, `../known-data.md`) and put back later, so that a
replay of the recorded input (`racer-controllers.md`) starts from
exactly the race's start. A block ("race snapshot", 38,928 bytes) is the
manager's 80 entries and counts (2,292 bytes), a word, then one
5,232-byte record a vehicle (`Record5232`, constructed by
`Record5232::Record5232` in `data-load.md`): +0 and +4 the vehicle's
+6148 (the car's flags; 0 for a vehicle not in use) and +6152 (its
contact bits), +16 its status (+6160, `VehicleStatus`), +128 and +224
its five track positions (+5232, +5328; `TrackPosition::Copy`), +608
its rigid body's first 516 bytes (`[+336]`), +1136 its physics part
(vehicle +0 to +3241, packed into 2,944 bytes, `Member1136`) and +4080
its collision object (`[+340]`, `Member4080`: whether there was one,
the two placed copies of its shape at +16 and +480 (`Part464`, 452
bytes of each), the shape record's box and motion, the push and torque
fields). Each part has a save (vehicle to record), a restore (record to
vehicle) and a copy (record to record, returning r3).

When: at the countdown (`RaceStates::OnCountdown`, `race-states.md`)
`RaceReplay::SaveStart` clears both recordings' counts and both blocks,
saves the race into block A (recordings + 0x20F590) and at once puts the
cars back from it (probably so the race itself starts from what the
snapshot holds, field for field). At the race's end
(`RaceStates::OnRaceEnd`) `Replay::Keep` copies block A, player 0's
recording and the race's set-up into the kept replay (`[0x826C5B50]`).
Before the replay (`RaceStates::ReplaySetUpTick`) `RaceReplay::SaveEnd`
saves the race as it ended into block B (recordings + 0x218DA0) and
`RaceReplay::RestoreStart` puts the cars back from block A; after it
(`RaceStates::AfterReplayTick`) `RaceReplay::RestoreEnd` puts them back
from block B, the players' cars where they were unless in contact, the
others placed on the route at their speed (`Vehicle::PlaceOnRoute`).
Each restore: `RaceManager::ResetWorld`, the manager's entries copied
back, `RaceManager::ResetVehicles`, then for each record in use
`Record5232::Restore`, `Vehicle::LinkTrackPositions` and
`CollisionObject::ResetTransform`, then `RaceManager::ListRacers`. Each
runs once at its event (not every tick).

Read by the B7 loop's mapping agent (2026-11-04) from two leads in
`../loops/leads.md` (the race data's leftovers `sub_82102AA8` and
`sub_82102B98`; the cars put back on the road, `82104310`-`82104E00`)
and, as the same blocks, the lead on the input recordings saved and
"rewound" (`sub_82104F68`, `sub_82104E68`, with `sub_82104EA8`'s
subtree): `sub_82104E68` saves the race's end, it doesn't rewind the
recordings (`RaceReplay::RestoreStart` and `SaveStart` zero their
counts). No subagents. Every function in full (the long copies by their
field maps: `Member1136::Restore` and `Save` mirror each other, checked
by a script over their 101 direct loads and stores). The callers are in
`race-states.md` and (not seen in play) `save-data.md`'s
`sub_82105688`. Shared, with rows elsewhere: `Vec4::Copy`,
`Vehicle::Get`, `Vehicle::Identity`, `Stages::Copy`,
`RacerRecords::At` (`vehicle-accessors.md`); `Vehicle::GetSpeedKmh`
(`functions.md`); `Matrix::CopyRows`, `Wheel::Copy`,
`WheelRecord2::Copy`, `CollisionObject::ResetTransform`
(`car-physics.md`); `TrackPosition::Copy`,
`Vehicle::LinkTrackPositions`, `Vehicle::PlaceOnRoute` (`vehicle.md`);
`RaceManager::ResetWorld`, `ResetVehicles`, `ListRacers`
(`race-world.md`); `memcpy`, the array destructor iterator
(`c-runtime.md`); `operator delete` (`heap.md`); the empty
`sub_82106540` (`audio.md`); the records' constructors (`data-load.md`).
Nothing in `itc_recomp/src` hooks these functions (checked 2026-11-04).
The kept replay's other readers (`sub_821A67E8` in the save screens'
lead; `sub_8221D678`, `sub_82287220`, `sub_8228AAB0`, `sub_82293028`
not seen in play) aren't mapped. Constant: `[0x82000A34]` 0.0. The word
at recordings + 0x2225B0 (0 at `InputRecordings::Create`) is set to 1
by both restores; its readers weren't found by its offset.

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


| `sub_82104D40` | `RaceReplay::SaveStart` | | No arguments. Both recordings' counts (recordings + 0x107AC0 and + 0x20F584) = 0; the seven records' words +0 of block A (recordings + 0x20FE90 + 5,232 i) and block B (+ 0x2196A0 + 5,232 i) = 0; `RaceSnapshot::Save`(recordings + 0x20F590); `RaceManager::ResetWorld`(`[0x826D82F4]`); `RaceManager::ResetVehicles`(manager, 0); for each of the seven records of block A whose word +0 isn't 0: vehicle i (`Vehicle::Get`), `Record5232::Restore`(the record, the vehicle), `Vehicle::LinkTrackPositions`, `CollisionObject::ResetTransform`(vehicle +340); `RaceManager::ListRacers`. The cars saved and at once put back from the snapshot. Only caller `RaceStates::OnCountdown` (`race-states.md`, without the replay flag 0x40), once a race. Read in full. |
| `sub_82104E68` | `RaceReplay::SaveEnd` | | No arguments. Block B's seven records' words +0 (recordings + 0x2196A0 + 5,232 i) = 0, then tail-calls `RaceSnapshot::Save`(recordings + 0x218DA0): the race as it ended, for `RaceReplay::RestoreEnd`. Only caller `RaceStates::ReplaySetUpTick` (unless kind 5), before `RaceReplay::RestoreStart`. Read in full. Called "the input recordings rewound" in the lead (`race-states.md`'s row corrected). |
| `sub_821046F8` | `RaceReplay::RestoreStart` | | No arguments. Both recordings' counts = 0 (the replay reads them from the start); `RaceManager::ResetWorld`(`[0x826D82F4]`); the manager's entries put back from block A's head (recordings + 0x20F590): 77 entries of 28 bytes to manager +4, three to +2160 (from +2156), seven words to +2336 (from +2240), +2364, +2368, +2372, +2376, +2380, +2388 (from +2268 to +2288; `memcpy` for the entries); `RaceManager::ResetVehicles`(manager, 1); for each record of block A in use (word +0 not 0): `Record5232::Restore`, `Vehicle::LinkTrackPositions`, `CollisionObject::ResetTransform` (as `SaveStart`); `RaceManager::ListRacers`; recordings + 0x2225B0 = 1. From `RaceStates::ReplaySetUpTick` (after `SaveEnd`) and `sub_82168858` (kind 5's replay, not seen in play). Read in full. Blocked only by `Record5232::Restore`'s group. |
| `sub_82104898` | `RaceReplay::RestoreEnd` | | No arguments. As `RestoreStart` (the counts zeroed, the manager put back, `ResetVehicles`(manager, 1)) with block B (recordings + 0x218DA0, records from + 0x2196A0), and after each `Record5232::Restore`: unless the vehicle's flags +6148 have 0x81 (the players' cars, probably) and its contact bits +6152 are 0, `Vehicle::PlaceOnRoute`(the vehicle, `Vehicle::Identity` + 4 (the car's route position), f1 = `Vehicle::GetSpeedKmh`, f2 = 0.0 (`[0x82000A34]`), r6 = 1 when the float +6224 is above 0.0 (else 0), r7 = 1); then `LinkTrackPositions` and `ResetTransform`; `ListRacers`; recordings + 0x2225B0 = 1. Only caller `RaceStates::AfterReplayTick` (its step 2). What +6224 is isn't known. Read in full. Blocked only by `Record5232::Restore`'s group. |
| `sub_82104AB8` | `RaceSnapshot::Save` | Overridden | r3 = a block (38,928 bytes): +2292 = 0; for i = 0 to 6, vehicle i (`Vehicle::Get`): when its +6148 isn't 0, `Record5232::Save`(block + 2304 + 5,232 i, the vehicle) (records of vehicles not in use are left as they were); then the manager's (`[0x826D82F4]`) entries and counts copied into the block's head (the reverse of `RestoreStart`'s copy: +4 → +0 77 × 28 bytes, +2160 → +2156, +2336 → +2240, +2364 to +2388 → +2268 to +2288). From `RaceReplay::SaveStart` and `SaveEnd`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::RaceSnapshotSave` in `src/replay_snapshots/records.cpp` (`Vehicle::Get`'s plain C++, memcpy through `native::Buffers_Memcpy`; r3 the last memcpy's, block + 2212), adapter in `src/hooks/replay_snapshots.cpp` (the original when memcpy's plain C++ is off). Harness `replay_snapshots` (24 cases: the game's vehicles and manager pointers at blocks of random words with NaNs planted, all, none or some vehicles in use, some without an object; the snapshot, the vehicles, their parts, the manager and r3 compared exactly). Not seen in an attract race (its callers didn't run in one, 2026-11-04). |
| `sub_82104EA8` | `RaceSnapshot::Copy` | Native | r3 = out, r4 = a block: `memcpy` of the head (2,292 bytes), +2292, then `Record5232::Copy` on the seven records (+2304 + 5,232 i); returns r3. Only caller `Replay::Keep`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::RaceSnapshotCopy` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits; r3 = out), no adapter: only `Replay::Keep`'s plain C++ calls it; harness `replay_snapshots` (12 cases through `Replay::Keep`). |
| `sub_82104630` | `RaceSnapshot::Destroy` | | r3 = a block: for its seven records from the last, the array destructor iterator (`sub_820900C0`) over the four 96-byte track positions at record +224 with the empty `sub_82106540`: nothing is done. From `InputRecordings::~InputRecordings` (both blocks), `Replay::Destroy` and the static replays' clean-ups (`sub_825DB170`, `sub_825DB1B0`, `data-load.md`). Not seen in play. Read in full. |
| `sub_82104538` | `Record5232::Save` | Native | r3 = a record, r4 = a vehicle: `Record5232::SaveTrack`(record, vehicle); `BodyState::Save`(record + 608, vehicle +336); `Member1136::Save`(record + 1136, vehicle); `Member4080::Save`(record + 4080, vehicle +340). Only caller `RaceSnapshot::Save`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Record5232Save` in `src/replay_snapshots/records.cpp` (r3 `Member4080::Save`'s), no adapter: only `RaceSnapshot::Save`'s plain C++ calls it; harness `replay_snapshots` (24 cases through `RaceSnapshot::Save`). |
| `sub_821044D8` | `Record5232::Restore` | Overridden | r3 = a record, r4 = a vehicle: `Record5232::RestoreTrack`; `BodyState::Restore`(record + 608, vehicle +336); `Member1136::Restore`(record + 1136, vehicle); `Member4080::Restore`(record + 4080, vehicle +340). From `RaceReplay::SaveStart`, `RestoreStart`, `RestoreEnd`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Record5232Restore` in `src/replay_snapshots/records.cpp` (r3 as `Member4080::Restore` leaves it), adapter in `src/hooks/replay_snapshots.cpp` (the original when memcpy's plain C++ is off, `native::Buffers_NativeMemcpy`). Harness `replay_snapshots` (40 cases: random words with NaNs and infinities planted, the collision flag 0, 1 or any, no object, both shape copies one shape, the first copy over the object's +40; vehicle, body, object, copies, record and r3 compared exactly). Not seen in an attract race (none of the three callers ran in one: a temporary counter on them, 2026-11-04). |
| `sub_82104BC0` | `Record5232::Copy` | Native | r3 = out, r4 = a record: `CopyTrack`, `BodyState::Copy` (+608), `Member1136::Copy` (+1136), `Member4080::Copy` (+4080); returns r3. Only caller `RaceSnapshot::Copy`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Record5232Copy` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits; r3 = out), no adapter: only `RaceSnapshot::Copy`'s plain C++ calls it; harness `replay_snapshots` (12 cases through `Replay::Keep`). |
| `sub_82104380` | `Record5232::SaveTrack` | Native | r3 = a record, r4 = a vehicle: record +0 = vehicle +6148 (the car's flags), +4 = +6152 (the contact bits); `memcpy`(record + 16, vehicle + 6160, 112) (the vehicle status and 4 bytes after it); `TrackPosition::Copy`(record + 128, vehicle + 5232) and the four wheels' (record + 224 + 96 i, vehicle + 5328 + 96 i). Only caller `Record5232::Save`. Read in full. Named `TrackPosition::Copy`'s callers in `vehicle.md`. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Record5232SaveTrack` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits), no adapter: only `Record5232::Save`'s plain C++ calls it; harness `replay_snapshots` (24 cases through `RaceSnapshot::Save`). |
| `sub_82104310` | `Record5232::RestoreTrack` | Native | r3 = a record, r4 = a vehicle: the reverse of `SaveTrack` (vehicle +6148, +6152, the 112 bytes at +6160, the five track positions from the record). Only caller `Record5232::Restore`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Record5232RestoreTrack` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits), no adapter: only `Record5232::Restore`'s plain C++ calls it; harness `replay_snapshots` (40 cases through `Record5232::Restore`). |
| `sub_821043F0` | `Record5232::CopyTrack` | Native | r3 = out, r4 = a record: words +0, +4, `memcpy` of +16 (112 bytes), `TrackPosition::Copy` of +128 and the four at +224; returns r3. Only caller `Record5232::Copy`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Record5232CopyTrack` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits; r3 = out), no adapter: only `Record5232::Copy`'s plain C++ calls it; harness `replay_snapshots` (12 cases through `Replay::Keep`). |
| `sub_821039F0` | `BodyState::Save` | Native | r3 = a record's +608, r4 = a rigid body (vehicle +336): copies the body's first 516 bytes at the same offsets: byte +0, words +4, +8, +48, +112, +160, +164, +384, +512, the doubleword pair +80, vectors (`Vec4::Copy`) +16, +32, +64, +96, +128, +144, +176, +192, +208, +352, +368, +400, +480, +496, matrices (`Matrix::CopyRows`) +224, +288, +416. Only caller `Record5232::Save`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::BodyStateSave` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits), no adapter: only `Record5232::Save`'s plain C++ calls it; harness `replay_snapshots` (24 cases through `RaceSnapshot::Save`). |
| `sub_821031A0` | `BodyState::Restore` | Native | r3 = a record's +608, r4 = a rigid body: the same copy the other way (record to body). Only caller `Record5232::Restore`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::BodyStateRestore` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits), no adapter: only `Record5232::Restore`'s plain C++ calls it; harness `replay_snapshots` (40 cases through `Record5232::Restore`). |
| `sub_82103B50` | `BodyState::Copy` | Native | r3 = out, r4 = a record's +608: the same 516-byte copy; returns r3. Only caller `Record5232::Copy`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::BodyStateCopy` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits; r3 = out), no adapter: only `Record5232::Copy`'s plain C++ calls it; harness `replay_snapshots` (12 cases through `Replay::Keep`). |
| `sub_821034B8` | `Member1136::Save` | Native | r3 = a record's +1136, r4 = a vehicle: the vehicle's physics part (+0 to +3241) packed into the record's 2,944 bytes, field by field: the same offsets to +271, then shifted (the vehicle's byte +276 to +272, +280 to +332 and +348 to +276 to +332, +352 to +400 vectors to +336 to +384, the four wheels +416 + 368 i to +400 + 368 i by `Wheel::Copy`, the axles +1888 and +2016 to +1872 and +2000 by `WheelRecord2::Copy`, `memcpy` of 144 bytes +2204 to +2156, 60 bytes twice +2988 to +2684, and words, bytes and vectors between, to the vehicle's +3240 and +3241 at +2936 and +2937). Only caller `Record5232::Save`. Read in full (the field map against `Restore` checked by a script). Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Member1136Save` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits), no adapter: only `Record5232::Save`'s plain C++ calls it; harness `replay_snapshots` (24 cases through `RaceSnapshot::Save`). |
| `sub_82102C60` | `Member1136::Restore` | Native | r3 = a record's +1136, r4 = a vehicle: the reverse of `Member1136::Save` (record to vehicle; its 101 direct loads and stores the mirror of `Save`'s, and the same calls). Only caller `Record5232::Restore`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Member1136Restore` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits), no adapter: only `Record5232::Restore`'s plain C++ calls it; harness `replay_snapshots` (40 cases through `Record5232::Restore`). |
| `sub_82103CB8` | `Member1136::Copy` | Native | r3 = out, r4 = a record's +1136: the 2,944-byte part copied field by field at the same offsets (`Vec4::Copy`, `Matrix::CopyRows`, `Wheel::Copy`, `WheelRecord2::Copy`, `memcpy`); returns r3. Only caller `Record5232::Copy`. Read in outline (its calls and loops; the same shape as `Save`). Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Member1136Copy` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits; r3 = out), no adapter: only `Record5232::Copy`'s plain C++ calls it; harness `replay_snapshots` (12 cases through `Replay::Keep`). |
| `sub_821033F0` | `Member4080::Save` | Native | r3 = a record's +4080, r4 = a collision object (vehicle +340): with r4 0, +0 = 0 and nothing else; else +0 = 1, +944 and +960 = the object's +48 and +64 (its shape record's box, min and max), +976 = its +80 (the motion matrix, `Matrix::CopyRows`), +1040 = +144, `Part464::Save`(+16, the object's +36) and (+480, its +40) (the shape record's previous and current placed copies), +1056 = +160, +1072 = +176, +1088 = +192, +1104 = +208, +1120 = +224, +1124 = +288, +1136 = +304. Only caller `Record5232::Save`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Member4080Save` in `src/replay_snapshots/records.cpp` (the object's +40, then +36, read after the first stores, as the original; r3 the part with no object, else the part + 1136), no adapter: only `Record5232::Save`'s plain C++ calls it; harness `replay_snapshots` (24 cases through `RaceSnapshot::Save`). |
| `sub_82102B98` | `Member4080::Restore` | Native | r3 = a record's +4080, r4 = a collision object: nothing when the record's +0 is 0 or r4 is 0; else the reverse of `Member4080::Save`, and also the object's +32 (the shape record's template) = its own +20 (its shape). Only caller `Record5232::Restore`. Read in full. Named in `car-physics.md`'s `Matrix::CopyRows` row. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Member4080Restore` in `src/replay_snapshots/records.cpp` (the object's +20 and +40 read before its first stores, +36 after, as the original; r3 the record part when nothing is done, else the object + 304), no adapter: only `Record5232::Restore`'s plain C++ calls it; harness `replay_snapshots` (40 cases through `Record5232::Restore`). |
| `sub_82104598` | `Member4080::Copy` | Native | r3 = out, r4 = a record's +4080: word +0, `Member4080::CopyShapes`(+16), then +1056, +1072, +1088, +1104, +1120, +1124, +1136; returns r3. Only caller `Record5232::Copy`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Member4080Copy` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits; r3 = out), no adapter: only `Record5232::Copy`'s plain C++ calls it; harness `replay_snapshots` (12 cases through `Replay::Keep`). |
| `sub_82104460` | `Member4080::CopyShapes` | Native | r3 = out, r4 = a `Member4080`'s +16: `Part464::Copy` at +0 and +464, `Vec4::Copy` +928 and +944, `Matrix::CopyRows` +960, word +1024; returns r3. Only caller `Member4080::Copy`. Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Member4080CopyShapes` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits; r3 = out), no adapter: only `Member4080::Copy`'s plain C++ calls it; harness `replay_snapshots` (12 cases through `Replay::Keep`). |
| `sub_82103300` | `Part464::Save` | Native | r3 = a record's part (`Member4080` +16 or +480), r4 = a placed copy of a collision shape (the box, at least 496 bytes): part +352 to +368 = shape +16 to +32 (five floats), +384 = +48, +400, +416, +432 = +64, +80, +96, +448 = the halfword +112 (as a word), +0 = +128, +16 = +144, +32 to +144 = +176 to +288 (eight vectors), and six (float, vector) pairs +160 + 32 k = +304 + 32 k. Only caller `Member4080::Save` (twice). Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Part464Save` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits), no adapter: only `Member4080::Save`'s plain C++ calls it; harness `replay_snapshots` (24 cases through `RaceSnapshot::Save`). |
| `sub_82102AA8` | `Part464::Restore` | Native | r3 = a part, r4 = a placed shape copy: the reverse of `Part464::Save` (the halfword +112 stored from the part's word +448). Only caller `Member4080::Restore` (twice). Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Part464Restore` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits), no adapter: only `Record5232::Restore`'s plain C++ calls it; harness `replay_snapshots` (40 cases through `Record5232::Restore`). |
| `sub_821041C0` | `Part464::Copy` | Native | r3 = out, r4 = a part: the 452 bytes copied at the same offsets (`Vec4::Copy`, floats +352 to +368, word +448); returns r3. Only caller `Member4080::CopyShapes` (twice). Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::Part464Copy` in `src/replay_snapshots/records.cpp` (the original's stores and calls in its order; the lfs and stfs pairs as the words' bits; r3 = out), no adapter: only `Member4080::CopyShapes`'s plain C++ calls it; harness `replay_snapshots` (12 cases through `Replay::Keep`). |
| `sub_82104F68` | `Replay::Keep` | Overridden | r3 = the kept replay (`[0x826C5B50]`), r4 = the race data, r5 = the recordings, r6 = player 0's save data or 0, r7 = 0 (a stage flag). `memcpy`(replay + 52, race data + 4, 32) (the route and the words to +35); +84, +88, +92, +96 = the race data's +0x59610 (the car count), +0x59614, +0x59618, +0x5961C; +0x2B5E8 = +0xA3AE0; for the 93 racer records (race data + 48 + 3,936 k) a 1,908-byte entry at +100 + 1,908 k: +0 the flags (+3888), +4 (+3892), +8 the record's first 1,884 bytes (`memcpy`, its source through the empty `sub_82106540`), +1892 and +1894 the start position halfwords (+3904, +3906), +1896 (+3908), +1900 the racer id (+3912), +1904 (+3920); the stage (`Stages::Copy` of the race data's stage list + 0x59620, the last one, or `[+804]` − 1 with r7 set) to +0x2B588 (80 bytes); the last kept result (`RacerRecords::At`(`[0x826C5B2C]` + 0x59AE8, −1)) +3916 to +3928 to +0x2B5D8 to +0x2B5E4; five words from +0xA3AE4 to +0x2B5EC; `RaceSnapshot::Copy`(replay + 0x2B610, block A at recordings + 0x20F590); `memcpy`(replay + 0x34E20, the recordings, 0x107AC4) (player 0's recording and its count); +0, +4, +8 = the race data's +4, +28, +32 (from the replay's copy); +12 + 4 i = entry +20 (the record's word +12) of the four racers the stage lists (its +44 + 8 i); +28 = the stage's word +0; +32 to +51 the save data's five words from +8, or without one bytes and words 0, 6, 0, 1, 6, 0, 1; +0x2B600 = 0 (1 from `Replay::Create`). Only caller `RaceStates::OnRaceEnd`, once a race (r6 the save data for kind 1). Read in full. Native under `--native_replay_snapshots` (B7 loop, 2026-11-04): `replay_snapshots::ReplayKeep` in `src/replay_snapshots/records.cpp` (`Stages::Copy`'s and `RacerRecords::At`'s plain C++, Stages::Copy's original where it refuses, run from its frame; the empty `sub_82106540` left out; the stage copy in the 272-byte frame's +80 as the original; r3 the recording's memcpy's, replay + 0x34E20), adapter in `src/hooks/replay_snapshots.cpp` (the original when memcpy's plain C++ is off). Harness `replay_snapshots` (12 cases: the race data, the recordings and the replay of random words with NaNs planted, the game's race data pointer at the race data, the stage the last or the current - 1 with its racers 0 to 92 or -1, with and without the save data; the replay, race data, recordings, the frame's stage copy and r3 compared exactly). Not seen in an attract race (`RaceStates::OnRaceEnd` didn't call it in one, 2026-11-04). |
| `sub_82104CE0` | `InputRecordings::~InputRecordings` (deleting) | | r3 = the recordings, r4 = flags: `RaceSnapshot::Destroy` on block B (+0x218DA0) and block A (+0x20F590); with r4 bit 0 `operator delete`(r3); returns r3. Only caller `InputRecordings::Destroy`. Not seen in play. Read in full. |
| `sub_82104F00` | `InputRecordings::Destroy` | | No arguments: when `[0x826C5B4C]` isn't 0, `InputRecordings::~InputRecordings`(it, 1) (tail call; the global not cleared). Only caller `sub_82105688` (a teardown, `save-data.md`). Not seen in play. Read in full. |
| `sub_82104F20` | `Replay::Destroy` | | No arguments: when `[0x826C5B50]` isn't 0, `RaceSnapshot::Destroy`(it + 0x2B610) and `operator delete`(it) (the global not cleared). Only caller `sub_82105688`. Not seen in play. Read in full. |
