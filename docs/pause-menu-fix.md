# Pause menu: input repeating too fast

In a race, the pause menu's input repeats too fast with frames uncapped
or above 30 fps: holding a direction moves through the menu one item every
tick once the repeat starts, and the repeat starts too soon. This is the plan to fix it and to translate the game's input
handling to native code. Step 0 (logging) is done and its results are in
"Step 0 results" below; the code that repeats too fast is found: the action
mappers' update `8210B018` runs once a frame (2026-10-18). The fix is
planned in "Revised plan" (not started).

Addresses below are guest addresses. `0x82000A30` and friends are floats in
the game's constant data, read with `guest::LoadU32` and bit-cast (their
values, logged in step 0, are in "Step 0 results"). Offsets are
in bytes. Every function named can be read in `generated/` (find it with
`grep -n "DEFINE_REX_FUNC(sub_XXXXXXXX)" itc_recomp/generated/default/*.cpp`).

## What's known

### The workaround on `pause-menu` (commit `78456a5`, to be removed)

`sub_8215B4F8` (the race draw step) is wrapped in
`src/frame_pacing/game_messages.cpp` to clear `g_race_draw_step_seen` when
`LoadU32(ctx.r31 + 588)` is non-zero. The pass is then not "in a race", so the
pacer runs every pass as a 30 Hz logic pass (as in menus) and the repeat
goes back to normal. The cost: the pause screen drops to 30 fps, and it takes a
fraction of a second for the frame rate to come back on resume.

What it reads isn't the flag `sub_8215B4F8` checks:

- `ctx.r31` on entry isn't an argument. The only caller, `sub_8215C770`,
  loads a global (`lwz r3,-32044(r11)`) and jumps on; r31 is whatever was
  left in it before.
- Inside, `sub_8215B4F8` sets r31 to the constant `0x826BAC00` before it
  reads +588. That address is an array of 21 records of 592 bytes (the camera
  passes, updated by `sub_820A0958` each). +588 is the last word of record
  0, and +1180 and +1772 are the same word in records 1 and 2 (read instead
  when the mode word at `[0x826C5B2C] + 0xA3AF0` is 4).
- There, 0 skips the race's body. Our check treats non-zero as paused,
  so it's reading some other object that happens to change on pause.

Step 0 showed `ctx.r31 + 588` isn't a pause flag either (it's 1 during the
race intro too); see "Step 0 results". The workaround is now behind
`--pause_workaround` (on by default; `=false` for tests).

### Input::Update (`sub_820990F0`)

The only path to the controllers: `sub_825BA3F8` (XInputGetState) has one
caller, `sub_82591C18`, whose only caller is `sub_820990F0`. It's already
overridden in `src/frame_pacing/game_messages.cpp` to be skipped on
render-only passes, so pad state changes once a tick. r3 = the input
manager, laid out as:

| Offset | What | Count × size |
|---|---|---|
| +8 | logical pads | 5 × 196 |
| +988 | physical pads | 4 × 212 |
| +1840 | per-pad processors (unknown) | 5 × 416 |
| +3924 | listener slots | 5 × 12 |

In order, it:

1. Reads each physical pad `i` (0-3) with `sub_82591C18(r3 = mgr + 988 +
   212i, r4 = pointer to a word holding i)`. Details below.
2. Builds each logical pad `j` (0-4) with `sub_82591918(r3 = mgr + 8 +
   196j)`. Details below.
3. For each processor `j`: calls its vtable slot 1 (`load(load(obj) + 4)`)
   with r3 = the processor (`mgr + 1840 + 416j`), r4 = logical pad `j`. These
   are probably where presses and repeats are worked out (not checked).
4. For each listener slot `j` (`s = mgr + 3924 + 12j`): skipped if
   `load(s + 4)` (a count) is 0. Otherwise `head = load(s)`, `node =
   load(head + 4)`; if `node == head` the game traps (`twi 31`, an empty
   list with a count, never expected). Otherwise it calls vtable slot 1 of
   `object = load(node + 8)` with r3 = object, r4 = logical pad `j`. Only the
   first node of each list is called.

Since pad state already changes once a tick, a native Input::Update with
the same results won't fix the pause menu on its own. What counts repeats
per frame is the action mappers' update (`8210B018`), which reads this
state on every pass; see "The pause menu's code" and its result below.

