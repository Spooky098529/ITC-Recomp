# Pause menu: input repeating too fast

In a race, the pause menu's input repeats too fast with frames uncapped
or above 30 fps: holding a direction moves through the menu one item every
tick once the repeat starts, and the repeat starts too soon. This is the plan to fix it and to translate the game's input
handling to native code. **Fixed** (2026-10-18, steps 1-3 of "Revised
plan"): the action mappers' update `8210B018` ran once a frame, so their
repeats counted frames; it now runs once a tick (`--input_mappers_by_tick`,
on by default, `src/hooks/input.cpp`), and the workaround that held the
pause screen at 30 fps is gone. Step 4 (native input) is later work.

Addresses below are guest addresses. `0x82000A30` and friends are floats in
the game's constant data, read with `guest::LoadU32` and bit-cast (their
values, logged in step 0, are in "Step 0 results"). Offsets are
in bytes. Every function named can be read in `generated/` (find it with
`grep -n "DEFINE_REX_FUNC(sub_XXXXXXXX)" itc_recomp/generated/default/*.cpp`).

## What's known

### The workaround on `pause-menu` (commit `78456a5`, removed in step 3)

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

### The input processors (`sub_82592188`), 416 bytes

Read 2026-10-19 (native since: `src/input/input_processor.cpp`). The
processors and the listeners' first objects share this class (vtable
`0x8206C0C4`); slot 1 is the update, r3 = object, r4 = its logical pad.
It calls nothing but the pad's slot 1 (its 40 values, `pad + 36`), and
leaves that address in r3.

| Offset | What |
|---|---|
| +16 | dead zones, 4 floats (read): x, y, trigger, unused; both sticks use them |
| +32 | the values 0-7 copied (LX, LY, LT, 0, RX, RY, RT, 0) |
| +64 | the same with the dead zone taken out |
| +96, +112 | each stick's (x, y, 0, 0) normalized, or 0 when centered |
| +128, +132 | each stick's length |
| +136 | the values 8-39 copied (32 floats) |
| +264 | pressed: bits newly set this tick |
| +268 | released: bits newly clear this tick |
| +272 | held: bit `i` set while value `8 + i` is above 0.5 (`0x82001BE4`) |
| +276, +280 | repeat delay and period (read) |
| +284 | 32 hold counters (ticks) |

In order:

1. Copy the values 0-7 to +32.
2. For each stick (values 0-3, then 4-7), for each of its four values
   `x` with dead zone `d = +16[i]`: if `x > 0`, `(x - d) / (1 - d)`, then
   0 if below 0; otherwise `(x + d) / (1 - d)`, then 0 if above 0 (NaN
   is kept). To +64.
3. Each stick's length `sqrt(x² + y²)` (an `fmuls` and two `fmadds`) to
   +128; its direction: (x, y, 0, 0) times the reciprocal square root of
   its dot product with one Newton step (AltiVec `vmsum3fp128`,
   `vrsqrtefp`, `vnmsubfp`, `vmaddfp`), or 0 where the dot product is 0,
   to +96 (stored with `stvx`: the object is 16-byte aligned).
4. For each value `8 + i`, `i` = 0..31: copy it to +136; if above 0.5,
   set bit `i` in held, add 1 to counter `i`, and if the counter is then
   above delay + period (signed), set it back to delay; else counter `i`
   = 0. This is the hold-to-repeat cycle: the counters seen at +180 and
   +190 in step 0 are counters 25 and 29 (stick directions).
5. pressed = held & (held ^ old held); released = (held ^ old held) &
   old held.

### Who reads the input manager (2026-10-19)

