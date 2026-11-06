# To check in a drive

The functions the B7 loop made native that have no `native running
(game):` line yet: not seen in the validation's attract race or the pad
replay, nor in a drive since. Kept across runs (not started afresh like
`b7-progress.md`); the lead adds each validation's `not in game:` line
here, under its run, with the `replay:` line beside it when the replay
broke. Their harnesses ran our code; this is the validation done by
playing.

Last drive: 2026-10-30, `itc_2819` (the parking area, a career race,
the rival intro; 166 functions off the list).

The user drives (menus, a race, a save) with `--log_level=info`; a
function goes from this list when a log shows its `native running
(game):` line (say which log in the commit). A run with nothing left
here loses its heading.

## Run 2026-11-06 14:33 (`b7-loop-06-11-26`)

- The stand models (attract race `itc_3761`, replay `itc_3762` ok;
  `stand-models.md`, `scene-models.md`): `sub_82179818`
  GarageStage::SetDecalA, `sub_821798D8` GarageStage::SetDecalB,
  `sub_82179050` GarageStage::SetInteriorWord, `sub_821791C0`
  GarageStage::SetInteriorLevel, `sub_82179250`
  GarageStage::SetInteriorPair, `sub_82179378` GarageStage::ClearChoice,
  `sub_82179548` GarageStage::SetAero, `sub_82178EC0`
  GarageStage::ReloadDecals, `sub_82179130` GarageStage::SetTireGrade,
  `sub_821CCA38` TuningScreen::SetTireGrade, `sub_821DFA78`
  TuningScreen::PreviewWheel, `sub_820E7470` SceneWheels::SetTireGrades,
  `sub_820D8F18` Wheels::SetTireTextures, `sub_82179998`
  GarageStage::AddSecondWheels, `sub_821799D8`
  GarageStage::ApplyWheelPaint, `sub_820D9E00`
  SceneModel::AddSecondWheels, `sub_82178B98` GarageStage::FitAxles,
  `sub_820E7380` SceneWheels::SetSizes, `sub_8217DD38`
  GarageStage::SetWheel.
- The `ReapplyRecord` cleanup lead (attract race `itc_3768`, replay
  `itc_3769` ok; `car-makers.md`): `sub_82178E60`
  GarageStage::ReapplyRecordNoDecals.
- The `SetCar` cleanup lead (attract race `itc_3786`, replay `itc_3787`
  ok; `car-makers.md`): `sub_8217FDC8` GarageStage::SetCarClass,
  `sub_82182C90` GarageStage::ShowCarById, `sub_82182D30`
  GarageStage::ShowCarByIdWithClass (and `sub_82178E60` above).
- The build screen, first migration (attract race `itc_3795`, replay
  `itc_3796` ok, neither reaches the screen; `build-screen.md`, names in
  its rows; the six UI movie helpers in `ui-movies-game.md`'s area of
  `build-screen.md`): `sub_8211C370`, `sub_8211C3F8`, `sub_8211C680`,
  `sub_821087D8`, `sub_8211CCC0`, `sub_8211CD28`, `sub_8219DB50`,
  `sub_8219BA50`, `sub_8219BB20`, `sub_8219BBF0`, `sub_8219BB80`,
  `sub_8219BC50`, `sub_8219BF08`, `sub_8219BAB0`, `sub_8219C078`,
  `sub_8219BCC0`, `sub_8219C230`, `sub_8219C1C8`, `sub_8219EC50`,
  `sub_8219CC70`, `sub_8217D0C8`, `sub_8217B7C8`, `sub_8219D1B0`,
  `sub_8219D4B8`, `sub_8219D7B0`, `sub_8219D440`, `sub_8219DA58`,
  `sub_8219D090`, `sub_8219BD30`, `sub_8219FA28`, `sub_8219CE08`,
  `sub_8219C528`, `sub_8219C6D0`, `sub_8219C850`, `sub_8219C8F8`,
  `sub_8219CA08`, `sub_8219CB18`, `sub_8219CBD8`, `sub_8219BFE8`,
  `sub_821A0128`, `sub_821A0D38`, `sub_8219D3E8`, `sub_8219D248`,
  `sub_8219D550`, `sub_8219CF08`, `sub_821A1008`.
- The build screen, second migration (attract race `itc_3820`, replay
  `itc_3821` ok, neither reaches the screen; `build-screen.md`; some
  also in the first migration's line above, their adapters since
  removed): `sub_821A0060`, `sub_821A08D8`, `sub_821A0A38`,
  `sub_8219FC60`, `sub_821A1138`, `sub_8219FF78`, `sub_8219F868`,
  `sub_8219FEC8`, `sub_8219F8E0`, `sub_8219D820`, `sub_8219CC70`,
  `sub_8219EC50`, `sub_8219BF08`, `sub_8219FA28`, `sub_8219CF08`,
  `sub_8219D090`, `sub_8219D1B0`, `sub_8219BA50`, `sub_8219BCC0`,
  `sub_8219D4B8`, `sub_8219D440`, `sub_8219BC50`, `sub_8219BBF0`.

