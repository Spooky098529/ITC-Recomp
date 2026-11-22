# E5 step 3: the swarm's reports

Each batch's report (`docs/loops/e5-loop.md`, "Step 3 as a swarm"), copied from
the Workflow's journal by the lead: what moved, what was skipped and why, the
lifted code the agents judged, proposed row changes, notes for the lead.

## Batch 1 (2026-11-22, `lifted_sites: false`)

11 areas, 11 agents (run `wf_0f4a9297-316`). 6 types moved, 12 skipped.
Checked by the lead: schema_gen, schema_check (0 accesses that fit no field),
`lift.py reemit` (no host type's access left raw; 13 lifted stores became
`set_*_bits`), `lift_originals.py --all`, the build (no errors), an attract
race (no error lines, no handle fault), `tick_replay.sh` (both races all
match), a frame dump (normal). The replay reaches only `TitleLoadBlock`'s
create and free (`TitleLoad::Create`, `Destroy` ran); the other five moves
are on paths neither race takes (the parking area, the type 5, 6, 7, 10 and
12 modes), so the build is their only check so far.

### Moved

| Type | Area | Allocation | Free |
|---|---|---|---|
| `Type5DialogData` (68 B) | type5-mode | `Type5Dialog::OnEnter` (VirtualAlloc at `0x8222593C`), handle in `[0x82814E44]` and r3 | `Type5Dialog::OnLeft` (VirtualFree at `0x822257F8`), r3 = 1 |
| `ReplayTheaterData` (16 B) | type10-mode | `ReplayTheater::Create` (`0x822894A4`), handle in `[0x828161E8]` and r3 | `ReplayTheater::Destroy` (`0x82289554`), r3 = 1 |
| `TitleLoadBlock` (68 B) | title-screen | `TitleLoad::Create` (`0x82220BDC`), handle in `[0x82814E28]` and r3 | `TitleLoad::Destroy` (`0x82220490`), r3 = 1 |
| `NetModeSelectData` (36 B) | network-mode | `NetModeSelect::Create` (`0x8227C45C`), handle in `[0x82816198]` and r3 | `NetModeSelect::Destroy` (`0x8227C540`), r3 = 1 |
| `LocationScreenPicture` (16 B) | location-screen | `OnPictureLoadedA` and `B` (`0x82228EB4`, `0x82228FF4`), handles in the screen's lists A (+8) and B (+276) | `FreePictures` (`0x822284E8`, `0x82228534`), r3 = 1 |
| `Type67Screen0Words` (76 B) | type6-type7-modes | `Type6Screen0::OnEnter`, `Type7Screen0::OnEnter` (the allocation layer's pages before), handle at the screen's +9640 / +9644 | `Type6Screen0::OnLeft`, `Type7Screen0::OnLeft`, `Delete()` |

Changes beyond the allocation and free sites:
- title-screen: `TitleLoad::Update` passed the transitions the block's +16
  (the started word's address); it now passes handle + 16 (opaque), and
  `PlayLogo` and the debug steps (Update's table is their only caller)
  read and write the word through the global's view.
- location-screen: `Picture::Free` passed `XPR::Release` the picture's +12
  (the slot's address); it now passes a frame word (frame+80) holding the
  XPR and copies it back. `PictureA` and `PictureB` got their adapters back
  (`src/hooks/location_screen.cpp`): the lifted `Transition6`, `30` and `32`
  call them through the dispatch table, and their originals would read the
  host pictures raw.
- type6-type7-modes: `Screen0OnLeft`'s check whether the native step runs
  covers only the pages and menu now (the words are a handle).
- network-mode: `NetModeSelect::Destroy`'s raw read of the data's +4 goes
  through the view.

Known limit (as the pilot's): with a type's `--native_*` flag off (or
`--native_heap=false` for type 6 and 7), the lifted originals allocate a
guest block the host views can't read. The flags are on by default.

### Skipped

| Type | Area | Why |
|---|---|---|
| `ModeType8` | boot-mode | Allocated and freed in lifted code; the type 8 (multiplayer) mode, unreachable. Drop from the pick list. |
| `TrackZones` | track-zones | One 1.2 MB block holding 70 embedded `TrackZone` objects whose addresses are listed, passed and memcpy'd into: memory used as memory. |
| `GameModeType6`, `GameModeType7` | game-modes | Reached as their base `GameMode` by many areas, virtual calls through a raw vtable read; move with `GameMode` or in step 4. |
| `ModeHolder` | game-modes | Allocated in app-states, an app state layer the state stack calls virtually. |
| `GarageScreenData` | garage-screen | `SetHeader` passes the `pick` field's guest address to `UiMovie::SetChildNameText` (ui-movies). Movable if the text were staged in a guest buffer: the user's call. |
| `CarSoundArray` | car-sounds | One block holding seven embedded `CarSounds` reached by address everywhere. |
| `CarSoundWaveBank` | car-sounds | A file-backed buffer (XWB) read raw by the XMA voices. |
| `Type4BlockPart` | title-screen | Part of `ModeType4::Init`'s one allocation; moves with its container. |
| `LocationScreenPictureTexture` | location-screen | Its address goes to `UiMovie::SetPictureTexture` and lifted transitions; the same channel carries guest textures. |
| `LocationScreenPart` | location-screen | `ModeType5::Init`'s 28,940-byte block holding several objects. |

Suggestions for `e5_movable.py` (from the agents): flag types with embedded
objects whose addresses are taken, file-backed buffers (an `Archive::Load`
destination), and polymorphic types reached through a base or virtual
calls.

### Proposed row changes

For the user's review (not written): see the lead's message of
2026-11-22; Status unchanged in every row (each function's other objects
are still guest objects), the Notes extended.
