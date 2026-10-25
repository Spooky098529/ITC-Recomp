# For the user's review

What the B7 loop's runs left for the user to review before a merge: the
function files' rows added or changed, and the choices made in them
(behavior kept or left on the recompiled path, corrections to a map).
Kept across runs (not started afresh like `b7-progress.md`), newest run
first; the lead adds to the current run's section as reports come in.
Harness groups expected to differ are in `expected-differences.md`, the
functions to check in a drive in the progress file.

When the user has reviewed a run's section, its heading gets
"(reviewed <date>)"; the section stays for the record.

## Run 2026-10-25 03:59 (`b7-loop-25-10-26-2`)

- `game-modes.md` (new, 7871fe6): 31 rows (3 moved from
  `functions.md`, 28 new): the game's modes (`Mode::Create`,
  `Modes::Tick`), the race mode (type 1, 167,264 bytes; constructor
  `sub_821073B8`, destructor `sub_82107518`), its message dispatcher and
  state machines (`StateMachine::Handle`, `Find`, `IndexOf`), the race
  overlays' state machine set-up `sub_8215E6C8` (33 transitions listed)
  and its four handlers, and the race-only `Buffer28::` functions in the
  camera modes' range. `known-data.md`: the mode, race mode, state
  machine and transition layouts, `0x826032DC`/`0x826032E0`,
  `0x826C5B68`; the race flow's row now says it's the race mode's
  +97548. `leads.md`: 2 leads added, notes on the camera modes and
  framework objects leads.
- `game-modes.md` migration 1 (21ff7d5..c504e59; `src/game_modes/`,
  `src/hooks/game_modes.cpp`, harness `src/harnesses/game_modes.cpp`):
  21 of 31 ours, `--native_game_modes`. For review:
  - `RaceOverlaysMachine::Init` (`sub_8215E6C8`) replays the original's
    377 stores and copies in the same stack frame (`r1 - 2272`, a table
    read off the generated code), so the transitions get the same three
    unwritten functor storage words (stack leftovers; nothing reads
    them; as track zones did).
  - `StateMachine::Find`'s default is returned as
    `0xFFFFFFFF826C5B68` in r3 (sign-extended, as the original).
  - `Transition::Make` (`sub_82105FE0`) and `Transition::Assign`
    (`sub_82106100`) are Merged into `StateMachine::Add`; their own
    harness cases went with them (Add's cover them).
  - New names: `RaceOverlaysMachine::SetFlags`, `ClearFlags`,
    `SetFlag820`, `ResetOverlays`, `Member67760::Member67760`,
    `Member13360::Member13360`, `Buffers28::~Buffers28`.
- `models.md` (f28c6f7): 16 new rows for the model class
  (`Model::Model`, `~Model`, `Model::Release` read), the texture
  provider and its `std::vector` helpers, `XPR::~XPR`, `XPR::Release`,
  the put-off frees (`Memory::FreeLater`, `FreePhysicalLater`, the
  pending frees list at `0x8296BD54`, freed by `sub_8258E1D8` each
  pass); `memmove_s` in `c-runtime.md`. `known-data.md`: the texture
  provider, the `XLMM` block header, `0x8296BD38`, `0x8296BD54`.
  `leads.md`: one lead (`sub_820C5008`/`sub_820C5168`), notes on three.
  The map stopped on the memory blocks' lock; the user's call
  (2026-10-25): port it too (a host `std::mutex`).
- `models.md` migration 1 (6148c1e..b35547a; `src/models/`,
  `src/hooks/models.cpp`, harness `src/harnesses/models.cpp`): all 17
  rows ours, `--native_models` (`memmove_s` under `--native_c_runtime`).
  For review:
  - The memory blocks' lock is a `std::recursive_mutex`, not a plain
    `std::mutex`: the critical section it replaces is recursive, and the
    harness holds it around its cases on the game's own pending-frees
    list while the functions take it again. No user in play takes it
    twice.
  - Every user of `0x8296BD38` takes it: `FreeLater` and
    `FreePhysicalLater` directly; the recompiled ones (`sub_8258DFF0`,
    `sub_8258E098`, `sub_8258E138`, `sub_8258E1D8`) through `ScopedLock`
    (`sub_8258E580`, `sub_8258E5B8`, `video-output.md`), overridden to
    take the host lock for this critical section only (every other one
    goes to `__imp__`). Host lock or guest critical section is chosen
    once, from `--native_models` at the first call.
  - New read-only helpers in the model animation code for
    `Model::~Model`'s checks: `TrackSetReleasable`,
    `SkeletonDestructible`.
  - An `UninitializedCopy` harness case with a null destination was
    fixed in the case (the original checks null per slot and writes
    from address 4), noted in the row.
- `game-modes.md` cleanup (no mapping; ce8d477..4ff0b54): 38 of 42
  ours. For review:
  - 11 new rows read on the way: `Mode::Create`'s mode constructors
    (types 2-7, 9, 10, 12) and `MachineMember::MachineMember`
    (`sub_82108160`); `sub_821077EC`, a piece of `Mode::Create`'s switch
    in its stack frame, Merged into it; type 8 (`sub_82107670`, never
    seen in play) still runs the original.
  - `sub_82106A50`, `sub_82107168`, `sub_82108160` and the mode
    constructors are Native with no adapter (only native code calls
    them).
  - `race-overlays.md`'s `RaceOverlays::Clear` (`sub_8215DE80`) adapter
    removed (RaceMode::RaceMode its only caller in play).
  - The pending-frees harness helpers moved to
    `src/harnesses/pending_frees.h`, shared by the models and game modes
    harnesses.
  - The first batch's two groups in one commit (ce8d477); one counter
    race for the six removals, each then built and committed alone.
- `mode-tick.md` (new, 2a11eec): 23 rows, what `Modes::Tick` calls
  besides the mode's slots: the per-mode settings (`Modes::SetChecks`,
  `SetSceneTargetMode`, `Presence::SetForMode`, presence probably
  Dropped at `Modes::Tick` by B5's call), the sign-in watch
  (`SignInWatch::Update`, `CheckIdle`, `OnModeChange`; its nine prompt
  handlers never ran, no rows), `Billboards::FlipRing`, the screen fades
  (two layers at `0x826A50AC`), `Music::Play` (`BGM_SYSTEM`,
  `bgm_tmp_01`-`17`). `game-modes.md`: `Modes::Modes` (`sub_821067F8`)
  added. `known-data.md`: their fields and globals. `leads.md`: the save
  data's creation and deletion. The lead's "loading screen" guess was
  wrong. Also: `models.md`'s `TextureProvider::Clear` row reworded ("it
  never reaches `memmove_s`") so `b7_status.py` doesn't read it as never
  called.

## Run 2026-10-25 01:17 (`by-loop-25-10-26`)

- `framework.md` (mapped in c4b5b86, last run; its rows are in that
  run's section below): map checked this run (0 errors, 68 of 68 in
  play). Note: `b7_status.py` flags `movies.md:108` `sub_82108018`, whose
  row says never called but which ran (the user's movie work, not this
  run's).
- `framework.md` migration 1 (f27748b..e080edd; `src/framework/`,
  `src/hooks/framework.cpp`, harness `src/harnesses/framework.cpp`):
  50 of 68 ours, 15 adapters removed (Native), `--native_framework`.
  For review:
  - The map's rotations were swapped: `sub_82092B10` is
    `HandlerMap::RotateLeft`, `sub_82092A00` `RotateRight`; corrected.
  - Paths handed to the original (never seen in play): InsertN's
    copy_backward path (316 Insert/InsertN cases go there, logged) and
    `ApplyRequest`'s removal types 1 and 3.
  - `TickHandler::Invoke` (`sub_82099A98`) and `sub_82096340`, Wrapped
    before: their existing overrides (`game_messages.cpp`,
    `frame_state_diff.cpp`) now call the native path (Overridden).
  - `DebugText::DebugText` stores an uninitialized word of its own stack
    frame as the job's state; the adapter reads and stores the same word.
  - `sub_82094350`'s adapter stays: `sub_820A8508` calls it through a
    vtable.
  - Not compared in the harnesses: InsertN's r3, r4-r11 after
    `Framework::Framework` and `Create`, a map head's key and delegate,
    the nodes' padding bytes +22/+23 (nothing sets or reads them).
- `framework.md` migration 2 (eb25d68, ad01ea7; `src/framework/main_loop.cpp`):
  the main group native (`main`, `sub_82090378`, `sub_8209ABE8`,
  `App::Run`, `ForEachTickHandler`, `ForEachRequest`), 7 adapters removed;
  56 of 68 ours, nothing ready left. For review:
  - Every virtual call and functor stays a guest call, from frames like
    the originals' (their return addresses and stack words); tick
    handlers still go through `TickHandler::Invoke`'s override.
  - The native loop sets r26 to r31 as the recompiled `App::Run` keeps
    them, and puts them back on return: a recompiled device setter reads
    r27's high word (`Device::SetDefaultState`'s harness differed without).
  - The originals' traps (tw 22) log a warning and go on, as the
    recompiled code does; none seen in play.
  - `main`'s harness makes the read-only page of its framework vtable
    (`0x82000B90`) writable through the SDK's `rex::memory::Protect` for
    its test slots, then puts the access back.
  - `FromFvf`'s and `SetFVF`'s pad byte groups now differ (400, 100):
    `expected-differences.md`.

## Run 2026-10-24 21:33 (`b7-loop-24-10-26-4`)

- Cleanup lead `sub_8209F430` ActionMappers::Init (`action-mappers.md`;
  3ac7be7, 9e36924): plain C++ `InitCreate`
  (`src/action_mappers/action_mappers.cpp`), under
  `--native_action_mappers` (needs `--native_heap`); 24 cases in the
  `action_mappers_binding` harness. 7 adapters removed (Native:
  `sub_8259C868`, `sub_8259C898`, `sub_8259C818`, `sub_8209F228`,
  `sub_8210AEB0`, `sub_8210AF00`, `sub_8210C180`). For review:
  - Init takes its five blocks from operator new first, in the
    original's order; if one isn't given, it frees them all and runs the
    original.
  - Its adapter runs BindAll's adapter inside the original's 112-byte
    frame; BindAll stays Overridden (a guest call to the owner's slot
    +36: `sub_820944C8` -> `sub_82094350` -> probably `sub_82090788`,
    the app's, unmapped).
  - The harness cases call Init's adapter (it makes guest calls), and
    give both versions the same operator new blocks with the same
    random leftovers.
  - Kept though `--removable` lists them: `sub_8210C370`,
    `sub_8258CF10`, `sub_825924E0` (the fallback's `--log_pause_menu`
    and `--log_input_reads` diagnostics), `sub_820991E0` and
    `sub_8210AF08` (guest calls through `ctx`).
- Three cleanup leads (2e5ce7c..b6888ac; `race-overlays.md`,
  `ai-drivers.md`, `c-runtime.md`):
  - `sub_8212BFE8` ParticlesB::ParticlesB and the deleting destructors
    `sub_8212B358`, `sub_8212C070` Overridden; `sub_8212B288`,
    `sub_8212BF88` Native from the start (only the native deleting
    destructors call them). The deleting destructors run natively only
    with `--native_heap`, and fall back to the original (bit 0 set) for
    a block the allocation layer didn't give. They're called from the
    recompiled CarOverlays::DeleteChildren at a race's end, so attract
    races don't reach them.
  - `sub_8213BDF0`, `sub_8213BE80` (JoinCornersByExit, ByEntry; never
    seen in play, no rows) native inside AiPath::JoinCornerPair;
    PathCalls' join callback gone. Like the other native joins, they
    don't write the original's fcfid scratch below the stack pointer.
    What's left of PathCalls is the +5532 normalizer's other targets
    (never in the game), so UpdateIdle's (`sub_821399A8`, listed
    removable) and DriveCar's adapters stay.
  - Flush-to-zero: the problem was real. ThinLTO had moved
    Smoke::InRegion's second `dpps` past the control word's restore: a
    new "tiny values" group had 135 of 400 differing. Fixed as in
    ee810a9 (a `Flushing()` helper, `vector_library::PinInMode`
    barriers); 0 differing since. The barriers changed Sparks::Emit's
    `mulps` operand order (14 NaN-only differences), so it now picks
    the kept NaN explicitly (`MulKeepingFirstNaN`); Sparks::UpdateStreak
    still relies on a plain `addps` that happens to match.
  - A latent harness input: with the shared random stream shifted,
    AiDriver::DriveCar's case 540 made the recompiled side fault without
    end (guest `0x7028C0CC`; `itc_1497`, `itc_1498`). The new groups
    were put last so earlier inputs stay as before, but DriveCar's case
    generation can make an input the original faults on.
  - `sub_82138D00` SetParams, `sub_82139DA0` StartRace,
    `sub_82139580` ResetControl: adapters removed (Native).
- `screen-objects.md` (new, mapped in 78f986b): 42 rows (1 moved from
  `functions.md`: `sub_82130978`, now `ScreenObject::Record`, Wrapped),
  the race's screen object (128,000 bytes, one a view: a 1024-wide
  A8R8G8B8 texture, a 720-triangle fan with `rand`-jittered UVs and 20
  rings of 36 points; strength = speed km/h / 300 at +76; probably a
  screen blur while the nitrous runs, started and stopped by the race's
  update from the vehicle's byte +2424). `Start` and `Stop`
  (`sub_8212EDF8`, `sub_8212EED0`) never ran, so `Record`'s 1,038-line
  body never ran either. For review:
  - Names on guesses: the `ScreenObject::` functions, `UvMap::` and
    `UvColumn::` (the two map types), the nitrous trigger, +84 as the
    sort key (599/601).
  - Correction: the lead said `std::string`-keyed trees; the keys are
    doubles (the string is only the `length_error` message).
  - `sub_82131680` (`UvColumn::Insert`) and `sub_82131FB0`
    (`UvMap::Insert`) mapped here, not under the containers' lead (only
    the screen objects use these two map types).
  - `post-effects.md`'s intro and `sub_8215B690`, `sub_8212EDE0` rows
    now point here; `known-data.md` gets the screen object and the race
    object's +31152/+31156/+31160.
  - Left open: the objects at +88 and +92 (set through vtable calls in
    `Record`; who makes them isn't known).
  Migration 1 (472d5ac..4e3513e): 38 of 42 native under
  `--native_screen_objects` (`src/screen_objects/`, adapters in
  `src/hooks/screen_objects.cpp`, harness `screen_objects`; the tree
  functions also need `--native_heap`, `~ScreenObject`
  `--native_video_output`), 8 adapters removed. For review:
  - `sub_8212EEF8` (UvColumn's iterator decrement, never seen in play,
    no row) ported so the native InsertUnique has no recompiled callee;
    its own harness group.
  - Handed to the original from the start: the trees' `bad_alloc` and
    `length_error`; erasing part of a tree (`sub_8212F030`,
    `sub_8212EFA8`, `sub_82132318`, `sub_821318B8`, never ran); a
    checked iterator's trap; SetSpeed while +24 is set (calls
    `sub_8212F778`, never ran); BuildRings over a map that isn't empty.
  - Inside a native caller, an operator new or delete refusal after the
    checks, or a broken tree, is logged and aborts (as
    `std_list/ptr_list.cpp`).
  - AddRingPoint's r3 set to what the original leaves (no caller reads
    it), compared only by kind for the freed head; its temporaries live
    in the original's 272-byte guest stack frame.
  - Two corrected notes: both AllocateNodes rows said 0 returns 0 (it
    calls operator new(0)).
  - Left: `rand` (`sub_825A9BB8`, the core's thread data) blocks
    BuildFan, Init and the constructor; `sub_822A8980` and the shader
    pairs not taken (they unblock nothing yet); `Record`'s body never
    ran, so no frame dump was compared.
- `framework.md` (new, mapped in c4b5b86): 68 rows (5 moved from
  `functions.md`: `main` `sub_82090108`, `App::Run` `sub_8209A760`,
  `ForEachTickHandler` `sub_82099D50`, `TickHandler::Invoke`
  `sub_82099A98`, `sub_82096340`), the app's framework (its services
  made through factories at boot, the run loop and its tick handlers);
  `known-data.md`'s framework, run loop, app (partial), `0x8296B910`,
  `0x8296B914`, `0x8266D080`, `0x8296BCE0`; 5 leads added. For review:
  - BindAll's slot +36 worked out: `sub_820944C8` -> `sub_82094350` ->
    `sub_82090830` -> `sub_82090778`, the input manager at framework+88
    (natively `[[owner + 8] + 52]`), not `sub_82090788` as the lead
    guessed (that's framework+124, the save devices); corrected in
    `action-mappers.md` and `input.md`.
  - Names on guesses: `Framework::`, `RunLoop::`, `HandlerMap::`
    (probably `std::map<unsigned, Delegate*>`), `RequestVector::`
    (probably `std::vector`), `FrameStart`, `DebugText`,
    `DebugFont::SetPosition`, `PoolMaker::PoolMaker`, the factories
    (`Users::`, `SaveDevices::`, `WorkerPool::`, `Renderer::`,
    `Scene::`, `App::Create`).
  - Corrections: `App::Run`'s "quit flag at this+12" is the tick
    handler map's size; `TickHandler::Invoke`'s r3 is the run loop;
    `system-services.md`'s "app" (`App::ReadUsers`, `App::PollSignIn`)
    is the users and sign-in object at framework+128 (noted in
    `framework.md` only, not renamed).
  - Taken from other leads: `sub_82093400`, `sub_82093740` (the
    containers'), `sub_820944C8`, `sub_82094350` (the app's start-up);
    both lines updated.

## Run 2026-10-24 17:46 (`b7-loop-24-10-26-3`) (reviewed 2026-10-24)

- `post-effects.md` (new, mapped in f3643e1): 57 rows (3 moved from
  `functions.md`: `Glow::Record`, `PostEffects::RecordAll`
  (`sub_82133960`, unnamed before), `sub_821363F0`), the post effects
  (up to 15 screen-space effects a view, objects at `0x826C5D80` made
  from the table at `0x826174A8`: the glow, probably a color filter, the
  motion blur, a two-pass filter over render target 3 or 4, two kinds
  never run); `known-data.md`'s "Values from the image" (the three
  `.data` tables, float constants); 3 leads. For review:
  - `sub_821363F0` renamed from `View::UploadPreviousViewProjection` to
    `MotionBlur::Record` (it records the whole motion blur), and the
    mention in `matrix-math.md`'s `Matrix::Copy` row changed to match.
  - `sub_8212EDE0` belongs to the race's screen objects (made by
    `sub_82133000`, now a lead), kept here beside its caller
    `sub_8215B690` (probably the pause toggle).
  - The glow's tables at `0x82617280` and `0x826171D8` probably the
    DirectX SDK HDR sample's glare library (`GLAREDEF`/`STARDEF`).
  - `RenderTextures::Acquire` (`sub_82133190`) returns an uninitialized
    stack word when an id is in use with another key (split screen's
    second player, probably; not checked whether it runs).
  Migration 1 (ece3c32..66b8f81): 43 functions native under
  `--native_post_effects` (`src/post_effects/`, adapters in
  `src/hooks/post_effects.cpp`, harness `post_effects`), 9 adapters
  removed. For review:
  - Stack leftovers below the stack pointer not reproduced
    (RecordTexelSize's c32, StarDef::Init's colors, AddQuad's integer
    conversions); nothing reads them.
  - The recording helpers check for room first; with the list full the
    whole original runs (its message and write through address 0); the
    command list's diagnostics (`--log_slot_callers`) skipped, as in
    earlier native recorders.
  - RecordBeginStates and RecordEndStates allocate one slot more than
    they write (24 and 13), as the original.
  - Left on the recompiled code behind fallbacks: the release paths and
    deleting destructors when a block isn't the allocation layer's or
    `--native_c_runtime_heap`/`--native_heap` is off; ReleaseSurface
    without `--native_video_output`; TargetFilter::TargetFilter where
    memcpy's copy can't be native or without
    `--native_graphics_resources`/`--native_heap`.
  - Frame interpolation's overrides untouched (`Glow::Record`, the
    motion blur's copy at 8213653C, `PostEffects::RecordAll`).
  - `RenderTextures::Acquire` not ported (blocked by `sub_8235A9F8`):
    its uninitialized-word path is still the original's.
  - The destructors, release paths and DestroyRace run only when a race
    ends: attract races don't reach them (harness only).
  - New helpers in `src/hooks/post_effects.h`: `PostEffectsReleaseTexture`,
    `PostEffectsReleaseSurface`, `PostEffectsDestroyCalls`.