## Run 2026-11-06 11:15 (`b7-loop-06-11-26`)

- The car makers (attract race `itc_3677`, replay `itc_3678` ok;
  `car-makers.md`): `sub_8219ED10` GarageScreen::ShowBusyMarker,
  `sub_8219EDB0` GarageScreen::HideBusyMarker.
- The power graph's cleanup lead (attract race `itc_3686`, replay
  `itc_3687` ok; `power-graph.md`): `sub_82250670`
  CarList::DrawEntryPowerGraph.

## Run 2026-11-06 01:02 (`b7-loop-06-11-26`)

- Predicated tiling (attract race `itc_3654`, replay `itc_3655` ok;
  `command-buffer.md`): `sub_8229E0C0` Device::RestoreSegment.

## Run 2026-11-04 19:51 (`b7-loop-03-11-26`, continued)

- The race's sounds, first migration (attract race `itc_3498`, replay
  `itc_3499` ok; `race-sounds.md`): `sub_82176E60`
  RaceSounds::OnLeaveAfterRace, `sub_821770C0`
  RaceSounds::OnRollingStart, `sub_82177130`
  RaceSounds::RollingStartTick, `sub_82175D48`
  RaceSounds::OnRollingStartDone, `sub_82175AE8` RaceSounds::Pause,
  `sub_82175B80` RaceSounds::Resume, `sub_82175C58`
  RaceSounds::ResumeCars.
- The race's sounds, second migration (attract race `itc_3502`, replay
  `itc_3503` ok): `sub_82175CA0` RaceSounds::OnAbort, `sub_82175CF8`
  RaceSounds::OnAbortDone (both never seen in play).

- The replay's snapshots (attract race `itc_3487`, replay `itc_3488`
  ok, reached a race but no replay; `replay-snapshots.md`; they run at
  a driven race's countdown and around its replay, which the attract
  race and the recording don't take): `sub_821044D8`
  Record5232::Restore, `sub_82104310` Record5232::RestoreTrack,
  `sub_821031A0` BodyState::Restore, `sub_82102C60`
  Member1136::Restore, `sub_82102B98` Member4080::Restore,
  `sub_82102AA8` Part464::Restore, `sub_82104AB8` RaceSnapshot::Save,
  `sub_82104538` Record5232::Save, `sub_82104380`
  Record5232::SaveTrack, `sub_821039F0` BodyState::Save,
  `sub_821034B8` Member1136::Save, `sub_821033F0` Member4080::Save,
  `sub_82103300` Part464::Save, `sub_82104F68` Replay::Keep,
  `sub_82104EA8` RaceSnapshot::Copy, `sub_82104BC0` Record5232::Copy,
  `sub_821043F0` Record5232::CopyTrack, `sub_82103B50`
  BodyState::Copy, `sub_82103CB8` Member1136::Copy, `sub_82104598`
  Member4080::Copy, `sub_82104460` Member4080::CopyShapes,
  `sub_821041C0` Part464::Copy, `sub_82104D40` RaceReplay::SaveStart,
  `sub_82104E68` RaceReplay::SaveEnd, `sub_821046F8`
  RaceReplay::RestoreStart, `sub_82104898` RaceReplay::RestoreEnd.

## Run 2026-11-04 08:44 (`b7-loop-03-11-26`)

- The front end's car draw (attract race `itc_3478`, replay `itc_3479`
  ok; `front-end-car-draw.md`): `sub_820DF190` Plates::Hide,
  `sub_820DF180` Plates::Show, `sub_82178B40` GarageStage::SetDrawWord,
  `sub_82179C88` GarageStage::StepAngles, `sub_82179D78`
  GarageAnims::Turn, `sub_82179678` GarageStage::SetLightKit (Show,
  StepAngles and Turn never seen in play).

## Run 2026-11-01 12:56 (`b7-loop-01-11-26`)

- The boot mode (attract race `itc_3219`, replay `itc_3220` ok;
  `boot-mode.md`, `movies.md`): `sub_82107358`
  ScreenMachine::ScreenMachine, `sub_820A4408` Movies::Pause,
  `sub_820A4420` Movies::Resume (the last two never seen in play).
