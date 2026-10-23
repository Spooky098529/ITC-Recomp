# Known functions: std::string

The C++ library's narrow string (`std::basic_string<char>`, Dinkumware's,
as linked into the game; `82091200`-`82091910`, `820A6E60`-`820A6F60`,
`825929E8`-`82593248`), as far as it runs in play, and the library's
wildcard search and replace over it beside its constructors. Read in full
by the B7 loop's mapping agent, 2026-10-23, from the UI movies' lead.

- **The layout** (28 bytes, `../known-data.md`): +0 the allocator (empty:
  nothing reads or writes it), +4 the text in place (16 bytes) while the
  capacity (+24) is under 16, else +4 holds a pointer to a heap block of
  capacity + 1 bytes; +20 the length; +24 the capacity (15 in place). The
  text always ends with a 0 byte. Blocks come from operator new (through
  the allocator, `sub_820912B0`) and go back with operator delete (both
  in `heap.md`, shared; Native behind adapters since the C runtime heap's
  migration), and text is copied with `memcpy_s` (`sub_825A7270`,
  `c-runtime.md`, shared; Overridden).
- **Growth** (`_Grow`, `_Copy`): a new capacity of the length asked for
  with its low four bits set, or 1.5 times the old one when that's more,
  a new block of capacity + 1, the old text copied over and the old block
  freed.
- **Where it's used in play:** the UI movies' named animations (an entry
  per scene animation, "scene" + a one-character separator + "animation";
  `ui-movies-game.md`), the sound manager's and the bank pairs' names
  (`sound-cues.md`), the TGA reader's set-up and clean-up
  (`car-models.md`), the loader's loose read (the request's path through
  the wildcard replace; `files-and-saves.md`), two global strings set at
  start-up, and the C++ containers' "too long" errors (`std::map` and
  `std::list` instantiations across the game, a lead in
  `../loops/leads.md`), whose error paths, where the strings are made,
  never ran.
- **Never ran in play** (no rows): the error paths, `_Xlen` (`sub_825A6A20`,
  `length_error`), `_Xran` (`sub_825A6B18`, `out_of_range`) and
  `bad_alloc`'s copy (`sub_82091200`); `erase` (`sub_82091398`, only on a
  self-assign); `append` from another string (`sub_820A6E60`, only for
  text inside the string itself); `replace` (`sub_82592E18`, only when the
  wildcard replace finds a match) and `sub_82592A20`.

