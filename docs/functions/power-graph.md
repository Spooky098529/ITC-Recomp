# Known functions: the power graph

The front end's power and torque graph: two curves drawn as line
segments over a car's engine, rpm across and output up, in the tuning
screen (the screen whose update is `sub_821E6C50`, a lead) and in a
screen that lists cars (`sub_8224BDC0`, a lead). `PowerGraph::Draw`
builds two sets of 32 single sprites on its stack (`sprites.md`'s
144-byte layout, reset by `Sprite::ResetStyle2`, `race-overlays.md`),
one segment each: corners at +80 and +96 (x, y, z), their colors at
+112 and +128 (red, green, blue, alpha words), the identity matrix at
+0. The curve's values come from a car data's torque curve (+1292, 32
points 400 rpm apart, scaled by +1420) as `PowerCurve::Build` reads it
(`vehicle.md`), converted to kgf·m and PS. Each segment is recorded as
a two-vertex line on the command list by `Line::RecordFlat`, the line
sibling of `Triangle::RecordFlat` (`minimap.md`) and
`ColorRect::RecordFlat` (`mode-tick.md`). The fields and globals are in
`../known-data.md` ("tuning screen", `0x826D83D8`, `0x82815EC0`).

Shared, with rows elsewhere: `Sprite::ResetStyle2` (`sub_82123A00`,
`race-overlays.md`); `` `vector destructor iterator' ``
(`sub_820900C0`, `c-runtime.md`) with the empty destructor
`sub_82106540` (`audio.md`); `UiMovies::Find` and
`UiMovie::AnimationTime` (`sub_8211B3F0`, `sub_8211BA00`,
`ui-movies-game.md`); `CommandList::OpenChunk`, `AllocateSlots`,
`AllocateVertices`, `CloseChunk`, `ChunkSlots` (`sub_8258C9D0`,
`sub_8258CAA0`, `sub_8258CB08`, `sub_8258CA50`, `sub_8258CA88`,
`command-list.md`); `Primitive::RecordTransform` (`sub_8217AEA8`,
`minimap.md`); `CarData::CarData` (`sub_820AEF80`, `car-physics.md`);
`Car::SetModelAndClass` (`sub_821104D8`, `career.md`). Reached through
vtables, without rows: the app's interface `[0x826BABA0]` (slot 8
`sub_820944D8`, its answer's slot 1 the command list and slot 2 the
render queue, whose slot 4 is `RenderQueue::Add`, `device-shaders.md`).
In the car objects' makers lead (`../loops/leads.md`), not here:
`sub_82179A60` (sets up a racer and its vehicle from a car into a car
data) and `sub_82110448` (clears a car object).

Making it native: `PowerGraph::Draw` with `Line::RecordFlat` is one
ready group (everything under it is ours but the render manager's vtable
calls, guest calls as in `Triangle::RecordFlat`); then
`TuningScreen::DrawPowerGraph`. `CarList::DrawEntryPowerGraph` waits on
`sub_82179A60` and `sub_82110448` (the car makers' lead).
`sub_82250700` and `sub_821B8F60` never ran in play.

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
| `sub_821DF350` | `TuningScreen::DrawPowerGraph` (probably) | | r3 = the tuning screen (the object `sub_821E6C50` updates; fields in `../known-data.md`'s "tuning screen"). A 112-byte frame. Finds movie 2 (`UiMovies::Find` on `0x826C5CCC`, the key 2 at its frame's +80). With the screen's state (+9652) 5: `UiMovie::AnimationTime`(movie, -1, 5); above 22.0 (`0x8200829C`), or (asked again) equal to -1.0 (`0x82001BE0`: animation 5 in neither list), it draws when +9656 is 0 and the byte +9660 is 2 or less (unsigned); any other time, nothing. With state 6: draws when +9656 is 0 and the byte +9660 is 2. Any other state: nothing. The draw: `PowerGraph::Draw`(r3 = the word at `[0x826D83E0]` + 0, the front end's car holder's first block, which the graph never reads; f1 = 1042.0 `0x8200AD98`, f2 = 520.0 `0x8200AD9C`, r6 = the screen's car data at +18912). Returns nothing. Only caller the tuning screen's update `sub_821E6C50` (reached through a vtable; also through the recompiler's switch-case pieces of it, `sub_821E6CD4` and `sub_821E6F90`), every update, after `sub_822036E8` and before `sub_821D8978`. Read in full. |
| `sub_8217C708` | `PowerGraph::Draw` (probably) | | r3 = passed to `Line::RecordFlat`, which doesn't read it (the callers pass `[0x826D83E0]`'s +0 word); f1 = the graph's left x, f2 = its baseline y (screen units, probably pixels: 1042, 520 or 500, or 1036, 508 by caller); r6 = a car data (`CarData::`, `car-physics.md`). r4 and r5 aren't read. Returns nothing. A 9,648-byte frame (stack probes at -4096 and -8192), f30 and f31 saved. Two sets of 32 stack sprites (144 bytes each), each reset with `Sprite::ResetStyle2`, 0 to 31: the torque set at r1 + 4976 first, then the power set at r1 + 368. For each segment i (0 to 31), both sets: +80 x = f1 + i × 6.4 and +96 x = f1 + (i + 1) × 6.4 (`0x82009ECC`; i converted through the frame's +80 and +88), +84 and +100 y = f2, +88 and +104 z = 0.0 (`0x82000A34`). Then the 32 points i (into the frame's +112 and +240): torque = car data +1292 + 4i × car data +1420 × 0.10197162 (`0x82009EC0`, 1/9.80665: N·m to kgf·m); power = the rpm (`fctiwz` of (i + 1) × 400.0, `0x82009EC4`, back to a float) × torque × 0.0013962635 (`0x82009EC8`, 1/716.2: PS), 0.0 when below 0.0 (the torque isn't clamped). For i = 0 to 25 only (26 points): the torque segment i's +100 and segment i + 1's +84 = f2 − torque × 3.2 (`0x8200822C`), the power set's = f2 − power × 0.32 (`0x82009EBC`); both corners' colors (+112 to +124 and +128 to +140): torque 198, 66, 14, 255 (orange), power 44, 242, 242, 255 (cyan). The count n = `fctiwz`(car data +1280 × 0.0025 + 1.0) (`0x82002CE8`, `0x82000A30`, one fused multiply-add; +1280 probably the rev limit, the curve's +1044 in `PowerCurve::FromEngine`); when n > 1, segments 1 to n − 1 are recorded, each torque then power: `Line::RecordFlat`(r3, the sprite, camera pass 0 `0x826BAC00`, the sort key 683). Segment 0 (from the baseline to the first point) is never drawn; segments past 26 run back to the baseline; nothing bounds n by 32. Then the sprites' destructors, inlined: the power set, then the torque set, sprites 31 to 0, each `vector destructor iterator`(sprite +112, 16, 2, `sub_82106540`) then (sprite +80, 16, 2, `sub_82106540`). Callers: `TuningScreen::DrawPowerGraph` (seen in play), `CarList::DrawEntryPowerGraph` (seen), `CarList::DrawCarPowerGraph` and `sub_821B8F60` (never seen). Read in full (B7 loop, 2026-11-06). |
| `sub_8217B508` | `Line::RecordFlat` (probably) | | r3 = not read (overwritten); r4 = a sprite (+0 its world matrix, +80 and +96 the two ends' x, y, z, +112 and +128 their colors as red, green, blue, alpha words), r5 = a camera pass, r6 = a sort key. A 128-byte frame. The app's interface `[0x826BABA0]`: slot 8 (+32), its answer's slot 1 (+4) = the command list; slot 8 again, its answer's slot 2 (+8) = the render queue; stored at `0x826D83D8` (the queue) and `0x826D83DC` (the list), every call. On the list: `CommandList::OpenChunk`; a header slot (`AllocateSlots`, 1); `Primitive::RecordTransform`(pass, sprite), which writes on the list at `[0x826D83D4]` (the other recorders' copy; the same list, probably); a slot [13, 2, 1, 0] (`Triangle::RecordFlat`'s is [13, 4, 0, 0]: +4 probably the primitive type, 2 a line list); `AllocateVertices`(list, 2, 1) (with its header, command 5): two 24-byte vertices, +0 x, +4 y, +8 z from the sprite's +80 and +96, +12 the color as ARGB (the alpha word's low byte, then the red, green and blue words' low bytes: bytes +115, +119, +123 and word +124; 16 bytes on for the second), +16 to +23 not written; a slot with +0 = 15 (the helper object's draw; +4 to +12 not written); `CloseChunk`; the header's halfwords +0 and +2 = 0, +4 and +12 = 0, +8 = `ChunkSlots` − 1; the queue's slot 4 (+16; `RenderQueue::Add`) with r4 = the header, r5 = the key, f1 = 0.0 (`0x82000A34`). Returns nothing. Only caller `PowerGraph::Draw` (2 × (n − 1) calls a draw). Read in full (B7 loop, 2026-11-06). |
| `sub_82250670` | `CarList::DrawEntryPowerGraph` (probably) | | r3 = an entry of the car list screen's list (`sub_8224BDC0`: the pointer at its +1820 array, index +236 + +240): nothing when the word at `0x82815EC0` is 0 (written by `sub_82250778`, `sub_82250890` and `sub_82251320`, cleared by `sub_82250AA8`; not read: probably whether the graph is shown). Else, in a 3,920-byte frame: a car object at r1 + 1936 cleared (`sub_82110448`, the car makers' lead) and given the entry's +8432 (`Car::SetModelAndClass`, `career.md`), a car data at r1 + 80 (`CarData::CarData`), `sub_82179A60`(`[0x826D83E0]`'s +0 word, the car, the car data) (the lead: sets the car data up from the car), then `PowerGraph::Draw`(the same word, f1 = 1042.0 `0x8200AD98`, f2 = 500.0 `0x82001FC0`, r6 = the car data). Returns nothing. Only caller `sub_8224BDC0` (a car list screen's update, a lead), when its +232 isn't 2. Read in full. |
| `sub_82250700` | `CarList::DrawCarPowerGraph` (probably) | | r3 = a car object (1,968 bytes; from `sub_8224BDC0`: when the screen's +232 is 2: a car record at `sub_82249668`'s answer + 6120 + 212 + 1,968 × a slot, the slot found by walking a list of halfword links from +204, +236 + +240 steps; probably the player's garage): nothing when `[0x82815EC0]` is 0; else a car data on its stack (r1 + 80, 1,952-byte frame; `CarData::CarData`), `sub_82179A60`(`[0x826D83E0]`'s +0 word, r3, the car data), `PowerGraph::Draw`(the word, 1042.0, 500.0, the car data), as `CarList::DrawEntryPowerGraph` without making the car. Returns nothing. Callers `sub_8224BDC0`, `sub_82244E00`, `sub_82234B80` (never seen). Not seen in play. Read in full. |
| `sub_821B8F60` | | | r3 = a screen (+9604 written). Movie 9 (`UiMovies::Find`); the object at `[0x826D8464]`: by its +12 (-1: `sub_8211E1B8`(movie, 0); 0 or 1: one of two functions on its stack, `0x821B8ED0` and `0x821B8588`, called with its +24, then its +12, +16, +20, +24 and +0 updated). Then `[0x826D846C]`'s +8 set to 21 (its old value into the screen's +9604 when not 21), and while `[0x826D846C]` +4 is set, `PowerGraph::Draw`(`[0x826D83E0]`'s +0 word, f1 = 1036.0 `0x8200A558`, f2 = 508.0 `0x8200A55C`, r6 = `[0x826D846C]` + 112, a car data). Starts with `sub_82106540`(100, 30, 0, a string at `0x8200A560`), the empty function. No direct callers (a vtable). Not seen in play. Read, not in detail. |