- The car draw (attract race `itc_3416`, replay `itc_3413` ok;
  `car-draw.md`):
  `sub_820E39F8` InteriorDials::DrawDigitalSpeed, `sub_820ECB68`
  WheelPoints::DrawMorphed, `sub_820E9318` WheelPoints::DrawTireMorphed,
  `sub_820EA148` WheelPoints::DrawRimAMorphed, `sub_820EBEB8`
  WheelPoints::DrawBrakesMorphed, `sub_82595110` Model::MorphShape,
  `sub_8258A9E8` WorkMem::AllocVertexBuffer, `sub_82594E88`
  Model::QueueJobRelease, `sub_8235A688` XGSetVertexBufferHeader (the
  morph chain and the morphed wheels).

## Run 2026-10-31 23:57 (`b7-loop-31-10-26-2`)

- The race HUD machine cleanup lead (attract race `itc_3072`, replay
  `itc_3073` ok; `race-hud-machine.md`, `race-hud-object.md`,
  `race-screens.md`): `sub_8216D6D0` RaceHudMachine::ResultsDraw,
  `sub_8216C6D8` RaceHudMachine::AfterRaceDraw, `sub_82211D10`
  RaceScreens::DrawAfterRaceMenu (`sub_8221A2F8` CareerResults::Draw
  and `sub_8221BB48` AfterRaceMenu::Draw, adapters removed this run,
  are under the previous run already).

- The render queue (attract race `itc_3092`, replay `itc_3093` ok;
  `device-shaders.md`): `sub_82591540` PtrVector::PushBack,
  `sub_825911C8` PtrVector::Insert (their native paths may run only
  when the vector grows with allocation allowed, and when `InsertN`
  takes the insert).

- The `XPR::GetTexture` cleanup lead (attract race `itc_3105`, replay
  `itc_3106` ok; `location-screen.md`, a career screen):
  `sub_82228E88` LocationScreen::OnPictureLoadedA, `sub_82228FC8`
  LocationScreen::OnPictureLoadedB.

- The car scene and its second set: nothing to drive (every function
  seen in `itc_3142`-`itc_3187`); the replays `itc_3143` and `itc_3187`
  broke with the SDK's "BaseHeap::Release failed because address is
  not a region start" at the race-end change (seen in replays before
  this run too; the native code under test not ruled out).