- Cleanup leads (no mapping; 1197f81..cdaa66d): `sub_820BAAA0`
  CollisionShape::~CollisionShape's adapter removed (`collision.md`);
  `AiDrivers::Destroy` and `DestroyRings` native under
  `--native_ai_drivers` (`ai-drivers.md`; fall back while
  `--native_heap` is off or the table isn't the layer's; run only when a
  race ends: harness only); the UI movie walk native under
  `--native_ui_movies` (`ui-movies.md`, a new Merged row `sub_8259D730`
  the clip intersection, never in play, ported into ApplyTransform;
  `src/ui_movies/draw.cpp`, strict fp). For review:
  - `UiMovie::Draw` stays Overridden (its work uses `ctx`, inside the
    `--log_pause_movie` watch override); DrawPicture's and
    DrawPrimitive's adapters stay (a guest call to `sub_8259EBE8`)
    though `--removable` lists them; DrawNode reaches them through a
    callback from the original's 272-byte frames.
  - Frame interpolation kept: every Matrix::Multiply and Copy in the
    walk noted through `BlendState_OnMultiply`/`OnCopy` with the
    original's return address; Vec4::Transform through
    `debug::Vec4Transform`. Frame dumps match `--native_ui_movies=false`
    (2,414 draws).
  - Left on the recompiled code: the text list's grow `sub_8259EC58`
    (never in play: a full list runs the original append or QueueText);
    a picture without a texture runs the original DrawPicture.
  - Float words copied bit for bit where the original pairs lfs/stfs
    (a signaling NaN keeps its bits).
  - The DrawPicture, DrawPrimitive and tree harnesses compare bytes
    exactly instead of with `harness::Outputs`.
  - Found along the way: `sub_8210A758` Text::Draw's harness compares a
    slot word as bytes outside the vertex ranges it marks as floats
    (a NaN sign showed as 1 difference when its random inputs changed);
    the new cases now use their own `Random`, Text::Draw back to 0. A
    gap in that harness worth a later look.
