# Known functions: the file layer

The game's file layer above the Win32-style file calls: the file
objects the game makes on the stack around a whole-file load (the debug
font, the archives' TOCs, `TXR_XACT.xgs`, the audio banks, the loader
thread's loose files), their class chain, the path objects that turn a
name into a full path with sprintf, and the game's path buffer the
movie class used. Worked on as B7 in `docs/rexglue-removal.md`; B4 made
the file calls under it native at the kernel boundary (`src/hooks/file_class.cpp`,
`src/game_data/`: nothing the game opens goes through the SDK's VFS).

The file object (16 bytes, `../known-data.md`): +0 the vtable, +4 the
handle (-1 when closed), +8 the position, +12 the size. Its classes, from
the game's down: `0x82002270` (the game's file, `GameFile::`, made by
`sub_820A6A00`), over `0x8206CC0C` (probably an overlapped, unbuffered
file, `AsyncFile::`: its own open, `sub_82593B10`), over `0x8206C05C`
(`File::`, the class whose open, read, seek, close and write B4 made
native), over `0x8206BFF8` (probably an abstract stream, written only by
the destructors). The vtables' slots weren't read (no image dump): the
functions without direct callers here are probably their slots.

The path object (`Path::`, vtable `0x8206CC3C`): +0 the vtable, +4 a
character buffer (probably 512 bytes; the open by name keeps it at
r1 + 80 in a 640-byte frame). `Path::Build` (`sub_82593CA0`) writes the
full path into it with sprintf by a kind: 0 with the format at
`0x8206CC48`, 1 with `0x8206CC54`, 2 with `0x8206CC60` and the two global
strings `0x8265EE7C` and `0x8265EE98` (`XENON_PRJ\TSP`; probably a
development path), 3 and up nothing. The format strings weren't read
(no image dump); in play every open by name passes kind 0, and the
paths the file class gets are `game:\` ones (`src/hooks/file_class.cpp`).
The game's path buffer (`GamePath::`, vtable `0x820022A0`: +0 the
vtable, +4 the text) copies a built path out of a `Path` on its own
stack.

Rows elsewhere, shared with this file (they stay there):
`files-and-saves.md` holds the open for read by name (`sub_820A6AB8`:
r3 = the file object, r4 = the name, r5 = a pointer to a byte its
callers set to 0 or 1, passed on to `File::OpenRead`, which doesn't read
it; a `Path` on its stack built with kind 0, then `File::OpenRead` on its
text; returns 1 when the open did, else 0), the `File::` class
(`sub_8258C608` OpenWrite, `sub_8258C688` OpenRead, `sub_8258C718` Close,
`sub_8258C768` Seek, `sub_8258C820` Read, `sub_8258C898` Write;
OpenRead, Close, Seek and Read Overridden since B4 step 2),
`File::GetSize` (`sub_822B80D8`), `AsyncFile`'s open (`sub_82593B10`) and the file
objects' users: the debug font (`sub_82090CC0`), the loose read
(`sub_8209DFA8`), `Archive::Init` (`sub_8209E210`, Overridden: its file
reads are ours and no longer make the object) and `TXR_XACT.xgs`'s open
(`sub_82118638`); `sound-cues.md` holds `BankPair::Load` (`sub_821178C8`,
the object made and released around its `Archive::Load` calls, never
opened there); `movies.md` holds `Movie::Open` (`sub_820AE5D0`, the
path buffer's only user, no longer called with
`--play_movies_with_ffmpeg`); `c-runtime.md` holds `sprintf`
(`sub_825A93D0`), on `_output_l` (`sub_825AF130`) and `_vsnprintf`
(`sub_825AB010`), all recompiled.

What's ready (`b7_groups.py`, 2026-10-28): the file object's constructor
and destructor with their bases (`sub_820A6A00` with `sub_82593AC0` and
`sub_8258C938`; `sub_820A6A40` with `sub_82593B00` and `sub_8258C5F8`),
the path object's constructor and destructor (`sub_82593C28`,
`sub_82593C48`) and the path buffer's (`sub_820A6BE0`). Those are native since the B7 loop (2026-10-28, `src/file_layer/`, `--native_file_layer`, harness `file_layer`). Then (the same run) sprintf with `_output_l`, as swprintf's `_woutput_l` (a step-by-step translation over guest arguments, `src/c_runtime/narrow_format.cpp`, `c-runtime.md`), and with it `Path::Build`, `GamePath::SetGame`, the open by name and the loose read; the formats in play are `"game:\%s"` (kind 0) and, elsewhere, `"%s%s"`, `"%s%s%d"` and `"data\audio\%s.xgs"` (logged 2026-10-28). Left recompiled: the file objects' other users (the debug font, `BankPair::Load`, `TXR_XACT.xgs`'s open, `Movie::Open`). The other opens by name, the deleting
destructors and the vtable-only methods never ran in play.

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
| `sub_820A6A00` | `GameFile::GameFile` | Overridden | r3 = a 16-byte file object (on the caller's stack): `AsyncFile::AsyncFile` (`sub_82593AC0`), then the vtable `0x82002270`. Returns r3. From the debug font (`sub_82090CC0`), the loose read (`sub_8209DFA8`), `BankPair::Load` (`sub_821178C8`, `sound-cues.md`), `TXR_XACT.xgs`'s open (`sub_82118638`) and `Archive::Init`'s original (`sub_8209E210`, twice; its override no longer makes it). Read in full. Native in `src/file_layer/file_layer.cpp` (adapter `src/hooks/file_layer.cpp`, `--native_file_layer`; harness `file_layer`). |
| `sub_820A6A40` | `GameFile::~GameFile` | Overridden | r3 = the file object: the vtable `0x82002270` written again, then `AsyncFile::~AsyncFile` (`sub_82593B00`). The handle isn't closed (every caller closes it first, `File::Close`). The same callers as `GameFile::GameFile`. Read in full. Native in `src/file_layer/file_layer.cpp` (adapter `src/hooks/file_layer.cpp`, `--native_file_layer`; harness `file_layer`). |
| `sub_820A6B88` | `GameFile` deleting destructor | | r3 = the object, r4 = flags: `~GameFile`'s body (the vtable `0x82002270`, then `sub_82593B00`), freed with `sub_825A70E8` (operator delete, `heap.md`) when bit 0 of r4 is set; returns r3. No direct callers (probably the vtable's slot 0). Read in full. Not seen in play. |
| `sub_820A6A50` | `GameFile::OpenWrite` (by name) | | As the open for read by name (`sub_820A6AB8`, `files-and-saves.md`): r3 = the file object, r4 = the name, r5 passed on; a `Path` on the stack (`sub_82593C28`) built with kind 0 (`sub_82593CA0`), `File::OpenWrite` (`sub_8258C608`) on its text, the `Path` released (`sub_82593C48`); 1 when the open did, else 0. No direct callers (probably a vtable slot of `0x82002270`). Read in full. Not seen in play. |
| `sub_820A6B20` | `GameFile::OpenAsync` (by name) | | As `sub_820A6A50`, with `AsyncFile`'s open (`sub_82593B10`, `files-and-saves.md`). No direct callers (probably a vtable slot). Read in full. Not seen in play. |
| `sub_82593AC0` | `AsyncFile::AsyncFile` | Native | Probably (the name from its open's flags). r3 = the object: `File::File` (`sub_8258C938`), then the vtable `0x8206CC0C`. Returns r3. Only from `GameFile::GameFile`. Read in full. Native in `src/file_layer/file_layer.cpp`, reached from `GameFile::GameFile`'s plain C++ (its adapter removed: `--native_file_layer` now switches only the callers; harness `file_layer`). |
| `sub_82593B00` | `AsyncFile::~AsyncFile` | Native | r3 = the object: the vtable `0x8206CC0C`, then `File::~File` (`sub_8258C5F8`). From `GameFile::~GameFile` and `GameFile`'s deleting destructor. Read in full. Native in `src/file_layer/file_layer.cpp`, reached from `GameFile::~GameFile`'s plain C++ (its adapter removed: `--native_file_layer` now switches only the callers; harness `file_layer`). |
| `sub_82593BC8` | `AsyncFile` deleting destructor | | r3 = the object, r4 = flags: the vtable `0x8206CC0C`, `File::~File` (`sub_8258C5F8`), freed with `sub_825A70E8` when bit 0 of r4 is set; returns r3. No direct callers (probably the vtable's slot 0). Read in full. Not seen in play. |
| `sub_82593BB0` | `AsyncFile::Detach` | | Probably. r3 = the object: returns the handle (+4) and sets +4 = -1, without closing it. No direct callers (probably a vtable slot of `0x8206CC0C`). Read in full. Not seen in play. |
| `sub_8258C938` | `File::File` | Native | r3 = the object: the vtable `0x8206C05C`, +4 = -1 (closed), +8 = 0 (the position), +12 = 0 (the size). Returns r3. Only from `AsyncFile::AsyncFile`. Read in full (named in `command-list.md`'s intro as probably this, B7 loop 2026-10-22). Native in `src/file_layer/file_layer.cpp`, reached from `AsyncFile::AsyncFile`'s plain C++ (its adapter removed: `--native_file_layer` now switches only the callers; harness `file_layer`). |
| `sub_8258C5F8` | `File::~File` | Native | r3 = the object: the vtable `0x8206BFF8` (probably the base stream's, inlined; `File`'s own destructor has nothing else to do and doesn't close the handle). From `AsyncFile::~AsyncFile` and `AsyncFile`'s deleting destructor. Read in full. Native in `src/file_layer/file_layer.cpp`, reached from `AsyncFile::~AsyncFile`'s plain C++ (its adapter removed: `--native_file_layer` now switches only the callers; harness `file_layer`). |
| `sub_8258C5B0` | stream deleting destructor | | Probably the base class's (vtable `0x8206BFF8`): r3 = the object, r4 = flags: the vtable written, freed with `sub_825A70E8` when bit 0 of r4 is set; returns r3. No direct callers. Read in full. Not seen in play. |
| `sub_82593C28` | `Path::Path` | Native | r3 = a path object (on the caller's stack): the vtable `0x8206CC3C`, the text's first byte (+4) = 0. From the opens by name (`sub_820A6A50`, `sub_820A6AB8`, `sub_820A6B20`) and `GamePath::SetGame`, `Set` (`sub_820A6BF0`, `sub_820A6C68`). Read in full. Native in `src/file_layer/file_layer.cpp` (harness `file_layer`). Its adapter removed (B7 loop, 2026-10-28; a temporary counter showed it uncalled in an attract race): reached from the open by name's adapter and `GamePath::SetGame`'s plain C++, so `--native_file_layer` now switches only those. |
| `sub_82593C48` | `Path::~Path` | Native | r3 = the path object: the vtable `0x8206CC3C` written again, nothing else. The same callers as `Path::Path`, at their ends. Read in full. Native in `src/file_layer/file_layer.cpp` (harness `file_layer`). Its adapter removed (B7 loop, 2026-10-28; a temporary counter showed it uncalled in an attract race): reached from the open by name's adapter and `GamePath::SetGame`'s plain C++, so `--native_file_layer` now switches only those. |
| `sub_82593C58` | `Path` deleting destructor | | r3 = the object, r4 = flags: the vtable `0x8206CC3C`, freed with `sub_825A70E8` when bit 0 of r4 is set; returns r3. No direct callers (probably the vtable's slot 0). Read in full. Not seen in play. |
| `sub_82593C40` | `Path::Text` | | Probably. r3 = the path object: returns r3 + 4 (the text). No direct callers (probably a vtable slot of `0x8206CC3C`). Read in full. Not seen in play. |
| `sub_82593CA0` | `Path::Build` | Native | r3 = the path object, r4 = the kind, r5 = the name. 0 when r5 is 0; else sprintf (`sub_825A93D0`, `c-runtime.md`) into +4: kind 0 with the format at `0x8206CC48` and the name, 1 with `0x8206CC54` and the name, 2 with `0x8206CC60`, the global strings' texts `[0x8265EE7C]` and `[0x8265EE98]` (`XENON_PRJ\TSP`; the in-place text or the heap block by the capacity at +24, `std-string.md`) and the name; 3 and up writes nothing. Returns r3 + 4 (the text). The formats weren't read (no image dump); in play only kind 0 (the opens by name and `GamePath::SetGame`), and the paths the file class gets in play are `game:\` ones. From the opens by name and `GamePath::SetGame`, `Set`. Read in full. The formats in play: kind 0's `"game:\%s"` (logged 2026-10-28). Ours (B7 loop, 2026-10-28): `file_layer::PathBuild` (`src/file_layer/file_layer.cpp`) over the native sprintf (`c_runtime::Sprintf`, sprintf's arguments and frames where the original's would be) where the native sprintf declines, its callers run their originals. Harness `file_layer`: 600 cases (kinds 0 to 4, names of 0 to 300 bytes with `%`, `\` and bytes above 127, at odd addresses, null), the scratch area and r3 the same. Its adapter removed (B7 loop, 2026-10-28; a temporary counter showed it uncalled in an attract race): reached from the open by name's adapter and `GamePath::SetGame`'s plain C++, so `--native_file_layer` now switches only those. |
| `sub_820A6BE0` | `GamePath::GamePath` | Overridden | Also its destructor (the same code: `Movie::Open` calls it at its start and its end). r3 = a path buffer (on the caller's stack): the vtable `0x820022A0`. Only from `Movie::Open` (`sub_820AE5D0`, `movies.md`, twice), no longer called with `--play_movies_with_ffmpeg`. Read in full. Native in `src/file_layer/file_layer.cpp` (adapter `src/hooks/file_layer.cpp`, `--native_file_layer`; harness `file_layer`). |
| `sub_820A6CE0` | `GamePath` deleting destructor | | r3 = the object, r4 = flags: the vtable `0x820022A0`, freed with `sub_825A70E8` when bit 0 of r4 is set; returns r3. No direct callers (probably the vtable's slot 0). Read in full. Not seen in play. |
| `sub_820A6BF0` | `GamePath::SetGame` | Overridden | Probably (kind 0, the game's own paths). r3 = the path buffer, r4 = a name: a `Path` on the stack (`sub_82593C28`) built with kind 0 (`sub_82593CA0`), its text copied into r3 + 4 (a byte loop, up to its 0), the `Path` released (`sub_82593C48`); returns r3 + 4. Only from `Movie::Open` (the name at its +48), no longer called with `--play_movies_with_ffmpeg`. Read in full. Ours (B7 loop, 2026-10-28): `file_layer::GamePathSetGame` (`src/file_layer/file_layer.cpp`, with `PathConstruct`, `PathBuild` and `PathDestruct`) behind the adapter in `src/hooks/file_layer.cpp` (`--native_file_layer`); where Path::Build's declines, the original runs. Harness `file_layer`: 573 cases (as Path::Build's with kind 0; not a null name, whose copy reads address 0), the scratch area and r3 the same. |
| `sub_820A6C68` | `GamePath::Set` (kind 1) | | As `GamePath::SetGame`, with kind 1 (the format at `0x8206CC54`). No direct callers (probably a vtable slot of `0x820022A0`). Read in full. Not seen in play. |
