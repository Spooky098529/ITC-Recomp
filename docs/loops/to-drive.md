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