- `input.md` (new, mapped in 167d407): 29 rows, the input manager and
  input processors (B1's 13 input rows moved from `functions.md`, kept
  with "B1's native code" or "Read (2026-10-24)" added; four unnamed
  ones named `AnyPressed`, `PressedOrRepeat`, `TestPressed`,
  `TestRepeat`, the getters `LogicalPad::Values` and
  `PhysicalPad::Values`); 1 lead (the app framework). For review:
  - The manager's vtable `0x82000F48` read from the image: slots 1
    `GetProcessor`, 2 `GetLogicalPad`, 3 `AddListener`, 5
    `GetPhysicalPad` in play.
  - Four `std::list` of pointers helpers mapped here as `PtrList::`
    (`sub_820AD058`, `sub_82099C08`, `sub_82095958`, `sub_820958B8`;
    the last taken from the C++ containers lead, which says so); shared
    with other subsystems (the sound manager).
  - Correction to the action mappers' map: the object
    `ActionMappers::Init` is given is probably the app object from
    `sub_82106548` (its slot +36 gives the input manager), not the input
    manager (`known-data.md`'s `0x826BAB78` row, `input.md`'s intro;
    `action-mappers.md`'s rows unchanged, its pointers moved to
    `input.md`).
  - The processor's constructor sets the repeat period to 4; the
    mappers' repeat objects use 3 (both as read).
  Migration 1 (57373be..4dd7a11): all 29 native under `--native_input`
  (`src/input/input_manager.*`, `src/std_list/ptr_list.*` for other
  subsystems to call, adapters in `src/hooks/input_manager.cpp`), with
  `ActionMapper::ActionMapper`, `ActionMapper::Bind` and
  `InputClientList::BindAll` (`action-mappers.md`, under
  `--native_action_mappers`); 16 adapters removed. For review:
  - A harness depends on the heap's layout: with the binding cases in
    the `action_mappers` harness, the later `strings_and_font` harness
    failed (`Font::Draw` differed, then its swprintf cases crashed on a
    read of guest 0x5E44, or a call to 0x00200020), and the race didn't
    start; one `memory::Alloc(0x4400)` and its `Free` before it is
    enough. Probably the same as the eleventh run's swprintf crash with
    other random inputs (above). Not investigated; worked around by
    putting the binding cases in their own harness,
    `action_mappers_binding`, run last.
    Looked into (the user's call, 2026-10-24): not the heap's layout
    (an Alloc and Free moved nothing) but the harnesses' one shared
    random generator: the binding cases drew from it, and later
    harnesses got other inputs. The crash was `swprintf`'s format
    generator (a bare "I" then '%' starts a new conversion; the original
    and the native agree), the differences NaN signs in vertices the
    drawing cases compared as bytes (clipped glyphs'). Fixed in the
    harnesses (`strings-and-font.md`'s intro): each harness starts its
    own generator, seeded from its name; `action_mappers_binding` runs
    after `action_mappers` again; 12 other seeds 0 differing.
  - Kept as guest calls (the functions stay Overridden): the manager's
    constructor's two handler registrations through the dispatcher's
    slots +8 and +4 (from the original's 192-byte frame); `BindAll`'s
    app slot +36 (logged: `sub_820944C8`, through `sub_82094350`,
    probably to `sub_82090788`; the input manager each time, players
    0-3).
  - Left on the recompiled code: `AddListener` on a list with a listener
    already (its reset `sub_82592400` never runs); `AllocateNodes`'
    `bad_alloc` and `IncSize`'s `length_error` paths (the latter doesn't
    throw in the original: a case past the limit crashed in it, so the
    cases stop at the limit); the constructor and Bind with vtable slots
    other than those seen in play; anything allocating with
    `--native_heap` off.
  - The `--log_input_reads` wrappers moved from
    `src/frame_pacing/game_messages.cpp` into the new adapters (same
    counts).

## Run 2026-10-24 16:01 (`b7-loop-24-10-26-2`)

- `c-runtime.md`: 2 new rows (mapped in 3a15cda), `sub_820911A8` and
  `sub_820900C0`, probably the compiler's `vector constructor iterator`
  and `vector destructor iterator` (no exception handling, so not the
  library's `__ehvec_*`), added to the C runtime file instead of a new
  one, with an intro paragraph; `known-data.md` (the race data's
  +0x59AEC records, `0x826D82D4`); 2 leads (the race object at
  `[0x826D82D4]`, `sub_8217C708` with `sub_821DF350`) and the race data
  lead extended with `sub_82101D20`.
  Migration 1 (f624a82, 93e739f): both helpers native under
  `--native_c_runtime` (`src/c_runtime/arrays.cpp`). For review: the
  native targets (`SetWsToOne`, `Particle::Particle`) called as plain
  C++, the empty destructor `sub_82106540` skipped, any other target
  (`sub_82101D20` in play) a guest call through an `ElementCall`
  callback from a 128-byte frame like the original's; r3 left as it
  came with the empty destructor (volatile, no caller reads it); a count
  of 0x80000000 wraps to 2^31 calls in both, left out of the harness.
  Adapters removed: wcslen, mbtowc, the wide write_char (Native);
  memcpy_s's (`sub_825A7270`) kept, its work a guest call to memcpy's
  override. To look at later: the new cases first drew from the shared
  `h.random`, which shifted later harnesses' inputs, and the
  strings-and-font swprintf harness (`sub_8210A490`) then crashed (guest
  writes at 0x70000000, probably an endless loop; `itc_1337.log`): a
  swprintf case, or the native swprintf, fails on other random inputs.
  The new cases use their own generator.
- `course-scenery.md` (new, 71 rows; 3 moved from `functions.md`:
  `Lights::FindNearest`, `TextureAnimations::Advance`, `sub_820F6068`
  now `Scenery::Draw`; 68 new; mapped in 043a0d2): the course's areas,
  their models and lights collected around each viewport and streamed
  through `Archive::Load` into five pools of physical memory (manager at
  `[0x826C5514]`/`[0x826C551C]`), the scenery's draw and the cars'
  headlight projectors (`sub_820F34C8`). Four rows read only in outline
  (`sub_820F6068`, `sub_820F3B70`, `sub_820F34C8`, `sub_820F52A8`);
  `known-data.md` 7 rows; 5 leads.
  Migration 1 (551a868..e7309a0): 37 of 73 ours under
  `--native_course_scenery` (`src/course_scenery/`; `sub_82146B20`,
  `sub_82146AC0` moved from `functions.md`, Native), six adapters
  removed. For review: the native key and reset slots called directly,
  other targets and every load slot through a `Calls` callback from a
  frame laid out like the original's (the slot adapters stay for the
  vtables); `Scenery::LimitLargeModels` (`sub_820F2B80`) reads past its
  5-entry stack table for an area above 4, matched by running
  `__savegprlr_27` first and keeping the table in guest stack memory;
  `ObjectPools::Init` and `Shutdown` native only with the native heap
  on; `TextureAnimations::Advance`'s render-only skip in
  `per_frame_logic.cpp` kept where it was; frame dumps with the flag on
  and off had the same 2,414 draws. Left: blocked on the critical
  section wrappers, `MmQueryAllocationSize`'s, memset,
  `DataFile::GetEntry`, the XPR, the archive's queue and the texture
  provider.
- `c-runtime.md`: `sub_822962B0` memset (probably XAPI's `XMemSet`; new
  row, a212f92: vector stores and `dcbzl` for whole 128-byte lines, the
  same bytes as a plain memset; its 16-byte loops step by the word at
  `[0x8263830C]`); `heap.md`: `sub_822965C0` (probably `XPhysicalSize`,
  over MmQueryAllocationSize) and `sub_822965D8` (probably
  `XQueryMemoryProtect`, over MmQueryAddressProtect), kept for the core;
  1 lead (`sub_82562FF0`). `atexit` left out of this subsystem (its
  critical sections).
  Migration 1 (671f5d0..b549e6c): XMemSet native under
  `--native_c_runtime`, with what it unblocked: `Scenery::Init`
  (`--native_course_scenery`), `CameraPasses::Init` and `InitPasses`
  (`--native_cameras`); `InitPasses`' and `SetMode`'s adapters removed.
  For review: `[0x8263830C]` logged as 16 (nothing in `generated/`
  stores there), so XMemSet is one host memset at stride 16 and follows
  the translated steps for any other stride (harness cases for 0, 8, 20,
  32, 48); the scratch below r1 not written; `CameraPass::Init`'s adapter
  has a thread-local test hook (`hooks::camera_pass_init_calls`, null
  outside the harness) so the harness can record its calls;
  `Scenery::Init` leaves r3 as the original does. `Viewports::Init` and
  `Viewport::Init` still wait on `atexit`.

## Run 2026-10-24 03:25 (`b7-loop-24-10-26`)

- `device-state.md` (new, 131 rows; 11 moved from `functions.md`, 120
  new; mapped in 8b1b86b), `known-data.md` (the setter tables at
  `0x826388F8` and `0x82638D78`, the device's fields); the blocker notes
  in `graphics-resources.md` and `video-output.md` point to it; 1 lead
  (the command buffer, `8229DC28`-`8229F228`). For review: the
  shadow, dirty masks, fetch constants, bound objects, scissor and the
  predicated tiling bit (+10808 bit 1) Kept (the renderer reads them);
  the packets written straight into the command buffer (the screen
  scissor, binning and WAIT_UNTIL, the GPU register split and its dummy
  draws, gamma) Dropped by the graphics rule (nothing reads them: the
  lead checked `gpu_link.cpp` only counts them for its log); the fence
  writes in SetStreamSource, SetIndices and SetTexture kept (recompiled
  code reads fences); the WRAP8-15 setters' dirty bit quirk (bit 45,
  0x2183's) kept.
  Migration 1 (7b1c218..c421c0b): 121 of 131 ours (the 105 state
  setters, the bound objects, constants, SetFVF, the scissor, viewport
  and surface info; `sub_822A6A88` VertexDeclaration::FromFvf Native;
  `sub_822A0F80` SetScreenScissor Dropped). For review: zero-difference
  "(expected to differ)" groups for an uninitialized stack word in the
  originals (the fence list entry's bit 31 in SetStreamSource,
  SetIndices, SetTexture; FromFvf's end-marker pad byte), written 0 by
  ours; a fence on a full fence list leaves the whole call to the
  original, and GetViewport's copy to a physical destination to
  memcpy's override (neither in play); native float copies keep a
  signaling NaN's bits where the originals quiet it (compared as
  floats); the constant copies as raw 16-byte copies (as lvlx/lvrx);
  the adapters written one per line for `check_function_statuses.py`;
  the setters' adapters stay (the device's tables point at them).
  Migration 2 (fd70fa2..fd026da): 131 of 131 ours; SetRenderTarget,
  SetRenderTarget_External, SetDepthStencilSurface, and the packet
  writers without their packets (SetDefaultState, DirtyAllState,
  `sub_8229B7D8`, SetShaderGprAllocation with `sub_8229F680`'s dummy
  draws and their reservations left out, SetGammaRamp); `sub_822B0C38`
  Dropped (no override: SetGammaRamp's C++ leaves the call out; its
  other caller `sub_822A3AA8` isn't seen in play); `functions.md`'s
  Device::SetUpDisplay, Init and SetDisplayMode rows (they call the plain
  C++; their guest calls in the new `src/hooks/device_state_calls.h`).
  For review: guest calls kept through `DeviceCalls` (the draw's Wrapped
  flush helpers `sub_822ADC08` and `sub_822ADE30`, so `sub_8229F680`'s
  mask flush and renderer marks match; non-native table setters, none in
  play; SetTexture's original on a full fence list); SetDefaultState
  calls each setter natively by its table address (the empty
  `sub_82106540` as nothing); the command buffer allocator's bookkeeping
  (+13532, +44, +48, +13516, +10809 bit 6) is read only by the command
  buffer code, not `src/native`; SetGammaRamp writes +13608 itself
  instead of calling memcpy (the original runs if the device were ever
  in the physical views); temporary logs showed no REG_TO_MEM in play
  and the gamma's COND_WRITE (display register 0x1922) the only one.

- `video-output.md` (new, 30 rows; 6 moved from `functions.md`:
  VideoOutput::SetUpDisplay, WaitForVblank, ResolveFrontBuffer,
  Event::Create, Wait, Signal; 24 new; mapped in 2fdfea4),
  `known-data.md` (the video output, renderer +384, device +10760, the
  semaphore's layout, `0x8266D088`, the vblank counts); 3 leads (the
  device lock's other callers, `sub_822A8A60` probably
  SetShaderGPRAllocation, the other ScopedLock users). Corrections to
  earlier maps: "state saved and restored" is "device locked and
  unlocked" in `cameras.md` (`sub_8209FBF0`, `sub_8209FC7C`,
  `sub_820A2F40`, `sub_820A2F94`, the intro) and `strings-and-font.md`
  (Font::Font, the intro).
  Migration 1 (c234559..bafb51c): the video output's constructor,
  LockDevice, UnlockDevice, Device::Acquire/ReleaseThreadOwnership and
  `sub_825B9A60` CurrentThreadId (`functions.md`; through the SDK's
  `XThread::GetCurrentThreadId`) native, with `cameras.md`'s
  MakeRenderTarget, SetUpSurfaces and Init (their helpers Merged); the
  sync wrappers' rows say they stay recompiled (the user's call). For
  review: an unlock without the lock held is logged and skipped (a
  binary semaphore can't count to 2 as the game's could; never seen);
  device +10760 written as the game does; SetUpSurfaces' leak of its
  level 0 surface, once a pass at boot, kept; the cube face's surface
  `sub_822A5C50` (never in play) left a guest call; the lock's guest-call
  return addresses in SetUpSurfaces per type, its other guest calls
  type 3's (nothing reads them); MakeRenderTarget's and SetUpSurfaces'
  adapters stay (guest calls through `ctx`). The user's call (2026-10-24): (a): the device lock alone over a host primitive, a
`std::binary_semaphore` (a semaphore may be released by another thread
than the one that took it; a mutex may not), through LockDevice,
UnlockDevice and the video output's constructor; the Semaphore class
stays recompiled for the archive loader.

- The graphics resources' first migration (e4730b8..e6b9811): 25 of 45
  ours (7 Native), the new row `sub_822B5EA8` Edram::Free (Native, not
  seen in play), `sub_822A6268` named `Resource::DestroySurface`, the
  file's intro. For review: native callers call `refcount::AddRef`
  without the lock whatever `--refcount_global_lock` says (the flag now
  switches only recompiled callers); `Texture::BaseSize` and
  `TextureLayout::LevelSize` read their in/out words once, not again
  after each store (matters only if an out pointed into the header: no
  caller does); `LevelSize` doesn't keep r3's upper 32 bits;
  `Resource::DestroySurface`'s adapter pops Destroy's 112-byte frame
  (the recompiler split it off); the buffers' and CreateSurface's
  failed-allocation paths have no cases (the allocation layer aborts
  when out of memory); a frame dump against the fallback flag showed the
  same targets and draws, the resolve addresses offset by a constant
  (probably the heap's layout; not confirmed with two fallback runs).
  Validated (`itc_1267`, `itc_1269`): `sub_822A5B78`
  Texture::GetLevelDesc has no print of its own (the original only
  jumps to LevelDesc, whose plain C++ its adapter calls; its harness
  and LevelDesc's game line cover it); AddRef's print added (2783ad0).
- The graphics resources' second migration (062da1d..4be0508): 46 of
  46 ours; new row `sub_822A4A40` TextureLayout::MipTailOffset (Native,
  not seen in play). `functions.md` rows changed: `sub_8229E908` (also a
  fence wait), `sub_822AC470` (the data cache flush), `sub_825B9A60` (the
  current thread's id), and new `sub_8229DDD8` (widens the device's
  pending flush range; Dropped from Lock); their Status blank (other
  recompiled callers). For review: paths kept on the originals, never
  taken in play (TextureHeader::Set for unknown types, which read an
  uninitialized stack word; levels from 2^31 in LevelOffset, LockLevel,
  LockRect, LockRectLevel; Destroy's type 9 with `sub_822AD9D8`;
  XGSetTextureHeader with its header out in the physical views or across
  two views); GetSurfaceLevel leaves out its LevelOffset call (it writes
  only its own stack); XGSetTextureHeaderEx stores the 52-byte header
  directly instead of calling memcpy; the lock count by compare-and-swap
  without the recompiler's global lock (as `refcount.cpp`); frame dumps
  matched the fallback flag's.

- `graphics-resources.md` (new, 44 rows; 8 moved from `functions.md`,
  36 new; mapped in 0e67c00), `known-data.md` (the texture, surface
  and buffer headers, the level description, the GPU format table at
  `0x82013468`, the EDRAM allocator at `0x82953E20`, the hi-z flag at
  `0x829537C4`, the device's target and stream fields, the device
  pointer at `0x820007B0`); 3 leads (the device's state setters at
  `822A3030`-`822A4340` and `sub_822A6008`, the boot movie player's
  graphics side at `8248xxxx`, the resources' other makers and users).
  Shared rows left where they were: `sub_822A5B68`, `sub_8235A9F8`,
  `sub_8235A990` (`models.md`), XMemAlloc and XMemFree (`heap.md`).
  The user's calls (2026-10-24): (1) `Edram::Alloc` (`sub_822B5EF8`) made native with a `std::mutex`
in place of the kernel's critical section (its bases kept: the
renderer keys targets by them); (2) `Resource::Lock`'s and
`Resource::Destroy`'s fence waits, `Lock`'s cache flush packet and
`Unlock`'s data cache flush (`sub_822AC470`) Dropped, the lock count,
the returned address and the freeing kept (a harness group for the
command buffer's words expected to differ).

## Run 2026-10-23 20:22 (`b7-loop-23-10-26-5`)

- `ai-drivers.md` (new, 148 rows; 2 moved from `functions.md`, 146
  new; mapped in 1eeb084), `known-data.md` (the driver, path, corner,
  other-racer, skill and car input record layouts; the route file's
  corners table and per-slice corner list; globals `0x826C6074`,
  `0x826C6078`, `0x826C5B30`, `0x826D82E8`; `.rdata` tables at
  `0x82008970`, `0x82008A08`), `track-route.md`'s intro; 2 leads (the
  race states' route users, the drivers' callers). For review: the two
  moved rows corrected (`sub_8213F530` is `AiPath::Update`,
  `sub_8213D438` `AiPath::Reset`, not the speed trail); unsure names:
  the other-racer record states 0-3, driver states 2-6, the corner
  points at data +48 to +54, `AiDriver::KeepsGapFrom`,
  `AiPath::RaceAllowsSides`, `AiDriver::IsFree`,
  `AiPath::LeaderBranchKind`, `AiPath::FollowLeaderBranch`, the skill
  block as a profile plus triggers, vehicle flags 0x81 as the player's
  car, input flag 0x200 as reverse when stuck; ~30 rows "Not read in
  detail" (the corner planning and avoidance math); `AiDrivers::Create`
  zeroes the table after `AiDriver::InitPath` set its floats (the
  game's own, kept).
  Migration 1's rows (c94d6be..91b8da2; `--native_ai_drivers`; 74 ours,
  24 Native): notes corrected for `sub_821390D0`, `sub_8213D438`,
  `sub_82139B40`, `sub_82139820`, `sub_8213C5C8`, `sub_8213C630`,
  `sub_82146408`, `sub_82139CB0`, `sub_8213ADF8`, `sub_821452A0`,
  `sub_821454C8`, `sub_8213C010`, `sub_8213C3A8`, `sub_8213C420`,
  `sub_8213AC70`, `sub_8213D020`. Code in `src/ai_drivers/` (strict
  float flags), adapters `src/hooks/ai_drivers.cpp`, harness
  `src/harnesses/ai_drivers.cpp`; the course's route builder moved,
  unchanged, from the track_route harness into
  `src/harnesses/test_route.h` (shared). Left on the recompiled code:
  UpdateSteerInput's kinds 2 and 3 (`sub_8213AB40`, `sub_82139A08`,
  never seen in play, tail-called); AllSettled and AdvanceCorners hand
  the call back to the original when memcpy's copy can't be native (a
  physical-view destination or a range crossing a view's end; none
  seen in the harness). Kept: stack-passed words written to the
  originals' frame offsets; r3 left as each original leaves it;
  single-word float copies keep a signaling NaN's bits; StartRace and
  LimitThrottle read the vehicle without a null check, as the
  originals. The AI harness refills its block every fourth case (about
  4 s to under 1 s).
  Migration 2's rows (e01cc35..24669a4; 18 more ours, 22 adapters
  removed: 92 ours, 46 Native): `sub_821462A0` (UpdateState's state 6,
  never seen in play, no row) ported with UpdateState and its own
  harness cases, said in UpdateState's row and the intro; JoinCornerPair's
  equal-direction steps (`sub_8213BDF0`, `sub_8213BE80`, never seen in
  play) left on the recompiled code, called by AddCornersAt's adapter
  from the original's stack frames; SteerToTarget's row corrected (+152
  is sin × +384 + cos × +392, not cos × +384 + sin × +392).
  Migration 3's rows (1521ad2..18ea84b; 22 more ours, 25 adapters
  removed: 114 ours, 71 Native; 23 new rows). Never seen in play,
  ported from reading only with their own harness cases (no rows; the
  file's "Not seen in play" paragraph), names guessed: UpdateSteerInput's
  kinds 2 and 3 (`sub_8213AB40` SteerIntoCorner, `sub_82139A08`
  SteerOutOfCorner, `sub_8213A200` SteerAtCornerPoint, `sub_82138FA8`
  SteerByRoom, `sub_821392D8` LimitSlide), `sub_8213D898` AvoidByLane
  (with AvoidOthers), `sub_8213F7E0` EntryWord and `sub_8213F868`
  EntryValue (with AiSkill::TriggerStarts), and `sub_8213F9A4`,
  `sub_8213FB98` (jump targets made functions, inside TriggerStarts and
  TriggerEnds). Corrections: the steering steps' f1 now compared (the
  original leaves the x difference to the target in f1 without atan2);
  MeasureOthers skips a negative racer slot (not the list's end), and
  its running sum and ring entry carry over between racers from entry 2
  (kept); DropOffRouteCorners changes the last kept record only when
  its +37 is set and more than one is kept. Kept, probably a game bug:
  SpreadOffset (`sub_8213B890`) adds the leftover after a clamp to the
  same entry several times, not the ones after it.
  Migration 4's rows (fcec6fd..2ea28d9; 16 more ours, 13 adapters
  removed: 130 ours, 84 Native). `AiDrivers::UpdateAll` keeps its
  adapter: each driver's update goes through the pointer at +6148,
  still recompiled targets, so the plain C++ takes a callback and the
  adapter makes the guest call from a frame like the original's.
  Corrections: `LeaderBranchKind` and `FollowLeaderBranch` also give
  kind 5 (branches 1 and 5, through `sub_8213D2B8` and `sub_8213EC8C`,
  jump targets made functions, never seen in play, ported with their
  callers); `BranchMasksFor`'s kinds 1, 6 and 7 fall through to the
  +32 and +48 lists' checks. r3 kept 64-bit where the original
  sign-extends (BranchMasksFor's list address, FindSideSection's -1).
  Harness limits: FindSideSection and CompareWith always given a
  vehicle (the originals read it without a null check), CompareWith
  and UpdateAll cases use finite floats in the first seven drivers.
  Migration 5's rows (5a7f90d..2ddd638; 13 more ours, 35 adapters
  removed: 143 ours, 119 Native). Fallbacks: when AdvanceCorners can't
  copy natively (corner records in the physical views, never with heap
  drivers), DropPassed, AiPath::Update, AiDriver::Update, DriveCar and
  UpdateIdle return false first and the top adapter runs the original
  whole. For review: `Extend`'s calls it can't make natively (the
  +5532 normalizer when not NormalizeByLane or NormalizeByBranch,
  AddCornersAt's join step; neither in play) go through a `PathCalls`
  struct the top adapter (DriveCar's or UpdateIdle's) hands down, as
  UpdateAll's +6148 calls; with that, `Extend`'s adapter was removed:
  does it fit "its work is plain C++"? Kept, the game's: `Extend`
  appends to the section list with no bound check (past 8 sections it
  writes into the corner count +5162; harness routes cross at most two
  a call). Corrections: StartRanked moves vehicles with flags 0x81 to
  the front, not the back, and sorts only with three or more;
  BranchMasks uses +5528's kind when +5524 is -1; UpdateIdle's reset
  bit is 0x2. Harness: DriveCar's cases also compare the vehicle's
  +2249 and +2424 (an "extras" mechanism); BranchMasks' cases have
  their own 0xA3B00-byte race-data block. Still recompiled: Destroy
  and DestroyRings (wait on `sub_820900C0`), UpdateStarting,
  UpdateTrack and NoteFixedSections (wait on `sub_821434E8`); 22
  adapters stay for recompiled callers in play, 2 (NormalizeByLane,
  NormalizeByBranch) for the +5532 pointer, and UpdateAll's for the
  +6148 targets.
- `racer-controllers.md` (new, 26 rows; 3 moved from `functions.md`,
  23 new; mapped in b5491fb): the per-tick code filling each car's
  input record from the pad or the AI driver (`CarInputs::Create` and
  `Destroy`, `PlayerControl::ReadPad`, the input recording and replay,
  the race flow's per-state handlers, `AiDriver::Setup`);
  `known-data.md` (the car input record is 20 bytes, +17 0 pad / 1
  wheel / 2 AI; `0x826C5B30`, the race data's +0x59AE4 flags, the race
  flow object, the control map, `0x826C5B4C`, `0x826C7960` and
  `0x826C7964`); 3 leads. For review: the handlers are reached through
  functors `sub_82160AE0` registers (a lead), so their adapters stay
  until that table is native; the state numbers from simulating those
  registrations, their meanings guessed (7 the race start, 23 the race,
  22 after the finish; messages 29, 30, 34 "probably" pause and
  replay); `sub_8209F340`, `sub_8209F3B8` named `Pad::IsConnected`,
  `Pad::IsWheel` (`sub_82591C08` moved too); `ai-drivers.md`'s input
  record size (18 bytes) and DriveCar's callers to correct;
  `RacerControl::DriveByAiWithPad` (`sub_8215F7E8`) would write the
  steering on its scale's overflow, which a clamped value can't reach
  (kept).
  Migration 1's rows (699bbde..7b85ae8; `--native_racer_controllers`;
  27 of 27 ours, 15 Native; `sub_821024D0` and `sub_82591C08` Merged;
  `sub_8215F5F0`, `sub_82160880`, `sub_82102A28`, never seen in play,
  ported with harness cases, no rows; `ai-drivers.md`'s DriveCar and
  UpdateIdle rows corrected). The agent ran `git checkout` on
  uncommitted work once (batch 4, after a mutation test), against the
  rules; it redid the same edits and committed them (a71062d), nothing
  lost. New row: `sub_8215FC38` (RaceFlow::OnState34) drives every car
  in attract races but isn't in `seen_in_play.txt` (found with a
  temporary log): the coverage list may miss other attract-race paths;
  it leaves r3 at the last racer count read (kept). Correction:
  `RaceFlow::AllByAi` passes hold flag 1 to both callees (the cars are
  held). For review: the input record's bytes +18 and +19 are written
  by nothing: `PlayerControl::Update` builds the record in its stack
  frame, so they're the frame's leftovers; native code keeps the same
  frame offsets (the harness matches), but the leftovers under native
  callers can differ in play from the recompiled ones (they look like
  padding nothing reads); `DriveByAiWithPad` likewise reads its control
  words from the frame when a control of 0 or above 30 writes nothing.
  DriveCar's and UpdateIdle's adapter work moved into
  `hooks::AiDriveCar` and `AiUpdateIdle` (`src/hooks/ai_drivers.h`),
  with stand-in pointers only the racer_controllers harness sets.
  ReadPad calls `Vehicle_GetSpeedKmh` directly (no `--capture_dir`
  note); `PlayerInput::Read` reads mapper slots 8, 10, 12 and 14
  through the action mappers' plain C++ when `--native_action_mappers`
  is on (slot 5, `sub_8210BA68`, still through the vtable). Adapters
  kept: the 8 handlers reached through the race flow's functors, and
  `sub_82102968`, `sub_82170FF8` for recompiled callers.
- `track-zones.md` (new, 92 rows, all new; mapped in 52cd53b): not the
  AI's path search the lead guessed but 70 hardcoded stretches of the
  course's route (zones, their set-ups the slot 0 of 70 vtables at
  `0x82011AF0`), each of up to 16 route ranges (start and end (section,
  slice), two 64-slot section lists), checked each tick against the
  player's car; the owner at `[0x828161EC]` (`TrackZones::`); fixed-size
  lists for 16, 64 and 128 slots; the HUD reads the flags
  (`sub_82212F00`, `sub_82213510`, a lead). `known-data.md`:
  `0x828161EC`, the zone and route range layouts, `0x82603274`,
  `0x82011AF0`, `0x8200D838`. For review: `sub_820C9718`
  (`Racers::Vehicle`) has its row here, though the race manager's (to
  move when that's mapped); `List16::Link` and `List64::Insert` shared
  with the camera modes and the model list; unsure: what the zones mean
  (probably expressway route names or signs), `RouteRange::ToNeighbor`,
  `TrackZones::Create`'s player choice (probably the second player);
  zone 55's only range is the point (0, 0)-(0, 0), probably a
  placeholder; `RouteRange::WithinEnds`'s "slice above -1" test as the
  code has it.
  Migration 1's rows (31bc1e8..a99b454; `--native_track_zones`; 92 of
  92 ours, 9 Native): the 70 set-ups as one plain C++ function each
  over a table of their ranges read from `generated/` (`kSetupRanges`,
  checked against the rows), each range built at the original's stack
  offset; `RouteRange::Init`'s row corrected (the end from r4 + 4, r5
  not read). The coverage list misses three camera-mode functions that
  run in attract races (`sub_82155288`, `sub_821562F8`, `sub_820FE830`,
  seen by temporary logs, no rows: the camera modes' lead): Init,
  AddBranch, Update and Test keep their adapters for them. For review:
  Create's harness zeroes the owner's lists' unwritten slots before
  comparing (copied from the set-ups' stack; in the original, its
  callees' frame leftovers, which native Create doesn't write; nothing
  reads them, the walks stop at the counts; the set-ups' own harness
  compares them byte for byte); the range tests' harnesses leave out
  cases whose walk might never end on the random route (the original
  wouldn't return either: OnPath 29 of 1000, Test 70, Update 51,
  Zone::Update 123 of 300, logged).

## Run 2026-10-23 17:22 (`b7-loop-23-10-26-4`)

- `strings-and-font.md` (new, 35 rows; 0 moved, 35 new; mapped in
  0974397), `known-data.md` (`0x826C5C34` and `0x826C5C00` split into
  their own rows; new rows: the font page, the text draw parameters, the
  draw counts at `0x826C5BCC`, the pixel shader sources at `0x826C5C04`);
  1 lead (the app's framework beside the string table). For review:
  `ui-movies-game.md` corrected (a text's +20 is the page shown, +24 the
  lines to skip; `sub_8210A5C0` counts page breaks, so `sub_8211C930`
  moves to the next page; `UiMovie::RevealText`'s name kept, probably
  "next page"); `files-and-saves.md`'s `sub_8210A8D0` named
  `Strings::Load`; unsure: `Font::SetParams`'s +8 and +12,
  `Font::DrawPass`'s slot layout (command 36 not in `gpu-functions.md`),
  `sub_82108860` probably `XPR::Release`, `sub_8210A490` probably
  `swprintf` (its ready group is the C runtime's wide printf).
  Migration 1's rows (16458d3..29aea2e; `--native_strings_and_font`;
  25 of 36 ours, 14 Native; `c-runtime.md`'s `sub_825A9A80` Merged,
  `sub_825A9980` and `sub_825B13D0` Native; `sub_82108810` a new row,
  not seen in play). For review: the agent ran `git checkout` on
  `src/strings_and_font/font.cpp` with uncommitted work in it (against
  the "commit before reverting" rule), then re-applied the same edits,
  checked with all harnesses (`itc_1029`) and a frame dump, before
  16b310a; kept on the recompiled code (never in play): rotated text
  (`sub_821097F0`, `sub_82108F68`), the second pass (`sub_82109D80`),
  DrawPass with a chunk open, a list possibly too full or the command
  list's diagnostics on, Strings::Init and Release without
  `--native_heap`, swprintf's `%n`, `%Z`, float types but `%f` and
  `%f` needing the heap or a forced point, output holding 0xFFFF,
  FormatGrouped's negative values holding 0xFFFF or over 240 units;
  swprintf's `_woutput_l` is a step-by-step translation
  (`src/c_runtime/wide_format.cpp`) run once dry to decide whether it
  can run natively, its buffer where the original's frame would be
  (2,542 of 3,441 harness cases took the native path); DrawPass's and
  Font::Draw's adapters stay (the render queue's call through its
  vtable); new helpers `src/strings_and_font/steps.h`,
  `c_runtime::Vswprintf` in `format.h`.
- `std-string.md` (new, 16 rows; 0 moved, 16 new; mapped in 7aeebb5:
  Dinkumware's `std::basic_string<char>`, the library's wildcard search,
  find and replace-all, two global strings' static set-ups),
  `known-data.md` (the 28-byte string layout, the global strings at
  `0x8266EE7C` and `0x8266EE98`); 1 lead (the containers'
  instantiations). For review, not edited: `ui-movies-game.md`'s
  `sub_8211E378` separator from `0x82001DE8` called "." without the
  image's string read (maybe "/"); `files-and-saves.md`'s `sub_8209DFA8`
  leaves out the path's copy into a string and its wildcard replace
  (`sub_82593088`, never matched in play); `sub_82593040` is the
  substring copy constructor, not "making an empty one"; unsure:
  `String::ReplaceAll`, `String::Find`, `WildcardSearch` (probably; the
  `*` backtracking not read in detail), the global strings' texts.
  Migration 1's rows (2028ce7..5fdcfc2; `--native_std_string`; 14 of
  16 ours, 5 Native). For review: paths left to the original, checked
  before anything changes (never in play): memcpy_s's and free's
  errors, operator new's new handler, `_Xlen`, `_Xran`, erase on a
  self-assign, `append` from the string's own text (`sub_820A6E60`),
  `replace` (`sub_82592E18`) when the wildcard replace matches, copies
  not below 0xA0000000 in one view (the mirrors' memcpy recording
  kept); the constructors write the empty start before their assign's
  checks (the original writes the same bytes again on a fallback);
  `_Tidy`'s r3 kept as the original leaves it; allocating paths native
  only with `--native_heap`; the harness's ReplaceAll patterns can't
  match except the game's (`/` to `\`: a matching-anything pattern
  makes the original loop forever), and `_Copy`, `_Grow` cases use
  capacities 15 and up (the `_Copy` quirk in the file's intro); new
  helpers in `src/std_string/std_string.cpp`.
- `track-route.md` (new, 40 rows; 0 moved, 40 new; mapped in e211173:
  route positions normalized and moved along linked sections, points,
  lanes and distances, `TrackRoute::Follow`), `known-data.md`
  (`0x826C5B10`'s full layout, the default branch mask at `0x82008A00`,
  the track position's +4 and +6 as a route position, the vehicle's
  +5232 as the car's track position); 1 lead (the race code's users of
  the route). For review: `collision-world.md`'s `TrackPosition::Init`
  (`sub_820F8578`) corrected (its point comes from its route position,
  not the other way round); `cameras.md`'s intro points to
  `track-route.md` for `sub_820AB408`, `sub_820AB498`;
  `vehicle-accessors.md`'s `sub_820C0168` `Vehicle::Identity` probably
  deserves a new name (left); `sub_820F985C` is a jump table's case 0
  split off by the recompiler; unsure: `TrackRoute::SliceTest`,
  `SpecialSection` (fixed section numbers, probably one course's),
  links 6 and 8 as the left and right neighbors, the two point tables
  as the road's edges, `LinkDistance`, `PathDistance`, `ToMainRoad`'s
  end not read in detail.
  Migration 1's rows (9c3cbaa..7785364; `--native_track_route`; 33
  rows changed, 17 Native, `sub_820F985C` Merged). For review: the map
  corrected for `sub_820F9448` (every linked section's lanes added up;
  kinds 1, 3 and 4 only not taken), `sub_820F9600` (taken when it's the
  only match), `sub_820FB030` (the shoulders start at half the width
  plus 0.5); kept on the recompiled code: SlicePoint's points 5 to 16
  (`sub_820F8FA0`), the unseen callers `sub_820F9DD0` and `sub_820FB2B0`;
  stack temporaries mostly not written (nothing reads them after the
  return), but Point and AlongSlice still write their position copy at
  the original's frame + 80; Distance's r3 kept as the original leaves
  it; new helpers inside `src/track_route/track_route.cpp`, built with
  `-ffp-model=strict -ffp-contract=off`. Attract races with all
  harnesses now reach 18 s at 30 ticks/s inside `attract_race.sh 25`
  (19 before): the harnesses take ~4.9 s before the race; without them
  23 s as before (`itc_1084`, checked by the lead). Since (the user's
  call, 2026-10-23), harness runs use `attract_race.sh 30`.
  Migration 2's rows (0b863a1..1b58c97; all 40 of `track-route.md`
  ours; `collision-world.md`'s `TrackPosition::Update` (`sub_820F8470`)
  and `Init` (`sub_820F8578`) native under `--native_collision_world`
  in `src/track_collision/track_cell.cpp`, calling the route's native
  Follow and Point whatever `--native_track_route` says; 7 adapters
  removed). For review: Follow's full search (`sub_820FB2B0`, never in
  play) ported as `TrackRoute::FullSearch`, with its own harness group
  but no row of its own (described in Follow's row; CLAUDE.md gives
  unseen functions rows marked "Not seen in play": a row may be
  wanted), keeping two quirks of the original (the next link from the
  last linked section in bounds; the whole kind byte tested `>= 0x30`);
  Follow, PathDistance, Update and Init don't keep the original's r3
  (no caller reads it); positions the callees read by address stay in
  the original's frames; PathDistance's harness uses routes of 8-slice
  sections (the original's loops can run forever on random links);
  18 adapters stay for the race and vehicle code's recompiled callers.

## Run 2026-10-23 13:44 (`b7-loop-23-10-26-3`)

- `model-animation.md` (new, 36 rows; 0 moved, 36 new; mapped in
  c425049), `known-data.md` (the model's +304 to +384 spelled out; new
  rows: the track set, the animation section with its 20-byte keys,
  the skeleton); 5 leads. For review: `models.md`'s `sub_82599A20`
  corrected (the per-node float4s' x, y, z set to 0.0, not 1.0) and
  `sub_8259A9E0`'s row given its r6 instance count (+160) and the
  per-instance cursors at +128; the primitive drawer (`sub_825A1438`,
  `sub_825A16B0`, `sub_825A17A0`) and `sub_825A18D8` aren't animation
  but have rows, as the lead named them; `sub_825A5778` to
  `sub_825A6460` are probably XAudio's effects (XAPO), not animation (a
  lead); only some track sets ran in play (the +364 material animation
  and +384 sets' helpers and `sub_825A20E8` never did); Model::Animate
  advances times by 1.0 a call from `sub_820F6068`, probably on
  render-only passes too (not checked).
  Migration 1's 36 rows (6f51fc2, 6714e72; `--native_model_animation`;
  16 Native, written without adapters as only native callers reach
  them; 3 new rows, Native and not seen in play: `sub_825A0FE0`,
  `sub_825A10A8`, `sub_825A20E8`; `models.md`'s `sub_8259C3A8` Native,
  named Skeleton::UpdateNodeMatrices; left: the primitive drawer's
  vtable methods, blocked by the graphics device). For review: the
  releases, set-ups, frees, destructors and the drawer's constructor
  run natively only under `--native_heap`, handing the call to the
  original when free or operator new would refuse it (the collision
  precedent), and abort if `OperatorNewArray` fails after those checks;
  `TrackSet::Setup208` is native only for sections with no tracks (all
  in play) and `sub_8259C3A8` hands node types 8 and 16 to the
  original, so `sub_825A6608`, `sub_825A1938`, `sub_8259B748` and
  `sub_8259BE40` stay recompiled (both checks assume the data isn't
  written during the call); `sub_8259C3A8` notes its multiplies with
  `BlendState_OnMultiply` under the original's return addresses;
  `Curve::Sample` corrected (before the second key it interpolates the
  first two, not clamps); the harnesses never build node types 8 or 16
  or Setup208 sections with tracks; the new code reuses
  `car_physics/float_ops.h` and `xact::WaveEntryConstruct`.
- `car-specs.md` (already mapped from the game data check; the map
  checked by the lead, not remapped). Migration 1's 46 rows (d87b553,
  04c3ebd, f1727ea, 80867d4, 114aa35; `--native_car_specs`; 9 adapters
  removed and 25 Native with no adapter; 12 unnamed rows named, among
  them `Car::CopySpec` `sub_820C4888`, `Car::CopyAeroParts`
  `sub_820C4568`, `CountBodyKits` `sub_820CBD20`; left: `Params::Load`,
  blocked by the archive, and `Params::Free`, not seen in play). For
  review: the swaps byte-swap integers where the original round-trips
  through lfs/stfs (folded in `generated/`; the harness checks
  signaling NaNs and denormals); the archive's TOC lookup `sub_8209D868`
  (no row anywhere) stays a guest call from the adapters; the four load
  callbacks keep their adapters (reached through a function pointer);
  `Car::Refresh` runs the original if a copy wouldn't be the host's
  memcpy or the car's span overlaps a source (never taken), its aero
  getters write into the car directly and the 36-byte parts block is a
  host array; the float adapters turn flush-to-zero off unconditionally
  (the data tables' precedent); `sub_820CB0C0`'s leftover r3 reproduced;
  the Refresh, CopySpec, CopyAeroParts and InchUp harnesses use made-up
  tables (the game's load after the harnesses), and some cases point
  the parameter global at a test object for the call (the data tables'
  precedent: the load callbacks run when the main thread lets them);
  `Params::Init` native only under `--native_heap`.
- `cameras.md` (new, 42 rows; 6 moved from `functions.md`, still
  Wrapped with their old text, 36 new; mapped in 8fb7201): the library's
  `Camera` class, the 21 camera passes at `0x826BAC00` and their render
  targets, the two 1,872-byte viewports at `[0x826C2CE8]`; 4 leads (the
  camera modes at `82152028`-`82156CC0`, a second camera class, the
  graphics device's resource calls, memset and atexit), and
  `sub_820AB408`, `sub_820AB498` added to the route lead. For review:
  `known-data.md`'s `0x826C2CE8` row corrected (1,872-byte viewports of
  twenty 80-byte entries, not 80-byte records) with new rows for
  `0x826BABC0`-`0x826BABFC`, the viewport and its entry; a reversed
  depth range from `Camera::BuildProjection` (+384 near, +388 far);
  "probably" for mode 1's passes 4-9 as a cube map's faces, the entries'
  +68 as a focal length in pixels; `CameraPass::RecordBegin`,
  `RecordEnd` and `Viewport::UpdateEntries` not read in detail.
  Migration 1's 20 rows (81d1c0c..1736a63; `--native_cameras`; 1
  Merged, `sub_8209F7DC`; `CameraPass::SetZoom`'s adapter removed). For
  review: interpolation's hooks kept: `CameraPass::Update` builds its
  view at the original's stack address (r1 + 128, the copy note's key)
  and notes the copy and multiply under the original's return addresses
  (0x820A3D18, 0x820A3D90), through a new
  `interpolation::BlendState_OnCopy` beside `BlendState_OnMultiply`
  (`--log_blend_state`: the same blend counts with the flag on and off);
  the `ITC_FRAME_STATE_WRAP` wrappers of RenderTarget, Disable and
  EnableForView written out with the native work inside, and the
  `--log_camera_pass_records` wrapper calling `hooks::CameraPassUpdate`
  (new `src/hooks/cameras.h`); vtable slots go to the plain C++ only
  when they hold the library's function (always in play); BuildProjection,
  SetViewportSize and Update run the original when the stack pointer
  isn't 16-byte aligned; RecordViewport and RecordClear keep guest calls
  (the scene's getters, the render queue) and run the original with the
  command list's diagnostics on, and where a chunk wouldn't fit (never in
  play) call the scene's getters once more than the original;
  float-register words copied as bits (signaling NaNs kept, as the
  translation does); `Viewports::Release` native only under
  `--native_heap`; the harness changes live game globals (the mode, the
  passes' enabled words, pass 0's near plane, pass 4's field of view,
  the viewports and scene pointers, the open-chunk count) and puts them
  back after each run; the SetZoom adapter removal (7b4c8b1) was built
  and raced only with the next commit; two functions named
  `RecordClear` (`sub_8209FA38` here, `sub_825A18D8` in
  `model-animation.md`).
  Migration 2's 13 rows (14a4cc8..11837c9; still `--native_cameras`;
  2 Merged, `sub_820A0D38`, `sub_820A1CA8`; the adapters of
  `Camera::BuildProjection` and `Viewport::UpdateEntries` removed). For
  review: Camera::Update leaves out the original's dead `tan` and `atan`
  (results never read; memory and r3 the same, only f1 on return
  differs, which the caller doesn't read); it keeps interpolation's
  notes (its view at the original's r1+112, the copies at 0x825936C4
  and 0x825936D4, the multiply at 0x82593798); the record passes keep
  their `frame_state_diff` wrapper (`--find_per_frame_state`,
  `--debug_hide`) and RecordEnd calls RenderTarget through its override
  with the original's return addresses; with the command list's
  diagnostics on, or a full list or another chunk open, the original
  runs and the scene's getters are called once more (never in play);
  RecordBegin, RecordEnd and RecordViewport keep their adapters (guest
  calls); the record-pass harness checks only the last submission to
  its made-up queue; map corrected: `sub_825D9310` (marked never run)
  is the 21 passes' static constructor, run at boot; +372 is height ×
  0.5 × tan(fov/2).

## Run 2026-10-23 11:06 (`b7-loop-23-10-26-2`)

- `action-mappers.md` (new, 40 rows; 15 moved from `functions.md`, 25
  new), 4 `known-data.md` rows (the globals `0x826BAB78`/`7C`/`80`, the
  client list at `0x82970C14`, the settings table at
  `0x82600B84`/`B88`, the action mapper's layout); 2 leads (the input
  processor's constructors, the app's start-up). For review: the read
  slots never seen in play (`sub_8210BA68`, `sub_8210BED8`,
  `sub_8210BFA8`) stay in `functions.md`; of the button tests only
  `sub_8258CF10` and `sub_825924E0` (called only by the mappers) moved;
  `sub_8210C370`'s old note (a copy of MenuList::Update's) kept, with
  the real description after it; the config slots
  (`sub_8210B698`, `sub_8210BDF0`, `sub_8210BE30`) matched to their
  slots from `sub_8209F228`'s arguments and the constants' values,
  both "probably" (the vtables and tables weren't read from the image).
  Migration 1's 36 rows (`--native_action_mappers`; 10 Native, adapters
  removed; 4 rows left: `sub_8210AF08` BindAll and `sub_8210B270` Bind,
  whose 7 calls into the input manager's vtables aren't identified, and
  `sub_8210C180`, `sub_8209F430`, blocked by `sub_8258CE60`). No behavior
  change (the user's call, 2026-10-23: migrate off the recompiled code
  only): every harness matches exactly, the registers the originals
  leave behind too; the per-tick override of `sub_8210B018` still skips
  render-only passes first. For review: bits 30-31 of the held,
  pressed and released words come from an uninitialized stack word in
  `sub_8210BBA0` (r25's high half as saved by `sub_8210B7D0`), and the
  native code is handed the same value (`docs/pause-menu-fix.md`); the
  update, Configure and the set-up run the original unless every repeat
  and curve object's slot is the function seen in play; their slot 0
  (`sub_8243DC88`, `sub_82106540`, shared with other classes) done in
  place, no rows; the `--log_pause_menu` and `--log_input_reads`
  diagnostics moved into `src/hooks/action_mappers.cpp` over new helpers
  in `src/debug/pause_menu_log` and `src/frame_pacing/game_messages`,
  and the adapters of `sub_8210C370`, `sub_8258CF10`, `sub_825924E0`
  kept for them on the fallback path; c67265f has a batch and four
  adapter removals together; new names `InputProcessor::AllHeld`,
  `TestHeld`, `ActionMapper::Held`, `Pressed`, `Fired`, `AnalogRaw`,
  `SetThreshold`, `SetRepeat`, `SetCurve`.
- `data-tables.md` (new, 18 rows, none moved): `[0x826C5CA0]`'s set-up
  (`sub_82116DB0`), twelve asset loads (94: 400 items of 3,008 bytes,
  "probably" the game's items, keyed by the save data's 12-byte records
  at +0x199F8; 84 with no reader but the load; eight 56-byte tables from
  the ids at `0x82616C64`; 95's 20-byte and 96's 40-byte entries), their
  load callbacks byte-swapping the 56-, 20- and 40-byte tables in place
  (the items aren't), and the accessors; 4 `known-data.md` rows; 4 leads
  (the boot data load, the save data's items, `82140340`-`82144E58`, the
  front-end callers at `822271F8`-`8222F0B0`). For review: the eight
  asset ids at `0x82616C64` not recorded (they need an image dump; one
  outside the repo wasn't used); `SwapWord` (`sub_820C97D8`) swaps a
  word through a float load and store, which in the recompiled code
  probably quiets a signaling NaN's bits (the 360 wouldn't): the native
  code keeps the recompiled result (no behavior change, the user's
  call; see migration 1 below: there's no quieting).
  Migration 1's 16 rows (`--native_data_tables`; 4 Native: SwapWord and
  the three entry swaps; left: DataTables::Init and Load, blocked by
  the archive's `sub_8209DE48`, `sub_8209D868`, `sub_820A0678`). For
  review: SwapWord's float round trip is folded away by the compiler in
  the release build (a plain byte swap; signaling NaNs come through
  unchanged), and the native code writes the same steps under the same
  floating-point settings, so it folds the same (256 signaling-NaN
  patterns compared exactly; the mapping's "probably quieted" corrected
  in the row); the adapter turns flush-to-zero off as the original
  does, and doesn't set f0 (no caller reads it); the accessors return
  r3 zero-extended where the originals keep the argument's upper 32
  bits (as the vehicle accessors do: the game reads the low word only);
  Entry56, Find20, Entry20, Entry40 and ItemRecords::Item weren't seen
  in the attract race (the front end, the save data); the five load
  callbacks keep their adapters (reached through Archive::Load's
  function pointer), the seven accessors too (recompiled callers).
  After the run's validation: `sub_82116DA8`'s adapter calls a new
  one-line `data_tables::RecordsItem` (over `Item`) with its own print,
  as its harness step does (the user's call).

## Run 2026-10-23 01:52 (`b7-loop-23-10-26`)

- `sound-cues.md` (new, 87 rows; 4 moved from `functions.md`:
  `sub_8259D1A8`, `sub_821184E8`, `sub_82118FB0`, `sub_82118C18`), 8 new
  `known-data.md` rows (the sound manager, the audio engine, the bank
  pair and holders, the sound, the emitter, the listener, the XACT and
  XAudio engines); 4 leads added (XACT's body, X3DAudio and the engine
  sounds, the data tables, a file-layer helper). For review: XACT's
  vtable slots named only "probably <method>"; the intro records the
  user's call that XACT will probably be translated (it replaces "keep
  the game's XACT" in `native-audio.md`'s decisions, edited there 2026-10-23
  there); the 3D path never ran in play.
  Migration 1's 30 rows (7 Native; the intro's "Native so far"). For
  review: X3DAudioInitialize (`sub_82480710`, a library entry point,
  pure math) translated; it doesn't store the speed at r1+28 in its
  caller's frame as the original does (no caller reads it);
  SoundBankHolderDelete leaves out the call to the empty `sub_82106540`;
  the bank slot functions run the original where it traps, and the
  allocating ones unless the native heap is on; `sub_8259D438` runs the
  original when a handle is to be closed (never in play). The message
  handler `sub_82119188` left recompiled (guest vtable calls); the rest
  waits on XACT (the cue and engine calls through its vtables), the
  file layer and the string helpers; `sub_82092B78` (a std allocator)
  has no row anywhere.
  `xact.md` (new, 149 rows: the engine with Initialize, DoWork, the
  bank creation and categories, its global settings and audio callback
  `sub_825C0248` run by XAudio on its own thread, the sound and wave
  banks, the cue, sound and fader), `known-data.md`'s XACT objects, the
  `.xsb` and `.xwb` layouts, volume tables and clock globals; 4 leads
  (the rest of XACT, about 265 functions; the voice class with XAudio;
  shared helpers). For review: names follow the XACT3 interfaces where
  slots and arguments make them clear, else "probably"; the boundary
  named in the intro (XAudio's voices and callback, the kernel's
  critical sections and clock, XMemAlloc); a correction to
  `sound-cues.md` and `known-data.md` (the cue's slot +60 is
  GetProperties, writing the emitter's +136 block's first 14 bytes, not
  3D settings). The mapping agent read the vtables from `default.xex`
  decrypted and decompressed into its scratchpad with Python's
  `cryptography` and the retail key (nothing of it in the repo).
  Migration 1's 55 rows (46 Overridden, 9 Native; the intro's
  paragraphs on how the translation is set up). For review: a function
  calling its own object's vtable slot runs natively only while the
  slot holds the already-native function (GetHeader at sound bank +64,
  GetEntry and GetSeekTable at wave bank +16 and +20, AddListener at
  the bank interface's +0), else hands off to the original before
  changing anything; partial ports taking only the paths seen in play
  (`sub_825CA980` only a sound without tracks, `sub_825CC1D0` only an
  idle fader, `sub_825C8230` hands complex cues to the original);
  allocating functions run the original unless `--native_heap` is on.
  Left recompiled: the engine's critical section (no native lock in
  `src/`; the core's), the global settings' unmapped vtable slots,
  calls through vtables into recompiled code, unmapped callees.
  XACT's second map (37 rows in `xact.md`: the global settings'
  accessors, the renderer's voice slots, the sound classes' and
  listener slots, the wave entry's getters; 2 in `functions.md`:
  `sub_822956F0` probably GetTickCount, `sub_8220BC28` probably
  MenuList::GetPreviousSelection; `known-data.md`'s sound fields +96,
  +104, +120, +128; 1 lead, the code above the tracks). For review: the
  settings' slots +16, +20, +32, +36, +44, +48, +52, +56 "probably",
  from what callers do (no image dump read); for your call:
  GetTickCount made native means reading the kernel's timestamp bundle
  the SDK fills, or a host clock (left recompiled).
  XACT's migration 2 (46 rows ours: 39 Overridden, the rest Native, 7
  of them by adapter removal; the intro's settings vtable +0 to +68,
  read from the loaded image). For review: paths seen in play only,
  the rest handed to the original (`sub_825CC9C8` no optional sections,
  `sub_825CB470` flags without 0x2, `sub_825C27C0` no presets and no
  effect or both, `sub_825C2910` +100 is 0, `sub_825C6A20` +240 is 0,
  `sub_825C7798` no listener cues); `sub_825CC380` reads the bank's
  entry through the accessors instead of a stack copy (same words, all
  read before the stores); the folded getters `sub_82099BA0`,
  `sub_820A8958`, `sub_82379D50` overridden (same results for the game
  code folded with them); the harness's `Case` compares f1, a
  `Relocate` helper. Waiting on GetTickCount (your call): XACT's clock
  `sub_825BFE28` and everything above it.
  XACT's migration 3: `sub_825C8440` GetWaveRegion (builds its entry
  accessor in the original's stack slot, as the collision and matrix
  adapters do) and the folded getter `sub_8220BC28` (`functions.md`,
  one load of +20, the menus' callers get the same result) under
  `--native_xact`; 4 adapters removed.

## Run 2026-10-22 22:11 (`b7-loop-22-10-26-2`)

- `collision-world.md` (new, 43 rows), `known-data.md`'s mesh, route,
  course table and track position rows; corrections: `car-physics.md`'s
  CollisionObject::Collide (the first query's arguments: r3 the
  records, r4 the shape record), `collision.md`'s box words +160 (the
  lower corners' track positions), `sub_820C0168` probably the car's own
  track position; 5 leads added. Migration 1's 29 rows (and the intro:
  its constants; the up vector at `0x82638280`, not `0x82628280`). For
  review: a new `memory::IsPages`; the native normalize reproduces the
  vector unit's; `TrackPosition::SearchLinked` (never ran in play) writes
  -1 past its six halfwords onto the route's first at `0x826C5B10`, as
  the game does, kept; four functions ported that never ran in play are
  described in FindCell's row; the "Negated NaN" groups
  (`expected-differences.md`): ours flips a NaN's sign where the
  translated code doesn't. In four functions (`sub_820F6940`,
  `sub_820F8618`, `sub_820FEBD0`, `sub_820FC188`) the print comes after
  a guard that falls back to the recompiled code (the validation agent
  didn't count them as missing).
  Migration 2's rows: 11 in `collision-world.md` (the wall query,
  Unload; `sub_820FC870` named WallQuery::ModeSkips, ours), 3 in
  `car-physics.md` (CollisionWorld::Destroy, CollisionObject's
  destructors, under `--native_collision_world`: their only caller is
  Unload). For review: the wall hits are reached only by the harness (the
  AI cars don't touch walls; every path but EdgesAcrossWall's
  equal-lengths sum); TestCell and QueryWalls' harnesses don't compare r3
  (their callers don't read it); TestCell's up vector at `0x826382B0`,
  not `0x826282B0`; prints moved before the guards in GatherCells,
  TrackMesh::Free, TrackRoute::Free; a new `CollisionShapeDestroyable`
  in `collision.h`.

- `collision.md` (new, 64 rows), `known-data.md`'s shape, box, pair
  query and table entries, `car-physics.md`'s intro; 1 lead added (the
  track's collision queries). Guesses to confirm: the floats at
  `0x82002B90`..`0x82002B98` and the impulse scale at `0x82600E84`.
  Migration 1's 53 rows (35 Overridden, 18 Native) and the intro (the
  box's vtable slots, read at run time). For review: the native code
  reads the constants at `0x82002B90` and `0x820021E8` from guest memory,
  leaves out the box's type calls (slot 4 is `li r3,2`) and runs the
  original for any shape that isn't a box or when the impact table's
  box-box entry isn't `sub_820BF038`; the box copies' memcpys are native
  only where `c_runtime::Memcpy` records nothing. Map corrections:
  LowerCorner's ties and NaNs (k^2), NextCheckTime's radii (the +20
  shape), Shape::Shape zeroes +108's w.
  Migration 2's rows: 23 in `collision.md` (all 64 ours: 35 Native, 29
  Overridden), `car-physics.md`'s `sub_820B7B60` CollisionObject::Translate
  and `sub_820B2CC0` Vehicle::ApplyForces, `vehicle-accessors.md`'s new
  `sub_820988C0` RigidBody::Force (keeps its adapter: reached
  indirectly in play) and `sub_82379328` RigidBody::Torque. For review:
  the lifetime functions use `memory::crt` only while `--native_heap` is
  on, checking error paths first with a new `memory::IsHeapBlock`;
  PushOutCorners does the work of `sub_820BE0D8` and `sub_820BC8A8`
  (never ran in play, no rows); Respond's and Solve's "a fixed body
  meeting the impulse" groups (`expected-differences.md`).