- The load poll (attract race `itc_3215`, replay `itc_3216` ok but for
  one BaseHeap line at the race-end change; `load-poll.md`,
  `material-passes.md`; none ever seen in play): `sub_820E32E8`
  InteriorMeters::SetGauge, `sub_820E61D0` SceneTraffic::ApplyPaint,
  `sub_820D7E18` TrafficCar::SetPaint, `sub_820D3658` (the gauge
  table's entry).

## Run 2026-10-31 16:54 (`b7-loop-31-10-26`, continued)

- The in-race screens, third migration (attract race `itc_3046`,
  replay `itc_3047`: broke as `itc_3038` did, no countdown; the pause
  menu opened and the replay quit through the dialog;
  `race-screens.md`, `race-hud-machine.md`): `sub_8221BB48`
  AfterRaceMenu::Draw, `sub_8221A2F8` CareerResults::Draw,
  `sub_82219210` CareerResults::Intro, `sub_822192B0`
  CareerResults::Show, `sub_822193D8` CareerResults::Close1,
  `sub_82219A78` CareerResults::Page1, `sub_8216DD88`
  RaceHudMachine::AfterRaceUpdate, `sub_8220D740`
  PauseMenu::SaveHudOptions (`AfterRaceMenu::Update` and
  `RaceHud::TickPanel` were seen in `itc_3021`).

- The in-race screens, second migration (attract race `itc_3037`,
  replay `itc_3038`: broke: the race set up at the same time as in
  `itc_3021` (~77.5 s) but its racers never added and no countdown, as
  in `itc_2987` and `itc_3004`; the pause press at ~109 s opened the
  pause menu before the start and the later presses quit through the
  dialog; not debugged, the native code under test not ruled out;
  `race-screens.md`, `race-hud-machine.md`): `sub_8220C930`
  PauseMenu::ToggleOptions, `sub_8220CAB0` PauseMenu::ShowAdjustRow,
  `sub_8220CBF8` PauseMenu::SetArrowA, `sub_8220D130`
  PauseMenu::ShowSlider, `sub_8220DD98` PauseMenu::MoveOptions,
  `sub_8220E1F8` PauseMenu::MoveAdjust, `sub_8220E888`
  PauseMenu::ShowAdjust, `sub_8220F1B8` PauseMenu::SetAdjust,
  `sub_8216DA40` RaceHudMachine::OpenRacePause, `sub_8216D8C8`
  RaceHudMachine::OpenCareerPause, `sub_8221A6C8`
  AfterRaceMenu::ConfirmPage, `sub_8221A730` AfterRaceMenu::Intro,
  `sub_8221A7A8` AfterRaceMenu::Show, `sub_8221A950`
  AfterRaceMenu::CloseMain, `sub_8221AA20` AfterRaceMenu::Move,
  `sub_8221AC00` AfterRaceMenu::ShowItems, `sub_8221B5C8`
  AfterRaceMenu::SetUpItems, `sub_8221B768` AfterRaceMenu::ColorItems,
  `sub_8221B860` AfterRaceMenu::Start, `sub_8221BA68`
  AfterRaceMenu::Open.

- The in-race screens, first migration (attract race `itc_3020`,
  replay `itc_3021`, ok: this replay reached the countdown, the race,
  the results and the menu after the race; `race-screens.md`):
  `sub_8220BC00` MenuList::SetEnabled, `sub_8220BC40` MenuList::Fit,
  `sub_8220BF90` MenuList::Pressed166, `sub_8220D740`
  PauseMenu::SaveHudOptions, `sub_8220D248` PauseMenu::StepAdjust
  (`RaceDialog::Answer` and `Update` since seen in the replay
  `itc_3047`).

- The race HUD machine (attract races `itc_2986`, `itc_3003`, replays
  `itc_2987`, `itc_3004`; the countdown and race handlers and
  `IsFirstDay` since seen in the replay `itc_3021`;
  `race-hud-machine.md`): `sub_8216C820` RaceHudMachine::OpponentPrize,
  `sub_8216C938` RaceHudMachine::ItemBonus.

- `RaceHud::RunWarning` (attract race `itc_2979`, replay `itc_2980`,
  ok; `race-hud-object.md`): none left to drive.

- The race HUD cleanup lead (attract race `itc_2973`, replay
  `itc_2974`, ok; the rest since seen in the replay `itc_3021`;
  `race-hud-object.md`): `sub_82214D78` RaceHud::TickTimer.

- The interior accessories (attract races `itc_2946`, `itc_2963`,
  replays `itc_2947`, `itc_2964`, ok; `accessories.md`): none left to
  drive.

## Run 2026-10-31 13:47 (`b7-loop-31-10-26`)

- The menu sounds (attract race `itc_2935`, replay `itc_2936`: broke,
  the SDK's "BaseHeap::Release failed because address is not a region
  start" three times at the race-end change, as in `itc_2931`; it ran
  to the end): none left to drive.

- The race scene (attract race `itc_2930`, replay `itc_2931`: broke,
  the SDK's "BaseHeap::Release failed because address is not a region
  start" once at the race-end change ~118 s in, as in `itc_2894`; it
  ran to the end; `race-scene.md`; never seen in play before either):
  `sub_8215D0D8` RaceSceneMachine::Resume.

- The alternate HUD (attract race `itc_2920`, replay `itc_2921`, ok;
  `alt-hud.md`; the replay's race set it up but never ticked it, so
  probably never showed it): `sub_82217AC8` AltHud::Tick,
  `sub_82216C00` AltHud::RunMovie, `sub_8220BF00` MenuList::PressedX,
  `sub_82216E00` AltHud::SetSpeed, `sub_82217A30`
  AltHud::UpdateExtraGauges, `sub_82217460` AltHud::SetGear,
  `sub_822173E8` AltHud::SetTach, `sub_822174E0` AltHud::SetNode25,
  `sub_822175A0` AltHud::SetBar, `sub_82217538` AltHud::SetBoost,
  `sub_82217638` AltHud::SetThrottle, `sub_82217690` AltHud::SetBrake,
  `sub_822176E8` AltHud::SetSteering.

## Run 2026-10-30 23:03 (`b7-loop-30-10-26-2`)

- The race HUD object, migration 2 (attract race `itc_2893`, replay
  `itc_2894`: broke, the SDK's "BaseHeap::Release failed because
  address is not a region start" once at the race-end change ~118 s in,
  as in `itc_2693` and `itc_2853`; it ran to the end):
  `sub_82212270` RaceHud::StartCountdown,
  `sub_82212E60` RaceHud::UpdateRecordingFull.

- The race HUD object, migration 1 (attract race `itc_2883`, replay
  `itc_2884`, ok; `race-hud-object.md`; the timer's and racers' bars'
  modes, not in the replay's race):
  `sub_82211FB8` HudBar::Step,
  `sub_82215820` RaceHud::Touching,
  `sub_82213750` RaceHud::SetTime,
  `sub_82214A50` RaceHud::SetDistance,
  `sub_82214D78` RaceHud::TickTimer.

- The car models, migration 2 (attract race `itc_2852`, replay
  `itc_2853`: broke, the SDK's "BaseHeap::Release failed because
  address is not a region start" once at the race-end change ~118 s in,
  as in `itc_2693` and `itc_2784`; it ran to the end; `car-models.md`):
  `sub_820D5D20` CarModel::ReloadDecals.

## Run 2026-10-30 11:39 (`b7-loop-30-10-26`)

- The race event, migration 1 (attract race `itc_2673`, replay
  `itc_2676`, ok; `race-event.md`, and three in `career.md`; mostly
  `EventCondition::Met`'s kind tests, which the replay's career doesn't
  meet):
  `sub_8213FE00` EventCondition::DayDivisible,
  `sub_8213FE98` EventCondition::RacedAll,
  `sub_8213FF38` EventCondition::RacedCount,
  `sub_8213FFC8` EventCondition::WonAll,
  `sub_82140048` EventCondition::WinCount,
  `sub_821400C8` EventCondition::LocationCleared,
  `sub_82140188` EventCondition::RecordWord6,
  `sub_82140208` EventCondition::ModeStreak,
  `sub_82140290` EventCondition::SectionRival,
  `sub_82140400` EventCondition::FloatAtLeast,
  `sub_82140458` EventCondition::DayAtLeast,
  `sub_821404D8` EventCondition::Weekday,
  `sub_82140568` EventCondition::DayDigit,
  `sub_82140608` EventCondition::DayDigitSum,
  `sub_821406A8` EventCondition::Money,
  `sub_82140718` EventCondition::Races22,
  `sub_82140788` EventCondition::Races20,
  `sub_821407F8` EventCondition::Wins,
  `sub_82140868` EventCondition::WonCategoryCount,
  `sub_82140938` EventCondition::Save6024,
  `sub_82140980` EventCondition::Save5952,
  `sub_821409C8` EventCondition::Save6032,
  `sub_82140A48` EventCondition::BestTime,
  `sub_82140AC8` EventCondition::WonCategories,
  `sub_82140B80` EventCondition::Won383To391,
  `sub_82140D18` EventCondition::FullCategoriesAny,
  `sub_82140DB8` EventCondition::Table8D94,
  `sub_82140E58` EventCondition::Save6052,
  `sub_82140EC8` EventCondition::Save6054,
  `sub_82140F38` EventCondition::Save5910,
  `sub_82140FA8` EventCondition::Save4908,
  `sub_82140FF0` EventCondition::GarageCount,
  `sub_82141158` EventCondition::CarSpec129,
  `sub_82141248` EventCondition::Tuning4,
  `sub_821412E8` EventCondition::EngineType7,
  `sub_821413A8` EventCondition::CarList1,
  `sub_82141468` EventCondition::Nineties,
  `sub_82141528` EventCondition::Spec92,
  `sub_821415F0` EventCondition::CarFloat,
  `sub_82141738` EventCondition::StockParts,
  `sub_82141858` EventCondition::PartsAtLeast,
  `sub_82141990` EventCondition::Tuning60,
  `sub_82141A60` EventCondition::Tuning24,
  `sub_82141B30` EventCondition::Tuning128,
  `sub_82141BD0` EventCondition::Tuning148,
  `sub_82141C70` EventCondition::Tuning144,
  `sub_82141D10` EventCondition::AeroFitted,
  `sub_82141E10` EventCondition::Tuning108,
  `sub_82141F08` EventCondition::Tuning64To76,
  `sub_82141FC8` EventCondition::Tuning80Not88,
  `sub_82142070` EventCondition::Tuning28,
  `sub_82142110` EventCondition::Tuning184,
  `sub_821421D8` EventCondition::Decals,
  `sub_82142280` EventCondition::Tuning116,
  `sub_82142328` EventCondition::Car1588,
  `sub_821423C0` EventCondition::Sticker101,
  `sub_82142558` EventCondition::CarBytes,
  `sub_82142648` EventCondition::Setting88,
  `sub_821426D8` EventCondition::SameCars,
  `sub_82142CE0` Unlock::Any,
  `sub_82142E18` Unlock::StateKind5,
  `sub_82142FC8` RaceEvent::FindRacer,
  `sub_82143740` RaceEvent::PlaceUnlocked,
  `sub_82143C70` EventGroup::PromoteFlagged,
  `sub_821440F8` EventPools::AddRivalsOf,
  `sub_821442D8` RaceEvent::PlaceUnlockedCurrent,
  `sub_82144300` RaceEvent::Refresh,
  `sub_82144720` RaceEvent::RefreshCurrent,
  `sub_82110588` DayBlock::BestStreak

- `Career::SetUpEvent`'s cleanup (attract race `itc_2692`, replay
  `itc_2693`: broke, the SDK's "BaseHeap::Release failed because
  address is not a region start" three times at the race-end
  transition ~118 s in, the intermittent error seen before; it ran to
  the end; `career.md`): `sub_82116CE0` Garage::Add,
  `sub_821104D8` Car::SetModelAndClass, `sub_821104B0`
  Car::SpecWord744, `sub_821154E0` Garage::InsertCar

- The parking area, migration 1 (attract race `itc_2783`, replay
  `itc_2784`: broke, the SDK's "BaseHeap::Release failed" twice at the
  race-end transition ~118 s in, the intermittent error; it ran to the
  end; `location-screen.md`; neither run goes to a parking area):
  `sub_82227158` LocationScreen::OnMessage14,
  `sub_822281D8` LocationScreen::HasMoreUnlocks,
  `sub_82227618` LocationScreen::NextUnlockEntry,
  `sub_8222C9B0` LocationScreen::ApplyQueuedPage

- The parking area, migration 2 (attract race `itc_2796`, replay
  `itc_2797`, ok; `location-screen.md`):
  `sub_822280D8` LocationScreen::Page9,
  `sub_8222A978` LocationScreen::ShowUnlockEntry,
  `sub_8222D1A8` LocationScreen::Transition25

## Run 2026-10-30 01:06 (`b7-loop-29-10-26-2`)

- The app's start-up, migration 3 (attract race `itc_2629`, replay
  `itc_2630`, ok): `sub_82379350` (the app's +112 getter, no game
  caller since `Movies::Set` reads the word itself), the `ScopedLock`
  adapters `sub_8258E580` and `sub_8258E5B8` on the movie wrapper's
  lock (`video-output.md`; their users run only with
  `--play_movies_with_ffmpeg=false`)