Asked for docs/rexglue-removal.md (B1): if the action mappers were
native, could the input manager (3,984 bytes at `0x40059F50`) and the
four listener objects (inside the mappers at +0x9A0) stop being written
to guest memory? Found with a page-protection read watch
(`--watch_input_reads`, `src/debug/input_read_watch.cpp` and
`tools/resolve_input_reads.py`, written by a subagent; not committed:
kept in a git stash, "input read watch (--watch_input_reads) from the
subagent", `git stash list` to find it; Linux only): the host pages behind the
ranges are made `PROT_NONE`, each fault logs the host instruction (local
builds keep symbols, so `addr2line` names the `sub_XXXXXXXX`) and the
offset, and the page is opened for one single-stepped instruction. Our
Input::Update runs with the pages open.

Three unattended runs (boot, the front end, attract races), then a
session the user played (2026-10-20, `itc_183.log`: the title, front-end
menus, a race with the rival intro, the pause menu and its sub-menus),
resolved with `tools/resolve_input_reads.py`:

| Reader | Reads | When |
|---|---|---|
| `sub_8210C370` (the mappers' update) | logical pads 0-3's values 0-7 (+44..+72 + 196j) | front end, race, paused |
| `sub_8258CF10` → `sub_825924E0` (button test, for the mappers) | listener objects' held word (+272) | front end, race, paused |
| `sub_8258CEB0` → `sub_82592500` (button test), called twice by `sub_82093DC0` | processor 0's pressed word (manager +2104 = +1840 + 264) | every phase |
| `sub_8258CEE0` → `sub_82592520`, called by `sub_82093DC0` | processor 0's held word (+2112), and for a held bit its counter (+284 + 4i) against the delay (+276): true on the first tick held or once repeating | every phase, render-only passes too |
| `sub_8209F340` → `sub_82591C08`, called by `sub_8215FCA8` and `sub_822231B0` | a physical pad's flags (+4; pad 0 at manager +992): connected | race |
| `sub_8209F3B8`, called by `sub_8215FCA8` | a physical pad's capabilities subtype (+193, bit 2: a wheel, XInput subtype 2, probably) | race |
| `sub_82093DC0`, `sub_820902C8`, `sub_8209F340`, `sub_8209F3B8` | the manager's vtable (+0) only, to call its methods | every pass |

`sub_8209F340` and `sub_8209F3B8` find the player's physical pad through
the manager (the global at `0x826BAB78`) and two of its methods (vtable
slots 9 and 5, with the player index at `[r3] + 2888`). The subtype test
is why our fixed capabilities report a gamepad (subtype 1), as the SDK
did.

- The mappers' +2880 points to logical pad `j`'s values (manager +44,
  +240, +436, +632 for mappers 0-3): they read the sticks from the
  logical pads, not from the processors.
- Readers outside the mappers: the app's update tests processor 0's
  pressed, held and repeat state directly (`sub_8258CEB0`,
  `sub_8258CEE0`), and in a race `sub_8215FCA8` and `sub_822231B0` ask
  whether the player's pad is connected and whether it's a wheel. The
  pause menu reads nothing here directly: it goes through the mappers.

### The action mappers' update (read 2026-10-20)

Read for making the mappers native (docs/rexglue-removal.md, B1). One
mapper per player (vtable `0x82007F78`); `sub_8210B018` updates each
with `sub_8210C370` (r3 = mapper). Actions are indexed 0-29 (id - 151).
Offsets are from the mapper.

| Offset | What |
|---|---|
| +372 | config: +0 a pointer to 30 words (each action's kind; 1 = digital), +4 to 30 words (a linked action, 30 = none), +8 and +12 two more such pointers for the analog stage (+8 kinds, +12 linked actions) |
| +388 | 30 u16: this tick's raw value per action |
| +448 | words: held, held before, pressed, released, fired, then +468 a "pending" word ORed into the next update's held-before and pressed |
| +472 | 30 repeat objects (vtable slot 1: r4 = held bit, returns "fire") |
| +1072 | 30 u16: the analog stage's values |
| +1132 | 30 floats: the analog stage's values through each action's curve |
| +1252 | 30 curve objects (vtable slot 1: f1 in, f1 out) |
| +1852 | 30 entries of 20 bytes: {float on, float off, u32 suppress ticks, u32 counter, u8 state} |
| +2464 | the listener object (416 bytes, our InputProcessor) |
| +2880 | pointer to logical pad `j`'s 40 values |

`sub_8210C370`, in order:

1. Clear the 30 raw u16s (+388).
2. Sticks and triggers from the logical pad's values `v`, each as
   `fctidz(|v * C|)` kept to 16 bits (`C` = float at `0x82007F58`):
   `v[1]` (LY) > 0 to index 16, < 0 to 17; `v[0]` (LX) < 0 to 18, > 0 to
   19; `v[5]` (RY) > 0 to 20, < 0 to 21; `v[4]` (RX) < 0 to 22, > 0 to
   23; `v[2]` (LT) to 12 and `v[6]` (RT) to 13, unconditionally.
3. Buttons: for 14 (mask, id) pairs, if the listener's held word has
   every bit of the mask (`sub_8258CF10` → `sub_825924E0`), raw[index of
   id] = 32767, the index from `sub_8210B140` (a jump table indexed by
   id - 151 through the bytes at `0x82007F20`, returning 30 for ids out
   of range; probably the identity, to check). Pairs: 0x1 153, 0x2 154,
   0x4 155, 0x8 156 (d-pad), 0x10 159 (A), 0x20 158 (B), 0x40 160 (X),
   0x80 157 (Y), 0x1000 161 (LB), 0x10000 162 (RB), 0x4000 165 (left
   stick), 0x40000 166 (right stick), 0x100000 152 (Start), 0x200000 151
   (Back). The masks are the processor's held bits (value slot numbers).
4. Six axes from pairs of the raw u16s, each `fctidz((0.5 - a*K + b*K)
   * C)` kept to 16 bits (`K` = float at `0x82007F5C`; `0.5 - a*K` is
   `fnmsubs`, `+ b*K` `fmadds`, both as `float(fma(double...))` in the
   translation): d-pad (4 left, 5 right) to 24, (2 up, 3 down) to 25;
   stick (18, 19) to 26, (16, 17) to 27; right stick (22, 23) to 28,
   (20, 21) to 29.
5. `sub_8210C0E0(r3 = mapper + 448, r4 = mapper + 372, r5 = mapper + 388)`.

`sub_8210C0E0` (`M` = mapper + 448):

1. The analog stage, `sub_8210B7D0(mapper + 1072, config + 8, raw)`: for
   each action `i`, with kind `k` = (+8)[i] and link `l` = (+12)[i]: if
   `l < i` and raw[l] ≠ 0, `k` = 0. `k` = 0: u16 and float 0. `k` = 1:
   u16 = raw[i], float = curve[i](raw[i] / 32767) (`sub_8210B4A8`, the
   constant at `0x82002BD4`). `k` = 2: u16 = raw[i], float =
   curve[i]((raw[i] - A) * B) (`sub_8210B500`, `A` at `0x82007F64`, `B`
   at `0x82007F60`). `k` ≥ 3: left as it was.
2. For each action `i`, `sub_8210B718(M, i, float[i], config)`: `x` =
   |float[i]| (times -1 when negative); with the entry at +1852 + 20i:
   if on == off, state = `x > off`; else state = 1 when `x >= on`, 0
   when `x <= off`, else kept. If the counter is nonzero it counts down
   and state = 0. When state is 1 and the action's link (config +4) isn't
   30, the linked action's counter = its suppress ticks. Bit `i` of a
   local word = state.
3. `sub_8210BBA0(M, config, &word)`: for each `i`, bit `i` of a new word
   = bit `i` of the input, if kind (config +0)[i] is 1 and its link `l`
   is either ≥ `i` or has bit `l` clear in the new word; else 0. Then
   held before = held | pending; held = new; changed = held before ^
   held; pressed = (held & changed) | pending; released = changed &
   ~held; fired = pressed; pending = 0. Then for each `i`, repeat[i]
   (vtable slot 1, r4 = bit `i` of held); if it returns true, fired |=
   bit `i`.

Both local words start as whatever is on the guest stack: only bits 0-29
are written, so bits 30 and 31 of held (and so of held before, pressed
and released) are stack leftovers. A native version would have them 0;
whether anything reads bit 30 (`sub_8210B140`'s "none") isn't known.

Not known yet (a run will tell): the constants at `0x82007F58`,
`0x82007F5C`, `0x82007F60` and `0x82007F64`; the bytes at `0x82007F20`;
the config's tables; and the classes of the repeat and curve objects
(their vtables, slot 1) in each mapper.

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

**Working hypothesis (not proven; superseded by "The pause menu's code"
below: it was the action mappers' update):** code that runs on every pass while
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

Result (a race the user drove, holding down on the analog stick in the
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

Done (2026-10-18): `--log_pause_menu` now overrides the seven read slots
and `8210B018`, and logs `pause menu reads: tick T running|paused
logic|render-only slot S (function) action A from LR: calls C non-zero
Z` once a second per key, plus `pause menu read on render-only pass:`
for each non-zero read on a render-only pass (own budget, 200 lines per
2 s). An attract race (`itc_025.log`, 144 fps) showed `8210B018` called
from `82107A24` on every pass (~30 logic, ~120 render-only a second) and
only "pressed" (slot 10) reads, of actions 152 and 159, all on logic
passes (from `821690C0`-`821691F4` and `82222230`-`822222CC`, probably
the attract mode watching for a press). A race the user drives is next.

**Result** (a race the user drove at 144 fps, `itc_027.log`; `itc_026.log`
was left in attract mode and not read): **every read is on a logic
pass**, paused and running. The only call on render-only passes is the
update `8210B018` itself (from `82107A24`), and no "read on render-only
pass" line was logged. Readers seen:

- Driving: `82102400` (slot 14, the u16 analog values of actions 155,
  156, 160, 163, 164, 169, 170, 173, 174; 164, 169 and 170 non-zero most
  often) and `82102468` (slot 10 pressed: 152, 157, 158, 161, 162; slot
  8 held: 159, 165, 166), each once a tick.
- The pause menu: `8220C020`'s tests (`8220C0B4`-`8220C1B8`, slot 12,
  the eight directions), and `8220BDAC`, `8220BE3C`, `8220BECC` (slot 10,
  actions 159, 158, 152: probably confirm, back, and a third). Holding
  the stick down for ~5 s paused gave 114 non-zero "fired" reads of 168,
  the too-fast repeat.
- Another menu, `821865E4`-`82186718` (slot 12, the directions) and
  `82186540` (slot 10: 158, 159, 163), running, not paused (probably the
  race's end screen), and the `82222230`-`822222CC` reads seen in attract
  mode.

So skipping the update on render-only passes (step 2) leaves nobody
reading the words it would have rebuilt there; nothing needs fixing
first.

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

Done (2026-10-18): `src/hooks/input.cpp` overrides `sub_8210B018` (the
override moved there from `pause_menu_log.cpp`, which now counts the
update's runs through `debug::PauseMenuLog_NoteMapperUpdate`, so skipped
calls aren't counted). An attract race with the flag on (`itc_028.log`)
shows the update on logic passes only, 30 a second, and none on
render-only passes. Next: the user's run with the flag.

**Checked by the user** (2026-10-18, 144 fps, `--pause_workaround=false`,
runs with the flag on, off, and on again): with the flag on the pause
menu's repeat is right, and driving feels the same as with it off. The
`--max_fps=30` reference run in the log wasn't done; the user's check
stands in for it.

### Step 3: remove the workaround

Once step 2 is checked: delete the `ctx.r31 + 588` check from the
`sub_8215B4F8` wrapper in `src/frame_pacing/game_messages.cpp` and the
`--pause_workaround` flag (and from `run.sh`/`run.bat` if they pass it).
The pause screen then stays at the full frame rate, interpolated, with
no slow return to it on resume.

Done (2026-10-18): the check and the flag are removed (no launch script
passed it); `InputLog_PauseSigns` still logs `r31 + 588` for
`--log_input`. `--input_mappers_by_tick` is on by default, so the launch
scripts don't need to pass it.

### Step 4: later, toward moving off ReXGlue (not needed for the fix)

Done differently (2026-10-19, see B1 in `docs/rexglue-removal.md`): no
`--native_input` flag or verify mode; Input::Update, the physical and
logical pads are native, reading SDL through our own XInput instead of
the SDK's (no `--mnk_mode` any more). The plan as written:

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

What's known and what isn't yet, for whoever picks this up:

- Known well enough to write natively: Input::Update's order, the
  physical and logical pads (layouts, constants, rounding, rumble), the
  manager's objects as built in races ("Step 0 results"), and the
  mapper's words and read slots ("The pause menu's code", "Revised
  plan"'s table).
- The processors' and listeners' update (vtable `0x8206C0C4` slot 1
  `0x82592188`): read since (2026-10-19), see "The input processors".
- Not read yet (read these first for the native mappers):
  - `8210C370`, `8210C0E0`, `8210BBA0` in full: how each action's held
    bit and its analog values (+1072 u16s, +1132 floats) come from the
    stick floats (`[+2880]`) and the listener at +2464.
  - `8210B140` (action id to index; ids 151-180 map to 0-29, 30 means
    none, but whether it's a plain subtraction isn't checked).
  - The repeat objects (pointers at +472; `8210B568` looks like one
    class) and their parameters from the table at `0x82600B88`, set by
    `8209F228` at boot, and slot 13 `8210B6E8`.
  - Which input each action id is: only the menu's directions (153-156
    d-pad, 167-170 stick) are known. The car reads 155, 156, 160, 163,
    164, 169, 170, 173, 174 (analog, `82102400`), 152, 157, 158, 161,
    162 (pressed) and 159, 165, 166 (held, `82102468`); the pause menu's
    "pressed" reads are 152, 158, 159 (`8220BDAC`, `8220BE3C`,
    `8220BECC`). A run with `--log_pause_menu` pressing one button at a
    time would name them.
- Every reader seen runs on logic passes ("Step 1" result), so a native
  update needs to run once a tick only, as `--input_mappers_by_tick`
  does now; a `--verify_native_mappers` comparing the +448..+467 words,
  +1072..+1131 and +1132..+1251 against the translated update each tick
  would check it, as `--verify_native_input` does for the pads.
- Reading the code: `grep -n "DEFINE_REX_FUNC(sub_XXXXXXXX)"
  itc_recomp/generated/default/*.cpp` finds a function; its PowerPC is
  in the `//` comments (`awk -v s=<line> 'NR>s && /^DEFINE_REX_FUNC/
  {exit} NR>=s' <file> | grep '^\s*// '`), and `grep -n "// bl
  0x<lowercase address>$"` lists direct callers. Vtables and tables like
  `0x82600B88`: `--dump_image=<path>` (see `docs/gpu-functions.md`;
  big-endian, offset = address - `0x82000000`).

### When done

Update the `Input::Update` row and "Menus run at 30 fps" in
`docs/functions.md` (the pause menu then runs at the full frame rate with
the right repeat; the front end's menus still run at 30 fps), this
document's status, and CLAUDE.md's line for this document.

Done (2026-10-18): `docs/functions.md` has a row for `sub_8210B018`
(the `Input::Update` row was still right) and the known issue says the
pause menu runs at the race's rate.

## The sub-menus' arrow (`pause-menu-arrow`, 2026-10-18, fixed)

In every pause sub-menu, an arrow points at the selected item. At 144 fps
the user sees it slide fast in the direction pressed and end up out of
position; after the second build below "it moves at 30 fps, but then keeps
moving, like something keeps reading the input and playing its
animation". Not fixed yet.

**What draws it.** The sub-screens (`8220DB98`, `8220E988`, `8220EBD8`,
`8220C578`, `8220C5D0`, picked by `8220F638`) don't queue text like the
main list: they look up nodes of a UI movie (`8211A578(root, type,
index)`, through `8259D658`; the root is `[[0x826D84D0]]`) and set flags
at +80 and +108 on them, or call `8211C7E8(movie, node, id, value)`
(from `8220E5FC`-`8220E6E0` when a sub-screen opens, and from
`8220D040`). `--log_pause_movie` (`src/debug/pause_movie_log.cpp`)
counts every UI movie update (`sub_8211D2F8`, reached through
`8211E1B8` from 634 call sites) by movie, caller and pass type, and
while paused compares each updated movie (its 0xA000 bytes, what the
UI movie replay keeps, and objects up to two pointers away, 0x200 bytes
each) before and after each update ("in update") and since the last
update ("between").

Result (`itc_039.log`, a race the user drove, paused, used a sub-menu):

- While paused two movies are updated (r4 = 0, advance and draw) on
  every pass: `400D25A0` from `822128C8` and the sub-menus' movie
  `417E7260` (the r3 of the `8211C7E8` calls), mostly from `8220FC78`
  and also from `8220EF84`, `8220F054`, `8220EE08`, `8220EEE4`,
  `8220DF24`, `8220C85C`.
- **Not the pause menu's own draw.** `8220B9E8` (from the pause
  screen's Draw `8216CF08`; movie 18 of the table at `0x826C5CCC`,
  `41620060`) is updated on every pass from the race start until ~30 s
  in, and not at all while paused. A first fix that made its
  render-only updates draw-only (`--per_tick_pause_menu_movie`, commit
  `816356d`) changed nothing and was removed.
- In `417E7260` the only words its updates change are animation clocks
  at +0x9154, +0x96E0 and +0x9708 (floats, +2 an update; reset to 0 when
  an animation starts, running to 40 or 120). On logic passes they
  advance once a tick: the UI movie replay works.
- Except right after an animation starts: the sub-menu starts it after
  the logic pass's movie update (pass 1780: new animation slots at
  +0x96C0..+0x970C, clocks reset), so the replay finds the movie
  "changed since the logic pass" and ran the update for real on each
  render-only pass of that tick (0 -> 8 by the next tick instead of 2).
  `--per_tick_changed_ui_movies` (on by default, commit `b9ef4ec`) now
  runs those updates from the current state and puts it back. With it
  the user saw **no change** (`itc_041.log`: the fallback happens ~4
  times every 2 s, those extra steps aren't what's seen). Kept for now;
  it's correct, but whether to keep it is open.
- Nothing else in the watched objects changes on render-only passes.
  So either the arrow's state is outside them (deeper objects, or not in
  a UI movie at all), or what's seen is the game's own behavior at its
  repeat rate.
- **The sub-menu list repeats every tick.** In `itc_037.log`, holding a
  direction in `8220DB98`'s sub-menu moved its list (`8220C1E0`, then
  the screen switch from `8220DBE0`) on every tick for ~2 s (64 moves),
  starting 11 ticks after the press; each move restarts the arrow's
  slide. The main pause list repeats every 4 ticks. Not compared with
  30 fps yet: that's the next run (`./run.sh --no-interpolation` flags,
  i.e. `--interpolate_draw_state=false --max_fps=30`, with
  `--log_pause_movie=true`), to see whether the arrow does the same
  there and the list repeats as often.

**At 30 fps** (`itc_042.log`, `--interpolate_draw_state=false
--max_fps=30`) the user saw the arrow right, and the held list moved
every tick there too (ticks 2080-2133): the repeat is the game's own, so
the difference is how the movie is updated between ticks. Per caller
while paused, the sub-menus' movie `417E7260` is updated once a pass from
`8220FC78` (in `8220FB10`), and on top of that on some ticks by the
sub-screens (`8220DF24` in `8220DD98`, `8220EE08`, `8220EF84`,
`8220F054`, `8220C62C`, `8220C794`, ...), at 144 fps on render-only
passes too, except `8220DF24`.

`8220DD98` is the selection change (it runs when its state +0 is 0): it
starts the movie's animations (`8211DE38`/`8211D978` with r4 = 8, r5 =
item + 4; `8211DDD8`; `8211DF20` 20 and 19; and `8211DAC8` with 20
after a move down, 19 up: probably the arrow's slide), then updates the
movie once more, on logic passes only. So a move tick's logic pass
updates the movie twice (`8220DF24`, then `8220FC78`) and its render-only
passes once, and the replay, matching calls by order in the pass,
compares the render-only `8220FC78` call with the logic pass's
`8220DD98` one. `--log_pause_movie` now also logs those animation calls
and, for 12 passes after each, every movie update with its caller, its
occurrence in the pass, the replay's outcome and the three clocks.

Result (`itc_043.log`, 144 fps): the movie side looks right. All 30
arrow slides (`8211DAC8`, from `8220DF14`) start on logic passes, followed
by the movie update from `8220DF24`; the render-only passes after each
replay `8220FC78`'s update from it ("replayed", clocks unchanged until the
next tick), as at 30 fps. Only a sub-screen opening starts animations on
render-only passes (`8220EE70`, `8220EEB0`, `8220C6C0`, `8220C8C8`, once
each). So the movie isn't what moves the arrow wrongly. The 30 fps
reference also had interpolation off, which blends HUD draws between
ticks: next, 144 fps with `--interpolate_draw_state=false`.

`825A08E0`, which each movie update calls per playing animation, is
keyframe interpolation (it finds the keys around the animation's time and
applies t = (time - k0) / (k1 - k0), clamped): it sets values from the
time, so replays can't step it. The game never measures time (see
"Fixed time step" in `docs/functions.md`), so the arrow isn't scaled by
frame time; something steps once a pass. With interpolation off at 144
fps the user still saw the arrow wrong, so it isn't the blending either.