- `heap.md`: 15 rows read in full and rewritten (no status changed), the
  intro's paragraph on native versions; migration 1's 15 rows (12
  Overridden, 3 Native; XMemAlloc's attribute bits corrected by the
  harness: protection `(attr >> 28) & 3`, default alignment field 12). For
  review: the adapters run the plain C++ only while `--native_heap` is
  active (`hooks::NativeHeapActive()`), and the error paths (errno, the
  new handler, `bad_alloc`, `_invalid_parameter`) run `__imp__` from the
  start; `known-data.md`'s heap globals;
  1 lead added (`sub_8235AAC0`), operator new taken from the
  VirtualAlloc lead.

## Run 2026-10-22 14:39 (`b7-loop-22-10-26`)

- `car-physics.md`: migration 1's 41 rows (names marked "ours" where the
  map had none) and the intro (the identity quaternion is at
  `0x826381B0`, not the map's `0x825F81B0`); `vehicle-accessors.md`:
  5 rows. For review: the vector library fixed (ee810a9): ThinLTO could
  move `dpps` out of its flush-to-zero window, found by the car physics'
  harness ("tiny values" groups added, at 0); ContactImpulse's harness
  has a "one NaN meeting its own negation (expected to differ)" group
  (3 cases, 2 differ); the "numbers" groups keep values below 10^6 (no
  infinities made). ContactImpulseWith runs the original for a fixed
  body (whose term the original reads from its stack's leftovers).
  Migration 2's 19 rows. For review: UpdateSteering's harness doesn't
  compare r3 (on the tan path the original leaves `_ctrlfp`'s word
  there; the step sets r3 again); the adapters run the original for
  UpdateSteering when tan would take its `_except1` path, and for
  UpdateTorqueSplit in mode 2 in a forward gear (neither seen in play).
  Migration 3's rows (38 in `car-physics.md`, one new: `sub_820BAA78`
  CollisionShape::Init; `sub_820B4CF0`'s r4 corrected: clear means
  automatic shifts) and 2 in `vehicle-accessors.md`.
  Migration 4's 15 rows in `car-physics.md` and 4 in
  `vehicle-accessors.md`. For review: RemovePair copies a moved pair
  with `c_runtime::Memcpy` only outside the physical views and for
  ranges that don't overlap; otherwise RemoveObject's and RemoveGroups'
  adapters run the original removals from the start (the same result).
- `car-physics.md` (new, 129 rows), `known-data.md`'s new rows, 5 leads
  added (the collision narrow phase and shapes, and others).
- `race-overlays.md`: the matrix-math lead's rows (UpdateLightSprites,
  UpdateParticlesB, UpdateParticlesC, UpdateSmoke, Smoke::SetFrame,
  ModelMatrix, ParticlesB::Emit's f1 corrected to +6256) and 23 adapters
  removed (Native). For review: native callers of Vec4::Transform go
  through a new `debug::Vec4Transform`, which keeps its override's
  `--debug_hide` and frame-state checks; UpdateSmoke stores its size word
  quieted when it comes from the table (as the translated code, which
  keeps it as a double); its harness never uses a smoke capacity of -1
  (the original would copy into 2^32 - 1 emitters).
- `matrix-math.md` (new, 28 rows; 4 moved from `functions.md`), 1 lead
  added. Migration 1's 21 rows (15 Overridden, 3 of them were Wrapped;
  3 Native with no adapter ever; 3 Merged). For review: the three
  Wrapped functions' overrides (Multiply and Copy in
  `src/interpolation/blend_state.cpp`, Vec4::Transform in
  `src/debug/frame_state_diff.cpp`) now do the work natively, with the
  same hooks around it; frame dumps with the flag on and off look the
  same.
  Migration 2's 8 rows. For review: the turns' "one NaN" harness group
  keeps the angle within 1024 (a huge finite angle overflows the game's
  sine and cosine polynomials to NaNs of both signs, which then meet in
  Multiply); the "NaNs anywhere" group keeps every angle. The native
  turns note their multiply for interpolation through a new
  `interpolation::BlendState_OnMultiply` (as the Multiply wrapper does).
