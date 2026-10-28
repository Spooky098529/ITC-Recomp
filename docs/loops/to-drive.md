# To check in a drive

The functions the B7 loop made native that have no `native running
(game):` line yet: not seen in the validation's attract race or the pad
replay, nor in a drive since. Kept across runs (not started afresh like
`b7-progress.md`); the lead adds each validation's `not in game:` line
here, under its run, with the `replay:` line beside it when the replay
broke. Their harnesses ran our code; this is the validation done by
playing.

The user drives (menus, a race, a save) with `--log_level=info`; a
function goes from this list when a log shows its `native running
(game):` line (say which log in the commit). A run with nothing left
here loses its heading.

## Run 2026-10-28 18:45 (`b7-loop-28-10-26-2`)

- The archive's migration 2 (attract race `itc_2276`, replay
  `itc_2277`, ok): `sub_8209DB90` Archive::CancelQueued, `sub_8209DB20`
  Archive::Cancel (`files-and-saves.md`, `archive.md`; probably a load
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

- The race world's migration 1 (attract race `itc_2139`, replay
  `itc_2140`, ok): `sub_82170A88` SortedList::Clear, `sub_821713C0`
  RaceWorld::ReleaseForStage (`race-world.md`; probably at a race's or
  stage's end; `sub_82116C38` SaveData::AddDistance seen in the replay
  `itc_2204`)
- The race world's migration 2 (attract race `itc_2203`, replay
  `itc_2204`, ok): `sub_82170668` RaceManager::ResetWorld,
  `sub_82171718` RaceManager::ResetVehicles, `sub_82143920`
  RaceEvent::PlaceGroupSplit, `sub_82165E20` RaceStart::Place,
  `sub_821662E0` RaceStart::PlaceRolling, `sub_82172D20`
  RaceManager::StreamTwoPlayers (`race-world.md`; a restart, other
  race kinds, two players)
- The race's states' migration 1 (attract race `itc_2235`, replay
  `itc_2236`, ok; the replay's race takes the free-start path; seven
  more seen in the replay `itc_2253`): `race-states.md`: `sub_821649B8`
  RaceProgress::SetChecks, `sub_82163440` ReleaseRacers, `sub_82163140`
  TickFraction, `sub_82162D80` Rank, `sub_821622A8`
  RoutePosition::NotAfter, `sub_82164090` CheckLap, `sub_82162230`
  Time::Sub, `sub_82164E18` CheckFinish, `sub_821632C8`
  RaceStates::OnRollingStart, `sub_821643B0` OnStandingStart,
  `sub_82163348` ClearStep, `sub_82163378` OnQuit, `sub_821647B0`
  OnReplay, `sub_82168500` QuitTick, `sub_82167970` AfterRaceTick,
  `sub_82166E50` RollingStartTick, `sub_82165AB0` RacersNear,
  `sub_82165C78` RacersClose, `sub_82164A90` RacersStill,
  `sub_821674A0` CountdownTick, `sub_821692C8` StandingStartTick (a
  race with a countdown, racing to the finish, quitting from the pause
  menu, the menu after a race)
- The race's states' migration 2 (attract race `itc_2252`, replay
  `itc_2253`, ok): `sub_82163000` (two sections joined by a forward
  link's side link), `sub_82162330` RaceProgress::RouteDistance
  (`race-states.md`; `CheckLap`, `CheckFinish`, `Rank` above)

## Run 2026-10-25 23:42 (`b7-loop-25-10-26-4`)

- The race's set-up's migration 1 (attract race `itc_1867`, replay
  `itc_1868`, ok): `sub_82101630`, `sub_82101700` Time::Pack
  (`race-setup.md`)
- The race's set-up's migration 2 (attract race `itc_1890`, replay
  `itc_1891`, ok): `sub_82101E90` Stages::SetRoute, `sub_82112DB0`
  Race::SetUpRival (`race-setup.md`; `sub_82101630` again, above)

## Run 2026-10-25 16:38 (`b7-loop-25-10-26-3`)

- The DrawTexts lead's migration 2 (attract race `itc_1729`, replay
  `itc_1730`, ok): `sub_82211A28` RivalLogo::Draw (`race-hud.md`; runs
  only with a rival at the race start)