The movie's nodes are at 0x03xxxxxx (`035D1B80` etc.), which the first
logs didn't follow; `itc_046.log` watches them (three pointer levels,
~4,900 objects). Inside render-only updates only counters change
(0xE8xx: draw stamps, in 0x0224xxxx and 0x0532xxxx) besides the scene
object's bookkeeping; nothing that looks like a position. Next, a test:
`--paused_render_only_skip=8258E5E0,8258ED78` skips Scene::Update's two
screen calls (which run on every pass and update the front end's
screens) on render-only passes while paused.

Result: skipping both crashed the game on pausing ("Call to invalid or
unregistered function at guest address 0x00000000", `itc_047.log`).
Reading them, neither is the arrow: slot 6 `8258E5E0` flips a 0/1
buffer index, and slot 2 `8258ED78` calls the screen's vtable slot 7, then
`8258EB38`, which clears its lists. They are the render lists' double
buffering. Not worth trying one at a time; `--paused_render_only_skip`
stays as a test flag for the other functions wrapped there.

**The sub-menus' draw** (read 2026-10-18). The pause screen's Draw
`8216CF08` calls `82211D08` (`r3 = [r3 + 1012]`, then a jump to
`8220FB10`). `8220FB10`, on every pass:

- if the sub-screen state `[0x826D84D8]`'s +12 isn't -1, it calls one of
  21 transition functions (a table on the stack, `0x821Fxxxx` and
  `0x8220xxxx`) with r3 = state + 24, then `8220BA28(state, result)`;
  `8220BA68(state, a, b, c)`, the "screen switch" in the logs, sets +4,
  +12 and +16 to a, b and c and +0 to -1 (or +0 to a and +12, +16 to -1
  when b is -1);
- otherwise it updates the movie `[r3]` (r4 = 0, from `8220FC78`);
- then calls `82218F30` when the state's +0 is 4.

Transition 8 is `8220DD98`, the selection change: the sub-menu
`8220DB98` switches to it (`8220BA68(state, 1, 8, -1)`, from `8220DBE0`)
on every move. Its first call starts the animations and returns after
updating the movie (`8220DF24`). So each pass either runs a transition
or updates the movie from `8220FC78`, at any frame rate, and the
movie's timeline per tick comes out the same at 30 and 144 fps, as the
logs show.

So far, ruled out as the cause: the input repeat (the game's own: the
held list moves every tick at 30 fps too), interpolation (wrong with it
off at 144 fps), the movie's state and animations (replayed, clocks per
tick, keyframes set from the time), Scene::Update's screen calls (render
list bookkeeping). Not ruled out: the transition functions other than 8
(the sub-screens' opening ones run on render-only passes:
`8220EE08`, `8220EF84`, `8220F054`, ...), `82218F30`, the HUD's movie
under the pause (`822128C8`), and draws that aren't UI movies at all.
Next: a frame dump while paused (`--native_renderer_dump`) to see the
arrow and find its draw.

