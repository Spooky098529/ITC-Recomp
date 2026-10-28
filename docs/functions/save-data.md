# Known functions: the players' save data

A player's save data in memory (`[0x826C5B58 + 4 × player]`, read
through `SaveData::Of`; 0x1F978 bytes, `../known-data.md`): made with its
defaults (`SaveData::Create`: `operator new`, then each part set up) and
deleted (`SaveData::Delete`), and the default values of its parts: the
progress (+0 to +0x1D52C: the player's stats record at +48, the location,
the money, the garage list, the item records, the unlock bit sets copied
from five static tables, and the play time at creation; cleared by
`Progress::Clear`, then given a new game's values by
`Progress::SetDefaults`), the 22 course records (+0x1D52C), the sound
bank holder (+0x1F850) and the options (+0x1F85C: settings words and
bytes, then the four default control maps that `PlayerControl::ReadPad`
reads). It's made once at boot for player 0 (from the data load
`sub_82105868`, a lead in `../loops/leads.md`), and made again for player
0 (deleted, made, the controllers reset) by `TitleScreen::ResetPlayer`
(`sub_82221D00`, a front-end screen's handler) and by
`SignInWatch::OnModeChange` when `0x826BAB70` is set (`mode-tick.md`,
which it blocks). This is the data in memory only: the save files on
disk are B4's (`files-and-saves.md`, `src/saves/`), and nothing here
calls an Xbox service. Shared, with their rows elsewhere:
`SaveData::Of` (`sub_82105620`, `vehicle-accessors.md`);
`CourseRecord::Reset` (`sub_82105C90`) and `sub_820FF558`
(`race-setup.md`); `SoundBankHolder::SoundBankHolder` (`sub_8259D3D8`,
`sound-cues.md`); `operator new` and `operator delete` (`sub_825A6880`,
`sub_825A70E8`), `VirtualAlloc` and `VirtualFree` (`sub_82295398`,
`sub_82295330`, `heap.md`); `memset` and `memcpy` (`sub_825A71D0`,
`sub_825A7410`, `c-runtime.md`); `SignInWatch::OnModeChange`
(`sub_8209E7E8`, `mode-tick.md`). Four functions not seen in play have rows marked so (`sub_82105BF8`,
`sub_82105740`, `sub_82105688`, `sub_82114128`). Not here (leads in `../loops/leads.md`): the player's items over
the save data (`sub_82111780` and the rest, their own lead), the data
load at boot (`sub_82105868`), and the front end's title menu and menu
lists (`sub_82222B60`, `sub_82221AC0`, `sub_8220BC20` and the rest, added
by this map; `MenuList::Init` is here, as only the title's reset needed
it so far). Mapped 2026-10-28 by the B7 loop from the lead "a player's
save data made"; every row below was read in full in `generated/`
(offsets are the save data's unless said).

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
| `sub_821057C8` | `SaveData::Create` | | r3 = a player: 0x1F978 bytes from `operator new` (`sub_825A6880`); when not null, `Stats::Stats(+48)`; the block (or 0 when the allocation failed: the set-ups below still run, on addresses near 0) stored at `[0x826C5B58 + 4 × player]`; then `CourseRecords::Reset(+0x1D52C)`, `Progress::SetDefaults(+0)`, `Options::SetDefaults(+0x1F85C)` and `SoundBankHolder::SoundBankHolder(+0x1F850)` (`sound-cues.md`), each from the slot read again. Callers: the data load at boot `sub_82105868` (player 0; the data load's lead), `TitleScreen::ResetPlayer` and `SignInWatch::OnModeChange` (`mode-tick.md`; player 0, when `0x826BAB70` is set, which never happened in play). Read in full. |
| `sub_821055D8` | `SaveData::Delete` | | r3 = a player: `operator delete([0x826C5B58 + 4 × player])` (`sub_825A70E8`; nothing when 0), then the slot = 0. No destructor runs (the sound bank holder isn't released). Callers: `TitleScreen::ResetPlayer`, `SignInWatch::OnModeChange` and `sub_82105688` (not seen in play). Leaf but for the call. Read in full. |
| `sub_821056C0` | `Stats::Stats` | | Probably the stats record's constructor. r3 = the record (save data + 48): the float `[0x82000A34]` (0.0) to +4892 and 0 to +4896; 15 pairs {that float, 0} at +4960 + 48i; nine pairs {float, 0} at +5880 to +5944 (step 8). From `SaveData::Create` and `sub_82105740` (not seen in play). Everything it writes is cleared and written again by `Progress::Clear`'s `Stats::Reset` right after (B7 changes no behavior: kept as it is). Leaf. Read in full. |
| `sub_82105D08` | `CourseRecords::Reset` | | r3 = the course records (save data + 0x1D52C): +0, +4, +8, +12 = 0, halfwords +16, +18 = 0; then 22 records of 408 bytes from +20 (11 × 2, in order), each `CourseRecord::Reset` (`sub_82105C90`, `race-setup.md`, which calls `sub_820FF558`). Only caller `SaveData::Create`. The records are picked by the course and the traffic by `sub_82105BF8` (not seen in play). Read in full. |
| `sub_82113E60` | `Progress::SetDefaults` | | r3 = the save data: a new game's progress. `Progress::Clear`; +0 = 1; `memcpy` (`sub_825A7410`) of 60 bytes from `Defaults::Table50A8` into +0x19930, 28 from `Defaults::Table3C24` into +0x199A8 and 28 from `Defaults::Table50E4` into +0x199C4; +0x199E0 = 0x703 (bits 0, 1, 8, 9, 10 set one store at a time over 0); +0x199E8 and +0x199EC = 0, then +0x199E8 = 0xCF0F (bits 0-3, 8-11, 14, 15, one at a time); 60 bytes of `Defaults::Table3044` on the stack, each of its 15 words AND NOT `Defaults::Table3E34`'s, copied into +0x1996C; +0x19924 \|= 0x6BFDF3 (bits 0, 1, 4-8, 10-17, 19, 21, 22, one at a time); `BitSet400::SetAll(+0x1ACE4)`, then bits 310 to 375 of it cleared (word i >> 5, bit 1 << (i & 31)); +6088 = the play time frame counter `[0x826C5B90]`. Callers seen: `SaveData::Create`, the title menu's handler `sub_82221AC0` (after `Progress::Clear`, a lead); not seen: `sub_82105740`, `sub_82114128`. Probably the unlocks (the bit sets) and the cars or parts offered at the start. Read in full. |
| `sub_82113BD8` | `Progress::Clear` | | r3 = the save data: `memset` (`sub_825A71D0`) of 0x1D52C bytes to 0, then (over the zeros) +0, +4 = 0; byte +8 = 0, +12 = 6, halfword +16 = 0, byte +18 = 1, +20 = 6, halfword +24 = 0, byte +26 = 1, and the same from +28 (byte +28, +32 = 6, +36, byte +38 = 1, +40 = 6, +44, byte +46 = 1); `Stats::Reset(+48)`; +6092, +6096, +6100 = 0; +6108 = 14 (the location, `../known-data.md`); +6112 = 48; +6116 = 3000 (probably the money); +0x1AD1C = 4, +0x1AD18 = 0; the garage list at +6120: 50 nodes of 4 bytes {+0 next = i + 1, +2 car = i}, then halfwords +6316, +6322, +6324 (the list's head), +6326 = -1 and +6320, +6328 = 0; +0x1991C = 50, +0x19920 (the selected car) = 0; +0x19924 to +0x1992C, 15 words at +0x19930, 7 at +0x199A8, 7 at +0x199C4, +0x199E0 and +0x199E4 = 0; the 400 item records at +0x199F8 (12 bytes: halfwords +0 to +6, word +8) = 0; +0x1ACE0 = 0; six words at +0x1AD80 + 100i = 0; 1,536 words at +0x1AF78 = 0; +0x1D524 = -1; +0x1CA78, +0x1CA7C, +0x1CA80, +0x1D528, +0x1D504 and seven words at +0x1D508 = 0. Callers seen: `Progress::SetDefaults`, the title menu's handler `sub_82221AC0`; not seen: `sub_82114128`. Read in full. |
| `sub_82113730` | `Stats::Reset` | | r3 = the stats record (save data + 48, 6,044 bytes): `memset` of 6,044 bytes, then of +0 (24 bytes), +24 (4,828), +4864 (24) and +4924 (24) to 0; +4860 = 0.0 (`[0x82000A34]`), halfwords +4852, +4854 and the word +4856 = 0; +4888 and +4892 = 0.0, +4896, +4900, +4908, +4912 = 0, halfwords +4916 to +4922 = 0; +6032 = 0, bytes +6036 to +6038 = 0; ten 12-byte entries at +5676 {0, 0.0, 0} and +5672 = 0; `Stats::ResetRecords(+4948)`; +6008 = 3 (the last mode, save data +6056); three 14-byte entries at +5796 (seven halfwords each; save data +5844 + 14 × mode, the counts by mode) = 0; 14 halfwords at +5838 (save data +5886, the counts by location) = 0; six at +5866 = 0; +5956 = 1. Only caller `Progress::Clear`. Read in full. |
| `sub_821136D8` | `Stats::ResetRecords` | | r3 = 15 records of 48 bytes after a count (stats record + 4948): each record from +4: +0, +4 = 0, +8 = 0.0 (`[0x82000A34]`), +12, +16 = 0, +20, +24, +28 = -1, +32, +36, +44 = 0 (+40 not written); then the count +0 = 0. Probably times (the -1 words unset). Only caller `Stats::Reset`. Leaf. Read in full. |
| `sub_821133A8` | `BitSet400::SetAll` | | r3 = a bit set of 13 words: the words cleared, then bits 0 to 399 set one at a time (word i >> 5, bit 1 << (i & 31)); bits 400 to 415 stay 0. Only caller `Progress::SetDefaults` (save data + 0x1ACE4). Leaf. Read in full. |
| `sub_820CCCC0` | `Defaults::Table50A8` | | Returns `0x826C50A8` (a static table; 60 bytes copied by `Progress::SetDefaults` into save data +0x19930). Only caller `Progress::SetDefaults`. Who fills the table isn't read (no other code forms its address directly). Leaf. |
| `sub_820CCCD0` | `Defaults::Table3044` | | Returns `0x826C3044` (a static table; 60 bytes, `Progress::SetDefaults` takes out `Defaults::Table3E34`'s bits and keeps it at save data +0x1996C). Callers: `Progress::SetDefaults` and `sub_82110448` (the car object's makers' lead). Who fills it isn't read. Leaf. |
| `sub_820CCCE0` | `Defaults::Table3E34` | | Returns `0x826C3E34` (a static table; 15 words whose bits `Progress::SetDefaults` clears from `Defaults::Table3044`'s). Only caller `Progress::SetDefaults`. Who fills it isn't read. Leaf. |
| `sub_820CCCF0` | `Defaults::Table3C24` | | Returns `0x826C3C24` (a static table; 28 bytes copied into save data +0x199A8). Only caller `Progress::SetDefaults`. Who fills it isn't read. Leaf. |
| `sub_820CCD00` | `Defaults::Table50E4` | | Returns `0x826C50E4` (a static table; 28 bytes copied into save data +0x199C4). Only caller `Progress::SetDefaults`. Who fills it isn't read. Leaf. |
| `sub_82105940` | `Options::SetDefaults` | | r3 = the options (save data + 0x1F85C): +0 to +20 = 1 (six words; +20 is save data +0x1F870), +24 = 2, bytes +28 and +29 = 15, +32 = 0, bytes +36 to +41 = 0 (the settings bytes, three a player, save data +0x1F880); the four control maps at +44, +104 (player 0's pad and wheel), +164 and +224 (player 1's), each `ControlMap::SetDefault`. Callers: `SaveData::Create`, `sub_82105740` (not seen in play). Read in full. |
| `sub_821058A8` | `ControlMap::SetDefault` | | r3 = a control map (15 words, `../known-data.md`): the defaults built on the stack and copied: 18, 14, 13, 9, 0, 10, 16, 15, 24, 7, 17, 0, 25, 8, 11 (control numbers, 0 to 30, `PlayerInput::Control`, for +0 steering, +4 throttle, +8 brake and the rest). r3 is left as it was (its caller adds 60 to it for the next map). Only caller `Options::SetDefaults` (four times). Leaf. Read in full. |
| `sub_82221D00` | `TitleScreen::ResetPlayer` | | Probably the title screen's reset, from a front-end screen's handler (`sub_82222B60`, seen in play, no direct callers; and `sub_82222C90`, not seen): `MenuList::Init([0x82814E30], 1, 1, 0, 154, 153, 155, 156, 159, 158)` (a one-item list, the text ids probably), the title screen's idle ticks (`[0x82814E34]` +36, `../title-screen.md`) = 0, the mode's system checks `0x826BAB40` +0 to +12 = 1 (`mode-tick.md`); then player 0's save data deleted and made again (`SaveData::Delete(0)`, `SaveData::Create(0)`) and `Players::ResetControllers`. So the progress goes back to its defaults each time this runs. Read in full. |
| `sub_8220BAC0` | `MenuList::Init` | | r3 = a front-end menu list, r4 = the item count, r5 = the items shown (+12 = the smaller of r4 and r5), r6 to +64, r7 to r10 and two stack words (caller's +84, +92) to +36 to +56 (probably text ids); +0 = the count, +4, +8, +16, +20, +24, +28 = 0; the old flags block (+68), when set, freed (`VirtualFree(block, 0, MEM_RELEASE)`, `sub_82295330`); a new one of count × 4 bytes (`VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE)`, `sub_82295398`) into +68, each word = 1 (probably each item enabled). About 75 callers, all front-end screens (`sub_82184338` to `sub_8228A560`); its neighbors `sub_8220BC20`, `sub_8220BD50`, `sub_8220BDE0`, `sub_8220C1E0` are the menu lists' lead. Read in full. |
| `sub_8209F5E0` | `Players::ResetControllers` | | Resets the player-to-controller table (`826BABA4`, 4 words) to 0-3 and asks for the profile read (`826C7938` = 1). From `sub_82106548` (boot) and `sub_82221D00` (`TitleScreen::ResetPlayer`). Moved from `functions.md` (2026-10-28, B7 loop); read in full: a leaf. |
| `sub_82114128` | `Progress::Restart` | | Not seen in play. Probably a new career that keeps what was earned: r3 = the save data; a 0x1E000-byte frame (`__chkstk`, `sub_825A9934`, `c-runtime.md`); keeps on the stack the 0x18134 bytes from +6120 (the garage, to +0x1991C), the 60 bytes at +0x19930 and the 28 at +0x199A8 and +0x199C4 (`memcpy`), +6116 (probably the money) and +0x19924 to +0x1992C; `Progress::Clear`, `Progress::SetDefaults`; +4 = 1; all of them copied back; then +0x1AD18 \|= 1, 2, 4, 8 (one at a time). Only caller the title menu's handler `sub_82221AC0` (its item 2 path, a lead). Read in full, its meaning not worked out. |
| `sub_82105740` | | | Not seen in play. Probably the two-player save data: 0x3AB74 bytes from `operator new` (`sub_825A6880`); when not null, `Stats::Stats` at +48 and +48 + 0x1D52C; the block (or 0) into `0x826C5B5C` (player 1's slot); `Progress::SetDefaults` on +0 and +0x1D52C, then `Options::SetDefaults(+0x3AA58)`. No course records or sound bank holder set up. Callers not read. Read in full. |
| `sub_82105688` | | | Not seen in play. Probably a teardown: `sub_82104F00`, `sub_82104F20`, `sub_820FF908` (not read), `SaveData::Delete(0)`, `sub_82116FB0` (not read). No arguments read. Callers not read. Read in full. |
| `sub_82105BF8` | `CourseRecords::Of` | | Not seen in play. r3 = the course records (save data + 0x1D52C), r4 = a pair of words {course, traffic}: returns r3 + 20 + 408 × (course × 2 + traffic), the 22 records `CourseRecords::Reset` sets up. Only caller `Race::PlaceRacers` (`race-setup.md`, its race data +0xA3AF4 0 or 4 path). Leaf. Read in full. |
