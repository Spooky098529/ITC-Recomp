# Pause menu: input repeating too fast

In a race, the pause menu's input repeats too fast with frames uncapped
or above 30 fps: holding a direction moves through the menu several items
a tick. This is the plan to fix it and to translate the game's input
handling to native code. Nothing here is done yet (2026-10-18).

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

### Input::Update (`sub_820990F0`)

The only path to the controllers: `sub_825BA3F8` (XInputGetState, a
thunk to `XamInputGetState`) has one caller, `sub_82591C18`, whose only
caller is `sub_820990F0`. It's already skipped on render-only passes, so
pad state changes once a tick. r3 = the input manager:

1. **Physical pads**: 4 at +988, 212 bytes each, read by `sub_82591C18`
   (r3 = pad, r4 = pointer to the user index):
   - Clears 160 bytes from +8, then calls XInputGetState.
   - The flags at +4 (bit 0 connected, bit 1/2 tracking connects and
     disconnects) are updated. On a connect it reads capabilities into +192
     (`sub_825BA3F0`).
   - Sends rumble from the motors at +168 and +180 (`sub_825BA408`,
     XInputSetState).
   - Buttons map to 1.0 (the float at `0x82000A30`) in float slot `n` at
     +(10 + n) * 4, from a table of (mask, slot) pairs: d-pad up/down/
     left/right → 0-3, A/B/X/Y (0x1000-0x8000) → 4-7, LB (0x100) → 12,
     left stick click (0x40) → 14, RB (0x200) → 16, right stick click
     (0x80) → 18, Start (0x10) → 20, Back (0x20) → 21.
   - Triggers and sticks are converted to floats and scaled into +16..+36,
     +92, +108 and +136..+164.
   - There's no edge detection and no repeat here.
2. **Logical pads**: 5 at +8, 196 bytes each, built by `sub_82591918`.
   Each one clears its values, then sums up to 4 bound devices (slots of {u8
   enabled, device pointer} from +4; each device's values come from its
   vtable slot 1) and clamps the sums.
3. **Unknown objects**: 5 at +1840, 416 bytes each, each one's vtable slot 1
   called with its logical pad. These are probably where presses and repeats are
   worked out (not checked).
4. **Listeners**: 5 slots at +3924, 12 bytes each. When one is set, its
   target's vtable slot 1 is called with the logical pad.

Since pad state already changes once a tick, a native Input::Update with
the same results won't fix the pause menu on its own. Something that runs
on every frame uses the pads: a Draw handler, or a tick handler other than
the input one. It takes a held button as new, or counts repeats per frame.

## Plan

### Step 0: logging (`--log_input`)

- Once, the targets of the +1840 objects' slot 1 and of the set +3924
  listeners: who handles presses and repeats.
- The pause state from `0x826BAC00 + 588` beside the `ctx.r31 + 588` that
  `78456a5` reads, and what `ctx.r31` points at, to find the real flag.
- While paused: the message handlers and tick handlers run on render-only
  passes (building on `--log_tick_handlers`), compared with a race that
  isn't paused.
- The user runs a race, pauses, holds a direction in the menu and quits; the
  log is read before step 2 is chosen.

### Step 1: native Input::Update (`src/hooks/input.cpp`, `--native_input`)

Off by default until checked.

- `sub_82591C18` natively: the same `XamInputGetState` and
  `XamInputSetState` calls (so `--mnk_mode` still works through the SDK),
  the button table, the stick and trigger scaling, and the connect flags.
- `sub_82591918` natively. Devices that are physical pads are summed
  directly; any other device goes through its guest vtable call.
- The +1840 and +3924 calls stay guest indirect calls for now.
- `--verify_native_input`: run both each tick and log any byte that differs
  in the 4 physical and 5 logical pads, before the flag goes on.

### Step 2: fix the pause menu

- Using step 0's log, make the consumer act once a tick: skip it on
  render-only passes, or, if it's one of the +1840 processors, give it presses
  and repeats counted per tick from step 1. The pause screen stays
  interpolated.
- Remove the `ctx.r31 + 588` check from `sub_8215B4F8`.

### Step 3: later, toward moving off ReXGlue

- The +1840 processors natively, once identified.
- Possibly read SDL directly instead of `XamInputGetState`. This needs our own
  mouse and keyboard handling to replace `--mnk_mode`.

### When done

Update the `Input::Update` row and "Menus run at 30 fps" in
`docs/functions.md`.