**Found (2026-10-18): the tracks add to their nodes.** A frame dump while
paused (`--native_renderer_dump_every=1`) showed the arrow: a line from
the screen's left edge ending in a point at the selected item. With the
first item ("Road Map") selected it's there; after a move to another item
it's gone from the screen. The user's idea, an add scaled by the frame
rate, was right. `825A08E0`, run by each movie update for every playing
animation, applies each track with `825A0548`, which ends with: when the
track descriptor's byte +8 is 0, `[node + offset] += value` (floats and
ints alike), otherwise `= value`. The nodes are outside the bytes the UI
movie replay restores, so each render-only replay added the track's
value again: ~5 adds a tick at 144 fps, the arrow sliding ~5x as far and
off its item. The movie's clocks were right all along; the node watch
probably didn't reach the arrow's node.

The fix (`--per_tick_ui_movie_tracks`, on by default, `per_frame_logic.cpp`):
inside a UI movie update (`sub_8211D2F8`) on a render-only pass,
`825A0548` doesn't run. The nodes keep what the logic pass applied for
the tick, so the render-only frame draws the same as the logic pass, and
each add happens once a tick. Applies outside a movie update (the
animation starts `8211DF20`, `8211DE38`, `8211DFF8`) run as before. This
covers every UI movie, the race HUD's too.