- The career, migration 1 (attract race `itc_2659`, replay `itc_2660`,
  ok; `career.md`; the replay doesn't reach a career race's results):
  `sub_821104B0` Car::SpecWord744,
  `sub_82115CC0` Career::PickFromCar,
  `sub_82114B08` ItemRecords::AddLoss,
  `sub_82113AB8` Stats::AddLoss,
  `sub_82111E80` Counter::AddLoss,
  `sub_82111F60` WeekCounter::AddLoss,
  `sub_82113580` Opponents::AddLoss,
  `sub_82114A88` ItemRecords::AddOutcome3,
  `sub_82113B50` Stats::AddOutcome3,
  `sub_82111EF0` Counter::AddOutcome3,
  `sub_82113628` Opponents::AddOutcome3,
  `sub_821154E0` Garage::InsertCar,
  `sub_82113468` GarageList::Insert,
  `sub_82114C08` Rewards::Grant,
  `sub_82111818` Items::AnyWonKind3Stage2,
  `sub_821118B0` Items::AllWonKind3,
  `sub_82111940` Items::AllWonKind4Stage2,
  `sub_821119D0` Items::AllWonKind4Stage3,
  `sub_82111A60` Items::AllWonKind5,
  `sub_82111D20` Locations::Unlock,
  `sub_82111D98` Locations::Unlock2