### Physical pad (`sub_82591C18`), 212 bytes

r3 = pad, r4 = pointer to the user index.

Layout: +4 flags, +8..+167 40 floats of values (cleared every read), +168
and +180 the two rumble motors (12 bytes each), +192 capabilities.

Flags at +4: bit 0 (1) connected, bit 1 (2) connected on this read, bit 2
(4) disconnected on this read. Exactly:

- `was = flags & 1` (read before anything else).
- `memset(pad + 8, 0, 160)`, then `XamInputGetState(r3 = user, r4 = 0, r5 =
  state)` (through `sub_825BA3F8`, which moves r4 to r5 and sets r4 = 0).
  The state is a 16-byte XINPUT_STATE on the stack: packet number +0,
  buttons (u16) +4, left trigger (u8) +6, right trigger (u8) +7, LX +8, LY +10,
  RX +12, RY +14 (s16 each; guest big-endian).
- **On failure** (r3 ≠ 0): clear bit 0. If `was`, set bit 2, else clear
  bit 2. Return: nothing below runs, so the values stay zero and rumble
  isn't sent.
- **On success**: set bit 0. If `was`, clear bit 1. Otherwise set bit 1 and
  call `XamInputGetCapabilities(r3 = user, r4 = 1, r5 = pad + 192)`
  (`sub_825BA3F0`). Bit 2 is left as it was.

Rumble, on success. A motor is {+0 float speed, +4 int "held", +8 int
ticks left}:

- Each motor ticks down (`sub_825A1900`): if held is 0 and ticks left > 0,
  decrement; when it reaches 0, speed = float `0x82000A34`. Since
  Input::Update runs once a tick, this counts ticks.
- Each speed (`sub_825A18F8` just loads +0) is multiplied by float
  `0x8200223C`, truncated toward zero to an int64 (`fctidz`: NaN gives
  `0x8000000000000000`, values above `LLONG_MAX` give `LLONG_MAX`, as in the
  generated code), and its low 16 bits are kept.
- Sent as XINPUT_VIBRATION {left u16 from +168, right u16 from +180} with
  `XamInputSetState(r3 = user, r4 = 0, r5 = vibration, r6 = r7 = r8 = 0)`
  (`sub_825BA408`).

Values (float slot `n` is at `pad + 40 + 4n`; all `float × float`
products, which the guest rounds once to float, so a native `float`
multiply gives the same bits):

- Buttons: slot = 1.0 (float `0x82000A30`) when its bit is set in the
  buttons word. (mask, slot) pairs: (0x0001, 0) d-pad up, (0x0002, 1) down,
  (0x0004, 2) left, (0x0008, 3) right, (0x1000, 4) A, (0x2000, 5) B,
  (0x4000, 6) X, (0x8000, 7) Y, (0x0100, 12) LB, (0x0040, 14) left stick
  click, (0x0200, 16) RB, (0x0080, 18) right stick click, (0x0010, 20)
  Start, (0x0020, 21) Back.
- With `T` = float `0x82002200`, `S` = float `0x82002BD4`, `K` = float
  `0x8206CBFC`, and the inputs converted to float:
  - +20 and +36 = float `0x82000A34`
  - +16 and +92 (slot 13) = LT × T; +32 and +108 (slot 17) = RT × T
  - +8 = LX × S; +12 = LY × S; +24 = RX × S; +28 = RY × S
  - slots 24-31 (+136..+164) = LY×S, LY×K, LX×S, LX×K, LY×S, LY×K, LX×S,
    LX×K

### Logical pad (`sub_82591918`), 196 bytes

r3 = logical pad. +4..+35: 4 device bindings of 8 bytes, {u8 enabled at
+4 + 8k, device pointer at +8 + 8k}. +36..+195: 40 floats of values.

- `memset(pad + 36, 0, 160)`.
- For each binding with enabled ≠ 0 and pointer ≠ 0: call the device's
  vtable slot 1 (r3 = device), which returns a pointer `v` to 40 floats, and
  add `v[m]` to value `m` (`pad + 36 + 4m`) for m = 0..39, in order
  (float adds, rounded each time).
- Clamp, with `lo` = float `0x82001BE0`, `hi` = float `0x82000A30`:
  values 0-7 (+36..+64): if `x > hi` then hi, else if `x < lo` then lo.
  Values 8-39 (+68..+192): if `x > hi` then hi; no lower clamp. NaN is
  kept in both (the compares are false).

