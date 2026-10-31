# Known functions: the race HUD object

The race's **HUD object** (`RaceHud::`, our name) and the **in-race screens' holder** around it
(`RaceScreens::`, our name; 1,040 bytes at `[0x826D84F0]`, the HUD at its +16, made by `RaceScreens::Create`
`sub_82211F40` from the race flow's `sub_8216CA28`, destroyed by
`RaceScreens::Destroy` `sub_82211D18` from `sub_8216C208`). The holder
loads the race's UI movies (a mask of 23 movies by the race kind,
`RaceScreens::MovieMask`), keeps the other in-race screens' globals
(`0x826D84D0`, `0x826D84E0`, `0x826D8500`, `0x826D8510`, `0x82814E10`,
`0x82814E18`, `0x82814E1C`; the pause menu and the rest, not mapped) and
holds the HUD at +16, a second HUD-like object at +656 (probably the
split screen's, never seen in play) and another at +1004 (both leads in
`../loops/leads.md`). The HUD keeps nine UI movies (+604 to +636, found
by id in the movie table at `RaceHud::Init`): +608 the dashboard (id 0:
the tachometer, the boost gauge, the speed, the gear, the extra gauges,
the bar at node 35, the rear-view mirror at node 19 with texture 56, the
expressway signs), +604 the racers' gauges (id 10, probably the battle's
spirit bars, one a racer, smoothed by `HudBar::Step`), +612 the start
countdown (id 11, sounds 8 and 9 at its frames 60 to 240), +616 a
warning (id 3, while the car's +6168 is set; probably wrong way), +620
and +628 (ids 2 and 1, not read), +624 the race timer (id 13), +632 and
+636 (ids 12 and 19, the time trial's splits, never seen in play). Its
state (+448, `RaceHud::SetMode` `sub_822157A8`, from the race flow's
`sub_8216CC60`) picks which racers' panel is set up, ticked
(`RaceHud::TickPanel` `sub_82215488`) and drawn (`RaceHud::RunMovie`,
`race-hud.md`).

The track zones (`track-zones.md`) are made by the HUD's set-up
(`RaceHud::SetUpSigns` `sub_822122B0`), freed by its teardown
(`sub_822141A0`) and updated by `RaceHud::UpdateSigns` (`sub_82214718`)
from the HUD's tick (`RaceHud::Tick` `sub_822153A0`), unless the race
data's word +4 (the course) is 8, 9 or 10. The first zone inside of
0-31 sets the **expressway sign** (`RaceHud::UpdateSign`
`sub_82212F00`): each zone has three values in the table at
`0x8200D838` + 24 (12 bytes a zone; -1 for none, 0 to 12 a picture, 13
a special one), one for each of the sign's three panels (nodes 23, 26
and 29 of the dashboard movie, their picture at +108, slid in and out
by the named animations 4/5, 2/3 and 0/1), and each panel's arrow is
colored (+56, one of the two colors at `0x8200D838` and `+12`) by the
course's flags (`0x8200D9D0`, 24 tables of 26 zones by
`Race::CourseLayout`, `scenery-lists.md`). The first zone inside of
32-69 sets a text (`RaceHud::UpdatePlaceName` `sub_82213510`: node 17's
text, string 1980 + the zone, cleared when none; probably the place's
name). Texture 56 isn't the zones': it is the rear-view mirror's render
target (`RaceHud::SetUpMirror` `sub_822135B0`), put back at teardown.

Read by the B7 loop's mapping agent (2026-10-31, from the lead in
`../loops/leads.md`). Shared, with rows elsewhere: `RaceHud::RunMovie`
(`sub_822141E8`), `RaceHud::RunTimerMovie` (`sub_82212908`) and the
rival logo's draw (`sub_82211A28`) in `race-hud.md`; the track zones'
`TrackZones::Create`, `Update`, `Destroy` and `IsInside` in
`track-zones.md`; the UI movie calls (`UiMovie::FindNode`, `SetText`,
`SetNodeText`, `ClearText`, `PlayNamed`, `FinishNamed`, `LoopNamed`,
`Run`, `Draw`, `SwapTexture`, `AnimationTime`, `UiMovies::Find`,
`UiMovies::Open`; `RaceHud::DrawMirror` `sub_8211BEE8`) in
`ui-movies-game.md`; `Vehicle::Get`, `SaveData::Of` and the vehicle's
floats in `vehicle-accessors.md`; `RaceTime::FromTicks` in
`ai-drivers.md`; `DataTables::Item` in `data-tables.md`;
`InputRecording::HasRoom` in `racer-controllers.md`;
`CameraPasses::RenderTarget` in `cameras.md`. Its callers are the race
flow's handlers (`8216C208`-`8216E668`: `sub_8216D450`, the HUD's
set-up by the holder's +9612 kind; the updates `sub_8216C210`,
`sub_8216C4C0`, `sub_8216C530`, `sub_8216CB98`, `sub_8216D918`,
`sub_8216DB70`, `sub_8216DD88`, `sub_8216DFD0`; the draws
`sub_8216C2A0`, `sub_8216C330`, `sub_8216CE00`, `sub_8216CF08`,
`sub_8216CFE0`, `sub_8216D0C8`), with the race flow's set-up lead
(`sub_82160AE0`); not mapped here. The menu sound (`sub_821103F0`, a
lead) is called by the countdown, the warning and the racers' gauges.

