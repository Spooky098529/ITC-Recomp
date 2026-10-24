# Known functions: the track zones

The track zones: 70 stretches of the course's route
(`track-route.md`) that the player's car is checked against every
update, for the HUD. The zones' owner (`[0x828161EC]`, 1,199,156 bytes,
`TrackZones::`) is made by the HUD's set-up (`sub_822122B0`) and freed by
its teardown (`sub_822141A0`); the HUD's update (`sub_82214718`) runs
`TrackZones::Update`, unless the race data's word +4 is 8, 9 or 10, then
asks each zone whether the car is inside (`TrackZones::IsInside`): zones
0-31 from `sub_82212F00`, which picks entries of a 12-byte table at
`0x8200D838` + 24 by zone and sets a UI movie's nodes (`UiMovie::FindNode`
on the HUD's +608, nodes 22, 24, 28 and 30; probably the name of the
expressway route being entered), zones 32-69 from `sub_82213510`
(probably a sign's texture, `UiMovie::SwapTexture` texture 56); both, and
the HUD, are in `../loops/leads.md`. Each zone (17,116 bytes, `Zone::`, a
vtable of three slots: its own set-up, then `Zone::Update` and
`Zone::IsInside`, shared by all 70) holds up to 16 route ranges and is
inside while the car's track position is in any of them. A route range
(1,064 bytes, `RouteRange::`) is a start and an end (section, slice) on
the route: a position is in it when it's on the path that follows the
sections' forward links from the start's section to the end's, between
the start's slice and the end's (and on the sections beside those, links
6 to 9, mapped across by `RouteRange::ToNeighbor`), taking at a fork the
link whose section is in the range's list of sections to take
(`RouteRange::AddBranch`); a second list of sections to leave out is
never filled in play. Each zone's ranges are hardcoded in its set-up
(the rows list them; "taking" names the sections added to take at
forks). The lead took this for the AI's path search; it isn't the AI's.
The fixed-size lists the zones are kept in (16, 64 and 128 slots) are
one layout, made per size by the compiler (`List64::`, `List16::`,
`List128::`): nodes of two halfwords (the next node, -1 for none, and a
data slot, slot i for node i at reset), then halfwords the free list's
head, one not read, the list's head, one not read and the count, then
the data. Read by the B7 loop's mapping agent, 2026-10-24, from its
lead (found by the collision world's mapping agent). The names are ours;
what the zones are for is guessed from the HUD's use (rows say so).
Fields: `../known-data.md` (`0x828161EC`, "track zone", "route range",
`0x82603274`, `0x82011AF0`, `0x8200D838`).

Shared with other files, without rows here: `Vehicle::Get`
(`sub_820C0468`), `Vehicle::Identity` (`sub_820C0168`, the car's track
position) and `Stages::Copy` (`sub_820FFD88`) (`vehicle-accessors.md`);
`memcpy` (`sub_825A7410`, `c-runtime.md`); operator new and delete
(`sub_825A6880`, `sub_825A70E8`, `heap.md`). The route's sections table
(`[0x826C5B10]` + 12, `track-route.md`) is read directly. `List64::Insert`
(`sub_820FE330`) is also called by `sub_82148428` (a list of models, a
lead), and `List16::Link` (`sub_821531C0`) by `sub_82154378`,
`sub_821543E8` and `sub_82154458` (the camera modes' area, in a lead).

Not seen in play, so without rows: `sub_820FE830` (a range's test with
its state forced to 1, returning the slices counted along the path in
`0x82603274` when the position is on it, else -1; no callers seen),
`sub_820FE598` (not read), and the camera modes' users of route ranges
`sub_82155288` (44 ranges made), `sub_821562F8` and `sub_82156980`
(`RouteRange::Update`, `Test`).

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
| `sub_8228E788` | `TrackZones::Create` | | No arguments. The owner made (operator new, 0x124C34 bytes; `TrackZones::TrackZones`, or 0 when new returns 0, not checked after) and stored at `0x828161EC`. The car's player: 1 when the race data's (`[0x826C5B2C]`) word +0xA3AF0 is 6, its +0x59614 is 2, the current stage (`Stages::Copy` with -1, the race data + 0x59620) has a word +0 other than 1 and `[0x826C7824]` is 0; else 0 (probably the second player's screen in a two-player mode; not known). The vehicle `Racers::Vehicle(player)`. The 128-slot list reset (nodes chained, free head +512 and count +520 = 0, head +516 and +508, +514, +518 = -1), the 70 zones (+1036 + 17,116 × i) added at its end (`List128::Insert`), then each zone's slot 0 called with r4 = &(i, the vehicle) in list order. Once per HUD set-up: only caller `sub_822122B0` (the HUD's, a lead). |
| `sub_8228B608` | `TrackZones::TrackZones` | | r3 = the owner: zone i's vtable (+1036 + 17,116 × i) = `0x82011AF0` + 12 × i, for the 70 zones; nothing else. Returns r3. Only caller `TrackZones::Create`. 364 lines of address arithmetic. |
| `sub_8228B500` | `TrackZones::Destroy` | | No arguments: `[0x828161EC]` deleted (operator delete) when not 0, and cleared. Only caller `sub_822141A0` (the HUD's teardown, a lead). |
| `sub_8228BD30` | `TrackZones::Update` | | r3 = the owner: each zone in its list (count +520), slot 1 (+4) of its vtable (`Zone::Update`) with r3 = the zone. Only caller `sub_82214718` (the HUD's update, a lead; skipped when the race data's word +4 is 8, 9 or 10); probably once a tick. |
| `sub_8228BDD0` | `TrackZones::IsInside` | | r3 = the owner, r4 = i: the list's i-th zone's slot 2 (+8, `Zone::IsInside`), tail-called; returns its flag. No bounds check. From `sub_82212F00` (zones 0-31 and the HUD's +416) and `sub_82213510` (zones 32-69 and the HUD's +428), each twice (the HUD's, a lead). |
| `sub_8228B5A0` | `List128::Link` | | `List64::Link` for 128 slots: r3 = the list, r4 = a position: the free head (+512) taken and linked in at the position (after the head +516 for 0, else after the node at position - 1), count +520 up by 1. No check for a full list. Leaf. Only caller `List128::Insert`. |
| `sub_8228BC18` | `List128::Insert` | | r3 = the list, r4 = &a word, r5 = a position: `List128::Link`, then the node at the position's data slot (+524 + 4 × slot) = the word. Only caller `TrackZones::Create` (70 times: the zones). |
| `sub_820C9718` | `Racers::Vehicle` | | r3 = a racer index: the racer `[[0x826D82F4] + 2308 + 4 × r3]` (the race's manager, `../known-data.md`); 0 when it's 0 or its word +0 lacks bit 0x40 (probably "has a car"), else `Vehicle::Get` of its +20 (tail call). No bounds check. 4 callers, all seen: `sub_820DAB58`, `sub_821591F0`, `sub_8215D2B0`, `TrackZones::Create`. The race manager's, kept here until that's mapped (its lead in `../loops/leads.md`). |
| `sub_8228B540` | `Zone::Reset` | | r3 = a zone, r4 = &(its index, a vehicle): its 16-slot range list (+4) reset (nodes chained, free head +68 and count +76 = 0, head +72 and +64, +70, +74 = -1), +17104 = the index, +17108 = the vehicle, +17112 = 0 (outside). Leaf. Called first by each of the 70 set-ups. |
| `sub_8228BC80` | `Zone::Update` | | Slot 1 of every zone's vtable (no direct callers; from `TrackZones::Update`): r3 = a zone. The vehicle's track position (`Vehicle::Identity` of +17108, 96 bytes copied to the stack, `memcpy`); +17112 = 0; then each range in the list (+80 + 1,064 × slot, in list order): `RouteRange::Update` with the copy + 4 (the section and slice); at the first that returns 1, +17112 = 1 and the rest aren't updated that call. |
| `sub_8228B4F8` | `Zone::IsInside` | | Slot 2 of every zone's vtable (no direct callers; from `TrackZones::IsInside`): r3 = a zone: its word +17112. Leaf. |
| `sub_8228BBA8` | `ZoneRanges::Insert` | | r3 = a zone's range list (zone + 4), r4 = a range, r5 = a position: `List16::Link`, then the range's 1,064 bytes copied (`memcpy`) to the node at the position's slot (+76 + 1,064 × slot). From the 70 set-ups, always at the end (r5 = the count +72). |
| `sub_821531C0` | `List16::Link` | | `List64::Link` for 16 slots: r3 = the list, r4 = a position: free head +64, head +68, count +72 up by 1. No check for a full list. Leaf. From `ZoneRanges::Insert` and `sub_82154378`, `sub_821543E8`, `sub_82154458` (the camera modes' area, `../loops/leads.md`; 16-slot lists there too). |
| `sub_820FE058` | `RouteRange::Init` | | r3 = a range, r4 = &the start, r5 = &the end (each a section and a slice, halfwords): +4 = the start, +8 = the end; both lists (+12 the sections to take, +536 the sections to leave out) reset (64 nodes chained, the free head and count 0, the head and the halfwords +258, +262 -1); +0 = 1 (the state), +1060 = 1 (the sections beside the ends are checked). Leaf. 71 callers (70 seen: the zones' set-ups, once to six times each; `sub_82155288` not seen). |
| `sub_820FE8C8` | `RouteRange::AddBranch` | | r3 = a range, r4 = a section: added at the end of the range's list of sections to take at forks (`List64::Insert` on +12, at its count +276). From 11 callers seen (the zones' set-ups) and `sub_82155288` (not seen, 44 calls). |
| `sub_820FE2C8` | `List64::Link` | | r3 = a 64-slot list, r4 = a position (signed halfword): the free list's head node (+256) taken off it (+256 = its next) and linked in at the position (after the list's head +260 for 0, else after the node at position - 1, following the nodes' +0), the count +264 up by 1. No check for a full list. Leaf. Only caller `List64::Insert`. |
| `sub_820FE330` | `List64::Insert` | | r3 = a 64-slot list, r4 = &a word, r5 = a position: `List64::Link`, then the node now at the position (from the head, following +0) has its data slot's word (+268 + 4 × the node's +2) = the word. From `RouteRange::AddBranch` and `sub_82148428` (a list of models, a lead). |
| `sub_820FE878` | `RouteRange::Update` | | r3 = a range, r4 = a position (section, slice). With a section below 0x8000 (unsigned; -1 is none): +0 = `RouteRange::Test`; returns 1 when +0 is now 2 (on the range), else 0 (the old state kept when there's no section). 3 callers (only `Zone::Update` seen; `sub_821562F8` and `sub_82156980` not). |
| `sub_820FE6F8` | `RouteRange::Test` | | r3 = a range, r4 = a position. 0 when the state +0 is 0. 1 when the position's section is in the list of sections to leave out (+536; empty in play). Else `RouteRange::OnPath`; when that isn't 2 and +1060 is set, for the position's section's links 6 to 9 (+52 to +64; -1 skipped) the position mapped onto that section (`RouteRange::ToNeighbor`), and `OnPath` with it: 2 when one is. Returns 0, 1 (not on it) or 2 (on it). 3 callers (`RouteRange::Update` seen; `sub_820FE830`, `sub_821562F8` not). |
| `sub_820FE398` | `RouteRange::OnPath` | | r3 = a range, r4 = a position. 0 when +0 is 0. `0x82603274` = the position's slice. When the position is on the start's section: 2 if `RouteRange::WithinEnds`, else 1. Else 1 when the start's and end's sections are the same. Else walks from the start's section: on each section, 2 when the position is on it (or on one of its forward links 0 to 2, +28 to +36) and `WithinEnds`; `0x82603274` += the section's slice count (+0); the next section is its one forward link, or with more than one the first whose section is in the list of sections to take (+12); 1 when there's none (no step) or on reaching the end's section. Returns 0, 1 or 2. Reads the route's sections (`[0x826C5B10]` + 12). Callers `RouteRange::Test` (twice). |
| `sub_820FE120` | `RouteRange::WithinEnds` | | r3 = a range, r4 = a position: 1 unless the position is on the start's section before the start's slice, or on the end's section after the end's slice. With +1060 set, also for the start's and end's sections' links 6 to 9: the ends mapped onto those sections (`RouteRange::ToNeighbor`) and the same tests against the mapped ends (one branch also clears "after the end" when the end's beside section is the position's and its slice is above -1; as the code has it). Returns 1 or 0. Callers `RouteRange::OnPath` (three times). |
| `sub_820FDF98` | `RouteRange::ToNeighbor` | | r3 = out (section, slice), r4 = a section, r5 = a position (section, slice): out = (r4, the slice) and returns 1, unless one of the position's section's forward links 0 and 1 has the same section as one of r4's (not -1) and the two have different slice counts: then the slice is moved by the difference (slice - its count + r4's count; aligned at their shared end) and 0 is returned when that's below 0 (out still written). Probably the position mapped onto the section beside it. Reads the route's sections (`[0x826C5B10]` + 12). Leaf. Callers `RouteRange::WithinEnds` (twice) and `RouteRange::Test`. |
| `sub_8228BE20` | `Zone0::Setup` | | Zone 0's set-up (slot 0 of `0x82011AF0`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (248, 0)-(248, 43). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228BE88` | `Zone1::Setup` | | Zone 1's set-up (slot 0 of `0x82011AFC`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (186, 113)-(183, 25). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228BEF0` | `Zone2::Setup` | | Zone 2's set-up (slot 0 of `0x82011B08`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (287, 4)-(288, 23). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228BF58` | `Zone3::Setup` | | Zone 3's set-up (slot 0 of `0x82011B14`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (288, 24)-(289, 24). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228BFC0` | `Zone4::Setup` | | Zone 4's set-up (slot 0 of `0x82011B20`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (224, 112)-(178, 31). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C028` | `Zone5::Setup` | | Zone 5's set-up (slot 0 of `0x82011B2C`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (119, 43)-(125, 21). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C090` | `Zone6::Setup` | | Zone 6's set-up (slot 0 of `0x82011B38`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (115, 52)-(116, 23). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C0F8` | `Zone7::Setup` | | Zone 7's set-up (slot 0 of `0x82011B44`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (95, 10)-(97, 17). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C160` | `Zone8::Setup` | | Zone 8's set-up (slot 0 of `0x82011B50`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (139, 10)-(139, 51). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C1C8` | `Zone9::Setup` | | Zone 9's set-up (slot 0 of `0x82011B5C`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (109, 40)-(91, 41). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C230` | `Zone10::Setup` | | Zone 10's set-up (slot 0 of `0x82011B68`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (6, 12)-(6, 53). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C298` | `Zone11::Setup` | | Zone 11's set-up (slot 0 of `0x82011B74`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (9, 69)-(9, 108). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C300` | `Zone12::Setup` | | Zone 12's set-up (slot 0 of `0x82011B80`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (2, 17)-(2, 25). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C368` | `Zone13::Setup` | | Zone 13's set-up (slot 0 of `0x82011B8C`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (270, 52)-(272, 21). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C3D0` | `Zone14::Setup` | | Zone 14's set-up (slot 0 of `0x82011B98`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (206, 1)-(207, 17). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C438` | `Zone15::Setup` | | Zone 15's set-up (slot 0 of `0x82011BA4`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (25, 9)-(26, 15). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C4A0` | `Zone16::Setup` | | Zone 16's set-up (slot 0 of `0x82011BB0`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (265, 5)-(265, 45). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C508` | `Zone17::Setup` | | Zone 17's set-up (slot 0 of `0x82011BBC`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (199, 1)-(200, 25). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C570` | `Zone18::Setup` | | Zone 18's set-up (slot 0 of `0x82011BC8`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (260, 40)-(261, 23). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C5D8` | `Zone19::Setup` | | Zone 19's set-up (slot 0 of `0x82011BD4`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (203, 127)-(171, 17). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C640` | `Zone20::Setup` | | Zone 20's set-up (slot 0 of `0x82011BE0`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (46, 17)-(47, 17). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C6A8` | `Zone21::Setup` | | Zone 21's set-up (slot 0 of `0x82011BEC`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (252, 94)-(253, 17). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C710` | `Zone22::Setup` | | Zone 22's set-up (slot 0 of `0x82011BF8`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (194, 22)-(174, 17). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C778` | `Zone23::Setup` | | Zone 23's set-up (slot 0 of `0x82011C04`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (76, 89)-(83, 19). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C7E0` | `Zone24::Setup` | | Zone 24's set-up (slot 0 of `0x82011C10`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (30, 683)-(29, 19). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C848` | `Zone25::Setup` | | Zone 25's set-up (slot 0 of `0x82011C1C`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (56, 210)-(57, 25). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C8B0` | `Zone26::Setup` | | Zone 26's set-up (slot 0 of `0x82011C28`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (276, 81)-(277, 23). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C918` | `Zone27::Setup` | | Zone 27's set-up (slot 0 of `0x82011C34`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (219, 77)-(220, 19). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C980` | `Zone28::Setup` | | Zone 28's set-up (slot 0 of `0x82011C40`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (140, 19)-(142, 9). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228C9E8` | `Zone29::Setup` | | Zone 29's set-up (slot 0 of `0x82011C4C`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (14, 99)-(16, 0). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228CA50` | `Zone30::Setup` | | Zone 30's set-up (slot 0 of `0x82011C58`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (33, 5)-(35, 13). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228CAB8` | `Zone31::Setup` | | Zone 31's set-up (slot 0 of `0x82011C64`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (72, 104)-(73, 11). Its flag read by `sub_82212F00` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228CB20` | `Zone32::Setup` | | Zone 32's set-up (slot 0 of `0x82011C70`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (246, 39)-(246, 87); (181, 88)-(182, 14). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228CBC8` | `Zone33::Setup` | | Zone 33's set-up (slot 0 of `0x82011C7C`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (246, 88)-(247, 26); (181, 25)-(181, 87). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228CC70` | `Zone34::Setup` | | Zone 34's set-up (slot 0 of `0x82011C88`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (248, 0)-(249, 68); (186, 113)-(185, 25). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228CD18` | `Zone35::Setup` | | Zone 35's set-up (slot 0 of `0x82011C94`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (249, 69)-(250, 53). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228CD80` | `Zone36::Setup` | | Zone 36's set-up (slot 0 of `0x82011CA0`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (251, 14)-(251, 93); (186, 40)-(186, 80). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228CE20` | `Zone37::Setup` | | Zone 37's set-up (slot 0 of `0x82011CAC`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 6 ranges: (252, 94)-(256, 15); (252, 94)-(242, 25) taking 239; (194, 22)-(191, 20) taking 188; (194, 22)-(177, 25) taking 174; (76, 89)-(82, 28) taking 79; (76, 89)-(257, 0) taking 83. Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228CFC8` | `Zone38::Setup` | | Zone 38's set-up (slot 0 of `0x82011CB8`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (258, 1)-(258, 155); (193, 55)-(194, 21). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228D070` | `Zone39::Setup` | | Zone 39's set-up (slot 0 of `0x82011CC4`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 6 ranges: (260, 40)-(263, 17) taking 261; (260, 40)-(237, 15) taking 235; (203, 127)-(197, 25) taking 195; (203, 127)-(173, 14) taking 171; (46, 17)-(49, 26) taking 47; (46, 17)-(52, 15) taking 50. Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228D220` | `Zone40::Setup` | | Zone 40's set-up (slot 0 of `0x82011CD0`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (264, 45)-(264, 120); (202, 5)-(203, 77). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228D2C8` | `Zone41::Setup` | | Zone 41's set-up (slot 0 of `0x82011CDC`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (265, 5)-(267, 17); (199, 1)-(202, 6). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228D370` | `Zone42::Setup` | | Zone 42's set-up (slot 0 of `0x82011CE8`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (269, 74)-(269, 236); (204, 119)-(205, 21). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228D418` | `Zone43::Setup` | | Zone 43's set-up (slot 0 of `0x82011CF4`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 6 ranges: (270, 52)-(274, 31); (270, 52)-(234, 9) taking 232; (206, 1)-(209, 20) taking 207; (206, 1)-(1, 2) taking 168; (25, 9)-(24, 22) taking 22; (25, 9)-(28, 30) taking 26. Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228D5C0` | `Zone44::Setup` | | Zone 44's set-up (slot 0 of `0x82011D00`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (276, 81)-(277, 23); (210, 0)-(210, 131). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228D668` | `Zone45::Setup` | | Zone 45's set-up (slot 0 of `0x82011D0C`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (278, 80)-(279, 21); (215, 5)-(217, 18). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228D710` | `Zone46::Setup` | | Zone 46's set-up (slot 0 of `0x82011D18`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (223, 41)-(212, 3). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228D778` | `Zone47::Setup` | | Zone 47's set-up (slot 0 of `0x82011D24`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 4 ranges: (280, 15)-(280, 55); (281, 15)-(281, 55); (221, 51)-(223, 40); (222, 51)-(223, 40). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228D870` | `Zone48::Setup` | | Zone 48's set-up (slot 0 of `0x82011D30`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 4 ranges: (282, 0)-(285, 11); (283, 0)-(285, 11); (219, 77)-(221, 50); (219, 77)-(222, 50). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228D968` | `Zone49::Setup` | | Zone 49's set-up (slot 0 of `0x82011D3C`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (286, 0)-(286, 50); (219, 24)-(219, 55). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228DA08` | `Zone50::Setup` | | Zone 50's set-up (slot 0 of `0x82011D48`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 6 ranges: (287, 4)-(290, 7) taking 288; (287, 4)-(245, 9) taking 243; (225, 17)-(227, 13); (178, 17)-(180, 9); (119, 43)-(124, 14) taking 122; (119, 43)-(127, 28) taking 125. Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228DBA8` | `Zone51::Setup` | | Zone 51's set-up (slot 0 of `0x82011D54`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (290, 8)-(291, 14); (224, 112)-(94, 25) taking 243. Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228DC60` | `Zone52::Setup` | | Zone 52's set-up (slot 0 of `0x82011D60`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (291, 15)-(291, 80); (224, 75)-(224, 110). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228DD00` | `Zone53::Setup` | | Zone 53's set-up (slot 0 of `0x82011D6C`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 3 ranges: (115, 52)-(119, 3) taking 113; (95, 10)-(103, 17) taking 96; (95, 10)-(103, 17) taking 97. Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228DDE8` | `Zone54::Setup` | | Zone 54's set-up (slot 0 of `0x82011D78`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (115, 20)-(115, 51). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228DE50` | `Zone55::Setup` | | Zone 55's set-up (slot 0 of `0x82011D84`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (0, 0)-(0, 0). One point, section 0 slice 0: probably a placeholder. Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228DEB0` | `Zone56::Setup` | | Zone 56's set-up (slot 0 of `0x82011D90`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (113, 37)-(114, 97); (105, 145)-(106, 36). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228DF58` | `Zone57::Setup` | | Zone 57's set-up (slot 0 of `0x82011D9C`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (112, 232)-(113, 36); (107, 10)-(107, 78). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228E000` | `Zone58::Setup` | | Zone 58's set-up (slot 0 of `0x82011DA8`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (107, 288)-(107, 348). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228E068` | `Zone59::Setup` | | Zone 59's set-up (slot 0 of `0x82011DB4`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (111, 112)-(111, 257). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228E0D0` | `Zone60::Setup` | | Zone 60's set-up (slot 0 of `0x82011DC0`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (139, 10)-(146, 12) taking 142; (109, 40)-(93, 13) taking 91. Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228E190` | `Zone61::Setup` | | Zone 61's set-up (slot 0 of `0x82011DCC`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (151, 1)-(152, 45); (161, 55)-(161, 149). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228E238` | `Zone62::Setup` | | Zone 62's set-up (slot 0 of `0x82011DD8`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (6, 12)-(8, 69); (9, 69)-(11, 16). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228E2E8` | `Zone63::Setup` | | Zone 63's set-up (slot 0 of `0x82011DE4`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (5, 20)-(5, 90); (12, 80)-(13, 70). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228E390` | `Zone64::Setup` | | Zone 64's set-up (slot 0 of `0x82011DF0`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its range: (17, 27)-(19, 13). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228E3F8` | `Zone65::Setup` | | Zone 65's set-up (slot 0 of `0x82011DFC`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (30, 205)-(30, 292); (45, 374)-(45, 504). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228E498` | `Zone66::Setup` | | Zone 66's set-up (slot 0 of `0x82011E08`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (30, 683)-(34, 20) taking 31; (38, 0)-(44, 35). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228E550` | `Zone67::Setup` | | Zone 67's set-up (slot 0 of `0x82011E14`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (55, 258)-(55, 476); (75, 271)-(75, 379). Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228E5F0` | `Zone68::Setup` | | Zone 68's set-up (slot 0 of `0x82011E20`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 2 ranges: (56, 75)-(56, 181); (72, 104)-(74, 20) taking 73. Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
| `sub_8228E6A0` | `Zone69::Setup` | | Zone 69's set-up (slot 0 of `0x82011E2C`; through the vtable from `TrackZones::Create`): `Zone::Reset`, then its 3 ranges: (56, 210)-(58, 35) taking 57; (67, 56)-(72, 10) taking 69; (67, 56)-(72, 10) taking 70. Its flag read by `sub_82213510` (the HUD, a lead). Not read in detail (its calls and numbers only). |
