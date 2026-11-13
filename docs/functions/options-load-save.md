# Known functions: the options' load and save pages

The options screen's two save-side categories
([`options-screen.md`](options-screen.md), which has the screen, its
blocks and the rest of its pages and steps): category 5 (page 9) is the
**load** of the game data (its menu answer calls `GameData::Load`, and a
good load is copied into player 0's save data and `Settings::Reload`
runs) and category 6 (page 10) the **save** (its menu answer calls
`GameData::Save`). The names given before this map (`Options::SavePage`,
`SetUpSavePage`, `ShowSaveDialog`, `ChangeSaveDialog` in
`options-screen.md` and `front-end-screen-helpers.md`) are category 5's,
the load's. Each page runs a sub-page from a table of its own built on
its frame (`Options::SavePage` `sub_821C6F58`: 7 entries, indexed by
`[0x826D847C]` +0; `Options::Category6Page` `sub_821BD228`: 6, by
`[0x826D8488]` +0; both blocks laid out as the options' page block, +0
the sub-page, +8 the one before, +12 and +16 -1 here). The sub-pages are
a small dialog machine over the save layer
([`save-interface.md`](save-interface.md): `SaveSlots::State`,
`Device`, `RequestDevice`, `GameData::Check`, `Load`, `Save`, `Word992`,
`Request980`; `SaveBuffers::Destroy`), the same flow as the save
screen's pages 2, 7 and 8 ([`save-screen.md`](save-screen.md)): 0 the
device check, 1 the dialog's menu (a yes or no question), 2 a note (a
message, closed by accept or after 45 passes), 3 the wait (90 passes,
then `GameData::Check` or the load's or save's end), 4 the close (step
41 or 42), 5 the cancel's (`Options::SavePage` sets it when `o` +708 is
set: `o` +712 = 1 only), 6 (load only, never seen) the device's answer
after a failed load. A dialog is set by `LoadPage::SetDialog` and
`SavePage::SetDialog` below: the kind (`o` +628, page 10 +648: the text,
`Options::ShowSaveDialog` and `ShowCategory6Dialog` in
`front-end-screen-helpers.md`, shared), its next state (`o` +624,
+644: 0 the question, 1 the note, 2 the wait) and the sub-page (the
state + 1), through steps 44 then 43 (page 9) or 47 then 46 (page 10)
(`../known-data.md`'s settings block `[0x826D8478]`, `o` in the rows).
The kinds used, by where they're set: page 9 (the load) 0 the check's
wait, 1 the question (data found), 2 and 3 the set-up's notes (no
device, a device), 4 no user or device (`SaveSlots::State` 1), 5 the
note when no data was found or there's no room, 6 the question after item 1 of
another question (item 0 closes, item 1 checks again), 7 the question when no device is chosen, 8 the loading wait, 9 the note
after a good load, 10 and 11 the device's answer after a failed load
(not seen), 12 the note when the loaded data's halfword +6328 is 0
(probably not the game's data); page 10 (the save) 0 the check's wait,
1 the question (no data yet), 2 and 3 the set-up's notes, 4 no user or
device, 5 the question (data found: overwrite), 6 as the load's, 7 the note when there's no room or the save failed (`Word992` 3), 8 the saving wait, 9 the note
after a good save, 10 the note when the save failed (`Word992` 2), 11
the question when no device is chosen.
The texts by kind are in `ShowSaveDialog`'s and `ShowCategory6Dialog`'s
rows. Item 0 of the dialog's menu (`[0x826D8484]` +0) loads or saves on
the first questions, and closes on the questions about leaving (kinds
6 and 7 on page 9, 6 and 11 on page 10).

Shared with other files and named only here: the save layer
([`save-interface.md`](save-interface.md); `GameData::Check` in
[`files-and-saves.md`](files-and-saves.md)), `Menu::Step` and
`Menu::Pressed` ([`menu-helpers.md`](menu-helpers.md)),
`MenuSound::Play` ([`menu-sounds.md`](menu-sounds.md)), `SaveData::Of`
([`vehicle-accessors.md`](vehicle-accessors.md)), `memcpy`
([`c-runtime.md`](c-runtime.md)), the UI movie calls
([`ui-movies-game.md`](ui-movies-game.md)), and from
[`options-screen.md`](options-screen.md) `Options::SetUpCategoryPage`
and `Settings::Reload`. The save screens' other part in
`82217E20`-`8221E908` (`8221BE90` on, never seen in play) is kind 6's
screen after the race, in the in-race screens' lead
(`../loops/leads.md`).

Native (B7 loop, 2026-11-13): all 16 in `src/options_screen/load_save.cpp`
(named for what they are: `LoadPage::` for category 5, `SavePage::` for
category 6), adapters beside the options screen's in
`src/hooks/options_screen.cpp` behind their own flag,
`--native_options_load_save` (on by default; the options screen's dispatch,
`Options::SavePage` and `Category6Page`, still calls the sub-pages through
their tables, so those adapters stay). The save layer, `Menu::Step` and
`Pressed`, `MenuSound::Play`, the UI movie calls and `memcpy` (where its
plain C++ declines) are called through their overrides, `Settings::Reload`
and `Options::SetUpCategoryPage` by their plain C++; the three save-layer
wrappers never seen in play (`sub_8228FA20`, `sub_8228FA58`,
`sub_8228FD50`) are the originals. The two never seen in play,
`OptionsSavePages::Cancel` and `LoadPage::DeviceAnswer`, were ported with
their callers (the user's preference, 2026-11-12). Harness
`options_load_save` (in `src/harnesses/options_screen.cpp`, over the
options' world plus the save system's interface laid as the save screen's
harness lays it, the save buffers' header and its game data's buffer):
each case shapes from a generator seeded by the first stack words
(`CaseRandom`), as the cases' streams are one stream shifted and could
otherwise repeat a case's shaping after the world's varying use; half
the cases make the dialog menu's player accept so every kind is reached.

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
| `sub_821BB858` | `LoadPage::SetDialog` | Native | r3 = the kind, r4 = the next state (0 the question, 1 the note, 2 the wait). For r4 below 3: the options' page block p `[0x826D8470]`: +8 = +0 unless it's 9, +12 = step 44 (`Options::ChangeSaveDialog`), +16 = step 43 (`ShowSaveDialog`), +4 = 9, +0 = -1; page 9's block `[0x826D847C]`: +8 = +0 unless it's r4 + 1, +0 = r4 + 1, +12 = +16 = -1; `o` +624 = r4 (for r4 2 the page 9 block is written after p, for 0 and 1 before). Always `o` +628 = r3 (with r4 3 or more only that). Leaf. From `LoadPage::CheckDevice`, `LoadPage::Answer`, `LoadPage::Note`, `LoadPage::Wait` and the unseen `LoadPage::DeviceAnswer`. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` (`--native_options_load_save`); harness `options_load_save` (100 cases, its register steps in the harness file). Native since 2026-11-13: its adapter removed (its callers in play, the plain C++ of `LoadPage::CheckDevice`, `Answer`, `Note`, `Wait` and `DeviceAnswer`, call it directly; the flag now switches only them). |
| `sub_821BB708` | `SavePage::SetDialog` | Native | As `LoadPage::SetDialog` for page 10: p +4 = 10, steps 47 then 46 (`Options::ShowCategory6Dialog`), page 10's block `[0x826D8488]`, `o` +644 = r4, +648 = r3. Leaf. From `SavePage::CheckDevice`, `SavePage::Answer`, `SavePage::Note`, `SavePage::Wait`. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` (`--native_options_load_save`); harness `options_load_save` (100 cases, its register steps in the harness file). Native since 2026-11-13: its adapter removed (its callers in play, the plain C++ of `SavePage::CheckDevice`, `Answer`, `Note` and `Wait`, call it directly; the flag now switches only them). |
| `sub_821BBC10` | `LoadPage::CheckDevice` | Overridden | Page 9's sub-page 0 (r3 not read). Unless `SaveSlots::State(0)` is 4 (busy): `SaveSlots::Device(0)` nonzero → `LoadPage::SetDialog(0, 2)` (the check's wait); zero → `LoadPage::SetDialog(7, 0)` (the question with no device). A 96-byte frame. No direct callers: entry 0 of `Options::SavePage`'s table. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (150 cases). |
| `sub_821BC7B0` | `LoadPage::Answer` | Overridden | Page 9's sub-page 1, the dialog's question. `Menu::Step([0x826D8484])`; when it moved (+8 not 0): p +8 = p +0 unless 9, +16 = -1, +12 = step 45 (`Options::MoveSaveDialogCursor`, `options-screen.md`), +4 = 9, +0 = -1, `MenuSound::Play(4)`. Else on accept (`Menu::Pressed(menu, 159)`): `MenuSound::Play(0)`, then by the kind `o` +628 and the cursor c (menu +0, read after the step): 0 and 1: c 0 → `GameData::Load(0)`, `LoadPage::SetDialog(8, 2)`; else `LoadPage::SetDialog(6, 0)`; 4: c 0 → `SaveSlots::RequestDevice(0)` and sub-page 0; else `LoadPage::SetDialog(6, 0)`; 5 and 6: c 0 → sub-page 4 (the close); else `LoadPage::SetDialog(0, 2)`; 7: c 0 → sub-page 4; else `RequestDevice(0)` and sub-page 0; 2, 3 and above 7 nothing. Sub-pages set on `[0x826D847C]` (+8 = +0 unless the same, +0, +12 = +16 = -1). Else `Menu::Pressed(menu, 158)`, its answer unused. A 112-byte frame (r30, r31). No direct callers: entry 1 of the table. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (400 cases). |
| `sub_821BC940` | `LoadPage::Note` | Overridden | Page 9's sub-page 2, a note. `Menu::Step([0x826D8484])`; `o` +632 up by one; at 45 or more with the kind 3 (the set-up's note with a device): `RequestDevice(0)`, sub-page 0. Else on accept (159): `MenuSound::Play(0)`, by the kind (a 13-entry jump table, above 12 nothing): 3 as above; 4, 5, 10, 11 → `LoadPage::SetDialog(6, 0)`; 0 to 2, 6 to 9, 12 → sub-page 4. No back press read. A 112-byte frame (r30, r31). No direct callers: entry 2 of the table. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (300 cases). |
| `sub_821C2620` | `LoadPage::Wait` | Overridden | Page 9's sub-page 3. s = `SaveSlots::State(0)`; s 1 → `LoadPage::SetDialog(4, 0)`. Else `o` +632 up by one; from 90 on, by the kind `o` +628: 0 → `GameData::Check(0)`: 1 (found) → `LoadPage::SetDialog(1, 0)`, 0 and 2 → `LoadPage::SetDialog(5, 1)`, others nothing (checked again next pass); 1 to 8 with s not 8: s 2 → w = `Word992(0)` (`sub_8228FA20`), `Request980(0)` (`sub_8228FA58`), w 4 → sub-page 6; s 3 → `SaveData::Of(0)`, and when the loaded buffer's (`[0x828161F0]` +0) halfword +6328 isn't 0: `memcpy(SaveData::Of(0), buffer, 129,400)`, `LoadPage::SetDialog(9, 1)`, `Settings::Reload` (`sub_821C0FF0`); else `LoadPage::SetDialog(12, 1)`. A 96-byte frame (r31). No direct callers: entry 3 of the table. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (400 cases). |
| `sub_821B9FB0` | `LoadPage::Close` | Overridden | Page 9's sub-page 4: p +8 = p +0 unless -1, +16 = -1, +12 = step 41 (`LoadPage::Leave`), +4 = -1, +0 = -1; `o` +712 = 1. Leaf. No direct callers: entry 4 of the table. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (40 cases). |
| `sub_821B9FF8` | `OptionsSavePages::Cancel` | Overridden | Sub-page 5 of both tables (pages 9 and 10; `Options::SavePage` and `Category6Page` set it when `o` +708, the cancel, is set, with step 41 or 42 already set by them): `o` +712 = 1. Leaf. Not seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (20 cases). Ported with its callers though never seen in play (the user's preference, 2026-11-12: as little left recompiled as possible); reached only through its table entry, so its adapter stays. |
| `sub_821BBC68` | `LoadPage::DeviceAnswer` (probably) | Overridden | Page 9's sub-page 6 (set by `LoadPage::Wait` when `Word992` answered 4). By `sub_8228FD50(0)` (`save-interface.md`: the look-up's answer): 3 → `Request980(0)`, then `LoadPage::SetDialog(10, 1)`; 7 → `LoadPage::SetDialog(10, 1)`; 8 → `LoadPage::SetDialog(11, 1)`; others nothing. A 96-byte frame. No direct callers: entry 6 of the table. Not seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (150 cases). Ported with its callers though never seen in play (the user's preference, 2026-11-12: as little left recompiled as possible); reached only through its table entry, so its adapter stays. |
| `sub_821C6468` | `LoadPage::Leave` | Overridden | Step 41 (page 9's close), r3 = the started word (p +24). m = UI movie 7 (`UiMovies::Find`). First call: `UiMovie::FinishNamed(m, 841)`, `FinishNamed(m, 843 + the dialog menu's cursor)` (`[0x826D8484]` +0), `UiMovie::Play(m, -1, 10, 0, 0, 0, 0)`, `Options::SetUpCategoryPage`, the word = 1. Every call `UiMovie::Run(m, 0)`; when it answers 0: p +8 = p +0 unless 0, +16 = -1, +4 = 0, +12 = step 5, +0 = -1, +24 = 0 (the started word), then `SaveBuffers::Destroy`. Returns 0 (the step ended by its own writes). A 128-byte frame (r29 to r31). No direct callers: step 41 of `Options::Update`'s table. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (150 cases). |
| `sub_821BBCC0` | `SavePage::CheckDevice` | Overridden | Page 10's sub-page 0: as `LoadPage::CheckDevice` with `SavePage::SetDialog(0, 2)` and, with no device, `SavePage::SetDialog(11, 0)`. A 96-byte frame. No direct callers: entry 0 of `Options::Category6Page`'s table. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (150 cases). |
| `sub_821BCA60` | `SavePage::Answer` | Overridden | Page 10's sub-page 1, the question. `Menu::Step([0x826D8484])`; moved → step 48 (`Options::MoveCategory6DialogCursor`) with p +4 = 10 as `LoadPage::Answer`'s, `MenuSound::Play(4)`. Else on accept (159): c = the cursor, `MenuSound::Play(0)`, by the kind `o` +648: 0, 1 and 5: c 0 → `SavePage::SetDialog(8, 2)` then `GameData::Save`; else `SavePage::SetDialog(6, 0)`; 4: c 0 → sub-page 0 and `RequestDevice(0)`; else `SavePage::SetDialog(6, 0)`; 6: c 0 → sub-page 4; else `SavePage::SetDialog(0, 2)`; 11: c 0 → sub-page 4; else sub-page 0 and `RequestDevice(0)`; others nothing. Else `Menu::Pressed(menu, 158)`, its answer unused. Sub-pages on `[0x826D8488]`. A 96-byte frame (r31). No direct callers: entry 1 of the table. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (400 cases). |
| `sub_821BCC10` | `SavePage::Note` | Overridden | Page 10's sub-page 2: as `LoadPage::Note` with `o` +652 and +648: at 45 with the kind 3 `RequestDevice(0)` and sub-page 0; on accept, `MenuSound::Play(0)` and by the kind: 2 and 9 → sub-page 4; 3 as above; 7 and 10 → `SavePage::SetDialog(6, 0)`; others nothing. A 112-byte frame (r30, r31). No direct callers: entry 2 of the table. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (300 cases). |
| `sub_821BBD18` | `SavePage::Wait` | Overridden | Page 10's sub-page 3. s = `SaveSlots::State(0)`; s 1 → `SavePage::SetDialog(4, 0)`. Else `o` +652 up by one; from 90 on, by the kind `o` +648: 0 → `GameData::Check(0)`: 0 → `SavePage::SetDialog(1, 0)`, 1 → `(5, 0)`, 2 → `(7, 1)`, others nothing; 8 with s not 7: s 2 → by `Word992(0)`: 2 → `o` +648 = 10, +644 = 1, page 10's block's sub-page 2, p's step 47 then 46, page 10 (as `SavePage::SetDialog(10, 1)`, written inline in another order); 3 → `SavePage::SetDialog(7, 1)`; then `Request980(0)` either way; s 3 → the same inline for the kind 9 (the note after a good save). A 96-byte frame (r31). No direct callers: entry 3 of the table. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (400 cases). |
| `sub_821BA508` | `SavePage::Close` | Overridden | Page 10's sub-page 4: as `LoadPage::Close` with step 42 (`SavePage::Leave`). Leaf. No direct callers: entry 4 of the table. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (40 cases). |
| `sub_821C6550` | `SavePage::Leave` | Overridden | Step 42 (page 10's close): as `LoadPage::Leave` with `FinishNamed(m, 857)`, `FinishNamed(m, 859 + the cursor)` and `Play(m, -1, 11, ...)`. A 128-byte frame (r29 to r31). No direct callers: step 42 of `Options::Update`'s table. Seen in play. Read in full. Ours (B7 loop, 2026-11-13): plain C++ in `src/options_screen/load_save.cpp` behind its adapter in `src/hooks/options_screen.cpp` (`--native_options_load_save`, falling back to the original); harness `options_load_save` (150 cases). |
