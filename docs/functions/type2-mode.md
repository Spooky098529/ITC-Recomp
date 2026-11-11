# Known functions: the type 2 mode

The type 2 front-end mode itself (`ModeType2::`, the name ours: the
career's garage menus; 852 bytes, made by `ModeType2::ModeType2` in
[`game-modes.md`](game-modes.md), released by `ModeType2::Release` in
[`front-end-screen-helpers.md`](front-end-screen-helpers.md)): its Init,
the delegate functions `Modes::Tick` runs every tick, its +32 interface
and the helpers over its block of 14 screen machines. The machines are
in [`type2-screens.md`](type2-screens.md) (the hub, the spec sheet, the
build screen, the license plate editor),
[`user-info.md`](user-info.md) (the player's record),
[`race-menu.md`](race-menu.md), [`tuning-machine.md`](tuning-machine.md),
[`car-settings.md`](car-settings.md), [`car-dealer.md`](car-dealer.md),
[`garage-screen.md`](garage-screen.md),
[`type2-dialog.md`](type2-dialog.md),
[`options-screen.md`](options-screen.md) and
[`save-screen.md`](save-screen.md).

Its vtable `0x82006B9C` has 14 slots: 0 `Init`, 1 `Release`, 2 to 5
`Mode::NoNextMode` (`sub_8222FC90`), 6 `NextMode`, 7
`Mode::DeletingDestructor` (`sub_82106E38`; both in `boot-mode.md`), and six
delegate functions the mode reaches through one-instruction thunks
(`Mode::SlotNThunk` below: r3 the 8-byte answer, r4 the mode; the
delegate at +8, pointed to by +4, holds the vtable `0x82010FA0`
(`ModeDelegate::Invoke`, [`boot-mode.md`](boot-mode.md)), the mode and
the thunk): 8 `Restart`, 9 `WaitForLoads`, 10 `LeaveScreen`, 11
`TearDown`, 12 `AfterLeaveScreen`, 13 `Leaving` (the type 4 mode's +32
vtable follows at `0x82006BD4`). Its +32 vtable `0x82006B90` has three:
`Interrupt`, `ForceExit` and `Resume`, as the type 6 and 7 modes' (who
calls them isn't known; `type6-type7-modes.md`). The type 6 and 7 modes
hold their delegate functions as plain function pointers instead.

The flow: `Init` builds the 14 machines in the 162,688-byte block at
+848 and adds them to the mode's dispatcher (+36: the handlers, the
count +236, the 50-entry message ring at +240, as `RaceMode::`'s), shows
the loading screen and sets the delegate to `WaitForLoads`. That runs
the loading screen until the 11 machines it checks have their movies
loaded (`AllLoaded`), then opens the first screen (message 6 the hub, 19
the car settings back from a test run, 10 the car dealer with an empty
garage), starts the music (bank 1's cue 2) and hands over to
`Mode::NextModeThunk` (`NextMode`, slot 6). `NextMode` queues Draw and
Update each tick and watches the shown screen (`FindShown`'s index at
block +0x27B68, the machine by `MachineOf`): its state 8 (a screen's
leave asked) goes through `LeaveScreen` (message 3 alone) and
`AfterLeaveScreen` (one tick) back to `NextMode`; 10 queues message 4
(leave every machine); 11, 12, 20 and the hub's or the car dealer's 14
pick the next mode (1 the race, 5 the parking area, 3, 4) into block
+0x27B74 and go to `TearDown`, which answers it once the stand's scene
is torn down (`StandScene::PollTeardown`).

Names are ours. Read 2026-11-11 by the B7 loop's mapping agent: every
function in full but `Init` (read for its calls and stores, its
branches traced). Fields and globals: `../known-data.md`'s "type 2 mode
block". Shared, with rows elsewhere: `Strings::Load`
([`files-and-saves.md`](files-and-saves.md)), `Archive::WaitIdle`
([`archive.md`](archive.md)), `StandScene::Create` and
`StandScene::PollTeardown` ([`type6-type7-modes.md`](type6-type7-modes.md)), `VirtualAlloc` ([`heap.md`](heap.md)),
the machines' Inits (their files above), `ScreenFades::FadeIn`
([`mode-tick.md`](mode-tick.md)), `SaveData::Of`
([`vehicle-accessors.md`](vehicle-accessors.md)), `Career::NextDay`
([`career.md`](career.md)), `LoadingScreen::Show`, `Run` and `Finish`
([`race-loading.md`](race-loading.md)), `Sound::Play`
([`menu-sounds.md`](menu-sounds.md)), `GameMessages::Dispatch`
([`game-modes.md`](game-modes.md)), `Type2Dialog::TakesInput`
([`type2-dialog.md`](type2-dialog.md)), the hub's `sub_8218F620` and
`sub_8218DE18` ([`type2-screens.md`](type2-screens.md)) and the screen
start `sub_8211AD00` ([`ui-movies-game.md`](ui-movies-game.md)).

Made native 2026-11-11 by the B7 loop (`--native_type2_mode`,
`itc_recomp/src/type2_mode/`, adapters in `src/hooks/type2_mode.cpp`,
the `type2_mode` harness, which lives in
`src/harnesses/type67_modes.cpp` for the stand's and the loading
screen's worlds): every function but `Resume` (a jump into the hub's
recompiled code), the ones never seen in play (`Restart`, `Leaving`,
`Interrupt`, `ForceExit`) ported too. The Init calls the 14 machines'
Inits through their adapters (each native behind its own flag), so those
adapters stay. The attract race doesn't reach the mode (the career's
garage menus).

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

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_82197278` | `ModeType2::Init` | Overridden | Slot 0 of vtable `0x82006B9C`, from `Modes::Tick` once the mode is made: r3 = the mode. A 224-byte frame (r19 to r31 and lr saved; r3 stored at its caller's frame's +20). `sub_8211AD00` (a screen's start); `Strings::Load([0x826C5C34], 0x100BB)`; `Archive::WaitIdle`; `StandScene::Create(3)`; mode +848 = `VirtualAlloc(0, 162688, 0x1000, 4)` (no check for 0). The 14 machines' Inits on the block, in this order (r3 the machine; their results not read): +0 `Type2Hub::InitStates`, +9640 `RaceMenu::Init`, +19792 `TuningMachine::Init`, +56624 `PowerGraphScreen2::InitStates`, +0x102D8 `UserInfo::InitStates` (`sub_8220B408`, [`user-info.md`](user-info.md)), +0x12880 `Type2Options::Init`, +0x14E28 `Type2ControllerOptions::Init`, +0x173D0 `CarSettings::Init`, +0x19978 `CarDealer::InitStates`, +0x1BF20 `BuildScreen::InitStates`, +0x23018 `GarageScreen::InitStates`, +0x255C0 `Type2SaveScreen::Init`, +0x1E4C8 `Type2Dialog::Init`, +0x20A70 `PlateEditor::InitStates`. `ScreenFades::FadeIn([0x82001BEC] (5), 0, 0, 0, 0)`. The 14 machines added to the dispatcher in the same order (handler [count] = the machine, mode +236 the count, up by one each). Player 0's save (`SaveData::Of(0)`): its day +6100 (clamped to 999) 0 → `Career::NextDay(save)`; then, with `[0x826032E0]` (the mode before) not 4 and the day (read again) 1, `Career::NextDay` again. The first screen into block +0x27B6C and +0x27B68 (`ModeType2::FindShown`'s word): 4 with the garage's count (the save's halfword +6328, read before the day's checks) at most 0; else 3 with `[0x826032E0]` 1 and the race data's (`[0x826C5B2C]`) +0xA3AF4 0 or 1; else 0. Each machine's +9612 = 1 (open on its message) and +9616 = 0 (the first four's copied from frame words +80 to +104 the Init wrote, the others stored directly; in the order hub, race menu, tuning, spec sheet, user info, options, controller options, car settings, car dealer, build screen, garage screen, dialog, plate editor, save screen). `LoadingScreen::Show(1, 0, -1)`. The delegate: mode +8 = {`0x82010FA0`, the mode, `0x8223D4C0` (`Mode::Slot9Thunk`: `WaitForLoads`), 0} (the last two words a doubleword from the frame's +104, both written), mode +4 = mode + 8 (the original's null check on +8 can't fail). Seen in play. Read in full (calls and stores; branches traced). Its group (`b7_groups.py`): it and `sub_8220B408`, ready. Plain C++ in `itc_recomp/src/type2_mode/init.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), from `Modes::Tick` through the vtable (the callees through their overrides: the 14 machines' Inits, `StandScene::Create`, `Career::NextDay`, `LoadingScreen::Show` and the rest; `SaveData::Of`'s plain C++); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_82197B00` | `ModeType2::WaitForLoads` | Overridden | Slot 9, the delegate's function while loading (set by `Init` and `Restart`, through `Mode::Slot9Thunk`), from `ModeDelegate::Invoke` every tick: r3 = the 8-byte answer, r4 = the mode. A 144-byte frame (r28 to r31 and lr saved). The answer cleared (byte +0, word +4); `FindShown(block)`; `LoadingScreen::Run(-1)`. When `AllLoaded(block)` is 1: `LoadingScreen::Finish(-1)`; by block +0x27B68: 0 → message 6 queued (the hub's), 3 → `sub_8218F620` (the hub set as if item 2 was picked, not seen in play, `type2-screens.md`) and message 19 (`CarSettings::ReenterToCourses`, `car-settings.md`), 4 → message 10 (the car dealer), others nothing (each queued as (message, 0, 0) in the ring at +240: count +600 (840), start +604, 50 entries); the delegate = {`0x82010FA0`, mode, `Mode::NextModeThunk` `0x82235C08`, 0} at +8 (+4 = +8, the frame's +88 doubleword copied); `Sound::Play(&1, &2)` (bank 1's cue 2: the music; the indexes at the frame's +88 and +80). Every call then queues message 1 (Draw) and `GameMessages::Dispatch(mode + 36)`. Returns r3 = the answer (always "none"). Seen in play. Read in full. One unseen callee (`sub_8218F620`). Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), reached through `Mode::Slot9Thunk` (its plain C++ calls this one's directly; `FindShown` and `AllLoaded` theirs, `LoadingScreen::Run` and `Finish`, `Sound::Play` and `GameMessages::Dispatch` through their overrides; `sub_8218F620`, never seen in play, a guest call to the original: its 3 branch isn't in the harness, which keeps machine 3 off the shown index); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_82197D50` | `ModeType2::NextMode` | Overridden | Slot 6, the next-mode query `Mode::NextModeThunk` calls every tick once loaded: r3 = the answer, r4 = the mode. A 144-byte frame (r26 to r31 and lr saved). The answer cleared; `FindShown(block)`; the shown machine (`MachineOf(block, [block +0x27B68])`) in state 8 → the delegate's function = `0x8223D4A0` (`Mode::Slot10Thunk`: `LeaveScreen`, the delegate rebuilt at +8 from the frame's +80); in state 10 → message 4 queued (leave: every machine's (5/9, 4) handler). Then message 1 (Draw) queued unless `[0x826C7960]` or `[0x826C7964]` (the pad not read: the system UI, probably) is set, message 0 (Update) queued, `GameMessages::Dispatch(mode + 36)`. After it, by the shown machine's state (looked up again): 11 → mode 1 (the race), 12 → 5 (the parking area), else the hub's state (block +9604) or the car dealer's (+0x19978 + 9604) 14 → 4, with `[0x826364EC]` = 0, else the shown machine's state 20 → 3; for each, block +0x27B74 = a doubleword from the frame's +80: byte 1, three bytes the frame holds (not written), the mode's word; and the delegate's function = `0x8222FC60` (`Mode::Slot11Thunk`: `TearDown`). Returns r3 = the answer (always "none" here). Seen in play. Read in full. Blocked (`b7_groups.py`) by `MachineOf` and `FindShown` only. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), reached through `Mode::NextModeThunk` (boot_mode's, a guest call) (`FindShown` and `MachineOf` their plain C++, `GameMessages::Dispatch` through its override); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_82197C88` | `ModeType2::LeaveScreen` (probably) | Overridden | Slot 10, the delegate's function for one tick after `NextMode` saw the shown machine in state 8: r3 = the answer, r4 = the mode. A 128-byte frame (r29 to r31 and lr saved). The answer cleared; `FindShown(block)`; message 3 queued alone (the machine's (8, 3) handler: `ScreenMachine::Wait` and the like), `GameMessages::Dispatch(mode + 36)` (no Draw or Update this tick); the delegate's function = `0x8222FC70` (`Mode::Slot12Thunk`: `AfterLeaveScreen`). Returns r3. Seen in play. Read in full. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), reached through `Mode::Slot10Thunk` (its plain C++ calls this one's directly); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_821977E0` | `ModeType2::AfterLeaveScreen` (probably) | Overridden | Slot 12, the delegate's function for the tick after `LeaveScreen`: r3 = the answer, r4 = the mode. A 96-byte frame. The answer cleared; `FindShown(block)`; the delegate's function = `Mode::NextModeThunk` (`0x82235C08`) again. Nothing queued or dispatched (a tick with no Draw or Update). Returns r3. Seen in play. Read in full. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), reached through `Mode::Slot12Thunk` (its plain C++ calls this one's directly); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_821970C0` | `ModeType2::TearDown` | Overridden | Slot 11, the delegate's function after `NextMode` (or `Leaving`) picked the next mode: r3 = the answer, r4 = the mode. A 112-byte frame. The answer cleared; when `StandScene::PollTeardown` answers nonzero, the answer = block +0x27B74 and +0x27B78 (the 8 bytes `NextMode` or `ForceExit` wrote, leftover bytes included), so `Modes::Tick` changes mode. Nothing queued or dispatched. Returns r3. Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), reached through `Mode::Slot11Thunk` (its plain C++ calls this one's directly; `StandScene::PollTeardown` through its override); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_82197A30` | `ModeType2::Restart` (probably) | Overridden | Slot 8, through `Mode::Slot8Thunk` `sub_8223D4B0` (no setter of it found in the type 2 mode's code; not looked for elsewhere): r3 = the answer, r4 = the mode. The answer cleared; the delegate's function = `Mode::Slot9Thunk` (`WaitForLoads`); message 1 (Draw) queued and `GameMessages::Dispatch(mode + 36)`. Returns r3. A 112-byte frame. Not seen in play. Read in full. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), reached through `Mode::Slot8Thunk`; ported though never seen in play; checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_82198008` | `ModeType2::Leaving` (probably) | Overridden | Slot 13, through `Mode::Slot13Thunk` `sub_8222FC80` (set by `ForceExit`): r3 = the answer, r4 = the mode. A 128-byte frame (r29 to r31 and lr saved). The answer cleared; message 4 queued (leave) and `GameMessages::Dispatch(mode + 36)`; then for r4 from 0 to 46, `MachineOf(block, r4)`: when every machine found is in state 6, 11, 14, 12 or 20, the delegate's function = `Mode::Slot11Thunk` (`TearDown`). Returns r3. Not seen in play. Read in full. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), reached through `Mode::Slot13Thunk`; ported though never seen in play; checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_82197858` | `ModeType2::Interrupt` (probably) | Overridden | The +32 vtable's (`0x82006B90`) slot 0: r3 = the mode + 32 (its +816 the block). A 96-byte frame. As `Type6Mode::Interrupt` (`type6-type7-modes.md`) without the hold: 0 until `AllLoaded(block)` is 1; then with the save screen's state (block +0x27B44, the machine at +0x255C0) 5, or the options settings' (`[0x826D8478]`) +712 0: message 48 queued (the dialog's `Type2Dialog::Cancel`, `type2-dialog.md`, and the hub's data +20; in the ring at r3 + 208 = mode + 240; no dispatch), 0 returned; else 1 when the dialog's status `[0x82636204]` is 3 (hidden); else when `Type2Dialog::TakesInput` answers nonzero message 48 and 0, else 0. Not seen in play. Read in full. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), through the +32 vtable (`Type2Dialog::TakesInput`'s plain C++); ported though never seen in play; checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_82197930` | `ModeType2::ForceExit` (probably) | Overridden | The +32 slot 1: r3 = the mode + 32, r4 = a number. r4 0 or 1: the delegate's function (at r3 - 32 + 8, built at r1 - 16, below the stack pointer) = `Mode::Slot13Thunk` (`Leaving`), block +0x27B74 = {byte 1, three bytes below the stack pointer (r1 - 15 to -13, not written), the word 4}, `[0x826364EC]` = 1; r4 2 or 3: the same with 12 and `[0x826364EC]` left; other values nothing. Leaf. The same 4 / 12 mapping as `Type6Mode::ForceExit`, without its hold. Not seen in play. Read in full. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), through the +32 vtable; ported though never seen in play. Correction (2026-11-11): the three bytes after the answer's byte 1 are written: the low three bytes of `0x8222FC80`, the functor's word the original stored at r1 - 16 first (`0x22`, `0xFC`, `0x80`); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_821970B8` | `ModeType2::Resume` (probably) | | The +32 slot 2: a jump to the hub's `sub_8218DE18` (r3 = the mode + 32, not read there): with the hub's data +20 (set by message 48) set, it's cleared and, when `Type2Hub::NextNotice2` finds a notice, the hub goes to page 5 (`type2-screens.md`). Not seen in play. Read in full. Left recompiled (B7 loop, 2026-11-11): a jump into the hub's recompiled `sub_8218DE18` (not read), nothing of its own to port; never seen in play. |
| `sub_82197130` | `ModeType2::AllLoaded` | Overridden | r3 = the block: 1 when the +9608 word (the movie loaded, set by each machine's `WaitForLoad`) of 11 machines is nonzero (the hub, +9640, +19792, +0x173D0, +0x19978, +0x23018, +0x1BF20, +0x12880, +56624, +0x102D8, +0x20A70; not the controller options +0x14E28, the dialog +0x1E4C8 or the save screen +0x255C0) and block +0x27B48 is nonzero too, else 0. Leaf. From `WaitForLoads` and `Interrupt`. Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/type2_mode/block.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), kept for its recompiled callers (the type 2 mode's own callers, `WaitForLoads` and `Interrupt`, call the plain C++); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_82197218` | `ModeType2::FindShown` (probably) | Overridden | r3 = the block: for r4 from 0 to 7, `MachineOf(block, r4)`; when it's in state 5 or 4 (running or starting), block +0x27B68 = r4 (the last such wins). The eight are the hub and the screens its items open (the race menu, the tuning screen, the car settings, the car dealer, the garage screen, the build screen, the options). A 96-byte frame. From `WaitForLoads`, `NextMode`, `LeaveScreen` and `AfterLeaveScreen`. Seen in play. Read in full. Blocked by `MachineOf` (ready). Plain C++ in `itc_recomp/src/type2_mode/block.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), kept for its recompiled callers (the delegate functions call the plain C++); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_82196FC0` | `ModeType2::MachineOf` (probably) | Overridden | r3 = the block, r4 = an index: the machine's address, or 0: 0 the hub (+0, through `sub_82197000`), 1 +9640 (the race menu), 2 +19792 (the tuning screen), 3 +0x173D0 (the car settings), 4 +0x19978 (the car dealer), 5 +0x23018 (the garage screen), 6 +0x1BF20 (the build screen), 7 +0x12880 (the options), 9 +56624 (the spec sheet), 10 +0x102D8 (the user info), 12 +0x14E28 (the controller options), 16 +0x1E4C8 (the dialog), 26 +0x20A70 (the plate editor), 44 +0x255C0 (the save screen); 8, 11 and the rest 0. Leaf (a jump table on 0 to 12). Seen in play. Read in full. Ready (with `sub_82197000`). Plain C++ in `itc_recomp/src/type2_mode/block.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), kept for its recompiled callers (the type 2 screens' code; `FindShown`, `NextMode` and `Leaving` call the plain C++), the case 0 `sub_82197000` merged; checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_82197000` | | Merged | `ModeType2::MachineOf`'s case 0 (the recompiler split it at the jump table's first target): returns r3 (the block, the hub's machine). Seen in play. Read in full. In `MachineOf`'s group. Merged into `MachineOf`'s plain C++ (`itc_recomp/src/type2_mode/block.cpp`, `--native_type2_mode`), its case 0; checked by the `type2_mode` harness's `MachineOf` cases (B7 loop, 2026-11-11). |
| `sub_8223D4B0` | `Mode::Slot8Thunk` | Overridden | r3 = the answer, r4 = an object: tail-calls the object's vtable slot 8 (+32) with them. A delegate's function for any mode (the type 2 mode's slot 8 is `Restart`; other setters not looked for). Leaf. Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), `type2_mode::SlotThunk` (slot 8): the type 2 mode's delegate functions called as plain C++, any other mode's function a guest call from the caller's frame with its return address (a tail call); reached through `ModeDelegate::Invoke` (a guest call); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_8223D4C0` | `Mode::Slot9Thunk` | Overridden | The same for slot 9 (+36): the type 2 mode's `WaitForLoads` (set by `Init` and `Restart`). Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), `type2_mode::SlotThunk` (slot 9): the type 2 mode's delegate functions called as plain C++, any other mode's function a guest call from the caller's frame with its return address (a tail call); reached through `ModeDelegate::Invoke` (a guest call); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_8223D4A0` | `Mode::Slot10Thunk` | Overridden | The same for slot 10 (+40): the type 2 mode's `LeaveScreen` (set by `NextMode`). Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), `type2_mode::SlotThunk` (slot 10): the type 2 mode's delegate functions called as plain C++, any other mode's function a guest call from the caller's frame with its return address (a tail call); reached through `ModeDelegate::Invoke` (a guest call); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_8222FC60` | `Mode::Slot11Thunk` | Overridden | The same for slot 11 (+44): the type 2 mode's `TearDown` (set by `NextMode` and `Leaving`). Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), `type2_mode::SlotThunk` (slot 11): the type 2 mode's delegate functions called as plain C++, any other mode's function a guest call from the caller's frame with its return address (a tail call); reached through `ModeDelegate::Invoke` (a guest call); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_8222FC70` | `Mode::Slot12Thunk` | Overridden | The same for slot 12 (+48): the type 2 mode's `AfterLeaveScreen` (set by `LeaveScreen`). Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), `type2_mode::SlotThunk` (slot 12): the type 2 mode's delegate functions called as plain C++, any other mode's function a guest call from the caller's frame with its return address (a tail call); reached through `ModeDelegate::Invoke` (a guest call); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
| `sub_8222FC80` | `Mode::Slot13Thunk` | Overridden | The same for slot 13 (+52): the type 2 mode's `Leaving` (set by `ForceExit`, not seen in play; another mode's slot 13 ran). Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/type2_mode/delegates.cpp` behind an adapter in `src/hooks/type2_mode.cpp` (`--native_type2_mode`), `type2_mode::SlotThunk` (slot 13): the type 2 mode's delegate functions called as plain C++, any other mode's function a guest call from the caller's frame with its return address (a tail call); reached through `ModeDelegate::Invoke` (a guest call); checked by the `type2_mode` harness (B7 loop, 2026-11-11). |