The HUD's work is in Update handlers (the tick) but for the movie runs
and the mirror's draw (`RaceHud::Draw` `sub_822128A8` and the other
`Run` calls with r4 = 1, from the race's Draw handlers, which run on
render-only passes too; the UI movies' own frame interpolation handling,
`--per_tick_ui_movie_tracks`, is inside `UiMovie::Run`); the countdown's
`RaceHud::RunCountdown` (`sub_822129A8`, from `sub_8216C330`, a Draw
handler) plays its sounds by the movie's animation time.

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
| `sub_82211F40` | `RaceScreens::Create` | | r3 = the race kind (passed on): the holder allocated (`VirtualAlloc(0, 1040, 0x1000, 4)`) and stored at `0x826D84F0`, the other in-race screens made (`sub_8220D7E8`, `sub_82210868`, `sub_8221B1F0`, `sub_82219138`, `sub_8221F390`, `sub_8221FA30`, `sub_82217D70`, `sub_8221BDD0`; not read, leads), then `RaceScreens::Init`. Only caller `sub_8216CA28` (the race flow, not read). Read in full. |
| `sub_82211DA0` | `RaceScreens::Init` | | r3 = the holder, r4 = the race kind: `sub_8211AD00` (the UI movies' start), `Strings::Load([0x826C5C34], 47)`, then for each bit i of `RaceScreens::MovieMask(r4)` the movie i opened (`UiMovies::Open(i, [0x8200D158 + 4i])`, 23 asset ids); +1012 to +1036 the other screens' globals (`[0x826D84D0]`, `[0x826D84E0]`, `[0x826D8510]`, `[0x826D8500]`, `[0x82814E18]`, `[0x82814E1C]`, `[0x82814E10]`), `RaceHud::Init(+16)`, `sub_82216A60(+1004)` (a lead), +8 = 0. Only caller `RaceScreens::Create`. Read in full. |
| `sub_82211BE8` | `RaceScreens::MovieMask` | | r3 = the holder (unused), r4 = the race kind: 0 above 10, else a mask of the movies to load by a jump table (`0x8200D1B8`): `0x406A5F`, `0x405A9F`, `0x401F`, `0x41C320`, `0x72591F`, `0x484F1F`, `0x40000`, `0x404010`, `0x7FFFFF` and `0x404E5F` (`sub_82211C1C`'s case). Leaf. Only caller `RaceScreens::Init`. Read in full. |
| `sub_82211C1C` | | | Not a function: one case of `RaceScreens::MovieMask`'s jump table (returns `0x404E5F`), which the recompiler made one. Read in full. |
| `sub_82211D18` | `RaceScreens::Destroy` | | No arguments: the other in-race screens destroyed (`sub_8220C4C8`, `sub_8220FC98`, `sub_8221A3E8`, `sub_82219170`, `sub_8221F3C8`, `sub_8221FA68`, `sub_82217CD0`, `sub_8221BE30`; not read), `RaceScreens::~RaceScreens([0x826D84F0])`, then the holder freed (`VirtualFree(.., 0, 0x8000)`) and `0x826D84F0` cleared; when it was 0, `OutputDebugStringA(0x8200D1C4)` instead. Only caller `sub_8216C208` (the race flow). Read in full. |
| `sub_82211B98` | `RaceScreens::~RaceScreens` | | r3 = the holder: the HUD's teardown (`sub_822141A0` on +16), `sub_82216BA8(+1004)` (a lead), `Strings::Release([0x826C5C34], 47)`, then `sub_8211E370` (the UI movies' end, `ui-movies-game.md`). Only caller `RaceScreens::Destroy`. Read in full. |
| `sub_82211D08` | | | r3 = the holder: tail call to `sub_8220FB10([+1012])` (another in-race screen's, not read). From `sub_8216CF08`, `sub_8216CFE0`, `sub_8216D0C8` (the race flow's draws). Read in full. |
| `sub_82211D10` | | | r3 = the holder: tail call to `sub_8221BB48([+1020])` (another in-race screen's, not read). From `sub_8216C6D8`. Read in full. |
| `sub_82211D00` | | | r3 = the holder: `RaceHud::StopCountdown(+16)` (tail call). From `sub_8216DB70` (the pause screen's update). Not seen in play. |
| `sub_82211CE0` | | | No arguments: `[0x826D84FC]` + 36 = 1. Leaf. From `sub_821511B8` (not read). Not seen in play. |
| `sub_82211CF8` | | | A jump to `sub_82292710` (not read). From `sub_821512F0`. Not seen in play. |
| `sub_82211E90` | | | No arguments: returns 1 unless `[0x826D84FC]` + 32 is 1, `[0x82814E10]` + 340 is 0 (then its +344 = 1) or `sub_8221ABE0` returns 0 (then `[0x826D8514]` + 20 = 1). From `sub_821511B8`. Not read in detail. Not seen in play. |
| `sub_82211F18` | | | No arguments: `[0x826D8514]` + 20 = 0 and `[0x82814E10]` + 352 = 1. Leaf. From `sub_82151388`. Not seen in play. |
| `sub_82215260` | `RaceHud::Init` | | r3 = the HUD: its nine movies found in the movie table (`UiMovies::Find(0x826C5CCC, id)`): +604 id 10, +608 id 0, +612 id 11, +616 id 3, +620 id 2, +624 id 13, +628 id 1, +632 id 12, +636 id 19; +588 = 0. Only caller `RaceScreens::Init`. Read in full. |
| `sub_82212690` | `RaceHud::SetPlayer` | | r3 = the HUD, r4 = a player: +16 = r4, +440 = `Vehicle::Get(r4)`. From `sub_8216D450` (r4 by the race data's racer flag, kind 6; else 0). Read in full. |
| `sub_822121C0` | `RaceHud::Reset` | | r3 = the HUD: the dashboard's node 1 children's alpha (+68) and node 5's set to 0.0, +492 = 0, +460 = -1, node 19 (the mirror) hidden (+80 = 0). From `sub_8216D450`. Read in full. |
| `sub_822122A0` | | | r3 = the HUD: +468 = 0 (the warning's shown flag). Leaf. From `sub_8216D450`. Read in full. |
| `sub_82213C90` | `RaceHud::SetUpGauges` | | r3 = the HUD: by the player's racer record (race data + 48 + 3,936 × +16): its +60 (an index) and +1704 (0-2 gives 0, 3-6 gives 1, 7 and up 2) pick a tachometer curve (one of three 112-byte curves from `0x8200D1F8`, by the table at `0x8200D3B8`, 16 bytes an index) copied to +20 (`memcpy`) and the dial's face (node (0, 8) +108); the boost curve (`0x8200D348`) copied to +132; both evaluated at 0.0 (`HudCurve::Eval`). A 480-byte frame (the three curves copied to it first). From `sub_8216D450`. Read in full; what +60 and +1704 are isn't known. |
| `sub_82212668` | | | r3 = the HUD: +404 and +408 = the car's float +2420 (`[+440]`), +412 = 0.0 (the bar at node 35's range). Leaf. From `sub_8216D450`. Read in full. |
| `sub_82213DB0` | `RaceHud::SetUpExtraGauges` | | r3 = the HUD: from the car's build (`[[+440] + 5712]`) +1832, five 12-byte entries: type 1 sets +476 (also when the build's +12 is 10), types 2, 3 and 4 a flag each; the three slots +480 to +488 (from the last) get the first flag set of the three (0, 1 or 2, the flag cleared) or -1; each slot's nodes shown or hidden (`RaceHud::ShowGauge`); node (0, 1) shown by +8 and +476. Probably the extra gauges a build has fitted. A 144-byte frame. From `sub_8216D450`. Read in full. |
| `sub_82212D08` | `RaceHud::ShowGauge` | | r3 = the HUD, r4 = a slot (0-2), r5 = shown: the dashboard's nodes (0, 12 + r4) and (0, 9 + r4) +80 = r5. Only caller `RaceHud::SetUpExtraGauges`. Read in full. |
| `sub_822135B0` | `RaceHud::SetUpMirror` | | r3 = the HUD: when +588 (the texture swapped out) isn't 0, put back (`UiMovie::SwapTexture(+608, 56, +588)`); then +592 = `CameraPasses::RenderTarget(11)`, its size (`Texture::GetLevelDesc` level 0) at +596 and +600, and +592's record swapped in as the dashboard's texture 56, the old one at +588. Probably the rear-view mirror's picture. A 144-byte frame. From `sub_8216D450`. Read in full. |
| `sub_82212768` | `RaceHud::ApplyOptions` | | r3 = the HUD: four words of player 0's save data (`SaveData::Of(0)` + 0x1F85C; probably the HUD options) copied to +0 to +12, then shown by them: node 4 by +12 (when +472), node 3 by +0, nodes (0, 3), (0, 4) by +8, (0, 1) by +8 and +476, node 1 by +4. From `sub_8216D450`, `sub_8216C658`, `sub_8216DB70`. Read in full. |
| `sub_822122B0` | `RaceHud::SetUpSigns` | | r3 = the HUD: +420 = the course's sign flags (`0x8200D9D0` + 312 × k, by `Race::CourseLayout`'s 0-23 through a table of 24 built in its 320-byte frame; -1 gives the third's), `TrackZones::Create`, +416 and +428 = -1 (no zone), +424 = 1 (signs on). From `sub_8216D450` (last). Read in full. |
| `sub_822141A0` | `RaceHud::TearDown` | | r3 = the HUD: `TrackZones::Destroy`, then when +588 isn't 0 the mirror's texture put back (`UiMovie::SwapTexture(+608, 56, +588)`). Only caller `RaceScreens::~RaceScreens`. Read in full. |
| `sub_822153A0` | `RaceHud::Tick` | | r3 = the HUD, r4 passed to `RaceHud::UpdateSigns`: by the car (`[+440]`): the speed (`Vehicle::GetSpeedKmh` × the double at `0x8200A510`, truncated; `RaceHud::SetSpeed`), the extra gauges (`RaceHud::UpdateExtraGauges` with f1 = the car's float +6260, r5 = +6256 and r6 = +6252 truncated from floats), the gear (+2252, `RaceHud::SetGear`), the tachometer (`sub_820C0140`'s float, `RaceHud::SetTach`), the bar (+2420, `RaceHud::SetBar`), the boost (+2944, `RaceHud::SetBoost`), node 13 +80 = the build's (`[+5712]`) +3892, then the signs (`RaceHud::UpdateSigns`). A 128-byte frame. From the race flow's updates `sub_8216C210`, `sub_8216C4C0`, `sub_8216C530`, `sub_8216CB98`, `sub_8216D918`, `sub_8216DB70`, `sub_8216DD88`, `sub_8216DFD0`; probably once a tick. Read in full. |
| `sub_82214260` | `RaceHud::SetSpeed` | | r3 = the HUD, r4 = the speed: when 999 or less, the dashboard's node 9 children (digits, last first) get +108 = digit + 1, leading zeros 0 (hidden; by `PowInt(10.0, n)`). A 160-byte frame. Only caller `RaceHud::Tick`. Read in full. |
| `sub_82215160` | `RaceHud::UpdateExtraGauges` | | r3 = the HUD, f1, r5, r6: for each slot (+480 to +488): 0, `RaceHud::GaugeFloat(f1)`; 1, `RaceHud::GaugeInt119(r5)`; 2, `RaceHud::GaugeInt149(r6)`; -1 nothing. Only caller `RaceHud::Tick`. Read in full. |
| `sub_82214330` | `RaceHud::GaugeFloat` | | r3 = the HUD, f1 = a value, r5 = the slot: from 0.0 to under the float at `0x82001FD8`, the slot's digits (node (0, 12 + r5) children: one decimal, a point (frame 11), the integer part) and node (0, 9 + r5) +108 = 0. A 208-byte frame. Only caller `RaceHud::UpdateExtraGauges`. Not read in detail. Not seen in play. |
| `sub_822144B0` | `RaceHud::GaugeInt119` | | r3 = the HUD, r4 = a value, r5 = the slot: when 119 or less, its digits as `RaceHud::SetSpeed`'s on node (0, 12 + r5), and node (0, 9 + r5) +108 = 1. Only caller `RaceHud::UpdateExtraGauges`. Not read in detail. Not seen in play. |
| `sub_822145A0` | `RaceHud::GaugeInt149` | | r3 = the HUD, r4 = a value, r5 = the slot: when 149 or less, its digits as `RaceHud::SetSpeed`'s on node (0, 12 + r5), and node (0, 9 + r5) shown with +108 = 2. Only caller `RaceHud::UpdateExtraGauges`. Not read in detail. Not seen in play. |
| `sub_82212DB0` | `RaceHud::SetGear` | | r3 = the HUD, r4 = the gear: node 11 +108 = 7 for 6, gear - 1 for 1-5, 5 for 0, 6 below 0 (probably reverse). Only caller `RaceHud::Tick`. Read in full. |
| `sub_82212D58` | `RaceHud::SetTach` | | r3 = the HUD, f1 = the value: `HudCurve::Eval(+20, f1)`, node (0, 7) +36 (the needle's rotation) = its output (+120) plus the float at `0x8200F720`. Only caller `RaceHud::Tick`. Read in full. |
| `sub_82212E18` | `RaceHud::SetBoost` | | r3 = the HUD, f1 = the value: `HudCurve::Eval(+132, f1)`, node (0, 5) +36 = its output (+232). Only caller `RaceHud::Tick`. Read in full; that it's the boost is a guess (car +2944). |
| `sub_82214698` | `RaceHud::SetBar` | | r3 = the HUD, f1 = the value: +404 = f1; node 35 shown when f1 is above +412, then its +28 (x scale) = +404 / (+408 - +412). Only caller `RaceHud::Tick`. Read in full; what the car's +2420 is isn't known. |
| `sub_82212118` | `HudCurve::Eval` | | r3 = a curve (up to 11 points of (x, y) floats at +0, +88 the largest x, +92 the smallest, +108 the count), f1 = x: +96 = x clamped to [+92, +88]; +100 = y linearly between the two points around x (0.0 when none). Leaf. From `RaceHud::SetTach`, `SetBoost`, `SetUpGauges` and the +656 object's (`sub_822158E8`, `sub_82215F60`) and +1004 object's (`sub_82216C40`, `sub_822173E8`, `sub_82217538`) gauges. Read in full. |
| `sub_82213C40` | `PowInt` | | f1 = x, r4 = n: x to the n (doubles, by squaring; 1 / x^-n below 0). Leaf. From `RaceHud::SetSpeed`, the extra gauges, `RaceHud::SetDistance` and the +656 and +1004 objects' digits (`sub_82215E58`, `sub_82216578`, `sub_82216E00`, `sub_82216EE8`, `sub_82216FD0`, `sub_82217168`, `sub_82217270`). Read in full. |
| `sub_82214718` | `RaceHud::UpdateSigns` | | r3 = the HUD, r4 passed on: unless the race data's (`[0x826C5B2C]`) +4 is 8, 9 or 10, `TrackZones::Update([0x828161EC])`, `RaceHud::UpdateSign(r3, r4)`, `RaceHud::UpdatePlaceName`. Only caller `RaceHud::Tick`. Read in full. |
| `sub_82212F00` | `RaceHud::UpdateSign` | | r3 = the HUD, r4 = a flag (from the race flow's update; not read): signs on (r29) when the car's +6168 is 0 and +424 isn't; while on and the zone (+416) is still inside (`TrackZones::IsInside`), only its panels whose value is 13 recolored (nodes 24/22 for the third, 30/28 for the first: color 1 when r4 or the race mode (`+0xA3AF4`) is 6). Else the first zone of 0-31 inside (none when off) becomes +416 when it changed, and for each panel whose value changed (the third: node 23, animations 5 and 4; the second: node 26, 3 and 2; the first: node 29, 1 and 0) its picture (+108) set and its animations finished and played (in only, out only, or out and in); a panel coming in gets its arrow color (+56 of node 24, 27 or 30 from `0x8200D838` + 0 or + 12, +80 of node 22, 25 or 28: 0 or 1) by the course flags (+420, the zone's entry, for zones under 26; a value of 13 in the first or third panel by r4 and the race mode instead). For no zone, the flags are read at +420 - 12 (an entry before the course's table; kept as the original does). A 224-byte frame. Only caller `RaceHud::UpdateSigns`. Read in full; that the panels are an expressway sign's route pictures is a guess. |
| `sub_82213510` | `RaceHud::UpdatePlaceName` | | r3 = the HUD: when the zone at +428 is no longer inside, the first of 32-69 inside (or -1): when it differs, node 17's text set (`UiMovie::SetText(+608, -1, 17, 1980 + zone)`; r7 to r10 unset) or cleared (`UiMovie::ClearText`), and +428 = it. A 128-byte frame. Only caller `RaceHud::UpdateSigns`. Read in full; probably the place's name. |
| `sub_82212C50` | `RaceHud::UpdateWarning` | | r3 = the HUD: when the car's +6168 is 0 and +468 is set, the warning movie (+616) finished (`UiMovie::Finish`) and +468 = 0; when +6168 is set and +468 isn't, finished, played (`UiMovie::Play`) and looped (`UiMovie::Loop`), +468 = 1. From `RaceHud::Tick`'s callers (seven) and `sub_8216DB70`. Read in full; probably wrong way. |
| `sub_82212A48` | `RaceHud::RunWarning` | | r3 = the HUD: while +468, sound 10 (`sub_821103F0`) unless the music player's (`[0x826C5C80]`) slot 14 says it's playing (r4 = 0, r5 = 10 in its frame), then the warning movie run (`UiMovie::Run(+616, 0)`). From the race flow's draws `sub_8216C2A0`, `sub_8216C330`, `sub_8216C5B8`, `sub_8216CE00`. Read in full. |
| `sub_82212AC8` | | | r3 = the HUD: while +468, the warning movie drawn only (`UiMovie::Run(+616, 1)`, tail call). From `sub_8216CF08`, `sub_8216CFE0`, `sub_8216D0C8`. Read in full. |
| `sub_822128A8` | `RaceHud::Draw` | | r3 = the HUD: the dashboard run (`UiMovie::Run(+608, 0)`), then when +472 and +12 the mirror drawn (`RaceHud::DrawMirror` on node 19). From ten of the race flow's draws (`sub_8216C2A0` to `sub_8216D6D0`). Read in full. |
| `sub_82212B68` | `RaceHud::UpdateView` | | r3 = the HUD: +432 = viewport 0's (`[[0x826C2CE8]]`) +1860 (probably the camera view); with +12 set: views 0 shows node 0, 1 shows node 4 and +472, 2 shows both and +472; without, +472 = 0 and node 0 shown for views 0 and 2. From `sub_8216C210`, `sub_8216C4C0`, `sub_8216C530`, `sub_8216CB98`, `sub_8216D918`. Read in full; which views are which isn't known. |
| `sub_82212E60` | `RaceHud::UpdateRecordingFull` | | r3 = the HUD, r4 = `InputRecording::HasRoom(0)`: 0 the first time (+492 = 1), named animation 6 finished and played on the dashboard; 1, +492 = 0 and node 5's alpha 0.0. Only caller `RaceHud::TickPanel`. Read in full; probably a notice that the replay stopped recording. |
| `sub_822157A8` | `RaceHud::SetMode` | | r3 = the HUD, r4 = the mode: +448 = r4, then the mode's set-up as a tail call: 0 `RaceHud::SetUpRacers`, 1 `SetUpTimer`, 2 `sub_82214C88`, 3 `sub_822155C0`, 4 `sub_822125B8`; nothing above. Only caller `sub_8216CC60` (by the holder's kind and the stage's type). Read in full. |
| `sub_82215488` | `RaceHud::TickPanel` | | r3 = the HUD: by +448: 0 `RaceHud::TickRacers`, 1 `TickTimer`, 2 `sub_82214E78`, 3 `sub_822150A8`, 4 `sub_822150F8`; then `RaceHud::UpdateRecordingFull(InputRecording::HasRoom(0))`. From `sub_8216CB98`, `sub_8216D918`, `sub_8216DB70`. Read in full. |
| `sub_82213F20` | `RaceHud::SetUpRacers` | | r3 = the HUD (mode 0, the racers' gauges on +604): the item popup hidden (`RaceHud::ShowItem(-1)`), +452 = the racers' count (`[+444]` +0), node 0's children hidden, the last racer's item picture (record +8 through `DataTables::Item`'s byte +34), nodes 3 and 4 by the count, each racer's name (player 0's from the save data, `sub_8211D930`; the others string 4650 + record +8, `UiMovie::SetText` with r7 to r10 unset) and bar (+244 + 40i: max and target the record's +788 as a float, min 0.0, steps two fractions of the max), animations 0-3 finished, +456 = 0. Records are 972 bytes from `[+444]`. A 176-byte frame. Only caller `RaceHud::SetMode`. Read in full; probably the battle's spirit bars. |
| `sub_822151F8` | `RaceHud::TickRacers` | | r3 = the HUD: +456 = 0, then `RaceHud::UpdateRacerBar(i, i)` for each racer (+452). Only caller `RaceHud::TickPanel`. Read in full. |
| `sub_82214790` | `RaceHud::UpdateRacerBar` | | r3 = the HUD, r4 = a bar, r5 = a racer: the bar (+244 + 40 × r4) stepped (`HudBar::Step`) toward the racer's +788; on its first contact (`RaceHud::Touching`, +36 unset) its trail (+8) set to the value; its four nodes (group r4 + 1's children 1-4) shown by the value against the minimum and a fraction (the double at `0x8200F738`) of the maximum, animation r4 played and looped once low (+32, +456 = 1; for bar 0 sound 7 unless the music player's slot 14 says playing), their x scales (+28) the value's and the trail's fractions, node 2's x (+12) placed after them. A 176-byte frame. From `RaceHud::TickRacers`, `sub_822150A8`. Read in full. |
| `sub_82211FB8` | `HudBar::Step` | | r3 = a bar (+0 value, +4 its step, +8 the trail, +12 its step, +16 min, +20 max, +24 the target, +28 and +36 flags), r4 = the target (an int): +24 = it as a float; the value and trail move toward it by at most their steps, clamped to [+16, +20]; +28 = 1 when the value is at or above the trail, the target and the direction is up (else 0); +36 = 0 when they meet. Leaf. From `RaceHud::UpdateRacerBar`, `sub_822161D0` (the +656 object). Read in full. |
| `sub_82212100` | `HudBar::Snap` | | r3 = a bar: the trail (+8) = the value, +36 = 1. Leaf. Only caller `sub_822161D0` (the +656 object, never seen in play). Not seen in play. |
| `sub_82215820` | `RaceHud::Touching` | | r3 unused, r4 = a player: 1 when the player's vehicle (`Vehicle::Get`) has a collision object (+340) with a contact of mask `0x07000000` (`CollisionObject::FirstContact`), else 0. A 96-byte frame. From `RaceHud::UpdateRacerBar`, `sub_822161D0`. Read in full; probably touching a wall. |
| `sub_82214238` | `RaceHud::UpdateItem` | | r3 = the HUD: when the race progress's (`[0x826D82E8]`) +7240 differs from +460, +460 = it and `RaceHud::ShowItem` (tail call). Only caller `sub_8216C210`. Read in full. |
| `sub_82213638` | `RaceHud::ShowItem` | | r3 = the HUD, r4 = an item id or -1: every `PRM`, `PIC` and `TXT` node of the racers' movie (+604, its live scene's list to the `TRM` tag) shown for -1 and hidden otherwise; for an item, nodes 19, 5 and (5, 0) shown, node 19's text the item's name (`UiMovie::SetNodeText`, string 4650 + id; r6 and up unset) and (5, 0)'s picture from `DataTables::Item(id)`'s byte +34 (48 and up: id - 262). A 128-byte frame. From `RaceHud::SetUpRacers`, `RaceHud::UpdateItem`, `sub_822155C0`. Read in full; probably the item won. |
| `sub_82212B08` | `RaceHud::RunItem` | | r3 = the HUD: when +460 isn't -1, the racers' movie run (`UiMovie::Run(+604, 0)`) and `sub_82106540(200, 500, 0, 0x8200F710, +460)` (empty; a debug print). Only caller `sub_8216C2A0`. Read in full. |
| `sub_822123C8` | `RaceHud::SetUpTimer` | | r3 = the HUD (mode 1, the timer movie +624): +464 = `[+444]` +12; when 0 or more, animations 0-4 finished, node 0 shown, animation +464 played and looped; else node 0 hidden; node 3's children hidden, the last racer's item picture shown (as `RaceHud::SetUpRacers`), the racers' names (nodes 27 to 29: string 4650 + record +8, or cleared) and player 0's (node 26, `sub_8211D930`). A 144-byte frame. Only caller `RaceHud::SetMode`. Read in full. |
| `sub_82214D78` | `RaceHud::TickTimer` | | r3 = the HUD: `RaceHud::SetTime(RaceTime::FromTicks([+444] +3932))`, `RaceHud::SetDistance` with (`[0x826D82E8]` +7156 - `[+444]` +828) times two constants, then the animation by `[+444]` +12 (node 0 shown, animations 0-4 finished, played and looped when it changed from +464; hidden below 0). A 128-byte frame. Only caller `RaceHud::TickPanel`. Read in full. |
| `sub_82213750` | `RaceHud::SetTime` | | r3 = the HUD, r4 = a packed time's address: clamped to 59:59.999, the timer movie's nodes 15/14 (minutes), 17/16 (seconds) and 20/19/18 (milliseconds) +108 = digit + 1. A 144-byte frame. Only caller `RaceHud::TickTimer`. Read in full. |
| `sub_82214A50` | `RaceHud::SetDistance` | | r3 = the HUD, f1 = a value: clamped to [0.0, the float at `0x8200F740`], its tenths rounded up into the timer movie's nodes 11 (the decimal), 10 and 9 (+108 = digit + 1, leading zeros hidden). A 160-byte frame. Only caller `RaceHud::TickTimer`. Read in full; what it shows isn't known. |
| `sub_822129A8` | `RaceHud::RunCountdown` | | r3 = the HUD: +436 = `UiMovie::Run(+612, 0)`; by the movie's animation time (`UiMovie::AnimationTime(+612, -1, 0)`, truncated): 60, 120 or 180 sound 8, 240 sound 9 (`sub_821103F0`). A 112-byte frame. Only caller `sub_8216C330` (a draw). Read in full. |
| `sub_82212270` | `RaceHud::StartCountdown` | | r3 = the HUD: +436 = 1, `UiMovie::Play(+612, -1, 0, ...)` (tail call). Only caller `sub_8216CC60`. Read in full. |
| `sub_82212888` | `RaceHud::StopCountdown` | | r3 = the HUD: +436 = 0, `UiMovie::Finish(+612, -1, 0)` (tail call). From `sub_8216C3E8`, `sub_8216E668`, `sub_82211D00`. Not seen in play. |
| `sub_82212A38` | | | r3 = the HUD: `UiMovie::Run(+612, 1)` (tail call; the countdown drawn only). Only caller `sub_8216D0C8`. Not seen in play. |
| `sub_822126C8` | | | r3 = the HUD: `UiMovie::PlayNamed(+620, 0, ...)` and `UiMovie::LoopNamed(+620, 0)`. From `sub_8216C3E8`, `sub_8216C470`. Not seen in play. |
| `sub_82212868` | | | r3 = the HUD: `UiMovie::FinishNamed(+620, 0)` (tail call). Only caller `sub_8216C500`. Not seen in play. |
| `sub_82212AE8` | | | r3 = the HUD: `UiMovie::Run(+620, 0)` (tail call). Only caller `sub_8216C480`. Not seen in play. |
| `sub_82212718` | | | r3 = the HUD: `UiMovie::PlayNamed(+628, 0, ...)` and `UiMovie::LoopNamed(+628, 0)`. Only caller `sub_8216C510`. Read in full. |
| `sub_82212878` | | | r3 = the HUD: `UiMovie::FinishNamed(+628, 0)` (tail call). Only caller `sub_8216C520`. Read in full. |
| `sub_82212AF8` | | | r3 = the HUD: `UiMovie::Run(+628, 0)` (tail call). Only caller `sub_8216C5B8`. Read in full. |
| `sub_82212958` | | | `RaceHud::RunMovie`'s state 3 (`race-hud.md`): the racers' movie (+604) run when +456, else drawn (`UiMovie::Draw`), then +636 drawn. Only caller `sub_822141E8` (a tail call). Read in full. Not seen in play. |
| `sub_82214C88` | | | r3 = the HUD: mode 2's set-up (the splits' movie +632): the movie finished (`sub_8211DFF8`), the best splits copied or reset from the race data (`memcpy` or `sub_820FF5A0`, `sub_820FF628`), the time shown (`sub_82213860`), the nodes' alpha 0.0, +572 and +584 set. Only caller `RaceHud::SetMode`. Not read in detail. Not seen in play. |
| `sub_82214E78` | | | r3 = the HUD: mode 2's tick: the time (`sub_82213860`, node 33), the splits (`sub_82214BA8`), on a new split a 972-byte record copied, animations 0-3 played, the split's time from `sub_820FF790`, `sub_820FF458` and `sub_82162230`. Only caller `RaceHud::TickPanel`. Not read in detail. Not seen in play. |
| `sub_822150A8` | | | r3 = the HUD: mode 3's tick: `RaceHud::UpdateRacerBar(0, +16)` and `(1, +16 xor 1)` (two players). Only caller `RaceHud::TickPanel`. Read in full. Not seen in play. |
| `sub_822150F8` | | | r3 = the HUD: mode 4's tick: the time (`RaceTime::FromTicks([+444] +3932)`, `sub_82213860` from node 33) and the splits (`sub_82214BA8` with `[+444]` + 76 × its +836 + 24). Only caller `RaceHud::TickPanel`. Read in full. Not seen in play. |
| `sub_822125B8` | | | r3 = the HUD: mode 4's set-up on +632: nodes 3, 21 hidden, node 0's alpha and node 1's children's 0.0. Only caller `RaceHud::SetMode`. Read in full. Not seen in play. |
| `sub_822155C0` | | | r3 = the HUD: mode 3's set-up on +604: `RaceHud::ShowItem(-1)`, the two players' nodes, their names (`sub_82213A70`, `sub_82215520`, `sub_8211C2C8`) and bars (`sub_82213AE0`), +456 set. Only caller `RaceHud::SetMode`. Not read in detail. Not seen in play. |
| `sub_82213860` | | | r3 = the HUD, r4 = a packed time's address, r5 = the first node: as `RaceHud::SetTime` on the splits' movie (+632) from node r5. From `sub_82214C88`, `sub_82214E78`, `sub_822150F8`. Not read in detail. Not seen in play. |
| `sub_82213968` | | | As `sub_82213860` for a split's row. Only caller `sub_82214BA8`. Not read in detail. Not seen in play. |
| `sub_82214BA8` | | | r3 = the HUD, r4 = splits: four rows of times (`sub_82213968`, from `sub_820FF790` and `sub_820FF458`) or their seven digits hidden. Probably the time trial's split list. From `sub_82214E78`, `sub_822150F8`. Not read in detail. Not seen in play. |
| `sub_82213A70` | | | A player's name into a buffer (`sub_8209F658`, `sub_82296720`). Only caller `sub_822155C0`. Not read in detail. Not seen in play. |
| `sub_82215520` | | | A name built through `sub_82296720` from the race data. Only caller `sub_822155C0`. Not read in detail. Not seen in play. |
| `sub_82213AE0` | | | Mode 3's bars set up (`sub_820FFE58`, nodes 0 and 1 of +604 and their children). Only caller `sub_822155C0`. Not read in detail. Not seen in play. |
| `sub_82212AE0` | | | Empty (`blr`). No direct caller (probably a vtable slot). Not seen in play. |
| `sub_82214258` | | | Empty (`blr`). No direct caller (probably a vtable slot). Not seen in play. |