A physical pad's values (+8, 40 floats) line up with these 40, so its
vtable slot 1 probably returns `pad + 8`: check that in step 0 (log the
device pointers' vtables and whether `v == device + 8`).

## Plan

### Step 0: logging (`--log_input`)

Where to add it:

- The `sub_820990F0` override in `src/frame_pacing/game_messages.cpp`
  (r3 = manager) for the manager's objects.
- `frame_pacing::GameMessages_CurrentHandler(handler, message)`
  (`game_messages.h`) gives the message handler running (0 and `~0u` outside
  one). Tick handlers don't set it: `--log_tick_handlers` (the
  `sub_82099A98` override, same file) lists tick handlers run on render-only
  passes, once a second.
- `GameMessages_IsRenderOnlyPass()` and `GameMessages_InRace()` say what the
  pass is.

What to log:

- Once (on the first call in a race): the manager address; each processor's
  vtable and slot 1; each listener slot's count and, when set, its object's
  vtable and slot 1; each logical pad's bindings (enabled, pointer, the
  device's vtable and slot 1, whether `v == device + 8`); and the floats at
  `0x82000A30`, `0x82000A34`, `0x82001BE0`, `0x82002200`, `0x8200223C`,
  `0x82002BD4` and `0x8206CBFC`.
- On every change: `LoadU32(0x826BAC00 + 588)`, `ctx.r31` and
  `LoadU32(ctx.r31 + 588)` as `sub_8215B4F8` is entered, to find the real
  pause flag and what object `78456a5` reads.
- Once a second, by pass type (logic or render-only) and by the pause sign
  above: the message handlers delivered (`Deliver` in `game_messages.cpp`)
  and the tick handlers run, to find what runs on render-only passes only
  while paused.
- The user runs a race, pauses, holds a direction in the menu and quits
  (`run-debug.sh`-style flags from CLAUDE.md plus `--log_input=true`); read
  the log before choosing step 2.

### Step 0 results (2026-10-18)

From an attract race and three races the user drove, paused, held a
direction in the pause menu, unpaused and quit (`--log_input=true
--pause_workaround=false`, then `--log_replays`, `--find_per_frame_state
--find_any_steps` and `--log_input_writes`).

**Constants**:

| Address | Bits | Value |
|---|---|---|
| `0x82000A30` | `3F800000` | 1 |
| `0x82000A34` | `00000000` | 0 |
| `0x82001BE0` | `BF800000` | -1 |
| `0x82002200` | `3B808081` | 1/255 |
| `0x8200223C` | `477FFF00` | 65535 |
| `0x82002BD4` | `38000100` | 1/32767 |
| `0x8206CBFC` | `B8000100` | -1/32767 |

**The manager's objects** (manager at `0x40059F50` in every run):

- Physical pads: vtable `0x8206CBF4`, slot 1 `0x820988B8`, which returns
  `device + 8` (checked by calling it).
- Logical pads 0-3: one binding each (binding 0, enabled), physical pad
  `j`. Bindings 1-3 are empty.
- Logical pad 4 ("any pad"): all four bindings enabled, bound to logical
  pads 0-3 (vtable `0x8206CBEC`, slot 1 `0x82591B30`, which returns
  `device + 36`, the logical pad's values). It's built after them, so step
  1 can sum their values natively too.
- Processors 0-4: all vtable `0x8206C0C4`, slot 1 `0x82592188`.
- Listener slots 0-3: count 1, the first object of the same class (vtable
  `0x8206C0C4`, slot 1 `0x82592188`), at `0x40071AE0`, `0x40072640`,
  `0x400731A0` and `0x40073D00`. Slot 4 is empty.

**Processors and listeners' state** (`--log_input_writes`, 416 bytes each).
Processors 0 and 4 and listener 0 change alike when pad 0 is used: floats at
+20..+104 (stick values; -1 and 1 for a full push), button masks at +108,
+10C and +110 (e.g. `22000000`), and two counters at +180 and +190 that go
up by one each tick while a direction is held (seen up to 0x17) and go back
to 0 when it's let go. Nothing but Input::Update (tick handler
`0x820990F0`) writes to the manager or the listener objects, and nothing
writes to them on render-only passes: whatever repeats too fast only reads
them, and keeps its own state elsewhere.

