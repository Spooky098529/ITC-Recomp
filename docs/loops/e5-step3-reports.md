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

With a type's `--native_*` flag off: checked during batch 3, none of
these allocation and free sites (but `Type67Screen0Words`', not checked)
is in a lifted original (the tools found no emitted call there), so
there is no guest-block fallback: an address with no entry stops the game
(stage 4's rule).

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

## Batch 2 (2026-11-22, `lifted_sites: true`)

9 areas, 9 agents (run `wf_0de3cceb-1eb`). 1 type moved by an agent, 1 by
the lead, 18 skipped (and `FloatPair`, moved earlier by the lead as the
check of `host_alloc` and `host_free`). Checked by the lead: schema_gen,
schema_check (0), `lift.py reemit` (no host type's access left raw),
`lift_originals.py --all`, the build (22 errors at first: the pause data's
words reached by address in `options.cpp` and `transitions.cpp`, which the
agent hadn't converted; fixed with `race_screens/pause_data.h`), an attract
race (no error lines), `tick_replay.sh` twice (both races all match;
`PauseMenu::Create`, `PauseMenu4::Create` and `PauseMenu::Destroy` ran), a
frame dump (normal).

The first `reemit` stopped: the pause data's sites are in the lifted
originals, and `originals.cpp` was stale at that point. Since then each
tool also accepts a site that is a call in the other tool's functions
(their generated bodies), so their order doesn't matter.

### Moved

| Type | Area | Allocation | Free |
|---|---|---|---|
| `PauseMenuData` (160 B) | race-screens | `PauseMenu::Create` (`Pages(160)` before; the original's `VirtualAlloc` at `0x8220D824`, `host_alloc` for the lifted original), handle in `[0x826D84D4]` | `PauseMenu::Destroy` through `DestroyFour`'s new `free_data` (the original's `VirtualFree` at `0x8220C524`, `host_free`, returns 1) |
| `PauseMenu4Data` (80 B) | race-screens-unseen (moved by the lead) | `PauseMenu4::Create` (`0x822108A4`, `host_alloc`), handle in `[0x826D84E4]` | `PauseMenu4::Destroy` (`0x8220FCF4`, `host_free`, returns 1) |

Changes beyond the sites: the pause data's 40 fields are consecutive
words the original reaches by address (a field's address + 4 x an index,
runs of 2 to 4 words copied, a table of field addresses in
`ToggleOption`'s frame); `race_screens/pause_data.h` reads and writes the
host struct's word at a byte offset (`PauseWord`, `SetPauseWord`,
`CopyPauseWords`), and `ToggleOption`'s frame table holds handle + offset
(read back as the offset). `StepVolume` and `StepAdjust` (the agent's)
switch on the index field by field.

### Skipped

| Type | Area | Why |
|---|---|---|
| `FloatTriple` | heap | `FloatTripleItem`'s vtable slot 14 (`sub_820A7BC0`, lifted) reads its +12 raw through the item; never measured. |
| `FloatPairItem`, `FloatTripleItem` | heap | Handed out by the factory `sub_820A81C0`; their vtable functions read them raw; dispatched by callers. |
| `ByteCountedList` | heap | Stored raw into a caller's frame copy and handed on; may mix guest and host lists. |
| `D3DXBuffer` | heap | Handed to the shader objects and the hardcoded shaders' adapter; `Release` reads it raw. |
| `FrameStart` | framework | A delegate target: the delegate thunks read its vtable through the generic `DelegateTarget`. |
| `HandlerMapNode` | framework | A red-black tree walked raw by lifted code and other areas. |
| `MorphJobMaker` | framework | `car_draw/morph.cpp` reads its vtable raw; dispatched virtually from morph-jobs. |
| `WorkerPool` | framework | Hands out an interior interface (pool + 8) to other areas; virtual calls through it. |
| `AppDelegate`, `AppStates` | app-states | Vtables read raw on every virtual call (`VirtualTarget`). |
| `StateLayer` | app-states | Embedded interface and lists whose addresses are handed out. |
| `CarListEntry` | car-list | A memcpy target for the car's 8,432-byte spec; read by address in other areas. |
| `CarDealerSwatch` | material-passes | Its hue's address passed to `Color::HsvToRgb`; its sites in car-dealer's files. |
| `StandModelsSpecCopy` | stand-models | A memcpy target; the Xbox Live mode only (unreachable). |
| `CarOverlays` | race-overlays | Also its base `Overlay`'s object; its address handed to `Archive::Load` and other areas. |
| `OverlayView` | race-overlays | `FreeView`'s per-slot path reads its vtable raw; the deleting destructor stores through a shared type. |
| `Kind6MenuPart` | race-screens-unseen | Part of `Kind6Menu`'s one block (the Xbox Live mode). |
| `CareerResultsScreen` | race-screens | A memcpy target and an embedded page object handed out by address. |
| `PauseMenuScreen` | race-screens | Its measure also takes the transitions' step word (a measure fault): lifted code would read a guest address as a handle. |

Patterns (for later batches and step 4): most of the rest has vtables
read raw (virtual dispatch), interfaces or objects embedded at offsets, or
is a memcpy target.

### Proposed row changes

For the user's review (not written); Status unchanged (each function's
other objects are still guest objects).

## Batch 3 (2026-11-22, `lifted_sites: true`)

9 areas, 9 agents (run `wf_a554839a-b7c`). 2 types moved, 18 skipped.
Checked by the lead: schema_gen, schema_check (0), `lift.py reemit` (no
host type's access left raw), `lift_originals.py --all`, the build (no
errors), an attract race (no error lines), `tick_replay.sh` twice (both
races all match), a frame dump (normal). Neither move is on the replay's
path (the cutscene shots and parking area; the Xbox Live mode): the build
is their check.

### Moved

| Type | Area | Allocation | Free |
|---|---|---|---|
| `CameraAnimLoad` (4 B) | camera-modes | `CameraAnims::Load` (`AnimsLoad`; the original's `VirtualAlloc` at `0x82150B3C`, `host_alloc` for its lifted original); the handle is the archive callback's context | `CameraAnims::OnLoaded` (`Delete()`; the original's `VirtualFree` at `0x82150940`, `host_free`, returns 1); the native path's guard asks for a handle |
| `NetScreen7Data` (8 B) | system-services (sites in network-mode's `machines.cpp`, made by the lead) | `Type12Screen7::Create` (`0x82268D8C`), handle in `[0x82816168]` and r3 | `Type12Screen7::Destroy` (`DestroyOf<6>`, `0x82268F54`), r3 = 1 |

### Skipped

| Type | Area | Why |
|---|---|---|
| `TodaysResultData` | todays-result | Its `pick` text's address goes to `UiMovie::SetChildNameText` (as `GarageScreenData`). |
| `TodaysEventImage` | todays-result | Shares the archive callback `Type2Hub::ImageLoaded` and `FreeImage` with type2-screens' image records: moves only with them, as one type. |
| `SysUsers` | system-services | save-interface reads it through its own view and its vtable raw; allocated in lifted framework code. |
| `SysLinkIdSetNode` | system-services | System link: unreachable. Drop from the pick list. |
| `CarDealerEntry` | car-dealer | A memcpy target for the car's spec; its address read by other areas. |
| `AppCamera` | cameras | Multiple inheritance (interfaces at +8, +12 handed out); its address in three delegates. |
| `AppCameraList` | cameras | Its vtable read raw by the hand port's virtual call; the lifted `Add` passes its +4 on. |
| `AppCameraEntry` | cameras | Its vtable read raw right after the lifted allocation. |
| `CameraCubeFaceSurface` | cameras | A D3D surface header handed to the graphics code. |
| `TrackCollisionObject` | collision-world | `TrackCollision::Load` stores through it raw (untyped). |
| `Vector16Storage` | car-scene-draw | A vector's element array (variable size), walked by pointer arithmetic. |
| `Type2Hub` | type2-screens | The hub's embedded state machine (a measure fault filed the data's allocation under it). |
| `Type2HubImage` | type2-screens | Shares `ImageLoaded` with todays-result's records. |
| `Type2HubTexture` | type2-screens | Its address stored into the hub movie's texture slots. |
| `CollisionObjectBlock` | car-physics | An `operator new[]` block of 320-byte objects reached by address arithmetic. |
| `CameraAnimBlock` | camera-modes | The originals read its +60 through `CameraAnimData`, a separate type (a measure fault); the agent lists how to make it movable. |

### Proposed row changes

For the user's review (not written); Status unchanged.
