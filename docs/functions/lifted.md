# Known functions: lifted

The functions the lift sweep (`../loops/lift-sweep.md`, 2026-11-15)
translated that had no row in another file. Their C++ was written by
`itc_recomp/tools/lift.py` from `generated/`, not read by hand: the rows
say only what the tool could tell (what each calls, whether it was seen
in play). Names are blank where nothing is known. A row moves to a
subsystem's file once someone works out what the function does.

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
- **Lifted**: translated from `generated/` by `itc_recomp/tools/lift.py`
  into plain C++ in `itc_recomp/src/lifted/` (the lift sweep,
  `docs/loops/lift-sweep.md`): typed parameters, registers as locals,
  calls direct or through the dispatch table. While the SDK or code not
  native yet still reaches it, its entry for them is an override in
  `src/lifted/` (deleted when nothing reaches it that way).

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_82090148` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82090190` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820901D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82090220` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82090268` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82090350` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820903D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820905E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls nothing. Not seen in play. Not read. |
| `sub_82090780` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls nothing. Not seen in play. Not read. |
| `sub_82090790` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_820905E0`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820907E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_820907E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_820907F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090778`. Not seen in play. Not read. |
| `sub_820907F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_82090800` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_82090808` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090778`. Not seen in play. Not read. |
| `sub_82090810` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_82090818` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_82090820` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_8248F030`. Not seen in play. Not read. |
| `sub_82090848` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_82090850` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_82090858` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82117770`. Not seen in play. Not read. |
| `sub_82090870` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_82090878` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_82090880` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_8248F030`. Not seen in play. Not read. |
| `sub_82090888` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090770`. Not seen in play. Not read. |
| `sub_82090898` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_820908A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_820908A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090780`. Not seen in play. Not read. |
| `sub_820908B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_8248F030`. Not seen in play. Not read. |
| `sub_820908B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090770`. Not seen in play. Not read. |
| `sub_820908C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_8248F030`. Not seen in play. Not read. |
| `sub_820908D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_820908F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_820908F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090790`. Not seen in play. Not read. |
| `sub_82090900` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090780`. Not seen in play. Not read. |
| `sub_82090910` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090380`, `sub_820903D0`, `sub_820A87A0`. Not seen in play. Not read. |
| `sub_82090D60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090D68`. Not seen in play. Not read. |
| `sub_82090D68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82090F88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_8258C3A8`, `sub_825A71B0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82091070` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_8258C060`, `sub_825A71C0`. Not seen in play. Not read. |
| `sub_82091110` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82091070`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82091200` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_825A69D8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82091260` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_825A69D8`. Not seen in play. Not read. |
| `sub_82091268` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820915F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82091328`. Not seen in play. Not read. |
| `sub_82091648` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls nothing. Not seen in play. Not read. |
| `sub_82091668` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_820915F8`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820916B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_820915F8`. Not seen in play. Not read. |
| `sub_820916C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_820915F8`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82091910` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82091328`, `sub_82091720`. Not seen in play. Not read. |
| `sub_82091968` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82091910`. Not seen in play. Not read. |
| `sub_820919B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82091910`. Not seen in play. Not read. |
| `sub_82091B98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090380`. Not seen in play. Not read. |
| `sub_82092650` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820926C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820927E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82092830` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82092A70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82092AC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82093638` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82093000`, `sub_82099B18`, `sub_82099CF8`. Not seen in play. Not read. |
| `sub_82093850` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82093638`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820941F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82090380`, `sub_82092A70`, `sub_82092AC0`, `sub_82093850`, `sub_82106718`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82094380` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82094398` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820943E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls nothing. Not seen in play. Not read. |
| `sub_820943F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls nothing. Not seen in play. Not read. |
| `sub_82094410` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls nothing. Not seen in play. Not read. |
| `sub_82094418` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls nothing. Not seen in play. Not read. |
| `sub_82094420` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82094470` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_820943F8`. Not seen in play. Not read. |
| `sub_82094478` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82094380`. Not seen in play. Not read. |
| `sub_82094488` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82094380`. Not seen in play. Not read. |
| `sub_82094490` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_82094368`. Not seen in play. Not read. |
| `sub_820944A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_820947E0`. Not seen in play. Not read. |
| `sub_820944B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_820947E0`. Not seen in play. Not read. |
| `sub_820944B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_820947E0`. Not seen in play. Not read. |
| `sub_820944C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_820947E0`. Not seen in play. Not read. |
| `sub_820944D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_820947E0`. Not seen in play. Not read. |
| `sub_820947E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_820941F0`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82094890` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_8209A930`. Not seen in play. Not read. |
| `sub_82094930` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_001.cpp`, `--native_lifted_001`). Calls `sub_8209A930`. Not seen in play. Not read. |
| `sub_820949D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_8209A930`. Not seen in play. Not read. |
| `sub_82094A70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_8209A930`. Not seen in play. Not read. |
| `sub_82094B68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82094BB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82094BE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82094B10`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82094C80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_8258E540`. Not seen in play. Not read. |
| `sub_82094CD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_8258E530`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82094D68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_820ACF88`, `sub_82106540`, `sub_8258DD60`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82094DE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82094D68`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82094E30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82098E80`, `sub_8258DD90`, `sub_8258E578`, `sub_825A6D28`. Not seen in play. Not read. |
| `sub_82094EE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_820AD058`, `sub_8258DCC8`, `sub_8258E548`. Not seen in play. Not read. |
| `sub_82094F30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82094EE0`, `sub_820AD058`, `sub_8258DCC8`, `sub_8258DE88`, `sub_8258DF18`, `sub_8258E548`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82095168` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls nothing. Not seen in play. Not read. |
| `sub_82095188` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_8258E578`, `sub_825A6D28`. Not seen in play. Not read. |
| `sub_820951D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls nothing. Not seen in play. Not read. |
| `sub_82095208` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82095FE0`. Not seen in play. Not read. |
| `sub_82095210` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82095FE0`. Not seen in play. Not read. |
| `sub_82095218` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls nothing. Not seen in play. Not read. |
| `sub_82095298` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82095218`, `sub_82098E80`, `sub_8258E580`, `sub_8258E5B8`. Not seen in play. Not read. |
| `sub_82095370` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82098E80`, `sub_820ACF88`. Not seen in play. Not read. |
| `sub_82095468` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82095298`, `sub_8258DD78`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820954F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82095370`, `sub_82098E80`, `sub_8258E580`, `sub_8258E5B8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82095678` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls nothing. Not seen in play. Not read. |
| `sub_820956F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82095678`, `sub_82098E80`, `sub_8258DD90`, `sub_8258E578`, `sub_825A6D28`. Not seen in play. Not read. |
| `sub_820957F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_820954F8`, `sub_82118F50`, `sub_8258DD78`, `sub_8258E578`, `sub_825A6D28`. Not seen in play. Not read. |
| `sub_820959D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82095958`, `sub_8258DD78`, `sub_8258E578`, `sub_825A6D28`. Not seen in play. Not read. |
| `sub_82095A28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82095958`, `sub_8258DD78`, `sub_8258E578`, `sub_825A6D28`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82095DF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82094B10`, `sub_82094D68`, `sub_82095958`, `sub_82095A28`, `sub_820ACF88`, `sub_82106540`, `sub_8258DD60`, `sub_8258DD78`, .... Not seen in play. Not read. |
| `sub_82095FE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82095DF8`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82098CE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82098F30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_820ACF88`, `sub_8258CEA0`, `sub_825918E0`, `sub_82591BB8`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82098FF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82098F30`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82099048` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82098D60`, `sub_82118790`, `sub_82592400`. Not seen in play. Not read. |
| `sub_820994E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82099528` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82294E20`. Not seen in play. Not read. |
| `sub_82099578` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls nothing. Not seen in play. Not read. |
| `sub_82099590` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82099710` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls nothing. Not seen in play. Not read. |
| `sub_82099740` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls nothing. Not seen in play. Not read. |
| `sub_82099780` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls nothing. Not seen in play. Not read. |
| `sub_820997B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls nothing. Not seen in play. Not read. |
| `sub_820997D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82099528`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82099A50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82099AD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82099BA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82099BA8`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82099CF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82099BA8`. Not seen in play. Not read. |
| `sub_8209A008` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82099D50`. Not seen in play. Not read. |
| `sub_8209A6E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_82093850`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8209AA58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_8209A6E0`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8209AB08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_8209A930`. Not seen in play. Not read. |
| `sub_8209ABA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_8209A930`. Not seen in play. Not read. |
| `sub_8209AC40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls nothing. Not seen in play. Not read. |
| `sub_8209AC60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls nothing. Not seen in play. Not read. |
| `sub_8209B2C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_822A67B0`, `sub_8258D8C8`, `sub_8258D908`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8209BCB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_8209B2C0`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8209C158` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_822A67B0`. Not seen in play. Not read. |
| `sub_8209F138` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_8210AFA0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8209F590` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls nothing. Not seen in play. Not read. |
| `sub_8209FB80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8209FBC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_820A2F38`. Not seen in play. Not read. |
| `sub_820A2F38` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_820A0C40`. Not seen in play. Not read. |
| `sub_820A44C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820A4508` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820A5660` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_820A4508`, `sub_825A70E8`, `sub_825A71C0`. Not seen in play. Not read. |
| `sub_820A6420` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls nothing. Not seen in play. Not read. |
| `sub_820A6470` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls nothing. Not seen in play. Not read. |
| `sub_820A6D28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_825A6B18`. Not seen in play. Not read. |
| `sub_820A6E00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_002.cpp`, `--native_lifted_002`). Calls `sub_820A6D28`. Not seen in play. Not read. |
| `sub_820A70A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A70B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820A70F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A6880`. Not seen in play. Not read. |
| `sub_820A7178` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A7180` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820A71C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A7180`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820A7608` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A7620` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A7628` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A7688` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A7AF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A6880`. Not seen in play. Not read. |
| `sub_820A7B70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A7B78` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820A7BC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A7BD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A7BE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A7C00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A7B78`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820A8050` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8068` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8088` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8098` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A80A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A80F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8108` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A81A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A81C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A70F8`, `sub_820A75E8`, `sub_820A7688`, `sub_820A7AF8`, `sub_825A6880`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A82E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A8340` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A83C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A8408` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A8420` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8438` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8460` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8490` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A84C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A84E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8508` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A8690` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A86B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A8708` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A87A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A87F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A8840` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A8850` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A8860` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A8870` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A87A0`, `sub_820A87F0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A88F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8910` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8928` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8940` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8948` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8950` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8960` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8978` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8990` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8998` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A89A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A89A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A89B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A89B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A89C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A89E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8A00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A0AC0`. Not seen in play. Not read. |
| `sub_820A8A08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A0AC0`. Not seen in play. Not read. |
| `sub_820A8A10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A0AC0`. Not seen in play. Not read. |
| `sub_820A8A18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8A30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8A48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_820A8A98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_820A8AE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_820A8B38` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A8870`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820A8EA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8EC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A8F20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A7348`. Not seen in play. Not read. |
| `sub_820A9010` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820A9078` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A8EC8`, `sub_820A8F20`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820A9200` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A9198`. Not seen in play. Not read. |
| `sub_820A9208` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A9010`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820A9270` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_82091200`, `sub_82091328`, `sub_820915F8`, `sub_820919B8`, `sub_825931D0`. Not seen in play. Not read. |
| `sub_820A92D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_82091200`, `sub_82091328`, `sub_820915F8`, `sub_820919B8`, `sub_825931D0`. Not seen in play. Not read. |
| `sub_820A9330` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A9270`. Not seen in play. Not read. |
| `sub_820A93E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_82092B78`, `sub_820A92D0`, `sub_825A70E8`, `sub_825A7348`. Not seen in play. Not read. |
| `sub_820A96D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A9330`. Not seen in play. Not read. |
| `sub_820A9738` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A93E8`. Not seen in play. Not read. |
| `sub_820A9818` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A9738`. Not seen in play. Not read. |
| `sub_820A9960` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_820A9970` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_820A9BD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820A9CF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_825A8770`, `sub_825A88A0`, `sub_825A8978`. Not seen in play. Not read. |
| `sub_820AA010` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820AA018` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820AA020` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820AA028` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820AA040` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820AA058` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820AA070` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls nothing. Not seen in play. Not read. |
| `sub_820AA088` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A6470`, `sub_820A9CF8`. Not seen in play. Not read. |
| `sub_820AA198` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_820A9CF8`. Not seen in play. Not read. |
| `sub_820AC950` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820ACCF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_003.cpp`, `--native_lifted_003`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820ACD40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820ACE80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_82090380`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820ACEC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_82090380`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820AD188` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820AD350`. Not seen in play. Not read. |
| `sub_820AD190` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820ACF88`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820AD228` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820AD228`. Not seen in play. Not read. |
| `sub_820AD2B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820AD300` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820AD350` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820AD190`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820AD5B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820AD3A0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820AD750` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820AD4E0`. Not seen in play. Not read. |
| `sub_820AD758` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820AD5B0`. Not seen in play. Not read. |
| `sub_820AD808` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820AD8D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820ACF88`, `sub_820AD3A0`, `sub_820AD808`. Not seen in play. Not read. |
| `sub_820ADA00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820AD8D0`. Not seen in play. Not read. |
| `sub_820ADA70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820ACFF8`, `sub_820AD768`. Not seen in play. Not read. |
| `sub_820ADB28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820ACFF8`, `sub_820AD768`. Not seen in play. Not read. |
| `sub_820ADB80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820ACFF8`, `sub_820AD768`. Not seen in play. Not read. |
| `sub_820AE198` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820AD228`. Not seen in play. Not read. |
| `sub_820AE2B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_82090380`. Not seen in play. Not read. |
| `sub_820AE2F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820AE2B0`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820AE9F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820AEBC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820AE9F8`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820B0098` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls nothing. Not seen in play. Not read. |
| `sub_820B00A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls nothing. Not seen in play. Not read. |
| `sub_820B00B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls nothing. Not seen in play. Not read. |
| `sub_820B0340` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls nothing. Not seen in play. Not read. |
| `sub_820B03E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls nothing. Not seen in play. Not read. |
| `sub_820B0428` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls nothing. Not seen in play. Not read. |
| `sub_820B6170` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_820B0668`, `sub_820B3E98`. Not seen in play. Not read. |
| `sub_820B95F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_82294170`, `sub_822942E0`. Not seen in play. Not read. |
| `sub_820B9650` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_82294320`. Not seen in play. Not read. |
| `sub_820B9680` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_82293590`, `sub_822942E0`, `sub_82294608`. Not seen in play. Not read. |
| `sub_820B97C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls nothing. Not seen in play. Not read. |
| `sub_820B97C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_822940E0`. Not seen in play. Not read. |
| `sub_820B97D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls nothing. Not seen in play. Not read. |
| `sub_820B97D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_822938F0`, `sub_822940E0`. Not seen in play. Not read. |
| `sub_820B98B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820B9950` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls nothing. Not seen in play. Not read. |
| `sub_820B9960` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls nothing. Not seen in play. Not read. |
| `sub_820B9970` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_82293F68`, `sub_822940E0`. Not seen in play. Not read. |
| `sub_820B9C30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls nothing. Not seen in play. Not read. |
| `sub_820B9FF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_822942A0`, `sub_822942E0`, `sub_82294430`, `sub_82294608`. Not seen in play. Not read. |
| `sub_820BA1D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_82294608`. Not seen in play. Not read. |
| `sub_820BA228` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_822940E0`, `sub_82294320`, `sub_82294470`. Not seen in play. Not read. |
| `sub_820BA688` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_822940E0`, `sub_82294270`, `sub_822942A0`, `sub_822942E0`, `sub_82294430`, `sub_82294608`. Not seen in play. Not read. |
| `sub_820BA888` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_822940E0`, `sub_822942A0`, `sub_822942E0`, `sub_82294430`, `sub_82294608`. Not seen in play. Not read. |
| `sub_820BAA48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls nothing. Not seen in play. Not read. |
| `sub_820BB270` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820B99D0`, `sub_820BA228`, `sub_82294430`. Not seen in play. Not read. |
| `sub_820BB410` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820BA688`. Not seen in play. Not read. |
| `sub_820BB450` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820BA688`, `sub_82293F68`, `sub_82293FD0`, `sub_822940E0`. Not seen in play. Not read. |
| `sub_820BC198` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_822940E0`. Not seen in play. Not read. |
| `sub_820BC4D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820BC198`. Not seen in play. Not read. |
| `sub_820BC560` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820B97C0`, `sub_820BA368`, `sub_820BAB40`, `sub_820BC198`, `sub_822940E0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820BC828` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820BC560`. Not seen in play. Not read. |
| `sub_820BD740` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820911F8`, `sub_820B97C0`, `sub_820BC908`, `sub_82294170`, `sub_82294270`, `sub_82294298`, `sub_822942A0`, `sub_822942E0`, .... Not seen in play. Not read. |
| `sub_820BD8F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820B97C0`, `sub_820BAB40`, `sub_820BC908`, `sub_82293590`, `sub_822940E0`, `sub_82294170`, `sub_822942A0`, `sub_822942E0`, .... Not seen in play. Not read. |
| `sub_820BDA48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820BC908`, `sub_822940E0`, `sub_82294170`, `sub_82294270`, `sub_82294298`, `sub_822942A0`, `sub_822942E0`, `sub_82294320`, .... Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820BDE40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820BC908`, `sub_82294270`, `sub_822942A0`, `sub_82294320`, `sub_82294430`, `sub_82294608`, `sub_822BE2C8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820BE0D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls nothing. Not seen in play. Not read. |
| `sub_820BE4F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820911F8`, `sub_820BC908`, `sub_82294568`, `sub_822945F8`, `sub_82294608`, `sub_822BE2C8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820BE860` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820BE490`, `sub_820BE4F0`. Not seen in play. Not read. |
| `sub_820BE8A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820911F8`, `sub_820B97C0`, `sub_820BAB40`, `sub_820BC908`, `sub_82294568`, `sub_822945F8`, `sub_82294608`, `sub_822BE2C8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820BEBB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820BE490`, `sub_820BE8A8`. Not seen in play. Not read. |
| `sub_820BEC50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820BDA48`, `sub_820BDE40`, `sub_820BEBF0`. Not seen in play. Not read. |
| `sub_820BF690` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820B6890`, `sub_820B97C0`, `sub_820B9C30`, `sub_820B9EE8`, `sub_820BA1D0`, `sub_820BA3B0`, `sub_820BA688`, `sub_820BAB40`, .... Not seen in play. Not read. |
| `sub_820BFBA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820BF690`. Not seen in play. Not read. |
| `sub_820BFBB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820BF690`. Not seen in play. Not read. |
| `sub_820CE600` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_820CE1D8`. Not seen in play. Not read. |
| `sub_820CE7F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820CE600`. Not seen in play. Not read. |
| `sub_820CEC20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_82594F58`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_820D8D20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_004.cpp`, `--native_lifted_004`). Calls `sub_820D0758`, `sub_82596920`, `sub_82596F68`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_820F1098` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_8209DE48`, `sub_820FB998`. Not seen in play. Not read. |
| `sub_820F1100` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_820FB998`, `sub_820FB9A8`. Not seen in play. Not read. |
| `sub_820F6C88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_820F6A38`. Not seen in play. Not read. |
| `sub_820F7D70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls nothing. Not seen in play. Not read. |
| `sub_820F7E30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_820F6C88`, `sub_820F7D70`. Not seen in play. Not read. |
| `sub_820F7EE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_820F6BE8`, `sub_820F7D70`. Not seen in play. Not read. |
| `sub_820F8FA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls nothing. Not seen in play. Not read. |
| `sub_820F9DD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_820AB498`, `sub_820F9768`. Not seen in play. Not read. |
| `sub_820FA8F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_820F8FA0`, `sub_820F9DD0`. Not seen in play. Not read. |
| `sub_820FB2B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_820F8EF0`, `sub_820FB1B8`. Not seen in play. Not read. |
| `sub_820FB9C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls nothing. Not seen in play. Not read. |
| `sub_820FBB68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_8209DE48`, `sub_820A0678`. Not seen in play. Not read. |
| `sub_820FF4D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_820FF458`, `sub_820FF790`. Not seen in play. Not read. |
| `sub_820FF5A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_825A71D0`. Not seen in play. Not read. |
| `sub_820FF628` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_820FF458`, `sub_820FF4D0`, `sub_820FF790`. Not seen in play. Not read. |
| `sub_820FF680` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls nothing. Not seen in play. Not read. |
| `sub_820FF908` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_825A71C0`. Not seen in play. Not read. |
| `sub_820FF958` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls nothing. Not seen in play. Not read. |
| `sub_820FFA88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls nothing. Not seen in play. Not read. |
| `sub_820FFB60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_82101940` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_82101860`. Not seen in play. Not read. |
| `sub_82102A28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls nothing. Not seen in play. Not read. |
| `sub_82108A08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_822A67B0`. Not seen in play. Not read. |
| `sub_82108A70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_8259C6C0`. Not seen in play. Not read. |
| `sub_82109400` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_82108860`, `sub_82108A08`, `sub_82295330`, `sub_8258D8C8`, `sub_8258D908`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82109A40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_82109400`, `sub_82295330`, `sub_82296270`. Not seen in play. Not read. |
| `sub_8210A4D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_82109A40`, `sub_82295330`, `sub_82296270`. Not seen in play. Not read. |
| `sub_8210AEA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_8259C6F0`. Not seen in play. Not read. |
| `sub_8210AFA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_8210B388`, `sub_8259C818`, `sub_8259C868`. Not seen in play. Not read. |
| `sub_8210B090` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_8259C6F0`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8210B0E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_8259C7F8`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8210B388` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8210B650` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_8210AEA0`, `sub_8210B388`, `sub_8258CEA0`. Not seen in play. Not read. |
| `sub_8210B6E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_8210B140`. Not seen in play. Not read. |
| `sub_8210B908` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_8210B650`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8210B958` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_8210B140`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8210B9C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_8210B140`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8210BDB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_8210B438`, `sub_8210B8A8`. Not seen in play. Not read. |
| `sub_82116FB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_825A70E8`, `sub_825A71C0`. Not seen in play. Not read. |
| `sub_82117DC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82117E90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82117EB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls nothing. Not seen in play. Not read. |
| `sub_82118610` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_005.cpp`, `--native_lifted_005`). Calls `sub_82118490`. Not seen in play. Not read. |
| `sub_82118930` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_821176E8`, `sub_82118438`. Not seen in play. Not read. |
| `sub_82119018` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_820ACF88`, `sub_820AD3A0`. Not seen in play. Not read. |
| `sub_82119390` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_821193A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_82117EA8`. Not seen in play. Not read. |
| `sub_821193B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_82119450`. Not seen in play. Not read. |
| `sub_821193B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_82119450`. Not seen in play. Not read. |
| `sub_8211EAA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_82293568`. Not seen in play. Not read. |
| `sub_8211F1C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_82293568`. Not seen in play. Not read. |
| `sub_8211F278` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls nothing. Not seen in play. Not read. |
| `sub_8211F530` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_8211F118`, `sub_82293568`. Not seen in play. Not read. |
| `sub_8211F6F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_8211F118`, `sub_82293568`. Not seen in play. Not read. |
| `sub_8211F8C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_825A88A0`, `sub_825A8978`. Not seen in play. Not read. |
| `sub_8211FF20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls nothing. Not seen in play. Not read. |
| `sub_82120008` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_8211EB60`, `sub_82126630`, `sub_82129298`. Not seen in play. Not read. |
| `sub_82120868` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_8211EB60`, `sub_82126630`, `sub_82129298`. Not seen in play. Not read. |
| `sub_82120BB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_8211EB60`, `sub_82126630`, `sub_82129298`. Not seen in play. Not read. |
| `sub_82121280` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_8211EB60`, `sub_82126630`, `sub_82129298`. Not seen in play. Not read. |
| `sub_82121590` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls nothing. Not seen in play. Not read. |
| `sub_82121760` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_033.cpp`, `--native_lifted_033`). Calls `sub_82121590`, `sub_82121860`. Not seen in play. Not read. |
| `sub_82121D78` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_8211EB60`, `sub_82126630`, `sub_82129298`. Not seen in play. Not read. |
| `sub_82122DB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_82126550`, `sub_82126BB0`, `sub_82127198`. Not seen in play. Not read. |
| `sub_82126818` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82126A00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_820C0170`. Not seen in play. Not read. |
| `sub_82126A38` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_820C0170`. Not seen in play. Not read. |
| `sub_82126A90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls nothing. Not seen in play. Not read. |
| `sub_82126D30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_82126D64`. Not seen in play. Not read. |
| `sub_82127188` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_82126D30`. Not seen in play. Not read. |
| `sub_82127198` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_82126BB0`, `sub_82126CB0`. Not seen in play. Not read. |
| `sub_8212EEF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls nothing. Not seen in play. Not read. |
| `sub_8212F538` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_825A9BB8`. Not seen in play. Not read. |
| `sub_8212F600` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_825A9D80`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8212F778` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_825A9D80`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82130330` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_821301F0`, `sub_82130290`, `sub_825A8CA0`. Not seen in play. Not read. |
| `sub_82130490` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_821301F0`, `sub_82130290`, `sub_825A8CA0`. Not seen in play. Not read. |
| `sub_82130618` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_82130330`, `sub_82130490`, `sub_825A88A0`, `sub_825A8978`. Not seen in play. Not read. |
| `sub_82133B10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_82133190`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_82133BB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_821332F0`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82133C18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82134410` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_82133190`, `sub_822A5E28`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_82134618` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_821332F0`, `sub_82133C60`. Not seen in play. Not read. |
| `sub_821350E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls nothing. Not seen in play. Not read. |
| `sub_821350F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls nothing. Not seen in play. Not read. |
| `sub_82135118` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls nothing. Not seen in play. Not read. |
| `sub_82135150` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls nothing. Not seen in play. Not read. |
| `sub_82135170` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls nothing. Not seen in play. Not read. |
| `sub_82135190` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls nothing. Not seen in play. Not read. |
| `sub_82135198` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls nothing. Not seen in play. Not read. |
| `sub_821351A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls nothing. Not seen in play. Not read. |
| `sub_821356B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_006.cpp`, `--native_lifted_006`). Calls `sub_82133340`, `sub_82133608`, `sub_82133E30`, `sub_82133F58`, `sub_825896D8`, `sub_82589700`, `sub_8258C9D0`, `sub_8258CA50`, .... Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82136DC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82133C60`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82136F50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82136F58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82136F60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82136F68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82136F70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82136F78` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82136F80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82136F88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82136F90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82136F98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82136FA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82138520` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82134618`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82138FA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_82138F00`. Not seen in play. Not read. |
| `sub_821392D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82139A08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_82138F00`, `sub_82138FA8`. Not seen in play. Not read. |
| `sub_8213A200` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82138F00`, `sub_821392D8`, `sub_82139A08`, `sub_82145408`, `sub_825A8770`. Not seen in play. Not read. |
| `sub_8213AB40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82139A08`, `sub_8213A200`. Not seen in play. Not read. |
| `sub_8213BDF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_8213BE80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_8213D898` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_8213CB30`, `sub_8213CD50`. Not seen in play. Not read. |
| `sub_8213F7E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls nothing. Not seen in play. Not read. |
| `sub_8213F868` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_821462A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82139728`, `sub_821451B8`. Not seen in play. Not read. |
| `sub_82148920` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82294E20`. Not seen in play. Not read. |
| `sub_821489D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82459C38`. Not seen in play. Not read. |
| `sub_82148B60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82294F70`, `sub_82295018`. Not seen in play. Not read. |
| `sub_82148D00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_825765F0`, `sub_825A6940`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_821495E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_822951A8`, `sub_82295208`, `sub_82295D30`, `sub_825A6940`, `sub_825A71C0`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_821497A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82149888` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_821498F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls nothing. Not seen in play. Not read. |
| `sub_82149A30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82091200`, `sub_825A6880`. Not seen in play. Not read. |
| `sub_82149AA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82149AA8`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82149B08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_82149C48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82149AA8`. Not seen in play. Not read. |
| `sub_82149D00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82149A30`. Not seen in play. Not read. |
| `sub_8214AA18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82149D00`. Not seen in play. Not read. |
| `sub_8214AFE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_8214AB48`. Not seen in play. Not read. |
| `sub_8214B188` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82149B08`. Not seen in play. Not read. |
| `sub_8214B2C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls `sub_8214B188`. Not seen in play. Not read. |
| `sub_8214B330` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_820C96E8`, `sub_821489D8`, `sub_82148A80`, `sub_82148B60`, `sub_82148D00`, `sub_821495E8`, `sub_8214AFE8`, `sub_8214B2C8`, .... Not seen in play. Not read. |
| `sub_8214BF20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82091200`, `sub_82091328`, `sub_820915F8`, `sub_820919B8`, `sub_821497A8`, `sub_82149888`, `sub_821498F0`, `sub_825931D0`, .... Not seen in play. Not read. |
| `sub_8214C828` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_821497A8`, `sub_82149C48`, `sub_8214BF20`. Not seen in play. Not read. |
| `sub_8214C930` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_8214C828`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8214E240` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82148920`, `sub_8214F240`. Not seen in play. Not read. |
| `sub_8214E608` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_825A71D0`. Not seen in play. Not read. |
| `sub_8214E678` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82294E20`, `sub_82576080`. Not seen in play. Not read. |
| `sub_8214E6C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82575E50`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_8214EC80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82295700`, `sub_825763D0`, `sub_825A6940`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_8214ED30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_8214E6C8`, `sub_8214EC80`, `sub_82576080`, `sub_825764A8`, `sub_82576540`, `sub_825A71C0`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_8214F240` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_82294E20`. Not seen in play. Not read. |
| `sub_8215F5F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_820C96E8`, `sub_821705F8`. Not seen in play. Not read. |
| `sub_82160880` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_007.cpp`, `--native_lifted_007`). Calls `sub_820C0468`, `sub_8215F968`, `sub_821604C8`, `sub_821705F8`. Not seen in play. Not read. |
| `sub_8217C4C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_008.cpp`, `--native_lifted_008`). Calls `sub_8217AEA8`, `sub_8258C9D0`, `sub_8258CA50`, `sub_8258CA88`, `sub_8258CAA0`, `sub_8258CB08`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8218D708` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_820C27E8`, `sub_820C4550`, `sub_820C4998`, `sub_820CD5D8`, `sub_820CE1D8`, `sub_820CE7D8`, `sub_8218D844`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_821AB518` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821AB5B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821AB698` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821AB7F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8209D0C8`, `sub_8211B3F0`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821AB898` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C5F8`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821ABC00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821ABD30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821ABDC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821ABEF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821ABFD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821AC068` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821AC0F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821AC188` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821AC218` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821AC2A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821AC338` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`. Not seen in play. Not read. |
| `sub_821AC408` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`. Not seen in play. Not read. |
| `sub_821AC4D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`. Not seen in play. Not read. |
| `sub_821AC5A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`, `sub_82180108`. Not seen in play. Not read. |
| `sub_821AC6A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`, `sub_82180108`. Not seen in play. Not read. |
| `sub_821AC7A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`. Not seen in play. Not read. |
| `sub_821AC878` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`. Not seen in play. Not read. |
| `sub_821AC948` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`. Not seen in play. Not read. |
| `sub_821ACA18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`, `sub_82180108`. Not seen in play. Not read. |
| `sub_821ACB18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`. Not seen in play. Not read. |
| `sub_821ACBE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`. Not seen in play. Not read. |
| `sub_821ACCB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`. Not seen in play. Not read. |
| `sub_821ACD88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`. Not seen in play. Not read. |
| `sub_821ACE58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`. Not seen in play. Not read. |
| `sub_821ACF28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211CC50`, `sub_8259D658`. Not seen in play. Not read. |
| `sub_821AD2E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`. Not seen in play. Not read. |
| `sub_821AD3B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`. Not seen in play. Not read. |
| `sub_821AD458` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211A5B8`, `sub_8211B3F0`. Not seen in play. Not read. |
| `sub_821AD530` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211A5B8`, `sub_8211B3F0`. Not seen in play. Not read. |
| `sub_821AD5D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C7E8`, `sub_8259D658`. Not seen in play. Not read. |
| `sub_821ADAF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C7E8`, `sub_8211CC50`, `sub_8259D658`. Not seen in play. Not read. |
| `sub_821ADF28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C7E8`, `sub_8259D658`. Not seen in play. Not read. |
| `sub_821AE198` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_009.cpp`, `--native_lifted_009`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C7E8`, `sub_8259D658`. Not seen in play. Not read. |
| `sub_821AE640` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A5B8`, `sub_8211B3F0`. Not seen in play. Not read. |
| `sub_821AE6D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C7E8`, `sub_8259D658`. Not seen in play. Not read. |
| `sub_821AEB48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211C1F8`. Not seen in play. Not read. |
| `sub_821AEC18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8259D658`. Not seen in play. Not read. |
| `sub_821B1678` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C5F8`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, .... Not seen in play. Not read. |
| `sub_821B1960` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C5F8`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, .... Not seen in play. Not read. |
| `sub_821B1B40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C5F8`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, .... Not seen in play. Not read. |
| `sub_821B1DA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C5F8`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, .... Not seen in play. Not read. |
| `sub_821B2100` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, .... Not seen in play. Not read. |
| `sub_821B23B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AC338`. Not seen in play. Not read. |
| `sub_821B2480` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`. Not seen in play. Not read. |
| `sub_821B25C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821AD840`. Not seen in play. Not read. |
| `sub_821B2658` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AC408`. Not seen in play. Not read. |
| `sub_821B2728` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`. Not seen in play. Not read. |
| `sub_821B2870` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821ADC48`. Not seen in play. Not read. |
| `sub_821B2900` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AC4D8`. Not seen in play. Not read. |
| `sub_821B29D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821ADDB8`. Not seen in play. Not read. |
| `sub_821B2A60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AC5A8`. Not seen in play. Not read. |
| `sub_821B2B30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, `sub_82180108`, `sub_821AD2E0`, `sub_821AD3B0`. Not seen in play. Not read. |
| `sub_821B2C78` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821ADF98`. Not seen in play. Not read. |
| `sub_821B2D08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AC6A8`. Not seen in play. Not read. |
| `sub_821B2DD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, `sub_82180108`, `sub_821AD2E0`, `sub_821AD3B0`. Not seen in play. Not read. |
| `sub_821B2F20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821AE098`. Not seen in play. Not read. |
| `sub_821B2FB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AC7A8`. Not seen in play. Not read. |
| `sub_821B3080` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821AE240`. Not seen in play. Not read. |
| `sub_821B3110` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AC878`. Not seen in play. Not read. |
| `sub_821B31E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821AE340`. Not seen in play. Not read. |
| `sub_821B3270` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AC948`. Not seen in play. Not read. |
| `sub_821B3340` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821AE440`. Not seen in play. Not read. |
| `sub_821B33D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821ACA18`. Not seen in play. Not read. |
| `sub_821B34A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, `sub_82180108`, `sub_821AD2E0`, `sub_821AD3B0`. Not seen in play. Not read. |
| `sub_821B35E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821AE540`. Not seen in play. Not read. |
| `sub_821B3670` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821ACB18`. Not seen in play. Not read. |
| `sub_821B3740` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821AE748`. Not seen in play. Not read. |
| `sub_821B37D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821ACBE8`. Not seen in play. Not read. |
| `sub_821B38A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`. Not seen in play. Not read. |
| `sub_821B39A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821AE808`. Not seen in play. Not read. |
| `sub_821B3A38` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821ACCB8`. Not seen in play. Not read. |
| `sub_821B3B08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821AE8C8`. Not seen in play. Not read. |
| `sub_821B3B98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821ACD88`. Not seen in play. Not read. |
| `sub_821B3C68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`. Not seen in play. Not read. |
| `sub_821B3D70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821AE988`. Not seen in play. Not read. |
| `sub_821B3E00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821ACE58`. Not seen in play. Not read. |
| `sub_821B3ED0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`. Not seen in play. Not read. |
| `sub_821B3FD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211E1B8`, `sub_821AD2E0`, `sub_821AD3B0`, `sub_821AEA88`. Not seen in play. Not read. |
| `sub_821B4068` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`, `sub_821ACF28`. Not seen in play. Not read. |
| `sub_821B4298` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`, `sub_821ACF28`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_821B4548` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`, `sub_821ACF28`. Not seen in play. Not read. |
| `sub_821B46D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`, `sub_821ACF28`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_821B4848` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`, `sub_821ACF28`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_821B49F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_8211C570`, `sub_8211C5F8`, `sub_821ACF28`, `sub_821AEB48`, `sub_821AEC18`, .... Not seen in play. Not read. |
| `sub_821B4CE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8209D038`, `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211E1B8`, `sub_821AD458`, `sub_821B4068`. Not seen in play. Not read. |
| `sub_821B4E10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD530`, `sub_821AD5D8`, `sub_821AD840`, `sub_821ADA08`, .... Not seen in play. Not read. |
| `sub_821B4F00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD458`, `sub_821B4068`. Not seen in play. Not read. |
| `sub_821B4FD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_82180108`, `sub_821ADF28`, `sub_821ADF98`, `sub_821AE018`, .... Not seen in play. Not read. |
| `sub_821B50E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD458`, `sub_821B4068`. Not seen in play. Not read. |
| `sub_821B51C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_82180108`, `sub_821AE198`, `sub_821AE240`, `sub_821AE2C0`, .... Not seen in play. Not read. |
| `sub_821B52D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD458`, `sub_821B4068`. Not seen in play. Not read. |
| `sub_821B53A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AE640`, `sub_821AE6D8`, `sub_821AE748`, `sub_821AE7A8`, .... Not seen in play. Not read. |
| `sub_821B5498` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD458`, `sub_821B4068`. Not seen in play. Not read. |
| `sub_821B5570` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211BB18`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821B49F8`. Not seen in play. Not read. |
| `sub_821B56C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211BB18`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD458`, `sub_821B4068`. Not seen in play. Not read. |
| `sub_821B5818` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8209D038`, `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821B49F8`. Not seen in play. Not read. |
| `sub_821B5A00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821AD530`, `sub_821B4298`. Not seen in play. Not read. |
| `sub_821B5AF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_010.cpp`, `--native_lifted_010`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821AD530`, `sub_821B4298`. Not seen in play. Not read. |
| `sub_821B5BF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821AD530`, `sub_821B4298`. Not seen in play. Not read. |
| `sub_821B5CE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821B4548`. Not seen in play. Not read. |
| `sub_821B5DF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821B4548`. Not seen in play. Not read. |
| `sub_821B5F08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821B46D8`. Not seen in play. Not read. |
| `sub_821B5FF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821B46D8`. Not seen in play. Not read. |
| `sub_821B60E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821B46D8`. Not seen in play. Not read. |
| `sub_821B61D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821B46D8`. Not seen in play. Not read. |
| `sub_821B62C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821AE640`, `sub_821B4848`. Not seen in play. Not read. |
| `sub_821B63D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821AE640`, `sub_821B4848`. Not seen in play. Not read. |
| `sub_821B64E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821AE640`, `sub_821B4848`. Not seen in play. Not read. |
| `sub_821B65F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821AE640`, `sub_821B4848`. Not seen in play. Not read. |
| `sub_821B6708` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DF20`, `sub_8211E1B8`, `sub_821AD3B0`, `sub_821AE640`, `sub_821B4848`. Not seen in play. Not read. |
| `sub_821B6818` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_820C4998`, `sub_820CD5D8`, `sub_82105620`, `sub_82106540`, `sub_8211B3F0`, `sub_8211E1B8`, `sub_8215AAD0`, `sub_82178B40`, .... Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_821B8588` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211D978`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_821B8630` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211A578`, `sub_8211B3F0`. Not seen in play. Not read. |
| `sub_821B8688` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211A578`, `sub_8211B3F0`. Not seen in play. Not read. |
| `sub_821B8728` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211A578`, `sub_8211B3F0`. Not seen in play. Not read. |
| `sub_821B87B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_821B8630`, `sub_821B8688`, `sub_821B8728`. Not seen in play. Not read. |
| `sub_821B8988` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211C1F8`, `sub_821B87B8`. Not seen in play. Not read. |
| `sub_821B8ED0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_011.cpp`, `--native_lifted_011`). Calls `sub_8211B3F0`, `sub_8211D978`, `sub_8211E1B8`, `sub_821B8988`. Not seen in play. Not read. |
| `sub_822116A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_012.cpp`, `--native_lifted_012`). Calls `sub_822962B0`, `sub_822A5B78`. Not seen in play. Not read. |
| `sub_822159F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_012.cpp`, `--native_lifted_012`). Calls `sub_8211B3F0`, `sub_8211DAC8`. Not seen in play. Not read. |
| `sub_82215A50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_012.cpp`, `--native_lifted_012`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211DF20`. Not seen in play. Not read. |
| `sub_82215B48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_012.cpp`, `--native_lifted_012`). Calls `sub_8211A578`, `sub_8211B3F0`, `sub_8211DAC8`, `sub_8211DDE0`, `sub_8211DF20`. Not seen in play. Not read. |
| `sub_82215D08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_012.cpp`, `--native_lifted_012`). Calls `sub_8211B3F0`, `sub_8211DF20`. Not seen in play. Not read. |
| `sub_82215D58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_012.cpp`, `--native_lifted_012`). Calls `sub_8211B3F0`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_82215DA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_821103F0`, `sub_8211B3F0`, `sub_8211BB18`, `sub_8211E1B8`. Not seen in play. Not read. |
| `sub_82216810` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_012.cpp`, `--native_lifted_012`). Calls `sub_82215A50`, `sub_82215B48`. Not seen in play. Not read. |
| `sub_82260800` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_821489D8`, `sub_8214AA70`, `sub_8214B330`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82291520` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_8209F658`, `sub_82296720`. Not seen in play. Not read. |
| `sub_822916D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_82106540`. Not seen in play. Not read. |
| `sub_82291760` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_8214B330`. Not seen in play. Not read. |
| `sub_822917B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_8214B330`, `sub_82294F70`, `sub_82575E50`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82291858` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_8209F658`, `sub_8214B330`, `sub_825761D8`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82291920` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_8214B330`, `sub_825764A8`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_822919A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_82576540`, `sub_825A71C0`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82291A40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_82576328`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82291AD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_82576080`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82291B50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_8214B330`, `sub_82295700`. Not seen in play. Not read. |
| `sub_82291C48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_82106540`. Not seen in play. Not read. |
| `sub_82291D28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_82106540`. Not seen in play. Not read. |
| `sub_82291F20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_82106540`. Not seen in play. Not read. |
| `sub_82292020` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_82106540`, `sub_82295700`. Not seen in play. Not read. |
| `sub_82292828` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_82106540`, `sub_822916D0`, `sub_82291C48`, `sub_82291D28`, `sub_82291F20`, `sub_82292020`. Not seen in play. Not read. |
| `sub_82292938` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_013.cpp`, `--native_lifted_013`). Calls `sub_8214B330`, `sub_82291520`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_822929E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8214B330`, `sub_822917B8`. Not seen in play. Not read. |
| `sub_82292A48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8214B330`, `sub_82292648`, `sub_822950C0`, `sub_825A6940`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82292B28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8214B330`, `sub_822919A0`, `sub_825766E0`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82292DB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8214B330`, `sub_82292648`, `sub_82295208`, `sub_82295D30`, `sub_825A6940`, `sub_825A71C0`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82292E98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8214B330`, `sub_82291858`, `sub_82291920`, `sub_82292A48`, `sub_82292B28`, `sub_82294E20`, `sub_82295700`, `sub_82576080`. Not seen in play. Not read. |
| `sub_82292FB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_82292B28`, `sub_82292E98`. Not seen in play. Not read. |
| `sub_82293028` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_821A66A8`, `sub_82576BE0`, `sub_82576F30`, `sub_825A6940`, `sub_825A71C0`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82293190` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_82291A40`, `sub_82291AD0`, `sub_82292DB8`, `sub_82293028`, `sub_82294E20`, `sub_82295700`, `sub_82576080`. Not seen in play. Not read. |
| `sub_82293288` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8214B330`, `sub_82292CC0`, `sub_82293190`. Not seen in play. Not read. |
| `sub_82293358` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_82106540`, `sub_82291760`, `sub_82291B50`, `sub_82292938`, `sub_822929E8`, `sub_82292FB0`, `sub_82293288`. Not seen in play. Not read. |
| `sub_82293CD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls nothing. Not seen in play. Not read. |
| `sub_82293F68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_82293CD8`, `sub_822942A0`, `sub_82294430`. Not seen in play. Not read. |
| `sub_82293FD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_82293CD8`, `sub_82294430`. Not seen in play. Not read. |
| `sub_822945F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls nothing. Not seen in play. Not read. |
| `sub_822947A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_825A9A90`. Not seen in play. Not read. |
| `sub_822947E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_825A9B58`. Not seen in play. Not read. |
| `sub_82295018` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_82294F78`. Not seen in play. Not read. |
| `sub_82295700` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_82295F18`. Not seen in play. Not read. |
| `sub_82296F48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Imports `KeDelayExecutionThread`. Not seen in play. Not read. |
| `sub_82298198` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_82297FD8`. Not seen in play. Not read. |
| `sub_82298950` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls nothing. Not seen in play. Not read. |
| `sub_8229BFD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8229F068`. Not seen in play. Not read. |
| `sub_8229D620` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_82297790`, `sub_8229EAF0`. Not seen in play. Not read. |
| `sub_8229D688` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8229EAF0`, `sub_822AC470`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_8229D7E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls nothing. Not seen in play. Not read. |
| `sub_8229D7F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_822976F8`. Not seen in play. Not read. |
| `sub_8229D850` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8229D7F8`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_8229DA28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_822A3758`, `sub_822A3908`, `sub_822A3D30`, `sub_822A4060`, `sub_822A6008`, `sub_822A8618`, `sub_822A8958`, `sub_822A8960`. Not seen in play. Not read. |
| `sub_8229FC40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8229E308`, `sub_8229EF48`, `sub_822ADC08`, `sub_822ADE30`, `sub_822AF978`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_822A31D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls nothing. Not seen in play. Not read. |
| `sub_822A5C50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_822976F8`, `sub_822A4FF0`, `sub_822A6738`. Not seen in play. Not read. |
| `sub_822A8418` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls nothing. Not seen in play. Not read. |
| `sub_822A84C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls nothing. Not seen in play. Not read. |
| `sub_822A8518` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls nothing. Not seen in play. Not read. |
| `sub_822AC5B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_822AC530`. Imports `_vsnprintf`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_822AC858` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_822AC5B8`. Not seen in play. Not read. |
| `sub_822AD058` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_82297790`, `sub_822AD058`. Not seen in play. Not read. |
| `sub_822AD4A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8229E098`, `sub_822AC2A0`, `sub_822ACB90`, `sub_822AD228`. Not seen in play. Not read. |
| `sub_822AD790` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8229E308`, `sub_822AC470`. Not seen in play. Not read. |
| `sub_822ADA28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls nothing. Not seen in play. Not read. |
| `sub_822ADA98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls nothing. Not seen in play. Not read. |
| `sub_822ADB50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8229EF48`. Not seen in play. Not read. |
| `sub_822AE918` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_822AE9E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_822AEAA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_822AE918`, `sub_822AE9E8`. Not seen in play. Not read. |
| `sub_822AEBF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_8229F148`, `sub_822AC470`, `sub_822AE340`, `sub_822AEAA0`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_822AF2D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls nothing. Not seen in play. Not read. |
| `sub_822AF3D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls nothing. Not seen in play. Not read. |
| `sub_822AF438` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_822AF3D0`. Not seen in play. Not read. |
| `sub_822AF550` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_822AF5AC`, `sub_825AAD20`. Not seen in play. Not read. |
| `sub_822AF8A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_822AF550`. Not seen in play. Not read. |
| `sub_822B0ED0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_822B0F40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_014.cpp`, `--native_lifted_014`). Calls nothing. Not seen in play. Not read. |
| `sub_822B4300` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_822B4318` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_822B4330` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls `sub_825AAF08`. Not seen in play. Not read. |
| `sub_822B4360` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls `sub_825A7F50`. Not seen in play. Not read. |
| `sub_822B43A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_822B43E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls `sub_825A71D0`, `sub_825A7F50`, `sub_825AACD0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_822B4680` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls `sub_82297790`, `sub_825A71D0`. Imports `MmFreePhysicalMemory`. Not seen in play. Not read. |
| `sub_822B4E78` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_822B62E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Imports `KeSetEvent`, `KeWaitForSingleObject`. Not seen in play. Not read. |
| `sub_822C3958` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_822F1218` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_82304258` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_82368F58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_82369050` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_82369138` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82369188` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82379360` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_82379368` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_8243DC88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_8244A528` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_015.cpp`, `--native_lifted_015`). Calls nothing. Not seen in play. Not read. |
| `sub_82461F10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_82461F20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_825B9A70`. Imports `KeAcquireSpinLockAtRaisedIrql`, `KeRaiseIrqlToDpcLevel`, `KeReleaseSpinLockFromRaisedIrql`, `KfLowerIrql`. Not seen in play. Not read. |
| `sub_82465398` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8245EF90`. Not seen in play. Not read. |
| `sub_82465590` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8245EF90`. Not seen in play. Not read. |
| `sub_82465848` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_824658C8`. Not seen in play. Not read. |
| `sub_82465850` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_82465A48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82465B48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82465BF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_82465C18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82465CF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Imports `KeAcquireSpinLockAtRaisedIrql`, `KeRaiseIrqlToDpcLevel`, `KeReleaseSpinLockFromRaisedIrql`, `KfLowerIrql`. Not seen in play. Not read. |
| `sub_82466058` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Imports `KeAcquireSpinLockAtRaisedIrql`, `KeRaiseIrqlToDpcLevel`, `KeReleaseSpinLockFromRaisedIrql`, `KfLowerIrql`. Not seen in play. Not read. |
| `sub_82466410` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_82465418`. Not seen in play. Not read. |
| `sub_82466990` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82466A80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82466C40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_82467450`. Not seen in play. Not read. |
| `sub_82466C48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_82466C58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_824699E0`. Not seen in play. Not read. |
| `sub_82466C60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82466CB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82466D00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82466D50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_82466DB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_82466DC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_82466DD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_825A9C00`. Not seen in play. Not read. |
| `sub_82466E18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_825A8ED0`. Not seen in play. Not read. |
| `sub_82466E60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_82466E70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_82466F40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Imports `KeAcquireSpinLockAtRaisedIrql`, `KeRaiseIrqlToDpcLevel`, `KeReleaseSpinLockFromRaisedIrql`, `KfLowerIrql`. Not seen in play. Not read. |
| `sub_82467140` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Imports `KeAcquireSpinLockAtRaisedIrql`, `KeRaiseIrqlToDpcLevel`, `KeReleaseSpinLockFromRaisedIrql`, `KfLowerIrql`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82467230` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Imports `KeAcquireSpinLockAtRaisedIrql`, `KeRaiseIrqlToDpcLevel`, `KeReleaseSpinLockFromRaisedIrql`, `KfLowerIrql`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82467360` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Imports `KeAcquireSpinLockAtRaisedIrql`, `KeRaiseIrqlToDpcLevel`, `KeReleaseSpinLockFromRaisedIrql`, `KfLowerIrql`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82467450` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8245EF90`, `sub_82469960`. Not seen in play. Not read. |
| `sub_82469A88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8245EF90`, `sub_8246E0E8`. Not seen in play. Not read. |
| `sub_8246A900` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246AAD0`. Not seen in play. Not read. |
| `sub_8246A908` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246A860`. Not seen in play. Not read. |
| `sub_8246A910` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246A7D0`. Not seen in play. Not read. |
| `sub_8246A930` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_8246A940` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_825A9C00`. Not seen in play. Not read. |
| `sub_8246A988` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_8246A998` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8246A9F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8246AA48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_8246AA58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_8246AAC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_82466990`. Not seen in play. Not read. |
| `sub_8246AAC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_82466A80`. Not seen in play. Not read. |
| `sub_8246AB00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_82469E58`. Not seen in play. Not read. |
| `sub_8246AD98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Imports `KeAcquireSpinLockAtRaisedIrql`, `KeRaiseIrqlToDpcLevel`, `KeReleaseSpinLockFromRaisedIrql`, `KfLowerIrql`. Not seen in play. Not read. |
| `sub_8246B318` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_825A7410`. Imports `KeAcquireSpinLockAtRaisedIrql`, `KeRaiseIrqlToDpcLevel`, `KeReleaseSpinLockFromRaisedIrql`, `KfLowerIrql`. Not seen in play. Not read. |
| `sub_8246B3F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246A940`. Not seen in play. Not read. |
| `sub_8246B3F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246AB00`. Not seen in play. Not read. |
| `sub_8246B400` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246AAC8`. Not seen in play. Not read. |
| `sub_8246B408` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246B0C8`. Not seen in play. Not read. |
| `sub_8246B410` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246A988`. Not seen in play. Not read. |
| `sub_8246B418` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246A920`. Not seen in play. Not read. |
| `sub_8246B420` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246AB48`. Not seen in play. Not read. |
| `sub_8246B428` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246AA58`. Not seen in play. Not read. |
| `sub_8246B430` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246AAC0`. Not seen in play. Not read. |
| `sub_8246B438` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246AC40`. Not seen in play. Not read. |
| `sub_8246B440` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246A930`. Not seen in play. Not read. |
| `sub_8246B448` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246AD98`. Not seen in play. Not read. |
| `sub_8246B450` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246B0C8`. Not seen in play. Not read. |
| `sub_8246C1B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246BFD8`. Not seen in play. Not read. |
| `sub_8246C1C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246B450`. Not seen in play. Not read. |
| `sub_8246C1C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246BE20`. Not seen in play. Not read. |
| `sub_8246CE60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246CF50`. Not seen in play. Not read. |
| `sub_8246CED0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_8246CEF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8246CF40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls nothing. Not seen in play. Not read. |
| `sub_8246D2E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_82466E80`. Not seen in play. Not read. |
| `sub_8246D2E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246CEB8`. Not seen in play. Not read. |
| `sub_8246D2F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246D008`. Not seen in play. Not read. |
| `sub_8246D2F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246CEC8`. Not seen in play. Not read. |
| `sub_8246D300` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246CEF0`. Not seen in play. Not read. |
| `sub_8246D308` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_016.cpp`, `--native_lifted_016`). Calls `sub_8246CE70`. Not seen in play. Not read. |
| `sub_8246D310` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246D0C8`. Not seen in play. Not read. |
| `sub_8246D318` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246CEC0`. Not seen in play. Not read. |
| `sub_8246D328` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246A998`. Not seen in play. Not read. |
| `sub_8246D330` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82460370`. Not seen in play. Not read. |
| `sub_8246D338` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246CFA8`. Not seen in play. Not read. |
| `sub_8246D340` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246A9F0`. Not seen in play. Not read. |
| `sub_8246D348` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Imports `KeAcquireSpinLockAtRaisedIrql`, `KeRaiseIrqlToDpcLevel`, `KeReleaseSpinLockFromRaisedIrql`, `KfLowerIrql`. Not seen in play. Not read. |
| `sub_8246D738` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246D630`. Not seen in play. Not read. |
| `sub_8246D740` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246D500`. Not seen in play. Not read. |
| `sub_8246D748` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246D410`. Not seen in play. Not read. |
| `sub_8246E7F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_8246EC08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_8246F300` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_8246F540` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_8246F7C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_8246FAC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_8246FE48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_824701E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_82470668` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_82470CA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_029.cpp`, `--native_lifted_029`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_82471510` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_824717F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_82471B40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_82471F68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_825A9DB0`. Not seen in play. Not read. |
| `sub_824724B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_824728B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82472BB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82472F30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_824733C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_824739B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82474578` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82474AE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82475200` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82475688` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_824759D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82475E20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82476418` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82476BF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82476FF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82477348` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82477760` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82477D20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_824784D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82478660` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82478770` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82478890` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_824789C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82478B18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82478D30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82478FB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82479158` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82479348` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82479540` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82479670` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_824797C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82479968` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82479B58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82479CA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82479DE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82479F48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_8247A108` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_8247A320` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_8247A610` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_8247A758` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_8247A8C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_8247AA80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_8247AC90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247B1E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247B3B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247B628` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247B9C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247BEB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247C408` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247C5D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247C850` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247CC00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247D110` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247D358` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247D548` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247D7B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247DAA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8247DE28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246F300`, `sub_824728B0`. Not seen in play. Not read. |
| `sub_8247DF00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246F540`, `sub_82472BB8`. Not seen in play. Not read. |
| `sub_8247DFD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246F7C0`, `sub_82472F30`. Not seen in play. Not read. |
| `sub_8247E0B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246FAC0`, `sub_824733C0`. Not seen in play. Not read. |
| `sub_8247E338` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246E7F8`, `sub_82474578`. Not seen in play. Not read. |
| `sub_8247E410` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246EC08`, `sub_82474AE8`. Not seen in play. Not read. |
| `sub_8247E4E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8246FE48`, `sub_82475688`. Not seen in play. Not read. |
| `sub_8247E5C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_824701E0`, `sub_824759D8`. Not seen in play. Not read. |
| `sub_8247E6A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82470668`, `sub_82475E20`. Not seen in play. Not read. |
| `sub_8247E788` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82470CA0`, `sub_82476418`. Not seen in play. Not read. |
| `sub_8247E868` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82471510`, `sub_82476FF0`. Not seen in play. Not read. |
| `sub_8247E940` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_824717F8`, `sub_82477348`. Not seen in play. Not read. |
| `sub_8247EA18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82471B40`, `sub_82477760`. Not seen in play. Not read. |
| `sub_8247EAF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82471F68`, `sub_82477D20`. Not seen in play. Not read. |
| `sub_8247EBC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82478660`. Not seen in play. Not read. |
| `sub_8247EDA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82478770`. Not seen in play. Not read. |
| `sub_8247EFD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_824789C8`. Not seen in play. Not read. |
| `sub_8247F320` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82478D30`. Not seen in play. Not read. |
| `sub_8247F7A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82479158`. Not seen in play. Not read. |
| `sub_8247FB28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82479540`. Not seen in play. Not read. |
| `sub_8247FE18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_8247A610`. Not seen in play. Not read. |
| `sub_824800A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_8247B1E8`. Not seen in play. Not read. |
| `sub_82480368` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_8247C408`. Not seen in play. Not read. |
| `sub_82481B88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82481B98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82483130`, `sub_82483460`, `sub_82483700`, `sub_82483770`, `sub_82483DC0`, `sub_82483F30`, `sub_824847D0`, `sub_82485350`, .... Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82482068` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82481B98`, `sub_82487660`, `sub_824877E0`, `sub_824878B0`, `sub_824878D0`, `sub_82487B28`, `sub_82487BE8`, `sub_82488F50`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82482730` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82482068`. Not seen in play. Not read. |
| `sub_82482780` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_824892A8`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82482968` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82482780`, `sub_82489048`. Not seen in play. Not read. |
| `sub_82483238` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_824892D0`, `sub_82489300`. Not seen in play. Not read. |
| `sub_82483388` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_824892D0`, `sub_82489300`. Not seen in play. Not read. |
| `sub_82483460` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_822976F8`. Not seen in play. Not read. |
| `sub_82483700` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82489048`, `sub_824892A8`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82483770` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8245A760`, `sub_8245A768`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82483DC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_822976F8`. Not seen in play. Not read. |
| `sub_82483DD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82482780`. Not seen in play. Not read. |
| `sub_82483F30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82482968`, `sub_82483DD0`. Not seen in play. Not read. |
| `sub_824847D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_822976F8`. Not seen in play. Not read. |
| `sub_82485350` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82483238`, `sub_82489048`, `sub_82489300`, `sub_824893B8`, `sub_8248CA80`. Not seen in play. Not read. |
| `sub_82485460` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_822976F8`. Not seen in play. Not read. |
| `sub_82486710` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82483388`, `sub_82489048`, `sub_82489300`, `sub_8248CBD0`, `sub_8248ED78`. Not seen in play. Not read. |
| `sub_82486810` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_822976F8`. Not seen in play. Not read. |
| `sub_82486F90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82489048`, `sub_8248F218`. Not seen in play. Not read. |
| `sub_824875A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_822976F8`. Not seen in play. Not read. |
| `sub_824875B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82490960`. Not seen in play. Not read. |
| `sub_82487660` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_822976F8`. Not seen in play. Not read. |
| `sub_824877E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82489048`. Not seen in play. Not read. |
| `sub_824878B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_822976F8`. Not seen in play. Not read. |
| `sub_824878D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82487B28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82489048`. Not seen in play. Not read. |
| `sub_82487BE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_822976F8`. Not seen in play. Not read. |
| `sub_82488F50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82489048`. Not seen in play. Not read. |
| `sub_82489048` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Imports `RtlInitializeCriticalSection`. Not seen in play. Not read. |
| `sub_824892A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_824892D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82489300` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82489048`. Not seen in play. Not read. |
| `sub_824893B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8248CB58`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8248CA80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_824960B8`, `sub_82496148`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_8248CB58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_8248CBD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8248CB58`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8248EC58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_824960B8`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_8248ED78` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_8248EC58`, `sub_82496148`. Not seen in play. Not read. |
| `sub_8248F218` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82489048`. Not seen in play. Not read. |
| `sub_8248F550` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls nothing. Not seen in play. Not read. |
| `sub_82490960` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82489048`, `sub_8248F550`. Not seen in play. Not read. |
| `sub_824960B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82496148` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_82489048`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_82575B68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_825AC970`. Not seen in play. Not read. |
| `sub_82575BF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_017.cpp`, `--native_lifted_017`). Calls `sub_825774D0`. Not seen in play. Not read. |
| `sub_825767A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82577088` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82578530`. Not seen in play. Not read. |
| `sub_825770B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82578500`, `sub_82578530`. Not seen in play. Not read. |
| `sub_82577110` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82578530`. Not seen in play. Not read. |
| `sub_82577140` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82578500`, `sub_82578530`. Not seen in play. Not read. |
| `sub_82577190` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825770B8`, `sub_82577140`, `sub_82577190`, `sub_825780F0`, `sub_825784A8`, `sub_82578500`, `sub_82578530`, `sub_82578580`. Not seen in play. Not read. |
| `sub_825774D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_82577528` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82577630` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825774D0`, `sub_825780D0`. Not seen in play. Not read. |
| `sub_825776A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825780D0`, `sub_825BA168`. Imports `KeGetCurrentProcessType`. Not seen in play. Not read. |
| `sub_82577788` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82577630`, `sub_825780D0`. Not seen in play. Not read. |
| `sub_82577850` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82577088`. Not seen in play. Not read. |
| `sub_825778D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82296720`, `sub_82577088`, `sub_825770B8`, `sub_82577140`, `sub_82577190`, `sub_82577528`, `sub_82577850`, `sub_825778D8`, .... Not seen in play. Not read. |
| `sub_825780A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825778D8`. Not seen in play. Not read. |
| `sub_825780D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_825780F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82578118` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82578168` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_82578118`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_82578248` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_825782C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82578340` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_825783C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82578470` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls nothing. Not seen in play. Not read. |
| `sub_825784A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_82578118`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_82578500` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82578168`. Not seen in play. Not read. |
| `sub_82578530` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_82578118`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_82578580` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82578118`. Not seen in play. Not read. |
| `sub_82578588` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82578608` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82578658` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_825786B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82577110`, `sub_82577528`, `sub_82578500`, `sub_82578608`. Not seen in play. Not read. |
| `sub_825787D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825780D0`, `sub_82578248`, `sub_825782C0`, `sub_82578340`, `sub_825783C8`. Not seen in play. Not read. |
| `sub_825788C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_822976F8`, `sub_82297790`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_82578AF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82297790`, `sub_825788C0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82578C88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82578CB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82578CD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_822962B0`, `sub_8257C160`. Not seen in play. Not read. |
| `sub_82578D70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82578CD0`. Not seen in play. Not read. |
| `sub_82578F00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_822962B0`, `sub_8245A050`, `sub_8245A168`, `sub_8245A208`, `sub_82578CD0`, `sub_8257C570`, `sub_8257D020`. Not seen in play. Not read. |
| `sub_825790F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82578D70`, `sub_82578F00`. Not seen in play. Not read. |
| `sub_82579108` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8245A1B0`, `sub_8257B9D0`, `sub_8257D6F8`. Not seen in play. Not read. |
| `sub_825791B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82578CD0`, `sub_8257C858`, `sub_8257D518`. Not seen in play. Not read. |
| `sub_82579280` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82297790`, `sub_82459E18`, `sub_8257C710`, `sub_8257CFA8`. Not seen in play. Not read. |
| `sub_82579318` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_82579448` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_822962B0`, `sub_82459EF0`, `sub_8245A768`. Not seen in play. Not read. |
| `sub_82579590` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8257D298`. Not seen in play. Not read. |
| `sub_82579630` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82579108`, `sub_8257B9A8`, `sub_8257D460`, `sub_8257D6D8`. Not seen in play. Not read. |
| `sub_825796C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_822976F8`, `sub_82579280`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82579798` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825796C0`. Not seen in play. Not read. |
| `sub_825797B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_825797C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_822962B0`, `sub_8257C160`. Not seen in play. Not read. |
| `sub_82579830` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8257B9D0`, `sub_8257EFF8`. Not seen in play. Not read. |
| `sub_82579880` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82579898` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_822962B0`, `sub_8257B9A8`, `sub_8257EF80`, `sub_8257F070`, `sub_8257F1E8`. Not seen in play. Not read. |
| `sub_82579968` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825797C0`, `sub_8257F198`. Not seen in play. Not read. |
| `sub_82579A88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_822962B0`, `sub_8257A3D0`, `sub_8257C160`, `sub_8257C858`, `sub_8257F070`, `sub_8257F1E8`, `sub_8257F388`. Not seen in play. Not read. |
| `sub_82579BE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82297790`, `sub_8257A3F0`, `sub_8257C710`, `sub_8257EDE8`. Not seen in play. Not read. |
| `sub_82579C48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_822976F8`, `sub_82579BE8`. Not seen in play. Not read. |
| `sub_82579CD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82579C48`. Not seen in play. Not read. |
| `sub_82579CF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82579D00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82579D40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82579D60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82579D88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_82579DA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8257A3D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_8257A3F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825AA840`. Not seen in play. Not read. |
| `sub_8257B9A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_8257B9D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_8257B9F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8257C570`. Not seen in play. Not read. |
| `sub_8257BA88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_822976F8`. Not seen in play. Not read. |
| `sub_8257BAA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_822962B0`. Not seen in play. Not read. |
| `sub_8257BAD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_8257BB00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8257FBF8`. Not seen in play. Not read. |
| `sub_8257BB28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82580020`. Not seen in play. Not read. |
| `sub_8257BB50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82297790`. Not seen in play. Not read. |
| `sub_8257BB60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_82580178`, `sub_82580188`, `sub_82580198`, `sub_825801A8`, `sub_825801B8`, `sub_825801C8`. Not seen in play. Not read. |
| `sub_8257BBC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8257BB60`, `sub_8257FBF8`, `sub_8257FD18`. Not seen in play. Not read. |
| `sub_8257BC48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8257C590`, `sub_8257F5E8`, `sub_8257FA90`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_8257BE48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8257BB60`, `sub_82580020`, `sub_82580170`. Not seen in play. Not read. |
| `sub_8257BEC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8257C590`, `sub_8257F5E8`, `sub_8257FEB8`, `sub_8257FF50`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_8257C160` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8257C610`. Not seen in play. Not read. |
| `sub_8257C2B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8257C590`. Not seen in play. Not read. |
| `sub_8257C570` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_8257C590` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_8257C610` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_822962B0`. Not seen in play. Not read. |
| `sub_8257C948` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_825BA0C8`. Not seen in play. Not read. |
| `sub_8257C9A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8257C948`. Not seen in play. Not read. |
| `sub_8257CA10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_8257CB38` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_8257CC08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls nothing. Not seen in play. Not read. |
| `sub_8257CCD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_018.cpp`, `--native_lifted_018`). Calls `sub_8257C570`. Not seen in play. Not read. |
| `sub_8257CF10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_8257C160`. Not seen in play. Not read. |
| `sub_8257CFA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82297790`, `sub_8257C710`. Not seen in play. Not read. |
| `sub_8257D020` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_822962B0`, `sub_8257C9A0`, `sub_8257CF10`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_8257D298` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_822956F0`, `sub_8257C948`, `sub_8257CA10`, `sub_8257CB38`, `sub_8257CC08`, `sub_8257CCD8`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_8257D460` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_8257D518` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82295F18`, `sub_822976F8`, `sub_8257C858`, `sub_8257CFA8`, `sub_825B9A70`. Not seen in play. Not read. |
| `sub_8257D6D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_8257D6F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls nothing. Not seen in play. Not read. |
| `sub_8257E8E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82459F48`. Not seen in play. Not read. |
| `sub_8257E9B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82459DB0`, `sub_82459E18`, `sub_82459EF0`. Not seen in play. Not read. |
| `sub_8257EAF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8257EF80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_8257EAF0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8257EFF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_8257EAF0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8257F070` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_822962B0`, `sub_8257E8E8`, `sub_8257E9B8`, `sub_8257EB68`. Not seen in play. Not read. |
| `sub_8257F198` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82579590`, `sub_8257EAF0`. Not seen in play. Not read. |
| `sub_8257F1E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_8257E8E8`. Not seen in play. Not read. |
| `sub_8257F5E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8257F970` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_825820E8`, `sub_82582158`, `sub_825828D0`. Not seen in play. Not read. |
| `sub_8257FA90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_8257F970`, `sub_825BA0C8`. Not seen in play. Not read. |
| `sub_8257FB38` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82582320`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8257FBF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_8257FB38`. Not seen in play. Not read. |
| `sub_8257FC00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_8257FB38`, `sub_825821F0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8257FD18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_8257FC00`. Not seen in play. Not read. |
| `sub_8257FD48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82582CC8`. Not seen in play. Not read. |
| `sub_8257FEB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_8257FD48`, `sub_825BA0C8`. Not seen in play. Not read. |
| `sub_8257FF50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_8257FF68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82583090`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82580020` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_8257FF68`. Not seen in play. Not read. |
| `sub_82580028` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_8257FF68`, `sub_82582EC0`, `sub_825B9A70`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82580170` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82580028`. Not seen in play. Not read. |
| `sub_82580178` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82580188` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82580198` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825801A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825801B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825801C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825820E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_825834F8`. Not seen in play. Not read. |
| `sub_82582158` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82583910`, `sub_82583BA8`, `sub_82583C00`, `sub_82583C98`, `sub_82583CE8`, `sub_82583D68`. Not seen in play. Not read. |
| `sub_825821F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82583FD0`, `sub_825840D0`, `sub_82584988`, `sub_82584A00`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82582320` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82584080`, `sub_82584178`, `sub_825876D8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825823B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82584758`, `sub_82584B90`, `sub_82584CF0`, `sub_82584E48`. Not seen in play. Not read. |
| `sub_82582498` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82585398`. Not seen in play. Not read. |
| `sub_82582658` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82582498`, `sub_82584B90`, `sub_82584EB0`, `sub_825850E0`, `sub_825854E0`, `sub_825855B0`, `sub_82585948`, `sub_82585B28`, .... Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825828D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_825823B0`, `sub_82582658`, `sub_82583DC8`, `sub_82584A70`, `sub_82584B90`, `sub_82584CF0`, `sub_82584EB0`, `sub_825850E0`, .... Not seen in play. Not read. |
| `sub_82582AD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_825868E8`, `sub_825ABE30`. Not seen in play. Not read. |
| `sub_82582CC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82582AD8`, `sub_82586B68`, `sub_82586C10`, `sub_82587088`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82582EC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82587658`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82583090` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_825876D8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82583180` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82584CB0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825834F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825835F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82583610` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82583688` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_82583610`. Not seen in play. Not read. |
| `sub_82583910` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_82583610`, `sub_82583688`. Not seen in play. Not read. |
| `sub_82583BA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82583C00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_825A9C00`. Not seen in play. Not read. |
| `sub_82583C98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_825ABE30`. Not seen in play. Not read. |
| `sub_82583CE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82583D68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_825835F8`. Not seen in play. Not read. |
| `sub_82583DC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82584B90`. Not seen in play. Not read. |
| `sub_82583FD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82584080` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825840D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82584178` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825841E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82584270` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82584C70`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82584448` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82584CB0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825845A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82584270`, `sub_82584448`, `sub_82585D78`, `sub_82586218`, `sub_82586428`, `sub_825AA0B8`. Not seen in play. Not read. |
| `sub_82584758` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_825841E8`, `sub_825845A8`, `sub_82584E78`, `sub_82584ED8`, `sub_82587728`, `sub_82587918`, `sub_82587BC8`. Not seen in play. Not read. |
| `sub_82584970` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82584988` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82584A00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82584A70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82584B90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82584C70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82587F98`, `sub_825881E0`. Not seen in play. Not read. |
| `sub_82584CB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_825880B8`, `sub_825881E0`. Not seen in play. Not read. |
| `sub_82584CF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82587F98`, `sub_825881E0`. Not seen in play. Not read. |
| `sub_82584E48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82584E78` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82584EB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82584ED8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825850E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825852F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82585398` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825853F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825854E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825855B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82584ED8`, `sub_825853F8`, `sub_82586460`, `sub_825884F8`, `sub_825A9D80`. Not seen in play. Not read. |
| `sub_825858A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82585948` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82584E48`, `sub_82586288`. Not seen in play. Not read. |
| `sub_82585B28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82584CB0`, `sub_82585398`, `sub_82585D78`, `sub_82588750`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82585D30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_825A80A8`. Not seen in play. Not read. |
| `sub_82585D78` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82585EB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82586048` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82585EB8`, `sub_825A7830`. Not seen in play. Not read. |
| `sub_82586218` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82586288` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825862D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82584E78`, `sub_82586218`. Not seen in play. Not read. |
| `sub_82586428` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82586460` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825865E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_820B6708`, `sub_825887C0`, `sub_82588908`, `sub_82588A18`, `sub_82588A70`, `sub_82588AE0`, `sub_82588B68`, `sub_82588D20`, .... Not seen in play. Not read. |
| `sub_82586758` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82584B90`, `sub_825850E0`, `sub_825852F0`, `sub_825858A0`, `sub_82585948`. Not seen in play. Not read. |
| `sub_825868E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82586B68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82587238`. Not seen in play. Not read. |
| `sub_82586C10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82584C70`, `sub_82588E30`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82587088` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82587188` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82587238` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_82583180`, `sub_82585398`, `sub_82587188`, `sub_82588FF0`. Not seen in play. Not read. |
| `sub_82587658` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825876D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_82587728` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82587918` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82587960` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82584ED8`. Not seen in play. Not read. |
| `sub_82587A88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82584ED8`. Not seen in play. Not read. |
| `sub_82587BC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls `sub_82584E48`, `sub_82587960`, `sub_82587A88`, `sub_82589158`. Not seen in play. Not read. |
| `sub_82587F98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825880B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825881E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825884F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82584970`, `sub_825892B8`. Not seen in play. Not read. |
| `sub_82588750` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_825887C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_825A8ED0`. Not seen in play. Not read. |
| `sub_82588908` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82588A18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82588A70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82588AE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82588B68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_019.cpp`, `--native_lifted_019`). Calls nothing. Not seen in play. Not read. |
| `sub_82588D20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82588DF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls nothing. Not seen in play. Not read. |
| `sub_82588E30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_82584ED8`, `sub_82585398`. Not seen in play. Not read. |
| `sub_82588FF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82589158` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_825892B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_82589480` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82589650` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_8258E410`. Not seen in play. Not read. |
| `sub_82589AB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_8258E580`, `sub_8258E5B8`, `sub_825A1318`. Not seen in play. Not read. |
| `sub_8258AB18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_82589AB0`. Not seen in play. Not read. |
| `sub_8258AC58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8258ADA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8258AF80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8258B1C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8258B368` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8258BCD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_8258AC58`, `sub_8258ADA0`, `sub_8258AF80`, `sub_8258B1C0`, `sub_8258B368`. Not seen in play. Not read. |
| `sub_8258BE58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls nothing. Not seen in play. Not read. |
| `sub_8258BFA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_8258CA50`, `sub_8258CAA0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8258C060` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_82597148`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8258C0B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_8258C9D0`, `sub_8258CAA0`, `sub_8258CB08`, `sub_82596E88`. Not seen in play. Not read. |
| `sub_8258C3A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_8258BE58`, `sub_8258BFA0`, `sub_8258C0B8`, `sub_8258E578`, `sub_825A6D28`, `sub_825A71B0`. Not seen in play. Not read. |
| `sub_8258CCC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825A71C0`. Not seen in play. Not read. |
| `sub_8258CE00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825A70E8`, `sub_825A71C0`. Not seen in play. Not read. |
| `sub_8258CEA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_82592178`. Not seen in play. Not read. |
| `sub_8258CF40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_82592178`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8258CFA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825B9A70`. Not seen in play. Not read. |
| `sub_8258D020` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls nothing. Not seen in play. Not read. |
| `sub_8258D030` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825BA0C8`. Not seen in play. Not read. |
| `sub_8258D090` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825BA0C8`. Not seen in play. Not read. |
| `sub_8258D0D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825BA0C8`. Not seen in play. Not read. |
| `sub_8258D170` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825BA0C8`. Not seen in play. Not read. |
| `sub_8258D1E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825BA0C8`. Not seen in play. Not read. |
| `sub_8258D248` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls nothing. Not seen in play. Not read. |
| `sub_8258D290` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825BA0C8`. Not seen in play. Not read. |
| `sub_8258D318` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825BA0C8`. Not seen in play. Not read. |
| `sub_8258D3A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825BA0C8`. Not seen in play. Not read. |
| `sub_8258D440` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls nothing. Not seen in play. Not read. |
| `sub_8258D448` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8258D490` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_82592638`. Not seen in play. Not read. |
| `sub_8258D4A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls nothing. Not seen in play. Not read. |
| `sub_8258D4C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls nothing. Not seen in play. Not read. |
| `sub_8258D508` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_82592638`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8258DBA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_8229D978`, `sub_8229D9D0`, `sub_822A4060`, `sub_822A4340`, `sub_822A67B0`, `sub_8258D9F8`, `sub_82592670`. Not seen in play. Not read. |
| `sub_8258DEB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_8258E530`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8258E530` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls nothing. Not seen in play. Not read. |
| `sub_8258E540` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_82295500`. Not seen in play. Not read. |
| `sub_82591898` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_825918E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls nothing. Not seen in play. Not read. |
| `sub_82591BB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_82106540`. Not seen in play. Not read. |
| `sub_82592058` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_82591BB8`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_825920A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls nothing. Not seen in play. Not read. |
| `sub_82592178` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls nothing. Not seen in play. Not read. |
| `sub_82592400` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825A71D0`. Not seen in play. Not read. |
| `sub_825925A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82592900` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls `sub_825A6CC0`, `sub_825A6D08`. Not seen in play. Not read. |
| `sub_82594190` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_031.cpp`, `--native_lifted_031`). Calls `sub_825940B0`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82594290` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_020.cpp`, `--native_lifted_020`). Calls nothing. Not seen in play. Not read. |
| `sub_82594440` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_82590D68`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_825946B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_82594570`. Not seen in play. Not read. |
| `sub_825968D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_82594F58`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_82599BE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_8259A9D0`, `sub_825A70E8`, `sub_825A71C0`. Not seen in play. Not read. |
| `sub_8259A2B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_820A0AC0`. Not seen in play. Not read. |
| `sub_8259A400` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8259ACE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_8209BD50`, `sub_820A2D30`, `sub_820A65F0`, `sub_820A67F8`. Not seen in play. Not read. |
| `sub_8259AF20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_8209BD50`, `sub_820A2D30`, `sub_820A65F0`, `sub_820A67F8`. Not seen in play. Not read. |
| `sub_8259B188` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_820A65F0`, `sub_820A67F8`, `sub_825A8770`. Not seen in play. Not read. |
| `sub_8259B600` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_8259A228`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8259B660` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_8259B188`. Not seen in play. Not read. |
| `sub_8259BB98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_820A0AC0`, `sub_8259A2B0`, `sub_8259A400`, `sub_8259B660`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8259C550` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_8259C6C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_8259C550`. Not seen in play. Not read. |
| `sub_8259C6F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_8259C700` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8259C770` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8259C7F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_8259C8C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8259C910` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_8259D160` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_820988B8`, `sub_82106540`, `sub_82481880`. Not seen in play. Not read. |
| `sub_8259D1C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8259D370` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A71C0`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_8259FAA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls nothing. Not seen in play. Not read. |
| `sub_8259FD40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_030.cpp`, `--native_lifted_030`). Calls `sub_825A8A50`. Not seen in play. Not read. |
| `sub_825A13C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_825A13D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_825A1610` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_822A67B0`. Not seen in play. Not read. |
| `sub_825A1810` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_822A8960`, `sub_825A7410`, `sub_825BDDA0`, `sub_825BDDC0`. Not seen in play. Not read. |
| `sub_825A2238` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_825A2250` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_8245ED60`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_825A22E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_8245ED60`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_825A2370` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_825A2388` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_825A23A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_825A2CD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_825A3108` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_825A31F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_825A3380` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_825A3408` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_825A3980` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_825A3B80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A88A0`, `sub_825A8978`. Not seen in play. Not read. |
| `sub_825A3E18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A88A0`, `sub_825A8978`. Not seen in play. Not read. |
| `sub_825A4600` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A2CD8`, `sub_825A31F0`, `sub_825A3408`, `sub_825A3980`. Not seen in play. Not read. |
| `sub_825A4680` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A88A0`, `sub_825A8978`. Not seen in play. Not read. |
| `sub_825A5020` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A8978`. Not seen in play. Not read. |
| `sub_825A5150` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A2CD8`, `sub_825A31F0`, `sub_825A3408`, `sub_825A3980`, `sub_825A3E18`, `sub_825A4680`. Not seen in play. Not read. |
| `sub_825A5320` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A3E18`, `sub_825A4680`. Not seen in play. Not read. |
| `sub_825A5408` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A3E18`, `sub_825A5150`, `sub_825A5320`. Not seen in play. Not read. |
| `sub_825A54C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A23A0`, `sub_825A2CD8`, `sub_825A3108`, `sub_825A31F0`, `sub_825A3380`, `sub_825A3E18`, `sub_825A4600`, `sub_825A5150`, .... Not seen in play. Not read. |
| `sub_825A5790` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_8245ED60`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_825A5818` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_825A5838` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_8245F718`. Not seen in play. Not read. |
| `sub_825A5BF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A8ED0`. Not seen in play. Not read. |
| `sub_825A5CC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A5BF0`, `sub_825A8ED0`. Not seen in play. Not read. |
| `sub_825A5D48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_8245ED60`, `sub_825A5BF0`, `sub_825A7410`. Not seen in play. Not read. |
| `sub_825A5E28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_8245ED60`. Not seen in play. Not read. |
| `sub_825A6070` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_8245ED60`. Not seen in play. Not read. |
| `sub_825A6110` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls nothing. Not seen in play. Not read. |
| `sub_825A6B18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_82091968`, `sub_825931D0`, `sub_825AD198`. Not seen in play. Not read. |
| `sub_825A6BD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_033.cpp`, `--native_lifted_033`). Calls `sub_825A6D20`. Not seen in play. Not read. |
| `sub_825A6C40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_033.cpp`, `--native_lifted_033`). Calls `sub_82106540`. Not seen in play. Not read. |
| `sub_825A6CC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825A6D28`. Not seen in play. Not read. |
| `sub_825A6D08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_8258E578`. Not seen in play. Not read. |
| `sub_825A6D20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Imports `RtlInitializeCriticalSection`. Not seen in play. Not read. |
| `sub_825A6D30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825BF650` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825D8F18`. Not seen in play. Not read. |
| `sub_825BF6B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825D8F18`. Not seen in play. Not read. |
| `sub_825BF720` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825D8F18`. Not seen in play. Not read. |
| `sub_825BF7A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls `sub_825BF720`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825BFCC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls nothing. Not seen in play. Not read. |
| `sub_825BFDF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_021.cpp`, `--native_lifted_021`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C0470` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C4580`. Not seen in play. Not read. |
| `sub_825C05C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C0698` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C0730` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C39F8`. Not seen in play. Not read. |
| `sub_825C07F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C0848` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C0958` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_82297790`. Not seen in play. Not read. |
| `sub_825C0B48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C1D90`. Not seen in play. Not read. |
| `sub_825C0B50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C0848`. Not seen in play. Not read. |
| `sub_825C0B58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C07F0`. Not seen in play. Not read. |
| `sub_825C0D90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_822976F8`, `sub_825BFEA0`, `sub_825C8698`, `sub_825C8808`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C0E88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_822976F8`, `sub_825BFEA0`, `sub_825C8C98`, `sub_825C8E68`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C0F90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825BFEA0`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C11A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C0698`, `sub_825C39F8`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C1238` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825BFEA0`, `sub_825C0698`, `sub_825C1F88`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C12F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C0698`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C1548` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls nothing. Not seen in play. Not read. |
| `sub_825C15E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C05C8`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C1738` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls nothing. Not seen in play. Not read. |
| `sub_825C1788` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C3548`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C1800` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825BFEA0`, `sub_825C1568`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C1BC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825BFEA0`, `sub_825C1788`, `sub_825C3548`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C1D00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C1800`. Not seen in play. Not read. |
| `sub_825C1D90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825A70E8`, `sub_825C1D00`. Not seen in play. Not read. |
| `sub_825C2308` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C2368` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C25B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C2650` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_82297790`, `sub_825A70E8`, `sub_825C37A8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C2B50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_82297790`, `sub_825C2650`. Not seen in play. Not read. |
| `sub_825C2D90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_8245A4E8`, `sub_8245A668`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C2FA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C2C60`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C3040` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls nothing. Not seen in play. Not read. |
| `sub_825C3050` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C1F30`. Not seen in play. Not read. |
| `sub_825C34B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_82297790`, `sub_825C3050`. Not seen in play. Not read. |
| `sub_825C37A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_82297790`, `sub_825A70E8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C39F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825BFC50`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C40E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_82297790`, `sub_825C37A8`. Not seen in play. Not read. |
| `sub_825C4580` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C1738`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C4940` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C4A58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C4A78` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls nothing. Not seen in play. Not read. |
| `sub_825C4FD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825CA808`. Not seen in play. Not read. |
| `sub_825C4FE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825A7410`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C5060` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_822976F8`, `sub_82297790`, `sub_825A7410`, `sub_825CA900`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C5128` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825CA940`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C5238` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_822976F8`, `sub_82297790`, `sub_825A7410`, `sub_825BFEA0`, `sub_825CA980`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C5350` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C53F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825CA9C0`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C54A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_822976F8`, `sub_82297790`, `sub_825A7410`, `sub_825BFEA0`, `sub_825CAA00`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C56D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C0730`, `sub_825C0748`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C5DA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C5C80`, `sub_825C6678`. Not seen in play. Not read. |
| `sub_825C5F88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C6560`. Not seen in play. Not read. |
| `sub_825C5F90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C6008` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C6020` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C6038` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls nothing. Not seen in play. Not read. |
| `sub_825C6080` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825CA860`, `sub_825CC0E0`. Not seen in play. Not read. |
| `sub_825C63D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825BFEA0`, `sub_825C1F88`, `sub_825C5960`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C6498` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825BFEA0`, `sub_825C5960`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C6560` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_82297790`, `sub_825C6148`. Not seen in play. Not read. |
| `sub_825C65B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls nothing. Not seen in play. Not read. |
| `sub_825C65F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls nothing. Not seen in play. Not read. |
| `sub_825C6678` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825A71D0`, `sub_825BF6B8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C6858` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C5E58`. Not seen in play. Not read. |
| `sub_825C68C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_022.cpp`, `--native_lifted_022`). Calls `sub_825C0470`. Not seen in play. Not read. |
| `sub_825C68D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C6918` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C6A10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825C7608`. Not seen in play. Not read. |
| `sub_825C6DF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825BFEA0`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C7018` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825C15E8`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825C70C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825BFC50`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C7608` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_82297790`, `sub_825C6148`. Not seen in play. Not read. |
| `sub_825C7750` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825C7960`. Not seen in play. Not read. |
| `sub_825C7960` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_82297790`, `sub_825C7860`. Not seen in play. Not read. |
| `sub_825C7A38` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825C65B8`, `sub_825C65F8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C7BD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C7C30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C7CD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C7D28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C7D40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C83B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C83C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C85C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C85D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C8698` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825CF9B0`. Not seen in play. Not read. |
| `sub_825C8708` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825C8C40`. Not seen in play. Not read. |
| `sub_825C8798` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C87A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C87B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C87C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C87D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C87E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C87F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C8808` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_822976F8`, `sub_825CF498`. Not seen in play. Not read. |
| `sub_825C88A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_822976F8`, `sub_82297790`, `sub_825C8710`, `sub_825CC610`, `sub_825CF498`, `sub_825CF4D8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C8A18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825CF498`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C8A98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C8AC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C8B28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C8B48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825C8B80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_82297790`, `sub_825CF308`, `sub_825CFF90`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C8C40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_82297790`, `sub_825C8B80`. Not seen in play. Not read. |
| `sub_825C8C98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825C6928`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C8E18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_82297790`. Not seen in play. Not read. |
| `sub_825C8E68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825A7410`. Not seen in play. Not read. |
| `sub_825C93A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825BFE28`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825C9558` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825C93A8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CA6F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825D1830`. Not seen in play. Not read. |
| `sub_825CA808` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825D03B0`. Not seen in play. Not read. |
| `sub_825CA900` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825D0510`. Not seen in play. Not read. |
| `sub_825CA940` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825D0528`. Not seen in play. Not read. |
| `sub_825CA9C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825D0558`. Not seen in play. Not read. |
| `sub_825CAA00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825D0570`. Not seen in play. Not read. |
| `sub_825CAC58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CACB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CAD18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CADC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CADF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CAE08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CB188` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825CB348` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825CAE08`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CB660` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825CB6A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825CB708` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825A71D0`, `sub_825BF6B8`, `sub_825BFE28`. Not seen in play. Not read. |
| `sub_825CBAD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_82297790`, `sub_825CAE90`. Not seen in play. Not read. |
| `sub_825CC090` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls `sub_825CBC58`. Not seen in play. Not read. |
| `sub_825CC0E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_023.cpp`, `--native_lifted_023`). Calls nothing. Not seen in play. Not read. |
| `sub_825CC3F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825CC6C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CC7B8`. Not seen in play. Not read. |
| `sub_825CC7B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_82297790`, `sub_825CC6D0`. Not seen in play. Not read. |
| `sub_825CC8F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls nothing. Not seen in play. Not read. |
| `sub_825CC900` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CC948` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls nothing. Not seen in play. Not read. |
| `sub_825CCC88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls nothing. Not seen in play. Not read. |
| `sub_825CCC98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CCCE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls nothing. Not seen in play. Not read. |
| `sub_825CCD60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CD020` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_822976F8`, `sub_825C5C80`, `sub_825D3360`, `sub_825D3618`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CD150` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CD180` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls nothing. Not seen in play. Not read. |
| `sub_825CD198` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825BFE28`. Not seen in play. Not read. |
| `sub_825CD210` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CD198`. Not seen in play. Not read. |
| `sub_825CD270` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C5E58`. Not seen in play. Not read. |
| `sub_825CD350` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CE770`. Not seen in play. Not read. |
| `sub_825CD358` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls nothing. Not seen in play. Not read. |
| `sub_825CD378` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CD388` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls nothing. Not seen in play. Not read. |
| `sub_825CD398` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CAC58`, `sub_825CD210`. Not seen in play. Not read. |
| `sub_825CD510` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CD198`. Not seen in play. Not read. |
| `sub_825CD620` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CADC0`, `sub_825CD198`. Not seen in play. Not read. |
| `sub_825CD668` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CD198`. Not seen in play. Not read. |
| `sub_825CD6A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CACB8`, `sub_825CAD18`, `sub_825CADC0`, `sub_825CB660`, `sub_825CB708`, `sub_825CD210`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CD858` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825BFE28`, `sub_825C4CF8`, `sub_825CA6F8`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825CD990` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C5238`, `sub_825CA980`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825CDA40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C54A0`, `sub_825CAA00`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825CDB10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C0698`, `sub_825C5960`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CDBD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C5060`, `sub_825CA900`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825CDC50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C5128`, `sub_825CA940`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825CDCC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825D3338`. Not seen in play. Not read. |
| `sub_825CDCE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C2C60`. Not seen in play. Not read. |
| `sub_825CDD30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C2C60`, `sub_825C6148`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CDEA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825BFE28`, `sub_825C4E88`, `sub_825CBB28`, `sub_825CBD10`, `sub_825CD398`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CE2F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_822976F8`, `sub_825C4A98`, `sub_825CDB10`, `sub_825D3360`, `sub_825D3688`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CE450` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C5C80`, `sub_825C5DA0`. Not seen in play. Not read. |
| `sub_825CE5D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C4A78`, `sub_825CBC58`, `sub_825CDCE0`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825CE718` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C60C0`. Not seen in play. Not read. |
| `sub_825CE770` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_82297790`, `sub_825CDD30`. Not seen in play. Not read. |
| `sub_825CE7C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C5C80`, `sub_825CD6A8`, `sub_825CE450`, `sub_825D2D68`, `sub_825D2EA0`, `sub_825D3778`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Not seen in play. Not read. |
| `sub_825CE968` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825BFE28`, `sub_825CE718`. Imports `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CEA38` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825C4F00`, `sub_825CB040`, `sub_825CE7C8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CEC60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CEC68`. Not seen in play. Not read. |
| `sub_825CEC68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_82297790`, `sub_825CC6D0`. Not seen in play. Not read. |
| `sub_825CEE50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls nothing. Not seen in play. Not read. |
| `sub_825CEE60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls nothing. Not seen in play. Not read. |
| `sub_825CEEE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls nothing. Not seen in play. Not read. |
| `sub_825CF150` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CEEE8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CF220` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CF080`. Not seen in play. Not read. |
| `sub_825CF308` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CC6D0`. Not seen in play. Not read. |
| `sub_825CF340` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CFA50`. Not seen in play. Not read. |
| `sub_825CF348` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_822976F8`, `sub_825D3F28`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CF410` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_82295F18`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CF498` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825CF410`. Not seen in play. Not read. |
| `sub_825CF4D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CF528` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CF598` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_82099BA0`, `sub_822976F8`, `sub_825CED88`, `sub_825CED98`, `sub_825CEE28`, `sub_825CEE50`, `sub_825CF210`, `sub_825D50F8`, .... Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CF8A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825D5088`, `sub_825D50E8`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CF910` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825D0020`. Not seen in play. Not read. |
| `sub_825CF918` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CF9B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825A71D0`, `sub_825CC620`. Not seen in play. Not read. |
| `sub_825CFA50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_82297790`, `sub_825CC6D0`. Not seen in play. Not read. |
| `sub_825CFAE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CFB28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_820A8958`, `sub_822976F8`, `sub_825CED88`, `sub_825CED98`, `sub_825CEE28`, `sub_825CEE50`, `sub_825CEEE8`, `sub_825CF150`, .... Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825CFF70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls nothing. Not seen in play. Not read. |
| `sub_825CFF90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_024.cpp`, `--native_lifted_024`). Calls `sub_825D5088`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825D8F18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9140` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9150` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D92E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9310` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_82593908`, `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9388` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9398` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D93A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D93E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D93F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9408` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9418` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9428` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9438` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9448` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9458` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9468` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9478` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9488` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9498` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D94A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D94B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D94C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls nothing. Not seen in play. Not read. |
| `sub_825D94F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9668` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9678` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_8211B0E8`, `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D96C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls nothing. Not seen in play. Not read. |
| `sub_825D96E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9710` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9720` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9730` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9740` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9750` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9760` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9770` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9780` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9790` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_025.cpp`, `--native_lifted_025`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D97A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D97B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D97C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D97E8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9810` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9828` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9840` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9850` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9860` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9888` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D98B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D98D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9900` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9928` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9950` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9978` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D99A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9A10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9A20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9A30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9A40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9A50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9A60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9A70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9A80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9A90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9AA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9AB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9AC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9AD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9AE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9AF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9B00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9B10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9B20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9B30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9B40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9B50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_8214E608`, `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9B88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9B98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_8214F660`, `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9BD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_8214AA18`, `sub_825A7068`, `sub_825A71D0`. Not seen in play. Not read. |
| `sub_825D9C40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls `sub_825A71D0`. Not seen in play. Not read. |
| `sub_825D9C58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9CA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9CC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9CF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9D00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9D28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9D50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9D78` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9DA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9DC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9DF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9E18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825D9E40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9E50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9EC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9ED0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9EE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9F48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9F58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825D9FC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_820BFBB8`. Not seen in play. Not read. |
| `sub_825DA050` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_822936E0`. Not seen in play. Not read. |
| `sub_825DA0B8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Imports `RtlInitializeCriticalSection`. Not seen in play. Not read. |
| `sub_825DA128` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A71D0`. Not seen in play. Not read. |
| `sub_825DA178` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_8258E548`, `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA1B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA1C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA1D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA1E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA1F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA200` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA210` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA238` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA260` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA288` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA2C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA308` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA348` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA388` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA3C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA408` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA418` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA428` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA438` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_8258E548`, `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA4A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_8258E548`, `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA4E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A6880`, `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA5C0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DA5D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls nothing. Not seen in play. Not read. |
| `sub_825DA648` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls nothing. Not seen in play. Not read. |
| `sub_825DA750` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A8ED0`. Not seen in play. Not read. |
| `sub_825DA8F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A8A50`. Not seen in play. Not read. |
| `sub_825DAA30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A8978`. Not seen in play. Not read. |
| `sub_825DABC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A6BD0`, `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DAC00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DAC10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A6BD0`, `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DAC48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_825A7068`. Not seen in play. Not read. |
| `sub_825DACA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_026.cpp`, `--native_lifted_026`). Calls `sub_8258E530`. Not seen in play. Not read. |
| `sub_825DAD08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAD90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_000.cpp`, `--native_lifted_000`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAD98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls `sub_820900C0`. Not seen in play. Not read. |
| `sub_825DADF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DADF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAE00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAE08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAE10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAE18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAE20` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAE28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls `sub_820900C0`. Not seen in play. Not read. |
| `sub_825DAE88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAE90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAE98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAEA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAEA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAEB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAEB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAEC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAEC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAED0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAED8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAEE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAEE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAEF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAEF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF00` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF08` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF10` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF18` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls `sub_825A71C0`. Not seen in play. Not read. |
| `sub_825DAF28` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF30` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF38` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF40` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF48` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF50` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF58` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF60` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF68` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF70` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF78` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF80` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF88` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF90` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAF98` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAFA0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAFA8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAFB0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAFB8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAFC0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAFC8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAFD0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAFD8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAFE0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAFE8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAFF0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DAFF8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB000` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB008` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB010` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB018` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB020` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB028` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB030` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls `sub_8214E678`. Not seen in play. Not read. |
| `sub_825DB040` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls `sub_8214F690`. Not seen in play. Not read. |
| `sub_825DB050` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_825DB0A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls `sub_8214C930`. Not seen in play. Not read. |
| `sub_825DB0F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB0F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB100` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB108` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB110` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB118` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB120` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB128` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB130` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB138` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB140` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB148` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB150` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB158` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB160` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB168` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB188` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB190` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB198` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB1A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB1A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB1C8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB1F8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB210` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls `sub_82106540`. Not seen in play. Not read. |
| `sub_825DB220` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB228` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_027.cpp`, `--native_lifted_027`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB230` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB238` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB240` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB248` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB250` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB258` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB260` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB268` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB270` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB278` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB280` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB288` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB290` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB298` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB2A0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB2A8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB2B0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls `sub_82106540`. Not seen in play. Not read. |
| `sub_825DB2D0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls `sub_82106540`. Not seen in play. Not read. |
| `sub_825DB2E0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls `sub_820ACF88`, `sub_825A70E8`. Not seen in play. Not read. |
| `sub_825DB328` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_825DB380` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls `sub_825A70E8`. Not seen in play. Not read. |
| `sub_825DB3D8` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB3F0` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls nothing. Not seen in play. Not read. |
| `sub_825DB428` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls `sub_825A6C40`. Not seen in play. Not read. |
| `sub_825DB438` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls `sub_82592900`. Calls through a function pointer or vtable. Not seen in play. Not read. |
| `sub_825DB488` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls `sub_825A6C40`. Not seen in play. Not read. |
| `sub_825DB498` | | Lifted | Lifted by `lift.py` (2026-11-15, `src/lifted/batch_028.cpp`, `--native_lifted_028`). Calls `sub_825A6D30`. Not seen in play. Not read. |