## Run 2026-10-29 22:22 (`b7-loop-29-10-26-2`)

- XAudio's objects' cleanup migration 1 (attract race `itc_2529`,
  replay `itc_2530`, ok): `sub_8247F558` (the resampler's stereo copy,
  entry 107), `sub_82478E60` (its scalar path, entry 32)
  (`xaudio-objects.md`); `sub_82469E58` and `sub_8246D1E0`, still
  unseen, are under the last run.

## Run 2026-10-29 15:44 (`b7-loop-29-10-26`)

- XACT's tracks' migration 1 (attract race `itc_2432`, replay
  `itc_2433`, ok): `sub_825D7B78` XACTWaveVariationEvent::SetWave
  (`xact-tracks.md`)
- XACT's waves' last two (attract race `itc_2481`, replay `itc_2482`,
  broke: the SDK's "BaseHeap::Release failed because address is not a
  region start" twice at a transition ~118 s in, the intermittent pair
  seen before in `itc_1528` and `itc_1719`; it ran to the end):
  `sub_825D1FD8` XACTWave::Finish (`xact-waves.md`)
- XAudio's objects' migration 1 (attract race `itc_2518`, replay
  `itc_2519`, ok): `sub_82469E58` CodecBase::SetFormat, `sub_8246D1E0`
  PcmCodec::SetFrequencyRatio, `sub_8246D320` (the PCM base's
  SetFrequencyRatio), `sub_8246CFA8` PcmCodec::SetFormat
  (`xaudio-objects.md`)

## Run 2026-10-28 22:22 (`b7-loop-28-10-26-3`)

