# Calls into hand ports with no entry

Branch `missing-functions` (from `main` at e4135872, 2026-11-28). The
problem is in `docs/known-issues.md`, "Calls by address with no dispatch
entry". In short, lifted code calls some functions by a constant address
(`lifted::Call(0x821824A8, LIFTED_ARGS(...))`). An address with nothing in
the dispatch table stops the program. Hand ports whose adapters were
deleted in the rewiring have nothing there. The fallback originals
(`src/lifted/originals.cpp`) still call them by address, so a rarer path
stops the game. A new career's first car dealer visit is one of those
paths. The user's call (2026-11-28): the originals call the hand ports' C++
directly. Hand ports don't get their dispatch entries back.

## The survey (2026-11-28)

`tools/call_targets.py` lists 1,720 constant call targets in `src/`, and
`itc --check_call_targets` finds 438 of them with no entry. Of those 438:

| Kind | Count | What a direct call needs |
|---|---|---|
| Hand port with a register step still in `src/hooks/` (once the harness's) | ~105 | Only the entry: the step already maps registers to the port |
| Hand port, plain parameters, no step | 199 | A shim that reads the registers, calls the port and writes r3 or f1 |
| Hand port taking a `Calls` struct (or a `GuestCall`), no step | 70 | A shim like the above that also builds the area's `Calls` from the register view |
| Hand port returning `std::optional` (it can decline), no step | 13 | A shim, plus the original lifted for the cases the port declines |
| No hand port of its own: merged into its caller's C++, never ported (stayed recompiled and was never lifted), or an import's wrapper | 51 | Nothing to call: its original gets lifted instead (as every other fallback) |

The "~105" comes from matching each port's name inside the steps' bodies,
so a few names shared by several areas (`Init`, `SetSpeed`) may be off by
a step. Each gets checked when its shim is written.

- The originals call these 438 at 733 sites. 435 are called only from
  `originals.cpp`.
- The other 3 (`825B0BB0`, `825B0BE4`, `825B8778`: the C runtime's
  floating-point exception paths, never reached in play) are called from
  live lifted code (`batch_041.cpp`). None of the 3 has a port.
- On the car dealer path the user hit, 3 of the 438 are left:
  SetNameTexts `82182388`, SetPriceTexts `82182418` and SetSpecTexts
  `821824A8`. The lifted ShowCarInfo `82182620` calls them, and it's a
  fallback reached from lifted OpenColors `821847A0`. All 3 still have
  steps (`hooks::CarDealerSet*Texts`).

Per area (the port's folder in `src/`):

| Area | Count | Has a step |
|---|---|---|
| car_draw | 46 | 0 |
| car_physics | 29 | 0 |
| ai_drivers | 22 | 1 |
| screen_constructors | 17 | 0 |
| tuning_screen | 16 | 11 |
| race_hud_object | 16 | 16 |
| alt_hud | 14 | 14 |
| car_list | 13 | 0 |
| course_scenery | 12 | 2 |
| camera_modes | 9 | 3 |
| screen_objects | 9 | 0 |
| graphics | 9 | 0 |
| 56 other areas (1 to 8 each) | 175 | 58 |
| no port | 51 | — |

The full list, by area, with each function's port name, is at the end.

## What's already there

- `tools/lift.py` already emits a direct call for a callee that has an
  entry, `lifted::entry::e<address>(LIFTED_ARGS(...))`, in place of
  `lifted::Call`. `lifted::Call` is only for addresses with no entry. The
  list of entries is `src/lifted/entries.h`, written by
  `tools/entry_list.py` from a build's symbols (`nm`).
- `tools/lift_originals.py` writes `originals.cpp` with the same code.
  Regenerated with its current set of 2,369 originals (2026-11-28), it
  writes exactly the committed file, so it can be run again safely.
- The 4 entries added on 2026-11-27 (SetColorBar and the others) are
  still called by address in `originals.cpp`, because `entries.h` wasn't
  written again after they were added.

## The mechanism

1. A new macro beside `ITC_NATIVE_ENTRY` in `src/hooks/native_entry.h`:
   `ITC_DIRECT_ENTRY(address)`. It defines `lifted::entry::e<address>`
   from a body on the register view, as `ITC_NATIVE_ENTRY` does, but
   doesn't register it in the dispatch table. Code that names the address
   calls it. A call through a pointer or a vtable to that address still
   stops, as it does today.
2. One shim per callee, next to the area's steps in `src/hooks/`. Where a
   step exists, the shim is one line:

   ```cpp
   // For the fallback originals (docs/missing-functions.md): lifted
   // ShowCarInfo sub_82182620 calls these three by address.
   ITC_DIRECT_ENTRY(82182388) { hooks::CarDealerSetNameTexts(ctx); }
   ITC_DIRECT_ENTRY(82182418) { hooks::CarDealerSetPriceTexts(ctx); }
   ITC_DIRECT_ENTRY(821824A8) { hooks::CarDealerSetSpecTexts(ctx); }
   ```

   Where no step exists, the shim maps the registers itself. This is the
   deleted adapter's body. The adapters are in git history from before
   their removal, and their argument mapping is in the area's
   `docs/functions/` rows:

   ```cpp
   ITC_DIRECT_ENTRY(820B0508) {
     ctx.f1.f64 = car_physics::VehicleSurfaceValue(ctx.r3.u32, ctx.r4.u32);
   }
   ```

   A port in an unnamed namespace, or a `static` one, gets declared in its
   area's header.
3. Build, then run `tools/entry_list.py` (the new entries go into
   `entries.h`), then `tools/lift_originals.py` with the current set (from
   `originals.h`) plus the 51 with no port. The originals then call the
   shims directly. The 51 and the 3 C runtime paths become lifted
   fallbacks. Build again.
4. Check: `call_targets.py` and `--check_call_targets` should find 0
   targets with no entry. A direct call to a shim that's missing fails
   the link, so nothing can stop at run time.

`generated/` isn't touched. Of the generators, only `entry_list.py`'s
input changes (the new symbols); `lift.py` and `lift_originals.py` stay
as they are.

## Order

1. The car dealer: the macro, its 3 shims, and the regeneration. Check: a
   new career with no save gets through the dealer.
2. The rest, area by area, in small commits: the step one-liners first,
   then the plain shims, then the `Calls` ones and the 13 that can
   decline. Then the 51 lifted, the regeneration, and the checks
   (`tick_replay.sh`, `scripted_run.sh race_test.jsonl`,
   `attract_race.sh 40`, the Windows build).

## The list

| Area | Count | Has a step | Functions |
|---|---|---|---|
| car_draw | 46 | 0 | `820DBD38` CockpitPointsDraw, `820DCC10` LightPointsDrawNeon, `820DD0B0` LightPointsDrawNeon, `820DD5C0` LightPointsDrawNeon, `820DE1C8` PartsDrawFrontBumperLights, `820DE3C0` PartsDrawRearBumperLights, `820DE6E0` PartsDrawDoorPanel, `820DE760` PartsDrawDoorWindows, `820DE8C8` PartsDrawDoorPanel, `820DE948` PartsDrawDoorWindows, `820DEA88` PartsDrawDoorsBody, `820DEAF0` PartsDrawDoorsWindows, `820DED88` PartsDrawFendersBody, `820DEDD8` PartsDrawFenderLights, `820DEF38` PartsDrawCabinWindow02, `820DEFA8` PartsDrawCabinWindow01, `820DF070` PartsDrawEngine, `820DF308` PartsDrawFrontBumper, `820DF458` PartsDrawRearBumper, `820DF8B0` PartsDrawTailLights, `820E0998` PartsDrawBonnet, `820E0B40` PartsDrawDoor, `820E0E38` PartsDrawDoor, `820E1138` PartsDrawShadowed, `820E1260` PartsDrawShadowed, `820E13E0` PartsDrawHeadlightBody, `820E1690` PartsDrawHeadlights, `820E1D88` PartsDrawShadows, `820E3170` InteriorDrawDialFaces, `820E3548` InteriorDrawRollCage, `820E3AD8` InteriorDrawMeterFaces, `820E3CF0` InteriorDrawSeats, `820E51D0` InteriorDrawReadouts, `820E55A0` TrafficDrawBody, `820E5668` TrafficDrawWindow, `820E5B08` TrafficDrawTailLights, `820E61D8` TrafficDrawHeadlights, `820E8AE8` WheelsDrawTire, `820E9318` WheelsDrawTireMorphed, `820E9A28` WheelsDrawRimA, `820EA148` WheelsDrawRimAMorphed, `820EA950` WheelsDrawRimB, `820EB388` WheelsDrawBrakes, `820EBEB8` WheelsDrawBrakesMorphed, `8235A688` XgSetVertexBufferHeader, `82597FE0` MatrixShadow |
| car_physics | 29 | 0 | `820B0508` VehicleSurfaceValue, `820B0560` VehicleSurfaceValue20, `820B05B8` VehicleSurfaceValue12, `820B0610` VehicleSurfaceValue16, `820B09C8` TireMagicFormula, `820B3010` VehicleUpdateFreeAxle, `820B3130` VehicleAxleReactionTorque, `820B31D0` VehicleUpdateDrivenAxle, `820B3C90` VehicleSetGearValues, `820B4CF0` VehicleUpdateGearbox, `820B5588` VehicleUpdateWheelSuspension, `820B6A38` RigidBodyEffectiveMassAt, `820B6E70` RigidBodyUpdateOrigin, `820B6F38` RigidBodyUpdateOrientation, `820B70C8` RigidBodyUpdateVelocity, `820B71B0` RigidBodySetCenterOfMassPosition, `820B7478` RigidBodyUpdateAngularVelocity, `820B7A00` CollisionObjectInit, `820B7AE8` CollisionObjectConstruct, `820B8798` CollisionWorldCallback, `820B8AF8` CollisionWorldRemovePairsByGroups, `820B94B0` CollisionWorldAddPairs, `820BBC40` CollisionWorldUnlinkPair, `820BBD30` CollisionWorldMovePair, `820BC1F8` CollisionSurfaceA, `820BC240` CollisionSurfaceB, `820BC288` CollisionSurfaceC, `820BC2D0` CollisionSurfaceD, `820BC550` CollisionWorldResetSurfaces |
| ai_drivers | 22 | 1 | `82138D20` SetFlag2, `82139580` ResetControl, `82139DA0` StartRace, `8213B058` Drive, `8213B5D8` RefreshPoints, `8213C5C8` BlockLeft, `8213C630` BlockRight, `8213D508` SetMode, `8213D670` SetRanges, `8213D9D8` FollowTargets, `8213E690` AvoidOthers, `8213EB48` FollowLeaderBranch, `8213ECF8` Extend, `8213F2E0` DropPassed, `8213F4B0` PlanLine, `8213F698` ResetSkill, `82145118` PickTarget, `821452A0` AdjustForOther, `821454C8` UpdateOther, `82145610` MirrorOther, `821456A0` UpdateOtherTimer, `82145CA8` ReactToOthers |
| screen_constructors | 17 | 0 | `82230800` PanelE, `822308E8` Panel6F, `82231C68` PanelGroup6B, `82231CD0` Panel6C, `82231E40` Panel6D, `82240110` PanelGroup, `82240268` Panel7F, `82241680` PanelGroup7B, `822416D8` Panel7C, `822418B8` Panel7D, `82249C08` GroupPair, `8224B660` PanelSet, `8224CF58` MachineScreen, `82252908` ListPart, `822550F8` ColorListScreen, `822570F0` TitledListScreen, `8225A3D8` NodeScreen |
| tuning_screen | 16 | 11 | `821CDB18` IsItemAvailable, `821CDED8` WheelNumber, `821D99A0` PickCameraPreset, `821DA3F8` ShowInteriorPicture, `821DE2D8` AeroPartValue, `821DF448` EquipChoice, `821DF788` SetCarStats, `821DFBE8` HideNode19, `821E07B0` PreviewItem, `821E22E8` ShowTireMarks, `821F1DE0` SetStateTexts, `821F2110` RefreshPaint, `821F2378` RefreshLightKits, `821F2AD8` RefreshInterior, `821F7A98` RefreshMovie14, `822076E8` RefreshMovie13 |
| race_hud_object | 16 | 16 | `82211BE8` MovieMask, `822123C8` SetUpTimer, `82212D08` ShowGauge, `82212D58` SetTach, `82212DB0` SetGear, `82212E18` SetBoost, `82212E60` UpdateRecordingFull, `82213F20` SetUpRacers, `82214260` SetSpeed, `82214698` SetBar, `82214718` UpdateSigns, `82214D78` TickTimer, `82215160` UpdateExtraGauges, `822151F8` TickRacers, `82215260` Init, `82216A60` Make1004 |
| alt_hud | 14 | 14 | `8220BF00` MenuListPressedX, `82216C40` SetUpGauges, `82216D78` ShowPanels, `82216E00` SetSpeed, `822173E8` SetTach, `82217460` SetGear, `822174E0` SetNode25, `82217538` SetBoost, `822175A0` SetBar, `82217638` SetThrottle, `82217690` SetBrake, `822176E8` SetSteering, `822178A0` SetUpExtraGauges, `82217A30` UpdateExtraGauges |
| car_list | 13 | 0 | `82249668` SaveData, `8224A490` ShowStandCar, `8224C008` UpdateStand, `8224D2D0` ColorSetArrows, `8224D3E0` ColorPlaceCursor, `8224D458` ColorSetCount, `8224D518` ColorSetSwatches, `8224D7F0` CarSetTexts, `8224D9A0` CarPlaceCursor, `8224E920` CarShowStats, `8224EF18` SetCaption, `8224F758` RowFill, `8224F888` RowRefresh |
| course_scenery | 12 | 2 | `820F1530` ModelAsset, `820F1598` ListLoadAll, `820F1A20` ModelListInit, `820F2030` PoolSizes, `820F2230` CourseSetInitParts, `820FB8E0` InUse, `820FB8F0` IsLoaded, `820FBBE8` Init, `820FBDC8` Shutdown, `820FC0E8` PoolObjectAt, `82146A38` FindAnimationByTexture, `82146AC0` AdvanceAnimation |
| camera_modes | 9 | 3 | `820AB6A0` SplinePoint, `820AC178` KeyValue, `8214FFF0` BlockFree, `821504E0` AnimEvalTarget, `82152BD8` ShakeRandom, `82153228` List40Link, `82154300` List16Copy32, `82211708` RivalLogoConstruct, `82293A48` HermiteBasis |
| screen_objects | 9 | 0 | `8212FC50` MapAllocateNodes, `8212FFC8` ColumnLowerBound, `82130028` MapLowerBound, `82130088` ColumnCopy, `82130140` ColumnConstruct, `82131CB8` ColumnInsertUnique, `82131EE0` ColumnDestroy, `821321F8` MapInsertUnique, `82132BD8` MapErase |
| graphics | 9 | 0 | `8229B7D8` SetRegisterF01Bits, `8229E768` WriteSegmentTail, `822A3C88` SetSurfaceInfo, `822A44A8` TextureFormat, `822A4530` TextureLevelCount, `822A46C8` TextureLayoutSizes, `822A4B98` TextureLevelLayout, `822A54B0` LockTextureLevel, `822AA0E8` SetFVF |
| collision | 8 | 0 | `820BAB48` ShapeCopyConstruct, `820BACB8` CollisionShapeType, `820BACD0` CollisionShapeTranslate, `820BB020` ShapeConstruct, `820BC858` ContactFlip, `820BD280` BoxBoxSeparated, `820BD578` BoxBoxSeparatedAfterMove, `820BF080` BoxBoxEdgesCrossing |
| action_mappers | 7 | 0 | `8210AE60` InputClientConstruct, `8210B718` Hysteresis, `8210B7D0` Analog, `8210BBA0` WordsFromState, `8210C078` ResetStages, `8259C7B8` ClientListConstruct, `8259C960` ClientListAdd |
| race_screens | 7 | 5 | `8220BA28` PagesFinish, `8220D6A0` PauseMenuSaveOptions, `8220F840` PauseMenuLoadData, `8220FB10` PauseMenuDraw, `822197A8` CareerResultsSetTexts, `8221AC00` AfterRaceMenuShowItems, `8221BB48` AfterRaceMenuDraw |
| vehicle_accessors | 6 | 0 | `820B7BF0` CollisionObjectLink, `820B7C18` CollisionObjectUnlink, `820B7D78` CollisionRecordCopy, `820C0180` VehicleWheelSlip, `82293818` MatrixSetTranslation, `82294348` Vec4DivideBy |
| replay_snapshots | 6 | 0 | `82102B98` Member4080Restore, `82102C60` Member1136Restore, `821031A0` BodyStateRestore, `82104310` Record5232RestoreTrack, `82104538` Record5232Save, `82104BC0` Record5232Copy |
| race_event | 6 | 6 | `82142EC0` UnlockState, `82143358` GroupFree, `821433A8` GroupAlloc, `82143420` PoolsCopyIds, `82143528` ItemsBeaten, `82144458` PoolsAddRivals |
| tuning_machine | 6 | 6 | `821CFFC8` LoadUnlockedParts, `821DA4B0` LoadCurrentChoices, `821DAD58` SetPrices, `821DBAB0` LoadOwnedParts, `821DE280` LoadInteriorSlots, `821F8950` SavePlaceholders14 |
| xact | 6 | 3 | `825C1DE0` VariableInit, `825C20B0` VariableConstruct, `825CEF50` XmaBlock, `825D2030` WaveNotify, `825D5520` TrackInstanceCancelEvents, `825D5A48` TrackInstancePrepare |
| second_set | 5 | 4 | `820E6980` TrafficPointsDestruct, `82594A60` ModelStreamData, `82594A78` ModelStreamSize, `82594A90` ModelStreamData2, `82594AA8` ModelStreamSize2 |
| car_models | 5 | 0 | `820EDE00` DrawSpan, `820EEBF8` TgaParseHeader, `820EED38` TgaImageClear, `820EED58` TgaImageFree, `820EF168` TgaDecode |
| strings_and_font | 5 | 0 | `82108458` FontLineWidth, `82108810` FontLineGlyphs, `82108B30` FontTextWidth, `82108C90` FontClipGlyph, `8210A5C0` StringsPageCount |
| tuning_pages | 5 | 5 | `821DE160` SetAeroAvailability, `821F89B8` RestoreStickerTexture, `821FE6A0` FreeTexture, `822081A0` RestoreMovieTexture, `8224AB50` HsvToRgb |
| model_animation | 5 | 0 | `825A0FE0` TrackSetSample32, `825A10A8` TrackSetAdvance32, `825A1C88` TrackSetSampleFloat4, `825A1D68` TrackSetAdvance, `825A66F8` KeyInterpolate |
| c_runtime | 5 | 0 | `825A9980` Vswprintf, `825AFDC8` Fltin2, `825B6378` DoubleToLd12, `825B7F10` Mtold12, `825B8B48` I10Output |
| framework | 4 | 0 | `82099C80` MapAllocateNodes, `82099E20` MapNewNodeOf, `8209A0E8` RequestsInsertN, `8209ABE8` RunFramework |
| app_start_up | 4 | 0 | `8209ADD8` TilingLargestTile, `82109228` FontConstruct, `82575B08` XNetStartup, `8259CD50` StartXAudio |
| car_specs | 4 | 0 | `820C46B8` InchUp, `820C9928` GetTire, `820CB588` SwapWheel, `820CC168` SwapCarSpec |
| race_overlays | 4 | 0 | `820D9DB0` ModelHasPoint, `820DA988` ModelPointPosition, `8212A350` SmokeEmit, `8215DE80` RaceOverlaysClear |
| accessories | 4 | 0 | `820F08D0` FileFreeData, `820F0920` EntryNew, `820F0C90` FileLoad, `820F0E60` SetConstruct |
| career | 4 | 0 | `82112028` DayLogPush, `821139B8` StatsAddWin, `82113AB8` StatsAddLoss, `82113B50` StatsAddOutcome3 |
| ui_movies | 4 | 0 | `8211B160` PairsAppend, `8259D730` ApplyTransform, `8259D9F8` CopyDrawContext, `8259E620` CopyQueuedText |
| post_effects | 4 | 0 | `8212EC90` GlareDefInit, `8212ED78` GlareDefConstruct, `821330D0` RenderTextureCreate, `82134060` GlowRecordStarPass |
| race_world | 4 | 4 | `82143508` RaceEventEntry, `82144A38` PlaceStartsCurrent, `82170CB8` BuildNeighbors, `82171070` PickModel |
| track_collision | 3 | 0 | `820F7AA0` TrackMeshGroundHeight, `820FC188` WallQueryGatherCells, `820FDC18` WallQueryTestCell |
| game_modes | 3 | 0 | `82107168` RaceModeMemberConstruct, `82108160` MachineMemberConstruct, `82152D28` Buffer28Destroy |
| type67_modes | 3 | 2 | `8217BE08` CameraPresetConstruct, `8217D760` GarageCameraFreePresets, `82180968` GarageStageInit |
| car_dealer | 3 | 3 | `82182388` SetNameTexts, `82182418` SetPriceTexts, `821824A8` SetSpecTexts |
| texture_compression | 3 | 0 | `8235AAC0` CompressSurface, `824206E8` PackR5G6B5, `82420AB8` FitColorEndpoints |
| xaudio | 3 | 3 | `8245B208` StageSwap, `82460A58` ReleaseOutputs, `82461140` SetFormatWords |
| car_scene_draw | 3 | 3 | `82597150` BoxExtend, `825971E8` Vector16Size, `825976C8` PlaneIntersect3 |
| std_string | 2 | 0 | `82091458` Copy, `82592740` WildcardSearch |
| minimap | 2 | 1 | `820F4040` TriangleInit, `820F40C0` List80Link |
| data_load | 2 | 0 | `82102888` Member1136Construct, `82104690` Member4080Construct |
| racer_controllers | 2 | 0 | `821029A8` RecordingAppend, `8215FCA8` ReadPad |
| race_states | 2 | 2 | `82162E70` SectionsBefore, `82163000` SectionsJoin |
| type2_screens | 2 | 2 | `8218F3F8` HubShowNoticeImage, `82199350` PlateShowDigitCells |
| achievements | 2 | 0 | `82290698` AllBeatenItems, `82290710` BeatsCourseTime |
| matrix_math | 2 | 0 | `82293998` InverseRotate, `82293E68` QuaternionFromAxisAngle |
| car_sounds | 2 | 2 | `82480818` EvaluateCurve, `82480F80` Pan |
| models | 2 | 0 | `8258E398` PendingFreesInsert, `82594380` PointersUninitializedCopy |
| sprites | 1 | 0 | `8209CDB0` DrawBillboards |
| data_tables | 1 | 0 | `820C97D8` SwapWord |
| car_makers | 1 | 0 | `820D0900` ResetBytes |
| stand_models | 1 | 1 | `820D8F18` WheelsSetTireTextures |
| scene_models | 1 | 1 | `820D9E00` SceneModelAddSecondWheels |
| race_setup | 1 | 1 | `820FF558` LapRecordReset |
| save_data | 1 | 1 | `821058A8` ControlMapSetDefault |
| race_scene | 1 | 1 | `8215D6C8` SceneConstruct |
| track_zones | 1 | 0 | `8228B5A0` List128Link |
| network_mode | 1 | 0 | `82291670` SessionRelease |
| archive | 1 | 0 | `82294BA8` ListAppend |
| memory | 1 | 0 | `822965C0` PhysicalSize |
| input | 1 | 0 | `825920D0` InputProcessorBaseConstruct |
| scene | 1 | 1 | `82593FF8` ListDeleteAll |
| shader_objects | 1 | 0 | `82594D20` LoadShaders |
| frame_driver | 1 | 1 | `825BA380` CreateThread |
| (no port) | 51 | 0 | `8209BD40`, `820B6938`, `820B6940`, `820BC8A8`, `820BCF28`, `820CB848`, `820CB8D0`, `820CB958`, `820CBA68`, `820CBAF0`, `820CBB78`, `820CBC00`, `82108F68`, `82296E68`, `8229DC28`, `822A5B68`, `822AD850`, `822B5C90`, `822B5D70`, `822B7D50`, `822B7E00`, `822B81D0`, `822B8D28`, `822BA3B0`, `822BA460`, `822BC068`, `822BE520`, `822C37C8`, `822CB808`, `822CD790`, `822E1788`, `8235D380`, `8235DC98`, `8235ED08`, `8235ED50`, `8235F548`, `8235F6C8`, `8235F6D0`, `8257C488`, `8259B748`, `8259BE40`, `8259EC58`, `825A7044`, `825AA1A8`, `825ABFE8`, `825AFD98`, `825B0BB0`, `825B0BE4`, `825B32F0`, `825B6890`, `825B8778` |