**Pause signs.** Neither candidate is a pause flag:

- `LoadU32(0x826BAC00 + 588)` (what `sub_8215B4F8` checks) becomes 1 when
  the race starts and stays 1, paused or not. Records 1 and 2 stayed 0.
- `ctx.r31` on entry was the same object all race (`0x417424B0` in the
  user's races, `0x40B00060` in the attract race), and its +588 is 1
  during the race intro and while paused, 0 while driving. In the attract
  race it's 1 throughout. The workaround worked only because of that.
- What does mark pausing: message `0x1D`, sent on pause (to `8215C750`,
  `8216D600`, `82175AE8`) and on resume (to `8215C750`, `8216C658`,
  `82175B80`), and the Update and Draw handlers that change (below).

**What runs, by phase** (`input runs` lines). Each phase has its own set of
Update (message 0) handlers and one Draw (message 1) handler, besides those
always there (`82106540`, `8215D538`, `8215C770`):

| Phase | Update handlers | Draw handler |
|---|---|---|
| Race intro | `821594B8`, `8215E150`, `8215FC28`, `82168B80`, `8216C530`, `82174BC0`, `821767A8` | `8216C5B8` |
| Driving | `821594B8`, `8215E150`, `82160940`, `82169290`, `8216C210`, `82174BC0`, `821767A8` | `8216C2A0` |
| Paused | `8215E158`, `8216DB70`, `82170560` | `8216CF08` |

While paused, the game still runs 120 render-only passes a second beside its
30 logic passes. On them: Draw (`82106540` 7 times, `8215C770`,
`8216CF08`) and all the tick handlers (`820902C8`, `82091A58`,
`820920D8`, `82093DC0`, `82096330`, `82096340`, `82096350`, `820988C8`,
`820990F0` (skipped inside), `82099828`, `820A4610`), the same set as in
every other phase. No Update reaches the pause screen's handlers on
render-only passes (only `8215E150`, the race overlays, and not while
paused). So the pause screen's Draw handler `8216CF08` is the only code
that runs on render-only passes only while paused.

**Ruled out:**

- The UI movie replay (`sub_8211D2F8`, `--per_tick_ui_movies`): with
  `--log_replays` it replayed every call while paused (~209 a second), none
  "changed since the logic pass", so UI movies advance once a tick there.
- `--find_per_frame_state --find_any_steps` while paused found only
  drawing bookkeeping (command lists, constants) that changes on every pass
  in every phase; no menu selection.

- The pause screen's Draw handler: with `--render_only_skip=8216CF08` the
  menu still repeats too fast.

**The game's own repeat timing is right.** Listener 0's counter at +180
counts ticks while a direction is held, reaches `0x18` (24 ticks), then
cycles `0x14` -> `0x18` (a repeat every 4 ticks). Only Input::Update writes
it, once a tick. So whatever repeats too fast acts on that state once a
frame instead of once a tick, or keeps its own timer.

**Scene::Update (tick handler `820988C8`).** Already overridden in
`src/hooks/title_screen.cpp` (for `--log_boot_modes`). It runs on every
pass, render-only ones too, and updates the front end's screens outside
the Update and Draw messages. With r3 = the scene (`0x40026FE0` in an
attract run), it:

1. calls `sub_8258C9C0` and `sub_8258C998` on r3+8 (a double buffer: flips
   the index at +4, moves the count at +12 to +16 and clears it);
2. calls `sub_82591838`, which flips the global `0x82970BC4` between 0 and 1;
3. calls slot 6 and slot 2 of the screen object embedded at r3+64 (vtable
   `0x8206CB8C`: slot 6 = `0x8258E5E0`, slot 2 = `0x8258ED78`).

Skipping parts of it on render-only passes isn't usable (`--render_only_scene_skip`,
commit `d68b95b`, reverted in `0df87df`): skipping all of it crashed race
loading (an endless read of guest `0x18`); skipping slot 6 froze the race's
loading screen; skipping the flag flip ran the game at ~1800 ticks/s, so
`0x82970BC4` is part of the tick timing, not input. That skipping changed
the loading screen at all suggests render-only passes run during race
loading (not checked).