- `race-overlays.md`: the no-mapping lead's 14 rows (the view's body and
  wheel accessors, IsBehindCamera, the save flags and stage check,
  UpdateTrails, MarkerKind; FlagsKind `sub_82122DB8` and NoSaveFlags
  `sub_82127198` ported with MarkerKind but never ran in play, so no
  rows) and 6 adapters removed; `vehicle-accessors.md`: `sub_820C0170`,
  `sub_820C0180` Native. The new harnesses run the originals with the
  accessors' flags off too (an `Original<>` wrapper).
- `vehicle-accessors.md`: migration 1's 127 rows (2 now Native, adapters
  removed). For review: the two Wrapped rows (`sub_82593FB0`,
  `sub_820FFD88`) had their frame-state-diff wrappers moved into their
  adapters (out of `src/debug/frame_state_diff.cpp`).
- `vehicle-accessors.md` (new, 132 rows; 3 moved from `functions.md`),
  `known-data.md`'s new rows, 5 leads added.

## Run 2026-10-22 10:47 (`b7-loop-22-10-26`)

- `c-runtime.md`, `ui-movies.md`, `ui-movies-game.md`, `sprites.md`: the
  adapters lead's rows (Overridden -> Native, and notes on the 4 kept).
- `race-overlays.md` (new, 154 rows; `sub_8215DEA0` moved from
  `functions.md`), `sprites.md`'s intro, `known-data.md`'s new rows;
  the first migration's 34 rows (Overridden). For review: RacerId's
  adapter (`sub_82126AE0`) sign-extends r3 where the original
  zero-extends a found id (only the upper 32 bits; callers compare 32).
  The second migration's 6 rows and the 9 names.
  The third migration's 10 rows (4 new). For review: NaN-only harness
  results depend on which operand the compiler kept in the translated
  vector adds: Smoke::Emit keeps the first operand's NaN (as the Xenon
  does), the sparks a plain `addps` matching how this build compiled
  theirs; a rebuild could bring back NaN-only differences there.
  The fourth migration's 32 rows, and `sub_820C0128`'s notes in
  `functions.md`. For review: on the native SpeedKmh path the state
  capture (`--capture_dir`) no longer hears about the vehicle (the
  game's other callers still pass it on).
  The fifth migration's 8 rows (4 Overridden, `sub_8215E078` Merged, 3
  Native). `attract_race.sh` now counts seconds logged at 31 ticks/s too
  (it reported 19 for a 22-second race).