- XAudio's voices (attract race `itc_2356`, replay `itc_2357`, ok):
  `sub_82459DB0` XAudioVoice::AddRef, `sub_82459EF0`
  XAudioVoice::SetOutputVoices, `sub_82459FF8`
  XAudioSourceVoice::SetFormat (the API's), `sub_8245A0B0`
  XAudioSourceVoice::Reset, `sub_8245A270` (the codec's +72),
  `sub_82461140` XAudioVoice::SetFormatWords, `sub_82460778`
  XAudioVoice::SetFormat, `sub_8245DCF0` XAudioSourceVoice::Reformat,
  `sub_8245DD60` XAudioSourceVoice::StopAndReset (`xaudio.md`)
## Run 2026-10-28 18:45 (`b7-loop-28-10-26-2`)

- The archive's migration 2 (attract race `itc_2276`, replay
  `itc_2277`, ok): `sub_8209DB20` Archive::Cancel (`files-and-saves.md`, `archive.md`; probably a load
  cancelled when leaving a screen mid-load)
- The file layer (attract race `itc_2291`, replay `itc_2293`, ok):
  `sub_825AB010` _vsnprintf, `sub_825AAF18` _vsnprintf_l
  (`c-runtime.md`; from `sub_822B8048`); `sub_820A6BE0`
  GamePath::GamePath, `sub_820A6BF0` GamePath::SetGame
  (`file-layer.md`; only from `Movie::Open`, which the FFmpeg movies no
  longer call: probably never runs now)
- The save interface (attract race `itc_2311`, replay `itc_2312`,
  ok): `sub_82290378` Replays::BuildList, `sub_822902A0`
  Replays::Count, `sub_822900E0` Replays::ReadEntry, `sub_8228F840`
  Replays::Sort (`save-interface.md`; the replays' screen with saved
  replays)

## Run 2026-10-28 08:36 (`b7-loop-28-10-26`)

- The race world's migration 2 (attract race `itc_2203`, replay
  `itc_2204`, ok): `sub_82143920` RaceEvent::PlaceGroupSplit,
  `sub_82172D20` RaceManager::StreamTwoPlayers (`race-world.md`; other
  race kinds, two players)
- The race's states' migration 1 (attract race `itc_2235`, replay
  `itc_2236`, ok; the replay's race takes the free-start path; seven
  more seen in the replay `itc_2253`): `race-states.md`: `sub_82163440`
  ReleaseRacers, `sub_82162230` Time::Sub
- The race's states' migration 2 (attract race `itc_2252`, replay
  `itc_2253`, ok): `sub_82163000` (two sections joined by a forward
  link's side link) (`race-states.md`)

## Run 2026-10-25 16:38 (`b7-loop-25-10-26-3`)

- The UI movies' ready groups (attract race `itc_1740`, replay
  `itc_1741`, ok): `sub_8211B160` (the pairs vector's growth)
  (`ui-movies-game.md`)
- The scenery lists (attract race `itc_1769`, replay `itc_1770`, ok):
  `sub_82146BD0` SceneryModel::RecordShape (`scenery-lists.md`)
- The scenery lists' migration 2 (attract race `itc_1777`, replay
  `itc_1778`, ok): `sub_82595550` Model::RecordShape (`models.md`; its
  work runs in play inside `Model::DrawShape`, which calls
  `ModelRecordShapeChecked` past the print; only its own adapter, from
  `sub_820EF790`, never ran)
## Run 2026-10-25 03:59 (`b7-loop-25-10-26-2`)

Until d83677c the replay's boot marker was a line gone since the movies
moved to our FFmpeg, so the first two validations' replays "stalled"
(`itc_1605`, `itc_1616`); the cleanup's replay with the new marker ran
(`itc_1625`, ok: the front end, a race, the screen after it) and showed
`sub_82152D28`, `sub_82106A00`, `sub_825943E0` and `sub_82594F58`, now
off the list; the mode tick's replay (`itc_1640`, ok) showed the race
overlays' machine's four handlers (`sub_8215DD58`, `sub_8215DD90`,
`sub_8215DDD0`, `sub_8215DE10`), off it too. The mode tick's functions
all ran in the attract race. Left:

- The model class (attract race `itc_1614`): `sub_825A7348` memmove_s
  (`c-runtime.md`)
- The `atexit` step (attract race `itc_1665`, replay `itc_1666`, ok):
  `sub_825B3430`
  _lock and `sub_825B32D0` _unlock (`c-runtime.md`: their plain C++
  runs only for lock 8, which only the exit path takes, never in play:
  closing the window hard-exits, so probably never in a drive either)
- The game modes' cleanup (`itc_1624`, `itc_1625`): the modes not
  entered, `sub_82246340` ModeType7::ModeType7,
  `sub_82107088` ModeType10::ModeType10, `sub_82106F88`
  ModeType12::ModeType12 (`game-modes.md`)

## Run 2026-10-24 21:33 (`b7-loop-24-10-26-4`)

The three cleanup leads' migration, not in the attract race or the
replay (`itc_1508`, `itc_1509`, ok):

- `sub_8213BDF0` AiPath::JoinCornersByExit, `sub_8213BE80`
  AiPath::JoinCornersByEntry (`ai-drivers.md`'s "Not seen in play"; no
  rows, never seen in play either)

The screen objects' migration 1, not in the attract race or the replay
(`itc_1518`, `itc_1519`, ok):