**Working hypothesis (not proven):** code that runs on every pass while
paused, most likely under the screen update (slot 6 `0x8258E5E0` or slot
2 `0x8258ED78`), reads the tick's input state and acts on it on each of a
tick's 4-5 frames. Or the menu keeps its own repeat timer and advances it
once a frame. The fixes differ (act once a tick against count ticks), so
find which by logging which code reads the input state, on which kind of
pass.

**The button tests.** The processors' and listeners' state is read
through three small functions, each r3 = processor (or listener), r4 =
pointer to a button mask, returning a bool:

- `825924E0`: every button in the mask held (`(+272 & mask) == mask`).
- `82592500`: any button in the mask set in +264.
- `82592520`: pressed or repeating. For the first held button of the mask
  (bit `b` set in +272), its counter at `+284 + 4b` is 1 or has reached the
  threshold at +276. (Listener 0's counters at +0x180 and +0x190 are bits
  25 and 29's.) This is what a menu's repeat would use.

Their only direct callers are the wrappers `8258CF10`, `8258CEB0` and
`8258CEE0` (same arguments, the mask copied to the stack). Every direct
call of those is from the tick handler `82093DC0` (and `8210C370` for
`8258CF10`). `82093DC0` runs on every pass, but it tests masks
`0x00100000` and `0x00200000` (Start and Back) and switches a mode with
flags at +112/+113, so it isn't the menu's d-pad. The menu must call the
wrappers or tests through a vtable, which grep can't find, so
`--log_input_reads` logs every call of the six once a second by caller
(`lr`), pass type, pause (the pause screen's Draw handler delivered on
the pass before), the message or tick handler running, object, mask and
result. A caller getting `1` on render-only passes while paused is the reader.

Result (a race the user drove, holding down on the analogue stick in the
pause menu): none of the six is called on render-only passes, and the only
caller is the tick handler `82093DC0` (through `8258CF30` and `8210C624`
too, indirectly). While paused every test returned 0, for every mask from
`0x1` to `0x200000`: the stick isn't a button bit there (it's in the
floats at +20..+104 and the counters at +0x180/+0x190, bits 25 and 29,
which these callers never test). So the menu doesn't use the button tests.
`821BAB58` and `821BAC78`, small functions reading +384 and +400, read a
global (key bindings), not the input state. Next: `--log_input_reads` also
logs the callers of the pads' values methods (`82591B30` logical,
`820988B8` physical), which return the 40 floats with the stick.

Result: neither is called by the menu. Input::Update calls them, and
`820988B8` turns out to be a generic "return this + 8" shared with draw
code (called from draws on the scene object on every pass). So the menu
reads the input state with inline loads, which can't be hooked.

**Finding it from what it writes (`--find_menu_selection`,
`src/debug/menu_selection_scan.cpp`).** While paused, guest memory is
compared over a render-only pass every 300 ms; words changing by small
integer steps only while the stick is held (listener 0's +0x180 or +0x190
non-zero) are watched around every tick handler, message handler and
Scene::Update screen call, on every pass, and each change is logged with
the pass type. In a race the user drove (16 idle, 22 held samples) most
candidates were draw bookkeeping (changed inside the Draw handlers
`8215C770` and `8216CF08`). One wasn't: **`40000168`** is set on
render-only passes by the tick handler `82093DC0` outside any message
(`0 -> 8`, `0 -> 4`, `0 -> 5`), and cleared on the next logic pass by the
pause screen's Update handler `8216DB70` (`8 -> 0`, `4 -> 0`, `5 -> 0`).
Written every frame and consumed once a tick by the pause menu; 4, 5 and 8
would fit a count of frames (~4.8 a tick at 144 fps). Not proven.

`82093DC0` is the app's update: besides the message dispatch it runs
component lists (at +28, +40, +52 through `82092BF0`, +76 through
`82092CD0`, +96 through `82091C48`), calling `82092968` per entry, which
calls the component's vtable slot 1 (r3 = component, r4 = app + 20).
Next: `--menu_watch=40000168` (watching without the scan) with
`82092968` wrapped, to name the component writing it.