- The UI movies' ready groups (attract race `itc_1740`, replay
  `itc_1741`, ok): `sub_8211B160` (the pairs vector's growth),
  `sub_8211C4F0` (a text's scroll delay and speed), `sub_8211D880`
  UiMovie::RevealText, `sub_8211D8D8` (whether a text has another
  page) (`ui-movies-game.md`)
- The scenery lists (attract race `itc_1769`, replay `itc_1770`, ok):
  `sub_82146BD0` SceneryModel::RecordShape, `sub_82148058`
  PointList::Draw (`scenery-lists.md`)
- The scenery lists' migration 2 (attract race `itc_1777`, replay
  `itc_1778`, ok): `sub_82595550` Model::RecordShape (`models.md`; its
  work runs in play inside `Model::DrawShape`, which calls
  `ModelRecordShapeChecked` past the print; only its own adapter, from
  `sub_820EF790`, never ran)
- The shader objects' migration 3 (attract race `itc_1852`, replay
  `itc_1853`, ok): `sub_8210A5C0` Strings::PageCount
  (`strings-and-font.md`)

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
- The minimap (attract race `itc_1648`, replay `itc_1649`, ok):
  `sub_82147D18` SetList::Point (`minimap.md`)
- The `atexit` step (attract race `itc_1665`, replay `itc_1666`, ok):
  `sub_820F4BA8` Minimap::DrawPointMarker (`minimap.md`); `sub_825B3430`
  _lock and `sub_825B32D0` _unlock (`c-runtime.md`: their plain C++
  runs only for lock 8, which only the exit path takes, never in play:
  closing the window hard-exits, so probably never in a drive either)
- The game modes' cleanup (`itc_1624`, `itc_1625`): the modes not
  entered, `sub_82106F08` ModeType5::ModeType5, `sub_82235E88`
  ModeType6::ModeType6, `sub_82246340` ModeType7::ModeType7,
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

- `sub_8213C358` AiPath::PreferredSections (`ai-drivers.md`)

(`sub_82145118` PickTarget, `sub_82146220` UpdateState4 and
`sub_82146388` UpdateState5 were seen in migration 2's replay,
`itc_1146`; `sub_82101760` RaceTime::FromTicks, `sub_8213ADF8`
AllSettled, `sub_821451B8` PickTargetBehind and `sub_82146408`
IsSettled in migration 3's, `itc_1170`.)

Migration 2's, not in the attract race or the replay (`itc_1145`,
`itc_1146`, ok):

- `sub_82139E40` AiDriver::SetState2or3 (`ai-drivers.md`)
- `sub_82145F48` AiDriver::UpdateState2 (`ai-drivers.md`)
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

- `sub_8213A000` AiDrivers::StartGroup (`ai-drivers.md`)

The racer controllers', not in the attract race or the replay
(`itc_1227`, `itc_1228`, ok):

- Never seen in play, no rows (ported with their callers,
  `racer-controllers.md`'s intro): `sub_8215F5F0`
  RaceFlow::ClearAllRecords, `sub_82160880`
  RaceFlow::PlayersAndAiNoPause

## Run 2026-10-23 17:22 (`b7-loop-23-10-26-4`)

Not in the attract race or the replay (`itc_1043`, `itc_1044`, ok):

- `sub_8210A5C0` Strings::PageCount (`strings-and-font.md`)
- `sub_82108810` Font::LineGlyphs (`strings-and-font.md`; never seen in
  play before either)

Not in the attract race or the replay (`itc_1085`, `itc_1086`, ok):

- `sub_820F8C58` TrackRoute::StepAlongPath (`track-route.md`)
- `sub_820F9F58` TrackRoute::NormalizeForwardPath (`track-route.md`)
- `sub_820FA0F8` TrackRoute::NormalizePath (`track-route.md`)

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

- `sub_820AAE48` Viewport::Set (`cameras.md`)

## Run 2026-10-23 11:06 (`b7-loop-23-10-26-2`)

Not in the attract race or two replays (`itc_922`, `itc_924`, both ok);
their callers are the front end's:

- `sub_821171B0` DataTables::Find20 (`data-tables.md`)
- `sub_82117210` DataTables::Entry20 (`data-tables.md`)

## Run 2026-10-22 14:39 (`b7-loop-22-10-26`, the third run)

Not in the drive `itc_733.log` or any log since (checked 2026-10-23):

- `sub_82126A00` WheelOnSurface8 (probably ground no drive touched)
- `sub_82126A38` WheelOnSurface1Or2 (probably ground no drive touched)
- `sub_82122DB8` FlagsKind (never seen in the coverage session either)
- `sub_82127198` NoSaveFlags (never seen in the coverage session either)
