# Known functions: the boot mode and the modes' other methods

The **boot mode** (`BootMode::`): the game mode of type 3 (10,460 bytes,
`Mode::Create` over `ModeType3::ModeType3` `sub_82106C80`, both in
[`game-modes.md`](game-modes.md); vtables `0x82006B70` at +0 and
`0x82006B64` at +32), the first mode the app makes (`Modes::Modes` with
3) probably also the mode for the movies between other modes (five
other functions use the table, not read: `sub_82150E00` (the race's
end, `race-end.md`: movies 6 and 7 or 8 and 9, then type 9),
`sub_82180CE8`, `sub_82188FE0`, `sub_8225A720`, `sub_8225B9B8`; the
last two queue a movie after the career's day, [`todays-result.md`](todays-result.md)). It plays
the movies listed in the table at `0x826C5BAC` (up to five movie ids, 11
ending the list; at boot 10, the UBI logo, then 11), one boot mode a
movie, through its state machine at +848 (three transitions: start the
movie, wait for it, end it; `BootMachine::Init`), and asks for the next
mode when the movie has ended (`BootMode::NextMode`, its slot 6): the
boot mode again while the list has movies left, else the type at
`0x826C5BC0` (4 after the logo: the front end). Beside it, the modes'
other methods in `82106040`-`82108160` without rows elsewhere: the
base's deleting destructor and default answer, the delegate that asks a
mode for the next one (`Modes::Tick`, `mode-tick.md` and
`game-modes.md`), the state machine with ten words after it that the
front-end screens make (`ScreenMachine::ScreenMachine`), the type 8
mode's constructor and destructors (never made in play) and the app's
shut-down (never reached). The movies themselves are in
[`movies.md`](movies.md) and `../native-movies.md`; the boot and the
title screen in `../title-screen.md`.

Shared, with rows in other files: `ModeType3::ModeType3`
(`sub_82106C80`), `MachineMember::MachineMember` (`sub_82108160`),
`StateMachine::StateMachine` (`sub_82107108`), `StateMachine::Add`
(`sub_82106158`), `GameMessages::Dispatch` (`sub_821062E8`),
`Mode::Mode` (`sub_82106C20`), `Mode::Create` (`sub_821077A8`) and
`RaceModeMember::~RaceModeMember` (`sub_82106AF0`) in
[`game-modes.md`](game-modes.md); `Movies::Play`, `Start`, `Stop`,
`Pause`, `Resume`, `State` and `Release` (`sub_820A43A8`,
`sub_820A43D8`, `sub_820A43F0`, `sub_820A4408`, `sub_820A4420`,
`sub_820A4450`, `sub_820A4438`) in [`movies.md`](movies.md);
`ScreenFades::FadeIn` and `FadeOut` (`sub_8209D038`, `sub_8209D0C8`) in
[`mode-tick.md`](mode-tick.md); the XMP playback calls (`sub_825894C8`,
`sub_825895A0`, Dropped) in [`system-services.md`](system-services.md);
the empty function `sub_82106540` in [`audio.md`](audio.md);
`operator delete` (`sub_825A70E8`) in [`heap.md`](heap.md);
`BootData::Unload` (`sub_821066D0`) in [`data-load.md`](data-load.md);
`App::Start` (`sub_82092878`, which sets the table at boot) in
[`app-states.md`](app-states.md). Layouts are in `../known-data.md`
("boot mode", `0x826C5BA4`, `0x826C5BA8`, `0x826C5BAC`). Mapped by the
B7 loop's mapping agent (`../loops/b7-loop.md`), 2026-11-01, from the
generated code and the image's vtables (`itc/xex_image/default.bin`; no
runs): call counts aren't known. Names are ours.

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
| `sub_82107F68` | `BootMode::Init` | Overridden | The boot mode's slot 0 (vtable `0x82006B70`), from `Modes::Tick` once the mode is made: r3 = the mode. `BootMachine::Init` on +848; +848 added to the dispatcher (+36: handler [count] = +848, the count +236 plus one); the delegate at +8 set (+4 = +8): vtable `0x82010FA0` (`ModeDelegate::Invoke`), +12 the mode, +16 `0x82235C08` (`Mode::NextModeThunk`), +20 = 0, as `RaceMode::Init` sets its own; then `[0x826C5BA8]` = 13 (no mode asked for) and `[0x826C5BA4]` = 0 (the movie not paused). Leaf but for `BootMachine::Init`. Seen in play (once a boot mode). Read in full 2026-11-01. Native: `boot_mode::Init` (`src/boot_mode/boot_mode.cpp`, adapter in `src/hooks/boot_mode.cpp`, `--native_boot_mode`; its frame's +80 and +84 written as the original builds the delegate there; r3 as `BootMachine::Init` leaves it); kept as an adapter: reached through the vtable. harness `boot_mode` (B7 loop, 2026-11-01). |
| `sub_82107DD0` | `BootMachine::Init` | Native | r3 = the boot mode's state machine (+848): three transitions through `StateMachine::Add`, each a member function functor (vtable `0x82011918`, the machine, the function): (state 0, message 0) → 1, `Boot::StartMovie`; (1, 0) → stay, `Boot::WaitForMovie`; (1, 1) → 2, `Boot::EndMovie`; then +9608 = 0 (the movie not started yet, `Boot::WaitForMovie`). The keys' two other words 0. From `BootMode::Init` only. Seen in play. Read in full 2026-11-01. Native: `boot_mode::MachineInit` (`src/boot_mode/boot_mode.cpp`), no adapter: its only caller, `BootMode::Init`, is native, and `--native_boot_mode` switches it. The transitions' functors are built in the original's 336-byte frame in its order (three of each functor's six words never written: the stack's words go into the transitions, as `RaceOverlaysMachine::Init`'s); `StateMachine::Add`'s plain C++ called directly. harness `boot_mode` (B7 loop, 2026-11-01). |
| `sub_82107B90` | `Boot::StartMovie` | Overridden | Message handler: plays the next boot movie, id from the table at `0x826C5BAC` (index at `0x826C5BC8`), via `Movies::Play` (`sub_820A43A8`, the path table at `0x82600BEC`; `movies.md`). See "The title screen" in `../title-screen.md`. Read 2026-11-01 (B7 loop): the boot machine's (state 0, Update) transition (`BootMachine::Init`), moving to state 1. First `sub_825894C8` (XMP's playback taken, Dropped, `system-services.md`), then `Movies::Play(table[index])`, the index plus one, and `ScreenFades::FadeIn([0x82001BEC], 0, 0, 0, 0)` (5 in the image; `mode-tick.md`). Seen in play. Native: `boot_mode::StartMovie` (`--native_boot_mode`): XMP's dropped take left out but for the last error it clears, `Movies::Play`'s and `ScreenFades::FadeIn`'s plain C++ called directly; r3 the passes word, as FadeIn leaves it. Kept as an adapter: reached through its functor. harness `boot_mode` (B7 loop, 2026-11-01). |
| `sub_82107C58` | `Boot::WaitForMovie` | Overridden | Message handler, every tick while a boot movie plays; ends the step on a button press if skippable (`0x826C5BC4`) or when the movie has finished. The movie started (`Movies::Start`) when its state (`Movies::State`, twice a tick) is 1 with +9608 = 0 (then 1); finished when the state is 1 again with +9608 = 1. `--skip_intro` makes it finish at once. Read 2026-11-01 (B7 loop): the boot machine's (state 1, Update) transition (`BootMachine::Init`); r3 = the state machine, r4 = the mode's dispatcher. The button: the current player's action mapper (`[0x826BAB80 + 8 × [0x826BABA4]]`, when its +4 isn't 1) asked through its slot 10 for action 152. To end: `ScreenFades::FadeOut(1, 0, 0, 0, 0)` and message 1 queued in the dispatcher's ring (+204: its count +804 plus one, the 12-byte message (1, 0, 0) at (start + count - 1) mod 50), which `Boot::EndMovie` takes. Seen in play. Native: `boot_mode::WaitForMovie` (`--native_boot_mode`), called by the `--skip_intro` and `--log_boot_modes` wrap in `src/hooks/title_screen.cpp` (its adapter) where it called the original; `Movies::State`'s, `Start`'s and the fades' plain C++ called directly, the action mapper's slot 10 a guest call; `--skip_intro`'s finished answer passed in. harness `boot_mode` (B7 loop, 2026-11-01). |
| `sub_82107BF8` | `Boot::EndMovie` | Overridden | Message handler: stops the movie (`Movies::Stop`, `Movies::Release`, then `sub_825895A0`); the front end (Genki logo, title) follows. Read 2026-11-01 (B7 loop): the boot machine's (state 1, message 1) transition (`BootMachine::Init`), moving to state 2, which `BootMode::NextMode` waits for; `sub_825895A0` is XMP's playback given back (Dropped, `system-services.md`). Seen in play. Native: `boot_mode::EndMovie` (`--native_boot_mode`): `Movies::Stop`'s and `Release`'s plain C++, XMP's dropped hand-back left out but for the last error it clears and its answer, 0 (r3). Kept as an adapter: reached through its functor. harness `boot_mode` (B7 loop, 2026-11-01). |
| `sub_82108018` | `BootMode::NextMode` | Overridden | The boot mode's slot 6 (vtable `0x82006B70`), the next-mode query `Modes::Tick` asks every tick through the delegate (`ModeDelegate::Invoke`, `Mode::NextModeThunk`): r3 = the 8-byte answer, r4 = the mode. The answer cleared (byte +0, word +4). Pauses (`Movies::Pause`) or resumes (`Movies::Resume`) the movie when the paused flag `[0x826C5BA4]` changes: set when either of `0x826C7960`/`64` (the pad blocked, `known-data.md`) is set and the movie is skippable (`[0x826C5BC4]`), or when a mode was asked for (`[0x826C5BA8]` isn't 13); in play neither (the logo isn't skippable, `[0x826C5BA8]` stays 13), so the two never ran. Then message 0 (Update) queued in the dispatcher's ring (+240: its count +840 plus one, at (start + count - 1) mod 50) and `GameMessages::Dispatch(mode + 36)`. When the machine's state (+10452, +848's +9604) is 2 (`Boot::EndMovie` ran): the answer's byte 1 and its type `[0x826C5BA8]` when a mode was asked for, else 3 (the boot mode again) while the table's next id (`0x826C5BAC` + 4 × `[0x826C5BC8]`, index below 5) isn't 11, else `[0x826C5BC0]` (4 at boot). Returns r3. No direct callers. Seen in play (every tick of a boot mode). Read in full 2026-11-01 (B7 loop; its row was in `movies.md` before, where only the pause part was read and it was taken for a function that doesn't run). Native: `boot_mode::NextMode` (`--native_boot_mode`): `Movies::Pause`'s and `Resume`'s plain C++, `GameMessages::Dispatch` a guest call to its override; the answer built in the original's frame (+80 to +87; bytes +81 to +83 never written, the stack's) and copied. Kept as an adapter: reached through the vtable (and `Mode::NextModeThunk`'s plain C++ calls it directly). harness `boot_mode` (B7 loop, 2026-11-01). |
| `sub_82106D08` | `BootMode::~BootMode` | Native | r3 = the boot mode: its vtables put back (`0x82006B70`, `0x82006B64`), the empty `sub_82106540` on +848 (the state machine needs nothing), then the base's vtables (`0x820069EC`, `0x82006A0C`). From `sub_82106D68`. Seen in play. Read in full 2026-11-01. Native: `boot_mode::Destruct`, no adapter: its only caller, `BootMode::DeletingDestructor`, is native, and `--native_boot_mode` switches it; the empty `sub_82106540` left out. harness `boot_mode` (B7 loop, 2026-11-01). |
| `sub_82106D68` | `BootMode::DeletingDestructor` | Overridden | The boot mode's slot 7, from `Modes::Tick` with r4 = 1 when the mode changes: `BootMode::~BootMode`, then `operator delete` when r4's bit 0 is set; returns r3. Seen in play. Read in full 2026-11-01. Native: `boot_mode::DeletingDestructor` (`--native_boot_mode`; operator delete a guest call to its adapter, `src/hooks/c_runtime_heap.cpp`). Kept as an adapter: reached through the vtable. harness `boot_mode` (B7 loop, 2026-11-01). |
| `sub_82107C20` | | | The boot mode's +32 vtable (`0x82006B64`) slot 0: r4 = a number; `[0x826C5BA8]` (the mode asked for) = 12 when it's 2 or 3, else 4; returns 1. Leaf. What calls the +32 interface, and what the number is, isn't known (the base's slot 0 `sub_825C8390` returns 1, `vehicle-accessors.md`). Not seen in play. Read in full 2026-11-01. |
| `sub_82107D80` | | | The +32 vtable's slot 1: r3 = the mode + 32; message 1 queued in its dispatcher's ring (+240 of the mode), as `Boot::WaitForMovie` does to end the movie. Leaf. Not seen in play. Read in full 2026-11-01. |
| `sub_82107C48` | | | The +32 vtable's slot 2: `[0x826C5BA8]` = 13 (no mode asked for). Leaf. Not seen in play. Read in full 2026-11-01. |
| `sub_82106E38` | `Mode::DeletingDestructor` | Overridden | The base's slot 7 (vtable `0x820069EC`), and the 852-byte modes' (types 2, 4, 5, 6, 7, 9, 10 and 12: their vtables' slot 7, as the image has them): the base's vtables put back (`0x820069EC`, `0x82006A0C`), then `operator delete` when r4's bit 0 is set; returns r3. Leaf but for `operator delete`. From `Modes::Tick` (slot 7, r4 = 1). Seen in play. Read in full 2026-11-01. Native: `boot_mode::ModeDeletingDestructor` (`--native_boot_mode`; operator delete a guest call to its adapter). Kept as an adapter: reached through the vtables. harness `boot_mode` (B7 loop, 2026-11-01). |
| `sub_8222FC90` | `Mode::NoNextMode` | | The answer cleared (r3's byte +0 and word +4 = 0): the base's slot 6 and every mode's slots 2 to 5 (as the image's vtables have them). Leaf. Not seen in play. Read in full 2026-11-01. |
| `sub_82235C08` | `Mode::NextModeThunk` | Overridden | r3 = the answer, r4 = the mode: tail-calls the mode's slot 6 (its next-mode query, `BootMode::NextMode` for the boot mode). The function the boot mode's and the race mode's delegates hold (`BootMode::Init`, `RaceMode::Init` in `game-modes.md`). No direct callers. Seen in play. Read in full 2026-11-01. Native: `boot_mode::NextModeThunk` (`--native_boot_mode`): `BootMode::NextMode`'s plain C++ where the slot holds it (from its frame below), else a guest call (the race mode's). Kept as an adapter: reached through the delegates. harness `boot_mode` (B7 loop, 2026-11-01). |
| `sub_8223D440` | `ModeDelegate::Invoke` | Overridden | The delegate's slot 0 (vtable `0x82010FA0`), from `Modes::Tick` every tick: r3 = the 8-byte answer, r4 = the delegate (+4 the object, +8 the function, +12 the object's adjustment): calls the function with (an answer on its stack, object + adjustment), then copies the 8 bytes to r3. A member function call as `Delegate::Invoke` (`sub_8223A3F0`, `functions.md`) makes for vtable `0x82011918`. Seen in play. Read in full 2026-11-01. Native: `boot_mode::ModeDelegateInvoke` (`--native_boot_mode`): `Mode::NextModeThunk`'s plain C++ where the delegate holds it, else a guest call. Kept as an adapter: reached through the vtable. harness `boot_mode` (B7 loop, 2026-11-01). |
| `sub_82107358` | `ScreenMachine::ScreenMachine` | Overridden | r3 = a state machine with ten words after it: `StateMachine::StateMachine`, then +9600 to +9636 = 0 (the count, the state and eight words). Returns r3. From the front-end screens (seen: `sub_82235BD8`, `sub_822366F0` four times, `sub_82245FF8`, `sub_82246B28` four times, `sub_8224CF58`, `sub_82252290`, `sub_8225A3D8`; not seen: `sub_8223D2F0`, and `sub_821075E0` four times); the seen ones are mapped in [`screen-constructors.md`](screen-constructors.md). Seen in play. Read in full 2026-11-01. Native: `boot_mode::ScreenMachineConstruct` (`--native_boot_mode`; `StateMachine::StateMachine`'s plain C++ called directly); its callers are recompiled. harness `boot_mode` (B7 loop, 2026-11-01). |
| `sub_82107670` | `ModeType8::ModeType8` | | r3 = a type 8 mode (87,768 bytes, `Mode::Create`): `Mode::Mode`; the dispatcher at +36 emptied (as `RaceMode::RaceMode`'s); vtables `0x82006D60` (+0) and `0x82006D54` (+32); then `ModeType8Member::ModeType8Member` on +848. Returns r3. From `Mode::Create` (type 8). Not seen in play (`Mode::Create` never made type 8). Read in full 2026-11-01. |
| `sub_821075E0` | `ModeType8Member::ModeType8Member` | | r3 = the type 8 mode's +848: `sub_8223D2F0` on itself, `ScreenMachine::ScreenMachine` at +9792, `sub_8223F140` at +19544, `sub_8223DFC0` at +30912, `sub_822386F0` at +42740, `ScreenMachine::ScreenMachine` at +57992, +67632 and +77272, then +86912's byte and +86916 = 0. Returns r3. The callees are front-end code, not read. From `ModeType8::ModeType8`. Not seen in play. Read 2026-11-01. |
| `sub_821076F8` | `ModeType8::~ModeType8` | | r3 = the type 8 mode: its vtables put back, `ModeType8Member::~ModeType8Member` on +848, then the base's vtables. From `sub_82107758`. Not seen in play. Read in full 2026-11-01. |
| `sub_82106BB0` | `ModeType8Member::~ModeType8Member` | | r3 = the member: `sub_82238368` on +42740, `sub_822531F0` on +41952, `sub_82255198` on +30912, `sub_822544E8` on +30388, `sub_82257548` on +19544, then `sub_8223BF00` on itself (front-end code, not read). From `ModeType8::~ModeType8`. Not seen in play. Read 2026-11-01. |
| `sub_82107758` | `ModeType8::DeletingDestructor` | | `ModeType8::~ModeType8`, then `operator delete` when r4's bit 0 is set; returns r3. No direct callers: the type 8 vtable's (`0x82006D60`) slot 7. Not seen in play. Read in full 2026-11-01. |
| `sub_82106B60` | `RaceModeMember::DeletingDestructor` | | The race mode's member at +10512's vtable (`0x82006B60`) slot 0: `RaceModeMember::~RaceModeMember` (`sub_82106AF0`, `game-modes.md`), then `operator delete` when r4's bit 0 is set; returns r3. No direct callers (`RaceMode::~RaceMode` calls the destructor itself). Not seen in play. Read in full 2026-11-01. |
| `sub_82106718` | `App::ShutDown` (probably) | | `BootData::Unload` (`sub_821066D0`, `data-load.md`), then `sub_82106678`. From `sub_820941F0` (not seen). Not seen in play (closing the window ends the title, CLAUDE.md). Read in full 2026-11-01. |
| `sub_82106678` | `App::ShutDownServices` (probably) | | Calls, in order and with no arguments set: `sub_82106540` twice, `sub_8214E240`, `sub_820A0008`, `sub_82110388`, `sub_8209C158`, `sub_820A4398`, `sub_82106540`, `sub_8209FBC8`, `sub_8209F138`, `sub_8209D3B0`, `sub_8209BCB8`, `sub_8209F590`, `sub_8209AC40`, `sub_8209AC60` (probably the mirrors of `App::StartUp`'s set-ups, `app-start-up.md`; not read). From `App::ShutDown`. Not seen in play. Read 2026-11-01. |