Result (`--menu_watch=40000168`, a race the user drove): outside the
pause `40000168` takes unrelated values (256, `0x2000000`, 8192) from many
handlers, so it's reused memory, not one variable. While paused it holds
bit flags (1, 2, 8, 16, 40 = 32|8, 64, 512): set by a component that is a
delegate (`82233460`, its target at +8), mostly on render-only passes and
sometimes several times in a tick (`8 -> 40`, `8 -> 513 -> 512`); read and
cleared by the pause screen's Update `8216DB70` on the next logic pass
(inside the component `8223A3F0`, the message dispatch). So a component
run every frame posts events the pause menu acts on. Next: the delegate's
target and the component's address (the wrapper now resolves delegates).

Result: the writer is component update `82119118`, run on every pass. It
calls `8259D1A8` on this+16 (a jump to vtable slot 4 of the object at
+8), `82094B10` on this+1584 when +1596 is 3, `821184E8` twice,
`82118FB0` and `82118C18` (removes the entries of a list at this+1528
that a callback picks). With those wrapped, `40000168` was written by
`8259D1A8` (from component updates `82119118` and `82117E88`, which jumps
to it on this+16) and by `82118C18`. Generic forwarding and list removal
writing a word many unrelated handlers also write outside the pause:
probably heap or list bookkeeping, not input events. **Dead end** (not
proven). The memory scan found no other candidate outside the Draw
handlers.

**The pause menu's code (read 2026-10-18).** From the pause screen's
Update `8216DB70` down; nothing here reads the pads' floats directly:

- `8216DB70`, when its state (+9612) is 6, updates the HUD under it
  (`822153A0`, `82212C50`, `82215488` on `[+9608] + 16`) and calls
  `8220F638`, which calls one of six functions by the index at
  `[[0x826D84D8]]` (-1: none): `8220D898`, `8220DB98`, `8220E988`,
  `8220EBD8`, `8220C578`, `8220C5D0`. They are the pause menu and its
  sub-screens; `8220ba68([0x826D84D8], state, ...)` switches between them.
- `8220D898` (the main list) queues each item's text (`82106540`, x 100,
  y 100 + 20i), calls the list update `8220C1E0` on the menu object
  `[0x826D84DC]`, and plays sound 4 (`821103F0`) when the selection
  (`8220BC20`, +16) differs from the previous one (`8220BC28`, +20).
  `8220BD50` is "confirm".
- **The menu list** (`8220C1E0`, generic: ~50 menus call it): +0 item
  count, +4 scroll, +12 items shown, **+16 selection** (relative to the
  scroll), +20 previous selection, +24 last move (1 down, -1 up), +28
  left/right, +36/+40/+44/+48 the action ids for down, up, left, right,
  +60 the player, +64 wraps, +68 per-item "disabled" flags. It moves by
  one item per call when `8220C020(menu, action)` is true.