- `sub_8212EEF8` UvColumn's iterator decrement (`screen-objects.md`'s
  InsertUnique row; no row, never seen in play either)

## Run 2026-10-24 16:01 (`b7-loop-24-10-26-2`)

The array helpers' migration 1 (with the adapters removed), not in the
attract race or the replay (`itc_1342`, `itc_1343`, ok):

- `sub_825B1368` the wide write_char (`c-runtime.md`)

## Run 2026-10-24 03:25 (`b7-loop-24-10-26`)

The graphics resources' migration 1, not in the attract race or the
replay (`itc_1267`, `itc_1269`, ok):

- `sub_822B5EA8` Edram::Free (`graphics-resources.md`; not seen in play
  either)

Migration 2's, not in the attract race or the replay (`itc_1292`,
`itc_1293`, ok):

- `sub_822A4A40` TextureLayout::MipTailOffset (`graphics-resources.md`;
  not seen in play either)

## Run 2026-10-23 20:22 (`b7-loop-23-10-26-5`)

Not in the attract race or the replay (`itc_1124`, `itc_1125`, ok):

(`sub_82145118` PickTarget, `sub_82146220` UpdateState4 and
`sub_82146388` UpdateState5 were seen in migration 2's replay,
`itc_1146`; `sub_82101760` RaceTime::FromTicks, `sub_8213ADF8`
AllSettled, `sub_821451B8` PickTargetBehind and `sub_82146408`
IsSettled in migration 3's, `itc_1170`.)

Migration 2's, not in the attract race or the replay (`itc_1145`,
`itc_1146`, ok):

- `sub_821462A0` AiDriver::UpdateState6 (never seen in play, no row;
  ported with UpdateState, `ai-drivers.md`'s intro)

Migration 3's, not in the attract race or the replay (`itc_1169`,
`itc_1170`, ok):

- Never seen in play, no rows (ported with their callers,
  `ai-drivers.md`'s "Not seen in play"): `sub_8213D898`
  AiPath::AvoidByLane, `sub_8213F7E0` AiSkill::EntryWord,
  `sub_8213F868` AiSkill::EntryValue, `sub_8213AB40`
  AiDriver::SteerIntoCorner, `sub_82139A08` AiDriver::SteerOutOfCorner,
  `sub_8213A200` AiDriver::SteerAtCornerPoint, `sub_82138FA8`
  AiDriver::SteerByRoom, `sub_821392D8` AiDriver::LimitSlide

Migration 5's, not in the attract race or the replay (`itc_1203`,
`itc_1204`, ok):

The racer controllers', not in the attract race or the replay
(`itc_1227`, `itc_1228`, ok):

- Never seen in play, no rows (ported with their callers,
  `racer-controllers.md`'s intro): `sub_8215F5F0`
  RaceFlow::ClearAllRecords, `sub_82160880`
  RaceFlow::PlayersAndAiNoPause

## Run 2026-10-23 17:22 (`b7-loop-23-10-26-4`)

Not in the attract race or the replay (`itc_1043`, `itc_1044`, ok):

- `sub_82108810` Font::LineGlyphs (`strings-and-font.md`; never seen in
  play before either)

Not in the attract race or the replay (`itc_1085`, `itc_1086`, ok):

Not in the attract race or the replay (`itc_1099`, `itc_1100`, ok);
never seen in play before either, ported with Follow:

- `sub_820FB2B0` TrackRoute::FullSearch (`track-route.md`, described
  in `sub_820FB680`'s row)

## Run 2026-10-23 13:44 (`b7-loop-23-10-26-3`)

Not in the drive `itc_1015.log` (2026-10-23) either.

Not in the attract race or the replay (`itc_932`, `itc_933`, ok); never
seen in play before either (the 32-bit and node channel tracks):

- `sub_825A0FE0` TrackSet::Sample32 (`model-animation.md`)
- `sub_825A10A8` TrackSet::Advance32 (`model-animation.md`)
- `sub_825A20E8` CursorCurve::Sample (`model-animation.md`)

Not in the attract race or the replay (`itc_1013`, `itc_1014`, ok):

## Run 2026-10-23 11:06 (`b7-loop-23-10-26-2`)

Not in the attract race or two replays (`itc_922`, `itc_924`, both ok);
their callers are the front end's:

## Run 2026-10-22 14:39 (`b7-loop-22-10-26`, the third run)

Not in the drive `itc_733.log` or any log since (checked 2026-10-23):

- `sub_82126A00` WheelOnSurface8 (probably ground no drive touched)
- `sub_82126A38` WheelOnSurface1Or2 (probably ground no drive touched)
- `sub_82122DB8` FlagsKind (never seen in the coverage session either)
- `sub_82127198` NoSaveFlags (never seen in the coverage session either)
