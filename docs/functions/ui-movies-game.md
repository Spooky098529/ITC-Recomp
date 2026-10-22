# Known functions: UI movies, the game's side

The game's side of the UI movies (`UiMovie::`, about `82119B80` to
`8211E808` in `default.xex`), over the movie library in
[`ui-movies.md`](ui-movies.md) (mostly native now, under
`--native_ui_movies`: read its intro and rows first). A movie object
(about 41 KB, the table of them by id at `0x826C5CCC`) holds three
copies of its movie file (the scene that's animated and drawn, the pose
saved between updates, and the file's own pose for resets), its textures,
its text list, the draw context, the named animations, two animation
lists (a queue played in chained groups, and the looping animations),
and its texts (UTF-16 buffers by id, filled from the game's string
table). The game reaches movies from about 630 call sites through
`sub_8211E1B8` (the update, `sub_8211D2F8`) and the node lookups and
setters below, from the HUD, the race start countdown, the menus and the
pause screens. Its layout, the slots' and the texts' are in
`../known-data.md` ("UI movie" and the rows after it). Mapped by the B7
loop (`../loops/b7-loop.md`), 2026-10-22.

Overrides kept from earlier work (all must be kept when these become
native): the update `sub_8211D2F8` and the rest pose `sub_82119B80` stay
in `ui-movies.md` (shared: the update replayed on render-only passes,
`--per_tick_ui_movies`, with its adding tracks per tick,
`--per_tick_ui_movie_tracks`; see "Per-frame logic" in
`../frame-interpolation.md` and "The sub-menus' arrow" in
`../pause-menu-fix.md`). Here, `sub_8211CD90`, `sub_8211B4D0` and
`sub_8211B080` are watched for writes for `--log_pause_movie`
(`WATCH_MOVIE_CALL` in `itc_recomp/src/debug/pause_movie_log.cpp`), and
`sub_8211C7E8`, `sub_8211D978`, `sub_8211DAC8`, `sub_8211DDD8`,
`sub_8211DE38` and `sub_8211DF20` are logged there (`debug::LogCall`) for
`--log_pause_movie`. The rival logo's draw (`sub_82211A28`, Wrapped for
`--redraw_rival_logo` in `itc_recomp/src/frame_pacing/per_frame_logic.cpp`)
finds its movie with `sub_8211B3F0`, updates it through `sub_8211E1B8` and
finishes it with `sub_8211DF20` (see "The rival logo at the race start" in
`../frame-interpolation.md`).

Made native by the B7 loop (2026-10-22), bottom up, under
`--native_ui_movies_game` (on by default; off runs the originals): plain
C++ in `itc_recomp/src/ui_movies/game.cpp`, adapters in
`itc_recomp/src/hooks/ui_movies_game.cpp`, checked by
`--run_harnesses=ui_movies_game` (`src/harnesses/ui_movies_game.cpp`:
made-up movies in a 64 KB block from the system heap, as movies span more
than the scratch area). Native code calls the library's native functions
(`ui_movies::Child`, `ui_movies::RestorePose`) directly; the pose resets
fall back to the originals where `sub_82119B80` wouldn't run natively
(`--native_ui_movies` off, or `--log_pause_movie` watching it). An attract
race calls few of these (the movie loads' layers and text ids, the pose
reset, a node shown); the menus' setters rely on the harness.

