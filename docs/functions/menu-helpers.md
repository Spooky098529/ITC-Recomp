# Known functions: the front end's menu helpers

The two helpers about a hundred front-end screens step their menus and
read their buttons with, each once a page handler runs (every update
while that page shows). A menu is a 68-byte object each screen
allocates (`VirtualAlloc`) and fills in itself, inline, in its create
function (no constructor or reset of its own; the parking area's four,
`LocationScreen::Create` in `location-screen.md`, and the screen at
`sub_821869B8`, in `../loops/leads.md`, are two): a cursor kept between
a first and a last item, whether it wraps, which inputs move it, and the
player whose pad drives it. Its layout is in `../known-data.md` ("front-end
menu"). `Menu::Step` moves the cursor from the player's action mapper
(`action-mappers.md`: `ActionMapper::Fired`, slot 12, for the d-pad and
the stick, with their repeat; `ActionMapper::Pressed`, slot 10, and
`Fired` for the shoulder buttons) and says which way it moved;
`Menu::Pressed` answers whether an action (159 accept and 158 back at
nearly every site) was pressed on a step that didn't move the menu. A
typical page handler: `Menu::Step`, then `Menu::Pressed(menu, 159)` and
`Menu::Pressed(menu, 158)`, each answered with `MenuSound::Play` (cue 0
and 2, `menu-sounds.md`) and a page transition. The action ids are the
ones in `../pause-menu-fix.md` (153 to 156 the d-pad's up, down, left
and right, 167 to 170 the stick's, 157 Y, 158 B, 159 A, 160 X, 161 and
162 the shoulder buttons LB and RB). Names are ours.

The player's mapper is found the way `MenuList::Fired` and
`PlayerInput::Read` find it (`race-screens.md`, `racer-controllers.md`):
the 8-byte entry at `0x826BAB80` + 8 × `[0x826BABA4 + 4 × player]`, its
+0 the mapper and its word +4 1 when the mappers are blocked
(`ActionMappers::SetBlocked`, `mode-tick.md`), in which case nothing is
read. The race screens' menu lists (`MenuList::`, `race-screens.md`,
made by `MenuList::Init` in `save-data.md`) are another, larger class
with their own update; the helpers here don't touch them.

Their callers (113 functions: 104 call `Menu::Pressed`, 101 `Menu::Step`, 92 both;
the screens, other subsystems, not mapped here): `82187160`-`82187618`
(the screen around these helpers, a lead), `8218AFD8`, `8218B2C8`,
`8218B750`, `8218E8F8`, `8218FCC8`, `821AA560`-`821C6F58` (39, the save
screens' range, a lead), `821D6AD0`-`821DFD48` (6, `Menu::Step` only,
beside the tuning screen), `821F9050`-`821FC270` (6), `8220B0D8` (the
screen at `sub_8220B408`, a lead), `8221E788`, `8221E980`, `8221EAE8`
(kind 6's in-race screen after the race, a lead), `82222378`,
`82225D10`, `82225F30`, `82226088`, `82228A90`-`8222CA48` (11, the
parking area's pages, `location-screen.md`), `82232328`-`82250948`
(9), `8225A9B0`, `8225C428`, `8225EC70` and `82262040`-`822860D8`
(22). Those seen in play and in no function file or lead are in the
lead "the front-end screens that use the menu helpers"
(`../loops/leads.md`). Action ids at `Menu::Pressed`'s call sites: 159
(98 sites), 158 (92), 160 (8), 163 (6), 164 (4), 157 (3), 161 and 162
(2 each).

Not here: `sub_82186428`, `sub_82186490` and `sub_821864A0`, just
before `Menu::Pressed` in the image, belong to the screen at
`sub_821869B8` (two transitions its set-up `sub_82188178` adds, and a
test of its `[0x82636204]`): in the lead with it. Shared, with rows
elsewhere: `ActionMapper::Pressed` and `ActionMapper::Fired`
(`sub_8210BF40`, `sub_8210C010`, `action-mappers.md`, reached through
the mapper's vtable), `MenuSound::Play` (`sub_821103F0`,
`menu-sounds.md`, from the callers).

Mapped by the B7 loop (2026-11-06; read in full). Both are ready groups
of one function each (`b7_groups.py`: leaves whose only calls are the
mapper's slots, through its vtable): `Menu::Step` and `Menu::Pressed`.

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
| `sub_82186568` | `Menu::Step` | | r3 = a menu (68 bytes, `../known-data.md`'s "front-end menu"). A 128-byte frame (r27 to r31 saved). +4 = the cursor (+0), +36 = 0; the player's mapper entry (`0x826BAB80` + 8 × `[0x826BABA4 + 4 × +64]`). Every read below is skipped when the entry's +4 is 1. Then, in order: (1) with +24 0 (up and down not locked), the first of these fired (slot 12, +48, `ActionMapper::Fired`) wins: 153 or 167 (up) the cursor - 1, 154 or 168 (down) the cursor + 1, 155 or 169 +36 = -1, 156 or 170 +36 = 1. (2) With +28 set (left and right step the cursor): the first of 155, 169 (cursor - 1), 156, 170 (cursor + 1) fired. (3) With +32 set (the shoulder buttons step it): 161 pressed (slot 10, +40, `ActionMapper::Pressed`), else 161 fired: cursor - 1; else 162 pressed, else 162 fired: cursor + 1. A call can step it in each of the three. Then with +20 set (it wraps): below the first item (+12) the cursor goes to the last (+16), above the last to the first, and +40 = 1 when it wrapped, else 0; with +20 0: +40 = 0 and the cursor clamped to +12..+16. Last, +8 = 1 when the cursor is above +4, -1 below, 0 the same, and returned. Up to 16 indirect calls. From 101 front-end page handlers (the file's intro), every update of their page. Read in full. Seen in play. |
| `sub_821864C8` | `Menu::Pressed` | | r3 = a menu, r4 = an action id. A 96-byte frame (lr only). 1 when the last `Menu::Step` didn't move the menu (+8 0) nor ask for a side (+36 0), the player's mapper entry (`0x826BAB80` + 8 × `[0x826BABA4 + 4 × +64]`) isn't blocked (its +4 not 1) and its mapper's slot 10 (+40, `ActionMapper::Pressed`) answers nonzero for r4 (passed through); else 0 (the mapper not asked when the menu moved or the entry is blocked). One indirect call. From 104 front-end page handlers, mostly with 159 (accept) and 158 (back), after `Menu::Step` on the same menu. Read in full. Seen in play. |