Status:

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
| `sub_825929E8` | `std::string::string` (empty) | | r3 = the string (r4, an allocator, not read): `_Tidy(r3, 0, 0)` (`sub_82091328`), so an empty string in place. Returns r3. From the named animations' set-up and append (`sub_8211D210`, `sub_8211E378`, `ui-movies-game.md`) and the loader's loose read (`sub_8209DFA8`, `files-and-saves.md`). |
| `sub_825931D0` | `std::string::string(const char*)` | | r3 = the string, r4 = a C string: `_Tidy(r3, 0, 0)` (`sub_82091328`), then `assign(r4, strlen(r4))` (`sub_82091808`; the length counted inline). Returns r3. 18 callers, 9 seen in play: `sub_82593248`, the sound manager's and the bank pairs' constructors (`sub_821197E0`, `sub_82117D08`, `sound-cues.md`, their names), and the containers' "too long" errors (`sub_82093400`, `sub_820958B8`, `sub_820AD768`, `sub_82131680`, `sub_82131FB0`, `sub_8258E2F8`: the call itself never ran there, only the functions). |
| `sub_82593248` | `std::string::string(const char*, const allocator&)` | | r3 = the string, r4 = a C string (r5, the allocator, not read): `sub_825931D0(r3, r4)`. Returns r3. Only from the loader's loose read (`sub_8209DFA8`, twice a read: the request's path, then the wildcard replace's result). |
| `sub_82593040` | `std::string::string(const string&, size_t off, size_t count)` | | r3 = the string, r4 = the source, r5 = the offset, r6 = the count (-1 for all; r7, the allocator, not read): `_Tidy(r3, 0, 0)` (`sub_82091328`), then `assign(r4, r5, r6)` (`sub_82091720`). Returns r3. From the named animations' set-up (`sub_8211D210`, the copy of an empty one) and append (`sub_8211E378`, offset 0 and count -1: a copy). |
| `sub_82091328` | `std::string::_Tidy` | | r3 = the string, r4 = built (a byte), r5 = the new length. When built and the capacity (+24) is 16 or more: the heap block's first r5 bytes copied into the string's own +4 (`memcpy_s(r3 + 4, 16, block, r5)`, `c-runtime.md`, skipped for 0) and the block freed (`sub_825A70E8`, operator delete, `heap.md`). Then +20 = r5, +24 = 15 and the byte at +4 + r5 = 0. With r4 = 1 and r5 = 0 it's the destructor; with r4 = 0 a constructor's start. 28 callers, 17 seen in play: the constructors here, `_Copy`, `_Grow`, the UI movies' named animations (`sub_8211D210`, `sub_8211E378`, `sub_8211D1B8`, the 1,024 entries' destructor), the bank pairs' destructor (`sub_82117B88`), the TGA reader's clean-up and set-up (`sub_820EEAE8`, `sub_820EEB58`, `car-models.md`), the loader's loose read (`sub_8209DFA8`, its temporaries) and the containers' "too long" errors (`sub_82093400`, `sub_820958B8`, `sub_820AD768`, `sub_82131680`, `sub_82131FB0`; never reached there). |
| `sub_820912B0` | `std::allocator<char>::allocate` | | r3 = the count: when the count isn't 0 and `0xFFFFFFFF / count` is under 1 (never for chars), a `std::bad_alloc` made on the stack (`sub_82091200`, the vtable `0x82000CCC` then `0x82000CC0`; never ran). Returns `operator new(count)` (`sub_825A6880`, `heap.md`; also for a count of 0). Only from `_Copy` (`sub_82091458`). |
| `sub_82091458` | `std::string::_Copy` | | r3 = the string, r4 = the new length, r5 = the length to keep. The new capacity is r4 with its low four bits set (r4 itself when that's over `0xFFFFFFFE`); when a third of it is under half the old capacity and the old capacity is at most `0xFFFFFFFE` - half of it, the old capacity × 1.5 (old + old / 2) instead. A block of capacity + 1 (`sub_820912B0`); r5 bytes of the old text copied into it (`memcpy_s(block, capacity + 1, old text, r5)`, skipped for 0); `_Tidy(r3, 1, 0)` (`sub_82091328`, the old block freed); +24 = the capacity, +4 = the block, +20 = r5, the byte at block + r5 = 0. No exception handling around the allocation (the library's `try` isn't there). Only from `_Grow` (`sub_82091540`). |
| `sub_82091540` | `std::string::_Grow` | | r3 = the string, r4 = the new length, r5 = trim (a byte). Over `0xFFFFFFFE`: `_Xlen` (`sub_825A6A20`, throws `length_error`; never ran). When the capacity (+24) is under r4: `_Copy(r3, r4, the length)` (`sub_82091458`); else with trim and r4 under 16, `_Tidy(r3, 1, min(r4, the length))` (`sub_82091328`); else when r4 is 0, the length = 0 and the first byte 0. Returns 1 when r4 isn't 0, else 0. 6 callers, 3 seen in play (`sub_82091720`, `sub_82091808`, `sub_820A6F60`, all with trim 0). |
| `sub_82091720` | `std::string::assign(const string&, size_t off, size_t count)` | | r3 = the string, r4 = the source, r5 = the offset, r6 = the count. When the source's length (+20) is under r5: `_Xran` (`sub_825A6B18`, throws `out_of_range`; never ran). The count cut to the length - r5. When r3 is r4: `erase(r5 + count)` then `erase(0, r5)` (`sub_82091398`; never ran in play). Else `_Grow(r3, count, 0)` (`sub_82091540`), and when it gives 1, the source's text from r5 copied in (`memcpy_s(text, the capacity, source + r5, count)`), +20 = count, the byte after it 0. Returns r3. 5 callers, 4 seen in play: `sub_82091808` (text inside the string itself; not seen taking that path), `sub_82593040`, the named animations' copy (`sub_8211E318`, `ui-movies-game.md`) and the loader's loose read (`sub_8209DFA8`, offset 0, count -1). |
| `sub_82091808` | `std::string::assign(const char*, size_t count)` | | r3 = the string, r4 = the text, r5 = the count. When r4 points inside the string's own text (from its start up to its end): `assign(r3, r3, r4 - text, r5)` (`sub_82091720`). Else `_Grow(r3, r5, 0)` (`sub_82091540`), and when it gives 1, the text copied in (`memcpy_s(text, the capacity, r4, r5)`), +20 = r5, the byte after it 0. Returns r3. 6 callers, 4 seen in play: `sub_825931D0`, the TGA reader's set-up (`sub_820EEB58`, `car-models.md`) and the two global strings' set-ups (`sub_825DA540`, `sub_825DA580`). |
| `sub_820A6F60` | `std::string::append(const char*, size_t count)` | | r3 = the string, r4 = the text, r5 = the count. When r4 points inside the string's own text: `append(r3, r3, r4 - text, r5)` (`sub_820A6E60`; never ran). Else when the length + r5 would pass `0xFFFFFFFF` (or wrap): `_Xlen` (`sub_825A6A20`; never ran). For a count of 0 nothing; else `_Grow(r3, length + r5, 0)` (`sub_82091540`), and when it gives 1 the text copied after the old text (`memcpy_s(text + length, capacity - length, r4, r5)`), +20 = the new length, the byte after it 0. Returns r3. Only from the named animations' append (`sub_8211E378`, three times an animation: the scene's name, the one-character separator at `0x82001DE8`, the animation's name). |
| `sub_82593088` | `String::ReplaceAll` (probably; the library's) | | r3 = a string, r4 = a pattern (C string, with the wildcards of `sub_82592740`), r5 = the replacement (C string), r6 = how many to replace (0 for all). Repeatedly finds the pattern from a position (`sub_82592970`, from 0) and replaces the match (`replace(pos, length, r5, strlen(r5))`, `sub_82592E18`), the search going on from the match's position + the replacement's length, until no match or r6 replacements. Returns the string's text (`c_str`). Only from the loader's loose read (`sub_8209DFA8`, once a read: the request's path, pattern `0x82001DE8`, replacement `0x82001DEC`, three characters at most, 4 bytes apart; their text not read). `replace` never ran in play: no request's path had a match. |
| `sub_82592970` | `String::Find` (probably; the library's) | | r3 = a string, r4 = a pattern, r5 = the match's position (in and out), r6 = the match's length (out), r7 = the position to start from. When the string isn't empty: `sub_82592740(text + r7, r4, r5, r6)`; when that finds a match, `*r5 += r7` (when r5 isn't null) and returns the match's address; else 0. Only from `sub_82593088`. |
| `sub_82592740` | `WildcardSearch` (probably) | | r3 = a text (C string), r4 = a pattern, r5 = the match's offset (out, may be null), r6 = its length (out, may be null). Finds the first match of the pattern in the text, trying each start: `?` (63) matches one character, `*` (42) any run (the `*`s and `?`s after it skipped first, then the next character looked for), the byte 2 anchors the match to the text's start and 3 to its end; other characters match themselves. Returns the match's start (and its offset and length through r5, r6), or 0. Not read in detail (the `*` backtracking). Only from `sub_82592970`. |
| `sub_825DA540` | | | Static set-up (in the start-up's initializer table, no direct callers): the global string at `0x8266EE7C` = 9 characters from `0x8206CC74` (`assign`, `sub_82091808`), its clean-up `0x825EB328` registered with `atexit` (`sub_825A7068`, a lead). The string's readers not looked for; its text not read. |
| `sub_825DA580` | | | Static set-up, as `sub_825DA540`: the global string at `0x8266EE98` = 13 characters from `0x8206CC80`, its clean-up `0x825EB380` registered with `atexit`. |