- `8220C020` tests action 153 or 167 (up), 154 or 168 (down), 155/169,
  156/170 (left, right: d-pad and stick ids, either one) with vtable slot
  12 of an **action mapper**: `[0x826BAB80 + 8 * [0x826BABA4 + 4 *
  menu[+60]]]` (skipped when the entry's +4 is 1). Mappers 0-3 are at
  `0x40071140`, `0x40071CA0`, `0x40072800`, `0x40073360` (at boot; in
  the user's runs the listener objects are at mapper + `0x9A0`).
- **The action mapper** (vtable `0x82007F78`, 15 slots; 30 actions,
  ids 151-180, index = id - 151 through `8210B140`). Slot 12 `8210C010`:
  bit `index` of the "fired" word at +464. Slot 9 `8210BED8`: the word at
  +452. Slot 1 `8210BDB8` resets. Its input update `8210C370` (called by
  the app's update `82093DC0`) reads the stick floats through `[+2880]`
  and the d-pad through the listener at +2464 (`0x9A0`, with the button
  test `8258CF10`), then `8210C0E0` -> `8210BBA0` rebuilds, at +448:
  held, held before, pressed (new), released, and **fired** (+464) =
  pressed, or'ed with each action's repeat object (pointers at +472,
  vtable slot 1 called with "held") saying it repeats. Repeat parameters
  per action are set at boot (`8209F228`) from the table at `0x82600B88`.
  `8210B568` looks like one such repeat: counts calls while held, fires
  at a start count and then every period, so it counts updates, not time.
- So the selection moves once per `8220C1E0` call while the fired bit is
  set, and the fired bit changes once per `8210C370` call. If both ran once
  a tick the repeat would be right. Earlier logs say the button tests that
  `8210C370` calls run on logic passes only; nothing yet says how often
  `8220C1E0` runs per mapper update.

`--log_pause_menu` (`src/debug/pause_menu_log.cpp`) logs every
`8220C1E0` call that moves a selection or sees a direction held or fired
(tick, pass, caller, list, whether it's the pause menu's, selection and
scroll before and after, the mapper's held and fired words), and every
`8210C370` call with a direction held or fired (fired before and after).
Direction bits: 2 (153) and 3 (154) d-pad up/down, 16 (167) and 17 (168)
stick up/down.

**Result: found** (`--log_pause_menu`, a race the user drove at 144 fps,
`itc_024.log`). The mapper's input update `8210C370` runs on **every
pass**, render-only ones too; the menu list `8220C1E0` (the pause menu's,
`[0x826D84DC]` = `0x01A00000`, called from `8220D970` in `8220D898`) runs
on logic passes only. `8210C370` is called by `8210B018`, which loops over
the objects of type 2 (`8259C898` count, `8259C818` get) and updates each
mapper; it's run from the app's update `82093DC0` on every pass. Holding
the stick down (fired bits `08020000`: bit 17, stick down, and bit 27):

- Tick 1415: pressed, fired on the logic pass, selection 0 -> 1. The
  render-only passes clear it again.
- Ticks 1416-1417: not fired. The repeat delay ran out on the 11th
  update (pass 2710, a render-only pass in tick 1417): about 2 ticks
  instead of ~11.
- From then on the bit is set on every update, so every logic pass sees
  it and the selection moves **one item every tick** (1418: 1 -> 2, 1419:
  2 -> 3, ... wrapping 4 -> 0), 30 items a second.

So the repeat objects count updates, the updates come once a frame, and
both the delay and the period run ~4.8 times too fast at 144 fps (the
period is shorter than the ~5 updates a tick, so it reads as "every
tick"). The menu itself is fine. This also corrects the `--log_input_reads`
result above: `8210C370` calls the button-test wrapper `8258CF10`
unconditionally, so it must have run on render-only passes then too; that
logging probably missed them (not checked).

The fix (step 2, not started): run `8210B018` (or `8210C370`) on logic
passes only, so the mappers count ticks as at 30 fps; the fired bits a
logic pass leaves stay in place for the render-only passes after it.
Check nothing else needs the mappers once a frame (e.g. the car's
steering in a race reads them on logic passes only, if they're read
there at all).

### Revised plan (2026-10-18)

The cause is known (above): the action mappers' update `8210B018` (each
mapper's `8210C370`) runs on every pass, so their repeat objects count
frames. The menu, Input::Update and the listeners' counters are right.
So the fix is to update the mappers once a tick, not to change who reads
them; Input::Update natively is no longer part of the fix and moves to
later work (step 4).

The mapper's read slots (vtable `0x82007F78`; r4 = action id 151-180,
index = id - 151):

| Slot | Function | Reads |
|---|---|---|
| 5 | `8210BA68` | float at +1132 + 4i (an analog value) |
| 8 | `8210BE70` | bit i of +448, held |
| 9 | `8210BED8` | bit i of +452, held before |
| 10 | `8210BF40` | bit i of +456, pressed (new this update) |
| 11 | `8210BFA8` | bit i of +460, released |
| 12 | `8210C010` | bit i of +464, fired (pressed or repeat) |
| 14 | `8210BA28` | u16 at +1072 + 2i (an analog value) |

Slots 2, 3, 4, 6, 7 set up actions (boot, `8209F228`); 13 (`8210B6E8`)
isn't read yet. Callers use the slots through the vtable (the menu
through `8220C020`, "confirm" `8220BD50` through slot 10), so grep can't
list them; that's step 1.

### Step 1: who reads the mappers, by pass type (logging)

Today a render-only pass rebuilds the words and clears the pressed and
fired bits a logic pass set (pass 2700 in the log above). Once the
update is skipped there, those bits stay set until the next logic pass:
harmless for code reading them on logic passes (as at 30 fps, where
every pass is one), but code reading them on render-only passes would
see one press ~5 times. Find out before changing anything:

- Add to `--log_pause_menu` (`src/debug/pause_menu_log.cpp`): override
  the read slots above (`8210BA68`, `8210BE70`, `8210BED8`, `8210BF40`,
  `8210BFA8`, `8210C010`, `8210BA28`; `grep -rn` each in `src/` first)
  and count calls once a second by caller (`lr`), slot, action and pass
  type, separately while paused (`GameMessages_Paused()`) and not. Log
  calls that return non-zero on render-only passes on their own line.
- Also log who calls `8210B018` (`lr`; it's called from `82107910` and
  sits in the table at `0x8207A158`, probably a component's update run
  through `82092968`) and on which passes.
- One run the user plays: drive a race a few seconds (steer, brake, use
  nitro, change camera, horn), pause, hold the stick down ~5 s, press
  confirm on an item that opens a sub-screen and back, unpause, finish
  or quit the race through its end screen if possible.
- Expected: every reader on logic passes only. A reader on render-only
  passes needs its own fix before step 2 (or it gets the old per-frame
  words kept for it).

### Step 2: update the mappers once a tick (`--input_mappers_by_tick`)

- Override `sub_8210B018` (not overridden yet) in a new
  `src/hooks/input.cpp`: return without calling `__imp__sub_8210B018` on
  render-only passes (`frame_pacing::GameMessages_IsRenderOnlyPass()`),
  as the `sub_820990F0` override does for Input::Update. Behind
  `REXCVAR_DEFINE_BOOL(input_mappers_by_tick, ...)`, off by default
  until checked, then on in `run.sh` and `run.bat` (and `run-debug.*`).
- Render-only passes only happen in races (menus run every pass as a
  logic pass), so no in-race check is needed; and the update doesn't
  record UI, so skipping it doesn't make anything flicker.
- `--log_pause_menu` should then show the mapper update on logic passes
  only, the first repeat ~11 ticks after the press and a move every few
  ticks after that, the same as `--max_fps=30` (compare the two in the
  log; take the 30 fps run as the reference rather than guessing the
  game's numbers).
- Run: the user does step 1's run with `--input_mappers_by_tick=true
  --pause_workaround=false` and says whether the menu feels like 30 fps
  and the car drives the same.

### Step 3: remove the workaround

Once step 2 is checked: delete the `ctx.r31 + 588` check from the
`sub_8215B4F8` wrapper in `src/frame_pacing/game_messages.cpp` and the
`--pause_workaround` flag (and from `run.sh`/`run.bat` if they pass it).
The pause screen then stays at the full frame rate, interpolated, with
no slow return to it on resume.

### Step 4: later, toward moving off ReXGlue (not needed for the fix)

- Native Input::Update (`--native_input`, in `src/hooks/input.cpp`):
  follow `src/hooks/lights.cpp` (the override calling the translated code
  when the flag is off), off by default until checked.
  - `sub_820990F0` keeps its render-only skip; its override moves into
    `input.cpp` (only one override per function can exist) or calls
    into it.
  - Physical and logical pads natively, as described above. Call the
    kernel imports as the thunks do (`__imp__XamInputGetState`,
    `__imp__XamInputGetCapabilities`, `__imp__XamInputSetState`, with
    arguments in ctx registers and the structs in guest memory, e.g. on a
    `guest::StackFrame`), so `--mnk_mode` still works through the SDK.
  - Logical pads sum their bound pads' values directly (step 0 showed
    slot 1 returns `device + 8` for physical pads and `device + 36` for
    logical ones); any other device goes through its guest vtable call
    (`REX_CALL_INDIRECT_FUNC`).
  - Processors and listeners stay guest calls, with r3/r4 set as above.
  - `--verify_native_input`: run both (translated first into a copy,
    then native) each tick and log any byte that differs in the 4
    physical and 5 logical pads. Running XInputGetState twice a tick is
    harmless; rumble timers aren't, so restore the motors (+168..+191)
    between the two. Check with an attract race and a race the user
    drives, then turn the flag on.
- The action mappers natively (`8210C370`, `8210C0E0`, `8210BBA0` and
  the repeat objects): layout as in "The pause menu's code"; read the
  repeat objects' classes and the parameter table at `0x82600B88` first.
- Possibly read SDL directly instead of `XamInputGetState`. This needs our
  own mouse and keyboard handling to replace `--mnk_mode`.

### When done

Update the `Input::Update` row and "Menus run at 30 fps" in
`docs/functions.md` (the pause menu then runs at the full frame rate with
the right repeat; the front end's menus still run at 30 fps), this
document's status, and CLAUDE.md's line for this document.
