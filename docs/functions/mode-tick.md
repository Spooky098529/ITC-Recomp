# Known functions: the mode manager's tick services

What the mode manager's tick (`Modes::Tick`, `sub_82107910`, its row in
[`game-modes.md`](game-modes.md), shared) runs besides the current mode's
own slots: the per-mode settings set when a mode is made (which system
checks the mode allows, the scene target's mode, the presence context),
the **sign-in watch** run every pass (the player's user and the
signed-in users compared with the last pass's; a change, a lost
connection or a user not on Live starts one of nine state handlers,
probably the system's prompts, none of which ran in play), the light
billboards' ring flipped, the two **screen fades** (a full-screen color
quad faded in or out over a number of passes, layer 1 over layer 0),
the menu music started once (`Music::Play` with `BGM_SYSTEM`), and the
input unblocked and the watch reset when the mode changes. The mode
holder's constructor (`Modes::Modes`, `sub_821067F8`) is in
`game-modes.md`, with the holder.

Shared, with rows in other files: `Modes::Tick` (`sub_82107910`) and
`Modes::Modes` (`sub_821067F8`) in [`game-modes.md`](game-modes.md);
XUserSetContextEx's wrapper (`sub_82294F70`, Dropped) and
`Network::Update` (`sub_8214E268`, which writes the signed-in users this
file's watch reads) in [`system-services.md`](system-services.md);
`wcscmp`, `wcscpy`, `memset` and `memcpy` (`sub_825A7EB0`,
`sub_825A7F30`, `sub_825A71D0`, `sub_825A7410`) in
[`c-runtime.md`](c-runtime.md); `operator new` and `delete`
(`sub_825A6880`, `sub_825A70E8`) in [`heap.md`](heap.md);
`CommandList::OpenChunk`, `AllocateSlots`, `CloseChunk` and
`AllocateVertices` (`sub_8258C9D0`, `sub_8258CAA0`, `sub_8258CA50`,
`sub_8258CB08`) in [`command-list.md`](command-list.md);
`Billboards::BuildVertices` (`sub_8209C6E0`, which writes the ring
`Billboards::FlipRing` flips) in [`sprites.md`](sprites.md);
`Stages::Copy` (`sub_820FFD88`) and `SaveData::Of` (`sub_82105620`) in
[`vehicle-accessors.md`](vehicle-accessors.md); SetLastError
(`sub_82296FD0`) in [`functions.md`](functions.md); atexit
(`sub_825A7068`, the core's, `cameras.md`'s intro). Not here: the save
data's creation and deletion the mode change runs when `0x826BAB70` is
set (`sub_821057C8`, `sub_821055D8`, and the defaults under them: the
data load's lead and a lead added by this map in `../loops/leads.md`);
the sign-in watch's nine state handlers (`sub_8209E4F0`,
`sub_8209E540`, `sub_8209E860`, `sub_8209E9A8`, `sub_8209E730`,
`sub_8209ECC8`, `sub_8209E610`, `sub_8209E698`, `sub_8209E790`) and
`sub_822910F0`, which never ran (no rows); `sub_8217A758`, the color
rectangle's other kind, never ran. The globals are in `../known-data.md`
(`0x826BAB1C`-`0x826BAB70`, the screen fades at `0x826A50AC`, the
billboards' ring at `0x826A5098`, `0x8266D08C`'s +28, `0x826C5C80`,
`0x826D83D0`, and the music names at `0x82006A18` in "Values from the
image"). Mapped by the B7 loop's mapping agent (`../loops/b7-loop.md`),
2026-10-25, from the generated code only (no runs): call counts aren't
known; vtable slots are inferred from the callers' offsets. Names are
ours.

The presence context (`Presence::SetForMode`) follows B5's call (map
Xbox features, don't port them; presence is one): XUserSetContextEx is
already Dropped at its wrapper (`sub_82294F70`, B5 step 2), and
`Presence::SetForMode` is Dropped too (B7 loop, 2026-10-25): its override
does only what's left of its calls, the last error the dropped wrapper
clears (`Modes::Tick` overwrites r3 right after the call and reads no last
error itself; the new mode's slot 0, called next, isn't read, so the
clearing stays until that's known).

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
| `sub_82106868` | `Modes::SetChecks` | Overridden | r3 = the holder (not read), r4 = the new mode's type: the seven words at `0x826BAB40` (the sign-in watch's checks a mode turns off, 1 = off; `SignInWatch::Update` reads +0 and +4, `SignInWatch::CheckIdle` +8, +16 and +20) by the type: 2, 5 to 10 and 12 clear all and set +16, +20, +24; 3 the same, or all 1 when `[0x826C5BAC]` (the boot movies' first id, `Boot::StartMovie` in `functions.md`) is 10 (the UBI logo); 4 and any other type (0, 11, above 12) all 1; 1 (the race) all 1 when the race data's word +0xA3AF0 (`[0x826C5B2C]`) is 0, else all cleared then +16, +20, +24 set, but with +0xA3AF0 6 (two human cars) only +20 and +24 set, and only when the current stage's word +0 (`Stages::Copy(frame, race data + 0x59620, -1)`) is 1 (else all stay 0). Leaf but `Stages::Copy`. Only caller `Modes::Tick`, once a mode made (after `Mode::Create`). Read in full. Native (B7 loop, 2026-10-25): `mode_tick::ModesSetChecks` (`src/mode_tick/`), adapter in `src/hooks/mode_tick.cpp`, `--native_mode_tick`; type 1's stage copied by Stages::Copy's plain C++ into the original's frame at +80 (r1 − 176, the leftovers as the original leaves them), r3 then its out as the original's; where that plain C++ declines, the original from the start. Harness `mode_tick` (types 0 to 13, −1, −2, random; type 1's branches). |
| `sub_82106740` | `Modes::SetSceneTargetMode` | Overridden | r3 = the holder (not read), r4 = the new mode's type: `[0x8266D08C]` + 28 (the 1024 × 576 target object's mode, which `sub_8209B3D0` turns into pass bits: 0 gives 7, 1 4, 2 0, 3 1, 4 8; a lead in `../loops/leads.md`) = 1 for type 1, 3 for type 3 (the chunk `sub_82106778`), 4 for types 2, 5 to 8 and 12, 0 for 4, 9, 10, 11 and any other. A switch over the type. Only caller `Modes::Tick`, once a mode made. Read in full. Native (B7 loop, 2026-10-25): `mode_tick::ModesSetSceneTargetMode`, `--native_mode_tick`; harness `mode_tick`. |
| `sub_82106778` |  | Merged | A piece of `Modes::SetSceneTargetMode`'s switch (type 3): the mode 3, then its common tail. Called only from it. Merged into `mode_tick::ModesSetSceneTargetMode` (B7 loop, 2026-10-25, its type 3); harness `mode_tick`'s `Modes::SetSceneTargetMode` cases. |
| `sub_822934B8` | `Presence::SetForMode` | Dropped | r3 = the new mode's type: XUserSetContextEx (`sub_82294F70`, Dropped in `system-services.md`) for player 1's controller's user (`[0x826BABA4]`) by the type: 1 nothing; 2, 3, 5 and 9 context 0 = 0 (the chunk `sub_82293504`), then context `0x8001` (presence) = 1; 6, 7, 8, 12 context 0 = 1, 2, 4, 3, then `0x8001` = 1; 4, 10, 11 and any other type only `0x8001` = 0. Only caller `Modes::Tick`, once a mode made. Presence is an Xbox feature (B5's call: mapped, not ported): probably Dropped at `Modes::Tick` when that's native, the call left out. Read in full. Dropped (B7 loop, 2026-10-25), following B5's call for presence (map Xbox features, don't port; XUserSetContextEx's wrapper `sub_82294F70` Dropped in B5 step 2): no presence is set, and what's left of its calls is what the dropped wrapper does, the last error cleared and r3 = 0 (type 1, which makes no call, leaves both); `mode_tick::PresenceSetForMode` behind `--native_mode_tick` (`src/hooks/mode_tick.cpp`). Harness `mode_tick`: the last error and r3 the same for types 0 to 13, −1, −2 and random. No presence machinery. |
| `sub_82293504` |  | Merged | A piece of `Presence::SetForMode`'s switch (types 2, 3, 5, 9): context 0 = 0, then context `0x8001` = 1. Called only from it. Merged into `mode_tick::PresenceSetForMode` (Dropped, B7 loop, 2026-10-25); harness `mode_tick`'s `Presence::SetForMode` cases. |
| `sub_8209EEC8` | `SignInWatch::Update` | | r3 = the object at `0x826C5B9C` (passed on to the state handlers only), r4 = the current mode (its +32 passed to handlers 2 and 6 to 9, or 0 with no mode). Every pass of `Modes::Tick` (its only caller). The user on player 1's controller (`[0x826BABA4]`, bit u): its name (`Players::SignedInSlot(0)`, the user table's 32-byte UTF-16 name at `0x826C7670` + 14 + 32 × slot, copied by `wcscpy` into its frame; zeros when not signed in). Against the last pass (`0x826BAB64` the signed-in mask, `0x826BAB60` the count, `0x826BAB1C` the name): with u not in the old mask and the mask changed, `0x826BAB68` = 1 when the count didn't drop, else `0x826BAB6C` = 1; with u in the old mask, `0x826BAB6C` = 1 when u is no longer in the mask (`[0x826C7954]`) or its name changed (`wcscmp`). The mode's checks (`0x826BAB40`, `Modes::SetChecks`) +0 = 1 clears `0x826BAB68`, +4 = 1 clears `0x826BAB6C`. Then by the state (`0x826BAB3C`, 0 to 9, a switch): 0 `SignInWatch::CheckIdle` (the chunk `sub_8209F040`), 1 to 9 the handlers `sub_8209E4F0`, `E540`, `E860`, `E9A8`, `E730`, `ECC8`, `E610`, `E698`, `E790` (none ran in play); in states 3 to 9 also `sub_822910F0` (never ran). Last, the mask and count (`[0x826C7954]`, `[0x826C7958]`, written by `Network::Update`) kept for the next pass, and the name too (`wcscpy`, or `memset` of 32 bytes when u isn't signed in). With our `Network::Update` (one user, written once) nothing changes after the first pass, so the state stays 0. Read in full but the handlers. |
| `sub_8209F040` | | | A piece of `SignInWatch::Update`'s switch (state 0): `SignInWatch::CheckIdle`, then the update's common tail (the mask, count and name kept). Called only from it. |
| `sub_8209ED58` | `SignInWatch::CheckIdle` | Overridden | No arguments read. State 0's check, every pass (from `SignInWatch::Update` through `sub_8209F040`): sets the reason `0x826BAB5C` and the state `0x826BAB3C` = 1 (a prompt; then handler 1 runs from the next pass) for the first of: `0x826BAB68` set (reason 0, another user signed in); `0x826BAB6C` set (reason 1, the player's user signed out or changed); check +8 off (0) and `[0x82816118]` 1 (reason 2; written by the original `Network::Update`, probably the Live connection lost); then two more, each overriding the one before: with check +20 off and no network session (`[0x826C7824]` 0 and `[0x826C7878]` not 0, or `[0x826C7824]` set and the 384-byte vector at `0x826C7308` empty; the network's, not read), reason 5; with check +16 off and u not in the Live mask (`[0x826C795C]`, 0 since B5), reason 4. In play every mode turns +16 off but type 1 with +0xA3AF0 6 (`Modes::SetChecks`), and none of the reasons came up (the handlers never ran). Leaf. Read in full. Native (B7 loop, 2026-10-25): `mode_tick::SignInWatchCheckIdle`, `--native_mode_tick`; where the original would trap (the vector's end before its begin, or a part entry: its traps only warn, then its loop runs until the address wraps), the original. Harness `mode_tick` (256 cases, every reason). |
| `sub_8209F658` | `Players::SignedInSlot` | Overridden | r3 = a player: the index among the signed-in users (the set bits of `[0x826C7954]`, lowest first, of 4) of the player's controller's user (`[0x826BABA4 + 4 × player]`), or -1 when that user isn't signed in. The slot in the user table at `0x826C7670` (`system-services.md`'s `Network::Update`). Leaf. From `SignInWatch::Update` (twice a pass) and others not read. Read in full. Native (B7 loop, 2026-10-25): `mode_tick::PlayersSignedInSlot` (r3 sign-extended), `--native_mode_tick`; harness `mode_tick`. |
| `sub_8209E7E8` | `SignInWatch::OnModeChange` | | r3 = `0x826C5B9C` (not read). From `Modes::Tick` when the mode's delegate asks for another type (after the old mode is deleted). When `0x826BAB70` is 1 (set by handler `sub_8209E698`, which never ran): player 0's save data deleted (`sub_821055D8(0)`) and made again (`sub_821057C8(0)`; a lead). Then the state `0x826BAB3C` = 0, the seven checks at `0x826BAB40` = 0 and `0x826BAB70` = 0. Read in full. |
| `sub_8209F318` | `ActionMappers::SetBlocked` | Overridden | r3 = a value: stored into the word +4 of each of the four action mapper entries (`0x826BAB80` + 8i + 4, `action-mappers.md`; 1 makes the menus and `PlayerInput::Read` skip the mapper, `racer-controllers.md`). From `Modes::Tick` with 0 on a mode change, and from the sign-in watch's handlers (`8209E5FC` to `8209ED48`, never ran). Leaf. Read in full. Native (B7 loop, 2026-10-25): `mode_tick::ActionMappersSetBlocked`, `--native_mode_tick`; harness `mode_tick`. |
| `sub_825D9518` | `SignInWatch::StaticInit` (probably) | | No arguments, no direct callers (probably a C runtime static initializer, at boot): the state `0x826BAB3C` = 0, the seven checks at `0x826BAB40` = 0, `0x826BAB70` = 0, the 32-byte name at `0x826BAB1C` cleared (`memset`), and atexit (`sub_825A7068`) with `sub_825DAF08` (an empty function, `blr`). Read in full. |
| `sub_8209BD18` | `Billboards::FlipRing` | Overridden | No arguments: the billboards' ring half `[0x826A5098]` = 1 when it was 0, else 0, and the vertex count `[0x826A509C]` = 0 (the ring `Billboards::BuildVertices` writes 28-byte vertices into at `0x8266D098` + ((half << 12) + count) × 28, `sprites.md`). Every pass of `Modes::Tick` (its only caller). Leaf. Read in full. Native (B7 loop, 2026-10-25): `mode_tick::BillboardsFlipRing` (`src/mode_tick/`), adapter in `src/hooks/mode_tick.cpp`, `--native_mode_tick`; harness `mode_tick`. |
| `sub_8209CDE8` | `ScreenFades::Init` | Overridden | No arguments: the two fades' alphas (`0x826A50D4`), out durations (`0x826A50AC`), in durations (`0x826A50B4`), out counts (`0x826A50E4`), in counts (`0x826A50DC`) and colors (`0x826A50BC`, 3 words each) = 0. Once at boot, from the app's start-up `sub_82106548` (a lead). Leaf. Read in full. Native (B7 loop, 2026-10-25): `mode_tick::ScreenFadesInit`, `--native_mode_tick`; harness `mode_tick`. |
| `sub_8209D038` | `ScreenFades::FadeIn` | Overridden | r3 = the passes, r4, r5, r6 = the color's red, green, blue (words), r7 = the layer (0 or 1): the layer's in count and in duration = r3, its out count and alpha = 0, its color set; the other layer's in and out counts and alpha = 0. Over the passes `ScreenFades::Update` takes the color's alpha from 255 down to 0 (the scene appears). 79 call sites (not read). Leaf. Read in full. Native (B7 loop, 2026-10-25): `mode_tick::ScreenFadesFadeIn` (the stores in the original's order: a layer above 1 lands on the other arrays), `--native_mode_tick`; harness `mode_tick` (layers 0 to 4). |
| `sub_8209D0C8` | `ScreenFades::FadeOut` | Overridden | r3 = the passes, r4, r5, r6 = the color, r7 = the layer: the layer's out count and out duration = r3, its in count and alpha = 0, its color set (the other layer left alone). Over the passes `ScreenFades::Update` takes the alpha up to 255, then holds it there. 65 call sites (not read). Leaf. Read in full. Native (B7 loop, 2026-10-25): `mode_tick::ScreenFadesFadeOut`, `--native_mode_tick`; harness `mode_tick` (layers 0 to 4). |
| `sub_8209D130` | `ScreenFades::Update` | | r3 not read (0 from `Modes::Tick`, which doesn't read the answer). Every pass, after the mode's delegate is asked. For layer 0 (only when layer 1's in and out counts are both 0) and layer 1: an in count above 0 goes down by 1 and the alpha = int(255.0 / in duration × count); then an out count above 0 goes down by 1 and the alpha = 255 at 0, else int(255.0 / out duration × (duration − count)) (floats, `[0x82000C44]` 255.0, truncated). Then a color rectangle on the stack (an identity matrix at +0, kind +64 = 0, +72 = 2, x, y 0, 1280 × 720, the layer's color and alpha at +96 to +108) and, when the alpha is above 0, `ScreenFades::RecordScissor(key)` and `ColorRect::Record(rectangle, 0x826BAC00 (camera pass 0), key + 1)`, the keys 668 and 676 (`0x82001BF4`, `0x82001BF8`; f1 −10,000.0 from `0x82001C00` passed, not read). Returns 1 while any of the four counts isn't 0, else 0. Read in full. |
| `sub_8209CE68` | `ScreenFades::RecordScissor` (probably) | | r3 = a sort key (f1 not read): on the scene's command list (`[0x826BABA0]`'s slot 8, then slot 1; the render queue through slot 8, then slot 2), two chunks: a header (halfword +2 = 5, +4 = 0), command 31 (1: a scissor rectangle, `../gpu-functions.md`) and a slot of halfwords (0, 0, 1280, 720), command 36; queued with the key and −10,000.0 (`0x82001C00`) through the queue's slot 4; then a header, command 1 (1), command 200 (0), command 36, queued with the key and −9,000.0 (`0x82001BFC`). Only caller `ScreenFades::Update`. Read in full; what commands 1 and 200 do isn't known. |
| `sub_8217C348` | `ColorRect::Record` | | r3 = a color rectangle, r4 = a camera pass (not read), r5 = a sort key: kind +64 0 tail-calls `ColorRect::RecordFlat`, 1 `sub_8217A758` (never ran), above 1 nothing. From `ScreenFades::Update` (and others not read). Read in full. |
| `sub_8217A370` | `ColorRect::RecordFlat` | | r3 = a color rectangle (+80 x, +84 y, +88 width, +92 height, integers; +96, +100, +104, +108 red, green, blue, alpha words), r5 = a sort key: on the command list at `[0x826D83D0 + 4]` (the render queue at `+0`) a chunk: the header (halfword +2 = 5, +4 = 0); with an alpha above 0 [1, 5, 0], [56, 6], [96, 0], [60, 1], [72, 6], [76, 7] (blending on), else [1, 3, 0], [56, 6], [96, 0], [60, 0]; then [13, 1, 0], [6, 1, 0], [5, 4, 0], four 24-byte vertices (`CommandList::AllocateVertices(4)`, no header: +0 x, +4 y, +8 z = 1.0 as floats, the corners; +12 the color as ARGB, the alpha word's low byte then the red, green and blue words' low bytes; +16 to +23 not written), [15, 1, 0] (the helper object's draw), [36, 0, 0]; closed and queued with the key and 0.0 (`[0x82000A34]`) through the queue's slot 4. Like `ScreenRect::Record` in `sprites.md`, without a texture. From `ColorRect::Record`. Read in full. |
| `sub_821063B0` | `Music::Play` (probably) | | r3, r5 = two pointers passed on (from `Modes::Tick`: a word 1 and a byte 0), r4 = a music name (C string): its index i in the 18 names at `0x82006A18`-`0x82006AE4` (`bgm_tmp_01` to `bgm_tmp_17` and `BGM_SYSTEM`, compared by `lstrcmpA`; `../known-data.md`), then `[0x826C5C80]`'s slot 4 (r4 = r3, r5 = the name, r6 = r5, r7 = &(22990 + i), r8 = &(22972 + i); a name not in the list gives 22888 and 22887: probably the song's two text ids). `[0x826C5C80]` is set by `sub_82110258` (in the X3DAudio lead). From `Modes::Tick` once, with `BGM_SYSTEM`, when the first menu-type mode runs (`0x826C5B94` 0 and `0x826C5B98` 1; `0x826C5B94` = 1 after), and 14 other call sites (the race code `82175EC0`-`82176DC4`, `82110358`, `821BA2F8`, `82264D18`, `82264D50`; not read). The slot's target not read (a lead). Read in full. |
| `sub_82296130` | `lstrcmpA` (probably) | | r3, r4 = two C strings: `CompareStringA(0, r3, -1, r4, -1)` − 2; when that fails (0), compares them itself: a null first string -1 (0 when both are null), a null second 1, else the first differing byte's difference (bytes as unsigned). XAPI's (kernel32's) string compare, a pure helper. From `Music::Play` (once a name tested). Read in full. |
| `sub_82297100` | `CompareStringA` (probably) | | r3 = flags, r4 = string 1, r5 = its length (-1: terminated), r6 = string 2, r7 = its length: 1 less, 2 equal, 3 greater; 0 with `SetLastError(87)` for a null string or a length below -1. Compares through the character type table at `0x82638340` (a word a byte), with flags 1 (probably NORM_IGNORECASE: upper case folded by adding 32), 4 and 0x1000 changing how symbols and marks count; for strings equal but for what was skipped, the answer from the table at `0x82638740`. Not read in detail. From `lstrcmpA` and `sub_822961E0` (never ran; probably `lstrcmpiA`). |
