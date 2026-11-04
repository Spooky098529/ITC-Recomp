# Known functions: the front end's car draw

The front end's car draw (`sub_8217FE60`): once a frame on the front-end
screens that show a car on its stand (17 callers, 11 seen in play: the
garage, the shops, the car select and the like, probably; not read),
with the car's stand around it (the "garage stage", `../known-data.md`):
the car's scene model (pool 7 of the car scene `[0x826C2E44]`,
`scene.md`, picked by the stage's +8) and the stage's own state. Each
call: a camera preset waiting is applied (`GarageStage::ApplyPreset`),
the car turns by 0.5° when the caller asks, the stage's animations run
(`GarageAnims::Run`: the turn, the bonnet and the doors opening and
closing), the garage camera orbits (`GarageCamera::Update`,
`camera-modes.md`), the light kit is switched off when the stage says
so, the number plates are shown or hidden, the stage's opening values
and angles are copied into the scene model, the scene's lights are set
from the stage's (`GarageStage::SetLights`) and the car is posed on the
stand (`GarageStage::PoseCar`: its height and pitch from the stage's
dimensions, its yaw, the four wheels' matrices). Then the car scene is
updated and drawn (`CarScene::Update`, `scene.md`; `CarScene::Draw`,
`car-scene-draw.md`): with the stage's +6052 set, twice, first mirrored
upside down (the lights' y negated, the cull modes swapped through
`[0x826C5100]`, the interiors' word 0, `CarScene::Draw` given the
mirror (1, -1, 1)) with the scene's own model at scene + 4160 (the shared
assets' part model of assets 108 and 109, set a only, probably the floor
or the stand) drawn at the car's rotation (`Floor::Draw`, probably), then
upright; else once, upright. Probably the car's reflection on a shiny
floor. Beside it, the stage's command function (`GarageStage::Command`,
39 callers, 14 seen) queues the stage's animations
(`GarageAnims::Add`), steps the car's angles or picks a camera preset,
and `GarageStage::SetDrawWord` sets the scene model's +4296 (0 hides
the car, by `Car::DrawUpdate`'s row in `car-draw.md`).

