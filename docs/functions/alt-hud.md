# Known functions: the alternate HUD

The race HUD holder's object at +1004 (`AltHud::`, our name; the holder
`RaceScreens::` at `[0x826D84F0]`, `race-hud-object.md`): a second
dashboard on UI movie 22 (found by id in the movie table at
`0x826C5CCC`, `UiMovies::Find`; loaded by most race kinds' masks,
`RaceScreens::MovieMask`). The holder's +1004 is two words: +0 the
object (264 bytes, cleared) and +4 a menu list (72 bytes, one item),
both made by `RaceScreens::Make1004` (`sub_82216A60`, its row in
`race-hud-object.md`, native since 2026-10-31) from `RaceScreens::Init`
and freed by `AltHud::Destroy` from `RaceScreens::~RaceScreens`. It
shows what the race HUD's dashboard does, from the same car fields and
in the same way (the speed, the gear, the tachometer and boost needles
by `HudCurve::Eval`, the bar, the three extra gauges' digits by
`PowInt`, node 25 by the build's +3892), plus the car input record's
throttle, brake and steering (`[0x826C5B30 + 4 × player]`,
`racer-controllers.md`), so probably the attract or replay races'
dashboard (a guess: which race kinds those are isn't known). The race
flow's handlers (`8216C208`-`8216E668`, a lead in `../loops/leads.md`)
pick it: the set-up handler `sub_8216D450` sets it up (`AltHud::SetUp`)
for the kinds at +9612 1, 2, 4 and 5 with player 0, and 6 with player 0
or 1 (by bit 0x200 of a racer record's word +0), beside the race HUD
but for kinds 4 and 5; the update handler `sub_8216D918` ticks it
(`AltHud::Tick`) instead of the race HUD for kind 5, and for kinds 1 to
3 and 6 when the holder's +8 has bit 0 set (cleared by
`RaceScreens::Init`; what sets it isn't known). The Draw handlers
`sub_8216C330`, `sub_8216CE00`, `sub_8216CF08`, `sub_8216CFE0` (and
`sub_8216C460`, never seen in play) run movie 22 (`AltHud::RunMovie`).
A view switch (`AltHud::ToggleView`, from `sub_8216CEB8`, never seen in
play) shows a big speed readout in place of the gauges.

Movie 22's nodes: the live scene's children 1 to 4, 15 to 17, 21, 23, 25
and 30 (by `UiMovie::FindNode(movie, -1, n)`), and sub-scene 7's
children 1, 5, 7, 8 and 9 to 14 (the gauges); +80 a node's shown flag,
+108 its frame (a digit + 1, 0 hidden), +36 its rotation, +28 its x
scale, +12 its x. Each function finds the movie itself, with the key
word 22 left in its frame (at r1 - frame + 80, + 92 in
`AltHud::SetUpExtraGauges`).

Read by the B7 loop's mapping agent (2026-10-31, from the lead in
`../loops/leads.md`), all in full but the never-seen digit setters.
Shared, with rows elsewhere: `RaceScreens::Make1004`, `HudCurve::Eval`
(`sub_82212118`) and `PowInt` (`sub_82213C40`) in `race-hud-object.md`;
`UiMovies::Find`, `UiMovie::FindNode` and `UiMovie::Run` in
`ui-movies-game.md`; `Vehicle::Get`, `sub_820C0140` and `sub_820C4550`
in `vehicle-accessors.md`; `Vehicle::GetSpeedKmh` in `functions.md`;
`CarData::CarData` in `car-physics.md`; `Car::SetupVehicle` in
`race-setup.md`; `PowerCurve::FromEngine` in `vehicle.md`; `memcpy` in
`c-runtime.md`; `operator delete` and `VirtualFree` in `heap.md`;
`MenuList::Init` in `save-data.md`; `ActionMapper::Pressed` (the
mappers' slot 10, `sub_8210BF40`) in `action-mappers.md`. The menu list functions here
(`MenuList::Release`, `MenuList::PressedX`) are generic, beside the menu
lists' lead in `../loops/leads.md`. The race HUD's own functions
(`RaceHud::`, `race-hud-object.md`) are the model for most rows: the
alternate HUD's are copies of them on movie 22, with the differences
noted. The +656 object (`sub_822157E8` and the rest, a lead) also calls
`AltHud::PowerClass` (`sub_822158E8`, never seen in play).

The work is in Update handlers (the tick) but for `AltHud::RunMovie`,
from Draw handlers, which run on render-only passes too (it runs the
movie with r4 = 0; the UI movies' frame interpolation handling,
`--per_tick_ui_movie_tracks`, is inside `UiMovie::Run`).

Native (B7 loop, 2026-10-31, the migration): `src/alt_hud/` behind
`--native_alt_hud` (adapters in `src/hooks/alt_hud.cpp` for the
recompiled callers' entries: `AltHud::SetUp`, `Tick`, `RunMovie`, and
`MenuList::Release` for its other callers; harness `alt_hud`). A
temporary counter showed none of the alternate HUD's functions called in
a 40-second attract race (the attract races tick the race HUD), so its
harness carries the check; `MenuList::Release` ran twice there (the
front end). Two rows were corrected on the way: `MenuList::PressedX`
answers 1 when +24 is 0 (so X flips the toggle), and `MenuList::Release`
returns 0 when there's no block.

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
| `sub_82217C70` | `AltHud::SetUp` | Overridden | r3 = the holder's +1004 (the object at +0), r4 = a player: +260 = r4, +240 = `Vehicle::Get(r4)` (the car), then `AltHud::SetUpGauges`, `AltHud::SetUpExtraGauges` and `AltHud::ShowPanels`. A 96-byte frame (r31 saved). From the race flow's set-up handler `sub_8216D450` (three call sites: kinds 1, 2, 4 and 5 with player 0, kind 6 with 0 or 1) and `sub_8216D328` (never seen in play); not read. Read in full. Returns `ShowPanels`' r3. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::SetUp`; `--native_alt_hud`, adapter in `src/hooks/alt_hud.cpp`, harness `alt_hud`): `Vehicle::Get` and its callees' plain C++; f1 left as `SetUpGauges` leaves it (the 0.0). Not called in a 40-second attract race (a temporary counter, 2026-10-31): the attract races' kind ticks the race HUD; checked by its harness. |
| `sub_82216C40` | `AltHud::SetUpGauges` | Native | r3 = the holder's +1004: as `RaceHud::SetUpGauges` on movie 22. By the player's racer record (race data `[0x826C5B2C]` + 48 + 3,936 × +260, a car object): its +12 (the record's +60, an index i) and `AltHud::PowerClass` (0-2, the class c) pick a tachometer curve (one of three 112-byte curves at `0x8200FD28`, `0x8200FD98`, `0x8200FE08`, copied first to its frame's +96, +208, +320; by the table at `0x8200FEE8`, 16 bytes an index: word +0 the curve) copied to +4 (`memcpy`) and the dial's face (`[0x8200FEEC + 16i + 4c]`) into node (7, 8) +108; the boost curve (`0x8200FE78`) copied to +116; both evaluated at the float at `0x82000A34` (probably 0.0; `HudCurve::Eval`). A 496-byte frame (r27 to r31 and f31 saved). Only caller `AltHud::SetUp`. Read in full. Unlike `RaceHud::SetUpGauges` (the record's +1704 for the class), the class is the engine's power. The three curves stay in its frame. Returns `HudCurve::Eval`'s r3. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::SetUpGauges`, harness `alt_hud`): memcpy's plain C++, or its guest call where that refuses; `HudCurve::Eval`'s and `PowerClass`' plain C++. No adapter: only `AltHud::SetUp`'s plain C++ reaches it, and `--native_alt_hud` switches that. |
| `sub_82216B08` | `AltHud::PowerClass` (our name) | Native | r3 not read, r4 = a car object: its spec's block 2 (`sub_820C4550`, car + 1888), a car data built on the stack (`CarData::CarData` at r1 - 3040 + 1152, `Car::SetupVehicle(r4, it)`) and its power curve (`PowerCurve::FromEngine` at + 80, from the car data's +1268); returns 0 when the curve's peak power (+1056) is below block 2's +12 × the float at `0x820029F0`, 1 when below +12 × the float at `0x82002C40`, else 2. Probably the engine's power against the stock car's, in three classes. A 3,040-byte frame (r30, r31 saved); the car data and the curve stay in it. From `AltHud::SetUpGauges` and the +656 object's `sub_822158E8` (never seen in play). Read in full; what block 2's +12 is isn't known. In the front end's power curve lead in `../loops/leads.md` until now. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::PowerClass`, harness `alt_hud`: Vehicle::SetupFromCar's cases for the car, the frame's power curve and car data compared as floats): `Object::Record1888`, `CarData::CarData`, `Car::SetupVehicle` (its override where its plain C++ refuses) and `PowerCurve::FromEngine`'s plain C++. No adapter: only `AltHud::SetUpGauges`' plain C++ reaches it in play (`sub_822158E8`, never seen in play, calls the original). |
| `sub_822178A0` | `AltHud::SetUpExtraGauges` | Native | r3 = the holder's +1004: as `RaceHud::SetUpExtraGauges` on movie 22: from the car's build (`[[+240] + 5712]`) +1832, five 12-byte entries: type 1 sets a flag kept in a register (also when the build's +12 is 10), types 2, 3 and 4 a flag each (its frame's +80, +84, +88); the three slots +244 to +252 (from the last) get the first flag set of the three (0, 1 or 2, the flag cleared) or -1; each slot's nodes shown when it isn't -1 (`AltHud::ShowGauge`); node (7, 1) +80 = the type 1 flag (not kept in the object, unlike the HUD's +476). Probably the extra gauges a build has fitted. A 160-byte frame (r27 to r31 saved). Only caller `AltHud::SetUp`. Read in full. The flags at r1 - 160 + 80 to + 88 and their addresses at + 96 to + 104 are left there, the key 22 at + 92. Returns node (7, 1). Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::SetUpExtraGauges`, harness `alt_hud`). No adapter: only `AltHud::SetUp`'s plain C++ reaches it. |
| `sub_82217380` | `AltHud::ShowGauge` | Native | r3 not read, r4 = a slot (0-2), r5 = shown: movie 22's nodes (7, 12 + r4) and (7, 9 + r4) +80 = r5. A 128-byte frame (r28 to r31 saved). Only caller `AltHud::SetUpExtraGauges`. Read in full. Returns the second node. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::ShowGauge`, harness `alt_hud`). No adapter: only `AltHud::SetUpExtraGauges`' plain C++ reaches it. |
| `sub_82216D78` | `AltHud::ShowPanels` (our name) | Native | r3 = the holder's +1004: movie 22's node 3 +80 = the object's +0 (the toggle `AltHud::Tick` flips), node 2 +80 = 0. A 112-byte frame (r30, r31 saved). Only caller `AltHud::SetUp`. Read in full. Returns node 2. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::ShowPanels`, harness `alt_hud`). No adapter: only `AltHud::SetUp`'s plain C++ reaches it. |
| `sub_82217AC8` | `AltHud::Tick` | Overridden | r3 = the holder's +1004: when `MenuList::PressedX(+4)` is true, the object's +0 flipped (xor 1) and node 3 +80 = it. Then by the car (+240) and the input record (`[0x826C5B30 + 4 × (the player +260 & 0xFF)]`): the speed (`Vehicle::GetSpeedKmh` × the double at `0x8200A510`, frsp, truncated; `AltHud::SetSpeed`), the extra gauges (`AltHud::UpdateExtraGauges` with f1 = the car's float +6260, r5 = +6256 and r6 = +6252 truncated from floats), the gear (+2252, `AltHud::SetGear`), the tachometer (`sub_820C0140`'s float, `AltHud::SetTach`), node 25 +80 = the build's (`[+5712]`) +3892 (`AltHud::SetNode25`), the bar (+2420, `AltHud::SetBar`), the boost (+2944, `AltHud::SetBoost`), then the input record's halfwords +4, +6 and +8 (the throttle, brake and steering, signed) each as a float × the float at `0x82002BD4` (probably 1 / 32,767) into `AltHud::SetThrottle`, `SetBrake` and `SetSteering`. A 128-byte frame (r29 to r31 and f31 saved). As `RaceHud::Tick` but for the order (node 25 before the bar and boost, the signs not updated) and the inputs. Only caller the race flow's update handler `sub_8216D918` (kind 5, or kinds 1 to 3 and 6 with the holder's +8 bit 0 set), probably once a tick. Read in full. Its fctiwz words and the halfwords' doublewords go through r1 - 128 + 80 (the last written stays). Returns 0 (so `sub_8216D918`'s code after the call, for a non-zero answer, never runs). Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::Tick`; `--native_alt_hud`, adapter in `src/hooks/alt_hud.cpp`, harness `alt_hud`): its callees' plain C++, `Vehicle_GetSpeedKmh` and `sub_820C0140`'s included; f1 left as `SetSteering`'s input. Not called in a 40-second attract race (a temporary counter, 2026-10-31): the attract races' kind ticks the race HUD; checked by its harness. |
| `sub_8220BF00` | `MenuList::PressedX` (probably) | Native | r3 = a menu list: the action mapper entry of its player's controller (`0x826BAB80` + 8 × `[0x826BABA4 + 4 × +60]`); unless the entry's +4 is 1, the mapper's slot 10 (+40, `ActionMapper::Pressed`) for action 160 (X, `../pause-menu-fix.md`); 1 when that's true and the list's +24 isn't 0, else 0. A 96-byte frame (r31 saved). Only caller `AltHud::Tick`. Read in full. Like the menu lists' "confirm" (`sub_8220BD50`) and `sub_8220BDE0` (the menu lists' lead), which test +24 the same way. `MenuList::Init` clears +24 and only `MenuList::Update` (`sub_8220C1E0`) and `sub_8220BC40` write it, which nothing calls on this list: here it probably stays 0, so X flips the toggle (corrected 2026-10-31: the function answers 1 when +24 is 0; not checked in play). Correction (B7 loop, 2026-10-31, the migration): it's 1 when the mapper's answer is true and +24 **is** 0 (the code keeps `li r3,1` when +24 is 0), so with +24 left 0 the toggle flips on X. Native in `src/alt_hud/menu_list.cpp` (`alt_hud::MenuListPressedX`, harness `alt_hud`): `ActionMapper::Pressed`'s plain C++ when the slot is it and `--native_action_mappers` is on (with the read slots' `--log_pause_menu` line), else the slot through the function table from its 96-byte frame (lr `0x8220BF5C`). No adapter: only `AltHud::Tick`'s plain C++ reaches it. |
| `sub_82216E00` | `AltHud::SetSpeed` | Native | r3 not read, r4 = the speed: as `RaceHud::SetSpeed` on movie 22's node 21: when 999 or less, its children (from its +100, +104 of them, last first) get +108 = digit + 1, leading zeros 0 (hidden; by `PowInt(10.0, n)`, the double at `0x8200F728`). A 160-byte frame (r25 to r31 and f31 saved). Only caller `AltHud::Tick`. Read in full. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::SetSpeed`, harness `alt_hud`; `PowInt`'s plain C++). Above 999 it returns `UiMovies::Find`'s answer (the value's address). No adapter: only `AltHud::Tick`'s plain C++ reaches it. |
| `sub_82217A30` | `AltHud::UpdateExtraGauges` | Native | r3 = the holder's +1004, f1, r5, r6: as `RaceHud::UpdateExtraGauges`: for each slot (+244 to +252, i 0-2): 0, `AltHud::GaugeFloat(f1, i)`; 1, `AltHud::GaugeInt119(r5, i)`; 2, `AltHud::GaugeInt149(r6, i)`; 3 and up (-1) nothing. A 144-byte frame (r27 to r31 and f31 saved). Only caller `AltHud::Tick`. Read in full. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::UpdateExtraGauges`, harness `alt_hud`); the three digit setters (never seen in play) through the function table (the originals). No adapter: only `AltHud::Tick`'s plain C++ reaches it. |
| `sub_82216FD0` | `AltHud::GaugeFloat` | | r3 not read, f1 = a value, r5 = the slot: as `RaceHud::GaugeFloat` on movie 22: from 0.0 to under the float at `0x82001FD8` (probably 10.0, also the tenths' factor), the slot's digits (node (7, 12 + r5) children: child 3 the tenths, child 2 the point (frame 11), then the integer part, leading zeros hidden by `PowInt`) and node (7, 9 + r5) +108 = 0. A 208-byte frame. Only caller `AltHud::UpdateExtraGauges`. Not read in detail. Not seen in play. Called through the function table (the original) by `alt_hud::UpdateExtraGauges`' plain C++. |
| `sub_82217168` | `AltHud::GaugeInt119` | | r3 not read, r4 = a value, r5 = the slot: when 119 or less, its digits as `AltHud::SetSpeed`'s on node (7, 12 + r5), and node (7, 9 + r5) +108 = 1. A 176-byte frame. Only caller `AltHud::UpdateExtraGauges`. Not read in detail. Not seen in play. Called through the function table (the original) by `alt_hud::UpdateExtraGauges`' plain C++. |
| `sub_82217270` | `AltHud::GaugeInt149` | | r3 not read, r4 = a value, r5 = the slot: when 149 or less, its digits as `AltHud::SetSpeed`'s on node (7, 12 + r5), and node (7, 9 + r5) shown with +108 = 2. A 176-byte frame. Only caller `AltHud::UpdateExtraGauges`. Not read in detail. Not seen in play. Called through the function table (the original) by `alt_hud::UpdateExtraGauges`' plain C++. |
| `sub_82217460` | `AltHud::SetGear` | Native | r3 not read, r4 = the gear: as `RaceHud::SetGear` on movie 22's node 23: +108 = 7 for 6, gear - 1 for 1-5, 5 for 0, 6 below 0 (probably reverse). A 112-byte frame (r31 saved). Only caller `AltHud::Tick`. Read in full. Returns the node. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::SetGear`, harness `alt_hud`). No adapter: only `AltHud::Tick`'s plain C++ reaches it. |
| `sub_822173E8` | `AltHud::SetTach` | Native | r3 = the holder's +1004, f1 = the value: `HudCurve::Eval(+4, f1)`, node (7, 7) +36 (the needle's rotation) = its output (+104) plus the float at `0x8200F720` (as `RaceHud::SetTach`). A 112-byte frame (r31 saved). Only caller `AltHud::Tick`. Read in full. Returns the node. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::SetTach`, harness `alt_hud`). No adapter: only `AltHud::Tick`'s plain C++ reaches it. |
| `sub_822174E0` | `AltHud::SetNode25` (our name) | Native | r3 not read, r4 = a word (the build's +3892): movie 22's node 25 +80 = r4 (as the HUD's node 13 in `RaceHud::Tick`). A 112-byte frame (r31 saved). Only caller `AltHud::Tick`. Read in full; what node 25 is isn't known. Returns the node. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::SetNode25`, harness `alt_hud`). No adapter: only `AltHud::Tick`'s plain C++ reaches it. |
| `sub_822175A0` | `AltHud::SetBar` | Native | r3 = the holder's +1004, f1 = the value: +228 = f1; when f1 is above +236, node 30's +28 (x scale) = +228 / (+232 - +236); else node 30 +80 = 0. A 112-byte frame (r31 and f31 saved). Only caller `AltHud::Tick` (the car's +2420). Read in full. Unlike `RaceHud::SetBar` it never shows the node (+80 = 1), and nothing found writes +232 and +236 (cleared by `RaceScreens::Make1004`; no `RaceHud::ResetBar` for it), so the scale is probably +228 / 0.0 (not checked in play). Returns the node. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::SetBar`, harness `alt_hud`). It loads +236 once, before the comparison (with f1 unrounded). No adapter: only `AltHud::Tick`'s plain C++ reaches it. |
| `sub_82217538` | `AltHud::SetBoost` | Native | r3 = the holder's +1004, f1 = the value: `HudCurve::Eval(+116, f1)`, node (7, 5) +36 = its output (+216) (as `RaceHud::SetBoost`). A 112-byte frame (r31 saved). Only caller `AltHud::Tick` (the car's +2944). Read in full; that it's the boost is a guess, as the HUD's. Returns the node. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::SetBoost`, harness `alt_hud`). No adapter: only `AltHud::Tick`'s plain C++ reaches it. |
| `sub_82217638` | `AltHud::SetThrottle` (probably) | Native | r3 not read, f1 = a value (the input record's +4 × `[0x82002BD4]`): movie 22's node 16 +28 (x scale) = f1. A 112-byte frame (f31 saved). Only caller `AltHud::Tick`. Read in full. Returns the node. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::SetThrottle`, harness `alt_hud`). No adapter: only `AltHud::Tick`'s plain C++ reaches it. |
| `sub_82217690` | `AltHud::SetBrake` (probably) | Native | As `AltHud::SetThrottle` on node 17 (the input record's +6). Only caller `AltHud::Tick`. Read in full. Returns the node. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::SetBrake`, harness `alt_hud`). No adapter: only `AltHud::Tick`'s plain C++ reaches it. |
| `sub_822176E8` | `AltHud::SetSteering` (probably) | Native | r3 not read, f1 = a value (the input record's +8 × `[0x82002BD4]`): movie 22's node 15 +12 (x) = 188 - trunc(-f1 × the float at `0x82010368`), as a float. A 112-byte frame (f31 saved). Only caller `AltHud::Tick`. Read in full. The truncated word and the difference's doubleword at r1 - 112 + 80. Returns the node. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::SetSteering`, harness `alt_hud`). No adapter: only `AltHud::Tick`'s plain C++ reaches it. |
| `sub_82216C00` | `AltHud::RunMovie` (our name) | Overridden | No arguments: `UiMovie::Run(movie 22, 0)` (advance and draw). A 96-byte frame. From the race flow's Draw handlers `sub_8216C330`, `sub_8216CE00`, `sub_8216CF08`, `sub_8216CFE0` and `sub_8216C460` (never seen in play), which run on render-only passes too. Read in full. Returns `UiMovie::Run`'s r3. Native in `src/alt_hud/alt_hud.cpp` (`alt_hud::RunMovie`; `--native_alt_hud`, adapter in `src/hooks/alt_hud.cpp`, harness `alt_hud`): `UiMovies::Find`'s plain C++, `UiMovie::Run` through its override (lr `0x82216C30`). Not called in a 40-second attract race (a temporary counter, 2026-10-31): the attract races' kind ticks the race HUD; checked by its harness. |
| `sub_82217778` | `AltHud::ToggleView` (our name) | | r3 = the holder's +1004: +256 = !+256; when set, movie 22's nodes 1 and 3 +80 = 0, node 2 +80 = 1 and the speed (as `AltHud::Tick`'s) into `AltHud::SetBigSpeed`; when clear, node 1 +80 = 1, node 3 +80 = the object's +0, node 2 +80 = 0. A 128-byte frame (r28 to r31 saved). From `sub_8216CEB8` (two tail calls: kind 5, or kinds up to 4 and 6 with the holder's +8 bit 0 set; probably a button's handler). Read in full. Not seen in play. |
| `sub_82216EE8` | `AltHud::SetBigSpeed` (our name) | | As `AltHud::SetSpeed` on movie 22's node 4. Only caller `AltHud::ToggleView`. Read in full. Not seen in play. |
| `sub_82216BA8` | `AltHud::Destroy` | | r3 = the holder's +1004: `MenuList::Release(+4)`, `operator delete(+0)`, then when +4 isn't 0, `MenuList::Release` on it again and `operator delete(+4)`. The first `Release` doesn't check +4 for 0. A 96-byte frame (r31 saved). Only caller `RaceScreens::~RaceScreens`. Read in full. Returns the last `operator delete`'s r3. |
| `sub_8220BBB0` | `MenuList::Release` (probably) | Overridden | r3 = a menu list: when its block (+68, `MenuList::Init`'s per-item words) is set, `VirtualFree(block, 0, MEM_RELEASE)` (`sub_82295330`) and +68 = 0. A 96-byte frame (r31 saved). About 50 call sites in 25 functions: the front end's screens (`sub_82180E70` to `sub_82259948`), the in-race screens' teardowns (`sub_8220C4C8`, `sub_8220FC98`, `sub_8221A3E8`, `sub_8221A498`, `sub_82217CD0`) and `AltHud::Destroy`. Read in full. Returns VirtualFree's r3, or the list. Correction (2026-10-31): with no block it returns 0 (the block it loaded), not the list. Native in `src/alt_hud/menu_list.cpp` (`alt_hud::MenuListRelease`; `--native_alt_hud` with `--native_heap`, adapter in `src/hooks/alt_hud.cpp`, harness `alt_hud`: the layer's pages or 0): the block freed with the allocation layer's `FreePages` (as `VirtualFree`'s adapter does), and a block that isn't the layer's pages (never seen) to the original. The adapter stays for its ~50 recompiled callers. Called twice in a 40-second attract race (the front end, from `0x82221F60` and `0x821A1CB8`). |