Outside this file, these block it (leads in `../loops/leads.md`): the C
runtime heap (`sub_825A6880`, `sub_825A6940`, `sub_825A70E8`,
`sub_825A71C0`), the game's VirtualAlloc and VirtualFree wrappers
(`sub_82295398`, `sub_82295330`: the movie files and the texts' buffers),
`std::string` (`sub_82091328`, `sub_82091720`, `sub_820A6F60`,
`sub_825929E8`, `sub_82593040`: the named animations), the string table
and the font (`sub_8210A5E8`, `sub_8210A758`, `sub_8210A990` and others),
the matrix and vector math (`sub_820AB560`, `sub_820A0AC0`), and the
movie's texture set (`sub_82596E68`, `sub_82596F68`, `sub_82597118`,
`sub_822A5B78`). `sub_82119B38` in this range belongs to the sound side
(a lead). `sub_8211E808`, which sets up a movie as `sub_82247AA0` and
`sub_822912A0` do (`sub_8211E1C0`, `sub_8211A9E8`, `sub_8211E798`), never
ran in play and has no row.

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
| `sub_8211E1B8` | `UiMovie::Run` | | A jump to the update `sub_8211D2F8` (in [`ui-movies.md`](ui-movies.md), shared): r3 = movie, r4 = 0 advance and draw, 1 draw only; returns 1 while an animation plays or is queued, else 0. 628 call sites in `generated/` (122 callers seen in play: the HUD, the menus, the pause screens, the rival logo's `sub_82211A28` from `0x82211A74`). No override: `--log_pause_movie` names its caller from lr. |
| `sub_8211E1C0` | `UiMovie::UiMovie` | | r3 = a new movie: the textures at +28 (`sub_8211AE18`), the text list at +40 (`sub_8211B018`, count +48 = 0), the draw context at +64 (`sub_8259D930`, in `ui-movies.md`), the 1,024 named animations from +176 (`sub_8211D210` each; count at +0x90B0 = 0), the texts' 128 pairs at +0x9BCC cleared (count +0x9FCC = 0). Returns r3. From `sub_82247AA0`, `sub_822912A0` (and `sub_8211E808`, never in play), which also call `sub_8211A9E8` and `sub_8211E798`. |
| `sub_8211A9E8` | | | r3 = a movie: clears +0..+24 and the loaded flag (+0x9FD0), and sets up its two animation lists (at +0x90B4, the queue, and +0x9640, the looping animations): 32 links each (+0 the next, +2 the slot), the free list from 0 (+128), none in use (+132 = -1, +136 the count = 0; +130, +134 = -1). From the same callers as `sub_8211E1C0`. |
| `sub_8211E798` | `UiMovie::Load` | | r3 = movie, r4 = an asset id: allocates the asset's size (`sub_820A0678(id)`, `sub_82295398`) and requests it (`Archive::Load`, `sub_8209DE48`, in `files-and-saves.md`) with `sub_8211E500` as the callback and the movie as its context. From the same callers as `sub_8211E1C0`. |
| `sub_8211E500` | `UiMovie::OnLoaded` | | `Archive::Load`'s callback (no direct callers; `0x8211E500` passed by `sub_8211E798`): r4 = the loaded `XMD` archive, r6 = the movie. Checks it (`sub_825A0CF8`), finds the `X2D` file (entry 0, `sub_825A0CD0`) and its size (up to its `TRM` chunk, `sub_8259D5C8`), allocates three copies (`sub_82295398`) at +0, +4, +8, fixes each up (`sub_825A0EF0`) and keeps their scenes (`sub_825A0EB0`) at +16 (live), +20 (saved) and +24 (the file's); assigns layers in each (`sub_8211BD10`), clears the live texts' ids (`sub_8211A6C0`) and the texts (`sub_8211AC90`), and builds the named animations (+0x90B0 = 0, `sub_8211E378`). When the archive has more entries (`sub_822B80D8` reads its +12, the count), entry 1 is a texture set: a 24-byte object at +12 (`sub_82596E68`, `sub_82596F68`) and, for each texture, a 12-byte entry (+0 the texture `sub_82597118`; +4, +8 the width and height from `sub_822A5B78`) pushed on +28 (`sub_8211AE68`). Frees the archive (`sub_82295330`) and sets +0x9FD0 = 1. Runs as each movie loads. |
| `sub_8211E378` | | | r3 = movie, r4 = a scene: recurses into its `SCE` children (`sub_8259D658`), then for each animation (`sub_8259D670`, [scene + 120]) appends an entry to the movie's named animations (36 bytes from +176, count at +0x90B0): a `std::string` "scene name.animation name" (the scene's +84, the string at `0x82001DE8`, the animation's +16; `sub_825929E8`, `sub_820A6F60`, `sub_82593040`), the scene and the animation (copied with `sub_8211E318`; temporaries freed with `sub_82091328`). No bound check against the 1,024 entries. From `sub_8211E500`. |
| `sub_8211E318` | | | r3, r4 = named animation entries: copies the string (`sub_82091720`, all of it) and +28, +32. Returns r3. From `sub_8211E378`. |
| `sub_8211D210` | | | r3 = a named animation entry (36 bytes): an empty `std::string` (`sub_825929E8`, `sub_82593040`, the temporary freed with `sub_82091328`), +28 and +32 = 0. From `sub_8211E1C0`, 1,024 times a movie. |
| `sub_8211D1B8` | | | r3 = movie + 176: destroys the 1,024 named animations' strings (`sub_82091328(entry, 1, 0)`), last first. From `sub_8211E160`. |
| `sub_8211BD10` | | | r3 = movie, r4 = the scene, r5 = a group (or the scene), r6 = a layer, r7 = 1 to number by bucket: sorts the group's children ([r5 + 100] on, [r5 + 104] of them, last first) into 64 buckets by their layer offset (+72, at most 63), then by bucket: `GRP` recurses (r7 = 0), `SCE` recurses into it as the scene, `PIC`, `PRM` and `TXT` get their layer from `sub_8211A660` (the bucket's number when r7 is set, else r6). A 16.8 KB stack frame. From `sub_8211E500` (three times a load) and itself. |
| `sub_8211A660` | | Overridden | r4 = a node, r5 = a number: sets the node's +72 to the game's layer: (n + 62) * 2 up to 147 for n at most 11, (n + 291) * 2 up to 667 for 12 to 43, else 667. Leaf (r3 not read). From `sub_8211BD10`. The 147 cap is never reached ((11 + 62) * 2 = 146). Native under `--native_ui_movies_game` (`ui_movies::SetLayer`, `Layer`). Called ~4,000 times in an attract race's movie loads (from `0x8211BE48`). |
| `sub_8211A6C0` | | Overridden | r3 = movie: walks the chunks after the live scene ([+16], each +4 the offset to the next) up to `TRM`, setting each `TXT` node's +108 (its text's id) to 0. Leaf. From `sub_8211E500`. The live scene's own chunk isn't tested. Native under `--native_ui_movies_game` (`ui_movies::ClearTextIds`). Called once a movie load in an attract race. |
| `sub_8211AC90` | | | r3 = movie: frees each text ([+0x9FCC] of them, the values from +0x9BD0): its buffer (`sub_82295330(buffer, 0, 0x8000)`, probably VirtualFree with MEM_RELEASE) and the 28-byte object (`sub_825A70E8`); then the count = 0 (the keys aren't cleared). From `sub_8211E500` and `sub_8211B540`. |
| `sub_8211B540` | `UiMovie::Unload` | | r3 = movie: frees the three file copies (`sub_82295330`), clears the scenes (+16, +20, +24), pops and deletes the textures (+28: each 12-byte entry cleared, `sub_825A70E8`) and the queued text items (+40: each copied out, `sub_8211AED0`, and destroyed, `sub_8211A8C8`), releases the texture set (+12, through `sub_82108860`, not read) and clears it, the loaded flag (+0x9FD0) and the texts (`sub_8211AC90`). From `sub_8211E258`. |
| `sub_8211E160` | `UiMovie::~UiMovie` | | r3 = movie: the named animations (`sub_8211D1B8` on +176), the draw context (`sub_82106540`, an empty function, on +64), the text items (`sub_8211AF90([+40], 3)`) and the textures' array (`sub_825A71C0([+28])`). From `sub_8211E258`. |
| `sub_8211E258` | `UiMovies::Delete` | | r3 = a movie id: finds it in the table at `0x826C5CCC` (`sub_8211B3F0`); when there, unloads (`sub_8211B540`), destroys (`sub_8211E160`) and frees it (`sub_825A70E8`), and sets the table's entry to 0 (`sub_8211C168`). 65 callers in `generated/` (25 seen in play: the screens' clean-ups). |
| `sub_8211E2C8` | `UiMovies::DeleteAll` | | Deletes every movie in the table at `0x826C5CCC` (`sub_8211E258` by each entry's id). From `sub_8211E370`. |
| `sub_8211E370` | | | A jump to `sub_8211E2C8`. From `sub_82196EC0`, `sub_82211B98`, `sub_82224AB8`, `sub_8222FB90`, `sub_8225D590` (seen in play). |
| `sub_8211AD00` | | | Probably a screen's start: clears `0x826C5CB0`, sets the UI movie library's renderer and command list (`sub_8259D798`, in `ui-movies.md`) from the object at `[0x826BABA0]` (vtable slot 8, then slot 2 for the renderer and slot 1 for the list: four indirect calls), marks every entry of the movie table at `0x826C5CCC` free (id -1, movie 0; nothing is freed), and clears `0x826C5CA4`, `0x826C5CA8`, `0x826C5CAC`, `0x826C5CC0` and `0x826C5CB4`..`0x826C5CBC`. From `sub_82197278`, `sub_82211DA0`, `sub_82224C70`, `sub_8222FD68`, `sub_8225D6D0`. |
| `sub_8211B0E8` | | | r3 = a vector of 8-byte pairs: capacity 64 (+4), 512 bytes allocated (`sub_825A6940`) and cleared at +0. From the static set-up `sub_825D9678`, for the movie table at `0x826C5CCC` (which then sets its count, +8, to 0 and registers its clean-up with `atexit`). |
| `sub_8211B3F0` | `UiMovies::Find` | | r3 = a vector of {key, value} pairs (+0 the data, +8 the count), r4 = the key's address: returns the value's address, or `0x826C5CDC` when not found. Leaf. Mostly on the movie table at `0x826C5CCC` (a movie by id): 1,256 call sites in `generated/`, 213 callers seen in play (every screen, the HUD, the rival logo). |
| `sub_8211BE98` | `UiMovies::IsLoaded` | | r3 = a movie id: the movie's loaded flag (+0x9FD0) is set (`sub_8211B3F0` on `0x826C5CCC`; no check that the movie exists). 19 callers seen in play. |
| `sub_8211C168` | | | r3 = a vector of pairs, r4 = the key's address, r5 = the value's address: sets the value when the key is there, else appends the pair (`sub_8211B458`). From `sub_8211E258` (and `sub_8211E808`, never in play). |
| `sub_8211B458` | | | r3 = a vector of 8-byte pairs (+0, +4 the capacity, +8 the count), r4 = a pair: appends it, doubling the capacity first when full (`sub_8211B160`). From `sub_8211C168`. |
| `sub_8211B160` | | | r3 = a vector of 8-byte pairs, r4 = the new capacity: when it differs, allocates it (`sub_825A6940`, -1 bytes on overflow), cleared, copies the pairs that fit and frees the old array (`sub_825A71C0`). From `sub_8211B458`. |
| `sub_8211AE18` | | | r3 = a vector of words: capacity 512 (+4), 2 KB allocated (`sub_825A6940`), count 0 (+8). Returns r3. The movie's textures; from `sub_8211E1C0`. |
| `sub_8211AE68` | | | r3 = a vector of words, r4 = a word's address: appends it, doubling the capacity first when full (`sub_8211A788`). From `sub_8211E500`. |
| `sub_8211A788` | | | r3 = a vector of words, r4 = the new capacity: when it differs, allocates it (`sub_825A6940`), copies the words that fit and frees the old array (`sub_825A71C0`). From `sub_8211AE68`. |
| `sub_8211B018` | | | r3 = a text list: capacity 512 (+4), an array of 512 128-byte text items allocated (`sub_825A6940`, 65,552 bytes, the count 512 in its first 16 bytes) and each made (`sub_8211A818`). Returns r3 (the count, +8, is set by the caller). From `sub_8211E1C0`. |
| `sub_8211AF90` | | | r3 = text items, r4 = flags: the text item's deleting destructor. With bit 1, an array (its count at r3 - 16): each destroyed last first (`sub_8211A8C8`), and with bit 0 the block freed (`sub_825A71C0`); without, one item, freed with `sub_825A70E8`. Returns the block. From `sub_8211E160` (r4 = 3). |
| `sub_8211A818` | `TextItem::TextItem` | | r3 = a 128-byte text item: +0 = 0, an identity matrix at +16 (built on the stack and copied with `memcpy`; the stores of the float at `0x82000A30` before it are overwritten), +80..+96 = the float at `0x82000A34` (0.0). Returns r3. From `sub_8211B018` and `sub_8259F770` (in `ui-movies.md`). |
| `sub_8211A8C8` | `TextItem::~TextItem` | | r3 = a text item: +80 = 0.0, +0 = 0 and the identity matrix at +16 (with `memcpy`). From `sub_8211AF90`, `sub_8211B540`, `sub_8211CD90` and `sub_8259F770`. |
| `sub_8211AED0` | | | r3 = destination, r4 = a text item: copies +0, the matrix at +16 (`memcpy`; the diagonal stores before it are overwritten), +80 and the 16 bytes at +84 and +100. Returns r3. The same as `sub_8259E620` (in `ui-movies.md`). From `sub_8211B540` and `sub_8211CD90`. |
| `sub_8211A948` | `Quad::Quad` | | r3 = a quad or primitive for the library's recorders (built on the caller's stack): an identity matrix at +0 (copied with `memcpy`), +64 and +68 (the blend mode) = 0, +72 = 2. Returns r3. From `sub_8259ED40`, `sub_8259F348` (in `ui-movies.md`) and `sub_8211BEE8`. |
| `sub_8211A578` | `UiMovie::FindNode` | Overridden | r3 = movie, r4 = a sub-scene's index in the live scene or -1, r5 = a child's index: returns the child (`sub_8259D658` on the live scene [+16], then on that sub-scene when r4 isn't -1). 517 call sites, 103 callers seen in play (the menus). Native under `--native_ui_movies_game` (`ui_movies::FindNode` in `src/ui_movies/game.cpp`; adapter in `src/hooks/ui_movies_game.cpp`). Not called in an attract race. |
| `sub_8211AC00` | `UiMovie::ShowNode` | Overridden | The same lookup as `sub_8211A578`, then sets the node's +80 (visible) to 1. 20 callers seen in play. Returns the node. Native under `--native_ui_movies_game` (adapter only: `ui_movies::FindNode`, then the store). Called in an attract race (from `0x822479A4`, `0x822479B4`). |
| `sub_8211AC48` | `UiMovie::HideNode` | Overridden | The same, setting +80 to 0. 22 callers seen in play. Returns the node. Native under `--native_ui_movies_game` (as `sub_8211AC00`). Not called in an attract race. |
| `sub_8211A5B8` | `UiMovie::SetNodeColor` | Overridden | The same lookup, then copies color r6 of the table at `0x82008410` (12 bytes each: r, g, b as integers) to the node's +56..+64. 9 callers seen in play. Returns the node. Native under `--native_ui_movies_game` (`ui_movies::FindNode` and `SetColor`). Not called in an attract race. |
| `sub_8211A630` | | Overridden | r4 = a node, r5 = a color: the same copy as `sub_8211A5B8` without the lookup. Leaf. From `sub_821D8978`, `sub_821E3AD0` (seen in play). r3 is left as it was. Native under `--native_ui_movies_game` (`ui_movies::SetColor`). Not called in an attract race. |
| `sub_8211A708` | `UiMovie::SetNodeTexture` | Overridden | The same lookup, then r6 to a primitive's +104 or a picture's +108 (their texture entry); other tags are left as they were. 6 callers seen in play. Returns the node. Native under `--native_ui_movies_game` (`ui_movies::SetNodeTexture`). Not called in an attract race. |
| `sub_8211AAA8` | `UiMovie::SetPictureTexture` | Overridden | The same lookup (a picture), then replaces the movie's texture its entry names (`sub_8259D648(picture, [+108])`, its word +0 indexing the array at [movie + 28]) with r6; returns the old one. 6 callers seen in play. (The original reads the texture entry twice; nothing is written between.) Native under `--native_ui_movies_game` (`ui_movies::PictureTextureSlot`). Not called in an attract race. |
| `sub_8211AB28` | `UiMovie::GetPictureTexture` | Overridden | The same lookup, then returns that texture. From `sub_82229238`. Native under `--native_ui_movies_game` (`ui_movies::PictureTextureSlot`). Not called in an attract race. |
| `sub_8211AB90` | `UiMovie::SwapTexture` | Overridden | r3 = movie, r4 = i, r5 = a texture: stores it as texture i ([movie + 28]) and returns the old one. Leaf. From `sub_82211980`, `sub_82211AB0`, `sub_822135B0`, `sub_822141A0` (the rival logo's and others). Native under `--native_ui_movies_game` (`ui_movies::SwapTexture`). Not called in an attract race. |
| `sub_8211ABA8` | `UiMovie::ResetPose` | Overridden | r3 = movie: the file's pose (+24) copied over the live (+16) and the saved (+20) scenes (`sub_82119B80`, r6 = r7 = 0). 13 callers seen in play. Returns the second `sub_82119B80`'s r3. Native under `--native_ui_movies_game` (`ui_movies::ResetPose`, calling `ui_movies::RestorePose` directly) where `sub_82119B80` would run natively (`--native_ui_movies`, no `--log_pause_movie`), else the original. Called in an attract race (from `0x82222AB8`, `0x8224790C`). |
| `sub_8211A1E0` | `UiMovie::ResetNode` | Overridden | r3 = movie, r4 = a sub-scene's index or -1, r5 = a child's index: copies that node's fields (+12..+80) from the file's scene (+24) to the live (+16) and saved (+20) ones, then its subtree (`sub_82119B80`) for each. From `sub_8222DC08`. The +36 and +68 floats go through registers (`lfs`, `stfs`), which leave the words as they were (the harness's odd floats); the scenes' pointers are read again after the copies. Returns the second `sub_82119B80`'s r3. Native as `sub_8211ABA8` (`ui_movies::ResetNode`). Not called in an attract race. |
| `sub_8211B230` | | | r3 = an animation list, r4 = a position: takes a slot off the free list (+128) and links it in at that position (+132 the first; 16-bit links, 4 bytes each: +0 the next, +2 the slot), count (+136) + 1. No check for a full list. Leaf. From `sub_8211B4D0`. |
| `sub_8211B4D0` | `AnimationList::Insert` | Wrapped | The UI movie update's callees besides its tracks (and `sub_82119B80`, in [`ui-movies.md`](ui-movies.md)), watched for writes for `--log_pause_movie` (`WATCH_MOVIE_CALL`). r3 = an animation list, r4 = a 40-byte slot, r5 = a position: links a free slot in at that position (`sub_8211B230`) and copies r4 into it (`memcpy`, at list + 140 + 40 * slot). Queues animations (`sub_8211D978`, `sub_8211DAC8`), moves them between the lists (`sub_8211DBE0`, `sub_8211DCE8`, `sub_8211DFF8`) and, in the update, onto its list of finished ones. |
| `sub_8211B080` | `AnimationList::RemoveAt` | Wrapped | The UI movie update's callees besides its tracks (and `sub_82119B80`, in [`ui-movies.md`](ui-movies.md)), watched for writes for `--log_pause_movie` (`WATCH_MOVIE_CALL`). r3 = an animation list, r4 = a position: unlinks that slot and puts it on the free list, count - 1. Leaf. From the update and `sub_8211DBE0`, `sub_8211DCE8`, `sub_8211DE38`, `sub_8211DF20`. |
| `sub_8211B298` | `AnimationList::Get` | | r3 = a 40-byte destination, r4 = an animation list, r5 = a position: copies that slot (`memcpy`). Returns r3. From `sub_8211DBE0`, `sub_8211DCE8`. |
| `sub_8211D978` | `UiMovie::Play` | Wrapped | The pause sub-menus' calls on their UI movie (setting animation targets and the children's +80 flags, starting animations, switching sub-screens); logged for `--log_pause_movie` (`src/debug/pause_movie_log.cpp`). r3 = movie, r4 = a sub-scene's index or -1, r5 = an animation's index in it (`sub_8259D670`), r6..r9 = the slot's +24 (chained), +28 (reversed), +32, +36 (paused): unless the looping list (+0x9640) already has it (slot +4 = r4, +0 = r5), queues it at the end of the queue (+0x90B4, `sub_8211B4D0`), its time 0 (the animation's length when reversed), and copies the slot to +0x9FD8 (the last started). 117 call sites, 61 callers seen in play. |
| `sub_8211DAC8` | `UiMovie::PlayNamed` | Wrapped | The pause sub-menus' calls on their UI movie (setting animation targets and the children's +80 flags, starting animations, switching sub-screens); logged for `--log_pause_movie` (`src/debug/pause_movie_log.cpp`). The same as `sub_8211D978` for named animation r4 (index into +176; nothing when negative; the slot's +12, +16 from the entry's +28, +32; +0 = +4 = -1, +8 = r4), r5..r8 the slot's +24..+36. 612 call sites, 69 callers seen in play (among them the pause sub-menus' arrow, r4 = 20 down and 19 up). |
| `sub_8211DBE0` | `UiMovie::MoveAnimation` | | r3 = movie, r4 = a sub-scene's index or -1, r5 = an animation's index, r6: with 1, finds it in the queue and moves it to the end of the looping list; with 0, the other way (`sub_8211B298`, `sub_8211B080`, `sub_8211B4D0`). From `sub_8211DDD8`, `sub_8211DDE8`, `sub_8211DE38`. |
| `sub_8211DCE8` | `UiMovie::MoveNamedAnimation` | | The same as `sub_8211DBE0` for named animation r4 (slot +8), r5 the direction. From `sub_8211DDE0`, `sub_8211DDE8`, `sub_8211DF20`. |
| `sub_8211DDD8` | `UiMovie::Loop` | Wrapped | The pause sub-menus' calls on their UI movie (setting animation targets and the children's +80 flags, starting animations, switching sub-screens); logged for `--log_pause_movie` (`src/debug/pause_movie_log.cpp`). `sub_8211DBE0` with r6 = 1: makes a queued animation loop. 27 callers seen in play. |
| `sub_8211DDE0` | `UiMovie::LoopNamed` | | `sub_8211DCE8` with r5 = 1. 29 callers seen in play. |
| `sub_8211DDE8` | `UiMovie::LoopLast` | | r3 = movie: makes the last started animation (the slot at +0x9FD8) loop, by name when its +8 isn't -1 (`sub_8211DCE8`), else by its +4 and +0 (`sub_8211DBE0`). 21 callers seen in play. |
| `sub_8211DE38` | `UiMovie::Finish` | Wrapped | The pause sub-menus' calls on their UI movie (setting animation targets and the children's +80 flags, starting animations, switching sub-screens); logged for `--log_pause_movie` (`src/debug/pause_movie_log.cpp`). r3 = movie, r4 = a sub-scene's index or -1, r5 = an animation's index: moves it from the looping list to the queue (`sub_8211DBE0`, r6 = 0), then finds it in the queue, applies it at its end (`sub_825A08E0` at 0 when reversed, else its length) and removes it (`sub_8211B080`): the live scene keeps its end pose. 36 callers seen in play. |
| `sub_8211DF20` | `UiMovie::FinishNamed` | Wrapped | The pause sub-menus' calls on their UI movie (setting animation targets and the children's +80 flags, starting animations, switching sub-screens); logged for `--log_pause_movie` (`src/debug/pause_movie_log.cpp`). The same as `sub_8211DE38` for named animation r4 (`sub_8211DCE8`, r5 = 0). 480 call sites, 49 callers seen in play; the rival logo's `sub_82211A28` calls it when its movie has ended. |
| `sub_8211DFF8` | `UiMovie::FinishAll` | | r3 = movie: moves every looping animation to the queue, then applies each queued one at its end and removes it (as `sub_8211DE38`). 26 callers seen in play. |
| `sub_8211BA00` | `UiMovie::AnimationTime` | | r3 = movie, r4 = a sub-scene's index or -1, r5 = an animation's index: the time (+20) of its slot, looked for in the looping list, then the queue; the float at `0x82001BE0` when neither has it. Leaf. 5 callers seen in play. |
| `sub_8211BB18` | `UiMovie::NamedAnimationTime` | | The same for named animation r4 (slot +8). Leaf. From `sub_821D8978`. |
| `sub_8211B310` | `TextMap::Find` | | r3 = a movie's texts (+0x9BCC: 128 {id, text} pairs, the count at +1024), r4 = an id's address: returns the text's address, or `0x826C5CD8` when not found. Leaf. From `sub_8211B660` and the text getters. |
| `sub_8211B370` | `TextMap::Set` | | r3 = the texts, r4 = an id's address, r5 = a text's address: sets it when the id is there, else appends the pair at the count; no check against the 128 entries. Leaf. From the text setters and `sub_8211B940`. |
| `sub_8211B660` | `UiMovie::GetText` | | r3 = movie, r4 = the address of a `TXT` node's +108 (its text's id): gives the node an id when it has none ([movie + 0x9FCC] + 1), and returns its 28-byte text (`sub_8211B310`), making one when there's none (`sub_825A6880`; a 4 KB buffer from `sub_82295398(0, 4096, 4096, 4)`, probably VirtualAlloc with MEM_COMMIT and PAGE_READWRITE, cleared with `memset`; +16 = 0.0, the rest 0); the caller stores it (`sub_8211B370`). From the text setters. |
| `sub_8211B720` | `UiMovie::TextString` | | r3 = movie, r4 = a text id: the text's buffer, or the empty string at `0x82008434` when it has none. From `sub_8211CD90`, `sub_8211B800`. |
| `sub_8211B770` | | | r3 = movie, r4 = a text id: the text's +20 (characters shown), or 0. From `sub_8211CD90`. |
| `sub_8211B7B8` | | | r3 = movie, r4 = a text id: the text's +24, or 0. Passed to the formatting (`sub_8210A5E8`); not set in this file. From `sub_8211CD90`. |
| `sub_8211B9B8` | | | r3 = movie, r4 = a text id: the text's +4 (scrolling), or 0. From `sub_8211CD90`. |
| `sub_8211B940` | | | r3 = movie, r4 = a text id: adds one to the text's +8 (the scroll counter) and stores it again (`sub_8211B370`). From `sub_8211CD90` when it advances. |
| `sub_8211B800` | `UiMovie::ScrollOffset` | | r3 = movie, r4 = a text's draw parameters (+0 x, +8, +28, +68, +120 the text id): probably a scrolling text's x: the text's width from the font (`sub_82108B30([0x826C5C00], params, string)`), the counter (+8) against the delay (+12): before it, x as it was; after it, x minus the ticks past it times the speed (+16), wrapped around by the width. Not read in detail. From `sub_8211CD90`. |
| `sub_8211C1F8` | `UiMovie::SetText` | | r3 = movie, r4 = a sub-scene's index or -1, r5 = a child's index, r6 = a string id (16-bit), r7..r10 its arguments (varargs): the node must be a `TXT` (else it loops forever: an assert); clears its text (`sub_8211B660`, `memset`), +20 = 0, formats the string into it (`sub_8210A990([0x826C5C34], buffer, id, args)`, the string table) and stores it. 260 call sites, 48 callers seen in play. |
| `sub_8211C7E8` | `UiMovie::SetNodeText` | Wrapped | The pause sub-menus' calls on their UI movie (setting animation targets and the children's +80 flags, starting animations, switching sub-screens); logged for `--log_pause_movie` (`src/debug/pause_movie_log.cpp`). The same as `sub_8211C1F8` with the node given (r4) instead of looked up: r5 = the string id, r6.. its arguments. 112 call sites, 18 callers seen in play, and `sub_8211CAA0`. |
| `sub_8211CBD8` | | | r3 = movie, r4 = a `TXT` node, r5 = a string id, r6 = one argument: the same without the tag check or clearing +20. 7 callers seen in play. |
| `sub_8211C740` | | | r3 = movie, r4/r5 the lookup, r6: clears the text and formats string 8 (r6 = 0) or 9 with r6 into it (`sub_8210ABC0`, not read). 8 callers seen in play. |
| `sub_8211CA18` | | | The same as `sub_8211C740` with the node given (r4), r5 the value. From `sub_821821C8`. |
| `sub_8211C470` | `UiMovie::ClearText` | | The lookup, then clears the text's buffer and its +8. 6 callers seen in play. |
| `sub_8211C4F0` | | | The lookup, then the text's +12 = r6 (the scroll delay) and +16 = f1 (the speed). From `sub_8222CFF0`. |
| `sub_8211C570` | `UiMovie::ScrollText` | | The lookup, then the text's +4 = r6 (scrolling), +12 = 45 and +16 = the float at `0x820021F8`. 9 callers seen in play. |
| `sub_8211C5F8` | | | The lookup, then the text's +8 (the scroll counter) = 0. 13 callers seen in play. |
| `sub_8211C930` | | | r3 = movie, r4 = a `TXT` node: probably reveals one more character: when the text's +20 + 1 is at most what `sub_8210A5C0([0x826C5C34], buffer)` returns (probably its length), +20 + 1 and returns 1, else 0. From `sub_8211D880`. |
| `sub_8211C9C0` | | | The same test without the step: 1 while there's more to show. From `sub_8211D8D8`. |
| `sub_8211D880` | `UiMovie::RevealText` | | The lookup, then `sub_8211C930`. From `sub_82229798`, `sub_82229888`, `sub_82229CF0`, `sub_82229E98`. |
| `sub_8211D8D8` | | | The lookup, then `sub_8211C9C0`. 9 callers seen in play. |
| `sub_8211CAA0` | | | r3 = movie, r4 = a `TXT` node, r5 = a record (+4 a kind, +8 a 16-bit index, +10 a flag; +12, +16, +18 a second part): probably names a control or binding: kind 5 sets string 5837 + index (`sub_8211C7E8`); else two string ids from the kinds (0: 5477 or 5657 + index, 1: 5908 +, 4: 5463 +; second, 2: 5567 or 5747 +, 3: 5912 +; each range-checked) formatted into string 532. Not read in detail. From `sub_8211D930`. |
| `sub_8211D930` | | | The lookup, then `sub_8211CAA0`. 8 callers seen in play. |
| `sub_8211CD90` | `UiMovie::DrawTexts` | Wrapped | The UI movie update's callees besides its tracks (and `sub_82119B80`, in [`ui-movies.md`](ui-movies.md)), watched for writes for `--log_pause_movie` (`WATCH_MOVIE_CALL`). r3 = movie, r4 = the text list (movie + 40), r5 = 1 to draw without advancing: pops each queued text item (last first; `sub_8211AED0`, `sub_8211A8C8`), transforms the origin by its matrix (`sub_820AB560`), writes the matrix's scale and the item's clip rectangle into the `TXT` node (+28, +32, +40..+52), and builds the text's draw parameters on its stack: position, color (the node's +56..+64 times the item's color and the float at `0x82002200`), alpha, the alignment from the node's +76 bits, the layer (+72), the text id (+108). A scrolling text (`sub_8211B9B8`) gets its x from `sub_8211B800` and, when r5 is 0, its counter stepped (`sub_8211B940`). Then formats the text (`sub_8210A5E8` with `sub_8211B720`, `sub_8211B770`, `sub_8211B7B8`) and draws it (`sub_8210A758`, not read). Runs every movie update (r5 = 1 from the draw-only update, 0 otherwise), from `sub_8211D2F8` and `sub_8211D280`. |
| `sub_8211D280` | `UiMovie::Draw` | | r3 = movie: draws it without applying animations: `sub_82119B80` (the live scene's pose saved into +20), `sub_8259FA00`, `sub_8211CD90` (r5 = 0) and `sub_8259DAC0` with 678 and 148, as the update's drawing. From `sub_822141E8`. |
| `sub_8211BEE8` | | | r3 = a primitive node: probably draws a full-screen effect over a frame's texture: a quad (`sub_8211A948`, blend mode 4) with the primitive's vertices plus its position, through a scale matrix (the float at `0x82001070`; `sub_820AB560`), textured with what `sub_8209F7A8(0)` returns (its size from `sub_822A5B78`), the node's color and alpha, recorded with `sub_8259EBE8` (key 597). Not read in detail. From `sub_822128A8`. |