Result: the arrow's position was right, but the pause menu flickered, and
on first opening it the sub-menus showed overlapped for a moment.
Skipping every track was too broad: tracks that set a value are needed
on every pass. Now only adding tracks are skipped. The track's property
id is its word +0 (`[r4]`); `8259FE68` returns that property's 12-byte
entry in a static table at `0x82970C28` (64 entries, filled on first use,
byte `0x82970C20` set then): +0 the node offset, +4 the type, byte +8 set
(non-zero) or add (0), byte +9 indirect. The override reads byte +8 there.

The first build of that read the property id as `[[r4]]` instead of
`[r4]` and crashed on the race loading screen ("read of guest
0x00000018", `itc_050.log`; render-only passes apply tracks during
loading). Fixed in `75271ce`; an attract race then loaded and ran.

**Result of skipping only adding tracks** (user, 2026-10-18, 144 fps): the
arrow's position is right, but there is **ghosting when the arrow moves**,
and **the race start countdown flickered** (also a UI movie, in the HUD).
So skipping isn't exact: some adding tracks start from a value that's
reset or set elsewhere during the update (the user's guess: alpha), so a
render-only frame without them draws something different from the logic
pass. The earlier build that skipped all tracks made the pause menu
flicker and showed the sub-menus overlapped for a moment on first
opening (not rechecked since).

**Planned, then superseded (see "Finding the arrow's track"): replay the tracks' writes exactly**, the
way `ReplayFromLogicPass` replays the movie object, instead of skipping:

- On a logic pass, in the `sub_825A0548` override inside a UI movie
  update, record each write's target address with its value before and
  after the call, per movie and per call occurrence (the replay's key).
- On a render-only replay of that call: put every recorded target back
  to its "before" value, let all tracks apply normally (set and add, so
  the frame draws exactly what the logic pass drew), then put the
  "after" values back.
- The fallbacks (`changed since the logic pass`, `no logic pass state`)
  need a choice: record the targets' values before the call and put them
  back after (as `--per_tick_changed_ui_movies` does for the movie).
- The target of a track (from `825A0548`): `node = r3`, `desc =
  0x82970C28 + 12 * [r4]`; `target = node + [desc + 0]`, plus, when
  byte `desc + 9` (indirect) is non-zero, `8259D600([r4 + 12], [node +
  112])`, which is `16 * (a + 2 * (b + 4))` (`addi r11,r4,4; slwi 1;
  add r3; slwi 4`). The value written is 4 bytes (float or int, by the
  type at `desc + 4`). Check the formula against the generated code
  before relying on it, or record writes generically by comparing the
  node's bytes around the target before and after.
- Check with the user: the arrow's position and no ghosting, the pause
  menu without flicker or overlap, and the race start countdown.

**Finding the arrow's track** (2026-10-18). Which adding track moves the
arrow had never been logged; the skip above covers every adding track in
every UI movie. `--log_pause_movie` now logs, while paused and for the 12
passes after each of the sub-menus' animation calls (the arrow's slide
`8211DAC8` among them):

- `pause movie track:` every adding track applied, and every setting track
  on a word an adding track has moved: movie, `adds`/`sets`, property,
  type, node, target word (and its offset in the node), value before and
  after with the difference, `skipped` on render-only passes. The target is
  worked out from the descriptor as above, checked against `generated/`:
  `825A0548`'s only store outside the stack is the one word at `r25`, none
  for types 3 and up, and the helpers it calls write only to the stack.
- `pause movie targets:` the words adding tracks have moved, at the start
  and end of each UI movie update.
- `pause movie target written:` which of the update's other calls
  (`82119B80`, `8259FA00`, `8211CD90`, `8259DAC0`, `8211B4D0`, `8211B080`,
  wrapped in `pause_movie_log.cpp`) changed one of those words.

Results (`itc_054`-`itc_056`, 144 fps, the user moving the selection in a
sub-menu, with the skip on):

- Only two adding properties are applied while paused, both in the
  sub-menus' movie `417E7260`: property 1 (float, node +0x10) and
  property 0 (float, node +0xC). Only three targets ever change:
  `035D19B0` (node `035D19A0` +0x10), `035D1A50` (node `035D1A40` +0x10,
  now and then) and `035DF5CC` (node `035DF5C0` +0xC, from 1280).
- **The arrow's track is property 1 on node `035D19A0`** (+0x10, its y):
  every slide moves it, from its rest value 6 to 27.6 then 39.6 on the
  first two ticks after a move down (r4 = 20), to -16.3 then -28.3 after a
  move up (19).
- Within one update the value goes 6 (update start) -> 27.6 (the track) ->
  6 (update end). What puts it back is **`82119B80`**, called at the end of
  the update (from `8211D7CC`, r3 = the movie, r4 = `035D05F0`, the tree's
  root), recursing into groups from `82119E90`.
- **`82119B80` restores the movie's rest pose.** `82119B80(movie, live
  tree, rest tree, ...)` walks the live tree's children (by index, through
  `8259D658`) with the rest tree's matching ones, and for nodes whose tag
  (word +0) is one of six types (`0x47525000` "GRP", recursed into;
  `0x50494300` "PIC", `0x50524D00` "PRM", `0x53434500` "SCE",
  `0x54464D00` "TFM", and `0x54585400` "TXT")
  copies the rest node's fields over
  the live one: +12..+60 and +68..+80 for all of them, and per type
  +100..+136 or more (`82119D74` copies a sub-list through `8259D628`).
- So each update applies the tracks onto the rest pose, draws, and
  copies the rest pose back. **Adds on fields `82119B80` restores can't
  pile up**: re-applying them on render-only passes gives the same value as
  the logic pass. Skipping them only makes render-only frames draw the
  rest pose: **that's the ghosting** (the arrow drawn mid-slide on logic
  frames and at its rest position on render-only frames). The countdown
  flicker is likely the same in the HUD's movie (not logged: the trace is
  paused-only).
- What the skip fixed (the arrow sliding too far) must then be adding
  tracks on fields or nodes `82119B80` doesn't restore (another node
  type, a field outside the copied ranges, or a movie updated without it);
  not identified yet. The arrow's own track isn't one, in the first two
  ticks of its slides that the trace covers.

**The fix** (`65b0a73`, `--per_tick_ui_movie_tracks`, on by default,
`per_frame_logic.cpp`). This is **a workaround to support frame rates
above 30**: the game only ever ran one movie update a tick, and at 30 fps
(no render-only passes) none of it does anything. On a logic pass, each
adding track's target is noted as put back (its value at the end of the
update is the one from before its first track in that update: the rest
pose restored it) or not. On render-only passes, adding tracks run as
usual when their target was put back, and are skipped otherwise, so adds
that really pile up happen once a tick. Setting tracks always run. The
movie's own bytes are still replayed as before (`--per_tick_ui_movies`).

Result (user, 2026-10-18, 144 fps, `itc_059.log`): **the arrow stops on
its item, no pause menu flicker or overlap, and the race start countdown is
right.** The arrow still leaves a trail when it moves: that's **the game's
own effect**, also seen on the same arrow in the front-end menus at 30 fps,
just easier to see at 144 fps. The ghosting the earlier skip caused was a
position problem (render-only frames drawing the rest pose).

In `itc_059.log` (a new session: the sub-menus' movie was `42170060`) the
targets still skipped were few: most often `039F18D0` (property 1), which
flips between "put back" and "not put back" every ~5 ticks, so it's put back
on ticks where its add is 0 and not when the add moves it: the adds that
pile up. Skipping it on zero ticks or not makes no difference (the
render-only replay adds the same 0). Others: `421A40E0` property 6
(eight targets), and a few in `40B00060` and `400F0030`.
`--log_pause_movie` logs each target's change of classification (`pause
movie track target:`, paused or not) and the skipped tracks every 2 s
(`pause movie skipped tracks:`).

Not done (later, if ever): a native fix instead of the workaround, e.g. the
UI movie update run once a tick with its drawing replayed (see
`docs/tick-replay.md`), or the adds that don't get put back identified and
made per tick by name. The exact replay planned above isn't needed.

**The front-end arrow snapping to the first item** (user, 2026-10-18):
in the front-end menus the arrow snaps when moving to a menu's first item,
instead of the trail it shows on other moves. Not caused by this branch:

- The front end has no render-only passes (`itc_058.log`, `itc_059.log`:
  every `fps` line equals `ticks/s`, 30/30), so the track skip and the
  movie replay's fallback never run there. On logic passes the
  `825A0548` override only reads memory and calls the original, and the
  wrappers in `pause_movie_log.cpp` only log (with `--log_pause_movie`).
- The user saw the same snap on a build from before the branch
  (`out/dist/itc-texture-binding-linux-amd64`, 2026-10-16).

The user then found a video of the Xbox 360 game doing the same: **it's
the game's own behavior**, nothing to fix.

**State of the branch** (`pause-menu-arrow`, all committed and pushed):

| Flag | Default | What | Keep? |
|---|---|---|---|
| `--per_tick_ui_movie_tracks` | on | Workaround for frame rates above 30: on render-only passes, skips adding tracks (`825A0548`) in UI movie updates whose target the update doesn't put back. Fixes the arrow; no ghosting or countdown flicker | Keep |
| `--per_tick_changed_ui_movies` | on | Render-only UI movie updates that fall back (movie changed since the logic pass, or no logic pass call to match) run from the current state, which is put back | Correct but made no visible difference; the user decides |
| `--log_pause_movie` | off | Logs UI movie updates by caller and pass type; while paused, diffs each movie and objects 3 pointers away; logs the sub-menus' animation calls and, after each, every update with its replay outcome and clocks, every adding track applied, the words they move around each update and which call writes them. Off, its wrappers and call sites check the flag first and only call the game's function, like the other debug logs | Debug |
| `--paused_render_only_skip` | off | Skipped functions wrapped in `menu_selection_scan.cpp` on render-only passes while paused | Removed |

Useful logs: `itc_037`-`itc_039` (first movie logs), `itc_042` (30 fps
reference), `itc_043` (the update trace around moves), `itc_046` (nodes
watched), `itc_047` (Scene::Update skip crash), `itc_050` (loading
crash). Frame dumps from the paused run (`shown_<n>.png`, the arrow at
"Road Map" and missing after moves) were in the session scratchpad, not
the repo.
