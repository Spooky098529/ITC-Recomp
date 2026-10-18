# Pause menu: input repeating too fast

In a race, the pause menu's input repeats too fast with frames uncapped
or above 30 fps: holding a direction moves through the menu several items
a tick. This is the plan to fix it and to translate the game's input
handling to native code. Nothing here is done yet (2026-10-18).

Addresses below are guest addresses. `0x82000A30` and friends are floats in
the game's constant data, read with `guest::LoadU32` and bit-cast (their
values aren't known yet; log them once rather than guessing). Offsets are
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

There's no known reliable pause flag yet: until step 0 finds one, the
`ctx.r31 + 588` value is the only sign of pausing we have.

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
the same results won't fix the pause menu on its own. Something that runs
on every frame uses the pads: a Draw handler, or a tick handler other than
the input one. It takes a held button as new, or counts repeats per frame.

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

### Step 1: native Input::Update (`src/hooks/input.cpp`, `--native_input`)

Follow `src/hooks/lights.cpp` (a `REXCVAR_DEFINE_BOOL`, the override
calling the translated code when the flag is off). Off by default until
checked.

- `sub_820990F0` keeps its render-only skip; its override moves into
  `input.cpp` (only one override per function can exist) or calls into it.
- Physical and logical pads natively, as described above. Call the kernel
  imports as the thunks do (`__imp__XamInputGetState`,
  `__imp__XamInputGetCapabilities`, `__imp__XamInputSetState`, with
  arguments in ctx registers and the structs in guest memory, e.g. on a
  `guest::StackFrame`), so `--mnk_mode` still works through the SDK.
- Logical pads sum physical pads' values directly when step 0 shows their
  slot 1 returns `device + 8`; any other device goes through its guest
  vtable call (`REX_CALL_INDIRECT_FUNC`).
- Processors and listeners stay guest calls, with r3/r4 set as above.
- `--verify_native_input`: run both (translated first into a copy, then
  native) each tick and log any byte that differs in the 4 physical and 5
  logical pads. Running XInputGetState twice a tick is harmless; rumble
  timers aren't, so restore the motors (+168..+191) between the two.
  Check with an attract race and a race the user drives, then turn the flag on.

### Step 2: fix the pause menu

- Using step 0's log, make the consumer act once a tick: skip it on
  render-only passes (`--render_only_skip` for a message handler, or the
  tick handler list for a tick handler), or, if it's one of the processors, give
  it presses and repeats counted per tick from step 1. The pause screen
  stays interpolated. Skipping something that also records UI makes it
  flicker (as menus did, see "Interpolation" in `docs/functions.md`).
- Remove the `ctx.r31 + 588` check from `sub_8215B4F8`.

### Step 3: later, toward moving off ReXGlue

- The processors natively, once identified.
- Possibly read SDL directly instead of `XamInputGetState`. This needs our own
  mouse and keyboard handling to replace `--mnk_mode`.

### When done

Update the `Input::Update` row and "Menus run at 30 fps" in
`docs/functions.md`, and this document's status.