Read by the B7 loop's mapping agent (2026-11-04) from two leads in
`../loops/leads.md` (the front end's car draw; `sub_820C8480`, the
model draws' lead), with no subagents: every function seen in play in
full, the ones never seen in play in full except the four door
animations (`sub_82179F60`, `sub_8217A0A8`, `sub_8217A008`,
`sub_8217A1D0`: their loads, stores and calls only). The callers (the
front-end screens) aren't read: a lead in `../loops/leads.md`. Shared,
with rows elsewhere: `CarScene::Update`, `CarScene::SetInteriorWord`
(`scene.md`); `CarScene::Draw` (`car-scene-draw.md`); `GarageCamera::Update`
(`camera-modes.md`); `ScenePools::GetB` (`vehicle-accessors.md`);
`SceneModel::PollLoad` and `SceneModel::ApplyLightKit` (`load-poll.md`);
`Model::DrawShape` (`scenery-lists.md`); `Matrix::Multiply`, `RotateX`,
`RotateY`, `RotateZ`, `Inverse` (`matrix-math.md`);
`Matrix::FromScaleRotationTranslation` (`accessories.md`); `memcpy`,
`cos`, `tan`, `atan2` (`c-runtime.md`); `MenuSound::Play`
(`menu-sounds.md`). Fields and globals: `../known-data.md` ("garage
stage", the scene model's row, `0x826C5100`, `0x82635E90`, the scene
lights' constants at `0x8296B950`-`0x8296BCDF` and `0x8265ED00`).
Nothing in `itc_recomp/src` hooks or wraps these functions (checked
2026-11-04: `sub_8217FE60` is named in comments of the native
`GarageCamera::Update` only); frame interpolation sees their work only
through `Matrix::Multiply` and the car scene's draw.

Constants (read from the image): `[0x82000A30]` 1.0, `[0x82000A34]` 0.0,
`[0x82001BE0]` -1.0, `[0x82001BE4]` 0.5, `[0x82002948]` π/180,
`[0x82009EA8]` π/360 (0.5°), `[0x820029C4]` -π, `[0x82002240]` π,
`[0x82009EB4]` π/3, `[0x82009EB8]` 640.0, `[0x82000F14]` 60.0,
`[0x82002568]` 20.0, `[0x82002244]` 360.0. The yaw steps wrap a
result below -π to π and above π to -π.

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
| `sub_8217FE60` | `GarageStage::Draw` (probably) | | r3 = a holder whose +0 is the stage (probably one of the three 4-byte blocks of `[0x826D83E0]`, the front end's car scene owners' lead), r5 = turn the car (r4 not read). A 256-byte frame. Nothing when the garage camera isn't set up (stage +112 is 0). A preset waiting (+6088 not -1): `GarageStage::ApplyPreset(stage + 5600, +6088, stage, +8)`. With r5: the yaw (+84) less `[0x82009EA8]` (0.5°), wrapped. `GarageAnims::Run(stage + 5600)`; `GarageCamera::Update(stage + 112)` (`camera-modes.md`); when +6060 is set, `GarageStage::SetLightKit(r3, 3, 0, 0, 0.0, 0.0, 0.0)` (all three groups off); `GarageStage::SyncPlates(r3)`. The scene model m (`ScenePools::GetB([0x826C2E44], 7, +8)`), its block b = m + 4288: b +8 (m +4296, the draw word) = +6056, b +640 (m +4928) = +32, b +656 to +687 (m +4944 to +4975) = +48 to +63 twice (doublewords). The frame's +80 matrix's w words (+92, +108, +124, +140) = 1.0. With +6052 set: `GarageStage::SetLights(stage + 4512, 1)`, `GarageStage::PoseCar(r3, b, frame + 80, 2)`, `[0x826C5100]` = !it, `CarScene::SetInteriorWord(scene, 0)`, `CarScene::Update`, `CarScene::Draw(scene, frame + 80)` (the mirror), b +16 (m +4304, the car's matrix) copied to frame + 144 (`memcpy`, 64 bytes), `Floor::Draw(scene, frame + 144, b +8)`, `SetLights(stage + 4512, 0)`, `PoseCar(r3, b, frame + 80, 1)`, `[0x826C5100]` = !it, `SetInteriorWord(scene, 1)`; else `SetLights(stage + 4512, 0)`, `PoseCar(..., 1)`, `CarScene::Update`. Then `CarScene::Draw(scene, 0)`. 17 callers (11 seen): `sub_821807D8`, `sub_82184B80`, `sub_82185600`, `sub_821907F8`, `sub_82196230`, `sub_821A1380`, `sub_821D9F10`, `sub_82234F08`, `sub_82235F28`, `sub_82245200`, `sub_822463D8` (not read: the front-end screens, a lead); `sub_821B6818`, `sub_8225F100`, `sub_82265E80`, `sub_82267A48`, `sub_82267ADC`, `sub_822750A0` never ran. Read in full. |
| `sub_8217D2B0` | `GarageStage::ApplyPreset` (probably) | | r3 = stage + 5600 (not read), r4 = a preset, r5 = the stage, r6 = the scene model's index (+8). When the garage camera (stage +112, c) isn't set up (c +0 is 0): stage +6088 = r4 (applied later by `GarageStage::Draw`) and nothing else. Else e = the word at `[c + 292]` + 4 × (m +48 × 290 + r4) (m the scene model, `ScenePools::GetB(scene, 7, r6)`; m +48 the car record's first word), -1 when not below `[c + 4392]`; p = `[c + 296 + 4e]` (a -1 gives p = 0, which the reads would fault on; never seen). c +100 = p +4 (the orbit's pitch), c +180 = p +8 (the distance), c +224 = p +12 (the field of view in degrees), c +108 = `[0x82009EB8]` 640.0 / tan(p +12 × `[0x82002948]`) (`tan`), c +240 to +255 = p +16 to +31, c +256 to +271 = p +32 to +47 (the orbit's translation), stage +80 to +95 = p +48 to +63 (the car's angles); +6088 = -1. A 128-byte frame. So each car has 290 preset slots (camera and car pose), picked by the screen. Callers: `GarageStage::Draw`, `GarageStage::Command` (commands 14 to 64). Read in full. |
| `sub_8217D120` | `GarageAnims::Run` (probably) | | r3 = the stage's animation list (stage + 5600: 16 entries of 28 bytes, count +448; an entry's +0 type, +4 the steps, +8 a word, +12 the value's address, +16 the scene model's index, +20 done, +24 a float). For each entry by its type: 0 `GarageAnims::Turn`, 2 `Open`, 3 `Close`, 4 `OpenDoors`, 5 `CloseDoors` (types 1, 6, 7 and others skipped); each returns 1 when the entry finished. When any did, the list is rebuilt without the done ones (+20 set), in their order: popped from the end into its 624-byte frame (`memcpy`, 28 bytes, the kept count at frame +560) and pushed back. Returns whether any finished. Only caller `GarageStage::Draw`. Read in full. |
| `sub_82179D78` | `GarageAnims::Turn` (probably) | | r3 = the list (not read), r4 = an entry: the yaw at `[entry +12]` + 52 (the stage's +84 in play's callers) less 0.5° (`[0x82009EA8]`), wrapped; done (+20 = 1, returns 1) when +8 is 0 and the yaw is then exactly 0.0, else 0. Added with +8 = 1 by `GarageStage::Command(0)`: turns until cancelled (command 1). Only caller `GarageAnims::Run`. Not seen in play. Read in full. |
| `sub_82179DF0` | `GarageAnims::Open` (probably) | | r3 = the list, r4 = an entry: max = the float at `0x82635E90` + 48 × m +48 (m the scene model, `ScenePools::GetB(scene, 7, +16)`), step = max / +4; `[+12]` (the stage's +32 from `GarageStage::Command(2)`) += step, clamped to [0.0, max]; `MenuSound::Play(15)` when the value is then at most step (the first step); at max: +20 = 1, the value = max, returns 1, else 0. Probably the bonnet opening (the value copied to the scene model's +4928 by `GarageStage::Draw`). Only caller `GarageAnims::Run`. Read in full. |
| `sub_82179EA8` | `GarageAnims::Close` (probably) | | As `GarageAnims::Open` the other way: the value less step, clamped to [0.0, max]; at 0.0 (or below): +20 = 1, the value = 0.0, `MenuSound::Play(15)`, returns 1, else 0. A 112-byte frame. Only caller `GarageAnims::Run`. Read in full. |
| `sub_8217B6B8` | `GarageAnims::OpenDoors` (probably) | | r3 = the list, r4 = an entry: by the scene model's (`ScenePools::GetB(scene, 7, +16)`) +3736 (the doors' kind, probably): 0 `sub_82179F60`, 1 `sub_8217A0A8` (r3 = the list, r4 = the entry, r5 = m +48); returns their r3, else 0. Only caller `GarageAnims::Run`. Not seen in play. Read in full. |
| `sub_8217B740` | `GarageAnims::CloseDoors` (probably) | | As `GarageAnims::OpenDoors` with `sub_8217A008` (kind 0) and `sub_8217A1D0` (kind 1). Only caller `GarageAnims::Run`. Not seen in play. Read in full. |
| `sub_82179F60` | `GarageAnims::OpenDoorsA` (probably) | | r4 = an entry: `[+12]` +20 (the stage's +52) stepped toward a per-car limit from a table by r5 (as `GarageAnims::Open`), +24 = 0.0, `MenuSound::Play`, +20 = 1 at the end. Only caller `GarageAnims::OpenDoors`. Not seen in play. Not read in detail (its loads, stores and calls). |
| `sub_8217A0A8` | `GarageAnims::OpenDoorsB` (probably) | | As `sub_82179F60` with a second value (`[+12]` +24, the stage's +56) and a word `[+12]` +64 set; two per-car limits. Only caller `GarageAnims::OpenDoors`. Not seen in play. Not read in detail. |
| `sub_8217A008` | `GarageAnims::CloseDoorsA` (probably) | | The closing of `sub_82179F60` (`[+12]` +20 down, +24 = 0.0, `MenuSound::Play` at the end). Only caller `GarageAnims::CloseDoors`. Not seen in play. Not read in detail. |
| `sub_8217A1D0` | `GarageAnims::CloseDoorsB` (probably) | | The closing of `sub_8217A0A8` (`[+12]` +20 and +24, the word +64). Only caller `GarageAnims::CloseDoors`. Not seen in play. Not read in detail. |
| `sub_8217F9E0` | `GarageAnims::Add` (probably) | | r3 = the list, r4 = a type, r5 = the value's address, r6 = the scene model's index, r7 = a word, f1 = the steps. Type 1 adds nothing: the list emptied of its type 0 entries (turns) and then of all but the earliest other entry (popped from the end into its 656-byte frame, `memcpy` 28 bytes, as `GarageAnims::Run`). Other types: nothing when an entry of that type is in the list; an entry of the opposite type (2 and 3, 4 and 5) is overwritten in place (+0, +4, +8, +12; +20 = 0; +16 and +24 kept); else a new entry at the end (+0 the type, +4 f1, +8 r7, +12 r5, +16 r6, +20 0, +24 0.0, or for type 6 (360.0 − `[r5 + 176]`) / f1, the camera's orbit angle). No bound check on the 16 entries. Only caller `GarageStage::Command`. Read in full. |
| `sub_82180108` | `GarageStage::Command` (probably) | | r3 = the holder (its +0 the stage s), r4 = a command: 0 `GarageAnims::Add(s + 5600, 0, s + 32, +8, 1, 60.0)` (turn until cancelled); 1 `Add(.., 1, 0, .., 1, 60.0)` then `Add(.., 1, s + 32, .., 0, 60.0)` (cancel the turns); 2 to 5 `Add(.., r4, s + 32, .., 0, 60.0)` (open, close, open and close the doors); 6 and 7 `Add(.., r4, s + 112, .., 0, 20.0)` (the camera: no runner in `GarageAnims::Run`, readers not looked for); 8 the list emptied (its count counted down to 0), `Add(.., 6, s + 112, .., 0, 20.0)`, `Add(.., 7, s + 32, .., 0, 20.0)`; 9 to 13 `GarageStage::StepAngles(s + 5600, r4, s + 32, +8)`; 14 to 64 `GarageStage::ApplyPreset(s + 5600, r4 − 14, s, +8)`; others nothing. 39 callers (14 seen: `sub_82185600`, `sub_821907F8`, `sub_82196230`, `sub_821A1380`, `sub_821D99A0`, `sub_821D9B60`, `sub_821D9F10`, `sub_821E3AD0`, `sub_822019D0`, `sub_82201B40`, `sub_82202928`, `sub_822338E8`, `sub_82243E80`, `sub_8224C008`; not read, the front-end screens' lead). Read in full. |
| `sub_82179C88` | `GarageStage::StepAngles` (probably) | | r4 = a command, r5 = the stage + 32 (r3, r6 not read): on the stage's +80 to +88 (r5 + 48): 9 +84 (the yaw) + 0.5°, 10 − 0.5°, 11 +88 + 0.5°, 12 − 0.5° (each wrapped), 13 +80, +88 and +84 = 0.0; others nothing. Leaf. Only caller `GarageStage::Command`. Not seen in play. Read in full. |
| `sub_82178B40` | `GarageStage::SetDrawWord` (probably) | | r3 = the holder (its +0 the stage), r4 = a word: the stage's +6056 and the scene model's (`ScenePools::GetB(scene, 7, +8)`) +4296 = r4 (0: `Car::DrawUpdate` draws nothing, `car-draw.md`). A 112-byte frame. 17 callers (6 seen: `sub_821807D8`, `sub_82184B80`, `sub_82235F28`, `sub_822463D8`, `sub_8224C008`, `sub_8224CB28`; not read). Read in full. |
| `sub_82179678` | `GarageStage::SetLightKit` (probably) | | r3 = the holder, r4 = a group (0 to 3), r5 = a word, r6 = an index, f1 to f3 = three floats: the scene model's (`ScenePools::GetB(scene, 7, [+0] + 8)`) pool 4 record (+4224, `../known-data.md`): group 0 +4228 = r5, +4232, +4236, +4240 = f1, f2, f3; 1 +4244 = r5, +4248 to +4256 = f1 to f3; 2 +4260, +4264 to +4272; 3 all three; then (groups 0 to 3) +4224 = the word at r6 in its frame's table (0, 4, 3, 5, 2, 7, 1, 6; r6 not checked). Other groups write nothing. Then `SceneModel::ApplyLightKit(m)` (`load-poll.md`). Probably the light kit's three groups (the front bumper's, side skirts' and rear bumper's lights) and a color. A 176-byte frame. Callers: `GarageStage::Draw` (3, 0, 0, 0.0 × 3), `sub_821E0FB0` (not read); `sub_821CC930` never ran. Read in full. |
| `sub_8217AFE8` | `GarageStage::SyncPlates` (probably) | | r3 = the holder: nothing while `GarageStage::IsBusy` says 1; else, with p the scene model's parts (m +5060, pool 1), when the stage's +6092 isn't p +2816 (the front bumper's plate flag, the point block's +2640): 1 `Plates::Show(p + 176)`, else `Plates::Hide(p + 176)`. Probably the number plates shown or not. A 112-byte frame. Only caller `GarageStage::Draw`. Read in full. |
| `sub_820DF190` | `Plates::Hide` (probably) | | r3 = the parts' point block (parts + 176): +2640 and +4576 = 0 (the front bumper's +2640 and the rear bumper's +1680, the groups' "own plate texture" flags read by `PartsPoints::DrawFrontBumper` and `DrawRearBumper`, `car-draw.md`; 0 gives the scene's XPR record 0's plate). Leaf. Only caller `GarageStage::SyncPlates`. Read in full. |
| `sub_820DF180` | `Plates::Show` (probably) | | As `Plates::Hide` with 1. Only caller `GarageStage::SyncPlates`. Not seen in play. Read in full. |
| `sub_82179BE0` | `GarageStage::IsBusy` (probably) | | r3 = the holder: 0 when the scene model (`ScenePools::GetB(scene, 7, [+0] + 8)`) is loaded (`SceneModel::PollLoad`, `load-poll.md`) and the stage's +4 is -1, else 1. A 96-byte frame. 15 callers (10 seen: `GarageStage::SyncPlates`, `sub_82185600`, `sub_821907F8`, `sub_82196230`, `sub_821A1380`, `sub_821D6100`, `sub_821D9F10`, `sub_82234F08`, `sub_82245200`, `sub_8224C008`). Moved here from the front end's re-applies' lead (2026-11-04). Read in full. |
| `sub_8217BA20` | `GarageStage::SetLights` (probably) | | r3 = the stage's lights (stage + 4512), r4 = mirrored. `0x8265ED00` = +1072 (16 bytes). Three lights (i 0 to 2, +32 + 96i): `0x8296B950` + 16i = +32 (y × -1.0 when mirrored), `0x8296B980` + 16i = +48, `0x8296B9B0` + 16i = +64. `0x8296BAE0` and `0x8296BB60` (128 bytes each) = 0.0. Four spots (i 0 to 3, q = +304 + 96i): `0x8296B9E0` + 16i = the position (q +0 to +15; y negated when mirrored), `0x8296BBE0` + 16i = the direction (q +0 to +8 less q +16 to +24, w 1.0) scaled by the reciprocal square root of its xyz length (`vrsqrtefp` and one Newton step; a zero length keeps 0; y negated when mirrored, after), `0x8296BAE0` + 16i = q +32, `0x8296BB60` + 16i = q +48, `0x8296BA60` + 16i = q +80 (probably the color), `0x8296BC60` + 16i = (cos(q +64 × 0.5 × π/180), cos(q +68 × 0.5 × π/180), 1 / (the first − the second), `[0x82009EB4]` π/3) (`cos` four times: the cone's inner and outer angles, probably). The scene lights' constants (`../known-data.md`). A 368-byte frame with v126, v127 saved. Only caller `GarageStage::Draw` (twice a frame with the reflection). Read in full. |
| `sub_8217CAB8` | `GarageStage::PoseCar` (probably) | | r3 = the holder (its +0 the stage s), r4 = the scene model's block b (m + 4288), r5 = out, r6 = the pass (1 or 2, not read). `ScenePools::GetB` called (its answer not used). b +732 (m +5020) = 1. b +16 (m +4304, the car's world matrix), its frame's +608 and out set to the identity (`memcpy`). s +6144 = (s +6136 + s +6140) × 0.5 (the body's height), s +6148 = atan2(s +6136 − s +6140, s +6152) (its pitch; probably the front and rear heights over the wheelbase). World = T(0, s +6144, 0) × world, then RotY(RotZ(I, s +6148), s +84) × world (`Matrix::Multiply`, `RotateZ`, `RotateY`); frame +608 = `Matrix::Inverse(world)` (not read after). The four wheels (i = 2a + side, a the axle): b +336 + 64i (m +4624, probably the wheels' rotations) = I with `RotateX` (front s +6112, rear +6116, probably the camber; negated on side 1) then `RotateY` (front s +6120, rear +6124, probably the toe; negated on side 0); b +80 + 64i (m +4368, probably their positions) = I × T(x, y, z) × RotZ(I, −s +6148), x = s +6152 × 0.5 (front) or its negation (rear), y = s +6104 (side 0) or +6108 (side 1) less s +6144, z = s +6184 (front) or +6200 (rear), negated on side 0. Then out = `Matrix::FromScaleRotationTranslation(angles 0, position 0, scale (1, -1, 1))`: the mirror. An 800-byte frame with v127 saved. Only caller `GarageStage::Draw` (twice a frame with the reflection). Read in full. |
| `sub_820C8480` | `Floor::Draw` (probably) | | r3 = the car scene, r4 = a world matrix (`GarageStage::Draw`'s copy of the car's), r5 = the car's draw word (m +4296). When r5 isn't 0 and the scene's +4 is 0 (the front end): the matrix's row 3 (+48) = (0.0, 0.0, 0.0, 1.0); then for each shape i of the part model at scene + 4160 (the shared assets' +4096, assets 108 and 109, set a only; its count +412, scene +4572): `Model::DrawShape(scene + 4160, camera pass 0's view-projection 0x826BAC00 + 144, r4, i, 0, 0, 0)` (`scenery-lists.md`). So the scene's own model, probably the floor or the stand, drawn at the car's rotation about the origin. A 144-byte frame. Only caller `GarageStage::Draw` (on the reflection's path). Read in full. |
